#pragma once
#include "TMonSpawnInspectorRes.h"

class CTMonSpawnInspectorHUGEMAP;

class CTMonSpawnInspectorMAP
{
public:
	static MAPTMAPINFO m_mapTMAPINFO;
	static FLOAT m_fMoveBound;

	CTMonSpawnInspectorMAP(void);
	virtual ~CTMonSpawnInspectorMAP(void);

	BOOL InitResource(
		CString strGroupID);

	VOID TermResource();

	VOID ReleaseTMAPINFO();

	VOID ClearCOLL();

	LPTMAPINFO GetMapINFO(
		DWORD dwMapID);

	FLOAT GetAttrFLOAT(
		LPMAPOBJ pTOBJ,
		DWORD dwID);

	int GetAttrINT(
		LPMAPOBJ pTOBJ,
		DWORD dwID);

	void LoadMAPOBJ(
		CTachyonHUGEMAP* pMAP );

	BOOL IsValidPoint(
		D3DXVECTOR3 vPos,
		FLOAT fRange);

	BOOL FindValidPoint(
		D3DXVECTOR3 vPos,
		FLOAT fRange,
		CTMonSpawnInspectorHUGEMAP* pMAP,
		D3DXVECTOR3* vValidPos );

	BOOL IsValidHeight(
		CTMonSpawnInspectorHUGEMAP* pMAP,
		D3DXVECTOR3 vPos,
		FLOAT fRange,
		FLOAT* pResultHeight,
		FLOAT* fLandHeight);

	FLOAT GetHeight(
		CTMonSpawnInspectorHUGEMAP* pMAP,
		LPVTOBJBASE pTMAPOBJ,
		CTMonSpawnInspectorObjBase *pOBJ,
		CTMonSpawnInspectorObjBase **pFLOOR,
		LPD3DXVECTOR3 pPOINT,
		FLOAT fMove,
		BYTE bCheckBound,
		FLOAT* fLandHeight);

public:
	CTMonSpawnInspectorRes		m_Res;
	//TOBJBLOCK							m_vBIGOBJ[9];
	VTMAPOBJ								m_vTMAPOBJ[9][TOBJCELL_COUNT];

	std::vector< CTMonSpawnInspectorObjBase* >		m_vCOLL;

private:
	void	InitTMAPINFO();	
};