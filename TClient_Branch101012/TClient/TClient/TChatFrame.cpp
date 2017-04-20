#include "Stdafx.h"
#include "TClientGame.h"
#include "TMiniPopupDlg.h"
#include "TClient.h"
#include "Curse.h"
#include "TClientWnd.h"


// ===============================================================================
#define REG_KEY					"ChatInterface"
#define REG_SET_MAXLIST			"MAXLIST"
#define REG_SET_CHATPOSX		"CHATPOSX%d"
#define REG_SET_CHATPOSY		"CHATPOSY%d"
#define REG_SET_CHATTAB			"CHATTAB%d"
#define REG_SET_CHATFLAG		"CHATFLAG%d"
#define REG_SET_CHATBASIS		"CHATBASIS%d"
#define REG_SET_CHATHEIGHT	"CHATHEIGHT%d"
#define REG_SET_NOTCHATPOSX		"NOTCHATX2"
#define REG_SET_NOTCHATPOSY		"NOTCHATY2"
#define REG_SET_NOTCHATBASIS		"NOTCHATBASIS2"
#define REG_SET_SHOWCHATPOSX		"SHOWCHATX"
#define REG_SET_SHOWCHATPOSY		"SHOWCHATY"
#define REG_SET_SHOWCHATBASIS		"SHOWCHATBASIS"
#define REG_RESET							"RESET"
#define CHATFRAME_RESET_IDENTITY			(0x00000023)
// ===============================================================================
const LONG		CTChatFrame::SNAP_REACT_VALUE = 15;
const CHAR		CTChatFrame::CHAT_CMD_MARK = '/';
// -------------------------------------------------------------------------------
const DWORD		CTChatFrame::CHAT_FLAGS[TCHAT_FLAG_COUNT] = 
{
	TCHAT_FLAG_WHISPER,	
	TCHAT_FLAG_NEAR,		
	TCHAT_FLAG_MAP,		
	(DWORD)T_INVALID,	
	TCHAT_FLAG_PARTY,	
	TCHAT_FLAG_GUILD_TACTICS,	
	TCHAT_FLAG_INFO,	
	TCHAT_FLAG_FORCE	
};
const DWORD		CTChatFrame::CHAT_FLAG_STRINGS[TCHAT_FLAG_COUNT] = 
{
	TSTR_CHATFLAG_WHISPER,				
	TSTR_CHATFLAG_NEAR,		
	TSTR_CHATFLAG_MAP,	
	(DWORD)T_INVALID,	
	TSTR_CHATFLAG_PARTY,	
	TSTR_CHATFLAG_GUILD,	
	TSTR_CHATFLAG_INFO,	
	TSTR_CHATFLAG_FORCE	
};
const DWORD		CTChatFrame::DEFAULT_CHAT_FLAGS[] = 
{
	TCHAT_FLAG_ALL_CHAT,						// 전체
	TCHAT_FLAG_ALL_CHAT_WITH_INFO,		// 일반
	TCHAT_FLAG_WHISPER,				// 귓말
	TCHAT_FLAG_PARTY,					// 일행
	TCHAT_FLAG_GUILD_TACTICS,					// 길드
	TCHAT_FLAG_FORCE,					// 전대
	TCHAT_FLAG_INFO						// 결과
};
const BYTE		CTChatFrame::CHAT_SHOW_NEWMSG[] = 
{
	FALSE,								// 전체
	FALSE,								// 일반
	TRUE,								// 귓말
	TRUE,								// 일행
	TRUE,								// 길드
	TRUE,								// 전대
	FALSE,								// 결과
};
const INT		CTChatFrame::POPUP_TO_LIST[] =
{
	T_INVALID,						// POPUP_CHATTYPE
	TCHAT_SET_ALL,				// POPUP_CHATFLAG_ALL,					
	TCHAT_SET_NORMAL,			// POPUP_CHATFLAG_NORMAL,
	T_INVALID,						// POPUP_LATEST_WHISPER
};
const DWORD		CTChatFrame::CHAT_MODE_CMD[] = 
{
	TSTR_KEYWORD_CHAT_WHISPER,			// CHAT_WHISPER
	TSTR_KEYWORD_CHAT_NORMAL,			// CHAT_NEAR
	TSTR_KEYWORD_CHAT_NATION,			// CHAT_MAP
	(DWORD)T_INVALID,					// CHAT_WORLD
	TSTR_KEYWORD_CHAT_PARTY,				// CHAT_PARTY
	TSTR_KEYWORD_CHAT_GUILD,				// CHAT_GUILD
	TSTR_KEYWORD_CHAT_FORCE,				// CHAT_FORCE
	(DWORD)T_INVALID,					// CHAT_OPERATOR	
	TSTR_KEYWORD_CHAT_TACTICS, // CHAT_TACTICS
};
// ===============================================================================

