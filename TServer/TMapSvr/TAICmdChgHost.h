#pragma once


class CTAICmdChgHost : public CTAICommand
{
public:
	virtual DWORD GetDelay( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);
	virtual BYTE CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);
	virtual BYTE ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);

public:
	CTAICmdChgHost();
	virtual ~CTAICmdChgHost();
};
