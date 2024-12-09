#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

#include "model/ApplicationSettings.h"

class ConfigHandler
{
public:
	ConfigHandler(ConfigHandler& other) = delete;
	void operator=(const ConfigHandler& other) = delete;

	static ConfigHandler* getInstance();

	inline static const std::string FILENAME = "config.txt";
	bool m_firstStart = FALSE;

	~ConfigHandler();

	std::wstring getRefreshToken();
	std::wstring getSecretKey();
	void updateRefreshToken(std::string token);
	void updateSecretKey(std::string token);

	ApplicationSettings getSettings();
	void updateSettings(ApplicationSettings settings);

	WINDOWPLACEMENT getPosition();
	void updatePosition(WINDOWPLACEMENT& pos);

	std::vector<std::string> getTickers();
	void updateTickers(std::vector<std::string> tickerIDs);

	void writeConfig();


protected:
	ConfigHandler();

	inline static ConfigHandler* m_configurationInstance;
	std::vector<std::wstring> m_toWriteBuffer;

	int findLineNumber(std::wstring line);
	std::vector<std::wstring> readConfig();
};

