// Game Studio.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "Game StudioDoc.h"
#include "Game StudioView.h"

#include "AnimationFrame.h"
#include "AnimationView.h"

#include "TextureFrame.h"
#include "TextureView.h"

#include "OBJTypeFrame.h"
#include "OBJTypeView.h"

#include "EXTMAPFrame.h"
#include "EXTMAPView.h"

#include "BSPMAPFrame.h"
#include "BSPMAPView.h"

#include "ImageFrame.h"
#include "ImageView.h"

#include "MediaFrame.h"
#include "MediaView.h"

#include "FRAMEFrame.h"
#include "FRAMEView.h"

#include "COMPFrame.h"
#include "COMPView.h"

#include "MeshFrame.h"
#include "MeshView.h"

#include "OBJFrame.h"
#include "OBJView.h"

#include "SFXFrame.h"
#include "SFXView.h"
#include ".\game studio.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CGameStudioApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
END_MESSAGE_MAP()

static UINT basic_command[] =
{
	ID_FILE_NEW,
	ID_FILE_OPEN,
	ID_FILE_CLOSE,
	ID_FILE_SAVE,
	ID_FILE_SAVE_AS,
	ID_EDIT_UNDO,
	ID_EDIT_REDO,
	ID_EDITMENU,
	ID_EDITFONT,
	ID_EXP_GEO,
	ID_EXP_OBJ,
	ID_IMP_GEO,
	ID_IMP_OBJ,
	ID_EXT_BUILDTEXTURESHADOW,
	ID_MAP_ENABLEUNIT,
	ID_MAP_EDITDETAIL,
	ID_EXP_BSP,
	ID_IMP_BSP,
	ID_EXP_BSPOBJ,
	ID_IMP_BSPOBJ,
	ID_APP_EXIT,
	ID_VIEW_WORKSPACE,
	ID_VIEW_PREVIEW,
	ID_HELP_FINDER,
	ID_APP_ABOUT,
	ID_ITEM_NEWPROJECT,
	ID_CANCEL,
	ID_ITEM_NEWFOLDER,
	ID_ITEM_ADD,
	ID_ITEM_DELETE,
	ID_ITEM_UPDATE, 0
};


CGameStudioApp::CGameStudioApp()
{
	EnableHtmlHelp();
	m_pGDI = NULL;
}

CGameStudioApp theApp;


