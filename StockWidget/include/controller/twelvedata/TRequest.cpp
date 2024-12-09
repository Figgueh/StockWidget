#include "TRequest.h"
#include <utility/Toolbox.h>
#include <utility/RequestError.h>

std::wstring TRequest::handleRequest(std::wstring path, std::map<std::string, std::string> headers)
{
	m_connection.open();
	m_connection.connect(L"api.twelvedata.com");
	m_connection.requestHandler(L"get", path.c_str());
	m_connection.addHeaders(headers);
	m_connection.sendRequest();
	return m_connection.recieveResponse();
}

StockWatch::Quote TRequest::getQuote(std::string symbol)
{
	m_path = toWString("/price?symbol=" + symbol + "&apikey=" + m_auth.m_key);
	try {
		std::wstring res = handleRequest(m_path, m_headers);

		if (res.compare(L"bad request") == 0)
			throw RequestError();

		nlohmann::json result = nlohmann::json::parse(toString(res));
		return result.template get<StockWatch::Quote>();
	}
	catch (nlohmann::json::exception& e) {
		std::string error = "error:" + std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"json parse error", MB_ICONERROR | MB_OK);
	}
}

StockWatch::Quotes TRequest::getQuotes(std::vector<std::string> symbols)
{
	std::string lookup = "";
	for (std::string symbol : symbols) {
		lookup.append(symbol);

		if (symbol != symbols.back())
			lookup.append(",");
	}

	m_path = toWString("price?symbol=" + lookup + "&interval=1min&apikey=" + m_auth.m_key);
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
