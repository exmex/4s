#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"


void CTMapSvrModule::SendSM_AICMD_REQ( DWORD dwAICmdHandle,
									   DWORD dwHostKEY,
									   DWORD dwEventHost,
									   DWORD dwRHId,
									   BYTE bRHType,
									   DWORD dwDelay,
									   DWORD dwMonID,
									   BYTE bChannel,
									   WORD wMapID,
									   WORD wPartyID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_AICMD_REQ)
		<< dwAICmdHandle
		<< dwHostKEY
		<< dwEventHost
		<< dwRHId
		<< bRHType
		<< dwDelay
		<< dwMonID
		<< bChannel
		<< wMapID
		<< wPartyID;

	SayToAI(pBUF);
}

void CTMapSvrModule::SendSM_AICMD_ACK( DWORD dwAICmdHandle,
									   DWORD dwHostKEY,
									   DWORD dwEventHost,
									   DWORD dwRHId,
									   BYTE bRHType,
									   DWORD dwMonID,
									   BYTE bChannel,
									   WORD wMapID,
									   WORD wPartyID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_AICMD_ACK)
		<< dwAICmdHandle
		<< dwHostKEY
		<< dwEventHost
		<< dwRHId
		<< bRHType
		<< dwMonID
		<< bChannel
		<< wMapID
		<< wPartyID;

	SayToBATCH(pBUF);
}

void CTMapSvrModule::SendSM_SWITCHSTART_REQ( BYTE bChannel,
											 WORD wPartyID,
											 WORD wMapID,
											 DWORD dwID,
											 DWORD dwDuration)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_SWITCHSTART_REQ)
		<< bChannel
		<< wPartyID
		<< wMapID
		<< dwID
		<< dwDuration;

	SayToAI(pBUF);
}

void CTMapSvrModule::SendSM_POSTBILL_REQ(DWORD dwCharID,
										 DWORD dwPostID,
										 __int64 timeDel)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_POSTBILL_REQ)
		<< dwCharID
		<< dwPostID
		<< timeDel;

	SayToAI(pBUF);
}

void CTMapSvrModule::SendSM_POSTBILLERASE_REQ(DWORD dwCharID,
                                              DWORD dwPostID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_POSTBILLERASE_REQ)
		<< dwCharID
		<< dwPostID;

	SayToAI(pBUF);
}

void CTMapSvrModule::SendSM_DUELSTART_REQ(DWORD dwDuelID, 
										  DWORD dwInviter, 
										  CString strInviter,
										  DWORD dwTarget,
										  CString strTarget)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_DUELSTART_REQ)
		<< dwDuelID
		<< dwInviter
		<< strInviter
		<< dwTarget
		<< strTarget;

	SayToAI(pBUF);
}
void CTMapSvrModule::SendSM_DUELEND_REQ(DWORD dwDuelID,
										DWORD dwLoser)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_DUELEND_REQ)
		<< dwDuelID
		<< dwLoser;

	SayToAI(pBUF);
}

void CTMapSvrModule::SendSM_RESETHOST_REQ(BYTE bChannel,
										  WORD wMapID,
										  WORD wPartyID,
										  DWORD dwID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_RESETHOST_REQ)
		<< bChannel
		<< wMapID
		<< wPartyID
		<< dwID;

	SayToAI(pBUF);
}

void CTMapSvrModule::SendSM_GODBALLCMD_REQ(WORD wGodBall,
										   BYTE bCmd,
										   BYTE bChannel,
										   WORD wMapID,
										   WORD wCastle,
										   DWORD dwDelay,
										   DWORD dwKey,
										   CString strCharName)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_GODBALLCMD_REQ)
		<< wGodBall
		<< bCmd
		<< bChannel
		<< wMapID
		<< wCastle
		<< dwDelay
		<< dwKey
		<< strCharName;

	SayToAI(pBUF);
}

void CTMapSvrModule::SendSM_GODBALLCMD_ACK(WORD wGodBall,
										   BYTE bCmd,
										   DWORD dwKey,
										   BYTE bChannel,
										   WORD wMapID,
										   WORD wCastle,
										   CString strCharName)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_GODBALLCMD_ACK)
		<< wGodBall
		<< bCmd
		<< dwKey
		<< bChannel
		<< wMapID
		<< wCastle
		<< strCharName;

	SayToBATCH(pBUF);
}


void CTMapSvrModule::SendSM_AUCTIONCMD_REQ(WORD wNpcID,
											DWORD dwAuctionID,
											__time64_t dEndTime)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_AUCTIONCMD_REQ)
		<< wNpcID
		<< dwAuctionID
		<< dEndTime;

	SayToLOG(pBUF);
}

void CTMapSvrModule::SendMW_CONNECT_ACK()
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CONNECT_ACK)
		<< MAKEWORD( m_bServerID, BYTE(SVRGRP_MAPSVR))
		<< BYTE(m_mapTLOGCHANNEL.size());

	MAPTCHANNEL::iterator itCH;
	for( itCH = m_mapTLOGCHANNEL.begin(); itCH != m_mapTLOGCHANNEL.end(); itCH++)
		(*pMSG) << BYTE((*itCH).first);

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_CLOSECHAR_ACK( DWORD dwCharID,
										   DWORD dwKEY)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CLOSECHAR_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_ADDCHAR_ACK( DWORD dwCharID,
										 DWORD dwKEY,
										 DWORD dwIPAddr,
										 WORD wPort,
										 DWORD dwUserID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_ADDCHAR_ACK)
		<< dwCharID
		<< dwKEY
		<< dwIPAddr
		<< wPort
		<< dwUserID;

	m_world.Say(pMSG);
}

 
void CTMapSvrModule::SendMW_HEROSELECT_ACK(WORD wBattleZoneID,
                                           CString strHeroName,
										   __time64_t timeHero)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_HEROSELECT_ACK)
		<< wBattleZoneID		
		<< strHeroName
		<< timeHero;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendDM_ACTEND_REQ(DWORD dwCharID)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_ACTEND_REQ)
		<< dwCharID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_EVENTQUARTER_REQ(CString strName,
											BYTE bDay,
											BYTE bHour,
											BYTE bMinute)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_EVENTQUARTER_REQ)
		<< strName
		<< bDay
		<< bHour
		<< bMinute;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_RECALLMONDEL_REQ(DWORD dwCharID, DWORD dwMonID)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_RECALLMONDEL_REQ)
		<< dwCharID
		<< dwMonID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_CREATERECALLMON_REQ(CTPlayer * pPlayer,
												WORD wMonID,
												DWORD dwATTR,
												WORD wPetID,
												CString strName,
												BYTE bLevel,
												BYTE bClass,
												BYTE bRace,
												BYTE bAction,
												BYTE bStatus,
												BYTE bMode,
												DWORD dwMaxHP,
												DWORD dwMaxMP,
												DWORD dwHP,
												DWORD dwMP,
												BYTE bHit,
												BYTE bSkillLevel,
												FLOAT fPosX,
												FLOAT fPosY,
												FLOAT fPosZ,
												WORD wDir,
												DWORD dwTime,
												VWORD vSkill)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_CREATERECALLMON_REQ)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< wMonID
		<< dwATTR
		<< wPetID
		<< strName
		<< bLevel
		<< bClass
		<< bRace
		<< bAction
		<< bStatus
		<< bMode
		<< dwMaxHP
		<< dwMaxMP
		<< dwHP
		<< dwMP
		<< bHit
		<< bSkillLevel
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wDir
		<< dwTime
		<< BYTE(vSkill.size());

	for(DWORD i=0; i< vSkill.size(); i++)
	{
		pBUF->m_packet
			<< vSkill[i];
	}

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_CLEARCURRENTUSER_REQ(DWORD dwCharID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_CLEARCURRENTUSER_REQ)
		<< dwCharID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_ENTERMAPSVR_REQ( BYTE bDBLoad,
											 DWORD dwUserID,
											 DWORD dwCharID,
											 DWORD dwKEY,
											 DWORD dwIPAddr,
											 WORD wPort,
											 BYTE bChannel)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_ENTERMAPSVR_REQ)
		<< bDBLoad
		<< dwUserID
		<< dwCharID
		<< dwKEY
		<< dwIPAddr
		<< wPort
		<< bChannel;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_TELEPORT_REQ( DWORD dwCharID,
										  DWORD dwKEY,
										  BYTE bChannel,
										  WORD wMapID,
										  FLOAT fPosX,
										  FLOAT fPosY,
										  FLOAT fPosZ,
										  BYTE bTutorialOut)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_TELEPORT_REQ)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< bTutorialOut;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_TELEPORT_ACK( DWORD dwCharID,
										  DWORD dwKEY,
										  BYTE bServerID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_TELEPORT_ACK)
		<< dwCharID
		<< dwKEY
		<< bServerID;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendDM_SMSSEND_REQ(DWORD dwCharID,
										DWORD dwKey,
										BYTE bInven,
										BYTE bItem,
										WORD wItemID,
										BYTE bType,
										DWORD dwGuildID,
										CString strTarget,
										CString strMsg)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_SMSSEND_REQ)
		<< dwCharID
		<< dwKey
		<< bInven
		<< bItem
		<< wItemID
		<< bType
		<< dwGuildID
		<< strTarget
		<< strMsg;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_ENTERMAPSVR_ACK( DWORD dwCharID,
											 DWORD dwKEY,
											 BYTE bResult,
											 BYTE bDBLoad)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_ENTERMAPSVR_ACK)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< bDBLoad;

	SayToBATCH(pBUF);
}

