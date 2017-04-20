#include "stdafx.h"
#include "Resource.h"
#include "TClientGame.h"
#include "TClientWnd.h"
#include "TNewQuestDlg.h"


CPoint CTNewQuestDlg::m_ptPOS = CPoint(0,0);

struct binary_qclass : public binary_function< CTClientQClass *, CTClientQClass *, bool>
{
	bool operator () ( const CTClientQClass * &_Left, const CTClientQClass * &_Right) const
	{
		return _Left->m_pTQCLASS && _Left->m_pTQCLASS->m_bMAIN ? true : false;
	};
};

struct binary_tquest : public binary_function< CTClientQuest *, CTClientQuest *, bool>
{
	bool operator () ( const CTClientQuest * &_Left, const CTClientQuest * &_Right) const
	{
		return binary_quest()( _Left->m_pTQUEST, _Right->m_pTQUEST);
	};
};

CTNewQuestDlg::CTNewQuestDlg( TComponent* pParent, LP_FRAMEDESC pDesc, CTClientChar* pHost)
: CTClientUIBase( pParent, pDesc), m_pHost(pHost)
{
	static DWORD dwItem[TREWARDITEMCOUNT] =
	{
		ID_CTRLINST_ITEM_1,
		ID_CTRLINST_ITEM_2
	};

	static DWORD dwSkill[TREWARDITEMCOUNT] =
	{
		ID_CTRLINST_SKILL_1,
		ID_CTRLINST_SKILL_2
	};

	static DWORD dwSel[TREWARDITEMCOUNT] =
	{
		ID_CTRLINST_SEL_1,
		ID_CTRLINST_SEL_2
	};

	m_pSummary = (TList*) FindKid( ID_CTRLINST_SUMMARY );
	m_pTerm = (TList*) FindKid( ID_CTRLINST_TERM );
	m_pContents = (TList*) FindKid( ID_CTRLINST_CONTENTS );
	m_pReward = (TList*) FindKid( ID_CTRLINST_REWARD );
	m_pAccept = (TButton*) FindKid( ID_CTRLINST_OK );
	m_pRefuse = (TButton*) FindKid( ID_CTRLINST_CANCEL );

	m_pSummary->m_nWheelMoveValue = 1;
	m_pTerm->m_nWheelMoveValue = 1;
	m_pContents->m_nWheelMoveValue = 1;
	m_pReward->m_nWheelMoveValue = 1;
	
	for( INT i=0; i < TREWARDITEMCOUNT ; ++i)
	{
		m_pITEM[i] = (TImageList*) FindKid( dwItem[i]);
		m_pSKILL[i] = (TImageList*) FindKid( dwSkill[i]);
		m_pSEL[i] = FindKid( dwSel[i] );
	}

	m_pREFUSE = (TButton *) FindKid(ID_CTRLINST_CANCEL);
	m_pACCEPT = (TButton *) FindKid(ID_CTRLINST_OK);
	m_pMONEY[0] = FindKid( ID_CTRLINST_GOLD );
	m_pMONEY[1] = FindKid( ID_CTRLINST_LUNA );
	m_pMONEY[2] = FindKid( ID_CTRLINST_CRON );
	m_pEXP = FindKid( ID_CTRLINST_EXP );

	m_pContents->ApplyUserColor(TRUE);

	m_pTTOPREWARD = NULL;
	m_pTSELREWARD = NULL;
	m_pTQUEST = NULL;
	m_pHost = pHost;
	m_bPrintMSG = TRUE;
	m_pQuestDlg = this;
	m_strNPCTalk = "";
	m_strNPCTitle = "";
	m_strAnswerWhenNPCTalk = "";

	m_dwCompleteID = TSTR_GIVEUP;
	m_dwAcceptID = TSTR_GIVEUP;

	m_pAccept->m_menu[TNM_LCLICK] = GM_QUEST_GIVEUP;
	m_pRefuse->m_menu[TNM_LCLICK] = GM_CLOSE_UI;

	m_pRefuse->m_strText = CTChart::LoadString( TSTR_CLOSE );

	// Quest Tree 초기화
	m_pQLIST = static_cast<TList*>( FindKid(ID_CTRLINST_TREE) );
	m_pQLIST->ApplyUserColor(TRUE);

	TButton* pCTMP = static_cast<TButton*>( FindKid(ID_CTRLINST_COLLAPSE) );
	TButton* pETMP = static_cast<TButton*>( FindKid(ID_CTRLINST_EXPAND) );
	TButton* pBTMP = static_cast<TButton*>( FindKid(ID_CTRLINST_CHECK) );
	
	if( pCTMP && pETMP )
	{
		RemoveKid(pCTMP);
		RemoveKid(pETMP);
		RemoveKid(pBTMP);

		CRect rcPOS;
		pCTMP->GetComponentRect(&rcPOS);
		CSize szITEM = m_pQLIST->GetItemSize(0);

		m_nGBTNCOUNT = m_pQLIST->GetItemPerPage();
		m_vTCOLLAPSE = new TButton*[m_nGBTNCOUNT];
		m_vTEXPEND = new TButton*[m_nGBTNCOUNT];
		m_vTCHECK = new TButton*[m_nGBTNCOUNT];
		
		for(INT i=0; i<m_nGBTNCOUNT; ++i)
		{
			m_vTCOLLAPSE[i] = new TButton(this, pCTMP->m_pDESC);
			m_vTEXPEND[i] = new TButton(this, pETMP->m_pDESC);
			m_vTCHECK[i] = new TButton(this, pBTMP->m_pDESC);

			m_vTCOLLAPSE[i]->m_id = GetUniqueID();
			m_vTEXPEND[i]->m_id = GetUniqueID();
			m_vTCHECK[i]->m_id = GetUniqueID();
			m_vTCHECK[i]->SetStyle(TBS_STATE_BUTTON);

			AddKid(m_vTCOLLAPSE[i]);
			AddKid(m_vTEXPEND[i]);
			AddKid(m_vTCHECK[i]);

			m_vTCOLLAPSE[i]->MoveComponent(rcPOS.TopLeft());
			m_vTEXPEND[i]->MoveComponent(rcPOS.TopLeft());
			m_vTCHECK[i]->MoveComponent(rcPOS.TopLeft());

			m_vTCOLLAPSE[i]->ShowComponent(FALSE);
			m_vTEXPEND[i]->ShowComponent(FALSE);
			m_vTCHECK[i]->ShowComponent(FALSE);

			rcPOS.top += szITEM.cy;
		}	

		delete pCTMP;
		delete pETMP;
		delete pBTMP;
	}
	else
		m_nGBTNCOUNT = 0;

	m_pStatusIcon[0] = (TImageList*) FindKid(ID_CTRLINST_STATUS_ICON0);
	m_pStatusIcon[1] = (TImageList*) FindKid(ID_CTRLINST_STATUS_ICON1);
	m_pStatusIcon[2] = (TImageList*) FindKid(ID_CTRLINST_STATUS_ICON2);
	m_pStatusIcon[3] = (TImageList*) FindKid(ID_CTRLINST_STATUS_ICON3);
	m_pStatusIcon[4] = (TImageList*) FindKid(ID_CTRLINST_STATUS_ICON4);
}

