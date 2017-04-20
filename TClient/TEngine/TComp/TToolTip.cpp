// TToolTip.cpp: implementation of the TToolTip class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

TToolTip::TToolTip()
{
	Reset();
	m_ptOffset = CPoint(0, 0);
	m_dwTickCount = GetTickCount();
}

TToolTip::~TToolTip()
{

}

void TToolTip::Offset(CPoint ptOffset)
{
	m_ptOffset = ptOffset;
}

void TToolTip::Reset()
{
	m_ptr = NULL;
	m_pt = CPoint(-1, -1);
}

void TToolTip::Setup( CPoint pt, TComponent *ptr, DWORD dwTickCount)
{
	m_ptr = ptr;
	m_pt = pt + m_ptOffset;		
	m_dwTickCount = dwTickCount;
}

BOOL TToolTip::IsVisible(DWORD dwCurTickCount)
{
	return (m_ptr && (m_dwTickCount + TOOLTIP_DELAY) < dwCurTickCount);
}

void TToolTip::GetTooltipText(CString & strTooltip)
{
	strTooltip.Empty();
	if(m_ptr) strTooltip = m_ptr->m_strTooltip;
}
