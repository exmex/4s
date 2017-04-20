// TMeshRegulator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TMeshRegulator.h"
#include "TMeshRegulatorDlg.h"
#include ".\tmeshregulator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTMeshRegulatorApp

BEGIN_MESSAGE_MAP(CTMeshRegulatorApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTMeshRegulatorApp construction

CTMeshRegulatorApp::CTMeshRegulatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTMeshRegulatorApp object

CTMeshRegulatorApp theApp;


// CTMeshRegulatorApp initialization

BOOL CTMeshRegulatorApp::InitInstance()
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

	CTMeshRegulatorDlg dlg;
	m_pMainWnd = &dlg;

	dlg.m_strDESTFOLDER.Empty();
	dlg.m_strTMFFOLDER.Empty();
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

int CTMeshRegulatorApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

void CTMeshRegulatorApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	((CTMeshRegulatorDlg *) m_pMainWnd)->m_strDESTFOLDER = GetProfileString(
		_T("Config"),
		_T("DEST Folder"),
		_T(""));

	((CTMeshRegulatorDlg *) m_pMainWnd)->m_strTMFFOLDER = GetProfileString(
		_T("Config"),
		_T("TMF Folder"),
		_T(""));

	((CTMeshRegulatorDlg *) m_pMainWnd)->m_strSRCFOLDER = GetProfileString(
		_T("Config"),
		_T("SRC Folder"),
		_T(""));
}

void CTMeshRegulatorApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileString(
		_T("Config"),
		_T("DEST Folder"),
		LPCSTR(((CTMeshRegulatorDlg *) m_pMainWnd)->m_strDESTFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("TMF Folder"),
		LPCSTR(((CTMeshRegulatorDlg *) m_pMainWnd)->m_strTMFFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("SRC Folder"),
		LPCSTR(((CTMeshRegulatorDlg *) m_pMainWnd)->m_strSRCFOLDER));
}
