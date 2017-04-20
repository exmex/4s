#pragma once

struct tagOBJINST
{
	LPOBJECT m_pOBJ;

	MAPATTRIBUTE m_mapATTR;
	MAPOBJPART m_mapDRAW;
	MAPCLKINST m_mapCLK;
	MAPACTINST m_mapACT;

	DWORD m_dwCurCLK;
	DWORD m_dwCurACT;

	tagOBJINST()
	{
		m_mapATTR.clear();
		m_mapDRAW.clear();
		m_mapCLK.clear();
		m_mapACT.clear();

		m_dwCurCLK = 0;
		m_dwCurACT = 0;
		m_pOBJ = NULL;
	};

	~tagOBJINST()
	{
		Release();
	};

	inline void Release();
};

class CTachyonObject : public CObject
{
public:
	CString GetAttrSTR( DWORD dwAttrID);
	FLOAT GetAttrFLOAT( DWORD dwAttrID);
	int GetAttrINT( DWORD dwAttrID);

	void SetATTR(
		DWORD dwAttrID,
		CString strValue);

	void SetATTR(
		DWORD dwAttrID,
		FLOAT fValue);

	void SetATTR(
		DWORD dwAttrID,
		int nValue);

	void SetPIVOT(
		DWORD dwPivotID,
		FLOAT fRotX,
		FLOAT fRotY,
		FLOAT fRotZ,
		FLOAT fScaleX,
		FLOAT fScaleY,
		FLOAT fScaleZ);

	void CalcFrame( BOOL bUpdate);
	void InitOBJ(
		CD3DDevice *pDevice,
		LPMAPOBJ pOBJ);

	void ResetOBJPart( CD3DDevice *pDevice);
	void ResetOBJ( CD3DDevice *pDevice);
	void ClearEquip();

	void ResetSND(
		LPMAPOBJSND pSND,
		BYTE bCheckStop);

	void ClearSND( LPMAPOBJSND pSND);
	void ClearSFX(
		LPVECTOROBJSFX pSFX,
		LPMAPDWORD pINDEX);

	void RemoveOBJSFX();
	void RemoveSFX(
		LPVECTOROBJSFX pSFX,
		LPMAPDWORD pINDEX);
	void RemoveSFX(
		LPVECTOROBJSFX pSFX,
		LPMAPDWORD pINDEX,
		DWORD dwFuncID,
		BOOL bForceDelete );

	void GenerateSND(
		LPMAPSNDINST pINST,
		LPMAPOBJSND pSND,
		FLOAT fActTime,
		DWORD dwFuncID);

	void GenerateSFX(
		LPMAPSFXINST pINST,
		LPVECTOROBJSFX pSFX,
		LPMAPDWORD pINDEX,
		FLOAT fActTime,
		DWORD dwFuncID);

	LPD3DXMATRIX GetMeshMatrix();
	LPANI GetCurANI();

	PSTYPE GetPSType( DWORD dwOP);
	OBJHIT HitTest(
		CD3DCamera *pCam,
		int nPosX,
		int nPosY);

	BOOL GetSize(
		D3DXVECTOR3& vMin,
		D3DXVECTOR3& vMax);

	BOOL OBJInRect(
		CD3DCamera *pCamera,
		CRect rect);

public:
	virtual void ClearMatrix();
	virtual void ClearSND();
	virtual void ClearSFX();
	virtual void Release();

	virtual void CalcOBJPIVOT( CD3DDevice *pDevice);
	virtual void CalcOBJSFX( CD3DDevice *pDevice);
	virtual void ClearACTSFX();
	virtual void ClearOBJSFX();
	virtual void ClearOBJSND();

	virtual void CalcSFX(
		CD3DDevice *pDevice,
		LPVECTOROBJSFX pSFX);

	virtual void CalcSND(
		LPMAPSNDINST pINST,
		LPMAPOBJSND pSND,
		FLOAT fActTime,
		DWORD dwFuncID);

	virtual void InitOBJ( LPOBJECT pOBJ);
	virtual void SetAction(
		DWORD dwAction,
		DWORD dwAni = -1);

	virtual void SetCloth(
		CD3DDevice *pDevice,
		DWORD dwKind,
		DWORD dwCloth,
		DWORD dwMesh);

	virtual void CalcTick(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

	virtual void Render(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera);

	virtual void RenderSILHOUETTE(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera,
		DWORD dwColor,
		BOOL bChild=FALSE );

protected:
	virtual void ApplyMatrix( CD3DDevice *pDevice);
	virtual void ApplyTexture(
		CD3DDevice *pDevice,
		LPOBJTEX pTEX,
		FLOAT *pARGCONST,
		int nSTG);

	virtual void BeginBlend(
		LPANISET pPREV,
		LPANISET pNEXT,
		FLOAT fPrevActTime,
		FLOAT fNextActTime);

public:
	static CTachyonMedia *m_pMedia;
	static FLOAT m_fMipFactor;
	static DWORD m_dwBlend;

public:
	LPD3DXQUATERNION m_pBlendKEY;
	LPTPIVOT m_pPivot;
	LPD3DXMATRIX m_pBlend;
	LPD3DXMATRIX m_pBone;

	D3DXMATRIX m_vPosition;
	D3DXMATRIX m_vWorld;
	D3DXMATRIX m_vScale;
	D3DXMATRIX m_vPreScale;

	MAPOBJECT m_mapEQUIP;
	MAPDWORD m_mapCOLOR;

	MAPSFXINST m_mapOBJSFXINST;
	MAPSFXINST m_mapACTSFXINST;
	MAPSFXINST m_mapANISFXINST;

	MAPSNDINST m_mapOBJSNDINST;
	MAPSNDINST m_mapACTSNDINST;
	MAPSNDINST m_mapANISNDINST;

	VECTOROBJSFX m_vOBJSFX;
	VECTOROBJSFX m_vACTSFX;
	VECTOROBJSFX m_vANISFX;

	MAPDWORD m_mapOBJSFX;
	MAPDWORD m_mapACTSFX;
	MAPDWORD m_mapANISFX;

	MAPOBJSND m_mapOBJSND;
	MAPOBJSND m_mapACTSND;
	MAPOBJSND m_mapANISND;

	DWORD m_dwOBJSFXFuncID;
	DWORD m_dwACTSFXFuncID;
	DWORD m_dwANISFXFuncID;

	DWORD m_dwOBJSNDFuncID;
	DWORD m_dwACTSNDFuncID;
	DWORD m_dwANISNDFuncID;

	DWORD m_dwBlendTick;
	DWORD m_dwTexTick;

	FLOAT m_fActTime;
	CWnd* m_pHost;

	BOOL m_bUseSHADER;
	BYTE m_bZEnable;
	BYTE m_bZWrite;
	BYTE m_bAlpha;
	BYTE m_bSelAlpha;
	BYTE m_bSFX;
	BYTE m_bLOD;
	int m_nKeyBase;
	BYTE m_bBlend;

public:
	OBJINST m_OBJ;
	VSTYPE m_nVS;

public:
	CTachyonObject();
	virtual ~CTachyonObject();
};
