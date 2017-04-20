#include "stdafx.h"
#include "TTournamentGage.h"
#include "TClientGame.h"
#include "TTournamentMode.h"
#include "TTournamentSelectTarget.h"

#ifdef ADD_TOURNAMENT

CTTournamentGage::CTTournamentGage( TComponent *pParent, LP_FRAMEDESC pDesc, TCMLParser *pParser, CTClientGame *pMainGame)
:CTClientUIBase( pParent, pDesc),
m_bShowPlayerInfo(TRUE)
{
	static DWORD dwTPOS[] =
	{
		ID_CTRLINST_LEFT,
		ID_CTRLINST_RIGHT
	};

	static DWORD dwBACK[]=
	{
		ID_CTRLINST_BACK1,
		ID_CTRLINST_BACK2,
		ID_CTRLINST_BACK3,
		ID_CTRLINST_BACK4,
		ID_CTRLINST_BACK5,
		ID_CTRLINST_BACK6,
		ID_CTRLINST_BACK7
	};

	static DWORD dwBASE[]=
	{
		ID_CTRLINST_BASE_1,
		ID_CTRLINST_BASE_2,
		ID_CTRLINST_BASE_3,
		ID_CTRLINST_BASE_4,
		ID_CTRLINST_BASE_5,
		ID_CTRLINST_BASE_6,
		ID_CTRLINST_BASE_7
	};

	static DWORD dwLVBASE[]=
	{
		ID_CTRLINST_LV_BASE1,
		ID_CTRLINST_LV_BASE2,
		ID_CTRLINST_LV_BASE3,
		ID_CTRLINST_LV_BASE4,
		ID_CTRLINST_LV_BASE5,
		ID_CTRLINST_LV_BASE6,
		ID_CTRLINST_LV_BASE7
	};

	static DWORD dwFACE[]=
	{
		ID_CTRLINST_PC_FACE1,
		ID_CTRLINST_PC_FACE2,
		ID_CTRLINST_PC_FACE3,
		ID_CTRLINST_PC_FACE4,
		ID_CTRLINST_PC_FACE5,
		ID_CTRLINST_PC_FACE6,
		ID_CTRLINST_PC_FACE7
	};

	static DWORD dwHPGAUGE[] =
	{
		ID_CTRLINST_HP1,
		ID_CTRLINST_HP2,
		ID_CTRLINST_HP3,
		ID_CTRLINST_HP4,
		ID_CTRLINST_HP5,
		ID_CTRLINST_HP6,
		ID_CTRLINST_HP7
	};

	static DWORD dwMPGAUGE[] =
	{
		ID_CTRLINST_MP1,
		ID_CTRLINST_MP2,
		ID_CTRLINST_MP3,
		ID_CTRLINST_MP4,
		ID_CTRLINST_MP5,
		ID_CTRLINST_MP6,
		ID_CTRLINST_MP7
	};

	static DWORD dwNAME[] =
	{
		ID_CTRLINST_TXT_NAME1,
		ID_CTRLINST_TXT_NAME2,
		ID_CTRLINST_TXT_NAME3,
		ID_CTRLINST_TXT_NAME4,
		ID_CTRLINST_TXT_NAME5,
		ID_CTRLINST_TXT_NAME6,
		ID_CTRLINST_TXT_NAME7
	};

	static DWORD dwHPTEXT[] =
	{
		ID_CTRLINST_TXT_HP1,
		ID_CTRLINST_TXT_HP2,
		ID_CTRLINST_TXT_HP3,
		ID_CTRLINST_TXT_HP4,
		ID_CTRLINST_TXT_HP5,
		ID_CTRLINST_TXT_HP6,
		ID_CTRLINST_TXT_HP7
	};

	static DWORD dwCLASSTEXT[] =
	{
#ifdef MODIFY_TG
		ID_CTRLINST_TXT_CLASS1,
		ID_CTRLINST_TXT_CLASS2,
		ID_CTRLINST_TXT_CLASS3,
		ID_CTRLINST_TXT_CLASS4,
		ID_CTRLINST_TXT_CLASS5,
		ID_CTRLINST_TXT_CLASS6,
		ID_CTRLINST_TXT_CLASS7
#else
		0,
		0,
		0,
		0,
		0,
		0,
		0
#endif
	};

	static DWORD dwLEVEL[] =
	{
		ID_CTRLINST_TXT_LEVEL1,
		ID_CTRLINST_TXT_LEVEL2,
		ID_CTRLINST_TXT_LEVEL3,
		ID_CTRLINST_TXT_LEVEL4,
		ID_CTRLINST_TXT_LEVEL5,
		ID_CTRLINST_TXT_LEVEL6,
		ID_CTRLINST_TXT_LEVEL7
	};

	static DWORD dwMAINTAIN[] =
	{
		ID_CTRLINST_MAINTAIN1,
		ID_CTRLINST_MAINTAIN2,
		ID_CTRLINST_MAINTAIN3,
		ID_CTRLINST_MAINTAIN4,
		ID_CTRLINST_MAINTAIN5,
		ID_CTRLINST_MAINTAIN6,
		ID_CTRLINST_MAINTAIN7
	};

	for( int i=0 ; i < 2 ; ++i )
	{
		CPoint point( 0, 0 );

		LP_FRAMEDESC pGAUGE = pParser->FindFrameTemplate( ID_FRAME_TOURNAMENT_PARTY );
		TComponent *pTPOS = FindKid( dwTPOS[i] );
		CTClientUIBase *pTFRAME = new CTClientUIBase( this, pGAUGE );
		pTFRAME->m_id = GetUniqueID();
		m_pPARTY_GAUGE[i] = pTFRAME;
		pTPOS->GetComponentPos(&point);
		AddKid(pTFRAME);

		pTFRAME->MoveComponent(point);

		for( int j=0 ; j < 7 ; ++j )
		{
			m_pBACK[ i ][ j ] = pTFRAME->FindKid( dwBACK[j] );
			m_pBASE[ i ][ j ] = pTFRAME->FindKid( dwBASE[j] );
			m_pBASE[ i ][ j ]->m_bHitTestType = HITTEST_TYPE_RECT;
			m_pLVBASE[ i ][ j ] = pTFRAME->FindKid( dwLVBASE[j] );
			m_pFACE[ i ][ j ] = (TImageList*) pTFRAME->FindKid(  dwFACE[j] );
			m_pHP[ i ][ j ] = (TGauge*) pTFRAME->FindKid( dwHPGAUGE[j] );
			m_pMP[ i ][ j ] = (TGauge*) pTFRAME->FindKid( dwMPGAUGE[j] );
			m_pHPText[ i ][ j ] = pTFRAME->FindKid( dwHPTEXT[j] );
			m_pCLASSText[ i ][ j ] = pTFRAME->FindKid( dwCLASSTEXT[j] );
			m_pNAME[ i ][ j ] = pTFRAME->FindKid( dwNAME[j] );
			m_pLEVEL[ i ][ j ] = pTFRAME->FindKid( dwLEVEL[j] );
			TComponent* pPannel = pTFRAME->FindKid( dwMAINTAIN[j] );

			m_pHP[ i ][ j ]->m_style = TGS_GROW_RIGHT;
			m_pMP[ i ][ j ]->m_style = TGS_GROW_RIGHT;

			m_pMAINTAIN[ i ][ j ] = new CTMaintainPannel( pTFRAME, pParser->FindFrameTemplate( ID_FRAME_MAINTAIN1R ) );
			m_pMAINTAIN[ i ][ j ]->m_id = pPannel->m_id;

			CPoint point( 0, 0 );
            pPannel->GetComponentPos(&point);
			pTFRAME->RemoveKid(pPannel);
			delete pPannel;
			pTFRAME->AddKid( m_pMAINTAIN[ i ][ j ] );

			m_pMAINTAIN[ i ][ j ]->MoveComponent(point);
			m_pMAINTAIN[ i ][ j ]->ShowComponent(TRUE);
			m_pMAINTAIN[ i ][ j ]->EnableFloat(FALSE);
		}
	}

	{
		m_pTournamentInfo = new CTTournamentMode(
			this,
			pParser->FindFrameTemplate(ID_FRAME_TOURNAMENT_MODE),
			CTTournamentMode::TM_WATCHING );

		AddKid(m_pTournamentInfo);
		m_pTournamentInfo->ShowComponent(TRUE);
		m_pTournamentInfo->EnableFloat(TRUE);
		TComponent *pTPOS = FindKid( ID_CTRLINST_POS_MODE );
		CPoint point;
		pTPOS->GetComponentPos(&point);
		m_pTournamentInfo->MoveComponent(point);
	}
}

