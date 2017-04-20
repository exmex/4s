#pragma once

class CTManager : public CTControlSession
{
public:
	BYTE m_bAuthority;
	BYTE m_bUpload;
	DWORD	m_dwID;
	CString m_strID;

public:
	CTManager();
	~CTManager();

public:
	BYTE CheckAuthority(BYTE m_bClass); // Çö½Â·æ ±ÇÇÑÀ» °Ë»çÇÏ¿© °á°ú¸¦ ¸®ÅÏ

public:
	void SendCT_OPLOGIN_ACK(BYTE bRet, BYTE bAuthority,DWORD dwID = 0); // Çö½Â·æ ¸Å´ÏÀú ±ÇÇÑ
	void SendCT_STLOGIN_ACK(BYTE bRet, BYTE bAuthority); // Çö½Â·æ CT_STLOGIN
	void SendCT_MACHINELIST_ACK(LPMAPTMACHINE pMachines);
	void SendCT_GROUPLIST_ACK(LPMAPTGROUP pGroups);
	void SendCT_SVRTYPELIST_ACK(LPMAPTSVRTYPE pType);
	void SendCT_SERVICESTAT_ACK(LPMAPTSVRTEMP pServices);
	void SendCT_SERVICECONTROL_ACK(BYTE bRet);
	void SendCT_SERVICECHANGE_ACK(
		DWORD dwID,
		DWORD dwStatus);
	void SendCT_SERVICEDATA_ACK(
		LPTSVRTEMP pSvr,
		DWORD dwSession,
		DWORD dwUser,
		DWORD dwPing,
		DWORD dwActiveUser);
	void SendCT_SERVICEUPLOADSTART_ACK(BYTE bRet);
	void SendCT_SERVICEUPLOAD_ACK();
	void SendCT_SERVICEUPLOADEND_ACK(BYTE bRet);	
	void SendCT_AUTHORITY_ACK(); // Çö½Â·æ CT_AUTHORITY_ACK
	void SendCT_ACCOUNTINPUT_ACK(BYTE bRet); // Çö½Â·æ CT_ACCOUNTINPUT_ACK
	void SendCT_PLATFORM_ACK(BYTE bMachineID, DWORD dwCPU, DWORD dwMEM, float fNET); // Çö½Â·æ CT_PLATFORM_ACK
	void SendCT_MONSPAWNFIND_ACK(CPacket *pMsg); // Çö½Â·æ CT_MONSPAWNFIND_ACK
	void SendCT_USERPROTECTED_ACK(BYTE bRet);
	void SendCT_SERVICEAUTOSTART_ACK(BYTE _bAutoStart);
	void SendCT_CHATBAN_ACK(BYTE _bRet);
	void SendCT_ITEMFIND_ACK(CPacket* pPacket);
	void SendCT_ITEMSTATE_ACK(CPacket* pPacket);
	void SendCT_CHATBANLIST_ACK(LPMAPBANINFO mapBanInfo);
	void SendCT_CASTLEINFO_ACK(CPacket* pPacket);
	void SendCT_CASTLEGUILDCHG_ACK(CPacket* pPacket);	
	void SendCT_EVENTCHANGE_ACK(BYTE bRet,BYTE bType,LPEVENTINFO pEvent);
	void SendCT_EVENTLIST_ACK(LPMAPEVENTINFO pMapEvent);
	void SendCT_CASHITEMLIST_ACK(LPVTCASHITEM pvCashItem);
	void SendCT_EVENTQUARTERLIST_ACK(CPacket* pPacket);  
	void SendCT_EVENTQUARTERUPDATE_ACK(CPacket* pPacket);
	void SendCT_TOURNAMENTEVENT_ACK(CPacket* pPacket);
	void SendCT_RPSGAMEDATA_ACK(CPacket* pPacket);
	void SendCT_PREVERSIONTABLE_ACK(LPVPATCHFILE pList);
	void SendCT_CMGIFTLIST_ACK(CPacket* pPacket);
	void SendCT_CMGIFT_ACK(BYTE bRet);

};
