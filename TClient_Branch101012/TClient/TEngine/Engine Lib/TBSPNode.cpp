#include "StdAfx.h"


D3DXVECTOR3 CTBSPNode::m_vMaxBound( 0.0f, 0.0f, 0.0f);
D3DXVECTOR3 CTBSPNode::m_vMinBound( 0.0f, 0.0f, 0.0f);
CD3DDevice *CTBSPNode::m_pDevice = NULL;

FNPROCLEVEL CTBSPNode::m_pProcLevel = NULL;
FNPROCSTEP CTBSPNode::m_pProcStep = NULL;

MAPTBSPNODE CTBSPNode::m_mapTBSPNode;

BYTE CTBSPNode::m_bBaseNodeID = 0xFF;
DWORD CTBSPNode::m_dwNodeID = 0;


CTBSPNode::CTBSPNode()
{
	m_pFrontNode = NULL;
	m_pBackNode = NULL;
	m_bValid = TRUE;

	m_mapLIGHT.clear();
	m_vPortal.clear();

	m_dwID = NODEID_NULL;
	GetNewID(this);
}

CTBSPNode::~CTBSPNode()
{
	ClearPortal();
	ClearNode();
	ClearLightMap();

	ReleaseID(m_dwID);
}

void CTBSPNode::InitCommon( CD3DDevice *pDevice)
{
	CTBSPPortal::InitCommon();

	m_mapTBSPNode.clear();
	m_pDevice = pDevice;

	m_dwNodeID = 0;
}

void CTBSPNode::ReleaseCommon()
{
	MAPTBSPNODE::iterator it;

	for( it = m_mapTBSPNode.begin(); it != m_mapTBSPNode.end(); it++)
	{
		CTBSPNode *pItem = (*it).second;

		if(pItem)
			delete pItem;
	}

	m_mapTBSPNode.clear();
	m_pDevice = NULL;
	m_dwNodeID = 0;

	CTBSPPortal::ReleaseCommon();
}

void CTBSPNode::GetNewID( CTBSPNode *pItem)
{
	if( !pItem || m_bBaseNodeID == 0xFF )
		return;

	DWORD dwNodeID = ((((DWORD) m_bBaseNodeID) << 24) & 0xFF000000) | (m_dwNodeID & 0x00FFFFFF);
	while(GetNode(dwNodeID))
	{
		m_dwNodeID++;
		if( m_dwNodeID > 0x00FFFFFF )
			m_dwNodeID = 0;

		dwNodeID = ((((DWORD) m_bBaseNodeID) << 24) & 0xFF000000) | (m_dwNodeID & 0x00FFFFFF);
	}
	pItem->m_dwID = dwNodeID;

	m_mapTBSPNode.insert( MAPTBSPNODE::value_type(
		pItem->m_dwID,
		pItem));
}

void CTBSPNode::ReleaseID( DWORD dwID)
{
	MAPTBSPNODE::iterator finder = m_mapTBSPNode.find(dwID);

	if( finder != m_mapTBSPNode.end() )
		m_mapTBSPNode.erase(finder);
}

BOOL CTBSPNode::SetID( DWORD dwID)
{
	ReleaseID(m_dwID);

	if(GetNode(dwID))
	{
		SetID(m_dwID);
		return FALSE;
	}

	if( dwID != NODEID_NULL )
	{
		m_mapTBSPNode.insert( MAPTBSPNODE::value_type(
			dwID,
			this));
	}
	m_dwID = dwID;

	return TRUE;
}

CTBSPNode *CTBSPNode::GetNode( DWORD dwID)
{
	MAPTBSPNODE::iterator finder = m_mapTBSPNode.find(dwID);

	if( finder != m_mapTBSPNode.end() )
		return (*finder).second;

	return NULL;
}

void CTBSPNode::WindingTest( LPVECTORVERTEXWINDING pData,
							 LPD3DXPLANE pPlane,
							 int *pDiff,
							 int *pCut)
{
	int nFront = 0;
	int nBack = 0;

	(*pCut) = 0;
	for( int i=0; i<pData->size(); i++)
		switch(CTMath::GetWindingPosition( (*pData)[i], pPlane))
		{
		case WINDINGPOS_ONFRONT		:
		case WINDINGPOS_FRONT		: nFront++; break;
		case WINDINGPOS_ONBACK		:
		case WINDINGPOS_BACK		: nBack++; break;
		case WINDINGPOS_NONE		: (*pCut)++; break;
		}

	(*pDiff) = INT(fabs((FLOAT) (nFront - nBack)));
}

int CTBSPNode::FindRootWinding( LPVECTORVERTEXWINDING pData)
{
	int nResult = -1;

	int nFaceDiff = 0;
	int nFaceCut = 0;

	for( int i=0; i<pData->size(); i++)
		if(!(*pData)[i]->m_bCheck)
		{
			int nDiff = 0;
			int nCut = 0;

			WindingTest(
				pData,
				&(*pData)[i]->m_vPlane,
				&nDiff,
				&nCut);

			if( nResult == -1 ||
				nFaceDiff > nDiff || (
				nFaceDiff == nDiff &&
				nFaceCut > nCut ))
			{
				nFaceDiff = nDiff;
				nFaceCut = nCut;
				nResult = i;
			}
		}

	return nResult;
}

void CTBSPNode::ClearNode()
{
	if(m_pFrontNode)
	{
		delete m_pFrontNode;
		m_pFrontNode = NULL;
	}

	if(m_pBackNode)
	{
		delete m_pBackNode;
		m_pBackNode = NULL;
	}

	while(!m_vWinding.empty())
	{
		delete m_vWinding.back();
		m_vWinding.pop_back();
	}
}

void CTBSPNode::ClearPortal()
{
	while(!m_vPortal.empty())
	{
		CTBSPPortal *pItem = m_vPortal.back();

		if(pItem)
		{
			if( pItem->m_dwFrontID == m_dwID )
				pItem->m_dwFrontID = NODEID_NULL;

			if( pItem->m_dwBackID == m_dwID )
				pItem->m_dwBackID = NODEID_NULL;

			if( pItem->m_dwFrontID == NODEID_NULL &&
				pItem->m_dwBackID == NODEID_NULL )
				delete pItem;
		}

		m_vPortal.pop_back();
	}
}

void CTBSPNode::ClearLightMap()
{
	MAPTEXWINDING::iterator it;

	if(m_pFrontNode)
		m_pFrontNode->ClearLightMap();

	if(m_pBackNode)
		m_pBackNode->ClearLightMap();

	for( it = m_mapLIGHT.begin(); it != m_mapLIGHT.end(); it++)
		delete (*it).second;

	m_mapLIGHT.clear();
}

void CTBSPNode::ClearLeaf()
{
	while(!m_vWinding.empty())
	{
		delete m_vWinding.back();
		m_vWinding.pop_back();
	}

	if(m_pFrontNode)
		m_pFrontNode->ClearLeaf();

	if(m_pBackNode)
		m_pBackNode->ClearLeaf();
}

void CTBSPNode::ClearPVS()
{
	for( int i=0; i<INT(m_vPortal.size()); i++)
	{
		m_vPortal[i]->m_mapFrontPVS.clear();
		m_vPortal[i]->m_mapBackPVS.clear();
	}

	if(m_pFrontNode)
		m_pFrontNode->ClearPVS();

	if(m_pBackNode)
		m_pBackNode->ClearPVS();
}

BOOL CTBSPNode::IsLeaf()
{
	return !m_pFrontNode && !m_pBackNode;
}

void CTBSPNode::Overlap( LPVECTORVERTEXWINDING pData)
{
	if( m_pFrontNode && m_pBackNode )
	{
		VECTORVERTEXWINDING vFront;
		VECTORVERTEXWINDING vBack;

		vFront.clear();
		vBack.clear();

		Overlap( pData, &vFront, &vBack);
		m_pFrontNode->Overlap(&vFront);
		m_pBackNode->Overlap(&vBack);

		vFront.clear();
		vBack.clear();
	}
	else if(m_pFrontNode)
		m_pFrontNode->Overlap(pData);
	else if(m_pBackNode)
		m_pBackNode->Overlap(pData);
	else
		BuildWinding(pData);
}

