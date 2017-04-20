// TTranslator.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TTranslator.h"
#include "TTranslatorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTTranslatorApp

BEGIN_MESSAGE_MAP(CTTranslatorApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CTTranslatorApp construction

CTTranslatorApp::CTTranslatorApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTTranslatorApp object

CTTranslatorApp theApp;


// CTTranslatorApp initialization

BOOL CTTranslatorApp::InitInstance()
{
	InitCommonControls();
	CWinApp::InitInstance();
	AfxEnableControlContainer();
	SetRegistryKey(_T("Tachyon Engine"));

	CTTranslatorDlg dlg;
	m_pMainWnd = &dlg;

	if(!dlg.LoadTID())
	{
		AfxMessageBox( IDS_ERROR_LOAD_IDH, MB_OK);
		return FALSE;
	}

	dlg.m_strSOURCE.Empty();
	dlg.m_strTARGET.Empty();
	dlg.m_strTEXT.Empty();
	dlg.m_nFROM = 0;
	dlg.m_nTO = 0;
	LoadStdProfileSettings();

	INT_PTR nResponse = dlg.DoModal();
	m_pMainWnd = &dlg;
	dlg.ReleaseTID();
	SaveStdProfileSettings();
	m_pMainWnd = NULL;

	if (nResponse == IDOK)
	{
	}
	else if (nResponse == IDCANCEL)
	{
	}

	return FALSE;
}

void CTTranslatorApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	((CTTranslatorDlg *) m_pMainWnd)->m_nFROM = GetProfileInt(
		_T("Config"),
		_T("LANG_FROM"), 0);

	((CTTranslatorDlg *) m_pMainWnd)->m_nTO = GetProfileInt(
		_T("Config"),
		_T("LANG_TO"), 0);

	((CTTranslatorDlg *) m_pMainWnd)->m_strSOURCE = GetProfileString(
		_T("Config"),
		_T("SOURCE"),
		_T(""));

	((CTTranslatorDlg *) m_pMainWnd)->m_strTARGET = GetProfileString(
		_T("Config"),
		_T("TARGET"),
		_T(""));

	((CTTranslatorDlg *) m_pMainWnd)->m_strTEXT = GetProfileString(
		_T("Config"),
		_T("TEXT"),
		_T(""));
}

void CTTranslatorApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileInt(
		_T("Config"),
		_T("LANG_FROM"),
		((CTTranslatorDlg *) m_pMainWnd)->m_nFROM);

	WriteProfileInt(
		_T("Config"),
		_T("LANG_TO"),
		((CTTranslatorDlg *) m_pMainWnd)->m_nTO);

	WriteProfileString(
		_T("Config"),
		_T("SOURCE"),
		LPCSTR(((CTTranslatorDlg *) m_pMainWnd)->m_strSOURCE));

	WriteProfileString(
		_T("Config"),
		_T("TARGET"),
		LPCSTR(((CTTranslatorDlg *) m_pMainWnd)->m_strTARGET));

	WriteProfileString(
		_T("Config"),
		_T("TEXT"),
		LPCSTR(((CTTranslatorDlg *) m_pMainWnd)->m_strTEXT));
}
