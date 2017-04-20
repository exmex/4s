#include "stdafx.h"
#include <stdio.h>
#include "TextOut.h"
#include "LogFile.h"

char szOutputString[OUTPUTSTR_MAX][_MAX_PATH*2]={{0,},};
char szOut[_MAX_PATH*2*300]={0,};
HWND hTextOutWnd=NULL;
HWND hTextOutEditWnd=NULL;
int nIDDlg;
BOOL bIsLogFile=FALSE;
BOOL bIsLogScreen=TRUE;
BOOL bIsLogFull=FALSE;

void ClearTextOut(void)
{
	int i;
	for(i=1 ; i<OUTPUTSTR_MAX ; i++)
		strcpy(szOutputString[i], "");
	strcpy(szOut, "");

	if(bIsLogScreen)
	{
		RenderText();
		if(hTextOutWnd==NULL) return;
		SendMessage(hTextOutWnd, WM_PAINT, 0, 0);
	}
}

void __cdecl TextoutScreen(char *szFormat, ...)
// "printf-like" 
{
	if(!bIsLogScreen&&!bIsLogFile) return;

	int i;
	char *pArguments;

	//	문자열 정리
	for(i=1 ; i<OUTPUTSTR_MAX ; i++)
		strcpy(szOutputString[i-1], szOutputString[i]);

	pArguments = (char *)&szFormat + sizeof(szFormat);

	//	마지막 줄에 복사
	vsprintf(szOutputString[OUTPUTSTR_MAX-1], szFormat, pArguments);
	if(bIsLogFile)
		LogFilePrintf(szOutputString[OUTPUTSTR_MAX-1], szFormat, pArguments);
	if(bIsLogScreen)
	{
		RenderText();
		if(hTextOutWnd==NULL) return;
//		SendMessage(hTextOutWnd, WM_PAINT, 0, 0);
	}
}

void RenderText(void)
{
	int i, j=0;
	int linecount;
/*
	HDC hdc;

	if(hTextOutWnd==NULL) return;
	hdc = GetDC(hTextOutWnd);

	HBRUSH hBrush;
	RECT rect;

	{
		//	지우기
		hBrush = (HBRUSH)CreateSolidBrush(RGB(255,255,255));
		GetClientRect(hTextOutWnd, &rect);
		FillRect(hdc, &rect, hBrush);
		DeleteObject(hBrush);

		strcpy(szOut, "");

		//	그리기

		for(i=0 ; i<OUTPUTSTR_MAX ; i++)
		{
			TextOut(hdc, 16, 8+i*16, szOutputString[i], strlen(szOutputString[i]));
			strcat(szOut, szOutputString[i]);
			strcat(szOut, "\r\n");
		}
	}

	ReleaseDC(hTextOutWnd, hdc);
//*/
//*
	strcpy(szOut, "");
	for(i=0 ; i<OUTPUTSTR_MAX ; i++)
	{
		if(strcmp(szOutputString[i], "")!=0)
		{
			if(j>0) strcat(szOut, "\r\n");
			strcat(szOut, szOutputString[i]);
			j=1;
		}
	}
//*/
	if(hTextOutEditWnd==NULL) return;
	SetDlgItemText(hTextOutEditWnd, nIDDlg, szOut);

	linecount = SendMessage(GetDlgItem(hTextOutEditWnd, nIDDlg), EM_GETLINECOUNT, 0, 0);

	SendMessage(GetDlgItem(hTextOutEditWnd, nIDDlg), EM_LINESCROLL, (WPARAM)0, (LPARAM)linecount);

//	SendMessage(hTextOutWnd, WM_PAINT, 0, 0);
}

void SetTextoutScreenHwnd(HWND hwnd)
{
	hTextOutWnd=hwnd;
}

void SetTextoutEditHwnd(HWND hwnd, int iID)
{
	hTextOutEditWnd=hwnd;
	nIDDlg=iID;
}

HWND GetTextoutEditHwnd(void)
{
	return GetDlgItem(hTextOutEditWnd, nIDDlg);
}

void SetIsLogFile(BOOL flag)
{
	bIsLogFile = flag;
}

BOOL GetIsLogFile(void)
{
	return bIsLogFile;
}

void SetIsLogScreen(BOOL flag)
{
	bIsLogScreen = flag;
}

BOOL GetIsLogScreen(void)
{
	return bIsLogScreen;
}

void SetIsLogFull(BOOL flag)
{
	bIsLogFull = flag;
}

BOOL GetIsLogFull(void)
{
	return bIsLogFull;
}