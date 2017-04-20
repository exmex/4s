#include "StdAfx.h"


CTachyonBSPMAP::CTachyonBSPMAP()
{
	m_bMAPType = MT_BSP;

	m_mapLIGHT.clear();
	m_mapTREF.clear();
	m_vTexture.clear();
	m_pRegionID = NULL;

	m_pLIGHT = NULL;
	m_pMESH = NULL;

	m_dwAmbient = 0xFF000000;
	m_dwMeshCount = 0;
	m_nTexIDX = 0;
	m_dwTick = 0;

	m_bLIGHTMAP = TRUE;
	m_bPVS = FALSE;
}

CTachyonBSPMAP::~CTachyonBSPMAP()
{
	Clear();
}

void CTachyonBSPMAP::Render( LPMAPVECTORTEXWINDING pLIGHT,
							 LPMAPVERTEXWINDING pWINDING,
							 LPMAPTBSPNODE pPVS)
{
	FLOAT fDepthBias = 3.0f * BSPBIAS_BASE;

	MAPVERTEXWINDING::iterator itMESH;
	MAPD3DLIGHT::iterator itLIGHT;

	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, m_dwAmbient);

	if(m_bLIGHTMAP)
	{
		MAPVECTORTEXWINDING::iterator itLIGHTVB;

		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);

		CTBSPNode::m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		CTBSPNode::m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);

		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

		CTBSPNode::m_pDevice->m_pDevice->SetStreamSource( 0, m_pMESH, 0, sizeof(MESHVERTEX));
		CTBSPNode::m_pDevice->m_pDevice->SetTexture( 0, NULL);
		CTBSPNode::m_pDevice->m_pDevice->SetFVF(T3DFVF_MESHVERTEX);

		for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
			(*itLIGHT).second->EnableLight((*itLIGHT).second->m_Light.Type == D3DLIGHT_DIRECTIONAL);

		for( itMESH = pWINDING->begin(); itMESH != pWINDING->end(); itMESH++)
			for( int i=0; i<INT((*itMESH).second->size()); i++)
				CTBSPNode::m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, (*(*itMESH).second)[i]->m_nIndex, (*(*itMESH).second)[i]->m_nCount);

		for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
			(*itLIGHT).second->EnableLight(FALSE);
		fDepthBias -= BSPBIAS_BASE;

		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF);

		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);

		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));

		CTBSPNode::m_pDevice->m_pDevice->SetStreamSource( 0, m_pLIGHT, 0, sizeof(LVERTEX));
		CTBSPNode::m_pDevice->m_pDevice->SetFVF(T3DFVF_LVERTEX);

		for( itLIGHTVB = pLIGHT->begin(); itLIGHTVB != pLIGHT->end(); itLIGHTVB++)
		{
			ApplyTexture((*itLIGHTVB).first);

			for( int i=0; i<INT((*itLIGHTVB).second->size()); i++)
				for( int j=0; j<INT((*(*itLIGHTVB).second)[i]->m_vWINDING.size()); j++)
					CTBSPNode::m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, (*(*itLIGHTVB).second)[i]->m_vWINDING[j]->m_nIndex, (*(*itLIGHTVB).second)[i]->m_vWINDING[j]->m_nCount);
		}
		fDepthBias -= BSPBIAS_BASE;

		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO);
	}
	else
	{
		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
		CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);
	}

	CTBSPNode::m_pDevice->m_pDevice->SetStreamSource( 0, m_pMESH, 0, sizeof(MESHVERTEX));
	CTBSPNode::m_pDevice->m_pDevice->SetFVF(T3DFVF_MESHVERTEX);

	for( itMESH = pWINDING->begin(); itMESH != pWINDING->end(); itMESH++)
	{
		ApplyTexture(m_vTexture[m_nTexIDX][(*itMESH).first]);

		for( int i=0; i<INT((*itMESH).second->size()); i++)
			CTBSPNode::m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLELIST, (*(*itMESH).second)[i]->m_nIndex, (*(*itMESH).second)[i]->m_nCount);
	}
	fDepthBias = EXTBIAS_BASE;

	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
}

void CTachyonBSPMAP::Render( LPMAPTBSPNODE pPVS,
							 LPMAPTEXVB pLIGHT,
							 LPMAPMESHVB pVB)
{
	FLOAT fDepthBias = 3.0f * BSPBIAS_BASE;

	MAPTEXVB::iterator itLIGHTVB;
	MAPMESHVB::iterator itMESH;

	MAPD3DLIGHT::iterator itLIGHT;

	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_COUNT2);
	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	CTBSPNode::m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	CTBSPNode::m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);

	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, m_dwAmbient);

	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, TRUE);

	CTBSPNode::m_pDevice->m_pDevice->SetFVF(T3DFVF_MESHVERTEX);
	CTBSPNode::m_pDevice->m_pDevice->SetTexture( 0, NULL);

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		(*itLIGHT).second->EnableLight((*itLIGHT).second->m_Light.Type == D3DLIGHT_DIRECTIONAL);

	for( itMESH = pVB->begin(); itMESH != pVB->end(); itMESH++)
	{
		CTBSPNode::m_pDevice->m_pDevice->SetStreamSource( 0, (*itMESH).second->m_vVB.GetVB(), 0, sizeof(MESHVERTEX));
		CTBSPNode::m_pDevice->m_pDevice->DrawPrimitive(
			D3DPT_TRIANGLELIST, 0,
			(*itMESH).second->m_dwCount);

		CD3DDevice::m_dwPolyCount += (*itMESH).second->m_dwCount;
	}

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		(*itLIGHT).second->EnableLight(FALSE);
	fDepthBias -= BSPBIAS_BASE;

	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_MODULATE);
	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCCOLOR);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_COLOR1);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
	CTBSPNode::m_pDevice->m_pDevice->SetFVF(T3DFVF_LVERTEX);

	for( itLIGHTVB = pLIGHT->begin(); itLIGHTVB != pLIGHT->end(); itLIGHTVB++)
	{
		CTBSPNode::m_pDevice->m_pDevice->SetStreamSource( 0, (*itLIGHTVB).second->m_vVB.GetVB(), 0, sizeof(LVERTEX));
		ApplyTexture((*itLIGHTVB).first);

		CTBSPNode::m_pDevice->m_pDevice->DrawPrimitive(
			D3DPT_TRIANGLELIST, 0,
			(*itLIGHTVB).second->m_dwCount);

		CD3DDevice::m_dwPolyCount += (*itLIGHTVB).second->m_dwCount;
	}
	fDepthBias -= BSPBIAS_BASE;

	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_DESTCOLOR);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ZERO);
	CTBSPNode::m_pDevice->m_pDevice->SetFVF(T3DFVF_MESHVERTEX);

	for( itMESH = pVB->begin(); itMESH != pVB->end(); itMESH++)
	{
		CTBSPNode::m_pDevice->m_pDevice->SetStreamSource( 0, (*itMESH).second->m_vVB.GetVB(), 0, sizeof(MESHVERTEX));
		if( m_nTexIDX < INT(m_vTexture.size()) )
			ApplyTexture(m_vTexture[m_nTexIDX][(*itMESH).first]);
		else
			ApplyTexture(NULL);

		CTBSPNode::m_pDevice->m_pDevice->DrawPrimitive(
			D3DPT_TRIANGLELIST, 0,
			(*itMESH).second->m_dwCount);

		CD3DDevice::m_dwPolyCount += (*itMESH).second->m_dwCount;
	}
	fDepthBias = EXTBIAS_BASE;

	CTBSPNode::m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	CTBSPNode::m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
}

