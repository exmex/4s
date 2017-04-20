// GridView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "GSDialogBar.h"
#include "GridView.h"
#include "MainFrm.h"
#include ".\gridview.h"

#define NORMAL_COLOR		0xFF606060
#define THICK_COLOR			0xFF303030
#define BORDER_COLOR		0xFFFFFF00
#define ZERO_COLOR			0xFF000000
#define BACK_COLOR			0x80

#define Z_LEVEL				655.35f
#define Z_MIN				65.535f
#define GRID_COUNT			20.0f
#define NAV_NEAR			1.0f
#define NAV_FAR				6553.5f

#define ZOOM_UNIT			30.0f
#define BORDER_SIZE			3


// CGridView

IMPLEMENT_DYNCREATE(CGridView, CView)

CGridView::CGridView()
{
	D3DLIGHT9 light;

	ZeroMemory( &light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.2f;
	light.Diffuse.g = 0.3f;
	light.Diffuse.b = 0.5f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 0.1f;
	light.Ambient.g = 0.1f;
	light.Ambient.b = 0.1f;
	light.Ambient.a = 1.0f;
	light.Range = T3DLIGHT_RANGE_MAX;
	light.Direction = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);

	m_LightCenter.SetLight(&light);
	m_LightRight.SetLight(&light);
	m_LightTop.SetLight(&light);

	m_bViewPort = VIEWPORT_PERSPECTIVE;
	m_fGrid = 1.0f;

	m_pBackIMGTEX	= NULL;
	m_bBackIMG		= FALSE;
	m_pBackIMGDlg	= NULL;
	m_nFrameMode	= GSVIEW_NONE;
	m_dwFPS			= 0;
}

CGridView::~CGridView()
{
}

BEGIN_MESSAGE_MAP(CGridView, CView)
	ON_WM_ERASEBKGND()
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_COMMAND(ID_GRID_SHOW, OnGridShow)
	ON_UPDATE_COMMAND_UI(ID_GRID_SHOW, OnUpdateGridShow)
	ON_COMMAND(ID_GRID_TOP, OnGridTop)
	ON_UPDATE_COMMAND_UI(ID_GRID_TOP, OnUpdateGridTop)
	ON_COMMAND(ID_GRID_PERSPECTIVE, OnGridPerspective)
	ON_UPDATE_COMMAND_UI(ID_GRID_PERSPECTIVE, OnUpdateGridPerspective)
	ON_UPDATE_COMMAND_UI(ID_GRID_NAVIGATE, OnUpdateGridNavigate)
	ON_COMMAND(ID_GRID_LEFT, OnGridLeft)
	ON_UPDATE_COMMAND_UI(ID_GRID_LEFT, OnUpdateGridLeft)
	ON_COMMAND(ID_GRID_FRONT, OnGridFront)
	ON_UPDATE_COMMAND_UI(ID_GRID_FRONT, OnUpdateGridFront)
	ON_COMMAND(ID_GRID_CAMRESET, OnGridCamreset)
	ON_WM_SETCURSOR()
	ON_WM_KEYDOWN()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_BACKIMG_LOAD, OnBackimgLoad)
	ON_COMMAND(ID_BACKIMG_SHOW, OnBackimgShow)
	ON_UPDATE_COMMAND_UI(ID_BACKIMG_SHOW, OnUpdateBackimgShow)
	ON_COMMAND(ID_BACKIMAGE_SETBACKIMAGE, OnSetBackIMG)
	ON_UPDATE_COMMAND_UI(ID_BACKIMAGE_SETBACKIMAGE, OnUpdateSetBackIMG)
END_MESSAGE_MAP()


// CGridView 그리기입니다.

void CGridView::OnDraw( CDC* pDC)
{
	Render();
}

// CGridView 진단입니다.

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CView::AssertValid();
}

void CGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG


// CGridView 메시지 처리기입니다.

void CGridView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	m_FontAxis.CreateFont(
		-8, 0, 0, 0, FW_NORMAL,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		_T("2002L"));

	m_Font.CreateFont(
		-13, 0, 0, 0, FW_BOLD,
		FALSE,
		FALSE,
		FALSE,
		DEFAULT_CHARSET,
		OUT_CHARACTER_PRECIS,
		CLIP_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		FF_DONTCARE,
		_T("2002L"));

	m_AxisType.SetFont(&m_FontAxis);
	m_ViewType.SetFont(&m_Font);

	m_bApplyMode = APPLY_NONE;
	m_bCamMode = CM_NONE;
	m_bGrid = m_bViewPort != VIEWPORT_NAVIGATE;

	InitCamera();
}

BOOL CGridView::OnEraseBkgnd(CDC* pDC)
{
	if(!IsPlay())
		Render();

	return TRUE;
}

void CGridView::DisableLight()
{
	m_LightCenter.EnableLight(FALSE);
	m_LightRight.EnableLight(FALSE);
	m_LightTop.EnableLight(FALSE);
}

void CGridView::EnableLight()
{
	D3DXVECTOR3 vAxisZ = m_Camera.m_vTarget - m_Camera.m_vPosition;
	D3DXVECTOR3 vAxisY = m_Camera.m_vUp;
	D3DXVECTOR3 vAxisX;

	D3DXVec3Cross(
		&vAxisX,
		&vAxisY,
		&vAxisZ);

	m_LightCenter.EnableLight();
	m_LightRight.EnableLight();
	m_LightTop.EnableLight();

	m_LightCenter.SetPosition( NULL, &vAxisZ);
	m_LightRight.SetPosition( NULL, &vAxisZ);
	m_LightTop.SetPosition( NULL, &vAxisZ);

	m_LightCenter.Rotate( &vAxisX, -D3DX_PI / 6.0f);
	m_LightRight.Rotate( &vAxisY, -D3DX_PI / 4.0f);
	m_LightTop.Rotate( &vAxisX, D3DX_PI / 4.0f);
}

