#pragma once

class CTMonSpawnInspectorObjBase : public CTachyonObject
{
public:
	void InitSIZE(
		FLOAT fSizeX,
		FLOAT fSizeY,
		FLOAT fSizeZ,
		FLOAT fDepth,
		FLOAT fSight);

	virtual void ReleaseData();

	void InitOBJ( LPOBJECT pOBJ);
	void InitOBJ( CD3DDevice *pDevice, LPMAPOBJ pOBJ);
	void ResetOBJPart( CD3DDevice *pDevice);

	void InitCollision();
	void PushCollisionPoly(
		LPD3DXVECTOR3 pPOLY,
		WORD wMESH,
		WORD wINDEX,
		WORD wPOLY);

	virtual BYTE CheckCollision(
		CTMonSpawnInspectorObjBase *pOBJ);

	virtual BYTE CheckPoly(
		CTMonSpawnInspectorObjBase *pOBJ);

	virtual BYTE CheckCylinder(
		CTMonSpawnInspectorObjBase *pOBJ);

	virtual BYTE CheckBox(
		CTMonSpawnInspectorObjBase *pOBJ);

	BYTE CheckPoly(
		LPD3DXPLANE pPLANE,
		LPD3DXVECTOR3 pPOLY,
		LPD3DXVECTOR3 pPOS,
		FLOAT fRadius,
		FLOAT fSizeY);

	BYTE GetCollisionPlane(
		LPD3DXPLANE pPLANE,
		LPD3DXVECTOR3 pPOLY);

	FLOAT GetHeight(
		CTMonSpawnInspectorObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	FLOAT GetCylinderHeight(
		CTMonSpawnInspectorObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	FLOAT GetBoxHeight(
		CTMonSpawnInspectorObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	FLOAT GetPolyHeight(
		CTMonSpawnInspectorObjBase *pOBJ,
		LPD3DXVECTOR3 pPOINT);

	FLOAT GetPolyHeight(
		LPD3DXVECTOR3 pPOLY,
		LPD3DXVECTOR3 pPOS,
		FLOAT fRadius);

public:
	BYTE m_bCollisionType;
	FLOAT m_fCamDIST;
	FLOAT m_fDepth;
	FLOAT m_fRadiusH;
	FLOAT m_fRadius;

	FLOAT m_fRange;

	FLOAT m_fSight;
	FLOAT m_fSizeX;
	FLOAT m_fSizeY;
	FLOAT m_fSizeZ;

	BYTE m_bCamCollision;
	BYTE m_bHouseMesh;
	BYTE m_bHouseOUT;
	BYTE m_bType;
	DWORD m_dwHouseID;
	DWORD m_dwID;
	BYTE m_bLastDraw;

	DWORD m_dwUnitID;

	VECTORTVERTEXINDEX m_vHCOLLISION;
	VECTORTVERTEXINDEX m_vVCOLLISION;

	static FLOAT m_fCamDist;

public:
	D3DXMATRIX& GetPositionMat();
	FLOAT GetPositionX();
	FLOAT GetPositionY();
	FLOAT GetPositionZ();

public:
	CTMonSpawnInspectorObjBase();
	virtual ~CTMonSpawnInspectorObjBase();
};