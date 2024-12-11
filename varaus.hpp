// varaus.hpp

#pragma once

#include <iostream>
#include <chrono>

class varaus {
	int huone;
	int id;
	int ale;
	std::string varaaja;
	std::chrono::time_point<std::chrono::system_clock> alku;
	std::chrono::time_point<std::chrono::system_clock> loppu;
public:
	varaus(int luku, int luku2, int luku3, std::string nimi,
		std::chrono::time_point<std::chrono::system_clock> aika,
		std::chrono::time_point<std::chrono::system_clock> aika2);
	void set_huone(int luku);
	void set_id(int luku);
	void set_ale(int luku);
	void set_varaaja(std::string nimi);
	void set_alku(std::chrono::time_point<std::chrono::system_clock> aika);
	void set_loppu(std::chrono::time_point<std::chrono::system_clock> aika);
	int get_huone();
	int get_id();
	int get_ale();
	std::string get_varaaja();
	std::chrono::time_point<std::chrono::system_clock> get_alku();
	std::chrono::time_point<std::chrono::system_clock> get_loppu();
	std::string tiedot();
};