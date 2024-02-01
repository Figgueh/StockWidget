#pragma once
#include "Authentication.h"
#include "WinHttp.h"
#include "Quote.h"

namespace Questrade
{
	class RequestHandler
	{
	private: 
		Authentication m_auth;
		WinHttp m_connection;

	public:
		RequestHandler(Authentication auth);
		std::wstring handleRequest(std::wstring path, std::map<std::string, std::string> headers);
		Questrade::Quotes getQuote(int stockId);
	};

}

