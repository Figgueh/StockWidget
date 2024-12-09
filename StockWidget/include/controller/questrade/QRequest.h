#pragma once
#include "controller/RequestHandler.h"
#include "model/questrade/QAuth.h"
#include "model/Quote.h"
#include "model/Symbol.h"
#include <model/questrade/QQuote.h>

#include <model/questrade/QSymbol.h>

class QRequest : public RequestHandler
{
public:
	inline QRequest(QAuth auth) : m_auth(auth) {};

	std::wstring handleRequest(std::wstring path, std::map<std::string, std::string> headers);
	virtual StockWatch::Quote getQuote(std::string stockid);
	virtual StockWatch::Quotes getQuotes(std::vector<std::string> watchlist);

	//inline Symbols* searchTicker(std::string ticker);
	//inline Symbol* findStockSymbolWithQuote(const Questrade::QQuote& quote, const int& id);


//protected:(
	QAuth m_auth;

};
