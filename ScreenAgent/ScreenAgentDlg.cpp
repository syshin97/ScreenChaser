
// ScreenAgentDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "ScreenAgent.h"
#include "ScreenAgentDlg.h"
#include "afxdialogex.h"

#include "shlwapi.h"
#include "psapi.h"
#include <TlHelp32.h>

#include "CSettingDlg.h"
#include "ComeOnWindow.h"

#pragma comment(lib, "Oleacc.lib")
#pragma comment(lib, "Shlwapi.lib")
#pragma comment(lib, "psapi.lib")



#ifdef _DEBUG
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#define new DEBUG_NEW
#endif

#define WM_MAIN_TRAY    (WM_USER + 1)




#ifdef _DEBUG
#define new DEBUG_NEW
#endif

map<HWND, WindowInfo> m_mWindowList;
map<HWND, WindowInfo2> m_mWindowList2;

queue<WindowInfo> m_qInsertWindowInfo;
queue<WindowInfo2> m_qInsertWindowInfo2;
HANDLE g_hMainThreadEndEvent = NULL;
bool g_bMainStopEvent = false;

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.
//CScreenAgentDlg::CScreenAgentDlg(CSetting* pParent /*=NULL*/)
//	: CDialogEx(IDD_DIALOG_SCREEN, pParent)
//{
//	m_pParentWnd = pParent;
//}
//CScreenAgentDlg::~CScreenAgentDlg()
//{
//
//}
class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()



// CScreenAgentDlg 대화 상자

ULONG ProcIDFromWnd(HWND hwnd) // 윈도우 핸들로 프로세스 아이디 얻기   
{
	ULONG idProc;
	GetWindowThreadProcessId(hwnd, &idProc);
	return idProc;
}

HWND GetWinHandle(ULONG pid) // 프로세스 아이디로 윈도우 핸들 얻기   
{
	HWND tempHwnd = FindWindow(NULL, NULL); // 최상위 윈도우 핸들 찾기   

	while (tempHwnd != NULL)
	{
		// 최상위 핸들인지 체크, 버튼 등도 핸들을 가질 수 있으므로 무시하기 위해
		if (GetParent(tempHwnd) == NULL)
			if (pid == ProcIDFromWnd(tempHwnd))
				return tempHwnd;
		tempHwnd = GetWindow(tempHwnd, GW_HWNDNEXT); // 다음 윈도우 핸들 찾기   
	}
	return NULL;
}

typedef struct _ProcessCallbackData
{
	DWORD m_pid;
	HWND  m_hwnd;
} ProcessCallbackData;


BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lParam)
{
	DWORD pid = 0;
	ProcessCallbackData* pcbd = (ProcessCallbackData*)lParam;

	if (IsWindowVisible(hwnd) == FALSE)
		return TRUE;

	GetWindowThreadProcessId(hwnd, &pid);

	if (pcbd->m_pid != pid)
		return TRUE;

	pcbd->m_hwnd = hwnd;

	return FALSE;
}

HWND GetHandleFromPID(DWORD pid)
{
	ProcessCallbackData cbd = { 0 };

	if (pid == 0)
		return NULL;

	cbd.m_pid = pid;
	cbd.m_hwnd = NULL;

	EnumWindows(EnumProc, (LPARAM)& cbd);

	return cbd.m_hwnd;
}

