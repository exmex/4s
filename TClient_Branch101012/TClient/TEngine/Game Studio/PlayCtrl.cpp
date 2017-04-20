// PlayCtrl.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "PlayCtrl.h"

#define TICMAX_DEFAULT			30
#define MARKER_SIZE				5
#define FREQ_DEFAULT			1


CMarkCtrl::CMarkCtrl()
{
	m_pPlayCtrl = NULL;
	m_pHandler = NULL;

	m_dwMoveID = NULL;
	m_dwCopyID = NULL;

	m_vMarker.clear();
	m_bMode = MM_NONE;

	m_nIndex = -1;
	m_nFrom = -1;
}

CMarkCtrl::~CMarkCtrl()
{
	m_vMarker.clear();
}

void CMarkCtrl::AddMark( int nPos)
{
	VECTORMARKER::iterator it;

	if( nPos == -1 )
		return;

	for( it = m_vMarker.begin(); it != m_vMarker.end(); it++)
		if( nPos < *it )
			break;
		else if( nPos == *it )
			return;

	if( it == m_vMarker.end() )
		m_vMarker.push_back(nPos);
	else
		m_vMarker.insert( it, nPos);

	Invalidate();
}

void CMarkCtrl::DelMark( int nPos)
{
	VECTORMARKER::iterator it = m_vMarker.begin();

	while( it != m_vMarker.end() )
		if( nPos == *it )
			m_vMarker.erase(it);
		else
			it++;

	Invalidate();
}

void CMarkCtrl::DelAll()
{
	m_vMarker.clear();
	Invalidate();
}

void CMarkCtrl::SetRange( int nMin, int nMax)
{
	if(m_pPlayCtrl)
	{
		int nInterval = m_pPlayCtrl->GetRangeMax() - m_pPlayCtrl->GetRangeMin();

		if( nInterval > 0 )
		{
			int nCount = INT(m_vMarker.size());
			VECTORMARKER vMarker;

			for( int i=0; i<nCount; i++)
				vMarker.push_back(m_vMarker[i]);
			m_vMarker.clear();

			for( i=0; i<nCount; i++)
				AddMark(vMarker[i] * (nMax - nMin) / nInterval);
			vMarker.clear();
		}

		m_pPlayCtrl->SetRange( nMin, nMax, TRUE);
		Invalidate();
	}
}

int CMarkCtrl::HitPosTest( CPoint point)
{
	CRect rect;
	GetClientRect(&rect);

	if(m_pPlayCtrl)
	{
		int nMax = m_pPlayCtrl->GetRangeMax();
		int nMin = m_pPlayCtrl->GetRangeMin();

		if( nMax > nMin )
		{
			int nCount = nMax - nMin;

			CRect rcCHANNEL;
			CRect rcTHUMB;

			m_pPlayCtrl->GetChannelRect(&rcCHANNEL);
			m_pPlayCtrl->GetThumbRect(&rcTHUMB);
			rcCHANNEL.DeflateRect( rcTHUMB.Width() / 2, 0);

			rcCHANNEL.right += rcCHANNEL.Width() / (2 * nCount);
			rcCHANNEL.left -=  rcCHANNEL.Width() / (2 * nCount);

			rcTHUMB.bottom = rect.bottom;
			rcTHUMB.top = rect.top;
			nCount++;

			for( int i=0; i<nCount; i++)
			{
				rcTHUMB.right = rcCHANNEL.left + (i + 1) * rcCHANNEL.Width() / nCount;
				rcTHUMB.left = rcCHANNEL.left + i * rcCHANNEL.Width() / nCount;

				if(rcTHUMB.PtInRect(point))
					return i;
			}
		}
	}

	return -1;
}

