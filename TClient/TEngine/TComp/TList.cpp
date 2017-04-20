// TList.cpp: implementtion of the TList class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


TList::TList(TComponent *pParent, LP_FRAMEDESC pDesc)
:TComponent(pParent, pDesc), m_nWheelMoveValue(3)
{
	INT nBackupX = pDesc->m_vCOMP.m_nPosX;
	INT nBackupY = pDesc->m_vCOMP.m_nPosY;

	m_bType = TCML_TYPE_LIST;
	m_style |= TS_FOCUSABLE;

	m_nTop = 0;
	m_nCurSel = -1;
	m_nColumnCount = 0;
	m_blUserColor = FALSE;

	m_pScroll = FindKid( pDesc->m_vCOMP.m_vEX.list.vs );

	m_nItemPerPage = 0;

	for( int i=0 ; i<pDesc->m_vCOMP.m_vEX.list.column ; ++i )
		InitItemHolder( pDesc->m_vCOMP.m_vEX.list.item[i], m_nColumnCount++, pDesc );

	UpdateScrollPosition();

	pDesc->m_vCOMP.m_nPosX = nBackupX;
	pDesc->m_vCOMP.m_nPosY = nBackupY;
}

TList::~TList()
{
	RemoveAll();	
	RemoveItemHolder();
}

void TList::RemoveAll()
{
	VECTOR_MAP_LISTITEM::iterator itVector;
	MAP_LISTITEM::iterator itMap;

	for( itVector=m_items.begin() ; itVector!=m_items.end() ; ++itVector )
	{
		MAP_LISTITEM* pItemArray = *itVector;

		if( pItemArray )
		{
			for( itMap=pItemArray->begin() ; itMap!=pItemArray->end() ; ++itMap )
				delete (*itMap).second;

			pItemArray->clear();
			delete pItemArray;
		}
	}

	m_items.clear();

	m_nTop = 0;
	m_nCurSel = -1;

	UpdateScrollPosition();
}

void TList::InitItemHolder( unsigned int item, int nColumnCount, LP_FRAMEDESC pDesc )
{
	TComponent* pItem = FindKid( item );

	if( pItem && pItem->IsTypeOf( TCML_TYPE_BUTTON ) )
	{
		CPoint ptFirst;
		CSize sizeItem;
		CSize sizeList = m_rc.Size();
		
		pItem->GetComponentPos( &ptFirst );
		pItem->GetComponentSize( &sizeItem );
	
		pItem->EnableComponent( FALSE );

		((TButton*) pItem)->SetStateButton();
		pItem->SetStyle(pItem->GetStyle()|TBS_NOTIFY_DBLCLICK);
		pItem->SetTextAlign(m_flagAlign);
		pItem->m_bWordBreak = FALSE;

		TCOMP_ARRAY* pHolder1st = nColumnCount == 0 ? new TCOMP_ARRAY : m_itemholders[0];
		pHolder1st->push_back( pItem );
		if( nColumnCount == 0 ) m_itemholders.push_back( pHolder1st );

		LP_FRAMEDESC pListItemDesc = RetrieveKidDesc( pItem->GetID(), pDesc );
		if( pListItemDesc && sizeItem.cy > 0 )
		{
			if(!nColumnCount)
				m_nItemPerPage = sizeList.cy / sizeItem.cy;
			int nRowHeight = sizeList.cy / m_nItemPerPage;

			for( int i=1 ; i<m_nItemPerPage ; ++i )
			{
				pDesc->m_vCOMP.m_nPosY += nRowHeight - 1;
				TComponent* pListItem = AddKid( pListItemDesc );

				if( pListItem )
				{
					pListItem->m_index = pItem->m_index + i;
					pListItem->MoveComponent( CPoint( ptFirst.x, ptFirst.y + nRowHeight * i ) );
					pListItem->EnableComponent( FALSE );
					pListItem->SetFont( pItem->GetFont(), pItem->GetTipFont());
					pListItem->m_bWordBreak = FALSE;

					if( pListItem->IsTypeOf( TCML_TYPE_BUTTON ) )
					{
						((TButton*) pListItem)->SetStateButton();
						pListItem->SetStyle( pListItem->GetStyle()|TBS_NOTIFY_DBLCLICK );
						pListItem->SetTextAlign(m_flagAlign);
					}

					TCOMP_ARRAY* pHolder2nd = nColumnCount == 0 ? new TCOMP_ARRAY : m_itemholders[i];
					pHolder2nd->push_back( pListItem );
					if( nColumnCount == 0 ) m_itemholders.push_back( pHolder2nd );
				}
			}
		}
	}
}

