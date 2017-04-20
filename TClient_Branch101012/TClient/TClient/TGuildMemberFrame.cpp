#include "Stdafx.h"
#include "TGuildMemberFrame.h"
#include "TCommunityDlg.h"
#include "TGuildPointRewardFrame.h"
#include "TClientGame.h"
#include "TMailSendDlg.h"
#include "Resource.h"
#include "TClient.h"
#include "TTacticsInviteDlg.h"

const CTGuildMemberFrame::PopupBtnInfo CTGuildMemberFrame::m_PopupBtnInfo[TGM_PBTN_COUNT] = 
{
	{TSTR_GMP_WHISPER,				GM_GMP_WHISPER			},	///< TGM_PBTN_WHISPER,		
	{TSTR_GMP_MAIL,					GM_GMP_MAIL				},	///< TGM_PBTN_MAIL,			
	{TSTR_GMP_POINT_REWARD,			GM_GMP_POINT_REWARD		},	///< TGM_PBTN_POINT_REWARD,
	{TSTR_GMP_PARTY,				GM_GMP_PARTY			},	///< TGM_PBTN_PARTY
	{TSTR_GMP_FRIEND,				GM_GMP_FRIEND			},	///< TGM_PBTN_FRIEND
	{TSTR_GMP_KICK,					GM_GMP_KICK				},	///< TGM_PBTN_KICK,	
	{TSTR_GMP_ET_GM,				GM_GMP_ET_GM			},	///< TGM_PBTN_SET_GM,		
	{TSTR_GMP_SET_GSM,				GM_GMP_SET_GSM			},	///< TGM_PBTN_SET_GSM,		
	{TSTR_GMP_CANCEL_GSM,			GM_GMP_CANCEL_GSM		},	///< TGM_PBTN_CANCEL_GSM,	
	{TSTR_GMP_PEER_BARON,			GM_GMP_PEER_BARON		},	///< TGM_PBTN_PEER_BARON,	
	{TSTR_GMP_PEER_VISCOUNT,		GM_GMP_PEER_VISCOUNT	},	///< TGM_PBTN_PEER_VISCOUNT,
	{TSTR_GMP_PEER_COUNT,			GM_GMP_PEER_COUNT		},	///< TGM_PBTN_PEER_COUNT,	
	{TSTR_GMP_PEER_MARQUIS,			GM_GMP_PEER_MARQUIS		},	///< TGM_PBTN_PEER_MARQUIS,	
	{TSTR_GMP_PEER_DUKE,			GM_GMP_PEER_DUKE		},	///< TGM_PBTN_PEER_DUKE,	
	{TSTR_GMP_CANCEL_PEER,			GM_GMP_CANCEL_PEER		},	///< TGM_PBTN_CANCEL_PEER,	
	{TSTR_GMP_CANCEL,				GM_GMP_CANCEL			},	///< TGM_PBTN_CANCEL,	
	{TSTR_GMP_VOLUNTEER_HESED,		GM_GMP_VOLUNTEER_HESED	},	///< PBTN_VOLUNTEER_HESED,
	{TSTR_GMP_VOLUNTEER_ARDRI,		GM_GMP_VOLUNTEER_ARDRI	},
	{TSTR_GMP_VOLUNTEER_TYCON,		GM_GMP_VOLUNTEER_TYCON	},
	{TSTR_GMP_VOLUNTEER_GEHBRA,		GM_GMP_VOLUNTEER_GEHBRA	},
	{TSTR_GMP_VOLUNTEER_CANCEL,		GM_GMP_VOLUNTEER_CANCEL	},

	{TSTR_GMP_TACTICS_RE_CONTRACT,	GM_GMP_TACTICS_RE_CONTRACT	}, ///< TGM_PBTN_TACTICS_RE_CONTRACT,			///< 용병 재계약
	{TSTR_GMP_TACTICS_KICK,			GM_GMP_TACTICS_KICK			}, ///< TGM_PBTN_TACTICS_KICK,						///< 용병 계약 파기
	{TSTR_GMP_TACTICS_REWARD_POINT,	GM_GMP_TACTICS_REWARD_POINT	}, ///< TGM_PBTN_TACTICS_REWARD_POINT,			///< 용병 포상
};

CTGuildMemberFrame::CTGuildMemberFrame( TComponent *pParent, LP_FRAMEDESC pDesc, LP_FRAMEDESC pPopupDesc)
: ITInnerFrame(pParent, pDesc, TCOMMUNITY_FRAME_GUILDMEMBER),
m_nLastSelType(T_INVALID),
m_nLastSelIdx(T_INVALID),
m_bPopupVisible(FALSE),
m_pDownItem(NULL),
//m_bGuildEnable(FALSE),
m_pPopup(NULL)
{
	m_pColumnC = (TList*) FindKid( ID_CTRLINST_COL_C );
	m_pColumnD = (TList*) FindKid( ID_CTRLINST_COL_D );
	m_pListC = (TList*) FindKid( ID_CTRLINST_LIST_C );
	m_pListD = (TList*) FindKid( ID_CTRLINST_LIST_D );

	m_pMissionTxt = FindKid( ID_CTRLINST_TXT_MISSION );
	m_pMemCntTxt = FindKid( ID_CTRLINST_TXT_COUNT );

	m_pTAB[0] = (TButton*) FindKid( ID_CTRLINST_TAB_1 );
	m_pTAB[1] = (TButton*) FindKid( ID_CTRLINST_TAB_2 );
	m_pTAB[2] = (TButton*) FindKid( ID_CTRLINST_TAB_3 );
	m_pTAB[3] = (TButton*) FindKid( ID_CTRLINST_TAB_4 );

	m_nTABType[0] = TPVPM_REGULAR;
	m_nTABType[1] = TPVPM_TACTICS;
	m_nTABType[2] = TPVPM_LATEST_PVPR;
	m_nTABType[3] = TPVPM_WEEK_PVPR;

	m_pColumnC->RemoveAll();
	
	INT i = -1;
	// 정규군
	i = m_pColumnC->AddString( CTChart::LoadString( TSTR_CHAR_NAME ) );
	m_pColumnC->SetItemString( i, 1, CTChart::LoadString( TSTR_GUILD_JOB ) );
	m_pColumnC->SetItemString( i, 2, CTChart::LoadString( TSTR_GUILD_LEVEL ) );
	m_pColumnC->SetItemString( i, 3, CTChart::LoadString( TSTR_GUILD_CONTRIBUTION ) );
	m_pColumnC->SetItemString( i, 4, CTChart::LoadString( TSTR_GUILD_RANK ) );
	m_pColumnC->SetItemString( i, 5, CTChart::LoadString( TSTR_GUILD_PEERAGE ) );
	m_pColumnC->SetItemString( i, 6, CTChart::LoadString( TSTR_GUILD_NOWPOS ) );

	// 용병
	i = m_pColumnC->AddString( CTChart::LoadString( TSTR_CHAR_NAME ) );
	m_pColumnC->SetItemString( i, 1, CTChart::LoadString( TSTR_GUILD_JOB ) );
	m_pColumnC->SetItemString( i, 2, CTChart::LoadString( TSTR_GUILD_LEVEL ) );
	m_pColumnC->SetItemString( i, 3, CTChart::LoadString( TSTR_GUILD_WAR_CONTRIBUTION ) );
	m_pColumnC->SetItemString( i, 4, CTChart::LoadString( TSTR_GUILDOFFER_COL_BENEFIT ) );
	m_pColumnC->SetItemString( i, 5, CTChart::LoadString( TSTR_GUILDOFFER_COL_EARNEST_PERIOD ) );
	m_pColumnC->SetItemString( i, 6, CTChart::LoadString( TSTR_GUILD_NOWPOS ) );
	m_pColumnC->SetTop( 0 );

	m_pColumnD->RemoveAll();
	i = m_pColumnD->AddString( CTChart::LoadString( TSTR_CHAR_NAME ) );
	m_pColumnD->SetItemString( i, 1, CTChart::LoadString( TSTR_GUILD_JOB ) );
	m_pColumnD->SetItemString( i, 2, CTChart::LoadString( TSTR_GUILD_LEVEL ) );
	m_pColumnD->SetItemString( i, 3, CTChart::LoadString( TSTR_GUILD_KILL_H) );
	m_pColumnD->SetItemString( i, 4, CTChart::LoadString( TSTR_GUILD_KILL_E) );
	m_pColumnD->SetItemString( i, 5, CTChart::LoadString( TSTR_GUILD_KILL_L) );
	m_pColumnD->SetItemString( i, 6, CTChart::LoadString( TSTR_GUILD_KILL_GODMON) );
	m_pColumnD->SetItemString( i, 7, CTChart::LoadString( TSTR_GUILD_ENTRY) );
	m_pColumnD->SetItemString( i, 8, CTChart::LoadString( TSTR_GUILD_SUM) );
	m_pColumnD->SetItemString( i, 9, CTChart::LoadString( TSTR_GUILD_SUM_KILL) );
	m_pColumnD->SetItemString( i, 10, CTChart::LoadString( TSTR_GUILD_SUM_DIE) );

	m_nCurPage = TPVPM_REGULAR;
	((TButton*) FindKid(ID_CTRLINST_TAB_1))->Select( TRUE );


	m_pPopup = new CTMiniPopupDlg(this ,pPopupDesc, FALSE);
	m_pPopup->m_id = GetUniqueID();

	m_pListC->ApplyUserColor( TRUE );
}