void CTachyonBSPMAP::ApplyTexture( LPTEXTURESET pTEX)
{
	static D3DXMATRIX vUV;

	if(pTEX)
	{
		pTEX->m_dwCurTick = pTEX->m_dwTotalTick ? m_dwTick % pTEX->m_dwTotalTick : 0;

		CTBSPNode::m_pDevice->m_pDevice->SetTexture( 0, pTEX->GetTexture());
		UVKEY vKey = pTEX->GetUV();
		CTMath::GetUVMatrix( &vUV, &vKey);

		CTBSPNode::m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPMAPLODBIAS, *((LPDWORD) &pTEX->m_fMipBias));
		CTBSPNode::m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, pTEX->m_dwMipFilter);
		CTBSPNode::m_pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vUV);
	}
	else
	{
		D3DXMatrixIdentity(&vUV);

		CTBSPNode::m_pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vUV);
		CTBSPNode::m_pDevice->m_pDevice->SetTexture( 0, NULL);
	}
}

void CTachyonBSPMAP::AddAllNode( LPMAPVECTORTEXWINDING pLIGHT,
								 LPMAPVERTEXWINDING pWINDING,
								 LPMAPTBSPNODE pPVS,
								 CTBSPNode *pNode)
{
	if(pNode->m_pFrontNode)
		AddAllNode( pLIGHT, pWINDING, pPVS, pNode->m_pFrontNode);

	if(pNode->m_pBackNode)
		AddAllNode( pLIGHT, pWINDING, pPVS, pNode->m_pBackNode);

	if(pNode->IsLeaf())
		AddPVS( pLIGHT, pWINDING, pPVS, pNode);
}

void CTachyonBSPMAP::BuildPVS( LPMAPVECTORTEXWINDING pLIGHT,
							   LPMAPVERTEXWINDING pWINDING,
							   LPMAPTBSPNODE pPVS,
							   DWORD dwNodeID)
{
	CTBSPNode *pNode = CTBSPNode::GetNode(dwNodeID);

	MAPVECTORTEXWINDING::iterator itLIGHT;
	MAPVERTEXWINDING::iterator itMESH;

	for( itLIGHT = pLIGHT->begin(); itLIGHT != pLIGHT->end(); itLIGHT++)
		delete (*itLIGHT).second;

	for( itMESH = pWINDING->begin(); itMESH != pWINDING->end(); itMESH++)
		delete (*itMESH).second;

	pWINDING->clear();
	pLIGHT->clear();
	pPVS->clear();

	if( m_bPVS && pNode && pNode->IsLeaf() )
	{
		AddPVS( pLIGHT, pWINDING, pPVS, pNode);

		for( int i=0; i<INT(pNode->m_vPortal.size()); i++)
			if(pNode->m_vPortal[i])
			{
				MAPDWORD *pNodeID = pNode->m_vPortal[i]->m_dwFrontID == pNode->m_dwID ? &pNode->m_vPortal[i]->m_mapBackPVS : &pNode->m_vPortal[i]->m_mapFrontPVS;
				MAPDWORD::iterator itID;

				for( itID = pNodeID->begin(); itID != pNodeID->end(); itID++)
					AddPVS( pLIGHT, pWINDING, pPVS, CTBSPNode::GetNode((*itID).second));
			}
	}
	else
		AddAllNode( pLIGHT, pWINDING, pPVS, &m_Head);
}

void CTachyonBSPMAP::BuildPVS( LPMAPTBSPNODE pPVS,
							   LPMAPTEXVB pLIGHT,
							   LPMAPMESHVB pVB,
							   DWORD dwNodeID)
{
	CTBSPNode *pNode = CTBSPNode::GetNode(dwNodeID);

	MAPVECTORTEXWINDING::iterator itLIGHT;
	MAPVERTEXWINDING::iterator itMESH;
	MAPMESHVB::iterator itMESHVB;
	MAPTEXVB::iterator itLIGHTVB;

	MAPVECTORTEXWINDING mapLIGHT;
	MAPVERTEXWINDING mapWinding;

	for( itLIGHTVB = pLIGHT->begin(); itLIGHTVB != pLIGHT->end(); itLIGHTVB++)
		delete (*itLIGHTVB).second;

	for( itMESHVB = pVB->begin(); itMESHVB != pVB->end(); itMESHVB++)
		delete (*itMESHVB).second;

	pLIGHT->clear();
	pPVS->clear();
	pVB->clear();

	mapWinding.clear();
	mapLIGHT.clear();

	if( m_bPVS && pNode && pNode->IsLeaf() )
	{
		AddPVS( &mapLIGHT, &mapWinding, pPVS, pNode);

		for( int i=0; i<INT(pNode->m_vPortal.size()); i++)
			if(pNode->m_vPortal[i])
			{
				MAPDWORD *pNodeID = pNode->m_vPortal[i]->m_dwFrontID == pNode->m_dwID ? &pNode->m_vPortal[i]->m_mapBackPVS : &pNode->m_vPortal[i]->m_mapFrontPVS;
				MAPDWORD::iterator itID;

				for( itID = pNodeID->begin(); itID != pNodeID->end(); itID++)
					AddPVS( &mapLIGHT, &mapWinding, pPVS, CTBSPNode::GetNode((*itID).second));
			}
	}
	else
		AddAllNode( &mapLIGHT, &mapWinding, pPVS, &m_Head);

	for( itMESH = mapWinding.begin(); itMESH != mapWinding.end(); itMESH++)
	{
		LPMESHVB pMESH = new MESHVB();

		pMESH->m_dwCount = 0;
		for( int i=0; i<INT((*itMESH).second->size()); i++)
			pMESH->m_dwCount += (*((*itMESH).second))[i]->m_vPoint.size() - 2;

		LPMESHVERTEX pVBDATA = new MESHVERTEX[3 * pMESH->m_dwCount];
		LPMESHVERTEX pDATA = pVBDATA;

		for( i=0; i<INT((*itMESH).second->size()); i++)
			for( int j=1; j<INT((*((*itMESH).second))[i]->m_vPoint.size()) - 1; j++)
			{
				memcpy( pDATA, (*((*itMESH).second))[i]->m_vPoint[0], sizeof(MESHVERTEX));
				pDATA++;

				memcpy( pDATA, (*((*itMESH).second))[i]->m_vPoint[j], sizeof(MESHVERTEX));
				pDATA++;

				memcpy( pDATA, (*((*itMESH).second))[i]->m_vPoint[j + 1], sizeof(MESHVERTEX));
				pDATA++;
			}
		delete (*itMESH).second;

		pMESH->m_vVB.LoadT3DVB(
			3 * pMESH->m_dwCount * sizeof(MESHVERTEX),
			0, T3DFVF_MESHVERTEX);

		pMESH->m_vVB.LoadT3DVBDATA(
			(LPBYTE) pVBDATA,
			3 * pMESH->m_dwCount * sizeof(MESHVERTEX), 0);
		delete[] pVBDATA;

		pVB->insert( MAPMESHVB::value_type(
			(*itMESH).first,
			pMESH));
	}

	for( itLIGHT = mapLIGHT.begin(); itLIGHT != mapLIGHT.end(); itLIGHT++)
	{
		LPMESHVB pLIGHTVB = new MESHVB();

		pLIGHTVB->m_dwCount = 0;
		for( int i=0; i<INT((*itLIGHT).second->size()); i++)
			for( int j=0; j<INT((*(*itLIGHT).second)[i]->m_vWINDING.size()); j++)
				pLIGHTVB->m_dwCount += (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint.size() - 2;

		LPLVERTEX pVBDATA = new LVERTEX[3 * pLIGHTVB->m_dwCount];
		LPLVERTEX pDATA = pVBDATA;

		for( i=0; i<INT((*itLIGHT).second->size()); i++)
			for( int j=0; j<INT((*(*itLIGHT).second)[i]->m_vWINDING.size()); j++)
				for( int k=1; k<INT((*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint.size()) - 1; k++)
				{
					pDATA->m_fPosX = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[0]->m_fPosX;
					pDATA->m_fPosY = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[0]->m_fPosY;
					pDATA->m_fPosZ = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[0]->m_fPosZ;
					pDATA->m_fU = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[0]->m_fU1;
					pDATA->m_fV = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[0]->m_fV1;
					pDATA->m_dwColor = (*(*itLIGHT).second)[i]->m_dwColor;
					pDATA++;

					pDATA->m_fPosX = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k]->m_fPosX;
					pDATA->m_fPosY = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k]->m_fPosY;
					pDATA->m_fPosZ = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k]->m_fPosZ;
					pDATA->m_fU = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k]->m_fU1;
					pDATA->m_fV = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k]->m_fV1;
					pDATA->m_dwColor = (*(*itLIGHT).second)[i]->m_dwColor;
					pDATA++;

					pDATA->m_fPosX = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k + 1]->m_fPosX;
					pDATA->m_fPosY = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k + 1]->m_fPosY;
					pDATA->m_fPosZ = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k + 1]->m_fPosZ;
					pDATA->m_fU = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k + 1]->m_fU1;
					pDATA->m_fV = (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint[k + 1]->m_fV1;
					pDATA->m_dwColor = (*(*itLIGHT).second)[i]->m_dwColor;
					pDATA++;
				}
		delete (*itLIGHT).second;

		pLIGHTVB->m_vVB.LoadT3DVB(
			3 * pLIGHTVB->m_dwCount * sizeof(LVERTEX),
			0, T3DFVF_LVERTEX);

		pLIGHTVB->m_vVB.LoadT3DVBDATA(
			(LPBYTE) pVBDATA,
			3 * pLIGHTVB->m_dwCount * sizeof(LVERTEX), 0);
		delete[] pVBDATA;

		pLIGHT->insert( MAPTEXVB::value_type(
			(*itLIGHT).first,
			pLIGHTVB));
	}

	mapWinding.clear();
	mapLIGHT.clear();
}

