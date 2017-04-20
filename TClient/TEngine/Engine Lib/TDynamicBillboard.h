#pragma once


class CTDynamicBillboard
{
protected:
	static LPDIRECT3DSURFACE9 m_pZBUF;

	static MAPTDBB m_mapTACTIVE;
	static VTDBB m_vTUPDATE;
	static VTDBB m_vTRENDER;

public:
	static FLOAT m_fMinDIST;
	static BYTE m_bLevel;

protected:
	static FLOAT CalcSIZE(
		FLOAT fRadius,
		FLOAT fHeight);

	static int CalcPixel(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera,
		FLOAT fDist,
		FLOAT fRadius,
		FLOAT fHeight);

public:
	static BYTE InitTDBB( CD3DDevice *pDevice);
	static BYTE CheckInit();
	static void ReleaseTDBB();

	static void ClearTACTIVE();
	static void ClearBUF();

	static BYTE Restore( CD3DDevice *pDevice);
	static void DeleteForRestore();

	static DWORD GetTDBBCount();
	static void UpdateBUF(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera);

	static void RenderBUF(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera);

	static BYTE CanUSE(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera,
		LPD3DXVECTOR3 pTPOS,
		FLOAT fRadius,
		FLOAT fHeight);

protected:
	LPDIRECT3DTEXTURE9 m_pTEX;

	CTachyonObject *m_pTOBJ;
	DWORD m_dwTick;

	FLOAT m_fSIZE;
	BYTE m_nTEX;

protected:
	D3DXVECTOR3 GetTOBJPOS();

public:
	BYTE CreateDynamicBillboard(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera,
		CTachyonObject *pTOBJ,
		FLOAT fRadius,
		FLOAT fHeight);

	void CalcTick( DWORD dwTick);
	void ResetTick();
	void Release();

public:
	CTDynamicBillboard();
	virtual ~CTDynamicBillboard();
};
