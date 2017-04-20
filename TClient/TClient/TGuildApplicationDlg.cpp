#include "Stdafx.h"
#include "TGuildApplicationDlg.h"
#include "TClientGame.h"
#include "TCommunityDlg.h"
#include "TGuildRegularOffer.h"
#include "TGuildTacticsOffer.h"
#include "TClient.h"

bool _compare_guildoffer_vol(const CTGuildCommander::GuildOffer& l, const CTGuildCommander::GuildOffer& r)
{
	return l.m_bApplied > r.m_bApplied;
}

bool _compare_tacticsoffer_vol(const CTGuildCommander::GuildTacticsOffer& l, const CTGuildCommander::GuildTacticsOffer& r)
{
	return l.m_bApplied > r.m_bApplied;
}

// CTGuildApplicationDlg
// ======================================================================
CTGuildApplicationDlg::CTGuildApplicationDlg(TComponent *pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc),
m_nCurPage( TGM_REGULAR ),
m_nLastSelType(T_INVALID),
m_nLastSelIdx(T_INVALID)
{
	m_pList = (TList*) FindKid( ID_CTRLINST_LIST );
	m_pColText[0] = FindKid( ID_CTRLINST_COL1 );
	m_pColText[1] = FindKid( ID_CTRLINST_COL2 );
	m_pColText[2] = FindKid( ID_CTRLINST_COL3 );
	m_pColText[3] = FindKid( ID_CTRLINST_COL4 );
	m_pColText[4] = FindKid( ID_CTRLINST_COL5 );
	m_pColText[5] = FindKid( ID_CTRLINST_COL6 );
	m_pColText[6] = FindKid( ID_CTRLINST_COL7 );
	m_pColText[7] = FindKid( ID_CTRLINST_COL8 );

	m_pButton[0] = (TButton*) FindKid( ID_CTRLINST_TAB1 );
	m_pButton[1] = (TButton*) FindKid( ID_CTRLINST_TAB2 );
	m_pButton[0]->SetStyle( m_pButton[0]->GetStyle() | TBS_STATE_BUTTON );
	m_pButton[1]->SetStyle( m_pButton[1]->GetStyle() | TBS_STATE_BUTTON );

	m_pList->SetColumnAlign( 2, ALIGN_CENTER );
	m_pList->SetColumnAlign( 3, ALIGN_CENTER );
	m_pList->SetColumnAlign( 4, ALIGN_CENTER );
	m_pList->SetColumnAlign( 5, ALIGN_CENTER );
	m_pList->SetColumnAlign( 6, ALIGN_CENTER );
	m_pList->SetColumnAlign( 7, ALIGN_CENTER );

	m_pList->LastItemToFront();
	m_pText = FindKid( ID_CTRLINST_TEXT );
	m_pBtnJOIN = (TButton*) FindKid( ID_CTRLINST_BTN_JOIN );
	m_pBtnJOINCANCEL = (TButton*) FindKid( ID_CTRLINST_BTN_JOINCANCEL );

	TButton* pBTN = (TButton*) FindKid( ID_CTRLINST_BTN_WRITE );
	pBTN->m_strText = CTChart::LoadString( TSTR_GUILD_WANTED_NEW );
	pBTN = (TButton*) FindKid( ID_CTRLINST_BTN_MODIFY );
	pBTN->m_strText = CTChart::LoadString( TSTR_GUILD_WANTED_MODIFY );
	pBTN = (TButton*) FindKid( ID_CTRLINST_BTN_DELETE );
	pBTN->m_strText = CTChart::LoadString( TSTR_GUILD_WANTED_DEL );
}
// ----------------------------------------------------------------------
CTGuildApplicationDlg::~CTGuildApplicationDlg()
{
}
// ======================================================================

