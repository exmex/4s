// TMacroMachineThread.cpp : CTMacroMachineDoc 클래스의 구현
//

#include "stdafx.h"
#include "TMacroMachine.h"

#include "TMacroMachineDoc.h"
#include "TMacroMachineView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


WORD CTMacroMachineDoc::GetSessionSize()
{
	return WORD(m_mapTSession.size());
}

WORD CTMacroMachineDoc::GetThreadSize()
{
	return m_wThread;
}

BOOL CTMacroMachineDoc::CreateSession(WORD wCnt,BYTE bConnectType)
{
	if(m_bRun)
		return FALSE;

	m_wThread += wCnt;

	CWnd *pWnd = AfxGetMainWnd();
	m_HWND =  pWnd->m_hWnd;

	
	for(WORD i=0; i<wCnt; i++)
	{
		CTachyonSession * pSession = new CTachyonSession();
		pSession->SetOwner(this);
		pSession->SetConnectType(bConnectType);
		pSession->m_wID = m_wSessionIndex;

		if(!ThreadCreate(pSession))
		{
			m_wThread--;
			delete pSession;
		}
		else
		{
			EnterCriticalSection(&m_cs);
			m_mapTSession.insert(MAPTSESSION::value_type(pSession->m_wID, pSession));
			LeaveCriticalSection(&m_cs);
		}
		
		//ResumeThread(pSession->m_hThread);		
		m_wSessionIndex++;
	}	

	StartThread();

	return TRUE;
}
CTachyonSession* CTMacroMachineDoc::CreateSession(BYTE bConnectType)
{
	m_wThread++;	
	
	CTachyonSession * pSession = new CTachyonSession();
	pSession->SetOwner(this);
	pSession->SetConnectType(bConnectType);
	pSession->m_wID = m_wSessionIndex;
	pSession->m_bRelayEnable = TRUE;

	if(!ThreadCreate(pSession))
	{
		TRACE("ThreadCreate Fail \n");
		m_wThread--;
		delete pSession;
		return NULL;
	}
	else
	{
		EnterCriticalSection(&m_cs);
		m_mapTSession.insert(MAPTSESSION::value_type(pSession->m_wID, pSession));
		LeaveCriticalSection(&m_cs);
	}

	ResumeThread(pSession->m_hThread);		
	m_wSessionIndex++;

	return pSession;
	
}


BOOL CTMacroMachineDoc::ThreadCreate(CTachyonSession * pSession)
{
	DWORD dwThreadID;

	HANDLE hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
    if(!hEvent)
		return FALSE;

	HANDLE hSleep = CreateEvent(NULL, TRUE, FALSE, NULL);
	if(!hSleep)
		return FALSE;

	HANDLE hThread = CreateThread(
		NULL, 0,
		_WorkThread,
		(LPVOID) this,
		CREATE_SUSPENDED,
		&dwThreadID);

	if(!dwThreadID)
	{
		CloseHandle(hEvent);
		return FALSE;
	}

	pSession->m_hEvent = hEvent;
	pSession->m_hThread = hThread;
	pSession->m_hSleep = hSleep;
	pSession->m_dwThreadID = dwThreadID;
	pSession->m_bRun = TRUE;

	return TRUE;
}

void CTMacroMachineDoc::StartThread()
{
	m_bRun = TRUE;

	MAPTSESSION::iterator itSession;
	for(itSession=m_mapTSession.begin(); itSession!=m_mapTSession.end(); itSession++)
		ResumeThread((*itSession).second->m_hThread);
}

void CTMacroMachineDoc::EndThread()
{
	m_bRun = FALSE;

	MAPTSESSION::iterator itSession;
	for(itSession=m_mapTSession.begin(); itSession!=m_mapTSession.end(); itSession++)
	{
		(*itSession).second->m_bRun = FALSE;
		SetEvent((*itSession).second->m_hSleep);
		SetEvent((*itSession).second->m_hEvent);
		if(WaitForSingleObject((*itSession).second->m_hThread, TIME_WAIT) == WAIT_TIMEOUT)
		{
			DWORD dwExitCode = 0;
			TerminateThread((*itSession).second->m_hThread, dwExitCode);
			CloseHandle((*itSession).second->m_hSleep);
			CloseHandle((*itSession).second->m_hEvent);
			TRACE(_T("TERMINATE THREAD\n"));
		}
		CloseHandle((*itSession).second->m_hThread);
		delete (*itSession).second;
	}

	EnterCriticalSection(&m_cs);
	m_mapTSession.clear();
	LeaveCriticalSection(&m_cs);
	m_HWND = NULL;

	m_mapTVALUE.clear();
	while(m_qTMACRO.size())
		m_qTMACRO.pop();

	m_wThread = 0;
	m_wSessionIndex = 0;
}

