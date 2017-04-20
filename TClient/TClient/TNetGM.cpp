#include "stdafx.h"
#include "TClient.h"
#include "TClientWnd.h"
#include "Curse.h"
#ifdef ADD_TEXTOOL
#include "TexTool.h"
#endif

int CTClientWnd::OnGM_START_4STORY()
{
	CTachyonRes::m_MEDIA.Play(
		MEDIA_TMUSIC,
		ID_BGM_INTRO);
	m_MainGame.m_dwBGM = ID_BGM_INTRO;

	return TERR_NONE;
}

int CTClientWnd::OnGM_TOGGLE_SCREEN_MODE()
{
	DWORD dwStyle = WS_POPUP|WS_VISIBLE|WS_CLIPCHILDREN; 
	CWnd *pWND = (CWnd *) &wndTopMost;
	CRect rect(
		0, 0,
		m_Device.m_option.m_dwScreenX,
		m_Device.m_option.m_dwScreenY);

	if(m_Device.m_option.m_bWindowedMode)
	{
		dwStyle |= WS_OVERLAPPED|WS_CAPTION;
		pWND = (CWnd *) &wndNoTopMost;
	}

	AdjustWindowRect(
		&rect,
		dwStyle,
		FALSE);

	rect.OffsetRect(
		-rect.left,
		-rect.top);

	SetWindowLong(
		GetSafeHwnd(),
		GWL_STYLE,
		dwStyle);
	RestoreDevice();

	SetWindowPos(
		pWND,
		0, 0,
		0, 0,
		SWP_NOMOVE|
		SWP_NOREDRAW|
		SWP_NOSIZE);
	MoveWindow(&rect);

	return TERR_NONE;
}

int CTClientWnd::OnGM_LOGIN()
{
	CloseMessageBox(TRUE);

	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_LOGIN_NEW);
	TComponent *pEditID = pFRAME->FindKid(ID_CTRLINST_ID);

	m_bAutoLogin = FALSE;
	m_dwLoginDelay = 0;

	if(!pEditID->m_strText.IsEmpty())
	{
		m_TNet.EnableComponent(FALSE);

		m_pTNet->m_bSVR = SVR_LOGIN;
		m_pTNet->m_bSvrID = 0;
		m_pTNet->m_bLogicalValid = TRUE;

		m_pTNet->Start(
			m_strIPAddr,
			m_dwPort);
	}
	else
		return TERR_LOGIN_NOID;

	return TERR_NONE;
}

int CTClientWnd::OnGM_NETCABLE_DISCONNECTED()
{
	CTClientGuildMark::MRelease();

	SaveCustomCompPos();

#ifndef TEST_MODE
	if ( CTNationOption::INSTALL_APEX )
		CHCEnd();
#endif

	OnGM_CLOSE_MSGBOX();

	m_TNet.EnableComponent(FALSE);

	OnGM_BACKTO_LOGIN();

	m_MainGame.ResetKICKOUTMAP();

#ifdef ADD_TEXTOOL
	CTexTool::GetInstance()->ReleaseData();
#endif

	return TERR_NETCABLE_DISCONNECTED;
}

int CTClientWnd::OnGM_EXIT_GAME()
{
	CTClientGuildMark::MRelease();

	SaveCustomCompPos();

#ifndef TEST_MODE
	if ( CTNationOption::INSTALL_APEX )
		CHCEnd();
#endif

	OnGM_CLOSE_MSGBOX();
	LoadingIMG();
	Render();

	m_TNet.EnableComponent(FALSE);
	m_bRelogin = TRUE;
	OnGM_BACKTO_LOGIN();

	m_MainGame.ResetKICKOUTMAP();

#ifdef ADD_TEXTOOL
	CTexTool::GetInstance()->ReleaseData();
#endif

	return TERR_NONE;
}

int CTClientWnd::OnGM_EXIT()
{
#ifndef TEST_MODE
	if ( CTNationOption::INSTALL_APEX )
		CHCEnd();	
#endif
	SaveCustomCompPos();
	PostQuitMessage(0);
	return TERR_NONE;
}

