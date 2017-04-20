#include "stdafx.h"
#include "Game Studio.h"
#include "ProgressDlg.h"


IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)
CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
	, m_strStatus(_T(""))
	, m_strCancel(_T(""))
	, m_strTitle(_T(""))
	, m_bPercent(0)
{
	m_pThreadFunc = NULL;
	m_pThread = NULL;
	m_lParam = 0;
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_STATUS, m_strStatus);
	DDX_Text(pDX, IDC_STATIC_PERCENT, m_bPercent);
	DDX_Control(pDX, IDC_PROGRESS, m_cProgressBar);
}


BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_COMMAND(ID_PROGRESS_UPDATE, OnProgressUpdate)
	ON_COMMAND(ID_PROGRESS_EXIT, OnProgressExit)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CProgressDlg::OnBnClickedCancel()
{
	if(m_pThread)
	{
		m_pThread->SuspendThread();

		if( MessageBox( m_strCancel, NULL, MB_YESNO) == IDYES )
		{
			m_pThread->ExitInstance();
			OnCancel();
		}
		else
			m_pThread->ResumeThread();
	}
}

void CProgressDlg::OnProgressUpdate()
{
	UpdateData(FALSE);
}

void CProgressDlg::OnProgressExit()
{
	CDialog::OnOK();
}

void CProgressDlg::OnDestroy()
{
	CDialog::OnDestroy();

	m_pThreadFunc = NULL;
	m_strTitle = _T("");
	m_pThread = NULL;
	m_lParam = 0;
}

BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	m_cProgressBar.SetRange( 0, 100);
	SetWindowText(m_strTitle);

	if(m_pThreadFunc)
	{
		m_pThread = AfxBeginThread(
			m_pThreadFunc,
			(LPVOID) m_lParam);
	}

	return TRUE;
}
