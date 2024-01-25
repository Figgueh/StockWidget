#include <fstream>

#include "ConfigHandler.h"

std::wstring ConfigHandler::getRefreshToken() {
	std::wifstream config;
	std::wstring token;
	config.open("config.txt", std::ios::in);
	if (config.is_open()) {
		while (config) {
			std::getline(config, token);
		}
	}
	else {
		//TODO:: Throw proper error
		//std::cout << "Couldn't open file\n";
	}

	return token;
}

void ConfigHandler::updateRefreshToken(std::string token) {
	std::ofstream config("config.txt");
	config << token;
	config.close();
}
