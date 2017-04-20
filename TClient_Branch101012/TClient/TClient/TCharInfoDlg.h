#pragma once


class CTCharInfoDlg : public CTInvenDlg
{
public:
	void EnableTLIGHT(
		CD3DCamera *pCamera,
		BYTE bENABLE);

	void ResetData(
		CTClientChar *pCHAR,
		CTachyonRes *pRES,
		BYTE bTabIndex);
	
	void ResetStatColor(
		CTClientChar *pCHAR);

	void ResetCHAR();

public:
	TComponent *m_vTCHARSTAT[TCHARSTAT_COUNT];
	TComponent *m_vTCHARSTATTEXT[TCHARSTAT_COUNT];
	
	TComponent *m_vTCHARBASIC[TCHARBASIC_COUNT];
	TComponent *m_vTCHARBASICTEXT[TCHARBASIC_COUNT];
	
	TComponent *m_vTCHARADV[TCHARADV_COUNT];
	TComponent *m_vTCHARPANE[TCHARPANE_COUNT];
	
	TFrame *m_vTCHARFRAME[TCHARFRAME_COUNT];
	CPoint m_vTPOS[TCHARFRAME_COUNT];

	TButton *m_pTHELMETHIDE;

	TComponent *m_pTDEFINVEN;										// Default inven ctrl
	TImageList *m_pTSUBINVEN[MAX_SUBINVEN];					// Sub inven ctrl

public:
	CD3DLight m_vLIGHT[TLIGHT_COUNT];

	CD3DDevice *m_pDevice;
	CD3DCamera m_vCamera;

	static BYTE m_bTabIndex;
	static CPoint m_ptPOS;

	CTClientChar m_vCHAR;

public:
	virtual HRESULT Render( DWORD dwTickCount);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual BOOL CanWithItemUI();

	virtual void OnLButtonDown( UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual TDROPINFO OnDrop( CPoint point);
	virtual BYTE OnBeginDrag(
		LPTDRAG pDRAG,
		CPoint point);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual void ResetPosition();

	ITDetailInfoPtr GetTInfoKey_Inven(const CPoint& point, INT nID);

	void ResetSubInven( BYTE bInvenID, WORD wImageID, BYTE bEnable);
	void ResetVisible();
	void SetPvPPoint( DWORD dwPvPTotalPoint, DWORD dwPvPUseablePoint );

	CTClientChar	*m_pHost;

public:
	CTCharInfoDlg( TComponent *pParent, LP_FRAMEDESC pDesc, TCMLParser *pParser, CD3DDevice *pDevice, WORD wFrameID, CTClientChar* pHost);
	virtual ~CTCharInfoDlg();
};
