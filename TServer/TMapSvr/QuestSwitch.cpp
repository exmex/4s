#include "StdAfx.h"
#include ".\questswitch.h"

CQuestSwitch::CQuestSwitch(void)
{
}

CQuestSwitch::~CQuestSwitch(void)
{
}
void CQuestSwitch::ExecQuest( CTPlayer *pPlayer,
                             DWORD dwTick,
                             LPMAPMAPVQUESTTEMP pTRIGGER,
                             LPMAPQUESTTEMP pQUESTTEMP,
                             LPMAPTITEMTEMP pITEMTEMP,
                             LPMAPTSKILLTEMP pSKILLTEMP,
                             LPMAPTMONSTERTEMP pMONTEMP,
                             LPMAPCLASS pCLASS)
{
	BYTE bLevel;
	if(!pPlayer->CanRunQuest( m_pQUEST, dwTick, bLevel))
	{
		BYTE bChangeSwitch = FALSE;
		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			if( m_pQUEST->m_vTerm[i]->m_bTermType == QTT_SWITCH )
				if(pPlayer->ChangeSwitch(m_pQUEST->m_vTerm[i]->m_dwTermID))
					bChangeSwitch = TRUE;

		if(bChangeSwitch)
			CQuest::ExecQuest(
				pPlayer,
				dwTick,
				pTRIGGER,
				pQUESTTEMP,
				pITEMTEMP,
				pSKILLTEMP,
				pMONTEMP,
				pCLASS);
	}
}
