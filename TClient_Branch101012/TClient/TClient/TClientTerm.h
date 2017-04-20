#pragma once


class CTClientTerm
{
public:
	LPTTERM m_pTTERM;

	BYTE m_bStatus;
	BYTE m_bCount;

public:
	BYTE GetResult();

public:
	CTClientTerm();
	virtual ~CTClientTerm();
};
