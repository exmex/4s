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
// -------------------------------------------------------------------------------
LPTQUEST CTTextLinkData::UnPackQuest()
{
	m_pLastUnpackQuest = NULL;
	/*ResetPosition();

	TTEXT_LINK_TYPE nType;
	DWORD dwID;

	(*this) >> nType;
	if( nType != TTEXT_LINK_TYPE_QUEST )
		return m_pLastUnpackQuest;

	(*this) >> dwID;
	m_pLastUnpackQuest = CTClientQuest::FindTQuest(dwID);*/

	return m_pLastUnpackQuest;
}
// -------------------------------------------------------------------------------
CTChatItem* CTTextLinkData::UnPackItem()
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

	LPTITEM pItem = CTChatItem::FindTItem(wItemID);
	if(!pItem)
		return m_pLastUnpackItem;

	m_pLastUnpackItem = new CTChatItem();
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

		m_pLastUnpackItem->AddMagicValue( bMagicID, wValue);
	}

	return m_pLastUnpackItem;
}

CString MakeNetToLinkText( const CString& strNetMsg)
{
	WORD wLENGTH = strNetMsg.GetLength();

	if( wLENGTH < TCHAT_DWORD_SIZE )
		return CString(_T(""));

	DWORD dwCODE = 0;
	WORD wHEAD = 0;
	WORD wBODY = 0;
	WORD wBASE = 0;

	sscanf( LPCSTR(strNetMsg.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
	wHEAD = HIWORD(dwCODE);
	wBODY = LOWORD(dwCODE);

	if( wHEAD + wBODY + TCHAT_DWORD_SIZE != wLENGTH )
		return CString(_T(""));

	CString strBODY = strNetMsg.Mid( TCHAT_DWORD_SIZE + wHEAD, wBODY);
	CString strHEAD = strNetMsg.Mid( TCHAT_DWORD_SIZE, wHEAD);
	wLENGTH = strHEAD.GetLength();

	while( wLENGTH >= TCHAT_DWORD_SIZE )
	{
		CString strNAME(_T(""));

		sscanf( LPCSTR(strHEAD.Left(TCHAT_DWORD_SIZE)), _T("%X"), &dwCODE);
		wBODY = LOWORD(dwCODE) + wBASE;
		wHEAD = HIWORD(dwCODE);

		if( TCHAT_DWORD_SIZE + wHEAD > wLENGTH ||
			wBODY > strBODY.GetLength() )
			return strBODY;

		CTTextLinkData Link;
		Link.FromStr(strHEAD.Mid( TCHAT_DWORD_SIZE, wHEAD));

		switch(Link.GetType())
		{
		case TTEXT_LINK_TYPE_QUEST	:
			{
				LPTQUEST pQuest = Link.UnPackQuest();

				if(pQuest)
				{
					strNAME = CString(LINK_STR_START) + pQuest->m_strTITLE + CString(LINK_STR_END);
				}
			}

			break;

		case TTEXT_LINK_TYPE_ITEM	:
			{
				CTChatItem *pItem = Link.UnPackItem();

				if(pItem)
				{
					strNAME = CString(LINK_STR_START) + pItem->m_pTITEM->m_strNAME + CString(LINK_STR_END);
				}
			}

			break;
		}

		WORD wLEN = strNAME.GetLength();
		if(wLEN)
		{
			strBODY.Insert( wBODY, LPCSTR(strNAME));
			wBASE += wLEN;
		}

		strHEAD = strHEAD.Mid(TCHAT_DWORD_SIZE + wHEAD);
		wLENGTH = strHEAD.GetLength();
	}

	return strBODY;
}