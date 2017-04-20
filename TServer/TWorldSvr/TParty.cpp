#include "StdAfx.h"

CTParty::CTParty(void)
{
	m_vMember.clear();
	m_wCorpsID = 0;
	m_dwChiefID = 0;
	m_dwOrder = 0;
	m_bArena = FALSE;
}

CTParty::~CTParty(void)
{
	m_vMember.clear();
}

void CTParty::SetID(WORD wID)
{
	m_wID = wID;
}
WORD CTParty::GetID()
{
	return m_wID;
}
BYTE CTParty::IsChief(DWORD dwID)
{
	if(dwID == m_dwChiefID)
		return TRUE;
	else
		return FALSE;
}
BYTE CTParty::IsMember(DWORD dwID)
{
	for(int i=0; i < (int)m_vMember.size(); i++)
	{
		if(dwID == m_vMember[i]->m_dwCharID )
			return TRUE;
	}
	return FALSE;
}
BYTE CTParty::IsMember(CString strName)
{
	for(int i=0; i < (int)m_vMember.size(); i++)
	{
		if(!strName.CompareNoCase(m_vMember[i]->m_strNAME))
			return TRUE;
	}
	return FALSE;
}
BYTE CTParty::GetSize()
{
	return (BYTE)m_vMember.size();
}
BYTE CTParty::AddMember(LPTCHARACTER pMember)
{
	if(!m_vMember.size())
		m_dwOrder = pMember->m_dwCharID;

	if(!pMember->m_pParty)
	{
		pMember->m_pParty = this;
		m_vMember.push_back(pMember);
		return TRUE;
	}
	else
		return FALSE;
}
BYTE CTParty::DelMember(DWORD dwID)
{
	for(int i=0; i< (int)m_vMember.size(); i++)
	{
		if(m_vMember[i]->m_dwCharID == dwID)
		{
			if(m_vMember[i]->m_dwCharID == m_dwOrder)
				SetNextOrder(dwID);

			memset(&(m_vMember[i]->m_command),0,sizeof(TCOMMAND));
			m_vMember[i]->m_pParty = NULL;
			m_vMember.erase(m_vMember.begin()+i);
			return TRUE;
		}
	}
	return FALSE;
}
BYTE CTParty::DelMember(CString strName)
{
	for(int i=0; i< (int)m_vMember.size(); i++)
	{
		if(m_vMember[i]->m_strNAME == strName)
		{
			if(m_vMember[i]->m_dwCharID == m_dwOrder)
				SetNextOrder(m_dwOrder);

			memset(&(m_vMember[i]->m_command),0,sizeof(TCOMMAND));
			m_vMember[i]->m_pParty = NULL;
			m_vMember[i]->m_bPartyWaiter = FALSE;
			m_vMember.erase(m_vMember.begin()+i);
			return TRUE;
		}
	}
	return FALSE;
}
void CTParty::SetChiefID(DWORD dwChiefID)
{
	m_dwChiefID = dwChiefID;
}
DWORD CTParty::GetChiefID()
{
	return m_dwChiefID;
}
LPTCHARACTER CTParty::GetMember(int index)
{
	if(index >= (int)m_vMember.size())
		return NULL;

	return m_vMember[index];
}
LPTCHARACTER CTParty::FindMember(DWORD dwCharID)
{
	for(DWORD i=0; i<m_vMember.size(); i++)
		if(m_vMember[i]->m_dwCharID == dwCharID)
			return m_vMember[i];
	return NULL;
}
LPTCHARACTER CTParty::GetNextChief(DWORD dwCharID)
{
	if(!IsChief(dwCharID))
		return NULL;

	if(m_vMember.size() > 1)
	{
		for(DWORD i=0; i<m_vMember.size(); i++)
		{
			if(m_dwChiefID != m_vMember[i]->m_dwCharID)
			{
				m_dwChiefID = m_vMember[i]->m_dwCharID;
				return m_vMember[i];
			}
		}
	}
	
	return NULL;
}
LPTCHARACTER CTParty::GetChief()
{
	return FindMember(m_dwChiefID);
}

BYTE CTParty::GetOrderIndex(DWORD dwID)
{
	for(BYTE i=0; i<(BYTE)m_vMember.size(); i++)
		if(m_vMember[i]->m_dwCharID == dwID)
			return i;

	return 0;
}
void CTParty::SetNextOrder(DWORD dwID)
{
	BYTE bIndex = GetOrderIndex(dwID) + 1;
	m_dwOrder = (m_vMember.size() <= bIndex) ? m_vMember[0]->m_dwCharID : m_vMember[bIndex]->m_dwCharID;
}
LPTCHARACTER CTParty::GetNextOrder(VECTORDWORD vMember)
{
	MAPBDWDRD mapINDEX;
	mapINDEX.clear();

	for(BYTE i=0; i<(BYTE)vMember.size(); i++)
		for(BYTE j=0; j<(BYTE)m_vMember.size(); j++)
			if(m_vMember[j]->m_dwCharID == vMember[i])
				mapINDEX.insert(MAPBDWDRD::value_type(j, m_vMember[j]->m_dwCharID));

	if(mapINDEX.empty())
		return NULL;

	LPTCHARACTER pNext = NULL;

	MAPBDWDRD::iterator it;
	for(it = mapINDEX.begin(); it!=mapINDEX.end(); it++)
		if((*it).second == m_dwOrder)
		{
			pNext = FindMember((*it).second);
			SetNextOrder((*it).second);
			mapINDEX.clear();
			return pNext;
		}

	BYTE bIndex = GetOrderIndex(m_dwOrder);
	for(it = mapINDEX.begin(); it!=mapINDEX.end(); it++)
		if((*it).first > bIndex)
		{
			pNext = FindMember((*it).second);
			SetNextOrder((*it).second);
			mapINDEX.clear();
			return pNext;
		}

	if(!pNext)
	{
		DWORD dwOrderID = (*mapINDEX.begin()).second;
		pNext = FindMember(dwOrderID);
		SetNextOrder(dwOrderID);
	}

	mapINDEX.clear();

	return pNext;
}