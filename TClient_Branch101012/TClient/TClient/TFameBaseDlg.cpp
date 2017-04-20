#include "Stdafx.h"
#include "TFameBaseDlg.h"
#include "TFameRank.h"
#include "TFameListDlg.h"
#include "TClientGame.h"

CString GetFameTitle( BYTE bType, DWORD bRank );

CTFameBaseDlg::CTFameBaseDlg( TComponent *pParent, LP_FRAMEDESC pDesc, TCMLParser *pParser )
: CTClientUIBase(pParent, pDesc),
m_pWarLord(NULL),
m_bType(FRT_HERO)
{
	CPoint ptTitleOffset, ptGuildOffset, ptNameOffset, ptMonthOffset, ptTotalOffset, ptButtonOffset;
	TButton* pHeroButton;
	m_pWarLord = new CTFameRank;

	m_pDATE = FindKid( ID_CTRLINST_DATE );
	m_pWARLORD_NAME = FindKid( ID_CTRLINST_WARLORD_NAME );
	m_pWARLORD_FACE = (TImageList *) FindKid( ID_CTRLINST_WARLORD_FACE );
	m_pWARLORD_CLASS = (TImageList *) FindKid( ID_CTRLINST_WARLORD_CLASS );
	m_pWARLORD_COUNTRY = (TImageList *) FindKid( ID_CTRLINST_WARLORD_COUNTRY );
	m_pWARLORD_RANK = FindKid( ID_CTRLINST_WARLORD_RANK );
	m_pWARLORD_SAY = FindKid( ID_CTRLINST_WARLORD_SAY );
	m_pWARLORD_FAME = FindKid( ID_CTRLINST_WARLORD_FAME );
	m_pWARLORD_BATTLE = FindKid( ID_CTRLINST_WARLORD_BATTLE );
	m_pWARLORD_LEVEL = FindKid( ID_CTRLINST_WARLORD_LEVEL );
	m_pWARLORD_TITLE = FindKid( ID_CTRLINST_WARLORD_TITLE );
	m_pLIST = (TList*) FindKid( ID_CTRLINST_LIST );
	m_pLIST->m_nWheelMoveValue = 0;
	pHeroButton = (TButton *) FindKid( ID_CTRLINST_HERO );
	pHeroButton->Select( TRUE );

	m_pFAME_FIRST_LIST = new CTFameListDlg(this, pParser->FindFrameTemplate(ID_FRAME_FAME_FIRST_LIST) );
	AddKid(m_pFAME_FIRST_LIST);
	CPoint pt(0,0);
	TComponent* pPOS = FindKid( ID_CTRLINST_POS );
	if( pPOS )
		pPOS->GetComponentPos(&pt);
	m_pFAME_FIRST_LIST->MoveComponent(pt);
	m_pFAME_FIRST_LIST->ShowComponent(FALSE);

	m_pLIST->LastItemToFront();
	m_pLIST->SetColumnAlign( 0, ALIGN_CENTER );
	m_pLIST->SetColumnAlign( 1, ALIGN_CENTER );
	m_pLIST->SetColumnAlign( 2, ALIGN_CENTER );
	m_pLIST->SetColumnAlign( 3, ALIGN_CENTER );
	m_pLIST->SetColumnAlign( 4, ALIGN_CENTER );

	SYSTEMTIME SystemTime;
	::GetSystemTime(&SystemTime);
	m_wYear = SystemTime.wYear;
	int nMonth = SystemTime.wMonth;
	--nMonth;
	if( nMonth <= 0 )
	{
		--m_wYear;
		nMonth = 12;
	}
	m_bMonth = (BYTE) nMonth;

}

CTFameBaseDlg::~CTFameBaseDlg()
{
	delete m_pWarLord;
	ClearRank();
}

void CTFameBaseDlg::ClearRank()
{
	TMAPFAMERANK::iterator it, end;
	it = m_mapFAMERANK.begin();
	end = m_mapFAMERANK.end();
	for(; it != end ; ++it )
		delete (*it).second;
	m_mapFAMERANK.clear();

	if( m_pWARLORD_SAY )
		m_pWARLORD_SAY->m_strText.Empty();
}

