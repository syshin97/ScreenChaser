// CScreen.cpp: 구현 파일
//

#include "pch.h"
#include "ScreenAgent.h"
#include "CScreen.h"
#include "afxdialogex.h"
#include "BufferDC.h"
#include "CSettingDlg.h"
#include "ScreenAgentDlg.h"
#include "ComeOnWindow.h"

#include <iostream>


int arr[] = {0,1,2,3,4,5,6,7,8};
#define N 9 
// CScreen 대화 상자

IMPLEMENT_DYNAMIC(CScreen, CDialog)

CScreen::CScreen(CSettingDlg* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_SCREEN, pParent)
{
	m_pParentWnd = pParent;
}

CScreen::~CScreen()
{
}
void CScreen::swap(int *a, int *b)
{
	int temp;
	temp = *a;
	*a = *b;
	*b = temp;
}

void CScreen::permutation(int n, int r, int depth, CRgn *rgn[])
{
	//CBufferDC pdc(this);

	if (r == depth) {

		CRgn Rgn;
		CPoint points[8];
		Rgn.CreatePolygonRgn(points, 8, WINDING);

		for (int i = 0; i < depth/2; i++)
		{
			str.Format(_T("%d %d"), arr[i],arr[i+1]);
			OutputDebugString(str);
			Rgn.CombineRgn(rgn[arr[i]], rgn[arr[i+1]], RGN_AND);
			
		}
		//pdc.SelectClipRgn(&Rgn);
		return ;
		
	}
	
	for (int i = depth; i < n; i++) {
		swap(&arr[i], &arr[depth]);
		permutation(n, r, depth + 1, rgn);
		swap(&arr[i], &arr[depth]);
	}

	
}
void CScreen::SetData(RegInfo data)
{
	m_MainData.typeChoice = data.typeChoice;
	m_MainData.ListType = data.ListType;

	m_MainData.LineCount = data.LineCount;
	m_MainData.RectCount = data.RectCount;

	m_MainData.Width = data.Width;
	m_MainData.Height = data.Height;

	m_MainData.str = data.str;
	m_MainData.fontStyle = data.fontStyle;
	m_MainData.FontSize = data.FontSize;
	m_MainData.degree = data.degree;
	m_MainData.color_r = data.color_r;
	m_MainData.color_g = data.color_g;
	m_MainData.color_b = data.color_b;
	m_MainData.Outline_r = data.Outline_r;
	m_MainData.Outline_g = data.Outline_g;
	m_MainData.Outline_b = data.Outline_b;

	m_MainData.OutlineSize = data.OutlineSize;

	m_MainData.blend = data.blend;
}

void CScreen::SetPosition(int width, int height, int LineCount,int RectCount, 
	 int Width, int Height, CString str, CString fontStyle, int FontSize, int degree)
{
	m_vStringPos.clear();

	CPaintDC dc(this);


	if (RectCount % LineCount == 0)   //나머지가 나누어떨어지면 줄 추가 x
		rm = 0;                        // 나머지가 하나라도 있으면 줄추가 o 
	else
		rm = 1;

	x = width / (LineCount * 3);                                     //3은 사각형 한 변이 영역의 1/3이 되어야하기때문 ,x = 사각형 가로
	y = height / (((RectCount / LineCount) + rm) * 3);//사각형 세로

	/*x2 = width / LineCount;
	y2 = height / ((RectCount / LineCount) + rm);*/

	x2 = width / Width;
	y2 = height / Height;

	
}


BOOL CScreen::OnInitDialog()
{
	CDialog::OnInitDialog();
	//AfxMessageBox(_T("aaa"));

	ModifyStyleEx(0, WS_EX_TRANSPARENT);
	LONG ExtendedStyle = GetWindowLong(GetSafeHwnd(), GWL_EXSTYLE);

	SetWindowLong(GetSafeHwnd(), GWL_EXSTYLE, ExtendedStyle | WS_EX_LAYERED | WS_EX_TOPMOST);

	COLORREF pcrKey;
	BYTE pbAlpha;
	DWORD pdwFlags;
	::GetLayeredWindowAttributes(GetSafeHwnd(), &pcrKey, &pbAlpha, &pdwFlags);

	::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(0, 0, 0), m_MainData.blend, LWA_COLORKEY | LWA_ALPHA);
	//::SetLayeredWindowAttributes( GetSafeHwnd(),RGB(240, 240, 240), 100, LWA_COLORKEY |LWA_ALPHA );

	::SetWindowPos(m_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);

	return TRUE;
}


void CScreen::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CScreen, CDialog)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CScreen 메시지 처리기

//빠른번호 일수록 위
int CScreen::GetZOrder(HWND hTarget)
{	

	int index = 0;
	
	HWND hNext = ::GetTopWindow(NULL);
//	TCHAR str[MAX_PATH]{ 0, };
//	_stprintf_s(str, _T("--------------->>  %d ---- %d"), m_vWindowHandle[0].z, m_vWindowHandle[1].z);
	
	
	do
	{
		hNext = ::GetNextWindow(hNext, GW_HWNDNEXT);
		if (hNext == hTarget)
			break;

		++index;

	} while (hNext);

	/*CString str;
	str.Format(_T("TEST [0x%X] [%d]"), hTarget, index);
	OutputDebugString(str);*/

	return index;
	
}

