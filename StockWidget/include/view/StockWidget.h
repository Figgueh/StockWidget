#pragma once
#include <thread>
#include <vector>
#include <mutex>

#include "controller/Authentication.h"
#include "controller/RequestHandler.h"
#include "controller/ConfigHandler.h"
#include "resource.h"
#include "model/ApplicationSettings.h"

struct stockListing
{
	HWND ticker;
	HWND price;
	double lastPrice;
};

inline Questrade::RequestHandler handle;
inline ApplicationSettings settings;
inline std::vector<int> watchlist;
inline std::vector<stockListing> priceLabels;
void initializeWatchlist(HWND hWnd, Questrade::Quotes quotes);
//void startWatching(HWND hwnd);

inline ConfigHandler configuration;
//std::mutex mymutex;
//std::condition_variable mycond;
//bool running = true;
