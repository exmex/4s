//	로그 파일
//	
//	
//	2003. 1. Bae, Jeong Yong


#pragma once

#include <windows.h>

void LogFileStart(char *fname="logfile.txt");
void LogFileRestart(void);
void LogFileEnd(void);
void __cdecl LogFilePrintf(char *, ...);
