#include "stdafx.h"
#include "TClient.h"
#include "TClientWnd.h"
#include "TMiniDump.h"

int CTClientWnd::OnCS_LOGIN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	SOCKADDR_IN vADDR;
	BYTE bResult;
	__time64_t dCurrentTime;
	INT64 dKey;

	(*pPacket)
		>> bResult
		>> m_dwUserID
		>> m_dwKickID
		>> m_dwKEY
		>> vADDR.sin_addr.S_un.S_addr
		>> vADDR.sin_port
		>> m_bCreateCardCnt
		>> m_bInPCROOM
		>> m_dwPrmPCROOM
		>> dCurrentTime
		>> dKey;

	// Get IP.
	SOCKADDR_IN _name;
	int _namelen = sizeof( SOCKADDR_IN );
	getsockname( pSession->m_sock, (SOCKADDR*)&_name, &_namelen );

	CString strIP;
	strIP.Format( _T("%d.%d.%d.%d"),
		_name.sin_addr.S_un.S_un_b.s_b1,
		_name.sin_addr.S_un.S_un_b.s_b2,
		_name.sin_addr.S_un.S_un_b.s_b3,
		_name.sin_addr.S_un.S_un_b.s_b4 );

	// Init EventSink.
	theApp.m_TNICEventSink.InitTEVENT( this, strIP, 2 );

	CTClientApp::m_dwAppTick = GetTickCount();
	CTClientApp::m_dAppDate = CTime(dCurrentTime);
	CTClientApp::m_dCurDate = CTClientApp::m_dAppDate;
	m_bResetTick = TRUE;

	theApp.m_dwUserID = m_dwUserID;
	theApp.m_dwPrmPCROOM = m_dwPrmPCROOM;

	if( bResult == LR_SUCCESS || bResult == LR_NEEDAGREEMENT )
	{
		TFrame* pFRAME = m_TNet.FindFrame(ID_FRAME_LOGIN);
		TComponent* pEditID = pFRAME->FindKid(ID_CTRLINST_ID);

#ifndef TEST_MODE
		if( CTNationOption::INSTALL_XTRAP )
		{
			XTrap_C_SetUserInfo(
				(CHAR*)(LPCSTR)pEditID->m_strText,	//UserID,
				"",									//ServerName,
				"",									//CharacterName,
				"",									//CharacterClass,
				0);									//CharacterLevel
		}

		// 게임가드.
		if( CTNationOption::RUSSIA )
		{
			theApp.m_pNpgl->Send( (CHAR*)(LPCSTR)pEditID->m_strText );
		}
#endif

		CTMiniDump::SetLogID((LPCSTR)pEditID->m_strText);

		if(!CheckModuleFile(TRUE, dKey))
			return TERR_INTERNAL;

		if( bResult == LR_NEEDAGREEMENT )
			OnAgreementPage();
		else if( m_bRelogin )
			SendCS_CHARLIST_REQ();
		else
			SendCS_GROUPLIST_REQ();
	}
	else if( bResult == LR_NEEDWORLDUNIFY )
	{
		theApp.m_bNeedWorldUnify = TRUE;
	}
	else
	{
		if( m_bRelogin && bResult == LR_DUPLICATE )
		{
			OnGM_BACKTO_LOGIN();
			return TERR_NONE;
		}

		m_TNet.EnableComponent(TRUE);
		ReFresh();

		m_pTNet->m_bSVR = SVR_NONE;
		m_pTNet->m_bSvrID = 0;
		m_pTNet->m_bLogicalValid = TRUE;
		m_pTNet->End();

		if(m_bRelogin)
		{
			m_TNet.SetLevel(ID_FRAME_LOGIN);
			m_bRelogin = FALSE;
		}

		switch(bResult)
		{
		case LR_INVALIDPASSWD	: return TERR_INVALID_PASSWD;
		case LR_VERSION			: return TERR_INVALID_VERSION;
		case LR_NOUSER			: return TERR_INVALID_USER;
		case LR_IPBLOCK			: return TERR_IPBLOCK;
		case LR_DUPLICATE		:
			if(vADDR.sin_port)
			{
				CTClientSession *pSESSION = GetIdleSession();

				m_TNet.EnableComponent(FALSE);
				pSESSION->m_bSVR = SVR_KICK;
				pSESSION->m_bSvrID = 0;
				pSESSION->m_bLogicalValid = TRUE;

				pSESSION->Start(
					inet_ntoa(vADDR.sin_addr),
					vADDR.sin_port);
			}
			else
			{
				m_bAutoLogin = TRUE;
				m_dwLoginDelay = 0;
			}

			return TERR_NONE;

		default					: return TERR_INTERNAL;
		}
	}

	return TERR_NONE;
}

