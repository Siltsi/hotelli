// main.cpp

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <limits>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <cctype>
#include <algorithm>
#include "huone.hpp"
#include "varaus.hpp"

// Huone ja varaus vektorit
std::vector<huone> huoneet;
std::vector<varaus> varaukset;

// Funktio joka muuntaa chrono time_pointin string muotoon
std::string tp_ajaksi(std::chrono::time_point<std::chrono::system_clock> tp) {
    std::time_t tt = std::chrono::system_clock::to_time_t(tp);
    std::tm local_time;
    localtime_s(&local_time, &tt);

    std::ostringstream oss;
    oss << std::put_time(&local_time, "%d.%m.%Y %H.%M");

    return oss.str();
}

// Funktio, joka lataa tallennetut huoneet filest‰ "huoneet.txt"
std::vector<huone> huoneiden_lataus() {
    system("CLS");
    std::cout << "Ladataan huoneita..." << std::endl;

    std::vector<huone> huoneet;
    std::ifstream inFile("huoneet.txt");
    std::string line;

    while (std::getline(inFile, line)) {
        std::stringstream ss(line);

        std::string tieto;
        std::string tiedot[2];

        for (int x = 0; x < 2; x++) {
            std::getline(ss, tieto, ',');
            tiedot[x] = tieto;
        }

        int id = 0;
        std::string varaaja = "-";
        bool varattu = false;

        for (int x = 0; x < varaukset.size(); x++) {
            if (stoi(tiedot[0]) == varaukset[x].get_huone() &&
                std::chrono::system_clock::now() > varaukset[x].get_alku() &&
                std::chrono::system_clock::now() < varaukset[x].get_loppu()) {
                id = varaukset[x].get_id();
                varaaja = varaukset[x].get_varaaja();
                varattu = true;
            }
        }

        huoneet.push_back(huone(stoi(tiedot[0]), stoi(tiedot[1]), id, varaaja, varattu));
    }

    inFile.close();

    std::cout << std::endl << "Huoneet ladattu.\n";

    system("CLS");

    return huoneet;
}

// Funktio, joka tallentaa huoneet fileen "huoneet.txt"
void huoneiden_tallennus(std::vector<huone> huoneet) {
    std::ofstream talletus(("huoneet.txt"));

    for (int x = 0; x < huoneet.size(); x++) {
        std::string huoneen_tiedot = huoneet[x].tiedot();
        talletus << huoneen_tiedot << "\n";
    }
    talletus.close();
}

// Funktio, jolla luodaan uudet huoneet, jos vanhoja ei ole olemassa (40-300)
void huoneiden_luonti() {

    std::cout << "Halutaanko huoneidenmaara arpoa vai paattaa? (a/p)" << std::endl << std::endl;
    int maara = 0;
    char valinta;

    srand(time(0));

    while (true) {
        std::cout << "Valinta: ";
        std::cin >> valinta;
        if (std::cin.fail() || (toupper(valinta) != 'A' && toupper(valinta) != 'P')) {
            std::cin.clear();
            std::cin.ignore();
            continue;
        }
        else {
            break;
        }
    }

    if (toupper(valinta) == 'A') {
        maara = 20 + rand() % 131;
        maara *= 2;
    }

    if (toupper(valinta) == 'P') {
        std::cout << "Montako huonetta luodaan (40-300)" << std::endl << std::endl;
        while (true) {
            std::cout << "Valinta: ";
            std::cin >> maara;
            if (std::cin.fail() || maara < 40 || maara > 300) {
                std::cin.clear();
                std::cin.ignore();
                continue;
            }
            else {
                break;
            }
        }
    }

    for (int x = 0; x < maara / 2; x++) {
        huoneet.push_back(huone(huoneet.size() + 1, 1, 0, "-", 0));
    }
    for (int x = 0; x < maara / 2; x++) {
        huoneet.push_back(huone(huoneet.size() + 1, 2, 0, "-", 0));
    }

    std::cout << "Huoneet luotu" << std::endl;
    huoneiden_tallennus(huoneet);

    while (true) {
        std::cout << "Syota k jatkaaksesi: ";
        char k;
        std::cin >> k;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore();
            continue;
        }
        if (toupper(k) == 'K') {
            break;
        }
    }
}

// Funktio, jolla p‰ivitet‰‰n huoneiden varaustentila (tarkistaa onko olemassa aktiivista varausta huoneille)
void huoneet_update() {
    for (int x = 0; x < huoneet.size(); x++) {
        bool aktiivinenvaraus = false;
        for (int y = 0; y < varaukset.size(); y++) {
            if (x + 1 == varaukset[y].get_huone() &&
                std::chrono::system_clock::now() > varaukset[y].get_alku() &&
                std::chrono::system_clock::now() < varaukset[y].get_loppu()) {
                huoneet[x].set_varausnro(varaukset[y].get_id());
                huoneet[x].set_varaaja(varaukset[y].get_varaaja());
                huoneet[x].set_varattu(true);
                aktiivinenvaraus = true;
                break;
            }
        }
        if (!aktiivinenvaraus) {
            huoneet[x].set_varausnro(0);
            huoneet[x].set_varaaja("-");
            huoneet[x].set_varattu(false);
        }
    }
}

