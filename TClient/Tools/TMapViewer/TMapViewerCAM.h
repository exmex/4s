#pragma once


class CTMapViewerCAM : public CD3DCamera
{
public:
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
	CTMapViewerCAM();
	virtual ~CTMapViewerCAM();
};
