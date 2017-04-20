#include "StdAfx.h"


CTAICommand::CTAICommand()
{
	m_pCOMMAND = NULL;
	m_dwDelay = 0;
	m_bLoop = FALSE;
}

CTAICommand::~CTAICommand()
{
}

CTAICommand *CTAICommand::CreateCMD( LPTAICOMMAND pCMD)
{
	CTAICommand *pResult = NULL;

	switch(pCMD->m_bType)
	{
	case AC_BEGINATK	: pResult = new CTAICmdBeginAtk(); break;
	case AC_GETAWAY		: pResult = new CTAICmdGetaway(); break;
	case AC_SETHOST		: pResult = new CTAICmdSetHost(); break;
	case AC_CHKHOST		: pResult = new CTAICmdChkHost(); break;
	case AC_CHGHOST		: pResult = new CTAICmdChgHost(); break;
	case AC_CHGMODE		: pResult = new CTAICmdChgMode(); break;
	case AC_ATTACK		: pResult = new CTAICmdAttack(); break;
	case AC_FOLLOW		: pResult = new CTAICmdFollow(); break;
	case AC_REFILL		: pResult = new CTAICmdRefill(); break;
	case AC_LEAVE		: pResult = new CTAICmdLeave(); break;
	case AC_REGEN		: pResult = new CTAICmdRegen(); break;
	case AC_ROAM		: pResult = new CTAICmdRoam(); break;
	case AC_REMOVE		: pResult = new CTAICmdRemove(); break;
	case AC_GOHOME		: pResult = new CTAICmdGohome(); break;
	case AC_LOTTERY		: pResult = new CTAICmdLottery(); break;
	default				: pResult = new CTAICommand(); break;
	}

	if(pResult)
		pResult->m_pCOMMAND = pCMD;

	return pResult;
}

DWORD CTAICommand::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return m_dwDelay;
}

BYTE CTAICommand::CheckCondition( LPTAICONDITION pCON,
								  CTMonster *pMON,
								  DWORD dwEventHost,
								  DWORD dwRHId,
								  BYTE bRHType)
{
	switch(pCON->m_bType)
	{
	case AN_PROB	: return DWORD(rand() % 100) < pCON->m_dwID ? TRUE : FALSE;
	case AN_MODE	: return pMON->m_bMode == pCON->m_dwID ? TRUE : FALSE;
	case AN_CHGHOST : pMON->m_dwTargetID == dwRHId && pMON->m_bTargetType == bRHType ? FALSE : TRUE;
	}

	return TRUE;
}

BYTE CTAICommand::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	for( int i=0; i<INT(m_pCOMMAND->m_vCONDITION.size()); i++)
		if(!CheckCondition( m_pCOMMAND->m_vCONDITION[i], pMON, dwEventHost, dwRHId, bRHType))
			return FALSE;

	return TRUE;
}

BYTE CTAICommand::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	pMON->OnEvent( AT_AICOMPLETE, m_pCOMMAND->m_dwID, dwEventHost, dwRHId, bRHType);
	return m_bLoop && GetDelay( pMON, dwEventHost, dwRHId, bRHType) ? TRUE : FALSE;
}
