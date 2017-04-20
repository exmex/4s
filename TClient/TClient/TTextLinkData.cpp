#include "Stdafx.h"
#include "TTextLinkData.h"


// ===============================================================================
const CHAR CTTextLinkData::REAL_REP_MARK = 0x02;
const CHAR CTTextLinkData::NULL_REP_MARK = 0x03;
// ===============================================================================
CTTextLinkData::CTTextLinkData()
	: m_nPos(0), m_pLastUnpackQuest(NULL), m_pLastUnpackItem(NULL)
{
}
// -------------------------------------------------------------------------------
CTTextLinkData::~CTTextLinkData()
{
	if(m_pLastUnpackItem)
		delete m_pLastUnpackItem;
}
// ===============================================================================
TTEXT_LINK_TYPE CTTextLinkData::GetType()
{
	UINT nBackup = m_nPos;
	m_nPos = 0;

	TTEXT_LINK_TYPE type;
	(*this) >> type;

	m_nPos = nBackup;
	return type;
}
// ===============================================================================
BOOL CTTextLinkData::IsItemType()
{
	TTEXT_LINK_TYPE t = GetType();
	return t == TTEXT_LINK_TYPE_ITEM ||
		t == TTEXT_LINK_TYPE_ITEM_OLD;
}
// ===============================================================================
void CTTextLinkData::PackQuest(LPTQUEST pQuest)
{
	(*this) << TTEXT_LINK_TYPE_QUEST << pQuest->m_dwID;
}
// -------------------------------------------------------------------------------
LPTQUEST CTTextLinkData::UnPackQuest()
{
	m_pLastUnpackQuest = NULL;
	ResetPosition();

	TTEXT_LINK_TYPE nType;
	DWORD dwID;

	(*this) >> nType;
	if( nType != TTEXT_LINK_TYPE_QUEST )
		return m_pLastUnpackQuest;

	(*this) >> dwID;
	m_pLastUnpackQuest = CTChart::FindTQUEST(dwID);

	return m_pLastUnpackQuest;
}
// ===============================================================================
void CTTextLinkData::PackItem(LPTITEM pItem, CTClientItem* pClientItem)
{
	(*this) 
		<< TTEXT_LINK_TYPE_ITEM 
		<< pItem->m_wItemID;
	
	if( pClientItem )
	{
		(*this) 
			<< pClientItem->GetGrade()
			<< pClientItem->GetDuraCurrent()
			<< pClientItem->GetDuraMax()
			<< pClientItem->GetRefineCurrent()
			<< pClientItem->GetRefineMax()
			<< pClientItem->CanGamble()
			<< pClientItem->GetWrap()
			<< pClientItem->GetELD();
	
		LPMAPTMAGIC pTMAGIC = pClientItem->GetTMAGIC();

		MAPTMAGIC::iterator itr,end;
		itr = pTMAGIC->begin();
		end = pTMAGIC->end();

		for(;itr!=end; ++itr)
		{
			(*this) 
				<< itr->first 
				<< itr->second->m_wValue;
		}
	}
}
// -------------------------------------------------------------------------------
CTClientItem* CTTextLinkData::UnPackItem()
{
	if(m_pLastUnpackItem)
	{
		delete m_pLastUnpackItem;
		m_pLastUnpackItem = NULL;
	}
	ResetPosition();

	TTEXT_LINK_TYPE nType;
	(*this) >> nType;

	if( nType != TTEXT_LINK_TYPE_ITEM )
		return m_pLastUnpackItem;

	WORD wItemID;
	(*this) >> wItemID;

	LPTITEM pItem = CTChart::FindTITEMTEMP(wItemID);
	if(!pItem)
		return m_pLastUnpackItem;

	m_pLastUnpackItem = new CTClientItem();
	m_pLastUnpackItem->SetItemID(wItemID);
	m_pLastUnpackItem->ClearMagic();

	if(!IsEndOfPosition())
	{
		BYTE bGrade;
		DWORD dwDuraCurrent;
		DWORD dwDuraMax;
		BYTE bRefineCurrent;
		BYTE bRefineMax;
		BYTE bCanGamble;
		BYTE bWrap;
		BYTE bELD;

		(*this)
			>> bGrade
			>> dwDuraCurrent
			>> dwDuraMax
			>> bRefineCurrent
			>> bRefineMax
			>> bCanGamble
			>> bWrap
			>> bELD;

		m_pLastUnpackItem->SetGrade(bGrade);
		m_pLastUnpackItem->SetDuraCurrent(dwDuraCurrent);
		m_pLastUnpackItem->SetDuraMax(dwDuraMax);
		m_pLastUnpackItem->SetRefineCurrent(bRefineCurrent);
		m_pLastUnpackItem->SetRefineMax(bRefineMax);
		m_pLastUnpackItem->SetCanGamble(bCanGamble);
		m_pLastUnpackItem->SetWrap( bWrap );
		m_pLastUnpackItem->SetELD( bELD );
		// jkchoi
	}

	while(!IsEndOfPosition())
	{
		BYTE bMagicID;
		WORD wValue;

		(*this)
			>> bMagicID 
			>> wValue;

		if( wValue != 0 )
			m_pLastUnpackItem->AddMagicValue( bMagicID, wValue);
	}

	return m_pLastUnpackItem;
}
// -------------------------------------------------------------------------------
CTClientItem* CTTextLinkData::UnPackItemOLD()
{
	if(m_pLastUnpackItem)
	{
		delete m_pLastUnpackItem;
		m_pLastUnpackItem = NULL;
	}
	ResetPosition();

	TTEXT_LINK_TYPE nType;
	(*this) >> nType;

	if( nType != TTEXT_LINK_TYPE_ITEM_OLD )
		return m_pLastUnpackItem;

	WORD wItemID;
	(*this) >> wItemID;

	LPTITEM pItem = CTChart::FindTITEMTEMP(wItemID);
	if(!pItem)
		return m_pLastUnpackItem;

	m_pLastUnpackItem = new CTClientItem();
	m_pLastUnpackItem->SetItemID(wItemID);
	m_pLastUnpackItem->ClearMagic();

	if(!IsEndOfPosition())
	{
		BYTE bGrade;
		DWORD dwDuraCurrent;
		DWORD dwDuraMax;
		BYTE bRefineCurrent;
		BYTE bRefineMax;
		BYTE bCanGamble;

		(*this)
			>> bGrade
			>> dwDuraCurrent
			>> dwDuraMax
			>> bRefineCurrent
			>> bRefineMax
			>> bCanGamble;

		m_pLastUnpackItem->SetGrade(bGrade);
		m_pLastUnpackItem->SetDuraCurrent(dwDuraCurrent);
		m_pLastUnpackItem->SetDuraMax(dwDuraMax);
		m_pLastUnpackItem->SetRefineCurrent(bRefineCurrent);
		m_pLastUnpackItem->SetRefineMax(bRefineMax);
		m_pLastUnpackItem->SetCanGamble(bCanGamble);
	}

	while(!IsEndOfPosition())
	{
		BYTE bMagicID;
		WORD wValue;

		(*this)
			>> bMagicID 
			>> wValue;

		if( wValue != 0 )
			m_pLastUnpackItem->AddMagicValue( bMagicID, wValue);
	}

	return m_pLastUnpackItem;
}