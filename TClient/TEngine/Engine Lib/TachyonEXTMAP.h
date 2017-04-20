#pragma once


class CTachyonEXTMAP : public CTachyonMAP
{
public:
	LPDIRECT3DSURFACE9 m_pTILETEX;
	LPDIRECT3DSURFACE9 m_pGEOTEX;
	VECTORSURFACE m_vSHADOWTEX;

	LPWORD m_pSHADOW;
	LPBYTE m_pREGION;
	LPBYTE m_pDETAIL;
	VECTORWORD m_vDETAIL;

	FLOAT* m_pDATA;

	int m_nTileCount;

public:
	WORD FindDetailID(
		int nTileX,
		int nTileZ);
	WORD FindDetailID( int nIndex );
	void Release();

public:
	virtual FLOAT GetHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

public:
	CTachyonEXTMAP();
	virtual ~CTachyonEXTMAP();
};
