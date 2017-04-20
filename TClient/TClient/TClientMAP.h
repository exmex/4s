#pragma once

class CTClientMAP
{
public:
	static LPTEXTURESET m_pTPURSUITDIR;
	static LPTEXTURESET m_pTDEADDIR;
	static LPTEXTURESET m_pTDEAD;
	static LPTEXTURESET m_pTPOS;
	static LPTEXTURESET m_pTPARTY;
	static LPTEXTURESET m_pTPARTYDIR;

	static LPTEXTURESET m_pTQUESTRECV_O;
	static LPTEXTURESET m_pTQUESTCOMP_O;
	static LPTEXTURESET m_pTQUESTRECV_X;
	static LPTEXTURESET m_pTQUESTCOMP_X;

	static LPTEXTURESET m_pTCOMMANDER;
	static LPTEXTURESET m_pTSQUAD;
	static LPTEXTURESET m_pTUNIT;
	static LPTEXTURESET m_pTMON;
	static LPTEXTURESET m_pTPC;

	static LPTEXTURESET m_pTCMDATTACK;
	static LPTEXTURESET m_pTCMDMOVE;
	static LPTEXTURESET m_pTCMDPROTECT;
	static LPTEXTURESET m_pTCMDALARM;

	static LPTEXTURESET m_pTSiegeGodBall;
	static LPTEXTURESET m_pTGodTower[3];

	static FLOAT m_fMoveBound;
	static FLOAT m_fGravity;

public:
	MAPMAPOBJ m_mapTMAPOBJ[9][TOBJCELL_COUNT];
	VTMAPSFX m_vTMAPSFX[9][TOBJCELL_COUNT];
	VTMAPSND m_vTMAPSND[9][TOBJCELL_COUNT];
	VTNPC m_vTMAPNPC[9][TOBJCELL_COUNT];

	MAPTHEIGHTINFO m_mapTHEIGHTINFO[9];
	CTClientPath m_vTNODE[9];

	TOBJBLOCK m_vTSILHOUETTE[9];
	TOBJBLOCK m_vBIGOBJ[9];
	VTBSP m_vTBSP[9];

	TOBJBLOCK m_vTOBJ[TDRAWCELL_COUNT];
	TOBJBLOCK m_vTNPC[TDRAWCELL_COUNT];
	TSFXBLOCK m_vTSFX[TDRAWCELL_COUNT];
	TSNDBLOCK m_vTSND[TDRAWCELL_COUNT];

	WORD m_vTCELL[TDRAWCELL_COUNT];
	BYTE m_vLOCK[TDRAWCELL_COUNT];
	BYTE m_vUNIT[9];

public:
	CTachyonHUGEMAP *m_pMAP;
	BYTE m_bSHADOW;
	BYTE m_bHouseMesh;

	MAPNPC m_mapTLOADEDNPC;

	MAPOBJECT m_mapTPortal;
	MAPOBJECT m_mapTNpc;
	MAPTGATE m_mapTGate;
	MAPTBSP m_mapTBsp;
	FLOAT m_fTSCALE;

	DWORD m_dwTMapCOLOR;
	DWORD m_dwHouseID;
	DWORD m_dwBspID;
	WORD m_wMapID;

	VECTORDWORD m_vNEWQUESTOBJ;
	VTOBJBASE m_vDRAWOBJ[9];

	MAPOBJECT m_mapTCOLLOBJ;
	MAPOBJECT m_mapTWATER;

	VTOBJBASE m_vTDRAWGATE;
	VTOBJBASE m_vTCOLLNPC;

	LISTTOBJBASE m_vTDRAW;
	VTPATHFLAG m_vTPATH;
	VTBSP m_vDRAWBSP;

protected:
	LPTMAPJOINT m_pTLOADJOINT;

	int m_nUnitCellX;
	int m_nUnitCellZ;

	int m_nTCellX;
	int m_nTCellZ;

///////////////////////////////////////////////////////////////////
// Load thread 내부에서 호출되는 함수들 이므로 이 함수들
// 안에서는 넘겨받은 파라메터와 로컬변수만 사용해야 함
public:
	void DoTLOADCMD_OBJ(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		CTClientApp *pTAPP,
		LPTLOADOBJ pTINFO);

	void LoadTHEIGHTINFO(
		LPMAPTHEIGHTINFO pTHEIGHTINFO,
		WORD wUnitID);

	void LoadTPATH(
		CTClientPath *pTPATH,
		WORD wUnitID);

	void LoadTFLAG(
		LPVTPATHFLAG pTFLAG,
		WORD wUnitID);