BOOL CGameStudioApp::InitInstance()
{
#ifdef USE_CONSOLE
	CTConsole* pConsole = CTConsole::GetInstance();
	pConsole->Create("Output", true);
#endif
	GdiplusStartupInput vGDIInput;

	CreateMutex( NULL, FALSE, _T("GAME_STUDIO"));
	if( GetLastError() != ERROR_ALREADY_EXISTS )
		CItemBase::m_bCanTempPath = TRUE;

	InitCommonControls();
	AfxOleInit();
	CWinApp::InitInstance();

	if( GdiplusStartup( &m_pGDI, &vGDIInput, NULL) != Ok )
		return FALSE;

	SetRegistryKey(_T("Tachyon Engine"));
	LoadStdProfileSettings();

	m_pTextureTemplate = new CMultiDocTemplate(
		IDR_GameStudioTYPE,
		RUNTIME_CLASS(CTextureDoc),
		RUNTIME_CLASS(CTextureFrame),
		RUNTIME_CLASS(CTextureView));

	m_pImageTemplate = new CMultiDocTemplate(
		IDR_GameStudioTYPE,
		RUNTIME_CLASS(CImageDoc),
		RUNTIME_CLASS(CImageFrame),
		RUNTIME_CLASS(CImageView));

	m_pMediaTemplate = new CMultiDocTemplate(
		IDR_GameStudioTYPE,
		RUNTIME_CLASS(CMediaDoc),
		RUNTIME_CLASS(CMediaFrame),
		RUNTIME_CLASS(CMediaView));

	m_pFRAMETemplate = new CMultiDocTemplate(
		IDR_GameStudioTYPE,
		RUNTIME_CLASS(CFRAMEDoc),
		RUNTIME_CLASS(CFRAMEFrame),
		RUNTIME_CLASS(CFRAMEView));

	m_pCOMPTemplate = new CMultiDocTemplate(
		IDR_GameStudioTYPE,
		RUNTIME_CLASS(CCOMPDoc),
		RUNTIME_CLASS(CCOMPFrame),
		RUNTIME_CLASS(CCOMPView));

	m_pAnimationTemplate = new CMultiDocTemplate(
		IDR_GRIDTYPE,
		RUNTIME_CLASS(CAnimationDoc),
		RUNTIME_CLASS(CAnimationFrame),
		RUNTIME_CLASS(CAnimationView));

	m_pMeshTemplate = new CMultiDocTemplate(
		IDR_GRIDTYPE,
		RUNTIME_CLASS(CMeshDoc),
		RUNTIME_CLASS(CMeshFrame),
		RUNTIME_CLASS(CMeshView));

	m_pOBJTypeTemplate = new CMultiDocTemplate(
		IDR_GameStudioTYPE,
		RUNTIME_CLASS(COBJTypeDoc),
		RUNTIME_CLASS(COBJTypeFrame),
		RUNTIME_CLASS(COBJTypeView));

	m_pOBJTemplate = new CMultiDocTemplate(
		IDR_OBJECT,
		RUNTIME_CLASS(COBJDoc),
		RUNTIME_CLASS(COBJFrame),
		RUNTIME_CLASS(COBJView));

	m_pSFXTemplate = new CMultiDocTemplate(
		IDR_GRIDTYPE,
		RUNTIME_CLASS(CSFXDoc),
		RUNTIME_CLASS(CSFXFrame),
		RUNTIME_CLASS(CSFXView));

	m_pEXTMAPTemplate = new CMultiDocTemplate(
		IDR_MAPTYPE,
		RUNTIME_CLASS(CEXTMAPDoc),
		RUNTIME_CLASS(CEXTMAPFrame),
		RUNTIME_CLASS(CEXTMAPView));

	m_pBSPMAPTemplate = new CMultiDocTemplate(
		IDR_MAPTYPE,
		RUNTIME_CLASS(CBSPMAPDoc),
		RUNTIME_CLASS(CBSPMAPFrame),
		RUNTIME_CLASS(CBSPMAPView));

	CMultiDocTemplate* pDocTemplate = new CMultiDocTemplate(
		IDR_GameStudioTYPE,
		RUNTIME_CLASS(CGameStudioDoc),
		NULL,
		RUNTIME_CLASS(CGameStudioView));
	AddDocTemplate(pDocTemplate);

	CTColorCtrl::m_nMsgColorChanged = WM_COLOR_SELECTED;
	CMainFrame *pMainFrame = new CMainFrame();
	if(!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;

	m_menuPopup.LoadMenu(IDR_POPUP);
	m_pMainWnd = pMainFrame;

	g_CmdManager->UpdateFromToolBar(
		m_pszProfileName,
		IDR_POPUP);

	g_CmdManager->UpdateFromToolBar(
		m_pszProfileName,
		IDR_OBJECT);

	g_CmdManager->SetBasicCommands(
		m_pszProfileName,
		basic_command);

	if(CItemBase::m_strTempPath.IsEmpty())
		if(!CItemBase::m_bCanTempPath)
		{
			AfxMessageBox( IDS_ERR_INIT_TEMPPATH, IDOK);
			return FALSE;
		}
		else
			pMainFrame->OnFileSetTemppath();

	if(CItemBase::m_bCanTempPath)
		ClearFolder(CItemBase::m_strTempPath);

	m_pMainWnd->DragAcceptFiles();
	EnableShellOpen();

	RegisterShellFileTypes(TRUE);
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	if(!ProcessShellCommand(cmdInfo))
		return FALSE;

	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	return TRUE;
}


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange( CDataExchange* pDX);    // DDX/DDV support

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
void CGameStudioApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}


// CGameStudioApp message handlers

void CGameStudioApp::OnFileNew()
{
}

void CGameStudioApp::OnFileOpen()
{
	((CMainFrame *) m_pMainWnd)->OnFileOpen();
}

void CGameStudioApp::OnFileSave()
{
	((CMainFrame *) m_pMainWnd)->OnFileSave();
}

void CGameStudioApp::OnFileSaveAs()
{
	((CMainFrame *) m_pMainWnd)->OnFileSaveAs();
}

