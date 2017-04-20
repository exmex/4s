/*
 *	UdpSocket.cpp
 *
 */

#include "StdAfx.h"
#include "AtlTime.h"

#ifdef DEF_UDPLOG

#include "TMapSvr.h"
#include "TMapSvrModule.h"


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
 *	LogMoney
 */
void CUdpSocket::LogMoney(DWORD pAction, CTPlayer *pPlayer, CTMonster *pMon, __int64 pMoney, LPQUESTTEMP pQUEST, BYTE bIsWorld)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction		=	pAction;
	LogData.dwServerID		=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level

	if(pMon)
	{	
		LogData.dwKey[2]	=	pMon->m_pMON->m_wID;			//	Monster	ID
	}

	LogData.dwKey[5]		=	pMoney;							//	Get Money   
	wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",			//	Now Money
											pPlayer->m_dwGold,		
											pPlayer->m_dwSilver, 
											pPlayer->m_dwCooper );

	if(pQUEST)
	{
		LogData.dwKey[6]	=	pQUEST->m_dwQuestID;			//	Quest
	}

	// Is World 	  
	LogData.dwFormat = LF_TEXT;

	wsprintf(LogData.szLog, "%d", bIsWorld);

	SendDMLog( LogData );
}

/*
 *	LogItem - NPC
 */
void CUdpSocket::LogItemByNPC(DWORD pAction, CTPlayer *pPlayer, CTNpc * pNpc, CTItem *pItem, int pMoney, LPQUESTTEMP pQUEST)
{
	if(!pPlayer ) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction		=	pAction;
	LogData.dwServerID		=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	if(pNpc)
	{
		LogData.dwKey[2]	=	pNpc->m_wID;					//	Npc ID
	}
	
	LogData.dwKey[5]		=	pMoney;							//	Get Money   

	wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",			//	Now Money
											pPlayer->m_dwGold,		
											pPlayer->m_dwSilver, 
											pPlayer->m_dwCooper );

	if(pQUEST)
	{
		LogData.dwKey[6]	=	pQUEST->m_dwQuestID;			//	Quest ID
	}

	//
	//	ITEM INFO
	//
	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			//	Item ID
		LogData.dwKey[7]		=	pItem->m_bCount;			//	Item Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		LogData.dwFormat = LF_ITEM;

		_LPLOG_ITEM_ pLogItem = (_LPLOG_ITEM_)LogData.szLog;
	
		pLogItem->dlID			=	pItem->m_dlID;				//	Unique Number
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호

		pLogItem->wItemID		=	pItem->m_wItemID;			//	Item Index

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성

		int	nOrder = 0;
		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;

			nOrder ++;
		}
		
		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;						//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;	//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;	//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;	//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}


/*
 *	LogItem - Monster
 */
void CUdpSocket::LogItemByMonster(DWORD pAction, CTPlayer *pPlayer, CTMonster *pMon, CTItem *pItem, __int64 pMoney)
{
	if(!pPlayer || !pItem ) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level

	if(pMon)
	{
		LogData.dwKey[2]	=	pMon->m_pMON->m_wID;			//	NPC ID
	}

	if(pMoney)
	{
		LogData.dwKey[5]		=	pMoney;						//	Get Money   
		wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",		//	Now Money
												pPlayer->m_dwGold,		
												pPlayer->m_dwSilver, 
												pPlayer->m_dwCooper );	
	}
	
	//
	//	ITEM INFO
	//
	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			//	Item Template ID
		LogData.dwKey[7]		=	pItem->m_bCount;			//	Item Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		LogData.dwFormat = LF_ITEM;

		_LPLOG_ITEM_ pLogItem = (_LPLOG_ITEM_)LogData.szLog;
		
		pLogItem->dlID			=	pItem->m_dlID;				//	Unique Number
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호

		pLogItem->wItemID		=	pItem->m_wItemID;			//	아이템 INDEX

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성	    

		int	nOrder = 0;

		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;

			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;								//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;			//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;			//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;			//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}


/*
 *	LogItem - Monster
 */
void CUdpSocket::LogItemByMonster(DWORD pAction, CTPlayer *pPlayer, int pMonTempID, CTItem *pItem )
{
	if(!pPlayer || !pItem ) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	


	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME );			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	LogData.dwKey[2]		=	pMonTempID;						//	Monster
																		
	//
	//	ITEM INFO
	//
	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			//	Item Template ID	
		LogData.dwKey[7]		=	pItem->m_bCount;			//	Item Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		LogData.dwFormat		=	LF_ITEM;

		_LPLOG_ITEM_ pLogItem = (_LPLOG_ITEM_)LogData.szLog;
		
		pLogItem->dlID			=	pItem->m_dlID;				//	Unique Number
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호

		pLogItem->wItemID		=	pItem->m_wItemID;			//	아이템 INDEX

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;
		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성	    

		int	nOrder = 0;
		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;

			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;							//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;		//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;		//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;		//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}

/*
 *	LogItemUpgrade
 */
void CUdpSocket::LogItemUpgrade(DWORD pAction, CTPlayer *pPlayer, CTItem *pItem, int pUpLevel)
{
	if(!pPlayer || !pItem ) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );

	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level

	//
	//	ITEM INFO
	//
	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			//	Item Template
		LogData.dwKey[7]		=	pItem->m_bCount;			//	Item Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		//	Upgrade Level
		if(pUpLevel)
		{
			LogData.dwKey[6]	=	pUpLevel;
			wsprintf( LogData.szKey[6], "%d -> %d", pItem->m_bLevel - pUpLevel, pItem->m_bLevel );
		}

		LogData.dwFormat = LF_ITEM;

		_LPLOG_ITEM_ pLogItem = (_LPLOG_ITEM_)LogData.szLog;

		pLogItem->dlID			=	pItem->m_dlID;				//	Unique Number
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호
		pLogItem->wItemID		=	pItem->m_wItemID;			//	Item Index

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성

		int	nOrder = 0;

		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;

			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;						//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;	//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;	//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;	//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}

