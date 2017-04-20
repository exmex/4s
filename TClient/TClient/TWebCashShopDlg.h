#pragma once

#ifdef ADD_WEB_CASHSHOP

class CTWebCashShopDlg : public CTClientUIBase
{
public:
	CTWebCashShopDlg( TComponent *pParent, LP_FRAMEDESC pDesc );
	virtual ~CTWebCashShopDlg();
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual void OnMouseMove( UINT nFlags, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	void InitWebCashShop();
	void Navigate();
	BOOL IsWebShop();
	

private:
	enum
	{ 
		WEBCASHSHOP_STRING_COUNT = 3,
		WEBCASHSHOP_AREAR_COUNT_KOREA = 6
	};

public:
	THttpCtrl*	m_pHTTP;
	CString		m_strURL;
	BOOL		m_bWebShop;
	BOOL		m_bNavigate;
};


#endif