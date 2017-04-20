// TMacroAddDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TMacroMachine.h"
#include "TMacroAddDlg.h"
#include ".\tmacroadddlg.h"


// CTMacroAddDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTMacroAddDlg, CDialog)
CTMacroAddDlg::CTMacroAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMacroAddDlg::IDD, pParent)
{
}

CTMacroAddDlg::~CTMacroAddDlg()
{
}

void CTMacroAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbLoop);
}


BEGIN_MESSAGE_MAP(CTMacroAddDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CTMacroAddDlg 메시지 처리기입니다.

BOOL CTMacroAddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cbLoop.AddString(_T("FALSE"));
	m_cbLoop.AddString(_T("TRUE"));
	m_cbLoop.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTMacroAddDlg::OnBnClickedOk()
{
	m_bLoop = m_cbLoop.GetCurSel();
	GetDlgItemText(IDC_EDIT1, m_strName);

	if(m_strName == _T(""))
	{
		AfxMessageBox(_T("이름에 공백을 넣을수는 없습니다."));
		return;
	}

	OnOK();
}