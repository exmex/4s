#pragma once


class CTInven
{
public:
	MAPTITEM m_mapTITEM;
	LPTITEM m_pTITEM;

	BYTE m_bInvenID;
	WORD m_wItemID;
	__time64_t m_dEndTime;
	BYTE m_bELD;

public:
	CTItem *FindTItem( BYTE bItemID);

	BYTE GetEasePos( CTItem *pTItem);
	BYTE GetBlankPos();

public:
	CTInven();
	virtual ~CTInven();
};
