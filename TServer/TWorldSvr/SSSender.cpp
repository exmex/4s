#include "StdAfx.h"

void CTServer::RelayCorpsMsg(DWORD dwCharID,
							  DWORD dwKey,
							  CPacket * pPacket,
							  WORD wMsgID)
{
	CPacket *pMSG = new CPacket();
	pMSG->Copy(pPacket);
	pMSG->SetID(wMsgID);
	pMSG->Rewind(FALSE);

	(*pMSG)
		<< dwCharID
		<< dwKey;

	Say(pMSG);
}

void CTServer::SendCT_SERVICEMONITOR_REQ(DWORD dwTick,
										 DWORD dwSession,
										 DWORD dwUser,
										 DWORD dwActiveUser)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CT_SERVICEMONITOR_REQ)
		<< dwTick
		<< dwSession
		<< dwUser
		<< dwActiveUser;

	Say(pMSG);
}

void CTServer::SendCT_USERMOVE_ACK( DWORD dwCharID,
								    DWORD dwKEY,
									BYTE bChannel,
									WORD wMapID,
									FLOAT fPosX,
									FLOAT fPosY,
									FLOAT fPosZ,
									WORD wPartyID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CT_USERMOVE_ACK)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< wPartyID;

	Say(pMSG);
}

 
void CTServer::SendCT_CASHITEMSALE_ACK(DWORD dwIndex,
									   WORD wValue)
{
	CPacket *pMSG = new CPacket();
	pMSG->SetID(CT_CASHITEMSALE_ACK)
		<< dwIndex
		<< wValue;

	Say(pMSG);
}

void CTServer::SendCT_RPSGAMEDATA_ACK(BYTE bChange, BYTE bGroup, LPMAPRPSGAME pRPS)
{
	CPacket *pMSG = new CPacket();
	pMSG->SetID(CT_RPSGAMEDATA_ACK)
		<< bChange
		<< bGroup
		<< WORD(pRPS->size());

	MAPRPSGAME::iterator it;
	for(it=pRPS->begin(); it!=pRPS->end(); it++)
	{
		TRPSGAME rps = (*it).second;
		(*pMSG)
			<< rps.m_bType
			<< rps.m_bWinCount
			<< rps.m_bProb[0]
			<< rps.m_bProb[1]
			<< rps.m_bProb[2]
			<< rps.m_wWinKeep
			<< rps.m_wWinPeriod;
	}

	Say(pMSG);
}

void CTServer::SendCT_CMGIFTLIST_ACK(DWORD dwManager, LPMAPCMGIFT pMap)
{
	CPacket *pMSG = new CPacket();
	pMSG->SetID(CT_CMGIFTLIST_ACK)
		<< dwManager
		<< (WORD)pMap->size();
	
	for(MAPCMGIFT::iterator it = pMap->begin(); it != pMap->end(); it++)
	{
		LPCMGIFT pGift = (LPCMGIFT)it->second;
		(*pMSG)
			<< pGift->m_wGiftID		
			<< pGift->m_bGiftType		
			<< pGift->m_dwValue		
			<< pGift->m_bCount			
			<< pGift->m_bTakeType		
			<< pGift->m_bMaxTakeCount	
			<< pGift->m_bToolOnly		
			<< pGift->m_wErrGiftID		
			<< pGift->m_strTitle
			<< pGift->m_strMsg;
	}
	Say(pMSG);
}

void CTServer::SendMW_MONTHRANKRESETCHAR_REQ(DWORD dwCharID)
{
	CPacket *pMSG = new CPacket();
	pMSG->SetID(MW_MONTHRANKRESETCHAR_REQ)
		<< dwCharID;

	Say(pMSG);
}

void CTServer::SendMW_HELPMESSAGE_REQ(BYTE bID,
									  INT64 dlStart,
									  INT64 dlEnd,
									  CString strMessage)
{
	CPacket *pMSG = new CPacket();
	pMSG->SetID(MW_HELPMESSAGE_REQ)
		<< bID
		<< dlStart
		<< dlEnd
		<< strMessage;

	Say(pMSG);
}

void CTServer::SendMW_USERPOSITION_REQ( DWORD dwCharID,
									    DWORD dwKey,
										CString strGMName)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_USERPOSITION_REQ)
		<< dwCharID
		<< dwKey
		<< strGMName;

	Say(pMSG);
}

void CTServer::SendMW_ENTERSVR_REQ( BYTE bDBLoad,
								    DWORD dwCharID,
								    DWORD dwKEY)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_ENTERSVR_REQ)
		<< bDBLoad
		<< dwCharID
		<< dwKEY;

	Say(pMSG);
}

void CTServer::SendMW_ENTERSVR_REQ(LPPACKETBUF pBUF)
{
	CPacket *pMSG = new CPacket();

	pMSG->Copy(&pBUF->m_packet);
	pMSG->SetID(MW_ENTERSVR_REQ);

	Say(pMSG);
}

void CTServer::SendMW_RELEASEMAIN_REQ( DWORD dwCharID,
									   DWORD dwKEY,
									   BYTE bChannel,
									   WORD wMapID,
									   FLOAT fPosX,
									   FLOAT fPosY,
									   FLOAT fPosZ)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_RELEASEMAIN_REQ)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMSG);
}

void CTServer::SendMW_MAPSVRLIST_REQ( DWORD dwCharID,
									  DWORD dwKEY,
									  BYTE bChannel,
									  WORD wMapID,
									  FLOAT fPosX,
									  FLOAT fPosY,
									  FLOAT fPosZ)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_MAPSVRLIST_REQ)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMSG);
}

void CTServer::SendMW_INVALIDCHAR_REQ( DWORD dwCharID,
									   DWORD dwKEY,
									   BYTE bReleaseMain)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_INVALIDCHAR_REQ)
		<< dwCharID
		<< dwKEY
		<< bReleaseMain;

	Say(pMSG);
}

void CTServer::SendMW_CHARDATA_REQ( DWORD dwCharID,
								    DWORD dwKEY)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CHARDATA_REQ)
		<< dwCharID
		<< dwKEY;

	Say(pMSG);
}

void CTServer::SendMW_ENTERCHAR_REQ( DWORD dwCharID,
									 DWORD dwKEY,
									 BYTE bStartAct,
									 CString strNAME,
									 WORD wMapID,
									 FLOAT fPosX,
									 FLOAT fPosY,
									 FLOAT fPosZ,
									 DWORD dwGuildID,
									 DWORD dwFame,
									 DWORD dwFameColor,
									 CString strGuildName,
									 BYTE bGuildDuty,
									 BYTE bGuildPeer,
                                     WORD wCastle,
                                     BYTE bCamp,
									 DWORD dwTacticsID,
									 CString strTacticsName,
									 WORD wPartyID,
									 BYTE bPartyType,
									 DWORD dwPartyChiefID,
									 WORD wCommander,
									 BYTE bLevel,
									 BYTE bHelmetHide,
									 BYTE bCountry,
									 BYTE bAidCountry,
									 BYTE bMode,
									 DWORD dwRiding,
									 __int64 nChatBanTime,
									 DWORD dwSoulmate,
									 DWORD dwSoulSilence,
									 CString strSoulmate,
									 BYTE bClass,
									 CPacket *pPacket)
{
	LPBYTE pOffset = pPacket->m_ptrOffset;
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_ENTERCHAR_REQ)
		<< dwCharID
		<< dwKEY
		<< bStartAct
		<< strNAME
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< dwGuildID
		<< dwFame
		<< dwFameColor
		<< strGuildName
		<< bGuildDuty
		<< bGuildPeer
		<< wCastle
		<< bCamp
		<< dwTacticsID
		<< strTacticsName
		<< wPartyID
		<< bPartyType
		<< dwPartyChiefID
		<< wCommander
		<< bLevel
		<< bHelmetHide
		<< bCountry
		<< bAidCountry
		<< bMode
		<< dwRiding
		<< nChatBanTime
		<< dwSoulmate
		<< dwSoulSilence
		<< strSoulmate
		<< bClass;


	BYTE bRecallCnt;
	(*pPacket)
		>> bRecallCnt;

	(*pMSG)
		<< bRecallCnt;

	for(BYTE bRC = 0; bRC < bRecallCnt; bRC++)
	{
		DWORD dwMonID;
		WORD wMon;
		DWORD dwATTR;
		WORD wPetID;
		CString strName;
		DWORD dwTime;
		BYTE bLevel;
		BYTE bClass;
		BYTE bRace;
		BYTE bAction;
		BYTE bStatus;
		BYTE bMode;
		DWORD dwMaxHP;
		DWORD dwMaxMP;
		DWORD dwHP;
		DWORD dwMP;
		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;
		WORD wDir;
		BYTE bHit;
		BYTE bSkillCount;
		WORD wSkillID;
		BYTE bSkillLevel;

		(*pPacket)
			>> dwMonID
			>> wMon
			>> dwATTR
			>> wPetID
			>> strName
			>> bLevel
			>> bClass
			>> bRace
			>> bAction
			>> bStatus
			>> bMode
			>> dwMaxHP
			>> dwMaxMP
			>> dwHP
			>> dwMP
			>> bHit
			>> bSkillLevel
			>> fPosX
			>> fPosY
			>> fPosZ
			>> wDir
			>> dwTime
			>> bSkillCount;

		(*pMSG)
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
			<< bSkillCount;

		for(BYTE sc=0; sc<bSkillCount; sc++)
		{
			(*pPacket)
				>> wSkillID;

			(*pMSG)
				<< wSkillID;
		}
	}

	CString strComment;
	(*pPacket)
		>> strComment;
	(*pMSG)
		<< strComment;

	pPacket->m_ptrOffset = pOffset;

	Say(pMSG);
}

void CTServer::SendMW_CONRESULT_REQ( DWORD dwCharID,
									 DWORD dwKEY,
									 BYTE bResult,
									 MAPTCHARCON mapTCON)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CONRESULT_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< BYTE(mapTCON.size());

	MAPTCHARCON::iterator it;
	for(it=mapTCON.begin(); it!=mapTCON.end(); it++)
	{
		(*pMSG)
			<< (*it).first;
	}

	Say(pMSG);
}

void CTServer::SendMW_CLOSECHAR_REQ( DWORD dwCharID,
									 DWORD dwKEY)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CLOSECHAR_REQ)
		<< dwCharID
		<< dwKEY;

	Say(pMSG);
}