/*
 *	LogItemTrade
 */
void CUdpSocket::LogItemTrade(DWORD pAction, CTPlayer *pPlayer, CTItem *pItem, CString pTargetName)
{
	if(!pPlayer || !pItem ) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level

	//	Target Name
	if( pTargetName.GetLength() && pTargetName.GetLength() < sizeof(LogData.szKey[1]) )
	{
		lstrcpy(LogData.szKey[2], pTargetName);
	}
	
	//
	//	ITEM INFO
	//
	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			//	Item Template
		LogData.dwKey[7]		=	pItem->m_bCount;			//	Item Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		LogData.dwFormat = LF_ITEM;

		_LPLOG_ITEM_ pLogItem = (_LPLOG_ITEM_)LogData.szLog;
		
		pLogItem->dlID			=	pItem->m_dlID;				//	Unique Number
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호

		pLogItem->wItemID		=	pItem->m_wItemID;			//	아이템 INDEX

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성

		int	nOrder = 0;

		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;

			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;						//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;	//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;	//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;	//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}


/*
 *	LogMoneyTrade
 */
void CUdpSocket::LogMoneyTrade(DWORD pAction, CTPlayer *pPlayer, CString pTargetName, __int64 pMoney,int pCost)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	//	Target Name
	if( pTargetName.GetLength() && pTargetName.GetLength() < sizeof(LogData.szKey[1]) )
	{
		lstrcpy(LogData.szKey[2], pTargetName);
	}

	LogData.dwKey[5]		=	pMoney;							//	Get Money   
	wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",			//	Now Money
											pPlayer->m_dwGold,		
											pPlayer->m_dwSilver, 
											pPlayer->m_dwCooper );	
    
	LogData.dwKey[6]		=	pCost;


	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

/*
 *	LogItemByStore
 */
void CUdpSocket::LogItemByStore(DWORD pAction, CTPlayer *pPlayer, CString pTargetName,	CTItem *pItem, __int64 pMoney, BYTE bCount)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level



	//	Target Name
	if( pTargetName.GetLength() && pTargetName.GetLength() < sizeof(LogData.szKey[1]) )
	{
		lstrcpy(LogData.szKey[2], pTargetName);
	}

	LogData.dwKey[5]		=	pMoney;							//	Get Money  
	wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",			//	Now Money
											pPlayer->m_dwGold,		
											pPlayer->m_dwSilver, 
											pPlayer->m_dwCooper );

	//
	//	ITEM INFO
	//
	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			//	Item Base ID
		LogData.dwKey[7]		=	bCount;
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		LogData.dwFormat		= LF_ITEM;

		_LPLOG_ITEM_ pLogItem = (_LPLOG_ITEM_)LogData.szLog;
		
		pLogItem->dlID			=	pItem->m_dlID;				//	Unique Number
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호

		pLogItem->wItemID		=	pItem->m_wItemID;			//	아이템 INDEX

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성

		int	nOrder = 0;
		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;

			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;						//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;	//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;	//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;	//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}


/*
 *	Log Exp - Monster
 */
void CUdpSocket::LogExpByMonster(DWORD pAction, CTPlayer *pPlayer, CTMonster *pMon, DWORD dwGain, BYTE bIsWorld)
{
	if(!pPlayer || !pMon ) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	


	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level



	if(pMon)
	{
		LogData.dwKey[2]	=	pMon->m_pMON->m_wID;			//	Npc ID
	}

	LogData.dwKey[6]		=	dwGain;							// Exp
	wsprintf(LogData.szKey[6],"Now Exp : %u", pPlayer->m_dwEXP);// Now Exp

	// Is World 	  
	LogData.dwFormat = LF_TEXT;

	wsprintf((char*)LogData.szLog, "%d", bIsWorld);

	SendDMLog( LogData );
}

/*
 *	Log Skill
 */
void CUdpSocket::LogSkillAct(DWORD pAction, CTPlayer *pPlayer, CTSkill *pSkill, __int64 pMoney)
{
	if(!pPlayer || !pSkill) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
		
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	LogData.dwKey[3]		=	pSkill->GetID();				// Skill

	
	LogData.dwKey[5]		=	pMoney;							//	Out Money 
	wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",			//	Now Money
											pPlayer->m_dwGold,		
											pPlayer->m_dwSilver, 
											pPlayer->m_dwCooper );

	LogData.dwKey[6]		=	pSkill->m_bLevel;				//	SkillLevel

	//
	//	SKILL INFO
	//
	LogData.dwFormat = LF_SKILL;

	_LPLOG_SKILL_ pLogSkill = (_LPLOG_SKILL_)LogData.szLog;

	
	pLogSkill->dwCharID		=	pPlayer->m_dwID;				//	Character ID
	lstrcpy(pLogSkill->szName,	pPlayer->m_strNAME	);			//	Character Name
	pLogSkill->wSkill		=	pSkill->GetID();				//	스킬 ID
	pLogSkill->bLevel		=	pSkill->m_bLevel;				//	스킬 Level

	SendDMLog( LogData );
}

/*
 *	Log PET
 */
void CUdpSocket::LogPet(DWORD pAction, CTPlayer *pPlayer, WORD wPeID, CString strName, __int64 dwTime )
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	LogData.dwKey[3]		=	wPeID;							// Pet
	if(strName)
	{
		lstrcpy( LogData.szKey[3], strName );					//	Pet Name
	}

	LogData.dwKey[6]		=	dwTime;							// Time

	//
	//	ITEM INFO
	//
	LogData.dwFormat = LF_PET;


	_LPLOG_PET_ pLogPet = (_LPLOG_PET_)LogData.szLog;

	
	pLogPet->dwCharID		=	pPlayer->m_dwID;				//	Character ID
	lstrcpy(pLogPet->szName,	pPlayer->m_strNAME	);			//	Character Name
	pLogPet->m_wPetID		=	wPeID;							//  Pet ID
	if(strName)
	{
        lstrcpy(pLogPet->szPetName, strName				);		//  Pet Name
	}
	pLogPet->m_ldwTime		=	dwTime;							//	Pet Tiem	

	SendDMLog( LogData );
}


