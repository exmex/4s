#include "StdAfx.h"
#include "TClientGame.h"
#include "TClientWnd.h"
#include "Resource.h"


CTSkillDlg::CTSkillDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase( pParent, pDesc)
{
	TComponent *pPOS = FindKid(ID_CTRLINST_TAB_WND);
	CPoint vPOS;

	m_pTTAB = (TTabCtrl *) FindKid(ID_CTRLINST_TAB);

	m_pTTAB->GetComponentPos(&vPOS);
	pPOS->GetComponentPos(&m_vPOS);
	RemoveKid(pPOS);
	delete pPOS;
	m_vPOS -= vPOS;

	m_bDropTarget = FALSE;
	m_bTRADE = FALSE;
	m_bSKILLINIT = FALSE;
	m_pMainChar = NULL;

	m_mapTSKILL.clear();
	m_pOpenByCash = NULL;
}

CTSkillDlg::~CTSkillDlg()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
}

void CTSkillDlg::SetOpenByCash( LPTOPENBYCASH pOpenByCash )
{
	m_pOpenByCash = pOpenByCash;
}

void CTSkillDlg::ClearOpenByCash()
{
	if( m_pOpenByCash )
		delete m_pOpenByCash;
	m_pOpenByCash = NULL;
}

BYTE CTSkillDlg::IsButtonVisible( LPTSKILL pTSKILL,
								  BYTE bLevel)
{
	if( !pTSKILL /*|| !m_bTRADE*/ )
		return FALSE;

	if( bLevel >= pTSKILL->m_bMaxLevel )
		return FALSE;

	if( m_mapTSKILL.find(pTSKILL->m_wSkillID) == m_mapTSKILL.end() )
		return FALSE;

	return TRUE;
}

BYTE CTSkillDlg::GetCurSel()
{
	for( BYTE i=0; i<INT(m_pTTAB->m_Frames.size()); i++)
		if(m_pTTAB->m_Frames[i]->IsVisible())
			return i;

	return INVALID_SLOT;
}

BYTE CTSkillDlg::StartSkillInitMode(BYTE bInvenID, BYTE bSlotID)
{
	/*if( m_bTRADE )
		return FALSE;*/

	m_bSKILLINIT	= TRUE;
	m_bSiInvenID	= bInvenID;
	m_bSiSlotID		= bSlotID;
	m_bSiSelectID	= 0;

	if( !IsVisible() )
		CTClientGame::GetInstance()->EnableUI(TFRAME_SKILL);

	return TRUE;
}

BYTE CTSkillDlg::EndSkillInitMode()
{
	if( !m_bSKILLINIT )
		return FALSE;

	m_bSKILLINIT	= FALSE;
	m_bSiInvenID	= INVEN_NULL;
	m_bSiSlotID		= 0;
	m_bSiSelectID	= 0;

	if( IsVisible() )
		CTClientGame::GetInstance()->DisableUI(TFRAME_SKILL);

	return TRUE;
}

void CTSkillDlg::ClearInitPossSkill()
{
	m_mapTINITSKILL.clear();
}

void CTSkillDlg::AddInitPossSkill(WORD wSkillID)
{
	CTClientSkill* pSkill = m_pMainChar->FindTSkill(wSkillID);
	if( pSkill )
		m_mapTINITSKILL.insert( std::make_pair(wSkillID, pSkill->m_pTSKILL) );
}