// ===============================================================================
CTChatFrame::CTChatFrame(TComponent *pParent, LP_FRAMEDESC pDesc, CTClientGame* pMainGame, TCMLParser *pParser)
	: CTClientUIBase(pParent, pDesc), m_pSelectTab(NULL), m_pHitTab(NULL),
	  m_iTabCount(TCHAT_SET_COUNT), m_dwTick(0), m_bChatEditON(FALSE),
	  m_bLock(FALSE), m_pMainGame(pMainGame), m_bChatMode(CHAT_MAP),
	  m_dwChatTargetID(0), m_pChatEditBack(NULL), m_pChatTabBack(NULL), m_dwNotifyTime(0)
{
	static const DWORD dwTabTitle[] = 
	{
		TSTR_CHAT_TAB_ALL,				// 전체
		TSTR_CHAT_TAB_NORMAL,			// 일반
		TSTR_CHAT_TAB_WHISPER,			// 귓말
		TSTR_CHAT_TAB_PARTY,				// 일행
		TSTR_CHAT_TAB_GUILD,				// 길드
		TSTR_CHAT_TAB_FORCE,				// 전대
		TSTR_CHAT_TAB_RESULT,			// 결과
	};

	static const DWORD dwChatTab[] = 
	{
		ID_CTRLINST_CHAT_TAB0,
		ID_CTRLINST_CHAT_TAB1,
		ID_CTRLINST_CHAT_TAB2,
		ID_CTRLINST_CHAT_TAB3,
		ID_CTRLINST_CHAT_TAB4,
		ID_CTRLINST_CHAT_TAB5,
		ID_CTRLINST_CHAT_TAB6
	};

	ZeroMemory(m_vPopup, sizeof(m_vPopup));

	m_pListTemplate = FindKid( ID_CTRLINST_CHAT_LIST );
	RemoveKid( m_pListTemplate );
	m_pListTemplate->GetComponentRect(&m_rcDefaultList);
	TComponent* pListTemplate2 = FindKid( ID_CTRLINST_CHAT_LIST2 );
	RemoveKid( pListTemplate2 );
	TComponent* pListTemplate3 = FindKid( ID_CTRLINST_CHAT_LIST3 );
	RemoveKid( pListTemplate3 );

	LP_FRAMEDESC pSizeBarFrameDesc = CTClientGame::GetInstance()->m_pTParser->FindFrameTemplate( ID_FRAME_SIZEBAR );
	TFrame* pSizeBarFrame = new TFrame( NULL, pSizeBarFrameDesc );
	TButton* pBtn = (TButton*) pSizeBarFrame->FindKid( ID_CTRLINST_BTN );
	LP_FRAMEDESC pSizeBarButtonDesc = pBtn->m_pDESC;

	CString title;
	for( INT i=0; i<TCHAT_SET_COUNT; ++i)
	{
		TComponent* pTabTemplate = FindKid( dwChatTab[i] );
		RemoveKid( pTabTemplate );

		pTabTemplate->GetComponentRect(&m_vTabRect[i]);

		m_vTabs[i] = new CTChatTabButton(this, pTabTemplate->m_pDESC);
		m_vTabs[i]->m_id = GetUniqueID(NULL);
		
		delete pTabTemplate;

		m_vLists[i] = new CTChatList(this, m_pListTemplate->m_pDESC, pParent);
		m_vLists[i]->m_id = GetUniqueID(pParent);
		m_vLists[i]->m_bHitTestType = HITTEST_TYPE_RECT;

		m_vLists[i]->m_pSizeBarUP = new TButton( m_vLists[i], pSizeBarButtonDesc );
		m_vLists[i]->m_pSizeBarDOWN = new TButton( m_vLists[i], pSizeBarButtonDesc );

		m_vLists[i]->m_pSizeBarUP->m_id = GetUniqueID( m_vLists[i]);
		m_vLists[i]->m_pSizeBarDOWN->m_id = GetUniqueID( m_vLists[i]);

		m_vLists[i]->AddKid( m_vLists[i]->m_pSizeBarUP );
		m_vLists[i]->AddKid( m_vLists[i]->m_pSizeBarDOWN );

		CRect rt;
		m_vLists[i]->GetComponentRect(&rt);

		TFRAMEOFFSET offset;
		CTClientUIBase::GetUIOffset( CTClientUIBase::m_rtDEFAULT_SCREEN, rt, &offset);
		// Arab Ver.
		m_vLists[i]->SetBasis(offset.m_bBasisPoint);
		m_vLists[i]->SetOffset(offset.m_vOffset);
				
		title = CTChart::LoadString( (TSTRING) dwTabTitle[i]);
        m_vLists[i]->Init( i, title, DEFAULT_CHAT_FLAGS[i], m_rcDefaultList, NULL);
		m_vTabs[i]->Init( i, m_vLists[i] );

		AddKid( m_vTabs[i] );
		AddKid( m_vLists[i] );
	}

	m_pNotifyList = new CTChatList(this, pListTemplate3->m_pDESC, pParent);
	m_pNotifyList->m_id = GetUniqueID(pParent);
	m_pNotifyList->m_bHitTestType = HITTEST_TYPE_RECT;
	m_pNotifyList->SetHideBackground(TRUE);
	m_pNotifyList->SetOuter(TRUE);
	m_pNotifyList->ShowComponent(FALSE);
	m_pNotifyList->m_bBasisPoint = TBASISPOINT_LEFT_BOTTOM;
	m_pNotifyList->m_vCompOffset = CPoint(0, -262);

	m_pUpgradeShowList = new CTChatList(this, pListTemplate2->m_pDESC, pParent);
	m_pUpgradeShowList->m_id = GetUniqueID(pParent);
	m_pUpgradeShowList->m_bHitTestType = HITTEST_TYPE_RECT;
	m_pUpgradeShowList->SetHideBackground(TRUE);
	m_pUpgradeShowList->SetOuter(TRUE);
	m_pUpgradeShowList->ShowComponent(FALSE);
	m_pUpgradeShowList->m_bBasisPoint = TBASISPOINT_LEFT_BOTTOM;
	m_pUpgradeShowList->m_vCompOffset = CPoint(0, -194);

	delete pSizeBarFrame;
	delete pListTemplate2;
	delete pListTemplate3;

	m_pChatEditCtrl = static_cast<TEdit*>( FindKid(ID_CTRLINST_CHAT_INPUT) );
	m_pChatWhisperTargetEditCtrl = static_cast<TEdit*>( FindKid(ID_CTRLINST_CHAT_TYPE_EDIT) );
	m_pChatMode = FindKid( ID_CTRLINST_CHAT_MODE );

	m_pChatTypeCtrl = FindKid(ID_CTRLINST_CHAT_TYPE);
	m_pChatTypeCtrl->m_strText = CTChart::LoadString( TSTR_CHAT_NORMAL );

	m_pChatImeCtrl = static_cast<TButton*>( FindKid(ID_CTRLINST_CHAT_LANGUAGE) );
	m_pChatImeCtrl->SetStateButton();
	NotifyImeLocalModeChange();

	m_pChatEditBack = FindKid(ID_CTRLINST_CHAT_INPUT_BASE);
	m_pChatTabBack = FindKid(ID_CTRLINST_CHAT_BOTTOM);

	m_bSND = FALSE;
	m_ptPrev = CPoint(0,0);
	m_tip.Reset();
	m_blDragging = FALSE;
	m_pFocus = NULL;
	m_bChatFrameVisible = m_bVisible;

	ChangeSelection(m_vTabs[0]);
	SetLock(TRUE);

	CString strTMARK, strCMD, strTOK;
	strTMARK.AppendChar(CHAT_CMD_MARK);
	for( i=0; i<CHAT_COUNT; ++i )
	{
		if( CHAT_MODE_CMD[i] == (DWORD)T_INVALID )
			continue;

		strCMD = CTChart::LoadString( (TSTRING) CHAT_MODE_CMD[i]);
		strCMD.MakeUpper();
		
		INT nPOS = 0;
		strTOK = strCMD.Tokenize(strTMARK, nPOS);
		while( !strTOK.IsEmpty() )
		{
			strTOK.Trim();
			AddChatCmdProc(strTOK, OnChatModeCmd, (DWORD)this);	
			strTOK = strCMD.Tokenize(strTMARK, nPOS);
		}
	}

	strCMD = CTChart::LoadString( TSTR_KEYWORD_CHAT_TITLE );
	strCMD.MakeUpper();

	INT nPOS = 0;
	strTOK = strCMD.Tokenize( strTMARK, nPOS);
	while( !strTOK.IsEmpty())
	{
		strTOK.Trim();
		AddChatCmdProc( strTOK, OnChatTitleCmd, (DWORD) this);
		strTOK = strCMD.Tokenize( strTMARK, nPOS);
	}
}
// -------------------------------------------------------------------------------
CTChatFrame::~CTChatFrame()
{
	for( INT i=0; i<POPUP_COUNT; ++i )
	{
		if( m_vPopup[i] )
		{
			RemoveKid(m_vPopup[i]);
			delete m_vPopup[i];
		}
	}

	delete m_pListTemplate;
}
// ===============================================================================
DWORD CTChatFrame::GetChatTypeColor(BYTE bChatMode)
{
	switch(bChatMode)
	{
	case CHAT_WHISPER	:  return TCOLOR_CHAT_WHISPER;
	case CHAT_NEAR		:  return TCOLOR_CHAT_NEAR;
	case CHAT_MAP		:  return TCOLOR_CHAT_MAP;
	case CHAT_WORLD		:  return TCOLOR_CHAT_WORLD;
	case CHAT_PARTY		:  return TCOLOR_CHAT_PARTY;
	case CHAT_GUILD		:  return TCOLOR_CHAT_GUILD;
	case CHAT_TACTICS	: return TCOLOR_CHAT_TACTICS;
	case CHAT_FORCE		:  return TCOLOR_CHAT_FORCE;
	case CHAT_OPERATOR	:  return TCOLOR_CHAT_OPERATOR;
	case CHAT_SHOW	:	return TCOLOR_CHAT_WORLD;
	}

	return 0;
}
// -------------------------------------------------------------------------------
const CString& CTChatFrame::GetChatTypeString(BYTE bChatMode)
{
	static const DWORD dwChatID[] = {
		TSTR_CHAT_WHISPER,			// CHAT_WHISPER
		TSTR_CHAT_NORMAL,			// CHAT_NEAR
		TSTR_CHAT_NATION,			// CHAT_MAP
		TSTR_CHAT_WORLD,				// CHAT_WORLD
		TSTR_CHAT_PARTY,				// CHAT_PARTY
		TSTR_CHAT_GUILD,				// CHAT_GUILD
		TSTR_CHAT_FORCE,				// CHAT_FORCE
		TSTR_CHAT_OPERATOR,	// CHAT_OPERATOR
		TSTR_CHAT_TACTICS, // CHAT_TACTICS
		TSTR_CHAT_SHOW }; // CHAT_SHOW

	static CString str;
	str = CTChart::LoadString( (TSTRING) dwChatID[bChatMode]);

	return str;
}
// -------------------------------------------------------------------------------
const CString& CTChatFrame::GetChatTypeRepresentation(BYTE bChatMode)
{
	static const DWORD dwChatType[TCHAT_POPUP_COUNT] = 
	{
		TSTR_REPRESENTATION_WHISPER,			// TCHAT_POPUP_WHISPER,	
		TSTR_REPRESENTATION_NORMAL,			// TCHAT_POPUP_NEAR,		
		TSTR_REPRESENTATION_NATION,			// TCHAT_POPUP_MAP,		
		TSTR_REPRESENTATION_PARTY,			// TCHAT_POPUP_PARTY,		
		TSTR_REPRESENTATION_GUILD,			// TCHAT_POPUP_GUILD,		
		TSTR_REPRESENTATION_FORCE,			// TCHAT_POPUP_FORCE,		
		TSTR_REPRESENTATION_GM				// TCHAT_POPUP_GM,			
	};

	static CString str;
	str = CTChart::LoadString( (TSTRING) dwChatType[bChatMode]);
	return str;
}
// ===============================================================================
INT CTChatFrame::GetPopupIndex(INT nListIndex)
{
	for(INT i=0; i<POPUP_COUNT; ++i)
	{
		if( POPUP_TO_LIST[i] == nListIndex )
			return i;
	}

	return T_INVALID;
}
// ===============================================================================





