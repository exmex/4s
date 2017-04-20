pragma once

#include "stdafx.h"
#include "TRelaySvr.h"
#include "TRelaySvrModule.h"

DWORD CTRelaySvrModule::OnSM_DELSESSION_REQ(LPPACKETBUF pBUF)
{
	SetEventCloseSession((CTRelaySession *)pBUF->m_pSESSION );

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnSM_QUITSERVICE_REQ(LPPACKETBUF pBUF)
{
	LogEvent(_T("SM_QUITSERVICE_REQ detected !!"));
	if(m_bService)
		SetServiceStatus(SERVICE_STOP_PENDING);
	PostThreadMessage( m_dwThreadID, WM_QUIT, 0, 0);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_CHATBAN_ACK(LPPACKETBUF pBUF)
{
	CString strName;
	__int64 nChatBanTime;

	pBUF->m_packet
		>> strName
		>> nChatBanTime;

	CTPlayer * pChar = FindPlayer(strName);
	if( pChar )
	{
		if( !pChar->m_bAuthorized )
			return EC_NOERROR;

		pChar->m_nChatBanTime = nChatBanTime;			
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_ENTERCHAR_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	CString strName;
	BYTE bResult;
	BYTE bCountry;
	BYTE bAidCountry;
	DWORD dwGuildID;
	DWORD dwGuildChief;
	BYTE bGuildDuty;
	WORD wPartyID;
	DWORD dwPartyChiefID;
	WORD wCorpsID;
	DWORD dwGeneralID;
	DWORD dwTacticsID;
	DWORD dwTacticsChief;
	WORD wMapID;
	WORD wUnitID;

	pBUF->m_packet
		>> dwCharID
		>> strName
		>> bResult
		>> bCountry
		>> bAidCountry
		>> dwGuildID
		>> dwGuildChief
		>> bGuildDuty
		>> wPartyID
		>> dwPartyChiefID
		>> wCorpsID
		>> dwGeneralID
		>> dwTacticsID
		>> dwTacticsChief
		>> wMapID
		>> wUnitID;

	CTPlayer* pPlayer = FindPlayer(dwCharID);
	if(!pPlayer)
		return EC_NOERROR;
	
	if(!bResult)
	{
		LogEvent("Enter Result Fail %d", pPlayer->m_dwID);
		pPlayer->SendCS_RELAYCHARDATA_ACK(bResult);
		CloseSession( (CTRelaySession*)pPlayer);
		return EC_NOERROR;
	}
	
	pPlayer->m_strNAME = strName;
	pPlayer->m_bCountry = bCountry;
	pPlayer->m_bAidCountry = bAidCountry;
	pPlayer->m_dwGuildID = dwGuildID;
	pPlayer->m_bGuildDuty = bGuildDuty;
	pPlayer->m_wPartyID = wPartyID;
	pPlayer->m_dwPartyChiefID = dwPartyChiefID;
	pPlayer->m_wCorpsID = wCorpsID;
	pPlayer->m_dwGeneralID = dwGeneralID;
	pPlayer->m_bAuthorized = TRUE;
	pPlayer->m_bReconnect = TRUE;
	pPlayer->m_dwTacticsID = dwTacticsID;
	pPlayer->m_wMapID = wMapID;
	pPlayer->m_wUnitID = wUnitID;

	CString strFindName = strName;
	strFindName.MakeUpper();
	m_mapPLAYERNAME.insert( MAPPLAYERNAME::value_type(strFindName, pPlayer) );

	CTGuild* pTGUILD = FindGuild(dwGuildID);
	if( pTGUILD )
	{		
		if(pTGUILD->FindMember(dwCharID))
			pTGUILD->DelMember(dwCharID);

		pTGUILD->AddMember(pPlayer);
	}
	else if( dwGuildID )
	{
		pTGUILD = new CTGuild();
		pTGUILD->m_dwID = dwGuildID;
		pTGUILD->m_dwChiefID = dwGuildChief;
		pTGUILD->AddMember(pPlayer);
		AddGuild(pTGUILD);
	}
	
	CTGuild* pTactics = FindGuild(dwTacticsID);
	if( pTactics )
	{		
		if(pTactics->FindTactics(dwCharID))
			pTactics->DelTactics(dwCharID);

		pTactics->AddTactics(pPlayer);
	}
	else if( dwTacticsID )
	{
		pTactics = new CTGuild();
		pTactics->m_dwID = dwTacticsID;
		pTactics->m_dwChiefID = dwTacticsChief;
		pTactics->AddTactics(pPlayer);
		AddGuild(pTactics);
	}

	CTParty* pTPARTY = FindParty(wPartyID);
	if(pTPARTY)
	{
		if(pTPARTY->m_wCorpsID != wCorpsID )
			LogEvent("Player CorpsID Error");

		if(pTPARTY->FindMember(dwCharID))
			pTPARTY->DelMember(dwCharID);

		pTPARTY->AddMember(pPlayer);
	}
	else if( wPartyID )
	{
		pTPARTY = new CTParty();
		pTPARTY->m_wID = wPartyID;
		pTPARTY->m_dwChiefID = dwPartyChiefID;
		pTPARTY->m_wCorpsID = wCorpsID;
		pTPARTY->AddMember(pPlayer);
		AddParty(pTPARTY);
	}

	CTCorps* pTCORPS = FindCorps(wCorpsID);
	if(pTCORPS)
	{
		if( pTPARTY )
		{
			if(!pTCORPS->FindMember(pTPARTY->m_wID))
				pTCORPS->AddMember(pTPARTY);
		}
	}
	else if(wCorpsID)
	{
		if( pTPARTY)
		{
			pTCORPS = new CTCorps();
			pTCORPS->m_wID = wCorpsID;
			pTCORPS->m_dwGeneralID = dwGeneralID;			
			pTCORPS->AddMember(pTPARTY);
			AddCorps(pTCORPS);
		}
	}

	if(IsMeetingRoom(wMapID, FALSE))
	{
		MAPMEETING::iterator it = m_mapMeeting.find(wMapID);
		if(it!=m_mapMeeting.end())
			it->second.insert(MAPPLAYER::value_type(dwCharID, pPlayer));
		else
		{
			MAPPLAYER mapPlayer;
			mapPlayer.insert(MAPPLAYER::value_type(dwCharID, pPlayer));
			m_mapMeeting.insert(MAPMEETING::value_type(wMapID, mapPlayer));
		}
	}

	pPlayer->SendCS_RELAYCHARDATA_ACK(bResult);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_PARTYADD_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	WORD wPartyID;
	DWORD dwChiefID;

	pBUF->m_packet
		>> dwCharID
		>> wPartyID
		>> dwChiefID;

	CTPlayer* pPlayer = FindPlayer(dwCharID);
	if( !pPlayer)
		return EC_NOERROR;

	if( wPartyID && pPlayer->m_wPartyID && wPartyID != pPlayer->m_wPartyID )
	{
		CTParty* pOldParty = FindParty(pPlayer->m_wPartyID);
		if(pOldParty)
			pOldParty->DelMember(pPlayer->m_dwID);
	}
	
	pPlayer->m_wPartyID = wPartyID;
	pPlayer->m_dwPartyChiefID = dwChiefID;
	
	CTParty* pParty = FindParty(wPartyID);
	if( pParty )
	{
		pParty->m_dwChiefID = dwChiefID;
		CTPlayer* pMember = pParty->FindMember(dwCharID);
		if(pMember)
			pParty->DelMember(dwCharID);

		pParty->AddMember(pPlayer);
	}
	else
	{
		pParty = new CTParty();
		pParty->m_wID = wPartyID;
		pParty->m_dwChiefID = dwChiefID;
		pParty->AddMember(pPlayer);
		AddParty(pParty);
	}

	pPlayer->m_wCorpsID = pParty->m_wCorpsID;

	if(pParty->m_wCorpsID && pParty->m_Command.m_bCommand )
	{		
		pPlayer->SendCS_CORPSCMD_ACK(
			pParty->m_Command.m_wSquadID,
			pParty->m_Command.m_dwCharID,
			pParty->m_Command.m_wMapID,
			pParty->m_Command.m_bCommand,
			pParty->m_Command.m_dwTgObjID,
			pParty->m_Command.m_bTgType,
			pParty->m_Command.m_wTgPosX,
			pParty->m_Command.m_wTgPosZ);
	}
	
	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_PARTYDEL_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	WORD wPartyID;
	DWORD dwChief;

	pBUF->m_packet
		>> dwCharID
		>> wPartyID
		>> dwChief;

	CTPlayer* pPlayer = FindPlayer(dwCharID);
	if( pPlayer )
	{
		pPlayer->m_wPartyID = 0;
		pPlayer->m_dwPartyChiefID = 0;
		pPlayer->m_wCorpsID = 0;	
	}

	CTParty* pParty = FindParty(wPartyID);
	if( pParty )
	{
		CTPlayer* pMember = pParty->FindMember(dwCharID);
		if(pMember)
		{
			pParty->m_dwChiefID = dwChief;
			pParty->DelMember(dwCharID);
		}

		CTCorps* pCorps = FindCorps(pParty->m_wCorpsID);
		if(pCorps && !pParty->GetMemberCount())
		{
			pCorps->DelMember(pParty->m_wID);

			if(!pCorps->GetMemberCount() )
				DelCorps(pCorps->m_wID);
		}

		if( !pParty->GetMemberCount())
			DelParty(pParty->m_wID);
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_PARTYCHGCHIEF_ACK(LPPACKETBUF pBUF)
{
	WORD wPartyID;
	DWORD dwChief;

	pBUF->m_packet
		>> wPartyID
		>> dwChief;

	CTParty* pParty = FindParty(wPartyID);
	if(pParty)	
        ChgPartyChief(pParty,dwChief);
			
	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_GUILDADD_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwGuildID;
	DWORD dwGuildMaster;

	pBUF->m_packet
		>> dwCharID
		>> dwGuildID
		>> dwGuildMaster;

	CTPlayer* pPlayer = FindPlayer(dwCharID);
	if( !pPlayer )
		return EC_NOERROR;

	if( dwGuildID && pPlayer->m_dwGuildID && dwGuildID != pPlayer->m_dwGuildID )
	{
		CTGuild* pOldGuild = FindGuild(pPlayer->m_dwGuildID);
		if(pOldGuild)
			pOldGuild->DelMember(pPlayer->m_dwID);
	}


	pPlayer->m_dwGuildID = dwGuildID;
	
	CTGuild* pGUILD = FindGuild(dwGuildID);
	if(pGUILD )
	{
		pGUILD->m_dwChiefID = dwGuildMaster;

		CTPlayer* pMember = pGUILD->FindMember(dwCharID);
		if(pMember)
			pGUILD->DelMember(dwCharID);

		pGUILD->AddMember(pPlayer);		
	}
	else
	{
		pGUILD = new CTGuild();
		pGUILD->m_dwID = dwGuildID;
		pGUILD->m_dwChiefID = dwGuildMaster;
		pGUILD->AddMember(pPlayer);
		AddGuild(pGUILD);
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_GUILDDEL_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwGuildID;

	pBUF->m_packet
		>> dwCharID
		>> dwGuildID;

	CTPlayer* pPlayer = FindPlayer(dwCharID);
	if(pPlayer)
		pPlayer->m_dwGuildID = 0;

	CTGuild* pGuild = FindGuild(dwGuildID);
	if(pGuild)
	{
		CTPlayer* pMember = pGuild->FindMember(dwCharID);
		if(pMember)
			pGuild->DelMember(dwCharID);

		if(pGuild->IsEmpty() )
			DelGuild(dwGuildID);
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_GUILDCHGMASTER_ACK(LPPACKETBUF pBUF)
{
	DWORD dwGuildID;
	DWORD dwMasterID;

	pBUF->m_packet
		>> dwGuildID
		>> dwMasterID;

	CTGuild* pGuild = FindGuild(dwGuildID);
	if(pGuild)
		ChgGuildMaster(pGuild,dwMasterID);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_CORPSJOIN_ACK(LPPACKETBUF pBUF)
{
	WORD wPartyID;
	WORD wCorpsID;
	WORD wCommander;

	pBUF->m_packet
		>> wPartyID
		>> wCorpsID
		>> wCommander;

	CTParty* pParty = FindParty(wPartyID);
	if(!pParty)
		return EC_NOERROR;
	
	if(wCorpsID && wPartyID && pParty->m_wCorpsID != wCorpsID )
	{
		CTCorps* pOldCorps = FindCorps(pParty->m_wCorpsID);
		if(pOldCorps)
			pOldCorps->DelMember(pParty->m_wID);
	}

	WORD wTCorpsID = wCorpsID ? wCorpsID : pParty->m_wCorpsID;
	CTCorps* pCorps = FindCorps(wTCorpsID);
	if( pCorps && wCorpsID )
	{					
		if(!pCorps->FindMember(wPartyID))
			pCorps->AddMember(pParty);	
		
		pCorps->m_wCommander = wCommander;
	}
	else if(wCorpsID)
	{
		if(pParty)
		{
			pCorps = new CTCorps();
			pCorps->m_wID = wCorpsID;
			pCorps->m_wCommander = wCommander;
			pCorps->AddMember(pParty);
			AddCorps(pCorps);
		}
	}	
	if(pCorps && !wCorpsID )
	{			
		pCorps->m_wCommander = wCommander;
		pCorps->DelMember(wPartyID);
		if( !pCorps->GetMemberCount() )
			DelCorps(wTCorpsID);
	}			

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_RELAYSVR_ACK(LPPACKETBUF pBUF)
{
	WORD wCount;
	DWORD dwOperatorID;

	pBUF->m_packet
		>> m_bNation
		>> wCount;

	for(DWORD i = 0; i < wCount ; i++)
	{
		pBUF->m_packet
			>> dwOperatorID;

		m_vTOPERATOR.push_back(dwOperatorID);
	}

	pBUF->m_packet
		>> wCount;

	DWORD dwID;
	CString strMsg;

	for(i=0; i<wCount; i++)
	{
		pBUF->m_packet
			>> dwID
			>> strMsg;

		m_mapTSvrMsg.insert(MAPTSTRING::value_type(dwID, strMsg));
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_CHANGENAME_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	BYTE bType;
	BYTE bValue;
	CString strName;

	pBUF->m_packet
		>> dwCharID
		>> bType
		>> bValue
		>> strName;

	CTPlayer* pPlayer = FindPlayer(dwCharID);
	if(!pPlayer)
		return EC_NOERROR;
    
	switch(bType)
	{
	case IK_NAME:
		{
			CString strOriName = pPlayer->m_strNAME;
			strOriName.MakeUpper();
			MAPPLAYERNAME::iterator itP = m_mapPLAYERNAME.find(strOriName);
			if(itP != m_mapPLAYERNAME.end() )
				m_mapPLAYERNAME.erase(itP);

			pPlayer->m_strNAME = strName;
			CString strChgName = strName;
			strChgName.MakeUpper();
			m_mapPLAYERNAME.insert( MAPPLAYERNAME::value_type(strChgName, pPlayer) );
		}
		break;
	case IK_COUNTRY:
		pPlayer->m_bCountry = bValue;
		break;
	case IK_AIDCOUNTRY:
		pPlayer->m_bAidCountry = bValue;
		break;
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_TACTICSADD_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwGuildID;
	DWORD dwGuildMaster;

	pBUF->m_packet
		>> dwCharID
		>> dwGuildID
		>> dwGuildMaster;

	CTPlayer* pPlayer = FindPlayer(dwCharID);
	if( !pPlayer )
		return EC_NOERROR;

	if( dwGuildID && pPlayer->m_dwTacticsID && dwGuildID != pPlayer->m_dwTacticsID )
	{
		CTGuild* pOldGuild = FindGuild(pPlayer->m_dwTacticsID);
		if(pOldGuild)
			pOldGuild->DelTactics(pPlayer->m_dwID);
	}

	pPlayer->m_dwTacticsID = dwGuildID;
	
	CTGuild* pGUILD = FindGuild(dwGuildID);
	if(pGUILD )
	{
		CTPlayer* pMember = pGUILD->FindTactics(dwCharID);
		if(pMember)
			pGUILD->DelTactics(dwCharID);

		pGUILD->AddTactics(pPlayer);		
	}
	else
	{
		pGUILD = new CTGuild();
		pGUILD->m_dwID = dwGuildID;
		pGUILD->m_dwChiefID = dwGuildMaster;
		pGUILD->AddTactics(pPlayer);
		AddGuild(pGUILD);
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_TACTICSDEL_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	DWORD dwGuildID;

	pBUF->m_packet
		>> dwCharID
		>> dwGuildID;

	CTPlayer* pPlayer = FindPlayer(dwCharID);
	if(pPlayer)
		pPlayer->m_dwTacticsID = 0;

	CTGuild* pGuild = FindGuild(dwGuildID);
	if(pGuild)
	{
		CTPlayer* pMember = pGuild->FindTactics(dwCharID);
		if(pMember)
			pGuild->DelTactics(dwCharID);

		if(pGuild->IsEmpty() )
			DelGuild(dwGuildID);
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnRW_CHANGEMAP_ACK(LPPACKETBUF pBUF)
{
	DWORD dwCharID;
	WORD wMapID;
	WORD wUnitID;

	pBUF->m_packet
		>> dwCharID
		>> wMapID
		>> wUnitID;

	CTPlayer * pPlayer = FindPlayer(dwCharID);
	if(!pPlayer)
		return EC_NOERROR;

	if(IsMeetingRoom(pPlayer->m_wMapID, FALSE))
	{
		MAPMEETING::iterator it = m_mapMeeting.find(pPlayer->m_wMapID);
		if(it!=m_mapMeeting.end())
			it->second.erase(dwCharID);
	}

	if(IsMeetingRoom(wMapID, FALSE))
	{
		MAPMEETING::iterator it = m_mapMeeting.find(wMapID);
		if(it!=m_mapMeeting.end())
			it->second.insert(MAPPLAYER::value_type(dwCharID, pPlayer));
		else
		{
			MAPPLAYER mapPlayer;
			mapPlayer.insert(MAPPLAYER::value_type(dwCharID, pPlayer));
			m_mapMeeting.insert(MAPMEETING::value_type(wMapID, mapPlayer));
		}
	}

	pPlayer->m_wMapID = wMapID;
	pPlayer->m_wUnitID = wUnitID;

	return EC_NOERROR;
}
