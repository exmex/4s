#include "StdAfx.h"


CTServer::CTServer()
{
	m_dwID = 0;
	m_pSVRTEMP = NULL;	
	m_dwRecvTick = 0;
	m_dwSendTick = 0;
}

CTServer::~CTServer()
{
}

void CTServer::SendCT_SERVICEMONITOR_ACK(DWORD dwTick)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEMONITOR_ACK)
		<< dwTick;

	Say(pMsg);
}

//////////////////////////////////////////////////////////////
// Çö½Â·æ CT_ANNOUNCEMENT_ACK
void CTServer::SendCT_ANNOUNCEMENT_ACK(CString strAnnounce)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_ANNOUNCEMENT_ACK)		
		<< strAnnounce;

	Say(pMsg);
}
// Çö½Â·æ CT_USERKICKOUT_ACK
void CTServer::SendCT_USERKICKOUT_ACK(CString strUser)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERKICKOUT_ACK)		
		<< strUser;

	Say(pMsg);
}
// Çö½Â·æ CT_USERMOVE_ACK
void CTServer::SendCT_USERMOVE_ACK(CString strUser, BYTE bChannel, WORD wMapID, FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERMOVE_ACK)		
		<< strUser
		<< bChannel
		<< wMapID
		<< fPosX
		<< fPosY
		<< fPosZ;

	Say(pMsg);
}
// Çö½Â·æ CT_USERPOSITION_ACK
void CTServer::SendCT_USERPOSITION_ACK(CString strUser, CString strTarget)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_USERPOSITION_ACK)		
		<< strUser
		<< strTarget;

	Say(pMsg);
}
// Çö½Â·æ CT_MONSPAWNFIND_ACK
void CTServer::SendCT_MONSPAWNFIND_ACK(DWORD dwManager, BYTE bChannel, WORD wMapID, WORD wSpawnID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_MONSPAWNFIND_ACK)
		<< dwManager
		<< bChannel
		<< wMapID
		<< wSpawnID;

	Say(pMsg);
}
// Çö½Â·æ CT_MONACTION_ACK
void CTServer::SendCT_MONACTION_ACK(BYTE bChannel, WORD wMapID, DWORD dwMonID, BYTE bAction, DWORD dwTriggerID, DWORD dwHostID, DWORD dwRHID, BYTE bRHType,WORD wSpawnID)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_MONACTION_ACK)
		<< bChannel
		<< wMapID
		<< dwMonID
		<< bAction
		<< dwTriggerID
		<< dwHostID
		<< dwRHID
		<< bRHType
		<< wSpawnID;

	Say(pMsg);
}


void CTServer::SendCT_CHATBAN_REQ(CString strName, WORD wMin, DWORD dwBanSeq, DWORD dwManagerID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CHATBAN_REQ)
		<< strName
		<< wMin
		<< dwBanSeq
		<< dwManagerID;

	Say(pMsg);
}

void CTServer::SendCT_CHARMSG_ACK(CString strName, CString strMsg)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CHARMSG_ACK)
		<< strName
		<< strMsg;

	Say(pMsg);
}

void CTServer::SendCT_SERVICEDATACLEAR_ACK()
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEDATACLEAR_ACK);		

	Say(pMsg);
}

void CTServer::SendCT_ITEMFIND_REQ(DWORD dwManagerID, WORD wItemID, CString strName)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_ITEMFIND_REQ)
		<< dwManagerID
		<< wItemID
		<< strName;

	Say(pMsg);
}

void CTServer::SendCT_ITEMSTATE_REQ(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);

	Say(pMsg);
}

void CTServer::SendCT_CTRLSVR_REQ()
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CTRLSVR_REQ);
		
	Say(pMsg);
}
	
void CTServer::SendCT_CASTLEINFO_REQ(DWORD dwManagerID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CASTLEINFO_REQ)
		<< dwManagerID;

	Say(pMsg);
}