void CTFameBaseDlg::ResetRankInfo()
{
	CString str;
	m_pDATE->m_strText = CTChart::Format( TSTR_FAMERANK_MONTH, m_wYear, m_bMonth );
	m_pLIST->RemoveAll();

	if( m_bType == FRT_GOD || m_bType == FRT_GODDESS )
	{
		for( INT i=0 ; i < 7 ; ++i )
		{
			TMAPFAMERANK::iterator it = m_mapFAMERANK.find( i );
			if( it == m_mapFAMERANK.end() )
				continue;

			CTFameRank* pRank = it->second;

			if( i == 0 )
				OnSelectRank( pRank, i, m_bType );

			if( !pRank->strName.IsEmpty() )
			{
				int nLine = m_pLIST->AddString( GetFameTitle(m_bType, i ), 0 );
				m_pLIST->SetItemData( nLine, 0, (DWORD)pRank );
				m_pLIST->SetItemString( nLine, 1, pRank->strName ); 
				m_pLIST->SetItemString( nLine, 2, pRank->strGuild ); 

				CString strFMT;
				if( pRank->dwMonthRank != 0 )
				{
					strFMT.Format( "%d", pRank->dwMonthRank);
					m_pLIST->SetItemString( nLine, 3, strFMT ); 
				}
				else
					m_pLIST->SetItemString( nLine, 3, "-" ); 

				if( pRank->dwTotalRank != 0 )
				{
					strFMT.Format( "%d", pRank->dwTotalRank);
					m_pLIST->SetItemString( nLine, 4, strFMT ); 
				}
				else
					m_pLIST->SetItemString( nLine, 4, "-" );
			}
		}
	}
	else
	{
		BYTE bSelectRank = FALSE;

		for( BYTE i = 0; i < FAMERANKCOUNT; ++i )
		{
			TMAPFAMERANK::iterator finder = m_mapFAMERANK.find( i );
			if( finder != m_mapFAMERANK.end() )
			{
				CTFameRank* pRank = finder->second;

				if( !bSelectRank && (pRank->dwCharID != 0) )
				{
					OnSelectRank( pRank, pRank->dwMonthRank, m_bType );
					bSelectRank = TRUE;
				}

				if( !pRank->strName.IsEmpty() )
				{
					int nLine = m_pLIST->AddString( GetFameTitle(m_bType, pRank->dwMonthRank ), 0 );
					m_pLIST->SetItemData( nLine, 0, (DWORD)pRank );
					m_pLIST->SetItemString( nLine, 1, pRank->strName ); 
					m_pLIST->SetItemString( nLine, 2, pRank->strGuild ); 

					if ( i != 0 )
					{
						CString strFMT;
						strFMT.Format("%d", pRank->dwMonthRank);
						m_pLIST->SetItemString( nLine, 3, strFMT ); 
						strFMT.Format("-");
						m_pLIST->SetItemString( nLine, 4, strFMT ); 
					}
					else
					{
						CString strFMT;
						strFMT.Format("1");
						m_pLIST->SetItemString( nLine, 4, strFMT ); 
						strFMT.Format("-");
						m_pLIST->SetItemString( nLine, 3, strFMT ); 
					}
				}
			}
		}	

		if ( !bSelectRank )
			OnSelectRank(NULL, 0, 0);
	}
}

