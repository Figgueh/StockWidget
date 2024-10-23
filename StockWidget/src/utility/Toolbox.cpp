#include "utility/Toolbox.h"

std::wstring const toWString(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}

std::string const toString(const std::wstring& str)
{
	return std::string(str.begin(), str.end());
}

std::vector<int> split(std::wstring s, std::string delimiter) {
    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
    std::wstring token;
    std::vector<int> res;

    while ((pos_end = s.find(toWString(delimiter), pos_start)) != std::string::npos) {
        token = s.substr(pos_start, pos_end - pos_start);
        pos_start = pos_end + delim_len;
        res.push_back(stol(token));
    }

    res.push_back(stol(s.substr(pos_start)));
    return res;
}

//std::vector<std::wstring> split(std::wstring s, std::string delimiter) {
//    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
//    std::wstring token;
//    std::vector<std::wstring> res;
//
//    while ((pos_end = s.find(toWString(delimiter), pos_start)) != std::string::npos) {
//        token = s.substr(pos_start, pos_end - pos_start);
//        pos_start = pos_end + delim_len;
//        res.push_back(token);
//    }
//
//    res.push_back(s.substr(pos_start));
//    return res;
//}


//template <class T> 
//std::vector<class T> split(std::wstring s, std::string delimiter) {
//    size_t pos_start = 0, pos_end, delim_len = delimiter.length();
//    std::wstring token;
//    std::vector<T> res;
//
//    while ((pos_end = s.find(toWString(delimiter), pos_start)) != std::string::npos) {
//        token = s.substr(pos_start, pos_end - pos_start);
//        pos_start = pos_end + delim_len;
//        res.push_back(token);
//    }
//
//    res.push_back(s.substr(pos_start));
//    return res;
//}


//std::vector<int> splitOpt(const std::wstring& str, const std::string delim)
//{
//	std::vector<int> result;
//	int index = 0;
//    std::string buff = "";
//
//	while(index <= str.size())
//	{
//		if (str[index] == delim[0])
//		{
//			buff.append(std::to_string(str[index]));
//		}
//		else
//		{
//			result.emplace_back(std::stoi(buff));
//			buff = "";
//		}
//		index++;
//	}
//
//	return result;
//}

