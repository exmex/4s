#include "StdAfx.h"
#include ".\tplayer.h"

CTPlayer::CTPlayer(void)
{
	m_bSessionType = SESSION_CLIENT;	

	m_dwID = 0;
	m_dwKEY = 0;
	m_dwGuildID = 0;
	m_dwPartyChiefID = 0;
	m_dwGeneralID = 0;
	m_dwCheckTick = 0;
	m_dwTacticsID = 0;

	m_wPort = 0;
	m_wPartyID = 0;
	m_wCorpsID = 0;
	m_wMapID = 0;
	m_wUnitID = 0;

	m_bCountry = TCONTRY_N;
	m_bAidCountry = TCONTRY_N;
	m_bGuildDuty = 0;
	m_bReconnect = FALSE;
	m_bAuthorized = FALSE;

	m_strNAME.Empty();
	m_nChatBanTime = 0;

	m_dwAcceptTick = 0;
	m_dwCloseTick = 0;
	m_chatlog.reset();
	m_vChatLog.clear();
}

CTPlayer::~CTPlayer(void)
{
}


WORD CTPlayer::GetPartyID()
{
	return m_wPartyID;
}

WORD CTPlayer::GetCorpsID()
{
	return m_wCorpsID;
}

DWORD CTPlayer::GetGuildID()
{
	return m_dwGuildID;
}

DWORD CTPlayer::GetPartyChiefID()
{
	return m_dwPartyChiefID;
}

BYTE CTPlayer::CheckProtected(CString strTarget, BYTE bOption)
{
	MAPTSTRBYTE::iterator it = m_mapTPROTECTED.find(strTarget);
	if(it!=m_mapTPROTECTED.end() &&
		((*it).second & bOption))
		return FALSE;

	return TRUE;
}

void CTPlayer::AddProtected(CString strName, BYTE bOption)
{
	MAPTSTRBYTE::iterator it = m_mapTPROTECTED.find(strName);
	if(it!=m_mapTPROTECTED.end())
		return;

	m_mapTPROTECTED.insert(MAPTSTRBYTE::value_type(strName, bOption));
}

void CTPlayer::DelProtected(CString strName)
{
	MAPTSTRBYTE::iterator it = m_mapTPROTECTED.find(strName);
	if(it!=m_mapTPROTECTED.end())
		m_mapTPROTECTED.erase(it);
}

BYTE CTPlayer::CheckChat(CString strTalk)
{
	DWORD dwCODE = 0;
	sscanf( LPCSTR(strTalk.Left(8)), _T("%X"), &dwCODE);
	WORD wNewSize = LOWORD(dwCODE);

	DWORD dwTick = GetTickCount();

	if(m_chatlog.m_dwRecent && dwTick - m_chatlog.m_dwRecent < 10000)
	{
		m_chatlog.m_dwSize += wNewSize;
		if(100 < m_chatlog.m_dwSize)
		{
			//CTime t(CTime::GetCurrentTime());
			//if(t.GetTime() < m_nChatBanTime)
			//	m_nChatBanTime += 300;
			//else
			//	m_nChatBanTime = t.GetTime() + 300;	

			m_chatlog.m_dwRecent = dwTick;
			m_chatlog.m_dwSize = 0;

			return FALSE;
		}
	}
	else
	{
		m_chatlog.m_dwRecent = dwTick;
		m_chatlog.m_dwSize = 0;
	}

	TCHATLOG log;
	log.m_dwRecent = dwTick;
	log.m_dwSize = wNewSize;
	m_vChatLog.push_back(log);

	int nCount = int(m_vChatLog.size());
	int nInterval = 0;
	for(int cl=0; cl<nCount-1; cl++)
	{
		int nItv = m_vChatLog[cl+1].m_dwRecent - m_vChatLog[cl].m_dwRecent;
		if(!nInterval)
			nInterval = nItv;
		else
		{
			if(abs(nInterval-nItv) > 1000)
			{
				m_vChatLog.clear();
				break;
			}
			else
				nInterval = nItv;
		}
	}

	if(m_vChatLog.size() > 8)
	{
		//CTime t(CTime::GetCurrentTime());
		//if(t.GetTime() < m_nChatBanTime)
		//	m_nChatBanTime += 300;
		//else
		//	m_nChatBanTime = t.GetTime() + 300;

		m_vChatLog.clear();

		return FALSE;
	}

	return TRUE;
}

BYTE CTPlayer::CanTalk(CTPlayer * pTarget, BYTE bChatType)
{
	if(m_bCountry == TCONTRY_PEACE ||
        pTarget->m_bCountry == TCONTRY_PEACE)
		return TRUE;

	switch(bChatType)
	{
	case CHAT_NEAR:
		{
			if(	(m_bCountry == TCONTRY_B ||
				pTarget->m_bCountry == TCONTRY_B) &&
				m_wUnitID == 3)
				return TRUE;

			if((m_bAidCountry != TCONTRY_N ? m_bAidCountry : m_bCountry) == (pTarget->m_bAidCountry != TCONTRY_N ? pTarget->m_bAidCountry : pTarget->m_bCountry))
				return TRUE;
		}
		break;
	case CHAT_WHISPER:
	case CHAT_SHOW:
	case CHAT_MAP:
	case CHAT_WORLD:
		{
			if((m_bAidCountry != TCONTRY_N ? m_bAidCountry : m_bCountry) == (pTarget->m_bAidCountry != TCONTRY_N ? pTarget->m_bAidCountry : pTarget->m_bCountry))
				return TRUE;
		}
		break;
	}

	return FALSE;
}
