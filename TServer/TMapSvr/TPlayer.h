#pragma once


class CTPlayer : public CTMapSession, public CTObjBase
{
public:
	MAPTHOTKEYINVEN m_mapHotkeyInven;
	MAPTCABINET m_mapCabinet;
	MAPTSTOREITEM m_mapStoreItem;
	MAPTRECALLMON m_mapRecallMon;
	MAPTSELFOBJ m_mapSelfMon;
	MAPTPET m_mapTPET;
	TDURINGITEM m_diPremium;
	TDURINGITEM m_diExp;

	MAPTPROTECTED m_mapTPROTECTED;
	LPTPOST m_pPost;

	MAPDWORD m_mapItemCoolTime;
	MAPDWORD m_mapLevelQuest;
	MAPQUEST m_mapQUEST;

	TDEALITEM m_dealItem;
	VTRECORDSET m_vPvPRecent;
	DWORD m_aPvPRecord[TCLASS_COUNT][2];

	VTRECORDSET m_vDuelRecordSet;
	DWORD m_aDuelScore[TCLASS_COUNT][2];
	
	VDWORD	m_vAuctionInterest;
	VDWORD	m_vAuctionBid;
	VDWORD	m_vAuctionReg;

	// 길드
	CTItem * m_guildItem;

	CPacket m_mainchar;

public:

#ifdef __HACK_SHIELD
	AHNHS_CLIENT_HANDLE m_hHackShield;
	DWORD m_dwHSSendTick;
	DWORD m_dwHSRecvTick;
#endif

#ifdef __N_PROTECT
	CCSAuth2* m_csNProtect;
	DWORD m_dwNPSendTime;
	BYTE m_bSend;
	BYTE m_bFirst;

	void SendCS_NPROTECT_ACK(BYTE bServerID, DWORD dwTick);
#endif

	CString m_strStoreName;
	CString m_strNAME;
	CString m_strComment;

	DWORD m_dwUserID;
	DWORD m_dwKEY;

	BYTE m_bStartAct;
	BYTE m_bRealSex;
	BYTE m_bCanHost;
	BYTE m_bIsGhost;
	BYTE m_bClass;
	BYTE m_bPants;
	BYTE m_bRace;
	BYTE m_bHair;
	BYTE m_bFace;
	BYTE m_bBody;
	BYTE m_bHand;
	BYTE m_bFoot;
	BYTE m_bSex;

	DWORD m_dwMoveTick;
	DWORD m_dwCooper;
	DWORD m_dwSilver;
	DWORD m_dwGold;
	DWORD m_dwEXP;
	DWORD m_dwPvPUseablePoint;
	DWORD m_dwPvPTotalPoint;
	DWORD m_dwPvPRankOrder;
	BYTE m_bPvPRankPercent;
	 
	DWORD m_dwMonthPvPoint;
	DWORD m_dwMonthRankOrder;
	BYTE  m_bMonthRankPercent;
	WORD  m_wMonthWin;
	WORD  m_wMonthLose;
	DWORD  m_dwTotalWin;
	DWORD  m_dwTotalLose;
	CString m_strMonthSay;

	FLOAT m_fGhostX;
	FLOAT m_fGhostZ;
	FLOAT m_fVital;

	DWORD m_dwPartyChiefID;
	DWORD m_dwLockedMonID;
	DWORD m_dwRiding;

	BYTE m_bHelmetHide;

	WORD m_wCommanderID;
	WORD m_wSkillPoint;
	WORD m_wSpawnID;

	DWORD m_dwGuildLeaveTime;
	DWORD m_dwSaveTick;

	BYTE m_bGuildLeave;
	BYTE m_bStore;

	DWORD m_dwDuelID;
	DWORD m_bDuelType;
	DWORD m_dwDuelTarget;

	BYTE m_bInPcBang;
	DWORD m_dwPcBangTime;

	__time64_t m_timeEnter;
	BYTE m_bPcBangItemCnt;

	BYTE m_bLuckyNumber;

	DWORD m_dwSoulmate;
	CString m_strSoulmate;
	DWORD m_dwSoulSilence;
	WORD m_wGodBall;
	WORD m_wLastSpawnID;
	DWORD m_dwLastDestination;
	CPacket m_questlist_possible;

	WORD m_wCastle;
	BYTE m_bCamp;
	BYTE m_bGraceExit;

	DWORD m_dwLastAttackCharID;
	DWORD m_dwLastAttackTick;

	WORD m_wPostTotal;
	WORD m_wPostRead;

	DWORD m_dwPostID;
	BYTE m_bRPSType;
	BYTE m_bRPSWin;

	WORD m_wArenaID;
	BYTE m_bArenaTeam;

public:
	DWORD m_dwIPAddr;
	WORD m_wPort;

	BYTE m_bCloseAll;
	BYTE m_bLogout;
	BYTE m_bMain;

	__int64 m_nChatBanTime;
	TCHATLOG m_chatlog;
	VCHATLOG m_vChatLog;

public:
	// 주의 : PushTItem() 호출전에 반드시 CanPush()가 성공인지 확인해야 하며 CanPush()와 PushTItem()사이에
	//		  아이템 삭제를 제외한 다른 인벤토리 작업을 하면 무한루프에 빠질 수 있다.
	virtual void ResetSize( WORD wMonTempID);
	virtual void PushTItem( LPVTITEM pTITEM);
	virtual BYTE PushTItem(
		CTItem *pItem,
		BYTE bInvenID,
		BYTE bSlotID);

	virtual DWORD GetPartyChiefID();
	virtual WORD GetCommanderID();

	virtual BYTE GetLevel();
	virtual BYTE GetClass();

	virtual void OnDie(
		DWORD dwAttackID,
		BYTE bObjectType, 
		WORD wTempMonID);

	virtual void Defend(
		CTSkillTemp *pTemp,
		BYTE bSkillLevel,
		DWORD dwHostID,
		DWORD dwAttackID,
		BYTE bAttackType,
		WORD wAttackPartyID,
		BYTE bAttackCountry,
		BYTE bAttackAidCountry,
		BYTE bAttackClass,
		DWORD dwActID,
		DWORD dwAniID,
		DWORD dwPysMinPower,
		DWORD dwPysMaxPower,
		DWORD dwMgMinPower,
		DWORD dwMgMaxPower,
		WORD wTransHP,
		WORD wTransMP,
		BYTE bCurseProb,
		BYTE bEquipSpecial,
		BYTE bCanSelect,
		WORD wAttackLevel,
		BYTE bAttackerLevel,
		BYTE bCP,
		BYTE bHitType,
		DWORD dwRemainTick,
        FLOAT fAtkPosX,
		FLOAT fAtkPosY,
		FLOAT fAtkPosZ,
        FLOAT fDefPosX,
		FLOAT fDefPosY,
		FLOAT fDefPosZ,
		BYTE bMirror);

