#pragma once
#include <string>
#include <exception>

#include "json.hpp"

namespace Questrade
{
	class Authentication
	{
	public:
		Authentication();
		static Authentication const authenticate(const std::wstring& refreshToken);

		std::string getTokenType() const;
		std::string getAccessToken() const;
		std::string getRefreshToken() const;
		std::string getApiServer() const;

		friend void to_json(nlohmann::json& j, const Authentication& p);
		friend void from_json(const nlohmann::json& j, Authentication& p);

		static inline bool m_isAuthenticated = false;

	private:
		std::string m_tokenType;
		std::string m_accessToken;
		int m_expiresIn;
		std::string m_refreshToken;
		std::string m_apiServer;
	};


	class AuthenticationError : public std::exception
	{
	public:
		AuthenticationError() = default;
		const char* what() const throw();
	};
}

extern Questrade::Authentication auth;



