#pragma once
#include <Windows.h>

//notepad
typedef struct windowInfo
{
	RECT rect;
	HWND hwnd;
	int z;
}WindowInfo, pWindowInfo;

//excel
typedef struct windowInfo2
{
	RECT rect;
	HWND hwnd;
	int z;

}WindowInfo2, pWindowInfo2;