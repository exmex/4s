#pragma once
#include "TExplorerGame.h"


class CTExplorerWnd : public CTachyonWnd
{
public:
	CTExplorerWnd();
	virtual ~CTExplorerWnd();

public:
	TLIGHT m_vTLIGHT[TLIGHT_COUNT];		// Light setting pool

// Attributes
public:
	BYTE m_bRaceID;
	CTExplorerGame m_MainGame;
	CTExplorerChar m_MainChar;
	CTExplorerCAM m_Camera;

	CD3DFont m_FontLIGHT;
	CD3DFont m_FontPOS;

	BYTE m_bResetTick;

public:
	DWORD GetTExplorerTickCount();

// Operations
public:
	virtual void OnCommand( TCOMMAND cmd);

	virtual void ProcessHotCommand( int nID);
	virtual void Render();

	virtual void OnChar(
		UINT nChar,
		UINT nRep,
		UINT nFlags);

	virtual void InitResource(
		CString strGroupID,
		BYTE bGroupID);

	void LoadImageFile( const CString& strFILE, CT3DImage* pT3DIMG);

// Implementation
public:
	void InitTRESIMG();
	void InitTRECTVB();
	void InitTMINIMAP( CString strPath );

	void ReleaseTMINIMAP();
	void ReleaseTRECTVB();
	void ReleaseTRESIMG();

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
