#pragma once


class CTMapViewerObjBase : public CTachyonObject
{
public:
	static DWORD m_vTRACE[TMAINRACE_COUNT][TSEX_COUNT];

	static TCLOTH m_vTFACE[TFACE_COUNT][THAIR_COUNT];
	static TCLOTH m_vTPANTS[TPANTS_COUNT];
	static TCLOTH m_vTBODY[TBODY_COUNT];
	static TCLOTH m_vTHAND[THAND_COUNT];
	static TCLOTH m_vTFOOT[TFOOT_COUNT];

	static FLOAT m_fCamDist;
	static CFont *m_pFONT;

	static LPDIRECT3DVERTEXBUFFER9 m_pTSQUARE;
	static LPTEXTURESET m_pTSHADOW;

	static MAPTACTION m_mapTACTIONTEMP;
	static MAPDWORD m_mapTSTEPSND;

	static VTACTIONDATA m_vTACTION;

public:
	static void BeginDrawSquare( CD3DDevice *pDevice);

	static void InitSquareResource(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);
	static void ReleaseSquareResource();

public:
	MAPSFXINST m_mapTSUBACTSFXINST;
	MAPSFXINST m_mapTSUBANISFXINST;

	MAPSNDINST m_mapTSUBACTSNDINST;
	MAPSNDINST m_mapTSUBANISNDINST;

	VECTOROBJSFX m_vTSUBACTSFX;
	VECTOROBJSFX m_vTSUBANISFX;

	MAPDWORD m_mapTSUBACTSFX;
	MAPDWORD m_mapTSUBANISFX;

	MAPOBJSND m_mapTSUBACTSND;
	MAPOBJSND m_mapTSUBANISND;
	MAPTINVEN m_mapTINVEN;
	ACTINST m_vTSUBACT;

	FLOAT m_fSubActTime;
	DWORD m_dwTSUBACT;
	int m_nSubKeyBase;

public:
	VECTOROBJSFX m_vTGARBAGESFX;
	VECTOROBJSFX m_vTACTIONSFX;

	DWORD m_dwID;
	BYTE m_bType;

	BYTE m_bCollisionType;
	BYTE m_bCamCollision;
	BYTE m_bDrawShadow;
	BYTE m_bEquipMode;
	BYTE m_bDrawName;
	BYTE m_bPrevSwim;
	BYTE m_bACTLevel;
	BYTE m_bInSHADOW;
	BYTE m_bSTEPSND;
	BYTE m_bColorID;
	BYTE m_bVisible;
	BYTE m_bAction;
	BYTE m_bFPView;
	BYTE m_bShoot;
	BYTE m_bLand;
	BYTE m_bMode;
	BYTE m_bSwim;
	BYTE m_bHide;

public:
	LPTREGIONINFO m_pTREGION;
	CD3DFont m_vNAME;

	DWORD m_dwSHADOWTick;
	DWORD m_dwAlphaTick;

	WORD m_wMoveDIR;
	WORD m_wPITCH;
	WORD m_wDIR;

	FLOAT m_fSquareHeight;
	FLOAT m_fSpeedFactor;
	FLOAT m_fCamDIST;
	FLOAT m_fRadiusH;
	FLOAT m_fRadius;
	FLOAT m_fZValue;
	FLOAT m_fRange;
	FLOAT m_fSizeX;
	FLOAT m_fSizeY;
	FLOAT m_fSizeZ;
	FLOAT m_fDepth;

protected:
	DWORD m_vWeaponPOS[TWEAPON_COUNT];
	DWORD m_dwActionID;
	DWORD m_dwAniID;

	BYTE m_bRootID;

public:
	CTMapViewerInven *FindTInven( BYTE bInvenID);
	void ClearInven();

	void RemoveTActionSFX( LPVECTOROBJSFX pTACTIONSFX);
	void ClearTActionSFX( LPVECTOROBJSFX pTACTIONSFX);
	void PlayTActionSFX(
		LPVECTOROBJSFX pTACTIONSFX,
		DWORD dwFuncID);

	void ResetRootID( DWORD dwRootID);
	void InitSIZE(
		FLOAT fSizeX,
		FLOAT fSizeY,
		FLOAT fSizeZ,
		FLOAT fDepth);

