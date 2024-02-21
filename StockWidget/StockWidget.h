#pragma once
#include "Authentication.h"
#include "RequestHandler.h"
#include "resource.h"

#include <vector>
struct stockListing
{
	HWND ticker;
	HWND price;
};

Questrade::RequestHandler handle;
void updateWatchlistPrice(std::vector<int>& const watchlist, std::vector<stockListing>& const priceLabels);
