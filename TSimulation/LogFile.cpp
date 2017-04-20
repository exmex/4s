//	로그 파일
//	
//	
//	2003. 1. Bae, Jeong Yong

#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include "LogFile.h"

FILE* fpLogFile=NULL;
char *szLofFileName=NULL;

void LogFileStart(char *fname)
{
	fpLogFile = fopen(fname, "wt");
	szLofFileName = fname;
}

void LogFileRestart(void)
{
	fclose(fpLogFile);
	fpLogFile = fopen(szLofFileName, "wt");
}

void LogFileEnd(void)
{
	if(fpLogFile==NULL)return;
//	fprintf(fpLogFile, "End Game\n");
	fclose(fpLogFile);
	fpLogFile=NULL;
	szLofFileName=NULL;
}

void __cdecl LogFilePrintf(char *szFormat, ...)
// "printf-like" 
{
	char *pArguments;
	if(fpLogFile==NULL) return;

	pArguments = (char *)&szFormat + sizeof(szFormat);
	vfprintf(fpLogFile, szFormat, pArguments);
	fprintf(fpLogFile, "\n");
	fflush(fpLogFile);
}
