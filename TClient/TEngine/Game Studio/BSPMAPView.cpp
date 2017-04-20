// BSPMAPView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BSPMAPView.h"
#include "MainFrm.h"
#include ".\bspmapview.h"


#define BIAS_SELECT			3
#define BIAS_WIRE			7

// CBSPMAPView

IMPLEMENT_DYNCREATE(CBSPMAPView, CMAPView)

CBSPMAPView::CBSPMAPView()
{
	m_mapLIGHT.clear();
	m_mapPVS.clear();
	m_mapVB.clear();

	m_dwNodeID = NODEID_NULL;
	m_dwNodeCNT = 0;
	m_nIndex = -1;
}

CBSPMAPView::~CBSPMAPView()
{
	MAPMESHVB::iterator itMESH;
	MAPTEXVB::iterator itLIGHT;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		delete (*itLIGHT).second;

	for( itMESH = m_mapVB.begin(); itMESH != m_mapVB.end(); itMESH++)
		delete (*itMESH).second;

	m_mapLIGHT.clear();
	m_mapPVS.clear();
	m_mapVB.clear();
}

BEGIN_MESSAGE_MAP(CBSPMAPView, CMAPView)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_HOTKET_EDIT_OBJECT, OnHotketEditObject)
END_MESSAGE_MAP()


// CBSPMAPView 진단입니다.

#ifdef _DEBUG
void CBSPMAPView::AssertValid() const
{
	CMAPView::AssertValid();
}

void CBSPMAPView::Dump(CDumpContext& dc) const
{
	CMAPView::Dump(dc);
}
#endif //_DEBUG


// CBSPMAPView 메시지 처리기입니다.

void CBSPMAPView::DrawContent()
{
	CGridView::DrawContent();

	CBSPMAPDoc *pDoc = GetDocument();

	if(pDoc)
	{
		CTBSPNode *pNode = pDoc->m_pItem->m_BSPMAP.m_Head.FindLeaf(&m_Camera.m_vPosition);
		CBSPMAPBar *pBar = pDoc->GetBSPMAPBar();
		DWORD dwNodeID = pNode && pNode->IsLeaf() ? pNode->m_dwID : NODEID_NULL;

		if( dwNodeID != m_dwNodeID || m_mapPVS.empty() )
		{
			pDoc->m_pItem->m_BSPMAP.BuildPVS( &m_mapPVS, &m_mapLIGHT, &m_mapVB, dwNodeID);
			m_dwNodeCNT = 0;

			if( dwNodeID != NODEID_NULL )
				for( int i=0; i<INT(pNode->m_vWinding.size()); i++)
					m_dwNodeCNT += INT((pNode->m_vWinding)[i]->m_vPoint.size()) - 2;
		}

		pDoc->m_pItem->m_BSPMAP.Render( &m_mapPVS, &m_mapLIGHT, &m_mapVB);
		m_dwNodeID = dwNodeID;

		MAPMESHVB::iterator finder = m_mapVB.find(pDoc->m_nIndex);
		if( finder != m_mapVB.end() )
		{
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

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
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFF00);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_SELECT);

			CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, (*finder).second->m_vVB.GetVB(), 0, sizeof(MESHVERTEX));
			CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive(
				D3DPT_TRIANGLELIST,
				0, (*finder).second->m_dwCount);

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFF000000);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_WIRE);

			CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive(
				D3DPT_TRIANGLELIST,
				0, (*finder).second->m_dwCount);
		}
		else if( pDoc->m_nIndex != -1 )
		{
			pDoc->m_nIndex = -1;
			pBar->SetMeshIDX();
		}

		if(m_bShowEdge)
		{
			MAPMESHVB::iterator it;

			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
			CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);
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
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFF000000);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);

			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_NORMALIZENORMALS, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
			CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, BIAS_WIRE);

			for( it = m_mapVB.begin(); it != m_mapVB.end(); it++)
			{
				CMainFrame::m_3DDevice.m_pDevice->SetStreamSource( 0, (*it).second->m_vVB.GetVB(), 0, sizeof(MESHVERTEX));
				CMainFrame::m_3DDevice.m_pDevice->DrawPrimitive(
					D3DPT_TRIANGLELIST,
					0, (*it).second->m_dwCount);
			}
		}

		MAPMESHVB::iterator it;
		DWORD dwPVS = 0;

		for( it = m_mapVB.begin(); it != m_mapVB.end(); it++)
			dwPVS += (*it).second->m_dwCount;

		pDoc->SetNodeInfo(
			this,
			m_dwNodeID,
			m_dwNodeCNT,
			dwPVS);

		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_AMBIENT, pDoc->m_pItem->m_BSPMAP.m_dwAmbient);
		CMainFrame::m_3DDevice.m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, 0);

		pDoc->m_pItem->m_BSPMAP.EnableLight();
		CMAPView::DrawContent();
		pDoc->m_pItem->m_BSPMAP.DisableLight();
	}
}