	virtual DWORD OnDamage(
		DWORD dwHostID,
		DWORD dwAttackID,
		BYTE bAttackType,
		BYTE bAttackCountry,
		BYTE bAttackAidCountry,
		BYTE bAtkCanSelect,
		WORD wPartyID,
		int nDamageHP,
		int nDamageMP);

	virtual void Recover(DWORD dwTick);
	void ClearMain();
	void ClearQuest();
	void ClearHotkey();
	void ClearStorageItem();
	void ClearRecallMon(BYTE bExitMap);
	void ClearSelfMon(BYTE bExitMap);
	void ClearPost();
	void ClearStore();
	void ClearPet();
	void ClearDuel();
	void ClearGuildItem(); // 길드
	void ClearDuringItem();
	void ClearProtected();

	void LeaveAllRecallMon();
	void AddHotKey(
		BYTE bInven,
		BYTE bPos,
		BYTE bType,
		WORD wID);

	void EraseHotKey(
		BYTE bInven,
		BYTE bPos);

	void CheckTimeRecallMon(DWORD dwTick);
	void CheckRecallMon(
		LPTMONSTER pMON,
		BYTE bForce);
	CTRecallMon *FindMainRecall();

	BYTE IsEmptyHotkey(LPTHOTKEYINVEN pInven);
	BYTE CheckPortalCondition(LPTPORTAL pPortal, BYTE bCondition, DWORD dwConditionID);

	// Quest management
	DWORD GetQuestLevel(LPQUESTTEMP pQUEST);
	void DropQuest(DWORD dwQuestID);

	BYTE CheckLevelCondition(
		LPQUESTTEMP pQUEST,
		DWORD dwTick,
		BYTE& bLevel);

	BYTE CheckQuestCondition(
		LPQUESTTEMP pQUEST,
		DWORD dwTick,
		BYTE& bLevel);

	BYTE IsRunningQuest(
		DWORD dwQuestID);

	BYTE CanRunQuest(
		LPQUESTTEMP pQUEST,
		DWORD dwTick,
		BYTE & bLevel);

	BYTE OnQuestComplete(
		LPMAPTSKILLTEMP pSKILLTEMP,
		LPMAPTITEMTEMP pITEMTEMP,
		LPMAPCLASS pCLASS,
		LPQUESTTEMP pQUEST,
		BYTE bRewardType,
		DWORD dwRewardID);

	DWORD CheckQuest(
		DWORD dwTick,
		DWORD dwTermID,
		BYTE bTermType,
		int nCount);

	DWORD GetCompleteQuestID(
		CQuest *pQuest,
		DWORD dwQuestID);
	DWORD GetPossibleQuestID( CQuest * pQuest );

	CQuest *FindQuest(DWORD dwQuestID);

	void OnTimer(DWORD dwTick);
	void GainExp(DWORD dwGain, BYTE bIsMon);
	BYTE UseExp(DWORD dwExp, BYTE bUsed);

	BYTE IsExpBenefit(WORD &wBonus);

	LPTCABINET GetCabinet(BYTE bCabinetID);
	DWORD GetCabinetItemIndex(LPTCABINET pCabinet);
	CTItem* GetCabinetItem(BYTE bCabinetID, DWORD dwStItemID);
	void FindCabinetItem(BYTE bCabinetID, VTITEM &vItem, CTItem * pItem);
	void EraseCabinetItem(BYTE bCabinetID, DWORD dwStItemID);
	LPTCABINET PutinCabinet(BYTE bCabinetID, BYTE bUse);
	BYTE PutinCabinetItem(BYTE bCabinetID, BYTE bInven, BYTE bItemID, BYTE bCount);
	BYTE TakeoutStorageItem(BYTE bCabinetID, DWORD dwStItemID, BYTE bCount, DWORD dwMoney, BYTE bInvenID, BYTE bItemID);
	
	BYTE UseMoney(__int64 llMoney, BYTE bUsed);
	BYTE EarnMoney(__int64 llMoney);
	BYTE IsEnoughSkillPoint(CTSkillTemp * pSkill);
	BYTE GetTermCount(DWORD dwTermID, BYTE bTermType);
	CTRecallMon *FindRecallMon(DWORD dwMonID);
	CTSelfObj *FindSelfObj(DWORD dwMonID);

	DWORD GetExpVital();
	FLOAT GetLevelRate(BYTE bLevel);
	BYTE SetAftermath(DWORD dwTick, BYTE bStep);
	void ResetAftermath(DWORD dwTick);
	void DeleteRecallMon(DWORD dwID);
	void DeleteSelfMon(DWORD dwID);

	void Revival(BYTE bType, CTSkillTemp * pTemp, BYTE bLevel);
	void GetSkillKindPoint(WORD wPoint[]);
	BYTE ChangeSwitch(DWORD dwSwitchID);
	void ClearDealItem();
	BYTE SetDealItemTarget(CString strTarget);
	void DeleteDealItem();
	void DeleteRecvDealItem();
	void DeleteSendDealItem();
	void EraseInvenDealItem();
	BYTE CheckDealItem(BYTE bInven, BYTE bItemID);
	BYTE CheckStoreItem(BYTE bInven, BYTE bItemID);
	BYTE ValidDealItem();
	void AutoEquipSkill();
	void SendQuestTimer(DWORD dwTick);
	void CheckEquipSkill();
	void CheckDuringItem(__time64_t tCurrent, DWORD dwPassTime);
	BYTE HangPremiumItem();
	void HangExpBuff();
	void MoveGhost(FLOAT fPosX, FLOAT fPosZ);
	BYTE CanHost(FLOAT fPosX, FLOAT fPosZ);
	LPTPOST MakePost(
		DWORD dwPostID,
		DWORD dwSendID,
		CString strSender,
        CString strTitle,
        CString strMessage,
        BYTE bType,
		BYTE bRead,
        __int64 timeRecv,
        DWORD dwGold,
		DWORD dwSilver,
        DWORD dwCooper);
	BYTE HavePostBill();
	CTRecallMon * FindRecallPet();
	BYTE GetRecallCount(BYTE bRecallType);
	DWORD GetRecallLifeTime();
	void StoreClose();
	void PetRiding(DWORD dwRiding);

	BYTE CanDuel(CTPlayer * pAttacker);
	BYTE DuelLose(DWORD dwAttackerID);
	void DeleteAllRecallMon();
	void DeleteAllSelfMon();
	void DeleteAllMaintainSkill();

	void CopyGuildItem(CTItem * pItem, BYTE bItem, BYTE bGenID);
	void BackGuildItem();

	BYTE CheckGuildDuty(BYTE bDuty);
	WORD MagicBackSkill(BYTE bMagic);