void CTMapSvrModule::SendMW_CHARDATA_ACK( CTPlayer *pPlayer)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CHARDATA_ACK)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< pPlayer->m_bStartAct
		<< pPlayer->m_bLevel
		<< pPlayer->GetMaxHP()
		<< pPlayer->m_dwHP
		<< pPlayer->GetMaxMP()
		<< pPlayer->m_dwMP
		<< pPlayer->m_bCountry
		<< pPlayer->m_bMode;

	WORD wSize = pMSG->GetSize();
	BYTE bCount = 0;
	(*pMSG) << bCount;

	MAPTRECALLMON::iterator it;
	for(it=pPlayer->m_mapRecallMon.begin(); it!=pPlayer->m_mapRecallMon.end(); it++)
	{
		CTRecallMon * pMon = (*it).second;
		DWORD dwTick = pMon->GetLifeTick(m_dwTick);
		if(pMon->m_dwDurationTick && !dwTick)
			continue;

		(*pMSG)
			<< pMon->m_dwID
			<< pMon->m_pMON->m_wID
			<< MAKELONG(pMon->m_pATTR->m_wID, pMon->m_bLevel)
			<< pMon->m_wPetID
			<< pMon->m_strName
			<< pMon->m_bLevel
			<< BYTE(pMon->m_pMON->m_wID ? pMon->m_pMON->m_bClass : pPlayer->m_bClass)
			<< BYTE(pMon->m_pMON->m_wID ? pMon->m_pMON->m_bRace : pPlayer->m_bRace)
			<< pMon->m_bAction
			<< pMon->m_bStatus
			<< pMon->m_bMode
			<< pMon->GetMaxHP()
			<< pMon->GetMaxMP()
			<< pMon->m_dwHP
			<< pMon->m_dwMP
			<< pMon->m_bHit
			<< pMon->m_bAtkSkillLevel
			<< pMon->m_fPosX
			<< pMon->m_fPosY
			<< pMon->m_fPosZ
			<< pMon->m_wDIR
			<< dwTick
			<< BYTE(pMon->m_mapTSKILL.size());

		MAPTSKILL::iterator itS;
		for(itS=pMon->m_mapTSKILL.begin(); itS!=pMon->m_mapTSKILL.end(); itS++)
		{
			(*pMSG)
				<< (*itS).second->m_pTSKILL->m_wID;
		}
		
		bCount++;
	}

	memcpy(pMSG->GetBuffer()+wSize, &bCount, sizeof(bCount));

	(*pMSG)
		<< pPlayer->m_strComment;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_CREATERECALLMON_ACK(DWORD dwCharID,
												DWORD dwKEY,
												DWORD dwMonID,
												WORD wMon,
												DWORD dwATTR,
												WORD wPetID,
												CString strName,
												DWORD dwTime,
                                                BYTE bLevel,
												BYTE bClass,
												BYTE bRace,
												BYTE bAction,
												BYTE bStatus,
												BYTE bMode,
												DWORD dwMaxHP,
												DWORD dwMaxMP,
												DWORD dwHP,
												DWORD dwMP,
												BYTE bHit,
												BYTE bSkillLevel,
												FLOAT fPosX,
                                                FLOAT fPosY,
                                                FLOAT fPosZ,
												WORD wDir,
												VWORD vSkill)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CREATERECALLMON_ACK)
		<< dwCharID
		<< dwKEY
		<< dwMonID
		<< wMon
		<< dwATTR
		<< wPetID
		<< strName
		<< bLevel
		<< bClass
		<< bRace
		<< bAction
		<< bStatus
		<< bMode
		<< dwMaxHP
		<< dwMaxMP
		<< dwHP
		<< dwMP
		<< bHit
		<< bSkillLevel
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wDir
		<< dwTime
		<< BYTE(vSkill.size());

	for(DWORD i=0; i<vSkill.size(); i++)
	{
		(*pMSG)
			<< vSkill[i];
	}

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_RECALLMONDEL_ACK(DWORD dwCharID,
											 DWORD dwKey,
											 DWORD dwMonID)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(MW_RECALLMONDEL_ACK)
		<< dwCharID
		<< dwKey
		<< dwMonID;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_ENTERCHAR_ACK( DWORD dwCharID,
										   DWORD dwKEY)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_ENTERCHAR_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_CHECKMAIN_ACK( DWORD dwCharID,
										   DWORD dwKEY)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CHECKMAIN_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_ENTERSVR_ACK( DWORD dwCharID,
										  DWORD dwKEY,
										  CString strNAME,
										  BYTE bLevel,
										  BYTE bRealSex,
										  BYTE bClass,
										  BYTE bRace,
										  BYTE bSex,
										  BYTE bFace,
										  BYTE bHair,
										  BYTE bHelmetHide,
										  BYTE bCountry,
										  BYTE bAidCountry,
										  DWORD dwRegion,
										  BYTE bChannel,
										  WORD wMapID,
										  FLOAT fPosX,
										  FLOAT fPosY,
										  FLOAT fPosZ,
										  BYTE bLogout,
										  BYTE bSave,
										  BYTE bResult,
										  DWORD dwUserIP)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_ENTERSVR_ACK)
		<< dwCharID
		<< dwKEY
		<< strNAME
		<< bLevel
		<< bRealSex
		<< bClass
		<< bRace
		<< bSex
		<< bFace
		<< bHair
		<< bHelmetHide
		<< bCountry
		<< bAidCountry
		<< dwRegion
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< bLogout
		<< bSave
		<< bResult
		<< dwUserIP;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendDM_SAVECHARPOSITION_REQ(CTPlayer *pPlayer,
                                                 WORD wACKMsg)
{
	WORD wMapID = pPlayer->m_wMapID;
	FLOAT fPosX = pPlayer->m_fPosX;
	FLOAT fPosY = pPlayer->m_fPosY;
	FLOAT fPosZ = pPlayer->m_fPosZ;

	if(pPlayer->m_wArenaID)
	{
		MAPARENA::iterator itAr = m_mapArena.find(pPlayer->m_wArenaID);
		if(itAr != m_mapArena.end())
		{
			wMapID = itAr->second->m_pOutPos->m_wMapID;
			fPosX = itAr->second->m_pOutPos->m_fPosX;
			fPosY = itAr->second->m_pOutPos->m_fPosY;
			fPosZ = itAr->second->m_pOutPos->m_fPosZ;
		}
	}
	else if(IsMeetingRoom(wMapID, TRUE))
	{
		MAPTSPAWNPOS::iterator itSp = m_mapTSPAWNPOS.find(MEETINGROOM_OUTPOS);
		if( itSp != m_mapTSPAWNPOS.end() )
		{
			wMapID = itSp->second->m_wMapID;
			fPosX = itSp->second->m_fPosX;
			fPosY = itSp->second->m_fPosY;
			fPosZ = itSp->second->m_fPosZ;
		}
	}

	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_SAVECHARPOSITION_REQ)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< pPlayer->m_wDIR
		<< wACKMsg;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_SAVECHAR_REQ( CTPlayer *pPlayer,
										  WORD wACKMsg)
{
	pPlayer->m_dwSaveTick = m_dwTick;

	if(pPlayer->ProtectTutorial())
	{
		if(wACKMsg)
		{
			LPPACKETBUF pMSG = new PACKETBUF();

			pMSG->m_packet.SetID(wACKMsg)
				<< pPlayer->m_dwID
				<< pPlayer->m_dwKEY;

			SayToBATCH(pMSG);
		}

		return;
	}

	if(pPlayer->m_mapTINVEN.empty())
	{
		LogEvent("Char Save Error %d",pPlayer->m_dwID);
		return;
	}

	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.ExpandIoBuffer(MAX_PACKET_SIZE);

	MAPTINVEN::iterator itTINVEN;
	MAPTSKILL::iterator itTSKILL;
	MAPTSKILL::iterator itExSkill;
	MAPTCABINET::iterator itCABINET;
	MAPTSTORAGEITEM::iterator itCABINETITEM;
	MAPTMAGIC::iterator itTMAGIC;
	MAPTHOTKEYINVEN::iterator itHOTKEY;
	MAPTDURINGITEM::iterator itDUITEM;

	WORD wMapID = pPlayer->m_wMapID;
	FLOAT fPosX = pPlayer->m_fPosX;
	FLOAT fPosY = pPlayer->m_fPosY;
	FLOAT fPosZ = pPlayer->m_fPosZ;
	DWORD dwLastDestination = pPlayer->m_dwLastDestination;

	if(pPlayer->m_wArenaID)
	{
		MAPARENA::iterator itAr = m_mapArena.find(pPlayer->m_wArenaID);
		if(itAr != m_mapArena.end())
		{
			wMapID = itAr->second->m_pOutPos->m_wMapID;
			fPosX = itAr->second->m_pOutPos->m_fPosX;
			fPosY = itAr->second->m_pOutPos->m_fPosY;
			fPosZ = itAr->second->m_pOutPos->m_fPosZ;
		}
	}
	else if(IsMeetingRoom(wMapID, TRUE))
	{
		MAPTSPAWNPOS::iterator itSp = m_mapTSPAWNPOS.find(MEETINGROOM_OUTPOS);
		if( itSp != m_mapTSPAWNPOS.end() )
		{
			wMapID = itSp->second->m_wMapID;
			fPosX = itSp->second->m_fPosX;
			fPosY = itSp->second->m_fPosY;
			fPosZ = itSp->second->m_fPosZ;
			dwLastDestination = 0;
		}
	}

	pBUF->m_packet.SetID(DM_SAVECHAR_REQ)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< pPlayer->m_bStartAct  
		<< pPlayer->m_bLevel
		<< pPlayer->m_bHelmetHide
		<< pPlayer->m_dwGold
		<< pPlayer->m_dwSilver
		<< pPlayer->m_dwCooper
		<< pPlayer->m_bGuildLeave
		<< pPlayer->m_dwGuildLeaveTime
		<< pPlayer->m_dwEXP
		<< pPlayer->m_dwHP
		<< pPlayer->m_dwMP
		<< pPlayer->m_wSkillPoint
		<< pPlayer->m_dwRegion
		<< wMapID
		<< pPlayer->m_wSpawnID
		<< pPlayer->m_wLastSpawnID
		<< dwLastDestination
		<< pPlayer->m_wTemptedMon
		<< pPlayer->m_aftermath.m_bStep
		<< fPosX
		<< fPosY
		<< fPosZ
		<< pPlayer->m_wDIR
		<< pPlayer->m_dwPcBangTime
		<< pPlayer->m_bPcBangItemCnt
		<< pPlayer->m_dwPvPUseablePoint
		<< pPlayer->m_dwPvPTotalPoint;

	for(BYTE cr=0; cr<TCLASS_COUNT; cr++)
	{
		pBUF->m_packet
			<< pPlayer->m_aPvPRecord[cr][0]
			<< pPlayer->m_aPvPRecord[cr][1];
	}

	pBUF->m_packet
		<< BYTE(pPlayer->m_mapTINVEN.size());

	for( itTINVEN = pPlayer->m_mapTINVEN.begin(); itTINVEN != pPlayer->m_mapTINVEN.end(); itTINVEN++)
	{
		CTInven *pTINVEN = (*itTINVEN).second;
		MAPTITEM::iterator itTITEM;

		pBUF->m_packet
			<< pTINVEN->m_bInvenID
			<< pTINVEN->m_wItemID
			<< pTINVEN->m_dEndTime
			<< pTINVEN->m_bELD
			<< BYTE(pTINVEN->m_mapTITEM.size());

		for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
		{
			CTItem *pTITEM = (*itTITEM).second;
			if(!pTITEM->m_dlID)
				pTITEM->m_dlID = GenItemID();

			pTITEM->WrapPacket(&(pBUF->m_packet));
		}
	}

	pBUF->m_packet
		<< BYTE(pPlayer->m_mapCabinet.size());
	
	for(itCABINET=pPlayer->m_mapCabinet.begin(); itCABINET!=pPlayer->m_mapCabinet.end(); itCABINET++)
	{
		pBUF->m_packet
			<< (*itCABINET).second->m_bCabinetID
			<< (*itCABINET).second->m_bUse
			<< WORD((*itCABINET).second->m_mapCabinetItem.size());

		for(itCABINETITEM=(*itCABINET).second->m_mapCabinetItem.begin(); itCABINETITEM!=(*itCABINET).second->m_mapCabinetItem.end(); itCABINETITEM++)
		{
			CTItem *pTITEM = (*itCABINETITEM).second;
			if(!pTITEM->m_dlID)
				pTITEM->m_dlID = GenItemID();

			pBUF->m_packet
				<< pTITEM->m_dwStItemID;

			pTITEM->WrapPacket(&(pBUF->m_packet));
		}
	}

	pBUF->m_packet << (BYTE)pPlayer->m_mapTSKILL.size();
	for(itTSKILL = pPlayer->m_mapTSKILL.begin(); itTSKILL != pPlayer->m_mapTSKILL.end(); itTSKILL++)
	{
		CTSkill * pSkill = (*itTSKILL).second;
		pBUF->m_packet
			<< pSkill->m_pTSKILL->m_wID
			<< pSkill->m_bLevel
			<< pSkill->GetReuseRemainTick(pPlayer->m_dwSaveTick);
	}

	WORD wSize = pBUF->m_packet.GetSize();
	WORD wCount = 0;
	pBUF->m_packet << wCount;

	VPLAYER vPlayer;
	vPlayer.clear();

	for(int is = 0; is<(BYTE)pPlayer->m_vMaintainSkill.size(); is++)
	{
		if(pPlayer->m_vMaintainSkill[is]->m_pTSKILL->m_wID == TSTORE_SKILL)
			continue;

		if(pPlayer->m_pLocal &&
			(pPlayer->m_pLocal->m_pZone->m_wSkill1 == pPlayer->m_vMaintainSkill[is]->m_pTSKILL->m_wID ||
			 pPlayer->m_pLocal->m_pZone->m_wSkill2 == pPlayer->m_vMaintainSkill[is]->m_pTSKILL->m_wID))
			 continue;

		DWORD dwRemain = pPlayer->m_vMaintainSkill[is]->GetRemainTick(pPlayer->m_dwSaveTick);
		if( !pPlayer->m_vMaintainSkill[is]->m_dwStartTick || 
			dwRemain)
		{
			pBUF->m_packet
				<< pPlayer->m_vMaintainSkill[is]->GetID()
				<< pPlayer->m_vMaintainSkill[is]->m_bLevel
				<< dwRemain
				<< pPlayer->m_vMaintainSkill[is]->m_bAttackType
				<< pPlayer->m_vMaintainSkill[is]->m_dwAttackID
				<< pPlayer->m_vMaintainSkill[is]->m_bHostType
				<< pPlayer->m_vMaintainSkill[is]->m_dwHostID
				<< pPlayer->m_vMaintainSkill[is]->m_bAttackCountry;

			wCount++;
		}
		else if(wACKMsg)
		{
			if(pPlayer->m_pMAP)
			{
				if(vPlayer.empty())
					pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

				for(DWORD i=0; i<vPlayer.size(); i++)
					vPlayer[i]->SendCS_SKILLEND_ACK(pPlayer->m_dwID, OT_PC, pPlayer->m_vMaintainSkill[is]->GetID());
			}
			else
				pPlayer->SendCS_SKILLEND_ACK(pPlayer->m_dwID, OT_PC, pPlayer->m_vMaintainSkill[is]->GetID());
		}
	}
	memcpy(pBUF->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));
	vPlayer.clear();

	wSize = pBUF->m_packet.GetSize();
	wCount = 0;
	pBUF->m_packet << wCount;

	if(pPlayer->m_diPremium.m_bType)
	{
		if(!((pPlayer->m_diPremium.m_bType == DURINGTYPE_DAY && pPlayer->m_diPremium.m_tEndTime <= m_timeCurrent) ||
			(pPlayer->m_diPremium.m_bType == DURINGTYPE_TIME && pPlayer->m_diPremium.m_dwRemainTime == 0)))
		{
			pBUF->m_packet
				<< BYTE(IK_GOLDPREMIUM)
				<< pPlayer->m_diPremium.m_pTITEM->m_wItemID
				<< pPlayer->m_diPremium.m_bType
				<< pPlayer->m_diPremium.m_dwRemainTime
				<< pPlayer->m_diPremium.m_tEndTime;

			wCount++;
		}
	}
	if(pPlayer->m_diExp.m_bType)
	{
		if(!((pPlayer->m_diExp.m_bType == DURINGTYPE_DAY && pPlayer->m_diExp.m_tEndTime <= m_timeCurrent) ||
			(pPlayer->m_diExp.m_bType == DURINGTYPE_TIME && pPlayer->m_diExp.m_dwRemainTime == 0)))
		{
			pBUF->m_packet
				<< BYTE(IK_EXPBONUS)
				<< pPlayer->m_diExp.m_pTITEM->m_wItemID
				<< pPlayer->m_diExp.m_bType
				<< pPlayer->m_diExp.m_dwRemainTime
				<< pPlayer->m_diExp.m_tEndTime;

			wCount++;
		}
	}
	memcpy(pBUF->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));

	wSize = pBUF->m_packet.GetSize();
	wCount = 0;
	pBUF->m_packet << wCount;
	MAPDWORD::iterator itIU;
	WORD wDelayGroupID;
	DWORD dwTick;
	for(itIU=pPlayer->m_mapItemCoolTime.begin(); itIU!=pPlayer->m_mapItemCoolTime.end(); itIU++)
	{
		wDelayGroupID = WORD((*itIU).first);
		dwTick = (*itIU).second;
		if(dwTick > pPlayer->m_dwSaveTick)
		{
			pBUF->m_packet
				<< wDelayGroupID
				<< dwTick - pPlayer->m_dwSaveTick;
			wCount++;
		}
	}
	memcpy(pBUF->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));

	wSize = pBUF->m_packet.GetSize();
	wCount = 0;

	pBUF->m_packet << wCount;
	for(itHOTKEY=pPlayer->m_mapHotkeyInven.begin(); itHOTKEY!=pPlayer->m_mapHotkeyInven.end(); itHOTKEY++)
	{
		LPTHOTKEYINVEN pHotkey = (*itHOTKEY).second;
		if(pPlayer->IsEmptyHotkey(pHotkey))
			pBUF->m_packet << BYTE(1);
		else if( pHotkey->m_bSave & ITEM_SAVE_INSERT)
			pBUF->m_packet << BYTE(2);
		else if( pHotkey->m_bSave & ITEM_SAVE_UPDATE)
			pBUF->m_packet << BYTE(3);
		else
			continue;
		
		pBUF->m_packet << BYTE((*itHOTKEY).first);
		for(BYTE hk=0; hk<MAX_HOTKEY_POS; hk++)
		{
			pBUF->m_packet
				<< pHotkey->m_hotkey[hk].m_bType
				<< pHotkey->m_hotkey[hk].m_wID;
		}

		pHotkey->m_bSave = ITEM_SAVE_LOAD;

		wCount++;
	}
	memcpy(pBUF->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));

	wSize = pBUF->m_packet.GetSize();
	WORD wQuestCount = 0; 
	WORD wTermCount = 0;
	MAPQUEST::iterator itQUEST;

	pBUF->m_packet
		<< wQuestCount;

	for( itQUEST = pPlayer->m_mapQUEST.begin(); itQUEST != pPlayer->m_mapQUEST.end(); itQUEST++)
	{
		DWORD dwTick = 0;

		if( (*itQUEST).second->m_bTriggerCount > (*itQUEST).second->m_bCompleteCount )
		{
			DWORD dwPassTick = pPlayer->m_dwSaveTick - (*itQUEST).second->m_dwBeginTick;

			dwTick = (*itQUEST).second->m_dwTick;
			if( dwTick > dwPassTick )
				dwTick -= dwPassTick;
			else
				dwTick = 0;
		}

		if( (*itQUEST).second->m_bSave == TRUE)  
		{
			pBUF->m_packet
				<< (*itQUEST).second->m_pQUEST->m_dwQuestID
				<< dwTick
				<< (*itQUEST).second->m_bCompleteCount
				<< (*itQUEST).second->m_bTriggerCount;

			wQuestCount += 1;
			wTermCount += INT((*itQUEST).second->m_vTERM.size());			
		}
	}

	memcpy(pBUF->m_packet.GetBuffer() + wSize,&wQuestCount,sizeof(wQuestCount) );	

	pBUF->m_packet
		<< wTermCount;

	for( itQUEST = pPlayer->m_mapQUEST.begin(); itQUEST != pPlayer->m_mapQUEST.end(); itQUEST++)
	{
		if( (*itQUEST).second->m_bSave == TRUE)
		{
			for( int i=0; i<INT((*itQUEST).second->m_vTERM.size()); i++)
			{			
				pBUF->m_packet
					<< (*itQUEST).second->m_pQUEST->m_dwQuestID
					<< (*itQUEST).second->m_vTERM[i]->m_dwTermID
					<< (*itQUEST).second->m_vTERM[i]->m_bTermType
					<< (*itQUEST).second->m_vTERM[i]->m_bCount;
			}

			(*itQUEST).second->m_bSave = FALSE;
		}
	}

	wSize = pBUF->m_packet.GetSize();
	wCount = 0;
	pBUF->m_packet << wCount;

	MAPTRECALLMON::iterator itCallMon;
	for(itCallMon=pPlayer->m_mapRecallMon.begin(); itCallMon !=pPlayer->m_mapRecallMon.end(); itCallMon++)
	{
		CTRecallMon * pMon = (*itCallMon).second;
		DWORD dwTick = pMon->GetLifeTick(pPlayer->m_dwSaveTick);
		if(pMon->m_dwDurationTick && !dwTick)
			continue;

		pBUF->m_packet
			<< pMon->m_dwID
			<< pMon->m_pMON->m_wID
			<< pMon->m_wPetID
			<< pMon->m_pATTR->m_dwAttr
			<< pMon->m_bLevel
			<< pMon->m_dwHP
			<< pMon->m_dwMP
			<< pMon->m_bAtkSkillLevel
			<< WORD(pMon->m_fPosX)
			<< WORD(pMon->m_fPosY)
			<< WORD(pMon->m_fPosZ)
			<< dwTick;

		wCount++;
	}
	memcpy(pBUF->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));

	wSize = pBUF->m_packet.GetSize();
	wCount = 0;
	pBUF->m_packet << wCount;

	for(itCallMon=pPlayer->m_mapRecallMon.begin(); itCallMon !=pPlayer->m_mapRecallMon.end(); itCallMon++)
	{
		CTRecallMon * pMon = (*itCallMon).second;
		DWORD dwTick = pMon->GetLifeTick(pPlayer->m_dwSaveTick);
		if(pMon->m_dwDurationTick && !dwTick)
			continue;

		VPLAYER vPlayer;
		vPlayer.clear();

		for(BYTE is=0; is<pMon->m_vMaintainSkill.size(); is++)
		{
			DWORD dwRemain = pMon->m_vMaintainSkill[is]->GetRemainTick(pPlayer->m_dwSaveTick);
			if( !pMon->m_vMaintainSkill[is]->m_dwStartTick || 
				dwRemain)
			{
				pBUF->m_packet
					<< pMon->m_dwID
					<< pMon->m_vMaintainSkill[is]->GetID()
					<< pMon->m_vMaintainSkill[is]->m_bLevel
					<< dwRemain
					<< pMon->m_vMaintainSkill[is]->m_bAttackType
					<< pMon->m_vMaintainSkill[is]->m_dwAttackID
					<< pMon->m_vMaintainSkill[is]->m_bHostType
					<< pMon->m_vMaintainSkill[is]->m_dwHostID
					<< pMon->m_vMaintainSkill[is]->m_bAttackCountry;

				wCount++;
			}
			else if(wACKMsg && pMon->m_pMAP)
			{
				if(wACKMsg)
				{
					if(vPlayer.empty())
						pPlayer->m_pMAP->GetNeighbor(&vPlayer, pMon->m_fPosX, pMon->m_fPosZ);

					for(DWORD i=0; i<vPlayer.size(); i++)
						vPlayer[i]->SendCS_SKILLEND_ACK(pMon->m_dwID, pMon->m_bType, pMon->m_vMaintainSkill[is]->GetID());
				}
				else
					pPlayer->SendCS_SKILLEND_ACK(pMon->m_dwID, pMon->m_bType, pMon->m_vMaintainSkill[is]->GetID());
			}
		}
		vPlayer.clear();
	}
	memcpy(pBUF->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));

	pBUF->m_packet
		<< BYTE(pPlayer->m_mapTPET.size());

	MAPTPET::iterator itPet;
	for(itPet=pPlayer->m_mapTPET.begin(); itPet!=pPlayer->m_mapTPET.end(); itPet++)
	{
		pBUF->m_packet
			<< WORD((*itPet).second->m_pTPET->m_bPetType)
			<< (*itPet).second->m_strName
			<< (*itPet).second->m_ldwTime;
	}

	pBUF->m_packet << WORD(pPlayer->m_vPvPRecent.size());
	for(WORD pr=0; pr<pPlayer->m_vPvPRecent.size(); pr++)
	{
		pBUF->m_packet
			<< pPlayer->m_vPvPRecent[pr].m_strName
			<< pPlayer->m_vPvPRecent[pr].m_bClass
			<< pPlayer->m_vPvPRecent[pr].m_bLevel
			<< pPlayer->m_vPvPRecent[pr].m_bWin
			<< pPlayer->m_vPvPRecent[pr].m_dwPoint
			<< pPlayer->m_vPvPRecent[pr].m_dTime;
	}

	pBUF->m_packet
		<< WORD(pPlayer->m_vDuelRecordSet.size());

	VTRECORDSET::iterator itD;
	for(itD = pPlayer->m_vDuelRecordSet.begin(); itD != pPlayer->m_vDuelRecordSet.end(); itD++)
	{		
		pBUF->m_packet
			<< (*itD).m_strName
			<< (*itD).m_bClass
			<< (*itD).m_bLevel
			<< (*itD).m_bWin
			<< (*itD).m_dwPoint
			<< (*itD).m_dTime;
	}

	for(WORD di = 0; di < TCLASS_COUNT; di++)
	{
		pBUF->m_packet
			<< pPlayer->m_aDuelScore[di][0]
			<< pPlayer->m_aDuelScore[di][1];
	}

	wSize = pBUF->m_packet.GetSize();
	wCount = 0;
	pBUF->m_packet << wCount;

	MAPTPROTECTED::iterator itPTD;
	for(itPTD=pPlayer->m_mapTPROTECTED.begin(); itPTD != pPlayer->m_mapTPROTECTED.end(); itPTD++)
	{
		if((*itPTD).second->m_bChanged)
		{
			pBUF->m_packet
				<< (*itPTD).second->m_dwCharID
				<< (*itPTD).second->m_bOption;

			wCount++;
		}
	}
	memcpy(pBUF->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));

		
	pBUF->m_packet 
		<< (WORD)pPlayer->m_vAuctionInterest.size();
	VDWORD::iterator itAI;
	for( itAI = pPlayer->m_vAuctionInterest.begin(); itAI != pPlayer->m_vAuctionInterest.end(); itAI++)
	{
		pBUF->m_packet
			<< (*itAI);
	}

	pBUF->m_packet
		<< pPlayer->m_dwMonthPvPoint
		<< pPlayer->m_wMonthWin
		<< pPlayer->m_wMonthLose
		<< pPlayer->m_strMonthSay
		<< pPlayer->m_bCountry;

	pBUF->m_packet
		<< wACKMsg;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_SAVEITEM_REQ(CTPlayer *pPlayer)
{
	if(pPlayer->ProtectTutorial())
		return;

	if(!pPlayer->m_bMain || pPlayer->m_mapTINVEN.empty())
		return;

	LPPACKETBUF pBUF = new PACKETBUF();
	MAPTINVEN::iterator itTINVEN;
	MAPTMAGIC::iterator itTMAGIC;

	pBUF->m_packet.SetID(DM_SAVEITEM_REQ)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< BYTE(pPlayer->m_mapTINVEN.size());

	for( itTINVEN = pPlayer->m_mapTINVEN.begin(); itTINVEN != pPlayer->m_mapTINVEN.end(); itTINVEN++)
	{
		CTInven *pTINVEN = (*itTINVEN).second;
		MAPTITEM::iterator itTITEM;

		pBUF->m_packet
			<< pTINVEN->m_bInvenID
			<< pTINVEN->m_wItemID
			<< pTINVEN->m_dEndTime
			<< pTINVEN->m_bELD
			<< BYTE(pTINVEN->m_mapTITEM.size());

		for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
		{
			CTItem *pTITEM = (*itTITEM).second;
			if(!pTITEM->m_dlID)
				pTITEM->m_dlID = GenItemID();

			pTITEM->WrapPacket(&(pBUF->m_packet));
		}
	}

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_LOGOUT_REQ( CTPlayer *pPlayer,
									    WORD wACKMsg)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_LOGOUT_REQ)
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< pPlayer->m_dwUserID
		<< wACKMsg;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_LOADCHAR_REQ( DWORD dwCharID,
										  DWORD dwKEY,
										  DWORD dwUserID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_LOADCHAR_REQ)
		<< dwCharID
		<< dwKEY
		<< dwUserID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_LOADPOSTMESSAGE_REQ(DWORD dwCharID,
                                                DWORD dwKEY)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_LOADPOSTMESSAGE_REQ)
		<< dwCharID
		<< dwKEY;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_LOGSKILL_REQ(DWORD dwCharID,
                                         DWORD dwGold,
                                         DWORD dwSilver,
                                         DWORD dwCooper,
										 WORD wSkill,
										 BYTE bLevel)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	
	pBUF->m_packet.SetID(DM_LOGSKILL_REQ)
		<< dwCharID
		<< dwGold
		<< dwSilver
		<< dwCooper
		<< wSkill
		<< bLevel;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_POSTRECV_REQ(DWORD dwSendID,
										 DWORD dwRecvID,
										 CString strSender,
                                         CString strTarget,
										 CString strTitle,
										 CString strMessage,
										 BYTE bType,
                                         DWORD dwGold,
                                         DWORD dwSilver,
                                         DWORD dwCooper,
                                         CTItem * pItem)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	
	pBUF->m_packet.SetID(DM_POSTRECV_REQ)
		<< dwSendID
		<< dwRecvID
		<< strSender
		<< strTarget
		<< strTitle
		<< strMessage
		<< bType
		<< dwGold
		<< dwSilver
		<< dwCooper;

	if(pItem)
	{
		pBUF->m_packet
			<< BYTE(1)
			<< pItem->m_bInven;

		pItem->WrapPacket(&(pBUF->m_packet));
	}
	else
	{
		pBUF->m_packet
			<< BYTE(0);
	}

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_POSTRECVFROMSVR_REQ(DWORD dwRecvID,
                                                CString strTarget,
                                                CString strTitle,
                                                CString strMessage,
                                                BYTE bType,
                                                DWORD dwGold,
                                                DWORD dwSilver,
                                                DWORD dwCooper,
                                                LPVTITEM vpItem)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	
	pBUF->m_packet.SetID(DM_POSTRECV_REQ)
		<< DWORD(0)
		<< dwRecvID
		<< GetSvrMsg(NAME_OPERATOR)
		<< strTarget
		<< strTitle
		<< strMessage
		<< bType
		<< dwGold
		<< dwSilver
		<< dwCooper;

	if(vpItem)
	{
		pBUF->m_packet
			<< BYTE(vpItem->size());

		while(!vpItem->empty())
		{
			CTItem * pItem = vpItem->back();

			pBUF->m_packet
				<< pItem->m_bInven;

			pItem->WrapPacket(&(pBUF->m_packet));

			delete pItem;
			vpItem->pop_back();
		}
	}
	else
	{
		pBUF->m_packet
			<< BYTE(0);
	}

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_POSTCANSEND_REQ(DWORD dwSendID,
											CString strSender,
											CString strTarget,
											CString strTitle,
											CString strMessage,
											BYTE bType,
											DWORD dwGold,
											DWORD dwSilver,
											DWORD dwCooper,
											BYTE bInven,
											BYTE bSlot)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	
	pBUF->m_packet.SetID(DM_POSTCANSEND_REQ)
		<< dwSendID
		<< strSender
		<< strTarget
		<< strTitle
		<< strMessage
		<< bType
		<< dwGold
		<< dwSilver
		<< dwCooper
		<< bInven
		<< bSlot;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_POSTDEL_REQ(DWORD dwCharID, DWORD dwKey, DWORD dwPostID)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	
	pBUF->m_packet.SetID(DM_POSTDEL_REQ)
		<< dwCharID
		<< dwKey
		<< dwPostID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_POSTGETITEM_REQ(DWORD dwCharID,
											DWORD dwPostID)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	
	pBUF->m_packet.SetID(DM_POSTGETITEM_REQ)
		<< dwCharID
		<< dwPostID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_POSTRECV_ACK(DWORD dwPostID,
										 CString strSender,
                                         CString strTarget,
										 CString strTitle,
										 BYTE bType)
{
	CPacket *pMSG = new CPacket();
	
	pMSG->SetID(MW_POSTRECV_ACK)
		<< dwPostID
		<< strSender
		<< strTarget
		<< strTitle
		<< bType;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendDM_LOCALOCCUPY_REQ(WORD wLocalID,
											BYTE bType,
											DWORD dwOccupiedGuild,
											DWORD dwOccupiedChar)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_LOCALOCCUPY_REQ)
		<< wLocalID
		<< bType
		<< dwOccupiedGuild
		<< dwOccupiedChar;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_CASTLEOCCUPY_REQ(WORD wCastleID,
											 BYTE bType,
                                             DWORD dwWinGuild,
											 DWORD dwLoseGuild)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_CASTLEOCCUPY_REQ)
		<< wCastleID
		<< bType
		<< dwWinGuild
		<< dwLoseGuild;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_MISSIONOCCUPY_REQ(WORD wLocalID,
                                              BYTE bType,
                                              BYTE bCountry,
                                              DWORD dwOccupiedChar)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_MISSIONOCCUPY_REQ)
		<< wLocalID
		<< bType
		<< bCountry
		<< dwOccupiedChar;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_CONLIST_ACK( DWORD dwCharID,
										 DWORD dwKEY,
										 LPMAPBYTE pSERVER)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CONLIST_ACK)
		<< dwCharID
		<< dwKEY;

	if(pSERVER)
	{
		(*pMSG) << BYTE(pSERVER->size());
		MAPBYTE::iterator it;

		for( it = pSERVER->begin(); it != pSERVER->end(); it++)
			(*pMSG) << (*it).second;
	}
	else
		(*pMSG) << BYTE(0);

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_MAPSVRLIST_ACK( DWORD dwCharID,
										    DWORD dwKEY,
											LPMAPBYTE pSERVER)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_MAPSVRLIST_ACK)
		<< dwCharID
		<< dwKEY;

	if(pSERVER)
	{
		(*pMSG) << BYTE(pSERVER->size());
		MAPBYTE::iterator it;

		for( it = pSERVER->begin(); it != pSERVER->end(); it++)
			(*pMSG) << (*it).second;
	}
	else
		(*pMSG) << BYTE(0);

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_CHECKCONNECT_ACK( DWORD dwCharID,
											  DWORD dwKEY,
											  BYTE bChannel,
											  WORD wMapID,
											  FLOAT fPosX,
											  FLOAT fPosY,
											  FLOAT fPosZ,
											  LPMAPBYTE pSERVER)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CHECKCONNECT_ACK)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	if(pSERVER)
	{
		(*pMSG) << BYTE(pSERVER->size());
		MAPBYTE::iterator it;

		for( it = pSERVER->begin(); it != pSERVER->end(); it++)
			(*pMSG) << (*it).second;
	}
	else
		(*pMSG) << BYTE(0);

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_RELEASEMAIN_ACK(BYTE bDBLoad,
											CTPlayer * pPlayer )
{
	if(pPlayer->m_mapTINVEN.empty())
		return;

	DWORD dwTick = m_dwTick;

	CPacket *pMSG = new CPacket();
	pMSG->ExpandIoBuffer(MAX_PACKET_SIZE);

	pMSG->SetID(MW_RELEASEMAIN_ACK)
		<< bDBLoad
		<< pPlayer->m_dwID
		<< pPlayer->m_dwKEY
		<< (BYTE) CN_SUCCESS
		<< pPlayer->m_strNAME
		<< pPlayer->m_bStartAct
		<< pPlayer->m_bRealSex
		<< pPlayer->m_bClass
		<< pPlayer->m_bLevel
		<< pPlayer->m_bRace
		<< pPlayer->m_bCountry
		<< pPlayer->m_bOriCountry
		<< pPlayer->m_bSex
		<< pPlayer->m_bHair
		<< pPlayer->m_bFace
		<< pPlayer->m_bBody
		<< pPlayer->m_bPants
		<< pPlayer->m_bHand
		<< pPlayer->m_bFoot
		<< pPlayer->m_bHelmetHide
		<< pPlayer->m_dwGold
		<< pPlayer->m_dwSilver
		<< pPlayer->m_dwCooper
		<< pPlayer->m_dwEXP
		<< pPlayer->m_dwHP
		<< pPlayer->m_dwMP
		<< pPlayer->m_wSkillPoint
		<< pPlayer->m_dwRegion
		<< pPlayer->m_bGuildLeave
		<< pPlayer->m_dwGuildLeaveTime
		<< pPlayer->m_wMapID
		<< pPlayer->m_wSpawnID
		<< pPlayer->m_wLastSpawnID
		<< pPlayer->m_dwLastDestination
		<< pPlayer->m_wTemptedMon
		<< pPlayer->m_aftermath.m_bStep
		<< pPlayer->m_fPosX
		<< pPlayer->m_fPosY
		<< pPlayer->m_fPosZ
		<< pPlayer->m_wDIR
		<< (dwTick - pPlayer->m_dwSaveTick)
		<< pPlayer->m_wLocalID
		<< pPlayer->m_bAidCountry
		<< pPlayer->m_dlAidDate
		<< pPlayer->m_bInPcBang
		<< pPlayer->m_dwPcBangTime
		<< pPlayer->m_bPcBangItemCnt
		<< pPlayer->m_bLuckyNumber
		<< pPlayer->m_wPostTotal
		<< pPlayer->m_wPostRead;

	MAPTINVEN::iterator itTINVEN;
	MAPTSKILL::iterator itTSKILL;
	MAPTSKILL::iterator itExSkill;
	MAPTCABINET::iterator itCABINET;
	MAPTSTORAGEITEM::iterator itCABINETITEM;
	MAPTMAGIC::iterator itTMAGIC;
	MAPTHOTKEYINVEN::iterator itHOTKEY;
	MAPTDURINGITEM::iterator itDUITEM;
	MAPQUEST::iterator itQUEST;

	(*pMSG)
		<< WORD(pPlayer->m_mapTINVEN.size());
	for( itTINVEN = pPlayer->m_mapTINVEN.begin(); itTINVEN != pPlayer->m_mapTINVEN.end(); itTINVEN++)
	{
		CTInven *pTINVEN = (*itTINVEN).second;
		MAPTITEM::iterator itTITEM;

		(*pMSG)
			<< pTINVEN->m_bInvenID
			<< pTINVEN->m_wItemID
			<< pTINVEN->m_dEndTime
			<< pTINVEN->m_bELD;
	}

	(*pMSG)
		<< WORD(pPlayer->m_mapCabinet.size());
	for(itCABINET=pPlayer->m_mapCabinet.begin(); itCABINET!=pPlayer->m_mapCabinet.end(); itCABINET++)
	{
		(*pMSG)
			<< (*itCABINET).second->m_bCabinetID
			<< (*itCABINET).second->m_bUse;
	}

	WORD wSize = pMSG->GetSize();
	WORD wCount = 0;
	(*pMSG)
		<< wCount;

	for( itTINVEN = pPlayer->m_mapTINVEN.begin(); itTINVEN != pPlayer->m_mapTINVEN.end(); itTINVEN++)
	{
		CTInven *pTINVEN = (*itTINVEN).second;
		MAPTITEM::iterator itTITEM;

		for( itTITEM = pTINVEN->m_mapTITEM.begin(); itTITEM != pTINVEN->m_mapTITEM.end(); itTITEM++)
		{
			CTItem *pTITEM = (*itTITEM).second;
			if(!pTITEM->m_dlID)
				pTITEM->m_dlID = GenItemID();

			(*pMSG)
				<< BYTE(STORAGE_INVEN)
				<< DWORD(pTINVEN->m_bInvenID)
				<< BYTE(TOWNER_CHAR)
				<< pPlayer->m_dwID;

			pTITEM->WrapPacket(pMSG);
			wCount++;
		}
	}
	for(itCABINET=pPlayer->m_mapCabinet.begin(); itCABINET!=pPlayer->m_mapCabinet.end(); itCABINET++)
	{
		for(itCABINETITEM=(*itCABINET).second->m_mapCabinetItem.begin(); itCABINETITEM!=(*itCABINET).second->m_mapCabinetItem.end(); itCABINETITEM++)
		{
			CTItem *pTITEM = (*itCABINETITEM).second;
			if(!pTITEM->m_dlID)
				pTITEM->m_dlID = GenItemID();

			(*pMSG)
				<< BYTE(STORAGE_CABINET)
				<< pTITEM->m_dwStItemID
				<< BYTE(TOWNER_CHAR)
				<< pPlayer->m_dwID;

			pTITEM->WrapPacket(pMSG);
			wCount++;
		}
	}
	memcpy(pMSG->GetBuffer()+wSize, &wCount, sizeof(wCount));

	(*pMSG)
		<< (WORD)pPlayer->m_mapTSKILL.size();
	for(itTSKILL = pPlayer->m_mapTSKILL.begin(); itTSKILL != pPlayer->m_mapTSKILL.end(); itTSKILL++)
	{
		CTSkill * pSkill = (*itTSKILL).second;
		(*pMSG)
			<< pSkill->m_bLevel
			<< pSkill->m_pTSKILL->m_wID
			<< pSkill->GetReuseRemainTick(dwTick);
	}

	VPLAYER vPlayer;
	vPlayer.clear();

	wSize = pMSG->GetSize();
	wCount = 0;
	(*pMSG)
		<< wCount;

	for(int is = 0; is<(BYTE)pPlayer->m_vMaintainSkill.size(); is++)
	{
		DWORD dwRemain = pPlayer->m_vMaintainSkill[is]->GetRemainTick(dwTick);
		if( !pPlayer->m_vMaintainSkill[is]->m_dwStartTick || 
			dwRemain)
		{
			(*pMSG)
				<< pPlayer->m_vMaintainSkill[is]->m_bLevel
				<< pPlayer->m_vMaintainSkill[is]->GetID()
				<< dwRemain
				<< pPlayer->m_vMaintainSkill[is]->m_bAttackType
				<< pPlayer->m_vMaintainSkill[is]->m_dwAttackID
				<< pPlayer->m_vMaintainSkill[is]->m_bHostType
				<< pPlayer->m_vMaintainSkill[is]->m_dwHostID
				<< pPlayer->m_vMaintainSkill[is]->m_bAttackCountry;

			wCount++;
		}
		else if(pPlayer->m_pMAP)
		{
			if(vPlayer.empty())
				pPlayer->m_pMAP->GetNeighbor(&vPlayer, pPlayer->m_fPosX, pPlayer->m_fPosZ);

			for(DWORD i=0; i<vPlayer.size(); i++)
				vPlayer[i]->SendCS_SKILLEND_ACK(pPlayer->m_dwID, OT_PC, pPlayer->m_vMaintainSkill[is]->GetID());
		}
		else
			pPlayer->SendCS_SKILLEND_ACK(pPlayer->m_dwID, OT_PC, pPlayer->m_vMaintainSkill[is]->GetID());
	}
	vPlayer.clear();
	memcpy(pMSG->GetBuffer()+wSize, &wCount, sizeof(wCount));

	(*pMSG)
		<< WORD(pPlayer->m_mapQUEST.size());
	for( itQUEST = pPlayer->m_mapQUEST.begin(); itQUEST != pPlayer->m_mapQUEST.end(); itQUEST++)
	{
		DWORD dwTick = 0;

		if( (*itQUEST).second->m_bTriggerCount > (*itQUEST).second->m_bCompleteCount )
		{
			DWORD dwPassTick = dwTick - (*itQUEST).second->m_dwBeginTick;

			dwTick = (*itQUEST).second->m_dwTick;
			if( dwTick > dwPassTick )
				dwTick -= dwPassTick;
			else
				dwTick = 0;
		}

		(*pMSG)
			<< (*itQUEST).second->m_pQUEST->m_dwQuestID
			<< dwTick
			<< (*itQUEST).second->m_bCompleteCount
			<< (*itQUEST).second->m_bTriggerCount
			<< (*itQUEST).second->m_bSave;
	}

	wSize = pMSG->GetSize();
	wCount = 0;
	(*pMSG)
		<< wCount;

	for( itQUEST = pPlayer->m_mapQUEST.begin(); itQUEST != pPlayer->m_mapQUEST.end(); itQUEST++)
	{
		for( int i=0; i<INT((*itQUEST).second->m_vTERM.size()); i++)
		{			
			(*pMSG)
				<< (*itQUEST).second->m_pQUEST->m_dwQuestID
				<< (*itQUEST).second->m_vTERM[i]->m_dwTermID
				<< (*itQUEST).second->m_vTERM[i]->m_bTermType
				<< (*itQUEST).second->m_vTERM[i]->m_bCount;

			wCount++;
		}
	}
	memcpy(pMSG->GetBuffer() + wSize,&wCount,sizeof(wCount) );	

	(*pMSG)
		<< WORD(pPlayer->m_mapHotkeyInven.size());

	for(itHOTKEY=pPlayer->m_mapHotkeyInven.begin(); itHOTKEY!=pPlayer->m_mapHotkeyInven.end(); itHOTKEY++)
	{
		LPTHOTKEYINVEN pHotkey = (*itHOTKEY).second;
	
		(*pMSG)
			<< BYTE((*itHOTKEY).first)
			<< pHotkey->m_bSave;

		for(BYTE hk=0; hk<MAX_HOTKEY_POS; hk++)
		{
			(*pMSG)
				<< pHotkey->m_hotkey[hk].m_bType
				<< pHotkey->m_hotkey[hk].m_wID;
		}
	}

	wSize = pMSG->GetSize();
	wCount = 0;
	(*pMSG)
		<< wCount;

	MAPDWORD::iterator itIU;
	WORD wDelayGroupID;
	DWORD dwEndTick;
	for(itIU=pPlayer->m_mapItemCoolTime.begin(); itIU!=pPlayer->m_mapItemCoolTime.end(); itIU++)
	{
		wDelayGroupID = WORD((*itIU).first);
		dwEndTick = (*itIU).second;
		if(dwEndTick > dwTick)
		{
			(*pMSG)
				<< wDelayGroupID
				<< dwEndTick - dwTick;
			wCount++;
		}
	}
	memcpy(pMSG->GetBuffer()+wSize, &wCount, sizeof(wCount));

	(*pMSG)
		<< WORD(pPlayer->m_mapTPET.size());

	MAPTPET::iterator itPet;
	for(itPet=pPlayer->m_mapTPET.begin(); itPet!=pPlayer->m_mapTPET.end(); itPet++)
	{
		(*pMSG)
			<< WORD((*itPet).second->m_pTPET->m_bPetType)
			<< (*itPet).second->m_strName
			<< (*itPet).second->m_ldwTime;
	}

	wSize = pMSG->GetSize();
	wCount = 0;
	(*pMSG) << wCount;

	if(pPlayer->m_diPremium.m_bType)
	{
		if(!((pPlayer->m_diPremium.m_bType == DURINGTYPE_DAY && pPlayer->m_diPremium.m_tEndTime <= m_timeCurrent) ||
			(pPlayer->m_diPremium.m_bType == DURINGTYPE_TIME && pPlayer->m_diPremium.m_dwRemainTime == 0)))
		{
			(*pMSG)
				<< pPlayer->m_diPremium.m_pTITEM->m_wItemID
				<< pPlayer->m_diPremium.m_bType
				<< pPlayer->m_diPremium.m_dwRemainTime
				<< pPlayer->m_diPremium.m_tEndTime;

			wCount++;
		}
	}
	if(pPlayer->m_diExp.m_bType)
	{
		if(!((pPlayer->m_diExp.m_bType == DURINGTYPE_DAY && pPlayer->m_diExp.m_tEndTime <= m_timeCurrent) ||
			(pPlayer->m_diExp.m_bType == DURINGTYPE_TIME && pPlayer->m_diExp.m_dwRemainTime == 0)))
		{
			(*pMSG)
				<< pPlayer->m_diExp.m_pTITEM->m_wItemID
				<< pPlayer->m_diExp.m_bType
				<< pPlayer->m_diExp.m_dwRemainTime
				<< pPlayer->m_diExp.m_tEndTime;

			wCount++;
		}
	}
	memcpy(pMSG->GetBuffer()+wSize, &wCount, sizeof(wCount));

	wSize = pMSG->GetSize();
	wCount = 0;
	(*pMSG) << wCount;

	MAPTRECALLMON::iterator itCallMon;
	for(itCallMon=pPlayer->m_mapRecallMon.begin(); itCallMon !=pPlayer->m_mapRecallMon.end(); itCallMon++)
	{
		CTRecallMon * pMon = (*itCallMon).second;
		DWORD dwLiveTick = pMon->GetLifeTick(dwTick);
		if(pMon->m_dwDurationTick && !dwLiveTick)
			continue;

		(*pMSG)
			<< pMon->m_dwID
			<< pMon->m_pMON->m_wID
			<< pMon->m_wPetID
			<< pMon->m_pATTR->m_dwAttr
			<< pMon->m_bLevel
			<< pMon->m_dwHP
			<< pMon->m_dwMP
			<< pMon->m_bAtkSkillLevel
			<< WORD(pMon->m_fPosX)
			<< WORD(pMon->m_fPosY)
			<< WORD(pMon->m_fPosZ)
			<< dwLiveTick;

		wCount++;
	}
	memcpy(pMSG->GetBuffer()+wSize, &wCount, sizeof(wCount));

	wSize = pMSG->GetSize();
	wCount = 0;
	(*pMSG) << wCount;

	for(itCallMon=pPlayer->m_mapRecallMon.begin(); itCallMon !=pPlayer->m_mapRecallMon.end(); itCallMon++)
	{
		CTRecallMon * pMon = (*itCallMon).second;
		DWORD dwLifeTick = pMon->GetLifeTick(dwTick);
		if(pMon->m_dwDurationTick && !dwLifeTick)
			continue;

		for(BYTE is=0; is<pMon->m_vMaintainSkill.size(); is++)
		{
			DWORD dwRemain = pMon->m_vMaintainSkill[is]->GetRemainTick(dwTick);
			if( !pMon->m_vMaintainSkill[is]->m_dwStartTick || 
				dwRemain)
			{
				(*pMSG)
					<< pMon->m_dwID
					<< pMon->m_vMaintainSkill[is]->m_bLevel
					<< pMon->m_vMaintainSkill[is]->GetID()
					<< dwRemain
					<< pMon->m_vMaintainSkill[is]->m_bAttackType
					<< pMon->m_vMaintainSkill[is]->m_dwAttackID
					<< pMon->m_vMaintainSkill[is]->m_bHostType
					<< pMon->m_vMaintainSkill[is]->m_dwHostID
					<< pMon->m_vMaintainSkill[is]->m_bAttackCountry;

				wCount++;
			}
			else if(pMon->m_pMAP)
			{
				vPlayer.clear();
				pPlayer->m_pMAP->GetNeighbor(&vPlayer, pMon->m_fPosX, pMon->m_fPosZ);

				for(DWORD i=0; i<vPlayer.size(); i++)
					vPlayer[i]->SendCS_SKILLEND_ACK(pMon->m_dwID, pMon->m_bType, pMon->m_vMaintainSkill[is]->GetID());
			}
			else
				pPlayer->SendCS_SKILLEND_ACK(pMon->m_dwID, pMon->m_bType, pMon->m_vMaintainSkill[is]->GetID());
		}
	}
	memcpy(pMSG->GetBuffer()+wSize, &wCount, sizeof(wCount));

	(*pMSG)
		<< WORD(pPlayer->m_mapTPROTECTED.size());

	MAPTPROTECTED::iterator itPTD;
	for(itPTD=pPlayer->m_mapTPROTECTED.begin(); itPTD != pPlayer->m_mapTPROTECTED.end(); itPTD++)
	{
		(*pMSG)
			<< (*itPTD).second->m_dwCharID
			<< (*itPTD).second->m_strName
			<< (*itPTD).second->m_bOption
			<< (*itPTD).second->m_bChanged;
	}

	(*pMSG)
		<< pPlayer->m_dwPvPUseablePoint
		<< pPlayer->m_dwPvPTotalPoint
		<< pPlayer->m_dwPvPRankOrder
		<< pPlayer->m_bPvPRankPercent;

	for(BYTE cr=0; cr<TCLASS_COUNT; cr++)
	{
		(*pMSG)
			<< pPlayer->m_aPvPRecord[cr][0]
			<< pPlayer->m_aPvPRecord[cr][1];
	}

	(*pMSG) << WORD(pPlayer->m_vPvPRecent.size());
	for(WORD pr=0; pr<pPlayer->m_vPvPRecent.size(); pr++)
	{
		(*pMSG)
			<< pPlayer->m_vPvPRecent[pr].m_strName
			<< pPlayer->m_vPvPRecent[pr].m_bClass
			<< pPlayer->m_vPvPRecent[pr].m_bLevel
			<< pPlayer->m_vPvPRecent[pr].m_bWin
			<< pPlayer->m_vPvPRecent[pr].m_dwPoint
			<< pPlayer->m_vPvPRecent[pr].m_dTime;
	}

	(*pMSG)
		<< WORD(pPlayer->m_vDuelRecordSet.size());

	VTRECORDSET::iterator itD;
	for(itD = pPlayer->m_vDuelRecordSet.begin(); itD != pPlayer->m_vDuelRecordSet.end(); itD++)
	{		
		(*pMSG)
			<< (*itD).m_strName
			<< (*itD).m_bClass
			<< (*itD).m_bLevel
			<< (*itD).m_bWin
			<< (*itD).m_dwPoint
			<< (*itD).m_dTime;
	}

	(*pMSG)
		<< WORD(1);

	for(WORD di = 0; di < TCLASS_COUNT; di++)
	{
		(*pMSG)
			<< pPlayer->m_aDuelScore[di][0]
			<< pPlayer->m_aDuelScore[di][1];
	}

	(*pMSG)
		<< WORD(pPlayer->m_vAuctionBid.size());

	VDWORD::iterator itDA;
	for(itDA = pPlayer->m_vAuctionBid.begin(); itDA != pPlayer->m_vAuctionBid.end(); itDA++)
	{
		(*pMSG)
			<< (*itDA);
	}

	(*pMSG)
		<< WORD(pPlayer->m_vAuctionInterest.size());
	for( itDA = pPlayer->m_vAuctionInterest.begin(); itDA != pPlayer->m_vAuctionInterest.end(); itDA++)
	{
		(*pMSG)
			<< (*itDA);
	}

	(*pMSG)
		<< WORD(pPlayer->m_vAuctionReg.size());
	for( itDA = pPlayer->m_vAuctionReg.begin(); itDA != pPlayer->m_vAuctionReg.end(); itDA++)
	{
		(*pMSG)
			<< (*itDA);
	}

	(*pMSG)
		<< pPlayer->m_dwMonthPvPoint
		<< pPlayer->m_wMonthWin
		<< pPlayer->m_wMonthLose
		<< pPlayer->m_dwMonthRankOrder
		<< pPlayer->m_bMonthRankPercent;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_CHGPARTYCHIEF_ACK(DWORD dwChiefID,
											  DWORD dwKEY,
											  DWORD dwTargetID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CHGPARTYCHIEF_ACK)
		<< dwChiefID
		<< dwKEY
		<< dwTargetID;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_CHGPARTYTYPE_ACK(DWORD dwCharID,
											 DWORD dwKEY,
											 BYTE bPartyType)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(MW_CHGPARTYTYPE_ACK)
		<< dwCharID
		<< dwKEY
		<< bPartyType;

	m_world.Say(pMSG);
}

