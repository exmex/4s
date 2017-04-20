#pragma once

class CTGuildExpDonDlg : public CTClientUIBase
{
protected:
	TComponent*		m_pTitle;
	TComponent*		m_pText1;
	TComponent*		m_pText2;
	TComponent*		m_pMyExpTxt;
	TComponent*		m_pDonExpTxt;
	TComponent*		m_pMinExpTxt;
	TComponent*		m_pMaxExpTxt;
	TScroll*		m_pDonExpBar;

	DWORD			m_dwMyExp;
	DWORD			m_dwDonExp;
	DWORD			m_dwMinExp;
	DWORD			m_dwMaxExp;
	FLOAT			m_fMaxRate;

public:
	BYTE m_bMode;
	BOOL InitValue(BYTE bMode);
	void UpdateValue();
	DWORD GetDonationExp();

public:
	virtual HRESULT Render( DWORD dwTickCount );

public:
	CTGuildExpDonDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildExpDonDlg();
};