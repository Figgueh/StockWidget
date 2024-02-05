#include "RequestHandler.h"
#include "Toolbox.h"

#include "Quote.h"

Questrade::RequestHandler::RequestHandler(Authentication& auth)
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
	m_path = toWString("/v1/markets/quotes/" + std::to_string(stockId));
	
	m_headers.emplace("Host", m_auth.getApiServer());
	m_headers.emplace("Authorization", m_auth.getTokenType() + " " + m_auth.getAccessToken());

	try {
		std::wstring res = handleRequest(m_path, m_headers);
		nlohmann::json result = nlohmann::json::parse(toString(res));
		return result.template get<Questrade::Quotes>();

	}
	catch (nlohmann::json::exception& e) {
		std::string error = "ERROR:" + std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"JSON parse error", MB_ICONERROR | MB_OK);
	}
}

Questrade::Symbols Questrade::RequestHandler::searchTicker(std::string ticker)
{
	m_path = toWString("/v1/symbols/search?prefix=" + ticker);

	m_headers.emplace("Host", m_auth.getApiServer());
	m_headers.emplace("Authorization", m_auth.getTokenType() + " " + m_auth.getAccessToken());

	try {
		std::wstring res = handleRequest(m_path, m_headers);
		nlohmann::json result = nlohmann::json::parse(toString(res));
		return result.template get<Questrade::Symbols>();

	}
	catch (nlohmann::json::exception& e) {
		std::string error = "ERROR:" + std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"JSON parse error", MB_ICONERROR | MB_OK);
	}
}
