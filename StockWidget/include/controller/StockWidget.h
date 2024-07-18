#pragma once
#include <thread>
#include <vector>
#include <mutex>

#include "model/Authentication.h"
#include "model/RequestHandler.h"
#include "api/resource.h"

struct stockListing
{
	HWND ticker;
	HWND price;
};

Questrade::RequestHandler handle;
std::vector<int> watchlist;
std::vector<stockListing> priceLabels;
void initializeWatchlist(HWND hWnd, Questrade::Quotes quotes);
void startWatching(HWND hwnd);

std::mutex mymutex;
std::condition_variable mycond;
bool running = true;