void CTServer::SendCT_CASTLEGUILDCHG_REQ(WORD wCastleID,DWORD dwDefGuildID,DWORD dwAtkGuildID,DWORD dwManagerID,__time64_t tTime)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CASTLEGUILDCHG_REQ)
		<< wCastleID
		<< dwDefGuildID
		<< dwAtkGuildID
		<< dwManagerID
		<< tTime;

	Say(pMsg);
}

void CTServer::SendCT_CASTLEENABLE_REQ(BYTE bType,BYTE bStatus, DWORD dwSecond)
{
	CPacket* pMsg = new CPacket();	
	pMsg->SetID(SM_BATTLESTATUS_REQ)
		<< bType
		<< bStatus
		<< (DWORD) 0
		<< dwSecond;

	Say(pMsg);
}

void CTServer::SendCT_EVENTUPDATE_REQ(BYTE bEventID, WORD wValue,LPEVENTINFO pEVENT)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_EVENTUPDATE_REQ)
		<< bEventID
		<< wValue;

	pEVENT->WrapPacketIn(pMsg);
	
	Say(pMsg);
}

void CTServer::SendCT_EVENTMSG_REQ( BYTE bEventID, BYTE bEventMsgType,CString strMsg )
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_EVENTMSG_REQ)
		<< bEventID
		<< bEventMsgType
		<< strMsg;

	Say(pMsg);
}

void CTServer::SendCT_CASHSHOPSTOP_REQ(BYTE bType)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CASHSHOPSTOP_REQ)
		<< bType;

	Say(pMsg);		
}

void CTServer::SendCT_CASHITEMSALE_REQ(DWORD dwIndex,WORD wValue,LPVTCASHITEMSALE pvSaleItem)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CASHITEMSALE_REQ)
		<< dwIndex
		<< wValue
		<< WORD(pvSaleItem->size());
	
	for(WORD i = 0; i < (WORD)pvSaleItem->size(); i++)
	{
		(*pMsg)
			<< pvSaleItem->at(i).m_wID
			<< pvSaleItem->at(i).m_bSaleValue;
	}

	Say(pMsg);
}
 
void CTServer::SendCT_EVENTQUARTERLIST_REQ(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(BYTE));
	pMsg->SetID(CT_EVENTQUARTERLIST_REQ);		

	Say(pMsg);
}

void CTServer::SendCT_EVENTQUARTERUPDATE_REQ(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(BYTE));
	pMsg->SetID(CT_EVENTQUARTERUPDATE_REQ);

	Say(pMsg);
}

void CTServer::SendCT_TOURNAMENTEVENT_REQ(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(BYTE));
	pMsg->SetID(CT_TOURNAMENTEVENT_REQ);

	Say(pMsg);
}

void CTServer::SendCT_HELPMESSAGE_REQ(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(BYTE));
	pMsg->SetID(CT_HELPMESSAGE_REQ);

	Say(pMsg);
}

void CTServer::SendCT_RPSGAMEDATA_REQ(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(CT_RPSGAMEDATA_REQ);

	Say(pMsg);	
}

void CTServer::SendCT_RPSGAMECHANGE_REQ(CPacket* pPacket)
{
	CPacket* pMsg = new CPacket();
	pMsg->Copy(pPacket);
	pMsg->SetID(CT_RPSGAMECHANGE_REQ);

	Say(pMsg);	
}
void CTServer::SendCT_CMGIFT_REQ(CString strTarget, WORD wGiftID, DWORD dwManagerID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CMGIFT_REQ)
		<< strTarget
		<< wGiftID
		<< dwManagerID;

	Say(pMsg);	
}

void CTServer::SendCT_CMGIFTLIST_REQ(DWORD dwManagerID)
{
	CPacket* pMsg = new CPacket();
	pMsg->SetID(CT_CMGIFTLIST_REQ)
		<< dwManagerID;

	Say(pMsg);
}

void CTServer::SendCT_CMGIFTCHARTUPDATE_REQ(CPacket* pPacket, DWORD dwManagerID)
{
	CPacket* pMsg = new CPacket();
	pMsg->CopyData(pPacket, sizeof(BYTE));
	pMsg->SetID(CT_CMGIFTCHARTUPDATE_REQ)
		<< dwManagerID;

	Say(pMsg);
}
