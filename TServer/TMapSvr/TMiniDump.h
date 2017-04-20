#pragma once

#include <dbghelp.h>

class CTMiniDump
{
public:
	CTMiniDump();
	~CTMiniDump();

	static void SetOption(_MINIDUMP_TYPE nDumpType);
	static void SetUserInfo(const char *pszTemp);
	static void SetHWND(HWND hWnd);
	static void SnapMemoryDump();
	
	static LONG WINAPI TopLevelExceptionFilter(_EXCEPTION_POINTERS *pException);

private:
	static char* GetFaultReason(_EXCEPTION_POINTERS* pExPtrs);
};

extern CTMiniDump theTMiniDump;