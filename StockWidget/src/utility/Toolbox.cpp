#include "utility/Toolbox.h"

std::wstring const toWString(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

std::string const toString(const std::wstring& str)
{
	return std::string(str.begin(), str.end());
}

std::vector<std::wstring> split(std::wstring s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::wstring token;
    std::vector<std::wstring> res;

    while ((pos_end = s.find(toWString(delimiter), pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(token);
    }

    res.push_back(s.substr(pos_start));
    return res;
}