int CTClientWnd::OnGM_NEXT_CHAR()
{
	m_TNet.m_fCharROT = 0.0f;
	m_bSlotID = (m_bSlotID + 1) % m_vTCHARINFO.size();
	SelectCHAR();
	m_TNet.SetTargetSlot( m_bSlotID, 1 );
	return TERR_NONE;
}

int CTClientWnd::OnGM_PREV_CHAR()
{
	m_TNet.m_fCharROT = 0.0f;
	m_bSlotID = (m_bSlotID + m_vTCHARINFO.size() - 1) % m_vTCHARINFO.size();
	SelectCHAR();
	m_TNet.SetTargetSlot( m_bSlotID, -1 );
	return TERR_NONE;
}

int CTClientWnd::OnGM_ROTATE_CHAR_LEFT()
{
	return TERR_NONE;
}

int CTClientWnd::OnGM_ROTATE_CHAR_RIGHT()
{
	return TERR_NONE;
}

int CTClientWnd::OnGM_CLOSE_MSGBOX()
{
	CloseMessageBox();
	return TERR_NONE;
}

int CTClientWnd::OnGM_BACKTO_LOGIN()
{
	m_TNet.m_bSelServerNum = (BYTE)T_INVALID;

	m_mapPendingMapSession.clear();

	for( int i=0; i<MAX_CON; i++)
	{
		if( m_session[i].m_bSVR != SVR_NONE )
		{
			m_bIntendLogout = TRUE; // 내가 의도한 세션종료이다.
			m_session[i].OnClose(0);
		}
	}

	theApp.m_TNICEventSink.ReleaseTEVENT();

	return TERR_NONE;
}

int CTClientWnd::OnGM_CHANNEL_POPUP_CLICKED()
{
	CTMiniPopupDlg* pPopup = m_TNet.m_pChannelDLG;
	if( !pPopup->CanProcess() )
		return TERR_NONE;

	TFrame* pFrame = m_TNet.FindFrame( ID_FRAME_SERVER_NEW );
	TList* pList = (TList*) pFrame->FindKid( ID_CTRLINST_LIST );
	if( pList->GetSel() != -1 )
	{
		static DWORD dwCHANNEL[3] =
		{
			ID_CTRLINST_CHANNEL_1,
			ID_CTRLINST_CHANNEL_2,
			ID_CTRLINST_CHANNEL_3
		};

		int n = pList->GetSel() - pList->GetTop();
		n = (0 <= n && n < 3) ? n : 0;
		TButton* pCHANNEL = (TButton*) pFrame->FindKid( dwCHANNEL[ n ] );
		pCHANNEL->Select( (BOOL) FALSE );

		LPTGROUP pGroup = (LPTGROUP) pList->GetItemData( pList->GetSel(), 0 );
		if( pGroup )
		{
			if( pPopup->CanProcess() && pPopup->GetLastSelIndex() != -1 )
			{
				INT nSel = pPopup->GetLastSelIndex();
				pGroup->m_bSelectedChannelIndex = nSel - 1;
			}
		}
	}

	pPopup->ShowComponent(FALSE);
	pPopup->ClearButtons();
	return TERR_NONE;
}

