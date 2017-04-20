#include "Stdafx.h"
#include "TGuildExpDonDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// ====================================================================
CTGuildExpDonDlg::CTGuildExpDonDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent,pDesc)
{
	m_pTitle = FindKid( ID_CTRLINST_TITLE );
	m_pText1 = FindKid( ID_CTRLINST_TEXT1 );
	m_pText2 = FindKid( ID_CTRLINST_TEXT2 );
	m_pMyExpTxt = FindKid(ID_CTRLINST_TXT_MYEXP);
	m_pDonExpTxt = FindKid(ID_CTRLINST_TXT_DNEXP);
	m_pMinExpTxt = FindKid( ID_CTRLINST_TXT_MIN );
	m_pMaxExpTxt = FindKid(ID_CTRLINST_TXT_MAX);
	
	m_pDonExpBar = static_cast<TScroll*>( FindKid(ID_CTRLINST_BAR) );
	m_pDonExpBar->SetScrollType(FALSE);
	m_bMode = 1;
}
// --------------------------------------------------------------------
CTGuildExpDonDlg::~CTGuildExpDonDlg()
{
}
// ====================================================================
BOOL CTGuildExpDonDlg::InitValue(BYTE bMode)
{
	m_bMode = bMode;

	if( bMode ) //경험치 기부
	{
		m_pTitle->m_strText = CTChart::LoadString( TSTR_GUILD_DONATE_EXP_TITLE );
		m_pText1->m_strText = CTChart::LoadString( TSTR_GUILD_DONATE_EXP_MY );
		m_pText2->m_strText = CTChart::LoadString( TSTR_GUILD_DONATE_EXP_DON );

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
		m_pMinExpTxt->m_strText = CTChart::Format( TSTR_FMT_NUMPERCENT, 1 );
		m_pMaxExpTxt->m_strText = CTChart::Format( TSTR_FMT_NUMPERCENT, dwMaxRate);
	}
	else // 공적 기부
	{
		m_pTitle->m_strText = CTChart::LoadString( TSTR_GUILD_DONATE_PVP_TITLE );
		m_pText1->m_strText = CTChart::LoadString( TSTR_GUILD_DONATE_PVP_MY );
		m_pText2->m_strText = CTChart::LoadString( TSTR_GUILD_DONATE_PVP_DON );

		CTClientGame* pGame = CTClientGame::GetInstance();
		CTClientChar* pMainChar = pGame->GetMainChar();

		m_fMaxRate = 1.0f;
		m_dwMaxExp = min(10000, pMainChar->m_dwPvPUseablePoint);
		m_dwMinExp = (DWORD)(1);
		m_dwDonExp = m_dwMinExp;

		m_pDonExpBar->SetScrollPos(m_dwMaxExp);
		m_pMyExpTxt->m_strText.Format("%u", DWORD(m_dwMaxExp));
		m_pDonExpTxt->m_strText.Format("%u", m_dwDonExp);
		m_pMinExpTxt->m_strText.Format( "%d", 1 );
		m_pMaxExpTxt->m_strText.Format( "%d", m_dwMaxExp);
	}

	return TRUE;
}
// --------------------------------------------------------------------
void CTGuildExpDonDlg::UpdateValue()
{
	if( m_bMode ) //경험치 기부
	{
		int nRng, nPos;
		m_pDonExpBar->GetScrollPos(nRng, nPos);
		m_dwDonExp  = m_dwMaxExp * nPos / 100;
		m_dwDonExp = max(m_dwDonExp, m_dwMinExp);
		m_pDonExpTxt->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwDonExp);
	}
	else // 공적 기부
	{
		int nRng, nPos;
		m_pDonExpBar->GetScrollPos(nRng, nPos);
		m_dwDonExp  = nPos;
		m_dwDonExp = max(m_dwDonExp, m_dwMinExp);
		m_pDonExpTxt->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_dwDonExp);
	}
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
