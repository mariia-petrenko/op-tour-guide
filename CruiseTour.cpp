// ============================================================================
// CruiseTour.cpp
// Призначення: реалізація методів класу CruiseTour (круїз).
// ============================================================================
#include "CruiseTour.h"
#include <sstream>

// Конструктор за замовчуванням: для круїзу транспорт завжди теплохід
CruiseTour::CruiseTour() : Tour(), route(""), liner("") {
    transport = Transport::Ship;
}

// Параметризований конструктор: транспорт фіксується як теплохід (Ship)
CruiseTour::CruiseTour(const std::string& country, const std::string& route,
                       const std::string& liner, Accommodation acc,
                       bool excursions, bool hostSvc, double price)
    : Tour(country, acc, Transport::Ship, excursions, hostSvc, price),
      route(route), liner(liner) {}

CruiseTour::~CruiseTour() {}

std::string CruiseTour::getRoute() const { return route; }
std::string CruiseTour::getLiner() const { return liner; }
void CruiseTour::setRoute(const std::string& r) { route = r; }
void CruiseTour::setLiner(const std::string& l) { liner = l; }

// Тип туру
std::string CruiseTour::getType() const { return "Круїз"; }

// Специфічні поля нащадка для опису
std::string CruiseTour::getDetails() const {
    std::ostringstream os;
    os << "Маршрут: " << route << "; Лайнер: " << liner;
    return os.str();
}

// Серіалізація: тип, спільні поля + власні поля
std::string CruiseTour::serialize() const {
    std::ostringstream os;
    os << "CRUISE|" << Tour::serialize() << '|' << route << '|' << liner;
    return os.str();
}
