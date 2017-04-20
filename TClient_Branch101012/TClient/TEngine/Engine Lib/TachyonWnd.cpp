// TachyonWnd.cpp: implementation of the CTachyonWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "THtmlDocWnd.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// New Arab Ver.
CString CTachyonWnd::m_strFont = " ";

BEGIN_MESSAGE_MAP( CTachyonWnd, CWnd)
	ON_WM_ACTIVATE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_NCHITTEST()
	ON_WM_NCLBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_NCRBUTTONDOWN()
END_MESSAGE_MAP()


CTachyonWnd::CTachyonWnd()
{
	m_bActivate = FALSE;
	m_bOnMovie = FALSE;
	m_bNcMove = FALSE;

	m_pMainFrame = NULL;
	m_pTParser = NULL;

	m_vNcPOS = CPoint( 0, 0);
	m_vCOMMAND.clear();

	m_bDirectLogin = FALSE;
	m_bSiteCode = 0xFF;

	m_hDEVICE = NULL;
	m_bRUN = FALSE;

	m_hRESET = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);

	m_hINIT = CreateEvent(
		NULL,
		FALSE,
		FALSE,
		NULL);
}

CTachyonWnd::~CTachyonWnd()
{
	if(m_pTParser)
	{
		delete m_pTParser;
		m_pTParser = NULL;
	}

	CloseHandle(m_hRESET);
	CloseHandle(m_hINIT);

	FinishTachyonComponent();
}

BOOL CALLBACK CTachyonWnd::__Present( HWND hWnd, LPARAM lParam)
{
	CTachyonWnd *pMAIN = (CTachyonWnd *) lParam;
	CWnd *pWND = FromHandlePermanent(hWnd);
	CRect rect;

	if( !pMAIN || !pWND || !pWND->IsWindowVisible() || pWND->GetWindowedChildCount() )
		return TRUE;

	pWND->GetClientRect(&rect);
	pWND->ClientToScreen(&rect);
	pMAIN->ScreenToClient(&rect);

	if(FAILED(pMAIN->m_Device.m_pDevice->Present( &rect, NULL, hWnd, NULL)))
		return FALSE;

	return TRUE;
}

DWORD WINAPI CTachyonWnd::__DeviceCtrl( LPVOID lpParam)
{
	CTachyonWnd *pMainWnd = (CTachyonWnd *) lpParam;

	if( !pMainWnd || !pMainWnd->InitDevices() )
	{
		SetEvent(pMainWnd->m_hINIT);
		return 0;
	}
	SetEvent(pMainWnd->m_hINIT);

	while(TRUE)
	{
		WaitForSingleObject( pMainWnd->m_hRESET, INFINITE);

		if(!pMainWnd->GetDeviceThreadFlag())
			break;

		pMainWnd->ResetDevices();
		pMainWnd->m_Device.SetResetFlag(FALSE);
	}

	return 0;
}

void CTachyonWnd::SetDeviceThreadFlag( BYTE bRUN)
{
	SMART_LOCKCS(&m_Device.m_cs);
	m_bRUN = bRUN;
}

BYTE CTachyonWnd::GetDeviceThreadFlag()
{
	SMART_LOCKCS(&m_Device.m_cs);
	return m_bRUN;
}

void CTachyonWnd::ReleaseThreadDevices()
{
	if(m_hDEVICE)
	{
		SetDeviceThreadFlag(FALSE);
		SetEvent(m_hRESET);

		WaitForSingleObject(
			m_hDEVICE,
			INFINITE);
	}

	m_Device.m_bUseTHREAD = FALSE;
	m_hDEVICE = NULL;
}

BOOL CTachyonWnd::InitThreadDevices()
{
	ReleaseThreadDevices();
	DWORD dwThread = 0;

	SetDeviceThreadFlag(TRUE);
	m_hDEVICE = CreateThread(
		NULL, 0,
		__DeviceCtrl,
		this, 0,
		&dwThread);

	if(!m_hDEVICE)
		return FALSE;

	WaitForSingleObject(
		m_hINIT,
		INFINITE);

	if(!m_Device.m_pDevice)
	{
		ReleaseThreadDevices();
		return FALSE;
	}
	m_Device.m_bUseTHREAD = TRUE;

	return TRUE;
}

BOOL CTachyonWnd::ResetThreadDevices()
{
	if(m_hDEVICE)
	{
		m_Device.SetResetFlag(TRUE);
		SetEvent(m_hRESET);
	}

	return TRUE;
}

