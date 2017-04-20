#include "StdAfx.h"
#include "TMonSpawnInspectorObject.h"

CTMonSpawnInspectorObject::CTMonSpawnInspectorObject(void)
:	CTMonSpawnInspectorObjBase()
{
}

CTMonSpawnInspectorObject::~CTMonSpawnInspectorObject(void)
{
}

void CTMonSpawnInspectorObject::ReleaseData()
{
	CTMonSpawnInspectorObjBase::ReleaseData();
	m_strNAME.Empty();
}

CString CTMonSpawnInspectorObject::GetName()
{
	return m_strNAME;
}