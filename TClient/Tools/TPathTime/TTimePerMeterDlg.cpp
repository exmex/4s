// TTimePerMeterDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TPathTime.h"
#include "TTimePerMeterDlg.h"


// CTTimePerMeterDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTTimePerMeterDlg, CDialog)
CTTimePerMeterDlg::CTTimePerMeterDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTTimePerMeterDlg::IDD, pParent)
	, m_fTIME(0)
{
}

CTTimePerMeterDlg::~CTTimePerMeterDlg()
{
}

void CTTimePerMeterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TIME, m_fTIME);
}


BEGIN_MESSAGE_MAP(CTTimePerMeterDlg, CDialog)
END_MESSAGE_MAP()


// CTTimePerMeterDlg 메시지 처리기입니다.

BOOL CTTimePerMeterDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_fTIME = theApp.m_fTIME;
	UpdateData(FALSE);

	return TRUE;
}
