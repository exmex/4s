#include "stdafx.h"
#include "TClient.h"
#include "TClientWnd.h"

void CTClientWnd::SendCS_LOGIN_REQ( CString strUserID, CString strPasswd, DWORD dwSiteCode)
{
	CPacket vMSG;

	WORD wVersion = TVERSION;
	vMSG.SetID(CS_LOGIN_REQ)
		<< wVersion
		<< strUserID
		<< strPasswd
		<< theApp.m_dlCheckFile;

	if( CTNationOption::MODIFY_DIRECTLOGIN )
		vMSG << dwSiteCode;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_GROUPLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_GROUPLIST_REQ);
	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_CHARLIST_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_CHARLIST_REQ)
		<< m_pTGROUP->m_bGroupID;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_CREATECHAR_REQ( BYTE bGroupID,
										 CString strNAME,
										 BYTE bSlotID,
										 BYTE bClass,
										 BYTE bRace,
										 BYTE bContry,
										 BYTE bSex,
										 BYTE bHair,
										 BYTE bFace,
										 BYTE bBody,
										 BYTE bPants,
										 BYTE bHand,
										 BYTE bFoot)
{
	CPacket vMSG;

	vMSG.SetID(CS_CREATECHAR_REQ)
		<< bGroupID
		<< strNAME
		<< bSlotID
		<< bClass
		<< bRace
		<< bContry
		<< bSex
		<< bHair
		<< bFace
		<< bBody
		<< bPants
		<< bHand
		<< bFoot;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_DELCHAR_REQ( BYTE bGroupID,
									  CString strPasswd,
									  DWORD dwCharID)
{
	CPacket vMSG;

	vMSG.SetID(CS_DELCHAR_REQ)
		<< bGroupID
		<< strPasswd
		<< dwCharID;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_START_REQ( BYTE bGroupID,
								    BYTE bChannel,
									DWORD dwCharID)
{
	CPacket vMSG;

	vMSG.SetID(CS_START_REQ)
		<< bGroupID
		<< bChannel
		<< dwCharID;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_CHANNELLIST_REQ(BYTE bGroupID)
{
	CPacket vMSG;

	vMSG.SetID(CS_CHANNELLIST_REQ)
		<< bGroupID;

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_AGREEMENT_REQ()
{
	CPacket vMSG;

	vMSG.SetID(CS_AGREEMENT_REQ);

	m_pTNet->Say(&vMSG);
}

void CTClientWnd::SendCS_HOTSEND_REQ(INT64 dlValue, BYTE bAll)
{
	CPacket vMSG;

	vMSG.SetID(CS_HOTSEND_REQ)
		<< dlValue
		<< bAll;

	m_pTNet->Say(&vMSG);
}





