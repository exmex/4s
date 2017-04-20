#pragma once


class CTObjBase
{
public:
	MAPTINVEN m_mapTINVEN;
	MAPTSKILL m_mapTSKILL;
	VTSKILL m_vMaintainSkill;
	VTSKILL m_vRemainSkill;

	CTMap *m_pMAP;
	DWORD m_dwID;

	LPTSTAT m_pTCLASS;
	LPTSTAT m_pTRACE;

	LPTLEVEL m_pTLEVEL;
	LPTLEVEL m_pTPREVLEVEL;
	LPTLOCAL m_pLocal;

	DWORD m_dwHP;
	DWORD m_dwMP;
	DWORD m_dwRegion;

	DWORD m_dwRecoverHPTick;
	DWORD m_dwRecoverMPTick;
	DWORD m_dwLastAtkTick;

	BYTE m_bChannel;
	BYTE m_bAction;
	BYTE m_bMode;
	BYTE m_bType;
	BYTE m_bBlock;
	BYTE m_bCanAttack;
	BYTE m_bExit;

	BYTE m_bLevel;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
	FLOAT m_fSize;

	WORD m_wCurChargeSkill;
	WORD m_wTemptedMon;
	WORD m_wMapID;
	WORD m_wPitch;
	WORD m_wDIR;
	WORD m_wLocalID;

	BYTE m_bMouseDIR;
	BYTE m_bKeyDIR;

 	WORD m_wPartyID;
	BYTE m_bPartyType;

	BYTE m_bCountry;
	BYTE m_bAidCountry;
	BYTE m_bOriCountry;
	BYTE m_bGetaway;
	INT64 m_dlAidDate;

	DWORD m_dwGuildID;
	CString m_strGuildName;
	BYTE m_bGuildDuty;
	BYTE m_bGuildPeer;

	DWORD m_dwTacticsID;
	CString m_strTacticsName;

	DWORD m_dwFame;
	DWORD m_dwFameColor;

	BYTE m_bStatus;
	BYTE m_bEquipSpecial;
	BYTE m_bIsSpecial;
	TAFTERMATH m_aftermath;

	CTSkill * m_pInstanceSkill;

public:
	BYTE CanFight(BYTE bType, BYTE bCountry, BYTE bAidCountry);
	BYTE GetWarCountry();
	DWORD GetGuild();

	CTInven *FindTInven( BYTE bInvenID);
	CTSkill * FindTSkill(WORD wSkill);
	CTSkill * FindMaintainSkill(WORD wSkill);
	CTSkill * FindRemainSkill(WORD wSkill);

	BYTE UpdateSkill(
		CTSkillTemp * pTemp,
		WORD wSkillID,
		BYTE bLevel);

	BYTE PerformSkill(
		DWORD dwAttack,
		BYTE bAtkerType,
		BYTE bAttackCountry,
		BYTE bAttackAidCountry,
		BYTE bAttackClass,
		DWORD dwHostID,
		CTSkillTemp * pTemp,
		BYTE bSkillLevel,
		BYTE bHit,
		DWORD dwPysMinPower,
		DWORD dwPysMaxPower,
		DWORD dwMgMinPower,
		DWORD dwMgMaxPower,
		WORD wTransHP,
		WORD wTransMP,
		BYTE bCurseProb,
		WORD wAtkLevel,
		BYTE bAttackerLevel,
		int nDamageHP,
		int nDamageMP,
		FLOAT fAtkPosX,
		FLOAT fAtkPosY,
		FLOAT fAtkPosZ,
		CTRecallMon ** pRecallMon);

