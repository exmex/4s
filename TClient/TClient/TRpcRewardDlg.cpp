#include "stdafx.h"
#ifdef ADD_RPS
#include "TRpcRewardDlg.h"

CTRpsRewardDlg::CTRpsRewardDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase(pParent, pDesc)
{
	m_pITEM0 = (TImageList*) FindKid( ID_CTRLINST_ITEM0 );
	m_pITEM1 = (TImageList*) FindKid( ID_CTRLINST_ITEM1 );
	m_pITEM2 = (TImageList*) FindKid( ID_CTRLINST_ITEM2 );
	m_pITEM3 = (TImageList*) FindKid( ID_CTRLINST_ITEM3 );

	m_pITEMNAME0 = FindKid( ID_CTRLINST_TXT_ITEM0 );
	m_pITEMNAME1 = FindKid( ID_CTRLINST_TXT_ITEM1 );
	m_pITEMNAME2 = FindKid( ID_CTRLINST_TXT_ITEM2 );
	m_pITEMNAME3 = FindKid( ID_CTRLINST_TXT_ITEM3 );

	m_pTEXT = FindKid( ID_CTRLINST_TEXT );

	m_pGold0 = FindKid( ID_CTRLINST_GOLD0 );
	m_pSilver0 = FindKid( ID_CTRLINST_SILVER0 );
	m_pCooper0 = FindKid( ID_CTRLINST_COPPER0 );
	m_pGold1 = FindKid( ID_CTRLINST_GOLD1 );
	m_pSilver1 = FindKid( ID_CTRLINST_SILVER1 );
	m_pCooper1 = FindKid( ID_CTRLINST_COPPER1 );

	m_pBtnTry = (TButton*) FindKid( ID_CTRLINST_TRY );
	m_pBtnGiveup = (TButton*) FindKid( ID_CTRLINST_GIVEUP );
}

CTRpsRewardDlg::~CTRpsRewardDlg()
{
}

