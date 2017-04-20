#include "Stdafx.h"
#include "TGuildNotifyDlg.h"
#include "TClientGame.h"
#include "Resource.h"

// ====================================================================
const BUTTON_STATE CTGuildNotifyDlg::MODE_BTN_VIEW[MODE_COUNT][BUTTON_COUNT] =
{
	//MODE_NORMAL,
	{	
		{0, 0, 0, 0 },							
		{0,	0, 0, 0 },
		{0,	0, 0, 0 },
	},								
	
	//MODE_MASTER,
	{ 
		{1, 1, GM_NEW_NOTIFY, TSTR_NEW_NOTIFY },
		{1,	1, GM_MOD_NOTIFY, TSTR_MODIFY},
		{1,	1, GM_DELETE_NOTIFY, TSTR_DELETE},
	},		
	
	//MODE_NEW,	
	{ 
		{0,	0, 0, 0 },
		{1, 1, GM_NEW_NOTIFY_OK, TSTR_OK },		
		{1,	1, GM_NEW_NOTIFY_CANCEL, TSTR_CANCEL},
	},	
	
	//MODE_EDIT,
	{ 
		{0,	0, 0, 0 },
		{1, 1, GM_MOD_NOTIFY_OK, TSTR_OK },		
		{1,	1, GM_MOD_NOTIFY_CANCEL, TSTR_CANCEL} ,
	}, 	
};
// ====================================================================


// ====================================================================
CTGuildNotifyDlg::CTGuildNotifyDlg( TComponent *pParent, LP_FRAMEDESC pDesc)
	: CTClientUIBase(pParent, pDesc), m_eMode(MODE_NORMAL), m_nPrvSel(T_INVALID)
{
	m_pList = static_cast<TList*>( FindKid(ID_CTRLINST_LIST_NOTIFY) );
	m_pViewContents = static_cast<TList*>( FindKid(ID_CTRLINST_LIST) );

	m_pViewAuthor = FindKid(ID_CTRLINST_TXT_AUTHOR);
	m_pViewTitle = FindKid(ID_CTRLINST_TXT_TITLE);

	m_pEditTitle = static_cast<TEdit*>( FindKid(ID_CTRLINST_EDIT_TITLE) );
	
	m_pBTN[0] = FindKid(ID_CTRLINST_BTN_1);
	m_pBTN[1] = FindKid(ID_CTRLINST_BTN_2);
	m_pBTN[2] = FindKid(ID_CTRLINST_BTN_3);

	TComponent* pComp = FindKid(ID_CTRLINST_EDIT);
	RemoveKid(pComp);

	m_pEditContents = new TMultiLineEdit(this, pComp->m_pDESC);
	m_pEditContents->m_id = pComp->m_id;
	AddKid(m_pEditContents);

	delete pComp;
	ShowComponent(FALSE);
}
// --------------------------------------------------------------------
CTGuildNotifyDlg::~CTGuildNotifyDlg()
{
}
// ====================================================================