void CTMacroMachineDoc::ConnetSession(CString strAddr, DWORD dwPort)
{
	MAPTSESSION::iterator itSession;
	for(itSession=m_mapTSession.begin(); itSession!=m_mapTSession.end(); itSession++)
	{
		Sleep(100);
		(*itSession).second->Start(strAddr, dwPort);
	}
}

DWORD CTMacroMachineDoc::_WorkThread(LPVOID lpParam)
{
	CTMacroMachineDoc *pModule = (CTMacroMachineDoc *) lpParam;
	return pModule->WorkThread();
}

DWORD CTMacroMachineDoc::WorkThread()
{
	CTachyonSession * pSession = NULL;

	MAPTSESSION::iterator itSession;
	for(itSession=m_mapTSession.begin(); itSession!=m_mapTSession.end(); itSession++)
	{
		if((*itSession).second->m_dwThreadID == GetCurrentThreadId())
		{
			pSession = (*itSession).second;
			break;
		}
	}

	if(!pSession)
		return 0;

	TRACE(_T("THREAD START %d\n"), pSession->m_wID);

	while(m_bRun)
	{
		if(!pSession->m_bRun)
			break;

		if(pSession->EmptyTType())
		{
			//WaitForSingleObject(pSession->m_hEvent, INFINITE);
			if(WaitForSingleObject(pSession->m_hEvent, TIME_ENDTHREAD) == WAIT_TIMEOUT)
			{
				if(pSession->IsExcuteMacro())
				{
					TRACE(_T("THREAD TIMEOUT : %d\n"),pSession->m_wID);
					//pSession->m_bRun = FALSE;
				}
			}
		}
		else
		{
			TTYPE tType = pSession->FrontTType();

			switch(tType.m_bType)
			{
			case TYPE_START:
				SendMacro(pSession);
				break;
			case TYPE_RECEIVE:
				pSession->OnReceive(tType.m_nErrorCode);
				break;
			case TYPE_MAPCONNECT:
				TRACE(_T("MAPCONNECT %s %d\n"), pSession->m_strMapIP, pSession->m_wMapPort);
				pSession->End();
				pSession->Start(pSession->m_strMapIP, pSession->m_wMapPort);
				pSession->ClearMapData();
				//TRACE(_T("MAPCONNECT SUCCESS\n"), pSession->m_strMapIP, pSession->m_wMapPort);
				pSession->SetConnectType(CON_MAP);
				break;
			}
			
			pSession->PopTType();
		}

		ResetEvent(pSession->m_hEvent);
	}
	
	//UpdateSessionCnt();
	pSession->End();
	CloseHandle(pSession->m_hSleep);
	CloseHandle(pSession->m_hEvent);

	TRACE(_T("THREAD END %d\n"), pSession->m_wID);

	return 0;
}

void CTMacroMachineDoc::SendMacroProtocol(CTachyonSession* pSession, TQPROTOCOL& tProtocol)
{
	MAPTPROTOCOL::iterator find = m_mapProtocol.find(tProtocol.m_dwID);
	if(find!=m_mapProtocol.end())
		TRACE(_T("# SEND MACRO (%s)  Session : % d \n"), (*find).second, pSession->m_wID );
	else
		return;

	CPacket * pMsg = new CPacket();
	pMsg->SetID((WORD)tProtocol.m_dwID);

	VTPTVALUE vValue = tProtocol.m_vType;
	for(DWORD i=0; i<vValue.size(); i++)
	{
		TPTVALUE tValue = vValue[i];
		InputValue(pSession, pMsg, tValue, tProtocol.m_dwID);
	}

	if(!pSession->Say(pMsg))
	{
		TRACE(_T("== SAY FALSE : %d \n"),pSession->m_wID);
	}

	delete pMsg;
}


