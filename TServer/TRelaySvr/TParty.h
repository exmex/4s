#pragma once

#define		MAX_PARTY_MEMBER		7

class CTParty
{
public:
	WORD	m_wID;
	DWORD	m_dwChiefID;
	WORD	m_wCorpsID;
	TCOMMAND	m_Command;
	MAPMEMBER	m_mapMember;

public:
	CTPlayer* FindMember(DWORD dwCharID);
	void AddMember(CTPlayer* pMember);
	void DelMember(DWORD dwCharID);
	BYTE IsChief(DWORD dwID);	
	BYTE GetMemberCount();
	void SetCommand(
		WORD wSquadID,
		DWORD dwCharID,
		WORD wMapID,
		BYTE bCMD, 
		BYTE bTargetType, 
		DWORD dwTargetID, 
		WORD wPosX, 
		WORD wPosZ);
	
	void ResetCorpsID(WORD wCorpsID);

public:
	CTParty(void);
	~CTParty(void);
};
