#pragma once

#define OUTPUTSTR_MAX	298

void ClearTextOut(void);
void __cdecl TextoutScreen(char *szFormat, ...);
void RenderText(void);
void SetTextoutScreenHwnd(HWND);
void SetTextoutEditHwnd(HWND, int);
HWND GetTextoutEditHwnd(void);
void SetIsLogFile(BOOL);
BOOL GetIsLogFile(void);
void SetIsLogScreen(BOOL);
BOOL GetIsLogScreen(void);
void SetIsLogFull(BOOL);
BOOL GetIsLogFull(void);
