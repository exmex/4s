#include "stdafx.h"
#include "TClient.h"


CTClientObject::CTClientObject()
{
}

CTClientObject::~CTClientObject()
{
}

void CTClientObject::ReleaseData()
{
	CTClientObjBase::ReleaseData();
	m_strNAME.Empty();
}

CString CTClientObject::GetName()
{
	return m_strNAME;
}
