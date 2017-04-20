// TSessionThread.cpp : CTSessionDoc 클래스의 구현
//

#include "stdafx.h"
#include "SvrTester.h"

#include "TSessionDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CTSessionDoc 명령

/////////////////////////////////////////////////////////////////////////
// Session Create / Start / Close
void CTSessionDoc::SessionCreate()
{
	for(DWORD i = 0; i < m_dwThreadNum; i++)
	{
		CTachyonSession *pSession;
		pSession = new CTachyonSession();
		pSession->SetOwner(this);
		pSession->SetID(i+1);

		if( GetMacroFlag() )
			SetSMacroInfo(pSession);

		m_mapSession.insert(MAPPSESSION::value_type(pSession->m_SessionID, pSession));
	}	
	ThreadCreate();
	ThreadStart();
}
void CTSessionDoc::SessionStart(CString strIP, DWORD dwPort)
{
	MAPPSESSION::iterator it;
	for(it = m_mapSession.begin(); it != m_mapSession.end(); it++)
	{
		(*it).second->Start(strIP, dwPort);
	}
}
void CTSessionDoc::SessionClose()
{	
	ThreadEnd();
	MAPPSESSION::iterator it;
	for(it = m_mapSession.begin(); it != m_mapSession.end(); it++)
	{
		TRACE(_T("SESSION CLOSE %d\n"), (*it).second->m_SessionID);

		(*it).second->End();
		(*it).second->ClearMessage();
		delete (*it).second;
	}
	m_mapSession.clear();
}

/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////
// Thread Create / Start / End
void CTSessionDoc::ThreadCreate()
{
	HINSTANCE hProc = (HINSTANCE) GetCurrentProcess();
	::SetPriorityClass(hProc, HIGH_PRIORITY_CLASS);

	MAPPSESSION::iterator it;
	DWORD dwThreadID;
	for(it = m_mapSession.begin(); it != m_mapSession.end(); it++)
	{
		HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if(!hEvent)
			continue;

		HANDLE hThread = CreateThread(
			NULL, 0,
			_SessionThread,
			(LPVOID) this,
			CREATE_SUSPENDED,
			&dwThreadID);

		if( !hThread || !dwThreadID )
		{
			CloseHandle(hEvent);
			continue;
		}

		::SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);

		m_mapThreadSession.insert(MAPTHREADSESSION::value_type(dwThreadID, (*it).second));

		(*it).second->m_hEvent = hEvent;
		(*it).second->m_hThread = hThread;
		(*it).second->m_dwThreadID = dwThreadID;
	}
}
void CTSessionDoc::ThreadStart()
{
	m_bRun = TRUE;

    MAPTHREADSESSION::iterator it;

	for(it = m_mapThreadSession.begin(); it != m_mapThreadSession.end(); it++)
		ResumeThread((*it).second->m_hThread);
}
void CTSessionDoc::ThreadEnd()
{
	TRACE(_T("Thread End Start\n"));

	m_bRun = FALSE;

	BOOL flag = FALSE;

    MAPTHREADSESSION::iterator it;
	for(it = m_mapThreadSession.begin(); it != m_mapThreadSession.end(); it++)
	{
		SetEvent((*it).second->m_hEvent);

		int i = 0;
		while(1)
		{
			if( i > 5 )
			{
				DWORD dwExitCode = 0;
				TerminateThread((*it).second->m_hThread, dwExitCode);
				CloseHandle((*it).second->m_hEvent);

				flag = TRUE;

                TRACE(_T("Terminate Thread %d\n"), (*it).second->m_SessionID);

				break;
			}

			if( ::WaitForSingleObject((*it).second->m_hThread, 1000) == WAIT_TIMEOUT )
			{
				TRACE(_T("(%d) Wait Timeout\n"), (*it).second->m_SessionID);

				::SetThreadPriority((*it).second->m_hThread, THREAD_PRIORITY_TIME_CRITICAL);
				ResumeThread((*it).second->m_hThread);
				SetEvent((*it).second->m_hEvent);
			}
			else
				break;

			i++;
		}

		CloseHandle((*it).second->m_hThread);

		TRACE(_T("Thread Close %d\n"), (*it).second->m_SessionID);
	}

	if( flag )
		AfxMessageBox(_T("Terminate End"));

	m_dwThreadNum = 0;
    m_mapThreadSession.clear();
}

// Thread Run
DWORD CTSessionDoc::_SessionThread( LPVOID lpParam)
{	
	CTSessionDoc *pModule = (CTSessionDoc *) lpParam;
	return pModule->SessionThread();		
}
DWORD CTSessionDoc::SessionThread()
{	
	MAPTHREADSESSION::iterator it = m_mapThreadSession.find(GetCurrentThreadId());
	if( it == m_mapThreadSession.end() )
		return 0;	

	CTachyonSession* pSession = (*it).second;
	HANDLE hEvent = pSession->m_hEvent;

	BYTE bContinue = TRUE;

	TRACE(_T("(%d) THREAD GO\n"), pSession->m_SessionID);

	while( m_bRun && bContinue )
	{
		::WaitForSingleObject( hEvent, INFINITE );
		if( !m_bRun || !bContinue ) break;

		STRUCTTHREAD sFront = pSession->FrontMessage();

		switch( sFront.type )
		{
		case THREAD_TYPE_SEND:
			{
				pSession->Encrypt(sFront.packet);
				if( !pSession->Say(sFront.packet) )
				{				
					PostSessionClose(pSession);
					bContinue = FALSE;
				}
			}
			break;
		case THREAD_TYPE_RECV:
			switch( WSAGETSELECTEVENT(sFront.lParam) )
			{
			case FD_CONNECT	: 
				{
					pSession->OnConnect(sFront.nError);
					if( sFront.nError )
						bContinue= FALSE;
				}
				break;
			case FD_READ	:
				{
					pSession->OnReceive(sFront.nError);
				}
				break;
			case FD_WRITE	:
				{
					pSession->OnSend(sFront.nError);
				}
				break;
			case FD_CLOSE	:
				{
					if( !pSession->m_bLogin )
					{
						pSession->OnClose(sFront.nError);
						bContinue = FALSE;

						TRACE(_T("FD_CLOSE\n"));
					}
					else
					{
						STRUCTTHREAD sThread;						
						sThread.type = THREAD_TYPE_MAPCONNECT;
						pSession->PushMessage(sThread); // Message Queue Push	
					}					
				}
				break;
			}
			break;
		case THREAD_TYPE_MAPCONNECT:
			{
				pSession->Start(pSession->m_sMapIP, pSession->m_wMapPort);
			}
			break;
		}

		pSession->PopMessage();

		EnterCriticalSection(&pSession->g_cs);		
		if( pSession->EmptyMessage() )
			ResetEvent(hEvent);
		LeaveCriticalSection(&pSession->g_cs);
	}

	CloseHandle( hEvent );

	TRACE(_T("(%d) THREAD END %d\n"), pSession->m_SessionID, pSession->m_dwThreadID);

	return 0;
}
/////////////////////////////////////////////////////////////////////////