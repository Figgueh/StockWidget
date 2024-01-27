#include "Toolbox.h"

std::wstring toWString(std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

std::string toString(std::wstring& str)
{
	return std::string(str.begin(), str.end());
}

