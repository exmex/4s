#pragma once

#include "stdafx.h"
#include "HSession.h"


class CGMToolDoc
{
private:
	CGMToolDoc(void);
	 ~CGMToolDoc(void);

public:
	static CGMToolDoc* GetInstance()
	{
		static CGMToolDoc m_Inst;
		return &m_Inst;
	}	


public:
	CHSession	m_session;	

	DWORD		m_dwIP;
	DWORD		m_dwPort;
	CString		m_strUserID;
	CString		m_strPasswd;

	BYTE		m_bAuthority;
	DWORD		m_dwManagerID;
	LISTUSERPOS m_listUSERPOS;
	MAPGROUP	m_mapGroup;
	BYTE		m_bLoadPosList;

	
	
public:

	void SavePos(USERPOS& _UserPos); // return 값이 true면 새로운 값 저장 false 면 기존값 수정
	void LoadPosListFromFile();	
	void SavePosListToFile();
	void SetUserData(CString strID, CString strPW);
	void ClearGroup();
	void ClearPosList();
	void ClearData();
	

public:
	void OnConnect(int nErrorCode);
	void OnClose(int nErrorCode);
	void OnReceive(CPacket * pPacket);
	void SessionStart(CString strIP, DWORD dwPort);
	void Say(CPacket * pPacket);
	void DisconnectServer();

	void OnCT_OPLOGIN_ACK(CPacket * pPacket);
	void OnCT_GROUPLIST_ACK(CPacket * pPacket);
	void OnCT_AUTHORITY_ACK(CPacket * pPacket);
	void OnCT_SERVICEDATA_ACK(CPacket * pPacket);
	void OnCT_PLATFORM_ACK(CPacket * pPacket);
	void OnCT_MONSPAWNFIND_ACK(CPacket * pPacket);
	void OnCT_CHATBAN_ACK(CPacket* pPacket);

	void SendCT_OPLOGIN_REQ();
	void SendCT_USERKICKOUT_REQ(CString strUser);
	void SendCT_USERMOVE_REQ(CString strUser, BYTE bChannel, WORD wMapID, FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ);
	void SendCT_CHARMSG_REQ(CString strName, CString strMsg);
	void SendCT_DISCONNECT_REQ(DWORD _dwID);
	void SendCT_CHATBAN_REQ(CString strName,WORD wMin,CString strReason); // 특정 캐릭터 채팅 금지 요청.
	void SendCT_USERPOSITION_REQ(BYTE bWorld, CString strUser, CString strTarget);
	void SendCT_ANNOUNCEMENT_REQ(DWORD nID, CString strAnnounce);
	void SendCT_MONSPAWNFIND_REQ(BYTE bGroupID, BYTE bChannel, WORD wMapID, WORD wSpawnID);
	void SendCT_MONACTION_REQ(BYTE bGroup, BYTE bChannel, WORD wMapID, DWORD dwMonID, BYTE bAction, DWORD dwTriggerID, DWORD dwHostID, DWORD dwRHID, BYTE bRHType);

};

//extern CGMToolDoc theDoc;