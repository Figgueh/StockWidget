#pragma once
#include <string>
#include <exception>

#include "utility/json.hpp"
#include "model/Auth.h"
#include "ConfigHandler.h"

class Authentication
{
public:
	Authentication() = default;

	virtual const void authenticate() = 0;

	Auth getAuth();
	static inline bool m_isAuthenticated = false;

protected:
	ConfigHandler* m_configuraiton = ConfigHandler::getInstance();
	Auth m_authenticationData;
};


class AuthenticationError : public std::exception
{
public:
	AuthenticationError() = default;
	const char* what() const throw();
};