CTGuildMemberFrame::~CTGuildMemberFrame()
{
	if( m_pPopup )
		delete m_pPopup;
}

void CTGuildMemberFrame::ChangePage( INT nType )
{
	BOOL bFind = FALSE;
	for( INT s=0 ; s < TPVPM_COUNT ; ++s )
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

	for( INT i=0 ; i < TPVPM_COUNT ; ++i )
	{
		switch( m_nTABType[i] )
		{
		case TPVPM_REGULAR:
			m_pTAB[ i ]->m_strText = CTChart::LoadString( TSTR_GUILD_REGULAR );
			break;
		case TPVPM_TACTICS:
			m_pTAB[ i ]->m_strText = CTChart::LoadString( TSTR_GUILD_TACTICS );
			break;
		case TPVPM_LATEST_PVPR:
			m_pTAB[ i ]->m_strText = CTChart::LoadString( TSTR_GUILD_LATEST_PVP );
			break;
		case TPVPM_WEEK_PVPR:
			m_pTAB[ i ]->m_strText = CTChart::LoadString( TSTR_GUILD_WEEK_PVP );
			break;
		}
	}

	m_nCurPage = nType;
	RequestInfo();
}


void CTGuildMemberFrame::ResetTabButton( BYTE bTabIndex )
{	
	TButton *pTAB[TPVPM_COUNT] = {
		(TButton *) FindKid(ID_CTRLINST_TAB_1),
			(TButton *) FindKid(ID_CTRLINST_TAB_2),
			(TButton *) FindKid(ID_CTRLINST_TAB_3),
			(TButton *) FindKid(ID_CTRLINST_TAB_4),
	};

	for( BYTE i=0; i<TPVPM_COUNT; i++)
	{
		pTAB[i]->Select( i == bTabIndex ? TRUE : FALSE);

		/*CGDIFont *pGDIFont = FindFont(pTAB[i]->IsStateDown() ? ID_FONT_LOGIN : ID_FONT_DISABLE);

		if( pTAB[i]->GetFont() != pGDIFont )
			pTAB[i]->SetFont( pGDIFont, NULL);*/
	}

	m_nCurPage = bTabIndex;
	RequestInfo();
}

void CTGuildMemberFrame::RequestInfo()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame )
		return;

	CTClientSession* pSession = pGame->GetSession();
	if( !pSession )
		return;

	CTClientChar* pMainChar = pGame->GetMainChar();
	if( !pMainChar )
		return;

	if( pMainChar->m_dwGuildID == 0 && pMainChar->m_dwTacticsID == 0 )
		return ;

	switch( m_nCurPage )
	{
		case TPVPM_REGULAR:
			pSession->SendCS_GUILDLOCALLIST_REQ();
			pSession->SendCS_GUILDMEMBERLIST_REQ();
			break;
		case TPVPM_TACTICS:
			pSession->SendCS_GUILDLOCALLIST_REQ();
			pSession->SendCS_GUILDTACTICSLIST_REQ();
			break;
		case TPVPM_LATEST_PVPR:
		case TPVPM_WEEK_PVPR:
			pSession->SendCS_GUILDPVPRECORD_REQ();
			break;
	}
}

