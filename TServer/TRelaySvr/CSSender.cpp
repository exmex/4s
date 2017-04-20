#pragma once

#include "stdafx.h"
#include "TRelaySvr.h"
#include "TRelaySvrModule.h"

void CTPlayer::SendCT_SERVICEMONITOR_REQ(DWORD dwTick, DWORD dwSession, DWORD dwUser, DWORD dwActiveUser)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CT_SERVICEMONITOR_REQ)
		<< dwTick
		<< dwSession
		<< dwUser
		<< dwActiveUser;

	Say(pMSG);
}

void CTPlayer::SendCT_CHATBAN_ACK(BYTE bRet, DWORD dwBanSeq)
{
	CPacket* pMSG = new CPacket();

	pMSG->SetID(CT_CHATBAN_ACK)
		<< bRet
		<< dwBanSeq;

	Say(pMSG);
}

void CTPlayer::SendCS_RELAYCHARDATA_ACK(BYTE bResult)
{
	CPacket* pMSG = new CPacket();

	pMSG->SetID(CS_RELAYCHARDATA_ACK)
		<< bResult;

	Say(pMSG);
}

void CTPlayer::SendCS_CHAT_ACK(BYTE bGroup,
							   DWORD dwSenderID,
							   CString strName,
							   CString strTalk)
{
	if(!CheckProtected(strName, PROTECTED_CHAT))
		return;

	CPacket* pMSG = new CPacket();

	pMSG->SetID(CS_CHAT_ACK)
		<< bGroup
		<< dwSenderID
		<< strName
		<< strTalk;

	Say(pMSG);
}

void CTPlayer::SendCS_SYSTEMMSG_ACK(BYTE bType,
									WORD wLocalID,
									DWORD dwSecond,
									CString strWinner,
									CString strLoser,
									BYTE bHour,
									BYTE bMinute,
									CString strPresent,
									BYTE bSelect)
{
	//if((bType == SM_BATTLE_OPENGATE ||
	//	bType == SM_BATTLE_BOSSDIE ) &&
	//	(!m_pLocal || m_pLocal->m_wLocal != wLocalID))
	//	return;

	CPacket * pMSG = new CPacket();
	pMSG->SetID(CS_SYSTEMMSG_ACK)
		<< bType;

	switch(bType)
	{
	case SM_BATTLE_NORMAL:
	case SM_BATTLE_START:
	case SM_BATTLE_BOSSDIE:
	case SM_BATTLE_OPENGATE:
		break;
	case SM_BATTLE_START_ALARM:
	case SM_BATTLE_END_ALARM:
	case SM_BATTLE_PEACE:
		(*pMSG)
			<< dwSecond;
		break;
	case SM_DUAL_WIN:
		(*pMSG)
			<< strWinner
			<< strLoser;
		break;
	case SM_EVENT_QUARTER:
		(*pMSG)
			<< bHour
			<< bMinute
			<< strPresent
			<< bSelect;
		break;
	case SM_CHAT_BAN:
		(*pMSG)
			<< dwSecond;
		break;
	default:
		delete pMSG;
		return;
	}

	Say(pMSG);
}

void CTPlayer::RelayCorpsMsg(CPacket * pPacket,
                             WORD wMsgID)
{
	CPacket *pMSG = new CPacket();
	pMSG->Copy(pPacket);
	pMSG->SetID(wMsgID);

	Say(pMSG);
}

void CTPlayer::SendCS_CORPSCMD_ACK(WORD wSquad,
								   DWORD dwCharID,
								   WORD wMapID,
								   BYTE bCMD,
								   DWORD dwTargetID,
								   BYTE bTargetType,
								   WORD wPosX,
								   WORD wPosZ)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(CS_CORPSCMD_ACK)
		<< wSquad
		<< dwCharID
		<< wMapID
		<< bCMD
		<< dwTargetID
		<< bTargetType
		<< wPosX
		<< wPosZ;

	Say(pMSG);
}
