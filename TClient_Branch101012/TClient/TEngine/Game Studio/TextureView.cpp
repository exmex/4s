// TextureView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "TextureView.h"
#include "StaticMesh.h"
#include "ExternVar.h"

#include "MainFrm.h"
#include ".\textureview.h"


LPDIRECT3DVERTEXBUFFER9 CTextureView::m_pVertexSphere = NULL;
LPDIRECT3DINDEXBUFFER9 CTextureView::m_pIndexSphere = NULL;

LPDIRECT3DVERTEXBUFFER9 CTextureView::m_pVertexPlane = NULL;
LPDIRECT3DVERTEXBUFFER9 CTextureView::m_pVertexBox = NULL;

// CTextureView

IMPLEMENT_DYNCREATE(CTextureView, CScrollView)

CTextureView::CTextureView()
{
	m_bCamMode = CM_NONE;
}

CTextureView::~CTextureView()
{
}

BEGIN_MESSAGE_MAP(CTextureView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEWHEEL()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

void CTextureView::ReleaseTextureMesh()
{
	if(m_pVertexSphere)
	{
		m_pVertexSphere->Release();
		m_pVertexSphere = NULL;
	}

	if(m_pVertexBox)
	{
		m_pVertexBox->Release();
		m_pVertexBox = NULL;
	}

	if(m_pVertexPlane)
	{
		m_pVertexPlane->Release();
		m_pVertexPlane = NULL;
	}

	if(m_pIndexSphere)
	{
		m_pIndexSphere->Release();
		m_pIndexSphere = NULL;
	}
}

void CTextureView::InitTextureMesh()
{
	LPMESHVERTEX pBuffer = NULL;
	LPWORD pIndex = NULL;
	ReleaseTextureMesh();

	CMainFrame::m_3DDevice.m_pDevice->CreateVertexBuffer(
		SPHERE_VERTEXCOUNT * sizeof(MESHVERTEX),
		0, T3DFVF_MESHVERTEX,
		D3DPOOL_MANAGED,
		&m_pVertexSphere, NULL);

	CMainFrame::m_3DDevice.m_pDevice->CreateVertexBuffer(
		BOX_VERTEXCOUNT * sizeof(MESHVERTEX),
		0, T3DFVF_MESHVERTEX,
		D3DPOOL_MANAGED,
		&m_pVertexBox, NULL);

	CMainFrame::m_3DDevice.m_pDevice->CreateVertexBuffer(
		PLANE_VERTEXCOUNT * sizeof(MESHVERTEX),
		0, T3DFVF_MESHVERTEX,
		D3DPOOL_MANAGED,
		&m_pVertexPlane, NULL);

	CMainFrame::m_3DDevice.m_pDevice->CreateIndexBuffer(
		SPHERE_INDEXCOUNT * 2,
		0, D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIndexSphere,
		NULL);

	m_pVertexSphere->Lock( 0, SPHERE_VERTEXCOUNT * sizeof(MESHVERTEX), (LPVOID *) &pBuffer, 0);
	memcpy( pBuffer, g_arraySphereVertex, SPHERE_VERTEXCOUNT * sizeof(MESHVERTEX));
	m_pVertexSphere->Unlock();

	m_pVertexBox->Lock( 0, BOX_VERTEXCOUNT * sizeof(MESHVERTEX), (LPVOID *) &pBuffer, 0);
	memcpy( pBuffer, g_arrayBoxVertex, BOX_VERTEXCOUNT * sizeof(MESHVERTEX));
	m_pVertexBox->Unlock();

	m_pVertexPlane->Lock( 0, PLANE_VERTEXCOUNT * sizeof(MESHVERTEX), (LPVOID *) &pBuffer, 0);
	memcpy( pBuffer, g_arrayPlaneVertex, PLANE_VERTEXCOUNT * sizeof(MESHVERTEX));
	m_pVertexPlane->Unlock();

	m_pIndexSphere->Lock( 0, SPHERE_INDEXCOUNT * 2, (LPVOID *) &pIndex, 0);
	memcpy( pIndex, g_arraySphereIndex, SPHERE_INDEXCOUNT * 2);
	m_pIndexSphere->Unlock();
}


// CTextureView 그리기입니다.

void CTextureView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	CSize size( 100, 100);

	SetScrollSizes( MM_TEXT, size);
}