void CTachyonBSPMAP::AddPVS( LPMAPVECTORTEXWINDING pLIGHT,
							 LPMAPVERTEXWINDING pWINDING,
							 LPMAPTBSPNODE pPVS,
							 CTBSPNode *pNode)
{
	if( pPVS->find(pNode->m_dwID) == pPVS->end() )
	{
		MAPTEXWINDING::iterator it;

		pPVS->insert( MAPTBSPNODE::value_type(
			pNode->m_dwID,
			pNode));

		for( it = pNode->m_mapLIGHT.begin(); it != pNode->m_mapLIGHT.end(); it++)
		{
			MAPVECTORTEXWINDING::iterator finder = pLIGHT->find((*it).second->m_pTEX);
			LPVECTORTEXWINDING pItem = NULL;

			if( finder == pLIGHT->end() )
			{
				pItem = new VECTORTEXWINDING();

				pLIGHT->insert( MAPVECTORTEXWINDING::value_type(
					(*it).second->m_pTEX,
					pItem));
			}
			else
				pItem = (*finder).second;

			pItem->push_back((*it).second);
		}

		for( int i=0; i<INT(pNode->m_vWinding.size()); i++)
			if( pNode->m_vWinding[i]->m_nMeshIndex < m_dwMeshCount )
			{
				MAPVERTEXWINDING::iterator finder = pWINDING->find(pNode->m_vWinding[i]->m_nMeshIndex);
				LPVECTORVERTEXWINDING pItem = NULL;

				if( finder == pWINDING->end() )
				{
					pItem = new VECTORVERTEXWINDING();

					pWINDING->insert( MAPVERTEXWINDING::value_type(
						pNode->m_vWinding[i]->m_nMeshIndex,
						pItem));
				}
				else
					pItem = (*finder).second;

				pItem->push_back(pNode->m_vWinding[i]);
			}
	}
}

void CTachyonBSPMAP::Clear()
{
	MAPD3DLIGHT::iterator it;

	m_Head.ClearPortal();
	m_Head.ClearNode();
	m_Head.ClearLightMap();

	if( CTBSPNode::m_bBaseNodeID == 0xFF )
		m_Head.SetID(NODEID_NULL);

	for( it = m_mapLIGHT.begin(); it != m_mapLIGHT.end(); it++)
		delete (*it).second;
	m_mapLIGHT.clear();

	while(!m_vTexture.empty())
	{
		delete[] m_vTexture.back();
		m_vTexture.pop_back();
	}

	if(m_pRegionID)
	{
		delete[] m_pRegionID;
		m_pRegionID = NULL;
	}

	if(m_pLIGHT)
	{
		m_pLIGHT->Release();
		m_pLIGHT = NULL;
	}

	if(m_pMESH)
	{
		m_pMESH->Release();
		m_pMESH = NULL;
	}

	m_dwMeshCount = 0;
	m_nTexIDX = 0;

	m_mapTREF.clear();
}

