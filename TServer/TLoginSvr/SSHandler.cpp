// SSHandler.cpp : S/S Protocol Handler 함수의 구현입니다.

#include "stdafx.h"
#include "TLoginSvr.h"
#include "TLoginSvrModule.h"


DWORD CTLoginSvrModule::OnSM_QUITSERVICE_REQ( CSqlDatabase *pDB, LPMAPTGROUP pGROUP, CTUser *pUser, CPacket& packet)
{
	LogEvent(_T("SM_QUITSERVICE_REQ detected !!"));
	if(m_bService)
		SetServiceStatus(SERVICE_STOP_PENDING);
	PostThreadMessage( m_dwThreadID, WM_QUIT, 0, 0);

	return EC_NOERROR;
}
