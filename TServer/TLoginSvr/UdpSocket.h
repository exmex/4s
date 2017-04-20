/*
 *	UdpSocket.h
 *
 *	For Send to LogServer
 */
#ifndef _UDPSOCKET_H_
#define _UDPSOCKET_H_

 
#include <LogPacket.h>


/*
 *	CUdpSocket
 */
class CUdpSocket 
{
public:
	CUdpSocket();
	~CUdpSocket();

private: 
	SOCKET				m_SendSock;
	SOCKADDR_IN			m_LogAddr;
	CRITICAL_SECTION	m_LogLock; 

	CHAR*	m_lpLogBuf;

	VOID CLOSESOCKET( SOCKET &x )	{	if(x != INVALID_SOCKET) closesocket(x), x = INVALID_SOCKET; }

public:

	//	Base Function
	BOOL Initialize(char *pIPAddr=NULL,int pPort=2000);

	void Log(_LPUDPPACKET pUDPPacket,int pLogSize=0);
	void SendToLogPacket(_LPUDPPACKET);
    
	//	Log Function
	void LogLogin(CTUser *pUser, DWORD pReturnCode);
	
	void LogCharCreate(CTUser *pUser, DWORD pReturnCode, BYTE pGroupID, LPTCHARACTER pChar);
	void LogCharDelete(CTUser *pUser, DWORD pReturnCode, BYTE pGroupID, LPTCHARACTER pChar);

	void LogGameStart(CTUser *pUser, DWORD pReturnCode, BYTE pGroupID, LPTCHARACTER pChar, char *pIP=NULL, WORD pPort=0, BYTE pServerID=0);
};

#endif	//	_UDPSOCKET_H_