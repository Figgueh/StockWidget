#pragma once
#include <string>
#include "utility/json.hpp"


class TError
{
public:
	int code;
	std::string message;
	std::string status;

};

void from_json(const nlohmann::json& j, TError& q);

