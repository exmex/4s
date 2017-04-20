// TPathTime.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TPathTime.h"
#include "MainFrm.h"

#include "TPathTimeDoc.h"
#include "TPathTimeView.h"
#include ".\tpathtime.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTPathTimeApp

BEGIN_MESSAGE_MAP(CTPathTimeApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CTPathTimeApp construction

CTPathTimeApp::CTPathTimeApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CTPathTimeApp object

CTPathTimeApp theApp;

// CTPathTimeApp initialization

BOOL CTPathTimeApp::InitInstance()
{
	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Tachyon Engine"));
	LoadStdProfileSettings();
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTPathTimeDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTPathTimeView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CTPathTimeApp::ExitInstance()
{
	SaveStdProfileSettings();
	return CWinApp::ExitInstance();
}

void CTPathTimeApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	m_strPATH = GetProfileString(
		_T("Config"),
		_T("Image Folder"),
		_T(""));

	m_nSNAP = GetProfileInt(
		_T("Config"),
		_T("Snap Size"),
		TDEF_SNAP_SIZE);

	m_vTJOINT.cx = GetProfileInt(
		_T("Config"),
		_T("Joint Width"),
		TDEF_JOINT_WIDTH);

	m_vTJOINT.cy = GetProfileInt(
		_T("Config"),
		_T("Joint Height"),
		TDEF_JOINT_HEIGHT);

	m_fTIME = FLOAT(GetProfileInt(
		_T("Config"),
		_T("Time Per Meter"),
		TDEF_TIME)) / 100.0f;
}

void CTPathTimeApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileString(
		_T("Config"),
		_T("Image Folder"),
		m_strPATH);

	WriteProfileInt(
		_T("Config"),
		_T("Snap Size"),
		m_nSNAP);

	WriteProfileInt(
		_T("Config"),
		_T("Joint Width"),
		m_vTJOINT.cx);

	WriteProfileInt(
		_T("Config"),
		_T("Joint Height"),
		m_vTJOINT.cy);

	WriteProfileInt(
		_T("Config"),
		_T("Time Per Meter"),
		INT(m_fTIME * 100.0f + 0.009f));
}


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

// App command to run the dialog
void CTPathTimeApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}
