// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"

#include "TextureView.h"
#include "EXTMAPView.h"
#include "BSPMAPView.h"
#include "GuideView.h"
#include "GSDialogBar.h"
#include "MAPOBJBar.h"
#include "OBJBar.h"
#include "PlayCtrl.h"
#include "TCMLTag.h"
#include "EditMenuDlg.h"
#include "EditFontDlg.h"
#include "EditFovDlg.h"
#include "SetIDSeedDlg.h"
#include "ItemFinder.h"
#include "ItemScreenShot.h"
#include "direct.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define INIT_BAR_SIZE_X			180
#define INIT_BAR_SIZE_Y			165
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	// Global help commands
	ON_COMMAND(ID_HELP_FINDER, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND(ID_HELP, CMDIFrameWnd::OnHelp)
	ON_COMMAND(ID_CONTEXT_HELP, CMDIFrameWnd::OnContextHelp)
	ON_COMMAND(ID_DEFAULT_HELP, CMDIFrameWnd::OnHelpFinder)
	ON_COMMAND_EX(ID_VIEW_MENUBAR, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_MENUBAR, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_WORKSPACE, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateControlBarMenu)
	ON_COMMAND_EX(ID_VIEW_PREVIEW, OnBarCheck)
	ON_UPDATE_COMMAND_UI(ID_VIEW_PREVIEW, OnUpdateControlBarMenu)
	ON_WM_DESTROY()
	ON_COMMAND(ID_FILE_MERGE, OnFileMerge)
	ON_COMMAND(ID_BUILD, OnBuild)
	ON_COMMAND(ID_EDITMENU, OnEditmenu)
	ON_COMMAND(ID_EDITFONT, OnEditfont)
	ON_COMMAND(ID_EDIT_FOV, OnEditFov)
	ON_COMMAND(ID_NOTUSE_VB, OnNotuseVb)
	ON_UPDATE_COMMAND_UI(ID_NOTUSE_VB, OnUpdateNotuseVb)
	ON_COMMAND(ID_RESET_VB, OnResetVb)
	ON_UPDATE_COMMAND_UI(ID_RESET_VB, OnUpdateResetVb)
	ON_COMMAND(ID_MENU_TEXHI, OnMenuTexhi)
	ON_UPDATE_COMMAND_UI(ID_MENU_TEXHI, OnUpdateMenuTexhi)
	ON_COMMAND(ID_MENU_TEXLOW, OnMenuTexlow)
	ON_UPDATE_COMMAND_UI(ID_MENU_TEXLOW, OnUpdateMenuTexlow)
	ON_COMMAND(ID_MENU_TEXMID, OnMenuTexmid)
	ON_UPDATE_COMMAND_UI(ID_MENU_TEXMID, OnUpdateMenuTexmid)
	ON_COMMAND(ID_MENU_AUTOLOD, OnMenuAutolod)
	ON_UPDATE_COMMAND_UI(ID_MENU_AUTOLOD, OnUpdateMenuAutolod)
	ON_COMMAND(ID_MENU_LOCEXP, OnMenuLocExp)
	ON_UPDATE_COMMAND_UI(ID_MENU_LOCEXP, OnUpdateMenuLocExp)
	ON_COMMAND(ID_MENU_LOCIMP, OnMenuLocImp)
	ON_UPDATE_COMMAND_UI(ID_MENU_LOCIMP, OnUpdateMenuLocImp)
	ON_COMMAND(ID_MENU_LISTUPDATE, OnMenuListUpdate)
	ON_COMMAND(ID_FINDER, OnItemFinder)
	ON_COMMAND(ID_ITEM_SCREENSHOT, OnItemScreenShot)
	ON_COMMAND(ID_MENU_UPDATE_SIZE, OnMenuUpdatesize)
	ON_UPDATE_COMMAND_UI(ID_MENU_UPDATE_SIZE, OnUpdateMenuUpdatesize)
	ON_COMMAND(ID_FILE_SET_TEMPPATH, OnFileSetTemppath)
	ON_UPDATE_COMMAND_UI(ID_FILE_SET_TEMPPATH, OnUpdateFileSetTemppath)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_SEPARATOR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

LPDIRECT3DSWAPCHAIN9	CMainFrame::m_pSWAPCHAIN	= NULL;
CTachyonMedia			CMainFrame::m_TachyonMedia;
CD3DDevice				CMainFrame::m_3DDevice;
BYTE					CMainFrame::m_bRender		= TRUE;
FLOAT					CMainFrame::m_fFOV			= 1.0f;
CItemFinder*			CMainFrame::m_pFinderDlg	= NULL;
CItemScreenShot*		CMainFrame::m_pScreenShotDlg = NULL;
CString					CMainFrame::m_strFileName;

CString g_strFILEHEADER("GAME STUDIO FILE");



// CMainFrame construction/destruction

CMainFrame::CMainFrame()
//: m_nItemIDSeedValue(0)
{
	m_dwToolBarID = IDR_MAINFRAME;
	m_strFileName = _T("");

	m_pDocAnimation = NULL;
	m_pDocTexture = NULL;
	m_pDocObject = NULL;
	m_pDocImage = NULL;
	m_pDocMedia = NULL;
	m_pDocFrame = NULL;
	m_pDocComp = NULL;
	m_pDocMesh = NULL;
	m_pDocSFX = NULL;
	m_pDocMAP = NULL;

	m_pFinderDlg = NULL;
	m_pScreenShotDlg = NULL;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
	CWinApp *pApp = ::AfxGetApp();
	InitDevice();

	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	g_CmdManager->ProfileSetup(
		pApp->m_pszProfileName,
		GetSafeHwnd());

	g_CmdManager->UpdateFromMenu(
		pApp->m_pszProfileName,
		IDR_MAINFRAME);

	m_wndMenuBar.SetMdiWindowPopupName(_T("Window"));
	if(!m_wndMenuBar.Create( _T("Menu Bar"), this, ID_VIEW_MENUBAR))
		return -1;

	if(!m_wndWorkSpace.Create( _T("Work Space"), this, ID_VIEW_WORKSPACE))
		return -1;

	if(!m_wndPreview.Create( _T("Preview"), this, ID_VIEW_PREVIEW))
		return -1;

	if( !m_wndToolBar.Create( _T("ToolBar"), this, AFX_IDW_TOOLBAR) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
		return -1;

	if( !m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(
		indicators,
		sizeof(indicators) / sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	for( int i=0; i<3; i++)
	{
		m_wndStatusBar.SetPaneInfo(
			i + 1,
			ID_SEPARATOR,
			SBPS_NORMAL,
			80);
	}

	if(!m_wndPreviewID.Create(
		IDD_DIALOG_PREVIEW,
		&m_wndPreview))
		return -1;

	m_listWorkspaceTabImages.Create(
		IDB_WORKSPACE_TAB,
		17, 1,
		RGB( 255, 255, 255));

	m_wndWorkSpace.ModifyTabStyle( TCS_SINGLELINE, TCS_MULTILINE);
	m_wndWorkSpace.SetTabImageList(&m_listWorkspaceTabImages);

	m_wndWorkSpace.EnableDocking(CBRS_ALIGN_ANY);
	m_wndPreview.EnableDocking(CBRS_ALIGN_ANY);
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);

	if(!CTControlBar::FrameEnableDocking(this))
		return -1;

	CGameStudioDoc::m_pPreview = &m_wndPreviewID;
	m_pDocAnimation = new CGameStudioDoc();
	m_pDocTexture = new CGameStudioDoc();
	m_pDocObject = new CGameStudioDoc();
	m_pDocImage = new CGameStudioDoc();
	m_pDocMedia = new CGameStudioDoc();
	m_pDocMesh = new CGameStudioDoc();
	m_pDocSFX = new CGameStudioDoc();
	m_pDocMAP = new CGameStudioDoc();
	m_pDocFrame = new CGameStudioDoc();
	m_pDocComp = new CGameStudioDoc();

	m_pDocAnimation->m_bType = ITEM_ANIMATION;
	m_pDocTexture->m_bType = ITEM_TEXTURE;
	m_pDocObject->m_bType = ITEM_OBJECT;
	m_pDocImage->m_bType = ITEM_IMAGE;
	m_pDocMedia->m_bType = ITEM_MEDIA;
	m_pDocFrame->m_bType = ITEM_FRAME;
	m_pDocComp->m_bType = ITEM_COMP;
	m_pDocMesh->m_bType = ITEM_MESH;
	m_pDocSFX->m_bType = ITEM_SFX;
	m_pDocMAP->m_bType = ITEM_EXTMAP;

	m_wndWorkSpace.AddView( _T("Texture"), RUNTIME_CLASS(CGameStudioView), m_pDocTexture);
	m_wndWorkSpace.AddView( _T("Image"), RUNTIME_CLASS(CGameStudioView), m_pDocImage);
	m_wndWorkSpace.AddView( _T("Media"), RUNTIME_CLASS(CGameStudioView), m_pDocMedia);
	m_wndWorkSpace.AddView( _T("Animation"), RUNTIME_CLASS(CGameStudioView), m_pDocAnimation);
	m_wndWorkSpace.AddView( _T("Mesh"), RUNTIME_CLASS(CGameStudioView), m_pDocMesh);
	m_wndWorkSpace.AddView( _T("Object"), RUNTIME_CLASS(CGameStudioView), m_pDocObject);
	m_wndWorkSpace.AddView( _T("SFX"), RUNTIME_CLASS(CGameStudioView), m_pDocSFX);
	m_wndWorkSpace.AddView( _T("Map"), RUNTIME_CLASS(CGameStudioView), m_pDocMAP);
	m_wndWorkSpace.AddView( _T("Control"), RUNTIME_CLASS(CGameStudioView), m_pDocComp);
	m_wndWorkSpace.AddView( _T("Frame"), RUNTIME_CLASS(CGameStudioView), m_pDocFrame);
	m_wndWorkSpace.SetActiveView(0);

	DockControlBar(&m_wndMenuBar);
	DockControlBar(&m_wndToolBar);
	DockControlBar( &m_wndWorkSpace, AFX_IDW_DOCKBAR_LEFT);
	m_wndWorkSpace.DockControlBar( &m_wndPreview, TRUE);

	m_wndWorkSpace.SetInitDesiredSizeVertical(CSize( INIT_BAR_SIZE_X, -1));
	m_wndPreview.SetInitDesiredSizeVertical(CSize(
		INIT_BAR_SIZE_X,
		INIT_BAR_SIZE_Y));

	LoadTextureFromResource( IDR_TEXTURE_LIGHTTARGET, &CMAPView::m_pLTARGET);
	LoadTextureFromResource( IDR_TEXTURE_LIGHT, &CMAPView::m_pLIGHT);
	LoadTextureFromResource( IDR_TEXTURE_SFX, &CMAPSFXBar::m_pTEX);
	LoadTextureFromResource( IDR_TEXTURE_SND, &CMAPSNDBar::m_pTEX);
	LoadMeshFromResource( IDR_MESH_BOX, &CMAPSFXBar::m_MESH);
	LoadMeshFromResource( IDR_MESH_BOX, &CMAPSNDBar::m_MESH);

	CGameStudioView::InitDragResource();
	CTextureView::InitTextureMesh();
	CEXTMAPView::InitSPRAY();
	CMAPOBJBar::InitOBJBAR();
	CMAPSFXBar::InitSFXBAR();
	CMAPSNDBar::InitSNDBAR();
	COBJBar::InitOBJBAR();
	CMAPView::InitRECT();
	CGSDialogBar::InitGSDialogBar();
	CPlayCtrl::InitPlayCtrl();
	RecalcLayout();

	CGuideView::m_wndToolTip.Create(this);
	CGuideView::m_wndToolTip.ShowWindow(SW_HIDE);

	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_3DDevice.m_pDevice->SetMaterial(&mtrl);

	m_3DDevice.m_pDevice->SetSoftwareVertexProcessing(FALSE);
	m_3DDevice.m_pDevice->SetVertexShader(NULL);
	m_TachyonMedia.InitMEDIA(GetSafeHwnd());

	CTachyonHUGEMAP::m_nIndexCount = 80000;
	CTachyonHUGEMAP::m_nLODBound = 32;

	CTachyonObject::m_pMedia = &m_TachyonMedia;
	ClearStaticMap();

	return 0;
}

void CMainFrame::RestoreDevice()
{
	LPDIRECT3DSURFACE9 pBACK = NULL;

	if(m_pSWAPCHAIN)
	{
		m_pSWAPCHAIN->Release();
		m_pSWAPCHAIN = NULL;
	}

	CItemBase::DeleteForReset(ITEM_EXTMAP);
	CTachyonDoc::DeleteForReset();

	while( m_3DDevice.m_pD3D && m_3DDevice.m_pDevice )
		if(m_3DDevice.Reset())
			break;
	D3DMATERIAL9 mtrl;

	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_3DDevice.m_pDevice->SetMaterial(&mtrl);

	m_3DDevice.m_pDevice->SetSoftwareVertexProcessing(FALSE);
	m_3DDevice.m_pDevice->SetVertexShader(NULL);

	m_3DDevice.m_pDevice->CreateAdditionalSwapChain(
		&m_3DDevice.m_vPRESENT,
		&m_pSWAPCHAIN);

	m_pSWAPCHAIN->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBACK);
	m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	pBACK->Release();

	CItemBase::Reset(ITEM_EXTMAP);
	CTachyonDoc::Reset();
}

void CMainFrame::ReleaseDevice()
{
	if(m_pSWAPCHAIN)
	{
		m_pSWAPCHAIN->Release();
		m_pSWAPCHAIN = NULL;
	}

	CD3DLight::SetDevice(NULL);
	CTBSPNode::ReleaseCommon();
	m_3DDevice.ReleaseDevice();
}

void CMainFrame::InitDevice()
{
	static DWORD vVS[VS_COUNT] = {
		IDR_VS_WMESH,		// VS_WMESHVERTEX
		IDR_VS_MESH,		// VS_MESHVERTEX
		IDR_VS_LVERTEX};	// VS_LVERTEX

	static WORD vConstantVS[VC_COUNT] = {
		0,		// VC_COMMON
		397,	// VC_WORLD
		7,		// VC_PROJ
		3,		// VC_TEXTRAN0
		5,		// VC_TEXTRAN1
		139,	// VC_MTRLAMBIENT
		140,	// VC_MTRLDIFFUSE
		2,		// VC_AMBIENT
		1,		// VC_CAMPOS
		0,		// VC_LIGHTCOUNT
		141,	// VC_LIGHTAMBIENT
		269,	// VC_LIGHTDIFFUSE
		11,		// VC_LIGHTDIR
		0};		// VC_SKINNING

	static DWORD vPS[PS_COUNT] = {
		IDR_PS_STAGE1,
		IDR_PS_MODULATE,
		IDR_PS_MODULATE2X,
		IDR_PS_MODULATE4X,
		IDR_PS_ADD,
		IDR_PS_ADDSIGNED,
		IDR_PS_ADDSIGNED2X,
		IDR_PS_SUBTRACT,
		IDR_PS_ADDSMOOTH,
		IDR_PS_BLENDDIFFUSEALPHA,
		IDR_PS_BLENDTEXTUREALPHA,
		IDR_PS_BLENDFACTORALPHA,
		IDR_PS_BLENDTEXTUREALPHAPM,
		IDR_PS_BLENDCURRENTALPHA,
		IDR_PS_MODULATEALPHA_ADDCOLOR,
		IDR_PS_MODULATECOLOR_ADDALPHA,
		IDR_PS_MODULATEINVALPHA_ADDCOLOR,
		IDR_PS_MODULATEINVCOLOR_ADDALPHA,
		IDR_PS_DOTPRODUCT3,
		IDR_PS_MULTIPLYADD,
		IDR_PS_LERP,
		IDR_PS_SHADER,
		IDR_PS_DETAILMAP,
		IDR_PS_MAP};

	static WORD vConstantPS[PC_COUNT] = {
		2,		// PC_COMMON
		1,		// PC_ARG
		0};		// PC_TFACTOR

	CWnd *pDeskTop = GetDesktopWindow();
	CRect rcScreen;

	LPDIRECT3DSURFACE9 pBACK = NULL;

	pDeskTop->GetWindowRect(&rcScreen);
	ReleaseDevice();

	m_3DDevice.m_option.m_dwBehavior |= D3DCREATE_MULTITHREADED;
	m_3DDevice.m_option.m_dwScreenX = rcScreen.Width();
	m_3DDevice.m_option.m_dwScreenY = rcScreen.Height();
	m_3DDevice.m_option.m_bWindowedMode = TRUE;
	CTachyonRes::m_pDEVICE = &m_3DDevice;

	memcpy( m_3DDevice.m_vConstantVS, vConstantVS, VC_COUNT * sizeof(WORD));
	memcpy( m_3DDevice.m_dwVertexShader, vVS, VS_COUNT * sizeof(DWORD));

	memcpy( m_3DDevice.m_vConstantPS, vConstantPS, PC_COUNT * sizeof(WORD));
	memcpy( m_3DDevice.m_dwPixelShader, vPS, PS_COUNT * sizeof(DWORD));

	m_3DDevice.m_strResourceType = "SHADER";
	m_3DDevice.InitDevices(pDeskTop);

	m_3DDevice.m_pDevice->CreateAdditionalSwapChain(
		&m_3DDevice.m_vPRESENT,
		&m_pSWAPCHAIN);

	m_pSWAPCHAIN->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBACK);
	m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	pBACK->Release();

	CD3DLight::SetDevice(m_3DDevice.m_pDevice);
	CTBSPNode::InitCommon(&m_3DDevice);
	CTachyonSFX::InitCommon();
	CSFX::InitTEXVB();
}

BOOL CMainFrame::PreCreateWindow( CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	cs.style = WS_OVERLAPPED|
		WS_CAPTION|
		FWS_ADDTOTITLE|
		WS_THICKFRAME|
		WS_MINIMIZEBOX|
		WS_MAXIMIZEBOX|
		WS_MAXIMIZE|
		WS_SYSMENU;

	cs.cx = GetSystemMetrics(SM_CXSCREEN);
	cs.cy = GetSystemMetrics(SM_CYSCREEN);


	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers

void CMainFrame::ClearStaticMap()
{
	MAPIDINDEX::iterator itINDEX;
	MAPGSFONT::iterator itFONT;

	for( itINDEX = CItemBase::m_mapLocalID.begin(); itINDEX != CItemBase::m_mapLocalID.end(); itINDEX++)
	{
		VECTORIDINDEX *pINDEX = (*itINDEX).second;

		while(!pINDEX->empty())
		{
			delete pINDEX->back();
			pINDEX->pop_back();
		}

		delete pINDEX;
	}

	for( itFONT = CItemBase::m_mapFont.begin(); itFONT != CItemBase::m_mapFont.end(); itFONT++)
		delete (*itFONT).second;

	CItemBase::m_mapGlobalID.clear();
	CItemBase::m_mapLocalID.clear();
	CItemBase::m_mapMenuID.clear();
	CItemBase::m_mapFont.clear();

	CItemBase::m_mapAnimationItem.clear();
	CItemBase::m_mapOBJTypeItem.clear();
	CItemBase::m_mapTextureItem.clear();
	CItemBase::m_mapImageItem.clear();
	CItemBase::m_mapMediaItem.clear();
	CItemBase::m_mapFrameItem.clear();
	CItemBase::m_mapCompItem.clear();
	CItemBase::m_mapMeshItem.clear();
	CItemBase::m_mapMAPItem.clear();
	CItemBase::m_mapOBJItem.clear();
	CItemBase::m_mapSFXItem.clear();

	CTachyonDoc::m_vDOC.clear();
}

void CMainFrame::UpdateTitle()
{
	CString strTitle;

	strTitle.LoadString(IDR_MAINFRAME);
	if(!m_strFileName.IsEmpty())
		m_strTitle.Format( "%s - %s", strTitle, m_strFileName);
	else
		m_strTitle = strTitle;

	SetWindowText(m_strTitle);
}

CDocument* CMainFrame::GetActiveDocument()
{
	ASSERT_VALID(this);
	CView* pView = GetActiveView();

	if( pView != NULL &&
		!pView->IsKindOf(RUNTIME_CLASS(CGameStudioView)))
		return pView->GetDocument();

	return NULL;
}

void CMainFrame::OnDestroy()
{
	while(!CTachyonDoc::m_vDOC.empty())
		CTachyonDoc::m_vDOC.back()->OnCloseDocument();

	m_wndPreviewID.SetItem(NULL);
	CItemBase::m_bRegulateAll = FALSE;
	CSFX::ReleaseTEXVB();
	CTachyonSFX::ReleaseCommon();
	CMDIFrameWnd::OnDestroy();

	CMAPView::m_pLTARGET->Release();
	CMAPView::m_pLIGHT->Release();
	CMAPSFXBar::m_pTEX->Release();
	CMAPSNDBar::m_pTEX->Release();
	CMAPSFXBar::m_MESH.Release();
	CMAPSNDBar::m_MESH.Release();

	m_listWorkspaceTabImages.DeleteImageList();
	CGameStudioView::InitDragResource();
	CGSDialogBar::ReleaseGSDialogBar();
	CTextureView::ReleaseTextureMesh();
	CEXTMAPView::ReleaseSPRAY();
	CMAPOBJBar::ReleaseOBJBAR();
	CMAPSFXBar::ReleaseSFXBAR();
	CMAPSNDBar::ReleaseSNDBAR();
	COBJBar::ReleaseOBJBAR();
	CMAPView::ReleaseRECT();
	CPlayCtrl::ReleasePlayCtrl();
	ClearStaticMap();
	ReleaseDevice();
}

void CMainFrame::OnBuild()
{
	CTShell dlg;

	dlg.m_strTitle = _T("Output Folder");
	if( dlg.BrowseForFolder(this) == IDOK )
	{
		BUILDTHREADPARAM Param;
		CProgressDlg Progress;

		Progress.m_strCancel.LoadString(IDS_BUILD_CANCEL);
		Progress.m_strTitle.LoadString(IDS_BUILD);
		Progress.m_pThreadFunc = (AFX_THREADPROC) _Build;
		Progress.m_lParam = &Param;

		Param.m_dwVER = __T3D_SDK_VER;
		Param.m_pOwner = this;
		Param.m_pWnd = &Progress;
		Param.m_strPATH = dlg.m_strPath;

		CTachyonDoc::CloseAllBSP();
		Progress.DoModal();
	}
}

void CMainFrame::OnFileSaveAs()
{
	CFileDialog dlg(
		FALSE,
		_T("*.Gsf"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Game Studio Files (*.Gsf)|*.gsf|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CString strName = dlg.GetPathName();
		char szTempPath[_MAX_PATH];
		char szTempFile[_MAX_PATH];

		GetTempPath(
			_MAX_PATH,
			szTempPath);

		GetTempFileName(
			szTempPath,
			_T("_GSF"),
			0, szTempFile);

		CFile file( szTempFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		FILETHREADPARAM Param;
		CProgressDlg Progress;

		Progress.m_strCancel.LoadString(IDS_SAVEGSF_CANCEL);
		Progress.m_strTitle.LoadString(IDS_SAVE_GSF);
		Progress.m_pThreadFunc = (AFX_THREADPROC) _Save;
		Progress.m_lParam = &Param;

		Param.m_pWnd = &Progress;
		Param.m_pOwner = this;
		Param.m_pFile = &file;
		CItemBase::RegulateAll();

		if( Progress.DoModal() == IDOK )
		{
			m_strFileName = strName;
			UpdateTitle();
			file.Close();

			DeleteFile(LPCSTR(strName));
			rename( szTempFile, LPCSTR(strName));
		}
	}
}

void CMainFrame::OnFileSave()
{
	if(!m_strFileName.IsEmpty())
	{
		char szTempPath[_MAX_PATH];
		char szTempFile[_MAX_PATH];

		GetTempPath(
			_MAX_PATH,
			szTempPath);

		GetTempFileName(
			szTempPath,
			_T("_GSF"),
			0, szTempFile);

		CFile file( szTempFile, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
		FILETHREADPARAM Param;
		CProgressDlg Progress;

		Progress.m_strCancel.LoadString(IDS_SAVEGSF_CANCEL);
		Progress.m_strTitle.LoadString(IDS_SAVE_GSF);
		Progress.m_pThreadFunc = (AFX_THREADPROC) _Save;
		Progress.m_lParam = &Param;

		Param.m_pWnd = &Progress;
		Param.m_pOwner = this;
		Param.m_pFile = &file;
		CItemBase::RegulateAll();

		if( Progress.DoModal() == IDOK )
		{
			file.Close();

			DeleteFile(LPCSTR(m_strFileName));
			rename( szTempFile, LPCSTR(m_strFileName));
		}
	}
	else
		OnFileSaveAs();
}

void CMainFrame::OnFileNew()
{
	m_strFileName = _T("");
	ClearStaticMap();
	UpdateTitle();

	m_wndPreviewID.SetItem(NULL);
	m_pDocAnimation->Clear();
	m_pDocTexture->Clear();
	m_pDocObject->Clear();
	m_pDocImage->Clear();
	m_pDocMedia->Clear();
	m_pDocFrame->Clear();
	m_pDocComp->Clear();
	m_pDocMesh->Clear();
	m_pDocSFX->Clear();
	m_pDocMAP->Clear();

	CItemBase::m_dwAnimationID = 1;
	CItemBase::m_dwOBJTypeID = 1;
	CItemBase::m_dwTextureID = 1;
	CItemBase::m_dwImageID = 1;
	CItemBase::m_dwMediaID = 1;
	CItemBase::m_dwFrameID = 1;
	CItemBase::m_dwCompID = 1;
	CItemBase::m_dwMeshID = 1;
	CItemBase::m_dwMAPID = 1;
	CItemBase::m_dwOBJID = 1;
	CItemBase::m_dwSFXID = 1;
	CItemBase::m_dwLocalID = 1;
	CItemBase::m_bMerge = FALSE;

	RedrawWindow(
		NULL, NULL,
		RDW_INVALIDATE|
		RDW_UPDATENOW|
		RDW_ERASE|
		RDW_ALLCHILDREN);
}

void CMainFrame::OnFileOpen()
{
	CFileDialog dlg(
		TRUE,
		_T("*.Gsf"),
		NULL,
		OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
		_T("Game Studio Files (*.Gsf)|*.gsf|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CString strName = dlg.GetPathName();

		CItemBase::m_bMerge = FALSE;
		TRY
		{
			CFile file( strName, CFile::modeRead|CFile::typeBinary);
			FILETHREADPARAM Param;
			CProgressDlg Progress;

			Progress.m_strCancel.LoadString(IDS_LOADGSF_CANCEL);
			Progress.m_strTitle.LoadString(IDS_LOAD_GSF);
			Progress.m_pThreadFunc = (AFX_THREADPROC) _Load;
			Progress.m_lParam = &Param;

			Param.m_pWnd = &Progress;
			Param.m_pOwner = this;
			Param.m_pFile = &file;

			m_strFileName = _T("");
			ClearStaticMap();
			UpdateTitle();
			m_wndPreviewID.SetItem(NULL);

			if( Progress.DoModal() == IDOK )
			{
				m_strFileName = strName;
				UpdateTitle();
				CItemBase::RegulateAll();
			}
		}
		CATCH_ALL(e)
		{
		}
		END_CATCH_ALL

		RedrawWindow(
			NULL, NULL,
			RDW_INVALIDATE|
			RDW_UPDATENOW|
			RDW_ERASE|
			RDW_ALLCHILDREN);
	}
}

void CMainFrame::OnFileMerge()
{
	CFileDialog dlg(
		TRUE,
		_T("*.Gsf"),
		NULL,
		OFN_HIDEREADONLY|OFN_PATHMUSTEXIST,
		_T("Game Studio Files (*.Gsf)|*.gsf|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CString strName = dlg.GetPathName();

		CItemBase::m_bMerge = TRUE;
		TRY
		{
			CFile file( strName, CFile::modeRead|CFile::typeBinary);
			FILETHREADPARAM Param;
			CProgressDlg Progress;

			// 06_02_14 박상연 ID Seed 값 지정
			//-------------------------------------------------------
			CSetIDSeedDlg dlg;
			DWORD	dwSeed;
				
			dlg.m_dwID_Animation	= CItemBase::m_dwAnimationID;	
			dlg.m_dwID_ObjectType	= CItemBase::m_dwOBJTypeID;	
			dlg.m_dwID_Texture		= CItemBase::m_dwTextureID;	
			dlg.m_dwID_Image		= CItemBase::m_dwImageID;	
			dlg.m_dwID_Media		= CItemBase::m_dwMediaID;		
			dlg.m_dwID_Frame		= CItemBase::m_dwFrameID;		
			dlg.m_dwID_Component	= CItemBase::m_dwCompID;		
			dlg.m_dwID_Mesh			= CItemBase::m_dwMeshID;		
			dlg.m_dwID_Map			= CItemBase::m_dwMAPID; 		
			dlg.m_dwID_Object		= CItemBase::m_dwOBJID;		
			dlg.m_dwID_SFX			= CItemBase::m_dwSFXID; 		
			
			BOOL m_bCheck = FALSE;
			while(!m_bCheck)
			{		
				INT_PTR OCButton;
				OCButton = dlg.DoModal();

				if( OCButton == IDOK)
				{
					dwSeed = dlg.m_dwSeed;

			  		if( dwSeed < CItemBase::m_dwAnimationID ||
						dwSeed < CItemBase::m_dwOBJTypeID ||
						dwSeed < CItemBase::m_dwTextureID ||
						dwSeed < CItemBase::m_dwImageID ||
						dwSeed < CItemBase::m_dwMediaID ||	
						dwSeed < CItemBase::m_dwFrameID ||	
						dwSeed < CItemBase::m_dwCompID ||	
						dwSeed < CItemBase::m_dwMeshID ||	
						dwSeed < CItemBase::m_dwMAPID ||	
						dwSeed < CItemBase::m_dwOBJID ||		
						dwSeed < CItemBase::m_dwSFXID || 
						dwSeed < 0  )
					{
						MessageBox( "Input the little more large Seed Value" );
					}
					else
					{
						CItemBase::m_dwAnimationID	= dwSeed;
						CItemBase::m_dwOBJTypeID	= dwSeed;
						CItemBase::m_dwTextureID	= dwSeed;
						CItemBase::m_dwImageID		= dwSeed;
						CItemBase::m_dwMediaID		= dwSeed;
						CItemBase::m_dwFrameID		= dwSeed;
						CItemBase::m_dwCompID		= dwSeed;
						CItemBase::m_dwMeshID		= dwSeed;
						CItemBase::m_dwMAPID 		= dwSeed;
						CItemBase::m_dwOBJID 		= dwSeed;
						CItemBase::m_dwSFXID 		= dwSeed;
						m_bCheck = TRUE;
					}
				}
				else if( OCButton == IDCANCEL  )
				{
					CItemBase::m_bMerge = FALSE;
					return;
				}

				
			}
			//-------------------------------------------------------

			Progress.m_strCancel.LoadString(IDS_MERGEGSF_CANCEL);
			Progress.m_strTitle.LoadString(IDS_MERGE_GSF);
			Progress.m_pThreadFunc = (AFX_THREADPROC) _Merge;
			Progress.m_lParam = &Param;

			Param.m_pWnd = &Progress;
			Param.m_pOwner = this;
			Param.m_pFile = &file;
			m_wndPreviewID.SetItem(NULL);
			CTachyonDoc::CloseAllBSP();

			if( Progress.DoModal() == IDOK )
				CItemBase::RegulateAll();
		}
		CATCH_ALL(e)
		{
		}
		END_CATCH_ALL

		CItemBase::m_bMerge = FALSE;
		RedrawWindow(
			NULL, NULL,
			RDW_INVALIDATE|
			RDW_UPDATENOW|
			RDW_ERASE|
			RDW_ALLCHILDREN);
	}
}

void CMainFrame::_Build( LPBUILDTHREADPARAM lParam)
{
	lParam->m_pOwner->Build(lParam);
}

void CMainFrame::_Load( LPFILETHREADPARAM lParam)
{
	lParam->m_pOwner->Load(lParam);
}

void CMainFrame::_Merge( LPFILETHREADPARAM lParam)
{
	lParam->m_pOwner->Merge(lParam);
}

void CMainFrame::_Save( LPFILETHREADPARAM lParam)
{
	lParam->m_pOwner->Save(lParam);
}

void CMainFrame::Load( LPFILETHREADPARAM lParam)
{
	CString strFILEHEADER;

	lParam->m_pFile->Read( strFILEHEADER.GetBuffer(g_strFILEHEADER.GetLength()), g_strFILEHEADER.GetLength());
	strFILEHEADER.ReleaseBuffer(g_strFILEHEADER.GetLength());

	if( g_strFILEHEADER == strFILEHEADER )
	{
		//TRY
		//{
		DWORD dwAnimationID;
		DWORD dwOBJTypeID;
		DWORD dwTextureID;
		DWORD dwImageID;
		DWORD dwMediaID;
		DWORD dwFrameID;
		DWORD dwCompID;
		DWORD dwMeshID;
		DWORD dwMAPID;
		DWORD dwOBJID;
		DWORD dwSFXID;

		m_pDocAnimation->Clear();
		m_pDocTexture->Clear();
		m_pDocObject->Clear();
		m_pDocImage->Clear();
		m_pDocMedia->Clear();
		m_pDocFrame->Clear();
		m_pDocComp->Clear();
		m_pDocMesh->Clear();
		m_pDocSFX->Clear();
		m_pDocMAP->Clear();

		lParam->m_pFile->Read( &lParam->m_dwVER, sizeof(DWORD));
		lParam->m_pFile->Read( &dwAnimationID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwOBJTypeID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwTextureID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwImageID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwMediaID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwFrameID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwCompID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwMeshID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwMAPID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwOBJID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwSFXID, sizeof(DWORD));

		CTBSPPortal::m_bBasePortalID = 0xFF;
		CTBSPNode::m_bBaseNodeID = 0xFF;

		CItemBase::m_dwAnimationID = dwAnimationID;
		CItemBase::m_dwOBJTypeID = dwOBJTypeID;
		CItemBase::m_dwTextureID = dwTextureID;
		CItemBase::m_dwImageID = dwImageID;
		CItemBase::m_dwMediaID = dwMediaID;
		CItemBase::m_dwFrameID = dwFrameID;
		CItemBase::m_dwCompID = dwCompID;
		CItemBase::m_dwMeshID = dwMeshID;
		CItemBase::m_dwMAPID = dwMAPID;
		CItemBase::m_dwOBJID = dwOBJID;
		CItemBase::m_dwSFXID = dwSFXID;

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_GLOBAL);
		lParam->m_pWnd->m_cProgressBar.SetPos(0);
		lParam->m_pWnd->m_bPercent = 0;
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		LoadGlobal(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_TEX);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocTexture->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_IMG);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocImage->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_MEDIA);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocMedia->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_ANI);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocAnimation->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_MESH);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocMesh->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_OBJ);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocObject->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_SFX);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocSFX->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_MAP);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocMAP->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_COMP);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocComp->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_FRAME);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocFrame->LoadData(lParam);

		CItemBase::m_dwAnimationID = dwAnimationID;
		CItemBase::m_dwOBJTypeID = dwOBJTypeID;
		CItemBase::m_dwTextureID = dwTextureID;
		CItemBase::m_dwImageID = dwImageID;
		CItemBase::m_dwMediaID = dwMediaID;
		CItemBase::m_dwFrameID = dwFrameID;
		CItemBase::m_dwCompID = dwCompID;
		CItemBase::m_dwMeshID = dwMeshID;
		CItemBase::m_dwMAPID = dwMAPID;
		CItemBase::m_dwOBJID = dwOBJID;
		CItemBase::m_dwSFXID = dwSFXID;
	}

	lParam->m_pWnd->SendMessage( WM_COMMAND, ID_PROGRESS_EXIT);
}

