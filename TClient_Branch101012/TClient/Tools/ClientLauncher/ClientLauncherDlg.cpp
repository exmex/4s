// ClientLauncherDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ClientLauncher.h"
#include "ClientLauncherDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define TMP_TIMER						(100)


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CClientLauncherDlg dialog



CClientLauncherDlg::CClientLauncherDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CClientLauncherDlg::IDD, pParent)
	, m_strPARAM(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_bHACK = TMP_HACK_NONE;
	m_bRUN = FALSE;
	m_hWND = NULL;
	m_hMP = NULL;
}

void CClientLauncherDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PARAM, m_strPARAM);
}

BEGIN_MESSAGE_MAP(CClientLauncherDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CClientLauncherDlg message handlers

BOOL CClientLauncherDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	SetHWND(GetSafeHwnd());

	// TODO: Add extra initialization here

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CClientLauncherDlg::OnDestroy()
{
	SetHWND(NULL);
	EndThread();
	HackMSG();

	CDialog::OnDestroy();
}

void CClientLauncherDlg::HackMSG()
{
	switch(m_bHACK)
	{
	case TMP_HACK_INVALID_PE	:
		{
			CString strTITLE;
			CString strMSG;

			strMSG.LoadString(IDS_ERROR_LOAD_FILE);
			strTITLE.LoadString(IDS_APP_NAME);
			::MessageBox( NULL, strMSG, strTITLE, MB_OK);
		}

		break;

	case TMP_HACK_TIMEOUT	:
		{
			CString strTITLE;
			CString strMSG;

			strMSG.LoadString(IDS_ERROR_TIMEOUT);
			strTITLE.LoadString(IDS_APP_NAME);
			::MessageBox( NULL, strMSG, strTITLE, MB_OK);
		}

		break;

	case TMP_HACK_FOUND		:
		{
			CString strTITLE;
			CString strMSG;

			strMSG.LoadString(IDS_ERROR_HACK_DETECTED);
			strTITLE.LoadString(IDS_APP_NAME);
			::MessageBox( NULL, strMSG, strTITLE, MB_OK);
		}

		break;
	}
}

void CClientLauncherDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CClientLauncherDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CClientLauncherDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CClientLauncherDlg::OnOK()
{
	EndThread();
	m_bHACK = LaunchTClient();

	CDialog::OnOK();
}

BYTE CClientLauncherDlg::BeginProtect()
{
	DWORD dwThreadID;

	InitializeCriticalSection(&m_cs);
	SetRUN(TRUE);

	if( !m_vModuleGuard.InitProtector( MAKEINTRESOURCE(IDR_MPCFILE), _T("MPCFILE")) ||
		!m_vModuleGuard.BeginWatch() )
	{
		EndProtect();
		m_bRUN = FALSE;

		return FALSE;
	}

	m_hMP = ::CreateThread(
		NULL, 0,
		_MPThread,
		(LPVOID) this,
		0, &dwThreadID);

	if(!m_hMP)
	{
		EndProtect();
		m_bRUN = FALSE;

		return FALSE;
	}

	return TRUE;
}

void CClientLauncherDlg::EndThread()
{
	if(m_hMP)
	{
		SetRUN(FALSE);
		WaitForSingleObject( m_hMP, INFINITE);

		m_hMP = NULL;
	}
}

void CClientLauncherDlg::EndProtect()
{
	EndThread();

	DeleteCriticalSection(&m_cs);
	m_vModuleGuard.ClearModule();
}

DWORD WINAPI CClientLauncherDlg::_MPThread( LPVOID lpParam)
{
	((CClientLauncherDlg *) lpParam)->MPThread();
	return 0;
}

void CClientLauncherDlg::MPThread()
{
	while(GetRUN())
	{
		if(!m_vModuleGuard.CheckValid())
			PostQUIT();

		Sleep(TMP_TIMER);
	}
}

void CClientLauncherDlg::SetRUN( BYTE bRUN)
{
	SMART_LOCKCS(&m_cs);
	m_bRUN = bRUN;
}

BYTE CClientLauncherDlg::GetRUN()
{
	SMART_LOCKCS(&m_cs);
	return m_bRUN ? TRUE : FALSE;
}

void CClientLauncherDlg::SetHWND( HWND hWND)
{
	SMART_LOCKCS(&m_cs);
	m_hWND = hWND;
}

void CClientLauncherDlg::PostQUIT()
{
	SMART_LOCKCS(&m_cs);

	if(!m_bHACK)
		m_bHACK = m_vModuleGuard.HackDetected() ? TMP_HACK_FOUND : TMP_HACK_TIMEOUT;

	if(m_hWND)
		::PostMessage( m_hWND, WM_COMMAND, IDCANCEL, 0);
}

BYTE CClientLauncherDlg::LaunchTClient()
{
	CTModuleProtector vLauncher;
	UpdateData();

	if(m_bHACK)
		return m_bHACK;

	if(!m_vModuleGuard.CheckValid())
		return m_vModuleGuard.HackDetected() ? TMP_HACK_FOUND : TMP_HACK_TIMEOUT;

	if(!vLauncher.InitProtector(CString(_T(".\\TClientMP.mpc"))))
		return TMP_HACK_INVALID_PE;

	PROCESS_INFORMATION ProcessInfo;
	STARTUPINFO StartupInfo;

	ZeroMemory( &ProcessInfo, sizeof(PROCESS_INFORMATION)); 
	ZeroMemory( &StartupInfo, sizeof(STARTUPINFO));

	StartupInfo.cb = sizeof(STARTUPINFO);
	StartupInfo.dwFlags = STARTF_USESHOWWINDOW;
	StartupInfo.wShowWindow = SW_SHOWNORMAL;

	CString strCommandLine;
	strCommandLine.Format( _T("TClient.exe %s"), m_strPARAM);

	if(!CreateProcess( _T("TClient.exe"), strCommandLine.LockBuffer(), 
		NULL, NULL, FALSE, 0, NULL, NULL, &StartupInfo, &ProcessInfo))
	{
		strCommandLine.ReleaseBuffer();
		return TMP_HACK_INVALID_PE;
	}
	strCommandLine.ReleaseBuffer();

	return TMP_HACK_NONE;
}
