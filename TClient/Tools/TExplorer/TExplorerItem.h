#pragma once


class CTExplorerItem
{
public:
	static LPTITEMVISUAL FindTItemVisual( WORD wVisualID);
	static LPTITEMVISUAL GetDefaultVisual( LPTITEM pTITEM);

public:
	LPTITEM m_pTITEM;

	BYTE m_bItemID;
	WORD m_wItemID;

	LPTITEMVISUAL GetVisual();

public:
	CTExplorerItem();
	virtual ~CTExplorerItem();
};
