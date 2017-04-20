#pragma once

#define ON_TRECEIVE(x)							case x	: nERROR = On##x( pSession, pPacket); break;
#define SESSIONBUF_SIZE							(1024 * 64)
#define KEY_COUNT								((BYTE) 7)

class CTachyonSession
{
public:
	static MAPSESSION m_mapSESSION;

	static CTachyonSession *GetSession( SOCKET sock);
	static void ReleaseTachyonSession();
	static void InitTachyonSession();

public:
	CTachyonWnd *m_pOwner;
	CPacket m_packet;

	SOCKADDR_IN m_target;
	SOCKET m_sock;
	BYTE m_bValid;

	DWORD m_dwSendNumber;
	DWORD m_dwRecvNumber;

public:
	void SetOwner( CTachyonWnd *pOwner);

	BYTE Start( LPCTSTR strAddr, DWORD dwPort, BYTE bType = SOCK_STREAM);
	BYTE Read( DWORD dwRead);
	BYTE IsValid();

	void Encrypt( CPacket *pPacket);

	void Say( CPacket *pPacket);
	void Flush();
	void End();

	int CheckMSG();

public:
	virtual BOOL OnReceive( int nErrorCode);

	virtual void OnConnect( int nErrorCode);
	virtual void OnClose( int nErrorCode);

public:
	CTachyonSession();
	~CTachyonSession();
};
