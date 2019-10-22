#pragma once
#include <vector>
#include "ComeOnData.h"
#include "ComeOnWindow.h"
using namespace std;

// CScreen 대화 상자


class CSettingDlg;
class CScreen : public CDialog
{
	DECLARE_DYNAMIC(CScreen)

public:
	//CScreen(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CScreen(CSettingDlg* pParent = NULL);
	virtual ~CScreen();
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SCREEN };
#endif

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private:
	int m_nX;
	int m_nY;
	int m_nWidth;
	int m_nHeight;

public:

	double back, round; // round - degree = back  degree만큼 회전, 출력,back만큼 회전하여 
						// 총 360도 회전하여 원위치 
	double degree;

	double center_x, center_y; //사각형 중심
	double half_t_x, half_t_y; //텍스트 사이즈 반

	CString fontStyle;
	CString str;
	CSize size;

	int rm;
	double x, y, x2, y2;
	double left_x, left_y, right_x, right_y;
	RegInfo m_MainData;
	WindowInfo m_WindowInfo;

public:
	vector<WindowInfo> m_vWindowHandle;
	vector<WindowInfo2> m_vWindowHandle2;

	void SetPosition(int width, int height, int LineCount, int RectCount, int Width, int Height, CString str, CString fontStyle,
		int FontSize, int degree);
	void SetData(RegInfo data);

	CString m_sMessage;
	CString m_sm;
	vector<CPoint> m_vStringPos;
	vector<CRect> rect;

	int GetZOrder(HWND hTarget);
	void swap(int *a, int *b);
	void permutation(int n, int r, int depth, CRgn *rgn[]);


	//CRect rt;
	
	
};
