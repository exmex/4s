#pragma once


class CTClientMonster : public CTClientMoveObj
{
public:
	LPTMONTEMP m_pMON;

public:
	D3DXVECTOR3 m_vTGOAL;

	LPTSKILL m_pTDEFSKILL;
	DWORD m_dwFollowID;
	BYTE m_bFollowType;
	FLOAT m_fAB;

	BYTE m_bGoHome;
	BYTE m_bUPDATE;
	BYTE m_bPATH;
	BYTE m_bTD;
	BYTE m_bBACK;
	BYTE m_bHostMainChar;

public:
	FLOAT GetLOST( CTClientObjBase *pTARGET);
	FLOAT GetAB( CTClientObjBase *pTARGET);
	FLOAT GetLB( CTClientObjBase *pTARGET);

	void InitMonster(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		WORD wTempID);

public:
	virtual BYTE Fall( LPD3DXVECTOR2 pFallDIR);
	virtual BYTE GetDrawName();
	virtual void ReleaseData();
	virtual CString GetTitle();
	virtual CString GetName();
	virtual BYTE GetTAction();
	virtual BYTE IsInBattle();
	virtual BYTE CanDIVE();
	virtual BYTE IsDrawNameWhenDead();
	virtual BYTE IsSlidingWhenLastHit();

	virtual BYTE GetDrawNodeMark(
		LPTTERMPOS pTNODEPATH,
		LPTNPCPORTAL pTNODEPORTAL);

	virtual BYTE CheckFall(
		CTClientMAP *pMAP,
		LPD3DXVECTOR2 pFallDIR);

	virtual void OnMaintainEnd(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		CTClientSkill *pTSKILL);

	virtual void DoTRANS(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		WORD wMonID);

	virtual void DoRETRANS(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);

	INT GetRandomAniIndex(
		DWORD dwActionID,
		INT nCount);

public:
	CTClientMonster();
	virtual ~CTClientMonster();
};