void TList::RemoveItemHolder()
{
	for( INT i=1 ; i < m_itemholders.size() ; ++i )
	{
		TCOMP_ARRAY& vItemArray = *m_itemholders[ i ];

		for( INT j=0 ; j < vItemArray.size() ; ++j )
		{
			RemoveKid( vItemArray[ j ] );
			delete vItemArray[ j ];
		}
	}

	for( INT i=0 ; i < m_itemholders.size() ; ++i  )
		delete m_itemholders[ i ];

	m_itemholders.clear();
}

MAP_LISTITEM* TList::GetItem( int nLine )
{
	if( nLine < 0 || nLine >= m_items.size() ) 
		return NULL;

	return m_items[nLine];
}

TListItem* TList::GetHitItem( const CPoint& pt, INT* outColumn )
{
	int i = m_nTop;

	VECTOR_TCOMP_ARRAY::iterator itr,end;
	itr = m_itemholders.begin();
	end = m_itemholders.end();

	size_t nItemSize = m_items.size();
	for(; itr != end; ++itr, ++i)
	{
		TCOMP_ARRAY* pHolder = *itr;
		if( !pHolder )
			continue;
		
		if( i >= nItemSize || i-m_nTop >= m_nItemPerPage )
			continue;

		for( int j=INT(pHolder->size()) - 1; j>=0; j-- )
		{
			TComponent* pHolderCol = pHolder->at(j);
			if( pHolderCol && pHolderCol->HitTest(pt) )
			{
				MAP_LISTITEM* pItems = m_items[i];
				MAP_LISTITEM::iterator finder = pItems->find(j);

				if( outColumn)
					*outColumn = j;

				if( finder == pItems->end() )
					return NULL;

				return finder->second;
			}
		}
	}

	return NULL;
}

DWORD TList::GetHitItemData(const CPoint& pt)
{
	TListItem* pItem = GetHitItem(pt);
	if( pItem )
		return pItem->m_param;

	return 0;
}

int TList::AddString( LPCTSTR strText, int nColumn )
{
	if( nColumn < 0 || nColumn >= m_nColumnCount )
		return -1;

	MAP_LISTITEM* pItemArray = new MAP_LISTITEM;

	if( !pItemArray )
		return -1;

	for( int i=0 ; i<m_nColumnCount ; ++i )
	{
		TListItem* pItem = new TListItem();

		if( pItem )
		{
			pItem->m_strText = ( i==nColumn ? strText : _T("") );
			pItem->m_param = -1;

			pItemArray->insert( MAP_LISTITEM::value_type(
				i,
				pItem ) );
		}
	}

	m_items.push_back( pItemArray );

	UpdateScrollPosition();

	return  m_items.size() - 1;
}

int TList::AddInt( int nText, int nColumn )
{
	CString strText;
	strText.Format( "%d", nText );
	return AddString( strText, nColumn );
}

int TList::AddItem( LPCTSTR strText, DWORD data, int nColumn )
{
	int nSize = AddString( strText, nColumn );

	if( nSize >= 0 )
		SetItemData( nSize, nColumn, data );

	return nSize;
}

int TList::DelString( int nLine )
{
	if( nLine < 0 || nLine >= m_items.size() )
		return -1;

	MAP_LISTITEM* pItemArray = m_items[nLine];

	if( !pItemArray )
		return -1;

	MAP_LISTITEM::iterator it;

	for( it=pItemArray->begin() ; it!=pItemArray->end() ; ++it )
		delete (*it).second;

	pItemArray->clear();
	delete pItemArray;

	m_items.erase( m_items.begin() + nLine );

	if( m_nCurSel == nLine ) SelectUp(1);

	m_bNeedUpdateRender = TRUE;
	return m_items.size();
}

