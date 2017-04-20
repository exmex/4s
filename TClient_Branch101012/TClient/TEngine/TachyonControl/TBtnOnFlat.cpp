#include "stdafx.h"
#include "TBtnOnFlat.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CWinApp theApp;
/////////////////////////////////////////////////////////////////////////////
// CTBtnOnFlat


CTBtnOnFlat::CTBtnOnFlat()
{
	m_bCurrentlyIsFlat = TRUE;
}

CTBtnOnFlat::~CTBtnOnFlat()
{
}


IMPLEMENT_DYNCREATE( CTBtnOnFlat, CButton);

BEGIN_MESSAGE_MAP(CTBtnOnFlat, CButton)
	//{{AFX_MSG_MAP(CTBtnOnFlat)
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTBtnOnFlat message handlers

void CTBtnOnFlat::OnMouseMove( UINT nFlags, CPoint point)
{
	SetTimer( 1, 50, NULL);
	CButton::OnMouseMove( nFlags, point);
}

void CTBtnOnFlat::OnTimer(UINT nIDEvent)
{
	CRect rcItem;
	POINT pt;

	GetCursorPos(&pt);
	GetWindowRect(&rcItem);

	static BOOL bPainted = FALSE;
	if(!rcItem.PtInRect(pt))
	{
		KillTimer(1);

		if(!m_bCurrentlyIsFlat)
		{
			m_bCurrentlyIsFlat = TRUE;
			ModifyStyle( 0, BS_FLAT);
			RedrawWindow();
		}

		return;
	}
	else if(m_bCurrentlyIsFlat)
	{
		m_bCurrentlyIsFlat = FALSE;
		ModifyStyle( BS_FLAT, 0);
		RedrawWindow();
	}

	CButton::OnTimer(nIDEvent);
}

BOOL CTBtnOnFlat::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

void CTBtnOnFlat::PreSubclassWindow()
{
	CButton::PreSubclassWindow();
	ModifyStyle( 0, BS_FLAT);
}
