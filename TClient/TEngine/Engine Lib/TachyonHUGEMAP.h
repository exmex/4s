#pragma once

struct tagIPOINT
{
	union
	{
		struct
		{
			SHORT m_nX;
			SHORT m_nY;
		};

		DWORD m_dwID;
	};

	tagIPOINT( SHORT nX, SHORT nY)
	{
		m_nX = nX;
		m_nY = nY;
	};

	tagIPOINT()
	{
		m_dwID = 0;
	};
};

struct tagIRECT
{
	union
	{
		SHORT m_nPOS[4];
		DWORD m_dwID;

		struct
		{
			SHORT m_nLEFT;
			SHORT m_nTOP;
			SHORT m_nRIGHT;
			SHORT m_nBOTTOM;
		};
	};

	SHORT Height()
	{
		return m_nBOTTOM - m_nTOP;
	};

	SHORT Width()
	{
		return m_nRIGHT - m_nLEFT;
	};

	IPOINT Center()
	{
		return IPOINT( m_nLEFT + Width() / 2, m_nTOP + Height() / 2);
	};

	tagIRECT()
	{
		for( int i=0; i<4; i++)
			m_nPOS[i] = 0;
	};
};

struct tagBLOCK
{
	VECTORIPOINT m_vPOINT;
	VECTORBYTE m_vPOLY;

	IRECT m_vRECT;
	WORD m_wTILE;

	FLOAT m_fRANGE;
	FLOAT m_fDIR;
	FLOAT m_fDIST;

	SHORT Height()
	{
		return m_vRECT.Height();
	};

	SHORT Width()
	{
		return m_vRECT.Width();
	};

	IPOINT Center()
	{
		return m_vRECT.Center();
	};

	DWORD GetID()
	{
		return m_vRECT.m_dwID;
	};

	tagBLOCK()
	{
		m_vPOINT.clear();
		m_vPOLY.clear();

		m_wTILE = TILE_NULL;
		m_fRANGE = 0.0f;
		m_fDIR = 0.0f;
		m_fDIST = 0.0f;
	};

	~tagBLOCK()
	{
		while(!m_vPOINT.empty())
		{
			delete m_vPOINT.back();
			m_vPOINT.pop_back();
		}

		m_vPOLY.clear();
	};
};

typedef vector<LPMAPDWORD>		VECTORMAPDWORD, *LPVECTORMAPDWORD;


class CTachyonHUGEMAP : public CTachyonMAP
{
public:
	static FLOAT m_fCullDIST;

	static int m_nDiffuseCount;
	static int m_nLODBound;
	static int m_nIndexCount;

	static int m_nMaxLength;
	static int m_nMaxLevel;

public:
	LPDIRECT3DVERTEXBUFFER9 m_pPOSVB;
	LPDIRECT3DVERTEXBUFFER9 m_pUVVB;

	LPDIRECT3DTEXTURE9 m_pSHADOWTEX;
	LPDIRECT3DTEXTURE9 m_pTILETEX;
	LPDIRECT3DTEXTURE9 m_pGEOTEX;

	LPDIRECT3DINDEXBUFFER9 m_pINDEX;
	LPVOID m_pINDEXBUF;
	LPBYTE m_pENABLE;

	CTachyonEXTMAP *m_pUNIT[9];
	CTachyonEXTMAP m_UNIT[9];
	CTachyonRes *m_pRES;

	MAPDWORD m_mapDIFFUSEINDEX;
	MAPOBJECT m_mapOBJECT[9];
	MAPDWORD m_mapMAPSFX[9];
	MAPSNDIDX m_mapMAPSND[9];

	VECTORTEXTURESET m_vDETAILDATA;
	VECTORTEXTURESET m_vDETAIL;
	VECTORBLOCK m_vBLOCK;
	LPBLOCK *m_pBLOCK;

	VECTORDWORD m_vDIFFUSELOCK;
	VECTORBYTE m_vDIFFUSEID;
	VECTORPTR m_vDIFFUSEBUF;
	VECTORPTR m_vBLENDTILE;
	VECTORVB m_vDIFFUSE;

	VECTORFLOAT m_vDETAILSDATA;
	VECTORFLOAT m_vDETAILS;

	VECTORDWORD m_vBSTART;
	VECTORDWORD m_vSTART;

	VECTORDWORD m_vBLEND;
	VECTORDWORD m_vCOUNT;

	// Cell management variable
	BYTE m_bINDEX[9];
	BYTE m_bLOCK[9];

