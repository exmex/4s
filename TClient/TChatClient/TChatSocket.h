#pragma once

#define ON_TRECEIVE(x)							case x	: nERROR = On##x( pTSocket, pPacket); break;

#define WM_SESSION_MSG							(WM_USER + 0x3000)
#define MAX_CON									(9)
#define KEY_COUNT								((BYTE) 7)

#include "Packet.h"

class CTChatSocket;
typedef map< SOCKET, CTChatSocket *>			MAPTCHATSOCKET, *LPMAPTCHATSOCKET;


class CTChatSocket
{
public:
	static MAPTCHATSOCKET m_mapSOCKET;

	static CTChatSocket *GetTChatSocket( SOCKET sock);
	static void ReleaseTChatSocket();
	static void InitTChatSocket();

public:
	CPacket m_packet;
	CWnd *m_pOwner;

	SOCKADDR_IN m_target;
	SOCKET m_sock;
	BYTE m_bValid;

	DWORD m_dwSendNumber;
	DWORD m_dwRecvNumber;

	INT64 m_dlCheckFile;
public:
	BYTE m_bSvrID;
	BYTE m_bSVR;
	BYTE m_bUseCrypt;

public:
	void SetOwner( CWnd *pOwner);

	BYTE Start( LPCTSTR strAddr, DWORD dwPort, BYTE bType = SOCK_STREAM);
	BYTE Read( DWORD dwRead);
	BYTE IsValid();

	void Encrypt( CPacket *pPacket);
	BYTE Decrypt( CPacket *pPacket);
	void Say( CPacket *pPacket);
	void Flush();
	void End();

	int CheckMSG();

	BYTE CheckModuleFile(BYTE bAll, INT64 dlCP);

// Message sender
public:
	void SendCS_CHANNELLIST_REQ( BYTE bGroupID);
	void SendCS_CHARLIST_REQ( BYTE bGroupID);

	void SendCS_DISCONNECT_REQ();
	void SendCS_GROUPLIST_REQ();

	void SendCS_LOGIN_REQ(
		CString strUserID,
		CString strPasswd,
		DWORD dwSiteCode);

	void SendCS_START_REQ(
		BYTE bGroupID,
		BYTE bChannel,
		DWORD dwCharID);

	void SendCS_CONNECT_REQ(
		WORD wVersion,
		BYTE bChannel,
		DWORD dwUserID,
		DWORD dwCharID,
		DWORD dwKEY,
		DWORD dwIPAddr,
		WORD wPort);

	void SendCS_CHAT_REQ(
		CString strName,
		BYTE bGroup,
		DWORD dwTargetID,
		CString strNAME,
		CString strMSG,
		BYTE bCountry,
		MAPCHAR & mapCHAR);

	void SendCS_RELAYCHARDATA_REQ(
		WORD wVersion,
		DWORD dwCharID,
		CString strName);

	void SendCT_SERVICEMONITOR_ACK(DWORD dwTick);

	void SendCS_CHECKRELAY_REQ();
	
	void SendCS_HOTSEND_REQ(INT64 dlValue, BYTE bAll);

public:
	CTChatSocket();
	virtual ~CTChatSocket();
};
