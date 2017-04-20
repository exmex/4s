#pragma once

class CTInstallSession;
struct InstFile;
struct TClientAddr;

typedef map< DWORD_PTR, CTInstallSession *>			MAPTSESSION, *LPMAPTSESSION;
typedef deque<InstFile *>							QINSTFILE, *LPQINSTFILE;

struct InstFile
{
	DWORD	m_dwVersion;
	CString m_strName;
	DWORD	m_dwSize;
	CString	m_strCheckSum;
};

#include "TInstallSession.h"