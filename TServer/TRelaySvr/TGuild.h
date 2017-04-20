#pragma once

class CTGuild
{
public:
	DWORD m_dwID;
	DWORD m_dwChiefID;
	MAPMEMBER	m_mapMember;
	MAPMEMBER	m_mapTactics;

public:
	CTPlayer* FindMember(DWORD dwID);
	void AddMember(CTPlayer* pPlayer);
	void DelMember(DWORD dwID);
	BYTE IsEmpty();

	CTPlayer * FindTactics(DWORD dwCharID);
	void AddTactics(CTPlayer * pPlayer);
	void DelTactics(DWORD dwCharID);

public:
	CTGuild(void);
	~CTGuild(void);
};
