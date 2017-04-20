#include "stdafx.h"
#include "TCraftQuestDlg.h"
#include "TClientGame.h"
#include "TClientChar.h"
#include "resource.h"

TComponent* CTCraftCategory::m_pTempCategoryBar = NULL;
TComponent* CTCraftCategory::m_pTempCategoryName = NULL;
TComponent* CTCraftCategory::m_pTempCategoryPlusBtn = NULL;
TComponent* CTCraftCategory::m_pTempCategoryMinusBtn = NULL;

CPoint CTCraftCategory::m_ptPlusBtnOffset;
CPoint CTCraftCategory::m_ptMinusBtnOffset;
CPoint CTCraftCategory::m_ptNameBtnOffset;

CPoint CTCraftQuestDlg::m_ptPOS = CPoint(0,0);

CTCraftCategory::CTCraftCategory()
{
	m_bOpened = FALSE;
}

CTCraftCategory::~CTCraftCategory()
{
	DeleteAllItem();
}

void CTCraftCategory::DeleteAllItem()
{
	m_vTQuest.clear();
}

void CTCraftCategory::SetCategoryPos( CPoint ptBase )
{
	m_pCategoryBar->MoveComponent( ptBase );
	m_pPlusBtn->MoveComponent( ptBase + CTCraftCategory::m_ptPlusBtnOffset );
	m_pMinusBtn->MoveComponent( ptBase + CTCraftCategory::m_ptMinusBtnOffset );
	m_pName->MoveComponent( ptBase + CTCraftCategory::m_ptNameBtnOffset );
}

void CTCraftCategory::SetName( CString strName )
{
	if( m_pName )
		m_pName->m_strText = strName;
}

INT CTCraftCategory::CheckExpandBtn( CPoint pt )
{
	if( m_pPlusBtn && m_pPlusBtn->IsVisible() )
	{
		if( m_pPlusBtn->HitTest( pt ) )
		{
			return 1;
		}
	}

	if( m_pMinusBtn && m_pMinusBtn->IsVisible() )
	{
		if( m_pMinusBtn->HitTest( pt ) )
		{
			return -1;
		}
	}

	return 0;
}

void CTCraftCategory::ShowComponent( BOOL bVisible )
{
	if( bVisible )
	{
		m_pMinusBtn->ShowComponent( m_bOpened);
		m_pPlusBtn->ShowComponent( !m_bOpened);	
	}
	else
	{
		m_pMinusBtn->ShowComponent( FALSE);
		m_pPlusBtn->ShowComponent( FALSE);
	}
}

// =================================================