int CTClientWnd::OnGM_SELECT_CHANNEL()
{
	static const BOOL bVALIDSTATUS[] = 
	{
		FALSE,		//TSTATUS_SLEEP 
		TRUE,			//TSTATUS_NORMAL
		TRUE,			//TSTATUS_BUSY,
		FALSE			//TSTATUS_FULL,
	};

	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_SERVER_NEW);
	TList *pLIST = (TList *) pFRAME->FindKid(ID_CTRLINST_LIST);
	DWORD dwItemData = pLIST->GetItemData( pLIST->GetSel(), 0);
	if( dwItemData == (DWORD)(-1) )
		return TERR_NONE;

	LPTGROUP pGROUP = (LPTGROUP) dwItemData;
	if( !pGROUP )
		return TERR_NONE;

	if( pGROUP->m_vChannel.empty() )
		return TERR_NONE;

	if( m_pTGROUP )
		delete m_pTGROUP;

	m_pTGROUP = pGROUP->Clone();

	if( m_pTGROUP->m_bSelectedChannelIndex == 0xFF ) // 채널 자동 선택
	{
		VECTORBYTE vSTATGRP[ TSTATUS_COUNT ];

		for( BYTE i=0 ; i < BYTE(m_pTGROUP->m_vChannel.size()) ; ++i )
		{
			BYTE bSTATUS = m_pTGROUP->m_vChannel[ i ]->bSTATUS;
			if( theApp.m_bForceConnect || bVALIDSTATUS[ bSTATUS ] )
				vSTATGRP[ bSTATUS ].push_back( i );
		}

		for(BYTE i=0; i<TSTATUS_COUNT; ++i)
		{
			BYTE bCNT = (BYTE) vSTATGRP[ i ].size();
			if( bCNT )
			{
				m_pTGROUP->m_bSelectedChannelIndex = vSTATGRP[ i ][ rand() % bCNT ];
				break;
			}
		}
	}
	
	if( m_pTGROUP->m_bSelectedChannelIndex != 0xFF )
	{
		LPTCHANNEL_INFO pChannel = m_pTGROUP->m_vChannel[ m_pTGROUP->m_bSelectedChannelIndex ];

		if( !theApp.m_bForceConnect && !bVALIDSTATUS[ pChannel->bSTATUS ] )
			return TERR_CHANNEL_BUSY;

		m_bChannel = pChannel->bID;

		m_TNet.m_dwLevelTime = 0;
		m_TNet.EnableComponent(FALSE);

		SendCS_CHARLIST_REQ();
	}
	else
	{
		return TERR_SERVER_BUSY;
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_CHANGE_NEWCHAR()
{
	BYTE bAni = TRUE;
	if( m_TNet.m_dwLevel == ID_FRAME_CHAR_NEW )
		bAni = FALSE;

	m_TNet.m_bWndAni = FALSE;

	if( INT(m_vTCHARINFO.size()) >= MAX_SLOT )
		return TERR_NOSLOT;

	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_NEWCHAR_NEW);
	TEdit *pEDIT = (TEdit *) pFRAME->FindKid(ID_CTRLINST_NAME);
	if( CTNationOption::RUSSIA )
		pEDIT->m_style |= TES_NOCONMODE;

	pEDIT->ClearText();

	InitUI();

	if( !m_vTCHARINFO.empty() && m_vTCHARINFO[0] )
		m_vNEWTCHAR.m_bContry = m_vTCHARINFO[0]->m_bContry;
	else
        m_vNEWTCHAR.m_bContry = TCONTRY_PEACE;

	m_vNEWTCHAR.m_strNAME.Empty();
	m_vNEWTCHAR.m_bLevel = 0;
	m_vNEWTCHAR.m_bPants = 0;
	m_vNEWTCHAR.m_bBody = 0;
	m_vNEWTCHAR.m_bHand = 0;
	m_vNEWTCHAR.m_bFoot = 0;
	m_vNEWTCHAR.m_dwID = 0;
	m_vNEWTCHAR.m_bClass = rand() % TCLASS_COUNT;

	if( CTNationOption::POLAND ||
		CTNationOption::FRANCE ||
		CTNationOption::CZECH ||
		CTNationOption::ITALY ||
		CTNationOption::SPAIN ||
		CTNationOption::UNITEDKINGDOM ||
		CTNationOption::GREECE ||
		CTNationOption::ROMANIA ||
		CTNationOption::UNITEDSTATE ||
		CTNationOption::HUNGARY ||
		CTNationOption::TURKEY ||
		CTNationOption::PORTUGAL ||
		CTNationOption::GERMANY )
		m_vNEWTCHAR.m_bRace = 0;
	else
		m_vNEWTCHAR.m_bRace = rand() % TMAINRACE_COUNT;

	m_vNEWTCHAR.m_bSex = rand() % TSEX_COUNT;
	m_vNEWTCHAR.m_bFace = rand() % TFACE_COUNT;
	m_vNEWTCHAR.m_bHair = rand() % THAIR_COUNT;

	m_TNet.SetLevel( ID_FRAME_NEWCHAR_NEW );
	m_TNet.EnableComponent( TRUE );
	pFRAME->SetFocus( ID_CTRLINST_NAME );

	SelectNEWTCHAR();
	ReFresh();

	m_TNet.m_fROT = m_TNet.GetSlotROT( m_vNEWTCHAR.m_bRace );
	m_TNet.m_fCharROT = 0.0f;

	if( bAni )
	{
		m_TNet.StartFadeOut( 1000,0 );
		m_TNet.m_bTickReset = TRUE;
	}
	return TERR_NONE;
}

