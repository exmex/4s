#pragma once


class CTExplorerInven
{
public:
	MAPTITEM m_mapTITEM;

public:
	CTExplorerItem *FindTItem( BYTE bItemID);
	void ClearInven();

public:
	CTExplorerInven();
	virtual ~CTExplorerInven();
};