int CGameStudioApp::ExitInstance()
{
	if(m_pAnimationTemplate)
	{
		delete m_pAnimationTemplate;
		m_pAnimationTemplate = NULL;
	}

	if(m_pTextureTemplate)
	{
		delete m_pTextureTemplate;
		m_pTextureTemplate = NULL;
	}

	if(m_pOBJTypeTemplate)
	{
		delete m_pOBJTypeTemplate;
		m_pOBJTypeTemplate = NULL;
	}

	if(m_pImageTemplate)
	{
		delete m_pImageTemplate;
		m_pImageTemplate = NULL;
	}

	if(m_pMediaTemplate)
	{
		delete m_pMediaTemplate;
		m_pMediaTemplate = NULL;
	}

	if(m_pFRAMETemplate)
	{
		delete m_pFRAMETemplate;
		m_pFRAMETemplate = NULL;
	}

	if(m_pCOMPTemplate)
	{
		delete m_pCOMPTemplate;
		m_pCOMPTemplate = NULL;
	}

	if(m_pMeshTemplate)
	{
		delete m_pMeshTemplate;
		m_pMeshTemplate = NULL;
	}

	if(m_pOBJTemplate)
	{
		delete m_pOBJTemplate;
		m_pOBJTemplate = NULL;
	}

	if(m_pSFXTemplate)
	{
		delete m_pSFXTemplate;
		m_pSFXTemplate = NULL;
	}

	if(m_pEXTMAPTemplate)
	{
		delete m_pEXTMAPTemplate;
		m_pEXTMAPTemplate = NULL;
	}

	if(m_pBSPMAPTemplate)
	{
		delete m_pBSPMAPTemplate;
		m_pBSPMAPTemplate = NULL;
	}

	GdiplusShutdown(m_pGDI);
	SaveStdProfileSettings();

	return CWinApp::ExitInstance();
}

BOOL CGameStudioApp::SaveAllModified()
{
	CString strFORMAT;
	CString strTITLE;
	CString strMSG;

	strFORMAT.LoadString(IDS_SAVENEXIT);
	strTITLE.LoadString(IDR_MAINFRAME);
	strMSG.Format( strFORMAT, ((CMainFrame *) m_pMainWnd)->GetFileTitle());

	while(!CTachyonDoc::m_vDOC.empty())
		CTachyonDoc::m_vDOC.back()->OnCloseDocument();

	switch(m_pMainWnd->MessageBox( strMSG, strTITLE, MB_YESNOCANCEL))
	{
	case IDYES		: m_pMainWnd->SendMessage( WM_COMMAND, ID_FILE_SAVE);
	case IDNO		: return TRUE;
	case IDCANCEL	: return FALSE;
	}

	return FALSE;
}

int CGameStudioApp::Run()
{
	_AFX_THREAD_STATE* pState = AfxGetThreadState();

	LONG lIdleCount = 0;
	BOOL bIdle = TRUE;
	MSG msg;

	while( m_pMainWnd && TRUE )
	{
		while( bIdle && !::PeekMessage( &msg, NULL, NULL, NULL, PM_NOREMOVE))
			if(!OnIdle(lIdleCount++))
				bIdle = FALSE;

		BOOL bResult = CPlayCtrl::m_vPlayCtrl.empty() ? GetMessage(
			&msg,
			NULL,
			0U, 0U) : PeekMessage(
			&msg,
			NULL,
			0U, 0U,
			PM_REMOVE);

		if(bResult)
		{
			if( WM_QUIT == msg.message )
				break;

			if(!AfxPreTranslateMessage(&msg))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			if(IsIdleMessage(&msg))
			{
				lIdleCount = 0;
				bIdle = TRUE;
			}
		}
		else
			CPlayCtrl::DrawProc();
	}

	return ExitInstance();
}

void CGameStudioApp::ClearFolder( CString strPATH)
{
	CFileFind finder;
	CString strFind;

	strPATH.TrimRight('\\');
	strFind.Format(
		_T("%s\\*"),
		LPCSTR(strPATH));

	BOOL bFind = finder.FindFile(strFind);
	while(bFind)
	{
		bFind = finder.FindNextFile();

		if(finder.IsDots())
			continue;

		if(finder.IsDirectory())
		{
			ClearFolder(finder.GetFilePath());
			RemoveDirectory(LPCSTR(finder.GetFilePath()));

			continue;
		}

		DeleteFile(LPCSTR(finder.GetFilePath()));
	}
}

void CGameStudioApp::LoadStdProfileSettings()
{
	CWinApp::LoadStdProfileSettings();

	CItemBase::m_strTempPath = GetProfileString(
		_T("Settings"),
		_T("Temp Path"),
		_T(""));
}

void CGameStudioApp::SaveStdProfileSettings()
{
	CWinApp::SaveStdProfileSettings();

	WriteProfileString(
		_T("Settings"),
		_T("Temp Path"),
		LPCSTR(CItemBase::m_strTempPath));
}