BOOL CTachyonWnd::Create()
{
	DWORD dwStyle = WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN;
	CString strTitle;
	CRect rect(
		0, 0,
		m_Device.m_option.m_dwScreenX,
		m_Device.m_option.m_dwScreenY);

	if(m_Device.m_option.m_bWindowedMode)
		dwStyle |= WS_OVERLAPPED|WS_CAPTION;
	strTitle.LoadString(AFX_IDS_APP_TITLE);

	::AdjustWindowRect(
		&rect,
		dwStyle,
		FALSE);

	rect.OffsetRect(
		-rect.left,
		-rect.top);

	BOOL bResult = CWnd::CreateEx(
		NULL,
		AfxRegisterWndClass( CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS),
		strTitle,
		dwStyle,
		rect,
		NULL,
		NULL);

#ifdef USE_CONSOLE
	CTConsole* pConsole = CTConsole::GetInstance();
	pConsole->Create("Output", true);
#endif

	if(!m_Device.m_option.m_bWindowedMode)
	{
		SetWindowPos( &wndTopMost, 0, 0, 0, 0, SWP_NOMOVE|SWP_NOREDRAW|SWP_NOSIZE);
		MoveWindow(&rect);
	}

	return bResult;
}

void CTachyonWnd::OnDestroy()
{
	if(m_pMainFrame)
	{
		delete m_pMainFrame;
		m_pMainFrame = NULL;
	}

	CTachyonMesh::ReleaseGlobalVB();
	CTachyonSFX::ReleaseCommon();
	CSFX::ReleaseTEXVB();

	THtmlDocWnd::ReleaseHook();
	THttpCtrl::ReleaseBUF();
	ReleaseThreadDevices();

	m_bActivate = FALSE;
	m_Font.DeleteObject();
	m_FontFPS.Release();
	m_Device.ReleaseDevice();
	m_vCOMMAND.clear();

	CWnd::OnDestroy();
}

BOOL CTachyonWnd::InitDevices()
{
	if(!m_Device.InitDevices(this))
		return FALSE;
	CD3DLight::SetDevice(m_Device.m_pDevice);

	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_Device.m_pDevice->SetMaterial(&mtrl);
	m_Device.m_pDevice->SetVertexShader(NULL);
	// New Arab Ver.
	m_Font.CreateFont(
		14, 0, 0, 0, 0,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		m_strFont);
/*
#if defined(GERMANY) || defined(ENGLISH) || defined(JAPAN)
		_T("Arial"));
#else
		_T("2002L"));
#endif
*/

	m_FontFPS.SetFont(&m_Font);
	m_bActivate = TRUE;

	CTachyonObject::m_pMedia = &CTachyonRes::m_MEDIA;
	CTachyonMesh::InitGlobalVB();

	CTachyonRes::m_MEDIA.InitMEDIA(GetSafeHwnd());
	CTachyonRes::m_pDEVICE = &m_Device;
	CTachyonSFX::InitCommon();
	CSFX::InitTEXVB();

	return TRUE;
}

BOOL CTachyonWnd::AdjustWindowSize( INT nWidth, INT nHeight)
{
	CRect rect(
		0, 0,
		nWidth,
		nHeight );

	DWORD dwStyle = WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN;

	if(m_Device.m_option.m_bWindowedMode)
		dwStyle |= WS_OVERLAPPED|WS_CAPTION;

	::AdjustWindowRect(
		&rect,
		dwStyle,
		FALSE);

	rect.OffsetRect(
		-rect.left,
		-rect.top);

	MoveWindow(&rect);

	return TRUE;
}

BOOL CTachyonWnd::Present()
{
	if(FAILED(m_Device.m_pDevice->Present( NULL, NULL, NULL, NULL)))
		return FALSE;

	if( m_Device.m_option.m_bWindowedMode && GetWindowedChildCount() &&
		!EnumChildWindows( GetSafeHwnd(), __Present, (LPARAM) this))
		return FALSE;
	THttpCtrl::m_bRedrawHTTP = FALSE;

	return TRUE;
}

BOOL CTachyonWnd::ResetDevices()
{
	if( m_Device.m_pD3D && m_Device.m_pDevice )
		if( m_Device.Reset() == FALSE )
			return FALSE;

	return TRUE;
}

void CTachyonWnd::ProcessHotCommand( int nID)
{
}

void CTachyonWnd::Render()
{
}

void CTachyonWnd::DrawFPS( int nPosX, int nPosY)
{
	static DWORD dwPrevTick = ::GetTickCount();
	static DWORD dwCount = 0;
	static CString strFPS(_T(""));

	DWORD dwTick = ::GetTickCount();

	if( dwTick - dwPrevTick > 1000 )
	{
		while( dwTick - dwPrevTick > 1000 )
			dwPrevTick += 1000;

		strFPS.Format( _T("[FPS : %d]"), dwCount);
		dwCount = 0;
	}
	else
		dwCount++;

	CString strText;
	CString strParticle;
	strParticle.Format( "\n[Particle : %d]", CTachyonSFX::m_dwParticleNum );

	strText = strFPS;// + strParticle;

	m_FontFPS.TextOut(
		m_Device.m_pDevice,
		strText,
		nPosX,
		nPosY);
}

