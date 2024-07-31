#pragma once
#include <thread>
#include <vector>
#include <mutex>

#include "model/Authentication.h"
#include "model/RequestHandler.h"
#include "model/ConfigHandler.h"
#include "api/resource.h"

struct stockListing
{
	HWND ticker;
	HWND price;

	////bool std::operator==(const stockListing& rhs , const stockListing& b) { return this->price == rhs.price; };
	//	bool std::operator==(const stockListing& a, const stockListing& b) { return a.price == b.price;
	//}


};

Questrade::RequestHandler handle;
std::vector<int> watchlist;
std::vector<stockListing> priceLabels;
void initializeWatchlist(HWND hWnd, Questrade::Quotes quotes);
void startWatching(HWND hwnd);


ConfigHandler configuration;
std::mutex mymutex;
std::condition_variable mycond;
bool running = true;