void CTMapSvrModule::SendMW_PARTYADD_ACK( CString strOrigin,
										  CString strTarget,
										  BYTE bObtainType,
										  DWORD dwMaxHP,
										  DWORD dwHP,
										  DWORD dwMaxMP,
										  DWORD dwMP)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_PARTYADD_ACK)
		<< strOrigin
		<< strTarget
		<< bObtainType
		<< dwMaxHP
		<< dwHP
		<< dwMaxMP
		<< dwMP;

	m_world.Say(pMSG);
}
void CTMapSvrModule::SendMW_PARTYJOIN_ACK(CString strOrigin,
										  CString strTarget,
										  BYTE bObtainType,
										  BYTE bResponse,
										  DWORD dwMaxHP,
										  DWORD dwHP,
										  DWORD dwMaxMP,
										  DWORD dwMP)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_PARTYJOIN_ACK)
		<< strOrigin
		<< strTarget
		<< bObtainType
		<< bResponse
		<< dwMaxHP
		<< dwHP
		<< dwMaxMP
		<< dwMP;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_PARTYDEL_ACK(WORD wPartyID,
										 DWORD dwTarget,
										 BYTE bKick)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_PARTYDEL_ACK)
		<< wPartyID
		<< dwTarget
		<< bKick;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_PARTYMANSTAT_ACK(WORD wPartyID,
											 DWORD dwID,
											 BYTE bType,
											 BYTE bLevel,
											 DWORD dwMaxHP,
											 DWORD dwCurHP,
											 DWORD dwMaxMP,
											 DWORD dwCurMP)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_PARTYMANSTAT_ACK)
		<< wPartyID
		<< dwID
		<< bType
		<< bLevel
		<< dwMaxHP
		<< dwCurHP
		<< dwMaxMP
		<< dwCurMP;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_LEVELUP_ACK(DWORD dwCharID,
										DWORD dwKey,
										BYTE bLevel)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(MW_LEVELUP_ACK)
		<< dwCharID
		<< dwKey
		<< bLevel;

	m_world.Say(pMSG);
}
////////////////////////////////////////////////////////////////////////////////////////////
// ±æµå
void CTMapSvrModule::SendMW_GUILDESTABLISH_ACK(DWORD dwCharID,
											   DWORD dwKey,
											   CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDESTABLISH_ACK)
		<< dwCharID
		<< dwKey
		<< strName;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDDISORGANIZATION_ACK(DWORD dwCharID,
                                                     DWORD dwKey,
													 BYTE bRet)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDDISORGANIZATION_ACK)
		<< dwCharID
		<< dwKey
		<< bRet;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDINVITE_ACK(DWORD dwCharID,
                                            DWORD dwKey,
											CString strTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDINVITE_ACK)
		<< dwCharID
		<< dwKey
		<< strTarget;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDINVITEANSWER_ACK(DWORD dwCharID,
                                                  DWORD dwKey,
												  BYTE bAnswer,
												  DWORD dwInviter)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDINVITEANSWER_ACK)
		<< dwCharID
		<< dwKey
		<< bAnswer
		<< dwInviter;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDLEAVE_ACK(DWORD dwCharID,
                                           DWORD dwKey)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDLEAVE_ACK)
		<< dwCharID
		<< dwKey;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDDUTY_ACK(DWORD dwCharID,
										  DWORD dwKey,
										  CString strTarget,
										  BYTE bDuty)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDDUTY_ACK)
		<< dwCharID
		<< dwKey
		<< strTarget
		<< bDuty;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDPEER_ACK(DWORD dwCharID,
										  DWORD dwKEY,
										  CString strTarget,
										  BYTE bPeer)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDPEER_ACK)
		<< dwCharID
		<< dwKEY
		<< strTarget
		<< bPeer;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDKICKOUT_ACK(DWORD dwCharID,
                                             DWORD dwKey,
                                             CString strTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDKICKOUT_ACK)
		<< dwCharID
		<< dwKey
		<< strTarget;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDMEMBERLIST_ACK(DWORD dwCharID,
												DWORD dwKey)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDMEMBERLIST_ACK)
		<< dwCharID
		<< dwKey;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDINFO_ACK(DWORD dwCharID,
										  DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDINFO_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_GUILDCABINETLIST_ACK(DWORD dwCharID,
												 DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDCABINETLIST_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendDM_GUILDCABINETPUTIN_REQ(DWORD dwCharID,
												  DWORD dwKEY,
												  DWORD dwGuildID,
												  CTItem * pItem,
												  BYTE bCount)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_GUILDCABINETPUTIN_REQ)
		<< dwCharID
		<< dwKEY
		<< dwGuildID
		<< bCount;

	pItem->WrapPacket(&(pBUF->m_packet));

	SayToDB(pBUF);
}
void CTMapSvrModule::SendDM_GUILDCABINETTAKEOUT_REQ(DWORD dwCharID,
													DWORD dwKEY,
													DWORD dwGuildID,
													DWORD dwItemID,
													BYTE bCount,
													BYTE bInvenID,
													BYTE bItemID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_GUILDCABINETTAKEOUT_REQ)
		<< dwCharID
		<< dwKEY
		<< dwGuildID
		<< dwItemID
		<< bCount
		<< bInvenID
		<< bItemID;

	SayToDB(pBUF);
}
void CTMapSvrModule::SendDM_GUILDCABINETROLLBACK_REQ(DWORD dwGuildID,
													 DWORD dwItemID,
													 CTItem * pItem)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_GUILDCABINETROLLBACK_REQ)
		<< dwGuildID
		<< dwItemID;

	pItem->WrapPacket(&(pBUF->m_packet));
	SayToDB(pBUF);
}
void CTMapSvrModule::SendMW_GUILDCABINETPUTIN_ACK(DWORD dwCharID,
												  DWORD dwKEY,
												  DWORD dwItemID,
												  CTItem * pItem)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDCABINETPUTIN_ACK)
		<< dwCharID
		<< dwKEY
		<< dwItemID;

	pItem->WrapPacket(pMsg);
	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_GUILDCABINETTAKEOUT_ACK(DWORD dwCharID,
													DWORD dwKEY,
													DWORD dwItemID,
													BYTE bCount)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDCABINETTAKEOUT_ACK)
		<< dwCharID
		<< dwKEY
		<< dwItemID
		<< bCount;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDCONTRIBUTION_ACK(DWORD dwCharID,
												  DWORD dwKEY,
												  DWORD dwExp, 
												  DWORD dwGold, 
												  DWORD dwSilver, 
												  DWORD dwCooper,
												  DWORD dwPvPoint)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDCONTRIBUTION_ACK)
		<< dwCharID
		<< dwKEY
		<< dwExp
		<< dwGold
		<< dwSilver
		<< dwCooper
		<< dwPvPoint;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDARTICLELIST_ACK(DWORD dwCharID,
												 DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDARTICLELIST_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDARTICLEADD_ACK(DWORD dwCharID,
												DWORD dwKEY,
												CString strTitle,
												CString strArticle)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDARTICLEADD_ACK)
		<< dwCharID
		<< dwKEY
		<< strTitle
		<< strArticle;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDARTICLEDEL_ACK(DWORD dwCharID,
												DWORD dwKEY,
												DWORD dwID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDARTICLEDEL_ACK)
		<< dwCharID
		<< dwKEY
		<< dwID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDARTICLEUPDATE_ACK(DWORD dwCharID,
												   DWORD dwKEY,
												   DWORD dwID,
												   CString strTitle,
												   CString strArticle)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDARTICLEUPDATE_ACK)
		<< dwCharID
		<< dwKEY
		<< dwID
		<< strTitle
		<< strArticle;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDFAME_ACK(DWORD dwCharID,
										  DWORD dwKEY,
										  DWORD dwFame,
										  DWORD dwFameColor)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDFAME_ACK)
		<< dwCharID
		<< dwKEY
		<< dwFame
		<< dwFameColor;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDWANTEDADD_ACK(DWORD dwCharID,
                                               DWORD dwKEY,
                                               DWORD dwID,
                                               CString strTitle,
                                               CString strText,
                                               BYTE bMinLevel,
                                               BYTE bMaxLevel)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDWANTEDADD_ACK)
		<< dwCharID
		<< dwKEY
		<< dwID
		<< strTitle
		<< strText
		<< bMinLevel
		<< bMaxLevel;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDWANTEDDEL_ACK(DWORD dwCharID,
                                               DWORD dwKEY,
                                               DWORD dwID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDWANTEDDEL_ACK)
		<< dwCharID
		<< dwKEY
		<< dwID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDWANTEDLIST_ACK(DWORD dwCharID,
                                                DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDWANTEDLIST_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDVOLUNTEERING_ACK(DWORD dwCharID,
                                                  DWORD dwKEY,
												  DWORD dwID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDVOLUNTEERING_ACK)
		<< dwCharID
		<< dwKEY
		<< dwID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDVOLUNTEERINGDEL_ACK(DWORD dwCharID,
                                                     DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDVOLUNTEERINGDEL_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDVOLUNTEERLIST_ACK(DWORD dwCharID,
                                                   DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDVOLUNTEERLIST_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDVOLUNTEERREPLY_ACK(DWORD dwCharID,
                                                    DWORD dwKEY,
													DWORD dwTarget,
													BYTE bReply)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDVOLUNTEERREPLY_ACK)
		<< dwCharID
		<< dwKEY
		<< dwTarget
		<< bReply;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSWANTEDADD_ACK(DWORD dwCharID,
                                                      DWORD dwKEY,
                                                      DWORD dwID,
                                                      CString strTitle,
                                                      CString strText,
													  BYTE bDay,
                                                      BYTE bMinLevel,
                                                      BYTE bMaxLevel,
                                                      DWORD dwPoint,
                                                      DWORD dwGold,
                                                      DWORD dwSilver,
                                                      DWORD dwCooper)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSWANTEDADD_ACK)
		<< dwCharID
		<< dwKEY
		<< dwID
		<< strTitle
		<< strText
		<< bDay
		<< bMinLevel
		<< bMaxLevel
		<< dwPoint
		<< dwGold
		<< dwSilver
		<< dwCooper;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSWANTEDDEL_ACK(DWORD dwCharID,
                                                      DWORD dwKEY,
													  DWORD dwID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSWANTEDDEL_ACK)
		<< dwCharID
		<< dwKEY
		<< dwID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSWANTEDLIST_ACK(DWORD dwCharID,
                                                       DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSWANTEDLIST_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSVOLUNTEERING_ACK(DWORD dwCharID,
                                                         DWORD dwKEY,
                                                         DWORD dwGuildID,
														 DWORD dwID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSVOLUNTEERING_ACK)
		<< dwCharID
		<< dwKEY
		<< dwGuildID
		<< dwID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSVOLUNTEERINGDEL_ACK(DWORD dwCharID,
                                                            DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSVOLUNTEERINGDEL_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSVOLUNTEERLIST_ACK(DWORD dwCharID,
                                                          DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSVOLUNTEERLIST_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSREPLY_ACK(DWORD dwCharID,
                                                  DWORD dwKEY,
                                                  DWORD dwTarget,
                                                  BYTE bReply)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSREPLY_ACK)
		<< dwCharID
		<< dwKEY
		<< dwTarget
		<< bReply;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSKICKOUT_ACK(DWORD dwCharID,
                                                    DWORD dwKEY,
                                                    DWORD dwTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSKICKOUT_ACK)
		<< dwCharID
		<< dwKEY
		<< dwTarget;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSINVITE_ACK(DWORD dwCharID,
                                                   DWORD dwKEY,
                                                   CString strName,
                                                   BYTE bDay,
                                                   DWORD dwPoint,
                                                   DWORD dwGold,
                                                   DWORD dwSilver,
                                                   DWORD dwCooper)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSINVITE_ACK)
		<< dwCharID
		<< dwKEY
		<< strName
		<< bDay
		<< dwPoint
		<< dwGold
		<< dwSilver
		<< dwCooper;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSANSWER_ACK(DWORD dwCharID,
                                                   DWORD dwKEY,
												   BYTE bAnswer,
                                                   CString strName,
                                                   BYTE bDay,
                                                   DWORD dwPoint,
                                                   DWORD dwGold,
                                                   DWORD dwSilver,
                                                   DWORD dwCooper)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSANSWER_ACK)
		<< dwCharID
		<< dwKEY
		<< bAnswer
		<< strName
		<< bDay
		<< dwPoint
		<< dwGold
		<< dwSilver
		<< dwCooper;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GUILDTACTICSLIST_ACK(DWORD dwCharID,
                                                 DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GUILDTACTICSLIST_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}

////////////////////////////////////////////////////////////////////////////////////////////
void CTMapSvrModule::SendMW_CHAT_ACK(BYTE bChannel,
									 DWORD dwSender,
									 DWORD dwSenderKEY,
									 CString strSenderName,
									 BYTE bType,
									 BYTE bGroup,
									 DWORD dwTarget,
									 CString strName,
									 CString strTalk)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CHAT_ACK)
		<< bChannel
		<< dwSender
		<< dwSenderKEY
		<< strSenderName
		<< bType
		<< bGroup
		<< dwTarget
		<< strName
		<< strTalk;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendDM_CABINETITEM_REQ(DWORD dwCharID, 
											DWORD dwKey)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_CABINETITEM_REQ)
		<< dwCharID
		<< dwKey;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_TAKEMONMONEY_ACK( DWORD dwCharID,
											  DWORD dwKEY,
											  DWORD dwMoney)
{
	CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_TAKEMONMONEY_ACK)
		<< dwCharID
		<< dwKEY
		<< dwMoney;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_MONSTERDIE_ACK(DWORD dwCharID,
                                           DWORD dwKEY,
										   DWORD dwExp,
                                           DWORD dwMonID,
                                           FLOAT fPosX,
                                           FLOAT fPosY,
                                           FLOAT fPosZ,
                                           WORD wMonTermID,
                                           BYTE bTermType,
										   BYTE bTriggerType,
                                           BYTE bCount)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MONSTERDIE_ACK)
		<< dwCharID
		<< dwKEY
		<< dwExp
		<< dwMonID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wMonTermID
		<< bTermType
		<< bTriggerType
		<< bCount;

	m_world.Say(pMsg);	
}