	CTSkill * MaintainSkill(
		CTSkill * pTemp,
        DWORD dwRemainTick,
        DWORD dwHostID,
        BYTE bHostType,
		DWORD dwAttackID,
		BYTE bAttackType,
		DWORD dwPysMinPower,
		DWORD dwPysMaxPower,
		DWORD dwMgMinPower,
		DWORD dwMgMaxPower,
		BYTE bCurseProb,
		BYTE bCanSelect,
		BYTE bAttackCountry,
		BYTE bHit,
        WORD wAttackLevel,
		BYTE bAttackerLevel,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	CTSkill * RemainSkill(CTSkill * pSkill, DWORD dwAtkID);

	void ClearInven();
	void ClearSkill();
	void ClearMaintainSkill();
	void EraseMaintainSkill(WORD wID);
	void EraseMaintainSkill(CTSkill * pSkill, DWORD index);
	void EraseHideBuff();
	void EraseOtherSelf();
	void EraseItemUpgradeBuff();
	void CancelBlockBuff();

	BYTE CheckPrevAct(WORD wActID);
	void CheckMaintainSkill(DWORD dwTick);
	BYTE UpdateBuffSkill(CTSkill * pTemp, DWORD dwAttacker, BYTE bAttackType);
	BYTE HaveDisWeapon();
	BYTE HaveDisDefend();
	BYTE HaveMirror();
	BYTE HaveStopRecover();
	BYTE HaveMask();
	void EraseStandHideBuff();
	WORD HaveItemProbBuff(BYTE bType);  
	void EraseItemProbBuff(BYTE bType);  
	INT HaveItemDropBuff(BYTE bItemProb);
	WORD GetMagicDropBuff();
	WORD GetGainExpBuff();

	///////////////////////////////////////////////////
	// ∫Ø¿Â
	BYTE HaveDisguiseBuff();
	BYTE GetCountry();
	///////////////////////////////////////////////////
	BYTE IsTrans();
	BYTE IsActionBlock();

	int ApplyEffectionBuff(DWORD &dwValue);
	DWORD DistributeSkill(DWORD dwDamage);
	BYTE GetAtkHitType(
		CTSkillTemp * pSkill,
		BYTE bCR,
		WORD bAL,
		BYTE bSkillLevel,
		BYTE bAttackerLevel,
		BYTE bEquipSpecial);

	BYTE SetTransAttack(WORD wMonID);
	BYTE ForceMaintain(
		WORD wSkillID,
		DWORD dwHostID,
		BYTE bHostType,
		DWORD dwAttack,
		BYTE bAtkType,
		DWORD dwRemainTick);

	BYTE IsEquipSpecial();
	BYTE IsEquipSkillItem(CTSkill * pSkill);
	BYTE HaveExcptPhysic();
	BYTE HaveExcptMagic();
	void SkillUse(CTSkill * pSkill,  DWORD dwTick);
	void CancelSkill(WORD wSkillID, DWORD dwTick);

	DWORD GetFormulaInit(BYTE bFormula);
	FLOAT GetFormulaRateX(BYTE bFormula);
	FLOAT GetFormulaRateY(BYTE bFormula);
	
	void CalcAfterMath(FLOAT &fValue);
	BYTE PushMaintainSkill(CTSkill * pSkill);
	void EraseBuffByDefend(CTSkillTemp * pCurSkill);
	void EraseBuffByAttack(CTSkillTemp * pCurSkill);
	void EraseBuffByRide();
	BYTE DeleteSkillItem(WORD wItemID);
	void CheckEternalBuff();
	void EraseDieSkillBuff();
	BYTE HaveDieBuff();
	BYTE CheckAttack();
	void DeletePositiveMaintainSkill();
	void DeleteNegativeMaintainSkill();
	BYTE IsInCastle();
	WORD MakeItemMagicValue(INT nBase, WORD wBound);
	void ReleaseMaintain(BYTE bNotify = FALSE);
	BYTE GetMonExpRate();

public:
	virtual void ResetSize(WORD wMonTempID);
	virtual void Recover(DWORD dwTick);
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

	virtual WORD GetPartyID();
	virtual BYTE GetLevel();
	virtual BYTE GetClass();
	virtual void OnDie(
		DWORD dwAttackID, 
		BYTE bObjectType,
		WORD wTempMonID);

	virtual BYTE CanEquip(
		CTItem *pTItem,
		BYTE bItemID);

	virtual BYTE CanPush(
		LPVTITEM pTITEM,
		BYTE bIndex);

	virtual BYTE CanPush(
		CTItem *pTItem,
		BYTE bInven,
		BYTE bSlot);

	virtual void CalcDamage(
		DWORD dwAttackID,
		BYTE bAttackType,
		MAPWORD & mapDamage,
		int & nDamageHP,
		int & nDamageMP,
		DWORD dwPysMinPower,
		DWORD dwPysMaxPower,
		DWORD dwMgMinPower,
		DWORD dwMgMaxPower,
		BYTE & bHit,
		CTSkill * pSkill,
		BYTE bDamageType);

	virtual void Defend(
		CTSkillTemp * pTemp,
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

	virtual void ChgMode(
		BYTE bMode);

	virtual int CalcAbilityValue(DWORD &dwValue, BYTE bExec, CTSkill * pInstanceSkill = NULL);
	virtual FLOAT GetBaseStatValue(BYTE bType, WORD wBaseMIN);
	virtual DWORD Calc2ndAbility(BYTE bFormula);
	virtual DWORD GetAbility(BYTE bType);
	virtual int CalcItemAbility(BYTE bType);
	virtual FLOAT GetSTR(BYTE bPure = FALSE, WORD wBaseMIN = 0);
	virtual FLOAT GetDEX(BYTE bPure = FALSE, WORD wBaseMIN = 0);
	virtual FLOAT GetCON(BYTE bPure = FALSE, WORD wBaseMIN = 0);
	virtual FLOAT GetINT(BYTE bPure = FALSE, WORD wBaseMIN = 0);
	virtual FLOAT GetWIS(BYTE bPure = FALSE, WORD wBaseMIN = 0);
	virtual FLOAT GetMEN(BYTE bPure = FALSE, WORD wBaseMIN = 0);

	virtual DWORD GetPureMaxHP();
	virtual DWORD GetPureMaxMP();
	virtual DWORD GetAtkSpeed(BYTE bType);
	virtual DWORD GetAtkSpeedRate(BYTE bType);
	virtual DWORD GetMaxHP();
	virtual DWORD GetMaxMP();
	virtual DWORD GetDefendPower();
	virtual WORD GetAttackLevel();
	virtual WORD GetDefendLevel();
	virtual WORD GetMagicAtkLevel();
	virtual WORD GetMagicDefLevel();
	virtual BYTE GetCritical(CTSkill * pSkill);
	virtual DWORD GetMaxAP(BYTE bArrow);
	virtual DWORD GetMinAP(BYTE bArrow);
	virtual BYTE GetCriticalPysProb();
	virtual BYTE GetCriticalMagicProb();
	virtual WORD GetPysResist();
	virtual DWORD GetMinMagicAP();
	virtual DWORD GetMaxMagicAP();
	virtual DWORD GetMagicDefPower();
	virtual BYTE GetChargeSpeed();
	virtual BYTE GetChargeProb();
	virtual WORD GetMagicResist();
	virtual DWORD GetShieldDP();
	virtual DWORD GetShieldMDP();
	virtual DWORD GetAvoidProb();
	virtual WORD GetFR();
	virtual WORD GetIR();
	virtual WORD GetER();
	virtual	WORD GetSR();
	virtual WORD GetDR();
	virtual DWORD GetHPR();
	virtual DWORD GetMPR();
	virtual BYTE UseSkillItem(CTSkill * pSkill, BYTE bNeedAddition);

public:
	CTObjBase();
	virtual ~CTObjBase();
};
