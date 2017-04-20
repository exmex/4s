// TIDHBuilder.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TIDHBuilder.h"
#include "TIDHBuilderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTIDHBuilderApp

BEGIN_MESSAGE_MAP(CTIDHBuilderApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTIDHBuilderApp construction

CTIDHBuilderApp::CTIDHBuilderApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTIDHBuilderApp object

CTIDHBuilderApp theApp;


// CTIDHBuilderApp initialization

BOOL CTIDHBuilderApp::InitInstance()
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

	CTIDHBuilderDlg dlg;
	m_pMainWnd = &dlg;

	dlg.m_strTSTRINGFOLDER.Empty();
	dlg.m_strMISSIONFOLDER.Empty();
	dlg.m_strTERMFOLDER.Empty();
	dlg.m_strLANGFOLDER.Empty();
	dlg.m_strINFOFOLDER.Empty();
	dlg.m_strNPCFOLDER.Empty();

	dlg.m_strHEADERFOLDER.Empty();
	dlg.m_strIDHFOLDER.Empty();

	dlg.m_strTSTRING.Empty();
	dlg.m_strMISSION.Empty();
	dlg.m_strTERM.Empty();
	dlg.m_strLANG.Empty();
	dlg.m_strINFO.Empty();
	dlg.m_strNPC.Empty();

	dlg.m_strHEADER.Empty();
	dlg.m_strIDH.Empty();
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

void CTIDHBuilderApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	((CTIDHBuilderDlg *) m_pMainWnd)->m_strTSTRINGFOLDER = GetProfileString(
		_T("Config"),
		_T("TSTRING"),
		_T(""));

	((CTIDHBuilderDlg *) m_pMainWnd)->m_strMISSIONFOLDER = GetProfileString(
		_T("Config"),
		_T("MISSION"),
		_T(""));

	((CTIDHBuilderDlg *) m_pMainWnd)->m_strTERMFOLDER = GetProfileString(
		_T("Config"),
		_T("TERM"),
		_T(""));

	((CTIDHBuilderDlg *) m_pMainWnd)->m_strNPCFOLDER = GetProfileString(
		_T("Config"),
		_T("NPC"),
		_T(""));

	((CTIDHBuilderDlg *) m_pMainWnd)->m_strLANGFOLDER = GetProfileString(
		_T("Config"),
		_T("LANG"),
		_T(""));

	((CTIDHBuilderDlg *) m_pMainWnd)->m_strINFOFOLDER = GetProfileString(
		_T("Config"),
		_T("INFO"),
		_T(""));

	((CTIDHBuilderDlg *) m_pMainWnd)->m_strHEADERFOLDER = GetProfileString(
		_T("Config"),
		_T("HEADER"),
		_T(""));

	((CTIDHBuilderDlg *) m_pMainWnd)->m_strIDHFOLDER = GetProfileString(
		_T("Config"),
		_T("IDH"),
		_T(""));
}

void CTIDHBuilderApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileString(
		_T("Config"),
		_T("TSTRING"),
		LPCSTR(((CTIDHBuilderDlg *) m_pMainWnd)->m_strTSTRINGFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("MISSION"),
		LPCSTR(((CTIDHBuilderDlg *) m_pMainWnd)->m_strMISSIONFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("TERM"),
		LPCSTR(((CTIDHBuilderDlg *) m_pMainWnd)->m_strTERMFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("NPC"),
		LPCSTR(((CTIDHBuilderDlg *) m_pMainWnd)->m_strNPCFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("LANG"),
		LPCSTR(((CTIDHBuilderDlg *) m_pMainWnd)->m_strLANGFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("INFO"),
		LPCSTR(((CTIDHBuilderDlg *) m_pMainWnd)->m_strINFOFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("HEADER"),
		LPCSTR(((CTIDHBuilderDlg *) m_pMainWnd)->m_strHEADERFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("IDH"),
		LPCSTR(((CTIDHBuilderDlg *) m_pMainWnd)->m_strIDHFOLDER));
}
