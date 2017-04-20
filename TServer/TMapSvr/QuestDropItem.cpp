// QuestDropItem.cpp: implementation of the CQuestDropItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestDropItem::CQuestDropItem()
{
}

CQuestDropItem::~CQuestDropItem()
{
}

void CQuestDropItem::ExecQuest( CTPlayer *pPlayer,
							    DWORD dwTick,
							    LPMAPMAPVQUESTTEMP pTRIGGER,
								LPMAPQUESTTEMP pQUESTTEMP,
								LPMAPTITEMTEMP pITEMTEMP,
								LPMAPTSKILLTEMP pSKILLTEMP,
								LPMAPTMONSTERTEMP pMONTEMP,
								LPMAPCLASS pCLASS)
{
	if(!m_pMap)
		return;

	VQUESTTERM vTERM;
	vTERM.clear();

	for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
		if( m_pQUEST->m_vTerm[i]->m_bTermType == QTT_ITEMID )
			vTERM.push_back(m_pQUEST->m_vTerm[i]);

	if(!vTERM.empty())
	{
		int nRand = rand() % INT(vTERM.size());
		MAPTITEMTEMP::iterator finder = pITEMTEMP->find((WORD)vTERM[nRand]->m_dwTermID);

		if( finder != pITEMTEMP->end() )
		{
			if(!vTERM[nRand]->m_bCount)
				return;

			CTItem *pItem = new CTItem();

			pItem->m_wItemID = (*finder).second->m_wItemID;
			pItem->m_pTITEM = (*finder).second;
			pItem->m_dwOwnerID = pPlayer->m_dwID;
			pItem->m_bCount	= vTERM[nRand]->m_bCount;

			CTMonster * pMon = m_pMap->FindMonster(m_dwMonID);
			if(pMon)
				pMon->AddItem(pItem);
		}
	}

	vTERM.clear();
}
