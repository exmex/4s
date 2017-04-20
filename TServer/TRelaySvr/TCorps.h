#pragma once

class CTCorps
{
public:
	WORD	m_wID;
	WORD	m_wCommander;
	DWORD	m_dwGeneralID;
	MAPTPARTY m_mapTParty;

public:	
	BYTE GetMemberCount();
	CTParty* FindMember(WORD wPartyID);
	void DelMember(WORD wPartyID);
	void AddMember(CTParty* pParty);
	
public:
	CTCorps(void);
	~CTCorps(void);
};
