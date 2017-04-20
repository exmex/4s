#pragma once

#include "stdafx.h"
#include "GMToolDoc.h"

void CGMToolDoc::SendCT_OPLOGIN_REQ()
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_OPLOGIN_REQ)
		<< m_strUserID
		<< m_strPasswd;

	Say(pMsg);
}

void CGMToolDoc::SendCT_CHARMSG_REQ(CString strName, CString strMsg)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_CHARMSG_REQ)
		<< strName
		<< strMsg;

	Say(pMsg);
}

void CGMToolDoc::SendCT_USERMOVE_REQ(CString strUser, BYTE bChannel, WORD wMapID, FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERMOVE_REQ)
		<< strUser
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMsg);
}

void CGMToolDoc::SendCT_CHATBAN_REQ(CString strName, WORD wMin,CString strReason)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CHATBAN_REQ)
		<< strName
		<< wMin
		<< strReason;

	Say(pMsg);
}

void CGMToolDoc::SendCT_USERKICKOUT_REQ(CString strUser)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERKICKOUT_REQ)
		<< strUser;

	Say(pMsg);
}

void CGMToolDoc::SendCT_USERPOSITION_REQ(BYTE bWorld, CString strUser, CString strTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERPOSITION_REQ)
		<< bWorld
		<< strUser
		<< strTarget;

	Say(pMsg);
}

void CGMToolDoc::SendCT_ANNOUNCEMENT_REQ(DWORD nID, CString strAnnounce)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_ANNOUNCEMENT_REQ)
		<< nID
		<< strAnnounce;

	Say(pMsg);
}

void CGMToolDoc::SendCT_MONSPAWNFIND_REQ(BYTE bGroupID, BYTE bChannel, WORD wMapID, WORD wSpawnID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_MONSPAWNFIND_REQ)
		<< bGroupID
		<< bChannel
		<< wMapID
		<< wSpawnID;

	Say(pMsg);
}

void CGMToolDoc::SendCT_MONACTION_REQ(BYTE bGroup, BYTE bChannel, WORD wMapID, DWORD dwMonID, BYTE bAction, DWORD dwTriggerID, DWORD dwHostID, DWORD dwRHID, BYTE bRHType)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_MONACTION_REQ)
		<< bGroup
		<< bChannel
		<< wMapID
		<< dwMonID
		<< bAction
		<< dwTriggerID
		<< dwHostID
		<< dwRHID
		<< bRHType;

	Say(pMsg);
}