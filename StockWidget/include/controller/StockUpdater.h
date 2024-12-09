//#pragma once
//#include <thread>
//#include <mutex>
//
//#include "api/framework.h"
//#include "model/StockListing.h"
//
//namespace StockWatch
//{
//	std::thread updater;
//	std::mutex mymutex;
//	std::condition_variable mycond;
//	bool running = true;
//
//	inline void startWatching(HWND hWnd)
//	{
//
//		OutputDebugStringW(L"Started\n");
//		while (running)
//		{
//			// Get their quotes
//			Quotes watchlistQuotes = makeRequest.getQuotes(watchlist);
//
//			for (Quote& quote : watchlistQuotes.m_quotes)
//			{
//
//				for (StockListing& listing : priceLabels)
//				{
//					// Get the text of the ticker
//					int length = GetWindowTextLength(listing.ticker);
//					wchar_t* ticker = new wchar_t[length + 1];
//					GetWindowText(listing.ticker, ticker, length + 1);
//
//					// If the stocks match, update the price
//					if (ticker == toWString(quote.m_symbol)) {
//
//						// Check if markets are closed
//						if (quote.m_price != 0.000000) {
//							// Update the last price
//							length = GetWindowTextLength(listing.price);
//							wchar_t* price = new wchar_t[length + 1];
//							GetWindowText(listing.price, price, length + 1);
//							listing.lastPrice = wcstod(price, NULL);
//
//							// Set the new price
//							SetWindowTextA(listing.price, std::to_string(quote.m_price).c_str());
//						}
//						else {
//							// Show the last traded price
//							std::string text = std::to_string(quote.lastTradePrice) + "*";
//							SetWindowTextA(listing.price, text.c_str());
//
//						}
//					}
//				}
//			}
//			OutputDebugStringW(L"Updated\n");
//			UpdateWindow(hWnd);
//
//			{
//				std::unique_lock<std::mutex> lock(mymutex);
//				mycond.wait_for(lock, std::chrono::seconds(10));
//			}
//		}
//
//	}
//
//	inline void stopWatching()
//	{
//		// Close the thread
//		{
//			std::lock_guard<std::mutex> guard(mymutex);
//			running = false;
//		}
//		mycond.notify_all();
//		updater.join();
//
//	}
//
//}
