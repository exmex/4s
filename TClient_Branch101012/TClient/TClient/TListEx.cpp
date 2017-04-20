#include "Stdafx.h"
#include "TListEx.h"


// CTListEx::TPrivateList
// ====================================================================
CTListEx::TPrivateList::TPrivateList(TComponent *pParent, LP_FRAMEDESC pDesc)
	: TList(pParent, pDesc)
{
}
// ====================================================================
void CTListEx::TPrivateList::SetTop(int nIdx)
{
	m_nTop = nIdx;
}
// ====================================================================
BOOL CTListEx::TPrivateList::SetItem(int nLine, int nColumn, const CString& strText, DWORD dwData, DWORD dwColor )
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
	pItem->m_param = dwData;
	if( dwColor )
		pItem->m_dwTextColor = dwColor;

	return TRUE;
}
// ====================================================================

// CTListEx
// ====================================================================
CTListEx::CTListEx(TComponent *pParent, LP_FRAMEDESC pDesc)
	: TComponent(pParent,pDesc), m_nLastID(0), m_nPage(T_INVALID), m_bReady(FALSE),
	  m_pCommonList(NULL), m_pScroll(NULL), m_pPageBtn(NULL), m_pTitleImgs(NULL),
	  m_bMovingScroll(FALSE), m_nItemPerPage(0), m_bNeedUpdate(FALSE)
{
}
// --------------------------------------------------------------------
CTListEx::~CTListEx()
{
}
// ====================================================================

// ====================================================================
void CTListEx::SetCommonList(LP_FRAMEDESC pDesc)
{
	if( m_bReady )
		return;

	if( m_pCommonList )
	{
		RemoveKid(m_pCommonList);
		delete m_pCommonList;
	}

	m_pCommonList = new TPrivateList(this,pDesc);
	m_pCommonList->m_id = GetUniqueID();

	CPoint pt;
	m_pCommonList->GetComponentPos(&pt);
	pt.x -= m_rc.left;
	pt.y -= m_rc.top;
	m_pCommonList->MoveComponent(pt);

	AddKid(m_pCommonList);
}
// --------------------------------------------------------------------
void CTListEx::SetScroll(LP_FRAMEDESC pDesc)
{
	if( m_bReady )
		return;

	if( m_pScroll )
	{
		RemoveKid(m_pScroll);
		delete m_pScroll;
	}

	m_pScroll = new TScroll(this,pDesc);
	m_pScroll->m_id = GetUniqueID();

	CPoint pt;
	m_pScroll->GetComponentPos(&pt);
	pt.x -= m_rc.left;
	pt.y -= m_rc.top;
	m_pScroll->MoveComponent(pt);

	AddKid(m_pScroll);
}
// --------------------------------------------------------------------
void CTListEx::SetPageButton(LP_FRAMEDESC pDesc)
{
	if( m_bReady )
		return;

	if( m_pPageBtn )
	{
		RemoveKid(m_pPageBtn);
		delete m_pPageBtn;
	}

	m_pPageBtn = new TButton(this,pDesc);
	m_pPageBtn->m_id = GetUniqueID();

	CPoint pt;
	m_pPageBtn->GetComponentPos(&pt);
	pt.x -= m_rc.left;
	pt.y -= m_rc.top;
	m_pPageBtn->MoveComponent(pt);

	AddKid(m_pPageBtn);
}
// --------------------------------------------------------------------
/*void CTListEx::SetTitle(LP_FRAMEDESC pDesc)
{
	if( m_bReady )
		return;

	m_pTitleImgs = new TImageList(this,pDesc);
	m_pTitleImgs->m_id = GetUniqueID();

	CPoint pt;
	m_pTitleImgs->GetComponentPos(&pt);
	pt.x -= m_rc.left;
	pt.y -= m_rc.top;
	m_pTitleImgs->MoveComponent(pt);

	AddKid(m_pTitleImgs);
}*/
// --------------------------------------------------------------------
void CTListEx::AddPageTitle( INT nCount, TComponent** pTitles )
{
	if( m_bReady )
		return;

	TITLECOMP vPage;

	for( INT i=0 ; i < nCount ; ++i )
	{
		TComponent* pTCOMP = new TComponent( this, pTitles[ i ]->m_pDESC );
		pTCOMP->m_id = GetUniqueID();

		CPoint pt;
		pTCOMP->GetComponentPos(&pt);
		pt.x -= m_rc.left;
		pt.y -= m_rc.top;
		pTCOMP->MoveComponent(pt);

		AddKid(pTCOMP);

		vPage.m_vComp.push_back( pTCOMP );
	}

	m_vTitle.push_back( vPage );
}
// --------------------------------------------------------------------
int CTListEx::AddPageList(LP_FRAMEDESC pDesc)
{
	if( m_bReady )
		return 0;

	TPrivateList* pList = new TPrivateList(this,pDesc);
	pList->m_id = GetUniqueID();

	CPoint pt;
	pList->GetComponentPos(&pt);
	pt.x -= m_rc.left;
	pt.y -= m_rc.top;
	pList->MoveComponent(pt);

	AddKid(pList);

	m_vPageLists.push_back(pList);
	return (int)m_vPageLists.size();
}
// ====================================================================
void CTListEx::Build()
{
	if( m_bReady )
		return;

	m_bReady = TRUE;
	m_bNeedUpdate = TRUE;
	m_bType = TCML_TYPE_LISTEX;

	m_nItemPerPage = 0;
	if( m_pCommonList )
	{
		m_pCommonList->EnableComponent(TRUE);
		m_pCommonList->ShowComponent(TRUE);
		
		m_nItemPerPage = m_pCommonList->GetItemPerPage();
	}

	int nPageCnt = (int)m_vPageLists.size();
	if( nPageCnt > 0 )
		m_nPage = 0;
	else
		m_nPage = T_INVALID;

	for(int i=0; i<nPageCnt; ++i)
	{
		m_vPageLists[i]->EnableComponent(TRUE);
		m_vPageLists[i]->ShowComponent( i == m_nPage );
		
		int nIPP = m_vPageLists[i]->GetItemPerPage();
		if( nIPP < m_nItemPerPage )
			m_nItemPerPage = nIPP;
	}

	if( m_pPageBtn )
		m_pPageBtn->ShowComponent(TRUE);

	if( m_pScroll )
	{
		m_pScroll->ShowComponent(TRUE);
		m_pScroll->SetScrollPos(0,0);
	}

	if( m_pTitleImgs )
	{
		m_pTitleImgs->ShowComponent(TRUE);
		m_pTitleImgs->SetCurImage(0);
	}

	TLISTARRAY(m_vPageLists).swap(m_vPageLists);
}
// ====================================================================

