#include "controller/requesthandler.h"
#include "model/quote.h"
#include "utility/requesterror.h"
#include "utility/toolbox.h"
#include <controller/questrade/QRequest.h>

//RequestHandler::RequestHandler(Authentication* auth)
//{
//	this->m_auth = auth;
//}

//std::wstring alphavantage::requesthandler::handlerequest(std::wstring path, std::map<std::string, std::string> headers)
//{
//	m_connection.open();
//	m_connection.connect(winhttp::striphost(std::string("https://www.alphavantage.co")).c_str());
//	m_connection.requesthandler(l"get", path.c_str());
//	m_connection.addheaders(headers);
//	m_connection.sendrequest();
//	return m_connection.recieveresponse();
//}
//
//std::wstring alphavantage::requesthandler::getquotea(int stockid)
//{
//	m_path = towstring("query?function=time_series_daily&symbol=ibm&apikey=" + m_key);
//
//	std::wstring res = handlerequest(m_path, m_headers);
//	if (res.compare(l"bad request") == 0)
//		throw requesterror();
//
//	return res;
//}
//
//inline Quote* RequestHandler::getQuote(int stockId) const
//{
//	return nullptr;
//}

// Quote RequestHandler::getQuote(int stockId)
//{
//	return Quote();
//}

void RequestHandler::handleWatchlist(std::vector<std::string> watchlist)
{
	for (std::string line : watchlist)
	{
		if (line[0] == 'Q')
			StockWatch::Quote q = QRequest::getQuote(line);
	}
}