void CTServer::SendMW_CHECKMAIN_REQ( DWORD dwCharID,
									 DWORD dwKEY,
									 BYTE bChannel,
									 WORD wMapID,
									 FLOAT fPosX,
									 FLOAT fPosY,
									 FLOAT fPosZ)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CHECKMAIN_REQ)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMSG);
}

void CTServer::SendMW_DELCHAR_REQ( DWORD dwCharID,
								   DWORD dwKEY,
								   BYTE bLogout,
								   BYTE bSave)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_DELCHAR_REQ)
		<< dwCharID
		<< dwKEY
		<< bLogout
		<< bSave;

	Say(pMSG);
}

void CTServer::SendMW_CHARINFO_REQ( DWORD dwCharID,
								    DWORD dwKEY,
									CTGuild * pGuild,
									CTGuild * pTactics,
									CTParty *pParty,
									BYTE bDuty,
									BYTE bPeer,
									WORD wCastle,
									BYTE bCamp)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CHARINFO_REQ)
		<< dwCharID
		<< dwKEY;

	if(pGuild)
	{
		(*pMSG)
			<< pGuild->m_dwID
			<< pGuild->m_strName
			<< pGuild->m_dwFame
			<< pGuild->m_dwFameColor;
	}
	else
	{
		(*pMSG)
			<< DWORD(0)
			<< NAME_NULL
			<< DWORD(0)
			<< DWORD(0);
	}

	if(pTactics)
	{
		(*pMSG)
			<< pTactics->m_dwID
			<< pTactics->m_strName;
	}
	else
	{
		(*pMSG)
			<< DWORD(0)
			<< NAME_NULL;
	}

	(*pMSG)
		<< bDuty
		<< bPeer
		<< wCastle
		<< bCamp
		<< WORD(pParty ? pParty->GetID() : 0)
		<< BYTE(pParty ? pParty->m_bObtainType : 0)
		<< DWORD(pParty ? pParty->GetChiefID() : 0);

	Say(pMSG);
}

void CTServer::SendMW_CONLIST_REQ( DWORD dwCharID,
								   DWORD dwKEY,
								   BYTE bChannel,
								   WORD wMapID,
								   FLOAT fPosX,
								   FLOAT fPosY,
								   FLOAT fPosZ)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CONLIST_REQ)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMSG);
}

void CTServer::SendMW_TELEPORT_REQ( DWORD dwCharID,
								    DWORD dwKEY,
									BYTE bChannel,
									WORD wMapID,
									FLOAT fPosX,
									FLOAT fPosY,
									FLOAT fPosZ,
									BYTE bResult)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_TELEPORT_REQ)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ
		<< bResult;

	Say(pMSG);
}

void CTServer::SendMW_ROUTE_REQ( DWORD dwCharID,
								 DWORD dwKEY,
								 BYTE bChannel,
								 WORD wMapID,
								 FLOAT fPosX,
								 FLOAT fPosY,
								 FLOAT fPosZ)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_ROUTE_REQ)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMSG);
}

void CTServer::SendMW_ENTERSOLOMAP_REQ( DWORD dwCharID,
									    DWORD dwKEY,
										WORD wPartyID,
										BYTE bPartyType,
										DWORD dwChiefID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_ENTERSOLOMAP_REQ)
		<< dwCharID
		<< dwKEY
		<< wPartyID
		<< bPartyType
		<< dwChiefID;

	Say(pMSG);
}

void CTServer::SendMW_CHGPARTYTYPE_REQ(DWORD dwCharID,
									   DWORD dwKEY,
									   BYTE bRet,
									   BYTE bPartyType)
{
	CPacket * pMSG = new CPacket();

	pMSG->SetID(MW_CHGPARTYTYPE_REQ)
		<< dwCharID
		<< dwKEY
		<< bRet
		<< bPartyType;

	Say(pMSG);
}

void CTServer::SendMW_PARTYADD_REQ( DWORD dwCharID,
									DWORD dwKEY,
									CString strRequest,
									CString strTarget,
									BYTE bObtainType,
									BYTE bResult,
									DWORD dwRequest)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_PARTYADD_REQ)
		<< dwCharID
		<< dwKEY
		<< strRequest
		<< strTarget
		<< bObtainType
		<< bResult
		<< dwRequest;

	Say(pMSG);
}

void CTServer::SendMW_PARTYJOIN_REQ( DWORD dwID,
									 DWORD dwKey,
									 WORD wPartyID,
									 DWORD dwChief,
									 WORD wCommanderID,
									 LPTCHARACTER pNew,
									 BYTE bObtainType)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_PARTYJOIN_REQ)
		<< dwID
		<< dwKey
		<< wPartyID
		<< pNew->m_strNAME
		<< pNew->m_dwCharID
		<< dwChief
		<< wCommanderID
		<< (pNew->m_pGuild ? pNew->m_pGuild->m_strName : NAME_NULL)
		<< pNew->m_bLevel
		<< pNew->m_dwMaxHP
		<< pNew->m_dwHP
		<< pNew->m_dwMaxMP
		<< pNew->m_dwMP
		<< pNew->m_bRace
		<< pNew->m_bSex
		<< pNew->m_bFace
		<< pNew->m_bHair
		<< bObtainType
		<< pNew->m_bClass;

	Say(pMsg);
}
void CTServer::SendMW_PARTYDEL_REQ( DWORD dwID,
									 DWORD dwKey,
									 DWORD dwTargetID,
									 DWORD dwChief,
									 WORD wCommander,
									 WORD wPartyID,
									 BYTE bKick)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_PARTYDEL_REQ)
		<< dwID
		<< dwKey
		<< dwTargetID
		<< dwChief
		<< wCommander
		<< wPartyID
		<< bKick;

	Say(pMsg);
}
void CTServer::SendMW_PARTYMANSTAT_REQ( DWORD dwCharID,
										DWORD dwKey,
										DWORD dwID,
										BYTE bType,
										BYTE bLevel,
										DWORD dwMaxHP,
										DWORD dwCurHP,
										DWORD dwMaxMP,
										DWORD dwCurMP)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_PARTYMANSTAT_REQ)
		<< dwCharID
		<< dwKey
		<< dwID
		<< bType
		<< bLevel
		<< dwMaxHP
		<< dwCurHP
		<< dwMaxMP
		<< dwCurMP;

	Say(pMsg);
}
void CTServer::SendMW_LEVELUP_REQ(DWORD dwCharID,
								  DWORD dwKey,
								  BYTE bLevel)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_LEVELUP_REQ)
		<< dwCharID
		<< dwKey
		<< bLevel;

	Say(pMsg);
}

void CTServer::SendMW_EVENTMSGLOTTERY_REQ(CString strTitle,
										  vector<LOTPACKET>& pLotPacket)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(MW_EVENTMSGLOTTERY_REQ)
		<< strTitle;

	WORD wItemCount = (WORD)pLotPacket.size();

	*pMSG
		<< wItemCount;

	for( WORD i = 0; i < wItemCount; i++)
	{
		WORD j = 0;
		WORD wUserCount = (WORD)pLotPacket[i].m_pChar.size();

		*pMSG
			<< (pLotPacket[i].m_pLot->m_wItemID)
			<< (pLotPacket[i].m_pLot->m_bNum)
			<< wUserCount;

		for(j = 0; j < wUserCount; j++)
		{
			*pMSG
				<< (pLotPacket[i].m_pChar[j]->m_strNAME);
		}
	}

	Say(pMSG);

}
/////////////////////////////////////////////////////////////////////////////////////////////////
// ±æµå
void CTServer::SendMW_GUILDESTABLISH_REQ(DWORD dwCharID,
										 DWORD dwKey,
										 BYTE bRet,
										 DWORD dwGuildID,
										 CString strName,
										 BYTE bEstablish)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDESTABLISH_REQ)
		<< dwCharID
		<< dwKey
		<< bRet
		<< dwGuildID
		<< strName
		<< bEstablish;

	Say(pMsg);
}

void CTServer::SendMW_GUILDDISORGANIZATION_REQ(DWORD dwCharID,
											   DWORD dwKey,
											   BYTE bDisorg)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDDISORGANIZATION_REQ)
		<< dwCharID
		<< dwKey
		<< bDisorg;

	Say(pMsg);
}
void CTServer::SendMW_GUILDINVITE_REQ(DWORD dwCharID,
									  DWORD dwKey,
									  CString strName,
									  DWORD dwInviter,
									  CString strInviterName)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDINVITE_REQ)
		<< dwCharID
		<< dwKey
		<< strName
		<< dwInviter
		<< strInviterName;

	Say(pMsg);
}

