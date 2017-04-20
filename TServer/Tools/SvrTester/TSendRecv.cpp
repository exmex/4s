// TSendRecv.cpp : CTSessionDoc 클래스의 구현
//

#include "stdafx.h"
#include "SvrTester.h"

#include "TSessionDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// String Type Cast
BYTE CTSessionDoc::strTobyte(CString str)
{
	BYTE value = atoi(str);
	return value;	
}
WORD CTSessionDoc::strToword(CString str)
{
	WORD value = (WORD)atol(str);		
	return value;
}
DWORD CTSessionDoc::strTodword(CString str)
{
	DWORD value = atol(str);	
	return value;
}
float CTSessionDoc::strTofloat(CString str)
{	
	float value = (float)atof(str);
	return value;
}
SOCKADDR_IN CTSessionDoc::strToSOCKADDR(CString str)
{
	SOCKADDR_IN addr;
	memset( &addr, 0x00, sizeof(SOCKADDR_IN));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = inet_addr(str);

    return addr;	
}

// Protocol Send
void CTSessionDoc::Send(DWORD pID, CString strParam, BOOL bAllFlag)
{
    CPacket *pMsg = new CPacket();
	pMsg->SetID((WORD)pID);

	CString str = strParam;		

	if( strParam != _T("") ) 
	{
		TCHAR* param = strParam.GetBuffer(0);	

		TCHAR* token;
		token = strtok( param, _T(" \n") ); 

		MAPPROTOCOLTYPE::iterator finder = m_mapType.find(pID);
		VECTORTYPE v = (*finder).second;

		if( !v.empty() )		
			SendPacket(pMsg, v, token);		
	}

	if( bAllFlag )
		SayToAll(pMsg, pID, str);
	else
	{
		CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
		CTSessionView* pView = pApp->GetTSessionView();

		int nID = pView->GetDataCurrentComboThread();
		if(nID <= 0)
		{
			delete pMsg;
			return;
		}
		SayToOne(pMsg, nID, pID, str);
	}
}
void CTSessionDoc::SendPacket(CPacket *pMsg, VECTORTYPE v, TCHAR* token)
{
	int cnt = 0;

	while( token != NULL ) 
	{
		switch(v[cnt])
		{
		case TYPE_BYTE: 
			(*pMsg) << strTobyte(token);
			break;
		case TYPE_WORD:
			(*pMsg) << strToword(token);
            break;
		case TYPE_DWORD:
			(*pMsg) << strTodword(token);
			break;
        case TYPE_FLOAT:
			(*pMsg) << strTofloat(token);
			break;
		case TYPE_STRING:
			(*pMsg) << token;
			break;
		case TYPE_SOCKADDR:
			(*pMsg) << strToSOCKADDR(token).sin_addr.s_addr;
			break;
		}
		cnt++;
		token = strtok( NULL, _T(" \n") ); 
	}
	return;
}

