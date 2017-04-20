#include "stdafx.h"
#include "TPatchSession.h"

void CTPatchSession::SendCT_SERVICEMONITOR_REQ(DWORD dwTick, DWORD dwCurSession, DWORD dwCurUser, DWORD dwActiveUser)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEMONITOR_REQ)
		<< INT64(0)
		<< dwTick
		<< dwCurSession
		<< dwCurUser
		<< dwActiveUser;

	Say(pMsg);
}

void CTPatchSession::SendCT_PREPATCH_ACK(CString strFTP,
									  SOCKADDR_IN * pLoginAddr,
									  LPVPATCHFILE pvPatch)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PREPATCH_ACK)
		<< strFTP
		<< pLoginAddr->sin_addr.s_addr
		<< pLoginAddr->sin_port
		<< WORD(pvPatch->size());

	for(DWORD i=0; i<pvPatch->size(); i++)
	{
		(*pMsg)
			<< (*pvPatch)[i].m_dwBetaVer
			<< (*pvPatch)[i].m_strPath
			<< (*pvPatch)[i].m_strName
			<< (*pvPatch)[i].m_dwSize;
	}

	Say(pMsg);
}

void CTPatchSession::SendCT_PATCH_ACK(CString strFTP,
									  SOCKADDR_IN * pLoginAddr,
									  LPVPATCHFILE pvPatch)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_PATCH_ACK)
		<< strFTP
		<< pLoginAddr->sin_addr.s_addr
		<< pLoginAddr->sin_port
		<< WORD(pvPatch->size());

	for(DWORD i=0; i<pvPatch->size(); i++)
	{
		(*pMsg)
			<< (*pvPatch)[i].m_dwVersion
			<< (*pvPatch)[i].m_strPath
			<< (*pvPatch)[i].m_strName
			<< (*pvPatch)[i].m_dwSize;
	}

	Say(pMsg);
}

void CTPatchSession::SendCT_NEWPATCH_ACK(CString strFTP,
									  SOCKADDR_IN * pLoginAddr,
									  LPVPATCHFILE pvPatch,
									  DWORD dwMinBetaVer)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_NEWPATCH_ACK)
		<< strFTP
		<< pLoginAddr->sin_addr.s_addr
		<< pLoginAddr->sin_port
		<< dwMinBetaVer
		<< WORD(pvPatch->size());

	for(DWORD i=0; i<pvPatch->size(); i++)
	{
		(*pMsg)
			<< (*pvPatch)[i].m_dwVersion
			<< (*pvPatch)[i].m_strPath
			<< (*pvPatch)[i].m_strName
			<< (*pvPatch)[i].m_dwSize
			<< (*pvPatch)[i].m_dwBetaVer;
	}

	Say(pMsg);
}