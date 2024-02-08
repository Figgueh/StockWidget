#include "ConfigHandler.h"
#include "Toolbox.h"


std::wstring ConfigHandler::getRefreshToken() 
{
	std::wstring currentLine;
	m_fileHandle.open(m_fileName, std::ios::in);
	if (m_fileHandle.is_open()) {
		while (m_fileHandle) {
			std::getline(m_fileHandle, currentLine);

			if (currentLine.find(L"RefreshToken: ") != std::string::npos) {
				return stripToken(currentLine);
			}
		}
	}
	else {
		setupConfig();
	}

	return L"";
}

std::wstring stripToken(std::wstring currentLine)
{
	return currentLine.erase(0, 15);
}

std::vector<std::wstring> ConfigHandler::readConfig()
{
	std::vector<std::wstring> fileData;
	std::wstring currentLine;

	m_fileHandle.open(m_fileName, std::ios::in);
	if (m_fileHandle.is_open()) {
		while (m_fileHandle) {
			std::getline(m_fileHandle, currentLine);
			fileData.push_back(currentLine);
		}
	}

	return fileData;
}

void ConfigHandler::writeConfig(std::vector<std::wstring> fileData)
{
	m_fileHandle.open(m_fileName, std::ios::out);
	for (std::wstring line : fileData) {
		m_fileHandle << line;
	}
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

	fileData.erase(fileData.begin(), fileData.begin() + 1);

	for (std::wstring ticker : fileData) {
		watchList.push_back(stoi(ticker));
	}

	return watchList;
}

void ConfigHandler::setupConfig()
{
	m_fileHandle.open(m_fileName, std::ios::out);
	m_fileHandle << "RefreshToken: \n";
	m_fileHandle << "Watchlist: ";
}
