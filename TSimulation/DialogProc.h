#pragma once

#include "prsht.h"

LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
LRESULT	CALLBACK MainProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	Page1Proc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	Page2Proc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	Page3Proc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	Page4Proc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	ReportPage1Proc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	ReportPage2Proc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	ReportPage3Proc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK	ReportPage4Proc(HWND, UINT, WPARAM, LPARAM);

void ListDelete(HWND);
void ListRebuild(HWND );

void UpdatePage1(HWND);
void UpdatePage2(HWND);
void UpdatePage3(HWND);
void CalcPage1(HWND);

void UpdateReportPage1(HWND);
void UpdateReportPage2(HWND);
void UpdateReportPage3(HWND);
void UpdateReportPage4(HWND);

void ReportPage1InitDlg(void);
void ReportPage2InitDlg(void);
void ReportPage3InitDlg(void);
void ReportPage4InitDlg(void);
void ReportPageAllInitDlg(void);
