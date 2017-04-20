#include "Stdafx.h"
#include "TFameRank.h"
#include "TCharRankInfoDlg.h"
#include "TClientGame.h"

BYTE CTCharRankInfoDlg::m_bTabIndex = CTCharRankInfoDlg::TRANKINFO_TAB_TOTAL;

CTCharRankInfoDlg::CTCharRankInfoDlg( TComponent *pParent, LP_FRAMEDESC pDesc )
: CTClientUIBase(pParent, pDesc) 
{
	CPoint ptRankOffset, ptGuildOffset, ptNameOffset, ptCountryOffset, ptMonthfameOffset, ptButtonOffset;

	{
		static DWORD dwBASE_NEW[ MAX_SLOT_COUNT ] =
		{
			ID_CTRLINST_SLOT1, //ID_CTRLINST_SLOT01
			ID_CTRLINST_SLOT2, //ID_CTRLINST_SLOT02
			ID_CTRLINST_SLOT3, //ID_CTRLINST_SLOT03
			ID_CTRLINST_SLOT4, //ID_CTRLINST_SLOT04
			ID_CTRLINST_SLOT5, //ID_CTRLINST_SLOT05
			ID_CTRLINST_SLOT6, //ID_CTRLINST_SLOT06
			ID_CTRLINST_SLOT7, //ID_CTRLINST_SLOT07
			ID_CTRLINST_SLOT8, //ID_CTRLINST_SLOT08
			ID_CTRLINST_SLOT9, //ID_CTRLINST_SLOT09
			ID_CTRLINST_SLOT10, //ID_CTRLINST_SLOT10
			ID_CTRLINST_SLOT11, //ID_CTRLINST_SLOT11
			ID_CTRLINST_SLOT12, //ID_CTRLINST_SLOT12
			ID_CTRLINST_SLOT13, //ID_CTRLINST_SLOT13
			ID_CTRLINST_SLOT14, //ID_CTRLINST_SLOT14
			ID_CTRLINST_SLOT15, //ID_CTRLINST_SLOT15
			ID_CTRLINST_SLOT16, //ID_CTRLINST_SLOT16
		};

		CPoint ptBase, ptRank, ptGuild, ptName, ptCountry, ptMonthfame, ptButton;

		m_pSLOT[0] = FindKid( ID_CTRLINST_SLOT1 );
		m_pRANK[0] = FindKid( ID_CTRLINST_RANK );
		m_pGUILD[0] = FindKid( ID_CTRLINST_GUILD );
		m_pNAME[0] = FindKid( ID_CTRLINST_NAME);
		m_pCOUNTRY[0] = FindKid( ID_CTRLINST_COUNTRY );
		m_pMONTHFAME[0] = FindKid( ID_CTRLINST_MONTH_FAME );
		m_pBUTTON[0] = (TButton*) FindKid( ID_CTRLINST_BTN_LIST );

		m_pSLOT[0]->GetComponentPos( &ptBase );
		m_pRANK[0]->GetComponentPos( &ptRank );
		m_pGUILD[0]->GetComponentPos( &ptGuild );
		m_pNAME[0]->GetComponentPos( &ptName );
		m_pCOUNTRY[0]->GetComponentPos( &ptCountry );
		m_pMONTHFAME[0]->GetComponentPos( &ptMonthfame );
		m_pBUTTON[0]->GetComponentPos( &ptButton );

		ptRankOffset = ptRank - ptBase;
		ptGuildOffset = ptGuild - ptBase;
		ptNameOffset = ptName - ptBase;
		ptCountryOffset = ptCountry - ptBase;
		ptMonthfameOffset = ptMonthfame - ptBase;
		ptButtonOffset = ptButton - ptBase;

		for( INT i = 1; i < MAX_SLOT_COUNT; ++i )
		{
			m_pSLOT[i] = FindKid( dwBASE_NEW[i] );
			m_pBUTTON[i] = new TButton( this, m_pBUTTON[0]->m_pDESC);
			m_pRANK[i] = new TComponent( this, m_pRANK[0]->m_pDESC);
			m_pGUILD[i] = new TComponent( this, m_pGUILD[0]->m_pDESC);
			m_pNAME[i] = new TComponent( this, m_pNAME[0]->m_pDESC);
			m_pCOUNTRY[i] = new TComponent( this, m_pCOUNTRY[0]->m_pDESC);
			m_pMONTHFAME[i] = new TComponent( this, m_pMONTHFAME[0]->m_pDESC);

			m_pBUTTON[i]->m_id = GetUniqueID();
			m_pRANK[i]->m_id = GetUniqueID();
			m_pGUILD[i]->m_id = GetUniqueID();
			m_pNAME[i]->m_id = GetUniqueID();
			m_pCOUNTRY[i]->m_id = GetUniqueID();
			m_pMONTHFAME[i]->m_id = GetUniqueID();

			AddKid( m_pBUTTON[i] );
			AddKid( m_pRANK[i] );
			AddKid( m_pGUILD[i] );
			AddKid( m_pNAME[i] );
			AddKid( m_pCOUNTRY[i] );
			AddKid( m_pMONTHFAME[i] );

			m_pSLOT[i]->GetComponentPos( &ptBase );

			m_pBUTTON[i]->MoveComponent( ptBase + ptButtonOffset );
			m_pRANK[i]->MoveComponent( ptBase + ptRankOffset );
			m_pGUILD[i]->MoveComponent( ptBase + ptGuildOffset );
			m_pNAME[i]->MoveComponent( ptBase + ptNameOffset );
			m_pCOUNTRY[i]->MoveComponent( ptBase + ptCountryOffset );
			m_pMONTHFAME[i]->MoveComponent( ptBase + ptMonthfameOffset );
		}
	}

	m_bFirstCount = 0;
}

