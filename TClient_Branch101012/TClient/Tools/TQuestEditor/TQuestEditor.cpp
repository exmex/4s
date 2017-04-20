// TQuestEditor.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TQuestEditor.h"

#include "MainFrm.h"
#include "TQuestEditorDoc.h"
#include "TQuestEditorView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorApp

BEGIN_MESSAGE_MAP(CTQuestEditorApp, CWinApp)
	//{{AFX_MSG_MAP(CTQuestEditorApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorApp construction

CTQuestEditorApp::CTQuestEditorApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CTQuestEditorApp object

CTQuestEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorApp initialization

BOOL CTQuestEditorApp::InitInstance()
{
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)
	// Register document templates

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CTQuestEditorDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CTQuestEditorView));
	AddDocTemplate(pDocTemplate);
	m_popup.LoadMenu(IDR_POPUP);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	if( __argc > 1 )
		m_nCmdShow = SW_HIDE;

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	if( __argc > 1 )
	{
		m_pMainWnd->ShowWindow(SW_HIDE);
		m_pMainWnd->UpdateWindow();

		CTQuestEditorDoc *pDOC = (CTQuestEditorDoc *) ((CMainFrame *) m_pMainWnd)->GetActiveDocument();
		if( pDOC && __argc > 3 )
		{
			CString strCON;

			strCON.Format( _T("ODBC;UID=%s;PWD=%s"), __argv[1], __argv[2]);
			if(pDOC->m_db.Open( _T("TQUEST"), FALSE, FALSE, LPCSTR(strCON)))
			{
				pDOC->LoadDataProc();
				pDOC->OnSaveDocument(__argv[3]);
			}
		}
		pDocTemplate->CloseAllDocuments(TRUE);

		return FALSE;
	}
	else
		m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

// App command to run the dialog
void CTQuestEditorApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorApp message handlers


int CTQuestEditorApp::ExitInstance() 
{
	m_popup.DestroyMenu();
	return CWinApp::ExitInstance();
}
