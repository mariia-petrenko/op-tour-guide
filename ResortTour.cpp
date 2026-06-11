// ============================================================================
// ResortTour.cpp
// Призначення: реалізація методів класу ResortTour (наземний тур).
// ============================================================================
#include "ResortTour.h"
#include <sstream>

// Конструктор за замовчуванням
ResortTour::ResortTour() : Tour(), city(""), nights(0) {}

// Параметризований конструктор: викликає конструктор базового класу
ResortTour::ResortTour(const std::string& country, const std::string& city,
                       int nights, Accommodation acc, Transport tr,
                       bool excursions, bool hostSvc, double price)
    : Tour(country, acc, tr, excursions, hostSvc, price),
      city(city), nights(nights) {}

ResortTour::~ResortTour() {}

std::string ResortTour::getCity() const { return city; }
int ResortTour::getNights() const { return nights; }
void ResortTour::setCity(const std::string& c) { city = c; }
void ResortTour::setNights(int n) { nights = n; }

// Тип туру
std::string ResortTour::getType() const { return "Наземний"; }

// Специфічні поля нащадка для опису
std::string ResortTour::getDetails() const {
    std::ostringstream os;
    os << "Місто: " << city << "; Ночей: " << nights;
    return os.str();
}

// Серіалізація: тип, спільні поля (з базового класу) + власні поля
std::string ResortTour::serialize() const {
    std::ostringstream os;
    os << "RESORT|" << Tour::serialize() << '|' << city << '|' << nights;
    return os.str();
}
