// QuestGiveSkill.cpp: implementation of the CQuestGiveSkill class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestGiveSkill::CQuestGiveSkill()
{
}

CQuestGiveSkill::~CQuestGiveSkill()
{
}

void CQuestGiveSkill::ExecQuest( CTPlayer *pPlayer,
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
		WORD wSkill = 0;
		BYTE bLevel = 0;

		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			if( m_pQUEST->m_vTerm[i]->m_bTermType == QTT_SKILLID )
			{
				wSkill = (WORD)m_pQUEST->m_vTerm[i]->m_dwTermID;
				bLevel = m_pQUEST->m_vTerm[i]->m_bCount;
			}

		MAPTSKILLTEMP::iterator itTEMP = pSKILLTEMP->find(wSkill);
		if( itTEMP != pSKILLTEMP->end() && ((*itTEMP).second->m_dwClassID & BITSHIFTID(pPlayer->m_bClass)) &&
			pPlayer->UpdateSkill( (*itTEMP).second, wSkill, max( bLevel, 1)) )
		{
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
}
