#pragma once
#include <string>
#include <vector>
#include "utility/json.hpp"

namespace StockWatch {
	class Quote
	{
	public:
		Quote() = default;
		std::string m_symbol;
		double m_price;



	};

	inline void from_json(const nlohmann::json& j, Quote& q)
	{
		if (j.contains("quotes")) {
			q.m_symbol = j.at("quotes")[0]["symbol"].get<std::string>();

			//Ask price can be null? - This is an error for the stock ID 10000 - or durring market close only.
			q.m_price = j.at("quotes")[0]["askPrice"].get<double>();
		}
		else
		{
			//q.m_symbol = j.at("symbol");
			q.m_price = std::atof(j.at("price").get<std::string>().c_str());
		}

	}

	class Quotes
	{
	public:
		std::vector<Quote> m_quotes;
	};


	inline void from_json(const nlohmann::json& j, Quotes& qs)
	{
		if (j.contains("quotes")) {
			j.at("quotes").get_to(qs.m_quotes);
		}
		else
		{
			for (auto& [ticker, entry] : j.items())
			{
				j.at(ticker).get_to(qs.m_quotes);
				//Quote q{ ticker, entry[0].get<double>()};
			}
		}
	}

}



