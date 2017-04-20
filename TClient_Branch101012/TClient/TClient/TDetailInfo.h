#pragma once

#define TDETINFO_OUTLOOK_STYLE_TITLE			1
#define TDETINFO_OUTLOOK_STYLE_UNTITLE		2

/// 상세정보 타입 열거
enum TDETINFO_TYPE
{
	TDETINFO_TYPE_NOR,			///< 타이틀과 설명만 있는 간단한 상세 정보
	TDETINFO_TYPE_ITEM,			///< 아이템 설명 타입의 상세 정보
	TDEFINFO_TYPE_INSTITEM,
	TDETINFO_TYPE_SEALEDITEM,	///< 봉인 아이템 설명 타입의 상세 정보
	TDETINFO_TYPE_OPTIONITEM,	///< 옵션 아이템 설명 타입의 상세 정보
	TDETINFO_TYPE_SKILL,		///< 스킬 설명 타입의 상세 정보
	TDETINFO_TYPE_CASTLE,	///< 성 설명 타입의 상세 정보
	TDETINFO_TYPE_TERRITORY, ///< 지역 설명 타입의 상세 정보
	TDETINFO_TYPE_STAT, ///< 스탯 정보
	TDETINFO_TYPE_PVP, ///< PVP상태 정보
	TDETINFO_TYPE_TEXT,
	TDEFINFO_TYPE_DEFTOOLTIP,	///< 설명만 있는 간단한 상세 정보
	TDETINFO_TYPE_FAMERANK,	///< 명예의 전당 정보
	TDETINFO_TYPE_TOURNAMENT_PLAYER,
	TDETINFO_TYPE_MISSION,
	TDETINFO_TYPE_COUNT,
};

#define TDEFAULT_TEXT_COLOR		D3DCOLOR_ARGB(255,249,233,202)
#define TNEXTLEV_TEXT_COLOR		D3DCOLOR_ARGB(255,176,174,253)

// =====================================================================
/**	@class		ITDetailInfo
	@brief		상세정보의 전달과 표시를 위한 기본 인터페이스.
	
*/// ===================================================================
class ITDetailInfo
{
public:
	/// _GetLineState() 의 반환값으로 사용.
	enum LINE_STATE
	{
		LS_VALID,		///< 아무 문제 없음
		LS_INVALID,		///< 이 라인은 적용할 수 없음
		LS_NEEDCONV,	///< 이 라인은 변환이 필요함
		LS_TITLE,		///< 타이틀용 라인임
		LS_COUNT
	};

public:
	/// 상세정보의 한 줄을 표시하기위한 정보.
	struct Line
	{
		CString		m_strText;
		DWORD		m_dwColor;

		Line(const CString& strText, DWORD dwColor)
		:	m_strText(strText),
			m_dwColor(dwColor)
		{}
	};

	typedef std::vector<Line> LineVec;

public:
	static const char m_cTokenMark;

protected:
	TDETINFO_TYPE	m_eType;			///< 상세정보 타입
	
	DWORD			m_dwInfoID;			///< 상세정보 아이디
	WORD			m_wImageID;			///< 상세정보를 대표하는 이미지의 아이디
	CRect			m_rcRef;			///< 정보창이 참조하는 곳의 UI 크기
	
	BOOL			m_bLRType;			///< 상세창을 참조영역에서 좌우측으로 표시할지 여부.
	BOOL			m_bPriorLeft;		///< 상세창을 왼쪽으로 먼저 표시할지 여부
	BOOL			m_bPriorTop;		///< 상세창을 위쪽으로 먼저 표시할지 여부

	BOOL			m_bNeedBuild;		///< 상세정보를 다시 구성해야 하는가를 위한 플래그

	Line			m_TitleB;			///< 상세정보를 대표하는 타이틀
	LineVec			m_LinesB;			///< 상세정보 줄
	INT			m_nOutlookStyle;

public:
	TDETINFO_TYPE GetType() const		{ return m_eType; }
	BOOL IsLRType() const				{ return m_bLRType; }
	BOOL IsPriorLeft() const			{ return m_bPriorLeft; }
	BOOL IsPriorTop() const				{ return m_bPriorTop; }
	DWORD GetInfoID() const				{ return m_dwInfoID; }
	WORD GetImageID() const				{ return m_wImageID; }
	const CRect& GetSize() const		{ return m_rcRef; }
	INT GetOutlookStyle() const { return m_nOutlookStyle; }

public:
	/**
		상세정보 UI의 기준 방향을 설정한다.
		이 설정에 따라 GetUIPosition() 로 부터 얻어지는
		상세정보창의 위치값이 결정된다.
	*/
	void SetDir(BOOL bLR, BOOL bPriorLeft, BOOL bPriorTop);

