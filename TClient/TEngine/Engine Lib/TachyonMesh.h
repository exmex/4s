// TachyonMesh.h: interface for the CTachyonMeshFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined __TACHYONMESH_H
#define __TACHYONMESH_H


class CTachyonMesh : public CObject
{
public:
	static MAPINDEXEDMESH m_mapVB;
	static VECTORFILEVB m_vFILEVB;

	static DWORD m_dwCurrentVB;
	static DWORD m_dwMaxVB;
	static FLOAT m_fLevelFactor;
	static BYTE m_bSoftwareVP;
	static BYTE m_gZEnable;
	static BYTE m_gZWriteable;

public:
	static void BeginGlobalDraw( LPDIRECT3DDEVICE9 pDevice);
	static void EndGlobalDraw( LPDIRECT3DDEVICE9 pDevice);
	static void SetGlobalZState( BOOL bZEnable, BOOL bZWriteable );
	static void ReleaseGlobalVB();
	static void InitGlobalVB();

	static LPINDEXEDMESH GetGlobalVB(
		BYTE bGroupID,
		BYTE bFileID,
		BYTE bMESHType,
		BYTE bVBID);

public:
	LPINDEXEDMESH GetGlobalVB();

	LPIBINFO BuildMeshINFO( LPVECTORDWORD pMESH);
	LPIBINFO BuildMeshINFO(
		LPWORD pMESH,
		DWORD dwCount);

	void LoadToGlobalVB( LPDIRECT3DDEVICE9 pDevice);
	void Restore( LPDIRECT3DDEVICE9 pDevice);

	void Lock(
		LPDIRECT3DDEVICE9 pDevice,
		LPVECTORSTRING pFILE);

	void ReleaseDATA( BYTE bReleaseVB);
	void Release();
	void Unlock();

	void Render(
		LPDIRECT3DDEVICE9 pDevice,
		int nLevel = -1);

	void Render(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwIndex,
		int nLevel);

	BOOL LoadMesh(
		LPDIRECT3DDEVICE9 pDevice,
		LPBYTE pData);

	int GetLevel( FLOAT fDist);
	int HitTest(
		CD3DCamera *pCam,
		int nPosX,
		int nPosY,
		int nLevel);

public:
	LPDIRECT3DVERTEXBUFFER9 m_pVB;
	LPVECTORIB m_pIB;

	LPVECTORIBINDEXINFO m_pMESHIB;
	LPVECTORIBINFO m_pMESH;
	VECTORFLOAT m_vDist;

	LPD3DXMATRIX m_pBones;
	LPVBDATA m_pDATA;

public:
	D3DXVECTOR3 m_vCenterPoint;
	FLOAT m_fRadius;
	BYTE m_bUseVB;

	DWORD m_dwNodeCount;
	DWORD m_dwMeshCount;

	// Global VB Index factor
	DWORD m_dwVBIndex;

	BYTE m_bMESHType;
	BYTE m_bVBType;

	BYTE m_bGroupID;
	BYTE m_bVBID;
	BYTE m_bFileID;

	DWORD m_dwInitVB;
	DWORD m_dwPOS;
	DWORD m_dwREF;

public:
	CTachyonMesh();
	virtual ~CTachyonMesh();
};


#endif // !defined __TACHYONMESH_H
