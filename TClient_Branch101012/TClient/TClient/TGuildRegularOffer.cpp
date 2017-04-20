#include "StdAfx.h"
#include "TGuildRegularOffer.h"
#include "TClientGame.h"

CTGuildRegularOffer::CTGuildRegularOffer( TComponent* pParent, LP_FRAMEDESC pDesc)
: CTClientUIBase( pParent, pDesc ),
m_nMode(0)
{
	m_pViewArticleTitle = FindKid( ID_CTRLINST_VIEW_ARTICLE_TITLE );
	m_pViewGuildName = FindKid( ID_CTRLINST_GUILD_NAME );
	m_pViewLevelMin = FindKid( ID_CTRLINST_VIEW_LEVEL_MIN );
	m_pViewLevelMax = FindKid( ID_CTRLINST_VIEW_LEVEL_MAX );
	m_pViewContent = (TList*) FindKid( ID_CTRLINST_LIST_CONTENTS );

	m_pEditArticleTitle = (TEdit*) FindKid( ID_CTRLINST_EDIT_ARTICLE_TITLE );

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
		TComponent* pComp = FindKid(ID_CTRLINST_EDIT_CONTENTS );
		m_pEditContent = new TMultiLineEdit(this, pComp->m_pDESC);
		RemoveKid(pComp);
		delete pComp;
		m_pEditContent->m_id = pComp->m_id;
		AddKid(m_pEditContent);
	}

	m_pBtnOK = (TButton*) FindKid( ID_CTRLINST_BTN_OK );
	m_pBtnCancel = (TButton*) FindKid( ID_CTRLINST_BTN_CANCEL );
	m_dwArticleID = 0;

	// ÅÇ¼ø¼­
	m_bTabAutoNext = FALSE;
	m_pEditArticleTitle->m_pNextTab = m_pEditLevelMin;
	m_pEditLevelMin->m_pNextTab = m_pEditLevelMax;
    m_pEditLevelMax->m_pNextTab = m_pEditContent;
	m_pEditContent->m_pNextTab = m_pEditArticleTitle;
}

CTGuildRegularOffer::~CTGuildRegularOffer()
{
}

