#include "model/questrade/QAuth.h"
#include <utility/Toolbox.h>

std::string QAuth::getTokenType() const {
	return m_tokenType;
}

std::string QAuth::getAccessToken() const {
	return m_accessToken;
}

std::string QAuth::getApiServer() const {
	return m_apiServer;
}

void to_json(nlohmann::json& j, const QAuth& a)
{
	j = nlohmann::json{ {"access_token", a.m_accessToken},
		{"token_type", a.m_tokenType}, {"expires_in", a.m_expiresIn},
	{"refresh_token", a.m_key}, {"api_server", a.m_apiServer}};
}

void from_json(const nlohmann::json& j, QAuth& a)
{
	j.at("access_token").get_to(a.m_accessToken);
	j.at("token_type").get_to(a.m_tokenType);
	j.at("expires_in").get_to(a.m_expiresIn);
	j.at("refresh_token").get_to(a.m_key);
	j.at("api_server").get_to(a.m_apiServer);
}
