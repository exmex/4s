#pragma once

#ifndef TEST_MODE
#include "ApexClient.h"
#include <HShield.h>
#include <HSUpChk.h>
#endif

class CTClientGame : public CFrameGroup
{
	friend class CTClientWnd;

public:
	CTClientGame();
	virtual ~CTClientGame();

	void StartApexClient( CTachyonSession *pSession );
#ifndef TEST_MODE
	_FUNC_C_REC		m_pApexfRec;
#endif

	static CTClientGame* m_pINSTANCE;
	static CTClientGame* GetInstance();
	static void DebugMSG( const CString& strMSG, BOOL bPrintChat=TRUE, BOOL bFile=FALSE, LPCTSTR szPATH="" );
	static CString GetMD5String( CString& strSOURCE );

	static bool GetSafeMainPos(int& outMap, float& outX, float& outY);
	static bool GetSafeMainTarget(CString& outType, unsigned int& outID);

	static VOID SplitMoney( INT64 dwPrice, DWORD *pbRune, DWORD *pbLuna, DWORD *pbCron);
	static VOID AddUpMoney( INT64* pPrice, DWORD dwRune, DWORD dwLuna, DWORD dwCron);
	static CString MakeMoneyStr(INT64 dwPrice);
	static CString MakeMoneyStr(DWORD dwRune, DWORD dwLuna, DWORD dwCron);

	// Option calculator
	static BYTE GetMapDETAILOption();
	static BYTE GetMapSHADOWOption();
	static BYTE GetObjSHADOWOption();
	static BYTE GetMapSFXOption();

	static BYTE GetNpcNAMEOption();
	static BYTE GetMonNAMEOption();
	static BYTE GetPcNAMEOption();

	static BYTE GetMainVolumeOption();
	static BYTE GetBGMVolumeOption();
	static BYTE GetSFXVolumeOption();

	static BYTE GetLIGHTMAPOption( BYTE bDungeon);
	static BYTE GetFarIMAGEOption();
	static BYTE GetHUDOption();
	static BYTE GetSNDOption();
	static BYTE GetBGMOption();

	static FLOAT GetObjDETAILOption();
	static FLOAT GetOBJRangeOption();

	static CString ToTimeString(DWORD dwTick);
	static CString ToTimeOneString(DWORD dwTick);

	static CString OnActCmdByMsg(
		const CString& strMSG,
		const VECTORSTRING& vPARAM,
		DWORD dwUSER);

	static void OnActionCmdBtnDown(
		CTMiniPopupDlg *pPopupDlg,
		CTMiniPopupDlg::CTButtonMP *pButton);

	static LPTEXTURESET m_vTNPCICON[TNPCICON_COUNT];			// NPC icon texture
	static BYTE m_vTNPCICONIDX[TNPC_COUNT];						// NPC icon index

	static LPTEXTURESET m_pTCOMMANDERICON;						// Commander icon texture
	static LPTEXTURESET m_pTCHIEFICON;							// Chief icon texture
	static LPTEXTURESET m_pTSELECTMARK;							// Selection mark texture
	static LPTEXTURESET m_pTSOULMATE;							// Soulate icon texture

	static LPTEXTURESET m_pTGROUNDTEX;							// Ground select texture
	static CT3DVertex m_vTRECTVB;								// Common rect vertex buffer
	static TOPTION m_vTOPTION;									// Advenced game option
	static BYTE m_bTOPTIONLEVEL;								// Advenced game option level

	static CTChatFrame *m_pChatFrame;							// Chat Frame

	static DWORD m_vTICONGAUGE[TINVENSLOT_COUNT];				// Icon gauge ID pool
	static DWORD m_vTICONSM[TSKILLSLOT_COUNT];					// Icon select marker ID pool
	static WORD m_vSlotID[MAX_DRAGSLOT];						// Slot ID pool

	static DWORD m_vTCOUNTRYSTR[TCONTRY_COUNT];					// Country name pool
	static DWORD m_vTCLASSSTR[TCLASS_COUNT];					// Class name pool
	static DWORD m_vTRACESTR[TRACE_COUNT];						// Race name pool
	static BOOL m_bNeedCharDetInfoInit;
	static DWORD m_dwReleaseMaintainFunctionWhenGameOut[4];

protected:
	DWORD m_dwNameTick;
	CPoint m_ptPrevPos;
	TComponent* m_pCandidate;

public:
	MAPMONSTER m_mapSLAVEMON;							// Slave monster pool
	MAPMONSTER m_mapMONSTER;							// Monster pool
	MAPTSWITCH m_mapTSwitch;							// Switch pool
	MAPPLAYER m_mapPLAYER;								// Player pool

	MAPRECALL m_mapSLAVEFIXRECALL;						// Slave non moveable recall pool
	MAPRECALL m_mapSLAVERECALL;							// Slave recall pool
	MAPRECALL m_mapFIXRECALL;							// Non moveable recall pool
	MAPRECALL m_mapRECALL;								// Recall pool
	MAPRECALL m_mapFAKETEMP;							// Fake character temporary pool

public:
	CTScenePlayer m_vTSCENE_PLAYER;

	VTMAINTAIN m_vTMAINTAINOBJ;							// Maintain Player, Monster, Recall pool
	TMAPFAMERANK m_mapTFAMERANK[FRT_COUNT];				// Fame Rank pool

public:
	static BYTE m_bSendedCS_CASHITEMPUTIN_REQ;

	VTNUMBER m_vTNUMBER;								// Number SFX pool
	VTARROW m_vTARROW;									// Arrow pool
	VTPARTY m_vTPARTY;									// Party pool
	BYTE m_bPartyItemType;								// Party item type

	CTClientUIBase *m_vTGAUGEFRAME[TGAUGE_FRAME_COUNT];	// Gauge set host
	CTClientUIBase *m_vTFRAME[TFRAME_COUNT];			// UI pool

	CTMaintainPannel *m_vTMAINTAIN[TPANNEL_COUNT];		// Maintain ctrl pool
	TComponent *m_vTLEADERMARK[TLEADERMARK_COUNT];		// Leader mark ctrl pool
	TComponent *m_vTPANNEL[TPANNEL_COUNT];				// Pannel pool
	TComponent *m_vTTEXT[TTEXT_COUNT];					// Text pool
	TComponent *m_pTPARTYITEM;							// Party item type ctrl

	TImageList *m_vTFACEIMG[TFACEIMG_COUNT];			// Face image pool
	TComponent *m_vTLEVELIMG[TLEVELIMG_COUNT];			// Level image pool
	TComponent *m_vTFACEBACKIMG[TFACEBACKIMG_COUNT];	// Level image pool
	TGauge *m_vTGAUGE[TGAUGE_COUNT];					// Gauge pool

	TComponent *m_vTHUDBASE[THUD_BASE_COUNT];
	TComponent *m_vTHUDTEXT[THUD_TEXT_COUNT];
	TGauge *m_vTHUDGAUGE[THUD_GAUGE_COUNT];

	CTDurationFrame* m_pDurationUI;
	CTRSCSDlg *m_pTRSCS;								// RSCS ctrl
	CTMinimapDlg *m_pTMinimap;							// Minimap ctrl

	CTPopupMenu *m_pTPOPUP;								// Popup menu ctrl
	CTPopupMenu *m_pChannelPOPUP;						// Popup menu ctrl for channel
	TComponent *m_pTCURFRAME;							// Current Frame

	CTDynamicHelpDlg *m_pNotifyFrame;					// Notify Frame
	CTMiniPopupDlg *m_pActListPopup;					// Actlist popup dialog
	TComponent* m_pResolutionBtn;
	CTMessengerBase *m_pMessengerBase;					// Messenger base frame
	CTMailListFrame *m_pMailList;						// Mail list frame

public:
	CTBlockListDlg* m_pBlockList;

protected:
	CTSystemSettingFrame *m_pSystemSetting;
	CTGameSettingFrame *m_pGameSetting;

	TButton* m_pLockButton;

	BOOL m_bDuel;
	CTClientChar* m_pDuelTarget;
	D3DXVECTOR2 m_vDuelCenter;
	DWORD m_dwDuelTick;
	DWORD m_dwDuelOutTick;
	DWORD m_dwDuelWaitTick;
	CString m_strDuelTargetName;
	DWORD m_dwDuelTargetID;

	BYTE m_bSkillInitInven;							// 스킬 초기화 아이템의 인벤 아이디
	BYTE m_bSkillInitSlot;							// 스킬 초기화 아이템의 슬롯 아이디
	
	BYTE m_bPortalItemInven;						// 포탈 아이템 인벤 아이디
	BYTE m_bPortalItemSlot;							// 포탈 아이템 슬롯 아이디

	CString m_bPartyRecallName;						// 파티 소환 초대자의 이름 릴레이
	BYTE m_bPartyRecallAnsInven;					// 파티 소환 아이템 인벤 아이디 릴레이
	BYTE m_bPartyRecallAnsSlot;						// 파티 소환 아이템 슬롯 아이디 릴레이

	BYTE m_bDurationRepType;						// 내구도 수리 타입(0:일반수리, 1:장비수리 2:전부수리)
	BYTE m_bDurationRepInven;						// 내구도 수리 대상 인벤 아이디
	BYTE m_bDurationRepSlot;						// 내구도 수리 대상 슬롯 아이디
	BYTE m_bRefineInven;							// 재련 대상 인벤 아이디
	BYTE m_bRefineSlot;								// 재련 대상 슬롯 아이디

	BYTE m_bLuckyID;								// 행운번호(이벤트용)

	LPTSKILL m_pSelectSkill;

	DWORD m_dwSoulRegion;
	DWORD m_dwSoulID;								// 소울메이트 케릭터아이디
	CString m_strSoulName;							// 소울메이트 케릭터이름
	CString m_strSoulRelay;

	BYTE m_bInvenDraging;							// 인벤을 드래그 중인지 여부
	BYTE m_bDrawOBJ;

	int m_nTotalDrawCount;
	int	m_vDrawCount[OT_COUNT];

	TComponent* m_pTOPFRAME;
	BYTE m_bTOPFRAMEID;

protected:
	CTClientSession *m_pSESSION;
	CTClientSession *m_pRelay;
	CTClientObjBase *m_pTARGET;
	CTClientObjBase *m_pACT;
	CTClientObjBase *m_pHIT;
	
	INT m_nDEADMONHITIDX;
	VTMONSTER m_vDEADMONHITS;

	CTClientSFX m_vTPOINTER;
	LPTPARTY m_pTPARTY;
	LPSFX m_pTPOINTER;
	LPBYTE m_pFUZZY;

	CD3DLight m_vLIGHT[TLIGHT_COUNT];
	CD3DDevice *m_pDevice;

#ifdef TEST_MODE
	CD3DFont m_FontPOS;
	CD3DFont m_FontTARGET;
#endif

	CTClientMoveObj *m_pCtrlHost;
	CTClientChar *m_pMainChar;
	CTClientChar m_vMainDead;

	CTClientWnd *m_pMainWnd;
	CTClientCAM *m_pTextCAM;
	CTClientCAM *m_pCamera;
	CTClientCAM m_SubCamera;

	CTachyonRes *m_pRES;

	CTClientInven m_vTMONINVEN;
	CTClientMAP m_vMAP;

	DWORD m_vMonMoney[TMONEY_COUNT];

protected:
	// RSCS SFX
	CTClientObjBase m_vTCMDTARGET;
	CTClientObjBase m_vTCMDDIR;
	CTClientArrow m_vTCMDARROW;

	// Sky Box
	CTClientObjBase m_vFARIMAGE[2];
	CTClientObjBase m_vSKY[2];

	DWORD m_dwFogCOLOR;
	DWORD m_dwTSKY;
	WORD m_wTSKY;
	BYTE m_bTSKY;

	// Object material
	D3DMATERIAL9 m_vOBJMTRL;
	D3DMATERIAL9 m_vNormal;
	D3DMATERIAL9 m_vShadow;

	// Battle factor
	D3DXVECTOR3 m_vTGROUND;