void CTextureView::InitDraw()
{
	D3DLIGHT9 light;

	m_Camera.InitCamera(
		CMainFrame::m_3DDevice.m_pDevice,
		0.5f, 1000.0f, D3DX_PI / 6.0f,
		CMainFrame::m_3DDevice.m_option.m_dwScreenX,
		CMainFrame::m_3DDevice.m_option.m_dwScreenY);

	m_Camera.SetPosition(
		D3DXVECTOR3( 0.0f, 0.0f, -8.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f),
		FALSE);

	ZeroMemory( &light, sizeof(D3DLIGHT9));
	light.Type = D3DLIGHT_DIRECTIONAL;
	light.Diffuse.r = 0.8f;
	light.Diffuse.g = 0.8f;
	light.Diffuse.b = 0.8f;
	light.Diffuse.a = 1.0f;

	light.Ambient.r = 0.2f;
	light.Ambient.g = 0.2f;
	light.Ambient.b = 0.2f;
	light.Ambient.a = 1.0f;
	light.Range = 1000.0f;

	light.Direction = D3DXVECTOR3( 0.0f, 0.0f, 1.0f);
	m_Light.SetLight(&light);
	m_Light.EnableLight();

	m_Light.Rotate(
		&D3DXVECTOR3( 1.0f, 0.0f, 0.0f),
		D3DX_PI / 4.0f);

	m_Light.Rotate(
		&D3DXVECTOR3( 0.0f, -1.0f, 0.0f),
		D3DX_PI / 4.0f);

	m_Light.EnableLight(TRUE);
	m_Camera.Activate(TRUE);
}

void CTextureView::OnDraw( CDC* pDC)
{
	Render();
}


// CTextureView 진단입니다.

#ifdef _DEBUG
void CTextureView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CTextureView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CTextureView 메시지 처리기입니다.

void CTextureView::Render()
{
	CTextureDoc *pDoc = GetDocument();
	static D3DXMATRIX matTex;

	if( pDoc && CMainFrame::m_bRender )
	{
		pDoc->m_pItem->GetTEXTURESET()->m_dwCurTick = pDoc->m_dwTick;
		UVKEY vKey = pDoc->m_pItem->GetTEXTURESET()->GetUV();
		CTMath::GetUVMatrix( &matTex, &vKey);

		CMainFrame::m_3DDevice.m_pDevice->Clear(
			0, NULL,
			D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
			D3DCOLOR_XRGB(
			GetRValue(pDoc->m_pItem->m_dwBackColor),
			GetGValue(pDoc->m_pItem->m_dwBackColor),
			GetBValue(pDoc->m_pItem->m_dwBackColor)),
			1.0f, 0);
		CMainFrame::m_3DDevice.m_pDevice->BeginScene();
		SetState();

		if( pDoc && pDoc->m_pItem )
			CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, pDoc->m_pItem->GetTEXTURESET()->GetTexture());
		else
			CMainFrame::m_3DDevice.m_pDevice->SetTexture( 0, NULL);

		CMainFrame::m_3DDevice.m_pDevice->SetTransform(
			D3DTS_WORLD,
			&pDoc->m_matObject);

		CMainFrame::m_3DDevice.m_pDevice->SetTransform(
			D3DTS_TEXTURE0,
			&matTex);

		if( pDoc->GetTextureBar()->m_bShowTiling )
		{
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP  );
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
		}
		else
		{
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_BORDER );
			CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_BORDER );
		}

		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &pDoc->m_pItem->GetTEXTURESET()->m_fMipBias));
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, pDoc->m_pItem->GetTEXTURESET()->m_dwMipFilter);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, pDoc->m_bRenderType == RT_PLANE ? 0xFFFFFFFF : 0x00000000);

		switch(pDoc->m_bRenderType)
		{
		case RT_SPHERE	:
			{
				CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, m_pVertexSphere, 0, sizeof(MESHVERTEX));
				CMainFrame::m_3DDevice.m_pDevice->SetIndices(m_pIndexSphere);
				CMainFrame::m_3DDevice.m_pDevice->DrawIndexedPrimitive(
					D3DPT_TRIANGLELIST, 0, 0,
					SPHERE_VERTEXCOUNT, 0,
					SPHERE_INDEXCOUNT / 3);
			}

			break;

		case RT_BOX		:
			for( int i=0; i<6; i++)
			{
				CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, m_pVertexBox, 0, sizeof(MESHVERTEX));
				CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive(
					D3DPT_TRIANGLESTRIP,
					4 * i, 2);
			}

			break;

		case RT_PLANE	:
			{
				CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, m_pVertexPlane, 0, sizeof(MESHVERTEX));
				CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
			}

			break;
		}
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP  );
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP );
		CMainFrame::m_3DDevice.m_pDevice->EndScene();

		CRect rect;
		GetClientRect(&rect);

		rect.OffsetRect(
			(CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			(CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

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

		m_Light.EnableLight(FALSE);
	}
}

void CTextureView::SetState()
{
	m_Light.EnableLight();
	m_Camera.Activate(TRUE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);

	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
	CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_MESHVERTEX);
}