/*
 *	Log LevelUp
 */
void CUdpSocket::LogLevelUp(DWORD pAction, CTPlayer *pPlayer, BYTE m_bLevel)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level

	LogData.dwKey[6]		=	m_bLevel;						//	SkillLevel
	
	SendDMLog( LogData );
}

/*
 *	Log Monster
 */
void CUdpSocket::LogMonster(DWORD pAction, CTPlayer *pPlayer, CTMonster *pMon)
{
	if(!pMon) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();	

	//
	//	Player
	//
	if(pPlayer)
	{
		lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	//	Connect IP Address

		LogData.dwKey[0]		=	pPlayer->m_dwUserID;					//	User ID

		LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq
		lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME			);			//	Character Name

		LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);					//	Country
		LogData.dwKey[10]		=	pPlayer->m_bLevel;						//	Level
	}

	//
	//	Monster
	//
	if( pMon )
	{
		LogData.wMapID			=	pMon->m_wMapID;					// Map ID
		LogData.nX				=	(int)pMon->m_fPosX;				//		X
		LogData.nY				=	(int)pMon->m_fPosY;				//		Y
		LogData.nZ				=	(int)pMon->m_fPosZ;				//		Z

		LogData.dwKey[2]		=	pMon->m_pMON->m_wID;			// Mon ID
	}

	SendDMLog( LogData );
}


/*
 *	Log Die
 */
void CUdpSocket::LogUserDie(DWORD pAction, CTPlayer *pPlayer, BYTE m_bType)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction		=	pAction;
	LogData.dwServerID		=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	LogData.dwKey[6]		=	m_bType;						//	Die Type

	SendDMLog( LogData );
}

/*
 *	Log Quest
 */
void CUdpSocket::LogQuest(DWORD pAction, CTPlayer *pPlayer, DWORD dwQuestID, DWORD dwGain)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );
		
	LogData.wMapID			=	pPlayer->m_wMapID;					//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;				//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;				//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;				//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;				//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;					//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);				//	Character Name
	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);				//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;					//	Level



	LogData.dwKey[5]		=	dwGain;								//	Exp
	wsprintf( LogData.szKey[5], "Now Exp : %u", pPlayer->m_dwEXP);	//	Now Exp

	LogData.dwKey[6]		=	dwQuestID;							//	Quest

	SendDMLog( LogData );
}


/*
 *	Log ENTER Map
 */
void CUdpSocket::LogEnterMap(DWORD pAction, CTPlayer *pPlayer)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[3]		=	pPlayer->m_bRace;
	LogData.dwKey[5]		=	pPlayer->m_bClass;

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level



	//
	wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",			//	Now Money
											pPlayer->m_dwGold,		
											pPlayer->m_dwSilver, 
											pPlayer->m_dwCooper );	

	
	wsprintf( LogData.szKey[6], "Now Exp : %u", pPlayer->m_dwEXP);	// Now Exp

	//
	//	Char Base
	//
	LogData.dwFormat = LF_CHARBASE;

	_LPLOG_CHARBASE_ pCharBase = (_LPLOG_CHARBASE_)LogData.szLog;
	
	pCharBase->dwCharID = pPlayer->m_dwID;
	lstrcpy( pCharBase->szName, pPlayer->m_strNAME);
	pCharBase->bSlot	= 0;
	pCharBase->bLevel	= pPlayer->m_bLevel;
	pCharBase->bClass	= pPlayer->m_bClass;
	pCharBase->bRace	= pPlayer->m_bRace;
	pCharBase->bCountry	= pPlayer->m_bCountry;
	pCharBase->bSex		= pPlayer->m_bSex;
	pCharBase->bHair	= pPlayer->m_bHair;
	pCharBase->bFace	= pPlayer->m_bFace;
	pCharBase->bBody	= pPlayer->m_bBody;
	pCharBase->bPants	= pPlayer->m_bPants;
	pCharBase->bHand	= pPlayer->m_bHand;
	pCharBase->bFoot	= pPlayer->m_bFoot;

	SendDMLog( LogData );	
}

/*
 *	Log LocalOccupy
 */
void CUdpSocket::LogLocalOccupy(DWORD pAction, BYTE bType, WORD wLocalID, BYTE bCountry, DWORD dwGuildID)
{
	_LOG_DATA_ LogData;

	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	LogData.dwKey[0]	=	wLocalID;

	LogData.dwKey[2]	=	bCountry;

	LogData.dwKey[3]	=	dwGuildID;

	LogData.dwKey[6]	=	bType;

	SendDMLog( LogData );	
}

/*
 *	Log LocalEnable
 */
void CUdpSocket::LogLocalEnable(DWORD pAction, WORD wLocalID, BYTE bStatus)
{
	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	LogData.dwKey[0]	=	wLocalID;

	LogData.dwKey[6]	=	bStatus;

	SendDMLog( LogData );
}

/*
 *
 */
void CUdpSocket::LogCabinet(DWORD pAction, CTPlayer *pPlayer, BYTE bCabinetID, CTItem *pItem, BYTE bCount, int pMoney)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name
	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level

	LogData.dwKey[2]		=	bCabinetID;						//	Cabinet ID	
	
	if(pMoney)
	{
        LogData.dwKey[5]		=	pMoney;					//	Out Money 

		wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",		//	Now Money
												pPlayer->m_dwGold,		
												pPlayer->m_dwSilver, 
												pPlayer->m_dwCooper );	
	}

	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			// Item
		LogData.dwKey[7]		=	bCount;						//	Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		//
		//	ITEM INFO
		//
		LogData.dwFormat		= LF_ITEM;

		_LPLOG_ITEM_ pLogItem	= (_LPLOG_ITEM_)LogData.szLog;
		
		pLogItem->dlID			=	pItem->m_dlID;				//	Unique Number
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호

		pLogItem->wItemID		=	pItem->m_wItemID;			//	아이템 INDEX

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성

		int	nOrder = 0;

		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;

			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;						//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;	//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;	//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;	//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
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


	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	if(pAction == LOGMAP_CHATWISPER || pAction == LOGMAP_CHATGUILD)
	{
		lstrcpy(LogData.szKey[2],	strName			);					//	Target Name
	}

	LogData.dwFormat = LF_TEXT;


	if(strTalk.GetLength() < sizeof(LogData.szLog))
	{
		lstrcpy(LogData.szLog, strTalk);
	}

	SendDMLog( LogData );
}

