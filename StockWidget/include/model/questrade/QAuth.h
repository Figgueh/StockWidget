#pragma once
#include <string>
#include "utility/json.hpp"
#include "model/Auth.h"

/**
* This class is a holder for the responce send back from questrade.
*/
class QAuth : public Auth
{
public:
	std::string getTokenType() const;
	std::string getAccessToken() const;
	std::string getApiServer() const;

	friend void to_json(nlohmann::json& j, const QAuth& p);
	friend void from_json(const nlohmann::json& j, QAuth& p);

private:
	std::string m_tokenType;
	std::string m_accessToken;
	int m_expiresIn = 0;
	std::string m_apiServer;

};