void CTGuildMemberFrame::ResetInfo()
{
	CTGuildCommander* pGuildCmd = CTGuildCommander::GetInstance();

	switch( m_nCurPage )
	{
	case TPVPM_REGULAR:
		{
			pGuildCmd->SortGuildMember();

			int nTop = m_pListC->GetTop();

			//ResetInfo_Member();
			m_pListC->RemoveAll();

			CTGuildCommander::GuildMemberVec::iterator itMEMBER, end;
			itMEMBER = pGuildCmd->m_GuildMemberVec.begin();
			end = pGuildCmd->m_GuildMemberVec.end();

			m_pMissionTxt->ShowComponent(FALSE);
			m_pMissionTxt->EnableComponent(FALSE);

			for(; itMEMBER != end ; ++itMEMBER )
			{
				int nLine = m_pListC->AddString("");

				DWORD dwColor = 0;
				switch( itMEMBER->m_wCastle )
				{
				case TCASTLE_HESED : dwColor = COLOR_CASTLE_HESED; break;
				case TCASTLE_ARDRI : dwColor = COLOR_CASTLE_ARDRI; break;
				case TCASTLE_GEHBRA : dwColor = COLOR_CASTLE_GEHBRA; break;
				case TCASTLE_TYCON : dwColor = COLOR_CASTLE_TYCON; break;
				default : dwColor = itMEMBER->m_dwTactics ? COLOR_TACTICS : COLOR_CASTLE_NONE; break;
				}

				for(int i=0; i<RINFO_COUNT; ++i)
				{
					switch( i )
					{
					case RINFO_NAME:
						{
							m_pListC->SetItemString( nLine, 0, itMEMBER->m_strName );
							m_pListC->SetItemData( nLine, 0, MAKELONG( nLine, RINFO_NAME) );
							m_pListC->SetUserColor( nLine, 0, dwColor );
						}
						break;
					
					case RINFO_CLASS:
						{
							m_pListC->SetItemString( nLine, 1,
								CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ itMEMBER->m_bClassID] ) );

							m_pListC->SetItemData( nLine, 1, MAKELONG( nLine, RINFO_CLASS) );
							m_pListC->SetUserColor( nLine, 1, dwColor );
						}
						break;

					case RINFO_LEVEL	:
						{
							m_pListC->SetItemString( nLine, 2,
								CTChart::Format( TSTR_FMT_NUMBER, itMEMBER->m_bLevel) );

							m_pListC->SetItemData( nLine, 2, MAKELONG( nLine, RINFO_LEVEL) );
							m_pListC->SetUserColor( nLine, 2, dwColor );
						}
						break;

					case RINFO_SERVICE:
						{
							m_pListC->SetItemString( nLine, 3,
								CTChart::Format( TSTR_FMT_NUMPERCENT, itMEMBER->m_bService) );

							m_pListC->SetItemData( nLine, 3, MAKELONG( nLine, RINFO_SERVICE) );
							m_pListC->SetUserColor( nLine, 3, dwColor );
						}
						break;

					case RINFO_DUTY:
						{
							m_pListC->SetItemString( nLine, 4,
								CTGuildCommander::GetDutyStr( itMEMBER->m_bDuty ) );

							m_pListC->SetItemData( nLine, 4, MAKELONG( nLine, RINFO_DUTY) );
							m_pListC->SetUserColor( nLine, 4, dwColor );
						}
						break;

					case RINFO_PEER:
						{
							CString strPEER;
							if( itMEMBER->m_dwTactics )
							{
								strPEER = CTChart::LoadString( TSTR_GUILD_PEER_TACTICS );
							}
							else
							{
								strPEER = CTGuildCommander::GetPeerageStr( itMEMBER->m_bPeer );
							}

							m_pListC->SetItemString( nLine, 5, strPEER );

							m_pListC->SetItemData( nLine, 5, MAKELONG( nLine, RINFO_PEER) );
							m_pListC->SetUserColor( nLine, 5, dwColor );
						}
						break;

					case RINFO_POSITION	:
						{
							if( itMEMBER->m_bIsConnect )
							{
								LPTREGIONINFO pRegion = CTChart::FindTREGIONINFO( itMEMBER->m_dwRegion);

								if( pRegion )
									m_pListC->SetItemString( nLine, 6, pRegion->m_strNAME);
							}
							else
							{
								m_pListC->SetItemString( nLine, 6, CTChart::LoadString( TSTR_STATE_DISCONNECT));
							}

							m_pListC->SetItemData( nLine, 6, MAKELONG( nLine, RINFO_POSITION) );
							m_pListC->SetUserColor( nLine, 6, dwColor );
						}
						break;
					}
				}
			}

			RemoveKid( m_pListC );
			AddKid( m_pListC );

			RemoveKid( m_pColumnC );
			AddKid( m_pColumnC );

			m_pColumnC->SetTop( 0 );
			m_pListC->SetTopSelItem( nTop );

			m_pMissionTxt->ShowComponent(FALSE);
			m_pMissionTxt->EnableComponent(FALSE);

			m_pMemCntTxt->ShowComponent(TRUE);
			m_pMemCntTxt->EnableComponent(TRUE);

			m_pMemCntTxt->m_strText = CTChart::Format(
				TSTR_FMT_GUILDCOUNTTXT,
				pGuildCmd->GetCountGuildMember() );
		}
		break;

	case TPVPM_TACTICS:
		{
			pGuildCmd->SortTactics();

			int nTop = m_pListC->GetTop();

			m_pListC->RemoveAll();

			CTGuildCommander::GuildTacticsVec::iterator itMEMBER, end;
			itMEMBER = pGuildCmd->m_GuildTacticsVec.begin();
			end = pGuildCmd->m_GuildTacticsVec.end();

			m_pMissionTxt->ShowComponent(FALSE);
			m_pMissionTxt->EnableComponent(FALSE);

			for(; itMEMBER != end ; ++itMEMBER )
			{
				int nLine = m_pListC->AddString("");

				DWORD dwColor = COLOR_CASTLE_NONE;
				switch( itMEMBER->m_wCastle )
				{
				case TCASTLE_HESED : dwColor = COLOR_CASTLE_HESED; break;
				case TCASTLE_ARDRI : dwColor = COLOR_CASTLE_ARDRI; break;
				case TCASTLE_GEHBRA : dwColor = COLOR_CASTLE_GEHBRA; break;
				case TCASTLE_TYCON : dwColor = COLOR_CASTLE_TYCON; break;
				default : dwColor = COLOR_CASTLE_NONE; break;
				}

				for(int i=0; i<TACTICS_INFO_COUNT; ++i)
				{
					CString strText;

					switch( i )
					{
					case TACTICS_INFO_NAME:
						{
							m_pListC->SetItemString( nLine, TACTICS_INFO_NAME, itMEMBER->m_strName );
							m_pListC->SetItemData( nLine, TACTICS_INFO_NAME, MAKELONG( nLine, TACTICS_INFO_NAME) );
							m_pListC->SetUserColor( nLine, TACTICS_INFO_NAME, dwColor );
						}
						break;

					case TACTICS_INFO_LEVEL	:
						{
							m_pListC->SetItemString( nLine, TACTICS_INFO_LEVEL,
									CTChart::Format( TSTR_FMT_NUMBER, itMEMBER->m_bLevel) );
							m_pListC->SetItemData( nLine, TACTICS_INFO_LEVEL, MAKELONG( nLine, TACTICS_INFO_LEVEL) );
							m_pListC->SetUserColor( nLine, TACTICS_INFO_LEVEL, dwColor );
						}
						break;

					case TACTICS_INFO_CLASS:
						{
							m_pListC->SetItemString( nLine, TACTICS_INFO_CLASS,
								CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ itMEMBER->m_bClass] ) );

							m_pListC->SetItemData( nLine, TACTICS_INFO_CLASS, MAKELONG( nLine, TACTICS_INFO_CLASS) );
							m_pListC->SetUserColor( nLine, TACTICS_INFO_CLASS, dwColor );
						}
						break;

					case TACTICS_INFO_DAY:
						{
							/*CTimeSpan timeSpan = CTime(itMEMBER->m_dlEndTime) - CTClientApp::m_dCurDate;
							DWORD dwDays = (DWORD) timeSpan.GetDays();

							CString strFMT;
							strFMT.Format( "%d/%d", dwDays, itMEMBER->m_bDay );*/

							CString strFMT;
							CTime t(itMEMBER->m_dlEndTime);
							strFMT = CTChart::Format( TSTR_GUILD_ENDTIME,
								t.GetYear(),
								t.GetMonth(),
								t.GetDay(),
								t.GetHour(),
								t.GetMinute() );

							m_pListC->SetItemString( nLine, TACTICS_INFO_DAY, strFMT );
							m_pListC->SetItemData( nLine, TACTICS_INFO_DAY, MAKELONG( nLine, TACTICS_INFO_DAY) );
							m_pListC->SetUserColor( nLine, TACTICS_INFO_DAY, dwColor );
						}
						break;

					case TACTICS_INFO_POINT:
						{
							m_pListC->SetItemString( nLine, TACTICS_INFO_POINT,
								CTChart::Format( TSTR_FMT_NUMBER, itMEMBER->m_dwPoint ) );

							m_pListC->SetItemData( nLine, TACTICS_INFO_POINT, MAKELONG( nLine, TACTICS_INFO_POINT) );
							m_pListC->SetUserColor( nLine, TACTICS_INFO_POINT, dwColor );
						}
						break;

					case TACTICS_INFO_GOTPOINT:
						{		
							m_pListC->SetItemString( nLine, TACTICS_INFO_GOTPOINT,
								CTChart::Format( TSTR_FMT_NUMBER, itMEMBER->m_dwGotPoint ) );

							m_pListC->SetItemData( nLine, TACTICS_INFO_GOTPOINT, MAKELONG( nLine, TACTICS_INFO_GOTPOINT) );
							m_pListC->SetUserColor( nLine, TACTICS_INFO_GOTPOINT, dwColor );
						}
						break;

					case TACTICS_INFO_REGION:
						{
							if( itMEMBER->m_dwRegion != 0 )
							{
								LPTREGIONINFO pRegion = CTChart::FindTREGIONINFO( itMEMBER->m_dwRegion);

								if( pRegion )
									m_pListC->SetItemString( nLine, TACTICS_INFO_REGION, pRegion->m_strNAME);
							}
							else
							{
								m_pListC->SetItemString( nLine, TACTICS_INFO_REGION, CTChart::LoadString( TSTR_STATE_DISCONNECT));
							}

							m_pListC->SetItemData( nLine, TACTICS_INFO_REGION, MAKELONG( nLine, TACTICS_INFO_REGION) );
							m_pListC->SetUserColor( nLine, TACTICS_INFO_REGION, dwColor );
						}
						break;
					}
				}
			}

			RemoveKid( m_pListC );
			AddKid( m_pListC );

			RemoveKid( m_pColumnC );
			AddKid( m_pColumnC );

			m_pColumnC->SetTop( 1 );

			m_pListC->SetCurSelItem( nTop );

			m_pMissionTxt->ShowComponent(FALSE);
			m_pMissionTxt->EnableComponent(FALSE);

			m_pMemCntTxt->ShowComponent(TRUE);
			m_pMemCntTxt->EnableComponent(TRUE);

			m_pMemCntTxt->m_strText = CTChart::Format(
				TSTR_FMT_TACTICSCOUNTTXT,
				pGuildCmd->GetCountTactics() );
		}
		break;

	case TPVPM_WEEK_PVPR:
		{
			pGuildCmd->SortWeekPVP();

			m_pListD->RemoveAll();

			CTGuildCommander::GuildWeekPvPVec::iterator itMEMBER, end;
			itMEMBER = pGuildCmd->m_GuildWeekPvPVec.begin();
			end = pGuildCmd->m_GuildWeekPvPVec.end();

			for(; itMEMBER != end ; ++itMEMBER )
			{
				int nLine = m_pListD->AddString("");

				CTGuildCommander::GuildMember* pCHAR =
					pGuildCmd->FindGuildMemberByID( itMEMBER->m_dwCharID );

				for(int i=0; i<WEEK_INFO_COUNT; ++i)
				{
					CString strText;

					switch( i )
					{
					case WEEK_INFO_NAME:
						{
							if( pCHAR )
								m_pListD->SetItemString( nLine, 0, pCHAR->m_strName );
							m_pListD->SetItemData( nLine, 0, MAKELONG( nLine, WEEK_INFO_NAME) );
						}
						break;
					case WEEK_INFO_JOB:
						{
							if( pCHAR )
								m_pListD->SetItemString( nLine, 1,
									CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ pCHAR->m_bClassID] ) );
							m_pListD->SetItemData( nLine, 1, MAKELONG( nLine, WEEK_INFO_JOB) );
						}
						break;
					case WEEK_INFO_LEVEL:
						{
							if( pCHAR )
							{
								CString strFMT;
								strFMT.Format( "%d", pCHAR->m_bLevel );
								m_pListD->SetItemString( nLine, 2, strFMT );
							}

							m_pListD->SetItemData( nLine, 2, MAKELONG( nLine, WEEK_INFO_LEVEL) );
						}
						break;
					case WEEK_INFO_KILLPOINT_H:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwKillPoint_H_W );
							m_pListD->SetItemString( nLine, 3, strFMT );
							m_pListD->SetItemData( nLine, 3, MAKELONG( nLine, WEEK_INFO_KILLPOINT_H) );
						}
						break;
					case WEEK_INFO_KILLPOINT_E:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwKillPoint_E_W );
							m_pListD->SetItemString( nLine, 4, strFMT );
							m_pListD->SetItemData( nLine, 4, MAKELONG( nLine, WEEK_INFO_KILLPOINT_E) );
						}
						break;
					case WEEK_INFO_KILLPOINT_L:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwKillPoint_L_W );
							m_pListD->SetItemString( nLine, 5, strFMT );
							m_pListD->SetItemData( nLine, 5, MAKELONG( nLine, WEEK_INFO_KILLPOINT_L) );
						}
						break;
					case WEEK_INFO_GODMONKILLPOINT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwGodMonKillPoint_W );
							m_pListD->SetItemString( nLine, 6, strFMT );
							m_pListD->SetItemData( nLine, 6, MAKELONG( nLine, WEEK_INFO_GODMONKILLPOINT) );
						}
						break;
					case WEEK_INFO_ENTRYPOINT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwEntryPoint_W );
							m_pListD->SetItemString( nLine, 7, strFMT );
							m_pListD->SetItemData( nLine, 7, MAKELONG( nLine, WEEK_INFO_ENTRYPOINT) );
						}
						break;
					case WEEK_INFO_RESULT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwResult );
							m_pListD->SetItemString( nLine, 8, strFMT );
							m_pListD->SetItemData( nLine, 8, MAKELONG( nLine, WEEK_INFO_RESULT) );
						}
						break;
					case WEEK_INFO_KILLCOUNT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_wKillCount_W );
							m_pListD->SetItemString( nLine, 9, strFMT );
							m_pListD->SetItemData( nLine, 9, MAKELONG( nLine, WEEK_INFO_KILLCOUNT) );
						}
						break;
					case WEEK_INFO_DIECOUNT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_wDieCount_W );
							m_pListD->SetItemString( nLine, 10, strFMT );
							m_pListD->SetItemData( nLine, 10, MAKELONG( nLine, WEEK_INFO_DIECOUNT) );
						}
						break;
					}
				}
			}

			RemoveKid( m_pListD );
			AddKid( m_pListD );

			RemoveKid( m_pColumnD );
			AddKid( m_pColumnD );

			m_pColumnD->SetTop( 1 );

			m_pMissionTxt->ShowComponent(FALSE);
			m_pMissionTxt->EnableComponent(FALSE);
			m_pMissionTxt->m_strText.Empty();

			m_pMemCntTxt->ShowComponent(FALSE);
			m_pMemCntTxt->EnableComponent(FALSE);
			m_pMemCntTxt->m_strText.Empty();
		}
		break;

	case TPVPM_LATEST_PVPR:
		{
			pGuildCmd->SortLatestPVP();

			m_pListD->RemoveAll();

			CTGuildCommander::GuildLatestPvPVec::iterator itMEMBER, end;
			itMEMBER = pGuildCmd->m_GuildLatestPvPVec.begin();
			end = pGuildCmd->m_GuildLatestPvPVec.end();

			for(; itMEMBER != end ; ++itMEMBER )
			{
				int nLine = m_pListD->AddString("");

				CTGuildCommander::GuildMember* pCHAR =
					pGuildCmd->FindGuildMemberByID( itMEMBER->m_dwCharID );

				for(int i=0; i<LATEST_INFO_COUNT; ++i)
				{
					CString strText;

					switch( i )
					{
					case LATEST_INFO_NAME:
						{
							if( pCHAR )
								m_pListD->SetItemString( nLine, 0, pCHAR->m_strName );
							m_pListD->SetItemData( nLine, 0, MAKELONG( nLine, LATEST_INFO_NAME) );
						}
						break;
					case LATEST_INFO_JOB:
						{
							if( pCHAR )
								m_pListD->SetItemString( nLine, 1, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ pCHAR->m_bClassID] ) );
							m_pListD->SetItemData( nLine, 1, MAKELONG( nLine, LATEST_INFO_JOB) );
						}
						break;
					case LATEST_INFO_LEVEL:
						{
							if( pCHAR )
							{
								CString strFMT;
								strFMT.Format( "%d", pCHAR->m_bLevel );
								m_pListD->SetItemString( nLine, 2, strFMT );
							}

							m_pListD->SetItemData( nLine, 2, MAKELONG( nLine, LATEST_INFO_LEVEL) );
						}
						break;
					case LATEST_INFO_KILLPOINT_H:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwKillPoint_H_D );
							m_pListD->SetItemString( nLine, 3, strFMT );
							m_pListD->SetItemData( nLine, 3, MAKELONG( nLine, LATEST_INFO_KILLPOINT_H) );
						}
						break;
					case LATEST_INFO_KILLPOINT_E:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwKillPoint_E_D );
							m_pListD->SetItemString( nLine, 4, strFMT );
							m_pListD->SetItemData( nLine, 4, MAKELONG( nLine, LATEST_INFO_KILLPOINT_E) );
						}
						break;
					case LATEST_INFO_KILLPOINT_L:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwKillPoint_L_D );
							m_pListD->SetItemString( nLine, 5, strFMT );
							m_pListD->SetItemData( nLine, 5, MAKELONG( nLine, LATEST_INFO_KILLPOINT_L) );
						}
						break;
					case LATEST_INFO_GODMONKILLPOINT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwGodMonKillPoint_D );
							m_pListD->SetItemString( nLine, 6, strFMT );
							m_pListD->SetItemData( nLine, 6, MAKELONG( nLine, LATEST_INFO_GODMONKILLPOINT) );
						}
						break;
					case LATEST_INFO_ENTRYPOINT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwEntryPoint_D );
							m_pListD->SetItemString( nLine, 7, strFMT );
							m_pListD->SetItemData( nLine, 7, MAKELONG( nLine, LATEST_INFO_ENTRYPOINT) );
						}
						break;
					case LATEST_INFO_RESULT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_dwResult );
							m_pListD->SetItemString( nLine, 8, strFMT );
							m_pListD->SetItemData( nLine, 8, MAKELONG( nLine, LATEST_INFO_RESULT) );
						}
						break;
					case LATEST_INFO_KILLCOUNT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_wKillCount_D );
							m_pListD->SetItemString( nLine, 9, strFMT );
							m_pListD->SetItemData( nLine, 9, MAKELONG( nLine, LATEST_INFO_KILLCOUNT) );
						}
						break;
					case LATEST_INFO_DIECOUNT:
						{
							CString strFMT;
							strFMT.Format("%d", itMEMBER->m_wDieCount_D );
							m_pListD->SetItemString( nLine, 10, strFMT );
							m_pListD->SetItemData( nLine, 10, MAKELONG( nLine, LATEST_INFO_DIECOUNT) );
						}
						break;
					}
				}
			}

			RemoveKid( m_pListD );
			AddKid( m_pListD );

			RemoveKid( m_pColumnD );
			AddKid( m_pColumnD );

			m_pColumnD->SetTop( 1 );

			m_pMissionTxt->ShowComponent(FALSE);
			m_pMissionTxt->EnableComponent(FALSE);
			m_pMissionTxt->m_strText.Empty();

			m_pMemCntTxt->ShowComponent(FALSE);
			m_pMemCntTxt->EnableComponent(FALSE);
			m_pMemCntTxt->m_strText.Empty();
		}
		break;
	}
}

