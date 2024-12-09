#include "include/controller/questrade/QAuthentication.h"
#include "controller/WinHttp.h"
#include <model/questrade/QAuth.h>
#include <utility/Toolbox.h>

void const QAuthentication::authenticate()
{
	WinHttp connection;
	std::wstring refreshToken = m_configuraiton->getRefreshToken();
	std::wstring path = L"/oauth2/token?grant_type=refresh_token&refresh_token=" + refreshToken;
	std::map<std::string, std::string> headers;
	headers.emplace("Host", "login.questrade.com");

	connection.open();
	connection.connect(L"login.questrade.com");
	connection.requestHandler(L"GET", path.c_str());
	connection.addHeaders(headers);
	connection.sendRequest();
	std::wstring wResponse = connection.recieveResponse();

	if (wResponse.compare(L"Bad Request") != 0 && !wResponse.empty())
		m_isAuthenticated = true;
	else
	{
		throw AuthenticationError();
	}

	nlohmann::json ans = nlohmann::json::parse(toString(wResponse));
	m_auth = ans.template get<QAuth>();

	m_configuraiton->updateRefreshToken(m_auth.m_key);
	m_configuraiton->writeConfig();
	OutputDebugStringW(L"Questrade account authenticated.\n");

}