void CScreen::OnPaint()
{
	CBufferDC pdc(this);
	CPaintDC dc(this);
	//CPaintDC pdc(this);	
	CRect Rect;

	//CPen myPen, * pOldPen;
	//myPen.CreatePen(PS_SOLID, 5, RGB(0, 0, 255));
	//pOldPen = pdc.SelectObject(&myPen);

	//CFont font, * pOldFont;
	//font.CreateFont(          // object의 생성
	//	15,                  // 문자 폭
	//	15,                  // 문자 높이 
	//	50,                  // 기울기 각도
	//	0,                   // 문자 방향
	//	FW_HEAVY,            // 문자 굵기
	//	FALSE,               // 문자 기울림 모양
	//	FALSE,               // 밑 줄
	//	FALSE,               // 취소선
	//	DEFAULT_CHARSET,     // 문자 셋
	//	OUT_DEFAULT_PRECIS,  // 출력 정확도
	//	CLIP_DEFAULT_PRECIS, // 킬립핑 정확도
	//	DEFAULT_QUALITY,     // 출력의 질
	//	DEFAULT_PITCH,       // 자간 거리
	//	_T("궁서")               // 문자 모양
	//);
	//pOldFont = (CFont*)pdc.SelectObject(&font);

	//pdc.SetTextColor(RGB(255, 0, 0));
	//pdc.SetBkMode(TRANSPARENT);
	
	::SetLayeredWindowAttributes(GetSafeHwnd(), RGB(0, 0, 0), m_MainData.blend * 2.55, LWA_COLORKEY | LWA_ALPHA);
	
	int size2 = m_vWindowHandle2.size(); //excel 총 개수
	int size3 = m_vWindowHandle.size();  //notepad 총 개수

	str.Format(_T("excel[%d]   notepad[%d]"), size2, size3);
	OutputDebugString(str);

	int MonitoCount = GetSystemMetrics(SM_CMONITORS);
	DISPLAY_DEVICE dd;
	DEVMODE dm;
	CRect* MonitorsRect = NULL;
	MonitorsRect = new CRect[MonitoCount];

	for (int i = 0, j = 0; i < MonitoCount; i++)
	{
		memset(&dd, 0, sizeof(DISPLAY_DEVICE));
		dd.cb = sizeof(DISPLAY_DEVICE);
		EnumDisplayDevices(NULL, i, &dd, 0);
		memset(&dm, 0, sizeof(DEVMODE));
		if (EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm))
		{
			MonitorsRect[j].left = dm.dmPosition.x;
			MonitorsRect[j].top = dm.dmPosition.y;
			MonitorsRect[j].right = dm.dmPosition.x + dm.dmPelsWidth;
			MonitorsRect[j].bottom = dm.dmPosition.y + dm.dmPelsHeight;
			j++;
		}
	}
	//모니터 사이즈 구하기

	int nMainX = 0;
	int nMainY = 0;
	int nMainWidth = 0;
	int nMainHeight = 0;

	
	for (int i = 0; i < MonitoCount; i++)
	{
		if (0 > MonitorsRect[i].left)
			nMainX = MonitorsRect[i].left;
		nMainY += MonitorsRect[i].top;
		nMainWidth += MonitorsRect[i].right - MonitorsRect[i].left;
		nMainHeight += (-MonitorsRect[0].top) + MonitorsRect[0].bottom;
	}
	
	
	//CRgn rgn1, rgn2, rgn3, rgn4;  //모서리 
	//CRgn rgn5, rgn6, rgn7, rgn8;              //변 
	//CRgn rgn9, rgn10, rgn11; //내부
	CRgn rgn, rgn1, rgn2, rgn3, rgn4;

	for (int i = 0; i < size2; i++)
	{
		/*for (int j = 0; j < size3; j++)
		{*/

			CRect rt(m_vWindowHandle2[i].rect.left, m_vWindowHandle2[i].rect.top,
				m_vWindowHandle2[i].rect.right, m_vWindowHandle2[i].rect.bottom);

			/*CRect rt2(m_vWindowHandle[j].rect.left, m_vWindowHandle[j].rect.top,
				m_vWindowHandle[j].rect.right, m_vWindowHandle[j].rect.bottom); */

			CRect rt_window(nMainX, nMainY, nMainWidth, nMainHeight);

			CRect overlapRect(0, 0, 0, 0);


			CRgn rgn_window;
			CPoint point[4];
			point[0] = CPoint(rt_window.left, rt_window.top);
			point[1] = CPoint(rt_window.right, rt_window.top);
			point[2] = CPoint(rt_window.right, rt_window.bottom);
			point[3] = CPoint(rt_window.left, rt_window.bottom);

			rgn_window.CreatePolygonRgn(point, 4, WINDING);

			/*str.Format(_T("%d     %d"), m_vWindowHandle2[i].z, m_vWindowHandle[j].z);
			OutputDebugString(str);*/
		   
			if (m_MainData.ListType == 1)
			{

				/*if (m_vWindowHandle2[i].z < m_vWindowHandle[j].z)
				{*/
				
			/*		CPoint points[4];
					points[0] = CPoint(rt.left, rt.top);
					points[1] = CPoint(rt.right, rt.top);
					points[2] = CPoint(rt.right, rt.bottom);
					points[3] = CPoint(rt.left, rt.bottom);

					rgn.CreatePolygonRgn(points, 4, WINDING);
					rgn.CombineRgn(&rgn_window, &rgn, RGN_AND);
					pdc.SelectClipRgn(&rgn);*/


					/*CPoint points[4];
					points[0] = CPoint(rt.left, rt.top);
					points[1] = CPoint(rt.right, rt.top);
					points[2] = CPoint(rt.right, rt.bottom);
					points[3] = CPoint(rt.left, rt.bottom);

					rgn.CreatePolygonRgn(points, 4, WINDING);

					CRgn RGN, RGN2;
					CPoint points2[8];
					rgn3.CreatePolygonRgn(points2, 8, WINDING);
					rgn3.CombineRgn(&rgn_window, &rgn, RGN_AND);

					CRgn* rgn_rt = &rgn_window;
					
					for (int i = 0; i < size3; i++)
					{

						CPoint points3[4];
						points3[0] = CPoint(rt2.left, rt2.top);
						points3[1] = CPoint(rt2.right, rt2.top);
						points3[2] = CPoint(rt2.right, rt2.bottom);
						points3[3] = CPoint(rt2.left, rt2.bottom);


						rgn_rt[i].CreatePolygonRgn(points3, 4, WINDING);
						rgn_rt[i].CombineRgn(&rgn_rt[i], &rgn_rt[i+1], RGN_OR);
						
						
						CPoint points4[20];
						rgn4.CreatePolygonRgn(points4, 20, WINDING);
			
					}
					rgn4.CombineRgn(&rgn3, rgn_rt, RGN_DIFF);
					pdc.SelectClipRgn(&rgn4);*/






					
				       

					//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
					//	rt.top < rt2.top && rt.right <= rt2.right && rt.bottom <= rt2.bottom)
					//{

					//	CPoint points[6];
					//	points[0] = CPoint(rt.right, rt.top);
					//	points[1] = CPoint(rt.left, rt.top);
					//	points[2] = CPoint(rt.left, rt.bottom);
					//	points[3] = CPoint(overlapRect.left, overlapRect.bottom);
					//	points[4] = CPoint(overlapRect.left, overlapRect.top);
					//	points[5] = CPoint(overlapRect.right, overlapRect.top);
					//	//CRgn rgn;

					//	rgn1.CreatePolygonRgn(points, 6, WINDING);

					//	//	pdc.SelectClipRgn(&rgn1);
					//}

					//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left >= rt2.left &&
					//	rt.top < rt2.top && rt.right > rt2.right && rt.bottom <= rt2.bottom)
					//{
					//	CPoint points2[6];
					//	points2[0] = CPoint(rt.left, rt.top);
					//	points2[1] = CPoint(rt.right, rt.top);
					//	points2[2] = CPoint(rt.right, rt.bottom);
					//	points2[3] = CPoint(overlapRect.right, overlapRect.bottom);
					//	points2[4] = CPoint(overlapRect.right, overlapRect.top);
					//	points2[5] = CPoint(overlapRect.left, overlapRect.top);

					//	//CRgn rgn2;

					//	rgn2.CreatePolygonRgn(points2, 6, WINDING);

					//	//	pdc.SelectClipRgn(&rgn2);
					//}

					//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
					//	rt.top >= rt2.top && rt.right <= rt2.right && rt.bottom > rt2.bottom)
					//{
					//	CPoint points[6];
					//	points[0] = CPoint(rt.left, rt.top);
					//	points[1] = CPoint(rt.left, rt.bottom);
					//	points[2] = CPoint(rt.right, rt.bottom);
					//	points[3] = CPoint(overlapRect.right, overlapRect.bottom);
					//	points[4] = CPoint(overlapRect.left, overlapRect.bottom);
					//	points[5] = CPoint(overlapRect.left, overlapRect.top);

					//	rgn3.CreatePolygonRgn(points, 6, WINDING);

					//	//	pdc.SelectClipRgn(&rgn3);
					//}

					//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left >= rt2.left &&
					//	rt.top >= rt2.top && rt.right > rt2.right && rt.bottom > rt2.bottom)
					//{
					//	CPoint points[6];
					//	points[0] = CPoint(overlapRect.right, overlapRect.top);
					//	points[1] = CPoint(overlapRect.right, overlapRect.bottom);
					//	points[2] = CPoint(overlapRect.left, overlapRect.bottom);
					//	points[3] = CPoint(rt.left, rt.bottom);
					//	points[4] = CPoint(rt.right, rt.bottom);
					//	points[5] = CPoint(rt.right, rt.top);

					//	rgn4.CreatePolygonRgn(points, 6, WINDING);

					//	//	pdc.SelectClipRgn(&rgn4);
					//}


					//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
					//	rt.top < rt2.top && rt.right > rt2.right && rt.bottom < rt2.bottom)
					//{

					//	CPoint points[8];
					//	points[0] = CPoint(rt.left, rt.top);
					//	points[1] = CPoint(rt.left, rt.bottom);
					//	points[2] = CPoint(overlapRect.left, overlapRect.bottom);
					//	points[3] = CPoint(overlapRect.left, overlapRect.top);
					//	points[4] = CPoint(overlapRect.right, overlapRect.top);
					//	points[5] = CPoint(overlapRect.right, overlapRect.bottom);
					//	points[6] = CPoint(rt.right, rt.bottom);
					//	points[7] = CPoint(rt.right, rt.top);

					//	rgn5.CreatePolygonRgn(points, 8, WINDING);

					//	//	pdc.SelectClipRgn(&rgn5);
					//}

					//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left > rt2.left &&
					//	rt.top < rt2.top && rt.right > rt2.right && rt.bottom > rt2.bottom)
					//{

					//	CPoint points[8];
					//	points[0] = CPoint(rt.left, rt.top);
					//	points[1] = CPoint(rt.right, rt.top);
					//	points[2] = CPoint(rt.right, rt.bottom);
					//	points[3] = CPoint(rt.left, rt.bottom);
					//	points[4] = CPoint(overlapRect.left, overlapRect.bottom);
					//	points[5] = CPoint(overlapRect.right, overlapRect.bottom);
					//	points[6] = CPoint(overlapRect.right, overlapRect.top);
					//	points[7] = CPoint(overlapRect.left, overlapRect.top);

					//	rgn6.CreatePolygonRgn(points, 8, WINDING);

					//	//	pdc.SelectClipRgn(&rgn6);

					//}

					//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
					//	rt.top > rt2.top && rt.right > rt2.right && rt.bottom > rt2.bottom)
					//{
					//	CPoint points[8];
					//	points[0] = CPoint(rt.left, rt.top);
					//	points[1] = CPoint(rt.left, rt.bottom);
					//	points[2] = CPoint(rt.right, rt.bottom);
					//	points[3] = CPoint(rt.right, rt.top);
					//	points[4] = CPoint(overlapRect.right, overlapRect.top);
					//	points[5] = CPoint(overlapRect.right, overlapRect.bottom);
					//	points[6] = CPoint(overlapRect.left, overlapRect.bottom);
					//	points[7] = CPoint(overlapRect.left, overlapRect.top);

					//	rgn7.CreatePolygonRgn(points, 8, WINDING);

					//	//	pdc.SelectClipRgn(&rgn7);
					//}

					////rt 오른쪽 위에 rt2 (rt > rt2)

					//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
					//	rt.top < rt2.top && rt.right < rt2.right && rt.bottom > rt2.bottom)
					//{
					//	CPoint points[8];
					//	points[0] = CPoint(rt.right, rt.top);
					//	points[1] = CPoint(rt.left, rt.top);
					//	points[2] = CPoint(rt.left, rt.bottom);
					//	points[3] = CPoint(rt.right, rt.bottom);
					//	points[4] = CPoint(overlapRect.right, overlapRect.bottom);
					//	points[5] = CPoint(overlapRect.left, overlapRect.bottom);
					//	points[6] = CPoint(overlapRect.left, overlapRect.top);
					//	points[7] = CPoint(overlapRect.right, overlapRect.top);

					//	rgn8.CreatePolygonRgn(points, 8, WINDING);

					//	//	pdc.SelectClipRgn(&rgn8);
					//}

					//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
					//	rt.top < rt2.top && rt.right > rt2.right && rt.bottom > rt2.bottom)
					//{
					//	CPoint points[4];
					//	points[0] = CPoint(rt.left, rt.top);
					//	points[1] = CPoint(rt.left, rt.bottom);
					//	points[2] = CPoint(rt.right, rt.bottom);
					//	points[3] = CPoint(rt.right, rt.top);

					//	rgn10.CreatePolygonRgn(points, 4, WINDING);

					//	CPoint points2[4];
					//	points2[0] = CPoint(rt2.left, rt2.top);
					//	points2[1] = CPoint(rt2.left, rt2.bottom);
					//	points2[2] = CPoint(rt2.right, rt2.bottom);
					//	points2[3] = CPoint(rt2.right, rt2.top);

					//	rgn11.CreatePolygonRgn(points2, 4, WINDING);

					//	CPoint points3[8];
					//	rgn9.CreatePolygonRgn(points3, 8, WINDING);
					//	rgn9.CombineRgn(&rgn10, &rgn11, RGN_DIFF);

					//	//pdc.SelectClipRgn(&rgn3);
					//}
					//CRgn* pRgn[] = { &rgn1, &rgn2, &rgn3, &rgn4, &rgn5, &rgn6, &rgn7, &rgn8, &rgn9 };
					//CRgn Rgn, Rgn1, Rgn2, Rgn3, Rgn4, Rgn5, Rgn6, Rgn7;
					//CPoint points[8];
					//Rgn.CreatePolygonRgn(points, 8, WINDING);

					//CPoint points2[10];
					//Rgn1.CreatePolygonRgn(points, 8, WINDING);
					//Rgn2.CreatePolygonRgn(points, 8, WINDING);
					//Rgn3.CreatePolygonRgn(points, 8, WINDING);
					//Rgn4.CreatePolygonRgn(points2, 10, WINDING);
					//Rgn5.CreatePolygonRgn(points2, 10, WINDING);
					//Rgn6.CreatePolygonRgn(points2, 10, WINDING);
					//Rgn7.CreatePolygonRgn(points2, 10, WINDING);

					//if (size3 == 1)
					//{
					//	int i, j;
					//	for (i = 0; i < N; i++) {

					//		Rgn.CombineRgn(pRgn[arr[i]], pRgn[arr[i]], RGN_AND);
					//	}
					//	pdc.SelectClipRgn(&Rgn);
					//	

					//}
					//if (size3 == 2)
					//{
					//	int i, j;
					//	for (i = 0; i < N; i++) {
					//		for (j = 0; j < N; j++) {
					//			if (i == j) continue;

					//			Rgn.CombineRgn(pRgn[arr[i]], pRgn[arr[j]], RGN_AND);
					//		}
					//	}
					//	
					//	pdc.SelectClipRgn(&Rgn);
					//
					//}

					//if (size3 == 3)
					//{
					//	int i, j, k;
					//	for (i = 0; i < N; i++) {
					//		for (j = 0; j < N; j++) {
					//			if (i == j) continue;
					//			for (k = 0; k < N; k++) {
					//				if (i == k) continue;
					//				if (j == k) continue;

					//				Rgn1.CombineRgn(pRgn[arr[i]], pRgn[arr[j]], RGN_AND);
					//				Rgn2.CombineRgn(pRgn[arr[i]], pRgn[arr[k]], RGN_AND);
					//				Rgn3.CombineRgn(pRgn[arr[j]], pRgn[arr[k]], RGN_AND);

					//				Rgn4.CombineRgn(&Rgn1, pRgn[arr[k]], RGN_AND);
					//				Rgn5.CombineRgn(&Rgn2, pRgn[arr[j]], RGN_AND);
					//				Rgn6.CombineRgn(&Rgn3, pRgn[arr[i]], RGN_AND);
					//			}
					//		}
					//	}
					//	pdc.SelectClipRgn(&Rgn4);
					//	pdc.SelectClipRgn(&Rgn5);
					//	pdc.SelectClipRgn(&Rgn6);
				
					//}

					//if (size3 == 4)
					//{
					//	int i, j, k, l;
					//	for (l = 0; l < N; l++) {
					//		for (i = 0; i < N; i++) {
					//			if (l == i) continue;
					//			for (j = 0; j < N; j++) {
					//				if (i == j) continue;
					//				if (l == j) continue;
					//				for (k = 0; k < N; k++) {
					//					if (i == k) continue;
					//					if (j == k) continue;
					//					if (l == k) continue;

					//					Rgn1.CombineRgn(pRgn[arr[i]], pRgn[arr[j]], RGN_AND);
					//					Rgn1.CombineRgn(&Rgn1, pRgn[arr[k]], RGN_AND);
					//					Rgn2.CombineRgn(&Rgn1, pRgn[arr[l]], RGN_AND);
					//				}
					//			}
					//		}
					//	}
					//	pdc.SelectClipRgn(&Rgn2);
					//
					//	
					//}
					//permutation(sizeof(arr) / sizeof(int), 2, 0, pRgn);

				//}

			 //   if(m_vWindowHandle[j].z < m_vWindowHandle2[i].z)  // 노트가 위에 있을 때
				//{
				
			        CPoint points[4];
				    points[0] = CPoint(rt.left, rt.top);
				    points[1] = CPoint(rt.right, rt.top);
				    points[2] = CPoint(rt.right, rt.bottom);
				    points[3] = CPoint(rt.left, rt.bottom);

				    rgn.CreatePolygonRgn(points, 4, WINDING);
					rgn.CombineRgn(&rgn_window, &rgn, RGN_AND);
					pdc.SelectClipRgn(&rgn);


					//CRgn Rgnn, rgn_1, rgn_2;
			
				    
					vector<WindowInfo>::iterator iWindow1;
					for (iWindow1 = m_vWindowHandle.begin(); iWindow1 != m_vWindowHandle.end(); ++iWindow1)
					{
	
						CRect iWindowRect((*iWindow1).rect.left, (*iWindow1).rect.top,
							(*iWindow1).rect.right, (*iWindow1).rect.bottom);

						/*str.Format(_T("left: %d  %d "), (*iWindow1).z, m_vWindowHandle2[i].z);
						OutputDebugString(str);*/
				
						if ((*iWindow1).z < m_vWindowHandle2[i].z)
						{
							
							str.Format(_T("iWindow Z %d"), (*iWindow1).z);
							OutputDebugString(str);

							//CRgn RGN, RGN2;
							//CPoint points2[8];
							//rgn3.CreatePolygonRgn(points2, 8, WINDING);
							//rgn3.CombineRgn(&rgn_window, &rgn, RGN_AND);

				
							
							////CRgn* rgn_rt = &rgn_window;
							//CRgn rgn_rt;
							//
							//CPoint points3[4];
							//points3[0] = CPoint((*iWindow1).rect.left, (*iWindow1).rect.top);
							//points3[1] = CPoint((*iWindow1).rect.right, (*iWindow1).rect.top);
							//points3[2] = CPoint((*iWindow1).rect.right, (*iWindow1).rect.bottom);
							//points3[3] = CPoint((*iWindow1).rect.left, (*iWindow1).rect.bottom);

							//rgn_rt.CreatePolygonRgn(points3, 4, WINDING);


							////rgn_rt.CombineRgn(&rgn_rt, &rgn_rt, RGN_OR);

							//CPoint points4[20];
							//rgn4.CreatePolygonRgn(points4, 20, WINDING);

							//rgn4.CombineRgn(&rgn3, &rgn_rt, RGN_DIFF);

							if (IntersectRect(overlapRect, rt, iWindowRect))
							{
								CPoint points5[4];
								points5[0] = CPoint(overlapRect.left, overlapRect.top);
								points5[1] = CPoint(overlapRect.right, overlapRect.top);
								points5[2] = CPoint(overlapRect.right, overlapRect.bottom);
								points5[3] = CPoint(overlapRect.left, overlapRect.bottom);
								
								rgn2.CreatePolygonRgn(points5, 4, WINDING);

						/*		CRgn* rgn2_all = &rgn;
								rgn2_all[i].CombineRgn(&rgn, &rgn2, RGN_DIFF);
								pdc.SelectClipRgn(rgn2_all);*/

								rgn.CombineRgn(&rgn, &rgn2, RGN_DIFF);
								pdc.SelectClipRgn(&rgn);
								
							}
							
						}
			
				
					//}
						
					
					//if ((m_vWindowHandle[i].z < m_vWindowHandle2[i].z && m_vWindowHandle[i + 1].z > m_vWindowHandle2[i].z) ||
					//	(m_vWindowHandle[i].z > m_vWindowHandle2[i].z && m_vWindowHandle[i + 1].z < m_vWindowHandle2[i].z))
					//{
					//	int arr2[2] = { m_vWindowHandle[i].z, m_vWindowHandle[i + 1].z };
					//	int i, min, minlocation;

					//	min = *arr2; 
					//	minlocation = 0;
					//	//for (i = 0; i < 2; i++)
					//		//arr2[i] = m_vWindowHandle[i].z;
					//	for (j = 0; j < 2; j++) {
					//		arr2[j] = m_vWindowHandle[j].z;
					//		if (min > * (arr2 + j)) {
					//			min = *(arr2 + j);
					//			minlocation = j;
					//		}
					//	}
					//	/*	str.Format(_T("노트(1) %d   노트(2) %d"), arr2[0], arr2[1]);
					//		OutputDebugString(str);
					//		str.Format(_T("상위 메모장 %d 번째"), minlocation);
					//		OutputDebugString(str);*/


					//	CRect rt3(m_vWindowHandle[minlocation].rect.left, m_vWindowHandle[minlocation].rect.top,
					//		m_vWindowHandle[minlocation].rect.right, m_vWindowHandle[minlocation].rect.bottom);



					//	CPoint points2[4];
					//	points2[0] = CPoint(rt3.left, rt3.top);
					//	points2[1] = CPoint(rt3.right, rt3.top);
					//	points2[2] = CPoint(rt3.right, rt3.bottom);
					//	points2[3] = CPoint(rt3.left, rt3.bottom);

					//	rgn2.CreatePolygonRgn(points2, 4, WINDING);

					//	/*	str.Format(_T("%d"), points2[0].x);
					//		OutputDebugString(str);
					//		str.Format(_T("rt3[%d]의 left 값 : %d\n\n"), minlocation, rt3.left);
					//		OutputDebugString(str);*/

					//	CPoint points3[6];
					//	Rgnn.CreatePolygonRgn(points3, 6, WINDING);

					//	Rgnn.CombineRgn(&rgn, &rgn2, RGN_DIFF);

					//	pdc.SelectClipRgn(&Rgnn);
					//	
					//}

				}

			}
		
			if (m_MainData.ListType == 2)
			{
				/*if (m_vWindowHandle2[i].z > m_vWindowHandle[j].z)
				{*/
					/*CRgn rgn;
					CPoint points[4];
					points[0] = CPoint(rt.left, rt.top);
					points[1] = CPoint(rt.right, rt.top);
					points[2] = CPoint(rt.right, rt.bottom);
					points[3] = CPoint(rt.left, rt.bottom);

					rgn.CreatePolygonRgn(points, 4, WINDING);

			
					CRgn RGN, RGN2;
					CPoint points2[8];
					RGN.CreatePolygonRgn(points2, 8, WINDING);
					RGN.CombineRgn(&rgn_window, &rgn, RGN_DIFF);

					for (int i = 0; i < size3; i++) 
					{
						CRgn *rgn_rt2 = &rgn_window;
						CPoint points3[4];
						points3[0] = CPoint(rt2.left, rt2.top);
						points3[1] = CPoint(rt2.right, rt2.top);
						points3[2] = CPoint(rt2.right, rt2.bottom);
						points3[3] = CPoint(rt2.left, rt2.bottom);

						rgn_rt2[i].CreatePolygonRgn(points3, 4, WINDING);
						rgn_rt2[i].CombineRgn(&rgn_rt2[i], &rgn_rt2[i + 1], RGN_OR);

						
						CPoint points4[20];
						RGN2.CreatePolygonRgn(points4, 20, WINDING);
						RGN2.CombineRgn(&RGN, rgn_rt2, RGN_OR);
			
					}
					pdc.SelectClipRgn(&RGN2);*/
				//}

				/*else
				{*/
					CRgn rgn;
					CPoint points[4];
					points[0] = CPoint(rt.left, rt.top);
					points[1] = CPoint(rt.right, rt.top);
					points[2] = CPoint(rt.right, rt.bottom);
					points[3] = CPoint(rt.left, rt.bottom);

					rgn.CreatePolygonRgn(points, 4, WINDING);

					CRgn RGN;
					CPoint points2[8];
					RGN.CreatePolygonRgn(points2, 8, WINDING);
					RGN.CombineRgn(&rgn_window, &rgn, RGN_DIFF);

					pdc.SelectClipRgn(&RGN);

					vector<WindowInfo>::iterator iWindow2;
					for (iWindow2 = m_vWindowHandle.begin(); iWindow2 != m_vWindowHandle.end(); iWindow2++)
					{

						CRect iWindowRect((*iWindow2).rect.left, (*iWindow2).rect.top,
							(*iWindow2).rect.right, (*iWindow2).rect.bottom);

						if ((*iWindow2).z < m_vWindowHandle2[i].z)
						{
							if (IntersectRect(&overlapRect, rt, iWindowRect))
							{
								CPoint point[4];
								point[0] = CPoint(overlapRect.left, overlapRect.top);
								point[1] = CPoint(overlapRect.right, overlapRect.top);
								point[2] = CPoint(overlapRect.right, overlapRect.bottom);
								point[3] = CPoint(overlapRect.left, overlapRect.bottom);

								rgn3.CreatePolygonRgn(point, 4, WINDING);

								str.Format(_T("overlapRect.left  %d"), overlapRect.left);
								OutputDebugString(str);

								CRgn* rgn3_all = &RGN;
								rgn3_all[i].CombineRgn(&RGN, &rgn3, RGN_OR);
								pdc.SelectClipRgn(rgn3_all);
								/*RGN.CombineRgn(&RGN, &rgn2, RGN_OR);
								pdc.SelectClipRgn(&RGN);*/

							}
						}

					}
				//}
			}
		//}
	}

			//안녕하세요 저는 신선영입니다 만나서 반갑습니다 다음에 또 만나요
			//こんにちは、私はシンソンヨウンですはじめまして次にまた会いましょう
			//Helló, én Shin Sun-fiatal vagyok. Örülök, hogy találkozunk. Legközelebb találkozunk.
			//您好，我是申善英，很高兴认识您。
		//}


	



