	/// 상세정보 UI를 놓을 위치
	virtual CPoint GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse );

	/// 상세정보를 대표하는 타이틀을 얻는다.
	const CString& GetTitleText();
	/// 상세정보를 대표하는 타이틀의 색을 얻는다.
	DWORD GetTitleColor();

	/// 상세정보의 전체 줄의 수를 얻는다.
	UINT GetLineCount();
	/// 상세정보의 전체 줄의 텍스트를 얻는다.
	const CString& GetLineText(UINT nLine);
	/// 상세정보의 전체 줄의 색을 얻는다.
	DWORD GetLineColor(UINT nLine);
	CString BuildTimeFormatSTR( DWORD dwTick);

public:	
	/// 서로 같은 타겟에 대한 상세정보인가를 비교한다.
	virtual BOOL Compare(const ITDetailInfo* pTarget) = 0;
	/// 상세정보를 구성한다.
	virtual void Build() = 0;
	/// 아이템 박스의 스킨을 사용할 것 인가?
	virtual INT CheckUsingSkin();

public :
//	virtual CTClientItem* GetItemInst() const	{ return NULL; }

protected:
	/// 주어진 문자열의 상태를 판단한다.
	virtual LINE_STATE _GetLineState(const CString& strSource, int nIdx);
	
	/// 주어진 문자열을 라인에 추가한다.
	virtual void _AddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	/// 주어진 문자열을 변환한 후 타이틀로 설정한다.
	virtual void _ConvertAndSetTitle(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	/// 주어진 문자열을 변환한 후 라인에 추가한다.
	virtual void _ConvertAndAddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);

protected:
	// Arab Ver.
	BOOL GetPriorDir(BOOL bPriorDir);
	CPoint _GetUIPosition_TB(const CRect& rcDetDlg, BOOL bPriorLeft, BOOL bPriorTop);
	CPoint _GetUIPosition_LR(const CRect& rcDetDlg, BOOL bPriorLeft, BOOL bPriorTop);

public:
	ITDetailInfo(
		TDETINFO_TYPE eType,
		DWORD dwInfoID, 
		WORD wImgID, 
		const CRect& rc);

	virtual ~ITDetailInfo();
};
// =====================================================================
/**	@class		CTStatDetInfo
	@brief		스탯 에대한 설명을 가진 상세정보의 전달과 
				표시를 위한 클래스.
	
*/// ===================================================================
class CTStatDetInfo : public ITDetailInfo
{
public:
	/// 아이템 능력치 토큰 타입
	enum TINFOTOKEN_STAT
	{
		TINFOTOKEN_STAT_STR,
		TINFOTOKEN_STAT_DEX,
		TINFOTOKEN_STAT_CON,
		TINFOTOKEN_STAT_INT,
		TINFOTOKEN_STAT_WIS,
		TINFOTOKEN_STAT_MEN,
		TINFOTOKEN_STAT_MIN_PHYSICAL_ATTACK_POWER,
		TINFOTOKEN_STAT_MAX_PHYSICAL_ATTACK_POWER,
		TINFOTOKEN_STAT_PHYSICAL_DEFENCE_POWER,
		TINFOTOKEN_STAT_MIN_LONG_ATTACK_POWER,
		TINFOTOKEN_STAT_MAX_LONG_ATTACK_POWER,
		TINFOTOKEN_STAT_ATTACK_DELAY_PHYSICAL,
		TINFOTOKEN_STAT_ATTACK_DELAY_LONG,
		TINFOTOKEN_STAT_ATTACK_DELAY_MAGIC,
		TINFOTOKEN_STAT_ATTACK_DELAY_RATE_PHYSICAL,
		TINFOTOKEN_STAT_ATTACK_DELAY_RATE_LONG,
		TINFOTOKEN_STAT_ATTACK_DELAY_RATE_MAGIC,
		TINFOTOKEN_STAT_ATTACK_LEVEL,
		TINFOTOKEN_STAT_DEFENCE_LEVEL,
		TINFOTOKEN_STAT_PHYSICAL_CRITICAL_PROB,
		TINFOTOKEN_STAT_MIN_MAGIC_ATTACK_POWER,
		TINFOTOKEN_STAT_MAX_MAGIC_ATTACK_POWER,
		TINFOTOKEN_STAT_MAGIC_DEFENCE_POWER,
		TINFOTOKEN_STAT_MAGIC_ATTACK_LEVEL,
		TINFOTOKEN_STAT_MAGIC_DEFENCE_LEVEL,
		TINFOTOKEN_STAT_CHARGE_SPEED,
		TINFOTOKEN_STAT_CHARGE_PROB,
		TINFOTOKEN_STAT_MAGIC_CRITICAL_PROB,
		TINFOTOKEN_STAT_SKILL_POINT,
		TINFOTOKEN_STAT_COUNT
	};

public:
	static const CString TSTAT_TOKENS[TINFOTOKEN_STAT_COUNT];

public:
	CTStatDetInfo(
		DWORD dwInfoID,
		LPTSTATINFO,
		DWORD dwTitleColor,
		const CRect& rc);

