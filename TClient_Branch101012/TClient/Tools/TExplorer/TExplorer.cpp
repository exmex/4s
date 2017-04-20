// TExplorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "TExplorer.h"
#include "TExplorerWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


WORD CTExplorerApp::m_vKEY[TKEY_COUNT] = {
	MAKEWORD( 7, 0),			// TKEY_NONE (Undefined VK, Do not save or load)
	MAKEWORD( 'W', 0),			// TKEY_FORWARD
	MAKEWORD( 'S', 0),			// TKEY_BACK
	MAKEWORD( 'A', 0),			// TKEY_LEFT
	MAKEWORD( 'D', 0),			// TKEY_RIGHT
	MAKEWORD( VK_SPACE, 0),		// TKEY_JUMP
	MAKEWORD( 'R', 0),			// TKEY_AUTORUN
	MAKEWORD( 'V', 0),			// TKEY_RUN
	MAKEWORD( VK_RBUTTON, 0),	// TKEY_ROT
	MAKEWORD( VK_LEFT, 0),		// TKEY_CAM_LEFT
	MAKEWORD( VK_RIGHT, 0),		// TKEY_CAM_RIGHT
	MAKEWORD( VK_UP, 0),		// TKEY_CAM_UP
	MAKEWORD( VK_DOWN, 0),		// TKEY_CAM_DOWN
	MAKEWORD( VK_LBUTTON, 0),	// TKEY_CAM_ROT
	MAKEWORD( VK_DELETE, 0),	// TKEY_ZOOM_IN
	MAKEWORD( VK_END, 0),		// TKEY_ZOOM_OUT
	MAKEWORD( 'C', 0),			// TKEY_ADDFLAG
	MAKEWORD( 'F', 0),			// TKEY_FINDPATH
	MAKEWORD( VK_RETURN, 0),	// TKEY_CHAT
	MAKEWORD( 'U', 1),				// TKEY_SHOW_UI
	MAKEWORD( 'G', 0)};			// TKEY_GETPOS_FOR_WRITE_FILE
	


CString CTExplorerApp::m_vKEYField[TKEY_COUNT] = {
	CString("TKEY_NONE"),
	CString("TKEY_FORWARD"),
	CString("TKEY_BACK"),
	CString("TKEY_LEFT"),
	CString("TKEY_RIGHT"),
	CString("TKEY_JUMP"),
	CString("TKEY_AUTORUN"),
	CString("TKEY_RUN"),
	CString("TKEY_ROT"),
	CString("TKEY_CAM_LEFT"),
	CString("TKEY_CAM_RIGHT"),
	CString("TKEY_CAM_UP"),
	CString("TKEY_CAM_DOWN"),
	CString("TKEY_CAM_ROT"),
	CString("TKEY_ZOOM_IN"),
	CString("TKEY_ZOOM_OUT"),
	CString("TKEY_ADDFLAG"),
	CString("TKEY_FINDPATH"),
	CString("TKEY_CHAT"),
	CString("TKEY_SHOW_UI"),
	CString("TKEY_GETPOS")};
DWORD CTExplorerApp::m_dwAppTick = 0;


// CTExplorerApp

BEGIN_MESSAGE_MAP(CTExplorerApp, CWinApp)
END_MESSAGE_MAP()


// CTExplorerApp construction

CTExplorerApp::CTExplorerApp()
{
	m_pGDI = NULL;
}


CTExplorerApp theApp;

BOOL CTExplorerApp::InitInstance()
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
	GdiplusStartupInput vGDIInput;

	AfxEnableControlContainer();
	CoInitialize(NULL);

	if( GdiplusStartup( &m_pGDI, &vGDIInput, NULL) != Ok )
		return FALSE;

	SetRegistryKey(_T("Tachyon Engine"));
	m_hAccel = LoadAccelerators(
		m_hInstance,
		MAKEINTRESOURCE(IDR_MAINFRAME));

	m_pTachyonWnd = new CTExplorerWnd();
	m_pMainWnd = m_pTachyonWnd;
	m_dwAppTick = GetTickCount();
	LoadStdProfileSettings();

	if(!m_pTachyonWnd->Create())
		return FALSE;

	memcpy( m_pTachyonWnd->m_Device.m_vConstantVS, vConstantVS, VC_COUNT * sizeof(WORD));
	memcpy( m_pTachyonWnd->m_Device.m_dwVertexShader, vVS, VS_COUNT * sizeof(DWORD));

	memcpy( m_pTachyonWnd->m_Device.m_vConstantPS, vConstantPS, PC_COUNT * sizeof(WORD));
	memcpy( m_pTachyonWnd->m_Device.m_dwPixelShader, vPS, PS_COUNT * sizeof(DWORD));
	m_pTachyonWnd->m_Device.m_strResourceType = "SHADER";

	m_pTachyonWnd->SetIcon( LoadIcon(IDR_MAINFRAME), TRUE);
	if(!m_pTachyonWnd->InitDevices())
		return FALSE;

	m_pTachyonWnd->InitResource( "TClient", 0);
	m_pTachyonWnd->ShowWindow(SW_SHOW);
	m_pTachyonWnd->UpdateWindow();

	return TRUE;
}

int CTExplorerApp::ExitInstance()
{
	GdiplusShutdown(m_pGDI);
	CoUninitialize();
	return CWinApp::ExitInstance();
}

void CTExplorerApp::LoadStdProfileSettings()
{
	LPMAPWORD pKEY = &((CTExplorerWnd *) m_pTachyonWnd)->m_MainGame.m_vKEY.m_mapKEY;
	pKEY->clear();
	CTachyonApp::LoadStdProfileSettings();

	for( WORD i=1; i<TKEY_COUNT; i++)
	{
		WORD wKEY = (WORD) GetProfileInt(
			_T("Keys"),
			m_vKEYField[i],
			m_vKEY[i]);

		pKEY->insert( MAPWORD::value_type( wKEY, i));
	}

	((CTExplorerWnd *) m_pTachyonWnd)->m_MainGame.m_vKEY.ResetSTATE();
}

void CTExplorerApp::SaveStdProfileSettings()
{
	LPMAPWORD pKEY = &((CTExplorerWnd *) m_pTachyonWnd)->m_MainGame.m_vKEY.m_mapKEY;
	MAPWORD::iterator itWORD;
	CTachyonApp::SaveStdProfileSettings();

	for( itWORD = pKEY->begin(); itWORD != pKEY->end(); itWORD++)
	{
		WriteProfileInt(
			_T("Keys"),
			m_vKEYField[(*itWORD).second],
			(*itWORD).first);
	}
}