void CTMapSvrModule::SendMW_ADDITEM_ACK(DWORD dwCharID,
                                        DWORD dwKEY,
										BYTE bServerID,
										BYTE bChannel,
										WORD wMapID,
										DWORD dwMonID,
										BYTE bInvenID,
										BYTE bSlotID,
										CTItem * pItem)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_ADDITEM_ACK)
		<< dwCharID
		<< dwKEY
		<< bServerID
		<< bChannel
		<< wMapID
		<< dwMonID
		<< bInvenID
		<< bSlotID;

	pItem->WrapPacket(pMsg);

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_PARTYORDERTAKEITEM_ACK(DWORD dwCharID,
												   DWORD dwKEY,
												   WORD wPartyID,
												   BYTE bServerID,
												   BYTE bChannel,
												   WORD wMapID,
												   DWORD dwMonID,
												   WORD wTempMonID,  
												   VDWORD vMember,
                                                   CTItem * pItem)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_PARTYORDERTAKEITEM_ACK)
		<< dwCharID
		<< dwKEY
		<< wPartyID
		<< bServerID
		<< bChannel
		<< wMapID
		<< dwMonID
		<< wTempMonID  
		<< BYTE(vMember.size());

	for(BYTE i=0; i<vMember.size(); i++)
	{
		(*pMsg)
			<< vMember[i];
	}

	pItem->WrapPacket(pMsg);

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_PROTECTEDCHECK_ACK(DWORD dwCharID,
											   DWORD dwKey,
											   BYTE bConnect,
											   CString strProtected)
{
    CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_PROTECTEDCHECK_ACK)
		<< dwCharID
		<< dwKey
		<< bConnect
		<< strProtected;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_FRIENDPROTECTEDASK_ACK(DWORD dwCharID, 
												   DWORD dwKey, 
												   CString strInviter)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDPROTECTEDASK_ACK)
		<< dwCharID
		<< dwKey
		<< strInviter;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_FRIENDASK_ACK(DWORD dwCharID,
										  DWORD dwKey,
										  CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDASK_ACK)
		<< dwCharID
		<< dwKey
		<< strName;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_FRIENDREPLY_ACK(DWORD dwCharID,
                                            DWORD dwKey,
                                            CString strName,
											BYTE bReply)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDREPLY_ACK)
		<< dwCharID
		<< dwKey
		<< strName
		<< bReply;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_FRIENDERASE_ACK(DWORD dwCharID,
                                            DWORD dwKey,
											DWORD dwTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDERASE_ACK)
		<< dwCharID
		<< dwKey
		<< dwTarget;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_FRIENDGROUPMAKE_ACK(DWORD dwCharID,
												DWORD dwKey,
												BYTE bGroup,
												CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDGROUPMAKE_ACK)
		<< dwCharID
		<< dwKey
		<< bGroup
		<< strName;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_FRIENDGROUPDELETE_ACK(DWORD dwCharID,
												  DWORD dwKey,
												  BYTE bGroup)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDGROUPDELETE_ACK)
		<< dwCharID
		<< dwKey
		<< bGroup;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_FRIENDGROUPCHANGE_ACK(DWORD dwCharID,
												  DWORD dwKey,
												  DWORD dwFriend,
												  BYTE bGroup)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDGROUPCHANGE_ACK)
		<< dwCharID
		<< dwKey
		<< dwFriend
		<< bGroup;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_FRIENDGROUPNAME_ACK(DWORD dwCharID,
												DWORD dwKey,
												BYTE bGroup,
												CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDGROUPNAME_ACK)
		<< dwCharID
		<< dwKey
		<< bGroup
		<< strName;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendDM_POSTLIST_REQ(DWORD dwCharID, 
										 DWORD dwKey,
										 WORD wPage)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_POSTLIST_REQ)
		<< dwCharID
		<< dwKey
		<< wPage;

	SayToDB(pBUF);
}
void CTMapSvrModule::SendMW_ADDITEMRESULT_ACK(DWORD dwCharID,
											  DWORD dwKEY,
											  BYTE bServerID,
											  BYTE bChannel,
											  WORD wMapID,
											  DWORD dwMonID,
											  BYTE bItemID,
											  BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_ADDITEMRESULT_ACK)
		<< dwCharID
		<< dwKEY
		<< bServerID
		<< bChannel
		<< wMapID
		<< dwMonID
		<< bItemID
		<< bResult;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_ENTERSOLOMAP_ACK( DWORD dwCharID,
											  DWORD dwKEY)
{
	CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_ENTERSOLOMAP_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_LEAVESOLOMAP_ACK( DWORD dwCharID,
											  DWORD dwKEY)
{
	CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_LEAVESOLOMAP_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_CHARSTATINFO_ACK(DWORD dwReqCharID, DWORD dwCharID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CHARSTATINFO_ACK)
		<< dwReqCharID
		<< dwCharID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CHARSTATINFOANS_ACK(DWORD dwReqCharID, CTPlayer * pPlayer)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CHARSTATINFOANS_ACK)
		<< dwReqCharID
		<< pPlayer->m_dwID
		<< (WORD)pPlayer->GetSTR()
		<< (WORD)pPlayer->GetDEX()
		<< (WORD)pPlayer->GetCON()
		<< (WORD)pPlayer->GetINT()
		<< (WORD)pPlayer->GetWIS()
		<< (WORD)pPlayer->GetMEN()
		<< pPlayer->GetMinAP(FALSE)
		<< pPlayer->GetMaxAP(FALSE)
		<< pPlayer->GetDefendPower()
		<< pPlayer->GetMinAP(TRUE)
		<< pPlayer->GetMaxAP(TRUE)
		<< pPlayer->GetAtkSpeed(TAD_PHYSICAL)
		<< pPlayer->GetAtkSpeed(TAD_LONG)
		<< pPlayer->GetAtkSpeed(TAD_MAGIC)
		<< pPlayer->GetAtkSpeedRate(TAD_PHYSICAL)
		<< pPlayer->GetAtkSpeedRate(TAD_LONG)
		<< pPlayer->GetAtkSpeedRate(TAD_MAGIC)
		<< pPlayer->GetAttackLevel()
		<< pPlayer->GetDefendLevel()
		<< pPlayer->GetCriticalPysProb()
		<< pPlayer->GetMinMagicAP()
		<< pPlayer->GetMaxMagicAP()
		<< pPlayer->GetMagicDefPower()
		<< pPlayer->GetMagicAtkLevel()
		<< pPlayer->GetMagicDefLevel()
		<< pPlayer->GetChargeSpeed()
		<< pPlayer->GetChargeProb()
		<< pPlayer->GetCriticalMagicProb()
		<< pPlayer->m_wSkillPoint
		<< pPlayer->m_aftermath.m_bStep;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_LOCALENABLE_ACK(WORD wID, BYTE bStatus)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_LOCALENABLE_ACK)
		<< wID
		<< bStatus;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CASTLEOCCUPY_ACK(BYTE bType,
											 WORD wCastleID,
											 DWORD dwGuildID,
											 BYTE bCountry,
											 DWORD dwLoseGuild)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CASTLEOCCUPY_ACK)
		<< bType
		<< wCastleID
		<< dwGuildID
		<< bCountry
		<< dwLoseGuild;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_LOCALOCCUPY_ACK(BYTE bType,
											WORD wLocalID,
											BYTE bCountry,
											DWORD dwGuildID,
											BYTE bCurCountry)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_LOCALOCCUPY_ACK)
		<< bType
		<< wLocalID
		<< bCountry
		<< dwGuildID
		<< bCurCountry;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_MISSIONOCCUPY_ACK(BYTE bType,
                                              WORD wLocalID,
                                              BYTE bCountry)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MISSIONOCCUPY_ACK)
		<< bType
		<< wLocalID
		<< bCountry;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CHGCORPSCOMMANDER_ACK(DWORD dwCharID,
												  DWORD dwKey,
												  WORD wPartyID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CHGCORPSCOMMANDER_ACK)
		<< dwCharID
		<< dwKey
		<< wPartyID;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_CORPSLEAVE_ACK(DWORD dwCharID,
                                           DWORD dwKey,
										   WORD wSquadID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CORPSLEAVE_ACK)
		<< dwCharID
		<< dwKey
		<< wSquadID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CORPSASK_ACK(DWORD dwCharID,
                                         DWORD dwKey,
                                         CString strTargetName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CORPSASK_ACK)
		<< dwCharID
		<< dwKey
		<< strTargetName;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CORPSREPLY_ACK(DWORD dwCharID,
                                           DWORD dwKey,
                                           BYTE bReply,
                                           CString strReqName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CORPSREPLY_ACK)
		<< dwCharID
		<< dwKey
		<< bReply
		<< strReqName;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CORPSCMD_ACK(DWORD dwGeneral,
                                         DWORD dwKey,
										 WORD wMapID,
                                         WORD wSquadID,
                                         DWORD dwCharID,
                                         BYTE bCMD,
                                         DWORD dwTargetID,
                                         BYTE bTargetType,
                                         WORD wPosX,
                                         WORD wPosZ)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CORPSCMD_ACK)
		<< dwGeneral
		<< dwKey
		<< wMapID
		<< wSquadID
		<< dwCharID
		<< bCMD
		<< dwTargetID
		<< bTargetType
		<< wPosX
		<< wPosZ;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_MOVECORPSENEMY_ACK(DWORD dwCharID,
											   DWORD dwKey,
											   DWORD dwReporterID,
											   DWORD dwEnemyID,
                                               BYTE bEnemyType,
                                               FLOAT fSPEED,
											   WORD wMapID,
                                               WORD wPosX,
                                               WORD wPosZ,
                                               WORD wDIR)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MOVECORPSENEMY_ACK)
		<< dwCharID
		<< dwKey
		<< dwReporterID
		<< dwEnemyID
		<< bEnemyType
		<< fSPEED
		<< wMapID
		<< wPosX
		<< wPosZ
		<< wDIR;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_MOVECORPSUNIT_ACK(DWORD dwCharID,
											  DWORD dwKey,
											  WORD wSquadID,
                                              DWORD dwTargetID,
                                              FLOAT fSPEED,
											  WORD wMapID,
											  DWORD dwBSPID,
                                              WORD wPosX,
                                              WORD wPosZ,
                                              WORD wDIR)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MOVECORPSUNIT_ACK)
		<< dwCharID
		<< dwKey
		<< wSquadID
		<< dwTargetID
		<< fSPEED
		<< wMapID
		<< dwBSPID
		<< wPosX
		<< wPosZ
		<< wDIR;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_ADDCORPSENEMY_ACK(DWORD dwCharID,
											  DWORD dwKey,
											  DWORD dwReporterID,
                                              DWORD dwEnemyID,
                                              BYTE bEnemyType,
                                              FLOAT fSPEED,
											  WORD wMapID,
                                              WORD wPosX,
                                              WORD wPosZ,
                                              WORD wDIR)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_ADDCORPSENEMY_ACK)
		<< dwCharID
		<< dwKey
		<< dwReporterID
		<< dwEnemyID
		<< bEnemyType
		<< fSPEED
		<< wMapID
		<< wPosX
		<< wPosZ
		<< wDIR;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_DELCORPSENEMY_ACK(DWORD dwCharID,
											  DWORD dwKey,
											  DWORD dwReporterID,
                                              DWORD dwEnemyID,
                                              BYTE bEnemyType)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_DELCORPSENEMY_ACK)
		<< dwCharID
		<< dwKey
		<< dwReporterID
		<< dwEnemyID
		<< bEnemyType;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CORPSHP_ACK(DWORD dwCharID,
                                        DWORD dwKey,
										WORD wSquadID,
                                        DWORD dwTargetID,
                                        DWORD dwMaxHP,
                                        DWORD dwHP)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CORPSHP_ACK)
		<< dwCharID
		<< dwKey
		<< wSquadID
		<< dwTargetID
		<< dwMaxHP
		<< dwHP;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_PARTYMOVE_ACK(DWORD dwCharID,
										  DWORD dwKey,
										  CString strTargetName,
										  CString strDestName,
										  WORD wDestParty)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_PARTYMOVE_ACK)
		<< dwCharID
		<< dwKey
		<< strTargetName
		<< strDestName
		<< wDestParty;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_TMSSEND_ACK(DWORD dwCharID,
										DWORD dwKey,
										DWORD dwTMS,
										CString strMessage)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TMSSEND_ACK)
		<< dwCharID
		<< dwKey
		<< dwTMS
		<< strMessage;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_TMSINVITEASK_ACK(DWORD dwCharID,
											 DWORD dwKEY,
											 DWORD dwTargetID,
											 DWORD dwTargetKEY,
											 BYTE bResult,
											 DWORD dwTMS,
											 CString strMessage)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TMSINVITEASK_ACK)
		<< dwCharID
		<< dwKEY
		<< dwTargetID
		<< dwTargetKEY
		<< bResult
		<< dwTMS
		<< strMessage;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_TMSOUT_ACK(DWORD dwCharID, 
									   DWORD dwKey,
									   DWORD dwTMS)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TMSOUT_ACK)
		<< dwCharID
		<< dwKey
		<< dwTMS;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CASTLEENABLE_ACK(WORD wCastle,
											 BYTE bStatus)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CASTLEENABLE_ACK)
		<< wCastle
		<< bStatus;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CASTLEAPPLY_ACK(DWORD dwCharID,
											DWORD dwKey,
											WORD wCastle,
											DWORD dwTarget,
											BYTE bCamp)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CASTLEAPPLY_ACK)
		<< dwCharID
		<< dwKey
		<< wCastle
		<< dwTarget
		<< bCamp;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_MONTEMPT_ACK(DWORD dwAttacker, WORD wMonID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MONTEMPT_ACK)
		<< dwAttacker
		<< wMonID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_GETBLOOD_ACK(DWORD dwAtkID, 
										 BYTE bAtkType,
										 DWORD dwHostID,
										 BYTE bBloodType,
										 DWORD dwBlood)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GETBLOOD_ACK)
		<< dwAtkID
		<< bAtkType
		<< dwHostID
		<< bBloodType
		<< dwBlood;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_DEALITEMERROR_ACK(CString strTarget,
											  CString strErrorChar,
											  BYTE bError)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_DEALITEMERROR_ACK)
		<< strTarget
		<< strErrorChar
		<< bError;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_MAGICMIRROR_ACK(WORD wParty, BYTE bCountry, BYTE bAidCountry, BYTE bClass, CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(MW_MAGICMIRROR_ACK)
		<< wParty
		<< bCountry
		<< bAidCountry
		<< bClass;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_REGION_ACK(DWORD dwCharID, 
									   DWORD dwKEY, 
									   DWORD dwRegion)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_REGION_ACK)
		<< dwCharID
		<< dwKEY
		<< dwRegion;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_ENDWAR_ACK(WORD wCastle)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_ENDWAR_ACK)
		<< wCastle;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendDM_PETDEL_REQ(DWORD dwUserID,
									   WORD wPetID)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_PETDEL_REQ)
		<< dwUserID
		<< wPetID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendCT_USERMOVE_ACK(CString strName,
										 BYTE bChannel,
										 WORD wMapID,
										 FLOAT fPosX,
										 FLOAT fPosY,
										 FLOAT fPosZ,
										 WORD wPartyID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERMOVE_ACK)
		<< strName
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPartyID;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendCT_USERPOSITION_ACK(CString strGMName, CString strTargetName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERPOSITION_ACK)
		<< strTargetName
		<< strGMName;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_BEGINTELEPORT_ACK( DWORD dwCharID,
											   DWORD dwKey,
											   BYTE bSameChannel,
											   BYTE bChannel,
											   WORD wMapID,
											   FLOAT fPosX,
											   FLOAT fPosY,
											   FLOAT fPosZ)
{
    CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_BEGINTELEPORT_ACK)
		<< dwCharID
		<< dwKey
		<< bSameChannel
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_PETRIDING_ACK(DWORD dwCharID,
										  DWORD dwKey,
										  DWORD dwRiding)
{
    CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_PETRIDING_ACK)
		<< dwCharID
		<< dwKey
		<< dwRiding;

	m_world.Say(pMsg);
}