// Funktio, joka tallentaa varaukset fileen "varaukset.txt"
void varauksien_tallennus(std::vector<varaus> varaukset) {
    std::ofstream talletus(("varaukset.txt"));

    for (int x = 0; x < varaukset.size(); x++) {
        std::string varauksen_tiedot = varaukset[x].tiedot();
        talletus << varauksen_tiedot << "\n";
    }
    talletus.close();
}

// Funktio, joka lataa tallennetut varaukset filest‰ "varaukset.txt"
std::vector<varaus> varauksien_lataus() {
    system("CLS");
    std::cout << "Ladataan varauksia..." << std::endl;

    std::vector<varaus> varaukset;
    std::ifstream inFile("varaukset.txt");
    std::string line;

    while (std::getline(inFile, line)) {
        std::stringstream ss(line);

        std::string tieto;
        std::string tiedot[6];

        for (int x = 0; x < 6; x++) {
            getline(ss, tieto, ',');
            tiedot[x] = tieto;
        }

        varaukset.push_back(varaus(
            stoi(tiedot[0]),
            stoi(tiedot[1]),
            stoi(tiedot[2]),
            tiedot[3],
            std::chrono::system_clock::time_point{ std::chrono::minutes{stoi(tiedot[4])} },
            std::chrono::system_clock::time_point{ std::chrono::minutes{stoi(tiedot[5])} }));
    }

    inFile.close();

    std::cout << std::endl << "Varaukset ladattu.\n";

    system("CLS");

    return varaukset;
}

// Funktio, joka p‰ivitt‰‰ varaukset vektorin (poistaa vanhentuneet varaukset)
void varaukset_update() {
    for (int x = 0; x < varaukset.size(); x++) {
        if (varaukset[x].get_loppu() < std::chrono::system_clock::now()) {
            varaukset.erase(varaukset.begin() + x);
        }
    }
    varauksien_tallennus(varaukset);
}

// Menu funktiot:

