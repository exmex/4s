// TButton.h: interface for the TButton class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TBUTTON_H__548B6C7D_AE7E_494E_A96C_9B68C037A1BE__INCLUDED_)
#define AFX_TBUTTON_H__548B6C7D_AE7E_494E_A96C_9B68C037A1BE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TBUTTON_STATE_NULL			((BYTE)0x00)
#define TBUTTON_STATE_DOWN			((BYTE)0x01)
#define TBUTTON_STATE_HOVER			((BYTE)0x02)


class TButton : public TComponent  
{
public:
	static DWORD m_dwHOVERSND;
	static DWORD m_dwDOWNSND;
	static DWORD m_dwUPSND;

	BYTE m_bSND;

public:
	BOOL IsStateDown();
	void SetPushButton();
	void SetStateButton();
	BYTE GetState();
	void Select( BOOL bSelect );
	void Select( BYTE bState );
	void SetImageIndex( int nIndex );
	
	virtual BOOL HitTest( CPoint pt);
	virtual void EnableComponent( BOOL bEnable = TRUE);

	TButton( TComponent* pParent, LP_FRAMEDESC pDesc );
	virtual ~TButton();

public:
	TComponent* m_pHover;
	TComponent* m_pDown;
	TComponent* m_pUp;

	TComponent* m_pILFirst;
	TComponent* m_pILSecond;

	BYTE		m_bState;
	BYTE		m_bImageIndex;
	BYTE		m_bTwinkleUpdate;
	DWORD	m_dwTwinkleAccTime;

public:
	BYTE		m_bTwinkle;
	WORD	m_wTwinkleGap;

public:	
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual void OnRButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);

	virtual HRESULT Render( DWORD dwTickCount );

protected:
	void UpdateTwinkle( DWORD dwTickCount );
};

typedef vector < TButton * > BUTTONARRAY;

#endif // !defined(AFX_TBUTTON_H__548B6C7D_AE7E_494E_A96C_9B68C037A1BE__INCLUDED_)