#ifdef	DEF_UDPLOG

void CTMapSvrModule::SendDM_UDPLOG_REQ(_LOG_DATA_ pLogData)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_UDPLOG_REQ);


	pBUF->m_packet.Write((LPVOID)&pLogData, sizeof(_LOG_DATA_) ); 
		
	SayToLOG(pBUF);
}

#endif

void CTMapSvrModule::SendDM_SAVEEXP_REQ(DWORD dwCharID,
										DWORD dwEXP)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_SAVEEXP_REQ)
		<< dwCharID
		<< dwEXP;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_SAVEMONEY_REQ(DWORD dwCharID,
										  DWORD dwGold,
										  DWORD dwSilver,
										  DWORD dwCooper)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_SAVEMONEY_REQ)
		<< dwCharID
		<< dwGold
		<< dwSilver
		<< dwCooper;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_HELMETHIDE_ACK(DWORD dwCharID,
										   DWORD dwKEY,
										   BYTE bHide)
{
	CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_HELMETHIDE_ACK)
		<< dwCharID
		<< dwKEY
		<< bHide;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_PARTYMEMBERRECALL_ACK(DWORD dwCharID,
												  DWORD dwKEY,
												  BYTE bInvenID,
												  BYTE bItemID,
												  CString strOrigin,
												  CString strTarget)
{
	CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_PARTYMEMBERRECALL_ACK)
		<< dwCharID
		<< dwKEY
		<< bInvenID
		<< bItemID
		<< strOrigin
		<< strTarget;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_PARTYMEMBERRECALLANS_ACK(BYTE bResult,
													 CString strUser,
													 CString strTarget,
													 BYTE bType,
                                                     BYTE bInvenID,
                                                     BYTE bItemID,
													 BYTE bChannel,
													 WORD wMapID,
													 FLOAT fPosX,
													 FLOAT fPosY,
													 FLOAT fPosZ)
{
	CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_PARTYMEMBERRECALLANS_ACK)
		<< bResult
		<< strUser
		<< strTarget
		<< bType
		<< bInvenID
		<< bItemID
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendDM_CASHITEMCABINET_REQ(DWORD dwCharID,
												DWORD dwKey,
												DWORD dwUserID)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_CASHITEMCABINET_REQ)
		<< dwCharID
		<< dwKey
		<< dwUserID;

	SayToDB(pBUF);
}
void CTMapSvrModule::SendDM_CASHITEMGET_REQ(DWORD dwCharID,
                                            DWORD dwKey,
											CString strName,
											DWORD dwUserID,
                                            DWORD dwID,
                                            BYTE bInven,
                                            BYTE bSlot)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_CASHITEMGET_REQ)
		<< dwCharID
		<< dwKey
		<< strName
		<< dwUserID
		<< dwID
		<< bInven
		<< bSlot;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_CASHSHOPITEMLIST_REQ(DWORD dwCharID,
                                                 DWORD dwKey,
												 DWORD dwUserID,
												 WORD wItemID)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_CASHSHOPITEMLIST_REQ)
		<< dwCharID
		<< dwKey
		<< dwUserID
		<< wItemID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_CASHITEMBUY_REQ(DWORD dwCharID,
                                            DWORD dwKey,
											DWORD dwUserID,
                                            WORD wCashItemID,
											BYTE bBuyType,
											CString strTarget)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_CASHITEMBUY_REQ)
		<< dwCharID
		<< dwKey
		<< dwUserID
		<< wCashItemID
		<< bBuyType
		<< strTarget;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_SOULMATESEARCH_ACK(DWORD dwCharID,
											   DWORD dwKEY,
											   BYTE bMinLevel,
											   BYTE bNpcInvenID,  
											   BYTE bNpcItemID)
{
	CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_SOULMATESEARCH_ACK)
		<< dwCharID
		<< dwKEY
		<< bMinLevel
		<< bNpcInvenID
		<< bNpcItemID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_SOULMATEREG_ACK(DWORD dwCharID,
											DWORD dwKEY,
											CString strName,
											BYTE bReg,											
											BYTE bNpcInvenID,
											BYTE bNpcItemID)
{
	CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_SOULMATEREG_ACK)
		<< dwCharID
		<< dwKEY
		<< strName
		<< bReg		
		<< bNpcInvenID
		<< bNpcItemID;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_SOULMATEEND_ACK(DWORD dwCharID,
											DWORD dwKEY)
{
	CPacket *pMsg = new CPacket();

	pMsg->SetID(MW_SOULMATEEND_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
void CTMapSvrModule::SendMW_CASTLEWARINFO_ACK(LPTLOCAL pCastle)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_CASTLEWARINFO_ACK);

	if(!pCastle)
	{
		(*pMsg)
			<< WORD(0);
	}
	else
	{
		if(!pCastle->m_bDay)
			return;

		(*pMsg)
			<< pCastle->m_wLocal
			<< pCastle->m_dwGuild
			<< BYTE(pCastle->m_mapLocal.size());

		MAPTLOCAL::iterator it;
		for(it=pCastle->m_mapLocal.begin(); it!=pCastle->m_mapLocal.end(); it++)
		{
			LPTLOCAL pLocal = (*it).second;
			(*pMsg)
				<< pLocal->m_wLocal;

			for(BYTE i=0; i<7; i++)
			{
				if(i+1 != pCastle->m_bDay)
				{
					(*pMsg)
						<< pLocal->m_occupyGuild[i]
						<< pLocal->m_occupyType[i];
				}
			}
		}
	}

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendDM_CASHCABINETBUY_REQ(DWORD dwCharID,
											   DWORD dwKey,
											   WORD wShopItemID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_CASHCABINETBUY_REQ)
		<< dwCharID
		<< dwKey
		<< wShopItemID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_CASHITEMPUTIN_REQ(DWORD dwCharID,
											  DWORD dwKey,
											  CString strName,
											  BYTE bInvenID,
											  BYTE bItemID,
											  CTItem * pItem)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_CASHITEMPUTIN_REQ)
		<< dwCharID
		<< dwKey
		<< strName
		<< bInvenID
		<< bItemID;

	pItem->WrapPacket(&(pBUF->m_packet));

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_DELETEDEALITEM_REQ(CTPlayer * pPlayer)
{
	if(pPlayer->m_dealItem.m_vRecvItem.empty() &&
		pPlayer->m_dealItem.m_vSendItem.empty())
		return;

	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_DELETEDEALITEM_REQ)
		<< BYTE(pPlayer->m_dealItem.m_vSendItem.size() + pPlayer->m_dealItem.m_vRecvItem.size());

	for(BYTE is=0; is<pPlayer->m_dealItem.m_vSendItem.size(); is++)
	{
		pBUF->m_packet
			<< pPlayer->m_dealItem.m_vSendItem[is]->m_dlID;
	}
	for(BYTE ir=0; ir<pPlayer->m_dealItem.m_vRecvItem.size(); ir++)
	{
		pBUF->m_packet
			<< pPlayer->m_dealItem.m_vRecvItem[ir]->m_dlID;
	}

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_RECALLMONDATA_ACK(CTRecallMon * pRecall)
{
	MAPPLAYER::iterator itChar = m_mapPLAYER.find(pRecall->m_dwHostID);
	if(itChar==m_mapPLAYER.end())
		return;

	CPacket * pMsg = new CPacket();

	DWORD dwTick = m_dwTick;

	pMsg->SetID(MW_RECALLMONDATA_ACK)
		<< (*itChar).second->m_dwID
		<< (*itChar).second->m_dwKEY
		<< pRecall->m_dwID
		<< pRecall->m_bChannel
		<< pRecall->m_fPosX
		<< pRecall->m_fPosZ
		<< pRecall->m_bStatus
		<< pRecall->m_bMode
		<< pRecall->m_dwHP
		<< pRecall->m_dwMP
		<< BYTE(pRecall->m_mapTSKILL.size());

	MAPTSKILL::iterator it;
	for(it=pRecall->m_mapTSKILL.begin(); it!=pRecall->m_mapTSKILL.end(); it++)
	{
		CTSkill * pSkill = (*it).second;
		(*pMsg)
			<< pSkill->m_pTSKILL->m_wID
			<< pSkill->GetReuseRemainTick(dwTick);
	}

	WORD wSize = pMsg->GetSize();
	WORD wCount = 0;
	(*pMsg) << wCount;

	for(WORD is=0; is<pRecall->m_vMaintainSkill.size(); is++)
	{
		if( !pRecall->m_vMaintainSkill[is]->m_dwStartTick || 
			pRecall->m_vMaintainSkill[is]->GetRemainTick(dwTick))
		{
			(*pMsg)
				<< pRecall->m_vMaintainSkill[is]->GetID()
				<< pRecall->m_vMaintainSkill[is]->m_bLevel
				<< pRecall->m_vMaintainSkill[is]->GetRemainTick(dwTick)
				<< pRecall->m_vMaintainSkill[is]->m_bAttackType
				<< pRecall->m_vMaintainSkill[is]->m_dwAttackID
				<< pRecall->m_vMaintainSkill[is]->m_bHostType
				<< pRecall->m_vMaintainSkill[is]->m_dwHostID
				<< pRecall->m_vMaintainSkill[is]->m_bAttackCountry;

			wCount++;
		}
	}
	memcpy(pMsg->GetBuffer()+wSize, &wCount, sizeof(wCount));

	pRecall->ClearMaintainSkill();

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendDM_POSTINVENITEM_REQ(DWORD dwID,
											  DWORD dwKey,
											  CString strName,
											  CTInven * pInven)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_POSTINVENITEM_REQ)
		<< dwID
		<< dwKey
		<< strName
		<< pInven->m_bInvenID
        << BYTE(pInven->m_mapTITEM.size());

	MAPTITEM::iterator itTITEM;
	for( itTITEM = pInven->m_mapTITEM.begin(); itTITEM != pInven->m_mapTITEM.end(); itTITEM++)
	{
		CTItem *pTITEM = (*itTITEM).second;
		pTITEM->WrapPacket(&(pBUF->m_packet));
	}

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_SAVECHARBASE_REQ(DWORD dwCharID,
											 BYTE bType,
											 BYTE bValue,
											 CString strName)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_SAVECHARBASE_REQ)
		<< dwCharID
		<< bType
		<< bValue
		<< strName;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_CHECKCHANGENAME_REQ(DWORD dwCharID,
												DWORD dwKey,
												BYTE bInven,
												BYTE bItem,
												CString strName)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_CHECKCHANGENAME_REQ)
		<< dwCharID
		<< dwKey
		<< bInven
		<< bItem
		<< strName;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_CHANGECHARBASE_ACK(DWORD dwCharID,
											   DWORD dwKey,
											   BYTE bType,
											   BYTE bValue,
											   CString strName)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_CHANGECHARBASE_ACK)
		<< dwCharID
		<< dwKey
		<< bType
		<< bValue
		<< strName;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_WARCOUNTRYBALANCE_ACK(DWORD dwCharID,
												  DWORD dwKey)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_WARCOUNTRYBALANCE_ACK)
		<< dwCharID
		<< dwKey;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendDM_STOPTHECLOCK_REQ(DWORD dwCharID,
											 DWORD dwKey,
                                             BYTE bInven,
                                             BYTE bItem,
                                             WORD wShopItemID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_STOPTHECLOCK_REQ)
		<< dwCharID
		<< dwKey
		<< bInven
		<< bItem
		<< wShopItemID;

	SayToDB(pBUF);
}

 
void CTMapSvrModule::SendDM_HEROSELECT_REQ(DWORD dwCharID,
                                           DWORD dwKey,
                                           DWORD dwGuildID,
                                           WORD wBattleZoneID,
                                           CString strHeroName,
										   __time64_t timeHero)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_HEROSELECT_REQ)
		<< dwCharID
		<< dwKey
		<< dwGuildID
		<< wBattleZoneID
		<< strHeroName
		<< timeHero;

	SayToDB(pMSG);
}