// Funktio, joka sis‰lt‰‰ menun, jossa varaus tehd‰‰n
void varauksen_teko() {
    system("CLS");

    std::cout << "|---------------------------------------|" << std::endl
        << "| Tee varaus (k: kumoa / t: taaksepain) |" << std::endl
        << "|---------------------------------------|" << std::endl << std::endl;

    int askel = 0;
    std::string input;

    int koko = 0;
    int id = 0;
    int yot = 0;
    int ale = 0;
    int hinta = 0;
    int huone = 0;
    int pos = 0;
    std::string varaaja;
    std::string aika;
    std::chrono::system_clock::time_point alku;
    std::chrono::system_clock::time_point loppu;

    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);

    std::tm local_time{};
    localtime_s(&local_time, &t);

    local_time.tm_hour = 0;
    local_time.tm_min = 0;
    local_time.tm_sec = 0;

    std::time_t midnight_time = std::mktime(&local_time);
    auto keskiyo = std::chrono::system_clock::from_time_t(midnight_time);

    srand(time(0));

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    while (true) {

        switch (askel) {

        case 0:
            input = " ";
            std::cout << "Minka kokoinen huone varataan?";
            std::getline(std::cin, input);

            if (input == "k" || input == "K") {
                return;
            }
            if (input == "t" || input == "T") {
                std::cout << "Tama on ensimmainen kysymys" << std::endl;
            }
            try {
                int koko_input = std::stoi(input);
                if (koko_input == 1 || koko_input == 2) {
                    koko = koko_input;
                    int vapaita = 0;
                    for (int x = 0; x < huoneet.size(); x++) {
                        bool ei_varattu = true;
                        if (huoneet[x].get_koko() == koko) {
                            for (int y = 0; y < varaukset.size(); y++) {
                                if (varaukset[y].get_huone() == x + 1
                                    && varaukset[y].get_alku() < keskiyo + std::chrono::hours(36) && varaukset[y].get_loppu() > keskiyo + std::chrono::hours(12)) {
                                    ei_varattu = false;
                                    break;
                                }
                            }
                        }
                        if (huoneet[x].get_koko() != koko) {
                            ei_varattu = false;
                        }
                        if (ei_varattu) {
                            vapaita++;
                        }
                    }
                    if (vapaita == 0) {
                        std::cout << koko << "hh huoneita ei ole vapaana." << std::endl;
                    }
                    else {
                        askel++;
                    }
                }
                else {
                    std::cout << "Virhe: Syota 1 tai 2." << std::endl;
                }
            }
            catch (...) {
                std::cout << "Virhe: Syota numero." << std::endl;
            }


            break;

        case 1:
            input = " ";
            std::cout << "Milla nimella huone varataan?";
            std::getline(std::cin, input);
            if (input == "k" || input == "K") {
                return;
            }
            if (input == "t" || input == "T") {
                askel--;
            }
            else if (!input.empty()) {
                varaaja = input;
                askel++;
            }
            else {
                std::cout << "Virhe: Nimi ei voi olla tyhja." << std::endl;
            }
            break;

        case 2:
            input = " ";
            std::cout << "Moneksiko yoksi huone varataan? ";
            std::getline(std::cin, input);
            if (input == "k" || input == "K") {
                return;
            }
            if (input == "t" || input == "T") {
                askel--;
                break;
            }
            try {
                yot = stoi(input);
                if (yot <= 0) {
                    std::cout << "Virhe: Syota positiivinen luku." << std::endl;
                    break;
                }

                alku = keskiyo + std::chrono::hours(12);
                loppu = alku + std::chrono::hours(24 * yot);

                askel++;
            }
            catch (...) {
                std::cout << "Virhe: Syota numero." << std::endl;
            }
            break;
        case 3:
            std::vector<int> vapaat_huoneet;

            for (int x = 0; x < huoneet.size(); x++) {
                bool ei_varattu = true;
                if (huoneet[x].get_koko() == koko) {
                    for (int y = 0; y < varaukset.size(); y++) {
                        if (varaukset[y].get_huone() == x + 1
                            && varaukset[y].get_alku() < loppu && varaukset[y].get_loppu() > alku) {
                            ei_varattu = false;
                            break;
                        }
                    }
                }
                if (huoneet[x].get_koko() != koko) {
                    ei_varattu = false;
                }
                if (ei_varattu) {
                    vapaat_huoneet.push_back(x + 1);
                }
            }

            if (vapaat_huoneet.size() == 0) {
                std::cout << "Huoneita ei ole vapaana." << std::endl;
                askel = 0;
            }

            while (true) {
                id = 10000 + rand() % 90000;
                bool on_uusi = true;
                for (int x = 0; x < varaukset.size(); x++) {
                    if (id == varaukset[x].get_id()) {
                        on_uusi = false;
                        break;
                    }
                }
                if (on_uusi) {
                    break;
                }
            }

            huone = vapaat_huoneet[rand() % vapaat_huoneet.size()];

            for (int x = 0; x < varaukset.size(); x++) {
                pos = x;
                if (alku < varaukset[x].get_alku() ||
                    (alku == varaukset[x].get_alku() && loppu < varaukset[x].get_loppu())) {
                    break;
                }
            }

            ale = 10 * (rand() % 3);

            if (koko == 1) {
                hinta = 100 * yot - 0.01 * ale * 100 * yot;
            }
            else {
                hinta = 150 * yot - 0.01 * ale * 150 * yot;
            }

            std::cout << std::endl << "Varauksen esikatselu: " << std::endl << std::endl
                << "|---------------------------------------------|" << std::endl
                << "| Varaus ID: " << id << "                            |" << std::endl
                << "| Huone: " << huone << "\t\t                      |" << std::endl
                << "| Koko: " << koko << "hh                                   |" << std::endl
                << "| Varaaja: " << varaaja << std::endl
                << "| Aika: " << tp_ajaksi(alku) << " - "
                << tp_ajaksi(loppu) << "   |" << std::endl
                << "| Ale: " << ale << " %\t                              |" << std::endl
                << "| Varauksen kokonaishinta: " << hinta << " e\t\t      |" << std::endl
                << "|---------------------------------------------|" << std::endl;

            while (true) {
                std::cout << "Tallennetaanko varaus (k/e/t): ";
                input = " ";
                std::cin >> input;

                if (input == "e" || input == "E") {
                    return;
                }
                if (input == "t" || input == "T") {
                    askel--;
                    break;
                }
                if (input == "k" || input == "K") {
                    varaukset.insert(varaukset.begin() + pos, varaus(huone, id, ale, varaaja, alku, loppu));
                    varauksien_tallennus(varaukset);
                    return;
                }
            }
            break;
        }
    }
}

// Funktio, jolla printataan huonelista
void huoneet_print() {
    system("CLS");
    for (int x = 0; x < huoneet.size(); x++) {
        std::cout << "Huonenro " << huoneet[x].get_huonenro()
            << " (" << huoneet[x].get_koko()
            << "hh): ";
        if (huoneet[x].get_varattu()) {
            std::cout << "Varaus " << huoneet[x].get_varausnro() <<
                ", nimella " << huoneet[x].get_varaaja()
                << " ajalle ";
            for (int y = 0; y < varaukset.size(); y++) {
                if (huoneet[x].get_varausnro() == varaukset[y].get_id()) {
                    std::cout << tp_ajaksi(varaukset[y].get_alku()) << " - "
                        << tp_ajaksi(varaukset[y].get_loppu()) << std::endl;
                    break;
                }
            }
        }
        else {
            std::cout << "Ei aktiivista varausta" << std::endl;
        }
    }
}

