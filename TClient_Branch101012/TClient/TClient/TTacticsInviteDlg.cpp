#include "stdafx.h"
#include "TTacticsInviteDlg.h"
#include "TClientGame.h"

CTTacticsInviteDlg::CTTacticsInviteDlg( TComponent* pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc ),
m_nMode( TACTICS_INVITE_DLG_MODE_EDIT )
{
	m_pTITLE = FindKid( ID_CTRLINST_TITLE );
	m_pMSG = FindKid( ID_CTRLINST_MSG );
	m_pBtnOK = (TButton*) FindKid( ID_CTRLINST_BTN_OK );
	m_pBtnCancel = (TButton*) FindKid( ID_CTRLINST_BTN_CANCEL );

	m_pPeriod = FindKid( ID_CTRLINST_PERIOD );
	m_pHornor = FindKid( ID_CTRLINST_HONOR );
	m_pRune = FindKid( ID_CTRLINST_RUNE );
	m_pLuna = FindKid( ID_CTRLINST_LUNA );
	m_pCron = FindKid( ID_CTRLINST_CRON );

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_PERIOD );
		m_pPeriodEdit = new CTNumberEdit( this, pTemp->m_pDESC, 2 );
		m_pPeriodEdit->SetUseLimit( 1, 7 );
		m_pPeriodEdit->SetDefault( "1" );
		RemoveKid( pTemp );
		AddKid( m_pPeriodEdit );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_HONOR );
		m_pPointEdit = new CTNumberEdit( this, pTemp->m_pDESC, 10 );
		m_pPointEdit->SetUseLimit( 0, INT_MAX );
		m_pPointEdit->SetDefault( "0" );
		RemoveKid( pTemp );
		AddKid( m_pPointEdit );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_RUNE );
		m_pRuneEdit = new CTNumberEdit( this, pTemp->m_pDESC, TRUNE_LENGTH );
		m_pRuneEdit->SetUseLimit( 0, 99999999 );
		m_pRuneEdit->SetDefault( "0" );
		RemoveKid( pTemp );
		AddKid( m_pRuneEdit );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_LUNA );
		m_pLunaEdit = new CTNumberEdit( this, pTemp->m_pDESC, TLUNA_LENGTH );
		m_pLunaEdit->SetUseLimit( 0, 999 );
		m_pLunaEdit->SetDefault( "0" );
		RemoveKid( pTemp );
		AddKid( m_pLunaEdit );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_CRON );
		m_pCronEdit = new CTNumberEdit( this, pTemp->m_pDESC, TCRON_LENGTH );
		m_pCronEdit->SetUseLimit( 0, 999 );
		m_pCronEdit->SetDefault( "0" );
		RemoveKid( pTemp );
		AddKid( m_pCronEdit );
		delete pTemp;
	}

	m_bTabAutoNext = FALSE;
	m_pPeriodEdit->m_pNextTab = m_pPointEdit;
	m_pPointEdit->m_pNextTab = m_pRuneEdit;
	m_pRuneEdit->m_pNextTab = m_pLunaEdit;
	m_pLunaEdit->m_pNextTab = m_pCronEdit;
	m_pCronEdit->m_pNextTab = m_pPeriodEdit;
}

CTTacticsInviteDlg::~CTTacticsInviteDlg()
{
}

TEdit* CTTacticsInviteDlg::GetCurEdit()
{
	if( !CanProcess() )
		return NULL;

	if( m_nMode != TACTICS_INVITE_DLG_MODE_EDIT )
		return NULL;

	if( m_pPeriodEdit->GetFocus() )
		return m_pPeriodEdit;
	if( m_pPointEdit->GetFocus() )
		return m_pPointEdit;
	if( m_pRuneEdit->GetFocus() )
		return m_pRuneEdit;
	if( m_pLunaEdit->GetFocus() )
		return m_pLunaEdit;
	if( m_pCronEdit->GetFocus() )
		return m_pCronEdit;

	return NULL;
}

void CTTacticsInviteDlg::ShowComponent( BOOL bVisible)
{
	static BOOL m_bPrevVisible = FALSE;

	CTClientUIBase::ShowComponent( bVisible );

	if( bVisible )
	{
		switch( m_nMode )
		{
		case TACTICS_INVITE_DLG_MODE_EDIT:
			m_pPeriod->ShowComponent( FALSE );
			m_pHornor->ShowComponent( FALSE );
			m_pRune->ShowComponent( FALSE );
			m_pLuna->ShowComponent( FALSE );
			m_pCron->ShowComponent( FALSE );
			m_pPeriodEdit->ShowComponent( TRUE ); 
			m_pPointEdit->ShowComponent( TRUE );
			m_pRuneEdit->ShowComponent( TRUE );
			m_pLunaEdit->ShowComponent( TRUE );
			m_pCronEdit->ShowComponent( TRUE );
			SwitchFocus( m_pPeriodEdit );
			break;

		case TACTICS_INVITE_DLG_MODE_VIEW:
			m_pPeriod->ShowComponent( TRUE );
			m_pHornor->ShowComponent( TRUE );
			m_pRune->ShowComponent( TRUE );
			m_pLuna->ShowComponent( TRUE );
			m_pCron->ShowComponent( TRUE );
			m_pPeriodEdit->ShowComponent( FALSE ); 
			m_pPointEdit->ShowComponent( FALSE );
			m_pRuneEdit->ShowComponent( FALSE );
			m_pLunaEdit->ShowComponent( FALSE );
			m_pCronEdit->ShowComponent( FALSE );
			break;		
		}
	}
	else
	{
		if( !m_bCloseUI && m_bPrevVisible && m_nMode == TACTICS_INVITE_DLG_MODE_VIEW )
			CTClientGame::GetInstance()->OnGM_GUILDTACTICSINVITE_ACK_BUSY();
	}

	m_bPrevVisible = bVisible;
	m_bCloseUI = FALSE;
}

