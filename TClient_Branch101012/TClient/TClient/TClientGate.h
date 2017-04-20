#pragma once


class CTClientGate : public CTClientObjBase
{
public:
	LPTGATE m_pTGATE;

	BYTE m_bResetGATE;
	BYTE m_bOPEN;

public:
	void InitTGate(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);
	void ResetTGate();

	BYTE OpenTGate( BYTE bOPEN);
	BYTE IsActiveTGate();

public:
	virtual LRESULT OnActEndMsg();

	virtual void ReleaseData();
	virtual void Release();

	virtual void SetAction(
		DWORD dwActionID,
		DWORD dwAniID = -1);

	virtual void CalcTick(
		LPDIRECT3DDEVICE9 pDevice,
		DWORD dwTick);

public:
	CTClientGate();
	virtual ~CTClientGate();
};