	CTSkill * FindTChildSkill(WORD wParentID);
	void InitializeSkill(CTSkill * pSkill);
	void InitSkillPossible(BYTE bLevel, LPVWORD vSkill);
	void EraseHotkey(WORD wSkillID);
	LPTDURINGITEM SetDuringItem(BYTE bKind, LPTITEM pITEM, BYTE bType, DWORD dwAdd);
	void ResetPcBangData(BYTE bInPcBang, DWORD dwRemainTime);
	LPTPET PetMake(WORD wPetID, CString strName, __int64 ldwTime);
	void RemoveGodBall(LPTGODBALL pBall = NULL);

	void ClearSoulmate();
	void SetSoulmate(DWORD dwSoulmate, CString strSoulmate, DWORD dwSilence);
	BYTE CheckSoulSilence(DWORD dwTime);
	void CheckSoulEXP(VPLAYER &vParty, DWORD dwEXP);
	void CheckSoulHP();
	BYTE CheckSoulTakeHP();
	void TransSoulHP(CTPlayer * pPlayer);
	void DropGodBall();
	void MapKillPoint();
	WORD DurationDec(BYTE bSlot, BYTE bDel = FALSE);
	void ChangeEquipItem();
	void ResetCoolTime(WORD wExceptSkill);
	void CheckItemEndTime(__time64_t dCurrentTime);
	BYTE ChangeCharBase(BYTE bType, CString strName = NAME_NULL, BYTE bValue=0);
	void SendQuestListPossible(CPacket * pPacket);
	BYTE CheckItemBuff(CTSkillTemp * pSkill);
	void DeleteItem(WORD wItemID);
	BYTE GetPvPStatus();
	void GainPvPoint(DWORD dwPoint, BYTE bEvent, BYTE bType, LPTRECORDSET pRec = NULL);
	void UsePvPoint(DWORD dwPoint, BYTE bEvent, BYTE bType, LPTRECORDSET pRec = NULL);
	void RecordPvP(LPTRECORDSET pRec);
	void SetDuelScore(CTPlayer* pChar,BYTE bWin);	
	void DuelScoreReset();	
	void RecordDuel(CTPlayer* pChar, BYTE bWin, DWORD dwPoint);
	CTSkill * GetMonRecallSkill();
	BYTE CheckProtected(DWORD dwTarget, BYTE bOption);
	void RecallRevival();
	void RecallRevivalErase();
	void ResetCloak();

	void ClearAuctionList();
	void AuctionInterestSave(BYTE bType,DWORD dwAuctionID);
	void AuctionBidListSave(BYTE bType, DWORD dwAuctionID);
	void AuctionRegListSave(BYTE bType, DWORD dwAuctionID);
	 
	void MonthRankRest();
	BYTE CanUseAuction();

	BYTE HaveItem(WORD wItemID, BYTE bCount);
	void UseItem(WORD wItemID, BYTE bCount);
	BYTE IsEquipShield();
	BYTE IsTutorial();
	BYTE ProtectTutorial();
	void ReleaseTutorial();
	BYTE InitCharInfo();
	BYTE HaveInvenBlank();
	void ResetRPS();
	BYTE CanTalk(BYTE bChatType, BYTE bCountry, BYTE bAidCountry);
	BYTE CheckChat(CString strTalk);
	DWORD GetAidLeftTime();

public:
	// Control Server Message
	void SendCT_SERVICEMONITOR_REQ(DWORD dwTick, DWORD dwSession, DWORD dwUser, DWORD dwActiveUser);
	void SendCT_MONSPAWNFIND_ACK(DWORD dwManager, WORD wMapID, WORD wSpawnID, CTMonSpawn * pSPAWN);
	void SendCT_CASTLEINFO_ACK(DWORD dwManagerID);
	

	// CS message sender
	void SendCS_GAMBLECHECK_ACK(BYTE bResult, BYTE bInven=0, BYTE bSlot=0, DWORD dwMoney=0, BYTE bDiscountRate=0);
	void SendCS_GAMBLEOPEN_ACK(
		BYTE bResult,
		BYTE bInven,
		BYTE bItem,
		CTItem * pItem = NULL);
	void SendCS_DEALITEMEND_ACK(BYTE bResult, CString strTarget);
	void SendCS_DEALITEMASK_ACK(CString strInviter);
	void SendCS_DEALITEMSTART_ACK(CString strTarget);
	void SendCS_DEALITEMADD_ACK(
		DWORD dwGold,
		DWORD dwSilver,
		DWORD dwCooper,
		LPVTITEM pvItem);

	void SendCS_TELEPORT_ACK(
		BYTE bResult,
		DWORD dwRange,
		DWORD dwID,
		BYTE bType,
		WORD wMapID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);
	void SendCS_BEGINTELEPORT_ACK( BYTE bChannel, WORD wMapID);

	void SendCS_MONEY_ACK();
	void SendCS_HOTKEYCHANGE_ACK(
		BYTE bInven,
		THOTKEY hotkey[],
		BYTE bCount);
	void SendCS_MOVEITEM_ACK( BYTE bResult);
	void SendCS_CONNECT_ACK( BYTE bResult, LPVBYTE vServerID = NULL);

	void SendCS_INVALIDCHAR_ACK();
	void SendCS_CHARINFO_ACK(
		DWORD dwTick,
		DWORD dwPrevExp,
		DWORD dwNextExp);

	void SendCS_ENTER_ACK(
		CTPlayer *pPlayer,
		BYTE bNewMember,
		DWORD dwTick);

	void SendCS_LEAVE_ACK(
		DWORD dwCharID,
		BYTE bExitMap);

	void SendCS_MOVE_ACK(
		DWORD dwCharID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wPitch,
		WORD wDIR,
		BYTE bMouseDIR,
		BYTE bKeyDIR,
		BYTE bAction);

	void SendCS_JUMP_ACK(
		DWORD dwObjID,
		BYTE bObjType,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wPitch,
		WORD wDIR,
		BYTE bAction);

	void SendCS_BLOCK_ACK(
		DWORD dwObjID,
		BYTE bObjType,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wPitch,
		WORD wDIR,
		BYTE bAction,
		BYTE bBlock);

	void SendCS_MONHOST_ACK(
		DWORD dwMonID,
		BYTE bSet);

	void SendCS_ADDMON_ACK(
		CTMonster *pMON,
		BYTE bNewMember,
		DWORD dwTick);

	void SendCS_ADDRECALLMON_ACK(
		CTRecallMon * pMON,
		BYTE bNewMember,
		DWORD dwTick);

	void SendCS_ADDSELFOBJ_ACK(
		CTSelfObj * pSelf,
		BYTE bNewMember,
		DWORD dwTick);

	void SendCS_DELMON_ACK(
		DWORD dwMonID,
		BYTE bExitMap);

	void SendCS_DELRECALLMON_ACK(
		DWORD dwHostID,
		DWORD dwMonID,
		BYTE bExitMap);

	void SendCS_DELSELFOBJ_ACK(
		DWORD dwObjID,
		BYTE bExitMap);

	void SendCS_MONACTION_ACK(
		DWORD dwMonID,
		BYTE bAction,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		DWORD dwTargetID=0,
		BYTE bTargetType=0);

