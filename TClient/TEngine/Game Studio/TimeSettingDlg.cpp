// TimeSettingDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "TimeSettingDlg.h"


// CTimeSettingDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTimeSettingDlg, CDialog)
CTimeSettingDlg::CTimeSettingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeSettingDlg::IDD, pParent)
	, m_dwTotalTick(0)
	, m_dwTotalFrame(0)
{
}

CTimeSettingDlg::~CTimeSettingDlg()
{
}

void CTimeSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_TOTAL_TICK, m_dwTotalTick);
	DDX_Text(pDX, IDC_EDIT_TOTAL_FRAME, m_dwTotalFrame);
}


BEGIN_MESSAGE_MAP(CTimeSettingDlg, CDialog)
END_MESSAGE_MAP()


// CTimeSettingDlg 메시지 처리기입니다.

BOOL CTimeSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);
	UpdateData(FALSE);

	return TRUE;
}

void CTimeSettingDlg::OnOK()
{
	UpdateData();

	if( m_dwTotalTick == 0 )
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_TOTAL_TICK);
		strTitle.LoadString(IDR_MAINFRAME);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		return;
	}

	if( m_dwTotalFrame == 0 )
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_TOTALFRAME);
		strTitle.LoadString(IDR_MAINFRAME);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		return;
	}

	CDialog::OnOK();
}
