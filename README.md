# Program służący do sprawdzenia czy należy zmienić opony w samochodzie na letnie lub zimowe

## Opis
Program działa dla dowolnego wpisanego przez użytkownika miasta w Polsce. Sprawdza on czy warunek jakim jest ustabilizowanie się średniej dobowej temperatury poniżej lub powyżej 7°C jest spełniony. Większość producentów podaje czas do sprawdzenia równy 2 tygodniom, jednak program pozwala również na samodzielne wprowadzenie liczby dni do sprawdzenia. 

## Szczegóły
Projekt jest napisany w języku C++ przy wykorzystaniu niestandardowych bibliotek _Curl_ oraz _RapidJSON_. Korzysta z danych pobranych ze stron _https://geocode.maps.co_ oraz _https://open-meteo.com_.