int TList::DelItem(int nLine)
{
	return DelString(nLine);
}

BOOL TList::SetItemString( int nLine, int nColumn, LPCTSTR strText )
{
	if( nLine < 0 || nLine >= m_items.size() ||
		nColumn < 0 || nColumn >= m_nColumnCount )
		return FALSE;

	MAP_LISTITEM* pItemArray = m_items[nLine];

	if( !pItemArray )
		return FALSE;

	MAP_LISTITEM::iterator finder = pItemArray->find( nColumn );

	if( finder == pItemArray->end() )
		return FALSE;

	TListItem* pItem = (*finder).second;

	if( !pItem )
		return FALSE;

	pItem->m_strText = strText;

	m_bNeedUpdateRender = TRUE;
	return TRUE;
}

CString TList::GetItemString( int nLine, int nColumn )
{
	if( nLine < 0 || nLine >= m_items.size() ||
		nColumn < 0 || nColumn >= m_nColumnCount )
		return CString();

	MAP_LISTITEM* pItemArray = m_items[nLine];

	if( !pItemArray )
		return CString();

	MAP_LISTITEM::iterator finder = pItemArray->find( nColumn );

	if( finder == pItemArray->end() )
		return CString();

	TListItem* pItem = (*finder).second;

	if( !pItem )
		return CString();

	return pItem->m_strText;
}

BOOL TList::SetItemInt( int nLine, int nColumn, int nText )
{
	CString strText;
	strText.Format( "%d", nText );
	return SetItemString( nLine, nColumn, strText );
}

BOOL TList::SetItemData( int nLine, int nColumn, DWORD data )
{
	if( nLine < 0 || nLine >= m_items.size() ||
		nColumn < 0 || nColumn >= m_nColumnCount )
		return FALSE;

	MAP_LISTITEM* pItemArray = m_items[nLine];

	if( !pItemArray )
		return FALSE;

	MAP_LISTITEM::iterator finder = pItemArray->find( nColumn );

	if( finder == pItemArray->end() )
		return FALSE;

	TListItem* pItem = (*finder).second;

	if( !pItem )
		return FALSE;

	pItem->m_param = data;

	return TRUE;
}

BOOL TList::SetImageIndex( int nLine, int nColumn, BOOL blIsImageList, int nImageIndex )
{
	if( nLine < 0 || nLine >= m_items.size() ||
		nColumn < 0 || nColumn >= m_nColumnCount )
		return FALSE;

	MAP_LISTITEM* pItemArray = m_items[nLine];

	if( !pItemArray )
		return FALSE;

	MAP_LISTITEM::iterator finder = pItemArray->find( nColumn );

	if( finder == pItemArray->end() )
		return FALSE;

	TListItem* pItem = (*finder).second;

	if( !pItem )
		return FALSE;

	pItem->m_blIsImageList = blIsImageList;
	pItem->m_nImageIndex = nImageIndex;

	m_bNeedUpdateRender = TRUE;
	return TRUE;
}

BOOL TList::SetItemDataAllColumn( int nLine, DWORD data )
{
	if( nLine < 0 || nLine >= m_items.size() )
		return FALSE;

	MAP_LISTITEM* pItemArray = m_items[nLine];

	if( !pItemArray )
		return FALSE;

	MAP_LISTITEM::iterator it, end;
	it = pItemArray->begin();
	end = pItemArray->end();

	for(; it != end ; ++it )
	{
		TListItem* pItem = (*it).second;

		if( !pItem )
			continue;

		pItem->m_param = data;
	}

	return TRUE;
}

MAP_LISTITEM* TList::GetCurSelItem()
{
	if( m_nCurSel < 0 || m_nCurSel >= (INT)m_items.size() )
		return NULL;

	return m_items[m_nCurSel];
}

