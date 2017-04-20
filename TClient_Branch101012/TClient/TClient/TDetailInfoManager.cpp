#include "stdafx.h"
#include "TDetailInfoManager.h"
#include "TClientGame.h"
#include "TClientWnd.h"

// ====================================================================================================
ITDetailInfoPtr		CTDetailInfoManager::m_pLastInfo(NULL);
DWORD				CTDetailInfoManager::m_dwInfoTick = 0;
DWORD				CTDetailInfoManager::m_dwInfoStaticTick = 0;
// ====================================================================================================
ITDetailInfoPtr CTDetailInfoManager::NewStatInst( DWORD dwInfoID, LPTSTATINFO pStatInfo, DWORD dwTitleColor, const CRect& rc)
{
	return ITDetailInfoPtr( new CTStatDetInfo(	 dwInfoID, pStatInfo, dwTitleColor, rc) );
}
// ====================================================================================================
ITDetailInfoPtr CTDetailInfoManager::NewNorInst(const CString& strTitle,
												DWORD dwInfoID, 
												WORD wImgID, 
												const CRect& rc)
{
	return ITDetailInfoPtr( new CTNorDetInfo(strTitle,dwInfoID,wImgID,rc) );
}
// ----------------------------------------------------------------------------------------------------
ITDetailInfoPtr CTDetailInfoManager::NewSkillInst(LPTSKILL pTSkill,
												  BYTE bLevel,
												  BOOL bSkillReq,
												  const CRect& rc)
{
	return ITDetailInfoPtr( new CTSkillDetInfo(pTSkill,bLevel,bSkillReq,rc) );
}
// ----------------------------------------------------------------------------------------------------
ITDetailInfoPtr CTDetailInfoManager::NewItemInst(LPTITEM pItem, const CRect& rc)
{
	return ITDetailInfoPtr( new CTItemDetInfo(pItem, rc) );
}
// ----------------------------------------------------------------------------------------------------
ITDetailInfoPtr CTDetailInfoManager::NewItemInst(CTClientItem* pItem, const CRect& rc)
{
	ITDetailInfoPtr pInfo;

	LPTITEM pTITEM = pItem->GetTITEM();
	if( !pTITEM )
		return pInfo;
	
	if( pTITEM->m_bType != IT_GAMBLE &&
		pItem->CanGamble() )
	{
		pInfo = ITDetailInfoPtr(new CTSealedItemDetInfo(pItem, rc));
	}
	else
	{
		if( pItem->GetQuality() & TITEM_QUALITY_NORMAL )
			pInfo = ITDetailInfoPtr(new CTItemInstDetInfo(TDEFINFO_TYPE_INSTITEM, pItem, rc));
		else
			pInfo = ITDetailInfoPtr(new CTOptionItemDetInfo(pItem, rc));
	}

	return pInfo;
}
// ====================================================================================================
ITDetailInfoPtr CTDetailInfoManager::NewTerritoryInst(
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
		const CRect& rc)
{
	return ITDetailInfoPtr( new CTCastleDefInfo(
		strCastle,
		timeNext,
		strAtkGuild,
		strDefGuild,
		wAtkGuildPoint,
		wAtkCountryPoint,
		bAtkCount,
		wDefGuildPoint,
		wDefCountryPoint,
		bDefCount,
		strMyGuild,
		wMyGuildPoint,
#ifdef MODIFY_GUILD
		vDTop3,
		vCTop3,
#endif
		rc) );
}

ITDetailInfoPtr CTDetailInfoManager::NewTerritoryInst(
								CString strTerritory,
								 CTime timeNext,
								 CString strHeroName,
								 const CRect& rc )
{
	return ITDetailInfoPtr( new CTTerritoryDetInfo(strTerritory, timeNext, strHeroName, rc) );
}

ITDetailInfoPtr CTDetailInfoManager::NewPvPInst(
	BYTE bTabIndex,
	CString strName,
	BYTE bWin,
	BYTE bClass,
	BYTE bLevel,
	DWORD dwPoint,
	CTime dlDate,
	DWORD dwTitleColor,
	const CRect& rc )
{
	return ITDetailInfoPtr( new CTPvPDetInfo(
		bTabIndex,
		strName,
		bWin,
		bClass,
		bLevel,
		dwPoint,
		dlDate,
		dwTitleColor,
		rc ) );
}
// ====================================================================================================
ITDetailInfoPtr CTDetailInfoManager::NewTextInst(
	CString& strText,
	CPoint pt )
{
	return ITDetailInfoPtr( new CTTextDetInfo( strText, pt ) );
}

