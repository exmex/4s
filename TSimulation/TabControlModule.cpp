#include "stdafx.h"
#include <commctrl.h>
#include "resource.h"
#include "Global.h"
#include "TabControlModule.h"
#include "DialogProc.h"

void WINAPI OnTabbedDialogInit_Data(HWND hwndDlg) 
{ 
	DLGHDR *pHdr = (DLGHDR *) LocalAlloc(LPTR, sizeof(DLGHDR)); 

	DWORD dwDlgBase = GetDialogBaseUnits(); 
	int cxMargin = LOWORD(dwDlgBase) / 4; 
	int cyMargin = HIWORD(dwDlgBase) / 8; 
	TCITEM tie; 
	RECT rcTab; 
	HWND hwndTab; 
	POINT poTab;
//	int i; 

	// Save a pointer to the DLGHDR structure. 
	SetWindowLong(hwndDlg, DWL_USER, (LONG) pHdr); 

	
	pHdr->hwndTab = hwndTab = GetDlgItem(hwndDlg, IDC_TAB_DATA);

	// Add tabs for each day of the week. 
	tie.mask = TCIF_TEXT | TCIF_IMAGE; 
	tie.iImage = -1; 
	tie.pszText = "초기 능력치"; 
	TabCtrl_InsertItem(pHdr->hwndTab, 0, &tie);
	tie.pszText = "능력치 변환 공식(물리)"; 
	TabCtrl_InsertItem(pHdr->hwndTab, 1, &tie);
	tie.pszText = "능력치 변환 공식(마법)"; 
	TabCtrl_InsertItem(pHdr->hwndTab, 2, &tie);
	tie.pszText = "보고서 출력)"; 
	TabCtrl_InsertItem(pHdr->hwndTab, 3, &tie);
	// Add a tab for each of the three child dialog boxes. 
	
	// Lock the resources for the three child dialog boxes. 
	pHdr->apRes[0] = DoLockDlgRes(MAKEINTRESOURCE(IDD_PAGE1)); 
	pHdr->apRes[1] = DoLockDlgRes(MAKEINTRESOURCE(IDD_PAGE2)); 
	pHdr->apRes[2] = DoLockDlgRes(MAKEINTRESOURCE(IDD_PAGE3)); 
	pHdr->apRes[3] = DoLockDlgRes(MAKEINTRESOURCE(IDD_PAGE4)); 

	//	프로시져 함수이름 저장
	pHdr->Proc[0] = (DLGPROC)Page1Proc;
	pHdr->Proc[1] = (DLGPROC)Page2Proc;
	pHdr->Proc[2] = (DLGPROC)Page3Proc;
	pHdr->Proc[3] = (DLGPROC)Page4Proc;

	//	탭 컨트롤의 대화상자를 기준으로한 좌상단 위치를 계산한다.
	GetClientRect(pHdr->hwndTab, &rcTab);
	GetWindowRect(pHdr->hwndTab, &rcTab);
	poTab.x = rcTab.right;
	poTab.y = rcTab.bottom;
	ScreenToClient(GetParent(pHdr->hwndTab), &poTab);
	poTab.x = rcTab.left;
	poTab.y = rcTab.top;
	ScreenToClient(GetParent(pHdr->hwndTab), &poTab);

	// Determine the bounding rectangle for all child dialog boxes. 
	//	탭 컨트롤에 소속한 자식 대화상자의 크기를 가져와서 탭 컨트롤의 크기를 계산한다.
//	SetRectEmpty(&rcTab); 
//	for (i = 0; i < C_PAGES; i++) { 
//		if (pHdr->apRes[i]->cx > rcTab.right) 
//			rcTab.right = pHdr->apRes[i]->cx; 
//		if (pHdr->apRes[i]->cy > rcTab.bottom) 
//			rcTab.bottom = pHdr->apRes[i]->cy; 
//	} 
//	rcTab.right = rcTab.right * LOWORD(dwDlgBase) / 4; 
//	rcTab.bottom = rcTab.bottom * HIWORD(dwDlgBase) / 8; 
	
	//	자식 대화상자가 보여질 위치를 계산한다.
    // Calculate how large to make the tab control, so 
    // the display area can accommodate all the child dialog boxes. 
    TabCtrl_AdjustRect(pHdr->hwndTab, TRUE, &rcTab); 
    OffsetRect(&rcTab, cxMargin - rcTab.left + poTab.x, 
            cyMargin - rcTab.top + poTab.y); 
 
    // Calculate the display rectangle. 
    CopyRect(&pHdr->rcDisplay, &rcTab); 
    TabCtrl_AdjustRect(pHdr->hwndTab, FALSE, &pHdr->rcDisplay); 

	//	탭 컨트롤의 위치를 계산된 곳으로 이동한다.
    // Set the size and position of the tab control, buttons, 
    // and dialog box. 
//	SetWindowPos(pHdr->hwndTab, NULL, rcTab.left, rcTab.top, 
//		rcTab.right - rcTab.left, rcTab.bottom - rcTab.top, 
//		SWP_NOZORDER); 

	// Simulate selection of the first item. 
	OnSelChanged(hwndDlg); 

} 

