// huone.cpp

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "huone.hpp"
#include <vector>
#include <stdlib.h>

// Constructori
huone::huone(int tieto0, int tieto1, int tieto2, std::string tieto3, bool tieto4) {
    huonenro = tieto0;
    koko = tieto1;
    varausnro = tieto2;
    varaaja = tieto3;
    varattu = tieto4;
}

// Funktiolla saadaan huoneen tallennettavat tiedot stringiksi
std::string huone::tiedot() {
    std::string tiedot = std::to_string(huonenro) + "," + std::to_string(koko);
    return tiedot;
}

void huone::set_huonenro(int luku) {
    huonenro = luku;
}

void huone::set_varausnro(int luku) {
    varausnro = luku;
}

void huone::set_varaaja(std::string nimi) {
    varaaja = nimi;
}

void huone::set_varattu(bool onko) {
    varattu = onko;
}

int huone::get_huonenro() {
    return huonenro;
}

int huone::get_koko() {
    return koko;
}

int huone::get_varausnro() {
    return varausnro;
}

std::string huone::get_varaaja() {
    return varaaja;
}

bool huone::get_varattu() {
    return varattu;
}