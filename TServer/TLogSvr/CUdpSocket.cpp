/*
 *	CUdpSocket.Cpp
 *
 */

#include "StdAfx.h"
#include "CUdpSocket.h"



/*
 *	Global Value
 */
extern HINSTANCE	g_hInstance;

ONUDPTRANSFUNC OnTransFunc[LP_COUNT];



/*
 *	
 */
CUdpSocket::CUdpSocket()
{
	m_lpRecvBuf = new char [sizeof(_UDPPACKET)+1] ;
	m_lpSendBuf = new char [sizeof(_UDPPACKET)+1] ;
}

CUdpSocket::~CUdpSocket()
{
	_Module.m_bService = FALSE;
	
	SetEvent(m_hReadEventCompleted);
	SetEvent(m_hWorkTickEvent);

	for(int i = 0; i<DEF_LISTHENTHREAD; i++)
	{
		bSendTo(_Module.m_szLogSvrIPAddr, _Module.m_nLogSvrPort );
	}

	CLOSEEVENT(m_hReadEventCompleted	);
	CLOSEEVENT(m_hWorkTickEvent	);

	CLOSETHREAD(m_hReadCompletedThread	);
	CLOSETHREAD(m_hWorkTickThread		);

	CLOSESOCKET(m_ListenSock			);
	CLOSESOCKET(m_SendSock				);


	m_listReadCompleted.RemoveAll();

	delete m_pIOPool;
	delete m_lpRecvBuf;
	delete m_lpSendBuf;
}

/*
 *	
 */
bool CUdpSocket::bSendTo(char * pIPAddr,int iPort)
{
	m_ReceivAddr.sin_port = htons(iPort);    

	if (NULL == pIPAddr)
	{
		m_ReceivAddr.sin_addr.s_addr =  ::htons(INADDR_ANY) ; 
	}
	else 
	{
		m_ReceivAddr.sin_addr.s_addr =  inet_addr(pIPAddr);
	}

   if ((m_nSendByte = sendto(m_SendSock, m_lpSendBuf, sizeof(_UDPPACKET), 0, 
       (SOCKADDR *)&m_ReceivAddr, sizeof(m_ReceivAddr))) == SOCKET_ERROR)
   {
      LogFileWrite("(X) ERROR: sendto failed with error %d\n", WSAGetLastError());

      return false;
   }
	return true ;
}

/*
 *	
 */
DWORD WINAPI CUdpSocket::ListenThreadRunner(LPVOID parameter)
{
	CUdpSocket * lpthread = static_cast < CUdpSocket * > (parameter);

	lpthread->ListenThreadProc();

	return 0;
}

/*
 *	
 */
DWORD WINAPI CUdpSocket::ReadThreadRunner(LPVOID parameter)
{
	CUdpSocket * lpthread = static_cast < CUdpSocket * > (parameter);

	lpthread->ReadThreadProc();

	return 0;
}


/*
 *	Work Tick처리 Thread
 *
 *	- DB Connection Check
 */
DWORD WINAPI CUdpSocket::WorkTickThreadRunner(LPVOID parameter)
{
	CUdpSocket * lpthread = static_cast < CUdpSocket * > (parameter);

	if( lpthread )
	{
		lpthread->WorkTickProc();
	}

	return 0;
}

/*
 *	
 */
void CUdpSocket::ListenThreadProc()
{

	SOCKADDR_IN			RecvAddr;
	int					RecvAddrSize = sizeof(RecvAddr);

	while( _Module.m_bService)
	{
		if ((m_nRecvByte =  recvfrom(m_ListenSock, m_lpRecvBuf, sizeof(_UDPPACKET), 0,
		   (SOCKADDR *)&RecvAddr, &RecvAddrSize)) == SOCKET_ERROR)
		{
			LogFileWrite( "(X) ERROR: recvfrom failed with error %d\n", WSAGetLastError());
		}

		if( !_Module.m_bService )			break;			// 프로그램 종료

		_LPUDPPACKET cTmpIO = m_pIOPool->Alloc() ;

		if(!cTmpIO )
		{				
			LogFileWrite( "(X) Memory Allocation Error" );
		}
		else
		{
			memcpy(cTmpIO,m_lpRecvBuf,sizeof(_UDPPACKET)) ;

			m_listReadCompleted.AddTail( cTmpIO );

			SetEvent( m_hReadEventCompleted );
		}
	}

	return ;
}