void CTServer::SendMW_GUILDJOIN_REQ(DWORD dwCharID,
									DWORD dwKey,
									BYTE bRet,
									DWORD dwGuildID,
									DWORD dwFame,
									DWORD dwFameColor,
									CString strGuildName,
									DWORD dwMemberID,
									CString strMemberName,
									BYTE bMaxGuildMember)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDJOIN_REQ)
		<< dwCharID
		<< dwKey
		<< bRet
		<< dwGuildID
		<< dwFame
		<< dwFameColor
		<< strGuildName
		<< dwMemberID
		<< strMemberName
		<< bMaxGuildMember;

	Say(pMsg);
}
void CTServer::SendMW_GUILDLEAVE_REQ(DWORD dwCharID,
									 DWORD dwKey,
									 CString strTarget,
									 BYTE bReason,
									 DWORD dwTime)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDLEAVE_REQ)
		<< dwCharID
		<< dwKey
		<< strTarget
		<< bReason
		<< dwTime;

	Say(pMsg);
}
void CTServer::SendMW_GUILDDUTY_REQ(DWORD dwCharID,
									DWORD dwKey,
									CString strTarget,
									BYTE bDuty)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDDUTY_REQ)
		<< dwCharID
		<< dwKey
		<< strTarget
		<< bDuty;

	Say(pMsg);
}
void CTServer::SendMW_GUILDPEER_REQ(DWORD dwCharID,
									DWORD dwKEY,
									BYTE bResult,
									CString strTarget,
									BYTE bPeer,
									BYTE bOldPeer)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDPEER_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< strTarget
		<< bPeer
		<< bOldPeer;

	Say(pMsg);
}
void CTServer::SendMW_GUILDMEMBERLIST_REQ(DWORD dwCharID,
										  DWORD dwKEY,
										  BYTE bRet,
										  CTGuild * pGuild)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDMEMBERLIST_REQ)
		<< dwCharID
		<< dwKEY
		<< bRet;

	if(pGuild)
	{
		(*pMsg)
			<< pGuild->m_dwID
			<< pGuild->m_strName
			<< WORD(pGuild->m_mapTMember.size());

		MAPTGUILDMEMBER::iterator it;
		for(it=pGuild->m_mapTMember.begin(); it!=pGuild->m_mapTMember.end(); it++)
		{
			(*pMsg)
				<< (*it).second->m_dwID
				<< (*it).second->m_strName
				<< BYTE((*it).second->m_pChar ? (*it).second->m_pChar->m_bLevel : (*it).second->m_bLevel)
				<< (*it).second->m_bClass
				<< (*it).second->m_bDuty
				<< (*it).second->m_bPeer
				<< BYTE((*it).second->m_pChar ? TRUE : FALSE)
				<< DWORD((*it).second->m_pChar ? (*it).second->m_pChar->m_dwRegion : 0)
				<< (*it).second->m_wCastle
				<< (*it).second->m_bCamp
				<< (*it).second->m_dwTactics
				<< (*it).second->m_bWarCountry
				<< (*it).second->m_dlConnectedDate;
		}
	}
	Say(pMsg);
}
void CTServer::SendMW_GUILDINFO_REQ(DWORD dwCharID,
									DWORD dwKEY,
									BYTE bRet,
									LPTGUILDMEMBER pChief,
									CTGuild * pGuild,
									BYTE bDuty,
									BYTE bPeer)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDINFO_REQ)
		<< dwCharID
		<< dwKEY
		<< bRet;

	if(pGuild)
	{
		(*pMsg)
			<< pGuild->m_dwID
			<< pGuild->m_strName
			<< pGuild->m_timeEstablish
			<< WORD(pGuild->GetMemberSize())
			<< WORD(pGuild->GetMaxMemberCnt())
			<< pChief->m_strName
			<< pChief->m_bPeer;

		BYTE bVice=0;
		MAPTGUILDMEMBER::iterator it;
		for(it=pGuild->m_mapTMember.begin(); it!=pGuild->m_mapTMember.end(); it++)
		{
			if((*it).second->m_bDuty == GUILD_DUTY_VICECHIEF)
			{
				bVice++;
				(*pMsg)
					<< (*it).second->m_strName;
			}
		}
		while(bVice<2)
		{
			(*pMsg)
				<< NAME_NULL;
			bVice++;
		}

		(*pMsg)
			<< pGuild->m_bLevel
			<< pGuild->m_dwFame
			<< pGuild->m_dwFameColor
			<< pGuild->m_dwGI
			<< pGuild->m_dwExp
			<< pGuild->m_pTLEVEL->m_dwEXP
			<< pGuild->m_bGPoint
			<< pGuild->m_bStatus
			<< pGuild->m_dwGold
			<< pGuild->m_dwSilver
			<< pGuild->m_dwCooper
			<< bDuty
			<< bPeer
			<< pGuild->m_strArticleTitle
			<< pGuild->m_dwPvPTotalPoint
			<< pGuild->m_dwPvPUseablePoint
			<< pGuild->m_dwPvPMonthPoint
			<< pGuild->m_dwRankTotal
			<< pGuild->m_dwRankMonth;
	}
	Say(pMsg);
}

void CTServer::SendMW_GUILDCABINETLIST_REQ(DWORD dwCharID, 
										   DWORD dwKEY,
										   CTGuild *pGuild)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDCABINETLIST_REQ)
		<< dwCharID
		<< dwKEY
		<< pGuild->m_bMaxCabinet
		<< pGuild->GetCabinetSize();

	MAPTITEM::iterator it;
	for(it=pGuild->m_mapTCabinet.begin(); it!=pGuild->m_mapTCabinet.end(); it++)
	{
		LPTITEM pItem = (*it).second;
		(*pMsg)
			<< pItem->m_dwItemID;

		WrapItem(pMsg, pItem);
	}

	Say(pMsg);
}
void CTServer::SendMW_GUILDCONTRIBUTION_REQ(DWORD dwCharID, 
											DWORD dwKEY,
											BYTE bResult,
											DWORD dwExp,
											DWORD dwGold,
											DWORD dwSilver,
											DWORD dwCooper,
											DWORD dwPvPoint)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDCONTRIBUTION_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< dwExp
		<< dwGold
		<< dwSilver
		<< dwCooper
		<< dwPvPoint;

	Say(pMsg);
}
void CTServer::SendMW_GUILDARTICLELIST_REQ(DWORD dwCharID, 
										   DWORD dwKEY,
										   CTGuild *pGuild)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDARTICLELIST_REQ)
		<< dwCharID
		<< dwKEY
		<< pGuild->GetArticleSize();

	MAPTGUILDARTICLE::iterator it;
	for(it=pGuild->m_mapTArticle.begin(); it!=pGuild->m_mapTArticle.end(); it++)
	{
		(*pMsg)
			<< (*it).second->m_dwID
			<< (*it).second->m_bDuty
			<< (*it).second->m_strWritter
			<< (*it).second->m_strTitle
			<< (*it).second->m_strArticle
			<< (*it).second->m_strDate;
	}

	Say(pMsg);
}
void CTServer::SendMW_GUILDARTICLEADD_REQ(DWORD dwCharID, 
										  DWORD dwKEY,
										  BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDARTICLEADD_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDARTICLEDEL_REQ(DWORD dwCharID, 
										  DWORD dwKEY,
										  BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDARTICLEDEL_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDARTICLEUPDATE_REQ(DWORD dwCharID,
											 DWORD dwKEY,
											 BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDARTICLEUPDATE_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDFAME_REQ(DWORD dwCharID, 
									DWORD dwKEY,
									BYTE bResult,
									DWORD dwID,
									DWORD dwFame,
									DWORD dwFameColor)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDFAME_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< dwID
		<< dwFame
		<< dwFameColor;

	Say(pMsg);
}
void CTServer::SendMW_GUILDWANTEDADD_REQ(DWORD dwCharID,
										 DWORD dwKey,
										 BYTE bResult)

{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDWANTEDADD_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDWANTEDDEL_REQ(DWORD dwCharID,
										 DWORD dwKey,
										 BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDWANTEDDEL_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDWANTEDLIST_REQ(DWORD dwCharID,
                                          DWORD dwKey,
										  BYTE bCountry,
                                          LPTGUILDWANTEDAPP pApp,
										  LPMAPTGUILDWANTED mapWanted)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDWANTEDLIST_REQ)
		<< dwCharID
		<< dwKey;

	WORD wSize = pMsg->GetSize();
	DWORD dwCount = 0;

	(*pMsg)
		<< dwCount;

	MAPTGUILDWANTED::iterator it;
	for(it=mapWanted->begin(); it!=mapWanted->end(); it++)
	{
		if((*it).second.m_bCountry == bCountry)
		{
			(*pMsg)
				<< (*it).second.m_dwGuildID
				<< (*it).second.m_strName
				<< (*it).second.m_strTitle
				<< (*it).second.m_strText
				<< (*it).second.m_bMinLevel
				<< (*it).second.m_bMaxLevel
				<< (*it).second.m_dlEndTime
				<< BYTE( !pApp ? FALSE : ((*it).second.m_dwGuildID == pApp->m_dwWantedID ? TRUE : FALSE) );

			dwCount++;
		}
	}

	memcpy(pMsg->GetBuffer()+wSize, &dwCount, sizeof(dwCount));

	Say(pMsg);
}
void CTServer::SendMW_GUILDVOLUNTEERING_REQ(DWORD dwCharID,
											DWORD dwKey,
											BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDVOLUNTEERING_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDVOLUNTEERINGDEL_REQ(DWORD dwCharID,
                                               DWORD dwKey,
                                               BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDVOLUNTEERINGDEL_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDVOLUNTEERLIST_REQ(DWORD dwCharID,
                                             DWORD dwKey,
                                             LPMAPTGUILDWANTEDAPP pApp)

{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDVOLUNTEERLIST_REQ)
		<< dwCharID
		<< dwKey
		<< DWORD(pApp ? pApp->size() : 0);

	if(pApp)
	{
		MAPTGUILDWANTEDAPP::iterator it;
		for(it=pApp->begin(); it!=pApp->end(); it++)
		{
			(*pMsg)
				<< (*it).second->m_dwCharID
				<< (*it).second->m_strName
				<< (*it).second->m_bLevel
				<< (*it).second->m_bClass
				<< (*it).second->m_dwRegion;
		}
	}

	Say(pMsg);
}

void CTServer::SendMW_GUILDVOLUNTEERREPLY_REQ(DWORD dwCharID,
                                              DWORD dwKey,
                                              BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDVOLUNTEERREPLY_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);


}
void CTServer::SendMW_GUILDTACTICSWANTEDADD_REQ(DWORD dwCharID,
                                                DWORD dwKey,
                                                BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSWANTEDADD_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSWANTEDDEL_REQ(DWORD dwCharID,
                                                DWORD dwKey,
                                                BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSWANTEDDEL_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSWANTEDLIST_REQ(DWORD dwCharID,
                                                 DWORD dwKey,
												 BYTE bCountry,
                                                 LPTGUILDTACTICSWANTEDAPP pApp,
                                                 LPMAPVTGUILDTACTICSWANTED mapWanted)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSWANTEDLIST_REQ)
		<< dwCharID
		<< dwKey;

	WORD wSize = pMsg->GetSize();
	DWORD dwCount = 0;

	(*pMsg)
		<< dwCount;
	
	MAPVTGUILDTACTICSWANTED::iterator it;

	for(it=mapWanted->begin(); it!=mapWanted->end(); it++)
	{
		for(DWORD i=0; i<(*it).second.size(); i++)
		{
			if(((*it).second)[i].m_bCountry == bCountry)
			{
				(*pMsg)
					<< ((*it).second)[i].m_dwID
					<< ((*it).second)[i].m_dwGuildID
					<< ((*it).second)[i].m_strName
					<< ((*it).second)[i].m_strTitle
					<< ((*it).second)[i].m_strText
					<< ((*it).second)[i].m_bDay
					<< ((*it).second)[i].m_bMinLevel
					<< ((*it).second)[i].m_bMaxLevel
					<< ((*it).second)[i].m_dwPoint
					<< ((*it).second)[i].m_dwGold
					<< ((*it).second)[i].m_dwSilver
					<< ((*it).second)[i].m_dwCooper
					<< ((*it).second)[i].m_dlEndTime
					<< BYTE(!pApp ? FALSE : ((*it).second)[i].m_dwID == pApp->m_dwWantedID ? TRUE : FALSE);

				dwCount++;
			}
		}
	}

	memcpy(pMsg->GetBuffer()+wSize, &dwCount, sizeof(dwCount));

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSVOLUNTEERING_REQ(DWORD dwCharID,
                                                   DWORD dwKey,
                                                   BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSVOLUNTEERING_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSVOLUNTEERINGDEL_REQ(DWORD dwCharID,
                                                      DWORD dwKey,
                                                      BYTE bResult)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSVOLUNTEERINGDEL_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSVOLUNTEERLIST_REQ(DWORD dwCharID,
                                                    DWORD dwKey,
                                                    LPMAPTGUILDTACTICSWANTEDAPP pApp)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSVOLUNTEERLIST_REQ)
		<< dwCharID
		<< dwKey
		<< DWORD(pApp->size());

	MAPTGUILDTACTICSWANTEDAPP::iterator it;
	for(it=pApp->begin(); it!=pApp->end(); it++)
	{
		(*pMsg)
			<< (*it).second->m_dwCharID
			<< (*it).second->m_strName
			<< (*it).second->m_bLevel
			<< (*it).second->m_bClass
			<< (*it).second->m_dwRegion
			<< (*it).second->m_bDay
			<< (*it).second->m_dwPoint
			<< (*it).second->m_dwGold
			<< (*it).second->m_dwSilver
			<< (*it).second->m_dwCooper;
	}

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSREPLY_REQ(DWORD dwCharID,
                                            DWORD dwKey,
                                            BYTE bResult,
											DWORD dwMemberID,
											DWORD dwGuildID,
											CString strGuildName,
											CString strMemberName,
											DWORD dwGold,
											DWORD dwSilver,
											DWORD dwCooper)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSREPLY_REQ)
		<< dwCharID
		<< dwKey
		<< bResult
		<< dwGuildID
		<< strGuildName
		<< dwMemberID
		<< strMemberName
		<< dwGold
		<< dwSilver
		<< dwCooper;

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSKICKOUT_REQ(DWORD dwCharID,
                                              DWORD dwKey,
                                              BYTE bResult,
											  DWORD dwTarget,
											  BYTE bKick)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSKICKOUT_REQ)
		<< dwCharID
		<< dwKey
		<< bResult
		<< dwTarget
		<< bKick;

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSINVITE_REQ(DWORD dwCharID,
                                             DWORD dwKEY,
											 CString strGuildName,
                                             CString strNAME,
                                             BYTE bDay,
                                             DWORD dwPoint,
                                             DWORD dwGold,
                                             DWORD dwSilver,
                                             DWORD dwCooper)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSINVITE_REQ)
		<< dwCharID
		<< dwKEY
		<< strGuildName
		<< strNAME
		<< bDay
		<< dwPoint
		<< dwGold
		<< dwSilver
		<< dwCooper;

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSANSWER_REQ(DWORD dwCharID,
                                             DWORD dwKey,
                                             BYTE bResult,
											 DWORD dwGuildID,
											 CString strGuildName,
											 DWORD dwMemberID,
											 CString strMemberName,
											 DWORD dwGold,
											 DWORD dwSilver,
											 DWORD dwCooper)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSANSWER_REQ)
		<< dwCharID
		<< dwKey
		<< bResult
		<< dwGuildID
		<< strGuildName
		<< dwMemberID
		<< strMemberName
		<< dwGold
		<< dwSilver
		<< dwCooper;

	Say(pMsg);
}
void CTServer::SendMW_GUILDTACTICSLIST_REQ(DWORD dwCharID,
                                           DWORD dwKEY,
                                           CTGuild * pGuild)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_GUILDTACTICSLIST_REQ)
		<< dwCharID
		<< dwKEY
		<< DWORD(pGuild->m_mapTTactics.size());

	MAPTTACTICSMEMBER::iterator it;
	for(it=pGuild->m_mapTTactics.begin(); it!=pGuild->m_mapTTactics.end(); it++)
	{
		(*pMsg)
			<< (*it).second->m_dwID
			<< (*it).second->m_strName
			<< BYTE((*it).second->m_pChar ? (*it).second->m_pChar->m_bLevel : (*it).second->m_bLevel)
			<< (*it).second->m_bClass
			<< (*it).second->m_bDay
			<< (*it).second->m_dwRewardPoint
			<< (*it).second->m_dlRewardMoney
			<< (*it).second->m_dlEndTime
			<< (*it).second->m_dwGainPoint
			<< DWORD((*it).second->m_pChar ? (*it).second->m_pChar->m_dwRegion : 0)
			<< (*it).second->m_wCastle
			<< (*it).second->m_bCamp;
	}

	Say(pMsg);
}