HRESULT CTGuildMemberFrame::Render( DWORD dwTickCount )
{
	//if( m_bGuildEnable )
	{
		switch( m_nCurPage )
		{
		case TPVPM_REGULAR:
		case TPVPM_TACTICS:
			{
				m_pColumnD->ShowComponent(FALSE);
				m_pColumnD->EnableComponent(FALSE);
				m_pListD->ShowComponent(FALSE);
				m_pListD->EnableComponent(FALSE);

				RemoveKid(m_pColumnC);
				RemoveKid(m_pListC);
				AddKid(m_pListC);
				AddKid(m_pColumnC);
				
				m_pColumnC->ShowComponent();
				m_pColumnC->EnableComponent();
				m_pListC->ShowComponent();
				m_pListC->EnableComponent();

				if( m_nCurPage == TPVPM_REGULAR )
					m_pColumnC->SetTop( 0 );
				else if( m_nCurPage == TPVPM_TACTICS )
					m_pColumnC->SetTop( 1 );
			}
			break;

		case TPVPM_LATEST_PVPR:
		case TPVPM_WEEK_PVPR:
			{
				m_pColumnC->ShowComponent(FALSE);
				m_pColumnC->EnableComponent(FALSE);
				m_pListC->ShowComponent(FALSE);
				m_pListC->EnableComponent(FALSE);

				m_pColumnD->ShowComponent();
				m_pColumnD->EnableComponent();
				m_pListD->ShowComponent();
				m_pListD->EnableComponent();

				RemoveKid(m_pColumnD);
				RemoveKid(m_pListD);
				AddKid(m_pListD);
				AddKid(m_pColumnD);

				m_pColumnD->SetTop( 0 );
			}
			break;
		}

		if( IsPopupVisible() )
		{
			RemoveKid( m_pPopup );
			AddKid( m_pPopup );
		}
	}
	/*else
	{
		m_pColumnC->ShowComponent(FALSE);
		m_pColumnC->EnableComponent(FALSE);
		m_pListC->ShowComponent(FALSE);
		m_pListC->EnableComponent(FALSE);

		m_pColumnD->ShowComponent(FALSE);
		m_pColumnD->EnableComponent(FALSE);
		m_pListD->ShowComponent(FALSE);
		m_pListD->EnableComponent(FALSE);
		
		m_bPopupVisible = FALSE;
		m_pPopup->ShowComponent( FALSE );
		
		m_pMissionTxt->ShowComponent( FALSE);
		m_pMemCntTxt->ShowComponent( FALSE);
	}*/
	return CTClientUIBase::Render( dwTickCount );
}

