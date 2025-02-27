#pragma once
#include <string>
#include <vector>

#include "utility/json.hpp"

namespace Questrade
{
	struct Quote {
		std::string symbol;
		int symbolId;
		double bidPrice;
		int bidSize;
		double askPrice;
		int askSize;
		double lastTradePriceTrHrs;
		double lastTradePrice;
		int lastTradeSize;
		int volume;
		double openPrice;
		double highPrice;
		double lowPrice;
		bool isHalted;

		Quote() = default;
	};

	void from_json(const nlohmann::json& j, Quote& q);

	struct Quotes {
		std::vector<Quote> quotes;

		Quotes() = default;
	};

	void from_json(const nlohmann::json& j, Quotes& q);
}