////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTServer::SendMW_CHAT_REQ(DWORD dwCharID,
							   DWORD dwKEY,
							   BYTE bChannel,
							   DWORD dwSenderID,
							   CString strSenderName,
							   BYTE bCountry,
							   BYTE bWarCountry,
							   BYTE bType,
							   BYTE bGroup,
							   DWORD dwTargetID,
							   CString strTalk)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_CHAT_REQ)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< dwSenderID
		<< strSenderName
		<< bCountry
		<< bWarCountry
		<< bType
		<< bGroup
		<< dwTargetID
		<< strTalk;
	Say(pMsg);
}

void CTServer::SendMW_TAKEMONMONEY_REQ(LPPACKETBUF pBUF)
{
	CPacket *pMsg = new CPacket();

	pMsg->Copy(&(pBUF->m_packet));
	pMsg->SetID(MW_TAKEMONMONEY_REQ);

	Say(pMsg);
}

void CTServer::SendMW_MONSTERDIE_REQ(LPPACKETBUF pBUF)
{
	CPacket * pMsg = new CPacket();

	pMsg->Copy(&(pBUF->m_packet));
	pMsg->SetID(MW_MONSTERDIE_REQ);

	Say(pMsg);
}
void CTServer::SendMW_ADDITEM_REQ(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();

	pMsg->Copy(pPacket);
	pMsg->SetID(MW_ADDITEM_REQ);

	Say(pMsg);
}
void CTServer::SendMW_PARTYORDERTAKEITEM_REQ(DWORD dwCharID,
											 DWORD dwKEY, 
											 BYTE bServerID,
											 BYTE bChannel,
											 WORD wMapID,
											 DWORD dwMonID,
											 WORD wTempMonID,  
											 LPTITEM pITEM)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_PARTYORDERTAKEITEM_REQ)
		<< dwCharID
		<< dwKEY
		<< bServerID
		<< bChannel
		<< wMapID
		<< dwMonID
		<< wTempMonID;  


	WrapItem(pMsg, pITEM);

	Say(pMsg);
}
void CTServer::SendMW_FRIENDLIST_REQ(LPTCHARACTER pChar)
{
	CPacket * pMsg = new CPacket();

	pMsg->SetID(MW_FRIENDLIST_REQ)
		<< pChar->m_dwCharID
		<< pChar->m_dwKEY;

	LPTSOULMATE pTSOUL = NULL;

	MAPTSOULMATE::iterator find = pChar->m_mapTSOULMATE.find(pChar->m_dwCharID);
	if(find!=pChar->m_mapTSOULMATE.end())
	{
		pTSOUL = (*find).second;
		
		(*pMsg)
			<< pTSOUL->m_dwTarget
			<< pTSOUL->m_strName
			<< pTSOUL->m_bLevel
			<< pTSOUL->m_bClass
			<< pTSOUL->m_bConnected;

		if(pTSOUL->m_bConnected)
			(*pMsg) << pTSOUL->m_dwRegion;
		else
			(*pMsg) << DWORD(0);
	}
	else
		(*pMsg) << DWORD(0);

	(*pMsg)
		<< BYTE(pChar->m_mapFRIENDGROUP.size());

	MAPBSTRING::iterator it;
	for(it=pChar->m_mapFRIENDGROUP.begin(); it != pChar->m_mapFRIENDGROUP.end(); it++)
	{
		(*pMsg)
			<< (*it).first
			<< (*it).second;
	}

	BYTE bCount = BYTE(pChar->m_mapTFRIEND.size());

	MAPTFRIEND::iterator itFr;
	for(itFr=pChar->m_mapTFRIEND.begin(); itFr!=pChar->m_mapTFRIEND.end(); itFr++)
	{
		if((*itFr).second->m_bType == FT_TARGET)
			bCount--;
	}
    
	(*pMsg)
		<< bCount;

	for(itFr=pChar->m_mapTFRIEND.begin(); itFr!=pChar->m_mapTFRIEND.end(); itFr++)
	{
		if((*itFr).second->m_bType == FT_TARGET)
			continue;

		(*pMsg)
			<< (*itFr).second->m_dwID
			<< (*itFr).second->m_strName
			<< (*itFr).second->m_bLevel
			<< (*itFr).second->m_bGroup
			<< (*itFr).second->m_bClass
			<< (*itFr).second->m_bConnected
			<< DWORD((*itFr).second->m_bConnected ? (*itFr).second->m_dwRegion : 0);
	}
	Say(pMsg);
}
void CTServer::SendMW_FRIENDADD_REQ(DWORD dwCharID,
									DWORD dwKey,
									BYTE bResult,
									DWORD dwFriendID,
									CString strName,
									BYTE bLevel,
									BYTE bGroup,
									BYTE bClass,
									DWORD dwRegion)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDADD_REQ)
		<< dwCharID
		<< dwKey
		<< bResult
		<< dwFriendID
		<< strName
		<< bLevel
		<< bGroup
		<< bClass
		<< dwRegion;

	Say(pMsg);
}
void CTServer::SendMW_FRIENDASK_REQ(DWORD dwCharID,
									DWORD dwKey,
									CString strInviter,
									DWORD dwInviter)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDASK_REQ)
		<< dwCharID
		<< dwKey
		<< strInviter
		<< dwInviter;

	Say(pMsg);
}
void CTServer::SendMW_FRIENDERASE_REQ(DWORD dwCharID,
                                      DWORD dwKey,
                                      BYTE bRet,
									  DWORD dwTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDERASE_REQ)
		<< dwCharID
		<< dwKey
		<< bRet
		<< dwTarget;

	Say(pMsg);
}
void CTServer::SendMW_CHGPARTYCHIEF_REQ(DWORD dwCharID,
										DWORD dwKey,
										BYTE bRET)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CHGPARTYCHIEF_REQ)
		<< dwCharID
		<< dwKey
		<< bRET;

	Say(pMsg);
}
void CTServer::SendMW_PARTYATTR_REQ(DWORD dwCharID,
									DWORD dwKey,
									WORD wPartyID,
									BYTE bPartyType,
									DWORD dwPartyChiefID,
									WORD wCommander)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_PARTYATTR_REQ)
		<< dwCharID
		<< dwKey
		<< wPartyID
		<< bPartyType
		<< dwPartyChiefID
		<< wCommander;

	Say(pMsg);
}
void CTServer::SendMW_CHGSQUADCHIEF_REQ(DWORD dwCharID,
										DWORD dwKey,
										WORD wSquadID,
										DWORD dwChiefID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CHGSQUADCHIEF_REQ)
		<< dwCharID
		<< dwKey
		<< wSquadID
		<< dwChiefID;

	Say(pMsg);
}