bool _compare_group(LPTGROUP pGL, LPTGROUP pGR)
{
	static const BYTE bPRIORITY[] = 
	{
		0,						//TSTATUS_SLEEP 
		3,						//TSTATUS_NORMAL
		2,						//TSTATUS_BUSY,
		1						//TSTATUS_FULL,
	};

	if( pGL->m_bStatus == TSTATUS_SLEEP && pGR->m_bStatus != TSTATUS_SLEEP )
		return FALSE;
	
	if( pGR->m_bStatus == TSTATUS_SLEEP && pGL->m_bStatus != TSTATUS_SLEEP )
		return TRUE;

	if( pGL->m_bCharCnt == pGR->m_bCharCnt )
	{
		if( bPRIORITY[pGL->m_bStatus] == bPRIORITY[pGR->m_bStatus] )
		{
			if( !pGL->m_dwRandomPriority )
				pGL->m_dwRandomPriority = (DWORD)::rand()+1;

			if( !pGR->m_dwRandomPriority )
				pGR->m_dwRandomPriority = (DWORD)::rand()+1;

			return pGL->m_dwRandomPriority > pGR->m_dwRandomPriority;
		}
		else
			return bPRIORITY[pGL->m_bStatus] > bPRIORITY[pGR->m_bStatus];
	}
	else
		return pGL->m_bCharCnt > pGR->m_bCharCnt;
}

int CTClientWnd::OnCS_GROUPLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	static DWORD dwHeaderID[] = {
		ID_CTRLINST_NAME,
		ID_CTRLINST_KIND,
		ID_CTRLINST_STATUS,
		ID_CTRLINST_CHAR};

	static DWORD dwGroupStatus[] = {
		TSTR_SERVER_SLEEP,					//TSTATUS_SLEEP 
		TSTR_SERVER_LOW,						//TSTATUS_NORMAL
		TSTR_SERVER_NORMAL,					//TSTATUS_BUSY,
		TSTR_SERVER_FULL};					//TSTATUS_FULL,

	static DWORD dwGroupCLR[] = {
		D3DCOLOR_ARGB(255, 171,160,153),	//TSTATUS_SLEEP 
		TCOLOR_LIGHT,						//TSTATUS_NORMAL
		TCOLOR_MIDDLE,						//TSTATUS_BUSY,
		TCOLOR_HEAVY};						//TSTATUS_FULL,

	static DWORD dwGroupType[] = {
		TSTR_NORMAL_SERVER,
		TSTR_BATTLE_SERVER};

	static DWORD dwNormalColor = D3DCOLOR_ARGB(255,219,214,202);

	MAPTGROUP::iterator itGROUP;
	BYTE bCount;

	for( itGROUP = m_mapTGROUP.begin(); itGROUP != m_mapTGROUP.end(); itGROUP++)
		delete (*itGROUP).second;
	m_mapTGROUP.clear();

	InitUI();
	m_TNet.SetLevel(ID_FRAME_SERVER);
	m_TNet.EnableComponent(TRUE);
	ReFresh();

	TFrame *pFRAME = m_TNet.GetCurrentFrame();
	for( BYTE i=0; i<4; i++)
	{
		TComponent *pTHEADER = pFRAME->FindKid(dwHeaderID[i]);
		CD3DFont *pTFONT = pTHEADER->Get3DFont();

		pTFONT->m_dwColor = TCOLOR_SERVER_HEADER;
	}

	TList *pLIST = (TList *) pFRAME->FindKid(ID_CTRLINST_LIST);
	pLIST->ApplyUserColor(TRUE);
	pLIST->RemoveAll();

	DWORD dwCP;
	(*pPacket)
		>> bCount
		>> dwCP;

	CheckModuleFile(FALSE, dwCP);

	TGROUPARRAY vGRPS;
	vGRPS.resize(bCount);

	for( BYTE i=0; i<bCount; i++)
	{
		vGRPS[i] = new TGROUP();
		vGRPS[i]->m_bNumber = i;

		(*pPacket)
			>> vGRPS[i]->m_strNAME
			>> vGRPS[i]->m_bGroupID
			>> vGRPS[i]->m_bType
			>> vGRPS[i]->m_bStatus
			>> vGRPS[i]->m_bCharCnt;

		m_mapTGROUP.insert( MAPTGROUP::value_type( vGRPS[i]->m_bGroupID, vGRPS[i]));
	}

	std::sort(vGRPS.begin(), vGRPS.end(), _compare_group);

	for( i=0; i<bCount; i++)
	{
		int nIndex = pLIST->AddItem( _T(""), (DWORD) vGRPS[i]);

		CString strTEXT;

		pLIST->SetItemString( nIndex, 1, vGRPS[i]->m_strNAME);
		pLIST->SetUserColor( nIndex, 1, dwNormalColor);

		strTEXT = CTChart::LoadString( (TSTRING) dwGroupType[vGRPS[i]->m_bType]);
		pLIST->SetItemString( nIndex, 2, strTEXT);
		pLIST->SetUserColor( nIndex, 2, dwNormalColor);

		strTEXT = CTChart::LoadString( (TSTRING) dwGroupStatus[vGRPS[i]->m_bStatus]);
		pLIST->SetItemString( nIndex, 3, strTEXT);
		pLIST->SetUserColor( nIndex, 3, dwGroupCLR[vGRPS[i]->m_bStatus]);

		pLIST->SetItemInt( nIndex, 4, vGRPS[i]->m_bCharCnt);
		pLIST->SetUserColor( nIndex, 4, dwNormalColor);

		if( !m_vTGROUP.m_strNAME.IsEmpty() && m_vTGROUP.m_bGroupID == vGRPS[i]->m_bGroupID )
			pLIST->SetCurSelItem(nIndex - 1);
	}

	if( pLIST->GetItemCount() > 0 && pLIST->GetSel() < 0 )
	{
		pLIST->SetCurSelItem(0);

		LPTGROUP pGROUP = (LPTGROUP) pLIST->GetItemData( 0, 0);
		m_vTGROUP.m_bGroupID = pGROUP->m_bGroupID;
	}
	pFRAME->SetFocus(ID_CTRLINST_LIST);

	m_TNet.m_bSelChannelNum = (BYTE)T_INVALID;
	m_TNet.m_bSelServerNum = (BYTE)T_INVALID;
	m_TNet.m_bSelServerGropNum = (BYTE)T_INVALID;

	return TERR_NONE;
}

