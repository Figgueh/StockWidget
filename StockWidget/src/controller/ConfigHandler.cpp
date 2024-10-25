#include "controller/ConfigHandler.h"
#include "utility/Toolbox.h"

bool exists() {
	struct stat buffer;
	return (stat(ConfigHandler::FILENAME.c_str(), &buffer) == 0);
}

ConfigHandler::ConfigHandler()
{
	if (!exists()) {
		m_toWriteBuffer = { 
			L"RefreshToken:",
			L"",
			L"Settings:",
			L"0,0",
			L"Position:", 
			L"0,0,215,20", 
			L"Watchlist:"
		};
	} else {
		m_toWriteBuffer = readConfig();
	}
}

ConfigHandler::~ConfigHandler()
{
	writeConfig(m_toWriteBuffer);
}

int ConfigHandler::findLineNumber(std::wstring line)
{
	std::vector<std::wstring>::iterator it = std::find(m_toWriteBuffer.begin(), m_toWriteBuffer.end(), line);
	if (it != m_toWriteBuffer.end())
	{
		return std::next(it) - m_toWriteBuffer.begin();
	}
	else {
		//Throw error
	}
}

std::wstring ConfigHandler::getRefreshToken() 
{
	return m_toWriteBuffer[findLineNumber(L"RefreshToken:")];
}

void ConfigHandler::updateRefreshToken(std::string token) 
{
	m_toWriteBuffer[findLineNumber(L"RefreshToken:")] = toWString(token) + L"";
}

ApplicationSettings ConfigHandler::getSettings()
{
	std::wstring data = m_toWriteBuffer[findLineNumber(L"Settings:")];
	std::vector<int> dataSplit = split(data, ",");
	return ApplicationSettings(dataSplit[0], dataSplit[1]);
}

void ConfigHandler::updateSettings(ApplicationSettings settings)
{
	m_toWriteBuffer[findLineNumber(L"Settings:")] = std::to_wstring(settings.alwaysOnTop) + L"," + std::to_wstring(settings.rememberLocation) + L"";
}

WINDOWPLACEMENT ConfigHandler::getPosition()
{
	std::wstring data = m_toWriteBuffer[findLineNumber(L"Position:")];
	std::vector<int> coords = split(data, ",");
	
	WINDOWPLACEMENT windowPos;
	windowPos.length = sizeof(windowPos);
	windowPos.flags = 0;
	windowPos.showCmd = 1;
	windowPos.ptMinPosition = POINT(-1, -1);
	windowPos.ptMaxPosition = POINT(-1, -1);

	RECT window = RECT(coords[0], coords[1], coords[2], coords[3]);
	windowPos.rcNormalPosition = window;

	return windowPos;
}

void ConfigHandler::updatePosition(WINDOWPLACEMENT& pos)
{
	RECT window = pos.rcNormalPosition;

	m_toWriteBuffer[findLineNumber(L"Position:")] = 
		std::to_wstring(window.left) + L"," + 
		std::to_wstring(window.top) + L","  +
		std::to_wstring(window.right) + L"," + 
		std::to_wstring(window.bottom) + L"";
}

std::vector<int> ConfigHandler::getTickers()
{
	std::vector<int> watchlist;
	for (int i = findLineNumber(L"Watchlist:"); i < m_toWriteBuffer.size(); i++){
		watchlist.emplace_back(stoi(m_toWriteBuffer[i]));
	}

	return watchlist;
}

void ConfigHandler::updateTickers(std::vector<int> tickerIDs)
{
	int dataLine = findLineNumber(L"Watchlist:");

	// If there are other tickers, remove them
	if (m_toWriteBuffer.size() > dataLine)
		m_toWriteBuffer.erase(m_toWriteBuffer.begin() + dataLine, m_toWriteBuffer.end());

	// Add all the tickers
	for (int ticker : tickerIDs) {
		m_toWriteBuffer.emplace_back(std::to_wstring(ticker));
	}
}

std::vector<std::wstring> ConfigHandler::readConfig()
{
	std::vector<std::wstring> fileData;
	std::wstring currentLine;
	std::wifstream fileHandle;

	fileHandle.open(FILENAME);
	if (fileHandle.is_open()) {
		while (std::getline(fileHandle, currentLine)) {
			fileData.emplace_back(currentLine);
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
			fileHandle << line << std::endl;
		}
	}
	fileHandle.close();
}

