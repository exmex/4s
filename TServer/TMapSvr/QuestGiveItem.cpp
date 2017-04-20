// QuestGiveItem.cpp: implementation of the CQuestGiveItem class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TMapSvr.h"
#include "TMapSvrModule.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQuestGiveItem::CQuestGiveItem()
{
}

CQuestGiveItem::~CQuestGiveItem()
{
}

void CQuestGiveItem::ExecQuest( CTPlayer *pPlayer,
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
		DWORD dwQuestID = 0;
		WORD wItemID = 0;
		BYTE bCount = 0;
		VTITEM vTITEM;	
		vTITEM.clear();

		for( int i=0; i<INT(m_pQUEST->m_vTerm.size()); i++)
			switch(m_pQUEST->m_vTerm[i]->m_bTermType)
			{
			case QTT_ITEMID		:
				{
					wItemID = (WORD)m_pQUEST->m_vTerm[i]->m_dwTermID;
					bCount = m_pQUEST->m_vTerm[i]->m_bCount;
					MAPTITEMTEMP::iterator finder = pITEMTEMP->find(wItemID);
					if( bCount && finder != pITEMTEMP->end() )
					{
						CTItem *pITEM = new CTItem();
						pITEM->m_dlID = _AtlModule.GenItemID();
						pITEM->m_pTITEM = (*finder).second;
						_AtlModule.SetItemAttr(pITEM, 0);
						pITEM->m_wItemID = wItemID;
						pITEM->m_bCount = bCount;
						pITEM->SetDuration(FALSE);
						vTITEM.push_back(pITEM);
					}
				}

				break;

			case QTT_COMPQUEST	:
				{
					dwQuestID = m_pQUEST->m_vTerm[i]->m_dwTermID;
					CQuest * pCompQ = pPlayer->FindQuest(dwQuestID);
					if(pCompQ)
					{
						pCompQ->ExecQuest(
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
				break;
			}

		VWORD vItemID;
		VBYTE vCount;
		vItemID.clear();
		vCount.clear();

		if(!vTITEM.empty())
		{
			if(pPlayer->CanPush(&vTITEM, 0))
			{
				for(DWORD k=0; k<vTITEM.size(); k++)
				{
					if(!vTITEM[k]->m_bCount)
						continue;

					vItemID.push_back(vTITEM[k]->m_pTITEM->m_wItemID);
					vCount.push_back(vTITEM[k]->m_bCount);

#ifdef	DEF_UDPLOG
					_AtlModule.m_pUdpSocket->LogItemByNPC(LOGMAP_ITEMQUESTGET, pPlayer, NULL, vTITEM[k], 0, m_pQUEST);
#endif	//	DEF_UDPLOG
				}

				pPlayer->PushTItem(&vTITEM);

				for(size_t k=0; k<vItemID.size(); k++)
				{
					_AtlModule.CheckQuest(
						pPlayer,
						0,
						pPlayer->m_fPosX,
						pPlayer->m_fPosY,
						pPlayer->m_fPosZ,
						vItemID[k],
						QTT_GETITEM,
						TT_GETITEM,
						vCount[k]);
				}

				pPlayer->SendCS_MONITEMTAKE_ACK(MIT_SUCCESS);
			}
			else
			{
				pPlayer->DropQuest(m_pQUEST->m_dwParentID);
				pPlayer->SendCS_QUESTCOMPLETE_ACK(
					QR_INVENTORYFULL,
					m_pQUEST->m_dwParentID,
					0, 0,
					m_pQUEST->m_dwParentID);

				while(!vTITEM.empty())
				{
					delete vTITEM.back();
					vTITEM.pop_back();
				}
			}
		}

		if(dwQuestID || !vItemID.empty())
			CQuest::ExecQuest(
				pPlayer,
				dwTick,
				pTRIGGER,
				pQUESTTEMP,
				pITEMTEMP,
				pSKILLTEMP,
				pMONTEMP,
				pCLASS);

		vTITEM.clear();
		vItemID.clear();
		vCount.clear();
	}
}
