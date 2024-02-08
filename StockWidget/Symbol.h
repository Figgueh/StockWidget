#pragma once
#include <string>
#include <vector>

#include "json.hpp"

namespace Questrade
{
	struct Symbol
	{
		std::string symbol;
		int symbolId;
		std::string description;
		bool isTradable;
		bool isQuotable;
		std::string securityType;

	};

	void from_json(const nlohmann::json& j, Symbol& s);

	struct Symbols
	{
		std::vector<Symbol> symbols;

	};

	void from_json(const nlohmann::json& j, Symbols& q);
}


