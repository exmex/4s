#pragma once


class CTMapViewerInven
{
public:
	MAPTITEM m_mapTITEM;

public:
	CTMapViewerItem *FindTItem( BYTE bItemID);
	void ClearInven();

public:
	CTMapViewerInven();
	virtual ~CTMapViewerInven();
};
