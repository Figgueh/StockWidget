// StockWidget.cpp : Defines the entry point for the application.
//

#include "api/framework.h"
#include "controller/StockWidget.h"
#include "utility/json.hpp"
#include "utility/Toolbox.h"
#include "utility/RequestError.h"
#include "controller/Search.h"
#include "controller/RefreshToken.h"

#include "controller/StockUpdater.h"


#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
Questrade::Authentication auth;
COLORREF backgroundColor = RGB(255, 240, 255);
//std::thread updater;

INT_PTR hwndSettings = NULL;  // Window handle of dialog box 

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

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

	StockWatch::stopWatching();

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

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP | WS_SYSMENU | WS_EX_LAYERED,
		0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);

	RegisterHotKey(hWnd, HOTKEY_SETTINGS, MOD_ALT, 0x53); //s
	RegisterHotKey(hWnd, HOTKEY_CLOSE, MOD_ALT, VK_ESCAPE); //esc

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
	watchlist = configuration.getTickers();

	// If there isn't anything in the watchlist then open the dialog to allow them to update it
	if (watchlist.empty()) {
		MessageBox(NULL, L"The application couldn't find any stocks for the watchlist.", L"watchlist parse error", MB_ICONERROR | MB_OK);
		int result = DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SEARCH), hWnd, WndSearchProc, (LPARAM)&handle);

		// If the save was sucessful update the watchlist.
		if (result == IDSAVE)
			watchlist = configuration.getTickers();
	}

	// Update the length of the window
	::SetWindowPos(hWnd, HWND_TOP, 0, 0, 215, watchlist.size() * 20, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOMOVE);

	// Load the previously saved location of the window
	WINDOWPLACEMENT placement = configuration.getPosition();
	SetWindowPlacement(hWnd, &placement);

	// Apply size changes
	UpdateWindow(hWnd);

	if (priceLabels.empty())
		initializeWatchlist(std::ref(hWnd), handle.getQuotes(watchlist));

	// Start the updater thread
	StockWatch::updater = std::thread(StockWatch::startWatching, std::ref(hWnd));

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

		//// Makes the main window transparent
		//SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		//SetLayeredWindowAttributes(hWnd, backgroundColor, 0, LWA_COLORKEY);

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_HOTKEY:
		switch (LOWORD(wParam))
		{
		case HOTKEY_SETTINGS:
		{
			hwndSettings = DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SEARCH), hWnd, WndSearchProc, (LPARAM)&handle);

			if (hwndSettings == IDSAVE) {
				StockWatch::stopWatching();
				OutputDebugStringW(L"Updater closed\n");

				// Remove old labels
				for (stockListing& listings : priceLabels) {
					DestroyWindow(listings.ticker);
					DestroyWindow(listings.price);
				}
				priceLabels.clear();

				// Reinitialize
				StockWatch::running = true;
				watchlist = configuration.getTickers();
				::SetWindowPos(hWnd, HWND_TOP, 0, 0, 215, watchlist.size() * 20, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
				initializeWatchlist(hWnd, handle.getQuotes(watchlist));
				StockWatch::updater = std::thread(StockWatch::startWatching, std::ref(hWnd));

				UpdateWindow(hWnd);
			}
		}
		break;
		case HOTKEY_CLOSE:
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			break;
		}
		break;
	case WM_ERASEBKGND:
	{
		SetWindowLong(hWnd, GWL_EXSTYLE, GetWindowLong(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd, backgroundColor, 0, LWA_COLORKEY);
		return TRUE;
	}
	case WM_CTLCOLORSTATIC:
	{
		// Look for the price lables
		std::vector<stockListing>::iterator it = std::find_if(priceLabels.begin(), priceLabels.end(), [&](const stockListing& listing) {return listing.price == (HWND)lParam; });
		HDC hdcStatic = (HDC)wParam;
		COLORREF textColour = RGB(255, 255, 255);

		auto position = it - priceLabels.begin();
		if (it != priceLabels.end()) {			// Colour for stock names
			int length = GetWindowTextLength(priceLabels.at(position).price);
			wchar_t* price = new wchar_t[length + 1];
			GetWindowText(priceLabels.at(position).price, price, length + 1);
			double priceD = wcstod(price, NULL);


			// If it is a gainer turn green
			if (priceD > priceLabels.at(position).lastPrice)
				textColour = RGB(0, 255, 0);

			// If it is a looser turn red
			if (priceD < priceLabels.at(position).lastPrice)
				textColour = RGB(255, 0, 0);

			// No change keep it normal
		}

		SetTextColor(hdcStatic, textColour);

		// Make the background transparent
		SetBkMode(hdcStatic, TRANSPARENT);
		SetDCBrushColor(hdcStatic, backgroundColor);
		return (LRESULT)GetStockObject(DC_BRUSH);
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
	case WM_CLOSE:
	case WM_DESTROY:
	case WM_QUIT:
	{
		// Get the positon of the window
		WINDOWPLACEMENT placement;
		GetWindowPlacement(hWnd, &placement);

		// Save it
		configuration.updatePosition(placement);

		PostQuitMessage(0);
	}
	break;

	case WM_NCHITTEST:
	{
		// Make the main window moveable just by clicking on it.
		LRESULT hit = DefWindowProc(hWnd, message, wParam, lParam);
		if (hit == HTCLIENT) {
			hit = HTCAPTION;

			// Start moving the window when ctrl is pressed.
			if (GetAsyncKeyState(VK_CONTROL) & 0x8000) {
				PostMessage(hWnd, WM_SYSCOMMAND, SC_SIZE + 9, 0);
			}

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
	case WM_SIZE:
	{
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

void initializeWatchlist(HWND hWnd, Questrade::Quotes quotes)
{
	for (Questrade::Quote& quote : quotes.quotes) {
		// Create two lables, one with the ticker and other with the price
		HWND ticker = CreateWindowEx(0, L"static", toWString(quote.symbol).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 0, priceLabels.size() * 20, 100, 20, hWnd, nullptr, nullptr, nullptr);
		HWND price = CreateWindowEx(0, L"static", std::to_wstring(quote.askPrice).c_str(), WS_VISIBLE | WS_CHILD | SS_CENTER, 100, priceLabels.size() * 20, 100, 20, hWnd, nullptr, nullptr, nullptr);
		priceLabels.emplace_back(stockListing{ ticker, price, quote.askPrice });
	}
	UpdateWindow(hWnd);
}
