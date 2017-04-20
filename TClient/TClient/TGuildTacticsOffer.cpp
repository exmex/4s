#include "StdAfx.h"
#include "TGuildTacticsOffer.h"
#include "TClientGame.h"

CTGuildTacticsOffer::CTGuildTacticsOffer( TComponent* pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc ),
m_nMode(0)
{
	m_pViewArticleTitle = FindKid( ID_CTRLINST_VIEW_ARTICLE_TITLE );
	m_pViewGuildName = FindKid( ID_CTRLINST_VIEW_GUILD_NAME );
	m_pViewHornor = FindKid( ID_CTRLINST_VIEW_HONOR );
	m_pViewPeriod = FindKid( ID_CTRLINST_VIEW_PERIOD );
	m_pViewLevelMin = FindKid( ID_CTRLINST_VIEW_LEVEL_MIN );
	m_pViewLevelMax = FindKid( ID_CTRLINST_VIEW_LEVEL_MAX );
	m_pViewRune = FindKid( ID_CTRLINST_VIEW_RUNE );
	m_pViewLuna = FindKid( ID_CTRLINST_VIEW_LUNA );
	m_pViewCron = FindKid( ID_CTRLINST_VIEW_CRON );
	m_pViewContent = (TList*) FindKid( ID_CTRLINST_VIEW_CONTENTS );

	m_pEditArticleTitle = (TEdit*) FindKid( ID_CTRLINST_EDIT_ARTICLE_TITLE );

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_HONOR );
		m_pEditHornor = new CTNumberEdit( this, pTemp->m_pDESC, 4 );
		RemoveKid( pTemp );
		AddKid( m_pEditHornor );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_PERIOD );
		m_pEditPeriod = new CTNumberEdit( this, pTemp->m_pDESC, 2 );
		m_pEditPeriod->SetUseLimit( 1, 7 );
		m_pEditPeriod->SetDefault( "1" );
		m_pEditPeriod->m_strWarning = CTChart::LoadString( TSTR_GUILD_MAX_CONTRACT );
		RemoveKid( pTemp );
		AddKid( m_pEditPeriod );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_LEVEL_MIN );
		m_pEditLevelMin = new CTNumberEdit( this, pTemp->m_pDESC, 3 );
		m_pEditLevelMin->SetUseLimit( 1, TMAX_CHARLEVEL );
		m_pEditLevelMin->SetDefault( "1" );
		RemoveKid( pTemp );
		AddKid( m_pEditLevelMin );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_LEVEL_MAX );
		m_pEditLevelMax = new CTNumberEdit( this, pTemp->m_pDESC, 3 );
		m_pEditLevelMax->SetUseLimit( 1, TMAX_CHARLEVEL );
		
		CString strMaxLevel;
		strMaxLevel.Format( "%d", TMAX_CHARLEVEL );
		m_pEditLevelMax->SetDefault( strMaxLevel );

		RemoveKid( pTemp );
		AddKid( m_pEditLevelMax );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_RUNE );
		m_pEditRune = new CTNumberEdit( this, pTemp->m_pDESC, 4 );
		RemoveKid( pTemp );
		AddKid( m_pEditRune );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_LUNA );
		m_pEditLuna = new CTNumberEdit( this, pTemp->m_pDESC, 3 );
		RemoveKid( pTemp );
		AddKid( m_pEditLuna );
		delete pTemp;
	}

	{
		TComponent* pTemp = FindKid( ID_CTRLINST_EDIT_CRON );
		m_pEditCron = new CTNumberEdit( this, pTemp->m_pDESC, 3 );
		RemoveKid( pTemp );
		AddKid( m_pEditCron );
		delete pTemp;
	}

	{
		TComponent* pComp = FindKid(ID_CTRLINST_EDIT_CONTENTS);
		m_pEditContent = new TMultiLineEdit(this, pComp->m_pDESC);
		m_pEditContent->m_id = pComp->m_id;
		RemoveKid(pComp);
		delete pComp;
		AddKid(m_pEditContent);
	}

	m_pBtnOK = (TButton*) FindKid( ID_CTRLINST_BTN_OK );
	m_pBtnCancel = (TButton*) FindKid( ID_CTRLINST_BTN_CANCEL );
	m_dwArticleID = 0;

	// ÅÇ ¼ø¼­
	m_bTabAutoNext = FALSE;
	m_pEditArticleTitle->m_pNextTab = m_pEditRune;
	m_pEditRune->m_pNextTab = m_pEditLuna;
	m_pEditLuna->m_pNextTab = m_pEditCron;
	m_pEditCron->m_pNextTab = m_pEditHornor;
	m_pEditHornor->m_pNextTab = m_pEditPeriod;
	m_pEditPeriod->m_pNextTab = m_pEditLevelMin;
	m_pEditLevelMin->m_pNextTab = m_pEditLevelMax;
	m_pEditLevelMax->m_pNextTab = m_pEditContent;
	m_pEditContent->m_pNextTab = m_pEditArticleTitle;
}

