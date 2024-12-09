#pragma once
#include <Windows.h>
#include "controller/ConfigHandler.h"

inline ConfigHandler* configurationHandler = ConfigHandler::getInstance();

LRESULT CALLBACK WndSettingProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
inline void updateCheckboxState(HWND hDlg);
inline void saveSettings(HWND hDlg);
