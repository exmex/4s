#include "Stdafx.h"
#include "TGuildRTTypeFrame.h"
#include "TGuildVolunteerFrame.h"
#include "TClientGame.h"
#include "TMailSendDlg.h"
#include "Resource.h"

#include "TCommunityDlg.h" //test

CTGuildVolunteerFrame::PopupBtnInfo CTGuildVolunteerFrame::m_PopupBtnInfo[GVFPM_COUNT] = 
{
//	{m_dwTXT,						m_dwGM	},
	{TSTR_GVFPM_MAIL,						GM_GVFPM_MAIL					},	///< GVFPM_MAIL
	{TSTR_GVFPM_REGULAR_ACCEPT,		GM_GVFPM_REGULAR_ACCEPT	},	///< GVFPM_REGULAR_ACCEPT
	{TSTR_GVFPM_REGULAR_REJECT,		GM_GVFPM_REGULAR_REJECT	},	///< GVFPM_REGULAR_REJECT
	{TSTR_GVFPM_TACTICS_ACCEPT,			GM_GVFPM_TACTICS_ACCEPT	},	///< GVFPM_TACTICS_ACCEPT
	{TSTR_GVFPM_TACTICS_REJECT,			GM_GVFPM_TACTICS_REJECT		},	///< GVFPM_TACTICS_REJECT
	{TSTR_GVFPM_CLOSE,						GM_GVFPM_CLOSE					},	///< GVFPM_CLOSE
};

CTGuildVolunteerFrame::CTGuildVolunteerFrame(TComponent *pParent, LP_FRAMEDESC pDesc, LP_FRAMEDESC pPopupDesc)
: ITInnerFrame( pParent, pDesc, TCOMMUNITY_FRAME_GUILDVOLUNTEER ),
m_nCurPage( TGM_REGULAR ),
m_nLastSelType(T_INVALID),
m_nLastSelIdx(T_INVALID)
{
	m_pTAB[0] = (TButton*) FindKid( ID_CTRLINST_TAB_1 );
	m_pTAB[1] = (TButton*) FindKid( ID_CTRLINST_TAB_2 );

	m_nTABType[0] = TGM_REGULAR;
	m_nTABType[1] = TGM_TACTICS;

	((TButton*) FindKid( ID_CTRLINST_TAB_1))->Select( TRUE );

	m_pList = (TList*) FindKid( ID_CTRLINST_LIST );

	m_pPopup = new CTMiniPopupDlg(this ,pPopupDesc, FALSE);
	m_pPopup->m_id = GetUniqueID();
	m_bPopupVisible = FALSE;

	m_pDownItem = NULL;

	m_pColumnC = (TList*) FindKid( ID_CTRLINST_COL );
	m_pColumnC->RemoveAll();
	
	INT i = -1;
	// 정규군
	i = m_pColumnC->AddString( CTChart::LoadString( TSTR_CHAR_NAME ) );
	m_pColumnC->SetItemString( i, 1, CTChart::LoadString( TSTR_GUILD_JOB ) );
	m_pColumnC->SetItemString( i, 2, CTChart::LoadString( TSTR_GUILD_LEVEL ) );
	m_pColumnC->SetItemString( i, 3, CTChart::LoadString( TSTR_GUILD_NOWPOS ) );

	// 용병
	i = m_pColumnC->AddString( CTChart::LoadString( TSTR_CHAR_NAME ) );
	m_pColumnC->SetItemString( i, 1, CTChart::LoadString( TSTR_GUILD_JOB ) );
	m_pColumnC->SetItemString( i, 2, CTChart::LoadString( TSTR_GUILD_LEVEL ) );
	m_pColumnC->SetItemString( i, 3, CTChart::LoadString( TSTR_GUILDOFFER_COL_EARNEST_PERIOD ) );
	m_pColumnC->SetItemString( i, 4, CTChart::LoadString( TSTR_GUILD_REWARD_POINT ) );
	m_pColumnC->SetItemString( i, 5, CTChart::LoadString( TSTR_GUILD_EARNESTMONEY ) );
	m_pColumnC->SetItemString( i, 6, CTChart::LoadString( TSTR_GUILD_NOWPOS ) );

	m_pColumnC->SetTop( 0 );
}

CTGuildVolunteerFrame::~CTGuildVolunteerFrame()
{
	if( m_pPopup )
		delete m_pPopup;
}

void CTGuildVolunteerFrame::RequestInfo()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientSession* pSESSION = pGame->GetSession();
	CTClientChar* pMainChar = pGame->GetMainChar();

	if( pMainChar->m_dwGuildID == 0 && pMainChar->m_dwTacticsID == 0 )
		return ;

	switch( m_nCurPage )
	{
	case TGM_REGULAR:
		if( pSESSION )
		{
			pSESSION->SendCS_GUILDVOLUNTEERLIST_REQ();
		}
		break;
	case TGM_TACTICS:
		if( pSESSION )
		{
			pSESSION->SendCS_GUILDTACTICSVOLUNTEERLIST_REQ();
		}
		break;
	}
}

