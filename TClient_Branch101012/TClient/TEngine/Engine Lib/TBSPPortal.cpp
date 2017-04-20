#include "StdAfx.h"


MAPTBSPPORTAL CTBSPPortal::m_mapTBSPPortal;

BYTE CTBSPPortal::m_bBasePortalID = 0xFF;
DWORD CTBSPPortal::m_dwPortalID = 0;


CTBSPPortal::CTBSPPortal( CTBSPPortal *pItem)
{
	m_bDirection = pItem->m_bDirection;
	m_dwFrontID = pItem->m_dwFrontID;
	m_dwBackID = pItem->m_dwBackID;
	m_dwNodeID = pItem->m_dwNodeID;

	m_vFace.m_vPlane = pItem->m_vFace.m_vPlane;
	m_vFace.m_vPoint.clear();

	for( int i=0; i<pItem->m_vFace.m_vPoint.size(); i++)
		m_vFace.m_vPoint.push_back(new D3DXVECTOR3(*pItem->m_vFace.m_vPoint[i]));

	m_dwID = NODEID_NULL;
	GetNewID(this);
}

CTBSPPortal::CTBSPPortal()
{
	m_dwFrontID = NODEID_NULL;
	m_dwBackID = NODEID_NULL;
	m_dwNodeID = NODEID_NULL;

	m_bDirection = PORTALDIR_FRONT;
	m_vFace.m_vPoint.clear();
	m_mapFrontPVS.clear();
	m_mapBackPVS.clear();

	GetNewID(this);
}

CTBSPPortal::~CTBSPPortal()
{
	while(!m_vFace.m_vPoint.empty())
	{
		delete m_vFace.m_vPoint.back();
		m_vFace.m_vPoint.pop_back();
	}

	m_mapFrontPVS.clear();
	m_mapBackPVS.clear();

	ReleaseID(m_dwID);
}

void CTBSPPortal::ReleaseCommon()
{
	MAPTBSPPORTAL::iterator it = m_mapTBSPPortal.begin();

	while(it != m_mapTBSPPortal.end())
	{
		CTBSPPortal *pItem = (*it).second;
		it++;

		if(pItem)
			delete pItem;
	}

	m_mapTBSPPortal.clear();
	m_dwPortalID = 0;
}

void CTBSPPortal::InitCommon()
{
	m_mapTBSPPortal.clear();
	m_dwPortalID = 0;
}

CTBSPPortal *CTBSPPortal::GetPortal( DWORD dwID)
{
	MAPTBSPPORTAL::iterator finder = m_mapTBSPPortal.find(dwID);

	if( finder != m_mapTBSPPortal.end() )
		return (*finder).second;

	return NULL;
}

void CTBSPPortal::ReleaseID( DWORD dwID)
{
	MAPTBSPPORTAL::iterator finder = m_mapTBSPPortal.find(dwID);

	if( finder != m_mapTBSPPortal.end() )
		m_mapTBSPPortal.erase(finder);
}

void CTBSPPortal::GetNewID( CTBSPPortal *pItem)
{
	if( !pItem || m_bBasePortalID == 0xFF )
		return;

	DWORD dwPortalID = ((((DWORD) m_bBasePortalID) << 24) & 0xFF000000) | (m_dwPortalID & 0x00FFFFFF);
	while(GetPortal(dwPortalID))
	{
		m_dwPortalID++;
		if( m_dwPortalID > 0x00FFFFFF )
			m_dwPortalID = 0;

		dwPortalID = ((((DWORD) m_bBasePortalID) << 24) & 0xFF000000) | (m_dwPortalID & 0x00FFFFFF);
	}
	pItem->m_dwID = dwPortalID;

	m_mapTBSPPortal.insert( MAPTBSPPORTAL::value_type(
		pItem->m_dwID,
		pItem));
}

BOOL CTBSPPortal::SetID( DWORD dwID)
{
	ReleaseID(m_dwID);

	if(GetPortal(dwID))
	{
		SetID(m_dwID);
		return FALSE;
	}

	if( dwID != NODEID_NULL )
	{
		m_mapTBSPPortal.insert( MAPTBSPPORTAL::value_type(
			dwID,
			this));
	}
	m_dwID = dwID;

	return TRUE;
}

BOOL CTBSPPortal::AddPVS( DWORD dwNodeID, BYTE bDirection)
{
	BOOL bResult = FALSE;

	switch(bDirection)
	{
	case PORTALDIR_FRONT	:
		{
			MAPDWORD::iterator finder = m_mapFrontPVS.find(dwNodeID);

			if( finder == m_mapFrontPVS.end() )
			{
				m_mapFrontPVS.insert( MAPDWORD::value_type( dwNodeID, dwNodeID));
				bResult = TRUE;
			}
		}

		break;

	case PORTALDIR_BACK		:
		{
			MAPDWORD::iterator finder = m_mapBackPVS.find(dwNodeID);

			if( finder == m_mapBackPVS.end() )
			{
				m_mapBackPVS.insert( MAPDWORD::value_type( dwNodeID, dwNodeID));
				bResult = TRUE;
			}
		}

		break;
	}

	return bResult;
}
