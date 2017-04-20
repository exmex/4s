// MainFrm.cpp : CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "TFontViewer.h"
#include "MainFrm.h"
#include "TFontTestView.h"
#include "TFontDialogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 상태 줄 표시기
		ID_INDICATOR_CAPS,
		ID_INDICATOR_NUM,
		ID_INDICATOR_SCRL,
};


// CMainFrame 생성/소멸
LPDIRECT3DSWAPCHAIN9	CMainFrame::m_pSWAPCHAIN	= NULL;
CD3DDevice				CMainFrame::m_3DDevice;
BYTE					CMainFrame::m_bRender		= TRUE;
FLOAT					CMainFrame::m_fFOV			= 1.0f;

CMainFrame::CMainFrame()
: m_pFontDialog(NULL)
, m_pFontView(NULL)
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	InitDevice();

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("도구 모음을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("상태 표시줄을 만들지 못했습니다.\n");
		return -1;      // 만들지 못했습니다.
	}
	// TODO: 도구 모음을 도킹할 수 없게 하려면 이 세 줄을 삭제하십시오.
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY); 
	DockControlBar(&m_wndToolBar);

	D3DMATERIAL9 mtrl;
	ZeroMemory( &mtrl, sizeof(D3DMATERIAL9));
	mtrl.Diffuse.r = mtrl.Ambient.r = 1.0f;
	mtrl.Diffuse.g = mtrl.Ambient.g = 1.0f;
	mtrl.Diffuse.b = mtrl.Ambient.b = 1.0f;
	mtrl.Diffuse.a = mtrl.Ambient.a = 1.0f;
	m_3DDevice.m_pDevice->SetMaterial(&mtrl);

	m_3DDevice.m_pDevice->SetSoftwareVertexProcessing(FALSE);
	m_3DDevice.m_pDevice->SetVertexShader(NULL);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	cs.style =  WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		| WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_MAXIMIZE | WS_SYSMENU;


	return TRUE;
}


// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

//--------------------------------------------------------------------------
// OnCreateClient 재정의
// 뷰 나누기
//--------------------------------------------------------------------------
BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	BOOL bResult = m_SplitWnd[0].CreateStatic( this, 2, 1);

	if(bResult)
	{
		m_SplitWnd[0].CreateView( 0, 0, RUNTIME_CLASS(CTFontDialogView), CSize(10,10), pContext );		// 다이얼로그 뷰
		m_SplitWnd[0].CreateView( 1, 0, RUNTIME_CLASS(CTFontTestView), CSize(10,10), pContext );		// 프레임 뷰
	}

	//// 뷰(윈도우) 포인터 얻기
	m_pFontDialog	= (CTFontDialogView*)	m_SplitWnd[0].GetPane( 0, 0 );
	m_pFontView = (CTFontTestView*)	m_SplitWnd[0].GetPane( 1, 0 );

	// 뷰(윈도우) 크기 재 설정
	ResizeSplitWnd();

	return TRUE;
}

//--------------------------------------------------------------------------
// ResizeSplitWnd
// 분할된 화면 크기 재 설정
//--------------------------------------------------------------------------
void CMainFrame::ResizeSplitWnd()
{
	// DialogBar 가로 크기
	m_SplitWnd[0].SetRowInfo( 0, 165, 100 );

	// 화면 크기 재 계산
	m_SplitWnd[0].RecalcLayout();

}

void CMainFrame::ReleaseDevice()
{
	if(m_pSWAPCHAIN)
	{
		m_pSWAPCHAIN->Release();
		m_pSWAPCHAIN = NULL;
	}

	m_3DDevice.ReleaseDevice();
}

void CMainFrame::RestoreDevice()
{
	LPDIRECT3DSURFACE9 pBACK = NULL;

	if(m_pSWAPCHAIN)
	{
		m_pSWAPCHAIN->Release();
		m_pSWAPCHAIN = NULL;
	}

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
}

void CMainFrame::InitDevice()
{
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

	m_3DDevice.InitDevices(pDeskTop);

	m_3DDevice.m_pDevice->CreateAdditionalSwapChain(
		&m_3DDevice.m_vPRESENT,
		&m_pSWAPCHAIN);

	m_pSWAPCHAIN->GetBackBuffer( 0, D3DBACKBUFFER_TYPE_MONO, &pBACK);
	m_3DDevice.m_pDevice->SetRenderTarget( 0, pBACK);
	pBACK->Release();

	CD3DLight::SetDevice(m_3DDevice.m_pDevice);

}

BOOL CMainFrame::LoadImageFile(const CString& strFILE, CD3DImage* pIMG)
{
	CFile vFILE;
	if( vFILE.Open( strFILE, CFile::modeRead|CFile::typeBinary) )
	{
		DWORD dwSIZE = DWORD(vFILE.GetLength());
		if( dwSIZE )
		{
			LPBYTE pDATA = new BYTE[dwSIZE];
			vFILE.Read( pDATA, dwSIZE);

			pIMG->LoadImageFile(
				m_3DDevice.m_pDevice,
				pDATA,
				dwSIZE);

			delete [] pDATA;
			return TRUE;
		}
	}

	return FALSE;
}

void CMainFrame::SetFont( LOGFONT* of, BOOL bOutline, BOOL bShadow, DWORD dwColor, INT nFontRender, CRect rt )
{
	m_pFontView->SetFont( of, bOutline, bShadow, dwColor, nFontRender, rt );
}

void CMainFrame::SetText( CString str )
{
	m_pFontView->SetText( str ) ;
}

void CMainFrame::LoadImage( CString str )
{
	m_pFontView->LoadImage( str );
}