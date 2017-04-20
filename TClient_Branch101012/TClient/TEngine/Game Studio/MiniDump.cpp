#include "stdafx.h"
#include "MiniDump.h"

CTMiniDump theTMiniDump;

CTMiniDump::CTMiniDump()
{
	// 예외가 발생하면 인터랩터
	::SetUnhandledExceptionFilter(TopLevelExceptionFilter);
}

CTMiniDump::~CTMiniDump()
{
}

LONG WINAPI CTMiniDump::TopLevelExceptionFilter(_EXCEPTION_POINTERS *pException)
{
	LONG lResult = EXCEPTION_CONTINUE_SEARCH;

	// 파일에 기록
	HANDLE hFile = CreateFile( "FUCK.dmp", GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == hFile )
		return lResult;

	HANDLE hProcess = GetCurrentProcess();
	DWORD dwProcessID = GetCurrentProcessId();
	DWORD dwThreadID = GetCurrentThreadId();

	MINIDUMP_EXCEPTION_INFORMATION sExceptionInfo;
	sExceptionInfo.ThreadId = dwThreadID;
	sExceptionInfo.ExceptionPointers = pException;
	sExceptionInfo.ClientPointers = FALSE;

	// 정보기록
	if( MiniDumpWriteDump( hProcess, dwProcessID, hFile, MiniDumpNormal, &sExceptionInfo, NULL, NULL) )
		lResult = EXCEPTION_EXECUTE_HANDLER;

	return lResult;
}
