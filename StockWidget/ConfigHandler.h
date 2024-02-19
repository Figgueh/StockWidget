#pragma once
#include <string>
#include <vector>
#include <fstream>

class ConfigHandler
{
public:
	ConfigHandler();
	std::wstring getRefreshToken();
	void updateRefreshToken(std::string token);
	std::vector<int> getTickers();
	void updateTickers(std::vector<int> tickerIDs);

	inline static const std::string FILENAME = "config.txt";

private:
	std::vector<std::wstring> readConfig();
	void writeConfig(std::vector<std::wstring> fileData);
};

