#include "utility/Toolbox.h"

std::wstring const toWString(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

std::string const toString(const std::wstring& str)
{
	return std::string(str.begin(), str.end());
}

