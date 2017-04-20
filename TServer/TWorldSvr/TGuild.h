#pragma once

class CTGuild
{
public:
	DWORD	m_dwID;
	CString m_strName;
	DWORD	m_dwChief;
	CString m_strChief;
	BYTE	m_bLevel;
	DWORD	m_dwFame;
	DWORD	m_dwFameColor;
	DWORD	m_dwGI;
	DWORD	m_dwExp;
	BYTE	m_bGPoint;
	BYTE	m_bStatus;
	DWORD	m_dwGold;
	DWORD	m_dwSilver;
	DWORD	m_dwCooper;
	BYTE	m_bDisorg;
	DWORD	m_dwTime;
	BYTE	m_bCountry;
	__time64_t	m_timeEstablish;
	CString m_strArticleTitle;
	DWORD m_dwPvPUseablePoint;
	DWORD m_dwPvPTotalPoint;
	DWORD m_dwPvPMonthPoint;
	DWORD m_dwRankTotal;
	DWORD m_dwRankMonth;

	BYTE	m_bMaxCabinet;
	MAPTITEM m_mapTCabinet;

	MAPTGUILDMEMBER	m_mapTMember;
	MAPTTACTICSMEMBER m_mapTTactics;

	DWORD	m_dwArticleIndex;
	MAPTGUILDARTICLE m_mapTArticle;

	VECTORDWORD	m_vAllience;
	VECTORDWORD m_vEnemy;
	VTGUILDPOINTREWARD m_vPointReward;

	LPTGUILDLEVEL m_pTLEVEL;

public:
	BYTE	AddMember(LPTGUILDMEMBER pMember);
	void	DelMember(LPTGUILDMEMBER pMember);
	LPTCHARACTER	Designate(DWORD dwID, BYTE bDuty);
	LPTCHARACTER	CreatePeerage(DWORD dwID, BYTE bPeer);
	LPTGUILDMEMBER	FindMember(DWORD dwID);
	LPTGUILDMEMBER	FindMember(CString strName);
	LPTGUILDMEMBER	FindDesignate(BYTE bDuty);
	LPTGUILDMEMBER	FindChief();

	BYTE	AddTactics(LPTTACTICSMEMBER pMember);
	BYTE	DelTactics(LPTTACTICSMEMBER pMember, BYTE bKick);

	BYTE	FindDuty(DWORD dwID);
	BYTE	FindPeer(DWORD dwID);
	void	SetMemberConnection(DWORD dwCharID, LPTCHARACTER pChar, INT64 dlTime);
	void	SetMemberLevel(DWORD dwCharID, BYTE bLevel);
	BYTE	CheckPeerage(BYTE bDuty, BYTE bPeer);
	BYTE	GetMaxMemberCnt();
	BYTE	GetMemberSize();
	BYTE	CanAddMember();
	BYTE	CanAddTactics();
	void	UpdateLevel();

	BYTE	GainEXP(LPTGUILDMEMBER pMember, DWORD dwExp);
	BYTE	GainMoney(DWORD dwGold, DWORD dwSilver, DWORD dwCooper);
	BYTE	UseMoney(INT64 dlMoney, BYTE bUse);
	BYTE	UseMoney(DWORD dwGold, DWORD dwSilver, DWORD dwCooper, BYTE bUse);
	BYTE	Contribution(DWORD dwCharID, DWORD dwExp, DWORD dwGold, DWORD dwSilver, DWORD dwCooper, DWORD dwPvPoint);

	BYTE	GetArticleSize();
	void	AddArticle(DWORD dwID, BYTE bDuty, CString strWritter, CString strTitle, CString strArticle, DWORD dwTime);
	DWORD	AddArticle(BYTE bDuty, CString strWritter, CString strTitle, CString strArticle, DWORD dwTime);
	BYTE	DelArticle(DWORD dwID);
	BYTE	UpdateArticle(DWORD dwID, CString strTitle, CString strArticle);

	void	SetFame(DWORD dwFame, DWORD dwFameColor);

	BYTE	GetCabinetSize();
	void	PutInItem(DWORD dwItemID, LPTITEM pItem);
	LPTITEM	FindCabinetItem(DWORD dwItemID);
	BYTE	TakeOutItem(DWORD dwItemID, BYTE bCount);

	LPTTACTICSMEMBER FindTactics(DWORD dwID);
	LPTTACTICSMEMBER FindTactics(CString strName);
	void	SetTacticsConnection(DWORD dwCharID, LPTCHARACTER pChar);
	void	GainPvPoint(DWORD dwPoint, BYTE bEvent, BYTE bType);
	void	UsePvPoint(DWORD dwPoint, BYTE bType);
	void	PointLog(DWORD dwPoint, CString strTarget, INT64 dlDate);
	void	CalcWeekRecord(LPTGUILDMEMBER pMember, DWORD dwDate);
	void	CalcWeekRecord(DWORD dwDate);
	BYTE	CanApplyWar(WORD wCastle);
	WORD	GetCastleApplicantCount(WORD wCastle);
	void	GetCastleApplicantCount(MAPWORD * mapCastle);

public:
	CTGuild(void);
	~CTGuild(void);
};
