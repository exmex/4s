// TMFConvertor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TMFConvertor.h"
#include "TMFConvertorDlg.h"
#include ".\TMFConvertor.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTMFConvertorApp

BEGIN_MESSAGE_MAP(CTMFConvertorApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTMFConvertorApp construction

CTMFConvertorApp::CTMFConvertorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTMFConvertorApp object

CTMFConvertorApp theApp;


// CTMFConvertorApp initialization

BOOL CTMFConvertorApp::InitInstance()
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

	CTMFConvertorDlg dlg;
	m_pMainWnd = &dlg;

	dlg.m_strDESTFOLDER.Empty();
	dlg.m_strSRCFOLDER.Empty();
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

int CTMFConvertorApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

void CTMFConvertorApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	((CTMFConvertorDlg *) m_pMainWnd)->m_strDESTFOLDER = GetProfileString(
		_T("Config"),
		_T("DEST Folder"),
		_T(""));

	((CTMFConvertorDlg *) m_pMainWnd)->m_strSRCFOLDER = GetProfileString(
		_T("Config"),
		_T("SRC Folder"),
		_T(""));
}

void CTMFConvertorApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileString(
		_T("Config"),
		_T("DEST Folder"),
		LPCSTR(((CTMFConvertorDlg *) m_pMainWnd)->m_strDESTFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("SRC Folder"),
		LPCSTR(((CTMFConvertorDlg *) m_pMainWnd)->m_strSRCFOLDER));
}

