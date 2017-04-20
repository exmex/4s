#include "Stdafx.h"
#include "TDetailInfo.h"
#include "TClientGame.h"
#include "TClient.h"
#include "Resource.h"

// ITDetailInfo
// ====================================================================================================
const char ITDetailInfo::m_cTokenMark = '/';
// ====================================================================================================
ITDetailInfo::ITDetailInfo(TDETINFO_TYPE eType, 
						   DWORD dwInfoID, 
						   WORD wImgID, 
						   const CRect& rc)
:	m_eType(eType),
	m_dwInfoID(dwInfoID),
	m_wImageID(wImgID),
	m_rcRef(rc),
	m_bNeedBuild(TRUE),
	m_TitleB("",0),
	m_bLRType(TRUE),
	m_bPriorLeft(FALSE),
	m_bPriorTop(TRUE),
	m_nOutlookStyle(TDETINFO_OUTLOOK_STYLE_TITLE)
{
}
// ----------------------------------------------------------------------------------------------------
ITDetailInfo::~ITDetailInfo()
{
}

CString ITDetailInfo::BuildTimeFormatSTR( DWORD dwTick)
{
	CString strRESULT;
	CString strTIME;

	DWORD dwHOUR = dwTick / 3600000;
	DWORD dwMIN = (dwTick % 3600000) / 60000;
	FLOAT fSEC = FLOAT(dwTick % 60000) / 1000.0f;

	strRESULT.Empty();
	if(dwHOUR)
		strRESULT = CTChart::Format( TSTR_FORMAT_HOUR, dwHOUR);

	if(dwMIN)
	{
		strTIME = CTChart::Format( TSTR_FORMAT_MIN, dwMIN);
		if(!strRESULT.IsEmpty())
			strRESULT += CString(" ");
		strRESULT += strTIME;
	}

	if( fSEC >= 0.01f )
	{
		strTIME = CTChart::Format( TSTR_FORMAT_FSEC, fSEC);
		if(!strRESULT.IsEmpty())
			strRESULT += CString(" ");
		strRESULT += strTIME;
	}

	return strRESULT;
}
// ====================================================================================================
void ITDetailInfo::SetDir(BOOL bLR, BOOL bPriorLeft, BOOL bPriorTop)
{
	m_bLRType = bLR;
	m_bPriorLeft = bPriorLeft;
	m_bPriorTop = bPriorTop;
}
// ----------------------------------------------------------------------------------------------------
CPoint ITDetailInfo::GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse )
{
	if( m_bLRType )
		return _GetUIPosition_LR(rcDetDlg, GetPriorDir(m_bPriorLeft), GetPriorDir(m_bPriorTop));
	else
		return _GetUIPosition_TB(rcDetDlg, GetPriorDir(m_bPriorLeft), GetPriorDir(m_bPriorTop));
}
// Arab Ver.
BOOL ITDetailInfo::GetPriorDir(BOOL bPriorDir)
{
	if(CTNationOption::FLIP_SCREEN)
	{
		switch(bPriorDir)
		{
		case TRUE:
			return FALSE;
		case FALSE:
			return TRUE;
		}
	}

	return bPriorDir;
}
// ----------------------------------------------------------------------------------------------------
const CString& ITDetailInfo::GetTitleText()
{
	return m_TitleB.m_strText;
}
DWORD ITDetailInfo::GetTitleColor()
{
	return m_TitleB.m_dwColor;
}
// ----------------------------------------------------------------------------------------------------
UINT ITDetailInfo::GetLineCount()
{
	return (UINT)m_LinesB.size();
}
const CString& ITDetailInfo::GetLineText(UINT nLine)
{
	return m_LinesB[nLine].m_strText;
}
DWORD ITDetailInfo::GetLineColor(UINT nLine)
{
	return m_LinesB[nLine].m_dwColor;
}
// ====================================================================================================
BOOL ITDetailInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !pTarget ||
		m_eType != pTarget->m_eType ||
		m_dwInfoID != pTarget->m_dwInfoID ||
		m_wImageID != pTarget->m_wImageID )
	{
		return FALSE;
	}

	return TRUE;
}
// ----------------------------------------------------------------------------------------------------
void ITDetailInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	m_LinesB.empty();

	LPTDETAILINFO pINFO = CTChart::FindTDETAILINFO( m_dwInfoID );
	if( pINFO )
	{
		for(BYTE i=0; i<pINFO->m_bCount; ++i)
		{
			switch( _GetLineState(pINFO->m_pTEXT[i],i) )
			{
			case LS_VALID	:
				_AddLine(pINFO->m_pTEXT[i], TDEFAULT_TEXT_COLOR);
				break;

			case LS_TITLE	: 
				_ConvertAndSetTitle(pINFO->m_pTEXT[i], TDEFAULT_TEXT_COLOR);
				break;

			case LS_NEEDCONV: 
				_ConvertAndAddLine(pINFO->m_pTEXT[i], TDEFAULT_TEXT_COLOR); 
				break;
			}
		}
	}

	m_bNeedBuild = FALSE;
}
// ====================================================================================================
ITDetailInfo::LINE_STATE ITDetailInfo::_GetLineState(const CString& strSource, int nIdx)
{
	return LS_VALID;
}
// ----------------------------------------------------------------------------------------------------
void ITDetailInfo::_AddLine(const CString& strSource, DWORD dwColor)
{
	m_LinesB.push_back( Line(strSource, dwColor) ); 
}
// ----------------------------------------------------------------------------------------------------
void ITDetailInfo::_ConvertAndSetTitle(const CString& strSource, DWORD dwColor)
{
	m_TitleB.m_strText = strSource;
	m_TitleB.m_dwColor = dwColor;
}
// ----------------------------------------------------------------------------------------------------
void ITDetailInfo::_ConvertAndAddLine(const CString& strSource, DWORD dwColor)
{
}
// ====================================================================================================
CPoint ITDetailInfo::_GetUIPosition_TB(const CRect& rcDetDlg, BOOL bPriorLeft, BOOL bPriorTop)
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	int nDetDlgW = rcDetDlg.Width();
	int nDetDlgH = rcDetDlg.Height();

	if( (bPriorLeft  && (m_rcRef.right - nDetDlgW + m_rcRef.Width() < 0)) ||
		(!bPriorLeft && (m_rcRef.left + nDetDlgW > pGame->GetScreenX())) )
	{
		bPriorLeft = !bPriorLeft;
	}

	if( (bPriorTop  && (m_rcRef.right - nDetDlgW < 0)) ||
		(!bPriorTop && (m_rcRef.bottom + nDetDlgH > pGame->GetScreenY())) )
	{
		bPriorTop = !bPriorTop;
	}

	CPoint pt;

	if( bPriorLeft )
		pt.x = m_rcRef.right - nDetDlgW;
	else
		pt.x = m_rcRef.left;

	if( bPriorTop )
		pt.y = m_rcRef.top - nDetDlgH;
	else
		pt.y = m_rcRef.bottom;

	return pt;
}
CPoint ITDetailInfo::_GetUIPosition_LR(const CRect& rcDetDlg, BOOL bPriorLeft, BOOL bPriorTop)
{
	CTClientGame* pGame = CTClientGame::GetInstance();

	int nDetDlgW = rcDetDlg.Width();
	int nDetDlgH = rcDetDlg.Height();

	if( (bPriorLeft  && (m_rcRef.left - nDetDlgW < 0)) ||
		(!bPriorLeft && (m_rcRef.right + nDetDlgW > pGame->GetScreenX())) )
	{
		bPriorLeft = !bPriorLeft;
	}

	if( (bPriorTop  && (m_rcRef.top + nDetDlgH > pGame->GetScreenY())) ||
		(!bPriorTop && (m_rcRef.bottom - nDetDlgH < 0)) )
	{
		bPriorTop = !bPriorTop;
	}

	CPoint pt;

	if( bPriorLeft )
		pt.x = m_rcRef.left - nDetDlgW;
	else
		pt.x = m_rcRef.right;

	if( bPriorTop )
		pt.y = m_rcRef.top;
	else
		pt.y = m_rcRef.bottom - nDetDlgH;

	return pt;
}
INT ITDetailInfo::CheckUsingSkin()
{
	return T_INVALID;
}
// ====================================================================================================

// CTStatDetInfo
// ====================================================================================================
CTStatDetInfo::CTStatDetInfo( DWORD dwInfoID, LPTSTATINFO pStatInfo, DWORD dwTitleColor, const CRect& rc)
: ITDetailInfo( TDETINFO_TYPE_STAT, dwInfoID, 0, rc ),
m_pStatInfo( pStatInfo),
m_dwTitleColor( dwTitleColor)
{
}

CTStatDetInfo::~CTStatDetInfo()
{
}

ITDetailInfo::LINE_STATE CTStatDetInfo::_GetLineState(const CString& strSource, int nIdx)
{
	if( nIdx == 0 )
		return LS_TITLE;

	if( strSource.Find(m_cTokenMark) != -1 )
		return LS_NEEDCONV;

	return LS_VALID;
}

BOOL CTStatDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !ITDetailInfo::Compare(pTarget) )
		return FALSE;

	const CTStatDetInfo* pTargetInfo = static_cast<const CTStatDetInfo*>(pTarget);
	return m_pStatInfo == pTargetInfo->m_pStatInfo;
}

void CTStatDetInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	ITDetailInfo::Build();
}

void CTStatDetInfo::_ConvertAndAddLine( const CString& strSource, DWORD dwColor)
{
	CString strLine(strSource);

	BOOL bAddLine = FALSE;
	for(int i=0; i< TINFOTOKEN_STAT_COUNT ; ++i)
	{
		int nFind = strLine.Find( TSTAT_TOKENS[i]);
		if( nFind == -1 )
			continue;

		CString strConv = _GetAttrString( (TINFOTOKEN_STAT) i );
		if( strConv.IsEmpty() )
			continue;

		int nTokLen = TSTAT_TOKENS[i].GetLength();

		while( nFind != -1 )
		{
			CString strNew;
			
			if( nFind )
				strNew = strLine.Left(nFind);

			strNew += strConv;

			int nRight = strLine.GetLength() - nTokLen - nFind;
			if( nRight )
				strNew += strLine.Right(nRight);

			strLine = strNew;
			nFind = strLine.Find( TSTAT_TOKENS[i]);
		}	

		bAddLine = TRUE;
	}

	if( bAddLine )
		m_LinesB.push_back( Line(strLine,dwColor) );
}

