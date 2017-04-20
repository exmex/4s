// TProgressDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TTranslator.h"
#include "TProgressDlg.h"
#include "TTranslatorDlg.h"


// CTProgressDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTProgressDlg, CDialog)
CTProgressDlg::CTProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTProgressDlg::IDD, pParent)
	, m_strCURRENT(_T(""))
	, m_strTOTAL(_T(""))
	, m_strACTION(_T(""))
{
	m_pTHREAD = NULL;
	m_hTHREAD = NULL;
	m_lParam = NULL;
}

CTProgressDlg::~CTProgressDlg()
{
}

void CTProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_CURRENT, m_cCURRENT);
	DDX_Control(pDX, IDC_PROGRESS_TOTAL, m_cTOTAL);
	DDX_Text(pDX, IDC_STATIC_ACTION, m_strACTION);
	DDX_Text(pDX, IDC_STATIC_TOTAL, m_strTOTAL);
	DDX_Text(pDX, IDC_STATIC_CURRENT, m_strCURRENT);
	DDX_Control(pDX, IDCANCEL, m_cCANCEL);
}


BEGIN_MESSAGE_MAP(CTProgressDlg, CDialog)
	ON_COMMAND(ID_MENU_UPDATE, OnMenuUpdate)
	ON_COMMAND(ID_MENU_CLOSE, OnMenuClose)
END_MESSAGE_MAP()


// CTProgressDlg 메시지 처리기입니다.

BOOL CTProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	DWORD dwTHREAD;

	SetIcon( hIcon, TRUE);
	SetIcon( hIcon, FALSE);

	m_cCURRENT.SetRange32( 0, TDEF_MAX_PROGRESS);
	m_cTOTAL.SetRange32( 0, TDEF_MAX_PROGRESS);

	m_cCURRENT.SetPos(0);
	m_cTOTAL.SetPos(0);

	if( m_pTHREAD && m_lParam && ((LPTBDPARAM) m_lParam)->m_pHOST && ((LPTBDPARAM) m_lParam)->m_pPROGRESS )
	{
		((CTTranslatorDlg *) ((LPTBDPARAM) m_lParam)->m_pHOST)->SetCANCEL(FALSE);

		m_hTHREAD = CreateThread(
			NULL, 0,
			m_pTHREAD,
			m_lParam,
			0, &dwTHREAD);
	}
	else
		m_hTHREAD = NULL;

	if(!m_hTHREAD)
	{
		AfxMessageBox(IDS_ERROR_CANNOT_CREATE_THREAD);
		return FALSE;
	}

	return TRUE;
}

void CTProgressDlg::OnMenuUpdate()
{
	UpdateData(FALSE);
}

void CTProgressDlg::OnMenuClose()
{
	CDialog::OnOK();
}

void CTProgressDlg::OnCancel()
{
	CTTranslatorDlg *pHOST = m_lParam ? (CTTranslatorDlg *) ((LPTBDPARAM) m_lParam)->m_pHOST : NULL;

	if( !m_hTHREAD || !pHOST )
	{
		CDialog::OnCancel();
		return;
	}

	SMART_LOCKCS(&pHOST->m_cs);
	if( !pHOST->m_bCANCEL && AfxMessageBox( IDS_FMT_CANCEL_BUILD, MB_YESNO) == IDYES )
	{
		DWORD dwTHREAD;

		m_strACTION.LoadString(IDS_FMT_ACTION_CANCEL);
		m_cCANCEL.EnableWindow(FALSE);
		pHOST->m_bCANCEL = TRUE;
		UpdateData(FALSE);

		HANDLE hTHREAD = CreateThread(
			NULL, 0,
			(LPTHREAD_START_ROUTINE) __WaitForCANCEL,
			this, 0,
			&dwTHREAD);

		if(!hTHREAD)
			CDialog::OnCancel();
	}
}

DWORD CTProgressDlg::__WaitForCANCEL( DWORD_PTR lParam)
{
	if(lParam)
		((CTProgressDlg *) lParam)->WaitForCANCEL();

	return 0;
}

void CTProgressDlg::WaitForCANCEL()
{
	WaitForSingleObject( m_hTHREAD, INFINITE);
	PostMessage( WM_COMMAND, ID_MENU_CLOSE);
}