void CTMacroMachineDoc::SendMacro(CTachyonSession * pSession)
{
	if(pSession->EmptyMacro())
		return;

	if(pSession->EmptyProtocol())
	{
		pSession->PopMacro();
		if(pSession->EmptyMacro())
			return;
	}

	//TRACE(_T("SEND MACRO START\n"));

	//TQPROTOCOL tProtocol = pSession->FronProtocol();
	//if(!pSession->CheckProtocolType(tProtocol.m_dwID) )
	//	pSession->PopProtocol();
	//
	//if(pSession->EmptyProtocol())
	//{
	//	pSession->PopMacro();
	//	if(pSession->EmptyMacro())
	//		return;
	//}
	//tProtocol = pSession->FronProtocol();

	TQPROTOCOL tProtocol = pSession->FronProtocol();
	while(!pSession->CheckProtocolType(tProtocol.m_dwID) )
	{
		pSession->PopProtocol();

		if(pSession->EmptyProtocol() )
		{
			pSession->PopMacro();
			if(pSession->EmptyMacro() )
				return;
		}
		tProtocol = pSession->FronProtocol();
	}

	if(tProtocol.m_wSleep)
	{
		//WORD dwTime = max(rand()%tProtocol.m_wSleep, 100);
		WORD dwTime = tProtocol.m_wSleep;
		TRACE(_T("SLEEP %d\n"), dwTime);
		WaitForSingleObject(pSession->m_hSleep, dwTime);
		//SleepEx(wTime, TRUE);
	}

	SendMacroProtocol(pSession,tProtocol);

	// Loop Protocol
	if(tProtocol.m_bLoop)
	{
		tProtocol.ResetAck();
		pSession->PushLoopProtocol(tProtocol);		
	}

	//
	TCHILD tChild = pSession->GetChild();

	if(!tChild.m_dwID )
	{
		pSession->PopProtocol();
		SendMacro(pSession);
		return;
	}

	//TRACE(_T("SEND MACRO END\n"));
}

BOOL CTMacroMachineDoc::SendLoopProtocol(CTachyonSession* pSession, CPacket* pPacket)
{		
	MAPTPROTOCOLTEMP::iterator itP = m_mapTProtocol.find(pPacket->GetID() );	
	if( itP == m_mapTProtocol.end() )
		return FALSE;

	DWORD dwParendID = (*itP).second->m_dwParent;

	MAPTQPROTOCOL::iterator finder = pSession->m_mapLoopProtocol.find(dwParendID) ;
	if( finder != pSession->m_mapLoopProtocol.end() )
	{		
		TQPROTOCOL tProtocol = (*finder).second;
		TCHILD tChild;

		for(DWORD dwC = 0; dwC < tProtocol.m_vTChild.size(); dwC++)
		{
			if(tProtocol.m_vTChild[dwC].m_dwID == pPacket->GetID() )
			{
				tChild = tProtocol.m_vTChild[dwC];
				break;
			}
		}

		//PacketOutput(pSession,pPacket,tChild);

		if(tChild.m_bEndChild)
		{
			if(tProtocol.m_wSleep)
			{
				WORD dwTime = max(rand()%tProtocol.m_wSleep, 100);
				TRACE(_T("SLEEP %d\n"), dwTime);
				WaitForSingleObject(pSession->m_hSleep, dwTime);
				//SleepEx(wTime, TRUE);
			}
			TRACE(_T("Send Loop Protocol  "));
			SendMacroProtocol( pSession , tProtocol);
		}
		return TRUE;
	}

	return FALSE;
}
void CTMacroMachineDoc::Receive(CTachyonSession * pSession, CPacket * pPacket)
{	
	BOOL bRet =	SendLoopProtocol(pSession,pPacket); // Loop Protocol Send

	if(pSession->EmptyMacro() )
		return;

	TCHILD tChild = pSession->GetChild();	

	if(!tChild.m_dwID )
	{
		pSession->PopProtocol();
		SendMacro(pSession);
		return;
	}

	MAPTPROTOCOL::iterator fffff = m_mapProtocol.find(pPacket->GetID());
	if(fffff != m_mapProtocol.end())
		TRACE(_T("REVEIVE MSG (%s)\n"), (*fffff).second);

	if(pPacket->GetID() != (WORD)tChild.m_dwID)		
		return ;
	else
		pSession->SetRecvAck(tChild.m_dwID);

	if(!bRet)
        PacketOutput(pSession,pPacket,tChild);

	if(pSession->CheckMapConnect(pPacket->GetID()))
		pSession->PushTType(0, TYPE_MAPCONNECT);
	
	if( pSession->m_bRelayEnable && pSession->CheckRelayConnect( pPacket->GetID() ) )
	{
		CTachyonSession * pRelaySession = CreateSession(BYTE(CON_RELAY));
		if(!pRelaySession)
			return;

		pRelaySession->SetQueue(pSession->m_qTMacro, pSession->m_mapTValue);
		if(pRelaySession->Start(pSession->m_strRelayIP,pSession->m_wRelayPort))
		{
			pRelaySession->ExcuteMacro();
			pRelaySession->PushTType(0,TYPE_START);
			TRACE(_T("%d Relay Session Create : %d \n"),pSession->m_wID,pRelaySession->m_wID);
		}
		else
			TRACE(_T("%d Relay Session Create FAIL : %d \n"),pSession->m_wID,pRelaySession->m_wID);		
	}

	if(tChild.m_bEndChild )
	{
        pSession->PopProtocol();

		if(!pSession->IsMapConnect())
			SendMacro(pSession);
	}

	//TRACE(_T("RECEIVE END\n"));
}

