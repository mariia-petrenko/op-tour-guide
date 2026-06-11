// ============================================================================
// TourGuide.h
// Призначення: оголошення класу TourGuide (електронний довідник туриста).
// Це головний клас-контейнер, що АГРЕГУЄ колекцію турагенцій і реалізує
// всю функціональність довідника: додавання, вилучення, редагування,
// пошук, сортування, фільтрацію та збереження/завантаження даних у файл.
// Використовувані засоби стандартної бібліотеки:
//   <string> - std::string; <vector> - std::vector;
//   <functional> - std::function (критерії фільтрації/сортування);
//   <fstream> (у .cpp) - файлове введення/виведення.
// ============================================================================
#ifndef TOUR_GUIDE_H
#define TOUR_GUIDE_H

#include "TravelAgency.h"
#include <string>
#include <vector>
#include <functional>
#include <utility>

// Поле, за яким виконується сортування турагенцій
enum class SortField {
    Name,      // за назвою
    MinPrice   // за мінімальною вартістю путівки
};

// ----------------------------------------------------------------------------
// Клас TourGuide - електронний довідник турагенцій.
// ----------------------------------------------------------------------------
class TourGuide {
private:
    std::vector<TravelAgency> agencies;  // перелік турагенцій (агрегація)

public:
    TourGuide();
    ~TourGuide();

    // --- Базові операції над довідником ---
    void addAgency(const TravelAgency& a);     // додати турагенцію
    bool removeAgency(size_t index);           // вилучити за індексом
    bool editAgencyName(size_t index, const std::string& newName); // редагувати
    size_t size() const;                       // кількість агенцій
    const std::vector<TravelAgency>& getAgencies() const;
    TravelAgency* at(size_t index);            // доступ за індексом (або nullptr)

    // --- Пошук ---
    // Пошук агенцій за фрагментом назви (без урахування регістру)
    std::vector<size_t> findByName(const std::string& namePart) const;
    // Пошук агенцій, що пропонують тури у задану країну
    std::vector<size_t> findByCountry(const std::string& country) const;

    // --- Сортування ---
    void sortAgencies(SortField field, bool ascending = true);
    // Повертає всі путівки усіх агенцій, відсортовані за вартістю,
    // у вигляді пар (назва агенції, вказівник на путівку)
    std::vector<std::pair<std::string, const Tour*>>
        toursSortedByPrice(bool ascending = true) const;

    // --- Фільтрація за довільним критерієм користувача ---
    // Повертає копії агенцій, що задовольняють предикат
    std::vector<TravelAgency> filter(
        const std::function<bool(const TravelAgency&)>& predicate) const;
    // Готовий фільтр: агенції з путівками, дешевшими за задану межу
    std::vector<TravelAgency> filterByMaxPrice(double maxPrice) const;

    // --- Робота з файлом (база даних) ---
    bool saveToFile(const std::string& filename) const;  // зберегти
    bool loadFromFile(const std::string& filename);       // завантажити

    // Вивід усіх даних довідника у текстовий звіт (файл)
    bool exportReport(const std::string& filename) const;

private:
    // Розбір рядка путівки з файлу у відповідний об'єкт-нащадок Tour
    Tour* parseTour(const std::string& line) const;
};

#endif // TOUR_GUIDE_H
