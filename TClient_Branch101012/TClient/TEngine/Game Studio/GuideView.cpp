// GuideView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "GuideView.h"
#include "MainFrm.h"
#include ".\guideview.h"


#define GUIDE_DIST					10

CGSToolTipWnd CGuideView::m_wndToolTip;

// CGuideView

IMPLEMENT_DYNCREATE( CGuideView, CScrollView)

CGuideView::CGuideView()
{
	m_nTimer = NULL;
}

CGuideView::~CGuideView()
{
	CGuideDoc *pDoc = GetDocument();
}


BEGIN_MESSAGE_MAP(CGuideView, CScrollView)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_DESTROY()
//	ON_WM_CHAR()
ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CGuideView 그리기입니다.

void CGuideView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();
	m_DropTarget.Register(this);
	CSize sizeTotal;

	sizeTotal.cx = sizeTotal.cy = 100;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CGuideView::OnDraw( CDC* pDC)
{
	CGuideDoc *pDoc = GetDocument();
	CRect rect;

	CBitmap bmp;
	CDC dc;

	ResetScrollSize();
	Render();

	int nPosX = GetScrollPos(SB_HORZ);
	int nPosY = GetScrollPos(SB_VERT);

	GetClientRect(&rect);
	dc.CreateCompatibleDC(pDC);
	bmp.CreateCompatibleBitmap( pDC, rect.Width(), rect.Height());
	CBitmap *pOLD = dc.SelectObject(&bmp);

	dc.FillRect(
		&rect,
		&CBrush(GetSysColor(COLOR_BTNFACE)));

	dc.FillRect(
		&CRect(
		GNC_SIZE_X - 1,
		GNC_SIZE_Y - 1,
		rect.Width(),
		rect.Height()),
		&CBrush(GetSysColor(COLOR_BTNSHADOW)));

	int nCount = INT(pDoc->m_pGuide->m_vGuideH.size());
	for( int i=0; i<nCount; i++)
		if( pDoc->m_pGuide->m_vGuideH[i] >= 0 )
		{
			dc.FillRect(
				&CRect(
				0, pDoc->m_pGuide->m_vGuideH[i] - GG_SIZE_Y - nPosY + GNC_SIZE_Y,
				GNC_SIZE_X,
				pDoc->m_pGuide->m_vGuideH[i] + GG_SIZE_Y - nPosY + GNC_SIZE_Y),
				&CBrush(RGB( 0xFF, 0x00, 0x00)));
		}

	nCount = INT(pDoc->m_pGuide->m_vGuideV.size());
	for( i=0; i<nCount; i++)
		if( pDoc->m_pGuide->m_vGuideV[i] >= 0 )
		{
			dc.FillRect(
				&CRect(
				pDoc->m_pGuide->m_vGuideV[i] - GG_SIZE_X - nPosX + GNC_SIZE_X, 0,
				pDoc->m_pGuide->m_vGuideV[i] + GG_SIZE_X - nPosX + GNC_SIZE_X,
				GNC_SIZE_Y),
				&CBrush(RGB( 0xFF, 0x00, 0x00)));
		}

	dc.FillRect(
		&CRect(
		0, 0,
		GNC_SIZE_X,
		GNC_SIZE_Y),
		&CBrush(GetSysColor(COLOR_BTNSHADOW)));

	dc.FillRect(
		&CRect(
		0, 0,
		GNC_SIZE_X - 1,
		GNC_SIZE_Y - 1),
		&CBrush(RGB( 0xFF, 0xFF, 0xFF)));

	pDC->ExcludeClipRect(
		&CRect(
		nPosX + GNC_SIZE_X,
		nPosY + GNC_SIZE_Y,
		nPosX + rect.Width(),
		nPosY + rect.Height()));

	pDC->BitBlt(
		nPosX, nPosY,
		rect.Width(),
		rect.Height(),
		&dc, 0, 0,
		SRCCOPY);

	dc.SelectObject(pOLD);
	bmp.DeleteObject();
	dc.DeleteDC();
}


// CGuideView 진단입니다.

#ifdef _DEBUG
void CGuideView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CGuideView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG


// CGuideView 메시지 처리기입니다.

