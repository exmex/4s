#include "StdAfx.h"

CTCorps::CTCorps(void)
{
	m_mapParty.clear();
}

CTCorps::~CTCorps(void)
{
	m_mapParty.clear();
}

void CTCorps::Enter(CTParty * pParty)
{
	m_mapParty.insert(MAPTPARTY::value_type(pParty->GetID(), pParty));
	pParty->m_wCorpsID = m_wID;
}

BYTE CTCorps::Leave(CTParty * pParty)
{
	m_mapParty.erase(pParty->GetID());
	pParty->m_wCorpsID = 0;
	memset(&(pParty->m_command), 0, sizeof(TCOMMAND));

	if(m_mapParty.size() == 1)
		return TRUE;
	else
		return FALSE;
}

BYTE CTCorps::IsParty(WORD wPartyID)
{
	MAPTPARTY::iterator it = m_mapParty.find(wPartyID);
	if(it!=m_mapParty.end())
		return TRUE;

	return FALSE;
}

CTParty *CTCorps::FindParty(WORD wPartyID)
{
	MAPTPARTY::iterator it = m_mapParty.find(wPartyID);
	if(it!=m_mapParty.end())
		return (*it).second;

	return NULL;
}

CTParty *CTCorps::GetNextCommander(WORD wPartyID)
{
	if(m_wCommander != wPartyID)
        return NULL;

	MAPTPARTY::iterator it;
	for(it=m_mapParty.begin(); it!=m_mapParty.end(); it++)
	{
		if(m_wCommander != (*it).second->GetID())
		{
			m_wCommander = (*it).second->GetID();
			m_dwGeneralID = (*it).second->GetChiefID();
			return (*it).second;
		}
	}

	return NULL;
}