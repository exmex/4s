#include "StdAfx.h"
#include ".\tchatitem.h"

MAPTITEMTEMP CTChatItem::m_mapTITEMTEMP;


CTChatItem::CTChatItem()
{
	m_pTITEM = NULL;

	m_bItemID = 0;
	m_wItemID = 0;
	m_bCount = 0;
	m_bGrade = 0;

	m_dwRune = 0;
	m_dwLuna = 0;
	m_dwCron = 0;
}

CTChatItem::~CTChatItem()
{
}

LPTITEM CTChatItem::FindTItem( WORD wItemID)
{
	MAPTITEMTEMP::iterator finder = m_mapTITEMTEMP.find(wItemID);

	if( finder != m_mapTITEMTEMP.end() )
		return (*finder).second;

	return NULL;
}

void CTChatItem::SetItemID(WORD p)
{
	m_wItemID = p;
	m_pTITEM = FindTItem(p);
}			

void CTChatItem::ClearMagic()
{
	/*MAPTMAGIC::iterator itTMAGIC;

	for( itTMAGIC = m_mapTMAGIC.begin(); itTMAGIC != m_mapTMAGIC.end(); itTMAGIC++)
		delete (*itTMAGIC).second;

	m_mapTMAGIC.clear();*/
}

void CTChatItem::SetGrade(BYTE p)
{
	m_bGrade = p;
}

void CTChatItem::SetDuraCurrent(DWORD p)
{
	//m_dwDuraCurrent = p;
}	

void CTChatItem::SetDuraMax(DWORD p)
{
	//m_dwDuraMax = p;
}		

void CTChatItem::SetRefineCurrent(BYTE p)
{
	//m_bRefineCurrent = p;
}

void CTChatItem::SetRefineMax(BYTE p)
{
	//m_bRefineMax = p;
}

void CTChatItem::SetCanGamble(BYTE p)
{
	//m_bCanGamble = p;
}

void CTChatItem::AddMagicValue(BYTE bMagicID, WORD wValue, DWORD dwTime)
{
	/*MAPTMAGIC::iterator finder = m_mapTMAGIC.find(bMagicID);
	
	if( finder != m_mapTMAGIC.end() )
	{
		LPTMAGIC pTMAGIC =  (*finder).second;
		
		pTMAGIC->m_wValue += wValue;
		pTMAGIC->m_dwTime = max(pTMAGIC->m_dwTime, dwTime);
	}
	else
	{
		LPTMAGIC pTMAGIC = new TMAGIC();
		
		pTMAGIC->m_wValue = wValue;
		pTMAGIC->m_dwTime = dwTime;

		m_mapTMAGIC.insert( make_pair(bMagicID, pTMAGIC) );
	}*/

	
}