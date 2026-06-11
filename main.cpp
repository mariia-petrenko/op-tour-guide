// ============================================================================
// main.cpp
// Призначення: окремий тестовий додаток (інтерфейсна частина) для
// демонстрації роботи класів предметної області "Електронний Довідник
// туриста". Реалізує консольне меню, через яке користувач виконує всі
// функції довідника: додавання, вилучення, редагування, пошук, сортування,
// фільтрацію, збереження/завантаження файлу та вивід даних на екран і у файл.
//
// Введення з консолі зведено до вибору пунктів меню та простих параметрів.
// Перевіряється коректність вводу числових значень.
//
// Використовувані засоби стандартної бібліотеки:
//   <iostream> - консольне введення/виведення;
//   <string>   - std::string; <limits> - очищення буфера вводу.
// ============================================================================
#include "TourGuide.h"
#include "ResortTour.h"
#include "CruiseTour.h"
#include <iostream>
#include <string>
#include <limits>
#include <cctype>

using namespace std;

// Ім'я файлу бази даних за замовчуванням
static const string DB_FILE = "database.txt";
static const string REPORT_FILE = "report.txt";

// --- Допоміжні функції безпечного вводу ---

// Зчитати ціле число у заданому діапазоні [lo, hi].
// У разі досягнення кінця вводу (EOF) повертає lo, щоб уникнути зациклення.
static int readInt(const string& prompt, int lo, int hi) {
    int value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= lo && value <= hi) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        if (cin.eof()) return lo;  // коректне завершення при EOF
        cout << "  Помилка: введіть число від " << lo << " до " << hi << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Зчитати дійсне число (>= 0). При EOF повертає 0.
static double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        if (cin >> value && value >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
        if (cin.eof()) return 0.0;  // коректне завершення при EOF
        cout << "  Помилка: введіть невід'ємне число.\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Зчитати рядок (цілий, з пробілами)
static string readLine(const string& prompt) {
    cout << prompt;
    string s;
    getline(cin, s);
    return s;
}

// Зчитати та перевірити номер телефону.
// Дозволені лише цифри та символи «+», «-», «(», «)» і пробіл;
// літери та інші символи не приймаються. Потрібно щонайменше 7 цифр.
static string readPhone(const string& prompt) {
    while (true) {
        string s = readLine(prompt);
        int digits = 0;
        bool valid = !s.empty();
        for (char c : s) {
            if (isdigit(static_cast<unsigned char>(c)))
                ++digits;
            else if (c != '+' && c != '-' && c != '(' && c != ')' && c != ' ')
                valid = false;
        }
        if (valid && digits >= 7)
            return s;
        cout << "  Помилка: телефон може містити лише цифри та символи "
                "+ - ( ) і пробіл (щонайменше 7 цифр).\n";
    }
}

// Зчитати відповідь так/ні
static bool readYesNo(const string& prompt) {
    cout << prompt << " (1 - так, 0 - ні): ";
    int v;
    if (cin >> v) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return v != 0;
    }
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    return false;
}

// Вивести перелік усіх турагенцій довідника
static void printAll(const TourGuide& guide) {
    const auto& agencies = guide.getAgencies();
    if (agencies.empty()) {
        cout << "  Довідник порожній.\n";
        return;
    }
    cout << "\n=== Перелік турагенцій (" << agencies.size() << ") ===\n";
    for (size_t i = 0; i < agencies.size(); ++i) {
        const TravelAgency& a = agencies[i];
        cout << (i + 1) << ") " << a.getName() << " | " << a.getAddress()
             << " | тел. " << a.getPhone()
             << " | путівок: " << a.tourCount();
        if (a.tourCount() > 0)
            cout << " | від " << a.minPrice() << " грн";
        cout << "\n";
        for (size_t j = 0; j < a.getTours().size(); ++j)
            cout << "      - " << a.getTours()[j]->toString() << "\n";
    }
    cout << "\n";
}