void CGuideView::ResetScrollSize()
{
	CSize size = GetDocument()->GetSize();
	CRect rect;

	GetClientRect(&rect);
	size.cx++;
	size.cy++;

	if( GetTotalSize() != size )
	{
		CGuideDoc *pDoc = GetDocument();
		VECTORINT::iterator it = pDoc->m_pGuide->m_vGuideV.begin();

		while(it != pDoc->m_pGuide->m_vGuideV.end())
			if( max( size.cx, rect.Width()) < (*it) )
				pDoc->m_pGuide->m_vGuideV.erase(it);
			else
				it++;

		it = pDoc->m_pGuide->m_vGuideH.begin();
		while(it != pDoc->m_pGuide->m_vGuideH.end())
			if( max( size.cy, rect.Height()) < (*it) )
				pDoc->m_pGuide->m_vGuideH.erase(it);
			else
				it++;

		SetScrollSizes( MM_TEXT, size);
		Invalidate();
	}
}

BOOL CGuideView::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CGuideView::OnLButtonDown( UINT nFlags, CPoint point)
{
	CGuideDoc *pDoc = GetDocument();
	
	m_vHIT = HitTest(point);
	switch(m_vHIT.m_bType)
	{
	case GH_GUIDE_X	:
		if( m_vHIT.m_nHIT == -1 )
		{
			pDoc->m_pGuide->m_vGuideV.push_back(GetScrollPos(SB_HORZ) + point.x - GNC_SIZE_X);

			m_vHIT.m_bType = GH_NONE;
			m_vHIT.m_point.x = 0;
			m_vHIT.m_point.y = 0;
			m_vHIT.m_nHIT = -1;

			Invalidate();
		}
		else
		{
			m_nTimer = SetTimer( (UINT_PTR) this, 1, 0);
			SetCapture();
			Invalidate();
		}

		break;

	case GH_GUIDE_Y	:
		if( m_vHIT.m_nHIT == -1 )
		{
			pDoc->m_pGuide->m_vGuideH.push_back(GetScrollPos(SB_VERT) + point.y - GNC_SIZE_Y);

			m_vHIT.m_bType = GH_NONE;
			m_vHIT.m_point.x = 0;
			m_vHIT.m_point.y = 0;
			m_vHIT.m_nHIT = -1;

			Invalidate();
		}
		else
		{
			m_nTimer = SetTimer( (UINT_PTR) this, 1, 0);
			SetCapture();
			Invalidate();
		}

		break;

	case GH_ITEM	:
		{
			switch(pDoc->m_bMouseCheck)
			{
			case GH_MS_NONE:
				if( GetAsyncKeyState(VK_LCONTROL) )
				{
				}
				else if( pDoc->m_vSEL.size() == 1 ) 
				{
					pDoc->m_vSEL.clear();
					InsertSelectITEM( m_vHIT.m_nHIT );
					pDoc->SetCurSel( m_vHIT.m_nHIT );
				}
				else if( pDoc->m_vSEL.empty() )
					InsertSelectITEM( m_vHIT.m_nHIT );
				break;
			default:
				GetDocument()->SetCurSel(m_vHIT.m_nHIT);
				SetCapture();
				break;
			}
		Invalidate();
		}

		break;
	case GH_NONE:
		GetDocument()->SetCurSel(-1);
		pDoc->m_vSEL.clear();
		pDoc->m_bMouseCheck = GH_MS_MLBPUSH;
		pDoc->m_rectMouseDrag.SetRect( point, point );
		if( !GetAsyncKeyState(VK_LCONTROL) ) pDoc->m_vSEL.clear();
		
		break;
	}

	CScrollView::OnLButtonDown( nFlags, point);
}