int CMarkCtrl::HitTest( CPoint point)
{
	CRect rect;
	GetClientRect(&rect);

	if(m_pPlayCtrl)
	{
		int nMax = m_pPlayCtrl->GetRangeMax();
		int nMin = m_pPlayCtrl->GetRangeMin();

		if( nMax > nMin )
		{
			int nCount = INT(m_vMarker.size());

			CRect rcCHANNEL;
			CRect rcTHUMB;

			m_pPlayCtrl->GetChannelRect(&rcCHANNEL);
			m_pPlayCtrl->GetThumbRect(&rcTHUMB);
			rcCHANNEL.DeflateRect( rcTHUMB.Width() / 2, 0);

			for( int i=0; i<nCount; i++)
			{
				rcTHUMB.right = rcCHANNEL.left + m_vMarker[i] * rcCHANNEL.Width() / (nMax - nMin) + MARKER_SIZE;
				rcTHUMB.left = rcCHANNEL.left + m_vMarker[i] * rcCHANNEL.Width() / (nMax - nMin) - MARKER_SIZE;
				rcTHUMB.bottom = rect.bottom;
				rcTHUMB.top = rect.top;

				if(rcTHUMB.PtInRect(point))
					return i;
			}
		}
	}

	return -1;
}

void CMarkCtrl::DrawItem( LPDRAWITEMSTRUCT /*lpDrawItemStruct*/)
{
}


BEGIN_MESSAGE_MAP( CMarkCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL CMarkCtrl::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CMarkCtrl::OnPaint()
{
	CPaintDC dc(this);
	CDC dcMEM;

	CBitmap *pOLD = NULL;
	CBitmap bmp;

	CRect rect;
	GetClientRect(&rect);

	dcMEM.CreateCompatibleDC(&dc);
	bmp.CreateCompatibleBitmap( &dc, rect.Width(), rect.Height());
	pOLD = dcMEM.SelectObject(&bmp);

	dcMEM.FillSolidRect(
		&rect,
		RGB( 210, 210, 180));

	if(m_pPlayCtrl)
	{
		int nMax = m_pPlayCtrl->GetRangeMax();
		int nMin = m_pPlayCtrl->GetRangeMin();

		if( nMax > nMin )
		{
			int nCount = INT(m_vMarker.size());

			CRect rcCHANNEL;
			CRect rcTHUMB;

			m_pPlayCtrl->GetChannelRect(&rcCHANNEL);
			m_pPlayCtrl->GetThumbRect(&rcTHUMB);
			rcCHANNEL.DeflateRect( rcTHUMB.Width() / 2, 0);

			for( int i=0; i<nCount; i++)
				if( m_vMarker[i] != -1 )
				{
					rcTHUMB.right = rcCHANNEL.left + m_vMarker[i] * rcCHANNEL.Width() / (nMax - nMin) + MARKER_SIZE;
					rcTHUMB.left = rcCHANNEL.left + m_vMarker[i] * rcCHANNEL.Width() / (nMax - nMin) - MARKER_SIZE;
					rcTHUMB.bottom = rect.bottom;
					rcTHUMB.top = rect.top;

					dcMEM.FillSolidRect( &rcTHUMB, RGB( 0, 0, 0));
					rcTHUMB.DeflateRect( 1, 1);
					dcMEM.FillSolidRect( &rcTHUMB, i == m_nIndex ? RGB( 255, 255, 255) : RGB( 255, 0, 0));
				}
		}
	}
	dc.BitBlt( rect.left, rect.top, rect.Width(), rect.Height(), &dcMEM, 0, 0, SRCCOPY);

	dcMEM.SelectObject(pOLD);
	bmp.DeleteObject();
	dcMEM.DeleteDC();
}

void CMarkCtrl::OnLButtonDown( UINT nFlags, CPoint point)
{
	m_nIndex = HitTest(point);

	if( m_nIndex != -1 )
	{
		if(nFlags&MK_CONTROL)
		{
			m_vMarker.push_back(m_vMarker[m_nIndex]);
			m_bMode = MM_COPY;
		}
		else
			m_bMode = MM_MOVE;

		m_nFrom = m_vMarker[m_nIndex];
		SetCapture();
		Invalidate();
	}

	CStatic::OnLButtonDown( nFlags, point);
}

void CMarkCtrl::OnMouseMove( UINT nFlags, CPoint point)
{
	switch(m_bMode)
	{
	case MM_MOVE	:
		{
			int nPos = HitPosTest(point);

			if( m_vMarker[m_nIndex] != nPos )
			{
				m_vMarker[m_nIndex] = nPos;
				Invalidate();
			}
		}

		break;

	case MM_COPY	:
		{
			int nPos = HitPosTest(point);

			if( m_vMarker.back() != nPos )
			{
				m_vMarker.back() = nPos;
				Invalidate();
			}
		}

		break;
	}

	CStatic::OnMouseMove( nFlags, point);
}

void CMarkCtrl::OnLButtonUp( UINT nFlags, CPoint point)
{
	if( m_bMode != MM_NONE )
	{
		switch(m_bMode)
		{
		case MM_MOVE	:
			if( m_nFrom != m_vMarker[m_nIndex] )
			{
				int nPos = m_vMarker[m_nIndex];

				DelMark(nPos);
				AddMark(nPos);

				if(m_pHandler)
				{
					m_pHandler->SendMessage(
						m_dwMoveID,
						(WPARAM) m_nFrom,
						(LPARAM) nPos);
				}
			}

			break;

		case MM_COPY	:
			{
				int nPos = m_vMarker.back();

				m_vMarker.pop_back();
				if( nPos != -1 && nPos != m_nFrom )
				{
					AddMark(nPos);

					if(m_pHandler)
					{
						m_pHandler->SendMessage(
							m_dwCopyID,
							(WPARAM) m_nFrom,
							(LPARAM) nPos);
					}
				}
			}

			break;
		}

		m_bMode = MM_NONE;
		m_nIndex = -1;
		m_nFrom = -1;

		ReleaseCapture();
		Invalidate();
	}

	CStatic::OnLButtonUp( nFlags, point);
}


// CPlayCtrl 대화 상자입니다.

IMPLEMENT_REGISTER_WINDOW( CPlayCtrl, CDialog, CS_HREDRAW|CS_VREDRAW, 0, 0, 0)

VECTORPLAYCTRL CPlayCtrl::m_vPlayCtrl;
CFont CPlayCtrl::m_Font;


CPlayCtrl::CPlayCtrl(CWnd* pParent /*=NULL*/)
: CDialog(CPlayCtrl::IDD, pParent)
, m_bLoop(TRUE)
{
	m_cMarker.m_pPlayCtrl = &m_cPlayCtrl;

	m_pHandler = NULL;
	m_dwProcID = NULL;
	m_dwPlayID = NULL;
	m_dwStopID = NULL;
	m_dwPosID = NULL;

	m_dwBeginTick = 0;
	m_dwTickCount = 0;
	m_bPlay = FALSE;
}

CPlayCtrl::~CPlayCtrl()
{
}

void CPlayCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_POSITION, m_cPlayCtrl);
	DDX_Control(pDX, IDC_STATIC_MARKER, m_cMarker);
	DDX_Control(pDX, IDC_BUTTON_PLAY, m_cPlayBtn);
	DDX_Control(pDX, IDC_BUTTON_STOP, m_cStopBtn);
	DDX_Control(pDX, IDC_STATIC_TIC, m_cTic);
	DDX_Check(pDX, IDC_CHECK_LOOP, m_bLoop);
	DDX_Control(pDX, IDC_CHECK_LOOP, m_cLoop);
}

