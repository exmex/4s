#include "StdAfx.h"

CQuestDrop::CQuestDrop(void)
{
}

CQuestDrop::~CQuestDrop(void)
{
}

void CQuestDrop::ExecQuest( CTPlayer *pPlayer,
								   DWORD dwTick,
								   LPMAPMAPVQUESTTEMP pTRIGGER,
								   LPMAPQUESTTEMP pQUESTTEMP,
								   LPMAPTITEMTEMP pITEMTEMP,
								   LPMAPTSKILLTEMP pSKILLTEMP,
								   LPMAPTMONSTERTEMP pMONTEMP,
								   LPMAPCLASS pCLASS)
{
	for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
	{
		pPlayer->DropQuest(m_pQUEST->m_vTerm[i]->m_dwTermID);

		pPlayer->SendCS_QUESTCOMPLETE_ACK(
			QR_DROP,
			m_pQUEST->m_vTerm[i]->m_dwTermID,
			0,0,
			m_pQUEST->m_vTerm[i]->m_dwTermID);
	}
}
