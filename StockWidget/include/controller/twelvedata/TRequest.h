#pragma once
#include "controller/RequestHandler.h"
#include "model/twelvedata/TAuth.h"

class TRequest : public RequestHandler
{
public:
	inline TRequest(TAuth auth) :m_auth(auth) {};

	std::wstring handleRequest(std::wstring path, std::map<std::string, std::string> headers);
	virtual StockWatch::Quote getQuote(std::string symbol);
	virtual StockWatch::Quotes getQuotes(std::vector<std::string>);

	TAuth m_auth;
};
