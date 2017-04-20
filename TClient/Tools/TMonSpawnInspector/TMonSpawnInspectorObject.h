#pragma once
#include "TMonSpawnInspectorObjBase.h"

class CTMonSpawnInspectorObject : public CTMonSpawnInspectorObjBase
{
public:
	CString m_strNAME;

public:
	virtual void ReleaseData();
	virtual CString GetName();

public:
	CTMonSpawnInspectorObject(void);
	virtual ~CTMonSpawnInspectorObject(void);
};