	void SendCS_MONMOVE_ACK(
		DWORD dwMonID,
		BYTE bType,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ,
		WORD wPitch,
		WORD wDIR,
		BYTE bMouseDIR,
		BYTE bKeyDIR,
		BYTE bAction);

	void SendCS_MONMOVE_ACK(LPMAPTMONSTER pMapMon);

	void SendCS_CHGMODE_ACK(
		DWORD dwID,
		BYTE bType,
		BYTE bMode);

	void SendCS_ACTION_ACK(
		BYTE bResult,
		DWORD dwObjID,
		BYTE bObjType,
		BYTE bActionID,
		DWORD dwActID,
		DWORD dwAniID,
		WORD wSkillID);

	void SendCS_MONATTACK_ACK(
		DWORD dwAttackID,
		DWORD dwTargetID,
		BYTE bAttackType,
		BYTE bTargetType,
		WORD wSkillID);

	void SendCS_DEFEND_ACK(
		DWORD dwAttackID,
		DWORD dwTargetID,
		BYTE bAttackType,
		BYTE bTargetType,
		DWORD dwHostID,
		BYTE bHostType,
		DWORD dwActID,
		DWORD dwAniID,
		BYTE bIsMaintain,
		DWORD dwMaintainTick,
		BYTE bHit,
		BYTE bAtkHit,
		WORD wAttackLevel,
		BYTE bAttackerLevel,
        DWORD dwPysMinPower,
        DWORD dwPysMaxPower,
		DWORD dwMgMinPower,
		DWORD dwMgMaxPower,
		BYTE bCanSelect,
		BYTE bCancelCharge,
		BYTE bAttackCountry,
		BYTE bAttackAidCountry,
		WORD wSkillID,
		BYTE bSkillLevel,
		WORD wBackSkill,
		BYTE bPerform,
		FLOAT fAtkPosX,
		FLOAT fAtkPosY,
		FLOAT fAtkPosZ,
		FLOAT fDefPosX,
		FLOAT fDefPosY,
		FLOAT fDefPosZ,
		MAPWORD* mapSkillDamage);

	void SendCS_HPMP_ACK(
		DWORD dwID,
		BYTE bType,
		BYTE bLevel,
		DWORD dwMaxHP,
		DWORD dwHP,
		DWORD dwMaxMP,
		DWORD dwMP);

	void SendCS_LEVEL_ACK(
		DWORD dwCharID,
		BYTE bLevel);

	void SendCS_EXP_ACK();

	void SendCS_DIE_ACK(
		DWORD dwID,
		BYTE bType);

