#include "stdafx.h"
#include "Resource.h"
#include "TMiniDump.h"
#include "TMailSender.h"
#include "TSystemInfo.h"
#include "TClientGame.h"

#define WINDOW_TITLE	"TDUMP"
#define WINDOW_CALSS	"DUMP"

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

BOOL CalculateBeginningOfCallInstruction ( UINT_PTR & dwRetAddr );
BSUMDRET OnMemoryDump();

extern "C" void * _ReturnAddress ( void ) ;

MINIDUMP_TYPE	g_eDumpType = MiniDumpNormal;
std::string		g_strUserData = "tttttt";
std::string		g_strLogID;
std::string		g_strCharID;
HWND			g_hWnd = NULL;
BOOL			g_bSaveMessage = FALSE;
CTMiniDump		theTMiniDump;
CTSystemInfo	g_SystemInfo;
CString 		g_strInfoMsg;
CString			g_strEnumMod;
SOCKET			g_sock;

CTMiniDump::CTMiniDump()
{
	// 예외가 발생하면 인터랩터
	::SetUnhandledExceptionFilter(TopLevelExceptionFilter);
}

CTMiniDump::~CTMiniDump()
{
}

BOOL CALLBACK ErrorDlgProc(HWND hDlg,UINT iMessage,WPARAM wParam,LPARAM lParam)
{
	switch(iMessage)
	{
	case WM_INITDIALOG:
		{
			CString strTITLE;
			strTITLE = CTChart::LoadString( TSTR_BUGREPORTER_TITLE);
			SetWindowText( hDlg, (LPCTSTR) strTITLE );


			static DWORD dwTEXT[][2] =
			{
				{ IDC_STATIC_MESSAGE, TSTR_BUGREPORTER_MESSAGE},
				{ IDC_STATIC_MESSAGE2, TSTR_BUGREPORTER_MESSAGE2},
				{ IDC_STATIC_CONTENTS, TSTR_BUGREPORTER_CONTENTS},
				{ IDOK, TSTR_BUGREPORTER_BTN_OK},
				{ IDCANCEL, TSTR_BUGREPORTER_BTN_CLOSE},
			};

			for( INT i=0 ; i < 5 ; ++i )
			{
				HWND hWnd = NULL;
				hWnd = GetDlgItem( hDlg, dwTEXT[ i ][0] );
				if( hWnd )
				{
					CString strTEXT;
					strTEXT = CTChart::LoadString( (TSTRING) dwTEXT[ i ][1] );
					SetWindowText( hWnd, (LPCTSTR)strTEXT);
				}
			}

			INT nPos = 0;
			CString str = g_strInfoMsg.Tokenize("\n",nPos);
			while( !str.IsEmpty() )
			{
				SendMessage( GetDlgItem(hDlg,IDC_LIST_ERROR), LB_ADDSTRING, 0, (LPARAM)str.GetBuffer());
				str = g_strInfoMsg.Tokenize("\n",nPos);
			}
			return TRUE;
		}
		break;

	case WM_COMMAND:
		switch (wParam)
		{
		case IDOK:
			{
				EndDialog(hDlg,IDOK);
				return TRUE;
			}
		case IDCANCEL:
			{
				EndDialog(hDlg,IDCANCEL);
				return TRUE;
			}
		}
		break;
	}
	
	return FALSE;
}

BOOL CALLBACK EnumerateLoadedModulesProc(PSTR ModuleName, ULONG ModuleBase, ULONG ModuleSize, PVOID UserContext)
{
	DWORD offset = *((DWORD*)UserContext);

	CString strTEMP;
	if (offset >= ModuleBase && offset <= ModuleBase + ModuleSize )
	{
		strTEMP.Format("%s", ModuleName);
		g_strEnumMod += strTEMP;
		return FALSE;
	} 
	else 
	{
		return TRUE;
	}
}

