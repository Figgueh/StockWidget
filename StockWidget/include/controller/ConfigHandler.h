#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

#include "model/ApplicationSettings.h"

class ConfigHandler
{
public:
	inline static const std::string FILENAME = "config.txt";

	ConfigHandler();
	~ConfigHandler();
	std::wstring getRefreshToken();
	void updateRefreshToken(std::string token);

	ApplicationSettings getSettings();
	void updateSettings(ApplicationSettings settings);

	WINDOWPLACEMENT getPosition();
	void updatePosition(WINDOWPLACEMENT& pos);

	std::vector<int> getTickers();
	void updateTickers(std::vector<int> tickerIDs);

private:
	std::vector<std::wstring> m_toWriteBuffer;

	int findLineNumber(std::wstring line);
	std::vector<std::wstring> readConfig();
	void writeConfig(std::vector<std::wstring> fileData);
};