CTNewQuestDlg::~CTNewQuestDlg(void)
{
	if( m_vTCOLLAPSE )
		delete [] m_vTCOLLAPSE;

	if( m_vTEXPEND )
		delete [] m_vTEXPEND;

	if( m_vTCHECK )
		delete [] m_vTCHECK;
}

HRESULT CTNewQuestDlg::Render( DWORD dwTickCount )
{
	if(IsVisible())
	{
		INT nIndex = this->GetSel();

		LPTQUEST pTQUEST = NULL;
		if( nIndex >= 0 )
			pTQUEST = this->GetTQUEST(nIndex);

		if( m_pTQUEST != pTQUEST )
			ResetTQUEST(pTQUEST);

		if(m_bPrintMSG)
			ResetButton();

		LPTREWARD pTREWARD[TREWARDITEMCOUNT] = { NULL, NULL};
		nIndex = m_pReward->GetTop();

		for( BYTE i=0; i<TREWARDITEMCOUNT; i++)
		{
			if( m_pReward->GetItemCount() > 0 && nIndex >= 0 )
			{
				DWORD dwValue = m_pReward->GetItemData( nIndex, i);

				if( dwValue == -1 )
                    pTREWARD[i] = NULL;
				else
					pTREWARD[i] = (LPTREWARD)dwValue;
			}

			if(!pTREWARD[i])
			{
				m_pITEM[i]->ShowComponent(FALSE);
				m_pSKILL[i]->ShowComponent(FALSE);
			}

			if( m_pSEL[i] )
				m_pSEL[i]->ShowComponent(FALSE);

			if( m_pTQUEST &&
				m_pTQUEST->m_bType == QT_COMPLETE &&
				!m_bPrintMSG &&
				pTREWARD[i] &&
				pTREWARD[i]->m_bMethod == RM_SELECT &&
				(!m_pTSELREWARD || m_pTSELREWARD == pTREWARD[i]) )
			{
				if( m_pSEL[i] )
					m_pSEL[i]->ShowComponent(TRUE);

				m_pTSELREWARD = pTREWARD[i];
			}
		}

		if( m_pTTOPREWARD != pTREWARD[0] )
		{
			m_pTTOPREWARD = pTREWARD[0];

			for( i=0; i<TREWARDITEMCOUNT; i++)
			{
				m_pSKILL[i]->ShowComponent(FALSE);
				m_pITEM[i]->ShowComponent(FALSE);

				if(pTREWARD[i])
					switch(pTREWARD[i]->m_bType)
					{
					case RT_SKILL	:
					case RT_SKILLUP	:
						{
							LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(WORD(pTREWARD[i]->m_dwID));

							if(pTSKILL)
							{
								m_pSKILL[i]->SetCurImage(pTSKILL->m_wIconID);
								m_pSKILL[i]->ShowComponent(TRUE);
							}
						}
						break;

					case RT_ITEM		:
					case RT_MAGICITEM	:
						{
							LPTITEM pTITEM = NULL;
							if( pTREWARD[i]->m_bType == RT_ITEM )
								pTITEM = CTChart::FindTITEMTEMP((WORD)pTREWARD[i]->m_dwID);
							else
							{
								LPTQUESTITEM pQuestItem = CTChart::FindTQUESTMAGICITEM( (WORD)pTREWARD[i]->m_dwID );
								if( pQuestItem )
									pTITEM = CTChart::FindTITEMTEMP( pQuestItem->m_wItemID );
								else
									pTITEM = NULL;
							}

							if( pTITEM )
							{
								if( pTREWARD[i]->m_bCount > 1 )
									m_pITEM[i]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pTREWARD[i]->m_bCount);
								else
									m_pITEM[i]->m_strText.Empty();

								LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTITEM);
								WORD wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;

								m_pITEM[i]->SetCurImage(wImg);
								m_pITEM[i]->ShowComponent(TRUE);
							}
						}
						break;
					}
			}
		}

		if( m_bPrintMSG )
		{
			int nCount = m_pQLIST->GetItemCount();
			int nTop = m_pQLIST->GetTop();
			for( INT i=0 ; i <5 ; ++i )
			{
				int nIndex = nTop + i;
				if( m_pStatusIcon[ i ] )
					m_pStatusIcon[ i ]->SetCurImage(0);

				if( !m_pQLIST->GetItemString( nIndex, COL_STATE ).IsEmpty() )
				{
					CTClientQuest* pTQuest = GetCQUEST(nIndex);
					if( pTQuest && m_pStatusIcon[ i ] )
					{
						switch( pTQuest->GetResult() )
						{
						case TTERMRESULT_COMPLETE: m_pStatusIcon[ i ]->SetCurImage(1); break;
						case TTERMRESULT_FAILED: m_pStatusIcon[ i ]->SetCurImage(2); break;
						case TTERMRESULT_TIMEOUT: m_pStatusIcon[ i ]->SetCurImage(3); break;
						}
					}
				}
			}
		}
		else
		{
			for( INT i=0 ; i <5 ; ++i )
				if( m_pStatusIcon[ i ] )
					m_pStatusIcon[ i ]->SetCurImage(0);
		}
	}

	return CTClientUIBase::Render(dwTickCount);
}

void CTNewQuestDlg::OnLButtonUp(UINT nFlags, CPoint pt)
{
	GetComponentPos( &CTNewQuestDlg::m_ptPOS);
	CTClientUIBase::OnLButtonUp( nFlags, pt);
}

void CTNewQuestDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnLButtonDown( nFlags, pt);

	if( !m_bPrintMSG && IsVisible() )
	{
		int nIndex = m_pReward->GetTop();

		if( m_pReward->GetItemCount() > 0 && nIndex >= 0 )
			for( BYTE i=0; i<TREWARDITEMCOUNT; i++)
			{
				LPTREWARD pTREWARD = (LPTREWARD) m_pReward->GetItemData( nIndex, i);

				if( pTREWARD && (DWORD)(pTREWARD) != (DWORD)(-1) &&
					pTREWARD->m_bMethod == RM_SELECT )
				{
					CRect rect;

					m_pSEL[i]->GetComponentRect(&rect);
					m_pSEL[i]->ComponentToScreen(&rect);

					if(rect.PtInRect(pt))
						m_pTSELREWARD = pTREWARD;
				}
			}
	}

	for(INT i=0; i<m_nGBTNCOUNT; ++i)
	{
		if( m_vTCOLLAPSE[i]->HitTest(pt) )
		{
			ExpendTree(i, FALSE);
			break;
		}

		if( m_vTEXPEND[i]->HitTest(pt) )
		{
			ExpendTree(i, TRUE);
			break;
		}

		if( m_vTCHECK[i]->HitTest(pt) && m_bPrintMSG )
		{
			CheckShowRight(i);
			break;
		}
	}
}

void CTNewQuestDlg::ShowComponent(BOOL bVisible)
{
	MoveComponent( CTNewQuestDlg::m_ptPOS );
	CTClientUIBase::ShowComponent(bVisible);

	if( bVisible )
	{
		ResetTQUEST(m_pTQUEST);

		for(INT i=0; i<m_nGBTNCOUNT; ++i)
		{
			m_vTCOLLAPSE[i]->ShowComponent(FALSE);
			m_vTEXPEND[i]->ShowComponent(FALSE);
			m_vTCHECK[i]->ShowComponent(FALSE);
		}
	}
}

ITDetailInfoPtr CTNewQuestDlg::GetTInfoKey(const CPoint& point)
{
	ITDetailInfoPtr pInfo;
	int nIndex = m_pReward->GetTop();

	for( BYTE i=0; i<TREWARDITEMCOUNT; i++)
	{
		LPTREWARD pTREWARD = NULL;

		if( m_pReward->GetItemCount() > 0 && nIndex >= 0 )
			pTREWARD = (LPTREWARD) m_pReward->GetItemData( nIndex, i );

		if(pTREWARD && (DWORD)(pTREWARD) != (DWORD)(-1) )
		{
			switch(pTREWARD->m_bType)
			{
			case RT_SKILL		:
			case RT_SKILLUP		:
				if(m_pSKILL[i]->HitTest(point))
				{
					LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(WORD(pTREWARD->m_dwID));

					if(pTSKILL)
					{
						CRect rc;
						GetComponentRect(&rc);

						if ( pTREWARD->m_bType == RT_SKILL )
						{
							pInfo = CTDetailInfoManager::NewSkillInst(
								pTSKILL, 1, FALSE, rc);
						}
						else if ( pTREWARD->m_bType == RT_SKILLUP )
						{
							pInfo = CTDetailInfoManager::NewSkillInst(
								pTSKILL, pTREWARD->m_bCount, FALSE, rc);
						}
					}
				}
				break;

			case RT_ITEM		:
			case RT_MAGICITEM	:
				if(m_pITEM[i]->HitTest(point))
				{
					CRect rc;
					GetComponentRect(&rc);

					if( pTREWARD->m_bType == RT_ITEM )
					{
						LPTITEM pTITEM = CTChart::FindTITEMTEMP(WORD(pTREWARD->m_dwID));
						pInfo = CTDetailInfoManager::NewItemInst(pTITEM, rc);
					}
					else
					{
						CTClientItem* pCItem = CTClientItem::FindTQuestMagicClientItem( (WORD) pTREWARD->m_dwID );
						pInfo = CTDetailInfoManager::NewItemInst(pCItem, rc);
					}
				}
				break;
			}
		}
	}

	return pInfo;
}

BOOL CTNewQuestDlg::GetTChatInfo(const CPoint& point, TCHATINFO& outInfo)
{
	if( m_pQLIST->HitTest(point) )
	{
		INT nColumn = 0;
		TListItem* pListItem = m_pQLIST->GetHitItem(point, &nColumn);
		if( !pListItem )
			return FALSE;

		DWORD dwID = pListItem->m_param;

		if( dwID && dwID != (DWORD)(-1) && nColumn == COL_ITEM )
		{
			LPTQUEST pTQUEST = NULL;

			if( m_bPrintMSG )
				pTQUEST = ((CTClientQuest*)(dwID))->m_pTQUEST;
			else
				pTQUEST = (LPTQUEST)dwID;

			if( pTQUEST )
			{
				outInfo.m_Type = TTEXT_LINK_TYPE_QUEST;
				outInfo.m_pQuest = pTQUEST;

				return TRUE;
			}
		}

		return FALSE;
	}
	else
	{
		int nIndex = m_pReward->GetTop();

		for( BYTE i=0; i<TREWARDITEMCOUNT; ++i )
		{
			LPTREWARD pTREWARD = NULL;

			if( m_pReward->GetItemCount() > 0 && nIndex >= 0 )
				pTREWARD = (LPTREWARD) m_pReward->GetItemData( nIndex, i);

			if( pTREWARD && (DWORD)(pTREWARD) != (DWORD)(-1) &&
				(pTREWARD->m_bType == RT_ITEM || pTREWARD->m_bType == RT_MAGICITEM) )
			{
				if(m_pITEM[i]->HitTest(point))
				{
					CTClientItem* pCItem;
					LPTITEM pTITEM;

					if( pTREWARD->m_bType == RT_ITEM )
					{
						pCItem = NULL;
						pTITEM = CTChart::FindTITEMTEMP(WORD(pTREWARD->m_dwID));
					}
					else
					{
						pCItem = CTClientItem::FindTQuestMagicClientItem( (WORD) pTREWARD->m_dwID );
						if( pCItem )
							pTITEM = pCItem->GetTITEM();
						else
							pTITEM = NULL;
					}

					if( pTITEM )
					{
						outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
						outInfo.m_pItem = pTITEM;
						outInfo.m_pClientItem = pCItem;
						
						return TRUE;
					}

					return FALSE;
				}
			}
		}
	}

	return FALSE;
}

void CTNewQuestDlg::Reset()
{
	RemoveAll();
	m_pTTOPREWARD = NULL;
	ResetTQUEST(NULL);
}

