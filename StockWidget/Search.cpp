// Search.cpp : Defines the possible actions related to searching for stock tickers.
//

#include "Search.h"
#include "Toolbox.h"
#include "RequestHandler.h"



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

	switch (message)
	{
	case  WM_INITDIALOG:
		//Convert lparam to Authentication variable
		handler = *(Questrade::RequestHandler*)lParam;
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

		case IDC_SEARCH:
			szText = new TCHAR[bufSize];
			GetDlgItemText(hDlg, IDC_TICKER, szText, bufSize);
			//std::string myString = toString(szText);
			MessageBox(NULL, szText, L"JSON parse error", MB_ICONERROR | MB_OK);
			res = handler.searchTicker(toString(szText));
			//MessageBox(NULL, toWString(searchAuth.getApiServer()).c_str(), L"JSON parse error", MB_ICONERROR | MB_OK);

			for (Questrade::Symbol& current : res.symbols)
			{
				SendDlgItemMessage(hDlg, IDC_TICKERLIST, LB_ADDSTRING, 0, (LPARAM)toWString(current.description).c_str());
			}

			break;
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