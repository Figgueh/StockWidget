#pragma once
#include <string>

#include "utility/json.hpp"
#include "model/Symbol.h"

	class QSymbol : public Symbol
	{
	public:
		int symbolId;
		bool isTradable;
		bool isQuotable;
		std::string securityType;

	};

	void from_json(const nlohmann::json& j, QSymbol& s);

	struct QSymbols
	{
	public:
		std::vector<QSymbol> symbols;

	};

	void from_json(const nlohmann::json& j, QSymbols& q);


