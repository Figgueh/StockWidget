// StockWidget.cpp : Defines the entry point for the application.
//

#include "api/framework.h"
#include "utility/json.hpp"
#include "utility/RequestError.h"
#include "utility/Toolbox.h"
#include "view/RefreshToken.h"
#include "view/Search.h"
#include "view/Settings.h"
#include "view/StockWidget.h"

#include "controller/StockUpdater.h"

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

	hWnd = CreateWindowW(szWindowClass, szTitle, WS_POPUP | WS_SYSMENU | WS_EX_LAYERED,
		0, 0, 0, 0, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);

	registerHotKeys();
	authenticate();
	loadUserSettings();
	prepareWatchlist();

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

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_HOTKEY:
		switch (LOWORD(wParam))
		{
		case HOTKEY_SETTINGS:
		{
			hwndSettings = DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SETTINGS), hWnd, WndSettingProc, (LPARAM)&configuration);
		}
		break;
		case HOTKEY_STOCKS:
		{
			hwndStocks = DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SEARCH), hWnd, WndSearchProc, (LPARAM)&configuration);

			if (hwndStocks == IDSAVE) {
				applyWatchlistUpdates();
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
		std::vector<StockListing>::iterator it = std::find_if(priceLabels.begin(), priceLabels.end(), [&](const StockListing& listing) {return listing.price == (HWND)lParam; });
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
		if (settings.rememberLocation)
		{
			// Get the positon of the window
			WINDOWPLACEMENT placement;
			GetWindowPlacement(hWnd, &placement);

			// Save it
			configuration.updatePosition(placement);
		}
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
		priceLabels.emplace_back(StockListing{ ticker, price, quote.askPrice });
	}
	UpdateWindow(hWnd);
}

inline void registerHotKeys()
{
	RegisterHotKey(hWnd, HOTKEY_SETTINGS, MOD_CONTROL, 0x53);	// Ctrl + s
	RegisterHotKey(hWnd, HOTKEY_STOCKS, MOD_ALT, 0x53);			// Alt + s
	RegisterHotKey(hWnd, HOTKEY_CLOSE, MOD_ALT, VK_ESCAPE);		//esc

}

inline void authenticate()
{
	std::string refreshToken;

	authentication = Questrade::Authentication::getInstance();

	try {
		authentication->authenticate(configuration.getRefreshToken());
		refreshToken = authentication->getData().getRefreshToken();

	}
	catch (Questrade::AuthenticationError& e) {
		std::string error = std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"Authentication error", MB_ICONERROR | MB_OK);
		DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_REFRESHTOKEN), hWnd, WndRefreshProc, (LPARAM)nullptr);
		refreshToken = authentication->getData().getRefreshToken();

	}
	catch (RequestError& e) {
		std::string error = std::string(e.what());
		MessageBox(NULL, toWString(error).c_str(), L"Bad request error", MB_ICONERROR | MB_OK);
	}
	catch (nlohmann::json::exception& e) {
		refreshToken = authentication->getData().getRefreshToken();

		std::string error = "ERROR:" + std::string(e.what());
	}

	configuration.updateRefreshToken(refreshToken);

}

inline void loadUserSettings()
{
	// Get the settings of the user
	settings = configuration.getSettings();

	// Get the list of ids thats in the watchlist
	watchlist = configuration.getTickers();
}

inline void prepareWatchlist()
{

	// If there isn't anything in the watchlist then open the dialog to allow them to update it
	if (watchlist.empty()) {
		//HandlerPackage package = {handle, &configuration};
		MessageBox(NULL, L"The application couldn't find any stocks for the watchlist.", L"watchlist parse error", MB_ICONERROR | MB_OK);
		int result = DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_SEARCH), hWnd, WndSearchProc, (LPARAM)&configuration);

		// If the save was sucessful update the watchlist.
		if (result == IDSAVE)
			watchlist = configuration.getTickers();
	}

	// Update the length of the window
	::SetWindowPos(hWnd, HWND_TOP, 0, 0, 215, watchlist.size() * 20, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW | SWP_NOMOVE);

	// Load the previously saved location of the window
	if (settings.rememberLocation) {
		WINDOWPLACEMENT placement = configuration.getPosition();
		SetWindowPlacement(hWnd, &placement);
	}

	// Apply size changes
	UpdateWindow(hWnd);

	if (priceLabels.empty())
		initializeWatchlist(std::ref(hWnd), makeRequest.getQuotes(watchlist));

}

inline void applyWatchlistUpdates()
{
	StockWatch::stopWatching();
	OutputDebugStringW(L"Updater closed\n");

	// Remove old labels
	for (StockListing& listings : priceLabels) {
		DestroyWindow(listings.ticker);
		DestroyWindow(listings.price);
	}
	priceLabels.clear();

	// Reinitialize
	StockWatch::running = true;
	watchlist = configuration.getTickers();
	::SetWindowPos(hWnd, HWND_TOP, 0, 0, 215, watchlist.size() * 20, SWP_FRAMECHANGED | SWP_NOZORDER | SWP_SHOWWINDOW);
	initializeWatchlist(hWnd, makeRequest.getQuotes(watchlist));
	StockWatch::updater = std::thread(StockWatch::startWatching, std::ref(hWnd));

	UpdateWindow(hWnd);

}