unsigned int __stdcall MainThreadProc(LPVOID pParam)
{
	CScreenAgentDlg* pMain = (CScreenAgentDlg*)pParam;
	try
	{
		while (TRUE)
		{
			//OutputDebugString(_T("Screener TEST"));
			
			if (true == g_bMainStopEvent)
			{
				SetEvent(g_hMainThreadEndEvent);
				break;
			}


			HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

			if (hProcessSnap == INVALID_HANDLE_VALUE) {
				_tprintf(_T("CreateToolhelp32Snapshot error \n"));
				continue;
			}
			
			CString strName = _T("notepad.exe");
			CString strName2 = _T("excel.exe");
			PROCESSENTRY32 pe32;
			pe32.dwSize = sizeof(PROCESSENTRY32);

			if (!Process32First(hProcessSnap, &pe32)) {
				_tprintf(_T("Process32First error ! \n"));
				CloseHandle(hProcessSnap);
				continue;
			}
	
	
			do {
				//_tprintf(_T("%25s %8d %8d %8d \n"), pe32.szExeFile, pe32.th32ProcessID, pe32.cntThreads, pe32.th32ParentProcessID);

				if (!strName.Compare(pe32.szExeFile) //||
					/*!strName2.CompareNoCase(pe32.szExeFile)*/ ) {					

					// 원하는 이름의 프로세스일 때 처리
					//HWND hwnd = GetWinHandle(pe32.th32ProcessID);

					HWND hwnd = GetHandleFromPID(pe32.th32ProcessID);
					/*RECT rt;
					GetClientRect(hwnd2, &rt);*/
					
					//int z = pMain->m_ScreenerView->GetZOrder(hwnd);
		
					//m_qInsertWindowInfo.push(winfo);
					

				/*	CString str;
					str.Format(_T("%d"), winfo.z);
					OutputDebugString(str);*/

					map<HWND, WindowInfo>::iterator itor;
					
					itor = m_mWindowList.find(hwnd);
			
	
					if (itor == m_mWindowList.end())
					{
						WindowInfo winfo;
						winfo.hwnd = hwnd;

						//winfo.z = z;
					

						//m_mWindowList.insert(make_pair(hwnd,winfo));
						m_qInsertWindowInfo.push(winfo);
					/*	CString str;
						str.Format(_T("test  %d"), winfo.z);
						OutputDebugString(str);*/
						
						//printf(_T("event Focus [%d] Create 1 : %s ==> %s \n"), event, className, buffer);
					}

			
				}
				else
				{

					HWND hwnd = GetHandleFromPID(pe32.th32ProcessID);
					RECT rtTop;
					::GetWindowRect(hwnd, &rtTop);


					if (rtTop.left < rtTop.right)
					{
						int iii = 0;
					}					

				}
				///////////////////////////////////////////////////////////
				if (!strName2.CompareNoCase(pe32.szExeFile)) {

					HWND hwnd = GetHandleFromPID(pe32.th32ProcessID);

					map<HWND, WindowInfo2>::iterator itor2;

					itor2 = m_mWindowList2.find(hwnd);


					if (itor2 == m_mWindowList2.end())
					{
						WindowInfo2 winfo2;
						winfo2.hwnd = hwnd;
						m_qInsertWindowInfo2.push(winfo2);
					}
				}
				else
				{
					HWND hwnd = GetHandleFromPID(pe32.th32ProcessID);
					RECT rtTop;
					::GetWindowRect(hwnd, &rtTop);

					if (rtTop.left < rtTop.right)
					{
						int iii = 0;
					}

				}

			} while (Process32Next(hProcessSnap, &pe32));
			CloseHandle(hProcessSnap);

			CTime cTime = CTime::GetCurrentTime(); // 현재 시스템으로부터 날짜 및 시간을 얻어 온다.
			CString strDate, strTime; // 반환되는 날짜와 시간을 저장할 CString 변수 선언
			strDate.Format(_T("%04d년 %02d월 %02d일"), cTime.GetYear(), // 현재 년도 반환
				cTime.GetMonth(), // 현재 월 반환
				cTime.GetDay()); // 현재 일 반환
			strTime.Format(_T("%02d시 %02d분 %02d초"), cTime.GetHour(), // 현재 시간 반환
				cTime.GetMinute(), // 현재 분 반환
				cTime.GetSecond()); // 현재 초 반환
			
			pMain->m_sScreener = _T("워터마크 테스트입니다. 실행 확인 중입니다. :: ") + strDate + _T("::") + strTime;
		
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
	
			if (pMain->m_ScreenerView->m_MainData.LineCount != 0 && pMain->m_ScreenerView->m_MainData.RectCount != 0)
			{
				pMain->m_ScreenerView->SetPosition(nMainWidth, nMainHeight/MonitoCount,
					pMain->m_ScreenerView->m_MainData.LineCount,
					pMain->m_ScreenerView->m_MainData.RectCount,
					pMain->m_ScreenerView->m_MainData.Width,
					pMain->m_ScreenerView->m_MainData.Height,
					pMain->m_ScreenerView->m_MainData.str,
					pMain->m_ScreenerView->m_MainData.fontStyle,
					pMain->m_ScreenerView->m_MainData.FontSize,
					pMain->m_ScreenerView->m_MainData.degree);
			}
			
			//::SetWindowPos(m_hWnd, HWND_TOPMOST,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE|SWP_NOACTIVATE);
			pMain->InvalidateRect(FALSE);
			
			//pMain->m_str = pMain->m_ScreenerView->m_MainData.str;

			pMain->m_ScreenerView->m_sMessage = pMain->m_sScreener;
			//pMain->m_ScreenerView->m_sm= pMain->m_str ;

			for (int i = 0; i < 5; i++)
			{
				if (m_qInsertWindowInfo.empty())
					break;
				else
				{
					WindowInfo winfo = m_qInsertWindowInfo.front();
					m_qInsertWindowInfo.pop();

					m_mWindowList.insert(make_pair(winfo.hwnd, winfo));
				/*	CString str;
					str.Format(_T("%d"), winfo.z);
					OutputDebugString(str);*/
				}
			}
			for (int i = 0; i < 5; i++)
			{
				if (m_qInsertWindowInfo2.empty())
					break;
				else
				{
					windowInfo2 winfo2 = m_qInsertWindowInfo2.front();
					m_qInsertWindowInfo2.pop();
					m_mWindowList2.insert(make_pair(winfo2.hwnd, winfo2));
				}
			}


			vector<HWND> vDeleteList;
			
			map<HWND, WindowInfo>::iterator mitor;
			map<HWND, WindowInfo2>::iterator mitor2;
			//원도우 상태 확인
			for (mitor2 = m_mWindowList2.begin(); mitor2 != m_mWindowList2.end(); mitor2++)
			{
				DWORD dw = 0;
				if (SendMessageTimeout((*mitor2).second.hwnd, NULL, NULL, NULL, SMTO_NORMAL, 2000, &dw)) {
					// 정상적으로 응답함

				}
				else {

					DWORD error = GetLastError();
					if (error == ERROR_TIMEOUT ||
						error == ERROR_INVALID_WINDOW_HANDLE) {
						// 응답하지 않음
						vDeleteList.push_back((*mitor2).second.hwnd);
					}
				}
			}
			for (mitor = m_mWindowList.begin(); mitor != m_mWindowList.end(); mitor++)
			{
				DWORD dw = 0;
				if (SendMessageTimeout((*mitor).second.hwnd, NULL, NULL, NULL, SMTO_NORMAL, 2000, &dw)) {
					// 정상적으로 응답함

				}

				else {

					DWORD error = GetLastError();
					if (error == ERROR_TIMEOUT ||
						error == ERROR_INVALID_WINDOW_HANDLE) {
						// 응답하지 않음
						vDeleteList.push_back((*mitor).second.hwnd);

					}
				}
			}
			

			vector<HWND>::iterator idel;

			for (idel = vDeleteList.begin(); idel != vDeleteList.end(); idel++)
			{
				mitor = m_mWindowList.find((*idel));
				mitor2 = m_mWindowList2.find((*idel));
				m_mWindowList.erase((*mitor).second.hwnd);
				m_mWindowList2.erase((*mitor2).second.hwnd);
			}
		
			/*pMain->m_ScreenerView->m_vWindowHandle.clear();*/
			pMain->m_ScreenerView->m_vWindowHandle2.clear();

			for (mitor2 = m_mWindowList2.begin(); mitor2 != m_mWindowList2.end(); ++mitor2)
			{
				WindowInfo2 winfo2;

				::GetWindowRect((*mitor2).second.hwnd, &winfo2.rect);

				int z2 = pMain->m_ScreenerView->GetZOrder((*mitor2).second.hwnd);

				winfo2.rect.top += (-pMain->m_nMainScreenerTopPos);
				winfo2.rect.bottom += (-pMain->m_nMainScreenerTopPos);

				winfo2.z = z2;
				if (winfo2.rect.left != winfo2.rect.right)
				{
					pMain->m_ScreenerView->m_vWindowHandle2.push_back(winfo2);
				}
			}
			pMain->m_ScreenerView->m_vWindowHandle.clear();
			for (mitor = m_mWindowList.begin(); mitor != m_mWindowList.end(); ++mitor)
			{
				WindowInfo winfo;


				::GetWindowRect((*mitor).second.hwnd, &winfo.rect);


				//HWND hwnd = GetHandleFromPID(pe32.th32ProcessID);
				int z = pMain->m_ScreenerView->GetZOrder((*mitor).second.hwnd);


				winfo.rect.top += (-pMain->m_nMainScreenerTopPos);
				winfo.rect.bottom += (-pMain->m_nMainScreenerTopPos);

				winfo.z = z;

				//winfo.z = (*mitor).second.z;

			/*	CString str;
				str.Format(_T("=gogogogo %X  %d"), winfo.hwnd, winfo.z);
				OutputDebugString(str);*/

				if (winfo.rect.left != winfo.rect.right)
				{
					pMain->m_ScreenerView->m_vWindowHandle.push_back(winfo);
				}
			}
			
			pMain->m_ScreenerView->InvalidateRect(FALSE);
			

			Sleep(1000);
		}
	}
	catch (CException* e)
	{
		TCHAR csExeptCause[255];
		e->GetErrorMessage(csExeptCause, 255);
		//printf(_T("Main Thread Exception ==> %s \n"), csExeptCause);
	}
	return 0;
}