void CGridView::GetGridBoundary( LPD3DXVECTOR3 pMinPoint,
								 LPD3DXVECTOR3 pMaxPoint)
{
	FLOAT fHeight = (FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenY;
	FLOAT fWidth = (FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenX;

	FLOAT fDistance = D3DXVec3Length(&(m_Camera.m_vTarget - m_Camera.m_vPosition));
	FLOAT fBound = 0.0f;

	if(!m_Camera.IsOrthoCamera())
	{
		FLOAT fFOV = m_Camera.m_fFOV / 2.0f;

		fFOV *= fWidth / fHeight;
		fBound = fDistance * ((FLOAT) fabs(tanf(fFOV)));

		fBound = m_fGrid * (1.0f + INT(fBound / m_fGrid)) + m_fGrid / 100.0f;
	}
	else
		fBound = fDistance * fWidth / (2.0f * fHeight);

	*pMaxPoint = m_Camera.m_vTarget + D3DXVECTOR3(
		fBound,
		fBound,
		fBound);

	*pMinPoint = m_Camera.m_vTarget - D3DXVECTOR3(
		fBound,
		fBound,
		fBound);
}

void CGridView::DrawGrid()
{
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(D3DFVF_XYZ);
	CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, NULL);


	switch(m_bViewPort)
	{
	case VIEWPORT_FRONT	:
		{
			D3DXVECTOR3 vMaxPoint;
			D3DXVECTOR3 vMinPoint;

			GetGridBoundary(
				&vMinPoint,
				&vMaxPoint);

			int nIndex = 0;
			for( FLOAT i=0.0f; i<=vMaxPoint.x; i+=m_fGrid)
			{
				if( i >= vMinPoint.x )
				{
					DrawLine(
						D3DXVECTOR3( i, vMinPoint.y, 0.0f),
						D3DXVECTOR3( i, vMaxPoint.y, 0.0f),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}

			nIndex = 0;
			for( i=0.0f; i>=vMinPoint.x; i-=m_fGrid)
			{
				if( i <= vMaxPoint.x )
				{
					DrawLine(
						D3DXVECTOR3( i, vMinPoint.y, 0.0f),
						D3DXVECTOR3( i, vMaxPoint.y, 0.0f),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}

			nIndex = 0;
			for( i=0.0f; i<=vMaxPoint.y; i+=m_fGrid)
			{
				if( i >= vMinPoint.y )
				{
					DrawLine(
						D3DXVECTOR3( vMinPoint.x, i, 0.0f),
						D3DXVECTOR3( vMaxPoint.x, i, 0.0f),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}

			nIndex = 0;
			for( i=0.0f; i>=vMinPoint.y; i-=m_fGrid)
			{
				if( i <= vMaxPoint.y )
				{
					DrawLine(
						D3DXVECTOR3( vMinPoint.x, i, 0.0f),
						D3DXVECTOR3( vMaxPoint.x, i, 0.0f),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}
		}

		break;

	case VIEWPORT_LEFT	:
		{
			D3DXVECTOR3 vMaxPoint;
			D3DXVECTOR3 vMinPoint;

			GetGridBoundary(
				&vMinPoint,
				&vMaxPoint);

			int nIndex = 0;
			for( FLOAT i=0.0f; i<=vMaxPoint.z; i+=m_fGrid)
			{
				if( i >= vMinPoint.z )
				{
					DrawLine(
						D3DXVECTOR3( 0.0f, vMinPoint.y, i),
						D3DXVECTOR3( 0.0f, vMaxPoint.y, i),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}

			nIndex = 0;
			for( i=0.0f; i>=vMinPoint.z; i-=m_fGrid)
			{
				if( i <= vMaxPoint.z )
				{
					DrawLine(
						D3DXVECTOR3( 0.0f, vMinPoint.y, i),
						D3DXVECTOR3( 0.0f, vMaxPoint.y, i),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}

			nIndex = 0;
			for( i=0.0f; i<=vMaxPoint.y; i+=m_fGrid)
			{
				if( i >= vMinPoint.y )
				{
					DrawLine(
						D3DXVECTOR3( 0.0f, i, vMinPoint.z),
						D3DXVECTOR3( 0.0f, i, vMaxPoint.z),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}

			nIndex = 0;
			for( i=0.0f; i>=vMinPoint.y; i-=m_fGrid)
			{
				if( i <= vMaxPoint.y )
				{
					DrawLine(
						D3DXVECTOR3( 0.0f, i, vMinPoint.z),
						D3DXVECTOR3( 0.0f, i, vMaxPoint.z),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}
		}

		break;

	default				:
		{
			D3DXVECTOR3 vMaxPoint;
			D3DXVECTOR3 vMinPoint;

			GetGridBoundary(
				&vMinPoint,
				&vMaxPoint);

			int nIndex = 0;
			for( FLOAT i=0.0f; i<=vMaxPoint.x; i+=m_fGrid)
			{
				if( i >= vMinPoint.x )
				{
					DrawLine(
						D3DXVECTOR3( i, 0.0f, vMinPoint.z),
						D3DXVECTOR3( i, 0.0f, vMaxPoint.z),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}

			nIndex = 0;
			for( i=0.0f; i>=vMinPoint.x; i-=m_fGrid)
			{
				if( i <= vMaxPoint.x )
				{
					DrawLine(
						D3DXVECTOR3( i, 0.0f, vMinPoint.z),
						D3DXVECTOR3( i, 0.0f, vMaxPoint.z),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}

			nIndex = 0;
			for( i=0.0f; i<=vMaxPoint.z; i+=m_fGrid)
			{
				if( i >= vMinPoint.z )
				{
					DrawLine(
						D3DXVECTOR3( vMinPoint.x, 0.0f, i),
						D3DXVECTOR3( vMaxPoint.x, 0.0f, i),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}

			nIndex = 0;
			for( i=0.0f; i>=vMinPoint.z; i-=m_fGrid)
			{
				if( i <= vMaxPoint.z )
				{
					DrawLine(
						D3DXVECTOR3( vMinPoint.x, 0.0f, i),
						D3DXVECTOR3( vMaxPoint.x, 0.0f, i),
						nIndex % 10 ? NORMAL_COLOR : nIndex == 0 ? ZERO_COLOR : THICK_COLOR);
				}

				nIndex++;
			}
		}

		break;
	}
}

void CGridView::DrawLine( D3DXVECTOR3 vFrom,
						  D3DXVECTOR3 vTo,
						  DWORD dwColor)
{
	D3DVECTOR vPoint[2] = {
		{ vFrom.x, vFrom.y, vFrom.z},
		{ vTo.x, vTo.y, vTo.z}};

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwColor);
	CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
		D3DPT_LINELIST, 1,
		vPoint,
		sizeof(D3DVECTOR));
}

void CGridView::DrawContent()
{
	CTachyonSFX::m_dwParticleNum = 0;
}

void CGridView::DrawFPSnParticle()
{
	// ViewInfo
	CRect rect;
	GetClientRect(&rect);

	rect.OffsetRect(
		(CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		(CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

	FLOAT fDigit = log10f(m_fGrid);
	CString strViewType;
	CString strFormat;

	strFormat.Format( ".%d", fDigit < 0.0f ? -INT(fDigit) : 0);
	strFormat = CString("%s (Grid = %") + strFormat + CString("f)");
	strViewType.Format( strFormat, m_strViewType, m_fGrid);

	CString strText = strViewType;

	static DWORD dwPrevTick = ::GetTickCount();
	static DWORD dwCount = 0;
	static CString strFPS(_T(""));
	CString strParticle(_T(""));

	if( m_bViewPort == VIEWPORT_PERSPECTIVE && m_nFrameMode == GSVIEW_SFX )
	{
		// FPS
		strFPS.Format( _T("\n[FPS : %d]"), m_dwFPS);

		// Particle
		strParticle.Format( "\n[nParticle : %d]", CTachyonSFX::m_dwParticleNum );

		strText += strFPS + strParticle;
	}

	m_ViewType.TextOut(
		CMainFrame::m_3DDevice.m_pDevice,
		strText,
		rect.left + BORDER_SIZE,
		rect.top);
}

void CGridView::Render()
{
	if( !IsWindowVisible() || !CMainFrame::m_bRender )
		return;

	D3DXMATRIX vWorld;
	CRect rect;

	D3DXMatrixIdentity(&vWorld);
	GetClientRect(&rect);

	rect.OffsetRect(
		(CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		(CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET|
		D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB(
		BACK_COLOR,
		BACK_COLOR,
		BACK_COLOR),
		1.0f, 0);

	CMainFrame::m_3DDevice.m_pDevice->BeginScene();

	m_Camera.Activate(TRUE);
	GetFitGrid();

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0x00000000);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_TEXTURE0, &vWorld);
	CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);

	if(m_pBackIMGTEX && m_bBackIMG)
		DrawBackIMG();

	if(m_bGrid)
		DrawGrid();

	DrawContent();
	DrawFPSnParticle();

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	HWND hWND = GetSafeHwnd();
	if(hWND)
	{
		HRESULT hr = CMainFrame::m_pSWAPCHAIN->Present(
			&rect,
			NULL,
			hWND,
			NULL, 0);

		if(FAILED(hr))
			CMainFrame::RestoreDevice();
	}
}

FLOAT CGridView::GetFitGrid()
{
	if( m_bViewPort != VIEWPORT_NAVIGATE )
	{
		FLOAT fHeight;

		if(!m_Camera.IsOrthoCamera())
			fHeight = D3DXVec3Length(&(m_Camera.m_vTarget - m_Camera.m_vPosition)) * ((FLOAT) fabs(tanf(m_Camera.m_fFOV / 2.0f))) / (GRID_COUNT / 2.0f);
		else
			fHeight = m_Camera.m_fHeight / GRID_COUNT;

		FLOAT fLOG = log10f(fHeight);
		FLOAT fGrid = powf( 10.0f, (FLOAT) INT(fLOG < 0.0f ? fLOG - 0.5f : fLOG + 0.5f));

		if( m_fGrid != fGrid )
			ResetCam(fGrid);

		return fGrid;
	}

	m_fGrid = 1.0f;
	return m_fGrid;
}

void CGridView::ResetCam( FLOAT fGrid)
{
	if(m_Camera.IsOrthoCamera())
	{
		m_Camera.m_fNearPlane = min( -fGrid * Z_LEVEL / 2.0f, -Z_MIN / 2.0f);
		m_Camera.m_fFarPlane = max( fGrid * Z_LEVEL / 2.0f, Z_MIN / 2.0f);

		D3DXMatrixOrthoLH(
			&m_Camera.m_matProjection,
			m_Camera.m_fWidth,
			m_Camera.m_fHeight,
			m_Camera.m_fNearPlane,
			m_Camera.m_fFarPlane);
	}
	else
	{
		m_Camera.m_fFarPlane = max( fGrid * Z_LEVEL, Z_MIN);
		m_Camera.m_fNearPlane = fGrid;

		D3DXMatrixPerspectiveFovLH(
			&m_Camera.m_matProjection,
			m_Camera.m_fFOV,
			m_Camera.m_fAspect,
			m_Camera.m_fNearPlane,
			m_Camera.m_fFarPlane);
	}

	CMainFrame::m_3DDevice.m_pDevice->SetTransform(
		D3DTS_PROJECTION,
		&m_Camera.m_matProjection);

	m_fGrid = fGrid;
}

void CGridView::InitFOV( FLOAT fFOV )
{
	m_Camera.ResetFOV( fFOV );
	CMainFrame::m_3DDevice.m_pDevice->SetTransform(
		D3DTS_PROJECTION,
		&m_Camera.m_matProjection);
	m_Camera.Activate( TRUE );
}

void CGridView::InitCamera()
{
	switch(m_bViewPort)
	{
	case VIEWPORT_TOP		:
		{
			m_Camera.InitOrthoCamera(
				CMainFrame::m_3DDevice.m_pDevice,
				min( -m_fGrid * Z_LEVEL / 2.0f, -Z_MIN / 2.0f),
				max( m_fGrid * Z_LEVEL / 2.0f, Z_MIN / 2.0f),
				m_fGrid * GRID_COUNT *
				((FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenX) /
				((FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenY),
				m_fGrid * GRID_COUNT);

			m_Camera.SetPosition(
				D3DXVECTOR3( 0.0f, m_fGrid * GRID_COUNT, 0.0f),
				D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
				D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
				FALSE);
			m_strViewType = _T("Top");
		}

		break;

	case VIEWPORT_FRONT		:
		{
			m_Camera.InitOrthoCamera(
				CMainFrame::m_3DDevice.m_pDevice,
				min( -m_fGrid * Z_LEVEL / 2.0f, -Z_MIN / 2.0f),
				max( m_fGrid * Z_LEVEL / 2.0f, Z_MIN / 2.0f),
				m_fGrid * GRID_COUNT *
				((FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenX) /
				((FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenY),
				m_fGrid * GRID_COUNT);

			m_Camera.SetPosition(
				D3DXVECTOR3( 0.0f, 0.0f, -m_fGrid * GRID_COUNT),
				D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
				D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
				FALSE);
			m_strViewType = _T("Front");
		}

		break;

	case VIEWPORT_LEFT		:
		{
			m_Camera.InitOrthoCamera(
				CMainFrame::m_3DDevice.m_pDevice,
				min( -m_fGrid * Z_LEVEL / 2.0f, -Z_MIN / 2.0f),
				max( m_fGrid * Z_LEVEL / 2.0f, Z_MIN / 2.0f),
				m_fGrid * GRID_COUNT *
				((FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenX) /
				((FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenY),
				m_fGrid * GRID_COUNT);

			m_Camera.SetPosition(
				D3DXVECTOR3( -m_fGrid * GRID_COUNT, 0.0f, 0.0f),
				D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
				D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
				FALSE);
			m_strViewType = _T("Left");
		}

		break;

	case VIEWPORT_NAVIGATE	:
		{
			m_Camera.InitCamera(
				CMainFrame::m_3DDevice.m_pDevice,
				NAV_NEAR,
				NAV_FAR,
				CMainFrame::m_fFOV,// D3DX_PI / 4.0f, 원형
				CMainFrame::m_3DDevice.m_option.m_dwScreenX,
				CMainFrame::m_3DDevice.m_option.m_dwScreenY);

			m_Camera.SetPosition(
				D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
				D3DXVECTOR3( 0.0f, 0.0f, 10.0f),
				D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
				FALSE);

			m_strViewType = _T("Navigate");
		}

		break;

	default				:
		{
			m_Camera.InitCamera(
				CMainFrame::m_3DDevice.m_pDevice,
				m_fGrid,
				max( m_fGrid * Z_LEVEL, Z_MIN),
				CMainFrame::m_fFOV,/* 원형 D3DX_PI / 4.0f,*/ // 02_02_14 박상연 수정
				CMainFrame::m_3DDevice.m_option.m_dwScreenX,
				CMainFrame::m_3DDevice.m_option.m_dwScreenY);

			m_Camera.SetPosition(
				D3DXVECTOR3( 0.0f, 0.0f, -m_fGrid * GRID_COUNT / (2.0f * ((FLOAT) fabs(tanf(m_Camera.m_fFOV / 2.0f))))),
				D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
				D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
				FALSE);

			m_Camera.Rotate(
				0.0f,
				-D3DX_PI / 9.0f,
				0.0f);
			m_strViewType = _T("Perspective");
		}

		break;
	}

	m_Camera.Activate(TRUE);
}

void CGridView::OnDestroy()
{
	m_FontAxis.DeleteObject();
	m_Font.DeleteObject();

	m_ViewType.Release();
	m_AxisType.Release();
	
	if(m_pBackIMGTEX)
	{
		m_pBackIMGTEX->Release();
		m_pBackIMGTEX	= NULL;
	}
	if(m_pBackIMGDlg)
	{
		delete m_pBackIMGDlg;
		m_pBackIMGDlg = NULL;
	}

	CView::OnDestroy();
}

void CGridView::OnSetFocus( CWnd* pOldWnd)
{
	GetParent()->Invalidate();
	CView::OnSetFocus(pOldWnd);
}

void CGridView::OnLButtonDown(UINT nFlags, CPoint point)
{
	if( m_bViewPort != VIEWPORT_NAVIGATE && (nFlags&MK_SHIFT) )
	{
		CPoint center;
		CRect rect;

		GetClientRect(&rect);
		ClientToScreen(&rect);
		center = rect.CenterPoint();

		SetCursorPos( center.x, center.y);
		m_bCamMode = CM_ZOOM;
		SetCursor(NULL);
	}
	else if( m_bViewPort == VIEWPORT_NAVIGATE || (nFlags&MK_CONTROL) )
	{
		CPoint center;
		CRect rect;

		GetClientRect(&rect);
		ClientToScreen(&rect);
		center = rect.CenterPoint();

		SetCursorPos( center.x, center.y);
		ScreenToClient(&center);
		center -= point;
		center = CPoint(
			abs(center.x),
			abs(center.y));

		if( center.x < CENTER_BOUND &&
			center.y < CENTER_BOUND )
			m_bApplyMode = APPLY_Z;
		else if( center.x > center.y )
			m_bApplyMode = APPLY_X;
		else if( center.x < center.y )
			m_bApplyMode = APPLY_Y;

		m_bCamMode = CM_ROT;
		SetCursor(NULL);
	}
	SetCapture();

	CView::OnLButtonDown( nFlags, point);
}

void CGridView::OnLButtonUp(UINT nFlags, CPoint point)
{
	if( m_bCamMode != CM_NONE )
	{
		m_bApplyMode = APPLY_NONE;
		m_bCamMode = CM_NONE;

		SetCursor(LoadCursor( NULL, IDC_ARROW));
	}
	ReleaseCapture();

	CView::OnLButtonUp( nFlags, point);
}

void CGridView::OnMButtonDown( UINT nFlags, CPoint point)
{
	CPoint center;
	CRect rect;

	GetClientRect(&rect);
	center = rect.CenterPoint();

	center -= point;
	center = CPoint(
		abs(center.x),
		abs(center.y));

	if( center.x < CENTER_BOUND &&
		center.y < CENTER_BOUND )
		m_bApplyMode = APPLY_Z;
	else if( center.x > center.y )
		m_bApplyMode = APPLY_X;
	else if( center.x < center.y )
		m_bApplyMode = APPLY_Y;
	m_point = point;

	m_bCamMode = CM_MOVE;
	SetCursor(CGSDialogBar::m_hHand);
	SetCapture();

	CView::OnMButtonDown( nFlags, point);
}

void CGridView::OnMButtonUp( UINT nFlags, CPoint point)
{
	if( m_bCamMode != CM_NONE )
	{
		m_bApplyMode = APPLY_NONE;
		m_bCamMode = CM_NONE;

		SetCursor(LoadCursor( NULL, IDC_ARROW));
	}
	ReleaseCapture();

	CView::OnMButtonUp( nFlags, point);
}

void CGridView::OnMouseMove( UINT nFlags, CPoint point)
{
	switch(m_bCamMode)
	{
	case CM_MOVE	:
		{
			CPoint screen = m_point - point;

			if( screen != CPoint( 0, 0) )
			{
				FLOAT fRatio = 1.0f;

				if( m_bViewPort == VIEWPORT_NAVIGATE )
					fRatio = 10.0f;
				else if(!m_Camera.IsOrthoCamera())
					fRatio = D3DXVec3Length(&(m_Camera.m_vTarget - m_Camera.m_vPosition)) * ((FLOAT) fabs(m_Camera.m_fFOV / 2.0f));
				else
					fRatio = m_Camera.m_fHeight / 2.0f;

				fRatio /= ((FLOAT) CMainFrame::m_3DDevice.m_option.m_dwScreenY) / 2.0f;
				MoveView(
					fRatio * screen.x,
					fRatio * screen.y,
					0.0f);

				m_point = point;
			}
		}

		break;

	case CM_ROT		:
		if( m_bViewPort == VIEWPORT_NAVIGATE || (nFlags&MK_CONTROL) )
		{
			CPoint screen = point;
			CPoint center;
			CRect rect;

			GetClientRect(&rect);
			ClientToScreen(&rect);
			ClientToScreen(&screen);
			center = rect.CenterPoint();
			screen = center - screen;

			if( screen != CPoint( 0, 0) )
			{
				RotateView(
					((FLOAT) screen.y) * D3DX_PI / 360.0f,
					((FLOAT) screen.x) * D3DX_PI / 360.0f,
					((FLOAT) (abs(screen.y) > abs(screen.x) ? screen.y : screen.x)) * D3DX_PI / 360.0f);

				SetCursorPos( center.x, center.y);
			}
		}
		else
		{
			m_bCamMode = CM_NONE;
			SetCursor(LoadCursor( NULL, IDC_ARROW));
		}

		break;

	case CM_ZOOM	:
		if(nFlags&MK_SHIFT)
		{
			CPoint screen = point;
			CPoint center;
			CRect rect;

			GetClientRect(&rect);
			ClientToScreen(&rect);
			ClientToScreen(&screen);
			center = rect.CenterPoint();
			screen = center - screen;

			if( screen.y != 0 )
			{
				ZoomView(((FLOAT) screen.y) / ZOOM_UNIT);
				SetCursorPos( center.x, center.y);
			}
		}
		else
		{
			m_bCamMode = CM_NONE;
			SetCursor(LoadCursor( NULL, IDC_ARROW));
		}

		break;

	default			:
		if( m_bViewPort == VIEWPORT_NAVIGATE || (nFlags&MK_CONTROL) )
		{
			CPoint center = point;
			CRect rect;

			GetClientRect(&rect);
			center -= rect.CenterPoint();
			center = CPoint(
				abs(center.x),
				abs(center.y));

			if( center.x < CENTER_BOUND &&
				center.y < CENTER_BOUND )
				m_bApplyMode = APPLY_Z;
			else if( center.x > center.y )
				m_bApplyMode = APPLY_X;
			else if( center.x < center.y )
				m_bApplyMode = APPLY_Y;
		}
		else
			m_bApplyMode = APPLY_NONE;

		break;
	}

	CView::OnMouseMove( nFlags, point);
}

BYTE CGridView::IsPlay()
{
	return FALSE;
}

void CGridView::ZoomView( FLOAT fZoom)
{
	if( ( m_fGrid > 100.0f && fZoom < 0.0f ) ||
		( m_fGrid < 0.01f && fZoom > 0.0f ))
		return;

	if( fZoom > 0.2f )
		fZoom = 0.2f;

	if( fZoom < -0.2f )
		fZoom = -0.2f;

	D3DXVECTOR3 vZoom = fZoom * (m_Camera.m_vTarget - m_Camera.m_vPosition);
	m_Camera.m_vPosition += vZoom;

	if(m_Camera.IsOrthoCamera())
	{
		FLOAT fHeight = fZoom < 0.0f ? D3DXVec3Length(&vZoom) : -D3DXVec3Length(&vZoom);

		fHeight *= m_Camera.m_fWidth / m_Camera.m_fHeight;
		m_Camera.ZoomOrthoCamera(fHeight);
	}

	m_Camera.SetPosition(
		m_Camera.m_vPosition,
		m_Camera.m_vTarget,
		m_Camera.m_vUp);

	if(!IsPlay())
		Render();
}

void CGridView::RotateView( FLOAT fRotX,
						    FLOAT fRotY,
							FLOAT fRotZ)
{
	if( m_bViewPort == VIEWPORT_NAVIGATE )
	{
		D3DXVECTOR3 vPOS = m_Camera.m_vPosition;

		m_Camera.Rotate(
			0.0f,
			fRotX,
			0.0f);

		m_Camera.Rotate(
			-fRotY,
			0.0f,
			0.0f,
			TRUE);

		D3DXVECTOR3 vAxisZ = m_Camera.m_vTarget - m_Camera.m_vPosition;
		D3DXVECTOR3 vAxisX = m_Camera.m_vAxisX;
		D3DXVECTOR3 vAxisY;

		vAxisZ /= D3DXVec3Length(&vAxisZ);
		vAxisX.y = 0.0f;
		vAxisX /= D3DXVec3Length(&vAxisX);

		D3DXVec3Cross(
			&vAxisY,
			&vAxisZ,
			&vAxisX);
		vAxisY /= D3DXVec3Length(&vAxisY);

		m_Camera.SetPosition(
			vPOS,
			vPOS + 10.0f * vAxisZ,
			vAxisY);
	}
	else
	{
		switch(m_bApplyMode)
		{
		case APPLY_X	:
			{
				m_Camera.Rotate(
					0.0f,
					fRotX,
					0.0f);
			}

			break;

		case APPLY_Y	:
			{
				m_Camera.Rotate(
					-fRotY,
					0.0f,
					0.0f,
					!m_Camera.IsOrthoCamera());
			}

			break;

		case APPLY_Z	:
			{
				m_Camera.Rotate(
					0.0f,
					0.0f,
					fRotZ);
			}

			break;
		}

		if(m_Camera.IsOrthoCamera())
			m_strViewType = _T("User");
	}

	if(!IsPlay())
		Render();
}

void CGridView::MoveView( FLOAT fMoveX,
						  FLOAT fMoveY,
						  FLOAT fMoveZ)
{
	m_Camera.Move( fMoveX, -fMoveY, 0.0f);
	if(!IsPlay())
		Render();
}

void CGridView::OnGridShow()
{
	m_bGrid = !m_bGrid;
	if(!IsPlay())
		Render();
}

void CGridView::OnUpdateGridShow( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bGrid);
}

void CGridView::OnGridTop()
{
	if( m_bViewPort != VIEWPORT_TOP )
	{
		D3DXVECTOR3 vTarget = m_Camera.m_vTarget;

		m_bViewPort = VIEWPORT_TOP;
		InitCamera();
		m_Camera.Move(
			vTarget.x,
			vTarget.y,
			vTarget.z,
			TRUE);

		if(!IsPlay())
			Render();
	}
}

void CGridView::OnUpdateGridTop( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bViewPort == VIEWPORT_TOP);
}

void CGridView::OnGridPerspective()
{
	if( m_bViewPort != VIEWPORT_PERSPECTIVE )
	{
		D3DXVECTOR3 vTarget = m_Camera.m_vTarget;

		m_bViewPort = VIEWPORT_PERSPECTIVE;
		InitCamera();
		m_Camera.Move(
			vTarget.x,
			vTarget.y,
			vTarget.z,
			TRUE);

		if(!IsPlay())
			Render();
	}
}

void CGridView::OnUpdateGridPerspective( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bViewPort == VIEWPORT_PERSPECTIVE);
}

void CGridView::OnUpdateGridNavigate( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bViewPort == VIEWPORT_NAVIGATE);
}

void CGridView::OnGridLeft()
{
	if( m_bViewPort != VIEWPORT_LEFT )
	{
		D3DXVECTOR3 vTarget = m_Camera.m_vTarget;

		m_bViewPort = VIEWPORT_LEFT;
		InitCamera();
		m_Camera.Move(
			vTarget.x,
			vTarget.y,
			vTarget.z,
			TRUE);

		if(!IsPlay())
			Render();
	}
}

void CGridView::OnUpdateGridLeft(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bViewPort == VIEWPORT_LEFT);
}

void CGridView::OnGridFront()
{
	if( m_bViewPort != VIEWPORT_FRONT )
	{
		D3DXVECTOR3 vTarget = m_Camera.m_vTarget;

		m_bViewPort = VIEWPORT_FRONT;
		InitCamera();
		m_Camera.Move(
			vTarget.x,
			vTarget.y,
			vTarget.z,
			TRUE);

		if(!IsPlay())
			Render();
	}
}

void CGridView::OnUpdateGridFront( CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_bViewPort == VIEWPORT_FRONT);
}

void CGridView::OnGridCamreset()
{
	InitCamera();
	if(!IsPlay())
		Render();
}

void CGridView::DeleteForRestore()
{
}

void CGridView::Restore()
{
}

void CGridView::FitCamDist( FLOAT fHeight)
{
	FLOAT fDist = D3DXVec3Length(&(m_Camera.m_vTarget - m_Camera.m_vPosition));
	FLOAT fZoom = 0.0f;

	if(m_Camera.IsOrthoCamera())
	{
		fZoom = fHeight - fDist;
		m_Camera.ZoomOrthoCamera(fZoom * m_Camera.m_fWidth / m_Camera.m_fHeight);
	}
	else
		fZoom = fHeight / (2.0f * ((FLOAT) fabs(tanf(m_Camera.m_fFOV / 2.0f)))) - fDist;

	D3DXVECTOR3 vZoom = m_Camera.m_vTarget - m_Camera.m_vPosition;
	vZoom *= -fZoom / fDist;

	m_Camera.m_vPosition += vZoom;
	m_Camera.SetPosition(
		m_Camera.m_vPosition,
		m_Camera.m_vTarget,
		m_Camera.m_vUp);
}

BOOL CGridView::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message)
{
	switch(m_bCamMode)
	{
	case CM_MOVE	: SetCursor(CGSDialogBar::m_hHand); return TRUE;
	default			:
		if( m_bCamMode != CM_NONE )
		{
			SetCursor(NULL);
			return TRUE;
		}

		break;
	}

	return CView::OnSetCursor( pWnd, nHitTest, message);
}

void CGridView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch(nChar)
	{
	case 'T'	:
	case 't'	: SendMessage( WM_COMMAND, ID_GRID_TOP); break;
	case 'F'	:
	case 'f'	: SendMessage( WM_COMMAND, ID_GRID_FRONT); break;
	case 'L'	:
	case 'l'	: SendMessage( WM_COMMAND, ID_GRID_LEFT); break;
	case 'P'	:
	case 'p'	: SendMessage( WM_COMMAND, ID_GRID_PERSPECTIVE); break;
	case 'N'	:
	case 'n'	: SendMessage( WM_COMMAND, ID_GRID_NAVIGATE); break;
	case 'E'	:
	case 'e'	: SendMessage( WM_COMMAND, ID_GRID_CAMRESET); break;
	case 'W'	:
	case 'w'	: GetParentFrame()->SendMessage( WM_COMMAND, ID_GRID_TOGGLE); break;
	case 'G'	:
	case 'g'	: SendMessage( WM_COMMAND, ID_GRID_SHOW); break;
	case VK_F3	: SendMessage( WM_COMMAND, ID_GRID_SHOW_EDGE); break;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CGridView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( m_bViewPort != VIEWPORT_NAVIGATE )
		ZoomView(((FLOAT) zDelta) / (((FLOAT) 30.0f) * ZOOM_UNIT));

	return CView::OnMouseWheel( nFlags, zDelta, pt);
}

void CGridView::OnBackimgLoad()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tga; *.bmp; *.png; *.jpg; *.dds"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Image Files (*.tga, *.bmp, *.png, *.jpg, *.dds)|*.tga; *.bmp; *.png; *.jpg; *.dds|All Files (*.*)|*.*||"));

	if(dlg.DoModal() == IDOK)
	{
		CString strFileName;
		m_bBackIMG = TRUE;

		strFileName = dlg.GetPathName();
		HRESULT hr;

		if( FAILED(hr = D3DXCreateTextureFromFileEx(
			CMainFrame::m_3DDevice.m_pDevice,
			strFileName,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			1, 0,
			D3DFMT_UNKNOWN,
			D3DPOOL_DEFAULT,
			D3DX_DEFAULT,
			D3DX_DEFAULT,
			0,
			&m_InfoBackIMG,
			NULL,
			&m_pBackIMGTEX)) )
		{
			m_bBackIMG = FALSE;
			CString strERR;
			switch(hr)
			{
				case D3DERR_NOTAVAILABLE		:	strERR.Format("This device does not support the queried technique."); break;
				case D3DERR_OUTOFVIDEOMEMORY	:	strERR.Format("Direct3D does not have enough display memory to perform the operation");	break;
				case D3DERR_INVALIDCALL			:	strERR.Format("The method call is invalid"); break;
				case D3DXERR_INVALIDDATA		:	strERR.Format("The data is invalid. ");	break;
				case E_OUTOFMEMORY				:	strERR.Format("Direct3D could not allocate sufficient memory to complete the call"); break;
			}
			AfxMessageBox(strERR);
			return;
		}

		m_BackIMG.m_nSizeX			= m_InfoBackIMG.Width;
		m_BackIMG.m_nSizeY			= m_InfoBackIMG.Height;
		m_BackIMG.m_fMinDist		= 0.0f;
		m_BackIMG.m_fMaxDist		= 3000.0f;
		m_BackIMG.m_fSizeRateX		= 1.0f;
		m_BackIMG.m_fSizeRateY		= 1.0f;
		m_BackIMG.m_bFixSizeScreen	= TRUE;
		m_BackIMG.m_bPosFix			= FALSE;
		m_BackIMG.m_fDist			= m_BackIMG.m_fMaxDist;

		m_bBackIMG = TRUE;
	}
}

void CGridView::DrawBackIMG(void)
{
	static PVERTEX vPOINT[4];

	INT nTexSizeX = m_InfoBackIMG.Width / 2;
	INT nTexSizeY = m_InfoBackIMG.Height / 2;
	
	CPoint center;
	CRect rect;

	GetClientRect(&rect);

	D3DXVECTOR3 vPOS[4];
	
	if(!m_BackIMG.m_bPosFix)
	{
		if(m_BackIMG.m_bFixSizeScreen)
		{
			vPOS[0] = m_Camera.GetCameraPosition(
							rect.left + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
							rect.top + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
			vPOS[1] = m_Camera.GetCameraPosition(
							rect.left + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
							rect.bottom + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
			vPOS[2] = m_Camera.GetCameraPosition(
							rect.right + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
							rect.top + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
			vPOS[3] = m_Camera.GetCameraPosition(
							rect.right + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
							rect.bottom + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
		}
		else
		{
			INT nIMGSizeX = INT(FLOAT(m_BackIMG.m_nSizeX) * m_BackIMG.m_fSizeRateX);
			INT nIMGSizeY = INT(FLOAT(m_BackIMG.m_nSizeY) * m_BackIMG.m_fSizeRateY);
			
			INT nCenterX = ((rect.left + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2) + rect.Width() / 2);
			INT nCenterY = ((rect.top + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2) + rect.Height() / 2);

			vPOS[0] = m_Camera.GetCameraPosition( nCenterX - (nIMGSizeX / 2),	nCenterY - (nIMGSizeY / 2));
			vPOS[1] = m_Camera.GetCameraPosition( nCenterX - (nIMGSizeX / 2),	nCenterY + (nIMGSizeY / 2));
			vPOS[2] = m_Camera.GetCameraPosition( nCenterX + (nIMGSizeX / 2),	nCenterY - (nIMGSizeY / 2));
			vPOS[3] = m_Camera.GetCameraPosition( nCenterX + (nIMGSizeX / 2),	nCenterY + (nIMGSizeY / 2));
		}

		D3DXVECTOR3 vDIR[4] =
		{
			m_Camera.GetRayDirection(
				rect.left + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
				rect.top + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2),
			m_Camera.GetRayDirection(
				rect.left + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
				rect.bottom + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2),
			m_Camera.GetRayDirection(
				rect.right + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
				rect.top + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2),
			m_Camera.GetRayDirection(
				rect.right + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
				rect.bottom + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2)
		};
		
		FLOAT fDist = CalcBackIMGDist();

		for(int i=0; i<4; i++)
		{
			vDIR[i] *= fDist;
			vPOS[i] += vDIR[i];
		}

		PVERTEX vPoint[4] =
		{
			{ vPOS[0].x, vPOS[0].y, vPOS[0].z, 0.0f, 0.0f},
			{ vPOS[2].x, vPOS[2].y, vPOS[2].z, 1.0f, 0.0f},
			{ vPOS[1].x, vPOS[1].y, vPOS[1].z, 0.0f, 1.0f},
			{ vPOS[3].x, vPOS[3].y, vPOS[3].z, 1.0f, 1.0f}
		};
		
		for(i=0; i<4; i++)
			vPOINT[i] = vPoint[i];
	}

	CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, m_pBackIMGTEX );
	
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_PVERTEX);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

	CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
		D3DPT_TRIANGLESTRIP, 2,
		vPOINT,
		sizeof(PVERTEX));
}

FLOAT CGridView::CalcBackIMGDist()
{
	static FLOAT fPreDIST = -1.0f;
	enum
	{
		GRID_1	= 32,
		GRID_2	= 327
	};
	switch(INT(m_fGrid * 100))
	{
	case 1		:
	case 10		:
		if(INT(m_BackIMG.m_fDist) > GRID_1)
		{
			fPreDIST = m_BackIMG.m_fDist;
			return FLOAT(GRID_1);
		}
		break;
	case 100	:
		if(INT(m_BackIMG.m_fDist) > GRID_2)
		{
			if(fPreDIST != -1.0 && fPreDIST < GRID_2)
			{
				return fPreDIST;
			}
			return FLOAT(GRID_2);
		}
		else
			fPreDIST = -1.0f;
		break;
	}

	return m_BackIMG.m_fDist;
}

void CGridView::OnBackimgShow()
{
	if(m_pBackIMGTEX)
	{
		m_bBackIMG = !m_bBackIMG;
		if(!IsPlay())
			Render();
	}
}

void CGridView::OnUpdateBackimgShow(CCmdUI *pCmdUI)
{
	if(m_pBackIMGTEX)
	{
		pCmdUI->Enable(TRUE);
		pCmdUI->SetCheck(m_bBackIMG);
	}
	else
		pCmdUI->Enable(FALSE);
}

void CGridView::OnSetBackIMG()
{
	if(m_pBackIMGTEX)
	{
		if(m_pBackIMGDlg)
		{
			delete m_pBackIMGDlg;
			m_pBackIMGDlg = NULL;
		}

		m_pBackIMGDlg = new CSetBackIMGDlg();
		BOOL ret = m_pBackIMGDlg->Create(IDD_DIALOG_SETBACKIMG, this);

		if(!ret)  //Create failed.
			AfxMessageBox("Error creating Dialog");

		m_pBackIMGDlg->InitData(&m_BackIMG,(LPVOID)this);

		m_pBackIMGDlg->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pBackIMGDlg->ShowWindow(SW_HIDE);
		m_pBackIMGDlg->DestroyWindow();

		delete m_pBackIMGDlg;
		m_pBackIMGDlg = NULL;
	}
}

void CGridView::OnUpdateSetBackIMG(CCmdUI *pCmdUI)
{
	if(m_pBackIMGTEX)
		pCmdUI->Enable(TRUE);
	else
		pCmdUI->Enable(FALSE);
}

