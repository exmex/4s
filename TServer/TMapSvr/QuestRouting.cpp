#include "StdAfx.h"
#include ".\questrouting.h"

CQuestRouting::CQuestRouting(void)
{
}

CQuestRouting::~CQuestRouting(void)
{
}
void CQuestRouting::ExecQuest( CTPlayer *pPlayer,
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
		VWORD vItem;
		vItem.clear();
		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			if( m_pQUEST->m_vTerm[i]->m_bTermType == QTT_ITEMID )
			{
				vItem.push_back(WORD(m_pQUEST->m_vTerm[i]->m_dwTermID));
			}

		pPlayer->SendCS_NPCITEMLIST_ACK(WORD(m_pQUEST->m_dwTriggerID), &vItem);
		vItem.clear();
	}
}
