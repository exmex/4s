#include "StdAfx.h"
#include "TWorldSvr.h"
#include "TWorldSvrModule.h"

void CTWorldSvrModule::SendDM_FRIENDLIST_REQ( DWORD dwCharID,
											  DWORD dwKey)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_FRIENDLIST_REQ)
		<< dwCharID
		<< dwKey;

	SayToDB(pBUF);
}

void CTWorldSvrModule::SendDM_SOULMATELIST_REQ(DWORD dwCharID,
											   DWORD dwKEY)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_SOULMATELIST_REQ)
		<< dwCharID
		<< dwKEY;

	SayToDB(pBUF);
}

void CTWorldSvrModule::SendDM_SOULMATEREG_REQ(DWORD dwCharID,
											  DWORD dwKey,
											  DWORD dwTarget,
											  BYTE bSearch,
											  BYTE bNpcInvenID,  
											  BYTE bNpcItemID)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_SOULMATEREG_REQ)
		<< dwCharID
		<< dwKey
		<< dwTarget
		<< bSearch
		<< bNpcInvenID
		<< bNpcItemID;

	SayToDB(pBUF);
}

void CTWorldSvrModule::SendDM_SOULMATEEND_REQ(DWORD dwCharID,
											  DWORD dwKey,
											  DWORD dwTime)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_SOULMATEEND_REQ)
		<< dwCharID
		<< dwKey
		<< dwTime;

	SayToDB(pBUF);
}

void CTWorldSvrModule::SendDM_SOULMATEDEL_REQ(DWORD dwCharID, DWORD dwSoul)
{
	LPPACKETBUF pBUF = new PACKETBUF();

	pBUF->m_packet.SetID(DM_SOULMATEDEL_REQ)
		<< dwCharID
		<< dwSoul;

	SayToDB(pBUF);
}

void CTWorldSvrModule::SendDM_GUILDDUTY_REQ(DWORD dwCharID,
											DWORD dwGuildID,
											BYTE bDuty)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDDUTY_REQ)
		<< dwCharID
		<< dwGuildID
		<< bDuty;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDPEER_REQ(DWORD dwCharID,
											DWORD dwGuildID,
											BYTE bPeer)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDPEER_REQ)
		<< dwCharID
		<< dwGuildID
		<< bPeer;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDMEMBERADD_REQ(DWORD dwGuildID,
												 DWORD dwCharID,
												 BYTE bLevel,
												 BYTE bDuty)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDMEMBERADD_REQ)
		<< dwGuildID
		<< dwCharID
		<< bLevel
		<< bDuty;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDARTICLEADD_REQ(DWORD dwGuildID,
												  DWORD dwID,
												  BYTE bDuty,
												  CString strWritter,
												  CString strTitle,
												  CString strArticle,
												  DWORD dwTime)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDARTICLEADD_REQ)
		<< dwGuildID
		<< dwID
		<< bDuty
		<< strWritter
		<< strTitle
		<< strArticle
		<< dwTime;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDARTICLEDEL_REQ(DWORD dwGuildID,
												  DWORD dwID)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDARTICLEDEL_REQ)
		<< dwGuildID
		<< dwID;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDARTICLEUPDATE_REQ(DWORD dwGuildID,
													 DWORD dwID,
													 CString strTitle,
													 CString strArticle)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDARTICLEUPDATE_REQ)
		<< dwGuildID
		<< dwID
		<< strTitle
		<< strArticle;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDFAME_REQ(DWORD dwGuildID,
											DWORD dwFame,
											DWORD dwFameColor)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDFAME_REQ)
		<< dwGuildID
		<< dwFame
		<< dwFameColor;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDLEVEL_REQ(DWORD dwGuildID,
											 BYTE bLevel)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDLEVEL_REQ)
		<< dwGuildID
		<< bLevel;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDCABINETMAX_REQ(DWORD dwGuildID, 
												  BYTE bMaxCabinet)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDCABINETMAX_REQ)
		<< dwGuildID
		<< bMaxCabinet;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDCONTRIBUTION_REQ(DWORD dwGuildID,
													DWORD dwCharID,
													DWORD dwExp,
													DWORD dwGold,
													DWORD dwSilver,
													DWORD dwCooper)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDCONTRIBUTION_REQ)
		<< dwGuildID
		<< dwCharID
		<< dwExp
		<< dwGold
		<< dwSilver
		<< dwCooper;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GUILDLOAD_REQ(DWORD dwCharID,
											DWORD dwKey,
											DWORD dwGuildID)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDLOAD_REQ)
		<< dwCharID
		<< dwKey
		<< dwGuildID;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_FRIENDERASE_REQ(DWORD dwCharID,
											  DWORD dwTarget)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_FRIENDERASE_REQ)
		<< dwCharID
		<< dwTarget;

	SayToDB(pMSG);
}

