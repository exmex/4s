// TChartProtect.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TChartProtect.h"
#include "TChartProtectDlg.h"
#include ".\tchartprotect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTChartProtectApp

BEGIN_MESSAGE_MAP(CTChartProtectApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTChartProtectApp construction

CTChartProtectApp::CTChartProtectApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTChartProtectApp object

CTChartProtectApp theApp;


// CTChartProtectApp initialization

BOOL CTChartProtectApp::InitInstance()
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

	CTChartProtectDlg dlg;
	m_pMainWnd = &dlg;

	dlg.m_strDESTFOLDER.Empty();
	dlg.m_strSRCFOLDER.Empty();
	dlg.m_strLISTFOLDER.Empty();
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

int CTChartProtectApp::ExitInstance()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	return CWinApp::ExitInstance();
}

void CTChartProtectApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	((CTChartProtectDlg *) m_pMainWnd)->m_strDESTFOLDER = GetProfileString(
		_T("Config"),
		_T("DEST Folder"),
		_T(""));

	((CTChartProtectDlg *) m_pMainWnd)->m_strSRCFOLDER = GetProfileString(
		_T("Config"),
		_T("SRC Folder"),
		_T(""));
}

void CTChartProtectApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileString(
		_T("Config"),
		_T("DEST Folder"),
		LPCSTR(((CTChartProtectDlg *) m_pMainWnd)->m_strDESTFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("SRC Folder"),
		LPCSTR(((CTChartProtectDlg *) m_pMainWnd)->m_strSRCFOLDER));
}