void CTachyonBSPMAP::Overlap( LPBYTE pData)
{
	if(!pData)
		return;

	int nTMFVersion = *((int*) pData); pData += sizeof(int);
	switch(nTMFVersion)
	{
	case 300	:
		{
			VECTORVERTEXWINDING vWinding;
			VECTORTBSPPORTAL vPortal;

			LPMESHVERTEX pVB = NULL;
			LPBYTE pBUF = NULL;

			DWORD dwBonesCount = 0;
			DWORD dwCount = 0;
			BOOL bInit = FALSE;

			dwBonesCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			pData += sizeof(FLOAT);
			pData += sizeof(D3DXVECTOR3);
			vWinding.clear();
			vPortal.clear();

			if( dwBonesCount > 0 )
				pData += sizeof(D3DMATRIX) * dwBonesCount;

			dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			if( dwCount > 0 )
			{
				LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pData;

				pBUF = new BYTE[dwCount * sizeof(MESHVERTEX)];
				pVB = (LPMESHVERTEX) pBUF;
				pData += dwCount * sizeof(WMESHVERTEX);

				for( DWORD i=0; i<dwCount; i++)
				{
					pVB[i].m_fNormalX = pDATA[i].m_fNormalX;
					pVB[i].m_fNormalY = pDATA[i].m_fNormalY;
					pVB[i].m_fNormalZ = pDATA[i].m_fNormalZ;

					pVB[i].m_fPosX = pDATA[i].m_fPosX;
					pVB[i].m_fPosY = pDATA[i].m_fPosY;
					pVB[i].m_fPosZ = pDATA[i].m_fPosZ;

					pVB[i].m_fU1 = pDATA[i].m_fU;
					pVB[i].m_fV1 = pDATA[i].m_fV;
					pVB[i].m_fU2 = pDATA[i].m_fU;
					pVB[i].m_fV2 = pDATA[i].m_fV;
				}
			}

			m_dwMeshCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			for( DWORD i=0; i<m_dwMeshCount; i++)
			{
				dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
				dwCount /= 3;

				if( dwCount > 0 )
				{
					for( DWORD j=0; j<dwCount; j++)
					{
						LPVERTEXWINDING pWinding = new VERTEXWINDING;

						for( BYTE k=0; k<3; k++)
						{
							LPMESHVERTEX pPoint = new MESHVERTEX;

							memcpy( pPoint, &pVB[*((LPWORD) pData)], sizeof(MESHVERTEX));
							pData += sizeof(WORD);
							pWinding->m_vPoint.push_back(pPoint);

							if(!bInit)
							{
								CTBSPNode::m_vMinBound = D3DXVECTOR3(
									pPoint->m_fPosX,
									pPoint->m_fPosY,
									pPoint->m_fPosZ);

								CTBSPNode::m_vMaxBound = CTBSPNode::m_vMinBound;
								bInit = TRUE;
							}
							else
							{
								if( CTBSPNode::m_vMaxBound.x < pPoint->m_fPosX )
									CTBSPNode::m_vMaxBound.x = pPoint->m_fPosX;

								if( CTBSPNode::m_vMaxBound.y < pPoint->m_fPosY )
									CTBSPNode::m_vMaxBound.y = pPoint->m_fPosY;

								if( CTBSPNode::m_vMaxBound.z < pPoint->m_fPosZ )
									CTBSPNode::m_vMaxBound.z = pPoint->m_fPosZ;

								if( CTBSPNode::m_vMinBound.x > pPoint->m_fPosX )
									CTBSPNode::m_vMinBound.x = pPoint->m_fPosX;

								if( CTBSPNode::m_vMinBound.y > pPoint->m_fPosY )
									CTBSPNode::m_vMinBound.y = pPoint->m_fPosY;

								if( CTBSPNode::m_vMinBound.z > pPoint->m_fPosZ )
									CTBSPNode::m_vMinBound.z = pPoint->m_fPosZ;
							}
						}

						D3DXVECTOR3 vPoint[3] = {
							D3DXVECTOR3(
							pWinding->m_vPoint[0]->m_fPosX,
							pWinding->m_vPoint[0]->m_fPosY,
							pWinding->m_vPoint[0]->m_fPosZ),
							D3DXVECTOR3(
							pWinding->m_vPoint[1]->m_fPosX,
							pWinding->m_vPoint[1]->m_fPosY,
							pWinding->m_vPoint[1]->m_fPosZ),
							D3DXVECTOR3(
							pWinding->m_vPoint[2]->m_fPosX,
							pWinding->m_vPoint[2]->m_fPosY,
							pWinding->m_vPoint[2]->m_fPosZ)};

						D3DXVECTOR3 vCross(
							0.0f,
							0.0f,
							0.0f);

						D3DXVec3Cross(
							&vCross,
							&(vPoint[1] - vPoint[0]),
							&(vPoint[2] - vPoint[0]));

						if( vCross != D3DXVECTOR3( 0.0f, 0.0f, 0.0f) )
						{
							D3DXPlaneFromPoints(
								&pWinding->m_vPlane,
								&vPoint[0],
								&vPoint[1],
								&vPoint[2]);

							pWinding->m_bCheck = FALSE;
							pWinding->m_nMeshIndex = i;

							vWinding.push_back(pWinding);
						}
						else
							delete pWinding;
					}
				}
			}

			m_Head.Overlap(&vWinding);
			vWinding.clear();

			if(pBUF)
				delete[] pBUF;
		}

		break;

	case 301	:
		{
			VECTORVERTEXWINDING vWinding;
			VECTORTBSPPORTAL vPortal;

			LPMESHVERTEX pVB = NULL;
			LPBYTE pBUF = NULL;

			DWORD dwBonesCount = 0;
			DWORD dwCount = 0;
			BOOL bInit = FALSE;

			dwBonesCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			pData += sizeof(FLOAT);
			pData += sizeof(D3DXVECTOR3);
			vWinding.clear();
			vPortal.clear();

			if( dwBonesCount > 0 )
				pData += sizeof(D3DMATRIX) * dwBonesCount;

			dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			if( dwCount > 0 )
			{
				LPTMESHBUFFER pDATA = (LPTMESHBUFFER) pData;

				pBUF = new BYTE[dwCount * sizeof(MESHVERTEX)];
				pVB = (LPMESHVERTEX) pBUF;
				pData += dwCount * sizeof(TMESHBUFFER);

				for( DWORD i=0; i<dwCount; i++)
				{
					pVB[i].m_fNormalX = pDATA[i].m_fNormalX;
					pVB[i].m_fNormalY = pDATA[i].m_fNormalY;
					pVB[i].m_fNormalZ = pDATA[i].m_fNormalZ;

					pVB[i].m_fPosX = pDATA[i].m_fPosX;
					pVB[i].m_fPosY = pDATA[i].m_fPosY;
					pVB[i].m_fPosZ = pDATA[i].m_fPosZ;

					pVB[i].m_fU1 = pDATA[i].m_fU1;
					pVB[i].m_fV1 = pDATA[i].m_fV1;
					pVB[i].m_fU2 = pDATA[i].m_fU2;
					pVB[i].m_fV2 = pDATA[i].m_fV2;
				}
			}

			m_dwMeshCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			for( DWORD i=0; i<m_dwMeshCount; i++)
			{
				dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
				dwCount /= 3;

				if( dwCount > 0 )
				{
					for( DWORD j=0; j<dwCount; j++)
					{
						LPVERTEXWINDING pWinding = new VERTEXWINDING;

						for( BYTE k=0; k<3; k++)
						{
							LPMESHVERTEX pPoint = new MESHVERTEX;

							memcpy( pPoint, &pVB[*((LPWORD) pData)], sizeof(MESHVERTEX));
							pData += sizeof(WORD);
							pWinding->m_vPoint.push_back(pPoint);

							if(!bInit)
							{
								CTBSPNode::m_vMinBound = D3DXVECTOR3(
									pPoint->m_fPosX,
									pPoint->m_fPosY,
									pPoint->m_fPosZ);

								CTBSPNode::m_vMaxBound = CTBSPNode::m_vMinBound;
								bInit = TRUE;
							}
							else
							{
								if( CTBSPNode::m_vMaxBound.x < pPoint->m_fPosX )
									CTBSPNode::m_vMaxBound.x = pPoint->m_fPosX;

								if( CTBSPNode::m_vMaxBound.y < pPoint->m_fPosY )
									CTBSPNode::m_vMaxBound.y = pPoint->m_fPosY;

								if( CTBSPNode::m_vMaxBound.z < pPoint->m_fPosZ )
									CTBSPNode::m_vMaxBound.z = pPoint->m_fPosZ;

								if( CTBSPNode::m_vMinBound.x > pPoint->m_fPosX )
									CTBSPNode::m_vMinBound.x = pPoint->m_fPosX;

								if( CTBSPNode::m_vMinBound.y > pPoint->m_fPosY )
									CTBSPNode::m_vMinBound.y = pPoint->m_fPosY;

								if( CTBSPNode::m_vMinBound.z > pPoint->m_fPosZ )
									CTBSPNode::m_vMinBound.z = pPoint->m_fPosZ;
							}
						}

						D3DXVECTOR3 vPoint[3] = {
							D3DXVECTOR3(
							pWinding->m_vPoint[0]->m_fPosX,
							pWinding->m_vPoint[0]->m_fPosY,
							pWinding->m_vPoint[0]->m_fPosZ),
							D3DXVECTOR3(
							pWinding->m_vPoint[1]->m_fPosX,
							pWinding->m_vPoint[1]->m_fPosY,
							pWinding->m_vPoint[1]->m_fPosZ),
							D3DXVECTOR3(
							pWinding->m_vPoint[2]->m_fPosX,
							pWinding->m_vPoint[2]->m_fPosY,
							pWinding->m_vPoint[2]->m_fPosZ)};

						D3DXVECTOR3 vCross(
							0.0f,
							0.0f,
							0.0f);

						D3DXVec3Cross(
							&vCross,
							&(vPoint[1] - vPoint[0]),
							&(vPoint[2] - vPoint[0]));

						if( vCross != D3DXVECTOR3( 0.0f, 0.0f, 0.0f) )
						{
							D3DXPlaneFromPoints(
								&pWinding->m_vPlane,
								&vPoint[0],
								&vPoint[1],
								&vPoint[2]);

							pWinding->m_bCheck = FALSE;
							pWinding->m_nMeshIndex = i;

							vWinding.push_back(pWinding);
						}
						else
							delete pWinding;
					}
				}
			}

			m_Head.Overlap(&vWinding);
			vWinding.clear();

			if(pBUF)
				delete[] pBUF;
		}

		break;

	default	: Clear(); break;
	}
}

