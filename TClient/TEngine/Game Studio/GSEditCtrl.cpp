// GSEditCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "GSEditCtrl.h"


// CGSEditCtrl

CGSEditCtrl::CGSEditCtrl()
{
	m_pDragEnter = NULL;
	m_pDragOver = NULL;
	m_pLeave = NULL;
	m_pDrop = NULL;
}

CGSEditCtrl::~CGSEditCtrl()
{
}

void CGSEditCtrl::PostNcDestroy()
{
}

CEdit *CGSEditCtrl::GetEditCtrl()
{
	return (CEdit *) this;
}

BEGIN_MESSAGE_MAP(CGSEditCtrl, CEditView)
	ON_CONTROL_REFLECT_EX(EN_CHANGE, OnEditChange)
	ON_WM_MOUSEACTIVATE()
END_MESSAGE_MAP()


// CGSEditCtrl 진단입니다.

#ifdef _DEBUG
void CGSEditCtrl::AssertValid() const
{
	CEditView::AssertValid();
}

void CGSEditCtrl::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif //_DEBUG


// CGSEditCtrl 메시지 처리기입니다.

DROPEFFECT CGSEditCtrl::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(m_pDragEnter)
		return m_pDragEnter( this, pDataObject, dwKeyState, point);
	else
		return CEditView::OnDragEnter( pDataObject, dwKeyState, point);
}

DROPEFFECT CGSEditCtrl::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	if(m_pDragOver)
		return m_pDragOver( this, pDataObject, dwKeyState, point);
	else
		return CEditView::OnDragOver( pDataObject, dwKeyState, point);
}

void CGSEditCtrl::OnDragLeave()
{
	if(m_pLeave)
		m_pLeave(this);
	else
		CEditView::OnDragLeave();
}

BOOL CGSEditCtrl::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	if(m_pDrop)
		return m_pDrop( this, pDataObject, dropEffect, point);
	else
		return CEditView::OnDrop( pDataObject, dropEffect, point);
}

int CGSEditCtrl::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CWnd::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CGSEditCtrl::OnEditChange()
{
	return FALSE;
}