void CTServer::SendMW_ADDITEMRESULT_REQ(DWORD dwCharID,
                                        DWORD dwKey,
                                        BYTE bChannel,
                                        WORD wMapID,
										DWORD dwMonID,
										BYTE bItemID,
										BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_ADDITEMRESULT_REQ)
		<< dwCharID
		<< dwKey
		<< bChannel
		<< wMapID
		<< dwMonID
		<< bItemID
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_CHARSTATINFO_REQ(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(MW_CHARSTATINFO_REQ);

	Say(pMsg);
}
void CTServer::SendMW_CHARSTATINFOANS_REQ(DWORD dwReqCharID, DWORD dwCharID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CHARSTATINFOANS_REQ)
		<< dwReqCharID
		<< dwCharID;

	Say(pMsg);
}
void CTServer::SendMW_LOCALENABLE_REQ(BYTE bStatus,
									  DWORD dwSecond,
									  DWORD dwLocalStart,
									  BYTE bCastleDay,
									  DWORD dwCastleStart)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_LOCALENABLE_REQ)
		<< bStatus
		<< dwSecond
		<< dwLocalStart
		<< bCastleDay
		<< dwCastleStart;

	Say(pMsg);
}
void CTServer::SendMW_CHGCORPSCOMMANDER_REQ(DWORD dwCharID,
											DWORD dwKey,
											BYTE bRET)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CHGCORPSCOMMANDER_REQ)
		<< dwCharID
		<< dwKey
		<< bRET;

	Say(pMsg);
}
void CTServer::SendMW_CORPSREPLY_REQ(DWORD dwCharID,
                                     DWORD dwKey,
                                     BYTE bResult,
									 CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CORPSREPLY_REQ)
		<< dwCharID
		<< dwKey
		<< bResult
		<< strName;

	Say(pMsg);
}
void CTServer::SendMW_ADDSQUAD_REQ(DWORD dwCharID,
								   DWORD dwKey,
								   CTParty * pParty)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_ADDSQUAD_REQ)
		<< dwCharID
		<< dwKey
		<< pParty->GetChiefID()
		<< pParty->GetID()
		<< pParty->GetSize();

	for(int i=0; i<pParty->GetSize(); i++)
	{
		(*pMsg)
			<< pParty->GetMember(i)->m_dwCharID
			<< pParty->GetMember(i)->m_strNAME
			<< FLOAT(1.0)
			<< pParty->GetMember(i)->m_command.m_dwTgObjID
			<< pParty->GetMember(i)->m_dwMaxHP
			<< pParty->GetMember(i)->m_dwHP
			<< pParty->GetMember(i)->m_command.m_wTgPosX
			<< pParty->GetMember(i)->m_command.m_wTgPosZ
			<< pParty->GetMember(i)->m_wMapID
			<< WORD(pParty->GetMember(i)->m_fPosX)
			<< WORD(pParty->GetMember(i)->m_fPosZ)
			<< MOVE_NONE
			<< pParty->GetMember(i)->m_command.m_bTgType
			<< pParty->GetMember(i)->m_bLevel
			<< pParty->GetMember(i)->m_bClass
            << pParty->GetMember(i)->m_bRace
			<< pParty->GetMember(i)->m_bSex
			<< pParty->GetMember(i)->m_bFace
			<< pParty->GetMember(i)->m_bHair
			<< pParty->GetMember(i)->m_command.m_bCommand;
	}

	Say(pMsg);
}

void CTServer::SendMW_DELSQUAD_REQ(DWORD dwCharID,
								   DWORD dwKey,
								   WORD wSquad)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_DELSQUAD_REQ)
		<< dwCharID
		<< dwKey
		<< wSquad;

	Say(pMsg);
}

void CTServer::SendMW_CORPSASK_REQ(DWORD dwCharID,
								   DWORD dwKey,
								   DWORD dwInviter,
								   CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CORPSASK_REQ)
		<< dwCharID
		<< dwKey
		<< dwInviter
		<< strName;

	Say(pMsg);
}
void CTServer::SendMW_CORPSJOIN_REQ(DWORD dwCharID,
									 DWORD dwKey,
									 WORD wID,
									 WORD wCommander)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CORPSJOIN_REQ)
		<< dwCharID
		<< dwKey
		<< wID
		<< wCommander;

	Say(pMsg);
}

void CTServer::SendMW_ADDCORPSUNIT_REQ(DWORD dwCharID,
                                       DWORD dwKey,
                                       WORD wSquadID,
                                       DWORD dwAddID,
                                       CString strNAME,
                                       FLOAT fSPEED,
                                       DWORD dwTargetID,
                                       DWORD dwMaxHP,
                                       DWORD dwHP,
                                       WORD wTargetX,
                                       WORD wTargetZ,
									   WORD wMapID,
                                       WORD wPosX,
                                       WORD wPosZ,
                                       WORD wDIR,
                                       BYTE bTargetType,
                                       BYTE bLevel,
                                       BYTE bClass,
									   BYTE bRaceID,
									   BYTE bSexID,
									   BYTE bFaceID,
									   BYTE bHairID,
                                       BYTE bTCMD)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_ADDCORPSUNIT_REQ)
		<< dwCharID
		<< dwKey
		<< wSquadID
		<< dwAddID
		<< strNAME
		<< fSPEED
		<< dwTargetID
		<< dwMaxHP
		<< dwHP
		<< wTargetX
		<< wTargetZ
		<< wMapID
		<< wPosX
		<< wPosZ
		<< wDIR
		<< bTargetType
		<< bLevel
		<< bClass
		<< bRaceID
		<< bSexID
		<< bFaceID
		<< bHairID
		<< bTCMD;

	Say(pMsg);
}

void CTServer::SendMW_DELCORPSUNIT_REQ(DWORD dwCharID,
                                       DWORD dwKey,
                                       WORD wSquad,
                                       DWORD dwTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_DELCORPSUNIT_REQ)
		<< dwCharID
		<< dwKey
		<< wSquad
		<< dwTarget;

	Say(pMsg);
}

void CTServer::SendMW_CORPSCMD_REQ(DWORD dwMemberID,
                                   DWORD dwKey,
                                   WORD wSquadID,
                                   DWORD dwCharID,
								   WORD wMapID,
                                   BYTE bCMD,
                                   DWORD dwTargetID,
                                   BYTE bTargetType,
                                   WORD wPosX,
                                   WORD wPosZ)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CORPSCMD_REQ)
		<< dwMemberID
		<< dwKey
		<< wSquadID
		<< dwCharID
		<< wMapID
		<< bCMD
		<< dwTargetID
		<< bTargetType
		<< wPosX
		<< wPosZ;

	Say(pMsg);
}

void CTServer::SendMW_REPORTENEMYLIST_REQ(DWORD dwCharID, 
									   DWORD dwKey, 
									   WORD wSquadID, 
									   DWORD dwChar)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_REPORTENEMYLIST_REQ)
		<< dwCharID
		<< dwKey
		<< wSquadID
		<< dwChar;

	Say(pMsg);
}

void CTServer::SendMW_PARTYMOVE_REQ(DWORD dwCharID,
                                    DWORD dwKey,
                                    BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_PARTYMOVE_REQ)
		<< dwCharID
		<< dwKey
		<< bResult;

	Say(pMsg);
}

void CTServer::SendMW_TMSRECV_REQ(DWORD dwCharID,
								  DWORD dwKey,
								  DWORD dwTMS,
								  CString strSender,
								  CString strMessage)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TMSRECV_REQ)
		<< dwCharID
		<< dwKey
		<< dwTMS
		<< strSender
		<< strMessage;

	Say(pMsg);
}

void CTServer::SendMW_TMSINVITEASK_REQ(DWORD dwCharID,
									   DWORD dwKEY,
									   DWORD dwTargetID,
									   DWORD dwTargetKEY,
									   DWORD dwTMS,
									   CString strMessage)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TMSINVITEASK_REQ)
		<< dwCharID
		<< dwKEY
		<< dwTargetID
		<< dwTargetKEY
		<< dwTMS
		<< strMessage;

	Say(pMsg);
}

void CTServer::SendMW_TMSINVITE_REQ(DWORD dwCharID,
									DWORD dwKey,
									DWORD dwInviter,
									LPTMS pTMS)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TMSINVITE_REQ)
		<< dwCharID
		<< dwKey
		<< dwInviter
		<< pTMS->m_dwID
		<< BYTE(pTMS->m_mapMember.size());

	MAPTCHARACTER::iterator it;
	for(it=pTMS->m_mapMember.begin(); it!=pTMS->m_mapMember.end(); it++)
	{
        (*pMsg)
			<< (*it).second->m_dwCharID
			<< (*it).second->m_strNAME
			<< (*it).second->m_bClass
			<< (*it).second->m_bLevel;
	}

	Say(pMsg);
}

void CTServer::SendMW_TMSOUT_REQ(DWORD dwCharID,
                                 DWORD dwKey,
								 DWORD dwTMS,
                                 CString strTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_TMSOUT_REQ)
		<< dwCharID
		<< dwKey
		<< dwTMS
		<< strTarget;

	Say(pMsg);
}

void CTServer::SendMW_POSTRECV_REQ(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(MW_POSTRECV_REQ);
	Say(pMsg);
}