void CTachyonBSPMAP::LoadFromTMF( LPARAM lParam, LPBYTE pData)
{
	if(!pData)
		return;

	m_Head.ClearPortal();
	m_Head.ClearNode();
	m_nTexIDX = 0;

	int nTMFVersion = *((int*) pData); pData += sizeof(int);
	switch(nTMFVersion)
	{
	case 300	:
		{
			VECTORVERTEXWINDING vWinding;
			VECTORTBSPPORTAL vPortal;

			LPMESHVERTEX pVB = NULL;
			LPBYTE pBUF = NULL;

			DWORD dwBonesCount = 0;
			DWORD dwCount = 0;
			BOOL bInit = FALSE;

			dwBonesCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			pData += sizeof(FLOAT);
			pData += sizeof(D3DXVECTOR3);
			vWinding.clear();
			vPortal.clear();

			if( dwBonesCount > 0 )
				pData += sizeof(D3DMATRIX) * dwBonesCount;

			dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			if( dwCount > 0 )
			{
				LPWMESHVERTEX pDATA = (LPWMESHVERTEX) pData;

				pBUF = new BYTE[dwCount * sizeof(MESHVERTEX)];
				pVB = (LPMESHVERTEX) pBUF;
				pData += dwCount * sizeof(WMESHVERTEX);

				for( DWORD i=0; i<dwCount; i++)
				{
					pVB[i].m_fNormalX = pDATA[i].m_fNormalX;
					pVB[i].m_fNormalY = pDATA[i].m_fNormalY;
					pVB[i].m_fNormalZ = pDATA[i].m_fNormalZ;

					pVB[i].m_fPosX = pDATA[i].m_fPosX;
					pVB[i].m_fPosY = pDATA[i].m_fPosY;
					pVB[i].m_fPosZ = pDATA[i].m_fPosZ;

					pVB[i].m_fU1 = pDATA[i].m_fU;
					pVB[i].m_fV1 = pDATA[i].m_fV;
					pVB[i].m_fU2 = pDATA[i].m_fU;
					pVB[i].m_fV2 = pDATA[i].m_fV;
				}
			}

			m_dwMeshCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			for( DWORD i=0; i<m_dwMeshCount; i++)
			{
				dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
				dwCount /= 3;

				if( dwCount > 0 )
				{
					for( DWORD j=0; j<dwCount; j++)
					{
						LPVERTEXWINDING pWinding = new VERTEXWINDING;

						for( BYTE k=0; k<3; k++)
						{
							LPMESHVERTEX pPoint = new MESHVERTEX;

							memcpy( pPoint, &pVB[*((LPWORD) pData)], sizeof(MESHVERTEX));
							pData += sizeof(WORD);
							pWinding->m_vPoint.push_back(pPoint);

							if(!bInit)
							{
								CTBSPNode::m_vMinBound = D3DXVECTOR3(
									pPoint->m_fPosX,
									pPoint->m_fPosY,
									pPoint->m_fPosZ);

								CTBSPNode::m_vMaxBound = CTBSPNode::m_vMinBound;
								bInit = TRUE;
							}
							else
							{
								if( CTBSPNode::m_vMaxBound.x < pPoint->m_fPosX )
									CTBSPNode::m_vMaxBound.x = pPoint->m_fPosX;

								if( CTBSPNode::m_vMaxBound.y < pPoint->m_fPosY )
									CTBSPNode::m_vMaxBound.y = pPoint->m_fPosY;

								if( CTBSPNode::m_vMaxBound.z < pPoint->m_fPosZ )
									CTBSPNode::m_vMaxBound.z = pPoint->m_fPosZ;

								if( CTBSPNode::m_vMinBound.x > pPoint->m_fPosX )
									CTBSPNode::m_vMinBound.x = pPoint->m_fPosX;

								if( CTBSPNode::m_vMinBound.y > pPoint->m_fPosY )
									CTBSPNode::m_vMinBound.y = pPoint->m_fPosY;

								if( CTBSPNode::m_vMinBound.z > pPoint->m_fPosZ )
									CTBSPNode::m_vMinBound.z = pPoint->m_fPosZ;
							}
						}

						D3DXVECTOR3 vPoint[3] = {
							D3DXVECTOR3(
							pWinding->m_vPoint[0]->m_fPosX,
							pWinding->m_vPoint[0]->m_fPosY,
							pWinding->m_vPoint[0]->m_fPosZ),
							D3DXVECTOR3(
							pWinding->m_vPoint[1]->m_fPosX,
							pWinding->m_vPoint[1]->m_fPosY,
							pWinding->m_vPoint[1]->m_fPosZ),
							D3DXVECTOR3(
							pWinding->m_vPoint[2]->m_fPosX,
							pWinding->m_vPoint[2]->m_fPosY,
							pWinding->m_vPoint[2]->m_fPosZ)};

						D3DXVECTOR3 vCross(
							0.0f,
							0.0f,
							0.0f);

						D3DXVec3Cross(
							&vCross,
							&(vPoint[1] - vPoint[0]),
							&(vPoint[2] - vPoint[0]));

						if( vCross != D3DXVECTOR3( 0.0f, 0.0f, 0.0f) )
						{
							D3DXPlaneFromPoints(
								&pWinding->m_vPlane,
								&vPoint[0],
								&vPoint[1],
								&vPoint[2]);

							pWinding->m_bCheck = FALSE;
							pWinding->m_nMeshIndex = i;

							vWinding.push_back(pWinding);
						}
						else
							delete pWinding;
					}
				}
			}

			if(CTBSPNode::m_pProcLevel)
				CTBSPNode::m_pProcLevel( lParam, 0, "Build Node");

			if(CTBSPNode::m_pProcStep)
				CTBSPNode::m_pProcStep( lParam, 0);
			m_Head.BuildNode( lParam, &vWinding);

			if(CTBSPNode::m_pProcLevel)
				CTBSPNode::m_pProcLevel( lParam, 0, "Build Portal");

			if(CTBSPNode::m_pProcStep)
				CTBSPNode::m_pProcStep( lParam, 0);
			BuildHeadPortal(&vPortal);
			m_Head.BuildPortal( lParam, &vPortal);

			if(CTBSPNode::m_pProcLevel)
				CTBSPNode::m_pProcLevel( lParam, 0, "Regulate Portal");

			if(CTBSPNode::m_pProcStep)
				CTBSPNode::m_pProcStep( lParam, 0);
			m_Head.RemoveInvalidPortal( lParam, &vPortal);

			if(CTBSPNode::m_pProcLevel)
				CTBSPNode::m_pProcLevel( lParam, 0, "Regulate Node");

			if(CTBSPNode::m_pProcStep)
				CTBSPNode::m_pProcStep( lParam, 0);
			m_Head.RemoveInvalidNode(lParam);

			if(m_bPVS)
				m_Head.BuildPVS(lParam);

			vWinding.clear();
			vPortal.clear();

			if(pBUF)
				delete[] pBUF;
		}

		break;

	case 301	:
		{
			VECTORVERTEXWINDING vWinding;
			VECTORTBSPPORTAL vPortal;

			LPMESHVERTEX pVB = NULL;
			LPBYTE pBUF = NULL;

			DWORD dwBonesCount = 0;
			DWORD dwCount = 0;
			BOOL bInit = FALSE;

			dwBonesCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			pData += sizeof(FLOAT);
			pData += sizeof(D3DXVECTOR3);
			vWinding.clear();
			vPortal.clear();

			if( dwBonesCount > 0 )
				pData += sizeof(D3DMATRIX) * dwBonesCount;

			dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			if( dwCount > 0 )
			{
				LPTMESHBUFFER pDATA = (LPTMESHBUFFER) pData;

				pBUF = new BYTE[dwCount * sizeof(MESHVERTEX)];
				pVB = (LPMESHVERTEX) pBUF;
				pData += dwCount * sizeof(TMESHBUFFER);

				for( DWORD i=0; i<dwCount; i++)
				{
					pVB[i].m_fNormalX = pDATA[i].m_fNormalX;
					pVB[i].m_fNormalY = pDATA[i].m_fNormalY;
					pVB[i].m_fNormalZ = pDATA[i].m_fNormalZ;

					pVB[i].m_fPosX = pDATA[i].m_fPosX;
					pVB[i].m_fPosY = pDATA[i].m_fPosY;
					pVB[i].m_fPosZ = pDATA[i].m_fPosZ;

					pVB[i].m_fU1 = pDATA[i].m_fU1;
					pVB[i].m_fV1 = pDATA[i].m_fV1;
					pVB[i].m_fU2 = pDATA[i].m_fU2;
					pVB[i].m_fV2 = pDATA[i].m_fV2;
				}
			}

			m_dwMeshCount = *((LPDWORD) pData); pData += sizeof(DWORD);
			for( DWORD i=0; i<m_dwMeshCount; i++)
			{
				dwCount = *((LPDWORD) pData); pData += sizeof(DWORD);
				dwCount /= 3;

				if( dwCount > 0 )
				{
					for( DWORD j=0; j<dwCount; j++)
					{
						LPVERTEXWINDING pWinding = new VERTEXWINDING;

						for( BYTE k=0; k<3; k++)
						{
							LPMESHVERTEX pPoint = new MESHVERTEX;

							memcpy( pPoint, &pVB[*((LPWORD) pData)], sizeof(MESHVERTEX));
							pData += sizeof(WORD);
							pWinding->m_vPoint.push_back(pPoint);

							if(!bInit)
							{
								CTBSPNode::m_vMinBound = D3DXVECTOR3(
									pPoint->m_fPosX,
									pPoint->m_fPosY,
									pPoint->m_fPosZ);

								CTBSPNode::m_vMaxBound = CTBSPNode::m_vMinBound;
								bInit = TRUE;
							}
							else
							{
								if( CTBSPNode::m_vMaxBound.x < pPoint->m_fPosX )
									CTBSPNode::m_vMaxBound.x = pPoint->m_fPosX;

								if( CTBSPNode::m_vMaxBound.y < pPoint->m_fPosY )
									CTBSPNode::m_vMaxBound.y = pPoint->m_fPosY;

								if( CTBSPNode::m_vMaxBound.z < pPoint->m_fPosZ )
									CTBSPNode::m_vMaxBound.z = pPoint->m_fPosZ;

								if( CTBSPNode::m_vMinBound.x > pPoint->m_fPosX )
									CTBSPNode::m_vMinBound.x = pPoint->m_fPosX;

								if( CTBSPNode::m_vMinBound.y > pPoint->m_fPosY )
									CTBSPNode::m_vMinBound.y = pPoint->m_fPosY;

								if( CTBSPNode::m_vMinBound.z > pPoint->m_fPosZ )
									CTBSPNode::m_vMinBound.z = pPoint->m_fPosZ;
							}
						}

						D3DXVECTOR3 vPoint[3] = {
							D3DXVECTOR3(
							pWinding->m_vPoint[0]->m_fPosX,
							pWinding->m_vPoint[0]->m_fPosY,
							pWinding->m_vPoint[0]->m_fPosZ),
							D3DXVECTOR3(
							pWinding->m_vPoint[1]->m_fPosX,
							pWinding->m_vPoint[1]->m_fPosY,
							pWinding->m_vPoint[1]->m_fPosZ),
							D3DXVECTOR3(
							pWinding->m_vPoint[2]->m_fPosX,
							pWinding->m_vPoint[2]->m_fPosY,
							pWinding->m_vPoint[2]->m_fPosZ)};

						D3DXVECTOR3 vCross(
							0.0f,
							0.0f,
							0.0f);

						D3DXVec3Cross(
							&vCross,
							&(vPoint[1] - vPoint[0]),
							&(vPoint[2] - vPoint[0]));

						if( vCross != D3DXVECTOR3( 0.0f, 0.0f, 0.0f) )
						{
							D3DXPlaneFromPoints(
								&pWinding->m_vPlane,
								&vPoint[0],
								&vPoint[1],
								&vPoint[2]);

							pWinding->m_bCheck = FALSE;
							pWinding->m_nMeshIndex = i;

							vWinding.push_back(pWinding);
						}
						else
							delete pWinding;
					}
				}
			}

			if(CTBSPNode::m_pProcLevel)
				CTBSPNode::m_pProcLevel( lParam, 0, "Build Node");

			if(CTBSPNode::m_pProcStep)
				CTBSPNode::m_pProcStep( lParam, 0);
			m_Head.BuildNode( lParam, &vWinding);

			if(CTBSPNode::m_pProcLevel)
				CTBSPNode::m_pProcLevel( lParam, 0, "Build Portal");

			if(CTBSPNode::m_pProcStep)
				CTBSPNode::m_pProcStep( lParam, 0);
			BuildHeadPortal(&vPortal);
			m_Head.BuildPortal( lParam, &vPortal);

			if(CTBSPNode::m_pProcLevel)
				CTBSPNode::m_pProcLevel( lParam, 0, "Regulate Portal");

			if(CTBSPNode::m_pProcStep)
				CTBSPNode::m_pProcStep( lParam, 0);
			m_Head.RemoveInvalidPortal( lParam, &vPortal);

			if(CTBSPNode::m_pProcLevel)
				CTBSPNode::m_pProcLevel( lParam, 0, "Regulate Node");

			if(CTBSPNode::m_pProcStep)
				CTBSPNode::m_pProcStep( lParam, 0);
			m_Head.RemoveInvalidNode(lParam);

			if(m_bPVS)
				m_Head.BuildPVS(lParam);

			vWinding.clear();
			vPortal.clear();

			if(pBUF)
				delete[] pBUF;
		}

		break;

	default	: Clear(); break;
	}
}

