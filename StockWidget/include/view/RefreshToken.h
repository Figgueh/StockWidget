#pragma once
#include "resource.h"
#include "api/framework.h"
#include "controller/Authentication.h"
#include "controller/ConfigHandler.h"


INT_PTR CALLBACK WndRefreshProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
inline Authentication* auth;
inline ConfigHandler* config = ConfigHandler::getInstance();