// Створення нової путівки (наземної або круїзу) через меню.
// Спершу запитуються поля, специфічні для обраного типу путівки,
// а потім спільні характеристики (проживання, екскурсії, сервіс, ціна).
static Tour* createTour() {
    cout << "  Тип путівки: 1 - Наземний тур, 2 - Круїз\n";
    int type = readInt("  Ваш вибір: ", 1, 2);
    string country = readLine("  Країна: ");

    // Поля, специфічні для конкретного типу путівки
    string city, route, liner;
    int nights = 0;
    Transport tr = Transport::Ship;
    if (type == 1) {
        city = readLine("  Місто/курорт: ");
        nights = readInt("  Кількість ночей: ", 1, 365);
        cout << "  Проїзд: 0-Літак, 1-Автобус, 2-Потяг, 3-Теплохід\n";
        tr = Tour::transportFromInt(readInt("  Вибір: ", 0, 3));
    } else {
        route = readLine("  Маршрут круїзу: ");
        liner = readLine("  Назва лайнера: ");
    }

    // Спільні характеристики путівки. Для круїзу проживання означає
    // категорію каюти на лайнері.
    if (type == 1)
        cout << "  Проживання: 0-2*, 1-3*, 2-4*, 3-5*, 4-Апартаменти, 5-Хостел\n";
    else
        cout << "  Категорія каюти: 0-2*, 1-3*, 2-4*, 3-5*, 4-Люкс, 5-Економ\n";
    Accommodation acc = Tour::accommodationFromInt(readInt("  Вибір: ", 0, 5));
    bool exc = readYesNo("  Екскурсійне обслуговування");
    bool host = readYesNo("  Сервіс приймаючої сторони");
    double price = readDouble("  Вартість путівки, грн: ");

    if (type == 1)
        return new ResortTour(country, city, nights, acc, tr, exc, host, price);
    else
        return new CruiseTour(country, route, liner, acc, exc, host, price);
}

// Заповнення довідника демонстраційними даними
static void seedDemoData(TourGuide& guide) {
    TravelAgency a1("Coral Travel", "м. Київ, вул. Хрещатик, 22", "+380441234567");
    a1.addTour(new ResortTour("Туреччина", "Анталія", 7,
               Accommodation::Hotel5, Transport::Plane, true, true, 32000));
    a1.addTour(new ResortTour("Єгипет", "Шарм-ель-Шейх", 10,
               Accommodation::Hotel4, Transport::Plane, true, true, 28500));

    TravelAgency a2("Join UP!", "м. Львів, пр. Свободи, 5", "+380322000111");
    a2.addTour(new ResortTour("Греція", "Крит", 7,
               Accommodation::Hotel4, Transport::Plane, false, true, 26000));
    a2.addTour(new CruiseTour("Італія", "Рим - Барселона - Марсель",
               "MSC Seaside", Accommodation::Hotel4, true, true, 45000));

    TravelAgency a3("TUI Ukraine", "м. Одеса, вул. Дерибасівська, 1", "+380487770099");
    a3.addTour(new CruiseTour("Норвегія", "Берген - фіорди - Гейрангер",
               "Costa Favolosa", Accommodation::Hotel5, true, true, 52000));
    a3.addTour(new ResortTour("Іспанія", "Барселона", 5,
               Accommodation::Hotel3, Transport::Bus, true, false, 19000));

    guide.addAgency(a1);
    guide.addAgency(a2);
    guide.addAgency(a3);
}

// Автоматичне збереження поточного стану довідника у файл бази даних.
// Викликається після кожної зміни даних (додавання, вилучення, редагування).
static void autoSave(const TourGuide& guide) {
    if (guide.saveToFile(DB_FILE))
        cout << "  (Зміни автоматично збережено у " << DB_FILE << ")\n";
}

// Головне меню програми
static void printMenu() {
    cout << "\n========= ЕЛЕКТРОННИЙ ДОВІДНИК ТУРИСТА =========\n"
         << " 1. Показати всі турагенції\n"
         << " 2. Додати турагенцію\n"
         << " 3. Додати путівку до агенції\n"
         << " 4. Вилучити турагенцію\n"
         << " 5. Редагувати назву турагенції\n"
         << " 6. Пошук за назвою агенції\n"
         << " 7. Пошук за країною туру\n"
         << " 8. Сортувати (за назвою / ціною)\n"
         << " 9. Фільтр за максимальною ціною\n"
         << "10. Зберегти у файл (БД)\n"
         << "11. Завантажити з файлу (БД)\n"
         << "12. Експорт звіту у файл\n"
         << "13. Заповнити демонстраційними даними\n"
         << " 0. Вихід\n"
         << "------------------------------------------------\n";
}

