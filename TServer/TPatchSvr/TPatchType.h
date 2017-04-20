#pragma once

typedef struct tagPATCHFILE							PATCHFILE, *LPPATCHFILE;

class CTPatchSession;

typedef map< DWORD_PTR, CTPatchSession *>			MAPTSESSION, *LPMAPTSESSION;
typedef vector<PATCHFILE>							VPATCHFILE, *LPVPATCHFILE;

struct tagPATCHFILE
{
	DWORD m_dwVersion;
	CString m_strPath;
	CString m_strName;
	DWORD m_dwSize;
	DWORD m_dwBetaVer;
};

#include "TNetLib.h"
#include "TPatchSession.h"