// DoLockDlgRes - loads and locks a dialog box template resource. 
// Returns the address of the locked resource. 
// lpszResName - name of the resource 

DLGTEMPLATE * WINAPI DoLockDlgRes(LPCSTR lpszResName) 
{ 
	HRSRC hrsrc = FindResource(NULL, lpszResName, RT_DIALOG); 
	HGLOBAL hglb = LoadResource(hInstMain, hrsrc); 
	return (DLGTEMPLATE *) LockResource(hglb); 
} 


//The following function processes the TCN_SELCHANGE notification message for the main dialog box. The function destroys the dialog box for the outgoing page, if any. Then it uses the CreateDialogIndirect function to create a modeless dialog box for the incoming page. 

// OnSelChanged - processes the TCN_SELCHANGE notification. 
// hwndDlg - handle to the parent dialog box. 

void WINAPI OnSelChanged(HWND hwndDlg, int iSetPage) 
{ 
	DLGHDR *pHdr = (DLGHDR *) GetWindowLong( 
		hwndDlg, DWL_USER); 
	int iSel = TabCtrl_GetCurSel(pHdr->hwndTab); 
	if(iSetPage != -1) 
	{
		iSel = iSetPage;
		TabCtrl_SetCurSel(pHdr->hwndTab, iSetPage);
	}
	
	// Destroy the current child dialog box, if any. 
	if (pHdr->hwndDisplay != NULL) 
		DestroyWindow(pHdr->hwndDisplay); 
	
	// Create the new child dialog box. 
	pHdr->hwndDisplay = CreateDialogIndirect(hInstMain, pHdr->apRes[iSel], hwndDlg, pHdr->Proc[iSel]); 

	OnChildDialogInit(pHdr->hwndDisplay);
} 

HWND WINAPI GetTabFocus(HWND hwndDlg) 
{ 
	DLGHDR *pHdr = (DLGHDR *) GetWindowLong( 
		hwndDlg, DWL_USER); 

	return pHdr->hwndDisplay;
} 

// OnChildDialogInit - Positions the child dialog box to fall 
// within the display area of the tab control. 

void WINAPI OnChildDialogInit(HWND hwndDlg) 
{ 
	RECT rcRECT;
	HWND hwndParent = GetParent(hwndDlg); 
	DLGHDR *pHdr = (DLGHDR *) GetWindowLong( 
		hwndParent, DWL_USER); 
	GetWindowRect(pHdr->hwndTab, &rcRECT);
	SetWindowPos(hwndDlg, HWND_TOP, 
		pHdr->rcDisplay.left, pHdr->rcDisplay.top, 
		0, 0, SWP_NOSIZE); 
	SetFocus(pHdr->hwndTab);
	ShowWindow(pHdr->hwndDisplay, SW_SHOW);
} 

