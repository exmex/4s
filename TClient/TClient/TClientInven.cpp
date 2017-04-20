#include "StdAfx.h"


CTClientInven::CTClientInven()
{
	m_mapTITEM.clear();
	m_pTITEM = NULL;

	m_bInvenID = 0;
	m_wItemID = 0;
	m_dEndTime = 0;
}

CTClientInven::~CTClientInven()
{
	ClearInven();
}

void CTClientInven::ClearInven()
{
	MAPTITEM::iterator itTITEM;

	for( itTITEM = m_mapTITEM.begin(); itTITEM != m_mapTITEM.end(); itTITEM++)
		delete (*itTITEM).second;

	m_mapTITEM.clear();
}

CTClientItem *CTClientInven::FindTItem( BYTE bItemID)
{
	MAPTITEM::iterator finder = m_mapTITEM.find(bItemID);

	if( finder != m_mapTITEM.end() )
		return (*finder).second;

	return NULL;
}

BOOL CTClientInven::CheckTItem( WORD wItemID, BYTE* pbItemID)
{
	MAPTITEM::iterator itr,end;
	itr = m_mapTITEM.begin();
	end = m_mapTITEM.end();

	for(;itr!=end; ++itr)
	{
		if( itr->second->GetTITEM()->m_wItemID == wItemID )
		{
			if( pbItemID )
				*pbItemID = itr->second->GetItemSlot();
			return TRUE;
		}
	}

	return FALSE;
}

BYTE CTClientInven::GetEasePos( CTClientItem *pTItem)
{
	MAPTITEM::iterator itTITEM;

	if(!m_pTITEM)
		return INVALID_SLOT;

	for( itTITEM = m_mapTITEM.begin(); itTITEM != m_mapTITEM.end(); itTITEM++)
	{
		if( (*(*itTITEM).second) == (*pTItem) &&
			(*itTITEM).second->GetCount() < (*itTITEM).second->GetTITEM()->m_bStack )
		{
			return (*itTITEM).second->GetItemSlot();
		}
	}

	return INVALID_SLOT;
}

BYTE CTClientInven::GetBlankPos()
{
	if(!m_pTITEM)
		return INVALID_SLOT;

	for( BYTE i=0; i<m_pTITEM->m_bSlotCount; ++i)
		if( m_mapTITEM.find(i) == m_mapTITEM.end() )
			return i;

	return INVALID_SLOT;
}

BYTE CTClientInven::GetItemPos(TITEM_TYPE eType, TITEM_KIND eKind)
{
	if(!m_pTITEM)
		return INVALID_SLOT;

	for( BYTE i=0; i<m_pTITEM->m_bSlotCount; ++i)
	{
		MAPTITEM::iterator itr = m_mapTITEM.find(i);
		if( itr == m_mapTITEM.end() )
			continue;

		CTClientItem* pTItem = itr->second;
		if( pTItem->GetTITEM()->m_bType == eType && pTItem->GetTITEM()->m_bKind == eKind )
			return i;
	}

	return INVALID_SLOT;
}

CTClientInven* CTClientInven::Clone()
{
	CTClientInven* pNewInven = new CTClientInven;
	pNewInven->m_pTITEM = m_pTITEM;
	pNewInven->m_bInvenID = m_bInvenID;
	pNewInven->m_wItemID = m_wItemID;
	pNewInven->m_dEndTime = m_dEndTime;

	MAPTITEM::iterator it = m_mapTITEM.begin();
	MAPTITEM::iterator end = m_mapTITEM.end();
	for(; it != end ; ++it)
	{
		CTClientItem* pNewItem = new CTClientItem;
		*pNewItem = *(it->second);
		pNewInven->m_mapTITEM.insert( MAPTITEM::value_type( it->first, pNewItem ) );
	}

	return pNewInven;
}











