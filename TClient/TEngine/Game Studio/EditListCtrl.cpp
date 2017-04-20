// EditListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EditListCtrl.h"


// CEditListCtrl

IMPLEMENT_DYNAMIC(CEditListCtrl, CListCtrl)
CEditListCtrl::CEditListCtrl()
{
	m_dwMessageID = 0;
	m_pHost = NULL;
}

CEditListCtrl::~CEditListCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditListCtrl, CListCtrl)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()



// CEditListCtrl 메시지 처리기입니다.


void CEditListCtrl::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	BOOL bUpdate = nPos != GetScrollPos(SB_HORZ);
	CListCtrl::OnHScroll( nSBCode, nPos, pScrollBar);

	if( bUpdate && m_pHost )
		m_pHost->SendMessage( m_dwMessageID, (WPARAM) this);
}

void CEditListCtrl::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	BOOL bUpdate = nPos != GetScrollPos(SB_VERT);
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);

	if( bUpdate && m_pHost )
		m_pHost->SendMessage( m_dwMessageID, (WPARAM) this);
}