void CTRpsRewardDlg::SetReward( BYTE bGameType, BYTE bWinCount )
{
	m_pITEM0->SetCurImage( 0 );
	m_pITEM1->SetCurImage( 0 );
	m_pITEM2->SetCurImage( 0 );
	m_pITEM3->SetCurImage( 0 );

	m_pITEM0->m_strText.Empty();
	m_pITEM1->m_strText.Empty();
	m_pITEM2->m_strText.Empty();
	m_pITEM3->m_strText.Empty();

	m_pITEMNAME0->m_strText.Empty();
	m_pITEMNAME1->m_strText.Empty();
	m_pITEMNAME2->m_strText.Empty();
	m_pITEMNAME3->m_strText.Empty();

	m_pGold0->m_strText.Empty();
	m_pSilver0->m_strText.Empty();
	m_pCooper0->m_strText.Empty();
	m_pGold1->m_strText.Empty();
	m_pSilver1->m_strText.Empty();
	m_pCooper1->m_strText.Empty();

	CString strFMT;
	strFMT = CTChart::Format(TSTR_CONTINUOUS_WIN,
		bWinCount);

	m_pTEXT->m_strText = strFMT;

	m_bClose = (bWinCount == 15);

	if( m_bClose )
		m_pBtnGiveup->m_strText = CTChart::LoadString(TSTR_RPS_CLOSE);
	else
		m_pBtnGiveup->m_strText = CTChart::LoadString(TSTR_RPS_GIVEUP);

	// 현재 보상
	LPTRPS pTRPS = CTChart::FindTRPS(bGameType, bWinCount);
	if( pTRPS )
	{
		if( 0 != pTRPS->wRewardItem1 )
		{
			LPTITEM pTITEM = CTChart::FindTITEMTEMP( pTRPS->wRewardItem1 );
			if( pTITEM )
			{
				LPTITEMVISUAL pTVisual = CTChart::FindTITEMVISUAL( pTITEM->m_wVisual[0] );
				m_pITEM0->SetCurImage( pTVisual->m_wIcon );	
				m_pITEMNAME0->m_strText = pTITEM->m_strNAME;

				if( pTRPS->bRewardCount1 != 0 )
				{
					CString strFMT;
					strFMT = CTChart::Format( TSTR_RPS_COUNT, pTRPS->bRewardCount1 );
					m_pITEMNAME0->m_strText += strFMT;
					strFMT.Format( "%d", pTRPS->bRewardCount1 );
					m_pITEM0->m_strText = strFMT;
				}
			}
		}

		if( 0 != pTRPS->wRewardItem2 )
		{
			LPTITEM pTITEM = CTChart::FindTITEMTEMP( pTRPS->wRewardItem2 );
			if( pTITEM )
			{
				LPTITEMVISUAL pTVisual = CTChart::FindTITEMVISUAL( pTITEM->m_wVisual[0] );
				m_pITEM1->SetCurImage( pTVisual->m_wIcon );	
				m_pITEMNAME1->m_strText = pTITEM->m_strNAME;

				if( pTRPS->bRewardCount2 != 0 )
				{
					CString strFMT;
					strFMT = CTChart::Format( TSTR_RPS_COUNT, pTRPS->bRewardCount2 );
					m_pITEMNAME1->m_strText += strFMT;
					strFMT.Format( "%d", pTRPS->bRewardCount2 );
					m_pITEM1->m_strText = strFMT;
				}
			}
		}

		if( 0 != pTRPS->dwRewardMoney )
		{
			DWORD dwPRICE = pTRPS->dwRewardMoney;
			m_pGold0->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
				dwPRICE / TRUNE_DIVIDER);

			m_pSilver0->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
				(dwPRICE % TRUNE_DIVIDER) / TLUNA_DIVIDER);

			m_pCooper0->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
				dwPRICE % TLUNA_DIVIDER);
		}
	}

	if( !m_bClose )
	{
		// 다음 보상
		LPTRPS pTRPS = CTChart::FindTRPS(bGameType, bWinCount+1);

		if( 0 != pTRPS->wRewardItem1 )
		{
			LPTITEM pTITEM = CTChart::FindTITEMTEMP( pTRPS->wRewardItem1 );
			if( pTITEM )
			{
				LPTITEMVISUAL pTVisual = CTChart::FindTITEMVISUAL( pTITEM->m_wVisual[0] );
				m_pITEM2->SetCurImage( pTVisual->m_wIcon );	
				m_pITEMNAME2->m_strText = pTITEM->m_strNAME;

				if( pTRPS->bRewardCount1 != 0 )
				{
					CString strFMT;
					strFMT = CTChart::Format( TSTR_RPS_COUNT, pTRPS->bRewardCount1 );
					m_pITEMNAME2->m_strText += strFMT;
					strFMT.Format( "%d", pTRPS->bRewardCount1 );
					m_pITEM2->m_strText = strFMT;
				}
			}
		}

		if( 0 != pTRPS->wRewardItem2 )
		{
			LPTITEM pTITEM = CTChart::FindTITEMTEMP( pTRPS->wRewardItem2 );
			if( pTITEM )
			{
				LPTITEMVISUAL pTVisual = CTChart::FindTITEMVISUAL( pTITEM->m_wVisual[0] );
				m_pITEM3->SetCurImage( pTVisual->m_wIcon );	
				m_pITEMNAME3->m_strText = pTITEM->m_strNAME;

				if( pTRPS->bRewardCount2 != 0 )
				{
					CString strFMT;
					strFMT = CTChart::Format( TSTR_RPS_COUNT, pTRPS->bRewardCount2 );
					m_pITEMNAME3->m_strText += strFMT;
					strFMT.Format( "%d", pTRPS->bRewardCount2 );
					m_pITEM3->m_strText = strFMT;
				}
			}
		}

		if( 0 != pTRPS->dwRewardMoney )
		{
			DWORD dwPRICE = pTRPS->dwRewardMoney;
			m_pGold1->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
				dwPRICE / TRUNE_DIVIDER);

			m_pSilver1->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
				(dwPRICE % TRUNE_DIVIDER) / TLUNA_DIVIDER);

			m_pCooper1->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
				dwPRICE % TLUNA_DIVIDER);
		}
	}
}

void CTRpsRewardDlg::ShowComponent(BOOL bVisible )
{
	CTClientUIBase::ShowComponent(bVisible);

	if( bVisible && m_bClose )
		m_pBtnTry->ShowComponent(FALSE);
}

BOOL CTRpsRewardDlg::CanDisable()
{
	return TRUE;
}

#endif