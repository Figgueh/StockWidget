#pragma once
#include <thread>
#include <vector>
#include <mutex>

#include "controller/Authentication.h"
#include "controller/RequestHandler.h"
#include "controller/ConfigHandler.h"
#include "resource.h"
#include "model/ApplicationSettings.h"
#include "model/StockListing.h"





#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
COLORREF backgroundColor = RGB(255, 240, 255);

// Windows handles:
INT_PTR hwndStocks = NULL;
INT_PTR hwndSettings = NULL;
HWND hWnd;

// Variables:
inline ApplicationSettings settings;
inline std::vector<std::string> watchlist;
inline std::vector<StockListing> priceLabels;
//inline Authentication* authentication;
inline std::vector<Authentication*> authentications;
inline ConfigHandler* configuration = ConfigHandler::getInstance();
RequestHandler* makeRequest;

// Functions
inline void registerHotKeys();
inline void authenticate();
inline void loadUserSettings();
inline void prepareWatchlist();
void initializeWatchlist(HWND hWnd, StockWatch::Quotes quotes);

inline void applyWatchlistUpdates();

inline void applyColorEffect();

