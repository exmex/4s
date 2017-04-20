// TToolTip.h: interface for the TToolTip class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTOOLTIP_H__9E0F35C3_08B3_4F7E_BE76_2CFBB13C7852__INCLUDED_)
#define AFX_TTOOLTIP_H__9E0F35C3_08B3_4F7E_BE76_2CFBB13C7852__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#define TOOLTIP_DELAY				1000 //1 sec

class TToolTip  
{
public:
	TToolTip();
	virtual ~TToolTip();
	
	void Reset();
	void Offset(CPoint ptOffset);
	BOOL IsVisible(DWORD dwCurTickCount);
	void GetTooltipText(CString & strTooltip);
	void Setup(CPoint pt, TComponent *ptr, DWORD dwTickCount);	
	

	CPoint			m_pt;
	CPoint			m_ptOffset;
	
	TComponent *	m_ptr;
	DWORD			m_dwTickCount;	
};

#endif // !defined(AFX_TTOOLTIP_H__9E0F35C3_08B3_4F7E_BE76_2CFBB13C7852__INCLUDED_)