	virtual ~CTStatDetInfo();

public:
	virtual LINE_STATE _GetLineState(const CString& strSource, int nIdx);
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();

public:
	/// 주어진 문자열을 변환한 후 라인에 추가한다.
	virtual void _ConvertAndAddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	virtual CString _GetAttrString( TINFOTOKEN_STAT eToken);

public:
	LPTSTATINFO m_pStatInfo;
	DWORD m_dwTitleColor;
};

// =====================================================================
/**	@class		CTNorDetInfo
	@brief		타이틀과 그에대한 설명을 가진 상세정보의 전달과 
				표시를 위한 클래스.
	
*/// ===================================================================
class CTNorDetInfo : public ITDetailInfo
{
protected:
	/// 타이틀
	CString m_strTITLE;	
	
public:	
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();

protected:
	virtual LINE_STATE _GetLineState(const CString& strSource, int nIdx);
	//virtual void _AddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	//virtual void _ConvertAndSetTitle(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	//virtual void _ConvertAndAddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);

public:
	CTNorDetInfo(
		const CString& strTitle,
		DWORD dwInfoID, 
		WORD wImgID, 
		const CRect& rc);

	virtual ~CTNorDetInfo();
};

// =====================================================================
/**	@class		CTItemDetInfo
	@brief		일반 아이템에 관한 상세정보의 전달과 표시를 위한 클래스.
	
*/// ===================================================================
class CTItemDetInfo : public ITDetailInfo
{
public:
	/// 아이템 능력치 토큰 타입
	enum TINFOTOKEN_ITEM
	{
		TINFOTOKEN_ITEM_ITEMMINAP,		///< 최소공격력
		TINFOTOKEN_ITEM_ITEMMAXAP,		///< 최대공격력
		TINFOTOKEN_ITEM_ITEMAPSEC,		///< 초당공격력
		TINFOTOKEN_ITEM_ITEMDP,			///< 물리방어력
		TINFOTOKEN_ITEM_ITEMMINMAP,		///< 최소마법공격력
		TINFOTOKEN_ITEM_ITEMMAXMAP,		///< 최대마법공격력
		TINFOTOKEN_ITEM_ITEMMAPSEC,		///< 초당마법공격력
		TINFOTOKEN_ITEM_ITEMMDP,		///< 마법방어력
		TINFOTOKEN_ITEM_ITEMBLOCK,		///< 방패방어율
		TINFOTOKEN_ITEM_ATTRLEVEL,		///< 성능레벨
		TINFOTOKEN_ITEM_CURDURA,		///< 현재 내구도
		TINFOTOKEN_ITEM_MAXDURA,		///< 최대 내구도
		TINFOTOKEN_ITEM_CURREFI,		///< 현재 재련횟수
		TINFOTOKEN_ITEM_MAXREFI,		///< 최대 재련횟수
		TINFOTOKEN_ITEM_SELLPRICE,		///< 판매 가격
		TINFOTOKEN_ITEM_ENDTIME,	///< 남은기한
		TINFOTOKEN_ITEM_NEEDSLEVEL, ///< 요구레벨
		TINFOTOKEN_ITEM_COUNT
	};