CTGuildTacticsOffer::~CTGuildTacticsOffer()
{
}

TEdit* CTGuildTacticsOffer::GetCurEdit()
{
	if( m_nMode == 1 )
	{
		if( m_pEditArticleTitle->GetFocus() )
			return m_pEditArticleTitle;

		if( m_pEditHornor->GetFocus() )
			return m_pEditHornor;

		if( m_pEditPeriod->GetFocus() )
			return m_pEditPeriod;

		if( m_pEditLevelMin->GetFocus() )
			return m_pEditLevelMin;

		if( m_pEditLevelMax->GetFocus() )
			return m_pEditLevelMax;

		if( m_pEditRune->GetFocus() )
			return m_pEditRune;

		if( m_pEditLuna->GetFocus() )
			return m_pEditLuna;

		if( m_pEditCron->GetFocus() )
			return m_pEditCron;

		if( m_pEditContent->GetFocus() )
			return m_pEditContent;
	}
	else if( m_nMode == 2 )
	{
		if( m_pEditArticleTitle->GetFocus() )
			return m_pEditArticleTitle;
	
		if( m_pEditContent->GetFocus() )
			return m_pEditContent;
	}

	return NULL;
}

void CTGuildTacticsOffer::ResetModifyInfo( CTGuildCommander::GuildTacticsOffer& offer )
{
	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();

	CString strFMT;

	m_nMode = 2;

	m_dwArticleID = offer.m_dwID;

	m_pEditArticleTitle->ResetTextSetting();
	m_pEditArticleTitle->ClearText();
	m_pEditArticleTitle->m_strText = pTextLinker->MakeNetToSimpleLinkText(m_pEditArticleTitle, offer.m_strTitle);
	m_pEditArticleTitle->MoveCaretToBack();

	m_pViewGuildName->m_strText = offer.m_strGuildName;

	strFMT.Format( "%d", offer.m_dwPoint );
	m_pViewHornor->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_bDay );
	m_pViewPeriod->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_bMinLevel );
	m_pViewLevelMin->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_bMaxLevel );
	m_pViewLevelMax->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_dwGold );
	m_pViewRune->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_dwSilver );
	m_pViewLuna->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_dwCooper );
	m_pViewCron->m_strText = strFMT;

	m_pEditContent->ResetTextSetting();
	m_pEditContent->ClearText();
	m_pEditContent->m_strText = pTextLinker->MakeNetToLinkText(m_pEditContent, offer.m_strText);
	m_pEditContent->MoveCaretToBack();

	m_pBtnOK->m_strText = CTChart::LoadString( TSTR_GUILDOFFEREDIT_MODIFY );
}

