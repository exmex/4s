/*
 *	UdpSocket.cpp
 *
 */
#include "StdAfx.h"
#include "AtlTime.h"


#ifdef DEF_UDPLOG

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

		int nSendByte = sendto(m_SendSock, (char*)pUdpPacket, pUdpPacket->dwSize, 0, (SOCKADDR *)&m_LogAddr, sizeof(m_LogAddr));

		if(SOCKET_ERROR == nSendByte) 
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
void CUdpSocket::Log(_LPUDPPACKET pUDPPacket, int pLogSize)
{
	_LPLOG_DATA_ pLogData = (_LPLOG_DATA_)pUDPPacket->szPacket;	

	//	Set Current Time
	CTime tNowTime = CTime::GetCurrentTime();
	tNowTime.GetAsDBTIMESTAMP( pLogData->tTimeStamp );

	//	Command & Size
	pUDPPacket->dwCommand	= LP_LOG;
	pUDPPacket->dwSize		= sizeof(_UDPPACKET) - sizeof(pUDPPacket->szPacket ) + sizeof(_LOG_DATA_) - sizeof(pLogData->szLog) + pLogSize;

	SendToLogPacket(pUDPPacket);
}


/*
 *	LogLogin
 */
void CUdpSocket::LogLogin(CTUser *pUser, DWORD pReturnCode)
{
	if(!pUser) return;

	_LPUDPPACKET pUDPPacket = new _UDPPACKET;
	memset( pUDPPacket, 0x00, sizeof(_UDPPACKET));

	_LPLOG_DATA_ pLogData = (_LPLOG_DATA_)pUDPPacket->szPacket;
    

	pLogData->dwAction	=	LOGLOGIN_LOGIN;	
	lstrcpy( pLogData->szClientIP, inet_ntoa(pUser->m_addr.sin_addr));

	pLogData->dwKey[0]	=	pUser->m_dwID;
	lstrcpy( pLogData->szKey[0], pUser->m_strUserID );

	switch(pReturnCode)
	{
	case LR_SUCCESS:		lstrcpy(pLogData->szKey[4], "SUCCESS");			break;
	case LR_NOUSER:			lstrcpy(pLogData->szKey[4], "NOUSER");			break;
	case LR_INVALIDPASSWD:	lstrcpy(pLogData->szKey[4], "INVALIDPASSWD");	break;
	case LR_DUPLICATE:		lstrcpy(pLogData->szKey[4], "DUPLICATE");		break;
	case LR_VERSION:		lstrcpy(pLogData->szKey[4], "VERSION");			break;	
	case LR_INTERNAL:		lstrcpy(pLogData->szKey[4], "INTERNAL");		break;	
	case LR_BLOCK:			lstrcpy(pLogData->szKey[4], "BLOCK");			break;	
	case LR_IPBLOCK:		lstrcpy(pLogData->szKey[4], "IPBLOCK");			break;
	case LR_NEEDAGREEMENT:	lstrcpy(pLogData->szKey[4], "NEEDAGREEMENT");	break;
	case LR_NEEDWORLDUNIFY:	lstrcpy(pLogData->szKey[4], "NEEDWORLDUNIFY");	break;
	}

	pLogData->dwFormat = LF_TEXT;

	Log( pUDPPacket, lstrlen(pLogData->szLog));
}

/*
 *	LogCharCreate
 */
