#include "include/model/Symbol.h"

void Questrade::from_json(const nlohmann::json& j, Symbol& s)
{
	j.at("symbol").get_to(s.m_symbol);
	j.at("description").get_to(s.m_name);
}

//void Questrade::from_json(const nlohmann::json& j, Symbols& s)
//{
//	j.at("symbols").get_to(s.m_symbols);
//}
