#pragma once

#include "stdAfx.h"
#include "TRelaySvr.h"
#include "TRelaySvrModule.h"


void CTRelaySvrModule::SendRW_ENTERCHAR_REQ(DWORD dwCharID,
											CString strName)
{
	CPacket* pMSG = new CPacket();

	pMSG->SetID(RW_ENTERCHAR_REQ)
		<< dwCharID
		<< strName;

	m_world.Say(pMSG);
}

void CTRelaySvrModule::SendRW_RELAYSVR_REQ()
{
	CPacket* pMSG = new CPacket();

	pMSG->SetID(RW_RELAYSVR_REQ)
		<< MAKEWORD(m_bServerID, BYTE(SVRGRP_RLYSVR) );

	m_world.Say(pMSG);
}

void CTRelaySvrModule::SendRW_RELAYCONNECT_REQ(DWORD dwCharID)
{
	CPacket* pMSG = new CPacket();
	pMSG->SetID(RW_RELAYCONNECT_REQ)
		<< dwCharID;		

	m_world.Say(pMSG);
}


void CTRelaySvrModule::SendRW_CHATBAN_ACK(CString strTarget, WORD wMin)
{
	CPacket * pMSG = new CPacket();
	pMSG->SetID(MW_CHATBAN_ACK)
		<< strTarget
		<< wMin
		<< WORD(0)
		<< WORD(0);
	m_world.Say(pMSG);
}