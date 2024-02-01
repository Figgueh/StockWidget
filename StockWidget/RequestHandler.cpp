#include "RequestHandler.h"
#include "Toolbox.h"

#include "Quote.h"

Questrade::RequestHandler::RequestHandler(Authentication auth)
{
	this->m_auth = auth;
}

std::wstring Questrade::RequestHandler::handleRequest(std::wstring path, std::map<std::string, std::string> headers)
{
	m_connection.open();
	m_connection.connect(WinHttp::stripHost(m_auth.getApiServer()).c_str());
	m_connection.requestHandler(L"GET", path.c_str());
	m_connection.addHeaders(headers);
	m_connection.sendRequest();
	return m_connection.recieveResponse();
}

Questrade::Quotes Questrade::RequestHandler::getQuote(int stockId)
{
	std::wstring path;
	std::map<std::string, std::string> headers;
		
	path = toWString("/v1/markets/quotes/" + std::to_string(stockId));
	
	headers.emplace("Host", m_auth.getApiServer());
	headers.emplace("Authorization", m_auth.getTokenType() + " " + m_auth.getAccessToken());

	try {
		std::wstring res = handleRequest(path, headers);
		nlohmann::json result = nlohmann::json::parse(toString(res));
		return result.template get<Questrade::Quotes>();

	}
	catch (nlohmann::json::exception& e) {
		std::string error = "ERROR:" + std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"JSON parse error", MB_ICONERROR | MB_OK);
	}

	
}
