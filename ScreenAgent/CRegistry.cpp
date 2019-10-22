#include "pch.h"
#include "CRegistry.h"
#include "CSettingDlg.h"

void CRegistry::GetRegistry()
{
	CSettingDlg dlg;

	dlg.typeChoice = AfxGetApp()->GetProfileInt(_T("Screen"), _T("typeChoice"), dlg.typeChoice);
	dlg.ListType = AfxGetApp()->GetProfileInt(_T("Screen"), _T("ListType"), dlg.ListType);

	dlg.LineCount = AfxGetApp()->GetProfileInt(_T("Screen"), _T("LineCount"), dlg.LineCount);
	dlg.RectCount = AfxGetApp()->GetProfileInt(_T("Screen"), _T("RectCount"), dlg.RectCount);

	dlg.Width = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Width"), dlg.Width);
	dlg.Height = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Height"), dlg.Height);

	dlg.str = AfxGetApp()->GetProfileString(_T("Screen"), _T("Text"), dlg.str);
	dlg.fontStyle = AfxGetApp()->GetProfileString(_T("Screen"), _T("FontStyle"), dlg.fontStyle);
	dlg.FontSize = AfxGetApp()->GetProfileInt(_T("Screen"), _T("FontSize"), dlg.FontSize);

	dlg.degree = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Degree"), dlg.degree);

	dlg.color_r = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Color_R"), dlg.color_r);
	dlg.color_g = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Color_G"), dlg.color_g);
	dlg.color_b = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Color_B"), dlg.color_b);

	dlg.Outline_r = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Outline_R"), dlg.Outline_r);
	dlg.Outline_g = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Outline_G"), dlg.Outline_g);
	dlg.Outline_b = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Outline_B"), dlg.Outline_b);

	dlg.OutlineSize = AfxGetApp()->GetProfileInt(_T("Screen"), _T("OutlineSize"), dlg.OutlineSize);
	
	dlg.blend = AfxGetApp()->GetProfileInt(_T("Screen"), _T("blend"), dlg.blend); //³óµµ
}
void CRegistry::CreateRegKey()
{
	dwReturn = 0;
	hKey = NULL;

	dwReturn = ::RegCreateKeyEx(HKEY_CURRENT_USER, _T("Software\\Wowsoft\\ScreenAgent\\Screen"), 0, NULL,
		REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, NULL);

	if (dwReturn != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("ERROR:Failed to create new registry key!"));
		return;
	}
	::RegCloseKey(hKey);

}
void CRegistry::OpenRegKey(int typeChoice, int ListType, int LineCount, int RectCount, int Width, int Height, CString str, CString fontStyle, int FontSize,
	int degree, int color_r, int color_g, int color_b, int Outline_r, int Outline_g, int Outline_b, int OutlineSize, int blend)
{
	dwReturn = ::RegOpenKeyEx(HKEY_CURRENT_USER, _T("Software\\Wowsoft\\ScreenAgent\\Screen"), 0, KEY_WRITE, &hKey);

	if (dwReturn == ERROR_SUCCESS)
	{
		::RegSetValueEx(hKey, _T("typeChoice"), NULL, REG_DWORD, (BYTE*)& typeChoice, sizeof(typeChoice));
		::RegSetValueEx(hKey, _T("ListType"), NULL, REG_DWORD, (BYTE*)& ListType, sizeof(ListType));

		::RegSetValueEx(hKey, _T("LineCount"), NULL, REG_DWORD, (BYTE*)& LineCount, sizeof(LineCount));
		::RegSetValueEx(hKey, _T("RectCount"), NULL, REG_DWORD, (BYTE*)& RectCount, sizeof(RectCount));

		::RegSetValueEx(hKey, _T("Width"), NULL, REG_DWORD, (BYTE*)& Width, sizeof(Width));
		::RegSetValueEx(hKey, _T("Height"), NULL, REG_DWORD, (BYTE*)& Height, sizeof(Height));

		::RegSetValueEx(hKey, _T("Text"), NULL, REG_SZ, (BYTE*)str.GetBuffer(str.GetLength()), str.GetLength() * sizeof(TCHAR));
		::RegSetValueEx(hKey, _T("FontStyle"), NULL, REG_SZ, (BYTE*)fontStyle.GetBuffer(str.GetLength()), fontStyle.GetLength() * sizeof(TCHAR));
		::RegSetValueEx(hKey, _T("FontSize"), NULL, REG_DWORD, (BYTE*)& FontSize, sizeof(FontSize));

		::RegSetValueEx(hKey, _T("Degree"), NULL, REG_DWORD, (BYTE*)& degree, sizeof(degree));

		::RegSetValueEx(hKey, _T("Color_R"), NULL, REG_DWORD, (BYTE*)& color_r, sizeof(color_r));
		::RegSetValueEx(hKey, _T("Color_G"), NULL, REG_DWORD, (BYTE*)& color_g, sizeof(color_g));
		::RegSetValueEx(hKey, _T("Color_B"), NULL, REG_DWORD, (BYTE*)& color_b, sizeof(color_b));

		::RegSetValueEx(hKey, _T("Outline_R"), NULL, REG_DWORD, (BYTE*)& Outline_r, sizeof(Outline_r));
		::RegSetValueEx(hKey, _T("Outline_G"), NULL, REG_DWORD, (BYTE*)& Outline_g, sizeof(Outline_g));
		::RegSetValueEx(hKey, _T("Outline_B"), NULL, REG_DWORD, (BYTE*)& Outline_b, sizeof(Outline_b));

		::RegSetValueEx(hKey, _T("OutlineSize"), NULL, REG_DWORD, (BYTE*)& OutlineSize, sizeof(OutlineSize));

		::RegSetValueEx(hKey, _T("blend"), NULL, REG_DWORD, (BYTE*)& blend, sizeof(blend));

		::RegCloseKey(hKey);

	}
}