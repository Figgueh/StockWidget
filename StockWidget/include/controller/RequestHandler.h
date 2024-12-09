#pragma once
#include <exception>
#include "Authentication.h"
#include "controller/WinHttp.h"
#include "model/Quote.h"
#include "model/Symbol.h"

#include "include/model/questrade/QQuote.h"

class RequestHandler
{
public:
	//RequestHandler(Authentication* auth);
	RequestHandler() = default;

	virtual std::wstring handleRequest
	(std::wstring path, std::map<std::string, std::string> headers) = 0;

	virtual StockWatch::Quote getQuote(std::string stockId) = 0;
	virtual StockWatch::Quotes getQuotes(std::vector<std::string> watchlist) = 0;
	void handleWatchlist(std::vector<std::string> watchlist);

	//virtual Quotes* getQuotes(std::vector<int> stockIds) = 0;
	//virtual Symbols* searchTicker(std::string ticker) = 0;
	//virtual Symbol* findStockSymbolWithQuote(const Quote& quote, const int& id) = 0;

//protected:
	std::wstring m_path;
	std::map<std::string, std::string> m_headers;
	WinHttp m_connection;
};

//namespace AlphaVantage
//{
//	class RequestHandler
//	{
//		RequestHandler() = default;
//		std::wstring handleRequest(std::wstring path, std::map<std::string, std::string> headers);
//		std::wstring getQuoteA(int stockId);
//
//	private:
//		std::string m_key = "J1FAQ28CEMLK05T8";
//		std::wstring m_path;
//		std::map<std::string, std::string> m_headers;
//		WinHttp m_connection;
//
//	};
//}