//	vector<WindowInfo>::iterator iWindow1;
//	for (iWindow1 = m_vWindowHandle.begin(); iWindow1 != m_vWindowHandle.end(); iWindow1++)
//	{
//		//pdc.Rectangle((*iWindow).left, (*iWindow).top,  (*iWindow).right, (*iWindow).bottom);
//
//
//		/*CRect rt((*iWindow1).rect.left, (*iWindow1).rect.top,
//			(*iWindow1).rect.right, (*iWindow1).rect.bottom);*/
//
//		CRect rt(iWindow1[0].rect.left, iWindow1[0].rect.top,
//			iWindow1[0].rect.right, iWindow1[0].rect.bottom);
//
//
//		z1 = iWindow1[0].z;
//		z2 = iWindow1[1].z;
//
//		CRect overlapRect(0, 0, 0, 0);
//
//		CRect rt2(iWindow1[1].rect.left, iWindow1[1].rect.top,
//			iWindow1[1].rect.right, iWindow1[1].rect.bottom);
//
//		//z가 전보다 크면 빼주고 z가 전보다 작으면 더한다
//
//		if (z1 != z2 && IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
//			rt.top < rt2.top && rt.right <= rt2.right && rt.bottom <= rt2.bottom)
//		{
//			if (z1 > z2)
//			{
//				CPoint points[6];
//				points[0] = CPoint(rt.left, rt.top);
//				points[1] = CPoint(rt.left, rt.bottom);
//				points[2] = CPoint(overlapRect.left, overlapRect.bottom);
//				points[3] = CPoint(overlapRect.left, overlapRect.top);
//				points[4] = CPoint(overlapRect.right, overlapRect.top);
//				points[5] = CPoint(rt.right, rt.top);
//				CRgn Rgn;
//
//				Rgn.CreatePolygonRgn(points, 6, WINDING);
//
//				pdc.SelectClipRgn(&Rgn, RGN_DIFF);
//			}
//
//			else
//			{
//
//				CPoint points[6];
//				points[0] = CPoint(overlapRect.right, overlapRect.top);
//				points[1] = CPoint(overlapRect.right, overlapRect.bottom);
//				points[2] = CPoint(overlapRect.left, overlapRect.bottom);
//				points[3] = CPoint(rt2.left, rt2.bottom);
//				points[4] = CPoint(rt2.right, rt2.bottom);
//				points[5] = CPoint(rt2.right, rt2.top);
//
//				CRgn Rgn;
//
//				Rgn.CreatePolygonRgn(points, 6, WINDING);
//
//				pdc.SelectClipRgn(&Rgn, RGN_DIFF);
//			}
//		}
	

	/*	if (iWindow1 != m_vWindowHandle.end())
		{
			iWindow1++;
		}*/

		/*CRect rt((*iWindow1).rect.left, (*iWindow1).rect.top,
			(*iWindow1).rect.right, (*iWindow1).rect.bottom);*/

		//CRect rt2((*iWindow1).rect.left, (*iWindow1).rect.top,
		//	(*iWindow1).rect.right, (*iWindow1).rect.bottom);

		///*CRect rt2((*iWindow1).rect.left, (*iWindow1).rect.top,
		//	(*iWindow1).rect.right, (*iWindow1).rect.bottom);*/

	
		//CRect overlapRect(0, 0, 0, 0);

		////rt 오른쪽 하단 위에 rt2
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
		//	rt.top < rt2.top && rt.right <= rt2.right && rt.bottom <= rt2.bottom)
		//{
		//

		//	CPoint points[6];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.left, rt.bottom);
		//	points[2] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[3] = CPoint(overlapRect.left, overlapRect.top);
		//	points[4] = CPoint(overlapRect.right, overlapRect.top);
		//	points[5] = CPoint(rt.right, rt.top);
		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 6, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt 왼쪽 하단 위에 rt2
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left >= rt2.left &&
		//	rt.top < rt2.top && rt.right > rt2.right && rt.bottom <= rt2.bottom)
		//{
		//	CPoint points[6];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.right, rt.top);
		//	points[2] = CPoint(rt.right, rt.bottom);
		//	points[3] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points[4] = CPoint(overlapRect.right, overlapRect.top);
		//	points[5] = CPoint(overlapRect.left, overlapRect.top);

		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 6, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt 왼쪽 상단 위에 rt2
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left >= rt2.left &&
		//	rt.top >= rt2.top && rt.right > rt2.right && rt.bottom > rt2.bottom)
		//{
		//	CPoint points[6];
		//	points[0] = CPoint(overlapRect.right, overlapRect.top);
		//	points[1] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points[2] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[3] = CPoint(rt.left, rt.bottom);
		//	points[4] = CPoint(rt.right, rt.bottom);
		//	points[5] = CPoint(rt.right, rt.top);

		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 6, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt 오른쪽 상단 위에 rt2
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
		//	rt.top >= rt2.top && rt.right <= rt2.right && rt.bottom > rt2.bottom)
		//{
		//	CPoint points[6];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.left, rt.bottom);
		//	points[2] = CPoint(rt.right, rt.bottom);
		//	points[3] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points[4] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[5] = CPoint(overlapRect.left, overlapRect.top);

		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 6, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt 아래 위에 rt2 (rt > rt2)
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
		//	rt.top < rt2.top && rt.right > rt2.right && rt.bottom <= rt2.bottom)
		//{
		//	CPoint points[8];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.left, rt.bottom);
		//	points[2] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[3] = CPoint(overlapRect.left, overlapRect.top);
		//	points[4] = CPoint(overlapRect.right, overlapRect.top);
		//	points[5] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points[6] = CPoint(rt.right, rt.bottom);
		//	points[7] = CPoint(rt.right, rt.top);

		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 8, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt 왼쪽 위에 rt2 (rt > rt2)
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left >= rt2.left &&
		//	rt.top < rt2.top && rt.right > rt2.right && rt.bottom > rt2.bottom)
		//{
		//	CPoint points[8];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.right, rt.top);
		//	points[2] = CPoint(rt.right, rt.bottom);
		//	points[3] = CPoint(rt.left, rt.bottom);
		//	points[4] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[5] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points[6] = CPoint(overlapRect.right, overlapRect.top);
		//	points[7] = CPoint(overlapRect.left, overlapRect.top);

		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 8, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt 상단 위에 rt2 (rt > rt2)
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
		//	rt.top >= rt2.top && rt.right > rt2.right && rt.bottom > rt2.bottom)
		//{
		//	CPoint points[8];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.left, rt.bottom);
		//	points[2] = CPoint(rt.right, rt.bottom);
		//	points[3] = CPoint(rt.right, rt.top);
		//	points[4] = CPoint(overlapRect.right, overlapRect.top);
		//	points[5] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points[6] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[7] = CPoint(overlapRect.left, overlapRect.top);


		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 8, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt 오른쪽 위에 rt2 (rt > rt2)
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
		//	rt.top < rt2.top && rt.right <= rt2.right && rt.bottom > rt2.bottom)
		//{
		//	CPoint points[8];
		//	points[0] = CPoint(rt.right, rt.top);
		//	points[1] = CPoint(rt.left, rt.top);
		//	points[2] = CPoint(rt.left, rt.bottom);
		//	points[3] = CPoint(rt.right, rt.bottom);
		//	points[4] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points[5] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[6] = CPoint(overlapRect.left, overlapRect.top);
		//	points[7] = CPoint(overlapRect.right, overlapRect.top);


		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 8, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt2가 rt 내부에 있을 경우
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
		//	rt.top < rt2.top && rt.right > rt2.right && rt.bottom > rt2.bottom)
		//{
		//	CPoint points[4];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.right, rt.top);
		//	points[2] = CPoint(overlapRect.right, overlapRect.top);
		//	points[3] = CPoint(overlapRect.left, overlapRect.top);

		//	CPoint points2[4];
		//	points2[0] = CPoint(rt.right, rt.top);
		//	points2[1] = CPoint(rt.right, rt.bottom);
		//	points2[2] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points2[3] = CPoint(overlapRect.right, overlapRect.top);

		//	CPoint points3[4];
		//	points3[0] = CPoint(rt.right, rt.bottom);
		//	points3[1] = CPoint(rt.left, rt.bottom);
		//	points3[2] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points3[3] = CPoint(overlapRect.right, overlapRect.bottom);

		//	CPoint points4[4];
		//	points4[0] = CPoint(rt.left, rt.top);
		//	points4[1] = CPoint(rt.left, rt.bottom);
		//	points4[2] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points4[3] = CPoint(overlapRect.left, overlapRect.top);

		//	CRgn Rgn, Rgn2, Rgn3, Rgn4;

		//	Rgn.CreatePolygonRgn(points, 4, WINDING);
		//	Rgn2.CreatePolygonRgn(points2, 4, WINDING);
		//	Rgn3.CreatePolygonRgn(points3, 4, WINDING);
		//	Rgn4.CreatePolygonRgn(points4, 4, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//	pdc.SelectClipRgn(&Rgn2, RGN_DIFF);
		//	pdc.SelectClipRgn(&Rgn3, RGN_DIFF);
		//	pdc.SelectClipRgn(&Rgn4, RGN_DIFF);
		//}

		////rt 오른쪽 위에 rt2 (rt < rt2)
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
		//	rt.top >= rt2.top && rt.right <= rt2.right && rt.bottom <= rt2.bottom)
		//{
		//	CPoint points[4];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.left, rt.bottom);
		//	points[2] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[3] = CPoint(overlapRect.left, overlapRect.top);

		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 4, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt 아래 위에 rt2(rt < rt2)
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left >= rt2.left &&
		//	rt.top < rt2.top && rt.right <= rt2.right && rt.bottom <= rt2.bottom)
		//{
		//	CPoint points[4];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.right, rt.top);
		//	points[2] = CPoint(overlapRect.right, overlapRect.top);
		//	points[3] = CPoint(overlapRect.left, overlapRect.top);

		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 4, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}

		////rt 왼쪽 위에 rt2(rt < rt2)
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left >= rt2.left &&
		//	rt.top >= rt2.top && rt.right > rt2.right && rt.bottom <= rt2.bottom)
		//{
		//	CPoint points[4];
		//	points[0] = CPoint(rt.right, rt.top);
		//	points[1] = CPoint(rt.right, rt.bottom);
		//	points[2] = CPoint(overlapRect.right, rt.bottom);
		//	points[3] = CPoint(overlapRect.right, rt.top);

		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 4, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}
		////rt 위쪽 위에 rt2(rt < rt2)
		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left >= rt2.left &&
		//	rt.top >= rt2.top && rt.right <= rt2.right && rt.bottom > rt2.bottom)
		//{
		//	CPoint points[4];
		//	points[0] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[1] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points[2] = CPoint(rt.right, rt.bottom);
		//	points[3] = CPoint(rt.left, rt.bottom);

		//	CRgn Rgn;

		//	Rgn.CreatePolygonRgn(points, 4, WINDING);

		//	pdc.SelectClipRgn(&Rgn, RGN_DIFF);
		//}


		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left >= rt2.left &&
		//	rt.top < rt2.top && rt.right <= rt2.right && rt.bottom > rt2.bottom)
		//{
		//	CPoint points[4];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.right, rt.top);
		//	points[2] = CPoint(overlapRect.right, overlapRect.top);
		//	points[3] = CPoint(overlapRect.left, overlapRect.top);

		//	CPoint points2[4];
		//	points2[0] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points2[1] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points2[2] = CPoint(rt.right, rt.bottom);
		//	points2[3] = CPoint(rt.left, rt.bottom);

		//	CRgn rgn, rgn2;

		//	rgn.CreatePolygonRgn(points, 4, WINDING);
		//	rgn2.CreatePolygonRgn(points2, 4, WINDING);

		//	pdc.SelectClipRgn(&rgn, RGN_DIFF);
		//	pdc.SelectClipRgn(&rgn2, RGN_DIFF);
		//}

		//if (IntersectRect(&overlapRect, &rt, &rt2) && rt.left < rt2.left &&
		//	rt.top >= rt2.top && rt.right > rt2.right && rt.bottom <= rt2.bottom)
		//{
		//	CPoint points[4];
		//	points[0] = CPoint(rt.left, rt.top);
		//	points[1] = CPoint(rt.left, rt.bottom);
		//	points[2] = CPoint(overlapRect.left, overlapRect.bottom);
		//	points[3] = CPoint(overlapRect.left, overlapRect.top);

		//	CPoint points2[4];
		//	points2[0] = CPoint(overlapRect.right, overlapRect.top);
		//	points2[1] = CPoint(overlapRect.right, overlapRect.bottom);
		//	points2[2] = CPoint(rt.right, rt.bottom);
		//	points2[3] = CPoint(rt.right, rt.top);

		//	CRgn rgn, rgn2;

		//	rgn.CreatePolygonRgn(points, 4, WINDING);
		//	rgn2.CreatePolygonRgn(points2, 4, WINDING);

		//	pdc.SelectClipRgn(&rgn, RGN_DIFF);
		//	pdc.SelectClipRgn(&rgn2, RGN_DIFF);
		//}


	//}




	//int nType = 1;
	//if (nType == 1)
	//{
	//	//화이트 리스트
	//	vector<WindowInfo>::iterator iWindow;
	//	for (iWindow = m_vWindowHandle.begin(); iWindow != m_vWindowHandle.end(); iWindow++)
	//	{
	//		//pdc.Rectangle((*iWindow).left, (*iWindow).top,  (*iWindow).right, (*iWindow).bottom);
	//	
	//		CRect rt((*iWindow).rect.left, (*iWindow).rect.top,
	//			(*iWindow).rect.right, (*iWindow).rect.bottom);
	//		CRgn rgn;
	//		rgn.CreateRectRgnIndirect(&rt);

	//		
	//		
	//		//pdc.FillRgn(&rgn, &CBrush(RGB(240,240,240)));
	//		pdc.SelectClipRgn(&rgn, RGN_DIFF);
	//		//pdc.InvertRgn(&rgn1);
	//		//pdc.FrameRgn(&rgn, &CBrush(RGB(0,0,0)),5,5);

	//		rgn.DeleteObject();

	//		HWND Top = ::GetForegroundWindow();

	//		RECT rtTop;
	//		::GetWindowRect(Top, &rtTop);

	//		CRgn rgnTop;
	//		rgnTop.CreateRectRgnIndirect(&rtTop);


	//		CRect rt2(rtTop.left, rtTop.top, rtTop.right, rtTop.bottom);

	//		pdc.FrameRgn(&rgnTop, &CBrush(RGB(0, 0, 0)), 5, 5);

	//	}
	//}
	//else if (nType == 2)
	//{
	//	//블랙 리스트	  

	//	CRgn rgn1;
	//	RECT rt;
	//	rt.left = m_nX;
	//	rt.top = m_nY;
	//	rt.right = m_nWidth;
	//	rt.bottom = (-m_nY) + m_nHeight;
	//	rgn1.CreateRectRgnIndirect(&rt);
	//	vector<WindowInfo>::iterator iWindow;
	//	for (iWindow = m_vWindowHandle.begin(); iWindow != m_vWindowHandle.end(); iWindow++)
	//	{
	//		//pdc.Rectangle((*iWindow).left, (*iWindow).top,  (*iWindow).right, (*iWindow).bottom);

	//		CRect rt((*iWindow).rect.left, (*iWindow).rect.top,
	//			(*iWindow).rect.right, (*iWindow).rect.bottom);
	//		CRgn rgn;
	//		rgn.CreateRectRgnIndirect(&rt);
	//		rgn1.CombineRgn(&rgn1, &rgn, RGN_DIFF);
	//		// pdc.FrameRgn(&rgn1, &CBrush(RGB(0,0,0)),5,5);
	//		rgn.DeleteObject();

	//	}

	//	//pdc.FrameRgn(&rgn1, &CBrush(RGB(250,0,0)),5,5);
	//	pdc.SelectClipRgn(&rgn1, RGN_DIFF);

	//	rgn1.DeleteObject();

	



	vector<CPoint>::iterator itor;
	for (itor = m_vStringPos.begin(); itor != m_vStringPos.end(); itor++)
	{
		pdc.TextOut((*itor).x, (*itor).y, m_sMessage);
	}


	/*pdc.SelectObject(pOldFont);

	font.DeleteObject();

	pdc.SelectObject(pOldPen);*/
	//CPaintDC pdc(this);

	if (m_MainData.typeChoice == 1)
	{
		for (int i = 0; i < m_MainData.RectCount; i++)
		{
			left_x = ((i + m_MainData.LineCount) % m_MainData.LineCount) * (3 * x) + x;
			left_y = ((i + m_MainData.LineCount) / m_MainData.LineCount) * (3 * y) - (2 * y);

			right_x = ((i + m_MainData.LineCount) % m_MainData.LineCount) * (3 * x) + (2 * x);
			right_y = ((i + m_MainData.LineCount) / m_MainData.LineCount) * (3 * y) - y;

			size = pdc.GetTextExtent(str + this->m_sMessage, m_MainData.FontSize);

			center_x = left_x + x / 2;  //사각형 중심
			center_y = left_y + y / 2;

			half_t_x = size.cx / 2;  //텍스트 사이즈 반
			half_t_y = size.cy / 2;

			SolidBrush sbrush(Color(m_MainData.color_r, m_MainData.color_g, m_MainData.color_b));

			Pen pen(Color(m_MainData.Outline_r, m_MainData.Outline_g, m_MainData.Outline_b), m_MainData.OutlineSize);

			Graphics graphics(pdc);
			graphics.SetSmoothingMode(SmoothingModeHighQuality);

			PointF ptText(center_x, center_y);

			StringFormat format;
			format.SetAlignment(StringAlignmentCenter);
			format.SetLineAlignment(StringAlignmentCenter);

			GraphicsPath path, path2;
			FontFamily fontFamily(m_MainData.fontStyle);

			path.AddString(m_MainData.str + this->m_sMessage, -1, &fontFamily, FontStyleBold, m_MainData.FontSize,
				RectF(left_x, left_y , right_x - left_x, right_y - left_y), &format);
			path2.AddString(m_MainData.str + this->m_sMessage, -1, &fontFamily, FontStyleBold, m_MainData.FontSize, ptText, &format);

			path.SetFillMode(FillModeWinding);
			path2.SetFillMode(FillModeWinding);

			Matrix transformMatrix;

			transformMatrix.Translate(center_x, center_y);        //기준점을 사각형 중심으로 바꿈
			transformMatrix.Rotate(m_MainData.degree);                       //입력 받은 각도 만큼 회전
			transformMatrix.Translate(-(center_x), -(center_y));
			graphics.SetTransform(&transformMatrix);

			//Image image(L"res\\Thor.jpg");
			//graphics.DrawImage(&image, int(left_x), int(left_y), x, y);

			if (size.cx > right_x - left_x || m_MainData.LineCount >= 1)
			{
				graphics.FillPath(&sbrush, &path);
				graphics.DrawPath(&pen, &path);
			}
			else
			{
				graphics.FillPath(&sbrush, &path2);
				graphics.DrawPath(&pen, &path2);
			}

		}
	}
	if(m_MainData.typeChoice == 2)
	{ 
		for (int i = -1; i < y2+1; i++)
		{
			for (int j = -1; j < x2+1; j++)
			{
				/*left_x = ((i + m_MainData.LineCount) % m_MainData.LineCount) * x2;
				left_y = ((i + m_MainData.LineCount) / m_MainData.LineCount) * y2 - y2;

				right_x = ((i + m_MainData.LineCount) % m_MainData.LineCount) * x2 + x2 ;
				right_y = ((i + m_MainData.LineCount) / m_MainData.LineCount) * y2;*/

				left_x = ((j + m_MainData.Width) % m_MainData.Width) * m_MainData.Width;
				left_y = ((j + m_MainData.Width) / m_MainData.Width) * m_MainData.Height - m_MainData.Height +(i*m_MainData.Height);

				right_x = ((j + m_MainData.Width) % m_MainData.Width) * m_MainData.Width + m_MainData.Width;
				right_y = ((j + m_MainData.Width) / m_MainData.Width) * m_MainData.Height + (i * m_MainData.Height);

				
				size = pdc.GetTextExtent(str + this->m_sMessage, m_MainData.FontSize);

				center_x = left_x + x2 / 2;  //사각형 중심
				center_y = left_y + y2 / 2;

				half_t_x = size.cx / 2;  //텍스트 사이즈 반
				half_t_y = size.cy / 2;

				SolidBrush sbrush(Color(m_MainData.color_r, m_MainData.color_g, m_MainData.color_b));

				Pen pen(Color(m_MainData.Outline_r, m_MainData.Outline_g, m_MainData.Outline_b), m_MainData.OutlineSize);

				Graphics graphics(pdc);
				graphics.SetSmoothingMode(SmoothingModeHighQuality);

				PointF ptText(center_x, center_y);

				StringFormat format;
				format.SetAlignment(StringAlignmentCenter);
				format.SetLineAlignment(StringAlignmentCenter);

				GraphicsPath path, path2;
				FontFamily fontFamily(m_MainData.fontStyle);

				path.AddString(m_MainData.str + this->m_sMessage, -1, &fontFamily, FontStyleBold, m_MainData.FontSize,
					RectF(left_x, left_y, right_x - left_x, right_y - left_y), &format);
				path2.AddString(m_MainData.str + this->m_sMessage, -1, &fontFamily, FontStyleBold, m_MainData.FontSize, ptText, &format);

				path.SetFillMode(FillModeWinding);
				path2.SetFillMode(FillModeWinding);

				Matrix transformMatrix;

				transformMatrix.Translate(center_x, center_y);        //기준점을 사각형 중심으로 바꿈
				transformMatrix.Rotate(m_MainData.degree);                       //입력 받은 각도 만큼 회전
				transformMatrix.Translate(-(center_x), -(center_y));
				graphics.SetTransform(&transformMatrix);

				//Image image(L"res\\Thor.jpg");
				//graphics.DrawImage(&image, int(left_x), int(left_y), m_MainData.Width, m_MainData.Height);

				if (size.cx > right_x - left_x || m_MainData.LineCount >= 1 || m_MainData.LineCount <= 8)
				{
					graphics.FillPath(&sbrush, &path);
					graphics.DrawPath(&pen, &path);
				}
				else
				{
					graphics.FillPath(&sbrush, &path2);
					graphics.DrawPath(&pen, &path2);
				}

			}

		}
	}
}


BOOL CScreen::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return TRUE;
	return CDialog::OnEraseBkgnd(pDC);
}



