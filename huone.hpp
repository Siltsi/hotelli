// huone.hpp

#pragma once

#include <iostream>
#include <vector>

class huone {
    int huonenro;
    int koko = 1;
    int varausnro = 0;
    std::string varaaja = "-";
    bool varattu = 0;
public:
    huone(int huonenro, int koko, int varausnro, std::string varaaja, bool varattu);
    std::string tiedot();
    void set_huonenro(int luku);
    void set_varausnro(int luku);
    void set_varaaja(std::string nimi);
    void set_varattu(bool onko);
    int get_huonenro();
    int get_koko();
    int get_varausnro();
    std::string get_varaaja();
    bool get_varattu();
};