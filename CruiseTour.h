// ============================================================================
// CruiseTour.h
// Призначення: оголошення класу CruiseTour (морський круїз).
// Є нащадком абстрактного класу Tour (відношення УСПАДКУВАННЯ).
// Додає специфічні поля: маршрут круїзу та назву лайнера.
// Використовувані засоби: <string> (std::string).
// ============================================================================
#ifndef CRUISE_TOUR_H
#define CRUISE_TOUR_H

#include "Tour.h"
#include <string>

// ----------------------------------------------------------------------------
// Клас CruiseTour - круїзний тур із заданим маршрутом.
// ----------------------------------------------------------------------------
class CruiseTour : public Tour {
private:
    std::string route;    // маршрут круїзу (перелік портів)
    std::string liner;    // назва лайнера

public:
    CruiseTour();
    CruiseTour(const std::string& country, const std::string& route,
               const std::string& liner, Accommodation acc,
               bool excursions, bool hostSvc, double price);
    ~CruiseTour() override;

    std::string getRoute() const;
    std::string getLiner() const;
    void setRoute(const std::string& r);
    void setLiner(const std::string& l);

    // Перевизначення чисто віртуальних методів базового класу (поліморфізм)
    std::string getType() const override;
    std::string getDetails() const override;
    std::string serialize() const override;
};

#endif // CRUISE_TOUR_H