void CTWorldSvrModule::SendDM_GUILDPVPOINT_REQ(DWORD dwGuildID,
											   DWORD dwTotalPoint,
											   DWORD dwUseablePoint,
											   DWORD dwMonthPoint)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDPVPOINT_REQ)
		<< dwGuildID
		<< dwTotalPoint
		<< dwUseablePoint
		<< dwMonthPoint;

	SayToDB(pMSG);
}

void CTWorldSvrModule::SendDM_GUILDPOINTREWARD_REQ(DWORD dwGuildID,
                                                   DWORD dwPoint,
                                                   CString strName,
												   DWORD dwTotalPoint,
												   DWORD dwUseablePoint)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDPOINTREWARD_REQ)
		<< dwGuildID
		<< dwPoint
		<< strName
		<< dwTotalPoint
		<< dwUseablePoint;

	SayToDB(pMSG);
}

void CTWorldSvrModule::SendDM_PVPRECORD_REQ(DWORD dwGuildID, LPTGUILDMEMBER pMember)
{
	if(!pMember || !pMember->m_bSaveRecord)
		return;

	pMember->m_bSaveRecord = FALSE;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_PVPRECORD_REQ)
		<< dwGuildID
		<< pMember->m_dwID;

	WORD wSize = pMSG->m_packet.GetSize();
	WORD wCount = 0;
	pMSG->m_packet << wCount;

	for(WORD i=0; i<pMember->m_vRecord.size(); i++)
	{
		if(!pMember->m_vRecord[i].m_bLoad)
		{
			pMSG->m_packet
				<< pMember->m_vRecord[i].m_dwDate
				<< pMember->m_vRecord[i].m_wKillCount
				<< pMember->m_vRecord[i].m_wDieCount;

			for(BYTE e=0; e<PVPE_COUNT; e++)
			{
				pMSG->m_packet
					<< pMember->m_vRecord[i].m_aGainPoint[e];
			}

			wCount++;
		}
	}
	memcpy(pMSG->m_packet.GetBuffer()+wSize, &wCount, sizeof(wCount));

	if(wCount)
		SayToDB(pMSG);
	else
		delete pMSG;
}

void CTWorldSvrModule::SendDM_CASTLEAPPLY_REQ(WORD wCastle,
											  DWORD dwTarget,
											  BYTE bCamp)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_CASTLEAPPLY_REQ)
		<< wCastle
		<< dwTarget
		<< bCamp;

	SayToDB(pMSG);
}
 
void CTWorldSvrModule::SendDM_CASHITEMSALE_REQ(DWORD dwIndex, WORD wValue, LPVTCASHITEMSALE vSALEITEM)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_CASHITEMSALE_REQ)
		<< dwIndex
		<< wValue
		<< (WORD)vSALEITEM->size();
		
	for(WORD i = 0; i < (WORD)vSALEITEM->size(); i++)
	{
		pMSG->m_packet
			<< vSALEITEM->at(i).m_wID
			<< vSALEITEM->at(i).m_bSaleValue;
	}

	SayToDB(pMSG);
}

