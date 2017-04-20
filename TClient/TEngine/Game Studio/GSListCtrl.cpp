// GSListCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "GSListCtrl.h"


// CGSListCtrl

CGSListCtrl::CGSListCtrl()
{
	m_pDragEnter = NULL;
	m_pDragOver = NULL;
	m_pLeave = NULL;
	m_pDrop = NULL;

	m_bUISelect = TRUE;
}

CGSListCtrl::~CGSListCtrl()
{
}

CListCtrl *CGSListCtrl::GetListCtrl()
{
	return (CListCtrl *) this;
}

void CGSListCtrl::PostNcDestroy()
{
}

void CGSListCtrl::SetCurSel( int nIndex)
{
	CListCtrl *pList = GetListCtrl();

	POSITION pos = pList->GetFirstSelectedItemPosition();
	BOOL bUpdate = nIndex != -1;

	m_bUISelect = FALSE;
	while(pos)
	{
		int nSel = pList->GetNextSelectedItem(pos);

		if( nSel != nIndex )
		{
			pList->SetItem(
				nSel, 0,
				LVIF_STATE,
				NULL, 0, 0,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				NULL);

			pList->Update(nSel);
		}
		else
			bUpdate = FALSE;
	}

	if(bUpdate)
	{
		pList->SetItem(
			nIndex, 0,
			LVIF_STATE,
			NULL, 0,
			LVIS_SELECTED|
			LVIS_FOCUSED,
			LVIS_SELECTED|
			LVIS_FOCUSED,
			NULL);

		pList->Update(nIndex);
	}

	m_bUISelect = TRUE;
}

int CGSListCtrl::GetCurSel()
{
	CListCtrl *pList = GetListCtrl();
	POSITION pos = pList->GetFirstSelectedItemPosition();

	if(pos)
		return pList->GetNextSelectedItem(pos);

	return -1;
}

BEGIN_MESSAGE_MAP(CGSListCtrl, CListView)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CGSListCtrl 진단입니다.

#ifdef _DEBUG
void CGSListCtrl::AssertValid() const
{
	CListView::AssertValid();
}

void CGSListCtrl::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG


// CGSListCtrl 메시지 처리기입니다.

DROPEFFECT CGSListCtrl::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(m_pDragEnter)
		return m_pDragEnter( this, pDataObject, dwKeyState, point);
	else
		return CListView::OnDragEnter( pDataObject, dwKeyState, point);
}

DROPEFFECT CGSListCtrl::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(m_pDragOver)
		return m_pDragOver( this, pDataObject, dwKeyState, point);
	else
		return CListView::OnDragOver( pDataObject, dwKeyState, point);
}

void CGSListCtrl::OnDragLeave()
{
	if(m_pLeave)
		m_pLeave(this);
	else
		CListView::OnDragLeave();
}

BOOL CGSListCtrl::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	if(m_pDrop)
		return m_pDrop( this, pDataObject, dropEffect, point);
	else
		return CListView::OnDrop( pDataObject, dropEffect, point);
}

void CGSListCtrl::UpdateColumnWidth()
{
	CListCtrl *pList = GetListCtrl();
	CRect rect;

	pList->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pList->SetColumnWidth( 0, -1);
	int nWidth = pList->GetColumnWidth(0);

	if( nWidth < rect.Width() )
		pList->SetColumnWidth( 0, rect.Width());
}

int CGSListCtrl::OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