	/// 아이템에서 공통적으로 사용하는 토큰 타입
	enum TINFO_ITEMTYPE
	{
		TINFO_ITEMTYPE_ITEMNORMAL,		///< 일반아이템
		TINFO_ITEMTYPE_ITEMGRADE,		///< 강화아이템
		TINFO_ITEMTYPE_ITEMMAGIC,		///< 매직아이템
		TINFO_ITEMTYPE_ITEMSET,			///< 세트아이템
		TINFO_ITEMTYPE_ITEMRARE,		///< 레어아이템
		TINFO_ITEMTYPE_ITEMUNIQUE,		///< 유니크아이템
		TINFO_ITEMTYPE_COUNT
	};

public:
	/// 아이템 능력치 토큰 리스트
	static const CString TITEM_TOKENS[TINFOTOKEN_ITEM_COUNT];
	/// 아이템의 공통 토큰 리스트
	static const CString TITEM_TYPE_TOKENS[TINFO_ITEMTYPE_COUNT];
	/// 아이템 성능 레벨등급별 이름
	static const DWORD TITEMATTR_NAMES[TATTRLEVELNAME_MAX];
	/// 아이템 성능 레벨이름 토큰
	static const CString TITEM_ATTRNAME_TOKEN;

protected:
	LPTITEM 	m_pItemTemp;		///< 상세하고자 하는 아이템의 템플릿 데이타
	WORD		m_wAttrID;			///< 이 아이템의 능력치 아이디
	CString		m_strAttrName;		///< 성능을 나타내는 등급명
	WORD		m_wAttrLevel;		///< 성능 레벨
	DWORD		m_dwSellPrice;		///< 판매 가격

public:
	LPTITEM GetItemTemp() const	{ return m_pItemTemp; }
	WORD GetAttrID() const		{ return m_wAttrID; }

public:	
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();

protected:
	virtual LINE_STATE _GetLineState(const CString& strSource, int nIdx);
	//virtual void _AddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	virtual void _ConvertAndSetTitle(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	virtual void _ConvertAndAddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);

	/// 아이템 아이콘을 정한다.
	virtual void _BuildItemIcon();
	/// 아이템 성능값을 정한다.
	virtual void _BuildAttrID();
	/// 아이템 성능관련 문자열을 얻는다.
	virtual CString _GetAttrString(TINFOTOKEN_ITEM eToken);
	/// 아이템 성능관련 문자열 색깔을 얻는다.
	virtual DWORD _GetAttrColor(TINFOTOKEN_ITEM eToken, DWORD dwDefaultColor );

protected:
	void _BuildAttrNameAndLevel();

public:
	CTItemDetInfo(
		LPTITEM pItemTemp,
		const CRect& rc);

	virtual ~CTItemDetInfo();
};

// =====================================================================
/**	@class		CTItemInstDetInfo
	@brief		강화나 매직등의 부가 옵션이 붙을 수 있는 인스턴스형 
				아이템에 관한 상세정보의 전달과 표시를 위한 인터페이스.
	
*/// ===================================================================
class CTItemInstDetInfo : public CTItemDetInfo
{
protected:
	/// 기본 템플릿 데이타로 부터 실제 생성된 아이템 인스턴스.
	CTClientItem*	m_pItemInst;

public:
	CTClientItem* GetItemInst() const	{ return m_pItemInst; }

public:	
	//virtual void Build() = 0;
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual INT CheckUsingSkin();

protected:
	//virtual LINE_STATE _GetLineState(const CString& strSource, int nIdx);
	//virtual void _AddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	virtual void _ConvertAndSetTitle(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	//virtual void _ConvertAndAddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	virtual void _BuildItemIcon();
	virtual void _BuildAttrID();
	virtual CString _GetAttrString(TINFOTOKEN_ITEM eToken);
	virtual DWORD _GetAttrColor(TINFOTOKEN_ITEM eToken, DWORD dwDefaultColor );

public:
	CTItemInstDetInfo(
		TDETINFO_TYPE eType,
		CTClientItem* pItemInst,
		const CRect& rc);

	virtual ~CTItemInstDetInfo();
};

// =====================================================================
/**	@class		CTSealedItemDetInfo
	@brief		봉인된 아이템에 관한 상세정보의 전달과 표시를 위한 클래스.
	
*/// ===================================================================
class CTSealedItemDetInfo : public CTItemInstDetInfo
{
public:	
	virtual void Build();
	//virtual BOOL Compare(const ITDetailInfo* pTarget);

protected:
	//virtual LINE_STATE _GetLineState(const CString& strSource, int nIdx);
	//virtual void _AddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	//virtual void _ConvertAndSetTitle(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	//virtual void _ConvertAndAddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	//virtual void _BuildItemIcon();
	//virtual void _BuildAttrID();
	//virtual CString _GetAttrString(TINFOTOKEN_ITEM eToken);

public:
	CTSealedItemDetInfo(
		CTClientItem* pItemInst,
		const CRect& rc);

