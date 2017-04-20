#pragma once
#include "TMapViewerGame.h"


class CTMapViewerWnd : public CTachyonWnd
{
public:
	CTMapViewerWnd();
	virtual ~CTMapViewerWnd();

public:
	TLIGHT m_vTLIGHT[TLIGHT_COUNT];		// Light setting pool

// Attributes
public:
	CTMapViewerGame m_MainGame;
	CTMapViewerChar m_MainChar;
	CTMapViewerCAM m_Camera;

	CD3DFont m_FontLIGHT;
	CD3DFont m_FontPOS;

	BYTE m_bResetTick;

	CTachyonRes		m_RESMAP;

public:
	DWORD GetTMapViewerTickCount();

// Operations
public:
	virtual void OnCommand( DWORD cmd);

	virtual void ProcessHotCommand( int nID);
	virtual void Render();

	virtual void OnChar(
		UINT nChar,
		UINT nRep,
		UINT nFlags);

	virtual void InitResource(
		CString strGroupID,
		BYTE bGroupID);

// Implementation
public:
	void InitTREGIONINFO();
	void InitTACTIONTEMP();
	void InitTACTIONDATA();
	void InitTPANTSTEMP();
	void InitTITEMTEMP();
	void InitTRACETEMP();
	void InitTFACETEMP();
	void InitTBODYTEMP();
	void InitTHANDTEMP();
	void InitTFOOTTEMP();
	void InitTSFXTEMP();
	void InitTMINIMAP();
	void InitTSTEPSND();
	void InitTMAPINFO();
	void InitTRECTVB();
	void InitTSKYBOX();
	void InitTRESIMG();
	void InitTLIGHT();
	void InitTNODE();
	void InitTFOG();
	void InitTBGM();

	void ReleaseTREGIONINFO();
	void ReleaseTACTIONTEMP();
	void ReleaseTACTIONDATA();
	void ReleaseTITEMTEMP();
	void ReleaseTSFXTEMP();
	void ReleaseTMINIMAP();
	void ReleaseTMAPINFO();
	void ReleaseTSTEPSND();
	void ReleaseTRECTVB();
	void ReleaseTSKYBOX();
	void ReleaseTRESIMG();
	void ReleaseTLIGHT();
	void ReleaseTFOG();

	void InitCamera();
	void InitLIGHT();
	void InitCHAR();
	void InitNAV();
	void InitCommonConst();

	void RestoreDevice();
	void ReFresh();

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnOBJMessage( WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
