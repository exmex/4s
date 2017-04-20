// GSToolTipWnd.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "GSToolTipWnd.h"


// CGSToolTipWnd

IMPLEMENT_DYNAMIC( CGSToolTipWnd, CWnd)
CGSToolTipWnd::CGSToolTipWnd()
{
	m_rect.SetRectEmpty();
	m_strText.Empty();
}

CGSToolTipWnd::~CGSToolTipWnd()
{
}


BEGIN_MESSAGE_MAP( CGSToolTipWnd, CWnd)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CGSToolTipWnd 메시지 처리기입니다.

BOOL CGSToolTipWnd::Create( CWnd *pParent)
{
	return CWnd::CreateEx(
		WS_EX_TOOLWINDOW|WS_EX_TOPMOST,
		::AfxRegisterWndClass(
		CS_VREDRAW|CS_HREDRAW),
		NULL,
		WS_POPUP,
		0, 0, 10, 10,
		pParent->GetSafeHwnd(),
		NULL);
}

BOOL CGSToolTipWnd::OnEraseBkgnd( CDC* pDC)
{
	Darw(pDC);
	return TRUE;
}

void CGSToolTipWnd::OnPaint()
{
	CPaintDC dc(this);
	Darw(&dc);
}

void CGSToolTipWnd::SetText( CString strText)
{
	CClientDC dc(this);

	m_strText = strText;
	dc.DrawText(
		m_strText,
		&m_rect,
		DT_CALCRECT);

	m_rect.InflateRect( 1, 1);
	MoveWindow(
		m_rect,
		FALSE);
}

void CGSToolTipWnd::SetPos( CPoint point)
{
	point.x -= m_rect.Width();
	m_rect.OffsetRect(
		point.x - m_rect.left,
		point.y - m_rect.top);

	MoveWindow(
		m_rect,
		FALSE);
}

void CGSToolTipWnd::Darw( CDC *pDC)
{
	CRect rect;

	CBitmap bmp;
	CDC dc;

	GetClientRect(&rect);
	dc.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap( pDC, rect.Width(), rect.Height());
	CBitmap *pOLD = dc.SelectObject(&bmp);

	dc.FillRect(
		&rect,
		&CBrush(GetSysColor(COLOR_BTNSHADOW)));

	dc.FillRect(
		&CRect(
		rect.left + 1,
		rect.top + 1,
		rect.right - 1,
		rect.bottom - 1),
		&CBrush(GetSysColor(COLOR_INFOBK)));

	dc.SetTextColor(GetSysColor(COLOR_INFOTEXT));
	dc.SetBkColor(GetSysColor(COLOR_INFOBK));

	dc.DrawText(
		m_strText,
		&CRect(
		rect.left + 1,
		rect.top + 1,
		rect.right - 1,
		rect.bottom - 1),
		DT_SINGLELINE|
		DT_CENTER|
		DT_VCENTER);

	pDC->BitBlt(
		0, 0,
		rect.Width(),
		rect.Height(),
		&dc, 0, 0,
		SRCCOPY);

	dc.SelectObject(pOLD);
	bmp.DeleteObject();
	dc.DeleteDC();
}