	void LoadTNPC(
		LPVTNPC pTNPC,
		WORD wUnitID);
///////////////////////////////////////////////////////////////////

protected:
	FLOAT GetAttrFLOAT(
		LPMAPOBJ pTOBJ,
		DWORD dwID);

	int GetAttrINT(
		LPMAPOBJ pTOBJ,
		DWORD dwID);

	BYTE IsValidUNIT(
		BYTE bCODE,
		int nIndex);

public:
	int GetTDRAWCELL(
		FLOAT fPosX,
		FLOAT fPosZ);

	BYTE IsInValidUNIT( CTClientObjBase *pTOBJ);
	BYTE IsValidLoadUNIT(
		LPTMAPJOINT pTJOINT,
		int nIndex);

	LPTMAPJOINT GetTLoadJOINT(
		int nPosX,
		int nPosZ);

	LPTMAPJOINT GetTMoveJOINT(
		int nPosX,
		int nPosZ);

	void GetLineCollisionOBJ(
		LPVTOBJBASE pTCOLLISION,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fWIDTH);

	void GetCollisionOBJ(
		LPVTOBJBASE pTCOLLISION,
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fRadius);

	void GetCollisionOBJ(
		LPVTOBJBASE pTCOLLISION,
		LPMAPOBJECT pTINST,
		LPMAPDWORD pTINDEX,
		BYTE bINDEX,
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fRadius,
		int nGRID);

	void GetCollisionOBJ(
		LPVTOBJBASE pTCOLLISION,
		LPMAPOBJECT pTINST,
		LPMAPDWORD pTINDEX,
		BYTE bINDEX,
		DWORD dwINDEX);

	void GetCollisionGATE(
		LPVTOBJBASE pTCOLLISION,
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fRadius);

	void GetCollisionNPC(
		LPVTOBJBASE pTCOLLISION,
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fRadius);

	BOOL CheckCollisionOBJ(
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fRadius);

	BOOL CheckCollisionOBJ(
		BYTE bINDEX,
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fRadius,
		int nGRID);

	BOOL CheckCollisionOBJ(
		BYTE bINDEX,
		DWORD dwINDEX);

	BOOL CheckCollisionGATE(
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fRadius);

	BOOL CheckCollisionNPC( 
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fRadius);

	void GetWaterOBJ(
		LPVTOBJBASE pTWATER,
		FLOAT fPosX,
		FLOAT fPosZ);

	void LoadMAPOBJ(
		CD3DDevice *pDevice,
		BYTE bLoadNOW);
	void ReleaseMAP();

	void LoadMAP(
		CD3DDevice *pDevice,
		CTClientCAM *pCamera,
		CTachyonRes *pRES,
		WORD wMapID);

	void LoadMAPOBJ(
		CD3DDevice *pDevice,
		LPVTNPC pTMAPNPC,
		LPMAPMAPOBJ pTMAPOBJ,
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
		LPD3DXVECTOR3 pPOS,
		BYTE bUPDATE);
	void CheckUNIT( BYTE bUPDATE);

	void CalcMAPOBJ(
		CD3DDevice *pDevice,
		CTClientCAM *pCamera,
		DWORD dwTick);

	void FixTOBJHeight( CTClientObjBase *pTOBJ);
	void LandTOBJ( CTClientObjBase *pTOBJ);

	CTClientObjBase *GetTNPCINST( DWORD dwNpcID);
	LPTNPC GetTNPC( WORD wNpcID);
	CTClientObjBase *FindTNPC( DWORD dwNpcID);

	void ClearTHEIGHTINFO( BYTE bINDEX);
	void ClearTSILHOUETTE( BYTE bINDEX);
	void ClearBIGOBJ( BYTE bINDEX);
	void ClearTNPC( LPVTNPC pTNPC);
	void ClearTBSP( BYTE bINDEX);
	void ClearTFLAG( WORD wUnitID);

	FLOAT GetIntensity();
	FLOAT GetSpecRange();
	FLOAT GetSpecMax();

	FLOAT GetLandHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	FLOAT GetWaterHeight(
		CTClientObjBase *pWATER,
		FLOAT fPosX,
		FLOAT fPosZ);

	FLOAT GetWaterHeight(
		FLOAT fPosX,
		FLOAT fPosZ);

	FLOAT GetHeight(
		CTClientObjBase *pOBJ,
		CTClientObjBase **pFLOOR,
		LPD3DXVECTOR3 pPOINT,
		FLOAT fMove,
		BYTE bCheckBound);

