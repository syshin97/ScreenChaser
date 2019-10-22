#pragma once
#include <Windows.h>

typedef struct regInfo
{
	int typeChoice;
	int ListType;
	int LineCount;
	int RectCount;
	int Width;
	int Height;
	CString str;
	CString fontStyle;
	int FontSize;
	int degree;
	int color_r, color_g, color_b;
	int Outline_r, Outline_g, Outline_b;
	int OutlineSize;
	int blend;
}RegInfo, *pRegInfo;