LONG WINAPI CTMiniDump::TopLevelExceptionFilter(_EXCEPTION_POINTERS *pException)
{
	if(g_sock != INVALID_SOCKET)
	{
		CTachyonSession * pSession = CTachyonSession::GetSession(g_sock);
		if(pSession)
		{
			CPacket vMSG;
			vMSG.SetID( CS_COUNTDOWN_REQ );
			pSession->Say( &vMSG );
		}
	}

	LONG lResult = EXCEPTION_CONTINUE_SEARCH;

	SYSTEMTIME time;
	::GetLocalTime(&time);
	
	CString strTime;
	strTime.Format("%d-%d-%d-%d-%d-%d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	
	g_SystemInfo.InitInfo();
	g_strInfoMsg = g_SystemInfo.GetInfo();

	if( g_hWnd )
		::ShowWindow(g_hWnd, FALSE);

	if( ::DialogBox(AfxGetInstanceHandle(), MAKEINTRESOURCE(ID_ERROR_DIALOG), NULL, ErrorDlgProc) == IDCANCEL )
	{
		Sleep( 1000 );
		return EXCEPTION_EXECUTE_HANDLER;
	}

	// 저장할 파일 이름 설정
	char szFilename[MAX_PATH];
	::GetModuleFileName(NULL, szFilename, MAX_PATH);
	CString strFileName = szFilename;

 	int iPos = strFileName.ReverseFind('.');
	if( iPos > 0 )
		strFileName = strFileName.Left(iPos);

	strFileName += "(" + strTime + ")" + ".dmp";

#ifdef MODIFY_ADD_MINIDUMP_FILE
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

	CString strFaultReason = GetFaultReason(pException);

	// 추가정보
	m_vUseData[0].Type = 0;
	m_vUseData[0].Buffer = (PVOID)((LPCSTR)strFaultReason);
	m_vUseData[0].BufferSize = strFaultReason.GetLength()+1;

	m_vUseData[1].Type = 1;
	m_vUseData[1].Buffer = (PVOID)g_strUserData.c_str();
	m_vUseData[1].BufferSize = g_strUserData.size();

	sUserInfo.UserStreamCount = 2; 
	sUserInfo.UserStreamArray = m_vUseData;

	// 정보기록
	if( MiniDumpWriteDump( hProcess, dwProcessID, hFile, g_eDumpType, &sExceptionInfo, &sUserInfo, NULL) )
		lResult = EXCEPTION_EXECUTE_HANDLER;
	
	CloseHandle(hFile);
#endif

	CString strSubject, strMailMsg;
	CString strErrInfo, strFirstStack;

	LogPlayerInfo(strMailMsg);

	if( GetExceptionDetail(pException, strErrInfo, strFirstStack) )
		strSubject.Format("[%.4u][%s] %s\n\n", DWORD(TBUILD_NUMBER), strFirstStack, szFilename);
	else
		strSubject.Format("[%.4u][0x00000000] %s\n\n", DWORD(TBUILD_NUMBER), szFilename);

	strMailMsg += strSubject + strErrInfo + g_strInfoMsg;

	CString strSenger = g_SystemInfo.m_strMachineName 
		+"(" + CString(g_strLogID.c_str()) + ")";

	CTMailSender MailSender;

	if( CTNationOption::GERMANY )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - Germany" );
	}
	else if ( CTNationOption::GREECE )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - Greece" );
	}
	else if( CTNationOption::FRANCE )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - france" );
	}
	else if( CTNationOption::ITALY )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - itay" );
	}
	else if( CTNationOption::POLAND )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - poland" );
	}
	else if( CTNationOption::CZECH )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - czech" );
	}
	else if( CTNationOption::SPAIN )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - spain" );
	}
	else if( CTNationOption::UNITEDKINGDOM )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - unitedkingdom" );
	}
	else if( CTNationOption::ENGLISH )
	{
		MailSender.SetMailServer("mail.zemiinc.co.kr");
		MailSender.SetReceiver("4sbug@zemiinc.co.kr", "4sbug");
		MailSender.SetSender("4sbug@zemiinc.co.kr", strSenger);
		MailSender.SetSubject("4Story BugReport (2) - English");
	}
	else if( CTNationOption::JAPAN )
	{
		MailSender.SetMailServer("mail.zemiinc.co.kr");
		MailSender.SetReceiver("4sbug@zemiinc.co.kr", "4sbug");
		MailSender.SetSender("4sbug@zemiinc.co.kr", strSenger);
		MailSender.SetSubject("4Story BugReport (2) - Japan");
	}
	else if ( CTNationOption::TAIWAN )
	{
		MailSender.SetMailServer("mail.zemiinc.co.kr");
		MailSender.SetReceiver("4sbug@zemiinc.co.kr", "4sbug");
		MailSender.SetSender("4sbug@zemiinc.co.kr", strSenger);
		MailSender.SetSubject("4Story Bugreport (2) - Taiwan");
	}
	else if ( CTNationOption::RUSSIA )
	{
		MailSender.SetMailServer("mail.zemiinc.co.kr");
		MailSender.SetReceiver("4sbug@zemiinc.co.kr", "4sbug");
		MailSender.SetSender("4sbug@zemiinc.co.kr", strSenger);
		MailSender.SetSubject("4Story Bugreport (2) - Russia");
	}
	else if( CTNationOption::ROMANIA )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - Romania" );
	}
	else if( CTNationOption::UNITEDSTATE )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - unitedstate" );
	}
	else if ( CTNationOption::HUNGARY )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport (2) - Hungary" );
	}
	else if ( CTNationOption::TURKEY )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport - Turkey" );
	}
	else if ( CTNationOption::PORTUGAL )
	{
		MailSender.SetMailServer("mail.4story.de");
		MailSender.SetReceiver("crash@4story.de", "crash");
		MailSender.SetSender("crash@4story.de", strSenger);
		MailSender.SetSubject( "4Story BugReport - Portugal" );
	}
	else
	{
		MailSender.SetMailServer("mail.zemiinc.co.kr");
		MailSender.SetReceiver("4sbug@zemiinc.co.kr", "4sbug");
		MailSender.SetSender("4sbug@zemiinc.co.kr", strSenger);
		MailSender.SetSubject("4Story BugReport (2) - Korea");
	}

	MailSender.SetMessage(strMailMsg);
