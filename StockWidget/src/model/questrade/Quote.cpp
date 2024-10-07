#include "questrade/Quote.h"

void Questrade::from_json(const nlohmann::json& j, Quote& q)
{
	j.at("symbol").get_to(q.symbol);
	j.at("symbolId").get_to(q.symbolId);

	if (!j.at("bidPrice").is_null())
		j.at("bidPrice").get_to(q.bidPrice);

	if (!j.at("bidSize").is_null())
		j.at("bidSize").get_to(q.bidSize);

	if (!j.at("askSize").is_null())
		j.at("askSize").get_to(q.askSize);

	if (!j.at("askPrice").is_null())
		j.at("askPrice").get_to(q.askPrice);

	if (!j.at("lastTradePriceTrHrs").is_null())
		j.at("lastTradePriceTrHrs").get_to(q.lastTradePriceTrHrs);

	if (!j.at("lastTradePrice").is_null())
		j.at("lastTradePrice").get_to(q.lastTradePrice);

	if (!j.at("lastTradeSize").is_null())
		j.at("lastTradeSize").get_to(q.lastTradeSize);

	if (!j.at("volume").is_null())
		j.at("volume").get_to(q.volume);

	if (!j.at("openPrice").is_null())
		j.at("openPrice").get_to(q.openPrice);

	if (!j.at("highPrice").is_null())
		j.at("highPrice").get_to(q.highPrice);

	if (!j.at("lowPrice").is_null())
		j.at("lowPrice").get_to(q.lowPrice);


	j.at("isHalted").get_to(q.isHalted);

}

void Questrade::from_json(const nlohmann::json& j, Quotes& q)
{
	j.at("quotes").get_to(q.quotes);
}
