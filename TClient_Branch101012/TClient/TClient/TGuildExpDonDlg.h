#pragma once

class CTGuildExpDonDlg : public CTClientUIBase
{
protected:
	TComponent*		m_pMyExpTxt;
	TComponent*		m_pDonExpTxt;
	TComponent*		m_pMaxExpTxt;
	TScroll*		m_pDonExpBar;

	DWORD			m_dwMyExp;
	DWORD			m_dwDonExp;
	DWORD			m_dwMinExp;
	DWORD			m_dwMaxExp;
	FLOAT			m_fMaxRate;

public:
	BOOL InitValue();
	void UpdateValue();

	DWORD GetDonationExp();

public:
	virtual HRESULT Render( DWORD dwTickCount );

public:
	CTGuildExpDonDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildExpDonDlg();
};