// ===============================================================================
void OnChatTypeSelect(CTMiniPopupDlg* pPopupDlg, CTMiniPopupDlg::CTButtonMP* pButton)
{
	CTChatFrame* pChatFrame = static_cast<CTChatFrame*>(pPopupDlg->GetParent());

	BYTE bBtnIndex = (BYTE)pButton->GetUserData();
	if( bBtnIndex >= TCHAT_POPUP_COUNT )
		return;

	switch( bBtnIndex )
	{
	case TCHAT_POPUP_WHISPER :
		{
			pChatFrame->ResetChatMsg(TRUE);
			pChatFrame->SetChatMode(CHAT_WHISPER, "" );
			pChatFrame->ShowWhisperTargetInput(TRUE);
			//pChatFrame->PushFrontMsgToEdit( "/" + pButton->m_strText + " " );
		}
		break;

	case TCHAT_POPUP_NEAR :	
		pChatFrame->SetChatMode(CHAT_NEAR);
		break;
	case TCHAT_POPUP_MAP :
		pChatFrame->SetChatMode(CHAT_MAP);
		break;
	case TCHAT_POPUP_PARTY :
		pChatFrame->SetChatMode(CHAT_PARTY);
		break;
	case TCHAT_POPUP_GUILD :
		pChatFrame->SetChatMode(CHAT_GUILD);
		break;
	case TCHAT_POPUP_FORCE :
		pChatFrame->SetChatMode(CHAT_FORCE);
		break;

	case TCHAT_POPUP_GM :
		{
			CString strGM;
			strGM = CTChart::LoadString( TSTR_GM_NAME);
			pChatFrame->SetChatMode(CHAT_WHISPER, strGM);
		}
		break;
	}

	pChatFrame->ShowPopup(CTChatFrame::POPUP_CHATTYPE, FALSE);
} 
// -------------------------------------------------------------------------------
void OnChatFlagSelect(CTMiniPopupDlg* pPopupDlg, CTMiniPopupDlg::CTButtonMP* pButton)
{
	CTChatFrame* pChatFrame = static_cast<CTChatFrame*>(pPopupDlg->GetParent());
	CTChatList* pList = pChatFrame->FindList(pPopupDlg);
	if( pList )
	{
		DWORD dwCurFlag = pList->GetChatFlag();
		DWORD dwBtnFlag = pButton->GetUserData();
		
		if( dwCurFlag & dwBtnFlag )
			dwCurFlag &= ~dwBtnFlag;
		else
			dwCurFlag |= dwBtnFlag;

		pList->SetChatFlag(dwCurFlag);
		pChatFrame->UpdateChatFlagToPopup();
	}
} 
// -------------------------------------------------------------------------------
void OnChatWhisperSelect(CTMiniPopupDlg* pPopupDlg, CTMiniPopupDlg::CTButtonMP* pButton)
{
	CTChatFrame* pChatFrame = static_cast<CTChatFrame*>(pPopupDlg->GetParent());
	pChatFrame->SetChatMode( CHAT_WHISPER, pButton->m_strText );
	pChatFrame->ShowPopup(CTChatFrame::POPUP_LATEST_WHISPER, FALSE);
} 
// ===============================================================================
void CTChatFrame::SetPopup(PopupEnum ePopup, CTMiniPopupDlg* pPopup)
{
	if( m_vPopup[ePopup] )
	{
		RemoveKid(m_vPopup[ePopup]);
		delete m_vPopup[ePopup];
	}

	m_vPopup[ePopup] = pPopup;
	m_vPopup[ePopup]->ClearButtons();
	m_vPopup[ePopup]->m_bSND = FALSE;

	switch(ePopup)
 	{
	case POPUP_CHATTYPE:
		{
			for(BYTE i=0; i<TCHAT_POPUP_COUNT; ++i)
			{
				const CString& strType = GetChatTypeRepresentation(i);
				m_vPopup[ePopup]->AddButton(strType, OnChatTypeSelect, i);
			}

			CSize sz; CPoint pt;
			m_vPopup[ePopup]->GetComponentSize(&sz);	
			m_pChatMode->GetComponentPos(&pt);
			m_vPopup[ePopup]->MoveComponent( CPoint(pt.x, pt.y-sz.cy) );
		}
		break;
	case POPUP_CHATFLAG_ALL:
	case POPUP_CHATFLAG_NORMAL:
		{
			CString strTitle;
			INT nListIndex = POPUP_TO_LIST[ePopup];

			for(BYTE i=0; i<TCHAT_FLAG_COUNT; ++i)
			{
				if( DEFAULT_CHAT_FLAGS[nListIndex] & CHAT_FLAGS[i] )
				{
					if( CHAT_FLAG_STRINGS[i] != (DWORD)T_INVALID )
					{
						strTitle = CTChart::LoadString( (TSTRING) CHAT_FLAG_STRINGS[i]);
						m_vPopup[ePopup]->AddButton(strTitle, OnChatFlagSelect, CHAT_FLAGS[i]);
					}
				}
			}
		}
		break;
	case POPUP_LATEST_WHISPER:
		{
			CSize sz; CPoint pt;
			m_vPopup[ePopup]->GetComponentSize(&sz);	
			m_pChatTypeCtrl->GetComponentPos(&pt);
			m_vPopup[ePopup]->MoveComponent( CPoint(pt.x, pt.y-sz.cy) );
		}
		break;
	}

	AddKid( m_vPopup[ePopup] );
	m_vPopup[ePopup]->ShowComponent(FALSE);
}
// -------------------------------------------------------------------------------
void CTChatFrame::ShowPopup(PopupEnum ePopup, BOOL bShow)
{
	if( !m_vPopup[ePopup] )
		return;
	
	if( bShow )
	{
		RemoveKid(m_vPopup[ePopup]);
		AddKid(m_vPopup[ePopup]);

		HideAllPopup();
		ResetTick();
	}
	
	m_vPopup[ePopup]->ShowComponent(bShow);

	if( bShow && POPUP_TO_LIST[ePopup] != T_INVALID )
	{
		INT nListIndex = POPUP_TO_LIST[ePopup];
		CTChatTabButton* pTab = FindTab( m_vLists[nListIndex] );
		
		CSize sz; CPoint pt;
		m_vPopup[ePopup]->GetComponentSize(&sz);	
		pTab->GetComponentPos(&pt);
		m_vPopup[ePopup]->MoveComponent( CPoint(pt.x, pt.y-sz.cy) );

		UpdateChatFlagToPopup();
	}
	else if( bShow && ePopup == POPUP_LATEST_WHISPER )
	{
		CSize sz; CPoint pt;
		m_vPopup[ePopup]->GetComponentSize(&sz);	
		m_pChatTypeCtrl->GetComponentPos(&pt);
		m_vPopup[ePopup]->MoveComponent( CPoint(pt.x, pt.y-sz.cy) );
	}
}
// -------------------------------------------------------------------------------
void CTChatFrame::HideAllPopup()
{
	for( INT i=0; i<POPUP_COUNT; ++i )
	{
		if( m_vPopup[i] )
			m_vPopup[i]->ShowComponent(FALSE);
	}
}
// -------------------------------------------------------------------------------
BOOL CTChatFrame::IsVisiblePopup(PopupEnum ePopup)
{ 
	return (m_vPopup[ePopup] && m_vPopup[ePopup]->IsVisible());
}
// ===============================================================================
void CTChatFrame::UpdateChatFlagToPopup()
{
	for(INT i=0; i<POPUP_COUNT; ++i)
	{
		if( !m_vPopup[i]->IsVisible() )
			continue;

		INT nListIdx = POPUP_TO_LIST[i];
		if( nListIdx == T_INVALID )
			continue;

		INT nBtnCnt = m_vPopup[i]->GetButtonCount();
		for(INT j=0; j<nBtnCnt; ++j)
		{
			DWORD dwBtnFlag = m_vPopup[i]->GetButtonUserData(j);
			m_vPopup[i]->SelectButton(j, m_vLists[nListIdx]->CheckChatFlag(dwBtnFlag));
		}
		break;
	}
}
// ===============================================================================
void CTChatFrame::SetHeightAllInnerChatList( INT nHeight )
{
	for( int i=0 ; i < TCHAT_SET_COUNT ; ++i )
	{
		if( m_vLists[ i ] && !m_vLists[ i ]->IsOuter() )
		{
			m_vLists[ i ]->SetHeight( nHeight );

			CRect rtDefault = GetDefaultListRect();

			m_vLists[ i ]->MoveComponent( CPoint(
				rtDefault.left,
				rtDefault.bottom - m_vLists[ i ]->m_rc.Height()) );
		}
	}
}
// ===============================================================================
void CTChatFrame::Update(DWORD dwTick)
{
	if( IsVisible() )
	{
		if( !IsChatEditON() )
		{
			m_dwTick += dwTick;

			if( m_dwTick > TCHAT_TIMER )
			{
				ShowComponent(FALSE);
				ResetTick();
			}
		}
		else
			NotifyImeLocalModeChange();
	}

	for( UINT i=0; i<TCHAT_SET_COUNT; ++i )
		m_vTabs[i]->Update(dwTick);

	// --------------------------------------------
	if( m_pNotifyList->IsVisible() != IsVisible())
		m_pNotifyList->ShowComponent(IsVisible());
	if( m_pUpgradeShowList->IsVisible() != IsVisible())
		m_pUpgradeShowList->ShowComponent(IsVisible());
	// --------------------------------------------
}
// ===============================================================================
void CTChatFrame::ChangeSelection(CTChatTabButton* pTab)
{
	if( m_pSelectTab == pTab || pTab->IsOuter() )
		return;

	if( m_pSelectTab )
		m_pSelectTab->SetSelect(FALSE);

	if( pTab )
		pTab->SetSelect(TRUE);
	
    m_pSelectTab = pTab;

	ResetTick();

//#ifdef DEBUG
//	_LOG(__FUNCTION__);
//#endif
}
// ===============================================================================
void CTChatFrame::ChangeSelection(TCHAT_SET set)
{
	ChangeSelection( m_vTabs[ set ] );
}
// ===============================================================================
void CTChatFrame::SwapTab(CTChatTabButton* pTab1, CTChatTabButton* pTab2)
{
	INT iSlot1 = pTab1->GetSlot();
	INT iSlot2 = pTab2->GetSlot();

	pTab1->SetSlot(iSlot2);
	pTab2->SetSlot(iSlot1);

	m_vTabs[iSlot1] = pTab2;
	m_vTabs[iSlot2] = pTab1;

//#ifdef DEBUG
//	_LOG(__FUNCTION__);
//#endif
}
// -------------------------------------------------------------------------------
void CTChatFrame::AddTabAndList(CTChatTabButton* pTab)
{
	if( pTab != m_vTabs[m_iTabCount] )
		SwapTab(pTab, m_vTabs[m_iTabCount]);

	pTab->ShowComponent(TRUE);
	pTab->EnableComponent(TRUE);

	CTChatList* pList = pTab->GetChatList();
	pList->SetOuter(FALSE);

	++m_iTabCount;

//#ifdef DEBUG
//	_LOG(__FUNCTION__);
//#endif
}
// -------------------------------------------------------------------------------
void CTChatFrame::RemoveTabAndList(CTChatTabButton* pTab)
{
	--m_iTabCount;

	pTab->ShowComponent(FALSE);
	pTab->EnableComponent(FALSE);

	for( INT i=pTab->GetSlot(); i<m_iTabCount; ++i )
	{
		m_vTabs[i] = m_vTabs[i+1];
		m_vTabs[i]->SetSlot(i);
	}

	m_vTabs[m_iTabCount] = pTab;
	pTab->SetSlot(m_iTabCount);

	CTChatList* pList = pTab->GetChatList();
	pList->SetOuter(TRUE);

//#ifdef DEBUG
//	_LOG(__FUNCTION__);
//#endif
}
// ===============================================================================
BOOL CTChatFrame::SnapList(CTChatList* pList, CRect& outRect)
{
	m_bProcSnapLeft		= FALSE;
	m_bProcSnapTop		= FALSE;
	m_bProcSnapRight	= FALSE;
	m_bProcSnapBottom	= FALSE;
	
	BOOL bResult = FALSE;

	CRect rcList, rcTemp;
	pList->GetComponentRect(&rcList);

	if( m_iTabCount < TCHAT_SET_COUNT )
	{
		for(INT i=0; i<TCHAT_SET_COUNT; ++i)
		{
			CTChatList* pCur = m_vLists[i];
			if( pCur != pList && pCur->IsOuter() )
			{
				pCur->GetComponentRect(rcTemp);
				if( SnapList(bResult?outRect:rcList, rcTemp, outRect) )
					bResult = TRUE;				
			}
		}
	}

	if( SnapList(bResult?outRect:rcList, m_rc, outRect) )
		bResult = TRUE;	

	return bResult;
}
// ===============================================================================
void CTChatFrame::ResetSetting()
{
	static DWORD defFLAG[TCHAT_SET_COUNT] =
	{
		TCHAT_FLAG_ALL_CHAT,
		TCHAT_FLAG_ALL_CHAT,
		TCHAT_FLAG_WHISPER,
		TCHAT_FLAG_PARTY,
		TCHAT_FLAG_GUILD_TACTICS,
		TCHAT_FLAG_FORCE,
		TCHAT_FLAG_INFO
	};

	static DWORD defPOS_X[TCHAT_SET_COUNT] =
	{
		T_INVALID,
		T_INVALID,
		T_INVALID,
		T_INVALID,
		T_INVALID,
		T_INVALID,
		DWORD(-431)
	};

	static DWORD defPOS_Y[TCHAT_SET_COUNT] =
	{
		T_INVALID,
		T_INVALID,
		T_INVALID,
		T_INVALID,
		T_INVALID,
		T_INVALID,
		DWORD(-213)
	};

	static DWORD defBASIS[TCHAT_SET_COUNT] =
	{
		TBASISPOINT_LEFT_BOTTOM,
		TBASISPOINT_LEFT_BOTTOM,
		TBASISPOINT_LEFT_BOTTOM,
		TBASISPOINT_LEFT_BOTTOM,
		TBASISPOINT_LEFT_BOTTOM,
		TBASISPOINT_LEFT_BOTTOM,
		TBASISPOINT_RIGHT_BOTTOM
	};

	static DWORD defTAB[TCHAT_SET_COUNT] =
	{
		0,
		1,
		2,
		3,
		4,
		5,
		6
	};

	CString str;

	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_MAXLIST), TCHAT_SET_COUNT);

	for( INT i=0; i<TCHAT_SET_COUNT; ++i )
	{
		str.Format(_T(REG_SET_CHATFLAG), i);
		theApp.WriteProfileInt(_T(REG_KEY), str, defFLAG[i]);

		str.Format(_T(REG_SET_CHATPOSX), i);	
		theApp.WriteProfileInt(_T(REG_KEY), str, defPOS_X[i]);

		str.Format(_T(REG_SET_CHATPOSY), i);	
		theApp.WriteProfileInt(_T(REG_KEY), str, defPOS_Y[i]);

		str.Format(_T(REG_SET_CHATBASIS), i);
		theApp.WriteProfileInt(_T(REG_KEY), str, defBASIS[i]);

		str.Format(_T(REG_SET_CHATTAB), i);	
		theApp.WriteProfileInt(_T(REG_KEY), str, defTAB[i]);

		str.Format( _T(REG_SET_CHATHEIGHT), i);
		theApp.WriteProfileInt(_T(REG_KEY), str, m_rcDefaultList.Height() );
	}

	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_NOTCHATPOSX), (DWORD)(0));
	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_NOTCHATPOSY), (DWORD)(-213));
	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_NOTCHATBASIS), TBASISPOINT_LEFT_BOTTOM);

	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_RESET), CHATFRAME_RESET_IDENTITY);
}
// ===============================================================================
void CTChatFrame::LoadSetting( BOOL bReset )
{
	if( CHATFRAME_RESET_IDENTITY != theApp.GetProfileInt(_T(REG_KEY), _T(REG_RESET), T_INVALID) || bReset )
		ResetSetting();

	UINT nCount = theApp.GetProfileInt(_T(REG_KEY), _T(REG_SET_MAXLIST), T_INVALID);
	if( nCount != TCHAT_SET_COUNT )
		return;

	UINT vTAB[TCHAT_SET_COUNT];

	CString str;
	for( INT i=0; i<TCHAT_SET_COUNT; ++i )
	{
		str.Format(_T(REG_SET_CHATTAB), i);	
		vTAB[i] = theApp.GetProfileInt(_T(REG_KEY), str, T_INVALID);

		if( vTAB[i] == T_INVALID )
			return;
	}

	for( INT i=0; i<TCHAT_SET_COUNT; ++i )
	{
		str.Format( _T(REG_SET_CHATHEIGHT), i );
		LONG height = theApp.GetProfileInt( _T(REG_KEY), str, m_rcDefaultList.Height() );
				
		str.Format(_T(REG_SET_CHATPOSX), i);
		LONG x = theApp.GetProfileInt(_T(REG_KEY), str, T_INVALID);
		
		if( x != T_INVALID )
		{
			str.Format(_T(REG_SET_CHATPOSY), i);
			LONG y = theApp.GetProfileInt(_T(REG_KEY), str, T_INVALID);

			if( y != T_INVALID )
			{	
				str.Format( _T(REG_SET_CHATBASIS), i);
				BYTE basis = theApp.GetProfileInt( _T(REG_KEY), str, 0xFF);

				if( basis != 0xFF )
				{
					m_vLists[i]->m_vCompOffset = CPoint(x,y);
					m_vLists[i]->m_bBasisPoint = basis;

					m_vLists[i]->SetHeight( height );
					m_vLists[i]->SetOuter(TRUE);
					m_vLists[i]->SetDragEnable(TRUE);
					m_vLists[i]->ResetPosition();
				}
			}
		}
		else
		{
			m_vLists[i]->SetHeight( height );

			CRect rtDefault = GetDefaultListRect();
			CRect rc;
			m_vLists[i]->GetComponentRect( &rc );
			m_vLists[i]->MoveComponent( CPoint(rtDefault.left, rtDefault.bottom - rc.Height()) );
		}

		str.Format(_T(REG_SET_CHATFLAG), i);
		DWORD flag = theApp.GetProfileInt(_T(REG_KEY), str, T_INVALID);

		if( flag != T_INVALID )
			m_vLists[i]->SetChatFlag(flag);
	}

	for( INT i=0; i<TCHAT_SET_COUNT; ++i )
	{
		CTChatList* pList = m_vLists[vTAB[i]];
        
		m_vTabs[i]->Init(i, pList);
		m_vTabs[i]->ShowComponent(!pList->IsOuter());

		if( m_iTabCount > i && pList->IsOuter() )
			m_iTabCount = i;
	}

	LONG x = theApp.GetProfileInt(_T(REG_KEY), _T(REG_SET_NOTCHATPOSX), T_INVALID);

	if( x != T_INVALID )
	{
		LONG y = theApp.GetProfileInt(_T(REG_KEY), _T(REG_SET_NOTCHATPOSY), T_INVALID);	

		if( y != T_INVALID )
		{
			BYTE basis = theApp.GetProfileInt( _T(REG_KEY), _T("NOTCHATBASIS"), 0xFF);

			if( basis != 0xFF )
			{
				m_pNotifyList->m_vCompOffset = CPoint(x,y);
				m_pNotifyList->m_bBasisPoint = basis;
				m_pNotifyList->ResetPosition();
			}
		}
	}

	x = theApp.GetProfileInt(_T(REG_KEY), _T(REG_SET_SHOWCHATPOSX), T_INVALID);

	if( x != T_INVALID )
	{
		LONG y = theApp.GetProfileInt(_T(REG_KEY), _T(REG_SET_SHOWCHATPOSY), T_INVALID);	

		if( y != T_INVALID )
		{
			BYTE basis = theApp.GetProfileInt( _T(REG_KEY), _T(REG_SET_SHOWCHATBASIS), 0xFF);

			if( basis != 0xFF )
			{
				m_pUpgradeShowList->m_vCompOffset = CPoint(x,y);
				m_pUpgradeShowList->m_bBasisPoint = basis;
				m_pUpgradeShowList->ResetPosition();
			}
		}
	}

//#ifdef DEBUG
//	_LOG(__FUNCTION__);
//#endif
}
// -------------------------------------------------------------------------------
void CTChatFrame::SaveSetting()
{
	CString str;
	CPoint offset;
	BYTE basis;

	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_MAXLIST), TCHAT_SET_COUNT);
	
	for( INT i=0; i<TCHAT_SET_COUNT; ++i )
	{
		if( m_vLists[i]->IsOuter() )
		{
			offset = m_vLists[i]->m_vCompOffset;
			basis = m_vLists[i]->m_bBasisPoint;
		}
		else
		{
			offset = CPoint(T_INVALID, T_INVALID);
			basis = 0xFF;
		}

		str.Format(_T(REG_SET_CHATPOSX), i);	
		theApp.WriteProfileInt(_T(REG_KEY), str, offset.x);

		str.Format(_T(REG_SET_CHATPOSY), i);	
		theApp.WriteProfileInt(_T(REG_KEY), str, offset.y);

		str.Format( _T("CHATBASIS%d"), i);
		theApp.WriteProfileInt(_T(REG_KEY), str, basis);

		str.Format(_T(REG_SET_CHATFLAG), i);
		theApp.WriteProfileInt(_T(REG_KEY), str, m_vLists[i]->GetChatFlag());

		str.Format(_T(REG_SET_CHATTAB), i);	
		theApp.WriteProfileInt(_T(REG_KEY), str, m_vTabs[i]->GetChatListIndex());

		CRect rt;
		m_vLists[i]->GetComponentRect( &rt );
		str.Format(_T(REG_SET_CHATHEIGHT), i );
		theApp.WriteProfileInt(_T(REG_KEY), str, rt.Height() );
	}

	offset = m_pNotifyList->m_vCompOffset;
	basis = m_pNotifyList->m_bBasisPoint;

	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_NOTCHATPOSX), offset.x);
	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_NOTCHATPOSY), offset.y);
	theApp.WriteProfileInt(_T(REG_KEY), _T("NOTCHATBASIS"), basis);

	offset = m_pUpgradeShowList->m_vCompOffset;
	basis = m_pUpgradeShowList->m_bBasisPoint;

	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_SHOWCHATPOSX), offset.x);
	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_SHOWCHATPOSY), offset.y);
	theApp.WriteProfileInt(_T(REG_KEY), _T(REG_SET_SHOWCHATBASIS), basis);
}
// ===============================================================================
CTChatTabButton* CTChatFrame::GetHitTab(const CPoint& pt)
{
	if( HitTest(pt) )
	{
		for(INT i=0; i<m_iTabCount; ++i)
		{
			if( m_vTabs[i]->HitTest(pt) )
				return m_vTabs[i];
		}
	}

	return NULL;
}
// -------------------------------------------------------------------------------
CTChatTabButton* CTChatFrame::FindTab(CTChatList* pList)
{
	INT i, e;
	if( pList->IsOuter() )
	{
		i = m_iTabCount;
		e = TCHAT_SET_COUNT;
	}
	else
	{
		i = 0;
		e = m_iTabCount;
	}

	for(; i < e ; ++i)
	{
		if( m_vTabs[i]->GetChatList() == pList )
			return m_vTabs[i];
	}

	return NULL;
}
// -------------------------------------------------------------------------------
CTChatList* CTChatFrame::GetHitList(const CPoint& pt)
{
	for( INT i=0; i<TCHAT_SET_COUNT; ++i )
	{
		if( m_vLists[i]->TList::HitTest(pt) )
			return m_vLists[i];
	}

	return NULL;
}
// -------------------------------------------------------------------------------
CTChatList* CTChatFrame::FindList(CTMiniPopupDlg* pPopup)
{
	for( INT i=0; i<POPUP_COUNT; ++i )
	{
		if( m_vPopup[i] == pPopup )
		{
			if( POPUP_TO_LIST[i] != T_INVALID )
				return m_vLists[ POPUP_TO_LIST[i] ];
		}
	}

	return NULL;
}

