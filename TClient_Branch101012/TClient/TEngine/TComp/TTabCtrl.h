// TTabCtrl.h: interface for the TTabCtrl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTABCTRL_H__013A523D_A66D_45CD_BC03_3E4A3AE92723__INCLUDED_)
#define AFX_TTABCTRL_H__013A523D_A66D_45CD_BC03_3E4A3AE92723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TFrame.h"
#include "TButton.h"

class TTabCtrl : public TComponent  
{
public:
	TTabCtrl(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~TTabCtrl();
	
	FRAMEARRAY	m_Frames;	
	BUTTONARRAY m_Buttons;

	void SelectItem( int nItem );
	void DeleteItem( int nItem );
	BOOL InsertItem( int nItem, LPCTSTR lpszItem, LP_FRAMEDESC pDescFrame );

	virtual void EnableComponent( BOOL bEnable = TRUE);
	virtual void ShowComponent( BOOL bVisible = TRUE);

public://Message Handler
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void OnLButtonUp( UINT nFlags, CPoint pt );
	virtual void OnMouseMove( UINT nFlags, CPoint pt );
	virtual void OnRButtonDown( UINT nFlags, CPoint pt );
	virtual void OnRButtonUp( UINT nFlags, CPoint pt );

	virtual void OnNotify( DWORD from, WORD msg, LPVOID param );
};

#endif // !defined(AFX_TTABCTRL_H__013A523D_A66D_45CD_BC03_3E4A3AE92723__INCLUDED_)
