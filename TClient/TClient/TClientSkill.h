#pragma once

class CTClientSkill
{
public:
	static DWORD m_vTSKILLLEVEL[TSKILLSLOT_COUNT];										// Skill level text
	static DWORD m_vTSKILLSLOT[TSKILLSLOT_COUNT];										// Skill slot data
	static DWORD m_vTSKILLADD[TSKILLSLOT_COUNT];										// Skill buy button

	static BYTE m_bDiscountRate;

public:
	static LPTSKILL FindTIMMER( LPTSKILL pTSKILL);

	static BYTE IsAttackSKILL( LPTSKILL pTSKILL);
	static BYTE IsFlySKILL( LPTSKILL pTSKILL);

	static int CalcTSKILLDATA(
		LPTSKILL pTSKILL,
		BYTE bINDEX,
		BYTE bLevel);

public:
	DWORD GetChargeTick( BYTE bChargeSpeed);
	DWORD GetReuseTick( CTClientObjBase* pHOST);
	void RemoveTSFX();

public:
	VECTOROBJSFX m_vTSFX;
	LPTSKILL m_pTSKILL;

	DWORD m_dwExceptTick;
	DWORD m_dwReuseTick;
	DWORD m_dwTick;

	BYTE m_bTimerON;
	BYTE m_bLoopON;
	BYTE m_bLevel;

public:
	CTClientSkill();
	virtual ~CTClientSkill();
};