CTCharRankInfoDlg::~CTCharRankInfoDlg(void)
{
	ReleaseData();
}

void CTCharRankInfoDlg::ReleaseData()
{
	TMAPFAMERANK::iterator itFRAMERANK;
	VTFAMERANK::iterator itVTFRAMERANK;

	for ( int i = 0; i < TCONTRY_COUNT; ++i )
	{
		for ( itFRAMERANK = m_mapMONTHFAMERANK[i].begin(); itFRAMERANK != m_mapMONTHFAMERANK[i].end(); itFRAMERANK++ )
			delete (*itFRAMERANK).second;

		m_mapMONTHFAMERANK[i].clear();
	}

	for ( itVTFRAMERANK = m_vtFIRSTGRADEGROUP.begin(); itVTFRAMERANK != m_vtFIRSTGRADEGROUP.end(); itVTFRAMERANK++ )
		delete (*itVTFRAMERANK);

	m_vtFIRSTGRADEGROUP.clear();
	m_vtFAMERANK.clear();
}

void CTCharRankInfoDlg::ResetData( BYTE bTopTabIndex, BYTE bBottonTabIndex )
{
	TMAPFAMERANK::iterator itFRAMERANK;
	
	m_vtFAMERANK.clear();

	if ( bTopTabIndex == TRANKINFO_TAB_TOTAL )
	{
		for( int i = 0; i < TCONTRY_COUNT; ++i )
		{
			for ( int j = 1; j < m_mapMONTHFAMERANK[i].size(); ++j )
			{
				if ( m_mapMONTHFAMERANK[i][j]->dwMonthPoint > 0 )
					m_vtFAMERANK.push_back( m_mapMONTHFAMERANK[i][j] );
			}
		}
		
		sort( m_vtFAMERANK.begin(), m_vtFAMERANK.end(), SortRank );

	}
	else if ( bTopTabIndex == TRANKINFO_TAB_COUNTRY )
	{
		BYTE bCountry = CTClientGame::GetInstance()->GetMainChar()->m_bContryID;

		for ( int i = 1; i < m_mapMONTHFAMERANK[bCountry].size(); ++i )
		{
			if ( m_mapMONTHFAMERANK[bCountry][i]->dwMonthPoint > 0 )
				m_vtFAMERANK.push_back( m_mapMONTHFAMERANK[bCountry][i] );
		}
	}
	else if ( bTopTabIndex == TRANKINFO_TAB_TOURNAMENT )
	{
		for ( int i = 0; i < m_vtFIRSTGRADEGROUP.size(); ++i )
		{
			m_vtFAMERANK.push_back( m_vtFIRSTGRADEGROUP[i] );
		}
	}

	for ( INT i = 0; i < MAX_SLOT_COUNT; ++i )
	{
		if ( m_vtFAMERANK.size() > i )
		{
			m_pRANK[i]->m_strText.Format("%d", i+1);
			m_pNAME[i]->m_strText = m_vtFAMERANK[i]->strName;
			m_pCOUNTRY[i]->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[m_vtFAMERANK[i]->bCountry] );
			m_pGUILD[i]->m_strText = m_vtFAMERANK[i]->strGuild;
			m_pMONTHFAME[i]->m_strText.Format("%d", m_vtFAMERANK[i]->dwMonthPoint );
			m_pBUTTON[i]->EnableComponent( FALSE );
		}
		else
		{
			m_pRANK[i]->m_strText = CString("");
			m_pNAME[i]->m_strText = CString("");
			m_pCOUNTRY[i]->m_strText = CString("");
			m_pGUILD[i]->m_strText = CString("");
			m_pMONTHFAME[i]->m_strText = CString("");
			m_pBUTTON[i]->EnableComponent( FALSE );
		}

		if ( ( bTopTabIndex == TRANKINFO_TAB_TOTAL && i < MAX_COLOR_SLOT_COUNT ) ||
			 ( bTopTabIndex == TRANKINFO_TAB_COUNTRY && i < (m_bFirstCount - 1) )  || 
			 bTopTabIndex == TRANKINFO_TAB_TOURNAMENT )
		{
			CGDIFont *pGDIFont = FindFont( ID_FONT_LOGIN_BROWN );

			if( m_pRANK[i]->GetFont() != pGDIFont )
				m_pRANK[i]->SetFont( pGDIFont, NULL);
			
			if( m_pNAME[i]->GetFont() != pGDIFont )
				m_pNAME[i]->SetFont( pGDIFont, NULL);
			
			if( m_pCOUNTRY[i]->GetFont() != pGDIFont )
				m_pCOUNTRY[i]->SetFont( pGDIFont, NULL);
			
			if( m_pGUILD[i]->GetFont() != pGDIFont )
				m_pGUILD[i]->SetFont( pGDIFont, NULL);
			
			if( m_pMONTHFAME[i]->GetFont() != pGDIFont )
				m_pMONTHFAME[i]->SetFont( pGDIFont, NULL);	
		}
		else
		{
			CGDIFont *pGDIFont = FindFont( ID_FONT_FAME_LIST );

			if( m_pRANK[i]->GetFont() != pGDIFont )
				m_pRANK[i]->SetFont( pGDIFont, NULL);

			if( m_pNAME[i]->GetFont() != pGDIFont )
				m_pNAME[i]->SetFont( pGDIFont, NULL);

			if( m_pCOUNTRY[i]->GetFont() != pGDIFont )
				m_pCOUNTRY[i]->SetFont( pGDIFont, NULL);

			if( m_pGUILD[i]->GetFont() != pGDIFont )
				m_pGUILD[i]->SetFont( pGDIFont, NULL);

			if( m_pMONTHFAME[i]->GetFont() != pGDIFont )
				m_pMONTHFAME[i]->SetFont( pGDIFont, NULL);	
		}

	}

	TButton *pTopTAB[TRANKINFO_TAB_COUNT] = {
		(TButton *) FindKid(ID_CTRLINST_TAB_1),
		(TButton *) FindKid(ID_CTRLINST_TAB_2),
		(TButton *) FindKid(ID_CTRLINST_TAB_3),
	};

	for ( BYTE i = 0; i < TRANKINFO_TAB_COUNT; ++i )
	{
		pTopTAB[i]->Select( i == bTopTabIndex ? TRUE : FALSE );

		CGDIFont *pGDIFont = FindFont(pTopTAB[i]->IsStateDown() ? ID_FONT_LOGIN : ID_FONT_DISABLE);

		if( pTopTAB[i]->GetFont() != pGDIFont )
			pTopTAB[i]->SetFont( pGDIFont, NULL);
	}

}


