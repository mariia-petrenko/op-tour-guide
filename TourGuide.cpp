// ============================================================================
// TourGuide.cpp
// Призначення: реалізація методів класу TourGuide (довідник туриста).
// ============================================================================
#include "TourGuide.h"
#include "ResortTour.h"
#include "CruiseTour.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>

TourGuide::TourGuide() {}
TourGuide::~TourGuide() {}

// Додати турагенцію до довідника
void TourGuide::addAgency(const TravelAgency& a) { agencies.push_back(a); }

// Вилучити турагенцію за індексом
bool TourGuide::removeAgency(size_t index) {
    if (index >= agencies.size()) return false;
    agencies.erase(agencies.begin() + index);
    return true;
}

// Редагувати назву турагенції за індексом
bool TourGuide::editAgencyName(size_t index, const std::string& newName) {
    if (index >= agencies.size()) return false;
    agencies[index].setName(newName);
    return true;
}

size_t TourGuide::size() const { return agencies.size(); }
const std::vector<TravelAgency>& TourGuide::getAgencies() const { return agencies; }

TravelAgency* TourGuide::at(size_t index) {
    if (index >= agencies.size()) return nullptr;
    return &agencies[index];
}

// Пошук агенцій за фрагментом назви (без урахування регістру, UTF-8)
std::vector<size_t> TourGuide::findByName(const std::string& namePart) const {
    std::vector<size_t> result;
    std::string needle = Tour::toLowerUtf8(namePart);
    for (size_t i = 0; i < agencies.size(); ++i) {
        if (Tour::toLowerUtf8(agencies[i].getName()).find(needle) != std::string::npos)
            result.push_back(i);
    }
    return result;
}

// Пошук агенцій за країною туру
std::vector<size_t> TourGuide::findByCountry(const std::string& country) const {
    std::vector<size_t> result;
    for (size_t i = 0; i < agencies.size(); ++i) {
        if (agencies[i].offersCountry(country))
            result.push_back(i);
    }
    return result;
}

// Сортування турагенцій за обраним полем.
// Додатково впорядковуються путівки всередині кожної агенції за вартістю
// (у тому ж напрямку), щоб перелік був повністю відсортований.
void TourGuide::sortAgencies(SortField field, bool ascending) {
    auto cmp = [field, ascending](const TravelAgency& a, const TravelAgency& b) {
        bool less;
        if (field == SortField::Name)
            less = Tour::toLowerUtf8(a.getName()) < Tour::toLowerUtf8(b.getName());
        else
            less = a.minPrice() < b.minPrice();
        return ascending ? less : !less;
    };
    std::sort(agencies.begin(), agencies.end(), cmp);
    for (TravelAgency& a : agencies)
        a.sortToursByPrice(ascending);
}

// Формує єдиний список усіх путівок усіх агенцій, відсортований за вартістю.
// Кожен елемент містить назву агенції та вказівник на путівку.
std::vector<std::pair<std::string, const Tour*>>
TourGuide::toursSortedByPrice(bool ascending) const {
    std::vector<std::pair<std::string, const Tour*>> all;
    for (const TravelAgency& a : agencies)
        for (const Tour* t : a.getTours())
            all.emplace_back(a.getName(), t);
    std::sort(all.begin(), all.end(),
        [ascending](const std::pair<std::string, const Tour*>& x,
                    const std::pair<std::string, const Tour*>& y) {
            return ascending ? (x.second->getPrice() < y.second->getPrice())
                             : (x.second->getPrice() > y.second->getPrice());
        });
    return all;
}

// Фільтрація за довільним предикатом користувача
std::vector<TravelAgency> TourGuide::filter(
    const std::function<bool(const TravelAgency&)>& predicate) const {
    std::vector<TravelAgency> result;
    for (const TravelAgency& a : agencies)
        if (predicate(a)) result.push_back(a);
    return result;
}

