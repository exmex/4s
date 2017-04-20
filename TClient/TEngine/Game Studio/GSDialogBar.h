#pragma once


// CGSDialogBar

class CGSDialogBar : public CControlBar
{
	DECLARE_DYNAMIC(CGSDialogBar)

public:
	CGSDialogBar();
	virtual ~CGSDialogBar();

	virtual CSize CalcFixedLayout(
		BOOL bStretch,
		BOOL bHorz);

	virtual void OnUpdateCmdUI(
		CFrameWnd* pTarget,
		BOOL bDisableIfNoHndler);

public:
	static void ReleaseGSDialogBar();
	static void InitGSDialogBar();

	static HCURSOR m_hHand;

protected:
	BOOL CanScroll();

protected:
	CPoint m_prev;
	BOOL m_bScroll;

public:
	CDialog* m_pContent;
	CSize m_szDefault;
	CSize m_szContent;

protected:
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL Create( CWnd* pParentWnd, CDialog *pContent, DWORD dwStyle, UINT nTemplateID);
	afx_msg void OnSize( UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
};