/*
 *	LogGuild
 */
void CUdpSocket::LogGuild(DWORD pAction, CTPlayer *pPlayer,DWORD pTargetID,CString pTargetName, DWORD pGuildID, CString strGuildName, BYTE bRet )
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	


	LogData.wMapID			=	pPlayer->m_wMapID;						//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;					//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;					//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;					//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;					// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME			);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	//	Target
	LogData.dwKey[2]		=	pTargetID;								//	Character Seq
	lstrcpy(LogData.szKey[2],	pTargetName		);						//	Character Name

	//	4: Guild
	LogData.dwKey[4]		=	pGuildID;
	lstrcpy(LogData.szKey[4],	strGuildName	);

	//	Return Value
	switch( bRet )
	{
	case GUILD_SUCCESS				: lstrcpy(LogData.szLog,	"SUCCESS"	);				break;        
	case GUILD_ALREADYGUILDNAME		: lstrcpy(LogData.szLog,	"ALREADYGUILDNAME"	);		break;
	case GUILD_NOTCHIEF				: lstrcpy(LogData.szLog,	"NOTCHIEF"	);				break;
	case GUILD_ALREADYMEMBER		: lstrcpy(LogData.szLog,	"ALREADYMEMBER"	);			break;
	case GUILD_NOTMEMBER			: lstrcpy(LogData.szLog,	"NOTMEMBER"	);				break;
	case GUILD_HAVEGUILD			: lstrcpy(LogData.szLog,	"HAVEGUILD"	);				break;
	case GUILD_NOTFOUND				: lstrcpy(LogData.szLog,	"NOTFOUND"	);				break;
	case GUILD_ESTABLISH_ERR		: lstrcpy(LogData.szLog,	"ESTABLISH_ERR"	);			break;
	case GUILD_DISORGANIZATION_ERR	: lstrcpy(LogData.szLog,	"DISORGANIZATION_ERR");		break;
	case GUILD_LEAVE_SELF			: lstrcpy(LogData.szLog,	"LEAVE_SELF");				break;
	case GUILD_LEAVE_KICK			: lstrcpy(LogData.szLog,	"LEAVE_KICK");				break;
	case GUILD_LEAVE_DISORGANIZATION: lstrcpy(LogData.szLog,	"LEAVE_DISORGANIZATION");	break;
	case GUILD_JOIN_SUCCESS			: lstrcpy(LogData.szLog,	"JOIN_SUCCESS");			break;
	case GUILD_JOIN_DENY			: lstrcpy(LogData.szLog,	"JOIN_DENY");				break;
	case GUILD_NODUTY				: lstrcpy(LogData.szLog,	"NODUTY");					break;
	default : break;
	}

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

/*
 *	LogGuildDutyPeer
 */
void CUdpSocket::LogGuildDutyPeer(DWORD pAction, CTPlayer *pPlayer,DWORD pTargetID,CString pTargetName, DWORD pGuildID, CString strGuildName, int pData, BYTE bRet)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	//	Target
	LogData.dwKey[2]		=	pTargetID;						//	Target Seq
	lstrcpy(LogData.szKey[2],	pTargetName		);				//	Target Name

	//	4: Guild
	LogData.dwKey[4]		=	pGuildID;
	lstrcpy(LogData.szKey[4],	strGuildName	);

	//	5:	Key
	LogData.dwKey[5]	=	pData;

	switch( pAction )
	{
	case	LOGMAP_GUILDDUTY	:	
		{			
			switch(pData)
			{
			case 0	:	lstrcpy(LogData.szKey[5], "-"			);	break;
			case 1	:	lstrcpy(LogData.szKey[5], "SubMaster"	);	break;
			case 2	:	lstrcpy(LogData.szKey[5], "Master"		);	break;
			}
		}		
		break;

	case	LOGMAP_GUILDPEER	:	
		{
			switch(pData)
			{
			case 0	:	lstrcpy(LogData.szKey[5],	"-"			);	break;
			case 1	:	lstrcpy(LogData.szKey[5],	"남작"		);	break;
			case 2	:	lstrcpy(LogData.szKey[5],	"자작"		);	break;
			case 3	:	lstrcpy(LogData.szKey[5],	"백작"		);	break;
			case 4	:	lstrcpy(LogData.szKey[5],	"후작"		);	break;
			case 5	:	lstrcpy(LogData.szKey[5],	"공작"		);	break;
			}
		}
		break;

	default : break;
	}

	//	Return Value
	switch( bRet )
	{
	case GUILD_SUCCESS				: lstrcpy(LogData.szLog,	"SUCCESS"	);				break;        
	case GUILD_ALREADYGUILDNAME		: lstrcpy(LogData.szLog,	"ALREADYGUILDNAME"	);		break;
	case GUILD_NOTCHIEF				: lstrcpy(LogData.szLog,	"NOTCHIEF"	);				break;
	case GUILD_ALREADYMEMBER		: lstrcpy(LogData.szLog,	"ALREADYMEMBER"	);			break;
	case GUILD_NOTMEMBER			: lstrcpy(LogData.szLog,	"NOTMEMBER"	);				break;
	case GUILD_HAVEGUILD			: lstrcpy(LogData.szLog,	"HAVEGUILD"	);				break;
	case GUILD_NOTFOUND				: lstrcpy(LogData.szLog,	"NOTFOUND"	);				break;
	case GUILD_ESTABLISH_ERR		: lstrcpy(LogData.szLog,	"ESTABLISH_ERR"	);			break;
	case GUILD_DISORGANIZATION_ERR	: lstrcpy(LogData.szLog,	"DISORGANIZATION_ERR");		break;
	case GUILD_LEAVE_SELF			: lstrcpy(LogData.szLog,	"LEAVE_SELF");				break;
	case GUILD_LEAVE_KICK			: lstrcpy(LogData.szLog,	"LEAVE_KICK");				break;
	case GUILD_LEAVE_DISORGANIZATION: lstrcpy(LogData.szLog,	"LEAVE_DISORGANIZATION");	break;
	case GUILD_JOIN_SUCCESS			: lstrcpy(LogData.szLog,	"JOIN_SUCCESS");			break;
	case GUILD_JOIN_DENY			: lstrcpy(LogData.szLog,	"JOIN_DENY");				break;
	case GUILD_NODUTY				: lstrcpy(LogData.szLog,	"NODUTY");					break;
	default : break;
	}

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