void CGuideView::OnLButtonUp( UINT nFlags, CPoint point)
{
	CGuideDoc *pDoc = GetDocument();

	switch(m_vHIT.m_bType)
	{
	case GH_GUIDE_X	:
		{
			CRect rect;

			pDoc->m_pGuide->m_vGuideV.erase(pDoc->m_pGuide->m_vGuideV.begin() + m_vHIT.m_nHIT);
			GetClientRect(&rect);
			rect.bottom = GNC_SIZE_Y;
			rect.left = GNC_SIZE_X;

			if(rect.PtInRect(point))
			{
				VECTORINT::iterator it = pDoc->m_pGuide->m_vGuideV.begin();
				int nPosX = point.x - m_vHIT.m_point.x;

				while(it != pDoc->m_pGuide->m_vGuideV.end())
					if( nPosX == (*it) )
						pDoc->m_pGuide->m_vGuideV.erase(it);
					else
						it++;

				pDoc->m_pGuide->m_vGuideV.push_back(nPosX);
			}

			KillTimer(m_nTimer);
			ReleaseCapture();
		}

		break;

	case GH_GUIDE_Y	:
		{
			CRect rect;

			pDoc->m_pGuide->m_vGuideH.erase(pDoc->m_pGuide->m_vGuideH.begin() + m_vHIT.m_nHIT);
			GetClientRect(&rect);
			rect.right = GNC_SIZE_X;
			rect.top = GNC_SIZE_Y;

			if(rect.PtInRect(point))
			{
				VECTORINT::iterator it = pDoc->m_pGuide->m_vGuideH.begin();
				int nPosY = point.y - m_vHIT.m_point.y;

				while(it != pDoc->m_pGuide->m_vGuideH.end())
					if( nPosY == (*it) )
						pDoc->m_pGuide->m_vGuideH.erase(it);
					else
						it++;

				pDoc->m_pGuide->m_vGuideH.push_back(nPosY);
			}

			KillTimer(m_nTimer);
			ReleaseCapture();
		}

		break;

	case GH_ITEM	: 
		switch(pDoc->m_bMouseCheck)
		{
		case GH_MS_NONE:
			if( GetAsyncKeyState(VK_LCONTROL) )
			{
				
				if( !IsSelectedItem( m_vHIT.m_nHIT ) )
					InsertSelectITEM( m_vHIT.m_nHIT );
				else
					DeleteSelectedITEM( m_vHIT.m_nHIT );
			}
			else
			{
				pDoc->m_vSEL.clear();
				pDoc->m_vSEL.push_back( m_vHIT.m_nHIT );
			}
			pDoc->m_vPreRECTinSEL = pDoc->m_vSEL;
			Invalidate();
			break;
		default :
			ReleaseCapture(); 
		}
		break;
	}

	switch( pDoc->m_bMouseCheck )
	{
	case GH_MS_MLBPUSH:
		pDoc->m_bMouseCheck = GH_MS_NONE;		// 06.02.20 박상연 Mouse State Reset
		pDoc->m_vPreRECTinSEL = pDoc->m_vSEL;
		break;
	case GH_MS_MOVE:
		pDoc->m_bMouseCheck = GH_MS_NONE;
		break;		
	}
	if( pDoc->m_vSEL.size() == 1 )
		pDoc->SetCurSel( m_vHIT.m_nHIT );

	m_vHIT.m_bType = GH_NONE;
	m_vHIT.m_point.x = 0;
	m_vHIT.m_point.y = 0;
	m_vHIT.m_nHIT = -1;
	Invalidate();
	DrawToolTip();

	CScrollView::OnLButtonUp( nFlags, point);
}

void CGuideView::OnMouseMove( UINT nFlags, CPoint point)
{
	CGuideDoc *pDoc = GetDocument();
	static CPoint prev( 0, 0);
	
		
	switch(m_vHIT.m_bType)
	{
	case GH_NONE:
		switch( pDoc->m_bMouseCheck )	
		{
		case GH_MS_MLBPUSH:
			pDoc->m_rectMouseDrag.right = point.x;
			pDoc->m_rectMouseDrag.bottom = point.y;
			Invalidate();
			HitTestRect(pDoc->m_rectMouseDrag);
			break;
		}
		break;
	case GH_GUIDE_X	:
		if( point != prev )
		{
			CRect rect;

			GetClientRect(&rect);
			rect.bottom = GNC_SIZE_Y;
			rect.left = GNC_SIZE_X;

			if(rect.PtInRect(point))
				pDoc->m_pGuide->m_vGuideV[m_vHIT.m_nHIT] = point.x - m_vHIT.m_point.x;
			else
				pDoc->m_pGuide->m_vGuideV[m_vHIT.m_nHIT] = -1;

			Invalidate();
		}

		break;

	case GH_GUIDE_Y	:
		if( point != prev )
		{
			CRect rect;

			GetClientRect(&rect);
			rect.right = GNC_SIZE_X;
			rect.top = GNC_SIZE_Y;

			if(rect.PtInRect(point))
				pDoc->m_pGuide->m_vGuideH[m_vHIT.m_nHIT] = point.y - m_vHIT.m_point.y;
			else
				pDoc->m_pGuide->m_vGuideH[m_vHIT.m_nHIT] = -1;

			Invalidate();
		}

		break;

	case GH_ITEM	:
		if( point != prev )
			if( pDoc->m_vSEL.size() != 1 && !pDoc->m_vSEL.empty() )//!pDoc->m_vSEL.empty() )
			{
				if( GetAsyncKeyState(VK_LCONTROL) )
				{
					if(!IsSelectedItem(m_vHIT.m_nHIT))
					{
						InsertSelectITEM( m_vHIT.m_nHIT );
					}
				}
				else if( !IsSelectedItem( m_vHIT.m_nHIT ) )
				{
					pDoc->m_vSEL.clear();
					InsertSelectITEM( m_vHIT.m_nHIT );
				}
				
				
				CPoint pt;
				pt = point - prev;
				
				MoveItem(point);
				pDoc->m_bMouseCheck = GH_MS_MOVE;
			}
			else
				MoveItem(point);
			break;
	}
	prev = point;

	CScrollView::OnMouseMove( nFlags, point);
}

