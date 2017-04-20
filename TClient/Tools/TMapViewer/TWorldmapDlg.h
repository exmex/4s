#pragma once


class CTWorldmapDlg : public TFrame
{
public:
	TImageList *m_pTWORLDMAP;
	TImageList *m_pTITLE;

	TImageList *m_pTCHAR;

	TComponent *m_pTEXT;

public:
	D3DXVECTOR2 m_vTCENTER;
	FLOAT m_fTSCALE;
	BYTE m_bMOVE;

	CTMapViewerChar *m_pHost;
	CTMapViewerMAP *m_pTMAP;

	CD3DDevice *m_pDevice;
	CTMapViewerCAM *m_pCAM;

public:
	void RenderWORLDMAP( DWORD dwTickCount);
	void RenderOBJ( DWORD dwTickCount);

	CPoint GetPosition(
		FLOAT fPosX,
		FLOAT fPosZ,
		int nWidth,
		int nHeight);

public:
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);
	virtual void OnLButtonUp( UINT nFlags, CPoint pt);
	virtual void OnMouseMove( UINT nFlags, CPoint pt);

	virtual HRESULT Render( DWORD dwTickCount);
	virtual BOOL HitTest( CPoint pt);

public:
	CTWorldmapDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTWorldmapDlg();
};