void CTBSPNode::Overlap( LPVECTORVERTEXWINDING pData,
						 LPVECTORVERTEXWINDING pFront,
						 LPVECTORVERTEXWINDING pBack)
{
	for( int i=0; i<pData->size(); i++)
	{
		BYTE bWindingPos = CTMath::GetWindingPosition(
			(*pData)[i],
			&m_vPlane);

		switch(bWindingPos)
		{
		case WINDINGPOS_ONFRONT		:
		case WINDINGPOS_FRONT		: pFront->push_back((*pData)[i]); break;
		case WINDINGPOS_ONBACK		:
		case WINDINGPOS_BACK		: pBack->push_back((*pData)[i]); break;
		case WINDINGPOS_NONE		:
			{
				LPVERTEXWINDING pFrontWinding = new VERTEXWINDING;
				LPVERTEXWINDING pBackWinding = new VERTEXWINDING;

				CTMath::SplitWinding(
					&m_vPlane,
					(*pData)[i],
					pFrontWinding,
					pBackWinding);

				if(!pFrontWinding->m_vPoint.empty())
					pFront->push_back(pFrontWinding);
				else
					delete pFrontWinding;

				if(!pBackWinding->m_vPoint.empty())
					pBack->push_back(pBackWinding);
				else
					delete pBackWinding;

				delete (*pData)[i];
			}

			break;
		}
	}
}

void CTBSPNode::BuildNode( LPARAM lParam, LPVECTORVERTEXWINDING pData)
{
	int nRootFace = FindRootWinding(pData);
	VECTORVERTEXWINDING vFront;
	VECTORVERTEXWINDING vBack;

	vFront.clear();
	vBack.clear();
	ClearPortal();
	ClearNode();

	InsertNode(
		pData,
		&vFront,
		&vBack,
		nRootFace);

	if(!vFront.empty())
	{
		m_pFrontNode = new CTBSPNode();
		m_pFrontNode->BuildNode( lParam, &vFront);
	}

	if(!vBack.empty())
	{
		m_pBackNode = new CTBSPNode();
		m_pBackNode->BuildNode( lParam, &vBack);
	}

	vFront.clear();
	vBack.clear();
}

void CTBSPNode::InsertNode( LPVECTORVERTEXWINDING pData,
						    LPVECTORVERTEXWINDING pFront,
							LPVECTORVERTEXWINDING pBack,
							int nRootIndex)
{
	if( nRootIndex >= 0 )
	{
		m_vPlane = (*pData)[nRootIndex]->m_vPlane;
		(*pData)[nRootIndex]->m_bCheck = TRUE;

		for( int i=0; i<pData->size(); i++)
		{
			BYTE bWindingPos = CTMath::GetWindingPosition(
				(*pData)[i],
				&m_vPlane);

			switch(bWindingPos)
			{
			case WINDINGPOS_ONFRONT		: (*pData)[i]->m_bCheck = TRUE;
			case WINDINGPOS_FRONT		: pFront->push_back((*pData)[i]); break;
			case WINDINGPOS_ONBACK		: (*pData)[i]->m_bCheck = TRUE;
			case WINDINGPOS_BACK		: pBack->push_back((*pData)[i]); break;
			case WINDINGPOS_NONE		:
				{
					LPVERTEXWINDING pFrontWinding = new VERTEXWINDING;
					LPVERTEXWINDING pBackWinding = new VERTEXWINDING;

					CTMath::SplitWinding(
						&m_vPlane,
						(*pData)[i],
						pFrontWinding,
						pBackWinding);

					if(!pFrontWinding->m_vPoint.empty())
						pFront->push_back(pFrontWinding);
					else
						delete pFrontWinding;

					if(!pBackWinding->m_vPoint.empty())
						pBack->push_back(pBackWinding);
					else
						delete pBackWinding;

					delete (*pData)[i];
				}

				break;
			}
		}
	}
	else
		BuildWinding(pData);
}

void CTBSPNode::BuildWinding( LPVECTORVERTEXWINDING pData)
{
	while(!m_vWinding.empty())
	{
		delete m_vWinding.back();
		m_vWinding.pop_back();
	}

	for( int i=0; i<INT(pData->size()); i++)
		m_vWinding.push_back((*pData)[i]);
}

void CTBSPNode::BuildPortal( LPARAM lParam, LPVECTORTBSPPORTAL pHead)
{
	if(!pHead)
		return;

	VECTORTBSPPORTAL vFront;
	VECTORTBSPPORTAL vBack;

	vFront.clear();
	vBack.clear();
	ClearPortal();

	InsertPortal(
		pHead,
		&vFront,
		&vBack);

	if(m_pFrontNode)
		m_pFrontNode->BuildPortal( lParam, &vFront);

	VECTORTBSPPORTAL::iterator it = vFront.begin();
	while(it != vFront.end())
	{
		CTBSPPortal *pItem = (*it);

		if( pItem && pItem->m_dwNodeID == m_dwID )
		{
			pItem->m_bDirection = PORTALDIR_BACK;
			vBack.push_back(pItem);

			vFront.erase(it);
		}
		else
			it++;
	}

	if(m_pBackNode)
		m_pBackNode->BuildPortal( lParam, &vBack);

	while(!vFront.empty())
	{
		CTBSPPortal *pItem = vFront.back();

		if(pItem)
			if( m_pFrontNode || pItem->m_dwNodeID == m_dwID )
				pHead->push_back(pItem);
			else
				delete pItem;

		vFront.pop_back();
	}

	while(!vBack.empty())
	{
		CTBSPPortal *pItem = vBack.back();

		if(pItem)
			if( m_pBackNode || pItem->m_dwNodeID == m_dwID )
				pHead->push_back(pItem);
			else
				delete pItem;

		vBack.pop_back();
	}
}

void CTBSPNode::InsertPortal( LPVECTORTBSPPORTAL pHead,
							  LPVECTORTBSPPORTAL pFront,
							  LPVECTORTBSPPORTAL pBack)
{
	if( !pHead || !pFront || !pBack )
		return;

	if(IsLeaf())
	{
		for( int i=0; i<pHead->size(); i++)
		{
			CTBSPPortal *pItem = (*pHead)[i];

			if(pItem)
				switch(pItem->m_bDirection)
				{
				case PORTALDIR_FRONT	: pItem->m_dwFrontID = m_dwID; break;
				case PORTALDIR_BACK		: pItem->m_dwBackID = m_dwID; break;
				}
		}
	}
	else
	{
		CreateNodePortal(
			pFront,
			pHead);

		for( int i=0; i<pHead->size(); i++)
		{
			BYTE bWindingPos = CTMath::GetWindingPosition(
				&(*pHead)[i]->m_vFace,
				&m_vPlane);

			switch(bWindingPos)
			{
			case WINDINGPOS_ONFRONT	:
			case WINDINGPOS_FRONT	: pFront->push_back((*pHead)[i]); break;
			case WINDINGPOS_ONBACK	:
			case WINDINGPOS_BACK	: pBack->push_back((*pHead)[i]); break;
			case WINDINGPOS_NONE	:
				{
					CTBSPPortal *pFrontPortal = new CTBSPPortal();
					CTBSPPortal *pBackPortal = new CTBSPPortal();

					pFrontPortal->m_bDirection = (*pHead)[i]->m_bDirection;
					pFrontPortal->m_dwFrontID = (*pHead)[i]->m_dwFrontID;
					pFrontPortal->m_dwBackID = (*pHead)[i]->m_dwBackID;
					pFrontPortal->m_dwNodeID = (*pHead)[i]->m_dwNodeID;

					pBackPortal->m_bDirection = (*pHead)[i]->m_bDirection;
					pBackPortal->m_dwFrontID = (*pHead)[i]->m_dwFrontID;
					pBackPortal->m_dwBackID = (*pHead)[i]->m_dwBackID;
					pBackPortal->m_dwNodeID = (*pHead)[i]->m_dwNodeID;

					CTMath::SplitWinding(
						&m_vPlane,
						&(*pHead)[i]->m_vFace,
						&pFrontPortal->m_vFace,
						&pBackPortal->m_vFace);

					if(!pFrontPortal->m_vFace.m_vPoint.empty())
						pFront->push_back(pFrontPortal);
					else
						delete pFrontPortal;

					if(!pBackPortal->m_vFace.m_vPoint.empty())
						pBack->push_back(pBackPortal);
					else
						delete pBackPortal;

					delete (*pHead)[i];
				}

				break;
			}
		}

		pHead->clear();
	}
}

