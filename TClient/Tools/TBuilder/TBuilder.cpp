// TBuilder.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
//

#include "stdafx.h"
#include "TBuilder.h"
#include "TBuilderDlg.h"
#include ".\tbuilder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTBuilderApp

BEGIN_MESSAGE_MAP(CTBuilderApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTBuilderApp 생성

CTBuilderApp::CTBuilderApp()
{
	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.
}


// 유일한 CTBuilderApp 개체입니다.

CTBuilderApp theApp;


// CTBuilderApp 초기화

BOOL CTBuilderApp::InitInstance()
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

	CTBuilderDlg dlg;
	m_pMainWnd = &dlg;

	strcpy( CItemBase::m_szTempPath, ".\\" );

	dlg.m_strHISTORYFOLDER.Empty();
	dlg.m_strLISTFOLDER.Empty();
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

int CTBuilderApp::ExitInstance()
{
	return CWinApp::ExitInstance();
}

void CTBuilderApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	((CTBuilderDlg *) m_pMainWnd)->m_bUsePrevID = GetProfileInt(
		_T("Config"),
		_T("Use Previous ID"), 1);

	((CTBuilderDlg *) m_pMainWnd)->m_strHISTORYFOLDER = GetProfileString(
		_T("Config"),
		_T("HISTORY Folder"),
		_T(""));

	((CTBuilderDlg *) m_pMainWnd)->m_strLISTFOLDER = GetProfileString(
		_T("Config"),
		_T("LIST Folder"),
		_T(""));

	((CTBuilderDlg *) m_pMainWnd)->m_strSRCFOLDER = GetProfileString(
		_T("Config"),
		_T("SRC Folder"),
		_T(""));

	((CTBuilderDlg *) m_pMainWnd)->m_strDESTFOLDER = GetProfileString(
		_T("Config"),
		_T("DEST Folder"),
		_T(""));
}

void CTBuilderApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileInt(
		_T("Config"),
		_T("Use Previous ID"),
		INT(((CTBuilderDlg *) m_pMainWnd)->m_bUsePrevID));

	WriteProfileString(
		_T("Config"),
		_T("HISTORY Folder"),
		LPCSTR(((CTBuilderDlg *) m_pMainWnd)->m_strHISTORYFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("LIST Folder"),
		LPCSTR(((CTBuilderDlg *) m_pMainWnd)->m_strLISTFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("SRC Folder"),
		LPCSTR(((CTBuilderDlg *) m_pMainWnd)->m_strSRCFOLDER));

	WriteProfileString(
		_T("Config"),
		_T("DEST Folder"),
		LPCSTR(((CTBuilderDlg *) m_pMainWnd)->m_strDESTFOLDER));
}