/*
 *	WorkTickProc
 *
 *	-	Check the Log DB Connection
 */
void CUdpSocket::WorkTickProc()
{
	while( _Module.m_bService)
	{
		DWORD ret = WaitForSingleObject( m_hWorkTickEvent, 30000);

		if(ret == WAIT_OBJECT_0)
		{
			//종료 이벤트 발생
			break;
		}
		else if(ret == WAIT_TIMEOUT)
		{		
			if(!m_LogDB.IsConnected())
			{
				m_LogDB.Close();
				m_LogDB.Init();

				char strConnection[200]={0,} ;				
				wsprintf(strConnection, "DRIVER={sql server};SERVER=%s;""UID=%s;PWD=%s;DATABASE=%s;", _Module.m_szLogDBIPAddr, _Module.m_szDBID, _Module.m_szDBPWD, _Module.m_szLogDBName);

				m_LogDB.SetLoginTimeout(5);
				m_LogDB.SetConnectionTimeout(5);


				if(!m_LogDB.DriverConnect(  strConnection  ))
				{
					LogFileWrite( "(X) Error: Cannot connect to DB. Error code = %s", m_LogDB.GetErrMsg());
				}
				else
				{
					LogFileWrite( "(O) Log DB reconnect the success" );
				}

			}
		}
	}
}


/*
 *	ReadPacket
 */
void CUdpSocket::ReadPacket( _LPUDPPACKET pUDPPacket)
{
	//	Check the Command
	if (pUDPPacket->dwCommand < 0 || pUDPPacket->dwCommand >= LP_COUNT ) 
	{
		m_pIOPool->Free(pUDPPacket) ;
		return ;
	}

	//
 	//	 try
	//	{

		if(!m_LogDB.IsConnected() )
		{
			m_listReadCompleted.AddTail(pUDPPacket);
			return;
		}

		BOOL bOK = OnTransFunc[pUDPPacket->dwCommand].proc(this, pUDPPacket );

		if( !bOK && !m_LogDB.IsConnected() )
		{
			m_listReadCompleted.AddTail(pUDPPacket);
			return;
		}
	//
	//	}
	//	catch(...)
	//	{
	//		static int x = 0;
	//		wsprintf(m_cMsg,"(X) UDP (%d) Unhandled msg[%d] ",x++,lpControlPacket->dwCommand ) ;
	//		_Module.LogEvent(m_cMsg) ;
	//	}

	m_pIOPool->Free(pUDPPacket) ;
}

/*
 *	
 */
void CUdpSocket::ReadThreadProc()
{

	_LPUDPPACKET cTmpIO ;

	while( _Module.m_bService)
	{
		if(m_listReadCompleted.IsEmpty())	
			WaitForSingleObject (m_hReadEventCompleted,INFINITE);
	
		if( !_Module.m_bService )			break;
		
		if(m_listReadCompleted.IsEmpty())	continue;

		cTmpIO = (_LPUDPPACKET) m_listReadCompleted.RemoveHead() ;

		ReadPacket(cTmpIO) ;
	}
	
	return ;
}




/*
 *	InitializeThread
 *
 */
