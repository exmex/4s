#include "Stdafx.h"
#include "TGuildTerritoryFrame.h"
#include "TClientGame.h"
#include "Resource.h"

// ====================================================================
CTGuildTerritoryFrame::CTGuildTerritoryFrame(TComponent *pParent, LP_FRAMEDESC pDesc)
	: ITInnerFrame(pParent, pDesc, TCOMMUNITY_FRAME_GUILDTERRITORY), m_nCurMode(MODE_CASTLE)
{
	m_pList = static_cast<TList*>( FindKid(ID_CTRLINST_LIST) );
	
	m_vTabBtn[MODE_CASTLE] = static_cast<TButton*>( FindKid(ID_CTRLINST_TAB_1) );
	m_vTabBtn[MODE_LOCAL]  = static_cast<TButton*>( FindKid(ID_CTRLINST_TAB_2) );		
	m_vTabBtn[MODE_MISSION]  = static_cast<TButton*>( FindKid(ID_CTRLINST_TAB_3) );		

	m_vCol[0] = FindKid(ID_CTRLINST_COL_1);
	m_vCol[1] = FindKid(ID_CTRLINST_COL_2);
	m_vCol[2] = FindKid(ID_CTRLINST_COL_3);
	m_vCol[3] = FindKid(ID_CTRLINST_COL_4);
	m_vCol[4] = FindKid(ID_CTRLINST_COL_5);
	m_vCol[5] = FindKid(ID_CTRLINST_COL_6);

	m_vColBack[0] = FindKid(ID_CTRLINST_COL_BACK);
	m_vColBack[1] = FindKid(ID_CTRLINST_COL_BACK2);

	m_vCol[1]->m_strText = CTChart::LoadString( TSTR_GUILD_OCCUPATIONGUILD );
	m_vCol[2]->m_strText = CTChart::LoadString( TSTR_GUILD_BELONGNATION );
	m_vCol[3]->m_strText = CTChart::LoadString( TSTR_MISSION_COL_LOCAL );
	m_vCol[4]->m_strText = CTChart::LoadString( TSTR_MISSION_COL_COUNTRY );
	m_vCol[5]->m_strText = CTChart::LoadString( TSTR_MISSION_COL_STATUS );

	TComponent* pComp = FindKid(ID_CTRLINST_BTN);
	if( pComp )
	{
		RemoveKid(pComp);
		delete pComp;
	}
}
// --------------------------------------------------------------------
CTGuildTerritoryFrame::~CTGuildTerritoryFrame()
{
}
// ====================================================================

