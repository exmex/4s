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
	//m_bResetTick = TRUE;

	theApp.m_dwUserID = m_dwUserID;
	theApp.m_dwPrmPCROOM = m_dwPrmPCROOM;

	if( bResult == LR_SUCCESS || bResult == LR_NEEDAGREEMENT )
	{
		TFrame* pFRAME = m_TNet.FindFrame(ID_FRAME_LOGIN_NEW);
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
		{
			OnGM_BACKTO_SERVER_DIRECT();
		}
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
			m_TNet.SetLevel(ID_FRAME_LOGIN_NEW);
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
	TFrame *pFRAME = m_TNet.FindFrame(ID_FRAME_SERVER_NEW);
	TList *pLIST = (TList *) pFRAME->FindKid(ID_CTRLINST_LIST);
	pLIST->RemoveAll();
	
	m_TNet.m_bSelServerNum = (BYTE)T_INVALID;

	TGROUPARRAY::iterator itG, endG;
	itG = m_vTGroups.begin();
	endG = m_vTGroups.end();
	for(; itG != endG ; ++itG )
		delete (*itG);

	m_vTGroups.clear();

	BYTE bCount=0;
	DWORD dwCP;
	(*pPacket)
		>> bCount
		>> dwCP;

	CheckModuleFile(FALSE, dwCP);


	for( BYTE i=0; i<bCount; i++)
	{
		LPTGROUP pGroup = new TGROUP();
		pGroup->m_bNumber = i;
		pGroup->m_bSelectedChannelIndex = 0xFF;

		(*pPacket)
			>> pGroup->m_strNAME
			>> pGroup->m_bGroupID
			>> pGroup->m_bType
			>> pGroup->m_bStatus
			>> pGroup->m_bCharCnt;

		m_vTGroups.push_back( pGroup );
	}

	if( !m_vTGroups.empty() )
	{
		std::sort(
			m_vTGroups.begin(),
			m_vTGroups.end(),
			_compare_group );

		m_TNet.m_bSelServerNum = 0;

		if( m_vTGroups[ 0 ] != NULL )
			SendCS_CHANNELLIST_REQ( m_vTGroups[ 0 ]->m_bGroupID );
	}

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

	if( bCount )
	{
		if( m_TNet.m_dwLevel == ID_FRAME_LOADING )
			OnGM_CHANGE_SELECT_CHAR();
		else
			m_TNet.StartFadeIn( 600, GM_CHANGE_SELECT_CHAR );
	}
	else
	{
		m_vNEWTCHAR.m_bContry = TCONTRY_PEACE;
		OnGM_GOTO_NEWCHAR();
	}

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

			if( pINFO->m_bContry == TCONTRY_D || pINFO->m_bContry == TCONTRY_C )
				pINFO->m_bLevel = CHOICE_COUNTRY_LEVEL;
			else
				pINFO->m_bLevel = 1;

			m_TNet.ResetSlot();
			m_bSlotID = BYTE(m_vTCHARINFO.size());
			m_TNet.m_fROT = m_TNet.GetSlotROT( m_bSlotID );
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

			for( int i=0 ; i < 6 ; ++i )
				m_TNet.ReleaseMODEL( i );

			if(!m_vTCHARINFO.empty())
			{
				INT n = 1;
				if( m_bSlotID >= INT(m_vTCHARINFO.size()) )
				{
					m_bSlotID = BYTE(INT(m_vTCHARINFO.size()) - 1);
					n = -1;
				}

				m_TNet.InitMODEL( m_vTCHARINFO );
				SelectCHAR();

				m_TNet.m_fCharROT = 0.0f;
				m_TNet.SetTargetSlot( m_bSlotID, n );
			}
			else
			{
				m_vNEWTCHAR.m_bContry = TCONTRY_PEACE;
				m_TNet.ResetSlot();
				m_bSlotID = 0;
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
		m_TNet.SetLevel(ID_FRAME_CHAR_NEW);
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
	BYTE bCount;
	DWORD dwCP;
	(*pPacket)
		>> bCount
		>> dwCP;

	CheckModuleFile(FALSE, dwCP);

	if( m_TNet.m_bSelServerNum == (BYTE)T_INVALID )
		return TERR_NONE;

	if( m_vTGroups.size() <= m_TNet.m_bSelServerNum )
		return TERR_NONE;

	LPTGROUP pGROUP = m_vTGroups.at( m_TNet.m_bSelServerNum );
	if( !pGROUP )
		return TERR_NONE;

	for( BYTE i=0 ; i < bCount ; ++i )
	{
		LPTCHANNEL_INFO pChannel = new TCHANNEL_INFO;
		(*pPacket)
			>> pChannel->strNAME
			>> pChannel->bID
			>> pChannel->bSTATUS;

		if( pGROUP->m_bStatus == TSTATUS_SLEEP )
			pChannel->bSTATUS = TSTATUS_SLEEP;

		pGROUP->m_vChannel.push_back( pChannel );
	}

	m_TNet.m_bSelServerNum += 1;
	if( m_TNet.m_bSelServerNum < m_vTGroups.size() )
	{
		SendCS_CHANNELLIST_REQ( m_vTGroups.at( m_TNet.m_bSelServerNum )->m_bGroupID );
	}
	else
	{
		m_TNet.m_bSelServerNum = 0;
		m_TNet.InitGroupUI();
		m_TNet.EnableComponent(TRUE);
		ReFresh();
	}

	return TERR_NONE;
}
