#pragma once

class CTPlayer :	public CTRelaySession
{
public:
	CTPlayer(void);
	virtual ~CTPlayer(void);

public:
	DWORD	m_dwID;
	DWORD	m_dwKEY;
	DWORD	m_dwGuildID;
	DWORD	m_dwPartyChiefID;
	DWORD	m_dwGeneralID;
	DWORD	m_dwCheckTick;
	DWORD	m_dwTacticsID;

	WORD	m_wPort;
	WORD	m_wPartyID;
	WORD	m_wCorpsID;
	WORD	m_wMapID;
	WORD	m_wUnitID;

	BYTE	m_bCountry;
	BYTE	m_bAidCountry;
	BYTE	m_bGuildDuty;
	BYTE	m_bReconnect;
	BYTE	m_bAuthorized;

	CString m_strNAME;

	__int64 m_nChatBanTime;	
	
	MAPTSTRBYTE	m_mapTPROTECTED;
	TCHATLOG m_chatlog;
	VCHATLOG m_vChatLog;

public:
	WORD	GetPartyID();
	WORD	GetCorpsID();
	DWORD	GetGuildID();	
	DWORD	GetPartyChiefID();

	BYTE	CheckProtected(CString strTarget, BYTE bOption);
	void	AddProtected(CString strName, BYTE bOption);
	void	DelProtected(CString strName);
	BYTE	CheckChat(CString strTalk);
	BYTE	CanTalk(CTPlayer * pTarget, BYTE bChatType);

public:
	// Control server message
	void SendCT_SERVICEMONITOR_REQ(DWORD dwTick, DWORD dwSesson, DWORD dwUser, DWORD dwActiveUser);
	void SendCT_CHATBAN_ACK(BYTE bRet,DWORD dwBanSeq);

	// CS message sender
	void SendCS_RELAYCHARDATA_ACK(BYTE bResult);
	void SendCS_CHAT_ACK(BYTE bGroup, DWORD dwSenderID, CString strName, CString strTalk);
	void SendCS_SYSTEMMSG_ACK(
		BYTE bType,
		WORD wLocalID = 0,
		DWORD dwSecond = 0,
		CString strWinner = NAME_NULL,
		CString strLoser = NAME_NULL,
		BYTE bHour = 0,
		BYTE bMinute = 0,
		CString strPresent = NAME_NULL,
		BYTE bSelect = 0);

	void RelayCorpsMsg(CPacket * pPacket, WORD wMsgID);
	void SendCS_CORPSJOIN_ACK(WORD wCorpsID,WORD wCommander);
	void SendCS_CORPSCMD_ACK(
		WORD wSquad,
		DWORD dwCharID,
		WORD wMapID,
		BYTE bCMD,
		DWORD dwTargetID,
		BYTE bTargetType,
		WORD wPosX,
		WORD wPosZ);
};
