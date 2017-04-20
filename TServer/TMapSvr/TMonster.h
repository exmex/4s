#pragma once


class CTMonster : public CTObjBase
{
public:
	CTMonSpawn *m_pSPAWN;
	LPTMONSTER m_pMON;
	LPTMONATTR m_pATTR;

	DWORD m_dwHostKEY;
	DWORD m_dwHostID;

	BYTE m_bEssential;

	BYTE m_bKeeperType;
	DWORD m_dwKeeperID;
	DWORD m_dwInvenLock;
	BYTE m_bLeaveCount;
	DWORD m_dwAtkWaitTick;

	DWORD m_dwMoney;
	CTSkill * m_pNextSkill;

	BYTE m_bCurPathID;
	BYTE m_bCurPathDir;

	FLOAT m_fStartX;
	FLOAT m_fStartY;
	FLOAT m_fStartZ;

	FLOAT m_fNextX;
	FLOAT m_fNextY;
	FLOAT m_fNextZ;

	BYTE m_bTargetType;
	DWORD m_dwTargetID;
	DWORD m_dwHelper;
	BYTE m_bHelpCount;
	WORD m_wRegenDelSpawn;

	MAPDWORD m_mapGuildDamage;  
	MAPINT64 m_mapDamage;
	MAPTAGGRO m_mapAggro;
	

	MAPTITEM m_mapTItemList;
	BYTE m_bRemove;

public:
	virtual void ResetSize(WORD wMonTempID);
	virtual void Recover(DWORD dwTick);
	virtual DWORD OnDamage(
		DWORD dwHostID,
		DWORD dwAttackID,
		BYTE bAttackType,
		BYTE bAttackCountry,
		BYTE bAttackAidCountry,
		BYTE bAtkCanSelect,
		WORD wPartyID,
		int nDamageHP,
		int nDamageMP);
	virtual BYTE GetLevel();
	virtual BYTE GetClass();
	virtual DWORD GetMaxAP(BYTE bArrow);
	virtual DWORD GetMinAP(BYTE bArrow);
	virtual DWORD GetDefendPower();
	virtual WORD GetAttackLevel();
	virtual WORD GetDefendLevel();
	virtual WORD GetMagicAtkLevel();
	virtual WORD GetMagicDefLevel();
	virtual BYTE GetCriticalPysProb();
	virtual BYTE GetCriticalMagicProb();
	virtual BYTE GetCritical(CTSkill * pSkill);
	virtual DWORD GetMaxHP();
	virtual DWORD GetMaxMP();
	virtual DWORD GetMinMagicAP();
	virtual DWORD GetMaxMagicAP();
	virtual DWORD GetMagicDefPower();
	virtual BYTE UseSkillItem(CTSkill * pSkill, BYTE bNeedAdditional);

	virtual void OnDie(
		DWORD dwAttackID,
		BYTE bObjectType, 
		WORD wTempMonID	);


	virtual void Defend(
		CTSkillTemp * pTemp,
		BYTE bSkillLevel,
		DWORD dwHostID,
		DWORD dwAttackID,
		BYTE bAttackType,
		WORD wAttackPartyID,
		BYTE bAttackCountry,
		BYTE bAttackAidCountry,
		BYTE bAttackClass,
		DWORD dwActID,
		DWORD dwAniID,
		DWORD dwPysMinPower,
		DWORD dwPysMaxPower,
		DWORD dwMgMinPower,
		DWORD dwMgMaxPower,
		WORD wTransHP,
		WORD wTransMP,
		BYTE bCurseProb,
		BYTE bEquipSpecial,
		BYTE bCanSelect,
		WORD wAttackLevel,
		BYTE bAttackerLevel,
		BYTE bCP,
		BYTE bHitType,
		DWORD dwRemainTick,
        FLOAT fAtkPosX,
		FLOAT fAtkPosY,
		FLOAT fAtkPosZ,
        FLOAT fDefPosX,
		FLOAT fDefPosY,
		FLOAT fDefPosZ,
		BYTE bMirror);

public:
	void NotifyHost(CTPlayer * pPrevHost);
	void Initialize();
	BYTE CanUseSkillNOW( CTSkill *pTSKILL);
	BYTE CanSilenceAttack(WORD wSkillID);
	void Blind();
	BYTE GetColor(DWORD dwCharID, DWORD dwPartyID, BYTE bCountry);

	void AddAggro(DWORD dwHostID, DWORD dwTarget, BYTE bTargetType, BYTE bCountry, DWORD dwAggro);
	void DelAggro(DWORD dwTarget, BYTE bTargetType);

	BYTE SetAggro(
		DWORD dwHostID,
		DWORD dwAttackID,
		BYTE bAttackType,
		BYTE bCountry,
		BYTE bClass,
		DWORD dwTarget,
		BYTE bTargetType,
		INT nAggro,
		BYTE bActive);

	BYTE LeaveAggro(DWORD dwHostID, DWORD dwRHId, BYTE bRHTType);

	DWORD FindAggro(DWORD dwID, BYTE bType);

	CTPlayer *FindHost( DWORD dwHostID);
	void SetEventToFollower();
	void MoveFollower(FLOAT fPosX, FLOAT fPosY, FLOAT fPosZ);

	void OnEvent(
		AI_TRIGGER nEvent,
		DWORD dwTriggerID,
		DWORD dwEventHost,
		DWORD dwRHId,
		BYTE bRHType);

	CTItem * SelectItem(BYTE bItemID);
	void AddItem(BYTE bCountry, WORD wAddMagicProb, INT nAddItemDrop);
	void AddItem(CTItem * pItem);
	BYTE EraseItem(BYTE bItemID);
	CTItem * GetItem(WORD wItemID);

	void AddItemList(CTItem * pItem);
	CTItem * EraseItemList(BYTE bItemID);
	
	void SetKeeper(BYTE bType, DWORD dwKeeper);
	BYTE Lottery(CTItem * pItem);
	DWORD GetExp(DWORD dwDamage);
	DWORD GetHunter();
	WORD SelectSkill(CTPlayer * pHost, BYTE bNew);
	void MakeSpecialItem(LPTMONITEM pMonItem, CTItem * pItem, WORD wAddMagicProb);
	void SetMagicOpt(CTItem * pItem, BYTE bOptType);
	void MakeSetItem(CTItem * pItem);
	BYTE MoveNext(FLOAT &fPosX, FLOAT &fPosY, FLOAT &fPosZ, BYTE &bAction, CTPlayer * pHOST);
	BYTE IsOutofRoamBound();
	FLOAT GetTargetDistance(CTObjBase * pTarget);
	FLOAT GetTargetDistance();
	BYTE OnBattleZoneEvent(DWORD dwAttackID);
	BYTE IsTransformation();
	BYTE Transformation();
	void ResetHost();
	BYTE IsInMissionMap();
	BYTE IsInPartyZone();

public:
	CTMonster();
	virtual ~CTMonster();
};
