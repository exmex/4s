#pragma once
#include "trecallmon.h"

class CTSelfObj :
	public CTRecallMon
{

public:
	DWORD m_dwPysMinPower;
	DWORD m_dwPysMaxPower;
	DWORD m_dwMgMinPower;
	DWORD m_dwMgMaxPower;
	WORD m_wAtkAL;

public:
	CTSelfObj(void);
	~CTSelfObj(void);

	virtual void Recover(DWORD dwTick);
	virtual void OnDie(
		DWORD dwAttackID,
		BYTE bObjectType, 
		WORD wTempMonID);
};
