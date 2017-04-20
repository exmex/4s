#include "StdAfx.h"


CTMapViewerInven::CTMapViewerInven()
{
	m_mapTITEM.clear();
}

CTMapViewerInven::~CTMapViewerInven()
{
	ClearInven();
}

void CTMapViewerInven::ClearInven()
{
	MAPTITEM::iterator itTITEM;

	for( itTITEM = m_mapTITEM.begin(); itTITEM != m_mapTITEM.end(); itTITEM++)
		delete (*itTITEM).second;

	m_mapTITEM.clear();
}

CTMapViewerItem *CTMapViewerInven::FindTItem( BYTE bItemID)
{
	MAPTITEM::iterator finder = m_mapTITEM.find(bItemID);

	if( finder != m_mapTITEM.end() )
		return (*finder).second;

	return NULL;
}