void WINAPI OnTabbedDialogInit_Report(HWND hwndDlg) 
{ 
	DLGHDR_REPORT *pHdr = (DLGHDR_REPORT *) LocalAlloc(LPTR, sizeof(DLGHDR_REPORT)); 

	DWORD dwDlgBase = GetDialogBaseUnits(); 
	int cxMargin = LOWORD(dwDlgBase) / 4; 
	int cyMargin = HIWORD(dwDlgBase) / 8; 
	TCITEM tie; 
	RECT rcTab; 
	HWND hwndTab; 
	POINT poTab;
//	int i; 

	// Save a pointer to the DLGHDR_REPORT structure. 
	SetWindowLong(hwndDlg, GWL_USERDATA, (LONG) pHdr); 

	
	pHdr->hwndTab = hwndTab = GetDlgItem(hwndDlg, IDC_TAB_REPORT);

	// Add tabs for each day of the week. 
	tie.mask = TCIF_TEXT | TCIF_IMAGE; 
	tie.iImage = -1; 
	tie.pszText = "기초 보고서"; 
	TabCtrl_InsertItem(pHdr->hwndTab, 0, &tie);
	tie.pszText = "모의 전투"; 
	TabCtrl_InsertItem(pHdr->hwndTab, 1, &tie);
	tie.pszText = "레벨업, 사용 시간"; 
	TabCtrl_InsertItem(pHdr->hwndTab, 2, &tie);
	tie.pszText = "전체 레벨업"; 
	TabCtrl_InsertItem(pHdr->hwndTab, 3, &tie);
	// Add a tab for each of the three child dialog boxes. 
	
	// Lock the resources for the three child dialog boxes. 
	pHdr->apRes[0] = DoLockDlgRes(MAKEINTRESOURCE(IDD_REPORT_PAGE1)); 
	pHdr->apRes[1] = DoLockDlgRes(MAKEINTRESOURCE(IDD_REPORT_PAGE2)); 
	pHdr->apRes[2] = DoLockDlgRes(MAKEINTRESOURCE(IDD_REPORT_PAGE3)); 
	pHdr->apRes[3] = DoLockDlgRes(MAKEINTRESOURCE(IDD_REPORT_PAGE4)); 

	//	프로시져 함수이름 저장
	pHdr->Proc[0] = (DLGPROC)ReportPage1Proc;
	pHdr->Proc[1] = (DLGPROC)ReportPage2Proc;
	pHdr->Proc[2] = (DLGPROC)ReportPage3Proc;
	pHdr->Proc[3] = (DLGPROC)ReportPage4Proc;

	//	탭 컨트롤의 대화상자를 기준으로한 좌상단 위치를 계산한다.
	GetClientRect(pHdr->hwndTab, &rcTab);
	GetWindowRect(pHdr->hwndTab, &rcTab);
	poTab.x = rcTab.right;
	poTab.y = rcTab.bottom;
	ScreenToClient(GetParent(pHdr->hwndTab), &poTab);
	poTab.x = rcTab.left;
	poTab.y = rcTab.top;
	ScreenToClient(GetParent(pHdr->hwndTab), &poTab);

    TabCtrl_AdjustRect(pHdr->hwndTab, TRUE, &rcTab); 
    OffsetRect(&rcTab, cxMargin - rcTab.left + poTab.x, 
            cyMargin - rcTab.top + poTab.y); 
 
    // Calculate the display rectangle. 
    CopyRect(&pHdr->rcDisplay, &rcTab); 
    TabCtrl_AdjustRect(pHdr->hwndTab, FALSE, &pHdr->rcDisplay); 

	// Simulate selection of the first item. 
	OnSelChanged_Report(hwndDlg); 

} 

void WINAPI OnSelChanged_Report(HWND hwndDlg, int iSetPage) 
{ 
	DLGHDR_REPORT *pHdr = (DLGHDR_REPORT *) GetWindowLong( 
		hwndDlg, GWL_USERDATA); 
	int iSel = TabCtrl_GetCurSel(pHdr->hwndTab); 
	if(iSetPage != -1) 
	{
		iSel = iSetPage;
		TabCtrl_SetCurSel(pHdr->hwndTab, iSetPage);
	}
	
	// Destroy the current child dialog box, if any. 
	if (pHdr->hwndDisplay != NULL) 
		DestroyWindow(pHdr->hwndDisplay); 
	
	// Create the new child dialog box. 
	pHdr->hwndDisplay = CreateDialogIndirect(hInstMain, pHdr->apRes[iSel], hwndDlg, pHdr->Proc[iSel]); 

	OnChildDialogInit_Report(pHdr->hwndDisplay);
} 

HWND WINAPI GetTabFocus_Report(HWND hwndDlg) 
{ 
	DLGHDR_REPORT *pHdr = (DLGHDR_REPORT *) GetWindowLong( 
		hwndDlg, GWL_USERDATA); 

	return pHdr->hwndDisplay;
} 

// OnChildDialogInit - Positions the child dialog box to fall 
// within the display area of the tab control. 

void WINAPI OnChildDialogInit_Report(HWND hwndDlg) 
{ 
	RECT rcRECT;
	HWND hwndParent = GetParent(hwndDlg); 
	DLGHDR_REPORT *pHdr = (DLGHDR_REPORT *) GetWindowLong( 
		hwndParent, GWL_USERDATA); 
	GetWindowRect(pHdr->hwndTab, &rcRECT);
	SetWindowPos(hwndDlg, HWND_TOP, 
		pHdr->rcDisplay.left, pHdr->rcDisplay.top, 
		0, 0, SWP_NOSIZE); 
	SetFocus(pHdr->hwndTab);
	ShowWindow(pHdr->hwndDisplay, SW_SHOW);
} 
