#pragma once


class CTClientMoveObj : public CTClientObjBase
{
public:
	D3DXMATRIX m_vScaleRACE;
	D3DXMATRIX m_vScaleSFX;

	BYTE m_bMouseDIR;
	BYTE m_bKeyDIR;
	BYTE m_bBlock;
	BYTE m_bBack;
	BYTE m_bDive;
	BYTE m_bRun;

	// Revision data of direction
	int m_nPITCH;
	int m_nDIR;

	// MainChair Àü¿ë
	D3DXVECTOR3 m_vSyncLastPosition;

protected:
	D3DXMATRIX GetPITCH( WORD wPITCH);
	void SetHeadDIR( FLOAT fDIR);

	virtual DWORD CalcJumpDamage();
	virtual DWORD CalcFallDamage();
	DWORD CalcPushDamage();

	DWORD CalcDropDamage(
		FLOAT fSpeed,
		FLOAT fGravity,
		FLOAT fTime);

public:
	virtual void ReleaseData();

	virtual void OnDIE(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void CalcDIR(
		DWORD dwTick,
		BYTE bMouseDIR,
		BYTE bKeyDIR);

	virtual void CalcDIR(
		WORD wMoveDIR,
		WORD wDIR,
		WORD wPITCH);

	virtual void CalcHeight(
		LPD3DXVECTOR3 pPREV,
		CTClientMAP *pMAP,
		DWORD dwTick);

	virtual void CalcTick(
		LPD3DXVECTOR3 pPREV,
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		DWORD dwTick);

	virtual void CalcSFX(
		CD3DDevice *pDevice,
		LPVECTOROBJSFX pSFX);

	virtual void ResetAction(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void SetAction(
		DWORD dwActionID,
		DWORD dwAniID = -1);

	virtual BYTE GetTAction();
	virtual BYTE GetTAction(
		BYTE bMouseDIR);

	virtual FLOAT GetFlyMAX(
		CTClientMAP *pMAP,
		FLOAT fPosX,
		FLOAT fPosZ);

	virtual WORD GetTargetPITCH(
		FLOAT fTargetX,
		FLOAT fTargetY,
		FLOAT fTargetZ);

	virtual WORD GetTargetDIR(
		FLOAT fTargetX,
		FLOAT fTargetZ);

	virtual void DoTRANS(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		WORD wMonID);

	virtual void DoRETRANS(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	virtual void DoSLIDE(
		LPD3DXVECTOR3 pATTACK,
		LPD3DXVECTOR3 pDEFEND,
		BYTE bForceRUN);

	virtual void Push(
		FLOAT fPushDist,
		LPD3DXVECTOR2 pDIR);

	virtual void CancelTAction();
	virtual void RemoveTAction();

	virtual BYTE CheckFall(
		CTClientMAP *pMAP,
		LPD3DXVECTOR2 pFallDIR);

	virtual BYTE Fall( LPD3DXVECTOR2 pFallDIR);
	virtual BYTE Jump();

public:
	CTClientMoveObj();
	virtual ~CTClientMoveObj();
};