GUIDEHIT CGuideView::HitTest( CPoint point)
{
	CGuideDoc *pDoc = GetDocument();

	GUIDEHIT vHIT;
	CRect rect;

	GetClientRect(&rect);
	rect.left = GNC_SIZE_X;
	rect.bottom = GNC_SIZE_Y;

	if(rect.PtInRect(point))
	{
		int nCount = INT(pDoc->m_pGuide->m_vGuideV.size());
		int nPosX = GetScrollPos(SB_HORZ);
		vHIT.m_bType = GH_GUIDE_X;

		for( int i=0; i<nCount; i++)
		{
			rect.SetRect(
				pDoc->m_pGuide->m_vGuideV[i] - GG_SIZE_X - nPosX + GNC_SIZE_X, 0,
				pDoc->m_pGuide->m_vGuideV[i] + GG_SIZE_X - nPosX + GNC_SIZE_X,
				GNC_SIZE_Y);

			if(rect.PtInRect(point))
			{
				vHIT.m_point.x = point.x - pDoc->m_pGuide->m_vGuideV[i];
				vHIT.m_nHIT = i;

				break;
			}
		}

		return vHIT;
	}

	GetClientRect(&rect);
	rect.right = GNC_SIZE_X;
	rect.top = GNC_SIZE_Y;

	if(rect.PtInRect(point))
	{
		int nCount = INT(pDoc->m_pGuide->m_vGuideH.size());
		int nPosY = GetScrollPos(SB_VERT);
		vHIT.m_bType = GH_GUIDE_Y;

		for( int i=0; i<nCount; i++)
		{
			rect.SetRect(
				0, pDoc->m_pGuide->m_vGuideH[i] - GG_SIZE_Y - nPosY + GNC_SIZE_Y,
				GNC_SIZE_X,
				pDoc->m_pGuide->m_vGuideH[i] + GG_SIZE_Y - nPosY + GNC_SIZE_Y);

			if(rect.PtInRect(point))
			{
				vHIT.m_point.y = point.y - pDoc->m_pGuide->m_vGuideH[i];
				vHIT.m_nHIT = i;

				break;
			}
		}

		return vHIT;
	}

	return vHIT;
}

CPoint CGuideView::GetHotPoint( CPoint point)
{
	CGuideDoc *pDoc = GetDocument();
	CRect rect;
	rect = GetItemRect(point);

	CPoint center = rect.CenterPoint();

	switch(pDoc->m_bHotPoint)
	{
	case GHP_TL	: return CPoint( rect.left, rect.top );
	case GHP_TC	: return CPoint( center.x, rect.top);
	case GHP_TR	: return CPoint( rect.right, rect.top);
	case GHP_CL	: return CPoint( rect.left, center.y);
	case GHP_CC	: return center;
	case GHP_CR : return CPoint( rect.right, center.y);
	case GHP_BL	: return CPoint( rect.left, rect.bottom);
	case GHP_BC : return CPoint( center.x, rect.bottom);
	case GHP_BR	: return rect.BottomRight();
	}

	return point;
}

CPoint CGuideView::GetTLPoint( CPoint point)
{
	return CPoint( 2 * point.x, 2 * point.y) - GetHotPoint(point);
}

void CGuideView::MoveItem( CPoint point)
{
	CGuideDoc *pDoc = GetDocument();
	
	pDoc->m_ptPreItemPos.x = GetItemRect( m_vHIT.m_nHIT ).left;
	pDoc->m_ptPreItemPos.y = GetItemRect( m_vHIT.m_nHIT ).top;

	point.x += GetScrollPos(SB_HORZ) - GNC_SIZE_X - m_vHIT.m_point.x;
	point.y += GetScrollPos(SB_VERT) - GNC_SIZE_Y - m_vHIT.m_point.y;

	point = GetNearPoint(point);
	DoMoveItem(point);

	pDoc->SetCurSel(m_vHIT.m_nHIT);
	Invalidate();
}

