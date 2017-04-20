/*
 *	CUdpSocket.h
 *
 */
#pragma once

#include "XptrList.h"
#include "TMemPool.h"
#include "ODBCDatabase.h"
#include "../../TProtocol/LogPacket.h"


class CUdpSocket 
{
public:    
    CUdpSocket();
    virtual ~CUdpSocket();

private:
	
	TMemPool<_UDPPACKET	> * m_pIOPool;
	
    SOCKET		m_ListenSock;
	SOCKET		m_SendSock;

	SOCKADDR_IN  m_ReceivAddr;
	
	HANDLE		m_hReadEventCompleted;				// IO READ 완료되었음을 알림
	HANDLE		m_hReadCompletedThread;				// IO 완료된후 처리하는 Thread

	HANDLE		m_hWorkTickThread;					// DB Connection Check
	HANDLE		m_hWorkTickEvent;					
													// 그렇지 않을 경우는 다른 작업 처리
	CXPtrList	m_listReadCompleted;				// 완료된 IO 큐, 처리를 기다린다.

	char		*m_lpRecvBuf;
	char		*m_lpSendBuf;

	int			m_nRecvByte;
	int			m_nSendByte;


	CODBCDatabase m_LogDB;

private:
	
	VOID CLOSEHANDLE( HANDLE &x )
	{
		if( x != NULL ){
			CloseHandle(x);
			x = NULL;
		}
	}

	VOID CLOSESOCKET( SOCKET &x )
	{
		if( x != INVALID_SOCKET ){
			closesocket(x);
			x = INVALID_SOCKET;
		}
	}

	VOID CLOSEEVENT( HANDLE &x )
	{
		if( x != NULL ){
			SetEvent(x);
			CloseHandle(x);
			x = NULL;
		}
	}

	VOID CLOSETHREAD( HANDLE &x )
	{
		DWORD dwCode = 0;

		if( x != NULL )
		{
			if(GetExitCodeThread(x, &dwCode))
			{
				if(dwCode == STILL_ACTIVE)
				{
					WaitForSingleObject(x,6000);

					if( GetExitCodeThread(x, &dwCode))
					{
						if(dwCode == STILL_ACTIVE)
						{
							TerminateThread(x,dwCode);
						}
					}
				}
			}

			CloseHandle(x);
			x = NULL;
		}
	}
	
public:

	//	Initialize Function
    BOOL Initialize(char *pIPAddr=NULL,int pPort=2000);

	BOOL InitializeSock();

	BOOL InitializeDB();

	BOOL InitializeThread();

	bool bSendTo(char * pIPAddr	= NULL,int pPort = 2000);

	//	Tread Function
	void ListenThreadProc() ;
	void ReadThreadProc() ;
	void WorkTickProc();

	void ReadPacket(_LPUDPPACKET) ;

	BOOL LogDBSave	(_LPLOG_DATA_ pPacket, bool bForceSave = false);


	friend BOOL Packet_LogDB		(CUdpSocket*,_LPUDPPACKET);

	friend BOOL Packet_Nothing		(CUdpSocket*,_LPUDPPACKET) ;

	static DWORD WINAPI ListenThreadRunner	(IN LPVOID ParamPtr) ;
	static DWORD WINAPI ReadThreadRunner	(IN LPVOID ParamPtr) ;
	static DWORD WINAPI WorkTickThreadRunner(IN LPVOID parameter);
};



typedef struct
{
	BOOL ( *proc) (CUdpSocket*, _LPUDPPACKET);
} ONUDPTRANSFUNC;