void CMainFrame::Merge( LPFILETHREADPARAM lParam)
{
	CString strFILEHEADER;

	lParam->m_pFile->Read( strFILEHEADER.GetBuffer(g_strFILEHEADER.GetLength()), g_strFILEHEADER.GetLength());
	strFILEHEADER.ReleaseBuffer(g_strFILEHEADER.GetLength());

	if( g_strFILEHEADER == strFILEHEADER )
	{
		DWORD dwAnimationID;
		DWORD dwOBJTypeID;
		DWORD dwTextureID;
		DWORD dwImageID;
		DWORD dwMediaID;
		DWORD dwFrameID;
		DWORD dwCompID;
		DWORD dwMeshID;
		DWORD dwMAPID;
		DWORD dwOBJID;
		DWORD dwSFXID;

		lParam->m_pFile->Read( &lParam->m_dwVER, sizeof(DWORD));
		lParam->m_pFile->Read( &dwAnimationID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwOBJTypeID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwTextureID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwImageID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwMediaID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwFrameID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwCompID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwMeshID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwMAPID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwOBJID, sizeof(DWORD));
		lParam->m_pFile->Read( &dwSFXID, sizeof(DWORD));

		CTBSPPortal::m_bBasePortalID = 0xFF;
		CTBSPNode::m_bBaseNodeID = 0xFF;

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_GLOBAL);
		lParam->m_pWnd->m_cProgressBar.SetPos(0);
		lParam->m_pWnd->m_bPercent = 0;
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		LoadGlobal(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_TEX);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocTexture->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_IMG);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocImage->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_MEDIA);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocMedia->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_ANI);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocAnimation->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_MESH);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocMesh->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_OBJ);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocObject->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_SFX);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocSFX->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_MAP);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocMAP->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_COMP);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocComp->LoadData(lParam);

		lParam->m_pWnd->m_strStatus.LoadString(IDS_LOAD_FRAME);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
		m_pDocFrame->LoadData(lParam);

		CItemBase::m_dwAnimationID	+= dwAnimationID; 
		CItemBase::m_dwOBJTypeID	+= dwOBJTypeID;
		CItemBase::m_dwTextureID	+= dwTextureID;
		CItemBase::m_dwImageID		+= dwImageID;
		CItemBase::m_dwMediaID		+= dwMediaID;
		CItemBase::m_dwFrameID		+= dwFrameID;
		CItemBase::m_dwCompID		+= dwCompID;
		CItemBase::m_dwMeshID		+= dwMeshID;
		CItemBase::m_dwMAPID 		+= dwMAPID;
		CItemBase::m_dwOBJID 		+= dwOBJID;
		CItemBase::m_dwSFXID 		+= dwSFXID;
	}

	lParam->m_pWnd->SendMessage( WM_COMMAND, ID_PROGRESS_EXIT);
}

