#include "StdAfx.h"


CTChannel::CTChannel()
{
	m_mapTINSTANCEMAPID.clear();
	m_mapTMAP.clear();
	m_bChannel = 0;
}

CTChannel::~CTChannel()
{
	MAPTMAP::iterator itMAP;

	for( itMAP = m_mapTMAP.begin(); itMAP != m_mapTMAP.end(); itMAP++)
		delete (*itMAP).second;

	m_mapTINSTANCEMAPID.clear();
	m_mapTMAP.clear();
}

void CTChannel::InitChannel( CSqlDatabase *pDB,
							 BYTE bServerID)
{
	MAPTMAP::iterator itMAP;

	for( itMAP = m_mapTMAP.begin(); itMAP != m_mapTMAP.end(); itMAP++)
		(*itMAP).second->InitMap( pDB, bServerID);
}

void CTChannel::AddMonSpawn( LPTMONSPAWN pSPAWN,
							 BYTE bCountry)
{
	MAPTMAP::iterator finder = m_mapTMAP.find(pSPAWN->m_wMapID);

	if( finder != m_mapTMAP.end() )
		(*finder).second->AddMonSpawn( pSPAWN, m_bChannel, bCountry);
}

void CTChannel::DelAllMonAtSpawn( LPTMONSPAWN pSPAWN)
{
	MAPTMAP::iterator finder = m_mapTMAP.find(pSPAWN->m_wMapID);

	if( finder != m_mapTMAP.end() )
		(*finder).second->DelAllMonAtSpawn(pSPAWN);
}

void CTChannel::DelMonSpawn( LPTMONSPAWN pSPAWN)
{
	MAPTMAP::iterator finder = m_mapTMAP.find(pSPAWN->m_wMapID);

	if( finder != m_mapTMAP.end() )
		(*finder).second->DelMonSpawn(pSPAWN);
}

WORD CTChannel::GetInstanceMapID( WORD wPartyID,
								  WORD wMapID)
{
	MAPDWORD::iterator finder = m_mapTINSTANCEMAPID.find( MAKELONG( wMapID, wPartyID));

	if( finder != m_mapTINSTANCEMAPID.end() )
		return WORD((*finder).second);

	return 0;
}

CTMap *CTChannel::FindTMap( WORD wPartyID,
						    WORD wMapID)
{
	MAPTMAP::iterator finder = m_mapTMAP.find(wMapID);

	if( finder == m_mapTMAP.end() )
		return NULL;
	CTMap *pTMAP = (*finder).second;

	if( pTMAP->m_bType != MAP_INDUNTEMP )
		return pTMAP;

	wMapID = GetInstanceMapID(
		wPartyID,
		wMapID);

	if(!wMapID)
		return NULL;
	finder = m_mapTMAP.find(wMapID);

	if( finder != m_mapTMAP.end() && (*finder).second->m_bType != MAP_INDUNTEMP )
		return (*finder).second;

	return NULL;
}

void CTChannel::DeleteInstanceMAP( CTMap *pTMAP)
{
	WORD wMapID = pTMAP->m_wMapID;

	if( pTMAP->m_bType == MAP_INDUN )
	{
		MAPDWORD::iterator finder = m_mapTINSTANCEMAPID.find( MAKELONG( pTMAP->m_wMapID, pTMAP->m_wPartyID));

		if( finder != m_mapTINSTANCEMAPID.end() )
		{
			wMapID = WORD((*finder).second);
			m_mapTINSTANCEMAPID.erase(finder);
		}
	}

	MAPTMAP::iterator finder = m_mapTMAP.find(wMapID);
	if( finder != m_mapTMAP.end() )
		m_mapTMAP.erase(finder);

	delete pTMAP;
}

CTMap *CTChannel::CreateInstanceMAP( WORD wPartyID,
									 WORD wMapID,
									 BYTE bCountryID)
{
	static WORD wInstanceID = 1;

	CTMap *pTMAP = FindTMap(
		wPartyID,
		wMapID);

	if(pTMAP)
		return pTMAP;

	MAPTMAP::iterator finder = m_mapTMAP.find(wMapID);
	if( finder == m_mapTMAP.end() || (*finder).second->m_bType != MAP_INDUNTEMP )
		return NULL;

	pTMAP = new CTMap( (*finder).second, bCountryID);
	pTMAP->m_wPartyID = wPartyID;

	while( !wInstanceID || m_mapTMAP.find(wInstanceID) != m_mapTMAP.end() )
		wInstanceID++;

	m_mapTINSTANCEMAPID.insert( MAPDWORD::value_type(
		MAKELONG( wMapID, wPartyID),
		wInstanceID));
	m_mapTMAP.insert( MAPTMAP::value_type( wInstanceID, pTMAP));

	return pTMAP;
}
