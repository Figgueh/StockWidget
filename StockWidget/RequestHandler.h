#pragma once
#include "Authentication.h"
#include "WinHttp.h"
#include "Quote.h"
#include "Symbol.h"

namespace Questrade
{
	class RequestHandler
	{
	public:
		RequestHandler() = default;
		RequestHandler(Authentication& auth);
		std::wstring handleRequest(std::wstring path, std::map<std::string, std::string> headers);
		Questrade::Quotes getQuote(int stockId);
		Questrade::Symbols searchTicker(std::string ticker);

	private:
		std::wstring m_path;
		std::map<std::string, std::string> m_headers;
		Authentication m_auth;
		WinHttp m_connection;
	};
}