	DWORD m_dwMaxChargeTick;
	DWORD m_dwChargeTick;
	DWORD m_dwGHOSTTick;
	DWORD m_dwFLASHTick;
	DWORD m_dwQUAKETick;
	DWORD m_dwSYNCTick;
	DWORD m_dwPOSTick;
	DWORD m_dwTGROUND;

	BYTE m_bGroundLOCK;
	BYTE m_bGroundATK;
	BYTE m_bLoopLock;
	BYTE m_bLAND;
	BYTE m_bDrawGROUND;

	BYTE m_bLOOPCHARGE;
	BYTE m_bCHARGE;
	BYTE m_bGHOST;
	BYTE m_bQUAKE;
	BYTE m_bRSCS;
	TCHARGETYPE m_eCHARGETYPE;

	WORD m_wNextSkillID;
	WORD m_wSkillID;

	// Drag Information
	TDRAG m_vDRAG;

public:
	bool m_bMainCharMoveLock;

	void SetTournamentInfo(
		LPTTOURNAMENTTEAM pTLeftTeam,
		LPTTOURNAMENTTEAM pTRightTeam,
		BOOL bResetOption );

	void SetTournamentRemainTime( BOOL onoff, DWORD dwTIME );
	void OnTournamentEnd();
	void OnTournamentEnter();

	LPTTOURNAMENTTEAM m_pTLeftTeam;
	LPTTOURNAMENTTEAM m_pTRightTeam;
	BOOL m_bTournamentBattle;
	BOOL m_bTournamentWatching;
	CTClientChar m_LeftChar;
	CTClientChar m_RightChar;
	D3DXVECTOR3 m_vLeftCharWaist;
	D3DXVECTOR3 m_vRightCharWaist;

	BYTE m_bAlreadyRecvSiegeAlarm;
	BYTE m_bTournamentGateOpened;
	BYTE m_bEnableNPCTick;

	// Camera setting
	BYTE m_bCamFIX;
	WORD m_wCamDIR;
	int m_nCamZOOM;

	// Additional main character moving setting (mouse input)
	D3DXPLANE m_vBOUND;
	FLOAT m_fBOUND;

	DWORD m_dwMoveGM;
	BYTE m_bMoveType;
	BYTE m_bAutoRun;

	// Relay
	DWORD m_dwRelayTick;
	BYTE m_bRevival;

	// Party message buf
	CString m_strReqNAME;
	BYTE m_bObtainType;

	// Mouse button state
	DWORD m_dwLButtonTick;
	DWORD m_dwRButtonTick;
	BYTE m_bLButtonDown;
	BYTE m_bRButtonDown;
	BYTE m_bDragSND;

	// Item trade message buf
	BYTE m_bInvenID;
	BYTE m_bSlotID;

	// Item move message buf
	BYTE m_bTargetInvenID;
	BYTE m_bTargetSlotID;

	// Item take message buf
	BYTE m_bMonSlotID;

	// Object square tick
	DWORD m_dwTSELECT;
	DWORD m_dwTSHADOW;

	// BGM data
	DWORD m_dwActiveVolume;
	DWORD m_dwENV;
	DWORD m_dwBGM;
	BYTE m_bStopBGM;

	// Dynamic help factor
	DWORD m_dwDHELPTICK;
	DWORD m_dwDHELP;

	// Popup menu setting
	BYTE m_bRSCSMENU;

	// System Info setting
	BYTE m_bTELEPORT;
	BYTE m_bShowUI;
	BOOL m_bShowDebugInfo;
	BOOL m_bShowFPSInfo;

	// Drag
	BOOL m_bNeedDrag;
	CPoint m_ptCurLBDown;

	// Sell
	INT m_nSellCount;

	// Change channel
	BOOL m_bDoChangeCH;
	DWORD m_dwLeftTickCH;
	BYTE m_bChannelID;

	DWORD m_dwLeftNoneInputLimitationTime;

	// Go back character select
	BOOL m_bDoSelectCHAR;
	DWORD m_dwLeftTickCHAR;

	// Auto Revival
	BOOL m_bDoAutoRevival;
	DWORD m_dwLeftTickAR;
	LONGLONG m_llPrevTSecond;

	// Game exit
	BOOL m_bDoEXIT;
	DWORD m_dwLeftTickEXIT;
	BOOL m_bDoKICKOUTMAP;
	DWORD m_dwLeftTickKICKOUTMAP;
	WORD m_wKICKOUTMAP;
	DWORD m_dwCmdParam;
	BOOL m_bOrgTKEY_ROT; 	// TKEY_CAM_ROT180, TKEY_CHAR_ROT180
	LPTOPENBYCASH m_pOpenShopByCash;
	INT m_nRunAwayIndex;
	TRUNAWAY_ARRAY m_vRunAway;
	CTClientKEY m_vKEY;

	BYTE m_bUseLButtonDblClick;
	BYTE m_bUseMButtonDblClick;
	BYTE m_bUseRButtonDblClick;
	BYTE m_bUseLButtonClick;
	BYTE m_bUseMButtonClick;
	BYTE m_bUseRButtonClick;
	BYTE m_bUseLButtonDown;
	BYTE m_bUseMButtonDown;
	BYTE m_bUseRButtonDown;
	BYTE m_bUseMouseWheel;
	BYTE m_bUseMouseMove;
	BYTE m_bUseKeyboardInput;
	BYTE m_bFirstConnect;

	BYTE m_bLockGM_CASHCAB_GETITEM;
	BYTE m_bCanUseSkill;
	BYTE m_bCanUseItem;

	CString m_strTacticsInvitationTarget;
	CString m_strCopyName;

	BYTE m_bItemLevelRevision;
	BYTE			m_bNPServerID;	// 게임가드.
	GG_AUTH_DATA	m_NPAuthData;	// 게임가드.
	DWORD m_dwRemainTickVerifyMaintain;

#ifdef ADD_RPS
	DWORD m_dwRPSGameType;
	BYTE m_bRPSWinCount;
	BOOL m_bRPSKeyEnable;
	BOOL m_bRPSGaming;
#endif

public:

	static const int MAX_FIRECRACKER = 4;

	DWORD m_dwFirecrackerID[MAX_FIRECRACKER];
	DWORD m_dwRemainLifeTimeFirecracker;
	void CalcFirecracker( DWORD dwTick );
	DWORD ShotFirecracker(
		WORD wTempID,
		DWORD dwLifeTime,
		INT nDIR );
	void ShotMissionWarFirecracker();

	void CalcWaitingRevivalItem( DWORD dwTick );
	DWORD m_dwWaitingRevivalItem;
	BOOL m_bShowingRevivalItemMessageBox;

	BOOL ShowChatDetailInfo(CTClientItem* pClientItem);
	
	TComponent* GetTopFrame(const CPoint& pt, UINT nFromTop, BYTE* outFrameID=NULL);
	BYTE GetFrameID(TComponent* pComp);
	
	TComponent* GetHitPartyItemLottery( CPoint point);
	void UpdateNameDetailTick( DWORD dwTick);
	void UpdateDetailNameInfo( CPoint point);

	void OnBeginDrag( CPoint point);
	void OnDragOver( CPoint point);
	void OnDrop( UINT nFlags, CPoint point);
	void DropItem( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );

