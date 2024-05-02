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
//void updateWatchlistPrice(std::vector<int>& const watchlist, std::vector<stockListing>& const priceLabels);
void updateWatchlistPrice(HWND hwnd, std::vector<stockListing> priceLabels);

std::mutex mymutex;
std::condition_variable mycond;
bool running = true;