void CTWorldSvrModule::SendDM_GUILDWANTEDADD_REQ(DWORD dwGuildID,
												 BYTE bMinLevel,
												 BYTE bMaxLevel,
												 CString strTitle,
												 CString strText)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDWANTEDADD_REQ)
		<< dwGuildID
		<< bMinLevel
		<< bMaxLevel
		<< strTitle
		<< strText;

	SayToDB(pMSG);
}

void CTWorldSvrModule::SendDM_GUILDWANTEDDEL_REQ(DWORD dwGuildID)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDWANTEDDEL_REQ)
		<< dwGuildID;

	SayToDB(pMSG);
}

void CTWorldSvrModule::SendDM_GUILDVOLUNTEERING_REQ(BYTE bType,
                                                    DWORD dwCharID,
													DWORD dwID)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDVOLUNTEERING_REQ)
		<< bType
		<< dwCharID
		<< dwID;

	SayToDB(pMSG);
}

void CTWorldSvrModule::SendDM_GUILDVOLUNTEERINGDEL_REQ(BYTE bType,
													   DWORD dwCharID)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDVOLUNTEERINGDEL_REQ)
		<< bType
		<< dwCharID;

	SayToDB(pMSG);
}
void CTWorldSvrModule::SendDM_GUILDTACTICSWANTEDADD_REQ(DWORD dwID,
                                                        DWORD dwGuildID,
                                                        DWORD dwPoint,
														DWORD dwGold,
														DWORD dwSilver,
														DWORD dwCooper,
                                                        BYTE bDay,
														BYTE bMinLevel,
														BYTE bMaxLevel,
                                                        CString strTitle,
														CString strText)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDTACTICSWANTEDADD_REQ)
		<< dwID
		<< dwGuildID
		<< dwPoint
		<< dwGold
		<< dwSilver
		<< dwCooper
		<< bDay
		<< bMinLevel
		<< bMaxLevel
		<< strTitle
		<< strText;

	SayToDB(pMSG);
}
void CTWorldSvrModule::SendDM_GUILDTACTICSWANTEDDEL_REQ(DWORD dwID)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDTACTICSWANTEDDEL_REQ)
		<< dwID;

	SayToDB(pMSG);
}
void CTWorldSvrModule::SendDM_GUILDTACTICSADD_REQ(DWORD dwGuildID,
                                                  DWORD dwTactics,
                                                  DWORD dwPoint,
												  INT64 dlMoney,
                                                  BYTE bDay,
                                                  INT64 dlEnd)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDTACTICSADD_REQ)
		<< dwGuildID
		<< dwTactics
		<< dwPoint
		<< dlMoney
		<< bDay
		<< dlEnd;

	SayToDB(pMSG);
}
void CTWorldSvrModule::SendDM_GUILDTACTICSDEL_REQ(DWORD dwCharID)
{
	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_GUILDTACTICSDEL_REQ)
		<< dwCharID;

	SayToDB(pMSG);
}

void CTWorldSvrModule::SendDM_TACTICSPOINT_REQ(LPTTACTICSMEMBER pTactics)
{
	if(!pTactics || !pTactics->m_dwGainPoint)
		return;

	LPPACKETBUF pMSG = new PACKETBUF();
	pMSG->m_packet.SetID(DM_TACTICSPOINT_REQ)
		<< pTactics->m_dwID
		<< pTactics->m_dwGainPoint;

	SayToDB(pMSG);
}

