#include "Authentication.h"
#include "Toolbox.h"


namespace Questrade
{
	std::wstring Questrade::Authentication::getRefreshToken() {
		return toWString(refreshToken);
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