void CUdpSocket::LogCharCreate(CTUser *pUser, DWORD pReturnCode, BYTE pGroupID, LPTCHARACTER pChar)
{
	if(!pUser || !pChar ) return;

	_LPUDPPACKET pUDPPacket = new _UDPPACKET;
	memset( pUDPPacket, 0x00, sizeof(_UDPPACKET));
	_LPLOG_DATA_ pLogData = (_LPLOG_DATA_)pUDPPacket->szPacket;


	pLogData->dwAction		=	LOGLOGIN_CHARCREATE;	
	pLogData->dwServerID	=	pGroupID;
	
	lstrcpy( pLogData->szClientIP, inet_ntoa(pUser->m_addr.sin_addr));

	pLogData->dwKey[0]	=	pUser->m_dwID;
	lstrcpy( pLogData->szKey[0], pUser->m_strUserID);


	lstrcpy( pLogData->szKey[1], pChar->m_strName);

	pLogData->dwKey[3]	=	pChar->m_bRace;
	pLogData->dwKey[5]	=	pChar->m_bClass;

	pLogData->dwKey[9]	=	pChar->m_bCountry;
	pLogData->dwKey[10]	=	pChar->m_bLevel;
    
	switch(pReturnCode)
	{
	case CR_SUCCESS:		lstrcpy( pLogData->szKey[4], "SUCCESS");	break;
	case CR_NOGROUP:		lstrcpy( pLogData->szKey[4], "NOGROUP");	break;
	case CR_DUPNAME:		lstrcpy( pLogData->szKey[4], "DUPNAME");	break;
	case CR_INVALIDSLOT:	lstrcpy( pLogData->szKey[4], "INVALIDSLOT");break;
	case CR_PROTECTED:		lstrcpy( pLogData->szKey[4], "PROTECTED");	break;	
	case CR_OVERCHAR:		lstrcpy( pLogData->szKey[4], "OVERCHAR");	break;
	case CR_INTERNAL:		lstrcpy( pLogData->szKey[4], "INTERNAL");	break;
	}
	
	pLogData->dwFormat = LF_CHARBASE;

	//	Set the Character Base Information
	_LPLOG_CHARBASE_ pCharBase = (_LPLOG_CHARBASE_)pLogData->szLog;
	
	pCharBase->dwCharID = pChar->m_dwCharID;
	lstrcpy( pCharBase->szName, pChar->m_strName);
	pCharBase->bSlot	= pChar->m_bSlot;
	pCharBase->bLevel	= pChar->m_bLevel;
	pCharBase->bClass	= pChar->m_bClass;
	pCharBase->bRace	= pChar->m_bRace;
	pCharBase->bCountry	= pChar->m_bCountry;
	pCharBase->bSex		= pChar->m_bSex;
	pCharBase->bHair	= pChar->m_bHair;
	pCharBase->bFace	= pChar->m_bFace;
	pCharBase->bBody	= pChar->m_bBody;
	pCharBase->bPants	= pChar->m_bPants;
	pCharBase->bHand	= pChar->m_bHand;
	pCharBase->bFoot	= pChar->m_bFoot;


	Log( pUDPPacket, sizeof(_LOG_CHARBASE_) );
}


/*
 *	LogCharDelete
 *
 */
void CUdpSocket::LogCharDelete(CTUser *pUser, DWORD pReturnCode, BYTE pGroupID, LPTCHARACTER pChar)
{
	if(!pUser || !pChar ) return;

	_LPUDPPACKET pUDPPacket = new _UDPPACKET;
	memset( pUDPPacket, 0x00, sizeof(_UDPPACKET));
	_LPLOG_DATA_ pLogData = (_LPLOG_DATA_)pUDPPacket->szPacket;


	pLogData->dwAction	=	LOGLOGIN_CHARDELETE;	
	pLogData->dwServerID	=	pGroupID;


	lstrcpy( pLogData->szClientIP, inet_ntoa(pUser->m_addr.sin_addr));

	pLogData->dwKey[0]	=	pUser->m_dwID;
	lstrcpy( pLogData->szKey[0], pUser->m_strUserID);

	pLogData->dwKey[1]	= pChar->m_dwCharID;
	lstrcpy( pLogData->szKey[1], pChar->m_strName);

	pLogData->dwKey[3]	=	pChar->m_bRace;
	pLogData->dwKey[5]	=	pChar->m_bClass;

	pLogData->dwKey[9]	=	pChar->m_bCountry;
	pLogData->dwKey[10]	=	pChar->m_bLevel;
	    
	switch(pReturnCode)
	{
	case DR_SUCCESS:		lstrcpy( pLogData->szKey[4], "SUCCESS");		break;
	case DR_INVALIDPASSWD:	lstrcpy( pLogData->szKey[4], "INVALIDPASSWD");	break;
	case DR_NOGROUP:		lstrcpy( pLogData->szKey[4], "NOGROUP");		break;
	case DR_INTERNAL:		lstrcpy( pLogData->szKey[4], "INTERNAL");		break;
	case DR_GUILD:			lstrcpy( pLogData->szKey[4], "GUILD");			break;	
	}

	pLogData->dwFormat = LF_CHARBASE;

	//	Set the Character Base Information
	_LPLOG_CHARBASE_ pCharBase = (_LPLOG_CHARBASE_)pLogData->szLog;

	pCharBase->dwCharID = pChar->m_dwCharID;
	lstrcpy( pCharBase->szName, pChar->m_strName);

	pCharBase->bSlot	= pChar->m_bSlot;

	pCharBase->bLevel	= pChar->m_bLevel;

	pCharBase->bClass	= pChar->m_bClass;

	pCharBase->bRace	= pChar->m_bRace;
	pCharBase->bCountry	= pChar->m_bCountry;
	pCharBase->bSex		= pChar->m_bSex;
	pCharBase->bHair	= pChar->m_bHair;
	pCharBase->bFace	= pChar->m_bFace;
	pCharBase->bBody	= pChar->m_bBody;
	pCharBase->bPants	= pChar->m_bPants;
	pCharBase->bHand	= pChar->m_bHand;
	pCharBase->bFoot	= pChar->m_bFoot;
	
	pCharBase->dwRegion	= pChar->m_dwRegion;

	Log( pUDPPacket, sizeof(_LOG_CHARBASE_) );
}


