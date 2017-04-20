#pragma once


class CTItem
{
public:
	MAPTMAGIC m_mapTMAGIC;

	LPTITEM m_pTITEM;
	LPTITEMATTR m_pTITEMATTR;

	__int64 m_dlID;
	BYTE m_bItemID;
	BYTE m_bLevel;
	BYTE m_bInven;	//사용자간 아이템 거래에서만 값 설정하여 사용중.
	DWORD m_dwOwnerID;	//퀘스트 아이템 주인설정

	WORD m_wItemID;
	BYTE m_bCount;
	BYTE m_bGLevel;
	DWORD m_dwDuraMax;
	DWORD m_dwDuraCur;
	BYTE m_bRefineCur;
	__time64_t m_dEndTime;
	__time64_t m_dAlarmTime;

	BYTE m_bStType;
	DWORD m_dwStItemID;

	BYTE m_bMaxRouting;
	MAPDWORD m_mapRoutingJoiner;
	BYTE m_bGradeEffect;
	BYTE m_bMagicDropLevel;
	DWORD m_dwExtValue[TMAGIC_MAX];
	DWORD m_dwMoney;

public:
	void Catalyzer(CTPlayer * pPlayer);
	DWORD RandMoney();
	BYTE SetGLevel(BYTE bLevel);
	void SetDuration(BYTE bMon);
	WORD GetMagicValue( BYTE bMagicID);
	WORD GetMagicValue( LPTMAGIC pTMAGIC);
	void Copy(CTItem * pItem, BYTE bGenID);
	void WrapPacket(CPacket * pPacket);
	void WrapPacketClient(CPacket * pPacket, DWORD dwCharID, BYTE bAddItemID = TRUE, WORD wUseTime = 0, BYTE bCashItem = FALSE);
	BYTE GetWeaponPowerLevel();
	BYTE GetShieldPowerLevel();
	BYTE GetPowerLevel();
	DWORD GetMaxAP();
	DWORD GetMinAP();
	DWORD GetMaxMagicAP();
	DWORD GetMinMagicAP();
	DWORD GetMaxLAP();
	DWORD GetMinLAP();
	DWORD GetDefendPower();
	DWORD GetMagicDefPower();
	DWORD GetRepairCost();
	DWORD DurationDec(BYTE bDel);
	BYTE CanUse();
	BYTE HavePower();
	DWORD GetPrice();
	DWORD GetPvPoint();
	void SetEndTime(DWORD dwUseTime = 0);
	WORD GetAutoSkill();
	BYTE GetEquipLevel();
	void SetGradeEffect(BYTE bEffect);
	void SetRevision(LPTMAGIC pTMAGIC);
	BYTE CanDeal();

public:

	BYTE operator == ( CTItem& param);
	BYTE operator != ( CTItem& param);

public:
	CTItem();
	virtual ~CTItem();
};
