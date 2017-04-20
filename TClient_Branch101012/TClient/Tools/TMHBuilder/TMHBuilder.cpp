// TMHBuilder.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TMHBuilder.h"
#include "TMHBuilderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTMHBuilderApp

BEGIN_MESSAGE_MAP(CTMHBuilderApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTMHBuilderApp construction

CTMHBuilderApp::CTMHBuilderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTMHBuilderApp object

CTMHBuilderApp theApp;


// CTMHBuilderApp initialization

BOOL CTMHBuilderApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Tachyon Engine"));

	CTMHBuilderDlg dlg;
	m_pMainWnd = &dlg;

	dlg.m_strLISTFOLDER.Empty();
	dlg.m_strRESFOLDER.Empty();
	dlg.m_strTMHFOLDER.Empty();
	LoadStdProfileSettings();

	INT_PTR nResponse = dlg.DoModal();
	m_pMainWnd = &dlg;
	SaveStdProfileSettings();
	m_pMainWnd = NULL;

	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

void CTMHBuilderApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	((CTMHBuilderDlg *) m_pMainWnd)->m_strLISTFOLDER = GetProfileString(
		_T("Config"),
		_T("LIST Folder"),
		_T(""));

	((CTMHBuilderDlg *) m_pMainWnd)->m_strRESFOLDER = GetProfileString(
		_T("Config"),
		_T("RES Folder"),
		_T(""));

	((CTMHBuilderDlg *) m_pMainWnd)->m_strTMHFOLDER = GetProfileString(
		_T("Config"),
		_T("TMH Folder"),
		_T(""));
}

void CTMHBuilderApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileString(
		_T("Config"),
		_T("LIST Folder"),
		LPCSTR(((CTMHBuilderDlg *) m_pMainWnd)->m_strLISTFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("RES Folder"),
		LPCSTR(((CTMHBuilderDlg *) m_pMainWnd)->m_strRESFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("TMH Folder"),
		LPCSTR(((CTMHBuilderDlg *) m_pMainWnd)->m_strTMHFOLDER));
}
