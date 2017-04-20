#include "StdAfx.h"


CTExplorerInven::CTExplorerInven()
{
	m_mapTITEM.clear();
}

CTExplorerInven::~CTExplorerInven()
{
	ClearInven();
}

void CTExplorerInven::ClearInven()
{
	MAPTITEM::iterator itTITEM;

	for( itTITEM = m_mapTITEM.begin(); itTITEM != m_mapTITEM.end(); itTITEM++)
		delete (*itTITEM).second;

	m_mapTITEM.clear();
}

CTExplorerItem *CTExplorerInven::FindTItem( BYTE bItemID)
{
	MAPTITEM::iterator finder = m_mapTITEM.find(bItemID);

	if( finder != m_mapTITEM.end() )
		return (*finder).second;

	return NULL;
}
