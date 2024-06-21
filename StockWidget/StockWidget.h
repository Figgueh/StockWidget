#pragma once
#include "Authentication.h"
#include "RequestHandler.h"
#include "resource.h"
#include <thread>

#include <vector>
#include <mutex>
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
