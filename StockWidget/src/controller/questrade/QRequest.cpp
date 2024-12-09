#include "include/controller/questrade/QRequest.h"
#include "include/controller/WinHttp.h"
#include "include/utility/toolbox.h"
#include <utility/RequestError.h>


StockWatch::Quote QRequest::getQuote(std::string stockid)
{
	m_path = toWString("/v1/markets/quotes/" + stockid);
	m_headers.emplace("host", m_auth.getApiServer());
	m_headers.emplace("authorization", m_auth.getTokenType() + " " + m_auth.getAccessToken());

	try {
		std::wstring res = handleRequest(m_path, m_headers);

		if (res.compare(L"bad request") == 0)
			throw RequestError();

		nlohmann::json result = nlohmann::json::parse(toString(res));
		return result.template get<StockWatch::Quote>();
		//return Quote();

	}
	catch (nlohmann::json::exception& e) {
		std::string error = "error:" + std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"json parse error", MB_ICONERROR | MB_OK);
	}
	//return Quote();
}

std::wstring QRequest::handleRequest(std::wstring path, std::map<std::string, std::string> headers)
{
	m_connection.open();
	m_connection.connect(WinHttp::stripHost(m_auth.getApiServer()).c_str());
	m_connection.requestHandler(L"get", path.c_str());
	m_connection.addHeaders(headers);
	m_connection.sendRequest();
	return m_connection.recieveResponse();
}

// Quote* QRequest::getQuote(int stockid) const
//{
//	return nullptr;
//}

//inline Quote QRequest::getQuote(int stockid)
//{
//	return Quote();
//}



//
//inline Quote Questrade::QRequest::getQuote(int stockid)
//{
//}


//
 StockWatch::Quotes QRequest::getQuotes(std::vector<std::string> watchlist)
{
	std::string lookup = "";
	for (std::string id : watchlist)
	{
		lookup.append(id);

		if (id != watchlist.back())
			lookup.append(",");
	}

	m_path = toWString("/v1/markets/quotes/?ids=" + lookup);
	m_headers.emplace("host", m_auth.getApiServer());
	m_headers.emplace("authorization", m_auth.getTokenType() + " " + m_auth.getAccessToken());

	try {
		std::wstring res = handleRequest(m_path, m_headers);

		if (res.compare(L"bad request") == 0)
			throw RequestError();

		nlohmann::json result = nlohmann::json::parse(toString(res));
		return result.template get<StockWatch::Quotes>();

	}
	catch (nlohmann::json::exception& e) {
		std::string error = "error:" + std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"json parse error", MB_ICONERROR | MB_OK);
	}
}
//
//inline Symbols* Questrade::QRequest::searchTicker(std::string ticker)
//{
//	return new Symbols();
//
//	//m_path = toWString("/v1/symbols/search?prefix=" + ticker);
//
//	//m_headers.emplace("host", m_auth->getApiServer());
//	//m_headers.emplace("authorization", m_auth->getTokenType() + " " + m_auth->getAccessToken());
//
//	//try {
//	//	std::wstring res = handleRequest(m_path, m_headers);
//
//	//	if (res.compare(L"bad request") == 0)
//	//		throw RequestError();
//
//	//	nlohmann::json result = nlohmann::json::parse(toString(res));
//	//	return result.template get<Symbols>();
//
//	//}
//	//catch (nlohmann::json::exception& e) {
//	//	std::string error = "error:" + std::string(e.what());
//	//	MessageBox(NULL, toWString(error).c_str(), L"json parse error", MB_ICONERROR | MB_OK);
//	//}
//}
//
//inline Symbol* Questrade::QRequest::findStockSymbolWithQuote(const Questrade::QQuote& quote, const int& id)
//{
//	return new Symbol;
//
//	//Symbols possibletickers = searchTicker(quote.m_symbol);
//
//	//for (Symbol currentticker : possibletickers.m_symbols) {
//	//	if (currentticker.m_symbolid == id)
//	//		return currentticker;
//	//}
//
//	//return {};
//}