void CTFameBaseDlg::ResetTab()
{
	TButton *pTAB[TFRAMEBASE_COUNT] = {
		(TButton *) FindKid(ID_CTRLINST_HERO), // FRT_HERO
		(TButton *) FindKid(ID_CTRLINST_GOD), // FRT_GOD
		(TButton *) FindKid(ID_CTRLINST_KNIGHT), // FRT_GODDESS
	};

	for( BYTE i=0; i<TFRAMEBASE_COUNT; i++)
	{
		if( i == m_bType )
		{
			pTAB[i]->Select(TRUE);
			pTAB[i]->SetTextClr(D3DCOLOR_XRGB(210,210,210));
		}
		else
		{
			pTAB[i]->Select(FALSE);
			pTAB[i]->SetTextClr(D3DCOLOR_XRGB(60,60,60));
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

void CTFameBaseDlg::ShowComponent(BOOL bVisible)
{
	TComponent::ShowComponent( bVisible );

	ResetTab();

	if( bVisible )
	{
		switch( m_bType )
		{
		case FRT_HERO:
		case FRT_GOD:
		case FRT_GODDESS:
			m_pFAME_FIRST_LIST->ShowComponent(FALSE);
			m_pLIST->ShowComponent(TRUE);
			break;
		case FRT_1ST:
			m_pFAME_FIRST_LIST->ShowComponent(TRUE);
			m_pLIST->ShowComponent(FALSE);
			break;
		};
	}
}

void CTFameBaseDlg::OnEnable()
{
	ResetTab();

	CTClientSession* pSESSION = CTClientGame::GetInstance()->GetSession();
	switch( m_bType )
	{
	case FRT_HERO:
	case FRT_GOD:
	case FRT_GODDESS:
		{
			m_pFAME_FIRST_LIST->ShowComponent(FALSE);
			m_pLIST->ShowComponent(TRUE);
			
			if( pSESSION )
				pSESSION->SendCS_FAMERANKLIST_REQ( m_bType, m_bMonth );
		}
		break;
	/*case FRT_1ST:
		{
			m_pFAME_FIRST_LIST->ShowComponent(TRUE);
			m_pLIST->ShowComponent(FALSE);

			if( pSESSION )
				pSESSION->SendCS_FIRSTGRADEGROUP_REQ();
		}
		break;*/
	};
}

void CTFameBaseDlg::OnNotify(DWORD from, WORD msg, LPVOID param)
{
	if( msg == TNM_SEL_CHANGE )
	{
		if( from == m_pLIST->m_id )
		{
			INT nCur = m_pLIST->GetCurSelIndex();

			if( nCur != -1 )
			{
				CTFameRank* pRank = (CTFameRank*) m_pLIST->GetItemData( nCur, 0 );
				if( m_bType == FRT_GOD || m_bType == FRT_GODDESS )
					OnSelectRank( pRank, nCur, m_bType );
				else
					OnSelectRank( pRank, pRank->dwMonthRank, m_bType );
			}
			else
				OnSelectRank( NULL, 0, m_bType );
		}
	}

	CTClientUIBase::OnNotify( from, msg, param );
}

void CTFameBaseDlg::OnSelectRank(CTFameRank* pRank, DWORD dwMonthRank, BYTE bType )
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( pRank && !pRank->strName.IsEmpty() )
	{
		DWORD dwMonthBattle = 0;
		DWORD dwTotalBattle = 0;

		if ( pRank->wWin > 0 )
			dwMonthBattle = (DWORD)(((FLOAT)pRank->wWin / (FLOAT)(pRank->wWin + pRank->wLose)) * 100);
		if( pRank->dwTotalWin > 0 )
			dwTotalBattle = (DWORD)(((FLOAT)pRank->dwTotalWin / (FLOAT)(pRank->dwTotalWin + pRank->dwTotalLose)) * 100);

		m_pWARLORD_NAME->m_strText = pRank->strName;
		m_pWARLORD_TITLE->m_strText = GetFameTitle(m_bType, dwMonthRank);

		if( bType == FRT_GOD || bType == FRT_GODDESS )
		{
			m_pWARLORD_RANK->m_strText = CTChart::Format( TSTR_FAME_WARLORD_RANK, pRank->dwMonthRank, pRank->dwTotalRank );
		}
		else
		{
			if( pRank->dwMonthRank != 0 )
				m_pWARLORD_RANK->m_strText = CTChart::Format( TSTR_FAME_WARLORD_RANK, pRank->dwMonthRank, 0 );
			else
				m_pWARLORD_RANK->m_strText = CTChart::Format( TSTR_FAME_WARLORD_RANK, 0, 1 );
		}

		m_pWARLORD_FAME->m_strText = CTChart::Format( TSTR_FAME_WARLORD_FAME, pRank->dwMonthPoint, pRank->dwTotalPoint );
		m_pWARLORD_BATTLE->m_strText = CTChart::Format( TSTR_FAME_WARLORD_BATTLE, dwMonthBattle, dwTotalBattle );

		m_pWARLORD_LEVEL->m_strText.Format( "%d", pRank->bLevel );

		if( pRank->dwMonthRank == 0 && m_bType == FRT_HERO )
			m_pWARLORD_SAY->m_strText = pRank->strSay;

		m_pWARLORD_FACE->SetCurImage( pGame->GetFACE(
			pRank->bRace,
			pRank->bSex,
			pRank->bFace,
			pRank->bHair));
		m_pWARLORD_COUNTRY->SetCurImage( pRank->bCountry );
		m_pWARLORD_CLASS->SetCurImage( pRank->bClass );
	}
	else
	{
		m_pWARLORD_NAME->m_strText.Empty();
		m_pWARLORD_TITLE->m_strText.Empty();
		m_pWARLORD_RANK->m_strText.Empty();
		m_pWARLORD_FAME->m_strText.Empty();
		m_pWARLORD_BATTLE->m_strText.Empty();

		m_pWARLORD_LEVEL->m_strText.Empty();
		
		m_pWARLORD_FACE->SetCurImage( TIMAGELIST_FACE_NULL );
		m_pWARLORD_COUNTRY->SetCurImage( TIMAGELIST_COUNTRY_NULL );
		m_pWARLORD_CLASS->SetCurImage( TIMAGELIST_CLASS_NULL );
	}
}