#pragma once
#include <exception>
#include "Authentication.h"
#include "controller/WinHttp.h"
#include "model/questrade/Quote.h"
#include "model/questrade/Symbol.h"

namespace Questrade
{
	class RequestHandler
	{
	public:
		RequestHandler();
		std::wstring handleRequest(std::wstring path, std::map<std::string, std::string> headers);
		Questrade::Quotes getQuote(int stockId);
		Questrade::Quotes getQuotes(std::vector<int> stockIds);
		Questrade::Symbols searchTicker(std::string ticker);
		Questrade::Symbol findStockSymbolWithQuote(const Questrade::Quote& quote, const int& id);

	private:
		std::wstring m_path;
		std::map<std::string, std::string> m_headers;
		Authentication* m_auth;
		WinHttp m_connection;
	};

	class RequestError : public std::exception
	{
	public:
		RequestError() = default;
		const char* what() const throw();
	};
}

