#include "Stdafx.h"
#include "TGuildExpDonDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// ====================================================================
CTGuildExpDonDlg::CTGuildExpDonDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent,pDesc)
{
	m_pMyExpTxt = FindKid(ID_CTRLINST_TXT_MYEXP);
	m_pDonExpTxt = FindKid(ID_CTRLINST_TXT_DNEXP);
	m_pMaxExpTxt = FindKid(ID_CTRLINST_TXT_MAX);
	
	m_pDonExpBar = static_cast<TScroll*>( FindKid(ID_CTRLINST_BAR) );
	m_pDonExpBar->SetScrollType(FALSE);
}
// --------------------------------------------------------------------
CTGuildExpDonDlg::~CTGuildExpDonDlg()
{
}
// ====================================================================
BOOL CTGuildExpDonDlg::InitValue()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = pGame->GetMainChar();
	
	FLOAT fCurExp = FLOAT( pMainChar->GetEXP() - pMainChar->m_dwPrevEXP );
	FLOAT fMaxExp = FLOAT( pMainChar->m_dwNextEXP - pMainChar->m_dwPrevEXP );

	FLOAT fCPM = fCurExp / fMaxExp;
	if( fCPM < MIN_EXP_DONATIONRATE )
	{
		CString strMSG;
		strMSG = CTChart::LoadString( TSTR_ERR_NOTENOUGHDNEXP);
		pGame->ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_ERROR);

		return FALSE;
	}

	if( fCPM > MAX_EXP_DONATIONRATE )
	{
		m_fMaxRate = MAX_EXP_DONATIONRATE;
		m_dwMaxExp = DWORD(fMaxExp * MAX_EXP_DONATIONRATE);
	}
	else
	{
		m_fMaxRate = fCPM;
		m_dwMaxExp = DWORD(fCurExp);
	}

	m_dwMinExp = DWORD(fMaxExp * MIN_EXP_DONATIONRATE);
	m_dwDonExp = m_dwMinExp;

	DWORD dwMaxRate = DWORD(m_fMaxRate*100.0f);
	m_pDonExpBar->SetScrollPos(dwMaxRate);

	m_pMyExpTxt->m_strText.Format("%u", DWORD(fCurExp));
	m_pDonExpTxt->m_strText.Format("%u", m_dwDonExp);
	m_pMaxExpTxt->m_strText = CTChart::Format( TSTR_FMT_NUMPERCENT, dwMaxRate);

	return TRUE;
}
// --------------------------------------------------------------------
void CTGuildExpDonDlg::UpdateValue()
{
	int nRng, nPos;
	m_pDonExpBar->GetScrollPos(nRng, nPos);
	m_dwDonExp  = m_dwMaxExp * nPos / 100;
	m_pDonExpTxt->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwDonExp);
}
// --------------------------------------------------------------------
DWORD CTGuildExpDonDlg::GetDonationExp()
{
	return m_dwDonExp;
}
// ====================================================================
HRESULT CTGuildExpDonDlg::Render( DWORD dwTickCount )
{
	if( IsVisible() )
		UpdateValue();

	return CTClientUIBase::Render(dwTickCount);
}
// ====================================================================