	void SendCS_REVIVAL_ACK(
		DWORD dwCharID,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	void SendCS_UPDATEITEM_ACK(
		CTItem *pTITEM,
		BYTE bInvenID);

	void SendCS_ADDITEM_ACK(
		CTItem *pTITEM,
		BYTE bInvenID);

	void SendCS_DELITEM_ACK(
		BYTE bInvenID,
		BYTE bItemID);

	void SendCS_EQUIP_ACK(
		CTPlayer *pPlayer);

	void SendCS_SKILLBUY_ACK(
		BYTE bRet,
		WORD wSkillID,
		BYTE bLevel);
	void SendCS_SKILLUSE_ACK(
		BYTE bResult,
		DWORD dwAttackID,
		BYTE bAttackType,
		WORD wSkillID,
		BYTE bActionID,
		DWORD dwAct,
		DWORD dwAniID,
		BYTE bSkillLevel=0,
		WORD wBackSkill=0,
		WORD wAttackLevel=0,
		BYTE bAttackerLevel=0,
		DWORD dwPysMinPower=0,
		DWORD dwPysMaxPower=0,
		DWORD dwMgMinPower=0,
		DWORD dwMgMaxPower=0,
		WORD wTransHP=0,
		WORD wTransMP=0,
		BYTE bCurseProb=0,
		BYTE bEquipSpecial=0,
		BYTE bCanSelect=0,
		BYTE bAttackCountry=0,
		BYTE bAttackAidCountry=0,
		BYTE bCP=0,
		FLOAT fGndPosX=0,
		FLOAT fGndPosY=0,
		FLOAT fGndPosZ=0,
		LPVDWORD vDefend=NULL,
		LPVBYTE vDefendType=NULL);
	void SendCS_LOOPSKILL_ACK(
		BYTE bResult,
		DWORD dwAttackID,
		BYTE bAttackType,
		WORD wSkillID,
		BYTE bSkillLevel=0,
		WORD wAttackLevel=0,
		BYTE bAttackerLevel=0,
		DWORD dwPysMinPower=0,
		DWORD dwPysMaxPower=0,
		DWORD dwMgMinPower=0,
		DWORD dwMgMaxPower=0,
		BYTE bCanSelect=0,
		BYTE bAttackCountry=TCONTRY_N,
		BYTE bAttackAidCountry=TCONTRY_N,
		BYTE bHit=0,
		FLOAT fGndPosX=0,
		FLOAT fGndPosY=0,
		FLOAT fGndPosZ=0,
		LPVDWORD vDefend=NULL,
		LPVBYTE vDefendType=NULL);
	void SendCS_SKILLEND_ACK(
		DWORD dwObjID,
		BYTE bObjType,
		WORD wSkillID);

	void SendCS_CHGPARTYTYPE_ACK(
		BYTE bRet,
		BYTE bPartyType);
	void SendCS_PARTYADD_ACK(
		CString strOrigin,
		CString strTarget,
		BYTE bReply);
	void SendCS_PARTYJOINASK_ACK(
		CString strOrigin,
		BYTE bObtainType);
	void SendCS_PARTYJOIN_ACK(
		WORD wPartyID,
		CString strMemberID,
		DWORD dwMemberID,
		DWORD dwChiefID,
		WORD wCommanderID,
		CString strGuildName,
		BYTE bLevel,
		DWORD dwMaxHP,
		DWORD dwHP,
		DWORD dwMaxMP,
		DWORD dwMP,
		BYTE bRace,
		BYTE bSex,
		BYTE bFace,
		BYTE bHair,
		BYTE bPartyType,
		BYTE bClass);
	void SendCS_PARTYDEL_ACK(
		DWORD dwMemberID,
		DWORD dwChief,
		WORD wCommander,
		WORD dwPartyID,
		BYTE bKick);
	void SendCS_PARTYMANSTAT_ACK(
		DWORD dwID,
		BYTE bType,
		BYTE bLevel,
		DWORD dwMaxHP,
		DWORD dwCurHP,
		DWORD dwMaxMP,
		DWORD dwCurMP);

	void SendCS_QUESTADD_ACK(
		DWORD dwQuestID,
		BYTE bType);
	void SendCS_QUESTUPDATE_ACK(
		DWORD dwQuestID,
		DWORD dwTermID,
		BYTE bType,
		BYTE bCount,
		BYTE bStatus);
	void SendCS_QUESTCOMPLETE_ACK(
		BYTE bResult,
		DWORD dwQuestID,
		DWORD dwTermID,
		BYTE bType,
		DWORD dwDropID);
	void SendCS_QUESTLIST_ACK( DWORD dwTick);
	void SendCS_QUESTLIST_COMPLETE_ACK();
	void SendCS_QUESTSTARTTIMER_ACK(
		DWORD dwQuestID,
		DWORD dwTick);

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	// 길드
	void SendCS_GUILDESTABLISH_ACK(BYTE bResult, DWORD dwID=0, CString strName=NAME_NULL);
	void SendCS_GUILDDISORGANIZATION_ACK(BYTE bResult);
	void SendCS_GUILDINVITE_ACK(BYTE bResult, CString strGuildName, DWORD dwInviter, CString strInviterName);
	void SendCS_GUILDJOIN_ACK(BYTE bRet, DWORD dwGuildID, CString strGuildName, DWORD dwNewMemberID, CString strNewMemberName,BYTE bMaxGuildMember);
	void SendCS_GUILDDUTY_ACK(BYTE bResult,	CString strTarget, BYTE bDuty);
	void SendCS_GUILDPEER_ACK(BYTE bResult, CString strTarget, BYTE bPeer, BYTE bOldPeer);
	void SendCS_GUILDLEAVE_ACK(BYTE bResult, CString strTarget, BYTE bReason);
	void SendCS_GUILDMEMBERLIST_ACK(LPPACKETBUF pBUF);
	void SendCS_GUILDINFO_ACK(LPPACKETBUF pBUF);
	void SendCS_GUILDLOCALLIST_ACK();
	void SendCS_GUILDLOCALRETURN_ACK(BYTE bResult);
	void SendCS_GUILDCABINETLIST_ACK(BYTE bMaxCabinet, VTITEM vItem);
	void SendCS_GUILDCABINETPUTIN_ACK(BYTE bResult);
	void SendCS_GUILDCABINETTAKEOUT_ACK(BYTE bResult);
	void SendCS_GUILDCONTRIBUTION_ACK(BYTE bResult);
	void SendCS_GUILDARTICLELIST_ACK(LPPACKETBUF pBUF);
	void SendCS_GUILDARTICLEADD_ACK(BYTE bResult);
	void SendCS_GUILDARTICLEDEL_ACK(BYTE bResult);
	void SendCS_GUILDARTICLEUPDATE_ACK(BYTE bResult);
	void SendCS_GUILDFAME_ACK(BYTE bResult, DWORD dwFame=0, DWORD dwFameColor=0);
	void SendCS_GUILDWANTEDADD_ACK(BYTE bResult);
	void SendCS_GUILDWANTEDDEL_ACK(BYTE bResult);
	void SendCS_GUILDWANTEDLIST_ACK(CPacket * pPacket);
	void SendCS_GUILDVOLUNTEERING_ACK(BYTE bResult);
	void SendCS_GUILDVOLUNTEERINGDEL_ACK(BYTE bResult);
	void SendCS_GUILDVOLUNTEERLIST_ACK(CPacket * pPacket);
	void SendCS_GUILDVOLUNTEERREPLY_ACK(BYTE bResult);
	void SendCS_GUILDTACTICSWANTEDADD_ACK(BYTE bResult);
	void SendCS_GUILDTACTICSWANTEDDEL_ACK(BYTE bResult);
	void SendCS_GUILDTACTICSWANTEDLIST_ACK(CPacket * pPacket);
	void SendCS_GUILDTACTICSVOLUNTEERING_ACK(BYTE bResult);
	void SendCS_GUILDTACTICSVOLUNTEERINGDEL_ACK(BYTE bResult);
	void SendCS_GUILDTACTICSVOLUNTEERLIST_ACK(CPacket * pPacket);
	void SendCS_GUILDTACTICSREPLY_ACK(BYTE bResult, DWORD dwTarget);
	void SendCS_GUILDTACTICSKICKOUT_ACK(BYTE bResult, DWORD dwTarget);
	void SendCS_GUILDTACTICSINVITE_ACK(
		CString strGuildName,
		CString strName,
		BYTE bDay,
		DWORD dwPoint,
		DWORD dwGold,
		DWORD dwSilver,
		DWORD dwCooper);
	void SendCS_GUILDTACTICSANSWER_ACK(BYTE bResult, DWORD dwMemberID);
	void SendCS_GUILDTACTICSLIST_ACK(LPPACKETBUF pBUF);

	/////////////////////////////////////////////////////////////////////////////////////////////////////

	void SendCS_CHAT_ACK(
		BYTE bGroup,
		DWORD dwSenderID,
		CString strName,
		CString strTalk);
	void SendCS_CABINETLIST_ACK();
	void SendCS_CABINETITEMLIST_ACK(BYTE bResult, LPTCABINET pTCABINET);
	void SendCS_CABINETOPEN_ACK(BYTE bResult, BYTE bCabinetID);
	void SendCS_NPCITEMLIST_ACK(CTNpc * pNpc);
	void SendCS_NPCITEMLIST_ACK(WORD wID, LPVWORD pvItem);
	void SendCS_ITEMBUY_ACK(
		BYTE bResult,
		WORD wItemID);
	void SendCS_MAGICITEMBUY_ACK(
		BYTE bResult);
	void SendCS_ITEMSELL_ACK(BYTE bResult);
	void SendCS_MONITEMTAKE_ACK( BYTE bRet);
	void SendCS_MONITEMLIST_ACK(BYTE bRet, DWORD dwMonID, DWORD dwMoney, CTInven * pInven, BYTE bUpdate);
	void SendCS_MONITEMLOTTERY_ACK(DWORD dwMonID, CTItem *pItem);
	void SendCS_QUESTLIST_POSSIBLE_ACK(LPMAPMAPDWORD mapNpcQuest);
	void SendCS_FRIENDASK_ACK(CString strInviter);
	void SendCS_FRIENDADD_ACK(BYTE bResult, DWORD dwFriendID, CString strName, BYTE bLevel, BYTE bGroup, BYTE bClass, DWORD dwRegion);
	void SendCS_FRIENDERASE_ACK(BYTE bResult, DWORD dwFriendID);
	void SendCS_FRIENDLIST_ACK(CPacket * pMsg);
	void SendCS_FRIENDCONNECTION_ACK(BYTE bResult, CString strName, DWORD dwRegion);
	void SendCS_FRIENDREGION_ACK(CString strName, DWORD dwRegion);
	void SendCS_CHGPARTYCHIEF_ACK(BYTE bRET);
	void SendCS_CHGSQUADCHIEF_ACK(WORD wSquadID, DWORD dwChiefID);
	void SendCS_PARTYATTR_ACK(DWORD dwCharID, WORD wPartyID, DWORD dwChiefID, WORD wCommander);
	void SendCS_GUILDATTR_ACK(DWORD dwCharID, DWORD dwGuildID, DWORD dwFame, DWORD dwFameColor, CString strName, BYTE bPeer, DWORD dwTacticsID, CString strTacticsName);
	void SendCS_POSTLIST_ACK(CPacket * pMSG);
	void SendCS_CHANGECOLOR_ACK(DWORD dwID, BYTE bType, BYTE bColor, BYTE bCountry);
	void SendCS_CHARSTATINFO_ACK(CTPlayer * pPlayer);
	void SendCS_CHARSTATINFO_ACK(CPacket * pPacket);
	void SendCS_CANCELACTION_ACK(DWORD dwObjID, BYTE bObjType);
	void SendCS_LOCALENABLE_ACK(WORD wID, BYTE bStatus);
	void SendCS_LOCALOCCUPY_ACK(BYTE bType, WORD wLocalID, BYTE bCountry, DWORD dwGuildID);
	void SendCS_CASTLEOCCUPY_ACK(BYTE bType, WORD wCastleID, DWORD dwGuildID);
	void SendCS_ITEMUPGRADE_ACK(BYTE bResult, BYTE bTargetInven, BYTE bTargetItemID, BYTE bGrade, BYTE bGradeEffect = 0, WORD wColor = 0);
	void SendCS_ITEMMAGICGRADE_ACK(BYTE bResult, BYTE bTargetInven, CTItem * pItem);
	void SendCS_CHGCORPSCOMMANDER_ACK(BYTE bRET);
	void SendCS_CORPSASK_ACK(CString strAskName);
	void SendCS_CORPSREPLY_ACK(BYTE bResult, CString strName);
	void SendCS_CORPSJOIN_ACK(WORD wCorpsID, WORD wCommander);
	void SendCS_ADDSQUAD_ACK(CPacket * pPacket);
	void SendCS_DELSQUAD_ACK(WORD wSquad);
	void SendCS_DELCORPSUNIT_ACK( WORD wSquad, DWORD dwTarget);
	void SendCS_ADDCORPSUNIT_ACK(CPacket * pPacket);
	void SendCS_CORPSCMD_ACK(WORD wSquadID, DWORD dwCharID, WORD wMapID, BYTE bCMD, DWORD dwTargetID, BYTE bTargetType, WORD wPosX, WORD wPosZ);
	void SendCS_CORPSENEMYLIST_ACK(CPacket * pMsg);
	void SendCS_MOVECORPSENEMY_ACK(CPacket * pMsg);
	void SendCS_MOVECORPSUNIT_ACK(CPacket * pMsg);
	void SendCS_ADDCORPSENEMY_ACK(CPacket * pMsg);
	void SendCS_DELCORPSENEMY_ACK(CPacket * pMsg);
	void SendCS_PROTECTEDLIST_ACK();
	void SendCS_CORPSHP_ACK(CPacket * pMsg);
	void SendCS_REPORTENEMYLIST_ACK(WORD wSquadID, DWORD dwCharID);
	void SendCS_PARTYMOVE_ACK(BYTE bResult);
	void SendCS_PROTECTEDADD_ACK(BYTE bResult, DWORD dwCharID, CString strProtected, BYTE bOption);
	void SendCS_PROTECTEDERASE_ACK(BYTE bResult, CString strProtected);
	void SendCS_FRIENDGROUPMAKE_ACK(BYTE bRet, BYTE bGroup, CString strName);
	void SendCS_FRIENDGROUPDELETE_ACK(BYTE bRet, BYTE bGroup);
	void SendCS_FRIENDGROUPCHANGE_ACK(BYTE bRet, BYTE bGroup, DWORD dwFriend);
	void SendCS_FRIENDGROUPNAME_ACK(BYTE bRet, BYTE bGroup, CString strName);
	void SendCS_TMSRECV_ACK(DWORD dwTMS, CString strSender, CString strMessage);
	void SendCS_TMSINVITE_ACK(CPacket * pPacket);
	void SendCS_TMSOUT_ACK(DWORD dwTMS, CString strMember);
	void SendCS_POSTSEND_ACK(BYTE bResult);
	void SendCS_POSTRECV_ACK(DWORD dwPostID, BYTE bRead, CString strSender, CString strTitle, BYTE bType, __int64 ldwTime);
	void SendCS_POSTVIEW_ACK(DWORD dwPostID);
	void SendCS_POSTDEL_ACK(DWORD dwPostID);
	void SendCS_POSTGETITEM_ACK(BYTE bResult);
	void SendCS_POSTRETURN_ACK(BYTE bResult);
	void SendCS_CASTLEENABLE_ACK(WORD wCastle, BYTE bStatus);
	void SendCS_CASTLEAPPLY_ACK(
		BYTE bResult,
		WORD wCastle = 0,
		DWORD dwTarget = 0,
		BYTE bCamp = 0);
	void SendCS_ITEMUSE_ACK(BYTE bResult, WORD wDelayGroupID, BYTE bKind, DWORD dwTick);
	void SendCS_ANNOUNCEMENTCHAT_ACK(CString strAnnounce); // 현승룡 공지사항
	void SendCS_MONSETSKILL_ACK(DWORD dwMonID, WORD wSkillID, WORD wAtkRange);
	void SendCS_NPCTALK_ACK(DWORD dwQuestID, WORD wNpcID);
	void SendCS_CHAPTERMSG_ACK(DWORD dwQuestID);
	void SendCS_INVENADD_ACK( BYTE bResult, BYTE bInvenID, WORD wItemID, __time64_t dEndTime);
	void SendCS_INVENDEL_ACK( BYTE bResult, BYTE bInvenID);
	void SendCS_INVENMOVE_ACK( BYTE bResult, BYTE bSRCInvenID, BYTE bDESTInvenID);
	void SendCS_GATEADD_ACK(DWORD dwGateID, BYTE bOpened);
	void SendCS_GATEDEL_ACK(DWORD dwGateID);
	void SendCS_GATECHANGE_ACK(DWORD dwGateID, BYTE bOpened);
	void SendCS_SWITCHADD_ACK(DWORD dwSwitchID, BYTE bOpened);
	void SendCS_SWITCHDEL_ACK(DWORD dwSwitchID);
	void SendCS_SWITCHCHANGE_ACK(BYTE bResult, DWORD dwSwitchID, BYTE bOpened);
	void SendCS_REVIVALASK_ACK(DWORD dwID, BYTE bType, WORD wSkillID, BYTE bLevel);
	void SendCS_REVIVALREPLY_ACK(BYTE bReply, DWORD dwDefender);
	void SendCS_DROPDAMAGE_ACK(DWORD dwID, BYTE bType, DWORD dwDamage);
	void SendCS_SETRETURNPOS_ACK(BYTE bResult);

	void SendCS_STOREOPEN_ACK(BYTE bResult, DWORD dwCharID, CString strName);
	void SendCS_STORECLOSE_ACK(BYTE bResult, DWORD dwCharID);
	void SendCS_STOREITEMLIST_ACK(DWORD dwID, CString strName, CTPlayer * pTarget);
	void SendCS_STOREITEMBUY_ACK(BYTE bResult, WORD wItemID, BYTE bCount);
	void SendCS_STOREITEMSELL_ACK(BYTE bItem, BYTE bCount);

	void SendCS_PETMAKE_ACK(BYTE bResult, WORD wPetID, CString strName, __int64 ldwTime);
	void SendCS_PETDEL_ACK(BYTE bResult, WORD wPetID);
	void SendCS_PETLIST_ACK();
	void SendCS_PETRECALL_ACK(BYTE bResult);
	void SendCS_PETRIDING_ACK(BYTE bResult, DWORD dwCharID, DWORD dwMonID, BYTE bAction);
	void SendCS_OTHERSELF_ACK(CTPlayer * pPlayer, CTRecallMon * pMon, DWORD dwTick, BYTE bNewMember);
	void SendCS_SHUTDOWN_ACK();
	void SendCS_CHGCHANNEL_ACK(BYTE bResult, BYTE bChannel);
	void SendCS_TESTMSG_ACK();

	void SendCS_PARTYITEMTAKE_ACK(DWORD dwCharID, CTItem *pItem);
	void SendCS_GETITEM_ACK(CTItem *pTITEM);

	void SendCS_DUELINVITE_ACK(DWORD dwInviter);
	void SendCS_DUELSTART_ACK(BYTE bResult, DWORD dwInviter, DWORD dwTarget);
	void SendCS_DUELEND_ACK(DWORD dwLoser);
	void SendCS_DUELSTANDBY_ACK(DWORD dwInviter, DWORD dwTarget, FLOAT fPosX=0.0, FLOAT fPosZ=0.0);

	void SendCS_GETTARGETANS_ACK(DWORD dwCharID);
	void SendCS_GETTARGET_ACK(DWORD dwTargetID, BYTE bTargetType);
	void SendCS_SMSSEND_ACK(BYTE bResult, BYTE bType, CString strTarget);

	void SendCS_SKILLINIT_ACK(BYTE bResult, WORD wSkillID=0);
	void SendCS_SKILLLIST_ACK(DWORD dwTick);
	void SendCS_SKILLINITPOSSIBLE_ACK(VWORD vSkill);

	void SendCS_HELMETHIDE_ACK(DWORD dwCharID, BYTE bHide);

	void SendCS_PARTYMEMBERRECALL_ACK(BYTE bResult, BYTE bType, CString strName = NAME_NULL);
	void SendCS_PARTYMEMBERRECALLANS_ACK(CString strInviter, BYTE bInven, BYTE bItem);

	void SendCS_CASHITEMGET_ACK(BYTE bResult, DWORD dwCashItemID);
	void SendCS_CASHSHOPITEMLIST_ACK(DWORD dwCash, DWORD dwCashBonus, WORD wItemID, VTCASHCATEGORY & cash);
	void SendCS_CASHITEMBUY_ACK(BYTE bResult, DWORD dwCash, DWORD dwCashBonus);
	void SendCS_CASHITEMPRESENT_ACK(BYTE bResult, DWORD dwCash, DWORD dwCashBonus);
	void SendCS_RESETPCBANG_ACK(DWORD dwCharID, BYTE bInPcBang);

	void SendCS_SOULMATE_ACK();
	void SendCS_SOULMATESEARCH_ACK(BYTE bResult, DWORD dwTargetID = 0, CString strName=NAME_NULL, DWORD dwRegion=0);
	void SendCS_SOULMATEREGREADY_ACK(BYTE bResult, CString strName, BYTE bNpcInven, BYTE bNpcItemID);
	void SendCS_SOULMATEREG_ACK(BYTE bResult, DWORD dwCharID=0, CString strName=NAME_NULL, DWORD dwRegion=0);
	void SendCS_SOULMATEEND_ACK(BYTE bResult);

	void SendCS_ADDGODTOWER_ACK(LPTGODTOWER pGodTower);
	void SendCS_ADDGODBALL_ACK(LPTGODBALL pGodBall);
	void SendCS_DELGODTOWER_ACK(WORD wID);
	void SendCS_DELGODBALL_ACK(WORD wID);
	void SendCS_TAKEGODBALL_ACK(
		DWORD dwCharID,
		WORD wGodBallID);
	void SendCS_MOUNTGODBALL_ACK(
        WORD wGodTower,
		WORD wGodBall,
		BYTE bCamp,
        DWORD dwCharID);
	void SendCS_DEMOUNTGODBALL_ACK(
		WORD wGodTower,
        DWORD dwCharID);
	void SendCS_BALANCEOFPOWER_ACK(
        FLOAT fPower_Def,
		DWORD dwLeftTick,
		WORD wKillPoint_Atk,
		WORD wKillPoint_Def,
		CString strOwner1_Atk,
		CString strOwner2_Atk,
		CString strOwner1_Def,
		CString strOwner2_Def,
		WORD wKeep1_Atk,
		WORD wKeep2_Atk,
		WORD wKeep1_Def,
		WORD wKeep2_Def);

	void SendCS_REMOVEGODBALL_ACK(DWORD dwCharID);
	void SendCS_DURATIONDEC_ACK(BYTE bInven, BYTE bItem, DWORD dwDuration);
	void SendCS_DURATIONEND_ACK(BYTE bInven, BYTE bItem, BYTE bDeleted);
	void SendCS_DURATIONREPCOST_ACK(DWORD dwCost,BYTE bDiscountRate);  
	void SendCS_DURATIONREP_ACK(BYTE bResult, VTITEM &vItem);
	void SendCS_REFINECOST_ACK(DWORD dwCost,BYTE bDiscountRate); 
	void SendCS_REFINE_ACK(BYTE bResult, BYTE bInven, CTItem * pItem);
	void SendCS_RESETCOOLTIME_ACK(WORD wExceptSkill);
	void SendCS_CASHCABINETBUY_ACK(
		BYTE bResult,
		__int64 ldwTime,
		DWORD dwCash,
		DWORD dwBonus);
	void SendCS_CASHITEMPUTIN_ACK(
		BYTE bResult,
		DWORD dwNewID,
		CTItem * pItem,
		DWORD dwCash=0,
		DWORD dwBonus=0);
	void SendCS_CASHCABINETBUYCASH_ACK(DWORD dwCash);
	void SendCS_CHANGECHARBASE_ACK(
		BYTE bResult,
		DWORD dwCharID,
		BYTE bType,
		BYTE bValue,
		CString strName,
		DWORD dwSecond = 0);
	void SendCS_STOPTHECLOCK_ACK(
		BYTE bResult,
		BYTE bInven,
		BYTE bItem,
		__time64_t dEndTime);
	void SendCS_HEROSELECT_ACK(BYTE bResult);    		
	void SendCS_HEROLIST_ACK();  
	void SendCS_SYSTEMMSG_ACK(
		BYTE bType,
		WORD wLocalID = 0,
		DWORD dwSecond = 0,
		CString strMsg1 = NAME_NULL,
		CString strMsg2 = NAME_NULL,
		BYTE bHour = 0,
		BYTE bMinute = 0,
		CString strPresent = NAME_NULL,
		BYTE bSelect = 0,
		INT64 dlValue = 0);
	void SendCS_SYSTEMMSG_ACK(BYTE bType, CPacket * pPacket);
	void SendCS_OPENMONEY_ACK(DWORD dwMoney);
	void SendCS_RELAYCONNECT_ACK(
		DWORD dwIP,
		WORD wPort);
	void SendCS_COMMENT_ACK(
		DWORD dwCharID,
		CString strComment);
	void SendCS_PVPPOINT_ACK(
		DWORD dwTotal,
		DWORD dwUseable,
		BYTE bEvent,
		DWORD dwMonthPvPoint);  
	void SendCS_GUILDPOINTLOG_ACK(LPPACKETBUF pBUF);
	void SendCS_GUILDPOINTREWARD_ACK(BYTE bRet, DWORD dwRemain);
	void SendCS_GUILDPVPRECORD_ACK(LPPACKETBUF pBUF);
	void SendCS_PVPRECORD_ACK(BYTE bType);
	void SendCS_ENDWAR_ACK(
		BYTE bType,
		DWORD dwWinGuildID,
		DWORD dwDefTotal,
		DWORD dwAtkTotal,
		CString strDefName,
		DWORD dwDefPower,
		WORD wDefPoint,
		WORD wDefKillPoint,
		CString strAtkName,
		DWORD dwAtkPower,
		WORD wAtkPoint,
		WORD wAtkKillPoint);
	void SendCS_ENTERCASTLE_ACK(WORD wCastle, BYTE bCamp, CString strName_Atk, CString strName_Def);
	void SendCS_LEAVECASTLE_ACK();
	void SendCS_WARP_ACK(DWORD dwObjID, BYTE bObjType, DWORD dwRange, FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ);
	void SendCS_MONSTERBUY_ACK(BYTE bResult);
	void SendCS_ITEMCHANGE_ACK(BYTE bResult, WORD wNewID=0, BYTE bNewCount=0);
	void SendCS_COUNTDOWN_ACK(DWORD dwCommand);
	void SendCS_CHANGEITEMATTR_ACK(BYTE bInven, CTItem * pItem);
	void SendCS_WASTEREFINE_ACK(BYTE bResult);
	void SendCS_CASHSHOPSTOP_ACK(BYTE bUse);	
	void SendCS_AUCTIONREG_ACK(BYTE bResult,DWORD dwAuctionID = 0);
	void SendCS_AUCTIONINTEREST_ACK(BYTE bResult,BYTE bType);
	void SendCS_AUCTIONINTERESTLIST_ACK(BYTE bResult,LPVTAUCTION vTAuction = NULL, WORD wPageNum = 0, WORD wStartNum = 0, WORD wEndNum = 0);
	void SendCS_AUCTIONBIDLIST_ACK(BYTE bResult,LPVTAUCTION vTAuction = NULL, WORD wPageNum = 0, WORD wStartNum = 0, WORD wEndNum = 0);
	void SendCS_AUCTIONREGCANCEL_ACK(BYTE bResult);
	void SendCS_AUCTIONBID_ACK(BYTE bResult);
	void SendCS_AUCTIONBUYDIRECT_ACK(BYTE bResult);
	void SendCS_AUCTIONFIND_ACK(BYTE bResult,LPVTAUCTION vTAuction = NULL, WORD wPageNum = 0, WORD wStartNum = 0, WORD wEndNum = 0);
	void SendCS_AUCTIONREGLIST_ACK(BYTE bResult,LPVTAUCTION vTAuction = NULL, WORD wPageNum = 0, WORD wStartNum = 0, WORD wEndNum = 0);
	void SendCS_KICKOUTMAP_ACK();
	void SendCS_REGGUILDCLOAK_ACK(BYTE bResult, BYTE bInven, BYTE bItemID, BYTE bReg);
	void SendCS_RESETCLOAK_ACK(DWORD dwCharID, BYTE bInven, BYTE bItemID);
	 
	void SendCS_FAMERANKLIST_ACK(BYTE bType, MONTHRANKER arFameRank[MONTHCOUNT][FAMERANKCOUNT],BYTE bMonth);
	void SendCS_UPDATEFAMERANKLIST_ACK(MONTHRANKER arFameRank[FRT_COUNT][MONTHCOUNT][FAMERANKCOUNT],BYTE bMonth);
	void SendCS_MONTHRANKLIST_ACK(BYTE bRankMonth,MONTHRANKER arMonthRank[COUNTRY_COUNT][MONTHRANKCOUNT]);
	void SendCS_FIRSTGRADEGROUP_ACK(BYTE bRankMonth,MONTHRANKER arMonthRank[COUNTRY_COUNT][FIRSTGRADEGROUPCOUNT]);
	
	void SendCS_AFTERMATH_ACK(DWORD dwCharID, BYTE bStep);
	void SendCS_CHATBAN_ACK(BYTE bRet);
	void SendCS_POSTINFO_ACK();
	void SendCS_TOURNAMENT_ACK(CPacket * pPacket, WORD wProtocol);
	void SendCS_TOURNAMENTRESULT_ACK(BYTE bStep, BYTE bRet, DWORD dwWin, DWORD dwLose, DWORD dwBlueHideTick, DWORD dwRedHideTick);
	void SendCS_TOURNAMENTENTER_ACK(CTMap * pMap);
	void SendCS_TOURNAMENTSELECTWINNER_ACK(WORD wBlueValue, WORD wRedValue);
	void SendCS_TOURNAMENTCHEER_ACK(DWORD dwCharID, DWORD dwTargetID);
	void SendCS_HELPMESSAGE_ACK(BYTE bID, CString strMessage);
	void SendCS_ITEMLEVELREVISION_ACK(BYTE bLevel);
	void SendCS_RPSSTART_ACK(BYTE bResult);
	void SendCS_RPSGAME_ACK(BYTE bWinCount, BYTE bPlayerRPS, BYTE bNpcRPS);
	void SendCS_WARCOUNTRYBALANCE_ACK(DWORD dwCountD, DWORD dwCountC);
	void SendCS_MEETINGROOM_ACK(BYTE bType, BYTE bResult, CString strName);
	void SendCS_ARENA_ACK(BYTE bResult);
	void SendCS_ARENATEAM_ACK(WORD wID, BYTE bTeam, VDWORD & vFighter);
	void SendCS_CMGIFT_ACK(BYTE bRet);

	//Apex
	void SendCS_APEXDATA_ACK(char * BufRecv, int nLen);

public:
	CTPlayer();
	virtual ~CTPlayer();
};