void CTNewQuestDlg::ResetTQUEST( LPTQUEST pTQUEST)
{
	LPTQUEST pTMISSION = pTQUEST;
	m_pTQUEST = pTQUEST;

	BOOL bCanPerf = TRUE;

	if( m_pTQUEST && m_pTQUEST->m_bType == QT_COMPLETE )
	{
		pTMISSION = CTChart::FindTMISSION(m_pTQUEST);

		if( !pTMISSION || pTMISSION->m_bType != QT_MISSION )
			m_pTQUEST = NULL;
		m_pACCEPT->m_strText = CTChart::LoadString( (TSTRING) m_dwCompleteID);
	}
	else if( m_pTQUEST && m_pTQUEST->m_bType == QT_MISSION )
	{
		CTClientGame* pTGAME = CTClientGame::GetInstance();
		CTClientObjBase* pTARGET = pTGAME->GetTargetObj();
		if( pTARGET )
		{
			MAPDWORD::iterator findCOND = pTARGET->m_mapTQUESTCOND.find(m_pTQUEST->m_dwID);
			if( findCOND != pTARGET->m_mapTQUESTCOND.end() )
			{
				if( findCOND->second != QCT_NONE )
					bCanPerf = FALSE;
			}
		}

		m_pACCEPT->m_strText = CTChart::LoadString( (TSTRING) m_dwAcceptID);
	}
	else
		m_pACCEPT->m_strText = CTChart::LoadString( (TSTRING) m_dwAcceptID);

	if(m_pTQUEST && bCanPerf)
		m_pACCEPT->EnableComponent(TRUE);
	else
		m_pACCEPT->EnableComponent(FALSE);

	m_pSummary->RemoveAll();
	m_pTerm->RemoveAll();
	m_pContents->RemoveAll();
	m_pReward->RemoveAll();
	m_pMONEY[0]->m_strText = "";
	m_pMONEY[1]->m_strText = "";
	m_pMONEY[2]->m_strText = "";
	m_pEXP->m_strText = "";

	int nIndex = 0;
	if(m_pTQUEST)
	{
		m_strNPCTitle = m_pTQUEST->m_strNPCName;

		if(!bCanPerf)
		{
			// 할수없는퀘스트
			CString strCANNOTMSG;
			strCANNOTMSG = CTChart::LoadString( TSTR_QUEST_CANNOTPERF);

			SummaryMessage( strCANNOTMSG);
		}
		else
		{
			// 목표정보
			INT nTermLine = 0;
			INT nNumber = 1;
			for( BYTE i=0; i<INT(pTMISSION->m_vTTERM.size()); i++)
			{
				if( pTMISSION->m_vTTERM[i]->m_bType != QTT_TIMER )
				{
					CString strRESULT;

					CTClientQuest *pTQuest = m_pHost->FindTQuest(pTMISSION->m_dwID);
					if(pTQuest)
					{
						CTClientTerm *pTTerm = pTQuest->FindTTerm(pTMISSION->m_vTTERM[i]);
						if(pTTerm)
							switch(pTTerm->GetResult())
							{
							case TTERMRESULT_INCOMPLETE	:
								strRESULT = CTChart::Format( TSTR_QUEST_STATUS, pTTerm->m_bCount, pTMISSION->m_vTTERM[i]->m_bCount);
								break;

							case TTERMRESULT_TIMEOUT	: strRESULT = CTChart::LoadString( TSTR_QRESULT_TIMEOUT); break;
							case TTERMRESULT_FAILED		: strRESULT = CTChart::LoadString( TSTR_QRESULT_FAILED); break;
							case TTERMRESULT_COMPLETE	: strRESULT = CTChart::LoadString( TSTR_QRESULT_COMPLETE); break;
							}
						else
						{
							strRESULT = CTChart::Format( TSTR_QUEST_STATUS, 0, pTMISSION->m_vTTERM[i]->m_bCount);
						}
					}

					TermMessage(
						pTMISSION->m_vTTERM[i]->m_strTermMSG,
						strRESULT,
						nTermLine,
						nNumber);
				}
			}

			// 요약정보
			SummaryMessage( pTMISSION->m_strSummaryMSG);

			// 대화내용
			if(m_bPrintMSG)
			{
				CString strHeader = GetSpeakerString(m_strNPCTitle);
				TextMessage( strHeader, pTMISSION->m_strTriggerMSG);

				strHeader = GetSpeakerString(m_pHost->GetName());
				TextMessage( strHeader, pTMISSION->m_strReply);

				strHeader = GetSpeakerString(m_strNPCTitle);
				TextMessage( strHeader, pTMISSION->m_strAcceptMSG);
			}

			INT nRewardItemCount = 0;

			// 보상목록
			 for( i=0; i<INT(pTMISSION->m_vTREWARD.size()); i++)
			{
				CString strREWARD;
				strREWARD.Empty();

				switch(pTMISSION->m_vTREWARD[i]->m_bType)
				{
				case RT_SKILL	:
				case RT_SKILLUP	:
					{
						LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(WORD(pTMISSION->m_vTREWARD[i]->m_dwID));
						CTClientSkill* pTHOSTSKILL = m_pHost->FindTSkill(WORD(pTMISSION->m_vTREWARD[i]->m_dwID));

						/*BOOL bHasAlready = (pTHOSTSKILL) &&
							(pTSKILL->m_dwClassID == pTHOSTSKILL->m_pTSKILL->m_dwClassID) &&
							(pTSKILL->m_bLevel == pTHOSTSKILL->m_pTSKILL->m_bLevel);*/

						if( pTSKILL &&
							TCHECK_CLASS( pTSKILL->m_dwClassID, m_pHost->m_bClassID) /*&&
							!bHasAlready*/ )
						{
							strREWARD = CTChart::Format( TSTR_FMT_SKILL_REWARD, pTSKILL->m_strNAME);
						}
					}

					break;

				case RT_ITEM	:
				case RT_MAGICITEM	:
					{
						LPTITEM pTITEM = NULL;
						if( pTMISSION->m_vTREWARD[i]->m_bType == RT_ITEM )
							pTITEM = CTChart::FindTITEMTEMP((WORD)pTMISSION->m_vTREWARD[i]->m_dwID);
						else
						{
							LPTQUESTITEM pQuestItem = CTChart::FindTQUESTMAGICITEM( (WORD)pTMISSION->m_vTREWARD[i]->m_dwID );
							if( pQuestItem )
								pTITEM = CTChart::FindTITEMTEMP( pQuestItem->m_wItemID );
							else
								pTITEM = NULL;
						}

						if( pTITEM && TCHECK_CLASS( pTITEM->m_dwClassID, m_pHost->m_bClassID) )
						{
							strREWARD = CTChart::Format( TSTR_FMT_REWARD, pTITEM->m_strNAME, pTMISSION->m_vTREWARD[i]->m_bCount);
						}
					}

					break;

				case RT_GOLD	:
					{
						//strREWARD = CTClientGame::MakeMoneyStr(pTMISSION->m_vTREWARD[i]->m_dwID);
						// 보상목록이 아니라 따로 넣어주는데가 있다.

						DWORD dwMONEY[3] = { 0, 0, 0 };

						CTClientGame::SplitMoney(
							pTMISSION->m_vTREWARD[i]->m_dwID,
							&dwMONEY[0],
							&dwMONEY[1],
							&dwMONEY[2]);

						CString strFMT;
						for( INT i=0 ; i < 3 ; ++i )
						{
							strFMT.Format( "%u", dwMONEY[i]);
							m_pMONEY[i]->m_strText = strFMT;
						}
					}

					break;

				case RT_EXP		:
					{
						CString strEXP;
						strEXP.Format( "%u", pTMISSION->m_vTREWARD[i]->m_dwID);
						m_pEXP->m_strText = strEXP;
					}

					break;
				}

				if(!strREWARD.IsEmpty())
				{
					switch(pTMISSION->m_vTREWARD[i]->m_bMethod)
					{
					case RM_SELECT	:
						{
							CString strTEXT;

							strTEXT = CTChart::LoadString( TSTR_TEXT_SELREWARD);
							strREWARD += strTEXT;
						}

						break;

					case RM_PROB	:
						{
							CString strTEXT;

							strTEXT = CTChart::Format( TSTR_FMT_PROBREWARD, pTMISSION->m_vTREWARD[i]->m_bProb, '%');
							strREWARD += strTEXT;
						}

						break;
					}

					INT nLine=0;
					INT nCol=0;

					if( (nRewardItemCount % 2) == 0 )
						nLine = m_pReward->AddString( strREWARD );
					else
					{
						nLine = m_pReward->GetItemCount()-1; // 마지막 라인
						nCol = 1;

						m_pReward->SetItemString( nLine, nCol, (LPCTSTR)strREWARD );
					}

					m_pReward->SetItemData( nLine, nCol, (DWORD) pTMISSION->m_vTREWARD[i] );
					++nRewardItemCount;
				}
			}
		}

		m_pTTOPREWARD = NULL;
		m_pTSELREWARD = NULL;
	}

	if( m_bPrintMSG == FALSE ) // NPC대화창이면
	{
		if( m_strAnswerWhenNPCTalk.GetLength() )
		{
			CString strHeader = GetSpeakerString(m_pHost->GetName());
			TextMessage( strHeader, m_strAnswerWhenNPCTalk);		
		}

		if( m_strNPCTalk.GetLength() )
		{
			CString strHeader = GetSpeakerString(m_strNPCTitle);
			TextMessage( strHeader, m_strNPCTalk );
		}
	}
}

