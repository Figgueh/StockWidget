#include "controller/Settings.h"
#include "api/Resource.h"

LRESULT CALLBACK WndSettingProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_COLOUR:
		{
			//CHOOSECOLOR cc;                 // common dialog box structure 
			//static COLORREF acrCustClr[16]; // array of custom colors 
			//HWND hwnd;                      // owner window
			//HBRUSH hbrush;                  // brush handle
			//static DWORD rgbCurrent;        // initial color selection

			//// Initialize CHOOSECOLOR 
			//ZeroMemory(&cc, sizeof(cc));
			//cc.lStructSize = sizeof(cc);
			//cc.hwndOwner = hDlg;
			//cc.lpCustColors = (LPDWORD)acrCustClr;
			//cc.rgbResult = rgbCurrent;
			//cc.Flags = CC_PREVENTFULLOPEN;

			//if (ChooseColor(&cc) == TRUE)
			//{
			//	hbrush = CreateSolidBrush(cc.rgbResult);
			//	rgbCurrent = cc.rgbResult;
			//}
		}
		break;
		default:
			return DefWindowProc(hDlg, message, wParam, lParam);

		}
	}



	case WM_DESTROY:
		EndDialog(hDlg, LOWORD(wParam));
		break;

	default:
		return DefWindowProc(hDlg, message, wParam, lParam);
	}

}