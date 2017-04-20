#pragma once


class CTMapViewerKEY
{
public:
	DWORD m_vTICK[TKEY_COUNT];
	BYTE m_vSTATE[TKEY_COUNT];
	MAPWORD m_mapKEY;

	CPoint m_point;
	DWORD m_dwTick;

public:
	WORD GetTKEY( WORD wKEY);
	BYTE GetCTRL();

	void ResetSTATE();
	void InitKEY();

public:
	CTMapViewerKEY();
	virtual ~CTMapViewerKEY();
};
