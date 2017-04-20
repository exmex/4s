/*
 *	UdpSocket.cpp
 *
 */

#include "StdAfx.h"
#include "AtlTime.h"

#ifdef DEF_UDPLOG

#include "TRelaySvr.h"
#include "TRelaySvrModule.h"


CUdpSocket::CUdpSocket()
{
	m_lpLogBuf  = new char [sizeof(_UDPPACKET)+1] ;

	InitializeCriticalSectionAndSpinCount(&m_LogLock, 4000);
}

CUdpSocket::~CUdpSocket()
{
	CLOSESOCKET(m_SendSock);

	delete m_lpLogBuf;

	DeleteCriticalSection(&m_LogLock);
}

/*
 *	Initialize
 */
BOOL CUdpSocket::Initialize(char *pIPAddr,int pPort)
{

	m_SendSock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

	if(INVALID_SOCKET == m_SendSock )
	{
		return FALSE;
	}

	m_LogAddr.sin_family		= AF_INET;
	m_LogAddr.sin_port			= htons(pPort);
	m_LogAddr.sin_addr.s_addr	= inet_addr(pIPAddr);

	return TRUE;
}


/*
 *	SendToLogPacket
 */
void CUdpSocket::SendToLogPacket(_LPUDPPACKET pUdpPacket)
{
	EnterCriticalSection(&m_LogLock);

		memcpy(m_lpLogBuf, pUdpPacket, sizeof(_UDPPACKET));

		m_nSendByte = sendto(m_SendSock, m_lpLogBuf, pUdpPacket->dwSize, 0, (SOCKADDR *)&m_LogAddr, sizeof(m_LogAddr));

		if(SOCKET_ERROR == m_nSendByte) 
		{
			//
			//	Mr Park Add Log
			//
			//	LogEvent("(X) UDP sendto failed with error");
			//
		}

	LeaveCriticalSection(&m_LogLock);

	if(pUdpPacket) 
	{
		delete pUdpPacket;
		pUdpPacket	= 0x00;
	}
}

/*
 *	Log
 */ 
void CUdpSocket::Log(_LPUDPPACKET pUDPPacket)
{
 	pUDPPacket->dwCommand	= LP_LOG;

	int pLogSize = sizeof(_UDPPACKET) - sizeof(pUDPPacket->szPacket ) + sizeof(_LOG_DATA_) - sizeof(((_LPLOG_DATA_)pUDPPacket->szPacket)->szLog);

	switch(	((_LPLOG_DATA_)pUDPPacket->szPacket)->dwFormat )
	{
	case 1:	pLogSize += sizeof(_LOG_CHARBASE_);		break;

	case 2:	pLogSize += sizeof(_LOG_ITEM_);			break;

	case 3:	pLogSize += sizeof(_LOG_SKILL_);		break;

	case 4:	pLogSize += sizeof(_LOG_PET_);			break;

	default:
		pLogSize += sizeof(((_LPLOG_DATA_)pUDPPacket->szPacket)->szLog);
		break;
	}
	pUDPPacket->dwSize	= pLogSize;
	
	SendToLogPacket(pUDPPacket);
}

/*
 *	SendDM_UPDLOG
 */
void CUdpSocket::SendDMLog(_LOG_DATA_ pLogData)
{
	//	Set Current Time
	CTime tNowTime = CTime::GetCurrentTime();
	tNowTime.GetAsDBTIMESTAMP( pLogData.tTimeStamp );

	//
	//	Send Batch Thead
	//
	_AtlModule.SendDM_UDPLOG_REQ(pLogData);
}



/*
 *	Chatting
 */
void CUdpSocket::LogChat(DWORD pAction, CTPlayer *pPlayer, CString strName, CString strTalk)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );

//	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
//	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
//	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
//	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

//	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
//	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level

	if( strName.GetLength() )
	{
		lstrcpy(LogData.szKey[2],	strName			);			//	Target Name
	}

	LogData.dwFormat = LF_TEXT;

	if(strTalk.GetLength() < sizeof(LogData.szLog))
	{
		lstrcpy(LogData.szLog, strTalk);
	}

	SendDMLog( LogData );
}

#endif