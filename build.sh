#!/bin/bash
# Хелпер для macOS/Linux: компілює та запускає "Електронний Довідник туриста".
# Запуск у Терміналі:  ./build.sh
# (за потреби спершу зробити файл виконуваним:  chmod +x build.sh)

# Перейти в папку, де лежить цей скрипт (щоб працювало з будь-якого місця)
cd "$(dirname "$0")" || exit 1

echo "Компіляція програми..."
g++ -std=c++17 -o tourist_guide \
    main.cpp Tour.cpp ResortTour.cpp CruiseTour.cpp \
    TravelAgency.cpp TourGuide.cpp

if [ $? -eq 0 ]; then
    echo "Компіляція успішна. Запуск:"
    echo
    ./tourist_guide
else
    echo "Помилка компіляції. Перевірте, чи встановлено компілятор (xcode-select --install)."
fi