// ----------------------------------------------------------------------------------------------------
ITDetailInfoPtr CTDetailInfoManager::NewDefTooltipInst( CString strTitle, CString strTooltip, const CRect& rc )
{
	return ITDetailInfoPtr( new CTDefToolTipInfo( strTitle, strTooltip, rc ) );
}
// ----------------------------------------------------------------------------------------------------

void CTDetailInfoManager::UpdateTick(DWORD dwTick)
{
	if( m_pLastInfo )
	{
		if( m_dwInfoTick < TDETAIL_INFO_TICK )
			m_dwInfoTick += dwTick;
	}
	else
	{
		if( m_dwInfoTick > (dwTick>>1) )
			m_dwInfoTick -= (dwTick>>1);
		else
			m_dwInfoTick = 0;
	}

	m_dwInfoStaticTick -= min(m_dwInfoStaticTick, dwTick);
}
// ----------------------------------------------------------------------------------------------------
void CTDetailInfoManager::UpdateInfo(const CPoint& ptMouse)
{
	CTClientWnd* pMainWnd = CTClientWnd::GetInstance();
	CTClientGame* pGame = CTClientGame::GetInstance();
	if( !pGame || !pMainWnd )
		return;

	if( m_dwInfoStaticTick != 0 )
		return ;

	ITDetailInfoPtr pCurInfo;

	if( pMainWnd->GetTMainFrame() == pGame )
	{
		TComponent* pHIT = pGame->GetTopFrame(ptMouse,0);
		if( !pHIT )
			pHIT = pGame->GetHitPartyItemLottery(ptMouse);

		if( pHIT )
		{
			CTClientUIBase* pFrame = static_cast<CTClientUIBase*>(pHIT);
			pCurInfo = pFrame->GetTInfoKey(ptMouse);
			if( pCurInfo && (!m_pLastInfo || !m_pLastInfo->Compare(pCurInfo.get())) )
			{
				m_pLastInfo = pCurInfo;
				pGame->DisableUI(TFRAME_DETAIL_INFO);
			}
		}
	}

	if( !pCurInfo )
	{
		m_pLastInfo = ITDetailInfoPtr(NULL);
		pGame->DisableUI(TFRAME_DETAIL_INFO);
		return;
	}
	
	CTDetailInfoDlg* pDetInfoDlg 
		= static_cast<CTDetailInfoDlg*>( pGame->GetFrame(TFRAME_DETAIL_INFO) );
	
	if( m_pLastInfo &&
		m_dwInfoTick >= TDETAIL_INFO_TICK && 
		!pDetInfoDlg->IsVisible() )
	{
		pDetInfoDlg->ResetINFO(m_pLastInfo);
		
		CRect rc;
		pDetInfoDlg->GetComponentRect(&rc);

		CPoint pt = m_pLastInfo->GetUIPosition(rc, ptMouse);
		pDetInfoDlg->MoveComponent(pt);

		pGame->EnableUI(TFRAME_DETAIL_INFO);
	}
}

// ====================================================================================================
ITDetailInfoPtr CTDetailInfoManager::NewFameInst(
	CString	strName,
	DWORD	dwTotalPoint,
	DWORD	dwMonthPoint,
	WORD	wWin,
	WORD	wLose,
	BYTE	bLevel,
	BYTE	bClass,
	const CRect& rc )
{
	return ITDetailInfoPtr( new CTFameRankDetInfo(
		strName,
		dwTotalPoint,
		dwMonthPoint,
		wWin,
		wLose,
		bLevel,
		bClass,
		rc ) );
}
// ====================================================================================================

ITDetailInfoPtr CTDetailInfoManager::NewTournamentPlayerInst(
	CString strName,
	BYTE bWin,
	BYTE bCountry,
	BYTE bClass,
	BYTE bLevel,
	const CRect& rc)
{
	return ITDetailInfoPtr( new CTTournamentPlayerInfo(
		strName,
		bWin,
		bCountry,
		bClass,
		bLevel,
		rc ) );
}

ITDetailInfoPtr CTDetailInfoManager::NewTournamentPlayerInst(
	CString strTitle,
	CString strText,
	int nLine,
	const CRect& rc)
{
	return ITDetailInfoPtr( new CTTournamentPlayerInfo(
		strTitle,
		strText,
		nLine,
		rc ) );
}

ITDetailInfoPtr CTDetailInfoManager::NewMissionInst(
	CString strTerritory,
	CTime timeNext,
	BYTE bCountry,
	BYTE bStatus,
	const CRect& rc )
{
	return ITDetailInfoPtr( new CTMissionDetInfo(
		strTerritory,
		timeNext,
		bCountry,
		bStatus,
		rc ) );
}