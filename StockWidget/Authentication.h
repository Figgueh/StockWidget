#pragma once
#include <string>
#include "json.hpp"


namespace Questrade
{
	class Authentication
	{
	private:
		std::string tokenType;
		std::string accessToken;
		int expiresIn;
		std::string refreshToken;
		std::string apiServer;

	public:
		Authentication();
		static Authentication authenticate(std::wstring refreshToken);

		std::string getRefreshToken();

		friend void to_json(nlohmann::json& j, const Authentication& p);
		friend void from_json(const nlohmann::json& j, Authentication& p);
	};
	
}