	LPTACTION FindTACTION(
		BYTE bActionID,
		BYTE bWeaponID);

	TACTION FindActionID(
		BYTE bActionID,
		BYTE bWeaponID);

	BYTE GetEquipMode(
		BYTE bActionID,
		BYTE bWeaponID);

	FLOAT GetPolyHeight(
		LPD3DXVECTOR3 pPOLY,
		LPD3DXVECTOR3 pPOS,
		FLOAT fRadius);

	BYTE CanUseSubANI();
	BYTE CheckPoly(
		LPD3DXPLANE pPLANE,
		LPD3DXVECTOR3 pPOLY,
		LPD3DXVECTOR3 pPOS,
		FLOAT fRadius,
		FLOAT fSizeY);

	BYTE GetCollisionPlane(
		LPD3DXPLANE pPLANE,
		LPD3DXVECTOR3 pPOLY);

public:
	virtual void ClearSND();
	virtual void ClearSFX();
	virtual void Release();

	virtual void CalcOBJSFX( CD3DDevice *pDevice);
	virtual void ClearOBJSFX();

	virtual void CancelTAction();
	virtual void RemoveTAction();
	virtual BYTE GetTAction();

	virtual void ReleaseData();
	virtual CString GetTitle();
	virtual CString GetName();

	virtual BYTE GetWeaponID( BYTE bMode);
	virtual BYTE CheckDIR( WORD wDIR);
	virtual BYTE IsJump();
	virtual BYTE IsFall();

	virtual void ApplyMatrix( CD3DDevice *pDevice);
	virtual void CalcSUBFrame();
	virtual void CalcSUBTick(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

	virtual WORD GetTargetDIR(
		FLOAT fTargetX,
		FLOAT fTargetZ);

	virtual void CalcDIR(
		WORD wMoveDIR,
		WORD wDIR);

	virtual void CalcTick(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

	virtual THIDEPART FindHidePart(
		CTMapViewerItem *pTITEM,
		BYTE bSlotID);
	virtual BYTE CheckOBJPart( LPTHIDEPART pTHIDE);

	virtual void ResetDEFOBJPart( LPTHIDEPART pTHIDE);
	virtual void ResetOBJPart( CD3DDevice *pDevice);

	virtual void Render(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera);

	virtual void ResetWeaponPOS(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		BYTE bEquipMode);

	virtual void ResetDEFEQUIP(
		CD3DDevice *pDevice,
		BYTE bEquipPOS);

	virtual void ResetEQUIP(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void ResetVisible(
		CTMapViewerCAM *pCamera,
		DWORD dwTick);

	virtual BYTE GetEquipModeIndex(
		BYTE bEquipPOS,
		BYTE bEquipMode);

	virtual BYTE HitTest(
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	virtual BYTE HitCylinder(
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	virtual BYTE HitBox(
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	virtual BYTE HitPoly(
		FLOAT *pDIST,
		LPD3DXVECTOR3 pSTART,
		LPD3DXVECTOR3 pEND,
		FLOAT fLineH,
		FLOAT fLineV);

	virtual void SetAction(
		DWORD dwActionID,
		DWORD dwAniID = -1);

	virtual void SetSubAction(
		DWORD dwActionID,
		DWORD dwAniID = -1);

	virtual void SetTAction(
		BYTE bAction);

	virtual FLOAT GetHeight(
		CTMapViewerObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	virtual FLOAT GetCylinderHeight(
		CTMapViewerObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	virtual FLOAT GetBoxHeight(
		CTMapViewerObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	virtual FLOAT GetPolyHeight(
		CTMapViewerObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	virtual BYTE CheckCollision(
		CTMapViewerObjBase *pOBJ);

	virtual BYTE CheckPoly(
		CTMapViewerObjBase *pOBJ);

	virtual BYTE CheckCylinder(
		CTMapViewerObjBase *pOBJ);

	virtual BYTE CheckBox(
		CTMapViewerObjBase *pOBJ);

public:
	CTMapViewerObjBase();
	virtual ~CTMapViewerObjBase();
};