CTTournamentGage::~CTTournamentGage()
{
}

BOOL CTTournamentGage::CanWithItemUI()
{
	return TRUE;
}

void CTTournamentGage::ResetPosition()
{
	CTClientUIBase::ResetPosition();

	m_pTournamentInfo->ResetPosition();
	m_pPARTY_GAUGE[0]->ResetPosition();
	m_pPARTY_GAUGE[1]->ResetPosition();
}

void CTTournamentGage::DefaultPosition( CPoint* vBASIS, BOOL bRestore )
{
	DWORD vCOMP[3][2] =
	{
		{ ID_FRAME_TOURNAMENT_MODE, TBASISPOINT_CENTER_BOTTOM },
		{ m_pPARTY_GAUGE[0]->m_id, TBASISPOINT_LEFT_TOP },
		{ m_pPARTY_GAUGE[1]->m_id, TBASISPOINT_RIGHT_TOP },
	};

	static CPoint vCOMP_OFFSET[3];
	static BYTE vCOMP_BASIS[3];

	if( bRestore )
	{
		for( INT i=0 ; i < 3 ; ++i )
		{
			CTClientUIBase* pCOMP = (CTClientUIBase*) FindKid( vCOMP[ i ][0] );
			if( pCOMP )
			{
				pCOMP->m_vCompOffset = vCOMP_OFFSET[i];
				pCOMP->m_bBasisPoint = vCOMP_BASIS[i];

				pCOMP->DefaultPosition( vBASIS, bRestore);
			}
		}
	}
	else
	{
		for( INT i=0 ; i < 3 ; ++i )
		{
			CTClientUIBase* pCOMP = (CTClientUIBase*) FindKid( vCOMP[ i ][0] );
			if( pCOMP )
			{
				CPoint pt;
				pCOMP->GetComponentPos(&pt);
				pt -= vBASIS[ vCOMP[i][1] ];

				pCOMP->m_vCompOffset = pt;
				pCOMP->m_bBasisPoint = (BYTE) vCOMP[i][1];

				vCOMP_OFFSET[i] = pCOMP->m_vCompOffset;
				vCOMP_BASIS[i] = pCOMP->m_bBasisPoint;

				pCOMP->DefaultPosition( vBASIS, bRestore);
			}
		}
	}
}