// ====================================================================
int CTListEx::AddItem()
{
	if( !m_bReady )
		return T_INVALID;

	int nIndex = T_INVALID;
	if( m_pCommonList )
		nIndex = m_pCommonList->AddItem("",0);

	int nCnt = GetPageCount();
	for(int i=0; i<nCnt; ++i)
		nIndex = m_vPageLists[i]->AddItem("",0);

	return nIndex;
}
// --------------------------------------------------------------------
void CTListEx::DelItem(int nLine)
{
	if( !m_bReady )
		return;

	int nIndex = T_INVALID;
	if( m_pCommonList )
		m_pCommonList->DelItem(nLine);

	int nCnt = GetPageCount();
	for(int i=0; i<nCnt; ++i)
		m_vPageLists[i]->DelItem(nLine);
}
// --------------------------------------------------------------------
void CTListEx::ClearItem()
{
	if( !m_bReady )
		return;

	if( m_pCommonList )
		m_pCommonList->RemoveAll();

	int nCnt = GetPageCount();
	for(int i=0; i<nCnt; ++i)
		m_vPageLists[i]->RemoveAll();
}
// ====================================================================
void CTListEx::SetCommonItem(int nLine, int nCol, const CString& strItem, DWORD dwUser, DWORD dwColor)
{
	if( m_bReady && m_pCommonList )
	{
		m_pCommonList->SetItem(nLine, nCol, strItem, dwUser, dwColor);

		if( dwColor )
		{
			m_pCommonList->ApplyUserColor( dwColor );
			m_pCommonList->SetUserColor( nLine, nCol, dwColor );
		}
	}
}
// --------------------------------------------------------------------
void CTListEx::SetPageItem(int nLine, int nPage, int nCol, const CString& strItem, DWORD dwUser, DWORD dwColor)
{
	if( m_bReady )
	{
		m_vPageLists[nPage]->SetItem(nLine, nCol, strItem, dwUser);

		if( dwColor )
		{
			m_vPageLists[nPage]->ApplyUserColor( dwColor );
			m_vPageLists[nPage]->SetUserColor( nLine, nCol, dwColor );
		}
	}
}
// ====================================================================
void CTListEx::SetCommonItemGM(int nCol, DWORD dwEvent, DWORD dwGM)
{
	if( m_bReady && m_pCommonList )
		m_pCommonList->SetItemGM(nCol, dwEvent, dwGM);
}
// --------------------------------------------------------------------
void CTListEx::SetPageItemGM(int nPage, int nCol, DWORD dwEvent, DWORD dwGM)
{
	if( m_bReady )
		m_vPageLists[nPage]->SetItemGM(nCol,dwEvent, dwGM);
}
// ====================================================================
DWORD CTListEx::GetCommonItemData(int nLine, int nCol)
{
	if( !m_bReady || !m_pCommonList )
		return 0;

	return m_pCommonList->GetItemData(nLine, nCol);
}
// --------------------------------------------------------------------
DWORD CTListEx::GetPageItemData(int nLine, int nPage, int nCol)
{
	if( !m_bReady )
		return 0;

	return m_vPageLists[nPage]->GetItemData(nLine, nCol);
}
// ====================================================================
TListItem* CTListEx::GetCommonItem(int nLine, int nCol)
{
	if( !m_bReady || !m_pCommonList )
		return NULL;

	MAP_LISTITEM* pLine = m_pCommonList->GetItem(nLine);
	return (*pLine)[nCol];
}
// --------------------------------------------------------------------
TListItem* CTListEx::GetPageItem(int nLine, int nPage, int nCol)
{
	if( !m_bReady )
		return NULL;

	MAP_LISTITEM* pLine = m_vPageLists[nPage]->GetItem(nLine);
	return (*pLine)[nCol];
}
// ====================================================================
TListItem* CTListEx::GetHitItem(const CPoint& pt)
{
	if( !m_bReady )
		return NULL;

	TListItem* pItem;
	if( m_pCommonList )
	{
		pItem = m_pCommonList->GetHitItem(pt);
		if( pItem )
			return pItem;
	}

	if( m_nPage >= 0 )
	{
		pItem = m_vPageLists[m_nPage]->GetHitItem(pt);
		if( pItem )
			return pItem;
	}

	return NULL;
}
// --------------------------------------------------------------------
DWORD CTListEx::GetHitItemData(const CPoint& pt)
{
	if( !m_bReady )
		return 0;

	TListItem* pItem = GetHitItem(pt);
	if( pItem )
		return pItem->m_param;

	return 0;
}
// ====================================================================
int CTListEx::GetItemCount()
{
	if( !m_bReady )
		return 0;

	if( m_pCommonList )
		return m_pCommonList->GetItemCount();

	if( !m_vPageLists.empty() )
		return m_vPageLists.front()->GetItemCount();

	return 0;
}
// ====================================================================