MAP_LISTITEM* TList::SetCurSelItem( int nLine )
{
	m_nCurSel = nLine;

	if( m_nCurSel < 0 )
		return NULL;

	if( m_nCurSel >= (INT)m_items.size() )
	{
		m_nTop = 0;
		m_nCurSel = 0;
	}

	if( m_nCurSel < m_nTop )
	{
		m_nTop = m_nCurSel;
	}
	else if( m_nCurSel + 1 >= m_nTop + m_nItemPerPage )
	{
		m_nTop += m_nCurSel - ( m_nTop + m_nItemPerPage ) + 1;

		if( m_nTop > m_items.size() - m_nItemPerPage ) 
			m_nTop = m_items.size() - m_nItemPerPage;
	}
	UpdateScrollPosition();

	MAP_LISTITEM* pSelItem = GetCurSelItem();
	SAFE_NOTIFY( m_id, TNM_SEL_CHANGE, (LPVOID) pSelItem );
	return pSelItem;
}

MAP_LISTITEM* TList::SetTopSelItem( int nLine )
{
	m_nTop = nLine;
	return SetCurSelItem( nLine );
}

void TList::SetTop( int nLine )
{
	m_nTop = nLine;
	UpdateScrollPosition();
}

void TList::ClipAndInsertString( CString& strTEXT )
{
	RemoveAll();

	CSize szITEM = this->GetItemSize(0);
	VECTORSTRING vStrings;

	INT iPos = 0;
	CString str = strTEXT.Tokenize( _T("\n"), iPos );
	SeparateTextFitSize( str, szITEM, vStrings);

	while( iPos != -1 )
	{
		CString str = strTEXT.Tokenize( "\n", iPos );
		SeparateTextFitSize( str, szITEM, vStrings, TRUE);
	}

	for( INT i=0 ; i < vStrings.size() ; ++i )
	{
		INT nIndex = this->AddString(_T(""));
		this->SetItemString( nIndex, 0, vStrings[i] );
	}

	UpdateScrollPosition();
}

void TList::AddTextSetting(INT iRow, INT iColunm, INT iStart, INT iEnd, DWORD dwColor, DWORD dwInfo)
{
	 MAP_LISTITEM* pItemArray = m_items[iRow];

	 MAP_LISTITEM::iterator itr;
	 itr = pItemArray->find(iColunm);
	 if( itr == pItemArray->end() )
		 return;

	 TextSetting setting;
	 setting.iStart = iStart;
	 setting.iEnd = iEnd;
	 setting.dwColor = dwColor;
	 setting.dwUser = dwInfo;
	 
	 itr->second->m_TextSettingSet.insert(setting);

	 m_bNeedUpdateRender = TRUE;
}

void TList::SetItemGM(int nCol, DWORD dwEvent, DWORD dwGM)
{
	VECTOR_TCOMP_ARRAY::iterator itr,end;
	itr = m_itemholders.begin();
	end = m_itemholders.end();

	for(;itr!=end; ++itr)
	{
		TCOMP_ARRAY* pCOMPS = *itr;
		(*pCOMPS)[nCol]->m_menu[dwEvent] = dwGM;
	}
}

int TList::GetColWidth(int nCol)
{
	VECTOR_TCOMP_ARRAY::iterator itr,end;
	itr = m_itemholders.begin();
	end = m_itemholders.end();

	if( !m_itemholders.empty() )
	{
		CRect rt = m_itemholders[0]->at(nCol)->m_rc;
		return rt.right - rt.left;
	}

	return 0;
}

CSize TList::GetItemSize( int nColumn)
{
	TButton* pButton = (TButton*)(*(*(m_itemholders.begin())))[nColumn];
	CSize size;
	pButton->GetComponentSize(&size);

	return size;
}

void TList::GetItemSize( LPSIZE lpSize )
{
	lpSize->cx = 0;
	lpSize->cy = 0;

	for( int n=0 ; n<m_nColumnCount ; ++n )
	{
		TButton* pButton = (TButton*)(*(*(m_itemholders.begin())))[n];

		if( pButton )
		{
			CSize size;
			pButton->GetComponentSize( &size );

			lpSize->cx += size.cx;
			if( n == 0 ) lpSize->cy += size.cy;
		}
	}
}

