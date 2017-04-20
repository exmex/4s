#include "StdAfx.h"

CTAICmdLottery::CTAICmdLottery()
{
}

CTAICmdLottery::~CTAICmdLottery()
{
}

BYTE CTAICmdLottery::CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	if( pMON->m_bStatus != OS_DEAD )
		return FALSE;

	return CTAICommand::CanRun( pMON, dwEventHost, dwRHId, bRHType);
}

BYTE CTAICmdLottery::ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType)
{
	CTInven * pInven = pMON->FindTInven(INVEN_DEFAULT);
	CTItem * pItem = NULL;
	if(pInven)
	{
		MAPTITEM::iterator it = pInven->m_mapTITEM.find(BYTE(dwRHId));
		if( it != pInven->m_mapTITEM.end())
		{
			pItem = (*it).second;
			pMON->Lottery(pItem);
		}
	}

	return CTAICommand::ExecAI( pMON, dwEventHost, dwRHId, bRHType);
}