TEdit* CTGuildRegularOffer::GetCurEdit()
{
	if( m_nMode == 1 )
	{
		if( m_pEditArticleTitle->GetFocus() )
			return m_pEditArticleTitle;

		if( m_pEditLevelMin->GetFocus() )
			return m_pEditLevelMin;

		if( m_pEditLevelMax->GetFocus() )
			return m_pEditLevelMax;
		
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

void CTGuildRegularOffer::ResetModifyInfo( CTGuildCommander::GuildOffer& offer )
{
	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();

	CString strFMT;

	m_nMode = 2;

	m_dwArticleID = offer.m_dwGuildID; //offer.m_dwID;

	m_pEditArticleTitle->ResetTextSetting();
	m_pEditArticleTitle->ClearText();
	m_pEditArticleTitle->m_strText = pTextLinker->MakeNetToSimpleLinkText(m_pEditArticleTitle, offer.m_strTitle);
	m_pEditArticleTitle->MoveCaretToBack();

	m_pViewGuildName->m_strText = offer.m_strGuildName;

	strFMT.Format( "%d", offer.m_bMinLevel );
	m_pViewLevelMin->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_bMaxLevel );
	m_pViewLevelMax->m_strText = strFMT;

	m_pEditContent->ResetTextSetting();
	m_pEditContent->ClearText();
	m_pEditContent->m_strText = pTextLinker->MakeNetToLinkText(m_pEditContent, offer.m_strText);
	m_pEditContent->MoveCaretToBack();

	m_pBtnOK->m_strText = CTChart::LoadString( TSTR_GUILDOFFEREDIT_MODIFY );
}

void CTGuildRegularOffer::ResetViewInfo( CTGuildCommander::GuildOffer& offer )
{
	CString strFMT;

	m_nMode = 0;

	m_dwArticleID = offer.m_dwGuildID; //offer.m_dwID;

	{
		ResetTextSetting();
		m_pViewArticleTitle->ResetTextSetting();
		m_pViewArticleTitle->ClearText();

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

	strFMT.Format( "%d", offer.m_bMinLevel );
	m_pViewLevelMin->m_strText = strFMT;

	strFMT.Format( "%d", offer.m_bMaxLevel );
	m_pViewLevelMax->m_strText = strFMT;

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

	m_pBtnOK->m_strText = CTChart::LoadString( TSTR_GUILDOFFEREDIT_OK  );
}

void CTGuildRegularOffer::ShowComponent( BOOL bVisible)
{
	CTClientUIBase::ShowComponent( bVisible );

	if( bVisible )
	{
		switch( m_nMode )
		{
		case 0:
			m_pViewArticleTitle->ShowComponent( TRUE );
			m_pViewGuildName->ShowComponent( TRUE );
			m_pViewLevelMin->ShowComponent( TRUE );
			m_pViewLevelMax->ShowComponent( TRUE );
			m_pViewContent->ShowComponent( TRUE );

			m_pEditArticleTitle->ShowComponent( FALSE );
			m_pEditLevelMin->ShowComponent( FALSE );
			m_pEditLevelMax->ShowComponent( FALSE );
			m_pEditContent->ShowComponent( FALSE );

			m_pEditArticleTitle->EnableComponent( FALSE );
			m_pEditLevelMin->EnableComponent( FALSE );
			m_pEditLevelMax->EnableComponent( FALSE );
			m_pEditContent->EnableComponent( FALSE );

			m_pBtnCancel->ShowComponent( FALSE );
			break;

		case 1:
			m_pViewArticleTitle->ShowComponent( FALSE );
			m_pViewGuildName->ShowComponent( TRUE );
			m_pViewLevelMin->ShowComponent( FALSE );
			m_pViewLevelMax->ShowComponent( FALSE );
			m_pViewContent->ShowComponent( FALSE );

			m_pEditArticleTitle->ShowComponent( TRUE );
			m_pEditLevelMin->ShowComponent( TRUE );
			m_pEditLevelMax->ShowComponent( TRUE );
			m_pEditContent->ShowComponent( TRUE );

			m_pEditArticleTitle->EnableComponent( TRUE );
			m_pEditLevelMin->EnableComponent( TRUE );
			m_pEditLevelMax->EnableComponent( TRUE );
			m_pEditContent->EnableComponent( TRUE );

			m_pBtnCancel->ShowComponent( TRUE );
			break;

		case 2:
			m_pViewArticleTitle->ShowComponent( FALSE );
			m_pViewGuildName->ShowComponent( TRUE );
			m_pViewLevelMin->ShowComponent( TRUE );
			m_pViewLevelMax->ShowComponent( TRUE );
			m_pViewContent->ShowComponent( FALSE );

			m_pEditArticleTitle->ShowComponent( TRUE );
			m_pEditLevelMin->ShowComponent( FALSE );
			m_pEditLevelMax->ShowComponent( FALSE );
			m_pEditContent->ShowComponent( TRUE );

			m_pEditArticleTitle->EnableComponent( TRUE );
			m_pEditLevelMin->EnableComponent( FALSE );
			m_pEditLevelMax->EnableComponent( FALSE );
			m_pEditContent->EnableComponent( TRUE );

			m_pBtnCancel->ShowComponent( TRUE );
			break;
		}

		SwitchFocus( m_pEditArticleTitle );
	}	
}

void CTGuildRegularOffer::ResetNewEdit()
{
	CString strFMT;

	m_nMode = 1;

	m_dwArticleID = 0;

	m_pEditArticleTitle->ClearText();
	m_pViewGuildName->m_strText = CTClientGame::GetInstance()->GetMainChar()->m_strGUILD;

	m_pEditLevelMin->ClearText();
	m_pEditLevelMin->SetText( "1" );

	CString strMaxLevel;
	strMaxLevel.Format( "%d", TMAX_CHARLEVEL );

	m_pEditLevelMax->ClearText();
	m_pEditLevelMax->SetText( strMaxLevel );

	m_pEditContent->ClearText();

	m_pBtnOK->m_strText = CTChart::LoadString( TSTR_GUILDOFFEREDIT_WRITE );
}

INT CTGuildRegularOffer::GetMode()
{
	return m_nMode;
}

void CTGuildRegularOffer::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
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

TEdit* CTGuildRegularOffer::GetCurEditLink()
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

void CTGuildRegularOffer::OnLButtonDown( UINT nFlags, CPoint pt)
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