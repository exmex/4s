#pragma once


class CTMapSession : public CSession
{
public:
	BYTE m_bCanDelete;

public:
	CTMapSession();
	virtual ~CTMapSession();
};