BYTE CTMacroMachineDoc::GetByte(CString str)
{
	BYTE bValue = (BYTE)atoi(str);
	return bValue;
}

WORD CTMacroMachineDoc::GetWord(CString str)
{
	WORD wValue = (WORD)atoi(str);
	return wValue;
}

DWORD CTMacroMachineDoc::GetDword(CString str)
{
	DWORD dwValue = (DWORD)atoi(str);
	return dwValue;
}

FLOAT CTMacroMachineDoc::GetFloat(CString str)
{
	FLOAT fValue = (FLOAT)atof(str);
	return fValue;
}

CString CTMacroMachineDoc::GetString(CString str)
{
	return str;
}

INT64 CTMacroMachineDoc::GetInt64(CString str)
{
	return 0;
}

void CTMacroMachineDoc::InputValue(CTachyonSession * pSession, CPacket * pMsg, TPTVALUE tValue, DWORD dwProtocolID)
{
	CString str = pSession->GetValue(tValue.m_dwValueID);

	switch(tValue.m_bValueType)
	{
	case TYPE_BYTE:
		(*pMsg)
			<< GetByte(str);
		TRACE(_T("BYTE :  %s\n"), str);
		break;
	case TYPE_WORD:
		(*pMsg)
			<< GetWord(str);
		TRACE(_T("WORD :  %s\n"), str);
		break;
	case TYPE_DWORD:
		(*pMsg)
			<< GetDword(str);
		TRACE(_T("DWORD :  %s\n"), str);
		break;
	case TYPE_FLOAT:
		(*pMsg)
			<< GetFloat(str);
		TRACE(_T("FLOAT :  %s\n"), str);
		break;
	case TYPE_STRING:
		if( dwProtocolID == CS_CHAT_REQ)
		{
			if(pSession->m_bSendMsg == 2 )
			{
				CString strHeader;
				CString strSIZE;
				strSIZE.Format( _T("%04X%04X"), strHeader.GetLength(), str.GetLength());
				str = strSIZE + strHeader + str;
				pSession->m_bSendMsg = 0;
			}
			else
				pSession->m_bSendMsg += 1;
		}

		(*pMsg)
			<< GetString(str);
		TRACE(_T("STRING :  %s\n"), str);
		break;
	case TYPE_INT64:
		(*pMsg)
			<< GetInt64(str);
		TRACE(_T("INT64 :  %s\n"), str);
		break;
	}
}

CString CTMacroMachineDoc::RecvByte(BYTE bValue)
{
	CString temp;	
	temp.Format(_T("%d"), bValue);
	return temp;
}

CString CTMacroMachineDoc::RecvWord(WORD wValue)
{
	CString temp;	
	temp.Format(_T("%d"), wValue);
	return temp;
}

CString CTMacroMachineDoc::RecvDword(DWORD dwValue)
{
	CString temp;	
	temp.Format(_T("%u"), dwValue);
	return temp;
}

CString CTMacroMachineDoc::RecvFloat(FLOAT fValue)
{
	CString temp;
	temp.Format(_T("%f"), fValue);
	return temp;
}