int CTClientWnd::OnGM_CHANGE_SELECT_CHAR()
{
	BOOL bAni = TRUE;
	if( m_TNet.m_dwLevel == ID_FRAME_LOADING )
		bAni = FALSE;

	m_TNet.m_bWndAni = FALSE;

	m_TNet.EnableComponent(TRUE);
	m_TNet.InitMODEL( m_vTCHARINFO );
	m_TNet.SetLevel( ID_FRAME_CHAR_NEW );
	m_TNet.m_fROT = 0.0f;
	m_TNet.m_fTargetROT = 0.0f;
	m_TNet.m_bTargetROT = FALSE;
	m_bSlotID = 0;
	SelectCHAR();

	if( bAni )
	{
		m_TNet.StartFadeOut( 1000,0 );
		m_TNet.m_bTickReset = TRUE;
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_CHANGE_SELECT_SERVER()
{
	m_TNet.m_bWndAni = FALSE;

	BOOL bAni = TRUE;
	if( m_TNet.m_dwLevel == ID_FRAME_CONTRY_NEW )
		bAni = FALSE;

	InitUI();
	m_TNet.SetLevel( ID_FRAME_SERVER_NEW );
	m_TNet.EnableComponent( TRUE );
	ReFresh();
	
	if( bAni )
		m_TNet.StartFadeOut( 1000,0 );

	m_TNet.m_bTickReset = TRUE;

	return TERR_NONE;
}

int CTClientWnd::OnGM_GOTO_CONTRY()
{
	return TERR_NONE;
}

int CTClientWnd::OnGM_GOTO_NEWCHAR()
{
	if( m_TNet.m_dwLevel == ID_FRAME_CHAR_NEW )
		return OnGM_CHANGE_NEWCHAR();
	else
		m_TNet.StartFadeIn( 600, GM_CHANGE_NEWCHAR );

	return TERR_NONE;
}

int CTClientWnd::OnGM_SEL_CHAR0()
{
	m_bSlotID = 0;
	SelectCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_SEL_CHAR1()
{
	m_bSlotID = 1;
	SelectCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_SEL_CHAR2()
{
	m_bSlotID = 2;
	SelectCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_SEL_CHAR3()
{
	m_bSlotID = 3;
	SelectCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_SEL_CHAR4()
{
	m_bSlotID = 4;
	SelectCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_SEL_CHAR5()
{
	m_bSlotID = 5;
	SelectCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_RSEL_HM()
{
	m_vNEWTCHAR.m_bRace = TRACE_HUMAN;
	m_vNEWTCHAR.m_bSex = TSEX_MAN;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_RSEL_HW()
{
	m_vNEWTCHAR.m_bRace = TRACE_HUMAN;
	m_vNEWTCHAR.m_bSex = TSEX_WOMAN;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_RSEL_BM()
{
	m_vNEWTCHAR.m_bRace = TRACE_BEAST;
	m_vNEWTCHAR.m_bSex = TSEX_MAN;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_RSEL_BW()
{
	m_vNEWTCHAR.m_bRace = TRACE_BEAST;
	m_vNEWTCHAR.m_bSex = TSEX_WOMAN;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_RSEL_NM()
{
	m_vNEWTCHAR.m_bRace = TRACE_NYMPH;
	m_vNEWTCHAR.m_bSex = TSEX_MAN;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_RSEL_NW()
{
	m_vNEWTCHAR.m_bRace = TRACE_NYMPH;
	m_vNEWTCHAR.m_bSex = TSEX_WOMAN;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_CSEL_0()
{
	m_vNEWTCHAR.m_bClass = TCLASS_WARRIOR;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_CSEL_1()
{
	m_vNEWTCHAR.m_bClass = TCLASS_RANGER;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_CSEL_2()
{
	m_vNEWTCHAR.m_bClass = TCLASS_ARCHER;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_CSEL_3()
{
	m_vNEWTCHAR.m_bClass = TCLASS_WIZARD;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_CSEL_4()
{
	m_vNEWTCHAR.m_bClass = TCLASS_PRIEST;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_CSEL_5()
{
	m_vNEWTCHAR.m_bClass = TCLASS_SORCERER;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_PREV_FACE()
{
	m_vNEWTCHAR.m_bFace = (m_vNEWTCHAR.m_bFace + TFACE_COUNT - 1) % TFACE_COUNT;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_PREV_HAIR()
{
	m_vNEWTCHAR.m_bHair = (m_vNEWTCHAR.m_bHair + THAIR_COUNT - 1) % THAIR_COUNT;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_NEXT_FACE()
{
	m_vNEWTCHAR.m_bFace = (m_vNEWTCHAR.m_bFace + 1) % TFACE_COUNT;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_NEXT_HAIR()
{
	m_vNEWTCHAR.m_bHair = (m_vNEWTCHAR.m_bHair + 1) % THAIR_COUNT;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_PREV_RACE()
{
	m_vNEWTCHAR.m_bRace = (m_vNEWTCHAR.m_bRace + TMAINRACE_COUNT - 1 ) % TMAINRACE_COUNT;
	m_TNet.UpdateRACE();
	m_TNet.m_fCharROT = 0.0f;
	m_TNet.SetTargetSlot( m_vNEWTCHAR.m_bRace, -1 );
	return TERR_NONE;
}

int CTClientWnd::OnGM_NEXT_RACE()
{
	m_vNEWTCHAR.m_bRace = (m_vNEWTCHAR.m_bRace + 1 ) % TMAINRACE_COUNT;
	m_TNet.UpdateRACE();
	m_TNet.m_fCharROT = 0.0f;
	m_TNet.SetTargetSlot( m_vNEWTCHAR.m_bRace, 1 );
	return TERR_NONE;
} 

int CTClientWnd::OnGM_PREV_CLASS()
{
	m_vNEWTCHAR.m_bClass = (m_vNEWTCHAR.m_bClass + TCLASS_COUNT - 1 ) % TCLASS_COUNT;
	SelectNEWTCHAR();
	return TERR_NONE;
}

int CTClientWnd::OnGM_NEXT_CLASS()
{
	m_vNEWTCHAR.m_bClass = (m_vNEWTCHAR.m_bClass + 1 ) % TCLASS_COUNT;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_RACE_SEL_M()
{
	m_vNEWTCHAR.m_bSex = TSEX_MAN;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_RACE_SEL_W()
{
	m_vNEWTCHAR.m_bSex = TSEX_WOMAN;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_BACKTO_SERVER()
{
	if( m_TNet.m_bWndAni )
		return TERR_NONE;

	m_TNet.EnableComponent(FALSE);
	m_TNet.m_bSelServerNum = (BYTE)T_INVALID;

	SendCS_GROUPLIST_REQ();

	if( m_TNet.m_dwLevel == ID_FRAME_CONTRY_NEW )
		return OnGM_CHANGE_SELECT_SERVER();
	else
		m_TNet.StartFadeIn( 1000, GM_CHANGE_SELECT_SERVER );

	return TERR_NONE;
}

int CTClientWnd::OnGM_BACKTO_SERVER_DIRECT()
{
	m_TNet.EnableComponent(FALSE);
	m_TNet.m_bSelServerNum = (BYTE)T_INVALID;

	SendCS_GROUPLIST_REQ();

	InitUI();
	m_TNet.SetLevel( ID_FRAME_SERVER_NEW );
	TFrame* pFRAME = m_TNet.FindFrame( ID_FRAME_SERVER_NEW );
	TList *pLIST = (TList *) pFRAME->FindKid( ID_CTRLINST_LIST );
	pLIST->ApplyUserColor(TRUE);
	pLIST->RemoveAll();

	m_TNet.EnableComponent(TRUE);
	ReFresh();
	
	return TERR_NONE;
}

int CTClientWnd::OnGM_BACKTO_CHAR()
{
	if( m_TNet.m_dwLevel == ID_FRAME_NEWCHAR_NEW )
		return OnGM_CHANGE_BACKTO_CHAR();
	else
		m_TNet.StartFadeIn( 1000, GM_CHANGE_BACKTO_CHAR );
	return TERR_NONE;
}

int CTClientWnd::OnGM_CHANGE_BACKTO_CHAR()
{
	BYTE bAni = TRUE;
	if( m_TNet.m_dwLevel == ID_FRAME_NEWCHAR_NEW )
		bAni = FALSE;

	m_TNet.m_bWndAni = FALSE;

	for( int i=0 ; i < 6 ; ++i )
		m_TNet.ReleaseMODEL( i );
	
	if(!m_vTCHARINFO.empty())
	{
		if( m_bSlotID >= INT(m_vTCHARINFO.size()) )
			m_bSlotID = BYTE(INT(m_vTCHARINFO.size()) - 1);

		m_TNet.m_dwLevelTime = 0;
		m_TNet.EnableComponent(TRUE);
		m_TNet.InitMODEL( m_vTCHARINFO );
		m_TNet.SetLevel(ID_FRAME_CHAR_NEW);
		m_TNet.m_fROT = m_TNet.GetSlotROT( m_bSlotID );
		m_TNet.m_fCharROT = 0.0f;
		SelectCHAR();
		ReFresh();
	}
	else
	{
		OnGM_BACKTO_SERVER(); //OnGM_GOTO_CONTRY();
	}

	if( bAni )
	{
		m_TNet.StartFadeOut( 1000,0 );
		m_TNet.m_bTickReset = TRUE;
	}
	return TERR_NONE;
}

int CTClientWnd::OnGM_NEW_CHAR()
{
	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_NEWCHAR_NEW);
	TEdit *pNAME = (TEdit *) pFRAME->FindKid(ID_CTRLINST_NAME);
	CString strNAME = pNAME->m_strText;

	strNAME.TrimRight();
	strNAME.TrimLeft();

	if( strNAME.IsEmpty() )
		return TERR_NONAME;

	if(strNAME.GetLength() > 16 || strNAME.GetLength() < 1)
		return TERR_NAME_TOO_LONG;

	if( CheckCharID(strNAME) < 0 )
		return TERR_INVALID_NAME;

	if( CCurse::IsCurse(strNAME) )
		return TERR_INVALID_NAME;

	for( BYTE i=0; i<MAX_SLOT; i++)
	{
		BYTE bEMPTY = TRUE;

		for( BYTE j=0; j<INT(m_vTCHARINFO.size()); j++)
			if( i == m_vTCHARINFO[j]->m_bSlotID )
			{
				bEMPTY = FALSE;
				break;
			}

		if(bEMPTY)
		{
			SendCS_CREATECHAR_REQ(
				m_pTGROUP->m_bGroupID,
				strNAME, i,
				m_vNEWTCHAR.m_bClass,
				m_vNEWTCHAR.m_bRace,
				m_vNEWTCHAR.m_bContry,
				m_vNEWTCHAR.m_bSex,
				m_vNEWTCHAR.m_bHair,
				m_vNEWTCHAR.m_bFace,
				m_vNEWTCHAR.m_bBody,
				m_vNEWTCHAR.m_bPants,
				m_vNEWTCHAR.m_bHand,
				m_vNEWTCHAR.m_bFoot);

			m_TNet.EnableComponent(FALSE);
			break;
		}
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_DEL_CHARMSG()
{
	if( m_bSlotID >= INT(m_vTCHARINFO.size()) )
		return TERR_NOCHAR;

	if( CTNationOption::MODIFY_DIRECTLOGIN )
	{
		MessageBoxYesNo(
			TSTR_DIRECT_DELCHAR_MSG,
			TSTR_DELCHAR,
			TSTR_CANCEL,
			GM_DEL_CHAR,
			GM_CLOSE_MSGBOX,
			GM_CLOSE_MSGBOX,
			TRUE,
			ID_SND_INFO,
			MEDIA_TSOUND,
			TMSGBOXOPEN_NONE,
			0,
			ID_FRAME_LOBBY_2BTN_MSGBOX );
	}
	else
	{
		TFrame *pFRAME = m_MessageBox.FindFrame(ID_FRAME_DELCHAR);
		TComponent* pPWEDIT = pFRAME->FindKid(ID_CTRLINST_EDIT);
		if(pPWEDIT)
			pPWEDIT->m_menu[TNM_ENTER] = TCML_ID_NULL;
		TEdit *pEDIT = (TEdit *) pFRAME->FindKid(ID_CTRLINST_EDIT);

		pEDIT->ClearText();
		MessageBox(
			TSTR_DELCHAR_MSG,
			TSTR_PASSWORD, 0,
			TSTR_DELCHAR,
			TSTR_CANCEL,
			ID_FRAME_DELCHAR,
			GM_DEL_CHAR,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			TRUE);
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_DEL_CHAR()
{
	TFrame *pFRAME = m_MessageBox.FindFrame(ID_FRAME_DELCHAR);
	TEdit *pEDIT = (TEdit *) pFRAME->FindKid(ID_CTRLINST_EDIT);
	OnGM_CLOSE_MSGBOX();

	if( m_bSlotID >= INT(m_vTCHARINFO.size()) )
		return TERR_NOCHAR;

	m_TNet.EnableComponent(FALSE);

	CString strPW;

	if( CTNationOption::ENGLISH  || CTNationOption::RUSSIA )
		strPW = CTClientGame::GetMD5String( pEDIT->m_strText);
	else if( CTNationOption::JAPAN )
		strPW = m_strPW;
	else
		strPW = pEDIT->m_strText;

	SendCS_DELCHAR_REQ(
		m_pTGROUP->m_bGroupID,
		strPW,
		m_vTCHARINFO[m_bSlotID]->m_dwID);

	return TERR_NONE;
}

int CTClientWnd::OnGM_START_GAME()
{
	if( m_TNet.m_dwLevelTime > TLEVEL_CHANGE_MIN_DELAY )
	{
		SendCS_START_REQ(
			m_pTGROUP->m_bGroupID,
			m_bChannel,
			m_vTCHARINFO[m_bSlotID]->m_dwID);

		LoadingIMG();

		m_strLOADMSG = CTChart::LoadString(TSTR_LOADMSG_WAITFORSTART);
		Render();
		m_TNet.ResetLight();
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_CHANGE_START_GAME()
{
	return TERR_NONE;
}

int CTClientWnd::OnGM_SHOW_LOADING_TEXT()
{
	return TERR_NONE;
}

int CTClientWnd::OnGM_CONTRY_D()
{
	if( m_vTCHARINFO.empty() )
	{
		m_vNEWTCHAR.m_bContry = TCONTRY_D;
		m_vNEWTCHAR.m_bRace = rand() % TMAINRACE_COUNT;
		SelectNEWTCHAR();
		ReFresh();
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_CONTRY_C()
{
	if( m_vTCHARINFO.empty() )
	{
		m_vNEWTCHAR.m_bContry = TCONTRY_C;
		m_vNEWTCHAR.m_bRace = rand() % TMAINRACE_COUNT;
		SelectNEWTCHAR();
		ReFresh();
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_AGREEMENT()
{
	SendCS_AGREEMENT_REQ();
	OnGM_BACKTO_SERVER();
	return TERR_NONE;
}

int CTClientWnd::OnGM_OPEN_CHANNEL_LIST()
{
	m_TNet.ProcGM_OPEN_CHANNEL_LIST();
	return TERR_NONE;
}

int CTClientWnd::OnGM_CHAR_NAMING_OK()
{
	m_TNet.m_pWarning->m_strText.Empty();
	return TERR_NONE;
}

int CTClientWnd::OnGM_CHAR_NAMING_LONG()
{
	m_TNet.m_pWarning->m_strText = CTChart::LoadString( TSTR_LOBBY_NAMING_LONG );
	return TERR_NONE;
}

int CTClientWnd::OnGM_CHAR_NAMING_WRONG()
{
	m_TNet.m_pWarning->m_strText = CTChart::LoadString( TSTR_LOBBY_NAMING_WRONG );
	return TERR_NONE;
}