#include "StdAfx.h"

void CTUser::SendCT_SERVICEMONITOR_REQ(DWORD dwTick,
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

void CTUser::SendCS_LOGIN_ACK( BYTE bResult,
							   DWORD dwUserID,
							   DWORD dwCharID,
							   DWORD dwKEY,
							   DWORD dwIPAddr,
							   WORD wPort,
							   BYTE bCreateCnt,
							   BYTE bInPcBang,
							   DWORD dwPremium,
							   __time64_t dCurTime)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_LOGIN_ACK)
		<< bResult
		<< dwUserID
		<< dwCharID
		<< dwKEY
		<< dwIPAddr
		<< wPort
		<< bCreateCnt
		<< bInPcBang
		<< dwPremium
		<< dCurTime
		<< m_dlCheckKey;

	Say(pMSG);
}

// Çö½Â·æ CS_TESTVERSION_ACK
void CTUser::SendCS_TESTVERSION_ACK( WORD wVersion)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_TESTVERSION_ACK)
		<< wVersion;

	Say(pMSG);
}

void CTUser::SendCS_CREATECHAR_ACK( BYTE bResult,
								    DWORD dwID,
									CString strNAME,
									BYTE bSlotID,
									BYTE bClass,
									BYTE bRace,
									BYTE bCountry,
									BYTE bSex,
									BYTE bHair,
									BYTE bFace,
									BYTE bBody,
									BYTE bPants,
									BYTE bHand,
									BYTE bFoot,
									BYTE bCreateCnt)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_CREATECHAR_ACK)
		<< bResult
		<< dwID
		<< strNAME
		<< bSlotID
		<< bClass
		<< bRace
		<< bCountry
		<< bSex
		<< bHair
		<< bFace
		<< bBody
		<< bPants
		<< bHand
		<< bFoot
		<< bCreateCnt;

	Say(pMSG);
}

void CTUser::SendCS_DELCHAR_ACK( BYTE bResult,
								 DWORD dwCharID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_DELCHAR_ACK)
		<< bResult
		<< dwCharID;

	Say(pMSG);
}

void CTUser::SendCS_START_ACK( BYTE bResult,
							   DWORD dwMapIP,
							   WORD wPort,
							   BYTE bServerID)
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_START_ACK)
		<< bResult
		<< dwMapIP
		<< wPort
		<< bServerID;

	Say(pMSG);
}

void CTUser::SendCS_HOTSEND_ACK()
{
	CPacket *pMSG = new CPacket();

	pMSG->SetID(CS_HOTSEND_ACK);

	Say(pMSG);
}