// ====================================================================
void CTGuildTerritoryFrame::SetCurMode(INT nMode)
{
	m_nCurMode = nMode;

	m_pList->RemoveAll();

	for(INT i=0; i<MODE_COUNT; ++i)
		m_vTabBtn[i]->Select( i == m_nCurMode );

	switch( m_nCurMode )
	{
	case MODE_CASTLE:
		m_vCol[0]->m_strText = CTChart::LoadString( TSTR_GUILD_CASTLE );
		m_vCol[0]->ShowComponent(TRUE);
		m_vCol[1]->ShowComponent(TRUE);
		m_vCol[2]->ShowComponent(TRUE);
		m_vCol[3]->ShowComponent(FALSE);
		m_vCol[4]->ShowComponent(FALSE);
		m_vCol[5]->ShowComponent(FALSE);
		m_vColBack[0]->ShowComponent(TRUE);
		m_vColBack[1]->ShowComponent(FALSE);
		break;
	case MODE_LOCAL:
		m_vCol[0]->m_strText = CTChart::LoadString( TSTR_GUILD_VILLAGE );
		m_vCol[0]->ShowComponent(TRUE);
		m_vCol[1]->ShowComponent(TRUE);
		m_vCol[2]->ShowComponent(TRUE);
		m_vCol[3]->ShowComponent(FALSE);
		m_vCol[4]->ShowComponent(FALSE);
		m_vCol[5]->ShowComponent(FALSE);
		m_vColBack[0]->ShowComponent(TRUE);
		m_vColBack[1]->ShowComponent(FALSE);
		break;
	case MODE_MISSION:
		m_vCol[0]->ShowComponent(FALSE);
		m_vCol[1]->ShowComponent(FALSE);
		m_vCol[2]->ShowComponent(FALSE);
		m_vCol[3]->ShowComponent(TRUE);
		m_vCol[4]->ShowComponent(TRUE);
		m_vCol[5]->ShowComponent(TRUE);
		m_vColBack[0]->ShowComponent(FALSE);
		m_vColBack[1]->ShowComponent(TRUE);
		break;
	default:
		m_vCol[0]->ShowComponent(FALSE);
		m_vCol[1]->ShowComponent(FALSE);
		m_vCol[2]->ShowComponent(FALSE);
		m_vCol[3]->ShowComponent(FALSE);
		m_vCol[4]->ShowComponent(FALSE);
		m_vCol[5]->ShowComponent(FALSE);
		m_vColBack[0]->ShowComponent(FALSE);
		m_vColBack[1]->ShowComponent(FALSE);
		break;
	};

	if( m_nCurMode == T_INVALID )
		return;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	if( m_nCurMode == MODE_CASTLE || m_nCurMode == MODE_LOCAL )
	{
		INT nCNT = (INT)pCmd->GetCountTerritory();
		for(INT i=0; i<nCNT; ++i)
		{
			const Territory& info = pCmd->GetTerritory(i);

			DWORD color = 0;
			switch( info.m_wCastleID )
			{
			case 4 : color = COLOR_CASTLE_HESED; break;
			case 8 : color = COLOR_CASTLE_ARDRI; break;
			case 12 : color = COLOR_CASTLE_GEHBRA; break;
			case 16 : color = COLOR_CASTLE_TYCON; break;
			default : color = COLOR_CASTLE_NONE;
			}

			if( m_nCurMode == MODE_CASTLE )
			{
				m_pList->ApplyUserColor( TRUE );

				int nLine = m_pList->AddString(info.m_strCastleName);
				m_pList->SetUserColor( nLine, 0, color );

				m_pList->SetItemString(nLine, 1, info.m_strGuildName);
				m_pList->SetUserColor( nLine, 1, color );

				CString strCountry;
				strCountry = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[info.m_bCountry] );
				m_pList->SetItemString(nLine, 2, strCountry);
				m_pList->SetUserColor( nLine, 2, color );

				DWORD dwInfoIndex = MAKELONG( 0, i );
				m_pList->SetItemDataAllColumn( nLine, dwInfoIndex );
			}
			else if( m_nCurMode == MODE_LOCAL )
			{
				m_pList->ApplyUserColor( TRUE );

				INT nLOCNT = (INT)info.m_vLocals.size();
				for(INT j=0; j<nLOCNT; ++j)
				{
					const LocalTerritory& local = info.m_vLocals[j];

					int nLine = m_pList->AddString(local.m_strLocalName);
					m_pList->SetUserColor( nLine, 0, color );

					m_pList->SetItemString(nLine, 1, local.m_strGuildName);
					m_pList->SetUserColor( nLine, 1, color );

					CString strCountry;
					strCountry = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[local.m_bCountry] );
					m_pList->SetItemString(nLine, 2, strCountry);
					m_pList->SetUserColor( nLine, 2, color );

					DWORD dwInfoIndex = MAKELONG( j, i );
					m_pList->SetItemDataAllColumn( nLine, dwInfoIndex );
				}
			}
		}
	}
	else if( m_nCurMode == MODE_MISSION )
	{
		m_pList->ApplyUserColor( TRUE );

		CTGuildCommander::MissionVec::iterator it, end;
		it = pCmd->m_MissionVec.begin();
		end = pCmd->m_MissionVec.end();
		int i=0;
		for(; it != end ; ++it )
		{
			int nLine = m_pList->AddString("");
			m_pList->SetItemString(nLine, 3, it->m_strMission);
			m_pList->SetUserColor( nLine, 3, COLOR_CASTLE_NONE );
			m_pList->SetColumnAlign( 3, ALIGN_LEFT );

			CString strCountry;
			if( it->m_bCountry < COUNTRY_COUNT )
				strCountry = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ it->m_bCountry ] );
			else
				strCountry = CTChart::LoadString( TSTR_MISSION_NEUTRAL );

			m_pList->SetItemString( nLine, 4, strCountry);
			m_pList->SetUserColor( nLine, 4, COLOR_CASTLE_NONE );
			m_pList->SetColumnAlign( 4, ALIGN_LEFT );

			CString strStatus;
			DWORD dwColor = COLOR_CASTLE_NONE;
			switch( it->m_bStatus )
			{
			case BS_NORMAL:
				strStatus = CTChart::LoadString( TSTR_MISSION_NORMAL );
				break;
			case BS_BATTLE:
				strStatus = CTChart::LoadString( TSTR_MISSION_WAR );
				dwColor = COLOR_CASTLE_GEHBRA;
				break;
			case BS_PEACE:
				strStatus = CTChart::LoadString( TSTR_MISSION_WAR_CLOSE );
				dwColor = COLOR_CASTLE_TYCON;
				break;
			}

			m_pList->SetItemString( nLine, 5, strStatus);
			m_pList->SetUserColor( nLine, 5, dwColor );
			m_pList->SetColumnAlign( 5, ALIGN_CENTER );

			m_pList->SetItemDataAllColumn( nLine, i++ );
		}
	}
}
// ====================================================================

// ====================================================================
void CTGuildTerritoryFrame::RequestInfo()
{
	CTGuildCommander::GetInstance()
		->RequestTerritoryList();
}
// --------------------------------------------------------------------
void CTGuildTerritoryFrame::ResetInfo()
{
	SetCurMode(m_nCurMode);
}
// ====================================================================