void CTBSPNode::CreateBigWinding( LPPOINTWINDING pResult,
								  LPD3DXVECTOR3 pMinPoint,
								  LPD3DXVECTOR3 pMaxPoint,
								  LPD3DXPLANE pPlane)
{
	D3DXVECTOR3 vPoint[4];
	D3DXPlaneNormalize(
		pPlane,
		pPlane);

	FLOAT fAxisX = fabs(pPlane->a);
	FLOAT fAxisY = fabs(pPlane->b);
	FLOAT fAxisZ = fabs(pPlane->c);
	FLOAT fMAX = max( max( fAxisX, fAxisY), fAxisZ);

	while(!pResult->m_vPoint.empty())
	{
		delete pResult->m_vPoint.back();
		pResult->m_vPoint.pop_back();
	}

	if( fMAX < DEF_EPSILON )
		return;

	if( fAxisX == fMAX )
	{
		vPoint[0] = D3DXVECTOR3( 0.0f, pMinPoint->y, pMinPoint->z);
		vPoint[1] = D3DXVECTOR3( 0.0f, pMaxPoint->y, pMinPoint->z);
		vPoint[2] = D3DXVECTOR3( 0.0f, pMaxPoint->y, pMaxPoint->z);
		vPoint[3] = D3DXVECTOR3( 0.0f, pMinPoint->y, pMaxPoint->z);

		for( int i=0; i<4; i++)
			vPoint[i].x = -(pPlane->b * vPoint[i].y + pPlane->c * vPoint[i].z + pPlane->d) / pPlane->a;
	}
	else if( fAxisY == fMAX )
	{
		vPoint[0] = D3DXVECTOR3( pMinPoint->x, 0.0f, pMinPoint->z);
		vPoint[1] = D3DXVECTOR3( pMinPoint->x, 0.0f, pMaxPoint->z);
		vPoint[2] = D3DXVECTOR3( pMaxPoint->x, 0.0f, pMaxPoint->z);
		vPoint[3] = D3DXVECTOR3( pMaxPoint->x, 0.0f, pMinPoint->z);

		for( int i=0; i<4; i++)
			vPoint[i].y = -(pPlane->a * vPoint[i].x + pPlane->c * vPoint[i].z + pPlane->d) / pPlane->b;
	}
	else
	{
		vPoint[0] = D3DXVECTOR3( pMinPoint->x, pMinPoint->y, 0.0f);
		vPoint[1] = D3DXVECTOR3( pMinPoint->x, pMaxPoint->y, 0.0f);
		vPoint[2] = D3DXVECTOR3( pMaxPoint->x, pMaxPoint->y, 0.0f);
		vPoint[3] = D3DXVECTOR3( pMaxPoint->x, pMinPoint->y, 0.0f);

		for( int i=0; i<4; i++)
			vPoint[i].z = -(pPlane->a * vPoint[i].x + pPlane->b * vPoint[i].y + pPlane->d) / pPlane->c;
	}

	D3DXVECTOR3 vNormal(
		pPlane->a,
		pPlane->b,
		pPlane->c);
	D3DXVECTOR3 vCross;

	D3DXVec3Cross(
		&vCross,
		&(vPoint[1] - vPoint[0]),
		&(vPoint[2] - vPoint[0]));

	vNormal /= D3DXVec3Length(&vNormal);
	vCross /= D3DXVec3Length(&vCross);

	if( D3DXVec3Dot( &vNormal, &vCross) < 0.0f )
	{
		vCross = vPoint[1];

		vPoint[1] = vPoint[3];
		vPoint[3] = vCross;
	}

	for( int i=0; i<4; i++)
		pResult->m_vPoint.push_back(new D3DXVECTOR3(vPoint[i]));
}

void CTBSPNode::CreateNodePortal( LPVECTORTBSPPORTAL pResult,
								  LPVECTORTBSPPORTAL pHead)
{
	if( !pResult || !pHead )
		return;
	CTBSPPortal *pItem = new CTBSPPortal();

	pItem->m_bDirection = PORTALDIR_FRONT;
	pItem->m_dwFrontID = NODEID_NULL;
	pItem->m_dwBackID = NODEID_NULL;
	pItem->m_dwNodeID = m_dwID;

	pItem->m_vFace.m_vPlane = m_vPlane;
	pItem->m_vFace.m_vPoint.clear();

	CreateBigWinding(
		&pItem->m_vFace,
		&CTBSPNode::m_vMinBound,
		&CTBSPNode::m_vMaxBound,
		&m_vPlane);

	for( int i=0; i<pHead->size(); i++)
	{
		D3DXPLANE vPlane = (*pHead)[i]->m_vFace.m_vPlane;

		if( (*pHead)[i]->m_bDirection == PORTALDIR_BACK )
			vPlane = -vPlane;

		CTMath::CutWinding(
			&vPlane,
			&pItem->m_vFace);

		if(pItem->m_vFace.m_vPoint.empty())
		{
			delete pItem;
			return;
		}
	}

	if(!pItem->m_vFace.m_vPoint.empty())
		pResult->push_back(pItem);
	else
		delete pItem;
}

void CTBSPNode::RemoveInvalidNode( LPARAM lParam)
{
	if(m_pFrontNode)
	{
		m_pFrontNode->RemoveInvalidNode(lParam);

		if(!m_pFrontNode->m_bValid)
		{
			delete m_pFrontNode;
			m_pFrontNode = NULL;
		}
	}

	if(m_pBackNode)
	{
		m_pBackNode->RemoveInvalidNode(lParam);

		if(!m_pBackNode->m_bValid)
		{
			delete m_pBackNode;
			m_pBackNode = NULL;
		}
	}

	if( m_bValid && IsLeaf() && m_vPortal.empty() )
		m_bValid = FALSE;
}

BOOL CTBSPNode::IsValidPortal( CTBSPPortal *pItem)
{
	for( int i=0; i<INT(m_vWinding.size()); i++)
	{
		D3DXPLANE vPlaneW = m_vWinding[i]->m_vPlane;
		D3DXPLANE vPlaneP = pItem->m_vFace.m_vPlane;

		D3DXVECTOR3 vNormalP(
			pItem->m_vFace.m_vPlane.a,
			pItem->m_vFace.m_vPlane.b,
			pItem->m_vFace.m_vPlane.c);

		D3DXVECTOR3 vNormalW(
			m_vWinding[i]->m_vPlane.a,
			m_vWinding[i]->m_vPlane.b,
			m_vWinding[i]->m_vPlane.c);

		D3DXVec3Normalize( &vNormalP, &vNormalP);
		D3DXVec3Normalize( &vNormalW, &vNormalW);
		D3DXPlaneNormalize( &vPlaneW, &vPlaneW);
		D3DXPlaneNormalize( &vPlaneP, &vPlaneP);

		if( D3DXVec3Dot( &vNormalP, &vNormalW) < 0.0f )
			vPlaneP = -vPlaneP;

		if( fabs(vPlaneW.a - vPlaneP.a) < DEF_EPSILON &&
			fabs(vPlaneW.b - vPlaneP.b) < DEF_EPSILON &&
			fabs(vPlaneW.c - vPlaneP.c) < DEF_EPSILON &&
			fabs(vPlaneW.d - vPlaneP.d) < DEF_EPSILON )
		{
			int nCount = INT(m_vWinding[i]->m_vPoint.size());
			CTBSPPortal vCOPY(pItem);

			for( int j=0; j<nCount; j++)
			{
				D3DXVECTOR3 vPoint[2] = {
					D3DXVECTOR3(
						m_vWinding[i]->m_vPoint[j]->m_fPosX,
						m_vWinding[i]->m_vPoint[j]->m_fPosY,
						m_vWinding[i]->m_vPoint[j]->m_fPosZ),
					D3DXVECTOR3(
						m_vWinding[i]->m_vPoint[(j + 1) % nCount]->m_fPosX,
						m_vWinding[i]->m_vPoint[(j + 1) % nCount]->m_fPosY,
						m_vWinding[i]->m_vPoint[(j + 1) % nCount]->m_fPosZ)};

				D3DXPlaneFromPoints(
					&vPlaneW,
					&vPoint[1],
					&vPoint[0],
					&(vPoint[0] + vNormalW));

				CTMath::CutWinding(
					&vPlaneW,
					&vCOPY.m_vFace);
			}

			if(!vCOPY.m_vFace.m_vPoint.empty())
				return FALSE;
		}
	}

	return TRUE;
}