#ifdef MODIFY_ADD_MINIDUMP_FILE
	MailSender.SetFile(strFileName);
#endif
	BOOL bSend = MailSender.SendMail();

#ifdef MODIFY_ADD_MINIDUMP_FILE
	DeleteFile(strFileName);
#endif
	return lResult;
}

void CTMiniDump::LogPlayerInfo(CString& outResult)
{
	CString strTEMP;

	outResult += "====================================\n";
	outResult += "Player Info\n";
	outResult += "------------------------------------\n";

	strTEMP.Format("Login ID: %s\n", g_strLogID.c_str());
	outResult += strTEMP;

	strTEMP.Format("Character ID: %s\n", g_strCharID.c_str());
	outResult += strTEMP;

	int nMap; float fPosX, fPosY;
	if( CTClientGame::GetSafeMainPos(nMap,fPosX,fPosY) )
	{
		strTEMP.Format("Position: Map=%d, X=%.2f, Z=%.2f\n", nMap,fPosX,fPosY);
		outResult += strTEMP;
	}

	CString strType; unsigned int nID;
	if( CTClientGame::GetSafeMainTarget(strType, nID) )
	{
		strTEMP.Format("Target: Type=%s, ID=%u\n", strType,nID);
		outResult += strTEMP;
	}

	outResult += "\n";
}