	virtual ~CTSealedItemDetInfo();
};

// =====================================================================
/**	@class		CTMagicItemDetInfo
	@brief		추가옵션(등급,매직,레어) 아이템에 관한 상세정보의 전달과 표시를 위한 클래스.
	
*/// ===================================================================
class CTOptionItemDetInfo : public CTItemInstDetInfo
{
public:
	/// 매직 옵션 토큰 타입
	enum TINFOTOKEN_MAGICOPTION
	{
		TINFOTOKEN_MAGICOPTION1,
		TINFOTOKEN_MAGICOPTION2,
		TINFOTOKEN_MAGICOPTION3,
		TINFOTOKEN_MAGICOPTION4,
		TINFOTOKEN_MAGICOPTION5,
		TINFOTOKEN_MAGICOPTION6,
		TINFOTOKEN_MAGICOPTION_COUNT,
	};

	/// 매직 데이타 토큰 타입
	enum TINFOTOKEN_MAGICDATA
	{
		TINFOTOKEN_MAGICDATA1,
		TINFOTOKEN_MAGICDATA2,
		TINFOTOKEN_MAGICDATA3,
		TINFOTOKEN_MAGICDATA4,
		TINFOTOKEN_MAGICDATA5,
		TINFOTOKEN_MAGICDATA6,
		TINFOTOKEN_MAGICDATA_COUNT
	};

	/// 매직 관련 토큰 타입
	enum TINFOTOKEN_MAGIC
	{
		TINFOTOKEN_MAGIC_TITLE,
		TINFOTOKEN_MAGIC_LEVEL,
		TINFOTOKEN_MAGIC_COUNT
	};

public:
	/// 아이템 강화 토큰
	static const CString TGRADEITEM_TOKEN;

protected:
	BOOL	m_bProcOption;

public:
	/// 매직 옵션 토큰 리스트
	static const CString TMAGICOPTION_TOKENS[TINFOTOKEN_MAGICOPTION_COUNT];
	/// 매직 데이타 토큰 리스트
	static const CString TMAGICDATA_TOKENS[TINFOTOKEN_MAGICDATA_COUNT];
	/// 매직 관련 토큰 리스트
	static const CString TMAGIC_TOKENS[TINFOTOKEN_MAGIC_COUNT];
	/// 매빅 옵션 레벨에 따른 문자열
	static const CString TMAGICLEVEL_NAMES[TMAGICLEVEL_MAX];

public:	
	//virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();

protected:
	virtual LINE_STATE _GetLineState(const CString& strSource, int nIdx);
	//virtual void _AddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	virtual void _ConvertAndSetTitle(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	virtual void _ConvertAndAddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	//virtual void _BuildAttrID();
	virtual CString _GetAttrString(TINFOTOKEN_ITEM eToken);

protected:
	BOOL _GetMagicOptionString(int nIdx, CString& outOption, CString& outData);
	CString _GetMagicString(int nIdx);
	BOOL _IsMagicLine( const CString& strLine );

public:
	CTOptionItemDetInfo(
		CTClientItem* pItemInst,
		const CRect& rc);

	virtual ~CTOptionItemDetInfo();
};