void CTMapSvrModule::SendDM_QUESTSENDPOST_REQ(DWORD dwQuestID,
											  DWORD dwCharID,
											  LPVTITEM pvItem)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_QUESTSENDPOST_REQ)
		<< dwQuestID
		<< dwCharID
		<< BYTE(pvItem->size());

	for(DWORD i = 0; i < pvItem->size(); i++)	
		(*pvItem)[i]->WrapPacket(&(pMSG->m_packet));	

	SayToDB(pMSG);
}

void CTMapSvrModule::SendMW_GAINPVPPOINT_ACK(BYTE bOwnerType,
                                             DWORD dwOwnerID,
                                             DWORD dwPoint,
                                             BYTE bEvent,
                                             BYTE bType,
                                             BYTE bGain,
											 CString strName,
											 BYTE bClass,
											 BYTE bLevel)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GAINPVPPOINT_ACK)
		<< bOwnerType
		<< dwOwnerID
		<< dwPoint
		<< bEvent
		<< bType
		<< bGain
		<< strName
		<< bClass
		<< bLevel;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_LOCALRECORD_ACK(CPacket * pMsg)
{
	pMsg->SetID(MW_LOCALRECORD_ACK);

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_GUILDPOINTLOG_ACK(DWORD dwCharID,
											  DWORD dwKey)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDPOINTLOG_ACK)
		<< dwCharID
		<< dwKey;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_GUILDPOINTREWARD_ACK(DWORD dwCharID,
												 DWORD dwKey,
												 CString strTarget,
												 DWORD dwPoint,
												 CString strMessage)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDPOINTREWARD_ACK)
		<< dwCharID
		<< dwKey
		<< strTarget
		<< dwPoint
		<< strMessage;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_GUILDPVPRECORD_ACK(DWORD dwCharID,
											   DWORD dwKey,
											   BYTE bType)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDPVPRECORD_ACK)
		<< dwCharID
		<< dwKey
		<< bType;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_MONSTERBUY_ACK(DWORD dwCharID,
                                           DWORD dwKey,
                                           WORD wNpcID,
										   WORD wID,
										   DWORD dwPrice)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_MONSTERBUY_ACK)
		<< dwCharID
		<< dwKey
		<< wNpcID
		<< wID
		<< dwPrice;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_GUILDMONEYRECOVER_ACK(DWORD dwGuildID,
                                                  DWORD dwPrice)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDMONEYRECOVER_ACK)
		<< dwGuildID
		<< dwPrice;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_FRIENDLIST_ACK(DWORD dwCharID,
                                           DWORD dwKEY)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_FRIENDLIST_ACK)
		<< dwCharID
		<< dwKEY;

	m_world.Say(pMsg);
}
 
