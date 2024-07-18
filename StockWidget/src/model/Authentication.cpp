#include "model/Authentication.h"
#include "utility/Toolbox.h"
#include "../WinHttp.h"
#include "utility/RequestError.h"

namespace Questrade
{

	Authentication::Authentication()
	{
	}

	Questrade::Authentication const Questrade::Authentication::authenticate(const std::wstring& refreshToken)
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
			throw AuthenticationError();

		nlohmann::json ans = nlohmann::json::parse(toString(wResponse));
		return ans.template get<Questrade::Authentication>();
	}

	std::string Questrade::Authentication::getTokenType() const {
		return m_tokenType;
	}
	std::string Questrade::Authentication::getAccessToken() const {
		return m_accessToken;
	}

	std::string Questrade::Authentication::getRefreshToken() const {
		return m_refreshToken;
	}

	std::string Questrade::Authentication::getApiServer() const {
		return m_apiServer;
	}

	void Questrade::to_json(nlohmann::json& j, const Authentication& a) 
	{
		j = nlohmann::json{ {"access_token", a.m_accessToken},
			{"token_type", a.m_tokenType}, {"expires_in", a.m_expiresIn},
		{"refresh_token", a.m_refreshToken}, {"api_server", a.m_apiServer} };
	}

	void Questrade::from_json(const nlohmann::json& j, Authentication& a) 
	{
		j.at("access_token").get_to(a.m_accessToken);
		j.at("token_type").get_to(a.m_tokenType);
		j.at("expires_in").get_to(a.m_expiresIn);
		j.at("refresh_token").get_to(a.m_refreshToken);
		j.at("api_server").get_to(a.m_apiServer);
	}

	const char* AuthenticationError::what() const throw()
	{
		return "The provided refresh token is incorrect";
	}

}