#include "stdafx.h"
#include "Resource.h"
#include "TClientWnd.h"
#include "TItemUpDlg.h"
#include "TItemRefineDlg.h"
#include "TItemRepairDlg.h"
#include "TItemGambleDlg.h"
#include "TPrivateShopDlg.h"
#include "TBlockListDlg.h"
#include "TMailListFrame.h"
#include "TMailAskMoneyDlg.h"
#include "TMailRecvDlg.h"
#include "TMailSendDlg.h"
#include "TPetMakeDlg.h"
#include "TPetManageDlg.h"
#include "TMessengerBase.h"
#include "TSoulmateFrame.h"
#include "TMiniDump.h"
#include "THelpFrame.h"
#include "TNewCashShopDlg.h"
#include "TCraftQuestDlg.h"
#include "TRentSkillDlg.h"
#include "TCommunityDlg.h"
#include "TGuildMemberFrame.h"
#include "TCharPvPInfoDlg.h"
#include "TGuildPointLogFrame.h"
#include "TGuildPointRewardFrame.h"
#include "TSiegeUI.h"
#include "TSiegeEnd.h"
#include "TSiegeManager.h"
#include "TMsgGroupList.h"
#include "TItemUpCashDlg.h"
#include "TPortalShopDlgNew.h"
#include "TGuildVolunteerFrame.h"
#include "TTacticsInviteDlg.h"
#include "TItemColorDlg.h"
#include "TFameRank.h"
#include "TFameBaseDlg.h"
#include "TFameListDlg.h"
#include "TCharRankInfoDlg.h"
#include "TChatCommander.h"
#include "TWebCashShopDlg.h"
#include "TTournament.h"
#include "TTournamentEvent.h"
#include "TTournamentGage.h"
#include "TTournamentSelectTarget.h"
#include "TTournamentResult.h"
#include "TTournamentMode.h"
#include "TRpcDlg.h"
#include "TRpcRewardDlg.h"

bool _SortMATCH( const LPTMATCH& a, const LPTMATCH& b )
{
	if( a->m_bGroup != b->m_bGroup )
		return a->m_bGroup < b->m_bGroup;
    
	return a->m_dwClass < b->m_bGroup;
}

int CTClientGame::OnCS_CONNECT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	m_pSESSION = NULL;
	CTMiniDump::SetSocket(INVALID_SOCKET);

	(*pPacket)
		>> bResult;

	switch(bResult)
	{
	case CN_ALREADYEXIST	: return TERR_DUP_CONNECT;
	case CN_INVALIDVER		: return TERR_INVALID_VERSION;
	case CN_NOCHANNEL		: return TERR_NOCHANNEL;
	case CN_NOCHAR			: return TERR_NOCHAR;
	case CN_SUCCESS			:
		{
			m_pSESSION = (CTClientSession *) pSession;
			CTMiniDump::SetSocket(m_pSESSION->m_sock);

			BYTE bCnt;
			(*pPacket) >> bCnt;

			for( BYTE i=0; i<bCnt; i++)
			{
				BYTE bSvrID;
				(*pPacket) >> bSvrID;

				SendCS_CONREADY_REQ(bSvrID);
			}

			TComponent *pFRAME = m_pMainWnd->GetMainFrame();
			while( pFRAME && pFRAME->GetParent() )
				pFRAME = pFRAME->GetParent();

			if( pFRAME != this )
			{
				m_pMainWnd->SetMainFrame(this);
				OnRegionChanged();
			}

			m_pMainWnd->m_strLOADMSG.Empty();
		}

		break;

	default					: return TERR_INTERNAL;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_INVALIDCHAR_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	return TERR_INVALID_CHAR;
}

int CTClientGame::OnCS_ADDCONNECT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;

	(*pPacket)
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		SOCKADDR_IN vTarget;
		BYTE bSvrID = 0;

		ZeroMemory( &vTarget, sizeof(SOCKADDR_IN) );

		(*pPacket)
			>> vTarget.sin_addr.S_un.S_addr
			>> vTarget.sin_port
			>> bSvrID;

		if( m_pMainWnd->IsExistingMapSession( bSvrID ) )
		{
			MAPTPENDINGMAPSESSION::iterator it = m_pMainWnd->m_mapPendingMapSession.find( bSvrID);

			if( it != m_pMainWnd->m_mapPendingMapSession.end() )
			{
				(*it).second = vTarget;
			}
			else
			{
				m_pMainWnd->m_mapPendingMapSession.insert( make_pair( bSvrID, vTarget ) );
			}
		}
		else
		{
			CTClientSession *pSESSION = m_pMainWnd->GetIdleSession();

			if( pSESSION )
			{
				pSESSION->m_target = vTarget;
				pSESSION->m_bSvrID = bSvrID;
				pSESSION->m_bSVR = SVR_MAP;
				pSESSION->m_bLogicalValid = FALSE; // CS_CONNECT_ACK를 받기전까진 이 소켓은 연결은 유효해도 통신해서는 안된다.

				pSESSION->Start(
					inet_ntoa(pSESSION->m_target.sin_addr),
					pSESSION->m_target.sin_port);
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SHUTDOWN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	((CTClientSession *) pSession)->SendCS_DISCONNECT_REQ();
	return TERR_NONE;
}

int CTClientGame::OnCS_CHARINFO_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	DWORD dwRegionID;
	BYTE bCount;
	WORD wDIR;
	BYTE bOpenCashShop = FALSE;

	CTClientItem::m_mapTITEMTICK.clear();
	CTClientItem::m_mapTITEMLOCK.clear();

	pTGaugePannel->ReleaseHotkey();
	m_vMAP.ReleaseMAP();

	SiegeMgr->Release();

	m_dwDHELPTICK = 0;
	m_dwDHELP = 0;

	m_dwTGROUND = 0;
	m_dwTSELECT = 0;
	m_dwTSHADOW = 0;

	m_bTournamentBattle = FALSE;
	m_bTournamentWatching = FALSE;

	m_bDuel = FALSE;
	m_pDuelTarget = NULL;
	m_dwDuelTick = 0;
	m_dwDuelOutTick = (DWORD)T_INVALID;
	m_dwDuelWaitTick = 0;
	m_strDuelTargetName.Empty();
	m_dwDuelTargetID = 0;

	OnGM_CLOSE_POPUP();

	ReleaseNumber();
	ReleaseParty();
	ReleaseArrow();

	DoMainRunAway(FALSE);

	m_pMainChar->m_bInPCROOM = m_pMainWnd->m_bInPCROOM;
	m_pMainChar->m_bDrawName = TRUE;

	(*pPacket)
		>> m_pMainChar->m_dwID
		>> m_pMainChar->m_strNAME
		>> m_bFirstConnect;

	CTMiniDump::SetCharID((LPCSTR)m_pMainChar->m_strNAME);

	DWORD dwGuildMark;
	DWORD dwGuildColor;
	DWORD dwEXP = 0;
	
	(*pPacket)
		>> m_pMainChar->m_bClassID
		>> m_pMainChar->m_bRaceID_
		>> m_pMainChar->m_bContryID
		>> m_pMainChar->m_bSex
		>> m_pMainChar->m_bHair
		>> m_pMainChar->m_bFace
		>> m_pMainChar->m_bBody
		>> m_pMainChar->m_bPants
		>> m_pMainChar->m_bHand
		>> m_pMainChar->m_bFoot
		>> m_pMainChar->m_bHelmetHide
		>> m_pMainChar->m_bLevel
		>> m_pMainChar->m_wPartyID
		>> m_pMainChar->m_dwGuildID
		>> dwGuildMark
		>> dwGuildColor
		>> m_pMainChar->m_bGuildDuty
		>> m_pMainChar->m_bGuildPeer
		>> m_pMainChar->m_strGUILD
		>> m_pMainChar->m_dwTacticsID
		>> m_pMainChar->m_strTACTICS
		>> m_pMainChar->m_dwRune
		>> m_pMainChar->m_dwLuna
		>> m_pMainChar->m_dwCron
		>> m_pMainChar->m_dwPrevEXP
		>> m_pMainChar->m_dwNextEXP
		>> dwEXP
		>> m_pMainChar->m_dwMaxHP
		>> m_pMainChar->m_dwHP
		>> m_pMainChar->m_dwMaxMP
		>> m_pMainChar->m_dwMP
		>> m_pMainChar->m_dwChiefID
		>> m_pMainChar->m_wCommanderID
		>> dwRegionID
		>> m_vMAP.m_wMapID
		>> m_pMainChar->m_vWorld._41
		>> m_pMainChar->m_vWorld._42
		>> m_pMainChar->m_vWorld._43
		>> wDIR
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_MINE]
		>> m_bLuckyID;

		if( CTNationOption::GERMANY || CTNationOption::FRANCE || CTNationOption::POLAND || CTNationOption::CZECH || 
			CTNationOption::ITALY || CTNationOption::SPAIN || CTNationOption::UNITEDKINGDOM ||
			CTNationOption::GREECE || CTNationOption::ROMANIA || CTNationOption::EGYPT || //CTNationOption::RUSSIA || 
			CTNationOption::UNITEDSTATE || CTNationOption::HUNGARY || CTNationOption::TURKEY ||
			CTNationOption::PORTUGAL )
		{
			(*pPacket)
				>> bOpenCashShop;
		}

	(*pPacket)
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB1]
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB2]
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB3]
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB4]
		>> bCount;

	m_pMainChar->SetEXP( dwEXP );

	if( m_bLuckyID != 255)
	{
		pTGaugePannel->m_pLUCKY->m_strText = CTChart::Format( TSTR_FMT_LUCKNUMBER, m_bLuckyID);
		pTGaugePannel->m_pLUCKY->ShowComponent(TRUE);
	}
	else
		pTGaugePannel->m_pLUCKY->ShowComponent(FALSE);

	m_pMainChar->m_pTREGION = CTChart::FindTREGIONINFO( dwRegionID);

#ifdef ADD_TEXTOOL
	static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] )->LoadTexToolData();
#endif

	if( m_pMainChar->m_dwGuildID && dwGuildMark )
	{
		if( m_pMainChar->m_pGuildMark )
		{
			m_pMainChar->m_pGuildMark->SetFront(
				TGMARK_MARK(dwGuildMark),TGMCOLOR_MARK(dwGuildColor));
			
			m_pMainChar->m_pGuildMark->SetBack(
				TGMARK_BACK(dwGuildMark),TGMCOLOR_BACK1(dwGuildColor), TGMCOLOR_BACK2(dwGuildColor));
		}
		else
		{
			m_pMainChar->m_pGuildMark = new CTClientGuildMark(
				TGMARK_MARK(dwGuildMark), TGMARK_BACK(dwGuildMark),
				TGMCOLOR_MARK(dwGuildColor), TGMCOLOR_BACK1(dwGuildColor),
				TGMCOLOR_BACK2(dwGuildColor) );
#ifdef ADD_TEXTOOL
				CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
				pTT->SetBACK( TGMARK_BACK(dwGuildMark) );
				pTT->SetMARK( TGMARK_MARK(dwGuildMark) );
#endif
		}
	}
	else if( m_pMainChar->m_pGuildMark )
	{
		delete m_pMainChar->m_pGuildMark;
		m_pMainChar->m_pGuildMark = NULL;
	}

	for( BYTE i=0; i<bCount; i++)
	{
		CTClientInven *pTINVEN = new CTClientInven();
		BYTE bItemCount;
		__time64_t dEndTime;

		(*pPacket)
			>> pTINVEN->m_bInvenID
			>> pTINVEN->m_wItemID
			>> dEndTime
			>> bItemCount;
		
		pTINVEN->m_dEndTime = dEndTime;
		pTINVEN->m_pTITEM = CTChart::FindTITEMTEMP(pTINVEN->m_wItemID);

		for( BYTE j=0; j<bItemCount; j++)
		{
			CTClientItem *pTITEM = new CTClientItem();

			pTITEM->SetItemSlot(pPacket);
			pTITEM->SetItemData(pPacket);
			pTITEM->SetItemMagic(pPacket);

			if( !pTITEM->CheckValid() )
			{
				delete pTITEM;
				continue;
			}

			pTINVEN->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
		}

		m_pMainChar->m_mapTINVEN.insert( make_pair( pTINVEN->m_bInvenID, pTINVEN) );
	}

	(*pPacket)
		>> bCount;

	for( i=0; i<bCount; i++)
	{
		CTClientSkill *pTSKILL = new CTClientSkill();
		WORD wSkillID;

		(*pPacket)
			>> wSkillID
			>> pTSKILL->m_bLevel
			>> pTSKILL->m_dwTick;

		if(pTSKILL->m_dwTick)
			pTSKILL->m_bTimerON = TRUE;
		pTSKILL->m_pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);

		m_pMainChar->m_mapTSKILL.insert( MAPTSKILL::value_type( wSkillID, pTSKILL));
	}

	(*pPacket)
		>> bCount;

	for( i=0; i<bCount; i++)
	{
		CTClientMaintain *pTMAINTAIN = new CTClientMaintain();

		(*pPacket)
			>> pTMAINTAIN->m_vTSKILLDATA.m_wID
			>> pTMAINTAIN->m_vTSKILLDATA.m_bLevel
			>> pTMAINTAIN->m_dwTick
			>> pTMAINTAIN->m_dwAttackID
			>> pTMAINTAIN->m_bAttackType
			>> pTMAINTAIN->m_dwHostID
			>> pTMAINTAIN->m_bHostType
			>> pTMAINTAIN->m_vTSKILLDATA.m_bHIT
			>> pTMAINTAIN->m_vTSKILLDATA.m_wAL
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkLevel
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAglow
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkCountryID
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.x
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.y
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.z;
		pTMAINTAIN->m_pTSKILL = CTChart::FindTSKILLTEMP(pTMAINTAIN->m_vTSKILLDATA.m_wID);

		if(pTMAINTAIN->m_pTSKILL)
		{
			pTMAINTAIN->m_pTIMMER = CTClientSkill::FindTIMMER(pTMAINTAIN->m_pTSKILL);
			pTMAINTAIN->m_bLevel = pTMAINTAIN->m_vTSKILLDATA.m_bLevel;

			pTMAINTAIN->m_bUnlimitedType = pTMAINTAIN->m_pTSKILL->m_dwDuration ? FALSE : TRUE;
			pTMAINTAIN->m_dwEndTick = pTMAINTAIN->m_dwTick + m_vKEY.m_dwTick;

			m_pMainChar->InsertMaintain( pTMAINTAIN->m_vTSKILLDATA.m_wID, pTMAINTAIN);
			m_pMainChar->PlayTActionSFX(
				&pTMAINTAIN->m_vTSFX,
				pTMAINTAIN->m_pTSKILL->m_dwSFX[TSKILLSFX_LOOP]);
		}
		else
			delete pTMAINTAIN;
	}

	(*pPacket)
		>> bCount;

	for( i=0; i<bCount; i++)
	{
		BYTE bSetID;

		(*pPacket)
			>> bSetID;

		for( BYTE j=0; j<MAX_HOTKEY_POS; j++)
		{
			(*pPacket)
				>> pTGaugePannel->m_vTHOTKEY[bSetID][j].m_bType
				>> pTGaugePannel->m_vTHOTKEY[bSetID][j].m_wID;
		}
	}

	(*pPacket)
		>> bCount;

	for( i=0; i<bCount; i++)
	{
		DWORD dwTick;
		WORD wDelayGroupID;

		(*pPacket)
			>> wDelayGroupID
			>> dwTick;

		CTClientItem::SetTick(
			wDelayGroupID,
			dwTick);
	}

	if ( pTGaugePannel->m_pLUCKY2 )
		pTGaugePannel->m_pLUCKY2->m_strText = CTChart::Format( TSTR_CHANNEL, m_pMainWnd->m_bChannel );

	(*pPacket)
		>> m_pMainChar->m_dwPvPTotalPoint
		>> m_pMainChar->m_dwPvPUseablePoint;
	
	static_cast<CTCharInfoDlg*>( m_vTFRAME[ TFRAME_CHAR_INFO ] )->SetPvPPoint(
		m_pMainChar->m_dwPvPTotalPoint, m_pMainChar->m_dwPvPUseablePoint );

	m_pMainChar->InitOBJ(m_pRES->GetOBJ( CTChart::m_vTRACE[m_pMainChar->GetRaceID()][m_pMainChar->m_bSex]));
	m_pMainChar->ResetEQUIP( m_pDevice, m_pRES);
	m_pMainChar->ResetRootID(ID_PIVOT_WAIST);
	m_pMainWnd->InitCamera();

	m_pMainChar->m_fBreathHeight = m_pMainChar->GetAttrFLOAT(ID_BREATH_HEIGHT);
	m_pMainChar->m_fJumpSpeed = m_pMainChar->GetAttrFLOAT(ID_JUMP_SPEED);

	m_pMainChar->InitSIZE(
		0.8f,
		m_pMainChar->GetAttrFLOAT(ID_SIZE_Y),
		0.8f,
		0.0f, 0.0f);
	m_pMainChar->SetPosition(m_pMainChar->m_vWorld);

	D3DXMatrixScaling(
		&m_pMainChar->m_vScaleRACE,
		CTChart::m_vTRACESCALE[m_pMainChar->GetRaceID()][m_pMainChar->m_bSex],
		CTChart::m_vTRACESCALE[m_pMainChar->GetRaceID()][m_pMainChar->m_bSex],
		CTChart::m_vTRACESCALE[m_pMainChar->GetRaceID()][m_pMainChar->m_bSex]);

	m_pMainChar->m_dwANISNDFuncID = TDEFSTEP_SND;
	m_pMainChar->m_bSTEPSND = TRUE;
	m_pMainChar->m_bSelectable = TRUE;

	if( m_vTFACEIMG[TFACEIMG_PLAYER] )
		m_vTFACEIMG[TFACEIMG_PLAYER]->SetCurImage(GetFACE(
			m_pMainChar->GetRaceID(),
			m_pMainChar->m_bSex,
			m_pMainChar->m_bFace,
			m_pMainChar->m_bHair));

	m_pMainChar->DoSkillFuncAll( this );
	ResetPlayerINFO(FALSE);
	ResetRecallINFO(FALSE);
	ResetTargetINFO(FALSE);
	ResetPartyINFO(FALSE);
	ResetRSCS(FALSE);

	ResetLeaderMark();
	pTGaugePannel->ResetHotkeyUI();
	ResetInvenUI();
	ResetSkillUI();
	ResetCharInfoUI();

	m_vMainDead.ReleaseData();
	m_vMainDead.InitOBJ(m_pMainChar->m_OBJ.m_pOBJ);
	m_vMainDead.m_strNAME = m_pMainChar->m_strNAME;
	m_vMainDead.m_bRaceID_ = m_pMainChar->m_bRaceID_;
	m_vMainDead.m_bSex = m_pMainChar->m_bSex;

	m_vMainDead.m_bPants = m_pMainChar->m_bPants;
	m_vMainDead.m_bHair = m_pMainChar->m_bHair;
	m_vMainDead.m_bFace = m_pMainChar->m_bFace;
	m_vMainDead.m_bBody = m_pMainChar->m_bBody;
	m_vMainDead.m_bHand = m_pMainChar->m_bHand;
	m_vMainDead.m_bFoot = m_pMainChar->m_bFoot;
	m_vMainDead.m_bType = OT_NONE;

	m_vMainDead.InitSIZE(
		m_pMainChar->m_fSizeX,
		m_pMainChar->m_fSizeY,
		m_pMainChar->m_fSizeZ,
		m_pMainChar->m_fDepth,
		m_pMainChar->m_fSight);
	m_pCtrlHost = m_pMainChar;

	if(m_bFirstConnect)
	{
		m_pCamera->m_wCamPITCH = TTUTORIAL_CAM_PITCH;
		m_wCamDIR = TTUTORIAL_CAM_DIR;

		wDIR = TTUTORIAL_CHAR_DIR;
	}
	else
		m_wCamDIR = wDIR;

	m_pMainChar->m_nDIR = INT(wDIR) - INT(m_pMainChar->m_wDIR);
	m_pCamera->m_fPrevDIST = m_pCamera->m_fZoomMAX / 2.0f;
	m_pCamera->m_fCamDIST = m_pCamera->m_fPrevDIST;
	CalcCamera(0);

	m_vMAP.LoadMAP(
		m_pDevice,
		m_pCamera,
		m_pRES,
		m_vMAP.m_wMapID);

	if(!m_pMainChar->m_dwHP)
	{
		m_dwGHOSTTick = 0;
		m_bGHOST = TRUE;

		OnGM_GOTO_THUMB();
	}
	else
	{
		m_dwGHOSTTick = TGHOST_TICK;
		m_bGHOST = FALSE;
	}

	if(m_pMainChar->m_bFlash)
		m_dwFLASHTick = TFLASH_TICK;
	else
		m_dwFLASHTick = 0;

	m_bMoveType = TMAINMOVE_NORMAL;
	m_dwMoveGM = 0;
	m_vMAP.FixTOBJHeight(m_pMainChar);

	if(m_pMainChar->m_pTREGION)
		OnRegionChanged();

	m_bTELEPORT = FALSE;
	m_bLoopLock = FALSE;
	m_wNextSkillID = 0;
	ResetTSKY();

	m_dwActiveVolume = TBGM_VOLUME;
	m_bStopBGM = TRUE;
	m_pMainWnd->m_bResetTick = TRUE;

	m_vTGAUGE[TGAUGE_BREATH]->SetGauge( TBREATH_TICK, TBREATH_TICK, FALSE);
	m_vTGAUGE[TGAUGE_BREATH]->m_strText.Empty();
	m_vTGAUGE[TGAUGE_BREATH]->ShowComponent(FALSE);

	((CTPrivateShopDlgForSell *) m_vTFRAME[TFRAME_PRIVATE_SELL])->StartSelling(FALSE);
	m_pMainChar->m_bPrivateShop = FALSE;
	m_pMainChar->m_strPrivateShop.Empty();
	EnableGuildUI( (m_pMainChar->m_dwGuildID || m_pMainChar->m_dwTacticsID) );

	m_pDurationUI->Update();
	CTCharInfoDlg *pTCharInfoDlg = (CTCharInfoDlg *) m_vTFRAME[TFRAME_CHAR_INFO];
	pTCharInfoDlg->ResetStatColor( m_pMainChar );
	m_pMainChar->m_strUserTitle.Empty();

	// 공성관련 초기화.
   	CTSiegeUI* pSiegeUI = static_cast< CTSiegeUI* >( m_vTFRAME[TFRAME_SIEGEUI] );
	pSiegeUI->InitSiegeUI();
	pSiegeUI->ShowComponent( FALSE );
	pSiegeUI->EnableComponent( FALSE );

	CTSiegeEnd* pSiegeEnd = static_cast< CTSiegeEnd* >( m_vTFRAME[TFRAME_SIEGEEND] );
	pSiegeEnd->InitSiegeEnd();
	pSiegeEnd->ShowComponent( FALSE );
	pSiegeEnd->EnableComponent( FALSE );

	CTShopDlg* pMonShop = static_cast< CTShopDlg* >( m_vTFRAME[TFRAME_MONSHOP] );
	pMonShop->ShowComponent( FALSE );
	pMonShop->EnableComponent( FALSE );

	SiegeMgr->Release();

	m_pMainChar->ClearGodBallSFX();
	m_pMainChar->LeaveCastle();
	m_pMainChar->SetCastleID( 0 );
	m_pMainChar->SetCamp( 0 );
	m_pMainChar->SetGodBallID( 0 );

	m_pChatFrame->ToggleTacticsChatMode( m_pMainChar->m_dwTacticsID );

#ifndef TEST_MODE
	if ( CTNationOption::INSTALL_APEX )
		StartApexClient( pSession );
#endif

	(*pPacket)
		>> m_pMainChar->m_dwPvPMonthPoint;

	if( pTGaugePannel->m_pLUCKY3 )
		pTGaugePannel->m_pLUCKY3->m_strText = CTChart::Format( TSTR_HONOR, m_pMainChar->m_dwPvPMonthPoint, m_pMainChar->m_dwPvPTotalPoint );
	if( pTGaugePannel->m_pLUCKY4 )
		pTGaugePannel->m_pLUCKY4->m_strText = CTChart::Format( TSTR_MERIT, m_pMainChar->m_dwPvPUseablePoint );

#ifdef ADD_WEB_CASHSHOP
	CTWebCashShopDlg* pWebShop = (CTWebCashShopDlg*) m_vTFRAME[TFRAME_WEB_CASHSHOP];

	if ( pWebShop->IsWebShop() && CTNationOption::USE_WEB_CASHSHOP )
		pWebShop->m_bNavigate = TRUE;

	if ( bOpenCashShop )
		OnGM_TOGGLE_CASH_SHOP();

#endif

	if( m_bFirstConnect && m_vMAP.m_wMapID == TUTORIAL_MAPID )
	{
		EnableUI(TFRAME_WELCOME);

		CTGaugePannel* pGaugePannel = static_cast<CTGaugePannel*>( m_vTFRAME[TFRAME_GAUGE] );
		pGaugePannel->AttachSiegeAlarm();
	}

	CheckValidHeightInfo();

	return TERR_NONE;
}

int CTClientGame::OnCS_MONACTION_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	D3DXVECTOR3 vTGOAL;

	DWORD dwFollowID;
	DWORD dwMonID;

	BYTE bFollowType;
	BYTE bAction;

	(*pPacket)
		>> dwMonID
		>> bAction
		>> vTGOAL.x
		>> vTGOAL.y
		>> vTGOAL.z
		>> dwFollowID
		>> bFollowType;

	CTClientMonster *pMON = FindSlave(dwMonID);
	if( !pMON || !pMON->m_pMON ||
		!pMON->m_pMON->m_bApplyAI )
		return TERR_NONE;

#if defined(TEST_MODE) || defined(DEBUG)
	if( !m_bMonRoaming && (
		bAction == TA_WALK ||
		bAction == TA_RUN) )
		return TERR_NONE;
#endif

	if(pMON->m_bGoHome)
	{
		if( pMON->m_vTGOAL != vTGOAL )
			pMON->m_vTGOAL = vTGOAL;

		return TERR_NONE;
	}

	if( pMON->m_bAction != bAction )
		pMON->m_bUPDATE = TRUE;
	pMON->SetTAction(bAction);

	switch(pMON->m_bAction)
	{
	case TA_STAND	:
		{
			pMON->m_bMouseDIR = TKDIR_N;
			pMON->m_bKeyDIR = TKDIR_N;
		}

		break;

	case TA_WALK	:
	case TA_RUN		:
		if( pMON->m_vTGOAL != vTGOAL )
		{
			pMON->m_vTGOAL = vTGOAL;
			pMON->m_bPATH = TRUE;
		}

		break;

	case TA_FOLLOW	:
		{
			pMON->m_bFollowType = bFollowType;
			pMON->m_dwFollowID = dwFollowID;
			pMON->m_bTD = TD_NONE;
		}

		break;

	default			: break;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_MONHOST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwMonID;
	BYTE bSET;

	(*pPacket)
		>> dwMonID
		>> bSET;
	CTClientMonster *pMON = FindMonster(dwMonID);

	if(!pMON)
		return TERR_NONE;

	pMON->m_bMouseDIR = TKDIR_N;
	pMON->m_bKeyDIR = TKDIR_N;
	pMON->SetTAction(TA_STAND);

	pMON->m_bUPDATE = FALSE;
	pMON->m_bPATH = FALSE;

	pMON->m_bHostMainChar = bSET;

	if(!bSET)
	{
		MAPMONSTER::iterator finder = m_mapSLAVEMON.find(dwMonID);

		if( finder != m_mapSLAVEMON.end() )
			m_mapSLAVEMON.erase(finder);
	}
	else
		m_mapSLAVEMON.insert( MAPMONSTER::value_type( pMON->m_dwID, pMON));

	return TERR_NONE;
}

int CTClientGame::OnCS_ENTER_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwRegionID;
	DWORD dwCharID;

	WORD wPITCH;
	WORD wDIR;

	BYTE bNewMember;
	BYTE bCount;

	(*pPacket)
		>> dwCharID;

	if(FindPlayer(dwCharID))
		return TERR_NONE;

	CTClientChar *pCHAR = new CTClientChar();
	pCHAR->m_pHost = m_pMainWnd;
	pCHAR->ReleaseData();

	DWORD dwGuildMark;
	DWORD dwGuildColor;
	DWORD dwRideMonID;

	WORD wCastleID;
	BYTE bCamp;
	WORD wBallID;

	(*pPacket)
		>> pCHAR->m_strNAME
		>> pCHAR->m_strUserTitle
		>> pCHAR->m_dwGuildID
		>> dwGuildMark
		>> dwGuildColor
		>> pCHAR->m_strGUILD
		>> pCHAR->m_bGuildPeer
		>> pCHAR->m_dwTacticsID
		>> pCHAR->m_strTACTICS
		>> pCHAR->m_bPrivateShop
		>> pCHAR->m_strPrivateShop
		>> dwRideMonID
		>> pCHAR->m_bClassID
		>> pCHAR->m_bRaceID_
		>> pCHAR->m_bContryID
		>> pCHAR->m_bSex
		>> pCHAR->m_bHair
		>> pCHAR->m_bFace
		>> pCHAR->m_bBody
		>> pCHAR->m_bPants
		>> pCHAR->m_bHand
		>> pCHAR->m_bFoot
		>> pCHAR->m_bLevel
		>> pCHAR->m_bHelmetHide
		>> pCHAR->m_dwMaxHP
		>> pCHAR->m_dwHP
		>> pCHAR->m_dwMaxMP
		>> pCHAR->m_dwMP
		>> pCHAR->m_dwChiefID
		>> pCHAR->m_wPartyID
		>> pCHAR->m_wCommanderID
		>> pCHAR->m_vWorld._41
		>> pCHAR->m_vWorld._42
		>> pCHAR->m_vWorld._43
		>> pCHAR->m_bAction
		>> pCHAR->m_bBlock
		>> pCHAR->m_bMode
		>> wPITCH
		>> wDIR
		>> pCHAR->m_bMouseDIR
		>> pCHAR->m_bKeyDIR
		>> pCHAR->m_bColorID
		>> dwRegionID
		>> pCHAR->m_bInPCROOM
		>> pCHAR->m_bAfterMath
		>> wCastleID
		>> bCamp
		>> wBallID
		>> bCount;
	
	pCHAR->m_pTREGION = CTChart::FindTREGIONINFO( dwRegionID);

	if( pCHAR->m_dwGuildID && dwGuildMark )
	{
		pCHAR->m_pGuildMark = new CTClientGuildMark(
			TGMARK_MARK(dwGuildMark), TGMARK_BACK(dwGuildMark),
			TGMCOLOR_MARK(dwGuildColor), TGMCOLOR_BACK1(dwGuildColor),
			TGMCOLOR_BACK2(dwGuildColor) );

		
	}

	for( BYTE i=0; i<bCount; i++)
	{
		CTClientMaintain *pTMAINTAIN = new CTClientMaintain();

		(*pPacket)
			>> pTMAINTAIN->m_vTSKILLDATA.m_wID
			>> pTMAINTAIN->m_vTSKILLDATA.m_bLevel
			>> pTMAINTAIN->m_dwTick
			>> pTMAINTAIN->m_dwAttackID
			>> pTMAINTAIN->m_bAttackType
			>> pTMAINTAIN->m_dwHostID
			>> pTMAINTAIN->m_bHostType
			>> pTMAINTAIN->m_vTSKILLDATA.m_bHIT
			>> pTMAINTAIN->m_vTSKILLDATA.m_wAL
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkLevel
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAglow
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkCountryID
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.x
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.y
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.z;
		pTMAINTAIN->m_pTSKILL = CTChart::FindTSKILLTEMP(pTMAINTAIN->m_vTSKILLDATA.m_wID);

		if(pTMAINTAIN->m_pTSKILL)
		{
			pTMAINTAIN->m_pTIMMER = CTClientSkill::FindTIMMER(pTMAINTAIN->m_pTSKILL);
			pTMAINTAIN->m_bLevel = pTMAINTAIN->m_vTSKILLDATA.m_bLevel;

			pTMAINTAIN->m_bUnlimitedType = pTMAINTAIN->m_pTSKILL->m_dwDuration ? FALSE : TRUE;
			pTMAINTAIN->m_dwEndTick = pTMAINTAIN->m_dwTick + m_vKEY.m_dwTick;

			pCHAR->InsertMaintain( pTMAINTAIN->m_vTSKILLDATA.m_wID, pTMAINTAIN);
			pCHAR->PlayTActionSFX(
				&pTMAINTAIN->m_vTSFX,
				pTMAINTAIN->m_pTSKILL->m_dwSFX[TSKILLSFX_LOOP]);
		}
		else
			delete pTMAINTAIN;
	}
	CTClientInven *pTEQUIP = new CTClientInven();

	pTEQUIP->m_bInvenID = INVEN_EQUIP;
	pTEQUIP->m_wItemID = 0;
	(*pPacket) >> bCount;

	for( i=0; i<bCount; i++)
	{
		CTClientItem *pTITEM = new CTClientItem();

		pTITEM->SetItemSlot(pPacket);
		pTITEM->SetItemData(pPacket);
		pTITEM->SetItemMagic(pPacket);

		if( !pTITEM->CheckValid() )
		{
			delete pTITEM;
			continue;
		}

		pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
	}

	(*pPacket)
		>> bNewMember;

	pCHAR->m_mapTINVEN.insert( MAPTINVEN::value_type( pTEQUIP->m_bInvenID, pTEQUIP));
	pCHAR->m_nPITCH = INT(wPITCH) - INT(pCHAR->m_wPITCH);
	pCHAR->m_nDIR = INT(wDIR) - INT(pCHAR->m_wDIR);
	pCHAR->m_dwID = dwCharID;

	pCHAR->InitOBJ(m_pRES->GetOBJ( CTChart::m_vTRACE[pCHAR->GetRaceID()][pCHAR->m_bSex]));
	pCHAR->ResetEQUIP( m_pDevice, m_pRES);
	pCHAR->ResetRootID(ID_PIVOT_WAIST);
	pCHAR->SetPosition(pCHAR->m_vWorld);

	pCHAR->m_bACTLevel = CTChart::GetTACTION( pCHAR->m_bAction )->m_bLevel;
	pCHAR->m_fBreathHeight = pCHAR->GetAttrFLOAT(ID_BREATH_HEIGHT);
	pCHAR->m_fJumpSpeed = pCHAR->GetAttrFLOAT(ID_JUMP_SPEED);
	pCHAR->InitSIZE(
		pCHAR->GetAttrFLOAT(ID_SIZE_X),
		pCHAR->GetAttrFLOAT(ID_SIZE_Y),
		pCHAR->GetAttrFLOAT(ID_SIZE_Z),
		0.0f, 0.0f);

	D3DXMatrixScaling(
		&pCHAR->m_vScaleRACE,
		CTChart::m_vTRACESCALE[pCHAR->GetRaceID()][pCHAR->m_bSex],
		CTChart::m_vTRACESCALE[pCHAR->GetRaceID()][pCHAR->m_bSex],
		CTChart::m_vTRACESCALE[pCHAR->GetRaceID()][pCHAR->m_bSex]);

	pCHAR->DoSkillFuncAll( this );

	m_mapPLAYER.insert( MAPPLAYER::value_type( pCHAR->m_dwID, pCHAR));
	m_vMAP.FixTOBJHeight(pCHAR);

	CheckMonthRank( pCHAR );
	if(bNewMember)
		pCHAR->ApplyTAction( m_pDevice, m_pRES, TA_SPAWN);

	if( GetPartyID(m_pMainChar) && IsRSCSEnemy(pCHAR) )
	{
		pCHAR->CalcDIR( 0, pCHAR->m_bMouseDIR, pCHAR->m_bKeyDIR);
		pCHAR->CalcTick(
			&D3DXVECTOR3(
			pCHAR->GetPositionX(),
			pCHAR->GetPositionY(),
			pCHAR->GetPositionZ()),
			m_pDevice,
			m_pRES, 0);

		RelaySession()->SendCS_ADDCORPSENEMY_REQ(
			m_pMainChar->m_dwID,
			m_vMAP.m_wMapID,
			pCHAR);
	}

	if( dwRideMonID )
		TakeUpPet(pCHAR->m_dwID, dwRideMonID);

	pCHAR->SetCastleID( wCastleID );
	pCHAR->SetCamp( bCamp );
	pCHAR->SetGodBallID( wBallID );
	pCHAR->LeaveCastle();

	if( wCastleID != 0 )
	{
		pCHAR->EnterCastle();

		BOOL bShow = !pCHAR->m_bHide || m_pMainChar->IsAlliance( pCHAR );

		pCHAR->PlayGodBallSFX( bShow );
	}

	LPTTOURNAMENTTEAM pTeam[2]=
	{
		m_pTLeftTeam,
		m_pTRightTeam
	};

	for( INT i=0 ; i < 2 ; ++i )
	{
		if( !pTeam[i] )
			continue;

		VTTOURNAMENTPLAYER::iterator itPlayer, endPlayer;
		itPlayer = pTeam[i]->m_vMembers.begin();
		endPlayer = pTeam[i]->m_vMembers.end();

        for(; itPlayer != endPlayer; ++itPlayer)
		{
			if( (*itPlayer)->m_dwCharID == dwCharID )
			{
				pCHAR->m_bTournamentTeam = pTeam[i]->m_bSlotID;
				break;
			}
		}
	}

	if( m_bTournamentWatching )
		static_cast<CTTournamentGage*>(m_vTFRAME[TFRAME_TOURNAMENT_GAGE])->ResetTournamentINFO();

	return TERR_NONE;
}

int CTClientGame::OnCS_LEAVE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	BYTE bExitMAP;

	(*pPacket)
		>> dwCharID
		>> bExitMAP;

	MAPPLAYER::iterator finder = m_mapPLAYER.find(dwCharID);
	if( finder != m_mapPLAYER.end() )
	{
		if( GetPartyID(m_pMainChar) && IsRSCSEnemy((*finder).second) )
		{
			RelaySession()->SendCS_DELCORPSENEMY_REQ(
				m_pMainChar->m_dwID,
				(*finder).second->m_dwID,
				(*finder).second->m_bType);
		}

		CTClientChar* pTCHAR = (*finder).second;
		CTClientPet* pTPET = pTCHAR->GetRidingPet();
		if( pTPET )
			pTPET->TakeDown();

		if( m_pMainChar->InCastle() )
		{
			pTCHAR->ClearGodBallSFX();
			pTCHAR->SetCamp( 0 );
			pTCHAR->SetGodBallID( 0 );
		}

		DeleteDynimicOBJ(pTCHAR);
		m_mapPLAYER.erase(finder);

		if( m_bTournamentWatching )
			static_cast<CTTournamentGage*>(m_vTFRAME[TFRAME_TOURNAMENT_GAGE])->ResetTournamentINFO();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_BLOCK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwObjID;
	BYTE bObjType;

	WORD wPITCH;
	WORD wDIR;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	BYTE bAction;

	(*pPacket)
		>> dwObjID
		>> bObjType;

	CTClientMoveObj *pTOBJ = (CTClientMoveObj *) FindObject(
		dwObjID,
		bObjType);

	if(!pTOBJ)
		return TERR_NONE;

	if( GetPartyID(m_pMainChar) && IsHostingEnemy(pTOBJ) )
		pTOBJ->m_bTCORPSMOVE = TCORPSMOVE_ENEMY;

	switch(bObjType)
	{
	case OT_RECALL	:
		if(FindSlaveRecall(dwObjID))
			return TERR_NONE;

		break;

	case OT_MON		:
		if(FindSlave(dwObjID))
			return TERR_NONE;

		break;

	case OT_SELF	:
		if(FindSlaveFixRecall(dwObjID))
			return TERR_NONE;

		break;

	case OT_PC		: 
		if( static_cast<CTClientChar*>(pTOBJ)->GetRidingPet() )
			return TERR_NONE;
		
		break;

	default			: return TERR_NONE;
	}

	(*pPacket)
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wPITCH
		>> wDIR
		>> bAction
		>> pTOBJ->m_bBlock;

	pTOBJ->m_nPITCH = INT(wPITCH) - INT(pTOBJ->m_wPITCH);
	pTOBJ->m_nDIR = INT(wDIR) - INT(pTOBJ->m_wDIR);

	if( CTChart::GetTACTION( bAction )->m_bNavAction )
		pTOBJ->SetTAction(bAction);

	D3DXVECTOR3 vMOVE(
		fPosX - pTOBJ->GetPositionX(),
		fPosY - pTOBJ->GetPositionY(),
		fPosZ - pTOBJ->GetPositionZ());

	pTOBJ->AddPositionX(vMOVE.x);
	pTOBJ->AddPositionY(vMOVE.y);
	pTOBJ->AddPositionZ(vMOVE.z);

	pTOBJ->m_vWorld._41 += vMOVE.x;
	pTOBJ->m_vWorld._42 += vMOVE.y;
	pTOBJ->m_vWorld._43 += vMOVE.z;

	return TERR_NONE;
}

int CTClientGame::OnCS_MOVE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	WORD wPITCH;
	WORD wDIR;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	BYTE bAction;

	(*pPacket)
		>> dwCharID;

	CTClientChar *pCHAR = FindPlayer(dwCharID);
	if(!pCHAR)
		return TERR_NONE;

	if( GetPartyID(m_pMainChar) && IsHostingEnemy(pCHAR) )
		pCHAR->m_bTCORPSMOVE = TCORPSMOVE_ENEMY;

	(*pPacket)
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wPITCH
		>> wDIR
		>> pCHAR->m_bMouseDIR
		>> pCHAR->m_bKeyDIR
		>> bAction;

	pCHAR->m_nPITCH = INT(wPITCH) - INT(pCHAR->m_wPITCH);
	pCHAR->m_nDIR = INT(wDIR) - INT(pCHAR->m_wDIR);

	if(CTChart::GetTACTION( bAction )->m_bNavAction)
	{
		if( pCHAR->m_bAction != bAction )
			pCHAR->CancelTAction();

		pCHAR->SetTAction(bAction);
	}

	D3DXVECTOR3 vMOVE(
		fPosX - pCHAR->GetPositionX(),
		fPosY - pCHAR->GetPositionY(),
		fPosZ - pCHAR->GetPositionZ());

	if( D3DXVec3Length(&vMOVE) > FIX_DIST )
	{
		pCHAR->AddPositionX(vMOVE.x);
		pCHAR->AddPositionY(vMOVE.y);
		pCHAR->AddPositionZ(vMOVE.z);

		pCHAR->m_vWorld._41 += vMOVE.x;
		pCHAR->m_vWorld._42 += vMOVE.y;
		pCHAR->m_vWorld._43 += vMOVE.z;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_JUMP_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwObjID;
	BYTE bObjType;

	WORD wPITCH;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	(*pPacket)
		>> dwObjID
		>> bObjType;

	CTClientMoveObj *pTOBJ = NULL;
	switch(bObjType)
	{
	case OT_RECALL	:
		if(!FindSlaveRecall(dwObjID))
			pTOBJ = FindRecall(dwObjID);

		break;

	case OT_MON		:
		if(!FindSlave(dwObjID))
			pTOBJ = FindMonster(dwObjID);

		break;

	case OT_SELF	:
		if(!FindSlaveFixRecall(dwObjID))
			pTOBJ = FindFixRecall(dwObjID);

		break;

	case OT_PC		: 
		pTOBJ = FindPlayer(dwObjID);
		if( !pTOBJ || static_cast<CTClientChar*>(pTOBJ)->GetRidingPet() )
			return TERR_NONE;

		break;
	}

	if(!pTOBJ)
		return TERR_NONE;

	(*pPacket)
		>> fPosX
		>> fPosY
		>> fPosZ
		>> wPITCH
		>> pTOBJ->m_wMoveDIR;

	D3DXVECTOR3 vMOVE(
		fPosX - pTOBJ->GetPositionX(),
		fPosY - pTOBJ->GetPositionY(),
		fPosZ - pTOBJ->GetPositionZ());

	if( D3DXVec3Length(&vMOVE) > FIX_DIST )
	{
		pTOBJ->AddPositionX(vMOVE.x);
		pTOBJ->AddPositionY(vMOVE.y);
		pTOBJ->AddPositionZ(vMOVE.z);

		pTOBJ->m_vWorld._41 += vMOVE.x;
		pTOBJ->m_vWorld._42 += vMOVE.y;
		pTOBJ->m_vWorld._43 += vMOVE.z;
	}

	pTOBJ->m_nPITCH = INT(wPITCH) - INT(pTOBJ->m_wPITCH);
	pTOBJ->CancelTAction();
	pTOBJ->Jump();

	return TERR_NONE;
}

int CTClientGame::OnCS_MONMOVE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wMonCount = 0;

	(*pPacket)
		>> wMonCount;

	for( WORD i=0 ; i < wMonCount ; ++i )
	{
		DWORD dwMonID;
		WORD wPITCH;
		WORD wDIR;

		FLOAT fPosX;
		FLOAT fPosY;
		FLOAT fPosZ;

		BYTE bObjType;
		BYTE bAction;

		BYTE bMouseDIR;
		BYTE bKeyDIR;

		(*pPacket)
			>> dwMonID
			>> bObjType
			>> fPosX
			>> fPosY
			>> fPosZ
			>> wPITCH
			>> wDIR
			>> bMouseDIR
			>> bKeyDIR
			>> bAction;

		CTClientMoveObj *pMON = (CTClientMoveObj *) FindObject(
			dwMonID,
			bObjType);

		if(!pMON)
			continue;

		if( GetPartyID(m_pMainChar) && IsHostingEnemy(pMON) )
			pMON->m_bTCORPSMOVE = TCORPSMOVE_ENEMY;

		switch(bObjType)
		{
		case OT_RECALL	:
			if(FindSlaveRecall(dwMonID))
				continue;

			break;

		case OT_MON		:
			if(FindSlave(dwMonID))
				continue;

			break;

		case OT_SELF	:
			if(FindSlaveFixRecall(dwMonID))
				continue;

			break;

		default			:
				continue;
		}

		pMON->m_bMouseDIR = bMouseDIR;
		pMON->m_bKeyDIR = bKeyDIR;

		D3DXVECTOR3 vMOVE(
			fPosX - pMON->GetPositionX(),
			fPosY - pMON->GetPositionY(),
			fPosZ - pMON->GetPositionZ());

		pMON->m_nPITCH = INT(wPITCH) - INT(pMON->m_wPITCH);
		pMON->m_nDIR = INT(wDIR) - INT(pMON->m_wDIR);

		if(CTChart::GetTACTION( bAction )->m_bNavAction)
			pMON->SetTAction(bAction);

		if( D3DXVec3Length(&vMOVE) > FIX_DIST )
		{
			pMON->AddPositionX(vMOVE.x);
			pMON->AddPositionY(vMOVE.y);
			pMON->AddPositionZ(vMOVE.z);

			pMON->m_vWorld._41 += vMOVE.x;
			pMON->m_vWorld._42 += vMOVE.y;
			pMON->m_vWorld._43 += vMOVE.z;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_ADDMON_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwRegionID;
	DWORD dwMonID;

	WORD wTempID;
	WORD wPITCH;
	WORD wDIR;

	BYTE bNewMember;
	BYTE bCount;

	(*pPacket)
		>> dwMonID;

	if(FindMonster(dwMonID))
		return TERR_NONE;

	//static int _COUNT = 0;
	//TRACE("#%d : %u : OnCS_ADDMON_ACK\n", ++_COUNT, m_mapMONSTER.size()+1);

	CTClientMonster *pMON = new CTClientMonster();
	pMON->m_pHost = m_pMainWnd;
	pMON->ReleaseData();

	(*pPacket)
		>> wTempID
		>> pMON->m_bLevel
		>> pMON->m_dwMaxHP
		>> pMON->m_dwHP
		>> pMON->m_dwMaxMP
		>> pMON->m_dwMP
		>> pMON->m_vWorld._41
		>> pMON->m_vWorld._42
		>> pMON->m_vWorld._43
		>> wPITCH
		>> wDIR
		>> pMON->m_bMouseDIR
		>> pMON->m_bKeyDIR
		>> pMON->m_bAction
		>> pMON->m_bMode
		>> bNewMember
		>> pMON->m_bContryID
		>> pMON->m_bColorID
		>> dwRegionID
		>> bCount;

	pMON->m_pTREGION = CTChart::FindTREGIONINFO( dwRegionID);

	for( BYTE i=0; i<bCount; i++)
	{
		CTClientMaintain *pTMAINTAIN = new CTClientMaintain();

		(*pPacket)
			>> pTMAINTAIN->m_vTSKILLDATA.m_wID
			>> pTMAINTAIN->m_vTSKILLDATA.m_bLevel
			>> pTMAINTAIN->m_dwTick
			>> pTMAINTAIN->m_dwAttackID
			>> pTMAINTAIN->m_bAttackType
			>> pTMAINTAIN->m_dwHostID
			>> pTMAINTAIN->m_bHostType
			>> pTMAINTAIN->m_vTSKILLDATA.m_bHIT
			>> pTMAINTAIN->m_vTSKILLDATA.m_wAL
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkLevel
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAglow
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkCountryID
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.x
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.y
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.z;
		pTMAINTAIN->m_pTSKILL = CTChart::FindTSKILLTEMP(pTMAINTAIN->m_vTSKILLDATA.m_wID);

		if(pTMAINTAIN->m_pTSKILL)
		{
			pTMAINTAIN->m_pTIMMER = CTClientSkill::FindTIMMER(pTMAINTAIN->m_pTSKILL);
			pTMAINTAIN->m_bLevel = pTMAINTAIN->m_vTSKILLDATA.m_bLevel;

			pTMAINTAIN->m_bUnlimitedType = pTMAINTAIN->m_pTSKILL->m_dwDuration ? FALSE : TRUE;
			pTMAINTAIN->m_dwEndTick = pTMAINTAIN->m_dwTick + m_vKEY.m_dwTick;

			pMON->InsertMaintain( pTMAINTAIN->m_vTSKILLDATA.m_wID, pTMAINTAIN);
			pMON->PlayTActionSFX(
				&pTMAINTAIN->m_vTSFX,
				pTMAINTAIN->m_pTSKILL->m_dwSFX[TSKILLSFX_LOOP]);
		}
		else
			delete pTMAINTAIN;
	}

	pMON->m_bACTLevel = CTChart::GetTACTION(pMON->m_bAction)->m_bLevel;
	pMON->m_nPITCH = INT(wPITCH) - INT(pMON->m_wPITCH);
	pMON->m_nDIR = INT(wDIR) - INT(pMON->m_wDIR);
	pMON->m_dwID = dwMonID;

	pMON->InitMonster(
		m_pDevice,
		m_pRES,
		wTempID);

	pMON->DoSkillFuncAll( this );

	m_mapMONSTER.insert( MAPMONSTER::value_type( pMON->m_dwID, pMON));
	m_vMAP.LandTOBJ(pMON);

	if(bNewMember)
	{
		pMON->ApplyTAction( m_pDevice, m_pRES, TA_SPAWN);

		if(pMON->m_pMON)
		{
			if(pMON->m_pMON->m_wSpawnSFX)
			{
				pMON->PlayTActionSFX(
					&pMON->m_vTGARBAGESFX,
					pMON->m_pMON->m_wSpawnSFX);
			}

			if(pMON->m_pMON->m_dwSpawnSND)
			{
				pMON->PlaySND(
					pMON->m_pMON->m_dwSpawnSND,
					MEDIA_TSOUND,
					TRUE);
			}
		}
	}

	if( GetPartyID(m_pMainChar) && IsRSCSEnemy(pMON) )
	{
		pMON->CalcDIR( 0, pMON->m_bMouseDIR, pMON->m_bKeyDIR);
		pMON->CalcTick(
			&D3DXVECTOR3(
			pMON->GetPositionX(),
			pMON->GetPositionY(),
			pMON->GetPositionZ()),
			m_pDevice,
			m_pRES, 0);

		RelaySession()->SendCS_ADDCORPSENEMY_REQ(
			m_pMainChar->m_dwID,
			m_vMAP.m_wMapID,
			pMON);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DELMON_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwMonID;
	BYTE bExitMap;

	(*pPacket)
		>> dwMonID
		>> bExitMap;

	MAPMONSTER::iterator finder = m_mapSLAVEMON.find(dwMonID);
	if( finder != m_mapSLAVEMON.end() )
		m_mapSLAVEMON.erase(finder);

	finder = m_mapMONSTER.find(dwMonID);
	if( finder != m_mapMONSTER.end() )
	{
		if( GetPartyID(m_pMainChar) && IsRSCSEnemy((*finder).second) )
		{
			RelaySession()->SendCS_DELCORPSENEMY_REQ(
				m_pMainChar->m_dwID,
				(*finder).second->m_dwID,
				(*finder).second->m_bType);
		}

		DeleteDynimicOBJ((*finder).second);
		m_mapMONSTER.erase(finder);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CHGMODE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;
	BYTE bType;

	(*pPacket)
		>> dwID
		>> bType;

	CTClientObjBase *pOBJ = FindOBJ(
		dwID,
		bType);

	if( pOBJ )
	{
		(*pPacket)
			>> pOBJ->m_bMode;

		if( pOBJ->m_bType == OT_MON )
		{
			CTClientMonster* pMONSTER = (CTClientMonster*) pOBJ;

			if( pMONSTER->m_bFollowType != OT_NONE )
			{
				CTClientObjBase* pBASE = FindOBJ( pMONSTER->m_dwFollowID, pMONSTER->m_bFollowType );

				if(pBASE)
					pBASE->LeaveAdjacency( pMONSTER );
			}

			switch( pMONSTER->m_bMode )
			{
			case MT_GOHOME	:
				{
					pMONSTER->m_bFollowType = OT_NONE;
					pMONSTER->m_dwFollowID = 0;
					pMONSTER->m_bTD = TD_NONE;
					pMONSTER->m_bGoHome = TRUE;

					pMONSTER->m_bMode = MT_NORMAL;
				}

				break;

			case MT_NORMAL	:
				{
					pMONSTER->m_bFollowType = OT_NONE;
					pMONSTER->m_dwFollowID = 0;
					pMONSTER->m_bTD = TD_NONE;
				}

			default			: pMONSTER->m_bGoHome = FALSE; break;
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_REVIVAL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	CTClientObjBase *pREVIVAL = NULL;
	DWORD dwCharID;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	(*pPacket)
		>> dwCharID
		>> fPosX
		>> fPosY
		>> fPosZ;

	pREVIVAL = FindPC(dwCharID);
	if( pREVIVAL && (pREVIVAL->IsDead() || pREVIVAL->m_bGhost) )
	{
		Revival( pREVIVAL, fPosX, fPosY, fPosZ);

		if( pREVIVAL == m_pTARGET )
			pTGaugePannel->ResetHotkeyUI();

		/*m_pTRSCS->OnRevivalChar(
			m_pMainChar,
			dwCharID,
			(CTClientChar*) pREVIVAL );*/
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_ACTION_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwObjID;
	DWORD dwActID;
	DWORD dwAniID;

	BYTE bActionID;
	BYTE bObjType;
	BYTE bResult;

	WORD wSkillID;

	(*pPacket)
		>> bResult
		>> dwObjID
		>> bObjType
		>> bActionID
		>> dwActID
		>> dwAniID
		>> wSkillID;

	CTClientObjBase *pOBJ = FindOBJ(
		dwObjID,
		bObjType);

	if( pOBJ == m_pMainChar )
	{
		if(m_bGroundLOCK)
			m_bGroundLOCK = FALSE;

		m_bLoopLock = FALSE;
		m_wNextSkillID = 0;

		if(CTChart::GetTACTION( bActionID )->m_bSkipMain)
			return TERR_NONE;
	}

	switch(bResult)
	{
	case SKILL_SUCCESS	:
		{
			CTClientSkill *pTMainSkill = NULL;
			if( m_pMainChar == pOBJ )
				pTMainSkill = m_pMainChar->FindTSkill(wSkillID);

			if(pOBJ)
			{
				if( m_pMainChar == pOBJ && m_bCHARGE )
					CancelCharge(wSkillID ? FALSE : TRUE);
				pOBJ->CancelTAction();

				LPTSKILL pTSKILL = NULL;
				if( wSkillID )
					pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);

				if( CTChart::GetTACTION( bActionID )->m_bLevel >= pOBJ->m_bACTLevel &&
					dwActID && dwAniID &&
					bActionID != TA_DIE &&
					!pOBJ->IsDead() &&
					!pOBJ->IsDown() &&
					(!pOBJ->IsBlock() || (pTSKILL && pTSKILL->m_bUseInHold)) )
				{
					if(wSkillID)
					{
						if( pTMainSkill )
						{
							BeginCharge(
								pTMainSkill->GetChargeTick(m_pMainChar->m_bChargeSpeed),
								TCHARGETYPE_SKILL);

							m_pMainChar->m_vTSKILLDATA.m_wID = wSkillID;

							pTSKILL = pTMainSkill->m_pTSKILL;
						}

						if(pTSKILL)
						{
							pOBJ->PlayTActionSFX(
								&pOBJ->m_vTACTIONSFX,
								pTSKILL->m_dwSFX[TSKILLSFX_SPELL]);
						}

						if( !pOBJ->IsBlock() )
							pOBJ->SetTAction(bActionID);
					}
					else
					{
						BYTE bLoopACT = pOBJ->GetTLOOPAction(bActionID);

						if( bLoopACT != TA_COUNT )
							pOBJ->SetTAction(bLoopACT);
						else if(CTChart::GetTACTION( bActionID )->m_bContinue)
							pOBJ->SetTAction(bActionID);
					}

					BYTE bHideOnCapeMode = CTChart::GetTACTION(bActionID)->m_bHideOnCapeMode;
					BYTE bEquipMode = pOBJ->GetEquipMode(
						bActionID,
						pOBJ->GetWeaponID(pOBJ->m_bMode));

					if( pOBJ->m_bHideOnCapeMode != bHideOnCapeMode ||
						pOBJ->m_bEquipMode != bEquipMode )
					{
						pOBJ->m_bHideOnCapeMode = bHideOnCapeMode;
						pOBJ->m_bEquipMode = bEquipMode;

						pOBJ->ResetEQUIP(
							m_pDevice,
							m_pRES);
					}

					if( pOBJ->CanUseSubANI() && CTChart::GetTACTION( bActionID )->m_bSubAction )
					{
						pOBJ->SetSubAction(
							dwActID,
							dwAniID);
					}
					else
					{
						pOBJ->m_bACTLevel = CTChart::GetTACTION( bActionID )->m_bLevel;
						pOBJ->SetAction(
							dwActID,
							dwAniID);
					}
				}
				else if( pOBJ == m_pMainChar )
					m_pACT = NULL;
			}
		}

		break;

	default				:
		if(pOBJ)
		{
			if( wSkillID && IsHostingOBJ( pOBJ->m_dwID, pOBJ->m_bType) )
			{
				CTClientSkill *pTSKILL = pOBJ->FindTSkill(wSkillID);

				if(pTSKILL)
					SkillMSG( pOBJ, bResult);
			}

			if( pOBJ == m_pMainChar )
				m_pACT = NULL;
		}

		break;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_MONATTACK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwAttackID;
	DWORD dwTargetID;

	BYTE bAttackType;
	BYTE bTargetType;
	WORD wSkillID;

	(*pPacket)
		>> dwAttackID
		>> dwTargetID
		>> bAttackType
		>> bTargetType
		>> wSkillID;

	LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);

	CTClientObjBase *pATTACK = FindOBJ(
		dwAttackID,
		bAttackType);

	CTClientObjBase *pDEFEND = FindOBJ(
		dwTargetID,
		bTargetType);

	if( pTSKILL->m_bTargetType == TSKILLTARGET_SELF ) /* ||
		pTSKILL->m_bTargetType == TSKILLTARGET_ALLIANCE )*/
		pDEFEND = pATTACK;

	if( pATTACK && pTSKILL )
	{
		D3DXVECTOR3 vGROUND = pDEFEND ? D3DXVECTOR3(
			pDEFEND->GetPositionX(),
			pDEFEND->GetPositionY(),
			pDEFEND->GetPositionZ()) : D3DXVECTOR3(
			pATTACK->GetPositionX(),
			pATTACK->GetPositionY(),
			pATTACK->GetPositionZ());

		TSKILLTARGET_PARAM param;
		if( FindSkillTarget(
			pATTACK, 
			pDEFEND, 
			&vGROUND, 
			pTSKILL, 
			&param) )
		{
			SendCS_SKILLUSE_REQ(
				pATTACK,
				pDEFEND,
				&vGROUND,
				pTSKILL,
				FALSE,
				&param);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SKILLUSE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwAttackID;
	WORD wSkillID;
	WORD wBackSkillID;	// 스킬 사용시 반격스킬(아이템스킬)의 아이디

	BYTE bAttackType;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> dwAttackID
		>> bAttackType
		>> wSkillID
		>> wBackSkillID;

	CTClientObjBase *pATTACK = FindOBJ(
		dwAttackID,
		bAttackType);

	if(!pATTACK)
		return TERR_NONE;

	if( m_pMainChar == pATTACK )
	{
		if(m_bGroundLOCK)
			m_bGroundLOCK = FALSE;

		if(m_bCHARGE)
			CancelCharge(TRUE);

		m_bLoopLock = FALSE;
		m_wNextSkillID = 0;
	}
	pATTACK->CancelTAction();

	switch(bResult)
	{
	case SKILL_SUCCESS	:
		{
			LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);

			if(!pTSKILL)
				return TERR_NONE;

			CTClientSkill *pTSkill = pATTACK->FindTSkill(wSkillID);
			
			BYTE bRANGE =
				((pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE) ||
				(pTSKILL->m_bRangeType == TSKILLRANGE_LINE) ||
				(pTSKILL->m_bRangeType == TSKILLRANGE_MCIRCLE)) ? TRUE : FALSE;

			BYTE bActionID;
			BYTE bCount;

			DWORD dwActID;
			DWORD dwAniID;

			TSKILLDATA vTSKILLDATA;

			(*pPacket)
				>> bActionID
				>> dwActID
				>> dwAniID
				>> vTSKILLDATA.m_bLevel
				>> vTSKILLDATA.m_wAL
				>> vTSKILLDATA.m_bAtkLevel
				>> vTSKILLDATA.m_dwMinAP
				>> vTSKILLDATA.m_dwMaxAP
				>> vTSKILLDATA.m_dwMinMP
				>> vTSKILLDATA.m_dwMaxMP
				>> vTSKILLDATA.m_wSTR
				>> vTSKILLDATA.m_wINT
				>> vTSKILLDATA.m_bCURSE
				>> vTSKILLDATA.m_bSpecial
				>> vTSKILLDATA.m_bAglow
				>> vTSKILLDATA.m_bAtkCountryID
				>> vTSKILLDATA.m_bHIT
				>> vTSKILLDATA.m_vTGROUND.x
				>> vTSKILLDATA.m_vTGROUND.y
				>> vTSKILLDATA.m_vTGROUND.z
				>> bCount;

			if( CTChart::GetTACTION( bActionID )->m_bLevel > pATTACK->m_bACTLevel &&
				(bRANGE || bCount > 0) &&
				!pATTACK->m_bGhost && !pATTACK->IsDead() && 
				!pATTACK->IsDown() &&
				(!pATTACK->IsBlock() || pTSKILL->m_bUseInHold) )
			{
				BYTE bSkillItemType = pATTACK->GetSkillItemType(pTSKILL);

				pATTACK->ClearSkillTarget();
				pATTACK->ClearTARROW();
				pATTACK->m_bCLEARTARGET = FALSE;
				
				if( !pTSKILL->m_wModeID && 
					pTSKILL->m_bTargetType == TSKILLTARGET_ENEMY )
				{
					pATTACK->m_bMode = MT_BATTLE;
				}

				for( BYTE i=0; i<bCount; i++)
				{
					LPTSKILLTARGET pTTARGET = new TSKILLTARGET();

					(*pPacket)
						>> pTTARGET->m_dwID
						>> pTTARGET->m_bType;

					pATTACK->m_vTSKILLTARGET.push_back(pTTARGET);
				}

				if( pATTACK->CanUseSubANI() && CTChart::GetTACTION( bActionID )->m_bSubAction )
				{
					pATTACK->m_dwTSUBACT = 0;
					pATTACK->SetSubAction(
						dwActID,
						dwAniID);
				}
				else
				{
					pATTACK->m_bACTLevel = CTChart::GetTACTION( bActionID )->m_bLevel;
					pATTACK->ClearAnimationID();

					pATTACK->SetAction(
						dwActID,
						dwAniID);
				}
				pATTACK->m_bActionLock = TRUE;

				if(pTSKILL->m_bContinue)
				{
					BYTE bLoopACT = pATTACK->GetTLOOPAction(bActionID);

					if( bLoopACT != TA_COUNT )
					{
						pATTACK->SetTAction(bLoopACT);

						if( pATTACK == m_pMainChar )
						{
							m_bLOOPCHARGE = TRUE;

							BeginCharge(
								pTSKILL->m_dwInterval,
								TCHARGETYPE_SKILL);
						}
					}
					else if( pATTACK == m_pMainChar )
						m_pACT = NULL;
				}

				if(pTSKILL->m_wItemID[bSkillItemType])
				{
					LPTITEM pTITEM = CTChart::FindTITEMTEMP(pTSKILL->m_wItemID[bSkillItemType]);
					pATTACK->m_bShoot = TRUE;

					LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTITEM);
					if(pTVISUAL && pTVISUAL->m_dwPivot[MT_NORMAL])
					{
						MAPDWORD::iterator itID = pATTACK->m_OBJ.m_pOBJ
							->m_mapPIVOT.find(pTVISUAL->m_dwPivot[MT_NORMAL]);

						if( itID != pATTACK->m_OBJ.m_pOBJ->m_mapPIVOT.end() )
						{
							CTClientArrow *pTARROW = new CTClientArrow();

							pTARROW->InitOBJ(m_pRES->GetOBJ(pTVISUAL->m_dwObjectID));
							pTARROW->SetCloth(
								m_pDevice,
								pTVISUAL->m_dwCLKID,
								pTVISUAL->m_dwCLIID,
								pTVISUAL->m_dwMesh[MT_NORMAL]);

							pTARROW->ResetOBJPart(m_pDevice);
							pATTACK->m_mapEQUIP.insert( MAPOBJECT::value_type( (*itID).second + 1, pTARROW));
						}
					}
				}
				else
					pATTACK->m_bShoot = FALSE;

				LPTACTION pTACTION = CTChart::FindTACTIONTEMP(
					bActionID,
					pATTACK->GetWeaponID(pATTACK->m_bMode) );
				BYTE bHideOnCapeMode = CTChart::GetTACTION(bActionID)->m_bHideOnCapeMode;

				if( pATTACK->m_bHideOnCapeMode != bHideOnCapeMode ||
					pATTACK->m_bEquipMode != pTACTION->m_bEquipMode )
				{
					pATTACK->m_bEquipMode = pTACTION->m_bEquipMode;
					pATTACK->m_bHideOnCapeMode = bHideOnCapeMode;

					pATTACK->ResetEQUIP(
						m_pDevice,
						m_pRES);
				}

				pATTACK->m_vTSKILLDATA = vTSKILLDATA;
				pATTACK->m_vTSKILLDATA.m_wID = wSkillID;

				// 아이템 스킬
				// --------------------------------------------------
				pATTACK->m_vTBACKSKILLDATA = pATTACK->m_vTSKILLDATA;
				pATTACK->m_vTBACKSKILLDATA.m_wID = wBackSkillID;
				pATTACK->m_vTBACKSKILLDATA.m_bLevel = 1;

				if( wBackSkillID )
				{
					LPTSKILL pTBACKSKILL = CTChart::FindTSKILLTEMP(wBackSkillID);
					BYTE bBackSkillItemType = pATTACK->GetSkillItemType(pTBACKSKILL);
					if( !CheckBackSkill(pTBACKSKILL, bBackSkillItemType) )
						pATTACK->m_vTBACKSKILLDATA.m_wID = 0;
				}
				// --------------------------------------------------
                
				if(pTACTION->m_dwTSLASH)
					pATTACK->PlayTSLASH(pTACTION->m_dwTSLASH);

				if( IsHostingOBJ( pATTACK->m_dwID, pATTACK->m_bType) && pTSkill )
					ApplySkillTick(pATTACK, pTSkill);
			}
			else if(IsHostingOBJ( pATTACK->m_dwID, pATTACK->m_bType))
			{
				CTClientSkill *pTSkill = pATTACK->FindTSkill(wSkillID);

				if(pTSkill)
				{
					EnableSkillTimer(pATTACK, pTSkill, FALSE);
					m_pSESSION->SendCS_CANCELSKILL_REQ(
						pATTACK->m_bType,
						pATTACK->m_dwID,
						pTSkill->m_pTSKILL->m_wSkillID);

					if( pTSkill->m_bLoopON && (
						pATTACK->m_bGhost ||
						pATTACK->IsDead() ||
						pATTACK->IsBlock() ||
						pATTACK->IsDown() || (
						bCount == 0 && !bRANGE)) )
						pTSkill->m_bLoopON = FALSE;

					if( pTSkill->m_bLoopON || pATTACK->m_bType == OT_RECALL || pATTACK->m_bType == OT_SELF )
						pTSkill->m_dwExceptTick = TEXCEPTSKILL_TICK;
				}

				if( pATTACK == m_pMainChar )
					m_pACT = NULL;
			}
		}

		break;

	default				:
		if(IsHostingOBJ( pATTACK->m_dwID, pATTACK->m_bType))
		{
			CTClientSkill *pTSKILL = pATTACK->FindTSkill(wSkillID);

			if(pTSKILL)
			{
				EnableSkillTimer(pATTACK, pTSKILL, FALSE);

				if( bResult != SKILL_SPEEDYUSE )
				{
					if(pTSKILL->m_bLoopON)
						pTSKILL->m_bLoopON = FALSE;

					SkillMSG( pATTACK, bResult);

					if( pATTACK->m_bType == OT_RECALL )
					{
						CTClientRecall* pTRECALL = (CTClientRecall*) pATTACK;
						pTRECALL->m_pTCURSKILL = NULL;
						pTRECALL->m_bSubAI = TRECALLAI_STAY;
						pTRECALL->m_bTargetType = OT_NONE;
					}
				}

				if( pTSKILL->m_bLoopON || pATTACK->m_bType == OT_RECALL || pATTACK->m_bType == OT_SELF )
					pTSKILL->m_dwExceptTick = TEXCEPTSKILL_TICK;
			}

			if( pATTACK == m_pMainChar )
				m_pACT = NULL;
		}

		break;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_LOOPSKILL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwAttackID;
	WORD wSkillID;

	BYTE bAttackType;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> dwAttackID
		>> bAttackType
		>> wSkillID;

	CTClientObjBase *pATTACK = FindOBJ(
		dwAttackID,
		bAttackType);

	if(!pATTACK)
	{
		if( m_pMainChar == pATTACK && m_bCHARGE )
			CancelCharge(TRUE);
		pATTACK->CancelTAction();

		return TERR_NONE;
	}

	switch(bResult)
	{
	case SKILL_SUCCESS		:
		{
			LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);

			if(!pTSKILL)
			{
				if( m_pMainChar == pATTACK && m_bCHARGE )
					CancelCharge(TRUE);
				pATTACK->CancelTAction();

				return TERR_NONE;
			}

			BYTE bRANGE = pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE ? TRUE : FALSE;
			BYTE bCount;

			(*pPacket)
				>> pATTACK->m_vTSKILLDATA.m_bLevel
				>> pATTACK->m_vTSKILLDATA.m_wAL
				>> pATTACK->m_vTSKILLDATA.m_bAtkLevel
				>> pATTACK->m_vTSKILLDATA.m_dwMinAP
				>> pATTACK->m_vTSKILLDATA.m_dwMaxAP
				>> pATTACK->m_vTSKILLDATA.m_dwMinMP
				>> pATTACK->m_vTSKILLDATA.m_dwMaxMP
				>> pATTACK->m_vTSKILLDATA.m_bAglow
				>> pATTACK->m_vTSKILLDATA.m_bAtkCountryID
				>> pATTACK->m_vTSKILLDATA.m_bHIT
				>> pATTACK->m_vTSKILLDATA.m_vTGROUND.x
				>> pATTACK->m_vTSKILLDATA.m_vTGROUND.y
				>> pATTACK->m_vTSKILLDATA.m_vTGROUND.z
				>> bCount;

			if( !pATTACK->m_bGhost && !pATTACK->IsDead() && 
				!pATTACK->IsDown() && 
				(bRANGE || bCount > 0) &&
				(!pATTACK->IsBlock() || pTSKILL->m_bUseInHold) )
			{
				BYTE bSkillItemType = pATTACK->GetSkillItemType(pTSKILL);

				pATTACK->ClearSkillTarget();
				pATTACK->ClearTARROW();

				for( BYTE i=0; i<bCount; i++)
				{
					LPTSKILLTARGET pTTARGET = new TSKILLTARGET();

					(*pPacket)
						>> pTTARGET->m_dwID
						>> pTTARGET->m_bType;

					pATTACK->m_vTSKILLTARGET.push_back(pTTARGET);
				}

				if(pTSKILL->m_wItemID[bSkillItemType])
				{
					pATTACK->m_bShoot = TRUE;

					LPTITEM pTITEM = CTChart::FindTITEMTEMP(pTSKILL->m_wItemID[bSkillItemType]);
					LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTITEM);
					if(pTVISUAL && pTVISUAL->m_dwPivot[MT_NORMAL])
					{
						MAPDWORD::iterator itID = pATTACK->m_OBJ.m_pOBJ->m_mapPIVOT.find(pTVISUAL->m_dwPivot[MT_NORMAL]);

						if( itID != pATTACK->m_OBJ.m_pOBJ->m_mapPIVOT.end() )
						{
							CTClientArrow *pTARROW = new CTClientArrow();

							pTARROW->InitOBJ(m_pRES->GetOBJ(pTVISUAL->m_dwObjectID));
							
							pTARROW->SetCloth(
								m_pDevice,
								pTVISUAL->m_dwCLKID,
								pTVISUAL->m_dwCLIID,
								pTVISUAL->m_dwMesh[MT_NORMAL]);
							pTARROW->ResetOBJPart(m_pDevice);

							pATTACK->m_mapEQUIP.insert( MAPOBJECT::value_type( (*itID).second + 1, pTARROW));
						}
					}
				}
				else
					pATTACK->m_bShoot = FALSE;
				pATTACK->m_vTSKILLDATA.m_wID = wSkillID;

				if( pATTACK->m_bShoot && pTSKILL->m_wItemID[pATTACK->GetSkillItemType(pTSKILL)] )
					ShotArrow( pATTACK, &pATTACK->m_vTSKILLDATA, pTSKILL );
				else
					ShotSkill( pATTACK, &pATTACK->m_vTSKILLDATA );
			}
			else
			{
				if( m_pMainChar == pATTACK && m_bCHARGE )
					CancelCharge(TRUE);

				pATTACK->CancelTAction();
			}
		}

		break;

	case SKILL_SPEEDYUSE	:
			if( m_pMainChar == pATTACK && m_bCHARGE )
				m_dwChargeTick += m_dwMaxChargeTick;

		break;

	default					:
		{
			if( m_pMainChar == pATTACK && m_bCHARGE )
				CancelCharge(TRUE);

			pATTACK->CancelTAction();
		}

		break;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DEFEND_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	D3DXVECTOR3 vAtkPos;
	D3DXVECTOR3 vDefPos;

	DWORD dwAttackID;
	DWORD dwTargetID;
	DWORD dwHostID;
	DWORD dwActID;
	DWORD dwAniID;
	DWORD dwMinAP;
	DWORD dwMaxAP;
	DWORD dwMinMP;
	DWORD dwMaxMP;
	DWORD dwTick;

	BYTE bCancelCharge;
	BYTE bAttackType;
	BYTE bTargetType;
	BYTE bHostType;
	BYTE bAtkLevel;
	BYTE bMaintain;
	BYTE bSkillAct;
	BYTE bTCR;
	BYTE bHit;

	WORD wBackSkillID;
	WORD wSkillID;
	WORD wAL;

	BYTE bAtkCountryID;
	BYTE bAglow;
	BYTE bLevel;
	BYTE bCount;

	(*pPacket)
		>> dwAttackID
		>> dwTargetID
		>> bAttackType
		>> bTargetType
		>> dwHostID
		>> bHostType
		>> dwActID
		>> dwAniID
		>> bMaintain
		>> dwTick
		>> bTCR
		>> bHit
		>> wAL
		>> bAtkLevel
		>> dwMinAP
		>> dwMaxAP
		>> dwMinMP
		>> dwMaxMP
		>> bAglow
		>> bCancelCharge
		>> bAtkCountryID
		>> wSkillID
		>> bLevel
		>> wBackSkillID
		>> bSkillAct
		>> vAtkPos.x
		>> vAtkPos.y
		>> vAtkPos.z
		>> vDefPos.x
		>> vDefPos.y
		>> vDefPos.z
		>> bCount;

	CTClientObjBase *pTDEFEND = FindOBJ(
		dwTargetID,
		bTargetType);

	CTClientObjBase *pTATTACK = FindOBJ(
		dwAttackID,
		bAttackType);
	CTGaugePannel *pTGaugePannel = (CTGaugePannel *) m_vTFRAME[TFRAME_GAUGE];

	if(pTDEFEND)
	{
		LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);

		if( m_pMainChar == pTDEFEND )
		{
			if(m_bTournamentWatching)
				return TERR_NONE;

			if( m_bCHARGE && (bCancelCharge || m_eCHARGETYPE == TCHARGETYPE_OPEN) &&
				pTSKILL && CTChart::IsNegativeSkill(pTSKILL) )
				CancelCharge(TRUE);

			if( CTChart::IsNegativeSkill(pTSKILL) &&
				(m_bDoEXIT || m_bDoSelectCHAR || m_bDoChangeCH) )
				CancelDoEXIT();
		}

		if( bCancelCharge || (
			pTSKILL->m_bActionID[TSKILLACTION_DEFEND] != TA_STAND &&
			pTSKILL->m_bActionID[TSKILLACTION_DEFEND] != TA_COUNT &&
			CTChart::GetTACTION(pTDEFEND->m_bAction)->m_bNavAction) )
			pTDEFEND->CancelTAction();

		if( pTSKILL && pTDEFEND->m_dwHP )
		{
			if( !m_pMainChar->m_bGhost && (
				IsHostingOBJ( pTDEFEND->m_dwID, pTDEFEND->m_bType) ||
				IsHostingOBJ( dwAttackID, bAttackType) ||
				m_bTournamentWatching) )
			{
				switch(bHit)
				{
				case HT_CRITICAL	:
					if(pTSKILL->m_bShowCritical)
					{
						if(CTNationOption::CAMERA_QUAKE)
						{
							if( m_pMainChar->m_dwID == dwAttackID && m_pMainChar->m_bType == bAttackType )
								DoQUAKE(TQUAKE_ATTACK);

							if( m_pMainChar == pTDEFEND )
								DoQUAKE(TQUAKE_DEFEND);
						}

						pTDEFEND->m_vTDAMAGE.push_back(TCRITICAL_NUMBER);
					}

					break;

				case HT_BLOCK		: pTDEFEND->m_vTDAMAGE.push_back(TBLOCK_NUMBER); break;
				case HT_MISS		: pTDEFEND->m_vTDAMAGE.push_back(TMISS_NUMBER); break;
				}
			}

			switch(bHit)
			{
			case HT_CRITICAL	:
				if( CTNationOption::ADD_SLIDE &&
					pTSKILL->m_bShowCritical )
					pTDEFEND->DoSLIDE( &vAtkPos, &vDefPos, FALSE);

				break;

			case HT_LASTHIT		:
				if(pTDEFEND->IsSlidingWhenLastHit())
					pTDEFEND->DoSLIDE( &vAtkPos, &vDefPos, TRUE);

				break;
			}

			for( BYTE i=0; i<bCount; i++)
			{
				DWORD dwDamage;
				BYTE bType;

				(*pPacket)
					>> bType
					>> dwDamage;

				if( !m_pMainChar->m_bGhost && dwDamage && (
					IsHostingOBJ( pTDEFEND->m_dwID, pTDEFEND->m_bType) ||
					IsHostingOBJ( dwAttackID, bAttackType) ||
					m_bTournamentWatching) )
				{
					if( bType == MTYPE_HP ||
						bType == MTYPE_DAMAGE )
					{
						CString strTYPE;
						CString strMSG;

						DWORD dwCLR;

						if( m_pMainChar == pTDEFEND )
						{
							if(pTATTACK)
								strMSG = CTChart::Format( TSTR_FMT_DEFENDMA, pTATTACK->GetName(), dwDamage);
							else
								strMSG = CTChart::Format( TSTR_FMT_DEFENDM, dwDamage);

							dwCLR = TCOLOR_DEFEND;
						}
						else if( m_pMainChar == pTATTACK )
						{
							strMSG = CTChart::Format( TSTR_FMT_ATTACK, pTDEFEND->GetName(), dwDamage);
							dwCLR = TCOLOR_ATTACK;
						}
						else if(m_pMainChar->IsAlliance(pTDEFEND))
						{
							if(pTATTACK)
								strMSG = CTChart::Format( TSTR_FMT_DEFENDA, pTATTACK->GetName(), pTDEFEND->GetName(), dwDamage);
							else
								strMSG = CTChart::Format( TSTR_FMT_DEFEND, pTDEFEND->GetName(), dwDamage);

							dwCLR = TCOLOR_DEFEND;
						}
						else if(pTATTACK)
						{
							strMSG = CTChart::Format( TSTR_FMT_ATTACKA, pTATTACK->GetName(), pTDEFEND->GetName(), dwDamage);
							dwCLR = TCOLOR_ATTACK;
						}
						else
						{
							strMSG = CTChart::Format( TSTR_FMT_ATTACK, pTDEFEND->GetName(), dwDamage);
							dwCLR = TCOLOR_ATTACK;
						}

						strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
						m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG,	dwCLR, TCHAT_FLAG_INFO);
					}

					pTDEFEND->m_vTDAMAGE.push_back(MAKE_TDAMAGE( dwDamage, bType));
				}
			}

			if(CTClientSkill::IsAttackSKILL(pTSKILL))
			{
				if( IsHostingOBJ( pTDEFEND->m_dwID, pTDEFEND->m_bType) && !m_pMainChar->m_bFlash )
				{
					CTClientRecall *pTRECALL = GetMainRecall();

					if( pTATTACK && pTATTACK->m_bCanSelected && !m_pTARGET )
					{
						CTClientObjBase::m_mapTSELECTOBJ.clear();
						//ResetTargetOBJ(pTATTACK);
						//ResetTargetINFO(FALSE);
					}

					if( pTRECALL && pTRECALL->m_pTCURSKILL &&
						pTRECALL->m_bSubAI == TRECALLAI_COUNT &&
						pTRECALL->m_bTargetType == OT_NONE &&
						pTRECALL->m_bAI != TRECALLAI_MANUAL )
						SetRecallTarget( dwAttackID, bAttackType, TMONSKILL_COUNT);
				}

				if( IsHostingOBJ( dwAttackID, bAttackType) &&
					pTDEFEND->m_bContryID != TCONTRY_N &&
					m_pMainChar->m_pTREGION &&
					m_pMainChar->m_pTREGION->m_bInfo != TREGIONINFO_BATTLE &&
					!m_pMainChar->IsAlliance(pTDEFEND) )
				{
					SendCS_DEFEND_REQ(
						m_pMainChar->GetHostID(),
						m_pMainChar->m_dwID,
						m_pMainChar->m_dwID,
						m_pMainChar->m_bType,
						m_pMainChar->m_bType,
						GetPartyID(m_pMainChar),
						0, 0,
						m_pMainWnd->m_bChannel,
						m_vMAP.m_wMapID,
						0, 0, 0, 0, 0,
						0,0,0,
						0, 0,
						m_pMainChar->m_bContryID,
						0, 0,
						TBATTLE_SKILL, 1,
						m_pMainChar->GetPositionX(),
						m_pMainChar->GetPositionY(),
						m_pMainChar->GetPositionZ(),
						m_pMainChar->GetPositionX(),
						m_pMainChar->GetPositionY(),
						m_pMainChar->GetPositionZ(), 0);
				}
			}
		}

		if( pTSKILL && (pTDEFEND->m_dwHP || pTSKILL->m_bStaticWhenDie) )
		{
			if(bMaintain)
			{
				pTDEFEND->EraseSendedAddMaintain(wSkillID);
				CTClientMaintain *pTMAINTAIN = pTDEFEND->FindMaintain(wSkillID);

				if(!pTMAINTAIN)
				{
					pTMAINTAIN = new CTClientMaintain();

					pTMAINTAIN->m_pTIMMER = CTClientSkill::FindTIMMER(pTSKILL);
					pTMAINTAIN->m_pTSKILL = pTSKILL;

					pTDEFEND->PlayTActionSFX(
						&pTMAINTAIN->m_vTSFX,
						pTMAINTAIN->m_pTSKILL->m_dwSFX[TSKILLSFX_LOOP]);

					pTDEFEND->InsertMaintain( wSkillID, pTMAINTAIN);
				}

				pTMAINTAIN->m_bAttackType = bAttackType;
				pTMAINTAIN->m_dwAttackID = dwAttackID;

				switch(bAttackType)
				{
				case OT_RECALL	:
					{
						pTMAINTAIN->m_bHostType = bHostType;
						pTMAINTAIN->m_dwHostID = dwHostID;
					}

					break;

				default			:
					{
						pTMAINTAIN->m_bHostType = bAttackType;
						pTMAINTAIN->m_dwHostID = dwAttackID;
					}

					break;
				}

				pTMAINTAIN->m_vTSKILLDATA.m_wID = pTSKILL->m_wSkillID;
				pTMAINTAIN->m_vTSKILLDATA.m_wAL = wAL;

				pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND = vAtkPos;
				pTMAINTAIN->m_vTSKILLDATA.m_bHIT = bTCR;

				pTMAINTAIN->m_vTSKILLDATA.m_dwMinAP = dwMinAP;
				pTMAINTAIN->m_vTSKILLDATA.m_dwMaxAP = dwMaxAP;
				pTMAINTAIN->m_vTSKILLDATA.m_dwMinMP = dwMinMP;
				pTMAINTAIN->m_vTSKILLDATA.m_dwMaxMP = dwMaxMP;

				pTMAINTAIN->m_vTSKILLDATA.m_bAtkLevel = bAtkLevel;
				pTMAINTAIN->m_vTSKILLDATA.m_bAglow = bAglow;
				pTMAINTAIN->m_vTSKILLDATA.m_bAtkCountryID = bAtkCountryID;
				pTMAINTAIN->m_vTSKILLDATA.m_bLevel = bLevel;

				if( pTDEFEND == m_pMainChar )
					pTGaugePannel->ResetHotkeyUI();

				pTMAINTAIN->m_bLevel = bLevel;
				pTMAINTAIN->m_dwTick = dwTick;

				pTMAINTAIN->m_bUnlimitedType = pTSKILL->m_dwDuration ? FALSE : TRUE;
				pTMAINTAIN->m_dwEndTick = pTMAINTAIN->m_dwTick + m_vKEY.m_dwTick;
			}

			if(bSkillAct)
			{
				pTDEFEND->PlayTActionSFX(
					&pTDEFEND->m_vTGARBAGESFX,
					pTSKILL->m_dwSFX[TSKILLSFX_DEFENDACT]);

				pTDEFEND->DoSkillFunc(
					pTSKILL,
					wAL,
					bLevel,
					&vAtkPos,
					&vDefPos,
					bMaintain,
					this);

				if(CheckSkillHasFunc( pTSKILL, SDT_STATUS, SDT_STATUS_MOVESPEED))
					pTDEFEND->AdjustSDT_STATUS_MOVESPEED();
			}
			BYTE bActionID = pTSKILL->m_bActionID[TSKILLACTION_DEFEND];

			if( bHit != HT_MISS )
			{
				if((pTDEFEND->m_bType != OT_RECALL || bActionID != TA_DEFEND) &&
					bActionID < TA_COUNT && bCount &&
					CTChart::GetTACTION(bActionID)->m_bLevel >= pTDEFEND->m_bACTLevel )
				{
					if( pTDEFEND->CanUseSubANI() && CTChart::GetTACTION(bActionID)->m_bSubAction )
					{
						if(!pTDEFEND->m_dwTSUBACT)
							pTDEFEND->SetSubAction( dwActID, dwAniID);
					}
					else if( pTDEFEND->m_bType != OT_PC || !((CTClientChar *) pTDEFEND)->GetRidingPet() )
					{
						pTDEFEND->m_bACTLevel = CTChart::GetTACTION(bActionID)->m_bLevel;
						pTDEFEND->SetAction( 0, 0);

						pTDEFEND->SetAction(
							dwActID,
							dwAniID);
					}
				}

				pTDEFEND->PlayTActionSFX(
					&pTDEFEND->m_vTGARBAGESFX,
					pTSKILL->m_dwSFX[TSKILLSFX_DEFEND]);
			}

			if( wBackSkillID && pTDEFEND == m_pMainChar && pTATTACK )
			{
				LPTSKILL pTBACKSKILL = CTChart::FindTSKILLTEMP(wBackSkillID);
				BYTE bBackSkillItemType = m_pMainChar->GetSkillItemType(pTBACKSKILL);

				if(CheckBackSkill( pTBACKSKILL, bBackSkillItemType))
				{
					VTSKILLTARGET vVTSKILLTARGET;

					TSKILLTARGET vTSKILLTARGET;
					TSKILLDATA vTSKILLDATA;

					vTSKILLTARGET.m_bType = pTATTACK->m_bType;
					vTSKILLTARGET.m_dwID = pTATTACK->m_dwID;
					vVTSKILLTARGET.clear();
					vVTSKILLTARGET.push_back(&vTSKILLTARGET);

					vTSKILLDATA.m_wID = wBackSkillID;
					vTSKILLDATA.m_bLevel = 1;
					vTSKILLDATA.m_bAtkCountryID = m_pMainChar->m_vTSKILLDATA.m_bAtkCountryID;
					vTSKILLDATA.m_vTGROUND = pTATTACK->GetPosition();
					vTSKILLDATA.m_wAL = wAL;
					vTSKILLDATA.m_bHIT = bTCR;
					vTSKILLDATA.m_dwMinAP = dwMinAP;
					vTSKILLDATA.m_dwMaxAP = dwMaxAP;
					vTSKILLDATA.m_dwMinMP = dwMinMP;
					vTSKILLDATA.m_dwMaxMP = dwMaxMP;
					vTSKILLDATA.m_bAtkLevel = bAtkLevel;
					vTSKILLDATA.m_bAglow = bAglow;

					if( pTSKILL->m_bRangeType == TSKILLRANGE_LINE )
						LineSHOT( m_pMainChar, &vTSKILLDATA, &vTSKILLDATA.m_vTGROUND, pTBACKSKILL, 0, TRUE);
					else if( pTSKILL->m_fAtkRange > 0.0f )
						RangeSHOT( m_pMainChar, &vTSKILLDATA, &vTSKILLDATA.m_vTGROUND, pTBACKSKILL, 0, TRUE);
					else
						ShotSkill( pTBACKSKILL, m_pMainChar, &vTSKILLDATA, &vVTSKILLTARGET);
				}
			}
		}
	}

	if(IsHostingOBJ( dwTargetID, bTargetType))
		ResetSkillICON();

	if( pTATTACK == m_pMainChar ||
		pTDEFEND == m_pMainChar )
		pTGaugePannel->ResetHotkeyUI();

	return TERR_NONE;
}

int CTClientGame::OnCS_LEVEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;

	(*pPacket)
		>> dwCharID;

	CTClientChar *pCHAR = FindPC(dwCharID);
	if(pCHAR)
	{
		BYTE bLevel = pCHAR->m_bLevel;

		CString strTYPE;
		CString strMSG;

		(*pPacket)
			>> pCHAR->m_bLevel;

		if( pCHAR == m_pMainChar )
		{
			strMSG = CTChart::Format( TSTR_FMT_LEVELUPM, pCHAR->m_bLevel);
			ResetPlayerINFO(FALSE);
		}
		else
		{
			if( pCHAR == m_pTARGET )
				ResetTargetINFO(FALSE);
			strMSG = CTChart::Format( TSTR_FMT_LEVELUP, pCHAR->GetName(), pCHAR->m_bLevel);
		}

		if( bLevel < pCHAR->m_bLevel )
		{
			CheckQuestOBJ(); // 레벨업 했을때 할 수 있는 퀘스트 정보를 다시 받아본다.

			strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
			m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, TCOLOR_LEVELUP, TCHAT_FLAG_INFO);

			pCHAR->PlayTActionSFX(
				&pCHAR->m_vTGARBAGESFX,
				TLEVELUP_SFX);

			pCHAR->PlaySND(
				ID_SND_LEVELUP,
				MEDIA_TSOUND,
				TRUE);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_HPMP_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;
	BYTE bType;

	DWORD dwMaxHP;
	DWORD dwMaxMP;
	DWORD dwHP;
	DWORD dwMP;

	(*pPacket)
		>> dwID
		>> bType
		>> dwMaxHP
		>> dwHP
		>> dwMaxMP
		>> dwMP;

	CTClientObjBase *pOBJ = FindOBJ(
		dwID,
		bType);

	if(pOBJ)
	{
		pOBJ->m_dwMaxHP = dwMaxHP;
		pOBJ->m_dwMaxMP = dwMaxMP;

		pOBJ->m_dwHP = dwHP;
		pOBJ->m_dwMP = dwMP;

		if( pOBJ == m_pMainChar )
		{
			WORD wSquadID = GetPartyID(m_pMainChar);

			if(wSquadID)
			{
				RelaySession()->SendCS_CORPSHP_REQ(
					wSquadID,
					m_pMainChar->m_dwID,
					m_pMainChar->m_dwMaxHP,
					m_pMainChar->m_dwHP);
			}
			ResetPlayerINFO(FALSE);
		}

		if( pOBJ == m_pTARGET )
			ResetTargetINFO(FALSE);

		if(IsHostingOBJ( dwID, bType))
			ResetRecallINFO(FALSE);

		if( m_bTournamentWatching )
			static_cast<CTTournamentGage*>(m_vTFRAME[TFRAME_TOURNAMENT_GAGE])->ResetTournamentINFO();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DIE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	DWORD dwID;
	BYTE bType;

	(*pPacket)
		>> dwID
		>> bType;

	CTClientObjBase *pDIE = FindOBJ(
		dwID,
		bType);

	if(pDIE)
	{
		pDIE->m_dwHP = 0;

		pDIE->OnDIE( m_pDevice, m_pRES);

		if( pDIE->m_bType == OT_PC &&
			pDIE->m_bTournamentTeam != TTOURNAMENT_TEAM_NONE)
		{
			if( m_pTLeftTeam && m_pTRightTeam )
			{
				if( m_pTLeftTeam->m_bSlotID == pDIE->m_bTournamentTeam )
					m_pTRightTeam->m_wKillCount += 1;
				else if( m_pTRightTeam->m_bSlotID == pDIE->m_bTournamentTeam )
					m_pTLeftTeam->m_wKillCount += 1;

				SetTournamentInfo( m_pTLeftTeam, m_pTRightTeam, FALSE );
			}
		}
	}

	if( pDIE == m_pMainChar )
	{
		if(m_vDRAG.m_pIMAGE)
		{
			delete m_vDRAG.m_pIMAGE;
			m_vDRAG.m_pIMAGE = NULL;

			m_vDRAG.m_bFrameID = TFRAME_COUNT;
			m_vDRAG.m_bSlotID = 0;
		}

		CloseAllUI();

		m_bTELEPORT = FALSE;
		m_bLButtonDown = FALSE;
		m_bRButtonDown = FALSE;
		m_bGroundLOCK = FALSE;
		m_bAutoRun = FALSE;
		m_bRevival = FALSE;

		m_dwLButtonTick = 0;
		m_dwRButtonTick = 0;

		ResetTargetOBJ(NULL);
		ResetTargetINFO(FALSE);

		m_vKEY.ResetSTATE();
		CancelCharge(TRUE);
		DisableUI(TFRAME_WORLDMAP);

		if( m_bTournamentBattle || m_bTournamentWatching )
		{
			m_bDoAutoRevival = TRUE;
			m_dwLeftTickAR = 5000;
		}
		else
		{
			BYTE bInven, bSlot;
			if( FindInven(IT_USE, IK_REVIVAL, bInven, bSlot) )
			{
				if( CTNationOption::WAITING_REVIVAL_ITEM )
				{
					if( m_dwWaitingRevivalItem == 0 ) // 부활 주문서 재사용시간이 0이면 바로 사용
					{
						m_pMainWnd->MessageBoxYesNo(
							TSTR_USEITEM_REVIVAL,
							TSTR_YES,
							TSTR_NO,
							GM_USEITE_REVIVAL,
							GM_GOTO_THUMB,
							0,
							TRUE,
							ID_SND_INFO,
							MEDIA_TSOUND,
							TMSGBOXOPEN_CLOSEALL_ALREADY);
					}
					else
					{
						m_bShowingRevivalItemMessageBox = TRUE;
						m_pMainWnd->MessageBoxOK(
							CTChart::Format(TSTR_GOTO_THUMB_MSG_SEC, m_dwWaitingRevivalItem),
							TSTR_OK,
							GM_GOTO_THUMB,
							0,
							TRUE,
							ID_SND_INFO,
							MEDIA_TSOUND,
							TMSGBOXOPEN_CLOSEALL_ALREADY);
					}
				}
				else
				{
					m_pMainWnd->MessageBoxYesNo(
						TSTR_USEITEM_REVIVAL,
						TSTR_YES,
						TSTR_NO,
						GM_USEITE_REVIVAL,
						GM_GOTO_THUMB,
						0,
						TRUE,
						ID_SND_INFO,
						MEDIA_TSOUND,
						TMSGBOXOPEN_CLOSEALL_ALREADY);
				}
			}
			else
			{
				m_pMainWnd->MessageBoxOK(
					TSTR_GOTO_THUMB_MSG,
					TSTR_OK,
					GM_GOTO_THUMB,
					0,
					TRUE,
					ID_SND_INFO,
					MEDIA_TSOUND,
					TMSGBOXOPEN_CLOSEALL_ALREADY);
			}
		}

		EnableChat(FALSE);
	}
	else if(pDIE)
	{
		if( pDIE == m_pTARGET )
		{
			pTGaugePannel->ResetHotkeyUI();
			LoopSkillOFF();
		}

		switch(bType)
		{
		case OT_PC	:
			{
				CString strTYPE;
				CString strMSG;

				strMSG = CTChart::Format( TSTR_FMT_DIE, pDIE->GetName());
				strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);

				m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG,	TCOLOR_DEAD, TCHAT_FLAG_INFO);

				/*m_pTRSCS->OnDieChar(
					m_pMainChar,
					dwID );*/
			}

			break;

		case OT_MON	:
			{
				CTClientMonster* pMONSTER = (CTClientMonster*) pDIE;
				if( pMONSTER->m_bFollowType != OT_NONE )
				{
					CTClientObjBase* pFOLLOW = FindOBJ( pMONSTER->m_dwFollowID, pMONSTER->m_bFollowType );
					if( pFOLLOW )
						pFOLLOW->LeaveAdjacency( pMONSTER );
				}

				MAPMONSTER::iterator finder = m_mapSLAVEMON.find(dwID);
				if( finder != m_mapSLAVEMON.end() )
					m_mapSLAVEMON.erase(finder);
			}

			break;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_EXP_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwOldEXP = m_pMainChar->GetEXP();
	DWORD dwNewEXP = 0;
	(*pPacket)
		>> dwNewEXP
		>> m_pMainChar->m_dwPrevEXP
		>> m_pMainChar->m_dwNextEXP;

	m_pMainChar->SetEXP( dwNewEXP );
	ResetPlayerINFO(FALSE);

	if( dwOldEXP < m_pMainChar->GetEXP() )
	{
		CString strTYPE;
		CString strMSG;

		strMSG = CTChart::Format( TSTR_FMT_EXP, m_pMainChar->GetEXP() - dwOldEXP);
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);

		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, TCOLOR_EXP, TCHAT_FLAG_INFO);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_RESETCOOLTIME_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	MAPTSKILL::iterator itTSKILL;
	WORD wExceptSkill;

	(*pPacket)
		>> wExceptSkill;

	for( itTSKILL = m_pMainChar->m_mapTSKILL.begin(); itTSKILL != m_pMainChar->m_mapTSKILL.end(); itTSKILL++)
	{
		CTClientSkill *pTSKILL = (*itTSKILL).second;

		if( pTSKILL->m_pTSKILL->m_wSkillID != wExceptSkill &&
			pTSKILL->m_dwTick && pTSKILL->m_bTimerON )
		{
			pTSKILL->m_dwExceptTick = 0;
			pTSKILL->m_dwTick = 0;
			pTSKILL->m_bTimerON = FALSE;

			if(m_pMainChar->IsReadyForUSE(pTSKILL))
				LoopTSKill(pTSKILL);
		}
	}
	CTClientItem::ClearTick();

	return TERR_NONE;
}

int CTClientGame::OnCS_MONEY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwRune = m_pMainChar->m_dwRune;
	DWORD dwLuna = m_pMainChar->m_dwLuna;
	DWORD dwCron = m_pMainChar->m_dwCron;

	(*pPacket)
		>> m_pMainChar->m_dwRune
		>> m_pMainChar->m_dwLuna
		>> m_pMainChar->m_dwCron;
	ResetInvenUI();

	__int64 nMoneyNext = m_pMainChar->m_dwRune * TRUNE_DIVIDER + m_pMainChar->m_dwLuna * TLUNA_DIVIDER + m_pMainChar->m_dwCron;
	__int64 nMoneyPrev = dwRune * TRUNE_DIVIDER + dwLuna * TLUNA_DIVIDER + dwCron;

	if( nMoneyPrev < nMoneyNext )
	{
		CString strTYPE;
		CString strMSG;

		nMoneyNext -= nMoneyPrev;
		strMSG = MakeMoneyStr(nMoneyNext);

		strTYPE = CTChart::LoadString( TSTR_GETMONEY);
		strMSG += strTYPE;
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);

		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, TCOLOR_MONEY, TCHAT_FLAG_INFO);
		m_pMainChar->PlaySND( ID_SND_GOLD, MEDIA_TSOUND, FALSE);

		ResetPlayerINFO(FALSE);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_UPDATEITEM_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bInvenID;

	(*pPacket)
		>> bInvenID;

	MAPTINVEN::iterator itTINVEN = m_pMainChar->m_mapTINVEN.find(bInvenID);
	if( itTINVEN == m_pMainChar->m_mapTINVEN.end() )
		return TERR_NONE;

	CTClientItem *pTITEM = new CTClientItem();

	pTITEM->SetItemSlot(pPacket);
	pTITEM->SetItemData(pPacket);
	pTITEM->SetItemMagic(pPacket);

#ifdef ADD_MISSION
	if( bInvenID == INVEN_EQUIP )
		pTITEM->SetInstGradeMax(m_bItemLevelRevision);
#endif

	if( !pTITEM->CheckValid() )
	{
		delete pTITEM;
		return TERR_NONE;
	}

	MAPTITEM::iterator itTITEM = (*itTINVEN).second->m_mapTITEM.find(pTITEM->GetItemSlot());
	if( itTITEM != (*itTINVEN).second->m_mapTITEM.end() )
	{
		delete (*itTITEM).second;
		(*itTINVEN).second->m_mapTITEM.erase(itTITEM);
	}

	(*itTINVEN).second->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
	
	if( bInvenID == INVEN_EQUIP && pTITEM->GetItemSlot() == ES_COSTUME_RACE )
	{
		m_pMainChar->DoResetObj( m_pDevice, m_pRES );
		m_pCamera->m_fZoomMIN = m_pMainChar->GetAttrFLOAT(ID_CAM_ZOOMMIN);
		m_pCamera->m_fZoomMAX = m_pMainChar->GetAttrFLOAT(ID_CAM_ZOOMMAX);
		m_pCamera->m_fCamHEIGHT = m_pMainChar->GetAttrFLOAT(ID_CAM_HEIGHT);
		m_pCamera->ZoomCamera(1,0);
	}

	m_pDurationUI->Update();
	((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();
	ResetInvenUI();

	return TERR_NONE;
}

int CTClientGame::OnCS_MOVEITEM_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	ResetCharInfoUI();
	ResetInvenUI();
	
	CancelTrade();
	
	BYTE bVisibleItemSlotDlg = TFRAME_COUNT;
	if( m_vTFRAME[TFRAME_ITEM_UP]->IsVisible() )
		bVisibleItemSlotDlg = TFRAME_ITEM_UP;
	else if( m_vTFRAME[TFRAME_ITEM_REPAIR]->IsVisible() )
		bVisibleItemSlotDlg = TFRAME_ITEM_REPAIR;
	else if( m_vTFRAME[TFRAME_ITEM_REFINE]->IsVisible() )
		bVisibleItemSlotDlg = TFRAME_ITEM_REFINE;
	else if( m_vTFRAME[TFRAME_ITEM_GAMBLE]->IsVisible() )
		bVisibleItemSlotDlg = TFRAME_ITEM_GAMBLE;
	else if( m_vTFRAME[TFRAME_ITEM_EXTPERIOD]->IsVisible() )
		bVisibleItemSlotDlg = TFRAME_ITEM_EXTPERIOD;

	if(bResult)
	{
		ItemMSG(bResult);
	}
	else if( bVisibleItemSlotDlg != TFRAME_COUNT )
	{
		CTItemSlotBaseDlg* pDlg = static_cast<CTItemSlotBaseDlg*>( m_vTFRAME[bVisibleItemSlotDlg] );
		for( BYTE i=0; i<pDlg->GetSlotCount(); ++i )
		{
			if( pDlg->GetItem(i) && 
				pDlg->GetLastInvenID(i) == m_bInvenID &&
				pDlg->GetLastInvenSlot(i) == m_bSlotID )
			{
				if( m_bTargetInvenID != INVEN_NULL )
				{
					CTClientInven* pTInven = m_pMainChar->FindTInven(m_bTargetInvenID);
					if( pTInven )
					{
						CTClientItem* pTItem = pTInven->FindTItem(m_bTargetSlotID);
						if( pTItem ) 
							pDlg->SetItem(i, pTItem->GetTITEM(), m_bTargetInvenID,m_bTargetSlotID);
					}
				}
				else
				{
					pDlg->SetItem(i, NULL);
				}

				break;
			}
		}
	}
	else if( m_vTFRAME[TFRAME_ITEM_REGBOX]->IsVisible() )
	{
		m_vTFRAME[TFRAME_ITEM_REGBOX]->ShowComponent(FALSE);
	}
	else if( m_vTFRAME[TFRAME_PRIVATE_SELL]->IsVisible() )
	{
		CTPrivateShopDlg* pDlg = static_cast<CTPrivateShopDlg*>( m_vTFRAME[TFRAME_PRIVATE_SELL] );

		if( m_bTargetInvenID != INVEN_NULL )
		{
			pDlg->ChangeItemInvenPos(m_bInvenID, m_bSlotID, m_bTargetInvenID, m_bTargetSlotID);

		}
		else
		{
			INT nPos = pDlg->GetItemIdxByInvenPos(m_bInvenID,m_bSlotID);
			if( nPos != T_INVALID )
				pDlg->RemoveItem(nPos);
		}
	}
	else if( m_vTFRAME[TFRAME_MAIL_SEND]->IsVisible() )
	{
		CTMailSendDlg* pDlg = static_cast<CTMailSendDlg*>( m_vTFRAME[TFRAME_MAIL_SEND] );
		BYTE nInvenID = pDlg->GetInvenID(); 
		BYTE nSlot = pDlg->GetInvenSlot();
		if( nInvenID != INVEN_NULL && nInvenID == m_bInvenID && nSlot == m_bSlotID )
		{
			if( m_bTargetInvenID != INVEN_NULL )
				pDlg->ChangeItemPos( m_bTargetInvenID,m_bTargetSlotID );
			else
				pDlg->SetItem(NULL);
		}
	}
	else if( m_vTFRAME[TFRAME_PET_MAKE]->IsVisible() )
	{
		CTPetMakeDlg* pDlg = static_cast<CTPetMakeDlg*>( m_vTFRAME[TFRAME_PET_MAKE] );
		if( m_bInvenID == pDlg->GetInvenID() && m_bSlotID == pDlg->GetInvenSlot() )
		{
			if( m_bTargetInvenID == INVEN_NULL )
				DisableUI(TFRAME_PET_MAKE);
			else
				pDlg->SetPetItem( m_bTargetInvenID,m_bTargetSlotID );
		}
	}
	else if( m_vTFRAME[TFRAME_SMS]->IsVisible() )
	{
		CTSMSDlg* pDlg = static_cast<CTSMSDlg*>( m_vTFRAME[TFRAME_SMS] );
		if( m_bInvenID == pDlg->GetInvenID() && m_bSlotID == pDlg->GetInvenSlot() )
		{
			if( m_bTargetInvenID == INVEN_NULL )
				DisableUI(TFRAME_SMS);
			else	
				pDlg->SetItem( m_bTargetInvenID,m_bTargetSlotID ); 
		}
	}
	else if( m_vTFRAME[TFRAME_SKILL]->IsVisible() )
	{
		CTSkillDlg* pDlg = static_cast<CTSkillDlg*>( m_vTFRAME[TFRAME_SKILL] );
		if( m_bInvenID == pDlg->m_bSiInvenID && m_bSlotID == pDlg->m_bSiSlotID )
		{
			if( m_bTargetInvenID == INVEN_NULL )
				DisableUI(TFRAME_SKILL);
			else
			{
				pDlg->m_bSiInvenID = m_bTargetInvenID;
				pDlg->m_bSiSlotID = m_bTargetSlotID; 
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_ADDITEM_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bInvenID;

	(*pPacket)
		>> bInvenID;

	MAPTINVEN::iterator itTINVEN = m_pMainChar->m_mapTINVEN.find(bInvenID);
	if( itTINVEN == m_pMainChar->m_mapTINVEN.end() )
		return TERR_NONE;

	CTClientItem *pTITEM = new CTClientItem();

	pTITEM->SetItemSlot(pPacket);
	pTITEM->SetItemData(pPacket);
	pTITEM->SetItemMagic(pPacket);

#ifdef ADD_MISSION
	if( bInvenID == INVEN_EQUIP )
		pTITEM->SetInstGradeMax(m_bItemLevelRevision);
#endif

	if( !pTITEM->CheckValid() )
	{
		delete pTITEM;
		return TERR_NONE;
	}

	MAPTITEM::iterator itTITEM = (*itTINVEN).second->m_mapTITEM.find(pTITEM->GetItemSlot());
	if( itTITEM != (*itTINVEN).second->m_mapTITEM.end() )
	{
		delete (*itTITEM).second;
		(*itTINVEN).second->m_mapTITEM.erase(itTITEM);
	}

	(*itTINVEN).second->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
	
	if( bInvenID == INVEN_EQUIP && pTITEM->GetItemSlot() == ES_COSTUME_RACE )
	{
		m_pMainChar->DoResetObj( m_pDevice, m_pRES );
		m_pCamera->m_fZoomMIN = m_pMainChar->GetAttrFLOAT(ID_CAM_ZOOMMIN);
		m_pCamera->m_fZoomMAX = m_pMainChar->GetAttrFLOAT(ID_CAM_ZOOMMAX);
		m_pCamera->m_fCamHEIGHT = m_pMainChar->GetAttrFLOAT(ID_CAM_HEIGHT);
		m_pCamera->ZoomCamera(1,0);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DELITEM_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bInvenID;
	BYTE bItemID;

	(*pPacket)
		>> bInvenID
		>> bItemID;

	MAPTINVEN::iterator itTINVEN = m_pMainChar->m_mapTINVEN.find(bInvenID);
	if( itTINVEN == m_pMainChar->m_mapTINVEN.end() )
		return TERR_NONE;

	BYTE bItemSlot = 0xFF;
	MAPTITEM::iterator itTITEM = (*itTINVEN).second->m_mapTITEM.find(bItemID);
	if( itTITEM != (*itTINVEN).second->m_mapTITEM.end() )
	{
		bItemSlot = (*itTITEM).second->GetItemSlot();
		delete (*itTITEM).second;
		(*itTINVEN).second->m_mapTITEM.erase(itTITEM);
	}

	if( bInvenID == INVEN_EQUIP && bItemSlot == ES_COSTUME_RACE )
	{
		m_pMainChar->DoResetObj( m_pDevice, m_pRES );
		m_pCamera->m_fZoomMIN = m_pMainChar->GetAttrFLOAT(ID_CAM_ZOOMMIN);
		m_pCamera->m_fZoomMAX = m_pMainChar->GetAttrFLOAT(ID_CAM_ZOOMMAX);
		m_pCamera->m_fCamHEIGHT = m_pMainChar->GetAttrFLOAT(ID_CAM_HEIGHT);
		m_pCamera->ZoomCamera(1,0);
	}
    
	((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();
	ResetInvenUI();

	// 08.11.25 권성혁
	CTCraftQuestDlg* pTCraft = (CTCraftQuestDlg*)m_vTFRAME[TFRAME_CRAFT];

	if( pTCraft->IsVisible() )
		pTCraft->ResetTerm();

	return TERR_NONE;
}

int CTClientGame::OnCS_ITEMUSE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwTick;
	BYTE bResult;
	WORD wGroupDelayID;
	BYTE bKind;

	(*pPacket)
		>> bResult
		>> wGroupDelayID
		>> bKind
		>> dwTick;

	switch(bResult)
	{
	case IU_SUCCESS			:
		{
			switch(bKind)
			{
			case IK_MAXHP	:
			case IK_HP		:
				m_pMainChar->PlayTActionSFX(
					&m_pMainChar->m_vTGARBAGESFX,
					THPITEM_SFX);

				break;

			case IK_MAXMP	:
			case IK_MP		:
				m_pMainChar->PlayTActionSFX(
					&m_pMainChar->m_vTGARBAGESFX,
					TMPITEM_SFX);

				break;
			}

			CTClientItem::SetTick( wGroupDelayID, dwTick);
		}
		break;

	case IU_OVERLAPPREMIUM	:
		{
			CString strMSG;
			strMSG = CTChart::LoadString( TSTR_IU_ERR_OVERLAPPREMIUM);
			ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_ERROR);
		}
		break;

	case IU_OVERLAPEXPBONUS:
		{
			CString strMSG;
			strMSG = CTChart::LoadString( TSTR_IU_OVERLAPEXPBONUS);
			ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_ERROR);
		}
		break;

	case IU_WRAPPING:
		{
			CString strMSG;
			strMSG = CTChart::LoadString( TSTR_CANT_USE_WRAPED_ITEM );
			ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_ERROR);
		}
		break;

	case IU_NOTFOUND		:
	case IU_NEEDTIME		: break;
	}

	CTClientItem::Unlock(wGroupDelayID);
	ResetCharInfoUI();
	((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();
	ResetInvenUI();
	CancelTrade();

	return TERR_NONE;
}

int CTClientGame::OnCS_EQUIP_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	BYTE bItemCount;

	(*pPacket)
		>> dwCharID
		>> bItemCount;

	CTClientChar *pTCHAR = FindPC(dwCharID);
	if(!pTCHAR)
		return TERR_NONE;

	if( pTCHAR != m_pMainChar )
	{
		BYTE bOrgRace = pTCHAR->GetRaceID();

		MAPTINVEN::iterator itTINVEN = pTCHAR->m_mapTINVEN.find(INVEN_EQUIP);
		CTClientInven *pTEQUIP = NULL;

		if( itTINVEN != pTCHAR->m_mapTINVEN.end() )
			pTEQUIP = (*itTINVEN).second;

		if(!pTEQUIP)
			return TERR_NONE;
		pTEQUIP->ClearInven();

		for( BYTE i=0; i<bItemCount; i++)
		{
			CTClientItem *pTITEM = new CTClientItem();
			
			pTITEM->SetItemSlot(pPacket);
			pTITEM->SetItemData(pPacket);
			pTITEM->SetItemMagic(pPacket);

			if( !pTITEM->CheckValid() )
			{
				delete pTITEM;
				continue;
			}

			pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
		}

		if( bOrgRace != pTCHAR->GetRaceID() )
			pTCHAR->DoResetObj( m_pDevice, m_pRES );
	}

	pTCHAR->ResetEQUIP(
		m_pDevice,
		m_pRES);

	if( pTCHAR == m_pMainChar )
	{
		ResetCharInfoUI();
		m_pDurationUI->Update();
		CTCharInfoDlg *pTCharInfoDlg = (CTCharInfoDlg *) m_vTFRAME[TFRAME_CHAR_INFO];
		pTCharInfoDlg->ResetStatColor( pTCHAR );

		if( m_vTFACEIMG[TFACEIMG_PLAYER] )
			m_vTFACEIMG[TFACEIMG_PLAYER]->SetCurImage(GetFACE(
				m_pMainChar->GetRaceID(),
				m_pMainChar->m_bSex,
				m_pMainChar->m_bFace,
				m_pMainChar->m_bHair));
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CORPSASK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strMSG;

	(*pPacket)
		>> m_strReqNAME;

	if( IsBusy() )
		m_pSESSION->SendCS_CORPSREPLY_REQ(m_strReqNAME, ASK_BUSY);
	else
	{
		strMSG = CTChart::Format( TSTR_FMT_CORPS_ASK, m_strReqNAME);
		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_CORPS_OK,
			GM_CORPS_NO,
			GM_CORPS_BUSY,
			FALSE );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CORPSREPLY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strNAME;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> strNAME;

	CString strMSG;
	switch(bResult)
	{
	case CORPS_DENY		:
		strMSG = CTChart::Format( TSTR_FMT_CORPS_DENY, strNAME);
		break;

	case CORPS_BUSY		:
		strMSG = CTChart::Format( TSTR_ERR_TARGET_BUSY, strNAME);
		break;

	default				: 
		CorpsMSG( m_pMainChar, bResult); 
		break;
	}

	if( !strMSG.IsEmpty() )
	{
		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_ITEMUPGRADE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	CString strJACKPOT;
	CString strCOUNTRY;
	CString strITEM;
	CString strMSG;
	
	CTItemUpDlg* pDlg = static_cast<CTItemUpDlg*>( m_vTFRAME[TFRAME_ITEM_UP] );

	DWORD dwSoundID;
	BYTE bSoundType;

	BYTE bTargetInven;
	BYTE bTargetItemID;
	BYTE bGrade;
	BYTE bGradeEffect;
	WORD wColor;

	(*pPacket)
		>> bTargetInven
		>> bTargetItemID
		>> bGrade
		>> bGradeEffect
		>> wColor;

	if( bResult == ITEMUPGRADE_SUCCESS )
	{
		pDlg->SetItem(CTItemUpDlg::SCROLL_SLOT, NULL);

		CTClientItem* pItem = pDlg->GetLastClientItem(CTItemUpDlg::TARGET_SLOT);

		if( pItem )
		{
			pItem->SetGrade(bGrade);
			pItem->SetGradeEffect(bGradeEffect);

			pDlg->SetItem( CTItemUpDlg::TARGET_SLOT, pItem->GetTITEM(), bTargetInven, bTargetItemID );
		}

		strMSG = CTChart::LoadString( TSTR_ITEMUP_SUCCESS);
		if( bGrade > 4 )
		{
			CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
			CTTextLinkData* pData;

			strCOUNTRY = CTChart::LoadString( (TSTRING) m_vTCOUNTRYSTR[m_pMainChar->m_bContryID]);
			strJACKPOT = CTChart::Format( TSTR_FMT_UPGRADE_SUCCESS,
				strCOUNTRY,
				m_pMainChar->GetName(),
				bGrade);

			DWORD dwID = pTextLinker->NewTextLinkData(&pData);
			pData->PackItem( pItem->GetTITEM(), pItem);

			VECTORDWORD vItemID;
			vItemID.push_back(dwID);

			strJACKPOT += CString(_T("%s"));
			strITEM = pTextLinker->MakeItemNetText( strJACKPOT, vItemID);
			vItemID.clear();

			RelaySession()->SendCS_CHAT_REQ(
				m_pMainChar->m_strNAME,
				CHAT_SHOW,
				strITEM.GetLength(), NULL,
				strITEM,
				m_pMainChar->m_bContryID,
				m_mapPLAYER);
		}

		dwSoundID = ID_SND_ITEM_SUCCESS;
		bSoundType = MEDIA_TSOUND;
		
		pDlg->ShowComponent(TRUE);
		ResetInvenUI();
	}
	else if( bResult == ITEMUPGRADE_SUCCESS_WRAP )
	{
		CTClientItem* pItem = pDlg->GetLastClientItem(CTItemUpDlg::TARGET_SLOT);
		if( pItem )
			pItem->SetWrap( bGrade );

		pDlg->SetItem(CTItemUpDlg::SCROLL_SLOT, NULL);
		pDlg->SetItem(CTItemUpDlg::TARGET_SLOT, NULL);

		dwSoundID = ID_SND_ITEM_SUCCESS;
		bSoundType = MEDIA_TSOUND;

		strMSG = CTChart::LoadString( TSTR_ITEMUP_WRAP_SUCCESS );

		pDlg->ShowComponent(TRUE);
		ResetInvenUI();
	}
	else if( bResult == ITEMUPGRADE_SUCCESS_ELD )
	{
		pDlg->SetItem(CTItemUpDlg::SCROLL_SLOT, NULL);

		CTClientItem* pItem = pDlg->GetLastClientItem(CTItemUpDlg::TARGET_SLOT);
		if( pItem )
			pItem->SetELD( bGrade );

		dwSoundID = ID_SND_ITEM_SUCCESS;
		bSoundType = MEDIA_TSOUND;

		strMSG = CTChart::LoadString( TSTR_ITEMUP_ELD_SUCCESS );

		pDlg->ShowComponent(TRUE);
		ResetInvenUI();
	}
	else if( bResult == ITEMUPGRADE_SUCCESS_CLEARREFINE )
	{
		pDlg->SetItem(CTItemUpDlg::SCROLL_SLOT, NULL);

		dwSoundID = ID_SND_ITEM_SUCCESS;
		bSoundType = MEDIA_TSOUND;

		strMSG = CTChart::LoadString( TSTR_ITEMUP_SUCCESS_CLEARREFINE );

		pDlg->ShowComponent(TRUE);
		ResetInvenUI();
	}
	else if( bResult == ITEMUPGRADE_SUCCESS_CHANGEEFFECT )
	{
		CTClientItem* pItem = pDlg->GetLastClientItem(CTItemUpDlg::TARGET_SLOT);
		if( pItem )
			pItem->SetGradeEffect( bGradeEffect );

		pDlg->SetItem(CTItemUpDlg::SCROLL_SLOT, NULL);
		pDlg->SetItem(CTItemUpDlg::TARGET_SLOT, NULL);

		dwSoundID = ID_SND_ITEM_SUCCESS;
		bSoundType = MEDIA_TSOUND;

		strMSG = CTChart::LoadString( TSTR_ITEMUP_SUCCESS_CHANGEEFFECT );

		pDlg->ShowComponent(TRUE);
		ResetInvenUI();
	}
	else if( bResult == ITEMUPGRADE_SUCCESS_COLOR )
	{
		CTClientItem* pItem = pDlg->GetLastClientItem(CTItemUpDlg::TARGET_SLOT);
		if( pItem )
			pItem->SetColor( wColor );
		
		pDlg->SetItem(CTItemUpDlg::SCROLL_SLOT, NULL);
		pDlg->SetItem(CTItemUpDlg::TARGET_SLOT, NULL);

		dwSoundID = ID_SND_ITEM_SUCCESS;
		bSoundType = MEDIA_TSOUND;

		strMSG = CTChart::LoadString( TSTR_ITEMUP_SUCCESS_COLOR );
	}
	else
	{
		switch(bResult)
		{
		case ITEMUPGRADE_MONEY:
			strMSG = CTChart::LoadString( TSTR_ITEMUP_MONEYFAIL);
			break;

		case ITEMUPGRADE_DOWNGRADE:
			{
				strMSG = CTChart::LoadString( TSTR_ITEMUP_DOWNGRADE ); // 장비 강화에 실패했습니다! (해당 장비의 강화 등급이 하락되었을 가능성이 있습니다)

				CTClientItem* pItem = pDlg->GetLastClientItem(CTItemUpDlg::TARGET_SLOT);
				if( pItem )
				{
					pItem->SetGrade(bGrade);
					pItem->SetGradeEffect( bGradeEffect );
				}
			}
			break;

		case ITEMUPGREAD_NOGRADE:
			strMSG = CTChart::LoadString( TSTR_ITEMUP_ERR_CANNOTGRADE ); // 이 아이템은 강화할 수 없습니다.
			break;

		case ITEMUPGRADE_MAGICFAIL:
			strMSG = CTChart::LoadString( TSTR_ITEMUP_ERR_MAGICFAIL ); // 아이템 제작이 실패했습니다!
			break;

		case ITEMUPGRADE_WRAPPING:
			strMSG = CTChart::LoadString( TSTR_ALREADY_WRAP );
			break;

		case ITEMUPGRADE_MAXELD:
			strMSG = CTChart::LoadString( TSTR_MAXELD );
			break;

		case ITEMUPGRADE_NOREFINE:	//제련되어 있지 않음
			strMSG = CTChart::LoadString( TSTR_ITEMUP_ERR_NOREFINE );
			break;

		case ITEMUPGRADE_NOGRADEEFFECT:	//강화 이펙트 없음
			strMSG = CTChart::LoadString( TSTR_ITEMUP_ERR_UNDER17 );
			break;

		default:
			strMSG = CTChart::LoadString( TSTR_ITEMUP_FAIL); // 장비 강화에 실패하여 해당 장비가 소멸되었습니다!
			break;	
		}

		pDlg->ClearItem();

		dwSoundID = ID_SND_ITEM_FAILED;
		bSoundType = MEDIA_TSOUND;

		pDlg->ShowComponent(TRUE);
		ResetInvenUI();
	}

	m_pMainWnd->MessageBoxOK(
		strMSG,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		0,
		TRUE,
		dwSoundID,
		bSoundType );

	return TERR_NONE;
}

int CTClientGame::OnCS_ITEMMAGICGRADE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	(*pPacket) >> bResult;

	CTItemUpDlg* pDlg = static_cast<CTItemUpDlg*>( m_vTFRAME[TFRAME_ITEM_UP] );
	pDlg->SetItem(CTItemUpDlg::SCROLL_SLOT, NULL);
	
	CTClientItem* pItem = pDlg->GetLastClientItem(CTItemUpDlg::TARGET_SLOT);
	
	CString strMSG;
	DWORD dwSoundID;
	BYTE bSoundType;

	if( bResult == ITEMUPGRADE_SUCCESS ||
		bResult == ITEMUPGRADE_SUCCESS_MAGICCLEAR )
	{
		BYTE bTargetInven;
		BYTE bTargetItemID;
		
		(*pPacket)
			>> bTargetInven
			>> bTargetItemID;
		
		if( pItem )
			pItem->SetItemMagic(pPacket);

		if( bResult == ITEMUPGRADE_SUCCESS )
			strMSG = CTChart::LoadString( TSTR_ITEMUP_SUCCESS);
		else if( bResult == ITEMUPGRADE_SUCCESS_MAGICCLEAR )
			strMSG = CTChart::LoadString( TSTR_ITEMUP_SUCCESS_MAGICCLEAR);

		dwSoundID = ID_SND_ITEM_SUCCESS;
		bSoundType = MEDIA_TSOUND;
	}
	else
	{
		pDlg->ClearItem();
		strMSG = CTChart::LoadString( TSTR_ITEMUP_ERR_MAGICFAIL);

		dwSoundID = ID_SND_ITEM_FAILED;
		bSoundType = MEDIA_TSOUND;
	}

	pDlg->ShowComponent(TRUE);
	ResetInvenUI();

	m_pMainWnd->MessageBoxOK(
		strMSG,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		0,
		TRUE,
		dwSoundID,
		bSoundType );

	return TERR_NONE;
}

int CTClientGame::OnCS_GETITEM_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTClientItem item;
	
	item.SetItemData(pPacket);
	item.SetItemMagic(pPacket);

	if( !item.CheckValid() )
		return TERR_NONE;

	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
	CTTextLinkData* pData;

	DWORD dwID = pTextLinker->NewTextLinkData(&pData);
	pData->PackItem(item.GetTITEM(), &item);

	VECTORDWORD vItemID;
	vItemID.push_back(dwID);

	CString strCHATMSG;
	strCHATMSG = CTChart::LoadString( TSTR_FMT_ADDITEM);
	strCHATMSG = pTextLinker->MakeItemNetText(strCHATMSG, vItemID);

	CString strTYPE;
	strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
	m_pChatFrame->ChatMSG(
		strTYPE,
		m_pMainChar->GetName(),
		strCHATMSG,
		TCOLOR_INFO,
		TCHAT_FLAG_INFO,
		ID_SND_INFO,
		MEDIA_TSOUND,
		-1 );

	return TERR_NONE;
}

int CTClientGame::OnCS_HELMETHIDE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	BYTE bHide;

	(*pPacket)
		>> dwCharID
		>> bHide;

	CTClientChar* pCHAR = FindPC(dwCharID);
	if( pCHAR )
	{
		pCHAR->m_bHelmetHide = bHide;
		pCHAR->ResetEQUIP(m_pDevice, m_pRES);

		if( pCHAR == m_pMainChar )
			ResetCharInfoUI();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYJOINASK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strMSG;

	(*pPacket)
		>> m_strReqNAME
		>> m_bObtainType;

	if( IsBusy() )
	{
		m_pSESSION->SendCS_PARTYJOIN_REQ(
			m_strReqNAME,
			m_bObtainType,
			ASK_BUSY);
	}
	else
	{
		strMSG = CTChart::Format( TSTR_FMT_PARTY_REQ, m_strReqNAME);
		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_PARTY_OK,
			GM_PARTY_NO,
			GM_PARTY_BUSY,
			FALSE );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strReqNAME;
	CString strAckNAME;
	BYTE bResult;

	(*pPacket)
		>> strReqNAME
		>> strAckNAME
		>> bResult;

	if(bResult)
	{
		CString strMSG;

		switch(bResult)
		{
		case PARTY_NOTCHIEF		: strMSG = CTChart::LoadString( TSTR_NOT_PARTYCHIEF); break;
		case PARTY_NOPARTY		: strMSG = CTChart::LoadString( TSTR_NOPARTY); break;
		case PARTY_ALREADY		: strMSG = CTChart::Format( TSTR_FMT_ALREADY_PARTY, strAckNAME); break;
		case PARTY_WAITERS		: strMSG = CTChart::Format( TSTR_FMT_PARTY_WAITERS, strAckNAME); break;
		case PARTY_NOUSER		: strMSG = CTChart::Format( TSTR_FMT_PARTY_NOUSER, strAckNAME); break;
		case PARTY_NOREQUSER	: strMSG = CTChart::Format( TSTR_FMT_PARTY_NOUSER, strReqNAME); break;
		case PARTY_DENY			: strMSG = CTChart::Format( TSTR_FMT_PARTY_DENY, strAckNAME); break;
		case PARTY_BUSY			: strMSG = CTChart::Format( TSTR_ERR_TARGET_BUSY, strAckNAME); break;
		case PARTY_FULL			: strMSG = CTChart::LoadString( TSTR_PARTY_FULL); break;
		case PARTY_COUNTRY		: strMSG = CTChart::Format( TSTR_FMT_PARTY_COUNTRY, strAckNAME); break;
		}

		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE,
			ID_SND_ERROR);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYJOIN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	LPTPARTY pTPARTY = new TPARTY();

	(*pPacket)
		>> m_pMainChar->m_wPartyID
		>> pTPARTY->m_strNAME
		>> pTPARTY->m_dwCharID
		>> m_pMainChar->m_dwChiefID
		>> m_pMainChar->m_wCommanderID
		>> pTPARTY->m_strGUILD
		>> pTPARTY->m_bLevel
		>> pTPARTY->m_dwMaxHP
		>> pTPARTY->m_dwHP
		>> pTPARTY->m_dwMaxMP
		>> pTPARTY->m_dwMP
		>> pTPARTY->m_bRaceID
		>> pTPARTY->m_bSexID
		>> pTPARTY->m_bFaceID
		>> pTPARTY->m_bHairID
		>> m_bPartyItemType
		>> pTPARTY->m_bClass;

	m_vTPARTY.push_back(pTPARTY);
	ResetPartyINFO(FALSE);

	// 여기다 RSCS
	MAPMONSTER::iterator itMON;
	MAPPLAYER::iterator itPC;
	MAPRECALL::iterator itRECALL;

	ReportENEMY();
	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwMemberID;
	BYTE bKick;

	(*pPacket)
		>> dwMemberID
		>> m_pMainChar->m_dwChiefID
		>> m_pMainChar->m_wCommanderID
		>> m_pMainChar->m_wPartyID
		>> bKick;

	if( dwMemberID != m_pMainChar->m_dwID )
	{
		VTPARTY::iterator it = m_vTPARTY.begin();

		while(it != m_vTPARTY.end())
		{
			LPTPARTY pTPARTY = (*it);

			if( pTPARTY->m_dwCharID == dwMemberID )
			{
				if( pTPARTY == m_pTPARTY )
					ResetPartyOBJ(NULL);

				m_vTPARTY.erase(it);
				delete pTPARTY;
			}
			else
				it++;
		}
	}
	else
	{
		m_pMainChar->m_wCommanderID = 0;
		m_pMainChar->m_dwChiefID = 0;
		m_pMainChar->m_wPartyID = 0;

		ReleaseParty();
	}
	ResetPartyINFO(FALSE);

	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYATTR_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;

	(*pPacket)
		>> dwCharID;

	CTClientChar *pPC = FindPC(dwCharID);
	if(pPC)
	{
		(*pPacket)
			>> pPC->m_wPartyID
			>> pPC->m_dwChiefID
			>> pPC->m_wCommanderID;

		if( pPC == m_pMainChar )
			m_pTPARTYITEM->EnableComponent( m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID );

		ResetLeaderMark();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYMOVE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYMANSTAT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	BYTE bType;

	(*pPacket)
		>> dwCharID
		>> bType;

	for( BYTE i=0; i<INT(m_vTPARTY.size()); i++)
		if( dwCharID == m_vTPARTY[i]->m_dwCharID )
		{
			(*pPacket)
				>> m_vTPARTY[i]->m_bLevel
				>> m_vTPARTY[i]->m_dwMaxHP
				>> m_vTPARTY[i]->m_dwHP
				>> m_vTPARTY[i]->m_dwMaxMP
				>> m_vTPARTY[i]->m_dwMP;

			ResetPartyINFO(FALSE);
			break;
		}

	return TERR_NONE;
}

int CTClientGame::OnCS_CHGPARTYTYPE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	CString strCHATMSG;

	if( bResult == 0 )
	{
		(*pPacket)
			>> m_bPartyItemType;

		switch(m_bPartyItemType)
		{
		case PT_FREE	: strCHATMSG = CTChart::LoadString( TSTR_CMSG_CHGPARTYTYPE_FREE); break;
		case PT_HUNTER	: strCHATMSG = CTChart::LoadString( TSTR_CMSG_CHGPARTYTYPE_HUNTER); break;
		case PT_LOTTERY	: strCHATMSG = CTChart::LoadString( TSTR_CMSG_CHGPARTYTYPE_LOTTERY); break;
		case PT_CHIEF	: strCHATMSG = CTChart::LoadString( TSTR_CMSG_CHGPARTYTYPE_CHIEF); break;
		case PT_ORDER	: strCHATMSG = CTChart::LoadString( TSTR_CMSG_CHGPARTYTYPE_SEQ); break;
		}

		ResetPartyINFO(FALSE);
	}

	if( !strCHATMSG.IsEmpty() )
	{
		CString strTYPE;
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
		m_pChatFrame->ChatSysMSG(strTYPE, m_pMainChar->GetName(), strCHATMSG, 
			TCOLOR_INFO, TCHAT_FLAG_INFO, ID_SND_INFO, MEDIA_TSOUND);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYITEMTAKE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	(*pPacket)
		>> dwCharID;

	CString strChar;
	INT nPCnt = (INT) m_vTPARTY.size();
	for(INT i=0; i<nPCnt; ++i)
	{
		if(m_vTPARTY[i]->m_dwCharID == dwCharID)
		{
			strChar = m_vTPARTY[i]->m_strNAME;
			break;
		}
	}

	if( strChar.IsEmpty() )
		return TERR_NONE;

	CTClientItem item;
	item.SetItemData(pPacket);
	item.SetItemMagic(pPacket);

	if( !item.CheckValid() )
		return TERR_NONE;

	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
	CTTextLinkData* pData;

	DWORD dwID = pTextLinker->NewTextLinkData(&pData);
	pData->PackItem(item.GetTITEM(), &item);

	VECTORDWORD vItemID;
	vItemID.push_back(dwID);

	CString strCHATMSG;
	strCHATMSG = CTChart::LoadString( TSTR_FMT_TPARTYITEMTAKE);
	INT nFind = strCHATMSG.Find("%s");
	if( nFind != -1 )
	{
		CString strFirst, strLast;

		if( nFind != 0 )
			strFirst = strCHATMSG.Left(nFind);

		nFind += 2;
		if( nFind < strCHATMSG.GetLength() )
			strLast = strCHATMSG.Right(strCHATMSG.GetLength() - nFind);

		strCHATMSG = strFirst + strChar +  strLast;
	}

	strCHATMSG = pTextLinker->MakeItemNetText(strCHATMSG, vItemID);

	CString strTYPE;
	strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
	m_pChatFrame->ChatMSG(
		strTYPE,
		m_pMainChar->GetName(),
		strCHATMSG,
		TCOLOR_INFO,
		TCHAT_FLAG_INFO,
		ID_SND_INFO,
		MEDIA_TSOUND,
		-1);

	return TERR_NONE;
}

int CTClientGame::OnCS_CHGPARTYCHIEF_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	if(bResult)
	{
		LPTSQUAD pTSQUAD = m_pTRSCS->FindTSQUAD(GetPartyID(m_pMainChar));

		if(pTSQUAD)
			m_pMainChar->m_dwChiefID = pTSQUAD->m_dwChiefID;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_NPCITEMLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	LPTOPENBYCASH pOpenShopByCash = NULL;
	if( m_pOpenShopByCash )
	{
		pOpenShopByCash = m_pOpenShopByCash;
		m_pOpenShopByCash = NULL;
	}

	WORD wNpcID;

	BYTE bCount;
	BYTE bType;
	BYTE bDiscountRate;

	(*pPacket)
		>> wNpcID
		>> bType
		>> bDiscountRate
		>> bCount;

	if( pOpenShopByCash )
	{
		if( pOpenShopByCash->m_wNPCID != wNpcID )
		{
			delete pOpenShopByCash;
			return TERR_NONE;
		}
	}
	else if( bType == TNPC_SKILL_MASTER )
	{
	}
	else
	{
		if( !m_pTARGET )
			return TERR_NONE;

		if( wNpcID != m_pTARGET->m_dwID )
			return TERR_NONE;
	}

	switch(bType)
	{
	case TNPC_SKILL_MASTER:
		{
			CTSkillDlg* pSKILLDLG = (CTSkillDlg*) GetFrame( TFRAME_SKILL );

			pSKILLDLG->m_mapTSKILL.clear();
			//pSKILLDLG->m_bTRADE = TRUE;
			CTClientSkill::m_bDiscountRate = bDiscountRate;

			for( BYTE i=0; i<bCount; i++)
			{
				DWORD dwPrice;
				WORD wSkillID;

				(*pPacket)
					>> wSkillID
					>> dwPrice;

				LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);
				if(pTSKILL)
					pSKILLDLG->m_mapTSKILL.insert( MAPTSKILLTEMP::value_type( wSkillID, pTSKILL));
			}

			ResetSkillICON();
			EnableUI(TFRAME_SKILL);
			pSKILLDLG->SetOpenByCash( pOpenShopByCash );
		}

		break;

	case TNPC_ITEM:
		{
			CTShopDlg* pShopDlg = static_cast<CTShopDlg*>(m_vTFRAME[TFRAME_ITEM_SHOP]);
			pShopDlg->SetPhurchaseType( TSHOP_PHURCHASE_TYPE_MONEY );

			CString strTitle;
			strTitle = CTChart::LoadString( TSTR_SHOPTITLE_ITEM);

			CString strDownTitle;

			if( bDiscountRate )
				strDownTitle = CTChart::Format( TSTR_SHOPTITLE_ITEM_SALE,
					bDiscountRate);
			else
				strDownTitle = "";

			pShopDlg->SetTitle(strTitle, strDownTitle);

			strTitle = CTChart::LoadString( TSTR_SHOPBUTTON_ITEM);
			pShopDlg->SetOkButtonTitle(strTitle);

			pShopDlg->SetOkGM(GM_ITEM_BUY);

			CTCtrlList* pList = pShopDlg->GetList();
			pList->ClearItem();

			for( BYTE i=0; i<bCount; ++i )
			{
				DWORD dwPrice;
				WORD wItemID;

				(*pPacket)
					>> wItemID
					>> dwPrice;

				CTShopNpcItem* pInfo = new CTShopNpcItem;
				pInfo->m_pTItem = CTChart::FindTITEMTEMP(wItemID);
				pInfo->m_pTItem->m_dwPrice = dwPrice;

				if ( (pInfo->m_pTItem->m_bLevel > m_pMainChar->m_bLevel) || !TCHECK_CLASS( pInfo->m_pTItem->m_dwClassID, m_pMainChar->m_bClassID ) )
					pInfo->m_bMark = TRUE;
				else
					pInfo->m_bMark = FALSE;

				if( bDiscountRate )
					pInfo->m_dwRealPrice = dwPrice - (dwPrice * bDiscountRate / 100);
				else
					pInfo->m_dwRealPrice = dwPrice;

				pList->AddItem(pInfo);
			}

			EnableUI(TFRAME_ITEM_SHOP);
			pShopDlg->SetOpenByCash( pOpenShopByCash );
			m_bInvenID = 0;
			m_bSlotID = 0;
		}

		break;

	case TNPC_PORTAL:
		{
			CTPortalShopDlg*	pPortalDlg = static_cast<CTPortalShopDlg*>(m_vTFRAME[TFRAME_PORTAL_SHOP_NEW]);

			CString strTitle;
			strTitle = CTChart::LoadString( TSTR_SHOPTITLE_PORTAL);
			pPortalDlg->m_pPotralName->m_strText = strTitle;

			CString strDownTitle;

			if( bDiscountRate )
				strDownTitle = CTChart::Format( TSTR_SHOPTITLE_PORTAL_SALE,
					bDiscountRate);
			else
				strDownTitle = "";

			pPortalDlg->ClearList();

			for( BYTE i=0; i<bCount; ++i )
			{
				DWORD dwPrice;
				DWORD dwRealPrice;
				WORD wPortalID;

				(*pPacket)
					>> wPortalID
					>> dwPrice;

				if( bDiscountRate )
					dwRealPrice = dwPrice - (dwPrice * bDiscountRate / 100);
				else
					dwRealPrice = dwPrice;

				pPortalDlg->AddPortal( wPortalID, dwPrice, dwRealPrice );
			}

			pPortalDlg->AlignPortalList();

			EnableUI(TFRAME_PORTAL_SHOP_NEW);
			//pPortalDlg->SetOpenByCash( pOpenShopByCash );
		}

		break;

	case TNPC_BOX:
		{
			ClearMonInven();

			for( BYTE i=0; i<bCount; i++)
			{
				CTClientItem *pTITEM = new CTClientItem();

				WORD wItemID;
				DWORD dwPrice;

				(*pPacket)
					>> wItemID
					>> dwPrice;

				pTITEM->SetItemID(wItemID);
				if( !pTITEM->CheckValid() )
				{
					delete pTITEM;
					continue;
				}
				
				pTITEM->SetItemSlot(i);

				m_vTMONINVEN.m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
			}

			EnableUI(TFRAME_MONINVEN);

			if( pOpenShopByCash )
				delete pOpenShopByCash;
		}

		break;

	case TNPC_SKILL_RENT:
		{
			CTRentSkillDlg* pTRentSkill = static_cast<CTRentSkillDlg*>(m_vTFRAME[TFRAME_RENT_SKILL]);

			CTCtrlList* pList = pTRentSkill->GetList();
			pList->ClearItem();

			for( BYTE i=0; i<bCount; ++i )
			{
				DWORD dwPrice;
				WORD wItemID;

				(*pPacket)
					>> wItemID
					>> dwPrice;

				CTRentNpcItem* pInfo = new CTRentNpcItem;
				pInfo->m_pSkill = CTChart::FindTSKILLTEMP(wItemID);
				pInfo->m_dwRealPrice = dwPrice;

				pList->AddItem(pInfo);
			}

			EnableUI(TFRAME_RENT_SKILL);
		}

		break;

	case TNPC_PVPOINT:
		{
			CTShopDlg* pShopDlg = static_cast<CTShopDlg*>(m_vTFRAME[TFRAME_ITEM_SHOP]);
			pShopDlg->SetPhurchaseType( TSHOP_PHURCHASE_TYPE_POINT );

			CString strTitle;
			strTitle = CTChart::LoadString( TSTR_SHOPTITLE_ITEM);

			CString strDownTitle;

			if( bDiscountRate )
				strDownTitle = CTChart::Format( TSTR_SHOPTITLE_ITEM_SALE,
					bDiscountRate);
			else
				strDownTitle = "";

			pShopDlg->SetTitle(strTitle, strDownTitle);

			strTitle = CTChart::LoadString( TSTR_SHOPBUTTON_ITEM);
			pShopDlg->SetOkButtonTitle(strTitle);

			pShopDlg->SetOkGM(GM_ITEM_BUY);

			CTCtrlList* pList = pShopDlg->GetList();
			pList->ClearItem();

			for( BYTE i=0; i<bCount; ++i )
			{
				DWORD dwPrice;
				WORD wItemID;

				(*pPacket)
					>> wItemID
					>> dwPrice;

				CTShopNpcItem* pInfo = new CTShopNpcItem;
				pInfo->m_pTItem = CTChart::FindTITEMTEMP(wItemID);
				pInfo->m_pTItem->m_dwPrice = dwPrice;

				if( bDiscountRate )
					pInfo->m_dwRealPrice = dwPrice - (dwPrice * bDiscountRate / 100);
				else
					pInfo->m_dwRealPrice = dwPrice;

				pList->AddItem(pInfo);
			}

			EnableUI(TFRAME_ITEM_SHOP);
			pShopDlg->SetOpenByCash( pOpenShopByCash );
			m_bInvenID = 0;
			m_bSlotID = 0;
		}

		break;

	case TNPC_MONSTER :
		{
			CTShopDlg* pShopDlg = static_cast<CTShopDlg*>(m_vTFRAME[TFRAME_MONSHOP]);
			pShopDlg->SetPhurchaseType( TSHOP_PHURCHASE_TYPE_MONEY );

			CString strTitle;
			strTitle = CTChart::Format( TSTR_TITLE_MONSHOP );

			CString strDownTitle;

			if( bDiscountRate )
				strDownTitle = CTChart::Format( TSTR_SHOPTITLE_ITEM_SALE,
					bDiscountRate);
			else
				strDownTitle = "";

			pShopDlg->SetTitle(strTitle, strDownTitle);

			strTitle = CTChart::LoadString( TSTR_SHOPBUTTON_ITEM);
			pShopDlg->SetOkButtonTitle(strTitle);

			pShopDlg->SetOkGM(GM_MONSTER_BUY);

			CTCtrlList* pList = pShopDlg->GetList();
			pList->ClearItem();

			for( BYTE i=0; i<bCount; ++i )
			{
				WORD wID;
				DWORD dwPrice;

				(*pPacket)
					>> wID
					>> dwPrice;

				CTMonsterShopItem* pInfo = new CTMonsterShopItem;
				pInfo->m_pTMon = CTChart::FindTMonShop((DWORD)wID);
				pInfo->m_dwRealPrice = dwPrice;

				pList->AddItem(pInfo);
			}

			EnableUI(TFRAME_MONSHOP);
			pShopDlg->SetOpenByCash( pOpenShopByCash );
			m_bInvenID = 0;
			m_bSlotID = 0;
		}
		break;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_ITEMBUY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	WORD wItemID;

	(*pPacket)
		>> bResult
		>> wItemID
		>> m_pMainChar->m_dwRune
		>> m_pMainChar->m_dwLuna
		>> m_pMainChar->m_dwCron;

	((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();
	ResetInvenUI();
	CancelTrade();

	CTShopDlg* pDLG = (CTShopDlg*) (m_vTFRAME[ TFRAME_ITEM_SHOP] );
	BOOL bPoint = (pDLG && pDLG->IsVisible() &&pDLG->GetPhurchaseType() == TSHOP_PHURCHASE_TYPE_POINT);

	switch(bResult)
	{
	case ITEMBUY_NOTFOUND	: return TERR_ITEM_NOT_FOUND;
	case ITEMBUY_NEEDMONEY	: return bPoint ? TERR_ITEM_NO_POINT : TERR_ITEM_NO_MONEY;
	case ITEMBUY_CANTPUSH	: return TERR_ITEM_NO_SLOT;
	case ITEMBUY_DEALING	: return TERR_ITEM_DEALING;
	case ITEMBUY_NPCCALLERROR: return TERR_INTERNAL;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SKILLBUY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wSkillID;
	BYTE bResult;
	BYTE bLevel;

	(*pPacket)
		>> bResult
		>> wSkillID
		>> bLevel;

	switch(bResult)
	{
	case SKILL_SUCCESS	:
		{
			CTClientSkill *pTSKILL = m_pMainChar->FindTSkill(wSkillID);

			if(!pTSKILL)
			{
				pTSKILL = new CTClientSkill();

				pTSKILL->m_pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);
				pTSKILL->m_bLevel = bLevel;
				pTSKILL->m_dwTick = 0;

				m_pMainChar->m_mapTSKILL.insert( MAPTSKILL::value_type( pTSKILL->m_pTSKILL->m_wSkillID, pTSKILL));
			}
			else
				pTSKILL->m_bLevel = bLevel;

			(*pPacket)
				>> m_pMainChar->m_dwRune
				>> m_pMainChar->m_dwLuna
				>> m_pMainChar->m_dwCron
				>> m_pMainChar->m_vTSKILLPOINT[TPOINT_MINE]
				>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB1]
				>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB2]
				>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB3]
				>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB4];

			ResetSkillICON();
			ResetInvenUI();
		}

		break;

	default				: SkillMSG( m_pMainChar, bResult); break;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_ITEMSELL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> m_pMainChar->m_dwRune
		>> m_pMainChar->m_dwLuna
		>> m_pMainChar->m_dwCron;

	((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();
	ResetInvenUI();
	CancelTrade();

	switch(bResult)
	{
	case ITEMSELL_NOTFOUND	: return TERR_ITEM_NOT_FOUND;
	case ITEMSELL_DEALING	: return TERR_ITEM_DEALING;
	case ITEMSELL_NPCCALLERROR: return TERR_INTERNAL;
	}
	
	return TERR_NONE;
}

int CTClientGame::OnCS_HOTKEYCHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	BYTE bSetID;
	BYTE bCount;

	(*pPacket)
		>> bSetID
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		BYTE bSlotID;

		(*pPacket)
			>> bSlotID;

		(*pPacket)
			>> pTGaugePannel->m_vTHOTKEY[bSetID][bSlotID].m_bType
			>> pTGaugePannel->m_vTHOTKEY[bSetID][bSlotID].m_wID;
	}
	pTGaugePannel->ResetHotkeyUI();

	return TERR_NONE;
}

int CTClientGame::OnCS_HELPMESSAGE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strTEXT;
	BYTE bID;

	(*pPacket)
		>> bID
		>> strTEXT;

	if(m_pNotifyFrame)
	{
		if( bID != HELPMESSAGE_NORMAL )
		{
			m_vTOPTION.m_bAutoHelp = TRUE;
			m_pGameSetting->UpdateUI();
		}

		m_pNotifyFrame->Release();
		m_pNotifyFrame->InitTEXT(strTEXT);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_QUESTLIST_COMPLETE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	return TERR_NONE;
}

int CTClientGame::OnCS_QUESTLIST_POSSIBLE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	static BYTE bLevel[QT_COUNT] = {
		THASQUEST_NONE,			// QT_NONE
		THASQUEST_NONE,			// QT_DEFTALK
		THASQUEST_NONE,			// QT_GIVESKILL
		THASQUEST_NONE,			// QT_GIVEITEM
		THASQUEST_NONE,			// QT_DROPITEM
		THASQUEST_NONE,			// QT_SPAWNMON
		THASQUEST_NONE,			// QT_TELEPORT
		THASQUEST_COMPLETE,		// QT_COMPLETE
		THASQUEST_MISSION,		// QT_MISSION
		THASQUEST_ROUTING,		// QT_ROUTING
		THASQUEST_NONE,			// QT_NPCTALK
		THASQUEST_NONE,			//QT_DROPQUEST,
		THASQUEST_NONE,			//QT_CHAPTERMSG,
		THASQUEST_NONE,			//QT_SWITCH,
		THASQUEST_NONE};		//QT_DIEMON

	BYTE bCount;

	(*pPacket)
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		BYTE bQuestCount;
		WORD wNpcID;
		BYTE bCountry;

		(*pPacket)
			>> wNpcID
			>> bCountry
			>> bQuestCount;

		CTClientObjBase *pTNPC = m_vMAP.FindTNPC(wNpcID);
		if(pTNPC)
		{
			pTNPC->m_bContryID = bCountry;
			pTNPC->m_bHasQuest = THASQUEST_NONE;
			pTNPC->m_mapTQUESTTEMP.clear();
			pTNPC->m_mapTQUESTCOND.clear();

			for( BYTE j=0; j<bQuestCount; j++)
			{
				DWORD dwQuestID;
				
				BYTE bQuestCON;

				(*pPacket)
					>> dwQuestID
					>> bQuestCON;

				LPTQUEST pTQUEST = CTChart::FindTQUEST( dwQuestID);
				if( pTQUEST )
				{
					BYTE bQuestSt = THASQUEST_NONE;

					if( pTQUEST->m_bType < QT_COUNT )
						bQuestSt = bLevel[pTQUEST->m_bType];

					if( bQuestSt == THASQUEST_MISSION )
					{
						if( bQuestCON == QCT_UPPERLEVEL )
							bQuestSt = THASQUEST_MISSION_X;
						else if( bQuestCON != QCT_NONE )
							continue;
					}
					else if( bQuestSt == THASQUEST_COMPLETE )
					{
						LPTQUEST pTMISSION = CTChart::FindTMISSION(pTQUEST);

					#ifdef TEST_MODE
						if( !pTMISSION )
						{
							CString strFILE;
							strFILE.Format("QuestError_%u", GetTickCount());

							CStdioFile file;
							if( file.Open( strFILE, CStdioFile::typeText | CStdioFile::modeCreate | CStdioFile::modeWrite ) )
							{
								CString strMsg;
								strMsg.Format( "%u 퀘스트의 Term중에 완료퀘스트가 있는데 그 퀘스트 아이디를 찾을 수 없다.",
									dwQuestID);
								file.WriteString(strMsg);
								file.Close();
							}
						}
					#endif

						CTClientQuest* pTQuest = m_pMainChar->FindTQuest(pTMISSION->m_dwID);
						if( !pTQuest )
							continue;

						if( pTQuest->GetResult() != TTERMRESULT_COMPLETE )
							bQuestSt = THASQUEST_COMPLETE_X;
					}

					if( pTNPC->m_bHasQuest < bQuestSt )
						pTNPC->m_bHasQuest = bQuestSt;

					pTNPC->m_mapTQUESTTEMP.insert(make_pair( dwQuestID, pTQUEST));
					pTNPC->m_mapTQUESTCOND.insert(make_pair( dwQuestID, bQuestCON));
				}
			}

			pTNPC->ResetTQUESTMARKER(m_pRES);
		}
	}

	CTNPCTalkDlg *pTNPCTalkDlg = (CTNPCTalkDlg *) m_vTFRAME[TFRAME_NPCDIALOG];
	if(pTNPCTalkDlg->IsVisible())
		ResetNPCTalkUI();

	return TERR_NONE;
}

int CTClientGame::OnCS_QUESTSTARTTIMER_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwQuestID;
	DWORD dwTick;

	(*pPacket)
		>> dwQuestID
		>> dwTick;

	CTClientQuest *pTQUEST = m_pMainChar->FindTQuest(dwQuestID);
	if(pTQUEST)
	{
		pTQUEST->m_dwTick = dwTick;
		pTQUEST->m_bTimmer = TRUE;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_QUESTCOMPLETE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTNPCTalkDlg *pTNPCTalkDlg = (CTNPCTalkDlg *) m_vTFRAME[TFRAME_NPCDIALOG];
	CTCraftQuestDlg *pTCraftDlg = (CTCraftQuestDlg*) m_vTFRAME[TFRAME_CRAFT];
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	DWORD dwQuestID;
	DWORD dwTermID;
	DWORD dwDropID;

	BYTE bReQuery = FALSE;
	BYTE bResult;
	BYTE bTermType;

	(*pPacket)
		>> bResult
		>> dwQuestID
		>> dwTermID
		>> bTermType
		>> dwDropID;

	switch(bResult)
	{
	case QR_INVENTORYFULL	:
		{
			LPTQUEST pTQUEST = CTChart::FindTQUEST( dwQuestID );

			if(pTNPCTalkDlg->IsVisible())
			{
				pTNPCTalkDlg->Talk(pTQUEST->m_strRejectMSG);
			}

			if(pTCraftDlg->IsVisible())
			{
				CString strMSG = pTQUEST->m_strRejectMSG;
				if( strMSG.IsEmpty() )
					strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_INVENFULL );

				ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_INFO);
			}

			if(dwDropID)
			{
				m_pMainChar->EraseTQuest(dwDropID);
				bReQuery = TRUE;
			}
		}

		break;

	case QR_TERM			:
		{
			if(pTNPCTalkDlg->IsVisible())
			{
				LPTQUEST pTQUEST = CTChart::FindTQUEST(dwQuestID);
				LPTTERM pTTERM = CTClientQuest::FindTTerm(
					pTQUEST,
					dwTermID,
					bTermType);

				if(pTTERM)
					pTNPCTalkDlg->Talk(pTTERM->m_strFailMSG);
			}

			if(dwDropID)
			{
				m_pMainChar->EraseTQuest(dwDropID);
				bReQuery = TRUE;
			}
		}

		break;

	case QR_SUCCESS			:
		{
			LPTQUEST pTQUEST = CTChart::FindTQUEST(dwQuestID);

			m_pMainChar->EraseTQuest(dwQuestID);
			if(dwDropID)
				m_pMainChar->EraseTQuest(dwDropID);

			if(pTNPCTalkDlg->IsVisible())
			{
				pTNPCTalkDlg->Talk(pTQUEST->m_strCompleteMSG);
				ResetNPCTalkUI();
			}

			if(pTCraftDlg->IsVisible())
			{
				ShowInfoMsgBox( pTQUEST->m_strCompleteMSG, TRUE, TUISND_TYPE_INFO);
				pTCraftDlg->ResetTerm();
			}

			pTGaugePannel->ResetHotkeyUI();
			ResetInvenUI();
			PlayUISound(ID_SND_COMP_QUEST);

			bReQuery = TRUE;
			if(pTQUEST)
			{
				CTHelpFrame *pHELPFRAME = (CTHelpFrame *) m_vTFRAME[TFRAME_HELP];

				pHELPFRAME->OnQuestEvent(
					THELPLINK_TRIGGER_COMPLETE,
					pTQUEST->m_dwID);
			}

			if( dwQuestID == TTUTORIAL_EXIT_QUEST_ID )
				OnGM_CLICKED_STOP_TUTORIAL();
		}

		break;

	case QR_DROP			:
		if(dwDropID)
		{
			m_pMainChar->EraseTQuest(dwDropID);
			bReQuery = TRUE;
		}

		break;
	}

	if(bReQuery)
	{
		MAPOBJECT::iterator itTNPC;

		m_vMAP.m_vNEWQUESTOBJ.clear();
		for( itTNPC = m_vMAP.m_mapTNpc.begin(); itTNPC != m_vMAP.m_mapTNpc.end(); itTNPC++)
			m_vMAP.m_vNEWQUESTOBJ.push_back(((CTClientObjBase *) (*itTNPC).second)->m_dwID);

		CheckQuestOBJ();
		ResetQuestUI();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_QUESTUPDATE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwQuestID;
	DWORD dwTermID;

	BYTE bTermType;
	BYTE bCount;
	BYTE bStatus;

	(*pPacket)
		>> dwQuestID
		>> dwTermID
		>> bTermType
		>> bCount
		>> bStatus;

	CTClientQuest *pTQuest = m_pMainChar->FindTQuest(dwQuestID);
	if(pTQuest)
	{
		CTClientTerm *pTTerm = pTQuest->FindTTerm(
			dwTermID,
			bTermType);

		if(!pTTerm)
		{
			pTTerm = new CTClientTerm();

			pTTerm->m_pTTERM = CTClientQuest::FindTTerm(
				pTQuest->m_pTQUEST,
				dwTermID,
				bTermType);
			pTQuest->m_vTTERM.push_back(pTTerm);
		}

		CTNPCTalkDlg *pTNPCTalkDlg = (CTNPCTalkDlg *) m_vTFRAME[TFRAME_NPCDIALOG];
		CTNewQuestDlg *pTQuestDlg = (CTNewQuestDlg *) m_vTFRAME[TFRAME_QUEST];
		CTGaugePannel *pTPANNEL = (CTGaugePannel *) m_vTFRAME[TFRAME_GAUGE];

		CString strRESULT;
		CString strTITLE;

		pTTerm->m_bStatus = bStatus;
		pTTerm->m_bCount = bCount;

		switch(pTTerm->GetResult())
		{
		case TTERMRESULT_INCOMPLETE	:
			{
				strRESULT = CTChart::Format( TSTR_QUEST_STATUS, pTTerm->m_bCount, pTTerm->m_pTTERM->m_bCount);
				strTITLE = pTTerm->m_pTTERM->m_strTermMSG;
			}

			break;

		case TTERMRESULT_TIMEOUT	:
			{
				strTITLE = pTQuest->m_pTQUEST->m_strTITLE;
				strRESULT = CTChart::LoadString( TSTR_QRESULT_TIMEOUT);
			}

			break;

		case TTERMRESULT_FAILED		:
			{
				strTITLE = pTQuest->m_pTQUEST->m_strTITLE;
				strRESULT = CTChart::LoadString( TSTR_QRESULT_FAILED);
			}

			break;

		case TTERMRESULT_COMPLETE	:
			{
				strRESULT = CTChart::LoadString( TSTR_QRESULT_COMPLETE);
				strTITLE = pTTerm->m_pTTERM->m_strTermMSG;

				MAPOBJECT::iterator itTNPC;
				m_vMAP.m_vNEWQUESTOBJ.clear();
				for( itTNPC = m_vMAP.m_mapTNpc.begin(); itTNPC != m_vMAP.m_mapTNpc.end(); itTNPC++)
					m_vMAP.m_vNEWQUESTOBJ.push_back(((CTClientObjBase *) (*itTNPC).second)->m_dwID);

				CheckQuestOBJ();
			}

			break;
		}

		if( bCount > 0 )
			((CTMainUI *) m_vTFRAME[TFRAME_MAIN])->ResetQuestMSG( strTITLE, strRESULT);

		if(pTNPCTalkDlg->IsVisible())
		{
			LPTQUEST pTMISSION = pTNPCTalkDlg->GetCurTMission();

			if( pTMISSION && pTMISSION->m_dwID == dwQuestID )
				pTNPCTalkDlg->ResetTQUEST(NULL);
		}

		if( pTQuestDlg->IsVisible() && pTQuestDlg->m_pTQUEST &&
			pTQuestDlg->m_pTQUEST->m_dwID == dwQuestID )
		{
			pTQuestDlg->ResetTree();
			pTQuestDlg->ResetTQUEST(NULL);
		}

		if(pTPANNEL)
			pTPANNEL->UpdateQuestINFO(pTQuest);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_QUESTLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;

	m_pMainChar->ReleaseData();
	m_pMainChar->m_bDrawName = FALSE;

	(*pPacket)
		>> bCount;

	for( BYTE i=0; i<bCount; i++)
	{
		CTClientQuest *pTQuest = new CTClientQuest();

		DWORD dwQuestID;
		BYTE bTermCount;
		BYTE bType;

		(*pPacket)
			>> dwQuestID
			>> bType
			>> bTermCount;

		pTQuest->m_pTQUEST = CTChart::FindTQUEST(dwQuestID);
		for( BYTE j=0; j<bTermCount; j++)
		{
			CTClientTerm *pTTerm = new CTClientTerm();

			DWORD dwTermID;
			BYTE bTermType;
			BYTE bNeedCount;

			(*pPacket)
				>> dwTermID
				>> bTermType
				>> bNeedCount
				>> pTTerm->m_bCount
				>> pTTerm->m_bStatus;

			if( bTermType == QTT_TIMER )
				pTQuest->m_bTimmer = TRUE;

			pTTerm->m_pTTERM = CTClientQuest::FindTTerm(
				pTQuest->m_pTQUEST,
				dwTermID,
				bTermType);
			pTQuest->m_vTTERM.push_back(pTTerm);
		}

		if( pTQuest->m_pTQUEST->m_bType == QT_MISSION )
			m_pMainChar->InsertTQuest(pTQuest);
		else
			delete pTQuest;
	}
	ResetQuestUI();

	return TERR_NONE;
}

int CTClientGame::OnCS_CHAPTERMSG_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwQuestID;

	(*pPacket)
		>> dwQuestID;

	LPTQUEST pTQUEST = CTChart::FindTQUEST(dwQuestID);
	if(pTQUEST)
		((CTMainUI *) m_vTFRAME[TFRAME_MAIN])->ResetChapterMSG(pTQUEST->m_strTITLE);

	return TERR_NONE;
}

int CTClientGame::OnCS_QUESTADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTClientQuest *pTQuest = new CTClientQuest();

	DWORD dwQuestID;
	BYTE bType;

	(*pPacket)
		>> dwQuestID
		>> bType;

	pTQuest->m_pTQUEST = CTChart::FindTQUEST(dwQuestID);
	if( pTQuest->m_pTQUEST->m_bType == QT_MISSION )
	{
		CTNPCTalkDlg *pTNPCTalkDlg = (CTNPCTalkDlg *) m_vTFRAME[TFRAME_NPCDIALOG];

		if(pTNPCTalkDlg->IsVisible())
		{
			pTNPCTalkDlg->Talk(
				pTQuest->m_pTQUEST->m_strAcceptMSG,
				pTQuest->m_pTQUEST->m_strReply);

			ResetNPCTalkUI();
		}

		m_pMainChar->InsertTQuest(pTQuest);
		ResetQuestUI();
		PlayUISound(ID_SND_BEGIN_QUEST);
	}
	else
		delete pTQuest;

	return TERR_NONE;
}

int CTClientGame::OnCS_MONITEMLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwMonID;

	DWORD dwRune;
	DWORD dwLuna;
	DWORD dwCron;

	BYTE bResult;
	BYTE bCount;
	BYTE bUpdate;

	(*pPacket)
		>> bResult
		>> bUpdate
		>> dwMonID
		>> dwRune
		>> dwLuna
		>> dwCron
		>> bCount;

	CTClientMonster* pMon = FindMonster(dwMonID);
	if( pMon && dwRune == 0 && dwLuna == 0 && dwCron == 0 && bCount == 0 )
		pMon->m_bTakenItemAll = TRUE;

	switch(bResult)
	{
	case MIL_SUCCESS	:
		if( m_pTARGET && m_pTARGET->m_bType == OT_MON && m_pTARGET->m_dwID == dwMonID )
		{
			ClearMonInven();
			m_vMonMoney[TMONEY_RUNE] = dwRune;
			m_vMonMoney[TMONEY_LUNA] = dwLuna;
			m_vMonMoney[TMONEY_CRON] = dwCron;

			for( BYTE i=0; i<bCount; i++)
			{
				CTClientItem *pTITEM = new CTClientItem();
				
				pTITEM->SetItemSlot(pPacket);
				pTITEM->SetItemData(pPacket);
				pTITEM->SetItemMagic(pPacket);

				if( !pTITEM->CheckValid() )
				{
					delete pTITEM;
					continue;
				}

				m_vTMONINVEN.m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
			}

			if(CanLookAt())
			{
				WORD wPITCH = m_pMainChar->GetTargetPITCH(
					m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
					m_pTARGET->GetPositionY() - m_pMainChar->GetPositionY(),
					m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ());

				WORD wDIR = m_pMainChar->GetTargetDIR(
					m_pTARGET->GetPositionX() - m_pMainChar->GetPositionX(),
					m_pTARGET->GetPositionZ() - m_pMainChar->GetPositionZ());

				LookAt( wDIR, wPITCH);
			}

			if(!bUpdate)
				EnableUI(TFRAME_MONINVEN);
			else
				ResetTMonInvenUI();
		}
		else
			SendCS_MONITEMLIST_REQ( FALSE, dwMonID);

		break;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_MONITEMTAKE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	switch(bResult)
	{
	case MIT_AUTHORITY	: 
	case MIT_NOTFOUND	:
	case MIT_LOTTERY	: break;
	
	case MIT_FULLINVEN	: 
		{
			CString strMSG;
			strMSG = CTChart::LoadString( TSTR_ITEM_NO_SLOT);
			ShowInfoChat(strMSG, TUISND_TYPE_ERROR);			
		}
		break;
	}

	((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();
	ResetInvenUI();

	return TERR_NONE;
}

int CTClientGame::OnCS_MONITEMLOTTERY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwMonID;

	(*pPacket)
		>> dwMonID;

	CTClientItem* pTITEM = new CTClientItem;

	pTITEM->SetItemData(pPacket);
	pTITEM->SetItemMagic(pPacket);

	if( !pTITEM->CheckValid() )
	{
		delete pTITEM;
		return TERR_NONE;
	}

	CTPartyItemLottery::NewInstance(dwMonID, pTITEM);

	return TERR_NONE;
}

int CTClientGame::OnCS_CHARSTATINFO_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	(*pPacket)
		>> dwCharID;
	
	CTClientChar *pTCHAR = m_pMainChar->m_dwID ? FindPC(dwCharID) : m_pMainChar;
	if(!pTCHAR)
		return TERR_NONE;

	(*pPacket)
		>> pTCHAR->m_StatInfo.m_wSTR
		>> pTCHAR->m_StatInfo.m_wDEX
		>> pTCHAR->m_StatInfo.m_wCON
		>> pTCHAR->m_StatInfo.m_wINT
		>> pTCHAR->m_StatInfo.m_wWIS
		>> pTCHAR->m_StatInfo.m_wMEN
		>> pTCHAR->m_StatInfo.m_dwMinPysAtkPower
		>> pTCHAR->m_StatInfo.m_dwMaxPysAtkPower
		>> pTCHAR->m_StatInfo.m_dwPysDefPower
		>> pTCHAR->m_StatInfo.m_dwMinLongAtkPower
		>> pTCHAR->m_StatInfo.m_dwMaxLongAtkPower
		>> pTCHAR->m_StatInfo.m_dwAttackDelay[TAD_PHYSICAL]
		>> pTCHAR->m_StatInfo.m_dwAttackDelay[TAD_LONG]
		>> pTCHAR->m_StatInfo.m_dwAttackDelay[TAD_MAGIC]
		>> pTCHAR->m_StatInfo.m_dwAttackDelayRate[TAD_PHYSICAL]
		>> pTCHAR->m_StatInfo.m_dwAttackDelayRate[TAD_LONG]
		>> pTCHAR->m_StatInfo.m_dwAttackDelayRate[TAD_MAGIC]
		>> pTCHAR->m_StatInfo.m_wAtkLevel
		>> pTCHAR->m_StatInfo.m_wDefLevel
		>> pTCHAR->m_StatInfo.m_bPysCriticalProb
		>> pTCHAR->m_StatInfo.m_dwMinMagicAP
		>> pTCHAR->m_StatInfo.m_dwMaxMagicAP
		>> pTCHAR->m_StatInfo.m_dwMagicDefPower
		>> pTCHAR->m_StatInfo.m_wMagicAtkLevel
		>> pTCHAR->m_StatInfo.m_wMagicDefLevel
		>> pTCHAR->m_StatInfo.m_bChargeSpeed
		>> pTCHAR->m_StatInfo.m_bChargeProb
		>> pTCHAR->m_StatInfo.m_bMagicCriticalProb
		>> pTCHAR->m_StatInfo.m_wSkillPoint
		>> pTCHAR->m_bAfterMath;

	CTCharInfoDlg *pUI = (CTCharInfoDlg *) m_vTFRAME[m_pMainChar == pTCHAR ? TFRAME_CHAR_INFO : TFRAME_TARGET_INFO];

	pUI->m_vTCHARSTAT[TCHARSTAT_STR]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wSTR);

	pUI->m_vTCHARSTAT[TCHARSTAT_DEX]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wDEX);

	pUI->m_vTCHARSTAT[TCHARSTAT_CON]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wCON);

	pUI->m_vTCHARSTAT[TCHARSTAT_INT]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wINT);

	pUI->m_vTCHARSTAT[TCHARSTAT_WIS]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wWIS);

	pUI->m_vTCHARSTAT[TCHARSTAT_MEN]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wMEN);

	pUI->m_vTCHARBASIC[TCHARBASIC_PAP]->m_strText = CTChart::Format( TSTR_FMT_CHARINFO_RANGE,
		pTCHAR->m_StatInfo.m_dwMinPysAtkPower,
		pTCHAR->m_StatInfo.m_dwMaxPysAtkPower);

	pUI->m_vTCHARBASIC[TCHARBASIC_PDP]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_dwPysDefPower);

	pUI->m_vTCHARBASIC[TCHARBASIC_PSP]->m_strText = CTChart::Format( TSTR_FMT_CHARINFO_RANGE,
		pTCHAR->m_StatInfo.m_dwMinLongAtkPower,
		pTCHAR->m_StatInfo.m_dwMaxLongAtkPower);

	pUI->m_vTCHARBASIC[TCHARBASIC_PAL]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wAtkLevel);

	pUI->m_vTCHARBASIC[TCHARBASIC_PDL]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wDefLevel);

	pUI->m_vTCHARBASIC[TCHARBASIC_PCR]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_bPysCriticalProb);

	pUI->m_vTCHARBASIC[TCHARBASIC_MAP]->m_strText = CTChart::Format( TSTR_FMT_CHARINFO_RANGE,
		pTCHAR->m_StatInfo.m_dwMinMagicAP,
		pTCHAR->m_StatInfo.m_dwMaxMagicAP);

	pUI->m_vTCHARBASIC[TCHARBASIC_MDP]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_dwMagicDefPower);

	pUI->m_vTCHARBASIC[TCHARBASIC_MAL]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wMagicAtkLevel);

	pUI->m_vTCHARBASIC[TCHARBASIC_MDL]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_wMagicDefLevel);
	
	pUI->m_vTCHARBASIC[TCHARBASIC_STA]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_bChargeProb);

	pUI->m_vTCHARBASIC[TCHARBASIC_MCR]->m_strText = CTChart::Format( TSTR_FMT_BASIC_STAT,
		pTCHAR->m_StatInfo.m_bMagicCriticalProb);

	pUI->m_vTCHARADV[TCHARADV_NATION]->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[pTCHAR->m_bContryID] );
	pUI->m_vTCHARADV[TCHARADV_RACE]->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTRACESTR[pTCHAR->GetRaceID()] );
	pUI->m_vTCHARADV[TCHARADV_CLASS]->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[pTCHAR->m_bClassID] );

	pTCHAR->m_StatInfo.m_dwAttackDelay[ TAD_NONE ] = 0;
	pTCHAR->m_StatInfo.m_dwAttackDelayRate[ TAD_NONE ] = 0;

	for( BYTE i=0; i<TAD_COUNT; ++i)
	{
		pTCHAR->m_dwAttackDelay[i] = pTCHAR->m_StatInfo.m_dwAttackDelay[i];
		pTCHAR->m_dwAttackDelayRate[i] = pTCHAR->m_StatInfo.m_dwAttackDelayRate[i];
	}

	pTCHAR->m_vTSKILLPOINT[TPOINT_MINE] = pTCHAR->m_StatInfo.m_wSkillPoint;
	pTCHAR->m_bChargeSpeed = pTCHAR->m_StatInfo.m_bChargeSpeed;

	if( m_pMainChar != pTCHAR )
	{
		pUI->ResetData(
			pTCHAR,
			m_pRES,
			pUI->m_bTabIndex);

		EnableUI(TFRAME_TARGET_INFO);
	}
	else
	{
		m_vTGAUGE[TGAUGE_VITAL]->SetGauge(min(TMAX_VITAL, m_pMainChar->m_bAfterMath), TMAX_VITAL);
		ResetSkillICON();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SKILLEND_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;
	BYTE bType;

	WORD wSkillID;

	(*pPacket)
		>> dwID
		>> bType
		>> wSkillID;

	CTClientObjBase *pOBJ = FindOBJ(
		dwID,
		bType);

	if(pOBJ)
	{
#ifdef TEST_MODE
		/*if( m_bFakePacket )
		{
			CString strFMT;
			strFMT.Format( "fake Recv SkillEnd %s (%d)",
				pOBJ->GetName(),
				wSkillID );
			CTClientGame::DebugMSG( strFMT );
		}
		else
		{
			CString strFMT;
			strFMT.Format( "Recv SkillEnd %s (%d)",
				pOBJ->GetName(),
				wSkillID );
			CTClientGame::DebugMSG( strFMT );
		}*/
#endif

		pOBJ->EraseSendedRemoveMaintain( wSkillID );

		CTClientMaintain* pTMAINTAIN = pOBJ->FindMaintain( wSkillID );
		if( pTMAINTAIN != NULL )
		{
			for( int i=0; i<INT(pTMAINTAIN->m_pTSKILL->m_vTFUNCTION.size()); i++)
				if( pTMAINTAIN->m_pTSKILL->m_vTFUNCTION[i]->m_bType == SDT_STATUS &&
					pTMAINTAIN->m_pTSKILL->m_vTFUNCTION[i]->m_bFunctionID == SDT_STATUS_FINISHCURSE )
				{
					LPTSKILL pTFINISH = CTChart::FindTSKILLTEMP(pTMAINTAIN->m_pTSKILL->m_vTFUNCTION[i]->m_wValue);

					if(pTFINISH)
					{
						CTClientObjBase *pTATTACK = FindOBJ(
							pTMAINTAIN->m_dwHostID,
							pTMAINTAIN->m_bHostType);

						if(CanDefend( pTATTACK, pOBJ, pTFINISH))
							Defend( pTATTACK, pOBJ, &pTMAINTAIN->m_vTSKILLDATA, &pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND, pTFINISH, 0, TRUE);
					}
				}

			pOBJ->OnMaintainEnd(
				m_pDevice,
				m_pRES,
				pTMAINTAIN);

			BOOL bMoveSpeed = FALSE;
			if( CheckSkillHasFunc(pTMAINTAIN->m_pTSKILL, SDT_STATUS, SDT_STATUS_MOVESPEED) )
				bMoveSpeed = TRUE;

			pOBJ->EraseMaintain( wSkillID );

			// finder를 지운다음에 AdjustSDT_STATUS_MOVESPEED를 실행시키는 이유는
			// 이 함수는 전체 MAINTAIN을 순회하면서 가장 낮은 이동속도를 캐릭터의 속도로 선택하는 일을 하는데
			// finder는 서버가 지우라고 한 MAINTAIN이기 때문에 얘를 포함해서 순회하면 안되기 때문이다.
			if( bMoveSpeed )
				pOBJ->AdjustSDT_STATUS_MOVESPEED();

			if( pOBJ == m_pMainChar )
				((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();

			if(GetPartyID(m_pMainChar))
				if(IsHostingEnemy(pOBJ))
					pOBJ->m_bTCORPSMOVE = TCORPSMOVE_ENEMY;
				else if(IsRSCSEnemy(pOBJ))
					RelaySession()->SendCS_ADDCORPSENEMY_REQ( m_pMainChar->m_dwID, m_vMAP.m_wMapID, (CTClientMoveObj *) pOBJ);
				else if( pOBJ == m_pMainChar )
					m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
		}
	}

	if( pOBJ == m_pTARGET ||
		pOBJ == m_pMainChar )
	{
		((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_MONSETSKILL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTClientMonster *pMON = NULL;
	DWORD dwMonID;

	WORD wAtkRange;
	WORD wSkillID;

	(*pPacket)
		>> dwMonID
		>> wSkillID
		>> wAtkRange;

	pMON = FindMonster(dwMonID);
	if(!pMON)
		return TERR_NONE;

	pMON->m_pTDEFSKILL = CTChart::FindTSKILLTEMP(wSkillID);
	pMON->m_fAB = FLOAT(wAtkRange);

	CTClientObjBase* pFOLLOW = FindOBJ( pMON->m_dwFollowID,
		pMON->m_bFollowType );

	if( pFOLLOW &&
		pFOLLOW->m_bType == OT_PC )
	{
		FLOAT fAB = pMON->GetAB( pFOLLOW );
		if( fAB >= TPATH_LONG )
		{
			pFOLLOW->LeaveAdjacency( pMON );
			pMON->m_bBACK = FALSE;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_NPCTALK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwQuestID;
	WORD wNpcID;

	(*pPacket)
		>> dwQuestID
		>> wNpcID;

	if( !m_pTARGET || m_pTARGET->m_bType != OT_NPC || m_pTARGET->m_dwID != wNpcID )
		return TERR_NONE;
	BYTE bHasQuest = m_pTARGET->m_bHasQuest;

	if(dwQuestID)
		bHasQuest = THASQUEST_NONE;

	m_pTARGET->PlayRandomSND(
		&m_pTARGET->m_mapOBJSNDINST,
		&m_pTARGET->m_mapOBJSND,
		((CTClientNpc *) m_pTARGET)->m_pTNPC->m_dwSND,
		FALSE);

	switch(bHasQuest)
	{
	case THASQUEST_COMPLETE		:
	case THASQUEST_MISSION		:
	case THASQUEST_COMPLETE_X	:
	case THASQUEST_MISSION_X	:
		{
			CTNPCTalkDlg *pTNPCTalkDlg = (CTNPCTalkDlg *) m_vTFRAME[TFRAME_NPCDIALOG];

			ResetNPCTalkUI();
			EnableUI(TFRAME_NPCDIALOG);

			if( pTNPCTalkDlg->GetCount() > 0 )
				pTNPCTalkDlg->SetCurSel(0);
			else
				pTNPCTalkDlg->DefaultTalk();
		}

		break;

	case THASQUEST_ROUTING	:
		{
			ResetActOBJ(NULL);

			BeginCharge(
				TBOX_CHARGE_TICK,
				TCHARGETYPE_OPEN);
		}

		break;

	default					:
		{
			CString strMSG = m_pTARGET->GetDefaultTALK();
			CString strTYPE;

			if(!strMSG.IsEmpty())
			{
				strMSG.Replace( "%s", m_pMainChar->GetName());
				strTYPE = CTChart::LoadString( TSTR_CHAT_NORMAL);

				strMSG = m_pChatFrame->ChatSysMSG(
					strTYPE,
					m_pTARGET->GetName(),
					strMSG,
					0xFFFFFFFF,
					TCHAT_FLAG_NEAR);

				m_pTARGET->ActivateTALK(
					m_pDevice->m_pDevice,
					strMSG,
					0xFFFFFFFF);
			}

			DisableTargetUI();
			CancelTrade();
		}

		break;
	}

	m_pMainChar->CancelHiding( this );
	return TERR_NONE;
}

int CTClientGame::OnCS_REVIVALASK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;
	BYTE bType;

	WORD wSkillID;
	BYTE bLevel;

	(*pPacket)
		>> dwID
		>> bType
		>> wSkillID
		>> bLevel;

	CTClientObjBase *pTOBJ = FindObject(
		dwID,
		bType);

	if( pTOBJ && (m_pMainChar->IsDead() || m_pMainChar->m_bGhost) )
	{
		CString strMSG;

		strMSG = CTChart::Format( TSTR_FMT_REVIVAL_REQ, pTOBJ->GetName());
		DisableUI(TFRAME_WORLDMAP);

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_REVIVAL_YES,
			GM_REVIVAL_NO,
			0,
			FALSE,
			ID_SND_INFO,
			MEDIA_TSOUND,
			TMSGBOXOPEN_OVERLAP );

		m_pMainChar->m_vTSKILLDATA.m_bAtkLevel = bType;
		m_pMainChar->m_vTSKILLDATA.m_dwMaxAP = dwID;

		m_pMainChar->m_vTSKILLDATA.m_bLevel = bLevel;
		m_pMainChar->m_vTSKILLDATA.m_wID = wSkillID;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_REVIVALREPLY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;
	BYTE bReply;

	(*pPacket)
		>> bReply
		>> dwID;

	CTClientChar *pTCHAR = FindPlayer(dwID);
	if( !bReply && pTCHAR )
	{
		CString strTYPE;
		CString strMSG;

		strMSG = CTChart::Format( TSTR_FMT_REVIVAL_REJECT, pTCHAR->GetName());
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, TCOLOR_ERROR, TCHAT_FLAG_INFO);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CHANGECOLOR_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;
	BYTE bType;

	(*pPacket)
		>> bType
		>> dwID;

	CTClientObjBase *pTOBJ = FindObject(
		dwID,
		bType);

	if(pTOBJ)
	{
		(*pPacket)
			>> pTOBJ->m_bColorID
			>> pTOBJ->m_bContryID;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_ADDRECALLMON_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwRegionID;
	DWORD dwHostID;
	DWORD dwMonID;
	DWORD dwLifeTick;

	WORD wTempID;
	WORD wPITCH;
	WORD wDIR;
	WORD wPetID;

	BYTE bNewMember;
	BYTE bCount;
	CString strPetName;

	(*pPacket)
		>> dwHostID
		>> dwMonID
		>> wTempID
		>> wPetID
		>> strPetName;

	CTClientChar *pHOST = FindPC(dwHostID);
	CTClientRecall *pTRECALL;

	if(FindRecall(dwMonID))
	{
		if( wPetID )
		{
			CTPetManageDlg *pPetDlg = static_cast<CTPetManageDlg *>(m_vTFRAME[TFRAME_PET_MANAGE]);
			pPetDlg->SetRecalling(FALSE);
		}

		return TERR_NONE;
	}

	if(!wTempID)
		pTRECALL = new CTClientFake();
	else if(wPetID)
		pTRECALL = new CTClientPet();
	else
		pTRECALL = new CTClientRecall();

	pTRECALL->m_pHost = m_pMainWnd;
	pTRECALL->ReleaseData();

	(*pPacket)
		>> pTRECALL->m_bContryID
		>> pTRECALL->m_bColorID
		>> pTRECALL->m_bLevel
		>> pTRECALL->m_dwMaxHP
		>> pTRECALL->m_dwHP
		>> pTRECALL->m_dwMaxMP
		>> pTRECALL->m_dwMP
		>> pTRECALL->m_vWorld._41
		>> pTRECALL->m_vWorld._42
		>> pTRECALL->m_vWorld._43
		>> wPITCH
		>> wDIR
		>> pTRECALL->m_bMouseDIR
		>> pTRECALL->m_bKeyDIR
		>> pTRECALL->m_bAction
		>> pTRECALL->m_bMode
		>> bNewMember
		>> dwRegionID
		>> pTRECALL->m_bRecallType
		>> pTRECALL->m_vTSKILLDATA.m_bHIT
		>> pTRECALL->m_vTSKILLDATA.m_bLevel
		>> pTRECALL->m_vTSKILLDATA.m_wAL
		>> pTRECALL->m_vTSKILLDATA.m_bAtkLevel
		>> pTRECALL->m_vTSKILLDATA.m_dwMinAP
		>> pTRECALL->m_vTSKILLDATA.m_dwMaxAP
		>> pTRECALL->m_vTSKILLDATA.m_dwMinMP
		>> pTRECALL->m_vTSKILLDATA.m_dwMaxMP
		>> dwLifeTick
		>> bCount;

	pTRECALL->m_dwEndLifeTick = ::GetTickCount() + dwLifeTick;
	pTRECALL->m_pTREGION = CTChart::FindTREGIONINFO( dwRegionID);

	for( BYTE i=0; i<bCount; i++)
	{
		CTClientMaintain *pTMAINTAIN = new CTClientMaintain();

		(*pPacket)
			>> pTMAINTAIN->m_vTSKILLDATA.m_wID
			>> pTMAINTAIN->m_vTSKILLDATA.m_bLevel
			>> pTMAINTAIN->m_dwTick
			>> pTMAINTAIN->m_dwAttackID
			>> pTMAINTAIN->m_bAttackType
			>> pTMAINTAIN->m_dwHostID
			>> pTMAINTAIN->m_bHostType
			>> pTMAINTAIN->m_vTSKILLDATA.m_bHIT
			>> pTMAINTAIN->m_vTSKILLDATA.m_wAL
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkLevel
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAglow
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkCountryID
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.x
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.y
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.z;
		pTMAINTAIN->m_pTSKILL = CTChart::FindTSKILLTEMP(pTMAINTAIN->m_vTSKILLDATA.m_wID);

		if(pTMAINTAIN->m_pTSKILL)
		{
			pTMAINTAIN->m_pTIMMER = CTClientSkill::FindTIMMER(pTMAINTAIN->m_pTSKILL);
			pTMAINTAIN->m_bLevel = pTMAINTAIN->m_vTSKILLDATA.m_bLevel;

			pTMAINTAIN->m_bUnlimitedType = pTMAINTAIN->m_pTSKILL->m_dwDuration ? FALSE : TRUE;
			pTMAINTAIN->m_dwEndTick = pTMAINTAIN->m_dwTick + m_vKEY.m_dwTick;

			pTRECALL->InsertMaintain( pTMAINTAIN->m_vTSKILLDATA.m_wID, pTMAINTAIN);
			pTRECALL->PlayTActionSFX(
				&pTMAINTAIN->m_vTSFX,
				pTMAINTAIN->m_pTSKILL->m_dwSFX[TSKILLSFX_LOOP]);
		}
		else
			delete pTMAINTAIN;
	}

	pTRECALL->m_bACTLevel = CTChart::GetTACTION(pTRECALL->m_bAction)->m_bLevel;
	pTRECALL->m_nPITCH = INT(wPITCH) - INT(pTRECALL->m_wPITCH);
	pTRECALL->m_nDIR = INT(wDIR) - INT(pTRECALL->m_wDIR);

	pTRECALL->m_dwHostID = dwHostID;
	pTRECALL->m_dwID = dwMonID;

	BOOL bFakeChar = wTempID ? FALSE : TRUE;
	if(bFakeChar)
	{
		static_cast<CTClientFake *>(pTRECALL)->m_pHOST = pHOST;

		if(!pHOST)
		{
			m_mapFAKETEMP.insert( MAPRECALL::value_type( pTRECALL->m_dwID, pTRECALL));
			m_pSESSION->SendCS_OTHERSELF_REQ( dwHostID, dwMonID);

			return TERR_NONE;
		}
	}

	pTRECALL->InitRecall(
		m_pDevice,
		m_pRES,
		wTempID,
		pTRECALL->m_vTSKILLDATA.m_bLevel);

	pTRECALL->DoSkillFuncAll( this );

	m_mapRECALL.insert( MAPRECALL::value_type( pTRECALL->m_dwID, pTRECALL));
	m_vMAP.LandTOBJ(pTRECALL);

	if( pHOST == m_pMainChar && !FindSlaveRecall(pTRECALL->m_dwID) )
	{
		m_mapSLAVERECALL.insert( MAPRECALL::value_type( pTRECALL->m_dwID, pTRECALL));
		pTRECALL->m_bTPOS = m_pMainChar->GetFreeRecallPOS();

		if( pTRECALL->m_bTPOS < TRECALL_MAX )
			m_pMainChar->m_vTRECALL[pTRECALL->m_bTPOS] = TRUE;

		if( pTRECALL->m_bRecallType == TRECALLTYPE_MAIN )
			pTRECALL->m_bAI = m_pMainChar->m_bPrevMainRecallAI;

		ResetRecallINFO(FALSE);
	}

	CheckAutoSKILL(pTRECALL);

	if( bNewMember && (pTRECALL->m_bRecallType != TRECALLTYPE_PET || (
		pHOST && pHOST->m_wMoveDIR == MOVE_NONE)) )
		pTRECALL->ApplyTAction( m_pDevice, m_pRES, TA_SPAWN);

	if( bFakeChar && pHOST != m_pMainChar &&
		!m_pMainChar->IsAlliance(pHOST) &&
		!(m_pMainChar->m_wTDETECT & (1 << OT_PC)) )
	{
		MAPRECALL::iterator itTRECALL;

		for( itTRECALL = m_mapSLAVERECALL.begin(); itTRECALL != m_mapSLAVERECALL.end(); itTRECALL++)
			if( (*itTRECALL).second != pTRECALL &&
				(*itTRECALL).second->m_bTargetType == pHOST->m_bType &&
				(*itTRECALL).second->m_dwTargetID == pHOST->m_dwID )
			{
				(*itTRECALL).second->m_bTargetType = pTRECALL->m_bType;
				(*itTRECALL).second->m_dwTargetID = pTRECALL->m_dwID;
			}

		if( m_pTARGET == pHOST )
		{
			CTClientObjBase::m_mapTSELECTOBJ.clear();

			ResetTargetOBJ(pTRECALL);
			ResetTargetINFO(FALSE);
		}
	}

	if(wPetID)
	{
		CTClientPet *pTPET = static_cast<CTClientPet *>(pTRECALL);

		pTPET->SetPetInfo( strPetName, wPetID, CTime(0));
		pTPET->m_fBreathHeight = pTPET->GetAttrFLOAT(ID_BREATH_HEIGHT);
		pTPET->m_fJumpSpeed = pTPET->GetAttrFLOAT(ID_JUMP_SPEED);

		if( pHOST == m_pMainChar )
		{
			CTPetManageDlg *pPetDlg = static_cast<CTPetManageDlg *>(m_vTFRAME[TFRAME_PET_MANAGE]);
			INT nIdx = pPetDlg->FindPetByID(wPetID);

			if( nIdx != T_INVALID )
			{
				pPetDlg->UpdatePetInfo(nIdx);
				pPetDlg->SetRecalling(FALSE);
			}

			pTPET->m_bCanSelected = TRUE;

			// PET_RIDING_2.2 펫 타기
			if( m_pSESSION &&
				pTPET->m_dwID &&
				m_bPetUseByHotbar)
			{
				m_pSESSION->SendCS_PETRIDING_REQ(pTPET->m_dwID, TRUE);
				m_bPetUseByHotbar = FALSE;
			}
		}
		else
			pTPET->m_bCanSelected = FALSE;
	}

	if( GetPartyID(m_pMainChar) && IsRSCSEnemy(pTRECALL) )
	{
		pTRECALL->CalcDIR( 0, pTRECALL->m_bMouseDIR, pTRECALL->m_bKeyDIR);
		pTRECALL->CalcTick(
			&D3DXVECTOR3(
			pTRECALL->GetPositionX(),
			pTRECALL->GetPositionY(),
			pTRECALL->GetPositionZ()),
			m_pDevice,
			m_pRES, 0);

		RelaySession()->SendCS_ADDCORPSENEMY_REQ(
			m_pMainChar->m_dwID,
			m_vMAP.m_wMapID,
			pTRECALL);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DELRECALLMON_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwHostID;
	DWORD dwMonID;

	BYTE bExitMAP;

	(*pPacket)
		>> dwHostID
		>> dwMonID
		>> bExitMAP;

	CTClientRecall *pTRECALL = FindRecall(dwMonID);
	if( !pTRECALL )
		return TERR_NONE;

	if( pTRECALL->m_bRecallType == TRECALLTYPE_PET )
	{
		CTClientPet* pTPET = static_cast<CTClientPet*>(pTRECALL);
		CTClientChar* pTCHAR = pTPET->GetTakeUpChar();
		if( pTCHAR )
		{
			pTPET->TakeDown();

			if( dwHostID == m_pMainChar->m_dwID )
				pTPET->m_bCanSelected = TRUE;
		}
	}

	if(bExitMAP)
	{
		pTRECALL->m_dwHP = 0;

		if( pTRECALL && !pTRECALL->IsDead() )
			pTRECALL->OnDIE(m_pDevice, m_pRES);
	}
	else if(DeleteRecall(dwMonID))
		ResetRecallINFO(FALSE);

	return TERR_NONE;
}

int CTClientGame::OnCS_ADDSELFOBJ_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwRegionID;
	DWORD dwHostID;
	DWORD dwMonID;
	DWORD dwLifeTick;

	WORD wTempID;
	WORD wPITCH;
	WORD wDIR;

	BYTE bNewMember;
	BYTE bCount;

	(*pPacket)
		>> dwHostID
		>> dwMonID;

	if(FindFixRecall(dwMonID))
		return TERR_NONE;

	CTClientRecall *pTRECALL = new CTClientRecall();
	CTClientChar *pHOST = FindPC(dwHostID);

	pTRECALL->m_pHost = m_pMainWnd;
	pTRECALL->ReleaseData();
	pTRECALL->m_bType = OT_SELF;

	(*pPacket)
		>> wTempID
		>> pTRECALL->m_bContryID
		>> pTRECALL->m_bColorID
		>> pTRECALL->m_bLevel
		>> pTRECALL->m_dwMaxHP
		>> pTRECALL->m_dwHP
		>> pTRECALL->m_dwMaxMP
		>> pTRECALL->m_dwMP
		>> pTRECALL->m_vWorld._41
		>> pTRECALL->m_vWorld._42
		>> pTRECALL->m_vWorld._43
		>> wPITCH
		>> wDIR
		>> pTRECALL->m_bAction
		>> pTRECALL->m_bMode
		>> bNewMember
		>> dwRegionID
		>> pTRECALL->m_bRecallType
		>> pTRECALL->m_vTSKILLDATA.m_bHIT
		>> pTRECALL->m_vTSKILLDATA.m_bLevel
		>> pTRECALL->m_vTSKILLDATA.m_wAL
		>> pTRECALL->m_vTSKILLDATA.m_bAtkLevel
		>> pTRECALL->m_vTSKILLDATA.m_dwMinAP
		>> pTRECALL->m_vTSKILLDATA.m_dwMaxAP
		>> pTRECALL->m_vTSKILLDATA.m_dwMinMP
		>> pTRECALL->m_vTSKILLDATA.m_dwMaxMP
		>> dwLifeTick
		>> bCount;

	pTRECALL->m_dwEndLifeTick = ::GetTickCount() + dwLifeTick;
	pTRECALL->m_pTREGION = CTChart::FindTREGIONINFO( dwRegionID);

	for( BYTE i=0; i<bCount; i++)
	{
		CTClientMaintain *pTMAINTAIN = new CTClientMaintain();

		(*pPacket)
			>> pTMAINTAIN->m_vTSKILLDATA.m_wID
			>> pTMAINTAIN->m_vTSKILLDATA.m_bLevel
			>> pTMAINTAIN->m_dwTick
			>> pTMAINTAIN->m_dwAttackID
			>> pTMAINTAIN->m_bAttackType
			>> pTMAINTAIN->m_dwHostID
			>> pTMAINTAIN->m_bHostType
			>> pTMAINTAIN->m_vTSKILLDATA.m_bHIT
			>> pTMAINTAIN->m_vTSKILLDATA.m_wAL
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkLevel
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxAP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMinMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_dwMaxMP
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAglow
			>> pTMAINTAIN->m_vTSKILLDATA.m_bAtkCountryID
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.x
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.y
			>> pTMAINTAIN->m_vTSKILLDATA.m_vTGROUND.z;
		pTMAINTAIN->m_pTSKILL = CTChart::FindTSKILLTEMP(pTMAINTAIN->m_vTSKILLDATA.m_wID);

		if(pTMAINTAIN->m_pTSKILL)
		{
			pTMAINTAIN->m_pTIMMER = CTClientSkill::FindTIMMER(pTMAINTAIN->m_pTSKILL);
			pTMAINTAIN->m_bLevel = pTMAINTAIN->m_vTSKILLDATA.m_bLevel;

			pTMAINTAIN->m_bUnlimitedType = pTMAINTAIN->m_pTSKILL->m_dwDuration ? FALSE : TRUE;
			pTMAINTAIN->m_dwEndTick = pTMAINTAIN->m_dwTick + m_vKEY.m_dwTick;

			pTRECALL->InsertMaintain( pTMAINTAIN->m_vTSKILLDATA.m_wID, pTMAINTAIN);
			pTRECALL->PlayTActionSFX(
				&pTMAINTAIN->m_vTSFX,
				pTMAINTAIN->m_pTSKILL->m_dwSFX[TSKILLSFX_LOOP]);
		}
		else
			delete pTMAINTAIN;
	}

	pTRECALL->m_bACTLevel = CTChart::GetTACTION(pTRECALL->m_bAction)->m_bLevel;
	pTRECALL->m_nPITCH = INT(wPITCH) - INT(pTRECALL->m_wPITCH);
	pTRECALL->m_nDIR = INT(wDIR) - INT(pTRECALL->m_wDIR);

	pTRECALL->m_dwHostID = dwHostID;
	pTRECALL->m_dwID = dwMonID;

	pTRECALL->InitRecall(
		m_pDevice,
		m_pRES,
		wTempID,
		pTRECALL->m_vTSKILLDATA.m_bLevel);
	
	pTRECALL->DoSkillFuncAll( this );

	m_mapFIXRECALL.insert( MAPRECALL::value_type( pTRECALL->m_dwID, pTRECALL));
	m_vMAP.LandTOBJ(pTRECALL);

	if( pHOST == m_pMainChar && !FindSlaveFixRecall(pTRECALL->m_dwID) )
		m_mapSLAVEFIXRECALL.insert( MAPRECALL::value_type( pTRECALL->m_dwID, pTRECALL));
	CheckAutoSKILL(pTRECALL);

	if(bNewMember)
		pTRECALL->ApplyTAction( m_pDevice, m_pRES, TA_SPAWN);

	if( GetPartyID(m_pMainChar) && IsRSCSEnemy(pTRECALL) )
	{
		pTRECALL->CalcDIR( 0, pTRECALL->m_bMouseDIR, pTRECALL->m_bKeyDIR);
		pTRECALL->CalcTick(
			&D3DXVECTOR3(
			pTRECALL->GetPositionX(),
			pTRECALL->GetPositionY(),
			pTRECALL->GetPositionZ()),
			m_pDevice,
			m_pRES, 0);

		RelaySession()->SendCS_ADDCORPSENEMY_REQ(
			m_pMainChar->m_dwID,
			m_vMAP.m_wMapID,
			pTRECALL);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DELSELFOBJ_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwMonID;
	BYTE bExitMAP;

	(*pPacket)
		>> dwMonID
		>> bExitMAP;
	DeleteFixRecall(dwMonID);

	return TERR_NONE;
}

int CTClientGame::OnCS_OTHERSELF_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwMonID;

	(*pPacket)
		>> dwMonID;

	MAPRECALL::iterator fitr = m_mapFAKETEMP.find(dwMonID);
	if( fitr == m_mapFAKETEMP.end() )
		return TERR_NONE;

	CTClientFake* pTFAKE = static_cast<CTClientFake*>( fitr->second );
	m_mapFAKETEMP.erase(fitr);

	DWORD dwCharID;

	(*pPacket)
		>> dwCharID;

	BOOL bHostMake = FALSE;
	CTClientChar* pHOST = FindPlayer(dwCharID);

	DWORD dwGuildMark;
	DWORD dwGuildColor;

	if( !pHOST )
	{
		bHostMake = TRUE;
		
		pHOST = new CTClientChar();
		pHOST->m_pHost = m_pMainWnd;
		pHOST->ReleaseData();

		(*pPacket)
			>> pHOST->m_strNAME
			>> pHOST->m_dwGuildID
			>> dwGuildMark
			>> dwGuildColor
			>> pHOST->m_strGUILD
			>> pHOST->m_bGuildPeer
			>> pHOST->m_dwTacticsID
			>> pHOST->m_strTACTICS
			>> pHOST->m_bClassID
			>> pHOST->m_bRaceID_
			>> pHOST->m_bContryID
			>> pHOST->m_bSex
			>> pHOST->m_bHair
			>> pHOST->m_bFace
			>> pHOST->m_bBody
			>> pHOST->m_bPants
			>> pHOST->m_bHand
			>> pHOST->m_bFoot
			>> pHOST->m_bHelmetHide
			>> pHOST->m_bLevel
			>> pHOST->m_dwChiefID
			>> pHOST->m_wPartyID
			>> pHOST->m_wCommanderID;
		
		if( pHOST->m_dwGuildID && dwGuildMark )
		{
			pHOST->m_pGuildMark = new CTClientGuildMark(
				TGMARK_MARK(dwGuildMark), TGMARK_BACK(dwGuildMark),
				TGMCOLOR_MARK(dwGuildColor), TGMCOLOR_BACK1(dwGuildColor),
				TGMCOLOR_BACK2(dwGuildColor) );
		}

		pHOST->InitOBJ(m_pRES->GetOBJ( CTChart::m_vTRACE[pHOST->GetRaceID()][pHOST->m_bSex]));

		CTClientInven *pTEQUIP = new CTClientInven();
		pTEQUIP->m_bInvenID = INVEN_EQUIP;
		pTEQUIP->m_wItemID = 0;
		
		BYTE bCount;
		(*pPacket) 
			>> bCount;

		for( BYTE i=0; i<bCount; ++i )
		{
			CTClientItem *pTITEM = new CTClientItem();
			
			pTITEM->SetItemSlot(pPacket);
			pTITEM->SetItemData(pPacket);
			pTITEM->SetItemMagic(pPacket);

			if( !pTITEM->CheckValid() )
			{
				delete pTITEM;
				continue;
			}

			pTEQUIP->m_mapTITEM.insert( make_pair( pTITEM->GetItemSlot(), pTITEM) );
		}

		pHOST->m_mapTINVEN.insert( MAPTINVEN::value_type( pTEQUIP->m_bInvenID, pTEQUIP));
		pHOST->InitSIZE(
			pHOST->GetAttrFLOAT(ID_SIZE_X),
			pHOST->GetAttrFLOAT(ID_SIZE_Y),
			pHOST->GetAttrFLOAT(ID_SIZE_Z),
			0.0f, 0.0f);
	}

	pTFAKE->m_pHOST = pHOST;
	pTFAKE->InitRecall(m_pDevice,m_pRES,0,0);
	pTFAKE->DoSkillFuncAll( this );
	
	m_mapRECALL.insert( MAPRECALL::value_type( pTFAKE->m_dwID, pTFAKE));
	m_vMAP.LandTOBJ(pTFAKE);
	CheckAutoSKILL(pTFAKE);

	if( GetPartyID(m_pMainChar) && IsRSCSEnemy(pTFAKE) )
	{
		pTFAKE->CalcDIR( 0, pTFAKE->m_bMouseDIR, pTFAKE->m_bKeyDIR);
		pTFAKE->CalcTick(
			&D3DXVECTOR3(
			pTFAKE->GetPositionX(),
			pTFAKE->GetPositionY(),
			pTFAKE->GetPositionZ()),
			m_pDevice,
			m_pRES, 0);

		RelaySession()->SendCS_ADDCORPSENEMY_REQ(
			m_pMainChar->m_dwID,
			m_vMAP.m_wMapID,
			pTFAKE);
	}

	if( pHOST != m_pMainChar && m_pTARGET == pHOST &&
		!m_pMainChar->IsAlliance(pHOST) && !(m_pMainChar->m_wTDETECT & (1 << OT_PC)) )
	{
		CTClientObjBase::m_mapTSELECTOBJ.clear();

		ResetTargetOBJ(pTFAKE);
		ResetTargetINFO(FALSE);
	}

	if( bHostMake )
	{
		pTFAKE->m_pHOST = NULL;
		delete pHOST;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SETRETURNPOS_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	if(bResult)
	{
		CString strTYPE;
		CString strMSG;

		strMSG = CTChart::LoadString( TSTR_SET_RETURNPOS);
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, TCOLOR_INFO, TCHAT_FLAG_INFO);
	}
	else
	{
		CString strTYPE;
		CString strMSG;

		strMSG = CTChart::LoadString( TSTR_ERROR_SET_RETURNPOS);
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, TCOLOR_ERROR, TCHAT_FLAG_INFO);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_WARP_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwObjID;
	DWORD dwRange;
	BYTE bObjType;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;

	(*pPacket)
		>> dwObjID
		>> bObjType
		>> dwRange
		>> fPosX
		>> fPosY
		>> fPosZ;

	CTClientMoveObj *pTOBJ = NULL;
	switch(bObjType)
	{
	case OT_RECALL	: pTOBJ = FindSlaveRecall(dwObjID); break;
	case OT_SELF	: pTOBJ = FindSlaveFixRecall(dwObjID); break;
	case OT_MON		: pTOBJ = FindSlave(dwObjID); break;
	case OT_PC		:
		if( FindPC(dwObjID) == m_pMainChar )
			pTOBJ = m_pMainChar;

		break;
	}

	if(pTOBJ)
	{
		if(dwRange)
		{
			GetRandomPosition(
				fPosX,
				fPosY,
				fPosZ, 
				pTOBJ,
				(FLOAT)dwRange, 
				TMAX_HEIGHT_OFFSET);
		}

		D3DXVECTOR3 vMOVE(
			fPosX - pTOBJ->GetPositionX(),
			fPosY - pTOBJ->GetPositionY(),
			fPosZ - pTOBJ->GetPositionZ());

		pTOBJ->AddPositionX(vMOVE.x);
		pTOBJ->AddPositionY(vMOVE.y);
		pTOBJ->AddPositionZ(vMOVE.z);

		pTOBJ->m_vWorld._41 += vMOVE.x;
		pTOBJ->m_vWorld._42 += vMOVE.y;
		pTOBJ->m_vWorld._43 += vMOVE.z;

		switch(bObjType)
		{
		case OT_RECALL	:
		case OT_SELF	:
		case OT_MON		:
			{
				SendCS_MONMOVE_REQ(
					pTOBJ,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					pTOBJ->GetPositionX(),
					pTOBJ->GetPositionY(),
					pTOBJ->GetPositionZ(),
					pTOBJ->m_wPITCH,
					pTOBJ->m_wDIR,
					pTOBJ->m_bMouseDIR,
					pTOBJ->m_bKeyDIR,
					pTOBJ->m_bAction);
			}

			break;

		case OT_PC		: m_dwSYNCTick = SYNC_TICK; break;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_TELEPORT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwObjID;
	DWORD dwRange;
	BYTE bObjType;
	BYTE bResult;

	FLOAT fPosX;
	FLOAT fPosY;
	FLOAT fPosZ;
	WORD wMapID;

	(*pPacket)
		>> bResult
		>> dwObjID
		>> bObjType
		>> dwRange
		>> wMapID
		>> fPosX
		>> fPosY
		>> fPosZ;

	CTClientMoveObj *pTOBJ = NULL;
	switch(bObjType)
	{
	case OT_RECALL	: pTOBJ = FindSlaveRecall(dwObjID); break;
	case OT_SELF	: pTOBJ = FindSlaveFixRecall(dwObjID); break;
	case OT_MON		: pTOBJ = FindSlave(dwObjID); break;
	case OT_PC		:
		if( FindPC(dwObjID) == m_pMainChar )
		{
			pTOBJ = m_pMainChar;

			if( wMapID == TTOURNAMENT_LOUNGE_ID )
			{
				CTGaugePannel *pDLG = static_cast<CTGaugePannel *>(m_vTFRAME[TFRAME_GAUGE]);

				pDLG->m_pTournamentInfo->SetInfoTMode(
					NULL,
					NULL,
					TRUE,
					CTChart::LoadString(TSTR_TOURNAMENT_LOBBY));
			}
		}

		break;
	}

	if( bResult != TPR_SUCCESS )
	{
		if( pTOBJ == m_pMainChar )
		{
			m_bTELEPORT = FALSE;
			m_bLoopLock = FALSE;
			m_wNextSkillID = 0;
		}

		if( bResult == TPR_NEEDMONEY )
			return TERR_ITEM_NO_MONEY;
		else
			return TERR_NONE;
	}

	if(pTOBJ)
	{
		if( wMapID == m_vMAP.m_wMapID )
		{
			if(dwRange)
			{
				GetRandomPosition(
					fPosX,
					fPosY,
					fPosZ, 
					pTOBJ,
					(FLOAT)dwRange, 
					TMAX_HEIGHT_OFFSET);
			}

			D3DXVECTOR3 vMOVE(
				fPosX - pTOBJ->GetPositionX(),
				fPosY - pTOBJ->GetPositionY(),
				fPosZ - pTOBJ->GetPositionZ());

			pTOBJ->AddPositionX(vMOVE.x);
			pTOBJ->AddPositionY(vMOVE.y);
			pTOBJ->AddPositionZ(vMOVE.z);

			pTOBJ->m_vWorld._41 += vMOVE.x;
			pTOBJ->m_vWorld._42 += vMOVE.y;
			pTOBJ->m_vWorld._43 += vMOVE.z;

			if( pTOBJ == m_pMainChar )
			{
				m_pMainWnd->m_bResetTick = TRUE;
				ReleaseForTELEPORT();

				m_vMAP.ResetTMAP(
					m_pDevice,
					m_pCamera,
					0, TRUE);

				if(!m_pMainChar->m_dwHP)
				{
					m_dwGHOSTTick = 0;
					m_bGHOST = TRUE;

					OnGM_GOTO_THUMB();
				}
				else
				{
					m_dwGHOSTTick = TGHOST_TICK;
					m_bGHOST = FALSE;
				}

				m_bTELEPORT = FALSE;
				m_bLoopLock = FALSE;
				m_wNextSkillID = 0;
				ResetTSKY();

				if(GetPartyID(m_pMainChar))
					m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;

				m_vKEY.ResetSTATE();
			}

			m_vMAP.FixTOBJHeight(pTOBJ);
			pTOBJ->m_fJumpHeight = pTOBJ->m_vWorld._42;
			pTOBJ->m_fFallHeight = pTOBJ->m_vWorld._42;
		}
		else if( pTOBJ == m_pMainChar )
		{
			m_vMAP.ReleaseMAP();
			m_vMAP.m_wMapID = wMapID;

			m_pMainChar->m_vWorld._41 = fPosX;
			m_pMainChar->m_vWorld._42 = fPosY;
			m_pMainChar->m_vWorld._43 = fPosZ;
			m_pMainWnd->m_bResetTick = TRUE;

			if( TTOURNAMENT_STADIUM_1 <= wMapID && wMapID <= TTOURNAMENT_STADIUM_32 )
			{
				CTachyonHUGEMAP::m_nMaxLength = TTOURNAMENT_STADIUM_MAXLENGTH;
				CTachyonHUGEMAP::m_nLODBound = TTOURNAMENT_STADIUM_LODBOUND;
			}
			else
			{
				CTachyonHUGEMAP::m_nLODBound = m_pMainWnd->m_Device.m_vCAPS.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT ? DEF_LODBOUND : TSWVP_DEF_LODBOUND;
				CTachyonHUGEMAP::m_nMaxLength = DEF_MAXLENGTH;
			}

			ReleaseForTELEPORT();
			m_vMAP.LoadMAP(
				m_pDevice,
				m_pCamera,
				m_pRES,
				m_vMAP.m_wMapID);

			if(!m_pMainChar->m_dwHP)
			{
				m_dwGHOSTTick = 0;
				m_bGHOST = TRUE;

				OnGM_GOTO_THUMB();
			}
			else
			{
				m_dwGHOSTTick = TGHOST_TICK;
				m_bGHOST = FALSE;
			}

			m_vMAP.FixTOBJHeight(m_pMainChar);
			m_pMainChar->m_fJumpHeight = m_pMainChar->m_vWorld._42;
			m_pMainChar->m_fFallHeight = m_pMainChar->m_vWorld._42;

			m_bTELEPORT = FALSE;
			m_bLoopLock = FALSE;
			m_wNextSkillID = 0;
			ResetTSKY();

			WORD wSquadID = GetPartyID(m_pMainChar);
			if(wSquadID)
			{
				LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
					m_pTRSCS->FindTSQUAD(wSquadID),
					m_pMainChar->m_dwID);

				m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
				if(pTUNIT)
				{
					pTUNIT->m_bTCMD = TRSCSCMD_NONE;
					pTUNIT->m_bTargetType = OT_NONE;
					pTUNIT->m_dwTargetID = 0;
					pTUNIT->m_wTargetX = 0;
					pTUNIT->m_wTargetZ = 0;

					ResetTRSCSCMD(
						&m_vTCMDTARGET,
						&m_vTCMDDIR,
						&m_vTCMDARROW,
						TRSCSCMD_NONE,
						0, 0, 0, 0);
					RefreshTRSCSCMD();

					RelaySession()->SendCS_CORPSCMD_REQ(
						wSquadID,
						m_pMainChar->m_dwID,
						0xFFFF,
						TRSCSCMD_NONE,
						0, 0, 0, 0);
				}
			}
			m_vKEY.ResetSTATE();

			if( m_bTournamentWatching || m_bTournamentBattle )
			{
				ResetTournamentWatching();

				m_bTournamentWatching = FALSE;
				m_bTournamentBattle = FALSE;

				DisableUI(TFRAME_TOURNAMENT_RESULT);
			}

			if(m_bFirstConnect)
			{
				m_pMainChar->m_nDIR = INT(m_pMainChar->m_bContryID == COUNTRY_DEFUGEL ? TSTART_D_CHAR_DIR : TSTART_C_CHAR_DIR) - m_pMainChar->m_wDIR;
				m_bFirstConnect = FALSE;

				if( m_vMAP.m_wMapID != TUTORIAL_MAPID )
					m_vTSCENE_PLAYER.StartScene( m_pDevice, m_pRES, TSCENE_SCRIPT_ENTER_WORLD);
			}
		}

		switch(bObjType)
		{
		case OT_RECALL	:
		case OT_SELF	:
		case OT_MON		:
			{
				SendCS_MONMOVE_REQ(
					pTOBJ,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					pTOBJ->GetPositionX(),
					pTOBJ->GetPositionY(),
					pTOBJ->GetPositionZ(),
					pTOBJ->m_wPITCH,
					pTOBJ->m_wDIR,
					pTOBJ->m_bMouseDIR,
					pTOBJ->m_bKeyDIR,
					pTOBJ->m_bAction);
			}

			break;

		case OT_PC:
			{
				m_dwSYNCTick = SYNC_TICK;

				if( pTOBJ == m_pMainChar )
					CheckValidHeightInfo();
			}
			break;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_BEGINTELEPORT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wMapID = (WORD) -1;

	(*pPacket)
		>> m_pMainWnd->m_bChannel
		>> wMapID;

	for( int i=0; i<INT(m_pMainWnd->m_vTCHANNELS.size()); i++)
		if( m_pMainWnd->m_bChannel == m_pMainWnd->m_vTCHANNELS[i].bID )
		{
			m_pMainWnd->m_bChannelNumber = (BYTE) i;
			break;
		}

	CTGaugePannel *pTGaugePannel = (CTGaugePannel *) m_vTFRAME[TFRAME_GAUGE];
	pTGaugePannel->m_pLUCKY2->m_strText = CTChart::Format( TSTR_CHANNEL, m_pMainWnd->m_bChannel);

	m_pMainWnd->m_strLOADMSG = CTChart::LoadString(TSTR_LOADMSG_LOADDATA);
	m_vTSCENE_PLAYER.Release();

	m_pMainWnd->LoadingIMG();
	m_pMainWnd->Render();

	return TERR_NONE;
}

int CTClientGame::OnCS_INVENMOVE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	BYTE bDESTInvenID;
	BYTE bSRCInvenID;

	(*pPacket)
		>> bResult
		>> bSRCInvenID
		>> bDESTInvenID;

	switch(bResult)
	{
	case INVEN_SUCCESS	:
		{
			CTClientInven *pTDEST = m_pMainChar->FindTInven(bDESTInvenID);
			CTClientInven *pTSRC = m_pMainChar->FindTInven(bSRCInvenID);

			m_pMainChar->m_mapTINVEN.erase(bDESTInvenID);
			m_pMainChar->m_mapTINVEN.erase(bSRCInvenID);

			if(pTDEST)
			{
				pTDEST->m_bInvenID = bSRCInvenID;
				m_pMainChar->m_mapTINVEN.insert( MAPTINVEN::value_type( pTDEST->m_bInvenID, pTDEST));
			}

			if(pTSRC)
			{
				pTSRC->m_bInvenID = bDESTInvenID;
				m_pMainChar->m_mapTINVEN.insert( MAPTINVEN::value_type( pTSRC->m_bInvenID, pTSRC));
			}
		}

		break;
	}

	ResetInvenUI();
	CancelAllItemUI();
	EnableUI(TFRAME_CHAR_INFO);

	return TERR_NONE;
}

int CTClientGame::OnCS_INVENADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bInvenID;
	WORD wItemID;
	__time64_t dEndTime;

	(*pPacket)
		>> bResult
		>> bInvenID
		>> wItemID
		>> dEndTime;

	switch(bResult)
	{
	case INVEN_SUCCESS	:
		if(!m_pMainChar->FindTInven(bInvenID))
		{
			CTClientInven *pTINVEN = new CTClientInven();

			pTINVEN->m_bInvenID = bInvenID;
			pTINVEN->m_wItemID = wItemID;
			pTINVEN->m_dEndTime = dEndTime;
			pTINVEN->m_pTITEM = CTChart::FindTITEMTEMP(wItemID);

			m_pMainChar->m_mapTINVEN.insert( MAPTINVEN::value_type( pTINVEN->m_bInvenID, pTINVEN));
		}

		break;

	case INVEN_LEVEL	:
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_ERROR_ITEM_LEVEL,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0 );
		}

		break;
		
	}

	ResetInvenUI();
	CancelAllItemUI();
	EnableUI(TFRAME_CHAR_INFO);

	return TERR_NONE;
}

int CTClientGame::OnCS_INVENDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bInvenID;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> bInvenID;

	switch(bResult)
	{
	case INVEN_SUCCESS	:
		{
			MAPTINVEN::iterator finder = m_pMainChar->m_mapTINVEN.find(bInvenID);

			if( finder != m_pMainChar->m_mapTINVEN.end() )
			{
				delete (*finder).second;
				m_pMainChar->m_mapTINVEN.erase(finder);
			}
		}

		break;
	}

	ResetInvenUI();
	CancelAllItemUI();
	EnableUI(TFRAME_CHAR_INFO);

	return TERR_NONE;
}

int CTClientGame::OnCS_SWITCHCHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bOPEN;

	DWORD dwID;

	(*pPacket)
		>> bResult
		>> dwID
		>> bOPEN;

	switch(bResult)
	{
	case SWITCH_SUCCESS	:
		{
			CTClientSwitch *pTSWITCH = FindSwitch(dwID);
			BYTE bCHANGE = TRUE;

			if(!pTSWITCH)
			{
				pTSWITCH = new CTClientSwitch();
				pTSWITCH->ReleaseData();

				pTSWITCH->m_pTSWITCH = CTChart::FindTSWITCH(dwID);
				pTSWITCH->m_bOPEN = bOPEN;
				pTSWITCH->InitTSWITCH(
					m_pDevice,
					m_pRES);

				m_mapTSwitch.insert( MAPTSWITCH::value_type( dwID, pTSWITCH));
			}
			else
			{
				bCHANGE = pTSWITCH->m_bOPEN != bOPEN ? TRUE : FALSE;
				pTSWITCH->TurnON(bOPEN);
			}

			if( bCHANGE && pTSWITCH && pTSWITCH->m_pTSWITCH->m_dwSoundID )
			{
				pTSWITCH->PlaySND(
					pTSWITCH->m_pTSWITCH->m_dwSoundID,
					MEDIA_TSOUND,
					TRUE);
			}
		}

		break;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SWITCHADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;
	BYTE bOPEN;

	(*pPacket)
		>> dwID
		>> bOPEN;

	LPTSWITCH pTSwitchTemp = CTChart::FindTSWITCH(dwID);
	if( !pTSwitchTemp || !pTSwitchTemp->m_dwOBJ )
		return TERR_NONE;

	CTClientSwitch *pTSWITCH = FindSwitch(dwID);
	if(!pTSWITCH)
	{
		pTSWITCH = new CTClientSwitch();
		pTSWITCH->ReleaseData();

		pTSWITCH->m_pTSWITCH = pTSwitchTemp;
		pTSWITCH->m_bOPEN = bOPEN;
		pTSWITCH->InitTSWITCH(
			m_pDevice,
			m_pRES);

		m_mapTSwitch.insert( MAPTSWITCH::value_type( dwID, pTSWITCH));
	}
	else
		pTSWITCH->TurnON(bOPEN);

	return TERR_NONE;
}

int CTClientGame::OnCS_SWITCHDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;

	(*pPacket)
		>> dwID;

	MAPTSWITCH::iterator finder = m_mapTSwitch.find(dwID);
	if( finder != m_mapTSwitch.end() )
	{
		DeleteDynimicOBJ((*finder).second);
		m_mapTSwitch.erase(finder);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GATECHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;
	BYTE bOPEN;

	(*pPacket)
		>> dwID
		>> bOPEN;

	CTClientGate *pTGATE = FindGate(dwID);
	if(!pTGATE)
	{
		pTGATE = new CTClientGate();
		pTGATE->ReleaseData();

		pTGATE->m_pTGATE = CTChart::FindTGATE(dwID);
		pTGATE->m_bOPEN = !bOPEN;
		pTGATE->InitTGate(
			m_pDevice,
			m_pRES);
		pTGATE->m_pHost = m_pMainWnd;

		m_vMAP.m_mapTGate.insert( MAPTGATE::value_type( dwID, pTGATE));
	}

	if( pTGATE->m_bOPEN != bOPEN )
	{
		DWORD dwSFX = bOPEN ? pTGATE->m_pTGATE->m_dwOpenSFX : pTGATE->m_pTGATE->m_dwCloseSFX;

		if(dwSFX)
		{
			pTGATE->PlayTActionSFX(
				&pTGATE->m_vTGARBAGESFX,
				dwSFX);
		}
	}
	pTGATE->OpenTGate(bOPEN);

	return TERR_NONE;
}

int CTClientGame::OnCS_GATEADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;
	BYTE bOPEN;

	(*pPacket)
		>> dwID
		>> bOPEN;

	CTClientGate *pTGATE = FindGate(dwID);
	if(!pTGATE)
	{
		pTGATE = new CTClientGate();
		pTGATE->ReleaseData();

		pTGATE->m_pTGATE = CTChart::FindTGATE(dwID);
		pTGATE->m_bOPEN = bOPEN;
		pTGATE->InitTGate(
			m_pDevice,
			m_pRES);
		pTGATE->m_pHost = m_pMainWnd;

		if( pTGATE->m_bCollisionType == TCOLLISION_POLY )
			pTGATE->InitCollision();

		m_vMAP.m_mapTGate.insert( MAPTGATE::value_type( dwID, pTGATE));
	}
	else
	{
		pTGATE->m_bOPEN = bOPEN;
		pTGATE->ResetTGate();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GATEDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID;

	(*pPacket)
		>> dwID;

	MAPTGATE::iterator finder = m_vMAP.m_mapTGate.find(dwID);
	if( finder != m_vMAP.m_mapTGate.end() )
	{
		DeleteDynimicOBJ((*finder).second);
		m_vMAP.m_mapTGate.erase(finder);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DROPDAMAGE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwDamage;
	DWORD dwID;
	BYTE bType;

	(*pPacket)
		>> dwID
		>> bType
		>> dwDamage;

	CTClientObjBase *pTOBJ = FindOBJ(
		dwID,
		bType);

	if( pTOBJ && !m_pMainChar->m_bGhost && dwDamage )
	{
		pTOBJ->m_vTDAMAGE.push_back(MAKE_TDAMAGE( dwDamage, MTYPE_DAMAGE));

		if( pTOBJ == m_pMainChar )
		{
			CString strTYPE;
			CString strMSG;

			strMSG = CTChart::Format( TSTR_FMT_DEFENDM, dwDamage);
			strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);

			m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, TCOLOR_DEFEND, TCHAT_FLAG_INFO);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DEALITEMSTART_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTTradeDlg *pTTradeDlg = (CTTradeDlg *) m_vTFRAME[TFRAME_TRADE];

	pTTradeDlg->Clear(TRUE);
	pTTradeDlg->Clear(FALSE);

	pTTradeDlg->SetMyMask(FALSE);
	pTTradeDlg->SetTargetMask(FALSE);

	CString strTarget;
	
	(*pPacket) 
		>> strTarget;

	pTTradeDlg->SetTargetName(strTarget);

	m_pMainWnd->CloseMessageBox();
	EnableUI(TFRAME_TRADE);

	return TERR_NONE;
}

int CTClientGame::OnCS_DEALITEMASK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strMSG;

	(*pPacket)
		>> m_strReqNAME;

	if( IsBusy() )
	{
		m_pSESSION->SendCS_DEALITEMRLY_REQ(
			ASK_BUSY,
			m_strReqNAME);
	}
	else
	{
		strMSG = CTChart::Format( TSTR_FMT_TRADE_REQ, m_strReqNAME);
		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_TRADE_YES,
			GM_TRADE_NO,
			GM_TRADE_BUSY,
			FALSE );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DEALITEMADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTTradeDlg *pTTradeDlg = (CTTradeDlg *) m_vTFRAME[TFRAME_TRADE];

	DWORD dwRune;
	DWORD dwLuna;
	DWORD dwCron;
	BYTE bCount;

	pTTradeDlg->Clear(TRUE);

	(*pPacket)
		>> dwRune
		>> dwLuna
		>> dwCron
		>> bCount;

	pTTradeDlg->SetTargetMoney(dwRune, dwLuna, dwCron);

	for( BYTE i=0; i<bCount; ++i )
	{
		CTTradeItem* pInfo = pTTradeDlg->AddEmptyItem(TRUE);

		pInfo->m_pClientItem->SetItemData(pPacket);
		pInfo->m_pClientItem->SetItemMagic(pPacket);

		if( !pInfo->m_pClientItem->CheckValid() )
			pTTradeDlg->RemoveItem(TRUE, pInfo->m_nIndex);
	}

	pTTradeDlg->SetTargetMask(TRUE);

	return TERR_NONE;
}

int CTClientGame::OnCS_DEALITEMEND_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strNAME;
	BYTE bResult;

	(*pPacket)
		>> bResult
		>> strNAME;

	CTTradeDlg *pTTradeDlg = (CTTradeDlg *) m_vTFRAME[TFRAME_TRADE];

	pTTradeDlg->SetTargetMask( FALSE );
	pTTradeDlg->SetMyMask( FALSE );

	DisableUI(TFRAME_TRADE);
	((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();
	ResetInvenUI();
	TradeMSG( strNAME, bResult);

	return TERR_NONE;
}

int CTClientGame::OnCS_STOREOPEN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwCharID;
	CString strName;
	
	(*pPacket) 
		>> bResult
		>> dwCharID
		>> strName;

	if( bResult == STORE_SUCCESS )
	{
		CTClientChar* pChar;
		if( m_pMainChar->m_dwID == dwCharID )
		{
			pChar = m_pMainChar;

			CTPrivateShopDlgForSell* pShopDlg = static_cast<CTPrivateShopDlgForSell*>(m_vTFRAME[TFRAME_PRIVATE_SELL]);

			pShopDlg->SetShopName(strName);
			pShopDlg->StartSelling(TRUE);

			DisableUI(TFRAME_DEFINVEN);
			EnableAllSubInven(FALSE);
		}
		else
			pChar = FindPlayer(dwCharID);
			
		if( pChar )
		{
			pChar->m_bPrivateShop = TRUE;
			pChar->m_strPrivateShop = strName;
		}
	}
	else
	{
		return TERR_FAILED_STOREOPEN;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_STORECLOSE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwCharID;

	(*pPacket) 
		>> bResult
		>> dwCharID;

	if( bResult == STORE_SUCCESS )
	{
		CTClientChar* pChar;
		if( m_pMainChar->m_dwID == dwCharID )
		{
			pChar = m_pMainChar;

			CTPrivateShopDlgForSell* pShopDlg = static_cast<CTPrivateShopDlgForSell*>(m_vTFRAME[TFRAME_PRIVATE_SELL]);
			pShopDlg->StartSelling(FALSE);

			EnableUI(TFRAME_DEFINVEN);
			EnableAllSubInven(TRUE);
		}
		else
		{
			pChar = FindPlayer(dwCharID);
			DisableUI(TFRAME_PRIVATE_BUY);
		}
			
		if( pChar )
		{
			pChar->m_bPrivateShop = FALSE;
			pChar->m_strPrivateShop.Empty();
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_STOREITEMLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	CString strName;
	BYTE bCount;
	BYTE bMagicCount;
	
	(*pPacket) 
		>> dwCharID
		>> strName
		>> bCount;

	if( m_pMainChar->m_dwID == dwCharID )
	{
		CTPrivateShopDlg* pShopDlg = static_cast<CTPrivateShopDlg*>(m_vTFRAME[TFRAME_PRIVATE_SELL]);
		if( !pShopDlg->IsVisible() || pShopDlg->GetItemCount() != bCount )
			return TERR_NONE;
	
		for(BYTE i=0; i<bCount; ++i)
		{
			CTPrivateShopItem* pItemInfo = pShopDlg->GetItem(i);	

			DWORD dwMoney;
			BYTE bItemCnt;
			WORD wItemID;
			BYTE bGrade;
			DWORD dwDuraMax;
			DWORD dwDuraCurrent;
			BYTE bRefineMax;
			BYTE bRefineCurrent;
			BYTE bCanGamble;
			__time64_t dEndTime;
			BYTE bGradeEffect;
			BYTE bWrap = 0;
			BYTE bELD = 0;
			WORD wColor = 0;
			BYTE bRegGuild = 0;

			(*pPacket) 
				>> pItemInfo->m_bNetID
				>> dwMoney
				>> dwMoney
				>> dwMoney
				>> wItemID
				>> bGrade
				>> bItemCnt
				>> dwDuraMax
				>> dwDuraCurrent
				>> bRefineMax
				>> bRefineCurrent
				>> bCanGamble
				>> dEndTime
				>> bGradeEffect
				>> bELD
				>> bWrap
				>> wColor
				>> bRegGuild
				>> bMagicCount;

			for(BYTE j=0; j<bMagicCount; ++j)
			{
				BYTE bMagic;
				WORD wValue;

				(*pPacket) 
					>> bMagic
					>> wValue;
			}
		}
	}
	else
	{
		CTPrivateShopDlg* pShopDlg = static_cast<CTPrivateShopDlg*>(m_vTFRAME[TFRAME_PRIVATE_BUY]);
		CTPrivateShopItem* pSelItem = pShopDlg->GetCurSelItem();
		INT nSelIdx = pSelItem? pSelItem->m_nIndex: T_INVALID;

		pShopDlg->Clear();
		pShopDlg->SetTitle(strName);
		
		for(BYTE i=0; i<bCount; ++i)
		{
			CTPrivateShopItem* pItemInfo = pShopDlg->AddEmptyItem();
			CTClientItem* pClientItem = pItemInfo->m_pClientItem;

			(*pPacket) 
				>> pItemInfo->m_bNetID
				>> pItemInfo->m_dwRune
				>> pItemInfo->m_dwLuna
				>> pItemInfo->m_dwCron;

			pClientItem->SetItemData(pPacket);
			pItemInfo->m_bItemCnt = pClientItem->GetCount();
			pClientItem->SetItemMagic(pPacket);

			if( !pClientItem->CheckValid() )
				pShopDlg->RemoveItem(pItemInfo->m_nIndex);
		}

		if( nSelIdx != T_INVALID && nSelIdx < bCount )
			pShopDlg->GetList()->Select(nSelIdx);
		
		EnableUI(TFRAME_PRIVATE_BUY);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_STOREITEMBUY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	WORD wItemID;
	BYTE bCount;

	(*pPacket) 
		>> bResult
		>> wItemID
		>> bCount;

	CString strCHATMSG(_T(""));
	CString strBOXMSG(_T(""));

	DWORD dwSound = ID_SND_ERROR;
	if( bResult == STORE_SUCCESS )
	{
		strCHATMSG = CTChart::Format( TSTR_PRVSHOP_BUY_SUCC, bCount);
		dwSound = ID_SND_INFO;
	}
	else if( bResult == STORE_ITEM_NOITEM )
		strBOXMSG = CTChart::LoadString( TSTR_PRVSHOP_BUY_NOITEM);
	else if( bResult == STORE_ITEM_NEEDMONEY )
		strBOXMSG = CTChart::LoadString( TSTR_PRVSHOP_BUY_NEEDMONEY);
	else if( bResult == STORE_ITEM_NOITEMCOUNT )
		strBOXMSG = CTChart::LoadString( TSTR_PRVSHOP_BUY_NOITEMCOUNT);
	else if( bResult == STORE_ITEM_INVENFULL )
		strBOXMSG = CTChart::LoadString( TSTR_PRVSHOP_BUY_INVENFULL);

	if( !strBOXMSG.IsEmpty() )
	{
		m_pMainWnd->MessageBoxOK(
			strBOXMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			dwSound);
	}

	if( !strCHATMSG.IsEmpty() )
	{
		CString strTYPE;
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strCHATMSG, TCOLOR_INFO, TCHAT_FLAG_INFO);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_STOREITEMSELL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bItem;
	BYTE bCount;

	(*pPacket) 
		>> bItem
		>> bCount;

	CTPrivateShopDlg* pShopDlg = static_cast<CTPrivateShopDlg*>(m_vTFRAME[TFRAME_PRIVATE_SELL]);
	INT nIdx = pShopDlg->GetItemIdxByNetID(bItem);

	if( nIdx != T_INVALID )
	{
		CTPrivateShopItem *pItemInfo = pShopDlg->GetItem(nIdx);
		CTClientItem *pItem = pItemInfo->m_pClientItem;

		CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
		CTTextLinkData* pData;

		DWORD dwID = pTextLinker->NewTextLinkData(&pData);
		pData->PackItem( pItem->GetTITEM(), pItem);

		VECTORDWORD vItemID;
		vItemID.push_back(dwID);

		CString strCHATMSG;
		strCHATMSG = CTChart::Format( TSTR_PRVSHOP_SELL_ACK, _T("%s"), bCount);
		strCHATMSG = pTextLinker->MakeItemNetText( strCHATMSG, vItemID);

		CString strTYPE;
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
		m_pChatFrame->ChatMSG(
			strTYPE,
			m_pMainChar->GetName(),
			strCHATMSG,
			TCOLOR_INFO,
			TCHAT_FLAG_INFO,
			ID_SND_INFO,
			MEDIA_TSOUND,
			-1);

		pShopDlg->ReduceItemCount(bItem, bCount);
		pShopDlg->NotifyUpdate();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDASK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strInviter;

	(*pPacket) 
		>> strInviter;

	CTMessengerCommander::GetInstance()
		->OnFriendInvite(strInviter);

	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	CTMessengerCommander::FriendMember frd;

	(*pPacket) 
		>> bResult
		>> frd.dwCharID
		>> frd.strName
		>> frd.bLevel
		>> frd.bGroup
		>> frd.bClass
		>> frd.dwRegion;

	frd.bConnected = TRUE;

	CTMessengerCommander::GetInstance()
		->OnAddFriend(bResult, &frd);

	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDERASE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwCharID;

	(*pPacket)
		>> bResult
		>> dwCharID;

	if( bResult == FRIEND_SUCCESS )
	{
		CTMessengerCommander::GetInstance()
			->OnEraseFriend(dwCharID);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bGroupCnt;
	BYTE bMemberCnt;

	CTMessengerCommander::FriendGroup* pGrps = NULL;
	CTMessengerCommander::FriendMember* pFrds = NULL;

	DWORD dwSoulID = 0;

	(*pPacket)
		>> dwSoulID;

	if( dwSoulID != 0 )
	{
		CString strSoulName;
		BYTE bSoulLevel;
		BYTE bSoulClass;
		BYTE bSoulConnected;
		DWORD dwSoulRegion;

		(*pPacket)
			>> strSoulName
			>> bSoulLevel
			>> bSoulClass
			>> bSoulConnected
			>> dwSoulRegion;

		m_pMessengerBase->GetMsgGroupList()->UpdateSoulmateInfo(
			dwSoulID,
			strSoulName,
			dwSoulRegion,
			bSoulLevel,
			bSoulClass,
			bSoulConnected );
	}
	else
	{
		m_pMessengerBase->GetMsgGroupList()->UpdateSoulmateInfo(
			0, "", 0, 255, 255, 255 );
	}

	(*pPacket) 
		>> bGroupCnt;

	if( bGroupCnt )
	{
		pGrps = new CTMessengerCommander::FriendGroup[bGroupCnt];
		for(BYTE i=0; i<bGroupCnt; ++i)
		{
			(*pPacket)
				>> pGrps[i].bGroupID
				>> pGrps[i].strName;
		}
	}

	(*pPacket)
		>> bMemberCnt;

	CTMessengerCommander::VTFRIENDMEMBER vMEMBERS;
	CTMessengerCommander::TMAPLINKGROUPMEMBER vGROUPS;

	if( bMemberCnt )
	{
		pFrds = new CTMessengerCommander::FriendMember[bMemberCnt];
		for(BYTE i=0; i<bMemberCnt; ++i)
		{
			(*pPacket)
				>> pFrds[i].dwCharID
				>> pFrds[i].strName
				>> pFrds[i].bLevel
				>> pFrds[i].bGroup
				>> pFrds[i].bClass
				>> pFrds[i].bConnected
				>> pFrds[i].dwRegion;

			CTMessengerCommander::TMAPLINKGROUPMEMBER::iterator it =
				vGROUPS.find( pFrds[i].bGroup );

			if( it != vGROUPS.end() )
				it->second.push_back( &pFrds[i] );
			else
			{
				vMEMBERS.clear();
				vMEMBERS.push_back( &pFrds[i] );
				vGROUPS.insert( make_pair( pFrds[i].bGroup, vMEMBERS ) );
			}
		}
	}

	CTMessengerCommander::GetInstance()->OnFriendlist(
		bGroupCnt,
		pGrps,
		bMemberCnt,
		pFrds,
		vGROUPS );

	if( pGrps )
		delete [] pGrps;
	if( pFrds )
		delete [] pFrds;

	CTFrameGroupBase* pMsgFrame = static_cast<CTFrameGroupBase*>(m_vTFRAME[TFRAME_MESSENGER]);

	EnableUI(TFRAME_MESSENGER);
	pMsgFrame->SetSelect(TMESSENGER_FRAME_MESSENGER);

	CTMainUI* pMain = static_cast<CTMainUI*>( m_vTFRAME[TFRAME_MAIN] );
	pMain->SetNewMessengerMsg(FALSE);
	
	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDCONNECTION_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	CString strName;
	DWORD dwRegion;

	(*pPacket)
		>> bResult
		>> strName
		>> dwRegion;

	CTMessengerCommander::GetInstance()
		->OnConnectFriend(bResult,strName,dwRegion);

	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDREGION_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strName;
	DWORD dwRegion;

	(*pPacket)
		>> strName
		>> dwRegion;

	CTMessengerCommander::GetInstance()
		->OnChangeFriendRegion(strName,dwRegion);

	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDGROUPMAKE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bGroupID;
	CString strName;

	(*pPacket)
		>> bResult
		>> bGroupID
		>> strName;

	CTMessengerCommander::GetInstance()
		->OnMakeFriendGroup(bResult,bGroupID,strName);

	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDGROUPDELETE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bGroupID;

	(*pPacket)
		>> bResult
		>> bGroupID;

	CTMessengerCommander::GetInstance()
		->OnEraseFriendGroup(bResult,bGroupID);

	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDGROUPCHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bGroup;
	DWORD dwFriend;

	(*pPacket)
		>> bResult
		>> bGroup
		>> dwFriend;

	CTMessengerCommander::GetInstance()
		->OnChangeFriendGroup(bResult,bGroup,dwFriend);

	return TERR_NONE;
}

int CTClientGame::OnCS_FRIENDGROUPNAME_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bGroup;
	CString strName;

	(*pPacket)
		>> bResult
		>> bGroup
		>> strName;

	CTMessengerCommander::GetInstance()
		->OnFriendGroupName(bResult,bGroup,strName);

	return TERR_NONE;
}

int CTClientGame::OnCS_TMSINVITE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwInvID;
	DWORD dwTMS;
	BYTE bCount;
	
	(*pPacket)
		>> dwInvID
		>> dwTMS
		>> bCount;

	CString strInviter;
	if( bCount )
	{
		MCTARGET_ARRAY vTARGET;
		for(BYTE i=0; i<bCount; ++i)
		{
			MSGCHAT_TARGET t;
			
			(*pPacket) 
				>> t.dwCharID
				>> t.strName
				>> t.bClass
				>> t.bLevel;

			if( t.strName != m_pMainChar->m_strNAME )
				vTARGET.push_back(t);
			
			if( dwInvID == t.dwCharID )
				strInviter = t.strName; 
		}

		CTMessengerCommander::GetInstance()
			->OnInviteChatTarget(dwTMS, strInviter, &vTARGET);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_TMSRECV_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwTMS;
	CString strSender;
	CString strMessage;

	(*pPacket)
		>> dwTMS
		>> strSender
		>> strMessage;

	CTMessengerCommander::GetInstance()
		->OnChat(dwTMS, strSender, strMessage);

	return TERR_NONE;
}

int CTClientGame::OnCS_TMSOUT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwTMS;
	CString strTarget;

	(*pPacket)
		>> dwTMS
		>> strTarget;

	CTMessengerCommander::GetInstance()
		->OnChatOut(dwTMS, strTarget);

	return TERR_NONE;
}

int CTClientGame::OnCS_PROTECTEDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;
	DWORD dwProtected;
	CString strName;
	BYTE bBlockOption;

	(*pPacket)
		>> bCount;

	CTBlockListDlg* pDlg = m_pBlockList;
	pDlg->Clear();

	for( BYTE i=0 ; i<bCount; ++i )
	{
		(*pPacket)
			>> dwProtected
			>> strName
			>> bBlockOption;

		pDlg->Add(
			dwProtected,
			strName,
			bBlockOption);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PROTECTEDADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID = 0;
	BYTE bResult;
	CString strName;
	BYTE bBlockOption = 0;

	(*pPacket)
		>> bResult
		>> dwCharID
		>> strName
		>> bBlockOption;

	if( bResult == FRIEND_SUCCESS )
	{
		CTBlockListDlg* pDlg = m_pBlockList;

		pDlg->Add(
			dwCharID,			
			strName,
			bBlockOption);

		if(m_pRelay)
			m_pRelay->SendCS_RELAYPROTECTEDADD_REQ(
				strName,
				bBlockOption);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PROTECTEDERASE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	CString strName;

	(*pPacket)
		>> bResult
		>> strName;

	if( bResult == FRIEND_SUCCESS )
	{
		CTBlockListDlg* pDlg = m_pBlockList;
		INT nIndex = pDlg->Find(strName);
		if( nIndex != T_INVALID )
			pDlg->Remvoe(nIndex);

		if(m_pRelay)
			m_pRelay->SendCS_RELAYPROTECTEDDEL_REQ(strName);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_POSTSEND_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTMailSendDlg* pDlg = static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND]);

	BYTE bResult;

	(*pPacket)
		>> bResult;

	CString strCMSG;
	CString strMMSG;

	DWORD dwSound = ID_SND_ERROR;

	switch(bResult)
	{
	case POST_SUCCESS:
		{
			strCMSG = CTChart::LoadString( TSTR_CMSG_POST_SUCCESS);
			dwSound = ID_SND_INFO;

			if( pDlg->m_bSendByCashItem == 2 ) // 캐쉬아이템에 의해서 편지보내기가 행해진 것이라면
			{
				CTClientInven *pTINVEN = m_pMainChar->FindTInven(pDlg->m_bInvenIDByCashItem);
				if(!pTINVEN)
					break;
				
				CTClientItem *pTITEM = pTINVEN->FindTItem(pDlg->m_bSlotIDByCashItem);
				if( !pTITEM )
					break;

				SendCS_ITEMUSE_REQ(
					&D3DXVECTOR2(
					m_pMainChar->GetPositionX(),
					m_pMainChar->GetPositionZ()),
					pTITEM->GetTITEM()->m_wItemID,
					pDlg->m_bInvenIDByCashItem,
					pDlg->m_bSlotIDByCashItem,
					pTITEM->GetTITEM()->m_wDelayGroupID);
			}
		}
		break;

	case POST_NORECEIVER:	strMMSG = CTChart::LoadString( TSTR_CMSG_POST_NORECEIVER); break;
	case POST_NEEDMONEY:	strMMSG = CTChart::LoadString( TSTR_CMSG_POST_NEEDMONEY); break;
	case POST_NOTITLE:		strMMSG = CTChart::LoadString( TSTR_CMSG_POST_NOTITLE); break;
	case POST_FULL:			strMMSG = CTChart::LoadString( TSTR_CMSG_POST_FULL); break;
	case POST_SAMEACCOUNT: strMMSG = CTChart::LoadString( TSTR_POST_SAMEACCOUNT); break;
	}

	if( !strCMSG.IsEmpty() )
	{
		CString strTYPE;
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
		m_pChatFrame->ChatSysMSG(strTYPE, m_pMainChar->GetName(), strCMSG, TCOLOR_INFO, TCHAT_FLAG_INFO);
	}

	if( !strMMSG.IsEmpty() )
	{
		m_pMainWnd->MessageBoxOK(
			strMMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			dwSound);
	}

	pDlg->ClearUsingByCash(); // 캐쉬아이템에 의해서 편지보내기 할때 사용하는 정보들은 이제 필요없으니 리셋.
	return TERR_NONE;
}

int CTClientGame::OnCS_POSTRECV_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	TMAIL_SIMPLE mail;

	(*pPacket)
		>> mail.m_dwPostID
		>> mail.m_bRead
		>> mail.m_bType
		>> mail.m_strSender
		>> mail.m_strTitle
		>> mail.m_nTime;

	CString strMSG;
	strMSG = CTChart::Format( TSTR_CMSG_RECVMAIL, (LPCSTR)mail.m_strSender);

	CString strTYPE;
	strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
	m_pChatFrame->ChatSysMSG(strTYPE, m_pMainChar->GetName(), strMSG, 
		TCOLOR_INFO, TCHAT_FLAG_INFO, ID_SND_INFO, MEDIA_TSOUND);

	NotifyNewMail();
	
	if( m_pMailList->m_wCurPage == 0 )
		++m_pMailList->m_wCurPage;

	return TERR_NONE;
}

int CTClientGame::OnCS_POSTLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wTotalCount;
	WORD wNotReadCount;
	WORD wPage;
	WORD wCount;

	(*pPacket)
		>> wTotalCount
		>> wNotReadCount
		>> wPage
		>> wCount;

	m_pMailList->ClearMail();

	if( wCount )
	{
		for(WORD i=0; i<wCount; ++i)
		{
			LPTMAIL_SIMPLE pMail = new TMAIL_SIMPLE;

			(*pPacket)
				>> pMail->m_dwPostID
				>> pMail->m_bRead
				>> pMail->m_bType
				>> pMail->m_strSender
				>> pMail->m_strTitle
				>> pMail->m_nTime;

			m_pMailList->AddMail(pMail);
		}
	}

	m_pMailList->SetInfo(
		wTotalCount,
		wNotReadCount,
		wPage);
	
	CTMainUI* pMain = static_cast<CTMainUI*>( m_vTFRAME[TFRAME_MAIN] );
	pMain->SetNewMail(FALSE);

	EnableUI(TFRAME_MAIL);
	return TERR_NONE;
}

int CTClientGame::OnCS_POSTINFO_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wTotalCount;
	WORD wNotReadCount;

	(*pPacket)
		>> wTotalCount
		>> wNotReadCount;

	m_pMailList->m_wTotalCount = wTotalCount;

	if( wTotalCount > 0 )
        m_pMailList->m_wCurPage = 1;
	else
		m_pMailList->m_wCurPage = 0;

	if( wNotReadCount != 0 )
		NotifyNewMail();

	return TERR_NONE;
}

int CTClientGame::OnCS_POSTVIEW_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bNotAskMail;
	BYTE bItemCnt;
	DWORD dwPostID;
	
	(*pPacket)
		>> dwPostID;

	INT nMailIdx = m_pMailList->FindIndexByPostID(dwPostID);
	if( nMailIdx == T_INVALID )
		return TERR_NONE;

	LPTMAIL pMail = new TMAIL;
	pMail->m_dwPostID = dwPostID;

	(*pPacket)
		>> pMail->m_bRead
		>> pMail->m_strMessage
		>> pMail->m_dwRune
		>> pMail->m_dwLuna
		>> pMail->m_dwCron
		>> bItemCnt;

	bNotAskMail = pMail->m_bRead;

	for(BYTE i=0; i<TMAIL_ITEM_MAX; ++i)
	{
		if( i >= bItemCnt )
		{
			pMail->m_vItems[i] = NULL;
			continue;
		}
		
		pMail->m_vItems[i] = new CTClientItem;

		pMail->m_vItems[i]->SetItemData(pPacket);
		pMail->m_vItems[i]->SetItemMagic(pPacket);

		if( !pMail->m_vItems[i]->CheckValid() )
			SAFE_DELETE(pMail->m_vItems[i]);
	}

	m_pMailList->SetMail(nMailIdx, pMail);
	LPTMAIL_SIMPLE pMailSimple = m_pMailList->GetMailSimple(nMailIdx);
	pMailSimple->m_bRead = pMail->m_bRead;

	if( bNotAskMail )
	{
		CTMailRecvDlg* pDlg = static_cast<CTMailRecvDlg*>(m_vTFRAME[TFRAME_MAIL_RECV]);
		pDlg->SetInfo(pMail, pMailSimple);
		
		EnableUI(TFRAME_MAIL_RECV);
	}
	else
	{
		CTMailAskMoneyDlg* pDlg = static_cast<CTMailAskMoneyDlg*>(m_vTFRAME[TFRAME_MAIL_ASK_MONEY]);
		pDlg->SetInfo(pMail, pMailSimple);
		
		EnableUI(TFRAME_MAIL_ASK_MONEY);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_POSTDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwPostID;
	
	(*pPacket)
		>> dwPostID;

	if( dwPostID != 0 )
	{
		DisableUI(TFRAME_MAIL_RECV);
		CTMailRecvDlg* pRecvDlg = static_cast<CTMailRecvDlg*>(m_vTFRAME[TFRAME_MAIL_RECV]);
		pRecvDlg->Clear();

		INT nCount = (INT) m_pMailList->m_wTotalCount;
		m_pMailList->ClearMail();
		if( --nCount > 0 )
		{
			INT nTotalPageCount = nCount / 7 + (nCount%7 != 0 ? 1 : 0);
			if( m_pMailList->m_wCurPage <= nTotalPageCount )
				m_pSESSION->SendCS_POSTLIST_REQ( m_pMailList->m_wCurPage );
			else
				m_pSESSION->SendCS_POSTLIST_REQ( nTotalPageCount );
		}
		else
		{
			m_pMailList->SetInfo( 0, 0, 0 );
			EnableUI( TFRAME_MAIL );
		}
	}
	else
	{
		m_pMainWnd->MessageBoxOK(
			CTChart::LoadString(TSTR_CANT_DELETE_MAIL),
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0 );
	}
	return TERR_NONE;
}

int CTClientGame::OnCS_POSTGETITEM_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	
	(*pPacket)
		>> bResult;

	if( bResult == POST_SUCCESS )
	{
		CTMailRecvDlg* pDlg = static_cast<CTMailRecvDlg*>(m_vTFRAME[TFRAME_MAIL_RECV]);
		pDlg->ClearAppendedItem();

		ResetInvenUI();
	}
	else
	{
		switch( bResult )
		{
		case POST_INVENFULL:
			{
				CString strMSG;
				strMSG = CTChart::LoadString( TSTR_ITEM_NO_SLOT);
				ShowInfoChat(strMSG, TUISND_TYPE_ERROR);
			}
			break;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_POSTRETURN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	if( bResult == POST_SUCCESS )
	{
		CTMailAskMoneyDlg* pAskDlg = static_cast<CTMailAskMoneyDlg*>(m_vTFRAME[TFRAME_MAIL_ASK_MONEY]);
		if( pAskDlg->IsWaitPayAck() )
		{
			LPTMAIL pMail = pAskDlg->GetMail();
			LPTMAIL_SIMPLE pMailSimple = pAskDlg->GetMailSimple();

			pMail->m_bRead = TRUE;
			pMailSimple->m_bRead = TRUE;

			if( pMail && pMailSimple )
			{
				CTMailRecvDlg* pRecvDlg = static_cast<CTMailRecvDlg*>(m_vTFRAME[TFRAME_MAIL_RECV]);
				pRecvDlg->SetInfo(pMail, pMailSimple);
				
				EnableUI(TFRAME_MAIL_RECV);
				pAskDlg->Clear();
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PETMAKE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	WORD wPetID;
	CString strName;
	__int64 ldwTime;

	(*pPacket)
		>> bResult
		>> wPetID
		>> strName
		>> ldwTime;

	CString strBoxMsg;
	DWORD dwSound= ID_SND_ERROR;

	switch( bResult )
	{
	case PET_SUCCESS:
		{
			CTPetManageDlg* pDlg = static_cast<CTPetManageDlg*>(m_vTFRAME[TFRAME_PET_MANAGE]);
	
			INT nId = pDlg->FindPetByID( wPetID );
			if( nId == T_INVALID )
			{
				pDlg->AddPet(strName, wPetID, CTime(ldwTime));
				strBoxMsg = CTChart::LoadString( TSTR_PETMAKE_ACK_SUCC);
			}
			else
			{
				CTime time(ldwTime);
				pDlg->SetPet(nId, strName, wPetID, time);
				strBoxMsg = CTChart::Format( TSTR_PETMAKE_ACK_EXPTIME, time.GetYear(), time.GetMonth(),time.GetDay());
			}

			dwSound = ID_SND_INFO;

			ResetInvenUI();

			EnableUI(TFRAME_PET_MANAGE);
		}
		break;

	case PET_FULL:
		strBoxMsg = CTChart::LoadString( TSTR_PETMAKE_ACK_FULL);
		break;

	case PET_USETIME:
		strBoxMsg = CTChart::LoadString( TSTR_PETMAKE_ACK_USETIME );
		break;
	}

	if( !strBoxMsg.IsEmpty() )
	{
		m_pMainWnd->MessageBoxOK(
			strBoxMsg,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			dwSound);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PETDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	WORD wPetID;

	(*pPacket)
		>> bResult
		>> wPetID;

	if( bResult == PET_SUCCESS )
	{
		CTPetManageDlg* pPetDlg = static_cast<CTPetManageDlg*>(m_vTFRAME[TFRAME_PET_MANAGE]);
		pPetDlg->RemovePet( wPetID );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PETLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;
	WORD wPetID;
	CString strName;
	__int64 ldwTime;

	(*pPacket)
		>> bCount;

	CTPetManageDlg* pDlg = static_cast<CTPetManageDlg*>(m_vTFRAME[TFRAME_PET_MANAGE]);
	pDlg->ClearPet();

	for(BYTE i=0; i<bCount; ++i)
	{
		(*pPacket)
			>> wPetID
			>> strName
			>> ldwTime;

		pDlg->AddPet(strName, wPetID, CTime(ldwTime));
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_PETRECALL_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	CString strChatMsg;
	switch( bResult )
	{
	case PET_USETIME:
		strChatMsg = CTChart::LoadString( TSTR_PETRECALL_ACK_USETIME);
		break;
	}

	if( !strChatMsg.IsEmpty() )
	{
		CString strTYPE;
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strChatMsg, TCOLOR_INFO, TCHAT_FLAG_INFO);
	}

	CTPetManageDlg* pPetDlg = static_cast<CTPetManageDlg*>(m_vTFRAME[TFRAME_PET_MANAGE]);
	pPetDlg->SetRecalling(FALSE);

	return TERR_NONE;
}

int CTClientGame::OnCS_PETRIDING_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwCharID;
	DWORD dwMonID;
	BYTE bAction;

	(*pPacket)
		>> bResult
		>> dwCharID
		>> dwMonID
		>> bAction;

	if( bResult == PET_SUCCESS )
	{
		if( bAction == PETACTION_RIDING )
		{
			TakeUpPet(dwCharID, dwMonID);

			if( dwCharID == m_pMainChar->m_dwID )
			{
				CTClientPet* pPet = static_cast<CTClientPet*>( FindRecall(dwMonID) );
				CTClientRecall* pTRECALL = GetMainRecall();
				if( pTRECALL && pPet )
					pTRECALL->m_fSpeedFactor = pPet->GetSpeedWhenRiding();
			}
		}
		else
		{
			CTClientPet* pPet = static_cast<CTClientPet*>( FindRecall(dwMonID) );
	
			if( pPet )
			{
				CTClientChar* pTCHAR = pPet->GetTakeUpChar();
				pPet->TakeDown();

				if( pTCHAR != m_pMainChar )
				{
					if( pTCHAR )
					{
						pTCHAR->m_bACTLevel = CTChart::GetTACTION( TA_STAND )->m_bLevel;
						pTCHAR->m_bAction = TA_STAND;
					}
				}
			}

			if( dwCharID == m_pMainChar->m_dwID )
			{
				CTClientRecall* pTPET = FindRecall(dwMonID);

				if( pTPET )
					pTPET->m_bCanSelected = TRUE;

				m_pMainChar->SetTAction(m_pMainChar->GetTAction());
				m_bMoveType = TMAINMOVE_NORMAL;
				m_bAutoRun = FALSE;

				// PET_RIDING_1.2 기존펫 해제
				if( m_pSESSION &&
					pTPET && 
					m_bPetUseByHotbar )
				{
					m_pSESSION->SendCS_DELRECALLMON_REQ(pTPET->m_dwID,pTPET->m_bType);

					if( m_nNextPetID == 0 )
						m_bPetUseByHotbar = FALSE;
				}

				CTClientRecall* pTRECALL = GetMainRecall();
				if( pTRECALL )
					pTRECALL->m_fSpeedFactor = pTRECALL->m_fTDEFAULTSPEED;
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CHGCHANNEL_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	switch(bResult)
	{
	case CHC_SAMECHANNEL:
		{
			ShowInfoChat(CTChart::LoadString(TSTR_GLOBAL_CHANNEL));
		}
		break;

	case CHC_NOCHANNEL:
	case CHC_SUCCESS:
		break;
	}
	return TERR_NONE;
}

int CTClientGame::OnCS_CABINETLIST_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;

	(*pPacket)
		>> bCount;

	CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_CABINET]);

	for(BYTE i=0; i<TMAX_CABINET_CNT; ++i)
		pDlg->SetCabinetInfo(i, FALSE);

	BYTE bFirstID = TMAX_CABINET_CNT;

	for(i=0; i<bCount; ++i)
	{
		BYTE bCabinetID;
		BYTE bOpen;

		(*pPacket)
			>> bCabinetID
			>> bOpen;
	
		if( bOpen && bFirstID > bCabinetID )
			bFirstID = bCabinetID;

		pDlg->SetCabinetInfo(bCabinetID, bOpen);
	}

	pDlg->CompleteCabUpdate();
	pDlg->RequestShowComponent();
	pDlg->SelectCab(bFirstID);

	return TERR_NONE;
}

int CTClientGame::OnCS_CABINETITEMLIST_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bCabinetID;
	DWORD bItemCount;

	(*pPacket)
		>> bResult;
	
	ResetInvenUI();

	if( bResult == CABINET_SUCCESS )
	{
		(*pPacket)
			>> bCabinetID
			>> bItemCount;

		CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_CABINET]);
		pDlg->Clear(bCabinetID);

		for(BYTE i=0; i<bItemCount; ++i)
		{
			DWORD dwStItemID;

			(*pPacket)
				>> dwStItemID;

			CTClientItem* pItem = new CTClientItem;

			pItem->SetItemData(pPacket);
			pItem->SetItemMagic(pPacket);

			if( !pItem->CheckValid() )
			{
				delete pItem;
				continue;
			}

			CTCabinetItem* pCabItem = new CTCabinetItem;
			pCabItem->m_dwStItemID = dwStItemID;
			pCabItem->m_pClientItem = pItem;
			pCabItem->m_bItemCnt = pItem->GetCount();

			pDlg->AddItem(bCabinetID, pCabItem);
		}

		pDlg->CompleteCabinetUpdate(bCabinetID);
	}
	else 
	{
		switch(bResult)
		{
		case CABINET_FULL:		return TERR_CABINET_NO_SLOT;
		case CABINET_NEEDMONEY:	return TERR_ITEM_NO_MONEY;
		case CABINET_NPCCALLERROR: return TERR_INTERNAL;
		default:				return TERR_CABINET;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CABINETOPEN_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bCabinetID;

	(*pPacket)
		>> bResult
		>> bCabinetID;

	if( bResult == CABINET_SUCCESS )
	{
		CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_CABINET]);
		pDlg->SetCabinetInfo(bCabinetID, TRUE);
		pDlg->SelectCab(bCabinetID);
	}
	else 
	{
		switch(bResult)
		{
		case CABINET_MAX:		return TERR_MAX_CABINET;
		case CABINET_NEEDMONEY:	return TERR_ITEM_NO_MONEY;
		default:				return TERR_CABINET;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_LOCALOCCUPY_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bType;
	WORD wLocalID;
	BYTE bCountry;
	DWORD dwGuildID;

	(*pPacket)
		>> bType
		>> wLocalID
		>> bCountry
		>> dwGuildID;

	m_vMAP.m_vNEWQUESTOBJ.clear();

	MAPOBJECT::iterator itTNPC;
	for( itTNPC = m_vMAP.m_mapTNpc.begin(); itTNPC != m_vMAP.m_mapTNpc.end(); itTNPC++)
	{
		CTClientNpc* pTNPC = static_cast<CTClientNpc*>(itTNPC->second);
		LPTREGIONINFO pTREGION = m_vMAP.GetRegionINFO(pTNPC->GetPositionX(), pTNPC->GetPositionZ());

		if( pTREGION && pTREGION->m_wLocalID == wLocalID )
			m_vMAP.m_vNEWQUESTOBJ.push_back(pTNPC->m_dwID);
	}

	if( !m_vMAP.m_mapTNpc.empty() )
		CheckQuestOBJ();

	return TERR_NONE;
}

int CTClientGame::OnCS_DUELINVITE_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwInviter;

	(*pPacket)
		>> dwInviter;

	CTClientChar* pInviter = FindPC(dwInviter);
	if( !IsBusy() && pInviter && !m_pDuelTarget )
	{
		m_pDuelTarget = pInviter;

		CString strMSG;
		strMSG = CTChart::Format( TSTR_DUELINVITE_MSG, pInviter->m_strNAME);

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_DUELINVITE_YES,
			GM_DUELINVITE_NO,
			TCOMMAND( GM_DUELINVITE_BUSY, dwInviter ),
			FALSE );
	}
	else if( m_pSESSION )
	{
		m_pSESSION->SendCS_DUELINVITEREPLY_REQ(ASK_BUSY, dwInviter);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DUELSTANDBY_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwInviter;
	DWORD dwTarget;

	(*pPacket)
		>> dwInviter
		>> dwTarget;

	DWORD dwMain,dwEnemy;
	if( m_pMainChar->m_dwID == dwInviter )
	{
		dwMain = dwInviter;
		dwEnemy = dwTarget;
	}
	else
	{
		dwMain = dwTarget;
		dwEnemy = dwInviter;
	}

	if( !m_pDuelTarget || m_pMainChar->m_dwID != dwMain || m_pDuelTarget->m_dwID != dwEnemy )
		m_pSESSION->SendCS_DUELEND_REQ();
	else
	{
		(*pPacket)
			>> m_vDuelCenter.x
			>> m_vDuelCenter.y;

		m_dwDuelWaitTick = DUEL_STANDBY_TIME*1000;
		m_dwDuelOutTick = (DWORD)T_INVALID;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_DUELSTART_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwInviter;
	DWORD dwTarget;

	(*pPacket)
		>> bResult
		>> dwInviter
		>> dwTarget;
	
	CString strCHATMSG;
	CString strBOXMSG;

	BOOL bDuelStart = FALSE;
	DWORD dwEnemy = m_pMainChar->m_dwID == dwInviter? dwTarget: dwInviter;

	if( bResult == DUEL_SUCCESS )
	{
		CTClientChar* pEnemy = FindPC(dwEnemy);
		if( pEnemy )
		{
			m_bDuel = TRUE;
			m_pDuelTarget = pEnemy;
			m_dwDuelWaitTick = 0;
			m_dwDuelOutTick = (DWORD)T_INVALID;
			m_dwDuelTick = DUEL_TIME*1000;
			m_strDuelTargetName = pEnemy->m_strNAME;
			m_dwDuelTargetID = pEnemy->m_dwID;
            
			bDuelStart = TRUE;

			strCHATMSG = CTChart::Format( TSTR_DUEL_START, pEnemy->m_strNAME);

			if( m_pTARGET == pEnemy )
				((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();				
		}
	}
	
	if( !bDuelStart )
	{
		m_bDuel = FALSE;
		m_pDuelTarget = FALSE;

		if( bResult == DUEL_REFUSE )
		{
			CTClientChar* pEnemy = FindPC(dwEnemy);
			if( m_pMainChar->m_dwID == dwInviter && pEnemy )
				strBOXMSG = CTChart::Format( TSTR_DUEL_REFUSE, pEnemy->m_strNAME);
		}
		else if( bResult == DUEL_BUSY )
		{
			CTClientChar* pEnemy = FindPC(dwEnemy);
			if( m_pMainChar->m_dwID == dwInviter && pEnemy )
				strBOXMSG = CTChart::Format( TSTR_ERR_TARGET_BUSY, pEnemy->m_strNAME);
		}
		else
		{
			strBOXMSG = CTChart::LoadString( TSTR_DUEL_ERROR);

			if( bResult == DUEL_SUCCESS )
			{
				m_bDuel = FALSE;
				m_pSESSION->SendCS_DUELEND_REQ();
			}
		}
	}

	if( !strCHATMSG.IsEmpty() )
		ShowInfoChat(strCHATMSG);
	if( !strBOXMSG.IsEmpty() )
		ShowInfoMsgBox(strBOXMSG, FALSE);

	return TERR_NONE;
}

int CTClientGame::OnCS_DUELEND_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwLoser;

	(*pPacket)
		>> dwLoser;

	CString strMSG;
	if( m_dwDuelTargetID )
	{
		if( dwLoser == 0 )
		{
			strMSG = CTChart::Format( TSTR_DUEL_DRAW, m_strDuelTargetName);
		}
		else if( m_dwDuelTargetID == dwLoser || m_pMainChar->m_dwID == dwLoser ) 
		{
			if( m_dwDuelTargetID == dwLoser )
				strMSG = CTChart::Format( TSTR_DUEL_WIN, m_strDuelTargetName);
			else
				strMSG = CTChart::Format( TSTR_DUEL_LOSE, m_strDuelTargetName);
		}
	}

	if( !strMSG.IsEmpty() )
		ShowInfoMsgBox(strMSG, FALSE, TUISND_TYPE_INFO);

	if( m_pTARGET && m_pTARGET == m_pDuelTarget )
	{
		ResetTargetOBJ(NULL);
		ResetTargetINFO(FALSE);
	}

	m_bDuel = FALSE;
	m_pDuelTarget = NULL;
	m_dwDuelWaitTick = 0;
	m_dwDuelOutTick = (DWORD)T_INVALID;
	m_dwDuelTick = 0;
	m_strDuelTargetName.Empty();
	m_dwDuelTargetID = 0;

	return TERR_NONE;
}

int CTClientGame::OnCS_GETTARGET_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwTargetID;
	BYTE bTargetType;

	(*pPacket)
		>> dwTargetID
		>> bTargetType;

	if( dwTargetID == (DWORD)T_INVALID )
		return TERR_NONE;

	CTClientObjBase* pTARGET;
	if( bTargetType == OT_NPC )
		pTARGET = m_vMAP.FindTNPC(dwTargetID);
	else
		pTARGET = FindOBJ(dwTargetID, bTargetType);

	ResetTargetOBJ(pTARGET);
	ResetTargetINFO(FALSE);

	return TERR_NONE;
}

int CTClientGame::OnCS_GETTARGETANS_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwAnswerCharID;

	(*pPacket)
		>> dwAnswerCharID;

	if( m_pTARGET )
		m_pSESSION->SendCS_GETTARGETANS_REQ(dwAnswerCharID, m_pTARGET->m_dwID, m_pTARGET->m_bType);
	else
		m_pSESSION->SendCS_GETTARGETANS_REQ(dwAnswerCharID, (DWORD)T_INVALID, 0);

	return TERR_NONE;
}

int CTClientGame::OnCS_SMSSEND_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bType;
	CString strTargetName;

	(*pPacket)
		>> bResult
		>> bType
		>> strTargetName;

	if( bResult == SMS_SUCCESS )
	{
		CString strMSG;
		strMSG = CTChart::LoadString( TSTR_SMS_SUCCESS);

		ShowInfoChat(strMSG, TUISND_TYPE_INFO);
		ResetInvenUI();
	}
	else
	{
		switch(bResult)
		{
		case SMS_INTERNAL	:
		case SMS_NOINVEN	: 
		case SMS_NOITEM		: return TERR_ITEM_INTERNAL;
		case SMS_NOTARGET	: return TERR_SND_TARGET;
		case SMS_NOTMYGUILD	:
		case SMS_NOGUILD	: return TERR_NO_GUILD;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SKILLINIT_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	WORD wSkillID;

	(*pPacket)
		>> bResult
		>> wSkillID;

	CString strMSG;
	TUISND_TYPE eSndType;

	if( bResult == SKILL_SUCCESS )
	{
		if( wSkillID )
		{
			LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);
			if( pTSKILL )
				strMSG = CTChart::Format( TSTR_SKILLINIT_ONE_SUCCESS, pTSKILL->m_strNAME);
		}
		else
		{
			strMSG = CTChart::LoadString( TSTR_SKILLINIT_ALL_SUCCESS);
			if( m_vTFRAME[TFRAME_SKILL]->IsVisible() )
				ResetSkillICON();
		}

		eSndType = TUISND_TYPE_INFO;
		ResetInvenUI();
	}
	else
	{
		strMSG = CTChart::LoadString( TSTR_SKILLINIT_FAIL);
		eSndType = TUISND_TYPE_ERROR;
	}

	if( !strMSG.IsEmpty() )
		ShowInfoMsgBox(strMSG, FALSE, eSndType);

	CTSkillDlg* pSKILLDLG = (CTSkillDlg*) GetFrame( TFRAME_SKILL );
	if( pSKILLDLG->IsVisible() )
	{
		DisableUI( TFRAME_SKILL );
		if( m_pSESSION )
			m_pSESSION->SendCS_NPCITEMLIST_REQ(TDEF_SKILL_NPC);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SKILLLIST_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;
	
	(*pPacket)
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_MINE]
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB1]
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB2]
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB3]
		>> m_pMainChar->m_vTSKILLPOINT[TPOINT_TAB4]
		>> bCount;

	m_pMainChar->ClearSkill();

	for(BYTE i=0; i<bCount; i++)
	{
		CTClientSkill *pTSKILL = new CTClientSkill();
		WORD wSkillID;

		(*pPacket)
			>> wSkillID
			>> pTSKILL->m_bLevel
			>> pTSKILL->m_dwTick;

		if(pTSKILL->m_dwTick)
			pTSKILL->m_bTimerON = TRUE;
		pTSKILL->m_pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);

		m_pMainChar->m_mapTSKILL.insert( MAPTSKILL::value_type( wSkillID, pTSKILL));
	}

	((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();

	return TERR_NONE;
}

int CTClientGame::OnCS_SKILLINITPOSSIBLE_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;
	(*pPacket) >> bCount;

	if( bCount == 0 || m_bSkillInitInven == INVEN_NULL )
	{
		CString strMSG;
		strMSG = CTChart::LoadString( TSTR_DONTHAVEINITSKILL);

		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE );

		return TERR_NONE;
	}

	CTSkillDlg* dlg = static_cast<CTSkillDlg*>(m_vTFRAME[TFRAME_SKILL]);
	//if( dlg->m_bTRADE )
	//	return TERR_NONE;

	dlg->ClearInitPossSkill();

	for(BYTE i=0; i<bCount; ++i)
	{
		WORD wSkillID;
		(*pPacket) >> wSkillID;
		dlg->AddInitPossSkill(wSkillID);
	}

	dlg->StartSkillInitMode( m_bSkillInitInven, m_bSkillInitSlot);

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDESTABLISH_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwGuildID;
	CString strName;

	(*pPacket)
		>> bResult
		>> dwGuildID
		>> strName;

	CTGuildCommander::GetInstance()
		->RecvGuildEstablish(bResult, dwGuildID, strName);

	return TERR_NONE;
}
int CTClientGame::OnCS_GUILDDISORGANIZATION_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	(*pPacket) >> bResult;

	CTGuildCommander::GetInstance()
		->RecvGuildDisorganization(bResult);

	return TERR_NONE;
}
int CTClientGame::OnCS_GUILDINVITE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	CString strGuildName;
	DWORD dwInviterID;
	CString strInviterName;

	(*pPacket) 
		>> bResult
		>> strGuildName
		>> dwInviterID
		>> strInviterName;

	CTGuildCommander::GetInstance()
		->RecvGuildInvite(bResult, dwInviterID, strInviterName, strGuildName);
	
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSINVITE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strGuild;
	CString strInviter;
	BYTE bDay;
	DWORD dwPoint;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	(*pPacket) 
		>> strGuild
		>> strInviter
		>> bDay
		>> dwPoint
		>> dwGold
		>> dwSilver
		>> dwCooper;

	if( IsBusy() )
	{
		m_pSESSION->SendCS_GUILDTACTICSANSWER_REQ(
			ASK_BUSY,
			strInviter,
			bDay,
			dwPoint,
			dwGold,
			dwSilver,
			dwCooper );
	}
	else
	{
		CTTacticsInviteDlg* pDLG = static_cast<CTTacticsInviteDlg*>( m_vTFRAME[ TFRAME_INVITE_TACTICS ] );
		pDLG->SetShowAccept(
			m_pMainChar->m_strNAME,
			strInviter,
			strGuild,
			bDay,
			dwPoint,
			dwGold,
			dwSilver,
			dwCooper );

		EnableUI( TFRAME_INVITE_TACTICS );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDJOIN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bRet;
	DWORD dwGuildID;
	CString strGuildName;
	DWORD dwNewMemberID;
	CString strNewMemberName;
	BYTE bMax = GUILD_MEMBER_MAX;

	(*pPacket) 
		>> bRet
		>> dwGuildID
		>> strGuildName
		>> dwNewMemberID
		>> strNewMemberName
		>> bMax;

	CTGuildCommander::GetInstance()
		->RecvGuildJoin(bRet, dwGuildID, strGuildName, dwNewMemberID, strNewMemberName, bMax);

	return TERR_NONE;
}
int CTClientGame::OnCS_GUILDLEAVE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bRet;
	CString strTarget;
	BYTE bReason;

	(*pPacket) 
		>> bRet
		>> strTarget
		>> bReason;

	CTGuildCommander::GetInstance()
		->RecvGuildLeave(bRet, strTarget, bReason);

	return TERR_NONE;
}
int CTClientGame::OnCS_GUILDDUTY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bRet;
	CString strTarget;
	BYTE bDuty;

	(*pPacket) 
		>> bRet
		>> strTarget
		>> bDuty;

	CTGuildCommander::GetInstance()
		->RecvGuildDuty(bRet, strTarget, bDuty);

	return TERR_NONE;
}
int CTClientGame::OnCS_GUILDMEMBERLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	if( bResult != GUILD_SUCCESS )
		return TERR_NONE;

	DWORD dwGuildID;
	CString strGuildName;
	WORD wCount;

	(*pPacket)
		>> dwGuildID
		>> strGuildName
		>> wCount;
	CTGuildCommander* pTGUILDCMDER = CTGuildCommander::GetInstance();
	pTGUILDCMDER->m_GuildMemberVec.clear();

	for(WORD i=0; i<wCount; ++i)
	{
		CTGuildCommander::GuildMember member;

		(*pPacket)
			>> member.m_dwCharID
			>> member.m_strName
			>> member.m_bLevel
			>> member.m_bClassID
			>> member.m_bDuty
			>> member.m_bPeer
			>> member.m_bIsConnect
			>> member.m_dwRegion
			>> member.m_bService
			>> member.m_wCastle
			>> member.m_bCamp
			>> member.m_dwTactics;

		pTGUILDCMDER->m_GuildMemberVec.push_back( member );
	}

	CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	if( pComm->IsVisible() &&
		pComm->IsSelect(TCOMMUNITY_FRAME_GUILDMEMBER) )
	{
		pComm->ResetInfo();
	}

	CTTournamentSelectTarget* pDLG = static_cast<CTTournamentSelectTarget*>( m_vTFRAME[TFRAME_TOURNAMENT_SELECT_TARGET] );
	if( pDLG && pDLG->m_bListening )
		pDLG->OnRecvGuildMemberList();

	return TERR_NONE;
}
int CTClientGame::OnCS_GUILDATTR_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD 	dwPlayerID;
	DWORD 	dwGuildID;
	CString	strGuildName;
	BYTE	bGuildPeer;
	DWORD	dwTacticsID;
	CString	strTacticsName;
	DWORD	dwMark;
	DWORD	dwMarkColor;

	(*pPacket) 
		>> dwPlayerID
		>> dwGuildID
		>> dwMark
		>> dwMarkColor
		>> strGuildName
		>> bGuildPeer
		>> dwTacticsID
		>> strTacticsName;

	CTClientGuildMark* pMark = NULL;
	if( dwMark )
	{
		pMark = new CTClientGuildMark(	TGMARK_MARK(dwMark),
										TGMARK_BACK(dwMark),
										TGMCOLOR_MARK(dwMarkColor),
										TGMCOLOR_BACK1(dwMarkColor),
										TGMCOLOR_BACK2(dwMarkColor) );
	}

	CTGuildCommander::GetInstance()
		->RecvGuildAttr(dwPlayerID, dwGuildID, strGuildName, bGuildPeer, dwTacticsID, strTacticsName, pMark);

	if( pMark )
		delete pMark;

	m_pChatFrame->ToggleTacticsChatMode( m_pMainChar->m_dwTacticsID );
	return TERR_NONE;
}
int CTClientGame::OnCS_GUILDPEER_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	CString strTarget;
	BYTE bPeer;

	(*pPacket) 
		>> bResult
		>> strTarget
		>> bPeer;

	CTGuildCommander::GetInstance()
		->RecvGuildPeer(bResult, strTarget, bPeer);

	return TERR_NONE;
}
int CTClientGame::OnCS_GUILDINFO_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket) >>
		bResult;

	if( bResult != GUILD_SUCCESS )
		return TERR_NONE;

	DWORD dwFame;
	DWORD dwFameColor;
	DWORD dwGI;
	BYTE bGPoint;
	BYTE bStatus;

	CTGuildCommander::GuildDetInfo info;

	(*pPacket) 
		>> info.m_dwGuildID
		>> info.m_strGName
		>> info.m_ldwGDate
		>> info.m_nGMbCnt
		>> info.m_nGMbMax
		>> info.m_strGMaster
		>> info.m_bGMasterPeer
		>> info.m_strGSubMaster1
		>> info.m_strGSubMaster2
		>> info.m_nGLev
		>> dwFame
		>> dwFameColor
		>> dwGI
		>> info.m_dwGExpCur
		>> info.m_dwGExpMax
		>> bGPoint
		>> bStatus
		>> info.m_nGRune
		>> info.m_nGLuna
		>> info.m_nGCron
		>> info.m_bMyDuty
		>> info.m_bMyPeerage
		>> info.m_bMyService
		>> info.m_strNotify
		>> info.m_dwPvPTotalPoint
		>> info.m_dwPvPUseablePoint;

	if( dwFame )
	{
		info.m_bShowMark		= TRUE;
		info.m_bMark			= TGMARK_MARK(dwFame);
		info.m_bMarkBack		= TGMARK_BACK(dwFame);
		info.m_bMarkColor		= TGMCOLOR_MARK(dwFameColor);
		info.m_bMarkBackColor1	= TGMCOLOR_BACK1(dwFameColor);
		info.m_bMarkBackColor2	= TGMCOLOR_BACK2(dwFameColor);
	}
	else
	{
		info.m_bShowMark		= FALSE;
		info.m_bMark			= 0;
		info.m_bMarkBack		= 0;
		info.m_bMarkColor		= 0;
		info.m_bMarkBackColor1	= 0;
		info.m_bMarkBackColor2	= 0;
	}

	CTGuildCommander::GetInstance()
		->RecvGuildDetInfo(info);

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDLOCALLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wCount;
	WORD wLocalCount;

	(*pPacket)
		>> wCount;

	if( wCount == 0 )
		return TERR_NONE;

#ifdef MODIFY_GUILD
	BYTE bTop3Count = 0;
#endif

	CTGuildCommander::TerritoryVec terrs;
	terrs.resize(wCount);

	for(BYTE i=0; i<wCount; ++i)
	{
		CTGuildCommander::Territory& info = terrs[i];

		(*pPacket)
			>> info.m_wCastleID
			>> info.m_strCastleName
			>> info.m_bCanApplyCastle
			>> info.m_dwGuildID
			>> info.m_strGuildName
			>> info.m_bCountry
			>> info.m_dlNextWar
			>> info.m_strHero
#ifdef MODIFY_GUILD
			>> info.m_strDefGuild
#endif
			>> info.m_strAtkGuild
			>> info.m_wDefGuildPoint
			>> info.m_wDefCountryPoint
			>> info.m_bDefCount //지원자 수
			>> info.m_wAtkGuildPoint
			>> info.m_wAtkCountryPoint
			>> info.m_bAtkCount //지원자 수
			>> info.m_wMyGuildPoint
#ifdef MODIFY_GUILD
			>> bTop3Count;

		for( BYTE m=0 ; m<bTop3Count ; ++m )
		{
			CString strName;
			WORD wPoint;

			(*pPacket)
				>> strName
				>> wPoint;

			info.m_vDTop3s.push_back( TOP3(strName, wPoint) );
		}

		(*pPacket)
			>> bTop3Count;

		for( BYTE m=0 ; m<bTop3Count ; ++m )
		{
			CString strName;
			WORD wPoint;

			(*pPacket)
				>> strName
				>> wPoint;

			info.m_vCTop3s.push_back( TOP3(strName, wPoint) );
		}

		(*pPacket)
#endif
			>> wLocalCount;

		if( wLocalCount != 0 )
			info.m_vLocals.resize(wLocalCount);

		for(BYTE j=0; j<wLocalCount; ++j)
		{
			CTGuildCommander::LocalTerritory& local = info.m_vLocals[j];

			(*pPacket)
				>> local.m_wLocalID
				>> local.m_strLocalName
				>> local.m_dwGuildID
				>> local.m_strGuildName
				>> local.m_bCountry
				>> local.m_dNextWar
				>> local.m_strHeroName;
		}
	}

#ifdef ADD_MISSION
	CTGuildCommander::GetInstance()->m_MissionVec.clear();

	BYTE bMissionCount = 0;
	(*pPacket)
		>> bMissionCount;

	for( BYTE i=0 ; i < bMissionCount ; ++i )
	{
		CTGuildCommander::Mission mission;
		INT64 dlNextWar;

		(*pPacket)
			>> mission.m_wMissionID
			>> mission.m_strMission
			>> mission.m_bCountry
			>> mission.m_bStatus
			>> dlNextWar;

			mission.m_NextWar = CTime(dlNextWar);

		CTGuildCommander::GetInstance()->m_MissionVec.push_back( mission );
	}
#endif

	CTGuildCommander::GetInstance()
		->RecvTerritoryList(&terrs);
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDLOCALRETURN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDCABINETLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bMaxCnt;
	BYTE bItemCount ;

	(*pPacket)
		>> bMaxCnt
		>> bItemCount;

	ResetInvenUI();

	CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[TFRAME_GUILDCABINET]);
	pDlg->ClearAll();
	pDlg->SetMaxStorage(bMaxCnt);

	for(BYTE i=0; i<bItemCount; ++i)
	{
		DWORD dwStItemID;

		(*pPacket)
			>> dwStItemID;

		CTClientItem* pItem = new CTClientItem;

		pItem->SetItemData(pPacket);
		pItem->SetItemMagic(pPacket);

		if( !pItem->CheckValid() )
		{
			delete pItem;
			continue;
		}
		
		CTCabinetItem* pCabItem = new CTCabinetItem;
		pCabItem->m_dwStItemID = dwStItemID;
		pCabItem->m_pClientItem = pItem;
		pCabItem->m_bItemCnt = pItem->GetCount();

		pDlg->AddItem(0, pCabItem);
	}

	if( !m_vTFRAME[TFRAME_GUILDCABINET]->IsVisible() )
		EnableUI(TFRAME_GUILDCABINET);

	CString strTITLE;
	strTITLE = CTChart::Format( TSTR_GUILD_CABINET_TITLE, m_pMainChar->m_strGUILD, bMaxCnt);
	pDlg->SetTitle(strTITLE);

	pDlg->CompleteCabUpdate();
	pDlg->SelectCab(0);
	pDlg->NotifyUpdate();

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDCABINETPUTIN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	CString strMSG;

	(*pPacket)
		>> bResult;
	
	switch(bResult)
	{
	case GUILD_CABINET_SUCCESS	:
		break;

	case GUILD_CABINET_FULL		:
		strMSG = CTChart::LoadString( TSTR_GUILD_CABINET_FULL);
		break;

	case GUILD_CABINET_LEVEL	:
		strMSG = CTChart::LoadString( TSTR_GUILD_CABINET_LEVEL);
		break;

	case GUILD_CABINET_NOTDUTY	:
		strMSG = CTChart::LoadString( TSTR_GUILD_CABINET_NOTDUTY);
		break;

	default						:
		strMSG = CTChart::LoadString( TSTR_GUILD_CABINET_PUTINFAIL);
		break;
	}

	if( !strMSG.IsEmpty() )
		ShowInfoChat(strMSG, TUISND_TYPE_ERROR);

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDCABINETTAKEOUT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	CString strMSG;

	(*pPacket)
		>> bResult;

	switch(bResult)
	{
	case GUILD_CABINET_SUCCESS		:
		break;

	case GUILD_CABINET_ITEMNOTFOUND :
		strMSG = CTChart::LoadString( TSTR_GUILD_CABINET_ITEMNOTFOUND);
		break;

	case GUILD_CABINET_INVENFULL	:
		strMSG = CTChart::LoadString( TSTR_GUILD_CABINET_INVENFULL);
		break;

	case GUILD_CABINET_NOTDUTY	:
		strMSG = CTChart::LoadString( TSTR_GUILD_CABINET_NOTDUTY);
		break;

	default							:
		strMSG = CTChart::LoadString( TSTR_GUILD_CABINET_TAKEOUTFAIL);
		break;
	}

	if( !strMSG.IsEmpty() )
		ShowInfoChat(strMSG, TUISND_TYPE_ERROR);

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDCONTRIBUTION_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket) 
		>> bResult;

	CTGuildCommander::GetInstance()
		->RecvGuildDonation(bResult);

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDARTICLELIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bCount;
	
	(*pPacket)
		>> bCount;

	CTGuildCommander::GuildNotify noti;
	CTGuildCommander::GuildNotifyVec nlist;

	for(WORD i=0; i<bCount; ++i)
	{
		(*pPacket)
			>> noti.m_dwID
			>> noti.m_bDuty
			>> noti.m_strAuthor
			>> noti.m_strTitle
			>> noti.m_strText
			>> noti.m_strDate;	

		nlist.push_back(noti);
	}

	CTGuildCommander::GetInstance()
		->RecvGuildNotifyList(&nlist);

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDARTICLEADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDARTICLEDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDFAME_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwMark;
	DWORD dwMarkColor;

	(*pPacket)
		>> bResult
		>> dwMark
		>> dwMarkColor;

	CString strMSG;
	TUISND_TYPE eType;
	if( bResult == GUILD_SUCCESS && dwMark )
	{
		strMSG = CTChart::LoadString( TSTR_FMT_GUILDMARKREG_SUCC);
		eType = TUISND_TYPE_INFO;
	}
	else if( bResult == GUILD_NOPOINT )
	{
		strMSG = CTChart::LoadString( TSTR_GUILDFAME_NO_POINT );
		eType = TUISND_TYPE_ERROR;
	}
	else
	{
		strMSG = CTChart::LoadString( TSTR_FMT_GUILDMARKREG_FAIL);
		eType = TUISND_TYPE_ERROR;
	}

	if( !strMSG.IsEmpty() )
		ShowInfoChat(strMSG, eType);

	return TERR_NONE;
}

bool SortGuildWanted(CTGuildCommander::GuildOffer& a, CTGuildCommander::GuildOffer& b)
{
	return a.m_dlTime > b.m_dlTime;
}

int CTClientGame::OnCS_GUILDWANTEDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCount;
	(*pPacket)
		>> dwCount;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	pCmd->m_GuildOfferVec.clear();
	
	for(DWORD i=0; i<dwCount; ++i)
	{
		CTGuildCommander::GuildOffer info;

		(*pPacket)
			//>> info.m_dwID
			>> info.m_dwGuildID
			>> info.m_strGuildName
			>> info.m_strTitle
			>> info.m_strText
			>> info.m_bMinLevel
			>> info.m_bMaxLevel
			>> info.m_dlTime
			>> info.m_bApplied;

		pCmd->m_GuildOfferVec.push_back( info );
	}

	std::sort(
		pCmd->m_GuildOfferVec.begin(),
		pCmd->m_GuildOfferVec.end(),
		SortGuildWanted );

	static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] )->ResetInfo();
	EnableUI( TFRAME_GUILDAPP );
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDWANTEDADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	(*pPacket)
		>> bResult;

	if( bResult != GUILD_SUCCESS )
	{
		CString strERR;
		strERR = CTChart::LoadString( TSTR_ERR_GUILDWANTEDADD);
		ShowInfoChat(strERR, TUISND_TYPE_ERROR);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDWANTEDDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDVOLUNTEERLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCount;

	(*pPacket)
		>> dwCount;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	pCmd->m_GuildVolunteerVec.clear();

	for(DWORD i=0; i<dwCount; ++i)
	{
		CTGuildCommander::GuildVolunteer ginfo;

		(*pPacket)
			>> ginfo.m_dwCharID
			>> ginfo.m_strName
			>> ginfo.m_bLevel
			>> ginfo.m_bClass
			>> ginfo.m_dwRegion;

		pCmd->m_GuildVolunteerVec.push_back( ginfo );
	}

	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	pFrame->ResetInfo();

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDVOLUNTEERREPLY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDVOLUNTEERING_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	CString strMSG;
	TUISND_TYPE eSndType;
	
	(*pPacket)
		>> bResult;

	switch(bResult)
	{
	case GUILD_SUCCESS	:
		{
			strMSG = CTChart::LoadString( TSTR_GUILDVOLUNTEERING_SUCC);
			eSndType = TUISND_TYPE_INFO;
		}
		break;

	case GUILD_HAVEGUILD:
		{
			strMSG = CTChart::LoadString( TSTR_GUILDVOLUNTEERING_HAVEGUILD);
			eSndType = TUISND_TYPE_ERROR;
		}
		break;
	case GUILD_AREADYAPPLY:
		{
			strMSG = CTChart::LoadString( TSTR_GUILD_AREADYAPPLY );
			eSndType = TUISND_TYPE_ERROR;
		}
		break;
	case GUILD_WANTEDEND:
		{
			strMSG = CTChart::LoadString( TSTR_GUILD_WANTEDEND );
			eSndType = TUISND_TYPE_ERROR;
		}
		break;
	case GUILD_MISMATCHLEVEL:
		{
			strMSG = CTChart::LoadString( TSTR_GUILD_MISMATCHLEVEL );
			eSndType = TUISND_TYPE_ERROR;
		}
		break;
	case GUILD_SAMEGUILDTACTICS:
		{
			strMSG = CTChart::LoadString( TSTR_GUILD_SAMEGUILDTACTICS );
			eSndType = TUISND_TYPE_ERROR;
		}
		break;
	default:
		{
			strMSG = CTChart::LoadString( TSTR_GUILD_VOLUNTEERING_FAIL );
			eSndType = TUISND_TYPE_ERROR;
		}
		break;
	}

	if( !strMSG.IsEmpty() )
		ShowInfoMsgBox(strMSG, TRUE, eSndType);

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDVOLUNTEERINGDEL_ACK( CTachyonSession* pSession, CPacket* pPacket )
{
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCount;
	
	(*pPacket)
		>> dwCount;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	pCmd->m_GuildTacticsVec.clear();

	for(BYTE i=0; i<dwCount; ++i)
	{
		CTGuildCommander::GuildTactics member;

		(*pPacket)
			>> member.m_dwCharID
			>> member.m_strName
			>> member.m_bLevel
			>> member.m_bClass
			>> member.m_bDay
			>> member.m_dwPoint
			>> member.m_dlEndTime
			>> member.m_dwGotPoint //계약기간동안 얻은 공적 포인트
			>> member.m_dwRegion
			>> member.m_wCastle
			>> member.m_bCamp;

		pCmd->m_GuildTacticsVec.push_back( member );
	}

	CTCommunityDlg* pComm = static_cast<CTCommunityDlg*>( m_vTFRAME[ TFRAME_COMMUNITY ] );
	if( pComm->IsVisible() &&
		pComm->IsSelect(TCOMMUNITY_FRAME_GUILDMEMBER) )
		pComm->ResetInfo();

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSANSWER_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strMSG;
	BYTE bResult;
	DWORD dwNewTacticsMemberID; //용병길드아이디

	(*pPacket)
		>> bResult
		>> dwNewTacticsMemberID;

	if( bResult == GUILD_SUCCESS )
	{
		if( dwNewTacticsMemberID == m_pMainChar->m_dwID ) // 용병 대상자
		{
			EnableGuildUI( TRUE );
		}
		else // 용병초대길드장
		{
			strMSG = CTChart::LoadString( TSTR_GUILD_TACTICS_ACCEPT );
		}
	}
	else
	{
		if( dwNewTacticsMemberID == m_pMainChar->m_dwID ) // 용병 대상자
		{
		}
		else // 용병초대길드장
		{
			switch( bResult )
			{
			case GUILD_JOIN_DENY:
				strMSG = CTChart::LoadString( TSTR_GUILD_TACTICS_REJECT );
				break;
			case GUILD_JOIN_BUSY:
				strMSG = CTChart::LoadString( TSTR_GUILD_TACTICS_BUSY );
				break;
			case GUILD_SAMEGUILDTACTICS:
				strMSG = CTChart::LoadString( TSTR_GUILD_SAMEGUILDTACTICS );
				break;
			case GUILD_HAVEGUILD:
				strMSG = CTChart::LoadString( TSTR_GUILD_ALREADY_TACTICS );
				break;
			case GUILD_MEMBER_FULL:
				strMSG = CTChart::LoadString( TSTR_GUILD_MEMBER_FULL );
				break;
			case GUILD_NOMONEY:
				strMSG = CTChart::LoadString( TSTR_GUILD_NOMONEY );
				break;
			case GUILD_NOPOINT:
				strMSG = CTChart::LoadString( TSTR_GUILD_NOPOINT );
				break;
			case GUILD_NOTFOUND:
				strMSG = CTChart::LoadString( TSTR_GUILD_NOT_ONLINE );
				break;
			}
		}
	}

	if( !strMSG.IsEmpty() )
	{
		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDARTICLEUPDATE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYMEMBERRECALL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bType;			// 0:파티원 소환 1:대상으로 이동
	CString strTarget;

	(*pPacket)
		>> bResult
		>> bType
		>> strTarget;

	ResetInvenUI();

	CString strMSG;
	TUISND_TYPE eType;

	if( bType == 0 )
	{
		switch(bResult)
		{
		case IU_SUCCESS	:
			{
				strMSG = CTChart::Format( TSTR_IU_SUCCESS, strTarget);
				eType = TUISND_TYPE_INFO;
			}
			break;

		case IU_DEALING			: //아이탬거래중
			{
				strMSG = CTChart::Format( TSTR_ERR_IU_DEALING, strTarget);
				eType = TUISND_TYPE_ERROR;
			}
			break;

		case IU_NOTFOUND		: //아이탬없던지 잘못썼던지
			{
				strMSG = CTChart::LoadString( TSTR_ERR_IU_NOTFOUND);
				eType = TUISND_TYPE_ERROR;
			}
			break;

		default					:
			{
				strMSG = CTChart::Format( TSTR_ERR_IU_BUSY, strTarget);
				eType = TUISND_TYPE_ERROR;
			}
			break;
		}
	}
	else
	{
		switch(bResult)
		{
		case IU_SUCCESS			:
			break;

		case IU_NOTFOUND		:
			{
				strMSG = CTChart::LoadString( TSTR_ERR_IU_NOTFOUND);
				eType = TUISND_TYPE_ERROR;
			}
			break;

		case IU_TARGETNOTFOUND	:
			{
				strMSG = CTChart::LoadString( TSTR_ERR_IU_TARGETNOTFOUND);
				eType = TUISND_TYPE_ERROR;
			}
			break;

		default					:
			{
				strMSG = CTChart::Format( TSTR_ERR_MOVETO, strTarget);
				eType = TUISND_TYPE_ERROR;
			}
			break;
		}
	}

	if( !strMSG.IsEmpty() )
		ShowInfoChat(strMSG, eType);

	return TERR_NONE;
}

int CTClientGame::OnCS_PARTYMEMBERRECALLANS_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CString strInviter;
	BYTE bInvenID;
	BYTE bItemID;

	(*pPacket)
		>> strInviter
		>> bInvenID
		>> bItemID;

	if( IsBusy() )
		m_pSESSION->SendCS_PARTYMEMBERRECALLANS_REQ(ASK_BUSY, strInviter, bInvenID, bItemID);
	else
	{
		m_bPartyRecallName		= strInviter;
		m_bPartyRecallAnsInven	= bInvenID;
		m_bPartyRecallAnsSlot	= bItemID;

		CString strMSG;
		strMSG = CTChart::Format( TSTR_FMT_PARTYMEMBERRECALLANS, strInviter, m_pMainChar->m_strNAME);

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_PARTYMEMBERRECALLANS_OK,
			GM_PARTYMEMBERRECALLANS_NO,
			0,
			FALSE );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CASHITEMCABINET_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	m_bLockGM_CASHCAB_GETITEM = FALSE;

	CTCashCabinetDlg* pDlg = static_cast<CTCashCabinetDlg*>(m_vTFRAME[TFRAME_CASHCABINET]);

	__int64 ldwTime;
	DWORD dwCash = 0;
	DWORD dwBonusCash = 0;

	(*pPacket)
		>> ldwTime
		>> dwCash
		>> dwBonusCash;

	DWORD dwItemCount=0;

	WORD wItemCount;

	(*pPacket)
		>> wItemCount;

	ResetInvenUI();

	pDlg->ClearAll();

	for(WORD i=0; i<wItemCount; ++i)
	{
		DWORD dwStItemID;
		(*pPacket)
			>> dwStItemID;
		
		CTClientItem* pItem = new CTClientItem;

		pItem->SetItemData(pPacket);
		pItem->SetItemMagic(pPacket);

		if( !pItem->CheckValid() )
		{
			delete pItem;
			continue;
		}

		CTCabinetItem* pCabItem = new CTCabinetItem;
		pCabItem->m_dwStItemID = dwStItemID;
		pCabItem->m_pClientItem = pItem;
		pCabItem->m_bItemCnt = pItem->GetCount();

		pDlg->AddItem(0, pCabItem);
	}
	
	pDlg->SetInfoCashCabinet(
		CTime(ldwTime),
		dwItemCount,
		dwCash,
		dwBonusCash );

	pDlg->ReCountItem();
	pDlg->CompleteCabUpdate();
	pDlg->SelectCab(0);
	pDlg->SelectLastestItem();
	pDlg->NotifyUpdate();

	if( pDlg->m_bRequestShowCashCabinet )
	{
		if( !m_vTFRAME[TFRAME_CASHCABINET]->IsVisible() )
			EnableUI(TFRAME_CASHCABINET);

		pDlg->m_bRequestShowCashCabinet = FALSE;
	}
	else
	{
		if( wItemCount != 0 )
			static_cast<CTMainUI*>(m_vTFRAME[ TFRAME_MAIN ])->SetNewCashCabinet(TRUE);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CASHITEMGET_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	TERROR_CODE bRET = TERR_NONE;

	BYTE bResult;
	DWORD dwCashItemID;

	(*pPacket)
		>> bResult
		>> dwCashItemID;

	if( bResult == CASHSHOP_SUCCESS )
	{
		CTCashCabinetDlg* pCashCabinetDlg = static_cast<CTCashCabinetDlg*>(m_vTFRAME[TFRAME_CASHCABINET]);

		if( pCashCabinetDlg)
			pCashCabinetDlg->DeleteItem( 0, dwCashItemID );

		if( m_pSESSION )
		{
			pCashCabinetDlg->m_bRequestShowCashCabinet = TRUE;
			m_pSESSION->SendCS_CASHITEMCABINET_REQ();
		}
	}
	else
	{
		switch( bResult )
		{
		case CASHSHOP_INVENFULL: bRET = TERR_ITEM_NO_SLOT; break;
		case CASHSHOP_NEEDCASH:
		case CASHSHOP_INTERNAL: bRET = TERR_NEED_CASH; break;
		case CASHSHOP_NOTFOUND: bRET = TERR_ITEM_NOT_FOUND; break;
		default: break;
		}

		m_bLockGM_CASHCAB_GETITEM = FALSE;
	}

	ResetInvenUI();

	return bRET;
}

int CTClientGame::OnCS_CASHSHOPITEMLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTNewCashShopDlg* pNewShopDlg = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];

	DWORD dwCash;
	DWORD dwCashBonus;
	BYTE bCategoryCount;

	(*pPacket)
		>> dwCash
		>> dwCashBonus
		>> bCategoryCount;

	pNewShopDlg->SetCash( dwCash, dwCashBonus );
	pNewShopDlg->ClearAllCategory();
	pNewShopDlg->AddCategory( 0, _T("New") );

	INT n = 0;
	for( BYTE i=0 ; i < bCategoryCount ; ++i )
	{
		CString strCategoryName;
		WORD wItemCount;

		(*pPacket)
			>> strCategoryName
			>> wItemCount;

		if( wItemCount == 0 )
			continue;

		pNewShopDlg->AddCategory( ++n, strCategoryName );

		for( WORD w=0 ; w < wItemCount ; ++w )
		{
			WORD wCashItemID;
			WORD wInfoID;
			DWORD dwMoney;
			BYTE bKind;							//New=1, Best=2
			WORD wItemID;
			BYTE bLevel;
			BYTE bCount;
			DWORD dwDuraMax;						//내구도
			DWORD dwDuraCurrent;
			BYTE bRefineMax;
			BYTE bRefineCurrent;
			BYTE bGLevel;
			WORD wUseDay;						//사용 기한
			BYTE bGradeEffect;
			BYTE bWrap = 0;
			BYTE bELD = 0;
			WORD wColor = 0;
			BYTE bRegGuild = 0;

			(*pPacket)
				>> wCashItemID
				>> wInfoID
				>> dwMoney
				>> bKind
				>> wItemID
				>> bLevel
				>> bCount
				>> dwDuraMax
				>> dwDuraCurrent
				>> bRefineMax
				>> bRefineCurrent
				>> bGLevel
				>> wUseDay	
				>> bGradeEffect
				>> bELD
				>> bWrap // jkchoi
				>> wColor
				>> bRegGuild;

			CTClientItem* pItem = new CTClientItem;
			pItem->SetItemMagic(pPacket);

			pItem->SetItemID( wInfoID );
			pItem->SetGrade( bLevel );
			pItem->SetCount( bCount );
			pItem->SetDuraMax( dwDuraMax );
			pItem->SetDuraCurrent( dwDuraCurrent );
			pItem->SetRefineMax( bRefineMax );
			pItem->SetRefineCurrent( bRefineCurrent );
			pItem->SetCanGamble( bGLevel );
			pItem->SetGradeEffect( bGradeEffect );
			pItem->SetWrap( bWrap );
			pItem->SetELD( bELD );
			pItem->SetColor( wColor );
			pItem->SetRegGuild( bRegGuild );

			if( !pItem->CheckValid() )
			{
				delete pItem;
				continue;
			}

			CTCashShopItem* pCashItem = new CTCashShopItem;
			pCashItem->m_wCashItemID = wCashItemID;
			pCashItem->m_wItemID = wItemID;
			pCashItem->m_pClientItem = pItem;
			pCashItem->m_bItemCnt = pItem->GetCount();
			pCashItem->m_dwPrice = dwMoney;
			pCashItem->m_wUseDay = wUseDay;
			pCashItem->m_bKind = bKind;

			pNewShopDlg->AddItemToCategory( n, pCashItem );

			if( bKind == CASHSHOPITEM_KIND_BEST ) // Best
			{
				CTClientItem* pNewItem = new CTClientItem;
				*pNewItem = *pItem;

				CTCashShopItem* pBestItem = new CTCashShopItem;
				pBestItem->m_wCashItemID = wCashItemID;
				pBestItem->m_wItemID = wItemID;
				pBestItem->m_pClientItem = pNewItem;
				pBestItem->m_bItemCnt = pNewItem->GetCount();
				pBestItem->m_dwPrice = dwMoney;
				pBestItem->m_wUseDay = wUseDay;
				pBestItem->m_bKind = bKind;

				pNewShopDlg->AddItemToNewCategory( pBestItem );
			}
			else if( bKind == CASHSHOPITEM_KIND_NEW )
			{
				CTClientItem* pNewItem = new CTClientItem;
				*pNewItem = *pItem;

				CTCashShopItem* pBestItem = new CTCashShopItem;
				pBestItem->m_wCashItemID = wCashItemID;
				pBestItem->m_wItemID = wItemID;
				pBestItem->m_pClientItem = pNewItem;
				pBestItem->m_bItemCnt = pNewItem->GetCount();
				pBestItem->m_dwPrice = dwMoney;
				pBestItem->m_wUseDay = wUseDay;
				pBestItem->m_bKind = bKind;

				pNewShopDlg->AddItemToCategory( 0, pBestItem );
			}
		}
	}

	CTCashShopCategory* pNewCategory = pNewShopDlg->GetCategory( 0 );
	if( 0 == pNewCategory->GetItemCount() )
	{
		pNewShopDlg->DeleteCategory( 0 );
		pNewShopDlg->OpenTopCategory();
	}
	else
		pNewShopDlg->OpenCategory( 0 );
	
	if( CTNewCashShopDlg::m_ExtendPeriodInfo.wItemID == 0 )
	{
		pNewShopDlg->SetCashShopDlgMode( CASHSHOPDLGMODE_NORMAL );
		EnableUI( TFRAME_CASHSHOP_NEW ); //EnableUI(TFRAME_CASHSHOP);
	}
	else
	{
		pNewShopDlg->SetCashShopDlgMode( CASHSHOPDLGMODE_PERIOD_EXTENSION );
		EnableUI( TFRAME_CASHSHOP_NEW ); //EnableUI(TFRAME_CASHSHOP);
	}

	m_bInvenID = 0;
	m_bSlotID = 0;

	return TERR_NONE;
}

int CTClientGame::OnCS_CASHITEMBUY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	
	(*pPacket)
		>> bResult;
		
	switch(bResult)
	{
	case CASHSHOP_SUCCESS	:
		{
			DWORD dwCash;
			DWORD dwCashBonus;

			(*pPacket)
				>> dwCash
				>> dwCashBonus;

			CTNewCashShopDlg* pNewShopDlg = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];
			pNewShopDlg->SetCash( dwCash, dwCashBonus);
			
			ResetInvenUI();

			CString strMSG;

			if ( pNewShopDlg->GetPresent() )
				strMSG = CTChart::LoadString( TSTR_CASHITEMPRESENT_SUCCESS );
			else
				strMSG = CTChart::LoadString( TSTR_CASHITEMBUY_SUCCESS);

			ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_INFO);
			static_cast<CTMainUI*>(m_vTFRAME[ TFRAME_MAIN ])->SetNewCashCabinet(TRUE);
		}
		break;

	case CASHSHOP_INVENFULL	:	return TERR_ITEM_NO_SLOT;
	case CASHSHOP_NEEDCASH	:	
	case CASHSHOP_INTERNAL	:	return TERR_NEED_CASH;
	case CASHSHOP_NOTARGET:	return TERR_CASHSHOP_NOTARGET;
	case CASHSHOP_NOCASHINFO:	return TERR_CASHSHOP_NOCASHINFO;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CASHITEMPRESENT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	switch(bResult)
	{
	case CASHSHOP_SUCCESS	:
		{
			CString strMSG;
			strMSG = CTChart::LoadString( TSTR_CASHITEMPRESENT_SUCCESS);
			ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_INFO);
		}
		break;

	case CASHSHOP_INTERNAL	:	return TERR_NEED_CASH;
	default					:	return TERR_NOTARGET_CASH;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_RESETPCBANG_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	BYTE bInPCROOM;

	(*pPacket)
		>> dwCharID
		>> bInPCROOM;
	
	if( m_pMainChar->m_dwID == dwCharID )
	{
		m_pMainChar->m_bInPCROOM = bInPCROOM;
		m_pMainWnd->m_bInPCROOM = bInPCROOM;
	}
	else
	{
		CTClientChar* pChar = FindPlayer(dwCharID);
		if( pChar )
			pChar->m_bInPCROOM = bInPCROOM;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CASHCABINETBUY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	if( CTNationOption::RUSSIA )
	{
	BYTE bResult;
	__int64 ldwTime;

	(*pPacket) 
		>> bResult
		>> ldwTime;

	switch( bResult )
	{
	case CASHSHOP_SUCCESS:
		{
//			CTNewCashShopDlg* pNewShopDlg = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];
//			pNewShopDlg->SetCash( dwCash, dwCashBonus );

			CTCashCabinetDlg* pCabinetDlg = static_cast<CTCashCabinetDlg*>(m_vTFRAME[TFRAME_CASHCABINET]);
			pCabinetDlg->SetInfoCashCabinet(
				CTime(ldwTime),
				pCabinetDlg->m_dwItemCount,
				pCabinetDlg->m_dwCash,
				pCabinetDlg->m_dwBonusCash);
		}
		break;

	case CASHSHOP_INVENFULL : return TERR_ITEM_NO_SLOT;
	case CASHSHOP_NEEDCASH :	
	case CASHSHOP_INTERNAL : return TERR_NEED_CASH;
	case CASHSHOP_NOTARGET : return TERR_CASHSHOP_NOTARGET;
	case CASHSHOP_NOCASHINFO : return TERR_CASHSHOP_NOCASHINFO;
	case CASHSHOP_NOITEM : return TERR_NOITEM;
	}

	return TERR_NONE;
	}
	else
	{
		BYTE bResult;
		__int64 ldwTime;
		DWORD dwCash;
		DWORD dwCashBonus;

		(*pPacket) >> bResult
			>> ldwTime
			>> dwCash
			>> dwCashBonus;

		switch( bResult )
		{
		case CASHSHOP_SUCCESS:
			{
				CTNewCashShopDlg* pNewShopDlg = (CTNewCashShopDlg*) m_vTFRAME[ TFRAME_CASHSHOP_NEW];
				pNewShopDlg->SetCash( dwCash, dwCashBonus );

				CTCashCabinetDlg* pCabinetDlg = static_cast<CTCashCabinetDlg*>(m_vTFRAME[TFRAME_CASHCABINET]);
				pCabinetDlg->SetInfoCashCabinet(
					CTime(ldwTime),
					pCabinetDlg->m_dwItemCount,
					dwCash,
					dwCashBonus);
			}
			break;

		case CASHSHOP_INVENFULL : return TERR_ITEM_NO_SLOT;
		case CASHSHOP_NEEDCASH :	
		case CASHSHOP_INTERNAL : return TERR_NEED_CASH;
		case CASHSHOP_NOTARGET : return TERR_CASHSHOP_NOTARGET;
		case CASHSHOP_NOCASHINFO : return TERR_CASHSHOP_NOCASHINFO;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CASHITEMPUTIN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	m_bSendedCS_CASHITEMPUTIN_REQ = FALSE;

	TERROR_CODE bRET = TERR_NONE;

	CTCashCabinetDlg* pCabinetDlg = static_cast<CTCashCabinetDlg*>(m_vTFRAME[TFRAME_CASHCABINET]);

	if( !pCabinetDlg )
		return TERR_NONE;

	BYTE bResult;

	(*pPacket)
		>> bResult;

	if( bResult == CASHSHOP_SUCCESS )
	{
		DWORD dwNewID;
		(*pPacket)
			>> dwNewID;

		CTClientItem* pItem = new CTClientItem;

		pItem->SetItemData(pPacket);
		pItem->SetItemMagic(pPacket);

		if( !pItem->CheckValid() )
		{
			delete pItem;
			return TERR_NONE;
		}

		CTCabinetItem* pCabItem = new CTCabinetItem;
		pCabItem->m_dwStItemID = dwNewID;
		pCabItem->m_pClientItem = pItem;
		pCabItem->m_bItemCnt = pItem->GetCount();

		pCabinetDlg->AddItem(0, pCabItem);

		if( m_pSESSION )
		{
			pCabinetDlg->m_bRequestShowCashCabinet = TRUE;
			m_pSESSION->SendCS_CASHITEMCABINET_REQ();
		}
	}
	else
	{
		switch( bResult )
		{
		case CASHSHOP_NOTFOUND: bRET = TERR_CASHSHOP_NOTFOUND; break;
		case CASHSHOP_MAXCOUNT: bRET = TERR_CASHSHOP_MAXCOUNT; break;
		case CASHSHOP_NEEDEXTEND: bRET = TERR_CASHSHOP_NEEDEXTEND; break;
		case CASHSHOP_NOITEM: bRET = TERR_NOITEM; break;
		default: break;
		};
	}

	ResetInvenUI();

	return bRET;
}

int CTClientGame::OnCS_CASHCABINETBUYCASH_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwNeedCash;

	(*pPacket)
		>> dwNeedCash;

	CString strFMT;
	strFMT = CTChart::LoadString( TSTR_CASH_WOULDBUY);
	
	CString strMSG;
	strMSG.Format( strFMT,
		dwNeedCash);

	m_pMainWnd->MessageBoxYesNo(
		strMSG,
		TSTR_BUY,
		TSTR_CANCEL,
		GM_CASHCAB_BUYCABINET_YES,
		GM_CLOSE_MSGBOX );

	return TERR_NONE;
}

int CTClientGame::OnCS_SOULMATE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	(*pPacket)
		>> m_dwSoulID
		>> m_strSoulName;

	m_pMessengerBase->GetMsgGroupList()->UpdateSoulmateInfo(
		m_dwSoulID,
		m_strSoulName,
		0,
		255, 255, 255 );

	return TERR_NONE;
}

int CTClientGame::OnCS_SOULMATESEARCH_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwCharID;		// 소울메이트아이디
	CString strName;	// 소울메이트이름
	DWORD dwRegion;
	
	(*pPacket)
		>> bResult
		>> dwCharID
		>> strName
		>> dwRegion;

	DWORD dwEndGM;

	CTProgressRegDlg* pDlg = static_cast<CTProgressRegDlg*>( m_vTFRAME[TFRAME_PROGRESS_REG] );

	switch( bResult )
	{
	case SOULMATE_SUCCESS	:	//성공
		{
			(*pPacket)
				>> m_pMainChar->m_dwRune
				>> m_pMainChar->m_dwLuna
				>> m_pMainChar->m_dwCron;
		
			ResetInvenUI();

			m_dwSoulRegion = dwRegion;
			m_dwSoulID = dwCharID;
			m_strSoulName = strName;

			dwEndGM = GM_SOULMATE_SUCCESS;
		}
		break;

	case SOULMATE_SILENCE	:	//영혼의침묵기
		return TERR_SOULMATE_SILENCE;
	case SOULMATE_NEEDMONEY :	//돈부족
		return TERR_SOULMATE_NEEDMONEY;
	
	default					:	//검색실패
		dwEndGM = GM_SOULMATE_NOTFOUND;
		break;
	}

	CString strTITLE;
	strTITLE = CTChart::LoadString( TSTR_SOULMATESEARCH_TITLE);

	CString strMSG;
	strMSG = CTChart::LoadString( TSTR_SOULMATESEARCH_MSG);

	pDlg->Show(
		strTITLE,
		strMSG,
		TSOULMATE_REG_DELAY,
		dwEndGM);

	return TERR_NONE;
}

int CTClientGame::OnCS_SOULMATEREGREADY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	CString strName;

	WORD wNPCID = 0;
	BYTE bInvenID = 0;
	BYTE bSlotID = 0;

	(*pPacket)
		>> bResult
		>> strName
		>> bInvenID
		>> bSlotID;

	WORD wItemID = MAKEWORD( bSlotID, bInvenID );
	DWORD dwPARAM = MAKELONG( wItemID, 0 );

	switch( bResult )
	{
	case SOULMATE_SUCCESS	:	//성공
		{
			m_strSoulRelay = strName;

			UINT64 nCOST = CTChart::GetSoulmateSearchPrice(m_pMainChar->m_bLevel);
			CString strCOST = MakeMoneyStr(nCOST);

			CString strMSG;
			strMSG = CTChart::Format( TSTR_SOULMATEBOX_REG, strCOST);

			m_pMainWnd->MessageBoxYesNo(
				strMSG,
				TSTR_YES,
				TSTR_NO,
				TCOMMAND( GM_SOULMATE_REGOK, dwPARAM ),
				GM_SOULMATE_NO,
				0 );
		}
		break;

	case SOULMATE_SILENCE 	:	//영혼의침묵기
		return TERR_SOULMATE_SILENCE;
	default					:	//실패
		return TERR_SOULMATE_FAIL;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SOULMATEREG_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwCharID;		// 소울메이트아이디
	CString strName;	// 소울메이트이름
	DWORD dwRegion;

	(*pPacket)
		>> bResult
		>> dwCharID
		>> strName
		>> dwRegion;

	DWORD dwEndGM;

	CTProgressRegDlg* pDlg = static_cast<CTProgressRegDlg*>( m_vTFRAME[TFRAME_PROGRESS_REG] );

	switch( bResult )
	{
	case SOULMATE_SUCCESS	:	//성공
		{
			(*pPacket)
				>> m_pMainChar->m_dwRune
				>> m_pMainChar->m_dwLuna
				>> m_pMainChar->m_dwCron;
		
			ResetInvenUI();

			m_dwSoulRegion = dwRegion;
			m_dwSoulID = dwCharID;
			m_strSoulName = strName;

			dwEndGM = GM_SOULMATE_SUCCESS;
		}
		break;

	case SOULMATE_NEEDMONEY :	//돈부족
		return TERR_SOULMATE_NEEDMONEY;
	
	default					:	//실패
		dwEndGM = GM_SOULMATE_FAIL; 
		break;
	}

	CString strTITLE;
	strTITLE = CTChart::LoadString( TSTR_SOULMATESEARCH_TITLE);

	CString strMSG;
	strMSG = CTChart::LoadString( TSTR_SOULMATESEARCH_MSG);

	pDlg->Show(
		strTITLE,
		strMSG,
		TSOULMATE_REG_DELAY,
		dwEndGM);

	return TERR_NONE;
}

int CTClientGame::OnCS_SOULMATEEND_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	if( bResult == SOULMATE_SUCCESS )
	{
		CString strMSG;
		strMSG = CTChart::Format( TSTR_SOULMATEEND_SUCCESS, m_strSoulName);
		ShowInfoChat(strMSG, TUISND_TYPE_INFO);

		m_dwSoulID = 0;
		m_strSoulName.Empty();
		m_dwSoulRegion = 0;

		m_pMessengerBase->GetMsgGroupList()->UpdateSoulmateInfo(
			m_dwSoulID,
			m_strSoulName,
			m_dwSoulRegion,
			255, 255, 255 );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GAMBLECHECK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bInven;
	BYTE bItemID;
	DWORD dwNeedMoney;
	BYTE bDiscountRate;

	(*pPacket)
		>> bResult
		>> bInven
		>> bItemID
		>> dwNeedMoney
		>> bDiscountRate;

	if( bResult == GAMBLE_SUCCESS )
	{
		CString strMSG;

		if( bDiscountRate )
		{
			dwNeedMoney -= dwNeedMoney * bDiscountRate / 100;
			CString strCost = MakeMoneyStr(dwNeedMoney);
			strMSG = CTChart::Format( TSTR_GAMBLE_ASK_SALE,
				(LPCTSTR)strCost,
				bDiscountRate);
		}
		else
		{
			CString strCost = MakeMoneyStr(dwNeedMoney);
			strMSG = CTChart::Format( TSTR_GAMBLE_ASK, (LPCTSTR)strCost);
		}

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_GAMBLE_PROGRESS,
			GM_GAMBLE_NO );
	}
	else
	{
		CString strMSG;

		switch( bResult )
		{
			case GAMBEL_NEEDMONEY:
				strMSG = CTChart::LoadString( TSTR_GAMBLE_FAIL_NEEDMONEY);
				break;

			case GAMBLE_INVALIDITEM:
			default:
				strMSG = CTChart::LoadString( TSTR_GAMBLE_CHECK_FAIL);
				break;
		}

		ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_ERROR);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GAMBLEOPEN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	CString strMSG;
	TUISND_TYPE eTYPE;
	CTClientItem* pTItem = NULL;

	if( bResult == GAMBLE_SUCCESS ||
		bResult == GAMBLE_REPFAIL )
	{
		BYTE bInvenID;
		BYTE bItemID;

		(*pPacket)
			>> m_pMainChar->m_dwRune
			>> m_pMainChar->m_dwLuna
			>> m_pMainChar->m_dwCron
			>> bInvenID
			>> bItemID;

		CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);
		if( !pTInven )
			return TERR_NONE;

		pTItem = pTInven->FindTItem(bItemID);
		if( !pTItem )
			return TERR_NONE;

		pTItem->SetItemSlot(bItemID);
		pTItem->SetItemData(pPacket);
		pTItem->SetItemMagic(pPacket);
	}

	if( pTItem )
		pTItem->SetCanGamble(0);

	switch(bResult)
	{
	case GAMBLE_SUCCESS	: 
		{
			eTYPE = TUISND_TYPE_INFO;
			strMSG = CTChart::LoadString( TSTR_GAMBLE_SUCC);
		}
		break;
	case GAMBLE_REPFAIL	: 
		{
			eTYPE = TUISND_TYPE_ERROR;
			strMSG = CTChart::LoadString( TSTR_GAMBLE_FAIL_REP);
		}
		break;
	case GAMBLE_FAIL	:
		{
			eTYPE = TUISND_TYPE_ERROR;
			strMSG = CTChart::LoadString( TSTR_GAMBLE_FAIL_OCC);
		}
		break;
	case GAMBEL_NEEDMONEY	: 
		{
			eTYPE = TUISND_TYPE_ERROR;
			strMSG = CTChart::LoadString( TSTR_GAMBLE_FAIL_NEEDMONEY);
		}
		break;
	default					:
		{
			eTYPE = TUISND_TYPE_ERROR;
			strMSG = CTChart::LoadString( TSTR_GAMBLE_FAIL);
		}
		break;
	}

	((CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE])->ResetHotkeyUI();
	ResetInvenUI();

	if( !strMSG.IsEmpty() )
		ShowInfoMsgBox(strMSG, TRUE, eTYPE);

	return TERR_NONE;
}

int CTClientGame::OnCS_ADDGODTOWER_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wGodTowerID;
	WORD wGodBallID;
	BYTE bCamp;
	D3DXVECTOR3 vPos;

	(*pPacket)
		>> wGodTowerID
		>> vPos.x
		>> vPos.y
		>> vPos.z
		>> wGodBallID
		>> bCamp;

	SiegeMgr->RecvAddGodTower( wGodTowerID, bCamp, vPos );


	// debug..
	TRACE("CS_ADDGODTOWER_ACK (wID:%u, wGodBallID:%u, bCamp:%u Pos(%f,%f,%f) )\n", 
		wGodTowerID,
		wGodBallID,
		bCamp,
		vPos.x,
		vPos.y,
		vPos.z);

	return TERR_NONE;
}

int CTClientGame::OnCS_DELGODTOWER_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wGodTowerID;

	(*pPacket)
		>> wGodTowerID;

	SiegeMgr->RecvDelGodTower( wGodTowerID );


	// debug..
	TRACE("CS_DELGODTOWER_ACK (wID:%u)\n", 
		wGodTowerID);

	return TERR_NONE;
}

int CTClientGame::OnCS_ADDGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD		wGodBallID;
	BYTE		bCamp;
	BYTE		bInGround;
	D3DXVECTOR3	vPos;

	(*pPacket)
		>> wGodBallID
		>> bCamp
		>> bInGround
		>> vPos.x
		>> vPos.y
		>> vPos.z;

	SiegeMgr->RecvAddGodBall( wGodBallID, bCamp, (BOOL)bInGround, vPos );


	// debug..
	TRACE("CS_ADDGODBALL_ACK (wID:%u, bCamp:%u, vPos:(%.1f,%.1f,%.1f))\n", 
		wGodBallID, 
		bCamp,
		vPos.x,
		vPos.y,
		vPos.z);

	return TERR_NONE;
}

int CTClientGame::OnCS_DELGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wGodBallID;

	(*pPacket)
		>> wGodBallID;

	SiegeMgr->RecvDelGodBall( wGodBallID );


	// debug..
	TRACE("CS_DELGODBALL_ACK (wID:%u)\n", 
		wGodBallID);

	return TERR_NONE;
}

int CTClientGame::OnCS_TAKEGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	WORD wGodBallID;

	(*pPacket)
		>> dwCharID
		>> wGodBallID;

	SiegeMgr->RecvTakeGodBall( dwCharID, wGodBallID );


	// debug..
	TRACE("CS_TAKEGODBALL_ACK (dwCharID:%u, wGodBallID:%u)\n", 
		dwCharID, 
		wGodBallID);

	return TERR_NONE;
}

int CTClientGame::OnCS_REMOVEGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;

	(*pPacket)
		>> dwCharID;

	SiegeMgr->RecvRemoveGodBall( dwCharID );


	// debug..
	TRACE("CS_REMOVEGODBALL_ACK (dwCharID:%u)\n", 
		dwCharID);

	return TERR_NONE;
}


// 삭제.
int CTClientGame::OnCS_DROPGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	//WORD wGodBallID;
	//BYTE bCamp;
	//DWORD dwCharID;
	//D3DXVECTOR3 vPos;

	//(*pPacket)
	//	>> wGodBallID
	//	>> bCamp
	//	>> dwCharID
	//	>> vPos.x
	//	>> vPos.y
	//	>> vPos.z;

	//CTSiegeManager::GetInstance()
	//	->DropBallToGround(wGodBallID, bCamp, dwCharID, vPos);


	//// debug..
	//TRACE("CS_DROPGODBALL_ACK (wGodBallID:%u, bCamp:%u, dwCharID:%u, vPos:(%.1f,%.1f,%.1f))\n", 
	//	wGodBallID, 
	//	bCamp,
	//	dwCharID,
	//	vPos.x,
	//	vPos.y,
	//	vPos.z);

	return TERR_NONE;
}

int CTClientGame::OnCS_MOUNTGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD	wGodTower;
	WORD	wGodBall;
	BYTE	bCamp;
	DWORD	dwCharID;

	(*pPacket)
		>> wGodTower
		>> wGodBall
		>> bCamp
		>> dwCharID;

	SiegeMgr->RecvMountGodBall( wGodTower, wGodBall, bCamp, dwCharID );


	// debug..
	TRACE("CS_MOUNTGODBALL_ACK (wGodTower:%u, wGodBall:%u, bCamp:%u, dwCharID:%u)\n", 
		wGodTower, 
		wGodBall,
		bCamp,
		dwCharID);

	return TERR_NONE;
}

int CTClientGame::OnCS_DEMOUNTGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD	wGodTower;
	DWORD	dwCharID;

	(*pPacket)
		>> wGodTower
		>> dwCharID;

	SiegeMgr->RecvDemountGodBall( wGodTower );


	// debug..
	TRACE("CS_DEMOUNTGODBALL_ACK (wGodTower:%u, dwCharID:%u)\n", 
		wGodTower, 
		dwCharID);

	return TERR_NONE;
}

int CTClientGame::OnCS_BALANCEOFPOWER_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	FLOAT fPower_Def;
	DWORD dwLeftTick;
	WORD wKillPoint_Atk;
	WORD wKillPoint_Def;
	CString strOwner1_Atk;
	CString strOwner2_Atk;
	CString strOwner1_Def;
	CString strOwner2_Def;
	WORD wKeep1_Atk;
	WORD wKeep2_Atk;
	WORD wKeep1_Def;
	WORD wKeep2_Def;

	(*pPacket)
		>> fPower_Def
		>> dwLeftTick
		>> wKillPoint_Atk
		>> wKillPoint_Def
		>> strOwner1_Atk
		>> strOwner2_Atk
		>> strOwner1_Def
		>> strOwner2_Def
		>> wKeep1_Atk
		>> wKeep2_Atk
		>> wKeep1_Def
		>> wKeep2_Def;

	CTSiegeUI* pSiegeUI = static_cast< CTSiegeUI* >( m_vTFRAME[ TFRAME_SIEGEUI ] );
	
	pSiegeUI->RecvBalanceOfPower(
        fPower_Def,
		dwLeftTick,
		wKillPoint_Atk,
		wKillPoint_Def,
		strOwner1_Atk,
		strOwner2_Atk,
		strOwner1_Def,
		strOwner2_Def,
		wKeep1_Atk,
		wKeep2_Atk,
		wKeep1_Def,
		wKeep2_Def );

	return TERR_NONE;
}

int CTClientGame::OnCS_CASTLEAPPLY_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult = 0;
	WORD wCastle = 0;
	DWORD dwTarget = 0;
	BYTE bCamp = 0;

	(*pPacket)
		>> bResult
		>> wCastle
		>> dwTarget
		>> bCamp;

	CString strMSG;

	switch( bResult )
	{
	case CBS_FULL		: strMSG = CTChart::Format( TSTR_CASTLEAPPLY_FULL ); break;
	case CBS_NOTFOUND	: strMSG = CTChart::Format( TSTR_CASTLEAPPLY_NOTFOUND ); break;
	case CBS_NOTREADY	: strMSG = CTChart::Format( TSTR_CASTLEAPPLY_NOTREADY ); break;
	case CBS_CANTAPPLY	: strMSG = CTChart::Format( TSTR_CASTLEAPPLY_CANTAPPLY ); break;
	}

	if( bResult != CBS_SUCCESS )
	{
		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE);

		return TERR_NONE;
	}

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();

	{
		size_t size = pCmd->GetCountGuildMember();
		for( size_t i=0; i<size; ++i )
		{
			if( pCmd->GetGuildMemberPtr( i )->m_dwCharID == dwTarget )
			{
				pCmd->GetGuildMemberPtr( i )->m_wCastle = wCastle;
				pCmd->GetGuildMemberPtr( i )->m_bCamp = bCamp;
				m_pSESSION->SendCS_GUILDMEMBERLIST_REQ();
				return TERR_NONE;
			}
		}	
	}

	{
		size_t size = pCmd->GetCountTactics();
		for( size_t i=0; i<size; ++i )
		{
			if( pCmd->GetTacticsPtr( i )->m_dwCharID == dwTarget )
			{
				pCmd->GetTacticsPtr( i )->m_wCastle = wCastle;
				pCmd->GetTacticsPtr( i )->m_bCamp = bCamp;
				m_pSESSION->SendCS_GUILDTACTICSLIST_REQ();
				return TERR_NONE;
			}
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_ENDWAR_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bType = 0;
	DWORD dwWinGuildID = 0;
	DWORD dwDefTotal = 0;
	DWORD dwAtkTotal = 0;
	CString strDefName = "";
	DWORD dwDefPower = 0;
	WORD wDefPoint = 0;
	WORD wDefKillPoint = 0;
	CString strAtkName = "";
	DWORD dwAtkPower = 0;
	WORD wAtkPoint = 0;
	WORD wAtkKillPoint = 0;

	(*pPacket)
		>> bType
		>> dwWinGuildID
		>> dwDefTotal
		>> dwAtkTotal
		>> strDefName
		>> dwDefPower
		>> wDefPoint
		>> wDefKillPoint
		>> strAtkName
		>> dwAtkPower
		>> wAtkPoint
		>> wAtkKillPoint;

	SiegeMgr->Endwar(
		bType,
		dwWinGuildID,
		dwDefTotal,
		dwAtkTotal,
		strDefName,
		dwDefPower,
		wDefPoint,
		wDefKillPoint,
		strAtkName,
		dwAtkPower,
		wAtkPoint,
		wAtkKillPoint );

	return TERR_NONE;
}

int CTClientGame::OnCS_ENTERCASTLE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	if( !m_pMainChar )
		return TERR_NONE;

	CString strName_Atk;
	CString strName_Def;

	(*pPacket)
		>> m_pMainChar->m_wCastleID
		>> m_pMainChar->m_bCamp
		>> strName_Atk
		>> strName_Def;

	m_pMainChar->EnterCastle();

	CTSiegeUI* pSiegeUI = static_cast< CTSiegeUI* >( m_vTFRAME[TFRAME_SIEGEUI] );
	pSiegeUI->InitSiegeUI();
	pSiegeUI->SetAtkGuildName( strName_Atk );
	pSiegeUI->SetDefGuildName( strName_Def );
	pSiegeUI->ShowComponent();
	pSiegeUI->EnableComponent();
	
	CTShopDlg* pMonShop = static_cast< CTShopDlg* >( m_vTFRAME[TFRAME_MONSHOP] );
	pMonShop->EnableComponent( TRUE );

	SiegeMgr->Release();
	m_pMainChar->ClearGodBallSFX();
	m_pMainChar->SetGodBallID( 0 );

	return TERR_NONE;
}

int CTClientGame::OnCS_LEAVECASTLE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
   	CTSiegeUI* pSiegeUI = static_cast< CTSiegeUI* >( m_vTFRAME[TFRAME_SIEGEUI] );
	pSiegeUI->InitSiegeUI();
	pSiegeUI->ShowComponent( FALSE );
	pSiegeUI->EnableComponent( FALSE );

	CTSiegeEnd* pSiegeEnd = static_cast< CTSiegeEnd* >( m_vTFRAME[TFRAME_SIEGEEND] );
	pSiegeEnd->InitSiegeEnd();
	pSiegeEnd->ShowComponent( FALSE );
	pSiegeEnd->EnableComponent( FALSE );

	CTShopDlg* pMonShop = static_cast< CTShopDlg* >( m_vTFRAME[TFRAME_MONSHOP] );
	pMonShop->ShowComponent( FALSE );
	pMonShop->EnableComponent( FALSE );

	SiegeMgr->Release();

	m_pMainChar->LeaveCastle();
	m_pMainChar->ClearGodBallSFX();
	m_pMainChar->SetGodBallID( 0 );
	m_pMainChar->SetCamp( 0 );
	m_pMainChar->SetCastleID( 0 );

	return TERR_NONE;
}

int CTClientGame::OnCS_NPCMONSTERLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	
	return TERR_NONE;
}

int CTClientGame::OnCS_MONSTERBUY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	CString strMSG;
	switch( bResult )
	{
	case MSB_SUCCESS :
		strMSG = CTChart::Format( TSTR_MONBUY_SUCCESS );
		break;
	case MSB_INVALIDNPC :
		strMSG = CTChart::Format( TSTR_MONBUY_INVALIDNPC );
		break;
	case MSB_NOTFOUND :
		strMSG = CTChart::Format( TSTR_MONBUY_NOTFOUND );
		break;
	case MSB_NEEDMONEY :
		strMSG = CTChart::Format( TSTR_MONBUY_NEEDMONEY );
		break;
	case MSB_CAMPMISMATCH :
		strMSG = CTChart::Format( TSTR_MONBUY_CAMPMISMATCH );
		break;
	case MSB_AUTHORITY :
		strMSG = CTChart::Format( TSTR_MONBUY_AUTHORITY );
		break;
	case MSB_ALREADY :
		strMSG = CTChart::Format( TSTR_MONBUY_ALREADY );
		break;
	}

	m_pMainWnd->MessageBoxOK(
		strMSG,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		0 );

	return TERR_NONE;
}

int CTClientGame::OnCS_DURATIONDEC_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bInven;
	BYTE bItemID;
	DWORD dwDuration;

	(*pPacket)
		>> bInven
		>> bItemID
		>> dwDuration;

	CTClientInven* pTINVEN = m_pMainChar->FindTInven(bInven);
	if( pTINVEN )
	{
		CTClientItem* pTITEM = pTINVEN->FindTItem(bItemID);
		if( pTITEM )
			pTITEM->SetDuraCurrent(dwDuration);
	}

	m_pDurationUI->Update();

	return TERR_NONE;
}

int CTClientGame::OnCS_DURATIONEND_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bInven;
	BYTE bItemID;
	BYTE bDeleted;

	(*pPacket)
		>> bInven
		>> bItemID
		>> bDeleted;

	CTClientInven* pTINVEN = m_pMainChar->FindTInven(bInven);
	if( pTINVEN )
	{
		CTClientItem* pTITEM = pTINVEN->FindTItem(bItemID);
		if( pTITEM )
		{
			pTITEM->SetDuraCurrent(0);

			CString strMSG;
			if( bDeleted )
				strMSG = CTChart::Format( TSTR_FMT_DURATIONDEL, pTITEM->GetTITEM()->m_strNAME);
			else
				strMSG = CTChart::Format( TSTR_FMT_DURATIONEND, pTITEM->GetTITEM()->m_strNAME);

			ShowInfoChat(strMSG, TUISND_TYPE_ERROR);
		}
	}

	m_pDurationUI->Update();

	return TERR_NONE;
}

int CTClientGame::OnCS_DURATIONREPCOST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCost;
	BYTE bDiscountRate;

	(*pPacket)
		>> dwCost
		>> bDiscountRate;

	CString strMSG;
	TSTRING dwMsgType[] =
	{
		TSTR_FMT_REPAIRCOST_NORMAL,
		TSTR_FMT_REPAIRCOST_EQUIP,
		TSTR_FMT_REPAIRCOST_ALL,
	};

	if( bDiscountRate )
	{
		dwCost -= dwCost * bDiscountRate / 100;
		CString strCost = MakeMoneyStr(dwCost);
		strMSG = CTChart::Format( dwMsgType[ m_bDurationRepType ],
			(LPCTSTR)strCost,
			bDiscountRate);
	}
	else
	{
		CString strCost = MakeMoneyStr(dwCost);
		strMSG = CTChart::Format( dwMsgType[ m_bDurationRepType ], (LPCTSTR)strCost);
	}

	m_pMainWnd->MessageBoxYesNo(
		strMSG,
		TSTR_YES,
		TSTR_NO,
		GM_DURATIONREP_PROGRESS,
		GM_CLOSE_MSGBOX );

	return TERR_NONE;
}

int CTClientGame::OnCS_DURATIONREP_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bCount;
	
	(*pPacket)
		>> bResult
		>> bCount;

	CString strMSG;
	TUISND_TYPE eSndType;

	if( bResult == ITEMREPAIR_SUCCESS )
	{
		for(BYTE i=0; i<bCount; ++i)
		{
			BYTE bInven;
			BYTE bSlotID;
			DWORD dwDuraMax;
			DWORD dwDuraCurrent;

			(*pPacket)
				>> bInven
				>> bSlotID
				>> dwDuraMax
				>> dwDuraCurrent;

			CTClientItem* pTItem = FindMainItemByInven(bInven,bSlotID);
			if( pTItem )
			{
				pTItem->SetDuraMax(dwDuraMax);
				pTItem->SetDuraCurrent(dwDuraCurrent);
			}
		}

		strMSG = CTChart::LoadString( TSTR_DURATIONREP_SUCC);
		eSndType = TUISND_TYPE_INFO;

		CTItemRepairDlg* pDlg = static_cast<CTItemRepairDlg*>(
			m_vTFRAME[TFRAME_ITEM_REPAIR]);
		
		pDlg->ClearItem();
	}
	else
	{
		switch(bResult)
		{
		case ITEMREPAIR_NEEDMONEY	:
			return TERR_DURATIONREP_NOMONEY;
		case ITEMREPAIR_DISALLOW	:
			return TERR_DURATIONREP_DISALLOW;
		case ITEMREPAIR_NOTFOUND:
			return TERR_DURATIONREP_NOTNEED;
		default						:
			return TERR_DURATIONREP_FAIL;
		}
		
	}

	m_pDurationUI->Update();

	if( !strMSG.IsEmpty() )
		ShowInfoMsgBox(strMSG, TRUE, eSndType);

	return TERR_NONE;
}

int CTClientGame::OnCS_REFINECOST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCost;
	BYTE bDiscountRate;

	(*pPacket)
		>> dwCost
		>> bDiscountRate;

	CString strMSG;

	if( m_vMAP.m_wMapID != TUTORIAL_MAPID &&
		(!m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_LEGENDREFINEBOOK ) || !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_MAGICTRANSFERSPELL )) )
	{
		CheckRefineUseCashItem( dwCost, bDiscountRate );
	}
	else
	{
		if( bDiscountRate )
		{
			dwCost -= dwCost * bDiscountRate / 100;
			CString strCost = MakeMoneyStr(dwCost);
			strMSG = CTChart::Format( TSTR_REFINE_COST_SALE,
				bDiscountRate,
				(LPCTSTR)strCost);
		}
		else
		{
			CString strCost = MakeMoneyStr(dwCost);
			strMSG = CTChart::Format( TSTR_REFINE_COST, (LPCTSTR)strCost);
		}

		m_pMainWnd->MessageBoxYesNo(
			strMSG,
			TSTR_YES,
			TSTR_NO,
			GM_REFINE_PROGRESS,
			GM_CLOSE_MSGBOX );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_REFINE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bReturn = TERR_NONE;
	__time64_t dEndTime=0;
	
	(*pPacket)
		>> bResult;

	CString strMSG;
	TUISND_TYPE eSndType;

	CTItemRefineDlg* pDlg = static_cast<CTItemRefineDlg*>(
		m_vTFRAME[TFRAME_ITEM_REFINE]);
	
	pDlg->ClearItem();

	if( bResult == ITEMREPAIR_SUCCESS )
	{
		BYTE bInven;
		BYTE bSlotID;
		
		(*pPacket)
			>> bInven
			>> bSlotID;

		CTClientItem* pTItem = FindMainItemByInven(bInven,bSlotID);
		if( pTItem )
		{
			pTItem->SetItemSlot( bSlotID );
			pTItem->SetItemData( pPacket );
			pTItem->SetItemMagic(pPacket);

			pDlg->SetItem(
				CTItemRefineDlg::TARGET_SLOT,
				pTItem->GetTITEM(),
				bInven,
				bSlotID);
		}
		
		strMSG = CTChart::LoadString( TSTR_REFINE_SUCC);
		eSndType = TUISND_TYPE_INFO;
	}
	else
	{
		switch(bResult)
		{
		case ITEMREPAIR_NEEDMONEY	:
			return TERR_REFINE_NOMONEY;
		case ITEMREPAIR_MAXREFINE	:
			return TERR_REFINE_MAXREFINE;
		case ITEMREPAIR_LEVELDIFF	:
			return TERR_REFINE_LEVELDIFF;
		}
	}

	ResetInvenUI();

	if( bResult == ITEMREPAIR_FAIL )
		return TERR_REFINE_FAIL;
	else
	{
		m_pDurationUI->Update();

		if( !strMSG.IsEmpty() )
			ShowInfoMsgBox(strMSG, TRUE, eSndType);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_HEROSELECT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	return TERR_NONE;
}

int CTClientGame::OnCS_HEROLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bLocalCount=0;

	(*pPacket)
		>> bLocalCount;

	for( INT i=0 ; i < bLocalCount; ++i)
	{
		WORD wLocalID;
		CString strHeroName;

		(*pPacket)
			>> wLocalID
			>> strHeroName;
	}

	BYTE bCastleCount=0;

	(*pPacket)
		>> bCastleCount;

	for( i=0 ; i < bCastleCount ; ++i)
	{
		WORD wCaltleID;
		CString strHeroName;

		(*pPacket)
			>> wCaltleID
			>> strHeroName;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CHANGECHARBASE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwCharID;
	BYTE bType;
	BYTE bValue;
	CString strName;

	(*pPacket)
		>> bResult
		>> dwCharID
		>> bType
		>> bValue
		>> strName;

	if( bResult == CCB_SUCCESS )
	{
		CTClientChar* pPLAYER = NULL;

		if( m_pMainChar && m_pMainChar->m_dwID == dwCharID )
		{
			pPLAYER = m_pMainChar;
		}
		else
		{
			MAPPLAYER::iterator it = m_mapPLAYER.find( dwCharID );
			if( it != m_mapPLAYER.end() )
				pPLAYER = it->second;
			else
				return TERR_NONE;
		}

		switch( bType)
		{
		case IK_FACE:
		case IK_HAIR:
			{
				switch( bType)
				{
				case IK_FACE:
					pPLAYER->m_bFace = bValue;
					break;
				case IK_HAIR:
					pPLAYER->m_bHair = bValue;
				};

				if( m_pMainChar && m_pMainChar->m_dwID == dwCharID )
				{
					if( m_vTFACEIMG[TFACEIMG_PLAYER] )
						m_vTFACEIMG[TFACEIMG_PLAYER]->SetCurImage(GetFACE(
							m_pMainChar->GetRaceID(),
							m_pMainChar->m_bSex,
							m_pMainChar->m_bFace,
							m_pMainChar->m_bHair));

					
					m_pMainChar->ResetEQUIP( m_pDevice, m_pRES);

					ResetPlayerINFO(FALSE);
					ResetCharInfoUI();
				}
				else if( m_pTARGET && m_pTARGET->m_dwID == dwCharID)
				{
					m_vTFACEIMG[TFACEIMG_TARGETPC]->SetCurImage(GetFACE(
						pPLAYER->GetRaceID(),
						pPLAYER->m_bSex,
						pPLAYER->m_bFace,
						pPLAYER->m_bHair));

					pPLAYER->ResetEQUIP( m_pDevice, m_pRES);
				}
			}
			break;

		case IK_RACE:
		case IK_SEX:
			{
				if( pPLAYER->m_bDoubleHead )
					pPLAYER->SetPIVOT( ID_PIVOT_HEAD, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f );

				switch( bType)
				{
				case IK_RACE:
					{
						pPLAYER->m_bRaceID_ = bValue;
						CTPetManageDlg *pPetDlg = static_cast<CTPetManageDlg *>(m_vTFRAME[TFRAME_PET_MANAGE]);
						pPetDlg->ResetRace( pPLAYER->GetRaceID() );
					}
					break;
				case IK_SEX:
					pPLAYER->m_bSex = bValue;
					break;
				}

				pPLAYER->DoResetObj(
					m_pDevice,
					m_pRES);

				if( m_pMainChar && m_pMainChar->m_dwID == dwCharID )
				{
					m_pMainWnd->m_Camera.m_fCamHEIGHT = m_pMainChar->GetAttrFLOAT(ID_CAM_HEIGHT);
					m_pMainWnd->m_Camera.m_fCamDIST = m_pMainChar->GetAttrFLOAT(ID_CAM_DIST);

					if( m_vTFACEIMG[TFACEIMG_PLAYER] )
						m_vTFACEIMG[TFACEIMG_PLAYER]->SetCurImage(GetFACE(
							m_pMainChar->GetRaceID(),
							m_pMainChar->m_bSex,
							m_pMainChar->m_bFace,
							m_pMainChar->m_bHair));
				}

				if( pPLAYER->m_bDoubleHead )
					pPLAYER->SetPIVOT( ID_PIVOT_HEAD, 0.0f, 0.0f, 0.0f, 2.0f, 2.0f, 2.0f );
			}
			break;

		case IK_NAME:
			{
				pPLAYER->m_strNAME = strName;
			}
			break;
		}

		ResetPlayerINFO(FALSE);
		ResetTargetINFO(FALSE);
		ResetCharInfoUI();
	}
	else
	{
		switch(bResult)
		{
		case CCB_DUPLICATE:
			return TERR_DUPNAME;
		case CCB_NOITEM:
			return TERR_ITEM_NOT_FOUND;
		case CCB_FAIL:
			return TERR_CHANGECHARBASE_FAIL;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_STOPTHECLOCK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bInvenID;
	BYTE bItem;
	__time64_t dEndTime;

	(*pPacket)
		>> bResult
		>> bInvenID
		>> bItem
		>> dEndTime;

	if( bResult == STC_SUCCESS )
	{
		if( bItem == INVALID_SLOT ) // 가방의 기한연장이다.
		{
			CTClientInven* pTINVEN = m_pMainChar->FindTInven(bInvenID);
			pTINVEN->m_dEndTime = dEndTime;
		}
		else // 아이템의 기한연장이다.
		{
			CTClientInven* pTINVEN = m_pMainChar->FindTInven(bInvenID);

			MAPTITEM::iterator it = pTINVEN->m_mapTITEM.find(bItem);
			if( it != pTINVEN->m_mapTITEM.end() )
			{
				CTClientItem* pTITEM = it->second;
				pTITEM->SetEndTime( dEndTime);
			}
		}

		CString strMSG;
		strMSG = CTChart::LoadString( TSTR_EXTENDED_PERIOD);

		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE );
	}
	else
	{
		switch( bResult )
		{
		case STC_NEEDCASH:
			{
				if( CTNationOption::RUSSIA )
					return TERR_NOITEM;
			}
			return TERR_NEED_CASH;
		case STC_FAIL:
			return TERR_EXTEND_PERIOD_FAIL;
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_SYSTEMMSG_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwChatFlag = TCHAT_FLAG_SHOW;
	CString strMSG;
	CString strGM = CTChart::LoadString( TSTR_REPRESENTATION_GM );
	BYTE bSysMsgType = SM_NONE;

	(*pPacket)
		>> bSysMsgType;

	switch( bSysMsgType )
	{
	default:
	case SM_NONE:
		return TERR_NONE;

	case SM_BATTLE_NORMAL:
		{
			strMSG = CTChart::LoadString( TSTR_SM_BATTLE_NORMAL);
		}
		break;
	case SM_BATTLE_START:
		{
			strMSG = CTChart::LoadString( TSTR_SM_BATTLE_START);
		}
		break;
	case SM_BATTLE_START_ALARM:
		{
			DWORD dwTIME = 0;

			(*pPacket)
				>> dwTIME;

			if( (dwTIME/60) <= 30 && m_vMAP.m_wMapID != TUTORIAL_MAPID )
			{
				CTGaugePannel* pGaugePannel = static_cast<CTGaugePannel*>( m_vTFRAME[TFRAME_GAUGE] );

				if( !m_bAlreadyRecvSiegeAlarm ) //20min
				{
					pGaugePannel->AttachSiegeAlarm();
					m_bAlreadyRecvSiegeAlarm = TRUE;
				}

				if( pGaugePannel->m_bAddedSiegeAlarm )
					pGaugePannel->SetNotifySiegeAlarm(TRUE);
			}

			CString strFMT;
			if( dwTIME > 60 )
			{
				strFMT = CTChart::LoadString( TSTR_SM_BATTLE_START_ALARM );
				dwTIME /= 60;
			}
			else
				strFMT = CTChart::LoadString( TSTR_SM_BATTLE_START_ALARM_SEC );

			strMSG.Format( strFMT,
				dwTIME);
		}
		break;
	case SM_BATTLE_END_ALARM:
		{
			DWORD dwTIME = 0;

			(*pPacket)
				>> dwTIME;

			CString strFMT;
			if( dwTIME > 60 )
			{
				strFMT = CTChart::LoadString( TSTR_SM_BATTLE_END_ALARM);
				dwTIME /= 60;
			}
			else
				strFMT = CTChart::LoadString( TSTR_SM_BATTLE_END_ALARM_SEC);

			strMSG.Format( strFMT,
				dwTIME);
		}
		break;
	case SM_BATTLE_PEACE:
		{
			DWORD dwTIME = 0;

			(*pPacket)
				>> dwTIME;

			dwTIME /= 60;

			CString strBP;
			strBP = CTChart::LoadString( TSTR_SM_BATTLE_PEACE);

			strMSG.Format( strBP, dwTIME );

			if( m_vMAP.m_wMapID != TUTORIAL_MAPID )
			{
				m_bAlreadyRecvSiegeAlarm = FALSE;
				static_cast<CTGaugePannel*>( m_vTFRAME[TFRAME_GAUGE] )->DettachSiegeAlarm();
			}
		}
		break;
	case SM_BATTLE_OPENGATE:
		{
			CString strLocal;
			(*pPacket)
				>> strLocal;

			strMSG = CTChart::Format( TSTR_SM_BATTLE_OPENGATE, strLocal );
			
		}
		break;
	case SM_BATTLE_BOSSDIE:
		{
			CString strLocal, strGuild;
			WORD wCountry;

			(*pPacket)
				>> strLocal
				>> strGuild
				>> wCountry; //0:드프겔 1:크락시온

			if( strGuild.IsEmpty() )
			{
				strMSG = CTChart::Format( TSTR_SM_BATTLE_BOSSDIE_NULL,
					strLocal,
					CTChart::LoadString((TSTRING) m_vTCOUNTRYSTR[wCountry]) );
			}
			else
			{
				strMSG = CTChart::Format( TSTR_SM_BATTLE_BOSSDIE,
					strLocal,
					strGuild,
					CTChart::LoadString((TSTRING) m_vTCOUNTRYSTR[wCountry]) );
			}
		}
		break;
	case SM_DUAL_WIN:
		{
			CString strWINNER;
			CString strLOSER;

			(*pPacket)
				>> strWINNER
				>> strLOSER;
			
			CString strFMT;
			strFMT = CTChart::LoadString( TSTR_SM_DUAL_WIN);

			strMSG.Format( strFMT,
				strWINNER,
				strLOSER);

			m_pChatFrame->ChatSysMSG(
				CTChatFrame::GetChatTypeString(CHAT_SHOW),
				"",
				strMSG,
				CTChatFrame::GetChatTypeColor(3),
				TCHAT_FLAG_SHOW,
				ID_SND_INFO,
				MEDIA_TSOUND);

			return TERR_NONE;
		}
		break;
	case SM_EVENT_QUARTER:
		{
			BYTE bHOUR = 0;
			BYTE bMIN = 0;
			CString strTARGET;
			BYTE bNumber = 0;

			(*pPacket)
				>> bHOUR
				>> bMIN
				>> strTARGET
				>> bNumber;

			CString strFMT;
			strFMT = CTChart::LoadString( TSTR_SM_EVENT_QUARTER);

			strMSG.Format( strFMT,
				bHOUR,
				bMIN,
				strTARGET,
				bNumber);
		}
		break;
	case SM_CHAT_BAN:
		{
			DWORD dwTIME=0;

			(*pPacket)
				>> dwTIME;

			CString strFMT;
			if( dwTIME > 60 )
			{
				strFMT = CTChart::LoadString( TSTR_SM_CHAT_BAN);
				dwTIME /= 60;
			}
			else
				strFMT = CTChart::LoadString( TSTR_SM_CHAT_BAN_SEC);

			strMSG.Format( strFMT,
				dwTIME);
			dwChatFlag = TCHAT_FLAG_WORLD;
		}
		break;
	case SM_CASTLE_NORMAL:
		{
			strMSG = CTChart::LoadString( TSTR_SM_CASTLE_NORMAL );
		}
		break;
	case SM_CASTLE_START:
		{
			strMSG = CTChart::LoadString( TSTR_SM_CASTLE_START );
		}
		break;
	case SM_CASTLE_START_ALARM:
		{
			DWORD dwTIME = 0;
			(*pPacket)
				>> dwTIME;

			if( dwTIME > 60 )
			{
				dwTIME /= 60;
				strMSG = CTChart::Format( TSTR_SM_CASTLE_START_ALARM_MIN, dwTIME );
			}
			else
				strMSG = CTChart::Format( TSTR_SM_CASTLE_START_ALARM_SEC, dwTIME );
		}
		break;
	case SM_CASTLE_END_ALARM:
		{
			DWORD dwTIME = 0;
			(*pPacket)
				>> dwTIME;

			if( dwTIME > 60 )
			{
				dwTIME /= 60;
				strMSG = CTChart::Format( TSTR_SM_CASTLE_END_ALARM_MIN,
					dwTIME );
			}
			else
				strMSG = CTChart::Format( TSTR_SM_CASTLE_END_ALARM_SEC,
					dwTIME );
		}
		break;
	case SM_CASTLE_PEACE:
		{
			strMSG = CTChart::LoadString( TSTR_SM_CASTLE_PEACE );
		}
		break;
	case SM_CASTLE_END:
		{
			CString strLOCAL;
			CString strGUILD;

			(*pPacket)
				>> strLOCAL
				>> strGUILD;

			if( !strGUILD.IsEmpty() )
				strMSG = CTChart::Format( TSTR_GUILD_SM_CASTLE_END,
					strLOCAL,
					strGUILD );
		}
		break;

	case SM_ITEM_EXPIRE:
		{
			DWORD dwSecond;
			BYTE bInvenID;
			BYTE bSlotID;

			(*pPacket)
				>> dwSecond
				>> bInvenID
				>> bSlotID;

			dwSecond /= 60;

			CTClientItem* pTItem = FindMainItemByInven( bInvenID, bSlotID );
			if( pTItem && pTItem->GetTITEM() )
			{
				strMSG = CTChart::Format( TSTR_SM_ITEM_EXPIRE,
					pTItem->GetTITEM()->m_strNAME,
					dwSecond );
				dwChatFlag = TCHAT_FLAG_WORLD;
			}
		}
		break;
	case SM_TOURNAMENT_STEP:
		{
			CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );

			BYTE bGroup;
			BYTE bStep;
			DWORD dwTIME;
			INT64 llNextStepTime;

			(*pPacket)
#ifdef MODIFY_TT
				>> bGroup
#endif
				>> bStep
				>> dwTIME //스텝유지시간 단위:sec
				>> llNextStepTime;

			CTime tNextStepTime(llNextStepTime);
			CString strCHAPTER;
			CString strREGION;

			switch( bStep )
			{
			case TNMTSTEP_READY:
				if( pDLG->IsVisible() )
					m_pSESSION->SendCS_TOURNAMENTSCHEDULE_REQ();
				break;
			case TNMTSTEP_1st:				// 1군 지원
				{
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_1ST_MSG);
				}
				break;
			case TNMTSTEP_NORMAL:		// 일반 지원
				{
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_NORMAL_MSG);
				}
				break;
			case TNMTSTEP_PARTY:			// 분대구성
				{
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_PARTY_MSG);
				}
				break;
			case TNMTSTEP_MATCH:		// 선발완료
				{
					//strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_MATCH_MSG);
					strMSG = CTChart::Format( TSTR_TOURNAMENT_STEP_MATCH_MSG,
						tNextStepTime.GetHour(),
						tNextStepTime.GetMinute());
				}
				break;
			case TNMTSTEP_ENTER: // 입장
				{
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_ENTER_MSG);
				}
				break;
			case TNMTSTEP_QFINAL: // 8강
				if( m_bTournamentBattle || m_bTournamentWatching )
				{
					strREGION = CTChart::LoadString(TSTR_TOURNAMENT_START);
					SetTournamentRemainTime( TRUE, dwTIME*1000 );
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_QFINAL_MSG);

					if( m_vTFRAME[TFRAME_TOURNAMENT_RESULT]->IsVisible() )
						DisableUI( TFRAME_TOURNAMENT_RESULT );
				}
				break;
			case TNMTSTEP_QFEND:
				if( m_bTournamentBattle || m_bTournamentWatching )
				{
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_QFEND_MSG);
				}
				break;
			case TNMTSTEP_SFENTER: // 4강준비
				{
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_SFENTER_MSG);
					OnTournamentEnter();
				}
				break;
			case TNMTSTEP_SFINAL: // 4강
				if( m_bTournamentBattle || m_bTournamentWatching )
				{
					strREGION = CTChart::LoadString(TSTR_TOURNAMENT_START);
					SetTournamentRemainTime( TRUE, dwTIME*1000 );
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_SFINAL_MSG);

					if( m_vTFRAME[TFRAME_TOURNAMENT_RESULT]->IsVisible() )
						DisableUI( TFRAME_TOURNAMENT_RESULT );
				}
				break;
			case TNMTSTEP_SFEND:
				if( m_bTournamentBattle || m_bTournamentWatching )
				{
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_SFEND_MSG);
				}
				break;
			case TNMTSTEP_FENTER: // 결승준비
				{
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_FENTER_MSG);
					OnTournamentEnter();
				}
				break;
			case TNMTSTEP_FINAL: // 결승
				if( m_bTournamentBattle || m_bTournamentWatching )
				{
					strREGION = CTChart::LoadString(TSTR_TOURNAMENT_START);
					SetTournamentRemainTime( TRUE, dwTIME*1000 );
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_FINAL_MSG);

					if( m_vTFRAME[TFRAME_TOURNAMENT_RESULT]->IsVisible() )
						DisableUI( TFRAME_TOURNAMENT_RESULT );
				}
				break;
			case TNMTSTEP_END: // 종료
				if( m_bTournamentBattle || m_bTournamentWatching )
				{
					strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_END_MSG);
				}
				break;
			}

			CTMainUI* pMainUI = static_cast<CTMainUI*>( m_vTFRAME[ TFRAME_MAIN ] );
			if( !strCHAPTER.IsEmpty() )
				pMainUI->ResetChapterMSG( strCHAPTER );
			if( !strREGION.IsEmpty() )
				pMainUI->ResetRegionMSG( strREGION );

#ifdef MODIFY_TT
			if( bGroup == 0 )
			{
				if( CTTournament::IsCommonSchedule( bStep ) )
				{
					// 모든 그룹의 CurrentStep을 bCurrentStep으로 맞춘다.
					CTTournament::MAPSCHEDULEGROUP::iterator it, end;
					it = pDLG->m_mapGroup.begin();
					end = pDLG->m_mapGroup.end();
					for(; it != end ; ++it )
						it->second->m_bCurrentStep = bStep;
				}
				else
				{
					CTTournament::ScheduleGroup* pCommonGroup = pDLG->FindScheduleGroup( 0 );
					if( pCommonGroup )
						pCommonGroup->m_bCurrentStep = bStep;
				}

			}

			CTTournament::ScheduleGroup* pGroup = pDLG->FindScheduleGroup( bGroup );
			if( pGroup )
			{
				pDLG->m_bCurrentGroup = bGroup;
				pGroup->m_bCurrentStep = bStep;
				if( pDLG->IsVisible() )
				{
					int nSel = pDLG->m_pMATCHLIST->GetCurSelIndex();

					if( nSel != -1 )
					{
						LPTMATCH pTMATCH = (LPTMATCH) pDLG->m_pMATCHLIST->GetItemData(nSel, 0);
						if( pTMATCH->m_bGroup == bGroup )
						{
							pDLG->m_pCurrentGroup = pGroup;
							pDLG->CalcScheduleTop();
							pDLG->UpdateSchedule();
						}
					}

					m_pSESSION->SendCS_TOURNAMENTAPPLYINFO_REQ();
				}
			}
#else
			pDLG->m_bCurrentStep = bStep;
			if( pDLG->IsVisible() )
			{
				pDLG->CalcScheduleTop();
				pDLG->UpdateSchedule();
				m_pSESSION->SendCS_TOURNAMENTAPPLYINFO_REQ();
			}
#endif
		}
		break;
	case SM_TOURNAMENT_COUNTDOWN:
		{
			BYTE bStep;
			DWORD dwTIME;
			(*pPacket)
				>> bStep
				>> dwTIME; //스텝유지시간 단위:sec

			switch(bStep)
			{
			case TNMTSTEP_READY:
			case TNMTSTEP_1st:
			case TNMTSTEP_NORMAL:
			case TNMTSTEP_PARTY:
			case TNMTSTEP_MATCH:
				SetTournamentRemainTime( FALSE, 0 );
				break;

			case TNMTSTEP_ENTER:
			case TNMTSTEP_SFENTER:
			case TNMTSTEP_FENTER:
				{
					SetTournamentRemainTime( FALSE, 0 );

					switch( bStep )
					{
					case TNMTSTEP_ENTER:
						strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_QFINAL_TIME_MSG);
						break;
					case TNMTSTEP_SFENTER:
						strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_SFINAL_TIME_MSG);
						break;
					case TNMTSTEP_FENTER:
						strMSG = CTChart::LoadString(TSTR_TOURNAMENT_STEP_FINAL_TIME_MSG);
						break;
					};

					DWORD dwMIN = dwTIME / 60;
					DWORD dwSEC = dwTIME % 60;

					if( dwMIN != 0 )
					{
						CString strFMT = CTChart::Format(TSTR_TOURNAMENT_STEP_MIN,
							dwMIN);
						strMSG += strFMT;
					}

					if( dwSEC != 0 )
					{
						CString strFMT = CTChart::Format(TSTR_TOURNAMENT_STEP_SEC,
							dwSEC);
						strMSG += strFMT;
					}

					if( dwMIN != 0 ||
						dwSEC != 0 )
					{
						strMSG += CTChart::LoadString(TSTR_TOURNAMENT_STEP_BEFORE);
					}
					else
						strMSG.Empty();
				}
				break;

			case TNMTSTEP_QFINAL:
			case TNMTSTEP_SFINAL:
			case TNMTSTEP_FINAL:
				{
					if( m_bTournamentWatching || m_bTournamentBattle )
						SetTournamentRemainTime( TRUE, dwTIME*1000 );
				}
				break;
			}
		}
		break;
#ifdef EVENT_LOTTERT
	case SM_EVENT_LOTTERY:
		{
			CString strTitle;
			WORD wItemCount;

			(*pPacket)
				>> strTitle
				>> wItemCount;

			strMSG = CTChart::Format( TSTR_ITEM_LOTTERY_BEGIN,
				(LPCTSTR) strTitle );

			m_pChatFrame->ChatSysMSG(
				CTChatFrame::GetChatTypeString(3),
				strGM,
				strMSG,
				CTChatFrame::GetChatTypeColor(3),
				TCHAT_FLAG_WORLD,
				ID_SND_INFO,
				MEDIA_TSOUND);

			for( WORD i=0 ; i < wItemCount ; i++ )
			{
				WORD wItemID;
				BYTE bItemCount;
				WORD wUserCount;

				(*pPacket)
					>> wItemID
					>> bItemCount
					>> wUserCount;

				INT nItemCount = bItemCount;

				CString strITEM;
				strITEM.Format( "%%s (%d개) - ", nItemCount );
				for(WORD j=0 ; j < wUserCount ; j++ )
				{
					if( j > 0 )
						strITEM += ", ";

					CString strName;
					(*pPacket)
						>> strName;

					CString strFMT;
					strFMT = CTChart::Format( TSTR_ITEM_LOTTERY_NIM,
						(LPCTSTR) strName );

					strITEM += strFMT;
				}
				strITEM += CTChart::LoadString( TSTR_ITEM_LOTTERY_GET );

				CTTextLinker* pTextLinker = CTTextLinker::GetInstance();
				CTTextLinkData* pData;
				DWORD dwID = pTextLinker->NewTextLinkData(&pData);
				pData->PackItem( CTChart::FindTITEMTEMP(wItemID) );

				VECTORDWORD vItemID;
				vItemID.push_back(dwID);
				strMSG = pTextLinker->MakeItemNetText( strITEM, vItemID);
				vItemID.clear();

				m_pChatFrame->ChatMSG(
						CTChatFrame::GetChatTypeString(3),
						strGM,
						strMSG,
						CTChatFrame::GetChatTypeColor(3),
						TCHAT_FLAG_WORLD,
						ID_SND_INFO,
						MEDIA_TSOUND,
						-1);
			}
		}
		return TERR_NONE;
#endif

#ifdef ADD_MISSION
	case SM_MISSION_NORMAL:		//%d (짝수: 홀수) 미션지역의 전쟁이 종료되었습니다.
		{
			WORD wValue;
			(*pPacket)
				>> wValue;

			if( wValue % 2 )
				strMSG = CTChart::LoadString( TSTR_MISSION_WAR_CLOSE_ODDNUMBER );
			else
				strMSG = CTChart::LoadString( TSTR_MISSION_WAR_CLOSE_EVENNUMBER );
		}
		break;

	case SM_MISSION_START:		//%d (짝수: 홀수) 미션지역의 전쟁이 시작되었습니다.
		{
			WORD wValue;
			(*pPacket)
				>> wValue;

			if( wValue % 2 )
				strMSG = CTChart::LoadString( TSTR_MISSION_WAR_START_ODDNUMBER );
			else
				strMSG = CTChart::LoadString( TSTR_MISSION_WAR_START_EVENNUMBER );
		}
		break;

	case SM_MISSION_BOSSDIE:		//%s 미션지역을 %d 국가가 점령했습니다.
		{
			CString strMissionName;
			WORD wCountry;
			DWORD dwMapID;

			(*pPacket)
				>> strMissionName
				>> wCountry
				>> dwMapID;

			strMSG = CTChart::Format( TSTR_MISSION_OCCUPATION,
				(LPCTSTR) strMissionName,
				CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ wCountry ] ) );
			
			if( dwMapID == m_vMAP.m_wMapID &&
				wCountry == m_pMainChar->m_bContryID )
				ShotMissionWarFirecracker();
		}
		break;

	case SM_MISSION_START_ALARM: //%d (짝.홀) 미션전쟁이 %d(분/초)후 시작됩니다.
		{
			WORD wValue;
			DWORD dwTIME;

			(*pPacket)
				>> wValue
				>> dwTIME;

			if( wValue % 2 )
			{
				if( dwTIME > 60 )
				{
					dwTIME /= 60;
					strMSG = CTChart::Format( TSTR_MISSION_WAR_START_ODDNUMBER_MIN,
						dwTIME);
				}
				else
					strMSG = CTChart::Format( TSTR_MISSION_WAR_START_ODDNUMBER_SEC,
						dwTIME);
			}
			else
			{
				if( dwTIME > 60 )
				{
					dwTIME /= 60;
					strMSG = CTChart::Format( TSTR_MISSION_WAR_START_EVENNUMBER_MIN,
						dwTIME);
				}
				else
					strMSG = CTChart::Format( TSTR_MISSION_WAR_START_EVENNUMBER_SEC,
						dwTIME);
			}
		}
		break;

	case SM_MISSION_END_ALARM:	//%d (짝.홀) 미션전쟁이 %d(분/초)후 종료됩니다.
		{
			WORD wValue;
			DWORD dwTIME;

			(*pPacket)
				>> wValue
				>> dwTIME;

			if( wValue % 2 )
			{
				if( dwTIME > 60 )
				{
					dwTIME /= 60;
					strMSG = CTChart::Format( TSTR_MISSION_WAR_CLOSE_ODDNUMBER_MIN,
						dwTIME);
				}
				else
					strMSG = CTChart::Format( TSTR_MISSION_WAR_CLOSE_ODDNUMBER_SEC,
						dwTIME);
			}
			else
			{
				if( dwTIME > 60 )
				{
					dwTIME /= 60;
					strMSG = CTChart::Format( TSTR_MISSION_WAR_CLOSE_EVENNUMBER_MIN,
						dwTIME);
				}
				else
					strMSG = CTChart::Format( TSTR_MISSION_WAR_CLOSE_EVENNUMBER_SEC,
						dwTIME);
			}
		}
		break;

	case SM_MISSION_PEACE:		//%d (짝수: 홀수) 미션지역의 전쟁이 종료되었습니다.
		{
			WORD wValue;
			DWORD dwSecond;
			
			(*pPacket)
				>> wValue
				>> dwSecond;

			if( wValue % 2 )
				strMSG = CTChart::LoadString( TSTR_MISSION_WAR_CLOSE_ODDNUMBER );
			else
				strMSG = CTChart::LoadString( TSTR_MISSION_WAR_CLOSE_EVENNUMBER );
		}
		break;

	case SM_MISSION_TIMEOUT:		//%s 미션지역을 점령한 국가가 없어 몬스터 지역으로 전환되었습니다. 
		{
			CString strMissionName;
			WORD wCountry;
			(*pPacket)
				>> strMissionName
				>> wCountry;

			strMSG = CTChart::Format( TSTR_MISSION_TIMEOUT,
				(LPCTSTR) strMissionName );
		}
		break;
#endif
	}

	m_pChatFrame->ChatSysMSG(
		CTChatFrame::GetChatTypeString(CHAT_SHOW),
		strGM,
		strMSG,
		CTChatFrame::GetChatTypeColor(CHAT_SHOW),
		dwChatFlag,
		ID_SND_INFO,
		MEDIA_TSOUND);

	return TERR_NONE;
}

int CTClientGame::OnCS_OPENMONEY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwMONEY = 0;

	(*pPacket)
		>> dwMONEY;

	CString strCost = MakeMoneyStr(dwMONEY);
	CString strMSG;
	strMSG = CTChart::Format( TSTR_OPENMONEY,
		(LPCTSTR)strCost);

	m_pMainWnd->MessageBoxOK(
		strMSG,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		0,
		FALSE,
		ID_SND_INFO,
		MEDIA_TSOUND,
		TMSGBOXOPEN_CLOSEALL_ALREADY);

	return TERR_NONE;
}

int CTClientGame::OnCS_ADDSQUAD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwChiefID;
	WORD wSquadID;
	BYTE bCount;

	(*pPacket)
		>> dwChiefID
		>> wSquadID
		>> bCount;

	if(m_pTRSCS->FindTSQUAD(wSquadID))
		return TERR_NONE;

	LPTSQUAD pTSQUAD = new TSQUAD(
		m_pDevice,
		m_pRES,
		CTRSCSDlg::m_pTARROW);

	pTSQUAD->m_dwChiefID = dwChiefID;
	pTSQUAD->m_wPartyID = wSquadID;

	for( BYTE i=0; i<bCount; i++)
	{
		LPTUNIT pTUNIT = new TUNIT();

		(*pPacket)
			>> pTUNIT->m_dwCharID
			>> pTUNIT->m_strNAME
			>> pTUNIT->m_fSPEED
			>> pTUNIT->m_dwTargetID
			>> pTUNIT->m_dwMaxHP
			>> pTUNIT->m_dwHP
			>> pTUNIT->m_wTargetX
			>> pTUNIT->m_wTargetZ
			>> pTUNIT->m_wMapID
			>> pTUNIT->m_wPosX
			>> pTUNIT->m_wPosZ
			>> pTUNIT->m_wDIR
			>> pTUNIT->m_bTargetType
			>> pTUNIT->m_bLevel
			>> pTUNIT->m_bClass
			>> pTUNIT->m_bRaceID
			>> pTUNIT->m_bSexID
			>> pTUNIT->m_bFaceID
			>> pTUNIT->m_bHairID
			>> pTUNIT->m_bTCMD;

		pTSQUAD->m_vTUNIT.push_back(pTUNIT);
		pTUNIT->m_dwTick = 0;
	}

	m_pTRSCS->m_vTCORPS.m_vTSQUAD.push_back(pTSQUAD);
	ResetRSCS(m_bRSCS);

	if(m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID )
		RelaySession()->SendCS_CORPSENEMYLIST_REQ( wSquadID, dwChiefID, &m_pTRSCS->m_vTCORPS);

	return TERR_NONE;
}

int CTClientGame::OnCS_DELSQUAD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	VTSQUAD::iterator itTSQUAD = m_pTRSCS->m_vTCORPS.m_vTSQUAD.begin();
	WORD wSquadID;

	(*pPacket)
		>> wSquadID;

	while(itTSQUAD != m_pTRSCS->m_vTCORPS.m_vTSQUAD.end())
		if( (*itTSQUAD)->m_wPartyID == wSquadID )
		{
			if( m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID )
				for( BYTE i=0; i<BYTE((*itTSQUAD)->m_vTUNIT.size()); i++)
					m_pTRSCS->DeleteReporter((*itTSQUAD)->m_vTUNIT[i]->m_dwCharID);

			if( m_pTRSCS->m_pTCURSQUAD == (*itTSQUAD) )
			{
				m_pTRSCS->m_pTCURSQUAD = NULL;
				m_pTRSCS->m_pTCURUNIT = NULL;
			}

			delete (*itTSQUAD);
			m_pTRSCS->m_vTCORPS.m_vTSQUAD.erase(itTSQUAD);
			ResetRSCS(m_bRSCS);

			return TERR_NONE;
		}
		else
			itTSQUAD++;

	return TERR_NONE;
}

int CTClientGame::OnCS_ADDCORPSUNIT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	WORD wSquadID;

	(*pPacket)
		>> wSquadID
		>> dwCharID;

	LPTSQUAD pTSQUAD = m_pTRSCS->FindTSQUAD(wSquadID);
	if(m_pTRSCS->FindTUNIT( pTSQUAD, dwCharID))
		return TERR_NONE;

	if(!pTSQUAD)
	{
		if( wSquadID != GetPartyID(m_pMainChar) )
			return TERR_NONE;

		pTSQUAD = new TSQUAD(
			m_pDevice,
			m_pRES,
			CTRSCSDlg::m_pTARROW);

		pTSQUAD->m_dwChiefID = m_pMainChar->m_dwChiefID;
		pTSQUAD->m_wPartyID = wSquadID;

		LPTUNIT pTUNIT = new TUNIT();
		pTUNIT->m_strNAME = m_pMainChar->GetName();
		pTUNIT->m_dwCharID = m_pMainChar->m_dwID;

		pTUNIT->m_dwMaxHP = m_pMainChar->m_dwMaxHP;
		pTUNIT->m_dwHP = m_pMainChar->m_dwHP;

		pTUNIT->m_bClass = m_pMainChar->m_bClassID;
		pTUNIT->m_bLevel = m_pMainChar->m_bLevel;
		pTUNIT->m_bRaceID = m_pMainChar->GetRaceID();
		pTUNIT->m_bSexID = m_pMainChar->m_bSex;
		pTUNIT->m_bFaceID = m_pMainChar->m_bFace;
		pTUNIT->m_bHairID = m_pMainChar->m_bHair;

		pTUNIT->m_wMapID = m_vMAP.m_wMapID;
		pTUNIT->m_wPosX = WORD(m_pMainChar->GetPositionX());
		pTUNIT->m_wPosZ = WORD(m_pMainChar->GetPositionZ());
		pTUNIT->m_wDIR = m_pMainChar->m_wMoveDIR;
		pTUNIT->m_fSPEED = m_pMainChar->m_fMoveSpeed;
		pTUNIT->m_dwTick = 0;

		pTUNIT->m_bTargetType = OT_NONE;
		pTUNIT->m_dwTargetID = 0;
		pTUNIT->m_wTargetX = 0;
		pTUNIT->m_wTargetZ = 0;
		pTUNIT->m_bTCMD = TRSCSCMD_NONE;

		m_pTRSCS->m_vTCORPS.m_vTSQUAD.push_back(pTSQUAD);
		pTSQUAD->m_vTUNIT.push_back(pTUNIT);

		if(!m_pTRSCS->m_pTCURSQUAD)
			m_pTRSCS->m_pTCURSQUAD = pTSQUAD;

		if( m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID )
			ReportENEMY();

		ResetRSCS(m_bRSCS);
	}

	if( dwCharID == m_pMainChar->m_dwID )
		return TERR_NONE;

	LPTUNIT pTUNIT = new TUNIT();
	pTUNIT->m_dwCharID = dwCharID;

	(*pPacket)
		>> pTUNIT->m_strNAME
		>> pTUNIT->m_fSPEED
		>> pTUNIT->m_dwTargetID
		>> pTUNIT->m_dwMaxHP
		>> pTUNIT->m_dwHP
		>> pTUNIT->m_wTargetX
		>> pTUNIT->m_wTargetZ
		>> pTUNIT->m_wMapID
		>> pTUNIT->m_wPosX
		>> pTUNIT->m_wPosZ
		>> pTUNIT->m_wDIR
		>> pTUNIT->m_bTargetType
		>> pTUNIT->m_bLevel
		>> pTUNIT->m_bClass
		>> pTUNIT->m_bRaceID
		>> pTUNIT->m_bSexID
		>> pTUNIT->m_bFaceID
		>> pTUNIT->m_bHairID
		>> pTUNIT->m_bTCMD;
	pTUNIT->m_dwTick = 0;

	if( dwCharID == m_pMainChar->m_dwChiefID )
		ReportENEMY();

	pTSQUAD->m_vTUNIT.push_back(pTUNIT);

	ResetRSCS(m_bRSCS);

	return TERR_NONE;
}

int CTClientGame::OnCS_DELCORPSUNIT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	VTSQUAD::iterator itTSQUAD = m_pTRSCS->m_vTCORPS.m_vTSQUAD.begin();

	DWORD dwCharID;
	WORD wSquadID;

	(*pPacket)
		>> wSquadID
		>> dwCharID;

	while(itTSQUAD != m_pTRSCS->m_vTCORPS.m_vTSQUAD.end())
		if( (*itTSQUAD)->m_wPartyID == wSquadID )
		{
			VTUNIT::iterator itTUNIT = (*itTSQUAD)->m_vTUNIT.begin();

			while(itTUNIT != (*itTSQUAD)->m_vTUNIT.end())
				if( (*itTUNIT)->m_dwCharID == dwCharID )
				{
					WORD wPartyID = GetPartyID(m_pMainChar);

					if( wPartyID == wSquadID || m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID )
						m_pTRSCS->DeleteReporter(dwCharID);

					if( dwCharID == m_pMainChar->m_dwID )
					{
						m_pTRSCS->m_pTCURSQUAD = NULL;
						m_pTRSCS->m_pTCURUNIT = NULL;

						m_bRSCS = FALSE;
					}

					if( m_pTRSCS->m_pTCURUNIT == (*itTUNIT) )
						m_pTRSCS->m_pTCURUNIT = NULL;

					delete (*itTUNIT);
					(*itTSQUAD)->m_vTUNIT.erase(itTUNIT);

					if((*itTSQUAD)->m_vTUNIT.empty())
					{
						if( m_pTRSCS->m_pTCURSQUAD == (*itTSQUAD) )
						{
							m_pTRSCS->m_pTCURSQUAD = NULL;
							m_pTRSCS->m_pTCURUNIT = NULL;
						}

						delete (*itTSQUAD);
						m_pTRSCS->m_vTCORPS.m_vTSQUAD.erase(itTSQUAD);
					}
					ResetRSCS(m_bRSCS);

					return TERR_NONE;
				}
				else
					itTUNIT++;

			return TERR_NONE;
		}
		else
			itTSQUAD++;

	return TERR_NONE;
}

int CTClientGame::OnCS_CORPSJOIN_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	(*pPacket)
		>> m_pTRSCS->m_vTCORPS.m_wID
		>> m_pMainChar->m_wCommanderID;
	ResetRSCS(m_bRSCS);

	return TERR_NONE;
}

int CTClientGame::OnCS_CHGCORPSCOMMANDER_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	if(bResult)
	{
		LPTSQUAD pTSQUAD = m_pTRSCS->FindTSQUAD(GetPartyID(m_pMainChar));

		if(pTSQUAD)
			m_pMainChar->m_dwChiefID = pTSQUAD->m_dwChiefID;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_CHGSQUADCHIEF_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwChiefID;
	WORD wSquadID;

	(*pPacket)
		>> wSquadID
		>> dwChiefID;

	LPTSQUAD pTSQUAD = m_pTRSCS->FindTSQUAD(wSquadID);
	if(!pTSQUAD)
		return TERR_NONE;

	if( wSquadID == GetPartyID(m_pMainChar) )
	{
		if( m_pMainChar->m_dwID == pTSQUAD->m_dwChiefID && m_pMainChar->m_dwID != dwChiefID )
			for( BYTE i=0; i<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
				if( m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_wPartyID != wSquadID )
					for( BYTE j=0; j<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT.size()); j++)
						m_pTRSCS->DeleteReporter(m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_dwCharID);

		m_pMainChar->m_dwChiefID = dwChiefID;
	}

	pTSQUAD->m_dwChiefID = dwChiefID;
	ResetRSCS(m_bRSCS);

	return TERR_NONE;
}

int CTClientGame::OnCS_COMMENT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	CString strComment;

	(*pPacket)
		>> dwCharID
		>> strComment;

	CTClientChar* pCHAR = FindPlayer(dwCharID);
	if( pCHAR )
		pCHAR->m_strUserTitle = strComment;

	return TERR_NONE;
}

int CTClientGame::OnCS_PVPPOINT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwTotalPoint;
	DWORD dwUseablePoint;
	DWORD dwMonthPvPoint;
	BYTE bEvent;

	(*pPacket)
		>> dwTotalPoint
		>> dwUseablePoint
		>> bEvent
		>> dwMonthPvPoint;

	if( m_pMainChar->m_dwPvPTotalPoint != dwTotalPoint ||
		m_pMainChar->m_dwPvPUseablePoint != dwUseablePoint )
	{
		CString strMSG;

		switch( bEvent )
		{
		case PVPE_KILL_H:
			if( m_pMainChar->m_dwPvPUseablePoint < dwUseablePoint )
			{
				strMSG = CTChart::Format( TSTR_PVPE_KILL_H_WIN,
					dwUseablePoint-m_pMainChar->m_dwPvPUseablePoint );
			}
			else if( m_pMainChar->m_dwPvPTotalPoint > dwTotalPoint )
			{
				strMSG = CTChart::Format( TSTR_PVPE_KILL_H_LOSE,
					m_pMainChar->m_dwPvPTotalPoint-dwTotalPoint );
			}
			break;

		case PVPE_KILL_E:
			if( m_pMainChar->m_dwPvPUseablePoint < dwUseablePoint )
			{
				strMSG = CTChart::Format( TSTR_PVPE_KILL_E_WIN,
					dwUseablePoint-m_pMainChar->m_dwPvPUseablePoint );
			}
			else if( m_pMainChar->m_dwPvPTotalPoint > dwTotalPoint )
			{
				strMSG = CTChart::Format( TSTR_PVPE_KILL_E_LOSE,
					m_pMainChar->m_dwPvPTotalPoint - dwTotalPoint );
			}
			break;

		case PVPE_KILL_L:
			if( m_pMainChar->m_dwPvPUseablePoint < dwUseablePoint )
			{
				strMSG = CTChart::Format( TSTR_PVPE_KILL_L_WIN,
					dwUseablePoint - m_pMainChar->m_dwPvPUseablePoint );
			}
			else if( m_pMainChar->m_dwPvPTotalPoint > dwTotalPoint )
			{
				strMSG = CTChart::Format( TSTR_PVPE_KILL_L_LOSE,
					m_pMainChar->m_dwPvPTotalPoint - dwTotalPoint );
			}
			break;

		case PVPE_GODMONKILL:
			if( m_pMainChar->m_dwPvPUseablePoint < dwUseablePoint )
			{
				strMSG = CTChart::Format( TSTR_PVPE_GODMONKILL,
					dwUseablePoint - m_pMainChar->m_dwPvPUseablePoint);
			}
			break;

		case PVPE_ENTRY:
			if( m_pMainChar->m_dwPvPUseablePoint < dwUseablePoint )
			{
				strMSG = CTChart::Format( TSTR_PVPE_ENTRY,				
					dwUseablePoint - m_pMainChar->m_dwPvPUseablePoint);
			}
			break;

		default:
		case PVPE_GUILD:
		case PVPE_WIN:
		case PVPE_DEFEND:
			strMSG.Empty();
			break;
		}

		m_pChatFrame->ChatSysMSG(
			CTChart::LoadString( TSTR_INFO_TITLE),
			m_pMainChar->GetName(),
			strMSG,
			TCOLOR_INFO,
			TCHAT_FLAG_INFO);
	}

	m_pMainChar->m_dwPvPMonthPoint = dwMonthPvPoint;
	m_pMainChar->m_dwPvPTotalPoint = dwTotalPoint;
	m_pMainChar->m_dwPvPUseablePoint = dwUseablePoint;

	static_cast<CTCharInfoDlg*>( m_vTFRAME[ TFRAME_CHAR_INFO ] )->SetPvPPoint(
		m_pMainChar->m_dwPvPTotalPoint, m_pMainChar->m_dwPvPUseablePoint );

	CTGaugePannel* pTGaugePannel = (CTGaugePannel*) m_vTFRAME[TFRAME_GAUGE];
	if( pTGaugePannel->m_pLUCKY3 )
		pTGaugePannel->m_pLUCKY3->m_strText = CTChart::Format( TSTR_HONOR, m_pMainChar->m_dwPvPMonthPoint, m_pMainChar->m_dwPvPTotalPoint );

	if( pTGaugePannel->m_pLUCKY4 )
		pTGaugePannel->m_pLUCKY4->m_strText = CTChart::Format( TSTR_MERIT, m_pMainChar->m_dwPvPUseablePoint );

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDPOINTLOG_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTGuildPointLogFrame* pDLG = static_cast<CTGuildPointLogFrame*>( m_vTFRAME[ TFRAME_GUILDPOINTLOG] );
	pDLG->m_pList->RemoveAll();

	WORD wCount;
	(*pPacket)
		>> wCount;

	for( WORD i=0 ; i < wCount ; ++i )
	{
		INT64 dlDate;
		CString strName;
		DWORD dwPoint;

		(*pPacket)
			>> dlDate
			>> strName
			>> dwPoint;

		CTime time( dlDate );
		CString strFMT;
		strFMT.Format("%d-%d-%d",
			time.GetYear(),
			time.GetMonth(),
			time.GetDay() );

		INT nLine = pDLG->m_pList->AddString( strFMT );
		pDLG->m_pList->SetItemString( nLine, 1, strName );
		strFMT.Format( "%d",
			dwPoint);
		pDLG->m_pList->SetItemString( nLine, 2, strFMT );
	}

	EnableUI( TFRAME_GUILDPOINTLOG );

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDPOINTREWARD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwRemainGuildPoint;

	(*pPacket)
		>> bResult
		>> dwRemainGuildPoint;

	CTGuildPointRewardFrame* pDLG = (CTGuildPointRewardFrame*) m_vTFRAME[ TFRAME_GUILDPOINTREWARD ];
	pDLG->m_pGuildPoint->m_strText.Format( "%d", dwRemainGuildPoint );

	CTGuildCommander* pTGUILDCMDER = CTGuildCommander::GetInstance();
	pTGUILDCMDER->GetGuildDetInfo().m_dwPvPUseablePoint = dwRemainGuildPoint;

	switch( bResult )
	{
	case GPR_SUCCESS:
		m_pMainWnd->MessageBoxYesNo(
			TSTR_POINT_REWARD_CONTINUE,
			TSTR_YES,
			TSTR_NO,
			GM_GUILD_REWARD_CONTINUE,
			GM_GUILD_REWARD_STOP );
		break;

	case GPR_NEEDPOINT:
		return TERR_GUILD_REWARD_NEEDPOINT;

	case GPR_NOMEMBER:
		return TERR_GUILD_REWARD_NOMEMBER;
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDPVPRECORD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	WORD wCount;
	(*pPacket)
		>> wCount;

	CTGuildCommander* pTGUILDCMDER = CTGuildCommander::GetInstance();
	pTGUILDCMDER->m_GuildLatestPvPVec.clear();
	pTGUILDCMDER->m_GuildWeekPvPVec.clear();

	for( WORD i=0 ; i < wCount ; ++i )
	{
		DWORD dwCharID;

		CTGuildCommander::GuildLatestPvP day;
		CTGuildCommander::GuildWeekPvP week;

		(*pPacket)
			>> dwCharID
			>> week.m_wKillCount_W
			>> week.m_wDieCount_W
			>> week.m_dwKillPoint_H_W
			>> week.m_dwKillPoint_E_W
			>> week.m_dwKillPoint_L_W
			>> week.m_dwGodMonKillPoint_W
			>> week.m_dwEntryPoint_W
			>> day.m_wKillCount_D
			>> day.m_wDieCount_D
			>> day.m_dwKillPoint_H_D
			>> day.m_dwKillPoint_E_D
			>> day.m_dwKillPoint_L_D
			>> day.m_dwGodMonKillPoint_D
			>> day.m_dwEntryPoint_D;

		week.m_dwCharID = dwCharID;
		day.m_dwCharID = dwCharID;

		week.m_dwResult = week.m_dwKillPoint_H_W +
			week.m_dwKillPoint_E_W +
			week.m_dwKillPoint_L_W +
			week.m_dwGodMonKillPoint_W +
			week.m_dwEntryPoint_W;

		day.m_dwResult = day.m_dwKillPoint_H_D + 
			day.m_dwKillPoint_E_D + 
			day.m_dwKillPoint_L_D +
			day.m_dwGodMonKillPoint_D +
			day.m_dwEntryPoint_D;

		pTGUILDCMDER->m_GuildWeekPvPVec.push_back( week );
		pTGUILDCMDER->m_GuildLatestPvPVec.push_back( day );
	}

	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildMemberFrame* pFrame = static_cast<CTGuildMemberFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDMEMBER) );
	if( pFrame->IsVisible() &&
		(pFrame->m_nCurPage == TPVPM_LATEST_PVPR || pFrame->m_nCurPage == TPVPM_WEEK_PVPR) )
	{
		pFrame->ResetInfo();
	}
	return TERR_NONE;
}

int CTClientGame::OnCS_PVPRECORD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTCharPvPInfoDlg* pDLG = static_cast<CTCharPvPInfoDlg*>( m_vTFRAME[ TFRAME_CHAR_PVP_INFO ] );

	BYTE bType;
	DWORD dwRankOrder;
	BYTE bRankPercent;
	DWORD dwMonthRankOrder = 0;			// 순위
	BYTE bMonthRankPercent = 0;			// 퍼센트
	WORD wMonthWin = 0;					// 승률
	WORD wMonthLose = 0;

	(*pPacket)
		>> bType
		>> dwRankOrder
		>> bRankPercent;

	DWORD dwKillCount[TCLASS_COUNT];
	DWORD dwDieCount[TCLASS_COUNT];

	for( INT i=0 ; i < TCLASS_COUNT ; ++i )
	{
		(*pPacket)
			>> dwKillCount[ i ]
			>> dwDieCount[ i ];
	}

	BYTE bCount;
	(*pPacket)
		>> bCount;

	VTLATESTPVPINFO vLatestPvPInfo;
	bCount = bCount > 10 ? 10 : bCount;

	for( INT i=0 ; i < bCount ; ++i )
	{
		LPTLATESTPVPINFO pLATESTPVPINFO = new TLATESTPVPINFO;
		INT64 dlDate;

		(*pPacket)
			>> pLATESTPVPINFO->m_strName
			>> pLATESTPVPINFO->m_bWin
			>> pLATESTPVPINFO->m_bClass
			>> pLATESTPVPINFO->m_bLevel
			>> pLATESTPVPINFO->m_dwPoint
			>> dlDate;

		pLATESTPVPINFO->m_dlDate = dlDate;
		vLatestPvPInfo.push_back( pLATESTPVPINFO );
	}

	if ( !bType )
	{
		(*pPacket)
			>> dwMonthRankOrder			// 순위
			>> bMonthRankPercent			// 퍼센트
			>> wMonthWin					// 승률
			>> wMonthLose;
	}
	pDLG->ResetData(
		bType ? TINFO_PVP : TINFO_WAR, // 0:전쟁 1:대결 
		dwRankOrder,
		bRankPercent,
		dwKillCount,
		dwDieCount,
		vLatestPvPInfo,
		dwMonthRankOrder,
		bMonthRankPercent,
		wMonthWin,
		wMonthLose);

	CTCharInfoDlg::m_bTabIndex = bType ? TINFO_PVP : TINFO_WAR;
	EnableUI( TFRAME_CHAR_PVP_INFO );

	return TERR_NONE;
}

int CTClientGame::UpdateAuctionList( CPacket* pPacket )
{
	BYTE bResult		= 0;

	(*pPacket)
		>> bResult;

	CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
	CTAuctionCtrlList* pList = pFrame->GetList();
	pList->ClearItems();

	CTAuctionCommander* pComm = CTAuctionCommander::GetInstance();
	if( !pComm->RecvFindList_Result( bResult ) )
	{
		pFrame->SetCurPage( 1 );
		pFrame->SetLastPage( 1 );
		pFrame->UpdateByListComp();

		return TERR_NONE;
	}

	WORD wTotalCount	= 0;
	WORD wPageNum		= 0;
	WORD wCount			= 0;

	(*pPacket)
		>> wTotalCount
		>> wPageNum
		>> wCount;

	DWORD dwLastPage = wTotalCount / TAUCTION_SLOT_COUNT;

	while( wTotalCount >= TAUCTION_SLOT_COUNT )
		wTotalCount -= TAUCTION_SLOT_COUNT;

	if( wTotalCount > 0 )
		dwLastPage++;

	dwLastPage = max( dwLastPage, 1 );

	pFrame->SetLastPage( dwLastPage );
	pFrame->UpdateByListComp();

	size_t i = 0;
	for( ; i < wCount; ++i )
	{
		CTAuctionItem* pItem = new CTAuctionItem;

		INT64	ldwDirectPrice	= 0;
		INT64	ldwStartPrice	= 0;
		INT64	ldwBidPrice		= 0;

		(*pPacket)
			>> pItem->m_dwAuctionID
			>> pItem->m_strSeller
			>> pItem->m_nEndTime
			>> pItem->m_dwRemainTick
			>> ldwDirectPrice
			>> ldwStartPrice
			>> ldwBidPrice
			>> pItem->m_bBidCount
			>> pItem->m_dwCode;

		SplitMoney(
			ldwDirectPrice,
			&pItem->m_dwBuyMoney[ TMONEY_RUNE ],
			&pItem->m_dwBuyMoney[ TMONEY_LUNA ],
			&pItem->m_dwBuyMoney[ TMONEY_CRON ] );

		SplitMoney(
			ldwBidPrice,
			&pItem->m_dwBidMoney[ TMONEY_RUNE ],
			&pItem->m_dwBidMoney[ TMONEY_LUNA ],
			&pItem->m_dwBidMoney[ TMONEY_CRON ] );

		pItem->m_pItem->SetItemSlot( pPacket );
		pItem->m_pItem->SetItemData( pPacket );
		pItem->m_pItem->SetItemMagic( pPacket );

		pList->AddItem( pItem );
	}

	pComm->RecvFindList();

	return TERR_NONE;
}

int CTClientGame::OnCS_AUCTIONFIND_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	UpdateAuctionList( pPacket );

	return TERR_NONE;
}

int CTClientGame::OnCS_AUCTIONREGLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	UpdateAuctionList( pPacket );

	return TERR_NONE;
}

int CTClientGame::OnCS_AUCTIONBIDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	UpdateAuctionList( pPacket );

	return TERR_NONE;
}

int CTClientGame::OnCS_AUCTIONINTERESTLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	UpdateAuctionList( pPacket );

	return TERR_NONE;
}

int CTClientGame::OnCS_AUCTIONBID_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult = 0;

	(*pPacket)
		>> bResult;

	CTAuctionCommander::GetInstance()
		->RecvBid_Result( bResult );

	return TERR_NONE;
}

int CTClientGame::OnCS_AUCTIONBUYDIRECT_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	CTAuctionCommander::GetInstance()
		->RecvBuy_Result( bResult );

	return TERR_NONE;
}

int CTClientGame::OnCS_AUCTIONINTEREST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult;
	BYTE bType;

	(*pPacket)
		>> bResult
		>> bType;

	if( bResult )
	{
		if( bType )	// 관심물품 등록.
		{
			CString strMSG = CTChart::Format( TSTR_AUCTIONBASKET_ADD );

			m_pMainWnd->MessageBoxOK(
				strMSG,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0 );
		}
		else	// 관심물품 취소.
		{
			CTAuctionMainFrame::GetInstance()->RequestUpdatePage();
		}
	}
	else
	{
		CString strMSG = CTChart::Format( TSTR_AUCTIONBASKET_ADDFAIL );

		m_pMainWnd->MessageBoxOK(
				strMSG,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0 );
	}

	return TERR_NONE;
}


int CTClientGame::OnCS_AUCTIONREG_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult = 0;
	DWORD dwAuctionID = 0;

	(*pPacket)
		>> bResult
		>> dwAuctionID;

	CTAuctionCommander::GetInstance()
		->RecvReg_Result( bResult );

	CTAuctionMainFrame::GetInstance()
		->RequestInfo();

	return TERR_NONE;
}

int CTClientGame::OnCS_AUCTIONREGCANCEL_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult = 0;

	(*pPacket)
		>> bResult;

	CTAuctionCommander::GetInstance()
		->RecvRegCancel_Result( bResult );

	return TERR_NONE;
}

int CTClientGame::OnCS_ITEMCHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	WORD wItem=0;
	BYTE bCount=0;

	(*pPacket)
		>> bResult
		>> wItem
		>> bCount;

	DWORD dwSoundID;
	CString strMSG;

	switch( bResult )
	{
	case ITEMCHANGE_SUCCESS:
		{
			LPTITEM pTITEM = CTChart::FindTITEMTEMP(wItem);
			if( pTITEM )
			{
				if( bCount == 1 )
				{
					strMSG = CTChart::Format( TSTR_ITEMCHANGE_SUCCESS,
						pTITEM->m_strNAME );
				}
				else if( bCount > 1 )
				{
					strMSG = CTChart::Format( TSTR_ITEMCHANGE_COUNT_SUCCESS,
						pTITEM->m_strNAME,
						bCount);
				}

				dwSoundID = ID_SND_ITEM_SUCCESS;
			}
		}
		break;

	case ITEMCHANGE_FAIL:
		strMSG = CTChart::LoadString( TSTR_ITEMCHANGE_FAIL );
		dwSoundID = ID_SND_ITEM_FAILED;
		break;

	case ITEMCHANGE_INVALID:
		strMSG = CTChart::LoadString( TSTR_ITEMCHANGE_INVALID );
		dwSoundID = ID_SND_ITEM_FAILED;
		break;

	case ITEMCHANGE_STATUS:
		strMSG = CTChart::LoadString( TSTR_ITEMCHANGE_STATUS );
		dwSoundID = ID_SND_ITEM_FAILED;
		break;

	case ITEMCHANGE_FULL:
		strMSG = CTChart::LoadString( TSTR_ITEMCHANGE_FULL );
		dwSoundID = ID_SND_ITEM_FAILED;
		break;
	}

	if( !strMSG.IsEmpty() )
	{
		/*m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			TRUE,
			dwSoundID,
			MEDIA_TSOUND );*/

		m_pChatFrame->ChatSysMSG(
			CTChart::LoadString( TSTR_INFO_TITLE),
			m_pMainChar->GetName(),
			strMSG,
			TCOLOR_INFO,
			TCHAT_FLAG_INFO,
			dwSoundID,
			MEDIA_TSOUND);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_COUNTDOWN_ACK(CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCommand;
	(*pPacket)
		>> dwCommand;

	TFrame::m_pCommandHandler->m_vCOMMAND.push_back(dwCommand);
	return TERR_NONE;
}

int CTClientGame::OnCS_CHANGEITEMATTR_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bInven;
	BYTE bSlotID;

	(*pPacket)
		>> bInven
		>> bSlotID;

	CTClientItem* pItem = FindMainItemByInven( bInven, bSlotID );
	if( pItem )
	{
		pItem->SetItemData( pPacket );
		pItem->SetItemMagic( pPacket );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_WASTEREFINE_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	CString strMSG;
	DWORD dwSoundID = ID_SND_ERROR;

	switch( bResult )
	{
	case WASTEREFINE_SUCCESS:
		strMSG = CTChart::LoadString( TSTR_WASTEREFINE_SUCCESS );
		dwSoundID = ID_SND_INFO;
		break;
	case WASTEREFINE_NOTFOUND:
		strMSG = CTChart::LoadString( TSTR_WASTEREFINE_NOTFOUND );
		dwSoundID = ID_SND_ERROR;
		break;
	case WASTEREFINE_NOREFINE:
		strMSG = CTChart::LoadString( TSTR_WASTEREFINE_NOREFINE );
		dwSoundID = ID_SND_ERROR;
		break;
	case WASTEREFINE_FULLINVEN:
		strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_INVENFULL );
		dwSoundID = ID_SND_ERROR;
		break;
	}

	m_pMainWnd->MessageBoxOK(
		strMSG,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		0,
		FALSE,
        dwSoundID,
		MEDIA_TSOUND );
    
	ResetInvenUI();
	return TERR_NONE;
}

int CTClientGame::OnCS_CASHSHOPSTOP_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bRet = 0;
	(*pPacket)
		>> bRet;

	m_pMainWnd->CloseMessageBox(TRUE);
	DisableUI( TFRAME_CASHSHOP_NEW );
	return TERR_NONE;
}

int CTClientGame::OnCS_APEXDATA_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
#ifndef TEST_MODE
	char cBuf[MAX_APEXPACKET];

	int nBufLen;

	(*pPacket)
		>> nBufLen;
	//	>> cBuf

	pPacket->DetachBinary( (LPVOID)cBuf );

	if(m_pApexfRec)
	{
		m_pApexfRec( cBuf, nBufLen );
	}
#endif

	return TERR_NONE;
}

int CTClientGame::OnCS_KICKOUTMAP_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	(*pPacket)
		>> m_wKICKOUTMAP;

	m_bDoKICKOUTMAP = TRUE;

#ifdef TEST_MODE
	m_dwLeftTickKICKOUTMAP = 1000*60; //1분
#else
	m_dwLeftTickKICKOUTMAP = 3600000; //1시간
#endif

	m_pMainWnd->MessageBoxOK(
		TSTR_KICKOUTMAP_BECAUSE_LEVELLIMIT,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		GM_CLOSE_MSGBOX,
		TRUE,
		ID_SND_INFO,
		MEDIA_TSOUND,
		TMSGBOXOPEN_CLOSEALL_ALREADY);

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSWANTEDADD_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	(*pPacket)
		>> bResult;

	CString strMSG;

	switch( bResult )
	{
	case GUILD_MAXWANTED:
		strMSG = CTChart::LoadString( TSTR_GUILD_MAXWANTED );
		break;
	}

	if( !strMSG.IsEmpty() )
	{
		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE,
			ID_SND_ERROR); 
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSWANTEDDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	return TERR_NONE;
}

bool SortGuildTacticsOffer(CTGuildCommander::GuildTacticsOffer& a, CTGuildCommander::GuildTacticsOffer& b)
{
	return a.m_dlTime > b.m_dlTime;
}

int CTClientGame::OnCS_GUILDTACTICSWANTEDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	pCmd->m_GuildTacticsOfferVec.clear();

	DWORD dwCount = 0;
	(*pPacket)
		>> dwCount;

	for( DWORD i=0 ; i < dwCount ; ++i )
	{
		CTGuildCommander::GuildTacticsOffer info;
		(*pPacket)
			>> info.m_dwID
			>> info.m_dwGuildID
			>> info.m_strGuildName
			>> info.m_strTitle
			>> info.m_strText
			>> info.m_bDay
			>> info.m_bMinLevel
			>> info.m_bMaxLevel	
			>> info.m_dwPoint
			>> info.m_dwGold
			>> info.m_dwSilver
			>> info.m_dwCooper
			>> info.m_dlTime
			>> info.m_bApplied;

		pCmd->m_GuildTacticsOfferVec.push_back( info );
	}

	std::sort(
		pCmd->m_GuildTacticsOfferVec.begin(),
		pCmd->m_GuildTacticsOfferVec.end(),
		SortGuildTacticsOffer);

	static_cast<CTGuildApplicationDlg*>( m_vTFRAME[ TFRAME_GUILDAPP ] )->ResetInfo();
	EnableUI( TFRAME_GUILDAPP );
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSVOLUNTEERING_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	(*pPacket)
		>> bResult;

	CString strMSG;

	switch( bResult )
	{
	case GUILD_SUCCESS:
		strMSG = CTChart::LoadString( TSTR_GUILD_VOL_SUCCESS );
		break;
	case GUILD_MISMATCHLEVEL:
		strMSG = CTChart::LoadString( TSTR_GUILD_MISMATCHLEVEL );
		break;
	case GUILD_SAMEGUILDTACTICS:
		strMSG = CTChart::LoadString( TSTR_GUILD_SAMEGUILDTACTICS3 );
		break;
	case GUILD_HAVEGUILD:
		strMSG = CTChart::LoadString( TSTR_GUILD_ALREADY_TACTICS );
		break;
	case GUILD_WANTEDEND:
		strMSG = CTChart::LoadString( TSTR_GUILD_WANTEDEND );
		break;
	case GUILD_AREADYAPPLY:
		strMSG = CTChart::LoadString( TSTR_GUILD_AREADYAPPLY );
		break;
	default:
		strMSG = CTChart::LoadString( TSTR_GUILD_VOLUNTEERING_FAIL );
		break;
	}

	if( !strMSG.IsEmpty() )
	{
		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE,
			ID_SND_ERROR);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSVOLUNTEERINGDEL_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSREPLY_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwTarget;
	(*pPacket)
		>> bResult
		>> dwTarget;

	if( dwTarget == m_pMainChar->m_dwID ) // 용병가입 대상자
	{
		if( bResult == GUILD_SUCCESS )
			EnableGuildUI(TRUE);
	}
	else // 길드장
	{
		CString strMSG;

		switch( bResult )
		{
		case GUILD_SUCCESS:
			strMSG = CTChart::LoadString( TSTR_GUILD_TACTICSREPLY_SUCCESS );
			break;
		case GUILD_MEMBER_FULL:
			strMSG = CTChart::LoadString( TSTR_GUILD_MEMBER_FULL );
			break;
		case GUILD_NOMONEY:
			strMSG = CTChart::LoadString( TSTR_GUILD_NOMONEY );
			break;
		case GUILD_NOPOINT:
			strMSG = CTChart::LoadString( TSTR_GUILD_NOPOINT );
			break;
		case GUILD_SAMEGUILDTACTICS: 
			strMSG = CTChart::LoadString( TSTR_GUILD_SAMEGUILDTACTICS4 );
			break;
		}

		if( !strMSG.IsEmpty() )
		{
			m_pMainWnd->MessageBoxOK(
				strMSG,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0,
				FALSE,
				ID_SND_ERROR);
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSKICKOUT_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	DWORD dwTarget;
	(*pPacket)
		>> bResult
		>> dwTarget;

	if( bResult == GUILD_SUCCESS )
	{
		if( dwTarget == m_pMainChar->m_dwID )
		{
			m_pMainChar->m_dwTacticsID = 0;
			m_pMainChar->m_strTACTICS.Empty();

			EnableGuildUI( m_pMainChar->m_dwGuildID );
			m_pChatFrame->ToggleTacticsChatMode( m_pMainChar->m_dwTacticsID );
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_GUILDTACTICSVOLUNTEERLIST_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCount;

	(*pPacket)
		>> dwCount;

	CTGuildCommander* pCmd = CTGuildCommander::GetInstance();
	pCmd->m_TacticsVolunteerVec.clear();

	for(DWORD i=0; i<dwCount; ++i)
	{
		CTGuildCommander::GuildTacticsVolunteer ginfo;

		(*pPacket)
			>> ginfo.m_dwCharID
			>> ginfo.m_strName
			>> ginfo.m_bLevel
			>> ginfo.m_bClass
			>> ginfo.m_dwRegion
			>> ginfo.m_bDay
			>> ginfo.m_dwPoint
			>> ginfo.m_dwGold
			>> ginfo.m_dwSilver
			>> ginfo.m_dwCooper;

		pCmd->m_TacticsVolunteerVec.push_back( ginfo );
	}

	CTCommunityDlg* pDlg = static_cast<CTCommunityDlg*>( m_vTFRAME[TFRAME_COMMUNITY] );
	CTGuildVolunteerFrame* pFrame = static_cast<CTGuildVolunteerFrame*>( pDlg->GetInnerFrame(TCOMMUNITY_FRAME_GUILDVOLUNTEER) );
	pFrame->ResetInfo();

	return TERR_NONE;
}

int CTClientGame::OnCS_REGGUILDCLOAK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	BYTE bResult;
	BYTE bInvenID;
	BYTE bItemID;
	BYTE bReg;

	(*pPacket)
		>> bResult
		>> bInvenID
		>> bItemID
		>> bReg;

	if( bResult == 0 )
	{
		if( bReg )
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_REGGUILDCLOAK_SUCCESS,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0,
				TRUE,
				ID_SND_ITEM_SUCCESS,
				MEDIA_TSOUND );
		}
		else
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_REGGUILDCLOAK_UNREG_SUCCESS,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0,
				TRUE,
				ID_SND_ITEM_SUCCESS,
				MEDIA_TSOUND );
		}
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_RESETCLOAK_ACK( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwCharID;
	BYTE bInvenID;
	BYTE bItemID;

	(*pPacket)
		>> dwCharID
		>> bInvenID
		>> bItemID;

	CTClientChar* pPC = FindPC( dwCharID );
	if( pPC )
	{
		MAPTINVEN::iterator itINVEN = pPC->m_mapTINVEN.find( bInvenID );
		if( itINVEN != pPC->m_mapTINVEN.end() )
		{
			CTClientItem* pTITEM = (*itINVEN).second->FindTItem( bItemID );
			if( pTITEM )
			{
				pTITEM->SetRegGuild( FALSE );
				pPC->ResetEQUIP( m_pDevice, m_pRES);
			}
		}
	}

	return TERR_NONE;
}


int CTClientGame::OnCS_FAMERANKLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTFameBaseDlg* pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[TFRAME_FAME_BASE] );

	BYTE bType;
	BYTE bCount;
	BYTE bMonth;
	WORD wYear;
	BYTE bSelectWarLord = TRUE;

	(*pPacket)
		>> bType
		>> wYear
		>> bMonth
		>> bCount;

	pDlg->m_bType = bType;
	pDlg->m_bMonth = bMonth;
	pDlg->m_wYear = wYear;
	pDlg->ClearRank();

	for( BYTE i = 0 ; i < bCount; ++i )
	{
		CTFameRank* pRank = new CTFameRank;

		(*pPacket)
			>> pRank->dwTotalRank
			>> pRank->dwMonthRank
			>> pRank->dwCharID
			>> pRank->strName
			>> pRank->dwTotalPoint
			>> pRank->dwMonthPoint
			>> pRank->wWin
			>> pRank->wLose
			>> pRank->dwTotalWin
			>> pRank->dwTotalLose
			>> pRank->bCountry
			>> pRank->bLevel
			>> pRank->bClass
			>> pRank->bRace
			>> pRank->bSex
			>> pRank->bHair
			>> pRank->bFace
			>> pRank->strSay
			>> pRank->strGuild;

		if( pDlg->m_bType == FRT_GOD ||
			pDlg->m_bType == FRT_GODDESS )
		{
			pDlg->m_mapFAMERANK.insert( TMAPFAMERANK::value_type( i, pRank ) );
		}
		else
		{
			pDlg->m_mapFAMERANK.insert( TMAPFAMERANK::value_type( pRank->dwMonthRank, pRank ) );
		}

		if( i == 0 )
		{
			*(pDlg->m_pWarLord) = *pRank;
		}
	}

	pDlg->ResetRankInfo();
	return TERR_NONE;
}


int CTClientGame::OnCS_MONTHRANKLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTCharRankInfoDlg* pDlg = static_cast<CTCharRankInfoDlg*>( m_vTFRAME[TFRAME_RANK_INFO] );
	TMAPFAMERANK::iterator itFRAMERANK;

	BYTE bMonth;
	BYTE bCount;
	BYTE bCountry;

	(*pPacket)
		>> bMonth
		>> pDlg->m_bFirstCount
		>> bCount;

	// 만약 데이터가 있으면 랭킹을 초기화 한다.
	if ( bCount > 0 )
		pDlg->ReleaseData();

	for ( BYTE i = 0; i < 2; ++i )
	{
		(*pPacket)
			>> bCountry;

		for( BYTE j = 0; j < bCount; ++j )
		{
			CTFameRank* pRank = new CTFameRank;

			(*pPacket)
				>> pRank->dwTotalRank
				>> pRank->dwMonthRank
				>> pRank->dwCharID
				>> pRank->strName
				>> pRank->dwTotalPoint
				>> pRank->dwMonthPoint
				>> pRank->wWin
				>> pRank->wLose
				>> pRank->dwTotalWin
				>> pRank->dwTotalLose
				>> pRank->bCountry
				>> pRank->bLevel
				>> pRank->bClass
				>> pRank->bRace
				>> pRank->bSex
				>> pRank->bHair
				>> pRank->bFace
				>> pRank->strSay
				>> pRank->strGuild;		 

			pDlg->m_mapMONTHFAMERANK[bCountry].insert( TMAPFAMERANK::value_type( pRank->dwMonthRank, pRank ));
		}
	}

	CTCharInfoDlg::m_bTabIndex = TINFO_RANK;
	pDlg->ResetData( CTCharRankInfoDlg::m_bTabIndex, CTCharInfoDlg::m_bTabIndex );
	pDlg->ShowComponent( TRUE );
	
	return TERR_NONE;
}

int CTClientGame::OnCS_UPDATEFAMERANKLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTFameBaseDlg* pDlg = static_cast<CTFameBaseDlg*>( m_vTFRAME[TFRAME_FAME_BASE] );

	BYTE bTypeCount;
	BYTE bMonth;
	WORD wYear;

	(*pPacket)
		>> wYear
		>> bMonth
		>> bTypeCount;

	for( BYTE b=0 ; b < bTypeCount ; ++b )
	{
		BYTE bType;
		BYTE bCount;

		(*pPacket)
			>> bType
			>> bCount;

		{
			TMAPFAMERANK::iterator it, end;
			it = m_mapTFAMERANK[bType].begin();
			end = m_mapTFAMERANK[bType].end();
			for(; it != end ; ++it )
				delete (*it).second;
			m_mapTFAMERANK[bType].clear();
		}
	
		BOOL bUpdateUI = (pDlg->m_bType == bType && pDlg->m_bMonth == bMonth && pDlg->m_wYear == wYear);
		if(bUpdateUI) // 명성UI 정보도 갱신하자.
			pDlg->ClearRank();

		for( BYTE i = 0; i < bCount; ++i )
		{
			CTFameRank* pRank = new CTFameRank;

			(*pPacket)
				>> pRank->dwTotalRank
				>> pRank->dwMonthRank
				>> pRank->dwCharID
				>> pRank->strName
				>> pRank->dwTotalPoint
				>> pRank->dwMonthPoint
				>> pRank->wWin
				>> pRank->wLose
				>> pRank->dwTotalWin
				>> pRank->dwTotalLose
				>> pRank->bCountry
				>> pRank->bLevel
				>> pRank->bClass
				>> pRank->bRace
				>> pRank->bSex
				>> pRank->bHair
				>> pRank->bFace
				>> pRank->strSay
				>> pRank->strGuild;

			if( bType == FRT_GOD ||
				bType == FRT_GODDESS )
			{
				std::pair<TMAPFAMERANK::iterator, bool> ret =  m_mapTFAMERANK[bType].insert( TMAPFAMERANK::value_type( i, pRank ) );
				if( !ret.second )
				{
					delete pRank;
					continue;
				}
			}
			else
			{
				std::pair<TMAPFAMERANK::iterator, bool> ret =  m_mapTFAMERANK[bType].insert( TMAPFAMERANK::value_type( pRank->dwMonthRank, pRank ) );
				if( !ret.second )
				{
					delete pRank;
					continue;
				}
			}

			if(bUpdateUI)
			{
				if( i == 0 )
				{
					*(pDlg->m_pWarLord) = *pRank;
				}

				CTFameRank* pRank2 = new CTFameRank;
				*pRank2 = *pRank;


				if( bType == FRT_GOD ||
					bType == FRT_GODDESS )
				{
					std::pair<TMAPFAMERANK::iterator, bool> ret =  pDlg->m_mapFAMERANK.insert( TMAPFAMERANK::value_type( i, pRank2 ) );
					if( !ret.second )
						delete pRank;
				}
				else
				{
					std::pair<TMAPFAMERANK::iterator, bool> ret =  pDlg->m_mapFAMERANK.insert( TMAPFAMERANK::value_type( pRank->dwMonthRank, pRank2 ) );
					if( !ret.second )
						delete pRank;
				}
			}
		}
	}

	pDlg->ResetRankInfo();
	CTClientGame::GetInstance()->UpdateMonthRank();
	return TERR_NONE;
}

int CTClientGame::OnCS_AFTERMATH_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	DWORD dwCharID;
	BYTE bStep;

	(*pPacket)
		>> dwCharID
		>> bStep;

	CTClientChar* pChar = FindPlayer( dwCharID );
	if( pChar )
		pChar->m_bAfterMath = bStep;

	return TERR_NONE;
}

int CTClientGame::OnCS_CHATBAN_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTChatCommander *pChatCommander = CTChatCommander::GetInstance();
	BYTE bRet;

	(*pPacket)
		>> bRet;

	switch (bRet)
	{
	case CHATBAN_SUCCESS:
		{
			pChatCommander->ChatSysMSG( CTChart::LoadString( TSTR_CMD_CHAT_BAN_SUCCESS ) );
		}
		break;
	case CHATBAN_NOTOPERATOR:
		{
			pChatCommander->ChatSysMSG( CTChart::LoadString( TSTR_CMD_CHAT_BAN_NOTOPERATOR ) );
		}
		break;
	case CHATBAN_INVALIDCHAR:
		{
			pChatCommander->ChatSysMSG( CTChart::LoadString( TSTR_CMD_CHAT_BAN_INVALIDCHAR ) );
		}
		break;
	case CHATBAN_MAXMIN:
		{
			pChatCommander->ChatSysMSG( CTChart::LoadString( TSTR_CMD_CHAT_BAN_MAXMIN ) );
		}
		break;
	}


	return TERR_NONE;

}

int CTClientGame::OnCS_FIRSTGRADEGROUP_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTCharRankInfoDlg* pDLG = static_cast<CTCharRankInfoDlg*>( m_vTFRAME[ TFRAME_RANK_INFO ] );

	BYTE bRankMonth;
	BYTE bCount;

	(*pPacket)
		>> bRankMonth
		>> bCount;

	for(BYTE i = 0; i < COUNTRY_COUNT; i++)
	{
		BYTE bCountry = 0;
		(*pPacket)
			>> bCountry;

		for(BYTE j = 0; j < bCount; j++)
		{
			CTFameRank* pFameRank = new CTFameRank;

			(*pPacket)
				>> pFameRank->dwTotalRank
				>> pFameRank->dwMonthRank
				>> pFameRank->dwCharID
				>> pFameRank->strName
				>> pFameRank->dwTotalPoint
				>> pFameRank->dwMonthPoint
				>> pFameRank->wWin
				>> pFameRank->wLose
				>> pFameRank->dwTotalWin
				>> pFameRank->dwTotalLose
				>> pFameRank->bCountry
				>> pFameRank->bLevel
				>> pFameRank->bClass
				>> pFameRank->bRace
				>> pFameRank->bSex
				>> pFameRank->bHair
				>> pFameRank->bFace
				>> pFameRank->strSay
				>> pFameRank->strGuild;

			if( pFameRank->dwMonthRank == 0 ||
				!pFameRank->dwCharID)
			{
				delete pFameRank;
				continue;
			}

			pDLG->m_vtFIRSTGRADEGROUP.push_back( pFameRank );
		}
	}

	CTCharInfoDlg::m_bTabIndex = TINFO_RANK;
	pDLG->ResetData( CTCharRankInfoDlg::m_bTabIndex, CTCharInfoDlg::m_bTabIndex );
	pDLG->ShowComponent( TRUE );

	return TERR_NONE;
}

#ifdef ADD_TOURNAMENT
int CTClientGame::OnCS_TOURNAMENTAPPLYINFO_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	pDLG->ReleaseData();

	BYTE bEntryCount; //종목수
	(*pPacket)
		>> bEntryCount;

	for( BYTE i=0 ; i < bEntryCount ; ++i )
	{
		TMATCH* pMatch = new TMATCH;

		(*pPacket)
#ifdef MODIFY_TT
			>> pMatch->m_bGroup
#endif
			>> pMatch->m_bEntryID
			>> pMatch->m_strEntry
			>> pMatch->m_bType
			>> pMatch->m_dwClass
			>> pMatch->m_bMyApplied
			>> pMatch->m_dwFee
			>> pMatch->m_dwBackFee
			>> pMatch->m_bPermitCount
			>> pMatch->m_bNeed
			>> pMatch->m_wTotalApplicant
			>> pMatch->m_bRewardCount;

		for( int r=0 ; r < pMatch->m_bRewardCount ; ++r )
		{
			TMATCHREWARD* pReward = new TMATCHREWARD;

			(*pPacket)
				>> pReward->m_bCheckShield
				>> pReward->m_bChartType // 0:퀘스트 1:아이템차트
				>> pReward->m_wItemID
				>> pReward->m_bCount;

			pMatch->m_vReward.push_back( pReward );
		}

		BYTE b1stCount; // 1군지원자
		(*pPacket)
			>> b1stCount;

		for( int f=0 ; f < b1stCount ; ++f )
		{
			TMATCHPLAYER* pElite = new TMATCHPLAYER;

			(*pPacket)
				>> pElite->m_dwCharID
				>> pElite->m_bCountry
				>> pElite->m_strName
				>> pElite->m_bLevel
				>> pElite->m_bClass
				>> pElite->m_dwRank
				>> pElite->m_dwMonthRank;

			pMatch->m_vPlayer.push_back( pElite );
		}

		pDLG->m_vMatch.push_back( pMatch );
	}

	std::sort(
		pDLG->m_vMatch.begin(),
		pDLG->m_vMatch.end(),
		_SortMATCH );

	pDLG->UpdateMatch( TT_ENTRY );
	if( pDLG->IsVisible() )
	{
		pDLG->ShowComponent(TRUE);
		pDLG->m_pMATCHLIST->SetCurSelItem(pDLG->m_nPrevSelectedMatch);
	}
	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTAPPLY_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult;
#ifdef MODIFY_TT
	BYTE bGroup;
#endif
	BYTE bEntryID;

	(*pPacket)
		>> bResult
#ifdef MODIFY_TT
		>> bGroup
#endif
		>> bEntryID;

	CString strMSG;
	switch( bResult )
	{
		case TOURNAMENT_SUCCESS:
			strMSG = CTChart::LoadString(TSTR_TOURNAMENT_JOIN_OK);
			break;
		case TOURNAMENT_FAIL:
			strMSG = CTChart::LoadString(TSTR_TOURNAMENT_JOIN_FAILED);
			break;
		case TOURNAMENT_TIMEOUT:
			strMSG = CTChart::LoadString(TSTR_TOURNAMENT_JOIN_EXPIRED);
			break;
		case TOURNAMENT_FULL:
			strMSG = CTChart::LoadString(TSTR_TOURNAMENT_JOIN_EXCESS);
			break;
		case TOURNAMENT_DISQUALIFY:
			strMSG = CTChart::LoadString(TSTR_TOURNAMENT_JOIN_1ST);
			break;
		case TOURNAMENT_CLASS:
			strMSG = CTChart::LoadString(TSTR_TOURNAMENT_NOT_CLASS);
			break;
		case TOURNAMENT_MONEY:
			strMSG = CTChart::LoadString(TSTR_TOURNAMENT_NEED_MONEY);
			break;
		case TOURNAMENT_ITEM:
			strMSG = CTChart::LoadString(TSTR_TOURNAMENT_NEED_ITEM);
			break;
	}

	if( !strMSG.IsEmpty() )
	{
		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTJOINLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	pDLG->ReleaseData();

	BYTE bEntryCount; // 종목수
	(*pPacket)
		>> bEntryCount;

	for( int i=0 ; i < bEntryCount ; ++i )
	{
		TMATCH* pMatch = new TMATCH;

		(*pPacket)
#ifdef MODIFY_TT
			>> pMatch->m_bGroup
#endif
			>> pMatch->m_bEntryID
			>> pMatch->m_strEntry //종목이름
			>> pMatch->m_bType
			>> pMatch->m_dwClass
			>> pMatch->m_bMyApplied
			>> pMatch->m_bRewardCount;

		for( int r=0 ; r < pMatch->m_bRewardCount ; ++r )
		{
			TMATCHREWARD* pReward = new TMATCHREWARD;

			(*pPacket)
				>> pReward->m_bCheckShield
				>> pReward->m_bChartType
				>> pReward->m_wItemID
				>> pReward->m_bCount;

			pMatch->m_vReward.push_back( pReward );
		}

		BYTE bJoinCount; // 참가자수
		(*pPacket)
			>> bJoinCount;

		for( int j=0 ; j < bJoinCount ; ++j )
		{
			TMATCHPLAYER* pEntry = new TMATCHPLAYER;

			(*pPacket)
				>> pEntry->m_dwCharID
				>> pEntry->m_bCountry
				>> pEntry->m_strName
				>> pEntry->m_bLevel
				>> pEntry->m_bClass
				>> pEntry->m_dwRank
				>> pEntry->m_dwMonthRank;

			pMatch->m_vPlayer.push_back( pEntry );
		}

		pDLG->m_vMatch.push_back( pMatch );
	}

	std::sort(
		pDLG->m_vMatch.begin(),
		pDLG->m_vMatch.end(),
		_SortMATCH );

	pDLG->UpdateMatch( TT_JOIN );
	if( pDLG->IsVisible() )
	{
		pDLG->ShowComponent(TRUE);
		pDLG->m_pMATCHLIST->SetCurSelItem(pDLG->m_nPrevSelectedMatch);
	}
	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTPARTYLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
    
	DWORD dwChiefID;
	BYTE bMemberCount;
	(*pPacket)
		>> dwChiefID
		>> bMemberCount;

	LPTMATCHPLAYER pPlayer = pDLG->FindPlayer( dwChiefID );
	if( pPlayer != NULL )
	{
		pPlayer->ClearParty();

		for( int i=0 ; i < bMemberCount ; ++i )
		{
			LPTMATCHPARTY pPARTY = new TMATCHPARTY;

			(*pPacket)
				>> pPARTY->m_dwCharID
				>> pPARTY->m_bCountry
				>> pPARTY->m_strName
				>> pPARTY->m_bLevel
				>> pPARTY->m_bClass
				>> pPARTY->m_dwRank
				>> pPARTY->m_dwMonthRank;

			pPlayer->m_vParty.push_back( pPARTY );
		}
	}

	pDLG->ResetParty();
	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTPARTYADD_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult;
	CString strName;

	(*pPacket)
		>> bResult
		>> strName;

	CString strMSG;
	switch( bResult )
	{
	case TOURNAMENT_SUCCESS:
		strMSG = CTChart::Format(TSTR_TOURNAMENT_PARTY_SUCCESS,
			strName);
		break;
	case TOURNAMENT_NOTFOUND:
		strMSG = CTChart::LoadString(TSTR_TOURNAMENT_PARTY_NOTFOUND);
		break;
	case TOURNAMENT_ALREADYREG:
		strMSG = CTChart::LoadString(TSTR_TOURNAMENT_PARTY_ALREADY);
		break;
	case TOURNAMENT_FULL:
		strMSG = CTChart::LoadString(TSTR_TOURNAMENT_PARTY_FULL);
		break;
	}

	if( !strMSG.IsEmpty() )
	{
		static_cast<CTTournamentSelectTarget*>( m_vTFRAME[ TFRAME_TOURNAMENT_SELECT_TARGET ] )->m_pEDIT_NAME->ClearText();

		m_pMainWnd->MessageBoxOK(
			strMSG,
			TSTR_OK,
			GM_CLOSE_MSGBOX,
			0,
			FALSE );
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTMATCHLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[TFRAME_TOURNAMENT] );
	pDLG->ReleaseData();

	BYTE bEntryCount; // 종목수
	(*pPacket)
		>> bEntryCount;

	for( int i=0 ; i < bEntryCount ; ++i )
	{
		TMATCH* pMatch = new TMATCH;

		(*pPacket)
#ifdef MODIFY_TT
			>> pMatch->m_bGroup
#endif
			>> pMatch->m_bEntryID
			>> pMatch->m_strEntry
			>> pMatch->m_bType
			>> pMatch->m_dwClass
			>> pMatch->m_bMyApplied
			>> pMatch->m_bRewardCount;

		for( int r=0 ; r < pMatch->m_bRewardCount ; ++r )
		{
			TMATCHREWARD* pReward = new TMATCHREWARD;

			(*pPacket)
				>> pReward->m_bCheckShield
				>> pReward->m_bChartType
				>> pReward->m_wItemID
				>> pReward->m_bCount;

			pMatch->m_vReward.push_back( pReward );
		}

		BYTE bJoinCount; //참가자수
		(*pPacket)
			>> bJoinCount;

		for( int j=0 ; j < bJoinCount ; ++j )
		{
			TMATCHPLAYER* pPlayer = new TMATCHPLAYER;

			(*pPacket)
				>> pPlayer->m_bSlotID
				>> pPlayer->m_dwCharID
				>> pPlayer->m_bCountry
				>> pPlayer->m_strName
				>> pPlayer->m_bLevel
				>> pPlayer->m_bClass
				>> pPlayer->m_dwRank
				>> pPlayer->m_dwMonthRank
				>> pPlayer->m_bQfWin // 8강 승패
				>> pPlayer->m_bSfWin // 4강 승패
				>> pPlayer->m_bFWin; // 결승 승패

			pMatch->m_vPlayer.push_back( pPlayer );
		}

		pDLG->m_vMatch.push_back( pMatch );
	}

	std::sort(
		pDLG->m_vMatch.begin(),
		pDLG->m_vMatch.end(),
		_SortMATCH );

	pDLG->UpdateMatch( TT_MATCH );
	if( pDLG->IsVisible() )
	{
		pDLG->ShowComponent(TRUE);
		pDLG->m_pMATCHLIST->SetCurSelItem(pDLG->m_nPrevSelectedMatch);
	}
	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTEVENTLIST_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTTournamentEvent* pDLG = (CTTournamentEvent*) m_vTFRAME[ TFRAME_TOURNAMENT_EVENT ];

	pDLG->ClearMatch();

	BYTE bBase; //기본 당첨금
	DWORD dwSum; //총 당첨금
	BYTE bEntryCount; // 종목수
	(*pPacket)
		>> bBase
		>> dwSum
		>> bEntryCount;

	pDLG->SetInfo( bBase, dwSum );

	CString strFMT;
	for( int i=0 ; i < bEntryCount ; ++i )
	{
		LPTTEVENTMATCH pMatch = new TTEVENTMATCH;

		(*pPacket)
			>> pMatch->m_bEntryID // 종목
			>> pMatch->m_strEntry //종목이름
			>> pMatch->m_bType // 구분
			>> pMatch->m_strChoice
			>> pMatch->m_bCountry
			>> pMatch->m_fRate //비율
			>> pMatch->m_dwAmount; //당청금

		pDLG->m_vMatch.push_back( pMatch );
	}

	pDLG->OnMatchDataReset();

	if( !pDLG->IsVisible() )
		EnableUI( TFRAME_TOURNAMENT_EVENT );

	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTEVENTINFO_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	CTTournamentEvent* pDLG = static_cast<CTTournamentEvent*>( m_vTFRAME[TFRAME_TOURNAMENT_EVENT] );

	BYTE bEntryID;
	BYTE bBase;
	DWORD dwSum;
	BYTE bJoinCount; // 참가자수
	(*pPacket)
		>> bEntryID
		>> bBase
		>> dwSum
		>> bJoinCount;

	pDLG->SetInfo(bBase, dwSum);

	LPTTEVENTMATCH pMatch = pDLG->GetEntry( bEntryID );
	if( !pMatch )
		return TERR_NONE;

	pMatch->ClearList();

	CString strFMT;
	for( int i=0 ; i < bJoinCount ; ++i )
	{
		LPTTEVENTTARGET pTarget = new TTEVENTTARGET;

		(*pPacket)
			>> pTarget->m_dwCharID
			>> pTarget->m_bCountry
			>> pTarget->m_strGuildName
			>> pTarget->m_strName
			>> pTarget->m_bLevel
			>> pTarget->m_bClass
			>> pTarget->m_dwRank
			>> pTarget->m_dwMonthRank
			>> pTarget->m_fRate
			>> pTarget->m_bPartyCount;

		for( int p=0 ; p < pTarget->m_bPartyCount ; ++p )
		{
			LPTTEVENTTARGETPARTY pParty = new TTEVENTTARGETPARTY;

			(*pPacket)
				>> pParty->m_dwCharID
				>> pParty->m_bCountry
				>> pParty->m_strGuildName
				>> pParty->m_strName
				>> pParty->m_bLevel
				>> pParty->m_bClass
				>> pParty->m_dwRank
				>> pParty->m_dwMonthRank;

			pTarget->m_vParty.push_back( pParty );
		}

		pMatch->m_vList.push_back( pTarget );
	}

	pDLG->OnListDataReset( pMatch );
	if( pDLG->IsVisible() )
	{
		pDLG->m_nMode = TTEVENT_SELECT_ENTRY;
		pDLG->ShowComponent(TRUE);
		pDLG->ResetShow();
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTEVENTJOIN_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult;
	(*pPacket)
		>>bResult;

	if( m_pSESSION )
		m_pSESSION->SendCS_TOURNAMENTEVENTLIST_REQ();

	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTCHEER_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	DWORD dwUseCharID;
	DWORD dwTargetCharID;

	(*pPacket)
		>> dwUseCharID
		>> dwTargetCharID;

	return TERR_NONE;
}

bool _SortTTEAM( const LPTTOURNAMENTTEAM a, const LPTTOURNAMENTTEAM b )
{
	return a->m_bSlotID < b->m_bSlotID;
}

int CTClientGame::OnCS_TOURNAMENTENTER_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	m_bTournamentWatching = TRUE;
	m_pMainChar->m_bTournamentTeam = TTOURNAMENT_TEAM_NONE;
	m_bTournamentBattle = FALSE;
	WORD wLeftKillCount = 0;
	WORD wRightKillCount = 0;
	BYTE bCount;
	VTTOURNAMENTTEAM vTEAM;

	(*pPacket)
		>> wLeftKillCount
		>> wRightKillCount
		>> bCount;

	for( BYTE i=0 ; i < bCount ; ++i )
	{
		TTOURNAMENTPLAYER* pTPlayer = new TTOURNAMENTPLAYER;
		BYTE bSlotID = 0;

		(*pPacket)
			>> bSlotID
			>> pTPlayer->m_dwCharID
			>> pTPlayer->m_bCountry
			>> pTPlayer->m_strName
			>> pTPlayer->m_bLevel
			>> pTPlayer->m_bClass
			>> pTPlayer->m_dwChiefID;

		CTClientChar* pTChar = FindPlayer( pTPlayer->m_dwCharID );
		if( pTChar )
			pTChar->m_bTournamentTeam = bSlotID;

		LPTTOURNAMENTTEAM pTEAM = NULL;
		for( VTTOURNAMENTTEAM::iterator itTEAM=vTEAM.begin() ; itTEAM != vTEAM.end() ; ++itTEAM )
			if( (*itTEAM)->m_bSlotID == bSlotID )
			{
				pTEAM = (*itTEAM);
				break;
			}

		if( pTEAM )
			pTEAM->m_vMembers.push_back( pTPlayer );
		else
		{
			pTEAM = new TTOURNAMENTTEAM;
			pTEAM->m_bSlotID = bSlotID;
			pTEAM->m_wKillCount = 0;
			pTEAM->m_vMembers.push_back( pTPlayer );
			vTEAM.push_back( pTEAM );
		}

		if( pTPlayer->m_dwCharID == m_pMainChar->m_dwID )
		{
			m_pMainChar->m_bTournamentTeam = bSlotID;
			m_bTournamentWatching = FALSE;
			m_bTournamentBattle = TRUE;
			m_pMainChar->ClearMaintainOnDie();
		}
	}

	if( m_pTLeftTeam )
		delete m_pTLeftTeam;

	if( m_pTRightTeam )
		delete m_pTRightTeam;

	m_pTLeftTeam = NULL;
	m_pTRightTeam = NULL;

	// SlotID가 작은 놈이 LeftTeam, SlotID가 큰 놈이 RightTeam
	std::sort( vTEAM.begin(), vTEAM.end(), _SortTTEAM );
	if( !vTEAM.empty() )
	{
		vTEAM[0]->m_wKillCount = wLeftKillCount;
		m_pTLeftTeam = vTEAM[0];

		if( vTEAM.size() > 1 )
		{
			vTEAM[1]->m_wKillCount = wRightKillCount;
			m_pTRightTeam = vTEAM[1];
		}
		else
			m_pTRightTeam = NULL;
	}

	SetTournamentInfo(
		m_pTLeftTeam,
		m_pTRightTeam,
		TRUE);

	if( m_bTournamentWatching )
	{
		EnableUI( TFRAME_TOURNAMENT_GAGE );
		//토너먼트 %d경기장에 입장하셨습니다. %s님은 이 경기장의 관람자입니다.",
	}
	else if( m_bTournamentBattle )
	{
		DisableUI( TFRAME_TOURNAMENT_GAGE );
		// 토너먼트 %d경기장에 입장하셨습니다. %s님은 이 경기장의 선수입니다.
	}

	ResetRSCS(FALSE);

	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTPARTYDEL_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult;
	DWORD dwTargetID;

	(*pPacket)
		>> bResult
		>> dwTargetID;

	/*CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	INT nCur = pDLG->m_pENTRY_PLAYER_LIST->GetCurSelIndex();
	if( nCur != -1 )
	{
		TMATCHPLAYER* pPLAYER = (TMATCHPLAYER*) pDLG->m_pENTRY_PLAYER_LIST->GetItemData( nCur, 0 );
		CTClientGame::GetInstance()->GetSession()->SendCS_TOURNAMENTPARTYLIST_REQ(pPLAYER->m_dwCharID);
	}*/

	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTRESULT_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bStep=0;
	BYTE bResult=0;
	DWORD dwWinID=0;
	DWORD dwLoseID=0;
	DWORD dwBlueTeamHideTick=0;
	DWORD dwRedTeamHideTick=0;

	(*pPacket)
		>> bStep
		>> bResult
		>> dwWinID
		>> dwLoseID
		>> dwBlueTeamHideTick
		>> dwRedTeamHideTick;

	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[TFRAME_TOURNAMENT] );

	VTMATCH::iterator itMATCH, endMATCH;
	itMATCH = pDLG->m_vMatch.begin();
	endMATCH = pDLG->m_vMatch.end();
	for(; itMATCH != endMATCH ; ++itMATCH )
	{
		VTMATCHPLAYER::iterator itPLAYER, endPLAYER;
		itPLAYER = (*itMATCH)->m_vPlayer.begin();
		endPLAYER = (*itMATCH)->m_vPlayer.end();

		for(; itPLAYER != endPLAYER ; ++itPLAYER )
		{
			if( (*itPLAYER)->m_dwCharID == dwWinID )
			{
				switch( bStep )
				{
					case TNMTSTEP_QFINAL: // 8강
						(*itPLAYER)->m_bQfWin = TNMTWIN_WIN;
						break;
					case TNMTSTEP_SFINAL: // 4강
						(*itPLAYER)->m_bSfWin = TNMTWIN_WIN;
						break;
					case TNMTSTEP_FINAL: // 결승
						(*itPLAYER)->m_bFWin = TNMTWIN_WIN;
						break;
				}
			}
			else if( (*itPLAYER)->m_dwCharID == dwLoseID )
			{
				switch( bStep )
				{
					case TNMTSTEP_QFINAL: // 8강
						(*itPLAYER)->m_bQfWin = TNMTWIN_LOSE;
						break;
					case TNMTSTEP_SFINAL: // 4강
						(*itPLAYER)->m_bSfWin = TNMTWIN_LOSE;
						break;
					case TNMTSTEP_FINAL: // 결승
						(*itPLAYER)->m_bFWin = TNMTWIN_LOSE;
						break;
				}
			}
		}
	}

	// 승자와 패자가 어느 팀에 있는지 알아내기.

	LPTTOURNAMENTPLAYER pWinner = NULL;
	LPTTOURNAMENTPLAYER pLoser = NULL;
	INT WinnerTeam = -1; //0:Right 1:Left
	INT LoserTeam = -1; //0:Right 1:Left

	if( m_pTLeftTeam )
	{
		LPTTOURNAMENTPLAYER p = m_pTLeftTeam->FindPlayer(dwWinID);
		if( p )
		{
			pWinner = p;
			WinnerTeam = 1;

			if( NULL == FindPlayer(pWinner->m_dwCharID) )
			{
				for( int i=0 ; i < m_pTLeftTeam->m_vMembers.size() ; ++i )
				{
					if( m_pTLeftTeam->m_vMembers[i]->m_dwCharID != pWinner->m_dwCharID &&
						NULL != FindPlayer(m_pTLeftTeam->m_vMembers[i]->m_dwCharID) )
					{
						pWinner = m_pTLeftTeam->m_vMembers[i];
						break;
					}
				}
			}
		}
		else if( m_pTRightTeam )
		{
			p = m_pTRightTeam->FindPlayer(dwWinID);
			if( p )
			{
				pWinner = p;
				WinnerTeam = 0;

				if( NULL == FindPlayer(pWinner->m_dwCharID) )
				{
					for( int i=0 ; i < m_pTRightTeam->m_vMembers.size() ; ++i )
					{
						if( m_pTRightTeam->m_vMembers[i]->m_dwCharID != pWinner->m_dwCharID &&
							NULL != FindPlayer(m_pTRightTeam->m_vMembers[i]->m_dwCharID) )
						{
							pWinner = m_pTRightTeam->m_vMembers[i];
							break;
						}
					}
				}
			}
		}
	}

	if( m_pTRightTeam )
	{
		LPTTOURNAMENTPLAYER p2 = m_pTRightTeam->FindPlayer(dwLoseID);
		if( p2 )
		{
			pLoser = p2;
			LoserTeam = 0;

			if( NULL == FindPlayer(pLoser->m_dwCharID) )
			{
				for( int i=0 ; i < m_pTRightTeam->m_vMembers.size() ; ++i )
				{
					if( m_pTRightTeam->m_vMembers[i]->m_dwCharID != pLoser->m_dwCharID &&
						NULL != FindPlayer(m_pTRightTeam->m_vMembers[i]->m_dwCharID) )
					{
						pLoser = m_pTRightTeam->m_vMembers[i];
						break;
					}
				}
			}
		}
		else if( m_pTLeftTeam )
		{
			p2 = m_pTLeftTeam->FindPlayer(dwLoseID);
			if( p2 )
			{
				pLoser = p2;
				LoserTeam = 1;

				if( NULL == FindPlayer(pLoser->m_dwCharID) )
				{
					for( int i=0 ; i < m_pTLeftTeam->m_vMembers.size() ; ++i )
					{
						if( m_pTLeftTeam->m_vMembers[i]->m_dwCharID != pLoser->m_dwCharID &&
							NULL != FindPlayer(m_pTLeftTeam->m_vMembers[i]->m_dwCharID) )
						{
							pLoser = m_pTLeftTeam->m_vMembers[i];
							break;
						}
					}
				}
			}
		}
	}

	if( (m_bTournamentBattle || m_bTournamentWatching) &&
		(pWinner || pLoser) )
	{
		CTTournamentResult* pResultDlg = static_cast<CTTournamentResult*>( m_vTFRAME[ TFRAME_TOURNAMENT_RESULT ] );
		DWORD dwLeftCharID = 0;
		DWORD dwRightCharID = 0;
		CString strLeft;
		CString strRight;
		DWORD dwLeftKillCount = 0;
		DWORD dwRightKillCount = 0;
		DWORD dwLeftHideTick = 0;
		DWORD dwRightHideTick = 0;
		BOOL bTrueIsLeftWinButRightWin = TRUE;

		if( pWinner )
		{
			if( WinnerTeam == 0 ) //승자가 레드
			{
				dwRightCharID = pWinner->m_dwCharID;
				strRight = pWinner->m_strName;
				dwRightKillCount = m_pTRightTeam ? m_pTRightTeam->m_wKillCount : 0;
				dwRightHideTick = dwRedTeamHideTick;
				bTrueIsLeftWinButRightWin = FALSE;

				if( m_bTournamentBattle && m_pTRightTeam )
				{
					LPTTOURNAMENTPLAYER pMainTPlayer = m_pTRightTeam->FindPlayer( m_pMainChar->m_dwID );
					if( pMainTPlayer )
					{
						dwRightCharID = pMainTPlayer->m_dwCharID;
						strRight = pMainTPlayer->m_strName;
					}
				}
			}
			else if( WinnerTeam == 1 ) //승자가 블루
			{
				dwLeftCharID = pWinner->m_dwCharID;
				strLeft = pWinner->m_strName;
				dwLeftKillCount = m_pTLeftTeam ? m_pTLeftTeam->m_wKillCount : 0;
				dwLeftHideTick = dwBlueTeamHideTick;
				bTrueIsLeftWinButRightWin = TRUE;

				if( m_bTournamentBattle && m_pTLeftTeam )
				{
					LPTTOURNAMENTPLAYER pMainTPlayer = m_pTLeftTeam->FindPlayer( m_pMainChar->m_dwID );
					if( pMainTPlayer )
					{
						dwLeftCharID = pMainTPlayer->m_dwCharID;
						strLeft = pMainTPlayer->m_strName;
					}
				}
			}
		}

		if( pLoser )
		{
			if( LoserTeam == 0 ) //패자가 레드
			{
				dwRightCharID = pLoser->m_dwCharID;
				strRight = pLoser->m_strName;
				dwRightKillCount = m_pTRightTeam ? m_pTRightTeam->m_wKillCount : 0;
				dwRightHideTick = dwRedTeamHideTick;

				if( m_bTournamentBattle && m_pTRightTeam )
				{
					LPTTOURNAMENTPLAYER pMainTPlayer = m_pTRightTeam->FindPlayer( m_pMainChar->m_dwID );
					if( pMainTPlayer )
					{
						dwRightCharID = pMainTPlayer->m_dwCharID;
						strRight = pMainTPlayer->m_strName;
					}
				}
			}
			else if( LoserTeam == 1 ) //패자가 블루
			{
				dwLeftCharID = pLoser->m_dwCharID;
				strLeft = pLoser->m_strName;
				dwLeftKillCount = m_pTLeftTeam ? m_pTLeftTeam->m_wKillCount : 0;
				dwLeftHideTick = dwBlueTeamHideTick;

				if( m_bTournamentBattle && m_pTLeftTeam )
				{
					LPTTOURNAMENTPLAYER pMainTPlayer = m_pTLeftTeam->FindPlayer( m_pMainChar->m_dwID );
					if( pMainTPlayer )
					{
						dwLeftCharID = pMainTPlayer->m_dwCharID;
						strLeft = pMainTPlayer->m_strName;
					}
				}
			}
		}

		pResultDlg->SetInfo(
			dwLeftCharID,
			dwRightCharID,
			strLeft,
			strRight,
			dwLeftKillCount,
			dwRightKillCount,
			dwLeftHideTick,
			dwRightHideTick,
			bResult,
			bTrueIsLeftWinButRightWin);

		EnableUI( TFRAME_TOURNAMENT_RESULT );
		OnTournamentEnd();
	}

	if( m_vTFRAME[TFRAME_TOURNAMENT]->IsVisible() )
		m_vTFRAME[TFRAME_TOURNAMENT]->ShowComponent();

	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTAPPLYCANCEL_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult;

	(*pPacket)
		>> bResult;

	return TERR_NONE;
}

int CTClientGame::OnCS_TOURNAMENTSCHEDULE_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
#ifdef MODIFY_TT
    CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	pDLG->ReleaseSchedule();

	BYTE bCurrentStep;
	BYTE bCount;

	(*pPacket)
		>> pDLG->m_bCurrentGroup
		>> bCurrentStep
		>> bCount;

	for( BYTE i=0 ; i < bCount ; ++i )
	{
		BYTE bGroup;
		BYTE bStep;
		INT64 dlStartTime;

		(*pPacket)
			>> bGroup
			>> bStep
			>> dlStartTime;

		LPTTOURNAMENTSCHEDULE pSchedule = new TTOURNAMENTSCHEDULE;
		pSchedule->m_bStep = bStep;
		pSchedule->m_StartTime = dlStartTime;

		CTTournament::ScheduleGroup* pGroup = pDLG->FindScheduleGroup( bGroup );
		pGroup->AddSchedule( pSchedule );
	}

	CTTournament::ScheduleGroup* pCommonGroup = pDLG->FindScheduleGroup( 0 );
	if( pCommonGroup )
	{
		CTTournament::MAPSCHEDULEGROUP::iterator it, end;
		it = pDLG->m_mapGroup.begin();
		end = pDLG->m_mapGroup.end();
		for(; it != end ; ++it )
		{
			if( it->first != 0 )
			{
				it->second->AddScheduleFrom( pCommonGroup );
				it->second->SortByStep();
			}
		}
	}

	if( pDLG->m_bCurrentGroup == 0 )
	{
		if( CTTournament::IsCommonSchedule( bCurrentStep ) )
		{
			// 모든 그룹의 CurrentStep을 bCurrentStep으로 맞춘다.
			CTTournament::MAPSCHEDULEGROUP::iterator it, end;
			it = pDLG->m_mapGroup.begin();
			end = pDLG->m_mapGroup.end();
			for(; it != end ; ++it )
				it->second->m_bCurrentStep = bCurrentStep;
		}
		else
		{
			CTTournament::ScheduleGroup* pCommonGroup = pDLG->FindScheduleGroup( 0 );
			if( pCommonGroup )
				pCommonGroup->m_bCurrentStep = bCurrentStep;
		}
	}
	else
	{
		CTTournament::MAPSCHEDULEGROUP::iterator it, end;
		it = pDLG->m_mapGroup.begin();
		end = pDLG->m_mapGroup.end();
		for(; it != end ; ++it )
		{
			if( it->second->m_bGroupID < pDLG->m_bCurrentGroup )
				it->second->m_bCurrentStep = -1;
			else if( pDLG->m_bCurrentGroup < it->second->m_bGroupID )
				it->second->SetLatestStepInCommon();
		}

		pDLG->m_pCurrentGroup = pDLG->FindScheduleGroup( pDLG->m_bCurrentGroup );
		if( pDLG->m_pCurrentGroup )
			pDLG->m_pCurrentGroup->m_bCurrentStep = bCurrentStep;
	}

	pDLG->CalcScheduleTop();
	pDLG->UpdateSchedule();
	m_llPrevTSecond = 0;

	return TERR_NONE;
#else
	CTTournament* pDLG = static_cast<CTTournament*>( m_vTFRAME[ TFRAME_TOURNAMENT ] );
	pDLG->ReleaseSchedule();

	BYTE bCount;
	(*pPacket)
		>> pDLG->m_bCurrentStep
		>> bCount;

	if( bCount > TNMTSTEP_COUNT )
		bCount = TNMTSTEP_COUNT;

	for( BYTE i=0 ; i < bCount ; ++i )
	{
		LPTTOURNAMENTSCHEDULE pSchedule = new TTOURNAMENTSCHEDULE;
		INT64 dlStartTime;
		(*pPacket)
			>> pSchedule->m_bStep
			>> dlStartTime;
		pSchedule->m_StartTime = dlStartTime;
		pDLG->m_vSchedule.push_back( pSchedule );
	}

	pDLG->CalcScheduleTop();
	pDLG->UpdateSchedule();
	m_llPrevTSecond = 0;
	return TERR_NONE;
#endif
}

int CTClientGame::OnCS_TOURNAMENTSELECTWINNER_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bLeftValue;
	BYTE bRightValue;

	(*pPacket)
		>>bLeftValue
		>> bRightValue;

	if( m_pTLeftTeam && m_pTRightTeam && (m_bTournamentBattle || m_bTournamentWatching) )
	{
		CTTournamentResult* pResultDlg = static_cast<CTTournamentResult*>( m_vTFRAME[ TFRAME_TOURNAMENT_RESULT ] );

		pResultDlg->BeginSelectWinner(
			bLeftValue,
			bRightValue );

		EnableUI( TFRAME_TOURNAMENT_RESULT );
	}

	return TERR_NONE;
}
#endif

#ifdef ADD_MISSION
int CTClientGame::OnCS_ITEMLEVELREVISION_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	(*pPacket)
		>> m_bItemLevelRevision;

	if( !m_pMainChar )
		return TERR_NONE;

	if( m_bItemLevelRevision == 0 )
	{
		MAPTINVEN::iterator itINVEN, endINVEN;
		itINVEN = m_pMainChar->m_mapTINVEN.begin();
		endINVEN = m_pMainChar->m_mapTINVEN.end();
		for(; itINVEN != endINVEN ; ++itINVEN )
		{
			CTClientInven* pINVEN = itINVEN->second;
			if( pINVEN )
			{
				MAPTITEM::iterator itITEM, endITEM;
				itITEM = pINVEN->m_mapTITEM.begin();
				endITEM = pINVEN->m_mapTITEM.end();

				for(; itITEM != endITEM ; ++itITEM )
				{
					if( itITEM->second )
					{
						itITEM->second->SetInstGradeMax( 0 );
						itITEM->second->NotifyUpdate();
						itITEM->second->Update();
					}
				}
			}
		}
	}
	else
	{
		MAPTINVEN::iterator itInven = m_pMainChar->m_mapTINVEN.find(INVEN_EQUIP);
		if( itInven != m_pMainChar->m_mapTINVEN.end() )
		{
			CTClientInven* pINVEN = itInven->second;
			if( pINVEN )
			{
				MAPTITEM::iterator itITEM, endITEM;
				itITEM = pINVEN->m_mapTITEM.begin();
				endITEM = pINVEN->m_mapTITEM.end();

				for(; itITEM != endITEM ; ++itITEM )
				{
					if( itITEM->second )
					{
						itITEM->second->SetInstGradeMax( m_bItemLevelRevision );
						itITEM->second->Update();
					}
				}
			}
		}
	}

	return TERR_NONE;
}
#endif

#ifndef TEST_MODE
int CTClientGame::OnCS_HACKSHIELD_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	AHNHS_TRANS_BUFFER stRequestBuf;
	AHNHS_TRANS_BUFFER stResponseBuf;

	BYTE bServerID;
	(*pPacket)
		>> bServerID
		>> stRequestBuf.nLength;

	if(stRequestBuf.nLength > ANTICPX_TRANS_BUFFER_MAX)
		return TERR_NONE;

	pPacket->Rewind(FALSE);
	(*pPacket)
		>> bServerID;

	pPacket->DetachBinary(stRequestBuf.byBuffer);
	int nRet = _AhnHS_MakeResponse(stRequestBuf.byBuffer, stRequestBuf.nLength, &stResponseBuf);

	if(nRet == ERROR_SUCCESS)
	{
		if(stResponseBuf.nLength > ANTICPX_TRANS_BUFFER_MAX)
			return TERR_NONE;

		CPacket vMSG;
		vMSG.SetID(CS_HACKSHIELD_REQ)
			<< bServerID;

		vMSG.AttachBinary( stResponseBuf.byBuffer, stResponseBuf.nLength );
		SayToAll(vMSG);
	}

	return TERR_NONE;
}
#endif

#ifdef ADD_RPS

int CTClientGame::OnCS_RPSSTART_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bResult;
	DWORD dwGold;
	DWORD dwSilver;
	DWORD dwCooper;

	(*pPacket)
		>> bResult
		>> dwGold
		>> dwSilver
		>> dwCooper;

	if( bResult == 0 ) // success
	{
		m_pMainChar->m_dwRune = dwGold;
		m_pMainChar->m_dwLuna = dwSilver;
		m_pMainChar->m_dwCron = dwCooper;

		ResetInvenUI();

		CTRpsDlg* pDlg = static_cast<CTRpsDlg*>( m_vTFRAME[TFRAME_RPS] );
		pDlg->m_bSelectMode = TRUE;
		pDlg->m_pNPC = m_pTARGET;
	
		m_bRPSWinCount = 0;
		m_bRPSKeyEnable = TRUE;
		m_bRPSGaming = TRUE;

		EnableUI(TFRAME_RPS);
	}

	return TERR_NONE;
}

int CTClientGame::OnCS_RPSGAME_ACK( CTachyonSession *pSession, CPacket *pPacket )
{
	BYTE bWinCount; //0:패배
	BYTE bPlayerRPS;
	BYTE bNpcRPS;

	(*pPacket)
		>> bWinCount
		>> bPlayerRPS
		>> bNpcRPS;

	CTRpsDlg* pDLG = static_cast<CTRpsDlg*>( m_vTFRAME[ TFRAME_RPS ] );
	pDLG->m_bSelectMode = FALSE;
	pDLG->m_ePlayer = (RPSCARD_TYPE) bPlayerRPS;
	pDLG->m_eNpc = (RPSCARD_TYPE) bNpcRPS;
	pDLG->m_pNPC = m_pTARGET;

	if( bPlayerRPS == bNpcRPS )
	{
		// 비겼다.
		m_pMainWnd->MessageBoxOK(
			TSTR_RPS_MSG_DRAW,
			TSTR_OK,
			GM_RPS_TRY,
			0,
			FALSE);
	}
	else if( bWinCount == 0 )
	{
		if( m_bRPSWinCount >= 5 )
		{
			// 패배공지
			CString strFMT;
			strFMT = CTChart::Format( TSTR_RPS_NOTICE_LOSE,
				m_pMainChar->m_strNAME,
				m_bRPSWinCount+1);

			CString strText = CTTextLinker::GetInstance()->BuildNetString(CString(), strFMT);

			RelaySession()->SendCS_CHAT_REQ(
				m_pMainChar->m_strNAME,
				CHAT_SHOW,
				strText.GetLength(), NULL,
				strText,
				m_pMainChar->m_bContryID,
				m_mapPLAYER);
		}

		// 졌다.
		m_pMainWnd->MessageBoxYesNo(
			TSTR_RPS_MSG_RETRY,
			TSTR_YES,
			TSTR_NO,
			TCOMMAND( GM_RPS_START, m_dwRPSGameType ),
			GM_RPS_END,
			GM_RPS_END,
			FALSE );

		m_bRPSGaming = FALSE;
	}
	else
	{
		// 이겼다.
		// bWinCount는 연속승수
		CTRpsRewardDlg* pDLG = static_cast<CTRpsRewardDlg*>( m_vTFRAME[ TFRAME_RPS_REWARD ] );
		pDLG->SetReward( (BYTE) m_dwRPSGameType, bWinCount );
		EnableUI( TFRAME_RPS_REWARD );

		if( bWinCount >= 5 )
		{
			//승리 공지
			CString strFMT;
			strFMT = CTChart::Format(TSTR_RPS_NOTICE_WIN,
				m_pMainChar->m_strNAME,
				bWinCount );

			CString strText = CTTextLinker::GetInstance()->BuildNetString(CString(), strFMT);

			RelaySession()->SendCS_CHAT_REQ(
				m_pMainChar->m_strNAME,
				CHAT_SHOW,
				strText.GetLength(), NULL,
				strText,
				m_pMainChar->m_bContryID,
				m_mapPLAYER);
		}
	}

	m_bRPSKeyEnable = FALSE;
	m_bRPSWinCount = bWinCount;

	return TERR_NONE;
}

#endif