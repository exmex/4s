#include "stdafx.h"
#include "TClient.h"


CTClientSession::CTClientSession()
{
	m_bSVR = SVR_NONE;
	m_bSvrID = 0;
	m_bLogicalValid = TRUE;
}

CTClientSession::~CTClientSession()
{
}
