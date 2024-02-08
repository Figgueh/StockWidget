#pragma once
#include <string>
#include <vector>
#include <fstream>

class ConfigHandler
{
public:
	std::wstring getRefreshToken();
	void updateRefreshToken(std::string);
	std::vector<int> getTickers();
	void setupConfig();

private:
	std::vector<std::wstring> readConfig();
	void writeConfig(std::vector<std::wstring> fileData);

	std::string m_fileName = "config2.txt";
	std::wfstream m_fileHandle{};

};

