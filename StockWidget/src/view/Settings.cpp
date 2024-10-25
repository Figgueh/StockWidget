#include "resource.h"
#include "view/Settings.h"

LRESULT WndSettingProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case  WM_INITDIALOG:
	{
		// Get the configuration handler variable from that main window 
		configurationHandler = (ConfigHandler*)lParam;

		// Set the boxes state to reflect the information from configuration file
		ApplicationSettings settings = configurationHandler->getSettings();
		if (settings.alwaysOnTop)
			CheckDlgButton(hDlg, IDC_ALWAYS_TOP, BST_CHECKED);

		if (settings.rememberLocation)
			CheckDlgButton(hDlg, IDC_REMEBER, BST_CHECKED);
	}
	break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);

		// Parse the menu selections:
		switch (wmId)
		{
		case IDSAVESETTINGS:
			ApplicationSettings settings;
			settings.alwaysOnTop = IsDlgButtonChecked(hDlg, IDC_ALWAYS_TOP);
			settings.rememberLocation = IsDlgButtonChecked(hDlg, IDC_REMEBER);

			configurationHandler->updateSettings(settings);
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