void CTNewQuestDlg::SummaryMessage( CString strText)
{
	CSize szITEM = m_pSummary->GetItemSize(0);
	CSize szTEXT(0,0);

	CString strFORMAT;
	CString strMSG;

	int nPOS = 0;

	if( CTNationOption::MODIFY_WORD_BREAK )
	{
		CSize szSpaceSize;
		m_pSummary->GetTextExtentPoint( " ", szSpaceSize);

		CString strLINE;
		CSize szLINE(0,0);

		strMSG = strText.Tokenize( " \n", nPOS);
		while(!strMSG.IsEmpty())
		{
			strMSG.Remove('\r');
			strMSG.Replace( "%s", m_pHost->GetName());

			CSize szMSG;
			m_pSummary->GetTextExtentPoint( LPCTSTR(strMSG), szMSG);

			if( szMSG.cx+szLINE.cx <= szITEM.cx)
			{
				strLINE += strMSG;
				szLINE.cx += szMSG.cx;
			}
			else
			{
				INT nIndex = m_pSummary->AddString(_T(""));

				m_pSummary->SetItemString( nIndex, 0, strLINE);
				m_pSummary->SetUserColor( nIndex, 0, TCOLOR_QTERM_TEXT);

				szLINE.cx = szMSG.cx;
				strLINE = strMSG;
			}

			BYTE c = strText.GetAt(nPOS-1); // 무엇에 의해서 Tokenize당했나.
			if( c == ' ')
			{
				strLINE += ' ';
				szLINE.cx += szSpaceSize.cx;
			}
			else if( c == '\n')
			{
				INT nIndex = m_pSummary->AddString(_T(""));

				m_pSummary->SetItemString( nIndex, 0, strLINE);
				m_pSummary->SetUserColor( nIndex, 0, TCOLOR_QTERM_TEXT);

				strLINE.Empty();
				szLINE.cx = 0;
			}

			strMSG = strText.Tokenize( " \n", nPOS);
		}

		if(!strLINE.IsEmpty())
		{
			INT nIndex = m_pSummary->AddString(_T(""));

			m_pSummary->SetItemString( nIndex, 0, strLINE);
			m_pSummary->SetUserColor( nIndex, 0, TCOLOR_QTERM_TEXT);
		}
	}
	else
	{
		szITEM.cx -= TMSG_MARGINE;

		strMSG = strText.Tokenize( "\n", nPOS);
		while(!strMSG.IsEmpty())
		{
			strMSG.Remove('\r');
			strMSG.Replace( "%s", m_pHost->GetName());

			while(!strMSG.IsEmpty())
			{
				int nIndex = m_pSummary->AddString(_T(" "));
				m_pSummary->SetUserColor(1, 0, TCOLOR_QTERM_TEXT);

				m_pSummary->GetTextExtentPoint( LPCTSTR(strMSG), szTEXT);
				CString strMBCS = BuildMBCSInfo(strMSG);
				strFORMAT = strMSG;

				while( szTEXT.cx > szITEM.cx )
				{
					int nLength = strFORMAT.GetLength() - 1;

					strFORMAT.Delete(nLength);
					nLength--;

					if( nLength >= 0 && IS_MBCS_LEAD(strMBCS.GetAt(nLength)) )
						strFORMAT.Delete(nLength);

					m_pSummary->GetTextExtentPoint( LPCTSTR(strFORMAT), szTEXT);
				}

				int nCount = strMSG.GetLength() - strFORMAT.GetLength();
				strFORMAT.TrimLeft(' ');

				if( nCount > 0 )
					strMSG = strMSG.Right(nCount);
				else
					strMSG.Empty();

				m_pSummary->SetItemString( nIndex, 0, strFORMAT);
				m_pSummary->SetUserColor( nIndex, 0, TCOLOR_QTERM_TEXT);
			}

			strMSG = strText.Tokenize( "\n", nPOS);
		}
	}
}