void CTMapSvrModule::SendMW_CASHITEMSALE_ACK(DWORD dwIndex,WORD wValue,BYTE bRet)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CASHITEMSALE_ACK)
		<< dwIndex
		<< wValue
		<< bRet;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_APEXDATA_ACK(DWORD dwCharID, DWORD dwKey, char * BufRecv, int nBufLen)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_APEXDATA_ACK)
		<< dwCharID
		<< dwKey
		<< nBufLen;

	pMsg->AttachBinary((LPVOID)BufRecv, nBufLen);

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_APEXSTART_ACK(DWORD dwCharID, DWORD dwKey, long nData)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_APEXSTART_ACK)
		<< dwCharID
		<< dwKey
		<< nData;

	m_world.Say(pMsg);
}


void CTMapSvrModule::SendDM_AUCTIONREG_REQ(DWORD dwSeller,
										   DWORD dwKEY,
										   CString strName,
										   WORD wNpcID,
										   WORD wHour,
										   BYTE bInvenID,										  							  
										   __int64 ldwDirectPrice,
										   __int64 ldwStartPrice,
										   CTItem* pTItem,
										   DWORD dwCode)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_AUCTIONREG_REQ)	
		<< dwSeller
		<< dwKEY
		<< strName
		<< wNpcID
		<< wHour
		<< bInvenID				
		<< ldwDirectPrice
		<< ldwStartPrice
		<< dwCode;

	pTItem->WrapPacket(&(pBUF->m_packet));

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_AUCTIONEND_REQ(WORD wNpcID,
											DWORD dwAuctionID,
											__time64_t dEndTime)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_AUCTIONEND_REQ)
		<< wNpcID
		<< dwAuctionID
		<< dEndTime;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_AUCTIONREGCANCEL_REQ(DWORD dwCharID,
												 DWORD dwKEY,
												 WORD wNpcID,
                                                 DWORD dwAuctionID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_AUCTIONREGCANCEL_REQ)
		<< dwCharID
		<< dwKEY
		<< wNpcID
		<< dwAuctionID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_AUCTIONBID_REQ(DWORD dwCharID,
										   DWORD dwKEY,
										   CString strName,
										   DWORD dwUserID,
										   WORD wNpcID,
										   DWORD dwAuctionID,										  
										   __int64 ldwBidPrice)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_AUCTIONBID_REQ)
		<< dwCharID
		<< dwKEY
		<< strName
		<< dwUserID
		<< wNpcID
		<< dwAuctionID		
		<< ldwBidPrice;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_AUCTIONBUYDIRECT_REQ(DWORD dwCharID,
										   DWORD dwKEY,
										   CString strName,
										   WORD wNpcID,
										   DWORD dwAuctionID,
										   BYTE bCount,
										   __int64 ldwBidPrice)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_AUCTIONBUYDIRECT_REQ)
		<< dwCharID
		<< dwKEY
		<< strName
		<< wNpcID
		<< dwAuctionID	
		<< bCount
		<< ldwBidPrice;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_GAINCASHBONUS_REQ(DWORD dwCharID,
                                              DWORD dwKEY,
											  CString strName,
                                              BYTE bInven,
											  BYTE bItem,
											  WORD wItemID,
											  DWORD dwBonus)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_GAINCASHBONUS_REQ)
		<< dwCharID
		<< dwKEY
		<< strName
		<< bInven
		<< bItem
		<< wItemID
		<< dwBonus;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_WARLORDSAY_REQ(BYTE bType,
										   BYTE bRankMonth,
										   DWORD dwCharID, 
										   CString strSay)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_WARLORDSAY_REQ)
		<< bType
		<< bRankMonth
		<< dwCharID
		<< strSay;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_MONTHRANKUPDATE_ACK(MONTHRANKER& stMonthRanker)
{
	CPacket * pMsg = new CPacket();	
	pMsg->SetID(MW_MONTHRANKUPDATE_ACK)
		<< m_bRankMonth
		<< stMonthRanker.m_bCountry;
	
	stMonthRanker.WrapPacketIn(pMsg);

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_FAMERANKUPDATE_ACK(BYTE bType,
											   BYTE bMonth,
											   BYTE bOrder,
											   MONTHRANKER& stFameRanker)
{
	CPacket * pMsg = new CPacket();	
	pMsg->SetID(MW_FAMERANKUPDATE_ACK)
		<< bType
		<< bMonth
		<< bOrder;

	stFameRanker.WrapPacketIn(pMsg);

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_TERMINATE_ACK(DWORD dwKey)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TERMINATE_ACK)
		<< dwKey;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendDM_POSTVIEW_REQ(DWORD dwCharID, DWORD dwKey, DWORD dwPostID)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_POSTVIEW_REQ)
		<< dwCharID
		<< dwKey
		<< dwPostID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_POSTBILL_REQ(DWORD dwCharID, DWORD dwKey)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_POSTBILL_REQ)
		<< dwCharID
		<< dwKey;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_POSTBILLUPDATE_REQ(DWORD dwCharID,
											   CString strChar,
											   DWORD dwPostID,
											   BYTE bType,
											   DWORD dwGold,
											   DWORD dwSilver,
											   DWORD dwCooper)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_POSTBILLUPDATE_REQ)
		<< dwCharID
		<< strChar
		<< dwPostID
		<< bType
		<< dwGold
		<< dwSilver
		<< dwCooper;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendDM_CMGIFTLOG_REQ(DWORD dwUserID, DWORD dwCharID, WORD wGiftID, DWORD dwGMID, WORD wErrID)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_CMGIFTLOG_REQ)
		<< dwUserID
		<< dwCharID
		<< wGiftID
		<< dwGMID
		<< wErrID;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_WARLORDSAY_ACK(BYTE bType, BYTE bRankMonth,DWORD dwCharID,CString strSay)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_WARLORDSAY_ACK)
		<< bType
		<< bRankMonth
		<< dwCharID		
		<< strSay;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_TOURNAMENT_ACK(DWORD dwCharID,
										   DWORD dwKey,
										   WORD wProtocol,
										   DWORD dwValue,
										   WORD wValue,
										   BYTE bValue,
										   CString strValue)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TOURNAMENT_ACK)
		<< dwCharID
		<< dwKey
		<< wProtocol;

	switch(wProtocol)
	{
	case MW_TOURNAMENTAPPLYINFO_ACK:
	case MW_TOURNAMENTJOINLIST_ACK:
	case MW_TOURNAMENTMATCHLIST_ACK:
	case MW_TOURNAMENTEVENTLIST_ACK:
	case MW_TOURNAMENTSCHEDULE_ACK:
		break;
	case MW_TOURNAMENTAPPLY_ACK:
	case MW_TOURNAMENTEVENTINFO_ACK:
		(*pMsg) << bValue;
		break;
	case MW_TOURNAMENTPARTYLIST_ACK:
		(*pMsg) << dwValue;
		break;
	case MW_TOURNAMENTPARTYADD_ACK:
		(*pMsg) << strValue;
		break;
	case MW_TOURNAMENTEVENTJOIN_ACK:
		(*pMsg)
			<< bValue
			<< dwValue;
		break;
	case MW_TOURNAMENTPARTYDEL_ACK:
		(*pMsg)
			<< dwValue;
		break;
	default:
		return;
	}

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_TOURNAMENTENTERGATE_ACK(DWORD dwCharID,
													DWORD dwKey,
													DWORD dwMoney,
													BYTE bEnter)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TOURNAMENTENTERGATE_ACK)
		<< dwCharID
		<< dwKey
		<< dwMoney
		<< bEnter;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_TOURNAMENTRESULT_ACK(BYTE bStep,
												 BYTE bRet,
												 DWORD dwWinner,
												 DWORD dwLoser,
												 DWORD dwBlueHideTick,
												 DWORD dwRedHideTick)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TOURNAMENTRESULT_ACK)
		<< bStep
		<< bRet
		<< dwWinner
		<< dwLoser
		<< dwBlueHideTick
		<< dwRedHideTick;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendDM_TOURNAMENTREWARD_REQ(BYTE bMonth,
												 WORD wTournamentID,
												 BYTE bEntryID,
												 DWORD dwChiefID,
												 DWORD dwWinner,
												 BYTE bIsEquipShield)
{
	LPPACKETBUF pBUF = new PACKETBUF();
	pBUF->m_packet.SetID(DM_TOURNAMENTREWARD_REQ)
		<< bMonth
		<< wTournamentID
		<< bEntryID
		<< dwChiefID
		<< dwWinner
		<< bIsEquipShield;

	SayToDB(pBUF);
}

