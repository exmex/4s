#pragma once


class CTExplorerFlag
{
public:
	static MAPTPATHFLAG m_mapTPATHFLAG;
	static CTachyonMesh m_vTMESH;
	static WORD m_wNewID;

public:
	static void InitTPATH( CD3DDevice *m_pDevice);
	static void ReleaseTPATH();

// Tool utility
public:
	static void RegTPATH(
		CTExplorerFlag *pTPATH,
		BYTE bUnitX,
		BYTE bUnitZ);

	static BYTE IsValidPoint(
		VTOBJBASE& vOBJ,
		LPD3DXVECTOR3 pPOINT);

public:
	void Render( CD3DDevice *pDevice);

public:
	CD3DFont m_vNAME;

public:
	VECTORDWORD m_vPATH;
	D3DXVECTOR3 m_vPOS;
	DWORD m_dwID;

public:
	CTExplorerFlag();
	virtual ~CTExplorerFlag();
};
