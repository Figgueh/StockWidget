#pragma once
#include <string>
#include <vector>


std::wstring const toWString(const std::string& str);
std::string const toString(const std::wstring& str);
std::vector<int> split(std::wstring s, std::string delimiter);
//std::vector<int> splitOpt(const std::wstring& s, const std::string delimiter);


