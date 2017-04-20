#pragma once


class CTClientObject : public CTClientObjBase
{
public:
	CString m_strNAME;

public:
	virtual void ReleaseData();
	virtual CString GetName();

public:
	CTClientObject();
	virtual ~CTClientObject();
};
