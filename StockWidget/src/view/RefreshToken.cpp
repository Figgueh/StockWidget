#include "view/RefreshToken.h"
#include "utility/Toolbox.h"


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
INT_PTR WndRefreshProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	LPTSTR szText{};
	int bufSize = 1024;

	switch (message)
	{

	case  WM_INITDIALOG:
		//Get the requesthandler variable from that main application
		auth = *(Questrade::Authentication*)lParam;
		break;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case IDCANCEL:
			MessageBox(NULL, L"In order to procced this application requires a valid refresh token. \nThe application will now terminate.", L"bad request error", MB_ICONERROR | MB_OK);
			PostQuitMessage(0);
			break;

		case IDSAVE:
			szText = new TCHAR[bufSize];
			GetDlgItemText(hDlg, IDC_EDIT, szText, bufSize);

			try {
				auth = Questrade::Authentication::authenticate(szText);
				EndDialog(hDlg, LOWORD(wParam));
			}
			catch (Questrade::AuthenticationError& e) {
				std::string error = std::string(e.what());
				MessageBox(NULL, toWString(error).c_str(), L"bad request error", MB_ICONERROR | MB_OK);
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

