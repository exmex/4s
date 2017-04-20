#include "stdafx.h"
#include "TRpsRewardList.h"

#define TRPS_GAME_MAX 15

CTRpsRewardList::CTRpsRewardList( TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase(pParent, pDesc)
{
	DWORD dwID[][7] = 
	{
		{ ID_CTRLINST_00_ITEM0,
		ID_CTRLINST_00_ITEM1,
		ID_CTRLINST_00_ITEM_TEXT0,
		ID_CTRLINST_00_ITEM_TEXT1,
		ID_CTRLINST_00_GOLD,
		ID_CTRLINST_00_SILVER,
		ID_CTRLINST_00_COOPER},

		{ ID_CTRLINST_01_ITEM0,
		ID_CTRLINST_01_ITEM1,
		ID_CTRLINST_01_ITEM_TEXT0,
		ID_CTRLINST_01_ITEM_TEXT1,
		ID_CTRLINST_01_GOLD,
		ID_CTRLINST_01_SILVER,
		ID_CTRLINST_01_COOPER},

		{ ID_CTRLINST_02_ITEM0,
		ID_CTRLINST_02_ITEM1,
		ID_CTRLINST_02_ITEM_TEXT0,
		ID_CTRLINST_02_ITEM_TEXT1,
		ID_CTRLINST_02_GOLD,
		ID_CTRLINST_02_SILVER,
		ID_CTRLINST_02_COOPER},

		{ ID_CTRLINST_10_ITEM0,
		ID_CTRLINST_10_ITEM1,
		ID_CTRLINST_10_ITEM_TEXT0,
		ID_CTRLINST_10_ITEM_TEXT1,
		ID_CTRLINST_10_GOLD,
		ID_CTRLINST_10_SILVER,
		ID_CTRLINST_10_COOPER},

		{ ID_CTRLINST_11_ITEM0,
		ID_CTRLINST_11_ITEM1,
		ID_CTRLINST_11_ITEM_TEXT0,
		ID_CTRLINST_11_ITEM_TEXT1,
		ID_CTRLINST_11_GOLD,
		ID_CTRLINST_11_SILVER,
		ID_CTRLINST_11_COOPER},

		{ ID_CTRLINST_12_ITEM0,
		ID_CTRLINST_12_ITEM1,
		ID_CTRLINST_12_ITEM_TEXT0,
		ID_CTRLINST_12_ITEM_TEXT1,
		ID_CTRLINST_12_GOLD,
		ID_CTRLINST_12_SILVER,
		ID_CTRLINST_12_COOPER},

		{ ID_CTRLINST_20_ITEM0,
		ID_CTRLINST_20_ITEM1,
		ID_CTRLINST_20_ITEM_TEXT0,
		ID_CTRLINST_20_ITEM_TEXT1,
		ID_CTRLINST_20_GOLD,
		ID_CTRLINST_20_SILVER,
		ID_CTRLINST_20_COOPER},

		{ ID_CTRLINST_21_ITEM0,
		ID_CTRLINST_21_ITEM1,
		ID_CTRLINST_21_ITEM_TEXT0,
		ID_CTRLINST_21_ITEM_TEXT1,
		ID_CTRLINST_21_GOLD,
		ID_CTRLINST_21_SILVER,
		ID_CTRLINST_21_COOPER},

		{ ID_CTRLINST_22_ITEM0,
		ID_CTRLINST_22_ITEM1,
		ID_CTRLINST_22_ITEM_TEXT0,
		ID_CTRLINST_22_ITEM_TEXT1,
		ID_CTRLINST_22_GOLD,
		ID_CTRLINST_22_SILVER,
		ID_CTRLINST_22_COOPER}
	};

	for(int i=0 ; i < 9 ; ++i )
	{
		m_vComp[ i ].m_pITEM0 = (TImageList*) FindKid( dwID[ i ][0] );
		m_vComp[ i ].m_pITEM1 = (TImageList*) FindKid( dwID[ i ][1] );
		m_vComp[ i ].m_pTEXT0 = FindKid( dwID[ i ][2] );
		m_vComp[ i ].m_pTEXT1 = FindKid( dwID[ i ][3] );
		m_vComp[ i ].m_pGOLD = FindKid( dwID[ i ][4] );
		m_vComp[ i ].m_pSILVER = FindKid( dwID[ i ][5] );
		m_vComp[ i ].m_pCOOPER = FindKid( dwID[ i ][6] );
		m_vComp[ i ].m_pGOLD->SetTextAlign( ALIGN_CENTER );
		m_vComp[ i ].m_pSILVER->SetTextAlign( ALIGN_CENTER );
		m_vComp[ i ].m_pCOOPER->SetTextAlign( ALIGN_CENTER );
	}

	m_pSCROLL = (TScroll*) FindKid( ID_CTRLINST_SCROLL );

	m_pCOL[0] = FindKid( ID_CTRLINST_COL0 );
	m_pCOL[1] = FindKid( ID_CTRLINST_COL1 );
	m_pCOL[2] = FindKid( ID_CTRLINST_COL2 );

	m_pROW[0] = FindKid( ID_CTRLINST_ROW0 );
	m_pROW[1] = FindKid( ID_CTRLINST_ROW1 );
	m_pROW[2] = FindKid( ID_CTRLINST_ROW2 );

	m_nListTop = 0;
	m_pSCROLL->SetScrollPos( 12, 0 );

	m_pCOL[0]->m_strText = CTChart::LoadString(TSTR_RPSGAME_TYPE_1);
	m_pCOL[1]->m_strText = CTChart::LoadString(TSTR_RPSGAME_TYPE_2);
	m_pCOL[2]->m_strText = CTChart::LoadString(TSTR_RPSGAME_TYPE_3);
}

CTRpsRewardList::~CTRpsRewardList()
{
}

void CTRpsRewardList::UpdateList( int top )
{
	top = TRPS_GAME_MAX - top;

	int n=0;
	for( int win=top ; win > top-3 ; --win )
	{
		UpdateElement( CTChart::FindTRPS( 1, win ), n++ );
		UpdateElement( CTChart::FindTRPS( 2, win ), n++ );
		UpdateElement( CTChart::FindTRPS( 3, win ), n++ );
	}

	m_pROW[0]->m_strText = CTChart::Format( TSTR_RPSGAME_LIST_WIN, top);
	m_pROW[1]->m_strText = CTChart::Format( TSTR_RPSGAME_LIST_WIN, top-1);
	m_pROW[2]->m_strText = CTChart::Format( TSTR_RPSGAME_LIST_WIN, top-2);
}

void CTRpsRewardList::UpdateElement( LPTRPS pTRPS, int index)
{
	if( NULL == pTRPS )
		return ;

	if( 0 != pTRPS->wRewardItem1 )
	{
		LPTITEM pTITEM = CTChart::FindTITEMTEMP( pTRPS->wRewardItem1 );
		if( pTITEM )
		{
			LPTITEMVISUAL pTVisual = CTChart::FindTITEMVISUAL( pTITEM->m_wVisual[0] );
			m_vComp[ index ].m_pITEM0->SetCurImage( pTVisual->m_wIcon );	
			m_vComp[ index ].m_pTEXT0->m_strText = pTITEM->m_strNAME;

			if( pTRPS->bRewardCount1 != 0 )
			{
				CString strFMT;
				strFMT.Format( "%d", pTRPS->bRewardCount1 );
				m_vComp[ index ].m_pITEM0->m_strText = strFMT;
			}
		}
	}
	else
	{
		m_vComp[ index ].m_pITEM0->SetCurImage( 0 );
		m_vComp[ index ].m_pITEM0->m_strText.Empty();
		m_vComp[ index ].m_pTEXT0->m_strText.Empty();
	}

	if( 0 != pTRPS->wRewardItem2 )
	{
		LPTITEM pTITEM = CTChart::FindTITEMTEMP( pTRPS->wRewardItem2 );
		if( pTITEM )
		{
			LPTITEMVISUAL pTVisual = CTChart::FindTITEMVISUAL( pTITEM->m_wVisual[0] );
			m_vComp[ index ].m_pITEM1->SetCurImage( pTVisual->m_wIcon );	
			m_vComp[ index ].m_pTEXT1->m_strText = pTITEM->m_strNAME;

			if( pTRPS->bRewardCount2 != 0 )
			{
				CString strFMT;
				strFMT.Format( "%d", pTRPS->bRewardCount2 );
				m_vComp[ index ].m_pITEM1->m_strText = strFMT;
			}
		}
	}
	else
	{
		m_vComp[ index ].m_pITEM1->SetCurImage( 0 );
		m_vComp[ index ].m_pITEM1->m_strText.Empty();
		m_vComp[ index ].m_pTEXT1->m_strText.Empty();
	}

	if( 0 != pTRPS->dwRewardMoney )
	{
		DWORD dwPRICE = pTRPS->dwRewardMoney;

		DWORD dwGOLD = dwPRICE / TRUNE_DIVIDER;
		if( dwGOLD != 0 )
			m_vComp[ index ].m_pGOLD->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwGOLD);
		else
			m_vComp[ index ].m_pGOLD->m_strText.Empty();

		DWORD dwSILVER = (dwPRICE % TRUNE_DIVIDER) / TLUNA_DIVIDER;
		if( dwSILVER != 0 )
			m_vComp[ index ].m_pSILVER->m_strText = CTChart::Format( TSTR_FMT_NUMBER, dwSILVER);
		else
			m_vComp[ index ].m_pSILVER->m_strText.Empty();

		m_vComp[ index ].m_pCOOPER->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			dwPRICE % TLUNA_DIVIDER );
	}
	else
	{
		m_vComp[ index ].m_pGOLD->m_strText.Empty();
		m_vComp[ index ].m_pSILVER->m_strText.Empty();
		m_vComp[ index ].m_pCOOPER->m_strText.Empty();
	}
}