void CTServer::SendMW_LOCALOCCUPY_REQ(BYTE bType,
									  WORD wLocalID,
									  BYTE bCountry,
									  DWORD dwGuildID,
									  CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_LOCALOCCUPY_REQ)
		<< bType
		<< wLocalID
		<< bCountry
		<< dwGuildID
		<< strName;

	Say(pMsg);
}
void CTServer::SendMW_CASTLEOCCUPY_REQ(BYTE bType,
									  WORD wCastle,
									  DWORD dwGuildID,
									  BYTE bCountry,
									  CString strGuildName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CASTLEOCCUPY_REQ)
		<< bType
		<< wCastle
		<< dwGuildID
		<< bCountry
		<< strGuildName;

	Say(pMsg);
}
void CTServer::SendMW_MISSIONOCCUPY_REQ(BYTE bType,
                                        WORD wLocalID,
                                        BYTE bCountry)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MISSIONOCCUPY_REQ)
		<< bType
		<< wLocalID
		<< bCountry;

	Say(pMsg);
}
void CTServer::SendMW_CASTLEENABLE_REQ(BYTE bStatus, DWORD dwSecond)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CASTLEENABLE_REQ)
		<< bStatus
		<< dwSecond;

	Say(pMsg);
}
void CTServer::SendMW_CASTLEAPPLY_REQ(DWORD dwCharID,
									  DWORD dwKey,
									  BYTE bResult,
									  WORD wCastle,
									  DWORD dwTarget,
									  BYTE bCamp)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CASTLEAPPLY_REQ)
		<< dwCharID
		<< dwKey
		<< bResult
		<< wCastle
		<< dwTarget
		<< bCamp;

	Say(pMsg);
}
void CTServer::SendMW_CASTLEJOIN_REQ(DWORD dwCharID,
									 DWORD dwKey,
									 DWORD dwCastleGuild,
									 WORD wMapID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_CASTLEJOIN_REQ)
		<< dwCharID
		<< dwKey
		<< dwCastleGuild
		<< wMapID;

	Say(pMsg);
}
void CTServer::SendMW_MONTEMPT_REQ(DWORD dwCharID,
								   DWORD dwKey,
								   WORD wMonID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MONTEMPT_REQ)
		<< dwCharID
		<< dwKey
		<< wMonID;

	Say(pMsg);
}
void CTServer::SendMW_GETBLOOD_REQ(DWORD dwCharID,
								   DWORD dwKey,
								   DWORD dwAtkID,
								   BYTE bAtkType,
								   BYTE bBloodType,
								   DWORD dwBlood)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_GETBLOOD_REQ)
		<< dwCharID
		<< dwKey
		<< dwAtkID
		<< bAtkType
		<< bBloodType
		<< dwBlood;

	Say(pMsg);
}

void CTServer::SendMW_FRIENDGROUPMAKE_REQ(DWORD dwCharID,
										  DWORD dwKey,
										  BYTE bRet,
										  BYTE bGroup,
										  CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDGROUPMAKE_REQ)
		<< dwCharID
		<< dwKey
		<< bRet
		<< bGroup
		<< strName;

	Say(pMsg);
}
void CTServer::SendMW_FRIENDGROUPDELETE_REQ(DWORD dwCharID,
											DWORD dwKey,
											BYTE bRet,
											BYTE bGroup)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDGROUPDELETE_REQ)
		<< dwCharID
		<< dwKey
		<< bRet
		<< bGroup;

	Say(pMsg);
}
void CTServer::SendMW_FRIENDGROUPCHANGE_REQ(DWORD dwCharID,
											DWORD dwKey,
											BYTE bRet,
											BYTE bGroup,
											DWORD dwFriend)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDGROUPCHANGE_REQ)
		<< dwCharID
		<< dwKey
		<< bRet
		<< bGroup
		<< dwFriend;

	Say(pMsg);
}
void CTServer::SendMW_FRIENDGROUPNAME_REQ(DWORD dwCharID,
										  DWORD dwKey,
										  BYTE bRet,
										  BYTE bGroup,
										  CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDGROUPNAME_REQ)
		<< dwCharID
		<< dwKey
		<< bRet
		<< bGroup
		<< strName;

	Say(pMsg);
}
void CTServer::SendMW_FRIENDCONNECTION_REQ(DWORD dwCharID,
										   DWORD dwKey,
										   BYTE bRet,
										   CString strName,
										   DWORD dwRegion)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_FRIENDCONNECTION_REQ)
		<< dwCharID
		<< dwKey
		<< bRet
		<< strName
		<< dwRegion;

	Say(pMsg);
}
void CTServer::SendMW_DEALITEMERROR_REQ(CString strTarget,
										CString strErrorChar,
										BYTE bResult)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_DEALITEMERROR_REQ)
		<< strTarget
		<< strErrorChar
		<< bResult;

	Say(pMsg);
}
void CTServer::SendMW_MAGICMIRROR_REQ(CPacket * pPacket)
{
	CPacket * pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(MW_MAGICMIRROR_REQ);

	Say(pMsg);
}

void CTServer::SendMW_CREATERECALLMON_REQ( DWORD dwCharID,
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

	pMSG->SetID(MW_CREATERECALLMON_REQ)
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

	Say(pMSG);
}

void CTServer::SendMW_RECALLMONDEL_REQ(DWORD dwCharID,
									   DWORD dwKey,
									   DWORD dwMonID)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(MW_RECALLMONDEL_REQ)
		<< dwCharID
		<< dwKey
		<< dwMonID;

	Say(pMSG);
}

void CTServer::SendMW_FRIENDREGION_REQ(DWORD dwCharID,
									   DWORD dwKEY,
									   CString strNAME,
									   DWORD dwRegion)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(MW_FRIENDREGION_REQ)
		<< dwCharID
		<< dwKEY
		<< strNAME
		<< dwRegion;

	Say(pMSG);
}

void CTServer::SendMW_STARTTELEPORT_REQ( DWORD dwCharID,
										 DWORD dwKEY,
										 BYTE bChannel,
										 WORD wMapID,
										 FLOAT fPosX,
										 FLOAT fPosY,
										 FLOAT fPosZ)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_STARTTELEPORT_REQ)
		<< dwCharID
		<< dwKEY
		<< bChannel
		<< wMapID
        << fPosX
        << fPosY
        << fPosZ;

	Say(pMSG);
}

void CTServer::SendMW_PETRIDING_REQ(DWORD dwCharID,
                                    DWORD dwKEY,
									DWORD dwRiding)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_PETRIDING_REQ)
		<< dwCharID
		<< dwKEY
		<< dwRiding;

	Say(pMSG);
}

void CTServer::SendMW_EVENTQUARTER_REQ(BYTE bDay,
									  BYTE bHour,
									  BYTE bMinute,
									  BYTE bSelect,
									  CString strPresent)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_EVENTQUARTER_REQ)
		<< bDay
		<< bHour
		<< bMinute
		<< bSelect
		<< strPresent;

	Say(pMSG);
}

void CTServer::SendMW_HELMETHIDE_REQ(DWORD dwCharID,
									 DWORD dwKEY,
									 BYTE bHide)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_HELMETHIDE_REQ)
		<< dwCharID
		<< dwKEY
		<< bHide;

	Say(pMSG);
}

void CTServer::SendMW_PARTYMEMBERRECALL_REQ(DWORD dwCharID,
											DWORD dwKEY,
											BYTE bResult,
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
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_PARTYMEMBERRECALL_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< strTarget
		<< bType
		<< bInvenID
		<< bItemID
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMSG);
}

void CTServer::SendMW_PARTYMEMBERRECALLANS_REQ(DWORD dwCharID,
											   DWORD dwKey,
											   CString strTarget,
											   BYTE bType,
											   BYTE bInvenID,
											   BYTE bItemID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_PARTYMEMBERRECALLANS_REQ)
		<< dwCharID
		<< dwKey
		<< strTarget
		<< bType
		<< bInvenID
		<< bItemID;

	Say(pMSG);
}

void CTServer::SendMW_SOULMATE_REQ(DWORD dwCharID,
								   DWORD dwKEY,
								   DWORD dwSoulmate,
								   CString strSoulmate,
								   DWORD dwTime)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_SOULMATE_REQ)
		<< dwCharID
		<< dwKEY
		<< dwSoulmate
		<< strSoulmate
		<< dwTime;

	Say(pMSG);
}

void CTServer::SendMW_SOULMATESEARCH_REQ(DWORD dwCharID,
										 DWORD dwKEY,
										 BYTE bResult,
										 DWORD dwSoulID,
										 CString strSoulmate,
										 DWORD dwRegion,
										 BYTE bNpcInvenID,
										 BYTE bNpcItemID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_SOULMATESEARCH_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< dwSoulID
		<< strSoulmate
		<< dwRegion
		<< bNpcInvenID
		<< bNpcItemID;

	Say(pMSG);
}

void CTServer::SendMW_SOULMATEREG_REQ(DWORD dwCharID,
									  DWORD dwKEY,
									  BYTE bResult,
									  BYTE bReg,
									  BYTE bNpcInvenID,  
									  BYTE bNpcItemID,
									  DWORD dwSoulmate,
									  CString strSoulmate,
									  DWORD dwRegion)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_SOULMATEREG_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< bReg
		<< bNpcInvenID
		<< bNpcItemID
		<< dwSoulmate
		<< strSoulmate
		<< dwRegion;

	Say(pMSG);
}

void CTServer::SendMW_SOULMATEEND_REQ(DWORD dwCharID,
									  DWORD dwKEY,
									  BYTE bResult,
									  DWORD dwTime)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_SOULMATEEND_REQ)
		<< dwCharID
		<< dwKEY
		<< bResult
		<< dwTime;

	Say(pMSG);
}

void CTServer::SendMW_CASTLEWARINFO_REQ(LPTCASTLEWARINFO pInfo,
                                        DWORD dwDefGuild,
                                        CString strDefName,
										BYTE bDefCountry,
                                        DWORD dwAtkGuild,
                                        CString strAtkName)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_CASTLEWARINFO_REQ)
		<< pInfo->m_wID
		<< dwDefGuild
		<< strDefName
		<< pInfo->m_bDefCountry
		<< pInfo->m_wCountryPoint[TCONTRY_D]
		<< dwAtkGuild
		<< strAtkName
		<< pInfo->m_wCountryPoint[TCONTRY_C]
		<< DWORD(pInfo->m_mapGuild.size());

	MAPDWORD::iterator it;
	for(it=pInfo->m_mapGuild.begin(); it!=pInfo->m_mapGuild.end(); it++)
	{
		(*pMSG)
			<< (*it).first
			<< (*it).second;
	}

	(*pMSG)
		<< BYTE(pInfo->m_mapTop3[TCONTRY_D].size() + pInfo->m_mapTop3[TCONTRY_C].size());

	MAPCASTLETOP3::iterator it3;
	for(it3=pInfo->m_mapTop3[TCONTRY_D].begin(); it3!=pInfo->m_mapTop3[TCONTRY_D].end(); it3++)
	{
		(*pMSG)
			<< BYTE(TCONTRY_D)
			<< it3->second.m_strName
			<< it3->second.m_wPoint;
	}
	for(it3=pInfo->m_mapTop3[TCONTRY_C].begin(); it3!=pInfo->m_mapTop3[TCONTRY_C].end(); it3++)
	{
		(*pMSG)
			<< BYTE(TCONTRY_C)
			<< it3->second.m_strName
			<< it3->second.m_wPoint;
	}
	Say(pMSG);
}