BYTE CBSPMAPView::GetCursorPos( LPD3DXVECTOR3 pPOS)
{
	CBSPMAPDoc *pDoc = GetDocument();
	CPoint point;
	CRect rect;
	FLOAT fDist;

	::GetCursorPos(&point);
	ScreenToClient(&point);
	GetClientRect(&rect);

	if(!rect.PtInRect(point))
		return FALSE;

	D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
		point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
	D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
		point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
		point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

	if( pDoc->m_pItem->m_BSPMAP.HitTest( &vPos, &vDir, &fDist) == -1 )
		return FALSE;

	vDir /= D3DXVec3Length(&vDir);
	(*pPOS) = vPos + fDist * vDir;

	return TRUE;
}

void CBSPMAPView::OnInitialUpdate()
{
	CMAPView::OnInitialUpdate();
	m_DropTarget.Register(this);
}

BYTE CBSPMAPView::CanDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_TEXTURE ? TRUE : FALSE;
}

DROPEFFECT CBSPMAPView::OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CBSPMAPDoc *pDoc = GetDocument();
	CBSPMAPBar *pBar = pDoc->GetBSPMAPBar();

	if(pBar->m_bRegion)
		m_nIndex = pDoc->m_nIndex;

	return CBSPMAPView::OnDragOver( pDataObject, dwKeyState, point);
}

DROPEFFECT CBSPMAPView::OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT nResult = DROPEFFECT_NONE;

	if(!IsPlay())
	{
		HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
		LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
		CPoint screenpos = point;

		switch(pData->m_bType)
		{
		case GSCT_ITEM		:
		case GSCT_TEX		:
			{
				CItemBase *pItem = (CItemBase *) pData->m_pData;
				CBSPMAPDoc *pDoc = GetDocument();
				CRect rect;

				GetClientRect(&rect);
				if(CanDrop(pItem))
				{
					D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
						point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
						point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
					D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
						point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
						point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
					FLOAT fDist;

					int nIndex = pDoc->m_pItem->m_BSPMAP.HitTest( &vPos, &vDir, &fDist);
					if( nIndex != -1 )
						nResult = DROPEFFECT_MOVE;

					if( pDoc->m_nIndex != nIndex )
					{
						pDoc->m_nIndex = nIndex;

						CGameStudioView::m_pDragImage->DragMove( CPoint(
							-10000,
							-10000));

						pDoc->RedrawAllView();
					}
				}
			}

			break;
		}

		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);

		ClientToScreen(&screenpos);
		CGameStudioView::m_pDragImage->DragMove(screenpos);
	}

	return nResult;
}

BOOL CBSPMAPView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	if(!IsPlay())
	{
		HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
		LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

		switch(pData->m_bType)
		{
		case GSCT_ITEM		:
		case GSCT_TEX		:
			{
				CItemBase *pItem = (CItemBase *) pData->m_pData;
				CBSPMAPDoc *pDoc = GetDocument();
				CRect rect;

				GetClientRect(&rect);
				if(CanDrop(pItem))
				{
					D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
						point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
						point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
					D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
						point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
						point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
					FLOAT fDist;

					pDoc->m_nIndex = pDoc->m_pItem->m_BSPMAP.HitTest( &vPos, &vDir, &fDist);
					if( pDoc->m_nIndex != -1 )
						InsertTexture((CTextureItem *) pItem);
				}
			}

			break;
		}

		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);
	}

	OnDragLeave();
	return TRUE;
}

void CBSPMAPView::OnDragLeave()
{
	CBSPMAPDoc *pDoc = GetDocument();
	CBSPMAPBar *pBar = pDoc->GetBSPMAPBar();

	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	if(pBar->m_bRegion)
		pDoc->m_nIndex = m_nIndex;
	else
		pDoc->m_nIndex = -1;

	if(!IsPlay())
		pDoc->RedrawAllView();

	CMAPView::OnDragLeave();
}

