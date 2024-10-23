#include "view/Settings.h"
#include "resource.h"
#include "controller/ConfigHandler.h"

LRESULT WndSettingProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	ConfigHandler config;

	switch (message)
	{
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

			config.updateSettings(settings);
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