CRect CGuideView::GetItemRect( CPoint point)
{
	return CRect( 0, 0, 0, 0);
}

CRect CGuideView::GetItemRect()
{
	return CRect( 0, 0, 0, 0);
}

CRect CGuideView::GetItemRect(int nCnt)
{
	return CRect( 0, 0, 0, 0);
}

void CGuideView::DoMoveItem( CPoint point)
{
}

BOOL CGuideView::OnScrollBy( CSize sizeScroll, BOOL bDoScroll)
{
	CScrollBar *pBar = GetScrollBarCtrl(SB_VERT);
	DWORD dwStyle = GetStyle();

	int nStartX;
	int nStartY;
	int nPosX;
	int nPosY;

	if( (pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_VSCROLL)))
		sizeScroll.cy = 0;

	pBar = GetScrollBarCtrl(SB_HORZ);
	if( (pBar != NULL && !pBar->IsWindowEnabled()) ||
		(pBar == NULL && !(dwStyle & WS_HSCROLL)))
		sizeScroll.cx = 0;

	nStartX = GetScrollPos(SB_HORZ);
	nPosX = nStartX;

	int nMaxX = GetScrollLimit(SB_HORZ);
	nPosX += sizeScroll.cx;

	if( nPosX < 0 )
		nPosX = 0;
	else if( nPosX > nMaxX )
		nPosX = nMaxX;

	nStartY = GetScrollPos(SB_VERT);
	nPosY = nStartY;

	int nMaxY = GetScrollLimit(SB_VERT);
	nPosY += sizeScroll.cy;

	if( nPosY < 0 )
		nPosY = 0;
	else if( nPosY > nMaxY )
		nPosY = nMaxY;

	if( nPosX == nStartX && nPosY == nStartY )
		return FALSE;

	if(bDoScroll)
	{
		if( nPosX != nStartX )
			SetScrollPos( SB_HORZ, nPosX);

		if( nPosY != nStartY )
			SetScrollPos( SB_VERT, nPosY);
	}
	Invalidate();

	return TRUE;
}

void CGuideView::DrawToolTip()
{
	CGuideDoc *pDoc = GetDocument();

	switch(m_vHIT.m_bType)
	{
	case GH_GUIDE_X	:
		if( m_vHIT.m_nHIT != -1 && pDoc->m_pGuide->m_vGuideV[m_vHIT.m_nHIT] >= 0 )
		{
			int nGuide = pDoc->m_pGuide->m_vGuideV[m_vHIT.m_nHIT];

			CString strText;
			CPoint point;

			strText.Format( "%d", nGuide);
			GetCursorPos(&point);

			m_wndToolTip.ShowWindow(SW_HIDE);
			m_wndToolTip.SetText(strText);
			m_wndToolTip.SetPos(point);
			m_wndToolTip.ShowWindow(SW_SHOWNA);
			m_wndToolTip.Invalidate();
		}
		else
			m_wndToolTip.ShowWindow(SW_HIDE);

		break;

	case GH_GUIDE_Y	:
		if( m_vHIT.m_nHIT != -1 && pDoc->m_pGuide->m_vGuideH[m_vHIT.m_nHIT] >= 0 )
		{
			int nGuide = pDoc->m_pGuide->m_vGuideH[m_vHIT.m_nHIT];

			CString strText;
			CPoint point;

			strText.Format( "%d", nGuide);
			GetCursorPos(&point);

			m_wndToolTip.ShowWindow(SW_HIDE);
			m_wndToolTip.SetText(strText);
			m_wndToolTip.SetPos(point);
			m_wndToolTip.ShowWindow(SW_SHOWNA);
			m_wndToolTip.Invalidate();
		}
		else
			m_wndToolTip.ShowWindow(SW_HIDE);

		break;

	default			: m_wndToolTip.ShowWindow(SW_HIDE); break;
	}
}

void CGuideView::OnTimer( UINT nIDEvent)
{
	DrawToolTip();
	CScrollView::OnTimer(nIDEvent);
}