BOOL CTCharRankInfoDlg::SortRank( CTFameRank* pLeft, CTFameRank* pRight )
{
	if ( pLeft->dwMonthPoint > pRight->dwMonthPoint )
	{
		return TRUE;
	}
	else if ( pLeft->dwMonthPoint == pRight->dwMonthPoint )
	{
		if ( pLeft->wWin > pRight->wWin )
		{
			return TRUE;
		}
		else if ( pLeft->wWin == pRight->wWin )
		{
			if( (pLeft->wWin+pLeft->wLose) > (pRight->wWin+pRight->wLose) )
			{
				return TRUE;
			}
			else if ( (pLeft->wWin+pLeft->wLose) == (pRight->wWin+pRight->wLose) )
			{
				return ( pLeft->strName < pRight->strName );
			}
			else
			{
				return FALSE;
			}
		}
		else
		{
			return FALSE;
		}
	}
	else
	{
		return FALSE;
	}
}




void CTCharRankInfoDlg::OnLButtonUp(UINT nFlags, CPoint pt)
{
	GetComponentPos( &CTCharInfoDlg::m_ptPOS);
	CTClientUIBase::OnLButtonUp( nFlags, pt );
}

void CTCharRankInfoDlg::ShowComponent( BOOL bVisible )
{
	MoveComponent( CTCharInfoDlg::m_ptPOS );

	if( bVisible )
	{
		CTClientUIBase* pDLG = CTClientGame::GetInstance()->GetFrame( TFRAME_CHAR_INFO );
		pDLG->ShowComponent( FALSE );

		pDLG = CTClientGame::GetInstance()->GetFrame( TFRAME_CHAR_PVP_INFO );
		pDLG->ShowComponent( FALSE );
	}

	CTClientUIBase::ShowComponent( bVisible );
}