	DWORD GetTStepSND(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	D3DXVECTOR3 GetTargetPoint(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pTARGET);

	CTClientFlag *FindPath(
		LPMAPTPATHFLAG pTPATH,
		CTClientFlag *pTFLAG,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pTARGET);

	LPTREGIONINFO GetRegionINFO(
		FLOAT fPosX,
		FLOAT fPosZ);

	BYTE HitTest(
		FLOAT *pDIST,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV,
		BYTE bOnlyForCam);

	BYTE HitTestHeight(
		FLOAT *pDIST,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	BYTE GetRegionID(
		FLOAT fPosX,
		FLOAT fPosZ);

	BYTE IsEXTVisible( CTClientCAM *pCamera);
	BYTE IsDrawOBJ(
		CTClientObjBase *pTOBJ,
		CTClientCAM *pCamera);
	BYTE HasEXTCell( LPD3DXVECTOR3 pPOS);

	BYTE PtInSHADOW(
		FLOAT fPosX,
		FLOAT fPosZ);

	BYTE CanMove(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pTARGET);

	BYTE CheckCollision(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pRESULT,
		LPD3DXVECTOR3 pPREV,
		FLOAT fNextH);

	BYTE CheckCollision(
		CTClientObjBase **pTCOLL,
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		FLOAT fSizeY);

	BYTE CheckCollision(
		CTClientObjBase *pOBJ,
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		FLOAT fSizeY,
		FLOAT *pDIST);

	BYTE CheckJoint(
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		FLOAT *pDIST);

	BYTE CheckRECT(
		LPD3DXVECTOR3 pTSTART,
		LPD3DXVECTOR3 pTEND,
		CRect vTRECT,
		FLOAT *pDIST);

	BYTE CheckLINE(
		LPD3DXVECTOR3 pTLINE,
		int nPosX,
		int nPosZ);

	void ResetVisibleOBJ(
		CD3DDevice *pDevice,
		CTClientObjBase *pTOBJ,
		CTClientCAM *pCamera,
		DWORD dwTick);

	void ResetVisibleOBJ(
		CD3DDevice *pDevice,
		CTClientCAM *pCamera,
		DWORD dwTick);

	void ResetTMAP(
		CD3DDevice *pDevice,
		CTClientCAM *pCamera,
		DWORD dwTick,
		BYTE bLoadNOW);

	BYTE ResetVB(
		CD3DDevice *pDevice,
		CTClientCAM *pCamera,
		BYTE bCullView,
		BYTE bTargetLOD);

	BYTE CheckVB(
		LPDIRECT3DDEVICE9 pDevice,
		LPIPOINT pPIVOT);

	void ResetTMINIMAPOBJ(
		CD3DDevice *pDevice,
		WORD wMapID,
		LPDIRECT3DTEXTURE9 pTDIRTEX,
		LPDIRECT3DTEXTURE9 pTPOSTEX,
		LPD3DXVECTOR2 pTCENTER,
		LPD3DXVECTOR2 pTPOS,
		FLOAT fSCALE);

	void ResetTMINIMAPSIEGE( CD3DDevice *pDevice,
		WORD wMapID,
		LPDIRECT3DTEXTURE9 pTTEX,
		LPD3DXVECTOR2 pTCENTER,
		LPD3DXVECTOR2 pTPOS,
		FLOAT fSCALE);

	void ResetTMINIMAP(
		LPDIRECT3DTEXTURE9 pTARGET,
		LPMAPMONSTER pTMON,
		CTRSCSDlg *m_pTRSCS,
		CTClientChar *pHost,
		CD3DDevice *pDevice,
		CTClientCAM *pCamera);

	BYTE ResetTMINIMAP(
		CD3DDevice *pDevice,
		WORD wMapID,
		FLOAT fCenterX,
		FLOAT fCenterZ,
		FLOAT fSCALE);

	void ResetTRSCSALARM( CTRSCSDlg *pTRSCS);
	void ResetTRSCSOBJ(
		CTRSCSDlg *pTRSCS,
		LPD3DXVECTOR2 pTPOS,
		BYTE bROT = FALSE);

	void ResetTRSCSMASK(
		CTRSCSDlg *pTRSCS,
		LPTUNIT pTUNIT);

	void ResetTRSCSUNIT(
		CTRSCSDlg *pTRSCS,
		LPTSQUAD pTSQUAD,
		LPTUNIT pTUNIT);

	void ResetTRSCS(
		LPDIRECT3DTEXTURE9 pTBACKTEX,
		LPDIRECT3DTEXTURE9 pTRSCSTEX,
		LPDIRECT3DTEXTURE9 pTMASKTEX,
		CTRSCSDlg *pTRSCS);

	void ResetPVS(
		CTClientBSP *pTBSP,
		CTClientCAM *pCamera);

	void RenderWATER(
		CD3DDevice *pDevice,
		CTClientCAM *pCamera);
	void Render( CD3DDevice *pDevice);

public:
	CTClientMAP();
	virtual ~CTClientMAP();
};