void CTachyonBSPMAP::BuildHeadPortal( LPVECTORTBSPPORTAL pResult)
{
	D3DXVECTOR3 vPoint[6][4] = {{
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMaxBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMaxBound.z)}, {
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMaxBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMaxBound.z)}, {
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMinBound.z)}, {
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMaxBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMaxBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMaxBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMaxBound.z)}, {
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMaxBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMaxBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMinBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMinBound.z)}, {
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMaxBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMaxBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMinBound.z),
		D3DXVECTOR3( CTBSPNode::m_vMaxBound.x, CTBSPNode::m_vMinBound.y, CTBSPNode::m_vMaxBound.z)}};

	for( int i=0; i<6; i++)
	{
		CTBSPPortal *pItem = new CTBSPPortal();

		D3DXPlaneFromPoints(
			&pItem->m_vFace.m_vPlane,
			&vPoint[i][0],
			&vPoint[i][1],
			&vPoint[i][2]);

		pItem->m_bDirection = PORTALDIR_FRONT;
		pItem->m_dwFrontID = NODEID_NULL;
		pItem->m_dwBackID = NODEID_NULL;
		pItem->m_dwNodeID = NODEID_NULL;

		for( int j=0; j<4; j++)
			pItem->m_vFace.m_vPoint.push_back(new D3DXVECTOR3(vPoint[i][j]));

		pResult->push_back(pItem);
	}
}

