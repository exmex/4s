#include "StdAfx.h"
#include ".\serverstatesock.h"
#include "Resource.h"
#include "ServerStateDlg.h"


CListCtrl *CServerStateSock::m_pctrlServerState = NULL;
CWnd *CServerStateSock::m_pHost = NULL;

CServerStateSock::CServerStateSock(void)
{
	m_nIndex = -1;
}

CServerStateSock::~CServerStateSock(void)
{
}

void CServerStateSock::OnClose( int nErrorCode)
{
	if( m_pHost && m_pctrlServerState )
	{
		m_pctrlServerState->SetItemText( m_nIndex, 3 ,_T("立加 阂啊") );
		((CServerStateDlg *) m_pHost)->DoBeep(TRUE);
	}

	CAsyncSocket::OnClose(nErrorCode);
}

void CServerStateSock::OnConnect(int nErrorCode)
{
	if( m_pHost && m_pctrlServerState )
		if(nErrorCode)
		{
			m_pctrlServerState->SetItemText( m_nIndex, 3 ,_T("立加 阂啊") );
			((CServerStateDlg *) m_pHost)->DoBeep(TRUE);
		}
		else
			m_pctrlServerState->SetItemText( m_nIndex, 3 ,_T("立加") );

	CAsyncSocket::OnConnect(nErrorCode);
}