BEGIN_MESSAGE_MAP(CPlayCtrl, CDialog)
	ON_WM_PAINT()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_CHECK_LOOP, OnBnClickedCheckLoop)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


void CPlayCtrl::ReleasePlayCtrl()
{
	m_Font.DeleteObject();
	m_vPlayCtrl.clear();
}

void CPlayCtrl::InitPlayCtrl()
{
	m_Font.CreateFont(
		25, 0, 0, 0,
		FW_NORMAL,
		FALSE,
		FALSE, 0,
		ANSI_CHARSET,
		OUT_DEFAULT_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY,
		DEFAULT_PITCH|FF_SWISS,
		"Arial Black");
	m_vPlayCtrl.clear();
}

void CPlayCtrl::DrawProc()
{
	int nCount = INT(m_vPlayCtrl.size());
	DWORD dwTick = ::GetTickCount();

	for( int i=0; i<nCount; i++)
	{
		CPlayCtrl *pCtrl = (CPlayCtrl *) m_vPlayCtrl[i];

		if(pCtrl->m_pHandler)
		{
			pCtrl->m_dwTickCount = dwTick;

			pCtrl->m_pHandler->SendMessage(
				pCtrl->m_dwProcID,
				dwTick - pCtrl->m_dwBeginTick,
				pCtrl->m_bLoop);
		}
	}
}


