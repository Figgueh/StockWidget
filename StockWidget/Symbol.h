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

	};

	void from_json(const nlohmann::json& j, Symbol& s);

	struct Symbols
	{
		std::vector<Symbol> symbols;

	};

	void from_json(const nlohmann::json& j, Symbols& q);
}


