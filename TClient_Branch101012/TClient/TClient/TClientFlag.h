#pragma once


class CTClientFlag
{
public:
	static MAPTPATHFLAG m_mapTPATHFLAG;

public:
	static CTClientFlag *FindTFLAG( DWORD dwID);

	static void ReleaseTPATH();
	static void InitTPATH();

public:
	VECTORDWORD m_vPATH;
	D3DXVECTOR3 m_vPOS;
	DWORD m_dwID;

public:
	CTClientFlag();
	virtual ~CTClientFlag();
};