void CTGuildTacticsOffer::ResetViewInfo( CTGuildCommander::GuildTacticsOffer& offer )
{
	CString strFMT;

	m_nMode = 0;

	m_dwArticleID = offer.m_dwID;

	{
		ResetTextSetting();
		m_pViewArticleTitle->ResetTextSetting();

		CTTextLinker* pTextLinker = CTTextLinker::GetInstance();

		TLLINESETTING_ARRAY2D vSETTINGS2D;
		TTLINKLINE_ARRAY vLINES;

		CString strPMSG = pTextLinker->MakeNetToLinkText( this, offer.m_strTitle );
		CSize szITEM;
		m_pViewArticleTitle->GetComponentSize( &szITEM );
		pTextLinker->SplitTextByComp( m_pViewArticleTitle, szITEM.cx, strPMSG, vLINES);
		pTextLinker->GetTextSettings( this, vLINES, vSETTINGS2D);

//		INT nLineSize = (INT) vLINES.size();
//		for( INT i=0; i<nLineSize/nLineSize; i++)
		if( vLINES.size() )
		{
			const TTEXT_LINK_LINE& line = vLINES[0];
			const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[0];

			m_pViewArticleTitle->m_strText = line.strMSG;
			for( INT k=0; k<INT(vLineSettings.size()); k++)
			{
				const TLINK_LINE_SETTING& s = vLineSettings[k];
				m_pViewArticleTitle->AddTextSetting( s.nStart, s.nEnd, s.dwColor, s.dwInfo);
			}
		}
	}

	m_pViewGuildName->m_strText = offer.m_strGuildName;

	strFMT.Format( "%d", offer.m_dwPoint );
	m_pViewHornor->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_bDay );
	m_pViewPeriod->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_bMinLevel );
	m_pViewLevelMin->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_bMaxLevel );
	m_pViewLevelMax->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_dwGold );
	m_pViewRune->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_dwSilver );
	m_pViewLuna->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_dwCooper );
	m_pViewCron->m_strText = strFMT;

	{
		ResetTextSetting();
		m_pViewContent->RemoveAll();
		m_pViewContent->ResetTextSetting();
		
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();

		TLLINESETTING_ARRAY2D vSETTINGS2D;
		TTLINKLINE_ARRAY vLINES;
		INT nPos = 0;

		CString strLINE = pTextLinker->SplitTextByCrLf(
			offer.m_strText,
			CTTextLinker::LINK_DEF_TOK,
			nPos, FALSE);

		while(!strLINE.IsEmpty())
		{
			CString strPMSG = pTextLinker->MakeNetToLinkText( this, strLINE);
			strPMSG.Remove('\r');

			CSize szITEM = m_pViewContent->GetItemSize(0);
			pTextLinker->SplitTextByComp( m_pViewContent, szITEM.cx, strPMSG, vLINES);
			pTextLinker->GetTextSettings( this, vLINES, vSETTINGS2D);

			INT nLineSize = (INT) vLINES.size();
			for( INT i=0; i<nLineSize; i++)
			{
				const TTEXT_LINK_LINE& line = vLINES[i];
				const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[i];

				INT iRow = m_pViewContent->AddString(line.strMSG);
				for( INT k=0; k<INT(vLineSettings.size()); k++)
				{
					const TLINK_LINE_SETTING& s = vLineSettings[k];
					m_pViewContent->AddTextSetting( iRow, 0, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
				}
			}

			vLINES.clear();
			vSETTINGS2D.clear();
			ResetTextSetting();

			strLINE = pTextLinker->SplitTextByCrLf( offer.m_strText, CTTextLinker::LINK_DEF_TOK, nPos, FALSE);
		}
	}

	m_pBtnOK->m_strText = CTChart::LoadString( TSTR_GUILDOFFEREDIT_OK );
}

