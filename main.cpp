#include <iostream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "curl/curl.h"
#include <string>
#include <conio.h>
#include <cstring>

using namespace rapidjson;
using namespace std;

size_t handle_received_data(void * ptr, size_t size, size_t n, string * data)
{
    size_t t = size * n;
    data->append((char *)ptr, t);
    return t;
}

char * get_json_data(const string url)
{
    curl_global_init(CURL_GLOBAL_ALL);
    CURL * curl = curl_easy_init();
    string response;

    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 10L);
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, handle_received_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);

    curl_easy_perform(curl);

    curl_easy_cleanup(curl);
    curl_global_cleanup();

    char * json = new char[response.length() + 1];
    for(int i = 0; i < response.length(); i++) json[i] = response[i];
    json[response.length()] = '\0';

    return json;
}

string zmien_wchar (wchar_t slowo[])
{
    string zmienione;
    for (int i = 0; i < wcslen(slowo) ; i++)
    {
        if (slowo[i] == L' ') zmienione.push_back('+');
        else if (slowo[i] == L'ó' || slowo[i] == L'Ó') zmienione.push_back('o');
        else if (slowo[i] == L'ę' || slowo[i] == L'Ę') zmienione.push_back('e');
        else if (slowo[i] == L'ą' || slowo[i] == L'Ą') zmienione.push_back('a');
        else if (slowo[i] == L'ł' || slowo[i] == L'Ł') zmienione.push_back('l');
        else if (slowo[i] == L'ć' || slowo[i] == L'Ć') zmienione.push_back('c');
        else if (slowo[i] == L'ń' || slowo[i] == L'Ń') zmienione.push_back('n');
        else if (slowo[i] == L'ż' || slowo[i] == L'Ż' || slowo[i] == L'ź' || slowo[i] == L'Ź') zmienione.push_back('z');
        else zmienione.push_back((char)slowo[i]);
    }
    return zmienione;
}

int main()
{
    setlocale(LC_ALL,"");
    SetConsoleOutputCP(1250);
    SetConsoleCP(1250);

    wcout << L"Podaj nazwę miasta w Polsce w którym chcesz sprawdzić czy należy zmienić opony: ";
    wchar_t bufer[256];
    wcin.getline(bufer, 256);

    const string miasto = zmien_wchar(bufer);
    if (miasto.length() < 3)
    {
        wcout << L"Podano złą nazwę miasta." << endl;
        getch();
        return 0;
    }

    const string url_miasto = "https://geocode.maps.co/search?city=" + miasto + "&country=PL";

    const char * json_miasto = get_json_data(url_miasto);
    if (strcmp(json_miasto, "[]") == 0)
    {
        delete[] json_miasto;
        wcout << L"Podano złą nazwę miasta." << endl;
        getch();
        return 0;
    }

    Document document;
    document.Parse(json_miasto);

    string latitude = (document[0]["lat"].GetString());
    latitude = latitude.substr(0, 5);
    string longitude = document[0]["lon"].GetString();
    longitude = longitude.substr(0, 5);

    wcout << L"Współrzędne geograficzne miasta to "; cout << latitude; wcout << L"°N "; cout << longitude; wcout << L"°E." << endl;

    int dni = 0;
    wcout << L"Podaj liczbę dni do sprawdzenia czy temperatura się ustabilizowała (zalecane jest 14): ";
    cin >> dni;

    if (dni < 1)
    {
        delete[] json_miasto;
        wcout << L"Nieprawidłowe dane." << endl;
        getch();
        return 0;
    }
    else if (dni > 90)
    {
        delete[] json_miasto;
        wcout << L"Liczba dni do sprawdzenia nie może przekraczać 90." << endl;
        getch();
        return 0;
    }

    const string url_pogoda = "https://api.open-meteo.com/v1/forecast?latitude=" + latitude + "&longitude=" +
            longitude +"&past_days=" + to_string(dni) + "&hourly=temperature_2m";

    const char * json_pogoda = get_json_data(url_pogoda);

    document.Parse(json_pogoda);

    const string s = document["hourly"]["time"].GetArray()[dni * 24].GetString();
    const int miesiac = stoi(s.substr(5, 2));

    int ile_przekroczylo = 0;

    for(int d = 0; d < dni; d++)
    {
        double suma = 0;
        for (int h = 0; h <= 23; h++) suma += document["hourly"]["temperature_2m"].GetArray()[d * 24 + h].GetDouble();
        if ((miesiac <= 8 && suma / 24.0 > 7.0) || (miesiac > 8 && suma / 24.0 < 7.0)) ile_przekroczylo++;
    }

    if (miesiac <= 8)
    {
        wcout << L"W ostatnich " << dni << L" dniach, średnia dobowa temperatura była wyższa niż 7°C "
            << ile_przekroczylo << L" razy" ;
        if (ile_przekroczylo == dni) wcout << L", a więc należy zmienić opony na letnie";
        else wcout << L", a więc nie jest wskazana zmiana opon";
    }
    else
    {
        wcout << L"W ostatnich " << dni << L" dniach, średnia dobowa temperatura była niższa niż 7°C "
            << ile_przekroczylo << L" razy";
        if (ile_przekroczylo == dni) wcout << L", a więc należy zmienić opony na zimowe";
        else wcout << L", a więc nie jest wskazana zmiana opon";
    }
    wcout << L"." << endl;

    delete[] json_miasto;
    delete[] json_pogoda;

    getch();

    return 0;
}