/*
 *	LogGuildDisorganization
 */
void CUdpSocket::LogGuildDisorganization(DWORD pAction, CTPlayer *pPlayer, 	BYTE pDisorg)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );

	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name
	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level




	//	4: Guild
	LogData.dwKey[4]		=	pPlayer->m_dwGuildID;
	lstrcpy(LogData.szKey[4],	pPlayer->m_strGuildName	);

	//	5
	switch(pDisorg)
	{
	case 0	:	lstrcpy(LogData.szKey[5],	"normal"	);	break;
	case 1	:	lstrcpy(LogData.szKey[5],	"apply"		);	break;
	}

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

/*
 *	LogGuildExpMoeny
 */
void CUdpSocket::LogGuildExpMoeny(DWORD pAction, CTPlayer *pPlayer,int pExp, int pGold, int pSilver, int pCooper, int pPvPoint, BYTE bRet)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;

	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	


	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name
	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	//	4: Guild
	LogData.dwKey[4]		=	pPlayer->m_dwGuildID;
	lstrcpy(LogData.szKey[4],	pPlayer->m_strGuildName	);

	//	5:	Moeny
	LogData.dwKey[5]	=	pGold*1000000 +	pSilver * 1000 +	pCooper;

	//	6: Exp
	LogData.dwKey[6]	=	pExp;

	//	7: PvPoint
	LogData.dwKey[7]	=	pPvPoint;

	//	Return Value
	switch( bRet )
	{
	case GUILD_SUCCESS				: lstrcpy(LogData.szLog,	"SUCCESS"	);				break;        
	case GUILD_ALREADYGUILDNAME		: lstrcpy(LogData.szLog,	"ALREADYGUILDNAME"	);		break;
	case GUILD_NOTCHIEF				: lstrcpy(LogData.szLog,	"NOTCHIEF"	);				break;
	case GUILD_ALREADYMEMBER		: lstrcpy(LogData.szLog,	"ALREADYMEMBER"	);			break;
	case GUILD_NOTMEMBER			: lstrcpy(LogData.szLog,	"NOTMEMBER"	);				break;
	case GUILD_HAVEGUILD			: lstrcpy(LogData.szLog,	"HAVEGUILD"	);				break;
	case GUILD_NOTFOUND				: lstrcpy(LogData.szLog,	"NOTFOUND"	);				break;
	case GUILD_ESTABLISH_ERR		: lstrcpy(LogData.szLog,	"ESTABLISH_ERR"	);			break;
	case GUILD_DISORGANIZATION_ERR	: lstrcpy(LogData.szLog,	"DISORGANIZATION_ERR");		break;
	case GUILD_LEAVE_SELF			: lstrcpy(LogData.szLog,	"LEAVE_SELF");				break;
	case GUILD_LEAVE_KICK			: lstrcpy(LogData.szLog,	"LEAVE_KICK");				break;
	case GUILD_LEAVE_DISORGANIZATION: lstrcpy(LogData.szLog,	"LEAVE_DISORGANIZATION");	break;
	case GUILD_JOIN_SUCCESS			: lstrcpy(LogData.szLog,	"JOIN_SUCCESS");			break;
	case GUILD_JOIN_DENY			: lstrcpy(LogData.szLog,	"JOIN_DENY");				break;
	case GUILD_NODUTY				: lstrcpy(LogData.szLog,	"NODUTY");					break;
	default : break;
	}

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

/*
 *	LogGuildCabinet
 */
