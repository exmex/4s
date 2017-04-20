#pragma once


class CTMapViewerSFX : public CTachyonSFX
{
public:
	static MAPTSFXTEMP m_mapTSFXTEMP;

public:
	static LPTSFXTEMP GetTSFXTEMP( DWORD dwFuncID);

public:
	DWORD m_dwID;

public:
	CTMapViewerSFX();
	virtual ~CTMapViewerSFX();
};
