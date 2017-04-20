#include "StdAfx.h"
#include "Resource.h"


CTMapViewerBSP::CTMapViewerBSP()
{
	D3DXMatrixIdentity(&m_vPOS);
	D3DXMatrixIdentity(&m_vINV);

	m_mapLIGHT.clear();
	m_mapMESH.clear();
	m_mapPVS.clear();
	m_vTDRAW.clear();

	m_dwNodeID = NODEID_NULL;
	m_dwBspID = NODEID_NULL;

	m_pINFO = NULL;
	m_pMAP = NULL;
}

CTMapViewerBSP::~CTMapViewerBSP()
{
	Release();
}

void CTMapViewerBSP::Release()
{
	MAPVECTORTEXWINDING::iterator itLIGHT;
	MAPVERTEXWINDING::iterator itMESH;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		delete (*itLIGHT).second;

	for( itMESH = m_mapMESH.begin(); itMESH != m_mapMESH.end(); itMESH++)
		delete (*itMESH).second;

	m_mapLIGHT.clear();
	m_mapMESH.clear();
	m_mapPVS.clear();
	m_vTDRAW.clear();

	if(m_pMAP)
	{
		delete m_pMAP;
		m_pMAP = NULL;
	}

	m_dwNodeID = NODEID_NULL;
	m_dwBspID = NODEID_NULL;
	m_pINFO = NULL;

	D3DXMatrixIdentity(&m_vPOS);
	D3DXMatrixIdentity(&m_vINV);
}

void CTMapViewerBSP::InitTBSP( LPMAPOBJ pTOBJ)
{
	MAPD3DLIGHT::iterator itLIGHT;
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;
	D3DXMATRIX vROT;

	D3DXMatrixScaling(
		&m_vPOS,
		pTOBJ->m_vSCALE.x,
		pTOBJ->m_vSCALE.y,
		pTOBJ->m_vSCALE.z);

	D3DXMatrixRotationX(
		&m_vINV,
		pTOBJ->m_vROT.x);
	m_vPOS *= m_vINV;

	D3DXMatrixRotationY(
		&m_vINV,
		pTOBJ->m_vROT.y);
	m_vPOS *= m_vINV;

	D3DXMatrixRotationZ(
		&m_vINV,
		pTOBJ->m_vROT.z);
	m_vPOS *= m_vINV;
	vROT = m_vPOS;

	m_vPOS._41 = pTOBJ->m_vPOS.x;
	m_vPOS._42 = pTOBJ->m_vPOS.y;
	m_vPOS._43 = pTOBJ->m_vPOS.z;
	m_vINV = CTMath::Inverse(&m_vPOS);

	for( itLIGHT = m_pMAP->m_mapLIGHT.begin(); itLIGHT != m_pMAP->m_mapLIGHT.end(); itLIGHT++)
	{
		D3DXVECTOR3 vPOS(
			(*itLIGHT).second->m_Light.Direction.x,
			(*itLIGHT).second->m_Light.Direction.y,
			(*itLIGHT).second->m_Light.Direction.z);

		CTMath::Transform(
			&vROT,
			&vPOS);

		(*itLIGHT).second->m_Light.Direction.x = vPOS.x;
		(*itLIGHT).second->m_Light.Direction.y = vPOS.y;
		(*itLIGHT).second->m_Light.Direction.z = vPOS.z;

		vPOS.x = (*itLIGHT).second->m_Light.Position.x;
		vPOS.y = (*itLIGHT).second->m_Light.Position.y;
		vPOS.z = (*itLIGHT).second->m_Light.Position.z;

		CTMath::Transform(
			&m_vPOS,
			&vPOS);

		(*itLIGHT).second->m_Light.Position.x = vPOS.x;
		(*itLIGHT).second->m_Light.Position.y = vPOS.y;
		(*itLIGHT).second->m_Light.Position.z = vPOS.z;

		(*itLIGHT).second->SetLight(&(*itLIGHT).second->m_Light);
	}
}

void CTMapViewerBSP::Render( DWORD dwNodeID)
{
	if( m_dwNodeID != dwNodeID )
	{
		m_pMAP->BuildPVS(
			&m_mapLIGHT,
			&m_mapMESH,
			&m_mapPVS,
			dwNodeID);

		m_dwNodeID = dwNodeID;
	}

	CTBSPNode::m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &m_vPOS);
	m_pMAP->Render(
		&m_mapLIGHT,
		&m_mapMESH,
		&m_mapPVS);
}

DWORD CTMapViewerBSP::GetNodeID( D3DXVECTOR3 vPOS)
{
	CTMath::Transform(
		&m_vINV,
		&vPOS);

	CTBSPNode *pNode = m_pMAP->m_Head.FindLeaf(&vPOS);
	if( pNode && pNode->IsLeaf() )
		return pNode->m_dwID;

	return NODEID_NULL;
}
