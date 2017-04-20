#pragma once


class CTachyonMAP
{
public:
	MAPMAPOBJ m_mapOBJ;
	MAPMAPSFX m_mapSFX;
	MAPMAPSND m_mapSND;
	BYTE m_bMAPType;

public:
	LPBYTE m_pDetail;

public:
	virtual FLOAT GetHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

public:
	CTachyonMAP();
	~CTachyonMAP();
};