// ====================================================================
UINT CTListEx::GetUniqueID()
{
	return m_nLastID++;
}
// --------------------------------------------------------------------
TComponent* CTListEx::GetHitComp(const CPoint& pt)
{
	if( m_pPageBtn && m_pPageBtn->HitTest(pt) )
		return m_pPageBtn;
	else if( m_pScroll && m_pScroll->HitTest(pt) )
		return m_pScroll;
	else if( m_pCommonList && m_pCommonList->HitTest(pt) )
		return m_pCommonList;
	else if( !m_vPageLists.empty() && m_nPage >= 0 && m_vPageLists[m_nPage]->HitTest(pt) )
		return m_vPageLists[m_nPage];

	return NULL;
}
// ====================================================================
void CTListEx::UpdateScroll()
{
	if( !m_pScroll )
		return;
	
	int nRng, nPos;
	m_pScroll->GetScrollPos(nRng,nPos);

	nRng = max(GetItemCount()-m_nItemPerPage, 0);
	nPos = min(nPos, nRng);

	if( m_pCommonList && m_pCommonList->CanProcess() )
		m_pCommonList->SetTop(nPos);

	int nCnt = GetPageCount();
	for(int i=0; i<nCnt; ++i)
		m_vPageLists[i]->SetTop(nPos);

	m_pScroll->SetScrollPos(nRng,nPos);
}
// --------------------------------------------------------------------
void CTListEx::Update(DWORD dwTick)
{
	UpdateScroll();

	if( !m_bNeedUpdate )
		return;
	
	int nPageCnt = GetPageCount();
	for(int i=0; i<nPageCnt; ++i)
		m_vPageLists[i]->ShowComponent( i == m_nPage );

	if( m_pTitleImgs )
	{
		m_pTitleImgs->SetCurImage(m_nPage);
	}

	m_bNeedUpdate = FALSE;
}
// ====================================================================