// ====================================================================
void CTGuildNotifyDlg::UpdateInfo()
{
	CTTextLinker *pTextLinker = CTTextLinker::GetInstance();

	m_pList->RemoveAll();
	m_pList->ResetTextSetting();

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	INT nCNT = (INT)pCmd->GetCountGuildNotify();
	for(INT i=0; i<nCNT; ++i)
	{
		const GuildNotify& info = pCmd->GetGuildNotify(i);

		m_pList->AddString(
			pTextLinker->MakeNetToLinkText( NULL, info.m_strTitle ),
			COL_TITLE);

		m_pList->SetItemString(i, COL_DATE, info.m_strDate);
	}

	m_nPrvSel = T_INVALID;

	if( !IsEditMode() )
		UpdateByListSel();
}
// ====================================================================
void CTGuildNotifyDlg::ResetMode()
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	CTClientChar* pMainChar = pGame->GetMainChar();

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	const CTGuildCommander::GuildDetInfo& info = pCmd->GetGuildDetInfo();

	if( info.m_strGMaster == pMainChar->m_strNAME )
		m_eMode = MODE_MASTER;
	else
		m_eMode = MODE_NORMAL;

	UpdateCompByMode();
}
// --------------------------------------------------------------------
void CTGuildNotifyDlg::UpdateCompByMode()
{
	for(INT i=0; i<BUTTON_COUNT; ++i)
	{
		const BUTTON_STATE& info = MODE_BTN_VIEW[m_eMode][i];

		m_pBTN[i]->ShowComponent(info.m_bShow);
		m_pBTN[i]->EnableComponent(info.m_bEnable);
		m_pBTN[i]->m_menu[TNM_LCLICK] = info.m_dwBtnGM;

		if( info.m_dwBtnTitle )
			m_pBTN[i]->m_strText = CTChart::LoadString( (TSTRING) info.m_dwBtnTitle);
		else
			m_pBTN[i]->m_strText.Empty();
	}
		
	BOOL bEDIT = IsEditMode();
	
	m_pViewTitle->ShowComponent(!bEDIT);
	m_pEditTitle->ShowComponent(bEDIT);
	m_pViewContents->ShowComponent(!bEDIT);
	m_pEditContents->ShowComponent(bEDIT);

	m_pList->EnableComponent(!bEDIT);

	if( bEDIT )
	{
		m_pEditTitle->ClearText();
		m_pEditContents->ClearText();

		m_pEditTitle->ResetTextSetting();
		m_pEditContents->ResetTextSetting();

		SwitchFocus(m_pEditTitle);
	}
	else
		UpdateByListSel();
}
// ====================================================================
void CTGuildNotifyDlg::StartNew()
{
	m_eMode = MODE_NEW;

	UpdateCompByMode();
}
// --------------------------------------------------------------------
void CTGuildNotifyDlg::EndNew(BOOL bOK)
{
	if( bOK )
	{
		CTTextLinker *pTTextLinker = CTTextLinker::GetInstance();

		CTGuildCommander::GetInstance()->RequestGuildNewNotify(
			pTTextLinker->MakeLinkToNetText( m_pEditTitle, TRUE, MAX_BOARD_TITLE),
			pTTextLinker->MakeLinkToNetText( m_pEditContents, TRUE, MAX_BOARD_TEXT));
	}

	ResetMode();
}
// ====================================================================
void CTGuildNotifyDlg::StartModify()
{
	CTTextLinker *pTextLinker = CTTextLinker::GetInstance();

	m_nModIdx = m_pList->GetSel();

	if( m_nModIdx >= 0 )
	{
		m_eMode = MODE_EDIT;
		UpdateCompByMode();

		CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
		const GuildNotify& info = pCmd->GetGuildNotify(m_nModIdx);

		m_pEditTitle->ResetTextSetting();
		m_pEditTitle->SetText(
			pTextLinker->MakeNetToSimpleLinkText(m_pEditTitle, info.m_strTitle) );

		m_pEditContents->ResetTextSetting();
		m_pEditContents->ClearText();
		m_pEditContents->m_strText = pTextLinker->MakeNetToLinkText(m_pEditContents, info.m_strText);
		m_pEditContents->MoveCaretToBack();
	}
}
// --------------------------------------------------------------------
void CTGuildNotifyDlg::EndModify(BOOL bOK)
{
	if( bOK && m_nModIdx >= 0 )
	{
		CTTextLinker *pTTextLinker = CTTextLinker::GetInstance();
		CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
		const GuildNotify& info = pCmd->GetGuildNotify(m_nModIdx);

		pCmd->RequestGuildModNotify(
			info.m_dwID,
			pTTextLinker->MakeLinkToNetText( m_pEditTitle, TRUE, MAX_BOARD_TITLE),
			pTTextLinker->MakeLinkToNetText( m_pEditContents, TRUE, MAX_BOARD_TEXT));
	}

	ResetMode();
}
// ====================================================================
void CTGuildNotifyDlg::Delete()
{
	m_nModIdx = m_pList->GetSel();

	if( m_nModIdx >= 0 )
	{
		CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
		const GuildNotify& info = pCmd->GetGuildNotify(m_nModIdx);

		pCmd->RequestGuildDelNotify(info.m_dwID);
	}
}
// ====================================================================
BOOL CTGuildNotifyDlg::IsEditMode()
{
	return ( m_eMode == MODE_NEW || m_eMode == MODE_EDIT );
}
// --------------------------------------------------------------------
TEdit* CTGuildNotifyDlg::GetCurEdit()
{
	if( !IsVisible() || !IsEditMode() )
		return NULL;

	if( m_pEditTitle->CanProcess() && m_pFocus == m_pEditTitle )
		return m_pEditTitle;

	if( m_pEditContents->CanProcess() && m_pFocus == m_pEditContents )
		return m_pEditContents;

	return NULL;
}
// ====================================================================

