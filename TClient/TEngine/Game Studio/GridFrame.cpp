// GridFrame.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "GridFrame.h"
#include "MainFrm.h"


BEGIN_MESSAGE_MAP(CGridSplitterWnd, CSplitterWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CGridSplitterWnd::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;

	GetClientRect(&rect);
	CDC dcMEM;
	CBitmap bmpMEM;

	dcMEM.CreateCompatibleDC(&dc);
	bmpMEM.CreateCompatibleBitmap( &dc, rect.Width(), rect.Height());
	CBitmap *pBMPOLD = dcMEM.SelectObject(&bmpMEM);
	CWnd *pActive = GetActivePane();

	dcMEM.FillSolidRect( rect, GetSysColor(COLOR_3DFACE));
	for( int i=0; i<m_nCols; i++)
		for( int j=0; j<m_nRows; j++)
		{
			CWnd *pWnd = GetPane( j, i);
			CRect rc;

			pWnd->GetWindowRect(&rc);
			ScreenToClient(&rc);
			dc.ExcludeClipRect(&rc);

			if( pWnd == pActive )
			{
				rc.InflateRect( 2, 2);
				dcMEM.FillSolidRect( rc, RGB( 255, 255, 0));
			}
			else
			{
				rc.InflateRect( 1, 1);
				dcMEM.FillSolidRect( rc, GetSysColor(COLOR_3DDKSHADOW));
			}
		}

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


// CGridFrame

IMPLEMENT_DYNCREATE(CGridFrame, CMDIChildWnd)

CGridFrame::CGridFrame()
{
	for( int i=0; i<2; i++)
		for( int j=0; j<2; j++)
			m_pGridView[i][j] = NULL;

	m_bFrameReady = FALSE;
	m_bFullScreen = FALSE;
}

CGridFrame::~CGridFrame()
{
}

BOOL CGridFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/, CCreateContext* pContext)
{
	BOOL bResult = m_wndSplitter.CreateStatic(
		this,
		2, 2);
	BYTE bViewPort = 0;

	if(!bResult)
		return FALSE;

	for( int i=0; i<2; i++)
		for( int j=0; j<2; j++)
		{
			bResult = m_wndSplitter.CreateView(
				i, j, pContext->m_pNewViewClass,
				CSize( 0, 0), pContext);

			if(!bResult)
				return FALSE;

			m_pGridView[i][j] = (CGridView *) m_wndSplitter.GetPane( i, j);
			m_pGridView[i][j]->m_bViewPort = bViewPort;

			bViewPort++;
		}

	m_bFullScreen = FALSE;
	m_bFrameReady = TRUE;

	return TRUE;
}

void CGridFrame::ResizeViews()
{
	if(m_bFrameReady)
	{
		CRect rect;

		RepositionBars(
			0x00000000,
			0xFFFFFFFF,
			0x00000000,
			CWnd::reposQuery,
			&rect);

		if( rect.Width() > 0 &&
			rect.Height() > 0 )
		{
			for( int i=0; i<2; i++)
			{
				m_wndSplitter.SetColumnInfo( i, rect.Width()/2, 0);
				m_wndSplitter.SetRowInfo( i, rect.Height()/2, 0);
			}

			m_wndSplitter.RecalcLayout();
		}
	}
}

void CGridFrame::SetFPS( DWORD dwFPS )
{
	for( int i=0; i<2; ++i )
		for( int j=0; j<2; ++j )
			m_pGridView[i][j]->m_dwFPS = dwFPS;
}

BEGIN_MESSAGE_MAP(CGridFrame, CMDIChildWnd)
	ON_COMMAND(ID_GRID_TOGGLE, OnGridToggle)
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CGridFrame 메시지 처리기입니다.

void CGridFrame::OnSize( UINT nType, int cx, int cy)
{
	CMDIChildWnd::OnSize( nType, cx, cy);
	ResizeViews();
}

BOOL CGridFrame::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle , const RECT& rect , CMDIFrameWnd* pParentWnd , CCreateContext* pContext)
{
	BOOL bResult = CMDIChildWnd::Create(
		lpszClassName,
		lpszWindowName,
		dwStyle,
		rect,
		pParentWnd,
		pContext);
	ShowWindow(SW_SHOWMAXIMIZED);

	return bResult;
}

void CGridFrame::OnGridToggle()
{
	if(m_bFullScreen)
	{
		m_wndSplitter.SetColumnCount(2);
		m_wndSplitter.SetRowCount(2);

		for( int i=0; i<2; i++)
			for( int j=0; j<2; j++)
			{
				SetWindowLong(
					m_pGridView[i][j]->GetSafeHwnd(),
					GWL_ID,
					m_wndSplitter.IdFromRowCol( i, j));

				m_pGridView[i][j]->ShowWindow(SW_SHOW);
			}

		m_wndSplitter.RecalcLayout();
	}
	else
	{
		CGridView *pView = (CGridView *) m_wndSplitter.GetActivePane();

		for( int i=0; i<2; i++)
			for( int j=0; j<2; j++)
			{
				CGridView *pPane = (CGridView *) m_wndSplitter.GetPane( i, j);

				if( pPane == pView )
				{
					SetWindowLong(
						pView->GetSafeHwnd(),
						GWL_ID,
						m_wndSplitter.IdFromRowCol( 0, 0));

					SetWindowLong(
						m_wndSplitter.GetPane( 0, 0)->GetSafeHwnd(),
						GWL_ID,
						m_wndSplitter.IdFromRowCol( i, j));
				}
				else
					pPane->ShowWindow(SW_HIDE);
			}

		m_wndSplitter.SetColumnCount(1);
		m_wndSplitter.SetRowCount(1);
		m_wndSplitter.RecalcLayout();
	}

	m_bFullScreen = !m_bFullScreen;
}

void CGridFrame::OnSetFocus(CWnd* pOldWnd)
{
	((CMainFrame *) AfxGetMainWnd())->LoadToolBar(IDR_GRIDTYPE);
	CMDIChildWnd::OnSetFocus(pOldWnd);
}

void CGridFrame::ResetCamera()
{
	for( int i=0; i<2; i++)
		for( int j=0; j<2; j++)
			m_pGridView[i][j]->SendMessage( WM_COMMAND, ID_GRID_CAMRESET);
}

CWnd *CGridFrame::GetActiveView()
{
	return m_wndSplitter.GetActiveView();
}
