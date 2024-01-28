#pragma once
#include "Authentication.h"

namespace Questrade
{

	class RequestHandler
	{
	private: 
		Authentication auth;

	public:
		RequestHandler(Authentication auth);
		std::wstring getQuote(int stockId);
	};

}

