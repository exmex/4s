// TCombo.cpp: implementation of the TCombo class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TCombo::TCombo(TComponent *pParent, LP_FRAMEDESC pDesc)
:TComponent(pParent, pDesc)
{
	m_bType = TCML_TYPE_COMBO;
	m_style |= TS_FOCUSABLE;

	m_pDrop = FindKid( pDesc->m_vCOMP.m_vEX.combo.drop );
	m_pDownList = FindKid( pDesc->m_vCOMP.m_vEX.combo.downlist );

	PullUp();
}

TCombo::~TCombo()
{

}

void TCombo::PullUp()
{
	CSize size = CSize(0, 0);
	if( m_pDrop )
		m_pDrop->GetComponentSize(&size);

	if(m_pDownList)
		m_pDownList->ShowComponent(FALSE);

	SetComponentSize(size);
}

void TCombo::DropDown()
{
	CSize sizeDrop = CSize(0, 0);
	CSize sizeDownList = CSize(0, 0);

	if(m_pDrop)
		m_pDrop->GetComponentSize(&sizeDrop);

	if(m_pDownList)
	{
		m_pDownList->ShowComponent(TRUE);
		m_pDownList->GetComponentSize(&sizeDownList);		
	}

	CSize sizeTotal = sizeDrop + sizeDownList;

	SetComponentSize(sizeTotal);
}

void TCombo::OnLoseFocus(TComponent *pSetFocus)
{
	PullUp();
}

void TCombo::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if( pKid &&
			pKid->IsVisible())
			pKid->OnKeyDown(nChar, nRepCnt, nFlags);
	}
}

void TCombo::OnLButtonDown(UINT nFlags, CPoint pt)
{
	TCOMP_LIST::iterator it =
		GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if( pKid &&
			pKid->IsVisible())
			pKid->OnLButtonDown(nFlags, pt);
	}
}

void TCombo::OnLButtonUp(UINT nFlags, CPoint pt)
{
	TCOMP_LIST::iterator it =
		GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if( pKid &&
			pKid->IsVisible())
			pKid->OnLButtonUp(nFlags, pt);
	}
}

void TCombo::OnMouseMove(UINT nFlags, CPoint pt)
{
	TCOMP_LIST::iterator it =
		GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if( pKid &&
			pKid->IsVisible())
			pKid->OnMouseMove(nFlags, pt);
	}	
}

void TCombo::OnNotify(DWORD from, WORD msg, LPVOID param)
{
	switch(msg)
	{
	case TNM_LCLICK:
		if( m_pDrop &&
			m_pDownList &&
			m_pDrop->m_id == from )
		{
			m_pDownList->IsVisible() ? PullUp() : DropDown();
		}
		break;

	case TNM_SEL_CHANGE:
		if( m_pDownList &&
			m_pDownList->m_id == from )
		{
			MAP_LISTITEM* pSelItem = (MAP_LISTITEM*) param;

			if( pSelItem && m_pDrop )
			{
				MAP_LISTITEM::iterator finder = pSelItem->find( 0 );

				if( finder != pSelItem->end() )
				{
					m_pDrop->SetComponentText( ((*finder).second)->m_strText );
					SAFE_NOTIFY( m_id, TNM_SEL_CHANGE, NULL );
				}
			}

			PullUp();
		}
		break;
	}
}

int TCombo::AddString( LPCTSTR strText, int nColumn )
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return -1;

	return ((TList*) m_pDownList)->AddString( strText, nColumn );
}

int TCombo::AddItem(LPCTSTR strText, DWORD data, int nColumn )
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) ) return -1;

	return ((TList*) m_pDownList)->AddItem( strText, data, nColumn );
}

int TCombo::DelString( int nLine )
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return -1;

	return ((TList*)m_pDownList)->DelString(nLine);
}

int TCombo::DelItem( int nLine )
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return -1;

	return ((TList*) m_pDownList)->DelItem(nLine);
}

MAP_LISTITEM* TCombo::GetCurSelItem()
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return NULL;

	return ((TList*) m_pDownList)->GetCurSelItem();
}

MAP_LISTITEM* TCombo::SetCurSelItem( int nLine )
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return NULL;

	return ((TList*) m_pDownList)->SetCurSelItem(nLine);
}

int TCombo::GetSel()
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return -1;

	return ((TList*) m_pDownList)->GetSel();
}

DWORD TCombo::GetItemData( int nLine, int nColumn )
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return -1;

	return ((TList*) m_pDownList)->GetItemData( nLine, nColumn );
}

MAP_LISTITEM* TCombo::GetItem( int nLine )
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return NULL;

	return ((TList*) m_pDownList)->GetItem(nLine);
}

BOOL TCombo::SetItemString( int nLine, int nColumn, LPCTSTR strText)
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return FALSE;

	return ((TList*) m_pDownList)->SetItemString( nLine, nColumn, strText );
}

BOOL TCombo::SetItemData( int nLine, int nColumn, DWORD data )
{
	if( !m_pDownList ||
		!m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		return FALSE;

	return ((TList*) m_pDownList)->SetItemData( nLine, nColumn, data );
}

void TCombo::RemoveAll()
{
	if( m_pDownList &&
		m_pDownList->IsTypeOf( TCML_TYPE_LIST ) )
		((TList*) m_pDownList)->RemoveAll();

	if( m_pDrop )
		m_pDrop->SetComponentText(_T(""));
}
