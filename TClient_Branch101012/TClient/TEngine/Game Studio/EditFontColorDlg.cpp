// EditFontColorDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EditFontColorDlg.h"
#include ".\editfontcolordlg.h"


// CEditFontColorDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditFontColorDlg, CDialog)
CEditFontColorDlg::CEditFontColorDlg(CWnd* pParent /*=NULL*/)
	: CBarContent(CEditFontColorDlg::IDD, pParent)
	, m_dwFontColor(NULL)
	, m_dwRValue(0)
	, m_dwGValue(0)
	, m_dwBValue(0)
	, m_bInit(FALSE)
	, m_dwFONTCOLORHEX(0)
	, m_strFontColorHex(_T(""))
{
}

CEditFontColorDlg::~CEditFontColorDlg()
{
}

void CEditFontColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FONT_CLRCTRL, m_cControlBox);
	DDX_Control(pDX, IDC_STATIC_FONT_COLOR, m_cColorBox);
	DDX_Control(pDX, IDC_FONT_SPIN_RVALUE, m_spinRValue);
	DDX_Control(pDX, IDC_FONT_SPIN_GVALUE, m_spinGValue);
	DDX_Control(pDX, IDC_FONT_SPIN_BVALUE, m_spinBValue);
	DDX_Text(pDX, IDC_EDIT_FONT_RVALUE, m_dwRValue);
	DDX_Text(pDX, IDC_EDIT_FONT_GVALUE, m_dwGValue);
	DDX_Text(pDX, IDC_EDIT_FONT_BVALUE, m_dwBValue);
	DDX_Text(pDX, IDC_EDIT_FONT_COLOR_HEX, m_strFontColorHex);
}


BEGIN_MESSAGE_MAP(CEditFontColorDlg, CDialog)
	ON_MESSAGE( WM_COLOR_SELECTED, OnColorSelected)
	ON_EN_CHANGE(IDC_EDIT_FONT_RVALUE, OnEnChangeEditColor)
	ON_EN_CHANGE(IDC_EDIT_FONT_GVALUE, OnEnChangeEditColor)
	ON_EN_CHANGE(IDC_EDIT_FONT_BVALUE, OnEnChangeEditColor)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CEditFontColorDlg 메시지 처리기입니다.

BOOL CEditFontColorDlg::OnInitDialog()
{
	CBarContent::OnInitDialog();

	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEL);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SETDIST);

	m_cControlBox.GetClientRect(&rect);
	m_cControlBox.ClientToScreen(&rect);
	ScreenToClient(&rect);
	rect.DeflateRect( 1, 1);

	if(!m_cColorCtrl.Create(
		NULL, NULL,
		WS_CHILD|WS_VISIBLE,
		rect, this, 0))
		return FALSE;
	m_cColorCtrl = CTColorCtrl::MODE_HSL_PANEL;

	m_dwFONTCOLORHEX = *m_dwFontColor;

	m_dwRValue = GETVALUE_R(m_dwFONTCOLORHEX);
	m_dwGValue = GETVALUE_G(m_dwFONTCOLORHEX);
	m_dwBValue = GETVALUE_B(m_dwFONTCOLORHEX);

	m_cColorCtrl.SetupColor( RGB(
		GETVALUE_R(m_dwFONTCOLORHEX),
		GETVALUE_G(m_dwFONTCOLORHEX),
		GETVALUE_B(m_dwFONTCOLORHEX)),
			TRUE);

	m_cColorBox.SetColor( RGB(
		GETVALUE_R(m_dwFONTCOLORHEX),
		GETVALUE_G(m_dwFONTCOLORHEX),
		GETVALUE_B(m_dwFONTCOLORHEX)));

	
	m_strFontColorHex.Format("0x%x", m_dwFONTCOLORHEX);

	m_spinRValue.SetRange( 0, 255);
	m_spinGValue.SetRange( 0, 255);
	m_spinBValue.SetRange( 0, 255);

	UpdateData();
	
	m_bInit = TRUE;
	return TRUE;
}

void CEditFontColorDlg::InitColor(DWORD* dwColor)
{
	m_dwFontColor = dwColor;
}

void CEditFontColorDlg::OnEnChangeEditColor()
{
	if( !m_bInit )
		return;
	UpdateData();

	m_cColorCtrl.SetupColor( RGB(
		m_dwRValue,
		m_dwGValue,
		m_dwBValue),
			TRUE);

	m_dwFONTCOLORHEX = VALUEARGB(
		0xFF,
		m_dwRValue,
		m_dwGValue,
		m_dwBValue);

	m_strFontColorHex.Format("0x%x", m_dwFONTCOLORHEX);
	UpdateData(FALSE);
}

LRESULT CEditFontColorDlg::OnColorSelected( WPARAM wParam, LPARAM lParam)
{
	UpdateData();
	*m_dwFontColor = ((CTColorCtrl::selection_colors *) lParam)->m_clrNew;
	
	m_dwRValue = GetRValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
	m_dwGValue = GetGValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
	m_dwBValue = GetBValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

	m_dwFONTCOLORHEX = VALUEARGB(
		0xFF,
		m_dwRValue,
		m_dwGValue,
		m_dwBValue);

	m_strFontColorHex.Format("0x%x", m_dwFONTCOLORHEX);

	m_cColorBox.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

	UpdateData(FALSE);

	return 0;
}

void CEditFontColorDlg::OnBnClickedOk()
{
	UpdateData();
    *m_dwFontColor = m_dwFONTCOLORHEX;
	CDialog::OnOK();
}
