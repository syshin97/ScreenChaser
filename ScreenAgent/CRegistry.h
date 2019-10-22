#pragma once
class ScreenAgent;
class CRegistry
{
public:

	CRegKey regKey;

	DWORD dwReturn;
	HKEY hKey;

	void GetRegistry();
	
	void CreateRegKey();
	void OpenRegKey(int typeChoice, int ListType, int LineCount, int RectCount, int Width, int Height, CString str, CString fontStyle, int FontSize,
		int degree, int color_r, int color_g, int color_b, int Outline_r, int Outline_g, int Outline_b, int OutlineSize, int blend);
};

