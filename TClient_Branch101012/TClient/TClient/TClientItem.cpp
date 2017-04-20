#include "StdAfx.h"
#include "TClientWnd.h"

// ====================================================================================================
MAPDWORD CTClientItem::m_mapTITEMTICK;
MAPDWORD CTClientItem::m_mapTITEMLOCK;
// ====================================================================================================
DWORD CTClientItem::m_dwEQUIPCOLOR = D3DCOLOR_ARGB(255, 255,250,237);
DWORD CTClientItem::m_dwNORMALCOLOR = D3DCOLOR_ARGB(255,219,214,202);
// ====================================================================================================
MAPTQUESTCLNTITEM CTClientItem::m_mapTQuestMagicClientItem;

CTClientItem* CTClientItem::FindTQuestMagicClientItem( WORD wRewardID )
{
	MAPTQUESTCLNTITEM::iterator finder = CTClientItem::m_mapTQuestMagicClientItem.find( wRewardID );
	if( finder != CTClientItem::m_mapTQuestMagicClientItem.end() )
		return finder->second;

	return NULL;
}
// ====================================================================================================
CTClientItem::CTClientItem()
:	m_pTITEM(NULL),
	m_bItemID(0),
	m_wItemID(0),
	m_bCount(0),
	m_bGrade(0),
	m_bMagicGrade(0),
	m_bInstGradeMax(0),
	m_dwDuraMax(0),
	m_dwDuraCurrent(0),
	m_bRefineMax(0),
	m_bRefineCurrent(0),
	m_bCanGamble(false),
	m_pTBestOption(NULL),
	m_bNeedUpdate(FALSE),
	m_dEndTime(0),
	m_bGradeEffect(0),
	m_bWrap(0),
	m_bELD(0),
	m_wColor(0),
	m_bRegGuild(0)
{
	m_mapTMAGIC.clear();
}
// ----------------------------------------------------------------------------------------------------
CTClientItem::~CTClientItem()
{
	ClearMagic();
}
// ====================================================================================================
DWORD CTClientItem::GetTick( WORD wDelayGroupID)
{
	DWORD dwValue = (DWORD)wDelayGroupID;
	MAPDWORD::iterator finder = m_mapTITEMTICK.find(dwValue);

	if( finder != m_mapTITEMTICK.end() )
		return (*finder).second;

	return 0;
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::SetTick(WORD wDelayGroupID, DWORD dwTick )
{
	DWORD dwValue = (DWORD)wDelayGroupID;
	MAPDWORD::iterator finder = m_mapTITEMTICK.find(dwValue);

	if(!dwTick)
		return;

	if( finder == m_mapTITEMTICK.end() )
		m_mapTITEMTICK.insert( MAPDWORD::value_type( dwValue, dwTick));
	else
		(*finder).second = dwTick;
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::ClearTick()
{
	m_mapTITEMTICK.clear();
}
// ====================================================================================================
BYTE CTClientItem::IsLocked( WORD wDelayGroupID)
{
	DWORD dwValue = (DWORD)wDelayGroupID;
	MAPDWORD::iterator finder = m_mapTITEMLOCK.find(dwValue);
	return finder != m_mapTITEMLOCK.end() ? TRUE : FALSE;
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::Lock( WORD wDelayGroupID)
{
	DWORD dwValue = (DWORD)wDelayGroupID;
	MAPDWORD::iterator finder = m_mapTITEMLOCK.find(dwValue);

	if( finder == m_mapTITEMLOCK.end() )
		m_mapTITEMLOCK.insert( MAPDWORD::value_type( dwValue, dwValue));
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::Unlock( WORD wDelayGroupID)
{
	DWORD dwValue = (DWORD)wDelayGroupID;
	MAPDWORD::iterator finder = m_mapTITEMLOCK.find(dwValue);

	if( finder != m_mapTITEMLOCK.end() )
		m_mapTITEMLOCK.erase(finder);
}
// ====================================================================================================
VECTORBYTE CTClientItem::GetEquipSlotList(LPTITEM pTITEM)
{
	VECTORBYTE res;
	for(BYTE i=0; i<ES_COUNT; ++i)
	{
		if( pTITEM->m_dwSlotID & (1<<i) )
			res.push_back(i);
	}

	return res;
}
// ----------------------------------------------------------------------------------------------------
LPTITEMVISUAL CTClientItem::GetDefaultVisual( LPTITEM pTITEM)
{
	return CTChart::FindTITEMVISUAL( pTITEM->m_wVisual[0] );
}
// ====================================================================================================
DWORD CTClientItem::CalcPrice(LPTITEM pTITEM, WORD wAttrLevel)
{
	MAPTLEVEL::iterator itr =  CTChart::m_mapTLEVEL.find((BYTE)wAttrLevel); 
	if( itr != CTChart::m_mapTLEVEL.end() )
		return (DWORD)(pTITEM->m_fPriceRate * (FLOAT)(itr->second.m_dwCost) + 0.99f);
	else
		return 0;
}
// ----------------------------------------------------------------------------------------------------
WORD CTClientItem::CalcViewAttrLevel(WORD wAttrLevel)
{
	if( wAttrLevel < TATTRLEVEL_SEPERATE )
		return wAttrLevel - (wAttrLevel+TATTRLEVEL_LOWUNIT-1)%TATTRLEVEL_LOWUNIT;
	else if( TATTRLEVEL_SEPERATE%2 )
		return wAttrLevel - (wAttrLevel+TATTRLEVEL_HIGHUNIT-1)%TATTRLEVEL_HIGHUNIT;
	else
		return wAttrLevel - wAttrLevel%TATTRLEVEL_HIGHUNIT;
}
// ====================================================================================================

// operator functions
// ====================================================================================================
BYTE CTClientItem::operator == ( CTClientItem& param)
{
	if( m_wItemID != param.m_wItemID ||
		m_bGrade != param.m_bGrade ||
		m_bInstGradeMax != param.m_bInstGradeMax ||
		m_dwDuraMax != param.m_dwDuraMax ||
		m_dwDuraCurrent != param.m_dwDuraCurrent ||
		m_bRefineMax != param.m_bRefineMax ||
		m_bRefineCurrent != param.m_bRefineCurrent ||
		m_bCanGamble != param.m_bCanGamble ||
		m_mapTMAGIC.size() != param.m_mapTMAGIC.size() ||
		m_bELD != param.m_bELD ||
		m_bWrap != param.m_bWrap ||
		m_wColor != param.m_wColor ||
		m_bRegGuild != param.m_bRegGuild ||
		m_dEndTime != param.m_dEndTime )
	{
		return FALSE;
	}
	
	MAPTMAGIC::iterator itTMAGIC;

	for( itTMAGIC = m_mapTMAGIC.begin(); itTMAGIC != m_mapTMAGIC.end(); itTMAGIC++)
	{
		MAPTMAGIC::iterator finder = param.m_mapTMAGIC.find((*itTMAGIC).first);

		if( finder == param.m_mapTMAGIC.end() )
			return FALSE;

		if( (*itTMAGIC).second->m_wValue != (*finder).second->m_wValue )
			return FALSE;
	}

	return TRUE;
}
// ----------------------------------------------------------------------------------------------------
BYTE CTClientItem::operator != ( CTClientItem& param)
{
	return (*this) == param ? FALSE : TRUE;
}
// ----------------------------------------------------------------------------------------------------
CTClientItem& CTClientItem::operator = ( CTClientItem& param)
{
	MAPTMAGIC::iterator itTMAGIC;

	for( itTMAGIC = m_mapTMAGIC.begin(); itTMAGIC != m_mapTMAGIC.end(); itTMAGIC++)
		delete (*itTMAGIC).second;
	
	m_mapTMAGIC.clear();

	m_bItemID = param.m_bItemID;
	m_wItemID = param.m_wItemID;
	m_pTITEM = param.m_pTITEM;
	m_bCount = param.m_bCount;
	m_bGrade = param.m_bGrade;
	m_bInstGradeMax = param.m_bInstGradeMax;
	m_dwDuraMax = param.m_dwDuraMax;
	m_dwDuraCurrent = param.m_dwDuraCurrent;
	m_bRefineMax = param.m_bRefineMax;
	m_bRefineCurrent = param.m_bRefineCurrent;
	m_bCanGamble = param.m_bCanGamble;
	m_dEndTime = param.m_dEndTime;
	m_bELD = param.m_bELD;
	m_bWrap = param.m_bWrap;
	m_wColor = param.m_wColor;
	m_bRegGuild = param.m_bRegGuild;

	for( itTMAGIC = param.m_mapTMAGIC.begin(); itTMAGIC != param.m_mapTMAGIC.end(); itTMAGIC++)
	{
		LPTMAGIC pTMAGIC = new TMAGIC();
		pTMAGIC->m_wValue = (*itTMAGIC).second->m_wValue;
		m_mapTMAGIC.insert( make_pair( (*itTMAGIC).first, pTMAGIC) );
	}

	m_bNeedUpdate = param.m_bNeedUpdate;
	
	if( !m_bNeedUpdate )
	{
		m_bMagicGrade = param.m_bMagicGrade;	
		m_pTBestOption = param.m_pTBestOption;	
		m_wAttrID = param.m_wAttrID;		
		m_wAttrLevel= param.m_wAttrLevel;	
		m_wViewAttrLevel = param.m_wViewAttrLevel;
		m_dwPrice = param.m_dwPrice;		
		m_vMagicInfos = param.m_vMagicInfos;	
		m_bApplyAP = param.m_bApplyAP;		
		m_bApplyMAP = param.m_bApplyMAP;	
		m_bApplyDP = param.m_bApplyDP;		
		m_bApplyMDP = param.m_bApplyMDP;	
		m_fOptionLv = param.m_fOptionLv;	
		m_wApplyMagicID = param.m_wApplyMagicID;
		m_itemAttr = param.m_itemAttr;		
		m_bHasMainOption = param.m_bHasMainOption;
	}

	return (*this);
}
// ====================================================================================================

// public functions
// ====================================================================================================
void CTClientItem::NotifyUpdate()
{
	m_bNeedUpdate = TRUE;
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::Update()
{
	if( m_bNeedUpdate )
	{
		CalcAttrID();
		CalcAttrNameAndLevel();

		m_bNeedUpdate = FALSE;
	}
}
// ====================================================================================================
WORD CTClientItem::GetMagicValue( BYTE bMagicID)
{
	MAPTMAGIC::iterator finder = m_mapTMAGIC.find(bMagicID);

	if( finder != m_mapTMAGIC.end() )
		return (*finder).second->m_wValue;

	return 0;
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::ClearMagic()
{
	MAPTMAGIC::iterator itTMAGIC;

	for( itTMAGIC = m_mapTMAGIC.begin(); itTMAGIC != m_mapTMAGIC.end(); itTMAGIC++)
		delete (*itTMAGIC).second;

	m_mapTMAGIC.clear();
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::AddMagicValue(BYTE bMagicID, WORD wValue)
{
	MAPTMAGIC::iterator finder = m_mapTMAGIC.find(bMagicID);
	
	if( finder != m_mapTMAGIC.end() )
	{
		LPTMAGIC pTMAGIC =  (*finder).second;		
		pTMAGIC->m_wValue += wValue;
	}
	else
	{
		LPTMAGIC pTMAGIC = new TMAGIC();
		
		pTMAGIC->m_wValue = wValue;
		m_mapTMAGIC.insert( make_pair(bMagicID, pTMAGIC) );
	}

	NotifyUpdate();
}
// ====================================================================================================
BOOL CTClientItem::IsEmptyMagicInfos()
{
	Update();

	return m_vMagicInfos.empty();
}
// ----------------------------------------------------------------------------------------------------
LPVTMAGICOPTION CTClientItem::GetMagicInfos()
{
	Update();

	return &m_vMagicInfos;
}
// ====================================================================================================
DWORD CTClientItem::GetQuality() const
{
	DWORD dwResult = 0;

	if( GetGrade() > 0 )
		dwResult |= TITEM_QUALITY_GRADE;

	if( !m_mapTMAGIC.empty() )
	{
		if( m_mapTMAGIC.size() > 2 )
			dwResult |= TITEM_QUALITY_RARE;
		else
			dwResult |= TITEM_QUALITY_MAGIC;
	}

	if( dwResult == 0 )
		dwResult = TITEM_QUALITY_NORMAL;

	return dwResult;
}
// ----------------------------------------------------------------------------------------------------
DWORD CTClientItem::GetQualityColor() const
{
	DWORD dwColor;

	DWORD qt = GetQuality();
	if( qt & TITEM_QUALITY_GRADE )
	{
		BYTE c = 240 - (GetGrade()*10);
		dwColor = D3DCOLOR_ARGB(255, 240,c,c);
	}
	else 
		dwColor = GetMagicColor();

	return dwColor;
}
// ----------------------------------------------------------------------------------------------------
DWORD CTClientItem::GetMagicColor() const
{
	DWORD dwColor;

	DWORD qt = GetQuality();
	if( qt & TITEM_QUALITY_RARE )
		dwColor = D3DCOLOR_ARGB(255, 255,177,42);
	else if( qt & TITEM_QUALITY_MAGIC )
		dwColor = D3DCOLOR_ARGB(255, 68,169,255);
	else
		dwColor = CTClientItem::m_dwNORMALCOLOR;

	return dwColor;
}
// ====================================================================================================
VECTORDWORD CTClientItem::GetSFX()
{
	VECTORDWORD vSFX;

	DWORD qt = GetQuality();

	if( qt & TITEM_QUALITY_GRADE )
	{
		vSFX.push_back(GetGradeSFX());
	}
	
	if( (qt & TITEM_QUALITY_GRADE) ||
		(qt & TITEM_QUALITY_MAGIC) )
	{
		vSFX.push_back(GetMagicSFX());
	}

	return vSFX;
}
// ----------------------------------------------------------------------------------------------------
DWORD CTClientItem::GetGradeSFX()
{
	if( !CheckValid() )
		return 0;

	BYTE bGrade = GetVisualGrade();
	if( bGrade )
	{
		LPTITEMGRADESFX pItemGradeSfx = CTChart::FindTITEMGRADESFX( m_pTITEM->m_wGradeSFX );
		if( pItemGradeSfx )
			return pItemGradeSfx->m_wSFX[ bGrade - 1 ];
	}

	return 0;
}
// ----------------------------------------------------------------------------------------------------
LPTITEMGRADEVISUAL CTClientItem::GetGradeVISUAL()
{
	if( !CheckValid() )
		return 0;

	if( m_bGrade )
	{
		WORD wGradeSkin = 0;

		if( 1 <= m_bGrade && m_bGrade <= 5 )
		{
			wGradeSkin = 0;
		}
		else if( 6 <= m_bGrade && m_bGrade <= 10 )
		{
			wGradeSkin = 1;
		}
		else if( 11 <= m_bGrade && m_bGrade <= 15 )
		{
			wGradeSkin = 2;
		}
		else if( 16 == m_bGrade )
		{
			wGradeSkin = 3;
		}
		else if( 17 <= m_bGrade && m_bGrade <= 24 )
		{
			switch( m_bGradeEffect)
			{
			case IE_SEA:
				wGradeSkin = 4;
				break;
			case IE_FIRE:
				wGradeSkin = 5;
				break;
			case IE_LIGHTING:
				wGradeSkin = 6;
				break;
			case IE_ICE:
				wGradeSkin = 7;
				break;
			case IE_BLACK:
				wGradeSkin = 8;
				break;
			default:
				wGradeSkin =0;
			}
		}

		TITEMGRADEVISUALKEY Key;
		Key.m_wGrade = wGradeSkin;
		Key.m_bKind = m_pTITEM->m_bKind;

		MAPTITEMGRADEVISUAL::iterator itr = CTChart::m_mapTITEMGRADEVISUAL.find(Key);
		if( itr != CTChart::m_mapTITEMGRADEVISUAL.end())
			return &(itr->second);
	}

	return NULL;
}
// ----------------------------------------------------------------------------------------------------
DWORD CTClientItem::GetMagicSFX()
{
	if( !CheckValid() )
		return 0;

	Update();

	if( m_pTBestOption )
	{
		BYTE bMagicLeve = min(m_bMagicGrade * TMAGIC_LEVEL_COUNT / (TMAGICLEVEL_MAX-1), TMAGIC_LEVEL_COUNT-1);
		WORD wSFX = m_pTITEM->m_wOptionSFX[bMagicLeve];

		LPTITEMMAGICSFX pItemMagicSfx = CTChart::FindTITEMMAGICSFX( wSFX );
		if( pItemMagicSfx )
			return pItemMagicSfx->m_wSFX[ m_pTBestOption->m_bSFX ];
	}

	return 0;
}
// ====================================================================================================
LPTITEMVISUAL CTClientItem::GetDefaultVisual()
{
	if( !CheckValid() )
		return NULL;

	return CTChart::FindTITEMVISUAL(m_pTITEM->m_wVisual[0]);
}
// ----------------------------------------------------------------------------------------------------
LPTITEMVISUAL CTClientItem::GetVisual()
{
	if( !CheckValid() )
		return NULL;

	return CTChart::FindTITEMVISUAL(m_pTITEM->m_wVisual[0]);
}
// ----------------------------------------------------------------------------------------------------
BYTE CTClientItem::GetVisualGrade()
{
	static const BYTE GRADE_RANGE[] = 
	{
		TGRADE_LEVEL_0,
		TGRADE_LEVEL_1,
		TGRADE_LEVEL_2,
		TGRADE_LEVEL_3,
		TGRADE_LEVEL_4,
	};

	if( GetGrade() )
	{
		for(BYTE i=0; i<TGRADE_LEVEL_COUNT-1; ++i)
		{
			if( GetGrade() < GRADE_RANGE[i+1] )
				return i;
		}

		return (TGRADE_LEVEL_COUNT-1);
	}

	return 0;
}
// ----------------------------------------------------------------------------------------------------
DWORD CTClientItem::GetVisualColor()
{
	static const DWORD GRADE_COLOR[] = 
	{
		TGRADE_COLOR_0,
		TGRADE_COLOR_1,
		TGRADE_COLOR_2,
		TGRADE_COLOR_3,
		TGRADE_COLOR_4,
	};

	return GRADE_COLOR[GetVisualGrade()];
}
// ====================================================================================================
void CTClientItem::SetItemSlot(CPacket* pPacket)
{
	(*pPacket)
		>> m_bItemID;
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::SetItemData(CPacket* pPacket, BOOL bUseEndTime)
{
	(*pPacket)
		>> m_wItemID
		>> m_bGrade
		>> m_bCount
		>> m_dwDuraMax
		>> m_dwDuraCurrent
		>> m_bRefineMax
		>> m_bRefineCurrent
		>> m_bCanGamble;
	
	if( bUseEndTime)
        (*pPacket) >> m_dEndTime;

	(*pPacket)
		>> m_bGradeEffect
		>> m_bELD
		>> m_bWrap
		>> m_wColor
		>> m_bRegGuild;

	m_pTITEM = CTChart::FindTITEMTEMP( m_wItemID );

	NotifyUpdate();
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::SetItemMagic(CPacket* pPacket)
{
	ClearMagic();

	BYTE bMagicCount;
	BYTE bMagicID;
	WORD wValue;
	
	(*pPacket)
		>> bMagicCount;

	for(BYTE i=0; i<bMagicCount; ++i)
	{
		(*pPacket)
			>> bMagicID
			>> wValue;

		AddMagicValue(bMagicID, wValue);
	}

	NotifyUpdate();
}
// ====================================================================================================
BOOL CTClientItem::CheckValid()
{
	if( m_pTITEM )
		return TRUE;

	TRACE("경고:주어진 아이디(%u)에 해당하는 아이템을 찾을 수 없습니다.\n", m_wItemID);
	return FALSE;
}
// ====================================================================================================

// protected fuctions
// ====================================================================================================
void CTClientItem::InitAttrVar()
{
	m_vMagicInfos.clear();

	m_fOptionLv		= 0.0f;
	m_bApplyAP		= FALSE;
	m_bApplyMAP		= FALSE;
	m_bApplyDP		= FALSE;
	m_bApplyMDP		= FALSE;
	m_wApplyMagicID	= 0;
	m_wAttrID		= m_pTITEM? m_pTITEM->m_wAttrID: 0;
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::CalcAttrID()
{
	InitAttrVar();

	m_wAttrID += CTChart::m_vTITEMGRADE[ GetGrade() ].m_bGrade;

	LPTITEMATTR pTITEMATTR = CTChart::FindTITEMATTR( m_wAttrID );
	if( pTITEMATTR )
	{
		CalcAttrID( *pTITEMATTR );

		if( m_bApplyMAP || m_bApplyMDP || m_bApplyAP || m_bApplyDP )
		{
			BYTE bLevel = GetPowerLevel();
			if( bLevel > pTITEMATTR->m_bGrade )
				m_wAttrID += bLevel - pTITEMATTR->m_bGrade;
		}
	}
}
// ----------------------------------------------------------------------------------------------------
void CTClientItem::CalcAttrID(const TITEMATTR& itemAttr)
{
	m_pTBestOption = NULL;
	m_bHasMainOption = FALSE;
	m_wApplyMagicID = 0;
	m_itemAttr = itemAttr;
	
	WORD wMaxVal = 0;
	FLOAT fMaxValue = 0.0f;
	FLOAT fTotalValue = 0.0f;

	MAPTMAGIC::iterator itrMag,endMag;
	itrMag = m_mapTMAGIC.begin();
	endMag = m_mapTMAGIC.end();

	for(; itrMag!=endMag; ++itrMag)
	{
		WORD wMagicID = itrMag->first;

		LPTITEMMAGIC pTITEMMAGIC = CTChart::FindTITEMMAGIC( wMagicID );
		if( !pTITEMMAGIC )
			continue;

		WORD wMagicValue = itrMag->second->m_wValue;

		FLOAT fCurValue = FLOAT(wMagicValue) * pTITEMMAGIC->m_fUtil;

		if( fMaxValue < fCurValue )
		{
			fMaxValue = fCurValue;
			m_pTBestOption = pTITEMMAGIC;
		}

		fTotalValue += fCurValue;

		if( pTITEMMAGIC->m_bSecOption )
		{
			TMAGICOPTION mo;
			mo.m_strOption = pTITEMMAGIC->m_strNAME;
			mo.m_wValue = wMagicValue;

			if( mo.m_wValue > 0 )
				m_vMagicInfos.push_back( mo );

			m_fOptionLv += fCurValue;
		}
		else
		{
			m_bHasMainOption = TRUE;

			if( wMaxVal < wMagicValue )
			{
				wMaxVal = wMagicValue;
				m_wApplyMagicID = wMagicID;
			}

			switch(wMagicID)
			{
			case MTYPE_LAP	:
			case MTYPE_PAP	:	
				m_bApplyAP = TRUE;
				m_itemAttr.m_wMinAP		+= wMagicValue;
				m_itemAttr.m_wMaxAP		+= wMagicValue;
				break;

			case MTYPE_MAP	:
				m_bApplyMAP = TRUE;
				m_itemAttr.m_wMinMAP	+= wMagicValue;
				m_itemAttr.m_wMaxMAP	+= wMagicValue;
				break;

			case MTYPE_PDP	:
				m_bApplyDP = TRUE;
				m_itemAttr.m_wDP		+= wMagicValue;
				break;

			case MTYPE_MDP	:
				m_bApplyMDP = TRUE;
				m_itemAttr.m_wMDP		+= wMagicValue;
				break;
			}
		}
	}
}
// ====================================================================================================
BYTE CTClientItem::GetWeaponPowerLevel()
{
	static FLOAT vPowerRate[IK_COUNT] = {
		0.0f,	// IK_NONE
		0.75f,	// IK_1HAND
		0.2f,	// IK_LHAND
		1.0f,	// IK_2HAND
		1.0f,	// IK_CAKRAM
		1.1f,	// IK_AX
		0.8f,	// IK_BOW
		1.0f,	// IK_ARBALEST
		0.8f,	// IK_MBAR
		1.0f,	// IK_MSTICK
		0.0f,	// IK_VAJRA
		0.0f,	// IK_MULTIVAJRA
		0.0f,	// IK_SHIELD
		0.0f,	// IK_CLOTH
		0.0f,	// IK_UNIFORM
		0.0f,	// IK_LLEATHER
		0.0f,	// IK_HLEATHER
		0.0f,	// IK_LARMOR
		0.0f,	// IK_HARMOR
		0.0f,	// IK_BACK
		0.0f,	// IK_NECK
		0.0f,	// IK_FINGER
		0.0f,	// IK_BALL
		0.0f,	// IK_PET
		0.0f,	// IK_ARROW
		0.0f,	// IK_SARROW
		0.0f,	// IK_HP
		0.0f,	// IK_MP
		0.0f,	// IK_AP
		0.0f,	// IK_UPGRADE
		0.0f,	// IK_DOWNGRADE
		0.0f,	// IK_MAGICGRADE
		0.0f,	// IK_RAREGRADE
		0.0f,	// IK_SKILL
		0.0f,	// IK_SMSGUILD
		0.0f,	// IK_SMSPERSON
		0.0f,	// IK_SKILLONEINIT
		0.0f,	// IK_SKILLALLINIT
		0.0f,	// IK_MEMBERRECALL
		0.0f,	// IK_MOVETOCHAR
		0.0f,	// IK_GOLDPREMIUM
		0.0f,	// IK_REVIVAL
		0.0f,	// IK_MAXHP
		0.0f};	// IK_MAXMP

	if( !CheckValid() )
		return 0;

	if( vPowerRate[m_pTITEM->m_bKind] == 0.0f )
		return BYTE(1);

	LPTFORMULA pTWEAPON = CTChart::FindTFORMULA(FTYPE_WEAPONPOWER);
	LPTFORMULA pT1STATTR = CTChart::FindTFORMULA(FTYPE_1ST);
	FLOAT fLevelConst = 1.0f / logf(pT1STATTR->m_fRateX);

	DWORD dwMinAP = m_itemAttr.m_wMinAP + m_itemAttr.m_wMinMAP;
	DWORD dwMaxAP = m_itemAttr.m_wMaxAP + m_itemAttr.m_wMaxMAP;

	BYTE bMinLevel = BYTE(logf(FLOAT(dwMinAP) / (vPowerRate[m_pTITEM->m_bKind] * pTWEAPON->m_fRateX)) * fLevelConst) + 1;
	BYTE bMaxLevel = BYTE(logf(FLOAT(dwMaxAP) / (vPowerRate[m_pTITEM->m_bKind] * pTWEAPON->m_fRateY)) * fLevelConst) + 1;

	return BYTE(max( (bMinLevel + bMaxLevel) / 2, 1));
}
// ----------------------------------------------------------------------------------------------------
BYTE CTClientItem::GetShieldPowerLevel()
{
	static FLOAT vPowerRate[IK_COUNT] = {
		0.0f,	// IK_NONE
		0.0f,	// IK_1HAND
		0.0f,	// IK_LHAND
		0.0f,	// IK_2HAND
		0.0f,	// IK_CAKRAM
		0.0f,	// IK_AX
		0.0f,	// IK_BOW
		0.0f,	// IK_ARBALEST
		0.0f,	// IK_MBAR
		0.0f,	// IK_MSTICK
		0.0f,	// IK_VAJRA
		0.8f,	// IK_MULTIVAJRA
		1.0f,	// IK_SHIELD
		1.3f,	// IK_CLOTH
		1.3f,	// IK_UNIFORM
		1.3f,	// IK_LLEATHER
		1.3f,	// IK_HLEATHER
		1.3f,	// IK_LARMOR
		1.3f,	// IK_HARMOR
		0.0f,	// IK_BACK
		0.0f,	// IK_NECK
		0.0f,	// IK_FINGER
		0.0f,	// IK_BALL
		0.0f,	// IK_PET
		0.0f,	// IK_ARROW
		0.0f,	// IK_SARROW
		0.0f,	// IK_HP
		0.0f,	// IK_MP
		0.0f,	// IK_AP
		0.0f,	// IK_UPGRADE
		0.0f,	// IK_DOWNGRADE
		0.0f,	// IK_MAGICGRADE
		0.0f,	// IK_RAREGRADE
		0.0f,	// IK_SKILL
		0.0f,	// IK_SMSGUILD
		0.0f,	// IK_SMSPERSON
		0.0f,	// IK_SKILLONEINIT
		0.0f,	// IK_SKILLALLINIT
		0.0f,	// IK_MEMBERRECALL
		0.0f,	// IK_MOVETOCHAR
		0.0f,	// IK_GOLDPREMIUM
		0.0f,	// IK_REVIVAL
		0.0f,	// IK_MAXHP
		0.0f};	// IK_MAXMP

	static FLOAT vPartsPower[ES_COUNT] = {
		0.0f,	// ES_PRMWEAPON
		0.3f,	// ES_SNDWEAPON
		0.0f,	// ES_LONGWEAPON
		0.2f,	// ES_HEAD
		0.0f,	// ES_BACK
		0.3f,	// ES_BODY
		0.25f,	// ES_PANTS
		0.15f,	// ES_FOOT
		0.1f,	// ES_HAND
		0.0f,	// ES_NECK
		0.0f,	// ES_LFINGER
		0.0f,	// ES_RFINGER
		0.0f,	// ES_LEAR
		0.0f,	// ES_REAR
		0.0f};	// ES_FACE

	if( !CheckValid() )
		return 0;

	if( vPartsPower[m_pTITEM->m_bPrmSlotID] == 0.0f ||
		vPowerRate[m_pTITEM->m_bKind] == 0.0f )
		return BYTE(1);

	LPTFORMULA pTSHIELD = CTChart::FindTFORMULA(FTYPE_ITEMPOWER);
	LPTFORMULA pT1STATTR = CTChart::FindTFORMULA(FTYPE_1ST);
	FLOAT fLevelConst = 1.0f / logf(pT1STATTR->m_fRateX);
	DWORD dwDP = m_itemAttr.m_wDP + m_itemAttr.m_wMDP;

	FLOAT fLevel = FLOAT(m_itemAttr.m_wDP + m_itemAttr.m_wMDP) / (vPowerRate[m_pTITEM->m_bKind] * vPartsPower[m_pTITEM->m_bPrmSlotID] * pTSHIELD->m_fRateX);
	fLevel = logf(fLevel) * fLevelConst + 1;

	if( fLevel < 0.0f )
		return 1;

	return max( BYTE(fLevel), 1);
}
// ----------------------------------------------------------------------------------------------------
BYTE CTClientItem::GetPowerLevel()
{
	if( CheckValid() )
	{
		switch(m_pTITEM->m_bType)
		{
		case IT_WEAPON		:
		case IT_LONG		: return GetWeaponPowerLevel();
		case IT_DEFENSIVE	:
		case IT_SHIELD		: return GetShieldPowerLevel();
		}
	}

	return BYTE(1);
}
// ====================================================================================================
void CTClientItem::CalcAttrNameAndLevel()
{
	if( !CheckValid() )
		return;

	m_wAttrLevel = m_pTITEM->m_bLevel;

	LPTITEMATTR pTITEMATTR = CTChart::FindTITEMATTR( m_wAttrID );
	if( pTITEMATTR )
	{
		if( m_wAttrLevel < pTITEMATTR->m_bGrade )
			m_wAttrLevel = pTITEMATTR->m_bGrade;
	}

	m_dwPrice = CalcPrice(m_pTITEM, m_wAttrLevel);
	m_wViewAttrLevel = CalcViewAttrLevel(m_wAttrLevel);
}
// ====================================================================================================

// ====================================================================================================
void CTClientItem::SetTITEM(LPTITEM p)
{
	m_pTITEM = p;
	
	if( p )
		m_wItemID = p->m_wItemID;
	else
		m_wItemID = NULL;

	NotifyUpdate();
}
void CTClientItem::SetItemSlot(BYTE p)
{
	m_bItemID = p;
}			
void CTClientItem::SetItemID(WORD p)
{
	m_wItemID = p;
	m_pTITEM = CTChart::FindTITEMTEMP( p );

	NotifyUpdate();
}			
void CTClientItem::SetCount(BYTE p)
{
	m_bCount = p;
}					
void CTClientItem::SetDuraMax(DWORD p)
{
	m_dwDuraMax = p;
}		
void CTClientItem::SetDuraCurrent(DWORD p)
{
	m_dwDuraCurrent = p;
}	
void CTClientItem::SetRefineMax(BYTE p)
{
	m_bRefineMax = p;
}
void CTClientItem::SetRefineCurrent(BYTE p)
{
	m_bRefineCurrent = p;
}
void CTClientItem::SetCanGamble(BYTE p)
{
	m_bCanGamble = p;
}
void CTClientItem::SetGrade(BYTE p)
{
	m_bGrade = p;

	NotifyUpdate();
}
void CTClientItem::SetGradeEffect(BYTE p)
{
	m_bGradeEffect = p;

	NotifyUpdate();
}
void CTClientItem::SetWrap( BYTE bWrap )
{
	m_bWrap = bWrap;

	NotifyUpdate();
}
void CTClientItem::SetELD( BYTE bELD )
{
	m_bELD = bELD;

	NotifyUpdate();
}
void CTClientItem::SetColor( WORD wColor )
{
	m_wColor = wColor;

	NotifyUpdate();
}
void CTClientItem::SetRegGuild( BYTE bRegGuild )
{
	m_bRegGuild = bRegGuild;

	NotifyUpdate();
}
void CTClientItem::SetEndTime( __time64_t endTime)
{
	m_dEndTime = endTime;
}
void CTClientItem::SetInstGradeMax(BYTE bMax)
{
	m_bInstGradeMax = bMax;
	NotifyUpdate();
}
// ====================================================================================================