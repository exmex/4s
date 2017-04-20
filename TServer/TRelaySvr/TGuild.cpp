#include "StdAfx.h"
#include ".\tguild.h"

CTGuild::CTGuild(void)
{
	m_dwID = 0;
	m_dwChiefID = 0;
	m_mapMember.clear();
	m_mapTactics.clear();
}

CTGuild::~CTGuild(void)
{
}

void CTGuild::AddMember(CTPlayer* pPlayer)
{
	if(!pPlayer)
		return;

	MAPMEMBER::iterator itM = m_mapMember.find(pPlayer->m_dwID);
	if( itM == m_mapMember.end() )
	{
		pPlayer->m_dwGuildID = m_dwID;
        m_mapMember.insert(MAPMEMBER::value_type(pPlayer->m_dwID,pPlayer) );
	}
}

void CTGuild::DelMember(DWORD dwID)
{
	if(!dwID)
		return;

	MAPMEMBER::iterator itM = m_mapMember.find(dwID);
	if( itM != m_mapMember.end() )
	{
		(*itM).second->m_dwGuildID = 0;
		m_mapMember.erase(itM);	
	}
}

CTPlayer* CTGuild::FindMember(DWORD dwID)
{
	if(!dwID)
		return NULL;

	MAPMEMBER::iterator itM = m_mapMember.find(dwID);
	if( itM != m_mapMember.end() )
		return (*itM).second;

	return NULL;
}

BYTE CTGuild::IsEmpty()
{
	return m_mapMember.empty() && m_mapTactics.empty();
}

void CTGuild::AddTactics(CTPlayer* pPlayer)
{
	if(!pPlayer)
		return;

	MAPMEMBER::iterator itM = m_mapTactics.find(pPlayer->m_dwID);
	if( itM == m_mapTactics.end() )
	{
		pPlayer->m_dwTacticsID = m_dwID;
        m_mapTactics.insert(MAPMEMBER::value_type(pPlayer->m_dwID,pPlayer) );
	}
}

void CTGuild::DelTactics(DWORD dwID)
{
	if(!dwID)
		return;

	MAPMEMBER::iterator itM = m_mapTactics.find(dwID);
	if( itM != m_mapTactics.end() )
	{
		(*itM).second->m_dwTacticsID = 0;
		m_mapTactics.erase(itM);	
	}
}

CTPlayer* CTGuild::FindTactics(DWORD dwID)
{
	if(!dwID)
		return NULL;

	MAPMEMBER::iterator itM = m_mapTactics.find(dwID);
	if( itM != m_mapTactics.end() )
		return (*itM).second;

	return NULL;
}