void CTachyonBSPMAP::LoadMAP( CFile* pFile)
{
	LPMESHVERTEX pMESH = NULL;
	LPLVERTEX pLIGHT = NULL;

	int nLIGHT = 0;
	int nMESH = 0;
	int nCount;

	if(!pFile)
		return;
	Clear();

	pFile->Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		CD3DLight *pLIGHT = new CD3DLight();
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pFile->Read( &pLIGHT->m_Light, sizeof(D3DLIGHT9));
		pLIGHT->SetLight(&pLIGHT->m_Light);

		m_mapLIGHT.insert( MAPD3DLIGHT::value_type( dwID, pLIGHT));
	}

	pFile->Read( &m_dwMeshCount, sizeof(m_dwMeshCount));
	pFile->Read( &m_dwAmbient, sizeof(m_dwAmbient));
	pFile->Read( &m_bPVS, sizeof(m_bPVS));
	pFile->Read( &nCount, sizeof(int));

	if( nCount > 0 )
	{
		CTBSPNode::m_pDevice->m_pDevice->CreateVertexBuffer(
			nCount * sizeof(MESHVERTEX),
			0, T3DFVF_MESHVERTEX,
			D3DPOOL_MANAGED,
			&m_pMESH, NULL);

		m_pMESH->Lock( 0, 0, (LPVOID *) &pMESH, 0);
	}
	pFile->Read( &nCount, sizeof(int));

	if( nCount > 0 )
	{
		CTBSPNode::m_pDevice->m_pDevice->CreateVertexBuffer(
			nCount * sizeof(LVERTEX),
			0, T3DFVF_LVERTEX,
			D3DPOOL_MANAGED,
			&m_pLIGHT, NULL);

		m_pLIGHT->Lock( 0, 0, (LPVOID *) &pLIGHT, 0);
	}

	m_Head.LoadMAP(
		pFile,
		pMESH,
		pLIGHT,
		nMESH,
		nLIGHT);

	if(m_pLIGHT)
		m_pLIGHT->Unlock();

	if(m_pMESH)
		m_pMESH->Unlock();
}

void CTachyonBSPMAP::SaveMAP( CFile* pFile)
{
	MAPVECTORTEXWINDING::iterator itLIGHT;
	MAPVERTEXWINDING::iterator itMESH;

	MAPVECTORTEXWINDING mapLIGHT;
	MAPVERTEXWINDING mapWinding;
	MAPTBSPNODE mapPVS;

	MAPD3DLIGHT::iterator it;

	if(!pFile)
		return;

	mapWinding.clear();
	mapLIGHT.clear();
	mapPVS.clear();

	int nCount = INT(m_mapLIGHT.size());
	pFile->Write( &nCount, sizeof(int));

	for( it = m_mapLIGHT.begin(); it != m_mapLIGHT.end(); it++)
		if( (*it).second->m_Light.Type == D3DLIGHT_POINT )
		{
			D3DLIGHT9 vLIGHT;

			ZeroMemory( &vLIGHT, sizeof(D3DLIGHT9));
			memcpy( &vLIGHT.Diffuse, &(*it).second->m_Light.Ambient, sizeof(D3DCOLORVALUE));
			memcpy( &vLIGHT.Position, &(*it).second->m_Light.Position, sizeof(D3DVECTOR));

			vLIGHT.Range = 1.5f * (*it).second->m_Light.Range;
			vLIGHT.Type = (*it).second->m_Light.Type;

			vLIGHT.Attenuation1 = 0.75f;
			vLIGHT.Ambient.a = 1.0f;
			vLIGHT.Falloff = 1.0f;

			pFile->Write( &(*it).first, sizeof(DWORD));
			pFile->Write( &vLIGHT, sizeof(D3DLIGHT9));
		}
		else
		{
			pFile->Write( &(*it).first, sizeof(DWORD));
			pFile->Write( &(*it).second->m_Light, sizeof(D3DLIGHT9));
		}

	pFile->Write( &m_dwMeshCount, sizeof(m_dwMeshCount));
	pFile->Write( &m_dwAmbient, sizeof(m_dwAmbient));
	pFile->Write( &m_bPVS, sizeof(m_bPVS));

	AddAllNode( &mapLIGHT, &mapWinding, &mapPVS, &m_Head);
	nCount = 0;

	for( itMESH = mapWinding.begin(); itMESH != mapWinding.end(); itMESH++)
	{
		for( int i=0; i<INT((*itMESH).second->size()); i++)
			nCount += INT((*((*itMESH).second))[i]->m_vPoint.size()) - 2;

		delete (*itMESH).second;
	}
	nCount *= 3;

	pFile->Write( &nCount, sizeof(int));
	nCount = 0;

	for( itLIGHT = mapLIGHT.begin(); itLIGHT != mapLIGHT.end(); itLIGHT++)
	{
		for( int i=0; i<INT((*itLIGHT).second->size()); i++)
			for( int j=0; j<INT((*(*itLIGHT).second)[i]->m_vWINDING.size()); j++)
				nCount += (*(*itLIGHT).second)[i]->m_vWINDING[j]->m_vPoint.size() - 2;

		delete (*itLIGHT).second;
	}

	mapWinding.clear();
	mapLIGHT.clear();
	mapPVS.clear();
	nCount *= 3;

	pFile->Write( &nCount, sizeof(int));
	m_Head.Save(pFile);
}

