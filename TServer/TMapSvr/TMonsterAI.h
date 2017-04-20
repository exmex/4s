#pragma once


class CTMonsterAI
{
public:
	MAPVTAICOMMAND m_mapVCOMMAND[AT_COUNT];
	BYTE m_bAIType;

public:
	CTMonsterAI();
	virtual ~CTMonsterAI();
};
