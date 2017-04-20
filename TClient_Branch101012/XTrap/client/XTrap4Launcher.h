
///////////////////////////////////////////////////////////////////////////////
// X-Trap API For Launcher
///////////////////////////////////////////////////////////////////////////////

#ifndef __XTRAPAPI_LAUNCHER_H
#define __XTRAPAPI_LAUNCHER_H

VOID XTrap_L_Patch(
	IN LPCSTR	lpArgv,
	IN LPCSTR	lpGamePath, 
	IN DWORD	dwTimeout
		);

VOID XTrap_L_Patch(
	IN  LPCSTR	lpArgv, 
	IN  LPCSTR	lpGamePath, 
	IN  DWORD	dwTimeout, 
	OUT LPCSTR	pMsg, 
	OUT LPCSTR	pErrCode,
	OUT BOOL   *pErrFlag
);

#endif
