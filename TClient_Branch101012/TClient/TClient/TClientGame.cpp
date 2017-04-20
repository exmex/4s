#include "stdafx.h"
#include "Resource.h"
#include "TClientWnd.h"
#include "TItemUpDlg.h"
#include "TItemUpGaugeDlg.h"
#include "TItemRefineDlg.h"
#include "TItemRepairDlg.h"
#include "TCommunityDlg.h"
#include "TPrivateShopDlg.h"
#include "TMailListFrame.h"
#include "TMailSendDlg.h"
#include "TMessengerChat.h"
#include "TMessengerBase.h"
#include "TMsgChatMemberList.h"
#include "TPetMakeDlg.h"
#include "TPetManageDlg.h"
#include "TMsgGroupList.h"
#include "TClient.h"
#include "PacketSpyDlg.h"
#include "TCandidateFrame.h"
#include "MD5.h"
#include "TItemGambleDlg.h"
#include "TMiniDump.h"
#include "TRentSkillDlg.h"
#include "TGuildPvPRecordFrame.h"
#include "TGuildPointRewardFrame.h"
#include "TUpItemSellDlg.h"
#include "TItemUpCashDlg.h"
#include "TPortalShopDlgNew.h"
#include "TGuildRegularOffer.h"
#include "TGuildTacticsOffer.h"
#include "TTacticsInviteDlg.h"
#include "TColorPicker.h"
#include "TFameRank.h"
#include "TFameBaseDlg.h"
#include "TSettingFrame.h"
#include "TTournament.h"
#include "TTournamentEvent.h"
#include "TTournamentGage.h"
#include "TTournamentSelectTarget.h"
#include "TTournamentMode.h"
#include "TTournamentResult.h"
#include "TRpcDlg.h"
#include "TWebCashShopDlg.h"

struct TOBJ_LENGTH_SORTER
{
	D3DXVECTOR3 m_vPOSCHAR;

	bool operator () ( CTClientObjBase* _Left, CTClientObjBase* _Right)
	{
		return D3DXVec3LengthSq( &(m_vPOSCHAR - _Left->GetPosition()))
			< D3DXVec3LengthSq( &(m_vPOSCHAR - _Right->GetPosition())) ;
	};
};

LPTEXTURESET CTClientGame::m_pTGROUNDTEX = NULL;
CT3DVertex CTClientGame::m_vTRECTVB;
BYTE CTClientGame::m_bTOPTIONLEVEL = TOPTIONLEVEL_HI;

LPTEXTURESET CTClientGame::m_pTCOMMANDERICON = NULL;
LPTEXTURESET CTClientGame::m_pTCHIEFICON = NULL;
LPTEXTURESET CTClientGame::m_pTSELECTMARK = NULL;
LPTEXTURESET CTClientGame::m_pTSOULMATE = NULL;

LPTEXTURESET CTClientGame::m_vTNPCICON[TNPCICON_COUNT];
TOPTION CTClientGame::m_vTOPTION;

BOOL CTClientGame::m_bNeedCharDetInfoInit = TRUE;
BYTE CTClientGame::m_bSendedCS_CASHITEMPUTIN_REQ = FALSE;

BYTE CTClientGame::m_vTNPCICONIDX[TNPC_COUNT] = {
	TNPCICON_NONE,		// TNPC_NONE
	TNPCICON_SKILL,		// TNPC_SKILL_MASTER
	TNPCICON_ITEM,		// TNPC_ITEM
	TNPCICON_REVIVAL,	// TNPC_REVIVAL
	TNPCICON_NONE,		// TNPC_MAKE
	TNPCICON_ITEMUP,	// TNPC_UPGRADE
	TNPCICON_NONE,		// TNPC_MAPPORTAL
	TNPCICON_NONE,		// TNPC_BOX
	TNPCICON_PORTAL,	// TNPC_PORTAL
	TNPCICON_BANK,		// TNPC_WAREHOUSE
	TNPCICON_AUCTION,	// TNPC_AUCTION
	TNPCICON_GUILD,		// TNPC_GUILD
	TNPCICON_NONE,		// TNPC_TOURNAMENT_GATE_KEEPER
	TNPCICON_CASTLE,	// TNPC_CASTLE
	TNPCICON_PORTAL,	// TNPC_RETURN
	TNPCICON_ITEM,		// TNPC_DIVINER
	TNPCICON_ITEM,		// TNPC_GAMBLE
	TNPCICON_ITEMUP,	// TNPC_REPAIR
	TNPCICON_ITEMUP,	// TNPC_REFINE
	TNPCICON_SKILL,		// TNPC_SKILL_RENT
	TNPCICON_ITEM,		// TNPC_CRAFT
	TNPCICON_ITEM,		// TNPC_PVPOINT
	TNPCICON_NONE		// TNPC_MONSTER
};

DWORD CTClientGame::m_vTICONGAUGE[TINVENSLOT_COUNT] = {
	ID_CTRLINST_ICON_GAUGE_1,
	ID_CTRLINST_ICON_GAUGE_2,
	ID_CTRLINST_ICON_GAUGE_3,
	ID_CTRLINST_ICON_GAUGE_4,
	ID_CTRLINST_ICON_GAUGE_5,
	ID_CTRLINST_ICON_GAUGE_6,
	ID_CTRLINST_ICON_GAUGE_7,
	ID_CTRLINST_ICON_GAUGE_8,
	ID_CTRLINST_ICON_GAUGE_9,
	ID_CTRLINST_ICON_GAUGE_10,
	ID_CTRLINST_ICON_GAUGE_11,
	ID_CTRLINST_ICON_GAUGE_12,
	ID_CTRLINST_ICON_GAUGE_13,
	ID_CTRLINST_ICON_GAUGE_14,
	ID_CTRLINST_ICON_GAUGE_15,
	ID_CTRLINST_ICON_GAUGE_16,
	ID_CTRLINST_ICON_GAUGE_17,
	ID_CTRLINST_ICON_GAUGE_18,
	ID_CTRLINST_ICON_GAUGE_19
	
};

DWORD CTClientGame::m_vTICONSM[TSKILLSLOT_COUNT] = {
	ID_CTRLINST_ICON_SEL_1,
	ID_CTRLINST_ICON_SEL_2,
	ID_CTRLINST_ICON_SEL_3,
	ID_CTRLINST_ICON_SEL_4,
	ID_CTRLINST_ICON_SEL_5,
	ID_CTRLINST_ICON_SEL_6,
	ID_CTRLINST_ICON_SEL_7,
	ID_CTRLINST_ICON_SEL_8,
	ID_CTRLINST_ICON_SEL_9,
	ID_CTRLINST_ICON_SEL_10,
	ID_CTRLINST_ICON_SEL_11,
	ID_CTRLINST_ICON_SEL_12,
	ID_CTRLINST_ICON_SEL_13,
	ID_CTRLINST_ICON_SEL_14};

WORD CTClientGame::m_vSlotID[MAX_DRAGSLOT] = {
	ID_CTRLINST_SLOT_1,
	ID_CTRLINST_SLOT_2,
	ID_CTRLINST_SLOT_3,
	ID_CTRLINST_SLOT_4,
	ID_CTRLINST_SLOT_5,
	ID_CTRLINST_SLOT_6,
	ID_CTRLINST_SLOT_7,
	ID_CTRLINST_SLOT_8,
	ID_CTRLINST_SLOT_9,
	ID_CTRLINST_SLOT_10,
	ID_CTRLINST_SLOT_11,
	ID_CTRLINST_SLOT_12,
	ID_CTRLINST_SLOT_13,
	ID_CTRLINST_SLOT_14,
	ID_CTRLINST_SLOT_15,
	ID_CTRLINST_SLOT_16,
	ID_CTRLINST_SLOT_17,
	ID_CTRLINST_SLOT_18,
	ID_CTRLINST_SLOT_19
	
};


DWORD CTClientGame::m_vTCOUNTRYSTR[TCONTRY_COUNT] = {
	TSTR_CONTRY_D,
	TSTR_CONTRY_C,
	TSTR_CONTRY_B};

DWORD CTClientGame::m_vTCLASSSTR[TCLASS_COUNT] = {
	TSTR_TCLASS_WARRIOR,
	TSTR_TCLASS_RANGER,
	TSTR_TCLASS_ARCHER,
	TSTR_TCLASS_WIZARD,
	TSTR_TCLASS_PRIEST,
	TSTR_TCLASS_SORCERER};

DWORD CTClientGame::m_vTRACESTR[TRACE_COUNT] = {
	TSTR_RACE_HUMAN,
	TSTR_RACE_BEAST,
	TSTR_RACE_NYMPH};

DWORD CTClientGame::m_dwReleaseMaintainFunctionWhenGameOut[4] =
{
	SDT_STATUS_CLARITY,
	SDT_STATUS_HIDE,
	SDT_STATUS_STANDHIDE,
	SDT_STATUS_NOTSELECT
};

struct binary_draw : public binary_function< CTClientObjBase *, CTClientObjBase *, bool>
{
	bool operator () ( const CTClientObjBase * &_Left, const CTClientObjBase * &_Right) const
	{
		if( _Left->m_bLastDraw == _Right->m_bLastDraw )
		{
			if( _Left->m_OBJ.m_pOBJ->m_bFileID > _Right->m_OBJ.m_pOBJ->m_bFileID )
				return true;
			else if( _Left->m_OBJ.m_pOBJ->m_bFileID < _Right->m_OBJ.m_pOBJ->m_bFileID )
				return false;

			return _Left->m_fZValue > _Right->m_fZValue ? true : false;
		}
		else
			return _Left->m_bLastDraw ? false : true;
	};
};

struct binary_pick : public binary_function< CTClientObjBase *, CTClientObjBase *, bool>
{
	bool operator () ( const CTClientObjBase * &_Left, const CTClientObjBase * &_Right) const
	{
		return _Left->m_fPickWeight > _Right->m_fPickWeight ? true : false;
	};
};

CTChatFrame* CTClientGame::m_pChatFrame = NULL;
CTClientGame* CTClientGame::m_pINSTANCE = NULL;
CTClientGame* CTClientGame::GetInstance() { return m_pINSTANCE;}

CString CTClientGame::GetMD5String( CString& strSOURCE )
{
	MD5 context;
	context.update( (unsigned char*) strSOURCE.GetBuffer(), strSOURCE.GetLength() );
	context.finalize();

	char* pstr = context.hex_digest();
	CString strRESULT( pstr );
	delete [] pstr;
	
	return strRESULT;
}

CTClientGame::CTClientGame()
{
	m_pINSTANCE = this;
	srand(time(NULL));
	m_pFUZZY = new BYTE[rand() % 1024];
	m_pMainChar = new CTClientChar();

	for( BYTE i=0; i<TPANNEL_COUNT; i++)
	{
		m_vTMAINTAIN[i] = NULL;
		m_vTPANNEL[i] = NULL;
	}

	for( i=0; i<TLEADERMARK_COUNT; i++)
		m_vTLEADERMARK[i] = NULL;

	for( i=0; i<TFACEIMG_COUNT; i++)
		m_vTFACEIMG[i] = NULL;

	for( i=0; i<TLEVELIMG_COUNT; i++)
		m_vTLEVELIMG[i] = NULL;

	for( i=0; i<TFACEBACKIMG_COUNT; i++)
		m_vTFACEBACKIMG[i] = NULL;

	for( i=0; i<TTEXT_COUNT; i++)
		m_vTTEXT[i] = NULL;

	for( i=0; i<TGAUGE_COUNT; i++)
		m_vTGAUGE[i] = NULL;

	for( i=0; i<TFRAME_COUNT; i++)
		m_vTFRAME[i] = NULL;

	m_pTPARTYITEM = NULL;
	m_pChatFrame = NULL;	
	m_pNotifyFrame = NULL;
	m_pMessengerBase = NULL;
	m_pMailList = NULL;
	m_pActListPopup = NULL;

	m_pSystemSetting = NULL;
	m_pGameSetting = NULL;

	m_pTCURFRAME = NULL;
	m_pTPOPUP = NULL;
	m_pChannelPOPUP = NULL;

	m_mapSLAVEFIXRECALL.clear();
	m_mapSLAVERECALL.clear();
	m_mapSLAVEMON.clear();

	m_mapFAKETEMP.clear();
	m_mapFIXRECALL.clear();
	m_mapMONSTER.clear();
	m_mapTSwitch.clear();
	m_mapPLAYER.clear();
	m_mapRECALL.clear();

	for( int i=0 ; i < FRT_COUNT ; ++i )
		m_mapTFAMERANK[ i ].clear();
	
	m_vTMAINTAINOBJ.clear();
	m_vTNUMBER.clear();
	m_vTARROW.clear();
	m_vTPARTY.clear();

	m_pSESSION = NULL;
	m_pRelay = NULL;
	m_pDevice = NULL;
	m_pCamera = NULL;

	m_pTMinimap = NULL;
	m_pCtrlHost = NULL;
	m_pMainWnd = NULL;
	m_pTRSCS = NULL;
	m_pRES = NULL;
	m_pDurationUI = NULL;

	m_pTPOINTER = NULL;
	m_pTPARTY = NULL;
	m_pTARGET = NULL;
	m_pACT = NULL;
	m_pHIT = NULL;
	m_nDEADMONHITIDX = 0;

	m_bDragSND = FALSE;
	m_bAutoRun = FALSE;
	m_bRevival = FALSE;
	m_bCamFIX = FALSE;

	m_bShowDebugInfo = FALSE;
	m_bShowFPSInfo = FALSE;
	m_bShowUI = TRUE;
	
	m_nCamZOOM = 0;
	m_wCamDIR = 0;
	m_bObtainType = PT_ORDER;

	m_dwActiveVolume = TBGM_VOLUME;
	m_bStopBGM = FALSE;
	m_dwBGM = ID_BGM_INTRO;
	m_dwENV = 0;

	m_bMoveType = TMAINMOVE_NORMAL;
	m_dwMoveGM = 0;
	m_fBOUND = 0.0f;

	m_vDRAG.m_bFrameID = TFRAME_COUNT;
	m_vDRAG.m_bSlotID = 0;
	m_vDRAG.m_pIMAGE = NULL;
	m_vDRAG.m_dwParam = 0;
	
	m_bLButtonDown = FALSE;
	m_bRButtonDown = FALSE;
	m_bLOOPCHARGE = FALSE;
	m_bDrawGROUND = FALSE;
	m_bGroundLOCK = FALSE;
	m_bGroundATK = FALSE;
	m_bRSCSMENU = FALSE;
	m_bLoopLock = FALSE;
	m_bTELEPORT = FALSE;
	m_bCHARGE = FALSE;
	m_eCHARGETYPE = TCHARGETYPE_UNKNOWN;
	m_bGHOST = FALSE;
	m_bLAND = FALSE;
	m_bRSCS = FALSE;

	m_dwFogCOLOR = 0;
	m_dwTSKY = 0;
	m_wTSKY = 0;
	m_bTSKY = 0;

	m_dwMaxChargeTick = 0;
	m_dwChargeTick = 0;
	m_dwLeftTickEXIT = 0;

	m_dwLButtonTick = 0;
	m_dwRButtonTick = 0;

	m_dwGHOSTTick = TGHOST_TICK;
	m_dwFLASHTick = 0;
	m_dwSYNCTick = 0;
	m_dwPOSTick = 0;
	m_dwTSELECT = 0;
	m_dwTSHADOW = 0;
	m_dwTGROUND = 0;

	m_dwDHELPTICK = 0;
	m_dwDHELP = 0;

	m_bMonSlotID = 0;
	m_bInvenID = 0;
	m_bSlotID = 0;

	m_bTargetInvenID = INVEN_NULL;
	m_bTargetSlotID = INVEN_NULL;
	m_bSkillInitInven = INVEN_NULL;
	m_bSkillInitSlot = INVEN_NULL;

	m_bPortalItemInven = INVEN_NULL;
	m_bPortalItemSlot = INVEN_NULL;

	m_wNextSkillID = 0;
	m_wSkillID = 0;

	m_bNeedDrag = FALSE;
	m_bDrawOBJ = TRUE;

	m_bDoChangeCH = FALSE;
	m_dwLeftTickCH = 0;
	m_bChannelID = 0;
	m_bDoSelectCHAR = FALSE;
	m_dwLeftTickCHAR = 0;
	m_bDoAutoRevival = FALSE;
	m_dwLeftTickAR = 0;
	m_llPrevTSecond=0;
	m_bDoEXIT = FALSE;
	m_bDoKICKOUTMAP = FALSE;
	m_dwLeftTickKICKOUTMAP = 0;
	m_wKICKOUTMAP = 0;
	
	m_bTournamentBattle = FALSE;
	m_bTournamentWatching = FALSE;
		
	m_bDuel = FALSE;
	m_pDuelTarget = NULL;
	m_dwDuelTick = 0;
	m_dwDuelOutTick = (DWORD)T_INVALID;
	m_dwDuelWaitTick = 0;
	m_strDuelTargetName.Empty();
	m_dwDuelTargetID = 0;

	m_dwSoulRegion = 0;
	m_dwSoulID = 0;
	m_bInvenDraging = FALSE;
	m_pTOPFRAME = NULL;

	ZeroMemory( &m_vOBJMTRL, sizeof(D3DMATERIAL9));
	ZeroMemory( &m_vNormal, sizeof(D3DMATERIAL9));
	ZeroMemory( &m_vShadow, sizeof(D3DMATERIAL9));

	m_vOBJMTRL.Ambient.a = 1.0f;
	m_vOBJMTRL.Ambient.r = 0.5f;
	m_vOBJMTRL.Ambient.g = 0.5f;
	m_vOBJMTRL.Ambient.b = 0.5f;
	m_vOBJMTRL.Diffuse.a = 1.0f;
	m_vOBJMTRL.Diffuse.r = 1.0f;
	m_vOBJMTRL.Diffuse.g = 1.0f;
	m_vOBJMTRL.Diffuse.b = 1.0f;

	m_vNormal.Ambient.a = 1.0f;
	m_vNormal.Ambient.r = 1.0f;
	m_vNormal.Ambient.g = 1.0f;
	m_vNormal.Ambient.b = 1.0f;
	m_vNormal.Diffuse.a = 1.0f;
	m_vNormal.Diffuse.r = 1.0f;
	m_vNormal.Diffuse.g = 1.0f;
	m_vNormal.Diffuse.b = 1.0f;

	m_vShadow.Ambient.a = 1.0f;
	m_vShadow.Ambient.r = 0.7f;
	m_vShadow.Ambient.g = 0.7f;
	m_vShadow.Ambient.b = 0.7f;
	m_vShadow.Diffuse.a = 1.0f;
	m_vShadow.Diffuse.r = 0.7f;
	m_vShadow.Diffuse.g = 0.7f;
	m_vShadow.Diffuse.b = 0.7f;

	m_bFirstConnect = FALSE;
	EnableAllInputDevice();

	m_vFARIMAGE[!m_bTSKY].m_dwSHADOWTick = TMAXSHADOW_TICK;
	m_vFARIMAGE[m_bTSKY].m_dwSHADOWTick = TMAXSHADOW_TICK;

	m_vFARIMAGE[!m_bTSKY].m_bZWrite = FALSE;
	m_vFARIMAGE[m_bTSKY].m_bZWrite = FALSE;

	m_vSKY[!m_bTSKY].m_dwSHADOWTick = TMAXSHADOW_TICK;
	m_vSKY[m_bTSKY].m_dwSHADOWTick = TMAXSHADOW_TICK;

	m_vSKY[!m_bTSKY].m_bZWrite = FALSE;
	m_vSKY[m_bTSKY].m_bZWrite = FALSE;
	m_vMAP.ReleaseMAP();
	ClearMonInven();
	EndQUAKE();

#if defined(TEST_MODE) || defined(DEBUG)
	ReleaseTSPAWN();

	m_bEditTSPAWN = FALSE;
	m_bMonRoaming = TRUE;
#endif

	m_dwCmdParam = 0;
	m_bOrgTKEY_ROT = 0;
	m_bPetUseByHotbar = FALSE;

	m_dwNameTick = 0;
	m_ptPrevPos = CPoint(-1,-1);

	m_pCandidate = NULL;
	m_pOpenShopByCash = NULL;
	m_dwRelayTick = 0;

	m_bLockGM_CASHCAB_GETITEM = FALSE;
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	CTClientObjBase::InitADJACENCY();
	CTMiniDump::SetSocket(INVALID_SOCKET);

#ifdef ADD_SNOW
	m_dwSnowEventEffect = 0;
	m_bSnowEvent = FALSE;
	m_bSnowEventUserOn = TRUE;
#endif
	m_bResetMAP = TRUE;

	m_pTLeftTeam = NULL;
	m_pTRightTeam = NULL;

	m_bAlreadyRecvSiegeAlarm = FALSE;
	m_bTournamentGateOpened = FALSE;
	m_bEnableNPCTick = TRUE;

	m_bItemLevelRevision = 0;
	m_dwWaitingRevivalItem = 0;
	m_bShowingRevivalItemMessageBox = FALSE;

#ifdef ADD_RPS
	m_dwRPSGameType = 0;
	m_bRPSWinCount = 0;
	m_bRPSKeyEnable = FALSE;
	m_bRPSGaming = FALSE;
#endif

	m_bSendedCS_CASHITEMPUTIN_REQ = FALSE;
	m_bMainCharMoveLock = false;
	m_dwRemainTickVerifyMaintain = 0;
}

CTClientGame::~CTClientGame()
{
	ReleaseGame();

	if(m_pFUZZY)
	{
		delete[] m_pFUZZY;
		m_pFUZZY = NULL;
	}

	if(m_pMainChar)
	{
		delete m_pMainChar;
		m_pMainChar = NULL;
	}
}

VOID CTClientGame::SplitMoney( INT64 dwPrice, DWORD *pbRune, DWORD *pbLuna, DWORD *pbCron)
{
	DWORD dwRune = DWORD( dwPrice / TRUNE_DIVIDER );
	DWORD dwRuLf = DWORD( dwPrice % TRUNE_DIVIDER );
	DWORD dwLuna = dwRuLf / TLUNA_DIVIDER;
	DWORD dwCron = dwRuLf % TLUNA_DIVIDER;

	if(pbRune)
		*pbRune = dwRune;
	if(pbLuna)
		*pbLuna = dwLuna;
	if(pbCron)
		*pbCron = dwCron;
}

VOID CTClientGame::AddUpMoney( INT64* pPrice, DWORD dwRune, DWORD dwLuna, DWORD dwCron)
{
	if( pPrice )
	{
		dwLuna = min( dwLuna, 999 );
		dwCron = min( dwCron, 999 );

		*pPrice = dwRune * TRUNE_DIVIDER;
		*pPrice += dwLuna * TLUNA_DIVIDER;
		*pPrice += dwCron;
	}
}

CString CTClientGame::MakeMoneyStr(INT64 dwPrice)
{
	DWORD dwRune = 0;
	DWORD dwLuna = 0;
	DWORD dwCron = 0;

	SplitMoney( dwPrice, &dwRune, &dwLuna, &dwCron );
	return MakeMoneyStr(dwRune, dwLuna, dwCron);
}

CString CTClientGame::MakeMoneyStr(DWORD dwRune, DWORD dwLuna, DWORD dwCron)
{
	CString strPrice;
	CString strTemp;

	if( dwRune )
	{
		strTemp = CTChart::Format( TSTR_FMT_RUNE, dwRune);
		strPrice += strTemp;
	}

	if( dwLuna )
	{
		if( dwRune )
			strPrice += " ";

		strTemp = CTChart::Format( TSTR_FMT_LUNA, dwLuna);
		strPrice += strTemp;
	}

	if( dwCron || (!dwRune && !dwLuna) )
	{
		if( dwLuna )
			strPrice += " ";

		strTemp = CTChart::Format( TSTR_FMT_CRON, dwCron);
		strPrice += strTemp;
	}

	return strPrice;
}

void CTClientGame::CalcSCENE( DWORD dwTick)
{
	D3DXVECTOR3 vPOS = m_pMainChar->GetPosition();

	m_dwSYNCTick += dwTick;
	m_dwPOSTick += dwTick;

	m_vMAP.CalcMAPOBJ(
		m_pDevice,
		m_pCamera,
		dwTick);
	CalcTSWITCH(dwTick);

	switch(m_bMoveType)
	{
	case TMAINMOVE_GROUND	:
	case TMAINMOVE_TARGET	: CalcTargetMove(dwTick); break;
	case TMAINMOVE_CLICK	: CalcClickMove(dwTick); break;
	case TMAINMOVE_FOLLOW	: CalcFollowMove(dwTick); break;
	default					: CalcMainChar(dwTick); break;
	}

	CalcReuseTick(dwTick);
	CalcCHARGE(dwTick);

	m_pMainChar->CalcMaintain(
		dwTick,
		TRUE,
		this);

	m_dwRemainTickVerifyMaintain -= min( dwTick, m_dwRemainTickVerifyMaintain );
	if( m_dwRemainTickVerifyMaintain == 0 )
	{
		if( !m_pMainChar->VerifySkillFunc() && GetSession() )
			GetSession()->SendCS_ACDCLOSE_REQ();
		m_dwRemainTickVerifyMaintain = 5000;//TINTERVAL_VERIFY_MAINTAIN;
	}

	if( m_pCtrlHost == m_pMainChar ||
		((CTClientRecall *) m_pCtrlHost)->m_bRecallType != TRECALLTYPE_PET ||
		((CTClientPet *) m_pCtrlHost)->GetTakeUpChar() != m_pMainChar )
	{
		m_pMainChar->CalcTick(
			&vPOS,
			m_pDevice,
			m_pRES,
			dwTick);

		D3DXVECTOR3 vTNEXT = m_pMainChar->GetPosition();
		FLOAT fNextH = vPOS.y;

		m_pMainChar->ForecastHeight(
			&m_vMAP,
			&fNextH,
			NULL,
			dwTick);

		CheckTPORTAL(
			m_pMainChar,
			&vPOS,
			&vTNEXT,
			fNextH);

		BYTE bBLOCK = m_vMAP.CheckCollision(
			m_pMainChar,
			&vTNEXT,
			&vPOS,
			fNextH);

		FLOAT fMove = D3DXVec2Length(&D3DXVECTOR2(
			vTNEXT.x - vPOS.x,
			vTNEXT.z - vPOS.z));

		if( m_vMAP.GetHeight( m_pMainChar, NULL, &vTNEXT, fMove, FALSE) > vPOS.y + fMove / CTClientMAP::m_fMoveBound )
		{
			vTNEXT = vPOS;
			bBLOCK = TRUE;
		}

		if(bBLOCK)
		{
			m_pMainChar->m_vWorld._41 += vTNEXT.x - m_pMainChar->GetPositionX();
			m_pMainChar->m_vWorld._43 += vTNEXT.z - m_pMainChar->GetPositionZ();

			m_pMainChar->CalcTick(
				&vPOS,
				m_pDevice,
				m_pRES, 0);

			if( !m_pMainChar->m_bGhost && !m_pMainChar->m_bBlock )
			{
				m_pMainChar->m_bBlock = TRUE;

				SendCS_BLOCK_REQ(
					m_pMainChar->m_dwID,
					m_pMainChar->m_bType,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					m_pMainChar->GetPositionX(),
					m_pMainChar->GetPositionY(),
					m_pMainChar->GetPositionZ(),
					m_pMainChar->m_wPITCH,
					m_pMainChar->m_wDIR,
					m_pMainChar->m_bAction,
					m_pMainChar->m_bBlock);

				if(GetPartyID(m_pMainChar))
					m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
			}
		}
		else if( !m_pMainChar->m_bGhost && m_pMainChar->m_bBlock )
		{
			m_pMainChar->m_bBlock = FALSE;

			SendCS_BLOCK_REQ(
				m_pMainChar->m_dwID,
				m_pMainChar->m_bType,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID,
				m_pMainChar->GetPositionX(),
				m_pMainChar->GetPositionY(),
				m_pMainChar->GetPositionZ(),
				m_pMainChar->m_wPITCH,
				m_pMainChar->m_wDIR,
				m_pMainChar->m_bAction,
				m_pMainChar->m_bBlock);

			if(GetPartyID(m_pMainChar))
				m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
		}
	}
	else
	{
		if( m_pCtrlHost->m_bType == OT_RECALL )
			CalcRECALL( (CTClientRecall*) m_pCtrlHost, dwTick);

		m_pMainChar->SetPosition(m_pCtrlHost->GetPositionMat());
		m_pMainChar->m_vWorld = m_pCtrlHost->m_vWorld;
	}

	LPTREGIONINFO pTREGION = m_pMainChar->m_pTREGION;
	m_pMainChar->CalcHeight(
		&vPOS,
		&m_vMAP,
		dwTick);

	CalcTOBJDamage(
		m_pMainChar,
		dwTick);
	CalcBREATH(dwTick);

	if(m_pMainChar->m_dwDropDamage &&
		(m_pMainChar->m_pTREGION ? m_pMainChar->m_pTREGION->m_bContryID != TCONTRY_PEACE : TRUE) )
	{
		SendCS_DROPDAMAGE_REQ(
			m_pMainChar->m_dwID,
			m_pMainChar->m_bType,
			m_pMainChar->m_dwDropDamage,
			m_pMainWnd->m_bChannel,
			m_vMAP.m_wMapID);

		m_pMainChar->m_dwDropDamage = 0;
	}

	if( m_pMainChar->m_pTREGION != pTREGION )
		OnRegionChanged();

	if( !m_pMainChar->m_bActionLock && m_wNextSkillID )
	{
		UseTSkill( m_wNextSkillID, FALSE, TRUE);
		m_wNextSkillID = 0;
	}

	if( m_pMainChar->GetRidingPet() != NULL )
	{
		D3DXMatrixIdentity( &(m_pMainChar->m_vWorld) );
		m_pMainChar->SetPosition( m_pMainChar->m_vWorld );
	}

	CalcMONSTER(dwTick);
	CalcRECALL(dwTick);
	CalcPLAYER(dwTick);

	CalcSLAVERECALL(dwTick);
	CalcSLAVEMON(dwTick);
	CalcARROW(dwTick);

	ReportTRSCSMOVE(m_pMainChar);
	for( int i=0; i<INT(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
		CalcTSQUADRSCS( m_pTRSCS->m_vTCORPS.m_vTSQUAD[i], dwTick);

	CalcTRSCS(dwTick);
	CalcMaintainOBJ(dwTick);

	if(m_pMainChar->InCastle())
		SiegeMgr->CalcTick( m_pDevice, dwTick);

	CalcTournamentChar(dwTick);
}

HRESULT CTClientGame::Render( DWORD dwTickCount)
{
	CTClientObjBase::ClearGARBAGE();

	if( m_pTARGET && m_pTARGET->m_bGARBAGE )
	{
		ResetTargetOBJ(NULL);
		ResetTargetINFO(FALSE);
	}

	m_pTOPFRAME = NULL;
	ClearDrawCount();

	FLOAT fWaterHeight;
	BYTE bIsEXTVisible;
	HRESULT hr = S_OK;

	m_vTSCENE_PLAYER.CalcTick( this, dwTickCount);
	CalcNoneInputLimitation(dwTickCount);
	CalcChangeChannel(dwTickCount);
	CalcSelectCharacter(dwTickCount);
	CalcExitGame(dwTickCount);
	CalcKickoutMap(dwTickCount);
	CalcMainRunAway(dwTickCount);
	CalcTournament(dwTickCount);
	CalcFirecracker(dwTickCount);
	CalcWaitingRevivalItem(dwTickCount);

	DWORD dwPCCount = DWORD(m_mapPLAYER.size());
	DWORD dwTotal = dwTickCount;

	if( !(m_pDevice->m_vCAPS.DevCaps&D3DDEVCAPS_HWTRANSFORMANDLIGHT) || dwPCCount > TLOWLEVEL_COUNT )
	{
		CTDynamicBillboard::m_bLevel = TDBB_LEVEL_LOW;
		m_bTOPTIONLEVEL = TOPTIONLEVEL_LOW;
	}
	else if( dwPCCount > TMEDLEVEL_COUNT )
	{
		CTDynamicBillboard::m_bLevel = TDBB_LEVEL_MID;
		m_bTOPTIONLEVEL = TOPTIONLEVEL_MED;
	}
	else
	{
		CTDynamicBillboard::m_bLevel = TDBB_LEVEL_HI;
		m_bTOPTIONLEVEL = TOPTIONLEVEL_HI;
	}
	m_pCtrlHost = GetCtrlOBJ();

	if( m_vKEY.m_vSTATE[TKEY_ROT] && m_vKEY.m_vTICK[TKEY_ROT] + CLICK_DELAY < m_vKEY.m_dwTick )
		m_pCtrlHost->m_nPITCH = (INT(m_pCamera->m_wCamPITCH) - INT(m_pCtrlHost->m_wPITCH) - 150) % 1800;

	((CTMainUI *) m_vTFRAME[TFRAME_MAIN])->CheckMSGCTRL();
	CheckHUD();

	m_vMAP.m_bHouseMesh = m_pCtrlHost->m_bHouseMesh;
	m_vMAP.m_dwHouseID = m_pCtrlHost->m_dwHouseID;

	m_vMAP.ResetVisibleOBJ(
		m_pDevice,
		m_pCamera,
		dwTickCount);

	while(dwTotal)
	{
		DWORD dwTick = min( dwTotal, TTICK_INTERVAL);

		CalcSCENE(dwTick);
		dwTotal -= dwTick;
	}

	if(!m_vTPOINTER.IsDead())
	{
		m_vTPOINTER.CalcTick(
			m_pDevice->m_pDevice,
			dwTickCount);
	}

	CTachyonSFX::CalcBUF(
		m_pDevice->m_pDevice,
		dwTickCount);
	
	fWaterHeight = CalcCamera(dwTickCount);
	ResetVisibleOBJ(dwTickCount);

	CalcDynamicHelp(dwTickCount);
	CalcTQuestTick(dwTickCount);
	CalcGlobalTEX(dwTickCount);
	CalcFLASHTick(dwTickCount);
	CalcBGM(dwTickCount);
	CalcDUEL(dwTickCount);

	CheckRelay(dwTickCount);

	m_pTRSCS->CalcTick(dwTickCount);
	m_pChatFrame->Update(dwTickCount);

	UpdateItemUpGauge(dwTickCount);
	UpdateProgressGauge(dwTickCount);

	CalcTSKY( fWaterHeight, dwTickCount);

	if(m_pMainChar->m_bGhost)
		CalcGhost(dwTickCount);
	
	CalcGHOSTScene(dwTickCount);
	CalcNUMBER(dwTickCount);

	if(CheckGHOSTScene())
		BeginGHOSTScene();

	m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFFFFFFFF);
	DisableLIGHT();
	ResetSCENEConstant();

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	m_vSKY[m_bTSKY].Render( m_pDevice, m_pCamera);
	m_vFARIMAGE[m_bTSKY].Render( m_pDevice, m_pCamera);
	if(!m_vSKY[!m_bTSKY].m_bHide)
	{
		SetMaterial(&m_vSKY[!m_bTSKY]);
		m_vSKY[!m_bTSKY].Render( m_pDevice, m_pCamera);
		m_vFARIMAGE[!m_bTSKY].Render( m_pDevice, m_pCamera);
		SetMaterial(NULL);
	}
	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);

	ResetTFOG(fWaterHeight > m_pCamera->m_vPosition.y ? TRUE : FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);

	if(m_pTARGET)
		ResetTARGET();
	ResetTMAINTAIN();

	if( m_pSESSION && !m_vMAP.m_vNEWQUESTOBJ.empty() )
		CheckQuestOBJ();

	CTDetailInfoManager::UpdateTick(dwTickCount);
	UpdateNameDetailTick(dwTickCount);

	bIsEXTVisible = m_vMAP.IsEXTVisible(m_pCamera);
	if( GetFarIMAGEOption() && bIsEXTVisible )
	{
		m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, m_dwFogCOLOR);
		DisableLIGHT();
		RenderTSILHOUETTE();
		m_pDevice->m_pDevice->SetRenderState( D3DRS_AMBIENT, 0xFF000000);
	}

	RenderBSP();
	DisableLIGHT();

	if( bIsEXTVisible)
		m_vMAP.Render(m_pDevice);

	CTDynamicBillboard::ClearBUF();
	SetAmbient(0,0,0);

	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		if(!m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.empty())
		{
			DisableLIGHT();

			if( m_vMAP.m_vDRAWBSP[i]->m_pINFO && GetLIGHTMAPOption(m_vMAP.m_vDRAWBSP[i]->m_pINFO->m_bDungeon) )
				EnableLIGHT(m_vMAP.m_vDRAWBSP[i]);
			else
				EnableLIGHT();

			RenderTOBJ( &m_vMAP.m_vDRAWBSP[i]->m_vTDRAW, dwTickCount, FALSE);
			CTachyonMesh::m_bSoftwareVP = FALSE;
		}

	DisableLIGHT();
	EnableLIGHT();

	RenderTOBJ( &m_vMAP.m_vTDRAW, dwTickCount, m_bDrawGROUND);
	CTDynamicBillboard::UpdateBUF(
		m_pDevice,
		m_pCamera);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, TRUE);
#ifdef TEST_MODE
	DWORD dwTDBBCount = CTDynamicBillboard::GetTDBBCount();
#endif
	CTDynamicBillboard::RenderBUF(
		m_pDevice,
		m_pCamera);

	RenderARROW();
	if(!m_vTSCENE_PLAYER.IsRunning())
		RenderTTEXT(bIsEXTVisible);

	if(!m_vTPOINTER.IsDead())
		m_vTPOINTER.Render( m_pDevice, m_pCamera);

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	m_vMAP.RenderWATER(
		m_pDevice,
		m_pCamera);
	RenderTSLASH();
	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);

	CTachyonSFX::RenderBUF(
		m_pDevice,
		m_pCamera,
		CTClientObjBase::GetCullDIST() / 2.0f);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

	DisableLIGHT();
	RenderTNUMBER();

	if(CheckGHOSTScene())
		EndGHOSTScene();

	if(m_dwFLASHTick)
		RenderFLASH();
	RenderTCMDDIR();

	m_vTSCENE_PLAYER.Render(this);
	if(m_bShowUI)
	{
		if( CTNationOption::JAPAN_IME || CTNationOption::TAIWAN_IME )
		{
			if( m_pCandidate && m_pCandidate->IsVisible())
			{
				RemoveKid(m_pCandidate);
				AddKid(m_pCandidate);
			}
		}

		hr = TFrame::Render(dwTickCount);
	}

	if( SUCCEEDED(hr) && m_vDRAG.m_pIMAGE )
		hr = m_vDRAG.m_pIMAGE->Render(dwTickCount);

#ifdef TEST_MODE
	if(m_bShowDebugInfo)
	{
		CString strPOS;
		strPOS.Format(
			"X = %.2f, Y = %.2f, Height = %.2f, Dir = %d, Region = %d TDBB Level = %d\n"
			"Camera (Dir:%d, Pitch:%d, Dist:%.2f)\n"
			"DrawCnt (TOT:%d TDBB:%d PC:%d NPC:%d MON:%d OBJ:%d)",
			m_pMainChar->GetPositionX(), 
			m_pMainChar->GetPositionZ(), 
			m_pMainChar->GetPositionY(), 
			m_pMainChar->m_wDIR / 5,
			m_pMainChar->m_pTREGION ? m_pMainChar->m_pTREGION->m_dwID : 0,
			CTDynamicBillboard::m_bLevel,

			m_wCamDIR ,
			m_pCamera->m_wCamPITCH ,
			m_pCamera->m_fCamDIST,

			m_nTotalDrawCount,
			dwTDBBCount,
			m_vDrawCount[OT_PC],
			m_vDrawCount[OT_NPC],
			m_vDrawCount[OT_MON],
			m_vDrawCount[OT_NONE]);
		m_FontPOS.TextOut( m_pDevice->m_pDevice, strPOS, 0, 675);

		if( m_pTARGET )
		{
			FLOAT fLength = D3DXVec2Length( &(D3DXVECTOR2(m_pTARGET->GetPositionX(), m_pTARGET->GetPositionZ()) - 
				D3DXVECTOR2(m_pMainChar->GetPositionX(), m_pMainChar->GetPositionZ())) );		

			strPOS.Format( "Target Object[%d][%s] X[%.2f] Y[%.2f] Z[%2.f] Dist with me[%f] Action[%d]",
				m_pTARGET->m_dwID,
				(LPCTSTR)m_pTARGET->GetName(),
				m_pTARGET->GetPositionX(),
				m_pTARGET->GetPositionY(),
				m_pTARGET->GetPositionZ(),
				fLength,
				m_pTARGET->m_bAction );

			m_FontTARGET.TextOut( m_pDevice->m_pDevice, strPOS, 0, 655);
		}
	}
#endif

	CTClientGuildMark::MReset();
	return hr;
}

void CTClientGame::ResetTFOG( BYTE bWATER)
{
	FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
	BYTE bUnitX = BYTE(m_pCamera->m_vTarget.x / fUnitLength);
	BYTE bUnitZ = BYTE(m_pCamera->m_vTarget.z / fUnitLength);

	FLOAT fPosX = m_pCamera->m_vTarget.x - FLOAT(bUnitX) * fUnitLength;
	FLOAT fPosZ = m_pCamera->m_vTarget.z - FLOAT(bUnitZ) * fUnitLength;

	TLIGHT vTLIGHT[TLIGHT_COUNT];
	TLIGHT vTSHADOW;
	TLIGHT vTMTRL;
	TLIGHT vTMAPCOLOR;
	TFOG vTFOG;

	LPTUNITFOG pTFOG = CTChart::FindTFOG(
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	LPTLIGHT pTLIGHT = NULL;

	if(pTFOG)
		memcpy( &vTFOG, pTFOG->m_pTGLOBAL, sizeof(TFOG));
	else
		memset( &vTFOG, 0, sizeof(TFOG));

	pTLIGHT = CTChart::FindTLIGHT(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TLIGHTSET_OBJMTRL, 0,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTLIGHT)
	{
		m_vOBJMTRL.Ambient.r = pTLIGHT->m_fAmbientR;
		m_vOBJMTRL.Ambient.g = pTLIGHT->m_fAmbientG;
		m_vOBJMTRL.Ambient.b = pTLIGHT->m_fAmbientB;

		m_vOBJMTRL.Diffuse.r = pTLIGHT->m_fDiffuseR;
		m_vOBJMTRL.Diffuse.g = pTLIGHT->m_fDiffuseG;
		m_vOBJMTRL.Diffuse.b = pTLIGHT->m_fDiffuseB;
	}

	pTLIGHT = CTChart::FindTLIGHT(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TLIGHTSET_MAPCOLOR, 0,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTLIGHT)
		memcpy( &vTMAPCOLOR, pTLIGHT, sizeof(TLIGHT));
	else
		memset( &vTMAPCOLOR, 0, sizeof(TLIGHT));

	pTLIGHT = CTChart::FindTLIGHT(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TLIGHTSET_SHADOW, 0,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTLIGHT)
		memcpy( &vTSHADOW, pTLIGHT, sizeof(TLIGHT));
	else
		memset( &vTSHADOW, 0, sizeof(TLIGHT));

	pTLIGHT = CTChart::FindTLIGHT(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TLIGHTSET_MTRL, 0,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTLIGHT)
		memcpy( &vTMTRL, pTLIGHT, sizeof(TLIGHT));
	else
		memset( &vTMTRL, 0, sizeof(TLIGHT));

	for( BYTE i=0; i<TLIGHT_COUNT; i++)
	{
		pTLIGHT = CTChart::FindTLIGHT(
			m_vMAP.m_pMAP->m_nUnitCountX,
			TLIGHTSET_MAIN, i,
			m_vMAP.m_wMapID,
			bUnitX,
			bUnitZ);

		if(pTLIGHT)
			memcpy( &vTLIGHT[i], pTLIGHT, sizeof(TLIGHT));
		else
			memset( &vTLIGHT[i], 0, sizeof(TLIGHT));
	}

	if( fPosX < TUNITLERP_LENGTH && bUnitX > 0 )
	{
		FLOAT fTIME = (TUNITLERP_LENGTH - fPosX) / (2.0f * TUNITLERP_LENGTH);

		for( i=0; i<TLIGHT_COUNT; i++)
			LerpTLIGHT( &vTLIGHT[i], TLIGHTSET_MAIN, i, bUnitX - 1, bUnitZ, fTIME);

		LerpTLIGHT( &vTMAPCOLOR, TLIGHTSET_MAPCOLOR, 0, bUnitX - 1, bUnitZ, fTIME);
		LerpTLIGHT( &vTSHADOW, TLIGHTSET_SHADOW, 0, bUnitX - 1, bUnitZ, fTIME);
		LerpTLIGHT( &vTMTRL, TLIGHTSET_MTRL, 0, bUnitX - 1, bUnitZ, fTIME);

		LerpTFOG( &vTFOG, bUnitX - 1, bUnitZ, fTIME);
	}
	else if( fPosX > fUnitLength - TUNITLERP_LENGTH && bUnitX < m_vMAP.m_pMAP->m_nUnitCountX - 1 )
	{
		FLOAT fTIME = (fPosX - fUnitLength + TUNITLERP_LENGTH) / (2.0f * TUNITLERP_LENGTH);

		for( i=0; i<TLIGHT_COUNT; i++)
			LerpTLIGHT( &vTLIGHT[i], TLIGHTSET_MAIN, i, bUnitX + 1, bUnitZ, fTIME);

		LerpTLIGHT( &vTMAPCOLOR, TLIGHTSET_MAPCOLOR, 0, bUnitX + 1, bUnitZ, fTIME);
		LerpTLIGHT( &vTSHADOW, TLIGHTSET_SHADOW, 0, bUnitX + 1, bUnitZ, fTIME);
		LerpTLIGHT( &vTMTRL, TLIGHTSET_MTRL, 0, bUnitX + 1, bUnitZ, fTIME);

		LerpTFOG( &vTFOG, bUnitX + 1, bUnitZ, fTIME);
	}

	if( fPosZ < TUNITLERP_LENGTH && bUnitZ > 0 )
	{
		FLOAT fTIME = (TUNITLERP_LENGTH - fPosZ) / (2.0f * TUNITLERP_LENGTH);

		for( i=0; i<TLIGHT_COUNT; i++)
			LerpTLIGHT( &vTLIGHT[i], TLIGHTSET_MAIN, i, bUnitX, bUnitZ - 1, fTIME);

		LerpTLIGHT( &vTMAPCOLOR, TLIGHTSET_MAPCOLOR, 0, bUnitX, bUnitZ - 1, fTIME);
		LerpTLIGHT( &vTSHADOW, TLIGHTSET_SHADOW, 0, bUnitX, bUnitZ - 1, fTIME);
		LerpTLIGHT( &vTMTRL, TLIGHTSET_MTRL, 0, bUnitX, bUnitZ - 1, fTIME);

		LerpTFOG( &vTFOG, bUnitX, bUnitZ - 1, fTIME);
	}
	else if( fPosZ > fUnitLength - TUNITLERP_LENGTH && bUnitZ < m_vMAP.m_pMAP->m_nUnitCountZ - 1 )
	{
		FLOAT fTIME = (fPosZ - fUnitLength + TUNITLERP_LENGTH) / (2.0f * TUNITLERP_LENGTH);

		for( i=0; i<TLIGHT_COUNT; i++)
			LerpTLIGHT( &vTLIGHT[i], TLIGHTSET_MAIN, i, bUnitX, bUnitZ + 1, fTIME);

		LerpTLIGHT( &vTMAPCOLOR, TLIGHTSET_MAPCOLOR, 0, bUnitX, bUnitZ + 1, fTIME);
		LerpTLIGHT( &vTSHADOW, TLIGHTSET_SHADOW, 0, bUnitX, bUnitZ + 1, fTIME);
		LerpTLIGHT( &vTMTRL, TLIGHTSET_MTRL, 0, bUnitX, bUnitZ + 1, fTIME);

		LerpTFOG( &vTFOG, bUnitX, bUnitZ + 1, fTIME);
	}

	if(pTFOG)
		for( i=0; i<BYTE(pTFOG->m_vTLOCAL.size()); i++)
		{
			FLOAT fLocalX = pTFOG->m_vTLOCAL[i]->m_fPosX - FLOAT(bUnitX) * fUnitLength;
			FLOAT fLocalZ = pTFOG->m_vTLOCAL[i]->m_fPosZ - FLOAT(bUnitZ) * fUnitLength;

			FLOAT fLength = D3DXVec2Length(&D3DXVECTOR2(
				fLocalX - fPosX,
				fLocalZ - fPosZ));

			if( fLength < pTFOG->m_vTLOCAL[i]->m_fRADIUS )
			{
				memcpy( &vTFOG, pTFOG->m_vTLOCAL[i], sizeof(TFOG));
				break;
			}
			else if( fLength < pTFOG->m_vTLOCAL[i]->m_fRANGE )
			{
				FLOAT fTIME = (pTFOG->m_vTLOCAL[i]->m_fRANGE - fLength) / (pTFOG->m_vTLOCAL[i]->m_fRANGE - pTFOG->m_vTLOCAL[i]->m_fRADIUS);
				LerpTFOG( &vTFOG, pTFOG->m_vTLOCAL[i], fTIME);
			}
		}

	if(bWATER)
	{
		pTFOG = CTChart::FindTFOG( 0xFFFF, 0, 0);

		if(pTFOG)
		{
			vTFOG.m_bRValue = pTFOG->m_pTGLOBAL->m_bRValue;
			vTFOG.m_bGValue = pTFOG->m_pTGLOBAL->m_bGValue;
			vTFOG.m_bBValue = pTFOG->m_pTGLOBAL->m_bBValue;

			vTFOG.m_fDENSITY = max( vTFOG.m_fDENSITY, pTFOG->m_pTGLOBAL->m_fDENSITY);
			vTFOG.m_fSTART = min( vTFOG.m_fSTART, pTFOG->m_pTGLOBAL->m_fSTART);
			vTFOG.m_fEND = min( vTFOG.m_fEND, pTFOG->m_pTGLOBAL->m_fEND);
		}
	}

	for( i=0; i<TLIGHT_COUNT; i++)
	{
		memset( &m_vLIGHT[i].m_Light, 0, sizeof(D3DLIGHT9));

		m_vLIGHT[i].m_Light.Type = D3DLIGHT_DIRECTIONAL;
		m_vLIGHT[i].m_Light.Direction.x = vTLIGHT[i].m_vDIR.x;
		m_vLIGHT[i].m_Light.Direction.y = vTLIGHT[i].m_vDIR.y;
		m_vLIGHT[i].m_Light.Direction.z = vTLIGHT[i].m_vDIR.z;

		m_vLIGHT[i].m_Light.Ambient.r = vTLIGHT[i].m_fAmbientR;
		m_vLIGHT[i].m_Light.Ambient.g = vTLIGHT[i].m_fAmbientG;
		m_vLIGHT[i].m_Light.Ambient.b = vTLIGHT[i].m_fAmbientB;
		m_vLIGHT[i].m_Light.Ambient.a = 1.0f;

		m_vLIGHT[i].m_Light.Diffuse.r = vTLIGHT[i].m_fDiffuseR;
		m_vLIGHT[i].m_Light.Diffuse.g = vTLIGHT[i].m_fDiffuseG;
		m_vLIGHT[i].m_Light.Diffuse.b = vTLIGHT[i].m_fDiffuseB;
		m_vLIGHT[i].m_Light.Diffuse.a = 1.0f;

		m_vLIGHT[i].SetLight(&m_vLIGHT[i].m_Light);
	}

	m_vMAP.m_dwTMapCOLOR = D3DCOLOR_ARGB(
		0xFF,
		BYTE(vTMAPCOLOR.m_fAmbientR * 0xFF),
		BYTE(vTMAPCOLOR.m_fAmbientG * 0xFF),
		BYTE(vTMAPCOLOR.m_fAmbientB * 0xFF));

	m_vMAP.m_bSHADOW = BYTE(vTSHADOW.m_fAmbientR * 0xFF);
	m_vMAP.m_pMAP->m_bSHADOW = m_vMAP.m_bSHADOW;

	m_vShadow.Ambient.r = vTMTRL.m_fAmbientR;
	m_vShadow.Ambient.g = vTMTRL.m_fAmbientG;
	m_vShadow.Ambient.b = vTMTRL.m_fAmbientB;
	m_vShadow.Diffuse.r = vTMTRL.m_fDiffuseR;
	m_vShadow.Diffuse.g = vTMTRL.m_fDiffuseG;
	m_vShadow.Diffuse.b = vTMTRL.m_fDiffuseB;

	if(m_pDevice->m_vCAPS.RasterCaps & D3DPRASTERCAPS_WFOG)
	{
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, vTFOG.m_bType);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	}
	else
	{
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGVERTEXMODE, vTFOG.m_bType);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE);
		m_pDevice->m_pDevice->SetRenderState( D3DRS_RANGEFOGENABLE, FALSE);
	}

	m_dwFogCOLOR = D3DCOLOR_ARGB( 0xFF, vTFOG.m_bRValue, vTFOG.m_bGValue, vTFOG.m_bBValue);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGCOLOR, m_dwFogCOLOR);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, TRUE);

	switch(vTFOG.m_bType)
	{
	case D3DFOG_LINEAR	:
		{
			m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGSTART, *(LPDWORD) (&vTFOG.m_fSTART));
			m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGEND, *(LPDWORD) (&vTFOG.m_fEND));
		}

		break;

	default				: m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGDENSITY, *(LPDWORD) (&vTFOG.m_fDENSITY)); break;
	}
}

void CTClientGame::LerpTLIGHT( LPTLIGHT pTLIGHT,
							   BYTE bSetID,
							   BYTE bIndex,
							   BYTE bUnitX,
							   BYTE bUnitZ,
							   FLOAT fTIME)
{
	LPTLIGHT pTBLEND = CTChart::FindTLIGHT(
		m_vMAP.m_pMAP->m_nUnitCountX,
		bSetID,
		bIndex,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTBLEND)
	{
		pTLIGHT->m_fAmbientR = FLOAT(pTLIGHT->m_fAmbientR) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fAmbientR) * fTIME;
		pTLIGHT->m_fAmbientG = FLOAT(pTLIGHT->m_fAmbientG) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fAmbientG) * fTIME;
		pTLIGHT->m_fAmbientB = FLOAT(pTLIGHT->m_fAmbientB) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fAmbientB) * fTIME;

		pTLIGHT->m_fDiffuseR = FLOAT(pTLIGHT->m_fDiffuseR) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fDiffuseR) * fTIME;
		pTLIGHT->m_fDiffuseG = FLOAT(pTLIGHT->m_fDiffuseG) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fDiffuseG) * fTIME;
		pTLIGHT->m_fDiffuseB = FLOAT(pTLIGHT->m_fDiffuseB) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fDiffuseB) * fTIME;
	}
}

void CTClientGame::LerpTFOG( LPTFOG pTFOG,
							 BYTE bUnitX,
							 BYTE bUnitZ,
							 FLOAT fTIME)
{
	LPTUNITFOG pTBLEND = CTChart::FindTFOG(
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	if(pTBLEND)
		LerpTFOG( pTFOG, pTBLEND->m_pTGLOBAL, fTIME);
}

void CTClientGame::LerpTFOG( LPTFOG pTFOG,
							 LPTFOG pTBLEND,
							 FLOAT fTIME)
{
	pTFOG->m_bRValue = BYTE(FLOAT(pTFOG->m_bRValue) * (1.0f - fTIME) + FLOAT(pTBLEND->m_bRValue) * fTIME);
	pTFOG->m_bGValue = BYTE(FLOAT(pTFOG->m_bGValue) * (1.0f - fTIME) + FLOAT(pTBLEND->m_bGValue) * fTIME);
	pTFOG->m_bBValue = BYTE(FLOAT(pTFOG->m_bBValue) * (1.0f - fTIME) + FLOAT(pTBLEND->m_bBValue) * fTIME);

	pTFOG->m_fDENSITY = FLOAT(pTFOG->m_fDENSITY) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fDENSITY) * fTIME;
	pTFOG->m_fSTART = FLOAT(pTFOG->m_fSTART) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fSTART) * fTIME;
	pTFOG->m_fEND = FLOAT(pTFOG->m_fEND) * (1.0f - fTIME) + FLOAT(pTBLEND->m_fEND) * fTIME;
}

void CTClientGame::ResetTARGET()
{
	if( !m_pTARGET || !m_pCtrlHost )
		return;

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		m_pTARGET->GetPositionX() - m_pCtrlHost->GetPositionX(),
		m_pTARGET->GetPositionZ() - m_pCtrlHost->GetPositionZ()));

	CTClientPet* pTPET = GetMainPet();

	if( fDist > TARGET_DIST || !m_pTARGET->CheckRequired() || (
		m_pMainChar->m_bStandHide && fDist - m_pTARGET->m_fSight > TSTANDHIDE_SIGHT_DIST) || (
		m_pMainChar->m_bGhost && m_pMainChar != m_pTARGET &&
		!m_pTARGET->m_bDrawGhost && D3DXVec3Length(&D3DXVECTOR3(
		m_pTARGET->GetPositionX() - m_pMainChar->m_vTDEAD.x,
		m_pTARGET->GetPositionY() - m_pMainChar->m_vTDEAD.y,
		m_pTARGET->GetPositionZ() - m_pMainChar->m_vTDEAD.z)) > TGHOST_DIST ))
	{
		CTClientObjBase::m_mapTSELECTOBJ.clear();
		CTClientObjBase::m_mapTSELECTDEADOBJ.clear();
		ResetTargetOBJ(NULL);
		ResetTargetINFO(FALSE);
	}
	else if( fDist > TARGETUI_DIST || (m_vTFRAME[TFRAME_MONINVEN]->IsVisible() && !m_pMainChar->GetRidingPet() &&
		m_pMainChar->m_bAction != m_pMainChar->GetTLOOPAction(TA_LOOT_START)) )
		DisableTargetUI();
}

void CTClientGame::ResetVisibleOBJ( DWORD dwTick)
{
	MAPTSWITCH::iterator itTSWITCH;
	MAPMONSTER::iterator itMON;
	MAPPLAYER::iterator itPC;
	MAPRECALL::iterator itRECALL;

	if(m_bResetMAP)
	{
		m_vMAP.ResetTMAP(
			m_pDevice,
			m_pCamera,
			dwTick,
			FALSE);
	}

	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.clear();
	m_vMAP.m_vTDRAW.clear();

	for( itTSWITCH = m_mapTSwitch.begin(); itTSWITCH != m_mapTSwitch.end(); itTSWITCH++)
	{
		ResetDetect((*itTSWITCH).second);

		(*itTSWITCH).second->ResetVisible(
			m_pCamera,
			m_vMAP.m_dwHouseID,
			m_vMAP.m_bHouseMesh,
			dwTick);

		if((*itTSWITCH).second->m_bVisible)
			PushDrawOBJ((*itTSWITCH).second);
		else
			(*itTSWITCH).second->CalcRender(m_pDevice);
	}

	for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
	{
		ResetDetect((*itRECALL).second);

		(*itRECALL).second->ResetVisible(
			m_pCamera,
			m_vMAP.m_dwHouseID,
			m_vMAP.m_bHouseMesh,
			dwTick);

		if( (*itRECALL).second->m_bVisible && !m_vTSCENE_PLAYER.IsRunning() )
			PushDrawOBJ((*itRECALL).second);
		else
			(*itRECALL).second->CalcRender(m_pDevice);
	}

	for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
	{
		ResetDetect((*itRECALL).second);

		(*itRECALL).second->ResetVisible(
			m_pCamera,
			m_vMAP.m_dwHouseID,
			m_vMAP.m_bHouseMesh,
			dwTick);

		if( (*itRECALL).second->m_bVisible && !m_vTSCENE_PLAYER.IsRunning() )
			PushDrawOBJ((*itRECALL).second);
		else
			(*itRECALL).second->CalcRender(m_pDevice);
	}

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
	{
		ResetDetect((*itMON).second);

		(*itMON).second->ResetVisible(
			m_pCamera,
			m_vMAP.m_dwHouseID,
			m_vMAP.m_bHouseMesh,
			dwTick);

		if( (*itMON).second->m_bVisible && !m_vTSCENE_PLAYER.IsRunning() )
			PushDrawOBJ((*itMON).second);
		else
			(*itMON).second->CalcRender(m_pDevice);
	}

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
	{
		CTClientChar *pTCHAR = itPC->second;

		if(!pTCHAR->GetRidingPet())
		{
			ResetDetect(pTCHAR);

			pTCHAR->ResetVisible(
				m_pCamera,
				m_vMAP.m_dwHouseID,
				m_vMAP.m_bHouseMesh,
				dwTick);

			if( pTCHAR->m_bVisible && !m_vTSCENE_PLAYER.IsRunning() )
				PushDrawOBJ(pTCHAR);
			else
				pTCHAR->CalcRender(m_pDevice);
		}
	}

	for( i=0; i<9; i++)
		for( int j=0; j<INT(m_vMAP.m_vDRAWOBJ[i].size()); j++)
			PushDrawOBJ(m_vMAP.m_vDRAWOBJ[i][j]);

	for( i=0; i<INT(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
		if( !m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTMARK.m_bHide || m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTMARK.m_bAlpha )
		{
			m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTMARK.ResetVisible(
				m_pCamera,
				0, FALSE,
				dwTick);

			if(m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTMARK.m_bVisible)
				PushDrawOBJ(&m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTMARK);
			else
				m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTMARK.CalcRender(m_pDevice);
		}

	if( !m_vTCMDTARGET.m_bHide || m_vTCMDTARGET.m_bAlpha )
	{
		m_vTCMDTARGET.ResetVisible(
			m_pCamera,
			0, FALSE,
			dwTick);

		if(m_vTCMDTARGET.m_bVisible)
			PushDrawOBJ(&m_vTCMDTARGET);
		else
			m_vTCMDTARGET.CalcRender(m_pDevice);
	}

	if(m_pMainChar->m_bGhost)
	{
		m_vMainDead.ResetVisible(
			m_pCamera,
			m_vMAP.m_dwHouseID,
			m_vMAP.m_bHouseMesh,
			dwTick);
		PushDrawOBJ(&m_vMainDead);
	}
	ResetDetect(m_pMainChar);

	if( !m_pMainChar->GetRidingPet() )
	{
		m_pMainChar->ResetVisible(
			m_pCamera,
			m_vMAP.m_dwHouseID,
			m_vMAP.m_bHouseMesh,
			dwTick);

		if( m_pMainChar->m_bAlpha )
			PushDrawOBJ(m_pMainChar);
	}

	if( m_pMainChar->InCastle() )
		SiegeMgr->ResetVisibleOBJ(dwTick);

	for( i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.sort(binary_draw());
	m_vMAP.m_vTDRAW.sort(binary_draw());

	ResetHitOBJ(m_vKEY.m_point, FALSE);
}

void CTClientGame::UpdateItemUpGauge(DWORD dwTick)
{
	CTItemUpGaugeDlg* pGaugeDlg = static_cast<CTItemUpGaugeDlg*>( m_vTFRAME[TFRAME_ITEM_UP_GAUGE] );
	if( !pGaugeDlg->IsVisible() )
		return;
		
	if( pGaugeDlg->Update(dwTick) )
	{
		CTItemUpDlg* pDlg = static_cast<CTItemUpDlg*>( m_vTFRAME[TFRAME_ITEM_UP] );
		CTColorPicker* pColorPicker = static_cast<CTColorPicker*>( m_vTFRAME[ TFRAME_COLOR_PICKER ] );

		if( !pDlg->m_pOpenByCash )
		{
			WORD wNpcID = (WORD) static_cast<CTClientNpc*>(m_pTARGET)->m_dwID;

			m_pSESSION->SendCS_ITEMUPGRADE_REQ(
				pDlg->GetLastInvenID(CTItemUpDlg::TARGET_SLOT), pDlg->GetLastInvenSlot(CTItemUpDlg::TARGET_SLOT),
				pDlg->GetLastInvenID(CTItemUpDlg::SCROLL_SLOT), pDlg->GetLastInvenSlot(CTItemUpDlg::SCROLL_SLOT),
				wNpcID,
				INVEN_NULL,
				INVALID_SLOT,
				pColorPicker->m_wSelectedColor );
		}
		else
		{
			m_pSESSION->SendCS_ITEMUPGRADE_REQ(
				pDlg->GetLastInvenID(CTItemUpDlg::TARGET_SLOT), pDlg->GetLastInvenSlot(CTItemUpDlg::TARGET_SLOT),
				pDlg->GetLastInvenID(CTItemUpDlg::SCROLL_SLOT), pDlg->GetLastInvenSlot(CTItemUpDlg::SCROLL_SLOT),
				pDlg->m_pOpenByCash->m_wNPCID,
				pDlg->m_pOpenByCash->m_bInvenID,
				pDlg->m_pOpenByCash->m_bSlotID,
				pColorPicker->m_wSelectedColor );
		}

		m_pMainWnd->CloseModalFrame();
	}
}

void CTClientGame::UpdateProgressGauge(DWORD dwTick)
{
	CTProgressRegDlg* pDlg = static_cast<CTProgressRegDlg*>( m_vTFRAME[TFRAME_PROGRESS_REG] );

	if( pDlg->IsVisible() )
		pDlg->Update(dwTick);
}

void CTClientGame::PushDrawOBJ( CTClientObjBase *pTOBJ)
{
	if(!m_vMAP.IsInValidUNIT(pTOBJ))
		return;

	BOOL bDraw = TRUE;
	switch(pTOBJ->m_bType)
	{
	case OT_GATE	:
		if(m_pMainChar->m_bGhost)
		{
			bDraw = FALSE;
		}
		break;

	case OT_RECALL	:
	case OT_SELF	:
	case OT_MON		:
	case OT_NPC		:
	case OT_PC		:
		if( !pTOBJ->m_bDrawGhost &&
			m_pMainChar->m_bGhost && pTOBJ != m_pMainChar &&
			(pTOBJ->m_fCamDIST > TGHOSTCAM_DIST ||
				D3DXVec3Length(&D3DXVECTOR3(
				m_pMainChar->m_vTDEAD.x - pTOBJ->GetPositionX(),
				m_pMainChar->m_vTDEAD.y - pTOBJ->GetPositionY(),
				m_pMainChar->m_vTDEAD.z - pTOBJ->GetPositionZ())) > TGHOST_DIST) )
		{
			bDraw = FALSE;
		}
		break;
	}

	if( bDraw )
	{
		if( pTOBJ->m_bType != OT_BSP )
		{
			BYTE bNodeOBJ = FALSE;

			for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
				if( pTOBJ->m_dwHouseID == m_vMAP.m_vDRAWBSP[i]->m_dwHouseID )
				{
					DWORD dwNodeID = m_vMAP.m_vDRAWBSP[i]->GetNodeID(D3DXVECTOR3(
						pTOBJ->GetPositionX(),
						pTOBJ->GetPositionY(),
						pTOBJ->GetPositionZ()));

					if( dwNodeID != NODEID_NULL )
					{
						if((m_vMAP.m_dwBspID == NODEID_NULL || m_vMAP.m_dwBspID == m_vMAP.m_vDRAWBSP[i]->m_dwBspID ||
							m_vMAP.m_dwHouseID == m_vMAP.m_vDRAWBSP[i]->m_dwHouseID) && (pTOBJ->m_bForceDraw ||
							m_vMAP.m_vDRAWBSP[i]->m_mapPVS.find(dwNodeID) != m_vMAP.m_vDRAWBSP[i]->m_mapPVS.end() ))
						{
							IncreaseDrawCount(pTOBJ);
							m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.push_back(pTOBJ);
							return;
						}

						bNodeOBJ = TRUE;
					}
				}

			if(bNodeOBJ)
				return;
		}

		IncreaseDrawCount(pTOBJ);
		m_vMAP.m_vTDRAW.push_back(pTOBJ);
	}
}

void CTClientGame::ClearDrawCount()
{
	m_nTotalDrawCount = 0;
	ZeroMemory(m_vDrawCount, sizeof(m_vDrawCount));
}

void CTClientGame::IncreaseDrawCount(CTClientObjBase *pTOBJ)
{
	++m_nTotalDrawCount;
	++m_vDrawCount[pTOBJ->m_bType];
}

void CTClientGame::ResetDetect( CTClientObjBase *pTOBJ)
{
	if(!m_pMainChar->IsAlliance(pTOBJ))
	{
		if( !pTOBJ->m_bClarity &&
			m_pMainChar->m_wTDETECT & (1 << pTOBJ->m_bType) )
		{
			pTOBJ->m_bDetect = TRUE;
		}
		else
			pTOBJ->m_bDetect = FALSE;
		
		if( pTOBJ->m_bHide && !pTOBJ->m_bDetect )
		{
			if( m_pACT == pTOBJ )
			{
				if(m_bCHARGE)
				{
					SendCS_ACTION_REQ(
						m_pMainChar->m_dwID,
						m_pMainChar->m_bType,
						0, 0, 0,
						m_pMainWnd->m_bChannel,
						m_vMAP.m_wMapID, 0);
				}

				CancelCharge(TRUE);
			}

			if( m_pTARGET == pTOBJ )
			{
				CTClientObjBase::m_mapTSELECTOBJ.clear();
				CTClientObjBase::m_mapTSELECTDEADOBJ.clear();
				ResetTargetOBJ(NULL);
				ResetTargetINFO(FALSE);
			}
		}
	}
	else
		pTOBJ->m_bDetect = TRUE;
}

void CTClientGame::RenderTSILHOUETTE()
{
	CTClientCAM vTCAM;

	vTCAM.InitCamera(
		m_pDevice->m_pDevice,
		TNOCULL_FACTOR * GetOBJRangeOption() * CTClientObjBase::m_fCamDist,
		m_pCamera->m_fFarPlane + m_pCamera->m_fFarPlane,
		m_pCamera->m_fFOV,
		INT(m_pCamera->m_fWidth),
		INT(m_pCamera->m_fHeight));

	vTCAM.SetPosition(
		m_pCamera->m_vPosition,
		m_pCamera->m_vTarget,
		m_pCamera->m_vUp,
		FALSE);
	vTCAM.Activate(TRUE);

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	for( BYTE i=0; i<9; i++)
		if(m_vMAP.m_pMAP->m_pUNIT[i])
			for( WORD j=0; j<m_vMAP.m_vTSILHOUETTE[m_vMAP.m_pMAP->m_bUNIT[i]].m_wCount; j++)
				if(m_vMAP.m_vTSILHOUETTE[m_vMAP.m_pMAP->m_bUNIT[i]].m_vTOBJ[j])
					m_vMAP.m_vTSILHOUETTE[m_vMAP.m_pMAP->m_bUNIT[i]].m_vTOBJ[j]->Render( m_pDevice, &vTCAM);

	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);
	m_pCamera->Activate(TRUE);
}

void CTClientGame::RenderBSP()
{
	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		if( m_vMAP.m_vDRAWBSP[i]->m_pINFO && m_vMAP.m_vDRAWBSP[i]->m_pMAP )
			if( m_vMAP.m_dwBspID != m_vMAP.m_vDRAWBSP[i]->m_dwBspID && m_vMAP.m_dwHouseID != m_vMAP.m_vDRAWBSP[i]->m_dwHouseID )
			{
				D3DXVECTOR3 vA = m_vMAP.m_vDRAWBSP[i]->m_vNodePOS - m_pCamera->m_vPosition;
				D3DXVECTOR3 vU = m_pCamera->m_vTarget - m_pCamera->m_vPosition;

				vU /= D3DXVec3Length(&vU);
				FLOAT fDIST = D3DXVec3Dot( &vA, &vU);

				if( m_vMAP.m_vDRAWBSP[i]->m_fCamDIST < m_vMAP.m_vDRAWBSP[i]->m_fRadius || fDIST + m_vMAP.m_vDRAWBSP[i]->m_fRadius > 0.5f )
				{
					vA = m_pCamera->m_vPosition + fDIST * vU - m_vMAP.m_vDRAWBSP[i]->m_vNodePOS;

					FLOAT fLength = D3DXVec3Length(&vA) - m_vMAP.m_vDRAWBSP[i]->m_fRadius;
					FLOAT fHeight = fDIST * tanf(m_pCamera->m_fFOV / 2.0f);
					FLOAT fWidth = fHeight * m_pCamera->m_fWidth / m_pCamera->m_fHeight;

					if( sqrtf(fHeight * fHeight + fWidth * fWidth) > fLength )
					{
						if(GetLIGHTMAPOption(m_vMAP.m_vDRAWBSP[i]->m_pINFO->m_bDungeon))
							DisableLIGHT();
						else
							EnableLIGHT();

						m_vMAP.m_vDRAWBSP[i]->Render();
					}
				}
			}
			else
			{
				if(GetLIGHTMAPOption(m_vMAP.m_vDRAWBSP[i]->m_pINFO->m_bDungeon))
					DisableLIGHT();
				else
					EnableLIGHT();

				m_vMAP.m_vDRAWBSP[i]->Render();
			}
}

void CTClientGame::RenderTSLASH()
{
	LISTTOBJBASE::iterator itTOBJ;

	CTachyonSlashSFX::BeginSFXScene(m_pDevice);
	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		if(!m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.empty())
		{
			DisableLIGHT();

			if( m_vMAP.m_vDRAWBSP[i]->m_pINFO && GetLIGHTMAPOption(m_vMAP.m_vDRAWBSP[i]->m_pINFO->m_bDungeon) )
				EnableLIGHT(m_vMAP.m_vDRAWBSP[i]);
			else
				EnableLIGHT();

			for( itTOBJ = m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.begin(); itTOBJ != m_vMAP.m_vDRAWBSP[i]->m_vTDRAW.end(); itTOBJ++)
				if((*itTOBJ)->m_bDynamicOBJ)
					(*itTOBJ)->RenderTSLASH(m_pDevice);

			CTachyonMesh::m_bSoftwareVP = FALSE;
		}

	DisableLIGHT();
	EnableLIGHT();

	for( itTOBJ = m_vMAP.m_vTDRAW.begin(); itTOBJ != m_vMAP.m_vTDRAW.end(); itTOBJ++)
		if((*itTOBJ)->m_bDynamicOBJ)
			(*itTOBJ)->RenderTSLASH(m_pDevice);

	for( i=0; i<INT(m_vTARROW.size()); i++)
		m_vTARROW[i]->m_vTSLASH.Render(m_pDevice);

	for( i=0; i<INT(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
		m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTARROW.m_vTSLASH.Render(m_pDevice);
	m_vTCMDARROW.m_vTSLASH.Render(m_pDevice);

	CTachyonSlashSFX::EndSFXScene(m_pDevice);
}

void CTClientGame::RenderTOBJ( LPLISTTOBJBASE pLIST,
							   DWORD dwTick,
							   BYTE bDrawGROUND)
{
	CTClientObjBase *pTARGET = NULL;
	LISTTOBJBASE::iterator itTOBJ;

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	for( itTOBJ = pLIST->begin(); itTOBJ != pLIST->end(); itTOBJ++)
	{
		if( !(*itTOBJ)->m_bDynamicOBJ &&
			!(*itTOBJ)->m_bLastDraw )
			RenderTOBJ( *itTOBJ, dwTick);

		if( (*itTOBJ) == m_pTARGET )
			pTARGET = m_pTARGET;
	}
	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);

	CTClientObjBase::BeginDrawSquare(m_pDevice);
	SetMaterial(NULL);

	CTClientObjBase::m_pTSHADOW->m_dwCurTick = m_dwTSHADOW;
	CTClientObjBase::m_pTSELECT->m_dwCurTick = m_dwTSELECT;
	m_pTGROUNDTEX->m_dwCurTick = m_dwTGROUND;

	UVKEY vKEY = CTClientObjBase::m_pTSHADOW->GetUV();
	FLOAT fDepthBias = TOBJSHADOW_BIAS;
	D3DXMATRIX vUV;

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
	m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xC0000000);
	m_pDevice->m_pDevice->SetTexture( 0, CTClientObjBase::m_pTSHADOW->GetTexture());
	CTMath::GetUVMatrix( &vUV, &vKEY);

	for( itTOBJ = pLIST->begin(); itTOBJ != pLIST->end(); itTOBJ++)
		if((*itTOBJ)->GetDrawShadow())
			RenderTSQUARE( (*itTOBJ), &vUV, NULL, TOBJSHADOW_SCALE);

	if(pTARGET)
	{
		D3DXVECTOR2 vDIR(
			pTARGET->GetPositionX() - m_pCamera->m_vPosition.x,
			pTARGET->GetPositionZ() - m_pCamera->m_vPosition.z);
		FLOAT fLength = D3DXVec2Length(&vDIR);

		if( fLength > 0.0f )
			vDIR /= fLength;
		fDepthBias = TOBJSELECT_BIAS;

		m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
		m_pDevice->m_pDevice->SetTexture( 0, CTClientObjBase::m_pTSELECT->GetTexture());
		m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, GetObjectColor(pTARGET));

		vKEY = CTClientObjBase::m_pTSELECT->GetUV();
		CTMath::GetUVMatrix( &vUV, &vKEY);

		RenderTSQUARE( pTARGET, &vUV, fLength > 0.0f ? &vDIR : NULL, TOBJSELECT_SCALE);
	}

	if(bDrawGROUND)
	{
		CTClientSkill *pTSKILL = m_pMainChar->FindTSkill(m_wSkillID);

		if(pTSKILL)
		{
			D3DXVECTOR2 vDIR(
				m_vTGROUND.x - m_pCamera->m_vPosition.x,
				m_vTGROUND.z - m_pCamera->m_vPosition.z);
			FLOAT fLength = D3DXVec2Length(&vDIR);

			if( fLength > 0.0f )
				vDIR /= fLength;
			fDepthBias = TOBJSELECT_BIAS;

			m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
			m_pDevice->m_pDevice->SetTexture( 0, m_pTGROUNDTEX->GetTexture());
			m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFF33CC33);

			vKEY = m_pTGROUNDTEX->GetUV();
			CTMath::GetUVMatrix( &vUV, &vKEY);

			RenderTSQUARE(
				&m_vTGROUND,
				&D3DXVECTOR2(
				2.0f * pTSKILL->m_pTSKILL->m_fAtkRange,
				2.0f * pTSKILL->m_pTSKILL->m_fAtkRange),
				&vUV,
				fLength > 0.0f ? &vDIR : NULL,
				1.0f,
				m_bLAND);
		}
	}

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	for( itTOBJ = pLIST->begin(); itTOBJ != pLIST->end(); itTOBJ++)
		if( (*itTOBJ)->m_bDynamicOBJ &&
			!(*itTOBJ)->m_bLastDraw )
			RenderTOBJ( *itTOBJ, dwTick);
	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);

	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	for( itTOBJ = pLIST->begin(); itTOBJ != pLIST->end(); itTOBJ++)
		if((*itTOBJ)->m_bLastDraw)
			RenderTOBJ( *itTOBJ, dwTick);
	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);
	fDepthBias = EXTBIAS_BASE;

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DEPTHBIAS, *((LPDWORD) &fDepthBias));
	SetMaterial(NULL);
}

void CTClientGame::RenderTOBJ( CTClientObjBase *pTOBJ,
							   DWORD dwTick)
{
	bool bHighlight = (pTOBJ == m_pHIT && (m_pHIT != m_pTARGET || m_pHIT->m_bType == OT_SWITCH));

	pTOBJ->m_bInSHADOW = m_vMAP.PtInSHADOW(
		pTOBJ->GetPositionX(),
		pTOBJ->GetPositionZ()) ? TRUE : FALSE;

	if( pTOBJ->m_bAlpha >= ALPHA_MAX && (
		pTOBJ->m_bType != OT_NPC ||
		pTOBJ->m_bNPCType != TNPC_BOX) &&
		CTDynamicBillboard::CanUSE(
		m_pDevice,
		m_pCamera,
		&pTOBJ->GetPosition(),
		pTOBJ->m_fRadius,
		pTOBJ->m_fSizeY) )
	{
		if(!pTOBJ->m_pTDBB)
		{
			pTOBJ->m_pTDBB = new CTDynamicBillboard();

			pTOBJ->m_pTDBB->CreateDynamicBillboard(
				m_pDevice,
				m_pCamera,
				pTOBJ,
				pTOBJ->m_fRadius,
				pTOBJ->m_fSizeY);
		}

		pTOBJ->m_pTDBB->CalcTick(dwTick);
	}
	else if(m_bDrawOBJ)
	{
		SetMaterial(pTOBJ);

		if(pTOBJ->m_pTDBB)
			pTOBJ->m_pTDBB->ResetTick();

		if(bHighlight)
			SetAmbient( TSELECT_COLOR, TSELECT_COLOR, TSELECT_COLOR);

		if( pTOBJ->m_bAlpha < ALPHA_MAX )
		{
			m_pDevice->m_pDevice->SetRenderState( D3DRS_COLORWRITEENABLE, 0);
			pTOBJ->Render( m_pDevice, m_pCamera);
			pTOBJ->m_bZWrite = FALSE;

			m_pDevice->m_pDevice->SetRenderState(
				D3DRS_COLORWRITEENABLE,
				D3DCOLORWRITEENABLE_ALPHA|
				D3DCOLORWRITEENABLE_RED|
				D3DCOLORWRITEENABLE_GREEN|
				D3DCOLORWRITEENABLE_BLUE);
		}

		pTOBJ->Render( m_pDevice, m_pCamera);
		pTOBJ->m_bZWrite = TRUE;

		if( bHighlight )
			SetAmbient( 0, 0, 0);
	}
	
	if( pTOBJ->m_bType == OT_NPC && pTOBJ->m_vTMARKER.m_OBJ.m_pOBJ )
		pTOBJ->m_vTMARKER.Render( m_pDevice, m_pCamera);
}

void CTClientGame::RenderTTEXT( BYTE bIsEXTVisible)
{
	if(!(m_pDevice->m_vCAPS.RasterCaps & D3DPRASTERCAPS_WFOG))
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);
	m_pTextCAM->Activate(TRUE);

	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		RenderTTEXT(&m_vMAP.m_vDRAWBSP[i]->m_vTDRAW);

	RenderTTEXT(&m_vMAP.m_vTDRAW);
	RenderTTEXT(m_pMainChar);
	m_pCamera->Activate(TRUE);

	if(!(m_pDevice->m_vCAPS.RasterCaps & D3DPRASTERCAPS_WFOG))
		m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, TRUE);
}

void CTClientGame::RenderTTEXT( LPLISTTOBJBASE pLIST)
{
	LISTTOBJBASE::iterator itTOBJ;

	for( itTOBJ = pLIST->begin(); itTOBJ != pLIST->end(); itTOBJ++)
		if( (*itTOBJ)->GetDrawName() || (*itTOBJ)->m_bDrawTalk || (*itTOBJ) == m_pTARGET )
			RenderTTEXT(*itTOBJ);
}

D3DXVECTOR3 CTClientGame::GetBaseNamePosition(CTClientObjBase *pTOBJ)
{
	D3DXVECTOR3 vTPOS = pTOBJ->GetPosition();
	CTClientObjBase *pTHOST = pTOBJ;

	if( pTOBJ->m_bType == OT_RECALL && ((CTClientRecall *) pTOBJ)->m_bRecallType == TRECALLTYPE_PET )
	{
		CTClientChar *pTCHAR = ((CTClientPet *) pTOBJ)->GetTakeUpChar();

		if(pTCHAR)
		{
			vTPOS.y += pTOBJ->GetAttrFLOAT(ID_MOUNT_HEIGHT) + pTCHAR->m_fSizeY * TMOUNTTEXT_SCALE;
			pTHOST = pTCHAR;
		}
		else
			vTPOS.y += pTOBJ->m_fSizeY;
	}
	else
		vTPOS.y += pTOBJ->m_fSizeY;

	CTMath::Transform(
		&m_pCamera->m_vPROJ,
		&vTPOS);

	vTPOS.y += TTEXT_HEIGHT;

	return vTPOS;
}

void CTClientGame::RenderTTEXT( CTClientObjBase *pTOBJ)
{
	D3DXVECTOR3 vTPOS = GetBaseNamePosition( pTOBJ );
	CTClientObjBase *pTHOST = pTOBJ;

	if( pTOBJ->m_bType == OT_RECALL && ((CTClientRecall *) pTOBJ)->m_bRecallType == TRECALLTYPE_PET )
	{
		CTClientChar *pTCHAR = ((CTClientPet *) pTOBJ)->GetTakeUpChar();

		if(pTCHAR)
			pTHOST = pTCHAR;
	}

	if( pTOBJ->GetDrawName() || (pTOBJ == m_pTARGET && pTOBJ->m_bType != OT_PC) )
	{
		CString strUserTITLE = pTOBJ->GetUserTitle();
		CString strTITLE = pTOBJ->GetTitle();
		CString strNAME = pTOBJ->GetName();
		CString strTEXT;

		pTOBJ->m_vNAME.m_dwColor = GetObjectColor(pTHOST);
		pTOBJ->m_vNAME.m_dwLineColor = 0xFF000000;

		if(!pTHOST->IsDead())
		{
			if( pTHOST->m_bType == OT_PC )
			{
				CTClientChar *pTCHAR = (CTClientChar *) pTHOST;

				if(pTCHAR->m_bPrivateShop)
				{
					strTITLE = CTChart::Format( TSTR_FMT_PRVSHOP_TITLE, strNAME);
					strNAME = pTCHAR->m_strPrivateShop;
				}
				else if(!pTCHAR->m_strTACTICS.IsEmpty())
				{
					strTITLE = pTCHAR->m_strTACTICS;

					if(pTCHAR->m_dwTacticsID)
						strNAME = CTChart::LoadString(TSTR_GUILD_PEER_TACTICS) + CString(_T(" ")) + strNAME;
					else if( pTCHAR->m_bGuildPeer != GUILD_PEER_NONE )
						strNAME = CTGuildCommander::GetPeerageStr(pTCHAR->m_bGuildPeer) + CString(_T(" ")) + strNAME;
				}
				else if(!pTCHAR->m_strGUILD.IsEmpty())
				{
					strTITLE = pTCHAR->m_strGUILD;

					if(pTCHAR->m_dwTacticsID)
						strNAME = CTChart::LoadString(TSTR_GUILD_PEER_TACTICS) + CString(_T(" ")) + strNAME;
					else if( pTCHAR->m_bGuildPeer != GUILD_PEER_NONE )
						strNAME = CTGuildCommander::GetPeerageStr(pTCHAR->m_bGuildPeer) + CString(_T(" ")) + strNAME;
				}
				else if( pTCHAR->m_wPartyID && pTCHAR->m_dwID == pTCHAR->m_dwChiefID )
					strTITLE = CTChart::Format(pTCHAR->m_wPartyID == pTCHAR->m_wCommanderID ? TSTR_FMT_PC_COMMANDER : TSTR_FMT_PC_CHIEF);

				if( pTCHAR->m_bUseFameTitle && !pTCHAR->m_strFameTitle.IsEmpty() )
					strNAME = CTChart::Format( TSTR_FMT_FIELDRANKNAME, pTCHAR->m_strFameTitle, strNAME);
			}

			if(strTITLE.IsEmpty())
				strTEXT = CTChart::Format( TSTR_FMT_FIELDNAME, strNAME);
			else
				strTEXT = CTChart::Format( TSTR_FMT_FIELDTITLE, strTITLE, strNAME);
		}
		else if(pTOBJ->IsDrawNameWhenDead())
			strTEXT = CTChart::Format( TSTR_FMT_DEAD, strNAME);
		else
			strTEXT = strNAME;

		pTOBJ->m_vNAME.MakeText( m_pDevice->m_pDevice, strTEXT, CRect( 0, 0, 0, 0), DT_CALCRECT|DT_CENTER);
		FLOAT fBaseHeight = FLOAT(pTOBJ->m_vNAME.GetHeight());
		D3DXMATRIX vWORLD;

		ResetObjNamePos( pTOBJ, vTPOS);
		D3DXMatrixIdentity(&vWORLD);
		vWORLD._41 = vTPOS.x;
		vWORLD._42 = vTPOS.y;
		vWORLD._43 = vTPOS.z;

		pTOBJ->m_vNAME.TextOut(
			m_pDevice->m_pDevice,
			vWORLD,
			strTEXT,
			fBaseHeight,
			DT_CENTER);

		FLOAT fRIGHT = FLOAT(pTOBJ->m_vNAME.GetWidth()) / 2.0f;
		FLOAT fLEFT = fRIGHT + TNPCICON_SIZE;

		BYTE bICON = RenderTICON( pTHOST, vTPOS, fBaseHeight, fLEFT);
		RenderTCAMP( pTHOST, vTPOS, fBaseHeight, fRIGHT);

		if( pTHOST == m_pTARGET )
		{
			FLOAT fRIGHT = fLEFT - TNPCICON_SIZE;

			if( bICON && strTITLE.IsEmpty() )
				fLEFT += TNPCICON_SIZE;

			RenderTSELMARK( pTHOST, vTPOS, TNPCICON_SIZE, fLEFT, fRIGHT);
		}

		fRIGHT = TNPCICON_SIZE / 2.0f;
		vTPOS.y += fBaseHeight;

		if(RenderTSOULMATE( pTHOST, vTPOS, TNPCICON_SIZE, -fRIGHT))
			vTPOS.y += TNPCICON_SIZE;

		if(!strUserTITLE.IsEmpty())
		{
			pTOBJ->m_vUSERTITLE.MakeText( m_pDevice->m_pDevice, strUserTITLE, CRect( 0, 0, 0, 0), DT_CALCRECT|DT_CENTER);
			fBaseHeight = FLOAT(pTOBJ->m_vUSERTITLE.GetHeight());

			ResetObjUserTitlePos( pTOBJ, vTPOS);
			vWORLD._41 = vTPOS.x;
			vWORLD._42 = vTPOS.y;
			vWORLD._43 = vTPOS.z;

			pTOBJ->m_vUSERTITLE.TextOut(
				m_pDevice->m_pDevice,
				vWORLD,
				strUserTITLE,
				fBaseHeight,
				DT_CENTER);

			vTPOS.y += fBaseHeight + TTEXT_HEIGHT;
		}
	}

	if( m_vTOPTION.m_bTalkBox && pTOBJ->m_bDrawTalk )
		pTOBJ->m_vTALK.Render( m_pDevice->m_pDevice, &vTPOS);
}

void CTClientGame::ResetObjNamePos( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ)
{
	FLOAT fCamH = m_pMainWnd->m_Camera.m_fHeight / 2.0f;
	FLOAT fCamW = m_pMainWnd->m_Camera.m_fWidth / 2.0f;

	int nNameH = pTOBJ->m_vNAME.GetHeight();
	int nNameW = pTOBJ->m_vNAME.GetWidth();

	pTOBJ->m_vNAMEPOS.x = vPROJ.x + fCamW - FLOAT(nNameW) / 2.0f;
	pTOBJ->m_vNAMEPOS.y = fCamH - vPROJ.y - FLOAT(nNameH);
}

void CTClientGame::ResetObjUserTitlePos( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ)
{
	FLOAT fCamH = m_pMainWnd->m_Camera.m_fHeight / 2.0f;
	FLOAT fCamW = m_pMainWnd->m_Camera.m_fWidth / 2.0f;

	int nTitleH = pTOBJ->m_vUSERTITLE.GetHeight();
	int nTitleW = pTOBJ->m_vUSERTITLE.GetWidth();

	pTOBJ->m_vUSERTITLEPOS.x = vPROJ.x + fCamW - FLOAT(nTitleW) / 2.0f;
	pTOBJ->m_vUSERTITLEPOS.y = fCamH - vPROJ.y - FLOAT(nTitleH);
}

BYTE CTClientGame::HitTestObjName( CTClientObjBase *pTOBJ, const CPoint& pt)
{
	if( !pTOBJ || (!pTOBJ->GetDrawName() && pTOBJ != m_pTARGET) ||
		pTOBJ->m_vNAMEPOS.x == (FLOAT) T_INVALID ||
		pTOBJ->m_vNAMEPOS.y == (FLOAT) T_INVALID)
		return FALSE;
	CRect rcName;

	rcName.left = INT(pTOBJ->m_vNAMEPOS.x);
	rcName.top = INT(pTOBJ->m_vNAMEPOS.y);
	rcName.right = rcName.left + pTOBJ->m_vNAME.GetWidth();
	rcName.bottom = rcName.top + pTOBJ->m_vNAME.GetHeight() + INT(TTEXT_HEIGHT);

	return rcName.PtInRect(pt);
}

BYTE CTClientGame::RenderTICON( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ, FLOAT fBaseHeight, FLOAT fLEFT)
{
	LPDIRECT3DTEXTURE9 pTEX = NULL;

	switch(pTOBJ->m_bType)
	{
	case OT_NPC	: pTEX = m_vTNPCICON[m_vTNPCICONIDX[((CTClientNpc *) pTOBJ)->m_bNPCType]]->GetTexture(); break;
	case OT_PC	:
		{
			CTClientChar *pTCHAR = (CTClientChar *) pTOBJ;

			if( pTCHAR->m_wPartyID && pTCHAR->m_dwID == pTCHAR->m_dwChiefID )
				pTEX = pTCHAR->m_wPartyID == m_pMainChar->m_wCommanderID ? m_pTCOMMANDERICON->GetTexture() : m_pTCHIEFICON->GetTexture();
			else if(pTCHAR->m_pGuildMark)
				pTEX = pTCHAR->m_pGuildMark->GetTexture();
		}

		break;
	}

	if(!pTEX)
		return FALSE;

	D3DXMATRIX vTICON;
	D3DXMATRIX vTROT;

	D3DXMatrixScaling(
		&vTICON,
		TNPCICON_SIZE,
		TNPCICON_SIZE,
		TNPCICON_SIZE);

	D3DXMatrixRotationX(
		&vTROT,
		-D3DX_PI / 2.0f);
	vTICON *= vTROT;

	vTICON._41 = vPROJ.x - fLEFT;
	vTICON._42 = vPROJ.y + fBaseHeight;
	vTICON._43 = vPROJ.z;

	vTICON._41 = (FLOAT) INT(vTICON._41);
	vTICON._42 = (FLOAT) INT(vTICON._42);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00);

	m_pDevice->m_pDevice->SetStreamSource( 0, m_vTRECTVB.GetVB(), 0, sizeof(PVERTEX));
	m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vTICON);
    m_pDevice->m_pDevice->SetTexture(0, pTEX);

	m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	return TRUE;
}

BYTE CTClientGame::RenderTCAMP( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ, FLOAT fBaseHeight, FLOAT fRIGHT)
{
	LPTEXTURESET pTEX = NULL;

	if( pTOBJ->m_bType == OT_PC )
	{
		CTClientChar *pTCHAR = (CTClientChar *) pTOBJ;

		if(pTCHAR->GetCamp())
			pTEX = CTClientMAP::m_pTGodTower[pTCHAR->GetCamp()];
	}

	if(!pTEX)
		return FALSE;

	D3DXMATRIX vTICON;
	D3DXMATRIX vTROT;

	D3DXMatrixScaling(
		&vTICON,
		TNPCICON_SIZE,
		TNPCICON_SIZE,
		TNPCICON_SIZE);

	D3DXMatrixRotationX(
		&vTROT,
		-D3DX_PI / 2.0f);
	vTICON *= vTROT;

	vTICON._41 = vPROJ.x + fRIGHT;
	vTICON._42 = vPROJ.y + fBaseHeight;
	vTICON._43 = vPROJ.z;

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00);

	m_pDevice->m_pDevice->SetStreamSource( 0, m_vTRECTVB.GetVB(), 0, sizeof(PVERTEX));
	m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vTICON);
    m_pDevice->m_pDevice->SetTexture( 0, pTEX->GetTexture());

	m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	return TRUE;
}

BYTE CTClientGame::RenderTSOULMATE( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ, FLOAT fBaseHeight, FLOAT fRIGHT)
{
	LPDIRECT3DTEXTURE9 pTEX = NULL;

	if( pTOBJ->m_bType == OT_PC && pTOBJ->m_dwID == m_dwSoulID && !m_dwSoulRegion )
		pTEX = m_pTSOULMATE->GetTexture();

	if(!pTEX)
		return FALSE;

	D3DXMATRIX vTICON;
	D3DXMATRIX vTROT;

	D3DXMatrixScaling(
		&vTICON,
		TNPCICON_SIZE,
		TNPCICON_SIZE,
		TNPCICON_SIZE);

	D3DXMatrixRotationX(
		&vTROT,
		-D3DX_PI / 2.0f);
	vTICON *= vTROT;

	vTICON._41 = vPROJ.x + fRIGHT;
	vTICON._42 = vPROJ.y + fBaseHeight;
	vTICON._43 = vPROJ.z;

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00);

	m_pDevice->m_pDevice->SetStreamSource( 0, m_vTRECTVB.GetVB(), 0, sizeof(PVERTEX));
	m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vTICON);
    m_pDevice->m_pDevice->SetTexture(0, pTEX);

	m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	return TRUE;
}

BYTE CTClientGame::RenderTSELMARK( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ, FLOAT fBaseHeight, FLOAT fLEFT, FLOAT fRIGHT)
{
	if(!m_pTSELECTMARK)
		return FALSE;

	D3DXMATRIX vTICON;
	D3DXMATRIX vTROT;

	D3DXMatrixScaling(
		&vTICON,
		TNPCICON_SIZE,
		TNPCICON_SIZE,
		TNPCICON_SIZE);

	D3DXMatrixRotationX(
		&vTROT,
		-D3DX_PI / 2.0f);
	vTICON *= vTROT;

	vTICON._42 = vPROJ.y + fBaseHeight;
	vTICON._43 = vPROJ.z;

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHAREF, 0x00);

	m_pDevice->m_pDevice->SetStreamSource( 0, m_vTRECTVB.GetVB(), 0, sizeof(PVERTEX));
	m_pDevice->m_pDevice->SetTexture(0, m_pTSELECTMARK->GetTexture());

	vTICON._41 = vPROJ.x - fLEFT;
	m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vTICON);
	m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	vTICON._41 = vPROJ.x + fRIGHT;
	m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vTICON);
	m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	return TRUE;
}

void CTClientGame::RenderTSQUARE( CTClientObjBase *pTOBJ,
								  LPD3DXMATRIX pUV,
								  LPD3DXVECTOR2 pDIR,
								  FLOAT fSCALE)
{
	if(!pTOBJ->m_bDynamicOBJ)
		return;
	BYTE bLand = pTOBJ->m_bLand && m_vMAP.m_pMAP ? TRUE : FALSE;

	RenderTSQUARE(
		&D3DXVECTOR3(
		pTOBJ->GetPositionX(),
		bLand ? pTOBJ->GetPositionY() : pTOBJ->m_fSquareHeight,
		pTOBJ->GetPositionZ()),
		&D3DXVECTOR2(
		pTOBJ->m_fSizeX,
		pTOBJ->m_fSizeZ),
		pUV, pDIR,
		fSCALE,
		bLand);
}

void CTClientGame::RenderTSQUARE( LPD3DXVECTOR3 pPOS,
								  LPD3DXVECTOR2 pSIZE,
								  LPD3DXMATRIX pUV,
								  LPD3DXVECTOR2 pDIR,
								  FLOAT fSCALE,
								  BYTE bLand)
{
	D3DXMATRIX vMAT(
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

	if(bLand)
	{
		FLOAT fSIZE = fSCALE * max( pSIZE->x, pSIZE->y);

		if( fSIZE > INT(fSIZE) )
			fSIZE = FLOAT(INT(fSIZE) + 1);
		else
			fSIZE = FLOAT(INT(fSIZE));
		fSIZE /= FLOAT(m_vMAP.m_pMAP->m_nTileLength);

		BYTE bTILE = BYTE(fSIZE) + 1;
		if(!(bTILE % 2))
			bTILE++;

		FLOAT fPosX = 2.0f * FLOAT(INT(pPOS->x) / 2) - m_vMAP.m_pMAP->m_nTileLength * (bTILE / 2);
		FLOAT fPosZ = 2.0f * FLOAT(INT(pPOS->z) / 2) - m_vMAP.m_pMAP->m_nTileLength * (bTILE / 2);

		m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vMAT);

		vMAT._11 = FLOAT(bTILE * m_vMAP.m_pMAP->m_nTileLength) / (fSCALE * pSIZE->x);
		vMAT._22 = FLOAT(bTILE * m_vMAP.m_pMAP->m_nTileLength) / (fSCALE * pSIZE->y);
		vMAT._31 = vMAT._11 * (fPosX - pPOS->x) / FLOAT(bTILE * m_vMAP.m_pMAP->m_nTileLength) + 0.5f;
		vMAT._32 = vMAT._22 * (fPosZ - pPOS->z) / FLOAT(bTILE * m_vMAP.m_pMAP->m_nTileLength) + 0.5f;
		vMAT = (*pUV) * vMAT;

		if(pDIR)
		{
			D3DXMATRIX vROT;

			D3DXMatrixTransformation2D(
				&vROT,
				NULL,
				0.0f,
				NULL,
				&D3DXVECTOR2( 0.5f, 0.5f),
				atan2f( pDIR->x, pDIR->y) + D3DX_PI,
				NULL);

			vROT._31 = vROT._41;
			vROT._32 = vROT._42;
			vROT._41 = 0.0f;
			vROT._42 = 0.0f;

			m_pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &(vMAT * vROT));
		}
		else
			m_pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, &vMAT);

		for( BYTE i=0; i<bTILE; i++)
		{
			fPosX = 2.0f * FLOAT(INT(pPOS->x) / 2) - m_vMAP.m_pMAP->m_nTileLength * (bTILE / 2);

			for( BYTE j=0; j<bTILE; j++)
			{
				PVERTEX vDATA[4] = {
					{								 fPosX, m_vMAP.m_pMAP->GetHeight(								 fPosX, pPOS->y,								fPosZ),								   fPosZ,	  FLOAT(j) / FLOAT(bTILE),	   FLOAT(i) / FLOAT(bTILE)},
					{								 fPosX, m_vMAP.m_pMAP->GetHeight(								 fPosX, pPOS->y, fPosZ + m_vMAP.m_pMAP->m_nTileLength), fPosZ + m_vMAP.m_pMAP->m_nTileLength,	  FLOAT(j) / FLOAT(bTILE), FLOAT(i + 1) / FLOAT(bTILE)},
					{ fPosX + m_vMAP.m_pMAP->m_nTileLength, m_vMAP.m_pMAP->GetHeight( fPosX + m_vMAP.m_pMAP->m_nTileLength, pPOS->y,								fPosZ),								   fPosZ, FLOAT(j + 1) / FLOAT(bTILE),	   FLOAT(i) / FLOAT(bTILE)},
					{ fPosX + m_vMAP.m_pMAP->m_nTileLength, m_vMAP.m_pMAP->GetHeight( fPosX + m_vMAP.m_pMAP->m_nTileLength, pPOS->y, fPosZ + m_vMAP.m_pMAP->m_nTileLength), fPosZ + m_vMAP.m_pMAP->m_nTileLength, FLOAT(j + 1) / FLOAT(bTILE), FLOAT(i + 1) / FLOAT(bTILE)}};

				m_pDevice->m_pDevice->DrawPrimitiveUP(
					D3DPT_TRIANGLESTRIP, 2,
					vDATA,
					sizeof(PVERTEX));
				fPosX += m_vMAP.m_pMAP->m_nTileLength;
			}

			fPosZ += m_vMAP.m_pMAP->m_nTileLength;
		}
	}
	else
	{
		m_pDevice->m_pDevice->SetTransform( D3DTS_TEXTURE0, pUV);

		D3DXMatrixScaling(
			&vMAT,
			min( 2.0f, fSCALE * pSIZE->x),
			1.0f,
			min( 2.0f, fSCALE * pSIZE->y));

		if(pDIR)
		{
			D3DXMATRIX vROT;

			D3DXMatrixRotationY(
				&vROT,
				atan2f( pDIR->x, pDIR->y) + D3DX_PI);
			vMAT *= vROT;
		}

		vMAT._41 = pPOS->x;
		vMAT._42 = pPOS->y;
		vMAT._43 = pPOS->z;

		m_pDevice->m_pDevice->SetStreamSource( 0, CTClientObjBase::m_vTSQUARE.GetVB(), 0, sizeof(PVERTEX));
		m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vMAT);
		m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	}
}

void CTClientGame::RenderTNUMBER()
{
	if(!m_vTNUMBER.empty())
	{
		CTClientNumber::BeginDraw(
			m_pDevice,
			m_pTextCAM);

		for( int i=0; i<INT(m_vTNUMBER.size()); i++)
			m_vTNUMBER[i]->Render( m_pDevice, &m_pCamera->m_vPROJ);

		CTClientNumber::EndDraw(m_pCamera);
	}
}

void CTClientGame::RenderFLASH()
{
	static D3DXMATRIX vWORLD(
		 1.0f, 0.0f, 0.0f, 0.0f,
		 0.0f, 1.0f, 0.0f, 0.0f,
		 0.0f, 0.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f);
	CD3DCamera vCAM;

	vCAM.InitOrthoCamera(
		m_pDevice->m_pDevice,
		-1.0f,
		1.0f,
		1.0f,
		1.0f);

	vCAM.SetPosition(
		D3DXVECTOR3( 0.0f, 0.5f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
		FALSE);
	vCAM.Activate(TRUE);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TFACTOR);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);
	m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

	DWORD dwFlashColor = (m_pMainChar->m_dwFlashColor | (DWORD(m_dwFLASHTick * 0xFF / TFLASH_TICK) << 24));
	m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwFlashColor);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);

	m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWORLD);
	m_pDevice->m_pDevice->SetVertexShader(NULL);
	m_pDevice->m_pDevice->SetPixelShader(NULL);

	m_pDevice->m_pDevice->SetStreamSource( 0, m_vTRECTVB.GetVB(), 0, sizeof(PVERTEX));
	m_pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);

	m_pDevice->m_pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,
		0, 2);
	m_pCamera->Activate(TRUE);
}

void CTClientGame::RenderTCMDDIR()
{
	if( m_vTCMDDIR.m_bHide || !m_vTCMDDIR.m_bAlpha )
		return;

	CD3DCamera vCAM;

	vCAM.InitOrthoCamera(
		m_pDevice->m_pDevice,
		-10.0f, 10.0f,
		10.0f * FLOAT(m_pDevice->m_option.m_dwScreenX) /
		FLOAT(m_pDevice->m_option.m_dwScreenY), 10.0f);

	vCAM.SetPosition(
		D3DXVECTOR3( 0.0f, 0.0f, -1.0f),
		D3DXVECTOR3( 0.0f, 0.0f,  0.0f),
		D3DXVECTOR3( 0.0f, 1.0f,  0.0f),
		FALSE);

	((CTCharInfoDlg *) m_vTFRAME[TFRAME_CHAR_INFO])->EnableTLIGHT( &vCAM, TRUE);
	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);

	SetMaterial(&m_vTCMDDIR);
	vCAM.Activate(TRUE);

	if( m_vTCMDDIR.m_bAlpha < ALPHA_MAX )
	{
		m_pDevice->m_pDevice->SetRenderState( D3DRS_COLORWRITEENABLE, 0);
		m_vTCMDDIR.Render( m_pDevice, &vCAM);
		m_vTCMDDIR.m_bZWrite = FALSE;

		m_pDevice->m_pDevice->SetRenderState(
			D3DRS_COLORWRITEENABLE,
			D3DCOLORWRITEENABLE_ALPHA|
			D3DCOLORWRITEENABLE_RED|
			D3DCOLORWRITEENABLE_GREEN|
			D3DCOLORWRITEENABLE_BLUE);
	}

	m_vTCMDDIR.Render( m_pDevice, &vCAM);
	m_vTCMDDIR.m_bZWrite = TRUE;

	((CTCharInfoDlg *) m_vTFRAME[TFRAME_CHAR_INFO])->EnableTLIGHT( &vCAM, FALSE);
	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);

	m_pCamera->Activate(TRUE);
	SetMaterial(NULL);
}

void CTClientGame::RenderARROW()
{
	CTachyonMesh::BeginGlobalDraw(m_pDevice->m_pDevice);
	for( int i=0; i<INT(m_vTARROW.size()); i++)
		m_vTARROW[i]->Render( m_pDevice, m_pCamera);
	CTachyonMesh::EndGlobalDraw(m_pDevice->m_pDevice);
}

void CTClientGame::CalcBREATH( DWORD dwTick)
{
	static DWORD dwDamageTick = 0;
	DWORD dwBreath = m_pMainChar->IsDead() || m_pMainChar->m_bGhost ? TBREATH_TICK : m_vTGAUGE[TGAUGE_BREATH]->GetCurValue();

	CTClientMoveObj* pTOBJ = m_pMainChar->GetRidingPet();
	if( !pTOBJ )
		pTOBJ = m_pMainChar;

	if( pTOBJ->m_bDive && dwBreath < dwTick )
	{
		dwDamageTick += dwTick - dwBreath;
		dwBreath = 0;

		if( dwDamageTick >= TBREATH_TIMER )
		{
			SendCS_DROPDAMAGE_REQ(
				m_pMainChar->m_dwID,
				m_pMainChar->m_bType,
				TBREATH_DAMAGE * m_pMainChar->m_dwMaxHP / 100 + TBREATH_DAMAGE,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID);

			dwDamageTick -= TBREATH_TIMER;
		}
	}
	else
	{
		if( !pTOBJ->m_bDive )
			dwBreath = min( dwBreath + TBREATH_FACTOR * dwTick, TBREATH_TICK);
		else
			dwBreath -= dwTick;

		dwDamageTick = 0;
	}
	
	BYTE bShow = pTOBJ->m_bDive || dwBreath < TBREATH_TICK ? TRUE : FALSE;

	m_vTGAUGE[TGAUGE_BREATH]->SetGauge(
		dwBreath,
		TBREATH_TICK,
		FALSE);

	m_vTGAUGE[TGAUGE_BREATH]->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		dwBreath * 100 / TBREATH_TICK);

	if( m_vTGAUGE[TGAUGE_BREATH]->IsVisible() != bShow )
		m_vTGAUGE[TGAUGE_BREATH]->ShowComponent(bShow);
}

void CTClientGame::CalcCHARGE( DWORD dwTick)
{
	CTClientSkill *pTSKILL = NULL;

	if( IsTSKILLCharge() )
		pTSKILL = m_pMainChar->FindTSkill(m_pMainChar->m_vTSKILLDATA.m_wID);

	if( m_bCHARGE && (!pTSKILL || pTSKILL->m_pTSKILL->m_bRangeType != TSKILLRANGE_MCIRCLE) )
	{
		D3DXVECTOR3 vTGROUND = D3DXVECTOR3(
			m_vTGROUND.x - m_pMainChar->GetPositionX(),
			m_vTGROUND.y - m_pMainChar->GetPositionY(),
			m_vTGROUND.z - m_pMainChar->GetPositionZ());

		D3DXVECTOR3 vTARGET = m_pACT ? D3DXVECTOR3(
			m_pACT->GetPositionX() - m_pMainChar->GetPositionX(),
			m_pACT->GetPositionY() - m_pMainChar->GetPositionY(),
			m_pACT->GetPositionZ() - m_pMainChar->GetPositionZ()) : vTGROUND;

		if(pTSKILL)
		{
			CTClientObjBase *pTARGET = NULL;

			if( pTSKILL->m_pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE ||
				pTSKILL->m_pTSKILL->m_bRangeType == TSKILLRANGE_POINT )
				vTARGET = vTGROUND;
			else if(m_pACT)
				pTARGET = m_pACT;
			FLOAT fDIST = D3DXVec3Length(&vTARGET);

			if( fDIST < m_pMainChar->GetMinRange( pTARGET, pTSKILL->m_pTSKILL) || fDIST > m_pMainChar->GetMaxRange( pTARGET, pTSKILL->m_pTSKILL) )
			{
				SendCS_ACTION_REQ(
					m_pMainChar->m_dwID,
					m_pMainChar->m_bType,
					0, 0, 0,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID, 0);

				CancelCharge(TRUE);
			}
		}

		if( m_bCHARGE && D3DXVec2Length(&D3DXVECTOR2( vTARGET.x, vTARGET.z)) > 0.0f )
		{
			WORD wPITCH = m_pMainChar->GetTargetPITCH(
				vTARGET.x,
				vTARGET.y,
				vTARGET.z);

			WORD wDIR = m_pMainChar->GetTargetDIR(
				vTARGET.x,
				vTARGET.z);

			int nDIR = INT(wDIR) - INT(m_pMainChar->m_wDIR);
			while(nDIR < 0)
				nDIR += 1800;
			nDIR %= 1800;

			if( nDIR > 900 )
				nDIR = 1800 - nDIR;

			if( nDIR > FRONT_DIR_RANGE && !CanLookAt() )
			{
				SendCS_ACTION_REQ(
					m_pMainChar->m_dwID,
					m_pMainChar->m_bType,
					0, 0, 0,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID, 0);

				CancelCharge(TRUE);
			}
			else if( nDIR > FRONT_DIR_RANGE )
				LookAt( wDIR, wPITCH);
		}
	}

	if(m_bCHARGE)
	{
		m_dwChargeTick += dwTick;

		if( m_dwChargeTick >= m_dwMaxChargeTick )
			if(m_bLOOPCHARGE)
			{
				m_dwChargeTick -= m_dwMaxChargeTick;

				if(pTSKILL)
				{
					SendCS_LOOPSKILL_REQ(
						m_pMainChar,
						m_pACT,
						&m_pMainChar->m_vTSKILLDATA.m_vTGROUND,
						pTSKILL->m_pTSKILL);
				}
			}
			else
			{
				m_vTGAUGE[TGAUGE_CHARGE]->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
					100);

				m_vTGAUGE[TGAUGE_CHARGE]->SetGauge(
					100,
					100,
					FALSE);

				if(m_pACT)
					switch(m_pACT->m_bType)
					{
					case OT_SWITCH	:
						if( (!((CTClientSwitch *) m_pACT)->m_pTSWITCH->m_bLockOnClose || ((CTClientSwitch *) m_pACT)->m_bOPEN) &&
							(!((CTClientSwitch *) m_pACT)->m_pTSWITCH->m_bLockOnOpen || !((CTClientSwitch *) m_pACT)->m_bOPEN) )
							m_pSESSION->SendCS_SWITCHCHANGE_REQ(m_pACT->m_dwID);

						break;

					case OT_NPC		:
						{
							MAPTQUESTTEMP::iterator itTQUEST;

							for( itTQUEST = m_pACT->m_mapTQUESTTEMP.begin(); itTQUEST != m_pACT->m_mapTQUESTTEMP.end(); itTQUEST++)
								if( (*itTQUEST).second->m_bType == QT_ROUTING )
								{
									m_pSESSION->SendCS_QUESTEXEC_REQ(
										(*itTQUEST).second->m_dwID,
										0, 0);

									break;
								}
						}

						break;

					case OT_GODTOWER	:
						SiegeMgr->ChargeAffterProcess( static_cast< CTGodTower* >( m_pACT ) );
						break;

					default			:
						if(m_pMainChar->m_vTSKILLDATA.m_wID)
							UseTSkill( m_pMainChar->m_vTSKILLDATA.m_wID, TRUE, FALSE);

						break;
					}
				else if(m_pMainChar->m_vTSKILLDATA.m_wID)
					UseTSkill( m_pMainChar->m_vTSKILLDATA.m_wID, TRUE, FALSE);

				CancelCharge(TRUE);
			}
		else if(!m_bLOOPCHARGE)
		{
			m_vTGAUGE[TGAUGE_CHARGE]->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
				m_dwChargeTick * 100 / m_dwMaxChargeTick);

			m_vTGAUGE[TGAUGE_CHARGE]->SetGauge(
				m_dwChargeTick,
				m_dwMaxChargeTick,
				FALSE);
		}
	}
	else if(m_vTGAUGE[TGAUGE_CHARGE]->IsVisible())
		m_vTGAUGE[TGAUGE_CHARGE]->ShowComponent(FALSE);
}

void CTClientGame::CalcReuseTick( DWORD dwTick)
{
	MAPRECALL::iterator itTRECALL;
	MAPTSKILL::iterator itTSKILL;

	for( itTSKILL = m_pMainChar->m_mapTSKILL.begin(); itTSKILL != m_pMainChar->m_mapTSKILL.end(); itTSKILL++)
	{
		CTClientSkill *pTSKILL = (*itTSKILL).second;

		if( pTSKILL->m_dwTick && pTSKILL->m_bTimerON )
		{
			pTSKILL->m_dwTick = pTSKILL->m_dwTick > dwTick ? pTSKILL->m_dwTick - dwTick : 0;

			if(!pTSKILL->m_dwTick)
				pTSKILL->m_bTimerON = FALSE;
		}

		if(pTSKILL->m_dwExceptTick)
			pTSKILL->m_dwExceptTick -= min( pTSKILL->m_dwExceptTick, dwTick);

		if(m_pMainChar->IsReadyForUSE(pTSKILL))
			LoopTSKill(pTSKILL);
	}

	MAPDWORD::iterator itTICK = CTClientItem::m_mapTITEMTICK.begin();
	while(itTICK != CTClientItem::m_mapTITEMTICK.end())
	{
		MAPDWORD::iterator itNEXT = itTICK;
		itNEXT++;

		(*itTICK).second -= min( (*itTICK).second, dwTick);
		if(!(*itTICK).second)
			CTClientItem::m_mapTITEMTICK.erase(itTICK);

		itTICK = itNEXT;
	}

	for( itTRECALL = m_mapSLAVEFIXRECALL.begin(); itTRECALL != m_mapSLAVEFIXRECALL.end(); itTRECALL++)
		for( itTSKILL = (*itTRECALL).second->m_mapTSKILL.begin(); itTSKILL != (*itTRECALL).second->m_mapTSKILL.end(); itTSKILL++)
		{
			if( (*itTSKILL).second->m_dwTick && (*itTSKILL).second->m_bTimerON )
			{
				(*itTSKILL).second->m_dwTick = (*itTSKILL).second->m_dwTick > dwTick ? (*itTSKILL).second->m_dwTick - dwTick : 0;

				if(!(*itTSKILL).second->m_dwTick)
					(*itTSKILL).second->m_bTimerON = FALSE;
			}

			if((*itTSKILL).second->m_dwExceptTick)
				(*itTSKILL).second->m_dwExceptTick -= min( (*itTSKILL).second->m_dwExceptTick, dwTick);
		}

	for( itTRECALL = m_mapSLAVERECALL.begin(); itTRECALL != m_mapSLAVERECALL.end(); itTRECALL++)
		for( itTSKILL = (*itTRECALL).second->m_mapTSKILL.begin(); itTSKILL != (*itTRECALL).second->m_mapTSKILL.end(); itTSKILL++)
		{
			if( (*itTSKILL).second->m_dwTick && (*itTSKILL).second->m_bTimerON )
			{
				(*itTSKILL).second->m_dwTick = (*itTSKILL).second->m_dwTick > dwTick ? (*itTSKILL).second->m_dwTick - dwTick : 0;

				if(!(*itTSKILL).second->m_dwTick)
					(*itTSKILL).second->m_bTimerON = FALSE;
			}

			if((*itTSKILL).second->m_dwExceptTick)
				(*itTSKILL).second->m_dwExceptTick -= min( (*itTSKILL).second->m_dwExceptTick, dwTick);
		}

	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
	pTGaugePannel->CalcRecallTick(GetMainRecall());
	pTGaugePannel->CalcHotkeyTick();
	pTGaugePannel->CalcCaution15Tick(dwTick);
}

void CTClientGame::CalcFollowMove( DWORD dwTick)
{
	if( !CanFollow(m_pTARGET) )
	{
		m_bMoveType = TMAINMOVE_NORMAL;

		m_dwMoveGM = 0;
		CalcMainChar(dwTick);

		return;
	}

	D3DXVECTOR3 vTARGET = D3DXVECTOR3(
		m_pTARGET->GetPositionX() - m_pCtrlHost->GetPositionX(),
		m_pTARGET->GetPositionY() - m_pCtrlHost->GetPositionY(),
		m_pTARGET->GetPositionZ() - m_pCtrlHost->GetPositionZ());

	if( D3DXVec3Length(&vTARGET) < m_fBOUND )
	{
		m_bMoveType = TMAINMOVE_NORMAL;
		CalcMainChar(dwTick);
		m_bMoveType = TMAINMOVE_FOLLOW;
		return;
	}

	m_pCtrlHost->CancelTAction();
	if(m_bCHARGE)
		CancelCharge(TRUE);

	WORD wPITCH = m_pCtrlHost->GetTargetPITCH(
		vTARGET.x,
		vTARGET.y,
		vTARGET.z);

	WORD wDIR = m_pCtrlHost->GetTargetDIR(
		vTARGET.x,
		vTARGET.z);

	if( m_pCtrlHost->CheckDIR( wDIR, wPITCH) || m_pCtrlHost->m_bKeyDIR == TKDIR_N )
	{
		m_pCtrlHost->m_bMouseDIR = TKDIR_F;
		m_pCtrlHost->m_bKeyDIR = TKDIR_F;
		m_pCtrlHost->SetTAction(m_pCtrlHost->GetTAction());

		m_dwSYNCTick = SYNC_TICK;
		MoveCtrlHost(
			wPITCH,
			wDIR,
			m_pCtrlHost->m_bMouseDIR,
			m_pCtrlHost->m_bKeyDIR);
		m_pCtrlHost->CalcDIR( wDIR, wDIR, wPITCH);

		if( GetPartyID(m_pMainChar) && CanControl() )
			m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
	}
}

void CTClientGame::CalcTargetMove( DWORD dwTick)
{
	m_pCtrlHost->CancelTAction();
	if(m_bCHARGE)
		CancelCharge(TRUE);

	if( m_pMainChar->IsDead() || m_pMainChar->IsBlock() || m_pMainChar->IsDown() || (!m_pTARGET && m_bMoveType == TMAINMOVE_TARGET) )
	{
		m_bMoveType = TMAINMOVE_NORMAL;

		m_dwMoveGM = 0;
		CalcMainChar(dwTick);

		return;
	}

	D3DXVECTOR3 vTARGET = m_bMoveType == TMAINMOVE_TARGET ? D3DXVECTOR3(
		m_pTARGET->GetPositionX() - m_pCtrlHost->GetPositionX(),
		m_pTARGET->GetPositionY() - m_pCtrlHost->GetPositionY(),
		m_pTARGET->GetPositionZ() - m_pCtrlHost->GetPositionZ()) : D3DXVECTOR3(
		m_vTGROUND.x - m_pCtrlHost->GetPositionX(),
		m_vTGROUND.y - m_pCtrlHost->GetPositionY(),
		m_vTGROUND.z - m_pCtrlHost->GetPositionZ());

	if( D3DXVec3Length(&vTARGET) < m_fBOUND )
	{
		DWORD dwMoveGM = m_dwMoveGM;

		CalcMainChar(dwTick);
		m_bMoveType = TMAINMOVE_NORMAL;
		m_dwMoveGM = 0;

		if(CanControl())
			m_pMainWnd->OnCommand(dwMoveGM);

		return;
	}

	WORD wPITCH = m_pCtrlHost->GetTargetPITCH(
		vTARGET.x,
		vTARGET.y,
		vTARGET.z);

	WORD wDIR = m_pCtrlHost->GetTargetDIR(
		vTARGET.x,
		vTARGET.z);

	if( wDIR == MOVE_NONE )
	{
		CTClientObjBase::m_mapTSELECTOBJ.clear();
		CTClientObjBase::m_mapTSELECTDEADOBJ.clear();
		ResetTargetOBJ(NULL);
		ResetTargetINFO(FALSE);
		CalcMainChar(dwTick);

		return;
	}

	if( m_pCtrlHost->CheckDIR( wDIR, wPITCH) || m_pCtrlHost->m_bKeyDIR == TKDIR_N )
	{
		m_pCtrlHost->m_bMouseDIR = TKDIR_F;
		m_pCtrlHost->m_bKeyDIR = TKDIR_F;
		m_pCtrlHost->SetTAction(m_pCtrlHost->GetTAction());

		m_dwSYNCTick = SYNC_TICK;
		MoveCtrlHost(
			wPITCH,
			wDIR,
			m_pCtrlHost->m_bMouseDIR,
			m_pCtrlHost->m_bKeyDIR);
		m_pCtrlHost->CalcDIR( wDIR, wDIR, wPITCH);

		if( GetPartyID(m_pMainChar) && CanControl() )
			m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
	}
}

void CTClientGame::CalcClickMove( DWORD dwTick)
{
	D3DXVECTOR3 vPOS(
		m_pCtrlHost->GetPositionX(),
		m_pCtrlHost->GetPositionY(),
		m_pCtrlHost->GetPositionZ());

	m_pCtrlHost->CancelTAction();
	if(m_bCHARGE)
		CancelCharge(TRUE);

	if( D3DXPlaneDotCoord( &m_vBOUND, &vPOS) < 0.0f || m_pMainChar->IsDead() ||
		m_pMainChar->IsBlock() || m_pMainChar->IsDown() )
	{
		m_bMoveType = TMAINMOVE_NORMAL;
		m_dwMoveGM = 0;
		CalcMainChar(dwTick);
	}
}

void CTClientGame::CalcMainChar( DWORD dwTick)
{
	static DWORD dwPrevCell = 0;

	static int nPITCH = 0;
	static int nDIR = 0;

	int nDirX = 1;
	int nDirZ = 1;

	if( !m_bMainCharMoveLock &&
		!m_pMainChar->IsDead() && !m_pMainChar->IsBlock() && !m_pMainChar->IsDown() && !m_pMainChar->m_bPrivateShop )
	{
		if( m_vKEY.m_vSTATE[TKEY_FORWARD] || m_bAutoRun )
			nDirZ--;

		if(m_vKEY.m_vSTATE[TKEY_BACK])
			nDirZ++;

		if( m_vKEY.m_vSTATE[TKEY_RIGHT] || m_vKEY.m_vSTATE[TKEY_RSIDE] )
			nDirX++;

		if( m_vKEY.m_vSTATE[TKEY_LEFT] || m_vKEY.m_vSTATE[TKEY_LSIDE] )
			nDirX--;
	}

	nPITCH += m_pCtrlHost->m_nPITCH;
	nDIR += m_pCtrlHost->m_nDIR;

	BYTE bSIDE = m_vKEY.m_vSTATE[TKEY_ROT] || m_vKEY.m_vSTATE[TKEY_LSIDE] || m_vKEY.m_vSTATE[TKEY_RSIDE] ? TRUE : FALSE;
	BYTE bMouseDIR = nDirZ * 3 + (bSIDE ? nDirX : 1);
	BYTE bKeyDIR = nDirZ * 3 + (bSIDE ? 1 : nDirX);

	if( bMouseDIR != TKDIR_N )
		m_pCtrlHost->CancelTAction();

	BYTE bAction = m_pCtrlHost->GetTAction(bMouseDIR);
	DWORD dwCell = MAKELONG(
		WORD(m_pCtrlHost->GetPositionX() / CELL_SIZE),
		WORD(m_pCtrlHost->GetPositionZ() / CELL_SIZE));
	WORD wSquadID = GetPartyID(m_pMainChar);
	FLOAT fSyncDist = D3DXVec3Length( &(m_pMainChar->m_vSyncLastPosition - m_pMainChar->GetPosition()) );

	if( abs(nDIR) > DIR_RANGE ||
		abs(nPITCH) > DIR_RANGE ||
		bMouseDIR != m_pCtrlHost->m_bMouseDIR ||
		bKeyDIR != m_pCtrlHost->m_bKeyDIR ||
		bAction != m_pCtrlHost->m_bAction ||
		dwPrevCell != dwCell ||
		m_dwSYNCTick > SYNC_TICK ||
		fSyncDist > SYNC_DIST )
	{
		int nNextPITCH = INT(m_pCtrlHost->m_wPITCH) + m_pCtrlHost->m_nPITCH;
		int nNextDIR = INT(m_pCtrlHost->m_wDIR) + m_pCtrlHost->m_nDIR;

		if( bAction != m_pCtrlHost->m_bAction )
		{
			m_pCtrlHost->CancelTAction();

			if(m_bCHARGE)
				CancelCharge(TRUE);
		}

		if( bMouseDIR != m_pCtrlHost->m_bMouseDIR ||
			bKeyDIR != m_pCtrlHost->m_bKeyDIR ||
			bAction != m_pCtrlHost->m_bAction ||
			dwPrevCell != dwCell )
			m_dwSYNCTick = SYNC_TICK;

		while(nNextPITCH < 0)
			nNextPITCH += 1800;
		nNextPITCH %= 1800;

		while(nNextDIR < 0)
			nNextDIR += 1800;
		nNextDIR %= 1800;

		m_pCtrlHost->SetTAction(bAction);
		dwPrevCell = dwCell;

		nPITCH = 0;
		nDIR = 0;

		MoveCtrlHost(
			WORD(nNextPITCH),
			WORD(nNextDIR),
			bMouseDIR,
			bKeyDIR);
	}
	else
		wSquadID = 0;

	m_pCtrlHost->CalcDIR(
		dwTick,
		bMouseDIR,
		bKeyDIR);

	if( wSquadID && CanControl() )
		m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
}

FLOAT CTClientGame::CalcCamera( DWORD dwTick)
{
	int nCamRotX = 0;
	int nCamRotY = 0;
	int nCamZoom = 0;

	if(m_vKEY.m_vSTATE[TKEY_CAM_RIGHT])
		nCamRotY--;

	if(m_vKEY.m_vSTATE[TKEY_CAM_LEFT])
		nCamRotY++;

	if(m_vKEY.m_vSTATE[TKEY_CAM_DOWN])
		nCamRotX--;

	if(m_vKEY.m_vSTATE[TKEY_CAM_UP])
		nCamRotX++;

	if(m_vKEY.m_vSTATE[TKEY_ZOOM_IN])
		nCamZoom--;

	if(m_vKEY.m_vSTATE[TKEY_ZOOM_OUT])
		nCamZoom++;

	RotCameraX(nCamRotX * INT(dwTick * m_pCamera->m_bCamRotSPEED));
	RotCameraY(nCamRotY * INT(dwTick * m_pCamera->m_bCamRotSPEED));

	if( !nCamZoom && m_nCamZOOM )
	{
		DWORD dwZoomTick = abs(m_nCamZOOM);

		if( dwZoomTick > dwTick )
			dwZoomTick -= dwTick;

		nCamZoom = m_nCamZOOM < 0 ? -1 : 1;
		m_pCamera->ZoomCamera(
			dwZoomTick,
			nCamZoom);

		m_nCamZOOM -= nCamZoom * INT(dwZoomTick);
	}
	else
	{
		m_pCamera->ZoomCamera(
			dwTick,
			nCamZoom);

		m_nCamZOOM = 0;
	}

	if( !nCamRotX && !nCamRotY && !m_vKEY.m_vSTATE[TKEY_CAM_ROT] )
		FixCamera(dwTick);
	m_bCamFIX = m_pCtrlHost->m_wDIR == m_wCamDIR ? TRUE : FALSE;

	if( m_pCamera->m_fPrevDIST < m_pCamera->m_fCamDIST )
		m_pCamera->m_fPrevDIST = min( m_pCamera->m_fPrevDIST + TCAMOUT_SPEED * FLOAT(dwTick), m_pCamera->m_fCamDIST);
	else
		m_pCamera->m_fPrevDIST = max( m_pCamera->m_fPrevDIST - TCAMOUT_SPEED * FLOAT(dwTick), m_pCamera->m_fCamDIST);

	m_pCamera->SetPosition(
		D3DXVECTOR3( 0.0f, 0.0f, max( m_pCamera->m_fCamDIST, m_pCamera->m_fPrevDIST)),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 1.0f, 0.0f));

	FLOAT fPitch = -FLOAT(m_pCamera->m_wCamPITCH) * D3DX_PI / 900.0f;
	FLOAT fYaw = FLOAT(m_wCamDIR) * D3DX_PI / 900.0f;

	m_pCamera->Rotate( fYaw, 0.0f, 0.0f, TRUE);
	m_pCamera->Rotate( 0.0f, fPitch, 0.0f, FALSE);

	D3DXVECTOR3 vCAMPOS(
		m_pCtrlHost->GetPositionX() - m_pCamera->m_vTarget.x,
		m_pCtrlHost->GetPositionY() - m_pCamera->m_vTarget.y + m_pCamera->m_fCamHEIGHT,
		m_pCtrlHost->GetPositionZ() - m_pCamera->m_vTarget.z);

	vCAMPOS += m_pCamera->m_vTOFFSET;
	m_pCamera->Move(
		vCAMPOS.x,
		vCAMPOS.y,
		vCAMPOS.z,
		TRUE);
	FLOAT fWaterHeight = CheckCamPos(dwTick);

	m_pCamera->m_vPROJ = m_pCamera->m_matView * m_pCamera->m_matProjection * m_pTextCAM->m_vPROJ;
	CD3DSound::ResetLISTENER(
		&m_pCamera->m_vPosition,
		&(m_pCamera->m_vTarget - m_pCamera->m_vPosition),
		&m_pCamera->m_vUp);

	if(m_bQUAKE)
	{
		FLOAT fQUAKE = m_bQUAKE == TQUAKE_DEFEND ? TDEFQUAKE_DIST : TATKQUAKE_DIST;

		FLOAT fQuakeX = (rand() % 2 ? 1.0f : -1.0f) * FLOAT(rand() % 100) * fQUAKE / 100.0f;
		FLOAT fQuakeY = (rand() % 2 ? 1.0f : -1.0f) * FLOAT(rand() % 100) * fQUAKE / 100.0f;

		m_pCamera->Move( fQuakeX, 0.0f, 0.0f, FALSE);
		m_pCamera->Move( 0.0f, fQuakeY, 0.0f, TRUE);
		m_dwQUAKETick -= min( m_dwQUAKETick, dwTick);

		if(!m_dwQUAKETick)
			EndQUAKE();
	}

	return fWaterHeight;
}

void CTClientGame::DoQUAKE( BYTE bQUAKE)
{
	m_bQUAKE = max( m_bQUAKE, bQUAKE);
	m_bQUAKE = min( m_bQUAKE, TQUAKE_ATTACK);
	m_dwQUAKETick = TQUAKE_TICK;

	if( m_bQUAKE == TQUAKE_ATTACK )
		PlayUISound(ID_SND_SPAWN_01);
}

void CTClientGame::EndQUAKE()
{
	m_bQUAKE = TQUAKE_NONE;
	m_dwQUAKETick = 0;
}

BYTE CTClientGame::FixCamera( DWORD dwTick)
{
	BYTE bResult = m_wCamDIR == m_pCtrlHost->m_wDIR && !m_vKEY.m_vSTATE[TKEY_ROT] ? FALSE : TRUE;

	if( m_pCtrlHost->m_wMoveDIR == MOVE_NONE &&
		m_pCtrlHost->m_bKeyDIR != TKDIR_L &&
		m_pCtrlHost->m_bKeyDIR != TKDIR_R &&
		!m_vKEY.m_vSTATE[TKEY_ROT] )
		return FALSE;

	if( m_bMoveType != TMAINMOVE_NORMAL )
		return FALSE;

	if(m_vKEY.m_vSTATE[TKEY_ROT])
	{
		if(!m_bCamFIX)
			m_pCtrlHost->m_nDIR = INT(m_wCamDIR) - INT(m_pCtrlHost->m_wDIR);
		else if( m_vKEY.m_vTICK[TKEY_ROT] + CLICK_DELAY < m_vKEY.m_dwTick )
			m_wCamDIR = m_pCtrlHost->m_wDIR;

		return m_vKEY.m_vTICK[TKEY_ROT] + CLICK_DELAY > m_vKEY.m_dwTick ? FALSE : bResult;
	}

	int nMOVE = INT(m_pCtrlHost->m_wDIR) - INT(m_wCamDIR);
	while(nMOVE < 0)
		nMOVE += 1800;
	nMOVE %= 1800;

	if( nMOVE > 900 )
		nMOVE -= 1800;

	int nROT = abs(nMOVE);
	if( nROT > dwTick )
		nROT = dwTick;

	nMOVE = INT(m_wCamDIR) + (nMOVE < 0 ? -nROT : nROT);
	while(nMOVE < 0)
		nMOVE += 1800;
	m_wCamDIR = WORD(nMOVE % 1800);

	return bResult;
}

FLOAT CTClientGame::CheckCamPos(DWORD dwTick)
{
	D3DXVECTOR3 vTARGET = m_pCamera->m_vTarget;
	D3DXVECTOR3 vPOS = m_pCamera->m_vPosition;
	D3DXVECTOR3 vDIR = vPOS - vTARGET;

	FLOAT fDIST = D3DXVec3Length(&vDIR);
	if( fDIST > 0.0f )
		vDIR /= fDIST;

	if(m_vMAP.HitTest( &fDIST, &vTARGET, &vPOS, 2.0f * m_pMainChar->m_fRadius, 2.0f * m_pMainChar->m_fRadius, TRUE))
	{
		fDIST = min( m_pCamera->m_fPrevDIST, fDIST);
		fDIST = max( m_pMainChar->m_fRadius, fDIST);
	}
	else
		fDIST = m_pCamera->m_fPrevDIST;
	vPOS = vTARGET + fDIST * vDIR;

	FLOAT fWaterHeight = FLOAT(INT(100.0f * m_vMAP.GetWaterHeight(
		vPOS.x,
		vPOS.z))) / 100.0f;

	FLOAT fLandHeight = FLOAT(INT(100.0f * m_vMAP.GetLandHeight(
		vPOS.x,
		vPOS.y,
		vPOS.z))) / 100.0f;

	if( fWaterHeight > fLandHeight && fabs(vPOS.y - fWaterHeight) < TCAM_SIZE / 2.0f )
		if( vPOS.y > fWaterHeight || fWaterHeight < fLandHeight + TCAM_SIZE )
			vPOS.y = fWaterHeight + TCAM_SIZE / 2.0f;
		else
			vPOS.y = fWaterHeight - TCAM_SIZE / 2.0f;

	vDIR = vTARGET - vPOS;
	m_pCamera->m_fPrevDIST = D3DXVec3Length(&vDIR);

	if( m_pCamera->m_fPrevDIST > 0.0f )
	{
		D3DXVECTOR3 vUP;

		vDIR /= m_pCamera->m_fPrevDIST;
		D3DXVec3Cross(
			&vUP,
			&vDIR,
			&m_pCamera->m_vAxisX);

		fDIST = D3DXVec3Length(&vUP);
		if( fDIST > 0.0f )
			vUP /= fDIST;

		m_pCamera->SetPosition(
			vPOS,
			vTARGET,
			vUP);
	}
	m_pCamera->Activate(TRUE);

	if( m_pCamera->m_fPrevDIST < m_pCamera->m_fZoomMIN + THEIGHT_ZERO )
		m_pCtrlHost->m_bFPView = TRUE;
	else
		m_pCtrlHost->m_bFPView = FALSE;

	return fWaterHeight;
}

void CTClientGame::CalcPLAYER( DWORD dwTick)
{
	MAPPLAYER::iterator itPC;

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
	{
		CTClientChar *pPC = (*itPC).second;

		if(!pPC->GetRidingPet())
		{
			D3DXVECTOR3 vPOS = pPC->GetPosition();

			pPC->CalcDIR(
				dwTick,
				pPC->m_bMouseDIR,
				pPC->m_bKeyDIR);

			pPC->CalcMaintain(
				dwTick,
				FALSE,
				this);

			pPC->CalcTick(
				&vPOS,
				m_pDevice,
				m_pRES,
				dwTick);

			if( pPC->m_bBlock || (
				!pPC->m_dwHP && (
				pPC->m_bSlide ||
				pPC->IsPush())) )
			{
				FLOAT fNextH = vPOS.y;
				D3DXVECTOR3 vTNEXT;

				pPC->ForecastHeight(
					&m_vMAP,
					&fNextH,
					NULL,
					dwTick);

				if(m_vMAP.CheckCollision( pPC, &vTNEXT, &vPOS, fNextH))
				{
					pPC->m_vWorld._41 += vTNEXT.x - pPC->GetPositionX();
					pPC->m_vWorld._43 += vTNEXT.z - pPC->GetPositionZ();

					pPC->CalcTick(
						&vPOS,
						m_pDevice,
						m_pRES, 0);
				}
			}

			pPC->CalcHeight(
				&vPOS,
				&m_vMAP,
				dwTick);

			CalcTOBJDamage(
				pPC,
				dwTick);
		}

		ReportTRSCSMOVE(pPC);
	}
}

void CTClientGame::CalcTOBJDamage( CTClientObjBase *pTOBJ,
								   DWORD dwTick)
{
	BYTE bCritical = FALSE;

	if(pTOBJ->IsDead())
		pTOBJ->m_vTDAMAGE.clear();

	while(!pTOBJ->m_vTDAMAGE.empty())
	{
		switch(pTOBJ->m_vTDAMAGE[0])
		{
		case TBLOCK_NUMBER		:
			{
				CalcTOBJHit( pTOBJ, HT_BLOCK, 0);
				bCritical = FALSE;
			}

			break;

		case TMISS_NUMBER		:
			{
				CalcTOBJHit( pTOBJ, HT_MISS, 0);
				bCritical = FALSE;
			}

			break;

		case TCRITICAL_NUMBER	:
			{
				CalcTOBJHit( pTOBJ, HT_CRITICAL, 0);
				bCritical = TRUE;
			}

			break;

		default					:
			{
				DWORD dwCOLOR = 0;

				switch(GET_TDAMAGE_TYPE(pTOBJ->m_vTDAMAGE[0]))
				{
				case MTYPE_DAMAGE	:
				case MTYPE_HP		: dwCOLOR = m_pMainChar->IsAlliance(pTOBJ) ? TCOLOR_DEFEND : TCOLOR_ATTACK; break;
				case MTYPE_MDAMAGE	:
				case MTYPE_MP		: dwCOLOR = TCOLOR_MPATTACK; break;
				}

				if(dwCOLOR)
				{
					CTClientNumber *pTSFX = new CTClientNumber();

					pTSFX->InitNumber(
						m_pDevice,
						dwCOLOR,
						GET_TDAMAGE(pTOBJ->m_vTDAMAGE[0]));

					pTSFX->m_fSCALE = bCritical ? TNUMBER_CRITICAL_SCALE : 1.0f;
					pTSFX->m_vPOS = pTOBJ->GetPosition();
					pTSFX->m_vPOS.y += pTOBJ->m_fSizeY;

					pTSFX->m_vDIR.x = pTOBJ->LockDamagePOS();
					pTSFX->m_vDIR.y = 1.0f;

					m_vTNUMBER.push_back(pTSFX);
				}

				bCritical = FALSE;
			}

			break;
		}

		pTOBJ->m_vTDAMAGE.erase(pTOBJ->m_vTDAMAGE.begin());
	}
}

void CTClientGame::CalcTOBJHit( CTClientObjBase *pTOBJ,
							    BYTE bHit,
								DWORD dwTick)
{
	LPSFX pTSFX = NULL;

	switch(bHit)
	{
	case HT_CRITICAL	:
		{
			MAPRES::iterator itTSFX = m_pRES->m_mapSFX.find(ID_SFX_CRITICAL);

			if( itTSFX != m_pRES->m_mapSFX.end() )
				pTSFX = (LPSFX) (*itTSFX).second;
		}

		break;

	case HT_BLOCK	:
		{
			MAPRES::iterator itTSFX = m_pRES->m_mapSFX.find(ID_SFX_BLOCK);

			if( itTSFX != m_pRES->m_mapSFX.end() )
				pTSFX = (LPSFX) (*itTSFX).second;			
		}

		break;

	case HT_MISS		:
		{
			MAPRES::iterator itTSFX = m_pRES->m_mapSFX.find(ID_SFX_MISS);

			if( itTSFX != m_pRES->m_mapSFX.end() )
				pTSFX = (LPSFX) (*itTSFX).second;
		}

		break;
	}

	if(pTSFX)
	{
		CTachyonSFX *pTHIT = new CTachyonSFX();

		D3DXMatrixIdentity(&pTHIT->m_vGlobal);
		pTHIT->m_vGlobal._41 = pTOBJ->GetPositionX();
		pTHIT->m_vGlobal._42 = pTOBJ->GetPositionY() + pTOBJ->m_fSizeY;
		pTHIT->m_vGlobal._43 = pTOBJ->GetPositionZ();

		pTHIT->InitSFX(
			m_pDevice,
			pTSFX);
		pTHIT->m_bHasHost = FALSE;

		pTHIT->CalcTick( m_pDevice->m_pDevice, dwTick);
		CTachyonSFX::RegSFX( pTHIT, 0);
	}
}

void CTClientGame::CalcNUMBER( DWORD dwTick)
{
	if(m_vTNUMBER.empty())
		return;
	VTNUMBER::iterator itTNUMBER = m_vTNUMBER.begin();

	while(itTNUMBER != m_vTNUMBER.end())
	{
		CTClientNumber *pTNumber = (*itTNUMBER);

		if(pTNumber->IsDead())
		{
			m_vTNUMBER.erase(itTNUMBER);
			delete pTNumber;
		}
		else
		{
			pTNumber->CalcTick(dwTick);
			itTNUMBER++;
		}
	}
}

void CTClientGame::CalcTSQUADRSCS( LPTSQUAD pTSQUAD,
								   DWORD dwTick)
{
	WORD wSquadID = GetPartyID(m_pMainChar);
	BYTE bRESET = TRUE;

	if( !pTSQUAD || ((
		wSquadID != pTSQUAD->m_wPartyID || (
		!m_vTCMDDIR.m_bAlpha &&
		m_vTCMDDIR.m_bHide)) &&
		!pTSQUAD->m_vTARROW.m_vTSLASH.IsPlay() &&
		!pTSQUAD->m_vTMARK.m_bAlpha &&
		pTSQUAD->m_vTMARK.m_bHide) )
		return;

	if( wSquadID == pTSQUAD->m_wPartyID )
	{
		for( int i=0; i<INT(pTSQUAD->m_vTUNIT.size()); i++)
			if( pTSQUAD->m_dwChiefID != pTSQUAD->m_vTUNIT[i]->m_dwCharID &&
				pTSQUAD->m_wTargetMapID == pTSQUAD->m_vTUNIT[i]->m_wMapID &&
				pTSQUAD->m_bTCMD == pTSQUAD->m_vTUNIT[i]->m_bTCMD )
			{
				bRESET = FALSE;
				break;
			}
	}
	else
	{
		LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
			pTSQUAD,
			pTSQUAD->m_dwChiefID);

		if( pTUNIT && pTUNIT->m_bTCMD == pTSQUAD->m_bTCMD &&
			pTUNIT->m_wMapID == pTSQUAD->m_wTargetMapID )
			bRESET = FALSE;
	}

	if(bRESET)
	{
		pTSQUAD->m_bTargetType = OT_NONE;
		pTSQUAD->m_dwTargetID = 0;
		pTSQUAD->m_bTCMD = TRSCSCMD_NONE;

		pTSQUAD->m_wTargetMapID = 0xFFFF;
		pTSQUAD->m_wTargetX = 0;
		pTSQUAD->m_wTargetZ = 0;

		pTSQUAD->m_vTARROW.m_vTSLASH.ResetKEY();
		pTSQUAD->m_vTMARK.m_bHide = TRUE;

		if( wSquadID == pTSQUAD->m_wPartyID )
			CheckTSQUADDIR(pTSQUAD);

		return;
	}

	switch(pTSQUAD->m_bTCMD)
	{
	case TRSCSCMD_PROTECT	:
		{
			CTClientObjBase *pTOBJ = FindOBJ(
				pTSQUAD->m_dwTargetID,
				OT_PC);

			if(pTOBJ)
			{
				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.x = pTOBJ->GetPositionX();
				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.y = pTOBJ->GetPositionY() + pTOBJ->m_fSizeY;
				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.z = pTOBJ->GetPositionZ();

				if( pTOBJ->IsDead() || m_vMAP.m_wMapID != pTSQUAD->m_wTargetMapID )
				{
					pTSQUAD->m_bTargetType = OT_NONE;
					pTSQUAD->m_dwTargetID = 0;
					pTSQUAD->m_bTCMD = TRSCSCMD_NONE;

					pTSQUAD->m_wTargetMapID = 0xFFFF;
					pTSQUAD->m_wTargetX = 0;
					pTSQUAD->m_wTargetZ = 0;

					pTSQUAD->m_vTARROW.m_vTSLASH.ResetKEY();
					pTSQUAD->m_vTMARK.m_bHide = TRUE;

					if( wSquadID == pTSQUAD->m_wPartyID )
						CheckTSQUADDIR(pTSQUAD);

					return;
				}
			}
			else
			{
				LPTUNIT pTPROTECT = m_pTRSCS->FindTUNIT(
					m_pTRSCS->FindTSQUAD(pTSQUAD->m_wTargetX),
					pTSQUAD->m_dwTargetID);

				if( !pTPROTECT || !pTPROTECT->m_dwHP || pTPROTECT->m_wMapID != pTSQUAD->m_wTargetMapID )
				{
					pTSQUAD->m_bTargetType = OT_NONE;
					pTSQUAD->m_dwTargetID = 0;
					pTSQUAD->m_bTCMD = TRSCSCMD_NONE;

					pTSQUAD->m_wTargetMapID = 0xFFFF;
					pTSQUAD->m_wTargetX = 0;
					pTSQUAD->m_wTargetZ = 0;

					pTSQUAD->m_vTARROW.m_vTSLASH.ResetKEY();
					pTSQUAD->m_vTMARK.m_bHide = TRUE;

					if( wSquadID == pTSQUAD->m_wPartyID )
						CheckTSQUADDIR(pTSQUAD);

					return;
				}

				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.x = FLOAT(pTPROTECT->m_wPosX);
				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.z = FLOAT(pTPROTECT->m_wPosZ);

				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.y = m_vMAP.GetHeight(
					m_pMainChar,
					NULL,
					&D3DXVECTOR3(
					pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.x,
					m_pMainChar->GetPositionY(),
					pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.z),
					0.0f, FALSE);
			}
		}

		break;

	case TRSCSCMD_ATTACK	:
		{
			CTClientObjBase *pTOBJ = FindOBJ(
				pTSQUAD->m_dwTargetID,
				pTSQUAD->m_bTargetType);

			if(pTOBJ)
			{
				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.x = pTOBJ->GetPositionX();
				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.y = pTOBJ->GetPositionY() + pTOBJ->m_fSizeY;
				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.z = pTOBJ->GetPositionZ();

				pTSQUAD->m_vTMARK.m_bHide = pTOBJ->m_bHide;

				if( wSquadID == pTSQUAD->m_wPartyID )
					m_vTCMDDIR.m_bHide = pTOBJ->m_bHide;

				if( m_vMAP.m_wMapID != pTSQUAD->m_wTargetMapID )
				{
					pTSQUAD->m_bTargetType = OT_NONE;
					pTSQUAD->m_dwTargetID = 0;
					pTSQUAD->m_bTCMD = TRSCSCMD_NONE;

					pTSQUAD->m_wTargetMapID = 0xFFFF;
					pTSQUAD->m_wTargetX = 0;
					pTSQUAD->m_wTargetZ = 0;

					pTSQUAD->m_vTARROW.m_vTSLASH.ResetKEY();
					pTSQUAD->m_vTMARK.m_bHide = TRUE;

					if( wSquadID == pTSQUAD->m_wPartyID )
						CheckTSQUADDIR(pTSQUAD);

					return;
				}
			}
			else
			{
				LPTENEMY pTENEMY = m_pTRSCS->FindTENEMY(
					pTSQUAD->m_dwTargetID,
					pTSQUAD->m_bTargetType);

				if( !pTENEMY || pTENEMY->m_wMapID != pTSQUAD->m_wTargetMapID )
				{
					pTSQUAD->m_bTargetType = OT_NONE;
					pTSQUAD->m_dwTargetID = 0;
					pTSQUAD->m_bTCMD = TRSCSCMD_NONE;

					pTSQUAD->m_wTargetMapID = 0xFFFF;
					pTSQUAD->m_wTargetX = 0;
					pTSQUAD->m_wTargetZ = 0;

					pTSQUAD->m_vTARROW.m_vTSLASH.ResetKEY();
					pTSQUAD->m_vTMARK.m_bHide = TRUE;

					if( wSquadID == pTSQUAD->m_wPartyID )
						CheckTSQUADDIR(pTSQUAD);

					return;
				}

				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.x = FLOAT(pTENEMY->m_wPosX);
				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.z = FLOAT(pTENEMY->m_wPosZ);

				pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.y = m_vMAP.GetHeight(
					m_pMainChar,
					NULL,
					&D3DXVECTOR3(
					pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.x,
					m_pMainChar->GetPositionY(),
					pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.z),
					0.0f, FALSE);
			}
		}

		break;

	case TRSCSCMD_MOVE		: break;
	default					:
		{
			pTSQUAD->m_bTargetType = OT_NONE;
			pTSQUAD->m_dwTargetID = 0;
			pTSQUAD->m_bTCMD = TRSCSCMD_NONE;

			pTSQUAD->m_wTargetMapID = 0xFFFF;
			pTSQUAD->m_wTargetX = 0;
			pTSQUAD->m_wTargetZ = 0;

			pTSQUAD->m_vTARROW.m_vTSLASH.ResetKEY();
			pTSQUAD->m_vTMARK.m_bHide = TRUE;

			if( wSquadID == pTSQUAD->m_wPartyID )
				CheckTSQUADDIR(pTSQUAD);
		}

		return;
	}

	if(pTSQUAD->m_vTARROW.m_vTSLASH.IsPlay())
	{
		D3DXVECTOR2 vDIR(
			pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.x - pTSQUAD->m_vTARROW.m_vWorld._41,
			pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.z - pTSQUAD->m_vTARROW.m_vWorld._43);
		FLOAT fLength = D3DXVec2Length(&vDIR);

		if( fLength > 0.0f )
		{
			vDIR = pTSQUAD->m_vTARROW.m_fVelocityX * FLOAT(dwTick) * vDIR / fLength;
			FLOAT fLocal = D3DXVec2Length(&vDIR);

			if( fLocal > fLength )
				vDIR *= fLength / fLocal;
		}

		if( fLength > TCMDARROW_BOUND )
		{
			D3DXMATRIX vROT;

			pTSQUAD->m_vTARROW.m_dwTick += dwTick;
			FLOAT fTime = FLOAT(pTSQUAD->m_vTARROW.m_dwTick) / 1000.0f;

			FLOAT fTotal = fTime + fLength / (1000.0f * pTSQUAD->m_vTARROW.m_fVelocityX);
			FLOAT fVelocityY = pTSQUAD->m_vTARROW.m_fGravity * fTotal / 2.0f + (pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.y - pTSQUAD->m_vTARROW.m_fStartH) / fTotal;

			fVelocityY -= pTSQUAD->m_vTARROW.m_fGravity * fTime;
			D3DXMatrixRotationX(
				&pTSQUAD->m_vTARROW.m_vPosition,
				atan2f( fVelocityY, 1000.0f * pTSQUAD->m_vTARROW.m_fVelocityX));

			D3DXMatrixRotationY(
				&vROT,
				D3DX_PI + atan2f( vDIR.x, vDIR.y));
			pTSQUAD->m_vTARROW.m_vPosition *= vROT;

			pTSQUAD->m_vTARROW.m_vPosition._41 = pTSQUAD->m_vTARROW.m_vWorld._41 + vDIR.x;
			pTSQUAD->m_vTARROW.m_vPosition._42 = pTSQUAD->m_vTARROW.m_vWorld._42 + fVelocityY * FLOAT(dwTick) / 1000.0f;
			pTSQUAD->m_vTARROW.m_vPosition._43 = pTSQUAD->m_vTARROW.m_vWorld._43 + vDIR.y;
			pTSQUAD->m_vTARROW.m_vWorld = pTSQUAD->m_vTARROW.m_vPosition;

			pTSQUAD->m_vTARROW.CalcTick(
				m_pDevice->m_pDevice,
				dwTick);
		}
		else
		{
			if( wSquadID == pTSQUAD->m_wPartyID )
			{
				m_vTCMDDIR.m_bHide = FALSE;
				m_vTCMDDIR.m_bAlpha = 0;
			}

			pTSQUAD->m_vTARROW.m_vTSLASH.SFXStop();
			pTSQUAD->m_vTMARK.m_bHide = FALSE;
			pTSQUAD->m_vTMARK.m_bAlpha = 0;

			dwTick = 0;
		}
	}

	if( !pTSQUAD->m_vTMARK.m_bHide || pTSQUAD->m_vTMARK.m_bAlpha )
	{
		if(!pTSQUAD->m_vTMARK.m_bHide)
		{
			pTSQUAD->m_vTMARK.m_vWorld._41 = pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.x;
			pTSQUAD->m_vTMARK.m_vWorld._42 = pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.y;
			pTSQUAD->m_vTMARK.m_vWorld._43 = pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND.z;
		}

		pTSQUAD->m_vTMARK.CalcTick(
			m_pDevice->m_pDevice,
			dwTick);
	}

	if( wSquadID == pTSQUAD->m_wPartyID && (!m_vTCMDDIR.m_bHide || m_vTCMDDIR.m_bAlpha) )
		CalcTCMDDIR( &pTSQUAD->m_vTARROW.m_vTSKILLDATA.m_vTGROUND, dwTick);
}

void CTClientGame::CalcTRSCS( DWORD dwTick)
{
	m_pTMinimap->ShowComponent(IsShowMinimap());

	CTGaugePannel *pGauge = static_cast<CTGaugePannel *>(m_vTFRAME[TFRAME_GAUGE]);
	pGauge->m_pTournamentInfo->ShowComponent(!m_pTMinimap->IsVisible() && !m_pTRSCS->IsVisible());

	if( !m_vTCMDARROW.m_vTSLASH.IsPlay() &&
		!m_pTRSCS->m_dwTALARM &&
		!m_vTCMDTARGET.m_bAlpha &&
		!m_vTCMDDIR.m_bAlpha &&
		m_vTCMDTARGET.m_bHide &&
		m_vTCMDDIR.m_bHide )
		return;

	WORD wSquadID = GetPartyID(m_pMainChar);

	LPTSQUAD pTSQUAD = m_pTRSCS->FindTSQUAD(wSquadID);
	LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
		pTSQUAD,
		m_pMainChar->m_dwID);

	if(m_pTRSCS->m_dwTALARM)
		m_pTRSCS->m_dwTALARM -= min( m_pTRSCS->m_dwTALARM, dwTick);

	if(!pTUNIT)
	{
		m_vTCMDARROW.m_vTSLASH.ResetKEY();
		m_vTCMDTARGET.m_bHide = TRUE;

		if( !pTSQUAD || pTSQUAD->m_bTCMD == TRSCSCMD_NONE )
			m_vTCMDDIR.m_bHide = TRUE;

		if(m_vTCMDTARGET.m_bAlpha)
		{
			m_vTCMDTARGET.CalcTick(
				m_pDevice->m_pDevice,
				dwTick);
		}

		if(m_vTCMDDIR.m_bAlpha)
		{
			m_vTCMDDIR.m_vWorld._41 = TCMDDIR_POS_X;
			m_vTCMDDIR.m_vWorld._42 = TCMDDIR_POS_Y;
			m_vTCMDDIR.m_vWorld._43 = TCMDDIR_POS_Z;

			m_vTCMDDIR.CalcTick(
				m_pDevice->m_pDevice,
				dwTick);

			m_vTCMDDIR.ResetVisible(
				m_pCamera,
				0, FALSE,
				dwTick);
		}

		return;
	}

	switch(pTUNIT->m_bTCMD)
	{
	case TRSCSCMD_PROTECT	:
		{
			CTClientObjBase *pTOBJ = FindOBJ(
				pTUNIT->m_dwTargetID,
				OT_PC);

			if(pTOBJ)
			{
				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.x = pTOBJ->GetPositionX();
				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.y = pTOBJ->GetPositionY() + pTOBJ->m_fSizeY;
				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.z = pTOBJ->GetPositionZ();

				if(pTOBJ->IsDead())
				{
					pTUNIT->m_bTCMD = TRSCSCMD_NONE;
					pTUNIT->m_dwTargetID = 0;
					pTUNIT->m_wTargetX = 0;

					m_vTCMDARROW.m_vTSLASH.ResetKEY();
					m_vTCMDTARGET.m_bHide = TRUE;

					if( pTSQUAD->m_bTCMD == TRSCSCMD_NONE )
						m_vTCMDDIR.m_bHide = TRUE;

					RelaySession()->SendCS_CORPSCMD_REQ(
						wSquadID,
						m_pMainChar->m_dwID,
						0xFFFF,
						TRSCSCMD_NONE,
						0, 0, 0, 0);

					return;
				}
			}
			else
			{
				LPTUNIT pTPROTECT = m_pTRSCS->FindTUNIT(
					m_pTRSCS->FindTSQUAD(pTUNIT->m_wTargetX),
					pTUNIT->m_dwTargetID);

				if( !pTPROTECT || !pTPROTECT->m_dwHP || pTPROTECT->m_wMapID != pTUNIT->m_wMapID )
				{
					pTUNIT->m_bTCMD = TRSCSCMD_NONE;
					pTUNIT->m_dwTargetID = 0;
					pTUNIT->m_wTargetX = 0;

					m_vTCMDARROW.m_vTSLASH.ResetKEY();
					m_vTCMDTARGET.m_bHide = TRUE;

					if( pTSQUAD->m_bTCMD == TRSCSCMD_NONE )
						m_vTCMDDIR.m_bHide = TRUE;

					RelaySession()->SendCS_CORPSCMD_REQ(
						wSquadID,
						m_pMainChar->m_dwID,
						0xFFFF,
						TRSCSCMD_NONE,
						0, 0, 0, 0);

					return;
				}

				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.x = FLOAT(pTPROTECT->m_wPosX);
				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.z = FLOAT(pTPROTECT->m_wPosZ);

				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.y = m_vMAP.GetHeight(
					m_pMainChar,
					NULL,
					&D3DXVECTOR3(
					m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.x,
					m_pMainChar->GetPositionY(),
					m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.z),
					0.0f, FALSE);
			}
		}

		break;

	case TRSCSCMD_ATTACK	:
		{
			CTClientObjBase *pTOBJ = FindOBJ(
				pTUNIT->m_dwTargetID,
				pTUNIT->m_bTargetType);

			if(pTOBJ)
			{
				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.x = pTOBJ->GetPositionX();
				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.y = pTOBJ->GetPositionY() + pTOBJ->m_fSizeY;
				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.z = pTOBJ->GetPositionZ();

				m_vTCMDTARGET.m_bHide = pTOBJ->m_bHide;

				if( !pTSQUAD || pTSQUAD->m_bTCMD == TRSCSCMD_NONE )
					m_vTCMDDIR.m_bHide = pTOBJ->m_bHide;
			}
			else
			{
				LPTENEMY pTENEMY = m_pTRSCS->FindTENEMY(
					pTUNIT->m_dwTargetID,
					pTUNIT->m_bTargetType);

				if( !pTENEMY || pTENEMY->m_wMapID != pTUNIT->m_wMapID )
				{
					pTUNIT->m_bTCMD = TRSCSCMD_NONE;
					pTUNIT->m_dwTargetID = 0;
					pTUNIT->m_bTargetType = OT_NONE;

					m_vTCMDARROW.m_vTSLASH.ResetKEY();
					m_vTCMDTARGET.m_bHide = TRUE;

					if( pTSQUAD->m_bTCMD == TRSCSCMD_NONE )
						m_vTCMDDIR.m_bHide = TRUE;

					RelaySession()->SendCS_CORPSCMD_REQ(
						wSquadID,
						m_pMainChar->m_dwID,
						0xFFFF,
						TRSCSCMD_NONE,
						0, 0, 0, 0);

					return;
				}

				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.x = FLOAT(pTENEMY->m_wPosX);
				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.z = FLOAT(pTENEMY->m_wPosZ);

				m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.y = m_vMAP.GetHeight(
					m_pMainChar,
					NULL,
					&D3DXVECTOR3(
					m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.x,
					m_pMainChar->GetPositionY(),
					m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.z),
					0.0f, FALSE);
			}
		}

		break;

	case TRSCSCMD_MOVE		:
		if( D3DXVec2Length(&D3DXVECTOR2(
			FLOAT(pTUNIT->m_wPosX) - m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.x,
			FLOAT(pTUNIT->m_wPosZ) - m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.z)) < TRSCSMOVE_BOUND )
		{
			pTUNIT->m_bTCMD = TRSCSCMD_NONE;
			pTUNIT->m_wTargetX = 0;
			pTUNIT->m_wTargetZ = 0;

			m_vTCMDARROW.m_vTSLASH.ResetKEY();
			m_vTCMDTARGET.m_bHide = TRUE;

			if( pTSQUAD->m_bTCMD == TRSCSCMD_NONE )
				m_vTCMDDIR.m_bHide = TRUE;

			RelaySession()->SendCS_CORPSCMD_REQ(
				wSquadID,
				m_pMainChar->m_dwID,
				0xFFFF,
				TRSCSCMD_NONE,
				0, 0, 0, 0);

			return;
		}

		break;

	default					:
		{
			m_vTCMDARROW.m_vTSLASH.ResetKEY();
			m_vTCMDTARGET.m_bHide = TRUE;

			if( pTSQUAD->m_bTCMD == TRSCSCMD_NONE )
				m_vTCMDDIR.m_bHide = TRUE;
		}

		break;
	}

	if(m_vTCMDARROW.m_vTSLASH.IsPlay())
	{
		D3DXVECTOR2 vDIR(
			m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.x - m_vTCMDARROW.m_vWorld._41,
			m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.z - m_vTCMDARROW.m_vWorld._43);
		FLOAT fLength = D3DXVec2Length(&vDIR);

		if( fLength > 0.0f )
		{
			vDIR = m_vTCMDARROW.m_fVelocityX * FLOAT(dwTick) * vDIR / fLength;
			FLOAT fLocal = D3DXVec2Length(&vDIR);

			if( fLocal > fLength )
				vDIR *= fLength / fLocal;
		}

		if( fLength > TCMDARROW_BOUND )
		{
			D3DXMATRIX vROT;

			m_vTCMDARROW.m_dwTick += dwTick;
			FLOAT fTime = FLOAT(m_vTCMDARROW.m_dwTick) / 1000.0f;

			FLOAT fTotal = fTime + fLength / (1000.0f * m_vTCMDARROW.m_fVelocityX);
			FLOAT fVelocityY = m_vTCMDARROW.m_fGravity * fTotal / 2.0f + (m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.y - m_vTCMDARROW.m_fStartH) / fTotal;

			fVelocityY -= m_vTCMDARROW.m_fGravity * fTime;
			D3DXMatrixRotationX(
				&m_vTCMDARROW.m_vPosition,
				atan2f( fVelocityY, 1000.0f * m_vTCMDARROW.m_fVelocityX));

			D3DXMatrixRotationY(
				&vROT,
				D3DX_PI + atan2f( vDIR.x, vDIR.y));
			m_vTCMDARROW.m_vPosition *= vROT;

			m_vTCMDARROW.m_vPosition._41 = m_vTCMDARROW.m_vWorld._41 + vDIR.x;
			m_vTCMDARROW.m_vPosition._42 = m_vTCMDARROW.m_vWorld._42 + fVelocityY * FLOAT(dwTick) / 1000.0f;
			m_vTCMDARROW.m_vPosition._43 = m_vTCMDARROW.m_vWorld._43 + vDIR.y;
			m_vTCMDARROW.m_vWorld = m_vTCMDARROW.m_vPosition;

			m_vTCMDARROW.CalcTick(
				m_pDevice->m_pDevice,
				dwTick);
		}
		else
		{
			m_vTCMDARROW.m_vTSLASH.SFXStop();
			m_vTCMDTARGET.m_bHide = FALSE;
			m_vTCMDTARGET.m_bAlpha = 0;
			m_vTCMDDIR.m_bHide = FALSE;
			m_vTCMDDIR.m_bAlpha = 0;

			dwTick = 0;
		}
	}

	if( !m_vTCMDTARGET.m_bHide || m_vTCMDTARGET.m_bAlpha )
	{
		if(!m_vTCMDTARGET.m_bHide)
		{
			m_vTCMDTARGET.m_vWorld._41 = m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.x;
			m_vTCMDTARGET.m_vWorld._42 = m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.y;
			m_vTCMDTARGET.m_vWorld._43 = m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND.z;
		}

		m_vTCMDTARGET.CalcTick(
			m_pDevice->m_pDevice,
			dwTick);
	}

	if((!m_vTCMDDIR.m_bHide || m_vTCMDDIR.m_bAlpha) && (!pTSQUAD || pTSQUAD->m_bTCMD == TRSCSCMD_NONE))
		CalcTCMDDIR( &m_vTCMDARROW.m_vTSKILLDATA.m_vTGROUND, dwTick);
}

void CTClientGame::CalcTCMDDIR( LPD3DXVECTOR3 pTARGET,
							    DWORD dwTick)
{
	static D3DXVECTOR3 vDIR( 0.0f, 0.0f, 1.0f);
	static DWORD dwDIRTick = 0;

	D3DXVECTOR3 vTCMDDIR( 0.0f, 0.0f, -1.0f);
	D3DXMATRIX vRotX;
	FLOAT fTSCALE;

	D3DXMatrixRotationY(
		&m_vTCMDDIR.m_vWorld,
		D3DX_PI + atan2f(
		pTARGET->x - m_pMainChar->GetPositionX(),
		pTARGET->z - m_pMainChar->GetPositionZ()) - atan2f(
		m_pCamera->m_vTarget.x - m_pCamera->m_vPosition.x,
		m_pCamera->m_vTarget.z - m_pCamera->m_vPosition.z));

	CTMath::Transform(
		&m_vTCMDDIR.m_vWorld,
		&vTCMDDIR);

	D3DXMatrixRotationX(
		&vRotX,
		D3DX_PI / 9.0f);
	m_vTCMDDIR.m_vWorld *= vRotX;

	m_vTCMDDIR.m_vWorld._41 = TCMDDIR_POS_X;
	m_vTCMDDIR.m_vWorld._42 = TCMDDIR_POS_Y;
	m_vTCMDDIR.m_vWorld._43 = TCMDDIR_POS_Z;

	fTSCALE = D3DXVec3Dot( &vDIR, &vTCMDDIR);
	if( fTSCALE < 0.0f || dwDIRTick )
	{
		dwDIRTick += dwTick;

		if( dwDIRTick < 2 * TCMDDIR_SCALE_TICK || fTSCALE < 0.0f )
		{
			dwDIRTick %= 2 * TCMDDIR_SCALE_TICK;
			fTSCALE = 1.0f + TCMDDIR_SCALE * FLOAT(dwDIRTick < TCMDDIR_SCALE_TICK ? dwDIRTick : 2 * TCMDDIR_SCALE_TICK - dwDIRTick) / FLOAT(TCMDDIR_SCALE_TICK);
		}
		else
		{
			dwDIRTick = 0;
			fTSCALE = 1.0f;
		}
	}
	else
		fTSCALE = 1.0f;

	D3DXMatrixScaling(
		&m_vTCMDDIR.m_vScale,
		fTSCALE,
		fTSCALE,
		fTSCALE);

	m_vTCMDDIR.CalcTick(
		m_pDevice->m_pDevice,
		dwTick);

	m_vTCMDDIR.ResetVisible(
		m_pCamera,
		0, FALSE,
		dwTick);
}

void CTClientGame::RefreshTRSCSCMD()
{
	WORD wSquadID = GetPartyID(m_pMainChar);

	for( int i=0; i<INT(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
		if( m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_wTargetMapID == m_vMAP.m_wMapID &&
			m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_bTCMD != TRSCSCMD_NONE )
		{
			if( wSquadID == m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_wPartyID )
			{
				m_vTCMDDIR.m_bHide = FALSE;
				m_vTCMDDIR.m_bAlpha = 0;
			}

			m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTMARK.m_bHide = FALSE;
			m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTMARK.m_bAlpha = 0;
		}
}

void CTClientGame::CheckTSQUADDIR( LPTSQUAD pTSQUAD)
{
	LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
		pTSQUAD,
		m_pMainChar->m_dwID);

	if( pTUNIT && (pTUNIT->m_bTCMD == TRSCSCMD_NONE) )
		m_vTCMDDIR.m_bHide = TRUE;
}

void CTClientGame::CalcARROW( DWORD dwTick)
{
	VTARROW::iterator itTARROW = m_vTARROW.begin();

	while(itTARROW != m_vTARROW.end())
	{
		CTClientArrow *pTARROW = (*itTARROW);
		CTClientObjBase *pTARGET = FindOBJ(
			pTARROW->m_pTARGET->m_dwID,
			pTARROW->m_pTARGET->m_bType);

		if(pTARGET)
		{
			D3DXVECTOR2 vDIR(
				pTARGET->GetPositionX() - pTARROW->m_vWorld._41,
				pTARGET->GetPositionZ() - pTARROW->m_vWorld._43);
			FLOAT fLength = D3DXVec2Length(&vDIR);

			if( fLength > 0.0f )
			{
				if( pTARROW->m_bAccel && pTARROW->m_dwTick < pTARROW->m_dwTotalTick )
				{
					FLOAT fTime = pTARROW->m_dwTotalTick ?
						FLOAT(pTARROW->m_dwTick) / FLOAT(pTARROW->m_dwTotalTick) : 0;

					D3DXVECTOR2 vTACCEL = (1.0f - fTime) * pTARROW->m_vTACCEL + fTime * vDIR / fLength;
					FLOAT fLocal = D3DXVec2Length(&vTACCEL);

					if( fLocal > 0.0f )
						vDIR = fLength * vTACCEL / fLocal;
				}

				vDIR = pTARROW->m_fVelocityX * FLOAT(dwTick) * vDIR / fLength;
				FLOAT fLocal = D3DXVec2Length(&vDIR);

				if( fLocal > fLength )
					vDIR *= fLength / fLocal;
			}

			fLength = D3DXVec2Length(&D3DXVECTOR2(
				pTARGET->GetPositionX() - pTARROW->m_vWorld._41 - vDIR.x,
				pTARGET->GetPositionZ() - pTARROW->m_vWorld._43 - vDIR.y));

			if( fLength > pTARGET->m_fRadius )
			{
				D3DXMATRIX vROT;

				pTARROW->m_dwTick += dwTick;
				FLOAT fTime = FLOAT(pTARROW->m_dwTick) / 1000.0f;

				FLOAT fTotal = fTime + fLength / (1000.0f * pTARROW->m_fVelocityX);
				FLOAT fVelocityY = pTARROW->m_fGravity * fTotal / 2.0f + (pTARGET->GetPositionY() + pTARGET->m_fSizeY / 2.0f - pTARROW->m_fStartH) / fTotal;

				fVelocityY -= pTARROW->m_fGravity * fTime;
				D3DXMatrixRotationX(
					&pTARROW->m_vPosition,
					atan2f( fVelocityY, 1000.0f * pTARROW->m_fVelocityX));

				D3DXMatrixRotationY(
					&vROT,
					D3DX_PI + atan2f( vDIR.x, vDIR.y));
				pTARROW->m_vPosition *= vROT;

				pTARROW->m_vPosition._41 = pTARROW->m_vWorld._41 + vDIR.x;
				pTARROW->m_vPosition._42 = pTARROW->m_vWorld._42 + fVelocityY * FLOAT(dwTick) / 1000.0f;
				pTARROW->m_vPosition._43 = pTARROW->m_vWorld._43 + vDIR.y;
				pTARROW->m_vWorld = pTARROW->m_vPosition;

				pTARROW->CalcTick(
					m_pDevice->m_pDevice,
					dwTick);

				itTARROW++;
			}
			else
			{
				CTClientObjBase *pTHOST = FindOBJ(
					pTARROW->m_dwHostID,
					pTARROW->m_bHostType);
				LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(pTARROW->m_vTSKILLDATA.m_wID);

				if(CanDefend( pTHOST, pTARGET, pTSKILL))
					Defend( pTHOST, pTARGET, &pTARROW->m_vTSKILLDATA, &pTARROW->m_vTSKILLDATA.m_vTGROUND, pTSKILL, 0, TRUE );
				m_vTARROW.erase(itTARROW);

				delete pTARROW;
			}
		}
		else
		{
			m_vTARROW.erase(itTARROW);
			delete pTARROW;
		}
	}
}

CTClientObjBase *CTClientGame::SeekRecallTarget( CTClientRecall *pRECALL)
{
	CTClientObjBase *pTARGET = NULL;
	FLOAT fDIST = 0.0f;

	MAPMONSTER::iterator itMON;
	MAPPLAYER::iterator itPC;
	MAPRECALL::iterator itRECALL;

	if( !pRECALL->m_pTCURSKILL || pRECALL->m_bFlash )
		return pTARGET;

	for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
		if( !(*itRECALL).second->IsDead() && CanDefend( pRECALL, (*itRECALL).second, pRECALL->m_pTCURSKILL->m_pTSKILL) )
		{
			FLOAT fLOCAL = D3DXVec3Length(&D3DXVECTOR3(
				pRECALL->GetPositionX() - (*itRECALL).second->GetPositionX(),
				pRECALL->GetPositionY() - (*itRECALL).second->GetPositionY(),
				pRECALL->GetPositionZ() - (*itRECALL).second->GetPositionZ()));

			if( fLOCAL < pRECALL->GetLB((*itRECALL).second) && (!pTARGET || fLOCAL < fDIST) && CanDetect( pRECALL, (*itRECALL).second) )
			{
				pTARGET = (*itRECALL).second;
				fDIST = fLOCAL;
			}
		}

	for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
		if( !(*itRECALL).second->IsDead() && CanDefend( pRECALL, (*itRECALL).second, pRECALL->m_pTCURSKILL->m_pTSKILL) )
		{
			FLOAT fLOCAL = D3DXVec3Length(&D3DXVECTOR3(
				pRECALL->GetPositionX() - (*itRECALL).second->GetPositionX(),
				pRECALL->GetPositionY() - (*itRECALL).second->GetPositionY(),
				pRECALL->GetPositionZ() - (*itRECALL).second->GetPositionZ()));

			if( fLOCAL < pRECALL->GetLB((*itRECALL).second) && (!pTARGET || fLOCAL < fDIST) && CanDetect( pRECALL, (*itRECALL).second) )
			{
				pTARGET = (*itRECALL).second;
				fDIST = fLOCAL;
			}
		}

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
		if( !(*itPC).second->IsDead() && CanDefend( pRECALL, (*itPC).second, pRECALL->m_pTCURSKILL->m_pTSKILL) )
		{
			FLOAT fLOCAL = D3DXVec3Length(&D3DXVECTOR3(
				pRECALL->GetPositionX() - (*itPC).second->GetPositionX(),
				pRECALL->GetPositionY() - (*itPC).second->GetPositionY(),
				pRECALL->GetPositionZ() - (*itPC).second->GetPositionZ()));

			if( fLOCAL < pRECALL->GetLB((*itPC).second) && (!pTARGET || fLOCAL < fDIST) && CanDetect( pRECALL, (*itPC).second) )
			{
				pTARGET = (*itPC).second;
				fDIST = fLOCAL;
			}
		}

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
		if( !(*itMON).second->IsDead() && CanDefend( pRECALL, (*itMON).second, pRECALL->m_pTCURSKILL->m_pTSKILL) )
		{
			FLOAT fLOCAL = D3DXVec3Length(&D3DXVECTOR3(
				pRECALL->GetPositionX() - (*itMON).second->GetPositionX(),
				pRECALL->GetPositionY() - (*itMON).second->GetPositionY(),
				pRECALL->GetPositionZ() - (*itMON).second->GetPositionZ()));

			if( fLOCAL < pRECALL->GetLB((*itMON).second) && (!pTARGET || fLOCAL < fDIST) && CanDetect( pRECALL, (*itMON).second) )
			{
				pTARGET = (*itMON).second;
				fDIST = fLOCAL;
			}
		}

	return pTARGET;
}

CTClientObjBase *CTClientGame::SeekMonTarget( CTClientMonster *pMON)
{
	if(pMON->m_bGoHome)
		return NULL;

	FLOAT fDIST = D3DXVec3Length(&D3DXVECTOR3(
		pMON->GetPositionX() - m_pMainChar->GetPositionX(),
		pMON->GetPositionY() - m_pMainChar->GetPositionY(),
		pMON->GetPositionZ() - m_pMainChar->GetPositionZ()));

	CTClientObjBase *pTARGET = m_pMainChar->IsDead() ||
		fDIST > pMON->GetLB(m_pMainChar) ||
		!CanDefend( pMON, m_pMainChar, pMON->m_pTDEFSKILL) ||
		!CanDetect( pMON, m_pMainChar) ? NULL : m_pMainChar;

	MAPRECALL::iterator itRECALL;
	MAPPLAYER::iterator itPC;

	for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
		if( !(*itRECALL).second->IsDead() && CanDefend( pMON, (*itRECALL).second, pMON->m_pTDEFSKILL) )
		{
			FLOAT fLOCAL = D3DXVec3Length(&D3DXVECTOR3(
				pMON->GetPositionX() - (*itRECALL).second->GetPositionX(),
				pMON->GetPositionY() - (*itRECALL).second->GetPositionY(),
				pMON->GetPositionZ() - (*itRECALL).second->GetPositionZ()));

			if( fLOCAL < pMON->GetLB((*itRECALL).second) && (!pTARGET || fLOCAL < fDIST) && CanDetect( pMON, (*itRECALL).second) )
			{
				pTARGET = (*itRECALL).second;
				fDIST = fLOCAL;
			}
		}

	for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
		if( !(*itRECALL).second->IsDead() && CanDefend( pMON, (*itRECALL).second, pMON->m_pTDEFSKILL) )
		{
			FLOAT fLOCAL = D3DXVec3Length(&D3DXVECTOR3(
				pMON->GetPositionX() - (*itRECALL).second->GetPositionX(),
				pMON->GetPositionY() - (*itRECALL).second->GetPositionY(),
				pMON->GetPositionZ() - (*itRECALL).second->GetPositionZ()));

			if( fLOCAL < pMON->GetLB((*itRECALL).second) && (!pTARGET || fLOCAL < fDIST) && CanDetect( pMON, (*itRECALL).second) )
			{
				pTARGET = (*itRECALL).second;
				fDIST = fLOCAL;
			}
		}

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
		if( !(*itPC).second->IsDead() && CanDefend( pMON, (*itPC).second, pMON->m_pTDEFSKILL) )
		{
			FLOAT fLOCAL = D3DXVec3Length(&D3DXVECTOR3(
				pMON->GetPositionX() - (*itPC).second->GetPositionX(),
				pMON->GetPositionY() - (*itPC).second->GetPositionY(),
				pMON->GetPositionZ() - (*itPC).second->GetPositionZ()));

			if( fLOCAL < pMON->GetLB((*itPC).second) && (!pTARGET || fLOCAL < fDIST) && CanDetect( pMON, (*itPC).second) )
			{
				pTARGET = (*itPC).second;
				fDIST = fLOCAL;
			}
		}

	return pTARGET;
}

void CTClientGame::CalcSLAVEMON( DWORD dwTick)
{
	static DWORD dwPATH = 0;
	static std::vector<CTClientMonster*> vPATHMON;

	MAPMONSTER::iterator itMON;
	VTMONMOVE vTMONMOVE;
	BYTE bPATH = FALSE;

	if( dwPATH > PATH_TICK )
	{
		bPATH = TRUE;
		dwPATH = 0;
	}
	else
		dwPATH += dwTick;

	for( itMON = m_mapSLAVEMON.begin(); itMON != m_mapSLAVEMON.end(); itMON++)
		if( (*itMON).second && (*itMON).second->m_pMON &&
			(*itMON).second->m_pMON->m_bApplyAI )
		{
			CTClientMonster *pMONSTER = (*itMON).second;
			D3DXVECTOR3 vTPOS = pMONSTER->GetPosition();
			BYTE bROAM = TRUE;

			if( pMONSTER->m_bFollowType == OT_NONE )
			{
#if defined(TEST_MODE) || defined(DEBUG)
				CTClientObjBase *pFollow = NULL;

				if(m_bMonRoaming)
					pFollow = SeekMonTarget(pMONSTER);
#else
				CTClientObjBase *pFollow = SeekMonTarget(pMONSTER);
#endif

				if( pMONSTER->m_nHostAdjacency != - 1)
					pMONSTER->m_nHostAdjacency = -1;

				if(pFollow)
				{
					D3DXVECTOR3 vFOLLOW = pFollow->GetPosition();
					FLOAT fLB = pMONSTER->GetLB(pFollow);
					FLOAT fAB = pMONSTER->GetAB(pFollow);

					if( !pFollow->CanFLY() || fAB >= fabs(vTPOS.y - vFOLLOW.y) )
					{
						FLOAT fDist = D3DXVec3LengthSq(&(vTPOS - vFOLLOW));

						if( fDist < fAB * fAB )
						{
							pMONSTER->m_bFollowType = pFollow->m_bType;
							pMONSTER->m_dwFollowID = pFollow->m_dwID;
							pMONSTER->m_bTD = TD_AB;

							SendCS_ENTERAB_REQ(
								pFollow->GetHostID(),
								pFollow->m_dwID,
								pFollow->m_bType,
								pMONSTER->m_dwID,
								m_pMainWnd->m_bChannel,
								m_vMAP.m_wMapID);

							if( fAB < TPATH_LONG )
								pFollow->AddAdjacency( pMONSTER );
						}
						else if( fDist < fLB * fLB )
						{
 							pMONSTER->m_bFollowType = pFollow->m_bType;
							pMONSTER->m_dwFollowID = pFollow->m_dwID;
							pMONSTER->m_bTD = TD_LB;

							SendCS_ENTERLB_REQ(
								pFollow->GetHostID(),
								pFollow->m_dwID,
								pFollow->m_bType,
								pMONSTER->m_dwID,
								m_pMainWnd->m_bChannel,
								m_vMAP.m_wMapID);
						}
					}
				}
			}
			else
			{
				CTClientObjBase *pFollow = FindOBJ(
					pMONSTER->m_dwFollowID,
					pMONSTER->m_bFollowType);

				if( pFollow && pMONSTER->m_bTD != TD_AB && pMONSTER->m_nHostAdjacency != -1 )
					pFollow->LeaveAdjacency( pMONSTER );

				if(!CanFollow( pMONSTER, pFollow))
				{
					pMONSTER->m_bFollowType = OT_NONE;
					pMONSTER->m_dwFollowID = 0;
					pMONSTER->m_bTD = TD_NONE;

					if(pFollow)
					{
						SendCS_LEAVELB_REQ(
							pFollow->GetHostID(),
							pFollow->m_dwID,
							pFollow->m_bType,
							pMONSTER->m_dwID,
							m_pMainWnd->m_bChannel,
							m_vMAP.m_wMapID);
					}

					pMONSTER->m_bMouseDIR = TKDIR_N;
					pMONSTER->m_bKeyDIR = TKDIR_N;
					pMONSTER->SetTAction(TA_STAND);

					LPTMONMOVE pMOVE = new TMONMOVE(
						pMONSTER,
						m_pMainWnd->m_bChannel,
						m_vMAP.m_wMapID,
						vTPOS.x,
						vTPOS.y,
						vTPOS.z,
						pMONSTER->m_wPITCH,
						pMONSTER->m_wDIR,
						pMONSTER->m_bMouseDIR,
						pMONSTER->m_bKeyDIR,
						pMONSTER->m_bAction);

					vTMONMOVE.push_back(pMOVE);
					bROAM = FALSE;
				}
				else
				{
					D3DXVECTOR3 vFOLLOW = pFollow->GetPosition();

					FLOAT fDist = fDist = D3DXVec3LengthSq(&(vTPOS - vFOLLOW));

					FLOAT fLOST = pMONSTER->GetLOST(pFollow);
					FLOAT fAB = pMONSTER->GetAB(pFollow);

					if( fDist >= fLOST * fLOST )
					{
						pFollow->LeaveAdjacency( pMONSTER );
						
						pMONSTER->m_bFollowType = OT_NONE;
						pMONSTER->m_dwFollowID = 0;
						pMONSTER->m_bTD = TD_NONE;
						pMONSTER->m_bBACK = FALSE;

						SendCS_LEAVELB_REQ(
							pFollow->GetHostID(),
							pFollow->m_dwID,
							pFollow->m_bType,
							pMONSTER->m_dwID,
							m_pMainWnd->m_bChannel,
							m_vMAP.m_wMapID);

						pMONSTER->m_bMouseDIR = TKDIR_N;
						pMONSTER->m_bKeyDIR = TKDIR_N;
						pMONSTER->SetTAction(TA_STAND);

						LPTMONMOVE pMOVE = new TMONMOVE(
							pMONSTER,
							m_pMainWnd->m_bChannel,
							m_vMAP.m_wMapID,
							vTPOS.x,
							vTPOS.y,
							vTPOS.z,
							pMONSTER->m_wPITCH,
							pMONSTER->m_wDIR,
							pMONSTER->m_bMouseDIR,
							pMONSTER->m_bKeyDIR,
							pMONSTER->m_bAction);

						vTMONMOVE.push_back(pMOVE);
						bROAM = FALSE;
					}
					else if( !pMONSTER->m_bBACK &&
						(fDist >= fAB * fAB || !CanDetect( pMONSTER, pFollow)) )
					{
						if( bPATH && pMONSTER->m_bAction == TA_FOLLOW )
							if( !pMONSTER->IsDead() && !pMONSTER->IsBlock() && !pMONSTER->IsDown() )
							{
								vPATHMON.push_back( pMONSTER );
					
								FLOAT fThata = FLOAT(rand() % 8) * 45.0f * D3DX_PI / 180.0f;

								D3DXVECTOR3 vTARGET = vFOLLOW;
								D3DXVECTOR3 vTSTART = vTPOS;

								vTARGET.x += TPATH_RADIUS * cosf(fThata);
								vTARGET.y += TPATH_HEIGHT;
								vTARGET.z += TPATH_RADIUS * sinf(fThata);
								vTSTART.y += TPATH_HEIGHT;
							
								D3DXVECTOR3 vTGOAL = m_vMAP.GetTargetPoint(
									pMONSTER,
									&vTARGET);

								vTGOAL -= vTSTART;

								WORD wPITCH = pMONSTER->GetTargetPITCH(
									vTGOAL.x,
									vTGOAL.y,
									vTGOAL.z);

								WORD wDIR = pMONSTER->GetTargetDIR(
									vTGOAL.x,
									vTGOAL.z);

								if( pMONSTER->CheckDIR( wDIR, wPITCH) || pMONSTER->m_bKeyDIR == TKDIR_N )
								{
									pMONSTER->m_nPITCH = INT(wPITCH) - INT(pMONSTER->m_wPITCH);
									pMONSTER->m_nDIR = INT(wDIR) - INT(pMONSTER->m_wDIR);

									pMONSTER->m_bMouseDIR = TKDIR_F;
									pMONSTER->m_bKeyDIR = TKDIR_F;

									LPTMONMOVE pMOVE = new TMONMOVE(
										pMONSTER,
										m_pMainWnd->m_bChannel,
										m_vMAP.m_wMapID,
										vTPOS.x,
										vTPOS.y,
										vTPOS.z,
										wPITCH,
										wDIR,
										pMONSTER->m_bMouseDIR,
										pMONSTER->m_bKeyDIR,
										pMONSTER->m_bAction);

									vTMONMOVE.push_back(pMOVE);
									bROAM = FALSE;
								}
							}
							else if( pMONSTER->m_bKeyDIR != TKDIR_N )
							{
								pMONSTER->m_bMouseDIR = TKDIR_N;
								pMONSTER->m_bKeyDIR = TKDIR_N;

								LPTMONMOVE pMOVE = new TMONMOVE(
									pMONSTER,
									m_pMainWnd->m_bChannel,
									m_vMAP.m_wMapID,
									vTPOS.x,
									vTPOS.y,
									vTPOS.z,
									pMONSTER->m_wPITCH,
									pMONSTER->m_wDIR,
									pMONSTER->m_bMouseDIR,
									pMONSTER->m_bKeyDIR,
									pMONSTER->m_bAction);

								vTMONMOVE.push_back(pMOVE);
								bROAM = FALSE;
							}

						if( pMONSTER->m_bTD == TD_AB )
						{
							pMONSTER->m_bFollowType = pFollow->m_bType;
							pMONSTER->m_dwFollowID = pFollow->m_dwID;
							pMONSTER->m_bTD = TD_LB;

							SendCS_LEAVEAB_REQ(
								pFollow->GetHostID(),
								pFollow->m_dwID,
								pFollow->m_bType,
								pMONSTER->m_dwID,
								m_pMainWnd->m_bChannel,
								m_vMAP.m_wMapID);
							
							pFollow->LeaveAdjacency( pMONSTER );
						}
					}
					else
					{
						// 여기는 AB안에 들어온 상태.
						if( pMONSTER->m_bAction == TA_FOLLOW )
						{
							if( !pMONSTER->IsDead() && !pMONSTER->IsBlock() && !pMONSTER->IsDown() )
							{
								FLOAT fMinDist = pFollow->m_fRadius + pMONSTER->m_fRadius;

								if( pMONSTER->m_bActionLock )
								{
									D3DXVECTOR3 vTDIR = vFOLLOW - vTPOS;

									WORD wPITCH = pMONSTER->GetTargetPITCH(
										vTDIR.x,
										vTDIR.y,
										vTDIR.z);

									WORD wDIR = pMONSTER->GetTargetDIR(
											vTDIR.x,
											vTDIR.z);

									if( pMONSTER->CheckDIR( wDIR, wPITCH) || pMONSTER->m_bKeyDIR == TKDIR_F )
									{
										pMONSTER->m_nPITCH = INT(wPITCH) - INT(pMONSTER->m_wPITCH);
										pMONSTER->m_nDIR = INT(wDIR) - INT(pMONSTER->m_wDIR);

										pMONSTER->m_bMouseDIR = TKDIR_N;
										pMONSTER->m_bKeyDIR = TKDIR_N;

										LPTMONMOVE pMOVE = new TMONMOVE(
											pMONSTER,
											m_pMainWnd->m_bChannel,
											m_vMAP.m_wMapID,
											vTPOS.x,
											vTPOS.y,
											vTPOS.z,
											wPITCH,
											wDIR,
											pMONSTER->m_bMouseDIR,
											pMONSTER->m_bKeyDIR,
											pMONSTER->m_bAction);

										vTMONMOVE.push_back(pMOVE);
										bROAM = FALSE;
									}
								}
								else if( !pMONSTER->m_bBACK &&
									fDist < fMinDist*fMinDist &&
									fAB < TPATH_LONG )
								{
									D3DXVECTOR3 vTDIR = vTPOS - vFOLLOW;

									WORD wPITCH = pMONSTER->GetTargetPITCH(
										vTDIR.x,
										vTDIR.y,
										vTDIR.z);

									WORD wDIR = pMONSTER->GetTargetDIR(
											vTDIR.x,
											vTDIR.z);

									pMONSTER->m_nPITCH = INT(wPITCH) - INT(pMONSTER->m_wPITCH);
									pMONSTER->m_nDIR = INT(wDIR) - INT(pMONSTER->m_wDIR);

									pMONSTER->m_bMouseDIR = TKDIR_F;
									pMONSTER->m_bKeyDIR = TKDIR_F;

									LPTMONMOVE pMOVE = new TMONMOVE(
										pMONSTER,
										m_pMainWnd->m_bChannel,
										m_vMAP.m_wMapID,
										vTPOS.x,
										vTPOS.y,
										vTPOS.z,
										wPITCH,
										wDIR,
										pMONSTER->m_bMouseDIR,
										pMONSTER->m_bKeyDIR,
										pMONSTER->m_bAction);

									vTMONMOVE.push_back(pMOVE);
									bROAM = FALSE;

									pMONSTER->m_bBACK = TRUE;
								}
								else if( pMONSTER->m_bBACK )
								{
									if( fDist >= fAB*fAB )
									{
										pMONSTER->m_bBACK = FALSE;

										D3DXVECTOR3 vTDIR = vFOLLOW - vTPOS;

										WORD wPITCH = pMONSTER->GetTargetPITCH(
											vTDIR.x,
											vTDIR.y,
											vTDIR.z);

										WORD wDIR = pMONSTER->GetTargetDIR(
											vTDIR.x,
											vTDIR.z);

										pMONSTER->m_nPITCH = INT(wPITCH) - INT(pMONSTER->m_wPITCH);
										pMONSTER->m_nDIR = INT(wDIR) - INT(pMONSTER->m_wDIR);

										pMONSTER->m_bMouseDIR = TKDIR_N;
										pMONSTER->m_bKeyDIR = TKDIR_N;

										LPTMONMOVE pMOVE = new TMONMOVE(
											pMONSTER,
											m_pMainWnd->m_bChannel,
											m_vMAP.m_wMapID,
											vTPOS.x,
											vTPOS.y,
											vTPOS.z,
											wPITCH,
											wDIR,
											pMONSTER->m_bMouseDIR,
											pMONSTER->m_bKeyDIR,
											pMONSTER->m_bAction);

										vTMONMOVE.push_back(pMOVE);
										bROAM = FALSE;
									}
									else
									{
										D3DXVECTOR3 vTDIR = vTPOS - vFOLLOW;

										WORD wPITCH = pMONSTER->GetTargetPITCH(
											vTDIR.x,
											vTDIR.y,
											vTDIR.z);

										WORD wDIR = pMONSTER->GetTargetDIR(
												vTDIR.x,
												vTDIR.z);

										if( pMONSTER->CheckDIR( wDIR, wPITCH) || pMONSTER->m_bKeyDIR == TKDIR_N )
										{
											pMONSTER->m_nPITCH = INT(wPITCH) - INT(pMONSTER->m_wPITCH);
											pMONSTER->m_nDIR = INT(wDIR) - INT(pMONSTER->m_wDIR);

											pMONSTER->m_bMouseDIR = TKDIR_F;
											pMONSTER->m_bKeyDIR = TKDIR_F;

											LPTMONMOVE pMOVE = new TMONMOVE(
												pMONSTER,
												m_pMainWnd->m_bChannel,
												m_vMAP.m_wMapID,
												vTPOS.x,
												vTPOS.y,
												vTPOS.z,
												wPITCH,
												wDIR,
												pMONSTER->m_bMouseDIR,
												pMONSTER->m_bKeyDIR,
												pMONSTER->m_bAction);

											vTMONMOVE.push_back(pMOVE);
											bROAM = FALSE;
										}
									}
								}
								else
								{
									D3DXVECTOR3 vTDIR = vFOLLOW - vTPOS;

									WORD wPITCH = pMONSTER->GetTargetPITCH(
										vTDIR.x,
										vTDIR.y,
										vTDIR.z);

									WORD wDIR = pMONSTER->GetTargetDIR(
										vTDIR.x,
										vTDIR.z);

									if( pMONSTER->CheckDIR( wDIR, wPITCH) || pMONSTER->m_bKeyDIR == TKDIR_F )
									{
										pMONSTER->m_nPITCH = INT(wPITCH) - INT(pMONSTER->m_wPITCH);
										pMONSTER->m_nDIR = INT(wDIR) - INT(pMONSTER->m_wDIR);

										pMONSTER->m_bMouseDIR = TKDIR_N;
										pMONSTER->m_bKeyDIR = TKDIR_N;

										LPTMONMOVE pMOVE = new TMONMOVE(
											pMONSTER,
											m_pMainWnd->m_bChannel,
											m_vMAP.m_wMapID,
											vTPOS.x,
											vTPOS.y,
											vTPOS.z,
											wPITCH,
											wDIR,
											pMONSTER->m_bMouseDIR,
											pMONSTER->m_bKeyDIR,
											pMONSTER->m_bAction);

										vTMONMOVE.push_back(pMOVE);
										bROAM = FALSE;
									}
								}
							}
							else if( pMONSTER->m_bKeyDIR != TKDIR_N )
							{
								pMONSTER->m_bMouseDIR = TKDIR_N;
								pMONSTER->m_bKeyDIR = TKDIR_N;

								LPTMONMOVE pMOVE = new TMONMOVE(
									pMONSTER,
									m_pMainWnd->m_bChannel,
									m_vMAP.m_wMapID,
									vTPOS.x,
									vTPOS.y,
									vTPOS.z,
									pMONSTER->m_wPITCH,
									pMONSTER->m_wDIR,
									pMONSTER->m_bMouseDIR,
									pMONSTER->m_bKeyDIR,
									pMONSTER->m_bAction);

								vTMONMOVE.push_back(pMOVE);
								bROAM = FALSE;
							}
						}
                        
						if( pMONSTER->m_bTD != TD_AB )
						{
							pMONSTER->m_bFollowType = pFollow->m_bType;
							pMONSTER->m_dwFollowID = pFollow->m_dwID;
							pMONSTER->m_bTD = TD_AB;

							SendCS_ENTERAB_REQ(
								pFollow->GetHostID(),
								pFollow->m_dwID,
								pFollow->m_bType,
								pMONSTER->m_dwID,
								m_pMainWnd->m_bChannel,
								m_vMAP.m_wMapID);
							
							if( fAB < TPATH_LONG )
								pFollow->AddAdjacency( pMONSTER );
						}
					}
				}
			}

			if(bROAM)
			{
				FLOAT fDist = D3DXVec2LengthSq(&D3DXVECTOR2(
					pMONSTER->m_vTGOAL.x - vTPOS.x,
					pMONSTER->m_vTGOAL.z - vTPOS.z));

				BYTE bCORRECT = pMONSTER->m_bUPDATE;
				WORD wPITCH = pMONSTER->m_wPITCH;
				WORD wDIR = pMONSTER->m_wDIR;

				if( fDist >= TROAM_BOUND * TROAM_BOUND &&
					pMONSTER->m_bGoHome &&
					pMONSTER->m_bAction != TA_RUN )
				{
					pMONSTER->m_bMouseDIR = TKDIR_F;
					pMONSTER->m_bKeyDIR = TKDIR_F;
					pMONSTER->SetTAction(TA_RUN);

					pMONSTER->m_bPATH = TRUE;
					bCORRECT = TRUE;
				}
				else if( pMONSTER->m_bAction == TA_RUN || pMONSTER->m_bAction == TA_WALK )
				{
					pMONSTER->m_bMouseDIR = TKDIR_F;
					pMONSTER->m_bKeyDIR = TKDIR_F;
				}

				if( pMONSTER->IsRoam() &&
					!pMONSTER->IsDead() &&
					!pMONSTER->IsBlock() &&
					!pMONSTER->IsDown() )
					if( fDist < TROAM_BOUND * TROAM_BOUND )
					{
						pMONSTER->m_bMouseDIR = TKDIR_N;
						pMONSTER->m_bKeyDIR = TKDIR_N;
						pMONSTER->SetTAction(TA_STAND);

						bCORRECT = TRUE;
					}
					else if( pMONSTER->m_bPATH || bPATH )
					{
						D3DXVECTOR3 vTARGET = pMONSTER->m_vTGOAL;
						D3DXVECTOR3 vTSTART = vTPOS;

						vTARGET.y += pMONSTER->m_fSizeY / 2.0f;
						vTSTART.y += TPATH_HEIGHT;

						FLOAT fTGoalY = m_vMAP.GetHeight(
							pMONSTER,
							NULL,
							&vTARGET,
							0.0f, FALSE) + TPATH_HEIGHT;

						vTARGET.y = pMONSTER->m_bSwim ? min(
							vTSTART.y, max(
							fTGoalY,
							vTARGET.y)) : fTGoalY;

						vTARGET = m_vMAP.GetTargetPoint(
							pMONSTER,
							&vTARGET) - vTSTART;

						wPITCH = pMONSTER->GetTargetPITCH(
							vTARGET.x,
							vTARGET.y,
							vTARGET.z);

						wDIR = pMONSTER->GetTargetDIR(
							vTARGET.x,
							vTARGET.z);

						if(pMONSTER->CheckDIR( wDIR, wPITCH))
						{
							pMONSTER->m_nPITCH = INT(wPITCH) - INT(pMONSTER->m_wPITCH);
							pMONSTER->m_nDIR = INT(wDIR) - INT(pMONSTER->m_wDIR);

							bCORRECT = TRUE;
						}
					}

				if(bCORRECT)
				{
					LPTMONMOVE pMOVE = new TMONMOVE(
						pMONSTER,
						m_pMainWnd->m_bChannel,
						m_vMAP.m_wMapID,
						vTPOS.x,
						vTPOS.y,
						vTPOS.z,
						wPITCH,
						wDIR,
						pMONSTER->m_bMouseDIR,
						pMONSTER->m_bKeyDIR,
						pMONSTER->m_bAction);

					vTMONMOVE.push_back(pMOVE);
				}
			}

			pMONSTER->m_bUPDATE = FALSE;
			pMONSTER->m_bPATH = FALSE;
		}

	if( !vPATHMON.empty() )
	{
		BYTE bAdjacency[ ADJACENCY_COUNT ];
		memcpy( bAdjacency, m_pMainChar->m_bAdjacencyMon, sizeof(BYTE)*ADJACENCY_COUNT );

		TOBJ_LENGTH_SORTER sort;
		sort.m_vPOSCHAR = m_pMainChar->GetPosition();
		std::sort( vPATHMON.begin(), vPATHMON.end(), sort );

		std::vector<CTClientMonster*>::iterator itPATHMON, end;
		itPATHMON = vPATHMON.begin();
		end = vPATHMON.end();

		for(; itPATHMON != end ; ++itPATHMON )
		{
			BOOL bDEFAULT = FALSE;
			CTClientMonster* pMONSTER = (*itPATHMON);

			CTClientObjBase *pFollow = FindOBJ(
				pMONSTER->m_dwFollowID,
				pMONSTER->m_bFollowType);

			D3DXVECTOR3 vTPOS = pMONSTER->GetPosition();
			D3DXVECTOR3 vFOLLOW = pFollow->GetPosition();
			D3DXVECTOR3 vTARGET = vFOLLOW;	
			D3DXVECTOR3 vTSTART = vTPOS;

			FLOAT fAB = pMONSTER->GetAB(pFollow);
			FLOAT fDist = D3DXVec2Length( &D3DXVECTOR2( vFOLLOW.x - vTPOS.x, vFOLLOW.z - vTPOS.z ) );

			if( pFollow == m_pMainChar &&
				fDist - fAB > 0.125f )
			{
				D3DXVECTOR3 n = vTPOS - vFOLLOW;
				D3DXVec3Normalize( &n, &n );
				INT index = m_pMainChar->FindNearAdjacencyIndex( n );

				INT SmallIndex = -1;
				BYTE SmallValue = 255;

				if( index >= 0 && index < ADJACENCY_COUNT )
				{
					SmallIndex = index;
					SmallValue = bAdjacency[ index ];

					D3DXVECTOR3 vCROSS;
					D3DXVec3Cross( &vCROSS, &(CTClientObjBase::m_vADJACENCY[ index ]), &n );

					// 가중치에 의해서 Left와 Right의 오프셋을 조절하자.
					INT nLeftCount = 5;
					INT nRightCount = 5;

					if( vCROSS.y >= 0.0f ) //기준 벡터에서 오른쪽에 있으면 크로스 결과벡터의 Y축이 양수다.
					{
						nLeftCount -= 1;
						nRightCount += 1;
					}
					else if( vCROSS.y < 0.0f ) //기준 벡터에서 왼쪽에 있으면 크로스 결과벡터의 Y축이 음수다.
					{
						nLeftCount += 1;
						nRightCount -= 1;
					}

					// Right
					for( INT i=1 ; i < nRightCount ; ++i )
					{
						INT RightIndex = m_pMainChar->GetAdjacencyIndex( index + i );

						if( SmallValue > bAdjacency[ RightIndex ] )
						{
							SmallIndex = RightIndex;
							SmallValue = bAdjacency[ RightIndex ];
						}
					}

					// Left
					for( INT i=1 ; i < nLeftCount ; ++i )
					{
						INT LeftIndex = m_pMainChar->GetAdjacencyIndex( index - i );

						if( SmallValue > bAdjacency[ LeftIndex ] )
						{
							SmallIndex = LeftIndex;
							SmallValue = bAdjacency[ LeftIndex ];
						}
					}

					if( SmallIndex != -1 && 
						fAB < TPATH_LONG )
					{
						INT nCount = pMONSTER->GetSizeGrade();

						for( INT j=1 ; j < nCount+1 ; ++j )
						{
							INT nLeft = pMONSTER->GetAdjacencyIndex( SmallIndex + j );
							INT nRight = pMONSTER->GetAdjacencyIndex( SmallIndex - j);

							if( bAdjacency[ nLeft ] < 1 )
								bAdjacency[ nLeft ] = 1;

							if( bAdjacency[ nRight ] < 1 )
								bAdjacency[ nRight ] = 1;
						}

						bAdjacency[ SmallIndex ] += 1;
						vTARGET = CTClientObjBase::m_vADJACENCY[ SmallIndex ] * (fAB - CTClientObjBase::m_fABOFFSET);

						if( bAdjacency[ SmallIndex ] > 1)
						{
							D3DXMATRIXA16 matY;
							D3DXMatrixRotationY(
								&matY,
								(rand()%2 ? -1.0f : 1.0f) * (rand()%10) * D3DX_PI / 180.0f );

							D3DXVec3TransformCoord(
								&vTARGET,
								&vTARGET,
								&matY );
						}

						vTARGET.x += vFOLLOW.x;
						vTARGET.y = vFOLLOW.y + TPATH_HEIGHT;
						vTARGET.z += vFOLLOW.z;
						vTSTART.y += TPATH_HEIGHT;

						if( m_vMAP.CheckCollisionOBJ(
							vTARGET.x,
							vTARGET.z,
							pMONSTER->m_fRadius ) )
						{		
							vTARGET = CTClientObjBase::m_vADJACENCY[ SmallIndex ] * TPATH_RADIUS;
							vTARGET.x += vFOLLOW.x;
							vTARGET.y = vFOLLOW.y + TPATH_HEIGHT;
							vTARGET.z += vFOLLOW.z;
						}
					}
					else
						bDEFAULT = TRUE;
				}
				else
					bDEFAULT = TRUE;
			}
			else
			{
				vTARGET.y += TPATH_HEIGHT;
				vTSTART.y += TPATH_HEIGHT;
				bDEFAULT = FALSE;
			}

			if( bDEFAULT )
			{
				FLOAT fThata = FLOAT(rand() % 8) * 45.0f * D3DX_PI / 180.0f;
				vTARGET.x += TPATH_RADIUS * cosf(fThata);
				vTARGET.y += TPATH_HEIGHT;
				vTARGET.z += TPATH_RADIUS * sinf(fThata);
				vTSTART.y += TPATH_HEIGHT;
			}

			D3DXVECTOR3 vTGOAL = m_vMAP.GetTargetPoint(
				pMONSTER,
				&vTARGET) - vTSTART;

			WORD wPITCH = pMONSTER->GetTargetPITCH(
				vTGOAL.x,
				vTGOAL.y,
				vTGOAL.z);

			WORD wDIR = pMONSTER->GetTargetDIR(
				vTGOAL.x,
				vTGOAL.z);

			if( pMONSTER->CheckDIR( wDIR, wPITCH) || pMONSTER->m_bKeyDIR == TKDIR_N )
			{
				pMONSTER->m_nPITCH = INT(wPITCH) - INT(pMONSTER->m_wPITCH);
				pMONSTER->m_nDIR = INT(wDIR) - INT(pMONSTER->m_wDIR);

				pMONSTER->m_bMouseDIR = TKDIR_F;
				pMONSTER->m_bKeyDIR = TKDIR_F;

				LPTMONMOVE pMOVE = new TMONMOVE(
					pMONSTER,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					vTPOS.x,
					vTPOS.y,
					vTPOS.z,
					wPITCH,
					wDIR,
					pMONSTER->m_bMouseDIR,
					pMONSTER->m_bKeyDIR,
					pMONSTER->m_bAction);

				vTMONMOVE.push_back(pMOVE);
			}
		}
	}
	vPATHMON.clear();

	if(!vTMONMOVE.empty())
		SendCS_MONMOVE_REQ(&vTMONMOVE);

	while(!vTMONMOVE.empty())
	{
		delete vTMONMOVE.back();
		vTMONMOVE.pop_back();
	}
}

void CTClientGame::CalcMONSTER( DWORD dwTick)
{
	MAPMONSTER::iterator itMON;

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
	{
		CTClientMonster *pMON = (*itMON).second;
		BYTE bHandling = m_mapSLAVEMON.find(pMON->m_dwID) != m_mapSLAVEMON.end() ? TRUE : FALSE;
		LPTREGIONINFO pTREGION = pMON->m_pTREGION;
		D3DXVECTOR3 vPOS = pMON->GetPosition();

		pMON->CalcDIR(
			dwTick,
			pMON->m_bMouseDIR,
			pMON->m_bKeyDIR);

		pMON->CalcMaintain(
			dwTick,
			bHandling,
			this);

		if( !m_pMainChar->m_bGhost || D3DXVec2Length( &D3DXVECTOR2(
			vPOS.x - m_pMainChar->GetPositionX(),
			vPOS.z - m_pMainChar->GetPositionZ())) < CELL_SIZE )
		{
			BYTE bCHECK = pMON->m_bSlide || pMON->IsPush() ? TRUE : FALSE;

			pMON->CalcTick(
				&vPOS,
				m_pDevice,
				m_pRES,
				dwTick);

			if(bCHECK)
			{
				FLOAT fNextH = vPOS.y;
				D3DXVECTOR3 vTNEXT;

				pMON->ForecastHeight(
					&m_vMAP,
					&fNextH,
					NULL,
					dwTick);

				if(m_vMAP.CheckCollision( pMON, &vTNEXT, &vPOS, fNextH))
				{
					pMON->m_vWorld._41 += vTNEXT.x - pMON->GetPositionX();
					pMON->m_vWorld._43 += vTNEXT.z - pMON->GetPositionZ();

					pMON->CalcTick(
						&vPOS,
						m_pDevice,
						m_pRES, 0);

					if(bHandling)
						pMON->m_bUPDATE = TRUE;
				}
			}

			pMON->CalcHeight(
				&vPOS,
				&m_vMAP,
				dwTick);
		}

		CalcTOBJDamage(
			pMON,
			dwTick);

		if( bHandling && pMON->m_pTREGION != pTREGION )
		{
			SendCS_REGION_REQ(
				pMON->m_dwID,
				pMON->m_bType,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID,
				pMON->m_pTREGION->m_dwID,
				pMON->m_pTREGION->m_wLocalID);
		}

		pMON->m_dwDropDamage = 0;
		ReportTRSCSMOVE(pMON);
	}
}

void CTClientGame::CalcTSWITCH( DWORD dwTick)
{
	MAPTSWITCH::iterator itTSWITCH;

	for( itTSWITCH = m_mapTSwitch.begin(); itTSWITCH != m_mapTSwitch.end(); itTSWITCH++)
		(*itTSWITCH).second->CalcTick( m_pDevice->m_pDevice, dwTick);
}

void CTClientGame::CalcSLAVERECALL( DWORD dwTick)
{
	static DWORD dwPATH = 0;
	static DWORD dwSYNC = 0;

	MAPRECALL::iterator itTRECALL;
	D3DXMATRIX vTROT;

	VTMONMOVE vTMONMOVE;
	VTRECALL vTATTACK;

	BYTE bSYNC = FALSE;
	BYTE bPATH = FALSE;

	D3DXMatrixRotationY(
		&vTROT,
		FLOAT(m_pMainChar->m_wDIR) * D3DX_PI / 900.0f);

	if( dwSYNC > SYNC_TICK )
	{
		bSYNC = TRUE;
		dwSYNC = 0;
	}
	else
		dwSYNC += dwTick;

	if( dwPATH > PATH_TICK )
	{
		bPATH = TRUE;
		dwPATH = 0;
	}
	else
		dwPATH += dwTick;

	for( itTRECALL = m_mapSLAVEFIXRECALL.begin(); itTRECALL != m_mapSLAVEFIXRECALL.end(); itTRECALL++)
		if( !IsCtrlRECALL((*itTRECALL).second) &&
			!(*itTRECALL).second->IsDead() &&
			!(*itTRECALL).second->IsBlock() &&
			!(*itTRECALL).second->IsPush() )
			CalcSLAVERECALL( (*itTRECALL).second, &vTROT, &vTMONMOVE, &vTATTACK, bSYNC, bPATH, dwTick);

	for( itTRECALL = m_mapSLAVERECALL.begin(); itTRECALL != m_mapSLAVERECALL.end(); itTRECALL++)
		if( !IsCtrlRECALL((*itTRECALL).second) &&
			!(*itTRECALL).second->IsDead() &&
			!(*itTRECALL).second->IsBlock() &&
			!(*itTRECALL).second->IsPush() )
			CalcSLAVERECALL( (*itTRECALL).second, &vTROT, &vTMONMOVE, &vTATTACK, bSYNC, bPATH, dwTick);

	if(!vTMONMOVE.empty())
		SendCS_MONMOVE_REQ(&vTMONMOVE);

	while(!vTMONMOVE.empty())
	{
		delete vTMONMOVE.back();
		vTMONMOVE.pop_back();
	}

	if(!vTATTACK.empty())
	{
		for( int i=0; i<INT(vTATTACK.size()); i++)
		{
			if( !UseTRecallSkill( vTATTACK[i], dwTick) &&
				vTATTACK[i]->m_bType == OT_SELF )
			{
				if( vTATTACK[i]->m_bMode != MT_NORMAL )
				{
					m_pSESSION->SendCS_CHGMODERECALLMON_REQ( vTATTACK[i]->m_dwID, MT_NORMAL);
					vTATTACK[i]->m_bMode = MT_NORMAL;
				}

				vTATTACK[i]->m_bTargetType = OT_NONE;
				vTATTACK[i]->m_dwTargetID = 0;
			}

			if(vTATTACK[i]->m_pTDEFSKILL)
			{
				vTATTACK[i]->m_pTCURSKILL = vTATTACK[i]->m_pTDEFSKILL;
				vTATTACK[i]->m_pTDEFSKILL = NULL;
			}
		}

		vTATTACK.clear();
	}
}

void CTClientGame::CalcSLAVERECALL( CTClientRecall *pTRECALL,
								    LPD3DXMATRIX pTDIR,
									LPVTMONMOVE pTMONMOVE,
									LPVTRECALL pTATTACK,
									BYTE bSYNC,
								    BYTE bPATH,
									DWORD dwTick)
{
	BOOL bOnSDT_RUNAWAY = !pTRECALL->m_vRecallRunAway.empty();

	CTClientObjBase *pTARGET = pTRECALL->m_bTargetType != OT_NONE ? FindOBJ(
		pTRECALL->m_dwTargetID,
		pTRECALL->m_bTargetType) : NULL;

	D3DXVECTOR3 vTPOS = pTRECALL->GetPosition();
	BYTE bROAM = TRUE;

	if( !pTARGET && pTRECALL->m_bAI == TRECALLAI_ACTIVE )
		pTARGET = SeekRecallTarget(pTRECALL);

	if( pTARGET && (pTRECALL->m_bFlash || !pTRECALL->m_pTCURSKILL ||
		pTRECALL->m_bSubAI != TRECALLAI_COUNT || pTARGET->IsDead() || (
		pTARGET->m_bHide && !pTARGET->m_bDetect)) )
		pTARGET = NULL;

	if(pTARGET && !bOnSDT_RUNAWAY)
	{
		D3DXVECTOR3 vTARGET = pTARGET->GetPosition();

		FLOAT fRANGE = pTRECALL->GetMinRange(
			pTARGET,
			pTRECALL->m_pTCURSKILL->m_pTSKILL);
		FLOAT fDIST = D3DXVec3LengthSq(&(vTARGET - vTPOS));

		pTRECALL->m_bTargetType = pTARGET->m_bType;
		pTRECALL->m_dwTargetID = pTARGET->m_dwID;

		if( CTClientSkill::IsAttackSKILL(pTRECALL->m_pTCURSKILL->m_pTSKILL) && pTRECALL->m_bMode != MT_BATTLE )
		{
			m_pSESSION->SendCS_CHGMODERECALLMON_REQ( pTRECALL->m_dwID, MT_BATTLE);
			pTRECALL->m_bMode = MT_BATTLE;
		}

		if( fDIST < fRANGE * fRANGE )
			pTRECALL->m_pTCURSKILL = pTRECALL->GetBestTSKILL(pTARGET);

		fRANGE = TMAXRANGE_RATIO * pTRECALL->GetMaxRange(
			pTARGET,
			pTRECALL->m_pTCURSKILL->m_pTSKILL);

		if( fDIST < fRANGE * fRANGE )
		{
			D3DXVECTOR3 vTDIR = vTARGET - vTPOS;
			BYTE bCORRECT = FALSE;

			WORD wPITCH = pTRECALL->GetTargetPITCH(
				vTDIR.x,
				vTDIR.y,
				vTDIR.z);

			WORD wDIR = pTRECALL->GetTargetDIR(
				vTDIR.x,
				vTDIR.z);

			if( pTRECALL->m_bAction != TA_STAND )
			{
				pTRECALL->m_bMouseDIR = TKDIR_N;
				pTRECALL->m_bKeyDIR = TKDIR_N;
				pTRECALL->SetTAction(TA_STAND);

				bCORRECT = TRUE;
			}

			if( bCORRECT || pTRECALL->CheckDIR( wDIR, wPITCH) )
			{
				pTRECALL->m_nPITCH = INT(wPITCH) - INT(pTRECALL->m_wPITCH);
				pTRECALL->m_nDIR = INT(wDIR) - INT(pTRECALL->m_wDIR);

				LPTMONMOVE pMOVE = new TMONMOVE(
					pTRECALL,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					vTPOS.x,
					vTPOS.y,
					vTPOS.z,
					wPITCH,
					wDIR,
					pTRECALL->m_bMouseDIR,
					pTRECALL->m_bKeyDIR,
					pTRECALL->m_bAction);

				pTMONMOVE->push_back(pMOVE);
				bROAM = FALSE;
			}

			if(pTRECALL->IsReadyForUSE(pTRECALL->m_pTCURSKILL))
			{
				pTRECALL->m_pTDEFSKILL = pTRECALL->GetBestTSKILL(pTARGET);
				pTATTACK->push_back(pTRECALL);
			}
		}
		else if( bPATH && pTRECALL->m_bType != OT_SELF )
		{
			D3DXVECTOR3 vTGOAL = vTARGET;
			D3DXVECTOR3 vSTART = vTPOS;

			vTGOAL.y += TPATH_HEIGHT;
			vSTART.y += TPATH_HEIGHT;

			vTGOAL = m_vMAP.GetTargetPoint(
				pTRECALL,
				&vTGOAL) - vSTART;

			WORD wPITCH = pTRECALL->GetTargetPITCH(
				vTGOAL.x,
				vTGOAL.y,
				vTGOAL.z);

			WORD wDIR = pTRECALL->GetTargetDIR(
				vTGOAL.x,
				vTGOAL.z);

			if( pTRECALL->m_bAction != TA_RUN ||
				pTRECALL->CheckDIR( wDIR, wPITCH) )
			{
				pTRECALL->m_bMouseDIR = TKDIR_F;
				pTRECALL->m_bKeyDIR = TKDIR_F;
				pTRECALL->SetTAction(TA_RUN);

				pTRECALL->m_nPITCH = INT(wPITCH) - INT(pTRECALL->m_wPITCH);
				pTRECALL->m_nDIR = INT(wDIR) - INT(pTRECALL->m_wDIR);

				LPTMONMOVE pMOVE = new TMONMOVE(
					pTRECALL,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					vTPOS.x,
					vTPOS.y,
					vTPOS.z,
					wPITCH,
					wDIR,
					pTRECALL->m_bMouseDIR,
					pTRECALL->m_bKeyDIR,
					pTRECALL->m_bAction);

				pTMONMOVE->push_back(pMOVE);
				bROAM = FALSE;
			}
		}
	}
	else
	{
		if( pTRECALL->m_bMode != MT_NORMAL )
		{
			m_pSESSION->SendCS_CHGMODERECALLMON_REQ( pTRECALL->m_dwID, MT_NORMAL);
			pTRECALL->m_bMode = MT_NORMAL;
		}

		pTRECALL->m_bTargetType = OT_NONE;
		pTRECALL->m_dwTargetID = 0;
	}

	if(bROAM || bOnSDT_RUNAWAY)
	{
		WORD wPITCH = pTRECALL->m_wPITCH;
		WORD wDIR = pTRECALL->m_wDIR;
		BYTE bCORRECT = FALSE;

		if(!pTARGET || bOnSDT_RUNAWAY)
		{
			D3DXVECTOR3 vTARGET = m_pMainChar->GetPosition();

			// 공포디버프구현
			if( bOnSDT_RUNAWAY ) // 공포효과가 없으면 그 타겟을 대상으로.
			{
				TRUNAWAY* pDATA = &(pTRECALL->m_vRecallRunAway[ pTRECALL->m_nRecallRunAwayIndex ]);
                
				if( pDATA->m_dwTime <= dwTick )
				{
					if( pTRECALL->m_nRecallRunAwayIndex < TRUNAWAYPOINT_COUNT )
						++pTRECALL->m_nRecallRunAwayIndex;
					else
						pTRECALL->m_nRecallRunAwayIndex = 0;

					pTRECALL->m_vRecallRunAwayTarget = ScreenClickToWorldPosition(
						pDATA->m_ptScreen,
						pTRECALL);

					pDATA->m_dwTime = (rand()%(TRUNAWAYTIME_MAX-TRUNAWAYTIME_MIN)) + TRUNAWAYTIME_MIN;
					pDATA->m_ptScreen.x = rand() % m_pDevice->m_option.m_dwScreenX;
					pDATA->m_ptScreen.y = rand() % m_pDevice->m_option.m_dwScreenY;
				}
				else
					pDATA->m_dwTime -= dwTick;

				vTARGET = pTRECALL->m_vRecallRunAwayTarget;
			}

			FLOAT fDIST = D3DXVec2LengthSq(&D3DXVECTOR2(
				vTARGET.x - vTPOS.x,
				vTARGET.z - vTPOS.z));
			FLOAT fRadius = m_pMainChar->m_fRadius + pTRECALL->m_fRadius;

			BYTE bActionID = pTRECALL->m_bAction;
			vTARGET.y += TPATH_HEIGHT;

			if( bActionID != TA_STAND || fDIST > TRECALL_STAND_BOUND * TRECALL_STAND_BOUND || fDIST < fRadius * fRadius )
			{
				if( !bOnSDT_RUNAWAY )
				{
					vTARGET = pTRECALL->GetRoamTarget(
						pTDIR,
						vTARGET.x,
						vTARGET.y,
						vTARGET.z);
					bActionID = pTRECALL->GetRoamACT(&vTARGET);
				}
				else
				{
					bActionID = TA_RUN;					
				}
			}

			if( bActionID != pTRECALL->m_bAction )
			{
				pTRECALL->SetTAction(bActionID);
				bCORRECT = TRUE;
			}

			if( bCORRECT || bPATH )
				if( pTRECALL->m_bAction == TA_STAND )
				{
					pTRECALL->m_bMouseDIR = TKDIR_N;
					pTRECALL->m_bKeyDIR = TKDIR_N;

					if( pTRECALL->m_bSubAI == TRECALLAI_BACK )
						pTRECALL->m_bSubAI = TRECALLAI_COUNT;
				}
				else
				{
					D3DXVECTOR3 vTSTART = vTPOS;
					vTSTART.y += TPATH_HEIGHT;

					D3DXVECTOR3 vTARGET_Prev = vTARGET;

					FLOAT fHEIGHT = m_vMAP.GetHeight(
						pTRECALL,
						NULL,
						&vTARGET,
						0.0f, FALSE) + TPATH_HEIGHT;

					if( fabs(fHEIGHT - vTARGET.y) > m_pMainChar->m_fSizeY )
					{
						vTARGET = m_pMainChar->GetPosition();
						vTARGET.y += TPATH_HEIGHT;
					}
					else
						vTARGET.y = fHEIGHT;

					if(m_pMainChar->IsFlyOBJ())
						vTARGET.y = max( vTARGET.y, m_pMainChar->GetPositionY() + TPATH_HEIGHT) + pTRECALL->m_fBreathHeight;

					vTARGET = m_vMAP.GetTargetPoint(
						pTRECALL,
						&vTARGET) - vTSTART;

					wPITCH = pTRECALL->GetTargetPITCH(
						vTARGET.x,
						vTARGET.y,
						vTARGET.z);

					wDIR = pTRECALL->GetTargetDIR(
						vTARGET.x,
						vTARGET.z);

					if(bCORRECT)
					{
						pTRECALL->m_bMouseDIR = TKDIR_F;
						pTRECALL->m_bKeyDIR = TKDIR_F;
					}

					if(pTRECALL->CheckDIR( wDIR, wPITCH))
					{
						pTRECALL->m_nPITCH = INT(wPITCH) - INT(pTRECALL->m_wPITCH);
						pTRECALL->m_nDIR = INT(wDIR) - INT(pTRECALL->m_wDIR);
						bCORRECT = TRUE;
					}
				}
		}

		if( bSYNC || bCORRECT )
		{
			LPTMONMOVE pMOVE = new TMONMOVE(
				pTRECALL,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID,
				vTPOS.x,
				vTPOS.y,
				vTPOS.z,
				wPITCH,
				wDIR,
				pTRECALL->m_bMouseDIR,
				pTRECALL->m_bKeyDIR,
				pTRECALL->m_bAction);

			pTMONMOVE->push_back(pMOVE);
		}
	}
}

void CTClientGame::CalcRECALL( DWORD dwTick)
{
	BYTE bRESET = FALSE;

	MAPRECALL::iterator itFIXRECALL = m_mapFIXRECALL.begin();
	while( itFIXRECALL != m_mapFIXRECALL.end() )
	{
		CTClientRecall* pRecall = (*itFIXRECALL).second;

		if( !pRecall->m_bDEAD )
		{
			CalcRECALL( pRecall, dwTick);
			++itFIXRECALL;
		}
		else
		{
			DeleteFixRecall( pRecall->m_dwID, false );
			itFIXRECALL = m_mapFIXRECALL.erase( itFIXRECALL );
		}
	}

	MAPRECALL::iterator itRECALL = m_mapRECALL.begin();
	while( itRECALL != m_mapRECALL.end() )
	{
		CTClientRecall* pRecall = (*itRECALL).second;

		if( !pRecall->m_bDEAD )
		{
			if( m_pCtrlHost != pRecall )
				CalcRECALL( pRecall, dwTick);

			++itRECALL;
		}
		else
		{
			bRESET = DeleteRecall(pRecall->m_dwID, false);
			itRECALL = m_mapRECALL.erase(itRECALL);
		}
	}

	if(bRESET)
		ResetRecallINFO(FALSE);

	ResetRecallTIME(dwTick);
}

void CTClientGame::CalcRECALL( CTClientRecall *pTRECALL,
							   DWORD dwTick)
{
	BYTE bHandling = IsHostingOBJ( pTRECALL->m_dwID, pTRECALL->m_bType);
	LPTREGIONINFO pTREGION = pTRECALL->m_pTREGION;
	D3DXVECTOR3 vPOS = pTRECALL->GetPosition();

	pTRECALL->CalcDIR(
		dwTick,
		pTRECALL->m_bMouseDIR,
		pTRECALL->m_bKeyDIR);

	pTRECALL->CalcMaintain(
		dwTick,
		bHandling,
		this);

	if( pTRECALL->m_bRecallType == TRECALLTYPE_PET )
	{
		CTClientChar *pTRIDER = ((CTClientPet *) pTRECALL)->GetTakeUpChar();

		if(pTRIDER)
			D3DXMatrixIdentity(&pTRIDER->m_vWorld);
	}

	pTRECALL->CalcTick(
		&vPOS,
		m_pDevice,
		m_pRES,
		dwTick);

	if( IsCtrlRECALL(pTRECALL) && (bHandling || pTRECALL->m_bBlock) )
	{
		D3DXVECTOR3 vTNEXT = pTRECALL->GetPosition();
		FLOAT fNextH = vPOS.y;

		pTRECALL->ForecastHeight(
			&m_vMAP,
			&fNextH,
			NULL,
			dwTick);

		if( pTRECALL == m_pCtrlHost )
			CheckTPORTAL( pTRECALL, &vPOS, &vTNEXT, fNextH);

		BYTE bBLOCK = m_vMAP.CheckCollision(
			pTRECALL,
			&vTNEXT,
			&vPOS,
			fNextH);

		if(bHandling)
		{
			FLOAT fMove = D3DXVec2Length(&D3DXVECTOR2(
				vTNEXT.x - vPOS.x,
				vTNEXT.z - vPOS.z));

			if( m_vMAP.GetHeight( pTRECALL, NULL, &vTNEXT, fMove, FALSE) > vPOS.y + fMove / CTClientMAP::m_fMoveBound )
			{
				vTNEXT = vPOS;
				bBLOCK = TRUE;
			}
		}

		if(bBLOCK)
		{
			pTRECALL->m_vWorld._41 += vTNEXT.x - pTRECALL->GetPositionX();
			pTRECALL->m_vWorld._43 += vTNEXT.z - pTRECALL->GetPositionZ();

			pTRECALL->CalcTick(
				&vPOS,
				m_pDevice,
				m_pRES, 0);

			if( bHandling && !m_pMainChar->m_bGhost && !pTRECALL->m_bBlock )
			{
				pTRECALL->m_bBlock = TRUE;

				SendCS_BLOCK_REQ(
					pTRECALL->m_dwID,
					pTRECALL->m_bType,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					pTRECALL->GetPositionX(),
					pTRECALL->GetPositionY(),
					pTRECALL->GetPositionZ(),
					pTRECALL->m_wPITCH,
					pTRECALL->m_wDIR,
					pTRECALL->m_bAction,
					pTRECALL->m_bBlock);

				if(GetPartyID(m_pMainChar))
					m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
			}
		}
		else if( bHandling && !m_pMainChar->m_bGhost && pTRECALL->m_bBlock )
		{
			pTRECALL->m_bBlock = FALSE;

			SendCS_BLOCK_REQ(
				pTRECALL->m_dwID,
				pTRECALL->m_bType,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID,
				pTRECALL->GetPositionX(),
				pTRECALL->GetPositionY(),
				pTRECALL->GetPositionZ(),
				pTRECALL->m_wPITCH,
				pTRECALL->m_wDIR,
				pTRECALL->m_bAction,
				pTRECALL->m_bBlock);

			if(GetPartyID(m_pMainChar))
				m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
		}
	}
	else if( pTRECALL->m_bSlide || pTRECALL->IsPush() )
	{
		FLOAT fNextH = vPOS.y;
		D3DXVECTOR3 vTNEXT;

		pTRECALL->ForecastHeight(
			&m_vMAP,
			&fNextH,
			NULL,
			dwTick);

		if(m_vMAP.CheckCollision( pTRECALL, &vTNEXT, &vPOS, fNextH))
		{
			pTRECALL->m_vWorld._41 += vTNEXT.x - pTRECALL->GetPositionX();
			pTRECALL->m_vWorld._43 += vTNEXT.z - pTRECALL->GetPositionZ();

			pTRECALL->CalcTick(
				&vPOS,
				m_pDevice,
				m_pRES, 0);
		}
	}

	pTRECALL->CalcHeight(
		&vPOS,
		&m_vMAP,
		dwTick);

	CalcTOBJDamage(
		pTRECALL,
		dwTick);

	if( pTRECALL->m_bRecallType == TRECALLTYPE_SKILL && pTRECALL->m_pTCURSKILL )
	{
		pTRECALL->m_pTCURSKILL->m_dwTick += dwTick;

		while( pTRECALL->m_pTCURSKILL->m_pTSKILL->m_dwDelay &&
			   pTRECALL->m_pTCURSKILL->m_dwTick >= pTRECALL->m_pTCURSKILL->m_pTSKILL->m_dwDelay )
		{
			pTRECALL->m_pTCURSKILL->m_dwTick -= pTRECALL->m_pTCURSKILL->m_pTSKILL->m_dwDelay;
			ShotSkill(pTRECALL, &pTRECALL->m_vTSKILLDATA);
		}
	}

	if(bHandling)
	{
		if( pTRECALL->m_pTREGION != pTREGION )
		{
			SendCS_REGION_REQ(
				pTRECALL->m_dwID,
				pTRECALL->m_bType,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID,
				pTRECALL->m_pTREGION->m_dwID,
				pTRECALL->m_pTREGION->m_wLocalID);
		}

		if( pTRECALL->m_dwDropDamage &&
			(pTRECALL->m_pTREGION ? pTRECALL->m_pTREGION->m_bContryID != TCONTRY_PEACE : TRUE) )
		{
			SendCS_DROPDAMAGE_REQ(
				pTRECALL->m_dwID,
				pTRECALL->m_bType,
				pTRECALL->m_dwDropDamage,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID);
		}

		if(pTRECALL->m_bDIE)
			m_pSESSION->SendCS_DELRECALLMON_REQ(pTRECALL->m_dwID, pTRECALL->m_bType);
	}

	if(pTRECALL->m_bDIE)
		pTRECALL->OnDIE( m_pDevice, m_pRES);

	pTRECALL->m_dwDropDamage = 0;
	pTRECALL->m_bDIE = FALSE;
	ReportTRSCSMOVE(pTRECALL);
}

void CTClientGame::RotCameraX( int nCamRotX)
{
	nCamRotX += INT(m_pCamera->m_wCamPITCH);

	while(nCamRotX < 0)
		nCamRotX += 1800;

	m_pCamera->m_wCamPITCH = WORD(nCamRotX % 1800);
	if( m_pCamera->m_wCamPITCH > 900 )
		m_pCamera->m_wCamPITCH = max( TPITCH_MAX, m_pCamera->m_wCamPITCH);
	else
		m_pCamera->m_wCamPITCH = min( TPITCH_MIN, m_pCamera->m_wCamPITCH);
}

void CTClientGame::RotCameraY( int nCamRotY)
{
	nCamRotY += INT(m_wCamDIR);

	while(nCamRotY < 0)
		nCamRotY += 1800;

	m_wCamDIR = WORD(nCamRotY % 1800);
}

void CTClientGame::CalcNoneInputLimitation(DWORD dwTick)
{
	CTClientRecall *pTRECALL = GetMainRecall();

	if( m_dwLeftNoneInputLimitationTime <= dwTick &&
		pTRECALL &&
		pTRECALL->m_bSubAI != TRECALLAI_STAY )
	{
		OnGM_RECALL_STAY_AI();

		m_pChatFrame->ChatSysMSG(
			CTChart::LoadString( TSTR_INFO_TITLE ),
			m_pMainChar->GetName(),
			CTChart::LoadString( TSTR_NOINPUT_LIMITATION),
			CTChatFrame::GetChatTypeColor(3),
			TCHAT_FLAG_WORLD,
			ID_SND_INFO,
			MEDIA_TSOUND);

		m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;
	}
	else
	{
		m_dwLeftNoneInputLimitationTime -= min( m_dwLeftNoneInputLimitationTime, dwTick );
	}
}

void CTClientGame::CalcChangeChannel(DWORD dwTick)
{
	if( !m_bDoChangeCH )
		return;

	if( m_dwLeftTickCH <= dwTick )
	{
		m_pMainWnd->CloseMessageBox(TRUE);

		m_bDoChangeCH = FALSE;
		m_pSESSION->SendCS_CHGCHANNEL_REQ(m_bChannelID);
	}
	else
	{
	 	DWORD dwLEFT = m_dwLeftTickCH - dwTick;
		if( m_dwLeftTickCH/TNOTIFY_PERIOD != dwLEFT/TNOTIFY_PERIOD )
		{
			CString strTYPE;
			CString strMSG;

			strMSG = CTChart::Format( TSTR_NOTIFY_CHANGE_CHANNEL, m_dwLeftTickCH/TNOTIFY_PERIOD);
			strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
			m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, 
				TCOLOR_INFO, TCHAT_FLAG_INFO, ID_SND_INFO, MEDIA_TSOUND);
		}

		m_dwLeftTickCH = dwLEFT;
	}
}

void CTClientGame::CalcSelectCharacter(DWORD dwTick)
{
	if( !m_bDoSelectCHAR )
		return;

	if( m_dwLeftTickCHAR <= dwTick )
	{
		m_pMainWnd->CloseMessageBox(TRUE);

		m_bDoSelectCHAR = FALSE;
		m_pSESSION->SendCS_COUNTDOWN_REQ(GM_EXIT_GAME);
	}
	else
	{
		DWORD dwLEFT = m_dwLeftTickCHAR - dwTick;
		if( m_dwLeftTickCHAR/TNOTIFY_PERIOD != dwLEFT/TNOTIFY_PERIOD )
		{
			CString strTYPE;
			CString strMSG;

			strMSG = CTChart::Format( TSTR_NOTIFY_SELECT_CHARACTER, m_dwLeftTickCHAR/TNOTIFY_PERIOD);
			strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
			m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, 
				TCOLOR_INFO, TCHAT_FLAG_INFO, ID_SND_INFO, MEDIA_TSOUND);
		}

		m_dwLeftTickCHAR = dwLEFT;
	}
}

void CTClientGame::CalcExitGame(DWORD dwTick)
{
	if( !m_bDoEXIT )
		return;

	if( m_dwLeftTickEXIT <= dwTick )
	{
		m_pMainWnd->CloseMessageBox(TRUE);

		m_bDoEXIT = FALSE;
		m_pSESSION->SendCS_COUNTDOWN_REQ(GM_EXIT);
	}
	else
	{
		DWORD dwLEFT = m_dwLeftTickEXIT - dwTick;
		if( m_dwLeftTickEXIT/TNOTIFY_PERIOD != dwLEFT/TNOTIFY_PERIOD )
		{
			CString strTYPE;
			CString strMSG;

			strMSG = CTChart::Format( TSTR_NOTIFY_SELECT_CHARACTER, m_dwLeftTickEXIT/TNOTIFY_PERIOD);
			strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
			m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, 
				TCOLOR_INFO, TCHAT_FLAG_INFO, ID_SND_INFO, MEDIA_TSOUND);
		}

		m_dwLeftTickEXIT = dwLEFT;
	}
}

void CTClientGame::CalcKickoutMap( DWORD dwTick )
{
	if( !m_bDoKICKOUTMAP )
		return;

	if( m_vMAP.m_wMapID != m_wKICKOUTMAP )
	{
		m_wKICKOUTMAP = 0;
		m_bDoKICKOUTMAP = FALSE;
		m_dwLeftTickKICKOUTMAP = 0;
		return ;
	}

	if( m_dwLeftTickKICKOUTMAP <= dwTick )
	{
		m_pMainWnd->CloseMessageBox(TRUE);

		m_wKICKOUTMAP = 0;
		m_bDoKICKOUTMAP = FALSE;
		m_dwLeftTickKICKOUTMAP = 0;

		m_pSESSION->SendCS_KICKOUTMAP_REQ();
	}
	else
	{
		DWORD dwLEFT = m_dwLeftTickKICKOUTMAP - dwTick;
		if( m_dwLeftTickKICKOUTMAP < TKICKOUTMAP_DELAY &&
			m_dwLeftTickKICKOUTMAP/TNOTIFY_PERIOD != dwLEFT/TNOTIFY_PERIOD )
		{
			CString strTYPE;
			CString strMSG;

			strMSG = CTChart::Format( TSTR_NOTIFY_KICKOUTMAP, m_dwLeftTickKICKOUTMAP/TNOTIFY_PERIOD);
			strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
			m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, 
				TCOLOR_INFO, TCHAT_FLAG_INFO, ID_SND_INFO, MEDIA_TSOUND);
		}

		m_dwLeftTickKICKOUTMAP = dwLEFT;
	}
}

void CTClientGame::DoChangeChannel(BYTE bChannel)
{
	m_dwLeftTickCH = TCHANGE_CHANNEL_DELAY;
	m_bDoChangeCH = TRUE;
	m_bChannelID = bChannel;
	
	StopMoveMainChar();

	m_pMainWnd->MessageBoxOK(
		TSTR_MBOX_CANCELCH,
		TSTR_CANCEL,
		GM_CLOSE_CANCELQUIT,
		0 );

	m_pMainChar->CancelMaintainByFunction( this, CTClientGame::m_dwReleaseMaintainFunctionWhenGameOut, sizeof(CTClientGame::m_dwReleaseMaintainFunctionWhenGameOut)/sizeof(DWORD) );
}

void CTClientGame::DoSelectCharacter()
{
	m_dwLeftTickCHAR = TSELECT_CHARACTER_DELAY;
	m_bDoSelectCHAR = TRUE;

	StopMoveMainChar();

	m_pMainWnd->MessageBoxOK(
		TSTR_MBOX_CANCELQUIT,
		TSTR_CANCEL,
		GM_CLOSE_CANCELQUIT,
		0 );

	m_pMainChar->CancelMaintainByFunction( this, CTClientGame::m_dwReleaseMaintainFunctionWhenGameOut, sizeof(CTClientGame::m_dwReleaseMaintainFunctionWhenGameOut)/sizeof(DWORD) );
}

int CTClientGame::OnCommand( TCOMMAND Command)
{
	int nERROR = FALSE;

	m_dwCmdParam = Command.m_dwParam;

	switch(Command.m_dwID)
	{
	ON_GM_COMMAND(GM_SYSTEM_SETTING)
	ON_GM_COMMAND(GM_CONTROL_SETTING)
	ON_GM_COMMAND(GM_GAME_SETTING)
	ON_GM_COMMAND(GM_OPEN_WORLDMAP)
	ON_GM_COMMAND(GM_MAIN_MENU)
	ON_GM_COMMAND(GM_SYSTEM)
	ON_GM_COMMAND(GM_QUIT)
	ON_GM_COMMAND(GM_GOBACK_CHARSEL)
	ON_GM_COMMAND(GM_BEGIN_EXIT)

	ON_GM_COMMAND(GM_BASIC_INFO)
	ON_GM_COMMAND(GM_MORE_INFO)

	ON_GM_COMMAND(GM_BLOCK_REQ)
	ON_GM_COMMAND(GM_UNBLOCK_REQ)

	ON_GM_COMMAND(GM_CLOSE_POPUP)
	ON_GM_COMMAND(GM_CLOSE_UI)

	ON_GM_COMMAND(GM_PARTYMEMBERRECALL)
	ON_GM_COMMAND(GM_USEITEM_PARTYMEMBERRECALL)
	ON_GM_COMMAND(GM_USEITEM_MOVETOCHAR)
	ON_GM_COMMAND(GM_PARTYMEMBERRECALLANS_OK)
	ON_GM_COMMAND(GM_PARTYMEMBERRECALLANS_NO)
	ON_GM_COMMAND(GM_PARTY_SETCHIEF)
	ON_GM_COMMAND(GM_FARPARTY_SETCHIEF)
	ON_GM_COMMAND(GM_PARTY_EXIT)
	ON_GM_COMMAND(GM_PARTY_KICK)
	ON_GM_COMMAND(GM_PARTY_REQ)
	ON_GM_COMMAND(GM_PARTY_OK)
	ON_GM_COMMAND(GM_PARTY_NO)
	ON_GM_COMMAND(GM_INVITE_FRIEND_TO_PARTY)
	ON_GM_COMMAND(GM_CHANGE_PARTY_ITEM_TYPE)		
	
	ON_GM_COMMAND(GM_CORPS_SETCOMMANDER)
	ON_GM_COMMAND(GM_CORPS_LEAVE)
	ON_GM_COMMAND(GM_CORPS_REQ)
	ON_GM_COMMAND(GM_CORPS_OK)
	ON_GM_COMMAND(GM_CORPS_NO)

	ON_GM_COMMAND(GM_REVIVAL_YES)
	ON_GM_COMMAND(GM_REVIVAL_NO)
	ON_GM_COMMAND(GM_REVIVAL)

	ON_GM_COMMAND(GM_SET_RETURNPOS)
	ON_GM_COMMAND(GM_GOTO_THUMB)
	ON_GM_COMMAND(GM_USEITE_REVIVAL)
	ON_GM_COMMAND(GM_ATTACK)

	ON_GM_COMMAND(GM_OPEN_SKILLINFO)
	ON_GM_COMMAND(GM_OPEN_PARTYINFO)
	ON_GM_COMMAND(GM_OPEN_GUILDINFO)
	ON_GM_COMMAND(GM_OPEN_CHARINFO)
	ON_GM_COMMAND(GM_OPEN_MONINVEN)
	ON_GM_COMMAND(GM_JOIN_ITEM_LOTTERY)
	ON_GM_COMMAND(GM_GIVEUP_ITEM_LOTTERY)
	
	ON_GM_COMMAND(GM_ACTIVATE_SWITCH)
	ON_GM_COMMAND(GM_NPC_REVIVAL)
	ON_GM_COMMAND(GM_NPC_TRADE)
	ON_GM_COMMAND(GM_NPC_TALK)
	ON_GM_COMMAND(GM_CRAFT_OK)
	ON_GM_COMMAND(GM_CRAFT)

	ON_GM_COMMAND(GM_TRADE_LOCK)
	ON_GM_COMMAND(GM_TRADE_YES)
	ON_GM_COMMAND(GM_TRADE_NO)
	ON_GM_COMMAND(GM_TRADE_OK)
	ON_GM_COMMAND(GM_TRADE_REQ)

	ON_GM_COMMAND(GM_SKILLLEVEL_1)
	ON_GM_COMMAND(GM_SKILLLEVEL_2)
	ON_GM_COMMAND(GM_SKILLLEVEL_3)
	ON_GM_COMMAND(GM_SKILLLEVEL_4)
	ON_GM_COMMAND(GM_SKILLLEVEL_5)
	ON_GM_COMMAND(GM_SKILLLEVEL_6)
	ON_GM_COMMAND(GM_SKILLLEVEL_7)
	ON_GM_COMMAND(GM_SKILLLEVEL_8)
	ON_GM_COMMAND(GM_SKILLLEVEL_9)
	ON_GM_COMMAND(GM_SKILLLEVEL_10)
	ON_GM_COMMAND(GM_SKILLLEVEL_11)
	ON_GM_COMMAND(GM_SKILLLEVEL_12)
	ON_GM_COMMAND(GM_SKILLLEVEL_13)
	ON_GM_COMMAND(GM_SKILLLEVEL_14)
	ON_GM_COMMAND(GM_SKILLLEVEL_15)
	ON_GM_COMMAND(GM_SKILLLEVEL_16)
	ON_GM_COMMAND(GM_SKILLLEVEL_17)
	ON_GM_COMMAND(GM_SKILLLEVEL_18)

	ON_GM_COMMAND(GM_WARN_GRADE_ITEM_DROP)
	ON_GM_COMMAND(GM_DROP_ITEM_YES)
	ON_GM_COMMAND(GM_DROP_ITEM_NO)
	ON_GM_COMMAND(GM_DIVIDE_ITEM)
	ON_GM_COMMAND(GM_ITEM_TAKE)
	ON_GM_COMMAND(GM_ITEM_SELL)
	ON_GM_COMMAND(GM_CONFIRM_ITEM_SELL)
	ON_GM_COMMAND(GM_ITEM_BUY)
	ON_GM_COMMAND(GM_MONSTER_BUY)
	ON_GM_COMMAND(GM_PORTAL_REQ)

	ON_GM_COMMAND(GM_WARN_GRADE_ITEM_SELL)
	ON_GM_COMMAND(GM_WARN_UPITEM_SELL_OK)
	ON_GM_COMMAND(GM_WARN_UPITEM_SELL_CANCEL)
	ON_GM_COMMAND(GM_WARN_UPITEM_SELL_DIFFERENT_ID)

	ON_GM_COMMAND(GM_GETMONEY)
	ON_GM_COMMAND(GM_GETALL)

	ON_GM_COMMAND(GM_ITEM_DO_SELL)
	ON_GM_COMMAND(GM_ITEM_DO_BUY)

	ON_GM_COMMAND(GM_MONSTER_DO_BUY)

	ON_GM_COMMAND(GM_HOTKEY_DOWN)
	ON_GM_COMMAND(GM_HOTKEY_UP)

	ON_GM_COMMAND(GM_TOGGLE_INVEN_SLOT)
	ON_GM_COMMAND(GM_TOGGLE_MAIN)

	ON_GM_COMMAND(GM_TOGGLE_SKILLINFO)
	ON_GM_COMMAND(GM_TOGGLE_QUESTINFO)
	ON_GM_COMMAND(GM_TOGGLE_CHARINFO)
	ON_GM_COMMAND(GM_TOGGLE_COMMUNITY)
	ON_GM_COMMAND(GM_TOGGLE_DEFINVEN)
	ON_GM_COMMAND(GM_TOGGLE_MINIMAP)
	ON_GM_COMMAND(GM_TOGGLE_SIEGE)
	ON_GM_COMMAND(GM_TOGGLE_CMDMAP)
	ON_GM_COMMAND(GM_TOGGLE_HIDE_SQUAD)

	ON_GM_COMMAND(GM_QUEST_ACCEPT)
	ON_GM_COMMAND(GM_QUEST_GIVEUP)
	ON_GM_COMMAND(GM_EXPAND_QUEST)
	ON_GM_COMMAND(GM_EXPAND_NPCQUEST)
	ON_GM_COMMAND(GM_QUEST_CRAFT)

	ON_GM_COMMAND(GM_MINIMAP_ZOOMOUT)
	ON_GM_COMMAND(GM_MINIMAP_ZOOMIN)
	ON_GM_COMMAND(GM_SELECT_WORLDMAP)

	ON_GM_COMMAND(GM_RECALL_PASSIVE_AI)
	ON_GM_COMMAND(GM_RECALL_ACTIVE_AI)
	ON_GM_COMMAND(GM_RECALL_MANUAL_AI)
	ON_GM_COMMAND(GM_RECALL_STAY_AI)
	ON_GM_COMMAND(GM_RECALL_BACK_AI)
	ON_GM_COMMAND(GM_DEL_RECALL)

	ON_GM_COMMAND(GM_TOGGLE_CHAT_LOCK)
	ON_GM_COMMAND(GM_TOGGLE_CHAT_LANG)
	ON_GM_COMMAND(GM_TOGGLE_ACTLIST)
	ON_GM_COMMAND(GM_TOGGLE_MESSENGER)
	ON_GM_COMMAND(GM_TOGGLE_MAIL)

	ON_GM_COMMAND(GM_CLOSE_CHATINFO)

	ON_GM_COMMAND(GM_ITEMUP_READY)
	ON_GM_COMMAND(GM_ITEMUP_CANCEL)
	ON_GM_COMMAND(GM_ITEMUP_START)

	ON_GM_COMMAND(GM_WARN_NOTUSE_CASHITEM_CASH)
	ON_GM_COMMAND(GM_WARN_NOTUSE_CASHITEM_OK)
	ON_GM_COMMAND(GM_WARN_NOTUSE_CASHITEM_CANCEL)

	ON_GM_COMMAND(GM_GUILD_NORMAL)
	ON_GM_COMMAND(GM_GUILD_MEMBER)
	ON_GM_COMMAND(GM_GUILD_TERRITORY)
	ON_GM_COMMAND(GM_GUILD_MERCENARY)
	ON_GM_COMMAND(GM_GUILD_REQ)
	ON_GM_COMMAND(GM_TACTICS_REQ)
	ON_GM_COMMAND(GM_SHOW_INVITE_TACTICS)
	ON_GM_COMMAND(GM_GUILD_EXIT)
	ON_GM_COMMAND(GM_ESTABLISH_GUILD)
	ON_GM_COMMAND(GM_NPC_GUILD_APP)
	ON_GM_COMMAND(GM_NPC_GUILD_NEW)
	ON_GM_COMMAND(GM_NPC_GUILD_MARK)
	ON_GM_COMMAND(GM_NPC_GUILD_DEL)
	ON_GM_COMMAND(GM_NPC_GUILD_DEL_CANCEL)
	ON_GM_COMMAND(GM_GUILDINVITE_ACK_YES)
	ON_GM_COMMAND(GM_GUILDINVITE_ACK_NO)
	ON_GM_COMMAND(GM_GUILDTACTICSINVITE_ACK_YES)
	ON_GM_COMMAND(GM_GUILDTACTICSINVITE_ACK_NO)
	ON_GM_COMMAND(GM_GUILDTACTICSINVITE_ACK_BUSY)
	ON_GM_COMMAND(GM_GUILD_DONATE_EXP)
	ON_GM_COMMAND(GM_GUILD_DONATE_MONEY)
	ON_GM_COMMAND(GM_WRAN_DONATE_EXP)
	ON_GM_COMMAND(GM_DO_DONATE_EXP)
	ON_GM_COMMAND(GM_WRAN_DONATE_MONEY)
	ON_GM_COMMAND(GM_DO_DONATE_MONEY)
	ON_GM_COMMAND(GM_GUILD_NOTIFY)
	ON_GM_COMMAND(GM_GUILD_COMMAND)
	ON_GM_COMMAND(GM_REG_GUILDMARK)
	ON_GM_COMMAND(GM_GUILDKICKOUT)
	ON_GM_COMMAND(GM_GUILDTACTICSKICKOUT)
	ON_GM_COMMAND(GM_GUILDCAB_GETITEM)
	ON_GM_COMMAND(GM_GUILD_EXIT_WARN)

	ON_GM_COMMAND(GM_NEW_NOTIFY)
	ON_GM_COMMAND(GM_MOD_NOTIFY)
	ON_GM_COMMAND(GM_NEW_NOTIFY_OK)
	ON_GM_COMMAND(GM_NEW_NOTIFY_CANCEL)
	ON_GM_COMMAND(GM_MOD_NOTIFY_OK)
	ON_GM_COMMAND(GM_MOD_NOTIFY_CANCEL)
	ON_GM_COMMAND(GM_DELETE_NOTIFY)
    
	ON_GM_COMMAND(GM_GINV_APP)		
	ON_GM_COMMAND(GM_GINV_EDIT)	
	ON_GM_COMMAND(GM_GINV_NEW)	
	ON_GM_COMMAND(GM_GINV_DEL)		
	ON_GM_COMMAND(GM_GINV_MOD_OK)
	ON_GM_COMMAND(GM_GINV_NEW_OK)	
	ON_GM_COMMAND(GM_GINV_MOD_CANCEL)
	ON_GM_COMMAND(GM_GINV_NEW_CANCEL)

	ON_GM_COMMAND(GM_GSEEK_INV)
	ON_GM_COMMAND(GM_GSEEK_EDIT)
	ON_GM_COMMAND(GM_GSEEK_NEW)
	ON_GM_COMMAND(GM_GSEEK_DEL)
	ON_GM_COMMAND(GM_GSEEK_MOD_OK)
	ON_GM_COMMAND(GM_GSEEK_NEW_OK)
	ON_GM_COMMAND(GM_GSEEK_MOD_CANCEL)
	ON_GM_COMMAND(GM_GSEEK_NEW_CANCEL)

	ON_GM_COMMAND(GM_FORCE_NEW_GUILDOFFER)
	ON_GM_COMMAND(GM_FORCE_NEW_GUILDSEEK)

	ON_GM_COMMAND(GM_GMP_WHISPER)		
	ON_GM_COMMAND(GM_GMP_MAIL)			
	ON_GM_COMMAND(GM_GMP_PARTY)
	ON_GM_COMMAND(GM_GMP_FRIEND)
	ON_GM_COMMAND(GM_GMP_KICK)			
	ON_GM_COMMAND(GM_GMP_DISORG_TACTICS)
	ON_GM_COMMAND(GM_GMP_ET_GM)		
	ON_GM_COMMAND(GM_GMP_SET_GSM)		
	ON_GM_COMMAND(GM_GMP_CANCEL_GSM)	
	ON_GM_COMMAND(GM_GMP_PEER_BARON)	
	ON_GM_COMMAND(GM_GMP_PEER_VISCOUNT)
	ON_GM_COMMAND(GM_GMP_PEER_COUNT)	
	ON_GM_COMMAND(GM_GMP_PEER_MARQUIS)	
	ON_GM_COMMAND(GM_GMP_PEER_DUKE)	
	ON_GM_COMMAND(GM_GMP_CANCEL_PEER)	
	ON_GM_COMMAND(GM_GMP_POINT_REWARD)
	ON_GM_COMMAND(GM_GMP_CANCEL)
	ON_GM_COMMAND(GM_GMP_VOLUNTEER_HESED)
	ON_GM_COMMAND(GM_GMP_VOLUNTEER_ARDRI)
	ON_GM_COMMAND(GM_GMP_VOLUNTEER_TYCON)
	ON_GM_COMMAND(GM_GMP_VOLUNTEER_GEHBRA)
	ON_GM_COMMAND(GM_GMP_VOLUNTEER_CANCEL)

	ON_GM_COMMAND(GM_GVP_WHISPER)
	ON_GM_COMMAND(GM_GVP_MAIL)
	ON_GM_COMMAND(GM_GVP_INVITE_YES)
	ON_GM_COMMAND(GM_GVP_INVITE_NO)
	ON_GM_COMMAND(GM_GVP_CANCEL)	

	ON_GM_COMMAND(GM_OPEN_PRIVSHOP_FOR_BUY)
	ON_GM_COMMAND(GM_OPEN_PRIVSHOP_FOR_SELL)
	ON_GM_COMMAND(GM_PRIVATE_SHOP_START)
	ON_GM_COMMAND(GM_PRIVATE_SHOP_END)
	ON_GM_COMMAND(GM_PRIVATE_SHOP_BUY)
	ON_GM_COMMAND(GM_PRIVATE_SHOP_BUY_ITEMS)
	ON_GM_COMMAND(GM_REG_ITEM_PRVSHOP)

	ON_GM_COMMAND(GM_FRIEND_REQ)
	ON_GM_COMMAND(GM_FARFRIEND_REQ)
	ON_GM_COMMAND(GM_ADD_FRIEND)
	ON_GM_COMMAND(GM_FRIENDINVITE_ACK_YES)
	ON_GM_COMMAND(GM_FRIENDINVITE_ACK_NO)
	ON_GM_COMMAND(GM_ADDTOBLOCKLIST)

	ON_GM_COMMAND(GM_NEW_FRIEND_GROUP)		
	ON_GM_COMMAND(GM_CHANGE_FRIEND_GROUP)
	ON_GM_COMMAND(GM_SHOW_TARGETMAIL)	
	ON_GM_COMMAND(GM_SHOW_GROUPLIST_POPUP)	
	ON_GM_COMMAND(GM_ADDTOBLOCKLIST_TARGET)
	ON_GM_COMMAND(GM_DELETE_FRIENDITEM)
	ON_GM_COMMAND(GM_ASK_NEW_FRIEND_GROUP)
	ON_GM_COMMAND(GM_NEW_MESSENGER_GROUPCHAT)
	ON_GM_COMMAND(GM_ASK_CHANGE_FRIEND_GROUP)
	ON_GM_COMMAND(GM_MOVE_FRIEND_GROUP)

	ON_GM_COMMAND(GM_SEND_MAIL)
	ON_GM_COMMAND(GM_SHOW_MAILREPLY)
	ON_GM_COMMAND(GM_DELETE_MAIL)
	ON_GM_COMMAND(GM_MESSENGER_DELETE_MAIL)
	ON_GM_COMMAND(GM_MAILBILL_PAY)
	ON_GM_COMMAND(GM_MAILBILL_RETURN)
	ON_GM_COMMAND(GM_MAIL_GETITEM)
	ON_GM_COMMAND(GM_SHOW_RECVMAIL)
	ON_GM_COMMAND(GM_SHOW_NEWMAIL)
	ON_GM_COMMAND(GM_NEW_MESSENGER_CHAT)
	ON_GM_COMMAND(GM_TOGGLE_BLOCKLIST)
	ON_GM_COMMAND(GM_MESSENGERCHAT_CLOSE)

	ON_GM_COMMAND(GM_PET_NEWMAKE)
	ON_GM_COMMAND(GM_PET_NAMEBOX)
	ON_GM_COMMAND(GM_PET_RECALL)
	ON_GM_COMMAND(GM_MENU_RIDEPET)
	ON_GM_COMMAND(GM_MENU_PETMANAGE)

	ON_GM_COMMAND(GM_SELECT_TEXTURE_DETAIL)
	ON_GM_COMMAND(GM_CHANGE_CHANNEL)
	ON_GM_COMMAND(GM_CLOSE_CANCELQUIT)

	ON_GM_COMMAND(GM_CABINET_SELECT)
	ON_GM_COMMAND(GM_CABINET_OPEN_WARN)
	ON_GM_COMMAND(GM_CABINET_OPEN)

	ON_GM_COMMAND(GM_FOLLOW_TARGET)

	ON_GM_COMMAND(GM_KEYSET_DEFAULT)
	ON_GM_COMMAND(GM_KEYSET_APPLY)
	ON_GM_COMMAND(GM_KEYSET_CANCEL)

	ON_GM_COMMAND(GM_MENU_DUEL)
	ON_GM_COMMAND(GM_DUELINVITE_YES)
	ON_GM_COMMAND(GM_DUELINVITE_NO)

	ON_GM_COMMAND(GM_SMS_SEND)
	ON_GM_COMMAND(GM_SMS_SEND_OK)

	ON_GM_COMMAND(GM_WHISPER_REPLY)

	ON_GM_COMMAND(GM_SKILLINIT_ALL)
	ON_GM_COMMAND(GM_SKILLINIT_ONE)

	ON_GM_COMMAND(GM_TOGGLE_HELMETHIDE)

	ON_GM_COMMAND(GM_TOGGLE_SUBCASHMENU)

	ON_GM_COMMAND(GM_TOGGLE_CASH_SHOP)
	ON_GM_COMMAND(GM_TOGGLE_CASH_CAB)
	ON_GM_COMMAND(GM_TOGGLE_CASH_CHARGE)

	ON_GM_COMMAND(GM_SHOW_ACCOUNT)
	ON_GM_COMMAND(GM_CLOSE_CASHCHARGE)
	ON_GM_COMMAND(GM_CASHCAB_GETITEM)
	ON_GM_COMMAND(GM_CASHCAB_EXTENSIONPERIOD)
	ON_GM_COMMAND(GM_CASHCAB_BUYCABINET_YES)
	ON_GM_COMMAND(GM_WARN_BUY_CASHITEM)
	ON_GM_COMMAND(GM_BUY_CASHITEM)
	ON_GM_COMMAND(GM_WARN_CASH_PRESENT)
	ON_GM_COMMAND(GM_CHECK_CASH_PRESENT)
	ON_GM_COMMAND(GM_CASH_PRESENT)

	ON_GM_COMMAND(GM_END_MAINTAIN)

	ON_GM_COMMAND(GM_SOULMATE_REGBOX)
	ON_GM_COMMAND(GM_SOULMATE_SEARCH)
	ON_GM_COMMAND(GM_SOULMATE_REGREADY)
	ON_GM_COMMAND(GM_SOULMATE_REGOK)
	ON_GM_COMMAND(GM_SOULMATE_SEARCHOK)
	ON_GM_COMMAND(GM_SOULMATE_NO)
	ON_GM_COMMAND(GM_SOULMATE_END)
	ON_GM_COMMAND(GM_SOULMATE_SUCCESS)
	ON_GM_COMMAND(GM_SOULMATE_FAIL)
	ON_GM_COMMAND(GM_SOULMATE_NOTFOUND)

	ON_GM_COMMAND(GM_GAMBLE_CHECK)
	ON_GM_COMMAND(GM_GAMBLE_PROGRESS)
	ON_GM_COMMAND(GM_GAMBLE_OK)
	ON_GM_COMMAND(GM_GAMBLE_NO)

	ON_GM_COMMAND(GM_NPC_DURATIONREP_NOR)		
	ON_GM_COMMAND(GM_NPC_DURATIONREP_ALL)		
	ON_GM_COMMAND(GM_NPC_DURATIONREP_EQUIP)	
	ON_GM_COMMAND(GM_DURATIONREP_NOR)
	ON_GM_COMMAND(GM_DURATIONREP_PROGRESS)	
	ON_GM_COMMAND(GM_DURATIONREP_REQ)		
	ON_GM_COMMAND(GM_NPC_REFINE)		
	ON_GM_COMMAND(GM_REFINE_COST)		
	ON_GM_COMMAND(GM_REFINE_PROGRESS)	
	ON_GM_COMMAND(GM_REFINE)

	ON_GM_COMMAND(GM_CHANGE_NAME)
	ON_GM_COMMAND(GM_EXTEND_ITEM_PERIOD)
	ON_GM_COMMAND(GM_EXTEND_INVEN_PERIOD)
	ON_GM_COMMAND(GM_STOPTHECLOCK)
	ON_GM_COMMAND(GM_NPC_EXTEND_ITEM_PERIOD)
	ON_GM_COMMAND(GM_WARN_EXTEND_PERIOD)
	ON_GM_COMMAND(GM_EXTEND_PERIOD)

	ON_GM_COMMAND(GM_CASHCABINET_PUTIN)
	ON_GM_COMMAND(GM_CLOSE_MSGBOX_RESETINVEN)

	ON_GM_COMMAND(GM_INIT_UI)
	ON_GM_COMMAND(GM_TOGGLE_RESOLUTION)
	ON_GM_COMMAND(GM_TOGGLE_PETMANAGE)

	ON_GM_COMMAND(GM_HELP_NEXT)
	ON_GM_COMMAND(GM_HELP_PREV)
	ON_GM_COMMAND(GM_TOGGLE_HELP)

	ON_GM_COMMAND(GM_SET_WHISPER_TARGET)
	ON_GM_COMMAND(GM_TOGGLE_WHISPER_TARGET)
	ON_GM_COMMAND(GM_TOGGLE_CHAT_TYPE)

#ifdef MODIFY_CNP_NAME
	ON_GM_COMMAND(GM_COPY_NAME)
	ON_GM_COMMAND(GM_PASTE_NAME)
#endif

	ON_GM_COMMAND(GM_DAY_GUILDPVPRECORD)
	ON_GM_COMMAND(GM_WEEKEN_GUILDPVPRECORD)

	ON_GM_COMMAND(GM_GUILDREWARD)
	ON_GM_COMMAND(GM_GUILD_PVPRECORD)
	ON_GM_COMMAND(GM_POINTREWARD)

	ON_GM_COMMAND(GM_PVP_INFO)
	ON_GM_COMMAND(GM_WAR_INFO)

	ON_GM_COMMAND(GM_GUILD_LIST_TAB1)
	ON_GM_COMMAND(GM_GUILD_LIST_TAB2)
	ON_GM_COMMAND(GM_GUILD_LIST_TAB3)
	ON_GM_COMMAND(GM_GUILD_LIST_TAB4)
	ON_GM_COMMAND(GM_GUILD_REWARD_STOP)
	ON_GM_COMMAND(GM_GUILD_REWARD_CONTINUE)
	ON_GM_COMMAND(GM_GUILD_MERCENARY_TAB1)
	ON_GM_COMMAND(GM_GUILD_MERCENARY_TAB2)

	ON_GM_COMMAND(GM_MOUNT_BALL2_TOWER)
	ON_GM_COMMAND(GM_PICKUP_BALL)

	ON_GM_COMMAND(GM_ITEM_UP_PROGRESS_CANCEL)

	ON_GM_COMMAND( GM_CHANGE_REFINE )
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_WRITE )
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_MODIFY )
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_DELETE )
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_DO_DELETE )
	
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_GUILDMEMBER )
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_TACTICS ) 

	ON_GM_COMMAND( GM_CORPS_BUSY )
	ON_GM_COMMAND( GM_PARTY_BUSY )
	ON_GM_COMMAND( GM_TRADE_BUSY )
	ON_GM_COMMAND( GM_DUELINVITE_BUSY )
	ON_GM_COMMAND( GM_GUILDINVITE_ACK_BUSY )

	ON_GM_COMMAND(GM_AUCTION_SEARCH_TAB)
	ON_GM_COMMAND(GM_AUCTION_REGIST_TAB)
	ON_GM_COMMAND(GM_AUCTION_BIDDING_TAB)
	ON_GM_COMMAND(GM_AUCTION_BASKET_TAB)

	ON_GM_COMMAND(GM_AUCTION_MOVEPAGE_LEFT)
	ON_GM_COMMAND(GM_AUCTION_MOVEPAGE_RIGHT)
	ON_GM_COMMAND(GM_AUCTION_MOVEPAGE_LEFTEND)
	ON_GM_COMMAND(GM_AUCTION_MOVEPAGE_RIGHTEND)

	ON_GM_COMMAND(GM_AUCTION_BID)
	ON_GM_COMMAND(GM_AUCTION_BUY)
	ON_GM_COMMAND(GM_AUCTION_DO_BUY)
	ON_GM_COMMAND(GM_AUCTION_CANCEL_REG)
	ON_GM_COMMAND(GM_AUCTION_DO_CANCEL_REG)
	ON_GM_COMMAND(GM_AUCTION_INSERT_BASKET)
	ON_GM_COMMAND(GM_AUCTION_ERASE_BASKET)
	
	ON_GM_COMMAND(GM_AUCTION_MOVE_BASKET_TAB)
	ON_GM_COMMAND(GM_AUCTION_MAKEABIDFORITEM)
	ON_GM_COMMAND(GM_AUCTION_REGIST_MANYITEMS)

	ON_GM_COMMAND(GM_AUCTION_SEARCH)
	ON_GM_COMMAND(GM_AUCTION_SEARCH_RESET)
	ON_GM_COMMAND(GM_AUCTION_REGIST)
	ON_GM_COMMAND(GM_AUCTION_DO_REGIST)
	ON_GM_COMMAND(GM_AUCTION_REGIST_RESET)

	ON_GM_COMMAND(GM_AUCTION_ALIGN_NAME)
	ON_GM_COMMAND(GM_AUCTION_ALIGN_CLASS)
	ON_GM_COMMAND(GM_AUCTION_ALIGN_LEVEL)
	ON_GM_COMMAND(GM_AUCTION_ALIGN_PRICE)
	ON_GM_COMMAND(GM_AUCTION_ALIGN_SELLER)
	ON_GM_COMMAND(GM_AUCTION_ALIGN_LEFTTIME)
	ON_GM_COMMAND(GM_AUCTION_ALIGN_BIDCOUNT)

	ON_GM_COMMAND( GM_GVFPM_MAIL )
	ON_GM_COMMAND( GM_GVFPM_REGULAR_ACCEPT )
	ON_GM_COMMAND( GM_GVFPM_REGULAR_REJECT )
	ON_GM_COMMAND( GM_GVFPM_TACTICS_ACCEPT )
	ON_GM_COMMAND( GM_GVFPM_TACTICS_REJECT )	
	ON_GM_COMMAND( GM_GVFPM_CLOSE )	

	ON_GM_COMMAND( GM_GUILD_INVITATION_REGULAR_OK )
	ON_GM_COMMAND( GM_GUILD_INVITATION_TACTICS_OK )

	ON_GM_COMMAND( GM_GMP_TACTICS_RE_CONTRACT )	
	ON_GM_COMMAND( GM_GMP_TACTICS_KICK )	
	ON_GM_COMMAND( GM_GMP_TACTICS_REWARD_POINT )	

	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_JOIN )
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_JOINCANCEL )
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_REFRESH )
	ON_GM_COMMAND( GM_GUILD_SHOW_MARKDLG )
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_JOIN_DO )
	ON_GM_COMMAND( GM_GUILD_OFFER_BBS_JOINCANCEL_DO )


#ifdef ADD_TEXTOOL
	ON_GM_COMMAND( GM_TEXTOOL_BACK )
	ON_GM_COMMAND( GM_TEXTOOL_BACK2 )
	ON_GM_COMMAND( GM_TEXTOOL_CENTERBOTTOM )
	ON_GM_COMMAND( GM_TEXTOOL_CENTERCENTER )
	ON_GM_COMMAND( GM_TEXTOOL_CENTERTOP )
	ON_GM_COMMAND( GM_TEXTOOL_EXT1 )
	ON_GM_COMMAND( GM_TEXTOOL_DOWN )
	ON_GM_COMMAND( GM_TEXTOOL_EXT2 )
	ON_GM_COMMAND( GM_TEXTOOL_LEFT )
	ON_GM_COMMAND( GM_TEXTOOL_LEFTBOTTOM )
	ON_GM_COMMAND( GM_TEXTOOL_LEFTCENTER )
	ON_GM_COMMAND( GM_TEXTOOL_LEFTTOP )
	ON_GM_COMMAND( GM_TEXTOOL_MARK )
	ON_GM_COMMAND( GM_TEXTOOL_EXT10 )
	ON_GM_COMMAND( GM_TEXTOOL_EXT3 )
	ON_GM_COMMAND( GM_TEXTOOL_RIGHT )
	ON_GM_COMMAND( GM_TEXTOOL_RIGHTBOTTOM )
	ON_GM_COMMAND( GM_TEXTOOL_RIGHTCENTER )
	ON_GM_COMMAND( GM_TEXTOOL_RIGHTTOP )
	ON_GM_COMMAND( GM_TEXTOOL_EXT9 )
	ON_GM_COMMAND( GM_TEXTOOL_UP )
	ON_GM_COMMAND( GM_TEXTOOL_EXT4 )
	ON_GM_COMMAND( GM_TEXTOOL_EXT5 )
	ON_GM_COMMAND( GM_TEXTOOL_EXT6 )
	ON_GM_COMMAND( GM_TEXTOOL_EXT8 )
	ON_GM_COMMAND( GM_TEXTOOL_EXT7 )
//	ON_GM_COMMAND( GM_TEXTOOL_SCALEDOWN )
//	ON_GM_COMMAND( GM_TEXTOOL_SCALEUP )
//	ON_GM_COMMAND( GM_TEXTOOL_COMPO )
#endif
	ON_GM_COMMAND( GM_CLOSE_MSGBOX_WITH_RESETCONMODE )
	ON_GM_COMMAND( GM_NPC_COLOR_MANTLE )
	ON_GM_COMMAND( GM_NPC_MAKE_GUILDMARK )
	ON_GM_COMMAND( GM_NPC_REG_GUILDMARK )
	ON_GM_COMMAND( GM_NPC_UNREG_GUILDMARK )

	//ON_GM_COMMAND( GM_ITEMCOLOR_READY )
	//ON_GM_COMMAND( GM_ITEMCOLOR_SELECT )
	//ON_GM_COMMAND( GM_ITEMCOLOR_DO )
	ON_GM_COMMAND( GM_REGGM_CHECK )
	ON_GM_COMMAND( GM_UNREGGM_CHECK )
	ON_GM_COMMAND( GM_APPLY_GUILDMARK )
	ON_GM_COMMAND( GM_GUILDMARK_CANCEL )

	ON_GM_COMMAND( GM_FAMERANK_MONTH_LEFT )
	ON_GM_COMMAND( GM_FAMERANK_MONTH_RIGHT )
	ON_GM_COMMAND( GM_TOGGLE_FAMERANK )
	ON_GM_COMMAND( GM_HERO_FAME )
	ON_GM_COMMAND( GM_KNIGHT_FAME )
	ON_GM_COMMAND( GM_GOD_FAME )
	ON_GM_COMMAND( GM_MONTH_FAME )
	ON_GM_COMMAND( GM_FIRSTGRADEGROUP_FAME )
	ON_GM_COMMAND( GM_WARLORD_SAY_CHANGE )
	ON_GM_COMMAND( GM_HERO_SAY_CHANGE )
	ON_GM_COMMAND( GM_KNIGHT_SAY_CHANGE )
	ON_GM_COMMAND( GM_GOD_SAY_CHANGE )
	//ON_GM_COMMAND( GM_FAMERANK_LIST_BTN1 )
	//ON_GM_COMMAND( GM_FAMERANK_LIST_BTN2 )
	ON_GM_COMMAND( GM_RANK_INFO )
	ON_GM_COMMAND( GM_RANK_INFO_TOTAL_TAB )
	ON_GM_COMMAND( GM_RANK_INFO_COUNTRY_TAB )

#ifdef ADD_CAUTION15
	ON_GM_COMMAND( GM_CAUTION15_CLICKED )
#endif	

	ON_GM_COMMAND( GM_CLOSE_RECV_MAIL )
	ON_GM_COMMAND(GM_TOURNAMENT_ADD_PARTY)
	ON_GM_COMMAND(GM_TOURNAMENT_ENTRY)
	ON_GM_COMMAND(GM_TOURNAMENT_EVENT_SELECT_PLAYER)
	ON_GM_COMMAND(GM_TOURNAMENT_GIVEUP	)
	ON_GM_COMMAND(GM_TOURNAMENT_GOTO_MATCH)
	ON_GM_COMMAND(GM_TOURNAMENT_REMOVE_PARTY)
	ON_GM_COMMAND(GM_TOURNAMENT_SELECT_TARGET)
	ON_GM_COMMAND(GM_TOURNAMENT_SELECT_WINNER)
	ON_GM_COMMAND(GM_TOURNAMENT_SHOW_MATCH)
	ON_GM_COMMAND(GM_TOURNAMENT_SHOW_EVENT)
	ON_GM_COMMAND(GM_TOURNAMENT_ENTRY_DO)
	ON_GM_COMMAND(GM_TOURNAMENT_GIVEUP_DO)
	ON_GM_COMMAND(GM_TOURNAMENT_EVENT_SELECT_PLAYER_DO)
	ON_GM_COMMAND(GM_TOURNAMENT_EVENT_PREV)
	ON_GM_COMMAND(GM_TOURNAMENT_REMOVE_PARTY_DO)
	
	ON_GM_COMMAND(GM_TOURNAMENT_OK)
	ON_GM_COMMAND(GM_TOURNAMENT_CANCEL)
	ON_GM_COMMAND(GM_TOURNAMENT_SCHEDULE_LEFT)
	ON_GM_COMMAND(GM_TOURNAMENT_SCHEDULE_RIGHT)
	ON_GM_COMMAND(GM_TOURNAMENT_TOGGLE_SHOW_PLAYER_INFO)
	
	ON_GM_COMMAND(GM_TOURNAMENT_SELECT_TARGET_GUILD)
	ON_GM_COMMAND(GM_TOURNAMENT_SELECT_TARGET_FRIEND)
	ON_GM_COMMAND(GM_TOURNAMENT_USE_CHEERITEM)

	ON_GM_COMMAND(GM_CLICKED_SIEGE_ALARM)
	ON_GM_COMMAND(GM_CLICKED_SIEGE_ALARM_DO)
	ON_GM_COMMAND(GM_CLICKED_STOP_TUTORIAL)

	ON_GM_COMMAND(GM_FIRSTRANK_MONTH_LEFT)
	ON_GM_COMMAND(GM_FIRSTRANK_MONTH_RIGHT)

	ON_GM_COMMAND(GM_SHOW_TOURNAMENT)
	ON_GM_COMMAND(GM_SHOW_TOURNAMENT_EVENT)
	
	ON_GM_COMMAND(GM_HELP_LEFT)
	ON_GM_COMMAND(GM_HELP_RIGHT)
	ON_GM_COMMAND(GM_HELP_UP)
	ON_GM_COMMAND(GM_HELP_DOWN)
	ON_GM_COMMAND(GM_SKIP_SCENE)
#ifdef ADD_RPS
	ON_GM_COMMAND(GM_RPS_GIVEUP)
	ON_GM_COMMAND(GM_RPS_SELECT_1)
	ON_GM_COMMAND(GM_RPS_SELECT_2)
	ON_GM_COMMAND(GM_RPS_SELECT_3)
	ON_GM_COMMAND(GM_RPS_TRY)
	ON_GM_COMMAND(GM_RPS_START)
	ON_GM_COMMAND(GM_RPS_END)
	ON_GM_COMMAND(GM_RPS_1)
	ON_GM_COMMAND(GM_RPS_10)
	ON_GM_COMMAND(GM_RPS_50)
#ifdef ADD_RPS_RL
	ON_GM_COMMAND(GM_RPS_SHOW_REWARD)
#endif
#endif
	ON_GM_COMMAND(GM_RANK_INFO_TOURNAMENT_TAB)
	
	default	: break;
	}

	return nERROR;
}

int CTClientGame::OnSessionMsg( CTachyonSession *pSession, CPacket *pPacket)
{
	DWORD dwID = pPacket->GetID();
	int nERROR = FALSE;

	switch(dwID)
	{
	// Relay message
	ON_TRECEIVE(CS_RELAYCONNECT_ACK)
	ON_TRECEIVE(CS_RELAYCHARDATA_ACK)

	// System message
	ON_TRECEIVE(CS_INVALIDCHAR_ACK)
	ON_TRECEIVE(CS_ADDCONNECT_ACK)
	ON_TRECEIVE(CS_SHUTDOWN_ACK)
	ON_TRECEIVE(CS_CONNECT_ACK)

	// PC information message
	ON_TRECEIVE(CS_CHARSTATINFO_ACK)
	ON_TRECEIVE(CS_CHARINFO_ACK)

	// AI message
	ON_TRECEIVE(CS_MONSETSKILL_ACK)
	ON_TRECEIVE(CS_MONACTION_ACK)
	ON_TRECEIVE(CS_MONHOST_ACK)

	// Switch/Gate message
	ON_TRECEIVE(CS_SWITCHCHANGE_ACK)
	ON_TRECEIVE(CS_SWITCHADD_ACK)
	ON_TRECEIVE(CS_SWITCHDEL_ACK)

	ON_TRECEIVE(CS_GATECHANGE_ACK)
	ON_TRECEIVE(CS_GATEADD_ACK)
	ON_TRECEIVE(CS_GATEDEL_ACK)

	// Navigation message
	ON_TRECEIVE(CS_BEGINTELEPORT_ACK)
	ON_TRECEIVE(CS_SETRETURNPOS_ACK)
	ON_TRECEIVE(CS_TELEPORT_ACK)
	ON_TRECEIVE(CS_ENTER_ACK)
	ON_TRECEIVE(CS_LEAVE_ACK)
	ON_TRECEIVE(CS_BLOCK_ACK)
	ON_TRECEIVE(CS_MOVE_ACK)
	ON_TRECEIVE(CS_JUMP_ACK)
	ON_TRECEIVE(CS_WARP_ACK)

	ON_TRECEIVE(CS_MONMOVE_ACK)
	ON_TRECEIVE(CS_ADDMON_ACK)
	ON_TRECEIVE(CS_DELMON_ACK)

	// Battle message
	ON_TRECEIVE(CS_REVIVALREPLY_ACK)
	ON_TRECEIVE(CS_CHANGECOLOR_ACK)
	ON_TRECEIVE(CS_REVIVALASK_ACK)
	ON_TRECEIVE(CS_DROPDAMAGE_ACK)
	ON_TRECEIVE(CS_MONATTACK_ACK)
	ON_TRECEIVE(CS_LOOPSKILL_ACK)
	ON_TRECEIVE(CS_SKILLUSE_ACK)
	ON_TRECEIVE(CS_SKILLEND_ACK)
	ON_TRECEIVE(CS_CHGMODE_ACK)
	ON_TRECEIVE(CS_REVIVAL_ACK)
	ON_TRECEIVE(CS_ACTION_ACK)
	ON_TRECEIVE(CS_DEFEND_ACK)
	ON_TRECEIVE(CS_LEVEL_ACK)
	ON_TRECEIVE(CS_HPMP_ACK)
	ON_TRECEIVE(CS_DIE_ACK)
	ON_TRECEIVE(CS_EXP_ACK)
	ON_TRECEIVE(CS_RESETCOOLTIME_ACK)

	// Item message
	ON_TRECEIVE(CS_UPDATEITEM_ACK)
	ON_TRECEIVE(CS_MOVEITEM_ACK)
	ON_TRECEIVE(CS_ADDITEM_ACK)
	ON_TRECEIVE(CS_DELITEM_ACK)
	ON_TRECEIVE(CS_ITEMUSE_ACK)
	ON_TRECEIVE(CS_MONEY_ACK)
	ON_TRECEIVE(CS_EQUIP_ACK)
	ON_TRECEIVE(CS_ITEMUPGRADE_ACK)
	ON_TRECEIVE(CS_ITEMMAGICGRADE_ACK)
	ON_TRECEIVE(CS_GETITEM_ACK)
	ON_TRECEIVE(CS_HELMETHIDE_ACK)

	// Inven message
	ON_TRECEIVE(CS_INVENMOVE_ACK)
	ON_TRECEIVE(CS_INVENADD_ACK)
	ON_TRECEIVE(CS_INVENDEL_ACK)

	// Corps message
	ON_TRECEIVE(CS_CHGCORPSCOMMANDER_ACK)
	ON_TRECEIVE(CS_CORPSREPLY_ACK)
	ON_TRECEIVE(CS_CORPSJOIN_ACK)
	ON_TRECEIVE(CS_CORPSASK_ACK)
	ON_TRECEIVE(CS_CORPSCMD_ACK)
	ON_TRECEIVE(CS_CORPSHP_ACK)
	ON_TRECEIVE(CS_REPORTENEMYLIST_ACK)
	ON_TRECEIVE(CS_CORPSENEMYLIST_ACK)
	ON_TRECEIVE(CS_MOVECORPSENEMY_ACK)
	ON_TRECEIVE(CS_MOVECORPSUNIT_ACK)
	ON_TRECEIVE(CS_ADDCORPSENEMY_ACK)
	ON_TRECEIVE(CS_DELCORPSENEMY_ACK)
	ON_TRECEIVE(CS_CHGSQUADCHIEF_ACK)
	ON_TRECEIVE(CS_ADDCORPSUNIT_ACK)
	ON_TRECEIVE(CS_DELCORPSUNIT_ACK)
	ON_TRECEIVE(CS_ADDSQUAD_ACK)
	ON_TRECEIVE(CS_DELSQUAD_ACK)

	// Party message
	ON_TRECEIVE(CS_CHGPARTYCHIEF_ACK)
	ON_TRECEIVE(CS_CHGPARTYTYPE_ACK)
	ON_TRECEIVE(CS_PARTYJOINASK_ACK)
	ON_TRECEIVE(CS_PARTYJOIN_ACK)
	ON_TRECEIVE(CS_PARTYATTR_ACK)
	ON_TRECEIVE(CS_PARTYMOVE_ACK)
	ON_TRECEIVE(CS_PARTYADD_ACK)
	ON_TRECEIVE(CS_PARTYDEL_ACK)
	ON_TRECEIVE(CS_PARTYMANSTAT_ACK)
	ON_TRECEIVE(CS_PARTYITEMTAKE_ACK)

	// Chat message
	ON_TRECEIVE(CS_CHAT_ACK)

	// Trade message
	ON_TRECEIVE(CS_NPCITEMLIST_ACK)
	ON_TRECEIVE(CS_SKILLBUY_ACK)
	ON_TRECEIVE(CS_ITEMSELL_ACK)
	ON_TRECEIVE(CS_ITEMBUY_ACK)

	ON_TRECEIVE(CS_DEALITEMSTART_ACK)
	ON_TRECEIVE(CS_DEALITEMASK_ACK)
	ON_TRECEIVE(CS_DEALITEMADD_ACK)
	ON_TRECEIVE(CS_DEALITEMEND_ACK)

	// Rooting message
	ON_TRECEIVE(CS_MONITEMLIST_ACK)
	ON_TRECEIVE(CS_MONITEMTAKE_ACK)
	ON_TRECEIVE(CS_MONITEMLOTTERY_ACK);

	// UI setting message
	ON_TRECEIVE(CS_HOTKEYCHANGE_ACK)
	ON_TRECEIVE(CS_HELPMESSAGE_ACK)

	// Summon message
	ON_TRECEIVE(CS_ADDRECALLMON_ACK)
	ON_TRECEIVE(CS_DELRECALLMON_ACK)
	ON_TRECEIVE(CS_ADDSELFOBJ_ACK)
	ON_TRECEIVE(CS_DELSELFOBJ_ACK)
	ON_TRECEIVE(CS_OTHERSELF_ACK);

	// Quest message
	ON_TRECEIVE(CS_QUESTLIST_COMPLETE_ACK)
	ON_TRECEIVE(CS_QUESTLIST_POSSIBLE_ACK)
	ON_TRECEIVE(CS_QUESTSTARTTIMER_ACK)
	ON_TRECEIVE(CS_QUESTCOMPLETE_ACK)
	ON_TRECEIVE(CS_QUESTUPDATE_ACK)
	ON_TRECEIVE(CS_CHAPTERMSG_ACK)
	ON_TRECEIVE(CS_QUESTLIST_ACK)
	ON_TRECEIVE(CS_QUESTADD_ACK)
	ON_TRECEIVE(CS_NPCTALK_ACK)

	// Private shop message
	ON_TRECEIVE(CS_STOREOPEN_ACK)
	ON_TRECEIVE(CS_STORECLOSE_ACK)
	ON_TRECEIVE(CS_STOREITEMLIST_ACK)
	ON_TRECEIVE(CS_STOREITEMBUY_ACK)
	ON_TRECEIVE(CS_STOREITEMSELL_ACK)

	// Messenger message
	ON_TRECEIVE(CS_FRIENDASK_ACK) 
	ON_TRECEIVE(CS_FRIENDADD_ACK) 
	ON_TRECEIVE(CS_FRIENDERASE_ACK)
	ON_TRECEIVE(CS_FRIENDLIST_ACK)
	ON_TRECEIVE(CS_FRIENDCONNECTION_ACK)
	ON_TRECEIVE(CS_FRIENDREGION_ACK)
	ON_TRECEIVE(CS_FRIENDGROUPMAKE_ACK)
	ON_TRECEIVE(CS_FRIENDGROUPDELETE_ACK)
	ON_TRECEIVE(CS_FRIENDGROUPCHANGE_ACK)
	ON_TRECEIVE(CS_FRIENDGROUPNAME_ACK)
	ON_TRECEIVE(CS_TMSINVITE_ACK)
	ON_TRECEIVE(CS_TMSRECV_ACK)
	ON_TRECEIVE(CS_TMSOUT_ACK)
	ON_TRECEIVE(CS_PROTECTEDLIST_ACK)
	ON_TRECEIVE(CS_PROTECTEDADD_ACK)
	ON_TRECEIVE(CS_PROTECTEDERASE_ACK)

	// Post message
	ON_TRECEIVE(CS_POSTSEND_ACK)
	ON_TRECEIVE(CS_POSTRECV_ACK)
	ON_TRECEIVE(CS_POSTLIST_ACK)
	ON_TRECEIVE(CS_POSTVIEW_ACK)
	ON_TRECEIVE(CS_POSTDEL_ACK)
	ON_TRECEIVE(CS_POSTGETITEM_ACK)
	ON_TRECEIVE(CS_POSTRETURN_ACK)
	ON_TRECEIVE(CS_POSTINFO_ACK)

	// Pet message
	ON_TRECEIVE(CS_PETMAKE_ACK)
	ON_TRECEIVE(CS_PETDEL_ACK)
	ON_TRECEIVE(CS_PETLIST_ACK)
	ON_TRECEIVE(CS_PETRECALL_ACK)
	ON_TRECEIVE(CS_PETRIDING_ACK)

	// Channel message
	ON_TRECEIVE(CS_CHGCHANNEL_ACK)

	// Cabinet message
	ON_TRECEIVE(CS_CABINETLIST_ACK)
	ON_TRECEIVE(CS_CABINETITEMLIST_ACK)
	ON_TRECEIVE(CS_CABINETOPEN_ACK)

	// Local messgage
	ON_TRECEIVE(CS_LOCALOCCUPY_ACK)

	// Duel message
	ON_TRECEIVE(CS_DUELINVITE_ACK)
	ON_TRECEIVE(CS_DUELSTANDBY_ACK)
	ON_TRECEIVE(CS_DUELSTART_ACK)
	ON_TRECEIVE(CS_DUELEND_ACK)

	// Get target message
	ON_TRECEIVE(CS_GETTARGET_ACK)
	ON_TRECEIVE(CS_GETTARGETANS_ACK)

	// SMS message
	ON_TRECEIVE(CS_SMSSEND_ACK)

	// Skill message
	ON_TRECEIVE(CS_SKILLINIT_ACK)
	ON_TRECEIVE(CS_SKILLLIST_ACK)
	ON_TRECEIVE(CS_SKILLINITPOSSIBLE_ACK)

	// Guild message
	ON_TRECEIVE(CS_GUILDESTABLISH_ACK)
	ON_TRECEIVE(CS_GUILDDISORGANIZATION_ACK)
	ON_TRECEIVE(CS_GUILDINVITE_ACK)
	ON_TRECEIVE(CS_GUILDJOIN_ACK)
	ON_TRECEIVE(CS_GUILDLEAVE_ACK)
	ON_TRECEIVE(CS_GUILDDUTY_ACK)
	ON_TRECEIVE(CS_GUILDMEMBERLIST_ACK)
	ON_TRECEIVE(CS_GUILDATTR_ACK)
	ON_TRECEIVE(CS_GUILDPEER_ACK)
	ON_TRECEIVE(CS_GUILDINFO_ACK)
	ON_TRECEIVE(CS_GUILDLOCALLIST_ACK)
	ON_TRECEIVE(CS_GUILDLOCALRETURN_ACK)
	ON_TRECEIVE(CS_GUILDCABINETLIST_ACK)
	ON_TRECEIVE(CS_GUILDCABINETPUTIN_ACK)
	ON_TRECEIVE(CS_GUILDCABINETTAKEOUT_ACK)
	ON_TRECEIVE(CS_GUILDCONTRIBUTION_ACK)
	ON_TRECEIVE(CS_GUILDARTICLELIST_ACK)
	ON_TRECEIVE(CS_GUILDARTICLEADD_ACK)
	ON_TRECEIVE(CS_GUILDARTICLEDEL_ACK)
	ON_TRECEIVE(CS_GUILDFAME_ACK)
	ON_TRECEIVE(CS_GUILDWANTEDLIST_ACK)
	ON_TRECEIVE(CS_GUILDWANTEDADD_ACK)
	ON_TRECEIVE(CS_GUILDWANTEDDEL_ACK)
	ON_TRECEIVE(CS_GUILDVOLUNTEERLIST_ACK)
	ON_TRECEIVE(CS_GUILDVOLUNTEERREPLY_ACK)
	ON_TRECEIVE(CS_GUILDVOLUNTEERING_ACK)
	ON_TRECEIVE(CS_GUILDVOLUNTEERINGDEL_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSLIST_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSINVITE_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSANSWER_ACK)
	ON_TRECEIVE(CS_GUILDARTICLEUPDATE_ACK)

	// Cash message
	ON_TRECEIVE(CS_CASHITEMCABINET_ACK)
	ON_TRECEIVE(CS_CASHITEMGET_ACK)
	ON_TRECEIVE(CS_CASHSHOPITEMLIST_ACK)
	ON_TRECEIVE(CS_CASHITEMBUY_ACK);
	ON_TRECEIVE(CS_PARTYMEMBERRECALL_ACK)
	ON_TRECEIVE(CS_PARTYMEMBERRECALLANS_ACK)
	ON_TRECEIVE(CS_CASHITEMPRESENT_ACK)
	ON_TRECEIVE(CS_RESETPCBANG_ACK)
	ON_TRECEIVE(CS_CASHCABINETBUY_ACK)
	ON_TRECEIVE(CS_CASHITEMPUTIN_ACK)
	ON_TRECEIVE(CS_CASHCABINETBUYCASH_ACK)

	// Soulmate message
	ON_TRECEIVE(CS_SOULMATE_ACK)
	ON_TRECEIVE(CS_SOULMATESEARCH_ACK)
	ON_TRECEIVE(CS_SOULMATEREGREADY_ACK)
	ON_TRECEIVE(CS_SOULMATEREG_ACK)
	ON_TRECEIVE(CS_SOULMATEEND_ACK)

	// Gamble message
	ON_TRECEIVE(CS_GAMBLECHECK_ACK)
	ON_TRECEIVE(CS_GAMBLEOPEN_ACK)

	// Siege message
	ON_TRECEIVE(CS_ADDGODTOWER_ACK)
	ON_TRECEIVE(CS_DELGODTOWER_ACK)
	ON_TRECEIVE(CS_ADDGODBALL_ACK)
	ON_TRECEIVE(CS_DELGODBALL_ACK)
	ON_TRECEIVE(CS_TAKEGODBALL_ACK)
	ON_TRECEIVE(CS_REMOVEGODBALL_ACK)
	ON_TRECEIVE(CS_DROPGODBALL_ACK)
	ON_TRECEIVE(CS_MOUNTGODBALL_ACK)
	ON_TRECEIVE(CS_DEMOUNTGODBALL_ACK)
	ON_TRECEIVE(CS_BALANCEOFPOWER_ACK)
	ON_TRECEIVE(CS_CASTLEAPPLY_ACK)
	ON_TRECEIVE(CS_ENDWAR_ACK)
	ON_TRECEIVE(CS_ENTERCASTLE_ACK)
	ON_TRECEIVE(CS_LEAVECASTLE_ACK)
	ON_TRECEIVE(CS_NPCMONSTERLIST_ACK)
	ON_TRECEIVE(CS_MONSTERBUY_ACK)

	// Duration message
	ON_TRECEIVE(CS_DURATIONDEC_ACK)
	ON_TRECEIVE(CS_DURATIONEND_ACK)
	ON_TRECEIVE(CS_DURATIONREPCOST_ACK)
	ON_TRECEIVE(CS_DURATIONREP_ACK)
	ON_TRECEIVE(CS_REFINECOST_ACK)
	ON_TRECEIVE(CS_REFINE_ACK)

	ON_TRECEIVE(CS_CHANGECHARBASE_ACK)
	ON_TRECEIVE(CS_STOPTHECLOCK_ACK)
	ON_TRECEIVE(CS_HEROSELECT_ACK)
	ON_TRECEIVE(CS_HEROLIST_ACK)

	ON_TRECEIVE(CS_SYSTEMMSG_ACK)
	ON_TRECEIVE(CS_OPENMONEY_ACK)

	ON_TRECEIVE(CS_COMMENT_ACK)
	ON_TRECEIVE(CS_ITEMCHANGE_ACK)
	ON_TRECEIVE(CS_COUNTDOWN_ACK)

	ON_TRECEIVE(CS_CHANGEITEMATTR_ACK)
	ON_TRECEIVE(CS_WASTEREFINE_ACK)
	ON_TRECEIVE(CS_CASHSHOPSTOP_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSWANTEDADD_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSWANTEDDEL_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSWANTEDLIST_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSVOLUNTEERING_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSVOLUNTEERINGDEL_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSREPLY_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSKICKOUT_ACK)
	ON_TRECEIVE(CS_GUILDTACTICSVOLUNTEERLIST_ACK)
	ON_TRECEIVE(CS_REGGUILDCLOAK_ACK)
	ON_TRECEIVE(CS_RESETCLOAK_ACK)

	ON_TRECEIVE(CS_PVPPOINT_ACK)
	ON_TRECEIVE(CS_GUILDPOINTLOG_ACK)
	ON_TRECEIVE(CS_GUILDPOINTREWARD_ACK)
	ON_TRECEIVE(CS_GUILDPVPRECORD_ACK)
	ON_TRECEIVE(CS_PVPRECORD_ACK)

	ON_TRECEIVE(CS_AUCTIONFIND_ACK)
	ON_TRECEIVE(CS_AUCTIONREGLIST_ACK)
	ON_TRECEIVE(CS_AUCTIONBIDLIST_ACK)
	ON_TRECEIVE(CS_AUCTIONINTERESTLIST_ACK)

	ON_TRECEIVE(CS_AUCTIONBID_ACK)
	ON_TRECEIVE(CS_AUCTIONBUYDIRECT_ACK)
	ON_TRECEIVE(CS_AUCTIONREG_ACK)
	ON_TRECEIVE(CS_AUCTIONREGCANCEL_ACK)
	ON_TRECEIVE(CS_AUCTIONINTEREST_ACK)

	ON_TRECEIVE( CS_KICKOUTMAP_ACK )
		
	ON_TRECEIVE( CS_FAMERANKLIST_ACK )
	ON_TRECEIVE( CS_MONTHRANKLIST_ACK )
	ON_TRECEIVE( CS_UPDATEFAMERANKLIST_ACK )

	ON_TRECEIVE( CS_AFTERMATH_ACK )
	ON_TRECEIVE( CS_CHATBAN_ACK )
	ON_TRECEIVE( CS_FIRSTGRADEGROUP_ACK )

#ifdef ADD_TOURNAMENT
	ON_TRECEIVE( CS_TOURNAMENTAPPLYINFO_ACK )
	ON_TRECEIVE( CS_TOURNAMENTAPPLY_ACK )
	ON_TRECEIVE( CS_TOURNAMENTJOINLIST_ACK )
	ON_TRECEIVE( CS_TOURNAMENTPARTYLIST_ACK )
	ON_TRECEIVE( CS_TOURNAMENTPARTYADD_ACK )
	ON_TRECEIVE( CS_TOURNAMENTMATCHLIST_ACK	 )
	ON_TRECEIVE( CS_TOURNAMENTEVENTLIST_ACK )
	ON_TRECEIVE( CS_TOURNAMENTEVENTINFO_ACK )
	ON_TRECEIVE( CS_TOURNAMENTEVENTJOIN_ACK )
	ON_TRECEIVE( CS_TOURNAMENTCHEER_ACK )
	ON_TRECEIVE( CS_TOURNAMENTENTER_ACK )
	ON_TRECEIVE( CS_TOURNAMENTPARTYDEL_ACK)
	ON_TRECEIVE( CS_TOURNAMENTRESULT_ACK)
	ON_TRECEIVE( CS_TOURNAMENTAPPLYCANCEL_ACK)
	ON_TRECEIVE( CS_TOURNAMENTSCHEDULE_ACK)
	ON_TRECEIVE( CS_TOURNAMENTSELECTWINNER_ACK)
#endif
	ON_TRECEIVE( CS_ITEMLEVELREVISION_ACK)
#ifndef TEST_MODE
	ON_TRECEIVE( CS_HACKSHIELD_ACK )
#endif

#ifdef ADD_RPS
	ON_TRECEIVE( CS_RPSSTART_ACK )
	ON_TRECEIVE( CS_RPSGAME_ACK )
#endif
	default	: break;
	}
	return nERROR;
}

void CTClientGame::ReleaseForTELEPORT()
{
	((CTClientObjBase *) m_pMainChar)->CalcTick( m_pDevice->m_pDevice, 0);
	m_pCtrlHost = m_pMainChar;

	m_vTPOINTER.Release();
	ResetTargetOBJ(NULL);
	ResetTargetINFO(FALSE);
	CalcMainChar(0);
	CalcCamera(0);

	m_bGroundLOCK = FALSE;
	m_bAutoRun = FALSE;
	m_bRevival = FALSE;
	m_pACT = NULL;
	m_pHIT = NULL;

	m_vDEADMONHITS.clear();
	m_nDEADMONHITIDX = 0;

	m_bMoveType = TMAINMOVE_NORMAL;
	m_dwMoveGM = 0;
	m_fBOUND = 0.0f;

	DisableUI(TFRAME_WORLDMAP);
	OnGM_CLOSE_POPUP();
	ReleaseNumber();
	ReleaseArrow();
	CancelCharge(TRUE);

	MAPTSKILL::iterator itTSKILL;

	for( itTSKILL = m_pMainChar->m_mapTSKILL.begin(); itTSKILL != m_pMainChar->m_mapTSKILL.end(); itTSKILL++)
	{
		CTClientSkill *pTSKILL = (*itTSKILL).second;

		if( pTSKILL->m_bTimerON && !pTSKILL->m_dwTick )
			pTSKILL->m_bTimerON = FALSE;
	}
	m_pMainWnd->CloseMessageBox(TRUE);

	if(m_pOpenShopByCash)
	{
		delete m_pOpenShopByCash;
		m_pOpenShopByCash = NULL;
	}

	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;
	m_bLockGM_CASHCAB_GETITEM = FALSE;

	ReleaseNumber();
}

void CTClientGame::ReleaseGame()
{
	MAPTSWITCH::iterator itTSWITCH;
	MAPMONSTER::iterator itMON;
	MAPPLAYER::iterator itPC;
	MAPRECALL::iterator itRECALL;	
	TMAPFAMERANK::iterator itFAMERANK;

	CTClientObjBase::m_mapTSELECTOBJ.clear();
	CTClientObjBase::m_mapTSELECTDEADOBJ.clear();
	CTClientItem::m_mapTITEMTICK.clear();
	CTClientItem::m_mapTITEMLOCK.clear();
	SiegeMgr->Release();

	ResetTargetOBJ(NULL);
	ResetTargetINFO(FALSE);

	for( int i=0 ; i < FRT_COUNT ; ++i )
	{
		for( itFAMERANK = m_mapTFAMERANK[ i ].begin(); itFAMERANK != m_mapTFAMERANK[ i ].end(); itFAMERANK++ )
			delete (*itFAMERANK).second;
		m_mapTFAMERANK[ i ].clear();
	}

	for( itRECALL = m_mapFAKETEMP.begin(); itRECALL != m_mapFAKETEMP.end(); itRECALL++)
		delete (*itRECALL).second;
	m_mapFAKETEMP.clear();

	for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
		delete (*itRECALL).second;
	m_mapSLAVEFIXRECALL.clear();
	m_mapFIXRECALL.clear();

	for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
		delete (*itRECALL).second;
	m_mapSLAVERECALL.clear();
	m_mapRECALL.clear();

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
		delete (*itMON).second;
	m_mapSLAVEMON.clear();
	m_mapMONSTER.clear();

	for( itTSWITCH = m_mapTSwitch.begin(); itTSWITCH != m_mapTSwitch.end(); itTSWITCH++)
		delete (*itTSWITCH).second;
	m_mapTSwitch.clear();

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
		delete (*itPC).second;
	m_mapPLAYER.clear();

	if(CTachyonRes::m_MEDIA.IsPlay( MEDIA_TSOUND, m_dwENV, 0))
		CTachyonRes::m_MEDIA.Stop( MEDIA_TSOUND, m_dwENV, 0);
	m_dwENV = 0;

	if(m_vDRAG.m_pIMAGE)
	{
		delete m_vDRAG.m_pIMAGE;
		m_vDRAG.m_pIMAGE = NULL;
		m_vDRAG.m_bFrameID = TFRAME_COUNT;
		m_vDRAG.m_bSlotID = 0;
		m_vDRAG.m_dwParam = 0;
	}

	m_pMainChar->SetRidingPet(NULL);

	m_bLButtonDown = FALSE;
	m_bRButtonDown = FALSE;
	m_bGroundLOCK = FALSE;
	m_bGroundATK = FALSE;
	m_bRSCSMENU = FALSE;
	m_bLoopLock = FALSE;
	m_bTELEPORT = FALSE;
	m_bRSCS = FALSE;

	m_dwTSKY = 0;
	m_wTSKY = 0;
	m_bTSKY = 0;

	m_dwLButtonTick = 0;
	m_dwRButtonTick = 0;

	m_pACT = NULL;
	m_pHIT = NULL;

	m_bAutoRun = FALSE;
	m_bRevival = FALSE;
	m_bCamFIX = FALSE;

	m_nCamZOOM = 0;
	m_wCamDIR = 0;

	m_bMoveType = TMAINMOVE_NORMAL;
	m_dwMoveGM = 0;
	m_fBOUND = 0.0f;

	m_dwTGROUND = 0;
	m_dwTSELECT = 0;
	m_dwTSHADOW = 0;

	m_dwDHELPTICK = 0;
	m_dwDHELP = 0;

	m_bMonSlotID = 0;
	m_bInvenID = 0;
	m_bSlotID = 0;

	m_wNextSkillID = 0;
	m_wSkillID = 0;

	m_pMainChar->m_bMode = MT_NORMAL;
	m_pMainChar->m_pTRANS = NULL;

	m_dwSoulRegion = 0;
	m_dwSoulID = 0;
	m_bInvenDraging = FALSE;
	m_pTOPFRAME = NULL;

	m_bFirstConnect = FALSE;
	EnableAllInputDevice();

	m_vTPOINTER.Release();
	m_vKEY.ResetSTATE();
	m_vMAP.ReleaseMAP();

	m_vTMAINTAINOBJ.clear();
	ReleaseNumber();
	ReleaseArrow();
	ReleaseParty();
	ReleaseRSCS();

	CancelCharge(TRUE);
	ClearMonInven();
	EndQUAKE();

	CTClientBSP::ReleaseTBSPMAP();
	CTMiniDump::SetCharID(NULL);
	CTMiniDump::SetSocket(INVALID_SOCKET);

	m_dwCmdParam = 0;
	m_dwNameTick = 0;
	m_ptPrevPos = CPoint(-1,-1);

	CTMainUI *pMainUI = (CTMainUI *) m_vTFRAME[TFRAME_MAIN];
	if(pMainUI)
	{
		pMainUI->m_bNotifyNewCashCabinet = FALSE;
		pMainUI->m_bNotifyNewMail = FALSE;
		pMainUI->m_bNotifyNewMsg = FALSE;
	}

	CTGaugePannel *pTPANNEL = (CTGaugePannel *) m_vTFRAME[TFRAME_GAUGE];
	if(pTPANNEL)
	{
		if(pTPANNEL->m_pQList)
			pTPANNEL->m_pQList->RemoveAll();

		pTPANNEL->m_dwCaution15Tick = 0;
		pTPANNEL->m_dwCaution15State = 0;
		pTPANNEL->DettachCaution15();

		if(pTPANNEL->m_pLUCKY3)
			pTPANNEL->m_pLUCKY3->m_strText.Empty();
		if(pTPANNEL->m_pLUCKY4)
			pTPANNEL->m_pLUCKY4->m_strText.Empty();
	}

#if defined(TEST_MODE) || defined(DEBUG)
	ReleaseTSPAWN();
#endif

	if( m_pOpenShopByCash )
	{
		delete m_pOpenShopByCash;
		m_pOpenShopByCash = NULL;
	}

	m_bLockGM_CASHCAB_GETITEM = FALSE;
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

#ifdef ADD_SNOW
	EndSnowEvent();
#endif

	m_bResetMAP = TRUE;

	if( m_pTLeftTeam )
		delete m_pTLeftTeam;
	m_pTLeftTeam = NULL;
	
	if( m_pTRightTeam )
		delete m_pTRightTeam;
	m_pTRightTeam = NULL;

	m_bDoAutoRevival = FALSE;
	m_dwLeftTickAR = 0;
	m_llPrevTSecond=0;

	m_bAlreadyRecvSiegeAlarm = FALSE;
	m_bTournamentGateOpened = FALSE;
	m_bEnableNPCTick = TRUE;

	m_bItemLevelRevision = 0;

	for( int i=0 ; i < MAX_FIRECRACKER ; ++i )
		m_dwFirecrackerID[ i ] = 0;
	m_dwRemainLifeTimeFirecracker = 0;
#ifdef ADD_RPS
	m_dwRPSGameType = 0;
	m_bRPSWinCount = 0;
	m_bRPSKeyEnable = FALSE;
	m_bRPSGaming = FALSE;
#endif

	m_bSendedCS_CASHITEMPUTIN_REQ = FALSE;
	m_bMainCharMoveLock = false;
	m_dwRemainTickVerifyMaintain = 0;
}

void CTClientGame::BeginCharge( DWORD dwTick, TCHARGETYPE eCHARGETYPE)
{
	if(!m_bLOOPCHARGE)
	{
		m_vTGAUGE[TGAUGE_CHARGE]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, 0);
		m_vTGAUGE[TGAUGE_CHARGE]->SetGauge( 0, dwTick);
		m_vTGAUGE[TGAUGE_CHARGE]->ShowComponent(TRUE);
	}

	m_dwMaxChargeTick = dwTick;
	m_dwChargeTick = 0;

	m_bCHARGE = TRUE;
	m_eCHARGETYPE = eCHARGETYPE;

	// 은신 중이면 푼다.
	if( eCHARGETYPE == TCHARGETYPE_OPEN)
		m_pMainChar->CancelHiding( this );
}

void CTClientGame::CancelCharge( BYTE bClearActOBJ)
{
	m_vTGAUGE[TGAUGE_CHARGE]->ShowComponent(FALSE);

	if(bClearActOBJ)
		m_pACT = NULL;

	m_dwMaxChargeTick = 0;
	m_dwChargeTick = 0;
	m_wNextSkillID = 0;

	m_bLOOPCHARGE = FALSE;
	m_bCHARGE = FALSE;
	m_bGroundLOCK = FALSE;
	m_eCHARGETYPE = TCHARGETYPE_UNKNOWN;
}

void CTClientGame::CancelDoEXIT()
{
	m_pMainWnd->CloseMessageBox();

	m_bDoEXIT = FALSE;
	m_dwLeftTickEXIT = 0;
	m_bDoSelectCHAR = FALSE;
	m_dwLeftTickCHAR = 0;
	m_bDoChangeCH = FALSE;
	m_dwLeftTickCH = 0;
}

void CTClientGame::CancelTrade()
{
	if(m_vTFRAME[TFRAME_TRADE]->IsVisible())
		m_pSESSION->SendCS_DEALITEM_REQ(FALSE);

	DisableUI(TFRAME_TRADE);
}

void CTClientGame::CancelPrivateBuy()
{
	DisableUI(TFRAME_PRIVATE_BUY);
}

void CTClientGame::CancelPrivateSell()
{
	if(m_vTFRAME[TFRAME_PRIVATE_SELL]->IsVisible())
	{
		CTPrivateShopDlgForSell* pDlg =
			static_cast<CTPrivateShopDlgForSell*>(m_vTFRAME[TFRAME_PRIVATE_SELL]);

		if( m_pSESSION && pDlg->IsSelling() )
			m_pSESSION->SendCS_STORECLOSE_REQ();

		DisableUI(TFRAME_PRIVATE_SELL);
	}
}

void CTClientGame::CancelAllItemUI()
{
	CancelTrade();
	
	CancelPrivateBuy();
	CancelPrivateSell();

	DisableUI(TFRAME_CHAR_INFO);
	
	DisableUI(TFRAME_ITEM_SHOP);
	DisableUI(TFRAME_RENT_SKILL);
	DisableUI(TFRAME_MONINVEN);
	DisableUI(TFRAME_NPCDIALOG);
	DisableUI(TFRAME_CRAFT);
		
	DisableUI(TFRAME_ITEM_UP);
	DisableUI(TFRAME_ITEM_REPAIR);
	DisableUI(TFRAME_ITEM_REFINE);
	DisableUI(TFRAME_ITEM_GAMBLE);
	DisableUI(TFRAME_ITEM_EXTPERIOD);

	DisableUI(TFRAME_MAIL_SEND);
	DisableUI(TFRAME_MAIL_RECV);
	
	DisableUI(TFRAME_PET_MANAGE);
	DisableUI(TFRAME_PET_MAKE);
	
	DisableUI(TFRAME_CABINET);
	DisableUI(TFRAME_GUILDCABINET);

    DisableUI(TFRAME_CASHCABINET);
	DisableUI(TFRAME_CASHSHOP_NEW);
	DisableUI(TFRAME_SMS);

	if( static_cast<CTSkillDlg*>(m_vTFRAME[TFRAME_SKILL])->m_bSKILLINIT )
		DisableUI(TFRAME_SKILL);
}

BOOL CTClientGame::IsItemUI(CTClientUIBase* pUI)
{
	if( m_vTFRAME[TFRAME_TRADE] == pUI ||
		m_vTFRAME[TFRAME_PRIVATE_SELL] == pUI ||
		m_vTFRAME[TFRAME_PRIVATE_BUY] == pUI ||
		m_vTFRAME[TFRAME_CHAR_INFO] == pUI ||
		m_vTFRAME[TFRAME_ITEM_SHOP] == pUI ||
		m_vTFRAME[TFRAME_MONINVEN] == pUI ||
		m_vTFRAME[TFRAME_NPCDIALOG] == pUI ||
		m_vTFRAME[TFRAME_ITEM_UP] == pUI ||
		m_vTFRAME[TFRAME_ITEM_REPAIR] == pUI ||
		m_vTFRAME[TFRAME_ITEM_REFINE] == pUI ||
		m_vTFRAME[TFRAME_ITEM_GAMBLE] == pUI ||
		m_vTFRAME[TFRAME_ITEM_EXTPERIOD] == pUI ||
		m_vTFRAME[TFRAME_MAIL_SEND] == pUI ||
		m_vTFRAME[TFRAME_MAIL_RECV] == pUI ||
		m_vTFRAME[TFRAME_PET_MAKE] == pUI ||
		m_vTFRAME[TFRAME_CABINET] == pUI ||
		m_vTFRAME[TFRAME_SMS] == pUI || 
		m_vTFRAME[TFRAME_GUILDCABINET] == pUI || 
		m_vTFRAME[TFRAME_CASHCABINET] == pUI ||
		m_vTFRAME[TFRAME_CASHSHOP_NEW] == pUI ||
		m_vTFRAME[TFRAME_CRAFT] == pUI ||
		m_vTFRAME[TFRAME_RENT_SKILL] == pUI )
	{
		return TRUE;
	}

	if( m_vTFRAME[TFRAME_SKILL] == pUI &&
		static_cast<CTSkillDlg*>(m_vTFRAME[TFRAME_SKILL])->m_bSKILLINIT )
	{
		return TRUE;
	}

	return FALSE;
}

void CTClientGame::DoHotKey(THOTKEY_BASE eTYPE, BYTE bHotkeyID)
{
	if( m_bTournamentWatching )
		return ;

#ifdef ADD_RPS
	if( m_bRPSGaming && m_bRPSKeyEnable )
	{
		switch(bHotkeyID)
		{
		case 0: GetSession()->SendCS_RPSGAME_REQ( RPSCARD_SCISSORS ); break; //1키
		case 1: GetSession()->SendCS_RPSGAME_REQ( RPSCARD_ROCK ); break; //2키
		case 2: GetSession()->SendCS_RPSGAME_REQ( RPSCARD_PAPER ); break; //3키
		}
		return ;
	}
#endif

	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
	LPTHOTKEY pTHOTKEY = pTGaugePannel->GetHotKey(eTYPE, bHotkeyID);

	if( pTHOTKEY )
	{
		m_pCtrlHost = GetCtrlOBJ();

		switch(pTHOTKEY->m_bType)
		{
		case HOTKEY_SKILL	:
			{
				CTClientSkill *pTSKILL = m_pMainChar->FindTSkill(pTHOTKEY->m_wID);

				if( pTSKILL )
					if(pTSKILL->m_bLoopON)
						pTSKILL->m_bLoopON = FALSE;
					else
						UseTSkill( pTSKILL->m_pTSKILL->m_wSkillID, FALSE, TRUE);
			}

			break;

		case HOTKEY_ITEM	: UseTItem(pTHOTKEY->m_wID); break;
		case HOTKEY_PET :
			{
				CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
				if( pTGaugePannel->m_pTHOTKEY[ eTYPE ][ bHotkeyID ][1]->IsEnable() )
				{
					UseTPet(pTHOTKEY->m_wID);
				}
			}
			break;
		}
	}
}

void CTClientGame::ReleaseNumber()
{
	while(!m_vTNUMBER.empty())
	{
		delete m_vTNUMBER.back();
		m_vTNUMBER.pop_back();
	}
}

void CTClientGame::ReleaseArrow()
{
	while(!m_vTARROW.empty())
	{
		delete m_vTARROW.back();
		m_vTARROW.pop_back();
	}
}

void CTClientGame::ReleaseParty()
{
	while(!m_vTPARTY.empty())
	{
		delete m_vTPARTY.back();
		m_vTPARTY.pop_back();
	}

	m_pTPARTY = NULL;
}

BYTE CTClientGame::GetHotKey( WORD wKEY)
{
	switch(wKEY)
	{
	case TKEY_SKILL2_1	:
	case TKEY_SKILL_1	: return 0;

	case TKEY_SKILL2_2	:
	case TKEY_SKILL_2	: return 1;

	case TKEY_SKILL2_3	:
	case TKEY_SKILL_3	: return 2;

	case TKEY_SKILL2_4	:
	case TKEY_SKILL_4	: return 3;

	case TKEY_SKILL2_5	:
	case TKEY_SKILL_5	: return 4;

	case TKEY_SKILL2_6	:
	case TKEY_SKILL_6	: return 5;

	case TKEY_SKILL2_7	:
	case TKEY_SKILL_7	: return 6;

	case TKEY_SKILL2_8	:
	case TKEY_SKILL_8	: return 7;

	case TKEY_SKILL2_9	:
	case TKEY_SKILL_9	: return 8;

	case TKEY_SKILL2_10	:
	case TKEY_SKILL_10	: return 9;

	case TKEY_SKILL2_11	:
	case TKEY_SKILL_11	: return 10;

	case TKEY_SKILL2_12	:
	case TKEY_SKILL_12	: return 11;
	}

	return MAX_HOTKEY_POS;
}

BYTE CTClientGame::CloseTopFrame()
{
	BYTE bFrameID = GetTopFrame();

	if( IsInvenFrame(bFrameID) )
	{
		DisableUI(TFRAME_DEFINVEN);
		EnableAllSubInven(FALSE);
		
		return bFrameID;
	}
	else if( bFrameID == TFRAME_PRIVATE_SELL )
	{
		CancelPrivateSell();

		return bFrameID;
	}
	else if( bFrameID == TFRAME_TRADE )
	{
		CancelTrade();

		return bFrameID;
	}
	else if( bFrameID == TFRAME_TOURNAMENT_GAGE )
	{
		if( m_bTournamentBattle || m_bTournamentWatching )
			return (BYTE)T_INVALID;
	}
	else if( bFrameID != (BYTE)T_INVALID )
	{
		DisableUI(bFrameID);
		return bFrameID;
	}

	return (BYTE)T_INVALID;
}

BYTE CTClientGame::GetTopFrame()
{
	TCOMP_LIST::reverse_iterator itr = m_kids.rbegin();
	for(; itr!=m_kids.rend(); ++itr)
	{
		TComponent* pKid = *itr;

		if( pKid->IsVisible() &&
			pKid != m_vTFRAME[TFRAME_MAIN] &&
			pKid != m_vTFRAME[TFRAME_GAUGE] &&
			pKid != m_vTFRAME[TFRAME_DETAIL_INFO] &&
			pKid != m_vTFRAME[TFRAME_CHAT_DETAIL_INFO] &&
			pKid != m_vTFRAME[TFRAME_SIEGEUI] &&
			pKid != m_vTFRAME[TFRAME_SIEGEEND] )
		{
			return GetFrameID(pKid);
		}
	}

	return (BYTE)T_INVALID;
}

void CTClientGame::OnActivateKEY( WORD wSCAN)
{
	DWORD dwKEY = CTKeySetting::GetInstance()
		->GetCurKeySet(wSCAN, CTClientKEY::GetCurMOD());

	switch( dwKEY )
	{
	case TKEY_CAM_ROT180:
		{
			if(!m_vKEY.m_vSTATE[TKEY_CAM_ROT180])
			{
				m_bOrgTKEY_ROT = m_vKEY.m_vSTATE[TKEY_ROT];
				m_vKEY.m_vSTATE[TKEY_CAM_ROT] = TKEY_STATE_PRESS;
				m_vKEY.m_vSTATE[TKEY_ROT] = TKEY_STATE_NONE;
				RotCameraY(900);
			}
		}

		break;
	}

#ifdef ADD_TEXTOOL
	if( m_vTFRAME[ TFRAME_TEXTOOL ]->IsVisible() )
	{
		CTexTool* pTT = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] );
		switch(dwKEY)
		{
		case TKEY_CAM_LEFT:
			pTT->MoveLeft();
			return ;
		case TKEY_CAM_RIGHT:
			pTT->MoveRight();
			return ;
		case TKEY_CAM_UP:
			pTT->MoveUp();
			return ;
		case TKEY_CAM_DOWN:
			pTT->MoveDown();
			return ;
		}
	}
#endif

	switch( m_vKEY.m_vSTATE[dwKEY] )
	{
	case TKEY_STATE_NONE :
		m_vKEY.m_vTICK[dwKEY] = m_vKEY.m_dwTick;
		m_vKEY.m_vSTATE[dwKEY] = TKEY_STATE_PRESS;
		break;

	case TKEY_STATE_PRESS : 
		if( dwKEY == TKEY_JUMP ) break;
		else return ;
	}

	switch(dwKEY)
	{
	case TKEY_CAM_LEFT			:
	case TKEY_CAM_RIGHT			:
	case TKEY_CAM_UP			:
	case TKEY_CAM_DOWN			:
	case TKEY_CAM_ROT			:
	case TKEY_ZOOM_IN			:
	case TKEY_ZOOM_OUT			:
	case TKEY_ROT				: break;

	case TKEY_WORLDMAP			:
		if(m_vTFRAME[TFRAME_WORLDMAP]->IsVisible())
			DisableUI(TFRAME_WORLDMAP);
		else
			OnGM_OPEN_WORLDMAP();

		break;

	case TKEY_CLOSE_UI			:
#ifdef ADD_RPS
		if( m_vTFRAME[TFRAME_RPS]->IsVisible() || m_vTFRAME[TFRAME_RPS_REWARD]->IsVisible() )
			break;
#endif
		if( m_pChatFrame->IsChatEditON() )
			m_pChatFrame->ResetChatMsg(FALSE);
		else if( CloseTopFrame() == (BYTE)T_INVALID )
		{
			if( m_pTARGET || m_pTPARTY )
			{
				ResetPartyOBJ(NULL);
				ResetTargetOBJ(NULL);
				ResetTargetINFO(FALSE);
			}
			else
				OnGM_MAIN_MENU();
		}
		break;

	case TKEY_HOTKEY_ACTION		: DoDEFACTION(); break;
	case TKEY_TRSCS_ME			:
	case TKEY_TRSCS_F1			:
	case TKEY_TRSCS_F2			:
	case TKEY_TRSCS_F3			:
	case TKEY_TRSCS_F4			:
	case TKEY_TRSCS_F5			:
	case TKEY_TRSCS_F6			:
	case TKEY_TRSCS_F7			: SelectTRSCS(dwKEY); break;
	
	case TKEY_HOTKEY_F1			: SelectHotKey(THOTKEYBASE_DEF, 0); break;
	case TKEY_HOTKEY_F2			: SelectHotKey(THOTKEYBASE_DEF, 1); break;
	case TKEY_HOTKEY_F3			: SelectHotKey(THOTKEYBASE_DEF, 2); break;
	case TKEY_HOTKEY_F4			: SelectHotKey(THOTKEYBASE_DEF, 3); break;
	case TKEY_HOTKEY_F5			: SelectHotKey(THOTKEYBASE_DEF, 4); break;
	case TKEY_HOTKEY_F6			: SelectHotKey(THOTKEYBASE_DEF, 5); break;
	case TKEY_HOTKEY_F7			: SelectHotKey(THOTKEYBASE_DEF, 6); break;
	case TKEY_HOTKEY_F8			: SelectHotKey(THOTKEYBASE_DEF, 7); break;
	case TKEY_HOTKEY_F9			: SelectHotKey(THOTKEYBASE_DEF, 8); break;
	case TKEY_HOTKEY_F10		: SelectHotKey(THOTKEYBASE_DEF, 9); break;

	case TKEY_HOTKEY_PREV		: 
		{
			CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
			pTGaugePannel->HotkeyPrev(THOTKEYBASE_DEF);
		}
		break;
	case TKEY_HOTKEY_NEXT		: 
		{
			CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
			pTGaugePannel->HotkeyNext(THOTKEYBASE_DEF);
		}
		break;
	
	case TKEY_RECALL_SKILL_1	:
	case TKEY_RECALL_SKILL_2	:
	case TKEY_RECALL_SKILL_3	:
	case TKEY_RECALL_SKILL_4	: DoRecallSKILL(dwKEY); break;

	case TKEY_RECALL_PASSIVE	:
	case TKEY_RECALL_ACTIVE		:
	case TKEY_RECALL_MANUAL		:
	case TKEY_RECALL_STAY		:
	case TKEY_RECALL_BACK		: DoRecallAI(dwKEY); break;
	
	case TKEY_SKILL_1			: DoHotKey(THOTKEYBASE_DEF,  0); break;
	case TKEY_SKILL_2			: DoHotKey(THOTKEYBASE_DEF,  1); break;
	case TKEY_SKILL_3			: DoHotKey(THOTKEYBASE_DEF,  2); break;
	case TKEY_SKILL_4			: DoHotKey(THOTKEYBASE_DEF,  3); break;
	case TKEY_SKILL_5			: DoHotKey(THOTKEYBASE_DEF,  4); break;
	case TKEY_SKILL_6			: DoHotKey(THOTKEYBASE_DEF,  5); break;
	case TKEY_SKILL_7			: DoHotKey(THOTKEYBASE_DEF,  6); break;
	case TKEY_SKILL_8			: DoHotKey(THOTKEYBASE_DEF,  7); break;
	case TKEY_SKILL_9			: DoHotKey(THOTKEYBASE_DEF,  8); break;
	case TKEY_SKILL_10			: DoHotKey(THOTKEYBASE_DEF,  9); break;
	case TKEY_SKILL_11			: DoHotKey(THOTKEYBASE_DEF, 10); break;
	case TKEY_SKILL_12			: DoHotKey(THOTKEYBASE_DEF, 11); break;

	case TKEY_SKILL2_1			: DoHotKey(THOTKEYBASE_SUB1,  0); break; 
	case TKEY_SKILL2_2			: DoHotKey(THOTKEYBASE_SUB1,  1); break;
	case TKEY_SKILL2_3			: DoHotKey(THOTKEYBASE_SUB1,  2); break;
	case TKEY_SKILL2_4			: DoHotKey(THOTKEYBASE_SUB1,  3); break;
	case TKEY_SKILL2_5			: DoHotKey(THOTKEYBASE_SUB1,  4); break;
	case TKEY_SKILL2_6			: DoHotKey(THOTKEYBASE_SUB1,  5); break;
	case TKEY_SKILL2_7			: DoHotKey(THOTKEYBASE_SUB1,  6); break;
	case TKEY_SKILL2_8			: DoHotKey(THOTKEYBASE_SUB1,  7); break;
	case TKEY_SKILL2_9			: DoHotKey(THOTKEYBASE_SUB1,  8); break;
	case TKEY_SKILL2_10			: DoHotKey(THOTKEYBASE_SUB1,  9); break;
	case TKEY_SKILL2_11			: DoHotKey(THOTKEYBASE_SUB1, 10); break;
	case TKEY_SKILL2_12			: DoHotKey(THOTKEYBASE_SUB1, 11); break;
		
	case TKEY_PICK_NEXT			: PickNext(TPICK_ALIVE_ALL); break;
	case TKEY_PICK_NEXT_DEAD:
		{
#ifdef TEST_MODE
			if( m_bShowDebugInfo && m_pMainChar)
			{
				CString strTEXT;
				D3DXVECTOR3 vTPOS = m_pMainChar->GetPosition();
				strTEXT.Format("%.2f\t%.2f\t%.2f\t%d", vTPOS.x, vTPOS.y, vTPOS.z, m_pMainChar->m_wDIR / 5);
				CopyToClipboard( strTEXT );
			}
			else
#endif
			PickNext(TPICK_DEAD_ALL);
		}
		break;

	case TKEY_AUTORUN			:
		if( !m_pMainChar->IsDead() && !m_pMainChar->IsBlock() && !m_pMainChar->IsDown() && !m_pMainChar->m_bPrivateShop )
		{
			m_bMoveType = TMAINMOVE_NORMAL;
			m_bAutoRun = !m_bAutoRun;
			m_dwMoveGM = 0;
		}

		break;

	case TKEY_JUMP				:
		if( !m_pMainChar->IsDead() && !m_pMainChar->IsBlock() && !m_pMainChar->IsDown() && !m_pMainChar->m_bPrivateShop )
		{
			m_pCtrlHost = GetCtrlOBJ();

			if(m_bCHARGE)
				CancelCharge(TRUE);

			if(!m_pMainChar->m_bGhost && !m_pMainChar->IsJump() )
			{
				SendCS_JUMP_REQ(
					m_pCtrlHost->m_dwID,
					m_pCtrlHost->m_bType,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					m_pCtrlHost->GetPositionX(),
					m_pCtrlHost->GetPositionY(),
					m_pCtrlHost->GetPositionZ(),
					m_pCtrlHost->m_wPITCH,
					m_pCtrlHost->m_wMoveDIR,
					m_pCtrlHost->m_bAction);
			}

			m_pCtrlHost->CancelTAction();
			m_pCtrlHost->Jump();

		}

		break;

	case TKEY_MODE				:
		if( m_pSESSION &&
			!m_pMainChar->IsDead() && !m_pMainChar->IsDown() && !m_pMainChar->IsFall() && 
			!m_pMainChar->IsJump() && !m_pMainChar->m_bPrivateShop && !m_pMainChar->GetRidingPet() )
		{
			if( m_pMainChar->m_bMode == MT_BATTLE )
			{
				LoopSkillOFF();
				m_pSESSION->SendCS_CHGMODE_REQ(MT_NORMAL);
			}

			BYTE bActionID;
			if( m_pMainChar->m_bAction == TA_SITDOWN )
				bActionID = TA_STAND;
			else
				bActionID = TA_SIT;

			TACTION vActionID = m_pMainChar->FindActionID(
					bActionID,
					m_pMainChar->GetWeaponID(m_pMainChar->m_bMode));

			SendCS_ACTION_REQ(
				m_pMainChar->m_dwID,
				m_pMainChar->m_bType,
				bActionID,
				vActionID.m_dwActID,
				vActionID.m_dwAniID,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID, 0);
		}

		break;

	case TKEY_GET_ALL			:
#ifdef TEST_MODE
		if( m_bEditTSPAWN && m_pTARGET && m_pTARGET->m_bType == OT_MON )
		{
			D3DXVECTOR3 vTPOS = m_pMainChar->GetPosition();

			AddTSPAWN(
				m_pTARGET,
				vTPOS.x,
				vTPOS.y,
				vTPOS.z,
				m_pMainChar->m_wDIR);
		}
		else if( m_bShowDebugInfo && m_pTARGET )
		{
			CString strTEXT;
			strTEXT.Format( "%d", LOWORD(m_pTARGET->m_dwID));
			CopyToClipboard( strTEXT );
		}
		else
#endif
		if( m_pTARGET && m_pSESSION && !m_pMainChar->IsDead() &&
			!m_pMainChar->m_bGhost && !m_pMainChar->IsBlock() && !m_pMainChar->IsDown() )
		{
			switch(m_pTARGET->m_bType)
			{
			case OT_NPC	:
				if( m_vTFRAME[TFRAME_MONINVEN]->IsVisible() && m_pTARGET->m_bNPCType == TNPC_BOX )
					OnGM_GETALL();

				break;

			case OT_MON	:
				if(m_pTARGET->IsDead())
				{
					if( m_vTFRAME[TFRAME_MONINVEN]->IsVisible() )
					{
						OnGM_GETMONEY();
						OnGM_GETALL();
					}
					else
						OnGM_GETALL();
				}

				break;
			}
		}

		break;

	case TKEY_QUEST_INFO:
		{
			OnGM_TOGGLE_QUESTINFO();
		}
		break;

	case TKEY_SKILL_INFO:
		{
			OnGM_TOGGLE_SKILLINFO();
		}
		break;

	case TKEY_CHAR_INFO:
		{
#ifdef TEST_MODE
			if( m_bShowDebugInfo && m_pTARGET )
			{
				CString strTEXT;
				strTEXT.Format( "%d", HIWORD(m_pTARGET->m_dwID));
				CopyToClipboard( strTEXT );
			}
			else
#endif
			OnGM_TOGGLE_CHARINFO();
		}
		break;

	case TKEY_TOGGLE_GUILD_UI	:
		{
			OnGM_TOGGLE_COMMUNITY();
		}
		break;

	case TKEY_PET_INFO			: OnGM_TOGGLE_PETMANAGE(); break;

	case TKEY_HELP:
		{
			OnGM_TOGGLE_HELP();
		}
		break;
	
	case TKEY_INVEN				:
		{
			if( !m_pMainChar->m_bPrivateShop)
			{
				OnGM_TOGGLE_DEFINVEN();
				EnableAllSubInven( m_vTFRAME[TFRAME_DEFINVEN]->IsVisible() );
			}
		}

		break;

	case TKEY_RUN:
		{
			m_pMainChar->m_bRun = !m_pMainChar->m_bRun;
		}
		break;

	case TKEY_SHOW_UI			:
		{
			CTSiegeEnd* pSiege = static_cast< CTSiegeEnd* >( m_vTFRAME[ TFRAME_SIEGEEND ] );
			if( pSiege->IsCounting() )
				m_bShowUI = FALSE;
			else
				m_bShowUI = !m_bShowUI;
		}break;

	case TKEY_FORWARD			:
	case TKEY_BACK				:
		{
			m_bMoveType = TMAINMOVE_NORMAL;
			m_bAutoRun = FALSE;
			m_dwMoveGM = 0;
		}

		break;

	case TKEY_HUD				:
		if(m_pGameSetting)
		{
			m_vTOPTION.m_bHUD = !m_vTOPTION.m_bHUD;
			m_pGameSetting->m_pTCHECK[TGAME_CHECK_HUD]->Select(m_vTOPTION.m_bHUD);
		}

		break;

	case TKEY_FOLLOW			:
		if( CanFollow(m_pTARGET) )
			OnGM_FOLLOW_TARGET();
		
		break;

	case TKEY_WHISPER_REPLY		:
		if( m_pCommandHandler )
			m_pCommandHandler->m_vCOMMAND.push_back(GM_WHISPER_REPLY);

		break;

	case TKEY_CHAR_ROT180:
		{
			RotCameraY(900);
			m_pCtrlHost->m_nDIR = 900;
		}

		break;

	case TKEY_FAMERANK: OnGM_TOGGLE_FAMERANK();	break;
	case TKEY_CAUTION15:
		{
			if( CTNationOption::KOR )
			{
				CTClientGame::m_vTOPTION.m_bShowCaution15 = !CTClientGame::m_vTOPTION.m_bShowCaution15;

				CTSettingFrame* pSettingFrame = static_cast<CTSettingFrame*>( m_vTFRAME[ TFRAME_SETTING ] );
				pSettingFrame->UpdateUI();

				CTGaugePannel* pTGAUGE = static_cast<CTGaugePannel*>( m_vTFRAME[ TFRAME_GAUGE ] );
				if( CTClientGame::m_vTOPTION.m_bShowCaution15 )
					pTGAUGE->AttachCaution15();
				else
					pTGAUGE->DettachCaution15();
			}
		}
		break;
	}
}

void CTClientGame::OnReleaseKEY( WORD wSCAN)
{
	DWORD dwKEY = CTKeySetting::GetInstance()
		->GetCurKeySet(wSCAN);

	for(DWORD i=0; i<TKEY_COUNT; ++i)
	{
		if( m_vKEY.m_vSTATE[i] &&
			CTKeySetting::GetInstance()->GetCurVKey((TKEY_SET)i) == wSCAN )
		{
			m_vKEY.m_vSTATE[i] = TKEY_STATE_NONE;
			m_vKEY.m_vTICK[i] = 0;
		}
	}

	switch(dwKEY)
	{
	case TKEY_ROT:
		m_bOrgTKEY_ROT = TKEY_STATE_NONE;
		break;

	case TKEY_CAM_ROT180:
		{
			m_vKEY.m_vSTATE[TKEY_CAM_ROT] = TKEY_STATE_NONE;
			
			TKEY_STATE key_state = TKEY_STATE_NONE;
			m_bOrgTKEY_ROT ? key_state = TKEY_STATE_PRESS : key_state = TKEY_STATE_NONE;
			m_vKEY.m_vSTATE[TKEY_ROT] = key_state;
			m_wCamDIR = m_pCtrlHost->m_wDIR;
		}
		break;
	}
}

void CTClientGame::OnRegionChanged()
{
	DWORD dwINFOSTR[] = 
	{
		0,						// TREGIONINFO_NONE
		TSTR_REGIONINFO_D,		// TREGIONINFO_D
		TSTR_REGIONINFO_C,		// TREGIONINFO_C
		TSTR_REGIONINFO_B,		// TREGIONINFO_B
		TSTR_REGIONINFO_BATTLE,	// TREGIONINFO_BATTLE
		TSTR_REGIONINFO_NORMAL,	// TREGIONINFO_NORMAL
		TSTR_REGIONINFO_PEACE,	// TREGIONINFO_PEACE
	};

	if(m_pMainChar->m_pTREGION)
	{
		SendCS_REGION_REQ(
			m_pMainChar->m_dwID,
			m_pMainChar->m_bType,
			m_pMainWnd->m_bChannel,
			m_vMAP.m_wMapID,
			m_pMainChar->m_pTREGION->m_dwID,
			m_pMainChar->m_pTREGION->m_wLocalID);

		CString strMSG = m_pMainChar->m_pTREGION->m_strNAME;
		if( m_pMainChar->m_pTREGION->m_bInfo )
		{
			CString strADD;
			strADD = CTChart::LoadString( (TSTRING) dwINFOSTR[m_pMainChar->m_pTREGION->m_bInfo]);
			
			strMSG += strADD;
		}
		((CTMainUI *) m_vTFRAME[TFRAME_MAIN])->ResetRegionMSG(strMSG);
		
		m_pTMinimap->m_pTITLE->m_strText = m_pMainChar->m_pTREGION->m_strNAME;
		m_pTRSCS->m_pTITLE->m_strText = m_pMainChar->m_pTREGION->m_strNAME;

		if( !m_pMainChar->m_pTREGION->m_bCanMail )
		{
			DisableUI(TFRAME_MAIL_RECV);
			DisableUI(TFRAME_MAIL_SEND);
		}

#ifdef ADD_SNOW
		if( m_pMainChar->m_pTREGION->m_dwID == 9729 || m_pMainChar->m_pTREGION->m_dwID == 10498 )
			StartSnowEvent();
		else
			EndSnowEvent();
#endif
	}

	m_dwActiveVolume = TBGM_VOLUME;
	m_bStopBGM = TRUE;
}

CTClientObjBase *CTClientGame::FindObject( DWORD dwID,
										   BYTE bType)
{
	switch(bType)
	{
	case OT_RECALL	: return FindRecall(dwID);
	case OT_SELF	: return FindFixRecall(dwID);
	case OT_MON		: return FindMonster(dwID);
	case OT_PC		: return FindPlayer(dwID);
	case OT_SWITCH	: return FindSwitch(dwID);
	case OT_GATE	: return FindGate(dwID);
	}

	return NULL;
}

CTClientObjBase *CTClientGame::FindOBJ( DWORD dwID,
									    BYTE bType)
{
	switch(bType)
	{
	case OT_RECALL	: return FindRecall(dwID);
	case OT_SELF	: return FindFixRecall(dwID);
	case OT_MON		: return FindMonster(dwID);
	case OT_PC		: return FindPC(dwID);
	case OT_SWITCH	: return FindSwitch(dwID);
	case OT_GATE	: return FindGate(dwID);
	}

	return NULL;
}

CTClientSwitch *CTClientGame::FindSwitch( DWORD dwID)
{
	MAPTSWITCH::iterator finder = m_mapTSwitch.find(dwID);

	if( finder != m_mapTSwitch.end() )
		return (*finder).second;

	return NULL;
}

CTClientGate *CTClientGame::FindGate( DWORD dwID)
{
	MAPTGATE::iterator finder = m_vMAP.m_mapTGate.find(dwID);

	if( finder != m_vMAP.m_mapTGate.end() )
		return (*finder).second;

	return NULL;
}

CTClientRecall *CTClientGame::FindSlaveRecall( DWORD dwID)
{
	MAPRECALL::iterator finder = m_mapSLAVERECALL.find(dwID);

	if( finder != m_mapSLAVERECALL.end() )
		return (*finder).second;

	return NULL;
}

CTClientRecall *CTClientGame::FindRecall( DWORD dwID)
{
	MAPRECALL::iterator finder = m_mapRECALL.find(dwID);

	if( finder != m_mapRECALL.end() )
		return (*finder).second;

	return NULL;
}

CTClientRecall *CTClientGame::FindSlaveFixRecall( DWORD dwID)
{
	MAPRECALL::iterator finder = m_mapSLAVEFIXRECALL.find(dwID);

	if( finder != m_mapSLAVEFIXRECALL.end() )
		return (*finder).second;

	return NULL;
}

CTClientRecall *CTClientGame::FindFixRecall( DWORD dwID)
{
	MAPRECALL::iterator finder = m_mapFIXRECALL.find(dwID);

	if( finder != m_mapFIXRECALL.end() )
		return (*finder).second;

	return NULL;
}

CTClientMonster *CTClientGame::FindMonster( DWORD dwID)
{
	MAPMONSTER::iterator finder = m_mapMONSTER.find(dwID);

	if( finder != m_mapMONSTER.end() )
		return (*finder).second;

	return NULL;
}

CTClientMonster *CTClientGame::FindSlave( DWORD dwID)
{
	MAPMONSTER::iterator finder = m_mapSLAVEMON.find(dwID);

	if( finder != m_mapSLAVEMON.end() )
		return (*finder).second;

	return NULL;
}

CTClientChar *CTClientGame::FindPC( DWORD dwID)
{
	if( dwID != m_pMainChar->m_dwID )
		return FindPlayer(dwID);

	return m_pMainChar;
}

CTClientChar *CTClientGame::FindPlayer( DWORD dwID)
{
	MAPPLAYER::iterator finder = m_mapPLAYER.find(dwID);

	if( finder != m_mapPLAYER.end() )
		return (*finder).second;

	return NULL;
}

LPTPARTY CTClientGame::FindParty( DWORD dwID)
{
	for( BYTE i=0; i<INT(m_vTPARTY.size()); i++)
		if( m_vTPARTY[i]->m_dwCharID == dwID )
			return m_vTPARTY[i];

	return NULL;
}

TEdit *CTClientGame::GetCurEdit()
{
	if( m_pMainWnd->m_MessageBox.GetParent() && m_pMainWnd->m_MessageBox.CanProcess() )
	{
		TFrame *pTFRAME = m_pMainWnd->m_MessageBox.GetCurrentFrame();

		if( pTFRAME && pTFRAME->CanProcess() )
		{
			TComponent *pTEDIT = pTFRAME->FindKid(ID_CTRLINST_EDIT);

			if( pTEDIT && pTEDIT->CanProcess() && pTEDIT->GetFocus() )
				return (TEdit *) pTEDIT;
		}
	}

	TEdit* pEdit = static_cast<CTPrivateShopDlgForSell*>(m_vTFRAME[TFRAME_PRIVATE_SELL])->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTTradeDlg*>(m_vTFRAME[TFRAME_TRADE])->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = CTMessengerChat::GetCurMsgChatEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND])->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTItemRegBox*>(m_vTFRAME[TFRAME_ITEM_REGBOX])->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTSMSDlg*>(m_vTFRAME[TFRAME_SMS])->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTGuildNotifyDlg*>(m_vTFRAME[TFRAME_GUILDNOTIFY])->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTGuildApplicationDlg*>(m_vTFRAME[TFRAME_GUILDAPP])->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTGuildPointRewardFrame*>( m_vTFRAME[ TFRAME_GUILDPOINTREWARD] )->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTUpItemSellDlg*>( m_vTFRAME[ TFRAME_WARN_UPITEM_SELL] )->GetCurEdit();
	if ( pEdit )
		return pEdit;

	pEdit = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] )->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast< CTAuctionBidOnMsgBox* >( m_vTFRAME[ TFRAME_BIDON ] )->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTGuildRegularOffer*>( m_vTFRAME[ TFRAME_GUILD_REGULAR_OFFER ] )->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTGuildTacticsOffer*>( m_vTFRAME[ TFRAME_GUILD_TACTICS_OFFER ] )->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTTacticsInviteDlg*>( m_vTFRAME[ TFRAME_INVITE_TACTICS ] )->GetCurEdit();
	if( pEdit )
		return pEdit;

	pEdit = static_cast<CTTournamentSelectTarget*>( m_vTFRAME[ TFRAME_TOURNAMENT_SELECT_TARGET ] )->GetCurEdit();
	if( pEdit )
		return pEdit;

#ifdef ADD_TEXTOOL
	pEdit = static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] )->GetCurEdit();
	if( pEdit )
		return pEdit;
#endif

	if( m_pChatFrame->IsChatEditON() )
		return m_pChatFrame->GetCurEdit();

	return NULL;
}

void CTClientGame::SayToAll( CPacket& packet)
{
	for( int i=0; i<MAX_CON; i++)
	{
		if( m_pMainWnd->m_session[i].m_bSVR == SVR_MAP &&
			m_pMainWnd->m_session[i].m_bLogicalValid )
		{
			CPacket copy;
			copy.Copy(&packet);
			m_pMainWnd->m_session[i].Say(&copy);
		}
	}
}

void CTClientGame::SayToSvr(CPacket& packet, BYTE bSvrID)
{
	for( int i=0; i<MAX_CON; i++)
		if( m_pMainWnd->m_session[i].m_bSvrID == bSvrID )
		{
			m_pMainWnd->m_session[i].m_bLogicalValid = TRUE;
			m_pMainWnd->m_session[i].Say(&packet);
			return;
		}
}

BYTE CTClientGame::RotateCamera( CPoint &point, CPoint &pt, BYTE bMOVE )
{
	point = m_vKEY.m_point;

	if( point.x <= 0 )
		point.x = 1;
	else if( point.x >= m_pMainWnd->m_Device.m_option.m_dwScreenX )
		point.x = m_pMainWnd->m_Device.m_option.m_dwScreenX-1;

	if( point.y <= 0 )
		point.y = 1;
	else if( point.y >= m_pMainWnd->m_Device.m_option.m_dwScreenY )
		point.y = m_pMainWnd->m_Device.m_option.m_dwScreenY-1;

	RotCameraX(pt.y - m_vKEY.m_point.y);
	RotCameraY(pt.x - m_vKEY.m_point.x);
	bMOVE = TRUE;	return bMOVE;
}

BYTE CTClientGame::RotateCharacter( CPoint &pt, CPoint &point, BYTE bMOVE )
{
	if( m_bCamFIX && !m_pMainChar->IsDead() && !m_pMainChar->IsBlock() && !m_pMainChar->IsDown() )
		m_pCtrlHost->m_nDIR = pt.x - m_vKEY.m_point.x;

	point = m_vKEY.m_point;

	if( point.x <= 0 )
		point.x = 1;
	else if( point.x >= m_pMainWnd->m_Device.m_option.m_dwScreenX )
		point.x = m_pMainWnd->m_Device.m_option.m_dwScreenX-1;

	if( point.y <= 0 )
		point.y = 1;
	else if( point.y >= m_pMainWnd->m_Device.m_option.m_dwScreenY )
		point.y = m_pMainWnd->m_Device.m_option.m_dwScreenY-1;

	RotCameraX(pt.y - m_vKEY.m_point.y);

	m_bMoveType = TMAINMOVE_NORMAL;
	m_dwMoveGM = 0;

	CTClientObjBase::m_mapTSELECTDEADOBJ.clear();
	CTClientObjBase::m_mapTSELECTOBJ.clear();

	bMOVE = TRUE;
	return bMOVE;
}


void CTClientGame::OnImeComposition( HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	if( !m_bUseKeyboardInput || m_vTFRAME[TFRAME_KEYSETTING]->IsVisible() )
		return;
	CFrameGroup::OnImeComposition( hWnd, wParam, lParam);

	TEdit *pTEDIT = GetCurEdit();
	if( pTEDIT == m_pChatFrame->GetChatEditCtrl() )
	{
		HIMC hImc = ImmGetContext(hWnd);

		CHAR szCompResultStr[1024];
		if( (lParam & GCS_RESULTSTR) == GCS_RESULTSTR )
		{
			INT nResultSize = ImmGetCompositionString( hImc, GCS_RESULTSTR, NULL, 0 );
			ImmGetCompositionString(hImc, GCS_RESULTSTR, szCompResultStr, nResultSize );
			szCompResultStr[ nResultSize ] = '\0';

			for( INT i=0 ; i < nResultSize ; ++i )
				if( szCompResultStr[ i ] == _T(' ') )
				{
					m_pChatFrame->CheckChatMode();
					break;
				}
		}
		ImmReleaseContext(hWnd, hImc);
	}
}

void CTClientGame::OnImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	CFrameGroup::OnImeNotify( hWnd, wParam, lParam);
}

BOOL CTClientGame::OnInputLangChangeRequest(HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	return CFrameGroup::OnInputLangChangeRequest( hWnd, wParam, lParam);
}

void CTClientGame::OnSetFocus( TComponent* pLoseFocus )
{
	CFrameGroup::OnSetFocus( pLoseFocus );
}

void CTClientGame::OnLoseFocus( TComponent* pSetFocus )
{
	m_vKEY.ResetSTATE();
	CFrameGroup::OnLoseFocus( pSetFocus );
}

void CTClientGame::OnChar( UINT nChar, int nRepCnt, UINT nFlags)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	if( m_vTFRAME[TFRAME_KEYSETTING]->IsVisible() )
		return;

	WORD wVKey = (WORD) MapVirtualKey(LOBYTE(nFlags),1);
	WORD wModKey = CTClientKEY::GetCurMOD();
	WORD wKEY = CTKeySetting::GetInstance()->GetCurKeySet(wVKey,wModKey);

	if(!m_bUseKeyboardInput)
	{
		if( m_vTFRAME[TFRAME_CINEMATIC]->IsVisible() && wKEY == TKEY_CLOSE_UI )
			OnGM_SKIP_SCENE();

		return;
	}
	TEdit *pTEDIT = GetCurEdit();

	if( pTEDIT == m_pChatFrame->GetWhisperTargetEditCtrl() )
	{
		switch( wKEY )
		{
		case TKEY_CHAT:
			m_pChatFrame->ProcWhisperTargetInput( FALSE );
			break;

		case TKEY_CLOSE_UI:
			m_pChatFrame->ProcWhisperTargetInput( TRUE );
			break;
		}
	}
	else if( wKEY == TKEY_CHAT )
	{
		BOOL bChat = TRUE;

		if( CTMessengerChat::GetCurMsgChatEdit() )
			bChat = FALSE;
		else if( static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND])->GetCurEdit() )
			bChat = FALSE;
		else if( static_cast<CTSMSDlg*>(m_vTFRAME[TFRAME_SMS])->GetCurEdit() )
			bChat = FALSE;
		else if( static_cast<CTGuildNotifyDlg*>(m_vTFRAME[TFRAME_GUILDNOTIFY])->GetCurEdit() )
			bChat = FALSE;
		else if( static_cast<CTGuildApplicationDlg*>(m_vTFRAME[TFRAME_GUILDAPP])->GetCurEdit() )
			bChat = FALSE;
		else if( static_cast<CTGuildPointRewardFrame*>( m_vTFRAME[ TFRAME_GUILDPOINTREWARD] )->GetCurEdit() )
			bChat = FALSE;
		else if ( static_cast<CTUpItemSellDlg*>( m_vTFRAME[ TFRAME_WARN_UPITEM_SELL ] )->GetCurEdit() )
			bChat = FALSE;
		else if( static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] )->GetCurEdit() )
			bChat = FALSE;
		else if( static_cast< CTAuctionBidOnMsgBox* >( m_vTFRAME[ TFRAME_BIDON ] )->GetCurEdit() )
			bChat = FALSE;
		else if( static_cast<CTGuildRegularOffer*>( m_vTFRAME[ TFRAME_GUILD_REGULAR_OFFER ] )->GetCurEdit() )
			bChat = FALSE;
		else if( static_cast<CTGuildTacticsOffer*>( m_vTFRAME[ TFRAME_GUILD_TACTICS_OFFER ] )->GetCurEdit() )
			bChat = FALSE;
		else if( static_cast<CTTournamentSelectTarget*>( m_vTFRAME[ TFRAME_TOURNAMENT_SELECT_TARGET ] )->GetCurEdit() )
			bChat = FALSE;
#ifdef ADD_TEXTOOL
		else if( static_cast<CTexTool*>( m_vTFRAME[ TFRAME_TEXTOOL ] )->OnEnterKey() )
			bChat = FALSE;
#endif
		if( bChat )
			DoCHAT();
	}

	if( pTEDIT == m_pChatFrame->GetChatEditCtrl() )
	{
		if( nChar == VK_SPACE )
			m_pChatFrame->CheckChatMode();
		else if( wKEY == TKEY_CLOSE_UI )
			m_pChatFrame->ResetChatMsg(FALSE);
	}

	CFrameGroup::OnChar( nChar, nRepCnt, nFlags);
}

void CTClientGame::OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags)
{
	CTKeySettingDlg *pKeyDlg = static_cast<CTKeySettingDlg *>(m_vTFRAME[TFRAME_KEYSETTING]);

	if(pKeyDlg->IsVisible())
	{
		if( CTNationOption::JAPAN_IME || CTNationOption::TAIWAN_IME )
		{
			HIMC hImc = ImmGetContext(m_pMainWnd->GetSafeHwnd());
			ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
			ImmReleaseContext( m_pMainWnd->GetSafeHwnd(), hImc);
		}

		if(pKeyDlg->IsWaitInput())
			pKeyDlg->OnKeyDown(nChar,nRepCnt,nFlags);

		return;
	}

	if(!m_bUseKeyboardInput)
	{
		if( CTNationOption::JAPAN_IME || CTNationOption::TAIWAN_IME )
		{
			HIMC hImc = ImmGetContext(m_pMainWnd->GetSafeHwnd());
			ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
			ImmReleaseContext( m_pMainWnd->GetSafeHwnd(), hImc);
		}

		if( m_vTFRAME[TFRAME_CINEMATIC]->IsVisible() && CTKeySetting::GetInstance()->GetCurKeySet( (WORD) MapVirtualKey( LOBYTE(nFlags), 1), CTClientKEY::GetCurMOD()) == TKEY_CLOSE_UI )
			OnGM_SKIP_SCENE();

		return;
	}
	TEdit *pTEDIT = GetCurEdit();

	m_bGroundATK = FALSE;
	OnGM_CLOSE_POPUP();

	if(!pTEDIT)
	{
		if( CTNationOption::JAPAN_IME || CTNationOption::TAIWAN_IME )
		{
			HIMC hImc = ImmGetContext(m_pMainWnd->GetSafeHwnd());
			ImmNotifyIME( hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0);
			ImmReleaseContext( m_pMainWnd->GetSafeHwnd(), hImc);
		}

		OnActivateKEY((WORD) MapVirtualKey( LOBYTE(nFlags), 1));
	}

	if(!CanProcess())
		return;

	TCOMP_LIST::iterator it = GetFirstKidsFinder();
	while(!EndOfKids(it))
	{
		TComponent *pKid = GetNextKid(it);

		if( pKid && pKid->GetFocus() )
			pKid->OnKeyDown( nChar, nRepCnt, nFlags);
	}
}

void CTClientGame::OnKeyUp( UINT nChar, int nRepCnt, UINT nFlags)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	CTKeySettingDlg* pKeyDlg = static_cast<CTKeySettingDlg*>(m_vTFRAME[TFRAME_KEYSETTING]);
	if( pKeyDlg->IsVisible() )
	{
		if( pKeyDlg->IsWaitInput() )
			pKeyDlg->OnKeyUp(nChar,nRepCnt,nFlags);

		return;
	}

	if( nChar == VK_SNAPSHOT )
		TCapture();

	TEdit *pTEDIT = GetCurEdit();

	if( !pTEDIT )
		OnReleaseKEY((WORD) MapVirtualKey(LOBYTE(nFlags),1));

	CFrameGroup::OnKeyUp( nChar, nRepCnt, nFlags);
}

BOOL CTClientGame::DoMouseWheel( UINT nFlags, short zDelta, CPoint pt)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	if(!m_bUseMouseWheel)
		return FALSE;

	BYTE bRESULT = CFrameGroup::DoMouseWheel( nFlags, zDelta, pt);
	if(!bRESULT)
		m_nCamZOOM -= zDelta;

	return bRESULT;
}

void CTClientGame::OnMouseMove( UINT nFlags, CPoint pt)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	if(!m_bUseMouseMove)
	{
		if(m_vTFRAME[TFRAME_CINEMATIC]->IsVisible())
			m_vTFRAME[TFRAME_CINEMATIC]->OnMouseMove( nFlags, pt);

		return;
	}

	CPoint point(
		m_pDevice->m_option.m_dwScreenX / 2,
		m_pDevice->m_option.m_dwScreenY / 2);

	BYTE bMOVE = FALSE;
	m_pCtrlHost = GetCtrlOBJ();

	CTGaugePannel *pTGaugePannel = static_cast<CTGaugePannel *>(m_vTFRAME[TFRAME_GAUGE]);
	pTGaugePannel->ShowHotkeyBase(m_vDRAG.m_pIMAGE || pTGaugePannel->CheckHotkeyBase(pt) ? TRUE : FALSE);

	if( m_vKEY.m_vSTATE[TKEY_CAM_ROT] && m_vKEY.m_vTICK[TKEY_CAM_ROT] + CLICK_DELAY < m_vKEY.m_dwTick )
		bMOVE = RotateCamera(point, pt, bMOVE);

	if( m_vKEY.m_vSTATE[TKEY_ROT] && m_vKEY.m_vTICK[TKEY_ROT] + CLICK_DELAY < m_vKEY.m_dwTick )
	{
		if( m_bMoveType == TMAINMOVE_NORMAL )
			bMOVE = RotateCharacter(pt, point, bMOVE);
		else
			bMOVE = RotateCamera(point, pt, bMOVE);
	}

	if(bMOVE)
	{
		pt = point;
		ClientToScreen( m_pMainWnd->GetSafeHwnd(), &point);
		SetCursorPos( point.x, point.y);
	}
	m_vKEY.m_point = pt;

	CFrameGroup::OnMouseMove( nFlags, pt);
	if(m_bLButtonDown && m_bNeedDrag)
	{
		LONG lX = m_ptCurLBDown.x - pt.x;
		LONG lY = m_ptCurLBDown.y - pt.y;
		LONG lMov2 = (lX*lX) + (lY*lY);
		if( lMov2 >= TDRAG_START_OFFSET2 )
		{
			OnBeginDrag(m_ptCurLBDown);
			m_bNeedDrag = FALSE;
		}
	}
	else if(m_vDRAG.m_pIMAGE)
	{
		OnDragOver(pt);
	}
}

void CTClientGame::OnMButtonDown( UINT nFlags, CPoint pt)
{
	if(!m_bUseMButtonDown)
		return;

#ifdef ADD_TEXTOOL
#else
	if( !m_pMainChar->IsDead() && !m_pMainChar->IsBlock() && !m_pMainChar->IsDown() && !m_pMainChar->m_bPrivateShop )
	{
		m_bMoveType = TMAINMOVE_NORMAL;
		m_bAutoRun = !m_bAutoRun;
		m_dwMoveGM = 0;
	}
#endif

	CFrameGroup::OnMButtonDown( nFlags, pt);
}

void CTClientGame::OnLButtonDown( UINT nFlags, CPoint pt)
{
	if(!m_bUseLButtonDown)
	{
		if(m_vTFRAME[TFRAME_CINEMATIC]->IsVisible())
			m_vTFRAME[TFRAME_CINEMATIC]->OnLButtonDown( nFlags, pt);

		return;
	}

	if(m_pTPOPUP->IsVisible())
	{
		if( m_pTPOPUP->HitTest(pt) )
		{
			CFrameGroup::OnLButtonDown( nFlags, pt);
			return;
		}

		OnGM_CLOSE_POPUP();
	}

	TEdit* pChatEdit = CTMessengerChat::GetCurMsgChatEdit();
	if( !pChatEdit && m_pChatFrame->IsChatEditON() )
	{
		pChatEdit = m_pChatFrame->GetChatEditCtrl();
		if( !pChatEdit->GetFocus() )
			pChatEdit = NULL;
	}

	if( !pChatEdit )
	{
		CTMailSendDlg* pSndMailDlg = static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND]);
		TEdit* pEdit = pSndMailDlg->GetCurEdit();

		if( pEdit == pSndMailDlg->GetMessageEdit() || pEdit == pSndMailDlg->GetTitleEdit() )
			pChatEdit = pEdit;
	}

	if( !pChatEdit )
		pChatEdit = static_cast<CTGuildNotifyDlg*>( m_vTFRAME[TFRAME_GUILDNOTIFY] )->GetCurEdit();

	if( !pChatEdit )
		pChatEdit = static_cast<CTGuildApplicationDlg*>(m_vTFRAME[TFRAME_GUILDAPP])->GetCurEdit();

	if( !pChatEdit )
		pChatEdit = static_cast< CTAuctionMainFrame* >( m_vTFRAME[TFRAME_AUCTION] )->GetCurEdit();

	if( !pChatEdit )
		pChatEdit = static_cast< CTAuctionBidOnMsgBox* >( m_vTFRAME[ TFRAME_BIDON ] )->GetCurEdit();
	
	if( !pChatEdit )
		pChatEdit = static_cast< CTGuildRegularOffer* >( m_vTFRAME[ TFRAME_GUILD_REGULAR_OFFER ] )->GetCurEditLink();

	if( !pChatEdit )
		pChatEdit = static_cast< CTGuildTacticsOffer* >( m_vTFRAME[ TFRAME_GUILD_TACTICS_OFFER ] )->GetCurEditLink();

    if( pChatEdit && (nFlags & MK_SHIFT) && m_pMainWnd->GetTMainFrame() == this )
	{
		TComponent* pHIT = GetTopFrame(pt,0);
		if( pHIT )
		{
			CTClientUIBase* pFrame = static_cast<CTClientUIBase*>(pHIT);
			TCHATINFO info;
			if( pFrame->GetTChatInfo(pt,info) )
			{
				CTTextLinker* pTLinker = CTTextLinker::GetInstance();
				if( info.m_Type == TTEXT_LINK_TYPE_QUEST )
					pTLinker->InsertLinkTextToEdit(pChatEdit, info.m_pQuest);
				else
					pTLinker->InsertLinkTextToEdit(pChatEdit, info.m_pItem, info.m_pClientItem);
			}
		}
	}
	else
	{
		BYTE bDRAG = FALSE;

		m_dwLButtonTick = m_vKEY.m_dwTick;
		m_bLButtonDown = TRUE;

		if(HitTest(pt))
		{
			if(m_pTRSCS->IsVisible())
				CheckTRSCS( nFlags, pt);

			m_bGroundATK = FALSE;
			bDRAG = TRUE;
		}
		else if(m_bGroundATK)
		{
			m_pCtrlHost = GetCtrlOBJ();

			UseTSkill( m_wSkillID, FALSE, FALSE);
			m_bGroundLOCK = TRUE;
		}
		else
			OnActivateKEY(VK_LBUTTON);

		m_vKEY.m_point = pt;
		
		CFrameGroup::OnLButtonDown( nFlags, pt);
		
		CTClientUIBase* pFrame = static_cast<CTClientUIBase*>( GetTopFrame(pt, 0) );
		m_bNeedDrag = ( bDRAG && !m_pMainChar->IsDead() && !m_pMainChar->m_bGhost &&
			!m_pMainChar->IsBlock() && !m_pMainChar->IsDown() ) &&
			pFrame && pFrame->OnBeginDrag(NULL, pt);

		m_ptCurLBDown = pt;
	}
}

void CTClientGame::OnLButtonDblClk( UINT nFlags, CPoint pt)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	if(!m_bUseLButtonDblClick)
		return;

	if( nFlags & MK_SHIFT )
		return;

	if( m_pMainChar->IsBlock() || m_pMainChar->IsDown() ||
		m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
		return;

	CFrameGroup::OnLButtonDblClk( nFlags, pt);
	BYTE bTFRAME = TFRAME_COUNT;

	for( BYTE i=0; i<TFRAME_COUNT; i++)
		if( m_vTFRAME[i] && m_vTFRAME[i]->HitTest(pt) )
		{
			bTFRAME = i;
			if( i == GetTopFrame() )
				break;
		}

	if( bTFRAME < TFRAME_COUNT )
	{
		switch(bTFRAME)
		{
		case TFRAME_DEFINVEN	:
		case TFRAME_INVEN_1		:
		case TFRAME_INVEN_2		:
		case TFRAME_INVEN_3		:
		case TFRAME_INVEN_4		:
		case TFRAME_INVEN_5		:
			{
				if( GetFrame( TFRAME_PRIVATE_SELL )->IsVisible() )
					break;

				CTClientInven *pTINVEN = m_pMainChar->FindTInven(GetInvenID(bTFRAME));

				if(pTINVEN)
				{
					CTClientItem *pTITEM = NULL;

					for( BYTE j=0; j<MAX_DRAGSLOT; j++)
					{
						TComponent *pTSLOT = m_vTFRAME[bTFRAME]->FindKid(m_vSlotID[j]);

						if( pTSLOT && pTSLOT->m_pDESC->m_vCOMP.m_bType == TCML_TYPE_IMAGELIST && pTSLOT->HitTest(pt) )
						{
							pTITEM = pTINVEN->FindTItem(j);
							break;
						}
					}

					if( pTITEM && m_pSESSION )
						if( pTITEM->GetWrap() )
						{
							m_pMainWnd->MessageBoxOK(
								(DWORD) TSTR_CANT_USE_WRAPED_ITEM,
								TSTR_OK,
								GM_CLOSE_MSGBOX,
								0,
								TRUE,
								ID_SND_ITEM_FAILED,
								MEDIA_TSOUND );

							return ;
						}
						else if(m_vTFRAME[TFRAME_ITEM_SHOP]->IsVisible())
						{
							m_bInvenID = pTINVEN->m_bInvenID;
							m_bSlotID = pTITEM->GetItemSlot();

							OnGM_ITEM_SELL();
							ResetInvenUI();
						}
						else if( pTITEM->GetTITEM()->m_bPrmSlotID != INVALID_SLOT )
						{
							if( m_vTFRAME[TFRAME_ITEM_UP]->IsVisible() ||
								m_vTFRAME[TFRAME_ITEM_REPAIR]->IsVisible() ||
								m_vTFRAME[TFRAME_ITEM_REFINE]->IsVisible() ||
								m_vTFRAME[TFRAME_ITEM_GAMBLE]->IsVisible())
							{
								return ;
							}

							m_bTargetInvenID = INVEN_NULL;
							m_bTargetSlotID = INVEN_NULL;

							VECTORBYTE bSLOTS = CTClientItem::GetEquipSlotList(pTITEM->GetTITEM());
							CTClientInven* pTEQUIP = m_pMainChar->m_mapTINVEN[INVEN_EQUIP];

							BYTE bPrmSlot = pTITEM->GetTITEM()->m_bPrmSlotID;
							for(INT idx=0; idx<INT(bSLOTS.size()); ++idx)
							{
								if( !pTEQUIP->FindTItem(bSLOTS[idx]) )
								{
									bPrmSlot = bSLOTS[idx];
									break;
								}
							}

							m_pSESSION->SendCS_MOVEITEM_REQ(
								pTINVEN->m_bInvenID,
								pTITEM->GetItemSlot(),
								INVEN_EQUIP,
								bPrmSlot,
								pTITEM->GetCount());
						}
						else if( pTITEM->GetTITEM()->m_bType == IT_USE )
							UseTItem( pTINVEN->m_bInvenID, pTITEM->GetItemSlot());
						else if( pTITEM->GetTITEM()->m_bType == IT_PET )
						{
							WORD wPetID = MAKEWORD(m_pMainChar->GetRaceID(), pTITEM->GetTITEM()->m_wUseValue);
							WORD wUseTime = pTITEM->GetTITEM()->m_wUseTime;
							BYTE bUseType = pTITEM->GetTITEM()->m_bUseType;
							
							LPTPET pPetTemp = CTChart::FindTPETTEMP( wPetID );
							if( !pPetTemp )
								return;
							
							CTPetMakeDlg* pMakeDlg = static_cast<CTPetMakeDlg*>(m_vTFRAME[TFRAME_PET_MAKE]);
							CTPetManageDlg* pMngDlg = static_cast<CTPetManageDlg*>(m_vTFRAME[TFRAME_PET_MANAGE]);
							
							CString strPetName;
							CString strBtnTxt;

							INT nPetID = pMngDlg->FindPetByID( wPetID );
							if( nPetID == T_INVALID )
								strBtnTxt = CTChart::LoadString( TSTR_PETMAKE_BTN_MAKE);
							else
							{
								CTClientPet* pPet = pMngDlg->GetPet(nPetID);
								strPetName = pPet->GetPetName();
								strBtnTxt = CTChart::LoadString( TSTR_PETMAKE_BTN_EXT);
							}

							if( (bUseType&DURINGTYPE_TIME) == DURINGTYPE_TIME )
							{
								pMakeDlg->m_strEndTime = CTChart::Format( TSTR_FMT_PET_USETIME_HOUR, wUseTime );
							}
							else if( (bUseType&DURINGTYPE_DAY) == DURINGTYPE_DAY )
							{
								pMakeDlg->m_strEndTime = CTChart::Format( TSTR_FMT_PET_USETIME, wUseTime );
							}

							pMakeDlg->GetMakeButton()->m_strText = strBtnTxt;
							pMakeDlg->SetDisplayPet(strPetName, wPetID, CTime(0));
							pMakeDlg->SetPetItem(pTINVEN->m_bInvenID, pTITEM->GetItemSlot()), 

							EnableUI(TFRAME_PET_MAKE);
						}
						else if( pTITEM->GetTITEM()->m_bType == IT_REFINE )
						{
							m_pMainWnd->MessageBoxYesNo(
								TSTR_WARN_CHANGE_REFINE,
								TSTR_YES,
								TSTR_NO,
								TCOMMAND( GM_CHANGE_REFINE, MAKELONG( GetInvenID(bTFRAME), j ) ),
								GM_CLOSE_MSGBOX,
								0,
								FALSE );
						}
						else if( pTITEM->GetTITEM()->m_bType == IT_INVEN )
						{
							// 인벤토리에 있는 가방을 가방슬롯에 꼽는 경우.
							CTCharInfoDlg* pChatInfoDlg = static_cast<CTCharInfoDlg*>(m_vTFRAME[TFRAME_CHAR_INFO]);

							for( BYTE nSLOT=0 ; nSLOT < MAX_SUBINVEN ; ++nSLOT )
							{
								if(pChatInfoDlg->m_pTSUBINVEN[nSLOT]->GetCurImage() == 0) // 빈 인벤토리 슬롯을 찾는다.
								{
									BYTE FromInven = pTINVEN->m_bInvenID;
									BYTE FromSlot = pTITEM->GetItemSlot();

									m_pSESSION->SendCS_INVENADD_REQ(
										CTMainUI::ToSlotNumber( CTMainUI::MakeSlotID(TMAINSLOTID_INVEN, nSLOT) ),
										FromInven,
										FromSlot);

									break;
								}
							}
						}
				}
			}

			break;

		case TFRAME_CHAR_INFO	:
			{
				CTClientInven *pTEQUIP = m_pMainChar->FindTInven(INVEN_EQUIP);
				CTClientItem *pTITEM = NULL;
				BYTE bReset = TRUE;

				// 장착아이템 검사
				for( BYTE j=0; j<MAX_DRAGSLOT; j++)
				{
					TComponent *pTSLOT = m_vTFRAME[bTFRAME]->FindKid(m_vSlotID[j]);

					if( pTSLOT && pTSLOT->m_pDESC->m_vCOMP.m_bType == TCML_TYPE_IMAGELIST && pTSLOT->HitTest(pt) )
					{
						pTITEM = pTEQUIP->FindTItem(j);
						break;
					}
				}

				if( pTITEM && m_pSESSION )
				{
					WORD wInvenPos = GetEaseInvenPos(pTITEM);

					if( LOBYTE(wInvenPos) == INVALID_SLOT )
						wInvenPos = GetBlankInvenPos();

					if( LOBYTE(wInvenPos) != INVALID_SLOT )
					{
						m_pSESSION->SendCS_MOVEITEM_REQ(
							pTEQUIP->m_bInvenID,
							pTITEM->GetItemSlot(),
							HIBYTE(wInvenPos),
							LOBYTE(wInvenPos),
							pTITEM->GetCount());
						bReset = FALSE;
					}
					else
						ItemMSG(MI_INVENFULL);
				}
				else
				{
					// 가방 검사

					CTCharInfoDlg* pChatInfoDlg = static_cast<CTCharInfoDlg*>(m_vTFRAME[TFRAME_CHAR_INFO]);

					for( BYTE n=0 ; n < MAX_SUBINVEN ; ++n )
					{
						if( pChatInfoDlg->m_pTSUBINVEN[n]->HitTest(pt) &&
							pChatInfoDlg->m_pTSUBINVEN[n]->GetCurImage() != 0)
						{
							WORD wInvenPos = GetBlankInvenPosExceptInven( n ); // 자신을 제외한 가방의 빈칸을 찾아야 한다.

							BYTE ToInven = HIBYTE(wInvenPos); // 인벤
							BYTE ToSlot = LOBYTE(wInvenPos); // 슬롯

							if( ToSlot != INVALID_SLOT &&
								ToInven != n )
							{
								m_pSESSION->SendCS_INVENDEL_REQ(
									CTMainUI::ToSlotNumber( CTMainUI::MakeSlotID(TMAINSLOTID_INVEN, n) ),
									ToInven,
									ToSlot);

								bReset = FALSE;
							}
							else
								ItemMSG(MI_INVENFULL);
						}
					}
				}

				if(bReset)
				{
					ResetCharInfoUI();
					ResetInvenUI();
				}
			}

			break;

		case TFRAME_MONINVEN	:
			{
				CTClientItem *pTITEM = NULL;

				for( BYTE j=0; j<MAX_DRAGSLOT; j++)
				{
					TComponent *pTSLOT = m_vTFRAME[bTFRAME]->FindKid(m_vSlotID[j]);

					if( pTSLOT && pTSLOT->m_pDESC->m_vCOMP.m_bType == TCML_TYPE_IMAGELIST && pTSLOT->HitTest(pt) )
					{
						pTITEM = m_vTMONINVEN.FindTItem(j);
						break;
					}
				}

				if( pTITEM && m_pSESSION )
				{
					WORD wInvenPos = GetEaseInvenPos(pTITEM);

					if( LOBYTE(wInvenPos) == INVALID_SLOT )
						wInvenPos = GetBlankInvenPos();

					if( LOBYTE(wInvenPos) != INVALID_SLOT )
					{
						m_bInvenID = HIBYTE(wInvenPos);
						m_bSlotID = LOBYTE(wInvenPos);
						m_bMonSlotID = pTITEM->GetItemSlot();
						OnGM_ITEM_TAKE();
					}
					else
						ItemMSG(MI_INVENFULL);
				}
			}

			break;

		case TFRAME_ITEM_SHOP	:
			{
				CTShopDlg* pShopDlg = static_cast<CTShopDlg*>(m_vTFRAME[TFRAME_ITEM_SHOP]);
				if( pShopDlg->GetList()->GetHitItem(pt) )
					OnGM_ITEM_BUY();
			}

			break;

		case TFRAME_PORTAL_SHOP:
			{
				CTShopDlg* pShopDlg = static_cast<CTShopDlg*>(m_vTFRAME[TFRAME_PORTAL_SHOP]);
				if( pShopDlg->GetList()->GetHitItem(pt) )
					OnGM_PORTAL_REQ();
			}

			break;

		case TFRAME_PORTAL_SHOP_NEW:
			{
				CTPortalShopDlg* pPortalDlg = static_cast<CTPortalShopDlg*>(m_vTFRAME[TFRAME_PORTAL_SHOP_NEW]);
				if( pPortalDlg->GetList()->GetHitItem(pt) )
					OnGM_PORTAL_REQ();
			}

			break;

		case TFRAME_MONSHOP :
			{
				CTShopDlg* pShopDlg = static_cast<CTShopDlg*>(m_vTFRAME[TFRAME_MONSHOP]);
				if( pShopDlg->GetList()->GetHitItem(pt) )
					OnGM_MONSTER_BUY();
			}
			break;
		}
	}
	else if(m_pHIT && m_vDEADMONHITS.empty())
	{
		ResetHitOBJ(pt, TRUE);
		if( !m_pHIT || m_pMainChar->m_bFlash )
			return;

		CTClientObjBase::m_mapTSELECTOBJ.clear();
		CTClientObjBase::m_mapTSELECTDEADOBJ.clear();

		ResetTargetOBJ(m_pHIT);
		ResetTargetINFO(FALSE);
		
		DoDEFACTION();
	}
}

void CTClientGame::OnRButtonDown( UINT nFlags, CPoint pt)
{
	if(!m_bUseRButtonDown)
		return;

	m_dwRButtonTick = m_vKEY.m_dwTick;
	m_bRButtonDown = TRUE;
	m_bGroundATK = FALSE;

	if(!HitTest(pt))
		OnActivateKEY(VK_RBUTTON);
	
	m_vKEY.m_point = pt;
	OnGM_CLOSE_POPUP();

	CFrameGroup::OnRButtonDown( nFlags, pt);
}

void CTClientGame::OnLButtonUp( UINT nFlags, CPoint pt)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	if(!m_bUseLButtonDown)
	{
		if(m_vTFRAME[TFRAME_CINEMATIC]->IsVisible())
			m_vTFRAME[TFRAME_CINEMATIC]->OnLButtonUp( nFlags, pt);

		return;
	}

	if(m_pTPOPUP->IsVisible())
	{
		CFrameGroup::OnLButtonUp( nFlags, pt);
		OnGM_CLOSE_POPUP();
	}
	else
	{
		if( m_bLButtonDown && !m_bGroundATK &&
			m_dwLButtonTick + CLICK_DELAY > m_vKEY.m_dwTick )
			OnClick( nFlags, pt);

		m_bLButtonDown = FALSE;
		m_dwLButtonTick = 0;
		m_bGroundATK = FALSE;

		OnReleaseKEY(VK_LBUTTON);
		CheckMainUI();

		CFrameGroup::OnLButtonUp( nFlags, pt);

		if(m_vDRAG.m_pIMAGE)
			OnDrop( nFlags, pt);
	}
}

void CTClientGame::OnRButtonUp( UINT nFlags, CPoint pt)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;
	OnGM_CLOSE_POPUP();

	if( m_bRButtonDown && m_dwRButtonTick + CLICK_DELAY > m_vKEY.m_dwTick )
		OnRClick( nFlags, pt);

	m_bRButtonDown = FALSE;
	m_dwRButtonTick = 0;
	OnReleaseKEY(VK_RBUTTON);

	CFrameGroup::OnRButtonUp( nFlags, pt);
}

void CTClientGame::OnClick( UINT nFlags, CPoint pt)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	if(!m_bUseLButtonClick)
		return;
	WORD wSquadID = GetPartyID(m_pMainChar);

	if(HitTest(pt))
	{
		BYTE bTopFrame = GetTopFrame();
		BOOL bHittedTopFrame = (bTopFrame == (BYTE)T_INVALID) ? FALSE : m_vTFRAME[ bTopFrame ]->HitRect( pt );
		BYTE bPannel = PannelHitTest(pt);

		if( !bHittedTopFrame && bPannel != 0xFF )
			if( wSquadID && m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID && (nFlags & MK_SHIFT) )
			{
				BYTE bToALL = nFlags & MK_CONTROL ? TRUE : FALSE;

				switch(bPannel)
				{
				case TPANNEL_PLAYER	: ExecTCMDFromTOBJ( m_pMainChar, bToALL); break;
				case TPANNEL_PARTY1	: ExecTCMDFromParty( 0, bToALL); break;
				case TPANNEL_PARTY2	: ExecTCMDFromParty( 1, bToALL); break;
				case TPANNEL_PARTY3	: ExecTCMDFromParty( 2, bToALL); break;
				case TPANNEL_PARTY4	: ExecTCMDFromParty( 3, bToALL); break;
				case TPANNEL_PARTY5	: ExecTCMDFromParty( 4, bToALL); break;
				case TPANNEL_PARTY6	: ExecTCMDFromParty( 5, bToALL); break;
				case TPANNEL_TARGET	: ExecTCMDFromTOBJ( m_pTARGET, bToALL); break;
				}
			}
			else
			{
				CTClientObjBase::m_mapTSELECTOBJ.clear();
				CTClientObjBase::m_mapTSELECTDEADOBJ.clear();

				switch(bPannel)
				{
				case TPANNEL_SUMMON	: ResetTargetOBJ(GetMainRecall()); break;
				case TPANNEL_PLAYER	: ResetTargetOBJ(m_pMainChar); break;
				case TPANNEL_PARTY1	: SelectParty(0); break;
				case TPANNEL_PARTY2	: SelectParty(1); break;
				case TPANNEL_PARTY3	: SelectParty(2); break;
				case TPANNEL_PARTY4	: SelectParty(3); break;
				case TPANNEL_PARTY5	: SelectParty(4); break;
				case TPANNEL_PARTY6	: SelectParty(5); break;
				}

				ResetTargetINFO(FALSE);

				if( m_pTARGET && m_pTARGET->m_bType == OT_PC && CTClientKEY::IsKeyDown(VK_MENU) )
					m_pSESSION->SendCS_GETTARGET_REQ(m_pTARGET->m_dwID);
			}
	}
	else if(m_pHIT)
	{
		ResetHitOBJ(pt, TRUE);
		if( !m_pHIT || m_pMainChar->m_bFlash )
			return;

		if( m_vDEADMONHITS.empty() )
		{
			if( wSquadID && m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID && (nFlags & MK_SHIFT) )
				ExecTCMDFromTOBJ( m_pHIT, (nFlags & MK_CONTROL) ? TRUE : FALSE);
			else
			{
				CTClientObjBase::m_mapTSELECTOBJ.clear();
				CTClientObjBase::m_mapTSELECTDEADOBJ.clear();
				ResetTargetOBJ(m_pHIT);
				ResetTargetINFO(FALSE);

				if( m_pTARGET && m_pTARGET->m_bType == OT_PC && CTClientKEY::IsKeyDown(VK_MENU) )
					m_pSESSION->SendCS_GETTARGET_REQ(m_pTARGET->m_dwID);
			}
		}
		else if( m_nDEADMONHITIDX >= 0 )
		{
			CTClientObjBase *pTARGET = NextHitDEADMON();

			if( pTARGET == m_pTARGET )
				pTARGET = NextHitDEADMON();

			ResetTargetOBJ(pTARGET); 
			ResetTargetINFO(FALSE);
		}
	}
	else if( !m_pMainChar->IsDead() && !m_pMainChar->IsBlock() && !m_pMainChar->IsDown() && !m_pMainChar->m_bPrivateShop )
		if( wSquadID && m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID && (nFlags & MK_SHIFT) )
			ExecTCMDFromPoint( pt.x, pt.y, (nFlags & MK_CONTROL) ? TRUE : FALSE);
		else if(!(nFlags & MK_CONTROL) && CTClientGame::m_vTOPTION.m_bMouseClickMove && !m_bMainCharMoveLock )
		{
			m_bAutoRun = FALSE;

			if(CanLookAt())
				MoveMainChar(pt);
		}
}

void CTClientGame::OnRClick( UINT nFlags, CPoint pt)
{
	m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;

	if(!m_bUseRButtonClick)
		return;
	WORD wSquadID = GetPartyID(m_pMainChar);

	CTClientObjBase::m_mapTSELECTOBJ.clear();
	CTClientObjBase::m_mapTSELECTDEADOBJ.clear();

	if(HitTest(pt))
	{
		BYTE bTopFrame = GetTopFrame();
		BOOL bHittedTopFrame = (bTopFrame == (BYTE)T_INVALID) ? FALSE : m_vTFRAME[ bTopFrame ]->HitRect( pt );
		BYTE bPannel = PannelHitTest(pt);

		if( bHittedTopFrame )
			return ;
		else if( bPannel != 0xFF )
		{
			if( bPannel != TPANNEL_TARGET )
			{
				ResetPartyOBJ(NULL);

				switch(bPannel)
				{
				case TPANNEL_SUMMON	: ResetTargetOBJ(GetMainRecall()); break;
				case TPANNEL_PLAYER	: ResetTargetOBJ(m_pMainChar); break;
				case TPANNEL_PARTY1	: SelectParty(0); break;
				case TPANNEL_PARTY2	: SelectParty(1); break;
				case TPANNEL_PARTY3	: SelectParty(2); break;
				case TPANNEL_PARTY4	: SelectParty(3); break;
				case TPANNEL_PARTY5	: SelectParty(4); break;
				case TPANNEL_PARTY6	: SelectParty(5); break;
				}

				ResetTargetINFO(FALSE);
			}

			if( m_pTARGET )
				DoPOPUP(m_pTARGET);
			else if( m_pTPARTY )
				DoPOPUP(m_pTPARTY);

			return;
		}

		if( m_pTRSCS->IsVisible() && m_pTRSCS->m_pTSQUAD->HitItemArea(pt) )
		{
			DoPOPUP(m_pTRSCS->m_pTCURSQUAD);
			return;
		}

#ifdef MODIFY_CNP_NAME
		TEdit* pEDIT = GetCurEdit();
		if( pEDIT && pEDIT->HitTest(pt) )
		{
			DoEditPOPUP( pEDIT, pt );
			return ;
		}
#endif
	}

	ITDetailInfoPtr pInfo = m_vTMAINTAIN[TPANNEL_PLAYER]->GetTInfoKey(pt);
	if( pInfo )
	{
		CTSkillDetInfoPtr pSkillInfo = pInfo.staticCast<CTSkillDetInfo>();
		LPTSKILL pTSKILL = pSkillInfo->GetSkillTemp();
		if( pTSKILL && pTSKILL->m_bCanCancel )
		{
			DoPOPUP(pTSKILL);
			return;
		}
	}

	TListItem* pListItem = m_pChatFrame->HitTestChatList( pt );
	if( pListItem && pListItem->m_param != m_pMainChar->m_dwID)
	{
		CTClientChar* pChar = FindPlayer( pListItem->m_param );

		if( pChar )
		{
			ResetTargetOBJ( pChar );
			ResetTargetINFO( FALSE );
			DoPOPUP( pChar );
		}
		else
		{
			CTChatListItem* pChatListItem = static_cast<CTChatListItem*>( pListItem );
			DoPOPUP( pChatListItem->m_strName );
		}

		return ;
	}

	if( m_pMainChar->m_bFlash )
		return ;

	if(m_pHIT)
	{
		ResetTargetOBJ(m_pHIT);
		ResetTargetINFO(FALSE);
		
		switch( m_pHIT->m_bType )
		{
		case OT_GODTOWER	:
		case OT_GODBALL		:
			{
				SiegeMgr->OnRClick();
			}
			break;

		default:
			DoPOPUP(m_pTARGET);
			break;
		}
	}
	else if( !m_pMainChar->IsDead() && !m_pMainChar->IsBlock() &&
		!m_pMainChar->IsDown() && !m_pMainChar->m_bPrivateShop &&
		wSquadID && m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID && (nFlags & MK_SHIFT) )
		ExecTCMD( TRSCSCMD_NONE, 0, OT_NONE, 0, 0, 0xFFFF, (nFlags & MK_CONTROL) ? TRUE : FALSE);
	else
	{
		ResetTargetOBJ(m_pHIT);
		ResetTargetINFO(FALSE);
	}
}

BYTE CTClientGame::PannelHitTest( CPoint point)
{
	for( BYTE i=0; i<TPANNEL_COUNT; i++)
		if(m_vTPANNEL[i]->HitTest(point))
			return i;

	for( i=0; i<TFACEIMG_COUNT; i++)
		if(m_vTFACEIMG[i]->HitTest(point))
			switch(i)
			{
			case TFACEIMG_PLAYER		: return TPANNEL_PLAYER;
			case TFACEIMG_SUMMON_S		:
			case TFACEIMG_SUMMON_N		: return TPANNEL_SUMMON;
			case TFACEIMG_TARGETMON_S	:
			case TFACEIMG_TARGETMON_N	:
			case TFACEIMG_TARGETPC		: return TPANNEL_TARGET;
			case TFACEIMG_PARTY1		: return TPANNEL_PARTY1;
			case TFACEIMG_PARTY2		: return TPANNEL_PARTY2;
			case TFACEIMG_PARTY3		: return TPANNEL_PARTY3;
			case TFACEIMG_PARTY4		: return TPANNEL_PARTY4;
			case TFACEIMG_PARTY5		: return TPANNEL_PARTY5;
			case TFACEIMG_PARTY6		: return TPANNEL_PARTY6;
			}

	return 0xFF;
}

const D3DMATERIAL9& CTClientGame::GetMaterial( CTClientObjBase *pTOBJ)
{
	static D3DMATERIAL9 vMAT = {
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		{ 0.0f, 0.0f, 0.0f, 0.0f},
		0.0f};

	if(pTOBJ)
	{
		FLOAT fValue = FLOAT(pTOBJ->m_dwSHADOWTick) / FLOAT(TMAXSHADOW_TICK);

		switch(pTOBJ->m_bType)
		{
		case OT_PC		:
		case OT_MON		:
		case OT_NPC		:
		case OT_RECALL	:
		case OT_SELF	:
			{
				vMAT.Ambient.r = m_vShadow.Ambient.r + fValue * (m_vNormal.Ambient.r - m_vShadow.Ambient.r);
				vMAT.Ambient.g = m_vShadow.Ambient.g + fValue * (m_vNormal.Ambient.g - m_vShadow.Ambient.g);
				vMAT.Ambient.b = m_vShadow.Ambient.b + fValue * (m_vNormal.Ambient.b - m_vShadow.Ambient.b);
				vMAT.Ambient.a = m_vShadow.Ambient.a + fValue * (m_vNormal.Ambient.a - m_vShadow.Ambient.a);

				vMAT.Diffuse.r = m_vShadow.Diffuse.r + fValue * (m_vNormal.Diffuse.r - m_vShadow.Diffuse.r);
				vMAT.Diffuse.g = m_vShadow.Diffuse.g + fValue * (m_vNormal.Diffuse.g - m_vShadow.Diffuse.g);
				vMAT.Diffuse.b = m_vShadow.Diffuse.b + fValue * (m_vNormal.Diffuse.b - m_vShadow.Diffuse.b);
				vMAT.Diffuse.a = FLOAT(pTOBJ->m_bAlpha) / FLOAT(ALPHA_MAX);
			}

			break;

		default			:
			{
				vMAT.Ambient.r = m_vOBJMTRL.Ambient.r;
				vMAT.Ambient.g = m_vOBJMTRL.Ambient.g;
				vMAT.Ambient.b = m_vOBJMTRL.Ambient.b;
				vMAT.Ambient.a = m_vOBJMTRL.Ambient.a;

				vMAT.Diffuse.r = m_vOBJMTRL.Diffuse.r;
				vMAT.Diffuse.g = m_vOBJMTRL.Diffuse.g;
				vMAT.Diffuse.b = m_vOBJMTRL.Diffuse.b;
				vMAT.Diffuse.a = m_vOBJMTRL.Diffuse.a;
			}

			break;
		}
	}
	else
	{
		vMAT.Ambient.r = m_vNormal.Ambient.r;
		vMAT.Ambient.g = m_vNormal.Ambient.g;
		vMAT.Ambient.b = m_vNormal.Ambient.b;
		vMAT.Ambient.a = m_vNormal.Ambient.a;

		vMAT.Diffuse.r = m_vNormal.Diffuse.r;
		vMAT.Diffuse.g = m_vNormal.Diffuse.g;
		vMAT.Diffuse.b = m_vNormal.Diffuse.b;
		vMAT.Diffuse.a = m_vNormal.Diffuse.a;
	}

	return vMAT;
}

void CTClientGame::SetMaterial( const D3DMATERIAL9& vMAT)
{
	if(m_pDevice->m_bEnableSHADER)
	{
		m_pDevice->m_pDevice->SetVertexShaderConstantF(
			m_pDevice->m_vConstantVS[VC_MTRLAMBIENT],
			(FLOAT *) &D3DXVECTOR4(
			vMAT.Ambient.r,
			vMAT.Ambient.g,
			vMAT.Ambient.b,
			vMAT.Ambient.a), 1);

		m_pDevice->m_pDevice->SetVertexShaderConstantF(
			m_pDevice->m_vConstantVS[VC_MTRLDIFFUSE],
			(FLOAT *) &D3DXVECTOR4(
			vMAT.Diffuse.r,
			vMAT.Diffuse.g,
			vMAT.Diffuse.b,
			vMAT.Diffuse.a), 1);
	}

	m_pDevice->m_pDevice->SetMaterial(&vMAT);
}

void CTClientGame::SetMaterial( CTClientObjBase *pTOBJ)
{
	SetMaterial( GetMaterial(pTOBJ) );
}

void CTClientGame::SetAmbient( BYTE bRValue,
							   BYTE bGValue,
							   BYTE bBValue)
{
	m_pDevice->m_pDevice->SetRenderState(
		D3DRS_AMBIENT,
		D3DCOLOR_ARGB(0xFF, bRValue,bGValue,bBValue));

	if(!m_pDevice->m_bEnableSHADER)
		return;

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_AMBIENT],
		(FLOAT *) &D3DXVECTOR4(
		FLOAT(bRValue) / 255.0f,
		FLOAT(bGValue) / 255.0f,
		FLOAT(bBValue) / 255.0f,
		1.0f), 1);
}

void CTClientGame::EnableLIGHT( CTClientBSP *pTBSP)
{
	MAPD3DLIGHT::iterator itLIGHT;
	VTLIGHT vTLIGHT;

	pTBSP->ResetTLIGHT();
	vTLIGHT.clear();

	for( itLIGHT = pTBSP->m_pMAP->m_mapLIGHT.begin(); itLIGHT != pTBSP->m_pMAP->m_mapLIGHT.end(); itLIGHT++)
	{
		BYTE bPUSH = TRUE;

		if( (*itLIGHT).second->m_Light.Type == D3DLIGHT_POINT )
		{
			DWORD dwNodeID = pTBSP->GetNodeID(D3DXVECTOR3(
				(*itLIGHT).second->m_Light.Position.x,
				(*itLIGHT).second->m_Light.Position.y,
				(*itLIGHT).second->m_Light.Position.z));

			if( dwNodeID != NODEID_NULL && pTBSP->m_mapPVS.find(dwNodeID) == pTBSP->m_mapPVS.end() )
				bPUSH = FALSE;
			else
				CTachyonMesh::m_bSoftwareVP = TRUE;
		}

		if(bPUSH)
		{
			(*itLIGHT).second->EnableLight(TRUE);
			vTLIGHT.push_back((*itLIGHT).second);
		}
	}

	if(!m_pDevice->m_bEnableSHADER)
	{
		vTLIGHT.clear();
		return;
	}

	int vLightCount[4] = { INT(vTLIGHT.size()), 0, 1, 0};
	FLOAT vCONST[1024];
	FLOAT *pCONST = vCONST;

	m_pDevice->m_pDevice->SetVertexShaderConstantI(
		m_pDevice->m_vConstantVS[VC_LIGHTCOUNT],
		vLightCount, 1);

	for( int i=0; i<vLightCount[0]; i++)
	{
		memcpy( pCONST, vTLIGHT[i]->m_Light.Type == D3DLIGHT_POINT ?
			&D3DXVECTOR4(
			1.0f / (vTLIGHT[i]->m_Light.Range * TLIGHT_SCALE * vTLIGHT[i]->m_Light.Range * TLIGHT_SCALE),
			0.0f,
			0.0f,
			0.0f) : &D3DXVECTOR4(
			vTLIGHT[i]->m_Light.Ambient.r,
			vTLIGHT[i]->m_Light.Ambient.g,
			vTLIGHT[i]->m_Light.Ambient.b,
			vTLIGHT[i]->m_Light.Ambient.a),
			4 * sizeof(FLOAT));
		pCONST += 4;
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTAMBIENT],
		vCONST, vLightCount[0]);
	pCONST = vCONST;

	for( i=0; i<vLightCount[0]; i++)
	{
		memcpy( pCONST, &D3DXVECTOR4(
			vTLIGHT[i]->m_Light.Diffuse.r,
			vTLIGHT[i]->m_Light.Diffuse.g,
			vTLIGHT[i]->m_Light.Diffuse.b,
			vTLIGHT[i]->m_Light.Diffuse.a),
			4 * sizeof(FLOAT));
		pCONST += 4;
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIFFUSE],
		vCONST, vLightCount[0]);
	pCONST = vCONST;

	for( i=0; i<vLightCount[0]; i++)
	{
		memcpy( pCONST, vTLIGHT[i]->m_Light.Type == D3DLIGHT_POINT ?
			&D3DXVECTOR4(
			vTLIGHT[i]->m_Light.Position.x,
			vTLIGHT[i]->m_Light.Position.y,
			vTLIGHT[i]->m_Light.Position.z,
			-1.0f) : &D3DXVECTOR4(
			vTLIGHT[i]->m_Light.Direction.x,
			vTLIGHT[i]->m_Light.Direction.y,
			vTLIGHT[i]->m_Light.Direction.z,
			1.0f),
			4 * sizeof(FLOAT));
		pCONST += 4;
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIR],
		vCONST, vLightCount[0]);
	vTLIGHT.clear();
}

void CTClientGame::EnableLIGHT()
{
	static int vLightCount[4] = { TLIGHT_COUNT, 0, 1, 0};

	for( int i=0; i<TLIGHT_COUNT; i++)
		m_vLIGHT[i].EnableLight();

	if(!m_pDevice->m_bEnableSHADER)
		return;

	FLOAT vCONST[16];
	FLOAT *pCONST = vCONST;

	m_pDevice->m_pDevice->SetVertexShaderConstantI(
		m_pDevice->m_vConstantVS[VC_LIGHTCOUNT],
		vLightCount, 1);

	for( i=0; i<TLIGHT_COUNT; i++)
	{
		memcpy( &vCONST[i * 4], &D3DXVECTOR4(
			m_vLIGHT[i].m_Light.Ambient.r,
			m_vLIGHT[i].m_Light.Ambient.g,
			m_vLIGHT[i].m_Light.Ambient.b,
			m_vLIGHT[i].m_Light.Ambient.a),
			4 * sizeof(FLOAT));
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTAMBIENT],
		vCONST, TLIGHT_COUNT);

	for( i=0; i<TLIGHT_COUNT; i++)
	{
		memcpy( &vCONST[i * 4], &D3DXVECTOR4(
			m_vLIGHT[i].m_Light.Diffuse.r,
			m_vLIGHT[i].m_Light.Diffuse.g,
			m_vLIGHT[i].m_Light.Diffuse.b,
			m_vLIGHT[i].m_Light.Diffuse.a),
			4 * sizeof(FLOAT));
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIFFUSE],
		vCONST, TLIGHT_COUNT);

	for( i=0; i<TLIGHT_COUNT; i++)
	{
		memcpy( &vCONST[i * 4], &D3DXVECTOR4(
			m_vLIGHT[i].m_Light.Direction.x,
			m_vLIGHT[i].m_Light.Direction.y,
			m_vLIGHT[i].m_Light.Direction.z,
			1.0f),
			4 * sizeof(FLOAT));
	}

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_LIGHTDIR],
		vCONST, TLIGHT_COUNT);
}

void CTClientGame::DisableLIGHT()
{
	static int vLightCount[4] = { 0, 0, 1, 0};

	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		m_vMAP.m_vDRAWBSP[i]->m_pMAP->DisableLight();

	for( i=0; i<TLIGHT_COUNT; i++)
		m_vLIGHT[i].EnableLight(FALSE);

	if(!m_pDevice->m_bEnableSHADER)
		return;

	m_pDevice->m_pDevice->SetVertexShaderConstantI(
		m_pDevice->m_vConstantVS[VC_LIGHTCOUNT],
		vLightCount, 1);
}

void CTClientGame::ResetSCENEConstant()
{
	m_pDevice->m_pDevice->SetMaterial(&m_vNormal);
	if(!m_pDevice->m_bEnableSHADER)
		return;

	FLOAT vCONST[16] = {
		1.0f,
		1.0f,
		m_vMAP.GetIntensity(),
		0.0f};

	m_pDevice->m_pDevice->SetPixelShaderConstantF(
		m_pDevice->m_vConstantPS[PC_ARG],
		vCONST, 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_MTRLAMBIENT],
		(FLOAT *) &D3DXVECTOR4(
		m_vNormal.Ambient.r,
		m_vNormal.Ambient.g,
		m_vNormal.Ambient.b,
		m_vNormal.Ambient.a), 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_MTRLDIFFUSE],
		(FLOAT *) &D3DXVECTOR4(
		m_vNormal.Diffuse.r,
		m_vNormal.Diffuse.g,
		m_vNormal.Diffuse.b,
		m_vNormal.Diffuse.a), 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_AMBIENT],
		(FLOAT *) &D3DXVECTOR4(
		m_vMAP.GetSpecRange(),
		m_vMAP.GetSpecMax(),
		0.0f,
		0.0f), 1);

	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_CAMPOS],
		(FLOAT *) &D3DXVECTOR4(
		m_pCamera->m_vPosition.x,
		m_pCamera->m_vPosition.y,
		m_pCamera->m_vPosition.z,
		0.0f), 1);

	D3DXMatrixTranspose( (LPD3DXMATRIX) vCONST, &(m_pCamera->m_matView * m_pCamera->m_matProjection));
	m_pDevice->m_pDevice->SetVertexShaderConstantF(
		m_pDevice->m_vConstantVS[VC_PROJ],
		vCONST, 4);
}

void CTClientGame::ShotSkill( CTClientObjBase *pTOBJ, LPTSKILLDATA pSKILLDATA)
{
	LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(pSKILLDATA->m_wID);
	if( pTSKILL )
	{
		ShotSkill(pTSKILL, pTOBJ, pSKILLDATA, &pTOBJ->m_vTSKILLTARGET);
		pTOBJ->OnShotSkill();
	}
}

void CTClientGame::ShotSkill( LPTSKILL pTSKILL, CTClientObjBase * pTOBJ, LPTSKILLDATA pSKILLDATA, VTSKILLTARGET* pSKILLTARGETS )
{
	if( pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE )
	{
		if(pTSKILL->m_dwSFX[TSKILLSFX_DEFEND])
		{
			LPTSFXTEMP pTEMP = CTChart::FindTSFXTEMP( pTSKILL->m_dwSFX[ TSKILLSFX_DEFEND ] );

			if( pTEMP && pTEMP->m_pTSFX )
			{
				CTachyonSFX *pTSFX = new CTachyonSFX();

				D3DXMatrixIdentity(&pTSFX->m_vGlobal);
				pTSFX->m_vGlobal._41 = pSKILLDATA->m_vTGROUND.x;
				pTSFX->m_vGlobal._42 = pSKILLDATA->m_vTGROUND.y;
				pTSFX->m_vGlobal._43 = pSKILLDATA->m_vTGROUND.z;

				pTSFX->InitSFX(
					m_pDevice,
					pTEMP->m_pTSFX);
				pTSFX->m_bHasHost = FALSE;

				CTachyonSFX::RegSFX( pTSFX, 0);
			}
		}

		for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
			if( pTSKILL->m_vTFUNCTION[i]->m_bType == SDT_STATUS &&
				pTSKILL->m_vTFUNCTION[i]->m_bFunctionID == SDT_STATUS_LINK &&
				!pTSKILL->m_vTFUNCTION[i]->m_wInc )
			{
				LPTSKILL pTLINK = CTChart::FindTSKILLTEMP(pTSKILL->m_vTFUNCTION[i]->m_wValue);

				if(pTLINK)
					RangeSHOT( pTOBJ, pSKILLDATA, &pSKILLDATA->m_vTGROUND, pTLINK, 0, TRUE);
			}
	}

	for( int i=0; i<INT(pSKILLTARGETS->size()); i++)
	{
		CTClientObjBase *pTDEFEND = FindOBJ(
			(*pSKILLTARGETS)[i]->m_dwID,
			(*pSKILLTARGETS)[i]->m_bType);

		if(CanDefend( pTOBJ, pTDEFEND, pTSKILL))
			Defend( pTOBJ, pTDEFEND, pSKILLDATA, &pSKILLDATA->m_vTGROUND, pTSKILL, 0, TRUE);
	}
}

void CTClientGame::RangeSHOT( CTClientObjBase *pTATTACK,
							  LPTSKILLDATA pTSKILLDATA,
							  LPD3DXVECTOR3 pTGROUND,
							  LPTSKILL pTSKILL,
							  DWORD dwTick,
							  BOOL bBroadCast )
{
	MAPMONSTER::iterator itMON;
	MAPPLAYER::iterator itPC;
	MAPRECALL::iterator itRECALL;

	for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
	{
		FLOAT fDIST = D3DXVec2Length(&D3DXVECTOR2(
			pTGROUND->x - (*itRECALL).second->GetPositionX(),
			pTGROUND->z - (*itRECALL).second->GetPositionZ()));

		if( CanDefend( pTATTACK, (*itRECALL).second, pTSKILL) && fDIST < pTSKILL->m_fAtkRange )
			Defend( pTATTACK, (*itRECALL).second, pTSKILLDATA, pTGROUND, pTSKILL, dwTick, bBroadCast);
	}

	for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
	{
		FLOAT fDIST = D3DXVec2Length(&D3DXVECTOR2(
			pTGROUND->x - (*itRECALL).second->GetPositionX(),
			pTGROUND->z - (*itRECALL).second->GetPositionZ()));

		if( CanDefend( pTATTACK, (*itRECALL).second, pTSKILL) && fDIST < pTSKILL->m_fAtkRange )
			Defend( pTATTACK, (*itRECALL).second, pTSKILLDATA, pTGROUND, pTSKILL, dwTick, bBroadCast);
	}

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
	{
		FLOAT fDIST = D3DXVec2Length(&D3DXVECTOR2(
			pTGROUND->x - (*itMON).second->GetPositionX(),
			pTGROUND->z - (*itMON).second->GetPositionZ()));

		if( CanDefend( pTATTACK, (*itMON).second, pTSKILL) && fDIST < pTSKILL->m_fAtkRange )
			Defend( pTATTACK, (*itMON).second, pTSKILLDATA, pTGROUND, pTSKILL, dwTick, bBroadCast);
	}

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
	{
		FLOAT fDIST = D3DXVec2Length(&D3DXVECTOR2(
			pTGROUND->x - (*itPC).second->GetPositionX(),
			pTGROUND->z - (*itPC).second->GetPositionZ()));

		if( CanDefend( pTATTACK, (*itPC).second, pTSKILL) && fDIST < pTSKILL->m_fAtkRange )
			Defend( pTATTACK, (*itPC).second, pTSKILLDATA, pTGROUND, pTSKILL, dwTick, bBroadCast);
	}

	FLOAT fDIST = D3DXVec2Length(&D3DXVECTOR2(
		pTGROUND->x - m_pMainChar->GetPositionX(),
		pTGROUND->z - m_pMainChar->GetPositionZ()));

	if( CanDefend( pTATTACK, m_pMainChar, pTSKILL) && fDIST < pTSKILL->m_fAtkRange )
		Defend( pTATTACK, m_pMainChar, pTSKILLDATA, pTGROUND, pTSKILL, dwTick, bBroadCast);
}

void CTClientGame::LineSHOT( CTClientObjBase *pTATTACK,
							 LPTSKILLDATA pTSKILLDATA,
							 LPD3DXVECTOR3 pTGROUND,
							 LPTSKILL pTSKILL,
							 DWORD dwTick,
							 BOOL bBroadCast )
{
	D3DXVECTOR3 vTDIR = D3DXVECTOR3(
		pTATTACK ? pTGROUND->x - pTATTACK->GetPositionX() : 0.0f,
		0.0f,
		pTATTACK ? pTGROUND->z - pTATTACK->GetPositionZ() : 0.0f);
	FLOAT fLength = D3DXVec3Length(&vTDIR);

	if( fLength > 0.0f )
	{
		MAPMONSTER::iterator itMON;
		MAPPLAYER::iterator itPC;
		MAPRECALL::iterator itRECALL;

		FLOAT fTSCALE[4] = { pTSKILL->m_fAtkRange, 1.0f, 1.0f, 1.0f};
		D3DXPLANE vTPLANE[4];
		D3DXMATRIX vTROT;

		D3DXMatrixRotationY(
			&vTROT,
			D3DX_PI / 2.0f);
		vTDIR /= fLength;

		for( BYTE i=0; i<4; i++)
		{
			D3DXVECTOR3 vTPOINT = (*pTGROUND) + fTSCALE[i] * vTDIR;

			D3DXPlaneFromPointNormal(
				&vTPLANE[i],
				&vTPOINT,
				&vTDIR);

			CTMath::Transform(
				&vTROT,
				&vTDIR);
		}

		for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
		{
			D3DXVECTOR3 vTPOS(
				(*itRECALL).second->GetPositionX(),
				(*itRECALL).second->GetPositionY(),
				(*itRECALL).second->GetPositionZ());

			if( CanDefend( pTATTACK, (*itRECALL).second, pTSKILL) && POSInRECT( &vTPOS, vTPLANE) )
				Defend( pTATTACK, (*itRECALL).second, pTSKILLDATA, &pTSKILLDATA->m_vTGROUND, pTSKILL, dwTick, bBroadCast);
		}

		for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
		{
			D3DXVECTOR3 vTPOS(
				(*itRECALL).second->GetPositionX(),
				(*itRECALL).second->GetPositionY(),
				(*itRECALL).second->GetPositionZ());

			if( CanDefend( pTATTACK, (*itRECALL).second, pTSKILL) && POSInRECT( &vTPOS, vTPLANE) )
				Defend( pTATTACK, (*itRECALL).second, pTSKILLDATA, &pTSKILLDATA->m_vTGROUND, pTSKILL, dwTick, bBroadCast);
		}

		for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
		{
			D3DXVECTOR3 vTPOS(
				(*itMON).second->GetPositionX(),
				(*itMON).second->GetPositionY(),
				(*itMON).second->GetPositionZ());

			if( CanDefend( pTATTACK, (*itMON).second, pTSKILL) && POSInRECT( &vTPOS, vTPLANE) )
				Defend( pTATTACK, (*itMON).second, pTSKILLDATA, &pTSKILLDATA->m_vTGROUND, pTSKILL, dwTick, bBroadCast );
		}

		for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
		{
			D3DXVECTOR3 vTPOS(
				(*itPC).second->GetPositionX(),
				(*itPC).second->GetPositionY(),
				(*itPC).second->GetPositionZ());

			if( CanDefend( pTATTACK, (*itPC).second, pTSKILL) && POSInRECT( &vTPOS, vTPLANE) )
				Defend( pTATTACK, (*itPC).second, pTSKILLDATA, &pTSKILLDATA->m_vTGROUND, pTSKILL, dwTick, bBroadCast );
		}

		D3DXVECTOR3 vTPOS(
			m_pMainChar->GetPositionX(),
			m_pMainChar->GetPositionY(),
			m_pMainChar->GetPositionZ());

		if( CanDefend( pTATTACK, m_pMainChar, pTSKILL) && POSInRECT( &vTPOS, vTPLANE) )
			Defend( pTATTACK, m_pMainChar, pTSKILLDATA, &pTSKILLDATA->m_vTGROUND, pTSKILL, dwTick, bBroadCast );
	}
}

BOOL CTClientGame::Defend( CTClientObjBase *pTATTACK,
						   CTClientObjBase *pTDEFEND,
						   LPTSKILLDATA pTSKILLDATA,
						   LPD3DXVECTOR3 pTGROUND,
						   LPTSKILL pTSKILL,
						   DWORD dwTick,
						   BOOL bBroadCast )
{
	BOOL bSendedPacket = FALSE;

	if(CheckDEFMSG( pTATTACK, pTDEFEND, bBroadCast))
	{
		WORD wAL = pTSKILLDATA->m_wAL;
		BYTE bDEFEND = TRUE;

		if(pTSKILL->m_bPursuit)
		{
			WORD wPursuit = GetPursuit(pTDEFEND);

			if( wPursuit == 0xFFFF )
			{
				SkillMSG( pTDEFEND, SKILL_WRONGTARGET);
				bDEFEND = FALSE;
			}
			else
				wAL = wPursuit;
		}

		if(bDEFEND)
		{
			TACTION vActionID = pTDEFEND->FindActionID(
				pTSKILL->m_bActionID[TSKILLACTION_DEFEND],
				pTDEFEND->GetWeaponID(pTDEFEND->m_bMode));

			if( bBroadCast )
			{
				SendCS_DEFEND_REQ(
					pTATTACK ? pTATTACK->GetHostID() : 0,
					pTATTACK ? pTATTACK->m_dwID : 0,
					pTDEFEND->m_dwID,
					pTATTACK ? pTATTACK->m_bType : OT_NONE,
					pTDEFEND->m_bType,
					GetPartyID(pTATTACK),
					vActionID.m_dwActID,
					vActionID.m_dwAniID,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					pTSKILLDATA->m_bAtkLevel,
					pTSKILLDATA->m_dwMinAP,
					pTSKILLDATA->m_dwMaxAP,
					pTSKILLDATA->m_dwMinMP,
					pTSKILLDATA->m_dwMaxMP,
					pTSKILLDATA->m_wSTR,
					pTSKILLDATA->m_wINT,
					pTSKILLDATA->m_bCURSE,
					pTSKILLDATA->m_bSpecial,
					pTSKILLDATA->m_bAglow,
					pTSKILLDATA->m_bAtkCountryID,
					wAL, pTSKILLDATA->m_bHIT,
					pTSKILL->m_wSkillID,
					pTSKILLDATA->m_bLevel,
					pTGROUND->x,
					pTGROUND->y,
					pTGROUND->z,
					pTDEFEND->GetPositionX(),
					pTDEFEND->GetPositionY(),
					pTDEFEND->GetPositionZ(),
					dwTick);

/*#ifdef TEST_MODE
				CString strFMT;
				strFMT.Format( "Send Defend %s To %s", pTATTACK->GetName(), pTDEFEND->GetName() );
				CTClientGame::DebugMSG( strFMT );
#endif*/
			}
			else
			{
				if( pTDEFEND )
				{
					// 이 디펜더는 m_pMainChar이거나 내가 호스트 중인 몬스터, 소환수일 수 없다.
					if( pTDEFEND == m_pMainChar ||
						(pTDEFEND->m_bType == OT_MON && ((CTClientMonster*)pTDEFEND)->m_bHostMainChar) ||
						(pTDEFEND->m_bType == OT_RECALL && ((CTClientRecall*)pTDEFEND)->m_dwHostID == m_pMainChar->m_dwID) )
					{
						m_pSESSION->SendCS_ACDCLOSE_REQ();
						return FALSE;
					}
				}

				CPacket packet;
				packet.SetID(CS_DEFEND_ACK)
					<< (DWORD)( pTATTACK ? pTATTACK->m_dwID : 0 )
					<< pTDEFEND->m_dwID
					<< (BYTE)( pTATTACK ? pTATTACK->m_bType : OT_NONE )
					<< pTDEFEND->m_bType
					<< (DWORD)( pTATTACK ? pTATTACK->GetHostID() : 0 )
					<< (BYTE)( pTATTACK ? pTATTACK->m_bType : OT_NONE )
					<< vActionID.m_dwActID
					<< vActionID.m_dwAniID
					<< (BYTE) 1
					<< dwTick
					<< pTSKILLDATA->m_bHIT
					<< (BYTE) HT_COUNT
					<< wAL
					<< pTSKILLDATA->m_bAtkLevel
					<< pTSKILLDATA->m_dwMinAP
					<< pTSKILLDATA->m_dwMaxAP
					<< pTSKILLDATA->m_dwMinMP
					<< pTSKILLDATA->m_dwMaxMP
					<< pTSKILLDATA->m_bAglow
					<< (BYTE) 0
					<< pTSKILLDATA->m_bAtkCountryID
					<< pTSKILL->m_wSkillID
					<< pTSKILLDATA->m_bLevel
					<< (WORD) 0
					<< (BYTE) 1
					<< pTGROUND->x
					<< pTGROUND->y
					<< pTGROUND->z
					<< pTDEFEND->GetPositionX()
					<< pTDEFEND->GetPositionY()
					<< pTDEFEND->GetPositionZ()
					<< (BYTE) 0;

				packet.Rewind(FALSE);
				OnSessionMsg( GetSession(), &packet );
			}
			
			bSendedPacket = TRUE;
		}
	}

	for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
		if( pTSKILL->m_vTFUNCTION[i]->m_bType == SDT_STATUS &&
			pTSKILL->m_vTFUNCTION[i]->m_bFunctionID == SDT_STATUS_LINK &&
			!pTSKILL->m_vTFUNCTION[i]->m_wInc )
		{
			LPTSKILL pTLINK = CTChart::FindTSKILLTEMP(pTSKILL->m_vTFUNCTION[i]->m_wValue);

			if(pTLINK)
			{
				D3DXVECTOR3 vTGROUND(
					pTDEFEND->GetPositionX(),
					pTDEFEND->GetPositionY(),
					pTDEFEND->GetPositionZ());

				if( pTLINK->m_bRangeType == TSKILLRANGE_LINE )
					LineSHOT( pTATTACK, pTSKILLDATA, &vTGROUND, pTLINK, dwTick, bBroadCast );
				else if( pTLINK->m_fAtkRange > 0.0f )
					RangeSHOT( pTATTACK, pTSKILLDATA, &vTGROUND, pTLINK, dwTick, bBroadCast );
				else if(CanDefend( pTATTACK, pTDEFEND, pTLINK))
					Defend( pTATTACK, pTDEFEND, pTSKILLDATA, &vTGROUND, pTLINK, dwTick, bBroadCast );
			}
		}

	return bSendedPacket;
}

DWORD CTClientGame::FindOBJPIVOT(CTClientObjBase* pTOBJ, LPTITEMVISUAL pTVISUAL)
{
	static DWORD vPIVOTS[] = 
	{
		NULL,
		ID_PIVOT_ARROW,
		ID_RHAND,
		ID_LHAND,
		ID_PIVOT_CARROW,
		NULL
	};

	vPIVOTS[0] = pTVISUAL->m_dwPivot[MT_BATTLE];

	int i = 0;
	while( vPIVOTS[i] )
	{
		MAPDWORD::iterator itID = pTOBJ->m_OBJ.m_pOBJ->m_mapPIVOT.find(vPIVOTS[i]);
		if( itID != pTOBJ->m_OBJ.m_pOBJ->m_mapPIVOT.end() )
			return itID->second;

		++i;
	}

	return (DWORD)T_INVALID;
}

void CTClientGame::ShotArrow( CTClientObjBase *pTOBJ, LPTSKILLDATA pSKILLDATA, LPTSKILL pTSKILL)
{
	LPTITEM pTITEM = CTChart::FindTITEMTEMP(pTSKILL->m_wItemID[pTOBJ->GetSkillItemType(pTSKILL)]);

	pTOBJ->ClearTARROW();
	if( pTITEM )
	{
		LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTITEM);
		if( pTVISUAL )
		{
			DWORD dwOBJPIVOT = FindOBJPIVOT(pTOBJ, pTVISUAL);
			if( dwOBJPIVOT == (DWORD)T_INVALID )
			{
				TRACE("Warning(CTClientGame::ShotArrow): 공격자(TYPE:%u ID:%u)가 발사지점으로 사용할 "
					"피봇점(0x%x)을 찾을 수 없어 타겟에 대하여 스킬(%s)을 사용할 수 없습니다.\n",
					pTOBJ->m_bType, 
					pTOBJ->m_dwID, 
					pTVISUAL->m_dwPivot[MT_BATTLE],
					pTSKILL->m_strNAME);

				return;
			}

			for( int i=0; i<INT(pTOBJ->m_vTSKILLTARGET.size()); i++)
			{
				CTClientObjBase *pTARGET = FindOBJ(
					pTOBJ->m_vTSKILLTARGET[i]->m_dwID,
					pTOBJ->m_vTSKILLTARGET[i]->m_bType);

				if( pTARGET )
				{
					CTClientArrow *pTARROW = new CTClientArrow();

					pTARROW->InitOBJ(m_pRES->GetOBJ(pTVISUAL->m_dwObjectID));
					pTARROW->SetCloth(
						m_pDevice,
						pTVISUAL->m_dwCLKID,
						pTVISUAL->m_dwCLIID,
						pTVISUAL->m_dwMesh[MT_NORMAL]);
					pTARROW->ResetOBJPart(m_pDevice);

					pTARROW->m_fVelocityX = pTITEM->m_fShotSpeed;
					pTARROW->m_fGravity = pTITEM->m_fGravity;
					pTARROW->m_fStartH = pTOBJ->m_pBone[dwOBJPIVOT + 1]._42;

					if(pTSKILL->m_vTARROWDIR.empty())
					{
						D3DXVECTOR2 vDIR(
							pTARGET->GetPositionX() - pTOBJ->m_pBone[dwOBJPIVOT + 1]._41,
							pTARGET->GetPositionZ() - pTOBJ->m_pBone[dwOBJPIVOT + 1]._43);

						D3DXMatrixRotationY(
							&pTARROW->m_vWorld,
							D3DX_PI + atan2f( vDIR.x, vDIR.y));

						pTARROW->m_bAccel = FALSE;
					}
					else
					{
						FLOAT fDIR = pTSKILL->m_vTARROWDIR[i % INT(pTSKILL->m_vTARROWDIR.size())] + D3DX_PI + FLOAT(pTOBJ->m_wDIR) * D3DX_PI / 900.0f;

						D3DXMatrixRotationY(
							&pTARROW->m_vWorld,
							fDIR);

						pTARROW->m_vTACCEL = D3DXVECTOR2(
							sinf(fDIR),
							cosf(fDIR));

						pTARROW->m_dwTotalTick = DWORD(D3DXVec2Length(&D3DXVECTOR2(
							pTARGET->GetPositionX() - pTOBJ->m_pBone[dwOBJPIVOT + 1]._41,
							pTARGET->GetPositionZ() - pTOBJ->m_pBone[dwOBJPIVOT + 1]._43)) / (2.0f * pTARROW->m_fVelocityX));
						pTARROW->m_bAccel = TRUE;
					}

					if(pTSKILL->m_dwSFX[TSKILLSFX_ARROW])
						pTARROW->PlayTArrowSFX(pTSKILL->m_dwSFX[TSKILLSFX_ARROW]);

					pTARROW->m_vWorld._41 = pTOBJ->m_pBone[dwOBJPIVOT + 1]._41;
					pTARROW->m_vWorld._42 = pTARROW->m_fStartH;
					pTARROW->m_vWorld._43 = pTOBJ->m_pBone[dwOBJPIVOT + 1]._43;
					pTARROW->m_vPosition = pTARROW->m_vWorld;

					memcpy( &pTARROW->m_vTSKILLDATA, pSKILLDATA, sizeof(TSKILLDATA));
					pTARROW->m_pTARGET = pTOBJ->m_vTSKILLTARGET[i];

					pTARROW->m_bHostType = pTOBJ->m_bType;
					pTARROW->m_dwHostID = pTOBJ->m_dwID;

					if( pTVISUAL->m_fSlashLen > 0.0f )
					{
						MAPRES::iterator itTEX = m_pRES->m_mapTEX.find(pTVISUAL->m_dwSlashTex);
						LPTEXTURESET pTEX = NULL;

						if( itTEX != m_pRES->m_mapTEX.end() )
							pTEX = (LPTEXTURESET) (*itTEX).second;

						pTARROW->m_vTSLASH.InitSFX(
							&pTARROW->m_vPosition,
							pTEX,
							pTVISUAL->m_dwSlashColor,
							pTVISUAL->m_fSlashLen,
							TSLASH_INTER);

						pTARROW->m_vTSLASH.m_vLENGTH._41 = -pTVISUAL->m_fSlashLen / 2.0f;
						pTARROW->m_vTSLASH.SFXStart();
					}

					m_vTARROW.push_back(pTARROW);
				}
				else
					delete pTOBJ->m_vTSKILLTARGET[i];
			}

			pTOBJ->m_vTSKILLTARGET.clear();
		}	
	}

	pTOBJ->OnShotSkill();
}

LRESULT CTClientGame::OnOBJMsg( LPOBJMSG pMSG)
{
	switch(pMSG->m_dwMSG)
	{
	case OM_TSUBACTENDING	:
		{
			CTClientObjBase *pTOBJ = (CTClientObjBase *) pMSG->m_pOBJ;

			if(pTOBJ->m_bCLEARTARGET)
				pTOBJ->ClearSkillTarget();

			pTOBJ->m_bActionLock = FALSE;
			pTOBJ->m_bSubActEnd = TRUE;
		}

		break;

	case OM_ACTENDING		: return OnOBJActEndMsg(pMSG);
	case OM_3RD_ATTACK		:
	case OM_2ND_ATTACK		:
	case OM_ATTACK			:
	case OM_SHOT			:
		{
			CTClientObjBase *pTOBJ = (CTClientObjBase *) pMSG->m_pOBJ;
			
			LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(pTOBJ->m_vTSKILLDATA.m_wID);
			if( pTSKILL )
			{
				pTOBJ->PlayTActionSFX(
					&pTOBJ->m_vTGARBAGESFX,
					pTSKILL->m_dwSFX[TSKILLSFX_ACT]);
			}

			OnOBjShotMsg(pTOBJ, FALSE);
			OnOBjShotMsg(pTOBJ, TRUE);

			pTOBJ->m_bCLEARTARGET = TRUE;
		}

		break;
	}

	return 0;
}

LRESULT CTClientGame::OnOBJActEndMsg( LPOBJMSG pMSG)
{
	CTClientObjBase *pTOBJ = (CTClientObjBase *) pMSG->m_pOBJ;

	if( pTOBJ == m_pMainChar )
		pTOBJ->m_bSTEPSND = TRUE;

	return pTOBJ ? pTOBJ->OnActEndMsg() : 0;
}

void CTClientGame::OnOBjShotMsg( CTClientObjBase * pTOBJ, BOOL bIsBackSkill )
{
	LPTSKILLDATA pTSKILLDATA;
	if( bIsBackSkill )
		pTSKILLDATA = &pTOBJ->m_vTBACKSKILLDATA;
	else
		pTSKILLDATA = &pTOBJ->m_vTSKILLDATA;

	LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(pTSKILLDATA->m_wID);
	if( !pTSKILL )
		return;

	CheckAttackMaintain( pTOBJ, pTSKILL);

	if( !bIsBackSkill )
	{
		if( pTOBJ->m_bShoot && pTSKILL && pTSKILL->m_wItemID[pTOBJ->GetSkillItemType(pTSKILL)] )
			ShotArrow( pTOBJ, pTSKILLDATA, pTSKILL );
		else
			ShotSkill( pTOBJ, pTSKILLDATA );
	}
	else
	{
		if( pTSKILL->m_bRangeType == TSKILLRANGE_LINE )
			LineSHOT( pTOBJ, pTSKILLDATA, &pTSKILLDATA->m_vTGROUND, pTSKILL, 0, TRUE );
		else if( pTSKILL->m_fAtkRange > 0.0f )
			RangeSHOT( pTOBJ, pTSKILLDATA, &pTSKILLDATA->m_vTGROUND, pTSKILL, 0, TRUE );
		else
			ShotSkill( pTOBJ, pTSKILLDATA );
	}
}

void CTClientGame::PlayUISound( DWORD dwSndID)
{
	CTachyonRes::m_MEDIA.Play( MEDIA_TSOUND, dwSndID);
}

void CTClientGame::SwitchFocus( TComponent *pCandidate)
{
	TComponent *pFocus = m_pFocus ? m_pFocus : m_vTFRAME[TFRAME_MAIN];

	if( m_pTCURFRAME != pCandidate &&
		m_vTFRAME[TFRAME_MAIN] != pCandidate &&
		m_vTFRAME[TFRAME_GAUGE] != pCandidate )
	{
		m_pTCURFRAME = pCandidate;
	}

	if( pCandidate == m_vTFRAME[TFRAME_MAIN] )
	{
		pFocus = m_vTFRAME[TFRAME_MAIN];
		m_pChatFrame->ForceFocus();
	}

	if( m_pFocus == pCandidate )
		return;

	if( pCandidate != m_vTFRAME[TFRAME_DETAIL_INFO] &&
		pCandidate != m_vTFRAME[TFRAME_CHAT_DETAIL_INFO] )
	{

		if( pCandidate != m_vTFRAME[TFRAME_MAIN] &&
			pCandidate != m_vTFRAME[TFRAME_GAUGE] )
		{
			RemoveKid(pCandidate);
			AddKid(pCandidate);
		}

		if(pCandidate)
		{
			if( pCandidate == m_vTFRAME[TFRAME_GUILDAPP] )
				pFocus = pCandidate;
			else if( pCandidate == m_vTFRAME[TFRAME_AUCTION] )
				pFocus = pCandidate;
			else
			{
				TCOMP_LIST::iterator itKID = pCandidate->GetFirstKidsFinder();

				while(!pCandidate->EndOfKids(itKID))
				{
					if( (*itKID)->m_bType == TCML_TYPE_EDIT || (*itKID)->m_bType == TCML_TYPE_HTTPCTRL )
					{
						pFocus = pCandidate;
						break;
					}

					pCandidate->GetNextKid(itKID);
				}
			}
		}

		if( m_pFocus == pFocus && m_pFocus != m_vTFRAME[TFRAME_MAIN] && m_pFocus != m_pTCURFRAME )
			pFocus = m_vTFRAME[TFRAME_MAIN];

		if( m_pFocus != pFocus )
		{
			if(m_pFocus)
			{
				m_pFocus->SetFocus(FALSE);
				m_pFocus->OnLoseFocus(pFocus);
			}

			if(pFocus)
			{
				pFocus->SetFocus(TRUE);
				pFocus->OnSetFocus(m_pFocus);
			}
			m_pFocus = pFocus;
		}
	}

	if(m_pTPOPUP && m_pTPOPUP->IsVisible())
	{
		RemoveKid(m_pTPOPUP);
		AddKid(m_pTPOPUP);
	}

	if(m_pChannelPOPUP && m_pChannelPOPUP->IsVisible())
	{
		RemoveKid(m_pChannelPOPUP);
		AddKid(m_pChannelPOPUP);
	}

	if(m_vTFRAME[TFRAME_DETAIL_INFO] && m_vTFRAME[TFRAME_DETAIL_INFO]->IsVisible())
	{
		RemoveKid(m_vTFRAME[TFRAME_DETAIL_INFO]);
		AddKid(m_vTFRAME[TFRAME_DETAIL_INFO]);
	}

	if(m_vTFRAME[TFRAME_CHAT_DETAIL_INFO] && m_vTFRAME[TFRAME_CHAT_DETAIL_INFO]->IsVisible())
	{
		RemoveKid(m_vTFRAME[TFRAME_CHAT_DETAIL_INFO]);
		AddKid(m_vTFRAME[TFRAME_CHAT_DETAIL_INFO]);
	}

	if(m_pActListPopup && m_pActListPopup->IsVisible())
	{
		RemoveKid(m_pActListPopup);
		AddKid(m_pActListPopup);
	}

	if( m_vTFRAME[TFRAME_DETAIL_NAME] && m_vTFRAME[TFRAME_DETAIL_NAME]->IsVisible() )
	{
		RemoveKid(m_vTFRAME[TFRAME_DETAIL_NAME]);
		AddKid(m_vTFRAME[TFRAME_DETAIL_NAME]);
	}

	if(m_vTFRAME[TFRAME_WORLDMAP] && m_vTFRAME[TFRAME_WORLDMAP]->IsVisible())
	{
		RemoveKid(m_vTFRAME[TFRAME_WORLDMAP]);
		AddKid(m_vTFRAME[TFRAME_WORLDMAP]);
	}
}

void CTClientGame::EnableAllInputDevice( BYTE bEnable)
{
	m_bUseLButtonDblClick = bEnable;
	m_bUseMButtonDblClick = bEnable;
	m_bUseRButtonDblClick = bEnable;
	m_bUseLButtonClick = bEnable;
	m_bUseMButtonClick = bEnable;
	m_bUseRButtonClick = bEnable;
	m_bUseLButtonDown = bEnable;
	m_bUseMButtonDown = bEnable;
	m_bUseRButtonDown = bEnable;
	m_bUseMouseWheel = bEnable;
	m_bUseMouseMove = bEnable;
	m_bUseKeyboardInput = bEnable;

	m_bCanUseSkill = bEnable;
	m_bCanUseItem = bEnable;
}

void CTClientGame::DisableTargetUI()
{
	DisableUI(TFRAME_NPCDIALOG);
	DisableUI(TFRAME_ITEM_SHOP);
	DisableUI(TFRAME_RENT_SKILL);
	DisableUI(TFRAME_MONINVEN);
	DisableUI(TFRAME_ITEM_UP);
	DisableUI(TFRAME_ITEM_REPAIR);
	DisableUI(TFRAME_ITEM_REFINE);
	DisableUI(TFRAME_ITEM_GAMBLE);
	DisableUI(TFRAME_ITEM_EXTPERIOD);
	DisableUI(TFRAME_PRIVATE_BUY);
	DisableUI(TFRAME_CABINET);
	DisableUI(TFRAME_PORTAL_SHOP);
	DisableUI(TFRAME_PORTAL_SHOP_NEW);
	DisableUI(TFRAME_CRAFT);
	DisableUI(TFRAME_AUCTION);
#ifdef ADD_RPS
	if( m_vTFRAME[TFRAME_RPS]->IsVisible() )
	{
		m_pMainWnd->CloseMessageBox(TRUE);

		CTRpsDlg* pDLG = static_cast<CTRpsDlg*>( m_vTFRAME[TFRAME_RPS] );
		pDLG->m_bSelectMode = FALSE;
		DisableUI( TFRAME_RPS );
		DisableUI( TFRAME_RPS_REWARD );

		m_bRPSGaming = FALSE;
		GetSession()->SendCS_RPSGAME_REQ( RPSCARD_COUNT ); // 포기
	}
#endif
}

void CTClientGame::CloseAllUI()
{
	m_pMainWnd->CloseModalFrame();

	for( INT i=0; i<TFRAME_COUNT; i++)
	{
		if( i == TFRAME_MAIN || i == TFRAME_GAUGE )
			continue;

		DisableUI( i );
	}	
}

void CTClientGame::ToggleUI( BYTE bUI)
{
	if(m_vTFRAME[bUI])
		if(m_vTFRAME[bUI]->IsVisible())
			DisableUI(bUI);
		else
			EnableUI(bUI);
}

void CTClientGame::DisableUI( BYTE bUI, BOOL bShow)
{
	if( m_vTFRAME[bUI] && m_vTFRAME[bUI]->IsVisible() )
	{
		if( m_vTFRAME[bUI]->CanDisable() )
			m_vTFRAME[bUI]->ShowComponent( bShow );
		else
			return ;

		switch(bUI)
		{
		case TFRAME_ITEM_SHOP:
			{
				CTShopDlg* pSHOPDLG = (CTShopDlg*) m_vTFRAME[bUI];
				pSHOPDLG->ClearOpenByCash();
			}
			break;

		case TFRAME_PORTAL_SHOP:
			{
				CTShopDlg* pSHOPDLG = (CTShopDlg*) m_vTFRAME[bUI];
				pSHOPDLG->ClearOpenByCash();
			}
			break;

		case TFRAME_ITEM_UP:
			{
				CTItemUpDlg* pITEMUPDLG = (CTItemUpDlg*) m_vTFRAME[bUI];
				pITEMUPDLG->ClearItem();
				pITEMUPDLG->ClearOpenByCash();
			}
			break;

		case TFRAME_TRADE		: 
			{
				((CTTradeDlg*) m_vTFRAME[bUI])->Clear(TRUE);
				((CTTradeDlg*) m_vTFRAME[bUI])->Clear(FALSE);
			}
			break;

		case TFRAME_MONINVEN	:
			{
				if( m_pMainChar->m_bAction == m_pMainChar->GetTLOOPAction(TA_LOOT_START) )
				{
					TACTION vActionID = m_pMainChar->FindActionID(
						TA_LOOT_END,
						m_pMainChar->GetWeaponID(m_pMainChar->m_bMode));

					SendCS_ACTION_REQ(
						m_pMainChar->m_dwID,
						m_pMainChar->m_bType,
						TA_LOOT_END,
						vActionID.m_dwActID,
						vActionID.m_dwAniID,
						m_pMainWnd->m_bChannel,
						m_vMAP.m_wMapID, 0);
				}

				if(m_pTARGET)
					switch(m_pTARGET->m_bType)
					{
					case OT_MON		: SendCS_MONITEMLIST_REQ( FALSE, m_pTARGET->m_dwID); break;
					case OT_NPC		: ((CTClientNpc *) m_pTARGET)->OpenBOX(FALSE); break;
					}

				ClearMonInven();
			}

			break;

		case TFRAME_CINEMATIC	:
			{
				m_vTFRAME[TFRAME_GAUGE]->ShowComponent(TRUE);
				m_vTFRAME[TFRAME_MAIN]->ShowComponent(TRUE);
				EnableAllInputDevice();

				ResetPlayerINFO(FALSE);
				ResetTargetINFO(FALSE);
				ResetPartyINFO(FALSE);
				ResetRSCS(m_bRSCS);
				EnableChat(FALSE);

				m_pChatFrame->ShowComponent(FALSE);
				m_pChatFrame->ResetTick();

				ResetRecallINFO(FALSE);
				CalcTRSCS(0);
			}

			break;

		case TFRAME_TOURNAMENT_GAGE:
			{
				m_vTFRAME[TFRAME_GAUGE]->ShowComponent(TRUE);

				ResetPlayerINFO(FALSE);
				ResetTargetINFO(FALSE);
				ResetPartyINFO(FALSE);
				ResetRSCS(m_bRSCS);
				ResetRecallINFO(FALSE);
				ResetSkillICON();
			}

			break;

		case TFRAME_ITEM_REPAIR:
			{
				CTItemRepairDlg* pITEMREPAIRDLG = (CTItemRepairDlg*) m_vTFRAME[bUI];
				pITEMREPAIRDLG->ClearItem();
				pITEMREPAIRDLG->ClearOpenByCash();
			}
			break;

		case TFRAME_ITEM_REFINE:
			{
				CTItemRefineDlg* pITEMREFINEDLG = (CTItemRefineDlg*) m_vTFRAME[bUI];
				pITEMREFINEDLG->ClearItem();
				pITEMREFINEDLG->ClearOpenByCash();
			}
			break;

		case TFRAME_ITEM_GAMBLE:
			{
				CTItemGambleDlg* pITEMGAMBLEDLG = (CTItemGambleDlg*) m_vTFRAME[bUI];
				pITEMGAMBLEDLG->ClearItem();
				pITEMGAMBLEDLG->ClearOpenByCash();
			}
			break;

		case TFRAME_PRIVATE_BUY:
			static_cast<CTPrivateShopDlg*>(m_vTFRAME[TFRAME_PRIVATE_BUY])->Clear();
			break;

		case TFRAME_PRIVATE_SELL:
			static_cast<CTPrivateShopDlgForSell*>(m_vTFRAME[TFRAME_PRIVATE_SELL])->Clear();
			break;

		case TFRAME_SKILL:
			{
				CTSkillDlg *pDlg = static_cast<CTSkillDlg *>(m_vTFRAME[TFRAME_SKILL]);

				if(pDlg->m_bSKILLINIT)
					pDlg->EndSkillInitMode();

				pDlg->ClearOpenByCash();
			}
			break;

		case TFRAME_CABINET:
			{
				CTNormalCabinetDlg *pNORMALCABINETDLG = (CTNormalCabinetDlg *) GetFrame( TFRAME_CABINET );
				pNORMALCABINETDLG->ClearOpenByCash();
			}
			break;

		case TFRAME_WARN_NOTUSE_CASHITEM :
			{
				CTItemUpCashDlg* pDlg = static_cast<CTItemUpCashDlg*>(m_vTFRAME[TFRAME_WARN_NOTUSE_CASHITEM]);
				pDlg->Clear();
			}
			break;

		case TFRAME_WARN_UPITEM_SELL :
			{
				CTUpItemSellDlg* pDlg = static_cast<CTUpItemSellDlg*>(m_vTFRAME[TFRAME_WARN_UPITEM_SELL]);
				pDlg->Clear();
			}
			break;

		case TFRAME_GUILDMARK:
			{
				m_pMainChar->ApplyMantleTexture();
			}
			break;
#ifdef ADD_RPS
		case TFRAME_RPS:
			{
				m_bRPSKeyEnable = FALSE;
			}
			break;
#endif
		}
	}
}

void CTClientGame::EnableUI( BYTE bUI)
{
	if(m_vTFRAME[bUI] && !m_vTFRAME[bUI]->IsVisible())
	{
		if( !m_vTFRAME[bUI]->CanWithItemUI() )
			CancelAllItemUI();

		if( IsItemUI(m_vTFRAME[bUI]) )
		{
			if( m_bRPSGaming )
			{
				m_pChatFrame->ChatSysMSG(
					CTChart::LoadString( TSTR_INFO_TITLE),
					m_pMainChar->GetName(),
					CTChart::LoadString(TSTR_RPSGAME_DONOT_OPEN_DLG),
					TCOLOR_ERROR, TCHAT_FLAG_INFO, ID_SND_ERROR, MEDIA_TSOUND);
				return ;
			}

			for(BYTE i=0; i<TFRAME_COUNT; ++i)
			{
				if( m_vTFRAME[i] && m_vTFRAME[i]->IsVisible() && !m_vTFRAME[i]->CanWithItemUI() )
				{
					switch(i)
					{
					case TFRAME_TRADE		:	CancelTrade(); break;
					case TFRAME_PRIVATE_SELL:	CancelPrivateSell(); break;
					case TFRAME_PRIVATE_BUY	:	CancelPrivateBuy(); break;
					default					:	DisableUI(i); break;
					}
				}
			}
		}

		switch(bUI)
		{
		case TFRAME_TRADE			: 
			{
				EnableUI(TFRAME_DEFINVEN);
				EnableAllSubInven(TRUE);

				static_cast<CTTradeDlg*>(m_vTFRAME[bUI])->SetLock(FALSE);
			}
			break;

		case TFRAME_ITEM_SHOP		:
			{
				EnableUI(TFRAME_DEFINVEN);
				EnableAllSubInven(TRUE);

				DisableTargetUI();
			}
			break;

		case TFRAME_RENT_SKILL		:
		case TFRAME_PORTAL_SHOP		:
		case TFRAME_PORTAL_SHOP_NEW	:
		case TFRAME_SKILL			: DisableTargetUI(); break;
		case TFRAME_MONINVEN		:
			{
				if(m_pMainChar->GetRidingPet())
				{
					DisableTargetUI();
					ResetTMonInvenUI();

					if( m_pTARGET && m_pTARGET->m_bType == OT_NPC )
						((CTClientNpc *) m_pTARGET)->OpenBOX(TRUE);

					break;
				}

				if(m_bCHARGE)
					CancelCharge(TRUE);
				m_pMainChar->CancelTAction();
				m_pMainChar->ApplyTAction( m_pDevice, m_pRES, TA_LOOT_START);

				if( m_pMainChar->m_bAction == m_pMainChar->GetTLOOPAction(TA_LOOT_START) )
				{
					TACTION vActionID = m_pMainChar->FindActionID(
						TA_LOOT_START,
						m_pMainChar->GetWeaponID(m_pMainChar->m_bMode));

					SendCS_ACTION_REQ(
						m_pMainChar->m_dwID,
						m_pMainChar->m_bType,
						TA_LOOT_START,
						vActionID.m_dwActID,
						vActionID.m_dwAniID,
						m_pMainWnd->m_bChannel,
						m_vMAP.m_wMapID, 0);

					DisableTargetUI();
					ResetTMonInvenUI();

					if( m_pTARGET && m_pTARGET->m_bType == OT_NPC )
						((CTClientNpc *) m_pTARGET)->OpenBOX(TRUE);
				}
				else
				{
					if( m_pTARGET && m_pTARGET->m_bType == OT_MON )
						SendCS_MONITEMLIST_REQ( FALSE, m_pTARGET->m_dwID);

					SendCS_ACTION_REQ(
						m_pMainChar->m_dwID,
						m_pMainChar->m_bType,
						0, 0, 0,
						m_pMainWnd->m_bChannel,
						m_vMAP.m_wMapID, 0);
					ClearMonInven();

					return;
				}
			}

			break;

		case TFRAME_CINEMATIC		:
			{
				EnableAllInputDevice(FALSE);
				EnableChat(FALSE);
				DisableTargetUI();
				CloseAllUI();

				m_vTFRAME[TFRAME_GAUGE]->ShowComponent(FALSE);
				m_vTFRAME[TFRAME_MAIN]->ShowComponent(FALSE);
				ResetRecallINFO(FALSE);
			}
			break;

		case TFRAME_TOURNAMENT_GAGE	:
			{
				DisableTargetUI();
				m_vTFRAME[TFRAME_GAUGE]->ShowComponent(FALSE);
				static_cast<CTTournamentGage*>(m_vTFRAME[TFRAME_TOURNAMENT_GAGE])->ResetTournamentINFO();
			}
			break;

		case TFRAME_QUEST			: m_vTFRAME[TFRAME_NPCDIALOG]->ShowComponent(FALSE); break;
		case TFRAME_NPCDIALOG		: m_vTFRAME[TFRAME_QUEST]->ShowComponent(FALSE); break;
		case TFRAME_CRAFT			: m_vTFRAME[TFRAME_NPCDIALOG]->ShowComponent(FALSE); break;

		case TFRAME_ITEM_UP			:
		case TFRAME_ITEM_REPAIR		:
		case TFRAME_ITEM_REFINE		:
		case TFRAME_ITEM_GAMBLE		:
		case TFRAME_ITEM_EXTPERIOD:
			{
				DisableTargetUI();
				
				EnableUI(TFRAME_DEFINVEN);
				EnableAllSubInven(TRUE);
			}

			break;

		case TFRAME_PRIVATE_SELL	:
			{
				CTPrivateShopDlgForSell* pPrvShopDlg = static_cast<CTPrivateShopDlgForSell*>(m_vTFRAME[TFRAME_PRIVATE_SELL]);

				pPrvShopDlg->Clear();
				DisableTargetUI();

				EnableUI(TFRAME_DEFINVEN);
				EnableAllSubInven(TRUE);
			}

			break;

		case TFRAME_MAIL_SEND		: DisableTargetUI(); break;
		case TFRAME_KEYSETTING		: static_cast<CTKeySettingDlg*>(m_vTFRAME[TFRAME_KEYSETTING])->NotifyUpdate(); break;

		case TFRAME_PET_MANAGE		:
			{
				if( m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
					return ;
			}
			break;
		case TFRAME_CASHCABINET		:
			{
				static_cast<CTMainUI*>(m_vTFRAME[ TFRAME_MAIN ])->SetNewCashCabinet(FALSE);
			}
			break;

		case TFRAME_TOURNAMENT					:
			{
				CTTournament* pDLG = static_cast<CTTournament*>(m_vTFRAME[bUI]);
				pDLG->m_pMATCHLIST->RemoveAll();
				pDLG->m_pENTRY_PLAYER_LIST->RemoveAll();
				pDLG->m_pENTRY_PARTY_LIST->RemoveAll();

				m_pSESSION->SendCS_TOURNAMENTSCHEDULE_REQ();
				m_pSESSION->SendCS_TOURNAMENTAPPLYINFO_REQ();
			}

			break;

		case TFRAME_TOURNAMENT_EVENT			:
			{
				CTTournamentEvent* pDLG = static_cast<CTTournamentEvent*>(m_vTFRAME[bUI]);
				pDLG->SetMode( TTEVENT_SELECT_EVENT );
				pDLG->m_pEVENTLIST->RemoveAll();
				m_pSESSION->SendCS_TOURNAMENTEVENTLIST_REQ();
			}
			break;

		case TFRAME_TOURNAMENT_SELECT_TARGET	:
			{
				CTTournamentSelectTarget* pDLG = static_cast<CTTournamentSelectTarget*>(m_vTFRAME[bUI]);
				pDLG->OnEnableUI();
			}
			break;

		case TFRAME_FAME_BASE					:
			{
				CTFameBaseDlg* pDLG = static_cast<CTFameBaseDlg*>( m_vTFRAME[bUI] );
				pDLG->OnEnable();
			}
			break;
#ifdef ADD_RPS
		case TFRAME_RPS:
			{
				m_bRPSKeyEnable = TRUE;
			}
			break;
#endif
		}

		m_vTFRAME[bUI]->ShowComponent(TRUE);
		SwitchFocus(m_vTFRAME[bUI]);
	}
}

void CTClientGame::ResetTargetOBJ( CTClientObjBase *pTARGET)
{
	if( pTARGET && !pTARGET->m_bSelectable )
		return;

	if( m_pTARGET != pTARGET )
	{
		if( m_bMoveType == TMAINMOVE_TARGET )
		{
			m_bMoveType = TMAINMOVE_NORMAL;
			m_dwMoveGM = 0;
		}

		if( m_bCHARGE && m_pACT && (m_pACT->m_bType == OT_NPC || m_pACT->m_bType == OT_SWITCH) )
		{
			SendCS_ACTION_REQ(
				m_pMainChar->m_dwID,
				m_pMainChar->m_bType,
				0, 0, 0,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID, 0);
			CancelCharge(TRUE);
		}

		if(pTARGET)
			CancelTrade();

		DisableTargetUI();
		LoopSkillOFF();

		m_pTARGET = pTARGET;
		m_wNextSkillID = 0;

		CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
		pTGaugePannel->ResetHotkeyUI();
	}

	if( m_pTARGET )
	{
		MAPTSELECTOBJ* pTSELMAP;
		if(m_pTARGET->IsDead())
			pTSELMAP = &CTClientObjBase::m_mapTSELECTDEADOBJ;
		else
			pTSELMAP = &CTClientObjBase::m_mapTSELECTOBJ;

		MAPTSELECTOBJ::iterator finder = pTSELMAP->find(m_pTARGET);

		if( finder == pTSELMAP->end() )
			pTSELMAP->insert( MAPTSELECTOBJ::value_type( m_pTARGET, m_pTARGET));

		if( m_pTARGET->m_bType == OT_PC &&
			(!m_pTPARTY  || m_pTPARTY->m_dwCharID != m_pTARGET->m_dwID) )
		{
			ResetPartyOBJ(NULL);

			if( GetPartyID(m_pMainChar) && GetPartyID(m_pTARGET) )
			{
				for(int i=0; i<GetPartyCnt(); ++i)
				{
					LPTPARTY pPARTY = GetParty(i);
					if( pPARTY->m_dwCharID == m_pTARGET->m_dwID )
					{
						ResetPartyOBJ(pPARTY);
						break;
					}
				}
			}
		}
	}
	else
	{
		CTClientObjBase::m_mapTSELECTDEADOBJ.clear();
		CTClientObjBase::m_mapTSELECTOBJ.clear();
	}
}

CTClientObjBase* CTClientGame::GetSkillTarget(CTClientObjBase* pTARGET, LPTSKILL pTSKILL)
{
	if( !pTSKILL )
		return NULL;

	if( pTARGET && ((pTARGET->m_bType == OT_RECALL || pTARGET->m_bType == OT_SELF) &&
		((CTClientRecall *) pTARGET)->m_bRecallType == TRECALLTYPE_MINE &&
		pTSKILL->m_bTargetType != TSKILLTARGET_MINE) || (
		pTSKILL->m_bRangeType == TSKILLRANGE_MCIRCLE) )
	{
		pTARGET = NULL;
	}

	switch( pTSKILL->m_bTargetType )
	{
	case TSKILLTARGET_CORPS			:
	case TSKILLTARGET_GUILD			:
	case TSKILLTARGET_SELF			: 
		pTARGET = m_pMainChar; 
		break;

	case TSKILLTARGET_MAINRECALL	:
	case TSKILLTARGET_RECALL		:
		{
			pTARGET = GetMainRecall();

			if( pTARGET && !((CTClientRecall *) pTARGET)->m_pTEMP->m_dwOBJ )
				pTARGET = NULL;
		}

		break;

	case TSKILLTARGET_ALLIANCE		:
		{
			BOOL bAlliance = FALSE;
			
			if( pTARGET )
				bAlliance = m_pMainChar->IsAlliance(pTARGET);

			// TARGET이 없거나 적국이면 스킬시전의 대상을 자신으로 한다.
			// TARGET이 동맹이면 PC,MON,RECALL,SELF가 아닌 경우(SWITCH나 GATE같은) 스킬시전의 대상을 자신으로 한다.
			// TARGET이 펫이면 대상을 자신으로 한다.
			if( !pTARGET ||
				!bAlliance ||
				(bAlliance && (pTARGET->m_bType != OT_PC && pTARGET->m_bType != OT_MON && pTARGET->m_bType != OT_RECALL && pTARGET->m_bType != OT_SELF)) ||
				((pTARGET->m_bType == OT_RECALL) && ( ((CTClientRecall*)(pTARGET))->m_bRecallType == TRECALLTYPE_PET)) )
				pTARGET = m_pMainChar;
		}
		break;

	case TSKILLTARGET_PARTYEXCEPTME	:
		{
			WORD wPartyID = GetPartyID(m_pMainChar);

			if( !pTARGET || pTARGET == m_pMainChar || !wPartyID || wPartyID != GetPartyID(pTARGET) )
				pTARGET = NULL;
		}
		break;

	case TSKILLTARGET_PARTY			:
		{
			WORD wPartyID = GetPartyID(m_pMainChar);

			if( !pTARGET || !wPartyID || wPartyID != GetPartyID(pTARGET) )
				pTARGET = m_pMainChar;
		}
		break;

	case TSKILLTARGET_ENEMY			:
		if( pTARGET && m_pMainChar->IsAlliance(pTARGET) )
			pTARGET = NULL;
		break;

	case TSKILLTARGET_NONE			: 
		pTARGET = NULL; 
		break;
	}

	return pTARGET;
}

void CTClientGame::ResetActOBJ( LPTSKILL pTSKILL)
{
	CancelCharge(TRUE);
	
	if( pTSKILL )
		m_pACT = GetSkillTarget(m_pTARGET,pTSKILL);
	else
		m_pACT = m_pTARGET;
}

void CTClientGame::ResetPartyOBJ( LPTPARTY pTPARTY)
{
	m_pTPARTY = pTPARTY;
}

void CTClientGame::ResetPlayerINFO( BYTE bContinue)
{
	BYTE bHUD = GetHUDOption();

	m_vTGAUGE[TGAUGE_PLAYER_HP]->SetGauge(
		m_pMainChar->m_dwHP,
		m_pMainChar->m_dwMaxHP,
		bContinue);

	m_vTTEXT[TTEXT_PLAYER_HP]->m_strText = CTChart::Format( TSTR_FMT_GAUGE,
		m_pMainChar->m_dwHP,
		m_pMainChar->m_dwMaxHP);

	m_vTGAUGE[TGAUGE_PLAYER_MP]->SetGauge(
		m_pMainChar->m_dwMP,
		m_pMainChar->m_dwMaxMP,
		bContinue);

	m_vTTEXT[TTEXT_PLAYER_MP]->m_strText = CTChart::Format( TSTR_FMT_GAUGE,
		m_pMainChar->m_dwMP,
		m_pMainChar->m_dwMaxMP);

	m_vTGAUGE[TGAUGE_PLAYER_EXP]->SetGauge(
		m_pMainChar->GetEXP() - m_pMainChar->m_dwPrevEXP,
		m_pMainChar->m_dwNextEXP - m_pMainChar->m_dwPrevEXP,
		bContinue);

	m_vTTEXT[TTEXT_PLAYER_NAME]->m_strText = CTChart::Format( TSTR_FMT_NAME,
		m_pMainChar->GetName());

	CString strCLASS;
	strCLASS = CTChart::LoadString( (TSTRING) m_vTCLASSSTR[m_pMainChar->m_bClassID] );

	m_vTTEXT[TTEXT_PLAYER_LEVEL]->m_strText = CTChart::Format( TSTR_FMT_LEVEL,
		m_pMainChar->m_bLevel);

	m_vTTEXT[TTEXT_PLAYER_CLASS]->m_strText = strCLASS;

	m_vTTEXT[TTEXT_PLAYER_EXP]->m_strText = CTChart::Format( TSTR_FMT_EXPTEXT,
		m_pMainChar->GetEXP(),
		m_pMainChar->m_dwNextEXP);

	if( bHUD )
	{
		m_vTHUDGAUGE[THUD_GAUGE_MY_HP]->SetGauge(
			m_pMainChar->m_dwHP,
			m_pMainChar->m_dwMaxHP,
			bContinue);

		m_vTHUDGAUGE[THUD_GAUGE_MY_MP]->SetGauge(
			m_pMainChar->m_dwMP,
			m_pMainChar->m_dwMaxMP,
			bContinue);

		m_vTHUDTEXT[THUD_TEXT_MY_HP]->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			m_pMainChar->m_dwHP);

		m_vTHUDTEXT[THUD_TEXT_MY_MP]->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			m_pMainChar->m_dwMP);
	}

	m_vTHUDGAUGE[THUD_GAUGE_MY_HP]->ShowComponent(bHUD);
	m_vTHUDGAUGE[THUD_GAUGE_MY_MP]->ShowComponent(bHUD);
	m_vTHUDBASE[THUD_BASE_MY_HP]->ShowComponent(bHUD);
	m_vTHUDBASE[THUD_BASE_MY_MP]->ShowComponent(bHUD);
	m_vTHUDTEXT[THUD_TEXT_MY_HP]->ShowComponent(bHUD);
	m_vTHUDTEXT[THUD_TEXT_MY_MP]->ShowComponent(bHUD);
}

void CTClientGame::ResetTargetINFO( BYTE bContinue)
{
	if( m_pTARGET )
		ResetTargetINFO( m_pTARGET, bContinue);
	else
		ResetTargetINFO( (CTClientObjBase*) NULL, bContinue);

	ResetHUDTargetINFO(bContinue);
	ResetLeaderMark();
}

void CTClientGame::ResetTargetINFO( CTClientObjBase *pTARGET, BYTE bContinue)
{
	m_vTFACEIMG[TFACEIMG_TARGETMON_N]->ShowComponent(FALSE);
	m_vTFACEIMG[TFACEIMG_TARGETMON_S]->ShowComponent(FALSE);
	m_vTFACEIMG[TFACEIMG_TARGETPC]->ShowComponent(FALSE);

	if( pTARGET && pTARGET->m_bType != OT_SWITCH )
	{
		m_vTTEXT[TTEXT_TARGET_NAME]->m_strText = CTChart::Format( TSTR_FMT_NAME,
			pTARGET->GetName());

		m_vTTEXT[TTEXT_TARGET_CLASS]->m_strText.Empty();

		m_vTPANNEL[TPANNEL_TARGET]->ShowComponent(TRUE);
		m_vTLEVELIMG[TLEVELIMG_TARGET]->ShowComponent(TRUE);
		m_vTFACEBACKIMG[TFACEBACKIMG_TARGET]->ShowComponent(TRUE);
		m_vTGAUGE[TGAUGE_TARGET_HP]->ShowComponent(TRUE);
		m_vTGAUGE[TGAUGE_TARGET_VITAL]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_TARGET_NAME]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_TARGET_HP]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_TARGET_CLASS]->ShowComponent(TRUE);

		if(pTARGET->m_dwMaxHP)
		{
			m_vTGAUGE[TGAUGE_TARGET_HP]->SetGauge(
				pTARGET->m_dwHP,
				pTARGET->m_dwMaxHP,
				bContinue);

			m_vTTEXT[TTEXT_TARGET_HP]->m_strText = CTChart::Format( TSTR_FMT_GAUGE,
				pTARGET->m_dwHP,
				pTARGET->m_dwMaxHP);
		}
		else
		{
			m_vTGAUGE[TGAUGE_TARGET_HP]->SetGauge( 100, 100, FALSE);
			m_vTTEXT[TTEXT_TARGET_HP]->m_strText.Empty();
		}

		m_vTGAUGE[TGAUGE_TARGET_VITAL]->SetGauge(
			min(TMAX_VITAL, pTARGET->GetAftermathStep()),
			TMAX_VITAL);

		if( pTARGET->m_bLevel )
		{
			m_vTTEXT[TTEXT_TARGET_LEVEL]->m_strText = CTChart::Format( TSTR_FMT_LEVEL,
				pTARGET->m_bLevel);
			m_vTTEXT[TTEXT_TARGET_LEVEL]->ShowComponent(TRUE);
		}
		else
			m_vTTEXT[TTEXT_TARGET_LEVEL]->ShowComponent(FALSE);

		switch(pTARGET->m_bType)
		{
		case OT_PC		:
			{
				m_vTFACEIMG[TFACEIMG_TARGETPC]->SetCurImage(GetFACE(
					((CTClientChar *) pTARGET)->GetRaceID(),
					((CTClientChar *) pTARGET)->m_bSex,
					((CTClientChar *) pTARGET)->m_bFace,
					((CTClientChar *) pTARGET)->m_bHair));

				m_vTFACEIMG[TFACEIMG_TARGETPC]->ShowComponent(TRUE);

				m_vTTEXT[TTEXT_TARGET_CLASS]->m_strText = CTChart::LoadString(
					(TSTRING) CTClientGame::m_vTCLASSSTR[ ((CTClientChar*)pTARGET)->m_bClassID ] );
			}

			break;

		case OT_RECALL	:
		case OT_SELF	:
			{
				LPTMONTEMP pTMON = ((CTClientRecall*) pTARGET)->m_pTEMP;

				if( pTMON->m_dwOBJ == 0 )
				{
					CTClientFake* pTFAKE = (CTClientFake*)pTARGET;
                    m_vTFACEIMG[TFACEIMG_TARGETPC]->SetCurImage(GetFACE(
						pTFAKE->m_bRaceID,
						pTFAKE->m_bSex,
						pTFAKE->m_bFace,
						pTFAKE->m_bHair));

					m_vTFACEIMG[TFACEIMG_TARGETPC]->ShowComponent(TRUE);
				}
				else if( CTChart::m_setTRACEOBJID.find(pTMON->m_dwOBJ) != CTChart::m_setTRACEOBJID.end() )
				{
					m_vTFACEIMG[TFACEIMG_TARGETMON_N]->SetCurImage(pTMON->m_wFaceIcon);
					m_vTFACEIMG[TFACEIMG_TARGETMON_N]->ShowComponent(TRUE);
				}
				else
				{
					m_vTFACEIMG[TFACEIMG_TARGETMON_S]->SetCurImage(pTMON->m_wFaceIcon);
					m_vTFACEIMG[TFACEIMG_TARGETMON_S]->ShowComponent(TRUE);
				}
			}

			break;

		case OT_NPC		:
			{
				LPTNPCTEMP pTNPC = ((CTClientNpc*) pTARGET)->m_pTNPC;

				if( CTChart::m_setTRACEOBJID.find(pTNPC->m_dwOBJ) != CTChart::m_setTRACEOBJID.end() )
				{
					m_vTFACEIMG[TFACEIMG_TARGETMON_N]->SetCurImage(pTNPC->m_wFaceIcon);
					m_vTFACEIMG[TFACEIMG_TARGETMON_N]->ShowComponent(TRUE);
				}
				else
				{
					m_vTFACEIMG[TFACEIMG_TARGETMON_S]->SetCurImage(pTNPC->m_wFaceIcon);
					m_vTFACEIMG[TFACEIMG_TARGETMON_S]->ShowComponent(TRUE);
				}
			}	

			break;

		case OT_MON		:
			{
				LPTMONTEMP pTMON = ((CTClientMonster*) pTARGET)->m_pMON;

				if( CTChart::m_setTRACEOBJID.find(pTMON->m_dwOBJ) != CTChart::m_setTRACEOBJID.end() )
				{
					m_vTFACEIMG[TFACEIMG_TARGETMON_N]->SetCurImage(pTMON->m_wFaceIcon);
					m_vTFACEIMG[TFACEIMG_TARGETMON_N]->ShowComponent(TRUE);
				}
				else
				{
					m_vTFACEIMG[TFACEIMG_TARGETMON_S]->SetCurImage(pTMON->m_wFaceIcon);
					m_vTFACEIMG[TFACEIMG_TARGETMON_S]->ShowComponent(TRUE);
				}
			}
			break;

		case OT_GODTOWER	:
			{
				LPTGODTOWER pTowerInfo =  static_cast< CTGodTower* >( pTARGET )->GetTowerInfo();

				if( pTowerInfo )
				{
					m_vTFACEIMG[TFACEIMG_TARGETMON_S]->SetCurImage( pTowerInfo->m_dwIconID );
					m_vTFACEIMG[TFACEIMG_TARGETMON_S]->ShowComponent( TRUE );
				}
			}
			break;
		case OT_GODBALL		:
			{
				LPTGODBALL pBallInfo = static_cast< CTGodBall*>( pTARGET )->GetBallInfo();
				if( pBallInfo )
				{
					m_vTFACEIMG[ TFACEIMG_TARGETMON_S ]->SetCurImage( pBallInfo->m_dwIconID );
					m_vTFACEIMG[ TFACEIMG_TARGETMON_S ]->ShowComponent( TRUE );
				}
			}
			break;
		}
	}
	else
	{
		m_vTPANNEL[TPANNEL_TARGET]->ShowComponent(FALSE);
		m_vTLEVELIMG[TLEVELIMG_TARGET]->ShowComponent(FALSE);
		m_vTFACEBACKIMG[TFACEBACKIMG_TARGET]->ShowComponent(FALSE);
		m_vTGAUGE[TGAUGE_TARGET_HP]->ShowComponent(FALSE);
		m_vTGAUGE[TGAUGE_TARGET_VITAL]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_TARGET_NAME]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_TARGET_LEVEL]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_TARGET_HP]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_TARGET_CLASS]->ShowComponent(FALSE);
	}
}

void CTClientGame::ResetTargetINFO( LPTPARTY pTPARTY, BYTE bContinue)
{
	m_vTFACEIMG[TFACEIMG_TARGETMON_N]->ShowComponent(FALSE);
	m_vTFACEIMG[TFACEIMG_TARGETMON_S]->ShowComponent(FALSE);
	m_vTFACEIMG[TFACEIMG_TARGETPC]->ShowComponent(FALSE);

	if( pTPARTY )
	{
		m_vTPANNEL[TPANNEL_TARGET]->ShowComponent(TRUE);
		m_vTLEVELIMG[TLEVELIMG_TARGET]->ShowComponent(TRUE);
		m_vTFACEBACKIMG[TFACEBACKIMG_TARGET]->ShowComponent(TRUE);
		m_vTGAUGE[TGAUGE_TARGET_HP]->ShowComponent(TRUE);
		m_vTGAUGE[TGAUGE_TARGET_VITAL]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_TARGET_NAME]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_TARGET_HP]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_TARGET_LEVEL]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_TARGET_CLASS]->ShowComponent(TRUE);
		m_vTFACEIMG[TFACEIMG_TARGETPC]->ShowComponent(TRUE);

		m_vTTEXT[TTEXT_TARGET_NAME]->m_strText = CTChart::Format( TSTR_FMT_NAME,
			pTPARTY->m_strNAME);

		if(pTPARTY->m_dwMaxHP)
		{
			m_vTGAUGE[TGAUGE_TARGET_HP]->SetGauge(
				pTPARTY->m_dwHP,
				pTPARTY->m_dwMaxHP,
				bContinue);

			m_vTTEXT[TTEXT_TARGET_HP]->m_strText = CTChart::Format( TSTR_FMT_GAUGE,
				pTPARTY->m_dwHP,
				pTPARTY->m_dwMaxHP);
		}
		else
		{
			m_vTGAUGE[TGAUGE_TARGET_HP]->SetGauge( 100, 100, FALSE);
			m_vTTEXT[TTEXT_TARGET_HP]->m_strText.Empty();
		}

		m_vTGAUGE[TGAUGE_VITAL]->SetGauge(
			min(TMAX_VITAL, 0),
			TMAX_VITAL);

		m_vTTEXT[TTEXT_TARGET_LEVEL]->m_strText = CTChart::Format( TSTR_FMT_LEVEL,
			pTPARTY->m_bLevel);

		m_vTFACEIMG[TFACEIMG_TARGETPC]->SetCurImage(GetFACE(
			pTPARTY->m_bRaceID,
			pTPARTY->m_bSexID,
			pTPARTY->m_bFaceID,
			pTPARTY->m_bHairID));
	}
	else
	{
		m_vTPANNEL[TPANNEL_TARGET]->ShowComponent(FALSE);
		m_vTLEVELIMG[TLEVELIMG_TARGET]->ShowComponent(FALSE);
		m_vTFACEBACKIMG[TFACEBACKIMG_TARGET]->ShowComponent(FALSE);
		m_vTGAUGE[TGAUGE_TARGET_HP]->ShowComponent(FALSE);
		m_vTGAUGE[TGAUGE_TARGET_VITAL]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_TARGET_NAME]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_TARGET_LEVEL]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_TARGET_HP]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_TARGET_CLASS]->ShowComponent(FALSE);
	}
}

void CTClientGame::ResetHUDTargetINFO( BYTE bContinue)
{
	BOOL bSHOW = GetHUDOption() && 
				 m_pTARGET && 
				 m_pTARGET->m_bType != OT_SWITCH &&
				 m_pTARGET->m_dwMaxHP &&
				 !m_pMainChar->IsAlliance(m_pTARGET);

	if( bSHOW )
	{
		m_vTHUDGAUGE[THUD_GAUGE_OP_HP]->SetGauge(
			m_pTARGET->m_dwHP,
			m_pTARGET->m_dwMaxHP,
			bContinue);

		m_vTHUDTEXT[THUD_TEXT_OP_HP]->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
			m_pTARGET->m_dwHP);
	}

	m_vTHUDBASE[THUD_BASE_OP_HP]->ShowComponent(bSHOW);
	m_vTHUDTEXT[THUD_TEXT_OP_HP]->ShowComponent(bSHOW);
	m_vTHUDGAUGE[THUD_GAUGE_OP_HP]->ShowComponent(bSHOW);

	m_vTHUDBASE[THUD_BASE_OP_MP]->ShowComponent(FALSE);
	m_vTHUDTEXT[THUD_TEXT_OP_MP]->ShowComponent(FALSE);
	m_vTHUDGAUGE[THUD_GAUGE_OP_MP]->ShowComponent(FALSE);
}

void CTClientGame::ResetPartyINFO( BYTE bContinue)
{
	static BYTE vHP[] = {
		TGAUGE_PARTY1_HP,
		TGAUGE_PARTY2_HP,
		TGAUGE_PARTY3_HP,
		TGAUGE_PARTY4_HP,
		TGAUGE_PARTY5_HP,
		TGAUGE_PARTY6_HP};

	static BYTE vTHP[] = {
		TTEXT_PARTY1_HP,
		TTEXT_PARTY2_HP,
		TTEXT_PARTY3_HP,
		TTEXT_PARTY4_HP,
		TTEXT_PARTY5_HP,
		TTEXT_PARTY6_HP};

	static BYTE vTCLASS[] = {
		TTEXT_PARTY1_CLASS,
		TTEXT_PARTY2_CLASS,
		TTEXT_PARTY3_CLASS,
		TTEXT_PARTY4_CLASS,
		TTEXT_PARTY5_CLASS,
		TTEXT_PARTY6_CLASS};

	static BYTE vMP[] = {
		TGAUGE_PARTY1_MP,
		TGAUGE_PARTY2_MP,
		TGAUGE_PARTY3_MP,
		TGAUGE_PARTY4_MP,
		TGAUGE_PARTY5_MP,
		TGAUGE_PARTY6_MP};

	static BYTE vPANNEL[] = {
		TPANNEL_PARTY1,
		TPANNEL_PARTY2,
		TPANNEL_PARTY3,
		TPANNEL_PARTY4,
		TPANNEL_PARTY5,
		TPANNEL_PARTY6};

	static BYTE vNAME[] = {
		TTEXT_PARTY1_NAME,
		TTEXT_PARTY2_NAME,
		TTEXT_PARTY3_NAME,
		TTEXT_PARTY4_NAME,
		TTEXT_PARTY5_NAME,
		TTEXT_PARTY6_NAME};

	static BYTE vLEVEL[] = {
		TTEXT_PARTY1_LEVEL,
		TTEXT_PARTY2_LEVEL,
		TTEXT_PARTY3_LEVEL,
		TTEXT_PARTY4_LEVEL,
		TTEXT_PARTY5_LEVEL,
		TTEXT_PARTY6_LEVEL};

	static BYTE vFACEIMG[] = {
		TFACEIMG_PARTY1,
		TFACEIMG_PARTY2,
		TFACEIMG_PARTY3,
		TFACEIMG_PARTY4,
		TFACEIMG_PARTY5,
		TFACEIMG_PARTY6};

	static BYTE vLEVELIMG[] = {
		TLEVELIMG_PARTY1,
		TLEVELIMG_PARTY2,
		TLEVELIMG_PARTY3,
		TLEVELIMG_PARTY4,
		TLEVELIMG_PARTY5,
		TLEVELIMG_PARTY6};          

	static BYTE vFACEBACKIMG[] = {
		TFACEBACKIMG_PARTY1,
		TFACEBACKIMG_PARTY2,
		TFACEBACKIMG_PARTY3,
		TFACEBACKIMG_PARTY4,
		TFACEBACKIMG_PARTY5,
		TFACEBACKIMG_PARTY6};        

	BYTE bCount = INT(m_vTPARTY.size());

	for( BYTE i=0; i<MAX_PARTY; i++)
	{
		if( i < bCount )
		{
			m_vTFACEIMG[vFACEIMG[i]]->SetCurImage(GetFACE(
				m_vTPARTY[i]->m_bRaceID,
				m_vTPARTY[i]->m_bSexID,
				m_vTPARTY[i]->m_bFaceID,
				m_vTPARTY[i]->m_bHairID));

			m_vTTEXT[ vNAME[i] ]->m_strText = CTChart::Format( TSTR_FMT_NAME,
				m_vTPARTY[i]->m_strNAME);

			m_vTTEXT[ vTCLASS[i] ]->m_strText = CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ m_vTPARTY[i]->m_bClass ] );

			m_vTTEXT[vLEVEL[i]]->m_strText = CTChart::Format( TSTR_FMT_LEVEL,
				m_vTPARTY[i]->m_bLevel);

			m_vTGAUGE[vHP[i]]->SetGauge(
				m_vTPARTY[i]->m_dwHP,
				m_vTPARTY[i]->m_dwMaxHP,
				bContinue);

			m_vTTEXT[vTHP[i]]->m_strText = CTChart::Format( TSTR_FMT_GAUGE,
				m_vTPARTY[i]->m_dwHP,
				m_vTPARTY[i]->m_dwMaxHP);

			m_vTGAUGE[vMP[i]]->SetGauge(
				m_vTPARTY[i]->m_dwMP,
				m_vTPARTY[i]->m_dwMaxMP,
				bContinue);

			m_vTFACEIMG[vFACEIMG[i]]->ShowComponent(TRUE);
			m_vTPANNEL[vPANNEL[i]]->ShowComponent(TRUE);
			m_vTLEVELIMG[vLEVELIMG[i]]->ShowComponent(TRUE);
			m_vTFACEBACKIMG[vFACEBACKIMG[i]]->ShowComponent(TRUE);
			m_vTTEXT[vLEVEL[i]]->ShowComponent(TRUE);
			m_vTTEXT[vNAME[i]]->ShowComponent(TRUE);
			m_vTTEXT[vTHP[i]]->ShowComponent(TRUE);
			m_vTTEXT[vTCLASS[i]]->ShowComponent(TRUE);
			m_vTGAUGE[vHP[i]]->ShowComponent(TRUE);
			m_vTGAUGE[vMP[i]]->ShowComponent(TRUE);
		}
		else
		{
			m_vTFACEIMG[vFACEIMG[i]]->ShowComponent(FALSE);
			m_vTPANNEL[vPANNEL[i]]->ShowComponent(FALSE);
			m_vTLEVELIMG[vLEVELIMG[i]]->ShowComponent(FALSE);
			m_vTFACEBACKIMG[vFACEBACKIMG[i]]->ShowComponent(FALSE);
			m_vTTEXT[vLEVEL[i]]->ShowComponent(FALSE);
			m_vTTEXT[vNAME[i]]->ShowComponent(FALSE);
			m_vTTEXT[vTHP[i]]->ShowComponent(FALSE);
			m_vTTEXT[vTCLASS[i]]->ShowComponent(FALSE);
			m_vTGAUGE[vHP[i]]->ShowComponent(FALSE);
			m_vTGAUGE[vMP[i]]->ShowComponent(FALSE);
			m_vTMAINTAIN[TPANNEL_PARTY1 + i]->HideAll();
		}
	}

	if( bCount )
	{
		switch( m_bPartyItemType )
		{
		case PT_FREE	: m_pTPARTYITEM->m_strText = CTChart::LoadString( TSTR_FMT_PARTY_ITEM_FREE);		break;
		case PT_HUNTER	: m_pTPARTYITEM->m_strText = CTChart::LoadString( TSTR_FMT_PARTY_ITEM_HUNTER);	break;
		case PT_LOTTERY	: m_pTPARTYITEM->m_strText = CTChart::LoadString( TSTR_FMT_PARTY_ITEM_LOTTERY);	break;
		case PT_CHIEF	: m_pTPARTYITEM->m_strText = CTChart::LoadString( TSTR_FMT_PARTY_ITEM_CHIEF);	break;
		case PT_ORDER	: m_pTPARTYITEM->m_strText = CTChart::LoadString( TSTR_FMT_PARTY_ITEM_SEQ);	break;
		default			: m_pTPARTYITEM->m_strText.Empty();
		}

		if( !m_pTPARTYITEM->m_strText.IsEmpty() )
		{
			m_pTPARTYITEM->EnableComponent(m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID);
			m_pTPARTYITEM->ShowComponent(TRUE);
		}
	}
	else
		m_pTPARTYITEM->ShowComponent(FALSE);

	ResetLeaderMark();
}

BYTE CTClientGame::ResetRSCS( BYTE bRSCS)
{
	CTGaugePannel *pTPANNEL = (CTGaugePannel *) m_vTFRAME[TFRAME_GAUGE];
	WORD wPartyID = GetPartyID(m_pMainChar);

	m_bRSCS = wPartyID && bRSCS ? TRUE : FALSE;
	m_pTRSCS->ShowComponent(m_bRSCS);

	if( pTPANNEL && pTPANNEL->m_pQList )
		pTPANNEL->m_pQList->ShowComponent(!m_bRSCS);

	if(bRSCS)
		m_pTRSCS->ResetCORPS();

	return wPartyID ? TRUE : FALSE;
}

void CTClientGame::ResetTSKY()
{
	FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
	BYTE bUnitX = BYTE(m_pMainChar->GetPositionX() / fUnitLength);
	BYTE bUnitZ = BYTE(m_pMainChar->GetPositionZ() / fUnitLength);

	LPTSKYBOX pTFAR = CTChart::FindTSKYBOX(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TSKYOBJ_FAR,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	LPTSKYBOX pTSKY = CTChart::FindTSKYBOX(
		m_vMAP.m_pMAP->m_nUnitCountX,
		TSKYOBJ_SKY,
		m_vMAP.m_wMapID,
		bUnitX,
		bUnitZ);

	m_wTSKY = MAKEWORD( bUnitX, bUnitZ);
	m_bTSKY = 0;
	m_dwTSKY = 0;

	m_vFARIMAGE[m_bTSKY].Release();
	m_vSKY[m_bTSKY].Release();

	if(pTFAR)
	{
		m_vFARIMAGE[m_bTSKY].InitOBJ(m_pRES->GetOBJ(pTFAR->m_dwOBJ));
		m_vFARIMAGE[m_bTSKY].m_vScale._11 = TSKY_SCALE;
		m_vFARIMAGE[m_bTSKY].m_vScale._22 = TSKY_SCALE;
		m_vFARIMAGE[m_bTSKY].m_vScale._33 = TSKY_SCALE;
		m_vFARIMAGE[m_bTSKY].SetCloth(
			m_pDevice,
			pTFAR->m_dwCLK,
			pTFAR->m_dwCL,
			pTFAR->m_dwMESH);
		m_vFARIMAGE[m_bTSKY].ResetOBJPart(m_pDevice);

		m_vFARIMAGE[m_bTSKY].SetAction(
			pTFAR->m_dwACT,
			pTFAR->m_dwANI);

		m_vFARIMAGE[m_bTSKY].m_bAlpha = ALPHA_MAX;
		m_vFARIMAGE[m_bTSKY].m_bHide = FALSE;
	}

	if(pTSKY)
	{
		m_vSKY[m_bTSKY].InitOBJ(m_pRES->GetOBJ(pTSKY->m_dwOBJ));
		m_vSKY[m_bTSKY].m_vScale._11 = TSKY_SCALE;
		m_vSKY[m_bTSKY].m_vScale._22 = TSKY_SCALE;
		m_vSKY[m_bTSKY].m_vScale._33 = TSKY_SCALE;
		m_vSKY[m_bTSKY].SetCloth(
			m_pDevice,
			pTSKY->m_dwCLK,
			pTSKY->m_dwCL,
			pTSKY->m_dwMESH);
		m_vSKY[m_bTSKY].ResetOBJPart(m_pDevice);

		m_vSKY[m_bTSKY].SetAction(
			pTSKY->m_dwACT,
			pTSKY->m_dwANI);

		m_vSKY[m_bTSKY].m_bAlpha = ALPHA_MAX;
		m_vSKY[m_bTSKY].m_bHide = FALSE;
	}

	m_vFARIMAGE[!m_bTSKY].m_bHide = TRUE;
	m_vSKY[!m_bTSKY].m_bHide = TRUE;
}

void CTClientGame::ResetTMAINTAIN()
{
	ResetTMAINTAIN( m_pMainChar, m_vTMAINTAIN[TPANNEL_PLAYER]);
	ResetTMAINTAIN( m_pTARGET, m_vTMAINTAIN[TPANNEL_TARGET]);
	ResetTMAINTAIN( GetMainRecall(), m_vTMAINTAIN[TPANNEL_SUMMON]);

	for( BYTE i=0; i<INT(m_vTPARTY.size()); i++)
		ResetTMAINTAIN( FindPlayer(m_vTPARTY[i]->m_dwCharID), m_vTMAINTAIN[TPANNEL_PARTY1 + i]);

	if( m_bTournamentWatching )
		static_cast<CTTournamentGage*>( m_vTFRAME[TFRAME_TOURNAMENT_GAGE] )->ResetTMAINTAIN();
}

void CTClientGame::ResetTMAINTAIN( CTClientObjBase *pTOBJ, CTMaintainPannel *pUI)
{
	if(pTOBJ)
		pUI->ResetMAINTAIN( pTOBJ, m_vKEY.m_dwTick);
	else
		pUI->HideAll();
}

void CTClientGame::ResetGroundPOS( int nPosX,
								   int nPosY)
{
	D3DXVECTOR3 vPOS = m_pCamera->GetCameraPosition( nPosX, nPosY);
	D3DXVECTOR3 vDIR = m_pCamera->GetRayDirection( nPosX, nPosY);
	FLOAT fDIST = 0.0f;

	vDIR /= D3DXVec3Length(&vDIR);
	D3DXVECTOR3 vTARGET = vPOS + TARGET_DIST * vDIR;

	if(m_vMAP.HitTest( &fDIST, &vPOS, &vTARGET, 0.0f, 0.0f, TRUE))
	{
		m_vTGROUND = vPOS + fDIST * vDIR;
		vPOS = m_vTGROUND;
		vPOS.y += m_pMainChar->m_fSizeY;

		FLOAT fLandH = m_vMAP.GetLandHeight(
			m_vTGROUND.x,
			m_vTGROUND.y,
			m_vTGROUND.z);

		m_vTGROUND.y = m_vMAP.GetHeight(
			m_pMainChar,
			NULL,
			&vPOS,
			0.0f,
			FALSE);

		m_bLAND = fabs(m_vTGROUND.y - fLandH) < THEIGHT_ZERO ? TRUE : FALSE;
		m_bDrawGROUND = TRUE;
	}
}

void CTClientGame::ResetTPOINTER( int nPosX,
								  int nPosY)
{
	D3DXVECTOR3 vPOS = m_pCamera->GetCameraPosition( nPosX, nPosY);
	D3DXVECTOR3 vDIR = m_pCamera->GetRayDirection( nPosX, nPosY);
	FLOAT fDIST = 0.0f;

	vDIR /= D3DXVec3Length(&vDIR);
	D3DXVECTOR3 vTARGET = vPOS + TARGET_DIST * vDIR;

	if(m_vMAP.HitTest( &fDIST, &vPOS, &vTARGET, 0.0f, 0.0f, FALSE))
	{
		vPOS += fDIST * vDIR;
		vPOS.y += m_pMainChar->m_fSizeY;

		vPOS.y = m_vMAP.GetHeight(
			m_pMainChar,
			NULL,
			&vPOS,
			0.0f,
			FALSE);

		D3DXMatrixTranslation(
			&m_vTPOINTER.m_vGlobal,
			vPOS.x,
			vPOS.y,
			vPOS.z);

		m_vTPOINTER.InitSFX(
			m_pDevice,
			m_pTPOINTER);
	}
}

bool compare_hit_dead_mon(CTClientMonster* pLM, CTClientMonster* pRM)
{
	return pLM->m_fZValue < pLM->m_fZValue;
}

void CTClientGame::ResetHitOBJ( CPoint point, BOOL bChkDeadMon)
{
	m_pHIT = NULL;

	if( m_vKEY.m_vSTATE[TKEY_CAM_ROT] && m_vKEY.m_vTICK[TKEY_CAM_ROT] + CLICK_DELAY < m_vKEY.m_dwTick )
		return;

	if( m_vKEY.m_vSTATE[TKEY_ROT] && m_vKEY.m_vTICK[TKEY_ROT] + CLICK_DELAY < m_vKEY.m_dwTick )
		return;

	CTDetailInfoManager::UpdateInfo(point);
	UpdateDetailNameInfo(point);

	if( HitTest(point) )
		return;

	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		ResetHitOBJ( &m_vMAP.m_vDRAWBSP[i]->m_vTDRAW, point);
	ResetHitOBJ( &m_vMAP.m_vTDRAW, point);

	if( bChkDeadMon )
	{
		if( !m_pHIT || m_pHIT->m_bType != OT_MON || !m_pHIT->IsDead() )
		{
			m_nDEADMONHITIDX = 0;
			m_vDEADMONHITS.clear();
			return;
		}

		VTMONSTER vTEMP;
		m_vDEADMONHITS.swap(vTEMP);

		for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
			BuildHitDEADMON( &m_vMAP.m_vDRAWBSP[i]->m_vTDRAW, point);
		BuildHitDEADMON( &m_vMAP.m_vTDRAW, point);

		if( m_vDEADMONHITS.size() > 1 )
		{
			::sort(m_vDEADMONHITS.begin(), m_vDEADMONHITS.end(), compare_hit_dead_mon);

			INT nNewCNT = (INT)m_vDEADMONHITS.size();
			INT nPrvCNT = (INT)vTEMP.size();
			if( nNewCNT == nPrvCNT )
			{
				for(INT i=0; i<nNewCNT; ++i)
				{
					if( m_vDEADMONHITS[i] != vTEMP[i] )
					{
						m_nDEADMONHITIDX = 0;
						break;
					}
				}
			}
			else
				m_nDEADMONHITIDX = 0;
		}
		else
		{
			m_vDEADMONHITS.clear();
			m_nDEADMONHITIDX = 0;
		}
	}
}

void CTClientGame::ResetHitOBJ( LPLISTTOBJBASE pLIST, CPoint point)
{
	LISTTOBJBASE::reverse_iterator itTOBJ;

	BOOL bMainCharAlive = (!m_pMainChar->IsDead() && !m_pMainChar->m_bGhost);

	for( itTOBJ = pLIST->rbegin(); itTOBJ != pLIST->rend(); itTOBJ++)
	{
		CTClientObjBase* pTOBJ = (*itTOBJ);

		switch(pTOBJ->m_bType)
		{
		case OT_SWITCH		:
			{
				CTClientSwitch* pSwitch = static_cast<CTClientSwitch*>(pTOBJ);
				if( pSwitch->m_pTSWITCH->m_bLockOnClose && !pSwitch->m_bOPEN ||
					pSwitch->m_pTSWITCH->m_bLockOnOpen && pSwitch->m_bOPEN )
				{
					break;
				}

				if( bMainCharAlive &&
					TestHitOBJ(pTOBJ, m_pHIT, point) )
				{
					m_pHIT = pTOBJ;
				}
			}
			break;

		case OT_GODTOWER	:
		case OT_GODBALL		:
		case OT_MON			:
		case OT_SELF		:
		case OT_PC			:
			if( bMainCharAlive &&
				TestHitOBJ(pTOBJ, m_pHIT, point) )
			{
				m_pHIT = pTOBJ;
			}
			break;

		case OT_NPC			:
			if( TestHitOBJ(pTOBJ, m_pHIT, point) )
			{
				m_pHIT = pTOBJ;
			}
			break;

		case OT_RECALL		:
			if( bMainCharAlive &&
				TestHitOBJ(pTOBJ, m_pHIT, point) )
			{
				if( static_cast<CTClientRecall*>(pTOBJ)->m_bRecallType == TRECALLTYPE_PET &&
					static_cast<CTClientPet*>(pTOBJ)->GetTakeUpChar() )
				{
					m_pHIT = static_cast<CTClientPet*>(pTOBJ)->GetTakeUpChar();
				}
				else
					m_pHIT = pTOBJ;
			}
			break;
		}
	}
}

BOOL CTClientGame::TestHitOBJ(CTClientObjBase* pTOBJ, CTClientObjBase* pTCURHIT, CPoint point)
{
	static const FLOAT fMaxTarDist2 = TARGET_DIST*TARGET_DIST;

	if( !pTOBJ->m_bCanSelected ||
		pTOBJ == m_pMainChar ||
		(pTCURHIT && pTOBJ->m_fZValue >= pTCURHIT->m_fZValue) )
	{
		return FALSE;
	}

	D3DXVECTOR2 vToMainChar;
	vToMainChar.x = pTOBJ->GetPositionX() - m_pMainChar->GetPositionX();
	vToMainChar.y = pTOBJ->GetPositionZ() - m_pMainChar->GetPositionZ();

	FLOAT fTarDist2 = D3DXVec2LengthSq(&vToMainChar);
	if( fTarDist2 < fMaxTarDist2 )
	{
		OBJHIT vHIT = pTOBJ->HitTest(m_pCamera, point.x,point.y, 2.0f);
		if( vHIT.m_nIndex >= 0 || HitTestObjName(pTOBJ, point) )
			return TRUE;
	}

	return FALSE;
}

void CTClientGame::BuildHitDEADMON( LPLISTTOBJBASE pLIST, CPoint point)
{
	LISTTOBJBASE::reverse_iterator itTOBJ;

	for( itTOBJ = pLIST->rbegin(); itTOBJ != pLIST->rend(); itTOBJ++)
	{
		CTClientObjBase* pTOBJ = (*itTOBJ);

		if( pTOBJ->m_bType != OT_MON || !pTOBJ->m_bCanSelected || !pTOBJ->IsDead() )
			continue;
		
		if( D3DXVec2Length(&D3DXVECTOR2(
			pTOBJ->GetPositionX() - m_pMainChar->GetPositionX(),
			pTOBJ->GetPositionZ() - m_pMainChar->GetPositionZ())) >= TARGET_DIST )
		{
			continue;
		}
	
		OBJHIT vHIT = pTOBJ->HitTest(
			m_pCamera,
			point.x,
			point.y);

		if( vHIT.m_nIndex >= 0 )
			m_vDEADMONHITS.push_back( static_cast<CTClientMonster*>(pTOBJ) );
	}
}

CTClientMonster* CTClientGame::NextHitDEADMON()
{
	CTClientMonster* pResult = m_vDEADMONHITS[m_nDEADMONHITIDX];

	INT nMAX = (INT)m_vDEADMONHITS.size()-1;
	m_nDEADMONHITIDX = (m_nDEADMONHITIDX >= nMAX)? 0: m_nDEADMONHITIDX+1;

	return pResult;
}

void CTClientGame::ResetTMonInvenUI()
{
	static DWORD dwMoneyID[TMONEY_COUNT] = {
		ID_CTRLINST_RUNE,
		ID_CTRLINST_LUNA,
		ID_CTRLINST_CRON};

	for( BYTE i=0; i<TMONEY_COUNT; i++)
	{
		TComponent *pTMONEY = m_vTFRAME[TFRAME_MONINVEN]->FindKid(dwMoneyID[i]);

		if(pTMONEY)
			pTMONEY->m_strText.Format( _T("%d"), m_vMonMoney[i]);
	}

	for( i=0; i<MAX_DRAGSLOT; i++)
	{
		TImageList *pTSLOT = (TImageList *) m_vTFRAME[TFRAME_MONINVEN]->FindKid(m_vSlotID[i]);

		if(pTSLOT)
		{
			CTClientItem *pTITEM = m_vTMONINVEN.FindTItem(i);

			if( pTITEM && pTITEM->GetCount() > 1 )
				pTSLOT->m_strText = CTChart::Format( TSTR_FMT_NUMBER, pTITEM->GetCount());
			else
				pTSLOT->m_strText.Empty();

			WORD wImg = 0;
			if( pTITEM )
			{
				LPTITEMVISUAL pTVISUAL = pTITEM->GetVisual();
				wImg = pTVISUAL? pTVISUAL->m_wIcon: 0;
			}

			pTSLOT->SetCurImage(wImg);
		}
	}
}

void CTClientGame::ClearMonInven()
{
	for( BYTE i=0; i<TMONEY_COUNT; i++)
		m_vMonMoney[i] = 0;
	m_vTMONINVEN.ClearInven();
}

void CTClientGame::ResetQuestUI()
{
	CTNewQuestDlg *pTNewQuestDlg = (CTNewQuestDlg *) m_vTFRAME[TFRAME_QUEST];
	CTGaugePannel *pTPANNEL = (CTGaugePannel *) m_vTFRAME[TFRAME_GAUGE];

	if(pTNewQuestDlg)
	{
		pTNewQuestDlg->Reset();
		pTNewQuestDlg->ResetTree();
	}

	if(pTPANNEL)
		pTPANNEL->UpdateAllQuestINFO(m_pMainChar);
}

void CTClientGame::ResetNPCTalkUI()
{
	MAPTQUESTTEMP::iterator it;

	LPVTQUESTTEMP pTDEFTALK = NULL;
	LISTTQUESTTEMP vTQUEST;
	VTQUESTTEMP vTDEFTALK;
	VTQUESTTEMP vTNPCTALK;

	if(!m_pTARGET)
		return;

	CTNPCTalkDlg *pTNPCTalkDlg = (CTNPCTalkDlg *) m_vTFRAME[TFRAME_NPCDIALOG];
	pTNPCTalkDlg->Reset();
	pTNPCTalkDlg->m_pHost = m_pMainChar;
	pTNPCTalkDlg->m_vTDEFTALK.clear();

	vTDEFTALK.clear();
	vTNPCTALK.clear();
	vTQUEST.clear();
	EnableUI(TFRAME_NPCDIALOG);

	for( it = m_pTARGET->m_mapTQUESTTEMP.begin(); it != m_pTARGET->m_mapTQUESTTEMP.end(); it++)
		switch((*it).second->m_bType)
		{
		case QT_NPCTALK		: vTNPCTALK.push_back((*it).second); break;
		case QT_DEFTALK		: vTDEFTALK.push_back((*it).second); break;
		case QT_COMPLETE	:
		case QT_MISSION		: vTQUEST.push_back((*it).second); break;
		}
	vTQUEST.sort(binary_quest());

	while(!vTQUEST.empty())
	{
		LPTQUEST pTQUEST = vTQUEST.front();

		pTNPCTalkDlg->AddQuest(pTQUEST);
		vTQUEST.pop_front();
	}

	if(vTNPCTALK.empty())
		pTDEFTALK = &vTDEFTALK;
	else
		pTDEFTALK = &vTNPCTALK;

	for( INT i=0; i<BYTE(pTDEFTALK->size()); i++)
		pTNPCTalkDlg->m_vTDEFTALK.push_back((*pTDEFTALK)[i]);

	vTDEFTALK.clear();
	vTNPCTALK.clear();
}

void CTClientGame::ResetSkillUI()
{
	static DWORD dwFrameID[][TCLASS_COUNT][TCONTRY_COUNT] = {{
		{ ID_FRAME_D_WARRIOR0, ID_FRAME_C_WARRIOR0, ID_FRAME_B_WARRIOR0},
		{ ID_FRAME_D_RANGER0, ID_FRAME_C_RANGER0, ID_FRAME_B_RANGER0},
		{ ID_FRAME_D_ARCHER0, ID_FRAME_C_ARCHER0, ID_FRAME_B_ARCHER0},
		{ ID_FRAME_D_WIZARD0, ID_FRAME_C_WIZARD0, ID_FRAME_B_WIZARD0},
		{ ID_FRAME_D_PRIEST0, ID_FRAME_C_PRIEST0, ID_FRAME_B_PRIEST0},
		{ ID_FRAME_D_SORCERER0, ID_FRAME_C_SORCERER0, ID_FRAME_B_SORCERER0}}, {
		{ ID_FRAME_D_WARRIOR1, ID_FRAME_C_WARRIOR1, ID_FRAME_B_WARRIOR1},
		{ ID_FRAME_D_RANGER1, ID_FRAME_C_RANGER1, ID_FRAME_B_RANGER1},
		{ ID_FRAME_D_ARCHER1, ID_FRAME_C_ARCHER1, ID_FRAME_B_ARCHER1},
		{ ID_FRAME_D_WIZARD1, ID_FRAME_C_WIZARD1, ID_FRAME_B_WIZARD1},
		{ ID_FRAME_D_PRIEST1, ID_FRAME_C_PRIEST1, ID_FRAME_B_PRIEST1},
		{ ID_FRAME_D_SORCERER1, ID_FRAME_C_SORCERER1, ID_FRAME_B_SORCERER1}}, {
		{ ID_FRAME_D_WARRIOR2, ID_FRAME_C_WARRIOR2, ID_FRAME_B_WARRIOR2},
		{ ID_FRAME_D_RANGER2, ID_FRAME_C_RANGER2, ID_FRAME_B_RANGER2},
		{ ID_FRAME_D_ARCHER2, ID_FRAME_C_ARCHER2, ID_FRAME_B_ARCHER2},
		{ ID_FRAME_D_WIZARD2, ID_FRAME_C_WIZARD2, ID_FRAME_B_WIZARD2},
		{ ID_FRAME_D_PRIEST2, ID_FRAME_C_PRIEST2, ID_FRAME_B_PRIEST2},
		{ ID_FRAME_D_SORCERER2, ID_FRAME_C_SORCERER2, ID_FRAME_B_SORCERER2}}, {
		{ ID_FRAME_D_WARRIOR3, ID_FRAME_C_WARRIOR3, ID_FRAME_B_WARRIOR3},
		{ ID_FRAME_D_RANGER3, ID_FRAME_C_RANGER3, ID_FRAME_B_RANGER3},
		{ ID_FRAME_D_ARCHER3, ID_FRAME_C_ARCHER3, ID_FRAME_B_ARCHER3},
		{ ID_FRAME_D_WIZARD3, ID_FRAME_C_WIZARD3, ID_FRAME_B_WIZARD3},
		{ ID_FRAME_D_PRIEST3, ID_FRAME_C_PRIEST3, ID_FRAME_B_PRIEST3},
		{ ID_FRAME_D_SORCERER3, ID_FRAME_C_SORCERER3, ID_FRAME_B_SORCERER3}}};

	static DWORD dwTitleID[][TCLASS_COUNT] = {
		// 첫번째 탭
		{
			TSTR_SKILLTAB_PASSIVE,		// WARRIOR
			TSTR_SKILLTAB_PASSIVE,		// RANGER
			TSTR_SKILLTAB_PASSIVE,		// ARCHER
			TSTR_SKILLTAB_PASSIVE,		// WIZARD
			TSTR_SKILLTAB_PASSIVE,		// PRIEST
			TSTR_SKILLTAB_PASSIVE		// SORCERER
		},

		// 두번째 탭
		{
			TSTR_SKILLTAB_ATTACK,
			TSTR_SKILLTAB_BATTLE,
			TSTR_SKILLTAB_BATTLE,
			TSTR_SKILLTAB_FIRE,
			TSTR_SKILLTAB_DIVINE,
			TSTR_SKILLTAB_SUPPORT  
		},

		// 세번째 탭
		{
			TSTR_SKILLTAB_DEFENSE,
			TSTR_SKILLTAB_ASSASSIN,
			TSTR_SKILLTAB_SHOOT,
			TSTR_SKILLTAB_ICE,
			TSTR_SKILLTAB_DARK,
			TSTR_SKILLTAB_SUMMON
		},

		// 네번째 탭
		{
			TSTR_SKILLTAB_STATE,
			TSTR_SKILLTAB_SURVIVAL,
			TSTR_SKILLTAB_SURVIVAL,
			TSTR_SKILLTAB_ELEC,
			TSTR_SKILLTAB_SACRIFICE,
			TSTR_SKILLTAB_CONTROL
		}
	};

	CTSkillDlg *pUI = (CTSkillDlg *) m_vTFRAME[TFRAME_SKILL];
	pUI->m_pMainChar = m_pMainChar;

	for( BYTE i=0; i<TSKILLTAB_COUNT; i++)
	{
		CString strTITLE;

		strTITLE = CTChart::LoadString( (TSTRING) dwTitleID[i][m_pMainChar->m_bClassID]);
		pUI->m_pTTAB->InsertItem(
			i, strTITLE,
			m_pTParser->FindFrameTemplate(dwFrameID[i][m_pMainChar->m_bClassID][0]));

		pUI->m_pTTAB->m_Frames[i]->MoveComponent(pUI->m_vPOS);
		pUI->m_pTTAB->m_Frames[i]->m_bSND = FALSE;

		for( BYTE j=0; j<TSKILLSLOT_COUNT; j++)
		{
			TImageList *pKid = (TImageList *) pUI->m_pTTAB->m_Frames[i]->FindKid(CTClientSkill::m_vTSKILLSLOT[j]);
			TGauge *pTGauge = (TGauge *) pUI->m_pTTAB->m_Frames[i]->FindKid(CTClientGame::m_vTICONGAUGE[j]);

			LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP( 
				CTChart::GetTSKILLID( m_pMainChar->m_bContryID, m_pMainChar->m_bClassID, i, j ));

			if( pKid && pTSKILL && pTGauge )
			{
				pTGauge->SetStyle(pTGauge->GetStyle()|TGS_GROW_UP);
				pKid->SetCurImage(pTSKILL->m_wIconID);
			}
		}
	}

	pUI->m_pTTAB->SelectItem(0);
}

void CTClientGame::ResetLeaderMark()
{
	static BYTE vLEADERMARK[MAX_PARTY] = {
		TLEADERMARK_PARTY1,
		TLEADERMARK_PARTY2,
		TLEADERMARK_PARTY3,
		TLEADERMARK_PARTY4,
		TLEADERMARK_PARTY5,
		TLEADERMARK_PARTY6};
	BYTE bCount = INT(m_vTPARTY.size());

	m_vTLEADERMARK[TLEADERMARK_TARGET]->ShowComponent(m_pTARGET && m_pTARGET->m_bType == OT_PC && ((CTClientChar *) m_pTARGET)->m_dwChiefID == m_pTARGET->m_dwID ? TRUE : FALSE);
	m_vTLEADERMARK[TLEADERMARK_PLAYER]->ShowComponent(m_pMainChar->m_dwChiefID == m_pMainChar->m_dwID ? TRUE : FALSE);

	for( BYTE i=0; i<MAX_PARTY; i++)
		m_vTLEADERMARK[vLEADERMARK[i]]->ShowComponent(i < bCount && m_vTPARTY[i]->m_dwCharID == m_pMainChar->m_dwChiefID ? TRUE : FALSE);
}

void CTClientGame::ResetSkillICON()
{
	CTSkillDlg *pUI = (CTSkillDlg *) m_vTFRAME[TFRAME_SKILL];
	TComponent *pPOINT = pUI->FindKid(ID_CTRLINST_ENABLE_POINT);

	for( BYTE i=0; i<TSKILLTAB_COUNT; i++)
		for( BYTE j=0; j<TSKILLSLOT_COUNT; j++)
		{
			TImageList *pKid = (TImageList *) pUI->m_pTTAB->m_Frames[i]->FindKid(CTClientSkill::m_vTSKILLSLOT[j]);
			TComponent *pLEVEL = pUI->m_pTTAB->m_Frames[i]->FindKid(CTClientSkill::m_vTSKILLLEVEL[j]);

			LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(
				CTChart::GetTSKILLID( m_pMainChar->m_bContryID, m_pMainChar->m_bClassID, i, j) );

			if( pKid && pTSKILL )
			{
				CTClientSkill *pTSkill = m_pMainChar->FindTSkill(pTSKILL->m_wSkillID);

				if(pTSkill)
				{
					if(pLEVEL)
					{
						pLEVEL->m_strText = CTChart::Format( TSTR_FMT_GAUGE, pTSkill->m_bLevel, pTSkill->m_pTSKILL->m_bMaxLevel);

						CD3DFont* pFont = pLEVEL->Get3DFont();
						pFont->m_bOutLine = TRUE;
						pFont->m_dwLineColor = 0xFF000000;
					}

					if(!m_pMainChar->CheckSkill(pTSKILL))
						pTSkill->m_bLoopON = FALSE;

					pTSkill->m_dwReuseTick = pTSkill->GetReuseTick(m_pMainChar);
					pKid->EnableComponent(TRUE);
				}
				else
				{
					if(pLEVEL)
						pLEVEL->m_strText.Empty();
					pKid->EnableComponent(FALSE);
				}
			}
		}
	pPOINT->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pMainChar->m_vTSKILLPOINT[TPOINT_MINE]);

	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
	pTGaugePannel->ResetHotkeyUI();
}

void CTClientGame::ResetCharInfoUI()
{
	CTCharInfoDlg *pTCharInfoDlg = (CTCharInfoDlg *) m_vTFRAME[TFRAME_CHAR_INFO];

	pTCharInfoDlg->ResetData(
		m_pMainChar,
		m_pRES,
		pTCharInfoDlg->m_bTabIndex);

	ResetSkillICON();
}

void CTClientGame::ResetInvenUI()
{
	static BYTE vInvenID[MAX_INVEN] = {
		TFRAME_DEFINVEN,
		TFRAME_CHAR_INFO,
		TFRAME_INVEN_1,
		TFRAME_INVEN_2,
		TFRAME_INVEN_3,
		TFRAME_INVEN_4,
		TFRAME_INVEN_5};

	m_vTFRAME[TFRAME_DEFINVEN]->FindKid(ID_CTRLINST_RUNE)->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		m_pMainChar->m_dwRune);

	m_vTFRAME[TFRAME_DEFINVEN]->FindKid(ID_CTRLINST_LUNA)->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		m_pMainChar->m_dwLuna);

	m_vTFRAME[TFRAME_DEFINVEN]->FindKid(ID_CTRLINST_CRON)->m_strText = CTChart::Format( TSTR_FMT_NUMBER,
		m_pMainChar->m_dwCron);

	m_vTTEXT[TTEXT_PLAYER_RUNE]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pMainChar->m_dwRune);
	m_vTTEXT[TTEXT_PLAYER_LUNA]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pMainChar->m_dwLuna);
	m_vTTEXT[TTEXT_PLAYER_CRON]->m_strText = CTChart::Format( TSTR_FMT_NUMBER, m_pMainChar->m_dwCron);

	for( BYTE i=0; i<MAX_INVEN; i++)
	{
		BYTE bInvenID = GetInvenID(vInvenID[i]);
		MAPTINVEN::iterator finder = m_pMainChar->m_mapTINVEN.find(bInvenID);

		if( finder != m_pMainChar->m_mapTINVEN.end() )
		{
			LPTITEM pTITEM = CTChart::FindTITEMTEMP((*finder).second->m_wItemID);
			LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(pTITEM);
			if( pTVISUAL )
			{
				if( bInvenID < MAX_SUBINVEN )
					((CTCharInfoDlg *) m_vTFRAME[TFRAME_CHAR_INFO])->ResetSubInven( bInvenID, pTVISUAL->m_wIcon, TRUE);

				if( m_vTFRAME[vInvenID[i]] &&
					MAKELONG( WORD(pTVISUAL->m_dwInvenID), WORD(vInvenID[i])) != m_vTFRAME[vInvenID[i]]->m_id )
				{			
					RemoveKid(m_vTFRAME[vInvenID[i]]);

					delete m_vTFRAME[vInvenID[i]];
					m_vTFRAME[vInvenID[i]] = NULL;
				}

				if( !m_vTFRAME[vInvenID[i]] )
				{
					m_vTFRAME[vInvenID[i]] = new CTInvenDlg( this, m_pTParser->FindFrameTemplate(pTVISUAL->m_dwInvenID), vInvenID[i]);
					AddKid(m_vTFRAME[vInvenID[i]]);

					m_vTFRAME[vInvenID[i]]->ShowComponent(FALSE);
					m_vTFRAME[vInvenID[i]]->EnableFloat(TRUE);
				}
			}

			((CTInvenDlg *) m_vTFRAME[vInvenID[i]])->m_pTINVEN = (*finder).second;

			for( BYTE j=0; j<pTITEM->m_bSlotCount; j++)
			{
				TImageList *pItemBox = (TImageList *) m_vTFRAME[vInvenID[i]]->FindKid(m_vSlotID[j]);

				if( pItemBox )
				{
					MAPTITEM::iterator itTITEM = (*finder).second->m_mapTITEM.find(j);
					if( itTITEM != (*finder).second->m_mapTITEM.end() )
					{
						CTClientItem* pItemInst = itTITEM->second;
						LPTITEMVISUAL pTVISUAL = pItemInst->GetVisual();
						if( pTVISUAL )
							pItemBox->SetCurImage(pTVISUAL->m_wIcon);

						DWORD qt = pItemInst->GetQuality();

						if( pItemInst->GetWrap() )
							pItemBox->SetSkinImage( TITEM_INDEX_WRAP_SKIN );
						else if ( qt & TITEM_QUALITY_GRADE )
							pItemBox->SetSkinImage( TITEM_INDEX_GRADE_SKIN + pItemInst->GetGrade() );
						else if ( qt & TITEM_QUALITY_RARE )
							pItemBox->SetSkinImage( TITEM_INDEX_RARE_SKIN );
						else if ( qt & TITEM_QUALITY_MAGIC )
							pItemBox->SetSkinImage( TITEM_INDEX_MAGIC_SKIN );
						else
							pItemBox->SetSkinImageEmpty();

						if( (*itTITEM).second->GetCount() > 1 )
							pItemBox->m_strText = CTChart::Format( TSTR_FMT_NUMBER, (*itTITEM).second->GetCount());
						else
							pItemBox->m_strText.Empty();
					}
					else
					{
						pItemBox->m_strText.Empty();
						pItemBox->SetCurImage(0);
						pItemBox->SetSkinImageEmpty();
					}
				}
			}
		}
		else
		{
			if( bInvenID < MAX_SUBINVEN )
				((CTCharInfoDlg *) m_vTFRAME[TFRAME_CHAR_INFO])->ResetSubInven( bInvenID, 0, FALSE);

			if(m_vTFRAME[vInvenID[i]])
			{
				RemoveKid(m_vTFRAME[vInvenID[i]]);

				delete m_vTFRAME[vInvenID[i]];
				m_vTFRAME[vInvenID[i]] = NULL;
			}
		}
	}

	CheckOpenCash();
}

void CTClientGame::ResetRecallINFO( BYTE bContinue)
{
	CTClientRecall *pTRECALL = GetMainRecall();

	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
	pTGaugePannel->CalcRecallTick(pTRECALL);
	pTGaugePannel->ResetRecallUI(pTRECALL);

	m_vTFACEIMG[TFACEIMG_SUMMON_N]->ShowComponent(FALSE);
	m_vTFACEIMG[TFACEIMG_SUMMON_S]->ShowComponent(FALSE);

	if( pTRECALL && pTRECALL->m_pTEMP->m_dwOBJ )
	{
		m_vTGAUGE[TGAUGE_SUMMON_HP]->ShowComponent(TRUE);
		m_vTGAUGE[TGAUGE_SUMMON_MP]->ShowComponent(TRUE);
		m_vTPANNEL[TPANNEL_SUMMON]->ShowComponent(TRUE);
		m_vTLEVELIMG[TLEVELIMG_SUMMON]->ShowComponent(TRUE);
		m_vTFACEBACKIMG[TFACEBACKIMG_SUMMON]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_SUMMON_NAME]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_SUMMON_LEVEL]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_SUMMON_HP]->ShowComponent(TRUE);
		m_vTTEXT[TTEXT_SUMMON_MP]->ShowComponent(TRUE);
		
		m_vTTEXT[TTEXT_SUMMON_NAME]->m_strText = CTChart::Format( TSTR_FMT_NAME,
			pTRECALL->GetName());

		m_vTTEXT[TTEXT_SUMMON_LEVEL]->m_strText = CTChart::Format( TSTR_FMT_LEVEL,
			pTRECALL->m_bLevel);

		m_vTGAUGE[TGAUGE_SUMMON_HP]->SetGauge(
			pTRECALL->m_dwHP,
			pTRECALL->m_dwMaxHP,
			bContinue);

		m_vTGAUGE[TGAUGE_SUMMON_MP]->SetGauge(
			pTRECALL->m_dwMP,
			pTRECALL->m_dwMaxMP,
			bContinue);

		m_vTTEXT[TTEXT_SUMMON_HP]->m_strText = CTChart::Format( TSTR_FMT_GAUGE,
			pTRECALL->m_dwHP,
			pTRECALL->m_dwMaxHP);

		m_vTTEXT[TTEXT_SUMMON_MP]->m_strText = CTChart::Format( TSTR_FMT_GAUGE,
			pTRECALL->m_dwMP,
			pTRECALL->m_dwMaxMP);

		if( CTChart::m_setTRACEOBJID.find(pTRECALL->m_pTEMP->m_dwOBJ) != CTChart::m_setTRACEOBJID.end() )
		{
			m_vTFACEIMG[TFACEIMG_SUMMON_N]->SetCurImage(pTRECALL->m_pTEMP->m_wFaceIcon);
			m_vTFACEIMG[TFACEIMG_SUMMON_N]->ShowComponent(TRUE);
		}
		else
		{
			m_vTFACEIMG[TFACEIMG_SUMMON_S]->SetCurImage(pTRECALL->m_pTEMP->m_wFaceIcon);
			m_vTFACEIMG[TFACEIMG_SUMMON_S]->ShowComponent(TRUE);
		}
	}
	else
	{
		m_vTGAUGE[TGAUGE_SUMMON_HP]->ShowComponent(FALSE);
		m_vTGAUGE[TGAUGE_SUMMON_MP]->ShowComponent(FALSE);
		m_vTPANNEL[TPANNEL_SUMMON]->ShowComponent(FALSE);
		m_vTLEVELIMG[TLEVELIMG_SUMMON]->ShowComponent(FALSE);
		m_vTFACEBACKIMG[TFACEBACKIMG_SUMMON]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_SUMMON_NAME]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_SUMMON_LEVEL]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_SUMMON_HP]->ShowComponent(FALSE);
		m_vTTEXT[TTEXT_SUMMON_MP]->ShowComponent(FALSE);
	}
}

void CTClientGame::ResetRecallTIME(DWORD dwTick)
{
	CTClientRecall *pTRECALL = GetMainRecall();

	if( pTRECALL && pTRECALL->m_pTEMP->m_dwOBJ )
	{
		CString strResult;

		DWORD dwLeftLifeTick = pTRECALL->GetLeftLifeTick();
		if( dwLeftLifeTick == 0 )
		{
			m_vTTEXT[TTEXT_SUMMON_TIME]->ShowComponent(FALSE);
			return;
		}

		DWORD dwT = dwLeftLifeTick / 3600000;
		if( dwT )
			strResult = CTChart::Format( TSTR_FMT_TIME_HOUR, dwT);
		else
		{
			DWORD dwL = dwLeftLifeTick % 3600000;
			dwT = dwL / 60000;
			dwL = dwL % 60000;
			
			if( dwT )
				strResult = CTChart::Format( TSTR_FMT_TIME_MINUTE, dwT, dwL / 1000);
			else
				strResult = CTChart::Format( TSTR_FMT_TIME_SECOND, dwL / 1000);
		}
		
		m_vTTEXT[TTEXT_SUMMON_TIME]->m_strText = strResult;
		m_vTTEXT[TTEXT_SUMMON_TIME]->ShowComponent(TRUE);
	}
	else
		m_vTTEXT[TTEXT_SUMMON_TIME]->ShowComponent(FALSE);
}

void CTClientGame::OnBeginDrag( CPoint point)
{
	if(m_vDRAG.m_pIMAGE)
	{
		delete m_vDRAG.m_pIMAGE;
		m_vDRAG.m_pIMAGE = NULL;
	}

	m_vDRAG.m_bFrameID = TFRAME_COUNT;
	m_vDRAG.m_bSlotID = 0;
	m_vDRAG.m_dwParam = 0;

	BYTE bFID;
	CTClientUIBase* pFrame = static_cast<CTClientUIBase*>( GetTopFrame(point, 0, &bFID) );
	if( pFrame && pFrame->OnBeginDrag(&m_vDRAG, point) )
	{
		m_vDRAG.m_bFrameID = bFID;
		m_bDragSND = TRUE;

		m_bInvenDraging = CheckInvenItem(bFID, m_vDRAG.m_bSlotID);
		
		if( m_bInvenDraging )
		{
			CTMainUI* pMainUI = static_cast<CTMainUI*>(m_vTFRAME[TFRAME_MAIN]);
			if( !pMainUI->m_bInvenSlot )
			{
				pMainUI->m_bInvenSlot = TRUE;
			}
		}
	}
}

void CTClientGame::OnDragOver( CPoint point)
{
	CSize size;

	m_vDRAG.m_pIMAGE->GetComponentSize(&size);
	point.x -= size.cx / 2;
	point.y -= size.cy / 2;

	m_vDRAG.m_pIMAGE->MoveComponent(point);
	m_vDRAG.m_pIMAGE->ShowComponent(TRUE);

	if(m_bDragSND)
	{
		PlayUISound(ID_SND_ITEM_DRAG);
		m_bDragSND = FALSE;
	}
}

void CTClientGame::OnDrop( UINT nFlags, CPoint point)
{
	TDROPINFO vTDROP;
	CTClientUIBase *pFrame = NULL;
	BYTE bFID = T_INVALID;

	TCOMP_LIST::reverse_iterator itr,end;
	itr = m_kids.rbegin();
	end = m_kids.rend();
	for( ; itr != end; ++itr )
	{
		if((*itr)->HitTest(point))
		{
			CTClientUIBase *pHIT = (CTClientUIBase *) *itr;
			BYTE bID = GetFrameID( pHIT );

			if( bID == TFRAME_DETAIL_INFO || bID == TFRAME_DETAIL_NAME || bID == TFRAME_CHAT_DETAIL_INFO )
				continue;

			pFrame = pHIT;
			bFID = bID;
			break;
		}
	}

	if( pFrame && pFrame != (CTClientUIBase*) &(m_pMainWnd->m_MessageBox) )
	{
		vTDROP = pFrame->OnDrop(point);

		if(vTDROP.m_bDrop)
			vTDROP.m_bTargetID = bFID;
	}

	DropItem(
		nFlags,
		vTDROP.m_bTargetID,
		vTDROP.m_bSlotID);

	if(m_vDRAG.m_pIMAGE)
	{
		delete m_vDRAG.m_pIMAGE;
		m_vDRAG.m_pIMAGE = NULL;
	}

	m_vDRAG.m_bFrameID = TFRAME_COUNT;
	m_vDRAG.m_bSlotID = 0;
	m_vDRAG.m_dwParam = 0;
	m_bInvenDraging = FALSE;

	PlayUISound(ID_SND_ITEM_DROP);
}

void CTClientGame::DropItem( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( m_vDRAG.m_bFrameID == TFRAME_GAUGE &&
		m_vDRAG.m_bFrameID == bTargetUI &&
		m_pTRSCS->m_bDropping )
	{
		DropItem_MainUIToMainUI(nFlags, bTargetUI, bSlotID);
		m_pTRSCS->m_bDropping = FALSE;
		return ;
	}

	if( !m_pSESSION || (
		m_vDRAG.m_bFrameID != TFRAME_GAUGE &&
		m_vDRAG.m_bFrameID == bTargetUI && (
		m_vDRAG.m_bFrameID == TFRAME_ITEM_SHOP ||
		m_vDRAG.m_bSlotID == bSlotID )))
	{
		switch(m_vDRAG.m_bFrameID)
		{
		case TFRAME_MONINVEN	: 		ResetTMonInvenUI(); break;
		case TFRAME_DEFINVEN	:
		case TFRAME_INVEN_1		:
		case TFRAME_INVEN_2		:
		case TFRAME_INVEN_3		:
		case TFRAME_INVEN_4		:
		case TFRAME_INVEN_5		:
		case TFRAME_CHAR_INFO	: 		ResetInvenUI(); break;
		}

		return;
	}

	switch(m_vDRAG.m_bFrameID)
	{
	case TFRAME_DEFINVEN	:
	case TFRAME_INVEN_1		:
	case TFRAME_INVEN_2		:
	case TFRAME_INVEN_3		:
	case TFRAME_INVEN_4		:
	case TFRAME_INVEN_5		:
	case TFRAME_CHAR_INFO	:
		{
			switch(bTargetUI)
			{
			case TFRAME_DEFINVEN	:
			case TFRAME_INVEN_1		:
			case TFRAME_INVEN_2		:
			case TFRAME_INVEN_3		:
			case TFRAME_INVEN_4		:
			case TFRAME_INVEN_5		:
			case TFRAME_CHAR_INFO	:
				{
					if( m_vDRAG.m_bFrameID == TFRAME_CHAR_INFO &&
						bTargetUI != TFRAME_CHAR_INFO &&
						TMAINSLOTID_INVEN == CTMainUI::ToSlotType(m_vDRAG.m_bSlotID) )
					{
						// 캐릭터정보창에서 인벤으로 가방을 옮길때
						DropItem_MainUIToInven(nFlags, bTargetUI, bSlotID);
					}
					else if( m_vDRAG.m_bFrameID != TFRAME_CHAR_INFO &&
						bTargetUI == TFRAME_CHAR_INFO &&
						TMAINSLOTID_INVEN == CTMainUI::ToSlotType(bSlotID) )
					{
						// 인벤에서 캐릭터정보창으로 가방을 옮길때
						DropItem_InvenToMainUI(nFlags, bTargetUI, bSlotID);
					}
					else if( m_vDRAG.m_bFrameID == TFRAME_CHAR_INFO &&
						bTargetUI == TFRAME_CHAR_INFO &&
						TMAINSLOTID_INVEN == CTMainUI::ToSlotType(m_vDRAG.m_bSlotID) )
					{
						// 캐릭터정보창에서 가방을 다른 가방슬롯으로 옮길때
						DropItem_MainUIToMainUI(nFlags, bTargetUI, bSlotID);
					}
					else
					{
						// 인벤에서 인벤으로 아이템을 옮길때
						DropItem_InvenToInven(nFlags, bTargetUI, bSlotID);
					}
				}
				break;
			
			case TFRAME_ITEM_UP		:	
			case TFRAME_ITEM_REPAIR	:
			case TFRAME_ITEM_REFINE	:
			case TFRAME_ITEM_GAMBLE	:
			case TFRAME_ITEM_REGGM:
			case TFRAME_ITEM_EXTPERIOD	:	DropItem_InvenToItemSlot(nFlags, bTargetUI, bSlotID); break;

			case TFRAME_ITEM_SHOP	:	DropItem_InvenToItemshop(nFlags, bTargetUI, bSlotID); break;
			case TFRAME_GAUGE		:	DropItem_InvenToMainUI(nFlags, bTargetUI, bSlotID); break;
			case TFRAME_TRADE		:	DropItem_InvenToTrade(nFlags, bTargetUI, bSlotID); break;
			case TFRAME_PRIVATE_SELL:	DropItem_InvenToPrivateSell(nFlags, bTargetUI, bSlotID); break;
			case TFRAME_MAIL_SEND	:	DropItem_InvenToMailSend(nFlags, bTargetUI, bSlotID); break;
			case TFRAME_CABINET		:	DropItem_InvenToCabinet(nFlags, bTargetUI, bSlotID); break;
			case TFRAME_GUILDCABINET:	DropItem_InvenToGuildCabinet(nFlags, bTargetUI, bSlotID); break;
			case TFRAME_CASHCABINET:	 DropItem_InvenToCashCabinet(nFlags, bTargetUI, bSlotID); break;
			case TFRAME_AUCTION		:	DropItem_InvenToAuctionReg( nFlags, bTargetUI, bSlotID); break;
			default					:	DropItem_InvenToDefault(nFlags, bTargetUI, bSlotID); break;
			}
		}
		break;

	case TFRAME_TRADE		:			DropItem_TradeToDefault(nFlags, bTargetUI, bSlotID); break;
	case TFRAME_PRIVATE_SELL:			DropItem_PrivateSellToDefault(nFlags, bTargetUI, bSlotID); break;
	case TFRAME_MAIL_SEND	:			DropItem_MailSendToDefault(nFlags, bTargetUI, bSlotID); break;
	case TFRAME_SKILL		:			DropItem_SkillToDefault(nFlags, bTargetUI, bSlotID); break;
	case TFRAME_RENT_SKILL	:			DropItem_RentSkillToDefault( nFlags, bTargetUI, bSlotID ); break;
	
	case TFRAME_GAUGE		:			
		{
			switch(bTargetUI)
			{
			case TFRAME_DEFINVEN	:
			case TFRAME_INVEN_1		:
			case TFRAME_INVEN_2		:
			case TFRAME_INVEN_3		:
			case TFRAME_INVEN_4		:
			case TFRAME_INVEN_5		:	DropItem_MainUIToInven(nFlags, bTargetUI, bSlotID); break;
            case TFRAME_GAUGE		:	DropItem_MainUIToMainUI(nFlags, bTargetUI, bSlotID); break;
			case TFRAME_PET_MANAGE	:
			default					:	DropItem_MainUIToDefault(nFlags, bTargetUI, bSlotID); break;
			}

			m_pTRSCS->m_bDropping = FALSE;
		}
		break;
		
	case TFRAME_ITEM_SHOP	:
		{
			switch(bTargetUI)
			{
			case TFRAME_DEFINVEN	:
			case TFRAME_INVEN_1		:
			case TFRAME_INVEN_2		:
			case TFRAME_INVEN_3		:
			case TFRAME_INVEN_4		:
			case TFRAME_INVEN_5		:	DropItem_ShopToInven(nFlags, bTargetUI, bSlotID); break;
			}
		}
		break;

	case TFRAME_MONINVEN	:
		{
			switch(bTargetUI)
			{
			case TFRAME_DEFINVEN	:
			case TFRAME_INVEN_1		:
			case TFRAME_INVEN_2		:
			case TFRAME_INVEN_3		:
			case TFRAME_INVEN_4		:
			case TFRAME_INVEN_5		:	DropItem_MonInvenToInven(nFlags, bTargetUI, bSlotID); break;
			default					:	ResetTMonInvenUI(); break;
			}
		}
		break;

	case TFRAME_CABINET			:
	case TFRAME_GUILDCABINET	:
	case TFRAME_CASHCABINET		:
		{
			switch(bTargetUI)
			{
			case TFRAME_DEFINVEN	:
			case TFRAME_INVEN_1		:
			case TFRAME_INVEN_2		:
			case TFRAME_INVEN_3		:
			case TFRAME_INVEN_4		:
			case TFRAME_INVEN_5		:	DropItem_CabinetToInven(nFlags, bTargetUI, bSlotID); break;
			}
		}
		break;
	case TFRAME_PET_MANAGE:
		{
			switch(bTargetUI)
			{
			case TFRAME_GAUGE: DropItem_PetToMain(nFlags, bTargetUI, bSlotID); break;
			}
		}
		break;
	}
}

void CTClientGame::DropItem_InvenToInven( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( m_pMainChar->m_bPrivateShop )
	{
		ResetInvenUI();
		return;
	}
	BYTE bInvenID = GetInvenID(m_vDRAG.m_bFrameID);

	MAPTINVEN::iterator itTINVEN = m_pMainChar->m_mapTINVEN.find(bInvenID);
	if( itTINVEN == m_pMainChar->m_mapTINVEN.end() )
	{
		ResetInvenUI();
		return;
	}

	MAPTITEM::iterator itTITEM = (*itTINVEN).second->m_mapTITEM.find(m_vDRAG.m_bSlotID);
	if( itTITEM == (*itTINVEN).second->m_mapTITEM.end() )
	{
		ResetInvenUI();
		return;
	}

	m_bInvenID = bInvenID;
	m_bSlotID = m_vDRAG.m_bSlotID;
	m_bTargetInvenID = GetInvenID(bTargetUI);
	m_bTargetSlotID = bSlotID;

	if( (*itTITEM).second->GetCount() > 1 && (nFlags & MK_SHIFT) )
	{
		CString strEDITTEXT;
		CString strTITLE;
		CString strEDIT;

		strEDIT.Format( _T("%d"), (*itTITEM).second->GetCount() / 2);
		strTITLE = CTChart::LoadString( TSTR_MOVEITEM_COUNT);
		strEDITTEXT = CTChart::LoadString( TSTR_EA);

		m_pMainWnd->MessageBox(
			strTITLE,
			strEDITTEXT,
			strEDIT,
			TSTR_OK,
			TSTR_CANCEL,
			ID_FRAME_2BTNMSGBOX,
			GM_DIVIDE_ITEM,
			GM_DROP_ITEM_NO,
			0,
			TRUE,
			TRUE);
	}
	else
	{
		m_pSESSION->SendCS_MOVEITEM_REQ(
			bInvenID,
			m_vDRAG.m_bSlotID,
			m_bTargetInvenID,
			bSlotID,
			(*itTITEM).second->GetCount());
	}
}

void CTClientGame::DropItem_InvenToItemshop( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( m_vDRAG.m_bFrameID != TFRAME_CHAR_INFO )
	{
		m_bInvenID = GetInvenID(m_vDRAG.m_bFrameID);
		m_bSlotID = m_vDRAG.m_bSlotID;

		OnGM_ITEM_SELL();
	}

	ResetInvenUI();
}

void CTClientGame::DropItem_InvenToMainUI( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	MAPTINVEN::iterator itTINVEN = m_pMainChar->m_mapTINVEN.find(GetInvenID(m_vDRAG.m_bFrameID));
	if( itTINVEN == m_pMainChar->m_mapTINVEN.end() )
	{
		ResetInvenUI();
		return;
	}

	MAPTITEM::iterator itTITEM = (*itTINVEN).second->m_mapTITEM.find(m_vDRAG.m_bSlotID);
	if( itTITEM == (*itTINVEN).second->m_mapTITEM.end() )
	{
		ResetInvenUI();
		return;
	}

	BOOL bNeedReset = TRUE;

	TMAINSLOTID_TYPE eSlotType = CTMainUI::ToSlotType(bSlotID);
	BYTE bSlotNum = CTMainUI::ToSlotNumber(bSlotID);
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	if( eSlotType == TMAINSLOTID_HOTKEY_DEF )
	{
		m_pSESSION->SendCS_HOTKEYADD_REQ(
			HOTKEY_ITEM,
			(*itTITEM).second->GetTITEM()->m_wItemID,
			pTGaugePannel->m_bHotkeyID[THOTKEYBASE_DEF],
			bSlotNum);
	}
	else if( eSlotType == TMAINSLOTID_HOTKEY_SUB1 )
	{
		m_pSESSION->SendCS_HOTKEYADD_REQ(
			HOTKEY_ITEM,
			(*itTITEM).second->GetTITEM()->m_wItemID,
			pTGaugePannel->m_bHotkeyID[THOTKEYBASE_SUB1],
			bSlotNum);
	}
	else if( eSlotType == TMAINSLOTID_INVEN && 
		(*itTITEM).second->GetTITEM()->m_bType == IT_INVEN &&
		m_vDRAG.m_bFrameID != TFRAME_CHAR_INFO && 
		!m_pMainChar->m_bPrivateShop )
	{
		m_pSESSION->SendCS_INVENADD_REQ(
			bSlotNum,
			(*itTINVEN).second->m_bInvenID,
			(*itTITEM).second->GetItemSlot());

		bNeedReset = FALSE;
	}

	if( bNeedReset )
		ResetInvenUI();
}

void CTClientGame::DropItem_InvenToTrade( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	CTTradeDlg *pTTradeDlg = (CTTradeDlg *) m_vTFRAME[TFRAME_TRADE];

	BYTE bInvenID = GetInvenID(m_vDRAG.m_bFrameID);
	if( pTTradeDlg->GetItemIdxByInvenPos(bInvenID, m_vDRAG.m_bSlotID) == T_INVALID )
	{
		CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);
		if( pTInven )
		{
			CTClientItem* pTItem = pTInven->FindTItem(m_vDRAG.m_bSlotID);
			if( pTItem &&
				(pTItem->GetWrap() || (pTItem->GetTITEM()->m_bCanTrade & (1<<TTRADEOP_TRADE))) )
			{
				pTTradeDlg->AddItem(FALSE, pTItem, bInvenID, m_vDRAG.m_bSlotID);
			}
			else
			{
				m_pMainWnd->MessageBoxOK(
					TSTR_ITEM_CANNOT_TRADE,
					TSTR_OK,
					GM_CLOSE_MSGBOX,
					0,
					TRUE,
					ID_SND_ERROR);
			}
		}
	}

	ResetInvenUI();
}

void CTClientGame::DropItem_InvenToItemSlot( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	ResetInvenUI();

	BYTE bInvenID = GetInvenID(m_vDRAG.m_bFrameID);
	CTClientItem* pTItem = FindMainItemByInven(bInvenID, m_vDRAG.m_bSlotID);
	if( !pTItem )
		return;

	CTItemSlotBaseDlg* pTargetDlg = (CTItemSlotBaseDlg*) m_vTFRAME[bTargetUI];
	if( !pTargetDlg->CheckFrame(m_vDRAG.m_bFrameID) )
		return;

	if( bSlotID >= pTargetDlg->GetSlotCount() )
		return;
	
	if( !pTargetDlg->SetItem(
			bSlotID, 
			pTItem->GetTITEM(), 
			bInvenID,
			m_vDRAG.m_bSlotID) )
	{
		const CString& strERR = pTargetDlg->GetLastErrorMsg();
		if( !strERR.IsEmpty() )
			ShowInfoMsgBox(strERR, TRUE, TUISND_TYPE_ERROR);
	}
}

void CTClientGame::DropItem_InvenToPrivateSell( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	CTPrivateShopDlgForSell* pPrvShop = (CTPrivateShopDlgForSell*) m_vTFRAME[TFRAME_PRIVATE_SELL];
	if( !pPrvShop->IsSelling() )
	{
		BYTE bInvenID = GetInvenID(m_vDRAG.m_bFrameID);
		if( pPrvShop->GetItemIdxByInvenPos(bInvenID, m_vDRAG.m_bSlotID) == T_INVALID )
		{
			CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);
			if( pTInven )
			{
				CTClientItem* pTItem = pTInven->FindTItem(m_vDRAG.m_bSlotID);
				if( pTItem &&
					(pTItem->GetWrap() || (pTItem->GetTITEM()->m_bCanTrade & (1<<TTRADEOP_TRADE))) )
				{
					CTItemRegBox* pBox = static_cast<CTItemRegBox*>(m_vTFRAME[TFRAME_ITEM_REGBOX]);
					pBox->SetItem(pTItem, bInvenID, m_vDRAG.m_bSlotID);
					EnableUI(TFRAME_ITEM_REGBOX);
				}
				else
				{
					m_pMainWnd->MessageBoxOK(
						TSTR_ITEM_CANNOT_TRADE,
						TSTR_OK,
						GM_CLOSE_MSGBOX,
						0,
						TRUE,
						ID_SND_ERROR);
				}
			}
		}
	}

	ResetInvenUI();
}

void CTClientGame::DropItem_InvenToMailSend( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	BYTE bInvenID = GetInvenID(m_vDRAG.m_bFrameID);
	CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);
	if( pTInven )
	{
		CTClientItem* pTItem = pTInven->FindTItem(m_vDRAG.m_bSlotID);
		if( pTItem &&
			(pTItem->GetWrap() || (pTItem->GetTITEM()->m_bCanTrade & (1<<TTRADEOP_TRADE))) )
		{
			CTMailSendDlg* pMailSnd = (CTMailSendDlg*) m_vTFRAME[TFRAME_MAIL_SEND];
			pMailSnd->SetItem(pTItem, bInvenID, m_vDRAG.m_bSlotID);
		}
		else
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_ITEM_CANNOT_TRADE,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0,
				TRUE,
				ID_SND_ERROR);
		}
	}

	ResetInvenUI();
}

void CTClientGame::DropItem_InvenToCabinet( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	BYTE bInvenID = GetInvenID(m_vDRAG.m_bFrameID);
	CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);
	if( pTInven )
	{
		CTClientItem* pTItem = pTInven->FindTItem(m_vDRAG.m_bSlotID);
		if( pTItem && (pTItem->GetTITEM()->m_bCanTrade & (1<<TTRADEOP_CABINET)) )
		{
			CTNormalCabinetDlg* pDlg = (CTNormalCabinetDlg*) m_vTFRAME[TFRAME_CABINET];

			if( !pDlg->m_pOpenByCash )
			{
				m_pSESSION->SendCS_CABINETPUTIN_REQ(
					pDlg->GetSelectCab(),
					bInvenID, 
					m_vDRAG.m_bSlotID, 
					pTItem->GetCount(),
					INVEN_NULL,
					INVALID_SLOT );
			}
			else
			{
				m_pSESSION->SendCS_CABINETPUTIN_REQ(
					pDlg->GetSelectCab(),
					bInvenID, 
					m_vDRAG.m_bSlotID, 
					pTItem->GetCount(),
					pDlg->m_pOpenByCash->m_bInvenID,
					pDlg->m_pOpenByCash->m_bSlotID );
			}
		}
		else
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_ITEM_CANNOT_CABINET,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0,
				TRUE,
				ID_SND_ERROR);
		}
	}

	ResetInvenUI();
}

void CTClientGame::DropItem_InvenToGuildCabinet( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	BYTE bInvenID = GetInvenID(m_vDRAG.m_bFrameID);
	CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);
	if( pTInven )
	{
		CTClientItem* pTItem = pTInven->FindTItem(m_vDRAG.m_bSlotID);
		if( pTItem && (pTItem->GetTITEM()->m_bCanTrade & (1<<TTRADEOP_CABINET)) )
		{
			m_pSESSION->SendCS_GUILDCABINETPUTIN_REQ(
				bInvenID, 
				m_vDRAG.m_bSlotID, 
				pTItem->GetCount());
		}
		else
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_ITEM_CANNOT_CABINET,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0,
				TRUE,
				ID_SND_ERROR);
		}
	}

	ResetInvenUI();
}

void CTClientGame::DropItem_InvenToCashCabinet( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( CTClientGame::m_bSendedCS_CASHITEMPUTIN_REQ )
		return ;

	BYTE bInvenID = GetInvenID(m_vDRAG.m_bFrameID);
	CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);

	if( pTInven )
	{
		CTClientItem* pTItem = pTInven->FindTItem(m_vDRAG.m_bSlotID);
		CTCashCabinetDlg* pDlg = static_cast<CTCashCabinetDlg*>(m_vTFRAME[TFRAME_CASHCABINET]);

		if( pTItem &&
			(pTItem->GetTITEM()->m_bCanTrade & (1<<TTRADEOP_CABINET)) )
		{
			if( pTItem->GetTITEM()->m_bIsSpecial == 1 ||
				(pDlg->IsPeriodRental() && pDlg->m_dwItemCount < CASHCABINET_MAX_ITEM_ON_PR) )
			{
				m_pSESSION->SendCS_CASHITEMPUTIN_REQ(
					bInvenID,
					m_vDRAG.m_bSlotID);
			}
			else
			{
				DWORD dwPARAM = MAKELONG( bInvenID, m_vDRAG.m_bSlotID );

				if( CTNationOption::RUSSIA )
				{
					m_pCommandHandler->m_vCOMMAND.push_back( TCOMMAND( GM_CASHCABINET_PUTIN, dwPARAM ) );
				}
				else
				{
					DWORD dwMSG;

					if( pDlg->IsPeriodRental() )
						dwMSG = TSTR_CASHCABINET_PUTIN_PR;
					else
						dwMSG = TSTR_CASHCABINET_PUTIN;

					m_pMainWnd->MessageBoxYesNo(
						dwMSG,
						TSTR_OK,
						TSTR_CANCEL,
						TCOMMAND(GM_CASHCABINET_PUTIN, dwPARAM),
						GM_CLOSE_MSGBOX_RESETINVEN,
						GM_CLOSE_MSGBOX_RESETINVEN );
				}
			}
		}
		else
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_ITEM_CANNOT_CABINET,
				TSTR_OK,
				GM_CLOSE_MSGBOX_RESETINVEN,
				0,
				TRUE,
				ID_SND_ERROR);
		}
	}
}

void CTClientGame::DropItem_InvenToAuctionReg( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	BYTE bInvenID = GetInvenID( m_vDRAG.m_bFrameID );
	CTClientInven* pTInven = m_pMainChar->FindTInven( bInvenID );

	if( pTInven )
	{
		CTClientItem* pItem = pTInven->FindTItem( m_vDRAG.m_bSlotID );
		CTAuctionMainFrame* pFrame = static_cast< CTAuctionMainFrame* >( m_vTFRAME[ TFRAME_AUCTION ] );
		CTAuctionRegist* pRegFrame = static_cast< CTAuctionRegist* >( pFrame->GetInnerFrame( TAUCTION_REGIST ) );
		if( pRegFrame->IsVisible() )
		{
			if( pItem &&
				pItem->GetWrap() || 
				pItem->GetTITEM()->m_bCanTrade & 1 ) // 거래가능 아이템
			{
				pRegFrame->SetItem( pItem );
				pRegFrame->SetInvenID( bInvenID );
				pRegFrame->SetItemCount( pItem->GetCount() );
				INT64 ldwStartPrice = (INT64)max( pItem->GetSellPrice(), 1 );
				pRegFrame->SetStartPrice( ldwStartPrice );

				if( pItem->GetTITEM()->m_bStack > 1 )
				{
					INT64 ldwBuyPrice = (INT64)max( pItem->GetSellPrice(), 1 );
					pRegFrame->SetBuyPrice( ldwBuyPrice * pItem->GetCount() );
					pRegFrame->SetEditEnable( FALSE );
					pRegFrame->SetEditUnitPrice_RuneLength( CTAuctionRegist::TRUNE_LENGTH_STACKITEM );
				}
				else
				{
					INT64 ldwBuyPrice = (INT64)max( pItem->GetPrice(), ldwStartPrice + 1 );
					pRegFrame->SetBuyPrice( ldwBuyPrice );
					pRegFrame->SetEditEnable( TRUE );
					pRegFrame->SetEditUnitPrice_RuneLength( CTAuctionRegist::TRUNE_LENGTH_ITEM );
				}

				pRegFrame->UpdateByComp();
			}
			else	// 거래불가.
			{
				m_pMainWnd->MessageBoxOK(
					TSTR_ITEM_CANNOT_TRADE,
					TSTR_OK,
					GM_CLOSE_MSGBOX_RESETINVEN,
					0 );
			}
		}
	}

	ResetInvenUI();
}

void CTClientGame::DropItem_InvenToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( m_pMainChar->m_bPrivateShop )
	{
		ResetInvenUI();
		return;
	}

	m_bInvenID = GetInvenID(m_vDRAG.m_bFrameID);
	m_bSlotID = m_vDRAG.m_bSlotID;

	CTClientInven *pTINVEN = m_pMainChar->FindTInven(m_bInvenID);
	if(!pTINVEN)
		return;

	CTClientItem *pTITEM = pTINVEN->FindTItem(m_bSlotID);
	if(!pTITEM)
		return;

	if( ( pTITEM->GetTITEM()->m_bCanGrade != 0 ||
		pTITEM->GetTITEM()->m_bIsSpecial != 0 ) &&
		pTITEM->GetGrade() > 0 )
	{
		ResetInvenUI();
		OnGM_WARN_GRADE_ITEM_DROP();
	}
	else
	{
		m_pMainWnd->MessageBoxYesNo(
			TSTR_DROP_ITEM,
			TSTR_YES,
			TSTR_NO,
			GM_DROP_ITEM_YES,
			GM_DROP_ITEM_NO,
			GM_DROP_ITEM_NO,
			FALSE);
	}
}

void CTClientGame::DropItem_TradeToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( bTargetUI != TFRAME_TRADE )
	{
		CTTradeDlg *pTTradeDlg = (CTTradeDlg *) m_vTFRAME[TFRAME_TRADE];
		pTTradeDlg->RemoveItem(FALSE, m_vDRAG.m_bSlotID);
	}
}

void CTClientGame::DropItem_MailSendToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	CTMailSendDlg* pMailSnd = (CTMailSendDlg *) m_vTFRAME[TFRAME_MAIL_SEND];
	pMailSnd->SetItem(NULL);
}

void CTClientGame::DropItem_PrivateSellToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( bTargetUI != TFRAME_PRIVATE_SELL )
	{
		CTPrivateShopDlgForSell* pPrvDlg = (CTPrivateShopDlgForSell *) m_vTFRAME[TFRAME_PRIVATE_SELL];
		if( !pPrvDlg->IsSelling() )
			pPrvDlg->RemoveItem(m_vDRAG.m_bSlotID);
	}
}

void CTClientGame::DropItem_SkillToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( !m_pSESSION || bTargetUI != TFRAME_GAUGE )
		return; 
		
	TMAINSLOTID_TYPE eSlotType = CTMainUI::ToSlotType(bSlotID);
	BYTE bSlotNum = CTMainUI::ToSlotNumber(bSlotID);			
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	if( eSlotType == TMAINSLOTID_HOTKEY_DEF ||
		eSlotType == TMAINSLOTID_HOTKEY_SUB1 )
	{
		CTSkillDlg *pTSkillDlg = (CTSkillDlg *) m_vTFRAME[TFRAME_SKILL];

		WORD wSID = CTChart::GetTSKILLID(
			m_pMainChar->m_bContryID,
			m_pMainChar->m_bClassID,
			pTSkillDlg->GetCurSel(),
			m_vDRAG.m_bSlotID);

		CTClientSkill *pTSKILL = m_pMainChar->FindTSkill(wSID);

		if( pTSKILL )
		{
			BYTE bHotkeyID;
			if( eSlotType == TMAINSLOTID_HOTKEY_DEF )
				bHotkeyID = pTGaugePannel->m_bHotkeyID[THOTKEYBASE_DEF];
			else
				bHotkeyID = pTGaugePannel->m_bHotkeyID[THOTKEYBASE_SUB1];

			m_pSESSION->SendCS_HOTKEYADD_REQ(
				HOTKEY_SKILL,
				pTSKILL->m_pTSKILL->m_wSkillID,
				bHotkeyID,
				bSlotNum);
		}
	}
}

void CTClientGame::DropItem_RentSkillToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( !m_pSESSION || bTargetUI != TFRAME_GAUGE )
		return; 

	TMAINSLOTID_TYPE eSlotType = CTMainUI::ToSlotType(bSlotID);
	BYTE bSlotNum = CTMainUI::ToSlotNumber(bSlotID);
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	if( eSlotType == TMAINSLOTID_HOTKEY_DEF ||
		eSlotType == TMAINSLOTID_HOTKEY_SUB1 )
	{
		CTRentSkillDlg *pTRentDlg = (CTRentSkillDlg *) m_vTFRAME[TFRAME_RENT_SKILL];
		CTRentNpcItem* pSkill = static_cast<CTRentNpcItem*>( pTRentDlg->GetList()->GetSelectItem() );

		if( pSkill )
		{
			BYTE bHotkeyID;
			if( eSlotType == TMAINSLOTID_HOTKEY_DEF )
				bHotkeyID = pTGaugePannel->m_bHotkeyID[THOTKEYBASE_DEF];
			else
				bHotkeyID = pTGaugePannel->m_bHotkeyID[THOTKEYBASE_SUB1];

			m_pSESSION->SendCS_HOTKEYADD_REQ(
				HOTKEY_SKILL,
				pSkill->m_pSkill->m_wSkillID,
				bHotkeyID,
				bSlotNum);
		}
	}
}

void CTClientGame::DropItem_MainUIToInven( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if( !m_pSESSION || m_pMainChar->m_bPrivateShop )
	{
		ResetInvenUI();
		return;
	}

	TMAINSLOTID_TYPE eSlotType = CTMainUI::ToSlotType(m_vDRAG.m_bSlotID);
	
	if( eSlotType == TMAINSLOTID_INVEN )
	{
		m_pSESSION->SendCS_INVENDEL_REQ(
			CTMainUI::ToSlotNumber(m_vDRAG.m_bSlotID),
			GetInvenID(bTargetUI),
			bSlotID);
	}
}

void CTClientGame::DropItem_MainUIToMainUI( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if(!m_pSESSION)
		return;

	if( m_pTRSCS->m_bDropping && m_pTRSCS->IsVisible() )
	{
		LPTUNIT pTUNIT = (LPTUNIT) m_pTRSCS->m_pTUNIT->GetItemData( m_vDRAG.m_bSlotID, 0);
		LPTSQUAD pTSQUAD = (LPTSQUAD) m_pTRSCS->m_pTSQUAD->GetItemData( bSlotID, 0);

		if( pTSQUAD && pTUNIT && pTSQUAD->m_vTUNIT.size() < (MAX_PARTY+1) && !m_pTRSCS->FindTUNIT( pTSQUAD, pTUNIT->m_dwCharID) )
		{
			m_pSESSION->SendCS_PARTYMOVE_REQ( pTUNIT->m_strNAME, _T(""), pTSQUAD->m_wPartyID);
			return;
		}
	}

	TMAINSLOTID_TYPE eSlotType = CTMainUI::ToSlotType(m_vDRAG.m_bSlotID);
	BYTE bSlotNum = CTMainUI::ToSlotNumber(m_vDRAG.m_bSlotID);

	if( eSlotType == TMAINSLOTID_HOTKEY_DEF ||
		eSlotType == TMAINSLOTID_HOTKEY_SUB1 )
	{
		THOTKEY_BASE eHotkeyType;
		if( eSlotType == TMAINSLOTID_HOTKEY_DEF )
			eHotkeyType = THOTKEYBASE_DEF;
		else
			eHotkeyType = THOTKEYBASE_SUB1;

		CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
		LPTHOTKEY pTHOTKEY = pTGaugePannel->GetHotKey(eHotkeyType, bSlotNum);
		if( !pTHOTKEY )
			return;

		if( pTHOTKEY->m_bType == HOTKEY_NONE )
		{
			m_pSESSION->SendCS_HOTKEYDEL_REQ(
				pTGaugePannel->m_bHotkeyID[eHotkeyType],
				bSlotNum);
		}

		TMAINSLOTID_TYPE eTargetSlotType = CTMainUI::ToSlotType(bSlotID);
		BYTE bTargetSlotNum = CTMainUI::ToSlotNumber(bSlotID);

		THOTKEY_BASE eTargetHotkeyType;

		switch( eTargetSlotType )
		{
		case TMAINSLOTID_HOTKEY_DEF		:
			eTargetHotkeyType = THOTKEYBASE_DEF;
			break;
		case TMAINSLOTID_HOTKEY_SUB1	:
			eTargetHotkeyType = THOTKEYBASE_SUB1;
			break;

		default							:
			{
				m_pSESSION->SendCS_HOTKEYDEL_REQ(
					pTGaugePannel->m_bHotkeyID[eHotkeyType],
					bSlotNum);
			}
			return;
		}

		LPTHOTKEY pTHOTKEY_TARGET = pTGaugePannel->GetHotKey(eTargetHotkeyType, bTargetSlotNum);
		if( pTHOTKEY_TARGET && pTHOTKEY_TARGET->m_bType != HOTKEY_NONE )
		{
			m_pSESSION->SendCS_HOTKEYADD_REQ(
				pTHOTKEY_TARGET->m_bType,
				pTHOTKEY_TARGET->m_wID,
				pTGaugePannel->m_bHotkeyID[eHotkeyType],
				bSlotNum);
		}
		else if( eHotkeyType != eTargetHotkeyType )
		{
			m_pSESSION->SendCS_HOTKEYDEL_REQ(
				pTGaugePannel->m_bHotkeyID[eHotkeyType],
				bSlotNum);
		}

		if( pTHOTKEY && pTHOTKEY->m_bType != HOTKEY_NONE )
		{
			m_pSESSION->SendCS_HOTKEYADD_REQ(
				pTHOTKEY->m_bType,
				pTHOTKEY->m_wID,
				pTGaugePannel->m_bHotkeyID[eTargetHotkeyType],
				bTargetSlotNum);
		}
	}
	else if( eSlotType == TMAINSLOTID_INVEN )
	{
		if( !m_pMainChar->m_bPrivateShop )
		{
			TMAINSLOTID_TYPE eTargetSlotType = CTMainUI::ToSlotType(bSlotID);

			if( eTargetSlotType == TMAINSLOTID_INVEN )
			{
				m_pSESSION->SendCS_INVENMOVE_REQ(
					bSlotNum,
					CTMainUI::ToSlotNumber(bSlotID));
			}
		}
		else
			ResetInvenUI();
	}
}

void CTClientGame::DropItem_MainUIToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	if(!m_pSESSION)
		return;

	if( m_vDRAG.m_dwParam == -1 ) // 이 문장과 관련된 로직 : CTRSCSDlg::OnBeginDrag
		return ;

	TMAINSLOTID_TYPE eSlotType = CTMainUI::ToSlotType(m_vDRAG.m_bSlotID);
	BYTE bSlotNum = CTMainUI::ToSlotNumber(m_vDRAG.m_bSlotID);
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	switch(eSlotType)
	{
	case TMAINSLOTID_HOTKEY_DEF		:
		{
			m_pSESSION->SendCS_HOTKEYDEL_REQ(
				pTGaugePannel->m_bHotkeyID[THOTKEYBASE_DEF],
				bSlotNum);
		}
		break;

	case TMAINSLOTID_HOTKEY_SUB1	:
		{
			m_pSESSION->SendCS_HOTKEYDEL_REQ(
				pTGaugePannel->m_bHotkeyID[THOTKEYBASE_SUB1],
				bSlotNum);
		}
		break;

	case TMAINSLOTID_INVEN			:
		{
			ResetInvenUI();
		}
		break;
	}
}

void CTClientGame::DropItem_ShopToInven( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	m_bInvenID = GetInvenID(bTargetUI);
	m_bSlotID = bSlotID;

	if( m_vDRAG.m_bFrameID == TFRAME_ITEM_SHOP )
		OnGM_ITEM_BUY();
	else
		OnGM_BUY_CASHITEM();
}

void CTClientGame::DropItem_MonInvenToInven( UINT nFlags, BYTE bTargetUI, BYTE bSlotID )
{
	m_bInvenID = GetInvenID(bTargetUI);
	m_bSlotID = bSlotID;
	m_bMonSlotID = m_vDRAG.m_bSlotID;

	OnGM_ITEM_TAKE();
}

void CTClientGame::DropItem_CabinetToInven(UINT nFlags, BYTE bTargetUI, BYTE bSlotID)
{
	m_bInvenID = GetInvenID(bTargetUI);
	m_bSlotID = bSlotID;

	CTCabinetDlg* pDlg = static_cast<CTCabinetDlg*>(m_vTFRAME[m_vDRAG.m_bFrameID]);
	CTCabinetItem* pCabItem = pDlg->GetCabItemByStID(m_vDRAG.m_bSlotID, m_vDRAG.m_dwParam);

	if( pCabItem )
	{
		switch( m_vDRAG.m_bFrameID )
		{
		case TFRAME_CABINET		:
			{
				CTNormalCabinetDlg* pNORMALCABINETDLG = (CTNormalCabinetDlg*) m_vTFRAME[ TFRAME_CABINET ];
				if( !pNORMALCABINETDLG->m_pOpenByCash )
				{
					m_pSESSION->SendCS_CABINETTAKEOUT_REQ(
						m_vDRAG.m_bSlotID,
						m_vDRAG.m_dwParam,
						pCabItem->m_bItemCnt,
						m_bInvenID,
						m_bSlotID,
						INVEN_NULL,
						INVALID_SLOT );
				}
				else
				{
					m_pSESSION->SendCS_CABINETTAKEOUT_REQ(
						m_vDRAG.m_bSlotID,
						m_vDRAG.m_dwParam,
						pCabItem->m_bItemCnt,
						m_bInvenID,
						m_bSlotID,
						pNORMALCABINETDLG->m_pOpenByCash->m_bInvenID,
						pNORMALCABINETDLG->m_pOpenByCash->m_bSlotID );
				}
			}
			break;
		case TFRAME_GUILDCABINET:
			{
				m_pSESSION->SendCS_GUILDCABINETTAKEOUT_REQ(
					m_vDRAG.m_dwParam,
					pCabItem->m_bItemCnt,
					m_bInvenID,
					m_bSlotID);
			}
			break;
		case TFRAME_CASHCABINET	:
			{
				m_pSESSION->SendCS_CASHITEMGET_REQ(
					m_vDRAG.m_dwParam,
					m_bInvenID,
					m_bSlotID);
			}
			break;
		}
	}
}

void CTClientGame::DropItem_PetToMain(UINT nFlags, BYTE bTargetUI, BYTE bSlotID)
{
	TMAINSLOTID_TYPE eSlotType = CTMainUI::ToSlotType(bSlotID);
	BYTE bSlotNum = CTMainUI::ToSlotNumber(bSlotID);
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	if( eSlotType == TMAINSLOTID_HOTKEY_DEF )
	{
		m_pSESSION->SendCS_HOTKEYADD_REQ(
			HOTKEY_PET,
			(WORD) m_vDRAG.m_dwParam,
			pTGaugePannel->m_bHotkeyID[THOTKEYBASE_DEF],
			bSlotNum);
	}
	else if( eSlotType == TMAINSLOTID_HOTKEY_SUB1 )
	{
		m_pSESSION->SendCS_HOTKEYADD_REQ(
			HOTKEY_PET,
			(WORD) m_vDRAG.m_dwParam,
			pTGaugePannel->m_bHotkeyID[THOTKEYBASE_SUB1],
			bSlotNum);
	}
}

void CTClientGame::CalcGhost( DWORD dwTick)
{
	FLOAT fDist = D3DXVec3Length(&D3DXVECTOR3(
		m_pMainChar->GetPositionX() - m_pMainChar->m_vTDEAD.x,
		m_pMainChar->GetPositionY() - m_pMainChar->m_vTDEAD.y,
		m_pMainChar->GetPositionZ() - m_pMainChar->m_vTDEAD.z));
	BYTE bCheck = CheckRevival( &(m_pMainChar->m_vTDEAD) );

	D3DXVECTOR3 vPOS(
		m_vMainDead.GetPositionX(),
		m_vMainDead.GetPositionY(),
		m_vMainDead.GetPositionZ());

	if( bCheck && fDist <= TREVIVAL_DIST && !m_bRevival )
	{
		DisableUI(TFRAME_WORLDMAP);

		m_pMainWnd->MessageBoxOK(
			TSTR_REVIVAL_MSG,
			TSTR_REVIVAL,
			GM_REVIVAL,
			0,
			FALSE);

		m_bRevival = TRUE;
	}

	if( m_bRevival && (!bCheck || fDist > TREVIVAL_DIST) )
	{
		m_pMainWnd->CloseMessageBox();
		m_bRevival = FALSE;
	}

	if( D3DXVec2Length( &D3DXVECTOR2(
		vPOS.x - m_pMainChar->GetPositionX(),
		vPOS.z - m_pMainChar->GetPositionZ())) < CTClientObjBase::m_fCamDist )
	{
		m_vMainDead.CalcTick(
			&vPOS,
			m_pDevice,
			m_pRES,
			dwTick);

		m_vMainDead.CalcHeight(
			&vPOS,
			&m_vMAP,
			dwTick);
	}
}

void CTClientGame::CheckPOPUP()
{
	if(!m_pTPOPUP->HitTest(m_vKEY.m_point))
		m_pTPOPUP->ShowComponent(FALSE);
}

void CTClientGame::CheckHUD()
{
	static BYTE bHUD = FALSE;

	if( m_vTOPTION.m_bHUD != bHUD )
	{
		bHUD = m_vTOPTION.m_bHUD;

		ResetHUDTargetINFO(FALSE);
		ResetPlayerINFO(FALSE);
	}
}

void CTClientGame::CheckMainUI()
{
	BYTE h(0), i(0);
	m_pCtrlHost = GetCtrlOBJ();
	CTGaugePannel* pUI = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);

	for( h=0; h<THOTKEYBASE_COUNT; ++h)
	{
		for( i=0; i<MAX_HOTKEY_POS; ++i)
		{
			if( pUI->m_pTHOTKEYDOWN[h][i]->IsVisible() && pUI->m_pTHOTKEYDOWN[h][i]->GetCurImage() )
			{
				switch(pUI->m_vTHOTKEY[pUI->m_bHotkeyID[h]][i].m_bType)
				{
				case HOTKEY_SKILL	:
					{
						CTClientSkill *pTSKILL = m_pMainChar->FindTSkill(pUI->m_vTHOTKEY[pUI->m_bHotkeyID[h]][i].m_wID);

						if(pTSKILL)
							if(pTSKILL->m_bLoopON)
								pTSKILL->m_bLoopON = FALSE;
							else
								UseTSkill( pTSKILL->m_pTSKILL->m_wSkillID, FALSE, TRUE);
					}

					break;

				case HOTKEY_ITEM	: UseTItem(pUI->m_vTHOTKEY[pUI->m_bHotkeyID[h]][i].m_wID); break;
				case HOTKEY_PET :
					{
						if( pUI->m_pTHOTKEY[h][i][1]->IsEnable() )
							UseTPet(pUI->m_vTHOTKEY[pUI->m_bHotkeyID[h]][i].m_wID);
					}
					break;
				}

				return;
			}
		}
	}

	for( i=0; i<TMONSKILL_COUNT; i++)
		if( pUI->m_pTRECALLSKILLDOWN[i]->IsVisible() && pUI->m_pTRECALLSKILLDOWN[i]->GetCurImage() )
		{
			if( m_pTARGET && CanControl() )
				SetRecallTarget( m_pTARGET->m_dwID, m_pTARGET->m_bType, i);

			return;
		}

	for( i=0; i<TRECALLAI_COUNT; i++)
		if( pUI->m_pTRECALLAIDOWN[i]->IsVisible() && pUI->m_pTRECALLAIDOWN[i]->GetCurImage() )
		{
			if(CanControl())
				switch(i)
				{
				case TRECALLAI_PASSIVE	: OnGM_RECALL_PASSIVE_AI(); break;
				case TRECALLAI_ACTIVE	: OnGM_RECALL_ACTIVE_AI(); break;
				case TRECALLAI_MANUAL	: OnGM_RECALL_MANUAL_AI(); break;
				case TRECALLAI_BACK		: OnGM_RECALL_BACK_AI(); break;
				case TRECALLAI_STAY		: OnGM_RECALL_STAY_AI(); break;
				}

			return;
		}
}

void CTClientGame::DoPOPUP(LPTSKILL pTSKILL)
{
	static DWORD dwMenuID[][2] = {
		{ GM_END_MAINTAIN, TSTR_MENU_END_MAINTAIN},
		{ GM_CLOSE_POPUP, TSTR_CANCEL}};

	m_pSelectSkill = pTSKILL;

	for( BYTE i=0; i<TMAINTAINMENU_COUNT; i++)
		m_pTPOPUP->AddMENU( dwMenuID[i][1], dwMenuID[i][0]);

	ShowPOPUP();
}

void CTClientGame::DoPOPUP(LPTPARTY pTPARTY)
{
	static DWORD dwMenuID[][2] = {
		{ GM_PARTYMEMBERRECALL, TSTR_MENU_PARTYMEMBERRECALL},
		{ GM_FARPARTY_SETCHIEF, TSTR_MENU_SETCHIEF},
		{ GM_PARTY_KICK, TSTR_MENU_PARTY_KICK},
		{ GM_FARFRIEND_REQ, TSTR_MENU_FRIEND_REQ},
		{ GM_CLOSE_POPUP, TSTR_CANCEL}};

	WORD wPartyID = GetPartyID(m_pMainChar);

	for( BYTE i=0; i<TFARPARTYMENU_COUNT; i++)
	{
		BYTE bADD = TRUE;

		switch(dwMenuID[i][0])
		{
		case GM_PARTYMEMBERRECALL		:
			{
				BYTE bInven, bSlot;
				if( !wPartyID || !FindInven(IT_USE, IK_MEMBERRECALL, bInven, bSlot) )
					bADD = FALSE;
			}
			break;

		case GM_FARPARTY_SETCHIEF		:
			if( !wPartyID || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID )
				bADD = FALSE;

			break;

		case GM_PARTY_KICK				:
			if( !wPartyID || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID )
				bADD = FALSE;

			break;

		case GM_FARFRIEND_REQ			:
			{
				CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
				if( pList->FindGroupByName(pTPARTY->m_strNAME) )
					bADD = FALSE;
			}

			break;
		}

		if(bADD)
			m_pTPOPUP->AddMENU( dwMenuID[i][1], dwMenuID[i][0]);
	}

	ShowPOPUP();
}

void CTClientGame::DoPOPUP( CTClientObjBase *pTOBJ)
{
	OnGM_CLOSE_POPUP();

	if( !pTOBJ || (!pTOBJ->m_bDrawGhost && (m_pMainChar->IsDead() || m_pMainChar->m_bGhost)) || m_pMainChar->m_bPrivateShop )
		return;

	switch(pTOBJ->m_bType)
	{
	case OT_RECALL	:
	case OT_SELF	: BuildRECALLMENU((CTClientRecall *) pTOBJ); break;
	case OT_NPC		: BuildNPCMENU((CTClientNpc *) pTOBJ); break;
	case OT_PC		:
		if( pTOBJ == m_pMainChar )
			BuildMAINCHARMENU();
		else
			BuildPCMENU((CTClientChar *) pTOBJ);
	}

	ShowPOPUP();
}

void CTClientGame::DoPOPUP( LPTSQUAD pTSQUAD)
{
	static DWORD dwMenuID[][2] = {
		{ GM_CORPS_SETCOMMANDER, TSTR_MENU_SETCOMMANDER},
		{ GM_CORPS_LEAVE, TSTR_MENU_CORPS_KICK},
		{ GM_CLOSE_POPUP, TSTR_CANCEL}};

	OnGM_CLOSE_POPUP();

	if( m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
	{
		CorpsMSG( m_pMainChar, CORPS_DEAD);
		return;
	}

	if(!pTSQUAD)
		return;

	WORD wSquadID = GetPartyID(m_pMainChar);

	DWORD dwParam = 0;
	for( BYTE i=0; i<TSQUADMENU_COUNT; i++)
	{
		BYTE bADD = TRUE;

		switch(dwMenuID[i][0])
		{
		case GM_CORPS_SETCOMMANDER	:
			if( !wSquadID || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID ||
				wSquadID != m_pMainChar->m_wCommanderID ||
				pTSQUAD->m_wPartyID == m_pMainChar->m_wCommanderID )
				bADD = FALSE;

			break;

		case GM_CORPS_LEAVE			:
			if( !wSquadID || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID ||
				wSquadID != m_pMainChar->m_wCommanderID )
			{
				bADD = FALSE;
			}
			else
				dwParam = pTSQUAD->m_wPartyID;

			break;

		case GM_CLOSE_POPUP			:
			if( m_pTPOPUP->m_vTMENU.empty() )
				bADD = FALSE;

			break;
		}

		if(bADD)
			m_pTPOPUP->AddMENU( dwMenuID[i][1], TCOMMAND( dwMenuID[i][0], dwParam ) );

		dwParam = 0;
	}

	if( !m_pTPOPUP->m_vTMENU.empty() )
	{
		ShowPOPUP();
		m_bRSCSMENU = TRUE;
	}
}
#ifdef MODIFY_CNP_NAME
void CTClientGame::DoEditPOPUP( TEdit* pEDIT, CPoint pt)
{
	static DWORD dwMenuID[][2] = {
		{ GM_PASTE_NAME, TSTR_MENU_PASTENAME },
		{ GM_CLOSE_POPUP, TSTR_CANCEL}};
		
	if( !pEDIT )
		return ;

	OnGM_CLOSE_POPUP();

	for( BYTE i=0; i<2; i++)
		m_pTPOPUP->AddMENU( dwMenuID[i][1], TCOMMAND( dwMenuID[i][0], (DWORD)(pEDIT) ) );

	ShowPOPUP();
}
#endif
void CTClientGame::DoPOPUP( CString strTarget )
{
	static DWORD dwMenuID[][2] = {
		{ GM_SET_WHISPER_TARGET, TSTR_MENU_WHISPER },
		{ GM_PARTY_REQ, TSTR_MENU_PARTY_REQ},
		{ GM_FRIEND_REQ, TSTR_MENU_FRIEND_REQ},
		{ GM_GUILD_REQ, TSTR_MENU_GUILD_REQ},
		{ GM_SHOW_INVITE_TACTICS, TSTR_MENU_TACTICS_REQ},
		{ GM_BLOCK_REQ, TSTR_MENU_BLOCK},
		{ GM_CLOSE_POPUP, TSTR_CANCEL}};
		
	OnGM_CLOSE_POPUP();

	if( (m_pMainChar->IsDead() || m_pMainChar->m_bGhost) || m_pMainChar->m_bPrivateShop )
		return;

	WORD wSquadID = GetPartyID(m_pMainChar);

	for( BYTE i=0; i<7; i++)
	{

		BOOL bADD = TRUE;

		switch(dwMenuID[i][0])
		{
		case GM_BLOCK_REQ :
			if( m_pBlockList->Find( strTarget ) != T_INVALID )
				bADD = FALSE;

			break;
		case GM_PARTY_REQ :
			if( (wSquadID && (
				m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID )) )
				bADD = FALSE;

			break;
		case GM_GUILD_REQ			:
			if( !m_pMainChar->m_dwGuildID || m_pMainChar->m_bGuildDuty == GUILD_DUTY_NONE )
				bADD = FALSE;

			break;
		case GM_SHOW_INVITE_TACTICS:
			if( !m_pMainChar->m_dwGuildID || m_pMainChar->m_bGuildDuty == GUILD_DUTY_NONE )
				bADD = FALSE;

			break;
		}

		if ( bADD )
			m_pTPOPUP->AddMENU( dwMenuID[i][1], dwMenuID[i][0] );
	}

	m_pChatFrame->SetCandiWhisperTarget( strTarget );
	ShowPOPUP();
}

void CTClientGame::ShowPOPUP()
{
	if( m_pTPOPUP->m_vTMENU.size() <= 1 )
		return;

	CRect rc;
	m_pTPOPUP->GetComponentRect(&rc);

	DWORD dwW = rc.Width();
	DWORD dwH = rc.Height();

	DWORD dwScrW = m_pMainWnd->m_Device.m_option.m_dwScreenX;
	DWORD dwScrH = m_pMainWnd->m_Device.m_option.m_dwScreenY;
	
	CPoint ptPos;
	if( m_vKEY.m_point.x + dwW > dwScrW )
		ptPos.x = m_vKEY.m_point.x - dwW;
	else
		ptPos.x = m_vKEY.m_point.x;

	if( m_vKEY.m_point.y + dwH > dwScrH )
		ptPos.y = m_vKEY.m_point.y - dwH;
	else
		ptPos.y = m_vKEY.m_point.y;

	m_pTPOPUP->MoveComponent(ptPos);
	m_pTPOPUP->ShowComponent(TRUE);
	OnMouseMove( 0, m_vKEY.m_point);

	TComponent* pTPARENT = m_pTPOPUP->GetParent();
	if( pTPARENT )
	{
		pTPARENT->RemoveKid(m_pTPOPUP);
		pTPARENT->AddKid(m_pTPOPUP);
	}

	m_bRSCSMENU = FALSE;
}

void CTClientGame::BuildRECALLMENU( CTClientRecall *pTRECALL)
{
	BYTE bGhost = BYTE(1 << (m_pMainChar->IsDead() || m_pMainChar->m_bGhost ? 1 : 0));

	if( pTRECALL->GetHostID() != m_pMainChar->m_dwID || !pTRECALL->m_pTEMP->m_pTPOPUP )
		return;

	LPTPOPUPMENU pTPOPUP = pTRECALL->m_pTEMP->m_pTPOPUP;
	for( BYTE i=0; i<BYTE(pTPOPUP->m_vTPOPUP.size()); ++i )
	{
		if(pTPOPUP->m_vTPOPUP[i]->m_bGhost & bGhost)
		{
			BYTE bADD = TRUE;

			switch(pTPOPUP->m_vTPOPUP[i]->m_dwMenuID)
			{
			case GM_MENU_RIDEPET :
				if( !m_pMainChar->IsDead() && 
					!m_vTFRAME[TFRAME_PRIVATE_SELL]->IsVisible() &&
					pTRECALL->m_bRecallType == TRECALLTYPE_PET )
				{
					if( CanRidePet(static_cast<CTClientPet*>(pTRECALL)) )
					{
						bADD = TRUE;
						break;
					}
				}
				
				bADD = FALSE;
				break;
			}

			if( bADD )
				m_pTPOPUP->AddMENU( pTPOPUP->m_vTPOPUP[i]->m_strTITLE, pTPOPUP->m_vTPOPUP[i]->m_dwMenuID);
		}
	}
}

void CTClientGame::BuildNPCMENU( CTClientNpc *pTNPC)
{
	BYTE bGhost = BYTE(1 << (m_pMainChar->IsDead() || m_pMainChar->m_bGhost ? 1 : 0));

	if(!pTNPC->m_pTPOPUP || pTNPC->m_bCamp != m_pMainChar->m_bCamp )
		return;

	BOOL bAlliance;
	
	if( pTNPC->m_bContryID == TCONTRY_N )
		bAlliance = TRUE;
	else
	{
		if( pTNPC->m_bNPCType != TNPC_PORTAL && m_pMainChar->IsDisguise() )
			bAlliance = !m_pMainChar->IsAlliance(pTNPC);
		else
			bAlliance = m_pMainChar->IsAlliance(pTNPC);
	}

	for( BYTE i=0; i<BYTE(pTNPC->m_pTPOPUP->m_vTPOPUP.size()); i++)
		if(pTNPC->m_pTPOPUP->m_vTPOPUP[i]->m_bGhost & bGhost)
		{
			BYTE bADD = TRUE;

			switch(pTNPC->m_pTPOPUP->m_vTPOPUP[i]->m_dwMenuID)
			{
			case GM_NPC_TALK				:
				if( !bAlliance )
					bADD = FALSE;

				break;

			case GM_NPC_TRADE				:
				if( !(pTNPC->m_bClassID & (1 << m_pMainChar->m_bClassID)) || !bAlliance )
					bADD = FALSE;

				if( bADD && pTNPC->m_bNPCType == TNPC_GUILD )
				{
					if( !m_pMainChar->m_dwGuildID || m_pMainChar->m_bGuildDuty == GUILD_DUTY_NONE )
						bADD = FALSE;
				}

				break;

			case GM_NPC_GUILD_NEW			:
				if( m_pMainChar->m_dwGuildID || !bAlliance )
					bADD = FALSE;
				
				break;

			case GM_NPC_GUILD_MARK			:
			case GM_NPC_GUILD_DEL			:
			case GM_NPC_GUILD_DEL_CANCEL	:
				if( !m_pMainChar->m_dwGuildID || !bAlliance || m_pMainChar->m_bGuildDuty != GUILD_DUTY_CHIEF )
					bADD = FALSE;

				break;

            case GM_NPC_GUILD_APP			:
			case GM_NPC_DURATIONREP_ALL		:
			case GM_NPC_DURATIONREP_EQUIP	:
			case GM_NPC_DURATIONREP_NOR		:
			case GM_NPC_REFINE				:
				if( !bAlliance )
					bADD = FALSE;

				break;

			}

			if(bADD)
				m_pTPOPUP->AddMENU( pTNPC->m_pTPOPUP->m_vTPOPUP[i]->m_strTITLE, pTNPC->m_pTPOPUP->m_vTPOPUP[i]->m_dwMenuID);
		}
}

void CTClientGame::BuildPCMENU( CTClientChar *pTCHAR)
{
	static DWORD dwMenuID[][2] = {
#ifdef MODIFY_CNP_NAME
		{ GM_COPY_NAME, TSTR_MENU_COPYNAME },
#endif
		{ GM_SET_WHISPER_TARGET, TSTR_MENU_WHISPER },
		{ GM_PARTYMEMBERRECALL, TSTR_MENU_PARTYMEMBERRECALL},
		{ GM_FOLLOW_TARGET, TSTR_FOLLOW_TARGET},
		{ GM_TRADE_REQ, TSTR_MENU_PC_TRADE},
		{ GM_PARTY_REQ, TSTR_MENU_PARTY_REQ},
		{ GM_MENU_DUEL, TSTR_MENU_DUEL},
		{ GM_PARTY_SETCHIEF, TSTR_MENU_SETCHIEF},
		{ GM_PARTY_KICK, TSTR_MENU_PARTY_KICK},
		{ GM_FRIEND_REQ, TSTR_MENU_FRIEND_REQ},
		{ GM_GUILD_REQ, TSTR_MENU_GUILD_REQ},
		{ GM_SHOW_INVITE_TACTICS, TSTR_MENU_TACTICS_REQ},
		{ GM_CORPS_REQ, TSTR_MENU_CORPS_REQ},
		{ GM_CORPS_SETCOMMANDER, TSTR_MENU_SETCOMMANDER},
		{ GM_CORPS_LEAVE, TSTR_MENU_CORPS_KICK},
		{ GM_OPEN_PRIVSHOP_FOR_BUY, TSTR_MENU_PRIVSHOP_FOR_BUY},
		{ GM_BLOCK_REQ, TSTR_MENU_BLOCK},
		{ GM_UNBLOCK_REQ, TSTR_MENU_UNBLOCK},
		{ GM_CLOSE_POPUP, TSTR_CANCEL}};

#ifdef MODIFY_CNP_NAME
	const DWORD dwCount = 19;
#else
	const DWORD dwCount = 18;
#endif

	WORD wSquadID = GetPartyID(m_pMainChar);
	WORD wPartyID = GetPartyID(pTCHAR);
	BOOL bAlliacne = m_pMainChar->IsAlliance(pTCHAR);
	DWORD dwParam = 0;
	for( BYTE i=0; i< dwCount; i++)
	{
		BYTE bADD = TRUE;

		switch(dwMenuID[i][0])
		{
		case GM_SET_WHISPER_TARGET:
			{
				if( bAlliacne )
				{
					m_pChatFrame->SetCandiWhisperTarget( pTCHAR->m_strNAME );
					bADD = TRUE;
				}
				else
					bADD = FALSE;
			}
			break;

		case GM_PARTYMEMBERRECALL	:
			{
				BYTE bInven, bSlot;
				if( m_bTournamentWatching || wPartyID != wSquadID || !wSquadID || !FindInven(IT_USE,IK_MEMBERRECALL, bInven, bSlot) )
					bADD = FALSE;
			}
			break;

		case GM_FOLLOW_TARGET		:
			if( !CanFollow(pTCHAR) )
				bADD = FALSE;

			break;

		case GM_TRADE_REQ			:
			if( m_bTournamentBattle || m_bTournamentWatching || !bAlliacne || pTCHAR->m_bPrivateShop )
				bADD = FALSE;

			break;

		case GM_MENU_DUEL			:
			if( m_bTournamentBattle || m_bTournamentWatching || !bAlliacne || !CanDuel(pTCHAR) )
				bADD = FALSE;

			break;

		case GM_CORPS_SETCOMMANDER	:
			if( m_bTournamentWatching || !wSquadID || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID ||
				!wPartyID || pTCHAR->m_dwChiefID != pTCHAR->m_dwID || pTCHAR->m_wCommanderID != wSquadID || (
				m_pTRSCS->m_vTCORPS.m_wID && wSquadID != m_pMainChar->m_wCommanderID) )
				bADD = FALSE;

			break;

		case GM_PARTY_SETCHIEF		:
			if( m_bTournamentWatching || wPartyID != wSquadID || !wSquadID || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID )
				bADD = FALSE;

			break;

		case GM_PARTY_REQ			:
			if( m_bTournamentWatching || !bAlliacne || (wSquadID && (
				m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID || wSquadID == wPartyID)) )
				bADD = FALSE;

			break;

		case GM_PARTY_KICK			:
			if( m_bTournamentWatching || wPartyID != wSquadID || !wSquadID || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID )
				bADD = FALSE;

			break;

		case GM_FRIEND_REQ			:
			{
				if( m_bTournamentBattle || m_bTournamentWatching || !bAlliacne )
					bADD = FALSE;
				else
				{
					CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
					if( pList->FindGroupByName(pTCHAR->m_strNAME) )
						bADD = FALSE;
				}
			}

			break;

		case GM_GUILD_REQ			:
			if( m_bTournamentBattle || m_bTournamentWatching || !bAlliacne || !m_pMainChar->m_dwGuildID || 
				pTCHAR->m_dwGuildID || m_pMainChar->m_bGuildDuty == GUILD_DUTY_NONE )
			{
				bADD = FALSE;
			}

			break;

		case GM_SHOW_INVITE_TACTICS:
			if( m_bTournamentBattle || m_bTournamentWatching || !bAlliacne || !m_pMainChar->m_dwGuildID || 
				pTCHAR->m_dwTacticsID || m_pMainChar->m_bGuildDuty == GUILD_DUTY_NONE )
			{
				bADD = FALSE;
			}
			
			break;

		case GM_CORPS_REQ			:
			if( m_bTournamentWatching || !bAlliacne || !wSquadID ||
				m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID || (
				m_pTRSCS->m_vTCORPS.m_wID && wSquadID != m_pMainChar->m_wCommanderID) )
				bADD = FALSE;

			break;

		case GM_CORPS_LEAVE			:
			if( m_bTournamentWatching || !wSquadID || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID ||
				!wPartyID || pTCHAR->m_dwChiefID != pTCHAR->m_dwID || pTCHAR->m_wCommanderID != wSquadID || (
				m_pTRSCS->m_vTCORPS.m_wID && wSquadID != m_pMainChar->m_wCommanderID) )
				bADD = FALSE;
			else
				dwParam = pTCHAR->m_wPartyID;

			break;

		case GM_OPEN_PRIVSHOP_FOR_BUY :
			if( m_bTournamentBattle || m_bTournamentWatching || !bAlliacne || !pTCHAR->m_bPrivateShop )
				bADD = FALSE;

			break;

		case GM_BLOCK_REQ :
			if ( m_bTournamentBattle || m_bTournamentWatching || !bAlliacne  )
				bADD = FALSE;
			else
			{
				if( m_pBlockList->Find( pTCHAR->GetName() ) != T_INVALID )
					bADD = FALSE;
			}

			break;

		case GM_UNBLOCK_REQ :
			if ( m_bTournamentBattle || m_bTournamentWatching || !bAlliacne )
				bADD = FALSE;
			else
			{
				if( m_pBlockList->Find( pTCHAR->GetName() ) == T_INVALID )
					bADD = FALSE;
			}

			break;
		}

		if(bADD)
			m_pTPOPUP->AddMENU( dwMenuID[i][1], TCOMMAND( dwMenuID[i][0], dwParam) );

		dwParam = 0;
	}
}

void CTClientGame::BuildMAINCHARMENU()
{
	static DWORD dwMenuID[][2] = {
		{ GM_OPEN_CHARINFO, TSTR_MENU_CHARINFO},
		{ GM_OPEN_SKILLINFO, TSTR_MENU_SKILLINFO},
		{ GM_PARTY_EXIT, TSTR_MENU_EXITPARTY},
		{ GM_CORPS_LEAVE, TSTR_MENU_EXITCORPS},
		{ GM_OPEN_GUILDINFO, TSTR_MENU_GUILDINFO},
		{ GM_OPEN_PRIVSHOP_FOR_SELL, TSTR_MENU_PRIVSHOP_FOR_SELL},
		{ GM_MENU_PETMANAGE, TSTR_MENU_PC_PETMANAGE},
		{ GM_MENU_RIDEPET, TSTR_MENU_PC_PETTAKEDOWN},
		{ GM_CLOSE_POPUP, TSTR_CANCEL}};

	DWORD dwParam = 0;
	for( BYTE i=0; i<TMAINCHARMENU_COUNT; i++)
	{
		BYTE bADD = TRUE;

		switch(dwMenuID[i][0])
		{
		case GM_PARTY_EXIT		:
			if(!GetPartyID(m_pMainChar))
				bADD = FALSE;

			break;

		case GM_CORPS_LEAVE		:
			if( !m_pTRSCS->m_vTCORPS.m_wID || !GetPartyID(m_pMainChar) || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID )
				bADD = FALSE;
			else
				dwParam = m_pMainChar->m_wPartyID;

			break;

		case GM_OPEN_GUILDINFO	:
			if(!m_pMainChar->m_dwGuildID)
				bADD = FALSE;

			break;

		case GM_OPEN_PRIVSHOP_FOR_SELL:
			if(m_bTournamentBattle || m_bTournamentWatching || 
				m_vTFRAME[TFRAME_PRIVATE_SELL]->IsVisible() || m_pMainChar->GetRidingPet())
				bADD = FALSE;

			break;
		case GM_MENU_PETMANAGE:
			if( m_vTFRAME[TFRAME_PRIVATE_SELL]->IsVisible() || 
				static_cast<CTPetManageDlg*>(m_vTFRAME[TFRAME_PET_MANAGE])->IsPetEmpty() )
			{
				bADD = FALSE;
			}

			break;
		case GM_MENU_RIDEPET:
			{
				CTClientPet* pPet = GetMainPet();
				if( !pPet || !pPet->GetTakeUpChar() )
					bADD = FALSE;
			}

			break;
		}

		if(bADD)
			m_pTPOPUP->AddMENU( dwMenuID[i][1], TCOMMAND( dwMenuID[i][0], dwParam ) );

		dwParam = 0;
	}
}

BOOL CTClientGame::CanRidePet(CTClientPet* pClientPet)
{
	if( !pClientPet || m_pMainChar->IsDead() || pClientPet != GetMainPet() )
		return FALSE;

	LPTPET pTPET = pClientPet->GetPetTemp();
	if( pTPET->m_bRecallKind1 != PETKIND_ONE && pTPET->m_bRecallKind2 != PETKIND_ONE )
		return FALSE;

	if( pClientPet->GetTakeUpChar() )
		return FALSE;

	if( m_pMainChar->CountCanNotUsePetMaintain() > 0 )
		return FALSE;

	return TRUE;
}

BOOL CTClientGame::CanDuel(CTClientChar* pTARGET)
{
	if( !pTARGET || 
		pTARGET->m_bPrivateShop || 
		m_pDuelTarget ||
		!m_pMainChar->m_pTREGION || 
		m_pMainChar->m_pTREGION->m_bInfo == TREGIONINFO_BATTLE ||
		!m_pMainChar->IsAlliance(pTARGET) )
	{
		return FALSE;
	}
	
	return TRUE;
}

void CTClientGame::SelectHotKey(THOTKEY_BASE eTYPE, BYTE bHotKey)
{
	CTGaugePannel* pTGaugePannel = static_cast<CTGaugePannel*>(m_vTFRAME[TFRAME_GAUGE]);
	pTGaugePannel->m_bHotkeyID[eTYPE] = bHotKey % MAX_HOTKEY_INVEN;
	pTGaugePannel->ResetHotkeyUI();
}

void CTClientGame::SelectParty( BYTE nIndex)
{
	if( nIndex >= (BYTE)m_vTPARTY.size() )
		return;

	CTClientObjBase::m_mapTSELECTOBJ.clear();
	CTClientObjBase::m_mapTSELECTDEADOBJ.clear();

	ResetPartyOBJ(m_vTPARTY[nIndex]);
	ResetTargetOBJ(FindPlayer(m_pTPARTY->m_dwCharID));
}

void CTClientGame::EnableChat( BYTE bEnable)
{
	m_pChatFrame->ResetChatMsg(bEnable);

	if(bEnable)
	{
		m_pChatFrame->ShowComponent(TRUE);
		m_pChatFrame->ResetTick();

		SwitchFocus(m_vTFRAME[TFRAME_MAIN]);
	}
	else
		SwitchFocus(m_pTCURFRAME);

	m_pChatFrame->ClearMsgInEdit();
}

void CTClientGame::DoCHAT()
{
	m_pChatFrame->TrimMsgInEdit();

	if( !m_pChatFrame->IsChatEditON() )
		EnableChat(TRUE);
	else
	{
		BOOL bDisableChat = !m_vTOPTION.m_bContinualChat;
		if( !m_pChatFrame->GetChatEditCtrl()->m_strText.IsEmpty() )
		{
			CString strMSG = m_pChatFrame->ProcChatMsgInEdit();

			if( CTNationOption::GERMANY || CTNationOption::FRANCE || CTNationOption::POLAND || CTNationOption::CZECH || 
				CTNationOption::ITALY || CTNationOption::SPAIN || CTNationOption::UNITEDKINGDOM ||
				CTNationOption::GREECE || CTNationOption::ROMANIA || CTNationOption::EGYPT || //CTNationOption::RUSSIA || 
				CTNationOption::UNITEDSTATE || CTNationOption::HUNGARY || CTNationOption::TURKEY ||
				CTNationOption::PORTUGAL )
			{
				if( (m_pChatFrame->GetChatMode() == CHAT_MAP) && ( (m_pMainChar->m_bLevel < TCHAT_WROLD_MIN_LEVEL) || ( m_vMAP.m_wMapID == TUTORIAL_MAPID ) ) )
				{
					m_pChatFrame->ChatSysMSG( CTChart::LoadString( TSTR_INFO_TITLE ), m_pMainChar->GetName(), CTChart::LoadString(TSTR_CHAT_WORLD_ERROR_MIN_LEVEL), TCOLOR_ERROR, TCHAT_FLAG_INFO, ID_SND_ERROR, MEDIA_TSOUND);
				}
				else if( m_pSESSION && !strMSG.IsEmpty() )
				{
					RelaySession()->SendCS_CHAT_REQ(
						m_pMainChar->CountMaintainFunc(SDT_STATUS, SDT_STATUS_MASK, 1) ? NAME_MASK : m_pMainChar->m_strNAME,
						m_pChatFrame->GetChatMode(),
						m_pChatFrame->GetChatTargetID(),
						m_pChatFrame->GetChatTarget(),
						strMSG,
						m_pMainChar->m_bContryID,
						m_mapPLAYER);
				}
			}
			else if( m_pSESSION && !strMSG.IsEmpty() )
			{
				RelaySession()->SendCS_CHAT_REQ(
					m_pMainChar->CountMaintainFunc(SDT_STATUS, SDT_STATUS_MASK, 1) ? NAME_MASK : m_pMainChar->m_strNAME,
					m_pChatFrame->GetChatMode(),
					m_pChatFrame->GetChatTargetID(),
					m_pChatFrame->GetChatTarget(),
					strMSG,
					m_pMainChar->m_bContryID,
					m_mapPLAYER);
			}

			m_pChatFrame->ResetChatMsg(TRUE);
		}
		else
			bDisableChat = TRUE;

		if( bDisableChat )
			EnableChat(FALSE);
	}
}

BYTE CTClientGame::GetInvenID( BYTE bFrameID)
{
	BYTE bResult = INVEN_DEFAULT;

	switch(bFrameID)
	{
	case TFRAME_DEFINVEN	: bResult = INVEN_DEFAULT; break;
	case TFRAME_INVEN_1		: bResult = 0x00; break;
	case TFRAME_INVEN_2		: bResult = 0x01; break;
	case TFRAME_INVEN_3		: bResult = 0x02; break;
	case TFRAME_INVEN_4		: bResult = 0x03; break;
	case TFRAME_INVEN_5		: bResult = 0x04; break;
	case TFRAME_CHAR_INFO	: bResult = INVEN_EQUIP; break;
	}

	return bResult;
}

BOOL CTClientGame::FindEmptyInven(BYTE& outInven, BYTE& outSlot)
{
	static const BYTE bFrameID[] = {
		TFRAME_DEFINVEN,
		TFRAME_INVEN_1,
		TFRAME_INVEN_2,
		TFRAME_INVEN_3,
		TFRAME_INVEN_4,
		TFRAME_INVEN_5 };

	for(INT i=0; i<MAX_SUBINVEN+1; ++i)
	{
		BYTE bInvenID = GetInvenID( bFrameID[i] );
		CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);
		if( pTInven )
		{
			BYTE bSlotID = pTInven->GetBlankPos();
			if( bSlotID != INVALID_SLOT )
			{
				outInven = bInvenID;
				outSlot = bSlotID;

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CTClientGame::FindInven(TITEM_TYPE eType, TITEM_KIND eKind, BYTE& outInven, BYTE& outSlot)
{
	static const BYTE bFrameID[] = {
		TFRAME_DEFINVEN,
		TFRAME_INVEN_1,
		TFRAME_INVEN_2,
		TFRAME_INVEN_3,
		TFRAME_INVEN_4,
		TFRAME_INVEN_5 };

	for(INT i=0; i<MAX_SUBINVEN+1; ++i)
	{
		BYTE bInvenID = GetInvenID( bFrameID[i] );
		CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);
		if( pTInven )
		{
			BYTE bSlotID = pTInven->GetItemPos(eType,eKind);
			if( bSlotID != INVALID_SLOT )
			{
				outInven = bInvenID;
				outSlot = bSlotID;

				return TRUE;
			}
		}
	}

	return FALSE;
}

BOOL CTClientGame::CheckInvenItem(BYTE bFrameID, BYTE bSlotID)
{
	switch( bFrameID )
	{
	case TFRAME_DEFINVEN	: 
	case TFRAME_INVEN_1		: 
	case TFRAME_INVEN_2		: 
	case TFRAME_INVEN_3		: 
	case TFRAME_INVEN_4		: 
	case TFRAME_INVEN_5		: 
		{
			CTClientItem* pTItem = FindMainItemByFrame(bFrameID, bSlotID);
			if( pTItem && pTItem->GetTITEM()->m_bType == IT_INVEN )
				return TRUE;
		}
		break;

	case TFRAME_MAIN		:
		{
			if( CTMainUI::ToSlotType(bSlotID) == TMAINSLOTID_INVEN )
				return TRUE;
		}
		break;
	}

	return FALSE;
}

BOOL CTClientGame::CheckInvenItem(WORD wItemID, BYTE* pbInvenID, BYTE* pbItemID)
{
	static const BYTE bFrameID[] = {
		TFRAME_DEFINVEN,
		TFRAME_INVEN_1,
		TFRAME_INVEN_2,
		TFRAME_INVEN_3,
		TFRAME_INVEN_4,
		TFRAME_INVEN_5 };

	for(INT i=0; i<MAX_SUBINVEN+1; ++i)
	{
		BYTE bInvenID = GetInvenID( bFrameID[i] );
		CTClientInven* pTInven = m_pMainChar->FindTInven(bInvenID);
		if( pTInven )
		{
			if( pTInven->CheckTItem(wItemID, pbItemID) )
			{
				if( pbInvenID )
					*pbInvenID = bInvenID;
				return TRUE;
			}
		}
	}

	return FALSE;
}

CTClientItem* CTClientGame::FindMainItemByFrame(BYTE bFrameID, BYTE bSlotID)
{
	static const BYTE bFRAMEID[] = {
		TFRAME_DEFINVEN,
		TFRAME_INVEN_1,
		TFRAME_INVEN_2,
		TFRAME_INVEN_3,
		TFRAME_INVEN_4,
		TFRAME_INVEN_5 };

	BYTE bInvenID = GetInvenID( bFRAMEID[bFrameID] );
	return FindMainItemByInven(bInvenID, bSlotID);
}

CTClientItem* CTClientGame::FindMainItemByInven(BYTE bInvenID, BYTE bSlotID)
{
	MAPTINVEN::iterator itTINVEN = m_pMainChar->m_mapTINVEN.find(bInvenID);
	if( itTINVEN != m_pMainChar->m_mapTINVEN.end() )
	{
		MAPTITEM::iterator itTITEM = (*itTINVEN).second->m_mapTITEM.find(bSlotID);
		if( itTITEM != (*itTINVEN).second->m_mapTITEM.end() )
			return itTITEM->second;
	}

	return NULL;
}

BYTE CTClientGame::GetFACE( BYTE bRaceID,
						    BYTE bSexID,
							BYTE bFaceID,
							BYTE bHairID)
{
	return THAIR_COUNT * ((bRaceID * TSEX_COUNT + bSexID) * TFACE_COUNT + bFaceID) + bHairID;
}

void CTClientGame::CheckQuestOBJ()
{
	CPacket vMSG;

	vMSG.SetID(CS_QUESTLIST_POSSIBLE_REQ)
		<< BYTE(m_vMAP.m_vNEWQUESTOBJ.size());

	while(!m_vMAP.m_vNEWQUESTOBJ.empty())
	{
		vMSG << WORD(m_vMAP.m_vNEWQUESTOBJ.back());
		m_vMAP.m_vNEWQUESTOBJ.pop_back();
	}

	m_pSESSION->Say(&vMSG);
}

void CTClientGame::UseTItem( WORD wTemplateID)
{
	MAPTINVEN::iterator itTINVEN;

	for( itTINVEN = m_pMainChar->m_mapTINVEN.begin(); itTINVEN != m_pMainChar->m_mapTINVEN.end(); itTINVEN++)
	{
		MAPTITEM::iterator itTITEM;

		for( itTITEM = (*itTINVEN).second->m_mapTITEM.begin(); itTITEM != (*itTINVEN).second->m_mapTITEM.end(); itTITEM++)
			if( (*itTITEM).second->GetTITEM()->m_wItemID == wTemplateID )
			{
				UseTItem( (*itTINVEN).first, (*itTITEM).first);
				return;
			}
	}
}

void CTClientGame::UseTItem( BYTE bInvenID,
							 BYTE bSlotID)
{
	if( !m_bCanUseItem )
		return;

	CTClientInven *pTINVEN = m_pMainChar->FindTInven(bInvenID);
	if(!pTINVEN)
		return;
	
	CTClientItem *pTITEM = pTINVEN->FindTItem(bSlotID);

	if( !pTITEM )
		return;

	if( pTITEM->GetTITEM()->m_bType != IT_USE )
		return;

	if( m_bTournamentWatching )
	{
		if( pTITEM->GetTITEM()->m_bKind != IK_CHEER )
		{
			CString strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
			CString strMSG = CTChart::LoadString( TSTR_TOURNAMENT_CANT_USE );
			m_pChatFrame->ChatSysMSG(
				strTYPE,
				m_pMainChar->GetName(),
				strMSG, 
				TCOLOR_ERROR,
				TCHAT_FLAG_INFO,
				ID_SND_ERROR,
				MEDIA_TSOUND);
			return ;
		}
	}

	if( m_bTournamentBattle )
	{
		if( pTITEM->GetTITEM()->m_bKind == TOURNAMENT_CANTUSEITEM_HP ||
			pTITEM->GetTITEM()->m_bKind == TOURNAMENT_CANTUSEITEM_MP )
		{
			CString strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
			CString strMSG = CTChart::LoadString(TSTR_TOURNAMENT_CANT_USE_CASHITEM);
			m_pChatFrame->ChatSysMSG(
				strTYPE,
				m_pMainChar->GetName(),
				strMSG, 
				TCOLOR_ERROR,
				TCHAT_FLAG_INFO,
				ID_SND_ERROR,
				MEDIA_TSOUND);

			return ;
		}
	}

	if( pTITEM->GetTITEM()->m_bKind != IK_REVIVAL )
	{
		if( m_pMainChar->IsBlock() || m_pMainChar->IsDown() )
		{
			ItemMSG(MI_BLOCK);
			return;
		}

		if( m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
		{
			ItemMSG(MI_DEAD);
			return;
		}

		if( m_pMainChar->GetRidingPet() )
		{
			m_pChatFrame->ChatSysMSG(
				CTChart::LoadString( TSTR_INFO_TITLE),
				m_pMainChar->GetName(),
				CTChart::LoadString(TSTR_DO_NOT_USE_ITEM_ON_RIDING),
				TCOLOR_ERROR,
				TCHAT_FLAG_INFO,
				ID_SND_ERROR,
				MEDIA_TSOUND);

			return;
		}
	}

	switch( pTITEM->GetTITEM()->m_bKind )
	{
	case IK_SMSGUILD		: 
		if( m_pMainChar->m_strGUILD.IsEmpty() )
		{
			m_pMainWnd->MessageBoxOK(
				TSTR_ERR_NO_GUILD,
				TSTR_OK,
				GM_CLOSE_MSGBOX,
				0 );

			return;
		}

	case IK_SMSPERSON		: 
		{
			if( m_vTFRAME[TFRAME_SMS]->IsVisible() )
				return;

			CTSMSDlg* pDLG = static_cast<CTSMSDlg*>(m_vTFRAME[TFRAME_SMS]);

            CTSMSDlg::TYPE type;
			if( pTITEM->GetTITEM()->m_bKind == IK_SMSGUILD )
				type = CTSMSDlg::TYPE_GUILD;
			else
				type = CTSMSDlg::TYPE_PERSON;

			pDLG->Set(type, bInvenID, bSlotID);
			EnableUI(TFRAME_SMS);
		}
		break;

	case IK_SKILLONEINIT	:
	case IK_SKILLALLINIT	:
		{
			if( pTITEM->GetTITEM()->m_bKind == IK_SKILLONEINIT )
			{
				if( m_pSESSION )
				{
					m_bSkillInitInven	= bInvenID;
					m_bSkillInitSlot	= bSlotID;

					m_pSESSION->SendCS_NPCITEMLIST_REQ(TDEF_SKILL_NPC);
					m_pSESSION->SendCS_SKILLINITPOSSIBLE_REQ(bInvenID,bSlotID);
				}
			}
			else
			{
				m_bSkillInitInven	= bInvenID;
				m_bSkillInitSlot	= bSlotID;

				m_pMainWnd->MessageBoxYesNo(
					TSTR_WARN_SKILLINIT_ALL,
					TSTR_YES,
					TSTR_NO,
					GM_SKILLINIT_ALL,
					GM_CLOSE_MSGBOX );
			}
		}
		break;

	case IK_MEMBERRECALL	:
		{
			BOOL bSUCC = FALSE;
			if( m_pTPARTY && GetPartyID(m_pMainChar) )
				bSUCC = TRUE;

			if( bSUCC )
			{
				m_bPortalItemInven	= bInvenID;
				m_bPortalItemSlot	= bSlotID;

				CString strMSG;
				strMSG = CTChart::Format( TSTR_USEITEM_MEMBERRECALL, m_pTPARTY->m_strNAME);

				m_pMainWnd->MessageBoxYesNo(
					strMSG,
					TSTR_YES,
					TSTR_NO,
					GM_USEITEM_PARTYMEMBERRECALL,
					GM_CLOSE_MSGBOX );
			}
			else
			{
				CString strMSG;
				strMSG = CTChart::LoadString( TSTR_MEMBERRECALL_INVALIDTARGET);
				ShowInfoMsgBox(strMSG, TRUE, TUISND_TYPE_ERROR);
			}
		}
		break;

	case IK_MOVETOCHAR		:
		{
			m_bPortalItemInven	= bInvenID;
			m_bPortalItemSlot	= bSlotID;

			m_pMainWnd->MessageBox(
				TSTR_USEITEM_MOVETOCHAR,
				TSTR_CHAR_NAME,
				0,
				TSTR_YES,
				TSTR_NO,
				ID_FRAME_2BTNMSGBOX,
				GM_USEITEM_MOVETOCHAR,
				GM_CLOSE_MSGBOX,
				0,
				TRUE,
				TRUE);
		}
		break;

	case IK_NAME:
		{
			DWORD dwPARAM = MAKELONG(
				(WORD) bInvenID,
				(WORD) bSlotID);

			if( CTNationOption::RUSSIA ) 
			{
				TFrame *pFRAME = m_pMainWnd->m_MessageBox.FindFrame(ID_FRAME_2BTNMSGBOX);
				TComponent *pEDIT = pFRAME->FindKid(ID_CTRLINST_EDIT);
				pEDIT->m_style |= TES_NOCONMODE;
			}

			m_pMainWnd->MessageBox(
				TSTR_CHANGE_NAME,
				TSTR_CHAR_NAME,
				0,
				TSTR_YES,
				TSTR_NO,
				ID_FRAME_2BTNMSGBOX,
				TCOMMAND( GM_CHANGE_NAME, dwPARAM),
				GM_CLOSE_MSGBOX_WITH_RESETCONMODE,
				GM_CLOSE_MSGBOX_WITH_RESETCONMODE,
				TRUE,
				TRUE);
		}
		break;

	case IK_MAIL:
		{
			CTMailSendDlg* pSndDlg = static_cast<CTMailSendDlg*>(m_vTFRAME[TFRAME_MAIL_SEND]);
			pSndDlg->SetUsingByCash( bInvenID, bSlotID);
			EnableUI(TFRAME_MAIL_SEND);
		}
		break;

	case IK_NPCCALL:
		{
			LPTMAPINFO pMapInfo = CTChart::GetTMAPINFO( m_vMAP.m_wMapID );
			if( !pMapInfo || !pMapInfo->m_bNPCCALL )
			{
				m_pMainWnd->MessageBoxOK(
					TSTR_CANTUSEINPLACE,
					TSTR_OK,
					GM_CLOSE_MSGBOX,
					0 );

				return ;
			}

			WORD wNPCID = pTITEM->GetTITEM()->m_wUseValue;
			LPTNPC pTNPC = CTChart::FindTNPCGlobal( wNPCID );
			if( pTNPC )
			{			
				ResetTargetOBJ(NULL);
				ResetTargetINFO(FALSE);

				switch( pTNPC->m_bNPCType )
				{
					case TNPC_TOURNAMENT_GATE_KEEPER:
					case TNPC_REVIVAL:
					case TNPC_MAKE:
					case TNPC_MAPPORTAL:
					case TNPC_BOX:
					case TNPC_AUCTION:
					case TNPC_GUILD:
					case TNPC_CASTLE:
					case TNPC_RETURN:
						return ;

					case TNPC_SKILL_MASTER:
						{
							if( m_pSESSION )
							{
								if( m_pOpenShopByCash )
									delete m_pOpenShopByCash;
								
								m_pOpenShopByCash = new TOPENBYCASH( wNPCID, bInvenID, bSlotID );
								m_pSESSION->SendCS_NPCITEMLIST_REQ( wNPCID );
							}
						}
						break;

					case TNPC_ITEM:
						{
							if( m_pSESSION )
							{
								if( m_pOpenShopByCash )
									delete m_pOpenShopByCash;
								
								m_pOpenShopByCash = new TOPENBYCASH( wNPCID, bInvenID, bSlotID );
								m_pSESSION->SendCS_NPCITEMLIST_REQ( wNPCID );
							}
						}
						break;

					case TNPC_PORTAL:
						{
							if( m_pSESSION )
							{
								if( m_pOpenShopByCash )
									delete m_pOpenShopByCash;
								
								m_pOpenShopByCash = new TOPENBYCASH( wNPCID, bInvenID, bSlotID );
								m_pSESSION->SendCS_NPCITEMLIST_REQ( wNPCID );
							}
						}
						break;

					case TNPC_UPGRADE:
						{
							CTItemUpDlg* pITEMUPDLG = (CTItemUpDlg*) GetFrame( TFRAME_ITEM_UP );
							pITEMUPDLG->ClearOpenByCash();
							pITEMUPDLG->SetOpenByCash( new TOPENBYCASH( wNPCID, bInvenID, bSlotID ) );
							EnableUI( TFRAME_ITEM_UP );
						}
						break;

					case TNPC_WAREHOUSE:
						{
							CTNormalCabinetDlg* pNORMALCABINETDLG = (CTNormalCabinetDlg*) GetFrame( TFRAME_CABINET );

							pNORMALCABINETDLG->ClearOpenByCash();
							pNORMALCABINETDLG->SetOpenByCash( new TOPENBYCASH( wNPCID, bInvenID, bSlotID ) );

							pNORMALCABINETDLG->RequestShowComponent();
						}
						break;

					case TNPC_DIVINER: //점술가(소울메이트)
						{
							WORD wItemID = MAKEWORD( bSlotID, bInvenID );
							DWORD dwPARAM = MAKELONG( wItemID, wNPCID );

							m_pMainWnd->MessageBoxYesNo(
								TSTR_SOULMATEBOX_SELREG,
								TSTR_YES,
								TSTR_NO,
								TCOMMAND( GM_SOULMATE_REGBOX, dwPARAM ),
								TCOMMAND( GM_SOULMATE_SEARCH, dwPARAM ) );

							EnableChat(FALSE);
						}
						break;

					case TNPC_GAMBLE: //겜블
						{
							CTItemGambleDlg* pITEMGAMBLEDLG = (CTItemGambleDlg*) GetFrame( TFRAME_ITEM_GAMBLE );
							pITEMGAMBLEDLG->ClearOpenByCash();
							pITEMGAMBLEDLG->SetOpenByCash( new TOPENBYCASH( wNPCID, bInvenID, bSlotID ) );
							EnableUI( TFRAME_ITEM_GAMBLE );
						}
						break;
					
					case TNPC_REPAIR:	//수리
						{
							CTItemRepairDlg* pITEMREPAIRDLG = (CTItemRepairDlg*) GetFrame( TFRAME_ITEM_REPAIR );
							pITEMREPAIRDLG->ClearOpenByCash();
							pITEMREPAIRDLG->SetOpenByCash( new TOPENBYCASH( wNPCID, bInvenID, bSlotID ) );
							EnableUI(TFRAME_ITEM_REPAIR);
						}
						break;

					case TNPC_REFINE:	//제련TSTR_ERROR_WRONGREGION_ITEM
						{
							CTItemRefineDlg* pITEMREFINDDLG = (CTItemRefineDlg*) GetFrame( TFRAME_ITEM_REFINE );
							pITEMREFINDDLG->ClearOpenByCash();
							pITEMREFINDDLG->SetOpenByCash( new TOPENBYCASH( wNPCID, bInvenID, bSlotID ) );
							EnableUI(TFRAME_ITEM_REFINE);
						}
						break;
				}
			}
		}
		break;

	case IK_CHANGE:
		{
			if( m_pSESSION )
				m_pSESSION->SendCS_ITEMCHANGE_REQ( bInvenID, bSlotID );					
		}

		break;

	case IK_CHEER:
		{
			if( m_bTournamentWatching )
			{
				CTTournamentSelectTarget* pDLG = static_cast<CTTournamentSelectTarget*>( m_vTFRAME[ TFRAME_TOURNAMENT_SELECT_TARGET ] );
				pDLG->SetCheerUpMode();
				pDLG->m_bCheerItemInven = bInvenID;
				pDLG->m_bCheerItemSlot = bSlotID;
				EnableUI( TFRAME_TOURNAMENT_SELECT_TARGET );
			}
		}
		return ;

	case IK_SKILL:
		{
			if( m_pMainChar->m_pTREGION &&
				!m_pMainChar->m_pTREGION->m_bCanFly)
			{
				LPTITEM pTItem = pTITEM->GetTITEM();
				WORD wSkillID = pTItem->m_wUseValue;
				LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP(wSkillID);
				LPTSKILLFUNCTION pTFUNC = pTSKILL->FindFunction(SDT_TRANS, SDT_TRANS_CROW);
				if( pTFUNC && pTFUNC->m_wValue )
				{
					LPTMONTEMP pTMON = CTChart::FindTMONTEMP(pTFUNC->m_wValue);
					if( pTMON->m_bCanFly )
					{
						m_pChatFrame->ChatSysMSG(
							CTChart::LoadString( TSTR_INFO_TITLE),
							m_pMainChar->GetName(), 
							CTChart::LoadString(TSTR_ERROR_WRONGREGION_ITEM),
							TCOLOR_ERROR,
							TCHAT_FLAG_INFO);
						return ;
					}
				}
			}
		}
	default:
		{
			if(CTClientItem::GetTick(pTITEM->GetTITEM()->m_wDelayGroupID))
				return;

			if(CTClientItem::IsLocked(pTITEM->GetTITEM()->m_wDelayGroupID))
				return;

			CTClientItem::Lock(pTITEM->GetTITEM()->m_wDelayGroupID);
			SendCS_ITEMUSE_REQ(
				&D3DXVECTOR2(
				m_pMainChar->GetPositionX(),
				m_pMainChar->GetPositionZ()),
				pTITEM->GetTITEM()->m_wItemID,
				bInvenID,
				bSlotID,
				pTITEM->GetTITEM()->m_wDelayGroupID);
		}

		break;
	}
}

void CTClientGame::UseTPet( WORD wPetID )
{
	BYTE bRaceID = m_pMainChar->m_bRaceID_;
	wPetID = MAKEWORD( bRaceID, wPetID );

	if( m_pMainChar->m_bPrivateShop ||
		m_pMainChar->IsDead() ||
		m_pMainChar->m_bGhost )
		return ;

	if( !m_pSESSION )
		return ;

	CTClientPet* pMainPet = GetMainPet();
	if( pMainPet )
	{
		if( m_pSESSION && pMainPet->m_dwID )
		{
			if( pMainPet->IsJump() || pMainPet->IsDown() || pMainPet->IsFall() )
				return ;

			SendCS_MOVE_REQ(
				m_vMAP.m_wMapID,
				m_pMainChar->GetPositionX(),
				m_pMainChar->GetPositionY(),
				m_pMainChar->GetPositionZ(),
				m_pMainChar->m_wPITCH,
				m_pMainChar->m_wDIR,
				m_pMainChar->m_bMouseDIR,
				m_pMainChar->m_bKeyDIR,
				m_pMainChar->m_bAction,
				m_pMainChar->m_bGhost);

			if( wPetID == pMainPet->GetPetID() )
			{
				if( pMainPet->GetTakeUpChar() == m_pMainChar )
				{
					m_pSESSION->SendCS_PETRIDING_REQ(pMainPet->m_dwID, FALSE);
					m_bPetUseByHotbar = TRUE;
				}
				else
					m_pSESSION->SendCS_PETRIDING_REQ(pMainPet->m_dwID, TRUE);

				m_nNextPetID = 0;
			}
			else
			{
				m_nNextPetID = wPetID;
				m_bPetUseByHotbar = TRUE;

				if( pMainPet->GetTakeUpChar() == m_pMainChar )
					m_pSESSION->SendCS_PETRIDING_REQ(pMainPet->m_dwID, FALSE);
				else
					m_pSESSION->SendCS_DELRECALLMON_REQ(pMainPet->m_dwID,pMainPet->m_bType);
			}
		}	
		// OnCS_PETRIDING_ACK
	}
	else
	{
		CTPetManageDlg* pDlg = static_cast<CTPetManageDlg*>(m_vTFRAME[TFRAME_PET_MANAGE]);

		if( m_pMainChar->IsJump() || m_pMainChar->IsDown() || m_pMainChar->IsFall() )
			return ;

		SendCS_MOVE_REQ(
			m_vMAP.m_wMapID,
			m_pMainChar->GetPositionX(),
			m_pMainChar->GetPositionY(),
			m_pMainChar->GetPositionZ(),
			m_pMainChar->m_wPITCH,
			m_pMainChar->m_wDIR,
			m_pMainChar->m_bMouseDIR,
			m_pMainChar->m_bKeyDIR,
			m_pMainChar->m_bAction,
			m_pMainChar->m_bGhost);

		if( m_pMainChar->m_bMode != MT_NORMAL )
			m_pSESSION->SendCS_CHGMODE_REQ(MT_NORMAL);

		if( !pDlg->IsRecalling() )
		{
			m_pSESSION->SendCS_PETRECALL_REQ(wPetID);
			pDlg->SetRecalling(TRUE);

			m_bPetUseByHotbar = TRUE;
		}
	}
}

void CTClientGame::LoopTSKill( CTClientSkill *pTSKILL)
{
	if( pTSKILL->m_bLoopON &&
		!m_bCHARGE &&
		!m_bLoopLock &&
		(m_dwMoveGM != GM_ATTACK || (m_bMoveType != TMAINMOVE_GROUND && m_bMoveType != TMAINMOVE_TARGET)) &&
		m_pMainChar->GetRidingPet() == NULL )
	{
		UseTSkill( pTSKILL->m_pTSKILL->m_wSkillID, FALSE, TRUE);
	}
}

BOOL CTClientGame::FindSkillTarget( CTClientObjBase *pATTACK,
								    CTClientObjBase *pDEFEND,
									LPD3DXVECTOR3 pGROUND,
									LPTSKILL pTSKILL,
									LPTSKILLTARGET_PARAM pRESULT)
{
	pRESULT->m_vTBOUND.clear();
	pRESULT->m_vTARGET.clear();

	pRESULT->m_bRANGE = pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE || pTSKILL->m_bRangeType == TSKILLRANGE_MCIRCLE ? TRUE : FALSE;
	pRESULT->m_bPOINT = pTSKILL->m_bRangeType == TSKILLRANGE_POINT ? TRUE : FALSE;

	if( pTSKILL->m_bRangeType == TSKILLRANGE_MCIRCLE && pGROUND )
	{
		(*pGROUND) = D3DXVECTOR3(
			pATTACK->GetPositionX(),
			pATTACK->GetPositionY(),
			pATTACK->GetPositionZ());
	}

	if((!pRESULT->m_bRANGE && !pDEFEND) || ((pRESULT->m_bRANGE || pRESULT->m_bPOINT) && !pGROUND))
	{
		if(IsHostingOBJ( pATTACK->m_dwID, pATTACK->m_bType))
		{
			WORD wSkillID = pATTACK->m_bType == OT_RECALL ||
				pATTACK->m_bType == OT_SELF ||
				!pTSKILL->m_dwSpellTick ? pTSKILL->m_wSkillID : 0;

			SendCS_ACTION_REQ(
				pATTACK->m_dwID,
				pATTACK->m_bType,
				0, 0, 0,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID,
				wSkillID);
		}

		if( pATTACK == m_pMainChar )
		{
			SkillMSG( m_pMainChar, SKILL_NOTARGET);
			if(m_bCHARGE)
				CancelCharge(TRUE);
		}

		return FALSE;
	}

	if(!pATTACK->CheckTransMode(pTSKILL))
	{
		if(IsHostingOBJ( pATTACK->m_dwID, pATTACK->m_bType))
		{
			WORD wSkillID = pATTACK->m_bType == OT_RECALL ||
				pATTACK->m_bType == OT_SELF ||
				!pTSKILL->m_dwSpellTick ? pTSKILL->m_wSkillID : 0;

			SendCS_ACTION_REQ(
				pATTACK->m_dwID,
				pATTACK->m_bType,
				0, 0, 0,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID,
				wSkillID);
		}

		if( pATTACK == m_pMainChar )
		{
			SkillMSG( m_pMainChar, SKILL_TRANS);
			if(m_bCHARGE)
				CancelCharge(TRUE);
		}

		return FALSE;
	}

	D3DXVECTOR2 vTPOS = pRESULT->m_bRANGE || pRESULT->m_bPOINT ? D3DXVECTOR2(
		pGROUND->x,
		pGROUND->z) : D3DXVECTOR2(
		pDEFEND->GetPositionX(),
		pDEFEND->GetPositionZ());
	
	MAPMONSTER::iterator itMON;
	MAPPLAYER::iterator itPC;
	MAPRECALL::iterator itRECALL;

	if( pDEFEND )
	{
		BYTE bKIND = GetTargetKind(
			pATTACK,
			pDEFEND,
			pDEFEND,
			pTSKILL,
			&vTPOS);

		switch(bKIND)
		{
		case TARGETKIND_TARGET	: pRESULT->m_vTARGET.push_back(pDEFEND); break;
		case TARGETKIND_BOUND	: pRESULT->m_vTBOUND.push_back(pDEFEND); break;
		}
	}

	for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
		if( pDEFEND != (*itRECALL).second )
		{
			BYTE bKIND = GetTargetKind(
				pATTACK,
				pDEFEND,
				(*itRECALL).second,
				pTSKILL,
				&vTPOS);

			switch(bKIND)
			{
			case TARGETKIND_TARGET	: pRESULT->m_vTARGET.push_back((*itRECALL).second); break;
			case TARGETKIND_BOUND	: pRESULT->m_vTBOUND.push_back((*itRECALL).second); break;
			}
		}

	for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
		if( pDEFEND != (*itRECALL).second )
		{
			BYTE bKIND = GetTargetKind(
				pATTACK,
				pDEFEND,
				(*itRECALL).second,
				pTSKILL,
				&vTPOS);

			switch(bKIND)
			{
			case TARGETKIND_TARGET	: pRESULT->m_vTARGET.push_back((*itRECALL).second); break;
			case TARGETKIND_BOUND	: pRESULT->m_vTBOUND.push_back((*itRECALL).second); break;
			}
		}

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
		if( pDEFEND != (*itMON).second )
		{
			BYTE bKIND = GetTargetKind(
				pATTACK,
				pDEFEND,
				(*itMON).second,
				pTSKILL,
				&vTPOS);

			switch(bKIND)
			{
			case TARGETKIND_TARGET	: pRESULT->m_vTARGET.push_back((*itMON).second); break;
			case TARGETKIND_BOUND	: pRESULT->m_vTBOUND.push_back((*itMON).second); break;
			}
		}

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
		if( pDEFEND != (*itPC).second )
		{
			BYTE bKIND = GetTargetKind(
				pATTACK,
				pDEFEND,
				(*itPC).second,
				pTSKILL,
				&vTPOS);

			switch(bKIND)
			{
			case TARGETKIND_TARGET	: pRESULT->m_vTARGET.push_back((*itPC).second); break;
			case TARGETKIND_BOUND	: pRESULT->m_vTBOUND.push_back((*itPC).second); break;
			}
		}

	if( pDEFEND != m_pMainChar )
	{
		BYTE bKIND = GetTargetKind(
			pATTACK,
			pDEFEND,
			m_pMainChar,
			pTSKILL,
			&vTPOS);

		switch(bKIND)
		{
		case TARGETKIND_TARGET	: pRESULT->m_vTARGET.push_back(m_pMainChar); break;
		case TARGETKIND_BOUND	: pRESULT->m_vTBOUND.push_back(m_pMainChar); break;
		}
	}

	return TRUE;
}

BOOL CTClientGame::UseTRecallSkill( CTClientRecall *pTRECALL, DWORD dwTick)
{
	CTClientObjBase *pTARGET = FindOBJ(
		pTRECALL->m_dwTargetID,
		pTRECALL->m_bTargetType);

	if( pTRECALL->IsBlock() ||
		pTRECALL->IsDead() ||
		pTRECALL->IsDown() ||
		!pTRECALL->m_pTCURSKILL ||
		(pTARGET && pTARGET->IsDead()) ||
		pTRECALL->m_pTCURSKILL->m_bTimerON ||
		pTRECALL->m_pTCURSKILL->m_dwTick ||
		pTRECALL->m_pTCURSKILL->m_dwExceptTick )
		return FALSE;

	D3DXVECTOR3 vGROUND = pTARGET && (pTRECALL->m_bRecallType != TRECALLTYPE_MINE ||
		pTRECALL->m_pTCURSKILL->m_pTSKILL->m_bRangeType != TSKILLRANGE_CIRCLE) ? D3DXVECTOR3(
		pTARGET->GetPositionX(),
		pTARGET->GetPositionY(),
		pTARGET->GetPositionZ()) : D3DXVECTOR3(
		pTRECALL->GetPositionX(),
		pTRECALL->GetPositionY(),
		pTRECALL->GetPositionZ());

	TSKILLTARGET_PARAM param;
	if( FindSkillTarget(
		pTRECALL, 
		pTARGET, 
		&vGROUND, 
		pTRECALL->m_pTCURSKILL->m_pTSKILL, 
		&param) )
	{
		SendCS_SKILLUSE_REQ(
			pTRECALL,
			pTARGET,
			&vGROUND,
			pTRECALL->m_pTCURSKILL->m_pTSKILL,
			TRUE,
			&param);

		return TRUE;
	}

	return FALSE;
}

void CTClientGame::UseTSkill( WORD wID, BYTE bActivate, BYTE bCheckGround)
{
	if( m_pMainChar->m_bPrivateShop )
		return;

	if( m_pMainChar->GetRidingPet() )
	{
		m_pChatFrame->ChatSysMSG(
			CTChart::LoadString( TSTR_INFO_TITLE),
			m_pMainChar->GetName(),
			CTChart::LoadString(TSTR_DO_NOT_USE_SKILL_ON_RIDING),
			TCOLOR_ERROR,
			TCHAT_FLAG_INFO,
			ID_SND_ERROR,
			MEDIA_TSOUND);

		return ;
	}
	
	if( !m_bCanUseSkill )
		return ;

	// 변장 상태에서의 스킬 사용을 막음!!
	if( m_pMainChar->IsDisguise() )
	{
		CString strERR;

		strERR = CTChart::LoadString( TSTR_ERR_DOSKILLDISGUISE);
		ShowInfoChat(strERR, TUISND_TYPE_ERROR);

		return;
	}

	if( m_bTournamentBattle )
	{
		if( wID == TOURNAMENT_CANTUSESKILL )
		{
			ShowInfoChat(CTChart::LoadString(TSTR_TOURNAMENT_CANT_USE_SKILL), TUISND_TYPE_ERROR);
			return ;
		}
	}

	CTClientSkill *pTSKILL = m_pMainChar->FindTSkill(wID);
	if( !CanControl() || m_bLoopLock )
		return;

	if(!pTSKILL)
	{
		SkillMSG( m_pMainChar, SKILL_NOTFOUND);
		return;
	}

	//////////////////////////////////////////////////////////////////////////
	// Skill Hot Key로 취소하기
	// 임시 블럭
	//CTClientMaintain* pTMAINTAIN = m_pMainChar->FindMaintain( wID ); //pOBJ->FindMaintain( wSkillID );
	//
	//if ( pTMAINTAIN != NULL )
	//{
	//	for( int i = 0; i < INT(pTMAINTAIN->m_pTSKILL->m_vTFUNCTION.size()); i++ )
	//	{
	//		if( (pTMAINTAIN->m_pTSKILL->m_vTFUNCTION[i]->m_bType == SDT_STATUS && pTMAINTAIN->m_pTSKILL->m_vTFUNCTION[i]->m_bFunctionID == SDT_STATUS_HIDE) ||
	//			(pTMAINTAIN->m_pTSKILL->m_vTFUNCTION[i]->m_bType == SDT_TRANS && pTMAINTAIN->m_pTSKILL->m_vTFUNCTION[i]->m_bFunctionID == SDT_TRANS_CROW) )
	//		{
	//			SendCS_SKILLEND_REQ(
	//				m_pMainChar->m_dwID,
	//				m_pMainChar->m_bType,
	//				m_pMainChar->GetHostID(),
	//				pTMAINTAIN->m_dwAttackID,
	//				pTMAINTAIN->m_bAttackType,
	//				wID,
	//				m_vMAP.m_wMapID,
	//				m_pMainWnd->m_bChannel);

	//			return;
	//		}
	//	}
	//}

	////////////////////////////////////////////////////////////////////////////

	if(m_pMainChar->m_bActionLock)
	{
		m_wNextSkillID = pTSKILL->m_pTSKILL->m_wSkillID;
		return;
	}

	BYTE bRANGE = pTSKILL->m_pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE ||
		pTSKILL->m_pTSKILL->m_bRangeType == TSKILLRANGE_LINE ||
		pTSKILL->m_pTSKILL->m_bRangeType == TSKILLRANGE_MCIRCLE ? TRUE : FALSE;

	BYTE bPOINT = pTSKILL->m_pTSKILL->m_bRangeType == TSKILLRANGE_POINT ? TRUE : FALSE;
	BYTE bMSG = !pTSKILL->m_bLoopON;

	if(!bActivate)
	{
		if( pTSKILL->m_bTimerON || pTSKILL->m_dwTick )
		{
			if(bMSG)
				SkillMSG( m_pMainChar, SKILL_SPEEDYUSE);

			return;
		}

		if(m_bCHARGE)
			return;

		if( CTClientGame::m_vTOPTION.m_bAutoTargeting &&
			(!m_pTARGET || m_pTARGET->m_bAction == TA_DEAD) &&
			(!m_pACT) &&
			pTSKILL->m_pTSKILL->m_bRangeType == TSKILLRANGE_NONE &&
			pTSKILL->m_pTSKILL->m_bTargetType == TSKILLTARGET_ENEMY )
		{
			PickNext(TPICK_ALIVE_MONSTER);
			m_pACT = m_pTARGET;
		}

		ResetActOBJ(pTSKILL->m_pTSKILL);
	}
	else if(!m_pACT)
	{
		m_pACT = GetSkillTarget( NULL, pTSKILL->m_pTSKILL);
	}

	BYTE bRESULT = CanUseSkill(
		m_pMainChar,
		m_pACT,
		bCheckGround ? NULL : &m_vTGROUND,
		pTSKILL->m_pTSKILL);

	switch(bRESULT)
	{
	case SKILL_WRONGDIR		:
		if(!CanLookAt())
		{
			if(bMSG)
				SkillMSG( m_pMainChar, SKILL_WRONGDIR);
			break;
		}
		else if( bRANGE || bPOINT || m_pMainChar != m_pACT )
		{
			D3DXVECTOR3 vTARGET = bRANGE || bPOINT ? D3DXVECTOR3(
				m_vTGROUND.x - m_pMainChar->GetPositionX(),
				m_vTGROUND.y - m_pMainChar->GetPositionY(),
				m_vTGROUND.z - m_pMainChar->GetPositionZ()) : D3DXVECTOR3(
				m_pACT->GetPositionX() - m_pMainChar->GetPositionX(),
				m_pACT->GetPositionY() - m_pMainChar->GetPositionY(),
				m_pACT->GetPositionZ() - m_pMainChar->GetPositionZ());

			WORD wPITCH = m_pMainChar->GetTargetPITCH(
				vTARGET.x,
				vTARGET.y,
				vTARGET.z);

			WORD wDIR = m_pMainChar->GetTargetDIR(
				vTARGET.x,
				vTARGET.z);

			LookAt( wDIR, wPITCH);
		}
	case SKILL_SUCCESS		:
		{
			if( m_bMoveType != TMAINMOVE_NORMAL && CanLookAt() )
			{
				m_bMoveType = TMAINMOVE_NORMAL;
				m_dwMoveGM = 0;
				m_pMainChar->m_wMoveDIR = MOVE_NONE;

				D3DXVECTOR3 vTARGET = bRANGE || bPOINT ? D3DXVECTOR3(
					m_vTGROUND.x - m_pMainChar->GetPositionX(),
					m_vTGROUND.y - m_pMainChar->GetPositionY(),
					m_vTGROUND.z - m_pMainChar->GetPositionZ()) : D3DXVECTOR3(
					m_pACT->GetPositionX() - m_pMainChar->GetPositionX(),
					m_pACT->GetPositionY() - m_pMainChar->GetPositionY(),
					m_pACT->GetPositionZ() - m_pMainChar->GetPositionZ());

				WORD wPITCH = m_pMainChar->GetTargetPITCH(
					vTARGET.x,
					vTARGET.y,
					vTARGET.z);

				WORD wDIR = m_pMainChar->GetTargetDIR(
					vTARGET.x,
					vTARGET.z);

				LookAt( wDIR, wPITCH);
			}

			if(pTSKILL->m_pTSKILL->m_bLoop)
			{
				LoopSkillOFF();
				pTSKILL->m_bLoopON = TRUE;
			}

			if( m_pACT && m_pACT != m_pMainChar )
			{
				CTClientObjBase::m_mapTSELECTOBJ.clear();
				CTClientObjBase::m_mapTSELECTOBJ.insert( MAPTSELECTOBJ::value_type( m_pACT, m_pACT));
			}

			if( CTClientSkill::IsAttackSKILL(pTSKILL->m_pTSKILL) && m_pMainChar->m_bMode != MT_BATTLE )
				m_pSESSION->SendCS_CHGMODE_REQ(MT_BATTLE);

			if((pTSKILL->m_pTSKILL->m_dwSpellTick ||
				!CTChart::GetTACTION( pTSKILL->m_pTSKILL->m_bActionID[TSKILLACTION_MOVEACTIVATE] )->m_bSubAction) && (
				m_pMainChar->m_wMoveDIR != MOVE_NONE ||
				m_pMainChar->IsJump() || m_pMainChar->IsFall()) )
			{
				SkillMSG( m_pMainChar, SKILL_NOTMOVESKILL);
				break;
			}

			if( !bActivate && pTSKILL->m_pTSKILL->m_dwSpellTick )
			{
				TACTION vActionID = m_pMainChar->FindActionID(
					pTSKILL->m_pTSKILL->m_bActionID[TSKILLACTION_SPELL],
					m_pMainChar->GetWeaponID(MT_BATTLE));
				m_bLoopLock = TRUE;

				SendCS_ACTION_REQ(
					m_pMainChar->m_dwID,
					m_pMainChar->m_bType,
					pTSKILL->m_pTSKILL->m_bActionID[TSKILLACTION_SPELL],
					vActionID.m_dwActID,
					vActionID.m_dwAniID,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID,
					pTSKILL->m_pTSKILL->m_wSkillID);
			}
			else
			{
				LPD3DXVECTOR3 pGROUND = bRANGE || bPOINT ? &m_vTGROUND : NULL;
				CTClientObjBase* pDEFEND = bRANGE ? NULL : m_pACT;

				TSKILLTARGET_PARAM param;
				m_bLoopLock = TRUE;

				if( FindSkillTarget(
					m_pMainChar, 
					pDEFEND,
					pGROUND,
					pTSKILL->m_pTSKILL,
					&param) )
				{
					SendCS_SKILLUSE_REQ(
						m_pMainChar,
						pDEFEND,
						pGROUND,
						pTSKILL->m_pTSKILL,
						TRUE,
						&param);
				}

				if(pTSKILL->m_pTSKILL->m_bContinue)
					LoopSkillOFF();
				else
					m_pACT = NULL;
			}
		}

		return;

	case SKILL_NEEDGROUND	:
		{
			m_wSkillID = pTSKILL->m_pTSKILL->m_wSkillID;

			m_bGroundATK = TRUE;
			m_bAutoRun = FALSE;

			if(m_bCHARGE)
			{
				SendCS_ACTION_REQ(
					m_pMainChar->m_dwID,
					m_pMainChar->m_bType,
					0, 0, 0,
					m_pMainWnd->m_bChannel,
					m_vMAP.m_wMapID, 0);

				CancelCharge(TRUE);
			}
		}

		return;

	case SKILL_TOOFAR		:
		{
			if( CTClientSkill::IsAttackSKILL(pTSKILL->m_pTSKILL) && m_pMainChar->m_bMode != MT_BATTLE )
				m_pSESSION->SendCS_CHGMODE_REQ(MT_BATTLE);

			if(pTSKILL->m_pTSKILL->m_bLoop)
			{
				LoopSkillOFF();
				pTSKILL->m_bLoopON = TRUE;
			}

			if(CanLookAt())
			{
				m_bMoveType = bRANGE || bPOINT ? TMAINMOVE_GROUND : TMAINMOVE_TARGET;
				m_wSkillID = pTSKILL->m_pTSKILL->m_wSkillID;
				m_bAutoRun = FALSE;

				m_fBOUND = TMAXRANGE_RATIO * m_pMainChar->GetMaxRange(
					!bRANGE && !bPOINT ? m_pACT : NULL,
					pTSKILL->m_pTSKILL);

				m_dwMoveGM = GM_ATTACK;
			}
			else if(bMSG)
				SkillMSG( m_pMainChar, SKILL_TOOFAR);
		}

		break;

	default					:
		if(bMSG)
			SkillMSG( m_pMainChar, bRESULT);

		break;
	}

	m_pMainChar->CancelTAction();
	SendCS_ACTION_REQ(
		m_pMainChar->m_dwID,
		m_pMainChar->m_bType,
		m_pMainChar->m_bAction,
		0, 0,
		m_pMainWnd->m_bChannel,
		m_vMAP.m_wMapID, 0);
	m_pACT = NULL;
}

void CTClientGame::LookAt( WORD wDIR,
						   WORD wPITCH)
{
	if( m_bAutoRun || m_pMainChar->m_wMoveDIR != MOVE_NONE ||
		m_vKEY.m_vSTATE[TKEY_FORWARD] ||
		m_vKEY.m_vSTATE[TKEY_BACK] ||
		m_vKEY.m_vSTATE[TKEY_RIGHT] ||
		m_vKEY.m_vSTATE[TKEY_LEFT] ||
		m_vKEY.m_vSTATE[TKEY_RSIDE] ||
		m_vKEY.m_vSTATE[TKEY_LSIDE] ||
		m_vKEY.m_vSTATE[TKEY_ROT] )
		return;

	m_pMainChar->m_bMouseDIR = TKDIR_N;
	m_pMainChar->m_bKeyDIR = TKDIR_N;

	if( m_pMainChar->GetRidingPet() == NULL )
		m_pMainChar->SetTAction(m_pMainChar->GetTAction(TKDIR_N));

	SendCS_MOVE_REQ(
		m_vMAP.m_wMapID,
		m_pMainChar->GetPositionX(),
		m_pMainChar->GetPositionY(),
		m_pMainChar->GetPositionZ(),
		wPITCH,
		wDIR,
		m_pMainChar->m_bMouseDIR,
		m_pMainChar->m_bKeyDIR,
		m_pMainChar->m_bAction,
		m_pMainChar->m_bGhost);

	m_pMainChar->CalcDIR( m_pMainChar->m_wMoveDIR, wDIR, wPITCH);

	if(GetPartyID(m_pMainChar))
		m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
}

void CTClientGame::LoopSkillOFF()
{
	MAPTSKILL::iterator itTSKILL;

	for( itTSKILL = m_pMainChar->m_mapTSKILL.begin(); itTSKILL != m_pMainChar->m_mapTSKILL.end(); itTSKILL++)
		(*itTSKILL).second->m_bLoopON = FALSE;
}

BYTE CTClientGame::CheckRevival( LPD3DXVECTOR3 vTARGET )
{
	MAPTGATE::iterator itTGATE;
	BYTE bRESULT = TRUE;

	FLOAT fDIST = D3DXVec2Length(&D3DXVECTOR2(
		m_pMainChar->GetPositionX() - vTARGET->x,
		m_pMainChar->GetPositionZ() - vTARGET->z)) + m_pMainChar->m_fRadius;

	for( itTGATE = m_vMAP.m_mapTGate.begin(); itTGATE != m_vMAP.m_mapTGate.end(); itTGATE++)
	{
		D3DXVECTOR3 vTEND = m_pMainChar->GetPosition();
		D3DXVECTOR3 vTSTART = (*vTARGET); //m_pMainChar->m_vTDEAD;
		FLOAT fRESULT = 0.0f;

		if( !(*itTGATE).second->m_bGARBAGE && (*itTGATE).second->IsActiveTGate() &&
			(*itTGATE).second && D3DXVec2Length(&D3DXVECTOR2(
			(*itTGATE).second->GetPositionX() - vTARGET->x,
			(*itTGATE).second->GetPositionZ() - vTARGET->z)) < (*itTGATE).second->m_fRadius + fDIST &&
			m_pMainChar->CheckCollision( (*itTGATE).second, &vTSTART, &vTEND, m_pMainChar->m_fSizeY, &fRESULT) )
		{
			bRESULT = FALSE;
			break;
		}
	}

	return bRESULT;
}

void CTClientGame::Revival( CTClientObjBase *pREVIVAL,
						    FLOAT fPosX,
							FLOAT fPosY,
							FLOAT fPosZ)
{
	pREVIVAL->m_bACTLevel = CTChart::GetTACTION( 
		TA_STAND )->m_bLevel;
	pREVIVAL->m_bAction = TA_STAND;
	pREVIVAL->m_bMode = MT_NORMAL;
	pREVIVAL->m_bGhost = FALSE;
	pREVIVAL->m_fSpeedFactor = TDEF_SPEED;

	pREVIVAL->m_dwHP = pREVIVAL->m_dwMaxHP;
	pREVIVAL->m_dwMP = pREVIVAL->m_dwMaxMP;

	pREVIVAL->m_vWorld._41 = fPosX;
	pREVIVAL->m_vWorld._42 = fPosY;
	pREVIVAL->m_vWorld._43 = fPosZ;
	pREVIVAL->SetPosition( pREVIVAL->m_vWorld );

	if( pREVIVAL == m_pMainChar )
	{
		ReleaseForTELEPORT();

		m_vMAP.ResetTMAP(
			m_pDevice,
			m_pCamera,
			0, TRUE);

		m_vMAP.FixTOBJHeight(m_pMainChar);
		ResetTSKY();
		m_bRevival = FALSE;
		ResetSkillICON();

		m_vMAP.LandTOBJ(m_pMainChar);
		m_vMainDead.RemoveAllTMagicItemSFX();
		m_vMainDead.RemoveAllEquipSFX();
		m_vMainDead.RemoveAllWeaponSFX();

		// 공성전중이면 부활하면서 ui를 켜준다.
		if( m_pMainChar->m_bEnterCastle )
		{
			CTSiegeUI* pSiegeUI = static_cast< CTSiegeUI* >( m_vTFRAME[TFRAME_SIEGEUI] );
			pSiegeUI->ShowComponent( TRUE );
			pSiegeUI->EnableComponent( TRUE );
		}
	}
}

BOOL CTClientGame::CheckSkillHasFunc( LPTSKILL pTSKILL,
									  BYTE bType,
									  BYTE bFunctionID)
{
	for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
	{
		LPTSKILLFUNCTION pTSKILLFUNC = pTSKILL->m_vTFUNCTION[i];

		if( pTSKILLFUNC->m_bType == bType && pTSKILLFUNC->m_bFunctionID == bFunctionID)
			return TRUE;
	}

	return FALSE;
}

void CTClientGame::NotifyObjHide(CTClientObjBase *pTARGET)
{
	if(	!pTARGET || 
		pTARGET->m_bType != OT_PC || 
		!GetPartyID(m_pMainChar) || 
		!pTARGET->m_bCanSelected ||
		m_pMainChar->IsAlliance(pTARGET))
	{
		return;
	}

	if( !pTARGET->m_bHide && m_pTRSCS->m_vTCORPS.m_mapPC.find(pTARGET->m_dwID) == m_pTRSCS->m_vTCORPS.m_mapPC.end() )
	{
		RelaySession()->SendCS_ADDCORPSENEMY_REQ(
			m_pMainChar->m_dwID,
			m_vMAP.m_wMapID,
			static_cast<CTClientMoveObj*>(pTARGET));
	}							
	else if( pTARGET->m_bHide && m_pTRSCS->m_vTCORPS.m_mapPC.find(pTARGET->m_dwID) != m_pTRSCS->m_vTCORPS.m_mapPC.end() )
	{
		RelaySession()->SendCS_DELCORPSENEMY_REQ(
			m_pMainChar->m_dwID,
			pTARGET->m_dwID,
			pTARGET->m_bType);
	}
}

void CTClientGame::NotifyObjDie(CTClientObjBase *pTARGET)
{
	if( pTARGET == m_pTARGET )
	{
		CTClientObjBase::m_mapTSELECTOBJ.clear();
		ResetTARGET();
	}
}

void CTClientGame::BuildPickList(
	LPLISTTOBJBASE pLIST, 
	TPICK ePick )
{
	m_pCtrlHost = GetCtrlOBJ();
	pLIST->clear();

	for( int i=0; i<INT(m_vMAP.m_vDRAWBSP.size()); i++)
		BuildPickList( pLIST, &m_vMAP.m_vDRAWBSP[i]->m_vTDRAW, ePick );
	BuildPickList( pLIST, &m_vMAP.m_vTDRAW, ePick );

	pLIST->sort(binary_pick());
}

void CTClientGame::BuildPickList(
	LPLISTTOBJBASE pLIST,
	LPLISTTOBJBASE pTSRC,
	TPICK ePick )
{
	LISTTOBJBASE::iterator itTOBJ;

	for( itTOBJ = pTSRC->begin(); itTOBJ != pTSRC->end(); itTOBJ++)
	{
		BYTE bType = (*itTOBJ)->m_bType;

		switch( ePick )
		{
		case TPICK_ALIVE_ALL:
			{
				if( (*itTOBJ)->IsDead() )
					continue;
			}
			break;
		case TPICK_DEAD_ALL:
			{
				if( FALSE == (*itTOBJ)->IsDead() )
					continue;
			}
			break;
		case TPICK_ALIVE_MONSTER:
			{
				if( bType != OT_MON )
					continue;
				
				if( (*itTOBJ)->IsDead() )
					continue;
			}
			break;

		case TPICK_ALIVE_PLAYER:
			{
				if( bType == OT_MON )
					continue;

				if( (*itTOBJ)->IsDead() )
					continue;
			}
			break;

		case TPICK_DEAD_MONSTER:
			{
				if( bType != OT_MON )
					continue;

				if( FALSE == (*itTOBJ)->IsDead() )
					continue;
			}
			break;

		case TPICK_DEAD_PLAYER:
			{
				if( bType == OT_MON )
					continue;

				if( FALSE == (*itTOBJ)->IsDead() )
					continue;
			}
			break;
		}

		if(!(*itTOBJ)->m_bGARBAGE)
		{
			switch( bType )
			{
			case OT_RECALL:
			case OT_SELF:
			case OT_MON:
			case OT_PC:
				if( !m_pMainChar->IsDead() && !m_pMainChar->m_bGhost &&
					(*itTOBJ)->m_bCanSelected && (*itTOBJ) != m_pMainChar &&
					!m_pMainChar->IsAlliance(*itTOBJ) )
				{
					(*itTOBJ)->m_fZValue = D3DXVec3Length(&D3DXVECTOR3(
						m_pCtrlHost->GetPositionX() - (*itTOBJ)->GetPositionX(),
						m_pCtrlHost->GetPositionY() - (*itTOBJ)->GetPositionY(),
						m_pCtrlHost->GetPositionZ() - (*itTOBJ)->GetPositionZ()));

					if( (*itTOBJ)->m_fZValue < TPICK_DIST )
					{
						D3DXVECTOR2 vDIR(
							(*itTOBJ)->GetPositionX() - m_pMainChar->GetPositionX(),
							(*itTOBJ)->GetPositionZ() - m_pMainChar->GetPositionZ()); // 캐릭터에서 타겟을 보는 Direction. (캐릭터가 보는 방향이 아니다)
						D3DXVec2Normalize( &vDIR, &vDIR);

						int nTargetDIR = m_pMainChar->GetTargetDIR( vDIR.x, vDIR.y); // Direction을 각도로 바꿈.
						nTargetDIR = abs(nTargetDIR-m_pMainChar->m_wDIR); // 캐릭터가 보고 있는 방향과의 차를 구함.
						nTargetDIR = nTargetDIR > 900 ? 1800-nTargetDIR : nTargetDIR; // 180도 넘는 경우, 예를 들면 270도를 90도로 바꿈.

						(*itTOBJ)->m_fPickWeight =
							(TPICK_DIST- (*itTOBJ)->m_fZValue ) / TPICK_DIST * 43.0f
							+ (900.0f - nTargetDIR) / 900.0f * 57.0f; // 픽킹가중치, 거리는 43%, 각도는 57%, 최대값은 100이다.

						if(!((*itTOBJ)->m_vTSKILLTARGET.empty()))
						{
							VTSKILLTARGET::iterator itST, endST;
							itST = (*itTOBJ)->m_vTSKILLTARGET.begin();
							endST = (*itTOBJ)->m_vTSKILLTARGET.end();

							for(; itST != endST ; ++itST)
								if( (*itST)->m_dwID == m_pMainChar->m_dwID )
								{
									(*itTOBJ)->m_fPickWeight += 100.0f; // 나를 타겟으로 삼고 있는 대상을 최우선으로 삼는다. (위의 최대값 100을 더해주는 이유)
									break;
								}
						}

						pLIST->push_back((*itTOBJ));
					}
				}

				break;
			}
		}
	}
}

void CTClientGame::PickNext( TPICK ePick )
{
	BOOL bDead = (ePick == TPICK_DEAD_ALL || ePick == TPICK_DEAD_MONSTER || ePick == TPICK_DEAD_PLAYER);

	CTClientObjBase *pTARGET = NULL;

	if(m_pMainChar->m_bFlash)
		return;

	LISTTOBJBASE vLIST;
	BuildPickList(
		&vLIST,
		ePick );

	MAPTSELECTOBJ* pTSELMAP;
	if( bDead )
		pTSELMAP = &CTClientObjBase::m_mapTSELECTDEADOBJ;
	else
		pTSELMAP = &CTClientObjBase::m_mapTSELECTOBJ;

	LISTTOBJBASE::iterator it;
	for( it = vLIST.begin(); it != vLIST.end(); it++)
	{
		MAPTSELECTOBJ::iterator finder = pTSELMAP->find(*it);

		if( finder == pTSELMAP->end() )
		{
			if( (*it) == m_pTARGET ||
				(bDead && (*it) && (*it)->m_bTakenItemAll) )
				continue;

			pTARGET = (*it);
			break;
		}
	}

	if(!pTARGET)
	{
		pTSELMAP->clear();

		if(!bDead && !vLIST.empty() )
			pTARGET = *(vLIST.begin());
	}

	if( pTARGET && pTARGET->m_bType == OT_RECALL &&
		static_cast<CTClientRecall*>(pTARGET)->m_bRecallType == TRECALLTYPE_PET )
	{
		CTClientPet* pPET = static_cast<CTClientPet*>(pTARGET);
		CTClientChar* pChar = pPET->GetTakeUpChar();
		if( pChar )
			pTARGET = pChar;
	}

	ResetTargetOBJ(pTARGET);
	ResetTargetINFO(FALSE);
	vLIST.clear();
}

void CTClientGame::SkillMSG( CTClientObjBase *pTATTACK,
							 BYTE bERROR)
{
	CString strTITLE = pTATTACK ? pTATTACK->GetName() : CString(_T(""));
	CString strTYPE;
	CString strMSG;
	BYTE bSound = TRUE;

	strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
	strMSG.Empty();

	switch(bERROR)
	{
	case SKILL_NEEDSKILLPOINT	: strMSG = CTChart::LoadString( TSTR_ERROR_NEEDSKILLPOINT); break;
	case SKILL_UNSUITWEAPON		: strMSG = CTChart::LoadString( TSTR_ERROR_NEEDWEAPON); break;
	case SKILL_NEEDPREVACT		: strMSG = CTChart::LoadString( TSTR_ERROR_NEEDPREVACT); break;
	case SKILL_NEEDLEVELUP		: strMSG = CTChart::LoadString( TSTR_ERROR_NEEDLEVELUP); break;
	case SKILL_CANNOTSEE		: strMSG = CTChart::LoadString( TSTR_ERROR_WRONGTARGET); break;
	case SKILL_WRONGTARGET		: strMSG = CTChart::LoadString( TSTR_ERROR_WRONGTARGET); break;
	case SKILL_WRONGREGION		: strMSG = CTChart::LoadString( TSTR_ERROR_WRONGREGION); break;
	case SKILL_NEEDPARENT		: strMSG = CTChart::LoadString( TSTR_ERROR_NEEDPARENT); break;
	case SKILL_MATCHCLASS		: strMSG = CTChart::LoadString( TSTR_ERROR_MATCHCLASS); break;
	case SKILL_NEEDMONEY		: strMSG = CTChart::LoadString( TSTR_ERROR_NEEDMONEY); break;
	case SKILL_NOTFOUND			: strMSG = CTChart::LoadString( TSTR_ERROR_NOTFOUND); break;
	case SKILL_TOOCLOSE			: strMSG = CTChart::LoadString( TSTR_ERROR_TOOCLOSE); break;
	case SKILL_WRONGDIR			: strMSG = CTChart::LoadString( TSTR_ERROR_LOOKAT); break;
	case SKILL_ALREADY			: strMSG = CTChart::LoadString( TSTR_ERROR_MAXLEVEL); break;
	case SKILL_SILENCE			: strMSG = CTChart::LoadString( TSTR_ERROR_SILENCE); break;
	case SKILL_NEEDMP			: strMSG = CTChart::LoadString( TSTR_ERROR_NEEDMP); break;
	case SKILL_NEEDHP			: strMSG = CTChart::LoadString( TSTR_ERROR_NEEDHP); break;
	case SKILL_TOOFAR			: strMSG = CTChart::LoadString( TSTR_ERROR_TOOFAR); break;
	case SKILL_TRANS			: strMSG = CTChart::LoadString( TSTR_ERROR_TRANS); break;
	case SKILL_MODE				: strMSG = CTChart::LoadString( TSTR_ERROR_SKILLMODE); break;
	case SKILL_HIDE				: strMSG = CTChart::LoadString( TSTR_ERROR_HIDE); break;
	case SKILL_STUN				: strMSG = CTChart::LoadString( TSTR_ERROR_STUN); break;
	case SKILL_DEAD				: strMSG = CTChart::LoadString( TSTR_ERROR_DEAD); break;
	case SKILL_PEACEZONE		: strMSG = CTChart::LoadString( TSTR_ERROR_PEACEZONE); break;
	case SKILL_NOTARGET			: strMSG = CTChart::LoadString( TSTR_ERROR_NOTARGET); break;
	case SKILL_NOTMOVESKILL		: strMSG = CTChart::LoadString( TSTR_ERROR_NOTMOVESKILL); break;
	case SKILL_ACTIONLOCK		: strMSG = CTChart::LoadString( TSTR_ERROR_ACTIONLOCK); break;
	case SKILL_SPEEDYUSE		:
		{
			strMSG = CTChart::LoadString( TSTR_ERROR_SPEEDYUSE);
			bSound = FALSE;
		}

		break;
	}

	if( !strTITLE.IsEmpty() &&
		!strTYPE.IsEmpty() &&
		!strMSG.IsEmpty() )
	{
		m_pChatFrame->ChatSysMSG( strTYPE, strTITLE, strMSG,
			TCOLOR_ERROR, TCHAT_FLAG_INFO, bSound ? ID_SND_ERROR : 0, MEDIA_TSOUND);
	}
}

void CTClientGame::CorpsMSG( CTClientObjBase *pTOBJ,
							 BYTE bERROR)
{
	CString strTITLE = pTOBJ ? pTOBJ->GetName() : CString(_T(""));
	CString strTYPE;
	CString strMSG;

	strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
	strMSG.Empty();

	switch(bERROR)
	{
	case CORPS_TARGET_NO_PARTY	: strMSG = CTChart::LoadString( TSTR_ERROR_TARGET_NO_PARTY); break;
	case CORPS_NOT_COMMANDER	: strMSG = CTChart::LoadString( TSTR_ERROR_NOT_COMMANDER); break;
	case CORPS_WRONG_TARGET: strMSG = CTChart::LoadString( TSTR_ERROR_WRONG_TARGET); break;
	case CORPS_NO_PARTY: strMSG = CTChart::LoadString( TSTR_ERROR_NEED_PARTY); break;
	case CORPS_ALREADY: strMSG = CTChart::LoadString( TSTR_ERROR_CORPS_ALREADY); break;
	case CORPS_DEAD: strMSG = CTChart::LoadString( TSTR_ERROR_CORPS_DEAD); break;
	case CORPS_MAX_PARTY: strMSG = CTChart::LoadString( TSTR_ERROR_CORPS_MAX ); break;
	}

	if( !strTITLE.IsEmpty() &&
		!strTYPE.IsEmpty() &&
		!strMSG.IsEmpty() )
	{
		m_pChatFrame->ChatSysMSG( strTYPE, strTITLE, strMSG, 
			TCOLOR_ERROR, TCHAT_FLAG_INFO, ID_SND_ERROR, MEDIA_TSOUND);
	}
}

void CTClientGame::TradeMSG( CString strNAME,
							 BYTE bERROR)
{
	CString strTYPE;
	CString strMSG;

	strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
	strMSG.Empty();

	switch(bERROR)
	{
	case DEALITEM_INVALIDITEM	: strMSG = CTChart::Format( TSTR_ERROR_TRADE_INVALIDITEM, strNAME); break;
	case DEALITEM_NOTARGET		: strMSG = CTChart::LoadString( TSTR_ERROR_TRADE_TARGET); break;
	case DEALITEM_OVERMONEY		:
	case DEALITEM_NOMONEY		: strMSG = CTChart::Format( TSTR_ERROR_TRADE_NOMONEY, strNAME); break;
	case DEALITEM_NOINVEN		:
	case DEALITEM_NOITEM		: strMSG = CTChart::Format( TSTR_ERROR_TRADE_NOITEM, strNAME); break;
	case DEALITEM_DEALING		: strMSG = CTChart::Format( TSTR_ERROR_TRADE_ALREADY, strNAME); break;
	case DEALITEM_CANCEL		:
		if( strNAME != m_pMainChar->GetName() )
			strMSG = CTChart::Format( TSTR_ERROR_TRADE_CANCEL, strNAME);

		break;

	case DEALITEM_DENY			:
		if( strNAME != m_pMainChar->GetName() )
			strMSG = CTChart::Format( TSTR_ERROR_TRADE_DENY, strNAME);

		break;

	case DEALITEM_BUSY			:
		if( strNAME != m_pMainChar->GetName() )
			strMSG = CTChart::Format( TSTR_ERR_TARGET_BUSY, strNAME);

		break;

	case DEALITEM_ENEMY			: strMSG = CTChart::LoadString( TSTR_ERROR_TRADE_ENEMY); break;
	case DEALITEM_CANTRECV		: strMSG = CTChart::LoadString( TSTR_ERROR_TRADE_CANTRECV); break;
	case DEALITEM_SUCCESS		: strMSG = CTChart::Format( TSTR_ERROR_TRADE_SUCCESS, strNAME); break;
	}

	if( !strTYPE.IsEmpty() &&
		!strMSG.IsEmpty() )
	{
		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, 
			TCOLOR_ERROR, TCHAT_FLAG_INFO, ID_SND_ERROR, MEDIA_TSOUND);
	}
}

void CTClientGame::ItemMSG( BYTE bERROR)
{
	CString strTYPE;
	CString strMSG;

	strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
	strMSG.Empty();

	switch(bERROR)
	{
	case MI_BOTHHANDWEAPON	: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_BOTHHANDWEAPON); break;
	case MI_NOMATCHCLASS	: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_NOMATCHCLASS); break;
	case MI_CANNOTEQUIP		: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_CANNOTEQUIP); break;
	case MI_NODESTINVEN		: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_NODESTINVEN); break;
	case MI_NOSRCINVEN		: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_NOSRCINVEN); break;
	case MI_NOSRCITEM		: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_NOSRCITEM); break;
	case MI_INVENFULL		: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_INVENFULL); break;
	case MI_LOWLEVEL		: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_LEVEL); break;
	case MI_DEALING			: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_TRADE); break;
	case MI_NOSKILL			: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_NOSKILL); break;
	case MI_BLOCK			: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_BLOCK); break;
	case MI_DEAD			: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_DEAD); break;
	case MI_CANTDROP	: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_CANTDROP); break;
	case MI_WRAP: strMSG = CTChart::LoadString( TSTR_ERROR_ITEM_WRAP ); break;
	}

	if( !strTYPE.IsEmpty() &&
		!strMSG.IsEmpty() )
	{
		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, 
			TCOLOR_ERROR, TCHAT_FLAG_INFO, ID_SND_ERROR, MEDIA_TSOUND);
	}
}

DWORD CTClientGame::GetObjectColor( CTClientObjBase *pTOBJ)
{
	if( pTOBJ->IsDead() )
		return TCOLOR_DEAD;

	if( m_bDuel )
	{
		CTClientObjBase* pTOBJHOST = pTOBJ;
		if( pTOBJ->m_bType == OT_RECALL || pTOBJ->m_bType == OT_SELF )
			pTOBJHOST = FindPC(pTOBJ->GetHostID());

		if( pTOBJHOST == m_pDuelTarget )
			return TCOLOR_ENEMY_HEAVY;
	}

	if( m_bTournamentBattle )
	{
		CTClientObjBase* pTOBJHOST = pTOBJ;
		if( pTOBJ->m_bType == OT_RECALL || pTOBJ->m_bType == OT_SELF )
			pTOBJHOST = FindPC(pTOBJ->GetHostID());

		if( pTOBJHOST &&
			(pTOBJHOST->m_bTournamentTeam != TTOURNAMENT_TEAM_NONE) &&
			(pTOBJHOST->m_bTournamentTeam != m_pMainChar->m_bTournamentTeam) )
			return TCOLOR_ENEMY_HEAVY;

		return TCOLOR_NEUTRALITY;
	}

	if( m_bTournamentWatching )
	{
		CTClientObjBase* pTOBJHOST = pTOBJ;
		if( pTOBJ->m_bType == OT_RECALL || pTOBJ->m_bType == OT_SELF )
			pTOBJHOST = FindPC(pTOBJ->GetHostID());

		if( pTOBJHOST )
		{
			if( m_pTLeftTeam &&
				m_pTLeftTeam->m_bSlotID == pTOBJHOST->m_bTournamentTeam )
				return TCOLOR_TOURNAMENT_RED;
			else if( m_pTRightTeam &&
				m_pTRightTeam->m_bSlotID == pTOBJHOST->m_bTournamentTeam )
				return TCOLOR_TOURNAMENT_BLUE;
			else
				return TCOLOR_ALLIANCE_NORMAL;
		}
	}

	if( pTOBJ->m_bColorID == TNCOLOR_ALLI )
	{
		if( pTOBJ->m_bType == OT_NPC )
			return TCOLOR_ALLIANCE_NPC;

		WORD wPartyID = GetPartyID(m_pMainChar);
		WORD wObjPartyID = GetPartyID(pTOBJ);
		if( wPartyID && wPartyID == wObjPartyID )
			return TCOLOR_ALLIANCE_PARTY;

		if ( wObjPartyID )
			return TCOLOR_ALLIANCE_OTHERPARTY;

		if( pTOBJ->m_bType == OT_RECALL && 
			static_cast<CTClientRecall*>(pTOBJ)->m_bRecallType == TRECALLTYPE_PET &&
			static_cast<CTClientPet*>(pTOBJ)->GetTakeUpChar() == NULL )
		{
			return TCOLOR_ALLIANCE_PET;
		}

		return TCOLOR_ALLIANCE_NORMAL;
	}
	else
	{
		BYTE bColorID = pTOBJ->m_bColorID;
		if( bColorID == TNCOLOR_ENEMY )
		{
			if( pTOBJ->m_bLevel + TLEVEL_BOUND < m_pMainChar->m_bLevel )
				bColorID = TNCOLOR_LIGHT;
			else if( m_pMainChar->m_bLevel + TLEVEL_BOUND < pTOBJ->m_bLevel )
				bColorID = TNCOLOR_HAVY;
			else
				bColorID = TNCOLOR_MIDDLE;
		}

		switch( bColorID )
		{
		case TNCOLOR_LIGHT:		return TCOLOR_ENEMY_LIGHT;
		case TNCOLOR_MIDDLE:	return TCOLOR_ENEMY_MIDDLE;
		case TNCOLOR_HAVY:		return TCOLOR_ENEMY_HEAVY;
		case TNCOLOR_NO_REWARD:	return TCOLOR_ENEMY_NOREWARD;
		}
	}

	return TCOLOR_NEUTRALITY;
}

BYTE CTClientGame::GetTargetKind( CTClientObjBase *pATTACK,
								  CTClientObjBase *pDEFEND,
								  CTClientObjBase *pTARGET,
								  LPTSKILL pTSKILL,
								  LPD3DXVECTOR2 pPOS)
{
	BYTE bRANGE = pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE ? TRUE : FALSE;
	BYTE bPOINT = pTSKILL->m_bRangeType == TSKILLRANGE_POINT ? TRUE : FALSE;

	if( !pTARGET->m_bCanSelected )
		return TARGETKIND_NONE;

	if( pTSKILL->m_bRangeType != TSKILLRANGE_NONE &&
		!CanDetect( pATTACK, pTARGET) )
		return TARGETKIND_NONE;

	if(!bRANGE)
		if( pTSKILL->m_bRangeType == TSKILLRANGE_TNEAR )
		{
			if( pTARGET == m_pMainChar )
				return TARGETKIND_TARGET;
		}
		else if( !bPOINT && pTSKILL->m_fAtkRange > 0.0f )
		{
			FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
				pTARGET->GetPositionX() - pPOS->x,
				pTARGET->GetPositionZ() - pPOS->y));

			if( fDist < pTSKILL->m_fAtkRange && CanDefend( pATTACK, pTARGET, pTSKILL) )
				return TARGETKIND_TARGET;
		}
		else if( pDEFEND == pTARGET && (bPOINT || CanDefend( pATTACK, pTARGET, pTSKILL)) )
			return TARGETKIND_TARGET;

	FLOAT fDist = D3DXVec2Length(&D3DXVECTOR2(
		pTARGET->GetPositionX() - pATTACK->GetPositionX(),
		pTARGET->GetPositionZ() - pATTACK->GetPositionZ()));

	if( fDist < TARGET_BOUND )
		return TARGETKIND_BOUND;

	return TARGETKIND_NONE;
}

BOOL CTClientGame::DoActCmd( const CString& strActCmd )
{
	if( m_pMainChar->IsDead() || m_pMainChar->IsDown() || m_pMainChar->IsFall() || 
		m_pMainChar->IsJump() || m_pMainChar->m_bPrivateShop || m_pMainChar->GetRidingPet() ||
		!m_bCanUseSkill )
	{
		return FALSE;
	}

	VTACTIONDATA::iterator itr, end;
	itr = CTChart::m_vTACTION.begin();
	end = CTChart::m_vTACTION.end();

	for(BYTE i=0; itr != end; ++itr,++i)
	{
		LPTACTIONDATA pACT = *itr;

		if( !pACT->m_strActCmd.IsEmpty() && strActCmd == pACT->m_strActCmd )
		{
			TACTION vActionID = m_pMainChar->FindActionID(
				i, m_pMainChar->GetWeaponID(m_pMainChar->m_bMode));

			m_pSESSION->SendCS_CHGMODE_REQ(MT_NORMAL);
			SendCS_ACTION_REQ(
				m_pMainChar->m_dwID,
				m_pMainChar->m_bType,
				i,
				vActionID.m_dwActID,
				vActionID.m_dwAniID,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID, 0);

			return TRUE;
		}
	}

	return FALSE;
}

void CTClientGame::EnableGuildUI(BOOL bEnable)
{
	CString msg;
	msg = CTChart::LoadString( TSTR_COMMMSG_NOGUILD);

	CTCommunityDlg* pCommDlg = static_cast<CTCommunityDlg*>(m_vTFRAME[TFRAME_COMMUNITY]);
	pCommDlg->SetBlank( TCOMMUNITY_FRAME_GUILDNORMAL,		!bEnable, msg );
	pCommDlg->SetBlank( TCOMMUNITY_FRAME_GUILDMEMBER,		!bEnable, msg );
	pCommDlg->SetBlank( TCOMMUNITY_FRAME_GUILDVOLUNTEER, 	!bEnable, msg );

	if( pCommDlg->IsVisible() )
	{
		pCommDlg->ShowComponent(FALSE);
		pCommDlg->ShowComponent(TRUE);
	}
}

void CTClientGame::NotifyNewMsgChat(CTMessengerChat* pChat)
{
	if( !pChat )
		return ;

	if( pChat->IsVisible() )
		return;

	// 메신저
	/*CTMsgChatMemberList* pList = m_pMessengerBase->GetMsgChatMemberList();
	INT nIdx = pList->FindByChat(pChat);
	if( nIdx != T_INVALID )
		pList->NotifyNewMsg(nIdx);*/

	LPMCTARGET_ARRAY pArray = pChat->GetTargets();
	if( !pArray->empty() )
	{
		CTMsgGroupList* pList = m_pMessengerBase->GetMsgGroupList();
		CTMsgGroupList::CMember* pMember = pList->FindMemberByID( (*pArray)[0].dwCharID );

		if( !pMember )
			return ;

		if( pMember->m_pParent )
			((CTMsgGroupList::CGroup*)(pMember->m_pParentGrp))->Spread(TRUE);

		pMember->m_bTwinkle = TRUE;
	}
    
	if( !m_pMessengerBase->IsVisible() )
	{
		CTMainUI* pMain = static_cast<CTMainUI*>( m_vTFRAME[TFRAME_MAIN] );
		pMain->SetNewMessengerMsg(TRUE);
	}
}

void CTClientGame::NotifyNewMail()
{
	if( !m_pMailList->IsVisible() )
	{
		CTMainUI* pMain = static_cast<CTMainUI*>( m_vTFRAME[TFRAME_MAIN] );
		pMain->SetNewMail(TRUE);
	}
}

void CTClientGame::TakeUpPet(DWORD dwCharID, DWORD dwMonID)
{
	CTClientChar* pChar;
	if( dwCharID == m_pMainChar->m_dwID )
		pChar = m_pMainChar;
	else
		pChar = FindPlayer(dwCharID);

	CTClientPet* pPet = static_cast<CTClientPet*>( FindRecall(dwMonID) );
	if( pChar && pPet && !pChar->IsDead() )
	{
		pChar->m_bMode = MT_NORMAL;
		pPet->SetPosition( pChar->GetPositionMat() );
		pPet->m_vWorld = pChar->GetPositionMat();
		pPet->TakeUp( m_pDevice, m_pRES, pChar, ID_PIVOT_MOUNT);
		pPet->m_fActTime = 0.0f;

		if( m_pTARGET == pPet )
		{
			ResetTargetOBJ(pChar);
			ResetTargetINFO(TRUE);
		}
	}
}

BOOL CTClientGame::IsInvenFrame(BYTE bInven)
{
	if( bInven == TFRAME_DEFINVEN ||
		bInven == TFRAME_INVEN_1 ||
		bInven == TFRAME_INVEN_2 ||
		bInven == TFRAME_INVEN_3 ||
		bInven == TFRAME_INVEN_4 ||
		bInven == TFRAME_INVEN_5 )
	{
		return TRUE;
	}

	return FALSE;
}	

void CTClientGame::EnableAllSubInven(BOOL bEnable)
{
	static const BYTE bFrameID[] = {
		TFRAME_INVEN_1,
		TFRAME_INVEN_2,
		TFRAME_INVEN_3,
		TFRAME_INVEN_4,
		TFRAME_INVEN_5 };

	for( INT i=0; i<MAX_SUBINVEN; ++i)
	{
		BYTE bInven = GetInvenID(bFrameID[i]);
		if( m_pMainChar->m_mapTINVEN.find(bInven) == m_pMainChar->m_mapTINVEN.end() )
			continue;

		if( bEnable )
			EnableUI(bFrameID[i]);
		else
			DisableUI(bFrameID[i]);
	}
}

BYTE CTClientGame::CheckDEFMSG( CTClientObjBase *pTATTACK,
							    CTClientObjBase *pTDEFEND,
								BOOL bBroadCast )
{
	if(!pTDEFEND)
		return FALSE;

	if( pTATTACK && (pTATTACK->m_bType == OT_RECALL || pTATTACK->m_bType == OT_SELF) )
		pTATTACK = FindPC(pTATTACK->GetHostID());

	if( pTDEFEND->m_bType == OT_RECALL || pTDEFEND->m_bType == OT_SELF )
		pTDEFEND = FindPC(pTDEFEND->GetHostID());

	if(!pTDEFEND)
		return pTATTACK == m_pMainChar ? TRUE : FALSE;

	if( !bBroadCast )
		return TRUE;

	return (pTATTACK == m_pMainChar && pTDEFEND->m_bType == OT_MON) || pTDEFEND == m_pMainChar ? TRUE : FALSE;
}

WORD CTClientGame::GetPartyID( CTClientObjBase *pTOBJ)
{
	if(!pTOBJ)
		return 0;

	if( pTOBJ->m_bType == OT_RECALL || pTOBJ->m_bType == OT_SELF )
		pTOBJ = FindPC(pTOBJ->GetHostID());

	return pTOBJ && pTOBJ->m_bType == OT_PC ? ((CTClientChar *) pTOBJ)->m_wPartyID : 0;
}

BYTE CTClientGame::IsInBattle(CTClientObjBase *pTATTACK, CTClientObjBase *pTDEFEND)
{
	if( !pTATTACK || !pTDEFEND )
		return FALSE;

	if( pTDEFEND->m_bType == OT_RECALL || pTDEFEND->m_bType == OT_SELF )
		pTDEFEND = FindPC(pTDEFEND->GetHostID());

	if( pTATTACK->m_bType == OT_RECALL || pTATTACK->m_bType == OT_SELF )
		pTATTACK = FindPC(pTATTACK->GetHostID());

	if( m_bDuel && 
		(m_pDuelTarget == pTATTACK && m_pMainChar == pTDEFEND) || 
		(m_pDuelTarget == pTDEFEND && m_pMainChar == pTATTACK) )
	{
		return TRUE;
	}

	if( m_bTournamentBattle &&
		(pTATTACK == m_pMainChar || pTDEFEND == m_pMainChar) &&
		pTATTACK->m_bTournamentTeam != TTOURNAMENT_TEAM_NONE &&
		pTDEFEND->m_bTournamentTeam != TTOURNAMENT_TEAM_NONE &&
		pTATTACK->m_bTournamentTeam != pTDEFEND->m_bTournamentTeam )
		return TRUE;

	return !pTDEFEND || pTDEFEND->IsInBattle() ? TRUE : FALSE;
}

BYTE CTClientGame::IsCtrlRECALL( CTClientRecall *pTOBJ)
{
	if( pTOBJ->m_bRecallType == TRECALLTYPE_SPY || (
		pTOBJ->m_bRecallType == TRECALLTYPE_PET &&
		((CTClientPet *) pTOBJ)->GetTakeUpChar()) )
		return TRUE;

	return FALSE;
}

void CTClientGame::MoveCtrlHost( WORD wPITCH,
								 WORD wDIR,
								 BYTE bMouseDIR,
								 BYTE bKeyDIR)
{
	if( m_dwSYNCTick < SYNC_TICK &&
		m_dwPOSTick < TPOS_TICK )
	{
		m_dwSYNCTick = SYNC_TICK - TPOS_TICK;
		return;
	}

	m_dwSYNCTick = 0;
	m_dwPOSTick = 0;

	if( m_pCtrlHost != m_pMainChar )
	{
		SendCS_MONMOVE_REQ(
			m_pCtrlHost,
			m_pMainWnd->m_bChannel,
			m_vMAP.m_wMapID,
			m_pCtrlHost->GetPositionX(),
			m_pCtrlHost->GetPositionY(),
			m_pCtrlHost->GetPositionZ(),
			wPITCH,
			wDIR,
			bMouseDIR,
			bKeyDIR,
			m_pCtrlHost->m_bAction);

		D3DXVECTOR3 vPOS = m_pMainChar->GetPosition();

		SendCS_MOVE_REQ(
			m_vMAP.m_wMapID,
			vPOS.x,
			vPOS.y,
			vPOS.z,
			m_pMainChar->m_wPITCH,
			m_pMainChar->m_wDIR,
			m_pMainChar->m_bMouseDIR,
			m_pMainChar->m_bKeyDIR,
			m_pMainChar->m_bAction,
			m_pMainChar->m_bGhost);

		if( vPOS != m_pCtrlHost->m_vSyncLastPosition )
		{
			m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;
			m_pCtrlHost->m_vSyncLastPosition = vPOS;
		}
	}
	else
	{
		D3DXVECTOR3 vPOS = m_pMainChar->GetPosition();

		SendCS_MOVE_REQ(
			m_vMAP.m_wMapID,
			vPOS.x,
			vPOS.y,
			vPOS.z,
			wPITCH,
			wDIR,
			bMouseDIR,
			bKeyDIR,
			m_pMainChar->m_bAction,
			m_pMainChar->m_bGhost);

		if( vPOS != m_pCtrlHost->m_vSyncLastPosition )
		{
			m_dwLeftNoneInputLimitationTime = TLIMITATION_TIME;
			m_pCtrlHost->m_vSyncLastPosition = vPOS;
		}
	}
}

CTClientMoveObj *CTClientGame::GetCtrlOBJ()
{
	MAPRECALL::iterator itTRECALL;

	for( itTRECALL = m_mapSLAVERECALL.begin(); itTRECALL != m_mapSLAVERECALL.end(); itTRECALL++)
		if( (*itTRECALL).second->m_bRecallType == TRECALLTYPE_SPY )
			return (*itTRECALL).second;

	CTClientPet *pPet = GetMainPet();
	if( pPet && pPet->GetTakeUpChar() == m_pMainChar )
		return pPet;

	return m_pMainChar;
}

CTClientRecall *CTClientGame::GetMainRecall()
{
	MAPRECALL::iterator itTRECALL;

	if(!m_vTFRAME[TFRAME_GAUGE]->IsVisible())
		return NULL;

	for( itTRECALL = m_mapSLAVERECALL.begin(); itTRECALL != m_mapSLAVERECALL.end(); itTRECALL++)
		if( (*itTRECALL).second->m_bRecallType == TRECALLTYPE_MAIN )
			return (*itTRECALL).second;

	return NULL;
}

CTClientPet *CTClientGame::GetMainPet()
{
	MAPRECALL::iterator itTRECALL;

	for( itTRECALL = m_mapSLAVERECALL.begin(); itTRECALL != m_mapSLAVERECALL.end(); itTRECALL++)
		if( (*itTRECALL).second->m_bRecallType == TRECALLTYPE_PET )
			return static_cast<CTClientPet*>((*itTRECALL).second);

	return NULL;
}

BOOL CTClientGame::IsPetRecalled( WORD wPetID )
{
	MAPRECALL::iterator itTRECALL;

	for( itTRECALL = m_mapSLAVERECALL.begin(); itTRECALL != m_mapSLAVERECALL.end(); itTRECALL++)
		if( (*itTRECALL).second->m_bRecallType == TRECALLTYPE_PET &&
			static_cast<CTClientPet*>((*itTRECALL).second)->GetPetID() == wPetID )
			return TRUE;

	return FALSE;
}

void CTClientGame::SetRecallTarget( DWORD dwTargetID,
								    BYTE bTargetType,
									BYTE bSKILL)
{
	CTClientRecall *pTRECALL = GetMainRecall();

	if( pTRECALL && !pTRECALL->m_bFlash )
	{
		CTClientObjBase *pTARGET = FindOBJ(
			dwTargetID,
			bTargetType);

		if( pTARGET && CanDetect( pTRECALL, pTARGET) )
		{
			if( bSKILL < TMONSKILL_COUNT )
				pTRECALL->m_pTCURSKILL = pTRECALL->FindTSkill(pTRECALL->m_pTEMP->m_wTSKILL[bSKILL]);

			if( pTRECALL->m_pTCURSKILL && CanDefend( pTRECALL, pTARGET, pTRECALL->m_pTCURSKILL->m_pTSKILL) )
			{
				pTRECALL->m_bTargetType = bTargetType;
				pTRECALL->m_dwTargetID = dwTargetID;
				pTRECALL->m_bSubAI = TRECALLAI_COUNT;
			}
		}
	}
}

BYTE CTClientGame::IsHostingOBJ( DWORD dwID, BYTE bType)
{
	switch(bType)
	{
	case OT_RECALL	: return FindSlaveRecall(dwID) ? TRUE : FALSE;
	case OT_SELF	: return FindSlaveFixRecall(dwID) ? TRUE : FALSE;
	case OT_PC		: return dwID == m_pMainChar->m_dwID ? TRUE : FALSE;
	}

	return FALSE;
}

void CTClientGame::DeleteFixRecall( DWORD dwID, bool bDirectDelete )
{
	MAPRECALL::iterator finder = m_mapSLAVEFIXRECALL.find(dwID);

	if( finder != m_mapSLAVEFIXRECALL.end() )
		m_mapSLAVEFIXRECALL.erase(finder);

	finder = m_mapFIXRECALL.find(dwID);
	if( finder != m_mapFIXRECALL.end() )
	{
		if( GetPartyID(m_pMainChar) && IsRSCSEnemy((*finder).second) )
		{
			RelaySession()->SendCS_DELCORPSENEMY_REQ(
				m_pMainChar->m_dwID,
				(*finder).second->m_dwID,
				(*finder).second->m_bType);
		}

		DeleteDynimicOBJ((*finder).second);

		if( bDirectDelete )
			m_mapFIXRECALL.erase(finder);
	}
}

BYTE CTClientGame::DeleteRecall( DWORD dwID, bool bDeleteDirect )
{
	MAPRECALL::iterator finder = m_mapSLAVERECALL.find(dwID);
	BYTE bRESULT = FALSE;

	if( finder != m_mapSLAVERECALL.end() )
	{
		CTClientRecall* pTRECALL = (*finder).second;

		if( pTRECALL->m_bTPOS < TRECALL_MAX )
			m_pMainChar->m_vTRECALL[pTRECALL->m_bTPOS] = FALSE;

		m_mapSLAVERECALL.erase(finder);
		bRESULT = TRUE;

		if( pTRECALL->m_bRecallType == TRECALLTYPE_PET )
		{
			CTPetManageDlg* pPetDlg = static_cast<CTPetManageDlg*>(m_vTFRAME[TFRAME_PET_MANAGE]);
			INT nIdx = pPetDlg->FindPetByID( static_cast<CTClientPet*>(pTRECALL)->GetPetID() );
			if( nIdx != T_INVALID )
				pPetDlg->UpdatePetInfo(nIdx);

			if( m_nNextPetID && !pPetDlg->IsRecalling() )
			{
				m_pSESSION->SendCS_PETRECALL_REQ( (WORD) m_nNextPetID);
				pPetDlg->SetRecalling(TRUE);
				m_nNextPetID = 0;
			}
		}
		else if( pTRECALL->m_bRecallType == TRECALLTYPE_MAIN)
			m_pMainChar->m_bPrevMainRecallAI = pTRECALL->m_bAI;
	}

	finder = m_mapRECALL.find(dwID);
	if( finder != m_mapRECALL.end() )
	{
		if( GetPartyID(m_pMainChar) && IsRSCSEnemy((*finder).second) )
		{
			RelaySession()->SendCS_DELCORPSENEMY_REQ(
				m_pMainChar->m_dwID,
				(*finder).second->m_dwID,
				(*finder).second->m_bType);
		}

		DeleteDynimicOBJ((*finder).second);

		if( bDeleteDirect )
			m_mapRECALL.erase(finder);
	}

	return bRESULT;
}

void CTClientGame::SelectTRSCS( DWORD wKEY )
{
	WORD wSquadID = GetPartyID(m_pMainChar);
	BYTE bINDEX = 0;

	if( wKEY != TKEY_TRSCS_ME && !wSquadID )
		return;

	CTClientObjBase::m_mapTSELECTDEADOBJ.clear();
	CTClientObjBase::m_mapTSELECTOBJ.clear();

	switch(wKEY)
	{
	case TKEY_TRSCS_F1			: SelectParty(0); break;
	case TKEY_TRSCS_F2			: SelectParty(1); break;
	case TKEY_TRSCS_F3			: SelectParty(2); break;
	case TKEY_TRSCS_F4			: SelectParty(3); break;
	case TKEY_TRSCS_F5			: SelectParty(4); break;
	case TKEY_TRSCS_F6			: SelectParty(5); break;
	case TKEY_TRSCS_F7			: return;
	case TKEY_TRSCS_ME			: ResetTargetOBJ(m_pMainChar); break;
	default						: return;
	}

	ResetTargetINFO(FALSE);
}

BOOL CTClientGame::IsBusy()
{
	return	(m_vTOPTION.m_bDenyCommunity ||
			 m_pMainWnd->IsMsgBoxVisible() ||
			 m_pMainWnd->GetCurrentFrame() != this);
}

void CTClientGame::DoRecallSKILL( DWORD wKEY)
{
	CTClientRecall *pTRECALL = GetMainRecall();
	m_pCtrlHost = GetCtrlOBJ();

	if( pTRECALL && pTRECALL->m_pTEMP->m_dwOBJ && CanControl() )
	{
		CTClientObjBase *pTARGET = m_pTARGET ? m_pTARGET : pTRECALL->m_bTargetType != OT_NONE ? FindOBJ(
			pTRECALL->m_dwTargetID,
			pTRECALL->m_bTargetType) : NULL;

		if(pTARGET)
		{
			BYTE bSKILL = TMONSKILL_COUNT;

			switch(wKEY)
			{
			case TKEY_RECALL_SKILL_1	: bSKILL = 0; break;
			case TKEY_RECALL_SKILL_2	: bSKILL = 1; break;
			case TKEY_RECALL_SKILL_3	: bSKILL = 2; break;
			case TKEY_RECALL_SKILL_4	: bSKILL = 3; break;
			}

			if( bSKILL < TMONSKILL_COUNT && pTRECALL->FindTSkill(pTRECALL->m_pTEMP->m_wTSKILL[bSKILL]) )
				SetRecallTarget( pTARGET->m_dwID, pTARGET->m_bType, bSKILL);
		}
	}
}

void CTClientGame::DoRecallAI( DWORD wKEY)
{
	CTClientRecall *pTRECALL = GetMainRecall();
	m_pCtrlHost = GetCtrlOBJ();

	if( pTRECALL && pTRECALL->m_pTEMP->m_dwOBJ && CanControl() )
		switch(wKEY)
		{
		case TKEY_RECALL_STAY		: pTRECALL->m_bSubAI = TRECALLAI_STAY; break;
		case TKEY_RECALL_BACK		: pTRECALL->m_bSubAI = TRECALLAI_BACK; break;
		case TKEY_RECALL_PASSIVE	: pTRECALL->m_bAI = TRECALLAI_PASSIVE; break;
		case TKEY_RECALL_ACTIVE		: pTRECALL->m_bAI = TRECALLAI_ACTIVE; break;
		case TKEY_RECALL_MANUAL		: pTRECALL->m_bAI = TRECALLAI_MANUAL; break;
		}
}

void CTClientGame::BuySKILL( BYTE bSlotID)
{
	LPTSKILL pTSKILL = CTChart::FindTSKILLTEMP( CTChart::GetTSKILLID(
		m_pMainChar->m_bContryID,
		m_pMainChar->m_bClassID,
		((CTSkillDlg *) m_vTFRAME[TFRAME_SKILL])->GetCurSel(),
		bSlotID));

	if( m_pSESSION && pTSKILL )
	{
		CTSkillDlg* pSKILLDLG = (CTSkillDlg*) GetFrame( TFRAME_SKILL );
		if( !pSKILLDLG->m_pOpenByCash )
		{
			m_pSESSION->SendCS_SKILLBUY_REQ(
				TDEF_SKILL_NPC,
				pTSKILL->m_wSkillID );

			m_pSESSION->SendCS_NPCITEMLIST_REQ(
				TDEF_SKILL_NPC );
		}
		else
		{
			/*m_pSESSION->SendCS_SKILLBUY_REQ(
				pSKILLDLG->m_pOpenByCash->m_wNPCID,
				pTSKILL->m_wSkillID,
				pSKILLDLG->m_pOpenByCash->m_bInvenID,
				pSKILLDLG->m_pOpenByCash->m_bSlotID );

			m_pSESSION->SendCS_NPCITEMLIST_REQ(
				pSKILLDLG->m_pOpenByCash->m_wNPCID);*/
		}
	}
}

void CTClientGame::EnableSkillTimer( CTClientObjBase *pATTACK,
									 CTClientSkill *pTSkill,
									 BOOL bEnable)
{
	MAPTSKILL::iterator itTSKILL;

	for( itTSKILL = pATTACK->m_mapTSKILL.begin(); itTSKILL != pATTACK->m_mapTSKILL.end(); itTSKILL++)
	{
		CTClientSkill *pTSKILL = (*itTSKILL).second;

		if( pTSKILL->m_pTSKILL->m_bKind == pTSkill->m_pTSKILL->m_bKind &&
			pTSKILL->m_pTSKILL->m_bActive &&
			pTSKILL->m_bTimerON != bEnable && (
			bEnable || !pTSKILL->m_dwTick) )
			pTSKILL->m_bTimerON = bEnable;
	}
}

void CTClientGame::ApplySkillTick( CTClientObjBase *pATTACK,
								   CTClientSkill *pTSkill)
{
	MAPTSKILL::iterator itTSKILL;

	pTSkill->m_dwReuseTick = pTSkill->GetReuseTick(m_pMainChar);
	pTSkill->m_dwTick = pTSkill->m_dwReuseTick;

	if( !pTSkill->m_dwTick && !pTSkill->m_pTSKILL->m_dwGroupTick )
	{
		EnableSkillTimer(
			pATTACK,
			pTSkill,
			FALSE);

		return;
	}

	for( itTSKILL = pATTACK->m_mapTSKILL.begin(); itTSKILL != pATTACK->m_mapTSKILL.end(); itTSKILL++)
	{
		CTClientSkill *pTSKILL = (*itTSKILL).second;

		if( pTSKILL->m_pTSKILL->m_bKind == pTSkill->m_pTSKILL->m_bKind &&
			pTSKILL->m_pTSKILL->m_bActive && pTSKILL->m_bTimerON )
			if(pTSKILL->m_dwTick)
			{
				pTSKILL->m_dwTick = max( pTSKILL->m_dwTick, pTSkill->m_pTSKILL->m_dwGroupTick);
				pTSKILL->m_dwReuseTick = max( pTSKILL->m_dwReuseTick, pTSKILL->m_dwTick);
			}
			else if(pTSkill->m_pTSKILL->m_dwGroupTick)
			{
				pTSKILL->m_dwReuseTick = pTSkill->m_pTSKILL->m_dwGroupTick;
				pTSKILL->m_dwTick = pTSKILL->m_dwReuseTick;
			}
			else
				pTSKILL->m_bTimerON = FALSE;
	}
}

D3DXVECTOR3 CTClientGame::GetWarpTARGET( CTClientObjBase *pTOBJ,
										 LPD3DXVECTOR3 pDIR,
										 FLOAT fMOVE)
{
	D3DXVECTOR3 vTARGET = pTOBJ->GetPosition();
	FLOAT fLength = D3DXVec3Length(pDIR);

	if( fLength != 0.0f )
	{
		D3DXVECTOR3 vTSTART = vTARGET;

		(*pDIR) /= fLength;
		fLength = 0.0f;

		vTARGET += (fMOVE + pTOBJ->m_fRadius) * (*pDIR);
		vTARGET.y += TTRANS_HEIGHT;
		vTSTART.y += TTRANS_HEIGHT;

		if(m_vMAP.HitTest( &fLength, &vTSTART, &vTARGET, 0.0f, 0.0f, FALSE))
		{
			fLength -= TTRANS_BOUND + pTOBJ->m_fRadius;
			fLength = max( 0.0f, fLength);
			fLength = min( fLength, fMOVE);

			vTARGET = vTSTART + fLength * (*pDIR);
		}
		else
			vTARGET = vTSTART + fMOVE * (*pDIR);

		vTARGET.y -= TTRANS_HEIGHT;
		vTSTART = vTARGET;
		vTSTART.y += pTOBJ->m_fSizeY;

		fLength = m_vMAP.GetHeight(
			pTOBJ,
			NULL,
			&vTSTART,
			0.0f,
			FALSE);

		if( fLength > vTARGET.y - pTOBJ->m_fSizeY )
			vTARGET.y = fLength;
	}

	return vTARGET;
}

BYTE CTClientGame::CanDetect( CTClientObjBase *pSEEKER,
							  CTClientObjBase *pTARGET)
{
	if( !pTARGET->m_bSelectable )
		return FALSE;

	if( !CanCharacterSee(pSEEKER, pTARGET) &&
		!(pSEEKER->m_wTDETECT & (1 << pTARGET->m_bType)) )
		return FALSE;

	D3DXVECTOR3 vSTART(
		pSEEKER->GetPositionX(),
		pSEEKER->GetPositionY() + pSEEKER->m_fSizeY / 2.0f,
		pSEEKER->GetPositionZ());

	D3DXVECTOR3 vTARGET(
		pTARGET->GetPositionX(),
		pTARGET->GetPositionY() + pTARGET->m_fSizeY / 2.0f,
		pTARGET->GetPositionZ());

	FLOAT fRange = D3DXVec2Length(&D3DXVECTOR2(
		vTARGET.x - vSTART.x,
		vTARGET.z - vSTART.z));

	if( fRange > 0.01f )
	{
		VTOBJBASE vTCOLLISION;

		vTCOLLISION.clear();
		m_vMAP.GetLineCollisionOBJ(
			&vTCOLLISION,
			&vSTART,
			&vTARGET,
			0.0f);

		int nCount = INT(vTCOLLISION.size());
		for( int i=0; i<nCount; i++)
		{
			CTClientObjBase *pTCOLLOBJ = vTCOLLISION[i];
			D3DXVECTOR2 vDIR(
				pTCOLLOBJ->GetPositionX() - vTARGET.x,
				pTCOLLOBJ->GetPositionZ() - vTARGET.z);

			if( !pTCOLLOBJ->m_bGARBAGE &&
				pTCOLLOBJ->m_bType != OT_PATHWALL &&
				D3DXVec2Length(&vDIR) > 0.01f &&
				pTCOLLOBJ->HitTest(
				&vTARGET,
				&vSTART,
				0.0f, 0.0f) )
			{
				vTCOLLISION.clear();
				return FALSE;
			}
		}

		vTCOLLISION.clear();
	}

	return TRUE;
}

BYTE CTClientGame::CanCharacterSee( CTClientObjBase *pSEEKER, 
								    CTClientObjBase *pTARGET)
{
	return pTARGET->m_bHide && !pSEEKER->IsAlliance(pTARGET) && (
		pSEEKER != m_pMainChar || !pTARGET->m_bDetect) ? FALSE : TRUE;
}

BYTE CTClientGame::CanAttackCountry( CTClientMonster *pMONSTER,
								     CTClientObjBase *pTARGET)
{
	if(pTARGET->IsDisguise())
		return pTARGET->m_bDisguise != pMONSTER->m_bContryID ? TRUE : FALSE;

	return pTARGET->m_bContryID != pMONSTER->m_bContryID ? TRUE : FALSE;
}

BYTE CTClientGame::CanFollow( CTClientObjBase *pTARGET)
{
	return pTARGET && !m_pMainChar->IsDead() &&
		!m_pMainChar->IsBlock() &&
		!m_pMainChar->IsDown() &&
		pTARGET->m_bType == OT_PC &&
		pTARGET->IsAlliance(m_pMainChar) ? TRUE : FALSE;
}

BYTE CTClientGame::CanFollow( CTClientMonster *pTSEEKER,
							  CTClientObjBase *pTARGET)
{
	return !pTSEEKER || !pTARGET || pTARGET->IsDead() ||
		!pTARGET->m_bSelectable || pTARGET->m_bGhost ||
		!CanAttackCountry( pTSEEKER, pTARGET) || (
		pTARGET->m_bHide &&
		!(pTSEEKER->m_wTDETECT & (1 << pTARGET->m_bType))) || (
		pTARGET->CanFLY() &&
		pTSEEKER->GetAB(pTARGET) < fabs(pTSEEKER->GetPositionY() - pTARGET->GetPositionY())) ? FALSE : TRUE;
}

BYTE CTClientGame::CanUseMail()
{
	return (m_pMainChar->m_bInPCROOM & PCBANG_PREMIUM1) == PCBANG_PREMIUM1 ||
		(m_pMainChar->m_pTREGION && m_pMainChar->m_pTREGION->m_bCanMail) ? TRUE : FALSE;
}

void CTClientGame::CheckTPORTAL( CTClientObjBase *pTOBJ,
								 LPD3DXVECTOR3 pPREV,
								 LPD3DXVECTOR3 pNEXT,
								 FLOAT fNextH)
{
	MAPOBJECT::iterator itTPORTAL;

	if( m_bTELEPORT ||
		m_pMainChar->m_bGhost ||
		m_pMainChar->IsDead() ||
		m_pMainChar->IsDown() ||
		m_pMainChar->IsPush() ||
		m_pMainChar->IsBlock() || (
		pTOBJ->m_bType == OT_RECALL && (
		((CTClientRecall *) pTOBJ)->m_bRecallType != TRECALLTYPE_PET ||
		((CTClientPet *) pTOBJ)->GetTakeUpChar() != m_pMainChar)))
		return;

	D3DXVECTOR3 vTSTART = (*pPREV);
	D3DXVECTOR3 vTEND = (*pNEXT);

	FLOAT fDIST = 0.0f;
	vTEND.y = fNextH;

	for( itTPORTAL = m_vMAP.m_mapTPortal.begin(); itTPORTAL != m_vMAP.m_mapTPortal.end(); itTPORTAL++)
		if(pTOBJ->CheckCollision( (CTClientObjBase *) (*itTPORTAL).second, &vTSTART, &vTEND, pTOBJ->m_fSizeY, &fDIST))
		{
			m_pSESSION->SendCS_TELEPORT_REQ(WORD(((CTClientNpc *) (*itTPORTAL).second)->m_dwMaxHP));
			m_bTELEPORT = TRUE;

			break;
		}
}

void CTClientGame::BeginGHOSTScene()
{
	m_pDevice->m_pDevice->SetRenderTarget( 0, m_pDevice->m_pBACKBUF);
	m_pDevice->m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER,
		D3DCOLOR_ARGB( 0, 0, 0, 0),
		1.0f, 0);
}

void CTClientGame::EndGHOSTScene()
{
	DWORD dwALPHA = DWORD((DWORD(m_dwGHOSTTick * ALPHA_MAX / TGHOST_TICK) << 24) & 0xFF000000);

	CTClientCAM vCAMERA;
	D3DXMATRIX vWorld;

	m_pDevice->m_pDevice->SetRenderTarget( 0, m_pDevice->m_pRTARGET);
	m_pDevice->m_pDevice->Clear(
		0, NULL,
		D3DCLEAR_TARGET,
		D3DCOLOR_ARGB( 0xFF, 0x00, 0x00, 0x00),
		1.0f, 0);
	D3DXMatrixIdentity(&vWorld);

	vCAMERA.InitOrthoCamera(
		m_pDevice->m_pDevice,
		-1.0f,
		1.0f,
		1.0f,
		1.0f);

	vCAMERA.SetPosition(
		D3DXVECTOR3( 0.0f, 0.5f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
		D3DXVECTOR3( 0.0f, 0.0f, 1.0f),
		FALSE);
	vCAMERA.Activate(TRUE);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
	m_pDevice->m_pDevice->SetTextureStageState( 1, D3DTSS_COLOROP, D3DTOP_DISABLE);

	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pDevice->m_pDevice->SetSamplerState( 0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0x004C4C4C);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHATESTENABLE, FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_FOGENABLE, FALSE);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_ZWRITEENABLE, FALSE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ZENABLE, FALSE);

	m_pDevice->m_pDevice->SetStreamSource( 0, m_pDevice->m_pBACKVB, 0, sizeof(PVERTEX));
	m_pDevice->m_pDevice->SetTexture( 0, m_pDevice->m_pBACKTEX);

	m_pDevice->m_pDevice->SetTransform( D3DTS_WORLD, &vWorld);
	m_pDevice->m_pDevice->SetFVF(T3DFVF_PVERTEX);
	m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_SUBTRACT);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ONE);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE);
	m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);

	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	m_pDevice->m_pDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TFACTOR);

	m_pDevice->m_pDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_BLENDOP, D3DBLENDOP_ADD);
	m_pDevice->m_pDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwALPHA);

	m_pDevice->m_pDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2);
	m_pCamera->Activate(TRUE);
}

BYTE CTClientGame::CheckGHOSTScene()
{
	return m_pMainChar->IsDead() || m_pMainChar->m_bGhost || m_dwGHOSTTick < TGHOST_TICK ? TRUE : FALSE;
}

void CTClientGame::CalcGHOSTScene( DWORD dwTick)
{
	BYTE bGHOST = m_pMainChar->IsDead() || m_pMainChar->m_bGhost ? TRUE : FALSE;

	if( bGHOST != m_bGHOST )
	{
		m_dwGHOSTTick = bGHOST ? TGHOST_TICK : 0;
		m_bGHOST = bGHOST;
	}

	if(m_bGHOST)
		m_dwGHOSTTick -= min( dwTick, m_dwGHOSTTick);
	else if( m_dwGHOSTTick < TGHOST_TICK )
		m_dwGHOSTTick = min( TGHOST_TICK, m_dwGHOSTTick + dwTick);
}

void CTClientGame::CalcFLASHTick( DWORD dwTick)
{
	if(m_pMainChar->m_bFlash)
		m_dwFLASHTick = min( m_dwFLASHTick + dwTick, TFLASH_TICK);
	else
		m_dwFLASHTick -= min( m_dwFLASHTick, dwTick);
}

void CTClientGame::CalcDynamicHelp( DWORD dwTick)
{
	if( !m_vTOPTION.m_bAutoHelp && m_pNotifyFrame->IsVisible() )
		m_pNotifyFrame->ShowComponent(FALSE);
	else if( m_vTOPTION.m_bAutoHelp && !m_pNotifyFrame->IsVisible() )
		m_pNotifyFrame->ShowComponent(TRUE);
}

void CTClientGame::CalcBGM( DWORD dwTick)
{
	static DWORD dwBGMTick = 0;
	static DWORD dwENVTick = 0;

	if(m_bStopBGM)
	{
		CT3DMusic *pTBGM = (CT3DMusic *) CTachyonRes::m_MEDIA.GetData(
			MEDIA_TMUSIC,
			m_dwBGM);

		CD3DSound *pTENV = (CD3DSound *) CTachyonRes::m_MEDIA.GetData(
			MEDIA_TSOUND,
			m_dwENV);

		m_dwActiveVolume -= min( m_dwActiveVolume, dwTick);
		if(m_dwActiveVolume)
		{
			BYTE bFadeVolume = BYTE(VOLUME_MAX * m_dwActiveVolume / TBGM_VOLUME);

			if(pTBGM)
			{
				pTBGM->m_bFadeVolume = bFadeVolume;
				pTBGM->ResetVolume();
			}

			if(pTENV)
			{
				pTENV->m_bFadeVolume = bFadeVolume;
				pTENV->ResetVolume();
			}
		}
		else
		{
			if(pTBGM)
				pTBGM->m_bFadeVolume = VOLUME_MAX;

			if(pTENV)
				pTENV->m_bFadeVolume = VOLUME_MAX;

			CTachyonRes::m_MEDIA.Stop(
				MEDIA_TMUSIC,
				m_dwBGM, 0);

			CTachyonRes::m_MEDIA.Stop(
				MEDIA_TSOUND,
				m_dwENV, 0);
			m_bStopBGM = FALSE;
		}
	}
	else if(m_pMainChar->m_pTREGION)
	{
		m_dwActiveVolume = TBGM_VOLUME;
		dwENVTick += dwTick;

		if( dwENVTick > TBGM_TIMER )
		{
			if( m_pMainChar->m_pTREGION->m_pVTENV &&
				!m_pMainChar->m_pTREGION->m_pVTENV->empty() &&
				!(rand() % TBGM_RATE) )
			{
				VECTORDWORD& vTENV = *(m_pMainChar->m_pTREGION->m_pVTENV);
				DWORD dwENV = vTENV[rand() % INT(vTENV.size())];

				CD3DSound *pWAV = (CD3DSound *) CTachyonRes::m_MEDIA.GetData(
					MEDIA_TSOUND,
					dwENV);

				if(pWAV)
				{
					int nIndex = pWAV->Lock();

					if( nIndex >= 0 )
					{
						LPDIRECTSOUND3DBUFFER p3DBUF = pWAV->GetDS3D(nIndex);

						if(p3DBUF)
						{
							D3DXVECTOR3 vSNDPOS = CD3DSound::ConvertPOS(
								m_pCamera->m_vPosition.x + FLOAT(rand() % (20 * INT(TENVSND_RANGE))) / 10.0f - TENVSND_RANGE,
								m_pCamera->m_vPosition.y,
								m_pCamera->m_vPosition.z + FLOAT(rand() % (20 * INT(TENVSND_RANGE))) / 10.0f - TENVSND_RANGE);

							p3DBUF->SetPosition(
								vSNDPOS.x,
								vSNDPOS.y,
								vSNDPOS.z,
								DS3D_IMMEDIATE);
						}

						pWAV->Play(nIndex);
						pWAV->Unlock(nIndex);
					}
				}
			}

			dwENVTick = 0;
		}

		if(!CTachyonRes::m_MEDIA.IsPlay( MEDIA_TMUSIC, m_dwBGM, 0))
		{
			dwBGMTick += dwTick;

			if( dwBGMTick > TBGM_TIMER )
			{
				if( m_pMainChar->m_pTREGION->m_pVTBGM &&
					!m_pMainChar->m_pTREGION->m_pVTBGM->empty() &&
					!(rand() % TBGM_RATE) )
				{
					CTachyonRes::m_MEDIA.Stop(
						MEDIA_TMUSIC,
						m_dwBGM, 0);

					VECTORDWORD& vTBGM = *(m_pMainChar->m_pTREGION->m_pVTBGM);

					m_dwBGM = vTBGM[rand() % INT(vTBGM.size())];
					m_bStopBGM = FALSE;

					CTachyonRes::m_MEDIA.Play(
						MEDIA_TMUSIC,
						m_dwBGM);
				}

				dwBGMTick = 0;
			}
		}

		if( !m_pMainChar->m_pTREGION->m_dwTENVSND_LOOP &&
			!CTachyonRes::m_MEDIA.IsPlay( MEDIA_TSOUND, m_dwENV, 0))
		{
			CTachyonRes::m_MEDIA.Stop(
				MEDIA_TSOUND,
				m_dwENV, 0);
			m_dwENV = m_pMainChar->m_pTREGION->m_dwTENVSND_LOOP;

			CTachyonRes::m_MEDIA.Play(
				MEDIA_TSOUND,
				m_dwENV, 0);
		}
	}
}

BOOL CTClientGame::CheckDuelRange()
{
	static const FLOAT fDuelRange2 = DUEL_AREARANGE*DUEL_AREARANGE;

	if( !m_bDuel || !m_pDuelTarget )
		return TRUE;

	D3DXVECTOR2 vPOS( m_pMainChar->GetPositionX(), m_pMainChar->GetPositionZ());
	vPOS -= m_vDuelCenter;

	return D3DXVec2LengthSq(&vPOS) < fDuelRange2 ? TRUE : FALSE;
}

void CTClientGame::CalcDUEL(DWORD dwTick)
{
	if( m_dwDuelWaitTick )
	{
		if( m_dwDuelWaitTick > dwTick )
		{
			m_dwDuelWaitTick -= dwTick;

			DWORD m_dwDuelWaitSec = m_dwDuelWaitTick/1000;
			if( m_dwDuelWaitSec && m_dwDuelWaitSec != (m_dwDuelWaitTick+dwTick)/1000 )
			{
				CString strMSG;
				strMSG = CTChart::Format( TSTR_DUEL_WAIT, m_dwDuelWaitSec);

				ShowInfoChat(strMSG, TUISND_TYPE_INFO);
			}
		}
		else
			m_dwDuelWaitTick = 0;
	}
	
	if( !m_bDuel )
		return;

	BOOL bInRange = CheckDuelRange();

	if( m_dwDuelOutTick == (DWORD)T_INVALID )
	{
		if( !bInRange )
			m_dwDuelOutTick = DUEL_AREAOUT_TIME*1000;
	}
	else
	{
		if( bInRange )
			m_dwDuelOutTick = (DWORD)T_INVALID;
		else if( m_dwDuelOutTick > dwTick )
		{
			m_dwDuelOutTick -= dwTick;

			DWORD m_dwDuelOutSec = m_dwDuelOutTick/1000;
			if( m_dwDuelOutSec && m_dwDuelOutSec != (m_dwDuelOutTick+dwTick)/1000 )
			{
				CString strMSG;
				strMSG = CTChart::Format( TSTR_DUEL_AREAOUT, m_dwDuelOutSec);

				ShowInfoChat(strMSG, TUISND_TYPE_INFO);
			}
		}
		else
		{
			m_bDuel = FALSE;
			m_pSESSION->SendCS_DUELEND_REQ();
		}
	}

	if( m_dwDuelTick > dwTick )
	{
		m_dwDuelTick -= dwTick;

		DWORD m_dwDuelMin = m_dwDuelTick/60000;
		if( m_dwDuelMin )
		{
			if( m_dwDuelMin != (m_dwDuelTick+dwTick)/60000 )
			{
				CString strMSG;
				strMSG = CTChart::Format( TSTR_DUEL_TIMEOUT_MIN, m_dwDuelMin);

				ShowInfoChat(strMSG, TUISND_TYPE_INFO);
			}
		}
		else
		{
			DWORD m_dwDuelSec = m_dwDuelTick/1000;
			if( m_dwDuelSec && m_dwDuelSec<=10 && m_dwDuelSec != (m_dwDuelTick+dwTick)/1000 )
			{
				CString strMSG;
				strMSG = CTChart::Format( TSTR_DUEL_TIMEOUT_SEC, m_dwDuelSec);

				ShowInfoChat(strMSG, TUISND_TYPE_INFO);
			}
		}
	}
	else
		m_dwDuelTick = 0;
}

void CTClientGame::CalcGlobalTEX( DWORD dwTick)
{
	if(CTClientObjBase::m_pTSELECT)
	{
		m_dwTSELECT += dwTick;
		m_dwTSELECT = CTClientObjBase::m_pTSELECT->m_dwTotalTick ?
			m_dwTSELECT % CTClientObjBase::m_pTSELECT->m_dwTotalTick : 0;
	}

	if(CTClientObjBase::m_pTSHADOW)
	{
		m_dwTSHADOW += dwTick;
		m_dwTSHADOW = CTClientObjBase::m_pTSHADOW->m_dwTotalTick ?
			m_dwTSHADOW % CTClientObjBase::m_pTSHADOW->m_dwTotalTick : 0;
	}

	m_bDrawGROUND = FALSE;
	if(m_bGroundATK)
		ResetGroundPOS( m_vKEY.m_point.x, m_vKEY.m_point.y);
	m_bDrawGROUND = CheckGroundDRAW();

	if( m_bDrawGROUND && m_pTGROUNDTEX )
	{
		m_dwTGROUND += dwTick;
		m_dwTGROUND = m_pTGROUNDTEX->m_dwTotalTick ?
			m_dwTGROUND % m_pTGROUNDTEX->m_dwTotalTick : 0;
	}
}

void CTClientGame::CalcTQuestTick( DWORD dwTick)
{
	CTNPCTalkDlg *pTNPCTalkDlg = (CTNPCTalkDlg *) m_vTFRAME[TFRAME_NPCDIALOG];
	CTNewQuestDlg *pTQuestDlg = (CTNewQuestDlg *) m_vTFRAME[TFRAME_QUEST];
	MAPTQCLASS::iterator itTQCLASS;

	for( itTQCLASS = m_pMainChar->m_mapTQCLASS.begin(); itTQCLASS != m_pMainChar->m_mapTQCLASS.end(); itTQCLASS++)
	{
		MAPTQUEST::iterator itTQUEST;

		for( itTQUEST = (*itTQCLASS).second->m_mapTQUEST.begin(); itTQUEST != (*itTQCLASS).second->m_mapTQUEST.end(); itTQUEST++)
			if((*itTQUEST).second->m_bTimmer)
				(*itTQUEST).second->m_dwTick -= min( (*itTQUEST).second->m_dwTick, dwTick);
	}

	int nTOP = pTQuestDlg->GetTop();
	int nCount = pTQuestDlg->GetCount() - nTOP;

	for( int i=0; i<nCount; i++)
	{
		LPTQUEST pTQUEST = pTQuestDlg->GetTQUEST(nTOP+i);
		
		if( pTQUEST )
			pTQuestDlg->SetQuestState(nTOP+i, pTQUEST);
	}

	if( pTNPCTalkDlg->IsVisible() && m_pTARGET )
	{
		int nTOP = pTNPCTalkDlg->GetTop();
		int nCount = pTNPCTalkDlg->GetCount() - nTOP;

		for( i=0; i<nCount; i++)
		{
			LPTQUEST pTQUEST = pTNPCTalkDlg->GetTQUEST(nTOP+i);

			if( pTQUEST && pTQUEST->m_bType == QT_COMPLETE )
			{
				pTQUEST = CTChart::FindTMISSION(pTQUEST);
				if( pTQUEST )
					pTNPCTalkDlg->SetQuestState(nTOP+i, pTQUEST);
			}
		}
	}
}

void CTClientGame::CalcMaintainOBJ( DWORD dwTick)
{
	static DWORD dwMaintainTick = 0;
	dwMaintainTick += dwTick;

	if( dwMaintainTick > TMCHECK_TICK )
	{
		BuildMaintainList();
		CheckMaintainOBJ();

		dwMaintainTick = 0;
	}
}

void CTClientGame::CalcTSKY( FLOAT fWaterHeight,
							 DWORD dwTick)
{
	m_vFARIMAGE[m_bTSKY].m_vWorld._41 = m_pCtrlHost->GetPositionX();
	m_vFARIMAGE[m_bTSKY].m_vWorld._42 = max( fWaterHeight, m_pCtrlHost->GetPositionY());
	m_vFARIMAGE[m_bTSKY].m_vWorld._43 = m_pCtrlHost->GetPositionZ();

	m_vSKY[m_bTSKY].m_vWorld._41 = m_vFARIMAGE[m_bTSKY].m_vWorld._41;
	m_vSKY[m_bTSKY].m_vWorld._42 = m_vFARIMAGE[m_bTSKY].m_vWorld._42;
	m_vSKY[m_bTSKY].m_vWorld._43 = m_vFARIMAGE[m_bTSKY].m_vWorld._43;

	m_vFARIMAGE[m_bTSKY].CalcTick( m_pDevice->m_pDevice, dwTick);
	m_vSKY[m_bTSKY].CalcTick( m_pDevice->m_pDevice, dwTick);

	if(!m_vSKY[!m_bTSKY].m_bHide)
	{
		m_vFARIMAGE[!m_bTSKY].m_vWorld._41 = m_vFARIMAGE[m_bTSKY].m_vWorld._41;
		m_vFARIMAGE[!m_bTSKY].m_vWorld._42 = m_vFARIMAGE[m_bTSKY].m_vWorld._42;
		m_vFARIMAGE[!m_bTSKY].m_vWorld._43 = m_vFARIMAGE[m_bTSKY].m_vWorld._43;

		m_vSKY[!m_bTSKY].m_vWorld._41 = m_vSKY[m_bTSKY].m_vWorld._41;
		m_vSKY[!m_bTSKY].m_vWorld._42 = m_vSKY[m_bTSKY].m_vWorld._42;
		m_vSKY[!m_bTSKY].m_vWorld._43 = m_vSKY[m_bTSKY].m_vWorld._43;

		m_vFARIMAGE[!m_bTSKY].CalcTick( m_pDevice->m_pDevice, dwTick);
		m_vSKY[!m_bTSKY].CalcTick( m_pDevice->m_pDevice, dwTick);
	}

	FLOAT fUnitLength = FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);
	BYTE bUnitX = BYTE(m_pCtrlHost->GetPositionX() / fUnitLength);
	BYTE bUnitZ = BYTE(m_pCtrlHost->GetPositionZ() / fUnitLength);
	WORD wTSKY = MAKEWORD( bUnitX, bUnitZ);

	if( wTSKY != m_wTSKY )
		if(m_vSKY[!m_bTSKY].m_bHide)
		{
			LPTSKYBOX pTFAR = CTChart::FindTSKYBOX(
				m_vMAP.m_pMAP->m_nUnitCountX,
				TSKYOBJ_FAR,
				m_vMAP.m_wMapID,
				bUnitX,
				bUnitZ);

			LPTSKYBOX pTSKY = CTChart::FindTSKYBOX(
				m_vMAP.m_pMAP->m_nUnitCountX,
				TSKYOBJ_SKY,
				m_vMAP.m_wMapID,
				bUnitX,
				bUnitZ);

			m_vFARIMAGE[!m_bTSKY].Release();
			m_vSKY[!m_bTSKY].Release();

			if( pTFAR && pTSKY )
			{
				m_vFARIMAGE[!m_bTSKY].InitOBJ(m_pRES->GetOBJ(pTFAR->m_dwOBJ));
				m_vFARIMAGE[!m_bTSKY].m_vScale._11 = TSKY_SCALE;
				m_vFARIMAGE[!m_bTSKY].m_vScale._22 = TSKY_SCALE;
				m_vFARIMAGE[!m_bTSKY].m_vScale._33 = TSKY_SCALE;
				m_vFARIMAGE[!m_bTSKY].SetCloth(
					m_pDevice,
					pTFAR->m_dwCLK,
					pTFAR->m_dwCL,
					pTFAR->m_dwMESH);
				m_vFARIMAGE[!m_bTSKY].ResetOBJPart(m_pDevice);

				m_vFARIMAGE[!m_bTSKY].SetAction(
					pTFAR->m_dwACT,
					pTFAR->m_dwANI);

				m_vFARIMAGE[!m_bTSKY].m_bHide = FALSE;
				m_vFARIMAGE[!m_bTSKY].m_bAlpha = 0;

				m_vSKY[!m_bTSKY].InitOBJ(m_pRES->GetOBJ(pTSKY->m_dwOBJ));
				m_vSKY[!m_bTSKY].m_vScale._11 = TSKY_SCALE;
				m_vSKY[!m_bTSKY].m_vScale._22 = TSKY_SCALE;
				m_vSKY[!m_bTSKY].m_vScale._33 = TSKY_SCALE;
				m_vSKY[!m_bTSKY].SetCloth(
					m_pDevice,
					pTSKY->m_dwCLK,
					pTSKY->m_dwCL,
					pTSKY->m_dwMESH);
				m_vSKY[!m_bTSKY].ResetOBJPart(m_pDevice);

				m_vSKY[!m_bTSKY].SetAction(
					pTSKY->m_dwACT,
					pTSKY->m_dwANI);

				m_vSKY[!m_bTSKY].m_bHide = FALSE;
				m_vSKY[!m_bTSKY].m_bAlpha = 0;

				m_vFARIMAGE[!m_bTSKY].CalcTick( m_pDevice->m_pDevice, 0);
				m_vSKY[!m_bTSKY].CalcTick( m_pDevice->m_pDevice, 0);
				m_dwTSKY = 0;
			}
			else
				m_wTSKY = wTSKY;
		}
		else
		{
			m_dwTSKY = min( m_dwTSKY + dwTick, TMAXSKY_TICK);

			m_vFARIMAGE[!m_bTSKY].m_bAlpha = BYTE(ALPHA_MAX * m_dwTSKY / TMAXSKY_TICK);
			m_vSKY[!m_bTSKY].m_bAlpha = m_vFARIMAGE[!m_bTSKY].m_bAlpha;

			if( m_vSKY[!m_bTSKY].m_bAlpha == ALPHA_MAX )
			{
				m_vFARIMAGE[m_bTSKY].m_bHide = TRUE;
				m_vFARIMAGE[m_bTSKY].m_bAlpha = 0;

				m_vSKY[m_bTSKY].m_bHide = TRUE;
				m_vSKY[m_bTSKY].m_bAlpha = 0;

				m_bTSKY = !m_bTSKY;
				m_wTSKY = wTSKY;
				m_dwTSKY = 0;
			}
		}
	else if(!m_vSKY[!m_bTSKY].m_bHide)
	{
		m_dwTSKY -= min( m_dwTSKY, dwTick);

		m_vFARIMAGE[!m_bTSKY].m_bAlpha = BYTE(ALPHA_MAX * m_dwTSKY / TMAXSKY_TICK);
		m_vSKY[!m_bTSKY].m_bAlpha = m_vFARIMAGE[!m_bTSKY].m_bAlpha;

		if(!m_vSKY[!m_bTSKY].m_bAlpha)
		{
			m_vFARIMAGE[!m_bTSKY].m_bHide = TRUE;
			m_vSKY[!m_bTSKY].m_bHide = TRUE;
		}
	}
}

BYTE CTClientGame::POSInRECT( LPD3DXVECTOR3 pTPOS,
							  LPD3DXPLANE pTRECT)
{
	for( BYTE i=0; i<4; i++)
		if( D3DXPlaneDotCoord( &pTRECT[i], pTPOS) > 0.0f )
			return FALSE;

	return TRUE;
}

void CTClientGame::DeleteDynimicOBJ( CTClientObjBase *pTOBJ)
{
	if( m_pACT == pTOBJ )
	{
		if(m_bCHARGE)
		{
			SendCS_ACTION_REQ(
				m_pMainChar->m_dwID,
				m_pMainChar->m_bType,
				0, 0, 0,
				m_pMainWnd->m_bChannel,
				m_vMAP.m_wMapID, 0);
		}

		CancelCharge(TRUE);
	}

	if( m_pTARGET == pTOBJ )
	{
		CTClientObjBase::m_mapTSELECTOBJ.clear();
		CTClientObjBase::m_mapTSELECTDEADOBJ.clear();

		ResetTargetOBJ(NULL);
		ResetTargetINFO(FALSE);
	}

	CTClientObjBase::DeleteOBJ(pTOBJ);
}

BYTE CTClientGame::CheckBackSkill( LPTSKILL pTBACKSKILL, BYTE bBackSkillItemType )
{
	return	pTBACKSKILL &&
			!pTBACKSKILL->m_wItemID[bBackSkillItemType] &&
			!(pTBACKSKILL->m_bPositive % 2) &&
			pTBACKSKILL->m_bTargetType == TSKILLTARGET_NONE ||
			pTBACKSKILL->m_bTargetType == TSKILLTARGET_ENEMY ||
			pTBACKSKILL->m_bTargetType == TSKILLTARGET_MONSTER ||
			pTBACKSKILL->m_bTargetType == TSKILLTARGET_ALL ||
			pTBACKSKILL->m_bTargetType == TSKILLTARGET_ENEMYRECALL;
}

BYTE CTClientGame::CheckSkillTarget( CTClientObjBase *pTATTACK,
									 CTClientObjBase *pTDEFEND,
									 LPTSKILL pTSKILL)
{
	if(!pTSKILL)
		return SKILL_NOTFOUND;

	if( !pTDEFEND || (!pTDEFEND->m_bCanSelected && pTATTACK != pTDEFEND) ||
		((pTSKILL->m_bTargetType != TSKILLTARGET_SELF) && !pTDEFEND->m_bSelectable) )
		return SKILL_WRONGTARGET;

	if(	pTDEFEND->m_bType != OT_PC &&
		pTDEFEND->m_bType != OT_MON &&
		pTDEFEND->m_bType != OT_RECALL &&
		pTDEFEND->m_bType != OT_SELF )
		return SKILL_WRONGTARGET;

	if( pTATTACK && pTSKILL->m_bRangeType != TSKILLRANGE_NONE &&
		!CanCharacterSee(pTATTACK,pTDEFEND) &&
 	    !(pTATTACK->m_wTDETECT & (1 << pTDEFEND->m_bType)) )
		return SKILL_CANNOTSEE;

	if( pTSKILL->m_wDefSkillID && !pTDEFEND->CountMaintain( pTSKILL->m_wDefSkillID, 1) )
		return SKILL_WRONGTARGET;

	if( (pTDEFEND->IsDead() || pTDEFEND->m_bGhost) &&
		(pTDEFEND->m_bType != OT_PC || pTSKILL->m_bTargetType != TSKILLTARGET_DEAD) &&
		(pTDEFEND->m_bType != OT_MON || pTSKILL->m_bTargetType != TSKILLTARGET_LOWLEVELMONDEAD) )
		return SKILL_WRONGTARGET;

	switch(pTDEFEND->m_bType)
	{
	case OT_RECALL	:
	case OT_SELF	:
		if( ((CTClientRecall *) pTDEFEND)->m_bRecallType == TRECALLTYPE_MINE )
			return pTSKILL->m_bTargetType == TSKILLTARGET_MINE ? SKILL_SUCCESS : SKILL_WRONGTARGET;
		break;
	case OT_PC		:
		{
			CTClientChar* pDefChar = static_cast< CTClientChar* >( pTDEFEND );
			if( pTATTACK &&
				m_bDuel == FALSE &&
				pDefChar->m_bPrivateShop == TRUE &&
				pDefChar->IsAlliance( pTATTACK ) )
			{
				return SKILL_WRONGTARGET;
			}
		}
		break;
	}

	if(!pTATTACK)
		return SKILL_SUCCESS;

	if( pTSKILL->m_bRangeType == TSKILLRANGE_TNEAR && pTDEFEND == m_pMainChar )
		return SKILL_SUCCESS;

	switch(pTSKILL->m_bTargetType)
	{
	case TSKILLTARGET_CORPS				:
	case TSKILLTARGET_GUILD				:
	case TSKILLTARGET_SELF				:
		if( pTATTACK == pTDEFEND )
			return SKILL_SUCCESS;

		break;

	case TSKILLTARGET_MAINRECALL		:
	case TSKILLTARGET_RECALL			:
		if( pTATTACK->GetHostID() == pTDEFEND->GetHostID() && (
			pTDEFEND->m_bType == OT_RECALL ||
			pTDEFEND->m_bType == OT_SELF) )
			if( pTSKILL->m_bTargetType == TSKILLTARGET_RECALL )
				return SKILL_SUCCESS;
			else if( pTDEFEND->m_bType == OT_RECALL && static_cast<CTClientRecall *>(pTDEFEND)->m_bRecallType == TRECALLTYPE_MAIN )
				return SKILL_SUCCESS;

		break;

	case TSKILLTARGET_ALLIANCE			:
		if(pTATTACK->IsAlliance(pTDEFEND))
			return SKILL_SUCCESS;

		break;

	case TSKILLTARGET_PARTY				:
	case TSKILLTARGET_PARTYEXCEPTME		:
		if( pTATTACK != pTDEFEND )
		{
			WORD wPartyID = GetPartyID(pTATTACK);

			if(wPartyID)
				return wPartyID == GetPartyID(pTDEFEND) ? SKILL_SUCCESS : SKILL_WRONGTARGET;
		}
		else if( pTSKILL->m_bTargetType == TSKILLTARGET_PARTYEXCEPTME )
			return SKILL_WRONGTARGET;
		else
			return SKILL_SUCCESS;

		break;

	case TSKILLTARGET_DEAD				:
		if( pTATTACK->IsAlliance(pTDEFEND) && pTDEFEND->m_bType == OT_PC && (
			pTDEFEND->IsDead() || pTDEFEND->m_bGhost) )
			return SKILL_SUCCESS;

		break;

	case TSKILLTARGET_ENEMY:
		{
			BOOL bAlliance = pTDEFEND->IsDisguise() &&
				pTATTACK->m_bType == OT_MON &&
				pTATTACK->m_bContryID != TCONTRY_N ?
				bAlliance = !pTATTACK->IsAlliance(pTDEFEND) :
				bAlliance = pTATTACK->IsAlliance(pTDEFEND);

			if( !bAlliance &&
				(pTATTACK->m_bType == OT_MON || IsInBattle( pTATTACK, pTDEFEND)) )
				return SKILL_SUCCESS;
		}
		break;

	case TSKILLTARGET_ALL:
		{
			BOOL bAlliance = pTDEFEND->IsDisguise() &&
				pTATTACK->m_bType == OT_MON &&
				pTATTACK->m_bContryID != TCONTRY_N ?
				bAlliance = !pTATTACK->IsAlliance(pTDEFEND) :
				bAlliance = pTATTACK->IsAlliance(pTDEFEND);

			if( bAlliance ||
				pTATTACK->m_bType == OT_MON ||
				IsInBattle( pTATTACK, pTDEFEND ) )
				return SKILL_SUCCESS;

			if( pTDEFEND->m_pTREGION &&
				pTDEFEND->m_pTREGION->m_bContryID == TCONTRY_PEACE &&
				!CTClientSkill::IsAttackSKILL(pTSKILL) )
				return SKILL_SUCCESS;
		}

		break;

	case TSKILLTARGET_ENEMYRECALL		:
		if( !pTATTACK->IsAlliance(pTDEFEND) &&
			IsInBattle(pTATTACK,pTDEFEND) && (
			pTDEFEND->m_bType == OT_RECALL ||
			pTDEFEND->m_bType == OT_SELF) )
			return SKILL_SUCCESS;

		break;

	case TSKILLTARGET_MONSTER			:
		if( pTDEFEND->m_bType == OT_MON )
			return SKILL_SUCCESS;

		break;

	case TSKILLTARGET_LOWLEVELMON		:
		if( pTDEFEND->m_bType == OT_MON &&
			pTATTACK->m_bLevel >= pTDEFEND->m_bLevel )
			return SKILL_SUCCESS;

		break;

	case TSKILLTARGET_LOWLEVELMONDEAD	:
		if( pTDEFEND->m_bType == OT_MON &&
			pTATTACK->m_bLevel >= pTDEFEND->m_bLevel &&
			pTDEFEND->IsDead() &&
			((CTClientMonster *) pTDEFEND)->m_pMON &&
			((CTClientMonster *) pTDEFEND)->m_pMON->m_bCanTame)
			return SKILL_SUCCESS;

		break;

	case TSKILLTARGET_ALLIANCEEXCEPTME	:
		if( pTATTACK != pTDEFEND && pTATTACK->IsAlliance(pTDEFEND) )
			return SKILL_SUCCESS;

		break;
	}

	return SKILL_WRONGTARGET;
}

BYTE CTClientGame::CheckSkillItem(LPTSKILL pTSKILL)
{
	return CheckInvenItem(pTSKILL->m_wNeedItemID);	
}

BYTE CTClientGame::CanUseSkill( CTClientObjBase *pTATTACK,
							    CTClientObjBase *pTDEFEND,
								LPD3DXVECTOR3 pTGROUND,
								LPTSKILL pTSKILL)
{
	if(!pTSKILL)
		return SKILL_NOTFOUND;

	if( pTATTACK->IsDown() || (
		pTATTACK->IsBlock() &&
		!pTSKILL->m_bUseInHold) )
		return SKILL_STUN;

	if( pTATTACK->IsDead() ||
		pTATTACK->m_bGhost )
		return SKILL_DEAD;

	if(!pTATTACK->CheckSkillMode(pTSKILL))
		return SKILL_MODE;

	if(!pTATTACK->CheckSkillSilence(pTSKILL))
		return SKILL_SILENCE;

	if(!pTATTACK->CheckTransMode( pTSKILL))
		return SKILL_TRANS;

	if(!pTATTACK->CheckSkillPrevACT(pTSKILL))
		return SKILL_NEEDPREVACT;

	if(!pTATTACK->CheckSkillWespon(pTSKILL))
		return SKILL_UNSUITWEAPON;

	if(!pTATTACK->CheckSkillRegion(pTSKILL))
		return SKILL_WRONGREGION;

	if( pTATTACK == m_pMainChar &&
		pTSKILL->m_wNeedItemID &&
		!CheckSkillItem(pTSKILL))
		return SKILL_NEEDITEM;

	if( pTSKILL->m_bRangeType == TSKILLRANGE_MCIRCLE )
		return SKILL_SUCCESS;

	BYTE bRANGE = pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE ? TRUE : FALSE;
	BYTE bPOINT = pTSKILL->m_bRangeType == TSKILLRANGE_POINT ? TRUE : FALSE;

	D3DXVECTOR3 vTARGET( 0.0f, 0.0f, 0.0f);
	FLOAT fDIST = 0.0f;

	if( !bRANGE && !bPOINT )
	{
		BYTE bRESULT = CheckSkillTarget(
			pTATTACK,
			pTDEFEND,
			pTSKILL);

		if( bRESULT != SKILL_SUCCESS )
			return bRESULT;

		if(!CanDetect( pTATTACK, pTDEFEND))
			return SKILL_HIDE;

		vTARGET = D3DXVECTOR3(
			pTDEFEND->GetPositionX() - pTATTACK->GetPositionX(),
			pTDEFEND->GetPositionY() - pTATTACK->GetPositionY(),
			pTDEFEND->GetPositionZ() - pTATTACK->GetPositionZ());
	}
	else if(pTGROUND)
	{
		vTARGET = D3DXVECTOR3(
			pTGROUND->x - pTATTACK->GetPositionX(),
			pTGROUND->y - pTATTACK->GetPositionY(),
			pTGROUND->z - pTATTACK->GetPositionZ());

		pTDEFEND = NULL;
	}
	else
		return SKILL_NEEDGROUND;

	if( !bRANGE && !bPOINT && pTSKILL->m_bTargetType == TSKILLTARGET_SELF )
		return SKILL_SUCCESS;
	fDIST = D3DXVec3Length(&vTARGET);

	if( fDIST < pTATTACK->GetMinRange( pTDEFEND, pTSKILL) )
		return SKILL_TOOCLOSE;

	if( fDIST > pTATTACK->GetMaxRange( pTDEFEND, pTSKILL) )
		return SKILL_TOOFAR;

	WORD wDIR = pTATTACK->GetTargetDIR(
		vTARGET.x,
		vTARGET.z);
	int nDIR = INT(wDIR) - INT(pTATTACK->m_wDIR);

	while(nDIR < 0)
		nDIR += 1800;
	nDIR %= 1800;

	if( nDIR > 900 )
		nDIR = 1800 - nDIR;

	if( nDIR > FRONT_DIR_RANGE )
		return SKILL_WRONGDIR;

	return SKILL_SUCCESS;
}

BYTE CTClientGame::CanDefend( CTClientObjBase *pTATTACK,
							  CTClientObjBase *pTDEFEND,
							  LPTSKILL pTSKILL)
{
	BYTE bResult = FALSE;

	if( pTDEFEND &&
		pTDEFEND->m_bCanDefend &&
		pTSKILL &&
		CheckSkillTarget( pTATTACK, pTDEFEND, pTSKILL) == SKILL_SUCCESS )
	{
		bResult = TRUE;

		if( pTATTACK != pTDEFEND )
			bResult = !pTDEFEND->m_bClarity;
	}

	return bResult;
}

BYTE CTClientGame::CanLookAt()
{
	return m_vKEY.m_vSTATE[TKEY_ROT] || m_vKEY.m_vSTATE[TKEY_BACK] ||
		m_vKEY.m_vSTATE[TKEY_LSIDE] || m_vKEY.m_vSTATE[TKEY_RSIDE] ||
		m_vKEY.m_vSTATE[TKEY_LEFT] || m_vKEY.m_vSTATE[TKEY_RIGHT] ||
		m_vKEY.m_vSTATE[TKEY_FORWARD] || m_bAutoRun ? FALSE : TRUE;
}

BYTE CTClientGame::CanControl()
{
	return m_pCtrlHost == m_pMainChar || ((CTClientRecall *) m_pCtrlHost)->m_bRecallType != TRECALLTYPE_SPY ? TRUE : FALSE;
}

BYTE CTClientGame::IsTSKILLCharge()
{
	if(!m_pMainChar->m_vTSKILLDATA.m_wID)
		return FALSE;

	if(!m_pACT)
		return TRUE;

	switch(m_pACT->m_bType)
	{
	case OT_SWITCH	:
	case OT_NPC		:
	case OT_GODTOWER :
	case OT_GODBALL : return FALSE;
	}

	return TRUE;
}

void CTClientGame::CheckAttackMaintain( CTClientObjBase *pTATTACK,
									    LPTSKILL pTSKILL)
{
	if( !pTATTACK || !pTSKILL )
		return;

	for( int i=0; i<INT(pTSKILL->m_vTFUNCTION.size()); i++)
		if( pTSKILL->m_vTFUNCTION[i]->m_bType == SDT_STATUS &&
			pTSKILL->m_vTFUNCTION[i]->m_bFunctionID == SDT_STATUS_LINK &&
			pTSKILL->m_vTFUNCTION[i]->m_wInc )
		{
			LPTSKILL pTLINK = CTChart::FindTSKILLTEMP(pTSKILL->m_vTFUNCTION[i]->m_wValue);

			if( pTLINK && CanDefend( pTATTACK, pTATTACK, pTLINK) )
			{
				D3DXVECTOR3 vTGROUND(
					pTATTACK->GetPositionX(),
					pTATTACK->GetPositionY(),
					pTATTACK->GetPositionZ());
				Defend( pTATTACK, pTATTACK, &pTATTACK->m_vTSKILLDATA, &vTGROUND, pTLINK, 0, TRUE );
			}
		}
}

void CTClientGame::CheckAutoSKILL( CTClientRecall *pTRECALL)
{
	pTRECALL->m_vTSKILLDATA.m_vTGROUND = D3DXVECTOR3(
		pTRECALL->GetPositionX(),
		pTRECALL->GetPositionY(),
		pTRECALL->GetPositionZ());

	if( pTRECALL->m_bRecallType == TRECALLTYPE_SKILL && pTRECALL->m_pTCURSKILL && pTRECALL->m_pTCURSKILL->m_pTSKILL )
	{
		pTRECALL->m_vTSKILLDATA.m_wID = pTRECALL->m_pTCURSKILL->m_pTSKILL->m_wSkillID;

		CheckAttackMaintain( pTRECALL, CTChart::FindTSKILLTEMP(pTRECALL->m_vTSKILLDATA.m_wID));
		ShotSkill(pTRECALL, &pTRECALL->m_vTSKILLDATA);
	}

	if(!IsHostingOBJ( pTRECALL->m_dwID, pTRECALL->m_bType))
		return;

	for( BYTE i=0; i<TMONSKILL_COUNT; i++)
		if(pTRECALL->m_pTEMP->m_wTSKILL[i])
		{
			CTClientSkill *pTSKILL = pTRECALL->FindTSkill(pTRECALL->m_pTEMP->m_wTSKILL[i]);

			if( pTSKILL && pTSKILL->m_pTSKILL && pTSKILL->m_pTSKILL->m_fBufRange > 0.0f &&
				!pTSKILL->m_pTSKILL->m_wItemID[pTRECALL->GetSkillItemType(pTSKILL->m_pTSKILL)] && (
				!CTClientSkill::IsAttackSKILL(pTSKILL->m_pTSKILL) || pTRECALL->m_bRecallType == TRECALLTYPE_MAINTAIN) )
			{
				CTClientMaintain* pTMAINTAIN = pTRECALL->FindMaintain( pTSKILL->m_pTSKILL->m_wSkillID );

				if( pTMAINTAIN != NULL )
				{
					SendCS_SKILLEND_REQ(
						pTRECALL->m_dwID,
						pTRECALL->m_bType,
						pTRECALL->GetHostID(),
						pTMAINTAIN->m_dwAttackID,
						pTMAINTAIN->m_bAttackType,
						pTSKILL->m_pTSKILL->m_wSkillID,
						m_vMAP.m_wMapID,
						m_pMainWnd->m_bChannel);
				}

				if( pTRECALL->m_bRecallType != TRECALLTYPE_MAINTAIN )
				{
					SendCS_LOOPSKILL_REQ(
						pTRECALL,
						pTRECALL,
						&pTRECALL->m_vTSKILLDATA.m_vTGROUND,
						pTSKILL->m_pTSKILL);
				}
				else if(CanDefend( pTRECALL, pTRECALL, pTSKILL->m_pTSKILL))
					Defend( pTRECALL, pTRECALL, &pTRECALL->m_vTSKILLDATA, &pTRECALL->m_vTSKILLDATA.m_vTGROUND, pTSKILL->m_pTSKILL, pTRECALL->GetLeftLifeTick(), TRUE );
			}
		}
}

void CTClientGame::BuildMaintainList()
{
	MAPMONSTER::iterator itTMON;
	MAPPLAYER::iterator itTPC;
	MAPRECALL::iterator itTRECALL;
	m_vTMAINTAINOBJ.clear();

	for( itTRECALL = m_mapFIXRECALL.begin(); itTRECALL != m_mapFIXRECALL.end(); itTRECALL++)
		(*itTRECALL).second->BuildMaintainList( this );

	for( itTRECALL = m_mapRECALL.begin(); itTRECALL != m_mapRECALL.end(); itTRECALL++)
		(*itTRECALL).second->BuildMaintainList( this );

	for( itTMON = m_mapMONSTER.begin(); itTMON != m_mapMONSTER.end(); itTMON++)
		(*itTMON).second->BuildMaintainList( this );

	for( itTPC = m_mapPLAYER.begin(); itTPC != m_mapPLAYER.end(); itTPC++)
		(*itTPC).second->BuildMaintainList( this );

	m_pMainChar->BuildMaintainList( this );
}

void CTClientGame::CheckMaintainOBJ()
{
	MAPMONSTER::iterator itTMON;
	MAPPLAYER::iterator itTPC;
	MAPRECALL::iterator itTRECALL;

	for( itTRECALL = m_mapSLAVEFIXRECALL.begin(); itTRECALL != m_mapSLAVEFIXRECALL.end(); itTRECALL++)
		(*itTRECALL).second->CheckMaintainOBJ( this );

	for( itTRECALL = m_mapSLAVERECALL.begin(); itTRECALL != m_mapSLAVERECALL.end(); itTRECALL++)
		(*itTRECALL).second->CheckMaintainOBJ( this );

	for( itTMON = m_mapSLAVEMON.begin(); itTMON != m_mapSLAVEMON.end(); itTMON++)
		(*itTMON).second->CheckMaintainOBJ( this );

	for( itTMON = m_mapMONSTER.begin(); itTMON != m_mapMONSTER.end(); itTMON++)
	{
		if( !(*itTMON).second->m_bHostMainChar )
			(*itTMON).second->CheckMaintainOBJ( this, FALSE );
	}

	for( itTRECALL = m_mapRECALL.begin(); itTRECALL != m_mapRECALL.end(); itTRECALL++)
	{
		if( (*itTRECALL).second->m_dwHostID != m_pMainChar->m_dwID )
			(*itTRECALL).second->CheckMaintainOBJ( this, FALSE );
	}

	for( itTPC = m_mapPLAYER.begin(); itTPC != m_mapPLAYER.end(); itTPC++)
	{
		if( (*itTPC).second != m_pMainChar )
			(*itTPC).second->CheckMaintainOBJ( this, FALSE );
	}

	m_pMainChar->CheckMaintainOBJ( this );
}

BYTE CTClientGame::CheckGroundDRAW()
{
	if( m_bDrawGROUND || m_bGroundLOCK || m_bMoveType == TMAINMOVE_GROUND )
		return TRUE;

	if( m_bCHARGE && !m_bLOOPCHARGE && IsTSKILLCharge() )
	{
		CTClientSkill *pTSKILL = m_pMainChar->FindTSkill(m_pMainChar->m_vTSKILLDATA.m_wID);

		if( pTSKILL && pTSKILL->m_pTSKILL )
		{
			BYTE bRANGE = pTSKILL->m_pTSKILL->m_bRangeType == TSKILLRANGE_CIRCLE ? TRUE : FALSE;
			BYTE bPOINT = pTSKILL->m_pTSKILL->m_bRangeType == TSKILLRANGE_POINT ? TRUE : FALSE;

			return bRANGE || bPOINT ? TRUE : FALSE;
		}
	}

	return FALSE;
}

BYTE CTClientGame::IsHostingEnemy( CTClientObjBase *pTOBJ)
{
	LPTENEMY pTENEMY = m_pTRSCS->FindTENEMY(
		pTOBJ->m_dwID,
		pTOBJ->m_bType);

	return pTENEMY && !pTENEMY->m_mapTREPORTER.empty() && (*pTENEMY->m_mapTREPORTER.begin()).second == m_pMainChar->m_dwID ? TRUE : FALSE;
}

BYTE CTClientGame::IsRSCSEnemy( CTClientObjBase *pTOBJ)
{
	return pTOBJ->m_bCanSelected && !pTOBJ->m_bHide && !m_pMainChar->IsAlliance(pTOBJ) ? TRUE : FALSE;
}

void CTClientGame::CheckTRSCS( UINT nFlags, CPoint point)
{
	if( !m_vMAP.m_pMAP || !GetPartyID(m_pMainChar) || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID )
		return;

	if( m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
	{
		CorpsMSG( m_pMainChar, CORPS_DEAD);
		return;
	}

	CRect rect(m_pTRSCS->m_vTAREA);
	CPoint vPOS;

	m_pTRSCS->GetComponentPos(&vPOS);
	rect.OffsetRect(
		vPOS.x,
		vPOS.y);

	if(rect.PtInRect(point))
	{
		for(INT i=0; i<TRSCSCOMP_COUNT; ++i)
		{
			if( m_pTRSCS->m_vTCOMP[i]->HitTest(point) )
				return;
		}

		FLOAT fSCALE = m_pTRSCS->m_fTSCALE / FLOAT(m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength);

		DWORD dwTargetID = 0;
		BYTE bTargetType = 0;
		BYTE bTCMD = 0;

		WORD wTargetX = 0;
		WORD wTargetZ = 0;

		point.x -= rect.left;
		point.y -= rect.top;

		point.x *= TMINIMAPTEX_SIZE;
		point.y *= TMINIMAPTEX_SIZE;

		point.y /= rect.Height();
		point.x /= rect.Width();

		LPTENEMY pTENEMY = PickTCMDATTACK( &m_pTRSCS->m_vTCORPS.m_mapPC, point, fSCALE);
		WORD wTPROTECT = 0xFFFF;

		if(!pTENEMY)
			pTENEMY = PickTCMDATTACK( &m_pTRSCS->m_vTCORPS.m_mapMON, point, fSCALE);

		if(!pTENEMY)
			pTENEMY = PickTCMDATTACK( &m_pTRSCS->m_vTCORPS.m_mapRECALL, point, fSCALE);

		if(!pTENEMY)
			pTENEMY = PickTCMDATTACK( &m_pTRSCS->m_vTCORPS.m_mapFIXRECALL, point, fSCALE);

		if(!pTENEMY)
			wTPROTECT = PickTCMDPROTECT( point, fSCALE);

		if(pTENEMY)
		{
			bTargetType = pTENEMY->m_bType;
			dwTargetID = pTENEMY->m_dwID;
			bTCMD = TRSCSCMD_ATTACK;
		}
		else if( wTPROTECT != 0xFFFF )
		{
			LPTSQUAD pTSQUAD = m_pTRSCS->m_vTCORPS.m_vTSQUAD[HIBYTE(wTPROTECT)];
			LPTUNIT pTUNIT = pTSQUAD->m_vTUNIT[LOBYTE(wTPROTECT)];

			dwTargetID = pTUNIT->m_dwCharID;
			wTargetX = pTSQUAD->m_wPartyID;
			bTCMD = TRSCSCMD_PROTECT;
		}
		else
		{
			D3DXVECTOR2 vTARGET = PickTCMDMOVE(
				point,
				fSCALE);

			wTargetX = WORD(vTARGET.x);
			wTargetZ = WORD(vTARGET.y);
			bTCMD = TRSCSCMD_MOVE;
		}

		ExecTCMD( bTCMD, dwTargetID, bTargetType, wTargetX, wTargetZ, m_pTRSCS->m_wMapID, (nFlags & MK_CONTROL) ? TRUE : FALSE);
	}
}

void CTClientGame::ExecTCMDFromTOBJ( CTClientObjBase *pTOBJ,
									 BYTE bToALL)
{
	if(!pTOBJ)
		return;

	switch(pTOBJ->m_bType)
	{
	case OT_PC	:
		{
			LPTENEMY pTENEMY = m_pTRSCS->FindTENEMY(
				pTOBJ->m_dwID,
				pTOBJ->m_bType);

			if(!pTENEMY)
			{
				WORD wSquadID = GetPartyID(pTOBJ);

				if(wSquadID)
				{
					LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
						m_pTRSCS->FindTSQUAD(wSquadID),
						pTOBJ->m_dwID);

					if(pTUNIT)
						ExecTCMD( TRSCSCMD_PROTECT, pTOBJ->m_dwID, pTOBJ->m_bType, wSquadID, 0, m_vMAP.m_wMapID, bToALL);
				}
			}
			else
				ExecTCMD( TRSCSCMD_ATTACK, pTENEMY->m_dwID, pTENEMY->m_bType, 0, 0, m_vMAP.m_wMapID, bToALL);
		}

		break;

	default		:
		{
			LPTENEMY pTENEMY = m_pTRSCS->FindTENEMY(
				pTOBJ->m_dwID,
				pTOBJ->m_bType);

			if(pTENEMY)
				ExecTCMD( TRSCSCMD_ATTACK, pTENEMY->m_dwID, pTENEMY->m_bType, 0, 0, m_vMAP.m_wMapID, bToALL);
		}

		break;
	}
}

void CTClientGame::ExecTCMDFromParty( BYTE bIndex,
									  BYTE bToALL)
{
	WORD wSquadID = GetPartyID(m_pMainChar);

	if(wSquadID)
	{
		LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
			m_pTRSCS->FindTSQUAD(wSquadID),
			m_vTPARTY[bIndex]->m_dwCharID);

		if(pTUNIT)
			ExecTCMD( TRSCSCMD_PROTECT, pTUNIT->m_dwCharID, OT_PC, wSquadID, 0, pTUNIT->m_wMapID, bToALL);
	}
}

void CTClientGame::ExecTCMDFromPoint( int nPosX,
									  int nPosY,
									  BYTE bToALL)
{
	D3DXVECTOR3 vPOS = m_pCamera->GetCameraPosition( nPosX, nPosY);
	D3DXVECTOR3 vDIR = m_pCamera->GetRayDirection( nPosX, nPosY);
	FLOAT fDIST = 0.0f;

	vDIR /= D3DXVec3Length(&vDIR);
	D3DXVECTOR3 vTARGET = vPOS + CTClientObjBase::m_fCamDist * vDIR;

	if(m_vMAP.HitTest( &fDIST, &vPOS, &vTARGET, 0.0f, 0.0f, FALSE))
	{
		vPOS += fDIST * vDIR;
		ExecTCMD( TRSCSCMD_MOVE, 0, 0, WORD(vPOS.x), WORD(vPOS.z), m_vMAP.m_wMapID, bToALL);
	}
}

void CTClientGame::ExecTCORPSCMD( LPTSQUAD pTSQUAD,
								  BYTE bTCMD,
								  DWORD dwTargetID,
								  BYTE bTargetType,
								  WORD wTargetX,
								  WORD wTargetZ,
								  WORD wMapID)
{
	if( !pTSQUAD || m_pMainChar->m_dwID != m_pMainChar->m_dwChiefID || GetPartyID(m_pMainChar) != m_pMainChar->m_wCommanderID )
		return;

	LPTUNIT pTUNIT = m_pTRSCS->FindTUNIT(
		pTSQUAD,
		pTSQUAD->m_dwChiefID);

	if(!pTUNIT)
		return;

	ExecTUNITCMD(
		pTUNIT,
		pTSQUAD->m_wPartyID,
		bTCMD,
		dwTargetID,
		bTargetType,
		wTargetX,
		wTargetZ,
		wMapID);

	if( wMapID == 0xFFFF || m_vMAP.m_wMapID == wMapID )
	{
		ResetTRSCSCMD(
			&pTSQUAD->m_vTMARK,
			NULL,
			&pTSQUAD->m_vTARROW,
			bTCMD,
			dwTargetID,
			bTargetType,
			wTargetX,
			wTargetZ);
	}

	pTSQUAD->m_bTargetType = bTargetType;
	pTSQUAD->m_dwTargetID = dwTargetID;

	pTSQUAD->m_wTargetMapID = wMapID;
	pTSQUAD->m_wTargetX = wTargetX;
	pTSQUAD->m_wTargetZ = wTargetZ;
	pTSQUAD->m_bTCMD = bTCMD;
}

void CTClientGame::ExecTSQUADCMD( WORD wSquadID,
								  BYTE bTCMD,
								  DWORD dwTargetID,
								  BYTE bTargetType,
								  WORD wTargetX,
								  WORD wTargetZ,
								  WORD wMapID)
{
	LPTSQUAD pTSQUAD = m_pTRSCS->FindTSQUAD(wSquadID);

	if(pTSQUAD)
	{
		ExecTSQUADCMD(
			pTSQUAD,
			bTCMD,
			dwTargetID,
			bTargetType,
			wTargetX,
			wTargetZ,
			wMapID);
	}
}

void CTClientGame::ExecTSQUADCMD( LPTSQUAD pTSQUAD,
								  BYTE bTCMD,
								  DWORD dwTargetID,
								  BYTE bTargetType,
								  WORD wTargetX,
								  WORD wTargetZ,
								  WORD wMapID)
{
	if( !pTSQUAD || m_pMainChar->m_dwID != m_pMainChar->m_dwChiefID )
		return;

	for( BYTE i=0; i<BYTE(pTSQUAD->m_vTUNIT.size()); i++)
		if( pTSQUAD->m_vTUNIT[i]->m_dwCharID != pTSQUAD->m_dwChiefID && (
			wMapID == 0xFFFF || pTSQUAD->m_vTUNIT[i]->m_wMapID == wMapID) )
		{
			ExecTUNITCMD(
				pTSQUAD->m_vTUNIT[i],
				pTSQUAD->m_wPartyID,
				bTCMD,
				dwTargetID,
				bTargetType,
				wTargetX,
				wTargetZ,
				wMapID);
		}

	if( wMapID == 0xFFFF || m_vMAP.m_wMapID == wMapID )
	{
		ResetTRSCSCMD(
			&pTSQUAD->m_vTMARK,
			NULL,
			&pTSQUAD->m_vTARROW,
			bTCMD,
			dwTargetID,
			bTargetType,
			wTargetX,
			wTargetZ);
	}

	pTSQUAD->m_bTargetType = bTargetType;
	pTSQUAD->m_dwTargetID = dwTargetID;

	pTSQUAD->m_wTargetMapID = wMapID;
	pTSQUAD->m_wTargetX = wTargetX;
	pTSQUAD->m_wTargetZ = wTargetZ;
	pTSQUAD->m_bTCMD = bTCMD;
}

void CTClientGame::ExecTUNITCMD( LPTUNIT pTUNIT,
								 WORD wSquadID,
								 BYTE bTCMD,
								 DWORD dwTargetID,
								 BYTE bTargetType,
								 WORD wTargetX,
								 WORD wTargetZ,
								 WORD wMapID)
{
	if( !pTUNIT || m_pMainChar->m_dwID != m_pMainChar->m_dwChiefID )
		return;

	RelaySession()->SendCS_CORPSCMD_REQ(
		wSquadID,
		pTUNIT->m_dwCharID,
		wMapID,
		bTCMD,
		dwTargetID,
		bTargetType,
		wTargetX,
		wTargetZ);

	pTUNIT->m_bTargetType = bTargetType;
	pTUNIT->m_dwTargetID = dwTargetID;

	pTUNIT->m_wTargetX = wTargetX;
	pTUNIT->m_wTargetZ = wTargetZ;
	pTUNIT->m_bTCMD = bTCMD;
}

void CTClientGame::ExecTCMD( BYTE bTCMD,
							 DWORD dwTargetID,
							 BYTE bTargetType,
							 WORD wTargetX,
							 WORD wTargetZ,
							 WORD wMapID,
							 BYTE bToALL)
{
	WORD wSquadID = GetPartyID(m_pMainChar);

	if( !wSquadID || m_pMainChar->m_dwChiefID != m_pMainChar->m_dwID )
		return;

	if( m_pMainChar->m_wCommanderID != wSquadID || (!bToALL && (!m_pTRSCS->m_pTCURSQUAD || m_pTRSCS->m_pTCURSQUAD->m_wPartyID == wSquadID)) )
	{
		ExecTSQUADCMD(
			wSquadID,
			bTCMD,
			dwTargetID,
			bTargetType,
			wTargetX,
			wTargetZ,
			wMapID);
	}
	else if( m_pMainChar->m_wCommanderID == wSquadID )
		if(bToALL)
			for( BYTE i=0; i<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
				if( m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_wPartyID != wSquadID )
				{
					ExecTCORPSCMD(
						m_pTRSCS->m_vTCORPS.m_vTSQUAD[i],
						bTCMD,
						dwTargetID,
						bTargetType,
						wTargetX,
						wTargetZ,
						wMapID);
				}
				else
				{
					ExecTSQUADCMD(
						m_pTRSCS->m_vTCORPS.m_vTSQUAD[i],
						bTCMD,
						dwTargetID,
						bTargetType,
						wTargetX,
						wTargetZ,
						wMapID);
				}
		else
		{
			ExecTCORPSCMD(
				m_pTRSCS->m_pTCURSQUAD,
				bTCMD,
				dwTargetID,
				bTargetType,
				wTargetX,
				wTargetZ,
				wMapID);
		}
}

void CTClientGame::ResetTRSCSCMD( CTClientObjBase *pTMARK,
								  CTClientObjBase *pTDIR,
								  CTClientArrow *pTARROW,
								  BYTE bTCMD,
								  DWORD dwTargetID,
								  BYTE bTargetType,
								  WORD wTargetX,
								  WORD wTargetZ)
{
	if( !pTARROW || !pTMARK )
		return;
	LPTSQUAD pTSQUAD = m_pTRSCS->FindTSQUAD(GetPartyID(m_pMainChar));

	pTARROW->m_vTSLASH.ResetKEY();
	pTMARK->m_bHide = TRUE;

	if( pTDIR && (bTCMD != TRSCSCMD_NONE || !pTSQUAD || pTSQUAD->m_bTCMD == TRSCSCMD_NONE) )
		pTDIR->m_bHide = TRUE;

	switch(bTCMD)
	{
	case TRSCSCMD_PROTECT	:
		{
			CTClientObjBase *pTOBJ = FindOBJ(
				dwTargetID,
				OT_PC);

			if(pTOBJ)
			{
				pTARROW->m_vTSKILLDATA.m_vTGROUND.x = pTOBJ->GetPositionX();
				pTARROW->m_vTSKILLDATA.m_vTGROUND.y = pTOBJ->GetPositionY() + pTOBJ->m_fSizeY;
				pTARROW->m_vTSKILLDATA.m_vTGROUND.z = pTOBJ->GetPositionZ();
			}
			else
			{
				LPTUNIT pTPROTECT = m_pTRSCS->FindTUNIT(
					m_pTRSCS->FindTSQUAD(wTargetX),
					dwTargetID);

				if( !pTPROTECT || !pTPROTECT->m_dwHP )
					return;

				pTARROW->m_vTSKILLDATA.m_vTGROUND.x = FLOAT(pTPROTECT->m_wPosX);
				pTARROW->m_vTSKILLDATA.m_vTGROUND.z = FLOAT(pTPROTECT->m_wPosZ);

				pTARROW->m_vTSKILLDATA.m_vTGROUND.y = m_vMAP.GetHeight(
					m_pMainChar,
					NULL,
					&D3DXVECTOR3(
					pTARROW->m_vTSKILLDATA.m_vTGROUND.x,
					m_pMainChar->GetPositionY(),
					pTARROW->m_vTSKILLDATA.m_vTGROUND.z),
					0.0f, FALSE);
			}
		}

		break;

	case TRSCSCMD_ATTACK	:
		{
			CTClientObjBase *pTOBJ = FindOBJ(
				dwTargetID,
				bTargetType);

			if(pTOBJ)
			{
				pTARROW->m_vTSKILLDATA.m_vTGROUND.x = pTOBJ->GetPositionX();
				pTARROW->m_vTSKILLDATA.m_vTGROUND.y = pTOBJ->GetPositionY() + pTOBJ->m_fSizeY;
				pTARROW->m_vTSKILLDATA.m_vTGROUND.z = pTOBJ->GetPositionZ();
			}
			else
			{
				LPTENEMY pTENEMY = m_pTRSCS->FindTENEMY(
					dwTargetID,
					bTargetType);

				if(!pTENEMY)
					return;

				pTARROW->m_vTSKILLDATA.m_vTGROUND.x = FLOAT(pTENEMY->m_wPosX);
				pTARROW->m_vTSKILLDATA.m_vTGROUND.z = FLOAT(pTENEMY->m_wPosZ);

				pTARROW->m_vTSKILLDATA.m_vTGROUND.y = m_vMAP.GetHeight(
					m_pMainChar,
					NULL,
					&D3DXVECTOR3(
					pTARROW->m_vTSKILLDATA.m_vTGROUND.x,
					m_pMainChar->GetPositionY(),
					pTARROW->m_vTSKILLDATA.m_vTGROUND.z),
					0.0f, FALSE);
			}
		}

		break;

	case TRSCSCMD_MOVE		:
		{
			pTARROW->m_vTSKILLDATA.m_vTGROUND.x = FLOAT(wTargetX);
			pTARROW->m_vTSKILLDATA.m_vTGROUND.z = FLOAT(wTargetZ);

			pTARROW->m_vTSKILLDATA.m_vTGROUND.y = m_vMAP.GetHeight(
				m_pMainChar,
				NULL,
				&D3DXVECTOR3(
				pTARROW->m_vTSKILLDATA.m_vTGROUND.x,
				m_pMainChar->GetPositionY(),
				pTARROW->m_vTSKILLDATA.m_vTGROUND.z),
				0.0f, FALSE);
		}

		break;

	default					: return;
	}

	D3DXVECTOR2 vDIR(
		pTARROW->m_vTSKILLDATA.m_vTGROUND.x - m_pMainChar->GetPositionX(),
		pTARROW->m_vTSKILLDATA.m_vTGROUND.z - m_pMainChar->GetPositionZ());

	pTARROW->m_fStartH = m_pMainChar->GetPositionY() + m_pMainChar->m_fSizeY + TCMDARROW_HEIGHT;
	D3DXMatrixRotationY(
		&pTARROW->m_vWorld,
		D3DX_PI + atan2f( vDIR.x, vDIR.y));

	pTARROW->m_vWorld._41 = m_pMainChar->GetPositionX();
	pTARROW->m_vWorld._42 = pTARROW->m_fStartH;
	pTARROW->m_vWorld._43 = m_pMainChar->GetPositionZ();
	pTARROW->m_vPosition = pTARROW->m_vWorld;

	pTARROW->m_vTSLASH.m_pPIVOT = &pTARROW->m_vPosition;
	pTARROW->m_vTSLASH.SFXStart();

	pTARROW->m_fGravity = TCMDARROW_GRAVITY / D3DXVec2Length(&vDIR);
	pTARROW->m_dwTick = 0;

	m_pTRSCS->m_vTALARM.x = pTARROW->m_vTSKILLDATA.m_vTGROUND.x;
	m_pTRSCS->m_vTALARM.y = pTARROW->m_vTSKILLDATA.m_vTGROUND.z;
	m_pTRSCS->m_dwTALARM = TCMDALARM_TICK;
}

LPTENEMY CTClientGame::PickTCMDATTACK( LPMAPTENEMY pTENEMYMAP,
									   CPoint point,
									   FLOAT fSCALE)
{
	MAPTENEMY::iterator itTENEMY;

	for( itTENEMY = pTENEMYMAP->begin(); itTENEMY != pTENEMYMAP->end(); itTENEMY++)
		if( HitRSCSOBJ( point, (*itTENEMY).second->m_wPosX, (*itTENEMY).second->m_wPosZ, fSCALE) && IsVisibleEnemy((*itTENEMY).second) )
			return (*itTENEMY).second;

	return NULL;
}

WORD CTClientGame::PickTCMDPROTECT( CPoint point,
								    FLOAT fSCALE)
{
	for( BYTE i=0; i<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
		for( BYTE j=0; j<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT.size()); j++)
			if(HitRSCSOBJ( point, m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_wPosX, m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j]->m_wPosZ, fSCALE))
				return MAKEWORD( j, i);

	return 0xFFFF;
}

BYTE CTClientGame::IsVisibleEnemy( LPTENEMY pTENEMY)
{
	D3DXVECTOR2 vTENEMY = m_pTRSCS->GetEnemyPOS(pTENEMY);

	for( BYTE i=0; i<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD.size()); i++)
		for( BYTE j=0; j<BYTE(m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT.size()); j++)
		{
			LPTUNIT pTUNIT = m_pTRSCS->m_vTCORPS.m_vTSQUAD[i]->m_vTUNIT[j];

			if( D3DXVec2Length(&(vTENEMY - m_pTRSCS->GetUnitPOS(pTUNIT))) < CTClientObjBase::m_fCamDist / 2.0f )
				return TRUE;
		}

	return FALSE;
}

D3DXVECTOR2 CTClientGame::PickTCMDMOVE( CPoint point,
									    FLOAT fSCALE)
{
	D3DXVECTOR2 vTPOS(
		FLOAT(point.x) - FLOAT(TMINIMAPTEX_SIZE) / 2.0f,
		FLOAT(TMINIMAPTEX_SIZE) / 2.0f - FLOAT(point.y));

	vTPOS /= fSCALE * FLOAT(TMINIMAPTEX_SIZE);
	vTPOS += m_pTRSCS->m_vTCENTER;

	return vTPOS;
}

BYTE CTClientGame::HitRSCSOBJ( CPoint point,
							   WORD wPosX,
							   WORD wPosZ,
							   FLOAT fSCALE)
{
	D3DXVECTOR2 vTPOS(
		FLOAT(wPosX) - m_pTRSCS->m_vTCENTER.x,
		m_pTRSCS->m_vTCENTER.y - FLOAT(wPosZ));

	vTPOS *= fSCALE * FLOAT(TMINIMAPTEX_SIZE);
	vTPOS.x += FLOAT(TMINIMAPTEX_SIZE) / 2.0f;
	vTPOS.y += FLOAT(TMINIMAPTEX_SIZE) / 2.0f;

	return D3DXVec2Length(&D3DXVECTOR2( FLOAT(point.x) - vTPOS.x, FLOAT(point.y) - vTPOS.y)) < TRSCSPICK_RANGE;
}

void CTClientGame::ReleaseRSCS()
{
	MAPTENEMY::iterator itTENEMY;

	for( itTENEMY = m_pTRSCS->m_vTCORPS.m_mapFIXRECALL.begin(); itTENEMY != m_pTRSCS->m_vTCORPS.m_mapFIXRECALL.end(); itTENEMY++)
		delete (*itTENEMY).second;

	for( itTENEMY = m_pTRSCS->m_vTCORPS.m_mapRECALL.begin(); itTENEMY != m_pTRSCS->m_vTCORPS.m_mapRECALL.end(); itTENEMY++)
		delete (*itTENEMY).second;

	for( itTENEMY = m_pTRSCS->m_vTCORPS.m_mapMON.begin(); itTENEMY != m_pTRSCS->m_vTCORPS.m_mapMON.end(); itTENEMY++)
		delete (*itTENEMY).second;

	for( itTENEMY = m_pTRSCS->m_vTCORPS.m_mapPC.begin(); itTENEMY != m_pTRSCS->m_vTCORPS.m_mapPC.end(); itTENEMY++)
		delete (*itTENEMY).second;

	m_pTRSCS->m_vTCORPS.m_mapFIXRECALL.clear();
	m_pTRSCS->m_vTCORPS.m_mapRECALL.clear();
	m_pTRSCS->m_vTCORPS.m_mapMON.clear();
	m_pTRSCS->m_vTCORPS.m_mapPC.clear();

	while(!m_pTRSCS->m_vTCORPS.m_vTSQUAD.empty())
	{
		delete m_pTRSCS->m_vTCORPS.m_vTSQUAD.back();
		m_pTRSCS->m_vTCORPS.m_vTSQUAD.pop_back();
	}

	m_pTRSCS->m_vTCORPS.m_wID = 0;
	m_pTRSCS->m_pTCURSQUAD = NULL;
	m_pTRSCS->m_pTCURUNIT = NULL;
	m_pTRSCS->ResetCORPS();

	m_pMainChar->m_wCommanderID = 0;
	ResetTRSCSCMD(
		&m_vTCMDTARGET,
		&m_vTCMDDIR,
		&m_vTCMDARROW,
		TRSCSCMD_NONE,
		0, 0, 0, 0);
}

void CTClientGame::ReportENEMY()
{
	MAPMONSTER::iterator itMON;
	MAPPLAYER::iterator itPC;
	MAPRECALL::iterator itRECALL;

	for( itRECALL = m_mapFIXRECALL.begin(); itRECALL != m_mapFIXRECALL.end(); itRECALL++)
		if(IsRSCSEnemy((*itRECALL).second))
			RelaySession()->SendCS_ADDCORPSENEMY_REQ( m_pMainChar->m_dwID, m_vMAP.m_wMapID, (*itRECALL).second);

	for( itRECALL = m_mapRECALL.begin(); itRECALL != m_mapRECALL.end(); itRECALL++)
		if(IsRSCSEnemy((*itRECALL).second))
			RelaySession()->SendCS_ADDCORPSENEMY_REQ( m_pMainChar->m_dwID, m_vMAP.m_wMapID, (*itRECALL).second);

	for( itMON = m_mapMONSTER.begin(); itMON != m_mapMONSTER.end(); itMON++)
		if(IsRSCSEnemy((*itMON).second))
			RelaySession()->SendCS_ADDCORPSENEMY_REQ( m_pMainChar->m_dwID, m_vMAP.m_wMapID, (*itMON).second);

	for( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++)
		if(IsRSCSEnemy((*itPC).second))
			RelaySession()->SendCS_ADDCORPSENEMY_REQ( m_pMainChar->m_dwID, m_vMAP.m_wMapID, (*itPC).second);
}

void CTClientGame::ReportTRSCSMOVE( CTClientMoveObj *pTOBJ)
{
	switch(pTOBJ->m_bTCORPSMOVE)
	{
	case TCORPSMOVE_ENEMY	:
		{
			RelaySession()->SendCS_MOVECORPSENEMY_REQ( m_pMainChar->m_dwID, pTOBJ, m_vMAP.m_wMapID);
			pTOBJ->m_bTCORPSMOVE = TCORPSMOVE_NONE;
		}

		break;

	case TCORPSMOVE_UNIT	:
		{
			WORD wSquadID = GetPartyID(pTOBJ);

			if(wSquadID)
				RelaySession()->SendCS_MOVECORPSUNIT_REQ( wSquadID, m_vMAP.m_wMapID, m_vMAP.m_dwBspID, pTOBJ);

			pTOBJ->m_bTCORPSMOVE = TCORPSMOVE_NONE;
		}

		break;
	}
}

WORD CTClientGame::GetPursuit( CTClientObjBase *pTOBJ)
{
	if( !pTOBJ || pTOBJ != m_pMainChar )
		return 0xFFFF;

	for( BYTE i=0; i<BYTE(pTOBJ->m_vTSKILLTARGET.size()); i++)
		if( pTOBJ->m_vTSKILLTARGET[i]->m_bType == OT_MON )
		{
			CTClientMonster *pTMON = FindMonster(pTOBJ->m_vTSKILLTARGET[i]->m_dwID);

			if( pTMON && pTMON->m_pMON )
				return pTMON->m_pMON->m_wKind;
		}

	return 0xFFFF;
}

WORD CTClientGame::GetEaseInvenPos( CTClientItem *pTITEM)
{
	MAPTINVEN::iterator it = m_pMainChar->m_mapTINVEN.find(INVEN_DEFAULT);

	if( it != m_pMainChar->m_mapTINVEN.end() && (*it).second )
	{
		BYTE bItemID = (*it).second->GetEasePos(pTITEM);

		if( bItemID != INVALID_SLOT )
			return MAKEWORD( bItemID, (*it).first);
	}

	for( it = m_pMainChar->m_mapTINVEN.begin(); it != m_pMainChar->m_mapTINVEN.end(); it++)
		if( (*it).first != INVEN_DEFAULT && (*it).first != INVEN_EQUIP )
		{
			BYTE bItemID = (*it).second->GetEasePos(pTITEM);

			if( bItemID != INVALID_SLOT )
				return MAKEWORD( bItemID, (*it).first);
		}

	return MAKEWORD( INVALID_SLOT, INVALID_SLOT);
}

WORD CTClientGame::GetBlankInvenPos()
{
	MAPTINVEN::iterator it = m_pMainChar->m_mapTINVEN.find(INVEN_DEFAULT);

	if( it != m_pMainChar->m_mapTINVEN.end() && (*it).second )
	{
		BYTE bItemID = (*it).second->GetBlankPos();

		if( bItemID != INVALID_SLOT )
			return MAKEWORD( bItemID, (*it).first);
	}

	for( it = m_pMainChar->m_mapTINVEN.begin(); it != m_pMainChar->m_mapTINVEN.end(); it++)
		if( (*it).first != INVEN_DEFAULT && (*it).first != INVEN_EQUIP )
		{
			BYTE bItemID = (*it).second->GetBlankPos();

			if( bItemID != INVALID_SLOT )
				return MAKEWORD( bItemID, (*it).first);
		}

	return MAKEWORD( INVALID_SLOT, INVALID_SLOT);
}

WORD CTClientGame::GetBlankInvenPosExceptInven( BYTE bExceptedInven )
{
	if( bExceptedInven != INVEN_DEFAULT )
	{
		MAPTINVEN::iterator it = m_pMainChar->m_mapTINVEN.find(INVEN_DEFAULT);

		if( it != m_pMainChar->m_mapTINVEN.end() && (*it).second )
		{
			BYTE bItemID = (*it).second->GetBlankPos();

			if( bItemID != INVALID_SLOT )
				return MAKEWORD( bItemID, (*it).first);
		}
	}

	for( MAPTINVEN::iterator it = m_pMainChar->m_mapTINVEN.begin(); it != m_pMainChar->m_mapTINVEN.end(); it++)
		if( (*it).first != INVEN_DEFAULT &&
			(*it).first != bExceptedInven &&
			(*it).first != INVEN_EQUIP )
		{
			BYTE bItemID = (*it).second->GetBlankPos();

			if( bItemID != INVALID_SLOT )
				return MAKEWORD( bItemID, (*it).first);
		}

		return MAKEWORD( INVALID_SLOT, INVALID_SLOT);
}

void CTClientGame::DoDEFACTION()
{
	if(!m_pTARGET)
		return;

	switch(m_pTARGET->m_bType)
	{
	case OT_NPC:
		{
			CTClientNpc* pTNPC = static_cast<CTClientNpc*>( m_pTARGET );

			if( pTNPC->m_bHasQuest == THASQUEST_NONE &&
				pTNPC->m_dwExecID )
			{
				OnCommand( pTNPC->m_dwExecID);
			}
			else
			{
				OnCommand( GM_NPC_TALK );
			}
		}

		break;

	case OT_MON		:
		if(!m_pMainChar->IsAlliance(m_pTARGET))
			if(!m_pTARGET->IsDead())
			{
				m_wSkillID = 31;
				OnGM_ATTACK();
			}
			else
				OnGM_OPEN_MONINVEN();

		break;

	case OT_RECALL	:
	case OT_PC		:
		if( !m_pMainChar->IsAlliance(m_pTARGET) && !m_pTARGET->IsDead() )
		{
			m_wSkillID = 31;
			OnGM_ATTACK();
		}
		else if( ((CTClientChar*)m_pTARGET)->m_bPrivateShop && !m_pTARGET->IsDead() )
		{
//			ResetTargetOBJ(m_pTARGET);
			OnGM_OPEN_PRIVSHOP_FOR_BUY();
		}

		break;

	case OT_SWITCH	: OnGM_ACTIVATE_SWITCH(); break;
	case OT_GODBALL : OnGM_PICKUP_BALL(); break;
	case OT_GODTOWER :
		{
			if( !m_pTARGET->IsDead() )
				OnGM_MOUNT_BALL2_TOWER();
		}
		break;
	}
}

BYTE CTClientGame::GetMapDETAILOption()
{
	return min( m_bTOPTIONLEVEL, m_vTOPTION.m_bMapDETAIL);
}

FLOAT CTClientGame::GetObjDETAILOption()
{
	static FLOAT fLevel[TDETAIL_COUNT] = { 0.9f, 1.4f, 5.0f};
	return min( fLevel[m_bTOPTIONLEVEL], fLevel[m_vTOPTION.m_bObjDETAIL]);
}

BYTE CTClientGame::GetMapSHADOWOption()
{
	static BYTE bENABLE[TOPTIONLEVEL_COUNT] = {
		FALSE,
		TRUE,
		TRUE};

	return m_vTOPTION.m_bMapSHADOW && bENABLE[m_bTOPTIONLEVEL] ? TRUE : FALSE;
}

BYTE CTClientGame::GetObjSHADOWOption()
{
	static BYTE bENABLE[TOPTIONLEVEL_COUNT] = {
		FALSE,
		TRUE,
		TRUE};

	return m_vTOPTION.m_bObjSHADOW && bENABLE[m_bTOPTIONLEVEL] ? TRUE : FALSE;
}

BYTE CTClientGame::GetMapSFXOption()
{
	static BYTE bENABLE[TOPTIONLEVEL_COUNT] = {
		FALSE,
		FALSE,
		TRUE};

	return m_vTOPTION.m_bMapSFX && bENABLE[m_bTOPTIONLEVEL] ? TRUE : FALSE;
}

BYTE CTClientGame::GetNpcNAMEOption()
{
	return m_vTOPTION.m_bNpcNAME;
}

BYTE CTClientGame::GetMonNAMEOption()
{
	return m_vTOPTION.m_bMonNAME;
}

BYTE CTClientGame::GetPcNAMEOption()
{
	return m_vTOPTION.m_bPcNAME;
}

BYTE CTClientGame::GetMainVolumeOption()
{
	return CTachyonMedia::m_bMasterVolume;
}

BYTE CTClientGame::GetBGMVolumeOption()
{
	return CT3DMusic::m_bMasterVolume;
}

BYTE CTClientGame::GetSFXVolumeOption()
{
	return CD3DSound::m_bMasterVolume;
}

BYTE CTClientGame::GetLIGHTMAPOption( BYTE bDungeon)
{
	static BYTE bENABLE[TOPTIONLEVEL_COUNT][2] = {
		{ FALSE, FALSE},
		{ TRUE, FALSE},
		{ TRUE, TRUE}};

	return ((bDungeon && m_vTOPTION.m_bDungeonLIGHTMAP) || (!bDungeon && m_vTOPTION.m_bFieldLIGHTMAP)) && bENABLE[m_bTOPTIONLEVEL][bDungeon ? 0 : 1] ? TRUE : FALSE;
}

BYTE CTClientGame::GetFarIMAGEOption()
{
	static BYTE bENABLE[TOPTIONLEVEL_COUNT] = {
		FALSE,
		TRUE,
		TRUE};

	return m_vTOPTION.m_bFarIMAGE && bENABLE[m_bTOPTIONLEVEL] ? TRUE : FALSE;
}

BYTE CTClientGame::GetHUDOption()
{
	return m_vTOPTION.m_bHUD;
}

BYTE CTClientGame::GetSNDOption()
{
	return CD3DSound::m_bON;
}

BYTE CTClientGame::GetBGMOption()
{
	return CT3DMusic::m_bON;
}

FLOAT CTClientGame::GetOBJRangeOption()
{
	return GetInstance()->m_pDevice->m_lVIDEOMEM < 256 ? min( m_vTOPTION.m_fOBJRange, 0.7f) : m_vTOPTION.m_fOBJRange;
}

CString CTClientGame::ToTimeString(DWORD dwTick)
{
	CString strResult;

	dwTick += 1000;

	DWORD dwT = dwTick / 3600000;
	if( dwT )
	{
		strResult = CTChart::Format( TSTR_FMT_TIME_HOUR, dwT);
		return strResult;
	}

	DWORD dwL = dwTick % 3600000;
	dwT = dwL / 60000;
	dwL = dwL % 60000;
	if( dwT )
	{
		strResult = CTChart::Format( TSTR_FMT_TIME_MINUTE, dwT, dwL / 1000);
		return strResult;
	}
	
	strResult = CTChart::Format( TSTR_FMT_TIME_SECOND, dwL / 1000);

	return strResult;
}

CString CTClientGame::ToTimeOneString(DWORD dwTick)
{
	CString strResult;

	dwTick += 1000;

	DWORD dwD = dwTick / 86400000;
	if( dwD )
	{
		strResult = CTChart::Format( TSTR_FMT_TIME_ONEDAY, dwD);
		return strResult;
	}

	DWORD dwT = dwTick / 3600000;
	if( dwT )
	{
		strResult = CTChart::Format( TSTR_FMT_TIME_ONEHOUR, dwT);
		return strResult;
	}

	DWORD dwL = dwTick % 3600000;
	dwT = dwL / 60000;
	if( dwT )
	{
		strResult = CTChart::Format( TSTR_FMT_TIME_ONEMINUTE, dwT);
		return strResult;
	}
	
	dwL = dwL % 60000;
	strResult = CTChart::Format( TSTR_FMT_TIME_SECOND, dwL / 1000);

	return strResult;
}

CString CTClientGame::OnActCmdByMsg(const CString& strMSG, const VECTORSTRING& vPARAM, DWORD dwUSER)
{
	CTClientGame* pGame = CTClientGame::GetInstance();
	pGame->DoActCmd(strMSG);
	return CString("");
}

void CTClientGame::OnActionCmdBtnDown(CTMiniPopupDlg* pPopupDlg, CTMiniPopupDlg::CTButtonMP* pButton)
{
	CTClientGame* pGame = static_cast<CTClientGame*>(pPopupDlg->GetParent());
	pGame->DoActCmd( pButton->m_strText );
}

BOOL CTClientGame::ShowChatDetailInfo(CTClientItem* pClientItem)
{
	CTDetailInfoDlg* pDetailInfo = 
		static_cast<CTDetailInfoDlg*>( m_vTFRAME[TFRAME_CHAT_DETAIL_INFO] );

	ITDetailInfoPtr pInfo = CTDetailInfoManager::NewItemInst(pClientItem);
	pDetailInfo->ResetINFO(pInfo);

	CRect rc;
	pDetailInfo->GetComponentRect(&rc);

	INT x, y;
	if( m_bNeedCharDetInfoInit )
	{
		x = (m_pDevice->m_option.m_dwScreenX - rc.Width()) / 2;
		y = m_pDevice->m_option.m_dwScreenY - rc.Height() - TCHAT_DETAIL_INFO_MARGINE;

		pDetailInfo->MoveComponent( CPoint(x,y) );
		m_bNeedCharDetInfoInit = FALSE;
	}
	else
	{
		if( rc.bottom > m_pDevice->m_option.m_dwScreenY )
		{
			x = rc.left;
			y = rc.top - rc.bottom + m_pDevice->m_option.m_dwScreenY;
			
			pDetailInfo->MoveComponent( CPoint(x,y) );
		}
	}
	
	EnableUI(TFRAME_CHAT_DETAIL_INFO);

	return TRUE;
}

TComponent* CTClientGame::GetTopFrame(const CPoint& pt, UINT nFromTop, BYTE* outFrameID)
{
	if( m_pTOPFRAME )
	{
		if( outFrameID )
			*outFrameID = m_bTOPFRAMEID;

		return m_pTOPFRAME;
	}

	TComponent* pHIT = NULL;

	TCOMP_LIST::reverse_iterator itr,end;
	itr = m_kids.rbegin();
	end = m_kids.rend();
	for( ; itr != end; ++itr )
	{
		if((*itr)->HitTest(pt))
		{
			if( nFromTop == 0 )
			{
				pHIT = *itr;
				break;
			}
			else
				--nFromTop;
		}
	}

	if( !pHIT )
		return NULL;

	BYTE dwFID = (BYTE)T_INVALID;
	dwFID = GetFrameID(pHIT);
	if( dwFID == (BYTE)T_INVALID )
		return NULL;

	if( outFrameID )
		*outFrameID = dwFID;

	m_bTOPFRAMEID = dwFID;
	m_pTOPFRAME = pHIT;

	return pHIT;
}

BYTE CTClientGame::GetFrameID(TComponent* pComp)
{
	for( BYTE i=0; i<TFRAME_COUNT; ++i)
	{
		if( pComp == m_vTFRAME[i] )
			return i;
	}

	return (BYTE) T_INVALID;
}

TComponent* CTClientGame::GetHitPartyItemLottery( CPoint point )
{
	INT nActCnt = CTPartyItemLottery::GetActiveInstCnt();
	for(INT i=0; i<nActCnt; ++i)
	{
		TComponent* pCand = CTPartyItemLottery::GetActiveInst(i);
		if( pCand->HitTest(point) )
			return pCand;
	}		

	return NULL;
}

void CTClientGame::TCapture()
{
	CString strFileName = m_pMainWnd->CaptureIMG();
	CString strTYPE;
	CString strMSG;

	if(!strFileName.IsEmpty())
	{
		strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);
		strMSG = CTChart::Format( TSTR_FMT_CAPTUREMSG, strFileName);

		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, TCOLOR_INFO, TCHAT_FLAG_INFO);
	}
}

void CTClientGame::DoTLOADCMD_OBJ( CTClientApp *pTAPP,
								   LPTLOADCMD pTCMD)
{
	m_vMAP.DoTLOADCMD_OBJ(
		m_pDevice,
		m_pRES,
		pTAPP,
		(LPTLOADOBJ) pTCMD->m_pTBUF);
}

void CTClientGame::ShowInfoChat(const CString& strMSG, TUISND_TYPE eSndType)
{
	if( m_pChatFrame )
	{
		static CString strTYPE;
		if( strTYPE.IsEmpty() )
			strTYPE = CTChart::LoadString( TSTR_INFO_TITLE);

		DWORD dwChatColor;
		DWORD dwSoundID;
		BYTE bSoundType;

		switch( eSndType )
		{
		case TUISND_TYPE_INFO:
			{
				dwChatColor = TCOLOR_INFO;
				dwSoundID = ID_SND_INFO;
				bSoundType = MEDIA_TSOUND;
			}
			break;
		case TUISND_TYPE_ERROR:
			{
				dwChatColor = TCOLOR_ERROR;
				dwSoundID = ID_SND_ERROR;
				bSoundType = MEDIA_TSOUND;
			}
			break;
		default:
			{
				dwChatColor = TCOLOR_INFO;
				dwSoundID = 0;
				bSoundType = MEDIA_TSOUND;
			}
			break;
		}

		m_pChatFrame->ChatSysMSG( strTYPE, m_pMainChar->GetName(), strMSG, 
			dwChatColor, TCHAT_FLAG_INFO, dwSoundID, bSoundType);
	}
}

void CTClientGame::ShowInfoMsgBox(const CString& strMSG, BOOL bModal, TUISND_TYPE eSndType)
{
	DWORD dwSoundID;

	switch( eSndType )
	{
	case TUISND_TYPE_INFO:
		dwSoundID = ID_SND_INFO;
		break;

	case TUISND_TYPE_ERROR:
		dwSoundID = ID_SND_ERROR;
		break;

	default:
		dwSoundID = 0;
		break;
	}

	m_pMainWnd->MessageBoxOK(
		strMSG,
		TSTR_OK,
		GM_CLOSE_MSGBOX,
		0,
		bModal,
		dwSoundID,
		MEDIA_TSOUND);
}

void CTClientGame::DoMainRunAway(BOOL bSTART)
{
	m_bUseLButtonDblClick = !bSTART;
	m_bUseMButtonDblClick = !bSTART;
	m_bUseRButtonDblClick = !bSTART;
	m_bUseLButtonClick = !bSTART;
	m_bUseMButtonClick = !bSTART;
	m_bUseRButtonClick = !bSTART;
	m_bUseMButtonDown = !bSTART;
	m_bUseRButtonDown = !bSTART;
	m_bUseKeyboardInput = !bSTART;

	m_bCanUseSkill = !bSTART;
	m_bCanUseItem = !bSTART;

	m_bMoveType = TMAINMOVE_NORMAL;
	m_bAutoRun = FALSE;
	m_dwMoveGM = 0;
	m_nRunAwayIndex = 0;

	m_vRunAway.clear();
	if(bSTART)
	{
		for(int i=0; i<TRUNAWAYPOINT_COUNT; ++i)
		{
			TRUNAWAY data;

			data.m_dwTime = (rand()%(TRUNAWAYTIME_MAX-TRUNAWAYTIME_MIN)) + TRUNAWAYTIME_MIN;
			data.m_ptScreen.x = rand() % m_pDevice->m_option.m_dwScreenX;
			data.m_ptScreen.y = rand() % m_pDevice->m_option.m_dwScreenY;

			m_vRunAway.push_back(data);
		}

		MoveMainChar( CPoint(
			rand() % m_pDevice->m_option.m_dwScreenX,
			rand() % m_pDevice->m_option.m_dwScreenY),
			FALSE);
	}
}

void CTClientGame::DoRecallRunAway( CTClientRecall* pTRECALL,
								   BOOL bSTART )
{
	pTRECALL->m_nRecallRunAwayIndex = 0;
	pTRECALL->m_vRecallRunAway.clear();
	pTRECALL->m_vRecallRunAwayTarget = D3DXVECTOR3(0.0f,0.0f,0.0f);

	if( bSTART )
	{
		for(int i=0; i<TRUNAWAYPOINT_COUNT; ++i)
		{
			TRUNAWAY data;

			data.m_dwTime = (rand()%(TRUNAWAYTIME_MAX-TRUNAWAYTIME_MIN)) + TRUNAWAYTIME_MIN;
			data.m_ptScreen.x = rand() % m_pDevice->m_option.m_dwScreenX;
			data.m_ptScreen.y = rand() % m_pDevice->m_option.m_dwScreenY;

			pTRECALL->m_vRecallRunAway.push_back(data);
		}

		pTRECALL->m_vRecallRunAwayTarget = ScreenClickToWorldPosition(
			pTRECALL->m_vRecallRunAway[0].m_ptScreen,
            pTRECALL);

		pTRECALL->m_nRecallRunAwayIndex = 1;
	}
}

void CTClientGame::CalcMainRunAway( DWORD dwTick)
{
	if(m_vRunAway.empty())
		return;
	LPTRUNAWAY pDATA = &m_vRunAway[m_nRunAwayIndex];

	if( pDATA->m_dwTime <= dwTick )
	{
		MoveMainChar(pDATA->m_ptScreen,FALSE);

		if( m_nRunAwayIndex >= TRUNAWAYPOINT_COUNT-1 )
			m_nRunAwayIndex = 0;
		else
			++m_nRunAwayIndex;

		pDATA->m_dwTime = (rand() % (TRUNAWAYTIME_MAX - TRUNAWAYTIME_MIN)) + TRUNAWAYTIME_MIN;
		pDATA->m_ptScreen.x = rand() % m_pDevice->m_option.m_dwScreenX;
		pDATA->m_ptScreen.y = rand() % m_pDevice->m_option.m_dwScreenY;
	}
	else
		pDATA->m_dwTime -= dwTick;
}

void CTClientGame::MoveMainChar( const CPoint &ptScreen, BOOL bShowPointer )
{
	D3DXVECTOR3 vTARGET = m_pCamera->m_vPosition + m_pCamera->GetRayDirection( ptScreen.x, ptScreen.y);
	D3DXVECTOR3 vSTART = m_pCamera->m_vPosition;
	D3DXVECTOR3 vDIR;
	D3DXPLANE vPLANE;

	m_pCtrlHost = GetCtrlOBJ();
	D3DXPlaneFromPointNormal(
		&vPLANE,
		&D3DXVECTOR3( 0.0f, m_pCtrlHost->GetPositionY(), 0.0f),
		&D3DXVECTOR3( 0.0f, 1.0f, 0.0f));

	if( CTMath::PlaneIntersectLine(
		&vDIR,
		&vSTART,
		&vTARGET,
		&vPLANE))
	{
		D3DXVECTOR3 vPOS(
			m_pCtrlHost->GetPositionX(),
			m_pCtrlHost->GetPositionY(),
			m_pCtrlHost->GetPositionZ());

		if( D3DXVec3Dot( &(vTARGET - vSTART), &(vDIR - vSTART)) < 0.0f )
		{
			vDIR -= vPOS;
			vDIR = -vDIR;
		}
		else
			vDIR -= vPOS;

		WORD wDIR = m_pCtrlHost->GetTargetDIR(
			vDIR.x,
			vDIR.z);

		if( wDIR != m_pCtrlHost->m_wDIR )
		{
			D3DXMATRIX vROT;

			vPOS = D3DXVECTOR3( 1.0f, 0.0f, 0.0f);
			D3DXMatrixRotationY(
				&vROT,
				FLOAT(wDIR) * D3DX_PI / 900);

			CTMath::Transform(
				&vROT,
				&vPOS);

			D3DXPlaneFromPoints(
				&m_vBOUND,
				&vSTART,
				&vTARGET,
				&(vTARGET + vPOS));

			m_pCtrlHost->m_bMouseDIR = TKDIR_F;
			m_pCtrlHost->m_bKeyDIR = TKDIR_F;
			m_pCtrlHost->SetTAction(m_pCtrlHost->GetTAction());

			m_dwSYNCTick = SYNC_TICK;
			MoveCtrlHost(
				m_pCtrlHost->m_wPITCH,
				wDIR,
				m_pCtrlHost->m_bMouseDIR,
				m_pCtrlHost->m_bKeyDIR);
			m_pCtrlHost->CalcDIR( wDIR, wDIR, m_pCtrlHost->m_wPITCH);

			if(bShowPointer)
				ResetTPOINTER( ptScreen.x, ptScreen.y);
			LoopSkillOFF();

			m_bMoveType = TMAINMOVE_CLICK;
			m_dwMoveGM = 0;
			m_bAutoRun = FALSE;

			if( GetPartyID(m_pMainChar) && CanControl() )
				m_pMainChar->m_bTCORPSMOVE = TCORPSMOVE_UNIT;
		}
	}
}

void CTClientGame::GetRandomPosition(FLOAT& fPosX, 
									 FLOAT& fPosY, 
									 FLOAT& fPosZ, 
									 CTClientObjBase* pTOBJ,
									 FLOAT fRange, 
									 FLOAT fMaxOffset)
{
	FLOAT fCurH = m_vMAP.GetHeight(
		pTOBJ,
		NULL,
		&D3DXVECTOR3(
		fPosX,
		fPosY,
		fPosZ),
		0.0f, FALSE);

	for(int i=0; i<TMAX_RAND_POS; ++i)
	{
		FLOAT fNewPosX = fPosX + (FLOAT((rand() % (int)(fRange*200)) - (int)(fRange*100)) / 100.0f);
		FLOAT fNewPosZ = fPosZ + (FLOAT((rand() % (int)(fRange*200)) - (int)(fRange*100)) / 100.0f);

		FLOAT fNewH = m_vMAP.GetHeight(
			pTOBJ,
			NULL,
			&D3DXVECTOR3(
			fNewPosX,
			fPosY,
			fNewPosZ),
			0.0f, FALSE);

		if( fabs(fCurH-fNewH) < fMaxOffset )
		{
			fPosX = fNewPosX;
			fPosZ = fNewPosZ;
			
			return;
		}
	}
}

bool CTClientGame::GetSafeMainPos(int& outMap, float& outX, float& outY)
{
	try
	{
		CTClientGame* pGAME = GetInstance();

		outMap = pGAME->m_vMAP.m_wMapID;
		outX = pGAME->m_pMainChar->GetPositionX();
		outY = pGAME->m_pMainChar->GetPositionZ();

		return true;
	}
	catch(...)
	{
		return false;
	}

	return false;
}

bool CTClientGame::GetSafeMainTarget(CString& outType, unsigned int& outID)
{
	try
	{
		CTClientGame* pGAME = GetInstance();
		
		if( pGAME->m_pTARGET )
		{
			switch( pGAME->m_pTARGET->m_bType )
			{
			case OT_NONE		: outType = "OT_NONE";			break;
			case OT_PC			: outType = "OT_PC";			break;	
			case OT_MON			: outType = "OT_MON";			break;
			case OT_NPC			: outType = "OT_NPC";			break;
			case OT_FOG			: outType = "OT_FOG";			break;
			case OT_WATER		: outType = "OT_WATER";			break;
			case OT_COLLISION	: outType = "OT_COLLISION";		break;
			case OT_RECALL		: outType = "OT_RECALL";		break;
			case OT_BSP			: outType = "OT_BSP";			break;
			case OT_SWITCH		: outType = "OT_SWITCH";		break;
			case OT_GATE		: outType = "OT_GATE";			break;
			case OT_SELF		: outType = "OT_SELF";			break;
			case OT_SILHOUETTE	: outType = "OT_SILHOUETTE";	break;
			case OT_PATHWALL	: outType = "OT_PATHWALL";		break;
			default				: outType = "unknown";			break;
			}

			outID = pGAME->m_pTARGET->m_dwID;
			return true;
		}
	}
	catch(...)
	{
		return false;
	}

	return false;
}

CTClientSession * CTClientGame::RelaySession()
{
	if(m_pRelay)
	{
		m_dwRelayTick = 0;
		return m_pRelay;
	}
	else
		return m_pSESSION;
}

#ifdef TEST_MODE
void CTClientGame::SetMonRoaming( BOOL bMonRoaming)
{
	m_bMonRoaming = bMonRoaming;

	if(!bMonRoaming)
	{
		MAPMONSTER::iterator itMON;

		for( itMON = m_mapSLAVEMON.begin(); itMON != m_mapSLAVEMON.end(); itMON++)
			if( (*itMON).second && (*itMON).second->m_pMON && (*itMON).second->m_pMON->m_bApplyAI )
			{
				CTClientMonster *pMONSTER = (*itMON).second;

				if( pMONSTER->m_bFollowType != OT_NONE ) // 무언가를 따라가는 몬스터는 그 대상과 해체!
				{
					CTClientObjBase *pFollow = FindOBJ(
						pMONSTER->m_dwFollowID,
						pMONSTER->m_bFollowType);

					if(pFollow)
					{
						SendCS_LEAVELB_REQ(
							pFollow->GetHostID(),
							pFollow->m_dwID,
							pFollow->m_bType,
							pMONSTER->m_dwID,
							m_pMainWnd->m_bChannel,
							m_vMAP.m_wMapID);
					}

					pMONSTER->m_bFollowType = OT_NONE;
					pMONSTER->m_dwFollowID = 0;
					pMONSTER->m_bTD = TD_NONE;
				}

				if( pMONSTER->m_bFollowType == OT_NONE && pMONSTER->IsRoam() )
				{
					pMONSTER->m_bMouseDIR = TKDIR_N;
					pMONSTER->m_bKeyDIR = TKDIR_N;
					pMONSTER->SetTAction(TA_STAND);

					SendCS_MONMOVE_REQ(
						pMONSTER,
						m_pMainWnd->m_bChannel,
						m_vMAP.m_wMapID,
						pMONSTER->GetPositionX(),
						pMONSTER->GetPositionY(),
						pMONSTER->GetPositionZ(),
						pMONSTER->m_wPITCH,
						pMONSTER->m_wDIR,
						pMONSTER->m_bMouseDIR,
						pMONSTER->m_bKeyDIR,
						pMONSTER->m_bAction);
				}
			}
	}
}

void CTClientGame::ReleaseTSPAWN()
{
	MAPTSTRING::iterator it;

	for( it = m_mapTSPAWN.begin(); it != m_mapTSPAWN.end(); it++)
		delete (*it).second;

	m_mapTSPAWN.clear();
}

void CTClientGame::BeginEditTSPAWN()
{
	m_bEditTSPAWN = TRUE;
	ReleaseTSPAWN();

	CTClientGame::DebugMSG(CString(_T("Edit TSPAWN Started")));
}

void CTClientGame::EndEditTSPAWN()
{
	CStdioFile vDEST( "TSPAWN.txt", CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	MAPTSTRING::iterator it;

	for( it = m_mapTSPAWN.begin(); it != m_mapTSPAWN.end(); it++)
	{
		vDEST.WriteString(LPCSTR((*(*it).second)));
		delete (*it).second;
	}

	m_bEditTSPAWN = FALSE;
	m_mapTSPAWN.clear();

	CTClientGame::DebugMSG(CString(_T("Edit TSPAWN Stopped")));
}

void CTClientGame::SaveEditTSPAWN()
{
	if(!m_bEditTSPAWN)
		return;

	CStdioFile vDEST( "TSPAWN.txt", CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	MAPTSTRING::iterator it;

	for( it = m_mapTSPAWN.begin(); it != m_mapTSPAWN.end(); it++)
		vDEST.WriteString(LPCSTR((*(*it).second)));

	CTClientGame::DebugMSG(CString(_T("Edit TSPAWN Saved")));
}

void CTClientGame::AddTSPAWN( CTClientObjBase *pTARGET,
							  FLOAT fPosX,
							  FLOAT fPosY,
							  FLOAT fPosZ,
							  WORD wDIR)
{
	if( !m_bEditTSPAWN || !pTARGET || pTARGET->m_bType != OT_MON )
		return;

	WORD wSpawnID = HIWORD(pTARGET->m_dwID);
	CString *pTSPAWN = NULL;

	MAPTSTRING::iterator finder = m_mapTSPAWN.find(wSpawnID);
	if( finder == m_mapTSPAWN.end() )
	{
		pTSPAWN = new CString();
		m_mapTSPAWN.insert( MAPTSTRING::value_type( wSpawnID, pTSPAWN));
	}
	else
		pTSPAWN = (*finder).second;

	(*pTSPAWN).Format(
		_T("%d\t%.2f\t%.2f\t%.2f\t%d\n"),
		wSpawnID,
		fPosX,
		fPosY,
		fPosZ,
		wDIR);

	((CTClientMoveObj *) pTARGET)->m_nDIR = INT(wDIR) - INT(pTARGET->m_wDIR);
	pTARGET->SetPosition( D3DXVECTOR3(
		fPosX,
		fPosY,
		fPosZ));
	pTARGET->m_vWorld._41 = fPosX;
	pTARGET->m_vWorld._42 = fPosY;
	pTARGET->m_vWorld._43 = fPosZ;

	if( m_mapSLAVEMON.find(pTARGET->m_dwID) != m_mapSLAVEMON.end() )
	{
		SendCS_MONMOVE_REQ(
			pTARGET,
			m_pMainWnd->m_bChannel,
			m_vMAP.m_wMapID,
			fPosX,
			fPosY,
			fPosZ,
			((CTClientMoveObj *) pTARGET)->m_wPITCH,
			wDIR,
			((CTClientMoveObj *) pTARGET)->m_bMouseDIR,
			((CTClientMoveObj *) pTARGET)->m_bKeyDIR,
			((CTClientMoveObj *) pTARGET)->m_bAction);
	}

	CTClientGame::DebugMSG(CString(_T("TSPAWN : ")) + (*pTSPAWN));
}
#endif

void CTClientGame::DebugMSG( const CString& strMSG, BOOL bPrintChat, BOOL bFile, LPCTSTR szPATH )
{
#ifdef TEST_MODE
	static INT nLine=0;
	static DWORD dwPrevTime = timeGetTime();
	DWORD dwTime = timeGetTime();

	CString strRESULT;
	strRESULT.Format( "%d\t %s",
		nLine++,
		(LPCTSTR)strMSG);

	if( m_pChatFrame && bPrintChat )
		m_pChatFrame->ChatSysMSG(
			"디버그",
			"디버그",
			strRESULT,
			0xFFFFFFFF,
			TCHAT_FLAG_INFO);

	if( bFile )
	{
		FILE* fp = fopen(szPATH,"at");
		if( !fp )
			return ;
		fprintf( fp, "%u\t%u\t%s\n", dwTime, dwTime-dwPrevTime, (LPCTSTR)strRESULT );
		fclose(fp);
	}

	dwPrevTime = dwTime;
#endif
}

BOOL CTClientGame::IsMoveOBJ( BYTE bType)
{
	switch(bType)
	{
	case OT_RECALL:
	case OT_SELF:
	case OT_MON:
	case OT_PC:
		return TRUE;

	case OT_SWITCH:
	case OT_GATE:
		return FALSE;
	}

	return FALSE;
}

D3DXVECTOR3 CTClientGame::ScreenClickToWorldPosition(
	CPoint ptScreen,
	CTClientObjBase *pOBJ)
{
	INT nPosX = ptScreen.x;
	INT nPosY = ptScreen.y;

	D3DXVECTOR3 vPOS = m_pCamera->GetCameraPosition( nPosX, nPosY);
	D3DXVECTOR3 vDIR = m_pCamera->GetRayDirection( nPosX, nPosY);
	FLOAT fDIST = 0.0f;

	vDIR /= D3DXVec3Length(&vDIR);
	D3DXVECTOR3 vTARGET = vPOS + TARGET_DIST * vDIR;

	if(m_vMAP.HitTest( &fDIST, &vPOS, &vTARGET, 0.0f, 0.0f, FALSE))
	{
		vPOS += fDIST * vDIR;
		vPOS.y += pOBJ->m_fSizeY;

		vPOS.y = m_vMAP.GetHeight(
			pOBJ,
			NULL,
			&vPOS,
			0.0f,
			FALSE);

		return vPOS;
	}

	return pOBJ->GetPosition();
}

void CTClientGame::UpdateNameDetailTick( DWORD dwTick)
{
	m_dwNameTick += dwTick;
}

void CTClientGame::UpdateDetailNameInfo( CPoint pt)
{
	static BOOL bShowNameDetail = FALSE;

	if( m_ptPrevPos != pt )
	{
		m_dwNameTick = 0;
		m_ptPrevPos = pt;
		bShowNameDetail = FALSE;
		DisableUI(TFRAME_DETAIL_NAME);
		return ;
	}

	if( m_dwNameTick > TDETAIL_NAME_TICK &&
		bShowNameDetail == FALSE )
	{
		m_dwNameTick = 0;
		bShowNameDetail = TRUE;

		TCOMP_LIST::reverse_iterator itr,end;
		itr = m_kids.rbegin();
		end = m_kids.rend();
		for( ; itr != end; ++itr )
		{
			TComponent* pKid = (*itr);

			if( pKid->IsVisible() )
			{
				TComponent* pTARGET = pKid->FindShrinkInChilds( pt );
				if( pTARGET )
				{
					CTDetailNameDlg* pDLG = static_cast<CTDetailNameDlg*>( m_vTFRAME[TFRAME_DETAIL_NAME] );
					pDLG->ResetText( pTARGET->m_strText);
					pDLG->SetPos( pt );
					EnableUI(TFRAME_DETAIL_NAME);
					return ;
				}
			}
		}
	}
}

void CTClientGame::ChangeResolution(
	INT nScreenX,
	INT nScreenY )
{
	m_pMainWnd->m_Device.m_option.m_dwScreenX = nScreenX;
	m_pMainWnd->m_Device.m_option.m_dwScreenY = nScreenY;
	CD3DImage::m_dwScreenX = nScreenX;
	CD3DImage::m_dwScreenY = nScreenY;
	m_pCommandHandler->m_vCOMMAND.push_back(GM_TOGGLE_SCREEN_MODE);

	CTClientUIBase::ResetBasis( 
		nScreenX,
		nScreenY);

	for( int i=0; i<TFRAME_COUNT; i++)
		if(m_vTFRAME[i])
			m_vTFRAME[i]->ResetPosition();

	CTChatList::SetDragZone( CRect( CPoint(0,0), CTClientUIBase::m_vBasis[TBASISPOINT_RIGHT_BOTTOM]) );

	m_pMainWnd->m_TNet.ResetPosition();
	m_pMainWnd->InitCamera();

	CString strFMT;
	strFMT = CTChart::Format( TSTR_FMT_RESOLUTION,
		nScreenX,
		nScreenY);

	m_pResolutionBtn->m_strText = strFMT;

	CTWebCashShopDlg* pWCSDLG = (CTWebCashShopDlg*) m_vTFRAME[ TFRAME_WEB_CASHSHOP ];
	if( pWCSDLG && pWCSDLG->CanProcess() && pWCSDLG->m_pHTTP )
		pWCSDLG->m_pHTTP->MoveWndHTTP();
}

#ifdef TEST_MODE
void CTClientGame::CopyToClipboard( CString strTEXT )
{
	if(!strTEXT.IsEmpty())
	{
		HGLOBAL hHANDLE = GlobalAlloc(
			GMEM_MOVEABLE,
			strTEXT.GetLength() + 1);

		if(!hHANDLE)
		{
			AfxMessageBox( "Cannot GlobalAlloc" );
			return;
		}
		LPTSTR pSTR = (LPTSTR) GlobalLock(hHANDLE);

		if(!pSTR)
		{
			GlobalFree(hHANDLE);
			AfxMessageBox( "Cannot GlobalLock" );
			return;
		}
		memcpy( pSTR, LPCTSTR(strTEXT), strTEXT.GetLength() * sizeof(TCHAR));
		pSTR[strTEXT.GetLength()] = (TCHAR) 0;
		GlobalUnlock(hHANDLE);

		if(!OpenClipboard(m_pMainWnd->GetSafeHwnd()))
		{
			GlobalFree(hHANDLE);
			AfxMessageBox( "Cannot open the Clipboard" );
			return;
		}

		if( !EmptyClipboard() )
		{
			GlobalFree(hHANDLE);
			AfxMessageBox( "Cannot empty the Clipboard" );
			return;  
		}

		if ( ::SetClipboardData( CF_TEXT, hHANDLE) == NULL )
		{
			GlobalFree(hHANDLE);
			AfxMessageBox( "Unable to set Clipboard data" );
			CloseClipboard();
			return;  
		}  

		CloseClipboard();
		return;
	}
}
#endif

void CTClientGame::CheckOpenCash()
{
	{
		CTShopDlg* pShopDlg = static_cast<CTShopDlg*>(m_vTFRAME[TFRAME_ITEM_SHOP]);
		if( pShopDlg->m_pOpenByCash &&
			FALSE == CheckOpenCash( pShopDlg->m_pOpenByCash, TNPC_ITEM ) )
				DisableUI( TFRAME_ITEM_SHOP);
	}

	{
		CTItemUpDlg* pITEMUPDLG = (CTItemUpDlg*) GetFrame( TFRAME_ITEM_UP );
		if( pITEMUPDLG->m_pOpenByCash &&
			FALSE == CheckOpenCash( pITEMUPDLG->m_pOpenByCash, TNPC_UPGRADE ) )
				DisableUI( TFRAME_ITEM_UP);
	}

	{
		CTNormalCabinetDlg* pNORMALCABINETDLG = (CTNormalCabinetDlg*) GetFrame( TFRAME_CABINET );
		if( pNORMALCABINETDLG->m_pOpenByCash &&
			FALSE == CheckOpenCash( pNORMALCABINETDLG->m_pOpenByCash, TNPC_WAREHOUSE ) )
				DisableUI( TFRAME_CABINET);
	}

	{
		CTItemGambleDlg* pITEMGAMBLEDLG = (CTItemGambleDlg*) GetFrame( TFRAME_ITEM_GAMBLE );
		if( pITEMGAMBLEDLG->m_pOpenByCash &&
			FALSE == CheckOpenCash( pITEMGAMBLEDLG->m_pOpenByCash, TNPC_GAMBLE ) )
				DisableUI( TFRAME_ITEM_GAMBLE);
	}

	{
		CTItemRepairDlg* pITEMREPAIRDLG = (CTItemRepairDlg*) GetFrame( TFRAME_ITEM_REPAIR );
		if( pITEMREPAIRDLG->m_pOpenByCash &&
			FALSE == CheckOpenCash( pITEMREPAIRDLG->m_pOpenByCash, TNPC_REPAIR ) )
				DisableUI( TFRAME_ITEM_REPAIR);
	}

	{
		CTItemRefineDlg* pITEMREFINDDLG = (CTItemRefineDlg*) GetFrame( TFRAME_ITEM_REFINE );
		if( pITEMREFINDDLG->m_pOpenByCash &&
			FALSE == CheckOpenCash( pITEMREFINDDLG->m_pOpenByCash, TNPC_REFINE ) )
				DisableUI( TFRAME_ITEM_REFINE);
	}
}

BOOL CTClientGame::CheckOpenCash( LPTOPENBYCASH pOpenByCash, TNPC_TYPE eNpcType )
{
	if( pOpenByCash )
	{
		CTClientItem* pITEM = FindMainItemByInven( pOpenByCash->m_bInvenID, pOpenByCash->m_bSlotID );
		if( !pITEM )
			return FALSE;

		if( pITEM->GetTITEM()->m_bKind != IK_NPCCALL )
			return FALSE;

		if( pITEM->GetTITEM()->m_wUseValue != pOpenByCash->m_wNPCID )
			return FALSE;

		LPTNPC pTNPC = CTChart::FindTNPCGlobal( pOpenByCash->m_wNPCID );
		if( !pTNPC )
			return FALSE;

		if( pTNPC->m_bNPCType != eNpcType )
			return FALSE;

		return TRUE;
	}

	return FALSE;
}

void CTClientGame::CheckItemupUseCashItem()
{
	CTItemUpCashDlg* pCashDlg = static_cast<CTItemUpCashDlg *>( m_vTFRAME[TFRAME_WARN_NOTUSE_CASHITEM] );

	CTItemUpDlg* pItemDlg = static_cast<CTItemUpDlg*>( m_vTFRAME[TFRAME_ITEM_UP] );
	CTTextLinker* pTextLinker = CTTextLinker::GetInstance();

	LPTITEM	pMatItem = pItemDlg->GetItem( CTItemUpDlg::SCROLL_SLOT );

	CString strMSG;

	switch( pMatItem->m_bKind )
	{
	case IK_UPGRADE	:
		{
			CTClientItem* pCTItem = pItemDlg->GetLastClientItem(CTItemUpDlg::TARGET_SLOT);
			if( !pCTItem )
				return;

			DWORD dwCost = CTChart::m_vTITEMGRADE[pCTItem->GetGrade()].m_dwCost;
			CString strCost = MakeMoneyStr(dwCost);
			BYTE bProtectionOfUpgrading = FALSE;

			// 행운의 물약 // 7609
			if ( !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_POTIONOFFORTUNE ) &&
				!m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_POTIONOFFORTUNE150 ) &&
				!m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_POTIONOFFORTUNE200 ) )
			{
				pCashDlg->AddItem( CTChart::FindTITEMTEMP( CTItemUpCashDlg::CASHITEM_POTIONOFFORTUNE ), CTChart::Format( TSTR_ITEMUP_WARN_UP_NOTUSE_POTIONOFFORTUNE ) );
			}

			// 강화 보호제 // 7654
			if ( !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_PROTECTIONOFUPGRADING ) )
			{
				pCashDlg->AddItem( CTChart::FindTITEMTEMP( CTItemUpCashDlg::CASHITEM_PROTECTIONOFUPGRADING ), CTChart::Format( TSTR_ITEMUP_WARN_UP_NOTUSE_PROTECTIONOFUPGRADING ) );
				bProtectionOfUpgrading = TRUE;
			}

			pCashDlg->m_pTopMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_UP_NOTUSE_CASHITEM_MESSAGE_1, strCost);
			
			if ( bProtectionOfUpgrading )

			{
				pCashDlg->m_pBottonMessage->m_pFont->m_dwColor = TCOLOR_TXT_WARN;
				pCashDlg->m_pBottonMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_NOTUSE_PROTECTIONOFUPGRADING );
				pCashDlg->m_pBottonMessage2->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_UP_NOTUSE_CASHITEM_MESSAGE_2 );
			}
			else
			{	
				pCashDlg->m_pBottonMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_UP_NOTUSE_CASHITEM_MESSAGE_2 );
			}	
		}
		break;

	case IK_MAGICGRADE	:
		{
			pCashDlg->m_pTopMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_MAGIC_NOTUSE_CASHITEM_MESSAGE_1 );
			pCashDlg->m_pBottonMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_MAGIC_NOTUSE_CASHITEM_MESSAGE_2 );

			// 전설의 제작 비전서 // 7611
			if ( !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_LEGENDENCHARTBOOK ) )
			{
				pCashDlg->AddItem( CTChart::FindTITEMTEMP( CTItemUpCashDlg::CASHITEM_LEGENDENCHARTBOOK ), CTChart::Format( TSTR_ITEMUP_WARN_MAGIC_NOTUSE_LEGENDENCHARTBOOK ) );
			}

		}
		break;

	case IK_RAREGRADE	:	
		{
			pCashDlg->m_pTopMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_RARE_NOTUSE_CASHITEM_MESSAGE_1 );
			pCashDlg->m_pBottonMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_RARE_NOTUSE_CASHITEM_MESSAGE_2 );

			// 전설의 제작 비전서 // 7611
			if ( !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_LEGENDENCHARTBOOK ) )
			{
				pCashDlg->AddItem( CTChart::FindTITEMTEMP( CTItemUpCashDlg::CASHITEM_LEGENDENCHARTBOOK ), CTChart::Format( TSTR_ITEMUP_WARN_MAGIC_NOTUSE_LEGENDENCHARTBOOK ) );
			}
		}
		break;
	}

	pCashDlg->SetMode( CTItemUpCashDlg::MODE_ITEMUP );
	pCashDlg->ShowComponent(TRUE);
	// m_pMainWnd->DoModalFrame( TFRAME_WARN_NOTUSE_CASHITEM );
	SwitchFocus( pCashDlg );	
}

void CTClientGame::CheckRefineUseCashItem( DWORD dwCost, BYTE bDiscountRate )
{
	CTItemUpCashDlg* pCashDlg = static_cast<CTItemUpCashDlg *>( m_vTFRAME[TFRAME_WARN_NOTUSE_CASHITEM] );
	CString strMSG;

	if ( bDiscountRate )
	{
		dwCost -= dwCost * bDiscountRate / 100;
		CString strCost = MakeMoneyStr(dwCost);
		pCashDlg->m_pTopMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_REFINE_SALE_NOTUSE_CASHITEM_MESSAGE_1, bDiscountRate, (LPCTSTR)strCost);
	}
	else
	{
		CString strCost = MakeMoneyStr(dwCost);
		pCashDlg->m_pTopMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_REFINE_NOTUSE_CASHITEM_MESSAGE_1, (LPCTSTR)strCost);
	}
	pCashDlg->m_pBottonMessage->m_strText = CTChart::Format( TSTR_ITEMUP_WARN_REFINE_NOTUSE_CASHITEM_MESSAGE_2 );

	// 전설의 제련 비전서 // 7612
	if ( !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_LEGENDREFINEBOOK ) )
	{
		pCashDlg->AddItem( CTChart::FindTITEMTEMP( CTItemUpCashDlg::CASHITEM_LEGENDREFINEBOOK ), CTChart::Format( TSTR_ITEMUP_WARN_RARE_NOTUSE_LEGENDREFINEBOOK ) );
	}

	// 마법속성 전이 주문서 // 7613
	if ( !m_pMainChar->FindMaintain( CTItemUpCashDlg::MAINTAIN_MAGICTRANSFERSPELL ) )
	{
		pCashDlg->AddItem( CTChart::FindTITEMTEMP( CTItemUpCashDlg::CASHITEM_MAGICTRANSFERSPELL ), CTChart::Format( TSTR_ITEMUP_WARN_RARE_NOTUSE_MAGICTRANSFERSPELL ) );
	}

	pCashDlg->SetMode( CTItemUpCashDlg::MODE_REFINE );
	pCashDlg->ShowComponent(TRUE);
	SwitchFocus( pCashDlg );
}

void CTClientGame::CheckRelay(DWORD dwTick)
{
	if(m_dwRelayTick > 90000)
	{
		RelaySession()->SendCS_CHECKRELAY_REQ();
		m_dwRelayTick = 0;
	}
	else
		m_dwRelayTick += dwTick;
}

#ifdef ADD_SNOW
void CTClientGame::StartSnowEvent()
{
	if( m_bSnowEventUserOn && !m_bSnowEvent )
	{
		MAPRES::iterator itFinder = m_pRES->m_mapSFX.find( ID_EVENT_SNOW4 );
		if( itFinder != m_pRES->m_mapSFX.end () )
		{
			CTClientSFX *pTSFX = new CTClientSFX();
			LPOBJSFX pItem = new OBJSFX();

			pItem->m_pSFX = new SFXINST();
			pItem->m_bUseOBJScale = TRUE;
			pItem->m_bDeleteINST = TRUE;

			pItem->m_pSFX->m_bDeleteOnStop = FALSE;
			pItem->m_pSFX->m_dwFuncID = 0;
			pItem->m_pSFX->m_nPivotID = 0;
			pItem->m_pSFX->m_dwTick = 0;
			pItem->m_pSFX->m_bBias = 0;

			FLOAT fCamDist = m_pMainChar->GetAttrFLOAT( ID_CAM_DIST );

			pItem->m_pSFX->m_vPOS = D3DXVECTOR3( 0.0f, fCamDist*2.0f, -5.0f);
			pItem->m_pSFX->m_vROT = D3DXVECTOR3( 0, 0, 0);
			pItem->m_pSFX->m_pSFX = (LPSFX) itFinder->second;

			pTSFX->m_bHasHost = TRUE;
			pTSFX->m_pHost = m_pMainWnd;
			pTSFX->m_dwInitTick = 0;

			m_dwSnowEventEffect = pItem->m_dwID = CTachyonSFX::RegSFX(
				pTSFX,
				pItem->m_pSFX->m_bBias);

			m_pMainChar->m_vTGARBAGESFX.push_back(pItem);
		}

		m_bSnowEvent = TRUE;
	}
}

void CTClientGame::EndSnowEvent()
{
	if( m_bSnowEvent )
	{
		VECTOROBJSFX::iterator it, end;
		it = m_pMainChar->m_vTGARBAGESFX.begin();
		end = m_pMainChar->m_vTGARBAGESFX.end();

		for(; it != end ; ++it )
		{
			if( (*it)->m_dwID == m_dwSnowEventEffect )
			{
				CTachyonSFX::Release( (*it)->m_dwID );
				delete (*it);
				m_pMainChar->m_vTGARBAGESFX.erase( it );

				m_bSnowEvent = FALSE;
				m_dwSnowEventEffect = 0;
				return ;
			}
		}
	}
}
#endif

void CTClientGame::StopMoveMainChar()
{
	m_bMoveType = TMAINMOVE_NORMAL;
	m_bAutoRun = FALSE;
	m_dwMoveGM = 0;

	m_vKEY.ResetSTATE();
}

void CTClientGame::StartApexClient( CTachyonSession *pSession )
{
#ifndef TEST_MODE
	long re = CHCStart( static_cast<CTClientSession*>(pSession)->SnedCS_APEXDATA_REQ, m_pApexfRec );

	static_cast<CTClientSession*>(pSession)->SendCS_APEXSTART_REQ( re );
#endif
}

void CTClientGame::ResetKICKOUTMAP()
{
	m_wKICKOUTMAP = 0;
	m_bDoKICKOUTMAP = FALSE;
	m_dwLeftTickKICKOUTMAP = 0;

	static_cast<CTGaugePannel*>( m_vTFRAME[TFRAME_GAUGE] )->DettachSiegeAlarm();
}

CString GetFameTitle( BYTE bType, DWORD dwRank )
{
	static DWORD ENTRYTITLE[] =
	{
		0,
		TSTR_TOURNAMENT_ENTRY_1, //여신의 기사단
		TSTR_TOURNAMENT_ENTRY_2, //팔켄하인의 화신
		TSTR_TOURNAMENT_ENTRY_3, //루마파르크의 화신
		TSTR_TOURNAMENT_ENTRY_4, //펜다트론의 화신
		TSTR_TOURNAMENT_ENTRY_5, //라피리스의 화신
		TSTR_TOURNAMENT_ENTRY_6, //파울드론의 화신
		TSTR_TOURNAMENT_ENTRY_7, //레카미에의 화신
		TSTR_TOURNAMENT_ENTRY_8, //엔덴드로스의 화신
		TSTR_TOURNAMENT_ENTRY_9 //이벤트
	};

	switch( bType )
	{
	case FRT_GODDESS:
		return CTChart::LoadString(TSTR_GODDESS);
	case FRT_GOD:
		{
			DWORD dwEntryID = dwRank + 2;
			return (dwEntryID) != 0 ? CTChart::LoadString( (TSTRING) ENTRYTITLE[dwEntryID] ) : _T("");
		}
		break;
	case FRT_HERO:
		return CTChart::FindTFAMETITLE(BYTE(dwRank))->m_strFameName;
	}
	return _T("");
}

void CTClientGame::CheckMonthRank( CTClientChar* pTOBJ )
{
	CTClientChar* pChar = static_cast<CTClientChar*>(pTOBJ);
	pChar->m_bUseFameTitle = FALSE;

	{
		TMAPFAMERANK::iterator it, end;
		it = m_mapTFAMERANK[ FRT_GODDESS ].begin();
		end = m_mapTFAMERANK[ FRT_GODDESS ].end();

		for(; it != end ; ++it )
		{
			if ( pChar->m_dwID == (*it).second->dwCharID )
			{
				pChar->m_bUseFameTitle = TRUE;
				pChar->m_strFameTitle = CTChart::LoadString(TSTR_GODDESS);
				pChar->m_strFameTitle = GetFameTitle( FRT_GODDESS, (*it).first);
				return;
			}
		}
	}

	{
		TMAPFAMERANK::iterator it, end;
		it = m_mapTFAMERANK[ FRT_GOD ].begin();
		end = m_mapTFAMERANK[ FRT_GOD ].end();

		for(; it != end ; ++it )
		{
			if ( pChar->m_dwID == (*it).second->dwCharID )
			{
				pChar->m_bUseFameTitle = TRUE;
				pChar->m_strFameTitle = GetFameTitle( FRT_GOD, (*it).first);
				return;
			}
		}
	}

	{
		TMAPFAMERANK::iterator it, end;
		it = m_mapTFAMERANK[ FRT_HERO ].begin();
		end = m_mapTFAMERANK[ FRT_HERO ].end();

		for(; it != end ; ++it )
		{
			if ( pChar->m_dwID == (*it).second->dwCharID )
			{
				pChar->m_bUseFameTitle = TRUE;
				pChar->m_strFameTitle = GetFameTitle( FRT_HERO, (*it).second->dwMonthRank );
				return;
			}
		}
	}
}

void CTClientGame::UpdateMonthRank()
{
	MAPPLAYER::iterator itPC;

	for ( itPC = m_mapPLAYER.begin(); itPC != m_mapPLAYER.end(); itPC++ )
		CheckMonthRank( (*itPC).second );

	CheckMonthRank( m_pMainChar );
}

void CTClientGame::SetTournamentWatching()
{
}

void CTClientGame::ResetTournamentWatching()
{
	DisableUI( TFRAME_TOURNAMENT_GAGE );
}

void CTClientGame::SetResetMAP( BOOL bResetMAP)
{
	m_bResetMAP = bResetMAP;

	if(m_bResetMAP)
	{
		m_vMAP.ResetTMAP(
			m_pDevice,
			m_pCamera,
			0,
			TRUE);
	}
}

void CTClientGame::CalcTournament( DWORD dwTick)
{
	if( m_bDoAutoRevival )
	{
		if( m_dwLeftTickAR <= dwTick )
		{
			m_pMainWnd->CloseMessageBox(TRUE);
			m_bDoAutoRevival = FALSE;

			if( m_pMainChar->IsDead() || m_pMainChar->m_bGhost )
			{
				SendCS_REVIVAL_REQ(
					m_pMainChar->GetPositionX(),
					m_pMainChar->GetPositionY(),
					m_pMainChar->GetPositionZ(),
					REVIVAL_GHOST);

				m_bAutoRun = FALSE;
			}
		}
		else
		{
			DWORD dwLEFT = m_dwLeftTickAR - dwTick;
			if( m_dwLeftTickAR/TNOTIFY_PERIOD != dwLEFT/TNOTIFY_PERIOD )
			{
				m_pChatFrame->ChatSysMSG(
					CTChart::LoadString(TSTR_INFO_TITLE),
					m_pMainChar->GetName(),
					CTChart::Format(TSTR_TOURNAMENT_AUTO_REVIVAL, m_dwLeftTickAR/TNOTIFY_PERIOD), 
					TCOLOR_INFO,
					TCHAT_FLAG_INFO,
					ID_SND_INFO,
					MEDIA_TSOUND);
			}

			m_dwLeftTickAR = dwLEFT;
		}
	}
}

void CTClientGame::CalcTournamentChar( DWORD dwTick)
{
	if( m_LeftChar.m_OBJ.m_pOBJ != NULL )
	{
		m_LeftChar.CalcTick(
			&m_LeftChar.GetPosition(),
			m_pDevice,
			CTClientGame::GetInstance()->GetResource(),
			dwTick);

		if( m_LeftChar.m_bActionLock == FALSE )
		{
			CTTournamentResult* pDLG = static_cast<CTTournamentResult*>( m_vTFRAME[ TFRAME_TOURNAMENT_RESULT ] );

			if( pDLG->m_bResult )
			{
				int i = pDLG->m_bTrueIsLeftWinButRightWin ? 109 : 110;
				TACTION vActionID = m_LeftChar.FindActionID( i, WT_NORMAL);
				m_LeftChar.ClearAnimationID();
				m_LeftChar.SetAction( vActionID.m_dwActID, vActionID.m_dwAniID );
				m_LeftChar.m_bACTLevel = CTChart::GetTACTION( i )->m_bLevel + 1;
			}
			else
			{
				TACTION vActionID = m_LeftChar.FindActionID( 0, WT_NORMAL);
				m_LeftChar.SetAction( vActionID.m_dwActID, vActionID.m_dwAniID );
			}

			m_LeftChar.m_bActionLock = TRUE;
		}
	}

	if( m_RightChar.m_OBJ.m_pOBJ != NULL )
	{
		m_RightChar.CalcTick(
			&m_RightChar.GetPosition(),
			m_pDevice,
			CTClientGame::GetInstance()->GetResource(),
			dwTick);

		if( m_RightChar.m_bActionLock == FALSE )
		{
			CTTournamentResult* pDLG = static_cast<CTTournamentResult*>( m_vTFRAME[ TFRAME_TOURNAMENT_RESULT ] );

			if( pDLG->m_bResult )
			{
				int i = !(pDLG->m_bTrueIsLeftWinButRightWin) ? 109 : 110;
				TACTION vActionID = m_RightChar.FindActionID(i, WT_NORMAL);
				m_RightChar.ClearAnimationID();
				m_RightChar.SetAction( vActionID.m_dwActID, vActionID.m_dwAniID );
				m_RightChar.m_bACTLevel = CTChart::GetTACTION( i )->m_bLevel + 1;
			}
			else
			{
				TACTION vActionID = m_RightChar.FindActionID( 0, WT_NORMAL);
				m_RightChar.SetAction( vActionID.m_dwActID, vActionID.m_dwAniID );
			}

			m_RightChar.m_bActionLock = TRUE;
		}
	}
}

void CTClientGame::SetTournamentInfo(
	LPTTOURNAMENTTEAM pTLeftTeam,
	LPTTOURNAMENTTEAM pTRightTeam,
	BOOL bResetOption )
{
	if( m_bTournamentBattle )
	{
		CTGaugePannel* pDLG = static_cast<CTGaugePannel*>( m_vTFRAME[ TFRAME_GAUGE ] );
		pDLG->m_pTournamentInfo->SetInfoTMode( pTLeftTeam, pTRightTeam, bResetOption );
	}
	else if( m_bTournamentWatching )
	{
		CTTournamentGage* pDLG = static_cast<CTTournamentGage*>( m_vTFRAME[ TFRAME_TOURNAMENT_GAGE ] );
		pDLG->m_pTournamentInfo->SetInfoTMode( pTLeftTeam, pTRightTeam, bResetOption );
	}
}

void CTClientGame::SetTournamentRemainTime( BOOL onoff, DWORD dwTIME )
{
	if( m_bTournamentBattle )
	{
		CTGaugePannel* pDLG = static_cast<CTGaugePannel*>( m_vTFRAME[ TFRAME_GAUGE ] );
		pDLG->m_pTournamentInfo->m_bRemainTime = onoff;
		pDLG->m_pTournamentInfo->m_dwRemainTime = dwTIME;
		pDLG->m_pTournamentInfo->m_pTIME->m_strText.Empty();
	}
	else if( m_bTournamentWatching )
	{
		CTTournamentGage* pDLG = static_cast<CTTournamentGage*>( m_vTFRAME[ TFRAME_TOURNAMENT_GAGE ] );
		pDLG->m_pTournamentInfo->m_bRemainTime = onoff;
		pDLG->m_pTournamentInfo->m_dwRemainTime = dwTIME;
		pDLG->m_pTournamentInfo->m_pTIME->m_strText.Empty();
	}
}

void CTClientGame::OnTournamentEnd()
{
	MAPOBJECT::iterator it, end;
	it = m_vMAP.m_mapTNpc.begin();
	end = m_vMAP.m_mapTNpc.end();

	for(; it != end ; ++it )
	{
		CTClientNpc* pTNpc = static_cast<CTClientNpc*>(it->second);
		
		if( pTNpc->m_bNPCType != TNPC_TOURNAMENT_GATE_KEEPER )
			continue;

        if( pTNpc->m_dwActionID == ID_DIE_NORMAL ||
			pTNpc->m_dwActionID == ID_WALK_NORMAL )
			continue;

		pTNpc->m_pHost = m_pMainWnd;
		pTNpc->m_dwActionID = ID_WALK_NORMAL;
		pTNpc->SetLogicAction(pTNpc->m_dwActionID);

		pTNpc->m_bLogic = TTGATE_LOGIC_END;
		pTNpc->m_bActionLock = TRUE;
	}
}

void CTClientGame::OnTournamentEnter()
{
	MAPOBJECT::iterator it, end;
	it = m_vMAP.m_mapTNpc.begin();
	end = m_vMAP.m_mapTNpc.end();

	for(; it != end ; ++it )
	{
		CTClientNpc* pTNpc = static_cast<CTClientNpc*>(it->second);
		
		if( pTNpc->m_bNPCType != TNPC_TOURNAMENT_GATE_KEEPER )
			continue;

        if( pTNpc->m_dwActionID == ID_STAND_NORMAL ||
			pTNpc->m_dwActionID == ID_DEAD_NORMAL )
			continue;

		pTNpc->m_pHost = m_pMainWnd;
		pTNpc->m_dwActionID = ID_DEAD_NORMAL;
		pTNpc->SetLogicAction(pTNpc->m_dwActionID);

		pTNpc->m_bLogic = TTGATE_LOGIC_ENTER;
		pTNpc->m_bActionLock = TRUE;
	}
}

BOOL CTClientGame::IsShowMinimap()
{
	if( 500 <= m_vMAP.m_wMapID && m_vMAP.m_wMapID <= 532 )
		return FALSE;

	return !m_pTRSCS->IsVisible() && !m_bTournamentBattle && !m_bTournamentWatching;
}

void CTClientGame::ExitTutorial()
{
	if(m_pSESSION)
		switch(m_pMainChar->m_bContryID)
		{
		case TCONTRY_D	: m_pSESSION->SendCS_TELEPORT_REQ(TSTART_D_PORTAL_ID); break;
		case TCONTRY_C	: m_pSESSION->SendCS_TELEPORT_REQ(TSTART_C_PORTAL_ID); break;
		}
}

DWORD CTClientGame::ShotFirecracker(
	WORD wTempID,
	DWORD dwLifeTime,
	INT nDIR )
{
	CTClientRecall *pTRECALL = new CTClientRecall();
	if( !pTRECALL )
		return 0;

	pTRECALL->m_pHost = m_pMainWnd;
	pTRECALL->ReleaseData();
	pTRECALL->m_bType = OT_SELF;
	pTRECALL->m_bContryID = m_pMainChar->m_bContryID;
	pTRECALL->m_bColorID = 0;
	pTRECALL->m_bLevel = 1;
	pTRECALL->m_dwMaxHP = 1;
	pTRECALL->m_dwHP = 1;
	pTRECALL->m_dwMaxMP = 1;
	pTRECALL->m_dwMP = 1;
	pTRECALL->m_vWorld._41 = m_pMainChar->GetPositionX();
	pTRECALL->m_vWorld._42 = m_pMainChar->GetPositionY();
	pTRECALL->m_vWorld._43 = m_pMainChar->GetPositionZ();
	pTRECALL->m_bAction = 0;
	pTRECALL->m_bMode = 0;
	pTRECALL->m_bRecallType = 4;
	pTRECALL->m_vTSKILLDATA.m_bHIT = 0;
	pTRECALL->m_vTSKILLDATA.m_bLevel = 1;
	pTRECALL->m_vTSKILLDATA.m_wAL = 0;
	pTRECALL->m_vTSKILLDATA.m_bAtkLevel = 0;
	pTRECALL->m_vTSKILLDATA.m_dwMinAP = 0;
	pTRECALL->m_vTSKILLDATA.m_dwMaxAP = 0;
	pTRECALL->m_vTSKILLDATA.m_dwMinMP = 0;
	pTRECALL->m_vTSKILLDATA.m_dwMaxMP = 0;
	pTRECALL->m_dwEndLifeTick = ::GetTickCount() + dwLifeTime;
	pTRECALL->m_pTREGION = NULL;
	pTRECALL->m_bACTLevel = CTChart::GetTACTION(pTRECALL->m_bAction)->m_bLevel;
	pTRECALL->m_wPITCH = 0;
	pTRECALL->m_wDIR = 0;
	pTRECALL->m_nPITCH = 0;
	pTRECALL->m_nDIR = nDIR;
	pTRECALL->m_dwHostID = m_pMainChar->m_dwID;
	pTRECALL->m_dwID = -1;
	while( FindFixRecall( pTRECALL->m_dwID ) )
		pTRECALL->m_dwID -= 1;

	pTRECALL->InitRecall(
		m_pDevice,
		m_pRES,
		wTempID,
		1 );

	m_mapFIXRECALL.insert( MAPRECALL::value_type( pTRECALL->m_dwID, pTRECALL));
	m_vMAP.LandTOBJ(pTRECALL);

	pTRECALL->ApplyTAction( m_pDevice, m_pRES, TA_SPAWN);

	return pTRECALL->m_dwID;
}

void CTClientGame::CalcFirecracker( DWORD dwTick )
{
	if( m_dwFirecrackerID[0] != 0 )
	{
		m_dwRemainLifeTimeFirecracker -= min(m_dwRemainLifeTimeFirecracker, dwTick);
		if( m_dwRemainLifeTimeFirecracker == 0 )
		{
			for( int i=0 ; i < MAX_FIRECRACKER ; ++i )
			{
				DeleteFixRecall( m_dwFirecrackerID[ i ] );
				m_dwFirecrackerID[ i ] = 0;
			}

			m_dwRemainLifeTimeFirecracker = 0;
		}
	}
}

void CTClientGame::ShotMissionWarFirecracker()
{
	if( m_dwFirecrackerID[ 0 ] == 0 )
	{
		m_dwRemainLifeTimeFirecracker = 23800;

		m_dwFirecrackerID[ 0 ] = ShotFirecracker(
			20005,
			m_dwRemainLifeTimeFirecracker,
			INT(GetMainChar()->m_wDIR) );

		m_dwFirecrackerID[ 1 ] = ShotFirecracker(
			20006,
			m_dwRemainLifeTimeFirecracker,
			INT(GetMainChar()->m_wDIR) + 450 );

		m_dwFirecrackerID[ 2 ] = ShotFirecracker(
			20007,
			m_dwRemainLifeTimeFirecracker,
			INT(GetMainChar()->m_wDIR) - 450 );

		m_dwFirecrackerID[ 3 ] = ShotFirecracker(
			20005,
			m_dwRemainLifeTimeFirecracker,
			INT(GetMainChar()->m_wDIR) + 900 );
	}
}

void CTClientGame::CalcWaitingRevivalItem( DWORD dwTick )
{
	if( m_dwWaitingRevivalItem != 0 )
	{
		m_dwWaitingRevivalItem -= min(m_dwWaitingRevivalItem, dwTick);

		if( m_bShowingRevivalItemMessageBox )
		{
			if( m_dwWaitingRevivalItem == 0 )
			{
				BYTE bInven, bSlot;
				if( FindInven(IT_USE, IK_REVIVAL, bInven, bSlot) )
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
				TFrame* pFRAME = m_pMainWnd->m_MessageBox.FindFrame(ID_FRAME_1BTNMSGBOX);
				if( pFRAME )
				{
					TComponent *pMSG = pFRAME->FindKid(ID_CTRLINST_MSG);
					if( pMSG )
						pMSG->m_strText = CTChart::Format(TSTR_GOTO_THUMB_MSG_SEC, m_dwWaitingRevivalItem / 1000 + 1);
				}
			}
		}
	}
}

void CTClientGame::CheckValidHeightInfo()
{
	if( m_vMAP.m_pMAP )
	{
		int nSIZE = m_vMAP.m_pMAP->m_nUnitLength * m_vMAP.m_pMAP->m_nTileLength;
		int nUnitX = INT(m_pMainChar->GetPositionX()) / nSIZE - m_vMAP.m_pMAP->m_nUnitX;
		int nUnitZ = INT(m_pMainChar->GetPositionZ()) / nSIZE - m_vMAP.m_pMAP->m_nUnitZ;

		if( nUnitX >= 0 && nUnitX < 3 && nUnitZ >= 0 && nUnitZ < 3 )
		{
			BYTE bINDEX = BYTE(nUnitZ * 3 + nUnitX);
			if( m_vMAP.m_pMAP->m_bUNIT[bINDEX] >= 0 && m_vMAP.m_pMAP->m_bUNIT[bINDEX] < 9 )
			{
				m_bMainCharMoveLock = (m_vMAP.m_mapTHEIGHTINFO[m_vMAP.m_pMAP->m_bUNIT[bINDEX]].empty());
			}
		}
	}
}