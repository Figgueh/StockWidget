#pragma once
#include <windows.h>
#include <winhttp.h>
#include <string>

class WinHttp
{
public:
    ~WinHttp();
    void Open(LPCWSTR userAgent);
    void Connect(LPCWSTR host);
    void RequestHandler(LPCWSTR verb, LPCWSTR path);
    void SendRequest(LPCWSTR headers, int length);
    std::wstring RecieveResponse();
    std::wstring ans;

private:
    HINTERNET m_session = NULL;
    HINTERNET m_connect = NULL;
    HINTERNET m_request = NULL;
};