void CGuideView::Render()
{
	CGuideDoc* pDoc = GetDocument();
	int nPosX = GetScrollPos(SB_HORZ);
	int nPosY = GetScrollPos(SB_VERT);

	CRect rect = GetItemRect();
	CRect dest;

	if(!CMainFrame::m_bRender)
		return;

	CMainFrame::m_3DDevice.m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_XRGB( 0xFF, 0xFF, 0xFF),
		1.0f, 0);
	CMainFrame::m_3DDevice.m_pDevice->BeginScene();
	DrawContent();
	DrawGuide();

	///////////////////////////////////////////////////////
	// 06.02.20 Item Select을 위한 Rectangle 그리기
	if( pDoc->m_bMouseCheck == GH_MS_MLBPUSH )
	{
		int nPosX = GetScrollPos(SB_HORZ);
		int nPosY = GetScrollPos(SB_VERT);
		CRect rect;
		GetClientRect(&rect);		

		TNLPOINT vPoint[5] = 
		{
			{ (FLOAT)pDoc->m_rectMouseDrag.left  - nPosX - GNC_SIZE_X + GetScrollPos(SB_HORZ),
			  (FLOAT)pDoc->m_rectMouseDrag.top - nPosY - GNC_SIZE_Y + GetScrollPos(SB_VERT),
			  0.0f,		// Top, Left
			  1.0f,
			  0xFF0000FF },
			{ (FLOAT)pDoc->m_rectMouseDrag.right - nPosX - GNC_SIZE_X + GetScrollPos(SB_HORZ),
			  (FLOAT)pDoc->m_rectMouseDrag.top - nPosY - GNC_SIZE_Y + GetScrollPos(SB_VERT),
			  0.0f,		// Top, Right
			  1.0f,
			  0xFF0000FF },
			{ (FLOAT)pDoc->m_rectMouseDrag.right - nPosX - GNC_SIZE_X + GetScrollPos(SB_HORZ),
			  (FLOAT)pDoc->m_rectMouseDrag.bottom - nPosY - GNC_SIZE_Y + GetScrollPos(SB_VERT),
			  0.0f,	// Bottom, Right
			  1.0f, 
			  0xFF0000FF },
			{ ( FLOAT)pDoc->m_rectMouseDrag.left - nPosX - GNC_SIZE_X + GetScrollPos(SB_HORZ), 
			  (FLOAT)pDoc->m_rectMouseDrag.bottom - nPosY - GNC_SIZE_Y + GetScrollPos(SB_VERT),
			  0.0f,	// Bottom, Left
			  1.0f, 
			  0xFF0000FF },
			{ (FLOAT)pDoc->m_rectMouseDrag.left  - nPosX - GNC_SIZE_X + GetScrollPos(SB_HORZ),
			  (FLOAT)pDoc->m_rectMouseDrag.top - nPosY - GNC_SIZE_Y + GetScrollPos(SB_VERT),
			  0.0f,		// Top, Left
			  1.0f, 
			  0xFF0000FF },
			
		};
		
		CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
				D3DPT_LINESTRIP, 4,
				vPoint, sizeof(TNLPOINT));
	}
	
	VECTORINT::iterator itSEL;
	size_t sz = pDoc->m_vSEL.size();
	DWORD dwColor;
	if( sz == 1 ) dwColor = 0xFF0000FF;
	else		  dwColor = 0xFFFF0000;
	for( itSEL = pDoc->m_vSEL.begin() ; itSEL != pDoc->m_vSEL.end() ; itSEL++ )
	{
		int nPosX = GetScrollPos(SB_HORZ);
		int nPosY = GetScrollPos(SB_VERT);
		
		int nnn = ( *itSEL );

		CRect _rect = GetItemRect((*itSEL));
		
		_rect.OffsetRect(
		-nPosX,
		-nPosY);

		TNLPOINT vPoint[5] = {{
			((FLOAT) _rect.left),
			((FLOAT) _rect.top),
			0.0f, 1.0f, dwColor}, {
			((FLOAT) _rect.right),
			((FLOAT) _rect.top),
			0.0f, 1.0f, dwColor}, {
			((FLOAT) _rect.right),
			((FLOAT) _rect.bottom), 
			0.0f, 1.0f, dwColor}, {
			((FLOAT) _rect.left),
			((FLOAT) _rect.bottom),
			0.0f, 1.0f, dwColor}, {
			((FLOAT) _rect.left),
			((FLOAT) _rect.top),
			0.0f, 1.0f, dwColor}};

		CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
			D3DPT_LINESTRIP, 4,
			vPoint, sizeof(TNLPOINT));
	}		


	rect.OffsetRect(
		-nPosX,
		-nPosY);

	if(!rect.IsRectEmpty() )
	{
		TNLPOINT vPoint[5] = {{
			((FLOAT) rect.left),
			((FLOAT) rect.top),
			0.0f, 1.0f, 0xFF0000FF}, {
			((FLOAT) rect.right),
			((FLOAT) rect.top),
			0.0f, 1.0f, 0xFF0000FF}, {
			((FLOAT) rect.right),
			((FLOAT) rect.bottom),
			0.0f, 1.0f, 0xFF0000FF}, {
			((FLOAT) rect.left),
			((FLOAT) rect.bottom),
			0.0f, 1.0f, 0xFF0000FF}, {
			((FLOAT) rect.left),
			((FLOAT) rect.top),
			0.0f, 1.0f, 0xFF0000FF}};

		CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
			D3DPT_LINESTRIP, 4,
			vPoint, sizeof(TNLPOINT));
	}


	CMainFrame::m_3DDevice.m_pDevice->EndScene();

	GetClientRect(&rect);
	dest = rect;

	rect.bottom -= GNC_SIZE_Y;
	rect.right -= GNC_SIZE_X;
	dest.left += GNC_SIZE_X;
	dest.top += GNC_SIZE_Y;

	HWND hWND = GetSafeHwnd();
	if(hWND)
	{
		HRESULT hr = CMainFrame::m_pSWAPCHAIN->Present(
			&rect,
			&dest,
			hWND,
			NULL, 0);

		if(FAILED(hr))
			CMainFrame::RestoreDevice();
	}
}

