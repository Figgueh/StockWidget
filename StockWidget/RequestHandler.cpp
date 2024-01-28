#include "RequestHandler.h"
#include "WinHttp.h"
#include "Toolbox.h"

Questrade::RequestHandler::RequestHandler(Authentication auth)
{
	this->auth = auth;
}

std::wstring Questrade::RequestHandler::getQuote(int stockId)
{
	WinHttp connection;
	std::wstring path = toWString("/v1/markets/quotes/" + std::to_string(stockId));
	std::map<std::string, std::string> headers;
	headers.emplace("Host", auth.getApiServer());
	headers.emplace("Authorization", auth.getTokenType() + " " + auth.getAccessToken());

	connection.open();
	connection.connect(WinHttp::stripHost(auth.getApiServer()).c_str());
	connection.requestHandler(L"GET", path.c_str());
	connection.addHeaders(headers);
	connection.sendRequest();
	std::wstring wResponse = connection.recieveResponse();
	return wResponse;
}