void CMainFrame::Save( LPFILETHREADPARAM lParam)
{
	lParam->m_dwVER = __T3D_SDK_VER;

	lParam->m_pFile->Write( LPCSTR(g_strFILEHEADER), g_strFILEHEADER.GetLength());
	lParam->m_pFile->Write( &lParam->m_dwVER, sizeof(DWORD));

	lParam->m_pFile->Write( &CItemBase::m_dwAnimationID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwOBJTypeID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwTextureID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwImageID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwMediaID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwFrameID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwCompID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwMeshID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwMAPID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwOBJID, sizeof(DWORD));
	lParam->m_pFile->Write( &CItemBase::m_dwSFXID, sizeof(DWORD));

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_GLOBAL);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	SaveGlobal(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_TEX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocTexture->SaveData(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_IMG);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocImage->SaveData(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_MEDIA);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocMedia->SaveData(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_ANI);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocAnimation->SaveData(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_MESH);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocMesh->SaveData(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_OBJ);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocObject->SaveData(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_SFX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocSFX->SaveData(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_MAP);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocMAP->SaveData(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_COMP);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocComp->SaveData(lParam);

	lParam->m_pWnd->m_strStatus.LoadString(IDS_SAVE_FRAME);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);
	m_pDocFrame->SaveData(lParam);

	lParam->m_pWnd->SendMessage( WM_COMMAND, ID_PROGRESS_EXIT);
}

