// TScroll.cpp: implementation of the TScroll class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TScroll::TScroll(TComponent *pParent, LP_FRAMEDESC pDesc)
:TComponent(pParent, pDesc)
{
	SetScrollType(TRUE);

	m_bType = TCML_TYPE_SCROLL;
	m_bDrag = FALSE;

	m_nRange = 400;
	m_nCurPos = 0;

	if(pDesc)
	{
		m_pBar = FindKid(pDesc->m_vCOMP.m_vEX.scroll.bar);
		m_pUpLeft = FindKid(pDesc->m_vCOMP.m_vEX.scroll.ul);
		m_pDownRight = FindKid(pDesc->m_vCOMP.m_vEX.scroll.dr);
	}
}

TScroll::~TScroll()
{

}

void TScroll::OnNotify(DWORD from, WORD msg, LPVOID param)
{
	switch(msg)
	{
	case TNM_LCLICK:
		if( m_pUpLeft && 
			m_pUpLeft->AreYou(from) )
		{
			m_nCurPos--;
			if(m_nCurPos < 0) m_nCurPos = 0;
		
			SetBarPosition();
			SAFE_NOTIFY(m_id, TNM_LINEUP, NULL);
		}
		else if( m_pDownRight &&
			m_pDownRight->AreYou(from) )
		{
			m_nCurPos++;
			if(m_nCurPos > m_nRange)
				m_nCurPos = m_nRange;

			SetBarPosition();
			SAFE_NOTIFY(m_id, TNM_LINEDOWN, NULL);
		}
		break;
	}
}

void TScroll::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if(!CanProcess()) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if( pKid )
			pKid->OnLButtonUp(nFlags, pt);
	}

	if(m_bDrag)
	{
		m_bDrag = FALSE;
		SAFE_NOTIFY(m_id, TNM_VSCROLL, NULL);
	}
}

void TScroll::OnMouseMove( UINT nFlags, CPoint pt)
{
	if(!CanProcess()) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if( pKid )
			pKid->OnMouseMove(nFlags, pt);
	}

	if(m_bDrag)
	{
		CPoint point = pt;
		CRect vRECT(m_rc);
		CRect rcBAR;
		CRect rcDR;
		CRect rcUL;

		vRECT.OffsetRect(
			-m_rc.left,
			-m_rc.top);

		if(m_pDownRight)
			m_pDownRight->GetComponentRect(&rcDR);
		else if(m_style&TSS_VSCROLL)
			rcDR.SetRect( vRECT.left, vRECT.bottom, vRECT.right, vRECT.bottom);
		else
			rcDR.SetRect( vRECT.right, vRECT.top, vRECT.right, vRECT.bottom);

		if(m_pUpLeft)
			m_pUpLeft->GetComponentRect(&rcUL);
		else if(m_style&TSS_VSCROLL)
			rcUL.SetRect( vRECT.left, vRECT.top, vRECT.right, vRECT.top);
		else
			rcUL.SetRect( vRECT.left, vRECT.top, vRECT.left, vRECT.bottom);

		m_pBar->GetComponentRect(&rcBAR);
		ScreenToComponent(&point);

		if(m_style&TSS_VSCROLL)
		{
			CRect rect(
				rcUL.left,
				rcUL.bottom + 1,
				rcUL.right,
				rcDR.top - rcBAR.Height() - 1);

			point.y = point.y - rcBAR.Height() / 2 - m_rc.top;
			point.x = rcBAR.left;

			if( point.y < rect.top )
				point.y = rect.top;
			else if( point.y > rect.bottom )
				point.y = rect.bottom;
		}
		else
		{
			CRect rect(
				rcUL.right + 1,
				rcUL.top,
				rcDR.left - rcBAR.Width() - 1,
				rcUL.bottom);

			point.x = point.x - rcBAR.Width() / 2 - m_rc.left;
			point.y = rcBAR.top;

			if( point.x < rect.left )
				point.x = rect.left;
			else if( point.x > rect.right )
				point.x = rect.right;
		}

		if(m_nRange)
			m_pBar->MoveComponent(point);
		int nNext = GetLogicalPos(point);

		if( m_nCurPos != nNext )
		{
			if( nNext > m_nRange )
				nNext = m_nRange;

			if( nNext < 0 )
				nNext = 0;

			m_nCurPos = nNext;
			SAFE_NOTIFY( m_id, TNM_VSCROLL, NULL);		
		}
	}
}

