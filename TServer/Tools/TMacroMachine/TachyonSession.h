#pragma once

#define ON_TRECEIVE(x)							case x	: nERROR = On##x( pSession, pPacket); break;
#define SESSIONBUF_SIZE							(1024 * 64)
#define KEY_COUNT								((BYTE) 7)

#include "TMacroMachineDoc.h"

class CTachyonSession
{
public:
	static MAPSESSION m_mapSESSION;

	static CTachyonSession *GetSession( SOCKET sock);
	static void ReleaseTachyonSession();
	static void InitTachyonSession();

public:
	//CTachyonWnd *m_pOwner;
	CTMacroMachineDoc *m_pOwner;
	CPacket m_packet;

	SOCKADDR_IN m_target;
	SOCKET m_sock;
	BYTE m_bValid;

	DWORD m_dwSendNumber;
	DWORD m_dwRecvNumber;
	BYTE m_bUseCrypt;
	

public:
	void SetOwner( CTMacroMachineDoc *pOwner);

	BYTE Start( LPCTSTR strAddr, DWORD dwPort, BYTE bType = SOCK_STREAM);
	BYTE Read( DWORD dwRead);
	BYTE IsValid();

	void Encrypt( CPacket *pPacket);
	BOOL Decrypt(CPacket * pPacket);
	BOOL Say( CPacket *pPacket);
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

public:
	WORD	m_wID;
	DWORD	m_dwThreadID;
	HANDLE	m_hEvent;
	HANDLE	m_hSleep;
	HANDLE	m_hThread;
	BOOL	m_bRun;
	BYTE	m_bRelayEnable;
	BYTE	m_bSendMsg;

    CRITICAL_SECTION g_cs;

	QTTYPE	m_qType;

	BOOL	m_bMacroExcute;
	void	ExcuteMacro();
	BOOL	IsExcuteMacro();

	BOOL	EmptyTType();
	void	PushTType(int nError, BYTE bType);
	TTYPE	FrontTType();
	void	PopTType();

	MAPTVALUE	m_mapTValue;
	QTMACRO		m_qTMacro;
	TMACRO		m_tMacro;

	MAPTQPROTOCOL m_mapLoopProtocol;
	void	PushLoopProtocol(TQPROTOCOL Protocol);
	BOOL	EmptyLoopProtocol();

	void	ClearQueue();
	void	SetQueue(QTMACRO qMacro, MAPTVALUE mapTValue);

	CString GetValue(DWORD dwID);

	BOOL	EmptyMacro();
	TMACRO	FrontMacro();
	void	PopMacro();

	BYTE	GetProtocolCount();
	BOOL	EmptyProtocol();
	
	void	PopProtocol();
	TCHILD	GetChild();
	BOOL	IsLoopProtocol();
	TQPROTOCOL	FronProtocol();

	void	UpdateValue(DWORD dwID, CString str);

	BOOL	CheckSelf(DWORD dwID, CString str);
	BOOL	CheckBreak(DWORD dwID);

	BOOL	m_bMapConnect;
	CString m_strMapIP;	
	CString m_strRelayIP;
	WORD	m_wMapPort;
	WORD	m_wRelayPort;
	
	void	ClearMapData();
	BOOL	IsMapConnect();
	BYTE	CheckMapConnect(DWORD dwID);
	void	CheckMapIP(DWORD dwID, DWORD dwIP);
	void	CheckMapPort(DWORD dwID, WORD wPort);
	void	CheckRelayIP(DWORD dwID, DWORD dwIP);	
	void	CheckRelayPort(DWORD dwID, WORD wPort);

	BYTE	m_bConnectType;
	BYTE	GetConnectType();
	void	SetConnectType(BYTE bConnectType);
	void	SetRecvAck(DWORD dwID,BOOL bRecvAck = TRUE);
	BYTE	CheckProtocolType(DWORD dwProtocolID);
	BYTE	CheckRelayConnect(DWORD dwID);
	void	CopyMacro();
};