void TList::OnNotify(DWORD from, WORD msg, LPVOID param)
{
	switch(msg)
	{
	case TNM_LINEUP:
		{
			m_nTop--;

			if(m_nTop < 0)
				m_nTop = 0;

			UpdateScrollPosition();
			//SAFE_NOTIFY( m_id, TNM_LINEUP, NULL );
		}
		break;

	case TNM_LINEDOWN:
		if(m_nItemPerPage < m_items.size()-m_nTop)
		{
			m_nTop ++;

			if(m_nTop > m_items.size()-m_nItemPerPage) 
				m_nTop = m_items.size()- m_nItemPerPage;

			UpdateScrollPosition();
			//SAFE_NOTIFY( m_id, TNM_LINEDOWN, NULL );
		}
		break;

	case TNM_LCLICK:
	case TNM_RCLICK:
	case TNM_DBLCLICK:
		{
			BOOL blStop = FALSE;
			for( int i=0 ; i<m_nColumnCount && !blStop ; ++i )
			{
				TComponent* pItem = (*(m_itemholders[0]))[i];

				if( pItem && pItem->m_id == from && param )
				{
					int nBaseIndex = pItem->m_index;
					int nItemIndex = *(int*)param;
					
					int nLine = nItemIndex - nBaseIndex + m_nTop;

					if( nLine < 0 )
						nLine = 0;

					MAP_LISTITEM* pSelItem = SetCurSelItem( nLine );
					blStop = TRUE;

					if(pSelItem)
						SAFE_NOTIFY( m_id, msg, (LPVOID) pSelItem );
				}
			}
		}

		break;

	case TNM_VSCROLL:
		if( m_pScroll && m_pScroll->IsTypeOf( TCML_TYPE_SCROLL ) )
		{
			int nRange;
			int nPos;

			((TScroll *) m_pScroll)->GetScrollPos( nRange, nPos);
			m_nTop = nRange ? nPos : 0;

			UpdateScrollPosition();
		}

		break;
	}
}

void TList::OnLButtonDblClk( UINT nFlags, CPoint pt )
{
	if(!CanProcess() || !HitTest(pt)) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid) pKid->OnLButtonDblClk(nFlags, pt);
	}
}

void TList::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if(!CanProcess() || !HitTest(pt)) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid)
			pKid->OnLButtonDown(nFlags, pt);
	}
}

void TList::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if(!CanProcess())
		return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid)
			pKid->OnLButtonUp(nFlags, pt);
	}
}

void TList::OnMouseMove(UINT nFlags, CPoint pt)
{
	if(!CanProcess()) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if( pKid )
			pKid->OnMouseMove(nFlags, pt);
	}
}

BOOL TList::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if(!CanProcess() || !m_pScroll) 
		return FALSE;

	CRect rc = m_rc;
	ComponentToScreen(&rc);
	if(!rc.PtInRect(pt))
		return FALSE;

	int nRange, nPos;
	((TScroll *) m_pScroll)->GetScrollPos( nRange, nPos);

	m_nTop += zDelta>0? -m_nWheelMoveValue: m_nWheelMoveValue;
	m_nTop = min(max(m_nTop, 0), nRange);

	UpdateScrollPosition();

	return TRUE;
}

void TList::OnRButtonDown(UINT nFlags, CPoint pt)
{
	if(!CanProcess() || !HitTest(pt)) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid)
			pKid->OnRButtonDown(nFlags, pt);
	}
}

void TList::OnRButtonUp(UINT nFlags, CPoint pt)
{
	if(!CanProcess() || !HitTest(pt)) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid)
			pKid->OnRButtonUp(nFlags, pt);
	}
}


void TList::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess()) return;

	switch(nChar)
	{
	case VK_LEFT:
	case VK_UP:			SelectUp(1);				break;
	case VK_RIGHT:
	case VK_DOWN:		SelectDown(1);				break;
	case VK_PRIOR:		SelectUp(m_nItemPerPage);	break;
	case VK_NEXT:		SelectDown(m_nItemPerPage); break;
	}
}

int TList::GetItemCount()
{
	return m_items.size();
}