CScreenAgentDlg::CScreenAgentDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SCREENAGENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CScreenAgentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CScreenAgentDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_MAIN_TRAY, &CScreenAgentDlg::OnMainTray)
	ON_COMMAND(ID_OPTION_SETTING, &CScreenAgentDlg::OnSetting)
	ON_COMMAND(ID_OPTION_EXIT, &CScreenAgentDlg::OnExit)
END_MESSAGE_MAP()


// CScreenAgentDlg 메시지 처리기

BOOL CScreenAgentDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	//m_MainRegInfo = new RegInfo;
	//트레이 아이콘
	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd;
	nid.uID = IDR_MAINFRAME;
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
	nid.uCallbackMessage = WM_MAIN_TRAY;
	nid.hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	_tcscpy_s(nid.szTip, _T("Tray"));
	Shell_NotifyIcon(NIM_ADD, &nid);



	//화면 숨기기
	ShowWindow(SW_SHOWMINIMIZED);//! 최소화후 숨겨야 화면에 나타나지 않음
	PostMessage(WM_SHOWWINDOW, FALSE, SW_OTHERUNZOOM);


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

	int nTitle = 0;
	int screen_x, screen_y;
	//screen_x = (int)GetSystemMetrics(SM_CXSCREEN);
	//screen_y = (int)GetSystemMetrics(SM_CYSCREEN);
	int x = MonitorsRect[0].left;

	//MoveWindow( x , MonitorsRect[0].top -nTitle , MonitorsRect[0].right - MonitorsRect[0].left,
		//(-MonitorsRect[0].top) + MonitorsRect[0].bottom);

	//모니터 사이즈 구하기
	
	int nMainX = 0;
	int nMainY = 0;
	int nMainWidth = 0;
	int nMainHeight = 0;
	

	CPaintDC dc(this);
	
	for (int i = 0; i < MonitoCount; i++)
	{
		if( 0 > MonitorsRect[i].left)
		nMainX = MonitorsRect[i].left;
		nMainY += MonitorsRect[i].top;
		nMainWidth += MonitorsRect[i].right - MonitorsRect[i].left;
		nMainHeight += (-MonitorsRect[0].top) + MonitorsRect[0].bottom;
	}

	m_nMainScreenerTopPos = nMainY;
	m_ScreenerView = new CScreen;
	m_ScreenerView->Create(IDD_DIALOG_SCREEN, NULL);
	m_ScreenerView->MoveWindow(nMainX, nMainY, nMainWidth, nMainHeight);
	m_ScreenerView->ShowWindow(SW_SHOW);
	//m_ScreenerView->SetPosition(nMainWidth, nMainHeight/2, 5, 9, str,fontStyle, 10);


	if (NULL == g_hMainThreadEndEvent)
		g_hMainThreadEndEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	m_hMainThread = (HANDLE)_beginthreadex(NULL, 0, MainThreadProc, this, 0, &m_uMainThreadID);


	//m_pDlg = new CSetting;
	//m_pDlg->Create(IDD_DIALOG_SETTING);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CScreenAgentDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();

	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CScreenAgentDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.
	
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}

}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CScreenAgentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CScreenAgentDlg::OnClose()
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	/*if (m_hWindowEventHook)
	{
		UnhookWinEvent(m_hWindowEventHook);
		m_hWindowEventHook = NULL;
	}*/

	if (NULL != m_hMainThread)
	{
		g_bMainStopEvent = true;
		DWORD	dwRet = WaitForSingleObject(g_hMainThreadEndEvent, INFINITE);
		if (dwRet == WAIT_OBJECT_0)
		{

		}
		CloseHandle(m_hMainThread);
		CloseHandle(g_hMainThreadEndEvent);
		m_hMainThread = NULL;
		g_hMainThreadEndEvent = NULL;
		g_bMainStopEvent = false;
	}

	NOTIFYICONDATA nid;
	ZeroMemory(&nid, sizeof(nid));
	nid.cbSize = sizeof(nid);
	nid.hWnd = m_hWnd;
	nid.uID = IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_DELETE, &nid);


	CDialogEx::OnClose();
}

afx_msg LRESULT CScreenAgentDlg::OnMainTray(WPARAM wParam, LPARAM lParam)
{
	switch (lParam) {
	
	case WM_RBUTTONUP:
		CMenu menu, * pMenu = NULL;
		CPoint pt;
		menu.LoadMenu(IDR_MENU1);
		pMenu = menu.GetSubMenu(0);
		GetCursorPos(&pt);
		SetForegroundWindow();
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
		::PostMessage(m_hWnd, WM_NULL, NULL, NULL);
		ReleaseCapture();
		break;
	}
	return 0;
}

void CScreenAgentDlg::OnExit()
{
	OnClose();
	exit(0);
}

void CScreenAgentDlg::OnSetting()
{
	//m_pDlg->ShowWindow(SW_SHOW);
	CSettingDlg* dlg = new CSettingDlg;
	dlg->SetRegistry();
	dlg->WriteRegistry();
	if (IDOK == dlg->DoModal())
	{
		
		m_ScreenerView->SetData(dlg->m_MainInfo);
		AfxMessageBox(_T("Setting END"));

	}
	
	
}