// CPlayCtrl 메시지 처리기입니다.

BOOL CPlayCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cPlayBtn.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_PLAY));
	m_cStopBtn.SetIcon(AfxGetApp()->LoadIcon(IDI_ICON_STOP));
	m_cStopBtn.EnableWindow(FALSE);

	m_cPlayCtrl.SetRange( 0, TICMAX_DEFAULT);
	m_cPlayCtrl.SetTicFreq(FREQ_DEFAULT);
	m_cTic.SetFont(&m_Font);
	UpdateData(FALSE);

	return TRUE;
}

void CPlayCtrl::ResetPosition()
{
	if(!GetSafeHwnd())
		return;

	CFrameWnd *pFrame = GetParentFrame();
	CWnd *pContainer = GetParent();
	CWnd *pContent = pContainer ? pContainer->GetParent() : NULL;

	if( pContent && pContainer && pFrame )
	{
		CRect rcContainer;
		CRect rcFrame;
		CRect rcCtrl;

		CRect rcButton;
		CRect rcSlider;
		CRect rcMarker;
		CRect rcLoop;
		CRect rcTic;

		pContainer->GetClientRect(&rcContainer);
		pContainer->ClientToScreen(&rcContainer);
		pFrame->ScreenToClient(&rcContainer);
		pFrame->GetClientRect(&rcFrame);

		rcContainer.right = rcFrame.right - ::GetSystemMetrics(SM_CXBORDER);
		pFrame->ClientToScreen(&rcContainer);
		GetWindowRect(&rcCtrl);

		rcContainer.bottom = rcCtrl.bottom;
		rcCtrl.right = rcContainer.right;

		pContent->ScreenToClient(&rcContainer);
		pContainer->ScreenToClient(&rcCtrl);

		pContainer->MoveWindow(&rcContainer);
		MoveWindow(rcCtrl);

		pContainer->ClientToScreen(&rcCtrl);
		ScreenToClient(&rcCtrl);

		m_cStopBtn.GetWindowRect(&rcButton);
		ScreenToClient(&rcButton);

		rcButton.OffsetRect( rcCtrl.right - rcButton.right, 0);
		m_cStopBtn.MoveWindow(&rcButton);

		rcButton.OffsetRect( -rcButton.Width(), 0);
		m_cPlayBtn.MoveWindow(&rcButton);

		m_cTic.GetWindowRect(&rcTic);
		ScreenToClient(&rcTic);

		rcTic.OffsetRect( rcButton.left - rcTic.right, 0);
		m_cTic.MoveWindow(&rcTic);

		m_cLoop.GetWindowRect(&rcLoop);
		ScreenToClient(&rcLoop);

		rcLoop.OffsetRect( rcButton.left - rcLoop.right, 0);
		m_cLoop.MoveWindow(&rcLoop);

		m_cPlayCtrl.GetWindowRect(&rcSlider);
		ScreenToClient(&rcSlider);

		rcSlider.right = rcTic.left;
		m_cPlayCtrl.MoveWindow(&rcSlider);

		m_cMarker.GetWindowRect(&rcMarker);
		ScreenToClient(&rcMarker);

		rcMarker.right = rcTic.left;
		m_cMarker.MoveWindow(&rcMarker);
	}
}

void CPlayCtrl::OnPaint()
{
	ResetPosition();
	CDialog::OnPaint();
}

void CPlayCtrl::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( (DWORD_PTR) pScrollBar == (DWORD_PTR) &m_cPlayCtrl )
		OnPosChanged();

	CDialog::OnHScroll( nSBCode, nPos, pScrollBar);
}

void CPlayCtrl::SetPos( int nPos)
{
	if( nPos != m_cPlayCtrl.GetPos() )
	{
		SetDlgItemInt( IDC_STATIC_TIC, nPos);
		m_cPlayCtrl.SetPos(nPos);
	}
}

