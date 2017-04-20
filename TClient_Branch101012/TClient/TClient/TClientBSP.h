#pragma once


class CTClientBSP
{
public:
	static MAPTBSPMAP m_mapTBSPMAP;

	static CTachyonBSPMAP *FindTBSPMAP( DWORD dwMapID);
	static void ReleaseTBSPMAP();
	static void InitTBSPMAP();

public:
	MAPVECTORTEXWINDING m_mapLIGHT;
	MAPVERTEXWINDING m_mapMESH;
	MAPTBSPNODE m_mapPVS;

	LISTTOBJBASE m_vTDRAW;
	VTLIGHTPOS m_vTLIGHT;

public:
	CTachyonBSPMAP *m_pMAP;
	D3DXVECTOR3 m_vNodePOS;
	LPTMAPINFO m_pINFO;

	DWORD m_dwHouseID;
	DWORD m_dwNodeID;
	DWORD m_dwBspID;

	FLOAT m_fCamDIST;
	FLOAT m_fRadius;

	D3DXMATRIX m_vPOS;
	D3DXMATRIX m_vINV;

public:
	DWORD GetNodeID( D3DXVECTOR3 vPOS);

	BYTE Lock(
		CTachyonRes *pTRES,
		DWORD dwBspID);
	void Release();

	void InitTBSP( LPTBSPINFO pTDATA);
	void ResetPVS( DWORD dwNodeID);

	void ResetTLIGHT();
	void Render();

public:
	CTClientBSP();
	virtual ~CTClientBSP();
};
