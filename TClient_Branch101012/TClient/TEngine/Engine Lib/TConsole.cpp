#include "Stdafx.h"
#include "tconsole.h"

#define ERR(bSuccess)	{ if(!bSuccess) return; }
#define CHECK(hHandle)	{ if(hHandle == NULL) return; };

CTConsole::CTConsole(void)
	: m_hConsole(NULL)
{
}

CTConsole::~CTConsole(void)
{
}

CTConsole* CTConsole::GetInstance()
{
	static CTConsole obj;
	return &obj;
}

bool CTConsole::Create(const char *szTitle, bool bNoClose)
{
	if(m_hConsole != NULL)
		return false;
	
	if(!AllocConsole())
		return false;
	
	m_hConsole = CreateFile("CONOUT$", GENERIC_WRITE|GENERIC_READ, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, 0);
	if(m_hConsole == INVALID_HANDLE_VALUE)
		return false;
	
	if(SetConsoleMode(m_hConsole, ENABLE_PROCESSED_OUTPUT | ENABLE_WRAP_AT_EOL_OUTPUT ) == 0)
		return false;
	
	if(bNoClose)
		DisableClose();
				
	if(szTitle != NULL)
		SetConsoleTitle(szTitle);

	return true;
}

void CTConsole::Color(WORD wColor)
{
	CHECK(m_hConsole);

	if(wColor != NULL)
		SetConsoleTextAttribute(m_hConsole, wColor );
	else
		SetConsoleTextAttribute(m_hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // white text on black bg
}

void CTConsole::Output(const char* szOutput, ...)
{
	CHECK(m_hConsole);

	DWORD		dwWritten;
	char		out[2048];
	va_list		va;

	if( szOutput == NULL )
		sprintf(out, "\n");
	else
	{
		va_start(va, szOutput);
		vsprintf(out, szOutput, va);
		va_end(va);	
	}
   
	WriteConsole(m_hConsole,out,strlen(out),&dwWritten,0);
}

void CTConsole::SetTitle(const char *title)
{
	SetConsoleTitle(title);
}

char* CTConsole::GetTitle()
{
	static char szWindowTitle[256] = "";
	GetConsoleTitle(szWindowTitle,sizeof(szWindowTitle));

	return szWindowTitle;
}


HWND CTConsole::GetHWND()
{
	if(m_hConsole == NULL) 
		return NULL;

	return FindWindow("ConsoleWindowClass",GetTitle());
}

void CTConsole::Show(bool bShow)
{
	CHECK(m_hConsole);

	HWND hWnd = GetHWND();
	if(hWnd != NULL)
		ShowWindow(hWnd, SW_HIDE ? SW_SHOW : bShow);
}

void CTConsole::DisableClose()
{
	CHECK(m_hConsole);

	HWND hWnd = GetHWND();
	
	if(hWnd != NULL)
	{
		HMENU hMenu = GetSystemMenu(hWnd,0);
		if(hMenu != NULL)
		{
			DeleteMenu(hMenu, SC_CLOSE, MF_BYCOMMAND);
			DrawMenuBar(hWnd);
		}
	}
}


void CTConsole::Clear()
{
	CHECK(m_hConsole);

	COORD coordScreen = { 0, 0 };

	BOOL bSuccess;
    DWORD cCharsWritten;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD dwConSize; 

    bSuccess = GetConsoleScreenBufferInfo( m_hConsole, &csbi );
    ERR(bSuccess);
    dwConSize = csbi.dwSize.X * csbi.dwSize.Y;

    bSuccess = FillConsoleOutputCharacter( m_hConsole, (TCHAR) ' ', dwConSize, coordScreen, &cCharsWritten );
    ERR(bSuccess);

    bSuccess = GetConsoleScreenBufferInfo( m_hConsole, &csbi );
    ERR(bSuccess);

    bSuccess = FillConsoleOutputAttribute( m_hConsole, csbi.wAttributes, dwConSize, coordScreen, &cCharsWritten );
    ERR(bSuccess);

    bSuccess = SetConsoleCursorPosition( m_hConsole, coordScreen );
    ERR(bSuccess);
}


HANDLE CTConsole::GetHandle()
{
	return m_hConsole;
}

void CTConsole::Close()
{
	FreeConsole();
	m_hConsole = NULL;
}

void TRACE(LPCSTR szOutput, ...)
{
#if defined(USE_CONSOLE) || defined(_DEBUG)
	
	static CStdioFile logfile("log.txt", CFile::modeCreate|CFile::modeWrite|CFile::typeText|CFile::shareDenyWrite);

	char out[2048];
	va_list va;

	if( szOutput == NULL ) 
		sprintf(out,"\n");
	else
	{
		char temp[2048];

		va_start(va, szOutput);
		vsprintf(temp, szOutput, va);
		va_end(va);	

		//SYSTEMTIME time;
		//::GetLocalTime(&time);

		//sprintf(out, "%.2d:%.2d:%.2d:%.3d> %s", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, temp);
		sprintf(out, "%s", temp);
	}

#ifdef USE_CONSOLE
	CTConsole* pConsole = CTConsole::GetInstance();
	if( pConsole->IsValid() )
		pConsole->Output(out);
#elif _DEBUG
	OutputDebugStr(out);
#endif

	logfile.WriteString(out);

#endif

}




