// Funktio, joka sis‰lt‰‰ menun, jossa huoneita voidaan lis‰t‰ tai v‰hent‰‰
void huoneiden_muokkaus() {
    int askel = 0;
    std::string input;
    int luku = 0;
    int kohta = 0;
    while (true) {
        switch (askel) {
        case 0:
            char valinta;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            huoneet_print();
            std::cout << std::endl << std::endl << "Halutaanko huoneita lisata vai poistaa?" << std::endl
                << "Lisaa/Poista/Kumoa (l/p/k): ";
            std::cin >> valinta;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
                continue;
            }
            if (toupper(valinta) == 'L') {
                askel = 1;
            }
            if (toupper(valinta) == 'P') {
                askel = 2;
            }
            if (toupper(valinta) == 'K') {
                return;
            }
            break;
        case 1:
            input = "";
            luku = 0;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << "Paljonko huoneita lisataan (k: Kumoa): ";
            getline(std::cin, input);
            if (input == "k" || input == "K") {
                return;
            }
            try {
                luku = stoi(input);
            }
            catch (const std::invalid_argument&) {
                std::cout << "Virhe: Syote ei ollut luku." << std::endl;
                break;
            }
            if (luku >= 2 && luku % 2 == 0 && huoneet.size() + luku <= 300) {
                kohta = huoneet.size() / 2;
                for (int x = 0; x < luku / 2; x++) {
                    huoneet.insert(huoneet.begin() + kohta, huone(kohta + 1, 1, 0, "-", 0));
                    kohta++;
                }
                while (kohta < huoneet.size()) {
                    for (int x = 0; x < varaukset.size(); x++) {
                        if (varaukset[x].get_huone() == huoneet[kohta].get_huonenro()) {
                            varaukset[x].set_huone(kohta + 1);
                        }
                    }
                    huoneet[kohta].set_huonenro(kohta + 1);
                    kohta++;
                }
                for (int x = 0; x < luku / 2; x++) {
                    huoneet.push_back(huone(kohta + 1, 2, 0, "-", 0));
                    kohta++;
                }
                huoneiden_tallennus(huoneet);
                varauksien_tallennus(varaukset);
                askel = 3;
            }
            else {
                std::cout << "Huoneita ei saa olla yli 300." << std::endl;
            }
            break;
        case 2:
            input = "";
            luku = 0;
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << std::endl << "Paljonko huoneita poistetaan? (k: Kumoa): ";
            getline(std::cin, input);
            if (input == "k" || input == "K") {
                return;
            }
            try {
                luku = stoi(input);
            }
            catch (const std::invalid_argument&) {
                std::cout << "Virhe: Syote ei ollut luku." << std::endl;
                break;
            }
            if (luku >= 2 && luku % 2 == 0 && huoneet.size() - luku >= 40) {
                std::vector<int> ristiriitaiset_varaukset;
                kohta = huoneet.size() / 2 - 1;
                for (int x = 0; x < luku / 2; x++) {
                    for (int y = 0; y < varaukset.size(); y++) {
                        if (huoneet[kohta].get_huonenro() == varaukset[y].get_huone()) {
                            ristiriitaiset_varaukset.push_back(kohta);
                        }
                    }
                    kohta--;
                }
                kohta = huoneet.size() - 1;
                for (int x = 0; x < luku / 2; x++) {
                    for (int y = 0; y < varaukset.size(); y++) {
                        if (huoneet[kohta].get_huonenro() == varaukset[y].get_huone()) {
                            ristiriitaiset_varaukset.push_back(kohta);
                        }
                    }
                    kohta--;
                }

                if (ristiriitaiset_varaukset.size() > 0) {
                    char valinta2;
                    std::cout << "Ristiriitaisia varauksia loydettiin " << ristiriitaiset_varaukset.size() << std::endl << std::endl;

                    for (int x = 0; x < ristiriitaiset_varaukset.size(); x++) {
                        int koko = huoneet[varaukset[ristiriitaiset_varaukset[x]].get_huone() - 1].get_koko();
                        int hinta = 0;
                        int yot = std::chrono::duration_cast<std::chrono::hours>(varaukset[ristiriitaiset_varaukset[x]].get_loppu() - varaukset[ristiriitaiset_varaukset[x]].get_alku()).count() / 24;
                        if (koko == 1) {
                            hinta = 100 * yot - 0.01 * varaukset[ristiriitaiset_varaukset[x]].get_ale() * 100 * yot;
                        }
                        else {
                            hinta = 150 * yot - 0.01 * varaukset[ristiriitaiset_varaukset[x]].get_ale() * 150 * yot;
                        }
                        std::cout << "|---------------------------------------------|" << std::endl
                            << "| ID: " << varaukset[ristiriitaiset_varaukset[x]].get_id() << "                                   |" << std::endl
                            << "| Huone " << varaukset[x].get_huone()
                            << " (" << koko << "hh)\t                      |" << std::endl
                            << "| Varaaja: " << varaukset[ristiriitaiset_varaukset[x]].get_varaaja() << std::endl
                            << "| Aika " << tp_ajaksi(varaukset[ristiriitaiset_varaukset[x]].get_alku())
                            << " - " << tp_ajaksi(varaukset[ristiriitaiset_varaukset[x]].get_loppu()) << "    |" << std::endl
                            << "| Ale: " << varaukset[ristiriitaiset_varaukset[x]].get_ale() << " %\t                              |" << std::endl
                            << "| Kokonaishinta: " << hinta << " e\t                      |" << std::endl
                            << "|---------------------------------------------|" << std::endl
                            << std::endl;
                    }

                    while (true) {
                        std::cout << "Siirretaanko vai poistetaanko varaukset vai kumotaanko muokkaus (s/p/c): ";
                        std::cin >> valinta2;

                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore();
                            continue;
                        }
                        if (toupper(valinta2) == 'S' ||
                            toupper(valinta2) == 'P' ||
                            toupper(valinta2) == 'C') {
                            break;
                        }
                    }

                    if (toupper(valinta2) == 'S') {
                        for (int x = 0; x < ristiriitaiset_varaukset.size(); x++) {
                            int nykyinenhuone = ristiriitaiset_varaukset[x];
                            int koko = huoneet[varaukset[nykyinenhuone].get_huone() - 1].get_koko();
                            bool huoneloydetty = false;

                            for (int y = 0; y < huoneet.size(); y++) {
                                if (huoneet[y].get_koko() == koko) {
                                    bool ei_varattu = true;

                                    for (int z = 0; z < varaukset.size(); z++) {
                                        if (varaukset[z].get_huone() == huoneet[y].get_huonenro() &&
                                            varaukset[z].get_alku() < varaukset[nykyinenhuone].get_loppu() &&
                                            varaukset[z].get_loppu() > varaukset[nykyinenhuone].get_alku()) {
                                            ei_varattu = false;
                                            break;
                                        }
                                    }

                                    if (ei_varattu) {
                                        varaukset[nykyinenhuone].set_huone(huoneet[y].get_huonenro());
                                        huoneloydetty = true;
                                        break;
                                    }
                                }
                            }

                            if (!huoneloydetty) {
                                std::cout << "Ei loytynyt sopivaa huonetta varaukselle ID "
                                    << varaukset[nykyinenhuone].get_id() << std::endl;
                            }
                        }
                    }

                    if (valinta2 == 'P') {
                        for (int x = 0; x < ristiriitaiset_varaukset.size(); x++) {
                            varaukset.erase(varaukset.begin() + ristiriitaiset_varaukset[x]);
                        }
                    }

                    if (valinta2 == 'C') {
                        std::cout << "Muokkaus peruutettu." << std::endl;
                        askel = 0;
                        break;
                    }

                    kohta = huoneet.size() / 2 - 1;

                    for (int x = 0; x < luku / 2; x++) {
                        huoneet.erase(huoneet.begin() + kohta);
                        kohta--;
                    }
                    kohta++;

                    while (kohta < huoneet.size()) {
                        huoneet[kohta].set_huonenro(kohta + 1);
                        kohta++;
                    }

                    kohta = huoneet.size() - 1;

                    for (int x = 0; x < luku / 2; x++) {
                        huoneet.erase(huoneet.begin() + kohta);
                        kohta--;
                    }

                    varauksien_tallennus(varaukset);
                    huoneiden_tallennus(huoneet);

                    askel = 3;
                }
                else {
                    kohta = huoneet.size() / 2 - 1;

                    for (int x = 0; x < luku / 2; x++) {
                        huoneet.erase(huoneet.begin() + kohta);
                        kohta--;
                    }
                    kohta++;

                    while (kohta < huoneet.size()) {
                        huoneet[kohta].set_huonenro(kohta + 1);
                        kohta++;
                    }

                    kohta = huoneet.size() - 1;

                    for (int x = 0; x < luku / 2; x++) {
                        huoneet.erase(huoneet.begin() + kohta);
                        kohta--;
                    }

                    varauksien_tallennus(varaukset);
                    huoneiden_tallennus(huoneet);

                    askel = 3;
                }
            }
            else {
                std::cout << "Huoneita saa olla vahintaan 40" << std::endl;
            }
            break;
        case 3:
            char valinta3;
            std::cout << "Palataanko valikkoon vai jatketaanko muokkauksia (v/j): ";
            std::cin >> valinta3;
            if (std::cin.fail()) {
                std::cin.clear();
                std::cin.ignore();
                continue;
            }
            if (toupper(valinta3) == 'V') {
                return;
            }
            if (toupper(valinta3) == 'J') {
                askel = 0;
            }
            break;
        }
    }
}

