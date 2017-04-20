#pragma once

#include "stdafx.h"
#include "TRelaySvr.h"
#include "TRelaySvrModule.h"


DWORD CTRelaySvrModule::OnCT_SERVICEMONITOR_ACK(LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;

	DWORD dwTick;
	pBUF->m_packet
		>> dwTick;

	pPlayer->SendCT_SERVICEMONITOR_REQ(dwTick, DWORD(m_mapSESSION.size()), DWORD(m_mapPLAYER.size()), 0 );

	// Connect 만 하고 오랜시간 동안 인증 받지 않은 세센 삭제
	static DWORD tick = GetTickCount();
	DWORD dwCurrent = GetTickCount();

	if(dwCurrent - tick > 10000)
	{
		MAPPLAYER::iterator it;
		for(it=m_mapSESSION.begin(); it!=m_mapSESSION.end(); it++)
		{
			if((*it).second->m_bSessionType == SESSION_CLIENT)
			{
				if((*it).second->m_dwAcceptTick &&
					dwCurrent - (*it).second->m_dwAcceptTick > 60000)
				{
					LogEvent("Accept Tick %d", (*it).second->m_dwID);
					CloseSession((*it).second);
				}

				if((*it).second->m_dwCheckTick &&
					!(*it).second->m_dwCloseTick &&
					dwCurrent - (*it).second->m_dwCheckTick > 300000)
				{
					CloseSession((*it).second);
				}
			}
		}
		tick = dwCurrent;
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCT_ANNOUNCEMENT_ACK( LPPACKETBUF pBUF )
{
	MAPPLAYER::iterator it;
	CString strAnnounce;

	pBUF->m_packet
		>> strAnnounce;

	for( it = m_mapPLAYER.begin(); it != m_mapPLAYER.end(); it++)
	{
		CTPlayer* pPlayer = (*it).second;
		if( pPlayer && pPlayer->m_bAuthorized )
            pPlayer->SendCS_CHAT_ACK( CHAT_WORLD, 0, GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strAnnounce));
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCT_CHATBAN_REQ( LPPACKETBUF pBUF)
{
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if(!pPlayer)
		return EC_NOERROR;

	CString strName;
	WORD wMin;
	DWORD dwBanSeq;

	pBUF->m_packet
		>> strName
		>> wMin
		>> dwBanSeq;

	CTPlayer* pTarget = FindPlayer(strName);
	if(!pTarget)
	{
		pPlayer->SendCT_CHATBAN_ACK(FALSE,dwBanSeq);
		return EC_NOERROR;
	}
	
	CTime t(CTime::GetCurrentTime());
	CTime BanTime(t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute()+wMin,t.GetSecond());

	pTarget->m_nChatBanTime = BanTime.GetTime();

	pPlayer->SendCT_CHATBAN_ACK(TRUE,dwBanSeq);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCT_CHARMSG_ACK(LPPACKETBUF pBUF)
{
	CString strName;
	CString strMsg;

	pBUF->m_packet
		>> strName
		>> strMsg;

	CTPlayer* pTarget = FindPlayer(strName);
	if(!pTarget)
		return EC_NOERROR;

	strMsg.Left(ONE_KBYTE);

	pTarget->SendCS_CHAT_ACK(CHAT_OPERATOR,0, GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strMsg));

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCT_SERVICEDATACLEAR_ACK(LPPACKETBUF pBUF)
{
	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCT_CTRLSVR_REQ(LPPACKETBUF pBUF)
{
	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCT_EVENTMSG_REQ(LPPACKETBUF pBUF)
{	
	BYTE bEventID;
	BYTE bEventMsgType;
	CString strMsg;

	pBUF->m_packet
		>> bEventID
		>> bEventMsgType
		>> strMsg;

	MAPPLAYER::iterator it;
	for( it = m_mapPLAYER.begin(); it != m_mapPLAYER.end(); it++)
	{
		CTPlayer* pPlayer = (*it).second;
		if( pPlayer && pPlayer->m_bAuthorized )
            pPlayer->SendCS_CHAT_ACK( CHAT_WORLD, 0, GetSvrMsg(NAME_OPERATOR), BuildNetString(NAME_NULL, strMsg));		
	}

	return EC_NOERROR;
}


////////////////////////////////////////////////////////////////////////////

DWORD CTRelaySvrModule::OnCS_RELAYCHARDATA_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer )
		return EC_NOERROR;

	WORD wVersion;	

	pBUF->m_packet
		>> wVersion
		>> pPlayer->m_dwID
		>> pPlayer->m_strNAME;

	if( wVersion != TVERSION )
	{
		LogEvent("Version Invalid %d", pPlayer->m_dwID);
		pPlayer->SendCS_RELAYCHARDATA_ACK(FALSE);

		return EC_SESSION_INVALIDCHAR;
	}

	MAPPLAYER::iterator itCHAR = m_mapPLAYER.find(pPlayer->m_dwID);
	if( itCHAR != m_mapPLAYER.end() )
	{
		CTPlayer * pChar = (*itCHAR).second;
		LogEvent("reconnect %d, %d", pPlayer->m_dwID, pPlayer->m_bAuthorized);
		ClearCharData(pChar);
		CloseSession(pChar);
	}

	pPlayer->m_dwAcceptTick = 0;

	m_mapPLAYER.insert( MAPPLAYER::value_type( pPlayer->m_dwID, pPlayer));
	SendRW_ENTERCHAR_REQ(pPlayer->m_dwID, pPlayer->m_strNAME);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_CHAT_REQ(LPPACKETBUF pBUF)
{	
	CTPlayer *pPlayer = (CTPlayer *) pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized)
		return EC_NOERROR;

	BYTE bGroup;
	DWORD dwTarget;
	CString strName; // target Name
	CString strTalk;
	CString strSender;
	WORD wCount;
	
	__int64 timeCurrent = CTime::GetCurrentTime().GetTime();

	pBUF->m_packet
		>> strSender
		>> bGroup
		>> dwTarget
		>> strName
		>> strTalk;

	if(bGroup == CHAT_WORLD || bGroup == CHAT_OPERATOR)
		return EC_NOERROR;

	strTalk.Left(ONE_KBYTE);

	BYTE bType = bGroup;

	if(IsOperator(pPlayer->m_dwID))
		bType = CHAT_OPERATOR;

	if(bGroup != CHAT_SHOW)
	{
		if(!pPlayer->CheckChat(strTalk))
			SendRW_CHATBAN_ACK(pPlayer->m_strNAME,5);
	}
	else
	{
		if(strTalk.Find(pPlayer->m_strNAME) == -1)
			return EC_NOERROR;

		if(dwTarget != strTalk.GetLength())
			return EC_NOERROR;
	}

	if( timeCurrent < pPlayer->m_nChatBanTime) 
	{
		pPlayer->SendCS_SYSTEMMSG_ACK(
			SM_CHAT_BAN,
			0, DWORD(pPlayer->m_nChatBanTime - timeCurrent),
			NAME_NULL, NAME_NULL,
			0,0,NAME_NULL,0);

		return EC_NOERROR;
	}

	switch(bGroup)
	{
	case CHAT_TACTICS:
		{
			dwTarget = pPlayer->m_dwTacticsID ? pPlayer->m_dwTacticsID : pPlayer->m_dwGuildID;			

			CTGuild * pGUILD = FindGuild(dwTarget);
			if(!pGUILD)
				return EC_NOERROR;

			MAPMEMBER::iterator itM;
			for(itM = pGUILD->m_mapMember.begin(); itM != pGUILD->m_mapMember.end(); itM++)
				(*itM).second->SendCS_CHAT_ACK(bGroup,pPlayer->m_dwID, strSender, strTalk);

			for(itM=pGUILD->m_mapTactics.begin(); itM!=pGUILD->m_mapTactics.end(); itM++)
				(*itM).second->SendCS_CHAT_ACK(bGroup,pPlayer->m_dwID, strSender, strTalk);
		}
		break;
	case CHAT_NEAR:
		{
			if(IsMeetingRoom(pPlayer->m_wMapID, FALSE))
			{
				MAPMEETING::iterator it = m_mapMeeting.find(pPlayer->m_wMapID);
				if(it!=m_mapMeeting.end())
				{
					MAPPLAYER::iterator itP;
					for(itP=it->second.begin(); itP != it->second.end(); itP++)
						itP->second->SendCS_CHAT_ACK(bGroup,pPlayer->m_dwID,strSender,strTalk);
				}
			}
			else
			{
				pBUF->m_packet
					>> wCount;

				for(DWORD i = 0; i < wCount; i++)
				{
					pBUF->m_packet
						>> dwTarget;

					CTPlayer * pTarget = FindPlayer(dwTarget);

					if(pTarget && pPlayer->CanTalk(pTarget, bGroup))
						pTarget->SendCS_CHAT_ACK(bGroup,pPlayer->m_dwID,strSender,strTalk);
				}

				pPlayer->SendCS_CHAT_ACK(bGroup,pPlayer->m_dwID,strSender,strTalk);
			}
		}
		break;

	case CHAT_WHISPER:
		{
			if(strName == GetSvrMsg(NAME_OPERATOR))
			{	
				BYTE bCount = 0;
				for(DWORD i = 0; i < m_vTOPERATOR.size(); i++)
				{
					CTPlayer* pOperator = FindPlayer( m_vTOPERATOR[i] );
					if(pOperator && pOperator->m_bCountry == pPlayer->m_bCountry)
					{
						pOperator->SendCS_CHAT_ACK(bType,pPlayer->m_dwID,strSender,strTalk);
						bCount++;
					}
				}
				if(!bCount)
				{
					strTalk = BuildNetString(NAME_NULL, GetSvrMsg(MSG_CHAR_LOGOUT));
					bGroup = CHAT_OPERATOR;
				}

#ifdef DEF_UDPLOG
				m_pUdpSocket->LogChat(LOGMAP_CHATOPERATOR, pPlayer, NULL, strTalk);
#endif

				pPlayer->SendCS_CHAT_ACK(bGroup,pPlayer->m_dwID,strName,strTalk);

				return EC_NOERROR;
			}


			MAPPLAYER::iterator itPlayer = m_mapPLAYER.find(dwTarget);
			if( itPlayer != m_mapPLAYER.end() &&
				pPlayer->CanTalk((*itPlayer).second, bGroup))
			{				
				(*itPlayer).second->SendCS_CHAT_ACK(
					bType,
					pPlayer->m_dwID,
					strSender,
					strTalk);
#ifdef DEF_UDPLOG
                    m_pUdpSocket->LogChat(LOGMAP_CHATWISPER, pPlayer, (*itPlayer).second->m_strNAME, strTalk);
#endif
				
				pPlayer->SendCS_CHAT_ACK(
					bGroup,
					pPlayer->m_dwID,
					(*itPlayer).second->m_strNAME,
					strTalk);

				return EC_NOERROR;
			}

			CTPlayer * pTarget = FindPlayer(strName);
			if( pTarget &&
				pPlayer->CanTalk(pTarget, bGroup) )
			{
				pTarget->SendCS_CHAT_ACK(
					bType,
					pPlayer->m_dwID,
					strSender,
					strTalk);
#ifdef DEF_UDPLOG
                    m_pUdpSocket->LogChat(LOGMAP_CHATWISPER, pPlayer, pTarget->m_strNAME, strTalk);
#endif

				pPlayer->SendCS_CHAT_ACK(
					bGroup,
					pPlayer->m_dwID,
					strName,
					strTalk);

				return EC_NOERROR;
			}
		}
		break;

	case CHAT_PARTY:
		{
			WORD wTarget = pPlayer->GetPartyID();			
			CTParty* pPARTY = FindParty(wTarget);
			if(!pPARTY)
				break;

			MAPMEMBER::iterator itM;
			for(itM = pPARTY->m_mapMember.begin(); itM != pPARTY->m_mapMember.end(); itM++)
			{
				CTPlayer* pTarget = (*itM).second;
				if( pTarget)
					pTarget->SendCS_CHAT_ACK(bGroup, pPlayer->m_dwID, strSender, strTalk);
			}
		}
		break;

	case CHAT_GUILD:
		{
			dwTarget = pPlayer->m_dwGuildID;
			
			CTGuild* pGUILD = FindGuild(dwTarget);
			if(!pGUILD)
				break;

			MAPMEMBER::iterator itM;
			for(itM = pGUILD->m_mapMember.begin(); itM != pGUILD->m_mapMember.end(); itM++)
			{
				CTPlayer* pTarget = (*itM).second;
				if( pTarget )
					pTarget->SendCS_CHAT_ACK(bGroup,pPlayer->m_dwID, strSender, strTalk);
			}
		}
		break;

	case CHAT_FORCE:
		{			
			CTCorps* pCORPS = FindCorps(pPlayer);
			if(!pCORPS)						
                return EC_NOERROR;
			
			
			MAPTPARTY::iterator itR;			
			for( itR = pCORPS->m_mapTParty.begin(); itR != pCORPS->m_mapTParty.end(); itR++)
			{
				CTParty* pPARTY = (*itR).second;
				if( !pPARTY )
					continue;
				
				MAPMEMBER::iterator itM;
				for( itM = pPARTY->m_mapMember.begin(); itM != pPARTY->m_mapMember.end(); itM++)
				{
					CTPlayer* pTarget = (*itM).second;
					if(pTarget)
						pTarget->SendCS_CHAT_ACK(bGroup,pPlayer->m_dwID, strSender, strTalk);
				}
			}
		}
		break;

	case CHAT_SHOW:
	case CHAT_MAP:
	case CHAT_WORLD:
		{
			MAPPLAYER::iterator itP;
			for(itP = m_mapPLAYER.begin(); itP != m_mapPLAYER.end(); itP++)
			{
				CTPlayer* pTarget = (*itP).second;
				if(pTarget && pTarget->m_bAuthorized && pPlayer->CanTalk(pTarget, bGroup))
					pTarget->SendCS_CHAT_ACK(bType, pPlayer->m_dwID, strSender, strTalk);
			}
		}

		break;
	}
#ifdef DEF_UDPLOG

		//	m_pUdpSocket->LogChat(LOGMAP_CHATWISPER + bGroup, pPlayer, pPlayer->m_strGuildName, strTalk);
		m_pUdpSocket->LogChat(LOGMAP_CHATWISPER + bGroup, pPlayer, NULL, strTalk);

#endif

	return EC_NOERROR;
}


DWORD CTRelaySvrModule::OnCS_CORPSENEMYLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer  = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	if(pPlayer->m_wPartyID )
		BroadcastCorps(pPlayer, &pBUF->m_packet, CS_CORPSENEMYLIST_ACK);

	return EC_NOERROR;

}

DWORD CTRelaySvrModule::OnCS_MOVECORPSENEMY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer  = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	if(pPlayer->m_wPartyID)
		BroadcastCorps(pPlayer, &pBUF->m_packet, CS_MOVECORPSENEMY_ACK);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_MOVECORPSUNIT_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer  = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	if(pPlayer->m_wPartyID)
		BroadcastCorps(pPlayer, &pBUF->m_packet, CS_MOVECORPSUNIT_ACK);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_ADDCORPSENEMY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer  = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	if(pPlayer->m_wPartyID)
		BroadcastCorps(pPlayer, &pBUF->m_packet, CS_ADDCORPSENEMY_ACK);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_DELCORPSENEMY_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer  = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	if(pPlayer->m_wPartyID)
		BroadcastCorps(pPlayer, &pBUF->m_packet, CS_DELCORPSENEMY_ACK);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_CORPSHP_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer  = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	if(pPlayer->m_wPartyID)
		BroadcastCorps(pPlayer, &pBUF->m_packet, CS_CORPSHP_ACK);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_CORPSCMD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized)
		return EC_NOERROR;

	WORD wSquadID;
	DWORD dwCharID;
	WORD wMapID;
	BYTE bCMD;
	DWORD dwTargetID;
	BYTE bTargetType;
	WORD wPosX;
	WORD wPosZ;

	pBUF->m_packet
		>> wSquadID
		>> dwCharID
		>> wMapID
		>> bCMD
		>> dwTargetID
		>> bTargetType
		>> wPosX
		>> wPosZ;

	CTPlayer* pChar = FindPlayer(dwCharID);	
	if(!pChar)
		return EC_NOERROR;

	CTParty* pParty = FindParty(pChar->m_wPartyID);
	if( pParty && pParty->IsChief(dwCharID) )	
		pParty->SetCommand(wSquadID,dwCharID,wMapID,bCMD,bTargetType,dwTargetID,wPosX,wPosZ);

	CTCorps* pCorps = FindCorps(pPlayer);
	if(pCorps)
	{
		MAPTPARTY::iterator itR;
		for( itR = pCorps->m_mapTParty.begin(); itR != pCorps->m_mapTParty.end(); itR++)
		{
			CTParty* pParty = (*itR).second;
			if(pParty)
				TransferCorpsCommand(pParty,wSquadID,dwCharID,wMapID,bCMD,dwTargetID,bTargetType,wPosX,wPosZ);
		}
	}
	else
	{
		CTParty* pParty = FindParty(pPlayer->m_wPartyID);
		if(pParty)
            TransferCorpsCommand(pParty,wSquadID,dwCharID,wMapID,bCMD,dwTargetID,bTargetType,wPosX,wPosZ);
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_RELAYPROTECTEDLIST_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	WORD wCount;
	CString strName;
	BYTE bOption;

	pBUF->m_packet
		>> wCount;

	for(DWORD i = 0; i < wCount; i++)
	{
		pBUF->m_packet
			>> strName
			>> bOption;

		pPlayer->AddProtected(strName, bOption);
	}

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_RELAYPROTECTEDADD_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	CString strName;
	BYTE bOption;

	pBUF->m_packet
		>> strName
		>> bOption;

	pPlayer->AddProtected(strName, bOption);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_RELAYPROTECTEDDEL_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	CString strName;

	pBUF->m_packet
		>> strName;

	pPlayer->DelProtected(strName);

	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_CHECKRELAY_REQ(LPPACKETBUF pBUF)
{
	return EC_NOERROR;
}

DWORD CTRelaySvrModule::OnCS_RELAYPROTECTEDOPTION_REQ(LPPACKETBUF pBUF)
{
	CTPlayer* pPlayer = (CTPlayer*)pBUF->m_pSESSION;
	if( !pPlayer || !pPlayer->m_bAuthorized )
		return EC_NOERROR;

	CString strName;
	BYTE bOption;

	pBUF->m_packet
		>> strName
		>> bOption;

	MAPTSTRBYTE::iterator it = pPlayer->m_mapTPROTECTED.find(strName);
	if(it==pPlayer->m_mapTPROTECTED.end())
		return EC_NOERROR;

	(*it).second = bOption;

	return EC_NOERROR;
}

#ifdef DEF_UDPLOG

DWORD CTRelaySvrModule::OnDM_UDPLOG_REQ( LPPACKETBUF pBUF)
{
	_LPUDPPACKET pUDPPacket = new _UDPPACKET;
	memset( pUDPPacket, 0x00, sizeof(_UDPPACKET));

	pBUF->m_packet.Read( &pUDPPacket->szPacket, sizeof(_LOG_DATA_));
				
	m_pUdpSocket->Log(pUDPPacket);

	return EC_NOERROR;
}

#endif	//	DEF_UDPLOG