// ============================================================================
// TravelAgency.h
// Призначення: оголошення класу TravelAgency (турагенція).
// Клас демонструє відношення КОМПОЗИЦІЇ/ВКЛЮЧЕННЯ: кожна турагенція
// містить колекцію путівок (об'єктів-нащадків Tour), якими володіє та
// керує. Зберігає реквізити агенції: назву, адресу, телефон.
// Використовувані засоби стандартної бібліотеки:
//   <string> - std::string;  <vector> - динамічний масив std::vector.
// ============================================================================
#ifndef TRAVEL_AGENCY_H
#define TRAVEL_AGENCY_H

#include "Tour.h"
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// Клас TravelAgency - турагенція з власним переліком путівок.
// ----------------------------------------------------------------------------
class TravelAgency {
private:
    std::string name;            // назва агенції
    std::string address;         // адреса
    std::string phone;           // телефон
    std::vector<Tour*> tours;    // колекція путівок (композиція)

public:
    TravelAgency();
    TravelAgency(const std::string& name, const std::string& address,
                 const std::string& phone);
    // Конструктор копіювання (глибоке копіювання путівок)
    TravelAgency(const TravelAgency& other);
    // Оператор присвоєння (глибоке копіювання)
    TravelAgency& operator=(const TravelAgency& other);
    // Деструктор: звільняє пам'ять усіх путівок
    ~TravelAgency();

    // --- Реквізити ---
    std::string getName() const;
    std::string getAddress() const;
    std::string getPhone() const;
    void setName(const std::string& n);
    void setAddress(const std::string& a);
    void setPhone(const std::string& p);

    // --- Керування путівками ---
    void addTour(Tour* t);                 // додати путівку
    bool removeTour(size_t index);         // вилучити путівку за індексом
    const std::vector<Tour*>& getTours() const;  // доступ до переліку
    size_t tourCount() const;              // кількість путівок

    // Мінімальна вартість путівки в агенції (0, якщо путівок немає)
    double minPrice() const;
    // Чи пропонує агенція тури у задану країну
    bool offersCountry(const std::string& country) const;
    // Сортувати путівки агенції за вартістю (зростання/спадання)
    void sortToursByPrice(bool ascending);

private:
    void clearTours();                     // допоміжне звільнення пам'яті
    void copyFrom(const TravelAgency& o);  // допоміжне копіювання
};

#endif // TRAVEL_AGENCY_H
