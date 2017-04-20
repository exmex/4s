#pragma once


class CTClientItem
{
public:
	static MAPTQUESTCLNTITEM m_mapTQuestMagicClientItem;
	static CTClientItem* FindTQuestMagicClientItem( WORD wRewardID );

	static MAPDWORD m_mapTITEMTICK;		// Item reuse time data
	static MAPDWORD m_mapTITEMLOCK;		// Item lock data
	static DWORD m_dwEQUIPCOLOR;
	static DWORD m_dwNORMALCOLOR;

public:
	static DWORD GetTick( WORD wDelayGroupID);
	static void SetTick(WORD wDelayGroupID, DWORD dwTick );
	static void ClearTick();

	static BYTE IsLocked( WORD wDelayGroupID);
	static void Unlock( WORD wDelayGroupID);
	static void Lock( WORD wDelayGroupID);

	static VECTORBYTE GetEquipSlotList(LPTITEM pTITEM);

	static LPTITEMVISUAL GetDefaultVisual( LPTITEM pTITEM);

	static DWORD CalcPrice(LPTITEM pTITEM, WORD wAttrLevel);
	static WORD CalcViewAttrLevel(WORD wAttrLevel);

protected:
	LPTITEM			m_pTITEM;
	BYTE			m_bItemID;
	
	WORD			m_wItemID;
	BYTE			m_bCount;

	DWORD			m_dwDuraMax;
	DWORD			m_dwDuraCurrent;
	BYTE 			m_bRefineMax;
	BYTE 			m_bRefineCurrent;
	BYTE 			m_bCanGamble;
	__time64_t	m_dEndTime;

	MAPTMAGIC		m_mapTMAGIC;
	BYTE			m_bGrade;
	BYTE			m_bGradeEffect;
	BYTE			m_bInstGradeMax; //일시적인 m_bGrade의 상한값 - CS_ITEMLEVELREVISION_ACK에 의해 컨트롤
	BYTE			m_bWrap;
	BYTE			m_bELD;
	WORD		m_wColor;
	BYTE			m_bRegGuild;

	BOOL			m_bNeedUpdate;
	BYTE			m_bMagicGrade;		///< 매직 옵션 등급
	LPTITEMMAGIC	m_pTBestOption;		///< 최공 성능의 옵션

	WORD			m_wAttrID;			///< 이 아이템의 능력치 아이디
	WORD			m_wAttrLevel;		///< 실제 성능 레벨
	WORD			m_wViewAttrLevel;	///< 표시되는 성능 레벨
	DWORD			m_dwPrice;			///< 성능 가격

	VTMAGICOPTION m_vMagicInfos;		///< 매직옵션 정보 리스트

	BOOL	 		m_bApplyAP;			///< 매직성능이 '물리공격력'에 적용되었는지 여부
	BOOL	 		m_bApplyMAP;		///< 매직성능이 '마법공격력'에 적용되었는지 여부
	BOOL	 		m_bApplyDP;			///< 매직성능이 '물리방어력'에 적용되었는지 여부
	BOOL	 		m_bApplyMDP;		///< 매직성능이 '마법방어력'에 적용되었는지 여부

	FLOAT			m_fOptionLv;		///< 보조 옵션 레벨

	WORD	 		m_wApplyMagicID;	///< 성능레벨에 사용된 옵션의 매직 아이디
	TITEMATTR		m_itemAttr;			///< 향상된 아이템 성능

	BOOL			m_bHasMainOption;	///< 메인옵션을 가지는지 여부

public:
	CTClientItem& operator = ( CTClientItem& param);

	BYTE operator == ( CTClientItem& param);
	BYTE operator != ( CTClientItem& param);

public:
	void NotifyUpdate();
	void Update();
	
	WORD GetMagicValue(BYTE bMagicID);
	void ClearMagic();
	void AddMagicValue(BYTE bMagicID, WORD wValue);

	BOOL IsEmptyMagicInfos();
	LPVTMAGICOPTION GetMagicInfos();

	DWORD GetQuality() const;
	DWORD GetQualityColor() const;
	DWORD GetMagicColor() const;