void CTMapSvrModule::SendMW_RPSGAME_ACK(DWORD dwCharID,
										DWORD dwKey,
										BYTE bType,
										BYTE bWinCount,
										BYTE bPlayerRPS)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_RPSGAME_ACK)
		<< dwCharID
		<< dwKey
		<< bType
		<< bWinCount
		<< bPlayerRPS;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_MONTHRANKRESETCHAR_ACK(DWORD dwCharID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MONTHRANKRESETCHAR_ACK)
		<< dwCharID;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_MEETINGROOM_ACK(DWORD dwCharID,
											DWORD dwKey,
											BYTE bType,
											BYTE bValue,
											CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MEETINGROOM_ACK)
		<< dwCharID
		<< dwKey
		<< bType
		<< bValue
		<< strName;

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendMW_ARENAJOIN_ACK(DWORD dwCharID,
										  DWORD dwKey,
										  BYTE bJoin,
										  VDWORD &vParty)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_ARENAJOIN_ACK)
		<< dwCharID
		<< dwKey
		<< bJoin
		<< DWORD(vParty.size());

	for(DWORD i=0; i<vParty.size(); i++)
		(*pMsg) << vParty[i];

	m_world.Say(pMsg);
}

void CTMapSvrModule::SendSM_MEETINGROOM_REQ(BYTE bType, DWORD dwCharID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(SM_MEETINGROOM_REQ)
		<< bType
		<< dwCharID
		<< m_dwTick;

	SayToAI(pBUF);
}


void CTMapSvrModule::SendMW_CHATBAN_ACK(CString strTarget, WORD wMin, DWORD dwOperatorID, DWORD dwOperatorKey)
{
	CPacket * pPacket = new CPacket();
	pPacket->SetID(MW_CHATBAN_ACK)
		<< strTarget
		<< wMin
		<< dwOperatorID
		<< dwOperatorKey;
	m_world.Say(pPacket);
}

void CTMapSvrModule::SendMW_CMGIFT_ACK(CString strTarget, WORD wGiftID, DWORD dwGMCharID)
{
	CPacket * pPacket = new CPacket();
	pPacket->SetID(MW_CMGIFT_ACK)
		<< strTarget
		<< wGiftID
		<< dwGMCharID;
	m_world.Say(pPacket);
}

void CTMapSvrModule::SendMW_CMGIFTRESULT_ACK(BYTE bRet, BYTE bTool, DWORD dwGMID)
{
	CPacket * pPacket = new CPacket();
	pPacket->SetID(MW_CMGIFTRESULT_ACK)
		<< bRet
		<< bTool
		<< dwGMID;
	m_world.Say(pPacket);
}
