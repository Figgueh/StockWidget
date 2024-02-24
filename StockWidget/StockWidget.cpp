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
COLORREF backgroundColor = RGB(255, 240, 255);
std::thread updater;
std::vector<int> watchlistG;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
int createItem(HWND& hWnd, Questrade::Quotes quotes, std::vector<stockListing>* priceLabels);

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

	{
		std::lock_guard<std::mutex> guard(mymutex);
		running = false;
	}
	mycond.notify_all();
	//running = false;
	updater.join();

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
	wcex.hbrBackground = CreateSolidBrush(backgroundColor);
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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP | WS_SYSMENU,
		CW_USEDEFAULT, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

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

	}
	catch (Questrade::AuthenticationError& e) {
		std::string error = std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"Authentication error", MB_ICONERROR | MB_OK);
		DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_REFRESHTOKEN), hWnd, WndRefreshProc, (LPARAM)&auth);
		refreshToken = auth.getRefreshToken();

	}
	catch (RequestError& e) {
		std::string error = std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"Bad request error", MB_ICONERROR | MB_OK);
	}
	catch (nlohmann::json::exception& e) {
		refreshToken = auth.getRefreshToken();

		configuration.updateRefreshToken(refreshToken);
		std::string error = "ERROR:" + std::string(e.what());
	}

	configuration.updateRefreshToken(refreshToken);
	handle = Questrade::RequestHandler(auth);


	// Get the list of ids thats in the watchlist
	std::vector<int> watchlist = configuration.getTickers();
	watchlistG = watchlist;

	// If there isn't anything in the watchlist then open the dialog to allow them to update it
	if (watchlist.empty()) {
		MessageBox(NULL, L"The application couldn't find any stocks for the watchlist.", L"watchlist parse error", MB_ICONERROR | MB_OK);
		DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SEARCH), hWnd, WndSearchProc, (LPARAM)&handle);
	}


	// Get their quotes
	Questrade::Quotes watchlistQuotes = handle.getQuotes(watchlist);
	std::vector<stockListing> priceLabels;

	int y = createItem(hWnd, watchlistQuotes, &priceLabels);

	::SetWindowPos(hWnd, nullptr, 0, 0, 215, y, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOZORDER);

	UpdateWindow(hWnd);

	updater = std::thread(updateWatchlistPrice, std::move(priceLabels));

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
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		// Makes the main window transparent
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, backgroundColor, 0, LWA_COLORKEY);

		EndPaint(hWnd, &ps);
	}
	break;
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
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam;
		SetTextColor((HDC)wParam, RGB(255, 255, 255));
		SetBkMode(hdcStatic, TRANSPARENT);
		return (LRESULT)GetStockObject(HOLLOW_BRUSH);
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
	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	case WM_NCHITTEST: {
		// Make the main window moveable just by clicking on it.
		LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
		if (hit == HTCLIENT) {
			hit = HTCAPTION;

			// Make background of main transparent and visible when moving it around.
			if ((GetAsyncKeyState(VK_LBUTTON) & 0x8000)) {
				SetLayeredWindowAttributes(hWnd, RGB(255, 255, 255), 150, LWA_ALPHA);
				SetTextColor((HDC)wParam, RGB(0, 0, 0));
			}
			else {
				SetLayeredWindowAttributes(hWnd, backgroundColor, 0, LWA_COLORKEY);
			}
		}

		return hit;
	}
					 break;
	case WM_SIZE: {
		// Make rounded corners.
		RECT wRect;
		if (::GetWindowRect(hWnd, &wRect)) {
			HRGN hRgn = ::CreateRoundRectRgn(wRect.left, wRect.top, wRect.right, wRect.bottom, 30, 30);
			::SetWindowRgn(hWnd, hRgn, TRUE);
			::DeleteObject(hRgn);
		}
	}
				break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}



int createItem(HWND& hWnd, Questrade::Quotes quotes, std::vector<stockListing>* outStockListings)
{
	int ypos = 0;
	for (Questrade::Quote& quote : quotes.quotes) {
		// Create two lables, one with the ticker and other with the price
		HWND ticker = CreateWindowW(L"static", toWString(quote.symbol).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 0, ypos, 100, 20, hWnd, nullptr, nullptr, nullptr);
		HWND price = CreateWindowW(L"static", std::to_wstring(quote.askPrice).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 100, ypos, 100, 20, hWnd, nullptr, nullptr, nullptr);
		outStockListings->emplace_back(stockListing{ ticker, price });
		ypos += 20;
	}

	return ypos;
}

void updateWatchlistPrice(std::vector<stockListing> priceLabels)
{
	while (running)
	{
		Questrade::Quotes watchListQuotes = handle.getQuotes(watchlistG);

		for (Questrade::Quote& quote : watchListQuotes.quotes)
		{
			for (stockListing& listing : priceLabels)
			{
				int length = GetWindowTextLength(listing.ticker);
				wchar_t* ticker = new wchar_t[length + 1];
				GetWindowText(listing.ticker, ticker, length + 1);

				if (ticker == toWString(quote.symbol))
					SetWindowTextA(listing.price, std::to_string(quote.askPrice).c_str());
			}
		}

		OutputDebugStringW(L"Updated\n");
		{
			std::unique_lock<std::mutex> lock(mymutex);
			mycond.wait_for(lock, std::chrono::seconds(10));
		}
	}


}