int CTClientWnd::OnCS_CHARLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;

	DWORD dwCP;
	(*pPacket) 
		>> dwCP
		>> bCount;

	CheckModuleFile(FALSE, dwCP);

	ReleaseTCHARINFO();

	m_TNet.SetLevel(bCount ? ID_FRAME_CHAR : ID_FRAME_NEWCHAR);
	m_TNet.EnableComponent(TRUE);
	m_bResetTick = TRUE;
	m_bRelogin = FALSE;
	ReFresh();

	for( BYTE i=0; i<bCount; i++)
	{
		LPTCHARINFO pINFO = new TCHARINFO();
		DWORD dwRegionID;
		BYTE bItemCount;

		(*pPacket)
			>> pINFO->m_dwID
			>> pINFO->m_strNAME
			>> pINFO->m_bStartAct
			>> pINFO->m_bSlotID
			>> pINFO->m_bLevel
			>> pINFO->m_bClass
			>> pINFO->m_bRace
			>> pINFO->m_bContry
			>> pINFO->m_bSex
			>> pINFO->m_bHair
			>> pINFO->m_bFace
			>> pINFO->m_bBody
			>> pINFO->m_bPants
			>> pINFO->m_bHand
			>> pINFO->m_bFoot
			>> dwRegionID
			>> pINFO->m_dwFame
			>> pINFO->m_dwFameColor
			>> bItemCount;

		pINFO->m_pTREGION = CTChart::FindTREGIONINFO( dwRegionID);

		for( BYTE j=0; j<bItemCount; j++)
		{
			CTClientItem *pTITEM = new CTClientItem();

			pTITEM->SetItemSlot(pPacket);

			WORD wItemID;
			BYTE bGrade;
			BYTE bGradeEffect;
			WORD wColor;
			BYTE bRegGuild;

			(*pPacket)
				>> wItemID
				>> bGrade
				>> bGradeEffect
				>> wColor
				>> bRegGuild;

			pTITEM->SetItemID( wItemID );
			pTITEM->SetGrade( bGrade );
			pTITEM->SetGradeEffect( bGradeEffect );
			pTITEM->SetColor( wColor );
			pTITEM->SetRegGuild( bRegGuild );

			pINFO->m_vEQUIP.m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
		}

		if( pINFO->m_bStartAct )
			EquipStartChar(pINFO);

		CTClientItem* pCOSTUME_RACE = pINFO->m_vEQUIP.FindTItem(ES_COSTUME_RACE);
		if( pCOSTUME_RACE && pCOSTUME_RACE->GetTITEM() )
			pINFO->m_bRace = (BYTE)( pCOSTUME_RACE->GetTITEM()->m_wUseValue );

		m_vTCHARINFO.push_back(pINFO);
	}

	if( m_bSlotID >= bCount )
		m_bSlotID = bCount - 1;

	if(!bCount)
		OnGM_GOTO_CONTRY();
	else
		SelectCHAR();

	return TERR_NONE;
}