void CTGuildTacticsOffer::ResetNewEdit()
{
	CString strFMT;

	m_nMode = 1;

	m_dwArticleID = 0;

	m_pEditArticleTitle->ClearText();
	m_pViewGuildName->m_strText = CTClientGame::GetInstance()->GetMainChar()->m_strGUILD;

	m_pEditHornor->ClearText();
	m_pEditHornor->SetText( "0" );

	m_pEditPeriod->ClearText();
	m_pEditPeriod->SetText( "1" );

	m_pEditLevelMin->ClearText();
	m_pEditLevelMin->SetText( "1" );

	m_pEditLevelMax->ClearText();

	CString strMaxLevel;
	strMaxLevel.Format( "%d", TMAX_CHARLEVEL );
	m_pEditLevelMax->SetText( strMaxLevel );

	m_pEditRune->ClearText();
	m_pEditRune->SetText( "0" );

	m_pEditLuna->ClearText();
	m_pEditLuna->SetText( "0" );

	m_pEditCron->ClearText();
	m_pEditCron->SetText( "0" );

	m_pEditContent->ClearText();

	m_pBtnOK->m_strText = CTChart::LoadString( TSTR_GUILDOFFEREDIT_WRITE );
}

void CTGuildTacticsOffer::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent( bVisible );

	if( bVisible )
	{
		switch( m_nMode )
		{
		case 0:
			m_pViewArticleTitle->ShowComponent( TRUE );
			m_pViewGuildName->ShowComponent( TRUE );
			m_pViewHornor->ShowComponent( TRUE );
			m_pViewPeriod->ShowComponent( TRUE );
			m_pViewLevelMin->ShowComponent( TRUE );
			m_pViewLevelMax->ShowComponent( TRUE );
			m_pViewRune->ShowComponent( TRUE );
			m_pViewLuna->ShowComponent( TRUE );
			m_pViewCron->ShowComponent( TRUE );
			m_pViewContent->ShowComponent( TRUE );

			m_pEditArticleTitle->ShowComponent( FALSE );
			m_pEditHornor->ShowComponent( FALSE );
			m_pEditPeriod->ShowComponent( FALSE );
			m_pEditLevelMin->ShowComponent( FALSE );
			m_pEditLevelMax->ShowComponent( FALSE );
			m_pEditRune->ShowComponent( FALSE );
			m_pEditLuna->ShowComponent( FALSE );
			m_pEditCron->ShowComponent( FALSE );
			m_pEditContent->ShowComponent( FALSE );

			m_pEditArticleTitle->EnableComponent( FALSE );
			m_pEditHornor->EnableComponent( FALSE );
			m_pEditPeriod->EnableComponent( FALSE );
			m_pEditLevelMin->EnableComponent( FALSE );
			m_pEditLevelMax->EnableComponent( FALSE );
			m_pEditRune->EnableComponent( FALSE );
			m_pEditLuna->EnableComponent( FALSE );
			m_pEditCron->EnableComponent( FALSE );
			m_pEditContent->EnableComponent( FALSE );

			m_pBtnCancel->ShowComponent( FALSE );
			break;

		case 1:
			m_pViewArticleTitle->ShowComponent( FALSE );
			m_pViewGuildName->ShowComponent( TRUE );
			m_pViewHornor->ShowComponent( FALSE );
			m_pViewPeriod->ShowComponent( FALSE );
			m_pViewLevelMin->ShowComponent( FALSE );
			m_pViewLevelMax->ShowComponent( FALSE );
			m_pViewRune->ShowComponent( FALSE );
			m_pViewLuna->ShowComponent( FALSE );
			m_pViewCron->ShowComponent( FALSE );
			m_pViewContent->ShowComponent( FALSE );

			m_pEditArticleTitle->ShowComponent( TRUE );
			m_pEditHornor->ShowComponent( TRUE );
			m_pEditPeriod->ShowComponent( TRUE );
			m_pEditLevelMin->ShowComponent( TRUE );
			m_pEditLevelMax->ShowComponent( TRUE );
			m_pEditRune->ShowComponent( TRUE );
			m_pEditLuna->ShowComponent( TRUE );
			m_pEditCron->ShowComponent( TRUE );
			m_pEditContent->ShowComponent( TRUE );

			m_pEditArticleTitle->EnableComponent( TRUE );
			m_pEditHornor->EnableComponent( TRUE );
			m_pEditPeriod->EnableComponent( TRUE );
			m_pEditLevelMin->EnableComponent( TRUE );
			m_pEditLevelMax->EnableComponent( TRUE );
			m_pEditRune->EnableComponent( TRUE );
			m_pEditLuna->EnableComponent( TRUE );
			m_pEditCron->EnableComponent( TRUE );
			m_pEditContent->EnableComponent( TRUE );

			m_pBtnCancel->ShowComponent( TRUE );
			break;

		case 2:
			m_pViewArticleTitle->ShowComponent( FALSE );
			m_pViewGuildName->ShowComponent( TRUE );
			m_pViewHornor->ShowComponent( TRUE );
			m_pViewPeriod->ShowComponent( TRUE );
			m_pViewLevelMin->ShowComponent( TRUE );
			m_pViewLevelMax->ShowComponent( TRUE );
			m_pViewRune->ShowComponent( TRUE );
			m_pViewLuna->ShowComponent( TRUE );
			m_pViewCron->ShowComponent( TRUE );
			m_pViewContent->ShowComponent( FALSE );

			m_pEditArticleTitle->ShowComponent( TRUE );
			m_pEditHornor->ShowComponent( FALSE );
			m_pEditPeriod->ShowComponent( FALSE );
			m_pEditLevelMin->ShowComponent( FALSE );
			m_pEditLevelMax->ShowComponent( FALSE );
			m_pEditRune->ShowComponent( FALSE );
			m_pEditLuna->ShowComponent( FALSE );
			m_pEditCron->ShowComponent( FALSE );
			m_pEditContent->ShowComponent( TRUE );

			m_pEditArticleTitle->EnableComponent( TRUE );
			m_pEditHornor->EnableComponent( FALSE );
			m_pEditPeriod->EnableComponent( FALSE );
			m_pEditLevelMin->EnableComponent( FALSE );
			m_pEditLevelMax->EnableComponent( FALSE );
			m_pEditRune->EnableComponent( FALSE );
			m_pEditLuna->EnableComponent( FALSE );
			m_pEditCron->EnableComponent( FALSE );
			m_pEditContent->EnableComponent( TRUE );

			m_pBtnCancel->ShowComponent( TRUE );
			break;
		}

		SwitchFocus( m_pEditArticleTitle );
	}	
}