void CTBSPNode::RemoveInvalidPortal( LPARAM lParam, LPVECTORTBSPPORTAL pPortal)
{
	if(!pPortal)
		return;

	VECTORTBSPPORTAL::iterator it = pPortal->begin();
	while( it != pPortal->end() )
	{
		CTBSPPortal *pItem = (*it);

		if(pItem)
		{
			CTBSPNode *pFrontNode = GetNode(pItem->m_dwFrontID);
			CTBSPNode *pBackNode = GetNode(pItem->m_dwBackID);

			if( pFrontNode && pBackNode )
			{
				if( pBackNode->m_bValid && !pFrontNode->IsValidPortal(pItem) )
					pBackNode->m_bValid = FALSE;

				if( pFrontNode->m_bValid && !pBackNode->IsValidPortal(pItem) )
					pFrontNode->m_bValid = FALSE;
			}

			if( pFrontNode && pBackNode &&
				pFrontNode->m_bValid &&
				pBackNode->m_bValid )
			{
				pFrontNode->m_vPortal.push_back(pItem);
				pBackNode->m_vPortal.push_back(pItem);
				it++;
			}
			else
			{
				pPortal->erase(it);
				delete pItem;
			}
		}
	}
}

CTBSPNode *CTBSPNode::FindLeaf( LPD3DXVECTOR3 pPoint)
{
	if(!pPoint)
		return NULL;

	if( !m_pFrontNode && !m_pBackNode )
		return this;

	switch(CTMath::GetPointPosition( pPoint, &m_vPlane))
	{
	case POINTPOS_ONPLANE	:
	case POINTPOS_FRONT		:
		if(m_pFrontNode)
			return m_pFrontNode->FindLeaf(pPoint);

		break;

	case POINTPOS_BACK		:
		if(m_pBackNode)
			return m_pBackNode->FindLeaf(pPoint);

		break;
	}

	return this;
}

void CTBSPNode::BuildPVS( LPARAM lParam)
{
	int nCount = INT(m_vPortal.size());

	if(IsLeaf())
	{
		CString strTitle;

		strTitle.Format( "Build PVS of Node [%d]", m_dwID);
		if(m_pProcLevel)
			m_pProcLevel( lParam, 0, strTitle);

		if(m_pProcStep)
			m_pProcStep( lParam, 0);
	}

	for( int i=0; i<nCount; i++)
	{
		if( m_vPortal[i]->m_mapFrontPVS.empty() &&
			m_vPortal[i]->m_mapBackPVS.empty() )
			BuildPVS(m_vPortal[i]);

		if(m_pProcStep)
			m_pProcStep( lParam, i * 100 / nCount);
	}

	if(m_pFrontNode)
		m_pFrontNode->BuildPVS(lParam);

	if(m_pBackNode)
		m_pBackNode->BuildPVS(lParam);
}

void CTBSPNode::BuildPVS( CTBSPPortal* pItem)
{
	if( !pItem->m_mapFrontPVS.empty() ||
		!pItem->m_mapBackPVS.empty() )
		return;

	D3DXPLANE vPlane[2] = {
		pItem->m_vFace.m_vPlane,
		-pItem->m_vFace.m_vPlane};

	DWORD dwNodeID[2] = {
		pItem->m_dwFrontID,
		pItem->m_dwBackID};

	BYTE bDirection[2] = {
		PORTALDIR_FRONT,
		PORTALDIR_BACK};

	for( int i=0; i<2; i++)
	{
		CTBSPNode *pNode = GetNode(dwNodeID[i]);

		if(pNode)
		{
			LPD3DXPLANE pBound = new D3DXPLANE;
			MAPPLANE mapBound;

			(*pBound) = vPlane[i];
			mapBound.clear();

			mapBound.insert( MAPPLANE::value_type(
				pItem->m_dwID,
				pBound));

			pItem->AddPVS(
				dwNodeID[i],
				bDirection[i]);

			for( int j=0; j<pNode->m_vPortal.size(); j++)
				if(IsValidTargetPortal( &mapBound, pNode->m_vPortal[j]))
				{
					BYTE bPassDir = GetPassPortalDir(
						pNode->m_vPortal[j],
						pNode->m_dwID);
					pBound = new D3DXPLANE;

					(*pBound) = GetPassPortalPlane(
						pNode->m_vPortal[j],
						bPassDir);

					mapBound.insert( MAPPLANE::value_type(
						pNode->m_vPortal[j]->m_dwID,
						pBound));

					BuildPVS(
						&mapBound,
						&vPlane[i],
						pItem,
						pNode->m_vPortal[j],
						bPassDir,
						bDirection[i]);

					mapBound.erase(pNode->m_vPortal[j]->m_dwID);
					delete pBound;
				}

			MAPPLANE::iterator it;
			for( it = mapBound.begin(); it != mapBound.end(); it++)
				delete (*it).second;
			mapBound.clear();
		}
	}
}

void CTBSPNode::BuildPVS( LPMAPPLANE pBoundPlane,
						  LPD3DXPLANE pHostPlane,
						  CTBSPPortal *pHost,
						  CTBSPPortal *pPass,
						  BYTE bPassDir,
						  BYTE bHostDir)
{
	if( !pHost || !pPass || !pBoundPlane ||
		pHost->m_vFace.m_vPoint.empty() ||
		pPass->m_vFace.m_vPoint.empty())
		return;

	CTBSPNode *pNode = GetNode(GetPassPortalNodeID( pPass, bPassDir));
	if(!pNode)
		return;

	pHost->AddPVS(
		pNode->m_dwID,
		bHostDir);

	for( int i=0; i<pNode->m_vPortal.size(); i++)
		if(IsValidTargetPortal( pBoundPlane, pNode->m_vPortal[i]))
		{
			CTBSPPortal *pNext = CullPortal(
				pHostPlane,
				pHost,
				pPass,
				pNode->m_vPortal[i]);

			if(pNext)
			{
				bPassDir = GetPassPortalDir(
					pNext,
					pNode->m_dwID);

				D3DXPLANE vBound = GetPassPortalPlane(
					pNext,
					bPassDir);

				pBoundPlane->insert( MAPPLANE::value_type(
					pNext->m_dwID,
					&vBound));

				BuildPVS(
					pBoundPlane,
					pHostPlane,
					pHost,
					pNext,
					bPassDir,
					bHostDir);

				pBoundPlane->erase(pNext->m_dwID);
				delete pNext;
			}
		}
}

CTBSPPortal *CTBSPNode::CullPortal( LPD3DXPLANE pHostPlane,
								    CTBSPPortal *pHost,
								    CTBSPPortal *pPass,
									CTBSPPortal *pTarget)
{
	D3DXVECTOR3 vNormal(
		pHostPlane->a,
		pHostPlane->b,
		pHostPlane->c);
	vNormal /= D3DXVec3Length(&vNormal);

	CTBSPPortal *pResult = new CTBSPPortal(pTarget);
	int nCount = pHost->m_vFace.m_vPoint.size();

	for( int i=0; i<nCount; i++)
	{
		D3DXPLANE vPlane = GetPortalCutter(
			pHost->m_vFace.m_vPoint[i],
			pHost->m_vFace.m_vPoint[(i+1) % nCount],
			pHost->m_vFace.m_vPoint[(i+2) % nCount],
			&vNormal,
			pHostPlane,
			pPass);

		CTMath::CutWinding(
			&vPlane,
			&pResult->m_vFace);

		if(pResult->m_vFace.m_vPoint.empty())
		{
			delete pResult;
			return NULL;
		}
	}

	return pResult;
}