void CTTacticsInviteDlg::SetInviteCond( CString& strTarget )
{
	m_strTarget = strTarget;
	m_pTITLE->m_strText = CTChart::LoadString( TSTR_MBOX_TACTICS_TITLE_INVITE );
	m_pMSG->m_strText = CTChart::LoadString( TSTR_MBOX_TACTICS_INVITE_COND );
	m_nMode = TACTICS_INVITE_DLG_MODE_EDIT;

	m_pBtnOK->m_menu[ TNM_LCLICK ] = GM_TACTICS_REQ;
	m_pBtnOK->m_strText = CTChart::LoadString( TSTR_TACTICS_WORD_INVITE );
	m_pBtnCancel->m_menu[ TNM_LCLICK ] = GM_CLOSE_UI;
	m_pBtnCancel->m_strText = CTChart::LoadString( TSTR_TACTICS_WORD_CANCEL );

	m_pPeriodEdit->ClearText();
	m_pPeriodEdit->SetText( "1" );

	m_pPointEdit->ClearText();
	m_pPointEdit->SetText( "0" );

	m_pRuneEdit->ClearText();
	m_pRuneEdit->SetText( "0" );

	m_pLunaEdit->ClearText();
	m_pLunaEdit->SetText( "0" );

	m_pCronEdit->ClearText();
	m_pCronEdit->SetText( "0" );
}

void CTTacticsInviteDlg::SetReInviteCond( CString strTarget )
{
	m_strTarget = strTarget;
	m_pTITLE->m_strText = CTChart::Format( TSTR_MBOX_TACTICS_TITLE_REINVITE );
	m_pMSG->m_strText = CTChart::LoadString( TSTR_MBOX_TACTICS_REINVITE_COND );
	m_nMode = TACTICS_INVITE_DLG_MODE_EDIT;

	m_pBtnOK->m_menu[ TNM_LCLICK ] = GM_TACTICS_REQ;
	m_pBtnOK->m_strText = CTChart::LoadString( TSTR_TACTICS_WORD_REINVITE );
	m_pBtnCancel->m_menu[ TNM_LCLICK ] = GM_CLOSE_UI;
	m_pBtnCancel->m_strText = CTChart::LoadString( TSTR_TACTICS_WORD_CANCEL );

	m_pPeriodEdit->ClearText();
	m_pPeriodEdit->SetText( "1" );

	m_pPointEdit->ClearText();
	m_pPointEdit->SetText( "0" );

	m_pRuneEdit->ClearText();
	m_pRuneEdit->SetText( "0" );

	m_pLunaEdit->ClearText();
	m_pLunaEdit->SetText( "0" );

	m_pCronEdit->ClearText();
	m_pCronEdit->SetText( "0" );
}

void CTTacticsInviteDlg::SetShowAccept(
	CString strTarget,
	CString strInviter,
	CString strGuild,
	BYTE bDay,
	DWORD dwPoint,
	DWORD dwGold,
	DWORD dwSilver,
	DWORD dwCooper )
{
	m_strInviter = strInviter;
	m_strTarget = strTarget;

	m_pTITLE->m_strText = CTChart::Format( TSTR_MBOX_TACTICS_TITLE_ACCEPT );

	m_pMSG->m_strText = CTChart::Format( TSTR_MBOX_TACTICS_INVITE,
		strInviter,
		strGuild );

	m_pPeriod->m_strText.Format( "%d", bDay );
	m_pHornor->m_strText.Format( "%d", dwPoint );
	m_pRune->m_strText.Format( "%d", dwGold );
	m_pLuna->m_strText.Format( "%d", dwSilver );
	m_pCron->m_strText.Format( "%d", dwCooper );

	m_nMode = TACTICS_INVITE_DLG_MODE_VIEW;

	m_pBtnOK->m_menu[ TNM_LCLICK ] = GM_GUILDTACTICSINVITE_ACK_YES;
	m_pBtnOK->m_strText = CTChart::LoadString( TSTR_TACTICS_WORD_ACCEPT );
	m_pBtnCancel->m_menu[ TNM_LCLICK ] = GM_GUILDTACTICSINVITE_ACK_NO;
	m_pBtnCancel->m_strText = CTChart::LoadString( TSTR_TACTICS_WORD_REJECT );
}