// ====================================================================
void CTListEx::ShowComponent( BOOL bVisible )
{
	TComponent::ShowComponent(bVisible);
	
	if( bVisible )
	{
		int nPageCnt = GetPageCount();
		for(int i=0; i<nPageCnt; ++i)
			m_vPageLists[i]->ShowComponent( i == m_nPage );
	}

	if( bVisible )
	{
		for( int i=0 ; i < m_vTitle.size() ; ++i )
			m_vTitle[ i ].ShowComponent( i == m_nPage );
	}
}
// ====================================================================
void CTListEx::OnMouseMove( UINT nFlags, CPoint pt )
{
	if( !m_bReady )
		return;

	TComponent* pHitComp = GetHitComp(pt);

	if( pHitComp )
		pHitComp->OnMouseMove(nFlags,pt);
}
// --------------------------------------------------------------------
void CTListEx::OnLButtonUp( UINT nFlags, CPoint pt )
{
	if( !m_bReady )
		return;

	TComponent* pHitComp = GetHitComp(pt);

	if( pHitComp )
		pHitComp->OnLButtonUp(nFlags,pt);
}
// --------------------------------------------------------------------
void CTListEx::OnRButtonUp( UINT nFlags, CPoint pt )
{
	if( !m_bReady )
		return;

	TComponent* pHitComp = GetHitComp(pt);

	if( pHitComp )
		pHitComp->OnLButtonUp(nFlags,pt);
}
// --------------------------------------------------------------------
void CTListEx::OnLButtonDown( UINT nFlags, CPoint pt )
{
	if( !m_bReady )
		return;

	TComponent* pHitComp = GetHitComp(pt);

	if( pHitComp )
	{
		if( pHitComp == m_pPageBtn )
		{
			if( m_nPage >= GetPageCount()-1 )
				m_nPage = 0;
			else
				++m_nPage;

			m_bNeedUpdate = TRUE;

			ShowComponent(m_bVisible);
		}

		pHitComp->OnLButtonDown(nFlags,pt);
	}
}
// --------------------------------------------------------------------
void CTListEx::OnRButtonDown( UINT nFlags, CPoint pt )
{
	if( !m_bReady )
		return;

	TComponent* pHitComp = GetHitComp(pt);

	if( pHitComp )
		pHitComp->OnLButtonDown(nFlags,pt);
}
// ====================================================================
BOOL CTListEx::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !m_bReady || !m_pScroll || !HitTest(pt) )
		return FALSE;

	int nRng, nPos;
	m_pScroll->GetScrollPos(nRng,nPos);

	nPos += zDelta>0? -1: 1;
	nPos = min(max(nPos, 0), nRng);
	m_pScroll->SetScrollPos(nRng,nPos);

	return TRUE;
}
// ====================================================================
HRESULT CTListEx::Render( DWORD dwTickCount )
{
	if( !m_bReady || !IsVisible() )
		return S_OK;

	Update(dwTickCount);

	return TComponent::Render(dwTickCount);
}
// ====================================================================


