void CTGuildMemberFrame::OnLButtonDown( UINT nFlags, CPoint pt )
{
	if( !IsPopupVisible() || !m_pPopup->HitTest(pt) )
	{
		switch( m_nCurPage )
		{
			case TPVPM_REGULAR:
			case TPVPM_TACTICS:
				{
					m_pDownItem = m_pListC->GetHitItem( pt);
				}
				break;

			case TPVPM_LATEST_PVPR:
			case TPVPM_WEEK_PVPR:
				{
					m_pDownItem = m_pListD->GetHitItem( pt);
				}
				break;
		}
	}

	CTClientUIBase::OnLButtonDown( nFlags, pt );
}

void CTGuildMemberFrame::OnLButtonUp( UINT nFlags, CPoint pt )
{
	CTClientUIBase::OnLButtonUp( nFlags, pt );

	TListItem* pDownItem = m_pDownItem;
	m_pDownItem = NULL;

	if( !IsPopupVisible() || !m_pPopup->HitTest(pt) )
	{
		TListItem* pItem = NULL;

		switch( m_nCurPage )
		{
			case TPVPM_REGULAR:
			case TPVPM_TACTICS:
				{
					pItem = m_pListC->GetHitItem( pt);
				}
				break;

			case TPVPM_LATEST_PVPR:
			case TPVPM_WEEK_PVPR:
				{
					pItem = m_pListD->GetHitItem( pt);
				}
				break;
		};

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

void CTGuildMemberFrame::ShowComponent(BOOL bVisible)
{

	BYTE bMissionTxt = m_pMissionTxt->IsEnable();
	BYTE bMemCntTxt = m_pMemCntTxt->IsEnable();
	
	CTClientUIBase::ShowComponent( bVisible );

	m_pMemCntTxt->ShowComponent(bMemCntTxt);
	m_pMissionTxt->ShowComponent(bMissionTxt);

}

void CTGuildMemberFrame::ShowPopup(const CPoint& pt)
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return;

	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildDetInfo& detif = pCmd->GetGuildDetInfo();

	for(INT i=0; i<TGM_PBTN_COUNT; ++i)
	{
		BOOL bADD = FALSE;

		switch(i)
		{
		case TGM_PBTN_WHISPER		:
		case TGM_PBTN_MAIL			:
			{
				if( m_nCurPage == TPVPM_REGULAR && m_nLastSelType == RINFO_NAME )
				{
					const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
					if( info.m_bIsConnect || i == TGM_PBTN_MAIL )
					{
						if( pMainChar->m_strNAME != info.m_strName )
							bADD = TRUE;
					}
				}
				else if( m_nCurPage == TPVPM_TACTICS && m_nLastSelType == TACTICS_INFO_NAME  )
					bADD = TRUE;
			}
			break;

		case TGM_PBTN_POINT_REWARD:
			{
				if( m_nCurPage == TPVPM_REGULAR )
				{
					if( m_nLastSelType == RINFO_NAME )
					{
						if( pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF )
						{
							bADD = TRUE;
						}
					}
				}
			}
			break;

		case TGM_PBTN_PARTY			:
			if( m_nLastSelType == RINFO_NAME )
			{
				if( m_nCurPage == TGM_REGULAR )
				{
					const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
					if ( info.m_bIsConnect && pMainChar->m_strNAME != info.m_strName )
					{
						WORD wSquadID = pGame->GetPartyID(pMainChar);
						if( (!wSquadID || (
							pMainChar->m_dwChiefID == pMainChar->m_dwID )) )
							bADD = TRUE;
					}
				}
				else if( m_nCurPage == TGM_TACTICS )
				{
					GuildTactics* pInfo = pCmd->GetTacticsPtr( m_nLastSelIdx );
					if( pInfo->m_dwRegion != 0 )
					{
						WORD wSquadID = pGame->GetPartyID(pMainChar);
						if( (!wSquadID || (
							pMainChar->m_dwChiefID == pMainChar->m_dwID )) )
							bADD = TRUE;
					}
				}
			}

			break;

		case TGM_PBTN_FRIEND			:
			if( m_nLastSelType == RINFO_NAME )
			{
				if( m_nCurPage == TGM_REGULAR )
				{
					const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
					if ( info.m_bIsConnect && pMainChar->m_strNAME != info.m_strName )
					{
						bADD = TRUE;
					}
				}
				else if( m_nCurPage == TGM_TACTICS )
				{
					GuildTactics* pInfo = pCmd->GetTacticsPtr( m_nLastSelIdx );
					if( pInfo->m_dwRegion != 0 )
					{
						bADD = TRUE;
					}
				}
			}
			break;

		case TGM_PBTN_KICK			:
			if( m_nCurPage == TPVPM_REGULAR && m_nLastSelType == RINFO_NAME )
			{
				const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);

				if( pMainChar->m_strNAME != info.m_strName )
				{
					if( pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF )
						bADD = TRUE;
					else if( pMainChar->m_bGuildDuty == GUILD_DUTY_VICECHIEF )
					{
						if( info.m_strName != detif.m_strGMaster )
							bADD = TRUE;
					}
				}
			}
			break;

		case TGM_PBTN_SET_GM		:	
		case TGM_PBTN_SET_GSM		:	
		case TGM_PBTN_CANCEL_GSM	:	
			if( m_nCurPage == TPVPM_REGULAR && m_nLastSelType == RINFO_DUTY )
			{
				const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);

				if( pMainChar->m_strNAME != info.m_strName &&
					pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF )
				{
					bADD = TRUE;
				}
			}
			break;

		case TGM_PBTN_PEER_BARON	:	
		case TGM_PBTN_PEER_VISCOUNT	:	
		case TGM_PBTN_PEER_COUNT	:	
		case TGM_PBTN_PEER_MARQUIS	:
		case TGM_PBTN_PEER_DUKE		:	
		case TGM_PBTN_CANCEL_PEER	:	
			if( m_nCurPage == TPVPM_REGULAR && m_nLastSelType == RINFO_PEER )
			{
				const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);

				if( pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF )
				{
					bADD = TRUE;
				}
			}
			break;
		case TGM_PBTN_VOLUNTEER_CANCEL	:
			if( pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF && m_nLastSelType == RINFO_NAME )
			{
				if( m_nCurPage == TPVPM_REGULAR )
				{
					const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
					if( info.m_wCastle )
						bADD = TRUE;
				}
				else if( m_nCurPage == TPVPM_TACTICS )
				{
					GuildTactics* info = pCmd->GetTacticsPtr( m_nLastSelIdx );
					if( info->m_wCastle )
						bADD = TRUE;
				}
			}
			break;
		case TGM_PBTN_VOLUNTEER_HESED : bADD = ShowPopupCastle( TCASTLE_HESED ); break;
		case TGM_PBTN_VOLUNTEER_ARDRI : bADD = ShowPopupCastle( TCASTLE_ARDRI ); break;
		case TGM_PBTN_VOLUNTEER_GEHBRA : bADD = ShowPopupCastle( TCASTLE_GEHBRA ); break;
		case TGM_PBTN_VOLUNTEER_TYCON : bADD = ShowPopupCastle( TCASTLE_TYCON ); break;

		case TGM_PBTN_TACTICS_RE_CONTRACT: ///< 용병 재계약
			if( m_nCurPage == TPVPM_TACTICS && m_nLastSelType == RINFO_NAME )
			{
				if( pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF )
				{
					bADD = TRUE;
				}
			}
			break;

		case TGM_PBTN_TACTICS_KICK: ///< 용병 계약 파기
			if( m_nCurPage == TPVPM_TACTICS && m_nLastSelType == RINFO_NAME )
			{
				if( pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF )
				{
					bADD = TRUE;
				}
			}
			break;

		/*case TGM_PBTN_TACTICS_REWARD_POINT: ///< 용병 포상
			if( m_nCurPage == TPVPM_TACTICS && m_nLastSelType == RINFO_NAME )
			{
				if( pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF )
				{
					bADD = TRUE;
				}
			}
			break;*/
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
		strTITLE = CTChart::LoadString( (TSTRING) m_PopupBtnInfo[TGM_PBTN_CANCEL].m_dwTXT);
		m_pPopup->AddButton(strTITLE, m_PopupBtnInfo[TGM_PBTN_CANCEL].m_dwGM);
	
		AddKid(m_pPopup);

		CRect rc = m_rc;
		ComponentToScreen(&rc);
		CPoint ptPOP = pt - rc.TopLeft();

		m_pPopup->MoveComponent(ptPOP);
		m_pPopup->ShowComponent(TRUE);

		m_bPopupVisible = TRUE;
	}
}
BOOL CTGuildMemberFrame::ShowPopupCastle( WORD wCastleID )
{
	if( wCastleID <= 0 )
		return FALSE;

	if( m_nLastSelType != RINFO_NAME )
		return FALSE;
	
	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	size_t index = ( wCastleID / TCASTLE_COUNT ) - 1;
	const CTGuildCommander::Territory &Terr = pCmd->GetTerritory( index );

	WORD wMyCastle = 0;
	if( m_nCurPage == TPVPM_REGULAR )
	{
		const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
		wMyCastle = info.m_wCastle;
	}
	else if( m_nCurPage == TPVPM_TACTICS )
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr( m_nLastSelIdx );
		wMyCastle = pInfo->m_wCastle;
	}

	if( pMainChar->m_bGuildDuty == GUILD_DUTY_CHIEF &&
		(pMainChar->m_strGUILD == Terr.m_strDefGuild || pMainChar->m_strGUILD == Terr.m_strAtkGuild) &&
		Terr.m_bCanApplyCastle == TRUE )
	{
		if( wMyCastle != wCastleID )
			return TRUE;
	}

	return FALSE;
}
// ----------------------------------------------------------------------
void CTGuildMemberFrame::CancelPopup()
{
	RemoveKid(m_pPopup);
	m_pPopup->ClearButtons();
	m_bPopupVisible = FALSE;
}
// ----------------------------------------------------------------------
BOOL CTGuildMemberFrame::IsPopupVisible() const
{
	return m_bPopupVisible;
}
int CTGuildMemberFrame::OnGM_GMP_WHISPER()
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	CString strCHAR;
	if( m_nCurPage == TPVPM_REGULAR )
	{
		const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
		strCHAR = info.m_strName;
	}
	else
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr(m_nLastSelIdx);
		strCHAR = pInfo->m_strName;
	}

	CTClientGame* pGame = CTClientGame::GetInstance();
	CTChatFrame* pChat = pGame->GetChatFrame();
	pChat->SetChatMode(CHAT_WHISPER, strCHAR);

	if( !pChat->IsChatEditON() )
		pGame->EnableChat(TRUE);

	return TERR_NONE;
}		
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_MAIL()
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame->CanUseMail() )
		return TERR_MAIL_REGION;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	CString strCHAR;
	if( m_nCurPage == TPVPM_REGULAR )
	{
		const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
		strCHAR = info.m_strName;
	}
	else
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr(m_nLastSelIdx);
		strCHAR = pInfo->m_strName;
	}

	CTMailSendDlg* pSndDlg = static_cast<CTMailSendDlg*>(pGame->GetFrame(TFRAME_MAIL_SEND));
	pSndDlg->SetReceiver(strCHAR);
	pSndDlg->SetTitle("");

	pGame->EnableUI(TFRAME_MAIL_SEND);

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_PARTY()
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = pGame->GetMainChar();

	BYTE bObtainType;
	if( pGame->GetPartyID(pMainChar) )
		bObtainType = pGame->GetPartyItemType();
	else
		bObtainType = PT_ORDER;

	if( m_nCurPage == TPVPM_REGULAR )
	{
		const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
		pGame->GetSession()->SendCS_PARTYADD_REQ( info.m_strName, bObtainType);
	}
	else if( m_nCurPage == TPVPM_TACTICS )
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr( m_nLastSelIdx );
		pGame->GetSession()->SendCS_PARTYADD_REQ( pInfo->m_strName, bObtainType);
	}

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_FRIEND()
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	if( m_nCurPage == TPVPM_REGULAR )
	{
		const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
		CTMessengerCommander::GetInstance()->DoAddFriend( info.m_strName );
	}
	else if( m_nCurPage == TPVPM_TACTICS )
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr( m_nLastSelIdx );
		CTMessengerCommander::GetInstance()->DoAddFriend( pInfo->m_strName );
	}

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_KICK()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildKickOut(info.m_strName, TRUE);

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_DISORG_TACTICS()
{
	CancelPopup();

	/// 미구현

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_ET_GM()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildDuty(info.m_strName, GUILD_DUTY_CHIEF);

	return TERR_NONE;
}	
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_SET_GSM()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildDuty(info.m_strName, GUILD_DUTY_VICECHIEF);

	return TERR_NONE;
}	
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_CANCEL_GSM()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildDuty(info.m_strName, GUILD_DUTY_NONE);

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_PEER_BARON()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildPeer(info.m_strName, GUILD_PEER_BARON);

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_PEER_VISCOUNT()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildPeer(info.m_strName, GUILD_PEER_VISCOUNT);

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_PEER_COUNT()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildPeer(info.m_strName, GUILD_PEER_COUNT);

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_PEER_MARQUIS()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildPeer(info.m_strName, GUILD_PEER_MARQUIS);

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_PEER_DUKE()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildPeer(info.m_strName, GUILD_PEER_DUKE);

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_POINT_REWARD()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildPointRewardFrame* pDLG = (CTGuildPointRewardFrame*)
		CTClientGame::GetInstance()->GetFrame( TFRAME_GUILDPOINTREWARD );

	pDLG->m_pGuildPoint->m_strText.Format( "%d",
		CTGuildCommander::GetInstance()->GetGuildDetInfo().m_dwPvPUseablePoint );

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	
	pDLG->m_pTargetEdit->ClearText();
	pDLG->m_pPointEdit->ClearText();
	pDLG->m_pMessage->ClearText();

	pDLG->m_pTargetEdit->SetText( pCmd->GetGuildMember( m_nLastSelIdx ).m_strName );

	pDLG->SwitchFocus( pDLG->m_pTargetEdit );
	pDLG->m_pTargetEdit->SetFocus(TRUE);

	CTClientGame::GetInstance()->EnableUI( TFRAME_GUILDPOINTREWARD );

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_CANCEL_PEER()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_REGULAR || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const GuildMember& info = pCmd->GetGuildMember(m_nLastSelIdx);
	pCmd->RequestGuildPeer(info.m_strName, GUILD_PEER_NONE);

	return TERR_NONE;
}
// ----------------------------------------------------------------------
int CTGuildMemberFrame::OnGM_GMP_CANCEL()
{
	CancelPopup();

	return TERR_NONE;
}

