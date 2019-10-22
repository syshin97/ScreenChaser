// CSetting.cpp: 구현 파일
//

#include "pch.h"
#include "ScreenAgent.h"
#include "CSettingDlg.h"
#include "afxdialogex.h"
#include "CScreen.h"
#include "ScreenAgentDlg.h"
#include "CRegistry.h"

// CSetting 대화 상자

IMPLEMENT_DYNAMIC(CSettingDlg, CDialogEx)

CSettingDlg::CSettingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SETTING, pParent)
	, LineCount(0)
	, RectCount(0)
	, str(_T(""))
	, fontStyle(_T(""))
	, FontSize(0)
	, degree(0)
	, color_r(0)
	, color_g(0)
	, color_b(0)
	, Outline_r(0)
	, Outline_g(0)
	, Outline_b(0)
	
	, typeChoice(0)
	, Width(0)
	, Height(0)
	, OutlineSize(0)
	, blend(0)
	, ListType(0)
{
	m_pParent = pParent;
	//m_MainInfo = new RegInfo;
}

CSettingDlg::~CSettingDlg()
{
}

void CSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT3, LineCount);
	DDV_MinMaxInt(pDX, LineCount, 1, 100);
	DDX_Text(pDX, IDC_EDIT4, RectCount);
	DDV_MinMaxInt(pDX, RectCount, 1, 100);
	DDX_Text(pDX, IDC_EDIT5, str);
	DDV_MaxChars(pDX, str, 500);
	DDX_Text(pDX, IDC_EDIT6, fontStyle);
	DDX_Text(pDX, IDC_EDIT7, FontSize);
	DDV_MinMaxInt(pDX, FontSize, 1, 100);
	DDX_Text(pDX, IDC_EDIT8, degree);
	DDV_MinMaxInt(pDX, degree, 0, INT_MAX);
	DDX_Text(pDX, IDC_EDIT9, color_r);
	DDV_MinMaxInt(pDX, color_r, 0, 255);
	DDX_Text(pDX, IDC_EDIT10, color_g);
	DDV_MinMaxInt(pDX, color_g, 0, 255);
	DDX_Text(pDX, IDC_EDIT11, color_b);
	DDV_MinMaxInt(pDX, color_b, 0, 255);
	DDX_Text(pDX, IDC_EDIT12, Outline_r);
	DDV_MinMaxInt(pDX, Outline_r, 0, 255);
	DDX_Text(pDX, IDC_EDIT13, Outline_g);
	DDV_MinMaxInt(pDX, Outline_g, 0, 255);
	DDX_Text(pDX, IDC_EDIT14, Outline_b);
	DDV_MinMaxInt(pDX, Outline_b, 0, 255);

	DDX_Text(pDX, IDC_EDIT1, typeChoice);
	DDV_MinMaxInt(pDX, typeChoice, 1, 2);
	DDX_Text(pDX, IDC_EDIT2, Width);
	DDX_Text(pDX, IDC_EDIT15, Height);
	DDX_Text(pDX, IDC_EDIT16, OutlineSize);
	DDX_Text(pDX, IDC_EDIT17, blend);
	DDX_Text(pDX, IDC_EDIT18, ListType);
}


BEGIN_MESSAGE_MAP(CSettingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CSettingDlg::OnBnClickedButton1)
	//ON_EN_CHANGE(IDC_EDIT1, &CSettingDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()



// CSetting 메시지 처리기