int CTClientWnd::OnCS_CREATECHAR_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;

	CString strNAME;
	BYTE bSlotID;

	BYTE bResult;
	BYTE bClass;

	BYTE bContry;
	BYTE bRace;
	BYTE bSex;

	BYTE bHair;
	BYTE bFace;
	BYTE bBody;
	BYTE bPants;
	BYTE bHand;
	BYTE bFoot;

	(*pPacket)
		>> bResult
		>> dwID
		>> strNAME
		>> bSlotID
		>> bClass
		>> bRace
		>> bContry
		>> bSex
		>> bHair
		>> bFace
		>> bBody
		>> bPants
		>> bHand
		>> bFoot
		>> m_bCreateCardCnt;

	m_TNet.EnableComponent(TRUE);
	ReFresh();

	switch(bResult)
	{
	case CR_INVALIDSLOT	: return TERR_INVALID_SLOT;
	case CR_PROTECTED	: return TERR_INVALID_NAME;
	case CR_OVERCHAR	: return TERR_NAME_TOO_LONG;
	case CR_NOGROUP		: return TERR_NOGROUP;
	case CR_DUPNAME		: return TERR_DUPNAME;
	case CR_SUCCESS		:
		{
			LPTCHARINFO pINFO = new TCHARINFO();

			pINFO->m_strNAME = strNAME;
			pINFO->m_bStartAct = 1;
			pINFO->m_bSlotID = bSlotID;
			pINFO->m_dwID = dwID;

			pINFO->m_bContry = bContry;
			pINFO->m_bClass = bClass;
			pINFO->m_bRace = bRace;
			pINFO->m_bSex = bSex;

			pINFO->m_bFace = bFace;
			pINFO->m_bHair = bHair;
			pINFO->m_bBody = bBody;
			pINFO->m_bPants = bPants;
			pINFO->m_bHand = bHand;
			pINFO->m_bFoot = bFoot;
			pINFO->m_bLevel = 1;

			m_bSlotID = BYTE(m_vTCHARINFO.size());
			EquipStartChar(pINFO);
			m_vTCHARINFO.push_back(pINFO);

			OnGM_BACKTO_CHAR();
		}

		break;

	default				: return TERR_INTERNAL;
	}

	return TERR_NONE;
}

int CTClientWnd::OnCS_DELCHAR_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> dwCharID;

	m_TNet.EnableComponent(TRUE);
	ReFresh();

	switch(bResult)
	{
	case DR_INVALIDPASSWD	: return TERR_INVALID_PASSWD;
	case DR_NOGROUP			: return TERR_NOGROUP;
	case DR_GUILD			: return TERR_DELCHAR_INGUILD;
	case DR_SUCCESS			:
		{
			for( BYTE i=0; i<INT(m_vTCHARINFO.size()); i++)
				if( dwCharID == m_vTCHARINFO[i]->m_dwID )
				{
					delete m_vTCHARINFO[i];
					m_vTCHARINFO.erase(m_vTCHARINFO.begin() + i);

					break;
				}

			if(!m_vTCHARINFO.empty())
			{
				if( m_bSlotID >= INT(m_vTCHARINFO.size()) )
					m_bSlotID = BYTE(INT(m_vTCHARINFO.size()) - 1);
				SelectCHAR();
			}
			else
			{
				OnGM_GOTO_NEWCHAR();
				ReFresh();
			}
		}

		break;

	default					: return TERR_INTERNAL;
	}

	return TERR_NONE;
}

