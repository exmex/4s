#pragma once


class CTExplorerObject : public CTExplorerObjBase
{
public:
	CString m_strNAME;

public:
	virtual void ReleaseData();
	virtual CString GetName();

public:
	CTExplorerObject();
	virtual ~CTExplorerObject();
};