CTChatList* CTChatFrame::FindList( DWORD dwChatFlag )
{
	for( INT i=0 ; i < TCHAT_SET_COUNT ; ++i )
		if(m_vLists[ i ]->GetChatFlag() == dwChatFlag)
			return m_vLists[i];

	return NULL;
}
// ===============================================================================
void CTChatFrame::ResetChatMsg(BOOL bEnable)
{
	m_pChatEditCtrl->ShowComponent(bEnable);
	m_pChatWhisperTargetEditCtrl->ShowComponent(bEnable);
	m_pChatTypeCtrl->ShowComponent(bEnable);
	m_pChatImeCtrl->ShowComponent(bEnable);
	m_pChatEditBack->ShowComponent(bEnable);
	m_pChatMode->ShowComponent(bEnable);
	
	m_pChatEditCtrl->ClearText();
	m_pChatEditCtrl->ResetTextSetting();
	
	m_pChatWhisperTargetEditCtrl->ClearText();
	m_pChatWhisperTargetEditCtrl->ResetTextSetting();

	m_bChatEditON = bEnable;

	if( bEnable )
		NotifyImeLocalModeChange();
	else
		ShowPopup(POPUP_CHATTYPE, FALSE);
}
// -------------------------------------------------------------------------------
void CTChatFrame::ShowWhisperTargetInput( BOOL bShow )
{
	m_pChatWhisperTargetEditCtrl->ClearText();
	m_pChatWhisperTargetEditCtrl->ResetTextSetting();
	m_pChatWhisperTargetEditCtrl->ShowComponent( bShow );

	if( bShow )
	{
		m_pChatTypeCtrl->m_strText.Empty();
		CTClientUIBase::SwitchFocus( m_pChatWhisperTargetEditCtrl );
	}
	else
	{
		m_pChatTypeCtrl->m_strText = (m_bChatMode == CHAT_WHISPER) ?
			m_strChatTarget : GetChatTypeString(m_bChatMode);
	}
}
// -------------------------------------------------------------------------------
void CTChatFrame::LoseFocusWhisperTargetInput( BOOL bCancel )
{
	if( !bCancel )
	{
		if( !m_pChatWhisperTargetEditCtrl->m_strText.IsEmpty() )
			SetChatMode( CHAT_WHISPER, m_pChatWhisperTargetEditCtrl->m_strText );
		else
			SetChatMode( m_bChatMode, m_strChatTarget );
	}

	ShowWhisperTargetInput(FALSE);
}
// -------------------------------------------------------------------------------
void CTChatFrame::ProcWhisperTargetInput( BOOL bCancel )
{
	if( !bCancel )
	{
		if( !m_pChatWhisperTargetEditCtrl->m_strText.IsEmpty() )
			SetChatMode( CHAT_WHISPER, m_pChatWhisperTargetEditCtrl->m_strText );
		else
			SetChatMode( m_bChatMode, m_strChatTarget );
	}

	ShowWhisperTargetInput(FALSE);
	SwitchFocus( m_pChatEditCtrl );
	CTClientGame* pGAME = CTClientGame::GetInstance();
	pGAME->EnableChat( TRUE );
}
// -------------------------------------------------------------------------------
void CTChatFrame::AddWhisperList( CString strWhisperTarget )
{
	BOOL bFindDuplicate = FALSE;
	STRINGQUEUE::iterator it, end;
	it = m_queLatestWhisper.begin();
	end = m_queLatestWhisper.end();
	for(; it != end ; ++it )
		if( (*it) == strWhisperTarget )
		{
			m_queLatestWhisper.erase( it );
			m_queLatestWhisper.push_back( strWhisperTarget ); // 가장 최근에 사용한 애는 계속 살아남을 수 있게 맨 뒤로 넣어준다.
			bFindDuplicate = TRUE;
			break;
		}

	if( FALSE == bFindDuplicate )
	{
		if( m_queLatestWhisper.size() >= 5 )
			m_queLatestWhisper.pop_front();
		m_queLatestWhisper.push_back( strWhisperTarget );
	}

	m_vPopup[ POPUP_LATEST_WHISPER ]->ClearButtons();

	it = m_queLatestWhisper.begin();
	end = m_queLatestWhisper.end();
	for(; it != end ; ++it )
	{
		m_vPopup[ POPUP_LATEST_WHISPER ]->AddButton(
			(*it),
			OnChatWhisperSelect);
	}
}
// -------------------------------------------------------------------------------
void CTChatFrame::ResetChatList()
{
	for(INT i=0; i<TCHAT_SET_COUNT; ++i)
		m_vLists[i]->RemoveAll();

	m_pNotifyList->RemoveAll();
	m_pUpgradeShowList->RemoveAll();

	while( !m_queLatestWhisper.empty() )
		m_queLatestWhisper.pop_back();
}
// ===============================================================================
void CTChatFrame::InsertInfoToChatEdit(LPTQUEST pQuest)
{
	CTTextLinker::GetInstance()
		->InsertLinkTextToEdit(m_pChatEditCtrl, pQuest);
}
// -------------------------------------------------------------------------------
void CTChatFrame::InsertInfoToChatEdit(LPTITEM pItem, CTClientItem* pClientItem)
{
	CTTextLinker::GetInstance()
		->InsertLinkTextToEdit(m_pChatEditCtrl, pItem, pClientItem);
}
// -------------------------------------------------------------------------------
void CTChatFrame::PushFrontMsgToEdit(const CString& strMsg)
{
	m_pChatEditCtrl->PushFrontString(strMsg);
}
// -------------------------------------------------------------------------------
CString CTChatFrame::ProcChatMsgInEdit()
{
	CString strMSG = m_pChatEditCtrl->m_strText;
	INT nLen = strMSG.GetLength();
	if( nLen == 0 )
		return CString("");

	if( nLen > 1 && strMSG.GetAt(0) == CHAT_CMD_MARK )
	{
		strMSG = strMSG.Right(nLen-1);
		
		INT nPos = 0;

		CString strTOK = strMSG.Tokenize(" ", nPos);
		CString strTOK_UPPER( strTOK );
		strTOK_UPPER.MakeUpper();
		
		TMAPCHATCMD::iterator itr = m_FpChatCmdMap.find(strTOK_UPPER);
		if( itr != m_FpChatCmdMap.end() )
		{
			VECTORSTRING vPARAM;
			
			strTOK = strMSG.Tokenize(" ", nPos);
			while( !strTOK.IsEmpty() )
			{
				vPARAM.push_back(strTOK);
				strTOK = strMSG.Tokenize(" ", nPos);
			}

			(*itr->second.pfProc)(itr->first, vPARAM, itr->second.dwUser);
		}
		else
		{
			static CString strTYPE;
			static CString strMSG;

			if( strTYPE.IsEmpty() )
			{
				strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
				strMSG = CTChart::LoadString( TSTR_CHATCMD_ERROR);
			}

			CTClientChar* pMainChar = m_pMainGame->GetMainChar();
			ChatSysMSG( strTYPE, pMainChar->GetName(), strMSG, 
				TCOLOR_ERROR, TCHAT_FLAG_INFO, ID_SND_ERROR, MEDIA_TSOUND);
		}

		return CString("");
	}
	else
	{
		return CTTextLinker::GetInstance()
			->MakeLinkToNetText(m_pChatEditCtrl, TRUE, TCHAT_CHAR_MAX);
	}
}
// -------------------------------------------------------------------------------
void CTChatFrame::TrimMsgInEdit()
{
	m_pChatEditCtrl->m_strText.TrimLeft();
	m_pChatEditCtrl->m_strText.TrimRight();
}
// -------------------------------------------------------------------------------
void CTChatFrame::ClearMsgInEdit()
{
	m_pChatEditCtrl->ClearText();
	m_pChatEditCtrl->ResetTextSetting();
}
// ===============================================================================
CString CTChatFrame::ChatSysMSG( const CString& strTYPE,
								 const CString& strNAME,
								 const CString& strMSG,
								 DWORD dwCOLOR,
								 DWORD dwChatFlag,
								 DWORD dwSoundID,
								 BYTE bSoundType)
{
	return ChatMSG(
		strTYPE,
		strNAME,
		CTTextLinker::GetInstance()->BuildNetString( CString(_T("")), strMSG),
		dwCOLOR,
		dwChatFlag,
		dwSoundID,
		bSoundType,
		-1);
}

