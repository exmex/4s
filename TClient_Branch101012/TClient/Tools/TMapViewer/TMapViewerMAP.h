#pragma once


class CTMapViewerMAP
{
public:
	static MAPTREGIONINFO m_mapTREGIONINFO;
	static MAPTMAPINFO m_mapTMAPINFO;
	static MAPTMINIMAP m_mapTMINIMAP;
	static LPDIRECT3DTEXTURE9 m_pTPOS;

	static FLOAT m_fClimbBound;
	static FLOAT m_fMoveBound;
	static FLOAT m_fFallBound;
	static FLOAT m_fGravity;

public:
	static LPTREGIONINFO GetRegionINFO( DWORD dwRegionID);
	static LPTMAPINFO GetMapINFO( DWORD dwMapID);

public:
	VTMAPOBJ m_vTMAPOBJ[9][TOBJCELL_COUNT];
	VTMAPSFX m_vTMAPSFX[9][TOBJCELL_COUNT];
	VTMAPSND m_vTMAPSND[9][TOBJCELL_COUNT];

	TOBJBLOCK m_vTSILHOUETTE[9];
	TOBJBLOCK m_vBIGOBJ[9];
	TBSPBLOCK m_vTBSP[9];

	TOBJBLOCK m_vTOBJ[TDRAWCELL_COUNT];
	TSFXBLOCK m_vTSFX[TDRAWCELL_COUNT];
	TSNDBLOCK m_vTSND[TDRAWCELL_COUNT];

	WORD m_vTCELL[TDRAWCELL_COUNT];
	BYTE m_vLOCK[TDRAWCELL_COUNT];

public:
	CTachyonHUGEMAP *m_pMAP;
	BYTE m_bSHADOW;

	MAPTBSP m_mapTBSP;
	FLOAT m_fTSCALE;

	DWORD m_dwTMapCOLOR;
	DWORD m_dwBspID;
	WORD m_wMapID;

	VTOBJBASE m_vDRAWOBJ;
	VTOBJBASE m_vWATER;

	LISTTOBJBASE m_vTDRAW;
	VTPATHFLAG m_vTPATH;
	VTBSP m_vDRAWBSP;

	CTachyonRes *m_pRESCHAR;
protected:
	int m_nTCellX;
	int m_nTCellZ;

public:
	FLOAT GetAttrFLOAT(
		LPMAPOBJ pTOBJ,
		DWORD dwID);

	int GetAttrINT(
		LPMAPOBJ pTOBJ,
		DWORD dwID);

public:
	void LoadMAPOBJ(
		CD3DDevice *pDevice,
		LPD3DXVECTOR3 pPOS);
	void ReleaseMAP();

	void LoadMAP(
		CD3DDevice *pDevice,
		CTMapViewerCAM *pCamera,
		CTachyonRes *pRES,
		WORD wMapID);

	void LoadMAPOBJ(
		CD3DDevice *pDevice,
		LPVTMAPOBJ pTMAPOBJ,
		LPVTMAPSFX pTMAPSFX,
		LPVTMAPSND pTMAPSND,
		LPTOBJBLOCK pTOBJ,
		LPTSFXBLOCK pTSFX,
		LPTSNDBLOCK pTSND);

	void ClearMAPOBJ(
		LPTOBJBLOCK pTOBJ,
		LPTSFXBLOCK pTSFX,
		LPTSNDBLOCK pTSND);

	void CheckMAPOBJ(
		CD3DDevice *pDevice,
		LPD3DXVECTOR3 pPOS);

	void CalcMAPOBJ(
		CD3DDevice *pDevice,
		CTMapViewerCAM *pCamera,
		DWORD dwTick);

	void LoadTFLAG( BYTE bINDEX);

	void ClearTSILHOUETTE( BYTE bINDEX);
	void ClearBIGOBJ( BYTE bINDEX);
	void ClearTBSP( BYTE bINDEX);
	void ClearTFLAG( BYTE bINDEX);

	FLOAT GetIntensity();
	FLOAT GetSpecRange();
	FLOAT GetSpecMax();

	FLOAT GetLandHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	FLOAT GetWaterHeight(
		FLOAT fPosX,
		FLOAT fPosZ);

	FLOAT GetHeight(
		LPVTOBJBASE pTMAPOBJ,
		CTMapViewerObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT,
		FLOAT fMove,
		BYTE bCheckBound);

	DWORD GetTStepSND(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	D3DXPLANE GetBoundPlane(
		LPD3DXVECTOR3 pPREV,
		LPD3DXVECTOR3 pNEXT);

	LPTREGIONINFO GetRegionINFO(
		BYTE bContryID,
		FLOAT fPosX,
		FLOAT fPosZ);

	BYTE HitTest(
		FLOAT *pDIST,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	BYTE HitTestHeight(
		FLOAT *pDIST,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	BYTE GetRegionID(
		FLOAT fPosX,
		FLOAT fPosZ);

	BYTE IsEXTVisible();
	BYTE PtInSHADOW(
		FLOAT fPosX,
		FLOAT fPosZ);

	BYTE CanMove(
		VTOBJBASE& vOBJ,
		CTMapViewerObjBase *pOBJ,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pTARGET);

	BYTE CheckCollision(
		CTMapViewerObjBase *pOBJ,
		LPD3DXVECTOR3 pPREV);

	void ResetVisibleOBJ(
		CD3DDevice *pDevice,
		CTMapViewerObjBase *pTOBJ,
		CTMapViewerCAM *pCamera,
		DWORD dwTick);

	void ResetVisibleOBJ(
		CD3DDevice *pDevice,
		CTMapViewerCAM *pCamera,
		DWORD dwTick);

	void ResetTMAP(
		CD3DDevice *pDevice,
		CTMapViewerCAM *pCamera,
		DWORD dwTick);

	void ResetTMINIMAP(
		LPDIRECT3DTEXTURE9 pTARGET,
		CTMapViewerChar *pHost,
		CD3DDevice *pDevice,
		CTMapViewerCAM *pCamera);

	BYTE ResetTMINIMAP(
		CD3DDevice *pDevice,
		WORD wMapID,
		FLOAT fCenterX,
		FLOAT fCenterZ,
		FLOAT fSCALE);

	void RenderWATER(
		CD3DDevice *pDevice,
		CTMapViewerCAM *pCamera);

	void RenderBSP( CTMapViewerCAM *pCamera);
	void Render( CD3DDevice *pDevice);

public:
	CTMapViewerMAP();
	virtual ~CTMapViewerMAP();
};