	VECTORDWORD GetSFX();
	DWORD GetGradeSFX();
	DWORD GetMagicSFX();

	LPTITEMGRADEVISUAL GetGradeVISUAL();
	LPTITEMVISUAL GetDefaultVisual();
	LPTITEMVISUAL GetVisual();
	BYTE GetVisualGrade();
	DWORD GetVisualColor();

	void SetItemSlot(CPacket* pPacket);
	void SetItemData(CPacket* pPacket, BOOL bUseEndTime=TRUE);
	void SetItemMagic(CPacket* pPacket);

	BOOL CheckValid();

protected:
	void InitAttrVar();
	void CalcAttrID();
	void CalcAttrID(const TITEMATTR& itemAttr);

	BYTE GetWeaponPowerLevel();
	BYTE GetShieldPowerLevel();
	BYTE GetPowerLevel();
	void CalcAttrNameAndLevel();

public:
	void SetTITEM(LPTITEM p);				
	void SetItemSlot(BYTE p);				
	void SetItemID(WORD p);				
	void SetCount(BYTE p);					
	void SetDuraMax(DWORD p);				
	void SetDuraCurrent(DWORD p);			
	void SetRefineMax(BYTE p);				
	void SetRefineCurrent(BYTE p);			
	void SetCanGamble(BYTE p);				
	void SetGrade(BYTE p);					
	void SetGradeEffect(BYTE p);
	void SetEndTime( __time64_t);
	void SetWrap( BYTE bWrap );
	void SetELD( BYTE bELD );
	void SetColor( WORD bColor );
	void SetRegGuild( BYTE bRegGuild );
	void SetInstGradeMax(BYTE bMax);

public:
	LPTITEM GetTITEM()					{ return m_pTITEM; }
	BYTE GetItemSlot()					{ return m_bItemID; }
	WORD GetItemID()					{ return m_wItemID; }
	BYTE GetCount()						{ return m_bCount; }
	DWORD GetDuraMax()					{ return m_dwDuraMax; }
	DWORD GetDuraCurrent()				{ return m_dwDuraCurrent; }
	BYTE GetRefineMax()					{ return m_bRefineMax; }
	BYTE GetRefineCurrent()				{ return m_bRefineCurrent; }
	BYTE CanGamble()					{ return m_bCanGamble; }
	LPMAPTMAGIC GetTMAGIC()				{ return &m_mapTMAGIC; }
	__time64_t GetEndTime()			{ return m_dEndTime; }
	BYTE GetWrap()						{ return m_bWrap; }
	BYTE GetELD()						{ return m_bELD; }
	WORD GetColor()					{ return m_wColor; }
	BYTE GetRegGuild()				{ return m_bRegGuild; }

	BOOL HasMainMagicOption()			{ Update(); return m_bHasMainOption; }
	WORD GetAttrID()					{ Update(); return m_wAttrID; }
	WORD GetAttrLevel()					{ Update(); return m_wAttrLevel; }
	WORD GetViewAttrLevel()				{ Update(); return m_wViewAttrLevel; }
	DWORD GetPrice()					{ Update(); return m_dwPrice; }
	DWORD GetSellPrice()				{ Update(); return m_dwPrice / 4; }
	BOOL GetApplyAP()					{ Update(); return m_bApplyAP; }
	BOOL GetApplyMAP()					{ Update(); return m_bApplyMAP; }	
	BOOL GetApplyDP()					{ Update(); return m_bApplyDP; }	
	BOOL GetApplyMDP()					{ Update(); return m_bApplyMDP; }
	FLOAT GetOptionLv()					{ Update(); return m_fOptionLv; }
	WORD GetApplyMagicID()				{ Update(); return m_wApplyMagicID; }
	LPTITEMATTR GetItemAttr()			{ Update(); return &m_itemAttr; }
	BYTE GetGrade() const
	{
		if( !m_bInstGradeMax || m_bGrade <= m_bInstGradeMax )
			return m_bGrade;
		return m_bInstGradeMax;
	}

public:
	CTClientItem();
	virtual ~CTClientItem();

};

