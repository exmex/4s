#include "StdAfx.h"
#include ".\tcorps.h"

CTCorps::CTCorps(void)
{
	m_wID = 0;
	m_wCommander = 0;
	m_dwGeneralID = 0;
	m_mapTParty.clear();
}

CTCorps::~CTCorps(void)
{
}

CTParty* CTCorps::FindMember(WORD wPartyID)
{
	MAPTPARTY::iterator itR = m_mapTParty.find(wPartyID);
	if( itR != m_mapTParty.end() )
		return (*itR).second;

	return NULL;
}

void CTCorps::AddMember(CTParty* pParty)
{
	MAPTPARTY::iterator itR = m_mapTParty.find(pParty->m_wID);
	if( itR == m_mapTParty.end() )
	{		
		pParty->ResetCorpsID(m_wID);
		m_mapTParty.insert(MAPTPARTY::value_type(pParty->m_wID,pParty) );
	}
}

void CTCorps::DelMember(WORD wPartyID)
{
	MAPTPARTY::iterator itR = m_mapTParty.find(wPartyID);
	if( itR != m_mapTParty.end() )
	{		
		(*itR).second->ResetCorpsID(0);
		m_mapTParty.erase(itR);
	}
}

BYTE CTCorps::GetMemberCount()
{
	return (BYTE)m_mapTParty.size();
}