// ======================================================================
TEdit* CTGuildApplicationDlg::GetCurEdit()
{
	/*FrameInfoArray::iterator itr,end;
	itr = m_FrameInfoArray.begin();
	end = m_FrameInfoArray.end();n

	for( ;itr!=end; ++itr)
	{
		FrameInfo* pInfo = &(*itr);
		TEdit* pEdit = static_cast<CTGuildAppInnerFrame*>(pInfo->m_pFrameCtrl)->GetCurEdit();
		if( pEdit )
			return pEdit;
	}*/

	return NULL;
}
// ----------------------------------------------------------------------
BOOL CTGuildApplicationDlg::IsEditMode()
{
	/*FrameInfoArray::iterator itr,end;
	itr = m_FrameInfoArray.begin();
	end = m_FrameInfoArray.end();

	for( ;itr!=end; ++itr)
	{
		FrameInfo* pInfo = &(*itr);
		if( static_cast<CTGuildAppInnerFrame*>(pInfo->m_pFrameCtrl)->IsEditMode() )
			return TRUE;
	}*/

	return FALSE;
}
// ======================================================================
void CTGuildApplicationDlg::RequestInfo()
{
	CTClientSession* pSESSION = CTClientGame::GetInstance()->GetSession();
	if( !pSESSION )
		return ;

	time_t t;
	time( &t );

	switch( m_nCurPage )
	{
	case TGM_REGULAR:
		{
			pSESSION->SendCS_GUILDWANTEDLIST_REQ();
		}
		break;
	case TGM_TACTICS:
		{
			pSESSION->SendCS_GUILDTACTICSWANTEDLIST_REQ();
		}
		break;
	}
}
// ======================================================================
void CTGuildApplicationDlg::ResetInfo()
{
	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	switch( m_nCurPage )
	{
	case TGM_REGULAR:
		{	
			m_pList->RemoveAll();

			m_pColText[0]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_GUILDNAME );
			m_pColText[1]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_TITLE );
			m_pColText[2]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_LEVEL );
			m_pColText[3]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_TIME );
			m_pColText[4]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_STATE );
			m_pColText[5]->m_strText = "";
			m_pColText[6]->m_strText = "";
			m_pColText[7]->m_strText = "";

			CTGuildCommander::GuildOfferVec::iterator itMEMBER, itEND;
			std::sort( pCmd->m_GuildOfferVec.begin(), pCmd->m_GuildOfferVec.end(), _compare_guildoffer_vol );
			itMEMBER = pCmd->m_GuildOfferVec.begin();
			itEND = pCmd->m_GuildOfferVec.end();

			for(; itMEMBER != itEND ; ++itMEMBER )
			{
				int nLine = m_pList->AddString("");

				for( INT i=0 ; i < GROI_COUNT ; ++i )
				{
					switch( i )
					{
					case GROI_NAME:
						{
							m_pList->SetItemString( nLine, 0, itMEMBER->m_strGuildName );
							m_pList->SetItemData( nLine, 0, MAKELONG( nLine, GROI_NAME) );
						}
						break;
					case GROI_TITLE:
						{
							ResetTextSetting();

							CTTextLinker* pTextLinker = CTTextLinker::GetInstance();

							TLLINESETTING_ARRAY2D vSETTINGS2D;
							TTLINKLINE_ARRAY vLINES;

							CString strPMSG = pTextLinker->MakeNetToLinkText( this, itMEMBER->m_strTitle );
							CSize szITEM = m_pList->GetItemSize( GROI_TITLE );
							pTextLinker->SplitTextByComp( m_pList, szITEM.cx, strPMSG, vLINES);
							pTextLinker->GetTextSettings( this, vLINES, vSETTINGS2D);

//							INT nLineSize = (INT) vLINES.size();
//							for( INT i=0; i<nLineSize/nLineSize; i++)
							if( vLINES.size() )
							{
								const TTEXT_LINK_LINE& line = vLINES[0];
								const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[0];

								//INT iRow = m_pContentList->AddString(line.strMSG);
								m_pList->SetItemString( nLine, 1, line.strMSG );
								for( INT k=0; k<INT(vLineSettings.size()); k++)
								{
									const TLINK_LINE_SETTING& s = vLineSettings[k];
									m_pList->AddTextSetting( nLine, GROI_TITLE, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
								}
							}

							//m_pList->SetItemString( nLine, 1, itMEMBER->m_strTitle );
							m_pList->SetItemData( nLine, 1, MAKELONG( nLine, GROI_TITLE) );
						}
						break;
					case GROI_LEVEL:
						{
							CString strFMT;
							strFMT.Format( "%d - %d",
								itMEMBER->m_bMinLevel,
								itMEMBER->m_bMaxLevel );

							m_pList->SetItemString( nLine, 2, strFMT );
							m_pList->SetItemData( nLine, 2, MAKELONG( nLine, GROI_LEVEL) );
						}
						break;
					case GROI_ENDTIME:
						{
							CTime t( itMEMBER->m_dlTime );
							CString strFMT;
							strFMT.Format( "%d/%d/%d %d:%d",
								t.GetYear(),
								t.GetMonth(),
								t.GetDay(),
								t.GetHour(),
								t.GetMinute() );

							m_pList->SetItemString( nLine, 3, strFMT );
							m_pList->SetItemData( nLine, 3, MAKELONG( nLine, GROI_ENDTIME) );
						}
						break;
					case GROI_APPLIED:
						{
							CString strFMT;
							if( CTime(itMEMBER->m_dlTime) < CTClientApp::m_dCurDate )
							{
								strFMT = CTChart::LoadString( TSTR_GUILD_INVITATION_FINISH );
							}
							else if( itMEMBER->m_bApplied )
							{
								strFMT = CTChart::LoadString( TSTR_GUILDOFFER_APPLYED );
							}
							else
							{
								strFMT = CTChart::LoadString( TSTR_GUILD_INVITATION_ING );
							}

							m_pList->SetItemString( nLine, 4, strFMT );
							m_pList->SetItemData( nLine, 4, MAKELONG( nLine, GROI_APPLIED) );
						}
						break;
					}
				}

				m_pList->SetItemData( nLine, 5, MAKELONG( nLine, 0 ) );
				m_pList->SetItemData( nLine, 6, MAKELONG( nLine, 0 ) );
				m_pList->SetItemData( nLine, 7, MAKELONG( nLine, 0 ) );
				m_pList->SetItemData( nLine, 8, MAKELONG( nLine, 0 ) );
			}
		}
		break;

	case TGM_TACTICS:
		{
			m_pList->RemoveAll();

			m_pColText[0]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_GUILDNAME );
			m_pColText[1]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_TITLE );
			m_pColText[2]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_LEVEL );
			m_pColText[3]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_EARNEST_MONEY );
			m_pColText[4]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_BENEFIT );
			m_pColText[5]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_EARNEST_PERIOD );
			m_pColText[6]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_TIME );
			m_pColText[7]->m_strText = CTChart::LoadString( TSTR_GUILDOFFER_COL_STATE );

			CTGuildCommander::GuildTacticsOfferVec::iterator itMEMBER, itEND;
			std::sort( pCmd->m_GuildTacticsOfferVec.begin(), pCmd->m_GuildTacticsOfferVec.end(), _compare_tacticsoffer_vol );
			itMEMBER = pCmd->m_GuildTacticsOfferVec.begin();
			itEND = pCmd->m_GuildTacticsOfferVec.end();

			for(; itMEMBER != itEND ; ++itMEMBER )
			{
				int nLine = m_pList->AddString("");

				for( INT i=0 ; i < GTOI_COUNT ; ++i )
				{
					switch( i )
					{
					case GTOI_NAME:
						{
							m_pList->SetItemString( nLine, 0, itMEMBER->m_strGuildName );
							m_pList->SetItemData( nLine, 0, MAKELONG( nLine, GTOI_NAME) );
						}
						break;
					case GTOI_TITLE:
						{
							ResetTextSetting();

							CTTextLinker* pTextLinker = CTTextLinker::GetInstance();

							TLLINESETTING_ARRAY2D vSETTINGS2D;
							TTLINKLINE_ARRAY vLINES;

							CString strPMSG = pTextLinker->MakeNetToLinkText( this, itMEMBER->m_strTitle );
							CSize szITEM = m_pList->GetItemSize( GTOI_TITLE );
							pTextLinker->SplitTextByComp( m_pList, szITEM.cx, strPMSG, vLINES);
							pTextLinker->GetTextSettings( this, vLINES, vSETTINGS2D);

//							INT nLineSize = (INT) vLINES.size();
//							for( INT i=0; i<nLineSize/nLineSize; i++)
							if( vLINES.size() )
							{
								const TTEXT_LINK_LINE& line = vLINES[0];
								const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[0];

								//INT iRow = m_pContentList->AddString(line.strMSG);
								m_pList->SetItemString( nLine, 1, line.strMSG );
								for( INT k=0; k<INT(vLineSettings.size()); k++)
								{
									const TLINK_LINE_SETTING& s = vLineSettings[k];
									m_pList->AddTextSetting( nLine, GTOI_TITLE, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
								}
							}

							//m_pList->SetItemString( nLine, 1, itMEMBER->m_strTitle );
							m_pList->SetItemData( nLine, 1, MAKELONG( nLine, GTOI_TITLE) );
						}
						break;
					case GTOI_LEVEL:
						{
							CString strFMT;
							strFMT.Format( "%d - %d",
								itMEMBER->m_bMinLevel,
								itMEMBER->m_bMaxLevel );

							m_pList->SetItemString( nLine, 2, strFMT );
							m_pList->SetItemData( nLine, 2, MAKELONG( nLine, GTOI_LEVEL) );
						}
						break;
					case GTOI_MONEY:
						{
							CString strFMT;
							strFMT.Format( "%d/%d/%d",
								itMEMBER->m_dwGold,
								itMEMBER->m_dwSilver,
								itMEMBER->m_dwCooper );

							m_pList->SetItemString( nLine, 3, strFMT );
							m_pList->SetItemData( nLine, 3, MAKELONG( nLine, GTOI_MONEY) );
						}
						break;
					case GTOI_POINT:
						{
							CString strFMT;
							strFMT.Format( "%d",
								itMEMBER->m_dwPoint );

							m_pList->SetItemString( nLine, 4, strFMT );
							m_pList->SetItemData( nLine, 4, MAKELONG( nLine, GTOI_POINT) );
						}
						break;
					case GTOI_DAY:
						{
							CString strFMT;
							strFMT.Format( "%d",
								itMEMBER->m_bDay );

							m_pList->SetItemString( nLine, 5, strFMT );
							m_pList->SetItemData( nLine, 5, MAKELONG( nLine, GTOI_DAY) );
						}
						break;
					case GTOI_ENDTIME:
						{
							CTime t( itMEMBER->m_dlTime );
							CString strFMT;
							strFMT.Format( "%d/%d/%d %d:%d",
								t.GetYear(),
								t.GetMonth(),
								t.GetDay(),
								t.GetHour(),
								t.GetMinute() );

							m_pList->SetItemString( nLine, 6, strFMT );
							m_pList->SetItemData( nLine, 6, MAKELONG( nLine, GTOI_ENDTIME) );
						}
						break;
					case GTOI_APPLIED:
						{
							CString strFMT;

							if( CTime(itMEMBER->m_dlTime) < CTClientApp::m_dCurDate )
							{
								strFMT = CTChart::LoadString( TSTR_GUILD_INVITATION_FINISH );
							}
							else if( itMEMBER->m_bApplied )
							{
								strFMT = CTChart::LoadString( TSTR_GUILDOFFER_APPLYED );
							}
							else
							{
								strFMT = CTChart::LoadString( TSTR_GUILD_INVITATION_ING );
							}

							m_pList->SetItemString( nLine, 7, strFMT );
							m_pList->SetItemData( nLine, 7, MAKELONG( nLine, GTOI_APPLIED) );
						}
						break;
					}
				}

				m_pList->SetItemData( nLine, 8, MAKELONG( nLine, 0 ) );
			}
		}
		break;
	}
}

