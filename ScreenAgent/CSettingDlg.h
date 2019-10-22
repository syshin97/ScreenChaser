#pragma once
#include "ComeOnData.h"

// CSetting 대화 상자

class CSettingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSettingDlg)

public:
	CSettingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSettingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SETTING };
#endif

private:
	CWnd* m_pParent;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	

	DECLARE_MESSAGE_MAP()
public:
	RegInfo m_MainInfo;
	void SetRegistry();
	void WriteRegistry();
public:
	int LineCount;
	int RectCount;
	CString str;
	CString fontStyle;
	int degree;
	int color_r;
	int color_g;
	int color_b;
	int Outline_r;
	int Outline_g;
	int Outline_b;
	int FontSize;
	afx_msg void OnBnClickedButton1();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	int typeChoice;
	int Width;   
	int Height;
	int OutlineSize;
	int blend;
	int ListType;
};
