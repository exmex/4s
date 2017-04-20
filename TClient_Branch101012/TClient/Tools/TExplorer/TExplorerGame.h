#pragma once

#include "TExplorerCMD.h"


class CTExplorerGame : public CFrameGroup
{
	friend class CTExplorerWnd;

public:
	static LPDIRECT3DVERTEXBUFFER9 m_pTRECTVB;		// Common rect vertex buffer
	static CString m_strTCMD[TCMD_COUNT];			// Command pool

protected:
	CTWorldmapDlg *m_pWorldmap;						// Worldmap ctrl
	CTMinimapDlg *m_pTMinimap;						// Minimap ctrl

	TEdit *m_pTCHATEDIT;							// Chat edit ctrl
	TList *m_pTCHAT;								// Chat list pool
	BYTE m_bChatON;
	
	BYTE m_bShowUI;
	BYTE m_bPATHFLAG;
	BYTE m_bPATHMODE;
	BYTE m_bLIGHT;
	BYTE m_bFPS;
	BYTE m_bPOS;
	BOOL m_bFogRender;

	FLOAT m_fDeleteRange;
	DWORD m_dwTMapCOLOR;

protected:
	CD3DLight m_vLIGHT[TLIGHT_COUNT];
	CD3DDevice *m_pDevice;

	CTExplorerChar *m_pMainChar;
	CTExplorerWnd *m_pMainWnd;
	CTExplorerCAM *m_pCamera;
	CTachyonRes *m_pRES;

	CTExplorerMAP m_vMAP;

protected:
	// Sky Box
	CTExplorerObjBase m_vFARIMAGE[2];
	CTExplorerObjBase m_vSKY[2];

	DWORD m_dwTSKY;
	WORD m_wTSKY;
	BYTE m_bTSKY;

	// Object material
	D3DMATERIAL9 m_vNormal;
	D3DMATERIAL9 m_vShadow;

	// Camera setting
	BYTE m_bCamFIX;
	WORD m_wCamDIR;
	int m_nCamZOOM;

	// Additional main character moving setting (mouse input)
	D3DXPLANE m_vBOUND;
	FLOAT m_fGlowRange;

	BYTE m_bMoveType;
	BYTE m_bAutoRun;

	// Mouse button state
	DWORD m_dwLButtonTick;
	DWORD m_dwRButtonTick;

	BYTE m_bLButtonDown;
	BYTE m_bRButtonDown;

	// Object square tick
	DWORD m_dwTSHADOW;

	// BGM data
	DWORD m_dwActiveVolume;
	DWORD m_dwBGM;

	BYTE m_bStopBGM;
	BYTE m_bUseImposter;

public:
	CTExplorerKEY m_vKEY;

	BYTE m_bLandCollision;
	BYTE m_bTLIGHTMode;

protected:
	void OnERROR( DWORD dwERROR);
	void OnCommand( TCOMMAND cmd);
	void OnCommand(
		DWORD dwCMD,
		CString strParam);

	void OnActivateKEY( BYTE bSCAN);
	void OnReleaseKEY( BYTE bSCAN);
	void OnRegionChanged();
	void ReleaseGame();

	LRESULT OnOBJMsg( LPOBJMSG pMSG);

	// Utility
	void LerpTLIGHT(
		LPTLIGHT pTLIGHT,
		BYTE bSetID,
		BYTE bIndex,
		BYTE bUnitX,
		BYTE bUnitZ,
		FLOAT fTIME);

	void LerpTFOG(
		LPTFOG pTFOG,
		BYTE bUnitX,
		BYTE bUnitZ,
		FLOAT fTIME);

	void LerpTFOG(
		LPTFOG pTFOG,
		LPTFOG pTBLEND,
		FLOAT fTIME);

	void DeleteFlag();
	void AddFlag();

	// Scene calculator
	void RotCameraX( int nCamRotX);
	void RotCameraY( int nCamRotY);

	void CalcClickMove( DWORD dwTick);
	void CalcMainChar( DWORD dwTick);
	void CalcSCENE( DWORD dwTick);

	void CalcGlobalTEX( DWORD dwTick);
	void CalcGhost( DWORD dwTick);
	void CalcBGM( DWORD dwTick);

	void CalcTSKY(
		FLOAT fWaterHeight,
		DWORD dwTick);

