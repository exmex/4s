#include "Stdafx.h"
#include "TCtrlList.h"

// CTCtrlListSlot

void CTCtrlListSlot::Update(CTCtrlListItem* pItem)
{
	m_pItem = pItem;

	if( m_pItem )
	{
		m_pItem->ReflectSlot(this);
		ShowComponent(TRUE);
	}
	else
		ShowComponent(FALSE);
}



void CTCtrlListSlot::ShowComponent(BOOL bShow)
{
	m_bShow = bShow;
	m_pBase->ShowComponent(bShow);
}


// CTCtrlList

CTCtrlList::CTCtrlList()
	: m_pScroll(NULL), m_nSlotPerScroll(0), m_bNeedUpdate(TRUE), m_nSelectItemIdx(T_INVALID), 
	  m_nPrvScrPos(0), m_nWheelMoveValue(2)
{
}

CTCtrlList::~CTCtrlList()
{
	ClearSlot();
	ClearItem();
}

void CTCtrlList::NotifyUpdate()
{
	m_bNeedUpdate = TRUE;
}

void CTCtrlList::SetupScroll(TScroll* pScroll, UINT nSlotPerScroll)
{
	m_pScroll = pScroll;
	m_nSlotPerScroll = nSlotPerScroll;

	m_pScroll->SetScrollPos(0,0);

	NotifyUpdate();
}

void CTCtrlList::SetSlot(CTCLSlotArray* pSlots)
{
	ClearSlot();

	if( pSlots )
		m_vSlots.swap(*pSlots);

	NotifyUpdate();
}

BOOL CTCtrlList::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if(!m_pScroll) 
		return FALSE;

	int nRange, nPos;
	m_pScroll->GetScrollPos( nRange, nPos);

	nPos += zDelta>0? -m_nWheelMoveValue: m_nWheelMoveValue;
	nPos = min(max(nPos, 0), nRange);

	m_pScroll->SetScrollPos( nRange, nPos);

	return TRUE;
}

void CTCtrlList::ShowComponent(BOOL bShow)
{
	for(size_t i=0; i<m_vSlots.size(); ++i)
		m_vSlots[i]->ShowComponent(bShow);
}

void CTCtrlList::Select(INT nIdx)
{
	m_nSelectItemIdx = nIdx;
	NotifyUpdate();
}

void CTCtrlList::Update()
{
	INT nScrPos = GetScrollPos();
	if( m_nPrvScrPos != nScrPos )
	{
		m_nPrvScrPos = nScrPos;
		m_bNeedUpdate = TRUE;
	}

	if( !m_bNeedUpdate )
		return;

	INT nItemCnt = GetItemCnt();
	INT nSlotCnt = GetSlotCnt();

	INT nRng = ((nItemCnt+1)/m_nSlotPerScroll) - ((nSlotCnt+1)/m_nSlotPerScroll);
	if( nRng < 0 )
		nRng = 0;

	if( nRng < nScrPos )
		nScrPos = nRng;

	SetScroll(nRng, nScrPos);

	INT nItemIdx = nScrPos * m_nSlotPerScroll;
	for(INT i = 0; i<nSlotCnt; ++i,++nItemIdx)
	{
		CTCtrlListItem* pItem;
		if( nItemIdx < nItemCnt )
			pItem = m_vItems[nItemIdx];
		else
			pItem = NULL;

		m_vSlots[i]->Update(pItem);

		if( pItem )
			m_vSlots[i]->Select( m_nSelectItemIdx == nItemIdx );
	}

	m_bNeedUpdate = FALSE;
}

void CTCtrlList::ClearSlot()
{
	for(size_t i=0; i<m_vSlots.size(); ++i)
		delete m_vSlots[i];

	m_vSlots.clear();
	
	NotifyUpdate();
}

INT CTCtrlList::GetSlotCnt() const
{
	return m_vSlots.size();
}

CTCtrlListSlot* CTCtrlList::GetSlot(INT nSlot) const
{
	const_cast<CTCtrlList*>(this)->Update();

	return m_vSlots[nSlot];
}

CTCtrlListSlot* CTCtrlList::GetSlotNotUpdate( INT nSlot ) const
{
	return m_vSlots[nSlot];
}

INT CTCtrlList::AddItem(CTCtrlListItem* pItem)
{
	pItem->m_nIndex = (UINT)m_vItems.size();
	m_vItems.push_back(pItem);
	NotifyUpdate();
	return (INT)pItem->m_nIndex;
}

void CTCtrlList::SetItem(INT nIdx, CTCtrlListItem* pItem)
{
	delete m_vItems[nIdx];
	pItem->m_nIndex = nIdx;
	m_vItems[nIdx] = pItem;
	NotifyUpdate();
}

void CTCtrlList::ClearItem()
{
	for(size_t i=0; i<m_vItems.size(); ++i)
		delete m_vItems[i];

	m_vItems.clear();
	m_nSelectItemIdx = T_INVALID;

	SetScroll(0,0);
	NotifyUpdate();
}

void CTCtrlList::RemoveItem(INT nIdx)
{
	CTCLItemArray::iterator itr = m_vItems.begin() + nIdx;
	delete *itr;
	m_vItems.erase(itr);

	INT nCnt = GetItemCnt();
	if( m_nSelectItemIdx >= nCnt )
		m_nSelectItemIdx = T_INVALID;

	for(INT i=0; i<nCnt; ++i)
		m_vItems[i]->m_nIndex = i;

	NotifyUpdate();
}

INT CTCtrlList::GetItemCnt() const
{
	return (INT) m_vItems.size();
}

BOOL CTCtrlList::IsItemEmpty() const
{
	return m_vItems.empty();
}

CTCtrlListItem* CTCtrlList::GetItem(INT nIdx) const
{
	const_cast<CTCtrlList*>(this)->Update();

	return m_vItems[nIdx];
}

CTCtrlListItem* CTCtrlList::GetSelectItem() const
{
	const_cast<CTCtrlList*>(this)->Update();

	return (m_nSelectItemIdx == T_INVALID)? NULL: m_vItems[m_nSelectItemIdx];
}

CTCtrlListItem* CTCtrlList::GetHitItem(const CPoint& pt)
{
	Update();

	INT nCnt = GetSlotCnt();
	for(INT i=0; i<nCnt; ++i)
	{
		if( m_vSlots[i]->m_pBase->HitTest(pt) )
			return m_vSlots[i]->m_pItem;
	}

	return NULL;
}

void CTCtrlList::SetAllSlotUserData( DWORD dwUserData )
{
	CTCLSlotArray::iterator it, end;
	it = m_vSlots.begin();
	end = m_vSlots.end();

	for(; it != end ; ++it )
		(*it)->m_dwUserData = dwUserData;
}

INT CTCtrlList::GetScrollPos()
{
	if( !m_pScroll )
		return 0;

	INT nRng,nPos;
	m_pScroll->GetScrollPos(nRng,nPos);

	return nPos;
}

INT CTCtrlList::GetScrollRng()
{
	if( !m_pScroll )
		return 0;

	INT nRng,nPos;
	m_pScroll->GetScrollPos(nRng,nPos);

	return nRng;
}

void CTCtrlList::SetScroll(INT nRng, INT nPos)
{
	if( m_pScroll )
	{
		m_pScroll->SetScrollPos(nRng,nPos);
		NotifyUpdate();
	}
}