ITDetailInfoPtr CTCharRankInfoDlg::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo;

	for ( INT i = 0; i < MAX_SLOT_COUNT ; ++i )
	{
		if ( m_pBUTTON[i]->HitTest( point ) && m_vtFAMERANK.size() > i )	
		{
			CRect rc;
			GetComponentRect(&rc);

			return CTDetailInfoManager::NewFameInst(
				m_vtFAMERANK[i]->strName,
				m_vtFAMERANK[i]->dwTotalPoint,
				m_vtFAMERANK[i]->dwMonthPoint,
				m_vtFAMERANK[i]->wWin,
				m_vtFAMERANK[i]->wLose,
				m_vtFAMERANK[i]->bLevel,
				m_vtFAMERANK[i]->bClass,
				rc );
		}
	}

	return pInfo;
}

void CTCharRankInfoDlg::ResetTabIndex( BYTE bTabIndex )
{
	TButton *pBottonTAB[TINFO_COUNT] = {
		(TButton *) FindKid(ID_CTRLINST_BASIC),
			(TButton *) FindKid(ID_CTRLINST_MORE),
			(TButton *) FindKid(ID_CTRLINST_WAR),
			(TButton *) FindKid(ID_CTRLINST_PVP),
			(TButton *) FindKid(ID_CTRLINST_BTN_RANK),
	};

	for( BYTE i = 0; i < TINFO_COUNT; ++i )
	{
		pBottonTAB[i]->Select( i == bTabIndex ? TRUE : FALSE);

		/*CGDIFont *pGDIFont = FindFont(pBottonTAB[i]->IsStateDown() ? ID_FONT_LOGIN : ID_FONT_DISABLE);

		if( pBottonTAB[i]->GetFont() != pGDIFont )
			pBottonTAB[i]->SetFont( pGDIFont, NULL);*/
	}
}

