#pragma once
#include <string>

class Auth {
public:
	std::string m_key;

	Auth() = default;
	Auth(std::string apiKey) : m_key(apiKey) {};
};
