#include "Stdafx.h"

// ===============================================================================
#define TAB_WINK_PERIOD		500
// ===============================================================================
const DWORD CTChatTabButton::FONT_COLOR_NORMAL	= 0xFFB19D67;
const DWORD CTChatTabButton::FONT_COLOR_PUSH	= 0xFFC1AD77;
const DWORD CTChatTabButton::FONT_COLOR_HOVER	= 0xFFD1BD87;
// ===============================================================================

// ===============================================================================
CTChatTabButton::CTChatTabButton(CTChatFrame* pHost, LP_FRAMEDESC pDesc) 
	: TButton(pHost, pDesc), m_pHost(pHost), m_pList(NULL), m_iSlot(T_INVALID), 
	  m_bSelect(FALSE), m_dwTotalTick(0), m_bNewMsg(FALSE), m_bHovering(FALSE)
{
	m_style |= TBS_STATE_BUTTON;
	ResetFontColor();
}
// ===============================================================================

// ===============================================================================
void CTChatTabButton::Init(INT iSlot, CTChatList* pList)
{
	LinkWithList(pList);
	SetSlot(iSlot);

	m_dwTotalTick = 0;
	m_bNewMsg = FALSE;
}
// -------------------------------------------------------------------------------
void CTChatTabButton::Update(DWORD dwTick)
{
	m_dwTotalTick += dwTick;

	if( IsVisible() && !m_bSelect && m_bNewMsg )
	{
		if( !m_bHovering )
		{
			INT tp = m_dwTotalTick % (TAB_WINK_PERIOD<<1);
			if( tp < TAB_WINK_PERIOD )
				m_bState |= TBUTTON_STATE_HOVER;
			else
				m_bState &= (~TBUTTON_STATE_HOVER);

			ResetFontColor();
		}
	}
	else
	{
		m_bNewMsg = FALSE;
	}
}
// ===============================================================================
void CTChatTabButton::LinkWithList(CTChatList* pList)
{
	m_pList = pList;
	m_strText = pList->GetTitle();
}
// -------------------------------------------------------------------------------
void CTChatTabButton::SetSlot(INT iSlot)
{
	m_iSlot = iSlot;
	m_rc = m_pHost->GetTabSlotRect(iSlot);
}
// ===============================================================================
void CTChatTabButton::SetSelect(BOOL bSel)
{
	if( m_bSelect == bSel )
		return;

	m_bSelect = bSel;

	if( m_bSelect )
		m_bState |= TBUTTON_STATE_DOWN;
	else
	{
		m_bState &= (~TBUTTON_STATE_DOWN);
	}

	if( m_pList && IsVisible() )
		m_pList->ShowComponent(m_bSelect);

	m_bNewMsg = FALSE;
	
	ResetFontColor();
}
// ===============================================================================
void CTChatTabButton::NotifyNewMsg()
{
	m_bNewMsg = TRUE;
}
// ===============================================================================

// ===============================================================================
void CTChatTabButton::OnMouseMove(UINT nFlags, CPoint pt)
{
	if(!CanProcess())
		return;

	if(!HitTest(pt))
	{
		m_bHovering = FALSE;
		m_bState &= (~TBUTTON_STATE_HOVER);

		if( !(m_style & TBS_STATE_BUTTON) )
			m_bState &= (~TBUTTON_STATE_DOWN);
	}
	else if( !(m_bState & TBUTTON_STATE_DOWN) && !(m_bState & TBUTTON_STATE_HOVER) && m_pHover )
	{
		m_bHovering = TRUE;
		m_bState |= TBUTTON_STATE_HOVER;
	}

	ResetFontColor();
}
// ===============================================================================

// ===============================================================================
void CTChatTabButton::ResetFontColor()
{
	if( m_bState & TBUTTON_STATE_HOVER )
		m_pFont->m_dwColor = FONT_COLOR_HOVER;
	else if( m_bState & TBUTTON_STATE_DOWN )
		m_pFont->m_dwColor = FONT_COLOR_PUSH;
	else
		m_pFont->m_dwColor = FONT_COLOR_NORMAL;
}
// ===============================================================================







