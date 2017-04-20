#include "StdAfx.h"
#include "TClientGame.h"


MAPTBSPMAP CTClientBSP::m_mapTBSPMAP;

CTClientBSP::CTClientBSP()
{
	D3DXMatrixIdentity(&m_vPOS);
	D3DXMatrixIdentity(&m_vINV);

	m_mapLIGHT.clear();
	m_mapMESH.clear();
	m_mapPVS.clear();
	m_vTDRAW.clear();
	m_vTLIGHT.clear();

	m_dwNodeID = NODEID_NULL;
	m_dwBspID = NODEID_NULL;
	m_dwHouseID = 0;

	m_vNodePOS = D3DXVECTOR3( 0.0f, 0.0f, 0.0f);
	m_pINFO = NULL;
	m_pMAP = NULL;

	m_fCamDIST = 0.0f;
	m_fRadius = 1.0f;
}

CTClientBSP::~CTClientBSP()
{
	Release();
}

CTachyonBSPMAP *CTClientBSP::FindTBSPMAP( DWORD dwMapID)
{
	MAPTBSPMAP::iterator finder = m_mapTBSPMAP.find(dwMapID);

	if( finder != m_mapTBSPMAP.end() )
		return (*finder).second;

	return NULL;
}

void CTClientBSP::ReleaseTBSPMAP()
{
	MAPTBSPMAP::iterator itTBSP;

	for( itTBSP = m_mapTBSPMAP.begin(); itTBSP != m_mapTBSPMAP.end(); itTBSP++)
		delete (*itTBSP).second;

	m_mapTBSPMAP.clear();
}

void CTClientBSP::InitTBSPMAP()
{
	ReleaseTBSPMAP();
}

BYTE CTClientBSP::Lock( CTachyonRes *pTRES,
					    DWORD dwBspID)
{
	if( !pTRES || !dwBspID )
		return FALSE;
	Release();

	m_pINFO = CTChart::GetTMAPINFO(dwBspID);
	if(!m_pINFO)
		return FALSE;

	m_pMAP = FindTBSPMAP(m_pINFO->m_dwMapID);
	if(!m_pMAP)
	{
		m_pMAP = (CTachyonBSPMAP *) pTRES->LockMAP(m_pINFO->m_dwMapID);

		if(!m_pMAP)
		{
			Release();
			return FALSE;
		}

		m_mapTBSPMAP.insert( MAPTBSPMAP::value_type( m_pINFO->m_dwMapID, m_pMAP));
	}

	MAPDWORD::iterator finder = m_pMAP->m_mapTREF.find(dwBspID);
	if( finder != m_pMAP->m_mapTREF.end() )
	{
		Release();
		return FALSE;
	}

	m_pMAP->m_mapTREF.insert( MAPDWORD::value_type( dwBspID, dwBspID));
	m_dwBspID = dwBspID;

	return TRUE;
}

void CTClientBSP::Release()
{
	MAPVECTORTEXWINDING::iterator itLIGHT;
	MAPVERTEXWINDING::iterator itMESH;

	for( itLIGHT = m_mapLIGHT.begin(); itLIGHT != m_mapLIGHT.end(); itLIGHT++)
		delete (*itLIGHT).second;

	for( itMESH = m_mapMESH.begin(); itMESH != m_mapMESH.end(); itMESH++)
		delete (*itMESH).second;

	while(!m_vTLIGHT.empty())
	{
		delete m_vTLIGHT.back();
		m_vTLIGHT.pop_back();
	}

	m_mapLIGHT.clear();
	m_mapMESH.clear();
	m_mapPVS.clear();
	m_vTDRAW.clear();

	if(m_pMAP)
	{
		MAPDWORD::iterator finder = m_pMAP->m_mapTREF.find(m_dwBspID);

		if( finder != m_pMAP->m_mapTREF.end() )
			m_pMAP->m_mapTREF.erase(finder);

		if( m_pINFO && m_pMAP->m_mapTREF.empty() )
		{
			MAPTBSPMAP::iterator itTBSP = m_mapTBSPMAP.find(m_pINFO->m_dwMapID);

			if( itTBSP != m_mapTBSPMAP.end() )
				m_mapTBSPMAP.erase(itTBSP);

			delete m_pMAP;
		}

		m_pMAP = NULL;
	}

	m_dwNodeID = NODEID_NULL;
	m_dwBspID = NODEID_NULL;
	m_dwHouseID = 0;
	m_pINFO = NULL;

	D3DXMatrixIdentity(&m_vPOS);
	D3DXMatrixIdentity(&m_vINV);
}

