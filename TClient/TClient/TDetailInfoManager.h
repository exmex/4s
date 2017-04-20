#pragma once


// =====================================================================
/**	@class		CTDetailInfoManager
	@brief		상세정보창에 관한 정보 유지, 검색 및 화면 출력을 담당한다.
	
*/// ===================================================================
class CTDetailInfoManager
{
public:
	/// 가장 최근에 표시한 상세정보
	static ITDetailInfoPtr	m_pLastInfo;

	/// 상세정보창을 표시하기위해 기다린 시간.
	static DWORD			m_dwInfoTick;

	/// 상세정보창을 마우스의 위치에 상관없이 일정시간 동안 보여주기 위한 시간.
	static DWORD			m_dwInfoStaticTick;

public:
	static ITDetailInfoPtr NewStatInst(
		DWORD dwInfoID,
		LPTSTATINFO,
		DWORD dwTitleColor,
		const CRect& rc = CRect(0,0,0,0) );

	/// 상세정보 인스턴스를 생성한다.
	static ITDetailInfoPtr NewNorInst(
		const CString& strTitle,
		DWORD dwInfoID, 
		WORD wImgID, 
		const CRect& rc = CRect(0,0,0,0));

	/// 스킬 상세정보 인스턴스를 생성한다.
	static ITDetailInfoPtr NewSkillInst(
		LPTSKILL pTSkill,
		BYTE bLevel,
		BOOL bSkillReq,
		const CRect& rc = CRect(0,0,0,0));

	/// 일반 아이템 상세정보 인스턴스를 생성한다.
	static ITDetailInfoPtr NewItemInst(
		LPTITEM pItem, 
		const CRect& rc = CRect(0,0,0,0));

	/// 주어진 아이템에 맞는 상세정보 인스턴스를 생성한다.
	static ITDetailInfoPtr NewItemInst(
		CTClientItem* pItem,
		const CRect& rc = CRect(0,0,0,0));

	/// 성정보에 맞는 상세정보 인스턴스를 생성한다.
	static ITDetailInfoPtr NewTerritoryInst(
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
		const CRect& rc = CRect(0,0,0,0));

	/// 지역정보에 맞는 상세정보 인스턴스를 생성한다.
	static ITDetailInfoPtr NewTerritoryInst(
		CString strTerritory,
		CTime timeNext,
		CString strHeroName,
		const CRect& rc = CRect(0,0,0,0));

	static ITDetailInfoPtr NewPvPInst(
		BYTE bTabIndex,
		CString strName,
		BYTE bWin,
		BYTE bClass,
		BYTE bLevel,
		DWORD dwPoint,
		CTime dlDate,
		DWORD dwTitleColor,
		const CRect& rc = CRect(0,0,0,0) );

	static ITDetailInfoPtr NewTextInst(
		CString& strText,
		CPoint pt );

	static ITDetailInfoPtr NewFameInst(
		CString	strName,
		DWORD	dwTotalPoint,
		DWORD	dwMonthPoint,
		WORD	wWin,
		WORD	wLose,
		BYTE	bLevel,
		BYTE	bClass,
		const CRect& rc = CRect(0,0,0,0)
		);

	static ITDetailInfoPtr NewDefTooltipInst(
		CString strTitle,
		CString strTooltip,
		const CRect& rc = CRect(0,0,0,0));

	static ITDetailInfoPtr NewTournamentPlayerInst(
		CString strName,
		BYTE bWin,
		BYTE bCountry,
		BYTE bClass,
		BYTE bLevel,
		const CRect& rc = CRect(0,0,0,0) );

	static ITDetailInfoPtr NewTournamentPlayerInst(
		CString strTitle,
		CString strText,
		int nLine,
		const CRect& rc = CRect(0,0,0,0) );

	static ITDetailInfoPtr NewMissionInst(
		CString strTerritory,
		CTime timeNext,
		BYTE bCountry,
		BYTE bStatus,
		const CRect& rc = CRect(0,0,0,0));

	static ITDetailInfoPtr NewPlayerInst(
		CString strName,
		BYTE bLevel,
		BYTE bRace,
		BYTE bSex,
		BYTE bFace,
		BYTE bHair,
		BYTE bSenior,
		BYTE bClass,
		const CRect& rc = CRect(0,0,0,0));

public:
	static void UpdateTick(DWORD dwTick);
	static void UpdateInfo(const CPoint& pt);
};
