#pragma once
#include "resource.h"
#include "api/framework.h"
#include "controller/ConfigHandler.h"
#include "controller/RequestHandler.h"
inline ConfigHandler* conf = ConfigHandler::getInstance();
//inline RequestHandler api;

INT_PTR CALLBACK WndSearchProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
inline void loadCurrentWatchlist(HWND hDlg);
inline void addSelectedToWatchlist(HWND hDlg);
inline void removeSelectedFromWatchlist(HWND hDlg);
inline void updateWatchlist(HWND hDlg);
inline void displaySearchResults(HWND hDlg);