CString CTMacroMachineDoc::RecvString(CString strValue)
{
	return strValue;
}

CString CTMacroMachineDoc::RecvInt64(INT64 iValue)
{
	CString temp = _T("");
	return temp;
}

BYTE CTMacroMachineDoc::OutputValue(CTachyonSession * pSession, CPacket * pMsg, TCHILD tChild, DWORD dwIndex)
{
	BYTE bTemp = 0;
	WORD wTemp = 0;
	DWORD dwTemp = 0;
	FLOAT fTemp = 0.0;
	CString strTemp = _T("");
	INT64 iTemp = 0;

	TPTVALUE tValue = tChild.m_vType[dwIndex];

	CString str = _T("");

	switch(tValue.m_bValueType)
	{
	case TYPE_BYTE:
		(*pMsg)
			>> bTemp;
		break;
	case TYPE_WORD:
		(*pMsg)
			>> wTemp;
		break;
	case TYPE_DWORD:
		(*pMsg)
			>> dwTemp;
		break;
	case TYPE_FLOAT:
		(*pMsg)
			>> fTemp;
		break;
	case TYPE_STRING:
		(*pMsg)
			>> strTemp;
		break;
	case TYPE_INT64:
		(*pMsg)
			>> iTemp;
		break;
	case TYPE_LOOPSTART:
		break;
	case TYPE_LOOPEND:
		return OUTPUT_BREAK;
	}

	if(tValue.m_dwValueID)
	{
		switch(tValue.m_bValueType)
		{
		case TYPE_BYTE:
			str = RecvByte(bTemp);
			TRACE(_T("BYTE :  %s\n"), str);
			break;
		case TYPE_WORD:
			str = RecvWord(wTemp);
			pSession->CheckMapPort(tChild.m_dwID, wTemp);
			pSession->CheckRelayPort(tChild.m_dwID,wTemp);
			TRACE(_T("WORD :  %s\n"), str);
			break;
		case TYPE_DWORD:
			str = RecvDword(dwTemp);
			pSession->CheckMapIP(tChild.m_dwID, dwTemp);
			pSession->CheckRelayIP(tChild.m_dwID, dwTemp);
			TRACE(_T("DWORD :  %s\n"), str);
			break;
		case TYPE_FLOAT:
			str = RecvFloat(fTemp);
			TRACE(_T("FLOAT :  %s\n"), str);
			break;
		case TYPE_STRING:
			str = RecvString(strTemp);
			TRACE(_T("STRING :  %s\n"), str);
			break;
		case TYPE_INT64:
			str = RecvInt64(iTemp);
			TRACE(_T("INT64 :  %s\n"), str);
			break;
		}

		if(tChild.m_bSelf)
		{
			if(!pSession->CheckSelf(tValue.m_dwValueID, str))
				return OUTPUT_SELF;
			else
				return OUTPUT_SUCCESS;
		}

		if(pSession->CheckBreak(tValue.m_dwValueID))
			return OUTPUT_END;
		
		pSession->UpdateValue(tValue.m_dwValueID, str);
	}
	
	return OUTPUT_SUCCESS;
}

void CTMacroMachineDoc::PacketOutput(CTachyonSession * pSession, CPacket * pPacket, TCHILD tChild)
{
	MAPTPROTOCOL::iterator finder = m_mapProtocol.find(pPacket->GetID());
	if(finder == m_mapProtocol.end())
		TRACE(_T("Not On List RECEIVE MSG (%s)  Session: %d \n"), (*finder).second, pSession->m_wID);

	
	//MAPTPROTOCOL::iterator find = m_mapProtocol.find(tChild.m_dwID);
	//if(find!=m_mapProtocol.end())
	//	TRACE(_T("REVEIVE MACRO (%s)\n"), (*find).second);

	BYTE bRet = OUTPUT_SUCCESS;

	VTPTVALUE vValue = tChild.m_vType;
	for(DWORD i=0; i<vValue.size(); i++)
	{
		bRet = OutputValue(pSession, pPacket, tChild, i);
		if(bRet == OUTPUT_BREAK)
			break;
		else if(bRet == OUTPUT_SELF)
		{
			TRACE(_T("SELF CHECK\n"));
			return ;
		}
		else if(bRet == OUTPUT_END)
		{
			TRACE(_T("BREAK CHECK\n"));
			break;
		}
	}	
}