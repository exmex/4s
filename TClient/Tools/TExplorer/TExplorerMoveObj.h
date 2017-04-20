#pragma once


class CTExplorerMoveObj : public CTExplorerObjBase
{
public:
	D3DXVECTOR2 m_vJumpDIR;
	D3DXVECTOR2 m_vFallDIR;

	FLOAT m_fBreathHeight;
	FLOAT m_fJumpHeight;
	FLOAT m_fFallHeight;

	FLOAT m_fMoveSpeed;
	FLOAT m_fJumpSpeed;

	DWORD m_dwJumpTick;
	DWORD m_dwFallTick;

	BYTE m_bMouseDIR;
	BYTE m_bKeyDIR;
	BYTE m_bBack;
	BYTE m_bRun;

	// Revision data of direction
	int m_nPITCH;
	int m_nDIR;

protected:
	D3DXMATRIX GetPITCH( WORD wDIR);

public:
	virtual void ReleaseData();

	virtual void CalcDIR(
		DWORD dwTick,
		BYTE bMouseDIR,
		BYTE bKeyDIR);

	virtual void CalcDIR(
		WORD wMoveDIR,
		WORD wDIR);

	virtual void CalcHeight(
		LPD3DXVECTOR3 pPREV,
		CTExplorerMAP *pMAP,
		DWORD dwTick);

	virtual void CalcTick(
		LPD3DXVECTOR3 pPREV,
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		DWORD dwTick);

	virtual void ResetAction(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void SetAction(
		DWORD dwActionID,
		DWORD dwAniID = -1);

	virtual BYTE GetTAction();
	virtual BYTE GetTAction(
		BYTE bMouseDIR);

	virtual WORD GetTargetDIR(
		FLOAT fTargetX,
		FLOAT fTargetZ);

	virtual void CancelTAction();
	virtual void RemoveTAction();

	virtual BYTE Fall( LPD3DXVECTOR2 pFallDIR);
	virtual BYTE Jump();

public:
	CTExplorerMoveObj();
	virtual ~CTExplorerMoveObj();
};