void CTSkillDlg::UpdateNormalMode()
{
	BYTE bCURSEL = GetCurSel();
	TFrame* pCURFRAME = m_pTTAB->m_Frames[bCURSEL];

	for( BYTE bSLOT=0; bSLOT<TSKILLSLOT_COUNT; ++bSLOT)
	{
		TGauge *pTGauge = (TGauge *) pCURFRAME->FindKid(CTClientGame::m_vTICONGAUGE[bSLOT]);
		TComponent *pTMarker = pCURFRAME->FindKid(CTClientGame::m_vTICONSM[bSLOT]);
		
		if( !pTGauge || !pTMarker )
			continue;
		
		TButton *pTADD = (TButton *) pCURFRAME->FindKid(CTClientSkill::m_vTSKILLADD[bSLOT]);
		LPTSKILL pTEMP = CTChart::FindTSKILLTEMP( CTChart::GetTSKILLID(
			m_pMainChar->m_bContryID,
			m_pMainChar->m_bClassID,
			bCURSEL, bSLOT));

		if( !pTEMP )
			continue;
		
		CTClientSkill *pTSKILL = m_pMainChar->FindTSkill(pTEMP->m_wSkillID);

		if( pTADD )
			pTADD->ShowComponent(IsButtonVisible( pTEMP, pTSKILL ? pTSKILL->m_bLevel : 0));

		pTMarker->ShowComponent(FALSE);
		pTGauge->ShowComponent(FALSE);

		if( pTSKILL )
		{
			if( pTSKILL->m_dwTick )
			{
				pTGauge->m_strText = CTClientGame::ToTimeString(pTSKILL->m_dwTick);

				pTGauge->SetGauge( pTSKILL->m_dwTick, pTSKILL->m_dwReuseTick);
				pTGauge->ShowComponent(TRUE);
			}

			if( pTSKILL->m_bLoopON )
				pTMarker->ShowComponent(TRUE);
		}
	}

	TComponent *pTGROUPPOINT = FindKid( ID_CTRLINST_USED_POINT);
	pTGROUPPOINT->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pMainChar->m_vTSKILLPOINT[bCURSEL]);

	TComponent *pTENABLEPOINT = FindKid( ID_CTRLINST_ENABLE_POINT);
	pTENABLEPOINT->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pMainChar->m_vTSKILLPOINT[TPOINT_MINE]);
}
	
void CTSkillDlg::UpdateInitMode()
{
	BYTE bCURSEL = GetCurSel();
	TFrame* pCURFRAME = m_pTTAB->m_Frames[bCURSEL];

	for( BYTE bSLOT=0; bSLOT<TSKILLSLOT_COUNT; ++bSLOT)
	{
		TGauge *pTGauge = (TGauge *) pCURFRAME->FindKid(CTClientGame::m_vTICONGAUGE[bSLOT]);
		TComponent *pTMarker = pCURFRAME->FindKid(CTClientGame::m_vTICONSM[bSLOT]);
		
		if( !pTGauge || !pTMarker )
			continue;
		
		TButton *pTADD = (TButton *) pCURFRAME->FindKid(CTClientSkill::m_vTSKILLADD[bSLOT]);
		if( pTADD )
			pTADD->ShowComponent(FALSE);

		LPTSKILL pTEMP = CTChart::FindTSKILLTEMP( CTChart::GetTSKILLID(
			m_pMainChar->m_bContryID,
			m_pMainChar->m_bClassID,
			bCURSEL, bSLOT));
		
		pTGauge->ShowComponent(FALSE);
		
		MAPTSKILLTEMP::iterator itr = m_mapTINITSKILL.find(pTEMP->m_wSkillID);
		TImageList* pTSlot = (TImageList *) pCURFRAME->FindKid(CTClientSkill::m_vTSKILLSLOT[bSLOT]);
		if( itr != m_mapTINITSKILL.end() )
		{
			pTSlot->EnableComponent(TRUE);
			pTMarker->ShowComponent(TRUE);
		}
		else
		{
			pTSlot->EnableComponent(FALSE);
			pTMarker->ShowComponent(FALSE);
		}
	}

	TComponent *pTGROUPPOINT = FindKid( ID_CTRLINST_USED_POINT);
	pTGROUPPOINT->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pMainChar->m_vTSKILLPOINT[bCURSEL]);

	TComponent *pTENABLEPOINT = FindKid( ID_CTRLINST_ENABLE_POINT);
	pTENABLEPOINT->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pMainChar->m_vTSKILLPOINT[TPOINT_MINE]);
}

void CTSkillDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( m_bSKILLINIT )
	{
		BYTE bTabID = GetCurSel();
		
		if( bTabID != INVALID_SLOT )
		{
			for( BYTE i=0; i<TSKILLSLOT_COUNT; i++)
			{
				TImageList *pTICON = (TImageList *) m_pTTAB->m_Frames[bTabID]->FindKid(CTClientSkill::m_vTSKILLSLOT[i]);

				if( pTICON && pTICON->HitTest(pt) )
				{
					WORD wSkillID = CTChart::GetTSKILLID(
										m_pMainChar->m_bContryID,
										m_pMainChar->m_bClassID,
										bTabID, i);

					MAPTSKILLTEMP::iterator itr = m_mapTINITSKILL.find(wSkillID);
					if( itr != m_mapTINITSKILL.end() )
					{
						m_bSiSelectID = wSkillID;

						CString strMSG;
						LPTSKILL pTSKILL = itr->second;
						strMSG = CTChart::Format( TSTR_WARN_SKILLINIT_ONE, pTSKILL->m_strNAME);

						CTClientWnd::GetInstance()->MessageBoxYesNo(
							strMSG,
							TSTR_YES,
							TSTR_NO,
							GM_SKILLINIT_ONE,
							GM_CLOSE_MSGBOX );
					}

					break;
				}
			}
		}
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);
}