BOOL CUdpSocket::InitializeThread()
{
	DWORD           ThreadID;
	DWORD			dwWorkerThreadCount;

	dwWorkerThreadCount = DEF_LISTHENTHREAD ;

	for(int i=0; i < (signed)dwWorkerThreadCount; i++)
	{
		HANDLE hListhenThread = (HANDLE) _beginthreadex
									(
										NULL, 
										0, 
										(unsigned(__stdcall*)(void*)) ListenThreadRunner,
										this, 
										0, 
										(unsigned int *)&ThreadID 
									);

		if( hListhenThread != NULL) 
		{
			ResumeThread( hListhenThread );
		}
		else
		{	
			LogFileWrite("(X) Error m_hListhenThreadThread Create. Error code = %d",GetLastError());

			return FALSE;
		}

		SetThreadPriority( hListhenThread, THREAD_PRIORITY_HIGHEST );	
	}
	
	m_hReadCompletedThread = (HANDLE) _beginthreadex
									(	
										NULL, 
										0, 
										(unsigned(__stdcall*)(void*)) ReadThreadRunner, 
										this, 
										0, 
										(unsigned int *)&ThreadID
									);

	if( m_hReadCompletedThread != NULL) 
	{
		ResumeThread( m_hReadCompletedThread );
	}
	else
	{	
		LogFileWrite("(X) Error m_hReadCompletedThread Create. Error code = %d",GetLastError());

		return FALSE;
	}

	m_hWorkTickThread	 =	(HANDLE)_beginthreadex
									(	
										NULL,
										0,
										(unsigned(__stdcall*)(void*))WorkTickThreadRunner,
										this,
										CREATE_SUSPENDED,
										(unsigned int *)&ThreadID
									);

	if( m_hWorkTickThread ) 
	{
		ResumeThread( m_hWorkTickThread  );
	}
	else
	{	
		LogFileWrite( "(X) Error Work Tick Thread Create. Error code = %d", GetLastError());

		return FALSE;
	}

	SetThreadPriority( m_hReadCompletedThread, THREAD_PRIORITY_HIGHEST );	

	//
	//	Create IO Completed 
	//
	m_hReadEventCompleted = CreateEvent(NULL, FALSE, FALSE, NULL);

	if( !m_hReadEventCompleted )
	{
		LogFileWrite("(X) Error IOCompletedEvent Create. Error code = %d", GetLastError());

		return FALSE;
	}

	//
	//	Create WorkTickThread Shutdown Event
	//
	m_hWorkTickEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

	if( !m_hWorkTickEvent )
	{
		LogFileWrite("(X) Error WorkTickThread Shutdown Event Create. Error code = %d", GetLastError());

		return FALSE;
	}

	return TRUE;
}


//
//	Initialize
//
BOOL CUdpSocket::Initialize(char *pIPAddr, int iPort)
{
	SOCKADDR_IN saTemp;

	if (!InitializeSock())
	{
		LogFileWrite("(X) Log Server bCreateSock Error");

		return FALSE;
	}

	if(!InitializeDB())
	{
		LogFileWrite("(X) Log Server bConnectDB Error");
		return FALSE ;
	}

	m_pIOPool = new TMemPool<_UDPPACKET>(MAX_IO_CONTEXT);
	if( !m_pIOPool )
	{
		LogFileWrite("(X) memory alloc error [%d]",GetLastError());
		return FALSE;
	}


	saTemp.sin_family = AF_INET;
	saTemp.sin_port = ::htons(iPort);

	
	//
	//	Server IP Address
	//
	saTemp.sin_addr.s_addr = inet_addr(pIPAddr);

	if (bind(m_ListenSock, (struct sockaddr *) &saTemp, sizeof(saTemp)) == SOCKET_ERROR)
	{
		CLOSESOCKET(m_ListenSock);

		LogFileWrite("(X) Bind Error: %d", WSAGetLastError() );

		return FALSE;
	}

	//
	//	thread Start 
	//
	if (!InitializeThread()) 
	{
		LogFileWrite("(X) Log Server bStartThread Error");
		return FALSE;
	}

	for(int i =0; i < LP_COUNT; i++ )
	{
		OnTransFunc[i].proc		= Packet_Nothing;
	}

	//
	//	호출할 함수 이름을 함수 포인터 배열에 집어 넣는다. 
	//
	OnTransFunc[LP_LOG].proc	= Packet_LogDB;

	return TRUE;
}

/*
 *	InitializeSock
 *
 */