	FLOAT CalcCamera( DWORD dwTick);
	FLOAT CheckCamPos();
	BYTE FixCamera( DWORD dwTick);

	void ResetVisibleOBJ( DWORD dwTick);
	void ResetTFOG( BYTE bWATER);
	void ResetTSKY();

	// Scene prepare function
	void PushDrawOBJ( CTExplorerObjBase *pTOBJ);
	void SetMaterial( CTExplorerObjBase *pTOBJ);

	void EnableLIGHT( CTExplorerBSP *pTBSP);
	void EnableLIGHT();
	void DisableLIGHT();

	void ResetSCENEConstant();
	void SetAmbient(
		BYTE bRValue,
		BYTE bGValue,
		BYTE bBValue);

	// Scene render function
	void RenderTSQUARE(
		CTExplorerObjBase *pTOBJ,
		LPD3DXMATRIX pUV,
		LPD3DXVECTOR2 pDIR,
		FLOAT fSCALE);

	void RenderTTEXT( LPLISTTOBJBASE pLIST);
	void RenderTTEXT(
		LPD3DXMATRIX pWORLD,
		CTExplorerObjBase *pTOBJ);

	void RenderTOBJ( CTExplorerObjBase *pTOBJ);
	void RenderTOBJ( LPLISTTOBJBASE pLIST);

	void RenderTSILHOUETTE();
	void RenderTPATH();

protected:
	void EnableChat( BYTE bEnable);
	void DoCHAT();
	void ParseCMD( CString strCMD);

	CString ChatMSG(
		CString strTYPE,
		CString strNAME,
		CString strMSG,
		DWORD dwCOLOR);

	void ErrorMSG(
		DWORD dwTypeID,
		DWORD dwTitleID,
		DWORD dwMsgID,
		DWORD dwCOLOR);

// Menu Handler
protected:
	int OnGM_MINIMAP_ZOOMOUT();
	int OnGM_MINIMAP_ZOOMIN();

	int OnGM_OPEN_WORLDMAP();
	int OnGM_SELECT_WORLDMAP();
	int OnGM_CLOSE_UI();

// Command handler
protected:
	DWORD OnTCMD_MATERIALA( CString strParam);
	DWORD OnTCMD_MATERIALD( CString strParam);
	DWORD OnTCMD_LOADCHAR( CString strParam);
	DWORD OnTCMD_MAPCOLOR( CString strParam);
	DWORD OnTCMD_FINDPATH( CString strParam);
	DWORD OnTCMD_LOADMAP( CString strParam);
	DWORD OnTCMD_MINIMAP( CString strParam);
	DWORD OnTCMD_SHADOW( CString strParam);
	DWORD OnTCMD_SPEED( CString strParam);
	DWORD OnTCMD_BUILD( CString strParam);
	DWORD OnTCMD_LIGHT( CString strParam);
	DWORD OnTCMD_PATH( CString strParam);
	DWORD OnTCMD_MOVE( CString strParam);
	DWORD OnTCMD_EXIT( CString strParam);
	DWORD OnTCMD_HELP( CString strParam);
	DWORD OnTCMD_FPS( CString strParam);
	DWORD OnTCMD_POS( CString strParam);

	DWORD OnTCMD_LANDCOLLISION( CString strParam);
	DWORD OnTCMD_LIGHTMODE( CString strParam);
	DWORD OnTCMD_LIGHTDIR( CString strParam);
	DWORD OnTCMD_LIGHTA( CString strParam);
	DWORD OnTCMD_LIGHTD( CString strParam);

	DWORD OnTCMD_PATHMODE( CString strParam);
	DWORD OnTCMD_DELRANGE( CString strParam);

	DWORD OnTCMD_IMPOSTER( CString strParam );
	DWORD OnTCMD_SAVEIMPOSTER( CString strParam );
	
	DWORD OnTCMD_FOGON( CString strParam );
	DWORD OnTCMD_FOGOFF( CString strParam );

public:
	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRClick(UINT nFlags, CPoint pt);
	virtual void OnClick(UINT nFlags, CPoint pt);
	virtual void SwitchFocus(TComponent *pCandidate);

	virtual HRESULT Render( DWORD dwTickCount);

public:
	CTExplorerGame();
	virtual ~CTExplorerGame();
	void WritePosToLogFile(void);
};
