#include "StdAfx.h"
#include ".\tparty.h"

CTParty::CTParty(void)
{
	m_wID = 0;
	m_dwChiefID = 0;
	m_wCorpsID = 0;	
	m_mapMember.clear();
}

CTParty::~CTParty(void)
{
	m_mapMember.clear();
}

CTPlayer* CTParty::FindMember(DWORD dwCharID)
{
	MAPMEMBER::iterator finder = m_mapMember.find(dwCharID);
	if( finder != m_mapMember.end() )
		return (*finder).second;

	return NULL;
}

void CTParty::AddMember(CTPlayer* pMember)
{
	if(!pMember)
		return;

	MAPMEMBER::iterator finder = m_mapMember.find(pMember->m_dwID);
	if(finder == m_mapMember.end() )
	{
		pMember->m_wPartyID = m_wID;
		pMember->m_wCorpsID = m_wCorpsID;
        m_mapMember.insert(MAPMEMBER::value_type(pMember->m_dwID, pMember) );
	}
}

void CTParty::DelMember(DWORD dwCharID)
{
	if(!dwCharID)
		return;

	MAPMEMBER::iterator finder = m_mapMember.find(dwCharID);
	if( finder != m_mapMember.end() )
	{
		(*finder).second->m_wPartyID = 0;
		(*finder).second->m_wCorpsID = 0;
		m_mapMember.erase(finder);	
	}
}

BYTE CTParty::IsChief(DWORD dwID)
{
	if(m_dwChiefID == dwID )
		return TRUE;

	return FALSE;
}

void CTParty::SetCommand(WORD wSquadID,DWORD dwCharID,WORD wMapID,BYTE bCMD, BYTE bTargetType, DWORD dwTargetID, WORD wPosX, WORD wPosZ)
{
	m_Command.m_wSquadID = wSquadID;
	m_Command.m_dwCharID = dwCharID;
	m_Command.m_wMapID = wMapID;
	m_Command.m_bCommand = bCMD;
	m_Command.m_bTgType = bTargetType;
	m_Command.m_dwTgObjID = dwTargetID;
	m_Command.m_wTgPosX = wPosX;
	m_Command.m_wTgPosZ = wPosZ;
}

void CTParty::ResetCorpsID(WORD wCorpsID)
{
	MAPMEMBER::iterator itM;
	for(itM = m_mapMember.begin(); itM != m_mapMember.end(); itM++)
	{
		CTPlayer* pMember = (*itM).second;
		if(pMember)
			pMember->m_wCorpsID = wCorpsID;
	}
	
	m_wCorpsID = wCorpsID;
}

BYTE CTParty::GetMemberCount()
{
	return (BYTE)m_mapMember.size();
}