/*void CTGuildMemberFrame::EnableGuildUI( BOOL bEnable )
{
	m_bGuildEnable = bEnable;
}*/

int CTGuildMemberFrame::OnGM_GMP_VOLUNTEER_HESED()
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( m_nCurPage == TPVPM_REGULAR )
	{
		GuildMember* info = pCmd->GetGuildMemberPtr(m_nLastSelIdx);
		info->m_wCastle = TCASTLE_HESED;
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( info->m_wCastle, info->m_dwCharID );
	}
	else if( m_nCurPage == TPVPM_TACTICS )
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr(m_nLastSelIdx);
		pInfo->m_wCastle = TCASTLE_HESED;
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( pInfo->m_wCastle, pInfo->m_dwCharID );
	}

//	ResetInfo();

	return TERR_NONE;
}

int CTGuildMemberFrame::OnGM_GMP_VOLUNTEER_ARDRI()
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( m_nCurPage == TPVPM_REGULAR )
	{
		GuildMember* info = pCmd->GetGuildMemberPtr(m_nLastSelIdx);
		info->m_wCastle = TCASTLE_ARDRI;
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( info->m_wCastle, info->m_dwCharID );
	}
	else if( m_nCurPage == TPVPM_TACTICS )
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr(m_nLastSelIdx);
		pInfo->m_wCastle = TCASTLE_ARDRI;
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( pInfo->m_wCastle, pInfo->m_dwCharID );
	}