void CTGuildVolunteerFrame::ResetInfo()
{
	CTGuildCommander* pGuildCmd = CTGuildCommander::GetInstance();

	switch( m_nCurPage )
	{
	case TGM_REGULAR:
		{
			m_pColumnC->SetTop( 0 );
			m_pList->RemoveAll();

			CTGuildCommander::GuildVolunteerVec::iterator itMEMBER, end;
			itMEMBER = pGuildCmd->m_GuildVolunteerVec.begin();
			end = pGuildCmd->m_GuildVolunteerVec.end();

			for(; itMEMBER != end ; ++itMEMBER )
			{
				int nLine = m_pList->AddString("");

				for(int i=0; i<GRVI_COUNT; ++i)
				{
					switch( i )
					{
					case GRVI_NAME:
						{
							m_pList->SetItemString( nLine, GRVI_NAME, itMEMBER->m_strName );
							m_pList->SetItemData( nLine, GRVI_NAME, MAKELONG( nLine, GRVI_NAME) );
						}
						break;

					case GRVI_CLASS:
						{
							m_pList->SetItemString( nLine, GRVI_CLASS,
								CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ itMEMBER->m_bClass] ) );

							m_pList->SetItemData( nLine, GRVI_CLASS, MAKELONG( nLine, GRVI_CLASS) );
						}
						break;

					case GRVI_LEVEL	:
						{
							m_pList->SetItemString( nLine, GRVI_LEVEL,
								CTChart::Format( TSTR_FMT_NUMBER, itMEMBER->m_bLevel) );

							m_pList->SetItemData( nLine, GRVI_LEVEL, MAKELONG( nLine, GRVI_LEVEL) );
						}
						break;

					case GRVI_REGION	:
						{
							if( itMEMBER->m_dwRegion != 0 )
							{
								LPTREGIONINFO pRegion = CTChart::FindTREGIONINFO( itMEMBER->m_dwRegion);

								if( pRegion )
									m_pList->SetItemString( nLine, GRVI_REGION, pRegion->m_strNAME);
							}
							else
							{
								m_pList->SetItemString( nLine, GRVI_REGION, CTChart::LoadString( TSTR_STATE_DISCONNECT));
							}

							m_pList->SetItemData( nLine, GRVI_REGION, MAKELONG( nLine, GRVI_REGION) );
						}
						break;
					}
				}
			}
		}
		break;

	case TGM_TACTICS:
		{
			m_pColumnC->SetTop( 1 );
			m_pList->RemoveAll();

			CTGuildCommander::GuildTacticsVolunteerVec::iterator itMEMBER, end;
			itMEMBER = pGuildCmd->m_TacticsVolunteerVec.begin();
			end = pGuildCmd->m_TacticsVolunteerVec.end();

			for(; itMEMBER != end ; ++itMEMBER )
			{
				int nLine = m_pList->AddString("");

				for(int i=0; i<GTVI_COUNT; ++i)
				{
					switch( i )
					{
					case GTVI_NAME:
						{
							m_pList->SetItemString( nLine, GTVI_NAME, itMEMBER->m_strName );
							m_pList->SetItemData( nLine, GTVI_NAME, MAKELONG( nLine, GRVI_NAME) );
						}
						break;

					case GTVI_CLASS:
						{
							m_pList->SetItemString( nLine, GTVI_CLASS,
								CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ itMEMBER->m_bClass] ) );

							m_pList->SetItemData( nLine, GTVI_CLASS, MAKELONG( nLine, GRVI_CLASS) );
						}
						break;

					case GTVI_LEVEL	:
						{
							m_pList->SetItemString( nLine, GTVI_LEVEL,
								CTChart::Format( TSTR_FMT_NUMBER, itMEMBER->m_bLevel) );

							m_pList->SetItemData( nLine, GTVI_LEVEL, MAKELONG( nLine, GRVI_LEVEL) );
						}
						break;

					case GTVI_DAY:
						{
							CString strFMT;
							strFMT.Format( "%d", itMEMBER->m_bDay );
							m_pList->SetItemString( nLine, GTVI_DAY, strFMT );
							m_pList->SetItemData( nLine, GTVI_DAY, MAKELONG( nLine, GTVI_DAY) );
						}
						break;

					case GTVI_POINT:
						{
							CString strFMT;
							strFMT.Format( "%d", itMEMBER->m_dwPoint );
							m_pList->SetItemString( nLine, GTVI_POINT, strFMT );
							m_pList->SetItemData( nLine, GTVI_POINT, MAKELONG( nLine, GTVI_POINT) );
						}
						break;

					case GTVI_MONEY:
						{
							CString strFMT;
							strFMT.Format( "%d/%d/%d", itMEMBER->m_dwGold,
								itMEMBER->m_dwSilver,
								itMEMBER->m_dwCooper );

							m_pList->SetItemString( nLine, GTVI_MONEY, strFMT );
							m_pList->SetItemData( nLine, GTVI_MONEY, MAKELONG( nLine, GTVI_MONEY) );
						}
						break;

					case GTVI_REGION	:
						{
							if( itMEMBER->m_dwRegion != 0 )
							{
								LPTREGIONINFO pRegion = CTChart::FindTREGIONINFO( itMEMBER->m_dwRegion);

								if( pRegion )
									m_pList->SetItemString( nLine, GTVI_REGION, pRegion->m_strNAME);
							}
							else
							{
								m_pList->SetItemString( nLine, GTVI_REGION, CTChart::LoadString( TSTR_STATE_DISCONNECT));
							}

							m_pList->SetItemData( nLine, GTVI_REGION, MAKELONG( nLine, GRVI_REGION) );
						}
						break;
					}
				}
			}
		}
		break;
	}
}