void CTNewQuestDlg::TermMessage( CString strText, CString strRESULT, INT nLine, INT& nNumber )
{
	CString strFORMAT;
	CString strMSG;
	INT nPOS = 0;

	CSize szITEM = m_pTerm->GetItemSize(1);
	CSize szTEXT;

	strMSG = strText.Tokenize( "\n", nPOS);
	while(!strMSG.IsEmpty())
	{
		CString strNUMBER;

		strNUMBER = CTChart::Format( TSTR_FMT_TERM, nNumber++);
		int nIndex = m_pTerm->AddString(strNUMBER);
		m_pTerm->SetUserColor(nIndex, 0, TCOLOR_QTERM_TEXT);

		if( strRESULT.GetLength())
		{
			m_pTerm->SetItemString( nIndex, 2, strRESULT);
			m_pTerm->SetUserColor( nIndex, 2, 0xFFFFFF00);
			strRESULT.Empty();
		}

		while(!strMSG.IsEmpty())
		{
			m_pTerm->SetUserColor(nIndex, 1, TCOLOR_QTERM_TEXT);

			m_pTerm->GetTextExtentPoint( LPCTSTR(strMSG), szTEXT);
			CString strMBCS = BuildMBCSInfo(strMSG);
			strFORMAT = strMSG;

			while( szTEXT.cx > szITEM.cx )
			{
				int nLength = strFORMAT.GetLength() - 1;

				strFORMAT.Delete(nLength);
				nLength--;

				if( nLength >= 0 && IS_MBCS_LEAD(strMBCS.GetAt(nLength)) )
					strFORMAT.Delete(nLength);

				m_pTerm->GetTextExtentPoint( LPCTSTR(strFORMAT), szTEXT);
			}

			int nCount = strMSG.GetLength() - strFORMAT.GetLength();
			strFORMAT.TrimLeft(' ');

			if( nCount > 0 )
				strMSG = strMSG.Right(nCount);
			else
				strMSG.Empty();

			m_pTerm->SetItemString( nIndex, 1, strFORMAT);
			m_pTerm->SetUserColor( nIndex, 1, TCOLOR_QTERM_TEXT);

			if(strMSG.GetLength())
				nIndex = m_pTerm->AddString(_T(" "));
		}

		strMSG = strText.Tokenize( "\n", nPOS);
	}
}

void CTNewQuestDlg::TextMessage(CString strTitle, CString strText)
{
	CSize szITEM = m_pContents->GetItemSize(1);
	CSize szTEXT(0,0);

	CString strFORMAT;
	CString strMSG;

	int nPOS = 0;

	if(!strTitle.IsEmpty())
	{
		strTitle.Replace( _T("%s"), m_pHost->GetName());
		INT nIdx = m_pContents->AddString(strTitle);
		m_pContents->SetUserColor(nIdx, 0, TCOLOR_QTERM_TITLE);
	}

	if( CTNationOption::MODIFY_WORD_BREAK )
	{
		CSize szSpaceSize(0,0);
		m_pContents->GetTextExtentPoint( " ", szSpaceSize);

		CString strLINE;
		CSize szLINE(0,0);

		strMSG = strText.Tokenize( " \n", nPOS);
		while(!strMSG.IsEmpty())
		{
			strMSG.Remove('\r');
			strMSG.Replace( "%s", m_pHost->GetName());

			CSize szMSG;
			m_pContents->GetTextExtentPoint( LPCTSTR(strMSG), szMSG);

			if( szMSG.cx+szLINE.cx <= szITEM.cx)
			{
				strLINE += strMSG;
				szLINE.cx += szMSG.cx;
			}
			else
			{
				INT nIndex = m_pContents->AddString(_T(""));

				m_pContents->SetItemString( nIndex, 1, strLINE);
				m_pContents->SetUserColor( nIndex, 1, TCOLOR_QTERM_TEXT);

				szLINE.cx = szMSG.cx;
				strLINE = strMSG;
			}

			BYTE c = strText.GetAt(nPOS - 1); // 무엇에 의해서 Tokenize당했나.
			if( c == ' ')
			{
				strLINE += ' ';
				szLINE.cx += szSpaceSize.cx;
			}
			else if( c == '\n')
			{
				INT nIndex = m_pContents->AddString(_T(""));

				m_pContents->SetItemString( nIndex, 1, strLINE);
				m_pContents->SetUserColor( nIndex, 1, TCOLOR_QTERM_TEXT);

				strLINE.Empty();
				szLINE.cx = 0;

				m_pContents->AddString(_T(""));
			}

			strMSG = strText.Tokenize( " \n", nPOS);
		}

		if( !strLINE.IsEmpty())
		{
			INT nIndex = m_pContents->AddString(_T(""));

			m_pContents->SetItemString( nIndex, 1, strLINE);
			m_pContents->SetUserColor( nIndex, 1, TCOLOR_QTERM_TEXT);
		}

		m_pContents->AddString(_T(""));
	}
	else
	{
		szITEM.cx -= TMSG_MARGINE;
		strMSG = strText.Tokenize( "\n", nPOS);
		while(!strMSG.IsEmpty())
		{
			strMSG.Remove('\r');
			strMSG.Replace( "%s", m_pHost->GetName());

			while(!strMSG.IsEmpty())
			{
				int nIndex = m_pContents->AddString(_T(" "));
				m_pContents->SetUserColor(1, 0, TCOLOR_QTERM_TEXT);

				m_pContents->GetTextExtentPoint( LPCTSTR(strMSG), szTEXT);
				CString strMBCS = BuildMBCSInfo(strMSG);
				strFORMAT = strMSG;

				while( szTEXT.cx > szITEM.cx )
				{
					int nLength = strFORMAT.GetLength() - 1;

					strFORMAT.Delete(nLength);
					nLength--;

					if( nLength >= 0 && IS_MBCS_LEAD(strMBCS.GetAt(nLength)) )
						strFORMAT.Delete(nLength);

					m_pContents->GetTextExtentPoint( LPCTSTR(strFORMAT), szTEXT);
				}

				int nCount = strMSG.GetLength() - strFORMAT.GetLength();
				strFORMAT.TrimLeft(' ');

				if( nCount > 0 )
					strMSG = strMSG.Right(nCount);
				else
					strMSG.Empty();

				m_pContents->SetItemString( nIndex, 1, strFORMAT);
				m_pContents->SetUserColor( nIndex, 1, TCOLOR_QTERM_TEXT);
			}

			strMSG = strText.Tokenize( "\n", nPOS);
			m_pContents->AddString(_T(" "));
		}
	}
}