// Valikkofunktio, josta p‰‰see huonelistaan tai huoneiden muokkaukseen
void huonevalikko() {
    char valinta;
    while (true) {
        system("CLS");
        varaukset_update();
        huoneet_update();
        std::cout << "|------------------------------|" << std::endl
            << "|                              |" << std::endl
            << "|     Mita halutaan tehda?     |" << std::endl
            << "|                              |" << std::endl
            << "|     l: Huonelista            |" << std::endl
            << "|     m: Muokkaa huoneita      |" << std::endl
            << "|     t: Takaisin              |" << std::endl
            << "|                              |" << std::endl
            << "| " << tp_ajaksi(std::chrono::system_clock::now())
            << "             |" << std::endl
            << "|------------------------------|" << std::endl << std::endl;

        while (true) {
            std::cout << "Valinta: ";
            std::cin >> valinta;
            if (std::cin.fail()
                || toupper(valinta) != 'L'
                && toupper(valinta) != 'M'
                && toupper(valinta) != 'T') {
                std::cin.clear();
                std::cin.ignore();
                continue;
            }
            break;
        }

        if (toupper(valinta) == 'L') {
            huoneet_print();
            char palaa;
            std::cout << "Syota k palataksesi valikkoon: ";
            std::cin >> palaa;
            if (toupper(palaa) != 'K') {
                break;
            }
        }
        else if (toupper(valinta) == 'M') {
            huoneiden_muokkaus();
        }
        else if (toupper(valinta) == 'T') {
            return;
        }
    }
}

