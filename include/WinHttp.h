#pragma once
#include <windows.h>
#include <winhttp.h>
#include <string>
#include <map>


class WinHttp
{
public:
    ~WinHttp();
    void open();
    void connect(LPCWSTR host);
    void requestHandler(LPCWSTR verb, LPCWSTR path);
    void addHeaders(std::map<std::string, std::string> headers);
    void sendRequest();
    std::wstring recieveResponse();

    static std::wstring stripHost(const std::string& host);

private:
    HINTERNET m_session = NULL;
    HINTERNET m_connect = NULL;
    BOOL m_result = NULL;
    HINTERNET m_request = NULL;
};

