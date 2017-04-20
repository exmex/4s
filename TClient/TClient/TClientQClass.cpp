#include "StdAfx.h"

CTClientQClass::CTClientQClass()
{
	m_mapTQUEST.clear();
	m_pTQCLASS = NULL;

	m_bExpend = TRUE;
}

CTClientQClass::~CTClientQClass()
{
	MAPTQUEST::iterator it;

	for( it = m_mapTQUEST.begin(); it != m_mapTQUEST.end(); it++)
		delete (*it).second;

	m_mapTQUEST.clear();
}

CTClientQuest *CTClientQClass::FindTQuest( DWORD dwQuestID)
{
	MAPTQUEST::iterator finder = m_mapTQUEST.find(dwQuestID);

	if( finder != m_mapTQUEST.end() )
		return (*finder).second;

	return NULL;
}
