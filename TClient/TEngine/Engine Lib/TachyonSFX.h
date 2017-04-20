#pragma once


class CSFX
{
public:
	void CalcFrame( BOOL bUpdate);
	void Release();

	BOOL IsDead();

	D3DXMATRIX GetBillMatrix(
		CD3DCamera *pCamera,
		LPD3DXVECTOR3 pTarget);

	void ApplyTexture( CD3DDevice *pDevice);
	BOOL InitSFX(
		CD3DDevice *pDevice,
		LPSFX pSFX);

	void RenderSINGLE(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera,
		BYTE bMaxAlpha );

public:
	virtual void CalcTick(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

	virtual void Render(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera,
		BYTE bMaxAlpha );

public:
	D3DXMATRIX m_vGlobal;
	LPSFX m_pSFX;

	D3DXVECTOR3 m_vERRScale;
	D3DXVECTOR3 m_vERRPos;
	D3DXVECTOR3 m_vERRRot;

	D3DXVECTOR3 m_vPFactor;
	D3DXVECTOR3 m_vLocalP;
	D3DXVECTOR3 m_vCurP;
	D3DXVECTOR3 m_vCurR;
	D3DXVECTOR3 m_vCurS;

	D3DXVECTOR3 m_vSCALE;
	D3DXVECTOR3 m_vROT;
	D3DXVECTOR3 m_vPOS;

	DWORD m_dwERRTime;
	DWORD m_dwTexTick;
	DWORD m_dwLife;
	FLOAT m_fTime;

	union
	{
		CTachyonObject *m_pOBJ;
		CTachyonSFX *m_pSFX;
		//LPMESHVB m_pVB;
	} m_SRC;

	FLOAT m_SRC_VB_X;
	FLOAT m_SRC_VB_Y;

public:
	static LPMESHVB m_pTEXVB;

	static void ReleaseTEXVB();
	static void InitTEXVB();

public:
	CSFX();
	virtual ~CSFX();
};


class CTachyonSFX
{
public :
	enum LODLEVEL
	{
		LODLEVEL_1 = 1,
		LODLEVEL_2,
		LODLEVEL_3,
		LODLEVEL_4,
		LODLEVEL_5,
		LODLEVEL_6,
	};

public:
	void Generate( CD3DDevice *pDevice, CD3DCamera *pCamera);
	void CalcFrame( BOOL bUpdate);
	void ClearDead();
	void Release();

	BOOL IsDead();

	BOOL InitSFX(
		CD3DDevice *pDevice,
		LPSFX pSFX);

public:
	virtual BYTE IsVisible(
		CD3DCamera *pCamera,
		FLOAT fDistMAX);

	virtual void CalcTick(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

	virtual void Render(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera,
		BYTE bMaxAlpha = 0xFF );

public:
	static MAPTSFX m_mapINST;
	static DWORD m_dwNextID;
	static BYTE m_bOptionLodLevel;
	static FLOAT m_fDistMAX;
	static DWORD m_dwParticleNum;

	static CTachyonSFX *GetSFX( DWORD dwID);

	static void Release( DWORD dwID);
	static DWORD RegSFX(
		CTachyonSFX *pSFX,
		BYTE bBias);

	static void ReleaseCommon();
	static void InitCommon();
	static void ReleaseNoHost();

	static void CalcBUF(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

	static void RenderBUF(
		CD3DDevice *pDevice,
		CD3DCamera *pCamera,
		FLOAT fDistMAX);

	static void SetOptionLOD(
		BYTE bSystemOption);

	static BYTE GetSystemOptionLOD();

public:
	VECTORTSFX m_vCSFX;
	VECTORCSFX m_vPSFX;

	D3DXMATRIX m_vGlobal;
	CSFX m_SINGLE;
	LPSFX m_pSFX;

	D3DXVECTOR3 m_vGMOVE;
	D3DXVECTOR3 m_vSCALE;
	D3DXVECTOR3 m_vROT;
	D3DXVECTOR3 m_vPOS;

	D3DXVECTOR3 m_vPFactor;
	D3DXVECTOR3 m_vCurP;
	D3DXVECTOR3 m_vCurR;
	D3DXVECTOR3 m_vCurS;

	DWORD m_dwInitTick;
	DWORD m_dwGTime;
	DWORD m_dwLife;
	FLOAT m_fTime;
	CWnd* m_pHost;

	BYTE m_bHasHost;
	BYTE m_bDeleted;

	BYTE m_bMaxAlpha;

	DWORD m_dwGlobalLodLevel;
	BYTE m_bShow;

public:
	CTachyonSFX();
	virtual ~CTachyonSFX();
};
