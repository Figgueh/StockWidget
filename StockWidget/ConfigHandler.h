#pragma once
#include <string>

class ConfigHandler
{
public:
	static std::wstring getRefreshToken();
	static void updateRefreshToken(std::string);
};

