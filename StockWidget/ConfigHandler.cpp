#include "ConfigHandler.h"
#include "Toolbox.h"

std::wstring stripToken(std::wstring currentLine)
{
	return currentLine.erase(0, 14);
}

bool exists() {
	struct stat buffer;
	return (stat(ConfigHandler::FILENAME.c_str(), &buffer) == 0);
}

ConfigHandler::ConfigHandler()
{
	//if (!exists()) {
	//	m_fileHandle.open(FILENAME, std::ios::out);
	//	m_fileHandle << "RefreshToken: \n";
	//	m_fileHandle << "Watchlist: \n";
	//	m_fileHandle.close();
	//}
}

std::wstring ConfigHandler::getRefreshToken() 
{
	std::wstring currentLine;
	std::wifstream fileHandle{};


	fileHandle.open(FILENAME, std::ios::in);
	if (fileHandle.is_open()) {
		while (std::getline(fileHandle, currentLine)) {
			
			if (currentLine.find(L"RefreshToken: ") != std::string::npos) {
				return stripToken(currentLine);
			}
		}
	}
	fileHandle.close();

	return L"";
}

void ConfigHandler::updateRefreshToken(std::string token) 
{
	std::vector<std::wstring> fileData = readConfig();

	fileData[0] = L"RefreshToken: " + toWString(token) + L"\n";

	writeConfig(fileData);
}

std::vector<int> ConfigHandler::getTickers()
{
	std::vector<int> watchList;
	std::vector<std::wstring> fileData = readConfig();

	fileData.erase(fileData.begin(), fileData.begin() + 2);

	for (std::wstring ticker : fileData) {
		watchList.emplace_back(stoi(ticker));
	}

	return watchList;
}

void ConfigHandler::updateTickers(std::vector<int> tickerIDs)
{
	std::vector<std::wstring> fileData = readConfig();

	fileData.erase(fileData.begin() + 2, fileData.end());

	for (int ticker : tickerIDs) {
		fileData.emplace_back(std::to_wstring(ticker) + L"\n");
	}

	writeConfig(fileData);
}

std::vector<std::wstring> ConfigHandler::readConfig()
{
	std::vector<std::wstring> fileData;
	std::wstring currentLine;
	std::wifstream fileHandle;

	fileHandle.open(FILENAME);
	if (fileHandle.is_open()) {
		while (std::getline(fileHandle, currentLine)) {
			fileData.emplace_back(currentLine + L"\n");
		}
	}
	fileHandle.close();

	return fileData;
}

void ConfigHandler::writeConfig(std::vector<std::wstring> fileData)
{
	std::wofstream fileHandle{};
	fileHandle.open(FILENAME);
	if (fileHandle.is_open()) {
		for (std::wstring line : fileData) {
			fileHandle << line;
		}
	}
	fileHandle.close();
}

