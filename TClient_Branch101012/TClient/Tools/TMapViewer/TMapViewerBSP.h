#pragma once


class CTMapViewerBSP
{
public:
	MAPVECTORTEXWINDING m_mapLIGHT;
	MAPVERTEXWINDING m_mapMESH;
	MAPTBSPNODE m_mapPVS;

	LISTTOBJBASE m_vTDRAW;

public:
	CTachyonBSPMAP *m_pMAP;
	LPTMAPINFO m_pINFO;

	DWORD m_dwNodeID;
	DWORD m_dwBspID;

	D3DXMATRIX m_vPOS;
	D3DXMATRIX m_vINV;

public:
	DWORD GetNodeID( D3DXVECTOR3 vPOS);

	void InitTBSP( LPMAPOBJ pTOBJ);
	void Render( DWORD dwNodeID);
	void Release();

public:
	CTMapViewerBSP();
	virtual ~CTMapViewerBSP();
};
