#include <codecvt>
#include <locale>

#include "Utilities.h"

std::wstring to_wstring(const std::string& stringToConvert)
{
    std::wstring wideString = std::wstring_convert<std::codecvt_utf8<wchar_t>>().from_bytes(stringToConvert);
    return wideString;
}
