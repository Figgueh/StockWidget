#include "questrade/Symbol.h"

void Questrade::from_json(const nlohmann::json& j, Symbol& s)
{
	j.at("symbol").get_to(s.symbol);
	j.at("symbolId").get_to(s.symbolId);
	j.at("description").get_to(s.description);
	j.at("isTradable").get_to(s.isTradable);
	j.at("isQuotable").get_to(s.isQuotable);
	j.at("securityType").get_to(s.securityType);
}

void Questrade::from_json(const nlohmann::json& j, Symbols& q)
{
	j.at("symbols").get_to(q.symbols);
}