BOOL CUdpSocket::InitializeSock()
{
	m_ListenSock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);

	if (INVALID_SOCKET == m_ListenSock)
	{
		LogFileWrite("(X) WSASocket(m_ListenSock): %d", WSAGetLastError());
		
		return FALSE;
	}
	
	m_SendSock = WSASocket(AF_INET, SOCK_DGRAM, IPPROTO_UDP, NULL, 0, 0);
	if (INVALID_SOCKET == m_SendSock)
	{
		LogFileWrite( "(X) WSASocket(m_SendSock): %d", WSAGetLastError());

		return TRUE;
	}
	
	m_ReceivAddr.sin_family = AF_INET;
	
	return TRUE;
}

/*
 *	InitializeDB
 *
 */
BOOL CUdpSocket::InitializeDB()
{
	char strConnection[1024]={0,} ;
	
	wsprintf(strConnection, "DRIVER={sql server};SERVER=%s; UID=%s;PWD=%s;DATABASE=%s;", _Module.m_szLogDBIPAddr, _Module.m_szDBID, _Module.m_szDBPWD, _Module.m_szLogDBName);

	m_LogDB.SetLoginTimeout(5);
	m_LogDB.SetConnectionTimeout(5);
	
	if(!m_LogDB.DriverConnect(  strConnection  ))
	{
		LogFileWrite("(X) Error: Cannot connect to DB. Error code = %s", m_LogDB.GetErrMsg());

		return FALSE;
	}

	return TRUE;
}


/*
 *	Purpose: 맵서버의 로그를 저장한다.
 *
 *		Input  : lpData - 로그 데이터 
 *		bForceSave - 서버 종료시
 */

BOOL CUdpSocket::LogDBSave(_LPLOG_DATA_ pPacket, bool bForceSave)
{
	if(!pPacket) return FALSE;

	CODBCRecordset LogTable(&m_LogDB);

		char szSQL[2048];		
		
		char szDate[20];
		wsprintf( szDate, DATETIME, pPacket->tTimeStamp.year,
									pPacket->tTimeStamp.month, 
									pPacket->tTimeStamp.day,
									pPacket->tTimeStamp.hour, 
									pPacket->tTimeStamp.minute, 
									pPacket->tTimeStamp.second);

		memset(szSQL, 0x00, sizeof(szSQL));


		//
		//	Get time as long integer.
		//
		SYSTEMTIME SysTime;
		GetLocalTime(&SysTime);

		wsprintf(szSQL, SQLQUERY_DGINSERT, 

						SysTime.wYear,
						SysTime.wMonth,
						SysTime.wDay,

						szDate,							

						pPacket->dwServerID,
						pPacket->szClientIP,
						pPacket->dwAction,
						pPacket->wMapID,
						pPacket->nX,
						pPacket->nY,
						pPacket->nZ,

						pPacket->dwKey[0],
						pPacket->dwKey[1],
						pPacket->dwKey[2],
						pPacket->dwKey[3],
						pPacket->dwKey[4],
						pPacket->dwKey[5],
						pPacket->dwKey[6],
						pPacket->dwKey[7],
						pPacket->dwKey[8],
						pPacket->dwKey[9],
						pPacket->dwKey[10],

						pPacket->szKey[0],
						pPacket->szKey[1],
						pPacket->szKey[2],
						pPacket->szKey[3],
						pPacket->szKey[4],
						pPacket->szKey[5],
						pPacket->szKey[6],
						
						pPacket->dwFormat
					);

		if(!LogTable.WriteBlob(szSQL, (void*)(pPacket->szLog), sizeof(pPacket->szLog)) )
		{
			LogFileWrite("(X) DB Error:%s", LogTable.GetErrMsg() );
			LogFileWrite("%s", szSQL );

			LogTable.Close();

			return FALSE;
		}
		
	LogTable.Close();

	return TRUE;
}

/*
 *	Packet_ItemLog
 */
BOOL Packet_LogDB(CUdpSocket* pUdpSocket, _LPUDPPACKET pUDPPacket) 
{
	return pUdpSocket->LogDBSave( (_LPLOG_DATA_)(pUDPPacket->szPacket) );
}


/*
 *	잘못된 메세지가 온 경우를 대비해서 만든 함수 
 */
BOOL Packet_Nothing(CUdpSocket* lpUdpSocket  ,_LPUDPPACKET lpControlPacket) 
{
	return TRUE;	
}



