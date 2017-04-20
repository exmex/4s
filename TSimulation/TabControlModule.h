#pragma once

#define C_PAGES 4 
typedef struct tag_dlghdr 
{
	HWND hwndTab; // tab control
	HWND hwndDisplay; // current child dialog box
	RECT rcDisplay; // display rectangle for the tab control
	DLGTEMPLATE *apRes[C_PAGES];
	DLGPROC Proc[C_PAGES];
} DLGHDR; 

#define C_REPORT_PAGES 4 
typedef struct tag_dlghdrReport 
{
	HWND hwndTab; // tab control
	HWND hwndDisplay; // current child dialog box
	RECT rcDisplay; // display rectangle for the tab control
	DLGTEMPLATE *apRes[C_REPORT_PAGES];
	DLGPROC Proc[C_REPORT_PAGES];
} DLGHDR_REPORT; 

void WINAPI OnTabbedDialogInit_Data(HWND);
DLGTEMPLATE * WINAPI DoLockDlgRes(LPCSTR);
void WINAPI OnSelChanged(HWND, int iSetPage=-1);
HWND WINAPI GetTabFocus(HWND);
void WINAPI OnChildDialogInit(HWND);

void WINAPI OnTabbedDialogInit_Report(HWND);
void WINAPI OnSelChanged_Report(HWND, int iSetPage=-1);
HWND WINAPI GetTabFocus_Report(HWND);
void WINAPI OnChildDialogInit_Report(HWND);
