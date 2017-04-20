// TZoomDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TZoomDlg.h"


// CTZoomDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTZoomDlg, CDialog)

CTZoomDlg::CTZoomDlg(CWnd* pParent /*=NULL*/)
:CDialog(CTZoomDlg::IDD, pParent)
, m_strSCALE(_T(""))
{
}

CTZoomDlg::~CTZoomDlg()
{
}

void CTZoomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_strSCALE);
}


BEGIN_MESSAGE_MAP(CTZoomDlg, CDialog)
END_MESSAGE_MAP()


// CTZoomDlg 메시지 처리기입니다.

BOOL CTZoomDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	return TRUE;
}

void CTZoomDlg::OnOK()
{
	CDialog::OnOK();
	FLOAT fSCALE;

	sscanf( LPCTSTR(m_strSCALE), _T("%f"), &fSCALE);
	m_strSCALE.Format( IDS_FMT_POS_DATA, fSCALE);
}
