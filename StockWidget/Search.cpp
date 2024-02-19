// Search.cpp : Defines the possible actions related to searching for stock tickers.
//

#include "Search.h"
#include "Toolbox.h"
#include "RequestHandler.h"
#include "ConfigHandler.h"
#include "resource.h"



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
INT_PTR CALLBACK WndSearchProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static Questrade::RequestHandler handler;
	LPTSTR szText{};
	Questrade::Symbols res;
	int bufSize = 1024;
	ConfigHandler configFile;
	std::vector<int> watchlistID;
	HWND hwndList, hwndWishList;
	int items, lbItem, i, pos;
	std::string text;
	Questrade::Quotes qs;
	Questrade::Symbol ss;

	switch (message)
	{
	case  WM_INITDIALOG:
		//Get the requesthandler variable from that main application
		handler = *(Questrade::RequestHandler*)lParam;

		watchlistID = configFile.getTickers();
		for (int id : watchlistID) {
			Questrade::Quotes quotes = handler.getQuote(id);
			Questrade::Symbol symbol = handler.findStockSymbolWithQuote(quotes.quotes.front(), id);
			int pos = (int)SendDlgItemMessage(hDlg, IDC_WATCHLIST, LB_ADDSTRING, id, (LPARAM)toWString(symbol.symbol + " - " + symbol.description).c_str());
			SendDlgItemMessage(hDlg, IDC_WATCHLIST, LB_SETITEMDATA, pos, (LPARAM)id);
		}

		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			break;
		case IDC_ADDWISHLIST:
			hwndList = GetDlgItem(hDlg, IDC_TICKERLIST);

			// Get selected index.
			lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

			// Get item data.
			i = (int)(SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0));

			qs = handler.getQuote(i);
			ss = handler.findStockSymbolWithQuote(qs.quotes.front(), i);

			pos = (int)SendDlgItemMessage(hDlg, IDC_WATCHLIST, LB_ADDSTRING, lbItem, (LPARAM)toWString(ss.symbol + " - " + ss.description).c_str());
			SendDlgItemMessage(hDlg, IDC_WATCHLIST, LB_SETITEMDATA, pos, i);

			break;
		case IDSAVE:
			hwndWishList = GetDlgItem(hDlg, IDC_WATCHLIST);
			items = (int)SendMessage(hwndWishList, LB_GETCOUNT, 0, 0);
			watchlistID = std::vector<int>();

			for (int cur = 0; cur <= items; cur++) {
				int id = (int)(SendMessage(hwndWishList, LB_GETITEMDATA, cur, 0));
				if (id != -1)
					watchlistID.push_back(id);
			}

			configFile.updateTickers(watchlistID);
			EndDialog(hDlg, LOWORD(wParam));
			break;

		case IDC_SEARCH:
			szText = new TCHAR[bufSize];
			GetDlgItemText(hDlg, IDC_TICKER, szText, bufSize);
			res = handler.searchTicker(toString(szText));

			for (Questrade::Symbol& current : res.symbols){
				if (current.isQuotable && current.isTradable && (current.securityType == "MutualFund" || current.securityType == "Stock")) {
					int pos = (int)SendDlgItemMessage(hDlg, IDC_TICKERLIST, LB_ADDSTRING, current.symbolId, (LPARAM)toWString(current.symbol + " - " + current.description).c_str());
					SendDlgItemMessage(hDlg, IDC_TICKERLIST, LB_SETITEMDATA, pos, (LPARAM)current.symbolId);
				}
			}
			break;

		case IDC_TICKERLIST:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				HWND hwndList = GetDlgItem(hDlg, IDC_TICKERLIST);

				// Get selected index.
				lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

				// Get item data.
				i = (int) (SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0));

				return TRUE;
			}
			}
		}
		default:
			return (INT_PTR)TRUE;
		}
	}
	break;

	case WM_DESTROY:
		EndDialog(hDlg, LOWORD(wParam));
		break;

	default:
		return (INT_PTR)FALSE;
	}
	delete[] szText;


	return 0;
}