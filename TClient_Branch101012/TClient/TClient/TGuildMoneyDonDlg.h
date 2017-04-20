#pragma once

class CTGuildMoneyDonDlg : public CTClientUIBase
{
protected:
	TComponent*		m_pDonRuneTxt;
	TComponent*		m_pDonLunaTxt;
	TComponent*		m_pDonCronTxt;
	TComponent*		m_pMaxMoneyTxt;
	TScroll*		m_pDonMoneyBar;

	DWORD			m_dwDonMoney;

public:
	BOOL InitValue();
	void UpdateValue();

	void GetDonationMoney(DWORD& dwRune, DWORD& dwLuna, DWORD& dwCrom);

public:
	virtual HRESULT Render( DWORD dwTickCount );

public:
	CTGuildMoneyDonDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildMoneyDonDlg();
};