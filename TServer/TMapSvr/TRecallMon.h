#pragma once
#include "tmonster.h"

class CTRecallMon :
	public CTMonster
{
public:
	CString m_strName;
	WORD m_wPetID;
	BYTE m_bMain;
	DWORD m_dwRecallTick;
	DWORD m_dwDurationTick;
	DWORD m_dwDieTick;
	BYTE m_bHit;

	DWORD m_dwMaxHP;		//ºÐ½Å
	DWORD m_dwMaxMP;
	BYTE m_bAtkLevel;
	BYTE m_bAtkSkillLevel;

public:
	CTRecallMon(void);
	~CTRecallMon(void);

	DWORD GetLifeTick(DWORD dwTick);

	virtual DWORD GetMaxHP();
	virtual DWORD GetMaxMP();
	virtual DWORD GetMaxAP(BYTE bArrow);
	virtual DWORD GetMinAP(BYTE bArrow);
	virtual DWORD GetDefendPower();
	virtual WORD GetAttackLevel();
	virtual WORD GetDefendLevel();
	virtual WORD GetMagicAtkLevel();
	virtual WORD GetMagicDefLevel();
	virtual BYTE GetCriticalPysProb();
	virtual BYTE GetCriticalMagicProb();
	virtual DWORD GetMinMagicAP();
	virtual DWORD GetMaxMagicAP();
	virtual DWORD GetMagicDefPower();
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

	virtual void OnDie(
		DWORD dwAttackID,
		BYTE bObjectType, 
		WORD wTempMonID);
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
};