// Funktio, jolla p‰‰st‰‰n menuun, josta voi hakea varauksia ID:ll‰ tai varaajan nimell‰
void varauksen_haku() {
    int askel = 0;
    std::string input;
    system("CLS");

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    std::cout << "|------------------------------------------------------|" << std::endl
              << "| Hae varausta ID:lla tai varaajan nimella. (k: kumoa) |" << std::endl
              << "|------------------------------------------------------|" << std::endl << std::endl;
    while (true) {
        switch (askel) {
        case 0:
            while (true) {
                input = "";
                std::cout << "Haku: ";
                getline(std::cin, input);
                break;
            }
            if (input == "k" || input == "K") {
                return;
            }
            askel++;
            break;
        case 1:
            std::vector<int> vastaavat_varaukset;
            for (int x = 0; x < varaukset.size(); x++) {
                try {
                    if (stoi(input) == varaukset[x].get_id()) {
                        vastaavat_varaukset.push_back(x);
                        break;
                    }
                }
                catch (const std::invalid_argument&) {
                    // Ei mit‰‰n
                }

                std::string varaaja = varaukset[x].get_varaaja();
                std::string hakusana = input;

                std::transform(varaaja.begin(), varaaja.end(), varaaja.begin(), ::tolower);
                std::transform(hakusana.begin(), hakusana.end(), hakusana.begin(), ::tolower);

                if (varaaja.find(hakusana) != std::string::npos) {
                    vastaavat_varaukset.push_back(x);
                }
            }
            if (vastaavat_varaukset.size() == 0) {
                std::cout << "Varauksia ei loytynyt." << std::endl << std::endl;
                askel--;
                break;
            }

            std::cout << "Varauksia loydettiin " << vastaavat_varaukset.size() << std::endl << std::endl;

            for (int x = 0; x < vastaavat_varaukset.size(); x++) {
                int koko = huoneet[varaukset[vastaavat_varaukset[x]].get_huone() - 1].get_koko();
                int hinta = 0;
                int yot = std::chrono::duration_cast<std::chrono::hours>(varaukset[vastaavat_varaukset[x]].get_loppu()
                    - varaukset[vastaavat_varaukset[x]].get_alku()).count() / 24;
                if (koko == 1) {
                    hinta = 100 * yot - 0.01 * varaukset[vastaavat_varaukset[x]].get_ale() * 100 * yot;
                }
                else {
                    hinta = 150 * yot - 0.01 * varaukset[vastaavat_varaukset[x]].get_ale() * 150 * yot;
                }
                std::cout << "|---------------------------------------------|" << std::endl
                    << "| ID: " << varaukset[vastaavat_varaukset[x]].get_id() << "                                   |" << std::endl
                    << "| Huone " << varaukset[vastaavat_varaukset[x]].get_huone()
                    << " (" << koko << "hh)\t                      |" << std::endl
                    << "| Varaaja: " << varaukset[vastaavat_varaukset[x]].get_varaaja() << std::endl
                    << "| Aika " << tp_ajaksi(varaukset[vastaavat_varaukset[x]].get_alku())
                    << " - " << tp_ajaksi(varaukset[vastaavat_varaukset[x]].get_loppu()) << "    |" << std::endl
                    << "| Ale: " << varaukset[vastaavat_varaukset[x]].get_ale() << " %\t                              |" << std::endl
                    << "| Kokonaishinta: " << hinta << " e\t                      |" << std::endl
                    << "|---------------------------------------------|" << std::endl
                    << std::endl;
            }
            while (true) {
                char input2;
                std::cout << std::endl << "Jatketaanko hakua vai palataanko takaisin valikkoon (h/t): ";
                std::cin >> input2;
                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore();
                    continue;
                }
                if (toupper(input2) == 'H') {
                    askel = 0;
                    break;
                }
                if (toupper(input2) == 'T') {
                    return;
                }
            }
            break;
        }
    }
}