void CSettingDlg::SetRegistry()
{
	HKEY mainKey = AfxGetApp()->GetAppRegistryKey();

	typeChoice = AfxGetApp()->GetProfileInt(_T("Screen"), _T("typeChoice"), typeChoice);
	ListType = AfxGetApp()->GetProfileIntW(_T("Screen"), _T("ListType"), ListType);

	LineCount = AfxGetApp()->GetProfileInt(_T("Screen"), _T("LineCount"), LineCount);
	RectCount = AfxGetApp()->GetProfileInt(_T("Screen"), _T("RectCount"), RectCount);

	Width = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Width"), Width);
	Height = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Height"), Height);

	str = AfxGetApp()->GetProfileString(_T("Screen"), _T("Text"), str);
	fontStyle = AfxGetApp()->GetProfileString(_T("Screen"), _T("FontStyle"), fontStyle);
	FontSize = AfxGetApp()->GetProfileInt(_T("Screen"), _T("FontSize"), FontSize);

	degree = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Degree"), degree);

	color_r = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Color_R"), color_r);
	color_g = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Color_G"), color_g);
	color_b = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Color_B"), color_b);

	Outline_r = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Outline_R"), Outline_r);
	Outline_g = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Outline_G"), Outline_g);
	Outline_b = AfxGetApp()->GetProfileInt(_T("Screen"), _T("Outline_B"), Outline_b);

	OutlineSize = AfxGetApp()->GetProfileInt(_T("Screen"), _T("OutlineSize"), OutlineSize);

	blend = AfxGetApp()->GetProfileInt(_T("Screen"), _T("blend"), blend);
}
void CSettingDlg::WriteRegistry()
{
	CWinApp* pApp = AfxGetApp();

	HKEY mainKey = AfxGetApp()->GetAppRegistryKey();

	theApp.WriteProfileInt(_T("Screen"), _T("typeChoice"), typeChoice);
	theApp.WriteProfileInt(_T("Screen"), _T("ListType"), ListType);

	theApp.WriteProfileInt(_T("Screen"), _T("LineCount"), LineCount);
	theApp.WriteProfileInt(_T("Screen"), _T("RectCount"), RectCount);

	theApp.WriteProfileInt(_T("Screen"), _T("Width"), Width);
	theApp.WriteProfileInt(_T("Screen"), _T("Height"), Height);

	theApp.WriteProfileString(_T("Screen"), _T("Text"), str);
	theApp.WriteProfileString(_T("Screen"), _T("FontStyle"), fontStyle);
	theApp.WriteProfileInt(_T("Screen"), _T("FontSize"), FontSize);

	theApp.WriteProfileInt(_T("Screen"), _T("Degree"), degree);

	theApp.WriteProfileInt(_T("Screen"), _T("Color_R"), color_r);
	theApp.WriteProfileInt(_T("Screen"), _T("Color_G"), color_g);
	theApp.WriteProfileInt(_T("Screen"), _T("Color_B"), color_b);


	theApp.WriteProfileInt(_T("Screen"), _T("Outline_R"), Outline_r);
	theApp.WriteProfileInt(_T("Screen"), _T("Outline_G"), Outline_g);
	theApp.WriteProfileInt(_T("Screen"), _T("Outline_B"), Outline_b);

	theApp.WriteProfileInt(_T("Screen"), _T("OutlineSize"), OutlineSize);

	theApp.WriteProfileInt(_T("Screen"), _T("blend"), blend);
}

void CSettingDlg::OnBnClickedButton1()
{
	//CWinThread* pThread = AfxBeginThread(pParam, this);
	
	CRegistry reg;
	reg.CreateRegKey();

	UpdateData(TRUE);

	m_MainInfo.typeChoice = typeChoice;
	m_MainInfo.ListType = ListType;

	m_MainInfo.LineCount = LineCount;
	m_MainInfo.RectCount = RectCount;

	m_MainInfo.Width = Width;
	m_MainInfo.Height = Height;

	m_MainInfo.str = str;
	m_MainInfo.fontStyle = fontStyle;
	m_MainInfo.FontSize = FontSize;
	m_MainInfo.degree = degree;
	m_MainInfo.color_r = color_r;
	m_MainInfo.color_g = color_g;
	m_MainInfo.color_b = color_b;
	m_MainInfo.Outline_r = Outline_r;
	m_MainInfo.Outline_g = Outline_g;
	m_MainInfo.Outline_b = Outline_b;

	m_MainInfo.OutlineSize = OutlineSize;
	
	m_MainInfo.blend = blend;
	//UpdateData(TRUE);
	reg.OpenRegKey(typeChoice, ListType, LineCount, RectCount, Width, Height, str, fontStyle, FontSize, degree, color_r, color_g, color_b,
		Outline_r, Outline_g, Outline_b, OutlineSize, blend);

		
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}



BOOL CSettingDlg::PreTranslateMessage(MSG* pMsg)
{
	//if (pMsg->wParam == WM_KEYDOWN || pMsg->wParam == VK_RETURN)
	//{
		if (GetDlgItem(IDC_EDIT5) == GetFocus())
		{
			if (pMsg->wParam == VK_RETURN) {
				printf("\n");
			}
		/*	CEdit* edit = (CEdit*)GetDlgItem(IDC_EDIT5);
			int nLen = edit->GetWindowTextLength();
			edit->SetSel(nLen, nLen);
			edit->ReplaceSel(_T("\r\n"));*/
		}
	//}
	
	return CDialogEx::PreTranslateMessage(pMsg);
}