CTCraftQuestDlg::CTCraftQuestDlg( TComponent* pParent, LP_FRAMEDESC pDesc )
: CTClientUIBase( pParent, pDesc )
{
	static DWORD dwItem[TREWARDITEMCOUNT] =
	{
		ID_CTRLINST_ICON,
		ID_CTRLINST_ICON2
	};

	m_dwCurCategory = -1;

	m_pQLIST = (TList*)FindKid( ID_CTRLINST_TREE );
	m_pRewardList = (TList*)FindKid( ID_CTRLINST_REWARD );
	m_pTermList = (TList*)FindKid( ID_CTRLINST_TERM );
	m_pSummaryList = (TList*)FindKid( ID_CTRLINST_SUMMARY );
	m_pCraftOK = (TButton*)FindKid( ID_CTRLINST_OK );

	for( INT i=0; i < TREWARDITEMCOUNT ; ++i)
		m_pITEM[i] = (TImageList*) FindKid( dwItem[i]);

	m_bCraftOK = FALSE;
	m_pCraftOK->EnableComponent( FALSE );
	m_pCraftOK->SetTextClr( 0xFF888888 );

	m_pQLIST->m_nWheelMoveValue = 1;
	m_pRewardList->m_nWheelMoveValue = 1;
	m_pTermList->m_nWheelMoveValue = 1;
	m_pSummaryList->m_nWheelMoveValue = 1;

	m_pTREWARD = NULL;

	TComponent* pCategory = FindKid( ID_CTRLINST_CATEGORY_BAR );
	TComponent* pCategoryName = FindKid( ID_CTRLINST_CATEGORY_TITLE );
	TComponent* pCategoryPlusBtn = FindKid( ID_CTRLINST_BTN_PLUS);
	TComponent* pCategoryMinusBtn = FindKid( ID_CTRLINST_BTN_MINUS );

	RemoveKid( pCategory );
	RemoveKid( pCategoryName );
	RemoveKid( pCategoryPlusBtn );
	RemoveKid( pCategoryMinusBtn );

	CTCraftCategory::m_pTempCategoryBar = pCategory;
	CTCraftCategory::m_pTempCategoryName = pCategoryName;
	CTCraftCategory::m_pTempCategoryPlusBtn = pCategoryPlusBtn;
	CTCraftCategory::m_pTempCategoryMinusBtn = pCategoryMinusBtn;

	CPoint ptCategory, ptCategoryName, ptCategoryPlusBtn, ptCategoryMinusBtn;
	pCategory->GetComponentPos( &ptCategory );
	pCategoryName->GetComponentPos( &ptCategoryName );
	pCategoryPlusBtn->GetComponentPos( &ptCategoryPlusBtn );
	pCategoryMinusBtn->GetComponentPos( &ptCategoryMinusBtn );

	m_ptBasePointCategory = ptCategory;

	CTCraftCategory::m_ptNameBtnOffset = ptCategoryName - ptCategory;
	CTCraftCategory::m_ptPlusBtnOffset = ptCategoryPlusBtn - ptCategory;
	CTCraftCategory::m_ptMinusBtnOffset = ptCategoryMinusBtn - ptCategory;

	SIZE sizeCategory;
	pCategory->GetComponentSize( &sizeCategory );
	m_nCategoryHeight = sizeCategory.cy;

//	CGDIFont *pGDIFONT = FindFont( ID_FONT_QUEST_TEXT );
//	m_pQLIST->SetFont( pGDIFONT, NULL );
	m_pQLIST->ApplyUserColor(TRUE);

	m_pTQUEST = NULL;
	m_pTREWARD = NULL;
}

CTCraftQuestDlg::~CTCraftQuestDlg()
{
	MAPTCRAFTCATEGORY::iterator it, end;
	it = m_mapCategory.begin();
	end = m_mapCategory.end();

	for(; it != end ; ++it )
		delete it->second;

	m_mapCategory.clear();

	if( CTCraftCategory::m_pTempCategoryBar )
		delete CTCraftCategory::m_pTempCategoryBar;

	if( CTCraftCategory::m_pTempCategoryName )
		delete CTCraftCategory::m_pTempCategoryName;

	if( CTCraftCategory::m_pTempCategoryPlusBtn )
		delete CTCraftCategory::m_pTempCategoryPlusBtn;

	if( CTCraftCategory::m_pTempCategoryMinusBtn )
		delete CTCraftCategory::m_pTempCategoryMinusBtn;
}


