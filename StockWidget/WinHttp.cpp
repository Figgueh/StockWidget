#include <atlbase.h>
#include <exception>

#include "WinHttp.h"
#include "RequestError.h"
#include "toolbox.h"
#include "Authentication.h"

void WinHttp::open()
{
	m_session = WinHttpOpen(L"StockWidget/1.0", WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
}

void WinHttp::connect(LPCWSTR host)
{
	if (m_session) 
	{
		m_connect = WinHttpConnect(m_session, host, INTERNET_DEFAULT_HTTPS_PORT, 0);
	}
}

void WinHttp::requestHandler(LPCWSTR verb, LPCWSTR path)
{
	if (m_connect) 
	{
        m_request = WinHttpOpenRequest(m_connect, verb, path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	}
}

void WinHttp::addHeaders(std::map<std::string, std::string> headers)
{
    for (auto const& [key, val] : headers)
    {
        //if (m_result)
            m_result = WinHttpAddRequestHeaders(m_request,
                toWString(key + ": " + val).c_str(),
                (DWORD)-1,
                WINHTTP_ADDREQ_FLAG_REPLACE | WINHTTP_ADDREQ_FLAG_ADD);
    }
}

void WinHttp::sendRequest()
{

	if (m_request) 
	{
        m_result = WinHttpSendRequest(m_request, WINHTTP_NO_ADDITIONAL_HEADERS, 0, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	}

    if (m_result)
        WinHttpReceiveResponse(m_request, NULL);
}

std::wstring WinHttp::recieveResponse()
{
    DWORD dwSize = 0;
    DWORD dwDownloaded = 0;
    LPSTR pszOutBuffer;
    std::wstring ans;

    do
    {
        // Check for available data.
        dwSize = 0;
        if (!WinHttpQueryDataAvailable(m_request, &dwSize))
            printf("Error %u in WinHttpQueryDataAvailable.\n",
                GetLastError());

        // Allocate space for the buffer.
        pszOutBuffer = new char[dwSize + 1];
        if (!pszOutBuffer)
        {
            printf("Out of memory\n");
            dwSize = 0;
        }
        else
        {
            // Read the data.
            ZeroMemory(pszOutBuffer, dwSize + 1);

            if (!WinHttpReadData(m_request, (LPVOID)pszOutBuffer,
                dwSize, &dwDownloaded))
                printf("Error %u in WinHttpReadData.\n", GetLastError());
            else
            {
                USES_CONVERSION;
                printf("%s", pszOutBuffer);
                LPCWSTR line = A2W(pszOutBuffer);
                
                if (ans == L"")
                {
                    ans = line;
                }
                else
                {
                    if(line != L"")
                    {
                        std::wstring df = std::wstring(ans) + line;
                        ans = df.c_str();
                    }
                    
                }
            }
        }
    } while (dwSize > 0);

    return ans;
}

std::wstring WinHttp::stripHost(const std::string& host)
{
    //Remove https://
    std::string httpLess = host;
    httpLess.erase(0, 8);
    httpLess.erase(httpLess.end()-1, httpLess.end());
    return toWString(httpLess);
}

WinHttp::~WinHttp()
{
    if (m_request) WinHttpCloseHandle(m_request);
    if (m_connect) WinHttpCloseHandle(m_connect);
    if (m_session) WinHttpCloseHandle(m_session);
}