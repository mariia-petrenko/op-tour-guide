// ============================================================================
// ResortTour.h
// Призначення: оголошення класу ResortTour (наземний/курортний тур).
// Є нащадком абстрактного класу Tour (відношення УСПАДКУВАННЯ).
// Додає специфічні поля: місто перебування та кількість ночей.
// Використовувані засоби: <string> (std::string).
// ============================================================================
#ifndef RESORT_TOUR_H
#define RESORT_TOUR_H

#include "Tour.h"
#include <string>

// ----------------------------------------------------------------------------
// Клас ResortTour - наземний тур з проживанням у конкретному місті.
// ----------------------------------------------------------------------------
class ResortTour : public Tour {
private:
    std::string city;  // місто (курорт) перебування
    int nights;        // кількість ночей

public:
    ResortTour();
    ResortTour(const std::string& country, const std::string& city, int nights,
               Accommodation acc, Transport tr, bool excursions,
               bool hostSvc, double price);
    ~ResortTour() override;

    std::string getCity() const;
    int getNights() const;
    void setCity(const std::string& c);
    void setNights(int n);

    // Перевизначення чисто віртуальних методів базового класу (поліморфізм)
    std::string getType() const override;
    std::string getDetails() const override;
    std::string serialize() const override;
};

#endif // RESORT_TOUR_H