// =====================================================================
/**	@class		CTSkillDetInfo
	@brief		스킬에 관한 상세정보의 전달과 표시를 위한 클래스.
*/// ===================================================================
class CTSkillDetInfo : public ITDetailInfo
{
public:
	/// 스킬 타입 토큰
	enum TINFOTOKEN_SKILL
	{
	TINFOTOKEN_SKILL_DATA1 = 0,
	TINFOTOKEN_SKILL_DATA2,
	TINFOTOKEN_SKILL_DATA3,
	TINFOTOKEN_SKILL_DATA4,
	TINFOTOKEN_SKILL_DATA5,
	TINFOTOKEN_SKILL_DATA6,
	TINFOTOKEN_SKILL_LEVEL,
	TINFOTOKEN_SKILL_HITRATE,
	TINFOTOKEN_SKILL_ACTTIME,
	TINFOTOKEN_SKILL_DURATION,
	TINFOTOKEN_SKILL_REUSE,
	TINFOTOKEN_SKILL_USEHP,
	TINFOTOKEN_SKILL_USEMP,
	TINFOTOKEN_SKILL_UPGRADETITLE,
	TINFOTOKEN_SKILL_PREVLEVEL,
	TINFOTOKEN_SKILL_CLASSPOINT,
	TINFOTOKEN_SKILL_SKILLPOINT,
	TINFOTOKEN_SKILL_NEXTLEVEL,
	TINFOTOKEN_SKILL_NEXTPRICE,
	TINFOTOKEN_SKILL_COUNT
	};

public:
	static const CString TSKILL_TOKENS[TINFOTOKEN_SKILL_COUNT];

protected:
    LPTSKILL	m_pTSkill;		///< 스킬 템플릿
	BYTE		m_bLevel;		///< 표기하고자 하는 스킬의 레벨
	BOOL		m_bSkillReq;	///< 스킬 요구사항 표시 여부

	/**
		Build 중 스킬의 현재 레벨에 대한 상세를 구성하는 
		중인지 다음에 업그레이드 하기위한 상세를 구성하는
		중인지를 구별하기위한 플래그.
	*/
	BOOL		m_bProcUpgrade;

public:
	LPTSKILL GetSkillTemp() const	{ return m_pTSkill; }
	BYTE GetSkillLevel() const		{ return m_bLevel; }
	BOOL GetSkillReq() const		{ return m_bSkillReq; }

public:
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();

protected:
	virtual LINE_STATE _GetLineState(const CString& strSource, int nIdx);
	virtual void _AddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	virtual void _ConvertAndSetTitle(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);
	virtual void _ConvertAndAddLine(const CString& strSource, DWORD dwColor = TDEFAULT_TEXT_COLOR);

protected:
	CString _GetSkillString(TINFOTOKEN_SKILL eToken);

public:
	CTSkillDetInfo(
		LPTSKILL  pTSkill,
		BYTE bLevel,
		BOOL bSkillReq,
		const CRect& rc);

	virtual ~CTSkillDetInfo();
};

// =====================================================================
/**	@class		CTCastleDetInfo
	@brief		성에 관한 상세정보의 전달과 표시를 위한 클래스.
*/// ===================================================================
class CTCastleDefInfo : public ITDetailInfo
{
public :
	CString	m_strCastle;
	CTime	m_timeNext;
	CString	m_strAtkGuild;
	CString m_strDefGuild;
	WORD m_wAtkGuildPoint;
	WORD m_wAtkCountryPoint;
	BYTE m_bAtkCount;
	WORD m_wDefGuildPoint;
	WORD m_wDefCountryPoint;
	BYTE m_bDefCount;
	CString m_strMyGuild;
	WORD m_wMyGuildPoint;
	VTOP3 m_vDTop3;
	VTOP3 m_vCTop3;

public :
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();
	virtual CPoint GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse );

public :
	CTCastleDefInfo(
		CString strCastle,
		CTime timeNext,
		CString strAtkGuild,
		CString strDefGuild,
		WORD wAtkGuildPoint,
		WORD wAtkCountryPoint,
		BYTE bAtkCount,
		WORD wDefGuildPoint,
		WORD wDefCountryPoint,
		BYTE bDefCount,
		CString strMyGuild,
		WORD wMyGuildPoint,
#ifdef MODIFY_GUILD
		VTOP3* vDTop3,
		VTOP3* vCTop3,
#endif
		const CRect& rc);

	virtual ~CTCastleDefInfo();

#ifdef MODIFY_GUILD
	void WriteCTop3();
	void WriteDTop3();
#endif
};

// =====================================================================
/**	@class		CTTerritoryDetInfo
	@brief		지역에 관한 상세정보의 전달과 표시를 위한 클래스.
*/// ===================================================================

class CTTerritoryDetInfo : public ITDetailInfo
{
public:
	CString m_strTerritory;
	CTime m_timeNext;
	CString m_strHeroName;

public:
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();
	virtual CPoint GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse );
public:
	CTTerritoryDetInfo(
		CString strTerritory, 
		CTime timeNext,
		CString strHeroName,
		const CRect& rc);

	virtual ~CTTerritoryDetInfo();
};

