#pragma once


class CTClientInven
{
public:
	MAPTITEM m_mapTITEM;
	LPTITEM m_pTITEM;

	BYTE m_bInvenID;
	WORD m_wItemID;
	__time64_t m_dEndTime;

public:
	CTClientItem *FindTItem( BYTE bSlot);
	BOOL CheckTItem( WORD wItemID, BYTE* pbItemID=NULL );

	BYTE GetEasePos( CTClientItem *pTItem);
	BYTE GetBlankPos();
	BYTE GetItemPos(TITEM_TYPE eType, TITEM_KIND eKind);

	void ClearInven();
	CTClientInven* Clone();

public:
	CTClientInven();
	virtual ~CTClientInven();
};