void CTachyonBSPMAP::Load( CFile* pFile)
{
	int nCount;

	if(!pFile)
		return;
	Clear();

	pFile->Read( &nCount, sizeof(int));
	for( int i=0; i<nCount; i++)
	{
		CD3DLight *pLIGHT = new CD3DLight();
		DWORD dwID;

		pFile->Read( &dwID, sizeof(DWORD));
		pFile->Read( &pLIGHT->m_Light, sizeof(D3DLIGHT9));

		pLIGHT->m_Light.Range = min( pLIGHT->m_Light.Range, T3DLIGHT_RANGE_MAX);
		if( pLIGHT->m_Light.Type != D3DLIGHT_DIRECTIONAL &&
			pLIGHT->m_Light.Attenuation0 == 0.0f &&
			pLIGHT->m_Light.Attenuation1 == 0.0f &&
			pLIGHT->m_Light.Attenuation2 == 0.0f )
			pLIGHT->m_Light.Attenuation1 = 1.0f;
		pLIGHT->SetLight(&pLIGHT->m_Light);

		m_mapLIGHT.insert( MAPD3DLIGHT::value_type( dwID, pLIGHT));
	}

	pFile->Read( &m_dwMeshCount, sizeof(m_dwMeshCount));
	pFile->Read( &m_dwAmbient, sizeof(m_dwAmbient));
	pFile->Read( &m_bPVS, sizeof(m_bPVS));

	m_Head.Load(pFile);
}

void CTachyonBSPMAP::Save( CFile *pFile)
{
	MAPD3DLIGHT::iterator it;

	if(!pFile)
		return;

	int nCount = INT(m_mapLIGHT.size());
	pFile->Write( &nCount, sizeof(int));

	for( it = m_mapLIGHT.begin(); it != m_mapLIGHT.end(); it++)
	{
		pFile->Write( &(*it).first, sizeof(DWORD));
		pFile->Write( &(*it).second->m_Light, sizeof(D3DLIGHT9));
	}

	pFile->Write( &m_dwMeshCount, sizeof(m_dwMeshCount));
	pFile->Write( &m_dwAmbient, sizeof(m_dwAmbient));
	pFile->Write( &m_bPVS, sizeof(m_bPVS));

	m_Head.Save(pFile);
}

int CTachyonBSPMAP::HitTest( LPD3DXVECTOR3 pLayPos,
							 LPD3DXVECTOR3 pLayDir,
							 FLOAT *pDistance)
{
	return m_Head.HitTest(
		pLayPos,
		pLayDir,
		pDistance);
}

void CTachyonBSPMAP::DeleteLight( DWORD dwID)
{
	MAPD3DLIGHT::iterator finder = m_mapLIGHT.find(dwID);

	if( finder != m_mapLIGHT.end() )
	{
		delete (*finder).second;
		m_mapLIGHT.erase(finder);

		m_Head.DeleteLight(dwID);
	}
}

void CTachyonBSPMAP::AddLight( LPTEXTURESET pTEX,
							   LPMAPLIGHT pLIGHT,
							   DWORD dwID)
{
	if( pLIGHT->m_bType == MLT_DIRECTION || pLIGHT->m_bUseDL )
	{
		CD3DLight *pD3DLIGHT = new CD3DLight();
		D3DLIGHT9 vLIGHT;

		ZeroMemory( &vLIGHT, sizeof(D3DLIGHT9));
		vLIGHT.Diffuse.r = ((FLOAT) GETVALUE_R(pLIGHT->m_dwDiffuse)) / 255.0f;
		vLIGHT.Diffuse.g = ((FLOAT) GETVALUE_G(pLIGHT->m_dwDiffuse)) / 255.0f;
		vLIGHT.Diffuse.b = ((FLOAT) GETVALUE_B(pLIGHT->m_dwDiffuse)) / 255.0f;
		vLIGHT.Diffuse.a = 1.0f;

		vLIGHT.Ambient.r = ((FLOAT) GETVALUE_R(pLIGHT->m_dwAmbient)) / 255.0f;
		vLIGHT.Ambient.g = ((FLOAT) GETVALUE_G(pLIGHT->m_dwAmbient)) / 255.0f;
		vLIGHT.Ambient.b = ((FLOAT) GETVALUE_B(pLIGHT->m_dwAmbient)) / 255.0f;
		vLIGHT.Ambient.a = 1.0f;

		vLIGHT.Type = pLIGHT->m_bType == MLT_DIRECTION ? D3DLIGHT_DIRECTIONAL : D3DLIGHT_POINT;
		vLIGHT.Range = pLIGHT->m_bType == MLT_DIRECTION ? T3DLIGHT_RANGE_MAX : min( T3DLIGHT_RANGE_MAX, pLIGHT->m_fRange);
		vLIGHT.Direction = pLIGHT->m_vDIR;
		vLIGHT.Position = pLIGHT->m_vPOS;

		if( vLIGHT.Type != D3DLIGHT_DIRECTIONAL )
		{
			vLIGHT.Attenuation0 = 0.0f;
			vLIGHT.Attenuation1 = 1.0f;
			vLIGHT.Attenuation2 = 0.0f;
		}

		pD3DLIGHT->SetLight(&vLIGHT);
		pD3DLIGHT->EnableLight(FALSE);

		m_mapLIGHT.insert( MAPD3DLIGHT::value_type( dwID, pD3DLIGHT));
	}

	if( pLIGHT->m_bType == MLT_POINT && pLIGHT->m_bUseLM )
		m_Head.AddLight( pTEX, pLIGHT, dwID);
}

void CTachyonBSPMAP::DisableLight()
{
	MAPD3DLIGHT::iterator itLIGHT;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		(*itLIGHT).second->EnableLight(FALSE);
}

void CTachyonBSPMAP::EnableLight()
{
	MAPD3DLIGHT::iterator itLIGHT;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		(*itLIGHT).second->EnableLight(TRUE);
}

void CTachyonBSPMAP::ResetLight()
{
	MAPD3DLIGHT::iterator itLIGHT;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		(*itLIGHT).second->SetLight();
}

FLOAT CTachyonBSPMAP::GetHeight( FLOAT fPosX,
								 FLOAT fPosY,
								 FLOAT fPosZ)
{
	return 0.0f;
}
