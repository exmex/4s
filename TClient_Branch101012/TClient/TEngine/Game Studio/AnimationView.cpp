// AnimationView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "AnimationView.h"
#include "TextureView.h"
#include "MainFrm.h"


// CAnimationView

IMPLEMENT_DYNCREATE(CAnimationView, CGridView)

CAnimationView::CAnimationView()
{
	m_vMove.x = 0.0f;
	m_vMove.y = 0.0f;
	m_vMove.z = 0.0f;
}

CAnimationView::~CAnimationView()
{
}

BEGIN_MESSAGE_MAP(CAnimationView, CGridView)
	ON_COMMAND(ID_GRID_CAMRESET, OnGridCamreset)
END_MESSAGE_MAP()


void CAnimationView::DrawContent()
{
	CGridView::DrawContent();

	CAnimationDoc *pDoc = GetDocument();

	if(pDoc)
	{
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);
		pDoc->m_pItem->m_Aniset.m_fCurTime = pDoc->m_fTime;

		LPANIDATA pANI = pDoc->m_pItem->m_Aniset.GetAniData();
		CAnimationItem *pAniItem = pDoc->m_pItem->GetAniItem(pANI);
		D3DXMATRIX vWorld = pDoc->CalcFrameMatrix();

		m_Camera.Move(
			vWorld._41 - m_vMove.x,
			vWorld._42 - m_vMove.y,
			vWorld._43 - m_vMove.z,
			TRUE);

		m_vMove.x = vWorld._41;
		m_vMove.y = vWorld._42;
		m_vMove.z = vWorld._43;
		EnableLight();

		if(!pANI)
		{
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

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_MESHVERTEX);
			CMainFrame::m_3DDevice.m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);

			for( int i=0; i<6; i++)
			{
				CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, CTextureView::m_pVertexBox, 0, sizeof(MESHVERTEX));
				CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive(
					D3DPT_TRIANGLESTRIP,
					4 * i, 2);
			}
		}
		else
		{
			pAniItem->Render(
				CMainFrame::m_3DDevice.m_pDevice,
				NULL,
				vWorld,
				pDoc->m_pItem->m_Aniset.m_fLocalTime);
		}

		DisableLight();
	}
}


// CAnimationView 진단입니다.

#ifdef _DEBUG
void CAnimationView::AssertValid() const
{
	CGridView::AssertValid();
}

void CAnimationView::Dump(CDumpContext& dc) const
{
	CGridView::Dump(dc);
}
#endif //_DEBUG


// CAnimationView 메시지 처리기입니다.

BYTE CAnimationView::IsPlay()
{
	CAnimationDoc *pDoc = GetDocument();
	return pDoc->GetPlayCtrl()->m_bPlay;
}

void CAnimationView::InitCamera()
{
	CAnimationDoc *pDoc = GetDocument();
	pDoc->m_pItem->m_Aniset.m_fCurTime = pDoc->m_fTime;
	LPANIDATA pANI = pDoc->m_pItem->m_Aniset.GetAniData();

	m_fGrid = 1.0f;
	CGridView::InitCamera();

	if( pANI && pANI->m_pAni )
	{
		D3DXVECTOR3 vMax;
		D3DXVECTOR3 vMin;

		if(pANI->m_pAni->GetSize( vMin, vMax))
		{
			FLOAT fHeight = max( vMax.x - vMin.x, vMax.y - vMin.y);
			CRect rect;

			fHeight = max( vMax.z - vMin.z, fHeight);
			GetClientRect(&rect);

			fHeight = fHeight * CMainFrame::m_3DDevice.m_option.m_dwScreenY / rect.Height() * 1.2f;
			FitCamDist(fHeight);
		}
	}
}

void CAnimationView::OnInitialUpdate()
{
	CGridView::OnInitialUpdate();
	CAnimationDoc *pDoc = GetDocument();

	D3DXMATRIX vWorld = pDoc->CalcFrameMatrix();
	D3DXVECTOR3 vMove(
		vWorld._41,
		vWorld._42,
		vWorld._43);

	pDoc->m_pItem->m_Aniset.m_fCurTime = pDoc->m_fTime;
	LPANIDATA pANI = pDoc->m_pItem->m_Aniset.GetAniData();

	if( pANI && pANI->m_pAni )
	{
		D3DXVECTOR3 vMax;
		D3DXVECTOR3 vMin;

		if(pANI->m_pAni->GetSize( vMin, vMax))
			vMove += (vMin + vMax) / 2.0f;
	}

	m_Camera.Move(
		vMove.x,
		vMove.y,
		vMove.z,
		TRUE);
}

void CAnimationView::OnGridCamreset()
{
	CAnimationDoc *pDoc = GetDocument();

	D3DXMATRIX vWorld = pDoc->CalcFrameMatrix();
	D3DXVECTOR3 vMove(
		vWorld._41,
		vWorld._42,
		vWorld._43);
	InitCamera();

	pDoc->m_pItem->m_Aniset.m_fCurTime = pDoc->m_fTime;
	LPANIDATA pANI = pDoc->m_pItem->m_Aniset.GetAniData();

	if( pANI && pANI->m_pAni )
	{
		D3DXVECTOR3 vMax;
		D3DXVECTOR3 vMin;

		if(pANI->m_pAni->GetSize( vMin, vMax))
			vMove += (vMin + vMax) / 2.0f;
	}

	m_Camera.Move(
		vMove.x,
		vMove.y,
		vMove.z,
		TRUE);

	if(!IsPlay())
		Render();
}