void TScroll::SetScrollType(BOOL bVertical)
{
	if(bVertical)
	{
		m_style &= (~TSS_HSCROLL);
		m_style |= TSS_VSCROLL;
	}
	else
	{
		m_style &= (~TSS_VSCROLL);
		m_style |= TSS_HSCROLL;
	}
}

void TScroll::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if(!CanProcess() || !HitTest(pt)) return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);
		if( pKid )
		{
			if(pKid == m_pBar && m_pBar->HitTest(pt))
				m_bDrag = TRUE;

			pKid->OnLButtonDown( nFlags, pt);
		}
	}
}

int TScroll::GetLogicalPos( CPoint pt)
{
	if( m_nRange <= 0 )
		return 0;

	CRect vRECT(m_rc);
	CRect rcBAR;
	CRect rcDR;
	CRect rcUL;

	int nBOUND;
	int nPOS;

	vRECT.OffsetRect(
		-m_rc.left,
		-m_rc.top);

	if(m_pDownRight)
		m_pDownRight->GetComponentRect(&rcDR);
	else if(m_style&TSS_VSCROLL)
		rcDR.SetRect( vRECT.left, vRECT.bottom, vRECT.right, vRECT.bottom);
	else
		rcDR.SetRect( vRECT.right, vRECT.top, vRECT.right, vRECT.bottom);

	if(m_pUpLeft)
		m_pUpLeft->GetComponentRect(&rcUL);
	else if(m_style&TSS_VSCROLL)
		rcUL.SetRect( vRECT.left, vRECT.top, vRECT.right, vRECT.top);
	else
		rcUL.SetRect( vRECT.left, vRECT.top, vRECT.left, vRECT.bottom);

	m_pBar->GetComponentRect(&rcBAR);
	if(m_style&TSS_VSCROLL)
	{
		nBOUND = rcDR.top - rcUL.bottom - rcBAR.Height() - 2;
		nPOS = pt.y - rcUL.bottom - 1;
	}
	else
	{
		nBOUND = rcDR.left - rcUL.right - rcBAR.Width() - 2;
		nPOS = pt.x - rcUL.right - 1;
	}

	if( nBOUND <= 0 )
		return 0;

	if( nPOS <= 0 )
		return 0;

	nPOS = m_nRange * nPOS / nBOUND;
	if( nPOS > m_nRange )
		nPOS = m_nRange;

	return nPOS;
}

void TScroll::GetPhysicalPos( int nLogicalPos, CPoint *ppt)
{
	if( m_nRange <= 0 )
		nLogicalPos = 0;

	CRect vRECT(m_rc);
	CRect rcBAR;
	CRect rcDR;
	CRect rcUL;

	vRECT.OffsetRect(
		-m_rc.left,
		-m_rc.top);

	if(m_pDownRight)
		m_pDownRight->GetComponentRect(&rcDR);
	else if(m_style&TSS_VSCROLL)
		rcDR.SetRect( vRECT.left, vRECT.bottom, vRECT.right, vRECT.bottom);
	else
		rcDR.SetRect( vRECT.right, vRECT.top, vRECT.right, vRECT.bottom);

	if(m_pUpLeft)
		m_pUpLeft->GetComponentRect(&rcUL);
	else if(m_style&TSS_VSCROLL)
		rcUL.SetRect( vRECT.left, vRECT.top, vRECT.right, vRECT.top);
	else
		rcUL.SetRect( vRECT.left, vRECT.top, vRECT.left, vRECT.bottom);

	m_pBar->GetComponentRect(&rcBAR);
	if(m_style&TSS_VSCROLL)
	{
		int nBOUND = rcDR.top - rcUL.bottom - rcBAR.Height() - 2;

		ppt->y = rcUL.bottom + 1 + (m_nRange ? nLogicalPos * nBOUND / m_nRange : 0);
		ppt->x = rcBAR.left;
	}
	else
	{
		int nBOUND = rcDR.left - rcUL.right - rcBAR.Width() - 2;

		ppt->x = rcUL.right + 1 + (m_nRange ? nLogicalPos * nBOUND / m_nRange : 0);
		ppt->y = rcBAR.top;
	}
}

void TScroll::SetScrollPos(int nRange, int nPos)
{
	m_nRange = nRange;
	if(nPos >= 0) m_nCurPos = nPos;

	SetBarPosition();
}

void TScroll::GetScrollPos(int &nRange, int& nPos)
{
	nRange = m_nRange;
	nPos = m_nCurPos;
}

void TScroll::SetBarPosition()
{
	if(m_pBar)
	{
		CPoint pt;

		GetPhysicalPos( m_nCurPos, &pt);
		m_pBar->MoveComponent(pt);
	}
}
