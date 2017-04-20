#pragma once

class CTServer : public CTControlSession
{
public:
	DWORD m_dwID;
	LPTSVRTEMP m_pSVRTEMP;	
	DWORD m_dwRecvTick;
	DWORD m_dwSendTick;

public:
	CTServer();
	~CTServer();

public:
	void SendCT_SERVICEMONITOR_ACK(DWORD dwTick);	
	void SendCT_ANNOUNCEMENT_ACK(CString strAnnounce); // Çö½Â·æ CT_ANNOUNCEMENT_ACK
	void SendCT_USERKICKOUT_ACK(CString strUser); // Çö½Â·æ CT_USERKICKOUT_ACK
	void SendCT_USERMOVE_ACK(CString strUser, BYTE bChannel, WORD wMapID, FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ); // Çö½Â·æ CT_USERMOVE_ACK
	void SendCT_USERPOSITION_ACK(CString strUser, CString strTarget); // Çö½Â·æ CT_USERPOSITION_ACK
	void SendCT_MONSPAWNFIND_ACK(DWORD dwManager, BYTE bChannel, WORD wMapID, WORD wSpawnID); // Çö½Â·æ CT_MONSPAWNFIND_ACK
	void SendCT_MONACTION_ACK(BYTE bChannel, WORD wMapID, DWORD dwMonID, BYTE bAction, DWORD dwTriggerID, DWORD dwHostID, DWORD dwRHID, BYTE bRHType,WORD wSpawnID);
	
	void SendCT_CHATBAN_REQ(CString strName, WORD wMin, DWORD dwBanSeq,DWORD dwManagerID); // Æ¯Á¤ Ä³¸¯ÅÍ Ã¤ÆÃ ±ÝÁö ¿äÃ».
	void SendCT_CHARMSG_ACK(CString strName, CString strMsg);
	void SendCT_SERVICEDATACLEAR_ACK();
	void SendCT_ITEMFIND_REQ(DWORD dwManagerID, WORD wItemID, CString strName);
	void SendCT_ITEMSTATE_REQ(CPacket* pPacket);
	void SendCT_CTRLSVR_REQ();
	void SendCT_CASTLEINFO_REQ(DWORD dwManagerID);
	void SendCT_CASTLEGUILDCHG_REQ(WORD wCastleID,DWORD dwDefGuildID,DWORD dwAtkGuildID,DWORD dwManagerID,__time64_t tTime);
	void SendCT_CASTLEENABLE_REQ(BYTE bType,BYTE bStatus, DWORD dwSecond);
	void SendCT_EVENTUPDATE_REQ(BYTE bEventID,WORD wValue,LPEVENTINFO pEVENT);
	void SendCT_EVENTMSG_REQ(BYTE bEventID, BYTE bEventMsgType,CString strMsg);
	void SendCT_CASHSHOPSTOP_REQ(BYTE bType);
	void SendCT_CASHITEMSALE_REQ(DWORD dwIndex,WORD wValue,LPVTCASHITEMSALE pvSaleItem);
	void SendCT_EVENTQUARTERLIST_REQ(CPacket* pPacket);  
	void SendCT_EVENTQUARTERUPDATE_REQ(CPacket* pPacket);
	void SendCT_TOURNAMENTEVENT_REQ(CPacket* pPacket);
	void SendCT_HELPMESSAGE_REQ(CPacket* pPacket);
	void SendCT_RPSGAMEDATA_REQ(CPacket* pPacket);
	void SendCT_RPSGAMECHANGE_REQ(CPacket* pPacket);
	void SendCT_CMGIFT_REQ(CString strTarget, WORD wGiftID, DWORD dwManagerID);
	void SendCT_CMGIFTLIST_REQ(DWORD dwManagerID);
	void SendCT_CMGIFTCHARTUPDATE_REQ(CPacket* pPacket, DWORD dwManagerID);
};
