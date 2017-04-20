#pragma once


class CTWorldmapDlg : public TFrame
{
public:
	TComponent *m_pTALLWORLDMAP;
	TCOMP_ARRAY m_vTWORLDBUTTON;

	TImageList *m_pTWORLDMAP;
	TImageList *m_pTITLE;

	TImageList *m_pTCHAR;
	TImageList *m_pTNPC;

	TComponent *m_pTEXT;

public:
	BOOL m_bTALLWORLDMODE;
	TComponent *m_pTLASTWBTN;

	D3DXVECTOR2 m_vTCENTER;
	FLOAT m_fTSCALE;
	BYTE m_bMOVE;

	CTExplorerChar *m_pHost;
	CTExplorerMAP *m_pTMAP;

	CD3DDevice *m_pDevice;
	CTExplorerCAM *m_pCAM;

public:
	void RenderALLWORLDMAP( DWORD dwTickCount);
	void RenderWORLDMAP( DWORD dwTickCount);
	void RenderOBJ( DWORD dwTickCount);

	CPoint GetPosition(
		FLOAT fPosX,
		FLOAT fPosZ,
		int nWidth,
		int nHeight);

	TComponent* GetHitWorldButton(const CPoint& pt);
	
	DWORD MakeUnitID(WORD wMapID, BYTE nUnitX, BYTE nUnitY);
	void GetUnitPosition(FLOAT fPosX, FLOAT fPosY, INT& outUnitX, INT& outUnitY);

	BYTE GetWorldmapLevel(FLOAT fPosX, FLOAT fPosY);

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
