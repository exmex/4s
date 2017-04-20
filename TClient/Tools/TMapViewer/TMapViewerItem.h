#pragma once


class CTMapViewerItem
{
public:
	static MAPTITEMTEMP m_mapTITEMTEMP;	// Item template data

public:
	static LPTITEM FindTItem( WORD wItemID);

public:
	LPTITEM m_pTITEM;

	BYTE m_bItemID;
	WORD m_wItemID;

public:
	CTMapViewerItem();
	virtual ~CTMapViewerItem();
};