void CPlayCtrl::OnPosChanged()
{
	int nPos = m_cPlayCtrl.GetPos();
	SetDlgItemInt( IDC_STATIC_TIC, nPos);

	if( m_pHandler && m_dwPosID )
		m_pHandler->SendMessage( m_dwPosID, 0, (LPARAM) nPos);
}

void CPlayCtrl::InitHandler( CWnd *pHandler,
							 DWORD dwProcID,
							 DWORD dwPlayID,
							 DWORD dwStopID,
							 DWORD dwPosID)
{
	m_dwProcID = dwProcID;
	m_dwPlayID = dwPlayID;
	m_dwStopID = dwStopID;
	m_dwPosID = dwPosID;

	m_pHandler = pHandler;
}

void CPlayCtrl::SetRange( int nMin, int nMax)
{
	m_cMarker.SetRange( nMin, nMax);
}

void CPlayCtrl::Play()
{
	if(!m_bPlay)
	{
		m_dwBeginTick = ::GetTickCount();
		m_bPlay = TRUE;

		if( m_pHandler && m_dwPlayID )
			m_pHandler->SendMessage(m_dwPlayID);

		m_vPlayCtrl.push_back(this);
	}
}

void CPlayCtrl::Stop()
{
	if(m_bPlay)
	{
		VECTORPLAYCTRL::iterator it = m_vPlayCtrl.begin();

		while( it != m_vPlayCtrl.end() )
			if( (*it) == this )
				m_vPlayCtrl.erase(it);
			else
				it++;

		m_dwBeginTick = 0;
		m_dwTickCount = 0;
		m_bPlay = FALSE;

		if( m_pHandler && m_dwStopID )
			m_pHandler->SendMessage(m_dwStopID);
	}
}

void CPlayCtrl::OnBnClickedButtonPlay()
{
	m_dwBeginTick = ::GetTickCount();
	m_bPlay = TRUE;

	if( m_pHandler && m_dwPlayID )
		m_pHandler->SendMessage(m_dwPlayID);
	m_vPlayCtrl.push_back(this);

	m_cPlayCtrl.EnableWindow(FALSE);
	m_cPlayBtn.EnableWindow(FALSE);
	m_cStopBtn.EnableWindow(TRUE);
	m_cMarker.EnableWindow(FALSE);
	m_cMarker.Invalidate(FALSE);
	m_cLoop.EnableWindow(FALSE);
}

void CPlayCtrl::OnBnClickedButtonStop()
{
	VECTORPLAYCTRL::iterator it = m_vPlayCtrl.begin();

	while( it != m_vPlayCtrl.end() )
		if( (*it) == this )
			m_vPlayCtrl.erase(it);
		else
			it++;

	m_cPlayCtrl.EnableWindow(TRUE);
	m_cPlayBtn.EnableWindow(TRUE);
	m_cStopBtn.EnableWindow(FALSE);
	m_cMarker.EnableWindow(TRUE);
	m_cMarker.Invalidate(FALSE);
	m_cLoop.EnableWindow(TRUE);

	m_dwBeginTick = 0;
	m_dwTickCount = 0;
	m_bPlay = FALSE;

	if( m_pHandler && m_dwStopID )
		m_pHandler->SendMessage(m_dwStopID);
}

void CPlayCtrl::OnBnClickedCheckLoop()
{
	UpdateData();
}

void CPlayCtrl::OnDestroy()
{
	m_pHandler = NULL;

	OnBnClickedButtonStop();
	CDialog::OnDestroy();
}

void CPlayCtrl::EnableCtrl( BOOL bEnable)
{
	m_cPlayCtrl.EnableWindow(bEnable && !m_bPlay);
	m_cPlayBtn.EnableWindow(bEnable && !m_bPlay);
	m_cStopBtn.EnableWindow(bEnable && m_bPlay);
	m_cMarker.EnableWindow(bEnable && !m_bPlay);
	m_cLoop.EnableWindow(bEnable && !m_bPlay);
	m_cTic.EnableWindow(bEnable && !m_bPlay);
}
