#pragma once


class CTAICmdLeave : public CTAICommand
{
public:
	virtual BYTE CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);
	virtual BYTE ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);

public:
	CTAICmdLeave();
	virtual ~CTAICmdLeave();
};
