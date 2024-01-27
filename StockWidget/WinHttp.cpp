#include <atlbase.h>
#include <exception>

#include "WinHttp.h"
#include "RequestError.h"


void WinHttp::Open(LPCWSTR userAgent)
{
	//Convert string to wstring to pass a LPCWSTR
	//std::wstring stemp = std::wstring(userAgent.begin(), userAgent.end());
	m_session = WinHttpOpen(userAgent, WINHTTP_ACCESS_TYPE_AUTOMATIC_PROXY, WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
}

void WinHttp::Connect(LPCWSTR host)
{
	if (m_session) 
	{
		//Convert string to wstring to pass a LPCWSTR
		//std::wstring stemp = std::wstring(host.begin(), host.end());
		m_connect = WinHttpConnect(m_session, host, INTERNET_DEFAULT_HTTPS_PORT, 0);
	}
}

void WinHttp::RequestHandler(LPCWSTR verb, LPCWSTR path)
{
	if (m_connect) 
	{
		//std::wstring stempVerb = std::wstring(verb.begin(), verb.end());
        //std::wstring stempPath = std::wstring(path.begin(), path.end());
        m_request = WinHttpOpenRequest(m_connect, verb, path, NULL, WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, WINHTTP_FLAG_SECURE);
	}
}

void WinHttp::SendRequest(LPCWSTR headers, int length)
{
    bool bResults;

	if (m_request) 
	{
        bResults = WinHttpSendRequest(m_request, headers, length, WINHTTP_NO_REQUEST_DATA, 0, 0, 0);
	}

    if(bResults)
        WinHttpReceiveResponse(m_request, NULL);
}


std::wstring WinHttp::RecieveResponse()
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

    if (ans._Equal(L"Bad Request"))
    {
        throw RequestError();
    }
    return ans;
}

WinHttp::~WinHttp()
{
    if (m_request) WinHttpCloseHandle(m_request);
    if (m_connect) WinHttpCloseHandle(m_connect);
    if (m_session) WinHttpCloseHandle(m_session);
}