D3DXPLANE CTBSPNode::GetPortalCutter( LPD3DXVECTOR3 pEdgeBegin,
									  LPD3DXVECTOR3 pEdgeEnd,
									  LPD3DXVECTOR3 pPoint,
									  LPD3DXVECTOR3 pHostNormal,
									  LPD3DXPLANE pHostPlane,
									  CTBSPPortal *pPass)
{
	D3DXPLANE vResult = (*pHostPlane);
	D3DXPLANE vAxis;
	FLOAT fMinDot = 1.0f;

	D3DXPlaneFromPoints(
		&vAxis,
		pEdgeBegin,
		pEdgeEnd,
		&((*pEdgeEnd) + (*pHostNormal)));

	if( CTMath::GetPointPosition( pPoint, &vAxis) == POINTPOS_BACK )
		vAxis = -vAxis;

	for( int i=0; i<pPass->m_vFace.m_vPoint.size(); i++)
	{
		D3DXVECTOR3 vNormal = (*pHostNormal);
		D3DXPLANE vPlane = (*pHostPlane);

		switch(CTMath::GetPointPosition( pPass->m_vFace.m_vPoint[i], pHostPlane))
		{
		case POINTPOS_FRONT		:
			{
				D3DXPlaneFromPoints(
					&vPlane,
					pEdgeBegin,
					pEdgeEnd,
					pPass->m_vFace.m_vPoint[i]);

				if( CTMath::GetPointPosition( pPoint, &vPlane) == POINTPOS_BACK )
					vPlane = -vPlane;

				vNormal = D3DXVECTOR3(
					vPlane.a,
					vPlane.b,
					vPlane.c);
				vNormal /= D3DXVec3Length(&vNormal);
			}

			break;

		case POINTPOS_ONPLANE	:
		case POINTPOS_BACK		:
			if( CTMath::GetPointPosition( pPass->m_vFace.m_vPoint[i], &vAxis) == POINTPOS_FRONT )
			{
				vNormal = -vNormal;
				vPlane = -vPlane;
			}

			break;
		}

		FLOAT fDot = D3DXVec3Dot(
			pHostNormal,
			&vNormal);

		if( fDot < fMinDot )
		{
			vResult = vPlane;
			fMinDot = fDot;
		}
	}

	return -vResult;
}

BOOL CTBSPNode::IsValidTargetPortal( LPMAPPLANE pBoundPlane,
									 CTBSPPortal *pItem)
{
	MAPPLANE::iterator it;

	for( it = pBoundPlane->begin(); it != pBoundPlane->end(); it++)
		if(!IsValidTargetPortal( (*it).second, pItem))
			return FALSE;

	return TRUE;
}

BOOL CTBSPNode::IsValidTargetPortal( LPD3DXPLANE pPlane,
									 CTBSPPortal *pItem)
{
	if( !pPlane || !pItem ||
		pItem->m_vFace.m_vPoint.empty() )
		return FALSE;

	BYTE bDet = CTMath::GetWindingPosition(
		&pItem->m_vFace,
		pPlane);

	return bDet == WINDINGPOS_NONE || bDet == WINDINGPOS_FRONT;
}

BYTE CTBSPNode::GetPassPortalDir( CTBSPPortal *pItem,
								  DWORD dwNodeID)
{
	return pItem->m_dwFrontID == dwNodeID ? PORTALDIR_BACK : PORTALDIR_FRONT;
}

D3DXPLANE CTBSPNode::GetPassPortalPlane( CTBSPPortal *pItem,
										 BYTE bDierction)
{
	return bDierction == PORTALDIR_FRONT ? pItem->m_vFace.m_vPlane : -pItem->m_vFace.m_vPlane;
}

DWORD CTBSPNode::GetPassPortalNodeID( CTBSPPortal *pItem,
									  BYTE bDierction)
{
	return bDierction == PORTALDIR_FRONT ? pItem->m_dwFrontID : pItem->m_dwBackID;
}

int CTBSPNode::HitTest( LPD3DXVECTOR3 pLayPos,
					    LPD3DXVECTOR3 pLayDir,
						FLOAT *pDistance)
{
	int nResult = -1;
	FLOAT fDistance;

	for( int i=0; i<INT(m_vWinding.size()); i++)
	{
		D3DXVECTOR3 vResult;

		if( CTMath::GetPointPosition(
			pLayPos,
			&m_vWinding[i]->m_vPlane) == POINTPOS_FRONT &&
			CTMath::PlaneIntersectLine(
			&vResult,
			pLayPos,
			&((*pLayPos) + (*pLayDir)),
			&m_vWinding[i]->m_vPlane) &&
			D3DXVec3Dot(
			&(vResult - (*pLayPos)),
			pLayDir) >= 0.0f )
		{
			D3DXVECTOR3 vNormal(
				m_vWinding[i]->m_vPlane.a,
				m_vWinding[i]->m_vPlane.b,
				m_vWinding[i]->m_vPlane.c);
			BOOL bResult = TRUE;
			int nCount = INT(m_vWinding[i]->m_vPoint.size());

			D3DXVec3Normalize( &vNormal, &vNormal);
			for( int j=0; j<nCount; j++)
			{
				D3DXVECTOR3 vPoint[3] = {
					D3DXVECTOR3(
						m_vWinding[i]->m_vPoint[j]->m_fPosX,
						m_vWinding[i]->m_vPoint[j]->m_fPosY,
						m_vWinding[i]->m_vPoint[j]->m_fPosZ),
					D3DXVECTOR3(
						m_vWinding[i]->m_vPoint[(j + 1) % nCount]->m_fPosX,
						m_vWinding[i]->m_vPoint[(j + 1) % nCount]->m_fPosY,
						m_vWinding[i]->m_vPoint[(j + 1) % nCount]->m_fPosZ),
					D3DXVECTOR3(
						m_vWinding[i]->m_vPoint[(j + 2) % nCount]->m_fPosX,
						m_vWinding[i]->m_vPoint[(j + 2) % nCount]->m_fPosY,
						m_vWinding[i]->m_vPoint[(j + 2) % nCount]->m_fPosZ)};
				D3DXPLANE vPlane;

				D3DXPlaneFromPoints(
					&vPlane,
					&vPoint[1],
					&vPoint[0],
					&(vPoint[0] + vNormal));

				if( CTMath::GetPointPosition( &vResult, &vPlane) == POINTPOS_BACK )
				{
					bResult = FALSE;
					break;
				}
			}

			if(bResult)
			{
				fDistance = D3DXVec3Length(&(vResult - (*pLayPos)));

				if( nResult == -1 || fDistance < (*pDistance) )
				{
					nResult = m_vWinding[i]->m_nMeshIndex;
					(*pDistance) = fDistance;
				}
			}
		}
	}

	if(m_pFrontNode)
	{
		int nIndex = m_pFrontNode->HitTest(
			pLayPos,
			pLayDir,
			&fDistance);

		if( nIndex != -1 && (
			nResult == -1 ||
			fDistance < *pDistance ))
		{
			*pDistance = fDistance;
			nResult = nIndex;
		}
	}

	if(m_pBackNode)
	{
		int nIndex = m_pBackNode->HitTest(
			pLayPos,
			pLayDir,
			&fDistance);

		if( nIndex != -1 && (
			nResult == -1 ||
			fDistance < *pDistance ))
		{
			*pDistance = fDistance;
			nResult = nIndex;
		}
	}

	return nResult;
}

