#include "controller/Authentication.h"
#include "controller/WinHttp.h"
#include "utility/RequestError.h"
#include "utility/Toolbox.h"

namespace Questrade
{
	QAuth Authentication::getAuth()
	{
		return m_authenticationData;
	}
	Authentication::Authentication()
	{
	}

	Authentication* Authentication::getInstance()
	{
		if (m_instance == nullptr)
		{
			m_instance = new Authentication();
		}
		return m_instance;
	}

	void const Questrade::Authentication::authenticate(const std::wstring& refreshToken)
	{
		WinHttp connection;
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
		m_authenticationData = ans.template get<Questrade::QAuth>();
	}


	const char* AuthenticationError::what() const throw()
	{
		return "The refresh token was refused. Please try again.";
	}

}