void CTServer::SendMW_ENDWAR_REQ(WORD wCastle)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(MW_ENDWAR_REQ)
		<< wCastle;

	Say(pMSG);
}

void CTServer::SendMW_CHATBAN_REQ(CString strName, __int64 nChatBanTime,BYTE bResult,DWORD dwCharID, DWORD dwKey)
{
	CPacket* pMSG = new CPacket();

	pMSG->SetID(MW_CHATBAN_REQ)
		<< strName
		<< nChatBanTime
		<< bResult
		<< dwCharID
		<< dwKey;

	Say(pMSG);
}

void CTServer::SendMW_CHARMSG_REQ(CString strName, CString strMsg)
{
	CPacket* pMSG = new CPacket();

	pMSG->SetID(MW_CHARMSG_REQ)
		<< strName
		<< strMsg;

	Say(pMSG);
}

void CTServer::SendMW_RECALLMONDATA_REQ(CPacket * pPacket)
{
	CPacket *pMSG = new CPacket();
	pMSG->Copy(pPacket);
	pMSG->SetID(MW_RECALLMONDATA_REQ);
	Say(pMSG);
}

void CTServer::SendMW_CHANGECHARBASE_REQ(DWORD dwCharID,
										 DWORD dwKey,
										 BYTE bType,
										 BYTE bValue,
										 CString strName)
{
	CPacket* pMSG = new CPacket();

	pMSG->SetID(MW_CHANGECHARBASE_REQ)
		<< dwCharID
		<< dwKey
		<< bType
		<< bValue
		<< strName;

	Say(pMSG);
}

 
void CTServer::SendMW_HEROSELECT_REQ(WORD wBattleZoneID,
									 CString strHeroName,
									 __time64_t timeHero)
{
	CPacket* pMSG = new CPacket();

	pMSG->SetID(MW_HEROSELECT_REQ)
		<< wBattleZoneID
		<< strHeroName
		<< timeHero;

	Say(pMSG);
}

void CTServer::SendMW_RELAYCONNECT_REQ(DWORD dwCharID, BYTE bRelayOn)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_RELAYCONNECT_REQ)
		<< dwCharID
		<< bRelayOn;

	Say(pMSG);
}

void CTServer::SendCT_CHATBAN_ACK(BYTE bRet,DWORD dwBanSeq,DWORD dwManagerID)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(CT_CHATBAN_ACK)
		<< bRet
		<< dwBanSeq
		<< dwManagerID;

	Say(pMSG);
}

void CTServer::SendMW_ITEMSTATE_REQ(CPacket* pPacket)
{
	CPacket* pMSG = new CPacket();
	pMSG->Copy(pPacket);
	pMSG->SetID(MW_ITEMSTATE_REQ);

	Say(pMSG);
}

void CTServer::SendCT_ITEMSTATE_ACK(CPacket* pPacket)
{
	CPacket* pMSG = new CPacket();
	pMSG->Copy(pPacket);
	pMSG->SetID(CT_ITEMSTATE_ACK);

	Say(pMSG);
}

void CTServer::SendCT_CASTLEGUILDCHG_ACK(DWORD dwManagerID,BYTE bRet,WORD wCastleID,DWORD dwDefGuildID,CString strDefGuild,DWORD dwAtkGuildID,CString strAtkGuild,__time64_t tTime)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(CT_CASTLEGUILDCHG_ACK)
		<< dwManagerID
		<< bRet
		<< wCastleID
		<< dwDefGuildID
		<< strDefGuild
		<< dwAtkGuildID
		<< strAtkGuild
		<< tTime;

	Say(pMSG);
}

void CTServer::SendMW_GAINPVPPOINT_REQ(DWORD dwOwnerID,
										DWORD dwPoint,
										BYTE bEvent,
										BYTE bType,
										BYTE bGain,
										CString strName,
										BYTE bClass,
										BYTE bLevel)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_GAINPVPPOINT_REQ)
		<< dwOwnerID
		<< dwPoint
		<< bEvent
		<< bType
		<< bGain
		<< strName
		<< bClass
		<< bLevel;

	Say(pMSG);
}

void CTServer::SendMW_GUILDPOINTLOG_REQ(DWORD dwCharID,
                                        DWORD dwKey,
										CTGuild * pGuild)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_GUILDPOINTLOG_REQ)
		<< dwCharID
		<< dwKey
		<< WORD(pGuild->m_vPointReward.size());

	for(WORD i=0; i<pGuild->m_vPointReward.size(); i++)
	{
		(*pMSG)
			<< pGuild->m_vPointReward[i].m_dlDate
			<< pGuild->m_vPointReward[i].m_strName
			<< pGuild->m_vPointReward[i].m_dwPoint;
	}

	Say(pMSG);
}

void CTServer::SendMW_GUILDPOINTREWARD_REQ(BYTE bResult,
										   DWORD dwCharID,
                                           DWORD dwKey,
										   DWORD dwRemainPoint,
										   DWORD dwPoint,
										   DWORD dwTargetID,
										   CString strTarget,
										   CString strMessage)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_GUILDPOINTREWARD_REQ)
		<< bResult
		<< dwCharID
		<< dwKey
		<< dwRemainPoint
		<< dwPoint
		<< dwTargetID
		<< strTarget
		<< strMessage;

	Say(pMSG);
}

void CTServer::SendMW_GUILDPVPRECORD_REQ(DWORD dwCharID,
                                         DWORD dwKey,
										 CTGuild * pGuild,
										 DWORD dwRecentRecordDate)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_GUILDPVPRECORD_REQ)
		<< dwCharID
		<< dwKey
		<< WORD(pGuild->m_mapTMember.size());

	MAPTGUILDMEMBER::iterator itM;
	for(itM=pGuild->m_mapTMember.begin(); itM!=pGuild->m_mapTMember.end(); itM++)
	{
		LPTGUILDMEMBER pMember = (*itM).second;
		(*pMSG)
			<< pMember->m_dwID
			<< pMember->m_weekrecord.m_wKillCount
			<< pMember->m_weekrecord.m_wDieCount;

		for(BYTE i=PVPE_KILL_H; i<PVPE_WIN; i++)
			(*pMSG) << pMember->m_weekrecord.m_aGainPoint[i];

		if(pMember->m_vRecord.size() && pMember->m_vRecord.back().m_dwDate >= dwRecentRecordDate)
		{
			(*pMSG)
				<< pMember->m_vRecord.back().m_wKillCount
				<< pMember->m_vRecord.back().m_wDieCount;

			for(BYTE i=PVPE_KILL_H; i<PVPE_WIN; i++)
				(*pMSG) << pMember->m_vRecord.back().m_aGainPoint[i];
		}
		else
		{
			(*pMSG)
				<< WORD(0)
				<< WORD(0);

			for(BYTE i=PVPE_KILL_H; i<PVPE_WIN; i++)
				(*pMSG) << DWORD(0);
		}
	}

	Say(pMSG);
}

void CTServer::SendMW_MONSTERBUY_REQ(BYTE bRet,
									 DWORD dwCharID,
                                     DWORD dwKey,
									 DWORD dwGuildID,
									 WORD wNpcID,
									 WORD wID,
									 DWORD dwPrice)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_MONSTERBUY_REQ)
		<< bRet
		<< dwCharID
		<< dwKey
		<< dwGuildID
		<< wNpcID
		<< wID
		<< dwPrice;

	Say(pMSG);
}

void CTServer::SendMW_CASTLEGUILDCHG_REQ(WORD wCastleID,
										 DWORD dwDefGuildID,
										 CString strDefGuild,
										 DWORD dwAtkGuildID,
										 CString strAtkGuild,
										 __time64_t tTime)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_CASTLEGUILDCHG_REQ)
		<< wCastleID
		<< dwDefGuildID
		<< strDefGuild
		<< dwAtkGuildID
		<< strAtkGuild
		<< tTime;

	Say(pMSG);
}
 
void CTServer::SendMW_EVENTUPDATE_REQ(BYTE bEvnetID,WORD wValue,LPEVENTINFO pEvent)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_EVENTUPDATE_REQ)
		<< bEvnetID
		<< wValue;

	pEvent->WrapPacketIn(pMSG);

	Say(pMSG);
	
}

void CTServer::SendMW_EVENTMSG_REQ(BYTE bEventID,BYTE bEventMsgType,CString strMsg)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_EVENTMSG_REQ)
		<< bEventID
		<< bEventMsgType
		<< strMsg;

	Say(pMSG);
}

void CTServer::SendMW_CASHSHOPSTOP_REQ(BYTE bType,BYTE bSendPlayer)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_CASHSHOPSTOP_REQ)
		<< bType
		<< bSendPlayer;

	Say(pMSG);
}

void CTServer::SendMW_CASHITEMSALE_REQ(DWORD dwIndex, WORD wValue, LPVTCASHITEMSALE pvSaleItem)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_CASHITEMSALE_REQ)
		<< dwIndex
		<< wValue
        << (WORD)pvSaleItem->size();

	for(WORD i = 0; i < (WORD)pvSaleItem->size(); i++)
	{
		(*pMSG)
			<< pvSaleItem->at(i).m_wID
			<< pvSaleItem->at(i).m_bSaleValue;		
	}
	
	Say(pMSG);
}

void CTServer::SendMW_WORLDPOSTSEND_REQ(BYTE	bType,
										DWORD	dwRecvID,
										CString	strRecver,
										CString	strTitle,
										CString	strMessage,
										WORD	wItemID,
										BYTE	bItemNum,
										WORD	wUseTime)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_WORLDPOSTSEND_REQ)
		<< bType
		<< dwRecvID
		<< strRecver
		<< strTitle
		<< strMessage
		<< wItemID
		<< bItemNum
		<< wUseTime;

	Say(pMSG);
}
void CTServer::SendMW_WORLDPOSTSEND_REQ(BYTE bType,
                                        DWORD dwValue,
                                        DWORD dwSenderID,
                                        CString strSender,
                                        DWORD dwRecvID,
                                        CString strRecver,
										INT64 dlValue)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_WORLDPOSTSEND_REQ)
		<< bType
		<< dwValue
		<< dwSenderID
		<< strSender
		<< dwRecvID
		<< strRecver
		<< dlValue;

	Say(pMSG);
}

