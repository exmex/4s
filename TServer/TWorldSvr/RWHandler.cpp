#include "stdafx.h"
#include "TWorldSvr.h"
#include "TWorldSvrModule.h"

DWORD CTWorldSvrModule::OnRW_RELAYSVR_REQ(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	WORD wID;
	pBUF->m_packet
		>> wID;

	pSERVER->m_wID = wID;
	m_pRelay = pSERVER;

	m_pRelay->SendRW_RELAYSVR_ACK(
		m_bNation,
		&m_vTOPERATOR,
		&m_mapTSvrMsg);

	MAPTSERVER::iterator it;
	for(it=m_mapSERVER.begin(); it!=m_mapSERVER.end(); it++)
		(*it).second->SendMW_RELAYCONNECT_REQ(0);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnRW_ENTERCHAR_REQ(LPPACKETBUF pBUF)
{
	CTServer *pSERVER = (CTServer *) pBUF->m_pSESSION;

	DWORD dwCharID;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> strName;

	LPTCHARACTER pChar = FindTChar(strName);
	if(!pChar ||
		pChar->m_dwCharID != dwCharID )
	{
		pSERVER->SendRW_ENTERCHAR_ACK(dwCharID, strName, FALSE);
		return EC_NOERROR;
	}

	DWORD dwGuildID = 0;
	DWORD dwGuildChief = 0;
	DWORD dwTacticsID = 0;
	DWORD dwTacticsChief = 0;
	BYTE bDuty = GUILD_DUTY_NONE;

	CTGuild * pGuild = pChar->m_pGuild;
	if(pGuild)
	{
		LPTGUILDMEMBER pMember = pGuild->FindMember(dwCharID);
		if(pMember)
		{
			dwGuildID = pGuild->m_dwID;
			dwGuildChief = pGuild->m_dwChief;
			bDuty = pMember->m_bDuty;
		}
	}
	CTGuild * pTactics = pChar->m_pTactics;
	if(pTactics)
	{
		LPTTACTICSMEMBER pMember = pTactics->FindTactics(dwCharID);
		if(pMember)
		{
			dwTacticsID = pTactics->m_dwID;
			dwTacticsChief = pTactics->m_dwChief;
		}
	}

	WORD wPartyID = 0;
	DWORD dwChiefID = 0;
	WORD wCorpsID = 0;
	DWORD dwGeneralID = 0;

	if(pChar->m_pParty)
	{
		wPartyID = pChar->m_pParty->GetID();
		dwChiefID = pChar->m_pParty->GetChiefID();
		CTCorps * pCorps = FindCorps(pChar->m_pParty->m_wCorpsID);
		if(pCorps)
		{
			wCorpsID = pCorps->m_wID;
			dwGeneralID = pCorps->m_dwGeneralID;
		}
	}

	pSERVER->SendRW_ENTERCHAR_ACK(
		dwCharID,
		strName,
		TRUE,
		pChar->m_bCountry,
		pChar->m_bAidCountry,
		dwGuildID,
		dwGuildChief,
		bDuty,
		wPartyID,
		dwChiefID,
		wCorpsID,
		dwGeneralID,
		dwTacticsID,
		dwTacticsChief,
		pChar->m_wMapID,
		MAKEWORD( BYTE(INT(pChar->m_fPosX) / UNIT_SIZE), BYTE(INT(pChar->m_fPosZ) / UNIT_SIZE)));

	CheckChatBan(pChar, TRUE);

	return EC_NOERROR;
}

DWORD CTWorldSvrModule::OnRW_RELAYCONNECT_REQ(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	pBUF->m_packet
		>> dwCharID;

	MAPTCHARACTER::iterator itCHAR = m_mapTCHAR.find(dwCharID);

	if( itCHAR == m_mapTCHAR.end())
		return EC_NOERROR;

	LPTCHARACTER pChar = (*itCHAR).second;
	if(pChar)
	{
		CTServer * pMain = FindMapSvr(pChar->m_bMainID);
		if(pMain)
			pMain->SendMW_RELAYCONNECT_REQ(dwCharID);
	}
	return EC_NOERROR;
}