void CBSPMAPView::OnLButtonDown( UINT nFlags, CPoint point)
{
	CMAPView::OnLButtonDown( nFlags, point);
	CBSPMAPDoc *pDoc = GetDocument();
	CBSPMAPBar *pBar = pDoc->GetBSPMAPBar();

	if( !IsPlay() && m_bCamMode == CM_NONE )
	{
		FLOAT fDist;
		CRect rect;
		GetClientRect(&rect);

		D3DXVECTOR3 vPos = m_Camera.GetCameraPosition(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
		D3DXVECTOR3 vDir = m_Camera.GetRayDirection(
			point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
			point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

		int nIndex = pDoc->m_pItem->m_BSPMAP.HitTest( &vPos, &vDir, &fDist);
		if(pBar->m_bRegion)
		{
			pDoc->m_nIndex = nIndex;
			pBar->SetMeshIDX();
		}
		else if( nIndex != -1 )
		{
			MAPDWORD::iterator itTEX = pDoc->m_pItem->m_vTEX[pDoc->m_pItem->m_BSPMAP.m_nTexIDX]->find(nIndex);
			ReleaseCapture();

			if( itTEX != pDoc->m_pItem->m_vTEX[pDoc->m_pItem->m_BSPMAP.m_nTexIDX]->end() )
			{
				CTextureItem *pItem = CItemBase::GetTextureItem((*itTEX).second);

				if(pItem)
				{
					RemoveTexture(nIndex);

					HGLOBAL hGlobal = ::GlobalAlloc( GMEM_SHARE|GMEM_MOVEABLE, sizeof(GSCDATA));
					COleDataSource source;

					CGameStudioView::m_pDragImage = new CImageList();
					LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
					pData->m_pData = (LPVOID) pItem;
					pData->m_bType = GSCT_TEX;
					::GlobalUnlock(hGlobal);

					source.CacheGlobalData( CGameStudioView::m_cfGS, hGlobal);
					if( source.DoDragDrop() == DROPEFFECT_NONE )
					{
						::GetCursorPos(&point);
						ScreenToClient(&point);

						vPos = m_Camera.GetCameraPosition(
							point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
							point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);

						vDir = m_Camera.GetRayDirection(
							point.x + (CMainFrame::m_3DDevice.m_option.m_dwScreenX - rect.Width()) / 2,
							point.y + (CMainFrame::m_3DDevice.m_option.m_dwScreenY - rect.Height()) / 2);
						pDoc->m_nIndex = pDoc->m_pItem->m_BSPMAP.HitTest( &vPos, &vDir, &fDist);

						if( pDoc->m_nIndex == nIndex )
							InsertTexture(pItem);

						pDoc->m_nIndex = -1;
						pDoc->RedrawAllView();
					}

					::GlobalFree(hGlobal);
					CGameStudioView::InitDragResource();
				}
			}
		}
	}
}

void CBSPMAPView::InsertTexture( CTextureItem *pItem)
{
	CBSPMAPDoc *pDoc = GetDocument();
	RemoveTexture(pDoc->m_nIndex);

	pDoc->m_pItem->m_BSPMAP.m_vTexture[pDoc->m_pItem->m_BSPMAP.m_nTexIDX][pDoc->m_nIndex]
		= pItem->GetTEXTURESET();

	pDoc->m_pItem->m_vTEX[pDoc->m_pItem->m_BSPMAP.m_nTexIDX]->insert( MAPDWORD::value_type(
		pDoc->m_nIndex,
		pItem->m_dwID));
}

void CBSPMAPView::RemoveTexture( int nIndex)
{
	CBSPMAPDoc *pDoc = GetDocument();
	MAPDWORD::iterator finder = pDoc->m_pItem->m_vTEX[pDoc->m_pItem->m_BSPMAP.m_nTexIDX]->find(nIndex);

	pDoc->m_pItem->m_BSPMAP.m_vTexture[pDoc->m_pItem->m_BSPMAP.m_nTexIDX][nIndex] = NULL;
	if( finder != pDoc->m_pItem->m_vTEX[pDoc->m_pItem->m_BSPMAP.m_nTexIDX]->end() )
		pDoc->m_pItem->m_vTEX[pDoc->m_pItem->m_BSPMAP.m_nTexIDX]->erase(finder);
}

void CBSPMAPView::OnHotketEditObject()
{
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
	CBSPMAPDoc *pDoc = GetDocument();
	CBSPMAPBar *pBar = pDoc->GetBSPMAPBar();
	pBar->OnEditObject();
}