void CGuideView::DrawContent()
{

}

void CGuideView::DrawGuide()
{
	CGuideDoc *pDoc = GetDocument();

	if(pDoc)
	{
		int nPosX = GetScrollPos(SB_HORZ);
		int nPosY = GetScrollPos(SB_VERT);
		CRect rect;

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		CMainFrame::m_3DDevice.m_pDevice->SetFVF(T3DFVF_TNLPOINT);

		GetClientRect(&rect);
		rect.bottom -= GNC_SIZE_Y;
		rect.right -= GNC_SIZE_X;

		int nCount = INT(pDoc->m_pGuide->m_vGuideV.size());
		for( int i=0; i<nCount; i++)
			if( pDoc->m_pGuide->m_vGuideV[i] >= 0 )
			{
				TNLPOINT vPoint[2] = {
					{ ((FLOAT) (pDoc->m_pGuide->m_vGuideV[i] - nPosX)), ((FLOAT) rect.top), 0.0f, 1.0f, 0xFFFF0000 },
					{ ((FLOAT) (pDoc->m_pGuide->m_vGuideV[i] - nPosX)), ((FLOAT) rect.bottom), 0.0f, 1.0f, 0xFFFF0000 }
				};

				CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
					D3DPT_LINELIST, 1,
					vPoint, sizeof(TNLPOINT));
			}

		nCount = INT(pDoc->m_pGuide->m_vGuideH.size());
		for( i=0; i<nCount; i++)
			if( pDoc->m_pGuide->m_vGuideH[i] >= 0 )
			{
				TNLPOINT vPoint[2] = {{
					((FLOAT) rect.left),
					((FLOAT) (pDoc->m_pGuide->m_vGuideH[i] - nPosY)),
					0.0f, 1.0f, 0xFFFF0000}, {
					((FLOAT) rect.right),
					((FLOAT) (pDoc->m_pGuide->m_vGuideH[i] - nPosY)),
					0.0f, 1.0f, 0xFFFF0000}};

				CMainFrame::m_3DDevice.m_pDevice->DrawPrimitiveUP(
					D3DPT_LINELIST, 1,
					vPoint, sizeof(TNLPOINT));
			}
	}
}

CPoint CGuideView::GetNearPoint( CPoint point)
{
	point = GetHotPoint(point);

	CGuideDoc *pDoc = GetDocument();
	CPoint result = point;
	BOOL bInit = FALSE;

	int nCount = INT(pDoc->m_pGuide->m_vGuideV.size());
	int nDist = 0;

	CRect rect = GetItemRect( m_vHIT.m_nHIT );
	CPoint pt;
	pt = GetRectDist();

	for( int i=0; i<nCount; i++)
	{
		int nLocal = abs(( point.x - pt.x ) - pDoc->m_pGuide->m_vGuideV[i]);

		if( nLocal < GUIDE_DIST && (
			!bInit || nLocal < nDist))
		{
			result.x = pDoc->m_pGuide->m_vGuideV[i] + pt.x ;
			nDist = nLocal;
		}
	}

	nCount = INT(pDoc->m_pGuide->m_vGuideH.size());
	bInit = FALSE;
	nDist = 0;

	for( i=0; i<nCount; i++)
	{
		int nLocal = abs((point.y - pt.y  ) - pDoc->m_pGuide->m_vGuideH[i]);

		if( nLocal < GUIDE_DIST && (
			!bInit || nLocal < nDist))
		{
			result.y = pDoc->m_pGuide->m_vGuideH[i] + pt.y;
			nDist = nLocal;
		}
	}

	return GetTLPoint(result);
}