HRESULT CTCraftQuestDlg::Render( DWORD dwTickCount )
{
	if( IsVisible() )
	{
		if( m_bNeedResetPos )
		{
//			ResetTQUEST( m_pTQUEST );
			ResetPos();
		}

		LPTREWARD pTREWARD[TREWARDITEMCOUNT] = { NULL, NULL};
		INT nIndex = m_pRewardList->GetTop();

		for( BYTE i=0; i<TREWARDITEMCOUNT; i++)
		{
			if( m_pRewardList->GetItemCount() > 0 && nIndex >= 0 )
			{
				DWORD dwValue = m_pRewardList->GetItemData( nIndex, i);

				if( dwValue == -1 )
                    pTREWARD[i] = NULL;
				else
					pTREWARD[i] = (LPTREWARD)dwValue;
			}

			if(!pTREWARD[i])
				m_pITEM[i]->ShowComponent(FALSE);
		}

		if( m_pTTOPREWARD != pTREWARD[0] )
		{
			m_pTTOPREWARD = pTREWARD[0];

			for( i=0; i<TREWARDITEMCOUNT; i++)
			{
				m_pITEM[i]->ShowComponent(FALSE);

				if(pTREWARD[i])
					switch(pTREWARD[i]->m_bType)
					{
					case RT_ITEM		:
					case RT_MAGICITEM	:
						{
							LPTITEM pTITEM = NULL;
							if( pTREWARD[i]->m_bType == RT_ITEM )
							{
								pTITEM = CTChart::FindTITEMTEMP( (WORD)pTREWARD[i]->m_dwID );
							}
							else
							{
								CTClientItem* pCItem = CTClientItem::FindTQuestMagicClientItem((WORD)pTREWARD[i]->m_dwID);
								if( pCItem )
									pTITEM = pCItem->GetTITEM();
								else
									pTITEM = NULL;
							}

							if( pTITEM )
							{
								if( pTREWARD[i]->m_bCount > 1 )
								{
									m_pITEM[i]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pTREWARD[i]->m_bCount);
								}
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

		return CTClientUIBase::Render( dwTickCount );
	}

	return S_OK;
}

void CTCraftQuestDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( !m_bVisible || !m_bEnable )	return ;

	CTClientUIBase::OnLButtonDown( nFlags, pt );

	MAPTCRAFTCATEGORY::iterator it = m_mapCategory.begin();
	MAPTCRAFTCATEGORY::iterator end = m_mapCategory.end();
	while( it != end )
	{
		switch( (it->second)->CheckExpandBtn( pt ) )
		{
		case 1:
			OpenCategory( it->first );
			break;
		case -1:
			OpenCategory( -1 );
			break;
		}

		++it;
	}

	if( m_pQLIST->HitTest(pt) )
	{
		int nIndex = GetSel();

		if( nIndex >= 0 )
		{
			LPTQUEST pTQUEST = NULL;
			pTQUEST = this->GetTQUEST(nIndex);

			if( m_pTQUEST != pTQUEST )
			{
				ResetTQUEST(pTQUEST);
			}
		}
		else
		{
			ResetTQUEST(NULL);
		}
	}

	if( m_pTermList->HitTest(pt) )
	{
		LPTTERM pTerm = GetSelTerm();
		if( pTerm )
		{
			SummaryMessage( pTerm->m_strFailMSG );
		}
	}
}

void CTCraftQuestDlg::OnLButtonUp(UINT nFlags, CPoint pt)
{
	m_blDragging = FALSE;
	GetComponentPos( &CTCraftQuestDlg::m_ptPOS);
	CTClientUIBase::OnLButtonUp( nFlags, pt );
}

void CTCraftQuestDlg::ShowComponent(BOOL bVisible)
{
	CTClientUIBase::ShowComponent( bVisible );

	CTClientGame* pGame = CTClientGame::GetInstance();
	pGame->m_bCanUseSkill = !bVisible;
	pGame->m_bCanUseItem = !bVisible;

	if( bVisible )
	{
		ResetTree();
		ResetTQUEST(NULL);
	}
}

ITDetailInfoPtr CTCraftQuestDlg::GetTInfoKey(const CPoint& point)
{
	ITDetailInfoPtr pInfo;
	int nIndex = m_pRewardList->GetTop();

	for( BYTE i=0; i<TREWARDITEMCOUNT; i++)
	{
		LPTREWARD pTREWARD = NULL;

		if( m_pRewardList->GetItemCount() > 0 && nIndex >= 0 )
			pTREWARD = (LPTREWARD) m_pRewardList->GetItemData( nIndex, i );

		if(pTREWARD && (DWORD)(pTREWARD) != (DWORD)(-1) )
		{
			switch(pTREWARD->m_bType)
			{
			case RT_ITEM		:
			case RT_MAGICITEM	:
				if(m_pITEM[i]->HitTest(point))
				{
					CRect rc;
					GetComponentRect(&rc);

					if( pTREWARD->m_bType == RT_ITEM )
					{
						LPTITEM pTITEM = CTChart::FindTITEMTEMP((WORD)pTREWARD->m_dwID);
						pInfo = CTDetailInfoManager::NewItemInst(pTITEM, rc);
					}
					else
					{
						CTClientItem* pCItem = CTClientItem::FindTQuestMagicClientItem((WORD)pTREWARD->m_dwID);
						pInfo = CTDetailInfoManager::NewItemInst(pCItem, rc);
					}
				}
				break;
			}
		}
	}

	return pInfo;
}

BOOL CTCraftQuestDlg::GetTChatInfo(const CPoint& point, TCHATINFO& outInfo)
{
	int nIndex = m_pRewardList->GetTop();

	for( BYTE i=0; i<TREWARDITEMCOUNT; i++)
	{
		LPTREWARD pTREWARD = NULL;

		if( m_pRewardList->GetItemCount() > 0 && nIndex >= 0 )
			pTREWARD = (LPTREWARD) m_pRewardList->GetItemData( nIndex, i );

		if(pTREWARD && (DWORD)(pTREWARD) != (DWORD)(-1) )
		{
			if( m_pITEM[i]->HitTest(point) )
			{
				CTClientItem* pCItem = NULL;
				LPTITEM pTITEM = NULL;

				switch( pTREWARD->m_bType )
				{
				case RT_ITEM:
					{
						pTITEM = CTChart::FindTITEMTEMP(WORD(pTREWARD->m_dwID));
					}
					break;

				case RT_MAGICITEM:
					{
						pCItem = CTClientItem::FindTQuestMagicClientItem( (WORD) pTREWARD->m_dwID );
						if( pCItem )
							pTITEM = pCItem->GetTITEM();
					}
					break;
				}

				if( pTITEM )
				{
					outInfo.m_Type = TTEXT_LINK_TYPE_ITEM;
					outInfo.m_pItem = pTITEM;
					outInfo.m_pClientItem = pCItem;
					return TRUE;
				}
			}
		}
	}

	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
DWORD CTCraftQuestDlg::CharItemCount( DWORD dwID )
{
	CTClientChar* pChar = CTClientGame::GetInstance()->GetMainChar();

	MAPTINVEN::iterator it = pChar->m_mapTINVEN.begin();
	MAPTINVEN::iterator end = pChar->m_mapTINVEN.end();

	DWORD dwCount = 0;

	while( it != end )
	{
		MAPTITEM::iterator item_it = (*it).second->m_mapTITEM.begin();
		MAPTITEM::iterator item_end = (*it).second->m_mapTITEM.end();

		while( item_it != item_end )
		{
			if( (*item_it).second->GetItemID() == (WORD)dwID )
				dwCount += (*item_it).second->GetCount();

			++item_it;
		}

		++it;
	}

	return dwCount;
}

void CTCraftQuestDlg::ResetTerm()
{
	m_pTermList->RemoveAll();
	m_bCraftOK = TRUE;

	if( m_pTQUEST )
	{
		// 목표정보
		INT nTermLine = 0;
		INT nNumber = 1;
		for( BYTE i=0; i<INT(m_pTQUEST->m_vTTERM.size()); i++)
		{
			CString strRESULT;
			BOOL bCraftOK = TRUE;

			DWORD item_count = CharItemCount( m_pTQUEST->m_vTTERM[i]->m_dwID );

			if( m_pTQUEST->m_vTTERM[i]->m_bCount <= item_count )
			{
//				item_count = m_pTQUEST->m_vTTERM[i]->m_bCount;

				if( m_bCraftOK )
					m_bCraftOK = TRUE;
			}
			else
			{
				m_bCraftOK = FALSE;
				bCraftOK = FALSE;
			}

			strRESULT = CTChart::Format( TSTR_QUEST_STATUS, item_count,
				m_pTQUEST->m_vTTERM[i]->m_bCount);

			TermMessage(
				m_pTQUEST->m_vTTERM[i]->m_strTermMSG,
				strRESULT,
				nTermLine,
				nNumber,
				bCraftOK);
		}

		if( m_bCraftOK )
		{
			m_dwQuestID = m_pTQUEST->m_dwID;
			m_pCraftOK->EnableComponent( TRUE );
			m_pCraftOK->SetTextClr( TCOLOR_TXT_NORMAL );
		}
		else
		{
			m_pCraftOK->EnableComponent( FALSE );
			m_pCraftOK->SetTextClr( 0xFF888888 );
		}
	}
}

void CTCraftQuestDlg::ResetTQUEST( LPTQUEST pTQuest )
{
	m_bCraftOK = FALSE;
	m_pTQUEST = pTQuest;

	m_pSummaryList->RemoveAll();
	m_pTermList->RemoveAll();
	m_pRewardList->RemoveAll();

	int nIndex = 0;
	if(m_pTQUEST)
	{
		// 목표정보
		ResetTerm();
		m_pTermList->SetCurSelItem(0);

		LPTTERM pTerm = GetSelTerm();
		if( pTerm )
		{
			SummaryMessage( pTerm->m_strFailMSG );
		}

		INT nRewardItemCount = 0;

		// 보상목록
		for( int i=0; i<INT(pTQuest->m_vTREWARD.size()); i++)
		{
			CString strREWARD;
			strREWARD.Empty();

			switch(pTQuest->m_vTREWARD[i]->m_bType)
			{
			case RT_ITEM	:
			case RT_MAGICITEM	:
				{
					LPTITEM pTITEM = NULL;
					if( pTQuest->m_vTREWARD[i]->m_bType == RT_ITEM )
						pTITEM = CTChart::FindTITEMTEMP((WORD)pTQuest->m_vTREWARD[i]->m_dwID);
					else
					{
						CTClientItem* pCItem = CTClientItem::FindTQuestMagicClientItem((WORD)pTQuest->m_vTREWARD[i]->m_dwID);
						if( pCItem )
							pTITEM = pCItem->GetTITEM();
						else
							pTITEM = NULL;
					}

					if( pTITEM )
					{
						strREWARD = CTChart::Format(
							TSTR_FMT_REWARD,
							pTITEM->m_strNAME,
							pTQuest->m_vTREWARD[i]->m_bCount);
					}
				}

				break;

			}

			if(!strREWARD.IsEmpty())
			{
				switch(pTQuest->m_vTREWARD[i]->m_bMethod)
				{
				case RM_SELECT	:
					{
						CString strTEXT;

						strTEXT = CTChart::LoadString( TSTR_TEXT_SELREWARD );
						strREWARD += strTEXT;
					}

					break;

				case RM_PROB	:
					{
						CString strTEXT;

						strTEXT = CTChart::Format( TSTR_FMT_PROBREWARD, pTQuest->m_vTREWARD[i]->m_bProb, '%');
						strREWARD += strTEXT;
					}

					break;
				}

				INT nLine=0;
				INT nCol=0;

				if( (nRewardItemCount % 2) == 0 )
					nLine = m_pRewardList->AddString( strREWARD );
				else
				{
					nLine = m_pRewardList->GetItemCount()-1; // 마지막 라인
					nCol = 1;

					m_pRewardList->SetItemString( nLine, nCol, (LPCTSTR)strREWARD );
				}

				m_pRewardList->SetItemData( nLine, nCol, (DWORD) pTQuest->m_vTREWARD[i] );
				++nRewardItemCount;
			}
		}

		m_pTTOPREWARD = NULL;
	}

//	m_bNeedResetPos = TRUE;
}

void CTCraftQuestDlg::TermMessage( CString strText, CString strRESULT, INT nLine, INT& nNumber, BOOL bCraftOK )
{
	DWORD line_color = bCraftOK ? TCOLOR_QUEST_BLUE : TCOLOR_TXT_NORMAL;

	m_pTermList->ApplyUserColor( TRUE );

	CString strNUMBER = CTChart::Format( TSTR_FMT_TERM, nNumber++ );

	int nIndex = m_pTermList->AddString( strNUMBER, 0 );
	m_pTermList->SetUserColor( nIndex, 0, line_color );

    m_pTermList->SetItemString( nIndex, 1, strText);
	m_pTermList->SetUserColor( nIndex, 1, line_color);

	m_pTermList->SetItemString( nIndex, 2, strRESULT );
	m_pTermList->SetUserColor( nIndex, 2, line_color );
}

void CTCraftQuestDlg::SummaryMessage( CString strText)
{
	m_pSummaryList->RemoveAll();

	CSize szITEM = m_pSummaryList->GetItemSize(0);
	CSize szTEXT(0,0);

	CString strFORMAT;
	CString strMSG;

	int nPOS = 0;

#ifdef MODIFY_WORD_BREAK
	CSize szSpaceSize;
	m_pSummaryList->GetTextExtentPoint( " ", szSpaceSize);

	CString strLINE;
	CSize szLINE(0,0);

	strMSG = strText.Tokenize( " \n", nPOS);
	while(!strMSG.IsEmpty())
	{
		strMSG.Remove('\r');
		strMSG.Replace( "%s", m_pHost->GetName());

		CSize szMSG;
		m_pSummaryList->GetTextExtentPoint( LPCTSTR(strMSG), szMSG);

		if( szMSG.cx+szLINE.cx <= szITEM.cx)
		{
			strLINE += strMSG;
			szLINE.cx += szMSG.cx;
		}
		else
		{
			INT nIndex = m_pSummaryList->AddString(_T(""));

			m_pSummaryList->SetItemString( nIndex, 0, strLINE);
			m_pSummaryList->SetUserColor( nIndex, 0, TCOLOR_QTERM_TEXT);

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
			INT nIndex = m_pSummaryList->AddString(_T(""));

			m_pSummaryList->SetItemString( nIndex, 0, strLINE);
			m_pSummaryList->SetUserColor( nIndex, 0, TCOLOR_QTERM_TEXT);

			strLINE.Empty();
			szLINE.cx = 0;
		}

		strMSG = strText.Tokenize( " \n", nPOS);
	}

	if(!strLINE.IsEmpty())
	{
		INT nIndex = m_pSummaryList->AddString(_T(""));

		m_pSummaryList->SetItemString( nIndex, 0, strLINE);
		m_pSummaryList->SetUserColor( nIndex, 0, TCOLOR_QTERM_TEXT);
	}
#else
	szITEM.cx -= TMSG_MARGINE;

	strMSG = strText.Tokenize( "\n", nPOS);
	while(!strMSG.IsEmpty())
	{
		strMSG.Remove('\r');
//		strMSG.Replace( "%s", m_pHost->GetName());

		while(!strMSG.IsEmpty())
		{
			int nIndex = m_pSummaryList->AddString(_T(" "));
			m_pSummaryList->SetUserColor(1, 0, TCOLOR_QTERM_TEXT);

			m_pSummaryList->GetTextExtentPoint( LPCTSTR(strMSG), szTEXT);
			CString strMBCS = BuildMBCSInfo(strMSG);
			strFORMAT = strMSG;

			while( szTEXT.cx > szITEM.cx )
			{
				int nLength = strFORMAT.GetLength() - 1;

				strFORMAT.Delete(nLength);
				nLength--;

				if( nLength >= 0 && IS_MBCS_LEAD(strMBCS.GetAt(nLength)) )
					strFORMAT.Delete(nLength);

				m_pSummaryList->GetTextExtentPoint( LPCTSTR(strFORMAT), szTEXT);
			}

			int nCount = strMSG.GetLength() - strFORMAT.GetLength();
			strFORMAT.TrimLeft(' ');

			if( nCount > 0 )
				strMSG = strMSG.Right(nCount);
			else
				strMSG.Empty();

			m_pSummaryList->SetItemString( nIndex, 0, strFORMAT);
			m_pSummaryList->SetUserColor( nIndex, 0, TCOLOR_QTERM_TEXT);
		}

		strMSG = strText.Tokenize( "\n", nPOS);
	}
#endif
}

void CTCraftQuestDlg::RemoveAllList()
{
	m_pQLIST->RemoveAll();
	m_pRewardList->RemoveAll();
	m_pSummaryList->RemoveAll();
	m_pTermList->RemoveAll();
}

void CTCraftQuestDlg::ClearAllCategory()
{
	{
		VTCRAFTCATEGORY::iterator it, end;
		it = m_vCategoryOrders.begin();
		end = m_vCategoryOrders.end();

		for(; it != end ;++it )
		{
			(*it)->DeleteAllItem();

			RemoveKid( (*it)->m_pCategoryBar );
			delete (*it)->m_pCategoryBar;

			RemoveKid( (*it)->m_pName );
			delete (*it)->m_pName;

			RemoveKid( (*it)->m_pPlusBtn );
			delete (*it)->m_pPlusBtn;
			
			RemoveKid( (*it)->m_pMinusBtn );
			delete (*it)->m_pMinusBtn;

			delete (*it);
		}

		m_vCategoryOrders.clear();
		m_mapCategory.clear();
	}

	m_bNeedResetPos = TRUE;
}

//////////////////////////////////////////////////////////////////////////

void CTCraftQuestDlg::ResetTree()
{
	RemoveAllList();
	ClearAllCategory();
	m_bCraftOK = FALSE;

	CTClientGame* pGame = CTClientGame::GetInstance();

	CTClientObjBase* pNPC = static_cast<CTClientNpc*>(pGame->GetTargetObj());

	if( !pNPC )
		return ;

	MAPTQUESTTEMP::iterator itor = pNPC->m_mapTQUESTTEMP.begin();
	while( itor != pNPC->m_mapTQUESTTEMP.end() )
	{
		LPTQUEST pQuest = (*itor).second;

		if( pQuest->m_bType == QT_CRAFT )
		{
			LPTQCLASS pQClass = CTChart::FindTQCLASS(  pQuest->m_dwClassID );
			if( pQClass == NULL ) break;

			AddCategory( pQClass->m_dwClassID, pQClass->m_strNAME );
			AddItemToCategory( pQClass->m_dwClassID, pQuest );
		}

		++itor;
	}

	m_bNeedResetPos = TRUE;
}

BOOL CTCraftQuestDlg::ResetPos()
{
	BOOL bShowList = FALSE;
	CPoint ptCurrent = m_ptBasePointCategory;
	m_pQLIST->m_pScroll->ShowComponent( FALSE );

	MAPTCRAFTCATEGORY::iterator it = m_mapCategory.begin();
	MAPTCRAFTCATEGORY::iterator end = m_mapCategory.end();

    while( it != end )
	{
		CTCraftCategory* pCategory = (*it).second;

		pCategory->SetCategoryPos( ptCurrent );

		if( pCategory->m_bOpened )
		{
			size_t cate_size = m_vCategoryOrders.size();
			size_t size = pCategory->m_vTQuest.size();
	
			CPoint ptList;
			m_pQLIST->GetComponentPos( &ptList );
			m_pQLIST->MoveComponent( CPoint(ptList.x, ptCurrent.y + m_nCategoryHeight) );

			m_pQLIST->ShowComponent( TRUE );
			
			SIZE itemSize;
			m_pQLIST->GetItemSize( &itemSize );

			BOOL bScrollShow = FALSE;

			if( cate_size + size <= 15 )
			{
				m_pQLIST->SetHeight2( itemSize.cy * size );
				ptCurrent.y += itemSize.cy * size;
			}
			else
			{
				bScrollShow = TRUE;
				m_pQLIST->SetHeight2( itemSize.cy * (15 - cate_size) );
				ptCurrent.y += itemSize.cy * (15 - cate_size);
			}

			m_pQLIST->m_pScroll->ShowComponent( bScrollShow );

			bShowList = TRUE;
		}

		pCategory->m_pPlusBtn->ShowComponent( !pCategory->m_bOpened );
		pCategory->m_pMinusBtn->ShowComponent( pCategory->m_bOpened );

		ptCurrent.y += m_nCategoryHeight;

		++it;
	}

	m_bNeedResetPos = FALSE;

	return bShowList;
}

void CTCraftQuestDlg::AddCategory( DWORD dwCategoryID, CString strName )
{
	MAPTCRAFTCATEGORY::iterator it  = m_mapCategory.find( dwCategoryID );
	if( it ==  m_mapCategory.end() )
	{
		CTCraftCategory* pCategory = new CTCraftCategory;

		pCategory->m_dwID = dwCategoryID;

		pCategory->m_pCategoryBar = new TComponent( this, CTCraftCategory::m_pTempCategoryBar->m_pDESC );
		pCategory->m_pName = new TComponent( this, CTCraftCategory::m_pTempCategoryName->m_pDESC );
		pCategory->m_pPlusBtn = new TButton( this, CTCraftCategory::m_pTempCategoryPlusBtn->m_pDESC );
		pCategory->m_pMinusBtn = new TButton( this, CTCraftCategory::m_pTempCategoryMinusBtn->m_pDESC );

		pCategory->m_pCategoryBar->m_id = GetUniqueID();
		pCategory->m_pName->m_id = GetUniqueID();
		pCategory->m_pPlusBtn->m_id = GetUniqueID();
		pCategory->m_pMinusBtn->m_id = GetUniqueID();

		AddKid( pCategory->m_pCategoryBar );
		AddKid( pCategory->m_pName );
		AddKid( pCategory->m_pPlusBtn );
		AddKid( pCategory->m_pMinusBtn );

		pCategory->SetName( strName );
		m_mapCategory.insert( make_pair( dwCategoryID, pCategory ));
		m_vCategoryOrders.push_back( pCategory );
	}
	else
	{
		CTCraftCategory* pCategory = it->second;
		pCategory->SetName( strName );
	}
}

void CTCraftQuestDlg::AddItemToCategory( DWORD dwCategoryID, LPTQUEST pTQuest )
{
	MAPTCRAFTCATEGORY::iterator it = m_mapCategory.find( dwCategoryID );
	if( it != m_mapCategory.end() )
	{
		(*it).second->m_vTQuest.push_back( pTQuest );
	}
}

void CTCraftQuestDlg::AddItemToList( LPTQUEST pTQuest )
{
	int index = m_pQLIST->AddItem( pTQuest->m_strTITLE, (DWORD)pTQuest, COL_ITEM );
	m_pQLIST->SetItemDataAllColumn( index, (DWORD)pTQuest );
	m_pQLIST->SetUserColor( index, COL_ITEM, TCOLOR_QUESTCLASS );
}

void CTCraftQuestDlg::OpenCategory( DWORD dwCategoryID )
{
	m_pTQUEST = NULL;
	RemoveAllList();

	MAPTCRAFTCATEGORY::iterator it = m_mapCategory.begin();
	MAPTCRAFTCATEGORY::iterator end = m_mapCategory.end();

	while( it != end )
	{
		if( (*it).second->m_dwID == dwCategoryID )
		{
			(*it).second->m_bOpened = TRUE;

			vector< LPTQUEST >& vTQuest = (*it).second->m_vTQuest;
			for ( int i=0; i<vTQuest.size(); ++i )
				AddItemToList( vTQuest[i] );
		}
		else
		{
			(*it).second->m_bOpened = FALSE;
		}
		
		++it;
	}

	m_bNeedResetPos = TRUE;
}

LPTQUEST CTCraftQuestDlg::GetTQUEST(INT nIdx)
{
	DWORD dwDATA = m_pQLIST->GetItemData(nIdx, COL_ITEM);

	if( dwDATA != (DWORD)-1 )
		return (LPTQUEST) dwDATA;
	else
		return NULL;
}

CTClientQClass* CTCraftQuestDlg::GetQCLASS(INT nIdx)
{
	DWORD dwDATA = m_pQLIST->GetItemData(nIdx, COL_CATEGORY);
	if( dwDATA != (DWORD)-1 )
		return (CTClientQClass*)(dwDATA);
	else
		return NULL;
}

LPTTERM CTCraftQuestDlg::GetSelTerm()
{
	INT idx = m_pTermList->GetSel();

	if( m_pTQUEST && idx >= 0 )
		if( idx < m_pTQUEST->m_vTTERM.size() )
			return m_pTQUEST->m_vTTERM[idx];

	return NULL;
}

BOOL CTCraftQuestDlg::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !m_bVisible || !m_bEnable )	return FALSE;

	return CTClientUIBase::DoMouseWheel( nFlags, zDelta, pt);
}

BOOL CTCraftQuestDlg::CanWithItemUI()
{
	return FALSE;
}
