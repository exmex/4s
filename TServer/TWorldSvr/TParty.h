#pragma once

#define		MAX_PARTY_MEMBER		7

class CTParty
{
private:
	WORD	m_wID;
	VECTORTMEMBER	m_vMember;

public:
	BYTE	m_bObtainType;
	WORD	m_wCorpsID;
	TCOMMAND m_command;
	DWORD	m_dwChiefID;
	DWORD	m_dwOrder;
	BYTE	m_bArena;
public:
	void	SetID(WORD wID);
	WORD	GetID();
	BYTE	GetSize();
	BYTE	IsMember(DWORD dwID);
	BYTE	IsMember(CString strName);
	BYTE	IsChief(DWORD dwID);
	BYTE	AddMember(LPTCHARACTER pMember);
	BYTE	DelMember(DWORD dwID);
	BYTE	DelMember(CString strName);
	void	SetChiefID(DWORD dwChiefID);
	DWORD	GetChiefID();
	BYTE	GetOrderIndex(DWORD dwID);
	void	SetNextOrder(DWORD dwID);
	LPTCHARACTER FindMember(DWORD dwCharID);
	LPTCHARACTER GetMember(int index);
	LPTCHARACTER GetNextChief(DWORD dwCharID);
    LPTCHARACTER GetChief();
	LPTCHARACTER GetNextOrder(VECTORDWORD vMember);

public:
	CTParty(void);
	~CTParty(void);
};