void CTGuildApplicationDlg::ChangePage( INT nType )
{
	m_nCurPage = nType;

	switch( m_nCurPage )
	{
	case TGM_REGULAR:
		m_pButton[0]->Select( (BOOL) TRUE );
		m_pButton[1]->Select( (BOOL) FALSE );
		break;
	case TGM_TACTICS:
		m_pButton[0]->Select( (BOOL) FALSE );
		m_pButton[1]->Select( (BOOL) TRUE );
		break;
	}
}

void CTGuildApplicationDlg::OnLButtonDown( UINT nFlags, CPoint pt )
{
	if( HitTest(pt) )
	{
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
		CTClientGame *pTGAME = CTClientGame::GetInstance();

		DWORD dwInfo;
		if( m_pList->HitTestTextSetting(pt, &dwInfo) )
		{
			CTTextLinkData *pLinkData = pTextLinker->GetTextLinkData(dwInfo);

			if( pLinkData && pLinkData->IsItemType() )
			{
				CTClientItem *pItem = pLinkData->GetLastUnkpackItem();

				if(pItem)
					pTGAME->ShowChatDetailInfo(pItem);

				return;
			}
		}

		//SwitchFocus( m_vInfoCtrl[INFO_NOTIFY]);
	}

	CTClientUIBase::OnLButtonDown( nFlags, pt );
}

