#pragma once


class CTClientQClass
{
public:
	MAPTQUEST m_mapTQUEST;
	LPTQCLASS m_pTQCLASS;

	BYTE m_bExpend;

public:
	CTClientQuest *FindTQuest( DWORD dwQuestID);

public:
	CTClientQClass();
	virtual ~CTClientQClass();
};
