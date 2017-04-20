#include "StdAfx.h"
#include ".\tchatinven.h"


CTChatInven::CTChatInven()
{
	m_mapTITEM.clear();
	m_pTITEM = NULL;

	m_bInvenID = 0;
	m_wItemID = 0;
}

CTChatInven::~CTChatInven()
{
	ClearInven();
}

void CTChatInven::ClearInven()
{
	MAPTITEM::iterator itTITEM;

	for( itTITEM = m_mapTITEM.begin(); itTITEM != m_mapTITEM.end(); itTITEM++)
		delete (*itTITEM).second;

	m_mapTITEM.clear();
}

CTChatItem *CTChatInven::FindTItem( BYTE bItemID)
{
	MAPTITEM::iterator finder = m_mapTITEM.find(bItemID);

	if( finder != m_mapTITEM.end() )
		return (*finder).second;

	return NULL;
}