CString CTStatDetInfo::_GetAttrString( TINFOTOKEN_STAT eToken)
{
	CString strRET;

	switch( eToken )
	{
		case TINFOTOKEN_STAT_STR: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wSTR ); break;
		case TINFOTOKEN_STAT_DEX: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wDEX ); break;
		case TINFOTOKEN_STAT_CON: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wCON ); break;
		case TINFOTOKEN_STAT_INT: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wINT ); break;
		case TINFOTOKEN_STAT_WIS: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wWIS ); break;
		case TINFOTOKEN_STAT_MEN: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wMEN ); break;
		case TINFOTOKEN_STAT_MIN_PHYSICAL_ATTACK_POWER: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwMinPysAtkPower ); break;
		case TINFOTOKEN_STAT_MAX_PHYSICAL_ATTACK_POWER: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwMaxPysAtkPower ); break;
		case TINFOTOKEN_STAT_PHYSICAL_DEFENCE_POWER: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwPysDefPower ); break;
		case TINFOTOKEN_STAT_MIN_LONG_ATTACK_POWER: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwMinLongAtkPower ); break;
		case TINFOTOKEN_STAT_MAX_LONG_ATTACK_POWER: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwMaxLongAtkPower ); break;
		case TINFOTOKEN_STAT_ATTACK_DELAY_PHYSICAL: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwAttackDelay[ TAD_PHYSICAL ] ); break;
		case TINFOTOKEN_STAT_ATTACK_DELAY_LONG: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwAttackDelay[ TAD_LONG ] ); break;
		case TINFOTOKEN_STAT_ATTACK_DELAY_MAGIC: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwAttackDelay[ TAD_MAGIC ] ); break;
		case TINFOTOKEN_STAT_ATTACK_DELAY_RATE_PHYSICAL: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwAttackDelayRate[ TAD_PHYSICAL ] ); break;
		case TINFOTOKEN_STAT_ATTACK_DELAY_RATE_LONG: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwAttackDelayRate[ TAD_LONG ] ); break;
		case TINFOTOKEN_STAT_ATTACK_DELAY_RATE_MAGIC: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwAttackDelayRate[ TAD_MAGIC ] ); break;
		case TINFOTOKEN_STAT_ATTACK_LEVEL: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wAtkLevel ); break;
		case TINFOTOKEN_STAT_DEFENCE_LEVEL: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wDefLevel ); break;
		case TINFOTOKEN_STAT_PHYSICAL_CRITICAL_PROB: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_bPysCriticalProb ); break;
		case TINFOTOKEN_STAT_MIN_MAGIC_ATTACK_POWER: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwMinMagicAP ); break;
		case TINFOTOKEN_STAT_MAX_MAGIC_ATTACK_POWER: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwMaxMagicAP ); break;
		case TINFOTOKEN_STAT_MAGIC_DEFENCE_POWER: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_dwMagicDefPower ); break;
		case TINFOTOKEN_STAT_MAGIC_ATTACK_LEVEL: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wMagicAtkLevel ); break;
		case TINFOTOKEN_STAT_MAGIC_DEFENCE_LEVEL: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wMagicDefLevel ); break;
		case TINFOTOKEN_STAT_CHARGE_SPEED: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_bChargeSpeed ); break;
		case TINFOTOKEN_STAT_CHARGE_PROB: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_bChargeProb ); break;
		case TINFOTOKEN_STAT_MAGIC_CRITICAL_PROB: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_bMagicCriticalProb ); break;
		case TINFOTOKEN_STAT_SKILL_POINT: strRET = CTChart::Format( TSTR_FMT_BASIC_STAT, m_pStatInfo->m_wSkillPoint ); break;
		default: strRET.Empty(); break;
	}

	return strRET;
}

// CTNorDetInfo
// ====================================================================================================
CTNorDetInfo::CTNorDetInfo(const CString& strTitle, 
						   DWORD dwInfoID, 
						   WORD wImgID, 
						   const CRect& rc)
:	ITDetailInfo(TDETINFO_TYPE_NOR, dwInfoID, wImgID, rc),
	m_strTITLE(strTitle)
{
}
// ----------------------------------------------------------------------------------------------------
CTNorDetInfo::~CTNorDetInfo()
{
}
// ====================================================================================================
BOOL CTNorDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !ITDetailInfo::Compare(pTarget) )
		return FALSE;

	const CTNorDetInfo* pTargetInfo = static_cast<const CTNorDetInfo*>(pTarget);
	return m_strTITLE == pTargetInfo->m_strTITLE;
}
// ----------------------------------------------------------------------------------------------------
void CTNorDetInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	m_TitleB.m_strText = m_strTITLE;
	m_TitleB.m_dwColor = TDEFAULT_TEXT_COLOR;
	
	ITDetailInfo::Build();
}
// ====================================================================================================
ITDetailInfo::LINE_STATE CTNorDetInfo::_GetLineState(const CString& strSource, int nIdx)
{
	if( nIdx == 0 && m_TitleB.m_strText.IsEmpty() )
		return LS_TITLE;
	else 
		return LS_VALID;
}
// ====================================================================================================



// CTItemDetInfo
// ====================================================================================================
const CString CTItemDetInfo::TITEM_TOKENS[TINFOTOKEN_ITEM_COUNT] = {
	CString(_T("/ItemMinAP")),			// TINFOTOKEN_ITEM_ITEMMINAP
	CString(_T("/ItemMaxAP")),			// TINFOTOKEN_ITEM_ITEMMAXAP
	CString(_T("/ItemAPSec")),			// TINFOTOKEN_ITEM_ITEMAPSEC
	CString(_T("/ItemDP")),				// TINFOTOKEN_ITEM_ITEMDP
	CString(_T("/ItemMinMAP")),			// TINFOTOKEN_ITEM_ITEMMINMAP
	CString(_T("/ItemMaxMAP")),			// TINFOTOKEN_ITEM_ITEMMAXMAP
	CString(_T("/ItemMAPSec")),			// TINFOTOKEN_ITEM_ITEMMAPSEC
	CString(_T("/ItemMDP")),			// TINFOTOKEN_ITEM_ITEMMDP
	CString(_T("/ItemBlock")),			// TINFOTOKEN_ITEM_ITEMBLOCK
	CString(_T("/GradeLevel")),			// TINFOTOKEN_ITEM_ATTRLEVEL
	CString(_T("/DuraMin")),			// TINFOTOKEN_ITEM_CURDURA
	CString(_T("/DuraMax")),			// TINFOTOKEN_ITEM_MAXDURA
	CString(_T("/RefineMin")),			// TINFOTOKEN_ITEM_CURREFI
	CString(_T("/RefineMax")),			// TINFOTOKEN_ITEM_MAXREFI
	CString(_T("/SellPrice")),			// TINFOTOKEN_ITEM_SELLPRICE
	CString(_T("/EndTime")),			// TINFOTOKEN_ITEM_ENDTIME
	CString(_T("/NeedsLevel")),		// TINFOTOKEN_ITEM_NEEDSLEVEL
};
// ----------------------------------------------------------------------------------------------------
const CString CTStatDetInfo::TSTAT_TOKENS[TINFOTOKEN_STAT_COUNT] = {
	CString(_T("/Str")),		//TINFOTOKEN_STAT_STR,
	CString(_T("/Dex")),		//TINFOTOKEN_STAT_DEX,
	CString(_T("/Con")),		//TINFOTOKEN_STAT_CON,
	CString(_T("/Int")),		//TINFOTOKEN_STAT_INT,
	CString(_T("/Wis")),		//TINFOTOKEN_STAT_WIS,
	CString(_T("/Men")),		//TINFOTOKEN_STAT_MEN,
	CString(_T("/MinPhyAP")),		//TINFOTOKEN_STAT_MIN_PHYSICAL_ATTACK_POWER,
	CString(_T("/MaxPhyAP")),		//TINFOTOKEN_STAT_MAX_PHYSICAL_ATTACK_POWER,
	CString(_T("/PhyDP")),		//TINFOTOKEN_STAT_PHYSICAL_DEFENCE_POWER,
	CString(_T("/MinLAP")),		//TINFOTOKEN_STAT_MIN_LONG_ATTACK_POWER,
	CString(_T("/MaxLAP")),		//TINFOTOKEN_STAT_MAX_LONG_ATTACK_POWER,
	CString(_T("/AtkDelayPhy")),		//TINFOTOKEN_STAT_ATTACK_DELAY_PHYSICAL,
	CString(_T("/AtkDelayLong")),		//TINFOTOKEN_STAT_ATTACK_DELAY_LONG,
	CString(_T("/AtkDelayMagic")),		//TINFOTOKEN_STAT_ATTACK_DELAY_MAGIC,
	CString(_T("/AtkDRatePhy")),		//TINFOTOKEN_STAT_ATTACK_DELAY_RATE_PHYSICAL,
	CString(_T("/AtkDRateLong")),		//TINFOTOKEN_STAT_ATTACK_DELAY_RATE_LONG,
	CString(_T("/AtkDRateMagic")),		//TINFOTOKEN_STAT_ATTACK_DELAY_RATE_MAGIC,
	CString(_T("/AtkLv")),		//TINFOTOKEN_STAT_ATTACK_LEVEL,
	CString(_T("/DefLv")),		//TINFOTOKEN_STAT_DEFENCE_LEVEL,
	CString(_T("/PhyCriticalProb")),		//TINFOTOKEN_STAT_PHYSICAL_CRITICAL_PROB,
	CString(_T("/MinMagicAP")),		//TINFOTOKEN_STAT_MIN_MAGIC_ATTACK_POWER,
	CString(_T("/MaxMagicAP")),		//TINFOTOKEN_STAT_MIN_MAGIC_ATTACK_POWER,
	CString(_T("/MagicDP")),		//TINFOTOKEN_STAT_MAGIC_DEFENCE_POWER,
	CString(_T("/MagicAtkLv")),		//TINFOTOKEN_STAT_MAGIC_ATTACK_LEVEL,
	CString(_T("/MagicDefLv")),		//TINFOTOKEN_STAT_MAGIC_DEFENCE_LEVEL,
	CString(_T("/ChargeSpeed")),		//TINFOTOKEN_STAT_CHARGE_SPEED,
	CString(_T("/ChargeProb")),		//TINFOTOKEN_STAT_CHARGE_PROB,
	CString(_T("/MagicCriticalProb")),		//TINFOTOKEN_STAT_MAGIC_CRITICAL_PROB,
	CString(_T("/SkillPoint")),					//TINFOTOKEN_STAT_SKILL_POINT,
};
// ----------------------------------------------------------------------------------------------------
const CString CTItemDetInfo::TITEM_TYPE_TOKENS[TINFO_ITEMTYPE_COUNT] = {
	CString(_T("/ItemNormal")),			// TINFO_ITEMTYPE_ITEMNORMAL
	CString(_T("/ItemGrade")),			// TINFO_ITEMTYPE_ITEMGRADE
	CString(_T("/ItemMagic")),			// TINFO_ITEMTYPE_ITEMMAGIC
	CString(_T("/ItemSet")),			// TINFO_ITEMTYPE_ITEMSET
	CString(_T("/ItemRare")),			// TINFO_ITEMTYPE_ITEMRARE
	CString(_T("/ItemUnique"))			// TINFO_ITEMTYPE_ITEMUNIQUE
};
// ----------------------------------------------------------------------------------------------------
const DWORD CTItemDetInfo::TITEMATTR_NAMES[TATTRLEVELNAME_MAX] = {
	TSTR_FMT_ITEMATTRLEVEL_0,
	TSTR_FMT_ITEMATTRLEVEL_1,
	TSTR_FMT_ITEMATTRLEVEL_2,
	TSTR_FMT_ITEMATTRLEVEL_3,
	TSTR_FMT_ITEMATTRLEVEL_4,
	TSTR_FMT_ITEMATTRLEVEL_5,
	TSTR_FMT_ITEMATTRLEVEL_6,
	TSTR_FMT_ITEMATTRLEVEL_7,
	TSTR_FMT_ITEMATTRLEVEL_8,
	TSTR_FMT_ITEMATTRLEVEL_9,
	TSTR_FMT_ITEMATTRLEVEL_10,
	TSTR_FMT_ITEMATTRLEVEL_11,
	TSTR_FMT_ITEMATTRLEVEL_12,
};
// ----------------------------------------------------------------------------------------------------
const CString CTItemDetInfo::TITEM_ATTRNAME_TOKEN(_T("/GradeName"));
// ====================================================================================================
CTItemDetInfo::CTItemDetInfo(LPTITEM pItemTemp, 
							 const CRect& rc)
