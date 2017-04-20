#include "stdafx.h"
#include "TExplorer.h"


CTExplorerObject::CTExplorerObject()
{
}

CTExplorerObject::~CTExplorerObject()
{
}

void CTExplorerObject::ReleaseData()
{
	CTExplorerObjBase::ReleaseData();
	m_strNAME.Empty();
}

CString CTExplorerObject::GetName()
{
	return m_strNAME;
}
