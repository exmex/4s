#pragma once


class CTAICommand
{
public:
	LPTAICOMMAND m_pCOMMAND;

	DWORD m_dwDelay;
	BYTE m_bLoop;

public:
	static CTAICommand *CreateCMD( LPTAICOMMAND pCMD);

public:
	virtual DWORD GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);
	virtual BYTE CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);
	virtual BYTE ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);

	virtual BYTE CheckCondition(
		LPTAICONDITION pCON,
		CTMonster *pMON,
		DWORD dwEventHost,
		DWORD dwRHId,
		BYTE bRHType);

public:
	CTAICommand();
	virtual ~CTAICommand();
};
