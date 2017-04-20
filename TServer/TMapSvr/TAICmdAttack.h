#pragma once


class CTAICmdAttack : public CTAICommand
{
public:
	virtual BYTE CanRun( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);
	virtual BYTE ExecAI( CTMonster *pMON, DWORD dwEventHost, DWORD dwRHId, BYTE bRHType);

public:
	CTAICmdAttack();
	virtual ~CTAICmdAttack();
};