void CTGuildVolunteerFrame::ChangePage( INT nType )
{
	BOOL bFind = FALSE;
	for( INT s=0 ; s < TGM_COUNT ; ++s )
	{
		if( nType == m_nTABType[s] )
		{
			bFind = TRUE;
			break;
		}
	}

	if( !bFind || s == 0 )
		return ;

	INT nSELECTED = m_nTABType[ s ];

	for( INT n=s ; n > 1 ; --n )
		m_nTABType[ n ] = m_nTABType[ n - 1 ];

	m_nTABType[ 1 ] = m_nTABType[ 0 ];
	m_nTABType[ 0 ] = nSELECTED;

	for( INT i=0 ; i < TGM_COUNT ; ++i )
	{
		switch( m_nTABType[i] )
		{
		case TGM_REGULAR:
			m_pTAB[ i ]->m_strText = CTChart::LoadString( TSTR_GUILD_REGULAR_VOL );
			break;
		case TGM_TACTICS:
			m_pTAB[ i ]->m_strText = CTChart::LoadString( TSTR_GUILD_TACTICS_VOL );
			break;
		}
	}

	m_nCurPage = nType;
	RequestInfo();
}

void CTGuildVolunteerFrame::ResetTabButton( BYTE bTabIndex )
{
	TButton *pTAB[TGM_COUNT] = {
		(TButton *) FindKid(ID_CTRLINST_TAB_1),
			(TButton *) FindKid(ID_CTRLINST_TAB_2),
	};

	for( BYTE i=0; i<TGM_COUNT; i++)
	{
		pTAB[i]->Select( i == bTabIndex ? TRUE : FALSE);

		/*CGDIFont *pGDIFont = FindFont(pTAB[i]->IsStateDown() ? ID_FONT_LOGIN : ID_FONT_DISABLE);

		if( pTAB[i]->GetFont() != pGDIFont )
			pTAB[i]->SetFont( pGDIFont, NULL);*/
	}

	m_nCurPage = bTabIndex;
	RequestInfo();
}

HRESULT CTGuildVolunteerFrame::Render( DWORD dwTickCount )
{
	return ITInnerFrame::Render( dwTickCount );
}

void CTGuildVolunteerFrame::OnLButtonDown( UINT nFlags, CPoint pt )
{
	if( !m_bPopupVisible || !m_pPopup->HitTest(pt) )
		m_pDownItem = m_pList->GetHitItem( pt);

	ITInnerFrame::OnLButtonDown( nFlags, pt );
}

void CTGuildVolunteerFrame::OnLButtonUp( UINT nFlags, CPoint pt )
{
	ITInnerFrame::OnLButtonUp( nFlags, pt );

	TListItem* pDownItem = m_pDownItem;
	m_pDownItem = NULL;

	if( !m_bPopupVisible || !m_pPopup->HitTest(pt) )
	{
		TListItem* pItem = m_pList->GetHitItem( pt);
		if( pItem != pDownItem )
			return;

		if( pItem )
		{
			m_nLastSelIdx 	= LOWORD(pItem->m_param);
			m_nLastSelType	= HIWORD(pItem->m_param);
		}
		else
		{
			m_nLastSelIdx	= T_INVALID;
			m_nLastSelType	= T_INVALID;
		}

		ShowPopup(pt);
	}
}

void CTGuildVolunteerFrame::ShowComponent(BOOL bVisible)
{
	ITInnerFrame::ShowComponent( bVisible );
}