// ====================================================================
void CTGuildNotifyDlg::UpdateByListSel()
{
	int nSel = m_pList->GetSel();
	if( m_nPrvSel == T_INVALID || m_nPrvSel != nSel )
	{
		if( nSel >= 0 )
		{
			CTTextLinker *pTextLinker = CTTextLinker::GetInstance();

			CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
			const GuildNotify& info = pCmd->GetGuildNotify(nSel);

			m_pViewAuthor->m_strText = info.m_strAuthor;
			m_pViewTitle->m_strText = pTextLinker->MakeNetToSimpleLinkText( m_pViewTitle, info.m_strTitle); //m_pViewTitle->m_strText = info.m_strTitle;

			m_pViewContents->RemoveAll();
			m_pViewContents->ResetTextSetting();

			TLLINESETTING_ARRAY2D vSETTINGS2D;
			TTLINKLINE_ARRAY vLINES;
			INT nPos = 0;

			CString strLINE = pTextLinker->SplitTextByCrLf( info.m_strText, CTTextLinker::LINK_DEF_TOK, nPos, FALSE);
			while(!strLINE.IsEmpty())
			{
				CString strPMSG = pTextLinker->MakeNetToLinkText( this, strLINE);
				strPMSG.Remove('\r');

				CSize szITEM = m_pViewContents->GetItemSize(0);
				pTextLinker->SplitTextByComp( m_pViewContents, szITEM.cx, strPMSG, vLINES);
				pTextLinker->GetTextSettings( this, vLINES, vSETTINGS2D);

				INT nLineSize = (INT) vLINES.size();
				for( INT i=0; i<nLineSize; i++)
				{
					const TTEXT_LINK_LINE& line = vLINES[i];
					const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[i];

					INT iRow = m_pViewContents->AddString(line.strMSG);
					for( INT k=0; k<INT(vLineSettings.size()); k++)
					{
						const TLINK_LINE_SETTING& s = vLineSettings[k];
						m_pViewContents->AddTextSetting( iRow, 0, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
					}
				}

				vLINES.clear();
				vSETTINGS2D.clear();
				ResetTextSetting();

				strLINE = pTextLinker->SplitTextByCrLf( info.m_strText, CTTextLinker::LINK_DEF_TOK, nPos, FALSE);
			}
		}
		else
		{
			m_pViewAuthor->m_strText.Empty();
			m_pViewTitle->m_strText.Empty();
			m_pViewTitle->ResetTextSetting();
			m_pViewContents->RemoveAll();
			m_pViewContents->ResetTextSetting();
		}
	}

	m_nPrvSel = nSel;
}
// ====================================================================
void CTGuildNotifyDlg::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if(!CanProcess()) 
		return;

	if(HitTest(pt))
	{
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();
		CTClientGame *pTGAME = CTClientGame::GetInstance();
		DWORD dwInfo;

		if( m_pViewContents->HitTestTextSetting(pt, &dwInfo) )
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

		if( m_pViewTitle->HitTestTextSetting(pt, &dwInfo) )
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

		SwitchFocus(m_pViewContents);
	}

	CTClientUIBase::OnLButtonDown( nFlags, pt);
}
// ====================================================================
void CTGuildNotifyDlg::OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags)
{
	if(!CanProcess()) 
		return;

	if( nChar == VK_RETURN && m_pEditContents->GetFocus() )
	{
		m_pEditContents->InsertReturnChar();
		m_pEditContents->MoveScrollToCaret(FALSE);
	}
	else
		CTClientUIBase::OnKeyDown(nChar,nRepCnt,nFlags);
}
// ====================================================================
void CTGuildNotifyDlg::ShowComponent( BOOL bVisible )
{
	if( m_bVisible == bVisible )
		return;

	CTClientUIBase::ShowComponent(bVisible);

	if( bVisible )
	{
		m_bVisible = bVisible;

		ResetMode();

		CTGuildCommander::GetInstance()
			->RequestGuildNotifyList();
	}
}
// ====================================================================
HRESULT CTGuildNotifyDlg::Render( DWORD dwTickCount )
{
	if( IsVisible() )
		UpdateByListSel();

	return CTClientUIBase::Render(dwTickCount);
}
// ====================================================================



















