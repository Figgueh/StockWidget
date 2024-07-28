#pragma once
#include <string>
#include <vector>

std::wstring const toWString(const std::string& str);
std::string const toString(const std::wstring& str);
std::vector<std::wstring> split(std::wstring s, std::string delimiter);
