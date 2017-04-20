#pragma once


class CTClientCAM : public CD3DCamera
{
public:
	D3DXVECTOR3 m_vTOFFSET;
	D3DXMATRIX m_vPROJ;

	FLOAT m_fZoomSPEED;
	FLOAT m_fZoomMIN;
	FLOAT m_fZoomMAX;

	FLOAT m_fCamHEIGHT;
	FLOAT m_fCamDIST;
	FLOAT m_fPrevDIST;

	BYTE m_bCamRotSPEED;
	WORD m_wCamPITCH;

public:
	void ZoomCamera(
		DWORD dwTick,
		int nCamZoom);

public:
	CTClientCAM();
	virtual ~CTClientCAM();
};