void CTGuildApplicationDlg::OnLButtonDblClk( UINT nFlags, CPoint pt )
{
	if( !m_bVisible || !m_bEnable )
		return ;

	CTClientGame* pGame = CTClientGame::GetInstance();
	CTGuildCommander* pGuild = CTGuildCommander::GetInstance();

	TListItem* pItem = m_pList->GetHitItem( pt );
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

	if( m_nLastSelIdx != T_INVALID )
	{
		switch( m_nCurPage )
		{
		case TGM_REGULAR:
			{
				static_cast<CTGuildRegularOffer*>( pGame->GetFrame( TFRAME_GUILD_REGULAR_OFFER ) )
					->ResetViewInfo( pGuild->m_GuildOfferVec[ m_nLastSelIdx ] );

				pGame->EnableUI( TFRAME_GUILD_REGULAR_OFFER );
			}
			break;
		case TGM_TACTICS:
			{
				static_cast<CTGuildTacticsOffer*>( pGame->GetFrame( TFRAME_GUILD_TACTICS_OFFER ) )
					->ResetViewInfo( pGuild->m_GuildTacticsOfferVec[ m_nLastSelIdx ] );

				pGame->EnableUI( TFRAME_GUILD_TACTICS_OFFER );
			}
			break;
		}
	}

	CTClientUIBase::OnLButtonDblClk( nFlags, pt );
}