/*
 *	LogGameStart
 */
void CUdpSocket::LogGameStart(CTUser *pUser, DWORD pReturnCode, BYTE pGroupID, LPTCHARACTER pChar, char *pIP, WORD pPort, BYTE pServerID)
{
	if(!pUser || !pChar ) return;
    
	_LPUDPPACKET pUDPPacket = new _UDPPACKET;
	memset( pUDPPacket, 0x00, sizeof(_UDPPACKET));
	_LPLOG_DATA_ pLogData = (_LPLOG_DATA_)pUDPPacket->szPacket;


	pLogData->dwAction	=	LOGLOGIN_GAMESTART;	
	pLogData->dwServerID=	pGroupID;

	lstrcpy( pLogData->szClientIP, inet_ntoa(pUser->m_addr.sin_addr));

	pLogData->dwKey[0]	=	pUser->m_dwID;
	lstrcpy( pLogData->szKey[0], pUser->m_strUserID);
	
	lstrcpy( pLogData->szKey[1], pChar->m_strName);


	pLogData->dwKey[3]	=	pChar->m_bRace;
	pLogData->dwKey[5]	=	pChar->m_bClass;

	pLogData->dwKey[9]	=	pChar->m_bCountry;
	pLogData->dwKey[10]	=	pChar->m_bLevel;
    
	switch(pReturnCode)
	{
	case SR_SUCCESS:		wsprintf(pLogData->szKey[4], "SUCCESS %s:%d (%d)",pIP, pPort, pServerID);	break;

	case SR_NOSERVER:		lstrcpy( pLogData->szKey[4], "NOSERVER");		break;
	case SR_NOGROUP:		lstrcpy( pLogData->szKey[4], "NOGROUP");		break;
	case SR_INTERNAL:		lstrcpy( pLogData->szKey[4], "INTERNAL");		break;
	}


	pLogData->dwFormat = LF_CHARBASE;

	//	Set the Character Base Information
	_LPLOG_CHARBASE_ pCharBase = (_LPLOG_CHARBASE_)pLogData->szLog;

	pCharBase->dwCharID = pChar->m_dwCharID;
	lstrcpy( pCharBase->szName, pChar->m_strName);
	

	pCharBase->bSlot	= pChar->m_bSlot;

	pCharBase->bLevel	= pChar->m_bLevel;

	pCharBase->bClass	= pChar->m_bClass;

	pCharBase->bRace	= pChar->m_bRace;
	pCharBase->bCountry	= pChar->m_bCountry;
	pCharBase->bSex		= pChar->m_bSex;
	pCharBase->bHair	= pChar->m_bHair;
	pCharBase->bFace	= pChar->m_bFace;
	pCharBase->bBody	= pChar->m_bBody;
	pCharBase->bPants	= pChar->m_bPants;
	pCharBase->bHand	= pChar->m_bHand;
	pCharBase->bFoot	= pChar->m_bFoot;
	
	pCharBase->dwRegion	= pChar->m_dwRegion;


	Log( pUDPPacket, sizeof(_LOG_CHARBASE_) );
}

#endif //	DEF_UDPLOG