void CTRpsRewardList::ShowComponent(BOOL bVisible)
{
	if( bVisible )
		UpdateList( m_nListTop );

	CTClientUIBase::ShowComponent(bVisible);
}

BOOL CTRpsRewardList::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	if( !CanProcess() || !HitTest(pt) )
		return FALSE;

	int range, pos;
	m_pSCROLL->GetScrollPos( range, pos );

	m_nListTop += zDelta > 0 ? -1 : +1;

	if( m_nListTop < 0 )
		m_nListTop = 0;

	if( m_nListTop > range )
		m_nListTop = range;

	m_pSCROLL->SetScrollPos( range, m_nListTop );
	UpdateList( m_nListTop );
	return TRUE;
}

void CTRpsRewardList::OnNotify(DWORD from, WORD msg, LPVOID param)
{
	switch( msg )
	{
	case TNM_LINEUP:
	case TNM_LINEDOWN:
	case TNM_VSCROLL:
		{
			if( m_pSCROLL && m_pSCROLL->IsTypeOf( TCML_TYPE_SCROLL ) )
			{
				int nRange;
				int nPos;
				m_pSCROLL->GetScrollPos( nRange, nPos);
				m_nListTop = nRange ? nPos : 0;
				UpdateList( m_nListTop );
			}
		}
		break;
	}
	CTClientUIBase::OnNotify( from, msg, param );
}