void CTTournamentGage::OnRButtonUp(UINT nFlags, CPoint pt)
{
	if( !IsVisible() )
		return ;

	CTClientGame* pGame = CTClientGame::GetInstance();

	for( int i=0 ; i < 2 ; ++i )
	{
		for( int n=0 ; n < 7 ; ++n )
		{
			if( m_pBASE[i][n]->IsVisible() &&
				m_pBASE[i][n]->HitTest(pt) &&
				!(m_pNAME[i][n]->m_strText.IsEmpty()) )
			{
				DWORD dwCharID = pGame->m_pTLeftTeam ? pGame->m_pTLeftTeam->FindPlayer(m_pNAME[i][n]->m_strText) : 0;
				if( !dwCharID )
					dwCharID = pGame->m_pTRightTeam ? pGame->m_pTRightTeam->FindPlayer(m_pNAME[i][n]->m_strText) : 0;

				if( dwCharID )
				{
					pGame->OnGM_CLOSE_POPUP();

					pGame->m_pTPOPUP->AddMENU(
						CTChart::LoadString(TSTR_TOURNAMENT_CHEER),
						TCOMMAND(GM_TOURNAMENT_USE_CHEERITEM, dwCharID) );

					pGame->m_pTPOPUP->AddMENU(
						CTChart::LoadString(TSTR_CANCEL),
						GM_CLOSE_POPUP );

					pGame->ShowPOPUP();
				}

				return ;
			}
		}
	}
}

