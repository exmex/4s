// ClientLauncher.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ClientLauncher.h"
#include "ClientLauncherDlg.h"
#include ".\clientlauncher.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClientLauncherApp

BEGIN_MESSAGE_MAP(CClientLauncherApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CClientLauncherApp construction

CClientLauncherApp::CClientLauncherApp()
{
}


// The one and only CClientLauncherApp object

CClientLauncherApp theApp;


// CClientLauncherApp initialization

BOOL CClientLauncherApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	if(!AfxSocketInit())
		return FALSE;

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Tachyon Engine"));
	CClientLauncherDlg dlg;

	if(!dlg.BeginProtect())
	{
		CString strTITLE;
		CString strMSG;

		strMSG.LoadString(IDS_ERROR_LOAD_FILE);
		strTITLE.LoadString(IDS_APP_NAME);
		MessageBox( NULL, strMSG, strTITLE, MB_OK);

		return FALSE;
	}

	m_pMainWnd = &dlg;
	dlg.m_strPARAM.Empty();
	LoadStdProfileSettings();

	INT_PTR nResponse = dlg.DoModal();
	dlg.EndProtect();
	m_pMainWnd = &dlg;
	SaveStdProfileSettings();
	m_pMainWnd = NULL;

	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CClientLauncherApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	((CClientLauncherDlg *) m_pMainWnd)->m_strPARAM = GetProfileString(
		_T("Config"),
		_T("PARAM"),
		_T(""));
}

void CClientLauncherApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileString(
		_T("Config"),
		_T("PARAM"),
		LPCSTR(((CClientLauncherDlg *) m_pMainWnd)->m_strPARAM));
}
