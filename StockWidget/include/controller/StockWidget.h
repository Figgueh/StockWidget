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

inline Questrade::RequestHandler handle;
inline std::vector<int> watchlist;
inline std::vector<stockListing> priceLabels;
void initializeWatchlist(HWND hWnd, Questrade::Quotes quotes);
//void startWatching(HWND hwnd);


inline ConfigHandler configuration;
//std::mutex mymutex;
//std::condition_variable mycond;
//bool running = true;