BOOL CTextureView::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CTextureView::OnLButtonDown( UINT nFlags, CPoint point)
{
	if(nFlags&MK_CONTROL)
	{
		CPoint center;
		CRect rect;

		GetClientRect(&rect);
		ClientToScreen(&rect);
		center = rect.CenterPoint();

		SetCursorPos( center.x, center.y);
		SetCursor(NULL);

		ScreenToClient(&center);
		center -= point;
		center = CPoint(
			abs(center.x),
			abs(center.y));

		if( center.x < CENTER_BOUND &&
			center.y < CENTER_BOUND )
			GetDocument()->m_bApplyMode = APPLY_Z;
		else if( center.x > center.y )
			GetDocument()->m_bApplyMode = APPLY_X;
		else if( center.x < center.y )
			GetDocument()->m_bApplyMode = APPLY_Y;

		m_bCamMode = CM_ROT;
		SetCursor(NULL);
	}
	else if(nFlags&MK_SHIFT)
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
	SetCapture();

	CScrollView::OnLButtonDown( nFlags, point);
}

void CTextureView::OnMouseMove( UINT nFlags, CPoint point)
{
	switch(m_bCamMode)
	{
	case CM_ROT		:
		if(nFlags&MK_CONTROL)
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
				CTextureDoc *pDoc = GetDocument();

				if( pDoc->m_bRenderType != RT_PLANE )
				{
					FLOAT fDistance = pDoc->m_matObject._43;
					D3DXMATRIX matRotX;
					D3DXMATRIX matRotY;
					D3DXMATRIX matRotZ;

					switch(pDoc->m_bApplyMode)
					{
					case APPLY_X	:
						{
							D3DXMatrixRotationX(
								&matRotX,
								((FLOAT) screen.y) * D3DX_PI / 180.0f);

							pDoc->m_matObject *= matRotX;
						}

						break;

					case APPLY_Y	:
						{
							D3DXMatrixRotationY(
								&matRotY,
								((FLOAT) screen.x) * D3DX_PI / 180.0f);

							pDoc->m_matObject *= matRotY;
						}

						break;

					case APPLY_Z	:
						{
							D3DXMatrixRotationZ(
								&matRotZ,
								((FLOAT) (abs(screen.x) > abs(screen.y) ? screen.x : screen.y)) * D3DX_PI / 180.0f);

							pDoc->m_matObject *= matRotZ;
						}
					}

					pDoc->m_matObject._41 = 0.0f;
					pDoc->m_matObject._42 = 0.0f;
					pDoc->m_matObject._43 = fDistance;

					SetCursorPos( center.x, center.y);
					if(!pDoc->GetPlayCtrl()->m_bPlay)
						Render();
				}
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
				CTextureDoc *pDoc = GetDocument();

				pDoc->m_matObject._43 -= 0.5f * FLOAT(screen.y);
				if( pDoc->m_matObject._43 < -5.0f )
					pDoc->m_matObject._43 = -5.0f;

				SetCursorPos( center.x, center.y);
				if(!pDoc->GetPlayCtrl()->m_bPlay)
					Render();
			}
		}
		else
		{
			m_bCamMode = CM_NONE;
			SetCursor(LoadCursor( NULL, IDC_ARROW));
		}

		break;

	default			:
		if(nFlags&MK_CONTROL)
		{
			CTextureDoc *pDoc = GetDocument();
			CPoint center = point;
			CRect rect;

			GetClientRect(&rect);
			center -= rect.CenterPoint();
			center = CPoint(
				abs(center.x),
				abs(center.y));

			if( center.x < CENTER_BOUND &&
				center.y < CENTER_BOUND )
				pDoc->m_bApplyMode = APPLY_Z;
			else if( center.x > center.y )
				pDoc->m_bApplyMode = APPLY_X;
			else if( center.x < center.y )
				pDoc->m_bApplyMode = APPLY_Y;
		}
		else
			GetDocument()->m_bApplyMode = APPLY_NONE;

		break;
	}

	CScrollView::OnMouseMove( nFlags, point);
}

void CTextureView::OnLButtonUp( UINT nFlags, CPoint point)
{
	if( m_bCamMode != CM_NONE )
	{
		GetDocument()->m_bApplyMode = APPLY_NONE;
		m_bCamMode = CM_NONE;

		SetCursor(LoadCursor( NULL, IDC_ARROW));
	}
	ReleaseCapture();

	CScrollView::OnLButtonUp( nFlags, point);
}

BOOL CTextureView::OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message)
{
	if( m_bCamMode != CM_NONE )
	{
		SetCursor(NULL);
		return TRUE;
	}

	return CScrollView::OnSetCursor( pWnd, nHitTest, message);
}

BOOL CTextureView::OnMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	CTextureDoc *pDoc = GetDocument();

	pDoc->m_matObject._43 -= 0.5f * ((FLOAT) zDelta) / ((FLOAT) 120.0f);
	if( pDoc->m_matObject._43 < -5.0f )
		pDoc->m_matObject._43 = -5.0f;

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		Render();

	return CScrollView::OnMouseWheel( nFlags, zDelta, pt);
}

void CTextureView::OnDestroy()
{
	CScrollView::OnDestroy();
}

void CTextureView::DeleteForRestore()
{
}

void CTextureView::Restore()
{
}