void CTGuildVolunteerFrame::ShowPopup(const CPoint& pt)
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return;

	for( INT i=0; i < GVFPM_COUNT ; ++i )
	{
		BOOL bADD = FALSE;

		switch( i )
		{
			case GVFPM_MAIL:
				{
					bADD = TRUE;
				}
				break;
			case GVFPM_REGULAR_ACCEPT:
				{
					if( m_nCurPage == TGM_REGULAR )
						bADD = TRUE;
				}
				break;
			case GVFPM_REGULAR_REJECT:
				{
					if( m_nCurPage == TGM_REGULAR )
						bADD = TRUE;
				}
				break;
			case GVFPM_TACTICS_ACCEPT:
				{
					if( m_nCurPage == TGM_TACTICS )
						bADD = TRUE;
				}
				break;
			case GVFPM_TACTICS_REJECT:
				{
					if( m_nCurPage == TGM_TACTICS )
						bADD = TRUE;
				}
				break;
		}

		if( bADD )
		{
			CString strTITLE;
			strTITLE = CTChart::LoadString( (TSTRING) m_PopupBtnInfo[i].m_dwTXT);
			m_pPopup->AddButton(strTITLE, m_PopupBtnInfo[i].m_dwGM);
		}
	}

	if( !m_pPopup->IsButtonEmpty() )
	{
		CString strTITLE;
		strTITLE = CTChart::LoadString( (TSTRING) m_PopupBtnInfo[GVFPM_CLOSE].m_dwTXT);
		m_pPopup->AddButton(strTITLE, m_PopupBtnInfo[GVFPM_CLOSE].m_dwGM);
	
		AddKid(m_pPopup);

		CRect rc = m_rc;
		ComponentToScreen(&rc);
		CPoint ptPOP = pt - rc.TopLeft();

		m_pPopup->MoveComponent(ptPOP);
		m_pPopup->ShowComponent(TRUE);

		m_bPopupVisible = TRUE;
	}
}

void CTGuildVolunteerFrame::CancelPopup()
{
	RemoveKid(m_pPopup);
	m_pPopup->ClearButtons();
	m_bPopupVisible = FALSE;
}

int CTGuildVolunteerFrame::OnGM_GVFPM_MAIL()	///< GVFPM_MAIL
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame->CanUseMail() )
		return TERR_MAIL_REGION;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	CString strCHAR;
	switch( m_nCurPage )
	{
	case TGM_REGULAR:
		strCHAR = pCmd->m_GuildVolunteerVec[ m_nLastSelIdx ].m_strName;
		break;

	case TGM_TACTICS:
		strCHAR = pCmd->m_TacticsVolunteerVec[ m_nLastSelIdx ].m_strName;
		break;
	}

	CTMailSendDlg* pSndDlg = static_cast<CTMailSendDlg*>(pGame->GetFrame(TFRAME_MAIL_SEND));
	pSndDlg->SetReceiver(strCHAR);
	pSndDlg->SetTitle("");

	pGame->EnableUI(TFRAME_MAIL_SEND);
	return TERR_NONE;
}

int CTGuildVolunteerFrame::OnGM_GVFPM_REGULAR_ACCEPT()	///< GVFPM_REGULAR_ACCEPT
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( pGame->GetSession() )
	{
		pGame->GetSession()->SendCS_GUILDVOLUNTEERREPLY_REQ(
			pCmd->m_GuildVolunteerVec[ m_nLastSelIdx ].m_dwCharID,
			TRUE );
	}

	return TERR_NONE;
}

int CTGuildVolunteerFrame::OnGM_GVFPM_REGULAR_REJECT()	///< GVFPM_REGULAR_REJECT
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( pGame->GetSession() )
	{
		pGame->GetSession()->SendCS_GUILDVOLUNTEERREPLY_REQ(
			pCmd->m_GuildVolunteerVec[ m_nLastSelIdx ].m_dwCharID,
			FALSE );
	}

	return TERR_NONE;
}

int CTGuildVolunteerFrame::OnGM_GVFPM_TACTICS_ACCEPT()	///< GVFPM_TACTICS_ACCEPT
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( pGame->GetSession() )
	{
		pGame->GetSession()->SendCS_GUILDTACTICSREPLY_REQ(
			pCmd->m_TacticsVolunteerVec[ m_nLastSelIdx ].m_dwCharID,
			TRUE );
	}

	return TERR_NONE;
}

int CTGuildVolunteerFrame::OnGM_GVFPM_TACTICS_REJECT()	///< GVFPM_TACTICS_REJECT
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( pGame->GetSession() )
	{
		pGame->GetSession()->SendCS_GUILDTACTICSREPLY_REQ(
			pCmd->m_TacticsVolunteerVec[ m_nLastSelIdx ].m_dwCharID,
			FALSE );
	}

	return TERR_NONE;
}

int CTGuildVolunteerFrame::OnGM_GVFPM_CLOSE()	///< GVFPM_CLOSE
{
	CancelPopup();
	return TERR_NONE;
}