void CTTournamentGage::ResetTMAINTAIN()
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	for( int i=0 ; i < 2 ; ++i )
	{
		for( int n=0 ; n < 7 ; ++n )
		{
			DWORD dwCharID = pGame->m_pTLeftTeam ? pGame->m_pTLeftTeam->FindPlayer(m_pNAME[i][n]->m_strText) : 0;
			if( !dwCharID )
				dwCharID = pGame->m_pTRightTeam ? pGame->m_pTRightTeam->FindPlayer(m_pNAME[i][n]->m_strText) : 0;

			if( dwCharID )
				pGame->ResetTMAINTAIN( pGame->FindPlayer(dwCharID), m_pMAINTAIN[ i ][ n ] );
			else
				m_pMAINTAIN[ i ][ n ]->HideAll();
		}
	}

	ResetTournamentINFO();
}

ITDetailInfoPtr CTTournamentGage::GetTInfoKey( const CPoint& point)
{
	for( int i=0 ; i < 2 ; ++i )
	{
		for( int n=0 ; n < 7 ; ++n )
		{
			if( m_pMAINTAIN[ i ][ n ] &&
				m_pMAINTAIN[ i ][ n ]->m_bVisible &&
				m_pMAINTAIN[ i ][ n ]->HitTest(point) )
			{
				return m_pMAINTAIN[ i ][ n ]->GetTInfoKey(point);
			}
		}
	}

	return ITDetailInfoPtr();
}