void CTNewQuestDlg::TextMessage(DWORD dwTitleID, CString strText)
{
	CString strTitle;
	strTitle = CTChart::LoadString( (TSTRING) dwTitleID);
	TextMessage( strTitle, strText);
}

CString CTNewQuestDlg::GetSpeakerString( CString strSpeaker)
{
	CString strResult(_T(""));

	strSpeaker.TrimRight();
	strSpeaker.TrimLeft();

	if(!strSpeaker.IsEmpty())
		strResult = CTChart::Format( TSTR_FMT_QUEST_SPEEKER, strSpeaker);

	return strResult;
}

CString CTNewQuestDlg::GetResultString(CTClientQuest* pTQUEST)
{
	BYTE bResult = pTQUEST->GetResult();
	CString strRESULT;
	strRESULT.Empty();

	if( bResult != TTERMRESULT_FAILED && bResult != TTERMRESULT_TIMEOUT && pTQUEST->m_bTimmer )
	{
		strRESULT = CTChart::Format( TSTR_FMT_TIMMER, (pTQUEST->m_dwTick % 3600000) / 60000, (pTQUEST->m_dwTick % 60000) / 1000);
		return strRESULT;
	}

	switch(bResult)
	{
	case TTERMRESULT_INCOMPLETE	: strRESULT = CTChart::LoadString( TSTR_QRESULT_INCOMPLETE); break;
	case TTERMRESULT_TIMEOUT	: strRESULT = CTChart::LoadString( TSTR_QRESULT_TIMEOUT); break;
	case TTERMRESULT_FAILED		: strRESULT = CTChart::LoadString( TSTR_QRESULT_FAILED); break;
	case TTERMRESULT_COMPLETE	: strRESULT = CTChart::LoadString( TSTR_QRESULT_COMPLETE); break;
	}

	return strRESULT;
}

DWORD CTNewQuestDlg::GetTQUESTColor(LPTQUEST pTQUEST)
{
	if( pTQUEST->m_bLevel + TQLEVEL_BOUND < m_pHost->m_bLevel )
		return TCOLOR_LIGHTQUEST;
	else if( m_pHost->m_bLevel + TQLEVEL_BOUND < pTQUEST->m_bLevel )
		return TCOLOR_HAVYQUEST;

	return TCOLOR_MIDDLEQUEST;
}

void CTNewQuestDlg::ExpendTree(BYTE bIndex, BYTE bExpend)
{
	int nIndex = m_pQLIST->GetTop() + bIndex;

	if( nIndex >= 0 && nIndex < m_pQLIST->GetItemCount() && !GetTQUEST(nIndex) )
	{
		CTClientQClass* pTQClass = GetQCLASS(nIndex);

		if(pTQClass)
		{
			pTQClass->m_bExpend = bExpend;
			ResetTree();
		}
	}
}