void TList::SelectUp(int nLine)
{
	int nTarget = m_nCurSel - nLine;
	if(nTarget <0)
		nTarget = (nTarget<0 && m_items.size()>0)? 0: -1;
	
	if(nTarget >=0 && nTarget < m_nTop)
		m_nTop = nTarget;

	MAP_LISTITEM* pTargetItem = SetCurSelItem(nTarget);
	UpdateScrollPosition();
}

void TList::SelectDown(int nLine)
{
	int nTarget = (m_nCurSel < 0)? 0: m_nCurSel + nLine;

	if(nTarget >= m_items.size())
		nTarget = m_items.size() -1;
	
	if(nTarget>=0)
	{
		if(nTarget < m_nTop)
			m_nTop = m_nCurSel;
		else if(abs(nTarget - m_nTop) >= m_nItemPerPage)
			m_nTop = m_nTop + nLine;
	}

	MAP_LISTITEM* pTargetItem = SetCurSelItem(nTarget);
	if( pTargetItem )
		UpdateScrollPosition();
}

void TList::UpdateScrollPosition()
{
	if( m_pScroll &&
	    m_pScroll->IsTypeOf(TCML_TYPE_SCROLL))
	{
		int nCurSel = (m_nCurSel < 0)? 0: m_nCurSel;

		int nRange = m_items.size()-m_nItemPerPage;
		if(nRange < 0)
			nRange = 0;

		((TScroll *)m_pScroll)->SetScrollPos( nRange, m_nTop);
	}

	m_bNeedUpdateRender = TRUE;
}

DWORD TList::GetItemData( int nLine, int nColumn )
{
	if( nLine < 0 || nLine >= GetItemCount() ||
		nColumn <0 || nColumn >= m_nColumnCount )
		return -1;

	return (*(m_items[nLine]))[nColumn]->m_param;
}

int TList::GetSel()
{
	return m_nCurSel;
}

int TList::GetTop()
{
	return m_nTop;
}

int TList::GetItemPerPage()
{
	return m_nItemPerPage;
}

void TList::ApplyUserColor( BOOL blUserColor )
{
	m_blUserColor = blUserColor;
	UpdateScrollPosition();
}

void TList::SetUserColor( int nLine, int nColumn, DWORD dwTextColor )
{
	if( nLine < 0 || nLine >= m_items.size() ||
		nColumn < 0 || nColumn >= m_nColumnCount )
		return;

	MAP_LISTITEM* pItemArray = m_items[nLine];

	if( !pItemArray )
		return;

	MAP_LISTITEM::iterator finder = pItemArray->find( nColumn );

	if( finder == pItemArray->end() )
		return;

	TListItem* pItem = (*finder).second;

	if( !pItem )
		return;

	pItem->m_dwTextColor = dwTextColor;

	UpdateScrollPosition();
}

void TList::SetUseShadowFont(BOOL bUse)
{
	if( m_pFont )
	{
		m_pFont->m_bShadow = bUse;
		m_pFont->m_dwLineColor = 0xFF000000;

		VECTOR_TCOMP_ARRAY::iterator itr1,end1;
		TCOMP_ARRAY::iterator itr2,end2;

		itr1 = m_itemholders.begin();
		end1 = m_itemholders.end();
		
		for(; itr1!=end1; ++itr1)
		{
			itr2 = (*itr1)->begin();
			end2 = (*itr1)->end();

			for(; itr2!=end2; ++itr2)
			{
				CD3DFont* pFont = (*itr2)->Get3DFont();
				
				if( pFont )
				{
					pFont->m_bShadow = bUse;
					pFont->m_dwLineColor = 0xFF000000;
				}
			}
		}
	}

	UpdateScrollPosition();
}

int TList::GetTextExtent()
{
	return (*(*(m_itemholders.begin())))[0]->GetTextExtent();
}

void TList::SetTextClr( DWORD clrText)
{
	TComponent::SetTextClr(clrText);
	VECTOR_TCOMP_ARRAY::iterator it;

	for( it = m_itemholders.begin(); it != m_itemholders.end(); it++)
	{
		TCOMP_ARRAY *pHolder = (*it);

		if(pHolder)
			for( int i=0; i<m_nColumnCount; i++)
			{
				TButton *pButton = (TButton *) (*pHolder)[i];

				if(pButton)
					pButton->SetTextClr(clrText);
			}
	}

	UpdateScrollPosition();
}

