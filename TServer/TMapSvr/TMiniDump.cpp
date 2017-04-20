#include "stdafx.h"

//#include "TMiniDump.h"
//#include <string>

//using namespace std;

enum BSUMDRET
{
	eDUMP_SUCCEEDED ,
	eDBGHELP_NOT_FOUND ,
	eDBGHELP_MISSING_EXPORTS ,
	eBAD_PARAM ,
	eOPEN_DUMP_FAILED ,
	eMINIDUMPWRITEDUMP_FAILED ,
	eGETTHREADCONTEXT_FAILED ,
	eREADPROCMEM_FAILED ,
	eINVALID_ERROR 
};

typedef BOOL (WINAPI *MINIDUMPWRITEDUMP)(
	HANDLE hProcess, DWORD dwPid, HANDLE hFile, MINIDUMP_TYPE DumpType,
	CONST PMINIDUMP_EXCEPTION_INFORMATION ExceptionParam,
	CONST PMINIDUMP_USER_STREAM_INFORMATION UserStreamParam,
	CONST PMINIDUMP_CALLBACK_INFORMATION CallbackParam );

BOOL CalculateBeginningOfCallInstruction ( UINT_PTR & dwRetAddr );
BSUMDRET OnMemoryDump();

extern "C" void * _ReturnAddress ( void ) ;

MINIDUMP_TYPE	g_eDumpType = MiniDumpNormal;
std::string		g_strUserData = "tttttt";
HWND			g_hWnd = NULL;
BOOL			g_bSaveMessage = FALSE;
CTMiniDump		theTMiniDump;
CString			g_strErrorMsg;

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

	char szDllHelpPath[MAX_PATH] = {0};
	HMODULE hDLL = NULL;

	// DLL 로딩
	if(::GetModuleFileName(NULL, szDllHelpPath, _MAX_PATH))
	{
		char *pSlash = ::strrchr(szDllHelpPath, '\\');
		if( pSlash )
		{
			::lstrcpy(pSlash + 1, "DBGHELP.DLL");
			hDLL = ::LoadLibrary(szDllHelpPath);
		}
	}

	if( NULL == hDLL )
		return lResult;
	
	// DLL에서 MiniDumpWriteDump함수 얻어오기
	MINIDUMPWRITEDUMP pMiniDumpWriteDump = (MINIDUMPWRITEDUMP)::GetProcAddress(hDLL, "MiniDumpWriteDump");
	if( pMiniDumpWriteDump == NULL )
		return lResult;

	SYSTEMTIME time;
	::GetLocalTime(&time);
	
	CString strTime;
	strTime.Format("%d-%d-%d-%d-%d-%d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	
	CString strFaultReason = GetFaultReason(pException);

	// 저장할 파일 이름 설정
	char szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, MAX_PATH);
	CString strFileName = szFilename;

 	int iPos = strFileName.ReverseFind('.');
	if( iPos > 0 )
		strFileName = strFileName.Left(iPos);

	strFileName += "(" + strTime + ")" + ".dmp";

	// 파일에 기록
	HANDLE hFile = CreateFile(strFileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if( INVALID_HANDLE_VALUE == hFile )
		return lResult;

	HANDLE hProcess = GetCurrentProcess();
	DWORD dwProcessID = GetCurrentProcessId();
	DWORD dwThreadID = GetCurrentThreadId();

	MINIDUMP_EXCEPTION_INFORMATION sExceptionInfo;
	sExceptionInfo.ThreadId = dwThreadID;
	sExceptionInfo.ExceptionPointers = pException;
	sExceptionInfo.ClientPointers = FALSE;

	MINIDUMP_USER_STREAM_INFORMATION sUserInfo; 
	MINIDUMP_USER_STREAM m_vUseData[2];

	// 추가정보
	m_vUseData[0].Type = 0;
	m_vUseData[0].Buffer = (PVOID)((LPCSTR)strFaultReason);
	m_vUseData[0].BufferSize = strFaultReason.GetLength()+1;

	m_vUseData[1].Type = 1;
	m_vUseData[1].Buffer = (PVOID)g_strUserData.c_str();
	m_vUseData[1].BufferSize = (ULONG)g_strUserData.size();

	sUserInfo.UserStreamCount = 2; 
	sUserInfo.UserStreamArray = m_vUseData;

	// 정보기록
	if( pMiniDumpWriteDump( hProcess, dwProcessID, hFile, g_eDumpType, &sExceptionInfo, &sUserInfo, NULL) )
		lResult = EXCEPTION_EXECUTE_HANDLER;
	
	CloseHandle(hFile);

	return lResult;
}



