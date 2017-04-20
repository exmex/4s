// BarContent.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BarContent.h"


CColorBox::CColorBox()
{
	m_Color = RGB( 0, 0, 0);
}

CColorBox::~CColorBox()
{
}

void CColorBox::SetColor( COLORREF color)
{
	m_Color = color;
	Invalidate();
}

BEGIN_MESSAGE_MAP( CColorBox, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CColorBox::OnEraseBkgnd( CDC* pDC)
{
	CRect rect;

	GetClientRect(&rect);
	pDC->FillSolidRect(
		&rect,
		m_Color);

	return TRUE;
}

void CColorBox::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;

	GetClientRect(&rect);
	dc.FillSolidRect(
		&rect,
		m_Color);
}


IMPLEMENT_DYNAMIC( CBarContent, CDialog)

CBarContent::CBarContent( UINT nTemplate, CWnd* pParent)
: CDialog( nTemplate, pParent)
{
}

CBarContent::~CBarContent()
{
}

void CBarContent::DoDataExchange( CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP( CBarContent, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CBarContent 메시지 처리기입니다.

void CBarContent::OnLButtonDown( UINT nFlags, CPoint point)
{
	CWnd *pWnd = GetParent();

	if(pWnd)
	{
		CPoint screen = point;
		ClientToScreen(&screen);
		pWnd->ScreenToClient(&screen);

		pWnd->SendMessage(
			WM_LBUTTONDOWN,
			(WPARAM) nFlags,
			(LPARAM) MAKELONG( screen.x, screen.y));
	}

	CDialog::OnLButtonDown( nFlags, point);
}

void CBarContent::OnOK()
{
}