void TList::SetTextClr( DWORD dwColor,
					    DWORD dwLineColor,
						BYTE bOutLine)
{
	TComponent::SetTextClr(
		dwColor,
		dwLineColor,
		bOutLine);
	VECTOR_TCOMP_ARRAY::iterator it;

	for( it = m_itemholders.begin(); it != m_itemholders.end(); it++)
	{
		TCOMP_ARRAY *pHolder = (*it);

		if(pHolder)
			for( int i=0; i<m_nColumnCount; i++)
			{
				TButton *pButton = (TButton *) (*pHolder)[i];

				if(pButton)
					pButton->SetTextClr( dwColor, dwLineColor, bOutLine);
			}
	}

	UpdateScrollPosition();
}

BYTE TList::HitItemArea( CPoint point)
{
	VECTOR_TCOMP_ARRAY::iterator it;

	for( it = m_itemholders.begin(); it != m_itemholders.end(); it++)
	{
		TCOMP_ARRAY *pHolder = (*it);

		if(pHolder)
			for( int i=m_nColumnCount - 1; i>=0; i--)
			{
				TButton *pButton = (TButton *) (*pHolder)[i];

				if(pButton->HitTest(point))
					return TRUE;
			}
	}

	return FALSE;
}

TComponent* TList::FindShrinkInChilds( POINT pt )
{
	VECTOR_TCOMP_ARRAY::iterator it;

	for( it=m_itemholders.begin() ; it!=m_itemholders.end() ; ++it )
	{
		TCOMP_ARRAY* pHolder = *it;

		if( pHolder )
		{
			for( int n=0 ; n<m_nColumnCount ; ++n )
			{
				TButton* pButton = (TButton*)(*pHolder)[n];

				if( pButton &&
					pButton->IsVisible() &&
					pButton->HitRect( pt ) &&
					pButton->m_bDidShrink )
				{
					return pButton;
				}
			}
		}
	}

	return NULL;
}

void TList::SetColumnAlign( int nCol, BYTE bAlign)
{
	if( nCol >= m_nColumnCount )
		return ;

	VECTOR_TCOMP_ARRAY::iterator it;

	for( it=m_itemholders.begin() ; it!=m_itemholders.end() ; ++it )
	{
		TCOMP_ARRAY* pHolder = *it;

		if( pHolder )
		{
			TButton* pButton = (TButton*)(*pHolder)[ nCol];
			pButton->SetTextAlign( bAlign );
		}
	}

	UpdateScrollPosition();
}

INT TList::GetMaxItemHeight()
{
	INT nMaxNewLineHeight = 0;
	for( INT i=0 ; i < m_pDESC->m_vCOMP.m_vEX.list.column ; ++i )
	{
		TComponent* pItem = FindKid( m_pDESC->m_vCOMP.m_vEX.list.item[i] );
		if( pItem && pItem->IsTypeOf( TCML_TYPE_BUTTON ) )
		{
			SIZE sizeItem;
			pItem->GetComponentSize( &sizeItem );
			nMaxNewLineHeight = sizeItem.cy > nMaxNewLineHeight ? sizeItem.cy : nMaxNewLineHeight;
		}
	}

	return nMaxNewLineHeight;
}

void TList::SetHeight2( INT nHeight )
{
	INT nItemHeight = GetMaxItemHeight();
	nHeight = nItemHeight * (nHeight / nItemHeight);

	m_rc.bottom = m_rc.top + nHeight;

	m_nTop = 0;
	m_nCurSel = -1;
	m_nColumnCount = 0;
	
	RemoveItemHolder();

	for( INT i=0 ; i< m_pDESC->m_vCOMP.m_vEX.list.column ; ++i )
		InitItemHolder(
			m_pDESC->m_vCOMP.m_vEX.list.item[i],
			m_nColumnCount++,
			m_pDESC );

	TScroll* pSCROLL = static_cast<TScroll*>( m_pScroll );
	if( pSCROLL->m_pDownRight )
	{
		CRect rtBtn;
		pSCROLL->m_pDownRight->GetComponentRect( &rtBtn );

		INT nHeight = rtBtn.Height();
		rtBtn.bottom = m_rc.Height();
		rtBtn.top = rtBtn.bottom - nHeight;

		pSCROLL->m_pDownRight->SetComponentRect( &rtBtn );
	}

	UpdateScrollPosition();
}

