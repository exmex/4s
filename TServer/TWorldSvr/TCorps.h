#pragma once

class CTCorps
{
public:
	WORD	m_wID;
	WORD	m_wCommander;
	DWORD	m_dwGeneralID;
	MAPTPARTY	m_mapParty;

public:
	void Enter(CTParty * pParty);
	BYTE Leave(CTParty * pParty);
	BYTE IsParty(WORD wPartyID);
	CTParty *FindParty(WORD wPartyID);
	CTParty *GetNextCommander(WORD wPartyID);

public:
	CTCorps(void);
	~CTCorps(void);
};
