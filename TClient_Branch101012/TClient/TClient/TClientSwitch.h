#pragma once


class CTClientSwitch : public CTClientObjBase
{
public:
	LPTSWITCH m_pTSWITCH;
	BYTE m_bOPEN;

public:
	void InitTSWITCH(
		CD3DDevice *pDevice,
		CTachyonRes *pRES);
	void TurnON( BYTE bOPEN);

public:
	virtual void ReleaseData();
	virtual void Release();

public:
	CTClientSwitch();
	virtual ~CTClientSwitch();
};
