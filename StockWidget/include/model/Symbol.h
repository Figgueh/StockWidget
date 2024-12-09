#pragma once
#include <string>
#include <vector>
#include "utility/json.hpp"


class Symbol {
public:
	std::string m_symbol;
	std::string m_name;

};

class Symbols {
public:
	std::vector<Symbol> m_symbols;
};

namespace Questrade {
	void from_json(const nlohmann::json& j, Symbol& s);
	void from_json(const nlohmann::json& j, Symbols& s);

};