void CTWorldSvrModule::SendDM_GUILDLEAVE_REQ(DWORD dwGuildID,
                                             DWORD dwCharID,
                                             BYTE bReason,
                                             DWORD dwTime)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GUILDLEAVE_REQ)
		<< dwGuildID
		<< dwCharID
		<< bReason
		<< dwTime;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_CLEARDATA_REQ(DWORD dwKey)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_CLEARDATA_REQ)
		<< dwKey;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_GETCHARINFO_REQ(DWORD dwCharID,
											  DWORD dwKey,
											  WORD wProtocol,
											  CString strTarget)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_GETCHARINFO_REQ)
		<< dwCharID
		<< dwKey
		<< wProtocol
		<< strTarget;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_TOURNAMENTPAYBACK_REQ(DWORD dwCharID,
													DWORD dwBack)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_TOURNAMENTPAYBACK_REQ)
		<< dwCharID
		<< dwBack;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_TOURNAMENTRESULT_REQ(BYTE bStep,
												   BYTE bRet,
												   DWORD dwWin,
												   DWORD dwLose)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_TOURNAMENTRESULT_REQ)
		<< bStep
		<< bRet
		<< dwWin
		<< dwLose;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_TOURNAMENTAPPLY_REQ(BYTE bAdd,
												  DWORD dwCharID,
												  BYTE bEntry,
												  DWORD dwChiefID)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_TOURNAMENTAPPLY_REQ)
		<< bAdd
		<< dwCharID
		<< bEntry
		<< dwChiefID;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_TOURNAMENTCLEAR_REQ()
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_TOURNAMENTCLEAR_REQ);

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_TNMTEVENTSCHEDULEADD_REQ(CPacket * pPacket, WORD wTourID)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.Copy(pPacket);
	pMsg->m_packet.SetID(DM_TNMTEVENTSCHEDULEADD_REQ);

	memcpy(pMsg->m_packet.GetBuffer() + PACKET_HEADER_SIZE + 5, &wTourID, sizeof(wTourID));

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_TNMTEVENTSCHEDULEDEL_REQ(WORD wTourID)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_TNMTEVENTSCHEDULEDEL_REQ)
		<< wTourID;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_TNMTEVENTENTRYADD_REQ(CPacket * pPacket)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.Copy(pPacket);
	pMsg->m_packet.SetID(DM_TNMTEVENTENTRYADD_REQ);

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_HELPMESSAGE_REQ(BYTE bID,
											  INT64 dlStart,
											  INT64 dlEnd,
											  CString strMessage)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_HELPMESSAGE_REQ)
		<< bID
		<< dlStart
		<< dlEnd
		<< strMessage;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_RPSGAMERECORD_REQ(BYTE bRecord,
												DWORD dwCharID,
												BYTE bType,
												BYTE bWinCount,
												INT64 dDate)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_RPSGAMERECORD_REQ)
		<< bRecord
		<< dwCharID
		<< bType
		<< bWinCount
		<< dDate;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_TOURNAMENTSTATUS_REQ(WORD wID,
												   BYTE bGroup,
                                                   BYTE bStep)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_TOURNAMENTSTATUS_REQ)
		<< wID
		<< bGroup
		<< bStep;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_ACTIVECHARUPDATE_REQ()
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_ACTIVECHARUPDATE_REQ);

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_CMGIFT_REQ(CString strTarget,
										 WORD wGiftID,
										 BYTE bTool,
										 DWORD dwGmID)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.SetID(DM_CMGIFT_REQ)
		<< strTarget
		<< wGiftID
		<< bTool
		<< dwGmID;

	SayToDB(pMsg);
}

void CTWorldSvrModule::SendDM_CMGIFT_ACK(BYTE bRet,
										 CString strTarget,
										 WORD wGiftID,
										 BYTE bTool,
										 DWORD dwGMID)
{
	LPPACKETBUF pMsg = new PACKETBUF();
	pMsg->m_packet.SetID(DM_CMGIFT_ACK)
		<< bRet
		<< strTarget
		<< wGiftID
		<< bTool
		<< dwGMID;

	SayToBATCH(pMsg);
}

void CTWorldSvrModule::SendDM_CMGIFTCHARTUPDATE_REQ(CPacket * pPacket)
{
	LPPACKETBUF pMsg = new PACKETBUF();

	pMsg->m_packet.Copy(pPacket);
	pMsg->m_packet.SetID(DM_CMGIFTCHARTUPDATE_REQ);

	SayToDB(pMsg);
}

