// varaus.cpp

#include "varaus.hpp"
#include <iostream>
#include <chrono>
#include <string>

// Constructori
varaus::varaus(int luku, int luku2, int luku3, std::string nimi,
	std::chrono::time_point<std::chrono::system_clock> aika,
	std::chrono::time_point<std::chrono::system_clock> aika2) {
	huone = luku;
	id = luku2;
	ale = luku3;
	varaaja = nimi;
	alku = aika;
	loppu = aika2;
}

void varaus::set_id(int luku) {
	id = luku;
}

void varaus::set_huone(int luku) {
	huone = luku;
}

void varaus::set_ale(int luku) {
	ale = luku;
}

void varaus::set_varaaja(std::string nimi) {
	varaaja = nimi;
}

void varaus::set_alku(std::chrono::time_point<std::chrono::system_clock> aika) {
	alku = aika;
}

void varaus::set_loppu(std::chrono::time_point<std::chrono::system_clock> aika) {
	loppu = aika;
}

int varaus::get_id() {
	return id;
}

int varaus::get_huone() {
	return huone;
}

int varaus::get_ale() {
	return ale;
}

std::string varaus::get_varaaja() {
	return varaaja;
}

std::chrono::time_point<std::chrono::system_clock> varaus::get_alku() {
	return alku;
}

std::chrono::time_point<std::chrono::system_clock> varaus::get_loppu() {
	return loppu;
}

// Funktiolla saadaan varauksen tallennettavat tiedot stringiksi
std::string varaus::tiedot() {
	std::string tiedot = std::to_string(huone) + "," +
		std::to_string(id) + "," +
		std::to_string(ale) + "," +
		varaaja + "," +
		std::to_string(std::chrono::duration_cast<std::chrono::minutes>(alku.time_since_epoch()).count()) + "," +
		std::to_string(std::chrono::duration_cast<std::chrono::minutes>(loppu.time_since_epoch()).count());

	return tiedot;
}