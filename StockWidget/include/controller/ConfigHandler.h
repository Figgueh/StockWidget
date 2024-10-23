#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <windows.h>

#include "model/ApplicationSettings.h"

class ConfigHandler
{
public:
	inline static const std::string FILENAME = "config.txt";

	ConfigHandler();
	std::wstring getRefreshToken();
	void updateRefreshToken(std::string token);

	ApplicationSettings getSettings();
	void updateSettings(ApplicationSettings settings);

	WINDOWPLACEMENT getPosition();
	void updatePosition(WINDOWPLACEMENT& pos);

	std::vector<int> getTickers();
	void updateTickers(std::vector<int> tickerIDs);

private:
	std::vector<std::wstring> readConfig();
	void writeConfig(std::vector<std::wstring> fileData);
};

