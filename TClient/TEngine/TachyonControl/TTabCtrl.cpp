#include "StdAfx.h"
#include "TTabCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTTabCtrl

CTTabCtrl::CTTabCtrl()
{
	m_nMenuID = 0;
	m_nPos = 0;
}

CTTabCtrl::~CTTabCtrl()
{
	m_Font.DeleteObject();
}

IMPLEMENT_DYNAMIC(CTTabCtrl, CTabCtrl)

BEGIN_MESSAGE_MAP(CTTabCtrl, CTabCtrl)
	//{{AFX_MSG_MAP(CTTabCtrl)
	ON_WM_RBUTTONDOWN()
	ON_WM_MEASUREITEM()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTTabCtrl message handlers

void CTTabCtrl::OnRButtonDown( UINT nFlags, CPoint point)
{
	if(m_nMenuID)
	{
		CPoint pt = point;
		ClientToScreen(&pt);

		m_Menu.LoadMenu(m_nMenuID);
		CMenu *pPopup = (CMenu *) m_Menu.GetSubMenu(m_nPos);
		CTPopupMenuWnd wndPopup;

		wndPopup.UpdateFromMenu(
			AfxGetMainWnd()->GetSafeHwnd(),
			pPopup);

		wndPopup.TrackPopupMenu(
			TPM_LEFTALIGN|TPM_RIGHTBUTTON,
			pt.x,
			pt.y);

		m_Menu.DestroyMenu();
	}
}

void CTTabCtrl::OnMeasureItem( int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	if( lpMeasureItemStruct->CtlType == ODT_MENU &&
		IsMenu((HMENU) lpMeasureItemStruct->itemID) )
	{
		CMenu *pMenu = CMenu::FromHandle((HMENU) lpMeasureItemStruct->itemID);

		if(::IsMenu(pMenu->GetSafeHmenu()))
		{
			m_Menu.MeasureItem(lpMeasureItemStruct);
			return;
		}
	}

	CTabCtrl::OnMeasureItem( nIDCtl, lpMeasureItemStruct);
}

BOOL CTTabCtrl::Create( CWnd *pParentWnd, UINT nID, DWORD dwStyle, const CRect& rect)
{
	if(!CTabCtrl::Create( dwStyle, rect, pParentWnd, nID))
		return FALSE;

	UpdateFont();
	return TRUE;
}

BOOL CTTabCtrl::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CTTabCtrl::UpdateFont()
{
	m_Font.DeleteObject();
	NONCLIENTMETRICS ncm;

	ncm.cbSize = sizeof(ncm);
	SystemParametersInfo(
		SPI_GETNONCLIENTMETRICS,
		sizeof(ncm),
		&ncm, 0);

	m_Font.CreateFontIndirect(&ncm.lfMessageFont);
	SetFont(&m_Font);
}

void CTTabCtrl::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;

	GetWindowRect(&rect);
	AdjustRect( FALSE, &rect);
	ScreenToClient(&rect);
	rect.bottom -= 3;
	dc.ExcludeClipRect(&rect);

	GetClientRect(&rect);
	CDC dcMEM;
	CBitmap bmpMEM;

	dcMEM.CreateCompatibleDC(&dc);
	bmpMEM.CreateCompatibleBitmap( &dc, rect.Width(), rect.Height());
	CBitmap *pBMPOLD = dcMEM.SelectObject(&bmpMEM);

	dcMEM.FillSolidRect( rect, afxData.clrBtnFace);
	CWnd::DefWindowProc( WM_PAINT, (WPARAM) dcMEM.m_hDC, 0);
	dc.BitBlt(
		rect.left,
		rect.top,
		rect.Width(),
		rect.Height(),
		&dcMEM,
		0, 0,
		SRCCOPY);

	dcMEM.SelectObject(pBMPOLD);
	dcMEM.DeleteDC();
	bmpMEM.DeleteObject();
}