//	ResetInfo();

	return TERR_NONE;
}

int CTGuildMemberFrame::OnGM_GMP_VOLUNTEER_TYCON()
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( m_nCurPage == TPVPM_REGULAR )
	{
		GuildMember* info = pCmd->GetGuildMemberPtr(m_nLastSelIdx);
		info->m_wCastle = TCASTLE_TYCON;
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( info->m_wCastle, info->m_dwCharID );
	}
	else if( m_nCurPage == TPVPM_TACTICS )
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr(m_nLastSelIdx);
		pInfo->m_wCastle = TCASTLE_TYCON;
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( pInfo->m_wCastle, pInfo->m_dwCharID );
	}

//	ResetInfo();

	return TERR_NONE;
}

int CTGuildMemberFrame::OnGM_GMP_VOLUNTEER_GEHBRA()
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( m_nCurPage == TPVPM_REGULAR )
	{
		GuildMember* info = pCmd->GetGuildMemberPtr(m_nLastSelIdx);
		info->m_wCastle = TCASTLE_GEHBRA;
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( info->m_wCastle, info->m_dwCharID );
	}
	else if( m_nCurPage == TPVPM_TACTICS )
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr(m_nLastSelIdx);
		pInfo->m_wCastle = TCASTLE_GEHBRA;
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( pInfo->m_wCastle, pInfo->m_dwCharID );
	}