void CTTournamentGage::ResetTournamentINFO()
{
	BOOL bIterating[2][7] =
	{
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE },
		{ FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE }
	};

	if( m_bShowPlayerInfo )
	{
		CTClientGame* pGame = CTClientGame::GetInstance();

		LPTTOURNAMENTTEAM pTeam[2]=
		{
			pGame->m_pTLeftTeam,
			pGame->m_pTRightTeam
		};

		for( INT t=0; t < 2 ; ++t )
		{
			if( !pTeam[t] )
				continue;

			BYTE bORDER = 0;

			for( INT m=0; m < pTeam[t]->m_vMembers.size() ; ++m )
			{
				++bORDER; //1부터 시작

				LPTTOURNAMENTPLAYER pTPlayer = pTeam[t]->m_vMembers[m];

				BYTE bOrder = bORDER;

				if( (pTPlayer->m_dwCharID == pTPlayer->m_dwChiefID) ||
					(m == pTeam[t]->m_vMembers.size()-1 && bIterating[t][0] == FALSE) )
				{
					--bORDER;
					bOrder = 0;
				}

				CTClientChar* pTChar = pGame->FindPC( pTPlayer->m_dwCharID );
				bIterating[t][bOrder] = TRUE;

				if( pTChar )
				{
					m_pBACK[t][ bOrder ]->ShowComponent(TRUE);
					m_pBASE[t][ bOrder ]->ShowComponent(TRUE);
					m_pLVBASE[t][ bOrder ]->ShowComponent(TRUE);

					m_pFACE[t][bOrder]->SetCurImage( pGame->GetFACE(
						pTChar->GetRaceID(),
						pTChar->m_bSex,
						pTChar->m_bFace,
						pTChar->m_bHair) );
					m_pFACE[t][bOrder]->ShowComponent(TRUE);

					m_pHP[t][bOrder]->SetGauge(
						pTChar->m_dwHP,
						pTChar->m_dwMaxHP,
						FALSE );
					m_pHP[t][bOrder]->ShowComponent(TRUE);

					m_pHPText[t][bOrder]->m_strText = CTChart::Format( TSTR_FMT_GAUGE,
						pTChar->m_dwHP,
						pTChar->m_dwMaxHP );
					m_pHPText[t][bOrder]->ShowComponent(TRUE);

					if( m_pCLASSText[t][bOrder] )
					{
						m_pCLASSText[t][bOrder]->m_strText = CTChart::LoadString(
							(TSTRING) CTClientGame::m_vTCLASSSTR[ pTChar->m_bClassID ] );
						m_pCLASSText[t][bOrder]->ShowComponent(TRUE);
					}

					m_pMP[t][bOrder]->SetGauge(
						pTChar->m_dwMP,
						pTChar->m_dwMaxMP,
						FALSE );
					m_pMP[t][bOrder]->ShowComponent(TRUE);

					m_pNAME[t][bOrder]->m_strText = CTChart::Format( TSTR_FMT_NAME, pTChar->m_strNAME);
					m_pNAME[t][bOrder]->ShowComponent(TRUE);

					m_pLEVEL[t][bOrder]->m_strText = CTChart::Format( TSTR_FMT_LEVEL, pTChar->m_bLevel);
					m_pLEVEL[t][bOrder]->ShowComponent(TRUE);

					m_pMAINTAIN[t][bOrder]->m_bVisible = TRUE;
				}
				else
				{
					m_pBACK[t][bOrder]->ShowComponent(TRUE);
					m_pBASE[t][bOrder]->ShowComponent(TRUE);
					m_pLVBASE[t][bOrder]->ShowComponent(TRUE);
					m_pFACE[t][bOrder]->SetCurImage( 0 );
					m_pFACE[t][bOrder]->ShowComponent(TRUE);
					m_pHP[t][bOrder]->SetGauge( 0, 0, FALSE );
					m_pHPText[t][bOrder]->m_strText.Empty();

					if( m_pCLASSText[t][bOrder] )
						m_pCLASSText[t][bOrder]->m_strText.Empty();

					m_pMP[t][bOrder]->SetGauge( 0, 0, FALSE );
					m_pNAME[t][bOrder]->m_strText = pTPlayer->m_strName;
					m_pNAME[t][bOrder]->ShowComponent(TRUE);
					m_pLEVEL[t][bOrder]->m_strText = CTChart::Format( TSTR_FMT_LEVEL, pTPlayer->m_bLevel );
					m_pLEVEL[t][bOrder]->ShowComponent(TRUE);
					m_pMAINTAIN[t][bOrder]->ShowComponent(FALSE);
				}
			}		
		}
	}

	for( INT bSlot=0 ; bSlot < 2 ; ++bSlot )
	{
		for( INT bOrder=0 ; bOrder < 7 ; ++bOrder )
		{
			if( !bIterating[bSlot][bOrder] )
			{
				m_pBACK[bSlot][bOrder]->ShowComponent(FALSE);
				m_pBASE[bSlot][bOrder]->ShowComponent(FALSE);
				m_pLVBASE[bSlot][bOrder]->ShowComponent(FALSE);
				m_pFACE[bSlot][bOrder]->ShowComponent(FALSE);
				m_pHP[bSlot][bOrder]->ShowComponent(FALSE);
				m_pHPText[bSlot][bOrder]->ShowComponent(FALSE);

				if( m_pCLASSText[bSlot][bOrder] )
					m_pCLASSText[bSlot][bOrder]->ShowComponent(FALSE);

				m_pMP[bSlot][bOrder]->ShowComponent(FALSE);
				m_pNAME[bSlot][bOrder]->ShowComponent(FALSE);
				m_pLEVEL[bSlot][bOrder]->ShowComponent(FALSE);
				m_pMAINTAIN[bSlot][bOrder]->ShowComponent(FALSE);
			}
		}
	}
}


#endif