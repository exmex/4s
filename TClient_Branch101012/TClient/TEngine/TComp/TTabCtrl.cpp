// TTabCtrl.cpp: implementation of the TTabCtrl class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TTabCtrl::TTabCtrl(TComponent *pParent, LP_FRAMEDESC pDesc)
:TComponent(pParent, pDesc)
{
	int nCount = 0;
	m_bType = TCML_TYPE_TABCTRL;


	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{//Should be state buttons for TabCtrl
		TComponent* pKid = GetNextKid(it);

		if( pKid &&
			pKid->IsTypeOf( TCML_TYPE_BUTTON ) )
		{
			nCount ++;
			((TButton *) pKid)->SetStateButton();
			m_Buttons.push_back((TButton *) pKid);

			pKid->SetTextAlign(m_flagAlign);
			pKid->EnableComponent(FALSE);
			pKid->ShowComponent(FALSE);
		}
	}

	for( int i=0; i<nCount; i++)
		m_Frames.push_back(NULL);
}

TTabCtrl::~TTabCtrl()
{
}

void TTabCtrl::OnLButtonDown( UINT nFlags, CPoint pt )
{
	if( !CanProcess() || !HitTest(pt) ) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid)
			pKid->OnLButtonDown(nFlags, pt);
	}	
}

void TTabCtrl::OnLButtonUp( UINT nFlags, CPoint pt )
{
	if( !CanProcess() || !HitTest(pt) ) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid)
			pKid->OnLButtonUp(nFlags, pt);
	}	
}

void TTabCtrl::OnMouseMove( UINT nFlags, CPoint pt )
{
	if( !CanProcess() || !HitTest(pt) ) return;
	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid)
			pKid->OnMouseMove(nFlags, pt);
	}	
}

void TTabCtrl::OnRButtonDown( UINT nFlags, CPoint pt )
{
	if( !CanProcess() || !HitTest(pt) ) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid)
			pKid->OnRButtonDown(nFlags, pt);
	}	
}

void TTabCtrl::OnRButtonUp( UINT nFlags, CPoint pt )
{
	if( !CanProcess() || !HitTest(pt) ) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if(pKid)
			pKid->OnRButtonUp(nFlags, pt);
	}
}

void TTabCtrl::OnNotify( DWORD from, WORD msg, LPVOID param )
{
	switch(msg)
	{
	case TNM_LCLICK		:
		if(FindKid(from))
			SelectItem(*(int *) param);

		break;
	}
}

BOOL TTabCtrl::InsertItem( int nItem, LPCTSTR lpszItem, LP_FRAMEDESC pDescFrame )
{
	if( !pDescFrame || pDescFrame->m_vCOMP.m_bType != TCML_TYPE_FRAME ||
		nItem < 0 || nItem >= m_Buttons.size() )
		return FALSE;
	DeleteItem(nItem);

	TFrame *pFrame = (TFrame *) AddKid(pDescFrame);
	CRect rect;

	GetComponentRect(&rect);
	pFrame->MoveComponent(CPoint( 0, rect.Height()));

	m_Buttons[nItem]->SetComponentText(lpszItem);
	m_Buttons[nItem]->EnableComponent(TRUE);
	m_Buttons[nItem]->ShowComponent(TRUE);
	m_Frames[nItem] = pFrame;
	SelectItem(nItem);

	return TRUE;
}

void TTabCtrl::DeleteItem( int nItem)
{
	if( nItem >= 0 && nItem < m_Frames.size() && nItem < m_Buttons.size() && m_Buttons[nItem] && m_Frames[nItem] )
	{
		m_Buttons[nItem]->SetComponentText(_T(""));
		m_Buttons[nItem]->EnableComponent(FALSE);
		m_Buttons[nItem]->ShowComponent(FALSE);
		RemoveKid(m_Frames[nItem]);

		delete m_Frames[nItem];
		m_Frames[nItem] = NULL;
	}
}

void TTabCtrl::SelectItem( int nItem)
{
	int nSize = m_Buttons.size();

	for( int i=0; i<nSize; i++)
	{
		m_Buttons[i]->Select(i == nItem);

		if(m_Frames[i])
			m_Frames[i]->ShowComponent(m_Buttons[i]->IsVisible() && i == nItem);
	}
}

void TTabCtrl::ShowComponent( BOOL bVisible)
{
	TCOMP_LIST::iterator it = GetFirstKidsFinder();

	m_bVisible = bVisible;
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);

		if( pKid && !pKid->m_blIsCaret &&
			pKid->IsTypeOf(TCML_TYPE_BUTTON) &&
			pKid->IsTypeOf(TCML_TYPE_FRAME) )
			pKid->ShowComponent(bVisible);
	}
}

void TTabCtrl::EnableComponent( BOOL bEnable)
{
	TCOMP_LIST::iterator it = GetFirstKidsFinder();

	m_bEnable = bEnable;
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);

		if( pKid && !pKid->m_blIsCaret &&
			pKid->IsTypeOf(TCML_TYPE_BUTTON) &&
			pKid->IsTypeOf(TCML_TYPE_FRAME) )
			pKid->EnableComponent(bEnable);
	}
}
