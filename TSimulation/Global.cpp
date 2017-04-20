#include "stdafx.h"
#include "Global.h"

HWND hWndMain, hWndEdit, hWndPage1, hWndPage2, hWndPage3, hWndPage4=NULL;
HWND hWndReportPage1, hWndReportPage2, hWndReportPage3, hWndReportPage4;
HINSTANCE hInstMain;


//	메뉴에서 사용되는 임시 변수
EditResultData gsLevelStat
	={1,0,0,0,
	1,0,0,
	1,5,0,
	1,0,2,6,
	1,5,11,6,
	0,2,6,6,
	FALSE};

//	보고서 저장 경로
char ReportPath[_MAX_FNAME];
