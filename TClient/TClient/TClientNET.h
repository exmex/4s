#pragma once

#include "TTextPopup.h"

struct tagTCHARINFO
{
	LPTREGIONINFO m_pTREGION;
	CTClientInven m_vEQUIP;
	DWORD m_dwID;

	CString m_strNAME;
	BYTE m_bStartAct;
	BYTE m_bSlotID;

	BYTE m_bContry;
	BYTE m_bClass;
	BYTE m_bLevel;
	BYTE m_bRace;
	BYTE m_bSex;

	BYTE m_bHair;
	BYTE m_bFace;
	BYTE m_bBody;
	BYTE m_bPants;
	BYTE m_bHand;
	BYTE m_bFoot;

	DWORD m_dwFame;
	DWORD m_dwFameColor;

	tagTCHARINFO()
	{
		m_pTREGION = NULL;
	};
};

class CTClientNET : public CFrameGroup
{
public:
	CTTextPopup* m_pCLASSTEXTPOPUP;
	CTTextPopup* m_pRACETEXTPOPUP;

	TComponent* m_pWarning;
	CTMiniPopupDlg* m_pChannelDLG;
	CTClientObject m_vBACK;
	CTClientChar* m_vMODEL[6];
	D3DXMATRIX m_matMODEL[6];
	INT m_nModelCount;
	CTClientCAM m_vCamera;
	CTClientWnd *m_pHost;
	CD3DDevice *m_pDevice;
	CD3DLight *m_pLIGHT;
	CTachyonRes *m_pRES;
	int m_nCamZOOM;
	BYTE m_bROT;
	BYTE m_bCharROT;
	FLOAT m_fROT;
	FLOAT m_fCharROT;
	BYTE m_bAniCharRot;
	FLOAT m_fAniCharRot;
	BYTE m_bSelServerNum;
	DWORD m_dwLevelTime;
	TComponent* m_pCandidate;
	BYTE m_bClickedIDBox;
	CString m_strCAURL;
	DWORD m_dwBackID;
	BYTE m_bWndAni;
	BYTE m_bWndAniType;
	BYTE m_bTickReset;
	BYTE m_bTargetROT;
	DWORD m_dwWndAni;
	DWORD m_dwWndAniMax;
	DWORD m_dwWndAniGM;
	FLOAT m_fTargetROT;
	FLOAT m_fRotDIR;
	D3DXVECTOR3 m_vPanPosition;
	FLOAT m_fPanRot;
	FLOAT m_fPanDist;
	void SetTargetSlot( INT nTargetSlot, INT nDir );
	void StartFadeIn( DWORD dwSec, DWORD dwGM );
	void StartFadeOut( DWORD dwSec, DWORD dwGM );

public:
	void ResetWndAni();
	void ResetSlot();
	void InitGroupUI();
	void UpdateRACE();

	void InitCameraLOGIN();
	void InitCameraCHAR();
	void InitMODEL( LPTCHARINFO pINFO, INT nIndex );
	void InitMODEL( VTCHARINFO& vCHARINFO );
	void CalcModelLocal( INT nCount );
	void ReleaseMODEL( INT nIndex );

	void InitLIGHT( BYTE bCountryID);
	void InitEffectLIGHT();
	void InitModelLIGHT(D3DXMATRIX& matWorld, BYTE bSelected);
	void InitDEFCAM();
	void InitCamera();

	void ResetLight();
	void ResetDEFFOG();
	void ResetFOG();
	void ClearModelSFX();

	// Scene calculator
	void CalcMODEL( DWORD dwTick);

	void LoadCAURL();

	D3DXVECTOR3 GetSlotDIR( INT nSlotID );
	D3DXVECTOR3 GetRotDIR( FLOAT fRadian );
	FLOAT GetSlotROT( INT nSlotID );

	void ChangeBACK( DWORD toState );
	void ClearModelAll();
	void DrawBack(DWORD dwColor);
	void DrawModelShadow();
	void ProcGM_OPEN_CHANNEL_LIST();

public:
	virtual void SetLevel( DWORD dwLevel);
	virtual BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void OnRButtonUp(UINT nFlags, CPoint pt);
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnChar( UINT nChar, int nRepCnt, UINT nFlags );
	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual void OnImeNotify( HWND hWnd, WPARAM wParam, LPARAM lParam);

	virtual HRESULT Render( DWORD dwTickCount);
	virtual void SwitchFocus(TComponent *pCandidate);
	void DefaultPosition( CPoint* vBASIS, BOOL bRestore );
	void ResetPosition();

public:
	CTClientNET();
	virtual ~CTClientNET();
};