:	ITDetailInfo(TDETINFO_TYPE_ITEM, 
				 pItemTemp->m_dwInfoID, 
				 0, 
				 rc),
	m_pItemTemp(pItemTemp),
	m_wAttrID(0),
	m_dwSellPrice(0)
{
}
// ----------------------------------------------------------------------------------------------------
CTItemDetInfo::~CTItemDetInfo()
{
}
// ====================================================================================================
BOOL CTItemDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !pTarget ||
		m_eType != pTarget->GetType() ||
		m_dwInfoID != pTarget->GetInfoID())
	{
		return FALSE;
	}

	const CTItemDetInfo* pTargetInfo = static_cast<const CTItemDetInfo*>(pTarget);
	return m_pItemTemp->m_wItemID == pTargetInfo->m_pItemTemp->m_wItemID;
}
// ----------------------------------------------------------------------------------------------------
void CTItemDetInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	_BuildItemIcon();
	_BuildAttrID();
	_BuildAttrNameAndLevel();

	m_TitleB.m_strText = m_pItemTemp->m_strNAME;
	m_TitleB.m_dwColor = TDEFAULT_TEXT_COLOR;

	ITDetailInfo::Build();
}
// ====================================================================================================
ITDetailInfo::LINE_STATE CTItemDetInfo::_GetLineState(const CString& strSource, int nIdx)
{
	for(int i=0; i<TINFO_ITEMTYPE_COUNT; ++i)
	{
		if( strSource.Find( TITEM_TYPE_TOKENS[i] ) != -1 )
		{
			if( i == TINFO_ITEMTYPE_ITEMNORMAL )
				return LS_TITLE;
			else 
				return LS_INVALID;
		}
	}

	if( strSource.Find(m_cTokenMark) != -1 )
		return LS_NEEDCONV;

	return LS_VALID;
}
// ----------------------------------------------------------------------------------------------------
void CTItemDetInfo::_ConvertAndSetTitle(const CString& strSource, DWORD dwColor)
{
	m_TitleB.m_dwColor = dwColor;

	m_TitleB.m_strText = strSource;
	m_TitleB.m_strText.Replace(TITEM_TYPE_TOKENS[TINFO_ITEMTYPE_ITEMNORMAL], "");
	m_TitleB.m_strText.Replace(TITEM_ATTRNAME_TOKEN, m_strAttrName);
}
// ----------------------------------------------------------------------------------------------------
void CTItemDetInfo::_ConvertAndAddLine(const CString& strSource, DWORD dwColor)
{
	CString strLine(strSource);

	BOOL bAddLine = FALSE;
	for(int i=0; i<TINFOTOKEN_ITEM_COUNT; ++i)
	{
		int nFind = strLine.Find(TITEM_TOKENS[i]);
		if( nFind == -1 )
			continue;

		CString strConv = _GetAttrString( (TINFOTOKEN_ITEM) i );
		if( strConv.IsEmpty() )
			continue;

		dwColor = _GetAttrColor( (TINFOTOKEN_ITEM) i, dwColor );

		int nTokLen = TITEM_TOKENS[i].GetLength();

		while( nFind != -1 )
		{
			CString strNew;
			
			if( nFind )
				strNew = strLine.Left(nFind);

			strNew += strConv;

			int nRight = strLine.GetLength() - nTokLen - nFind;
			if( nRight )
				strNew += strLine.Right(nRight);

			strLine = strNew;
			nFind = strLine.Find(TITEM_TOKENS[i]);
		}	

		bAddLine = TRUE;
	}

	if( bAddLine )
		m_LinesB.push_back( Line(strLine,dwColor) ); 
}
// ----------------------------------------------------------------------------------------------------
void CTItemDetInfo::_BuildItemIcon()
{
	LPTITEMVISUAL pTVISUAL = CTClientItem::GetDefaultVisual(m_pItemTemp);
	m_wImageID = pTVISUAL? pTVISUAL->m_wIcon: 0;
}
// ----------------------------------------------------------------------------------------------------
void CTItemDetInfo::_BuildAttrID()
{
	m_wAttrID = m_pItemTemp->m_wAttrID;
}
// ----------------------------------------------------------------------------------------------------
CString CTItemDetInfo::_GetAttrString(TINFOTOKEN_ITEM eToken)
{
	CString strResult;

	LPTITEMATTR pTITEMATTR = CTChart::FindTITEMATTR( m_wAttrID );
	if( !pTITEMATTR )
		return strResult;

	const TITEMATTR& itemAttr = *pTITEMATTR;
	
	switch(eToken)
	{
	case TINFOTOKEN_ITEM_ITEMMINAP:
		{
			if( 0 == itemAttr.m_wMinAP && 0 == itemAttr.m_wMaxAP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, itemAttr.m_wMinAP); 
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMAXAP:
		{
			if( 0 == itemAttr.m_wMinAP && 0 == itemAttr.m_wMaxAP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, itemAttr.m_wMaxAP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMDP	:
		{
			if( 0 == itemAttr.m_wDP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, itemAttr.m_wDP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMINMAP:
		{
			if( 0 == itemAttr.m_wMinMAP && 0 == itemAttr.m_wMaxMAP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, itemAttr.m_wMinMAP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMAXMAP:
		{
			if( 0 == itemAttr.m_wMinMAP && 0 == itemAttr.m_wMaxMAP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, itemAttr.m_wMaxMAP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMDP:
		{
			if( 0 == itemAttr.m_wMDP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, itemAttr.m_wMDP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMBLOCK:
		{
			if( 0 == itemAttr.m_bBlockProb )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, itemAttr.m_bBlockProb);
		}
		break;

	case TINFOTOKEN_ITEM_ATTRLEVEL:
		{
			if( 0 == m_wAttrLevel )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_wAttrLevel);
		}
		break;

	case TINFOTOKEN_ITEM_CURDURA:
		{
			if( 0 == m_pItemTemp->m_dwDuraMax )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pItemTemp->m_dwDuraMax);
		}
		break;

	case TINFOTOKEN_ITEM_MAXDURA:
		{
			if( 0 == m_pItemTemp->m_dwDuraMax )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pItemTemp->m_dwDuraMax);
		}
		break;

	case TINFOTOKEN_ITEM_CURREFI:
		{
			if( 0 == m_pItemTemp->m_bRefineMax )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, 0);
		}
		break;

	case TINFOTOKEN_ITEM_MAXREFI:
		{
			if( 0 == m_pItemTemp->m_bRefineMax )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pItemTemp->m_bRefineMax);
		}
		break;

	case TINFOTOKEN_ITEM_SELLPRICE:
		{
			if( 0 == m_dwSellPrice )
				return _T("");

			strResult = CTClientGame::MakeMoneyStr(m_dwSellPrice);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMAPSEC	:
		{
			FLOAT fAPS = (itemAttr.m_wMinAP+itemAttr.m_wMaxAP) * 0.5f *
						 ((FLOAT)itemAttr.m_bSpeedInc * 0.01f);

			if( 0.0f == fAPS )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_FLOAT_NUMBER, fAPS);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMAPSEC	:
		{
			FLOAT fMPS = (itemAttr.m_wMinMAP+itemAttr.m_wMaxMAP) * 0.5f *
						 ((FLOAT)itemAttr.m_bSpeedInc * 0.01f);

			if( 0.0f == fMPS )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_FLOAT_NUMBER, fMPS);
		}
		break;

	case TINFOTOKEN_ITEM_NEEDSLEVEL:
		{
			if( 0 == m_pItemTemp->m_bLevel )
				return _T("");

			strResult.Format( "%d", m_pItemTemp->m_bLevel );
		}
		break;
	}

	return strResult;
}
// ----------------------------------------------------------------------------------------------------
DWORD CTItemDetInfo::_GetAttrColor(TINFOTOKEN_ITEM eToken, DWORD dwDefaultColor )
{
	return dwDefaultColor;
}
// ====================================================================================================
void CTItemDetInfo::_BuildAttrNameAndLevel()
{
	m_strAttrName.Empty();
	m_wAttrLevel = m_pItemTemp->m_bLevel;

	LPTITEMATTR pTITEMATTR = CTChart::FindTITEMATTR( m_wAttrID );

	if( pTITEMATTR )
	{
		const TITEMATTR& itemAttr = *pTITEMATTR;
		
		if( m_wAttrLevel < itemAttr.m_bGrade )
			m_wAttrLevel = itemAttr.m_bGrade;

        BYTE bGradeIdx = min((m_wAttrLevel-1)/TATTRLEVELNAME_UNIT, TATTRLEVELNAME_MAX-1);
		m_strAttrName = CTChart::LoadString( (TSTRING) TITEMATTR_NAMES[bGradeIdx] );
	}

	m_dwSellPrice = CTClientItem::CalcPrice(m_pItemTemp, m_wAttrLevel) / 4;
	m_wAttrLevel  = CTClientItem::CalcViewAttrLevel(m_wAttrLevel);
}
// ====================================================================================================


// CTItemInstDetInfo
// ====================================================================================================
CTItemInstDetInfo::CTItemInstDetInfo(TDETINFO_TYPE eType,
									 CTClientItem* pItemInst, 
									 const CRect& rc)
:	CTItemDetInfo(pItemInst->GetTITEM(), rc)
{
	m_eType = eType;

	m_pItemInst = new CTClientItem;
	*m_pItemInst = *pItemInst;
}
// ----------------------------------------------------------------------------------------------------
CTItemInstDetInfo::~CTItemInstDetInfo()
{
	delete m_pItemInst;
}
// ====================================================================================================
BOOL CTItemInstDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !CTItemDetInfo::Compare(pTarget) )
		return FALSE;

	CTClientItem* pTargetItem = static_cast<const CTItemInstDetInfo*>(pTarget)->GetItemInst();
	if( !pTargetItem )
		return NULL;

	return ( *m_pItemInst == *pTargetItem );
}
// ====================================================================================================
void CTItemInstDetInfo::_BuildItemIcon()
{
	LPTITEMVISUAL pTVISUAL = m_pItemInst->GetVisual();
	m_wImageID = pTVISUAL? pTVISUAL->m_wIcon: 0;
}
// ----------------------------------------------------------------------------------------------------
void CTItemInstDetInfo::_BuildAttrID()
{
	m_wAttrID = m_pItemInst->GetAttrID();
}
// ----------------------------------------------------------------------------------------------------
CString CTItemInstDetInfo::_GetAttrString(TINFOTOKEN_ITEM eToken)
{
	CString strResult;

	switch(eToken)
	{
	case TINFOTOKEN_ITEM_CURDURA:
		{
			if( 0 == m_pItemInst->GetDuraCurrent() && 0 == m_pItemInst->GetDuraMax() )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pItemInst->GetDuraCurrent());
		}
		break;

	case TINFOTOKEN_ITEM_MAXDURA:
		{
			if( 0 == m_pItemInst->GetDuraCurrent() && 0 == m_pItemInst->GetDuraMax() )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pItemInst->GetDuraMax());
		}
		break;

	case TINFOTOKEN_ITEM_CURREFI:
		{
			if( 0 == m_pItemInst->GetRefineCurrent() && 0 == m_pItemInst->GetRefineMax() )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pItemInst->GetRefineCurrent());
		}
		break;

	case TINFOTOKEN_ITEM_MAXREFI:
		{
			if( 0 == m_pItemInst->GetRefineCurrent() && 0 == m_pItemInst->GetRefineMax() )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pItemInst->GetRefineMax());
		}
		break;

	case TINFOTOKEN_ITEM_ENDTIME:
		{
			if( m_pItemInst->GetEndTime() != 0 )
			{
				CTime endTime = CTime(m_pItemInst->GetEndTime());
	            
				if( endTime > CTClientApp::m_dCurDate)
				{
					CTimeSpan timeSpan = endTime-CTClientApp::m_dCurDate;

					DWORD dwDays = (DWORD) timeSpan.GetDays();
					DWORD dwHour = (DWORD) timeSpan.GetHours();
					DWORD dwMin = (DWORD) timeSpan.GetMinutes();
					DWORD dwSec = (DWORD) timeSpan.GetSeconds();

					strResult = CTChart::LoadString( TSTR_REMAIN_PERIOD );

					if( dwDays != 0 )
						strResult += CTChart::Format( TSTR_FMT_DAYS, dwDays);

					if( dwHour != 0 )
					{
						strResult += _T(" ");
						strResult += CTChart::Format( TSTR_FORMAT_HOUR, dwHour);
					}

					if( dwMin != 0 && dwDays == 0 )
					{
						strResult += _T(" ");
						strResult += CTChart::Format( TSTR_FORMAT_MIN, dwMin);
					}

					if( dwSec != 0 && dwDays == 0 )
					{
						strResult += _T(" ");
						//strResult += CTChart::Format( TSTR_FORMAT_SEC, dwSec);
					}
				}
				else
				{
					strResult = CTChart::LoadString(TSTR_FMT_PET_PERIOD_FIRED);
				}
			}
		}
		break;
	case TINFOTOKEN_ITEM_NEEDSLEVEL:
		{
			if( m_pItemInst->GetELD() > 0 )
				strResult.Format( "%d", m_pItemInst->GetTITEM()->m_bLevel - m_pItemInst->GetELD() );
			else
				strResult.Format( "%d", m_pItemInst->GetTITEM()->m_bLevel );
		}
		break;

	default:
		return CTItemDetInfo::_GetAttrString(eToken);
	}

	return strResult;
}
// ----------------------------------------------------------------------------------------------------
DWORD CTItemInstDetInfo::_GetAttrColor(TINFOTOKEN_ITEM eToken, DWORD dwDefaultColor )
{
	switch( eToken )
	{
	case TINFOTOKEN_ITEM_NEEDSLEVEL:
		{
			if( m_pItemInst->GetELD() > 0 )
				return D3DCOLOR_ARGB(255, 68,169,255);
		}
		break;
	}

	return CTItemDetInfo::_GetAttrColor( eToken, dwDefaultColor );
}

