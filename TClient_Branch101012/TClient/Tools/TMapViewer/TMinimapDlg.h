#pragma once


class CTMinimapDlg : public TFrame
{
public:
	static LPDIRECT3DTEXTURE9 m_pTMINIMAP;
	static LPDIRECT3DTEXTURE9 m_pTMASK;

public:
	TComponent *m_vTCOMP[TMINIMAPCOMP_COUNT];
	CD3DDevice *m_pDevice;

	CTMapViewerChar *m_pHost;
	CTMapViewerMAP *m_pTMAP;
	CTMapViewerCAM *m_pCAM;
	CRect m_rcAREA;

public:
	void RenderTMINIMAP();

public:
	virtual HRESULT Render( DWORD dwTickCount);

public:
	CTMinimapDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMinimapDlg();
};