int main() {
    TourGuide guide;
    cout << "Запуск електронного довідника туриста...\n";

    // Автоматичне завантаження збережених даних з файлу при запуску
    if (guide.loadFromFile(DB_FILE) && guide.size() > 0)
        cout << "  Завантажено збережені дані: " << guide.size()
             << " турагенції.\n";

    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Оберіть пункт меню: ", 0, 13);
        switch (choice) {
            case 1:
                printAll(guide);
                break;
            case 2: {
                string name = readLine("Назва агенції: ");
                string addr = readLine("Адреса: ");
                string phone = readPhone("Телефон: ");
                guide.addAgency(TravelAgency(name, addr, phone));
                cout << "  Турагенцію додано.\n";
                autoSave(guide);
                break;
            }
            case 3: {
                if (guide.size() == 0) { cout << "  Немає агенцій.\n"; break; }
                printAll(guide);
                int num = readInt("Номер агенції: ", 1,
                                  static_cast<int>(guide.size()));
                Tour* t = createTour();
                guide.at(num - 1)->addTour(t);
                cout << "  Путівку додано.\n";
                autoSave(guide);
                break;
            }
            case 4: {
                if (guide.size() == 0) { cout << "  Немає агенцій.\n"; break; }
                int num = readInt("Номер для вилучення: ", 1,
                                  static_cast<int>(guide.size()));
                if (guide.removeAgency(num - 1)) {
                    cout << "  Вилучено.\n";
                    autoSave(guide);
                } else {
                    cout << "  Помилка.\n";
                }
                break;
            }
            case 5: {
                if (guide.size() == 0) { cout << "  Немає агенцій.\n"; break; }
                int num = readInt("Номер агенції: ", 1,
                                  static_cast<int>(guide.size()));
                string nn = readLine("Нова назва: ");
                guide.editAgencyName(num - 1, nn);
                cout << "  Назву змінено.\n";
                autoSave(guide);
                break;
            }
            case 6: {
                string part = readLine("Фрагмент назви: ");
                auto res = guide.findByName(part);
                cout << "  Знайдено: " << res.size() << "\n";
                for (size_t i : res)
                    cout << "   " << (i + 1) << ") " << guide.getAgencies()[i].getName() << "\n";
                break;
            }
            case 7: {
                string country = readLine("Країна: ");
                auto res = guide.findByCountry(country);
                cout << "  Знайдено агенцій: " << res.size() << "\n";
                for (size_t i : res)
                    cout << "   " << (i + 1) << ") " << guide.getAgencies()[i].getName() << "\n";
                break;
            }
            case 8: {
                cout << "  1 - агенції за назвою, 2 - путівки за ціною\n";
                int f = readInt("  Вибір: ", 1, 2);
                bool asc = readYesNo("  За зростанням");
                if (f == 1) {
                    guide.sortAgencies(SortField::Name, asc);
                    cout << "  Відсортовано за назвою.\n";
                    printAll(guide);
                } else {
                    auto tours = guide.toursSortedByPrice(asc);
                    cout << "\n=== Путівки за ціною (" << tours.size() << ") ===\n";
                    for (size_t i = 0; i < tours.size(); ++i)
                        cout << (i + 1) << ") " << tours[i].second->getPrice()
                             << " грн | " << tours[i].first << " | "
                             << tours[i].second->toString() << "\n";
                    cout << "\n";
                }
                break;
            }
            case 9: {
                double mx = readDouble("Максимальна ціна, грн: ");
                auto res = guide.filterByMaxPrice(mx);
                cout << "  Підходить агенцій: " << res.size() << "\n";
                for (const auto& a : res)
                    cout << "   - " << a.getName()
                         << " (від " << a.minPrice() << " грн)\n";
                break;
            }
            case 10:
                cout << (guide.saveToFile(DB_FILE)
                         ? "  Збережено у " + DB_FILE + "\n"
                         : "  Помилка збереження.\n");
                break;
            case 11:
                cout << (guide.loadFromFile(DB_FILE)
                         ? "  Завантажено з " + DB_FILE + "\n"
                         : "  Помилка завантаження.\n");
                break;
            case 12:
                cout << (guide.exportReport(REPORT_FILE)
                         ? "  Звіт збережено у " + REPORT_FILE + "\n"
                         : "  Помилка експорту.\n");
                break;
            case 13: {
                seedDemoData(guide);
                size_t toursTotal = 0;
                for (const auto& a : guide.getAgencies())
                    toursTotal += a.tourCount();
                cout << "  Демонстраційні дані успішно завантажено: "
                     << guide.size() << " турагенції, "
                     << toursTotal << " путівок.\n";
                autoSave(guide);
                break;
            }
            case 0:
                running = false;
                cout << "Завершення роботи.\n";
                break;
        }
    }
    return 0;
}
