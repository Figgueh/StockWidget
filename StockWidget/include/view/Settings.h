#pragma once
#include <Windows.h>
#include <controller/ConfigHandler.h>

inline ConfigHandler* configurationHandler;

LRESULT CALLBACK WndSettingProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

