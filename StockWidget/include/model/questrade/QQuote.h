#pragma once

#include "utility/json.hpp"
#include "model/Quote.h"

class QQuote : public StockWatch::Quote {
		QQuote() = default;

	public:
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

	};

	inline void from_json(const nlohmann::json& j, QQuote& q);

	class QQuotes {
	public:
		std::vector<QQuote> quotes;

		QQuotes() = default;
	};

	void from_json(const nlohmann::json& j, QQuotes& q);


