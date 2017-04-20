#pragma once

class CTSkill
{
public:
	DWORD m_dwAttackID;
	BYTE m_bAttackType;
	DWORD m_dwHostID;
	BYTE m_bHostType;
	BYTE m_bHit;
	WORD m_wAttackLevel;
	BYTE m_bAttackerLevel;
	DWORD m_dwPysMinPower;
	DWORD m_dwPysMaxPower;
	DWORD m_dwMgMinPower;
	DWORD m_dwMgMaxPower;
	BYTE m_bCanSelect;
	BYTE m_bAttackCountry;
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;

	WORD m_wSilencedSkill;
	DWORD m_dwChargeTick;	//스킬사용시 시전시작 tick, 버프시 버프 걸린 tick
	DWORD m_dwDelayTick;
	DWORD m_dwUseTick;
	DWORD m_dwStartTick;
	DWORD m_dwMaintainTick;
	BYTE m_bLevel;
	CTSkillTemp * m_pTSKILL;

public:
	BYTE GetNextSkillPoint();
	WORD GetUsedSkillPoint();
	BYTE CheckParentSkill(BYTE bParentLevel);

	void Use(BYTE bType, DWORD dwTick, DWORD dwAttackSpeed, DWORD dwRate);
	DWORD GetReuseDelay(BYTE bType, DWORD dwAttackSpeed, DWORD dwRate);
	DWORD GetReuseRemainTick(DWORD dwTick);
	int CalcAbilityValue(BYTE bAction, DWORD dwValue, BYTE bExec);
	int CalcCure(CTSkill * pSkill, DWORD dwValue, BYTE bExec);
	DWORD GetRequiredHP(DWORD dwMaxHP);
	DWORD GetRequiredMP(DWORD dwMaxMP);
	BYTE GetNextLevel();
	DWORD GetValue(LPTSKILLDATA pData);
	DWORD GetValue(BYTE bType, BYTE bExec);
	BYTE GetProb(LPTSKILLDATA pData);
	BYTE CanUse(DWORD dwTick);
	WORD GetID();
	void SetEndTick(DWORD dwTick);
	void SetLoopEndTick(DWORD dwTick, DWORD dwRemain);
	BYTE IsEnd(DWORD dwTick);
	DWORD GetMaintainTick();
	DWORD GetBlood(DWORD dwDamage, BYTE bExec);
	DWORD GetAggro();
	BYTE GetCountMultiAttack();
	DWORD GetRemainTick(DWORD dwTick);

	void SetMaintain(
		DWORD dwAtkID,
		BYTE bAtkType,
		DWORD dwHostID,
		BYTE bHostType,
		BYTE bHit,
		WORD wAtkLevel,
		BYTE bAttackerLevel,
        DWORD dwPsyMinPower,
        DWORD dwPsyMaxPower,
		DWORD dwMgMinPower,
		DWORD dwMgMaxPower,
		BYTE bCanSelect,
		BYTE bAttackCountry,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	CTSkill();
	virtual ~CTSkill();
};