INT CTGuildTacticsOffer::GetMode()
{
	return m_nMode;
}

void CTGuildTacticsOffer::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess()) 
		return;

	if( nChar == VK_RETURN && m_pEditContent->GetFocus() )
	{
		m_pEditContent->InsertReturnChar();
		m_pEditContent->MoveScrollToCaret(FALSE);
	}
	else
		CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
}

TEdit* CTGuildTacticsOffer::GetCurEditLink()
{
	if( !CanProcess() )
		return NULL;

	if( m_nMode == 1 || m_nMode == 2 )
	{
		if( m_pEditArticleTitle->GetFocus() )
			return m_pEditArticleTitle;
		if( m_pEditContent->GetFocus() )
			return m_pEditContent;
	}

	return NULL;
}

void CTGuildTacticsOffer::OnLButtonDown( UINT nFlags, CPoint pt)
{
	if( HitTest(pt) )
	{
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
		CTClientGame *pTGAME = CTClientGame::GetInstance();

		if( m_nMode == 0 )
		{
			if( m_pViewArticleTitle->HitTest( pt ) )
			{
				DWORD dwInfo;
				if( m_pViewArticleTitle->HitTestTextSetting(pt, &dwInfo) )
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
			}

			if( m_pViewContent->HitTest( pt ) )
			{
				DWORD dwInfo;
				if( m_pViewContent->HitTestTextSetting(pt, &dwInfo) )
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
			}
		}
		else
		{
		}

		//SwitchFocus( m_vInfoCtrl[INFO_NOTIFY]);
	}

	CTClientUIBase::OnLButtonDown( nFlags, pt );
}