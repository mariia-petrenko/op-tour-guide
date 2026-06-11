// ============================================================================
// Tour.h
// Призначення: оголошення абстрактного базового класу Tour (путівка/тур).
// Описує загальні характеристики будь-якої путівки, що пропонує турагенція:
// країну, умови проживання, умови проїзду, екскурсійне обслуговування,
// сервіс приймаючої сторони та вартість путівки.
// Використовувані засоби стандартної бібліотеки:
//   <string>  - клас std::string для роботи з текстовими полями.
// ============================================================================
#ifndef TOUR_H
#define TOUR_H

#include <string>

// Тип проживання туриста
enum class Accommodation {
    Hotel2,     // готель 2*
    Hotel3,     // готель 3*
    Hotel4,     // готель 4*
    Hotel5,     // готель 5*
    Apartment,  // апартаменти
    Hostel      // хостел
};

// Тип проїзду (транспорт)
enum class Transport {
    Plane,  // літак
    Bus,    // автобус
    Train,  // потяг
    Ship    // теплохід/лайнер
};

// ----------------------------------------------------------------------------
// Клас Tour - абстрактний базовий клас путівки.
// Є базовим для класів ResortTour (наземний тур) та CruiseTour (круїз),
// демонструючи відношення УСПАДКУВАННЯ та ПОЛІМОРФІЗМ.
// ----------------------------------------------------------------------------
class Tour {
protected:
    std::string country;          // країна призначення
    Accommodation accommodation;  // умови проживання
    Transport transport;          // умови проїзду
    bool hasExcursions;           // наявність екскурсійного обслуговування
    bool hostService;             // сервіс приймаючої сторони (трансфер, гід)
    double price;                 // вартість путівки, грн

public:
    // Конструктор за замовчуванням
    Tour();
    // Параметризований конструктор
    Tour(const std::string& country, Accommodation acc, Transport tr,
         bool excursions, bool hostSvc, double price);
    // Віртуальний деструктор (обов'язковий для базового класу з нащадками)
    virtual ~Tour();

    // --- Методи доступу (геттери) ---
    std::string getCountry() const;
    Accommodation getAccommodation() const;
    Transport getTransport() const;
    bool getHasExcursions() const;
    bool getHostService() const;
    double getPrice() const;

    // --- Методи зміни (сеттери) ---
    void setCountry(const std::string& c);
    void setAccommodation(Accommodation a);
    void setTransport(Transport t);
    void setHasExcursions(bool e);
    void setHostService(bool h);
    void setPrice(double p);

    // --- Чисто віртуальні методи (поліморфізм) ---
    // Повертає тип туру у текстовому вигляді ("Наземний" / "Круїз")
    virtual std::string getType() const = 0;
    // Повертає короткий опис специфічних полів нащадка
    virtual std::string getDetails() const = 0;

    // Серіалізація туру у рядок для збереження у файл
    virtual std::string serialize() const;
    // Повний опис путівки для виводу на екран
    virtual std::string toString() const;

    // --- Допоміжні статичні методи перетворення перелічень у текст ---
    static std::string accommodationToString(Accommodation a);
    static std::string transportToString(Transport t);
    static Accommodation accommodationFromInt(int i);
    static Transport transportFromInt(int i);

    // Зведення рядка UTF-8 до нижнього регістру (латиниця + українська
    // кирилиця). Застосовується для регістронезалежного пошуку.
    static std::string toLowerUtf8(const std::string& s);
};

#endif // TOUR_H
