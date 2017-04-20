#include "StdAfx.h"

CQuestDeleteItem::CQuestDeleteItem(void)
{
}

CQuestDeleteItem::~CQuestDeleteItem(void)
{
}


void CQuestDeleteItem::ExecQuest(CTPlayer *pPlayer,
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
		WORD wItemID = 0;		
		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
		{
			switch(m_pQUEST->m_vTerm[i]->m_bTermType)
			{
			case QTT_ITEMID		:
				{
					wItemID = (WORD)m_pQUEST->m_vTerm[i]->m_dwTermID;					
					MAPTITEMTEMP::iterator finder = pITEMTEMP->find(wItemID);
					if( finder != pITEMTEMP->end() )
					{
						pPlayer->DeleteItem(wItemID);
					}
				}
				break;
			}
		}		
	}
}