void CTItemInstDetInfo::_ConvertAndSetTitle(const CString& strSource, DWORD dwColor)
{
	CTItemDetInfo::_ConvertAndSetTitle( strSource, dwColor );

	if( m_pItemInst->GetWrap() )
		_AddLine( CTChart::LoadString( TSTR_WRAP_FOOTER ), D3DCOLOR_XRGB( 255, 100, 100 ) );
}
INT CTItemInstDetInfo::CheckUsingSkin()
{
	DWORD qt = m_pItemInst->GetQuality();

	if ( m_pItemInst->GetWrap() )
		return TITEM_INDEX_WRAP_SKIN;
	else if ( qt & TITEM_QUALITY_GRADE )
		return TITEM_INDEX_GRADE_SKIN + m_pItemInst->GetGrade();
	else if ( qt & TITEM_QUALITY_RARE )
		return TITEM_INDEX_RARE_SKIN;
	else if ( qt & TITEM_QUALITY_MAGIC )
		return TITEM_INDEX_MAGIC_SKIN;
	
	return T_INVALID;
}
// ====================================================================================================

// CTSealedItemDetInfo
// ====================================================================================================
CTSealedItemDetInfo::CTSealedItemDetInfo(CTClientItem* pItemInst, 
										 const CRect& rc)
:	CTItemInstDetInfo(TDETINFO_TYPE_SEALEDITEM, pItemInst, rc)
{
}
// ----------------------------------------------------------------------------------------------------
CTSealedItemDetInfo::~CTSealedItemDetInfo()
{
}
// ====================================================================================================
void CTSealedItemDetInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	_BuildItemIcon();

	m_TitleB.m_strText = m_pItemTemp->m_strNAME;
	m_TitleB.m_dwColor = TDEFAULT_TEXT_COLOR;

	m_LinesB.empty();

	LPTDETAILINFO pINFO = CTChart::FindTDETAILINFO(m_dwInfoID);
	if( pINFO )
	{
		for(BYTE i=0; i<pINFO->m_bCount; ++i)
		{
			switch( _GetLineState(pINFO->m_pTEXT[i],i) )
			{
			case LS_TITLE	: 
				_ConvertAndSetTitle(pINFO->m_pTEXT[i], TDEFAULT_TEXT_COLOR);
				break;
			}
		}
	}

	CString strMSG(" ");
	_AddLine(strMSG, TDEFAULT_TEXT_COLOR);
	
	strMSG = CTChart::LoadString( TSTR_DETMSG_SEALEDITEM);
	_AddLine(strMSG, TDEFAULT_TEXT_COLOR);

	m_bNeedBuild = FALSE;
}
// ====================================================================================================


// CTMagicItemDetInfo
// ====================================================================================================
const CString CTOptionItemDetInfo::TMAGICOPTION_TOKENS[TINFOTOKEN_MAGICOPTION_COUNT] = {
	CString(_T("/OptionName1")),		// TINFOTOKEN_MAGICOPTION1
	CString(_T("/OptionName2")),		// TINFOTOKEN_MAGICOPTION2
	CString(_T("/OptionName3")),		// TINFOTOKEN_MAGICOPTION3
	CString(_T("/OptionName4")),		// TINFOTOKEN_MAGICOPTION4
	CString(_T("/OptionName5")),		// TINFOTOKEN_MAGICOPTION5
	CString(_T("/OptionName6")),		// TINFOTOKEN_MAGICOPTION6
};
// ----------------------------------------------------------------------------------------------------
const CString CTOptionItemDetInfo::TMAGICDATA_TOKENS[TINFOTOKEN_MAGICDATA_COUNT] = {
	CString(_T("/OptionData1")),		// TINFOTOKEN_MAGICDATA1
	CString(_T("/OptionData2")),		// TINFOTOKEN_MAGICDATA2
	CString(_T("/OptionData3")),		// TINFOTOKEN_MAGICDATA3
	CString(_T("/OptionData4")),		// TINFOTOKEN_MAGICDATA4
	CString(_T("/OptionData5")),		// TINFOTOKEN_MAGICDATA5
	CString(_T("/OptionData6")),		// TINFOTOKEN_MAGICDATA6
};
// ----------------------------------------------------------------------------------------------------
const CString CTOptionItemDetInfo::TMAGIC_TOKENS[TINFOTOKEN_MAGIC_COUNT] = {
	CString(_T("/OptionBonus")),		// TINFOTOKEN_MAGIC_TITLE
	CString(_T("/OptionLevel")),		// TINFOTOKEN_MAGIC_LEVEL
};
// ----------------------------------------------------------------------------------------------------
const CString CTOptionItemDetInfo::TMAGICLEVEL_NAMES[TMAGICLEVEL_MAX] = {
	CString(_T("F")),
	CString(_T("E")),
	CString(_T("D")),
	CString(_T("C")),
	CString(_T("B")),
	CString(_T("A")),
	CString(_T("S")),
	CString(_T("SS")),
	CString(_T("SSS")),
};
// ====================================================================================================
const CString CTOptionItemDetInfo::TGRADEITEM_TOKEN(_T("/Grade"));
// ====================================================================================================
CTOptionItemDetInfo::CTOptionItemDetInfo(CTClientItem* pItemInst, 
									   const CRect& rc)