void CTMiniDump::SetOption(_MINIDUMP_TYPE nDumpType)
{
	g_eDumpType = nDumpType;
}
void CTMiniDump::SetUserInfo(const char *pszTemp)
{
	g_strUserData = pszTemp;
}
void CTMiniDump::SetHWND(HWND hWnd)
{
	g_hWnd = hWnd;
}

char* CTMiniDump::GetFaultReason(_EXCEPTION_POINTERS* pExPtrs)
{
	if( NULL == pExPtrs || NULL == pExPtrs->ExceptionRecord || NULL ==  pExPtrs->ExceptionRecord->ExceptionCode )
		return "Unknown";

	if (::IsBadReadPtr(pExPtrs, sizeof(EXCEPTION_POINTERS))) 
		return "BAD EXCEPTION POINTERS";

	// 간단한 에러 코드라면 그냥 변환할 수 있다.
	//http://msdn.microsoft.com/library/default.asp?url=/library/en-us/debug/base/exception_record_str.asp
	switch( pExPtrs->ExceptionRecord->ExceptionCode )
	{
	case EXCEPTION_ACCESS_VIOLATION:         return "EXCEPTION_ACCESS_VIOLATION";		// 잘못된 참조
	case EXCEPTION_DATATYPE_MISALIGNMENT:    return "EXCEPTION_DATATYPE_MISALIGNMENT";  // 정렬되지않은 데이타접근
	case EXCEPTION_BREAKPOINT:               return "EXCEPTION_BREAKPOINT";				// 브레이크포인트에 걸렸을때
	case EXCEPTION_SINGLE_STEP:              return "EXCEPTION_SINGLE_STEP";			// 하나의 명령어를 실행할때
	case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:    return "EXCEPTION_ARRAY_BOUNDS_EXCEEDED";	// 배열이 넘어접근
	case EXCEPTION_FLT_DENORMAL_OPERAND:     return "EXCEPTION_FLT_DENORMAL_OPERAND";	// 정규화가 되지않는 작은값
	case EXCEPTION_FLT_DIVIDE_BY_ZERO:       return "EXCEPTION_FLT_DIVIDE_BY_ZERO";		// float 0으로나눔
	case EXCEPTION_FLT_INEXACT_RESULT:       return "EXCEPTION_FLT_INEXACT_RESULT";		// 소수를 정확하게 나타낼수없음
	case EXCEPTION_FLT_INVALID_OPERATION:    return "EXCEPTION_FLT_INVALID_OPERATION";	// 기타예외
	case EXCEPTION_FLT_OVERFLOW:             return "EXCEPTION_FLT_OVERFLOW";			// float오버플로우
	case EXCEPTION_FLT_STACK_CHECK:          return "EXCEPTION_FLT_STACK_CHECK";		// 스택에값이 넘치거나 너무작을경우
	case EXCEPTION_FLT_UNDERFLOW:            return "EXCEPTION_FLT_UNDERFLOW";			// 값이 작을때
	case EXCEPTION_INT_DIVIDE_BY_ZERO:       return "EXCEPTION_INT_DIVIDE_BY_ZERO";		// int를 0으로 나눔
	case EXCEPTION_INT_OVERFLOW:             return "EXCEPTION_INT_OVERFLOW";			// int오버플로우
	case EXCEPTION_PRIV_INSTRUCTION:         return "EXCEPTION_PRIV_INSTRUCTION";		// 명령어를 실행 할 수 없을때
	case EXCEPTION_IN_PAGE_ERROR:            return "EXCEPTION_IN_PAGE_ERROR";			// page를 로드 할 수 없는데 접근한경우
	case EXCEPTION_ILLEGAL_INSTRUCTION:      return "EXCEPTION_ILLEGAL_INSTRUCTION";	// 잘못된 명령어
	case EXCEPTION_NONCONTINUABLE_EXCEPTION: return "EXCEPTION_NONCONTINUABLE_EXCEPTION";// 진행할수 없는데 계속 진행시킬 경우
	case EXCEPTION_STACK_OVERFLOW:           return "EXCEPTION_STACK_OVERFLOW";			// 스택오버플로우
	case EXCEPTION_INVALID_DISPOSITION:      return "EXCEPTION_INVALID_DISPOSITION";	// 잘못된기능(?)접근 C에서는 발생하지 않음
	case EXCEPTION_GUARD_PAGE:               return "EXCEPTION_GUARD_PAGE";				// 접근하지못하는 page에 접근
	case EXCEPTION_INVALID_HANDLE:           return "EXCEPTION_INVALID_HANDLE";			// 잘못된 핸들
	case 0xE06D7363:                         return "Microsoft C++ Exception";			// Visual C++ 오류
	default:
		break;
	}

	return "Unknown";
}

