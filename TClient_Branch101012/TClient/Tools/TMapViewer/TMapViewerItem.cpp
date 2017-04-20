#include "StdAfx.h"


MAPTITEMTEMP CTMapViewerItem::m_mapTITEMTEMP;

CTMapViewerItem::CTMapViewerItem()
{
	m_pTITEM = NULL;

	m_bItemID = 0;
	m_wItemID = 0;
}

CTMapViewerItem::~CTMapViewerItem()
{
}

LPTITEM CTMapViewerItem::FindTItem( WORD wItemID)
{
	MAPTITEMTEMP::iterator finder = m_mapTITEMTEMP.find(wItemID);

	if( finder != m_mapTITEMTEMP.end() )
		return (*finder).second;

	return NULL;
}
