#pragma once

class CTChatItem
{
public:
	static MAPTITEMTEMP m_mapTITEMTEMP;	// Item template data

public:
	static LPTITEM FindTItem( WORD wItemID);

public:
	BYTE m_bNetID;
	BYTE m_bGrade;

	LPTITEM m_pTITEM;
	BYTE m_bItemID;

	WORD m_wItemID;
	BYTE m_bCount;

	DWORD m_dwRune;
	DWORD m_dwLuna;
	DWORD m_dwCron;

public:
	CTChatItem();
	virtual ~CTChatItem();

	void SetItemID(WORD p);				
	void ClearMagic();
	void SetGrade(BYTE p);					
	void SetDuraMax(DWORD p);				
	void SetDuraCurrent(DWORD p);			
	void SetRefineCurrent(BYTE p);			
	void SetRefineMax(BYTE p);				
	void SetCanGamble(BYTE p);				
	void AddMagicValue(BYTE bMagicID, WORD wValue, DWORD dwTime=0);
};