void CTMiniDump::SnapMemoryDump()
{
	/*
	// http://windowssdk.msdn.microsoft.com/en-us/library/ms680519.aspx
	typedef enum _MINIDUMP_TYPE {
	MiniDumpNormal                         = 0x0000,	// 모든 쓰레드의 현재의 스택값만 저장
	MiniDumpWithDataSegs                   = 0x0001,	// 로드된 모튤의 데이타영역 저장
	MiniDumpWithFullMemory                 = 0x0002,	// 프로세서의 메모리 전체를 덤프
	MiniDumpWithHandleData                 = 0x0004,
	MiniDumpFilterMemory                   = 0x0008,
	MiniDumpScanMemory                     = 0x0010,
	MiniDumpWithUnloadedModules            = 0x0020,
	MiniDumpWithIndirectlyReferencedMemory = 0x0040,
	MiniDumpFilterModulePaths              = 0x0080,
	MiniDumpWithProcessThreadData          = 0x0100,
	MiniDumpWithPrivateReadWriteMemory     = 0x0200,
	} MINIDUMP_TYPE;
	*/
	MINIDUMP_TYPE ePreDumpType = g_eDumpType;
	g_eDumpType = MiniDumpWithFullMemory;
	g_bSaveMessage = FALSE;
	Sleep(1000);
	OnMemoryDump();
	Sleep(1000);
	g_bSaveMessage = TRUE;
	g_eDumpType = ePreDumpType;
}


#define SNAPPROLOG(Cntx)                                                   \
	__asm push  EBP														   \
	__asm mov   EBP , ESP												   \
	__asm sub   ESP , __LOCAL_SIZE										   \
	__asm mov   Cntx.Eax , EAX                                             \
	__asm mov   Cntx.Ebx , EBX                                             \
	__asm mov   Cntx.Ecx , ECX                                             \
	__asm mov   Cntx.Edx , EDX                                             \
	__asm mov   Cntx.Edi , EDI                                             \
	__asm mov   Cntx.Esi , ESI                                             \
	__asm xor   EAX , EAX                                                  \
	__asm mov   AX , GS                                                    \
	__asm mov   Cntx.SegGs , EAX                                           \
	__asm mov   AX , FS                                                    \
	__asm mov   Cntx.SegFs , EAX                                           \
	__asm mov   AX , ES                                                    \
	__asm mov   Cntx.SegEs , EAX                                           \
	__asm mov   AX , DS                                                    \
	__asm mov   Cntx.SegDs , EAX                                           \
	__asm mov   AX , CS                                                    \
	__asm mov   Cntx.SegCs , EAX                                           \
	__asm mov   AX , SS                                                    \
	__asm mov   Cntx.SegSs , EAX                                           \
	__asm mov  EAX , DWORD PTR [EBP]                                       \
	__asm mov  Cntx.Ebp , EAX                                              \
	__asm mov  EAX , EBP                                                   \
	__asm add  EAX , 8                                                     \
	__asm mov  Cntx.Esp , EAX                                              \
	__asm push ESI                                                         \
	__asm push EDI                                                         \
	__asm push EBX                                                         \
	__asm push ECX                                                         \
	__asm push EDX