BOOL CTMiniDump::GetExceptionDetail(_EXCEPTION_POINTERS* pExceptionInfo, CString& outResult, CString& outFirstStack) 
{
	try
	{
		CString strTEMP;

		HANDLE hProcess	= GetCurrentProcess();
		HANDLE hThread	= GetCurrentThread();

		char module_name[256];
		time_t module_time;
		HMODULE hModule = GetModuleHandle(NULL);

		outResult.Empty();
		outFirstStack.Empty();

		GetModuleFileName(hModule, module_name, sizeof(module_name));
		module_time = (time_t)GetTimestampForLoadedLibrary(hModule);

		outResult += "====================================\n";
		outResult += "Error Description\n";
		outResult += "------------------------------------\n";

		strTEMP.Format("Module Base: %p\n", hModule);
		outResult += strTEMP;

		strTEMP.Format("Module Name: %s\n", module_name);
		outResult += strTEMP;

		strTEMP.Format("Module Time: 0x%08x - %s\n", module_time, ctime(&module_time));
		outResult += strTEMP;
		
		strTEMP.Format("Exception Type: 0x%08x\n", pExceptionInfo->ExceptionRecord->ExceptionCode);
		outResult += strTEMP;

		strTEMP.Format("Fault Reason: %s\n\n", GetFaultReason(pExceptionInfo));
		outResult += strTEMP;

		outResult += "====================================\n";
		outResult += "Register\n";
		outResult += "------------------------------------\n";

		CONTEXT&	context		= *pExceptionInfo->ContextRecord;

		strTEMP.Format("eax: 0x%08x\tebx: 0x%08x\n", context.Eax, context.Ebx);
		outResult += strTEMP;

		strTEMP.Format("ecx: 0x%08x\tedx: 0x%08x\n", context.Ecx, context.Edx);
		outResult += strTEMP;

		strTEMP.Format("esi: 0x%08x\tedi: 0x%08x\n", context.Esi, context.Edi);
		outResult += strTEMP;

		strTEMP.Format("ebp: 0x%08x\tesp: 0x%08x\n\n", context.Ebp, context.Esp);
		outResult += strTEMP;

		outResult += "====================================\n";
		outResult += "Call Stack\n";
		outResult += "------------------------------------\n";

		STACKFRAME stackFrame = {0,};
		stackFrame.AddrPC.Offset	= context.Eip;
		stackFrame.AddrPC.Mode		= AddrModeFlat;
		stackFrame.AddrStack.Offset	= context.Esp;
		stackFrame.AddrStack.Mode	= AddrModeFlat;
		stackFrame.AddrFrame.Offset	= context.Ebp;
		stackFrame.AddrFrame.Mode	= AddrModeFlat;

		for (int i=0; i < 512 && stackFrame.AddrPC.Offset; ++i) 
		{ 
			if (StackWalk(IMAGE_FILE_MACHINE_I386, hProcess, hThread, &stackFrame, &context, NULL, NULL, NULL, NULL) != FALSE)
			{
				strTEMP.Format("0x%08x", stackFrame.AddrPC.Offset);
				outResult += strTEMP + "\t";

				if( outFirstStack.IsEmpty() )
					outFirstStack = strTEMP;
		
				g_strEnumMod.Empty();
				EnumerateLoadedModules(hProcess, EnumerateLoadedModulesProc, &stackFrame.AddrPC.Offset);
				outResult += g_strEnumMod + "\n";
			} 
			else
				break;
		} 
	 
		outResult += "\n";

		//outResult += "====================================\n";
		//outResult += "Memory\n";
		//outResult += "------------------------------------\n";

		//BYTE* stack = (BYTE*)(context.Esp);
		//strTEMP.Format("< %08x - %08x >\n", context.Esp, context.Esp+1024);
		//outResult += strTEMP;

		//for(i=0; i<16; ++i)
		//{
		//	strTEMP.Format("%08X : ", context.Esp+i*16);
		//	outResult += strTEMP;

		//	for(int j=0; j<16; ++j) 
		//	{
		//		strTEMP.Format("%02X ", stack[i*16+j]);
		//		outResult += strTEMP;
		//	}

		//	outResult += "\n";
		//}
		
		//outResult += "\n";
	}
	catch(...)
	{
		outResult += "Crashed in GetExceptionDetail\n";
		return 0;
	}

	return 1;
}

void CTMiniDump::SetOption(_MINIDUMP_TYPE nDumpType)
{
	g_eDumpType = nDumpType;
}
void CTMiniDump::SetUserInfo(const char *pszTemp)
{
	g_strUserData = pszTemp;
}
void CTMiniDump::SetLogID(const char *pszID)
{
	if( pszID )
		g_strLogID = pszID;
	else
		g_strLogID.empty();
}
void CTMiniDump::SetCharID(const char *pszID)
{
	if( pszID )
		g_strCharID = pszID;
	else
		g_strLogID.empty();
}

void CTMiniDump::SetHWND(HWND hWnd)
{
	g_hWnd = hWnd;
}

void CTMiniDump::SetSocket(SOCKET sock)
{
	g_sock = sock;
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



