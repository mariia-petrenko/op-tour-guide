// ============================================================================
// Tour.cpp
// Призначення: реалізація методів абстрактного базового класу Tour.
// ============================================================================
#include "Tour.h"
#include <sstream>
#include <cctype>

// Конструктор за замовчуванням: ініціалізує поля нейтральними значеннями
Tour::Tour()
    : country(""), accommodation(Accommodation::Hotel3),
      transport(Transport::Plane), hasExcursions(false),
      hostService(false), price(0.0) {}

// Параметризований конструктор
Tour::Tour(const std::string& country, Accommodation acc, Transport tr,
           bool excursions, bool hostSvc, double price)
    : country(country), accommodation(acc), transport(tr),
      hasExcursions(excursions), hostService(hostSvc), price(price) {}

// Віртуальний деструктор
Tour::~Tour() {}

// --- Геттери ---
std::string Tour::getCountry() const { return country; }
Accommodation Tour::getAccommodation() const { return accommodation; }
Transport Tour::getTransport() const { return transport; }
bool Tour::getHasExcursions() const { return hasExcursions; }
bool Tour::getHostService() const { return hostService; }
double Tour::getPrice() const { return price; }

// --- Сеттери ---
void Tour::setCountry(const std::string& c) { country = c; }
void Tour::setAccommodation(Accommodation a) { accommodation = a; }
void Tour::setTransport(Transport t) { transport = t; }
void Tour::setHasExcursions(bool e) { hasExcursions = e; }
void Tour::setHostService(bool h) { hostService = h; }
void Tour::setPrice(double p) { price = p; }

// Серіалізація спільних полів (нащадки доповнюють своїми даними)
std::string Tour::serialize() const {
    std::ostringstream os;
    os << country << '|'
       << static_cast<int>(accommodation) << '|'
       << static_cast<int>(transport) << '|'
       << (hasExcursions ? 1 : 0) << '|'
       << (hostService ? 1 : 0) << '|'
       << price;
    return os.str();
}

// Текстовий опис путівки для виводу на екран
std::string Tour::toString() const {
    std::ostringstream os;
    os << "[" << getType() << "] Країна: " << country
       << "; " << getDetails()
       << "; Проживання: " << accommodationToString(accommodation)
       << "; Проїзд: " << transportToString(transport)
       << "; Екскурсії: " << (hasExcursions ? "так" : "ні")
       << "; Сервіс приймаючої сторони: " << (hostService ? "так" : "ні")
       << "; Вартість: " << price << " грн";
    return os.str();
}

// Перетворення типу проживання у текст
std::string Tour::accommodationToString(Accommodation a) {
    switch (a) {
        case Accommodation::Hotel2:    return "Готель 2*";
        case Accommodation::Hotel3:    return "Готель 3*";
        case Accommodation::Hotel4:    return "Готель 4*";
        case Accommodation::Hotel5:    return "Готель 5*";
        case Accommodation::Apartment: return "Апартаменти";
        case Accommodation::Hostel:    return "Хостел";
        default:                       return "Невідомо";
    }
}

// Перетворення типу транспорту у текст
std::string Tour::transportToString(Transport t) {
    switch (t) {
        case Transport::Plane: return "Літак";
        case Transport::Bus:   return "Автобус";
        case Transport::Train: return "Потяг";
        case Transport::Ship:  return "Теплохід";
        default:               return "Невідомо";
    }
}

// Безпечне перетворення цілого числа у тип проживання
Accommodation Tour::accommodationFromInt(int i) {
    if (i < 0 || i > 5) return Accommodation::Hotel3;
    return static_cast<Accommodation>(i);
}

// Безпечне перетворення цілого числа у тип транспорту
Transport Tour::transportFromInt(int i) {
    if (i < 0 || i > 3) return Transport::Plane;
    return static_cast<Transport>(i);
}

// Зведення рядка UTF-8 до нижнього регістру.
// Опрацьовує латиницю (A-Z) та українську кирилицю (А-Я, Ё, Є, І, Ї, Ґ),
// інші символи залишає без змін.
std::string Tour::toLowerUtf8(const std::string& s) {
    std::string out;
    size_t i = 0, n = s.size();
    while (i < n) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c < 0x80) {
            // Однобайтовий символ ASCII
            out += static_cast<char>(std::tolower(c));
            ++i;
        } else if ((c & 0xE0) == 0xC0 && i + 1 < n) {
            // Двобайтовий символ UTF-8 (зокрема кирилиця)
            unsigned char c2 = static_cast<unsigned char>(s[i + 1]);
            unsigned int cp = ((c & 0x1F) << 6) | (c2 & 0x3F);
            if (cp >= 0x0410 && cp <= 0x042F) cp += 0x20;  // А-Я -> а-я
            else if (cp == 0x0401) cp = 0x0451;            // Ё -> ё
            else if (cp == 0x0404) cp = 0x0454;            // Є -> є
            else if (cp == 0x0406) cp = 0x0456;            // І -> і
            else if (cp == 0x0407) cp = 0x0457;            // Ї -> ї
            else if (cp == 0x0490) cp = 0x0491;            // Ґ -> ґ
            out += static_cast<char>(0xC0 | (cp >> 6));
            out += static_cast<char>(0x80 | (cp & 0x3F));
            i += 2;
        } else {
            // Інші багатобайтові символи — без змін
            out += static_cast<char>(c);
            ++i;
        }
    }
    return out;
}
