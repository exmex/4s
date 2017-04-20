#pragma once


class CTMapViewerObject : public CTMapViewerObjBase
{
public:
	CString m_strNAME;

public:
	virtual void ReleaseData();
	virtual CString GetName();

public:
	CTMapViewerObject();
	virtual ~CTMapViewerObject();
};
