#pragma once

typedef DWORD (CALLBACK *FNPROCLEVEL)( LPARAM, int, CString);
typedef DWORD (CALLBACK *FNPROCSTEP)( LPARAM, int);


class CTBSPNode
{
protected:
	static MAPTBSPNODE m_mapTBSPNode;
	static DWORD m_dwNodeID;

	static void GetNewID( CTBSPNode *pItem);
	static void ReleaseID( DWORD dwID);

	BOOL m_bValid;

public:
	static void InitCommon( CD3DDevice *pDevice);
	static void ReleaseCommon();

	static CTBSPNode *GetNode( DWORD dwID);

	void RegulateLight( LPMAPTEXTURESET pTEX);
	void DeleteLight( DWORD dwID);
	void AddLight(
		LPTEXTURESET pTEX,
		LPMAPLIGHT pLIGHT,
		DWORD dwID);

	void RemoveInvalidPortal( LPARAM lParam, LPVECTORTBSPPORTAL pPortal);
	void RemoveInvalidNode( LPARAM lParam);

	void BuildPortal( LPARAM lParam, LPVECTORTBSPPORTAL pHead);
	void BuildNode( LPARAM lParam, LPVECTORVERTEXWINDING pData);
	void BuildPVS( LPARAM lParam);

	int HitTest(
		LPD3DXVECTOR3 pLayPos,
		LPD3DXVECTOR3 pLayDir,
		FLOAT *pDistance);

	void Overlap( LPVECTORVERTEXWINDING pData);
	void LoadMAP(
		CFile* pFile,
		LPMESHVERTEX pMESH,
		LPLVERTEX pLIGHT,
		int& nMESH,
		int& nLIGHT);
	void Load( CFile* pFile);
	void Save( CFile* pFile);

	CTBSPNode *FindLeaf( LPD3DXVECTOR3 pPoint);
	BOOL SetID( DWORD dwID);
	BOOL IsLeaf();

	void ClearLightMap();
	void ClearPortal();
	void ClearNode();
	void ClearLeaf();
	void ClearPVS();

protected:
	int FindRootWinding( LPVECTORVERTEXWINDING pData);

	void BuildWinding( LPVECTORVERTEXWINDING pData);
	void WindingTest(
		LPVECTORVERTEXWINDING pData,
		LPD3DXPLANE pPlane,
		int *pDiff,
		int *pCut);

	void CreateBigWinding(
		LPPOINTWINDING pResult,
		LPD3DXVECTOR3 pMinPoint,
		LPD3DXVECTOR3 pMaxPoint,
		LPD3DXPLANE pPlane);

	void CreateNodePortal(
		LPVECTORTBSPPORTAL pResult,
		LPVECTORTBSPPORTAL pHead);

	void InsertPortal(
		LPVECTORTBSPPORTAL pHead,
		LPVECTORTBSPPORTAL pFront,
		LPVECTORTBSPPORTAL pBack);

	void InsertNode(
		LPVECTORVERTEXWINDING pData,
		LPVECTORVERTEXWINDING pFront,
		LPVECTORVERTEXWINDING pBack,
		int nRootIndex);

	void Overlap(
		LPVECTORVERTEXWINDING pData,
		LPVECTORVERTEXWINDING pFront,
		LPVECTORVERTEXWINDING pBack);

	void BuildPVS( CTBSPPortal* pItem);
	void BuildPVS(
		LPMAPPLANE pBoundPlane,
		LPD3DXPLANE pHostPlane,
		CTBSPPortal *pHost,
		CTBSPPortal *pPass,
		BYTE bPassDir,
		BYTE bHostDir);

	D3DXPLANE GetPassPortalPlane(
		CTBSPPortal *pItem,
		BYTE bDierction);

	DWORD GetPassPortalNodeID(
		CTBSPPortal *pItem,
		BYTE bDierction);

	BYTE GetPassPortalDir(
		CTBSPPortal *pItem,
		DWORD dwNodeID);

	BOOL IsValidPortal( CTBSPPortal *pItem);
	BOOL IsValidTargetPortal(
		LPMAPPLANE pBoundPlane,
		CTBSPPortal *pItem);

	BOOL IsValidTargetPortal(
		LPD3DXPLANE pPlane,
		CTBSPPortal *pItem);

	CTBSPPortal *CullPortal(
		LPD3DXPLANE pHostPlane,
		CTBSPPortal *pHost,
		CTBSPPortal *pPass,
		CTBSPPortal *pTarget);

	D3DXPLANE GetPortalCutter(
		LPD3DXVECTOR3 pEdgeBegin,
		LPD3DXVECTOR3 pEdgeEnd,
		LPD3DXVECTOR3 pPoint,
		LPD3DXVECTOR3 pHostNormal,
		LPD3DXPLANE pHostPlane,
		CTBSPPortal *pPass);

public:
	static FNPROCLEVEL m_pProcLevel;
	static FNPROCSTEP m_pProcStep;

	static D3DXVECTOR3 m_vMaxBound;
	static D3DXVECTOR3 m_vMinBound;

	static CD3DDevice *m_pDevice;
	static BYTE m_bBaseNodeID;

	CTBSPNode *m_pFrontNode;
	CTBSPNode *m_pBackNode;

	VECTORVERTEXWINDING m_vWinding;
	VECTORTBSPPORTAL m_vPortal;
	MAPTEXWINDING m_mapLIGHT;

	D3DXPLANE m_vPlane;
	DWORD m_dwID;

public:
	CTBSPNode();
	virtual ~CTBSPNode();
};