void CUdpSocket::LogGuildCabinet(DWORD pAction, CTPlayer *pPlayer, DWORD pGuildID, CString strGuildName,BYTE bCabinetID, CTItem *pItem, BYTE bCount, DWORD dwMoney)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;

	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	


	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name
	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level

	LogData.dwKey[2]		=	bCabinetID;						//	Cabinet ID	
	
	LogData.dwKey[4]		=	pPlayer->m_dwGuildID;			//	Guild
	lstrcpy(LogData.szKey[4],	pPlayer->m_strGuildName	);
    
	if(dwMoney)
	{
        LogData.dwKey[5]		=	dwMoney;					//	Out Money 
		wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",		//	Now Money
												pPlayer->m_dwGold,		
												pPlayer->m_dwSilver, 
												pPlayer->m_dwCooper );	
	}

	if(pItem)
	{		
		LogData.dwKey[3]		=	pItem->m_wItemID;			//	Item
		LogData.dwKey[7]		=	bCount;						//	Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		//
		//	ITEM INFO
		//
		LogData.dwFormat		= LF_ITEM;

		_LPLOG_ITEM_ pLogItem	= (_LPLOG_ITEM_)LogData.szLog;

		pLogItem->dlID			=	pItem->m_dlID;				//	Unique Number
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호
		pLogItem->wItemID		=	pItem->m_wItemID;			//	아이템 INDEX


		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성

		int	nOrder = 0;

		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;

			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;						//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;	//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;	//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;	//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}

/*
 *	LogCashItem
 */
void CUdpSocket::LogCashItem(DWORD pAction, CTPlayer *pPlayer,  DWORD pTargetID, CString pTargetName, CTItem *pItem,int pCashID, int pCash, int pBonus )
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name
	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level
    

	//	Target
	if( pTargetName.GetLength() && pTargetName.GetLength() < sizeof(LogData.szKey[2]) )
	{
		LogData.dwKey[2]		=	pTargetID;							
		lstrcpy(LogData.szKey[2], pTargetName);
	}

	LogData.dwKey[5]		=	pCash + pBonus;					//	Cash
	wsprintf( LogData.szKey[5],"Cash=%u Bonus=%u",pCash,pBonus);//	Cash
	LogData.dwKey[6]		=	pCashID;						//	Cash ID

	//
	//	ITEM INFO
	//
	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			// Item
		LogData.dwKey[7]		=	pItem->m_bCount;			//	Item Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		LogData.dwFormat = LF_ITEM;

		_LPLOG_ITEM_ pLogItem = (_LPLOG_ITEM_)LogData.szLog;
		
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호

		pLogItem->wItemID		=	pItem->m_wItemID;			//	아이템 INDEX

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성

		int	nOrder = 0;
		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;

			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;						//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;	//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;	//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;	//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}


void CUdpSocket::LogCashCabinetBuy(DWORD pAction, CTPlayer *pPlayer, int pCashID, __time64_t pDate, int pCash, int pBonus,  int pResult)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );
		
	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z
	
	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);			//	Character Name
	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	TIMESTAMP_STRUCT	TempDate;
	__TIMETODB(pDate, TempDate);
	wsprintf( LogData.szKey[4], "%04d-%02d-%02d", TempDate.year, TempDate.month, TempDate.day);	//	Date
	
	LogData.dwKey[5]		=	pCash + pBonus;					//	Cash
	wsprintf( LogData.szKey[5],"Cash=%u Bonus=%u",pCash,pBonus);//	Cash
	LogData.dwKey[6]		=	pCashID;						//	Cash ID
    
	LogData.dwFormat = LF_TEXT;

	wsprintf( LogData.szLog,"%d", pResult );

	SendDMLog( LogData );
}


/*
 *	LogItem - NPC
 */
void CUdpSocket::LogTeleport(DWORD pAction, CTPlayer *pPlayer, CTNpc * pNpc, int pPrice, int pPortalID)
{
	if(!pPlayer ) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction		=	pAction;
	LogData.dwServerID		=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	if(pNpc)
	{
		LogData.dwKey[2]	=	pNpc->m_wID;					//	Npc ID
	}

	LogData.dwKey[4]		=	pPortalID;						//	Portal ID

	LogData.dwKey[5]		=	pPrice;							//	Get Money   

	wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",			//	Price
											pPlayer->m_dwGold,		
											pPlayer->m_dwSilver, 
											pPlayer->m_dwCooper );

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

void CUdpSocket::LogPvPointChar(DWORD pAction, CTPlayer *pPlayer,DWORD dwPoint,BYTE bEventType, BYTE bPointType, CString pTargetName)
{
	if(!pPlayer ) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction		=	pAction;
	LogData.dwServerID		=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME);			//	Character Name

	LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
	LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level


	//PvP Point
	LogData.dwKey[2]		=	TOWNER_CHAR;					//	Char or Guild
	LogData.dwKey[3]		=	bEventType;						//	PVP_EVENT	
	lstrcpy(LogData.szKey[2],	pTargetName);

	if(bPointType & PVP_TOTAL)
        LogData.dwKey[4]		=	dwPoint;
	if(bPointType & PVP_USEABLE)
		LogData.dwKey[5]		=	dwPoint;	

	LogData.dwKey[6]		=	pPlayer->m_dwPvPTotalPoint;
	LogData.dwKey[7]		=	pPlayer->m_dwPvPUseablePoint;
	
	
	SendDMLog( LogData );
}

void CUdpSocket::LogPvPointGuild(DWORD pAction, DWORD dwPoint,WORD wLocalID, BYTE bCountry, DWORD dwGuildID, BYTE bEventType,BYTE bPointType)
{
	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction		=	pAction;
	LogData.dwServerID		=	_AtlModule.GetServerID();

	LogData.dwKey[2]		= TOWNER_GUILD;
	LogData.dwKey[3]		= bEventType;	

	if(bPointType & PVP_TOTAL)
        LogData.dwKey[4]		=	dwPoint;
	if(bPointType & PVP_USEABLE)
		LogData.dwKey[5]		=	dwPoint;

	LogData.dwKey[8]		= dwGuildID;
	LogData.dwKey[9]		= bCountry;
	LogData.dwKey[10]		= wLocalID;


	SendDMLog( LogData );
}

void CUdpSocket::LogAuctionReg(DWORD pAction, CTPlayer *pPlayer,DWORD dwAuctionID,__int64 ldwStartPrice,__int64 ldwDirectPrice, CTItem *pItem,DWORD dwCharID,CString strNAME)
{
	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction		=	pAction;
	LogData.dwServerID		=	_AtlModule.GetServerID();

	if(pPlayer)
	{
		lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

		LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
		LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
		LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
		LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

		LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
		LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
		lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME);			//	Character Name

		LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
		LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level
	}
	else
	{
		LogData.dwKey[1]		=	dwCharID;						//	Character Seq
		lstrcpy(LogData.szKey[1],	strNAME);						//	Character Name
	}

	LogData.dwKey[2]		=	dwAuctionID;
	LogData.dwKey[5]		=	ldwStartPrice;
	LogData.dwKey[6]		=	ldwDirectPrice;

	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			// Item
		LogData.dwKey[7]		=	pItem->m_bCount;			//	Item Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		LogData.dwFormat = LF_ITEM;

		_LPLOG_ITEM_ pLogItem = (_LPLOG_ITEM_)LogData.szLog;
		
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호

		pLogItem->wItemID		=	pItem->m_wItemID;			//	아이템 INDEX

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성

		int	nOrder = 0;
		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;
			
			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;						//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;	//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;	//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;	//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}

