#pragma once

#include "TChatItem.h"

class CTChatInven
{
public:
	MAPTITEM m_mapTITEM;
	LPTITEM m_pTITEM;

	BYTE m_bInvenID;
	WORD m_wItemID;

public:
	CTChatItem *FindTItem( BYTE bItemID);
	void ClearInven();

public:
	CTChatInven();
	virtual ~CTChatInven();
};