	// Unit management variable
	BYTE m_bUPDATE[9];
	BYTE m_bUNIT[9];
	BYTE m_bLOAD[9];

	int m_nTextureSize;
	int m_nShadowPixel;
	int m_nTileLength;
	int m_nUnitLength;
	int m_nUnitCountX;
	int m_nUnitCountZ;
	int m_nBlockCount;
	int m_nCellCount;

	int m_nUnitX;
	int m_nUnitZ;

	int m_nCellX;
	int m_nCellZ;

public:
	IPOINT m_vCENTER;
	IPOINT m_vPIVOT;
	FLOAT m_fDIR;

public:
	D3DXMATRIX m_vSHADOWTRANS;
	D3DXVECTOR3 m_vLIGHT;
	DWORD m_dwID;

	FLOAT m_fIntensity;
	FLOAT m_fSpecRange;
	FLOAT m_fSpecMax;

	BYTE m_bDrawShadow;
	BYTE m_bUseSHADER;
	BYTE m_bResetVB;
	BYTE m_bSHADOW;

public:
	void InitVB( CD3DDevice *pDevice);
	void DeleteForRestore();
	void Release();

	void Restore(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera);

	void Render(
		CD3DDevice *pDevice);

	void ResetVB(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera,
		BYTE bCullView,
		BYTE bTargetLOD);
	void ResetUV();

	void ResetTEX(
		LPDIRECT3DDEVICE9 pDevice,
		LPIPOINT pCENTER);

	void LoadUNIT(
		LPDIRECT3DDEVICE9 pDevice,
		CTachyonEXTMAP *pUNIT,
		int nUnitID);

	void LoadVB(
		CTachyonEXTMAP *pUNIT,
		BYTE bCELL,
		int nUnitX,
		int nUnitZ,
		int nCellX,
		int nCellZ);

	void ReleaseUNIT( CTachyonEXTMAP *pUNIT);
	void FreeDIFFUSE( BYTE bINDEX);
	BYTE FindFreeUNIT();
	BYTE FindFreeCELL();

	void BuildDIFFUSE(
		LPDIRECT3DDEVICE9 pDevice,
		BYTE bCELL,
		int nCellX,
		int nCellZ);

	BYTE HitTest(
		LPD3DXVECTOR3 pResult,
		LPD3DXVECTOR3 pPOS,
		LPD3DXVECTOR3 pDIR);

	BYTE GetRegionID(
		int nTileX,
		int nTileZ);

	WORD FindDetailID(
		int nTileX,
		int nTileZ);

	BYTE PtInSHADOW(
		FLOAT fPosX,
		FLOAT fPosZ);

	void BuildBLOCKIDX(
		LPMAPDWORD pTILE,
		LPIPOINT pCENTER,
		FLOAT fRANGE,
		FLOAT fDIR,
		BYTE bCullView);

	void ResetVB32(
		CD3DDevice *pDevice,
		LPMAPDWORD pTILE,
		LPIPOINT pCENTER);

	void ResetVB16(
		CD3DDevice *pDevice,
		LPMAPDWORD pTILE,
		LPIPOINT pCENTER);

protected:
	void BuildBLOCKDIR( LPBLOCK pBLOCK);
	void BuildBLOCK( CRect rect);
	void ComplateBLOCK();
	int GetLODLevel( CRect rect);

	BYTE FindFreeDIFFUSE( LPDIRECT3DDEVICE9 pDevice);
	BYTE IsValidBLOCK(
		LPBLOCK pBLOCK,
		LPIPOINT pCENTER);

	BYTE BLOCKInVIEW(
		LPBLOCK pBLOCK,
		FLOAT fDIR,
		FLOAT fRANGE);

	LPDWORD GetDIFFUSEBUF(
		LPDIRECT3DDEVICE9 pDevice,
		BYTE bCELL,
		WORD wTileID);

	DWORD GetVBIndex32(
		LPIPOINT pCENTER,
		LPBLOCK pBLOCK,
		BYTE bPOINT);

	WORD GetVBIndex16(
		LPIPOINT pCENTER,
		LPBLOCK pBLOCK,
		BYTE bPOINT);

	void InitForNULLTILE();
	void BlendTILE(
		LPDIRECT3DDEVICE9 pDevice,
		LPIPOINT pPOINT,
		LPDWORD pDIFFUSE,
		WORD wTileID);

	void CheckVB(
		LPDIRECT3DDEVICE9 pDevice,
		LPIPOINT pPIVOT);

public:
	virtual FLOAT GetHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

public:
	CTachyonHUGEMAP();
	virtual ~CTachyonHUGEMAP();
};
