#pragma once

class CTSkillTemp
{
public:
	WORD m_wID;
	BYTE m_bPositive;
	BYTE m_bPriority;

	WORD m_wPrevActiveID;
	WORD m_wParentSkillID;
	WORD m_wPosture;

	WORD m_wMaxRange;
	WORD m_wMinRange;
	WORD m_wTargetActiveID;
	WORD m_wUseItem;

	DWORD m_dwConditionID;
	DWORD m_dwClassID;
	BYTE m_bKind;

	DWORD m_dwActionTime;
	DWORD m_dwDuration;
	DWORD m_dwDurationInc;
	DWORD m_dwReuseDelay;
	DWORD m_dwLoopDelay;
	DWORD m_dwKindDelay;
	DWORD m_dwWeapon;
	DWORD m_dwUseMP;
	BYTE m_bUseMPType;
	DWORD m_dwUseHP;
	BYTE m_bUseHPType;
	DWORD m_dwAggro;
	int m_nReuseDelayInc;

	BYTE m_bStartLevel;
	BYTE m_bMaxLevel;
	BYTE m_bNextLevel;

	BYTE m_bTarget;
	BYTE m_bTargetRange;
	BYTE m_bIsuse;
	BYTE m_bSpeedApply;
	BYTE m_bTargetHit;
	BYTE m_bMaintainType;

	BYTE m_bType;
	FLOAT m_fPrice;

	BYTE m_bCanLearn;
	BYTE m_bORadius;
	BYTE m_bIsRide;
	BYTE m_bIsDismount;
	BYTE m_bDuraSlot;
	BYTE m_bCanCancel;
	BYTE m_bHitTest;
	BYTE m_bHitInit;
	BYTE m_bHitInc;
	BYTE m_bGlobal;
	BYTE m_bRadius;
	BYTE m_bStatic;
	BYTE m_bEraseAct;
	BYTE m_bEraseHide;
	BYTE m_bIsHideSkill;
	BYTE m_bRunFromServer;
	BYTE m_bCheckAttacker;
	WORD m_wTriggerID;
	WORD m_wMapID;

	FLOAT m_f1stRateX;
	
	VTSKILLDATA m_vData;
	MAPTSKILLPOINT m_mapTSkillPoint;

public:
	DWORD GetAggro(BYTE bLevel);
	BYTE GetNeedSkillPoint(BYTE bLevel);
	BYTE GetNeedKindPoint(BYTE bLevel);
	BYTE CheckParentSkill(BYTE bLearnLevel, BYTE bParentLevel);

	int CalcValue(BYTE bLevel, BYTE bType, BYTE bExec, DWORD dwValue);
	int Calculate(BYTE bLevel, DWORD index, DWORD dwValue);
	BYTE IsDamageType();
	BYTE GetAttackType();
	BYTE HaveSkillData(LPTSKILLDATA pData);
	int GetValue(LPTSKILLDATA pData, BYTE bLevel);
	WORD GetAutoExp();

	DWORD GetPrice(DWORD dwMoney);
	BYTE CanEquip(BYTE bKind);
	WORD GetID();
	BYTE CheckCountry(BYTE bCountry);
	BYTE IsMaintainType();
	BYTE IsBuffType();
	BYTE IsDotType();
	BYTE IsLongAttack();
	BYTE IsPositive();
	BYTE IsNegative();
	BYTE IsRemainType();
	BYTE IsPassiveType();
	BYTE IsRevivalType();
	BYTE IsBlockType();
	BYTE IsPosture();
	BYTE IsHideBuff();
	BYTE IsCancelMine();
	BYTE IsSilence();
	BYTE IsTrans();
	BYTE IsRandomTrans();
	BYTE IsItemDelTarget();
	BYTE IsItemDelUser();
	BYTE IsDie();
	BYTE GetTransHPMPType();
	BYTE IsMirrorType();
	BYTE IsMultiAttack();
	BYTE IsAfterMath();
	WORD GetItemProbBuff(BYTE bType);
	BYTE IsRandomBuff();  
	BYTE CanDefendAtDie();
	BYTE IsMonRecall();
	BYTE IsLuckyPotion();
	BYTE IsExpPotion();
	BYTE IsRecall();

	CTSkillTemp(void);
	~CTSkillTemp(void);
};