class CTPvPDetInfo : public ITDetailInfo
{
public:
	BYTE m_bTabIndex;
	CString m_strName;
	BYTE m_bWin;
	BYTE m_bClass;
	BYTE m_bLevel;
	DWORD m_dwPoint;
	CTime m_dlDate;
	DWORD m_dwTitleColor;	

public:
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();

public:
	CTPvPDetInfo(
		BYTE bTabIndex,
		CString strName,
		BYTE bWin,
		BYTE bClass,
		BYTE bLevel,
		DWORD dwPoint,
		CTime dlDate,
		DWORD dwTitleColor,
		const CRect& rc );

	virtual ~CTPvPDetInfo();
};

class CTTextDetInfo : public ITDetailInfo
{
public:
	CString m_strText;
	CPoint m_ptPosition;

public:
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();
	virtual CPoint GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse );

public:
	CTTextDetInfo(
		CString& strText,
		CPoint pt );

	virtual ~CTTextDetInfo();
};

class CTFameRankDetInfo : public ITDetailInfo
{
public:
	CString m_strName;
	DWORD	m_dwTotalPoint;
	DWORD	m_dwMonthPoint;
	WORD	m_wWin;
	WORD	m_wLose;
	BYTE	m_bLevel;
	BYTE	m_bClass;

public:

	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();

public:
	CTFameRankDetInfo(
		CString strName,
		DWORD dwTotalPoint,
		DWORD dwMonthPoint,
		WORD wWin,
		WORD wLose,
		BYTE bLevel,
		BYTE bClass,
		const CRect& rc);

	virtual ~CTFameRankDetInfo();
};

class CTDefToolTipInfo : public ITDetailInfo
{
public :
	CString	m_strToolTip;
	CString m_strTitle;

public :
	virtual BOOL Compare( const ITDetailInfo* pTarget );
	virtual void Build();

public :
	CTDefToolTipInfo( CString strTitle, CString strToolTip, const CRect& rc );
	virtual ~CTDefToolTipInfo();

};

class CTTournamentPlayerInfo : public ITDetailInfo
{
public:
	CString m_strName;
	BYTE m_bWin;
	BYTE m_bCountry;
	BYTE m_bClass;
	BYTE m_bLevel;
	BYTE m_bInfoType;
	CString m_strText;
	INT m_nLine;

public:
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();

public:
	CTTournamentPlayerInfo(
		CString strName,
		BYTE bWin,
		BYTE bCountry,
		BYTE bClass,
		BYTE bLevel,
		const CRect& rc );

	CTTournamentPlayerInfo(
		CString strTitle,
		CString strText,
		int nLine,
		const CRect& rc );

	virtual ~CTTournamentPlayerInfo();
};

class CTMissionDetInfo : public ITDetailInfo
{
public:
	CString m_strMission;
	CTime m_timeNext;
	BYTE m_bCountry;
	BYTE m_bStatus;

public:
	virtual BOOL Compare(const ITDetailInfo* pTarget);
	virtual void Build();
	virtual CPoint GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse );

public:
	CTMissionDetInfo(
		CString m_strMission, 
		CTime timeNext,
		BYTE m_bCountry,
		BYTE m_bStatus,
		const CRect& rc);

	virtual ~CTMissionDetInfo();
};

typedef SharedPtr<ITDetailInfo>			ITDetailInfoPtr;
typedef SharedPtr<CTNorDetInfo>			CTNorDetInfoPtr;
typedef SharedPtr<CTItemDetInfo>		CTItemDetInfoPtr;
typedef SharedPtr<CTItemInstDetInfo>	CTItemInstDetInfoPtr;
typedef SharedPtr<CTSealedItemDetInfo>	CTSealedItemDetInfoPtr;
typedef SharedPtr<CTOptionItemDetInfo>	CTOptionItemDetInfoPtr;
typedef SharedPtr<CTSkillDetInfo>		CTSkillDetInfoPtr;
typedef SharedPtr<CTCastleDefInfo>		CTCastleDefInfoPtr;
typedef SharedPtr<CTTerritoryDetInfo>	CTTerritoryDetInfoPtr;
typedef SharedPtr<CTFameRankDetInfo>	CTFameRankDetInfoPtr;
typedef SharedPtr<CTTournamentPlayerInfo>	CTTournamentPlayerDetInfoPtr;
typedef SharedPtr<CTMissionDetInfo>	CTMissionDetInfoPtr;