// Funktio, joka printtaa kaikki olemassa olevat varaukset
void varaukset_print() {
    system("CLS");
    std::cout << "|------------------------------" << std::endl
        << "| Varauksia yhteensa " << varaukset.size() << std::endl
        << "|------------------------------" << std::endl << std::endl;
    for (int x = 0; x < varaukset.size(); x++) {
        int koko = huoneet[varaukset[x].get_huone() - 1].get_koko();
        int hinta = 0;
        int yot = std::chrono::duration_cast<std::chrono::hours>(varaukset[x].get_loppu() - varaukset[x].get_alku()).count() / 24;
        if (koko == 1) {
            hinta = 100 * yot - 0.01 * varaukset[x].get_ale() * 100 * yot;
        }
        else {
            hinta = 150 * yot - 0.01 * varaukset[x].get_ale() * 150 * yot;
        }
        std::cout << "|---------------------------------------------|" << std::endl
            << "| ID: " << varaukset[x].get_id() << "                                   |" << std::endl
            << "| Huone " << varaukset[x].get_huone()
            << " (" << koko << "hh)\t                      |" << std::endl
            << "| Varaaja: " << varaukset[x].get_varaaja() << std::endl
            << "| Aika " << tp_ajaksi(varaukset[x].get_alku())
            << " - " << tp_ajaksi(varaukset[x].get_loppu()) << "    |" << std::endl
            << "| Ale: " << varaukset[x].get_ale() << " %\t                              |" << std::endl
            << "| Kokonaishinta: " << hinta << " e\t                      |" << std::endl
            << "|---------------------------------------------|" << std::endl
            << std::endl;
    }
}