void CTServer::SendMW_CASTLEAPPLICANTCOUNT_REQ(WORD wCastle, DWORD dwGuildID, BYTE bCamp, BYTE bCount)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_CASTLEAPPLICANTCOUNT_REQ)
		<< wCastle
		<< dwGuildID
		<< bCamp
		<< bCount;

	Say(pMSG);
}

 
void CTServer::SendMW_MONTHRANKLIST_REQ(MONTHRANKER arMonthRank[COUNTRY_COUNT][MONTHRANKCOUNT],BYTE bRankMonth)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_MONTHRANKLIST_REQ)
		<< bRankMonth
		<< (BYTE)MONTHRANKCOUNT;

	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
        for(BYTE j = 0; j < MONTHRANKCOUNT; j++)
			arMonthRank[i][j].WrapPacketIn(pMSG);

	Say(pMSG);
}

void CTServer::SendMW_MONTHRANKRESET_REQ(MONTHRANKER arMonthRank[],BYTE bRankMonth)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_MONTHRANKRESET_REQ)
		<< bRankMonth
		<< (BYTE)FAMERANKCOUNT;

	for(BYTE i = 0; i < FAMERANKCOUNT; i++)
        arMonthRank[i].WrapPacketIn(pMSG);

	Say(pMSG);	
}

void CTServer::SendMW_FAMERANKUPDATE_REQ(LPPACKETBUF pBUF)
{
	CPacket *pMsg = new CPacket();

	pMsg->Copy(&(pBUF->m_packet));
	pMsg->SetID(MW_FAMERANKUPDATE_REQ);

	Say(pMsg);
}

void CTServer::SendMW_MONTHRANKUPDATE_REQ(BYTE bMonth,BYTE bCountry,BYTE bStartRank,BYTE bEndRank,MONTHRANKER arMonthRank[COUNTRY_COUNT][MONTHRANKCOUNT], BYTE bNewWarlord)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_MONTHRANKUPDATE_REQ)
		<< bMonth
		<< bCountry
		<< bStartRank
		<< bEndRank;		

	if(bStartRank && bEndRank)
		for(BYTE i = bStartRank; i <= bEndRank; i++)
			arMonthRank[bCountry][i].WrapPacketIn(pMSG);
        
	if(!bNewWarlord)
	{
		(*pMSG) 
			<< BYTE(FALSE);
	}
	else
	{
		(*pMSG)
			<< (BYTE)TRUE;

		arMonthRank[bCountry][0].WrapPacketIn(pMSG);
	}
	
	Say(pMSG);
}

void CTServer::SendMW_WARLORDSAY_REQ(BYTE bType, BYTE bRankMonth,DWORD dwCharID,CString strSay)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_WARLORDSAY_REQ)
		<< bType
		<< bRankMonth
		<< dwCharID
		<< strSay;

	Say(pMSG);
}

void CTServer::SendMW_FIRSTGRADEGROUP_REQ(MONTHRANKER arMonthRank[COUNTRY_COUNT][FIRSTGRADEGROUPCOUNT],BYTE bRankMonth)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_FIRSTGRADEGROUP_REQ)
		<< bRankMonth
		<< (BYTE)FIRSTGRADEGROUPCOUNT;

	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
        for(BYTE j = 0; j < FIRSTGRADEGROUPCOUNT; j++)
			arMonthRank[i][j].WrapPacketIn(pMSG);

	Say(pMSG);
}

void CTServer::SendMW_TERMINATE_REQ(DWORD dwKey)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_TERMINATE_REQ)
		<< dwKey;

	Say(pMSG);
}

void CTServer::SendMW_TOURNAMENTENABLE_REQ(BYTE bGroup, BYTE bStep, DWORD dwPeriod, INT64 dlNextStep)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_TOURNAMENTENABLE_REQ)
		<< bGroup
		<< bStep
		<< dwPeriod
		<< dlNextStep;

	Say(pMSG);
}

void CTServer::SendMW_TOURNAMENT_REQ(CPacket* pMSG)
{
	pMSG->SetID(MW_TOURNAMENT_REQ);
	Say(pMSG);
}

void CTServer::SendMW_TOURNAMENT_REQ(DWORD dwCharID, DWORD dwKey, WORD wProtocol, BYTE bResult)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_TOURNAMENT_REQ)
		<< dwCharID
		<< dwKey
		<< wProtocol
		<< bResult;

	Say(pMSG);
}

void CTServer::SendMW_TOURNAMENTINFO_REQ(BYTE bFirstGroupCount,
										 LPMAPTOURNAMENTENTRY pTour,
										 BYTE bGroup,
										 BYTE bStep,
										 BYTE bMaxLevel)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_TOURNAMENTINFO_REQ)
		<< bFirstGroupCount
		<< bGroup
		<< bStep
		<< BYTE(pTour ? pTour->size() : 0);

	if(pTour)
	{
		MAPTOURNAMENTENTRY::iterator it;
		for(it=pTour->begin(); it!=pTour->end(); it++)
		{
			LPTOURNAMENTENTRY pEntry = (*it).second;

			(*pMSG)
				<< pEntry->m_bGroup
				<< pEntry->m_bEntryID
				<< pEntry->m_strName
				<< pEntry->m_bType
				<< pEntry->m_dwClass
				<< pEntry->m_dwFee
				<< pEntry->m_dwFeeBack
				<< pEntry->m_wPermitItemID
				<< pEntry->m_bPermitCount
				<< pEntry->m_bMinLevel
				<< pEntry->m_bMaxLevel;

			(*pMSG)<< BYTE(pEntry->m_vReward.size());

			for(BYTE i=0; i<pEntry->m_vReward.size(); i++)
			{
				(*pMSG)
					<< pEntry->m_vReward[i].m_bChartType
					<< pEntry->m_vReward[i].m_wItemID
					<< pEntry->m_vReward[i].m_bCount;
			}
		}
	}

	Say(pMSG);
}

void CTServer::SendMW_TOURNAMENTMATCH_REQ(LPMAPTNMTPLAYER mapPlayer)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_TOURNAMENTMATCH_REQ)
		<< BYTE(mapPlayer->size());

	MAPTNMTPLAYER::iterator itTm;
	for(itTm=mapPlayer->begin(); itTm!=mapPlayer->end(); itTm++)
	{
		LPTNMTPLAYER pPlayer = (*itTm).second;
		(*pMSG)
			<< pPlayer->m_bEntryID
			<< pPlayer->m_bSlotID
			<< pPlayer->m_dwCharID
			<< pPlayer->m_bCountry
			<< pPlayer->m_strName
			<< pPlayer->m_bLevel
			<< pPlayer->m_bClass
			<< pPlayer->m_dwChiefID
			<< pPlayer->m_bResult[MATCH_QFINAL]
			<< pPlayer->m_bResult[MATCH_SFINAL]
			<< pPlayer->m_bResult[MATCH_FINAL];
	}

	Say(pMSG);
}

void CTServer::SendMW_TOURNAMENTRESULT_REQ(WORD wTournamentID,
										   BYTE bStep,
										   BYTE bRet,
										   DWORD dwWin,
										   DWORD dwLose,
										   DWORD dwBlueHideTick,
										   DWORD dwRedHideTick,
										   VECTORDWORD & vPlayer)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(MW_TOURNAMENTRESULT_REQ)
		<< wTournamentID
		<< bStep
		<< bRet
		<< dwWin
		<< dwLose
		<< dwBlueHideTick
		<< dwRedHideTick
		<< BYTE(vPlayer.size());

	for(DWORD i=0; i<vPlayer.size(); i++)
		(*pMSG) << vPlayer[i];

	Say(pMSG);
}

void CTServer::SendMW_TOURNAMENTBATPOINT_REQ(DWORD dwCharID,
											 CString strName,
											 DWORD dwAmount)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(MW_TOURNAMENTBATPOINT_REQ)
		<< dwCharID
		<< strName
		<< dwAmount;

	Say(pMSG);
}

void CTServer::SendMW_MISSIONENABLE_REQ(BYTE bStatus, DWORD dwStart, DWORD dwSecond)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MISSIONENABLE_REQ)
		<< bStatus
		<< dwStart
		<< dwSecond;

	Say(pMsg);
}

void CTServer::SendMW_RPSGAME_REQ(DWORD dwCharID, DWORD dwKey, BYTE bResult, BYTE bPlayerRPS)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_RPSGAME_REQ)
		<< dwCharID
		<< dwKey
		<< bResult
		<< bPlayerRPS;

	Say(pMsg);
}

void CTServer::SendMW_RPSGAMECHANGE_REQ(LPPACKETBUF pBUF)
{
	CPacket * pMsg = new CPacket();
	pMsg->Copy(&(pBUF->m_packet));
	pMsg->SetID(MW_RPSGAMECHANGE_REQ);

	Say(pMsg);
}

void CTServer::SendMW_WARCOUNTRYBALANCE_REQ(DWORD dwCharID,
											DWORD dwKey,
											DWORD dwCount_D,
											DWORD dwCount_C,
											BYTE bGap)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_WARCOUNTRYBALANCE_REQ)
		<< dwCharID
		<< dwKey
		<< dwCount_D
		<< dwCount_C
		<< bGap;

	Say(pMsg);
}

void CTServer::SendMW_MEETINGROOM_REQ(DWORD dwCharID,
                                      DWORD dwKey,
                                      BYTE bType,
                                      BYTE bResult,
                                      CString strName)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(MW_MEETINGROOM_REQ)
		<< dwCharID
		<< dwKey
		<< bType
		<< bResult
		<< strName;

	Say(pMsg);
}

//////////////////////////////////////////////////////////////////////////
void CTServer::SendMW_APEXDATA_REQ(DWORD dwCharID, DWORD dwKey, char * pBuf,int nBufLen)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(MW_APEXDATA_REQ)
		<< dwCharID
		<< dwKey
		<< nBufLen;

	pMSG->AttachBinary((LPVOID)pBuf, nBufLen);

	Say(pMSG);
}
