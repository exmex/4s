#include "StdAfx.h"

CQuestDefendSkill::CQuestDefendSkill(void)
{
}

CQuestDefendSkill::~CQuestDefendSkill(void)
{
}

void CQuestDefendSkill::ExecQuest( CTPlayer *pPlayer,
								   DWORD dwTick,
								   LPMAPMAPVQUESTTEMP pTRIGGER,
								   LPMAPQUESTTEMP pQUESTTEMP,
								   LPMAPTITEMTEMP pITEMTEMP,
								   LPMAPTSKILLTEMP pSKILLTEMP,
								   LPMAPTMONSTERTEMP pMONTEMP,
								   LPMAPCLASS pCLASS)
{
	BYTE bLevel;
	BYTE bRun = FALSE;
	if(!pPlayer->CanRunQuest( m_pQUEST, dwTick, bLevel))
	{
		WORD wSkill = 0;
		BYTE bLevel = 0;

		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			if( m_pQUEST->m_vTerm[i]->m_bTermType == QTT_SKILLID )
			{
				wSkill = (WORD)m_pQUEST->m_vTerm[i]->m_dwTermID;
				bLevel = m_pQUEST->m_vTerm[i]->m_bCount;

				MAPTSKILLTEMP::iterator itTEMP = pSKILLTEMP->find(wSkill);
				if( itTEMP != pSKILLTEMP->end() )
				{
					pPlayer->ForceMaintain(wSkill, pPlayer->m_dwID, OT_PC, pPlayer->m_dwID, OT_PC, 0);
					bRun = TRUE;
				}
			}

		if(bRun)
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