// Receive Message Value
CString CTSessionDoc::ReceiveByte(VECTORTYPE* vLoop, BYTE value)
{
	CString temp;	
	vLoop->push_back(value);

	temp.Format(_T("BYTE(%d)"), value);
	return temp;
}
CString CTSessionDoc::ReceiveWord(VECTORTYPE* vLoop, WORD value)
{
	CString temp;	
	vLoop->push_back(value);

	temp.Format(_T("WORD(%d)"), value);
	return temp;
}
CString CTSessionDoc::ReceiveDword(VECTORTYPE* vLoop, DWORD value)
{
	CString temp;	
	vLoop->push_back(value);

	temp.Format(_T("DWORD(%ld)"), value);
	return temp;
}
CString CTSessionDoc::ReceiveFloat(float value)
{
	CString temp;	

	temp.Format(_T("FLOAT(%lf)"), value);
	return temp;
}
CString CTSessionDoc::ReceiveString(CString value)
{
	CString temp = _T("");
	temp = temp + _T("STRING(") + value + _T(")");
	return temp;
}
CString CTSessionDoc::ReceiveInt64(__int64 value)
{
	CString temp;

	CTime t(value);
	CString strTime;
	strTime.Format(_T("%d-%d-%d %d:%d"),t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
	
	temp.Format(_T("INT64(%s)"), strTime);
	return temp;
}
CString CTSessionDoc::ReceiveSOCKADDR(DWORD value)
{
	CString temp;		

	SOCKADDR_IN addr;
	addr.sin_addr.s_addr = value;
	CString strAddr = inet_ntoa(addr.sin_addr);	

	temp.Format(_T("SOCKADDR(%s)"), strAddr);
	return temp;
}

// Protocol Receive
void CTSessionDoc::Receive(CTachyonSession* pSession, DWORD pID, CPacket* pPacket)
{
	if( !m_bRun )
		return;

	// Receive Type -> String	
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();

	CString data = _T("");

	MAPPROTOCOLTYPE::iterator Tfinder = m_mapType.find(pID);
	if(Tfinder == m_mapType.end())
		return;

	VECTORTYPE vType = (*Tfinder).second;

	ReceivePacket(vType, pPacket, &data);

	MAPPROTOCOL::iterator Pfinder = m_mapProtocol.find(pID);
	CString strP = (*Pfinder).second;

	BOOL bDis = pApp->GetTSessionFrm()->GetTSessionDlg()->GetDisregardList(strP);	

	BOOL bSelf = TRUE;
	if( GetMacroFlag() )
		bSelf = pSession->IsSelfCheck(pID, data);	
		
	if( !bDis )
		PostPushList(pSession, strP, data);
	
	pSession->SetCharInfo(strP, data);
	
	int nIndex = pView->GetIndexComboThread(pSession->m_SessionID);
	if( nIndex < 0 )
		return;
	
	if( !bDis )
	{
		if( pSession->m_SessionID == pView->GetDataCurrentComboThread() ) // Current Thread Receive
		{
			PostCharinfoView();
			PostSessionlistView();
		}
	}	

	// Macro Protocol Send
	if( GetMacroFlag() )
	{
		if( pSession->IsEmptyMacro() )
			return;

		// Set Param Data
		BOOL bSetP = pSession->SetParamData(pID, data);

		if( strP == _T("CS_START_ACK") )
			return;

		if( bSetP && bSelf )
			MacroSend(pSession);
	}
}
void CTSessionDoc::ReceivePacket(VECTORTYPE vType, CPacket *pPacket, CString* data)
{
	if( !m_bRun )
		return;

	if( vType.size() == 0 )
		return;

	BYTE bData;
	WORD wData;
	DWORD dwData;
	float fData;
	CString sData;
	__int64 iData;

	VECTORTYPE vLoop;

	for(int i = 0; i < (int)vType.size(); i++)
	{
		if(pPacket->IsEOF())
			return;
		switch( vType[i] )
		{
		case TYPE_BYTE:			
            (*pPacket) >> bData;			
			*data += ReceiveByte(&vLoop, bData);			
			break;
		case TYPE_WORD:			
			(*pPacket) >> wData;			
			*data += ReceiveWord(&vLoop, wData);
			break;
		case TYPE_DWORD:		
			(*pPacket) >> dwData;			
			*data += ReceiveDword(&vLoop, dwData);
			break;
		case TYPE_FLOAT:			
			(*pPacket) >> fData;			
			*data += ReceiveFloat(fData);
			break;
		case TYPE_STRING:
			(*pPacket) >> sData;
			*data += ReceiveString(sData);
			break;
		case TYPE_INT64:
			(*pPacket) >> iData;
			*data += ReceiveInt64(iData);
			break;
		case TYPE_SOCKADDR:
			(*pPacket) >> dwData;
			*data += ReceiveSOCKADDR(dwData);
			break;
		case TYPE_LOOPSTART:
			i = ReceivePacketLoop(vType, &vLoop, pPacket, ++i, data);
			break;
		case TYPE_LOOPEND:
			break;
		}		
	}

	return;
}
int CTSessionDoc::ReceivePacketLoop(VECTORTYPE vType, VECTORTYPE* vLoop, CPacket *pPacket, int pos, CString* data)
{
	if( !m_bRun )
		return (int)vType.size();

	BYTE bData;
	WORD wData;
	DWORD dwData;
	float fData;
	CString sData;
	__int64 iData;

	int start = pos;	
	int last;

	int cnt = vLoop->back();
	if( cnt == 0 )
	{
		cnt = 1;
		while(cnt)
		{
			pos++;
            if( vType[pos] == TYPE_LOOPEND )
				cnt--;
			else if( vType[pos] == TYPE_LOOPSTART )
				cnt++;			
		}	
		return pos;
	}
	
    for(int i = 0; i < cnt; i++)
	{			
		while(1)
		{
			BOOL flag = FALSE;

			if(pPacket->IsEOF())
				return (int)vType.size();

			switch( vType[pos] )
			{
			case TYPE_BYTE:	
				(*pPacket) >> bData;			
				*data += ReceiveByte(vLoop, bData);
				break;
			case TYPE_WORD:
				(*pPacket) >> wData;
				*data += ReceiveWord(vLoop, wData);
				break;
			case TYPE_DWORD:			
				(*pPacket) >> dwData;				
				*data += ReceiveDword(vLoop, dwData);
				break;
			case TYPE_FLOAT:			
				(*pPacket) >> fData;
				*data += ReceiveFloat(fData);
				break;
			case TYPE_STRING:			
				(*pPacket) >> sData;				
				*data += ReceiveString(sData);
				break;
			case TYPE_INT64:
				(*pPacket) >> iData;
				*data += ReceiveInt64(iData);
				break;
			case TYPE_SOCKADDR:
				(*pPacket) >> dwData;
				*data += ReceiveSOCKADDR(dwData);
				break;
			case TYPE_LOOPSTART:			
				pos = ReceivePacketLoop(vType, vLoop, pPacket, ++pos, data);
				break;
			case TYPE_LOOPEND:				
				last = pos;
				pos = start;
				flag = TRUE;				
				break;
			}

			if(flag) 
				break;
			pos++;
		}
	}

	return last;
}

// Current Session List Show
void CTSessionDoc::ShowCurSessionList()
{
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();

	int nID = pView->GetDataCurrentComboThread();
    if(nID <= 0)		
		return;	

	MAPPSESSION::iterator finder = m_mapSession.find(nID);
	if( finder == m_mapSession.end() )
		return;
	CTachyonSession* pSession = (*finder).second;

	pView->InsertList(pSession);	
}
// CharInfo Show
void CTSessionDoc::ShowCharInfo()
{
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();
	CTSessionFrm* pFrm = pApp->GetTSessionFrm();
	int nIndex = pView->GetIndexCurrentComboThread();	

	if(nIndex < 0)
	{	
		CString str = _T("");
		pFrm->m_dlgSideBar.SetCharInfo(str, str, str, str, str, str);
		return;
	}

	MAPPSESSION::iterator finder = m_mapSession.find(pView->GetDataComboThread(nIndex));
	if( finder == m_mapSession.end() )
		return;
	CTachyonSession* pSession = (*finder).second;

	pFrm->m_dlgSideBar.SetCharInfo(pSession->GetUserID(), pSession->GetCharID(),
		pSession->GetKEY(), pSession->GetGroupID(), pSession->GetChannel(), pSession->GetSvrID());
}