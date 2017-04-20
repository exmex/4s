#pragma once

class CTTournamentResult : public CTClientUIBase
{
public:
	CTTournamentResult( TComponent *pParent, CD3DDevice *pDevice, LP_FRAMEDESC pDesc);
	virtual ~CTTournamentResult();

	void SetInfo(
		DWORD dwLeftCharID,
		DWORD dwRightCharID,
		CString strLeft,
		CString strRight,
		DWORD dwLeftKillCount,
		DWORD dwRightKillCount,
		DWORD dwLeftHideTick,
		DWORD dwRightHideTick,
		BOOL bResult,
		BOOL bTrueIsLeftWinButRightWin );

	void BeginSelectWinner(
		BYTE bLeftValue,
		BYTE bRightValue);

	HRESULT Render( DWORD dwTickCount );
	void RenderCharacter(
		CTClientChar* pTCHAR,
		TComponent* pTargetComp,
		FLOAT fDist,
		FLOAT fTheta,
		FLOAT fPhi,
		FLOAT vOffsetX,
		FLOAT vOffsetY,
		FLOAT vOffsetZ);

	void SetCharacter(
		CTClientChar* pTCHAR,
		CTClientChar* pTARGET,
		BYTE bResult,
		BOOL bWin );

	void ShowComponent(BOOL bVisible=1);
	void OnLButtonDown(UINT nFlags, CPoint pt);
	void OnLButtonUp(UINT nFlags, CPoint pt);
	void OnMouseMove(UINT nFlags, CPoint pt);
	void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	BOOL DoMouseWheel(UINT nFlags, short zDelta, CPoint pt);

public:
	TComponent* m_pKILL1;
	TComponent* m_pKILL2;
	TComponent* m_pNAME1;
	TComponent* m_pNAME2;
	TComponent* m_pCAMERA_LEFT;
	TComponent* m_pCAMERA_RIGHT;
	TComponent* m_pWL1;
	TComponent* m_pWL2;
	TComponent* m_pDRAW;
	TComponent* m_pWINNER;
	TComponent* m_pWINNER_LEFT;
	TComponent* m_pWINNER_RIGHT;
	TComponent* m_pTEXT;
	TComponent* m_pTICK1;
	TComponent* m_pTICK2;
	TComponent* m_pTXT_HIDE;
	CD3DDevice *m_pDevice;
	DWORD m_dwLeftCharID;
	DWORD m_dwRightCharID;
	BOOL m_bResult;
	BOOL m_bTrueIsLeftWinButRightWin;
	CD3DCamera m_vCamera;
	CPoint m_ptDRAW;
	CPoint m_ptTEXT;

	BOOL m_bBeginDice;
	DWORD m_dwPrevTickDice;
	DWORD m_dwTickDice;
	BYTE m_bDiceLeftTeam;
	BYTE m_bDiceRightTeam;

	// ±¸¸éÁÂÇ¥°è
	FLOAT m_fDist;
	FLOAT m_fTheta;
	FLOAT m_fPhi;
	D3DXVECTOR3 m_vPosOffset;
	BOOL m_bDrag;
};