	void DropItem_InvenToInven( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_InvenToItemshop( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_InvenToMainUI( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_InvenToTrade( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_InvenToItemSlot( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_InvenToPrivateSell(UINT nFlags, BYTE bTargetUI, BYTE bSlotID);
	void DropItem_InvenToMailSend(UINT nFlags, BYTE bTargetUI, BYTE bSlotID);
	void DropItem_InvenToCabinet(UINT nFlags, BYTE bTargetUI, BYTE bSlotID);
	void DropItem_InvenToGuildCabinet(UINT nFlags, BYTE bTargetUI, BYTE bSlotID);
	void DropItem_InvenToCashCabinet(UINT nFlags, BYTE bTargetUI, BYTE bSlotID);
	void DropItem_InvenToAuctionReg( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_InvenToDefault(UINT nFlags, BYTE bTargetUI, BYTE bSlotID);
	void DropItem_TradeToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_CabinetToInven(UINT nFlags, BYTE bTargetUI, BYTE bSlotID);
	void DropItem_PrivateSellToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_MailSendToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_SkillToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_RentSkillToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_MainUIToInven( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_MainUIToMainUI( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_MainUIToDefault( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_ShopToInven( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_MonInvenToInven( UINT nFlags, BYTE bTargetUI, BYTE bSlotID );
	void DropItem_PetToMain(UINT nFlags, BYTE bTargetUI, BYTE bSlotID);

	void ResetPlayerINFO( BYTE bContinue);
	void ResetRecallINFO( BYTE bContinue);
	void ResetRecallTIME( DWORD dwTick );
	void ResetTargetINFO( BYTE bContinue);
	void ResetTargetINFO( CTClientObjBase *pTARGET, BYTE bContinue);
	void ResetTargetINFO( LPTPARTY pTPARTY, BYTE bContinue);
	void ResetPartyINFO( BYTE bContinue);
	BYTE ResetRSCS( BYTE bRSCS);
	void ResetHUDTargetINFO( BYTE bContinue);

	void ResetTargetOBJ( CTClientObjBase *pTARGET);
	void ResetPartyOBJ( LPTPARTY pTPARTY);
	//void ResetDETAILINFO( CPoint point);

	void ResetHitOBJ( CPoint point, BOOL bChkDeadMon);
	BOOL TestHitOBJ(CTClientObjBase* pTOBJ, CTClientObjBase* pTCURHIT, CPoint point);

	CTClientObjBase* GetSkillTarget(CTClientObjBase* pTARGET, LPTSKILL pTSKILL);

	void ResetActOBJ( LPTSKILL pTSKILL);
	void ResetHitOBJ(
		LPLISTTOBJBASE pLIST,
		CPoint point);

	void BuildHitDEADMON(
		LPLISTTOBJBASE pLIST, 
		CPoint point);

	CTClientMonster* NextHitDEADMON();

	void ResetTMAINTAIN(
		CTClientObjBase *pTOBJ,
		CTMaintainPannel *pUI);
	void ResetTMAINTAIN();
	void ResetTSKY();

	void ResetTMonInvenUI();
	void ResetCharInfoUI();
	void ResetNPCTalkUI();
	void ResetQuestUI();
	void ResetInvenUI();
	void ResetSkillUI();
	void DisableTargetUI();
	void CloseAllUI();

	void ResetLeaderMark();
	void ResetSkillICON();
	void ClearMonInven();

	BOOL CanRidePet(CTClientPet* pClientPet);
	BOOL CanDuel(CTClientChar* pTARGET);

	void SelectHotKey( THOTKEY_BASE eTYPE, BYTE bHotKey);
	void SelectParty( BYTE nIndex);
	void SelectTRSCS( DWORD wKEY);

	void EnableAllInputDevice( BYTE bEnable = TRUE);
	BOOL IsBusy();

	void DisableUI( BYTE bUI, BOOL bShow = FALSE);
	void EnableUI( BYTE bUI);
	void ToggleUI( BYTE bUI);
	void CheckPOPUP();

	void DoPOPUP(LPTSKILL pTSKILL);
	void DoPOPUP( LPTPARTY pTPARTY);
	void DoPOPUP( CTClientObjBase *pTOBJ);
	void DoPOPUP( LPTSQUAD pTSQUAD);
#ifdef MODIFY_CNP_NAME
	void DoEditPOPUP( TEdit* pEDIT, CPoint pt);
#endif
	void DoPOPUP( CString strTarget );

	void ShowPOPUP();

	void BuildRECALLMENU( CTClientRecall *pTRECALL);
	void BuildPCMENU( CTClientChar *pTCHAR);
	void BuildNPCMENU( CTClientNpc *pTNPC);
	void BuildMAINCHARMENU();

	void CheckAttackMaintain(
		CTClientObjBase *pTATTACK,
		LPTSKILL pTSKILL);
	void EnableChat( BYTE bEnable);

	void CheckAutoSKILL( CTClientRecall *pTRECALL);
	void CheckQuestOBJ();
	void CheckMainUI();
	void CheckHUD();
	
	void CancelTrade();
	void CancelPrivateBuy();
	void CancelPrivateSell();
	void CancelAllItemUI();

	BOOL IsItemUI(CTClientUIBase* pUI);

	void DoHotKey(THOTKEY_BASE eTYPE, BYTE bHotkeyID);

	void DoDEFACTION();
	void DoCHAT();

	void SkillMSG(
		CTClientObjBase *pTATTACK,
		BYTE bERROR);

	void CorpsMSG(
		CTClientObjBase *pTOBJ,
		BYTE bERROR);

	void TradeMSG(
		CString strNAME,
		BYTE bERROR);
	void ItemMSG( BYTE bERROR);

	void ShotSkill( 
		CTClientObjBase *pTOBJ, 
		LPTSKILLDATA pSKILLDATA);
	void ShotSkill( 
		LPTSKILL pTSKILL, 
		CTClientObjBase * pTOBJ, 
		LPTSKILLDATA pSKILLDATA,
		VTSKILLTARGET* pSKILLTARGETS );
	
	DWORD FindOBJPIVOT(
		CTClientObjBase* pTOBJ, 
		LPTITEMVISUAL pTVISUAL);

	void ShotArrow(
		CTClientObjBase *pTOBJ,
		LPTSKILLDATA pSKILLDATA,
		LPTSKILL pTSKILL);

	void RangeSHOT(
		CTClientObjBase *pTATTACK,
		LPTSKILLDATA pTSKILLDATA,
		LPD3DXVECTOR3 pTGROUND,
		LPTSKILL pTSKILL,
		DWORD dwTick,
		BOOL bBroadCast );

	void LineSHOT(
		CTClientObjBase *pTATTACK,
		LPTSKILLDATA pTSKILLDATA,
		LPD3DXVECTOR3 pTGROUND,
		LPTSKILL pTSKILL,
		DWORD dwTick,
		BOOL bBroadCast );

	BOOL Defend(
		CTClientObjBase *pTATTACK,
		CTClientObjBase *pTDEFEND,
		LPTSKILLDATA pTSKILLDATA,
		LPD3DXVECTOR3 pTGROUND,
		LPTSKILL pTSKILL,
		DWORD dwTick,
		BOOL bBroadCast );

	void SetRecallTarget(
		DWORD dwTargetID,
		BYTE bTargetType,
		BYTE bSKILL);

	void MoveCtrlHost(
		WORD wPITCH,
		WORD wDIR,
		BYTE bMouseDIR,
		BYTE bKeyDIR);

	void UseTItem( WORD wTemplateID);
	void UseTItem(
		BYTE bInvenID,
		BYTE bSlotID);

	INT  m_nNextPetID;
	BYTE m_bPetUseByHotbar;
	void UseTPet( WORD wPetID);

	void CancelCharge( BYTE bClearActOBJ);
	void CancelDoEXIT();
	void BeginCharge( DWORD dwTick, TCHARGETYPE);

	BOOL CheckSkillHasFunc( LPTSKILL pTSKILL,
		BYTE bType,
		BYTE bFunctionID );

	void NotifyObjHide( CTClientObjBase *pTARGET);
	void NotifyObjDie( CTClientObjBase *pTARGET);
	void LoopTSKill( CTClientSkill *pTSKILL);

	BOOL UseTRecallSkill(
		CTClientRecall *pTRECALL,
		DWORD dwTick);

	void UseTSkill(
		WORD wID,
		BYTE bActivate,
		BYTE bCheckGround);

	BOOL FindSkillTarget(
		CTClientObjBase* pATTACK,
		CTClientObjBase* pDEFEND, 
		LPD3DXVECTOR3 pGROUND, 
		LPTSKILL pTSKILL,
		LPTSKILLTARGET_PARAM pRESULT);

	void BuySKILL( BYTE bSlotID);

	void EnableSkillTimer(CTClientObjBase *pATTACK, CTClientSkill *pTSkill, BOOL bEnable);
	void ApplySkillTick(CTClientObjBase *pATTACK, CTClientSkill *pTSkill);

	void Revival(
		CTClientObjBase *pREVIVAL,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);
	void LoopSkillOFF();

	void LookAt(
		WORD wDIR,
		WORD wPITCH);

	void DoRecallSKILL( DWORD wKEY);
	void DoRecallAI( DWORD wKEY);

	DWORD GetObjectColor( CTClientObjBase *pTOBJ);
	WORD GetPursuit( CTClientObjBase *pTOBJ);
	BYTE GetHotKey( WORD wKEY);
	BYTE CloseTopFrame();
	BYTE GetTopFrame();

	void DeleteFixRecall( DWORD dwID, bool bDeleteDirect=true );
	BYTE DeleteRecall( DWORD dwID, bool bDeleteDirect=true );
	BYTE CheckRevival(LPD3DXVECTOR3 vTARGET);
	BYTE CheckDEFMSG(
		CTClientObjBase *pTATTACK,
		CTClientObjBase *pTDEFEND,
		BOOL bBroadCast=TRUE );

	BYTE IsTSKILLCharge();
	BYTE IsHostingOBJ(
		DWORD dwID,
		BYTE bType);

	BYTE CanDetect(
		CTClientObjBase *pSEEKER,
		CTClientObjBase *pTARGET);

	BYTE CanCharacterSee(
		CTClientObjBase *pSEEKER,
		CTClientObjBase *pTARGET);

	BYTE CanAttackCountry(
		CTClientMonster *pMONSTER,
		CTClientObjBase *pTARGET);

	BYTE CanFollow( CTClientObjBase *pTARGET);
	BYTE CanFollow(
		CTClientMonster *pTSEEKER,
		CTClientObjBase *pTARGET);
	BYTE CanUseMail();

	BYTE IsCtrlRECALL( CTClientRecall *pTOBJ);
	BYTE IsInBattle(CTClientObjBase *pTATTACK, CTClientObjBase *pTDEFEND);

	WORD GetPartyID( CTClientObjBase *pTOBJ);
	BYTE GetInvenID( BYTE bFrameID);
	BOOL FindEmptyInven(BYTE& outInven, BYTE& outSlot);
	BOOL FindInven(TITEM_TYPE eType, TITEM_KIND eKind, BYTE& outInven, BYTE& outSlot);
	BOOL CheckInvenItem(BYTE bFrameID, BYTE bSlotID);
	BOOL CheckInvenItem(WORD wItemID, BYTE* pbInvenID=NULL, BYTE* pbItemID=NULL);

	CTClientItem* FindMainItemByFrame(BYTE bFrameID, BYTE bSlotID);
	CTClientItem* FindMainItemByInven(BYTE bInvenID, BYTE bSlotID);

	BYTE GetFACE(
		BYTE bRaceID,
		BYTE bSexID,
		BYTE bFaceID,
		BYTE bHairID);

	BYTE GetTargetKind(
		CTClientObjBase *pATTACK,
		CTClientObjBase *pDEFEND,
		CTClientObjBase *pTARGET,
		LPTSKILL pTSKILL,
		LPD3DXVECTOR2 pPOS);

	BOOL DoActCmd(const CString& strActCmd);
	
	void EnableGuildUI(BOOL bEnable);

	void NotifyNewMsgChat(CTMessengerChat* pChat);
	void NotifyNewMail();

	void TakeUpPet(DWORD dwCharID, DWORD dwMonID);

	BOOL IsInvenFrame(BYTE bInven);
	void EnableAllSubInven(BOOL bEnable);

	void ShowInfoChat(const CString& strMSG, TUISND_TYPE eSndType=TUISND_TYPE_NONE);
	void ShowInfoMsgBox(const CString& strMSG, BOOL bModal, TUISND_TYPE eSndType=TUISND_TYPE_NONE);

	void DoMainRunAway(BOOL bSTART);
	void DoRecallRunAway(CTClientRecall*, BOOL bSTART);
	void CalcMainRunAway(DWORD dwTick);
	void MoveMainChar( const CPoint &ptScreen, BOOL bShowPointer=TRUE );
	
	void GetRandomPosition(
		FLOAT& fPosX,
		FLOAT& fPosY,
		FLOAT& fPosZ, 
		CTClientObjBase* pTOBJ,
		FLOAT fRange, 
		FLOAT fMaxOffset);

	void CheckRelay(DWORD dwTick);
	int OnSessionMsg( CTachyonSession *pSession, CPacket *pPacket);
	int OnCommand( TCOMMAND cmd);

	void OnActivateKEY( WORD wSCAN);
	void OnReleaseKEY( WORD wSCAN);
	void OnRegionChanged();

	void ReleaseForTELEPORT();
	void ReleaseNumber();
	void ReleaseParty();
	void ReleaseArrow();
	void ReleaseGame();

	LRESULT OnOBJMsg( LPOBJMSG pMSG);

	LRESULT OnOBJActEndMsg( LPOBJMSG pMSG );
	void OnOBjShotMsg( CTClientObjBase * pTOBJ, BOOL bIsBackSkill );
	
	// Utility
	CTClientObjBase *SeekRecallTarget( CTClientRecall *pRECALL);
	CTClientObjBase *SeekMonTarget( CTClientMonster *pMON);
	CTClientMonster *FindMonster( DWORD dwID);
	CTClientMonster *FindSlave( DWORD dwID);
	CTClientRecall *FindSlaveFixRecall( DWORD dwID);
	CTClientRecall *FindSlaveRecall( DWORD dwID);
	CTClientRecall *FindFixRecall( DWORD dwID);
	CTClientRecall *FindRecall( DWORD dwID);
	CTClientSwitch *FindSwitch( DWORD dwID);
	CTClientGate *FindGate( DWORD dwID);
	CTClientChar *FindPlayer( DWORD dwID);
	CTClientChar *FindPC( DWORD dwID);
	LPTPARTY FindParty( DWORD dwID);

	WORD GetEaseInvenPos( CTClientItem *pTITEM);
	WORD GetBlankInvenPos();
	WORD GetBlankInvenPosExceptInven( BYTE bExceptedInven );
		
	TEdit *GetCurEdit();
	
	BYTE CheckBackSkill( LPTSKILL pTBACKSKILL, BYTE bBackSkillItemType );

	BYTE CheckSkillTarget(
		CTClientObjBase *pTATTACK,
		CTClientObjBase *pTDEFEND,
		LPTSKILL pTSKILL);

	BYTE CheckSkillItem(LPTSKILL pTSKILL);

	BYTE CanUseSkill(
		CTClientObjBase *pTATTACK,
		CTClientObjBase *pTDEFEND,
		LPD3DXVECTOR3 pTGROUND,
		LPTSKILL pTSKILL);

	BYTE CanDefend(
		CTClientObjBase *pTATTACK,
		CTClientObjBase *pTDEFEND,
		LPTSKILL pTSKILL);

	void StopMoveMainChar();
	void ExitTutorial();

	//BYTE CheckTradeItem( WORD wItemPOS);
	BYTE PannelHitTest( CPoint point);
	BYTE CanControl();
	BYTE CanLookAt();

	BYTE POSInRECT(
		LPD3DXVECTOR3 pTPOS,
		LPD3DXPLANE pTRECT);

	CTClientObjBase *FindObject(
		DWORD dwID,
		BYTE bType);

	CTClientObjBase *FindOBJ(
		DWORD dwID,
		BYTE bType);

	CTClientMoveObj *GetCtrlOBJ();
	CTClientRecall *GetMainRecall();
	CTClientPet *GetMainPet();

	BOOL IsPetRecalled( WORD wPetID );

	D3DXVECTOR3 GetWarpTARGET(
		CTClientObjBase *pTOBJ,
		LPD3DXVECTOR3 pDIR,
		FLOAT fMOVE);

	void LerpTLIGHT(
		LPTLIGHT pTLIGHT,
		BYTE bSetID,
		BYTE bIndex,
		BYTE bUnitX,
		BYTE bUnitZ,
		FLOAT fTIME);

	void LerpTFOG(
		LPTFOG pTFOG,
		BYTE bUnitX,
		BYTE bUnitZ,
		FLOAT fTIME);

	void LerpTFOG(
		LPTFOG pTFOG,
		LPTFOG pTBLEND,
		FLOAT fTIME);

	void DeleteDynimicOBJ( CTClientObjBase *pTOBJ);
	void PlayUISound( DWORD dwSndID);
	void SayToAll( CPacket& packet);
	void SayToSvr( CPacket& packet, BYTE bSvrID);
	void TCapture();

	// RSCS Utility
	BYTE IsHostingEnemy( CTClientObjBase *pTOBJ);
	BYTE IsRSCSEnemy( CTClientObjBase *pTOBJ);

	BYTE IsVisibleEnemy( LPTENEMY pTENEMY);
	BYTE HitRSCSOBJ(
		CPoint point,
		WORD wPosX,
		WORD wPosZ,
		FLOAT fSCALE);

	void ResetTRSCSCMD(
		CTClientObjBase *pTMARK,
		CTClientObjBase *pTDIR,
		CTClientArrow *pTARROW,
		BYTE bTCMD,
		DWORD dwTargetID,
		BYTE bTargetType,
		WORD wTargetX,
		WORD wTargetZ);

	void CheckTSQUADDIR( LPTSQUAD pTSQUAD);
	void RefreshTRSCSCMD();
	void ReleaseRSCS();

	void ReportTRSCSMOVE( CTClientMoveObj *pTOBJ);
	void ReportENEMY();

	void CheckTRSCS(
		UINT nFlags,
		CPoint point);

	void ExecTCMDFromTOBJ(
		CTClientObjBase *pTOBJ,
		BYTE bToALL);

	void ExecTCMDFromParty(
		BYTE bIndex,
		BYTE bToALL);

	void ExecTCMDFromPoint(
		int nPosX,
		int nPosY,
		BYTE bToALL);

	void ExecTCORPSCMD(
		LPTSQUAD pTSQUAD,
		BYTE bTCMD,
		DWORD dwTargetID,
		BYTE bTargetType,
		WORD wTargetX,
		WORD wTargetZ,
		WORD wMapID);

	void ExecTSQUADCMD(
		WORD wSquadID,
		BYTE bTCMD,
		DWORD dwTargetID,
		BYTE bTargetType,
		WORD wTargetX,
		WORD wTargetZ,
		WORD wMapID);

	void ExecTSQUADCMD(
		LPTSQUAD pTSQUAD,
		BYTE bTCMD,
		DWORD dwTargetID,
		BYTE bTargetType,
		WORD wTargetX,
		WORD wTargetZ,
		WORD wMapID);

	void ExecTUNITCMD(
		LPTUNIT pTUNIT,
		WORD wSquadID,
		BYTE bTCMD,
		DWORD dwTargetID,
		BYTE bTargetType,
		WORD wTargetX,
		WORD wTargetZ,
		WORD wMapID);

	void ExecTCMD(
		BYTE bTCMD,
		DWORD dwTargetID,
		BYTE bTargetType,
		WORD wTargetX,
		WORD wTargetZ,
		WORD wMapID,
		BYTE bToALL);

	LPTENEMY PickTCMDATTACK(
		LPMAPTENEMY pTENEMYMAP,
		CPoint point,
		FLOAT fSCALE);

	WORD PickTCMDPROTECT(
		CPoint point,
		FLOAT fSCALE);

	D3DXVECTOR2 PickTCMDMOVE(
		CPoint point,
		FLOAT fSCALE);

	// Scene calculator
	void RotCameraX( int nCamRotX);
	void RotCameraY( int nCamRotY);

	void BuildMaintainList();
	void CheckMaintainOBJ();

	void CheckTPORTAL(
		CTClientObjBase *pTOBJ,
		LPD3DXVECTOR3 pPREV,
		LPD3DXVECTOR3 pNEXT,
		FLOAT fNextH);

	BYTE CheckGroundDRAW();
	void ResetGroundPOS(
		int nPosX,
		int nPosY);

	void ResetTPOINTER(
		int nPosX,
		int nPosY);

	void CalcTournament( DWORD dwTick);
	void CalcTournamentChar( DWORD dwTick);

	void CalcFollowMove( DWORD dwTick);
	void CalcTargetMove( DWORD dwTick);
	void CalcReuseTick( DWORD dwTick);
	void CalcClickMove( DWORD dwTick);
	void CalcMainChar( DWORD dwTick);

	void CalcDynamicHelp( DWORD dwTick);
	void CalcMaintainOBJ( DWORD dwTick);
	void CalcGHOSTScene( DWORD dwTick);
	void CalcTQuestTick( DWORD dwTick);
	void CalcFLASHTick( DWORD dwTick);
	void CalcGlobalTEX( DWORD dwTick);
	void CalcTRSCS( DWORD dwTick);
	void CalcGhost( DWORD dwTick);
	void CalcBGM( DWORD dwTick);

	BOOL CheckDuelRange();
	void CalcDUEL(DWORD dwTick);

	void CalcTSQUADRSCS(
		LPTSQUAD pTSQUAD,
		DWORD dwTick);

	void CalcTCMDDIR(
		LPD3DXVECTOR3 pTARGET,
		DWORD dwTick);

	void CalcTOBJDamage(
		CTClientObjBase *pTOBJ,
		DWORD dwTick);

	void CalcTOBJHit(
		CTClientObjBase *pTOBJ,
		BYTE bHit,
		DWORD dwTick);

	void CalcTSKY(
		FLOAT fWaterHeight,
		DWORD dwTick);

	void CalcSLAVERECALL( DWORD dwTick);
	void CalcSLAVEMON( DWORD dwTick);
	void CalcMONSTER( DWORD dwTick);
	void CalcTSWITCH( DWORD dwTick);
	void CalcPLAYER( DWORD dwTick);
	void CalcRECALL( DWORD dwTick);
	void CalcBREATH( DWORD dwTick);
	void CalcCHARGE( DWORD dwTick);
	void CalcNUMBER( DWORD dwTick);
	void CalcARROW( DWORD dwTick);
	void CalcSCENE( DWORD dwTick);

	void CalcSLAVERECALL(
		CTClientRecall *pTRECALL,
		LPD3DXMATRIX pTDIR,
		LPVTMONMOVE pTMONMOVE,
		LPVTRECALL pTATTACK,
		BYTE bSYNC,
		BYTE bPATH,
		DWORD dwTick);

	void CalcRECALL(
		CTClientRecall *pTRECALL,
		DWORD dwTick);

	FLOAT CalcCamera( DWORD dwTick);
	FLOAT CheckCamPos( DWORD dwTick);

	
	void CalcNoneInputLimitation(DWORD dwTick);
	void CalcChangeChannel(DWORD dwTick);
	void CalcSelectCharacter(DWORD dwTick);
	void CalcExitGame(DWORD dwTick);
	void CalcKickoutMap(DWORD dwTick);

	void DoChangeChannel(BYTE bChannel);
	void DoSelectCharacter();

	BYTE FixCamera( DWORD dwTick);
	BYTE CheckGHOSTScene();

	void DoQUAKE( BYTE bQUAKE);
	void EndQUAKE();

	void ResetVisibleOBJ( DWORD dwTick);
	void ResetTFOG( BYTE bWATER);
	void ResetTARGET();

	void UpdateItemUpGauge(DWORD dwTick);
	void UpdateProgressGauge(DWORD dwTick);

	// Object pick function
	void BuildPickList(
		LPLISTTOBJBASE pLIST, 
		TPICK ePick );

	void BuildPickList(
		LPLISTTOBJBASE pLIST,
		LPLISTTOBJBASE pTSRC,
		TPICK ePick );

	void PickNext(TPICK ePick);

	// Scene prepare function
	void ResetDetect( CTClientObjBase *pTOBJ);
	void PushDrawOBJ( CTClientObjBase *pTOBJ);

	void ClearDrawCount();
	void IncreaseDrawCount( CTClientObjBase *pTOBJ);
	
	const D3DMATERIAL9& GetMaterial( CTClientObjBase *pTOBJ);
	
	void SetMaterial( const D3DMATERIAL9& vMAT);
	void SetMaterial( CTClientObjBase *pTOBJ);

	void EnableLIGHT( CTClientBSP *pTBSP);
	void EnableLIGHT();
	void DisableLIGHT();

	void ResetSCENEConstant();
	void BeginGHOSTScene();
	void EndGHOSTScene();

	void SetAmbient(
		BYTE bRValue,
		BYTE bGValue,
		BYTE bBValue);

	// Scene render function
	void RenderTSQUARE(
		CTClientObjBase *pTOBJ,
		LPD3DXMATRIX pUV,
		LPD3DXVECTOR2 pDIR,
		FLOAT fSCALE);

	void RenderTSQUARE(
		LPD3DXVECTOR3 pPOS,
		LPD3DXVECTOR2 pSIZE,
		LPD3DXMATRIX pUV,
		LPD3DXVECTOR2 pDIR,
		FLOAT fSCALE,
		BYTE bLand);

	void RenderTTEXT( CTClientObjBase *pTOBJ);
	void RenderTTEXT( LPLISTTOBJBASE pLIST);
	void RenderTTEXT( BYTE bIsEXTVisible);
	D3DXVECTOR3 GetBaseNamePosition(CTClientObjBase *pTOBJ);

	void ResetObjUserTitlePos( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ);
	void ResetObjNamePos( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ);

	BYTE HitTestObjName( CTClientObjBase *pTOBJ, const CPoint& pt);

	BYTE RenderTICON( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ, FLOAT fBaseHeight, FLOAT fLEFT);
	BYTE RenderTCAMP( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ, FLOAT fBaseHeight, FLOAT fRIGHT);
	BYTE RenderTSOULMATE( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ, FLOAT fBaseHeight, FLOAT fRIGHT);
	BYTE RenderTSELMARK( CTClientObjBase *pTOBJ, const D3DXVECTOR3 &vPROJ, FLOAT fBaseHeight, FLOAT fLEFT, FLOAT fRIGHT);

	void RenderTOBJ(
		CTClientObjBase *pTOBJ,
		DWORD dwTick);

	void RenderTOBJ(
		LPLISTTOBJBASE pLIST,
		DWORD dwTick,
		BYTE bDrawGROUND);

	void RenderTSILHOUETTE();
	void RenderTNUMBER();
	void RenderTCMDDIR();
	void RenderTSLASH();
	void RenderFLASH();
	void RenderARROW();
	void RenderBSP();

	BYTE RotateCamera( CPoint &point, CPoint &pt, BYTE bMOVE );
	BYTE RotateCharacter( CPoint &pt, CPoint &point, BYTE bMOVE );

	D3DXVECTOR3 ScreenClickToWorldPosition(
		CPoint,
		CTClientObjBase *pOBJ);

	void ChangeResolution(
		INT nScreenX,
		INT nScreenY );

	CTClientSession * RelaySession();
	CTClientSession * GetRelaySession() { return m_pRelay; }

	void CheckOpenCash();
	BOOL CheckOpenCash( LPTOPENBYCASH, TNPC_TYPE );

	void CheckItemupUseCashItem();
	void CheckRefineUseCashItem( DWORD dwCost, BYTE bDiscountRate );

#ifdef ADD_SNOW
	DWORD m_dwSnowEventEffect;
	BOOL m_bSnowEvent;
	BOOL m_bSnowEventUserOn;
	void StartSnowEvent();
	void EndSnowEvent();
#endif

	void ResetKICKOUTMAP();

	void CheckMonthRank( CTClientChar* pTCHAR );
	void UpdateMonthRank();

	void SetTournamentWatching();
	void ResetTournamentWatching();
	void CheckValidHeightInfo();

public:
	BOOL m_bResetMAP;
	void SetResetMAP( BOOL bResetMAP );

// Msg Sender (send to all enabled map)
public:
	void SendCS_CONREADY_REQ(BYTE bSvrID);
	void SendCS_REVIVAL_REQ(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		BYTE bType);

	void SendCS_BLOCK_REQ(
		DWORD dwObjID,
		BYTE bObjType,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wPITCH,
		WORD wDIR,
		BYTE bAction,
		BYTE bBlock);

	void SendCS_JUMP_REQ(
		DWORD dwObjID,
		BYTE bObjType,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wPITCH,
		WORD wDIR,
		BYTE bAction);

	void SendCS_MOVE_REQ(
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wPITCH,
		WORD wDIR,
		BYTE bMouseDIR,
		BYTE bKeyDIR,
		BYTE bAction,
		BYTE bGhost);

	void SendCS_MONMOVE_REQ(
		CTClientObjBase *pTMON,
		BYTE bChannel,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wPITCH,
		WORD wDIR,
		BYTE bMouseDIR,
		BYTE bKeyDIR,
		BYTE bAction);
	void SendCS_MONMOVE_REQ( LPVTMONMOVE pTMONMOVE);

	void SendCS_ENTERLB_REQ(
		DWORD dwHostID,
		DWORD dwTargetID,
		BYTE bTargetType,
		DWORD dwMonID,
		BYTE bChannel,
		WORD wMapID);

	void SendCS_ENTERAB_REQ(
		DWORD dwHostID,
		DWORD dwTargetID,
		BYTE bTargetType,
		DWORD dwMonID,
		BYTE bChannel,
		WORD wMapID);

	void SendCS_LEAVELB_REQ(
		DWORD dwHostID,
		DWORD dwTargetID,
		BYTE bTargetType,
		DWORD dwMonID,
		BYTE bChannel,
		WORD wMapID);

	void SendCS_LEAVEAB_REQ(
		DWORD dwHostID,
		DWORD dwTargetID,
		BYTE bTargetType,
		DWORD dwMonID,
		BYTE bChannel,
		WORD wMapID);

	void SendCS_DEFEND_REQ(
		DWORD dwHostID,
		DWORD dwAttackID,
		DWORD dwTargetID,
		BYTE bAttackType,
		BYTE bTargetType,
		WORD wPartyID,
		DWORD dwActID,
		DWORD dwAniID,
		BYTE bChannel,
		WORD wMapID,
		BYTE bAtkLevel,
		DWORD dwMinAP,
		DWORD dwMaxAP,
		DWORD dwMinMP,
		DWORD dwMaxMP,
		WORD wSTR,
		WORD wINT,
		BYTE bCURSE,
		BYTE bSpecial,
		BYTE bAglow,
		BYTE bCountryID,
		WORD wAL,
		BYTE bHIT,
		WORD wSkillID,
		BYTE bLevel,
		FLOAT fAtkPosX,
		FLOAT fAtkPosY,
		FLOAT fAtkPosZ,
		FLOAT fDefPosX,
		FLOAT fDefPosY,
		FLOAT fDefPosZ,
		DWORD dwTick);

	void SendCS_ACTION_REQ(
		DWORD dwObjID,
		BYTE bObjType,
		BYTE bActionID,
		DWORD dwActID,
		DWORD dwAniID,
		BYTE bChannel,
		WORD wMapID,
		WORD wSkillID);

	void SendCS_MONATTACK_REQ(
		DWORD dwAttackID,
		DWORD dwTargetID,
		BYTE bAttackType,
		BYTE bTargetType,
		DWORD dwActID,
		DWORD dwAniID,
		BYTE bChannel,
		WORD wMapID);

	void SendCS_MONITEMLIST_REQ(
		BYTE bOpen,
		DWORD dwMonID);

	void SendCS_MONMONEYTAKE_REQ(
		CTClientObjBase* pTARGET);

	void SendCS_MONITEMTAKE_REQ(
		DWORD dwMonID,
		BYTE bMonSlotID,
		BYTE bInvenID,
		BYTE bSlotID);

	void SendCS_MONITEMLOTTERY_REQ(
		BOOL bJoinLottery,
		DWORD dwMonID,
		WORD wItemID);

	void SendCS_MONITEMTAKEALL_REQ(
		CTClientObjBase* pTARGET);

	void SendCS_SKILLUSE_REQ(
		CTClientObjBase *pATTACK,
		CTClientObjBase *pDEFEND,
		LPD3DXVECTOR3 pGROUND,
		LPTSKILL pTSKILL,
		BOOL bTimerON,
		LPTSKILLTARGET_PARAM pPARAM);

	void SendCS_LOOPSKILL_REQ(
		CTClientObjBase *pATTACK,
		CTClientObjBase *pDEFEND,
		LPD3DXVECTOR3 pGROUND,
		LPTSKILL pTSKILL);

	void SendCS_ITEMUSE_REQ(
		LPD3DXVECTOR2 pPOS,
		WORD wTempID,
		BYTE bInvenID,
		BYTE bSlotID,
		WORD wDelayGroupID);

	void SendCS_REGION_REQ(
		DWORD dwID,
		BYTE bType,
		BYTE bChannel,
		WORD wMapID,
		DWORD dwRegionID,
		WORD wLocalID);

	void SendCS_SKILLEND_REQ(
		DWORD dwObjID,
		BYTE bObjType,
		DWORD dwHostID,
		DWORD dwAttackID,
		BYTE bAttackType,
		WORD wSkillID,
		WORD wMapID,
		BYTE bChannel);

	void SendCS_REVIVALASK_REQ(
		BYTE bReply,
		DWORD dwAttackID,
		BYTE bAttackType,
		WORD wSkillID,
		BYTE bLevel);

	void SendCS_DROPDAMAGE_REQ(
		DWORD dwID,
		BYTE bType,
		DWORD dwDamage,
		BYTE bChannel,
		WORD wMapID);
	
	void SendCS_COMMENT_REQ(
		CString& strComment);

	// Load command Handler
	void DoTLOADCMD_OBJ(
		CTClientApp *pTAPP,
		LPTLOADCMD pTCMD);

	// Menu Handler
	int OnGM_SYSTEM_SETTING();
	int OnGM_CONTROL_SETTING();
	int OnGM_GAME_SETTING();
	int OnGM_OPEN_WORLDMAP();
	int OnGM_DIVIDE_ITEM();
	int OnGM_MAIN_MENU();
	int OnGM_SYSTEM();
	int OnGM_QUIT();
	int OnGM_GOBACK_CHARSEL();
	int OnGM_BEGIN_EXIT();

	int OnGM_BASIC_INFO();
	int OnGM_MORE_INFO();

	int OnGM_BLOCK_REQ();
	int OnGM_UNBLOCK_REQ();

	int OnGM_CLOSE_POPUP();
	int OnGM_CLOSE_UI();

	int OnGM_PARTYMEMBERRECALL();
	int OnGM_USEITEM_PARTYMEMBERRECALL();
	int OnGM_USEITEM_MOVETOCHAR();
	int OnGM_PARTYMEMBERRECALLANS_OK();
	int OnGM_PARTYMEMBERRECALLANS_NO();
	int OnGM_PARTY_SETCHIEF();
	int OnGM_FARPARTY_SETCHIEF();
	int OnGM_PARTY_EXIT();
	int OnGM_PARTY_KICK();
	int OnGM_PARTY_REQ();
	int OnGM_PARTY_OK();
	int OnGM_PARTY_NO();
	int OnGM_INVITE_FRIEND_TO_PARTY();
	int OnGM_CHANGE_PARTY_ITEM_TYPE();		

	int OnGM_CORPS_SETCOMMANDER();
	int OnGM_CORPS_LEAVE();
	int OnGM_CORPS_REQ();
	int OnGM_CORPS_OK();
	int OnGM_CORPS_NO();

	int OnGM_REVIVAL_YES();
	int OnGM_REVIVAL_NO();
	int OnGM_REVIVAL();

	int OnGM_SET_RETURNPOS();
	int OnGM_GOTO_THUMB();
	int OnGM_USEITE_REVIVAL(); 
	int OnGM_ATTACK();

	int OnGM_OPEN_SKILLINFO();
	int OnGM_OPEN_PARTYINFO();
	int OnGM_OPEN_GUILDINFO();
	int OnGM_OPEN_CHARINFO();
	int OnGM_OPEN_MONINVEN();
	int OnGM_JOIN_ITEM_LOTTERY();
	int OnGM_GIVEUP_ITEM_LOTTERY();

	int OnGM_ACTIVATE_SWITCH();
	int OnGM_NPC_REVIVAL();
	int OnGM_NPC_TRADE();
	int OnGM_NPC_TALK();
	int OnGM_CRAFT_OK();
	int OnGM_CRAFT();

	int OnGM_TRADE_LOCK();
	int OnGM_TRADE_YES();
	int OnGM_TRADE_NO();
	int OnGM_TRADE_OK();
	int OnGM_TRADE_REQ();

	int OnGM_SKILLLEVEL_1();
	int OnGM_SKILLLEVEL_2();
	int OnGM_SKILLLEVEL_3();
	int OnGM_SKILLLEVEL_4();
	int OnGM_SKILLLEVEL_5();
	int OnGM_SKILLLEVEL_6();
	int OnGM_SKILLLEVEL_7();
	int OnGM_SKILLLEVEL_8();
	int OnGM_SKILLLEVEL_9();
	int OnGM_SKILLLEVEL_10();
	int OnGM_SKILLLEVEL_11();
	int OnGM_SKILLLEVEL_12();
	int OnGM_SKILLLEVEL_13();
	int OnGM_SKILLLEVEL_14();
	int OnGM_SKILLLEVEL_15();
	int OnGM_SKILLLEVEL_16();
	int OnGM_SKILLLEVEL_17();
	int OnGM_SKILLLEVEL_18();

	int OnGM_WARN_GRADE_ITEM_DROP();
	int OnGM_DROP_ITEM_YES();
	int OnGM_DROP_ITEM_NO();
	int OnGM_ITEM_TAKE();
	int OnGM_ITEM_SELL();
	int OnGM_CONFIRM_ITEM_SELL();
	int OnGM_ITEM_BUY();
	int OnGM_PORTAL_REQ();
	int OnGM_GETMONEY();
	int OnGM_GETALL();
	int OnGM_MONSTER_BUY();
	int OnGM_MONSTER_DO_BUY();

	int OnGM_WARN_GRADE_ITEM_SELL();
	int OnGM_WARN_UPITEM_SELL_OK();
	int OnGM_WARN_UPITEM_SELL_CANCEL();
	int OnGM_WARN_UPITEM_SELL_DIFFERENT_ID();

	int OnGM_ITEM_DO_SELL();
	int OnGM_ITEM_DO_BUY();

	int OnGM_HOTKEY_DOWN();
	int OnGM_HOTKEY_UP();

	int OnGM_TOGGLE_INVEN_SLOT();
	int OnGM_TOGGLE_MAIN();

	int OnGM_TOGGLE_SKILLINFO();
	int OnGM_TOGGLE_QUESTINFO();
	int OnGM_TOGGLE_CHARINFO();
	int OnGM_TOGGLE_COMMUNITY();
	int OnGM_TOGGLE_DEFINVEN();
	int OnGM_TOGGLE_MINIMAP();
	int OnGM_TOGGLE_SIEGE();
	int OnGM_TOGGLE_CMDMAP();
	int OnGM_TOGGLE_HIDE_SQUAD();
	int OnGM_TOGGLE_PETMANAGE();
	int OnGM_TOGGLE_HELP();

	int OnGM_QUEST_ACCEPT();
	int OnGM_QUEST_GIVEUP();
	int OnGM_EXPAND_QUEST();
	int OnGM_EXPAND_NPCQUEST();
	int OnGM_QUEST_CRAFT();

	int OnGM_MINIMAP_ZOOMOUT();
	int OnGM_MINIMAP_ZOOMIN();
	int OnGM_SELECT_WORLDMAP();

	int OnGM_RECALL_PASSIVE_AI();
	int OnGM_RECALL_ACTIVE_AI();
	int OnGM_RECALL_MANUAL_AI();
	int OnGM_RECALL_STAY_AI();
	int OnGM_RECALL_BACK_AI();
	int OnGM_DEL_RECALL();

	int OnGM_TOGGLE_CHAT_LOCK();
	int OnGM_TOGGLE_CHAT_LANG();
	int OnGM_TOGGLE_ACTLIST();
	int OnGM_TOGGLE_MESSENGER();
	int OnGM_TOGGLE_MAIL();

	int OnGM_CLOSE_CHATINFO();

	int OnGM_ITEMUP_READY();
	int OnGM_ITEMUP_CANCEL();
	int OnGM_ITEMUP_START();

	int OnGM_ITEMUP_CHECK_USE_CASHITEM();
	int OnGM_REFINE_CHECK_USE_CASHITEM( DWORD dwCost, BYTE bDiscountRate );
	int OnGM_WARN_NOTUSE_CASHITEM_CASH();
	int OnGM_WARN_NOTUSE_CASHITEM_OK();
	int OnGM_WARN_NOTUSE_CASHITEM_CANCEL();

	int OnGM_GUILD_NORMAL();
	int OnGM_GUILD_MEMBER();
	int OnGM_GUILD_TERRITORY();
	int OnGM_GUILD_MERCENARY();
	int OnGM_GUILD_REQ();
	int OnGM_TACTICS_REQ();
	int OnGM_SHOW_INVITE_TACTICS();
	int OnGM_GUILD_EXIT_WARN();
	int OnGM_GUILD_EXIT();
	int OnGM_ESTABLISH_GUILD();
	int OnGM_NPC_GUILD_APP();
	int OnGM_NPC_GUILD_NEW();
	int OnGM_NPC_GUILD_MARK();
	int OnGM_NPC_GUILD_DEL();
	int OnGM_NPC_GUILD_DEL_CANCEL();
	int OnGM_GUILDINVITE_ACK_YES();
	int OnGM_GUILDINVITE_ACK_NO();
	int OnGM_GUILDTACTICSINVITE_ACK_YES();
	int OnGM_GUILDTACTICSINVITE_ACK_NO();
	int OnGM_GUILDTACTICSINVITE_ACK_BUSY();
	int OnGM_GUILD_DONATE_EXP();
	int OnGM_GUILD_DONATE_MONEY();
	int OnGM_WRAN_DONATE_EXP();
	int OnGM_DO_DONATE_EXP();
	int OnGM_WRAN_DONATE_MONEY();
	int OnGM_DO_DONATE_MONEY();
	int OnGM_GUILD_NOTIFY();
	int OnGM_GUILD_COMMAND();
	int OnGM_REG_GUILDMARK();
	int OnGM_GUILDKICKOUT();
	int OnGM_GUILDTACTICSKICKOUT();
	int OnGM_GUILDCAB_GETITEM();
	int OnGM_TACINVOFFER_ACK_YES();
	int OnGM_TACINVOFFER_ACK_NO();
	int OnGM_TACINVSEEK_ACK_YES();
	int OnGM_TACINVSEEK_ACK_NO();

	int OnGM_NEW_NOTIFY();
	int OnGM_MOD_NOTIFY();
	int OnGM_NEW_NOTIFY_OK();
	int OnGM_NEW_NOTIFY_CANCEL();
	int OnGM_MOD_NOTIFY_OK();
	int OnGM_MOD_NOTIFY_CANCEL();
	int OnGM_DELETE_NOTIFY();

	int OnGM_OPEN_PRIVSHOP_FOR_BUY();
	int OnGM_OPEN_PRIVSHOP_FOR_SELL();
	int OnGM_PRIVATE_SHOP_START();
	int OnGM_PRIVATE_SHOP_END();
	int OnGM_PRIVATE_SHOP_BUY();
	int OnGM_PRIVATE_SHOP_BUY_ITEMS();
	int OnGM_REG_ITEM_PRVSHOP();

	int OnGM_GINV_APP();
	int OnGM_GINV_EDIT();
	int OnGM_GINV_NEW();
	int OnGM_GINV_DEL();
	int OnGM_GINV_MOD_OK();
	int OnGM_GINV_NEW_OK();
	int OnGM_GINV_MOD_CANCEL();
	int OnGM_GINV_NEW_CANCEL();

	int OnGM_GSEEK_INV();
	int OnGM_GSEEK_EDIT();
	int OnGM_GSEEK_NEW();
	int OnGM_GSEEK_DEL();
	int OnGM_GSEEK_MOD_OK();
	int OnGM_GSEEK_NEW_OK();
	int OnGM_GSEEK_MOD_CANCEL();
	int OnGM_GSEEK_NEW_CANCEL();

	int OnGM_FORCE_NEW_GUILDOFFER();
	int OnGM_FORCE_NEW_GUILDSEEK();

	int OnGM_GMP_WHISPER();	
	int OnGM_GMP_MAIL();	
	int OnGM_GMP_PARTY();
	int OnGM_GMP_FRIEND();
	int OnGM_GMP_KICK();		
	int OnGM_GMP_DISORG_TACTICS();
	int OnGM_GMP_ET_GM();		
	int OnGM_GMP_SET_GSM();	
	int OnGM_GMP_CANCEL_GSM();	
	int OnGM_GMP_PEER_BARON();	
	int OnGM_GMP_PEER_VISCOUNT();
	int OnGM_GMP_PEER_COUNT();	
	int OnGM_GMP_PEER_MARQUIS();
	int OnGM_GMP_PEER_DUKE();
	int OnGM_GMP_CANCEL_PEER();	
	int OnGM_GMP_POINT_REWARD();
	int OnGM_GMP_CANCEL();
	int OnGM_GUILD_REWARD_STOP();
	int OnGM_GUILD_REWARD_CONTINUE();
	int OnGM_GMP_VOLUNTEER_HESED();
	int OnGM_GMP_VOLUNTEER_ARDRI();
	int OnGM_GMP_VOLUNTEER_TYCON();
	int OnGM_GMP_VOLUNTEER_GEHBRA();
	int OnGM_GMP_VOLUNTEER_CANCEL();

	int OnGM_GVP_WHISPER();
	int OnGM_GVP_MAIL();
	int OnGM_GVP_INVITE_YES();
	int OnGM_GVP_INVITE_NO();
	int OnGM_GVP_CANCEL();

	int OnGM_FRIEND_REQ();
	int OnGM_FARFRIEND_REQ();
	int OnGM_ADD_FRIEND();
	int OnGM_FRIENDINVITE_ACK_YES();
	int OnGM_FRIENDINVITE_ACK_NO();
	int OnGM_ADDTOBLOCKLIST();

	int OnGM_NEW_FRIEND_GROUP();		
	int OnGM_CHANGE_FRIEND_GROUP();
	int OnGM_SHOW_TARGETMAIL();	
	int OnGM_SHOW_GROUPLIST_POPUP();	
	int OnGM_ADDTOBLOCKLIST_TARGET();
	int OnGM_DELETE_FRIENDITEM();
	int OnGM_ASK_NEW_FRIEND_GROUP();
	int OnGM_NEW_MESSENGER_GROUPCHAT();
	int OnGM_ASK_CHANGE_FRIEND_GROUP();
	int OnGM_MOVE_FRIEND_GROUP();

	int OnGM_SEND_MAIL();
	int OnGM_SHOW_MAILREPLY();
	int OnGM_DELETE_MAIL();
	int OnGM_MESSENGER_DELETE_MAIL();
	int OnGM_MAILBILL_PAY();
	int OnGM_MAILBILL_RETURN();
	int OnGM_MAIL_GETITEM();
	int OnGM_SHOW_RECVMAIL();
	int OnGM_SHOW_NEWMAIL();
	int OnGM_NEW_MESSENGER_CHAT();
	int OnGM_TOGGLE_BLOCKLIST();
	int OnGM_MESSENGERCHAT_CLOSE();

	int OnGM_PET_NEWMAKE();
	int OnGM_PET_NAMEBOX();
	int OnGM_PET_RECALL();
	int OnGM_MENU_RIDEPET();
	int OnGM_MENU_PETMANAGE();

	int OnGM_SELECT_TEXTURE_DETAIL();
	int OnGM_CHANGE_CHANNEL();
	int OnGM_CLOSE_CANCELQUIT();

	int OnGM_CABINET_SELECT();
	int OnGM_CABINET_OPEN_WARN();
	int OnGM_CABINET_OPEN();

	int OnGM_FOLLOW_TARGET();

	int OnGM_KEYSET_DEFAULT();
	int OnGM_KEYSET_APPLY();
	int OnGM_KEYSET_CANCEL();

	int OnGM_MENU_DUEL();
	int OnGM_DUELINVITE_YES();
	int OnGM_DUELINVITE_NO();

	int OnGM_SMS_SEND();
	int OnGM_SMS_SEND_OK();

	int OnGM_WHISPER_REPLY();

	int OnGM_SKILLINIT_ALL();
	int OnGM_SKILLINIT_ONE();

	int OnGM_TOGGLE_HELMETHIDE();

	int OnGM_TOGGLE_SUBCASHMENU();
	int OnGM_TOGGLE_CASH_SHOP();
	int OnGM_TOGGLE_CASH_CAB();
	int OnGM_TOGGLE_CASH_CHARGE();

	int OnGM_SHOW_ACCOUNT();

	int OnGM_CLOSE_CASHCHARGE();
	int OnGM_CASHCAB_GETITEM();
	int OnGM_CASHCAB_EXTENSIONPERIOD();
	int OnGM_CASHCAB_BUYCABINET_YES();
	int OnGM_WARN_BUY_CASHITEM();
	int OnGM_BUY_CASHITEM();
	int OnGM_WARN_CASH_PRESENT();
	int OnGM_CHECK_CASH_PRESENT();
	int OnGM_CASH_PRESENT();
	int OnGM_WARN_EXTEND_PERIOD();
	int OnGM_EXTEND_PERIOD();
	int OnGM_CASHCABINET_PUTIN();
	int OnGM_END_MAINTAIN();

	int OnGM_SOULMATE_REGBOX();
	int OnGM_SOULMATE_SEARCH();
	int OnGM_SOULMATE_REGREADY();
	int OnGM_SOULMATE_REGOK();
	int OnGM_SOULMATE_SEARCHOK();
	int OnGM_SOULMATE_NO();
	int OnGM_SOULMATE_END();
	int OnGM_SOULMATE_SUCCESS();
	int OnGM_SOULMATE_FAIL();
	int OnGM_SOULMATE_NOTFOUND();

	int OnGM_GAMBLE_CHECK();
	int OnGM_GAMBLE_PROGRESS();
	int OnGM_GAMBLE_OK();
	int OnGM_GAMBLE_NO();

	int OnGM_NPC_DURATIONREP_NOR();		
	int OnGM_NPC_DURATIONREP_ALL();	
	int OnGM_NPC_DURATIONREP_EQUIP();
	int OnGM_DURATIONREP_NOR();
	int OnGM_DURATIONREP_PROGRESS();
	int OnGM_DURATIONREP_REQ();	
	int OnGM_NPC_REFINE();		
	int OnGM_REFINE_COST();	
	int OnGM_REFINE_PROGRESS();
	int OnGM_REFINE();
	int OnGM_CHANGE_NAME();
	int OnGM_EXTEND_INVEN_PERIOD();
	int OnGM_EXTEND_ITEM_PERIOD();
	int OnGM_STOPTHECLOCK();
	int OnGM_NPC_EXTEND_ITEM_PERIOD();
	int OnGM_CLOSE_MSGBOX_RESETINVEN();
	int OnGM_INIT_UI();
	int OnGM_TOGGLE_RESOLUTION();

	int OnGM_HELP_NEXT();
	int OnGM_HELP_PREV();

	int OnGM_SET_WHISPER_TARGET();
	int OnGM_TOGGLE_WHISPER_TARGET();
	int OnGM_TOGGLE_CHAT_TYPE();

	int OnGM_COPY_NAME();
	int OnGM_PASTE_NAME();

	int OnGM_DAY_GUILDPVPRECORD();
	int OnGM_WEEKEN_GUILDPVPRECORD();

	int OnGM_GUILDREWARD();
	int OnGM_GUILD_PVPRECORD();
	int OnGM_POINTREWARD();

	int OnGM_PVP_INFO();
	int OnGM_WAR_INFO();

	int OnGM_GUILD_LIST_TAB1();
	int OnGM_GUILD_LIST_TAB2();
	int OnGM_GUILD_LIST_TAB3();
	int OnGM_GUILD_LIST_TAB4();

	int OnGM_PICKUP_BALL();
	int OnGM_MOUNT_BALL2_TOWER();

	int OnGM_ITEM_UP_PROGRESS_CANCEL();

	int OnGM_CHANGE_REFINE();
	int OnGM_GUILD_OFFER_BBS_WRITE();
	int OnGM_GUILD_OFFER_BBS_MODIFY();
	int OnGM_GUILD_OFFER_BBS_DELETE();
	int OnGM_GUILD_OFFER_BBS_DO_DELETE();

	int OnGM_GUILD_MERCENARY_TAB1();
	int OnGM_GUILD_MERCENARY_TAB2();

	int OnGM_GUILD_OFFER_BBS_GUILDMEMBER();
	int OnGM_GUILD_OFFER_BBS_TACTICS();

	int OnGM_GVFPM_MAIL();
	int OnGM_GVFPM_REGULAR_ACCEPT();
	int OnGM_GVFPM_REGULAR_REJECT();
	int OnGM_GVFPM_TACTICS_ACCEPT();
	int OnGM_GVFPM_TACTICS_REJECT();
	int OnGM_GVFPM_CLOSE();

	int OnGM_GUILD_INVITATION_REGULAR_OK();
	int OnGM_GUILD_INVITATION_TACTICS_OK();

	int OnGM_GMP_TACTICS_RE_CONTRACT();
	int OnGM_GMP_TACTICS_KICK();
	int OnGM_GMP_TACTICS_REWARD_POINT();
	int OnGM_GUILD_OFFER_BBS_JOIN();
	int OnGM_GUILD_OFFER_BBS_JOINCANCEL();
	int OnGM_GUILD_OFFER_BBS_JOIN_DO();
	int OnGM_GUILD_OFFER_BBS_JOINCANCEL_DO();
	int OnGM_GUILD_OFFER_BBS_REFRESH();
	int OnGM_GUILD_SHOW_MARKDLG();

#ifdef ADD_TEXTOOL
	int OnGM_TEXTOOL_BACK();
	int OnGM_TEXTOOL_BACK2();
	int OnGM_TEXTOOL_CENTERBOTTOM();
	int OnGM_TEXTOOL_CENTERCENTER();
	int OnGM_TEXTOOL_CENTERTOP();
	int OnGM_TEXTOOL_EXT1();
	int OnGM_TEXTOOL_DOWN();
	int OnGM_TEXTOOL_EXT2();
	int OnGM_TEXTOOL_LEFT();
	int OnGM_TEXTOOL_LEFTBOTTOM();
	int OnGM_TEXTOOL_LEFTCENTER();
	int OnGM_TEXTOOL_LEFTTOP();
	int OnGM_TEXTOOL_MARK();
	int OnGM_TEXTOOL_EXT10();
	int OnGM_TEXTOOL_EXT3();
	int OnGM_TEXTOOL_RIGHT();
	int OnGM_TEXTOOL_RIGHTBOTTOM();
	int OnGM_TEXTOOL_RIGHTCENTER();
	int OnGM_TEXTOOL_RIGHTTOP();
	int OnGM_TEXTOOL_EXT9();
	int OnGM_TEXTOOL_UP();
	int OnGM_TEXTOOL_EXT4();
//	int OnGM_TEXTOOL_SCALEDOWN();
//	int OnGM_TEXTOOL_SCALEUP();
//	int OnGM_TEXTOOL_COMPO();
	int OnGM_TEXTOOL_EXT5();
	int OnGM_TEXTOOL_EXT6();
	int OnGM_TEXTOOL_EXT8();
	int OnGM_TEXTOOL_EXT7();
#endif

	int OnGM_CORPS_BUSY();
	int OnGM_PARTY_BUSY();
	int OnGM_TRADE_BUSY();
	int OnGM_DUELINVITE_BUSY();
	int OnGM_GUILDINVITE_ACK_BUSY();

	int OnGM_AUCTION_SEARCH_TAB();
	int OnGM_AUCTION_REGIST_TAB();
	int OnGM_AUCTION_BIDDING_TAB();
	int OnGM_AUCTION_BASKET_TAB();

	int OnGM_AUCTION_MOVEPAGE_RIGHT();
	int OnGM_AUCTION_MOVEPAGE_LEFT();
	int OnGM_AUCTION_MOVEPAGE_RIGHTEND();
	int OnGM_AUCTION_MOVEPAGE_LEFTEND();

	int OnGM_AUCTION_BID();
	int OnGM_AUCTION_BUY();
	int OnGM_AUCTION_DO_BUY();
	int OnGM_AUCTION_CANCEL_REG();
	int OnGM_AUCTION_DO_CANCEL_REG();
	int OnGM_AUCTION_INSERT_BASKET();
	int OnGM_AUCTION_ERASE_BASKET();

	int OnGM_AUCTION_MOVE_BASKET_TAB();
	int OnGM_AUCTION_MAKEABIDFORITEM();
	int OnGM_AUCTION_REGIST_MANYITEMS();

	int OnGM_AUCTION_SEARCH();
	int OnGM_AUCTION_SEARCH_RESET();
	int OnGM_AUCTION_REGIST();
	int OnGM_AUCTION_DO_REGIST();
	int OnGM_AUCTION_REGIST_RESET();

	int OnGM_AUCTION_ALIGN_NAME();
    int OnGM_AUCTION_ALIGN_CLASS();
	int OnGM_AUCTION_ALIGN_LEVEL();
	int OnGM_AUCTION_ALIGN_PRICE();
	int OnGM_AUCTION_ALIGN_SELLER();
	int OnGM_AUCTION_ALIGN_LEFTTIME();
	int OnGM_AUCTION_ALIGN_BIDCOUNT();

	int OnGM_CLOSE_MSGBOX_WITH_RESETCONMODE();
	int OnGM_NPC_COLOR_MANTLE();
	int OnGM_NPC_MAKE_GUILDMARK();
	int OnGM_NPC_UNREG_GUILDMARK();
	int OnGM_NPC_REG_GUILDMARK();
	int OnGM_ITEMCOLOR_READY();
	int OnGM_ITEMCOLOR_SELECT();
	int OnGM_ITEMCOLOR_DO();
	int OnGM_REGGM_CHECK();
	int OnGM_UNREGGM_CHECK();
	int OnGM_APPLY_GUILDMARK();
	int OnGM_GUILDMARK_CANCEL();

	int OnGM_FAMERANK_MONTH_LEFT();
	int OnGM_FAMERANK_MONTH_RIGHT();
	int OnGM_TOGGLE_FAMERANK();
	int OnGM_HERO_FAME();
	int OnGM_KNIGHT_FAME();
	int OnGM_GOD_FAME();
	int OnGM_MONTH_FAME();
	int OnGM_FIRSTGRADEGROUP_FAME();
	int OnGM_WARLORD_SAY_CHANGE();
	int OnGM_HERO_SAY_CHANGE();
	int OnGM_KNIGHT_SAY_CHANGE();
	int OnGM_GOD_SAY_CHANGE();
	int OnGM_RANK_INFO();
	int OnGM_RANK_INFO_TOTAL_TAB();
	int OnGM_RANK_INFO_COUNTRY_TAB();
	int OnGM_RANK_INFO_TOURNAMENT_TAB();

	int OnGM_CAUTION15_CLICKED();

	int OnGM_CLOSE_RECV_MAIL();
	int OnGM_TOURNAMENT_ADD_PARTY();
	int OnGM_TOURNAMENT_ENTRY();
	int OnGM_TOURNAMENT_EVENT_SELECT_PLAYER();
	int OnGM_TOURNAMENT_GIVEUP();
	int OnGM_TOURNAMENT_GOTO_MATCH();
	int OnGM_TOURNAMENT_REMOVE_PARTY();
	int OnGM_TOURNAMENT_SELECT_TARGET();
	int OnGM_TOURNAMENT_SELECT_WINNER();
	int OnGM_TOURNAMENT_SHOW_MATCH();
	int OnGM_TOURNAMENT_SHOW_EVENT();
	int OnGM_TOURNAMENT_ENTRY_DO();
	int OnGM_TOURNAMENT_GIVEUP_DO();
	int OnGM_TOURNAMENT_EVENT_SELECT_PLAYER_DO();
	int OnGM_TOURNAMENT_EVENT_PREV();
	int OnGM_TOURNAMENT_REMOVE_PARTY_DO();

	int OnGM_TOURNAMENT_OK();
	int OnGM_TOURNAMENT_CANCEL();
	int OnGM_TOURNAMENT_SCHEDULE_LEFT();
	int OnGM_TOURNAMENT_SCHEDULE_RIGHT();
	int OnGM_TOURNAMENT_TOGGLE_SHOW_PLAYER_INFO();
	int OnGM_TOURNAMENT_SELECT_TARGET_GUILD();
	int OnGM_TOURNAMENT_SELECT_TARGET_FRIEND();
	int OnGM_TOURNAMENT_USE_CHEERITEM();

	int OnGM_FIRSTRANK_MONTH_LEFT();
	int OnGM_FIRSTRANK_MONTH_RIGHT();
	int OnGM_CLICKED_SIEGE_ALARM();
	int OnGM_CLICKED_SIEGE_ALARM_DO();
	int OnGM_CLICKED_STOP_TUTORIAL();
	int OnGM_SHOW_TOURNAMENT();
	int OnGM_SHOW_TOURNAMENT_EVENT();
	
	int OnGM_HELP_LEFT();
	int OnGM_HELP_RIGHT();
	int OnGM_HELP_UP();
	int OnGM_HELP_DOWN();
	int OnGM_SKIP_SCENE();
#ifdef ADD_RPS
	int OnGM_RPS_GIVEUP();
	int OnGM_RPS_SELECT_1();
	int OnGM_RPS_SELECT_2();
	int OnGM_RPS_SELECT_3();
	int OnGM_RPS_TRY();
	int OnGM_RPS_START();
	int OnGM_RPS_END();
	int OnGM_RPS_ASK_COST();
	int OnGM_RPS_1();
	int OnGM_RPS_10();
	int OnGM_RPS_50();
	int OnGM_RPS_SHOW_REWARD();
#endif
	// Msg Handler

	// Relay message
	int OnCS_RELAYCONNECT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_RELAYCHARDATA_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// System message
	int OnCS_INVALIDCHAR_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ADDCONNECT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SHUTDOWN_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CONNECT_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// PC information message
	int OnCS_CHARSTATINFO_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CHARINFO_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// AI message
	int OnCS_MONSETSKILL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MONACTION_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MONHOST_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Navigation message
	int OnCS_BEGINTELEPORT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SETRETURNPOS_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_TELEPORT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ENTER_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_LEAVE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_BLOCK_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MOVE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_JUMP_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_WARP_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_MONMOVE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ADDMON_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELMON_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Switch/Gate message
	int OnCS_SWITCHCHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SWITCHADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SWITCHDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_GATECHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GATEADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GATEDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Battle message
	int OnCS_REVIVALREPLY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CHANGECOLOR_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_REVIVALASK_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DROPDAMAGE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MONATTACK_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_LOOPSKILL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SKILLUSE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SKILLEND_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CHGMODE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_REVIVAL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ACTION_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DEFEND_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_LEVEL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_HPMP_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DIE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_EXP_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_RESETCOOLTIME_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Item message
	int OnCS_UPDATEITEM_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MOVEITEM_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ADDITEM_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELITEM_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ITEMUSE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MONEY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_EQUIP_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ITEMUPGRADE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ITEMMAGICGRADE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GETITEM_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_HELMETHIDE_ACK(CTachyonSession *pSession, CPacket *pPacket);

	// Inven message
	int OnCS_INVENMOVE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_INVENADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_INVENDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Corps message
	int OnCS_CHGCORPSCOMMANDER_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CORPSREPLY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CORPSJOIN_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CORPSASK_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CORPSCMD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CORPSHP_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_REPORTENEMYLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CORPSENEMYLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MOVECORPSENEMY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MOVECORPSUNIT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ADDCORPSENEMY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELCORPSENEMY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CHGSQUADCHIEF_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ADDCORPSUNIT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELCORPSUNIT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ADDSQUAD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELSQUAD_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Party message
	int OnCS_CHGPARTYCHIEF_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PARTYMANSTAT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PARTYJOINASK_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PARTYJOIN_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PARTYATTR_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PARTYMOVE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PARTYADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PARTYDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CHGPARTYTYPE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PARTYITEMTAKE_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Chat message
	int OnCS_CHAT_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Trade message
	int OnCS_NPCITEMLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SKILLBUY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ITEMSELL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ITEMBUY_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_DEALITEMSTART_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DEALITEMASK_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DEALITEMADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DEALITEMEND_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Rooting message
	int OnCS_MONITEMLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MONITEMTAKE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MONITEMLOTTERY_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// UI setting message
	int OnCS_HOTKEYCHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_HELPMESSAGE_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Summon message
	int OnCS_ADDRECALLMON_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELRECALLMON_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ADDSELFOBJ_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELSELFOBJ_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_OTHERSELF_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Quest message
	int OnCS_QUESTLIST_COMPLETE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_QUESTLIST_POSSIBLE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_QUESTSTARTTIMER_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_QUESTCOMPLETE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_QUESTUPDATE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CHAPTERMSG_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_QUESTLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_QUESTADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_NPCTALK_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Private shop message
	int OnCS_STOREOPEN_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_STORECLOSE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_STOREITEMLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_STOREITEMBUY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_STOREITEMSELL_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Messenger message
	int OnCS_FRIENDASK_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_FRIENDADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_FRIENDERASE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_FRIENDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_FRIENDCONNECTION_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_FRIENDREGION_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_FRIENDGROUPMAKE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_FRIENDGROUPDELETE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_FRIENDGROUPCHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_FRIENDGROUPNAME_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_TMSINVITE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_TMSRECV_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_TMSOUT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PROTECTEDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PROTECTEDADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PROTECTEDERASE_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Post message
	int OnCS_POSTSEND_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_POSTRECV_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_POSTLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_POSTINFO_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_POSTVIEW_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_POSTDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_POSTGETITEM_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_POSTRETURN_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Pet message
	int OnCS_PETMAKE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PETDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PETLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PETRECALL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PETRIDING_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Channel message
	int OnCS_CHGCHANNEL_ACK(CTachyonSession *pSession, CPacket *pPacket);

	// Cabinet message
	int OnCS_CABINETLIST_ACK(CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CABINETITEMLIST_ACK(CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CABINETOPEN_ACK(CTachyonSession *pSession, CPacket *pPacket);

	// Local messgage
	int OnCS_LOCALOCCUPY_ACK(CTachyonSession *pSession, CPacket *pPacket);

	// Duel message
	int OnCS_DUELINVITE_ACK(CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DUELSTANDBY_ACK(CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DUELSTART_ACK(CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DUELEND_ACK(CTachyonSession *pSession, CPacket *pPacket);

	// Get target message
	int OnCS_GETTARGET_ACK(CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GETTARGETANS_ACK(CTachyonSession *pSession, CPacket *pPacket);

	// SMS message
	int OnCS_SMSSEND_ACK(CTachyonSession *pSession, CPacket *pPacket);

	// Skill message
	int OnCS_SKILLINIT_ACK(CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SKILLLIST_ACK(CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SKILLINITPOSSIBLE_ACK(CTachyonSession *pSession, CPacket *pPacket);

	// Guild message
	int OnCS_GUILDESTABLISH_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDDISORGANIZATION_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDINVITE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDJOIN_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDLEAVE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDDUTY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDMEMBERLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDATTR_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDPEER_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDINFO_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDLOCALLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDLOCALRETURN_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDCABINETLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDCABINETPUTIN_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDCABINETTAKEOUT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDCONTRIBUTION_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDARTICLELIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDARTICLEADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDARTICLEDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDFAME_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDWANTEDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDWANTEDADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDWANTEDDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDVOLUNTEERLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDVOLUNTEERREPLY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDVOLUNTEERING_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDVOLUNTEERINGDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSINVITE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSANSWER_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDARTICLEUPDATE_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Cash message
	int OnCS_PARTYMEMBERRECALL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PARTYMEMBERRECALLANS_ACK( CTachyonSession *pSession, CPacket *pPacket); 
	int OnCS_CASHITEMCABINET_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CASHITEMGET_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CASHSHOPITEMLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CASHITEMBUY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CASHITEMPRESENT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_RESETPCBANG_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CASHCABINETBUY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CASHITEMPUTIN_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CASHCABINETBUYCASH_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Soulmate message
	int OnCS_SOULMATE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SOULMATEREGREADY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SOULMATESEARCH_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SOULMATEREG_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_SOULMATEEND_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Gamble message
	int OnCS_GAMBLECHECK_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GAMBLEOPEN_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Siege message
	int OnCS_ADDGODTOWER_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELGODTOWER_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ADDGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DELGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_TAKEGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_REMOVEGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DROPGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MOUNTGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DEMOUNTGODBALL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_BALANCEOFPOWER_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_CASTLEAPPLY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ENDWAR_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ENTERCASTLE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_LEAVECASTLE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_NPCMONSTERLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_MONSTERBUY_ACK( CTachyonSession *pSession, CPacket *pPacket);


	// Duration message
	int OnCS_DURATIONDEC_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DURATIONEND_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DURATIONREPCOST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_DURATIONREP_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_REFINECOST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_REFINE_ACK( CTachyonSession *pSession, CPacket *pPacket);

	// Hero Registration
	int OnCS_HEROSELECT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_HEROLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_CHANGECHARBASE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_STOPTHECLOCK_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_SYSTEMMSG_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_OPENMONEY_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_COMMENT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_ITEMCHANGE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_COUNTDOWN_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_PVPPOINT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDPOINTLOG_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDPOINTREWARD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDPVPRECORD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_PVPRECORD_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_CHANGEITEMATTR_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_WASTEREFINE_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSWANTEDADD_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSWANTEDDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSWANTEDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSVOLUNTEERING_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSVOLUNTEERINGDEL_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSREPLY_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSKICKOUT_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_GUILDTACTICSVOLUNTEERLIST_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_REGGUILDCLOAK_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_RESETCLOAK_ACK( CTachyonSession *pSession, CPacket *pPacket);

	int OnCS_CASHSHOPSTOP_ACK( CTachyonSession *pSession, CPacket *pPacket);
	int OnCS_APEXDATA_ACK( CTachyonSession *pSession, CPacket *pPacket);
#ifndef TEST_MODE
	int OnCS_HACKSHIELD_ACK( CTachyonSession *pSession, CPacket * pPacket);
#endif
	int UpdateAuctionList( CPacket* pPacket );

	int OnCS_AUCTIONFIND_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_AUCTIONREGLIST_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_AUCTIONBIDLIST_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_AUCTIONINTERESTLIST_ACK( CTachyonSession *pSession, CPacket *pPacket );

	int OnCS_AUCTIONBID_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_AUCTIONBUYDIRECT_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_AUCTIONINTEREST_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_AUCTIONREG_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_AUCTIONREGCANCEL_ACK( CTachyonSession *pSession, CPacket *pPacket );

	int OnCS_KICKOUTMAP_ACK( CTachyonSession *pSession, CPacket *pPacket );

	int OnCS_FAMERANKLIST_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_MONTHRANKLIST_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_UPDATEFAMERANKLIST_ACK( CTachyonSession *pSession, CPacket *pPacket );

	int OnCS_AFTERMATH_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_FIRSTGRADEGROUP_ACK( CTachyonSession *pSession, CPacket *pPacket );

	// Chat Commander
	int OnCS_CHATBAN_ACK( CTachyonSession *pSession, CPacket *pPacket );

	// nProtect. 게임가드.
	int OnCS_NPROTECT_ACK( CTachyonSession *pSession, CPacket *pPacket );

#ifdef ADD_TOURNAMENT
	int OnCS_TOURNAMENTAPPLYINFO_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTAPPLY_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTJOINLIST_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTPARTYLIST_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTPARTYADD_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTMATCHLIST_ACK	( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTEVENTLIST_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTEVENTINFO_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTEVENTJOIN_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTCHEER_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTENTER_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTPARTYDEL_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTRESULT_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTAPPLYCANCEL_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTSCHEDULE_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_TOURNAMENTSELECTWINNER_ACK( CTachyonSession *pSession, CPacket *pPacket );
#endif
	int OnCS_ITEMLEVELREVISION_ACK( CTachyonSession *pSession, CPacket *pPacket );
	
#ifdef ADD_RPS
	int OnCS_RPSSTART_ACK( CTachyonSession *pSession, CPacket *pPacket );
	int OnCS_RPSGAME_ACK( CTachyonSession *pSession, CPacket *pPacket );
#endif

	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void OnImeNotify(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInputLangChangeRequest(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void OnSetFocus( TComponent* pLoseFocus );
	virtual void OnLoseFocus( TComponent* pSetFocus );
	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags);
	virtual BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual void OnMButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRClick(UINT nFlags, CPoint pt);
	virtual void OnClick(UINT nFlags, CPoint pt);
	virtual void SwitchFocus(TComponent *pCandidate);

	virtual HRESULT Render( DWORD dwTickCount);

public:
	CD3DDevice*			GetDevice() const				{ return m_pDevice; }
	CTClientCAM*		GetMainCam() const				{ return m_pCamera; }
	CTClientWnd*		GetMainWnd() const				{ return m_pMainWnd; }
	CTClientChar*		GetMainChar() const				{ return m_pMainChar; }
	CTClientMoveObj*	GetCtrlHost() const				{ return m_pCtrlHost; }
	CTClientMAP*		GetClientMap() const			{ return const_cast<CTClientMAP*>(&m_vMAP); }
	CTClientSession*	GetSession() const				{ return m_pSESSION; }
	CTChatFrame*		GetChatFrame() const			{ return m_pChatFrame; }
	CTDynamicHelpDlg*	GetNotifyFrame() const			{ return m_pNotifyFrame; }
	CTMiniPopupDlg*	GetActListPopup() const			{ return m_pActListPopup; }
	CTMiniPopupDlg*	GetResolutionListPopup() const { return m_pSystemSetting ? m_pSystemSetting->m_pResolutionListPopup : NULL; }
	CTClientUIBase*		GetFrame(BYTE bFID) const		{ return m_vTFRAME[bFID]; }
	CTMessengerBase*	GetMessengerBase() const		{ return m_pMessengerBase; }
	CTMailListFrame*	GetMailList() const				{ return m_pMailList; }
	CTachyonRes*		GetResource() const				{ return m_pRES; }
	int					GetPartyCnt() const				{ return (int)m_vTPARTY.size(); }
	LPTPARTY			GetParty(int i) const			{ return const_cast<LPTPARTY>(m_vTPARTY[i]); }
	BYTE				GetPartyItemType() const		{ return m_bPartyItemType; }
	CTClientObjBase*	GetTargetObj() const			{ return m_pTARGET; }
	CTClientChar*		GetDuelTarget() const			{ return m_pDuelTarget; }
	BOOL				IsDuel() const					{ return m_bDuel; }
	BOOL				IsTournamentBattle() const { return m_bTournamentBattle; }
	BOOL				IsTournamentWatching() const { return m_bTournamentWatching; }
	BOOL				IsInvenDraging() const			{ return m_bInvenDraging; }
	const CString&		GetSoulName() const				{ return m_strSoulName; }
	DWORD				GetScreenX() const				{ return m_pDevice->m_option.m_dwScreenX; }
	DWORD				GetScreenY() const				{ return m_pDevice->m_option.m_dwScreenY; }
	int					GetTotalDrawCount() const		{ return m_nTotalDrawCount; }
	int					GetDrawCount(BYTE bType) const	{ return m_vDrawCount[bType]; }
	BOOL				IsDrawOBJ() const				{ return m_bDrawOBJ; }
	BOOL				IsShowDebugInfo() const			{ return m_bShowDebugInfo; }

	void				SetDrawOBJ(BOOL p)				{ m_bDrawOBJ = p; }
	void				SetShowDebugInfo(BOOL p)		{ m_bShowDebugInfo = p; }

	BOOL				IsMoveOBJ( BYTE bType);
	BOOL IsShowMinimap();
	

	// Utility
	
#ifdef TEST_MODE
public:
	void CopyToClipboard( CString str );
	void SetMonRoaming( BOOL bMonRoaming);
	void ReleaseTSPAWN();
	void BeginEditTSPAWN();
	void EndEditTSPAWN();
	void SaveEditTSPAWN();
	void AddTSPAWN(
		CTClientObjBase *pTARGET,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wDIR);

public:
	MAPTSTRING m_mapTSPAWN;
	BOOL m_bEditTSPAWN; // 디폴트:FALSE
	BOOL m_bMonRoaming;	// 디폴트:TRUE
#endif
};