BYTE CTSkillDlg::OnBeginDrag( LPTDRAG pDRAG,
							  CPoint point)
{
	if( m_bSKILLINIT )
		return FALSE;

	BYTE bTabID = GetCurSel();

	if( bTabID != INVALID_SLOT )
	{
		TImageList *pTIMG = NULL;
		BYTE bTreeID;

		for( BYTE i=0; i<TSKILLSLOT_COUNT; i++)
		{
			TImageList *pKid = (TImageList *) m_pTTAB->m_Frames[bTabID]->FindKid(CTClientSkill::m_vTSKILLSLOT[i]);

			if( pKid && pKid->HitTest(point) )
			{
				pTIMG = pKid;
				bTreeID = i;

				break;
			}
		}

		if( pTIMG && pTIMG->CanProcess() )
		{
			LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP( CTChart::GetTSKILLID(
				m_pMainChar->m_bContryID,
				m_pMainChar->m_bClassID,
				bTabID,
				bTreeID));

			if( pTSKILL && pTSKILL->m_bActive )
			{
				if( pDRAG )
				{
					CPoint pt = point;
					CSize size;

					pDRAG->m_pIMAGE = new TImageList(
						NULL,
						pTIMG->m_pDESC);

					pDRAG->m_pIMAGE->SetCurImage(pTSKILL->m_wIconID);
					pDRAG->m_bSlotID = bTreeID;

					pDRAG->m_pIMAGE->GetComponentSize(&size);
					pt.x -= size.cx / 2;
					pt.y -= size.cy / 2;

					pDRAG->m_pIMAGE->ShowComponent(FALSE);
					pDRAG->m_pIMAGE->MoveComponent(pt);
				}

				return TRUE;
			}
		}
	}

	return FALSE;
}

ITDetailInfoPtr CTSkillDlg::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	BYTE bTabID = GetCurSel();
	if( bTabID != INVALID_SLOT )
	{
		for( BYTE i=0; i<TSKILLSLOT_COUNT; i++)
		{
			TImageList *pTICON = (TImageList *) m_pTTAB->m_Frames[bTabID]->FindKid(CTClientSkill::m_vTSKILLSLOT[i]);

			if( pTICON && pTICON->HitTest(point) )
			{
				LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP( CTChart::GetTSKILLID(
					m_pMainChar->m_bContryID,
					m_pMainChar->m_bClassID,
					bTabID, i));

				if( pTSKILL )
				{
					CTClientSkill *pTSkill = m_pMainChar->FindTSkill(pTSKILL->m_wSkillID);

					CRect rc;
					GetComponentRect(&rc);

					pInfo = CTDetailInfoManager::NewSkillInst(
						pTSKILL, 
						pTSkill ? pTSkill->m_bLevel : 0,
						TRUE,
						rc);
				}

				break;
			}
		}
	}

	return pInfo;
}

BOOL CTSkillDlg::CanWithItemUI()
{
	return TRUE;
}

HRESULT CTSkillDlg::Render( DWORD dwTickCount)
{
	if( IsVisible() )
	{
		CPoint point;
		GetCursorPos(&point);
		m_pCommandHandler->ScreenToClient(&point);

		BYTE bCNT = (BYTE)m_pTTAB->m_Buttons.size();
		for( BYTE i=0; i<bCNT; ++i)
		{
			if(!m_pTTAB->m_Buttons[i]->IsVisible())
				continue;
			
			CGDIFont *pGDIFont = FindFont(m_pTTAB->m_Buttons[i]->IsStateDown() ? ID_FONT_LOGIN_BOLD : ID_FONT_LOGIN);

			if( m_pTTAB->m_Buttons[i]->GetFont() != pGDIFont )
				m_pTTAB->m_Buttons[i]->SetFont( pGDIFont, NULL);
		}

		if( m_bSKILLINIT )
			UpdateInitMode();
		else
			UpdateNormalMode();
	}

	return CTClientUIBase::Render(dwTickCount);
}




















