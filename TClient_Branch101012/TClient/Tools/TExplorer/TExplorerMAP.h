#pragma once


class CTExplorerMAP
{
public:
	static MAPTMINIMAP m_mapTMINIMAP;
	static LPDIRECT3DTEXTURE9 m_pTPOS;

	static MAPDWORD m_mapTBTNID2UNIT;
	static MAPDWORD m_mapTBTNID2WORLD;

	static FLOAT m_fClimbBound;
	static FLOAT m_fMoveBound;
	static FLOAT m_fFallBound;
	static FLOAT m_fGravity;

public:
	VTMAPOBJ m_vTMAPOBJ[9][TOBJCELL_COUNT];
	VTMAPSFX m_vTMAPSFX[9][TOBJCELL_COUNT];
	VTMAPSND m_vTMAPSND[9][TOBJCELL_COUNT];
	VTNPC m_vTMAPNPC[9][TOBJCELL_COUNT];

	TOBJBLOCK m_vTSILHOUETTE[9];
	TOBJBLOCK m_vBIGOBJ[9];
	TBSPBLOCK m_vTBSP[9];

	TOBJBLOCK m_vTOBJ[TDRAWCELL_COUNT];
	TOBJBLOCK m_vTNPC[TDRAWCELL_COUNT];
	TSFXBLOCK m_vTSFX[TDRAWCELL_COUNT];
	TSNDBLOCK m_vTSND[TDRAWCELL_COUNT];

	WORD m_vTCELL[TDRAWCELL_COUNT];
	BYTE m_vLOCK[TDRAWCELL_COUNT];

public:
	CTachyonHUGEMAP *m_pMAP;
	BYTE m_bSHADOW;
	BYTE m_bHouseMesh;

	MAPOBJECT m_mapTNPC;
	MAPTBSP m_mapTBSP;
	FLOAT m_fTSCALE;

	DWORD m_dwTMapCOLOR;
	DWORD m_dwHouseID;
	DWORD m_dwBspID;
	WORD m_wMapID;

	VTOBJBASE m_vDRAWOBJ;
	VTOBJBASE m_vWATER;

	LISTTOBJBASE m_vTDRAW;
	VTPATHFLAG m_vTPATH;
	VTBSP m_vDRAWBSP;

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
		CTExplorerCAM *pCamera,
		CTachyonRes *pRES,
		WORD wMapID);

	void LoadMAPOBJ(
		CD3DDevice *pDevice,
		LPVTNPC pTMAPNPC,
		LPVTMAPOBJ pTMAPOBJ,
		LPVTMAPSFX pTMAPSFX,
		LPVTMAPSND pTMAPSND,
		LPTOBJBLOCK pTOBJ,
		LPTOBJBLOCK pTNPC,
		LPTSFXBLOCK pTSFX,
		LPTSNDBLOCK pTSND);

	void ClearMAPOBJ(
		LPTOBJBLOCK pTOBJ,
		LPTOBJBLOCK pTNPC,
		LPTSFXBLOCK pTSFX,
		LPTSNDBLOCK pTSND);

	void CheckMAPOBJ(
		CD3DDevice *pDevice,
		LPD3DXVECTOR3 pPOS);

	void CalcMAPOBJ(
		CD3DDevice *pDevice,
		CTExplorerCAM *pCamera,
		DWORD dwTick);

	void LoadTFLAG( BYTE bINDEX);
	void LoadTNPC(
		CD3DDevice *pDevice,
		BYTE bINDEX);

	CTExplorerObjBase *FindTNPC( DWORD dwNpcID);
	void ClearTSILHOUETTE( BYTE bINDEX);
	void ClearBIGOBJ( BYTE bINDEX);
	void ClearTNPC( LPVTNPC pTNPC);
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
		CTExplorerObjBase *pWATER,
		FLOAT fPosX,
		FLOAT fPosZ);

	FLOAT GetWaterHeight(
		FLOAT fPosX,
		FLOAT fPosZ);

	FLOAT GetHeight(
		LPVTOBJBASE pTMAPOBJ,
		CTExplorerObjBase *pOBJ,
		CTExplorerObjBase **pFLOOR,
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
	BYTE IsDrawOBJ(
		CTExplorerObjBase *pTOBJ,
		CTExplorerCAM *pCamera);

	BYTE PtInSHADOW(
		FLOAT fPosX,
		FLOAT fPosZ);

	BYTE CanMove(
		VTOBJBASE& vOBJ,
		CTExplorerObjBase *pOBJ,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pTARGET);

	BYTE CheckCollision(
		CTExplorerObjBase *pOBJ,
		LPD3DXVECTOR3 pPREV);

	void ResetVisibleOBJ(
		CD3DDevice *pDevice,
		CTExplorerObjBase *pTOBJ,
		CTExplorerCAM *pCamera,
		DWORD dwTick);

	void ResetVisibleOBJ(
		CD3DDevice *pDevice,
		CTExplorerCAM *pCamera,
		DWORD dwTick);

	void ResetTMAP(
		CD3DDevice *pDevice,
		CTExplorerCAM *pCamera,
		DWORD dwTick);

	void ResetTMINIMAP(
		LPDIRECT3DTEXTURE9 pTARGET,
		CTExplorerChar *pHost,
		CD3DDevice *pDevice,
		CTExplorerCAM *pCamera);

	BYTE ResetTMINIMAP(
		CD3DDevice *pDevice,
		WORD wMapID,
		FLOAT fCenterX,
		FLOAT fCenterZ,
		FLOAT fSCALE);

	void ResetPVS(
		CTExplorerBSP *pTBSP,
		CTExplorerCAM *pCamera);

	void RenderWATER(
		CD3DDevice *pDevice,
		CTExplorerCAM *pCamera);

	void RenderBSP( CTExplorerCAM *pCamera);
	void Render( CD3DDevice *pDevice);

public:
	CTExplorerMAP();
	virtual ~CTExplorerMAP();
};