// Готовий фільтр за максимальною ціною путівки
std::vector<TravelAgency> TourGuide::filterByMaxPrice(double maxPrice) const {
    return filter([maxPrice](const TravelAgency& a) {
        return a.tourCount() > 0 && a.minPrice() <= maxPrice;
    });
}

// Збереження довідника у текстовий файл (проста БД).
// Формат: A|назва|адреса|телефон     - запис турагенції
//         T|<серіалізований тур>      - запис путівки агенції
bool TourGuide::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;
    for (const TravelAgency& a : agencies) {
        out << "A|" << a.getName() << '|' << a.getAddress()
            << '|' << a.getPhone() << '\n';
        for (Tour* t : a.getTours())
            out << "T|" << t->serialize() << '\n';
    }
    out.close();
    return true;
}

// Розбір рядка путівки (після префікса "T|") у об'єкт-нащадок Tour
Tour* TourGuide::parseTour(const std::string& line) const {
    std::vector<std::string> f;
    std::stringstream ss(line);
    std::string token;
    while (std::getline(ss, token, '|')) f.push_back(token);
    if (f.empty()) return nullptr;

    // Спільні поля: country, acc, transport, excursions, host, price
    // Розташування: f[1..6] для тіла Tour::serialize(), далі специфічні
    try {
        if (f[0] == "RESORT" && f.size() >= 9) {
            std::string country = f[1];
            Accommodation acc = Tour::accommodationFromInt(std::stoi(f[2]));
            Transport tr = Tour::transportFromInt(std::stoi(f[3]));
            bool exc = std::stoi(f[4]) != 0;
            bool host = std::stoi(f[5]) != 0;
            double price = std::stod(f[6]);
            std::string city = f[7];
            int nights = std::stoi(f[8]);
            return new ResortTour(country, city, nights, acc, tr, exc, host, price);
        } else if (f[0] == "CRUISE" && f.size() >= 9) {
            std::string country = f[1];
            Accommodation acc = Tour::accommodationFromInt(std::stoi(f[2]));
            bool exc = std::stoi(f[4]) != 0;
            bool host = std::stoi(f[5]) != 0;
            double price = std::stod(f[6]);
            std::string route = f[7];
            std::string liner = f[8];
            return new CruiseTour(country, route, liner, acc, exc, host, price);
        }
    } catch (...) {
        return nullptr;  // некоректний рядок ігнорується
    }
    return nullptr;
}

// Завантаження довідника з текстового файлу
bool TourGuide::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;
    agencies.clear();
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        if (line.rfind("A|", 0) == 0) {
            // Новий запис турагенції
            std::vector<std::string> f;
            std::stringstream ss(line.substr(2));
            std::string token;
            while (std::getline(ss, token, '|')) f.push_back(token);
            std::string name = f.size() > 0 ? f[0] : "";
            std::string addr = f.size() > 1 ? f[1] : "";
            std::string phone = f.size() > 2 ? f[2] : "";
            agencies.emplace_back(name, addr, phone);
        } else if (line.rfind("T|", 0) == 0 && !agencies.empty()) {
            // Путівка належить останній доданій агенції
            Tour* t = parseTour(line.substr(2));
            if (t) agencies.back().addTour(t);
        }
    }
    in.close();
    return true;
}

// Вивід усіх даних довідника у читабельний текстовий звіт
bool TourGuide::exportReport(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;
    out << "ЕЛЕКТРОННИЙ ДОВІДНИК ТУРИСТА — ЗВІТ\n";
    out << "Кількість турагенцій: " << agencies.size() << "\n\n";
    for (size_t i = 0; i < agencies.size(); ++i) {
        const TravelAgency& a = agencies[i];
        out << (i + 1) << ". " << a.getName()
            << " | " << a.getAddress() << " | тел. " << a.getPhone() << "\n";
        const auto& tours = a.getTours();
        for (size_t j = 0; j < tours.size(); ++j)
            out << "    " << (j + 1) << ") " << tours[j]->toString() << "\n";
        out << "\n";
    }
    out.close();
    return true;
}
