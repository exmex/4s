#pragma once


class CTMHBuilderOBJ : public CTachyonObject
{
public:
	void InitSIZE(
		FLOAT fSizeX,
		FLOAT fSizeY,
		FLOAT fSizeZ,
		FLOAT fDepth);

public:
	BYTE RectTest(
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fGrid);

	BYTE CylinderRectTest(
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fGrid);

	BYTE BoxRectTest(
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fGrid);

	BYTE PolyRectTest(
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fGrid);

	BYTE PolyRectTest(
		LPD3DXVECTOR3 pPOLY,
		FLOAT fPosX,
		FLOAT fPosZ,
		FLOAT fGrid);

public:
	BYTE m_bCollisionType;
	BYTE m_bCamCollision;
	BYTE m_bType;

	FLOAT m_fRadiusH;
	FLOAT m_fRadius;
	FLOAT m_fSizeX;
	FLOAT m_fSizeY;
	FLOAT m_fSizeZ;
	FLOAT m_fDepth;

public:
	CTMHBuilderOBJ();
	virtual ~CTMHBuilderOBJ();
};
