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

		connection.Open(L"StockWidget application/1.0");
		connection.Connect(L"login.questrade.com");
		connection.RequestHandler(L"GET", path.c_str());
		connection.SendRequest(L"Host: login.questrade.com", 26);
		std::wstring wResponse = connection.RecieveResponse();

		nlohmann::json ans = nlohmann::json::parse(toString(wResponse));
		return ans.template get<Questrade::Authentication>();
	}

	std::string Questrade::Authentication::getRefreshToken() {
		return refreshToken;
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