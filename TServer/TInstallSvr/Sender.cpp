#include "TInstallSession.h"
#include "stdafx.h"

void CTInstallSession::SendCT_SERVICEMONITOR_REQ(DWORD dwTick, DWORD dwCurSession, DWORD dwCurUser, DWORD dwActiveUser)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_SERVICEMONITOR_REQ)
		<< dwTick
		<< dwCurSession
		<< dwCurUser
		<< dwActiveUser;

	Say(pMsg);
};

void CTInstallSession::SendCT_INSTALL_ACK(DWORD dwVersion, DWORD dwSegmentSize, CString strFTP, LPQINSTFILE pQFile)
{
	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_INSTALL_ACK)
		<< dwVersion
		<< dwSegmentSize
		<< strFTP
		<< WORD(pQFile->size());

	InstFile* pfile = pQFile->front();
	while(pQFile->size())
	{
		(*pMsg)
			<< pfile->m_dwVersion
			<< pfile->m_strName
			<< pfile->m_dwSize
			<< pfile->m_strCheckSum;

		delete pfile;
		pQFile->pop_front();
		pfile = pQFile->front();
	}

	Say(pMsg);
};
//<<파일ID
//<<세그먼트ID
//<<Addr갯수(최대 20)
//    <<Addr

void CTInstallSession::SendCT_FILEADDR_ACK(DWORD dwFileID, WORD wSegmentID, CString strCheckSum, deque<DWORD>* qIpAddr)
{
	if(!qIpAddr)
		return;

	int	nSize = (WORD)qIpAddr->size();
	if(nSize > 20)
		nSize = 20;

	CPacket * pMsg = new CPacket();
	pMsg->SetID(CT_FILEADDR_ACK)
		<< dwFileID
		<< wSegmentID
		<< strCheckSum
		<< (WORD)nSize;

	for(int i = 0; i < nSize; i++)
	{
		(*pMsg)
			<< (DWORD)qIpAddr->front();
		qIpAddr->pop_front();
	}

	Say(pMsg);
};