void CTGuildApplicationDlg::ShowComponent( BOOL bVisible )
{
	CTClientUIBase::ShowComponent( bVisible );

	switch( m_nCurPage )
	{
	case TGM_REGULAR:
		m_pText->ShowComponent( FALSE );
		break;
	case TGM_TACTICS:
		break;
	}
}

void CTGuildApplicationDlg::ResetPosition()
{
	m_pList->RemoveAll();

	if( m_bVisible )
		RequestInfo();
}

ITDetailInfoPtr CTGuildApplicationDlg::GetTInfoKey( const CPoint& point )
{
	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	TListItem* pHITITEM = m_pList->GetHitItem( point );

	if( pHITITEM )
	{
		WORD wLine = LOWORD(pHITITEM->m_param);
		WORD wCol = HIWORD(pHITITEM->m_param);

		CRect rc;
		GetComponentRect( &rc );
		ComponentToScreen( &rc);

		if( m_nCurPage == TGM_REGULAR )
		{
			if( wCol == GROI_TITLE )
			{
				if( !pCmd->m_GuildOfferVec[ wLine ].m_strText.IsEmpty() )
				{
					return CTDetailInfoManager::NewTextInst(
						pCmd->m_GuildOfferVec[ wLine ].m_strText,
						point);
				}
			}
		}
		else if( m_nCurPage == TGM_TACTICS )
		{
			if( wCol == GTOI_TITLE )
			{
				if( !pCmd->m_GuildTacticsOfferVec[ wLine ].m_strText.IsEmpty() )
				{
					return CTDetailInfoManager::NewTextInst(
						pCmd->m_GuildTacticsOfferVec[ wLine ].m_strText,
						point);
				}
			}
		}
	}

	return ITDetailInfoPtr(NULL);
}

HRESULT CTGuildApplicationDlg::Render( DWORD dwTickCount )
{
	INT nIndex = m_pList->GetCurSelIndex();

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();

	if( nIndex != -1 )
	{
		if( m_nCurPage == TGM_REGULAR &&
			nIndex < pCmd->m_GuildOfferVec.size() )
		{
			if( (pMainChar->m_dwTacticsID != 0 && pCmd->m_GuildOfferVec[ nIndex ].m_dwGuildID == pMainChar->m_dwTacticsID) ||
				(pMainChar->m_dwGuildID != 0 && pCmd->m_GuildOfferVec[ nIndex ].m_dwGuildID == pMainChar->m_dwGuildID) )
			{
				m_pBtnJOIN->ShowComponent( FALSE );
				m_pBtnJOINCANCEL->ShowComponent( FALSE );
			}
			else
			{
				BOOL b = pCmd->m_GuildOfferVec[ nIndex ].m_bApplied;
				m_pBtnJOIN->ShowComponent( !b );
				m_pBtnJOINCANCEL->ShowComponent( b );
			}		
		}
		else if( m_nCurPage == TGM_TACTICS &&
			nIndex < pCmd->m_GuildTacticsOfferVec.size() )
		{
			if( (pMainChar->m_dwTacticsID != 0 && pMainChar->m_dwTacticsID == pCmd->m_GuildTacticsOfferVec[ nIndex ].m_dwGuildID) ||
				(pMainChar->m_dwGuildID != 0 && pMainChar->m_dwGuildID == pCmd->m_GuildTacticsOfferVec[ nIndex ].m_dwGuildID) )
			{
				m_pBtnJOIN->ShowComponent( FALSE );
				m_pBtnJOINCANCEL->ShowComponent( FALSE );
			}
			else
			{
				BOOL b = pCmd->m_GuildTacticsOfferVec[ nIndex ].m_bApplied;
				m_pBtnJOIN->ShowComponent( !b );
				m_pBtnJOINCANCEL->ShowComponent( b );
			}		
		}
	}
	else
	{
		m_pBtnJOIN->ShowComponent( TRUE );
		m_pBtnJOINCANCEL->ShowComponent( TRUE );
	}

	return CTClientUIBase::Render( dwTickCount );
}

void CTGuildApplicationDlg::OnNotify( DWORD from, WORD msg, LPVOID param )
{
	CTClientUIBase::OnNotify( from, msg, param );
}