// ====================================================================
void CTGuildTerritoryFrame::OnLButtonDown( UINT nFlags, CPoint pt )
{
	for(INT i=0; i<MODE_COUNT; ++i)
	{
		if( m_vTabBtn[i]->HitTest(pt) )
		{
			SetCurMode(i);
			return;
		}
	}

	ITInnerFrame::OnLButtonDown(nFlags,pt);
}
// ====================================================================
void CTGuildTerritoryFrame::SwitchFocus(TComponent *pCandidate)
{
	if( m_pFocus == pCandidate )
		return;

	if( m_pFocus )
		m_pFocus->SetFocus(FALSE);

	if( pCandidate )
		pCandidate->SetFocus(TRUE);

	m_pFocus = pCandidate;
}
// ====================================================================
ITDetailInfoPtr CTGuildTerritoryFrame::GetTInfoKey( const CPoint& point )
{
	ITDetailInfoPtr pInfo;
	TListItem* pHITITEM = m_pList->GetHitItem( point );
	INT nIndex = m_pList->GetSel();

	if( pHITITEM || nIndex!=-1 )
	{
		DWORD dwParam = 0;

		if( pHITITEM != NULL )
			dwParam = pHITITEM->m_param;
		else
			dwParam = m_pList->GetItemData( nIndex, 0 );

		if( m_nCurMode == MODE_CASTLE )
		{
			WORD dwCastleIndex = HIWORD(dwParam);
			WORD dwLocalIndex = LOWORD(dwParam);

			CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
			if( dwCastleIndex >= pCmd->GetCountTerritory() )
				return pInfo;

			CTGuildCommander::Territory& terr = pCmd->GetTerritory( dwCastleIndex );

			CRect rc;
			TComponent* pPARENT = GetParent();
			pPARENT->GetComponentRect( &rc);
			pPARENT->ComponentToScreen( &rc);

			CString strMyGuild;
			CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
			if( pMainChar->m_dwGuildID )
				strMyGuild = pMainChar->m_strGUILD;
			else if( pMainChar->m_dwTacticsID )
				strMyGuild = pMainChar->m_strTACTICS;

			return CTDetailInfoManager::NewTerritoryInst(
				terr.m_strCastleName,
				terr.m_dlNextWar,
				terr.m_strAtkGuild,
#ifdef MODIFY_GUILD
				terr.m_strDefGuild,
#else
				terr.m_strGuildName,
#endif
				terr.m_wAtkGuildPoint,
				terr.m_wAtkCountryPoint,
				terr.m_bAtkCount,
				terr.m_wDefGuildPoint,
				terr.m_wDefCountryPoint,
				terr.m_bDefCount,
				strMyGuild,
				terr.m_wMyGuildPoint,
#ifdef MODIFY_GUILD
				&terr.m_vDTop3s,
				&terr.m_vCTop3s,
#endif
				rc);
		}
		else if( m_nCurMode == MODE_LOCAL )
		{
			DWORD dwCastleIndex = HIWORD(dwParam);
			DWORD dwLocalIndex = LOWORD(dwParam);

			CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
			if( dwCastleIndex >= pCmd->GetCountTerritory() )
				return pInfo;

			const CTGuildCommander::Territory& terr = pCmd->GetTerritory( dwCastleIndex );
			if( dwLocalIndex >= terr.m_vLocals.size() )
				return pInfo;

			CRect rc;
			TComponent* pPARENT = GetParent();
			pPARENT->GetComponentRect( &rc);
			pPARENT->ComponentToScreen( &rc);

			return CTDetailInfoManager::NewTerritoryInst(
				terr.m_vLocals[ dwLocalIndex ].m_strLocalName,
				terr.m_vLocals[ dwLocalIndex ].m_dNextWar,
				terr.m_vLocals[ dwLocalIndex ].m_strHeroName,
				rc);
		}
		else if( m_nCurMode == MODE_MISSION )
		{
			CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
			if( dwParam >= pCmd->m_MissionVec.size() )
				return pInfo;

			CRect rc;
			TComponent* pPARENT = GetParent();
			pPARENT->GetComponentRect( &rc);
			pPARENT->ComponentToScreen( &rc);

			return CTDetailInfoManager::NewMissionInst(
				pCmd->m_MissionVec[ dwParam ].m_strMission,
				pCmd->m_MissionVec[ dwParam ].m_NextWar,
				pCmd->m_MissionVec[ dwParam ].m_bCountry,
				pCmd->m_MissionVec[ dwParam ].m_bStatus,
				rc);
		}
	}

	return pInfo;
}

void CTGuildTerritoryFrame::ShowComponent(BOOL bVisible)
{
	ITInnerFrame::ShowComponent(bVisible);

	if( bVisible )
		SetCurMode( m_nCurMode );
}











