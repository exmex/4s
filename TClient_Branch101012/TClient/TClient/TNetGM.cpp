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

	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_LOGIN);
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

int CTClientWnd::OnGM_CLOSE_MSGBOX()
{
	CloseMessageBox();
	return TERR_NONE;
}

int CTClientWnd::OnGM_BACKTO_LOGIN()
{
//	m_TNet.m_bSelServerNum = (BYTE)T_INVALID;
	m_TNet.m_bSelChannelNum = (BYTE)T_INVALID;

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

int CTClientWnd::OnGM_SELECT_CHANNEL()
{
	static const BOOL bVALIDSTATUS[] = 
	{
		FALSE,			//TSTATUS_SLEEP 
		TRUE,			//TSTATUS_NORMAL
		TRUE,			//TSTATUS_BUSY,
		FALSE			//TSTATUS_FULL,
	};

	CTMiniPopupDlg* pPopup = m_TNet.m_pChannelDLG;
	if( m_vTCHANNELS.size() > 1 && !pPopup->CanProcess() )
		return TERR_NONE;

	INT nSel = 0;
	if( pPopup->CanProcess() && pPopup->GetLastSelIndex() != -1 )
		nSel = pPopup->GetLastSelIndex();
	
	m_TNet.m_bSelChannelNum = (BYTE)T_INVALID;

	if( nSel == 0 )
	{
		VECTORBYTE vSTATGRP[TSTATUS_COUNT];
		for(BYTE i=0; i<BYTE(m_vTCHANNELS.size()); ++i)
		{
			BYTE bSTATUS = m_vTCHANNELS[i].bSTATUS;
			if( theApp.m_bForceConnect || bVALIDSTATUS[bSTATUS] )
				vSTATGRP[bSTATUS].push_back(i);
		}

		for(i=0; i<TSTATUS_COUNT; ++i)
		{
			BYTE bCNT = (BYTE) vSTATGRP[i].size();
			if( bCNT )
			{
				m_TNet.m_bSelChannelNum = vSTATGRP[i][rand()%bCNT];
				break;
			}
		}
	}
	else
		m_TNet.m_bSelChannelNum = nSel - 1;

	if( m_TNet.m_bSelChannelNum != (BYTE)T_INVALID )
	{
		if( !theApp.m_bForceConnect && !bVALIDSTATUS[ m_vTCHANNELS[m_TNet.m_bSelChannelNum].bSTATUS ] )
		{
			if( pPopup->CanProcess() )
			{
				for(INT i=0; i<pPopup->GetButtonCount(); ++i)
					pPopup->SelectButton(i, i == 0);
			}

			return TERR_CHANNEL_BUSY;
		}

		m_bChannelNumber = m_TNet.m_bSelChannelNum;
		m_bChannel = m_vTCHANNELS[m_bChannelNumber].bID;

		OnGM_GOTO_CHAR();
	}
	else
	{
		if( pPopup->CanProcess() )
		{
			for(INT i=0; i<pPopup->GetButtonCount(); ++i)
				pPopup->SelectButton(i, i == 0);
		}

		return TERR_SERVER_BUSY;
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_GOTO_CHAR()
{
	m_TNet.m_dwLevelTime = 0;
	m_TNet.EnableComponent(FALSE);
	
	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_SERVER);
	TList *pLIST = (TList *) pFRAME->FindKid(ID_CTRLINST_LIST);
	LPTGROUP pGROUP = (LPTGROUP) pLIST->GetItemData( pLIST->GetSel(), 0);

	m_vTGROUP.m_bGroupID = pGROUP->m_bGroupID;
	m_vTGROUP.m_strNAME = pGROUP->m_strNAME;
		
	SendCS_CHARLIST_REQ();

	return TERR_NONE;
}

int CTClientWnd::OnGM_GOTO_CONTRY()
{
	//*
	if( CTNationOption::ENGLISH )
		m_vNEWTCHAR.m_bContry = TCONTRY_C;
	else
		m_vNEWTCHAR.m_bContry = BYTE(::rand()) % TCONTRY_B;
	/*/
	m_vNEWTCHAR.m_bContry = BYTE(::rand()) % TCONTRY_B;
	/**/

	m_TNet.m_bInvertCountrySel = m_vNEWTCHAR.m_bContry;
	m_TNet.SetLevel(ID_FRAME_CONTRY);

	SelectCONTRY();
	ReFresh();

	return TERR_NONE;
}

int CTClientWnd::OnGM_GOTO_NEWCHAR()
{
	if( INT(m_vTCHARINFO.size()) >= MAX_SLOT )
		return TERR_NOSLOT;

	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_NEWCHAR);
	TEdit *pEDIT = (TEdit *) pFRAME->FindKid(ID_CTRLINST_NAME);
	if( CTNationOption::RUSSIA )
		pEDIT->m_style |= TES_NOCONMODE;

	pEDIT->ClearText();

	InitUI();
	m_TNet.SetLevel(ID_FRAME_NEWCHAR);
	m_TNet.EnableComponent(TRUE);

	pFRAME->SetFocus(ID_CTRLINST_NAME);

	m_vNEWTCHAR.m_strNAME.Empty();
	m_vNEWTCHAR.m_bLevel = 0;
	m_vNEWTCHAR.m_bPants = 0;
	m_vNEWTCHAR.m_bBody = 0;
	m_vNEWTCHAR.m_bHand = 0;
	m_vNEWTCHAR.m_bFoot = 0;
	m_vNEWTCHAR.m_dwID = 0;

	if( !m_vTCHARINFO.empty() )
		m_vNEWTCHAR.m_bContry = m_vTCHARINFO.front()->m_bContry;

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
		CTNationOption::GERMANY ||
		// Arab Ver.
		CTNationOption::EGYPT )
		m_vNEWTCHAR.m_bRace = 0;
	else
		m_vNEWTCHAR.m_bRace = rand() % TMAINRACE_COUNT;

	m_vNEWTCHAR.m_bSex = rand() % TSEX_COUNT;

	m_vNEWTCHAR.m_bFace = rand() % TFACE_COUNT;
	m_vNEWTCHAR.m_bHair = rand() % THAIR_COUNT;

	SelectNEWTCHAR();
	ReFresh();

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
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_PREV_CLASS()
{
	m_vNEWTCHAR.m_bClass = (m_vNEWTCHAR.m_bClass + TCLASS_COUNT - 1 ) % TCLASS_COUNT;
	SelectNEWTCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnGM_NEXT_RACE()
{
	m_vNEWTCHAR.m_bRace = (m_vNEWTCHAR.m_bRace + 1 ) % TMAINRACE_COUNT;
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
	m_TNet.EnableComponent(FALSE);
	m_TNet.m_bSelChannelNum = (BYTE)T_INVALID;
	m_TNet.m_bSelServerNum = (BYTE)T_INVALID;
	m_TNet.m_bSelServerGropNum = (BYTE)T_INVALID;
	
	SendCS_GROUPLIST_REQ();
	
	return TERR_NONE;
}

int CTClientWnd::OnGM_BACKTO_CHAR()
{
	if(!m_vTCHARINFO.empty())
	{
		m_TNet.m_dwLevelTime = 0;

		m_TNet.SetLevel(ID_FRAME_CHAR);
		ReFresh();

		if( m_bSlotID >= INT(m_vTCHARINFO.size()) )
			m_bSlotID = BYTE(INT(m_vTCHARINFO.size()) - 1);

		SelectCHAR();
	}
	else
		OnGM_GOTO_CONTRY();

	return TERR_NONE;
}

int CTClientWnd::OnGM_NEW_CHAR()
{
	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_NEWCHAR);
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

//	if( CCurse::IsCurse(strNAME) )
	if( CTStrFilter::CheckFobiddenWords( strNAME ) )
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
				m_vTGROUP.m_bGroupID,
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
			GM_CLOSE_MSGBOX );
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
		m_vTGROUP.m_bGroupID,
		strPW,
		m_vTCHARINFO[m_bSlotID]->m_dwID);

	return TERR_NONE;
}

int CTClientWnd::OnGM_START_GAME()
{
	if( m_TNet.m_dwLevelTime > TLEVEL_CHANGE_MIN_DELAY )
	{
		SendCS_START_REQ(
			m_vTGROUP.m_bGroupID,
			m_bChannel,
			m_vTCHARINFO[m_bSlotID]->m_dwID);

		LoadingIMG();

		m_strLOADMSG = CTChart::LoadString(TSTR_LOADMSG_WAITFORSTART);
		Render();
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_CONTRY_D()
{
	if( m_vTCHARINFO.empty() )
	{
		m_vNEWTCHAR.m_bContry = TCONTRY_D;
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
		SelectNEWTCHAR();
		ReFresh();
	}

	return TERR_NONE;
}

int CTClientWnd::OnGM_AGREEMENT()
{
	SendCS_AGREEMENT_REQ();
	SendCS_GROUPLIST_REQ();
	return TERR_NONE;
}