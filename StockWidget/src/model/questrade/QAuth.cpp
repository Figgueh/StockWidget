#include "model/questrade/QAuth.h"
#include <utility/Toolbox.h>

std::string Questrade::QAuth::getTokenType() const {
	return m_tokenType;
}
std::string Questrade::QAuth::getAccessToken() const {
	return m_accessToken;
}

std::string Questrade::QAuth::getRefreshToken() const {
	return m_refreshToken;
}

std::string Questrade::QAuth::getApiServer() const {
	return m_apiServer;
}

void Questrade::to_json(nlohmann::json& j, const QAuth& a)
{
	j = nlohmann::json{ {"access_token", a.m_accessToken},
		{"token_type", a.m_tokenType}, {"expires_in", a.m_expiresIn},
	{"refresh_token", a.m_refreshToken}, {"api_server", a.m_apiServer} };
}

void Questrade::from_json(const nlohmann::json& j, QAuth& a)
{
	j.at("access_token").get_to(a.m_accessToken);
	j.at("token_type").get_to(a.m_tokenType);
	j.at("expires_in").get_to(a.m_expiresIn);
	j.at("refresh_token").get_to(a.m_refreshToken);
	j.at("api_server").get_to(a.m_apiServer);
}