void CTClientBSP::InitTBSP( LPTBSPINFO pTDATA)
{
	MAPD3DLIGHT::iterator itLIGHT;
	MAPMAPOBJ::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;
	D3DXMATRIX vROT;

	D3DXMatrixScaling(
		&m_vPOS,
		pTDATA->m_vSCALE.x,
		pTDATA->m_vSCALE.y,
		pTDATA->m_vSCALE.z);

	D3DXMatrixRotationX(
		&m_vINV,
		pTDATA->m_vROT.x);
	m_vPOS *= m_vINV;

	D3DXMatrixRotationY(
		&m_vINV,
		pTDATA->m_vROT.y);
	m_vPOS *= m_vINV;

	D3DXMatrixRotationZ(
		&m_vINV,
		pTDATA->m_vROT.z);
	m_vPOS *= m_vINV;
	vROT = m_vPOS;

	m_vPOS._41 = pTDATA->m_vPOS.x;
	m_vPOS._42 = pTDATA->m_vPOS.y;
	m_vPOS._43 = pTDATA->m_vPOS.z;
	m_vINV = CTMath::Inverse(&m_vPOS);

	for( itLIGHT = m_pMAP->m_mapLIGHT.begin(); itLIGHT != m_pMAP->m_mapLIGHT.end(); itLIGHT++)
	{
		LPTLIGHTPOS pTLIGHT = new TLIGHTPOS;

		pTLIGHT->m_dwID = (*itLIGHT).first;
		pTLIGHT->m_vDIR = D3DXVECTOR3(
			(*itLIGHT).second->m_Light.Direction.x,
			(*itLIGHT).second->m_Light.Direction.y,
			(*itLIGHT).second->m_Light.Direction.z);

		pTLIGHT->m_vPOS = D3DXVECTOR3(
			(*itLIGHT).second->m_Light.Position.x,
			(*itLIGHT).second->m_Light.Position.y,
			(*itLIGHT).second->m_Light.Position.z);

		CTMath::Transform(
			&vROT,
			&pTLIGHT->m_vDIR);

		CTMath::Transform(
			&m_vPOS,
			&pTLIGHT->m_vPOS);

		m_vTLIGHT.push_back(pTLIGHT);
	}
}

void CTClientBSP::ResetTLIGHT()
{
	if(!m_pMAP)
		return;

	for( int i=0; i<INT(m_vTLIGHT.size()); i++)
	{
		MAPD3DLIGHT::iterator finder = m_pMAP->m_mapLIGHT.find(m_vTLIGHT[i]->m_dwID);

		if( finder != m_pMAP->m_mapLIGHT.end() )
			(*finder).second->SetPosition( &m_vTLIGHT[i]->m_vPOS, &m_vTLIGHT[i]->m_vDIR);
	}
}

void CTClientBSP::ResetPVS( DWORD dwNodeID)
{
	if( !m_pINFO || !m_pMAP )
		return;

	if( m_dwNodeID != dwNodeID )
	{
		m_pMAP->BuildPVS(
			&m_mapLIGHT,
			&m_mapMESH,
			&m_mapPVS,
			dwNodeID);

		m_dwNodeID = dwNodeID;
	}
}

void CTClientBSP::Render()
{
	if( !m_pINFO || !m_pMAP )
		return;

	m_pMAP->m_bLIGHTMAP = CTClientGame::GetLIGHTMAPOption(m_pINFO->m_bDungeon);
	CTBSPNode::m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &m_vPOS);

	if(m_pMAP->m_bLIGHTMAP)
		ResetTLIGHT();

	m_pMAP->Render(
		&m_mapLIGHT,
		&m_mapMESH,
		&m_mapPVS);
}

DWORD CTClientBSP::GetNodeID( D3DXVECTOR3 vPOS)
{
	if( !m_pINFO || !m_pMAP )
		return NODEID_NULL;

	CTMath::Transform(
		&m_vINV,
		&vPOS);

	CTBSPNode *pNode = m_pMAP->m_Head.FindLeaf(&vPOS);
	if( pNode && pNode->IsLeaf() )
		return pNode->m_dwID;

	return NODEID_NULL;
}