BYTE CGuideView::CanDrop( CItemBase *pItem)
{
	return FALSE;
}

DROPEFFECT CGuideView::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return CGuideView::OnDragOver( pDataObject, dwKeyState, point);
}

void CGuideView::OnDragLeave()
{
	CGuideDoc *pDoc = GetDocument();

	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));
	pDoc->UpdateAllViews(NULL);

	CScrollView::OnDragLeave();
}

DROPEFFECT CGuideView::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	DROPEFFECT nResult = DROPEFFECT_NONE;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	CPoint screenpos = point;

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;
			CRect rect;

			GetClientRect(&rect);
			if(CanDrop(pItem))
				nResult = DROPEFFECT_MOVE;
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	ClientToScreen(&screenpos);
	CGameStudioView::m_pDragImage->DragMove(screenpos);

	return nResult;
}

BOOL CGuideView::OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;
			CRect rect;

			GetClientRect(&rect);
			if(CanDrop(pItem))
				AddItem(pItem);
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnDragLeave();
	return TRUE;
}

BOOL CGuideView::IsSelectedItem(int nSelected)
{
	CGuideDoc *pDoc = GetDocument();
	VECTORINT::iterator it;
	for( it = pDoc->m_vSEL.begin() ; it != pDoc->m_vSEL.end() ; it++ )
	{
		if( (*it) == nSelected )
			return TRUE;
	}
	return FALSE;
}

void CGuideView::DeleteSelectedITEM(int nSelected)
{
	CGuideDoc *pDoc = GetDocument();
	VECTORINT::iterator it;
	for( it = pDoc->m_vSEL.begin() ; it != pDoc->m_vSEL.end() ; it++ )
	{
		if( (*it) == nSelected )
		{
			pDoc->m_vSEL.erase(it);
			return;
		}
	}
}

void CGuideView::InsertSelectITEM(int nSelect)
{
	CGuideDoc *pDoc = GetDocument();
	pDoc->m_vSEL.push_back( nSelect );
}

BOOL CGuideView::IsPreRectInDrag(int nSelect)
{
	CGuideDoc *pDoc = GetDocument();
	VECTORINT::iterator it;
	for( it = pDoc->m_vPreRECTinSEL.begin() ; it != pDoc->m_vPreRECTinSEL.end() ; it++ )
	{
		if( (*it) == nSelect )
			return TRUE;
	}
	return FALSE;
}

void CGuideView::DeletePreRectInITEM(int nSelected)
{
	CGuideDoc *pDoc = GetDocument();
	VECTORINT::iterator it;
	for( it = pDoc->m_vPreRECTinSEL.begin() ; it != pDoc->m_vPreRECTinSEL.end() ; it++ )
	{
		if( (*it) == nSelected )
		{
			pDoc->m_vPreRECTinSEL.erase(it);
			return;
		}
	}
}

void CGuideView::InsertPreRectInITEM(int nSelect)
{
	CGuideDoc *pDoc = GetDocument();
	pDoc->m_vPreRECTinSEL.push_back( nSelect );
}


void CGuideView::AddItem( CItemBase *pItem)
{
}

void CGuideView::OnDestroy()
{
	CScrollView::OnDestroy();
}

void CGuideView::DeleteForRestore()
{
}

void CGuideView::Restore()
{
}
void CGuideView::HitTestRect(CRect rect)
{
}
CRect CGuideView::GetSelectItemRECT(void)
{
	return CRect();
}
void CGuideView::MoveItemByKeyBoard(CPoint pt)
{

}

void CGuideView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CPoint pt(0,0);
	static int nSpeed = 1;


	switch( nChar )
	{
	case VK_UP:
		pt.y = -nSpeed;
		break;
	case VK_DOWN:
		pt.y = nSpeed;
		break;
	case VK_LEFT:
		pt.x = -nSpeed;
		break;
	case VK_RIGHT:
		pt.x = nSpeed;
		break;
	}

	MoveItemByKeyBoard( pt );
	Invalidate();

	CScrollView::OnKeyDown(nChar, nRepCnt, nFlags);
}

CPoint CGuideView::GetRectDist(void)
{
	return CPoint();
}