void TList::UpdateList()
{
	int i = m_nTop;

	VECTOR_TCOMP_ARRAY::iterator it;

	for( it=m_itemholders.begin() ; it!=m_itemholders.end() ; ++it, ++i )
	{
		TCOMP_ARRAY* pHolder = *it;

		if( pHolder )
		{
			if( i >= m_items.size() || i - m_nTop >= m_nItemPerPage )
			{
				for( int n=0 ; n<m_nColumnCount ; ++n )
				{
					TButton* pButton = (TButton*)(*pHolder)[n];

					if( pButton )
					{
						pButton->SetComponentText(_T(""));
						pButton->EnableComponent(FALSE);
						pButton->ShowComponent(FALSE);
					}
				}
			}
			else
			{
				MAP_LISTITEM* pItemArray = m_items[i];

				if( pItemArray )
				{
					for( int n=0 ; n<m_nColumnCount ; ++n )
					{
						TButton* pButton = (TButton*)(*pHolder)[n];

						MAP_LISTITEM::iterator finder = pItemArray->find( n );

						if( pButton && finder != pItemArray->end() )
						{
							TListItem* pItem = (*finder).second;

							if( pItem )
							{
								if( m_blUserColor )
									pButton->SetTextClr( pItem->m_dwTextColor );

								pButton->SetComponentText( pItem->m_strText.IsEmpty() ? _T("") : pItem->m_strText );
								pButton->EnableComponent(TRUE);
								pButton->ShowComponent(TRUE);
								pButton->m_nHMargine = pItem->m_nHMargine;

								if( pItem->m_blIsImageList )
									pButton->SetImageIndex( pItem->m_nImageIndex );

								pButton->Select( i==m_nCurSel );

								TextSettingSet::iterator itr,end;
								itr = pItem->m_TextSettingSet.begin();
								end = pItem->m_TextSettingSet.end();

								pButton->ResetTextSetting();
								for( ; itr != end; ++itr )
								{
									pButton->AddTextSetting(itr->iStart,
										itr->iEnd, itr->dwColor, itr->dwUser );
								}
							}
						}
					}
				}
			}
		}
	}	
}

void TList::ShowComponent( BOOL bVisible )
{
	m_bVisible = bVisible;
}

HRESULT TList::Render( DWORD dwTickCount )
{
	if( m_bNeedUpdateRender )
	{
		UpdateList();
		m_bNeedUpdateRender = FALSE;
	}

	return TComponent::Render( dwTickCount );
}

void TList::LastItemToFront()
{
	VECTOR_TCOMP_ARRAY::iterator itLINE, itEND_LINE;
	itLINE = m_itemholders.begin();
	itEND_LINE = m_itemholders.end();

	for(; itLINE != itEND_LINE ; ++itLINE )
	{
		TCOMP_ARRAY& vCOL = *(*itLINE);
		TComponent* pKid = vCOL[ vCOL.size()-1 ];
		//pKid->m_bNoHIT = TRUE;

		for( TCOMP_LIST::iterator it = m_kids.begin(); it != m_kids.end(); it++)
		{
			if( (*it) == pKid )
			{
				m_kids.erase(it);
				break;
			}
		}

		m_kids.insert( m_kids.begin(), pKid );
	}
}

void TList::ShowScroll( bool bVisible )
{
	if( bVisible )
	{
		TCOMP_LIST::iterator it, end;
		it = m_kids.begin();
		end = m_kids.end();
		for(; it != end ; ++it )
			if( (*it) == m_pScroll )
				return ;

		AddKid( m_pScroll );
		m_pScroll->ShowComponent(TRUE);
	}
	else
	{
		RemoveKid( m_pScroll );
	}
}