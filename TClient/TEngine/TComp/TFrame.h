// TFrame.h: interface fo the TFrame class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TFRAME_H__9EDF45D2_9439_430C_9242_31622B4F2C76__INCLUDED_)
#define AFX_TFRAME_H__9EDF45D2_9439_430C_9242_31622B4F2C76__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TTooltip.h"
class CTachyonWnd;

class TFrame : public TComponent
{
public:
	static CTachyonWnd *m_pCommandHandler;
	static DWORD m_dwCLOSESND;
	static DWORD m_dwOPENSND;

	TFrame( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~TFrame();
	virtual DWORD SetFocus( DWORD dwID);
	virtual void SwitchFocus(TComponent *pCandidate);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual BOOL HitTest( CPoint pt);
	virtual BOOL HitTest( CPoint pt, LPIMAGESET pImgset);
	TToolTip		m_tip;		
	TComponent *	m_pFocus;
	CPoint			m_ptPrev;
	BOOL			m_blDragging;
	BYTE			m_bSND;
	BOOL		m_bTabAutoNext;

public:
	void EnableFloat(BOOL bFloatable);
	void SetCommandHandler(CTachyonWnd *pCmdHandler);	

	// message handlers
	virtual void OnSetFocus( TComponent* pLoseFocus );
	virtual void OnLoseFocus( TComponent* pSetFocus );
	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnMButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRButtonUp(UINT nFlags, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnMButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);	
	virtual void OnMButtonDblClk(UINT nFlags, CPoint pt);	
	virtual void OnRButtonDblClk(UINT nFlags, CPoint pt);
	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void OnImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInputLangChangeRequest( HWND hWnd, WPARAM wParam, LPARAM lParam );
	virtual BOOL OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message, CPoint pt);

	virtual void OnNotify(DWORD from, WORD msg, LPVOID param);

	//drawing members
	virtual HRESULT Render( DWORD dwTickCount );
	virtual HRESULT DrawKids( DWORD dwTickCount );
};

typedef vector < TFrame * > FRAMEARRAY;

#endif // !defined(AFX_TFRAME_H__9EDF45D2_9439_430C_9242_31622B4F2C76__INCLUDED_)
