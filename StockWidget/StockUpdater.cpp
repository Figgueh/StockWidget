//#include "StockUpdater.h"
//#include <mutex>
//
//
//StockUpdater::StockUpdater(): is_terminated(false)
//{
//}
//
//void StockUpdater::accept()
//{
//	std::unique_lock<std::mutex> lock(mymutex);
//	mycond.wait_for(lock,
//		std::chrono::seconds(1000),
//		[]() { return is_terminated; });
//
//	while (!is_terminated)
//	{
//		Questrade::Quotes watchListQuotes = handle.getQuotes(watchlistG);
//
//		for (Questrade::Quote& quote : watchListQuotes.quotes)
//		{
//			for (stockListing& listing : priceLabels)
//			{
//				int length = GetWindowTextLength(listing.ticker);
//				wchar_t* ticker = new wchar_t[length + 1];
//				GetWindowText(listing.ticker, ticker, length + 1);
//
//				if (ticker == toWString(quote.symbol))
//					SetWindowTextA(listing.price, std::to_string(quote.askPrice).c_str());
//			}
//		}
//		OutputDebugStringW(L"Updated\n");
//
//	}
//
//}
//void StockUpdater::terminate()
//{
//	is_terminated = true;
//}
