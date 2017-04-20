#pragma once


class CTachyonBSPMAP : public CTachyonMAP
{
// For editing
public:
	void DeleteLight( DWORD dwID);
	void AddLight(
		LPTEXTURESET pTEX,
		LPMAPLIGHT pLIGHT,
		DWORD dwID);

	void BuildPVS(
		LPMAPTBSPNODE pPVS,
		LPMAPTEXVB pLIGHT,
		LPMAPMESHVB pVB,
		DWORD dwNodeID);

	void Render(
		LPMAPTBSPNODE pPVS,
		LPMAPTEXVB pLIGHT,
		LPMAPMESHVB pVB);

	void LoadFromTMF( LPARAM lParam, LPBYTE pData);
	void Overlap( LPBYTE pData);

	void Load( CFile* pFile);
	void Save( CFile* pFile);

	void DisableLight();
	void EnableLight();
	void ResetLight();
	void Clear();

	int HitTest(
		LPD3DXVECTOR3 pLayPos,
		LPD3DXVECTOR3 pLayDir,
		FLOAT *pDistance);

public:
	virtual FLOAT GetHeight(
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

protected:
	void BuildHeadPortal( LPVECTORTBSPPORTAL pResult);
	void ApplyTexture( LPTEXTURESET pTEX);

	void AddAllNode(
		LPMAPVECTORTEXWINDING pLIGHT,
		LPMAPVERTEXWINDING pWINDING,
		LPMAPTBSPNODE pPVS,
		CTBSPNode *pNode);

	void AddPVS(
		LPMAPVECTORTEXWINDING pLIGHT,
		LPMAPVERTEXWINDING pWINDING,
		LPMAPTBSPNODE pPVS,
		CTBSPNode *pNode);

// For game data
public:
	void LoadMAP( CFile* pFile);
	void SaveMAP( CFile* pFile);

	void BuildPVS(
		LPMAPVECTORTEXWINDING pLIGHT,
		LPMAPVERTEXWINDING pWINDING,
		LPMAPTBSPNODE pPVS,
		DWORD dwNodeID);

	void Render(
		LPMAPVECTORTEXWINDING pLIGHT,
		LPMAPVERTEXWINDING pWINDING,
		LPMAPTBSPNODE pPVS);

public:
	LPDIRECT3DVERTEXBUFFER9 m_pLIGHT;
	LPDIRECT3DVERTEXBUFFER9 m_pMESH;

	VECTORTEXSET m_vTexture;
	MAPD3DLIGHT m_mapLIGHT;
	MAPDWORD m_mapTREF;
	CTBSPNode m_Head;

	DWORD m_dwMeshCount;
	int m_nTexIDX;

	LPBYTE m_pRegionID;
	DWORD m_dwAmbient;
	DWORD m_dwTick;

	BYTE m_bLIGHTMAP;
	BYTE m_bPVS;

public:
	CTachyonBSPMAP();
	virtual ~CTachyonBSPMAP();
};
