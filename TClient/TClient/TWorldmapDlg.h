#pragma once


class CTWorldmapDlg : public CTClientUIBase
{
public:
	TComponent *m_pTMYPOS;
	TComponent *m_pTCURSORPOS;

	TComponent *m_pTALLWORLDMAP;
	TCOMP_ARRAY m_vTWORLDBUTTON;
	VTFRAMEOFFSET m_vTWORLDBUTTON_OFFSET;

	TImageList *m_pTWORLDMAP;
	TImageList *m_pTITLE;

	TImageList *m_pTCHAR;
	TImageList *m_pTNPC;

	TComponent *m_pTPURSUIT;
	TComponent *m_pTDEAD;
	TComponent *m_pTEXT;
	TComponent *m_pNPCNAME;

public:
	BOOL m_bTALLWORLDMODE;
	TComponent *m_pTLASTWBTN;

	D3DXVECTOR2 m_vTCENTER;
	FLOAT m_fTSCALE;
	BYTE m_bMOVE;

	CTClientChar *m_pHost;
	CTClientChar *m_pDead;
	LPMAPMONSTER m_pTMON;
	CTClientMAP *m_pTMAP;
	CTRSCSDlg *m_pTRSCS;

	CD3DDevice *m_pDevice;
	CTClientCAM *m_pCAM;

	BOOL m_bDrawNPC;

	INT m_nMainUnitX;
	INT m_nMainUnitZ;
	
public:
	void RenderALLWORLDMAP( DWORD dwTickCount);
	void RenderWORLDMAP( DWORD dwTickCount);
	void RenderOBJ( DWORD dwTickCount);

	CPoint GetPosition(
		FLOAT fPosX,
		FLOAT fPosZ,
		int nWidth,
		int nHeight);

	D3DXVECTOR2 GetPositionInverse(
		CPoint vRESULT,
		int nWidth,
		int nHeight);

	TComponent* GetHitWorldButton( const CPoint& pt);
	BYTE CheckRequireItem( LPTNPC pTNPC);

	DWORD MakeUnitID(
		WORD wMapID,
		BYTE nUnitX,
		BYTE nUnitY);

	BYTE GetWorldmapLevel(
		FLOAT fPosX,
		FLOAT fPosY);

	void GetUnitPosition(
		FLOAT fPosX,
		FLOAT fPosY,
		INT& outUnitX,
		INT& outUnitY);
	void DrawBack();

public:
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual void OnRButtonDown( UINT nFlags, CPoint pt);
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);
	virtual void OnRButtonUp( UINT nFlags, CPoint pt);
	virtual void OnLButtonUp( UINT nFlags, CPoint pt);
	virtual void OnMouseMove( UINT nFlags, CPoint pt);
	virtual HRESULT Render( DWORD dwTickCount);
	virtual BOOL HitTest( CPoint pt);
	virtual void DefaultPosition( CPoint* vBASIS, BOOL bRestore );
	virtual void ResetPosition();

public:
	CTWorldmapDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTWorldmapDlg();
};
