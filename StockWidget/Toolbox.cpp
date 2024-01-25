#include "Toolbox.h"

std::wstring toWString(std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

