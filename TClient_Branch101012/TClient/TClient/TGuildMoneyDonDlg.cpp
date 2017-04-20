#include "Stdafx.h"
#include "TGuildMoneyDonDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// ====================================================================
CTGuildMoneyDonDlg::CTGuildMoneyDonDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent,pDesc)
{
	m_pDonRuneTxt = FindKid(ID_CTRLINST_RUNE);
	m_pDonLunaTxt = FindKid(ID_CTRLINST_LUNA);
	m_pDonCronTxt = FindKid(ID_CTRLINST_CRON);

	m_pMaxMoneyTxt = FindKid(ID_CTRLINST_TXT_MAX);
	m_pMaxMoneyTxt->m_strText = CTChart::Format( TSTR_FMT_RUNE, MAX_MONEY_DONATION/TRUNE_DIVIDER);
	
	m_pDonMoneyBar = static_cast<TScroll*>( FindKid(ID_CTRLINST_BAR) );
	m_pDonMoneyBar->SetScrollType(FALSE);
	m_pDonMoneyBar->SetScrollPos(100);
}
// --------------------------------------------------------------------
CTGuildMoneyDonDlg::~CTGuildMoneyDonDlg()
{
}
// ====================================================================
BOOL CTGuildMoneyDonDlg::InitValue()
{
	m_pDonMoneyBar->SetScrollPos(100,0);
	UpdateValue();

	return TRUE;
}
// --------------------------------------------------------------------
void CTGuildMoneyDonDlg::UpdateValue()
{
	int nRng, nPos;
	m_pDonMoneyBar->GetScrollPos(nRng,nPos);

	m_dwDonMoney = max( MIN_MONEY_DONATION, (MAX_MONEY_DONATION*nPos/100) );
	
	DWORD dwMinRune, dwMinLuna, dwMinCron;
	GetDonationMoney(dwMinRune, dwMinLuna, dwMinCron);

	m_pDonRuneTxt->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwMinRune);
	m_pDonLunaTxt->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwMinLuna);
	m_pDonCronTxt->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwMinCron);
}
// --------------------------------------------------------------------
void CTGuildMoneyDonDlg::GetDonationMoney(DWORD& dwRune, DWORD& dwLuna, DWORD& dwCrom)
{
	dwRune = m_dwDonMoney/TRUNE_DIVIDER;
	dwLuna = (m_dwDonMoney%TRUNE_DIVIDER)/TLUNA_DIVIDER;
	dwCrom = (m_dwDonMoney%TRUNE_DIVIDER)%TLUNA_DIVIDER;
}
// ====================================================================
HRESULT CTGuildMoneyDonDlg::Render( DWORD dwTickCount )
{
	if( IsVisible() )
		UpdateValue();

	return CTClientUIBase::Render(dwTickCount);
}
// ====================================================================
