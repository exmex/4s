#pragma once

class CTNpc
{
public:
	CTNpc(void);
	~CTNpc(void);

	WORD GetSpawnPos();
	LPTITEM GetItem(WORD wID);
	CTItem* GetMagicItem(DWORD dwID);
	CTSkillTemp * GetSkill(WORD wSkillID);
	LPTPORTAL GetPortal();
	BYTE CanTalk(BYTE bCountry, BYTE bAidCountry, BYTE bDisguise);
	LPTMONSTERSHOP GetMon(WORD wID);
	BYTE AddAuction(DWORD dwAuctionID, LPTAUCTION pAuction);
	void DelAuction(DWORD dwAuctionID,DWORD dwCode);
	BYTE AuctionCondition(LPTAUCTION pAuction,BYTE bMinWearLv,BYTE bMaxWearLv,BYTE bMinPowerLv,BYTE bMaxPowerLv,BYTE bOption,WORD wItemID = 0);	
	void SearchAuctions(LPMAPDWORD pmapDWORD,DWORD dwCode,BYTE bMinWearLv,BYTE bMaxWearLv,BYTE bMinPowerLv,BYTE bMaxPowerLv,BYTE bOption,WORD wItemID = 0);
	void SearchAuctionAll(LPVTAUCTION pVTAUCTION,BYTE bMinWearLv,BYTE bMaxWearLv,BYTE bMinPowerLv,BYTE bMaxPowerLv,BYTE bOption,WORD wItemID = 0);
	void AlignAuction(LPVTAUCTION pVTAUCTION,BYTE bAlignKind);

public:
	WORD m_wID;
	BYTE m_bType;
	BYTE m_bCountry;
	LPTLOCAL m_pLocal;
	WORD m_wSpawnPosID;
	LPTPORTAL m_pPortal;
	LPTARENA m_pArena;
	BYTE m_bDiscountCondition; 
	BYTE m_bDiscountRate;  
	BYTE m_bAddProb;
	WORD m_wItemID;
	MAPTITEMTEMP m_mapItem;
	MAPTSKILLTEMP m_mapSkill;
	MAPQUESTTEMP m_mapQuest;
	MAPTMONSTERSHOP m_mapMon;
	MAPTAUCTION m_mapTAuction;
	MAPTAUCTION m_mapTAuctionItemID;
	MAPMAPTAUCTION m_mapTAuctionFind;
	MAPTSTORAGEITEM m_mapMagicItem;
	WORD m_wMapID;  
	FLOAT m_fPosX;
	FLOAT m_fPosY;
	FLOAT m_fPosZ;
};
