#include "stdafx.h"
#include "TMapViewer.h"


CTMapViewerObject::CTMapViewerObject()
{
}

CTMapViewerObject::~CTMapViewerObject()
{
}

void CTMapViewerObject::ReleaseData()
{
	CTMapViewerObjBase::ReleaseData();
	m_strNAME.Empty();
}

CString CTMapViewerObject::GetName()
{
	return m_strNAME;
}
