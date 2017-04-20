// TSessionMacro.cpp : CTSessionDoc 클래스의 구현
//

#include "stdafx.h"
#include "SvrTester.h"

#include "TSessionDoc.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

////////////////////////////////////////////////////////////////////////////////////////////
// Macro Flag
void CTSessionDoc::SetMacroFlag(BOOL flag)
{
	m_bMacro = flag;
}
BOOL CTSessionDoc::GetMacroFlag()
{
	return m_bMacro;
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
// Session Macro Map Insert / Clear / Init
void CTSessionDoc::InsertSMacroMap(DWORD mID, CTMacro* pMacro)
{
	InsertSMacroQueue(mID);
	m_mapSMacro.insert(MAPMACRO::value_type(mID, pMacro));

	TRACE(_T("Session Macro Map Insert : %ld %s\n"), mID, pMacro->GetMacroName());
}
void CTSessionDoc::ClearSMacroMap()
{		
	m_mapSMacro.clear();
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
// Session Macro Queue Insert / Clear
void CTSessionDoc::InsertSMacroQueue(DWORD mID)
{
	m_qMID.push(mID);
}
void CTSessionDoc::ClearSMacroQueue()
{
	while( !m_qMID.empty() )
		m_qMID.pop();
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
// Set Session Macro Info
void CTSessionDoc::SetSMacroInfo(CTachyonSession* pSession)
{	
	pSession->SetMacroInfo(m_qMID, m_mapSMacro);
}
////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////////////////
// Macro Send
BOOL CTSessionDoc::MacroSend(CTachyonSession* pSession)
{	
	if( !m_bRun )
		return FALSE;

	if( pSession->IsEmptyMacro() )
		return FALSE;

	STRUCTPINFO pInfo = pSession->GetSendMsg();

	if( pInfo.strName == _T("") )
		return FALSE;

	if(pInfo.bSleep)
		Sleep(pInfo.sleep);
	
	CString strParam;

	VECTORPARAM vParam = pInfo.vParam;
	VECTORPARAM v = pSession->GetMacroParam();
	for(int i = 0; i < (int)vParam.size(); i++)
	{		
		for(int j = 0; j < (int)v.size(); j++)
		{			
			if( vParam[i].strName == v[j].strName &&
				vParam[i].strType == v[j].strType )
			{
                strParam = strParam + v[j].strData + _T(" ");
				break;
			}
		}
	}

	TRACE(_T("SendMsg : ")+pInfo.strName+_T("\n"));
	Send(pSession, pInfo.dwPID, strParam);

	return TRUE;
}
// Macro Protocol Message Send
void CTSessionDoc::Send(CTachyonSession* pSession, DWORD pID, CString strParam)
{
	if( !m_bRun )
		return;

	CString str = strParam;
    CPacket *pMsg = new CPacket();
	pMsg->SetID((WORD)pID);
	
	if( str != _T("") )
	{
		TCHAR* param = str.GetBuffer(0);

		TCHAR* token;
		token = strtok( param, _T(" \n") );

		MAPPROTOCOLTYPE::iterator finder = m_mapType.find(pID);
		VECTORTYPE v = (*finder).second;

		if( !v.empty() )
			SendPacket(pMsg, v, token);
	}

	MAPPROTOCOL::iterator Pfinder = m_mapProtocol.find(pID);
	if( (*Pfinder).second == _T("CS_CHANNELLIST_REQ") ||
		(*Pfinder).second == _T("CS_CHARLIST_REQ") )
		pSession->SetCharInfo(1, strParam);
	if( (*Pfinder).second == _T("CS_START_REQ") )
		pSession->SetCharInfo(2, strParam);
	
	pSession->Say(pMsg);
}
////////////////////////////////////////////////////////////////////////////////////////////