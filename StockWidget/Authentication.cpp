#include "Authentication.h"
#include "Toolbox.h"
#include "WinHttp.h"
#include "RequestError.h"
namespace Questrade
{

	Authentication::Authentication()
	{
	}

	Questrade::Authentication Questrade::Authentication::authenticate(std::wstring refreshToken)
	{
		WinHttp connection;
		std::wstring path = L"/oauth2/token?grant_type=refresh_token&refresh_token=" + refreshToken;
		std::map<std::string, std::string> headers;
		headers.emplace("Host", "login.questrade.com");

		connection.open(L"StockWidget application/1.0");
		connection.connect(L"login.questrade.com");
		connection.requestHandler(L"GET", path.c_str());
		connection.addHeaders(headers);
		connection.sendRequest();
		std::wstring wResponse = connection.recieveResponse();

		nlohmann::json ans = nlohmann::json::parse(toString(wResponse));
		return ans.template get<Questrade::Authentication>();
	}

	std::string Questrade::Authentication::getTokenType() const {
		return tokenType;
	}
	std::string Questrade::Authentication::getAccessToken() const {
		return accessToken;
	}

	std::string Questrade::Authentication::getRefreshToken() const {
		return refreshToken;
	}

	std::string Questrade::Authentication::getApiServer() const {
		return apiServer;
	}

	void Questrade::to_json(nlohmann::json& j, const Authentication& a) 
	{
		j = nlohmann::json{ {"access_token", a.accessToken},
			{"token_type", a.tokenType}, {"expires_in", a.expiresIn},
		{"refresh_token", a.refreshToken}, {"api_server", a.apiServer} };
	}

	void Questrade::from_json(const nlohmann::json& j, Authentication& a) 
	{
		j.at("access_token").get_to(a.accessToken);
		j.at("token_type").get_to(a.tokenType);
		j.at("expires_in").get_to(a.expiresIn);
		j.at("refresh_token").get_to(a.refreshToken);
		j.at("api_server").get_to(a.apiServer);
	}

}