// Funktio, jolla p‰‰st‰‰n menuun, jossa voidaan poistaa varauksia, niiden ID:ll‰
void varauksen_poisto() {
    int askel = 0;
    std::string input;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    while (true) {
        switch (askel) {
        case 0:
            varaukset_print();
            input = "";
            std::cout << "|--------------------------------------------|" << std::endl
                      << "| Syota poistettavan varauksen ID (k: kumoa) |" << std::endl
                      << "|--------------------------------------------|" << std::endl 
                      << std::endl << "ID: ";
            getline(std::cin, input);

            if (input == "k" || input == "K") {
                return;
            }

            try {
                std::stoi(input);
                askel = 1;
            }
            catch (const std::invalid_argument&) {
                std::cout << "Virheellinen syote. Yrita uudelleen." << std::endl;
            }
            break;
        case 1:
        {
            bool onnistui = false;
            int id = stoi(input);
            for (int x = 0; x < varaukset.size(); x++) {
                if (id == varaukset[x].get_id()) {
                    varaukset.erase(varaukset.begin() + x);
                    onnistui = true;
                    std::cout << "Varaus ID:lla " << input << " poistettu." << std::endl << std::endl;
                    varauksien_tallennus(varaukset);
                    askel = 2;
                    break;
                }
            }
            if (!onnistui) {
                std::cout << "Varausta ei loytynyt. Yrita uudelleen." << std::endl << std::endl;
                askel = 0;
            }
        }
        break;
        case 2:
            char valinta;
            std::cout << "Jatketaanko poistamista vai palataanko valikkoon? (j/m): ";
            std::cin >> valinta;

            if (toupper(valinta) == 'J') {
                askel = 0;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            else if (toupper(valinta) == 'M') {
                return;
            }
            else {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
            break;
        }
    }
}

// Valikkofunktio, josta p‰‰st‰‰n varauksiin liittyviin menuihin
void varausvalikko() {
    char valinta;
    while (true) {
        system("CLS");
        varaukset_update();
        huoneet_update();
        std::cout << "|------------------------------|" << std::endl
            << "|                              |" << std::endl
            << "|     Mita halutaan tehda?     |" << std::endl
            << "|                              |" << std::endl
            << "|     l: Varauslista           |" << std::endl
            << "|     v: Varauksenteko         |" << std::endl
            << "|     h: Varauksenhaku         |" << std::endl
            << "|     p: Poista varauksia      |" << std::endl
            << "|     t: Takaisin              |" << std::endl
            << "|                              |" << std::endl
            << "| " << tp_ajaksi(std::chrono::system_clock::now())
            << "             |" << std::endl
            << "|------------------------------|" << std::endl << std::endl;

        while (true) {
            std::cout << "Valinta: ";
            std::cin >> valinta;
            if (std::cin.fail()
                || toupper(valinta) != 'L'
                && toupper(valinta) != 'V'
                && toupper(valinta) != 'H'
                && toupper(valinta) != 'P'
                && toupper(valinta) != 'T') {
                std::cin.clear();
                std::cin.ignore();
                continue;
            }
            break;
        }

        if (toupper(valinta) == 'L') {
            varaukset_print();
            char palaa;
            std::cout << "Syota k palataksesi valikkoon: ";
            std::cin >> palaa;
            if (toupper(palaa) != 'K') {
                break;
            }
        }
        else if (toupper(valinta) == 'V') {
            varauksen_teko();
        }
        else if (toupper(valinta) == 'H') {
            varauksen_haku();
        }
        else if (toupper(valinta) == 'P') {
            varauksen_poisto();
        }
        else if (toupper(valinta) == 'T') {
            return;
        }

    }
}

// P‰‰valikkofunktio, josta p‰‰st‰‰n huonevalikkoon tai varausvalikkoon, voidaan myˆs nollata ohjelma kokonaan
void paavalikko() {
    char valinta;
    while (true) {
        system("CLS");
        varaukset_update();
        huoneet_update();
        std::cout << "|------------------------------|" << std::endl
            << "|                              |" << std::endl
            << "|     Mita halutaan tehda?     |" << std::endl
            << "|                              |" << std::endl
            << "|     h: Huoneet               |" << std::endl
            << "|     v: Varaukset             |" << std::endl
            << "|     0: Nollaa ohjelma        |" << std::endl
            << "|                              |" << std::endl
            << "| " << tp_ajaksi(std::chrono::system_clock::now())
            << "             |" << std::endl
            << "|------------------------------|" << std::endl;

        while (true) {
            std::cout << std::endl << "Valinta: ";
            std::cin >> valinta;
            if (std::cin.fail()
                || toupper(valinta) != 'H'
                && toupper(valinta) != 'V'
                && valinta != '0') {
                std::cin.clear();
                std::cin.ignore();
                continue;
            }
            break;
        }

        if (toupper(valinta) == 'H') {
            huonevalikko();
        }
        else if (toupper(valinta) == 'V') {
            varausvalikko();
        }
        else if (valinta == '0') {
            char valinta2;
            std::cout << "Oletko varma, etta haluat jatkaa?" << std::endl
                << "Tama poistaa kaikki huoneet ja varaukset." << std::endl;
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            while (true) {
                std::cout << "Kylla/Ei (k / e): ";
                std::cin >> valinta2;
                if (std::cin.fail() ||
                    toupper(valinta2) != 'K'
                    && toupper(valinta2) != 'E') {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }
                if (toupper(valinta2) == 'K') {
                    varaukset.clear();
                    huoneet.clear();
                    varauksien_tallennus(varaukset);
                    huoneiden_tallennus(huoneet);
                    system("CLS");
                    std::cout << "Huoneita ei ole viela luotu. Luodaan huoneet:" << std::endl;
                    huoneiden_luonti();
                    break;
                }
                if (toupper(valinta2) == 'E') {
                    break;
                }
            }
        }
    }
}

// ||\          /||    //\\    ||||||  ||\     ||
// ||\\        //||   //  \\     ||    ||\\    ||
// || \\      // ||  ||    ||    ||    || \\   ||
// ||  \\    //  ||  ||====||    ||    ||  \\  ||
// ||   \\  //   ||  ||    ||    ||    ||   \\ ||
// ||    \\//    ||  ||    ||    ||    ||    \\||
// ||     \/     ||  ||    ||  ||||||  ||     \||

int main() {

    // Ladataan varaukset ja huoneet
    varaukset = varauksien_lataus();
    huoneet = huoneiden_lataus();

    // Jos olemassa ei ole huoneita, luodaan ne
    if (huoneet.size() == 0) {
        std::cout << "Huoneita ei ole viela luotu. Luodaan huoneet:" << std::endl;
        huoneiden_luonti();
    }

    // Menn‰‰n p‰‰valikkoon
    paavalikko();

    return 0;
}