void CMainFrame::SaveGlobal( LPFILETHREADPARAM lParam)
{
	int nTotal = INT(CItemBase::m_mapGlobalID.size()) + INT(CItemBase::m_mapLocalID.size()) + INT(CItemBase::m_mapFont.size()) + INT(CItemBase::m_mapMenuID.size());
	int nCount = INT(CItemBase::m_mapGlobalID.size());
	int nIndex = 0;

	MAPIDINDEX::iterator itL;
	MAPGSFONT::iterator itF;
	MAPSTRID::iterator itG;
	MAPSTR::iterator itM;

	lParam->m_pFile->Write( &CItemBase::m_dwLocalID, sizeof(DWORD));
	lParam->m_pFile->Write( &nCount, sizeof(int));

	for( itG = CItemBase::m_mapGlobalID.begin(); itG != CItemBase::m_mapGlobalID.end(); itG++)
	{
		CItemBase::WriteString( lParam->m_pFile, CString((*itG).first));
		lParam->m_pFile->Write( &(*itG).second, sizeof(DWORD));
		nIndex++;

		lParam->m_pWnd->m_bPercent = nIndex * 100 / nTotal;
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	nCount = INT(CItemBase::m_mapLocalID.size());
	lParam->m_pFile->Write( &nCount, sizeof(int));

	for( itL = CItemBase::m_mapLocalID.begin(); itL != CItemBase::m_mapLocalID.end(); itL++)
	{
		CItemBase::WriteString( lParam->m_pFile, CString((*itL).first));
		nCount = INT((*itL).second->size());
		lParam->m_pFile->Write( &nCount, sizeof(int));

		for( int i=0; i<nCount; i++)
		{
			lParam->m_pFile->Write( &((*((*itL).second))[i]->m_bType), sizeof(BYTE));
			lParam->m_pFile->Write( &((*((*itL).second))[i]->m_dwID), sizeof(DWORD));
		}
		nIndex++;

		lParam->m_pWnd->m_bPercent = nIndex * 100 / nTotal;
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	nCount = INT(CItemBase::m_mapFont.size());
	lParam->m_pFile->Write( &nCount, sizeof(int));

	for( itF = CItemBase::m_mapFont.begin(); itF != CItemBase::m_mapFont.end(); itF++)
	{
		lParam->m_pFile->Write( &(*itF).first, sizeof(DWORD));
		lParam->m_pFile->Write( &(*itF).second->m_FONT, sizeof(LOGFONT));
		lParam->m_pFile->Write( &(*itF).second->m_dwColor, sizeof(DWORD));
		CItemBase::WriteString( lParam->m_pFile, (*itF).second->m_strFontID);
		nIndex++;

		lParam->m_pWnd->m_bPercent = nIndex * 100 / nTotal;
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	nCount = INT(CItemBase::m_mapMenuID.size());
	lParam->m_pFile->Write( &nCount, sizeof(int));

	for( itM = CItemBase::m_mapMenuID.begin(); itM != CItemBase::m_mapMenuID.end(); itM++)
	{
		lParam->m_pFile->Write( &(*itM).first, sizeof(DWORD));
		CItemBase::WriteString( lParam->m_pFile, (*itM).second);
		nIndex++;

		lParam->m_pWnd->m_bPercent = nIndex * 100 / nTotal;
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}
}

void CMainFrame::LoadGlobal( LPFILETHREADPARAM lParam)
{
	DWORD dwLength = (DWORD) lParam->m_pFile->GetLength();
	int nCount = 0;

	if(!CItemBase::m_bMerge)
		lParam->m_pFile->Read( &CItemBase::m_dwLocalID, sizeof(DWORD));
	else
		lParam->m_pFile->Seek( sizeof(DWORD), CFile::current);
	lParam->m_pFile->Read( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		CString strID = CItemBase::LoadString(lParam->m_pFile);
		DWORD dwID;

		lParam->m_pFile->Read( &dwID, sizeof(DWORD));
		if(!CItemBase::m_bMerge)
			CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( strID, dwID));

		lParam->m_pWnd->m_bPercent = (BYTE) (lParam->m_pFile->GetPosition() * 100 / dwLength);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	lParam->m_pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		CString strID = CItemBase::LoadString(lParam->m_pFile);
		VECTORIDINDEX *pINDEX = new VECTORIDINDEX();
		int nINDEX = 0;

		lParam->m_pFile->Read( &nINDEX, sizeof(int));
		for( int j=0; j<nINDEX; j++)
		{
			LPIDINDEX pID = new IDINDEX();

			lParam->m_pFile->Read( &pID->m_bType, sizeof(BYTE));
			lParam->m_pFile->Read( &pID->m_dwID, sizeof(DWORD));

			if(!CItemBase::m_bMerge)
				pINDEX->push_back(pID);
			else
				delete pID;
		}

		if(!CItemBase::m_bMerge)
			CItemBase::m_mapLocalID.insert( MAPIDINDEX::value_type( strID, pINDEX));
		else
			delete pINDEX;

		lParam->m_pWnd->m_bPercent = (BYTE) (lParam->m_pFile->GetPosition() * 100 / dwLength);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	lParam->m_pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		LPGSFONT pFONT = new GSFONT();
		DWORD dwID;

		lParam->m_pFile->Read( &dwID, sizeof(DWORD));
		lParam->m_pFile->Read( &pFONT->m_FONT, sizeof(LOGFONT));
		lParam->m_pFile->Read( &pFONT->m_dwColor, sizeof(DWORD));
		pFONT->m_strFontID = CItemBase::LoadString(lParam->m_pFile);
		pFONT->m_GDIFONT.CreateFontIndirect(&pFONT->m_FONT);
		pFONT->m_D3DFONT.SetFont(&pFONT->m_GDIFONT);
		pFONT->m_D3DFONT.m_dwColor = pFONT->m_dwColor;

		if(CItemBase::m_bMerge)
			if( CItemBase::m_mapGlobalID.find(pFONT->m_strFontID) == CItemBase::m_mapGlobalID.end() )
			{
				dwID = 1;

				while( CItemBase::m_mapFont.find(dwID) != CItemBase::m_mapFont.end() )
					dwID++;

				CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( pFONT->m_strFontID, dwID));
				CItemBase::m_mapFont.insert( MAPGSFONT::value_type( dwID, pFONT));
			}
			else
				delete pFONT;
		else
			CItemBase::m_mapFont.insert( MAPGSFONT::value_type( dwID, pFONT));

		lParam->m_pWnd->m_bPercent = (BYTE) (lParam->m_pFile->GetPosition() * 100 / dwLength);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	lParam->m_pFile->Read( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
	{
		CString strID;
		DWORD dwID;

		lParam->m_pFile->Read( &dwID, sizeof(DWORD));
		strID = CItemBase::LoadString(lParam->m_pFile);

		if( CItemBase::m_bMerge && CItemBase::m_mapGlobalID.find(strID) == CItemBase::m_mapGlobalID.end() )
		{
			dwID = 1;

			while( CItemBase::m_mapMenuID.find(dwID) != CItemBase::m_mapMenuID.end() )
				dwID++;

			CItemBase::m_mapGlobalID.insert( MAPSTRID::value_type( strID, dwID));
			CItemBase::m_mapMenuID.insert( MAPSTR::value_type( dwID, strID));
		}
		else
			CItemBase::m_mapMenuID.insert( MAPSTR::value_type( dwID, strID));

		lParam->m_pWnd->m_bPercent = (BYTE) (lParam->m_pFile->GetPosition() * 100 / dwLength);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}
}

void CMainFrame::Build( LPBUILDTHREADPARAM lParam)
{
	MAPRESINDEX::iterator it;
	MAPRESINDEX mapINDEX;
	VECTORSTRING vFILE;
	CTShell shell;

	CTreeCtrl *pTree = &m_pDocTexture->GetTreeCtrl();
	HTREEITEM itemPos = pTree->GetRootItem();

	CString strTITLE = shell.GetFileTitle(m_strFileName);
	CString strINDEX;

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_TEXDAT);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	while(itemPos)
	{
		BuildTEX( lParam, pTree, itemPos, vFILE, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_TEXIDX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	strINDEX.Format( IDS_FMT_TEXIDX, strTITLE);
	BuildINDEX( lParam, strINDEX, vFILE, mapINDEX);

	for( it = mapINDEX.begin(); it != mapINDEX.end(); it++)
		delete (*it).second;

	mapINDEX.clear();
	vFILE.clear();

	pTree = &m_pDocImage->GetTreeCtrl();
	itemPos = pTree->GetRootItem();

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_IMGDAT);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	while(itemPos)
	{
		BuildIMG( lParam, pTree, itemPos, vFILE, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_IMGIDX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	strINDEX.Format( IDS_FMT_IMGIDX, strTITLE);
	BuildINDEX( lParam, strINDEX, vFILE, mapINDEX);

	for( it = mapINDEX.begin(); it != mapINDEX.end(); it++)
		delete (*it).second;

	mapINDEX.clear();
	vFILE.clear();

	pTree = &m_pDocMedia->GetTreeCtrl();
	itemPos = pTree->GetRootItem();

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_MEDIADAT);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	while(itemPos)
	{
		BuildMEDIA( lParam, pTree, itemPos, vFILE, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_MEDIAIDX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	strINDEX.Format( IDS_FMT_MEDIAIDX, strTITLE);
	BuildINDEX( lParam, strINDEX, vFILE, mapINDEX);

	for( it = mapINDEX.begin(); it != mapINDEX.end(); it++)
		delete (*it).second;

	mapINDEX.clear();
	vFILE.clear();

	pTree = &m_pDocAnimation->GetTreeCtrl();
	itemPos = pTree->GetRootItem();

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_ANIDAT);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	while(itemPos)
	{
		BuildANI( lParam, pTree, itemPos, vFILE, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_ANIIDX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	strINDEX.Format( IDS_FMT_ANIIDX, strTITLE);
	BuildINDEX( lParam, strINDEX, vFILE, mapINDEX);

	for( it = mapINDEX.begin(); it != mapINDEX.end(); it++)
		delete (*it).second;

	mapINDEX.clear();
	vFILE.clear();

	pTree = &m_pDocMesh->GetTreeCtrl();
	itemPos = pTree->GetRootItem();

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_MESHDAT);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	while(itemPos)
	{
		BuildMESH( lParam, pTree, itemPos, vFILE, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_MESHIDX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	strINDEX.Format( IDS_FMT_MESHIDX, strTITLE);
	BuildINDEX( lParam, strINDEX, vFILE, mapINDEX);

	for( it = mapINDEX.begin(); it != mapINDEX.end(); it++)
		delete (*it).second;

	mapINDEX.clear();
	vFILE.clear();

	pTree = &m_pDocObject->GetTreeCtrl();
	itemPos = pTree->GetRootItem();

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_OBJDAT);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	while(itemPos)
	{
		BuildOBJ( lParam, pTree, itemPos, vFILE, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_OBJIDX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	strINDEX.Format( IDS_FMT_OBJIDX, strTITLE);
	BuildINDEX( lParam, strINDEX, vFILE, mapINDEX);

	for( it = mapINDEX.begin(); it != mapINDEX.end(); it++)
		delete (*it).second;

	mapINDEX.clear();
	vFILE.clear();

	pTree = &m_pDocSFX->GetTreeCtrl();
	itemPos = pTree->GetRootItem();

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_SFXDAT);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	while(itemPos)
	{
		BuildSFX( lParam, pTree, itemPos, vFILE, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_SFXIDX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	strINDEX.Format( IDS_FMT_SFXIDX, strTITLE);
	BuildINDEX( lParam, strINDEX, vFILE, mapINDEX);

	for( it = mapINDEX.begin(); it != mapINDEX.end(); it++)
		delete (*it).second;

	mapINDEX.clear();
	vFILE.clear();

	pTree = &m_pDocMAP->GetTreeCtrl();
	itemPos = pTree->GetRootItem();

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_MAPDAT);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	while(itemPos)
	{
		BuildMAP( lParam, pTree, itemPos, vFILE, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_MAPIDX);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	strINDEX.Format( IDS_FMT_MAPIDX, strTITLE);
	BuildINDEX( lParam, strINDEX, vFILE, mapINDEX);

	for( it = mapINDEX.begin(); it != mapINDEX.end(); it++)
		delete (*it).second;

	mapINDEX.clear();
	vFILE.clear();

	lParam->m_pWnd->m_strStatus.LoadString(IDS_BUILD_HEADER);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	BuildTSC( lParam, strTITLE);
	BuildHEADER( lParam, strTITLE);

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}

void CMainFrame::LoadToolBar( UINT nResourceID)
{
	if( m_dwToolBarID != nResourceID )
	{
		m_wndToolBar.LoadToolBar(nResourceID);
		m_dwToolBarID = nResourceID;
	}
}

void CMainFrame::OnUpdateFrameTitle( BOOL bAddToTitle)
{
	BOOL bMaximized = TRUE;

	if(!MDIGetActive(&bMaximized))
		LoadToolBar(IDR_MAINFRAME);

	CMDIFrameWnd::OnUpdateFrameTitle(bAddToTitle);
}

BOOL CMainFrame::OnCommand( WPARAM wParam, LPARAM lParam)
{
	switch(wParam & 0xFFFF)
	{
	case ID_FILE_SAVE_AS	: OnFileSaveAs(); return TRUE;
	case ID_FILE_SAVE		: OnFileSave(); return TRUE;
	case ID_FILE_OPEN		: OnFileOpen(); return TRUE;
	case ID_FILE_NEW		: OnFileNew(); return TRUE;
	case ID_MDI_NEXT		: MDINext(); return TRUE;
	}

	return CMDIFrameWnd::OnCommand(wParam, lParam);
}

void CMainFrame::BuildINDEX( LPBUILDTHREADPARAM lParam,
							 CString strINDEX,
							 VECTORSTRING& vFILE,
							 MAPRESINDEX& mapINDEX)
{
	CString strPATH = lParam->m_strPATH;

	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Index\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strINDEX;

	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
	MAPRESINDEX::iterator it;

	int nCount = INT(vFILE.size());
	int nIndex = 0;

	file.Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
		CItemBase::WriteString( &file, vFILE[i]);

	nCount = INT(mapINDEX.size());
	file.Write( &nCount, sizeof(int));

	for( it = mapINDEX.begin(); it != mapINDEX.end(); it++)
	{
		LPRESINDEX pRES = (*it).second;

		file.Write( &(*it).first, sizeof(DWORD));
		file.Write( &pRES->m_dwFileID, sizeof(DWORD));
		file.Write( &pRES->m_dwPOS, sizeof(DWORD));
		nIndex++;

		lParam->m_pWnd->m_cProgressBar.SetPos(nIndex / nCount);
		lParam->m_pWnd->m_bPercent = nIndex / nCount;
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}
}

void CMainFrame::BuildHEADER( LPBUILDTHREADPARAM lParam, CString strTITLE)
{
	int nCount = INT(CItemBase::m_mapGlobalID.size());
	int nIndex = 0;

	CString strPATH = lParam->m_strPATH;
	MAPSTRID::iterator it;

	strPATH.TrimRight('\\');
	strPATH = strPATH + _T('\\') + strTITLE + _T("ID.h");
	CStdioFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeText);

	file.WriteString("#pragma once\n\n");
	for( it = CItemBase::m_mapGlobalID.begin(); it != CItemBase::m_mapGlobalID.end(); it++)
	{
		CString strLINE;

		strLINE.Format( IDS_FMT_DEFINE, (*it).first, (*it).second);
		file.WriteString(strLINE);

		nIndex++;
		lParam->m_pWnd->m_cProgressBar.SetPos(nIndex / nCount);
		lParam->m_pWnd->m_bPercent = nIndex / nCount;
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}
}

void CMainFrame::BuildTEX( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   VECTORSTRING& vFILE,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pFILE = (CItemBase *) pTree->GetItemData(itemPos);
	CString strPATH = lParam->m_strPATH;
	CString strFILE;

	DWORD dwFileID = (DWORD) vFILE.size();
	int nIndex = 0;

	strFILE.Format( IDS_EXT_TEX, pFILE->m_strName);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("Skin\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	vFILE.push_back(_T("Skin\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildTEX( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildIMG( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   VECTORSTRING& vFILE,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pFILE = (CItemBase *) pTree->GetItemData(itemPos);
	CString strPATH = lParam->m_strPATH;
	CString strFILE;

	DWORD dwFileID = (DWORD) vFILE.size();
	int nIndex = 0;

	strFILE.Format( IDS_EXT_IMG, pFILE->m_strName);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("Img\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	vFILE.push_back(_T("Img\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildIMG( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildMEDIA( LPBUILDTHREADPARAM lParam,
							 CTreeCtrl *pTree,
							 HTREEITEM itemPos,
							 VECTORSTRING& vFILE,
							 MAPRESINDEX& mapINDEX)
{
	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);

	if( pItem->m_bType == ITEM_MEDIA )
	{
		DWORD dwFileID = (DWORD) vFILE.size();
		LPRESINDEX pRES = new RESINDEX();

		CString strPATH = lParam->m_strPATH;
		CString strFILE;

		strFILE.Format( IDS_EXT_MEDIA, ((CMediaItem *) pItem)->GetPlayerType(), pItem->m_dwID);
		strPATH.TrimRight('\\');
		strPATH = strPATH + _T("\\Data\\");

		CreateDirectory( strPATH, NULL);
		strPATH = strPATH + _T("Media\\");

		CreateDirectory( strPATH, NULL);
		strPATH = strPATH + strFILE;

		pRES->m_dwPOS = MAKELONG( WORD(((CMediaItem *) pItem)->GetPlayerType()), WORD(((CMediaItem *) pItem)->m_bVolume));
		pRES->m_dwFileID = dwFileID;

		mapINDEX.insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
		vFILE.push_back(_T("Media\\") + strFILE);

		CopyFile( ((CMediaItem *) pItem)->m_szFileName, strPATH, FALSE);
	}

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildMEDIA( lParam, pTree, itemPos, vFILE, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildANI( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   VECTORSTRING& vFILE,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pFILE = (CItemBase *) pTree->GetItemData(itemPos);
	CString strPATH = lParam->m_strPATH;
	CString strFILE;

	DWORD dwFileID = (DWORD) vFILE.size();
	int nIndex = 0;

	strFILE.Format( IDS_EXT_ACT, pFILE->m_strName);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("Action\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	vFILE.push_back(_T("Action\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildANI( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildMESH( LPBUILDTHREADPARAM lParam,
						    CTreeCtrl *pTree,
							HTREEITEM itemPos,
							VECTORSTRING& vFILE,
							MAPRESINDEX& mapINDEX)
{
	CItemBase *pFILE = (CItemBase *) pTree->GetItemData(itemPos);
	CString strPATH = lParam->m_strPATH;
	CString strFILE;

	DWORD dwFileID = (DWORD) vFILE.size();
	int nIndex = 0;

	strFILE.Format( IDS_EXT_MESH, pFILE->m_strName);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("Mesh\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	vFILE.push_back(_T("Mesh\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildMESH( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildOBJ( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   VECTORSTRING& vFILE,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pFILE = (CItemBase *) pTree->GetItemData(itemPos);
	CString strPATH = lParam->m_strPATH;
	CString strFILE;

	DWORD dwFileID = (DWORD) vFILE.size();
	int nIndex = 0;

	strFILE.Format( IDS_EXT_OBJ, pFILE->m_strName);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("OBJ\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	vFILE.push_back(_T("OBJ\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildOBJ( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildSFX( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   VECTORSTRING& vFILE,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pFILE = (CItemBase *) pTree->GetItemData(itemPos);
	CString strPATH = lParam->m_strPATH;
	CString strFILE;

	DWORD dwFileID = (DWORD) vFILE.size();
	int nIndex = 0;

	strFILE.Format( IDS_EXT_SFX, pFILE->m_strName);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("SFX\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	vFILE.push_back(_T("SFX\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildSFX( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildMAP( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   VECTORSTRING& vFILE,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pFILE = (CItemBase *) pTree->GetItemData(itemPos);
	CString strPATH = lParam->m_strPATH;
	CString strFILE;

	DWORD dwFileID = (DWORD) vFILE.size();
	int nIndex = 0;

	strFILE.Format( IDS_EXT_MAP, pFILE->m_strName);
	strPATH.TrimRight('\\');
	strPATH = strPATH + _T("\\Data\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + _T("MAP\\");

	CreateDirectory( strPATH, NULL);
	strPATH = strPATH + strFILE;

	vFILE.push_back(_T("MAP\\") + strFILE);
	CFile file( strPATH, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildMAP( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildTEX( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   DWORD dwFileID,
						   int& nIndex,
						   CFile& file,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	int nCount = pTree->GetCount();

	if( pItem->m_bType == ITEM_TEXTURE )
	{
		LPRESINDEX pRES = new RESINDEX();

		pRES->m_dwPOS = (DWORD) file.GetPosition();
		pRES->m_dwFileID = dwFileID;

		mapINDEX.insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
		BuildTEX( (CTextureItem *) pItem, file);
	}

	nIndex++;
	lParam->m_pWnd->m_cProgressBar.SetPos(nIndex / nCount);
	lParam->m_pWnd->m_bPercent = nIndex / nCount;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildTEX( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildIMG( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   DWORD dwFileID,
						   int& nIndex,
						   CFile& file,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	int nCount = pTree->GetCount();

	if( pItem->m_bType == ITEM_IMAGE )
	{
		LPRESINDEX pRES = new RESINDEX();

		pRES->m_dwPOS = (DWORD) file.GetPosition();
		pRES->m_dwFileID = dwFileID;

		mapINDEX.insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
		BuildIMG( (CImageItem *) pItem, file);
	}

	nIndex++;
	lParam->m_pWnd->m_cProgressBar.SetPos(nIndex / nCount);
	lParam->m_pWnd->m_bPercent = nIndex / nCount;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildIMG( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildANI( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   DWORD dwFileID,
						   int& nIndex,
						   CFile& file,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	int nCount = pTree->GetCount();

	if( pItem->m_bType == ITEM_ANIMATION )
	{
		LPRESINDEX pRES = new RESINDEX();

		pRES->m_dwPOS = (DWORD) file.GetPosition();
		pRES->m_dwFileID = dwFileID;

		mapINDEX.insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
		BuildANI( (CAnimationItem *) pItem, file);
	}

	nIndex++;
	lParam->m_pWnd->m_cProgressBar.SetPos(nIndex / nCount);
	lParam->m_pWnd->m_bPercent = nIndex / nCount;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildANI( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildMESH( LPBUILDTHREADPARAM lParam,
						    CTreeCtrl *pTree,
							HTREEITEM itemPos,
							DWORD dwFileID,
							int& nIndex,
							CFile& file,
							MAPRESINDEX& mapINDEX)
{
	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	int nCount = pTree->GetCount();

	if( pItem->m_bType == ITEM_MESH )
	{
		LPRESINDEX pRES = new RESINDEX();

		pRES->m_dwPOS = (DWORD) file.GetPosition();
		pRES->m_dwFileID = dwFileID;

		mapINDEX.insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
		BuildMESH( (CMeshItem *) pItem, file);
	}

	nIndex++;
	lParam->m_pWnd->m_cProgressBar.SetPos(nIndex / nCount);
	lParam->m_pWnd->m_bPercent = nIndex / nCount;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildMESH( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildOBJ( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   DWORD dwFileID,
						   int& nIndex,
						   CFile& file,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	int nCount = pTree->GetCount();

	if( pItem->m_bType == ITEM_OBJECT )
	{
		LPRESINDEX pRES = new RESINDEX();

		pRES->m_dwPOS = (DWORD) file.GetPosition();
		pRES->m_dwFileID = dwFileID;

		mapINDEX.insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
		BuildOBJ( (COBJItem *) pItem, file);
	}

	nIndex++;
	lParam->m_pWnd->m_cProgressBar.SetPos(nIndex / nCount);
	lParam->m_pWnd->m_bPercent = nIndex / nCount;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildOBJ( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildSFX( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   DWORD dwFileID,
						   int& nIndex,
						   CFile& file,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	int nCount = pTree->GetCount();

	if( pItem->m_bType == ITEM_SFX )
	{
		LPRESINDEX pRES = new RESINDEX();

		pRES->m_dwPOS = (DWORD) file.GetPosition();
		pRES->m_dwFileID = dwFileID;

		mapINDEX.insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
		BuildSFX( (CSFXItem *) pItem, file);
	}

	nIndex++;
	lParam->m_pWnd->m_cProgressBar.SetPos(nIndex / nCount);
	lParam->m_pWnd->m_bPercent = nIndex / nCount;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildSFX( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildMAP( LPBUILDTHREADPARAM lParam,
						   CTreeCtrl *pTree,
						   HTREEITEM itemPos,
						   DWORD dwFileID,
						   int& nIndex,
						   CFile& file,
						   MAPRESINDEX& mapINDEX)
{
	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	int nCount = pTree->GetCount();

	if( pItem->m_bType == ITEM_EXTMAP )
	{
		LPRESINDEX pRES = new RESINDEX();

		pRES->m_dwPOS = (DWORD) file.GetPosition();
		pRES->m_dwFileID = dwFileID;

		mapINDEX.insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
		BuildEXTMAP( (CEXTMAPItem *) pItem, file);
	}
	else if( pItem->m_bType == ITEM_BSPMAP )
	{
		LPRESINDEX pRES = new RESINDEX();

		pRES->m_dwPOS = (DWORD) file.GetPosition();
		pRES->m_dwFileID = dwFileID;

		mapINDEX.insert( MAPRESINDEX::value_type( pItem->m_dwID, pRES));
		BuildBSPMAP( (CBSPMAPItem *) pItem, file);
	}

	nIndex++;
	lParam->m_pWnd->m_cProgressBar.SetPos(nIndex / nCount);
	lParam->m_pWnd->m_bPercent = nIndex / nCount;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		BuildMAP( lParam, pTree, itemPos, dwFileID, nIndex, file, mapINDEX);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::BuildTEX( CTextureItem *pItem, CFile& file)
{
	int nCount = INT(pItem->m_vTextureID.size());

	file.Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
		file.Write( &pItem->m_vTextureID[i], sizeof(DWORD));

	nCount = INT(pItem->GetTEXTURESET()->m_vKey.size());
	file.Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		file.Write( &pItem->GetTEXTURESET()->m_vKey[i]->m_dwTick, sizeof(DWORD));
		file.Write( &pItem->GetTEXTURESET()->m_vKey[i]->m_fKeyU, sizeof(FLOAT));
		file.Write( &pItem->GetTEXTURESET()->m_vKey[i]->m_fKeyV, sizeof(FLOAT));
		file.Write( &pItem->GetTEXTURESET()->m_vKey[i]->m_fKeyR, sizeof(FLOAT));
		file.Write( &pItem->GetTEXTURESET()->m_vKey[i]->m_fKeySU, sizeof(FLOAT));
		file.Write( &pItem->GetTEXTURESET()->m_vKey[i]->m_fKeySV, sizeof(FLOAT));
	}

	file.Write( &pItem->GetTEXTURESET()->m_dwTotalTick, sizeof(DWORD));
	file.Write( &pItem->GetTEXTURESET()->m_dwMipFilter, sizeof(DWORD));
	file.Write( &pItem->GetTEXTURESET()->m_fMipBias, sizeof(FLOAT));
	file.Write( &pItem->m_bTextureOption, sizeof(BYTE));
	file.Write( &pItem->m_dwSize, sizeof(DWORD));

	if( pItem->m_dwSize > 0 )
		file.Write( pItem->GetTexData(), pItem->m_dwSize);
}

void CMainFrame::BuildIMG( CImageItem *pItem, CFile& file)
{
	int nCount = INT(pItem->m_vImageID.size());

	file.Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
		file.Write( &pItem->m_vImageID[i], sizeof(DWORD));

	nCount = INT(pItem->m_ImageSet.m_vKey.size());
	file.Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		file.Write( &pItem->m_ImageSet.m_vKey[i]->m_dwTick, sizeof(DWORD));
		file.Write( &pItem->m_ImageSet.m_vKey[i]->m_dwColor, sizeof(DWORD));
	}

	file.Write( &pItem->m_ImageSet.m_dwTotalTick, sizeof(DWORD));
	file.Write( &pItem->m_dwSize, sizeof(DWORD));

	if( pItem->m_dwSize > 0 )
		file.Write( pItem->m_pData, pItem->m_dwSize);
}

void CMainFrame::BuildANI( CAnimationItem *pItem, CFile& file)
{
	int nCount = INT(pItem->m_vAniID.size());

	file.Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		file.Write( &pItem->m_vAniID[i], sizeof(DWORD));
		file.Write( &pItem->m_vReverse[i], sizeof(BYTE));
	}

	nCount = INT(pItem->m_Aniset.m_vKey.size());
	file.Write( &nCount, sizeof(int));

	for( i=0; i<nCount; i++)
	{
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fTime, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fPosX, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fPosY, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fPosZ, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fRotX, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fRotY, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fRotZ, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fScaleX, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fScaleY, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_fScaleZ, sizeof(FLOAT));
		file.Write( &pItem->m_Aniset.m_vKey[i]->m_dwEventID, sizeof(DWORD));
	}

	file.Write( &pItem->m_Aniset.m_fTimeScale, sizeof(FLOAT));
	file.Write( &pItem->m_Aniset.m_dwLoopID, sizeof(DWORD));
	file.Write( &pItem->m_Aniset.m_bLoop, sizeof(BYTE));

	if( pItem->m_dwSize > 0 )
	{
		LPBYTE pDATA = new BYTE[pItem->m_dwSize];

		DWORD dwNodeCount = *((LPDWORD) &pItem->GetAniData()[sizeof(int)]);
		DWORD dwSIZE = pItem->m_dwSize;
		int nRES = dwSIZE;

		LPBYTE pSRC = pItem->GetAniData();
		LPBYTE pDEST = pDATA;
		int nVersion = *((int *) pSRC);

		memcpy( pDEST, pSRC, sizeof(int) + 3 * sizeof(DWORD) + dwNodeCount * sizeof(D3DMATRIX));
		pDEST += sizeof(int) + 3 * sizeof(DWORD) + dwNodeCount * sizeof(D3DMATRIX);
		pSRC += sizeof(int) + 3 * sizeof(DWORD) + dwNodeCount * sizeof(D3DMATRIX);
		nRES -= sizeof(int) + 3 * sizeof(DWORD) + dwNodeCount * sizeof(D3DMATRIX);

		for( i=0; i<dwNodeCount; i++)
		{
			DWORD dwCount = *((LPDWORD) pSRC);

			*((LPDWORD) pDEST) = 0;
			pDEST += sizeof(DWORD);
			pSRC += sizeof(DWORD);
			nRES -= sizeof(DWORD);

			DWORD dwSKIP = dwCount * (nVersion < 301 ? sizeof(WMESHVERTEX) : sizeof(TMESHBUFFER));
			pSRC += dwSKIP;
			nRES -= dwSKIP;

			if( dwSIZE > dwSKIP )
				dwSIZE -= dwSKIP;
			else
				dwSIZE = 0;
		}

		if( nRES > 0 )
			memcpy( pDEST, pSRC, nRES);

		file.Write( &dwSIZE, sizeof(DWORD));
		if( dwSIZE > 0 )
			file.Write( pDATA, dwSIZE);

		delete[] pDATA;
	}
	else
		file.Write( &pItem->m_dwSize, sizeof(DWORD));
}

void CMainFrame::BuildMESH( CMeshItem *pItem, CFile& file)
{
	DWORD dwLevel = INT(pItem->m_Mesh.m_vDist.size() + 1);

	file.Write( &pItem->m_Mesh.m_dwMeshCount, sizeof(DWORD));
	file.Write( &pItem->m_Mesh.m_dwNodeCount, sizeof(DWORD));
	file.Write( &pItem->m_Mesh.m_bUseVB, sizeof(BYTE));
	file.Write( &dwLevel, sizeof(DWORD));

	file.Write( &pItem->m_Mesh.m_vCenterPoint, sizeof(D3DXVECTOR3));
	file.Write( &pItem->m_Mesh.m_fRadius, sizeof(FLOAT));

	if( pItem->m_Mesh.m_dwNodeCount > 0 )
		file.Write( pItem->m_Mesh.m_pBones, pItem->m_Mesh.m_dwNodeCount * sizeof(D3DXMATRIX));

	if(pItem->m_Mesh.m_pDATA)
	{
		file.Write( &pItem->m_Mesh.m_pDATA->m_dwCount, sizeof(DWORD));
		file.Write( pItem->m_Mesh.m_pDATA->m_pVB, pItem->m_Mesh.m_pDATA->m_dwCount * (pItem->m_Mesh.m_dwNodeCount > 0 ? sizeof(WMESHVERTEX) : sizeof(MESHVERTEX)));
	}
	else
	{
		DWORD dwCount = 0;
		file.Write( &dwCount, sizeof(DWORD));
	}

	for( int i=0; i<pItem->m_Mesh.m_dwMeshCount; i++)
		for( int j=0; j<dwLevel; j++)
		{
			DWORD dwCount = DWORD(pItem->m_Mesh.m_pMESH[i][j]->m_vIB.size());
			file.Write( &dwCount, sizeof(DWORD));

			for( DWORD k=0; k<dwCount; k++)
			{
				LPIBDATA pDATA = pItem->m_Mesh.m_pMESH[i][j]->m_vIB[k];

				file.Write( &pDATA->m_dwCount, sizeof(DWORD));
				file.Write( &pDATA->m_wMatrix, sizeof(WORD));

				if( pDATA->m_dwCount > 0 )
					file.Write( pDATA->m_pIB, pDATA->m_dwCount * sizeof(WORD));
			}
		}

	for( i=0; i<INT(dwLevel) - 1; i++)
		file.Write( &pItem->m_Mesh.m_vDist[i], sizeof(FLOAT));
}

void CMainFrame::BuildOBJ( COBJItem *pItem, CFile& file)
{
	MAPATTRITEM::iterator itATTR;
	MAPSFXINST::iterator itSFX;
	MAPSNDINST::iterator itSND;
	MAPACTION::iterator itACT;
	MAPCLKIND::iterator itCLK;
	MAPSTR::iterator itSTR;

	BYTE bPivotCount = BYTE(pItem->GetPivotCount() + 1);
	file.Write( &bPivotCount, sizeof(BYTE));

	int nCount = INT(pItem->m_mapPIVOT.size());
	file.Write( &nCount, sizeof(int));

	for( itSTR = pItem->m_mapPIVOT.begin(); itSTR != pItem->m_mapPIVOT.end(); itSTR++)
	{
		DWORD dwID = CItemBase::GetID((*itSTR).second);

		file.Write( &(*itSTR).first, sizeof(DWORD));
		file.Write( &dwID, sizeof(DWORD));
	}

	nCount = INT(pItem->m_OBJ.m_mapSFX.size());
	file.Write( &nCount, sizeof(int));

	for( itSFX = pItem->m_OBJ.m_mapSFX.begin(); itSFX != pItem->m_OBJ.m_mapSFX.end(); itSFX++)
	{
		MAPDWORD::iterator itID = pItem->m_mapSFX.find((*itSFX).first);

		file.Write( &(*itID).first, sizeof(DWORD));
		file.Write( &(*itID).second, sizeof(DWORD));
		file.Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
		file.Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
		file.Write( &(*itSFX).second->m_nPivotID, sizeof(int));
		file.Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
		file.Write( &(*itSFX).second->m_dwFuncID, sizeof(DWORD));
		file.Write( &(*itSFX).second->m_dwTick, sizeof(DWORD));
		file.Write( &(*itSFX).second->m_bDeleteOnStop, sizeof(BOOL));
	}

	nCount = INT(pItem->m_OBJ.m_mapSND.size());
	file.Write( &nCount, sizeof(int));

	for( itSND = pItem->m_OBJ.m_mapSND.begin(); itSND != pItem->m_OBJ.m_mapSND.end(); itSND++)
	{
		file.Write( &(*itSND).first, sizeof(DWORD));
		file.Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
		file.Write( &(*itSND).second->m_dwFuncID, sizeof(DWORD));
		file.Write( &(*itSND).second->m_dwTick, sizeof(DWORD));
		file.Write( &(*itSND).second->m_bDeleteOnStop, sizeof(BOOL));
	}

	nCount = INT(pItem->m_mapATTR.size());
	file.Write( &nCount, sizeof(int));

	for( itATTR = pItem->m_mapATTR.begin(); itATTR != pItem->m_mapATTR.end(); itATTR++)
	{
		itSTR = pItem->m_mapNAME.find((*itATTR).first);
		DWORD dwATTR = CItemBase::GetID((*itSTR).second);

		file.Write( &dwATTR, sizeof(DWORD));
		file.Write( &(*itATTR).second->m_bType, sizeof(BYTE));

		switch((*itATTR).second->m_bType)
		{
		case ATTRTYPE_INT		:
			{
				int nValue = atoi((*itATTR).second->m_strValue);
				int nSize = sizeof(int);

				file.Write( &nSize, sizeof(int));
				file.Write( &nValue, nSize);
			}

			break;

		case ATTRTYPE_FLOAT		:
			{
				FLOAT fValue = (FLOAT) atof((*itATTR).second->m_strValue);
				int nSize = sizeof(FLOAT);

				file.Write( &nSize, sizeof(int));
				file.Write( &fValue, nSize);
			}

			break;

		case ATTRTYPE_STRING	:
			{
				int nSize = (*itATTR).second->m_strValue.GetLength();

				file.Write( &nSize, sizeof(int));
				file.Write( LPCSTR((*itATTR).second->m_strValue), nSize);
			}

			break;
		}
	}

	nCount = INT(pItem->m_OBJ.m_mapACTION.size());
	file.Write( &nCount, sizeof(int));

	for( itACT = pItem->m_OBJ.m_mapACTION.begin(); itACT != pItem->m_OBJ.m_mapACTION.end(); itACT++)
	{
		LPACTION pACT = (*itACT).second;
		MAPANI::iterator itANI;

		itSTR = pItem->m_mapNAME.find((*itACT).first);
		DWORD dwACT = CItemBase::GetID((*itSTR).second);

		file.Write( &dwACT, sizeof(DWORD));
		nCount = INT(pACT->m_mapSFX.size());
		file.Write( &nCount, sizeof(int));

		for( itSFX = pACT->m_mapSFX.begin(); itSFX != pACT->m_mapSFX.end(); itSFX++)
		{
			MAPDWORD::iterator itID = pItem->m_mapSFX.find((*itSFX).first);

			file.Write( &(*itID).first, sizeof(DWORD));
			file.Write( &(*itID).second, sizeof(DWORD));
			file.Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
			file.Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
			file.Write( &(*itSFX).second->m_nPivotID, sizeof(int));
			file.Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
			file.Write( &(*itSFX).second->m_dwFuncID, sizeof(DWORD));
			file.Write( &(*itSFX).second->m_dwTick, sizeof(DWORD));
			file.Write( &(*itSFX).second->m_bDeleteOnStop, sizeof(BOOL));
		}

		nCount = INT(pACT->m_mapSND.size());
		file.Write( &nCount, sizeof(int));

		for( itSND = pACT->m_mapSND.begin(); itSND != pACT->m_mapSND.end(); itSND++)
		{
			file.Write( &(*itSND).first, sizeof(DWORD));
			file.Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
			file.Write( &(*itSND).second->m_dwFuncID, sizeof(DWORD));
			file.Write( &(*itSND).second->m_dwTick, sizeof(DWORD));
			file.Write( &(*itSND).second->m_bDeleteOnStop, sizeof(BOOL));
		}

		nCount = INT(pACT->m_mapANI.size());
		file.Write( &nCount, sizeof(int));

		for( itANI = pACT->m_mapANI.begin(); itANI != pACT->m_mapANI.end(); itANI++)
		{
			MAPDWORD::iterator finder = pItem->m_mapANI.find((*itANI).first);

			itSTR = pItem->m_mapNAME.find((*itANI).first);
			DWORD dwANI = CItemBase::GetID((*itSTR).second);

			file.Write( &dwANI, sizeof(DWORD));
			file.Write( &(*finder).second, sizeof(DWORD));

			nCount = INT((*itANI).second->m_mapSFX.size());
			file.Write( &nCount, sizeof(int));

			for( itSFX = (*itANI).second->m_mapSFX.begin(); itSFX != (*itANI).second->m_mapSFX.end(); itSFX++)
			{
				MAPDWORD::iterator itID = pItem->m_mapSFX.find((*itSFX).first);

				file.Write( &(*itID).first, sizeof(DWORD));
				file.Write( &(*itID).second, sizeof(DWORD));
				file.Write( &(*itSFX).second->m_vPOS, sizeof(D3DXVECTOR3));
				file.Write( &(*itSFX).second->m_vROT, sizeof(D3DXVECTOR3));
				file.Write( &(*itSFX).second->m_nPivotID, sizeof(int));
				file.Write( &(*itSFX).second->m_bBias, sizeof(BYTE));
				file.Write( &(*itSFX).second->m_dwFuncID, sizeof(DWORD));
				file.Write( &(*itSFX).second->m_dwTick, sizeof(DWORD));
				file.Write( &(*itSFX).second->m_bDeleteOnStop, sizeof(BOOL));
			}

			nCount = INT((*itANI).second->m_mapSND.size());
			file.Write( &nCount, sizeof(int));

			for( itSND = (*itANI).second->m_mapSND.begin(); itSND != (*itANI).second->m_mapSND.end(); itSND++)
			{
				file.Write( &(*itSND).first, sizeof(DWORD));
				file.Write( &(*itSND).second->m_dwMediaID, sizeof(DWORD));
				file.Write( &(*itSND).second->m_dwFuncID, sizeof(DWORD));
				file.Write( &(*itSND).second->m_dwTick, sizeof(DWORD));
				file.Write( &(*itSND).second->m_bDeleteOnStop, sizeof(BOOL));
			}
		}
	}

	nCount = INT(pItem->m_OBJ.m_mapCLKIND.size());
	file.Write( &nCount, sizeof(int));

	for( itCLK = pItem->m_OBJ.m_mapCLKIND.begin(); itCLK != pItem->m_OBJ.m_mapCLKIND.end(); itCLK++)
	{
		LPCLKIND pCLK = (*itCLK).second;
		MAPCLOTH::iterator itCL;

		itSTR = pItem->m_mapNAME.find((*itCLK).first);
		DWORD dwCLK = CItemBase::GetID((*itSTR).second);

		file.Write( &dwCLK, sizeof(DWORD));
		nCount = INT(pCLK->m_mapCLOTH.size());
		file.Write( &nCount, sizeof(int));

		for( itCL = pCLK->m_mapCLOTH.begin(); itCL != pCLK->m_mapCLOTH.end(); itCL++)
		{
			LPCLOTH pCL = (*itCL).second;
			MAPMESH::iterator itMESH;

			itSTR = pItem->m_mapNAME.find((*itCL).first);
			DWORD dwCL = CItemBase::GetID((*itSTR).second);

			file.Write( &dwCL, sizeof(DWORD));
			nCount = INT(pCL->m_mapMESH.size());
			file.Write( &nCount, sizeof(int));

			for( itMESH = pCL->m_mapMESH.begin(); itMESH != pCL->m_mapMESH.end(); itMESH++)
			{
				MAPDWORD::iterator finder = pItem->m_mapMESH.find((*itMESH).first);
				LPMESH pMESH = (*itMESH).second;

				itSTR = pItem->m_mapNAME.find((*itMESH).first);
				DWORD dwMESH = CItemBase::GetID((*itSTR).second);

				file.Write( &dwMESH, sizeof(DWORD));
				file.Write( &(*finder).second, sizeof(DWORD));
				nCount = INT(pMESH->m_vTEX.size());
				file.Write( &nCount, sizeof(int));

				for( int i=0; i<nCount; i++)
				{
					LPOBJTEX pTEX = pMESH->m_vTEX[i];

					DWORD dwTEX = pItem->GetTexID( (*itMESH).first, i, 0);
					finder = pItem->m_mapTEX.find(dwTEX);
					if( finder != pItem->m_mapTEX.end() )
						dwTEX = (*finder).second;
					else
						dwTEX = 0;
					file.Write( &dwTEX, sizeof(DWORD));

					dwTEX = pItem->GetTexID( (*itMESH).first, i, 1);
					finder = pItem->m_mapTEX.find(dwTEX);
					if( finder != pItem->m_mapTEX.end() )
						dwTEX = (*finder).second;
					else
						dwTEX = 0;
					file.Write( &dwTEX, sizeof(DWORD));

					file.Write( &pTEX->m_bType[0], sizeof(BYTE));
					file.Write( &pTEX->m_bType[1], sizeof(BYTE));

					DWORD dwCOLORID = pItem->GetColorID( (*itMESH).first, i);
					itSTR = pItem->m_mapNAME.find(dwCOLORID);
					if( itSTR != pItem->m_mapNAME.end() )
						dwCOLORID = CItemBase::GetID((*itSTR).second);
					else
						dwCOLORID = 0;
					file.Write( &dwCOLORID, sizeof(DWORD));

					file.Write( &pTEX->m_dwCOLOR, sizeof(DWORD));
					file.Write( &pTEX->m_dwOrder, sizeof(DWORD));
					file.Write( &pTEX->m_dwOP, sizeof(DWORD));
					file.Write( &pTEX->m_dwDESTBlend, sizeof(DWORD));
					file.Write( &pTEX->m_dwSRCBlend, sizeof(DWORD));
					file.Write( &pTEX->m_dwBlendOP, sizeof(DWORD));
					file.Write( &pTEX->m_fIntensity, sizeof(FLOAT));
					file.Write( &pTEX->m_bMinAlpha, sizeof(BYTE));
					file.Write( &pTEX->m_bZEnable, sizeof(BYTE));
					file.Write( &pTEX->m_bZWrite, sizeof(BYTE));
					file.Write( &pTEX->m_b2side, sizeof(BYTE));
					file.Write( &pTEX->m_bUseDirLight, sizeof(BOOL));
					file.Write( &pTEX->m_dwAmbientCOLOR, sizeof(DWORD));
				}
			}
		}
	}
}

void CMainFrame::BuildSFX( CSFXItem *pItem, CFile& file)
{
	int nCount = INT(pItem->m_SFX.m_vKey.size());

	file.Write( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		file.Write( &pItem->m_SFX.m_vKey[i]->m_dwColor, sizeof(DWORD));
		file.Write( &pItem->m_SFX.m_vKey[i]->m_dwTick, sizeof(DWORD));
	}

	nCount = INT(pItem->m_vSFXID.size());
	file.Write( &nCount, sizeof(int));
	for( i=0; i<nCount; i++)
		file.Write( &pItem->m_vSFXID[i], sizeof(DWORD));

	file.Write( &pItem->m_SFX.m_bSFXType, sizeof(BYTE));
	file.Write( &pItem->m_SFX.m_bSRCType, sizeof(BYTE));
	file.Write( &pItem->m_dwSFXSRC, sizeof(DWORD));

	file.Write( &pItem->m_dwANISRC, sizeof(DWORD));
	file.Write( &pItem->m_SFX.m_dwLifeTime, sizeof(DWORD));

	file.Write( &pItem->m_SFX.m_fFollow, sizeof(FLOAT));
	file.Write( &pItem->m_SFX.m_fSizeX, sizeof(FLOAT));
	file.Write( &pItem->m_SFX.m_fSizeY, sizeof(FLOAT));

	if( pItem->m_SFX.m_bSRCType == SFXSRC_OBJ )
	{
		COBJItem *pOBJ = CItemBase::GetOBJItem(pItem->m_dwSFXSRC);
		MAPCLKINST::iterator itCI;

		nCount = INT(pItem->m_SFX.m_mapCLK.size());
		file.Write( &nCount, sizeof(int));
		for( itCI = pItem->m_SFX.m_mapCLK.begin(); itCI != pItem->m_SFX.m_mapCLK.end(); itCI++)
		{
			LPCLKINST pCI = (*itCI).second;

			MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find((*itCI).first);
			DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			file.Write( &dwID, sizeof(DWORD));

			itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurCL);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			file.Write( &dwID, sizeof(DWORD));

			itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurMESH);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			file.Write( &dwID, sizeof(DWORD));
		}

		MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find(pItem->m_SFX.m_dwACT);
		DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
		file.Write( &dwID, sizeof(DWORD));

		itSTR = pOBJ->m_mapNAME.find(pItem->m_SFX.m_dwANI);
		dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
		file.Write( &dwID, sizeof(DWORD));
	}
	else
	{
		nCount = 0;
		file.Write( &nCount, sizeof(int));

		file.Write( &pItem->m_SFX.m_dwACT, sizeof(DWORD));
		file.Write( &pItem->m_SFX.m_dwANI, sizeof(DWORD));
	}

	file.Write( &pItem->m_SFX.m_dwOBJFuncID, sizeof(DWORD));
	file.Write( &pItem->m_SFX.m_dwACTFuncID, sizeof(DWORD));
	file.Write( &pItem->m_SFX.m_dwANIFuncID, sizeof(DWORD));

	file.Write( &pItem->m_dwSANISRC, sizeof(DWORD));
	file.Write( &pItem->m_SFX.m_bSprayType, sizeof(BYTE));
	file.Write( &pItem->m_SFX.m_bZEnable, sizeof(BYTE));
	file.Write( &pItem->m_SFX.m_bZWrite, sizeof(BYTE));
	file.Write( &pItem->m_SFX.m_bStrict, sizeof(BYTE));
	file.Write( &pItem->m_SFX.m_bFollow, sizeof(BYTE));

	file.Write( &pItem->m_SFX.m_dwDESTBlend, sizeof(DWORD));
	file.Write( &pItem->m_SFX.m_dwSRCBlend, sizeof(DWORD));
	file.Write( &pItem->m_SFX.m_dwBlendOP, sizeof(DWORD));

	file.Write( &pItem->m_SFX.m_dwGTime, sizeof(DWORD));
	file.Write( &pItem->m_SFX.m_dwGCnt, sizeof(DWORD));

	file.Write( &pItem->m_SFX.m_vOutBound, sizeof(D3DXVECTOR3));
	file.Write( &pItem->m_SFX.m_vInBound, sizeof(D3DXVECTOR3));

	file.Write( &pItem->m_SFX.m_dwPLifeTime, sizeof(DWORD));
	file.Write( &pItem->m_SFX.m_dwERRTime, sizeof(DWORD));

	file.Write( &pItem->m_SFX.m_vERRScale, sizeof(D3DXVECTOR3));
	file.Write( &pItem->m_SFX.m_vERRPos, sizeof(D3DXVECTOR3));
	file.Write( &pItem->m_SFX.m_vERRRot, sizeof(D3DXVECTOR3));
	file.Write( &pItem->m_SFX.m_dwLodLevel, sizeof(DWORD));
}

void CMainFrame::BuildEXTMAP( CEXTMAPItem *pItem, CFile& file)
{
	CEXTMAPDoc *pDoc = (CEXTMAPDoc *) pItem->GetMainDocument();
	if(pDoc)
		pDoc->UpdateData();

	CString strPATH = file.GetFilePath();
	BYTE vDETAIL[256];
	BYTE bINDEX = 0;

	strPATH = strPATH.Left(strPATH.GetLength() - file.GetFileName().GetLength());
	file.Write( &pItem->m_bMAPType, sizeof(BYTE));
	file.Write( &pItem->m_HugeMAP.m_nUnitCountX, sizeof(int));
	file.Write( &pItem->m_HugeMAP.m_nUnitCountZ, sizeof(int));
	file.Write( &pItem->m_HugeMAP.m_nUnitLength, sizeof(int));
	file.Write( &pItem->m_HugeMAP.m_nTileLength, sizeof(int));
	file.Write( &pItem->m_HugeMAP.m_nCellCount, sizeof(int));
	file.Write( &pItem->m_HugeMAP.m_nTextureSize, sizeof(int));
	file.Write( &pItem->m_HugeMAP.m_nShadowPixel, sizeof(int));
	file.Write( &pItem->m_HugeMAP.m_vLIGHT, sizeof(D3DXVECTOR3));

	file.Write( &pItem->m_HugeMAP.m_bUseSHADER, sizeof(BYTE));
	file.Write( &pItem->m_HugeMAP.m_fIntensity, sizeof(FLOAT));
	file.Write( &pItem->m_HugeMAP.m_fSpecRange, sizeof(FLOAT));
	file.Write( &pItem->m_HugeMAP.m_fSpecMax, sizeof(FLOAT));

	int nCount = INT(pItem->m_vDETAIL.size());
	for( int i=0; i<nCount; i++)
	{
		CTextureItem *pTEX = CItemBase::GetTextureItem(pItem->m_vDETAIL[i]);

		if(pTEX)
		{
			vDETAIL[i] = bINDEX;
			bINDEX++;
		}
		else
			vDETAIL[i] = TILE_NULL;
	}

	file.Write( &bINDEX, sizeof(BYTE));
	for( i=0; i<nCount; i++)
		if( vDETAIL[i] != TILE_NULL )
		{
			file.Write( &pItem->m_vDETAILS[i], sizeof(FLOAT));
			file.Write( &pItem->m_vDETAIL[i], sizeof(DWORD));
		}

	nCount = pItem->m_HugeMAP.m_nUnitLength;
	vDETAIL[TILE_NULL] = TILE_NULL;

	FLOAT *pHEIGHT = new FLOAT[(nCount + 1) * (nCount + 1)];
	LPWORD pSHADOW = new WORD[nCount * nCount];
	LPBYTE pREGION = new BYTE[nCount * nCount];
	LPBYTE pDETAIL = new BYTE[nCount * nCount];

	nCount = pItem->m_HugeMAP.m_nUnitCountX * pItem->m_HugeMAP.m_nUnitCountZ;
	file.Write( pItem->m_HugeMAP.m_pENABLE, nCount * sizeof(BYTE));

	for( i=0; i<nCount; i++)
		if(pItem->m_HugeMAP.m_pENABLE[i])
		{
			LPDIRECT3DSURFACE9 pSHADE = NULL;
			LPDIRECT3DSURFACE9 pSKIN = NULL;
			LPDIRECT3DSURFACE9 pTILE = NULL;

			MAPMAPOBJ::iterator itOBJ;
			MAPMAPSFX::iterator itSFX;
			MAPMAPSND::iterator itSND;

			MAPMAPOBJ mapOBJ;
			MAPMAPSFX mapSFX;
			MAPMAPSND mapSND;

			MAPDWORD mapOBJID;
			MAPDWORD mapSFXID;

			VECTORSURFACE vSHADOW;
			CString strUNIT;

			strUNIT.Format( "%sEM%d_%d.tmu", strPATH, pItem->m_dwID, i);
			CFile unit( strUNIT, CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

			mapOBJ.clear();
			mapSFX.clear();
			mapSND.clear();

			mapOBJID.clear();
			mapSFXID.clear();
			vSHADOW.clear();

			pItem->LoadUNIT( &pSHADE, &pSKIN, &pTILE, &vSHADOW, pREGION, pDETAIL, pHEIGHT, i);
			pItem->LoadOBJ(
				&mapOBJ,
				&mapSFX,
				&mapSND,
				&mapOBJID,
				&mapSFXID, i);

			int nSIZE = (pItem->m_HugeMAP.m_nUnitLength + 1) * (pItem->m_HugeMAP.m_nUnitLength + 1);
			unit.Write( &nSIZE, sizeof(int));

			if( nSIZE > 0 )
				unit.Write( pHEIGHT, nSIZE * sizeof(FLOAT));

			nSIZE = pItem->m_HugeMAP.m_nUnitLength * pItem->m_HugeMAP.m_nUnitLength;
			unit.Write( &nSIZE, sizeof(int));

			if( nSIZE > 0 )
				unit.Write( pREGION, nSIZE * sizeof(BYTE));

			pItem->BuildSREGION( &vSHADOW, pSHADOW);
			unit.Write( &nSIZE, sizeof(int));

			if( nSIZE > 0 )
				unit.Write( pSHADOW, nSIZE * sizeof(WORD));

			unit.Write( &nSIZE, sizeof(int));
			for( int j=0; j<nSIZE; j++)
				unit.Write( &vDETAIL[pDETAIL[j]], sizeof(BYTE));

			nSIZE = INT(mapOBJ.size());
			unit.Write( &nSIZE, sizeof(int));

			for( itOBJ = mapOBJ.begin(); itOBJ != mapOBJ.end(); itOBJ++)
			{
				MAPDWORD::iterator itID = mapOBJID.find((*itOBJ).first);
				COBJItem *pOBJ = CItemBase::GetOBJItem((*itID).second);
				LPMAPOBJ pMAPOBJ = (*itOBJ).second;

				MAPATTRIBUTE::iterator itATTR;
				MAPCLKINST::iterator itCI;

				pItem->SyncOBJATTR( pOBJ, pMAPOBJ);
				unit.Write( &(*itOBJ).first, sizeof(DWORD));
				unit.Write( &(*itID).second, sizeof(DWORD));

				unit.Write( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
				unit.Write( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
				unit.Write( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

				unit.Write( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
				unit.Write( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
				unit.Write( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

				MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find(pMAPOBJ->m_dwCurACT);
				DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
				unit.Write( &dwID, sizeof(DWORD));

				itSTR = pOBJ->m_mapNAME.find(pMAPOBJ->m_dwCurANI);
				dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
				unit.Write( &dwID, sizeof(DWORD));

				int nCI = INT(pMAPOBJ->m_mapCLK.size());
				unit.Write( &nCI, sizeof(int));

				for( itCI = pMAPOBJ->m_mapCLK.begin(); itCI != pMAPOBJ->m_mapCLK.end(); itCI++)
				{
					LPCLKINST pCI = (*itCI).second;

					itSTR = pOBJ->m_mapNAME.find((*itCI).first);
					DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
					unit.Write( &dwID, sizeof(DWORD));

					itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurCL);
					dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
					unit.Write( &dwID, sizeof(DWORD));

					itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurMESH);
					dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
					unit.Write( &dwID, sizeof(DWORD));
				}

				int nATTR = (INT) pMAPOBJ->m_mapATTR.size();
				unit.Write( &nATTR, sizeof(int));

				for( itATTR = pMAPOBJ->m_mapATTR.begin(); itATTR != pMAPOBJ->m_mapATTR.end(); itATTR++)
				{
					itSTR = pOBJ->m_mapNAME.find((*itATTR).first);
					DWORD dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
					unit.Write( &dwID, sizeof(DWORD));

					unit.Write( &(*itATTR).second->m_bValueType, sizeof(BYTE));
					unit.Write( &(*itATTR).second->m_nSize, sizeof(int));
					unit.Write( (*itATTR).second->m_pValue, (*itATTR).second->m_nSize);
				}

				delete pMAPOBJ;
			}

			nSIZE = INT(mapSFX.size());
			unit.Write( &nSIZE, sizeof(int));

			for( itSFX = mapSFX.begin(); itSFX != mapSFX.end(); itSFX++)
			{
				MAPDWORD::iterator itID = mapSFXID.find((*itSFX).first);
				LPMAPSFX pMAPSFX = (*itSFX).second;

				unit.Write( &(*itSFX).first, sizeof(DWORD));
				unit.Write( &(*itID).second, sizeof(DWORD));
				unit.Write( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
				unit.Write( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
				unit.Write( &pMAPSFX->m_bBias, sizeof(BYTE));

				delete pMAPSFX;
			}

			nSIZE = INT(mapSND.size());
			unit.Write( &nSIZE, sizeof(int));

			for( itSND = mapSND.begin(); itSND != mapSND.end(); itSND++)
			{
				LPMAPSND pMAPSND = (*itSND).second;

				unit.Write( &(*itSND).first, sizeof(DWORD));
				unit.Write( &pMAPSND->m_dwMediaID, sizeof(DWORD));
				unit.Write( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));

				delete pMAPSND;
			}

			mapOBJ.clear();
			mapSFX.clear();
			mapSND.clear();

			mapOBJID.clear();
			mapSFXID.clear();

			if( pSKIN && pSHADE )
			{
				pItem->Blend( pSKIN, pSHADE, 0, 0);
				LPT3DDATA pDXT = CD3DImage::CreateDXT(
					m_3DDevice.m_pDevice,
					pSKIN, DXT1);

				unit.Write( &pDXT->m_dwSize, sizeof(DWORD));
				unit.Write( pDXT->m_pData, pDXT->m_dwSize);

				delete pDXT;
			}
			else
			{
				DWORD dwLength = 0;
				unit.Write( &dwLength, sizeof(DWORD));
			}

			if(pTILE)
			{
				LPT3DDATA pDXT = CD3DImage::CreateDXT(
					m_3DDevice.m_pDevice,
					pTILE, DXT1);

				unit.Write( &pDXT->m_dwSize, sizeof(DWORD));
				unit.Write( pDXT->m_pData, pDXT->m_dwSize);

				delete pDXT;
			}
			else
			{
				DWORD dwLength = 0;
				unit.Write( &dwLength, sizeof(DWORD));
			}

			nSIZE = INT(vSHADOW.size());
			unit.Write( &nSIZE, sizeof(int));

			for( j=0; j<nSIZE; j++)
			{
				LPT3DDATA pDXT = CD3DImage::CreateDXT(
					m_3DDevice.m_pDevice,
					vSHADOW[j], DXT1);
				vSHADOW[j]->Release();

				unit.Write( &pDXT->m_dwSize, sizeof(DWORD));
				unit.Write( pDXT->m_pData, pDXT->m_dwSize);

				delete pDXT;
			}
			vSHADOW.clear();

			if(pTILE)
				pTILE->Release();

			if(pSHADE)
				pSHADE->Release();

			if(pSKIN)
				pSKIN->Release();
		}

	delete[] pHEIGHT;
	delete[] pSHADOW;
	delete[] pREGION;
	delete[] pDETAIL;
}

void CMainFrame::BuildBSPMAP( CBSPMAPItem *pItem, CFile& file)
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) pItem->GetMainDocument();

	MAPMAPLIGHT::iterator itLIGHT;
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;
	MAPDWORD::iterator itID;

	if(pDoc)
	{
		pDoc->ClearSELLIGHT();
		pDoc->ClearSELOBJ();
		pDoc->ClearSELSFX();
		pDoc->ClearSELSND();
	}

	pItem->LoadDATA();
	file.Write( &pItem->m_bMAPType, sizeof(BYTE));
	pItem->m_BSPMAP.SaveMAP(&file);

	int nCount = INT(pItem->m_vTEX.size());
	file.Write( pItem->m_BSPMAP.m_pRegionID, pItem->m_BSPMAP.m_dwMeshCount * sizeof(BYTE));
	file.Write( &nCount, sizeof(int));

	for( int i=0; i<nCount; i++)
	{
		int nTEXCount = INT(pItem->m_vTEX[i]->size());
		file.Write( &nTEXCount, sizeof(int));

		for( itID = pItem->m_vTEX[i]->begin(); itID != pItem->m_vTEX[i]->end(); itID++)
		{
			file.Write( &(*itID).first, sizeof(DWORD));
			file.Write( &(*itID).second, sizeof(DWORD));
		}
	}

	nCount = INT(pItem->m_mapLIGHT.size());
	file.Write( &nCount, sizeof(int));

	for( itLIGHT = pItem->m_mapLIGHT.begin(); itLIGHT != pItem->m_mapLIGHT.end(); itLIGHT++)
	{
		file.Write( &(*itLIGHT).first, sizeof(DWORD));
		file.Write( &(*itLIGHT).second->m_dwTextureID, sizeof(DWORD));
	}

	nCount = INT(pItem->m_BSPMAP.m_mapOBJ.size());
	file.Write( &nCount, sizeof(int));

	for( itOBJ = pItem->m_BSPMAP.m_mapOBJ.begin(); itOBJ != pItem->m_BSPMAP.m_mapOBJ.end(); itOBJ++)
	{
		LPMAPOBJ pMAPOBJ = (*itOBJ).second;
		DWORD dwID = (*itOBJ).first;
		itID = pItem->m_mapOBJ.find(dwID);

		COBJItem *pOBJ = CItemBase::GetOBJItem((*itID).second);
		MAPATTRIBUTE::iterator itATTR;
		MAPCLKINST::iterator itCI;

		pItem->SyncOBJATTR( pOBJ, pMAPOBJ);
		file.Write( &dwID, sizeof(DWORD));
		file.Write( &(*itID).second, sizeof(DWORD));

		file.Write( &pMAPOBJ->m_vSCALE, sizeof(D3DXVECTOR3));
		file.Write( &pMAPOBJ->m_vPOS, sizeof(D3DXVECTOR3));
		file.Write( &pMAPOBJ->m_vROT, sizeof(D3DXVECTOR3));

		file.Write( &pMAPOBJ->m_dwOBJFuncID, sizeof(DWORD));
		file.Write( &pMAPOBJ->m_dwACTFuncID, sizeof(DWORD));
		file.Write( &pMAPOBJ->m_dwANIFuncID, sizeof(DWORD));

		MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find(pMAPOBJ->m_dwCurACT);
		dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
		file.Write( &dwID, sizeof(DWORD));

		itSTR = pOBJ->m_mapNAME.find(pMAPOBJ->m_dwCurANI);
		dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
		file.Write( &dwID, sizeof(DWORD));

		int nCI = INT(pMAPOBJ->m_mapCLK.size());
		file.Write( &nCI, sizeof(int));

		for( itCI = pMAPOBJ->m_mapCLK.begin(); itCI != pMAPOBJ->m_mapCLK.end(); itCI++)
		{
			LPCLKINST pCI = (*itCI).second;

			itSTR = pOBJ->m_mapNAME.find((*itCI).first);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			file.Write( &dwID, sizeof(DWORD));

			itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurCL);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			file.Write( &dwID, sizeof(DWORD));

			itSTR = pOBJ->m_mapNAME.find(pCI->m_dwCurMESH);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			file.Write( &dwID, sizeof(DWORD));
		}

		int nATTR = (INT) pMAPOBJ->m_mapATTR.size();
		file.Write( &nATTR, sizeof(int));

		for( itATTR = pMAPOBJ->m_mapATTR.begin(); itATTR != pMAPOBJ->m_mapATTR.end(); itATTR++)
		{
			itSTR = pOBJ->m_mapNAME.find((*itATTR).first);
			dwID = itSTR != pOBJ->m_mapNAME.end() ? CItemBase::GetID((*itSTR).second) : 0;
			file.Write( &dwID, sizeof(DWORD));

			file.Write( &(*itATTR).second->m_bValueType, sizeof(BYTE));
			file.Write( &(*itATTR).second->m_nSize, sizeof(int));
			file.Write( (*itATTR).second->m_pValue, (*itATTR).second->m_nSize);
		}
	}

	nCount = INT(pItem->m_BSPMAP.m_mapSFX.size());
	file.Write( &nCount, sizeof(int));

	for( itSFX = pItem->m_BSPMAP.m_mapSFX.begin(); itSFX != pItem->m_BSPMAP.m_mapSFX.end(); itSFX++)
	{
		LPMAPSFX pMAPSFX = (*itSFX).second;

		itID = pItem->m_mapSFX.find((*itSFX).first);
		file.Write( &(*itSFX).first, sizeof(DWORD));
		file.Write( &(*itID).second, sizeof(DWORD));

		file.Write( &pMAPSFX->m_vPOS, sizeof(D3DXVECTOR3));
		file.Write( &pMAPSFX->m_vROT, sizeof(D3DXVECTOR3));
		file.Write( &pMAPSFX->m_bBias, sizeof(BYTE));
	}

	nCount = INT(pItem->m_BSPMAP.m_mapSND.size());
	file.Write( &nCount, sizeof(int));

	for( itSND = pItem->m_BSPMAP.m_mapSND.begin(); itSND != pItem->m_BSPMAP.m_mapSND.end(); itSND++)
	{
		LPMAPSND pMAPSND = (*itSND).second;

		file.Write( &(*itSND).first, sizeof(DWORD));
		file.Write( &pMAPSND->m_dwMediaID, sizeof(DWORD));
		file.Write( &pMAPSND->m_vPOS, sizeof(D3DXVECTOR3));
	}

	pItem->ClearDATA();
}

void CMainFrame::PrintText( CStdioFile& file, CString strText)
{
	CString strValue;

	strValue.Format( "\"%s\"", strText);
	file.WriteString(strValue);
}

void CMainFrame::PrintText( CStdioFile& file,
						    CString strTag,
							CString strText)
{
	CString strValue;

	strValue.Format( "%s=\"%s\"", strTag, strText);
	file.WriteString(strValue);
}

void CMainFrame::PrintInt( CStdioFile& file, int nData)
{
	CString strValue;

	strValue.Format( "%lu", nData);
	file.WriteString(strValue);
}

void CMainFrame::PrintInt( CStdioFile& file,
						   CString strTag,
						   int nData)
{
	CString strValue;

	strValue.Format( "%s=%lu", strTag, nData);
	file.WriteString(strValue);
}

void CMainFrame::PrintDWORD( CStdioFile& file,
							 CString strTag,
							 DWORD dwData)
{
	CString strValue;

	strValue.Format( "%s=%lu", strTag, dwData);
	file.WriteString(strValue);
}

void CMainFrame::PrintOpenTag( CStdioFile& file, CString strTag)
{
	CString strValue;

	strValue.Format( "<%s>", strTag);
	file.WriteString(strValue);
}

void CMainFrame::PrintCloseTag( CStdioFile& file, CString strTag)
{
	CString strValue;

	strValue.Format( "</%s>", strTag);
	file.WriteString(strValue);
}

void CMainFrame::PrintNewLine( CStdioFile& file, int nCount)
{
	for( int i=0; i<nCount; i++)
	{
		CString strValue;

		strValue.Format("\n");
		file.WriteString(strValue);
	}
}

void CMainFrame::PrintTab( CStdioFile& file, int nCount)
{
	for( int i=0; i<nCount; i++)
	{
		CString strValue;

		strValue.Format("\t");
		file.WriteString(strValue);
	}
}

void CMainFrame::PrintTime( CStdioFile& file)
{
	CTime time = CTime::GetCurrentTime();
	CString strValue;

	strValue.Format(
		"/* Tachyon Component Markup Language ( %d/%d/%d %02d:%02d ) */",
		time.GetYear(),
		time.GetMonth(),
		time.GetDay(),
		time.GetHour(),
		time.GetMinute());

	file.WriteString(strValue);
}

CString CMainFrame::GetAlign( BYTE bAlign)
{
	switch(bAlign)
	{
	case GSTA_LEFT		: return _T("ALIGN_LEFT"); break;
	case GSTA_CENTER	: return _T("ALIGN_CENTER"); break;
	case GSTA_RIGHT		: return _T("ALIGN_RIGHT"); break;
	}

	return _T("");
}

void CMainFrame::PrintID( CStdioFile& file,
						  CString strID,
						  DWORD dwID)
{
	CString strValue;

	strValue.Format( "<DEFINE %s = %d>", strID, dwID);
	file.WriteString(strValue);
}

void CMainFrame::PrintCTRL( CStdioFile& file,
						    LPCTRL pCTRL,
							LPCOMP pCOMP,
							BOOL bFRAME,
							int nTab)
{
	CCOMPItem *pItem = CItemBase::GetCOMPItem(pCOMP->m_dwCompID);
	CString strCTRL(_T(""));

	CString strNotify[TCML_MENU_COUNT] = {
		ATAG_LCLICK,
		ATAG_RCLICK,
		ATAG_DBLCLICK,
		ATAG_LINEUP,
		ATAG_LINEDOWN,
		ATAG_VSCROLL,
		ATAG_HSCROLL,
		ATAG_SEL_CHANGE,
		ATAG_TAB_ACTIVE,
		ATAG_ENTER,
		ATAG_ESC};

	switch(pItem->m_bCtrlType)
	{
	case CT_NONE		:
		if(!pItem->m_vCOMP.empty())
		{
			int nCount = INT(pItem->m_vCOMP.size());

			for( int i=0; i<nCount; i++)
			{
				PrintCTRL(
					file,
					pCTRL,
					pItem->m_vCOMP[i],
					FALSE,
					nTab);
			}

			return;
		}
		else
			strCTRL.Format(HTAG_COMPONENT);

		break;

	case CT_BUTTON		: strCTRL.Format(HTAG_BUTTON); break;
	case CT_SCROLL		: strCTRL.Format(HTAG_SCROLL); break;
	case CT_EDIT		: strCTRL.Format(HTAG_EDIT); break;
	case CT_LIST		: strCTRL.Format(HTAG_LIST); break;
	case CT_COMBO		: strCTRL.Format(HTAG_COMBO); break;
	case CT_GAUGE		: strCTRL.Format(HTAG_GAUGE); break;
	case CT_IMGLIST		: strCTRL.Format(HTAG_IMAGELIST); break;
	case CT_METER		: strCTRL.Format(HTAG_METER); break;
	case CT_TABCTRL		: strCTRL.Format(HTAG_TABCTRL); break;
	}

	CImageItem *pDefault = CItemBase::GetImageItem(pItem->m_dwDefaultID);
	CImageItem *pDisable = CItemBase::GetImageItem(pItem->m_dwDisableID);
	CString strDefaultID(_T(""));
	CString strDisableID(_T(""));
	CString strAlign(_T(""));

	if(bFRAME)
		strAlign = GetAlign(pCTRL->m_bAlign);

	if(pDefault)
		strDefaultID.Format( "ID_IMAGE_%d", pDefault->m_dwID);

	if(pDisable)
		strDisableID.Format( "ID_IMAGE_%d", pDisable->m_dwID);

	PrintTab( file, nTab);
	file.WriteString(MTAG_OPENBRACE);
	file.WriteString(strCTRL);
	file.WriteString(MTAG_SPACE);

	PrintText( file, ATAG_ID, bFRAME ? pCTRL->m_strCtrlID : pItem->m_strID);
	file.WriteString(MTAG_SPACE);

	if(!strDefaultID.IsEmpty())
	{
		PrintText( file, ATAG_DEFAULT, strDefaultID);
		file.WriteString(MTAG_SPACE);
	}

	if(!strDisableID.IsEmpty())
	{
		PrintText( file, ATAG_DISABLE, strDisableID);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && !pCTRL->m_strText.IsEmpty() )
	{
		PrintText( file, ATAG_TEXT, pCTRL->m_strText);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && !pCTRL->m_strToolTip.IsEmpty() )
	{
		PrintText( file, ATAG_TOOLTIP, pCTRL->m_strToolTip);
		file.WriteString(MTAG_SPACE);
	}

	if(!strAlign.IsEmpty())
	{
		PrintText( file, ATAG_ALIGN, strAlign);
		file.WriteString(MTAG_SPACE);
	}

	if( pItem->m_nHMargine > 0 )
	{
		PrintInt( file, ATAG_HMARGINE, pItem->m_nHMargine);
		file.WriteString(MTAG_SPACE);
	}

	if( pItem->m_nVMargine > 0 )
	{
		PrintInt( file, ATAG_VMARGINE, pItem->m_nVMargine);
		file.WriteString(MTAG_SPACE);
	}

	if( pItem->m_nWidth > 0 )
	{
		PrintInt( file, ATAG_WIDTH, pItem->m_nWidth);
		file.WriteString(MTAG_SPACE);
	}

	if( pItem->m_nHeight > 0 )
	{
		PrintInt( file, ATAG_HEIGHT, pItem->m_nHeight);
		file.WriteString(MTAG_SPACE);
	}

	if( pCOMP->m_nPosX > 0 )
	{
		PrintInt( file, ATAG_POSX, pCOMP->m_nPosX);
		file.WriteString(MTAG_SPACE);
	}

	if( pCOMP->m_nPosY > 0 )
	{
		PrintInt( file, ATAG_POSY, pCOMP->m_nPosY);
		file.WriteString(MTAG_SPACE);
	}

	if( pCOMP->m_dwMediaID > 0 )
	{
		PrintInt( file, ATAG_SOUND, pCOMP->m_dwMediaID);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && !pCTRL->m_strFontID.IsEmpty() )
	{
		PrintText( file, ATAG_FONT, pCTRL->m_strFontID);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && !pCTRL->m_strTipID.IsEmpty() )
	{
		PrintText( file, ATAG_TIPFONT, pCTRL->m_strTipID);
		file.WriteString(MTAG_SPACE);
	}

	if( bFRAME && (pCTRL->m_dwFlag&TS_CUSTOM_COLOR) )
	{
		PrintInt( file, ATAG_COLOR, pCTRL->m_dwColor);
		file.WriteString(MTAG_SPACE);
	}

	if(bFRAME)
	{
		for( int i=0; i<TCML_MENU_COUNT; i++)
			if(!pCTRL->m_strMenuID[i].IsEmpty())
			{
				PrintText( file, strNotify[i], pCTRL->m_strMenuID[i]);
				file.WriteString(MTAG_SPACE);
			}
	}

	switch(pItem->m_bCtrlType)
	{
	case CT_METER		:
		if( pItem->m_nLevel > 0 )
		{
			PrintInt( file, ATAG_METER_SUPERLEVEL, pItem->m_nLevel);
			file.WriteString(MTAG_SPACE);
		}

		break;

	case CT_EDIT		:
		if( bFRAME && (pCTRL->m_dwFlag&TES_PASSWORD) )
		{
			PrintInt( file, ATAG_EDIT_PASSWORD, 1);
			file.WriteString(MTAG_SPACE);
		}

		break;

	case CT_BUTTON		:
		if( bFRAME && (pCTRL->m_dwFlag&TBS_STATE_BUTTON) )
		{
			PrintInt( file, ATAG_BUTTON_STATEBUTTON, 1);
			file.WriteString(MTAG_SPACE);
		}

		break;
	}

	int nCount = INT(pItem->m_vCOMP.size());
	for( int i=0; i<nCount; i++)
	{
		CString strType[3] = {
			_T(""),
			_T(""),
			_T("")};

		switch(pItem->m_bCtrlType)
		{
		case CT_NONE		:
		case CT_IMGLIST		:
		case CT_TABCTRL		: break;
		case CT_BUTTON		:
			{
				strType[0].Format(TAG_BUTTON_DOWN);
				strType[1].Format(TAG_BUTTON_UP);
				strType[2].Format(TAG_BUTTON_HOVER);
			}

			break;

		case CT_SCROLL		:
			{
				strType[0].Format(TAG_SCROLL_UL);
				strType[1].Format(TAG_SCROLL_DR);
				strType[2].Format(TAG_SCROLL_SCROLL);
			}

			break;

		case CT_EDIT		:
			{
				strType[0].Format(TAG_EDIT_CARET);
			}

			break;

		case CT_LIST		:
			{
				strType[0].Format(TAG_LIST_ITEM);
				strType[1].Format(TAG_LIST_VERTICAL);
			}

			break;

		case CT_COMBO		:
			{
				strType[0].Format(TAG_COMBO_DROP);
				strType[1].Format(TAG_COMBO_DOWNLIST);
			}

			break;

		case CT_GAUGE		:
			{
				strType[0].Format(TAG_GAUGE_BAR);
			}

			break;

		case CT_METER		:
			{
				strType[0].Format(TAG_METER_SUB);
				strType[1].Format(TAG_METER_SUPER);
			}

			break;
		}

		if( pItem->m_vCOMP[i]->m_bUseage > 0 )
		{
			CCOMPItem *pComp = CItemBase::GetCOMPItem(pItem->m_vCOMP[i]->m_dwCompID);

			PrintText( file, strType[pItem->m_vCOMP[i]->m_bUseage - 1], pComp->m_strID);
			file.WriteString(MTAG_SPACE);
		}
	}

	file.WriteString(MTAG_CLOSEBRACE);
	PrintNewLine(file);

	for( i=0; i<nCount; i++)
	{
		PrintCTRL(
			file,
			pCTRL,
			pItem->m_vCOMP[i],
			FALSE,
			nTab + 1);
	}

	PrintTab( file, nTab);
	PrintCloseTag( file, strCTRL);
	PrintNewLine(file);
}

void CMainFrame::PrintFrame( CStdioFile& file, CFRAMEItem *pFRAME)
{
	CImageItem *pDefault = CItemBase::GetImageItem(pFRAME->m_dwDefaultID);
	CImageItem *pDisable = CItemBase::GetImageItem(pFRAME->m_dwDisableID);
	CString strDefaultID(_T(""));
	CString strDisableID(_T(""));

	if(pDefault)
		strDefaultID.Format( "ID_IMAGE_%d", pDefault->m_dwID);

	if(pDisable)
		strDisableID.Format( "ID_IMAGE_%d", pDisable->m_dwID);

	file.WriteString(MTAG_OPENBRACE);
	file.WriteString(HTAG_FRAME);
	file.WriteString(MTAG_SPACE);

	PrintText( file, ATAG_ID, pFRAME->m_strID);
	file.WriteString(MTAG_SPACE);

	if(!strDefaultID.IsEmpty())
	{
		PrintText( file, ATAG_DEFAULT, strDefaultID);
		file.WriteString(MTAG_SPACE);
	}

	if(!strDisableID.IsEmpty())
	{
		PrintText( file, ATAG_DISABLE, strDisableID);
		file.WriteString(MTAG_SPACE);
	}

	if( pFRAME->m_nWidth > 0 )
	{
		PrintInt( file, ATAG_WIDTH, pFRAME->m_nWidth);
		file.WriteString(MTAG_SPACE);
	}

	if( pFRAME->m_nHeight > 0 )
	{
		PrintInt( file, ATAG_HEIGHT, pFRAME->m_nHeight);
		file.WriteString(MTAG_SPACE);
	}

	if(!pFRAME->m_strFontID.IsEmpty())
	{
		PrintText( file, ATAG_FONT, pFRAME->m_strFontID);
		file.WriteString(MTAG_SPACE);
	}

	file.WriteString(MTAG_CLOSEBRACE);
	PrintNewLine(file);

	int nCount = INT(pFRAME->m_vCTRL.size());
	for( int i=0; i<nCount; i++)
	{
		COMP vCOMP;

		vCOMP.m_dwCompID = pFRAME->m_vCTRL[i]->m_dwCompID;
		vCOMP.m_nPosX = pFRAME->m_vCTRL[i]->m_nPosX;
		vCOMP.m_nPosY = pFRAME->m_vCTRL[i]->m_nPosY;
		vCOMP.m_bUseage = 0;

		PrintCTRL(
			file,
			pFRAME->m_vCTRL[i],
			&vCOMP,
			TRUE, 1);
	}

	PrintCloseTag( file, HTAG_FRAME);
	PrintNewLine( file, 2);
}

void CMainFrame::PrintFontType( CStdioFile& file,
								LPGSFONT pFONT)
{
	file.WriteString(MTAG_OPENBRACE);
	file.WriteString(ATAG_FONT);
	file.WriteString(MTAG_SPACE);

	PrintText( file, ATAG_ID, pFONT->m_strFontID);
	file.WriteString(MTAG_SPACE);

	PrintText( file, ATAG_FACENAME, pFONT->m_FONT.lfFaceName);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_WIDTH, pFONT->m_FONT.lfWidth);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_HEIGHT, pFONT->m_FONT.lfHeight);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_ESCAPEMENT, pFONT->m_FONT.lfEscapement);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_ORIENTATION, pFONT->m_FONT.lfOrientation);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_WEIGHT, pFONT->m_FONT.lfWeight);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_ITALIC, pFONT->m_FONT.lfItalic);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_UNDERLINE, pFONT->m_FONT.lfUnderline);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_STRIKEOUT, pFONT->m_FONT.lfStrikeOut);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_CHARSET, pFONT->m_FONT.lfCharSet);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_OUTPRECISION, pFONT->m_FONT.lfOutPrecision);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_CLIPPRECISION, pFONT->m_FONT.lfClipPrecision);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_QUALITY, pFONT->m_FONT.lfQuality);
	file.WriteString(MTAG_SPACE);

	PrintInt( file, ATAG_PITCHANDFAMILY, pFONT->m_FONT.lfPitchAndFamily);
	file.WriteString(MTAG_SPACE);

	PrintDWORD( file, ATAG_FACECLR, pFONT->m_dwColor);
	file.WriteString(MTAG_SPACE);
	file.WriteString(MTAG_CLOSEBRACE);
}

void CMainFrame::BuildTSC( LPBUILDTHREADPARAM lParam, CString strTITLE)
{
	CString strPATH = lParam->m_strPATH;

	strPATH.TrimRight('\\');
	strPATH = strPATH + _T('\\') + strTITLE + _T(".tsc");

	CStdioFile file(
		strPATH,
		CFile::modeCreate|
		CFile::modeWrite|
		CFile::typeText);

	PrintTime(file);
	PrintNewLine( file, 2);

	PrintOpenTag( file, HTAG_TCML);
	PrintNewLine( file, 2);

	PrintOpenTag( file, HTAG_HEAD);
	PrintNewLine(file);

	MAPITEM::iterator it;
	for( it = CItemBase::m_mapImageItem.begin(); it != CItemBase::m_mapImageItem.end(); it++)
	{
		CString strImageID;
		strImageID.Format( "ID_IMAGE_%d", (*it).second->m_dwID);

		PrintTab(file);
		PrintID( file, strImageID, (*it).second->m_dwID);
		PrintNewLine(file);
	}
	PrintNewLine(file);

	for( it = CItemBase::m_mapCompItem.begin(); it != CItemBase::m_mapCompItem.end(); it++)
	{
		PrintTab(file);
		PrintID( file, (*it).second->m_strID, (*it).second->m_dwID);
		PrintNewLine(file);
	}
	PrintNewLine(file);

	for( it = CItemBase::m_mapFrameItem.begin(); it != CItemBase::m_mapFrameItem.end(); it++)
	{
		CFRAMEItem *pFRAME = (CFRAMEItem *) (*it).second;

		if(pFRAME)
		{
			int nCount = INT(pFRAME->m_vCTRL.size());

			for( int i=0; i<nCount; i++)
			{
				PrintTab(file);
				PrintID( file, pFRAME->m_vCTRL[i]->m_strCtrlID, CItemBase::GetID(pFRAME->m_vCTRL[i]->m_strCtrlID));
				PrintNewLine(file);
			}
		}
	}
	PrintNewLine(file);

	for( it = CItemBase::m_mapFrameItem.begin(); it != CItemBase::m_mapFrameItem.end(); it++)
	{
		PrintTab(file);
		PrintID( file, (*it).second->m_strID, (*it).second->m_dwID);
		PrintNewLine(file);
	}
	PrintNewLine(file);

	MAPSTR::iterator itMENU;
	for( itMENU = CItemBase::m_mapMenuID.begin(); itMENU != CItemBase::m_mapMenuID.end(); itMENU++)
	{
		PrintTab(file);
		PrintID( file, (*itMENU).second, (*itMENU).first);
		PrintNewLine(file);
	}
	PrintNewLine(file);

	MAPGSFONT::iterator itFONT;
	for( itFONT = CItemBase::m_mapFont.begin(); itFONT != CItemBase::m_mapFont.end(); itFONT++)
	{
		PrintTab(file);
		PrintID( file, (*itFONT).second->m_strFontID, (*itFONT).first);
		PrintNewLine(file);
	}
	PrintNewLine(file);

	for( itFONT = CItemBase::m_mapFont.begin(); itFONT != CItemBase::m_mapFont.end(); itFONT++)
	{
		PrintTab(file);
		PrintFontType( file, (*itFONT).second);
		PrintNewLine(file);
	}
	PrintCloseTag( file, HTAG_HEAD);
	PrintNewLine( file, 2);

	for( it = CItemBase::m_mapFrameItem.begin(); it != CItemBase::m_mapFrameItem.end(); it++)
		PrintFrame( file, (CFRAMEItem *) (*it).second);

	PrintCloseTag( file, HTAG_TCML);
}

void CMainFrame::LoadTextureFromResource( DWORD dwResourceID, LPDIRECT3DTEXTURE9 *ppTexture)
{
	HRSRC hResInfo = FindResource( NULL, MAKEINTRESOURCE(dwResourceID), "TEXTURE");
	HGLOBAL hGlobal = LoadResource( NULL, hResInfo);

	D3DXCreateTextureFromFileInMemory(
		m_3DDevice.m_pDevice,
		LockResource(hGlobal),
		SizeofResource( NULL, hResInfo),
		ppTexture);
}

void CMainFrame::LoadMeshFromResource( DWORD dwResourceID, CTachyonMesh *pMesh)
{
	HRSRC hResInfo = FindResource( NULL, MAKEINTRESOURCE(dwResourceID), "MESH");
	HGLOBAL hGlobal = LoadResource( NULL, hResInfo);

	pMesh->LoadMesh(
		m_3DDevice.m_pDevice,
		(LPBYTE) LockResource(hGlobal));
}

void CMainFrame::OnEditmenu()
{
	CEditMenuDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnEditfont()
{
	CEditFontDlg dlg;
	dlg.DoModal();
}

CString CMainFrame::GetFileTitle()
{
	CString strTitle = m_strFileName;

	if(!strTitle.IsEmpty())
	{
		CTShell dlg;

		strTitle = dlg.GetFileName(LPCSTR(m_strFileName));
	}
	else
		strTitle.LoadString(IDS_NONAME);

	return strTitle;
}

void CMainFrame::SetActiveWorkspace( INT nItemType )
{
	m_wndWorkSpace.SetActiveView( nItemType );
}

void CMainFrame::SetPaneText( CString strText, int nIndex)
{
	m_wndStatusBar.SetPaneText(
		nIndex,
		LPCTSTR(strText));
}

void CMainFrame::OnEditFov()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CEditFovDlg dlg;
	
	FLOAT fFOV = m_fFOV;
	fFOV *= 180.0f / D3DX_PI;
    dlg.m_strFOV.Format( "%.2f", fFOV );
    
	if( dlg.DoModal() == IDOK )
	{
		FLOAT fFOV = (FLOAT) atof( dlg.m_strFOV );
		if( fFOV == 0 )
		{
			fFOV = 1;
		}
		fFOV *= D3DX_PI / 180.0f;
		CTachyonDoc::UpdateFOV(fFOV);

		m_fFOV = fFOV;
	}
}

void CMainFrame::OnNotuseVb()
{
	CGameStudioView *pView = (CGameStudioView *) m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CTreeCtrl& cTree = pView->GetTreeCtrl();
		HTREEITEM itemPos = cTree.GetSelectedItem();

		ResetUseVB( &cTree, itemPos, FALSE);
	}
}

void CMainFrame::OnUpdateNotuseVb( CCmdUI *pCmdUI)
{
	CView *pView = m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc *pDoc = (CGameStudioDoc *) pView->GetDocument();
		pCmdUI->Enable(pDoc && pDoc->m_bType == ITEM_MESH ? TRUE : FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnResetVb()
{
	CGameStudioView *pView = (CGameStudioView *) m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CTreeCtrl& cTree = pView->GetTreeCtrl();
		HTREEITEM itemPos = cTree.GetSelectedItem();

		ResetUseVB( &cTree, itemPos, TRUE);
	}
}

void CMainFrame::OnUpdateResetVb( CCmdUI *pCmdUI)
{
	CView *pView = m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc *pDoc = (CGameStudioDoc *) pView->GetDocument();
		pCmdUI->Enable(pDoc && pDoc->m_bType == ITEM_MESH ? TRUE : FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::ResetUseVB( CTreeCtrl *pTree,
							 HTREEITEM itemPos,
							 BYTE bUseVB)
{
	if(!itemPos)
		return;

	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	if( pItem && pItem->m_bType == ITEM_MESH )
		((CMeshItem *) pItem)->m_Mesh.m_bUseVB = bUseVB;

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		ResetUseVB( pTree, itemPos, bUseVB);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::OnMenuUpdatesize()
{
	CGameStudioView *pView = (CGameStudioView *) m_wndWorkSpace.GetActiveView();

	while(!CTachyonDoc::m_vDOC.empty())
		CTachyonDoc::m_vDOC.back()->OnCloseDocument();

	if(pView)
	{
		CTreeCtrl& cTree = pView->GetTreeCtrl();
		HTREEITEM itemPos = cTree.GetSelectedItem();

		UpdateSizeAttr( &cTree, itemPos);
	}
}

void CMainFrame::UpdateSizeAttr( CTreeCtrl *pTree,
								 HTREEITEM itemPos)
{
	if(!itemPos)
		return;

	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	if( pItem && pItem->m_bType == ITEM_OBJECT )
	{
		COBJItem *pTOBJ = (COBJItem *) pItem;
		int nCollType = 0;

		CString strCollTypeID(_T("ID_COLLISION_TYPE"));
		DWORD dwCollTypeID = pTOBJ->GetAttributeID(strCollTypeID);

		if( dwCollTypeID != -1 )
		{
			MAPATTRITEM::iterator itATTR = pTOBJ->m_mapATTR.find(dwCollTypeID);

			if( itATTR != pTOBJ->m_mapATTR.end() && (*itATTR).second )
				nCollType = atoi((*itATTR).second->m_strValue);
		}

		if( nCollType != 1 && nCollType != 2 )
		{
			MAPDWORD::iterator itID;

			FLOAT fSizeX = 0.0f;
			FLOAT fSizeY = 0.0f;
			FLOAT fSizeZ = 0.0f;
			FLOAT fDepth = 0.0f;

			for( itID = pTOBJ->m_mapMESH.begin(); itID != pTOBJ->m_mapMESH.end(); itID++)
			{
				CMeshItem *pMESH = CItemBase::GetMeshItem((*itID).second);

				if( pMESH->m_Mesh.m_dwNodeCount > 0 )
				{
					LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pMESH->m_Mesh.m_pDATA->m_pVB;

					for( int i=0; i<pMESH->m_Mesh.m_pDATA->m_dwCount; i++)
					{
						FLOAT fX = fabs(pDATA[i].m_fPosX);
						FLOAT fZ = fabs(pDATA[i].m_fPosZ);

						fSizeX = max( fSizeX, fX);
						fSizeZ = max( fSizeZ, fZ);

						if( pDATA[i].m_fPosY < 0.0f )
							fDepth = max( fDepth, -pDATA[i].m_fPosY);
						else
							fSizeY = max( fSizeY, pDATA[i].m_fPosY);
					}
				}
				else
				{
					LPMESHVERTEX pDATA = (LPMESHVERTEX) pMESH->m_Mesh.m_pDATA->m_pVB;

					for( int i=0; i<pMESH->m_Mesh.m_pDATA->m_dwCount; i++)
					{
						FLOAT fX = fabs(pDATA[i].m_fPosX);
						FLOAT fZ = fabs(pDATA[i].m_fPosZ);

						fSizeX = max( fSizeX, fX);
						fSizeZ = max( fSizeZ, fZ);

						if( pDATA[i].m_fPosY < 0.0f )
							fDepth = max( fDepth, -pDATA[i].m_fPosY);
						else
							fSizeY = max( fSizeY, pDATA[i].m_fPosY);
					}
				}
			}

			fSizeX *= 2.0f;
			fSizeZ *= 2.0f;

			if( fSizeX > 0.0f )
			{
				CString strID(_T("ID_SIZE_X"));
				DWORD dwID = pTOBJ->GetAttributeID(strID);

				if( dwID == -1 )
				{
					LPATTRITEM pATTR = new ATTRITEM();

					if(!pTOBJ->m_mapATTR.empty())
					{
						MAPATTRITEM::iterator it = pTOBJ->m_mapATTR.end();
						it--;

						dwID = (*it).first + 1;
					}
					else
						dwID = ROOT_ATTRID;

					pTOBJ->m_mapATTR.insert( MAPATTRITEM::value_type( dwID, pATTR));
					pTOBJ->m_mapNAME.insert( MAPSTR::value_type( dwID, strID));

					CItemBase::RegLocalID( strID, pTOBJ);
				}

				MAPATTRITEM::iterator itATTR = pTOBJ->m_mapATTR.find(dwID);
				if( itATTR != pTOBJ->m_mapATTR.end() )
				{
					(*itATTR).second->m_strValue.Format( "%.2f", fSizeX + 0.0099f);
					(*itATTR).second->m_bType = ATTRTYPE_FLOAT;
				}
			}

			if( fSizeY > 0.0f )
			{
				CString strID(_T("ID_SIZE_Y"));
				DWORD dwID = pTOBJ->GetAttributeID(strID);

				if( dwID == -1 )
				{
					LPATTRITEM pATTR = new ATTRITEM();

					if(!pTOBJ->m_mapATTR.empty())
					{
						MAPATTRITEM::iterator it = pTOBJ->m_mapATTR.end();
						it--;

						dwID = (*it).first + 1;
					}
					else
						dwID = ROOT_ATTRID;

					pTOBJ->m_mapATTR.insert( MAPATTRITEM::value_type( dwID, pATTR));
					pTOBJ->m_mapNAME.insert( MAPSTR::value_type( dwID, strID));

					CItemBase::RegLocalID( strID, pTOBJ);
				}

				MAPATTRITEM::iterator itATTR = pTOBJ->m_mapATTR.find(dwID);
				if( itATTR != pTOBJ->m_mapATTR.end() )
				{
					(*itATTR).second->m_strValue.Format( "%.2f", fSizeY + 0.0099f);
					(*itATTR).second->m_bType = ATTRTYPE_FLOAT;
				}
			}

			if( fSizeZ > 0.0f )
			{
				CString strID(_T("ID_SIZE_Z"));
				DWORD dwID = pTOBJ->GetAttributeID(strID);

				if( dwID == -1 )
				{
					LPATTRITEM pATTR = new ATTRITEM();

					if(!pTOBJ->m_mapATTR.empty())
					{
						MAPATTRITEM::iterator it = pTOBJ->m_mapATTR.end();
						it--;

						dwID = (*it).first + 1;
					}
					else
						dwID = ROOT_ATTRID;

					pTOBJ->m_mapATTR.insert( MAPATTRITEM::value_type( dwID, pATTR));
					pTOBJ->m_mapNAME.insert( MAPSTR::value_type( dwID, strID));

					CItemBase::RegLocalID( strID, pTOBJ);
				}

				MAPATTRITEM::iterator itATTR = pTOBJ->m_mapATTR.find(dwID);
				if( itATTR != pTOBJ->m_mapATTR.end() )
				{
					(*itATTR).second->m_strValue.Format( "%.2f", fSizeZ + 0.0099f);
					(*itATTR).second->m_bType = ATTRTYPE_FLOAT;
				}
			}

			if( fDepth > 0.0f )
			{
				CString strID(_T("ID_DEPTH"));
				DWORD dwID = pTOBJ->GetAttributeID(strID);

				if( dwID == -1 )
				{
					LPATTRITEM pATTR = new ATTRITEM();

					if(!pTOBJ->m_mapATTR.empty())
					{
						MAPATTRITEM::iterator it = pTOBJ->m_mapATTR.end();
						it--;

						dwID = (*it).first + 1;
					}
					else
						dwID = ROOT_ATTRID;

					pTOBJ->m_mapATTR.insert( MAPATTRITEM::value_type( dwID, pATTR));
					pTOBJ->m_mapNAME.insert( MAPSTR::value_type( dwID, strID));

					CItemBase::RegLocalID( strID, pTOBJ);
				}

				MAPATTRITEM::iterator itATTR = pTOBJ->m_mapATTR.find(dwID);
				if( itATTR != pTOBJ->m_mapATTR.end() )
				{
					(*itATTR).second->m_strValue.Format( "%.2f", fDepth + 0.0099f);
					(*itATTR).second->m_bType = ATTRTYPE_FLOAT;
				}
			}
		}
	}

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		UpdateSizeAttr( pTree, itemPos);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::OnUpdateMenuUpdatesize( CCmdUI *pCmdUI)
{
	CView *pView = m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc *pDoc = (CGameStudioDoc *) pView->GetDocument();
		pCmdUI->Enable(pDoc && pDoc->m_bType == ITEM_OBJECT ? TRUE : FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::ResetTextureOption( CTreeCtrl *pTree,
									 HTREEITEM itemPos,
									 BYTE bTextureOption)
{
	if(!itemPos)
		return;

	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	if( pItem && pItem->m_bType == ITEM_TEXTURE )
		((CTextureItem *) pItem)->m_bTextureOption = bTextureOption;

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		ResetTextureOption( pTree, itemPos, bTextureOption);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::OnMenuTexhi()
{
	CGameStudioView *pView = (CGameStudioView *) m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CTreeCtrl& cTree = pView->GetTreeCtrl();
		HTREEITEM itemPos = cTree.GetSelectedItem();

		ResetTextureOption( &cTree, itemPos, TEXTURE_DETAIL_HI);
	}
}

void CMainFrame::OnUpdateMenuTexhi( CCmdUI *pCmdUI)
{
	CView *pView = m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc *pDoc = (CGameStudioDoc *) pView->GetDocument();
		pCmdUI->Enable(pDoc && pDoc->m_bType == ITEM_TEXTURE ? TRUE : FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnMenuTexmid()
{
	CGameStudioView *pView = (CGameStudioView *) m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CTreeCtrl& cTree = pView->GetTreeCtrl();
		HTREEITEM itemPos = cTree.GetSelectedItem();

		ResetTextureOption( &cTree, itemPos, TEXTURE_DETAIL_MED);
	}
}

void CMainFrame::OnUpdateMenuTexmid( CCmdUI *pCmdUI)
{
	CView *pView = m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc *pDoc = (CGameStudioDoc *) pView->GetDocument();
		pCmdUI->Enable(pDoc && pDoc->m_bType == ITEM_TEXTURE ? TRUE : FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnMenuTexlow()
{
	CGameStudioView *pView = (CGameStudioView *) m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CTreeCtrl& cTree = pView->GetTreeCtrl();
		HTREEITEM itemPos = cTree.GetSelectedItem();

		ResetTextureOption( &cTree, itemPos, TEXTURE_DETAIL_LOW);
	}
}

void CMainFrame::OnUpdateMenuTexlow( CCmdUI *pCmdUI)
{
	CView *pView = m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc *pDoc = (CGameStudioDoc *) pView->GetDocument();
		pCmdUI->Enable(pDoc && pDoc->m_bType == ITEM_TEXTURE ? TRUE : FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnMenuAutolod()
{
	CGameStudioView *pView = (CGameStudioView *) m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CTreeCtrl& cTree = pView->GetTreeCtrl();
		HTREEITEM itemPos = cTree.GetSelectedItem();

		AutoLOD( &cTree, itemPos);
	}
}

void CMainFrame::OnUpdateMenuAutolod( CCmdUI *pCmdUI)
{
	CView *pView = m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc *pDoc = (CGameStudioDoc *) pView->GetDocument();
		pCmdUI->Enable(pDoc && pDoc->m_bType == ITEM_MESH ? TRUE : FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::AutoLOD( CTreeCtrl *pTree,
						  HTREEITEM itemPos)
{
	if(!itemPos)
		return;

	CItemBase *pItem = (CItemBase *) pTree->GetItemData(itemPos);
	if( pItem && pItem->m_bType == ITEM_MESH )
		((CMeshItem *) pItem)->AutoLOD();

	itemPos = pTree->GetChildItem(itemPos);
	while(itemPos)
	{
		AutoLOD( pTree, itemPos);
		itemPos = pTree->GetNextSiblingItem(itemPos);
	}
}

void CMainFrame::OnMenuLocExp()
{
	CGameStudioView *pView = (CGameStudioView *) m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc* pDoc = (CGameStudioDoc *) pView->GetDocument();
		if( pDoc && pDoc->m_bType == ITEM_FRAME )
			pDoc->ExportLoc();
	}
}

void CMainFrame::OnUpdateMenuLocExp(CCmdUI *pCmdUI)
{
	CView *pView = m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc *pDoc = (CGameStudioDoc *) pView->GetDocument();
		pCmdUI->Enable(pDoc && pDoc->m_bType == ITEM_FRAME ? TRUE : FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnMenuLocImp()
{
	CGameStudioView *pView = (CGameStudioView *) m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc* pDoc = (CGameStudioDoc *) pView->GetDocument();
		if( pDoc && pDoc->m_bType == ITEM_FRAME )
			pDoc->ImportLoc();
	}
}

void CMainFrame::OnUpdateMenuLocImp(CCmdUI *pCmdUI)
{
	CView *pView = m_wndWorkSpace.GetActiveView();

	if(pView)
	{
		CGameStudioDoc *pDoc = (CGameStudioDoc *) pView->GetDocument();
		pCmdUI->Enable(pDoc && pDoc->m_bType == ITEM_FRAME ? TRUE : FALSE);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CMainFrame::OnMenuListUpdate()
{
	CString strBasePath;
	CTShell shell;

	shell.m_strTitle = _T("Base Folder");
	if( shell.BrowseForFolder(this) != IDOK )
		return;

	strBasePath = shell.m_strPath;
	strBasePath.TrimRight(_T("\\"));

	CFileDialog dlg(
		FALSE,
		_T("*.txt"),
		NULL,
		OFN_READONLY,
		_T("Text Files (*.txt)|*.txt|All Files (*.*)|*.*||"));

	if( dlg.DoModal() != IDOK )
		return;

	CStdioFile file;
	if( FALSE == file.Open(dlg.GetPathName(), CStdioFile::modeRead ))
		return;

	CString strLINE;
	file.ReadString(strLINE);

	while(file.ReadString(strLINE))
	{
		strLINE.TrimRight();
		strLINE.TrimLeft();

		if(!strLINE.IsEmpty())
		{
			CString strTYPE;
			CString strPATH;
			CString strID;
			DWORD dwID;

			int nPOS = 0;

			strID = strLINE.Tokenize( _T("\t"), nPOS);
			strTYPE = strLINE.Tokenize( _T("\t"), nPOS);
			strPATH = strLINE.Tokenize( _T("\t"), nPOS);

			strTYPE.TrimRight();
			strTYPE.TrimLeft();

			strPATH.TrimRight();
			strPATH.TrimLeft();
			strPATH.TrimLeft(_T("\\"));

			strID.TrimRight();
			strID.TrimLeft();

			strPATH = strBasePath + CString("\\") + strPATH;
			dwID = atoi(LPCSTR(strID));

			if( strTYPE == "ITEM_TEXTURE" )
			{
				CItemBase::TextureItemUpdateByID(
					dwID,
					strPATH);
			}
			else if( strTYPE == "ITEM_IMAGE" )
			{
				CItemBase::ImageItemUpdateByID(
					dwID,
					strPATH);
			}
			else if( strTYPE == "ITEM_MEDIA")
			{
				CItemBase::MediaItemUpdateByID(
					dwID,
					strPATH);
			}
			else if( strTYPE == "ITEM_ANIMATION")
			{
				CItemBase::AnimationItemUpdateByID(
					dwID,
					strPATH);
			}
			else if( strTYPE == "ITEM_MESH" )
			{
				CItemBase::MeshItemUpdateByID(
					dwID,
					strPATH);
			}
		}
	}
}

void CMainFrame::OnItemFinder()
{
	if( m_pFinderDlg != NULL )
	{
		m_pFinderDlg->SetFocus();
	}
	else
	{
		m_pFinderDlg = new CItemFinder;
		m_pFinderDlg->Create( IDD_DIALOG_ITEMFINDER, this );
		m_pFinderDlg->ShowWindow( SW_SHOW );
	}
}

void CMainFrame::OnItemScreenShot()
{
	if( m_pScreenShotDlg != NULL )
	{
		m_pScreenShotDlg->SetFocus();
	}
	else
	{
		m_pScreenShotDlg = new CItemScreenShot;
		m_pScreenShotDlg->Create( IDD_DIALOG_ITEMSCREENSHOT, NULL );
		m_pScreenShotDlg->ShowWindow( SW_SHOW );
	}
}

void CMainFrame::OnFileSetTemppath()
{
	CString strFolderName;

	strFolderName.LoadString(IDS_TEMPFOLDER_GAMESTUDIO);
	if(CItemBase::m_strTempPath.IsEmpty())
	{
		char szTempPath[_MAX_PATH];
		CString strTempPath;

		ZeroMemory( szTempPath, _MAX_PATH);
		GetTempPath(
			_MAX_PATH,
			szTempPath);

		int nCurPos = 0;
		strTempPath.Format( "%s", szTempPath);

		strTempPath = strTempPath.Tokenize( "\\", nCurPos);
		CItemBase::m_strTempPath.Format( "%s\\%s\\", strTempPath, strFolderName);
	}

	if( CItemBase::m_bCanTempPath && GetItemCount() == 0 )
	{
		CTShell dlg;

		dlg.m_strTitle = _T("Game Studio Temp Folder");
		dlg.m_strSelDir = CItemBase::m_strTempPath;
		dlg.m_strSelDir.TrimRight("\\");
		dlg.m_strSelDir = dlg.m_strSelDir.Left(dlg.m_strSelDir.GetLength() - strFolderName.GetLength());

		if( dlg.BrowseForFolder(this) == IDOK )
		{
			dlg.m_strPath.TrimRight("\\");
			CItemBase::m_strTempPath.Format( "%s\\%s\\", dlg.m_strPath, strFolderName);
		}
	}

	mkdir(LPCSTR(CItemBase::m_strTempPath));
}

void CMainFrame::OnUpdateFileSetTemppath( CCmdUI *pCmdUI)
{
	pCmdUI->Enable( CItemBase::m_bCanTempPath && GetItemCount() == 0 ? TRUE : FALSE);
}

int CMainFrame::GetItemCount()
{
	CGameStudioDoc *pDOC[10] = {
		m_pDocAnimation,
		m_pDocTexture,
		m_pDocObject,
		m_pDocImage,
		m_pDocMedia,
		m_pDocFrame,
		m_pDocComp,
		m_pDocMesh,
		m_pDocSFX,
		m_pDocMAP};
	int nCount = 0;

	for( int i=0; i<10; i++)
	{
		CTreeCtrl& cTree = pDOC[i]->GetTreeCtrl();
		nCount += cTree.GetCount();
	}

	return nCount;
}
