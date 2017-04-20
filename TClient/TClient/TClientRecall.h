#pragma once


class CTClientRecall : public CTClientMoveObj
{
public:
	static D3DXVECTOR2 m_vTPOS[TRECALL_MAX];

public:
	LPTMONTEMP m_pTEMP;

	CTClientSkill *m_pTDEFSKILL;
	CTClientSkill *m_pTCURSKILL;

	DWORD m_dwTargetID;
	DWORD m_dwHostID;

	DWORD m_dwEndLifeTick;

	BYTE m_bTargetType;
	BYTE m_bRecallType;
	BYTE m_bSubAI;
	BYTE m_bTPOS;
	BYTE m_bDEAD;
	BYTE m_bDIE;
	BYTE m_bAI;

	INT m_nRecallRunAwayIndex;
	TRUNAWAY_ARRAY m_vRecallRunAway;
	D3DXVECTOR3 m_vRecallRunAwayTarget;

public:
	FLOAT GetLOST( CTClientObjBase *pTARGET);
	FLOAT GetAB( CTClientObjBase *pTARGET);
	FLOAT GetLB( CTClientObjBase *pTARGET);
	DWORD GetLeftLifeTick();

	CTClientSkill *GetBestTSKILL( CTClientObjBase *pTARGET);
	BYTE GetRoamACT( LPD3DXVECTOR3 pTARGET);

public:
	virtual DWORD CalcJumpDamage();
	virtual DWORD CalcFallDamage();
	virtual BYTE IsAlliance( CTClientObjBase *pTARGET);
	virtual BYTE Fall( LPD3DXVECTOR2 pFallDIR);
	virtual BYTE GetDrawName();
	virtual void ReleaseData();
	virtual CString GetTitle();
	virtual CString GetName();
	virtual DWORD GetHostID();
	virtual BYTE GetTAction();
	virtual BYTE CanDIVE();
	virtual void InitRecall(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		WORD wTempID,
		BYTE bLevel);

	virtual D3DXVECTOR3 GetRoamTarget(
		LPD3DXMATRIX pDIR,
		FLOAT fPosX,
		FLOAT fPosY,
		FLOAT fPosZ);

	virtual D3DXVECTOR3 AdjustRoamTarget(
		CTClientChar* pHOST,
		CTClientMAP* pMAP,
		LPD3DXMATRIX pDIR,
		D3DXVECTOR3 vTARGET );

	virtual BYTE CheckFall(
		CTClientMAP *pMAP,
		LPD3DXVECTOR2 pFallDIR);

	virtual LRESULT OnActEndMsg();
	virtual void OnShotSkill();
	virtual void DoRETRANS(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	void ResetDefaultSkill();

public:
	CTClientRecall();
	virtual ~CTClientRecall();
};