void CTBSPNode::LoadMAP( CFile* pFile,
						 LPMESHVERTEX pMESH,
						 LPLVERTEX pLIGHT,
						 int& nMESH,
						 int& nLIGHT)
{
	if(!pFile)
		return;

	ClearPortal();
	ClearNode();
	ClearLightMap();

	DWORD dwNodeID = NODEID_NULL;
	BYTE bFront = FALSE;
	BYTE bBack = FALSE;
	int nCount = 0;

	pFile->Read( &dwNodeID, sizeof(dwNodeID));
	pFile->Read( &bFront, sizeof(bFront));
	pFile->Read( &bBack, sizeof(bBack));
	pFile->Read( &m_vPlane, sizeof(m_vPlane));
	pFile->Read( &nCount, sizeof(nCount));
	SetID(dwNodeID);

	for( int i=0; i<nCount; i++)
	{
		LPVERTEXWINDING pWINDING = new VERTEXWINDING();
		int nPoint = 0;

		pFile->Read( &nPoint, sizeof(int));
		pWINDING->m_nCount = nPoint - 2;
		pWINDING->m_nIndex = nMESH;

		for( int j=0; j<nPoint; j++)
		{
			LPMESHVERTEX pPoint = new MESHVERTEX();

			pFile->Read( &pPoint->m_fNormalX, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fNormalY, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fNormalZ, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fPosX, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fPosY, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fPosZ, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fU1, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fV1, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fU2, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fV2, sizeof(FLOAT));

			pWINDING->m_vPoint.push_back(pPoint);
		}

		for( j=1; j<INT(pWINDING->m_vPoint.size()) - 1; j++)
		{
			memcpy( &pMESH[nMESH++], pWINDING->m_vPoint[0], sizeof(MESHVERTEX));
			memcpy( &pMESH[nMESH++], pWINDING->m_vPoint[j], sizeof(MESHVERTEX));
			memcpy( &pMESH[nMESH++], pWINDING->m_vPoint[j + 1], sizeof(MESHVERTEX));
		}

		pFile->Read( &pWINDING->m_vPlane, sizeof(D3DXPLANE));
		pFile->Read( &pWINDING->m_nMeshIndex, sizeof(int));

		m_vWinding.push_back(pWINDING);
	}

	pFile->Read( &nCount, sizeof(nCount));
	for( i=0; i<nCount; i++)
	{
		CTBSPPortal *pItem = NULL;
		DWORD dwPortalID;
		BYTE bData;

		pFile->Read( &dwPortalID, sizeof(dwPortalID));
		pFile->Read( &bData, sizeof(bData));
		pItem = CTBSPPortal::GetPortal(dwPortalID);

		if(!pItem)
		{
			pItem = new CTBSPPortal();
			pItem->SetID(dwPortalID);
		}

		if(bData)
		{
			int nPointCount = 0;
			int nFrontPVS = 0;
			int nBackPVS = 0;

			pFile->Read( &pItem->m_vFace.m_vPlane, sizeof(pItem->m_vFace.m_vPlane));
			pFile->Read( &pItem->m_bDirection, sizeof(pItem->m_bDirection));
			pFile->Read( &pItem->m_dwFrontID, sizeof(pItem->m_dwFrontID));
			pFile->Read( &pItem->m_dwBackID, sizeof(pItem->m_dwBackID));
			pFile->Read( &pItem->m_dwNodeID, sizeof(pItem->m_dwNodeID));

			pFile->Read( &nPointCount, sizeof(nPointCount));
			pFile->Read( &nFrontPVS, sizeof(nFrontPVS));
			pFile->Read( &nBackPVS, sizeof(nBackPVS));

			for( int j=0; j<nPointCount; j++)
			{
				LPD3DXVECTOR3 pPoint = new D3DXVECTOR3;

				pFile->Read( pPoint, sizeof(D3DXVECTOR3));
				pItem->m_vFace.m_vPoint.push_back(pPoint);
			}

			for( j=0; j<nFrontPVS; j++)
			{
				DWORD dwPVS = NODEID_NULL;

				pFile->Read( &dwPVS, sizeof(DWORD));
				pItem->AddPVS( dwPVS, PORTALDIR_FRONT);
			}

			for( j=0; j<nBackPVS; j++)
			{
				DWORD dwPVS = NODEID_NULL;

				pFile->Read( &dwPVS, sizeof(DWORD));
				pItem->AddPVS( dwPVS, PORTALDIR_BACK);
			}
		}

		m_vPortal.push_back(pItem);
	}

	pFile->Read( &nCount, sizeof(nCount));
	for( i=0; i<nCount; i++)
	{
		LPTEXWINDING pWINDING = new TEXWINDING();

		DWORD dwLightID;
		int nWINDING;

		pFile->Read( &dwLightID, sizeof(DWORD));
		pFile->Read( &pWINDING->m_dwColor, sizeof(DWORD));
		pWINDING->m_pTEX = NULL;

		pFile->Read( &nWINDING, sizeof(int));
		for( int j=0; j<nWINDING; j++)
		{
			LPVERTEXWINDING pWinding = new VERTEXWINDING();
			int nPoint;

			pFile->Read( &pWinding->m_vPlane, sizeof(D3DXPLANE));
			pFile->Read( &pWinding->m_nMeshIndex, sizeof(int));
			pFile->Read( &nPoint, sizeof(int));

			pWinding->m_nCount = nPoint - 2;
			pWinding->m_nIndex = nLIGHT;

			for( int k=0; k<nPoint; k++)
			{
				LPMESHVERTEX pPoint = new MESHVERTEX();

				pFile->Read( &pPoint->m_fNormalX, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fNormalY, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fNormalZ, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fPosX, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fPosY, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fPosZ, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fU1, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fV1, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fU2, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fV2, sizeof(FLOAT));

				pWinding->m_vPoint.push_back(pPoint);
			}

			for( k=1; k<INT(pWinding->m_vPoint.size()) - 1; k++)
			{
				pLIGHT[nLIGHT].m_dwColor = pWINDING->m_dwColor;
				pLIGHT[nLIGHT].m_fPosX = pWinding->m_vPoint[0]->m_fPosX;
				pLIGHT[nLIGHT].m_fPosY = pWinding->m_vPoint[0]->m_fPosY;
				pLIGHT[nLIGHT].m_fPosZ = pWinding->m_vPoint[0]->m_fPosZ;
				pLIGHT[nLIGHT].m_fU = pWinding->m_vPoint[0]->m_fU1;
				pLIGHT[nLIGHT].m_fV = pWinding->m_vPoint[0]->m_fV1;
				nLIGHT++;

				pLIGHT[nLIGHT].m_dwColor = pWINDING->m_dwColor;
				pLIGHT[nLIGHT].m_fPosX = pWinding->m_vPoint[k]->m_fPosX;
				pLIGHT[nLIGHT].m_fPosY = pWinding->m_vPoint[k]->m_fPosY;
				pLIGHT[nLIGHT].m_fPosZ = pWinding->m_vPoint[k]->m_fPosZ;
				pLIGHT[nLIGHT].m_fU = pWinding->m_vPoint[k]->m_fU1;
				pLIGHT[nLIGHT].m_fV = pWinding->m_vPoint[k]->m_fV1;
				nLIGHT++;

				pLIGHT[nLIGHT].m_dwColor = pWINDING->m_dwColor;
				pLIGHT[nLIGHT].m_fPosX = pWinding->m_vPoint[k + 1]->m_fPosX;
				pLIGHT[nLIGHT].m_fPosY = pWinding->m_vPoint[k + 1]->m_fPosY;
				pLIGHT[nLIGHT].m_fPosZ = pWinding->m_vPoint[k + 1]->m_fPosZ;
				pLIGHT[nLIGHT].m_fU = pWinding->m_vPoint[k + 1]->m_fU1;
				pLIGHT[nLIGHT].m_fV = pWinding->m_vPoint[k + 1]->m_fV1;
				nLIGHT++;
			}

			pWINDING->m_vWINDING.push_back(pWinding);
		}

		m_mapLIGHT.insert( MAPTEXWINDING::value_type( dwLightID, pWINDING));
	}

	if(bFront)
	{
		if(m_pFrontNode)
			delete m_pFrontNode;

		m_pFrontNode = new CTBSPNode();
		m_pFrontNode->LoadMAP(
			pFile,
			pMESH,
			pLIGHT,
			nMESH,
			nLIGHT);
	}

	if(bBack)
	{
		if(m_pBackNode)
			delete m_pBackNode;

		m_pBackNode = new CTBSPNode();
		m_pBackNode->LoadMAP(
			pFile,
			pMESH,
			pLIGHT,
			nMESH,
			nLIGHT);
	}
}

