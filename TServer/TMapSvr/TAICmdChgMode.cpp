#include "StdAfx.h"


CTAICmdChgMode::CTAICmdChgMode()
{
}

CTAICmdChgMode::~CTAICmdChgMode()
{
}

DWORD CTAICmdChgMode::GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	return 0;
}

BYTE CTAICmdChgMode::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_WAKEUP ||
		pMON->m_dwHostID != dwEventHost ||
		(pMON->m_bMode == MT_NORMAL && !pMON->CheckAttack()))
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdChgMode::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	switch(pMON->m_bMode)
	{
	case MT_NORMAL	:
		pMON->m_bMode = MT_BATTLE;
		break;
	case MT_BATTLE	:
		pMON->m_bMode = MT_GOHOME;
		pMON->m_dwTargetID = 0;
		pMON->m_bTargetType = 0;
		break;
	case MT_GOHOME	:
		pMON->m_bMode = MT_NORMAL;
		pMON->m_dwTargetID = 0;
		pMON->m_bTargetType = 0;
		break;
	}

	pMON->ChgMode(pMON->m_bMode);
	pMON->m_dwHostKEY++;

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}
