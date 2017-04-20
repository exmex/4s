#pragma once

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
	CTClientObject m_vDEFBACK;
	CTClientObject m_vBACK;
	CTClientChar m_vMODEL;
	CTClientCAM *m_pCamera;
	CTClientWnd *m_pHost;

	CD3DDevice *m_pDevice;
	CD3DLight *m_pLIGHT;
	CTachyonRes *m_pRES;

	// Camera setting
	int m_nCamZOOM;

	WORD m_wROT;
	BYTE m_bROT;

	CTMiniPopupDlg* m_pChannelDLG;
	BYTE m_bSelChannelNum;
	BYTE m_bSelServerNum;
	BYTE m_bSelServerGropNum;

	DWORD m_dwLevelTime;

	TComponent* m_pCandidate;

	BOOL m_bInvertCountrySel;
	BYTE m_bClickedIDBox;

	CString m_strCAURL; // CA:Create Account

public:
	void InitMODEL( LPTCHARINFO pINFO);
	void InitLIGHT( BYTE bCountryID);
	void InitDEFCAM();
	void InitCamera();

	void InitDEFBACK();
	void InitBACK();

	void ResetConstant();
	void ResetDEFFOG();
	void ResetFOG();

	// Scene calculator
	void CalcDEFCamera( DWORD dwTick);
	void CalcCamera( DWORD dwTick);
	void CalcMODEL( DWORD dwTick);

	void LoadCAURL();
public:
	virtual BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnImeNotify( HWND hWnd, WPARAM wParam, LPARAM lParam);

	virtual HRESULT Render( DWORD dwTickCount);
	virtual void SwitchFocus(TComponent *pCandidate);
	void DefaultPosition( CPoint* vBASIS, BOOL bRestore );
	void ResetPosition();

public:
	CTClientNET();
	virtual ~CTClientNET();
};
