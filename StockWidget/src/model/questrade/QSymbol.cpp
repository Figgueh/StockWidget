#include "model/questrade/QSymbol.h"

void from_json(const nlohmann::json& j, QSymbol& s)
{
	j.at("symbol").get_to(s.m_symbol);
	j.at("symbolId").get_to(s.symbolId);
	j.at("description").get_to(s.m_name);
	j.at("isTradable").get_to(s.isTradable);
	j.at("isQuotable").get_to(s.isQuotable);
	j.at("securityType").get_to(s.securityType);
}

void from_json(const nlohmann::json& j, QSymbols& q)
{
	j.at("symbols").get_to(q.symbols);
}