void CTachyonWnd::OnCommand( TCOMMAND cmd)
{
}

void CTachyonWnd::OnSessionMsg( CTachyonSession *pSession, CPacket *pPacket)
{
}

void CTachyonWnd::OnConnect( CTachyonSession *pSession, int nErrorCode)
{
}

void CTachyonWnd::OnClose( CTachyonSession *pSession, int nErrorCode)
{
}

class CTachyonTCMLParserProgress : public TCMLParserProgress
{
public:
	virtual void OnProgress(float percent)
	{
		CTachyonRes::RenderBACK( 9, (INT)(percent * 100) );
	}
};

void CTachyonWnd::InitResource( CString strGroupID,
							    BYTE bGroupID)
{
	m_RES.Init( strGroupID, bGroupID);

	if(m_pTParser)
		delete m_pTParser;

	CTachyonTCMLParserProgress progress;
	progress.OnProgress(0.0f);

	m_pTParser = new TCMLParser();
	m_pTParser->Load((char *) LPCTSTR(strGroupID + _T("Cmd.TIF")), &progress );

	InitTachyonComponent( &m_RES, &m_RES, m_pTParser);
	TFrame::m_pCommandHandler = this;
}

void CTachyonWnd::OnImeComposition( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(m_pMainFrame)
		m_pMainFrame->OnImeComposition( hWnd, wParam, lParam);
}

void CTachyonWnd::OnImeNotify( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	if(m_pMainFrame)
		m_pMainFrame->OnImeNotify( hWnd, wParam, lParam);
}

BOOL CTachyonWnd::OnInputLangChangeRequest( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	if(m_pMainFrame)
		return m_pMainFrame->OnInputLangChangeRequest( hWnd, wParam, lParam );
	return TRUE;
}

void CTachyonWnd::OnChar( UINT nChar, UINT nRep, UINT nFlags)
{
	if(m_pMainFrame)
		m_pMainFrame->OnChar(nChar, nRep, nFlags);
}

void CTachyonWnd::OnKeyDown( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(m_pMainFrame)
		m_pMainFrame->OnKeyDown( nChar, nRepCnt, nFlags);
}

void CTachyonWnd::OnKeyUp( UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if(m_pMainFrame)
		m_pMainFrame->OnKeyUp( nChar, nRepCnt, nFlags);
}

UINT CTachyonWnd::OnNcHitTest( CPoint point)
{
	UINT nHIT = CWnd::OnNcHitTest(point);

	if( nHIT == HTSYSMENU )
		nHIT = HTCAPTION;

	return nHIT;
}

void CTachyonWnd::OnNcLButtonDown( UINT nHitTest, CPoint point)
{
	if( nHitTest == HTCAPTION )
	{
		m_vNcPOS = point;
		m_bNcMove = TRUE;

		SetCapture();
	}

	CWnd::OnNcLButtonDown( nHitTest, point);
}

void CTachyonWnd::OnLButtonUp( UINT nFlags, CPoint point)
{
	if(m_bNcMove)
	{
		m_bNcMove = FALSE;
		ReleaseCapture();
	}

	CWnd::OnLButtonUp( nFlags, point);
}

void CTachyonWnd::OnMouseMove( UINT nFlags, CPoint point)
{
	if(m_bNcMove)
	{
		CPoint screen = point;
		CRect rect;

		ClientToScreen(&screen);
		GetWindowRect(&rect);

		screen -= m_vNcPOS;
		m_vNcPOS += screen;

		rect.OffsetRect(
			screen.x,
			screen.y);

		MoveWindow(
			rect.left,
			rect.top,
			rect.Width(),
			rect.Height(),
			TRUE);
	}

	CWnd::OnMouseMove( nFlags, point);
}

void CTachyonWnd::OnActivate( UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate( nState, pWndOther, bMinimized);

	BYTE bBACK = GetForegroundWindow() == this ? FALSE : TRUE;
	if(m_bActivate)
	{
		if(m_pMainFrame)
			if(bBACK)
				m_pMainFrame->OnLoseFocus(NULL);
			else
				m_pMainFrame->OnSetFocus(NULL);

		if( CTachyonMedia::m_bBACK != bBACK )
			CTachyonRes::m_MEDIA.OnSetFocus(bBACK);
	}
	else if( CTachyonMedia::m_bBACK != bBACK )
		CTachyonMedia::m_bBACK = bBACK;

	if( !m_Device.m_option.m_bWindowedMode &&
		!bMinimized && bBACK )
		ShowWindow(SW_MINIMIZE);
}

void CTachyonWnd::OnNcRButtonDown( UINT nHitTest, CPoint point)
{
}
