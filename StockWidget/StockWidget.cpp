// StockWidget.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "StockWidget.h"
#include "ConfigHandler.h"
#include "json.hpp"
#include "Toolbox.h"
#include "RequestError.h"
#include "Search.h"
#include "RefreshToken.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
Questrade::Authentication auth;
std::vector<HWND> priceLables;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void createItem(HWND& hWnd, Questrade::Quotes quotes);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.

	// Initialize global strings
	LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadStringW(hInstance, IDC_STOCKWIDGET, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_STOCKWIDGET));

	MSG msg;

	// Main message loop:
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int)msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_STOCKWIDGET));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = CreateHatchBrush(HS_DIAGCROSS, RGB(255, 0, 0));
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_STOCKWIDGET);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, 500, 400, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	RegisterHotKey(hWnd, HOTKEY_SETTINGS, MOD_ALT, 0x53); //s
	RegisterHotKey(hWnd, HOTKEY_CLOSE, MOD_ALT, VK_ESCAPE); //esc

	ConfigHandler configuration;
	std::string refreshToken;

	try {
		auth = Questrade::Authentication::authenticate(configuration.getRefreshToken());
		refreshToken = auth.getRefreshToken();
		configuration.updateRefreshToken(refreshToken);
	}
	catch (Questrade::AuthenticationError& e) {
		std::string error = std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"Authentication error", MB_ICONERROR | MB_OK);
		DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_REFRESHTOKEN), hWnd, WndRefreshProc, (LPARAM)&auth);
	}
	catch (RequestError& e) {
		std::string error = std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"Bad request error", MB_ICONERROR | MB_OK);
	}
	catch (nlohmann::json::exception& e) {
		configuration.updateRefreshToken(refreshToken);
		std::string error = "ERROR:" + std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"JSON parse error", MB_ICONERROR | MB_OK);
	}

	handle = Questrade::RequestHandler(auth);


	// Get the list of ids thats in the watchlist
	std::vector<int> watchlist = configuration.getTickers();

	// Get their quotes
	Questrade::Quotes watchlistQuotes = handle.getQuotes(watchlist);

	createItem(hWnd, watchlistQuotes);



	UpdateWindow(hWnd);

	return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE: Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_HOTKEY:
		switch (LOWORD(wParam)) {
			case HOTKEY_SETTINGS:
				DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SEARCH), hWnd, WndSearchProc, (LPARAM)&handle);
				break;
			case HOTKEY_CLOSE:
				PostQuitMessage(0);
				break;
		}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code that uses hdc here...
		EndPaint(hWnd, &ps);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void createItem(HWND& hWnd, Questrade::Quotes quotes)
{
	int ypos = 0;
	for (Questrade::Quote quote : quotes.quotes) {
		// Create two lables, one with the ticker and other with the price
		HWND ticker = CreateWindowW(L"static", toWString(quote.symbol).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 0, ypos, 100, 20, hWnd, NULL, NULL, NULL);
		HWND price = CreateWindowW(L"static", std::to_wstring(quote.askPrice).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 100, ypos, 100, 20, hWnd, NULL, NULL, NULL);
		priceLables.push_back(price);
		ypos += 20;
	}
}