int CTClientWnd::OnCS_START_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	SOCKADDR_IN vADDR;

	BYTE bServerID;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> vADDR.sin_addr.S_un.S_addr
		>> vADDR.sin_port
		>> bServerID;

	if(bResult)
	{
		m_TNet.SetLevel(ID_FRAME_CHAR);
		ReFresh();
	}

	switch(bResult)
	{
	case SR_NOSERVER	: return TERR_NOSERVER;
	case SR_NOGROUP		: return TERR_NOGROUP;
	case SR_SUCCESS		:
		{
			m_mapPendingMapSession.clear();

			CTClientSession *pSESSION = GetIdleSession();

			pSESSION->m_bSvrID = bServerID;
			pSESSION->m_bSVR = SVR_MAP;
			pSESSION->m_bLogicalValid = FALSE;

			pSESSION->Start(
				inet_ntoa(vADDR.sin_addr),
				vADDR.sin_port);

			m_strLOADMSG = CTChart::LoadString(TSTR_LOADMSG_WAITFORCON);
			Render();

			OnClose( pSession, 0);
			pSession->End();
   		}

		break;

	default				: return TERR_INTERNAL;
	}

	return TERR_NONE;
}

int CTClientWnd::OnCS_CHANNELLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	static DWORD dwGroupStatus[] = {
		TSTR_SERVER_SLEEP,
		TSTR_SERVER_LOW,
		TSTR_SERVER_NORMAL,
		TSTR_SERVER_FULL};

	static DWORD dwGroupCLR[] = {
		D3DCOLOR_ARGB(255, 171,160,153),	//TSTATUS_SLEEP 
		TCOLOR_LIGHT,						//TSTATUS_NORMAL
		TCOLOR_MIDDLE,						//TSTATUS_BUSY,
		TCOLOR_HEAVY};						//TSTATUS_FULL,

	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_SERVER);
	TList *pLIST = (TList *) pFRAME->FindKid(ID_CTRLINST_LIST);
	LPTGROUP pGROUP = (LPTGROUP) pLIST->GetItemData( pLIST->GetSel(), 0);
	BYTE bCount;
	DWORD dwCP;

	(*pPacket)
		>> bCount
		>> dwCP;

	CheckModuleFile(FALSE, dwCP);

	m_vTCHANNELS.clear();

	CTMiniPopupDlg* pDlg = m_TNet.m_pChannelDLG;
	pDlg->ClearButtons();

	if( bCount && ( pGROUP->m_bStatus != TSTATUS_FULL ) )
	{
		BYTE i;
		
		m_vTCHANNELS.resize(bCount);
		for(i=0; i<bCount; ++i)
		{
			TCHANNEL_INFO& info = m_vTCHANNELS[i];
			info.bNUMBER = i;

			(*pPacket)
				>> info.strNAME
				>> info.bID
				>> info.bSTATUS;

			if( pGROUP->m_bStatus == TSTATUS_SLEEP )
				info.bSTATUS = TSTATUS_SLEEP;
		}

		if( bCount > 1 )
		{
			CString strNAME;
			strNAME = CTChart::LoadString( TSTR_AUTOCHANNEL);
			pDlg->AddButton(strNAME, GM_SELECT_CHANNEL);
			pDlg->SelectButton(0, TRUE);
			pDlg->SetButtonColor(0, dwGroupCLR[TSTATUS_NORMAL]);

			for(i=0; i<bCount; ++i)
			{
				const TCHANNEL_INFO& info = m_vTCHANNELS[i];

				CString strSTATUS;
				strSTATUS = CTChart::LoadString( (TSTRING) dwGroupStatus[info.bSTATUS]);
				strNAME = CTChart::Format( TSTR_FMT_CHANNELBUTTON, info.strNAME, strSTATUS);

				pDlg->AddButton(strNAME, GM_SELECT_CHANNEL);
				pDlg->SelectButton(i+1, FALSE);
				pDlg->SetButtonColor(i+1, dwGroupCLR[info.bSTATUS]);
			}
			TComponent* pTPOPUPPOS = pFRAME->FindKid(ID_CTRLINST_POS_CHANNEL);

			CPoint ptPOPUPPOS;
			pTPOPUPPOS->GetComponentPos(&ptPOPUPPOS);

			INT nCenter = (pDlg->GetButtonHeight() * pDlg->GetButtonCount() +
				pDlg->GetUpperHeight() * 2) / 2;

			ptPOPUPPOS.y -= nCenter;
			pDlg->MoveComponent(ptPOPUPPOS);
			pDlg->ShowComponent(TRUE);
		}
		else
			pDlg->ShowComponent(FALSE);
	}
	else
		pDlg->ShowComponent(FALSE);

	m_TNet.EnableComponent(TRUE);

	return TERR_NONE;
}