CString CTChatFrame::ChatMSG( const CString& strTYPE,
							  const CString& strNAME,
							  const CString& strMSG,
							  DWORD dwCOLOR,
							  DWORD dwChatFlag,
							  DWORD dwSoundID,
							  BYTE bSoundType,
							  DWORD dwSenderID )
{
	if( (dwChatFlag&TCHAT_FLAG_WHISPER) &&
		m_pMainGame->GetMainChar()->m_strNAME != strNAME )
	{
		m_strLastWhisper = strNAME;
	}

	TTLINKLINE_ARRAY vLINES;
	TLLINESETTING_ARRAY2D vSETTINGS2D;

	BOOL bPlaySnd = FALSE;

	CString strResult;
	CString strTYPENAME;

	switch( dwChatFlag )
	{
	case TCHAT_FLAG_INFO:
	case TCHAT_FLAG_SHOW:
		{
			strTYPENAME = CTChart::Format( TSTR_FMT_CHAT_INFO_HEADER,
				strTYPE );
			strTYPENAME.Remove('\n');		
		}
		break;
	default:
		{
			strTYPENAME = CTChart::Format( TSTR_FMT_CHAT_NAME, strTYPE, strNAME);
			strTYPENAME.Remove('\n');
		}
		break;
	};
	
	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();

	INT nPos = 0;
	CString strLINE = pTextLinker->SplitTextByCrLf(strMSG, CTTextLinker::LINK_DEF_TOK, nPos);
	while( !strLINE.IsEmpty() )
	{
		CString strPMSG = pTextLinker
			->MakeNetToLinkText(this, strLINE);

		if( !strResult.IsEmpty() )
			strResult += "\n";

		CSize szITEM;
		CSize szHeader;
		switch( dwChatFlag )
		{
		case TCHAT_FLAG_INFO:
		case TCHAT_FLAG_SHOW:
			{
				szITEM = m_vLists[0]->GetItemSize(TCHATCOL_INFO);
				m_vLists[0]->GetTextExtentPoint( strTYPENAME, szHeader );

				szITEM.cx -= TCHAT_MARGINE;
				szITEM.cx -= szHeader.cx;
			}
			break;
		default:
			{
				szITEM = m_vLists[0]->GetItemSize(TCHATCOL_MSG);
				szITEM.cx -= TCHAT_MARGINE;
			}
			break;
		};

		strResult += pTextLinker->SplitTextByComp(m_vLists[0], szITEM.cx, strPMSG, vLINES);
		pTextLinker->GetTextSettings(this, vLINES, vSETTINGS2D);			

		INT nLineSize = (INT)vLINES.size();
		for( INT i=0; i<nLineSize; ++i)
		{
			const TTEXT_LINK_LINE& line =  vLINES[i];
			const TLLINESETTING_ARRAY& vLineSettings = vSETTINGS2D[i];

			if( (dwChatFlag & TCHAT_FLAG_WORLD) == TCHAT_FLAG_WORLD )
			{
				INT iRow = m_pNotifyList->AddMsgLine( 
					(i == 0)? strTYPENAME: "",
					line.strMSG,
					dwCOLOR,
					CString(""),
					-1,
					dwChatFlag );
				
				for( INT k=0; k < (INT)vLineSettings.size(); ++k )
				{
					const TLINK_LINE_SETTING& s = vLineSettings[k];
					m_pNotifyList->AddTextSetting(iRow, TCHATCOL_MSG, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
				}

				m_pNotifyList->SetCurSelItem( max(m_pNotifyList->GetItemCount()-1, 0) );
				m_pNotifyList->ShowComponent(TRUE);

				m_dwNotifyTime = TNOTIFYCHAT_TIME;
				bPlaySnd = TRUE;
			}
			else if( (dwChatFlag & TCHAT_FLAG_SHOW) == TCHAT_FLAG_SHOW )
			{
				INT iRow = m_pUpgradeShowList->AddMsgLine( 
					"",
					i == 0 ? strTYPENAME+line.strMSG : line.strMSG,
					dwCOLOR,
					strNAME,
					dwSenderID,
					dwChatFlag,
					i == 0 ? 0 : szHeader.cx );

				INT nHeaderLen = strTYPENAME.GetLength();

				for( INT k=0; k < (INT)vLineSettings.size(); ++k )
				{
					const TLINK_LINE_SETTING& s = vLineSettings[k];
					m_pUpgradeShowList->AddTextSetting(
						iRow,
						TCHATCOL_INFO,
						s.nStart + (i == 0 ? nHeaderLen : 0),
						s.nEnd + (i == 0 ? nHeaderLen : 0),
						s.dwColor,
						s.dwInfo);
				}
				
				m_pUpgradeShowList->SetCurSelItem( max(m_pUpgradeShowList->GetItemCount()-1, 0) );
				m_pUpgradeShowList->ShowComponent(TRUE);

				m_dwNotifyTime = TNOTIFYCHAT_TIME;
				bPlaySnd = TRUE;
			}
			else
			{
				for( INT j=0; j<TCHAT_SET_COUNT; ++j )
				{
					CTChatList* pList = m_vTabs[j]->GetChatList();
					if( !pList->CheckChatFlag(dwChatFlag) )
						continue;

					if( CHAT_SHOW_NEWMSG[pList->GetIndexInGroup()] && !m_vTabs[j]->IsSelect() )
						m_vTabs[j]->NotifyNewMsg();

					switch( dwChatFlag )
					{
					case TCHAT_FLAG_INFO:
						{
							INT iRow = pList->AddMsgLine( 
								"",
								i == 0 ? strTYPENAME+line.strMSG : line.strMSG,
								dwCOLOR,
								strNAME,
								dwSenderID,
								dwChatFlag,
								i == 0 ? 0 : szHeader.cx );

							INT nHeaderLen = strTYPENAME.GetLength();

							for( INT k=0; k < (INT)vLineSettings.size(); ++k )
							{
								const TLINK_LINE_SETTING& s = vLineSettings[k];
								pList->AddTextSetting(
									iRow,
									TCHATCOL_INFO,
									s.nStart + nHeaderLen,
									s.nEnd + nHeaderLen,
									s.dwColor,
									s.dwInfo);
							}
						}
						break;

					default:
						{
							INT iRow = pList->AddMsgLine( 
								(i == 0)? strTYPENAME: "",
								line.strMSG,
								dwCOLOR,
								strNAME,
								dwSenderID,
								dwChatFlag );

							for( INT k=0; k < (INT)vLineSettings.size(); ++k )
							{
								const TLINK_LINE_SETTING& s = vLineSettings[k];
								pList->AddTextSetting(iRow, TCHATCOL_MSG, s.nStart, s.nEnd, s.dwColor, s.dwInfo);
							}
						}
						break;
					};

					if( !bPlaySnd && (!IsVisible() || pList->IsVisible()) )
						bPlaySnd = TRUE;
				}
			}
		}
		
		strTYPENAME.Empty();
		vLINES.clear();
		vSETTINGS2D.clear();

		ResetTextSetting();

		strLINE = pTextLinker->SplitTextByCrLf(strMSG, CTTextLinker::LINK_DEF_TOK, nPos);
	}
	
	if( ((dwChatFlag & TCHAT_FLAG_WORLD) != TCHAT_FLAG_WORLD) ||
		((dwChatFlag & TCHAT_FLAG_SHOW) != TCHAT_FLAG_SHOW) )
		ResetTick();

	if( !m_bChatFrameVisible )
	{
		if( (dwChatFlag & TCHAT_FLAG_WORLD) == TCHAT_FLAG_WORLD )
			ShowComponent(TRUE);
		else
		{
			BOOL bFind = FALSE;
			for( INT i=0; i<TCHAT_SET_COUNT; ++i )
			{
				CTChatList* pList = m_vTabs[i]->GetChatList();
				if( (m_vTabs[i]->IsSelect() || pList->IsOuter()) &&
					pList->CheckChatFlag(dwChatFlag) )
				{
					bFind = TRUE;
					break;
				}
			}

			if( bFind )
				ShowComponent(TRUE);
		}
	}

	if( dwSoundID && bPlaySnd )
		CTachyonRes::m_MEDIA.Play( bSoundType, dwSoundID);

	return strResult;
}
// ===============================================================================
void CTChatFrame::ForceFocus()
{
	if( !GetFocus() )
		m_pParent->SetFocus(m_id);

	if( IsChatEditON() && !m_pChatEditCtrl->GetFocus() && m_pFocus != m_pChatWhisperTargetEditCtrl )
		CTClientUIBase::SwitchFocus(m_pChatEditCtrl);

	{
		TComponent* pParent = m_pUpgradeShowList->GetParent();
		pParent->RemoveKid(m_pUpgradeShowList);
		pParent->AddKid(m_pUpgradeShowList);
	}

	{
		TComponent* pParent = m_pNotifyList->GetParent();
		pParent->RemoveKid(m_pNotifyList);
		pParent->AddKid(m_pNotifyList);
	}
}
// ===============================================================================
void CTChatFrame::SetHideBackgroundOfList(BOOL bHide)
{
	for(INT i=0; i<TCHAT_SET_COUNT; ++i)
		m_vLists[i]->SetHideBackground(bHide);
}
// ===============================================================================
void CTChatFrame::SetLock(BOOL bLock)
{
	if( m_bLock == bLock )
		return;

	m_bLock = bLock;

	SetHideBackgroundOfList(bLock);
	//m_pChatLockCtrl->Select(bLock);
}
// ===============================================================================
void CTChatFrame::NotifyImeLocalModeChange()
{
	m_pChatImeCtrl->Select( CTClientApp::IsImeLocalMode() );
}
// ===============================================================================
void CTChatFrame::CheckChatMode()
{
	CString strMsg = m_pChatEditCtrl->m_strText;
	
	strMsg.TrimRight();
	strMsg.TrimLeft();

	if( strMsg.IsEmpty() || strMsg[0] != '/' )
		return;

	INT nPos = 0;
	CString strTok = strMsg.Tokenize( _T("/ "), nPos);

	for( BYTE i=0; i<CHAT_COUNT; ++i )
	{
		if( CHAT_MODE_CMD[i] == (DWORD)T_INVALID )
			continue;

		CString strKEY;
		strKEY = CTChart::LoadString( (TSTRING) CHAT_MODE_CMD[i]);
		strKEY.MakeUpper();

		CString strTOK(strTok);
		strTOK.MakeUpper();

		int pos = 0;
		CString tok = (strKEY.Tokenize( _T("/ "), pos )).Trim();
		while( !tok.IsEmpty() )
		{
			if( tok == strTOK )
			{
				if( i == CHAT_WHISPER )
				{
					CString strTarget = strMsg.Tokenize(" ", nPos);
					if( strTarget.IsEmpty() )
						return;

					SetChatMode(i, strTarget);
				}
				else
					SetChatMode(i);

				ResetChatMsg(TRUE);
				return;
			}

			tok = (strKEY.Tokenize( _T("/ "), pos )).Trim();
		}
	}
}
// -------------------------------------------------------------------------------
void CTChatFrame::SetChatMode(BYTE bChatMode, CString strTarget)
{
	if( bChatMode == CHAT_WHISPER )
	{
		m_pChatTypeCtrl->m_strText = strTarget;
		SetChatTarget(strTarget);
	}
	else if( bChatMode == CHAT_GUILD ||
		bChatMode == CHAT_TACTICS )
	{
		if( CTClientGame::GetInstance()->GetMainChar()->m_dwTacticsID )
			bChatMode = CHAT_TACTICS;

		m_pChatTypeCtrl->m_strText = GetChatTypeString(bChatMode);
		SetChatTarget("");
	}
	else
	{
		m_pChatTypeCtrl->m_strText = GetChatTypeString(bChatMode);
		SetChatTarget("");
	}

	m_bChatMode = bChatMode;

	DWORD dwColor = GetChatTypeColor(bChatMode);
	m_pChatTypeCtrl->SetTextClr(dwColor);
	m_pChatEditCtrl->SetTextClr(dwColor);
}
// -------------------------------------------------------------------------------
void CTChatFrame::SetChatTarget(const CString& strTarget, DWORD dwTargetID)
{
	m_strChatTarget = strTarget;
	m_dwChatTargetID = dwTargetID;
}
// -------------------------------------------------------------------------------
CString CTChatFrame::OnChatModeCmd(const CString& strMSG, const VECTORSTRING& vPARAM, DWORD dwUSER)
{
	CTChatFrame* pChatFrame = (CTChatFrame*)dwUSER;
	
	for( BYTE i=0; i<CHAT_COUNT; ++i )
	{
		if( CHAT_MODE_CMD[i] == (DWORD)T_INVALID )
			continue;

		CString strKEY;
		strKEY = CTChart::LoadString( (TSTRING) CHAT_MODE_CMD[i]);
		strKEY.MakeUpper();

		int pos = 0;
		CString tok = (strKEY.Tokenize( _T("/ "), pos )).Trim();
		while( !tok.IsEmpty() )
		{
			if( tok == strMSG )
			{
				if( i == CHAT_WHISPER )
				{
					CString strTarget;
					if( vPARAM.empty() )
						strTarget = pChatFrame->GetLastWhisperTarget();
					else
						strTarget = vPARAM.front();

					if( !strTarget.IsEmpty() )
						pChatFrame->SetChatMode(i, strTarget);
				}
				else
				{
					pChatFrame->SetChatMode( i );
				}

				return CString("");
			}

			tok = (strKEY.Tokenize( _T("/ "), pos )).Trim();
		}
	}

	return CString("");
}
// ===============================================================================
CString CTChatFrame::OnChatTitleCmd(const CString& strMSG, const VECTORSTRING& vPARAM, DWORD dwUSER)
{
	CTChatFrame* pChatFrame = (CTChatFrame*)dwUSER;

	const CString& strTEXT = pChatFrame->GetChatEditCtrl()->m_strText;
	
	INT nPos = 0;
	CString strKEY = strTEXT.Tokenize( _T(" "), nPos );
	strKEY.MakeUpper();

	if( strKEY.Find( strMSG) >= 0 )
	{
		CString str = strTEXT.Mid( nPos );

		CTClientGame *pGAME = CTClientGame::GetInstance();

		CString strCOMMENT;
		CString info = BuildMBCSInfo( str);

		INT nMaxBytes = 60;
		for( INT i=0 ; i < info.GetLength() ; ++i )
		{
			CHAR c = info.GetAt( i );
			if( IS_MBCS_LEAD(c) )
			{
				strCOMMENT.AppendChar( str.GetAt( i ) );
				strCOMMENT.AppendChar( str.GetAt( i+1) );
				++i;
				nMaxBytes -= 2;
			}
			else
			{
				strCOMMENT.AppendChar( str.GetAt( i ) );
				nMaxBytes -= 1;
			}

			if( nMaxBytes <= 0 )
				break;
		}

		if( CTClientWnd::CheckComment( strCOMMENT) < 0 )
			return CString("");

//		strCOMMENT = CCurse::ConvertString( strCOMMENT, FALSE );
		if( !CTStrFilter::ConvertString( strCOMMENT, false ) )
			return CString("");

		pGAME->GetMainChar()->m_strUserTitle = strCOMMENT;
		pGAME->SendCS_COMMENT_REQ( strCOMMENT );
	}

	return CString("");
}
// ===============================================================================
void CTChatFrame::AddChatCmdProc(const CString& strMSG, FP_CHATCMDPROC fp, DWORD dwUser)
{
	CString strMSG_UPPER( strMSG);
	strMSG_UPPER.MakeUpper();

	ChatCmdData data;
	data.pfProc = fp;
	data.dwUser = dwUser;

	m_FpChatCmdMap[strMSG_UPPER] = data;
}
// -------------------------------------------------------------------------------
void CTChatFrame::ClearChatCmdProc()
{
	m_FpChatCmdMap.clear();
}
// ===============================================================================



// ===============================================================================
BOOL CTChatFrame::CanWithItemUI()
{
	return TRUE;
}
// -------------------------------------------------------------------------------
void CTChatFrame::ShowComponent(BOOL bVisible)
{
	m_bChatFrameVisible = bVisible;

	CTClientUIBase::ShowComponent(TRUE);

	for(INT i=0; i<TCHAT_SET_COUNT; ++i)
	{
		if( m_vLists[i]->IsOuter() )
			m_vLists[i]->ShowComponent(bVisible);
		else
		{
			if( bVisible )
				m_vLists[i]->ShowComponent(i == m_pSelectTab->GetChatListIndex());
			else
				m_vLists[i]->ShowComponent(FALSE);
		}

		if( !m_vTabs[i]->IsOuter() )
			m_vTabs[i]->ShowComponent(TRUE);
		else
			m_vTabs[i]->ShowComponent(FALSE);
	}

	if( !m_bChatFrameVisible )
	{
		ResetChatMsg(FALSE);

		if( m_pChatTypeCtrl )
			m_pChatTypeCtrl->ShowComponent(FALSE);
	}

	HideAllPopup();
	ResetTick();

	if( !m_bChatEditON )
		ResetChatMsg(FALSE);

	// 채팅창 아래 배경에 깔리는 바는 항상 출력되야 한다.
	m_pChatTabBack->ShowComponent(TRUE);	

//#ifdef DEBUG
//	_LOG(__FUNCTION__);
//#endif
}
// -------------------------------------------------------------------------------
BOOL CTChatFrame::HitTest( CPoint pt)
{
	if( CTClientUIBase::HitTest(pt) )
		return TRUE;

	CRect rc(m_vTabRect[0].TopLeft(), m_vTabRect[m_iTabCount-1].BottomRight());
	m_vTabs[0]->ComponentToScreen(&rc);
	if( rc.PtInRect(pt) )
		return TRUE;
	
	return FALSE;
}
// -------------------------------------------------------------------------------
void CTChatFrame::OnLButtonDown(UINT nFlags, CPoint pt)
{
	if( m_pChatWhisperTargetEditCtrl->HitTest(pt) )
	{
		if( m_bChatMode != CHAT_WHISPER )
			return ;
	}

	CTClientUIBase::OnLButtonDown(nFlags,pt);

	m_pHitTab = GetHitTab(pt);
	if( m_pHitTab )
		ChangeSelection(m_pHitTab);
	
	for(INT i=0; i<POPUP_COUNT; ++i)
	{
		if( m_vPopup[i]->IsVisible() && !m_vPopup[i]->HitTest(pt) )
		{
			ShowPopup((PopupEnum)i, FALSE);
			break;
		}
	}
}
// -------------------------------------------------------------------------------
void CTChatFrame::OnLButtonUp(UINT nFlags, CPoint pt)
{
	if( m_pChatWhisperTargetEditCtrl->HitTest(pt) )
	{
		if( m_bChatMode == CHAT_WHISPER )
			ShowWhisperTargetInput(TRUE);

		CTClientGame* pGAME = CTClientGame::GetInstance();
		pGAME->OnGM_TOGGLE_WHISPER_TARGET();

		return ;
	}

	CTClientUIBase::OnLButtonUp(nFlags,pt);
	m_pHitTab = NULL;
}
// -------------------------------------------------------------------------------
void CTChatFrame::OnRButtonDown(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnRButtonUp(nFlags,pt);
}
// -------------------------------------------------------------------------------
void CTChatFrame::OnRButtonUp(UINT nFlags, CPoint pt)
{
	CTChatTabButton* pTab = GetHitTab(pt);
	if( pTab )
	{
		CTChatList* pList = pTab->GetChatList();
        if( pList )
		{
			INT nPId = GetPopupIndex(pList->GetIndexInGroup());
			if( nPId == T_INVALID )
				HideAllPopup();
			else
				ShowPopup((PopupEnum)nPId, TRUE);
		}
	}
}
// -------------------------------------------------------------------------------
void CTChatFrame::OnMouseMove(UINT nFlags, CPoint pt)
{
	CTClientUIBase::OnMouseMove(nFlags,pt);

	if( !IsVisible() )
	{
		m_pHitTab = NULL;
		return;
	}

	if( m_pHitTab && !m_pHitTab->HitTest(pt) && !m_bLock )
	{
		CTChatTabButton* pCurHitBtn = GetHitTab(pt);
		
		if( pCurHitBtn )
		{
			SwapTab(m_pHitTab, pCurHitBtn);
		}
		else if( !HitTest(pt) && m_iTabCount > 1 )
		{
			RemoveTabAndList(m_pHitTab);
			ChangeSelection( m_vTabs[0] );

			m_pHitTab->GetChatList()->StartDrag(pt);
			m_pHitTab = NULL;
		}

		ResetTick();
	}
	else 
	{
		BOOL bHit = (HitTest(pt) || GetHitList(pt));
		
		if( !bHit )
			HideAllPopup();

		if( m_bLock )
		{
			SetHideBackgroundOfList(!bHit);
			m_pHitTab = NULL;
		}
	}
}
// -------------------------------------------------------------------------------
void CTChatFrame::OnLoseFocus( TComponent* pSetFocus )
{
	CTClientUIBase::OnLoseFocus( pSetFocus );
}
// -------------------------------------------------------------------------------
void CTChatFrame::SwitchFocus(TComponent* pCandidate)
{
	if( m_pFocus == m_pChatWhisperTargetEditCtrl && pCandidate != m_pChatWhisperTargetEditCtrl )
	{
		if( !m_pChatWhisperTargetEditCtrl->m_strText.IsEmpty() )
			SetChatMode( CHAT_WHISPER, m_pChatWhisperTargetEditCtrl->m_strText );
		else
			SetChatMode( m_bChatMode, m_strChatTarget );

		m_pChatTypeCtrl->m_strText = (m_bChatMode == CHAT_WHISPER) ?
			m_strChatTarget : GetChatTypeString(m_bChatMode);

		m_pChatWhisperTargetEditCtrl->ClearText();
		m_pChatWhisperTargetEditCtrl->ResetTextSetting();
	}

	if( IsChatEditON() )
	{
		if( pCandidate == m_pChatWhisperTargetEditCtrl )
			ShowWhisperTargetInput(TRUE);
		else
			CTClientUIBase::SwitchFocus( m_pChatEditCtrl);	
	}
	else
		CTClientUIBase::SwitchFocus(pCandidate);

	for(INT i=0; i<POPUP_COUNT; ++i)
	{
		if( m_vPopup[i] && m_vPopup[i]->IsVisible() )
		{
			RemoveKid(m_vPopup[i]);
			AddKid(m_vPopup[i]);
			break;
		}
	}
}
// ===============================================================================



// ===============================================================================
UINT CTChatFrame::GetUniqueID(TComponent* pParent)
{
	UINT nResult;
	do { nResult = (UINT)::rand(); }
	while( (pParent && pParent->FindKid(nResult)) || FindKid(nResult) );
	return nResult;
}
// ===============================================================================
BOOL CTChatFrame::SnapList(const CRect& rcFrom, const CRect& rcTo, CRect& outRect)
{
	BOOL bResult = FALSE;

	if( SnapList_RightToLeft(rcFrom, rcTo, outRect) )
		bResult = TRUE;
	else if( SnapList_LeftToRight(rcFrom, rcTo, outRect) )
		bResult = TRUE;
	

	if( SnapList_UpToDown(bResult? outRect: rcFrom, rcTo, outRect) )
		bResult = TRUE;
	else if( SnapList_DownToUp(bResult? outRect: rcFrom, rcTo, outRect) )
		bResult = TRUE;

	return bResult;
}
// -------------------------------------------------------------------------------
BOOL CTChatFrame::SnapList_LeftToRight(const CRect& rcFrom, const CRect& rcTo, CRect& outRect)
{
	if( m_bProcSnapRight || m_bProcSnapLeft ||
		rcFrom.bottom < rcTo.top ||
		rcFrom.top > rcTo.bottom ||
		rcFrom.right < rcTo.left - SNAP_REACT_VALUE ||
		rcFrom.right > rcTo.left + SNAP_REACT_VALUE )
	{
		return FALSE;
	}

	INT w = rcFrom.Width();
	outRect.right = rcTo.left;
	outRect.left = rcTo.left - w;
	outRect.top = rcFrom.top;
	outRect.bottom = rcFrom.bottom;

	m_bProcSnapRight = TRUE;

	return TRUE;
}
// -------------------------------------------------------------------------------
BOOL CTChatFrame::SnapList_RightToLeft(const CRect& rcFrom, const CRect& rcTo, CRect& outRect)
{
	if( m_bProcSnapLeft || m_bProcSnapLeft ||
		rcFrom.bottom < rcTo.top ||
		rcFrom.top > rcTo.bottom ||
		rcFrom.left < rcTo.right - SNAP_REACT_VALUE ||
		rcFrom.left > rcTo.right + SNAP_REACT_VALUE )
	{
		return FALSE;
	}

	INT w = rcFrom.Width();
	outRect.left = rcTo.right;
	outRect.right = rcTo.right + w;
	outRect.top = rcFrom.top;
	outRect.bottom = rcFrom.bottom;

	m_bProcSnapLeft = TRUE;

	return TRUE;
}
// -------------------------------------------------------------------------------
BOOL CTChatFrame::SnapList_UpToDown(const CRect& rcFrom, const CRect& rcTo, CRect& outRect)
{
	if( m_bProcSnapTop || m_bProcSnapBottom ||
		rcFrom.right < rcTo.left ||
		rcFrom.left > rcTo.right ||
		rcFrom.bottom < rcTo.top - SNAP_REACT_VALUE ||
		rcFrom.bottom > rcTo.top + SNAP_REACT_VALUE )
	{
		return FALSE;
	}

	INT h = rcFrom.Height();
	outRect.bottom = rcTo.top;
	outRect.top = rcTo.top - h;
	outRect.left = rcFrom.left;
	outRect.right = rcFrom.right;

	m_bProcSnapBottom = TRUE;

	return TRUE;
}
// -------------------------------------------------------------------------------
BOOL CTChatFrame::SnapList_DownToUp(const CRect& rcFrom, const CRect& rcTo, CRect& outRect)
{
	if( m_bProcSnapTop || m_bProcSnapBottom ||
		rcFrom.right < rcTo.left ||
		rcFrom.left > rcTo.right ||
		rcFrom.top < rcTo.bottom - SNAP_REACT_VALUE ||
		rcFrom.top > rcTo.bottom + SNAP_REACT_VALUE )
	{
		return FALSE;
	}

	INT h = rcFrom.Height();
	outRect.top = rcTo.bottom;
	outRect.bottom = rcTo.bottom + h;
	outRect.left = rcFrom.left;
	outRect.right = rcFrom.right;

	m_bProcSnapTop = TRUE;

	return TRUE;
}

// ===============================================================================
#ifdef DEBUG
void CTChatFrame::_LOG(LPCSTR func)
{
	static CString str, temp;

	str  = "----------------------------------------------------------------------\n";
	str += "<" + CString(func) + ">\n";

	temp.Format("m_iTabCount : %d\n", m_iTabCount);
	str += temp;

	temp.Format("m_pSelectTab : %d\n", m_pSelectTab->GetChatListIndex());
	str += temp;

	temp.Format("m_pHitTab : %d\n", m_pHitTab? m_pHitTab->GetSlot(): T_INVALID);
	str += temp;

	temp.Format("m_dwTick : %u\n", m_dwTick);
	str += temp;

	for( INT i=0; i<TCHAT_SET_COUNT; ++i )
	{
		CTChatList* pList = m_vTabs[i]->GetChatList();

		CRect rc;
		pList->GetComponentRect(&rc);

		temp.Format("TAB#%d(SLOT:%u VIS:%u) LIST#%u(NAME:%s OUT:%u VIS:%u POS:(%d,%d) SIZE:(%d,%d))\n", 
			i, 
			m_vTabs[i]->GetSlot(), 
			m_vTabs[i]->IsVisible(),
			pList->GetIndexInGroup(), 
			pList->GetTitle(),
			pList->IsOuter(),
			pList->IsVisible(),
			rc.left, rc.top,
			rc.Width(), rc.Height() );

		str += temp;
	}
	str += "----------------------------------------------------------------------\n";

	::OutputDebugString(str);
}
#endif
// ===============================================================================

void CTChatFrame::ResetPosition()
{
	CTClientUIBase::ResetPosition();

	for( INT i=0; i<TCHAT_SET_COUNT; ++i)
		m_vLists[i]->ResetPosition();

	m_pNotifyList->ResetPosition();
	m_pUpgradeShowList->ResetPosition();

	CTMiniPopupDlg* pActListPopup = CTClientGame::GetInstance()->GetActListPopup();

	CRect rcBtn,rcPopup;
	pActListPopup->GetComponentRect(&rcPopup);

	TComponent* pComp = FindKid(ID_CTRLINST_ACTLIST);
	pComp->GetComponentRect(&rcBtn);
	pComp->ComponentToScreen(&rcBtn);

	pActListPopup->MoveComponent( CPoint(rcBtn.left, rcBtn.top-rcPopup.Height()) );
	pActListPopup->ShowComponent(FALSE);
}

void CTChatFrame::DefaultPosition( CPoint* vBASIS, BOOL bRestore )
{
	if(bRestore)
		LoadSetting(TRUE);
}

TEdit* CTChatFrame::GetCurEdit()
{
	if( !IsVisible() || !GetFocus() )
		return NULL;

	if( m_pChatEditCtrl->GetFocus() )
		return m_pChatEditCtrl;

	if( m_pChatWhisperTargetEditCtrl->GetFocus() )
		return m_pChatWhisperTargetEditCtrl;

	return NULL;
}

BOOL CTChatFrame::GetApplyCursor( LPTCURSOR_TYPE pbCursor )
{
	for( INT i=0 ; i < TCHAT_SET_COUNT ; ++i )
		if( m_vLists[ i ]->GetApplyCursor( pbCursor ) )
			return TRUE;
	return FALSE;
}

TListItem* CTChatFrame::HitTestChatList( CPoint pt )
{
	for( INT i=0; i<TCHAT_SET_COUNT; ++i)
	{
		if( m_vLists[ i ]->IsOuter() || m_pSelectTab->GetChatList() == m_vLists[ i ] )
		{
			if( m_vLists[ i ]->HitRect( pt ) )
			{
				TListItem *pItem = m_vLists[ i ]->GetHitItem( pt );
				if( pItem )
					return pItem;
			}
		}
	}

	return NULL;
}

void CTChatFrame::ToggleTacticsChatMode( DWORD dwTactics )
{
	CTMiniPopupDlg::ButtonArray::iterator it, end;
	it = m_vPopup[POPUP_CHATTYPE]->m_vButtons.begin();
	end = m_vPopup[POPUP_CHATTYPE]->m_vButtons.end();

	for(; it != end ; ++it )
	{
		if( (*it)->GetUserData() == TCHAT_POPUP_GUILD )
		{
			if( dwTactics )
				(*it)->m_strText = CTChart::LoadString( TSTR_REPRESENTATION_TACTICS );
			else
				(*it)->m_strText = CTChart::LoadString( TSTR_REPRESENTATION_GUILD );

			return ;
		}
	}

	if( dwTactics )
	{
		if( GetChatMode() == CHAT_GUILD )
			SetChatMode( CHAT_TACTICS, _T("") );
	}
	else
	{
		if( GetChatMode() == CHAT_TACTICS )
			SetChatMode( CHAT_WORLD, _T("") );
	}
}