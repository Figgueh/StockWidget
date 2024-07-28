#include "model/ConfigHandler.h"
#include "utility/Toolbox.h"

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

WINDOWPLACEMENT ConfigHandler::getPosition()
{
	std::vector<std::wstring> fileData = readConfig();

	// Get the position of where the coordinates are
	ptrdiff_t pos = (std::find(fileData.begin(), fileData.end(), L"Position:\n") - fileData.begin() + 1);

	std::wstring positioning = fileData[pos];
	std::vector<std::wstring> coords = split(positioning, ",");

	WINDOWPLACEMENT windowPos;
	windowPos.length = sizeof(windowPos);
	windowPos.flags = 0;
	windowPos.showCmd = 1;
	windowPos.ptMinPosition = POINT(-1, -1);
	windowPos.ptMaxPosition = POINT(-1, -1);

	RECT window = RECT(stol(coords[0]), stol(coords[1]), stol(coords[2]), stol(coords[3]));
	windowPos.rcNormalPosition = window;
	return windowPos;
	
}

void ConfigHandler::updatePosition(WINDOWPLACEMENT& pos)
{
	std::vector<std::wstring> fileData = readConfig();

	// Get the position of where the coordinates are
	ptrdiff_t index = (std::find(fileData.begin(), fileData.end(), L"Position:\n") - fileData.begin() + 1);

	RECT window = pos.rcNormalPosition;
	std::wstring positioning =	std::to_wstring(window.left) + L"," + 
								std::to_wstring(window.top) + L","  +
								std::to_wstring(window.right) + L"," + 
								std::to_wstring(window.bottom) + L"\n";

	fileData[index] = positioning;
	writeConfig(fileData);
}

std::vector<int> ConfigHandler::getTickers()
{
	std::vector<int> watchList;
	std::vector<std::wstring> fileData = readConfig();

	// Get the position of where it lists the watchlist
	ptrdiff_t pos = (std::find(fileData.begin(), fileData.end(), L"Watchlist:\n") - fileData.begin() + 1);
	fileData.erase(fileData.begin(), fileData.begin() + pos);

	// Read untill EOF
	for (std::wstring ticker : fileData) {
		watchList.emplace_back(stoi(ticker));
	}

	return watchList;
}

void ConfigHandler::updateTickers(std::vector<int> tickerIDs)
{
	std::vector<std::wstring> fileData = readConfig();

	// Get the position of where is lists the watchlist
	ptrdiff_t pos = (std::find(fileData.begin(), fileData.end(), L"Watchlist:\n") - fileData.begin() + 1);
	fileData.erase(fileData.begin() + pos, fileData.end());

	// Replace untill EOF with new tickers
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

