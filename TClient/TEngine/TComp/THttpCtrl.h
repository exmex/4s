#pragma once

class THtmlDocWnd;

class THttpCtrl : public TComponent
{
public:
	THttpCtrl( TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~THttpCtrl();

public:
	static LPDIRECT3DTEXTURE9 m_pTIMG;
	static CSize m_vTBUFSIZE;

public:
	static BYTE m_bRedrawHTTP;

private:
	static void CheckBUF();

public:
	static BYTE IsFocusedHTML( HWND hWND);
	static void ReleaseBUF();
	static void UpdateTIMGBUF();

public:
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render( DWORD dwTickCount);

public:
	virtual BOOL OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message, CPoint pt);
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual void OnLButtonDblClk( UINT nFlags, CPoint pt);
	virtual void OnMButtonDblClk( UINT nFlags, CPoint pt);
	virtual void OnRButtonDblClk( UINT nFlags, CPoint pt);
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);
	virtual void OnMButtonDown( UINT nFlags, CPoint pt);
	virtual void OnRButtonDown( UINT nFlags, CPoint pt);
	virtual void OnLButtonUp( UINT nFlags, CPoint pt);
	virtual void OnMButtonUp( UINT nFlags, CPoint pt);
	virtual void OnRButtonUp( UINT nFlags, CPoint pt);
	virtual void OnMouseMove( UINT nFlags, CPoint pt);

	virtual void OnChar( UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnLoseFocus( TComponent *pSetFocus);
	virtual void OnSetFocus( TComponent *pLoseFocus);

private:
	CWnd *GetChildFromPoint( CPoint point);

	BYTE OnMouseMSG(
		UINT nMSG,
		UINT nFlags,
		CPoint point);

public:
	CString m_strURL;

public:
	void ReleaseHTTP();
	void MoveWndHTTP();
	BYTE Navigate( LPCTSTR szURL);
	BYTE InitHTTP();
	BYTE HaveCaret( CRect *pRECT);
	void Redraw();

public:
	IWebBrowser2 *m_pHTTP;
	static CWnd m_vHTTP;
	DWORD m_dwTick;
	BYTE m_bCaret;
	BYTE m_bLButtonDown;
};
