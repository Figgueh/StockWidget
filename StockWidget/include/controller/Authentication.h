#pragma once
#include <string>
#include <exception>

#include "utility/json.hpp"
#include <model/questrade/QAuth.h>

namespace Questrade
{
	class Authentication
	{
	public:
		Authentication(const Authentication& obj) = delete;
		static Authentication* getInstance();

		void const authenticate(const std::wstring& refreshToken);

		QAuth getAuth();
		static inline bool m_isAuthenticated = false;
	private:
		Authentication();

		inline static Authentication* m_instance;
		QAuth m_authenticationData;
	};


	class AuthenticationError : public std::exception
	{
	public:
		AuthenticationError() = default;
		const char* what() const throw();
	};
}




