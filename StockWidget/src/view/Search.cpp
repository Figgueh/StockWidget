// Search.cpp : Defines the possible actions related to searching for stock tickers.
//
//
//#include "controller/RequestHandler.h"
//#include "resource.h"
//#include "utility/Toolbox.h"
//#include "view/Search.h"
//#include "view/Settings.h"
//
//
//
////
////  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
////
////  PURPOSE: Processes messages for the main window.
////
////  WM_COMMAND  - process the application menu
////  WM_PAINT    - Paint the main window
////  WM_DESTROY  - post a quit message and return
////
////
//INT_PTR CALLBACK WndSearchProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
//{
//
//	switch (message)
//	{
//	case  WM_INITDIALOG:
//	{
//		//Get the config handler variable from that main application
//		conf = (ConfigHandler*)lParam;
//		loadCurrentWatchlist(hDlg);
//	}
//	break;
//	case WM_COMMAND:
//	{
//		int wmId = LOWORD(wParam);
//
//		// Parse the menu selections:
//		switch (wmId)
//		{
//		case IDOK:
//		case IDCANCEL:
//			EndDialog(hDlg, LOWORD(wParam));
//			break;
//
//		case IDC_ADDWISHLIST:
//			addSelectedToWatchlist(hDlg);
//			break;
//
//		case IDC_REMOVE:
//			removeSelectedFromWatchlist(hDlg);
//			break;
//
//		case IDSAVE:
//			updateWatchlist(hDlg);
//
//			EndDialog(hDlg, LOWORD(wParam));
//			break;
//
//		case IDC_SEARCH:
//			displaySearchResults(hDlg);
//			break;
//
//		case IDC_TICKERLIST:
//		{
//			switch (HIWORD(wParam))
//			{
//			case LBN_SELCHANGE:
//			{
//				HWND hwndList = GetDlgItem(hDlg, IDC_TICKERLIST);
//
//				// Get selected index.
//				int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
//
//				// Get item data.
//				(int)(SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0));
//
//				return TRUE;
//			}
//			}
//		}
//		break;
//		case IDC_ADVANCED:
//		{
//			DialogBoxW(GetModuleHandle(NULL), (LPWSTR)IDD_SETTINGS, hDlg, WndSettingProc);
//
//		}
//		default:
//			return (INT_PTR)TRUE;
//		}
//	}
//	break;
//
//	case WM_DESTROY:
//		EndDialog(hDlg, LOWORD(wParam));
//		break;
//
//	default:
//		return (INT_PTR)FALSE;
//	}
//
//
//	return 0;
//}
//
//inline void loadCurrentWatchlist(HWND hDlg)
//{
//
//	for (int id : conf->getTickers()) {
//		Quote quote = api.getQuote(id);
//		Symbol symbol = api.findStockSymbolWithQuote(quote, id);
//		int pos = (int)SendDlgItemMessage(hDlg, IDC_WATCHLIST, LB_ADDSTRING, id, (LPARAM)toWString(symbol.m_symbol + " - " + symbol.m_name).c_str());
//		SendDlgItemMessage(hDlg, IDC_WATCHLIST, LB_SETITEMDATA, pos, (LPARAM)id);
//	}
//
//}
//
//inline void addSelectedToWatchlist(HWND hDlg)
//{
//	HWND hwndQuerrylist = GetDlgItem(hDlg, IDC_TICKERLIST);
//	Quote quote;
//	Symbol symbol;
//	int selectedIndex, selectedData, newEntry;
//
//	// Get selected index.
//	selectedIndex = (int)SendMessage(hwndQuerrylist, LB_GETCURSEL, 0, 0);
//
//	// Get item data.
//	selectedData = (int)(SendMessage(hwndQuerrylist, LB_GETITEMDATA, selectedIndex, 0));
//
//	quote = api.getQuote(selectedData);
//	symbol = api.findStockSymbolWithQuote(quote, selectedData);
//
//	newEntry = (int)SendDlgItemMessage(hDlg, IDC_WATCHLIST, LB_ADDSTRING, selectedIndex, (LPARAM)toWString(symbol.m_symbol + " - " + symbol.m_name).c_str());
//	SendDlgItemMessage(hDlg, IDC_WATCHLIST, LB_SETITEMDATA, newEntry, selectedData);
//}
//
//inline void removeSelectedFromWatchlist(HWND hDlg)
//{
//	// Get the selected index.
//	HWND hwndList = GetDlgItem(hDlg, IDC_WATCHLIST);
//	int index = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);
//
//	// Remove the item
//	SendMessage(hwndList, LB_DELETESTRING, index, 0);
//}
//
//inline void updateWatchlist(HWND hDlg)
//{
//	HWND hwndWatchlist = GetDlgItem(hDlg, IDC_WATCHLIST);
//	int items = (int)SendMessage(hwndWatchlist, LB_GETCOUNT, 0, 0);
//	std::vector<int> watchlist = std::vector<int>();
//
//	for (int cur = 0; cur <= items; cur++) {
//		int id = (int)(SendMessage(hwndWatchlist, LB_GETITEMDATA, cur, 0));
//		if (id != -1)
//			watchlist.push_back(id);
//	}
//
//	conf->updateTickers(watchlist);
//}
//
//inline void displaySearchResults(HWND hDlg)
//{
//	LPTSTR szText{};
//	Symbols res;
//	int bufSize = 1024;
//
//	szText = new TCHAR[bufSize];
//	GetDlgItemText(hDlg, IDC_TICKER, szText, bufSize);
//	res = api.searchTicker(toString(szText));
//
//	// clear the listbox.
//	SendDlgItemMessage(hDlg, IDC_TICKERLIST, LB_RESETCONTENT, 0, 0);
//
//	for (Symbol& current : res.m_symbols) {
//
//		int pos = (int)SendDlgItemMessage(hDlg, IDC_TICKERLIST, LB_ADDSTRING, 0, (LPARAM)toWString(current.m_symbol + " - " + current.m_name).c_str());
//		SendDlgItemMessage(hDlg, IDC_TICKERLIST, LB_SETITEMDATA, pos, 0);
//
//
//		//if (current.isQuotable && current.isTradable && (current.securityType == "MutualFund" || current.securityType == "Stock")) {
//		//	int pos = (int)SendDlgItemMessage(hDlg, IDC_TICKERLIST, LB_ADDSTRING, current.symbolId, (LPARAM)toWString(current.symbol + " - " + current.description).c_str());
//		//	SendDlgItemMessage(hDlg, IDC_TICKERLIST, LB_SETITEMDATA, pos, (LPARAM)current.symbolId);
//		//}
//	}
//	delete[] szText;
//
//}