#define SNAPEPILOG(eRetVal)                                                \
	__asm pop     EDX											           \
	__asm pop     ECX                                                      \
	__asm pop     EBX                                                      \
	__asm pop     EDI                                                      \
	__asm pop     ESI                                                      \
	__asm mov     EAX , eRetVal									           \
	__asm mov     ESP , EBP												   \
	__asm pop     EBP											           \
	__asm ret                     

#define COPYKEYCONTEXTREGISTERS(stFinalCtx,stInitialCtx)               \
	stFinalCtx.Eax   = stInitialCtx.Eax   ;                                \
	stFinalCtx.Ebx   = stInitialCtx.Ebx   ;                                \
	stFinalCtx.Ecx   = stInitialCtx.Ecx   ;                                \
	stFinalCtx.Edx   = stInitialCtx.Edx   ;                                \
	stFinalCtx.Edi   = stInitialCtx.Edi   ;                                \
	stFinalCtx.Esi   = stInitialCtx.Esi   ;                                \
	stFinalCtx.SegGs = stInitialCtx.SegGs ;                                \
	stFinalCtx.SegFs = stInitialCtx.SegFs ;                                \
	stFinalCtx.SegEs = stInitialCtx.SegEs ;                                \
	stFinalCtx.SegDs = stInitialCtx.SegDs ;                                \
	stFinalCtx.SegCs = stInitialCtx.SegCs ;                                \
	stFinalCtx.SegSs = stInitialCtx.SegSs ;                                \
	stFinalCtx.Ebp   = stInitialCtx.Ebp

#define k_CALLNEARBACK  5
#define k_CALLFARBACK   6

BOOL CalculateBeginningOfCallInstruction ( UINT_PTR & dwRetAddr )
{
	BOOL bRet = TRUE ;

	__try
	{
		BYTE * pBytes = (BYTE*)dwRetAddr ;

		if ( 0xE8 == *(pBytes - k_CALLNEARBACK) )
		{
			dwRetAddr -= k_CALLNEARBACK ;
		}
		else if ( 0xFF == *(pBytes - k_CALLFARBACK) )
		{
			dwRetAddr -= k_CALLFARBACK ;
		}
		else
		{
			bRet = FALSE ;
		}
	}
	__except ( EXCEPTION_EXECUTE_HANDLER )
	{
		bRet = FALSE ;
	}
	return ( bRet ) ;
}

BSUMDRET __declspec ( naked ) OnMemoryDump()
{
	_EXCEPTION_POINTERS sDumpInfo;

	CONTEXT stInitialCtx ;
	CONTEXT stFinalCtx ;

	BSUMDRET    eRet ;
	BOOL        bRetVal ;

	eRet = eDUMP_SUCCEEDED ;

	SNAPPROLOG ( stInitialCtx ) ;

	ZeroMemory ( &stFinalCtx , sizeof ( CONTEXT ) ) ;

	stFinalCtx.ContextFlags = 
		CONTEXT_FULL               |
		CONTEXT_CONTROL            |
		CONTEXT_DEBUG_REGISTERS    |
		CONTEXT_EXTENDED_REGISTERS |
		CONTEXT_FLOATING_POINT       ;

	bRetVal = GetThreadContext ( GetCurrentThread ( ) ,&stFinalCtx);
	if ( TRUE == bRetVal )
	{
		COPYKEYCONTEXTREGISTERS ( stFinalCtx , stInitialCtx ) ;

		UINT_PTR dwRetAddr = (UINT_PTR)_ReturnAddress() ;
		bRetVal = CalculateBeginningOfCallInstruction ( dwRetAddr );
		if ( TRUE == bRetVal )
		{
			stFinalCtx.Eip = (DWORD)dwRetAddr ;

			sDumpInfo.ContextRecord = &stFinalCtx;
			CTMiniDump::TopLevelExceptionFilter( &sDumpInfo );
			eRet = eDUMP_SUCCEEDED;
		}
		else
		{
			eRet = eGETTHREADCONTEXT_FAILED;
		}
	}

	SNAPEPILOG ( eRet ) ;
}



