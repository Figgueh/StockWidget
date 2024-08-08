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
	double lastPrice;
};

Questrade::RequestHandler handle;
std::vector<int> watchlist;
std::vector<stockListing> priceLabels;
void initializeWatchlist(HWND hWnd, Questrade::Quotes quotes);

ConfigHandler configuration;