void CTBSPNode::Load( CFile* pFile)
{
	if(!pFile)
		return;

	ClearPortal();
	ClearNode();
	ClearLightMap();

	DWORD dwNodeID = NODEID_NULL;
	BYTE bFront = FALSE;
	BYTE bBack = FALSE;
	int nCount = 0;

	pFile->Read( &dwNodeID, sizeof(dwNodeID));
	pFile->Read( &bFront, sizeof(bFront));
	pFile->Read( &bBack, sizeof(bBack));
	pFile->Read( &m_vPlane, sizeof(m_vPlane));
	pFile->Read( &nCount, sizeof(nCount));
	SetID(dwNodeID);

	for( int i=0; i<nCount; i++)
	{
		LPVERTEXWINDING pWINDING = new VERTEXWINDING();
		int nPoint = 0;

		pFile->Read( &nPoint, sizeof(int));
		for( int j=0; j<nPoint; j++)
		{
			LPMESHVERTEX pPoint = new MESHVERTEX();

			pFile->Read( &pPoint->m_fNormalX, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fNormalY, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fNormalZ, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fPosX, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fPosY, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fPosZ, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fU1, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fV1, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fU2, sizeof(FLOAT));
			pFile->Read( &pPoint->m_fV2, sizeof(FLOAT));

			pWINDING->m_vPoint.push_back(pPoint);
		}

		pFile->Read( &pWINDING->m_vPlane, sizeof(D3DXPLANE));
		pFile->Read( &pWINDING->m_nMeshIndex, sizeof(int));

		m_vWinding.push_back(pWINDING);
	}

	pFile->Read( &nCount, sizeof(nCount));
	for( i=0; i<nCount; i++)
	{
		CTBSPPortal *pItem = NULL;
		DWORD dwPortalID;
		BYTE bData;

		pFile->Read( &dwPortalID, sizeof(dwPortalID));
		pFile->Read( &bData, sizeof(bData));
		pItem = CTBSPPortal::GetPortal(dwPortalID);

		if(!pItem)
		{
			pItem = new CTBSPPortal();
			pItem->SetID(dwPortalID);
		}

		if(bData)
		{
			int nPointCount = 0;
			int nFrontPVS = 0;
			int nBackPVS = 0;

			pFile->Read( &pItem->m_vFace.m_vPlane, sizeof(pItem->m_vFace.m_vPlane));
			pFile->Read( &pItem->m_bDirection, sizeof(pItem->m_bDirection));
			pFile->Read( &pItem->m_dwFrontID, sizeof(pItem->m_dwFrontID));
			pFile->Read( &pItem->m_dwBackID, sizeof(pItem->m_dwBackID));
			pFile->Read( &pItem->m_dwNodeID, sizeof(pItem->m_dwNodeID));

			pFile->Read( &nPointCount, sizeof(nPointCount));
			pFile->Read( &nFrontPVS, sizeof(nFrontPVS));
			pFile->Read( &nBackPVS, sizeof(nBackPVS));

			for( int j=0; j<nPointCount; j++)
			{
				LPD3DXVECTOR3 pPoint = new D3DXVECTOR3;

				pFile->Read( pPoint, sizeof(D3DXVECTOR3));
				pItem->m_vFace.m_vPoint.push_back(pPoint);
			}

			for( j=0; j<nFrontPVS; j++)
			{
				DWORD dwPVS = NODEID_NULL;

				pFile->Read( &dwPVS, sizeof(DWORD));
				pItem->AddPVS( dwPVS, PORTALDIR_FRONT);
			}

			for( j=0; j<nBackPVS; j++)
			{
				DWORD dwPVS = NODEID_NULL;

				pFile->Read( &dwPVS, sizeof(DWORD));
				pItem->AddPVS( dwPVS, PORTALDIR_BACK);
			}
		}

		m_vPortal.push_back(pItem);
	}

	pFile->Read( &nCount, sizeof(nCount));
	for( i=0; i<nCount; i++)
	{
		LPTEXWINDING pWINDING = new TEXWINDING();

		DWORD dwLightID;
		int nWINDING;

		pFile->Read( &dwLightID, sizeof(DWORD));
		pFile->Read( &pWINDING->m_dwColor, sizeof(DWORD));
		pWINDING->m_pTEX = NULL;

		pFile->Read( &nWINDING, sizeof(int));
		for( int j=0; j<nWINDING; j++)
		{
			LPVERTEXWINDING pWinding = new VERTEXWINDING();
			int nPoint;

			pFile->Read( &pWinding->m_vPlane, sizeof(D3DXPLANE));
			pFile->Read( &pWinding->m_nMeshIndex, sizeof(int));
			pFile->Read( &nPoint, sizeof(int));

			for( int k=0; k<nPoint; k++)
			{
				LPMESHVERTEX pPoint = new MESHVERTEX();

				pFile->Read( &pPoint->m_fNormalX, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fNormalY, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fNormalZ, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fPosX, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fPosY, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fPosZ, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fU1, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fV1, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fU2, sizeof(FLOAT));
				pFile->Read( &pPoint->m_fV2, sizeof(FLOAT));

				pWinding->m_vPoint.push_back(pPoint);
			}

			pWINDING->m_vWINDING.push_back(pWinding);
		}

		m_mapLIGHT.insert( MAPTEXWINDING::value_type( dwLightID, pWINDING));
	}

	if(bFront)
	{
		if(m_pFrontNode)
			delete m_pFrontNode;

		m_pFrontNode = new CTBSPNode();
		m_pFrontNode->Load(pFile);
	}

	if(bBack)
	{
		if(m_pBackNode)
			delete m_pBackNode;

		m_pBackNode = new CTBSPNode();
		m_pBackNode->Load(pFile);
	}
}

void CTBSPNode::Save( CFile* pFile)
{
	if(!pFile)
		return;

	BYTE bFront = m_pFrontNode ? TRUE : FALSE;
	BYTE bBack = m_pBackNode ? TRUE : FALSE;
	int nCount = INT(m_vWinding.size());

	pFile->Write( &m_dwID, sizeof(m_dwID));
	pFile->Write( &bFront, sizeof(bFront));
	pFile->Write( &bBack, sizeof(bBack));
	pFile->Write( &m_vPlane, sizeof(m_vPlane));
	pFile->Write( &nCount, sizeof(nCount));

	for( int i=0; i<nCount; i++)
	{
		int nPoint = INT(m_vWinding[i]->m_vPoint.size());

		pFile->Write( &nPoint, sizeof(int));
		for( int j=0; j<nPoint; j++)
		{
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fNormalX, sizeof(FLOAT));
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fNormalY, sizeof(FLOAT));
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fNormalZ, sizeof(FLOAT));
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fPosX, sizeof(FLOAT));
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fPosY, sizeof(FLOAT));
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fPosZ, sizeof(FLOAT));
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fU1, sizeof(FLOAT));
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fV1, sizeof(FLOAT));
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fU2, sizeof(FLOAT));
			pFile->Write( &m_vWinding[i]->m_vPoint[j]->m_fV2, sizeof(FLOAT));
		}

		pFile->Write( &m_vWinding[i]->m_vPlane, sizeof(D3DXPLANE));
		pFile->Write( &m_vWinding[i]->m_nMeshIndex, sizeof(int));
	}

	nCount = INT(m_vPortal.size());
	pFile->Write( &nCount, sizeof(nCount));

	for( i=0; i<nCount; i++)
	{
		BYTE bData = m_vPortal[i]->m_dwFrontID == m_dwID ? TRUE : FALSE;

		pFile->Write( &m_vPortal[i]->m_dwID, sizeof(m_vPortal[i]->m_dwID));
		pFile->Write( &bData, sizeof(bData));

		if(bData)
		{
			int nPointCount = INT(m_vPortal[i]->m_vFace.m_vPoint.size());
			int nFrontPVS = INT(m_vPortal[i]->m_mapFrontPVS.size());
			int nBackPVS = INT(m_vPortal[i]->m_mapBackPVS.size());

			MAPDWORD::iterator it;

			pFile->Write( &m_vPortal[i]->m_vFace.m_vPlane, sizeof(m_vPortal[i]->m_vFace.m_vPlane));
			pFile->Write( &m_vPortal[i]->m_bDirection, sizeof(m_vPortal[i]->m_bDirection));
			pFile->Write( &m_vPortal[i]->m_dwFrontID, sizeof(m_vPortal[i]->m_dwFrontID));
			pFile->Write( &m_vPortal[i]->m_dwBackID, sizeof(m_vPortal[i]->m_dwBackID));
			pFile->Write( &m_vPortal[i]->m_dwNodeID, sizeof(m_vPortal[i]->m_dwNodeID));

			pFile->Write( &nPointCount, sizeof(nPointCount));
			pFile->Write( &nFrontPVS, sizeof(nFrontPVS));
			pFile->Write( &nBackPVS, sizeof(nBackPVS));

			for( int j=0; j<nPointCount; j++)
				pFile->Write( m_vPortal[i]->m_vFace.m_vPoint[j], sizeof(D3DXVECTOR3));

			for( it = m_vPortal[i]->m_mapFrontPVS.begin(); it != m_vPortal[i]->m_mapFrontPVS.end(); it++)
				pFile->Write( &(*it).second, sizeof(DWORD));

			for( it = m_vPortal[i]->m_mapBackPVS.begin(); it != m_vPortal[i]->m_mapBackPVS.end(); it++)
				pFile->Write( &(*it).second, sizeof(DWORD));
		}
	}

	nCount = INT(m_mapLIGHT.size());
	pFile->Write( &nCount, sizeof(nCount));
	MAPTEXWINDING::iterator it;

	for( it = m_mapLIGHT.begin(); it != m_mapLIGHT.end(); it++)
	{
		int nWINDING = INT((*it).second->m_vWINDING.size());

		pFile->Write( &(*it).first, sizeof(DWORD));
		pFile->Write( &(*it).second->m_dwColor, sizeof(DWORD));

		pFile->Write( &nWINDING, sizeof(int));
		for( i=0; i<nWINDING; i++)
		{
			int nPoint = INT((*it).second->m_vWINDING[i]->m_vPoint.size());

			pFile->Write( &(*it).second->m_vWINDING[i]->m_vPlane, sizeof(D3DXPLANE));
			pFile->Write( &(*it).second->m_vWINDING[i]->m_nMeshIndex, sizeof(int));
			pFile->Write( &nPoint, sizeof(int));

			for( int j=0; j<nPoint; j++)
			{
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fNormalX, sizeof(FLOAT));
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fNormalY, sizeof(FLOAT));
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fNormalZ, sizeof(FLOAT));
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fPosX, sizeof(FLOAT));
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fPosY, sizeof(FLOAT));
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fPosZ, sizeof(FLOAT));
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fU1, sizeof(FLOAT));
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fV1, sizeof(FLOAT));
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fU2, sizeof(FLOAT));
				pFile->Write( &(*it).second->m_vWINDING[i]->m_vPoint[j]->m_fV2, sizeof(FLOAT));
			}
		}
	}

	if(m_pFrontNode)
		m_pFrontNode->Save(pFile);

	if(m_pBackNode)
		m_pBackNode->Save(pFile);
}

