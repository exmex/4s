// MeshView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MeshView.h"
#include "MainFrm.h"


// CMeshView

IMPLEMENT_DYNCREATE(CMeshView, CGridView)

CMeshView::CMeshView()
{
}

CMeshView::~CMeshView()
{
}

BEGIN_MESSAGE_MAP(CMeshView, CGridView)
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_GRID_CAMRESET, OnGridCamreset)
END_MESSAGE_MAP()

void CMeshView::DrawContent()
{
	CGridView::DrawContent();

	CMeshDoc *pDoc = GetDocument();
	static D3DXMATRIX vWorld(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	if(pDoc)
	{
		CMeshBar *pBar = pDoc->GetMeshBar();

		D3DXVECTOR3 vDist = m_Camera.m_vTarget - m_Camera.m_vPosition;
		FLOAT fDist = D3DXVec3Length(&vDist);
		int nLevel = pBar->m_bTestMode ? pDoc->m_pItem->m_Mesh.GetLevel(fDist) : (INT) pBar->m_dwLevel;

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		CMainFrame::m_3DDevice.m_pDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

		CMainFrame::m_3DDevice.m_pDevice->SetFVF(pDoc->m_pItem->m_Mesh.m_dwNodeCount ? T3DFVF_WMESHVERTEX : T3DFVF_MESHVERTEX);
		CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);
		EnableLight();

		for( DWORD i=0; i<pDoc->m_pItem->m_Mesh.m_dwMeshCount; i++)
		{
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState(
				D3DRS_TEXTUREFACTOR,
				pDoc->m_nCurSel == i ?
				pDoc->m_pCheck[i] ?
				0xFF00FFFF : 0xFF00FF00 :
				pDoc->m_pCheck[i] ?
				0xFFFFFF00 : 0xFFFFFFFF);

			pDoc->m_pItem->m_Mesh.Render( CMainFrame::m_3DDevice.m_pDevice, i, nLevel);
		}

		DisableLight();
		pDoc->SetCamDist( this, fDist);
	}
}

void CMeshView::OnInitialUpdate()
{
	CMeshDoc *pDoc = GetDocument();
	CGridView::OnInitialUpdate();

	m_Camera.Move(
		pDoc->m_pItem->m_Mesh.m_vCenterPoint.x,
		pDoc->m_pItem->m_Mesh.m_vCenterPoint.y,
		pDoc->m_pItem->m_Mesh.m_vCenterPoint.z,
		TRUE);
}

void CMeshView::InitCamera()
{
	CMeshDoc *pDoc = GetDocument();
	CGridView::InitCamera();

	FLOAT fHeight = pDoc->m_pItem->m_Mesh.m_fRadius;
	CRect rect;

	GetClientRect(&rect);
	fHeight = fHeight * CMainFrame::m_3DDevice.m_option.m_dwScreenY / rect.Height() * 1.2f;
	FitCamDist(fHeight);
}

void CMeshView::OnGridCamreset()
{
	CMeshDoc *pDoc = GetDocument();
	InitCamera();

	m_Camera.Move(
		pDoc->m_pItem->m_Mesh.m_vCenterPoint.x,
		pDoc->m_pItem->m_Mesh.m_vCenterPoint.y,
		pDoc->m_pItem->m_Mesh.m_vCenterPoint.z,
		TRUE);

	Render();
}


// CMeshView 진단입니다.

#ifdef _DEBUG
void CMeshView::AssertValid() const
{
	CGridView::AssertValid();
}

void CMeshView::Dump(CDumpContext& dc) const
{
	CGridView::Dump(dc);
}
#endif //_DEBUG


// CMeshView 메시지 처리기입니다.

void CMeshView::OnMButtonDown( UINT nFlags, CPoint point)
{
	CView::OnMButtonDown( nFlags, point);
}

void CMeshView::OnMButtonUp( UINT nFlags, CPoint point)
{
	CView::OnMButtonUp( nFlags, point);
}

void CMeshView::OnMouseMove( UINT nFlags, CPoint point)
{
	CMeshDoc *pDoc = GetDocument();
	int nPrevSel = pDoc->m_nCurSel;

	if( m_bCamMode == CM_NONE && (nFlags&~MK_LBUTTON) == 0 )
	{
		CMeshBar *pBar = pDoc->GetMeshBar();
		CRect rect;
		int nLevel = pBar->m_bTestMode ? pDoc->m_pItem->m_Mesh.GetLevel(D3DXVec3Length(&(m_Camera.m_vTarget - m_Camera.m_vPosition))) : (INT) pBar->m_dwLevel;

		GetClientRect(&rect);
		pDoc->m_nCurSel = pDoc->m_pItem->m_Mesh.HitTest(
			&m_Camera,
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2,
			nLevel);
	}
	else
		pDoc->m_nCurSel = -1;

	if( nPrevSel != pDoc->m_nCurSel )
		pDoc->RedrawAllView();

	CGridView::OnMouseMove( nFlags, point);
}

void CMeshView::OnLButtonUp( UINT nFlags, CPoint point)
{
	CMeshDoc *pDoc = GetDocument();
	CMeshBar *pBar = pDoc->GetMeshBar();

	if( m_bCamMode == CM_NONE && pDoc->m_nCurSel != -1 )
	{
		pDoc->m_pCheck[pDoc->m_nCurSel] = !pDoc->m_pCheck[pDoc->m_nCurSel];
		pBar->SetItemData();

		if(pBar->m_bTestMode)
			pDoc->RedrawAllView();
	}

	CGridView::OnLButtonUp( nFlags, point);
}
