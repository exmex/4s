// HoverButton.cpp : implementation file
//

#include "stdafx.h"
#include "HoverButton.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHoverButton

CHoverButton::CHoverButton()
{
	m_bHover = FALSE;
	m_bTracking = FALSE;
	m_bHoverTemp = FALSE;
	m_bFlag = FALSE;
}

CHoverButton::~CHoverButton()
{
	m_bUp.Detach();
	m_bDown.Detach();
	m_bOver.Detach();
	m_bDisable.Detach();
}
IMPLEMENT_DYNAMIC(CHoverButton, CBitmapButton)

BEGIN_MESSAGE_MAP(CHoverButton, CBitmapButton)
	//{{AFX_MSG_MAP(CHoverButton)
	ON_WM_MOUSEMOVE()
ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////
 //	CHoverButton message handlers
		
void CHoverButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	//	TODO: Add your message handler code here and/or call default

	if (!m_bTracking)
	{
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE|TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}

	if(m_bHover)
	{
		CRect rect;
		GetClientRect(&rect);

		if(!rect.PtInRect(point))
		{
			m_bHover = FALSE;
			Invalidate();
			ReleaseCapture();
			return;
		}		
	}
	else
	{		
		m_bHover = TRUE;
		Invalidate();
		SetCapture();
	}

	CBitmapButton::OnMouseMove(nFlags, point);
}

BOOL CHoverButton::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	InitToolTip();
	m_ToolTip.RelayEvent(pMsg);		
	return CButton::PreTranslateMessage(pMsg);
}

// Set the tooltip with a string resource
void CHoverButton::SetToolTipText(int nId, BOOL bActivate)
{
	CString sText;

	// load string resource
	sText.LoadString(nId);
	// If string resource is not empty
	if(sText.IsEmpty() == FALSE) 
		SetToolTipText(&sText, bActivate);
}

// Set the tooltip with a CString
void CHoverButton::SetToolTipText(CString *spText, BOOL bActivate)
{
	// We cannot accept NULL pointer
	if (spText == NULL) return;

	// Initialize ToolTip
	InitToolTip();

	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, (LPCTSTR)*spText, rectBtn, 1);
	}

	// Set text for tooltip
	m_ToolTip.UpdateTipText((LPCTSTR)*spText, this, 1);
	m_ToolTip.Activate(bActivate);
}

void CHoverButton::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		// Create ToolTip control
		m_ToolTip.Create(this);
		// Create inactive
		m_ToolTip.Activate(FALSE);
	}
} // End of InitToolTip

// Activate the tooltip
void CHoverButton::ActivateTooltip(BOOL bActivate)
{
	// If there is no tooltip then do nothing
	if (m_ToolTip.GetToolCount() == 0) return;

	// Activate tooltip
	m_ToolTip.Activate(bActivate);
} // End of EnableTooltip

void CHoverButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC *mydc=CDC::FromHandle(lpDrawItemStruct->hDC);

	CDC * pMemDC = new CDC;
	pMemDC -> CreateCompatibleDC(mydc);

	CBitmap * pOldBitmap;
	
	CRect rect;
	GetClientRect(&rect);

	if(lpDrawItemStruct->itemState & ODS_DISABLED)
	{
		if(m_bFlag)
			pOldBitmap = pMemDC->SelectObject(&m_bDisable);
		else
            pOldBitmap = pMemDC->SelectObject(&m_bUp);
	}
	else if(lpDrawItemStruct->itemState & ODS_SELECTED)
		pOldBitmap = pMemDC->SelectObject(&m_bDown);
	else
	{
		if(m_bHover)
			pOldBitmap = pMemDC->SelectObject(&m_bOver);
		else
			pOldBitmap = pMemDC->SelectObject(&m_bUp);
	}

	mydc->BitBlt(0, 0, rect.right, rect.bottom, pMemDC, 0, 0, SRCCOPY);

	// clean up
	pMemDC->SelectObject(pOldBitmap);
	delete pMemDC;
}

// Load a bitmap from the resources in the button, the bitmap has to have 3 buttonsstates next to each other: Up/Down/Hover
BOOL CHoverButton::LoadBitmap(UINT UpID, UINT DownID, UINT OverID, UINT DisID)
{
	m_bUp.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(UpID), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	m_bDown.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(DownID), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));
	m_bOver.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(OverID), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));	
	if(DisID)
	{
		m_bDisable.Attach(::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(DisID), IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS));	
		m_bFlag = TRUE;
	}

	BITMAP bitmapbits;
	m_bUp.GetBitmap(&bitmapbits);

	CSize ButtonSize;
	ButtonSize.cy = bitmapbits.bmHeight;
	ButtonSize.cx = bitmapbits.bmWidth;
	SetWindowPos( NULL, 0, 0, ButtonSize.cx ,ButtonSize.cy, SWP_NOMOVE | SWP_NOOWNERZORDER );

	return TRUE;
}

LRESULT CHoverButton::OnMouseLeave(WPARAM wparam, LPARAM lparam)
{
	m_bTracking = FALSE;
	m_bHover=FALSE;
	Invalidate();
	return 0;
}

void CHoverButton::FlashButton()
{
	m_bHoverTemp = m_bHover;
	m_bHover = !m_bHover;
	Invalidate();
}