void CUdpSocket::LogAuctionBid(DWORD pAction, CTPlayer *pPlayer,DWORD dwAuctionID,__int64 ldwPrice,CTItem *pItem,BYTE bCount,DWORD dwCharID,CString strNAME)
{
	//if(!pPlayer ) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction		=	pAction;
	LogData.dwServerID		=	_AtlModule.GetServerID();

	if(pPlayer)
	{
		lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

		LogData.wMapID			=	pPlayer->m_wMapID;				//	Map ID
		LogData.nX				=	(int)pPlayer->m_fPosX;			//	X
		LogData.nY				=	(int)pPlayer->m_fPosY;			//	Y
		LogData.nZ				=	(int)pPlayer->m_fPosZ;			//	Z

		LogData.dwKey[0]		=	pPlayer->m_dwUserID;			//	User ID
		LogData.dwKey[1]		=	pPlayer->m_dwID;				//	Character Seq
		lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME);			//	Character Name

		LogData.dwKey[9]		=	pPlayer->m_bCountry + (pPlayer->m_bAidCountry == TCONTRY_N ? 0 : (pPlayer->m_bAidCountry+1)*10);			//	Country
		LogData.dwKey[10]		=	pPlayer->m_bLevel;				//	Level
	}
	else
	{
		LogData.dwKey[1]	= dwCharID;								//	Character Seq
		lstrcpy(LogData.szKey[1],	strNAME);						//	Character Name
	}

	LogData.dwKey[2]		=	bCount;
	LogData.dwKey[5]		=	dwAuctionID;
	LogData.dwKey[6]		=	ldwPrice;

	if(pItem)
	{
		LogData.dwKey[3]		=	pItem->m_wItemID;			// Item
		LogData.dwKey[7]		=	pItem->m_bCount;			//	Item Count
		LogData.dwKey[8]		=	pItem->m_dlID;				//	Unique Key

		LogData.dwFormat = LF_ITEM;

		_LPLOG_ITEM_ pLogItem = (_LPLOG_ITEM_)LogData.szLog;
		
		pLogItem->bInven		=	pItem->m_bInven;			//	인벤 위치
		pLogItem->bItemID		=	pItem->m_bItemID;			//	인벤 번호

		pLogItem->wItemID		=	pItem->m_wItemID;			//	아이템 INDEX

		//
		//	Game Part에 업무 지원 요청
		//	lstrcpy( pLogItem->szItemName, pItem->m_pTITEM-> )	//	아이템 명

		pLogItem->bLevel		=	pItem->m_bLevel;			//	업그레이트 레벨
		pLogItem->bCount		=	pItem->m_bCount;				

		pLogItem->bGLevel		=	pItem->m_bGLevel;			//	봉인 아이템 확인용
		pLogItem->dwDuraMax		=	pItem->m_dwDuraMax;			//	최대 내구도
		pLogItem->dwDuraCur		=	pItem->m_dwDuraCur;			//	현재 내구도	
		pLogItem->bRefineCur	=	pItem->m_bRefineCur;		//	제력 횟수

		pLogItem->bGradeEffect	=	pItem->m_bGradeEffect;		//	아이템 강화 속성

		int	nOrder = 0;
		MAPTMAGIC::iterator itTMAGIC;
		for( itTMAGIC = pItem->m_mapTMAGIC.begin(); itTMAGIC != pItem->m_mapTMAGIC.end(); itTMAGIC++)
		{
			pLogItem->bMagic[nOrder] =	(*itTMAGIC).first; 
			pLogItem->wValue[nOrder] =	(*itTMAGIC).second->m_wValue;
			
			nOrder ++;
		}

		//	Item Type
		if( pItem->m_bGLevel )
		{
			LogData.dwKey[4] = 3;						//	Seal Item
		}
		else
		{
			switch(nOrder)
			{
			case 0 :	LogData.dwKey[4] = 0;	break;	//	Normal Item

			case 1 : 
			case 2 :	LogData.dwKey[4] = 1;	break;	//	Magic Item

			case 3 :	LogData.dwKey[4] = 2;	break;	//	Rare Item

			case 4 :	break;

			case 5 :	break;
			}
		}

		pLogItem->dEndTime	=	pItem->m_dEndTime;
	}

	SendDMLog( LogData );
}

/*
*	LogTournament
*/

void CUdpSocket::LogTournamentApply(DWORD pAction, CTPlayer *pPlayer, CString pEventName, BYTE bParty, CString strTarget)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;						//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;					//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;					//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;					//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;					// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq	
	LogData.dwKey[2]		=	pPlayer->m_bCountry;					//	Country
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);					//	Character Name

	LogData.dwKey[3]		=	pPlayer->m_bLevel;						//	Level
	LogData.dwKey[4]		=	pPlayer->m_bClass;						//	Class
	
	lstrcpy(LogData.szKey[2],	strTarget);								//	Target Name

	lstrcpy(LogData.szKey[4],	pEventName	);							//	Event Name
	if(bParty == 1)
		lstrcpy(LogData.szKey[5],	"Party"	);							//	Party
	else lstrcpy(LogData.szKey[5],	"Single");							
	
	lstrcpy(LogData.szLog,	"Tournament Apply"	);

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

