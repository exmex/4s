#pragma once


class CTMinimapDlg : public TFrame
{
public:
	static LPDIRECT3DTEXTURE9 m_pTMINIMAP;
	static LPDIRECT3DTEXTURE9 m_pTMASK;

public:
	TComponent *m_vTCOMP[TMINIMAPCOMP_COUNT];
	TComponent *m_pTITLE;
	TComponent *m_pTOPFRAME;
	TComponent *m_pMINMAXBTN;
	CD3DDevice *m_pDevice;
	BYTE m_bMinimize;

	CTExplorerChar *m_pHost;
	CTExplorerMAP *m_pTMAP;
	CTExplorerCAM *m_pCAM;
	CRect m_rcAREA;

	TScroll *m_pTZOOMSCROLL;

public:
	void RenderTMINIMAP();
	void ResetVisible();

public:
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render( DWORD dwTickCount);

public:
	CTMinimapDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMinimapDlg();
};
