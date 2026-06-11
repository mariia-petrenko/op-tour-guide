// ============================================================================
// TravelAgency.cpp
// Призначення: реалізація методів класу TravelAgency (турагенція).
// ============================================================================
#include "TravelAgency.h"
#include "ResortTour.h"
#include "CruiseTour.h"
#include <limits>
#include <algorithm>

// Конструктор за замовчуванням
TravelAgency::TravelAgency() : name(""), address(""), phone("") {}

// Параметризований конструктор
TravelAgency::TravelAgency(const std::string& name, const std::string& address,
                           const std::string& phone)
    : name(name), address(address), phone(phone) {}

// Допоміжний метод: глибоке копіювання путівок з іншої агенції.
// Для коректного копіювання поліморфних об'єктів визначаємо реальний тип.
void TravelAgency::copyFrom(const TravelAgency& o) {
    name = o.name;
    address = o.address;
    phone = o.phone;
    for (Tour* t : o.tours) {
        if (const ResortTour* r = dynamic_cast<const ResortTour*>(t)) {
            tours.push_back(new ResortTour(*r));
        } else if (const CruiseTour* c = dynamic_cast<const CruiseTour*>(t)) {
            tours.push_back(new CruiseTour(*c));
        }
    }
}

// Допоміжний метод: звільнення пам'яті всіх путівок
void TravelAgency::clearTours() {
    for (Tour* t : tours) delete t;
    tours.clear();
}

// Конструктор копіювання
TravelAgency::TravelAgency(const TravelAgency& other) { copyFrom(other); }

// Оператор присвоєння
TravelAgency& TravelAgency::operator=(const TravelAgency& other) {
    if (this != &other) {
        clearTours();
        copyFrom(other);
    }
    return *this;
}

// Деструктор
TravelAgency::~TravelAgency() { clearTours(); }

// --- Реквізити ---
std::string TravelAgency::getName() const { return name; }
std::string TravelAgency::getAddress() const { return address; }
std::string TravelAgency::getPhone() const { return phone; }
void TravelAgency::setName(const std::string& n) { name = n; }
void TravelAgency::setAddress(const std::string& a) { address = a; }
void TravelAgency::setPhone(const std::string& p) { phone = p; }

// Додати путівку до агенції
void TravelAgency::addTour(Tour* t) {
    if (t != nullptr) tours.push_back(t);
}

// Вилучити путівку за індексом; повертає true у разі успіху
bool TravelAgency::removeTour(size_t index) {
    if (index >= tours.size()) return false;
    delete tours[index];
    tours.erase(tours.begin() + index);
    return true;
}

const std::vector<Tour*>& TravelAgency::getTours() const { return tours; }
size_t TravelAgency::tourCount() const { return tours.size(); }

// Мінімальна вартість путівки серед усіх путівок агенції
double TravelAgency::minPrice() const {
    if (tours.empty()) return 0.0;
    double m = std::numeric_limits<double>::max();
    for (Tour* t : tours)
        if (t->getPrice() < m) m = t->getPrice();
    return m;
}

// Чи пропонує агенція хоча б одну путівку у задану країну
// (порівняння без урахування регістру)
bool TravelAgency::offersCountry(const std::string& country) const {
    std::string needle = Tour::toLowerUtf8(country);
    for (Tour* t : tours)
        if (Tour::toLowerUtf8(t->getCountry()) == needle) return true;
    return false;
}

// Сортування путівок агенції за вартістю (зростання або спадання)
void TravelAgency::sortToursByPrice(bool ascending) {
    std::sort(tours.begin(), tours.end(), [ascending](Tour* a, Tour* b) {
        return ascending ? (a->getPrice() < b->getPrice())
                         : (a->getPrice() > b->getPrice());
    });
}