:	CTItemInstDetInfo(TDETINFO_TYPE_OPTIONITEM, pItemInst, rc),
	m_bProcOption(FALSE)
{
}
// ----------------------------------------------------------------------------------------------------
CTOptionItemDetInfo::~CTOptionItemDetInfo()
{
}
// ====================================================================================================
void CTOptionItemDetInfo::Build()
{
	m_bProcOption = FALSE;

	CTItemInstDetInfo::Build();
}
// ====================================================================================================
ITDetailInfo::LINE_STATE CTOptionItemDetInfo::_GetLineState(const CString& strSource, int nIdx)
{
	DWORD q = m_pItemInst->GetQuality();

	for(int i=0; i<TINFO_ITEMTYPE_COUNT; ++i)
	{
		if( strSource.Find( TITEM_TYPE_TOKENS[i] ) != -1 )
		{
			if( q&TITEM_QUALITY_GRADE )
			{
				if( i == TINFO_ITEMTYPE_ITEMGRADE )
					return LS_TITLE;
			}
			else if( q&TITEM_QUALITY_MAGIC )
			{
				if( i == TINFO_ITEMTYPE_ITEMMAGIC )
					return LS_TITLE;
			}
			else if( q&TITEM_QUALITY_RARE )
			{
				if( i == TINFO_ITEMTYPE_ITEMRARE )
					return LS_TITLE;
			}
			else if( q&TITEM_QUALITY_NORMAL )
			{
				if( i == TINFO_ITEMTYPE_ITEMNORMAL )
					return LS_TITLE;
			}
			
			return LS_INVALID;
		}
	}

	if( !m_bProcOption && strSource.Find(TMAGIC_TOKENS[TINFOTOKEN_MAGIC_TITLE]) != -1 )
		m_bProcOption = TRUE;

    if( m_bProcOption && m_pItemInst->IsEmptyMagicInfos() )
		return LS_INVALID;
	else if( strSource.Find(m_cTokenMark) != -1 )
		return LS_NEEDCONV;
	else
		return LS_VALID;
}
// ====================================================================================================
void CTOptionItemDetInfo::_ConvertAndSetTitle(const CString& strSource, DWORD dwColor)
{
	m_TitleB.m_dwColor = m_pItemInst->GetQualityColor();

	CString strGrade;
	strGrade = CTChart::Format( TSTR_FMT_NUMBER, m_pItemInst->GetGrade());

	m_TitleB.m_strText = strSource;
	m_TitleB.m_strText.Replace(TITEM_ATTRNAME_TOKEN, m_strAttrName);
    m_TitleB.m_strText.Replace(TGRADEITEM_TOKEN, strGrade);
	m_TitleB.m_strText.Replace(TITEM_TYPE_TOKENS[TINFO_ITEMTYPE_ITEMGRADE], "");
	m_TitleB.m_strText.Replace(TITEM_TYPE_TOKENS[TINFO_ITEMTYPE_ITEMMAGIC], "");
	m_TitleB.m_strText.Replace(TITEM_TYPE_TOKENS[TINFO_ITEMTYPE_ITEMRARE], "");

	if( m_pItemInst->GetWrap() )
		_AddLine( CTChart::LoadString( TSTR_WRAP_FOOTER ), D3DCOLOR_XRGB( 255, 100, 100 ) );
}
// ----------------------------------------------------------------------------------------------------
void CTOptionItemDetInfo::_ConvertAndAddLine(const CString& strSource, DWORD dwColor)
{
	CString strLine(strSource);

	if( m_bProcOption )
	{
		for(int i=0; i<TINFOTOKEN_MAGICOPTION_COUNT; ++i)
		{
			int nFind = strLine.Find(TMAGICOPTION_TOKENS[i]);
			if( nFind == -1 )
				continue;

			CString strOption, strData; 
			if( !_GetMagicOptionString(i, strOption,strData) )
				continue;

			strLine.Replace(TMAGICOPTION_TOKENS[i], strOption);
			strLine.Replace(TMAGICDATA_TOKENS[i], strData);
			
			_AddLine(strLine, m_pItemInst->GetMagicColor());
			return;
		}

		for(int i=0; i<TINFOTOKEN_MAGIC_COUNT; ++i)
		{
			int nFind = strLine.Find(TMAGIC_TOKENS[i]);
			if( nFind == -1 )
				continue;

			CString strREPLACE = _GetMagicString(i);
			if( !strREPLACE.IsEmpty() )
				strLine.Replace(TMAGIC_TOKENS[i], strREPLACE);

			_AddLine(strLine, m_pItemInst->GetMagicColor());
			return;
		}
	}
	else
	{
		if( _IsMagicLine(strLine) )
			dwColor = m_pItemInst->GetMagicColor();
			
		CTItemInstDetInfo::_ConvertAndAddLine(strLine, dwColor);
	}
}
// ----------------------------------------------------------------------------------------------------
CString CTOptionItemDetInfo::_GetAttrString(TINFOTOKEN_ITEM eToken)
{
	CString strResult;
	
	LPTITEMATTR pItemAttr = m_pItemInst->GetItemAttr();

	switch(eToken)
	{
	case TINFOTOKEN_ITEM_ITEMMINAP:
		{
			if( 0 == pItemAttr->m_wMinAP && 0 == pItemAttr->m_wMaxAP)
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, pItemAttr->m_wMinAP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMAXAP:
		{
			if( 0 == pItemAttr->m_wMinAP && 0 == pItemAttr->m_wMaxAP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, pItemAttr->m_wMaxAP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMDP	:
		{
			if( 0 == pItemAttr->m_wDP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, pItemAttr->m_wDP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMINMAP:
		{
			if( 0 == pItemAttr->m_wMinMAP && 0 == pItemAttr->m_wMaxMAP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, pItemAttr->m_wMinMAP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMAXMAP:
		{
			if( 0 == pItemAttr->m_wMinMAP && 0 == pItemAttr->m_wMaxMAP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, pItemAttr->m_wMaxMAP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMDP:
		{
			if( 0 == pItemAttr->m_wMDP )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, pItemAttr->m_wMDP);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMBLOCK:
		{
			if( 0 == pItemAttr->m_bBlockProb )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, pItemAttr->m_bBlockProb);
		}
		break;

	case TINFOTOKEN_ITEM_ATTRLEVEL:
		{
			if( 0 == m_pItemInst->GetViewAttrLevel() )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pItemInst->GetViewAttrLevel());
		}
		break;

	case TINFOTOKEN_ITEM_ITEMAPSEC	:
		{
			FLOAT fAPS = (pItemAttr->m_wMinAP+pItemAttr->m_wMaxAP) * 0.5f *
						((FLOAT)pItemAttr->m_bSpeedInc * 0.01f);

			if( 0.0f == fAPS )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_FLOAT_NUMBER, fAPS);
		}
		break;

	case TINFOTOKEN_ITEM_ITEMMAPSEC	:
		{
			FLOAT fMPS = (pItemAttr->m_wMinMAP+pItemAttr->m_wMaxMAP) * 0.5f *
						((FLOAT)pItemAttr->m_bSpeedInc * 0.01f);

			if( 0.0f == fMPS )
				return _T("");

			strResult = CTChart::Format( TSTR_FMT_FLOAT_NUMBER, fMPS);
		}
		break;

	default:
		return CTItemInstDetInfo::_GetAttrString(eToken);
	}

	return strResult;
}
// ====================================================================================================
BOOL CTOptionItemDetInfo::_GetMagicOptionString(int nIdx, CString& outOption, CString& outData)
{
	LPVTMAGICOPTION pMagicInfos = m_pItemInst->GetMagicInfos();

	if( nIdx >= 0 && nIdx < pMagicInfos->size() )
	{
		const TMAGICOPTION& mo = (*pMagicInfos)[nIdx];

		outOption = mo.m_strOption;
		outData = CTChart::Format( TSTR_FMT_ITEMMAGIC, '+', mo.m_wValue);

		return TRUE;
	}

	return FALSE;
}
// ----------------------------------------------------------------------------------------------------
CString CTOptionItemDetInfo::_GetMagicString(int nIdx)
{
	CString strRESULT;

	switch(nIdx)
	{
	case TINFOTOKEN_MAGIC_TITLE	:
		{
			strRESULT = CTChart::LoadString( TSTR_FMT_ITEMMAGIC_TITLE);
		}
		break;
	case TINFOTOKEN_MAGIC_LEVEL	:
		{
			INT nOpLv = min(INT(m_pItemInst->GetOptionLv())/TMAGICLEVEL_UNIT, TMAGICLEVEL_MAX-1);
			strRESULT = TMAGICLEVEL_NAMES[nOpLv];
		}
		break;
	}

	return strRESULT;
}
// ----------------------------------------------------------------------------------------------------
BOOL CTOptionItemDetInfo::_IsMagicLine( const CString& strLine )
{
	if( m_pItemInst->GetApplyMagicID() )
	{
		if( m_pItemInst->GetApplyAP() && strLine.Find(TITEM_TOKENS[TINFOTOKEN_ITEM_ITEMMINAP]) != -1 )
			return TRUE;
		else if( m_pItemInst->GetApplyMAP() && strLine.Find(TITEM_TOKENS[TINFOTOKEN_ITEM_ITEMMINMAP]) != -1 )
			return TRUE;
		else if( m_pItemInst->GetApplyDP() && strLine.Find(TITEM_TOKENS[TINFOTOKEN_ITEM_ITEMDP]) != -1 )
			return TRUE;
		else if( m_pItemInst->GetApplyMDP() && strLine.Find(TITEM_TOKENS[TINFOTOKEN_ITEM_ITEMMDP]) != -1 )
			return TRUE;
	}	
	
	return FALSE;
}
// ====================================================================================================

// CTSkillDetInfo
// ====================================================================================================
const CString CTSkillDetInfo::TSKILL_TOKENS[TINFOTOKEN_SKILL_COUNT] = {
	CString(_T("/Data1")),				// TINFOTOKEN_SKILL_DATA1
	CString(_T("/Data2")),				// TINFOTOKEN_SKILL_DATA2
	CString(_T("/Data3")),				// TINFOTOKEN_SKILL_DATA3
	CString(_T("/Data4")),				// TINFOTOKEN_SKILL_DATA4
	CString(_T("/Data5")),				// TINFOTOKEN_SKILL_DATA5
	CString(_T("/Data6")),				// TINFOTOKEN_SKILL_DATA6
	CString(_T("/SkillLevel")),			// TINFOTOKEN_SKILL_LEVEL
	CString(_T("/HitRate")),			// TINFOTOKEN_SKILL_HITRATE
	CString(_T("/ActionTime")),			// TINFOTOKEN_SKILL_ACTTIME
	CString(_T("/Duration")),			// TINFOTOKEN_SKILL_DURATION
	CString(_T("/Reuse")),				// TINFOTOKEN_SKILL_REUSE
	CString(_T("/UseHP")),				// TINFOTOKEN_SKILL_USEHP
	CString(_T("/UseMP")),				// TINFOTOKEN_SKILL_USEMP
	CString(_T("/UpgradeTitle")),		// TINFOTOKEN_SKILL_UPGRADETITLE
	CString(_T("/PrevLevel")),			// TINFOTOKEN_SKILL_PREVLEVEL
	CString(_T("/ClassPoint")),			// TINFOTOKEN_SKILL_CLASSPOINT
	CString(_T("/SkillPoint")),			// TINFOTOKEN_SKILL_SKILLPOINT
	CString(_T("/NextLevel")),			// TINFOTOKEN_SKILL_NEXTLEVEL
	CString(_T("/NextPrice")),			// TINFOTOKEN_SKILL_NEXTPRICE
};
// ====================================================================================================
CTSkillDetInfo::CTSkillDetInfo(LPTSKILL pTSkill, 
							   BYTE bLevel, 
							   BOOL bSkillReq, 
							   const CRect& rc)
:	ITDetailInfo(TDETINFO_TYPE_SKILL, 
				 pTSkill->m_dwInfoID,
				 pTSkill->m_wIconID, 
				 rc),
	m_pTSkill(pTSkill),
	m_bLevel(bLevel),
	m_bSkillReq(bSkillReq),
	m_bProcUpgrade(FALSE)
{
}
// ----------------------------------------------------------------------------------------------------
CTSkillDetInfo::~CTSkillDetInfo()
{
}
// ====================================================================================================
BOOL CTSkillDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !ITDetailInfo::Compare(pTarget) )
		return FALSE;

	const CTSkillDetInfo* pSkillDet = static_cast<const CTSkillDetInfo*>( pTarget );
	if( m_pTSkill->m_wSkillID == pSkillDet->m_pTSkill->m_wSkillID &&
		m_bLevel == pSkillDet->m_bLevel &&
		m_bSkillReq == pSkillDet->m_bSkillReq )
	{
		return TRUE;
	}

	return FALSE;
}
// ----------------------------------------------------------------------------------------------------
void CTSkillDetInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	m_TitleB.m_dwColor = TDEFAULT_TEXT_COLOR;
	m_TitleB.m_strText = m_pTSkill->m_strNAME;

	m_bProcUpgrade = (m_bLevel > 0)? FALSE: TRUE;

	ITDetailInfo::Build();
}
// ====================================================================================================
ITDetailInfo::LINE_STATE CTSkillDetInfo::_GetLineState(const CString& strSource, int nIdx)
{
	if( m_bLevel )
	{
		if( !m_bProcUpgrade && strSource.Find(TSKILL_TOKENS[TINFOTOKEN_SKILL_UPGRADETITLE]) != -1 )
			m_bProcUpgrade = TRUE;

		BYTE bLevel = m_bProcUpgrade? m_bLevel+1: m_bLevel;

		if( (bLevel == 0) ||
			(bLevel > m_pTSkill->m_bMaxLevel) ||
			(!m_bSkillReq && m_bProcUpgrade) )
		{
			return LS_INVALID;
		}
		else if( nIdx == 0 )
			return LS_TITLE;
		else if( strSource.Find(m_cTokenMark) != -1 )
			return LS_NEEDCONV;
		else
			return LS_VALID;
	}
	else
	{
		if( strSource.Find(TSKILL_TOKENS[TINFOTOKEN_SKILL_UPGRADETITLE]) != -1 )
			return LS_INVALID;
		
		if( nIdx == 0 )
			return LS_TITLE;
		else if( strSource.Find(m_cTokenMark) != -1 )
			return LS_NEEDCONV;
		else
			return LS_VALID;
	}
}
// ----------------------------------------------------------------------------------------------------
void CTSkillDetInfo::_AddLine(const CString& strSource, DWORD dwColor)
{
	m_LinesB.push_back( Line(strSource,
		m_bProcUpgrade? TNEXTLEV_TEXT_COLOR: TDEFAULT_TEXT_COLOR) ); 
}
// ----------------------------------------------------------------------------------------------------
void CTSkillDetInfo::_ConvertAndSetTitle(const CString& strSource, DWORD dwColor)
{
	m_TitleB.m_dwColor = dwColor;
	m_TitleB.m_strText = strSource;
	
	CString strLevel; 
	strLevel = CTChart::Format( TSTR_FMT_NUMBER, max(1,m_bLevel));
	m_TitleB.m_strText.Replace(TSKILL_TOKENS[TINFOTOKEN_SKILL_LEVEL], strLevel);
}
// ----------------------------------------------------------------------------------------------------
void CTSkillDetInfo::_ConvertAndAddLine(const CString& strSource, DWORD dwColor)
{
	CString strLine(strSource);

	for(int i=0; i<TINFOTOKEN_SKILL_COUNT; ++i)
	{
		int nFind = strLine.Find(TSKILL_TOKENS[i]);
		if( nFind == -1 )
			continue;

		CString strConv = _GetSkillString((TINFOTOKEN_SKILL)i);
		if( strConv.IsEmpty() )
			continue;

		int nTokLen = TSKILL_TOKENS[i].GetLength();

		while( nFind != -1 )
		{
			CString strNew;
			
			if( nFind )
				strNew = strLine.Left(nFind);

			strNew += strConv;

			int nRight = strLine.GetLength() - nTokLen - nFind;
			if( nRight )
				strNew += strLine.Right(nRight);

			strLine = strNew;
			nFind = strLine.Find(TSKILL_TOKENS[i]);
		}	
	}

	if( m_bProcUpgrade )
		dwColor = TNEXTLEV_TEXT_COLOR;

	m_LinesB.push_back( Line(strLine,dwColor) ); 
}

// ====================================================================================================
CString CTSkillDetInfo::_GetSkillString(TINFOTOKEN_SKILL eToken)
{
	CString strResult;

	BYTE bLevel = max(1, m_bProcUpgrade ? m_bLevel + 1: m_bLevel);
	if( bLevel == 0 )
		return strResult;
	
	BYTE bCurLevel = m_pTSkill->m_bLevel + (bLevel - 1) * m_pTSkill->m_bLevelInc;

	switch(eToken)
	{
	case TINFOTOKEN_SKILL_DATA1			:
	case TINFOTOKEN_SKILL_DATA2			:
	case TINFOTOKEN_SKILL_DATA3			:
	case TINFOTOKEN_SKILL_DATA4			:
	case TINFOTOKEN_SKILL_DATA5			:
	case TINFOTOKEN_SKILL_DATA6			:
		{
			int nINDEX = eToken - TINFOTOKEN_SKILL_DATA1;

			if( nINDEX >= 0 && INT(m_pTSkill->m_vTFUNCTION.size()) > nINDEX )
			{
				int nVALUE = CTClientSkill::CalcTSKILLDATA(
					m_pTSkill,
					nINDEX,
					bLevel);
				strResult = CTChart::Format( TSTR_FMT_NUMBER, nVALUE);
			}
		}

		break;

	case TINFOTOKEN_SKILL_LEVEL			:
		if( m_pTSkill->m_bMaxLevel == bLevel )
			strResult = CTChart::LoadString( TSTR_TEXT_MASTER);
		else
			strResult = CTChart::Format( TSTR_FMT_GAUGE, bLevel, m_pTSkill->m_bMaxLevel);

		break;

	case TINFOTOKEN_SKILL_HITRATE		: strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pTSkill->m_bHitInit + (bLevel - 1) * m_pTSkill->m_bHitInc); break;
	case TINFOTOKEN_SKILL_DURATION		: strResult = BuildTimeFormatSTR(m_pTSkill->m_dwDuration + (bLevel - 1) * m_pTSkill->m_dwDurationInc); break;
	
	case TINFOTOKEN_SKILL_ACTTIME		: 
		{
			DWORD dwResult;
			CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
			if( pMainChar->m_bChargeSpeed )
				dwResult = m_pTSkill->m_dwSpellTick * 100 / pMainChar->m_bChargeSpeed;
			else
				dwResult = m_pTSkill->m_dwSpellTick;

			strResult = BuildTimeFormatSTR(dwResult);
		}
		break;

	case TINFOTOKEN_SKILL_REUSE			: 
		{
			DWORD dwResult = m_pTSkill->m_dwDelay + (bLevel - 1) * m_pTSkill->m_nDelayInc;
			if( m_pTSkill->m_bDelayType )
			{
				CTClientChar* pMainChar = CTClientGame::GetInstance()->GetMainChar();
				DWORD dwAD = pMainChar->m_dwAttackDelay[ m_pTSkill->m_bDelayType ];
				DWORD dwADR = pMainChar->m_dwAttackDelayRate[ m_pTSkill->m_bDelayType ];

				dwResult = (dwResult+dwAD) * dwADR / 100;
			}

			strResult = BuildTimeFormatSTR(dwResult);
		}
		break;

	case TINFOTOKEN_SKILL_USEHP			: strResult = CTChart::Format( TSTR_FMT_NUMBER, INT(CTChart::m_vLEVELRATE[bCurLevel] * FLOAT(m_pTSkill->m_wUseHP) / 100.0f)); break;
	case TINFOTOKEN_SKILL_USEMP			: strResult = CTChart::Format( TSTR_FMT_NUMBER, INT(CTChart::m_vLEVELRATE[bCurLevel] * FLOAT(m_pTSkill->m_wUseMP) / 100.0f)); break;
	case TINFOTOKEN_SKILL_UPGRADETITLE	: strResult = CTChart::LoadString( TSTR_SKILLUPGRADEINFO_TITLE); break;

	case TINFOTOKEN_SKILL_PREVLEVEL		:
		if( m_pTSkill->m_vTUPGRADEINFO && bLevel - 1 < m_pTSkill->m_bMaxLevel )
			strResult = CTChart::Format( TSTR_FMT_LEVELB, m_pTSkill->m_vTUPGRADEINFO[bLevel - 1].m_bPrevLevel);

		break;

	case TINFOTOKEN_SKILL_CLASSPOINT	:
		if( m_pTSkill->m_vTUPGRADEINFO && bLevel - 1 < m_pTSkill->m_bMaxLevel )
			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pTSkill->m_vTUPGRADEINFO[bLevel - 1].m_bGroupPoint);

		break;

	case TINFOTOKEN_SKILL_SKILLPOINT	:
		if( m_pTSkill->m_vTUPGRADEINFO && bLevel - 1 < m_pTSkill->m_bMaxLevel )
			strResult = CTChart::Format( TSTR_FMT_NUMBER, m_pTSkill->m_vTUPGRADEINFO[bLevel - 1].m_bSkillPoint);

		break;

	case TINFOTOKEN_SKILL_NEXTLEVEL		: strResult = CTChart::Format( TSTR_FMT_NUMBER, bCurLevel); break;
	case TINFOTOKEN_SKILL_NEXTPRICE		:
		{
			DWORD dwPrice = DWORD(FLOAT( CTChart::GetLevelPrice(bCurLevel)) * m_pTSkill->m_fPrice);

			if( CTClientSkill::m_bDiscountRate )
			{
				dwPrice -= dwPrice * CTClientSkill::m_bDiscountRate / 100;

				CString strSALE;
				strSALE = CTChart::Format( TSTR_SKILLNEXTPRICE_SALE,
					CTClientSkill::m_bDiscountRate);

				strResult = CTClientGame::MakeMoneyStr(dwPrice);
				strResult += strSALE;
			}
			else
			{
				strResult = CTClientGame::MakeMoneyStr(dwPrice);
			}
		}

		break;
	}

	return strResult;
}
// ====================================================================================================

CTCastleDefInfo::CTCastleDefInfo(
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
	const CRect& rc )
:
ITDetailInfo(TDETINFO_TYPE_CASTLE, 0, 0, rc),
m_strCastle( strCastle ),
m_timeNext( timeNext ),
m_strAtkGuild( strAtkGuild ),
m_strDefGuild( strDefGuild ),
m_wAtkGuildPoint( wAtkGuildPoint ),
m_wAtkCountryPoint( wAtkCountryPoint ),
m_bAtkCount(bAtkCount),
m_wDefGuildPoint( wDefGuildPoint ),
m_wDefCountryPoint( wDefCountryPoint ),
m_bDefCount(bDefCount),
m_strMyGuild( strMyGuild ),
m_wMyGuildPoint( wMyGuildPoint )
{
#ifdef MODIFY_GUILD
	if( vDTop3 )
		m_vDTop3.insert(m_vDTop3.begin(), vDTop3->begin(), vDTop3->end() );

	if( vCTop3 )
		m_vCTop3.insert(m_vCTop3.begin(), vCTop3->begin(), vCTop3->end() );
#endif
}

CTCastleDefInfo::~CTCastleDefInfo()
{

}


CPoint CTCastleDefInfo::GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse )
{
	return CPoint( m_rcRef.right,
		m_rcRef.top + m_rcRef.Height()/2 - rcDetDlg.Height()/2 );
}