void CTNewQuestDlg::CheckShowRight( BYTE bIndex)
{
	CTClientWnd *pMainWnd = (CTClientWnd *) AfxGetMainWnd();
	int nIndex = m_pQLIST->GetTop() + bIndex;

	if( pMainWnd && nIndex >= 0 && nIndex < m_pQLIST->GetItemCount() && GetTQUEST(nIndex) )
	{
		CTClientQuest *pCQuest = GetCQUEST(nIndex);

		if(pCQuest)
		{
			CTGaugePannel *pTPANNEL = (CTGaugePannel *) pMainWnd->m_MainGame.GetFrame(TFRAME_GAUGE);

			pCQuest->m_bCheckShowRight = m_vTCHECK[bIndex]->GetState() ? TRUE : FALSE;
			if(pTPANNEL)
				pTPANNEL->UpdateQuestINFO(pCQuest);
		}
	}
}
// ----------------------------------------------------------------------------------------------------
void CTNewQuestDlg::ResetTree()
{
	CTClientQClass* pTQClass = NULL;
	LISTTQCLASS vTQCLASS;

	int nCurIndex = m_pQLIST->GetSel();
	if( nCurIndex >= 0 && m_pQuestDlg->m_pTQUEST )
		pTQClass = GetQCLASS(nCurIndex);
	m_pQLIST->RemoveAll();

	MAPTQCLASS::iterator itr = m_pHost->m_mapTQCLASS.begin();
	MAPTQCLASS::iterator end = m_pHost->m_mapTQCLASS.end();

	for(; itr != end; ++itr)
		vTQCLASS.push_back(itr->second);
	vTQCLASS.sort(binary_qclass());

	while(!vTQCLASS.empty())
	{
		CTClientQClass* pTCLASS = vTQCLASS.front();
		INT nIndex = AddQClass(pTCLASS);
		
		if( pTQClass == pTCLASS )
			nCurIndex = nIndex;

		if(pTCLASS->m_bExpend)
		{
			LISTTQUEST vTQUEST;

			MAPTQUEST::iterator itQUEST, endQUEST;
			itQUEST = pTCLASS->m_mapTQUEST.begin();
			endQUEST = pTCLASS->m_mapTQUEST.end();

			for(; itQUEST != endQUEST; ++itQUEST)
				vTQUEST.push_back(itQUEST->second);

			vTQUEST.sort(binary_tquest());

			while(!vTQUEST.empty())
			{
				CTClientQuest* pTQUEST = vTQUEST.front();
				nIndex = AddQuest(pTQUEST->m_pTQUEST, pTQUEST, FALSE);

				if( m_pQuestDlg->m_pTQUEST == pTQUEST->m_pTQUEST )
					nCurIndex = nIndex;
				
				vTQUEST.pop_front();
			}
		}

		vTQCLASS.pop_front();
	}

	nCurIndex = min( m_pQLIST->GetItemCount() - 1, nCurIndex);
	if( nCurIndex >= 0 )
		m_pQLIST->SetCurSelItem(nCurIndex);

	ResetButton();
}
// ----------------------------------------------------------------------------------------------------
void CTNewQuestDlg::ResetButton()
{
	int nCount = m_pQLIST->GetItemCount();
	int nTop = m_pQLIST->GetTop();

	for( INT i=0; i<m_nGBTNCOUNT; ++i )
	{
		int nIndex = nTop + i;
		LPTQUEST pTQUEST = GetTQUEST(nIndex);

		if( nIndex < nCount && !pTQUEST )
		{
			CTClientQClass* pTQClass = GetQCLASS(nIndex);

			m_vTCOLLAPSE[i]->EnableComponent(pTQClass->m_bExpend);
			m_vTCOLLAPSE[i]->ShowComponent(pTQClass->m_bExpend);

			m_vTEXPEND[i]->EnableComponent(!pTQClass->m_bExpend);
			m_vTEXPEND[i]->ShowComponent(!pTQClass->m_bExpend);

			m_vTCHECK[i]->EnableComponent(FALSE);
			m_vTCHECK[i]->ShowComponent(FALSE);
		}
		else
		{
			m_vTCOLLAPSE[i]->EnableComponent(FALSE);
			m_vTCOLLAPSE[i]->ShowComponent(FALSE);

			m_vTEXPEND[i]->EnableComponent(FALSE);
			m_vTEXPEND[i]->ShowComponent(FALSE);

			if( pTQUEST && m_bPrintMSG )
			{
				CTClientQuest* pCQUEST = GetCQUEST(nIndex);

				m_vTCHECK[i]->Select(pCQUEST->m_bCheckShowRight);
				m_vTCHECK[i]->EnableComponent(TRUE);
				m_vTCHECK[i]->ShowComponent(TRUE);
			}
			else
			{
				m_vTCHECK[i]->EnableComponent(FALSE);
				m_vTCHECK[i]->ShowComponent(FALSE);
			}
		}
	}
}
// ====================================================================================================
INT CTNewQuestDlg::AddQuest(LPTQUEST pTQUEST, CTClientQuest* pCQUEST, BOOL bCheckComplete)
{
	LPTQUEST pTMISSION = pTQUEST;
	int nIndex = -1;

	if( bCheckComplete && pTQUEST->m_bType == QT_COMPLETE )
		pTMISSION = CTChart::FindTMISSION(pTQUEST);

	if( m_bPrintMSG )
	{
		nIndex = m_pQLIST->AddItem(pTMISSION->m_strTITLE, (DWORD)pCQUEST, COL_ITEM);
		m_pQLIST->SetItemDataAllColumn( nIndex, (DWORD)pCQUEST );
	}
	else
	{
		nIndex = m_pQLIST->AddItem(pTMISSION->m_strTITLE, (DWORD)pTQUEST, COL_ITEM);
		m_pQLIST->SetItemDataAllColumn( nIndex, (DWORD)pTQUEST );
	}

	if( !pCQUEST )
		pCQUEST = m_pHost->FindTQuest(pTMISSION->m_dwID);

	if( pCQUEST )
		m_pQLIST->SetItemString(nIndex, COL_STATE, m_pQuestDlg->GetResultString(pCQUEST));

	if( pTQUEST->m_dwMinLevel || pTQUEST->m_dwMaxLevel )
	{
		DWORD dwMin = max(pTQUEST->m_dwMinLevel, 1);
		
		CString strLEVEL;
		if( pTQUEST->m_dwMaxLevel )
			strLEVEL = CTChart::Format( TSTR_FMT_QUESTLEVEL_LIM, dwMin, pTQUEST->m_dwMaxLevel);
		else
			strLEVEL = CTChart::Format( TSTR_FMT_QUESTLEVEL, dwMin);

		m_pQLIST->SetItemString(nIndex, COL_LEVEL, strLEVEL);
	}

	DWORD dwTCLR = m_pQuestDlg->GetTQUESTColor(pTMISSION);

	m_pQLIST->SetUserColor(nIndex, COL_ITEM, dwTCLR);
	m_pQLIST->SetUserColor(nIndex, COL_LEVEL, dwTCLR);
	m_pQLIST->SetUserColor(nIndex, COL_STATE, dwTCLR);

	return nIndex;
}
// ----------------------------------------------------------------------------------------------------
INT CTNewQuestDlg::AddQClass(CTClientQClass* pQCLASS)
{
	int nIndex = m_pQLIST->AddItem(pQCLASS->m_pTQCLASS->m_strNAME, NULL, COL_CATEGORY);
	m_pQLIST->SetItemDataAllColumn( nIndex, NULL );
	m_pQLIST->SetItemData( nIndex, COL_CATEGORY, (DWORD)pQCLASS);

	//CString strCount;
	//strCount = CTChart::Format( TSTR_FMT_COUNT, INT(pQCLASS->m_mapTQUEST.size()));
	//m_pQLIST->SetItemString(nIndex, COL_STATE, strCount);

	m_pQLIST->SetUserColor(nIndex, COL_CATEGORY, TCOLOR_QUESTCLASS);
	m_pQLIST->SetUserColor(nIndex, COL_STATE, TCOLOR_QUESTCLASS);

	return nIndex;
}
// ----------------------------------------------------------------------------------------------------
void CTNewQuestDlg::SetQuestState(INT nIdx, LPTQUEST pTQUEST)
{
	CTClientQuest *pTQuest = m_pHost->FindTQuest(pTQUEST->m_dwID);

	if( pTQuest && pTQuest->m_bTimmer )
	{
		m_pQLIST->SetItemString(nIdx, COL_STATE, m_pQuestDlg->GetResultString(pTQuest));

		CTClientWnd *pMainWnd = (CTClientWnd *) AfxGetMainWnd();
		CTGaugePannel *pTPANNEL = (CTGaugePannel *) pMainWnd->m_MainGame.GetFrame(TFRAME_GAUGE);
		pTPANNEL->UpdateQuestINFO( pTQuest );
	}
}
// ====================================================================================================
LPTQUEST CTNewQuestDlg::GetTQUEST(INT nIdx)
{
	DWORD dwDATA = m_pQLIST->GetItemData(nIdx, COL_ITEM);

	if( dwDATA != (DWORD)-1 )
	{
		if( m_bPrintMSG )
		{
			CTClientQuest* pCQUEST = (CTClientQuest*) dwDATA;

			if( pCQUEST )
				return pCQUEST->m_pTQUEST;
		}
		else
		{
			return (LPTQUEST) dwDATA;
		}	
	}

	return NULL;
}

CTClientQuest* CTNewQuestDlg::GetCQUEST(INT nIdx)
{
	DWORD dwDATA = m_pQLIST->GetItemData(nIdx, COL_ITEM);

	if( dwDATA != (DWORD)-1 )
		return (CTClientQuest*) dwDATA;
	else
		return NULL;
}
// ----------------------------------------------------------------------------------------------------
CTClientQClass* CTNewQuestDlg::GetQCLASS(INT nIdx)
{
	DWORD dwDATA = m_pQLIST->GetItemData(nIdx, COL_CATEGORY);
	if( dwDATA != (DWORD)-1 )
		return (CTClientQClass*)(dwDATA);
	else
		return NULL;
}

BOOL CTNewQuestDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	return CTClientUIBase::DoMouseWheel( nFlags, zDelta, pt);
}