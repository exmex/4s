#pragma once


class CTClientMaintain : public CTClientSkill
{
public:
	TSKILLDATA m_vTSKILLDATA;
	LPTSKILL m_pTIMMER;

	DWORD m_dwAttackID;
	DWORD m_dwHostID;
	DWORD m_dwTIMMER;
	DWORD m_dwEndTick;

	BYTE m_bUnlimitedType;
	BYTE m_bAttackType;
	BYTE m_bHostType;

public:
	DWORD GetLeftTick( DWORD dwTick);

public:
	CTClientMaintain();
	virtual ~CTClientMaintain();
};