//	ResetInfo();

	return TERR_NONE;
}

int CTGuildMemberFrame::OnGM_GMP_VOLUNTEER_CANCEL()
{
	CancelPopup();

	if( m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();

	if( m_nCurPage == TPVPM_REGULAR )
	{
		GuildMember* info = pCmd->GetGuildMemberPtr(m_nLastSelIdx);
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( info->m_wCastle, info->m_dwCharID );
	}
	else if( m_nCurPage == TPVPM_TACTICS )
	{
		GuildTactics* pInfo = pCmd->GetTacticsPtr(m_nLastSelIdx);
		pGame->GetSession()->SendCS_CASTLEAPPLY_REQ( pInfo->m_wCastle, pInfo->m_dwCharID );
	}

//	ResetInfo();

	return TERR_NONE;
}

int CTGuildMemberFrame::OnGM_GMP_TACTICS_RE_CONTRACT()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_TACTICS || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	GuildTactics* pInfo = pCmd->GetTacticsPtr(m_nLastSelIdx);

	CTClientGame* pGAME = CTClientGame::GetInstance();

	if( !pInfo->m_strName.IsEmpty() )
	{
		CTTacticsInviteDlg* pDLG = static_cast<CTTacticsInviteDlg*>( pGAME->GetFrame( TFRAME_INVITE_TACTICS ) );
		pDLG->SetReInviteCond( pInfo->m_strName );
		pGAME->EnableUI( TFRAME_INVITE_TACTICS );
	}

	return TERR_NONE;
}

int CTGuildMemberFrame::OnGM_GMP_TACTICS_KICK()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_TACTICS || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	GuildTactics* pInfo = pCmd->GetTacticsPtr(m_nLastSelIdx);

	pCmd->RequestGuildTacticsKickOut(
		pInfo->m_strName,
		pInfo->m_dwCharID,
		TRUE );

	return TERR_NONE;
}

int CTGuildMemberFrame::OnGM_GMP_TACTICS_REWARD_POINT()
{
	CancelPopup();

	if( m_nCurPage != TPVPM_TACTICS || m_nLastSelIdx == T_INVALID )
		return TERR_NONE;

	CTGuildPointRewardFrame* pDLG = (CTGuildPointRewardFrame*)
		CTClientGame::GetInstance()->GetFrame( TFRAME_GUILDPOINTREWARD );

	pDLG->m_pGuildPoint->m_strText.Format( "%d",
		CTGuildCommander::GetInstance()->GetGuildDetInfo().m_dwPvPUseablePoint );

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	
	pDLG->m_pTargetEdit->ClearText();
	pDLG->m_pPointEdit->ClearText();
	pDLG->m_pMessage->ClearText();

	pDLG->m_pTargetEdit->SetText( pCmd->GetTacticsPtr( m_nLastSelIdx )->m_strName );

	pDLG->SwitchFocus( pDLG->m_pTargetEdit );
	pDLG->m_pTargetEdit->SetFocus(TRUE);

	CTClientGame::GetInstance()->EnableUI( TFRAME_GUILDPOINTREWARD );

	return TERR_NONE;
}