void CTBSPNode::RegulateLight( LPMAPTEXTURESET pTEX)
{
	MAPTEXWINDING::iterator it;

	if(m_pFrontNode)
		m_pFrontNode->RegulateLight(pTEX);

	if(m_pBackNode)
		m_pBackNode->RegulateLight(pTEX);

	for( it = m_mapLIGHT.begin(); it != m_mapLIGHT.end(); it++)
	{
		MAPTEXTURESET::iterator finder = pTEX->find((*it).first);
		(*it).second->m_pTEX = (*finder).second;
	}
}

void CTBSPNode::DeleteLight( DWORD dwID)
{
	MAPTEXWINDING::iterator finder = m_mapLIGHT.find(dwID);

	if(m_pFrontNode)
		m_pFrontNode->DeleteLight(dwID);

	if(m_pBackNode)
		m_pBackNode->DeleteLight(dwID);

	if( finder != m_mapLIGHT.end() )
	{
		delete (*finder).second;
		m_mapLIGHT.erase(finder);
	}
}

void CTBSPNode::AddLight( LPTEXTURESET pTEX,
						  LPMAPLIGHT pLIGHT,
						  DWORD dwID)
{
	MAPTEXWINDING::iterator finder = m_mapLIGHT.find(dwID);

	if(m_pFrontNode)
		m_pFrontNode->AddLight( pTEX, pLIGHT, dwID);

	if(m_pBackNode)
		m_pBackNode->AddLight( pTEX, pLIGHT, dwID);

	if( finder != m_mapLIGHT.end() )
	{
		delete (*finder).second;
		m_mapLIGHT.erase(finder);
	}

	if(IsLeaf())
	{
		LPTEXWINDING pWINDING = new TEXWINDING();

		pWINDING->m_dwColor = pLIGHT->m_dwAmbient;
		pWINDING->m_pTEX = pTEX;
		pWINDING->m_vWINDING.clear();

		for( int i=0; i<INT(m_vWinding.size()); i++)
		{
			FLOAT fDist = (FLOAT) fabs(D3DXPlaneDotCoord(
				&m_vWinding[i]->m_vPlane,
				&pLIGHT->m_vPOS));

			if( fDist < pLIGHT->m_fRange )
			{
				FLOAT fWidth = pLIGHT->m_fRange * sinf(acosf(fDist / pLIGHT->m_fRange));
				FLOAT fDistX = m_vWinding[i]->m_vPlane.a * m_vWinding[i]->m_vPlane.a;
				FLOAT fDistY = m_vWinding[i]->m_vPlane.b * m_vWinding[i]->m_vPlane.b;
				FLOAT fDistZ = m_vWinding[i]->m_vPlane.c * m_vWinding[i]->m_vPlane.c;

				LPVERTEXWINDING pWinding = CTMath::CopyWinding(m_vWinding[i]);
				D3DXVECTOR3 vNormal;
				D3DXVECTOR3 vResult;
				D3DXMATRIX vROT;

				if( fDistY > fDistX &&
					fDistY > fDistZ )
				{
					D3DXMatrixRotationZ(
						&vROT,
						D3DX_PI / 2.0f);
				}
				else
				{
					D3DXMatrixRotationY(
						&vROT,
						D3DX_PI / 2.0f);
				}

				vResult = D3DXVECTOR3(
					m_vWinding[i]->m_vPlane.a,
					m_vWinding[i]->m_vPlane.b,
					m_vWinding[i]->m_vPlane.c);

				CTMath::Transform(
					&vROT,
					&vResult);

				D3DXVec3Cross(
					&vNormal,
					&D3DXVECTOR3(
					m_vWinding[i]->m_vPlane.a,
					m_vWinding[i]->m_vPlane.b,
					m_vWinding[i]->m_vPlane.c),
					&vResult);
				vNormal /= D3DXVec3Length(&vNormal);

				D3DXMatrixRotationAxis(
					&vROT,
					&D3DXVECTOR3(
					m_vWinding[i]->m_vPlane.a,
					m_vWinding[i]->m_vPlane.b,
					m_vWinding[i]->m_vPlane.c),
					pLIGHT->m_fROT);

				CTMath::Transform(
					&vROT,
					&vNormal);

				D3DXMatrixRotationAxis(
					&vROT,
					&D3DXVECTOR3(
					m_vWinding[i]->m_vPlane.a,
					m_vWinding[i]->m_vPlane.b,
					m_vWinding[i]->m_vPlane.c),
					D3DX_PI / 2.0f);

				D3DXVECTOR3 vAxisV = vNormal;
				D3DXVECTOR3 vAxisU = vNormal;
				D3DXPLANE vPlaneU;
				D3DXPLANE vPlaneV;

				CTMath::Transform(
					&vROT,
					&vAxisU);

				D3DXPlaneFromPointNormal(
					&vPlaneU,
					&(pLIGHT->m_vPOS + fWidth * vAxisU),
					&vAxisU);

				D3DXPlaneFromPointNormal(
					&vPlaneV,
					&(pLIGHT->m_vPOS + fWidth * vAxisV),
					&vAxisV);

				for( int j=0; j<INT(pWinding->m_vPoint.size()); j++)
				{
					pWinding->m_vPoint[j]->m_fU1 = -D3DXPlaneDotCoord(
						&vPlaneU,
						&D3DXVECTOR3(
						pWinding->m_vPoint[j]->m_fPosX,
						pWinding->m_vPoint[j]->m_fPosY,
						pWinding->m_vPoint[j]->m_fPosZ)) / (2.0f * fWidth);

					pWinding->m_vPoint[j]->m_fV1 = -D3DXPlaneDotCoord(
						&vPlaneV,
						&D3DXVECTOR3(
						pWinding->m_vPoint[j]->m_fPosX,
						pWinding->m_vPoint[j]->m_fPosY,
						pWinding->m_vPoint[j]->m_fPosZ)) / (2.0f * fWidth);

					pWinding->m_vPoint[j]->m_fU2 = pWinding->m_vPoint[j]->m_fU1;
					pWinding->m_vPoint[j]->m_fV2 = pWinding->m_vPoint[j]->m_fV1;
				}

				for( j=0; j<4; j++)
				{
					D3DXPLANE vPlane;

					D3DXPlaneFromPointNormal(
						&vPlane,
						&(pLIGHT->m_vPOS + fWidth * vNormal),
						&vNormal);
					vPlane = -vPlane;

					CTMath::CutWinding(
						&vPlane,
						pWinding);

					CTMath::Transform(
						&vROT,
						&vNormal);
				}

				if(!pWinding->m_vPoint.empty())
					pWINDING->m_vWINDING.push_back(pWinding);
				else
					delete pWinding;
			}
		}

		if(!pWINDING->m_vWINDING.empty())
			m_mapLIGHT.insert( MAPTEXWINDING::value_type( dwID, pWINDING));
		else
			delete pWINDING;
	}
}