void CUdpSocket::LogTournamentWin(DWORD pAction, LPTOURNAMENTPLAYER pWin, LPTOURNAMENTPLAYER pLos, CString pTitle, DWORD dwValue, BYTE bStep)
{
	if(!pWin) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	LogData.wMapID			=	pWin->m_wMapID;							//	Map ID
	LogData.dwKey[1]		=	pWin->m_dwCharID;						//	Character Seq	
	LogData.dwKey[2]		=	pWin->m_bCountry;						//	Country
	lstrcpy(LogData.szKey[1],	pWin->m_strName	);						//	Character Name
	
	lstrcpy(LogData.szKey[4],	pTitle	);								//	Event Entry Title

	if(dwValue)
	{
		lstrcpy(LogData.szKey[5],	"Party"	);							//	Party
		LogData.dwKey[3]	=	dwValue;								//	Party LederID
	}
	else lstrcpy(LogData.szKey[5],	"Single");							

	CString strLog = "Tournament Win";

	switch(bStep)
	{
	case TNMTSTEP_QFINAL: strLog+= " [QFINAL]";	break;
	case TNMTSTEP_SFINAL: strLog+= " [SFINAL]";	break;
	case TNMTSTEP_FINAL: strLog+= " [FINAL]";	break;
	}

	if(pLos)
	{
		//	Target
		LogData.dwKey[6]		=	pLos->m_dwCharID;						//	Character Seq
		lstrcpy(LogData.szKey[2],	pLos->m_strName		);					//	Character Name
	}

	lstrcpy(LogData.szLog,	strLog	);

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

void CUdpSocket::LogTournamentEvent	(DWORD pAction, CTPlayer *pPlayer, DWORD dwTargetID, CString pTitle, BYTE bType)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	
	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;						//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;					//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;					//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;					//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;					// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq	
	LogData.dwKey[2]		=	pPlayer->m_bCountry;					//	Country
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);					//	Character Name
	
	lstrcpy(LogData.szKey[4],	pTitle	);								//	Event Entry Title
	if(bType == 1)
		lstrcpy(LogData.szKey[5],	"Party"	);							//	Party
	else lstrcpy(LogData.szKey[5],	"Single");							

	//	Target
	LogData.dwKey[6]		=	dwTargetID;								//	Character Seq

	lstrcpy(LogData.szLog,	"Tournament Batting"	);

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );

}

void CUdpSocket::LogRPSGame(DWORD pAction, CTPlayer *pPlayer, BYTE bType, BYTE bWin, DWORD dwMoney)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	
	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;						//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;					//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;					//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;					//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;					// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq	
	LogData.dwKey[2]		=	pPlayer->m_bCountry;					//	Country
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);					//	Character Name
	
	LogData.dwKey[3]		=	bType;
	LogData.dwKey[4]		=	bWin;
	LogData.dwKey[5]		=	dwMoney;

	wsprintf( LogData.szKey[5], "Now Money : %u/%u/%u",			//	Price
											pPlayer->m_dwGold,		
											pPlayer->m_dwSilver, 
											pPlayer->m_dwCooper );

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

void CUdpSocket::LogTactics(DWORD pAction, CTPlayer *pPlayer, DWORD dwGuild, CString strName)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	
	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;						//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;					//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;					//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;					//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;					// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq	
	LogData.dwKey[2]		=	pPlayer->m_bCountry;					//	Country
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);					//	Character Name
	
	LogData.dwKey[3]		=	dwGuild;
	lstrcpy(LogData.szKey[3],	strName);

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

void CUdpSocket::LogAidCountry(DWORD pAction, CTPlayer *pPlayer, BYTE bNowAid, BYTE bPrevAid)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	
	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;						//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;					//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;					//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;					//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;					// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq	
	LogData.dwKey[2]		=	pPlayer->m_bCountry;					//	Country
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);					//	Character Name
	
	LogData.dwKey[3]		=	bNowAid;
	LogData.dwKey[4]		=	bPrevAid;

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

void CUdpSocket::LogCountry(DWORD pAction, CTPlayer *pPlayer, BYTE bNowCountry, BYTE bPrevCountry)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));

	
	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;						//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;					//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;					//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;					//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;					// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq	
	LogData.dwKey[2]		=	pPlayer->m_bCountry;					//	Country
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);					//	Character Name
	
	LogData.dwKey[3]		=	bNowCountry;
	LogData.dwKey[4]		=	bPrevCountry;

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}


void CUdpSocket::LogKickOut(DWORD pAction, CTPlayer *pPlayer, BYTE bType)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));
	
	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;						//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;					//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;					//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;					//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;					// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq	
	LogData.dwKey[2]		=	pPlayer->m_bCountry;					//	Country
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);					//	Character Name
	
	LogData.dwKey[3]		=	bType;

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}


void CUdpSocket::LogCMGift(DWORD pAction, CTPlayer *pPlayer, WORD wGiftID, DWORD dwGMID, BYTE bType, DWORD dwValue, BYTE bCount)
{
	if(!pPlayer) return;

	_LOG_DATA_ LogData;
	memset( &LogData, 0x00, sizeof(_LOG_DATA_));
	
	LogData.dwAction	=	pAction;
	LogData.dwServerID	=	_AtlModule.GetServerID();

	lstrcpy( LogData.szClientIP, inet_ntoa(pPlayer->m_addr.sin_addr) );	

	LogData.wMapID			=	pPlayer->m_wMapID;						//	Map ID
	LogData.nX				=	(int)pPlayer->m_fPosX;					//	X
	LogData.nY				=	(int)pPlayer->m_fPosY;					//	Y
	LogData.nZ				=	(int)pPlayer->m_fPosZ;					//	Z

	LogData.dwKey[0]		=	pPlayer->m_dwUserID;					// User ID
	LogData.dwKey[1]		=	pPlayer->m_dwID;						//	Character Seq	
	LogData.dwKey[2]		=	pPlayer->m_bCountry;					//	Country
	lstrcpy(LogData.szKey[1],	pPlayer->m_strNAME	);					//	Character Name
	
	LogData.dwKey[3]		=	wGiftID;								//	Gift Chart ID
	LogData.dwKey[4]		=	dwGMID;
	LogData.dwKey[5]		=	bType;									//	Gift Type  (typedef enum CMGIFT_TYPE)
	LogData.dwKey[6]		=	dwValue;								//	Item ID or Money, merit Value
	LogData.dwKey[7]		=	bCount;									//	Item Count

	LogData.dwFormat = LF_TEXT;

	SendDMLog( LogData );
}

#endif