BOOL CTCastleDefInfo::Compare(const ITDetailInfo* pTarget)
{
	if( pTarget->GetType() != TDETINFO_TYPE_CASTLE)
		return FALSE;

	const CTCastleDefInfo* pTargetInfo = static_cast<const CTCastleDefInfo*>( pTarget );

	if( pTargetInfo->m_strCastle != m_strCastle )
		return FALSE;
	
	if( pTargetInfo->m_timeNext != m_timeNext )
		return FALSE;

	if( pTargetInfo->m_strAtkGuild != m_strAtkGuild )
		return FALSE;

	if( pTargetInfo->m_strDefGuild != m_strDefGuild )
		return FALSE;

	if( pTargetInfo->m_wAtkGuildPoint != m_wAtkGuildPoint )
		return FALSE;

	if( pTargetInfo->m_wAtkCountryPoint != m_wAtkCountryPoint )
		return FALSE;

	if( pTargetInfo->m_wDefGuildPoint != m_wDefGuildPoint )
		return FALSE;

	if( pTargetInfo->m_wDefCountryPoint != m_wDefCountryPoint )
		return FALSE;

	if( pTargetInfo->m_strMyGuild != m_strMyGuild )
		return FALSE;

	if( pTargetInfo->m_wMyGuildPoint != m_wMyGuildPoint )
		return FALSE;

	if( pTargetInfo->m_bAtkCount != m_bAtkCount )
		return FALSE;

	if( pTargetInfo->m_bDefCount != m_bDefCount )
		return FALSE;

	return TRUE;
}

void CTCastleDefInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	m_LinesB.empty();

	CString strDATE;
	strDATE = CTChart::Format( TSTR_FMT_TERRITORYTIME,
		m_timeNext.GetYear(),
		m_timeNext.GetMonth(),
		m_timeNext.GetDay(),
		m_timeNext.GetHour(),
		m_timeNext.GetMinute() );

	_AddLine( strDATE, TDEFAULT_TEXT_COLOR);
	_AddLine( " \n", TDEFAULT_TEXT_COLOR);
	
	if( !m_strDefGuild.IsEmpty() )
	{
		CString strFMT;

		DWORD dwColor = D3DCOLOR_XRGB(128, 204, 253);

		strFMT = CTChart::Format( TSTR_FMT_CASTLE_DEFGUILD, m_strDefGuild );
		_AddLine( strFMT, dwColor);
#ifdef MODIFY_GUILD
#else
		strFMT = CTChart::Format( TSTR_GUILD_COUNTRY_POINT, m_wDefCountryPoint );
		_AddLine( strFMT, dwColor);
#endif
		strFMT = CTChart::Format( TSTR_GUILD_GUILDPOINT, m_wDefGuildPoint );
		_AddLine( strFMT, dwColor);

		strFMT = CTChart::Format( TSTR_GUILD_VOL_COUNT, m_bDefCount );
		_AddLine( strFMT, dwColor);
	}

	if( !m_strAtkGuild.IsEmpty() )
	{
		CString strFMT;

		DWORD dwColor = D3DCOLOR_XRGB(254, 208, 0);

		strFMT = CTChart::Format( TSTR_FMT_CASTLE_ATKGUILD, m_strAtkGuild );
		_AddLine( strFMT, dwColor);
#ifdef MODIFY_GUILD
#else
		strFMT = CTChart::Format( TSTR_GUILD_COUNTRY_POINT, m_wAtkCountryPoint );
		_AddLine( strFMT, dwColor);
#endif
		strFMT = CTChart::Format( TSTR_GUILD_GUILDPOINT, m_wAtkGuildPoint );
		_AddLine( strFMT, dwColor);

		strFMT = CTChart::Format( TSTR_GUILD_VOL_COUNT, m_bAtkCount );
		_AddLine( strFMT, dwColor);
	}

#ifdef MODIFY_GUILD
	if( !m_vDTop3.empty() || !m_vCTop3.empty() )
		_AddLine( " \n", TDEFAULT_TEXT_COLOR);

	if( CTClientGame::GetInstance()->GetMainChar()->m_bContryID == TCONTRY_D )
	{
		WriteDTop3();
		WriteCTop3();
	}
	else
	{
		WriteCTop3();
		WriteDTop3();
	}

#endif
	
	_AddLine( " \n", TDEFAULT_TEXT_COLOR);

	if( !m_strMyGuild.IsEmpty() )
	{
		CString strFMT;
		strFMT = CTChart::Format( TSTR_FMT_CASTLE_MYGUILD, m_strMyGuild );
		_AddLine( strFMT, TDEFAULT_TEXT_COLOR);
		
		strFMT = CTChart::Format( TSTR_GUILD_GUILDPOINT, m_wMyGuildPoint );
		_AddLine( strFMT, TDEFAULT_TEXT_COLOR);	
	}

	m_TitleB.m_strText = m_strCastle;
	m_TitleB.m_dwColor = TDEFAULT_TEXT_COLOR;

	m_bNeedBuild = FALSE;
}
#ifdef MODIFY_GUILD
void CTCastleDefInfo::WriteCTop3()
{
	if( !m_vCTop3.empty() )
	{
		CString strFMT;
		strFMT = CTChart::Format( TSTR_FMT_CASTLE_C_TOP3,
			m_wAtkCountryPoint );

		_AddLine( strFMT, TDEFAULT_TEXT_COLOR);

		for( int i=0 ; i < m_vCTop3.size() ; ++i )
		{
			strFMT = CTChart::Format(TSTR_FMT_CASTLE_TOP3,
				m_vCTop3[ i ].m_wPoint,
				m_vCTop3[ i ].m_strName );

			_AddLine( strFMT, TDEFAULT_TEXT_COLOR);
		}
	}
}

void CTCastleDefInfo::WriteDTop3()
{
	if( !m_vDTop3.empty() )
	{
		CString strFMT;
		strFMT = CTChart::Format(TSTR_FMT_CASTLE_D_TOP3,
			m_wDefCountryPoint );

		_AddLine( strFMT, TDEFAULT_TEXT_COLOR);

		for( int i=0 ; i < m_vDTop3.size() ; ++i )
		{
			strFMT = CTChart::Format(TSTR_FMT_CASTLE_TOP3,
				m_vDTop3[ i ].m_wPoint,
				m_vDTop3[ i ].m_strName );

			_AddLine( strFMT, TDEFAULT_TEXT_COLOR);
		}
	}
}
#endif
// ====================================================================================================

CTTerritoryDetInfo::CTTerritoryDetInfo( CString strTerritory, CTime timeNext, CString strHeroName, const CRect& rc)
: ITDetailInfo(TDETINFO_TYPE_TERRITORY, 0, 0, rc),
m_strTerritory(strTerritory),
m_timeNext(timeNext),
m_strHeroName(strHeroName)
{
}

CTTerritoryDetInfo::~CTTerritoryDetInfo()
{
}

CPoint CTTerritoryDetInfo::GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse )
{
	return CPoint( m_rcRef.right,
		m_rcRef.top + m_rcRef.Height()/2 - rcDetDlg.Height()/2 );
}

BOOL CTTerritoryDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( pTarget->GetType() != TDETINFO_TYPE_TERRITORY)
		return FALSE;

	const CTTerritoryDetInfo* pTargetInfo = static_cast<const CTTerritoryDetInfo*>( pTarget );

	if( pTargetInfo->m_strTerritory != m_strTerritory )
		return FALSE;
	
	if( pTargetInfo->m_timeNext != m_timeNext )
		return FALSE;

	if( pTargetInfo->m_strHeroName != m_strHeroName )
		return FALSE;

	return TRUE;
}

void CTTerritoryDetInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	m_LinesB.empty();

	CString strDATE;
	strDATE = CTChart::Format( TSTR_FMT_TERRITORYTIME,
		m_timeNext.GetYear(),
		m_timeNext.GetMonth(),
		m_timeNext.GetDay(),
		m_timeNext.GetHour(),
		m_timeNext.GetMinute() );

    CString strHERO;
	if( !m_strHeroName.IsEmpty() )
	{
		strHERO = CTChart::Format( TSTR_FMT_HERO,
			m_strHeroName);
	}

//	_AddLine( "", TDEFAULT_TEXT_COLOR);
	_AddLine( strDATE, TDEFAULT_TEXT_COLOR);

	if( !strHERO.IsEmpty())
		_AddLine( strHERO, TDEFAULT_TEXT_COLOR);

	m_TitleB.m_strText = m_strTerritory;
	m_TitleB.m_dwColor = TDEFAULT_TEXT_COLOR;

	m_bNeedBuild = FALSE;
}

CTPvPDetInfo::CTPvPDetInfo(
	BYTE bTabIndex,
	CString strName,
	BYTE bWin,
	BYTE bClass,
	BYTE bLevel,
	DWORD dwPoint,
	CTime dlDate,
	DWORD dwTitleColor,
	const CRect& rc )
: ITDetailInfo( TDETINFO_TYPE_PVP, 0, 0, rc ),
m_strName(strName),
m_bWin(bWin),
m_bClass(bClass),
m_bLevel(bLevel),
m_dwPoint(dwPoint),
m_dlDate(dlDate),
m_dwTitleColor(dwTitleColor),
m_bTabIndex(bTabIndex)
{
}

CTPvPDetInfo::~CTPvPDetInfo()
{
}

BOOL CTPvPDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !ITDetailInfo::Compare(pTarget) )
		return FALSE;

	const CTPvPDetInfo* pTargetInfo = static_cast<const CTPvPDetInfo*>(pTarget);

	return pTargetInfo->m_strName == m_strName &&
		pTargetInfo->m_bWin == m_bWin &&
		pTargetInfo->m_bClass == m_bClass &&
		pTargetInfo->m_bLevel == m_bLevel &&
		pTargetInfo->m_dwPoint == m_dwPoint &&
		pTargetInfo->m_dlDate == m_dlDate;
}

void CTPvPDetInfo::Build()
{
	if( m_bWin )
		_ConvertAndSetTitle( CTChart::LoadString( TSTR_PVP_DET_WIN), TDEFAULT_TEXT_COLOR);
	else
		_ConvertAndSetTitle( CTChart::LoadString( TSTR_PVP_DET_LOSE), TDEFAULT_TEXT_COLOR);

	_AddLine( CTChart::Format( TSTR_PVP_DET_NAME, m_strName),
		TDEFAULT_TEXT_COLOR);

	_AddLine(
		CTChart::Format( TSTR_PVP_DET_CLASS,
			CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ m_bClass ] ) ),
		TDEFAULT_TEXT_COLOR);
	
	_AddLine( CTChart::Format( TSTR_PVP_DET_LEVEL, m_bLevel ),
		TDEFAULT_TEXT_COLOR);

	_AddLine(
		CTChart::Format( TSTR_PVP_DET_DATE,
			m_dlDate.GetYear(),
			m_dlDate.GetMonth(),
			m_dlDate.GetDay(),
			m_dlDate.GetHour(),
			m_dlDate.GetMinute()),
		TDEFAULT_TEXT_COLOR);

	if( m_bTabIndex == TINFO_WAR )
	{
		if( m_dwPoint == 0 )
		{
			_AddLine( 
				CTChart::Format( TSTR_PVP_DET_POINT,
					m_dwPoint ),
				TDEFAULT_TEXT_COLOR);
		}
		else if( m_bWin )
		{
			_AddLine( 
				CTChart::Format( TSTR_PVP_DET_PLUS_POINT,
					m_dwPoint ),
				TDEFAULT_TEXT_COLOR);
		}
		else
		{
			_AddLine( 
				CTChart::Format( TSTR_PVP_DET_MINUS_POINT,
					m_dwPoint ),
				TDEFAULT_TEXT_COLOR);
		}
	}
}

CTTextDetInfo::CTTextDetInfo(
	CString& strText,
	CPoint pt )
: ITDetailInfo( TDETINFO_TYPE_TEXT, 0, 0, CRect(0, 0, 0 ,0) ),
m_strText(strText)
{
	m_ptPosition = pt;
	m_nOutlookStyle = TDETINFO_OUTLOOK_STYLE_UNTITLE;
}

CTTextDetInfo::~CTTextDetInfo()
{
}

BOOL CTTextDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !ITDetailInfo::Compare(pTarget) )
		return FALSE;

	const CTTextDetInfo* pTargetInfo = static_cast<const CTTextDetInfo*>(pTarget);
	return pTargetInfo->m_strText == this->m_strText;
}

void CTTextDetInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	{
		CTTextLinker *pTextLinker = CTTextLinker::GetInstance();

		INT nPos = 0;
		CString strLINE = pTextLinker->SplitTextByCrLf(
			m_strText,
			CTTextLinker::LINK_DEF_TOK,
			nPos, FALSE);

		while(!strLINE.IsEmpty())
		{
			CString strPMSG = pTextLinker->MakeNetToLinkText( NULL, strLINE);
			strPMSG.Remove('\r');

			_AddLine( strPMSG, TDEFAULT_TEXT_COLOR );

			strLINE = pTextLinker->SplitTextByCrLf( m_strText, CTTextLinker::LINK_DEF_TOK, nPos, FALSE);
		}
	}
}

CPoint CTTextDetInfo::GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse )
{
	CRect rtScreen = CTClientUIBase::GetScreenRect();
	CPoint ptResult = m_ptPosition;

	BOOL bInvX = FALSE;
	BOOL bInvY = FALSE;

	if( rtScreen.Width() - ptMouse.x < rcDetDlg.Width()+32 )
	{
		ptResult.x -= rcDetDlg.Width();
		bInvX = TRUE;
	}

	if( rtScreen.Height() - ptMouse.y < rcDetDlg.Height()+32 )
	{
		ptResult.y -= rcDetDlg.Height();
		bInvY = TRUE;
	}

	if( bInvX == FALSE &&
		bInvY == FALSE )
	{
		ptResult.x += 32;
		ptResult.y += 32;
	}

	return ptResult;
}

// ====================================================================================================
CTFameRankDetInfo::CTFameRankDetInfo(
									 CString	strName,
									 DWORD	dwTotalPoint,
									 DWORD	dwMonthPoint,
									 WORD	wWin,
									 WORD	wLose,
									 BYTE	bLevel,
									 BYTE	bClass,
									 const CRect& rc  )
									 : ITDetailInfo( TDETINFO_TYPE_FAMERANK, 0, 0, rc ),
									 m_strName(strName),
									 m_dwTotalPoint(dwTotalPoint),
									 m_dwMonthPoint(dwMonthPoint),
									 m_wWin(wWin),
									 m_wLose(wLose),
									 m_bLevel(bLevel),
									 m_bClass(bClass)
{
	// m_nOutlookStyle = TDETINFO_OUTLOOK_STYLE_UNTITLE;
}

CTFameRankDetInfo::~CTFameRankDetInfo()
{
}

BOOL CTFameRankDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !ITDetailInfo::Compare(pTarget) )
		return FALSE;

	const CTFameRankDetInfo* pTargetInfo = static_cast<const CTFameRankDetInfo*>(pTarget);
	return pTargetInfo->m_strName == m_strName &&
		pTargetInfo->m_dwTotalPoint == m_dwTotalPoint &&
		pTargetInfo->m_dwMonthPoint == m_dwMonthPoint &&
		pTargetInfo->m_wWin == m_wWin &&
		pTargetInfo->m_wLose == m_wLose &&
		pTargetInfo->m_bLevel == m_bLevel &&
		pTargetInfo->m_bClass == m_bClass;
}

void CTFameRankDetInfo::Build()
{
	if ( !m_bNeedBuild )
		return;
	{
		_ConvertAndSetTitle( m_strName );

		_AddLine( CTChart::Format( TSTR_FAMERANKDEF_LEVEL, m_bLevel ) );
		_AddLine( CTChart::Format( TSTR_FAMERANKDEF_CLASS, CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ m_bClass] )));
		_AddLine( CTChart::Format( TSTR_FAMERANKDEF_BATTLE, m_wWin, m_wLose ) );
	}

	m_bNeedBuild = FALSE;
}
// ====================================================================================================


CTDefToolTipInfo::CTDefToolTipInfo( CString strTitle, CString strToolTip, const CRect& rc )
:	ITDetailInfo( TDEFINFO_TYPE_DEFTOOLTIP, 0, 0, rc ),
m_strTitle( strTitle ),
m_strToolTip( strToolTip )
{

}

CTDefToolTipInfo::~CTDefToolTipInfo()
{

}

BOOL CTDefToolTipInfo::Compare( const ITDetailInfo* pTarget )
{
	if( !ITDetailInfo::Compare(pTarget) )
		return FALSE;

	const CTDefToolTipInfo* pTargetInfo = static_cast<const CTDefToolTipInfo*>(pTarget);
	return pTargetInfo->m_strToolTip == this->m_strToolTip;
}

void CTDefToolTipInfo::Build()
{
	m_TitleB.m_strText = m_strTitle;
	m_TitleB.m_dwColor = TDEFAULT_TEXT_COLOR;

	_AddLine( m_strToolTip );
}

// -----------------------------------------------------------------------------------

// ====================================================================================================
CTTournamentPlayerInfo::CTTournamentPlayerInfo(
	CString strName,
	BYTE bWin,
	BYTE bCountry,
	BYTE bClass,
	BYTE bLevel,
	const CRect& rc )
: ITDetailInfo( TDETINFO_TYPE_TOURNAMENT_PLAYER, 0, 0, rc ),
m_strName(strName),
m_bWin(bWin),
m_bCountry(bCountry),
m_bClass(bClass),
m_bLevel(bLevel),
m_bInfoType(0)
{
}

CTTournamentPlayerInfo::CTTournamentPlayerInfo(
	CString strTitle,
	CString strText,
	INT nLine,
	const CRect& rc )
: ITDetailInfo( TDETINFO_TYPE_TOURNAMENT_PLAYER, 0, 0, rc ),
m_strName(strTitle),
m_strText(strText),
m_nLine(nLine),
m_bInfoType(1)
{
}

CTTournamentPlayerInfo::~CTTournamentPlayerInfo()
{
}

BOOL CTTournamentPlayerInfo::Compare(const ITDetailInfo* pTarget)
{
	if( !ITDetailInfo::Compare(pTarget) )
		return FALSE;

	const CTTournamentPlayerInfo* pTargetInfo = static_cast<const CTTournamentPlayerInfo*>(pTarget);

	if( m_bInfoType == 0 )
	{
		return pTargetInfo->m_strName == m_strName &&
			pTargetInfo->m_bWin == m_bWin &&
			pTargetInfo->m_bClass == m_bClass &&
			pTargetInfo->m_bLevel == m_bLevel;
	}
	else
	{
		return pTargetInfo->m_strName == m_strName &&
			pTargetInfo->m_strText == m_strText &&
			pTargetInfo->m_nLine == m_nLine;
	}
}

void CTTournamentPlayerInfo::Build()
{
	if( m_bInfoType == 0 )
	{
		switch( m_bWin )
		{
		case TNMTWIN_WIN:
			_ConvertAndSetTitle( CTChart::LoadString( TSTR_PVP_DET_WIN), TDEFAULT_TEXT_COLOR);
			break;
		case TNMTWIN_LOSE:
			_ConvertAndSetTitle( CTChart::LoadString( TSTR_PVP_DET_LOSE), TDEFAULT_TEXT_COLOR);
			break;
		case TNMTWIN_NONE:
			break;
		};

		_AddLine( CTChart::Format( TSTR_FAMERANKDEF_NAME, m_strName),
			TDEFAULT_TEXT_COLOR);

		_AddLine( CTChart::Format( TSTR_FAMERANKDEF_COUNTRY,
			CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ m_bCountry ] ) ),
			TDEFAULT_TEXT_COLOR);

		_AddLine( CTChart::Format( TSTR_FAMERANKDEF_CLASS,
				CTChart::LoadString( (TSTRING) CTClientGame::m_vTCLASSSTR[ m_bClass ] ) ),
				TDEFAULT_TEXT_COLOR);
		
		_AddLine( CTChart::Format( TSTR_FAMERANKDEF_LEVEL, m_bLevel ),
			TDEFAULT_TEXT_COLOR);
	}
	else
	{
		_ConvertAndSetTitle( m_strName, TDEFAULT_TEXT_COLOR );
		_AddLine( m_strText, TDEFAULT_TEXT_COLOR );
	}
}





// ====================================================================================================

CTMissionDetInfo::CTMissionDetInfo(
								   CString strMission,
								   CTime timeNext,
								   BYTE bCountry,
								   BYTE bStatus,
								   const CRect& rc)
: ITDetailInfo(TDETINFO_TYPE_MISSION, 0, 0, rc),
m_strMission(strMission),
m_timeNext(timeNext),
m_bCountry(bCountry),
m_bStatus(bStatus)
{
}

CTMissionDetInfo::~CTMissionDetInfo()
{
}

CPoint CTMissionDetInfo::GetUIPosition(const CRect& rcDetDlg, const CPoint& ptMouse )
{
	return CPoint( m_rcRef.right,
		m_rcRef.top + m_rcRef.Height()/2 - rcDetDlg.Height()/2 );
}

BOOL CTMissionDetInfo::Compare(const ITDetailInfo* pTarget)
{
	if( pTarget->GetType() != TDETINFO_TYPE_MISSION)
		return FALSE;

	const CTMissionDetInfo* pTargetInfo = static_cast<const CTMissionDetInfo*>( pTarget );

	if( pTargetInfo->m_strMission != m_strMission )
		return FALSE;
	
	if( pTargetInfo->m_timeNext != m_timeNext )
		return FALSE;

	if( pTargetInfo->m_bCountry != m_bCountry )
		return FALSE;

	if( pTargetInfo->m_bStatus != m_bStatus )
		return FALSE;

	return TRUE;
}

void CTMissionDetInfo::Build()
{
	if( !m_bNeedBuild )
		return;

	m_LinesB.empty();

	CString strCountry;
	if( m_bCountry < COUNTRY_COUNT )
	{
		strCountry = CTChart::Format( TSTR_MISSION_COUNTRY,
			(LPCTSTR) CTChart::LoadString( (TSTRING) CTClientGame::m_vTCOUNTRYSTR[ m_bCountry ]) );
	}
	else
		strCountry = CTChart::LoadString( TSTR_MISSION_COUNTRY_NEUTRAL );

	_AddLine( strCountry, TDEFAULT_TEXT_COLOR);

	CString strStatus;
	switch( m_bStatus )
	{
	case BS_NORMAL:
		strStatus = CTChart::LoadString( TSTR_MISSION_STATUS_NORMAL );
		break;
	case BS_BATTLE:
		strStatus = CTChart::LoadString( TSTR_MISSION_STATUS_WAR );
		break;
	case BS_PEACE:
		strStatus = CTChart::LoadString( TSTR_MISSION_STATUS_WAR_CLOSE );
		break;
	}

	_AddLine( strStatus, TDEFAULT_TEXT_COLOR);

	CString strDATE;
	strDATE = CTChart::Format( TSTR_FMT_TERRITORYTIME,
		m_timeNext.GetYear(),
		m_timeNext.GetMonth(),
		m_timeNext.GetDay(),
		m_timeNext.GetHour(),
		m_timeNext.GetMinute() );

	_AddLine( strDATE, TDEFAULT_TEXT_COLOR);

	m_TitleB.m_strText = m_strMission;
	m_TitleB.m_dwColor = TDEFAULT_TEXT_COLOR;

	m_bNeedBuild = FALSE;
}