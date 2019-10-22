
// ScreenAgentDlg.h: 헤더 파일
//

#pragma once
#include "CScreen.h"

#include <vector>
#include <map>
#include <queue>
#include "ComeOnData.h"
//#include "ComeOnWindow.h"
using namespace std;

//typedef struct windowInfo
//{
//	RECT rect;
//	HWND hwnd;
//	int z;
//}WindowInfo, pWindowInfo;

// CScreenAgentDlg 대화 상자
class CSettingDlg;
class CScreenAgentDlg : public CDialogEx
{
// 생성입니다.
public:
	CScreenAgentDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	/*CScreenAgentDlg(CSetting* pParent = NULL);
	virtual ~CScreenAgentDlg();*/
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCREENAGENT_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LRESULT CScreenAgentDlg::OnMainTray(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()

public:
	RegInfo m_MainInfo;
	WindowInfo m_WindowInfo;
	
	int z1,z2;

public:
	HHOOK m_ThreadHook;
	long CALLBACK ProcessThreadMessage(int code, WPARAM removal, LPARAM message);
	void StayOnTop();
	CString m_sScreener;
	CString m_str;
	vector<CPoint> m_vStringPos;
	vector<CScreen*> m_vChilds;
	CRect m_MainRect;
	static BOOL GetProcessName(HANDLE hProcess, char* pPName);
	static void CALLBACK WindowEvent(HWINEVENTHOOK hook, DWORD event, HWND hwnd, LONG idObject,
		LONG idChild, DWORD dwEventThread, DWORD dwmsEventTime);
	HANDLE m_hMainThread;
	UINT m_uMainThreadID;

	void OnExit();
	void OnSetting();

	RegInfo m_MainRegInfo;

	CScreen* m_ScreenerView;
	int m_nMainScreenerTopPos;
	afx_msg void OnClose();



protected:
	//CSetting* m_pDlg;
};
