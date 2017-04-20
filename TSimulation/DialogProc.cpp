#include "stdafx.h"
#include <prsht.h>
#include <commctrl.h>
#include <richedit.h>
#include "resource.h"
#include "Global.h"
#include "DialogProc.h"
#include "OutputModule.h"
#include "ChartDB.h"
#include "TabControlModule.h"
#include "FileBox.h"
#include "TextOut.h"
#include "TreeView.h"

#define DEFAULT_SAVEFILENAME	"NoName.Sta"

BOOL IsSaved=TRUE, IsSaveView=TRUE;
char SaveFilename[_MAX_FNAME]=DEFAULT_SAVEFILENAME;
char PathFilename[_MAX_FNAME]="경로만 인식합니다.";
char EditorPath[_MAX_FNAME];

HWND	hWndReport1ComboRace, hWndReport1ComboRace2, hWndReport1ComboClass, hWndReport1LevelCombo;
HWND	hWndReport2ComboRaceAt, hWndReport2ComboClassAt, hWndReport2ComboLevelAt;
HWND	hWndReport2ComboRaceDf, hWndReport2ComboClassDf, hWndReport2ComboLevelDf;
HWND	hWndReport3ComboRaceAt, hWndReport3ComboRaceAt2, hWndReport3ComboClassAt;
HWND	hWndReport3ComboRaceDf, hWndReport3ComboRaceDf2, hWndReport3ComboClassDf;
HWND	hWndReport3ProgressBar;
HWND	hWndReport4ComboRaceAt, hWndReport4ComboRaceAt2, hWndReport4ComboClassAt;
HWND	hWndReport4ComboClassDf;
HWND	hWndReport4ProgressBar;
HWND	hWndChkbox, hWndEditPath;
HWND	hWndListRaceStat, hWndListRaceUseItem, hWndListRaceUseItemArmor;

int iCurrentRaceList=0;

// Mesage handler for about box.
BOOL CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}

LRESULT	CALLBACK MainProc(HWND hWnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	HICON       hicon ;
	char str[_MAX_FNAME];
	HWND HwndTemp;

    switch (iMsg)
    {
		case WM_INITDIALOG :
			hicon = LoadIcon (hInstMain, MAKEINTRESOURCE (IDI_TSIMULATION)) ;
			SendMessage (hWnd, WM_SETICON, (WPARAM) (BOOL) TRUE, (LPARAM) (HICON) hicon) ;

			hWndEdit = hWnd;
			hWndChkbox = GetDlgItem(hWnd, IDC_CHECK_ISFILE);
			hWndEditPath = GetDlgItem(hWnd, IDC_EDIT_VIEWPATH);
			SendMessage(hWndChkbox, BM_SETCHECK, (WPARAM)gsLevelStat.mIsFile, 0);
			GetCurrentDirectory(_MAX_FNAME, EditorPath);
			strcpy(ReportPath, EditorPath);
			SetDlgItemText(hWnd, IDC_EDIT_VIEWPATH, ReportPath);
			

			OnTabbedDialogInit_Data(hWnd);
			OnTabbedDialogInit_Report(hWnd);

			OnSelChanged(hWnd, 0);
//			SetTextoutScreenHwnd(hWndEdit);
			SetIsLogFile(FALSE);
//			SetTextoutEditHwnd(hWnd, IDC_EDIT_RESULT);
			{
				BOOL flag=GetIsLogScreen();
				SetIsLogScreen(TRUE);
				TextoutScreen("\r\n\t\t\t\t보고서 출력 화면");
				SetIsLogScreen(flag);

			}
//			SetDlgItemText(hWndPage4, IDC_EDIT_RESULT, "메인 출력");
			IsSaved=TRUE;
			IsSaveView=TRUE;

			break;
		case WM_NOTIFY:
			{
				LPNMHDR pnmh;

				switch(wParam)
				{
					case IDC_TAB_DATA:
						pnmh=(LPNMHDR)lParam;
						switch(pnmh->code)
						{
							case TCN_SELCHANGE:
							OnSelChanged(hWnd);
							break;
						}
						break;
					case IDC_TAB_REPORT:
						pnmh=(LPNMHDR)lParam;
						switch(pnmh->code)
						{
							case TCN_SELCHANGE:
							OnSelChanged_Report(hWnd);
							break;
						}
						break;
				}
			}
			break;
		case WM_COMMAND :
			switch(LOWORD(wParam))
			{
				case IDM_ABOUT:
					//	OnSelChanged(hWnd);
					DialogBox(hInstMain, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
					//	DoCreateTabControl(hWnd);
					break;
				case IDM_NEW:
					if(!IsSaved) 
					{
						int result;
						result=MessageBox(hWnd, 
								"내용이 변경되었습니다.\n저장할까요 ?", "파일 저장 확인",
								MB_ICONEXCLAMATION | MB_YESNOCANCEL);
						if (IDYES == result)
						{
							SendMessage( hWnd, WM_COMMAND, IDM_SAVE, 0 );
						}
						if (IDCANCEL==result) return 0;
					}
					DeleteClassModule();
					InitClassModule();
					strcpy(SaveFilename, DEFAULT_SAVEFILENAME);
					iCurrentRaceList=0;
					OnSelChanged(hWnd, 0);
					SendMessage(hWndPage1, WM_USER, 0, 0 );
					IsSaved=TRUE;
					ClearTextOut();
					SendMessage( hWnd, WM_PAINT, 0, 0 );
					IsSaveView=TRUE;
					break;
				case IDM_LOAD:
					if(!IsSaved) 
					{
						int result;
						result=MessageBox(hWnd, 
								"내용이 변경되었습니다.\n저장할까요 ?", "파일 저장 확인",
								MB_ICONEXCLAMATION | MB_YESNOCANCEL);
						if (IDYES == result)
						{
							SendMessage( hWnd, WM_COMMAND, IDM_SAVE, 0 );
						}
						if (IDCANCEL==result) return 0;
					}
					if(OpenBox(hWnd, SaveFilename, EditorPath))
					{
						if(!pBalance->IsLoadFile(SaveFilename))
						{
							MessageBox(hWnd, "올바른 파일이 아닙니다.", "열기 실패", MB_OK );
							break;
						}
						DeleteClassModule();
						InitClassModule();
						if(!pBalance->LoadFile(SaveFilename))
						{
							MessageBox(hWnd, "올바른 파일이 아닙니다.", "열기 실패", MB_OK );
							DeleteClassModule();
							InitClassModule();
						}
//						UpdateWindow(hWnd);
						iCurrentRaceList=0;
						OnSelChanged(hWnd, 0);
						SendMessage(hWndPage1, WM_USER, 0, 0 );
						IsSaved=TRUE;
						ClearTextOut();
						SendMessage( hWnd, WM_PAINT, 0, 0 );
					}
					IsSaveView=TRUE;
					break;
				case IDM_SAVE:
//					pBalance->SaveFile("1.sta");
					if(strcmp(DEFAULT_SAVEFILENAME, SaveFilename)==0)
					{
						if(SaveBox(hWnd, SaveFilename, EditorPath))
						{
							pBalance->SaveFile(SaveFilename);
							IsSaved=TRUE;
							SendMessage( hWnd, WM_PAINT, 0, 0 );
						}
					}
					else
					{
						pBalance->SaveFile(SaveFilename);
						IsSaved=TRUE;
						SendMessage( hWnd, WM_PAINT, 0, 0 );
					}
					break;
				case IDM_SAVEAS:
					if(SaveBox(hWnd, SaveFilename, EditorPath))
					{
						pBalance->SaveFile(SaveFilename);
						IsSaved=TRUE;
						SendMessage( hWnd, WM_PAINT, 0, 0 );
					}
					break;
				case IDM_EXIT:
					SendMessage( hWnd, WM_DESTROY, 0, 0 );
					break;
				case ID_ACCELERATOR_ENTER:
				case ID_ACCELERATOR_TAB:					
					HwndTemp = GetNextDlgTabItem(GetTabFocus(hWnd), GetFocus(), FALSE);
					SetFocus(HwndTemp);
					SendMessage(HwndTemp, EM_SETSEL, (WPARAM) wParam, 0); 
					break;
				case ID_ACCELERATOR_STAB:
					HwndTemp = GetNextDlgTabItem(GetTabFocus(hWnd), GetFocus(), TRUE);
					SetFocus(HwndTemp);
					SendMessage(HwndTemp, EM_SETSEL, (WPARAM) wParam, 0); 
					break;
				case IDC_CHECK_ISFILE:
					{
						LRESULT lResult;
						lResult = SendMessage(hWndChkbox, BM_GETCHECK, 0, 0);
						if(lResult==BST_CHECKED)
							gsLevelStat.mIsFile = TRUE;
						else 
							gsLevelStat.mIsFile = FALSE;
					}
					break;
				case IDC_BUTTON_SETPATH:
					{
						char PrePathName[_MAX_FNAME];
						strcpy(PrePathName, ReportPath);

						if(PathBox(hWnd, hInstMain, PathFilename, PrePathName))
						{
							strcpy(ReportPath, PrePathName);
							SetDlgItemText(hWnd, IDC_EDIT_VIEWPATH, ReportPath);
						}
					}
					break;
			}
			break;
		case WM_PAINT :
			sprintf(str, "캐릭터 밸런스 - ");
			strcat(str, SaveFilename);
			if(!IsSaved) strcat(str, " (저장안됨)");
			SetWindowText(hWnd, str);
			break;
		case WM_KEYDOWN:
//			SendMessage( hWndPage1, WM_KEYDOWN, wParam, lParam );
            break;
		case WM_CLOSE :
		case WM_DESTROY:
			if(!IsSaved) 
			{
				int result;
				result=MessageBox(hWnd, 
					"내용이 변경되었습니다.\n저장할까요 ?", "파일 저장 확인",
					MB_ICONEXCLAMATION | MB_YESNOCANCEL);
				if (IDYES == result)
				{
					SendMessage( hWnd, WM_COMMAND, IDM_SAVE, 0 );
				}
				if (IDCANCEL==result) return 0;
			}

			SendMessage(hWndMain, WM_DESTROY, 0, 0 );
            return 0 ;
    }
    return FALSE ;
}

BOOL CALLBACK Page1Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int iData;
	double fData;
	int iOffset;
	int i;
	char str[_MAX_FNAME];
	LPNM_LISTVIEW linfo;

	switch (message)
	{
		case WM_INITDIALOG:
			hWndPage1 = hDlg;

			hWndListRaceStat = GetDlgItem(hWndPage1, IDC_LIST_RACESTAT);
			hWndListRaceUseItem = GetDlgItem(hDlg, IDC_COMBO_RACE_USEITEM);
			hWndListRaceUseItemArmor = GetDlgItem(hDlg, IDC_COMBO_RACE_USEITEM_ARMOR);
			//	종족 스탯 리스트 박스 초기화
			ListView_SetExtendedListViewStyle(hWndListRaceStat,LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); // 
			AddListviewCollum(hWndListRaceStat,0,28,"No");
			AddListviewCollum(hWndListRaceStat,1,90,"종족");
			AddListviewCollum(hWndListRaceStat,2,26,"힘");
			AddListviewCollum(hWndListRaceStat,3,26,"민첩");
			AddListviewCollum(hWndListRaceStat,4,26,"체력");
			AddListviewCollum(hWndListRaceStat,5,26,"지능");
			AddListviewCollum(hWndListRaceStat,6,26,"지혜");
			AddListviewCollum(hWndListRaceStat,7,26,"정신");
			AddListviewCollum(hWndListRaceStat,8,36,"합계");
			AddListviewCollum(hWndListRaceStat,9,53,"무기");
			AddListviewCollum(hWndListRaceStat,10,54,"방어구");
			AddListviewCollum(hWndListRaceStat,11,60,"이동속도");

			SendMessage(hWndListRaceUseItem, CB_ADDSTRING, 0, (LPARAM)"미사용");
			SendMessage(hWndListRaceUseItem, CB_ADDSTRING, 0, (LPARAM)"사용");
			SendMessage(hWndListRaceUseItemArmor, CB_ADDSTRING, 0, (LPARAM)"미사용");
			SendMessage(hWndListRaceUseItemArmor, CB_ADDSTRING, 0, (LPARAM)"사용");

			ListRebuild(hWndListRaceStat);
/*
			for(i=0 ; i<DATA_RACESTAT_MAX; i++)
			{
				sprintf(str, "%d", i);
				AddListviewData(hWndListRaceStat, i, 0, str);
				AddListviewData(hWndListRaceStat, i, 1, pBalance->GetRaceName(i));
				sprintf(str, "%d", pBalance->GetRaceStat(i)->GetStr());
				AddListviewData(hWndListRaceStat, i, 2, str);
				sprintf(str, "%d", pBalance->GetRaceStat(i)->GetDex());
				AddListviewData(hWndListRaceStat, i, 3, str);
				sprintf(str, "%d", pBalance->GetRaceStat(i)->GetCon());
				AddListviewData(hWndListRaceStat, i, 4, str);
				sprintf(str, "%d", pBalance->GetRaceStat(i)->GetInt());
				AddListviewData(hWndListRaceStat, i, 5, str);
				sprintf(str, "%d", pBalance->GetRaceStat(i)->GetWis());
				AddListviewData(hWndListRaceStat, i, 6, str);
				sprintf(str, "%d", pBalance->GetRaceStat(i)->GetMen());
				AddListviewData(hWndListRaceStat, i, 7, str);

				pBalance->GetRaceStat(i)->CalcTotal();
				sprintf(str, "%d", pBalance->GetRaceStat(i)->GetTotal());
				AddListviewData(hWndListRaceStat, i, 8, str);
			}
				sprintf(str, "%d", 6);
				AddListviewData(hWndListRaceStat, 6, 0, str);
				AddListviewData(hWndListRaceStat, 6, 1, "1234567890");
//*/
			for(i=0 ; i<DATA_CLASSSTAT_MAX ; i++)
			{
				SetDlgItemText(hDlg, IDC_STATIC_CLASS1+i, pBalance->GetClassStat(i)->GetName());
			}

			//	자료내용 보여주기
			ListView_SetItemState(hWndListRaceStat, iCurrentRaceList, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
			UpdatePage1(hDlg);
			break;
		case WM_NOTIFY:
			{
				LPNMHDR pnmh;

				if(wParam==IDC_LIST_RACESTAT)
				{
					pnmh=(LPNMHDR)lParam;
					switch(pnmh->code)
					{
						case LVN_ITEMCHANGED:
							linfo=(LPNM_LISTVIEW)lParam;
							iCurrentRaceList = linfo->iItem;
							UpdatePage1(hDlg);
							break;
						case LVN_BEGINLABELEDIT:
						case LVN_ENDLABELEDIT:
							break;
					}
				}
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_EDIT_STATPOINT:
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->SetStatPoint(iData);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_BASE_STR:
					iData = GetDlgItemInt(hDlg, IDC_EDIT_BASE_STR, NULL, TRUE);
					pBalance->GetBaseStat(0)->SetStr(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
				   break;
				case IDC_EDIT_BASE_DEX:
					iData = GetDlgItemInt(hDlg, IDC_EDIT_BASE_DEX, NULL, TRUE);
					pBalance->GetBaseStat(0)->SetDex(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_BASE_CON:
					iData = GetDlgItemInt(hDlg, IDC_EDIT_BASE_CON, NULL, TRUE);
					pBalance->GetBaseStat(0)->SetCon(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_BASE_INT:
					iData = GetDlgItemInt(hDlg, IDC_EDIT_BASE_INT, NULL, TRUE);
					pBalance->GetBaseStat(0)->SetInt(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_BASE_WIS:
					iData = GetDlgItemInt(hDlg, IDC_EDIT_BASE_WIS, NULL, TRUE);
					pBalance->GetBaseStat(0)->SetWis(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_BASE_MEN:
					iData = GetDlgItemInt(hDlg, IDC_EDIT_BASE_MEN, NULL, TRUE);
					pBalance->GetBaseStat(0)->SetMen(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_STR1:
				case IDC_EDIT_CLASS_STR2:
				case IDC_EDIT_CLASS_STR3:
				case IDC_EDIT_CLASS_STR4:
				case IDC_EDIT_CLASS_STR5:
				case IDC_EDIT_CLASS_STR6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_STR1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetStr(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_DEX1:
				case IDC_EDIT_CLASS_DEX2:
				case IDC_EDIT_CLASS_DEX3:
				case IDC_EDIT_CLASS_DEX4:
				case IDC_EDIT_CLASS_DEX5:
				case IDC_EDIT_CLASS_DEX6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_DEX1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetDex(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_CON1:
				case IDC_EDIT_CLASS_CON2:
				case IDC_EDIT_CLASS_CON3:
				case IDC_EDIT_CLASS_CON4:
				case IDC_EDIT_CLASS_CON5:
				case IDC_EDIT_CLASS_CON6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_CON1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetCon(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_INT1:
				case IDC_EDIT_CLASS_INT2:
				case IDC_EDIT_CLASS_INT3:
				case IDC_EDIT_CLASS_INT4:
				case IDC_EDIT_CLASS_INT5:
				case IDC_EDIT_CLASS_INT6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_INT1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetInt(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_WIS1:
				case IDC_EDIT_CLASS_WIS2:
				case IDC_EDIT_CLASS_WIS3:
				case IDC_EDIT_CLASS_WIS4:
				case IDC_EDIT_CLASS_WIS5:
				case IDC_EDIT_CLASS_WIS6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_WIS1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetWis(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_MEN1:
				case IDC_EDIT_CLASS_MEN2:
				case IDC_EDIT_CLASS_MEN3:
				case IDC_EDIT_CLASS_MEN4:
				case IDC_EDIT_CLASS_MEN5:
				case IDC_EDIT_CLASS_MEN6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_MEN1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetMen(iData);
					CalcPage1(hDlg);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_RANGE_MIN1:
				case IDC_EDIT_CLASS_RANGE_MIN2:
				case IDC_EDIT_CLASS_RANGE_MIN3:
				case IDC_EDIT_CLASS_RANGE_MIN4:
				case IDC_EDIT_CLASS_RANGE_MIN5:
				case IDC_EDIT_CLASS_RANGE_MIN6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_RANGE_MIN1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetRange(iData, 0);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_RANGE_MAX1:
				case IDC_EDIT_CLASS_RANGE_MAX2:
				case IDC_EDIT_CLASS_RANGE_MAX3:
				case IDC_EDIT_CLASS_RANGE_MAX4:
				case IDC_EDIT_CLASS_RANGE_MAX5:
				case IDC_EDIT_CLASS_RANGE_MAX6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_RANGE_MAX1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetRange(iData, 1);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_RRANGE_MIN1:
				case IDC_EDIT_CLASS_RRANGE_MIN2:
				case IDC_EDIT_CLASS_RRANGE_MIN3:
				case IDC_EDIT_CLASS_RRANGE_MIN4:
				case IDC_EDIT_CLASS_RRANGE_MIN5:
				case IDC_EDIT_CLASS_RRANGE_MIN6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_RRANGE_MIN1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetRange(iData, 2);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_RRANGE_MAX1:
				case IDC_EDIT_CLASS_RRANGE_MAX2:
				case IDC_EDIT_CLASS_RRANGE_MAX3:
				case IDC_EDIT_CLASS_RRANGE_MAX4:
				case IDC_EDIT_CLASS_RRANGE_MAX5:
				case IDC_EDIT_CLASS_RRANGE_MAX6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_RRANGE_MAX1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetRange(iData, 3);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;

				case IDC_EDIT_CLASS_MRANGE_MIN1:
				case IDC_EDIT_CLASS_MRANGE_MIN2:
				case IDC_EDIT_CLASS_MRANGE_MIN3:
				case IDC_EDIT_CLASS_MRANGE_MIN4:
				case IDC_EDIT_CLASS_MRANGE_MIN5:
				case IDC_EDIT_CLASS_MRANGE_MIN6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_MRANGE_MIN1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetRange(iData, 4);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_MRANGE_MAX1:
				case IDC_EDIT_CLASS_MRANGE_MAX2:
				case IDC_EDIT_CLASS_MRANGE_MAX3:
				case IDC_EDIT_CLASS_MRANGE_MAX4:
				case IDC_EDIT_CLASS_MRANGE_MAX5:
				case IDC_EDIT_CLASS_MRANGE_MAX6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_MRANGE_MAX1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetRange(iData, 5);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_MRRANGE_MIN1:
				case IDC_EDIT_CLASS_MRRANGE_MIN2:
				case IDC_EDIT_CLASS_MRRANGE_MIN3:
				case IDC_EDIT_CLASS_MRRANGE_MIN4:
				case IDC_EDIT_CLASS_MRRANGE_MIN5:
				case IDC_EDIT_CLASS_MRRANGE_MIN6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_MRRANGE_MIN1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetRange(iData, 6);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_CLASS_MRRANGE_MAX1:
				case IDC_EDIT_CLASS_MRRANGE_MAX2:
				case IDC_EDIT_CLASS_MRRANGE_MAX3:
				case IDC_EDIT_CLASS_MRRANGE_MAX4:
				case IDC_EDIT_CLASS_MRRANGE_MAX5:
				case IDC_EDIT_CLASS_MRRANGE_MAX6:
					iOffset = LOWORD(wParam) - IDC_EDIT_CLASS_MRRANGE_MAX1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetClassStat(iOffset)->SetRange(iData, 7);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_RACE_NAME:
					GetDlgItemText(hDlg, LOWORD(wParam), str, 21);
					pBalance->GetRaceStat(iCurrentRaceList)->SetName(str);
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 1, str);
					ReportPageAllInitDlg();
					break;
				case IDC_EDIT_RACE_STR:
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetRaceStat(iCurrentRaceList)->SetStr(iData);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetStr());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 2, str);

					pBalance->GetRaceStat(iCurrentRaceList)->CalcTotal();
					SetDlgItemInt(hDlg, IDC_EDIT_RACE_TOTAL, pBalance->GetRaceStat(iCurrentRaceList)->GetTotal(), FALSE);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetTotal());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 8, str);

					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_RACE_DEX:
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetRaceStat(iCurrentRaceList)->SetDex(iData);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetDex());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 3, str);

					pBalance->GetRaceStat(iCurrentRaceList)->CalcTotal();
					SetDlgItemInt(hDlg, IDC_EDIT_RACE_TOTAL, pBalance->GetRaceStat(iCurrentRaceList)->GetTotal(), FALSE);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetTotal());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 8, str);

					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_RACE_CON:
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetRaceStat(iCurrentRaceList)->SetCon(iData);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetCon());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 4, str);

					pBalance->GetRaceStat(iCurrentRaceList)->CalcTotal();
					SetDlgItemInt(hDlg, IDC_EDIT_RACE_TOTAL, pBalance->GetRaceStat(iCurrentRaceList)->GetTotal(), FALSE);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetTotal());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 8, str);

					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_RACE_INT:
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetRaceStat(iCurrentRaceList)->SetInt(iData);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetInt());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 5, str);

					pBalance->GetRaceStat(iCurrentRaceList)->CalcTotal();
					SetDlgItemInt(hDlg, IDC_EDIT_RACE_TOTAL, pBalance->GetRaceStat(iCurrentRaceList)->GetTotal(), FALSE);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetTotal());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 8, str);

					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_RACE_WIS:
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetRaceStat(iCurrentRaceList)->SetWis(iData);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetWis());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 6, str);

					pBalance->GetRaceStat(iCurrentRaceList)->CalcTotal();
					SetDlgItemInt(hDlg, IDC_EDIT_RACE_TOTAL, pBalance->GetRaceStat(iCurrentRaceList)->GetTotal(), FALSE);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetTotal());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 8, str);

					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_RACE_MEN:
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetRaceStat(iCurrentRaceList)->SetMen(iData);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetMen());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 7, str);

					pBalance->GetRaceStat(iCurrentRaceList)->CalcTotal();
					SetDlgItemInt(hDlg, IDC_EDIT_RACE_TOTAL, pBalance->GetRaceStat(iCurrentRaceList)->GetTotal(), FALSE);
					sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetTotal());
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 8, str);

					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_BUTTON_RACE_ADD:
					sprintf(str, "'%s' 다음에 새로 추가합니다.", pBalance->GetRaceStat(iCurrentRaceList)->GetName());
					if(MessageBox(hDlg, str, "종족 추가", MB_YESNO)==IDNO) break;

					ListView_DeleteAllItems(hWndListRaceStat);
					pBalance->InsertRace(iCurrentRaceList);
					ListRebuild(hWndListRaceStat);
					ListView_SetItemState(hWndListRaceStat, iCurrentRaceList, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
					ListView_Scroll(hWndListRaceStat, 0, int(iCurrentRaceList-ListView_GetCountPerPage(hWndListRaceStat)/2)*13);
					IsSaved=FALSE;
					ReportPageAllInitDlg();
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_BUTTON_RACE_DELETE:
					sprintf(str, "'%s' 종족을 삭제합니다.", pBalance->GetRaceStat(iCurrentRaceList)->GetName());
					if(MessageBox(hDlg, str, "종족 삭제", MB_YESNO)==IDNO) break;
					ListView_DeleteAllItems(hWndListRaceStat);
					pBalance->DeleteRace(iCurrentRaceList);
					ListRebuild(hWndListRaceStat);
					ListView_SetItemState(hWndListRaceStat, iCurrentRaceList, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
					ListView_Scroll(hWndListRaceStat, 0, int(iCurrentRaceList-ListView_GetCountPerPage(hWndListRaceStat)/2)*13);
					IsSaved=FALSE;
					ReportPageAllInitDlg();
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_COMBO_RACE_USEITEM:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndListRaceUseItem, CB_GETCURSEL, 0,0);
						pBalance->GetRaceStat(iCurrentRaceList)->SetUseItem(char(iData));
						if(pBalance->GetRaceStat(iCurrentRaceList)->GetUseItem()==1)
							AddListviewData(hWndListRaceStat, iCurrentRaceList, 9, "사용");
						else
							AddListviewData(hWndListRaceStat, iCurrentRaceList, 9, "미사용");

						IsSaved=FALSE;
						SendMessage( hDlg, WM_PAINT, 0, 0 );
					}
					break;
				case IDC_COMBO_RACE_USEITEM_ARMOR:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndListRaceUseItemArmor, CB_GETCURSEL, 0,0);
						pBalance->GetRaceStat(iCurrentRaceList)->SetUseItemArmor(char(iData));
						if(pBalance->GetRaceStat(iCurrentRaceList)->GetUseItemArmor()==1)
							AddListviewData(hWndListRaceStat, iCurrentRaceList, 10, "사용");
						else
							AddListviewData(hWndListRaceStat, iCurrentRaceList, 10, "미사용");

						IsSaved=FALSE;
						SendMessage( hDlg, WM_PAINT, 0, 0 );
					}
					break;
				case IDC_EDIT_RACE_SPEED:
					GetDlgItemText(hDlg, LOWORD(wParam), str, 21);
					fData = atof(str);

					pBalance->GetRaceStat(iCurrentRaceList)->SetSpeed(fData);
					AddListviewData(hWndListRaceStat, iCurrentRaceList, 11, str);

					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
			}
			break;
		case WM_KEYDOWN:
            break;
		case WM_PAINT :
			if(IsSaveView) 
				if(!IsSaved) 
				{
					SendMessage( hWndEdit, WM_PAINT, 0, 0 );
					IsSaveView=FALSE;
				}
			break;
		case WM_USER:
			//	화면에 데이터 재계산하여 보여주기
				UpdatePage1(hDlg);
				CalcPage1(hDlg);
				SendMessage( hDlg, WM_PAINT, 0, 0 );
			break;
	}
    return FALSE;
}

BOOL CALLBACK Page2Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int iData;
	int iOffset;
	int i;
	double dData;
	char str[_MAX_FNAME];

	switch (message)
	{
		case WM_INITDIALOG:
			hWndPage2 = hDlg;

			for(i=0 ; i<12 ; i++)
			{
				SetDlgItemText(hDlg, IDC_STATIC_TEXT1+i, pBalance->GetFormula(i)->GetName());
			}

			UpdatePage2(hDlg);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_EDIT_INIT1:
				case IDC_EDIT_INIT2:
				case IDC_EDIT_INIT3:
				case IDC_EDIT_INIT4:
				case IDC_EDIT_INIT5:
				case IDC_EDIT_INIT6:
				case IDC_EDIT_INIT7:
				case IDC_EDIT_INIT8:
				case IDC_EDIT_INIT9:
				case IDC_EDIT_INIT10:
				case IDC_EDIT_INIT11:
				case IDC_EDIT_INIT12:
					iOffset = LOWORD(wParam) - IDC_EDIT_INIT1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetFormula(iOffset)->SetInit(iData);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_X1:
				case IDC_EDIT_X2:
				case IDC_EDIT_X3:
				case IDC_EDIT_X4:
				case IDC_EDIT_X5:
				case IDC_EDIT_X6:
				case IDC_EDIT_X7:
				case IDC_EDIT_X8:
				case IDC_EDIT_X9:
				case IDC_EDIT_X10:
				case IDC_EDIT_X11:
				case IDC_EDIT_X12:
					iOffset = LOWORD(wParam) - IDC_EDIT_X1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					GetDlgItemText(hDlg, LOWORD(wParam), str, 21);
					dData = (double)atof(str);
					pBalance->GetFormula(iOffset)->SetX(dData);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_Y1:
				case IDC_EDIT_Y2:
				case IDC_EDIT_Y3:
				case IDC_EDIT_Y4:
				case IDC_EDIT_Y5:
				case IDC_EDIT_Y6:
				case IDC_EDIT_Y7:
				case IDC_EDIT_Y8:
				case IDC_EDIT_Y9:
				case IDC_EDIT_Y10:
				case IDC_EDIT_Y11:
				case IDC_EDIT_Y12:
					iOffset = LOWORD(wParam) - IDC_EDIT_Y1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					GetDlgItemText(hDlg, LOWORD(wParam), str, 21);
					dData = (double)atof(str);
					pBalance->GetFormula(iOffset)->SetY(dData);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
			}
			break;
		case WM_PAINT :
			if(IsSaveView) 
				if(!IsSaved) 
				{
					SendMessage( hWndEdit, WM_PAINT, 0, 0 );
					IsSaveView=FALSE;
				}
			break;
	}
    return FALSE;
}

BOOL CALLBACK Page3Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int iData;
	int iOffset, iOffset2=12;
	int i;
	double dData;
	char str[_MAX_FNAME];

	switch (message)
	{
		case WM_INITDIALOG:
			hWndPage3 = hDlg;

			for(i=0 ; i<11 ; i++)
			{
				SetDlgItemText(hDlg, IDC_STATIC_TEXT1+i, pBalance->GetFormula(i+iOffset2)->GetName());
			}

			UpdatePage3(hDlg);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_EDIT_INIT1:
				case IDC_EDIT_INIT2:
				case IDC_EDIT_INIT3:
				case IDC_EDIT_INIT4:
				case IDC_EDIT_INIT5:
				case IDC_EDIT_INIT6:
				case IDC_EDIT_INIT7:
				case IDC_EDIT_INIT8:
				case IDC_EDIT_INIT9:
				case IDC_EDIT_INIT10:
				case IDC_EDIT_INIT11:
					iOffset = LOWORD(wParam) - IDC_EDIT_INIT1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					pBalance->GetFormula(iOffset+iOffset2)->SetInit(iData);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_X1:
				case IDC_EDIT_X2:
				case IDC_EDIT_X3:
				case IDC_EDIT_X4:
				case IDC_EDIT_X5:
				case IDC_EDIT_X6:
				case IDC_EDIT_X7:
				case IDC_EDIT_X8:
				case IDC_EDIT_X9:
				case IDC_EDIT_X10:
				case IDC_EDIT_X11:
					iOffset = LOWORD(wParam) - IDC_EDIT_X1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					GetDlgItemText(hDlg, LOWORD(wParam), str, 21);
					dData = (double)atof(str);
					pBalance->GetFormula(iOffset+iOffset2)->SetX(dData);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
				case IDC_EDIT_Y1:
				case IDC_EDIT_Y2:
				case IDC_EDIT_Y3:
				case IDC_EDIT_Y4:
				case IDC_EDIT_Y5:
				case IDC_EDIT_Y6:
				case IDC_EDIT_Y7:
				case IDC_EDIT_Y8:
				case IDC_EDIT_Y9:
				case IDC_EDIT_Y10:
				case IDC_EDIT_Y11:
					iOffset = LOWORD(wParam) - IDC_EDIT_Y1;
					iData = GetDlgItemInt(hDlg, LOWORD(wParam), NULL, TRUE);
					GetDlgItemText(hDlg, LOWORD(wParam), str, 21);
					dData = (double)atof(str);
					pBalance->GetFormula(iOffset+iOffset2)->SetY(dData);
					IsSaved=FALSE;
					SendMessage( hDlg, WM_PAINT, 0, 0 );
					break;
			}
			break;
		case WM_PAINT :
			if(IsSaveView) 
				if(!IsSaved) 
				{
					SendMessage( hWndEdit, WM_PAINT, 0, 0 );
					IsSaveView=FALSE;
				}
			break;
	}
    return FALSE;
}

BOOL CALLBACK Page4Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
			hWndPage4 = hDlg;

	InitCommonControls();

			SetTextoutEditHwnd(hDlg, IDC_EDIT_RESULT);
			RenderText();
//			UpdatePage4(hDlg);
//			SendMessage( GetTextoutEditHwnd(), EM_SETREADONLY, (WPARAM)TRUE, 0 );
			
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_EDIT_RESULT:
				break;
			}
			break;
		case WM_PAINT :
			if(IsSaveView) 
				if(!IsSaved) 
				{
					SendMessage( hWndEdit, WM_PAINT, 0, 0 );
					IsSaveView=FALSE;
				}
			break;
	}
    return FALSE;
}

void ListDelete(HWND hWnd)
{
	int i;
	for(i=0 ; i<pBalance->GetRaceMax(); i++)
	{
		ListView_DeleteItem(hWnd, 1);
	}
}

void ListRebuild(HWND hWnd)
{
	char str[_MAX_FNAME];
	int i;
	for(i=0 ; i<pBalance->GetRaceMax(); i++)
	{
		sprintf(str, "%d", i);
		AddListviewData(hWnd, i, 0, str);
		AddListviewData(hWnd, i, 1, pBalance->GetRaceStat(i)->GetName());
		sprintf(str, "%d", pBalance->GetRaceStat(i)->GetStr());
		AddListviewData(hWnd, i, 2, str);
		sprintf(str, "%d", pBalance->GetRaceStat(i)->GetDex());
		AddListviewData(hWnd, i, 3, str);
		sprintf(str, "%d", pBalance->GetRaceStat(i)->GetCon());
		AddListviewData(hWnd, i, 4, str);
		sprintf(str, "%d", pBalance->GetRaceStat(i)->GetInt());
		AddListviewData(hWnd, i, 5, str);
		sprintf(str, "%d", pBalance->GetRaceStat(i)->GetWis());
		AddListviewData(hWnd, i, 6, str);
		sprintf(str, "%d", pBalance->GetRaceStat(i)->GetMen());
		AddListviewData(hWnd, i, 7, str);

		pBalance->GetRaceStat(i)->CalcTotal();
		sprintf(str, "%d", pBalance->GetRaceStat(i)->GetTotal());
		AddListviewData(hWnd, i, 8, str);

		if(pBalance->GetRaceStat(i)->GetUseItem()==1)
			AddListviewData(hWnd, i, 9, "사용");
		else
			AddListviewData(hWnd, i, 9, "미사용");
		if(pBalance->GetRaceStat(i)->GetUseItemArmor()==1)
			AddListviewData(hWnd, i, 10, "사용");
		else
			AddListviewData(hWnd, i, 10, "미사용");
		sprintf(str, "%.2f", pBalance->GetRaceStat(i)->GetSpeed());
		AddListviewData(hWnd, i, 11, str);
	}
}

void UpdatePage1(HWND hWnd)
{
	int i;
	char str[_MAX_FNAME];

	//	Set Data
	//	레벨업 추가 스탯 포인트
	SetDlgItemInt(hWnd, IDC_EDIT_STATPOINT, pBalance->GetStatPoint(), FALSE);

	//	기본
	SetDlgItemInt(hWnd, IDC_EDIT_BASE_STR, pBalance->GetBaseStat(0)->GetStr(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_BASE_DEX, pBalance->GetBaseStat(0)->GetDex(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_BASE_CON, pBalance->GetBaseStat(0)->GetCon(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_BASE_INT, pBalance->GetBaseStat(0)->GetInt(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_BASE_WIS, pBalance->GetBaseStat(0)->GetWis(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_BASE_MEN, pBalance->GetBaseStat(0)->GetMen(), FALSE);

	//	직업별
	for(i=0 ; i<DATA_CLASSSTAT_MAX ; i++)
	{
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_STR1+i, pBalance->GetClassStat(i)->GetStr(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_DEX1+i, pBalance->GetClassStat(i)->GetDex(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_CON1+i, pBalance->GetClassStat(i)->GetCon(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_INT1+i, pBalance->GetClassStat(i)->GetInt(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_WIS1+i, pBalance->GetClassStat(i)->GetWis(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_MEN1+i, pBalance->GetClassStat(i)->GetMen(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_RANGE_MIN1+i, pBalance->GetClassStat(i)->GetRange(0), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_RANGE_MAX1+i, pBalance->GetClassStat(i)->GetRange(1), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_RRANGE_MIN1+i, pBalance->GetClassStat(i)->GetRange(2), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_RRANGE_MAX1+i, pBalance->GetClassStat(i)->GetRange(3), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_MRANGE_MIN1+i, pBalance->GetClassStat(i)->GetRange(4), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_MRANGE_MAX1+i, pBalance->GetClassStat(i)->GetRange(5), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_MRRANGE_MIN1+i, pBalance->GetClassStat(i)->GetRange(6), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_MRRANGE_MAX1+i, pBalance->GetClassStat(i)->GetRange(7), FALSE);
	}

	//	종족별
	for(i=0 ; i<DATA_RACESTAT_MAX ; i++)
	{
		SetDlgItemInt(hWnd, IDC_EDIT_RACE_STR1+i, pBalance->GetRaceStat(i)->GetStr(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_RACE_DEX1+i, pBalance->GetRaceStat(i)->GetDex(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_RACE_CON1+i, pBalance->GetRaceStat(i)->GetCon(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_RACE_INT1+i, pBalance->GetRaceStat(i)->GetInt(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_RACE_WIS1+i, pBalance->GetRaceStat(i)->GetWis(), FALSE);
		SetDlgItemInt(hWnd, IDC_EDIT_RACE_MEN1+i, pBalance->GetRaceStat(i)->GetMen(), FALSE);
	}

	SetDlgItemText(hWnd, IDC_EDIT_RACE_NAME, pBalance->GetRaceStat(iCurrentRaceList)->GetName());

	SetDlgItemInt(hWnd, IDC_EDIT_RACE_STR, pBalance->GetRaceStat(iCurrentRaceList)->GetStr(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_RACE_DEX, pBalance->GetRaceStat(iCurrentRaceList)->GetDex(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_RACE_CON, pBalance->GetRaceStat(iCurrentRaceList)->GetCon(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_RACE_INT, pBalance->GetRaceStat(iCurrentRaceList)->GetInt(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_RACE_WIS, pBalance->GetRaceStat(iCurrentRaceList)->GetWis(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_RACE_MEN, pBalance->GetRaceStat(iCurrentRaceList)->GetMen(), FALSE);
	SetDlgItemInt(hWnd, IDC_EDIT_RACE_TOTAL, pBalance->GetRaceStat(iCurrentRaceList)->GetTotal(), FALSE);

	SendMessage(hWndListRaceUseItem, CB_SETCURSEL, (WPARAM)pBalance->GetRaceStat(iCurrentRaceList)->GetUseItem(), 0);
	SendMessage(hWndListRaceUseItemArmor, CB_SETCURSEL, (WPARAM)pBalance->GetRaceStat(iCurrentRaceList)->GetUseItemArmor(), 0);

	sprintf(str, "%.2f", pBalance->GetRaceStat(iCurrentRaceList)->GetSpeed());
	SetDlgItemText(hWnd, IDC_EDIT_RACE_SPEED, str);
	
	/*
	sprintf(str, "%d", iCurrentRaceList);
	AddListviewData(hWndListRaceStat, iCurrentRaceList, 0, str);
	AddListviewData(hWndListRaceStat, iCurrentRaceList, 1, pBalance->GetRaceName(i));

	sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetStr());
	AddListviewData(hWndListRaceStat, iCurrentRaceList, 2, str);
	sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetDex());
	AddListviewData(hWndListRaceStat, iCurrentRaceList, 3, str);
	sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetCon());
	AddListviewData(hWndListRaceStat, iCurrentRaceList, 4, str);
	sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetInt());
	AddListviewData(hWndListRaceStat, iCurrentRaceList, 5, str);
	sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetWis());
	AddListviewData(hWndListRaceStat, iCurrentRaceList, 6, str);
	sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetMen());
	AddListviewData(hWndListRaceStat, iCurrentRaceList, 7, str);

	pBalance->GetRaceStat(iCurrentRaceList)->CalcTotal();
	sprintf(str, "%d", pBalance->GetRaceStat(iCurrentRaceList)->GetTotal());
	AddListviewData(hWndListRaceStat, iCurrentRaceList, 8, str);
*/
}

void UpdatePage2(HWND hWnd)
{
	int i;
	char str[_MAX_FNAME];

	//	Set Data
	//	레벨업 추가 스탯 포인트
	for(i=0 ; i<12 ; i++)
	{
		//	물리
		SetDlgItemInt(hWnd, IDC_EDIT_INIT1+i, pBalance->GetFormula(i)->GetInit(), FALSE);
		sprintf(str, "%.3f", pBalance->GetFormula(i)->GetX());
		SetDlgItemText(hWnd, IDC_EDIT_X1+i, str);
		sprintf(str, "%.3f", pBalance->GetFormula(i)->GetY());
		SetDlgItemText(hWnd, IDC_EDIT_Y1+i, str);
	}
}

void UpdatePage3(HWND hWnd)
{
	int i;
	char str[_MAX_FNAME];

	int iOffset = 12;

	//	Set Data
	//	레벨업 추가 스탯 포인트
	for(i=0 ; i<11 ; i++)
	{
		//	마법
		SetDlgItemInt(hWnd, IDC_EDIT_INIT1+i, pBalance->GetFormula(i+iOffset)->GetInit(), FALSE);
		sprintf(str, "%.3f", pBalance->GetFormula(i+iOffset)->GetX());
		SetDlgItemText(hWnd, IDC_EDIT_X1+i, str);
		sprintf(str, "%.3f", pBalance->GetFormula(i+iOffset)->GetY());
		SetDlgItemText(hWnd, IDC_EDIT_Y1+i, str);
	}
}

void CalcPage1(HWND hWnd)
{
	int i;

	//	Set Data - 합계 계산해서 보여주기
	//	기본
	pBalance->GetBaseStat(0)->CalcTotal();
	SetDlgItemInt(hWnd, IDC_EDIT_BASE_TOTAL, 
		 pBalance->GetBaseStat(0)->GetTotal()
		, FALSE);

	//	직업별
	for(i=0 ; i<DATA_CLASSSTAT_MAX ; i++)
	{
		pBalance->GetClassStat(i)->CalcTotal();
		SetDlgItemInt(hWnd, IDC_EDIT_CLASS_TOTAL1+i, 
			 pBalance->GetClassStat(i)->GetTotal()
			, FALSE);
	}

	//	종족별
	for(i=0 ; i<DATA_RACESTAT_MAX ; i++)
	{
		pBalance->GetRaceStat(i)->CalcTotal();
		SetDlgItemInt(hWnd, IDC_EDIT_RACE_TOTAL1+i, 
			 pBalance->GetRaceStat(i)->GetTotal()
			, FALSE);
	}
}

BOOL CALLBACK ReportPage1Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	int i;
	char szStr[_MAX_FNAME];
	int iData;

	switch (message)
	{
		case WM_INITDIALOG:
			hWndReportPage1 = hDlg;
			hWndReport1LevelCombo = GetDlgItem(hDlg, IDC_COMBO_LEVELSTAT);
			hWndReport1ComboRace = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE);
			hWndReport1ComboRace2 = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE2);
			hWndReport1ComboClass = GetDlgItem(hDlg, IDC_COMBO_COMBAT_CLASS);

			ReportPage1InitDlg();
			/*
			for(i=0 ; i<DATA_RACESTAT_MAX+1 ; i++)
			{
				SendMessage(hWndReport1ComboRace, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));
			}

			for(i=0 ; i<giTCLASSCHART_COUNT ; i++)
			{
				SendMessage(hWndReport1ComboClass, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassStat(i)->GetName());
			}

			for(i=0 ; i<pBalance->GetClassExCount() ; i++)
			{
				SendMessage(hWndReport1ComboClass, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
			}
			*/
			for(i=1 ; i<_BAL_LEVELMAX+1 ; i++)
			{
				sprintf(szStr, "%d", i);
				SendMessage(hWndReport1LevelCombo, CB_ADDSTRING, 0, (LPARAM)szStr);
			}

			UpdateReportPage1(hDlg);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_BUTTON_REPORT_STAT:
					OnSelChanged(hWndEdit, 3);
					BaseStat(ReportPath);
					break;
				case IDC_COMBO_LEVELSTAT:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport1LevelCombo, CB_GETCURSEL, 0,0);
						gsLevelStat.mLevel = iData+1;
					}
					if(HIWORD(wParam)==CBN_EDITCHANGE)
					{
						GetDlgItemText(hDlg, LOWORD(wParam), szStr, 21);
						iData = atoi(szStr);
						if(iData<1)
						{
							iData=1;
							gsLevelStat.mLevel = iData;
							UpdateReportPage1(hDlg);
						}
						if(iData>_BAL_LEVELMAX)
						{
							iData=_BAL_LEVELMAX;
							gsLevelStat.mLevel = iData;
							UpdateReportPage1(hDlg);
						}
						gsLevelStat.mLevel = iData;
					}
					break;
				case IDC_COMBO_COMBAT_RACE:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport1ComboRace, CB_GETCURSEL, 0,0);
						gsLevelStat.mRace = iData;
						gsLevelStat.mRace2 = iData;
						UpdateReportPage1(hDlg);
					}
					break;
				case IDC_COMBO_COMBAT_RACE2:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport1ComboRace2, CB_GETCURSEL, 0,0);
						gsLevelStat.mRace2 = iData;
						if(gsLevelStat.mRace2<gsLevelStat.mRace)
						{
							gsLevelStat.mRace2=gsLevelStat.mRace;
							UpdateReportPage1(hDlg);
						}
					}
					break;
				case IDC_COMBO_COMBAT_CLASS:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport1ComboClass, CB_GETCURSEL, 0,0);
						gsLevelStat.mClass = iData;
					}
					break;
				case IDC_BUTTON_LEVELSTAT:
					OnSelChanged(hWndEdit, 3);
					LevelStat(ReportPath);
					break;
				case IDC_BUTTON_STATALLLEVEL:
					OnSelChanged(hWndEdit, 3);
					AllLevelStat(ReportPath);
					break;
			}
			break;
		case WM_PAINT :
			break;
	}
    return FALSE;
}

BOOL CALLBACK ReportPage2Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	char szStr[_MAX_FNAME];
	int i;
	int iData;

	switch (message)
	{
		case WM_INITDIALOG:
			hWndReportPage2 = hDlg;

			hWndReport2ComboRaceAt = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE_AT);
			hWndReport2ComboClassAt = GetDlgItem(hDlg, IDC_COMBO_COMBAT_CLASS_AT);
			hWndReport2ComboLevelAt = GetDlgItem(hDlg, IDC_COMBO_COMBAT_LEVEL_AT);

			hWndReport2ComboRaceDf = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE_DF);
			hWndReport2ComboClassDf = GetDlgItem(hDlg, IDC_COMBO_COMBAT_CLASS_DF);
			hWndReport2ComboLevelDf = GetDlgItem(hDlg, IDC_COMBO_COMBAT_LEVEL_DF);

			ReportPage2InitDlg();
			/*
			for(i=0 ; i<DATA_RACESTAT_MAX ; i++)
			{
				SendMessage(hWndReport2ComboRaceAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));
				SendMessage(hWndReport2ComboRaceDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));
			}

			for(i=0 ; i<DATA_CLASSSTAT_MAX ; i++)
			{
				SendMessage(hWndReport2ComboClassAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
				SendMessage(hWndReport2ComboClassDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
			}
*/
			for(i=1 ; i<_BAL_LEVELMAX+1 ; i++)
			{
				sprintf(szStr, "%d", i);
				SendMessage(hWndReport2ComboLevelAt, CB_ADDSTRING, 0, (LPARAM)szStr);
				SendMessage(hWndReport2ComboLevelDf, CB_ADDSTRING, 0, (LPARAM)szStr);
			}
			UpdateReportPage2(hDlg);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_COMBO_COMBAT_LEVEL_AT:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport2ComboLevelAt, CB_GETCURSEL, 0,0);
						gsLevelStat.mLevelAt = iData+1;
					}
					if(HIWORD(wParam)==CBN_EDITCHANGE)
					{
						GetDlgItemText(hDlg, LOWORD(wParam), szStr, 21);
						iData = atoi(szStr);
						if(iData<1)
						{
							iData=1;
							gsLevelStat.mLevelAt = iData;
							UpdateReportPage2(hDlg);
						}
						if(iData>_BAL_LEVELMAX)
						{
							iData=_BAL_LEVELMAX;
							gsLevelStat.mLevelAt = iData;
							UpdateReportPage2(hDlg);
						}
						gsLevelStat.mLevelAt = iData;
					}
					break;
				case IDC_COMBO_COMBAT_RACE_AT:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport2ComboRaceAt, CB_GETCURSEL, 0,0);
						gsLevelStat.mRaceAt = iData;
					}
					break;
				case IDC_COMBO_COMBAT_CLASS_AT:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport2ComboClassAt, CB_GETCURSEL, 0,0);
						gsLevelStat.mClassAt = iData;
					}
					break;
				case IDC_COMBO_COMBAT_LEVEL_DF:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport2ComboLevelDf, CB_GETCURSEL, 0,0);
						gsLevelStat.mLevelDf = iData+1;
					}
					if(HIWORD(wParam)==CBN_EDITCHANGE)
					{
						GetDlgItemText(hDlg, LOWORD(wParam), szStr, 21);
						iData = atoi(szStr);
						if(iData<1)
						{
							iData=1;
							gsLevelStat.mLevelDf = iData;
							UpdateReportPage2(hDlg);
						}
						if(iData>_BAL_LEVELMAX)
						{
							iData=_BAL_LEVELMAX;
							gsLevelStat.mLevelDf = iData;
							UpdateReportPage2(hDlg);
						}
						gsLevelStat.mLevelDf = iData;
					}
					break;
				case IDC_COMBO_COMBAT_RACE_DF:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport2ComboRaceDf, CB_GETCURSEL, 0,0);
						gsLevelStat.mRaceDf = iData;
					}
					break;
				case IDC_COMBO_COMBAT_CLASS_DF:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport2ComboClassDf, CB_GETCURSEL, 0,0);
						gsLevelStat.mClassDf = iData;
					}
					break;
				case IDC_BUTTON_REPORT2_1:
					OnSelChanged(hWndEdit, 3);
					Combat1(ReportPath);
					break;
//				case IDC_BUTTON_REPORT_LEVELEXP:
//					ExpLevel(ReportPath);
//					break;
			}
			break;
		case WM_PAINT :
			break;
	}
    return FALSE;
}

BOOL CALLBACK ReportPage3Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
//	int i;
	int iData;

	switch (message)
	{
		case WM_INITDIALOG:
			hWndReportPage3 = hDlg;

			hWndReport3ComboRaceAt = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE_AT);
			hWndReport3ComboRaceAt2 = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE_AT2);
			hWndReport3ComboClassAt = GetDlgItem(hDlg, IDC_COMBO_COMBAT_CLASS_AT);

			hWndReport3ComboRaceDf = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE_DF);
			hWndReport3ComboRaceDf2 = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE_DF2);
			hWndReport3ComboClassDf = GetDlgItem(hDlg, IDC_COMBO_COMBAT_CLASS_DF);

			hWndReport3ProgressBar = GetDlgItem(hDlg, IDC_PROGRESS1);

			ReportPage3InitDlg();
			/*
			for(i=0 ; i<DATA_RACESTAT_MAX ; i++)
			{
				SendMessage(hWndReport3ComboRaceAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));
				SendMessage(hWndReport3ComboRaceDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));
			}
			SendMessage(hWndReport3ComboRaceAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(DATA_RACESTAT_MAX));
//			SendMessage(hWndReport3ComboRaceDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));

			for(i=0 ; i<DATA_CLASSSTAT_MAX+1 ; i++)
			{
				SendMessage(hWndReport3ComboClassAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
				SendMessage(hWndReport3ComboClassDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
			}
*/
			SendMessage(hWndReport3ProgressBar, PBM_SETPOS, (WPARAM)0, 0);
			
			
			UpdateReportPage3(hDlg);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_COMBO_COMBAT_RACE_AT:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport3ComboRaceAt, CB_GETCURSEL, 0,0);
						gsLevelStat.mExpRaceAt = iData;
						gsLevelStat.mExpRaceAt2 = iData;
						UpdateReportPage3(hDlg);
					}
					break;
				case IDC_COMBO_COMBAT_RACE_AT2:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport3ComboRaceAt2, CB_GETCURSEL, 0,0);
						gsLevelStat.mExpRaceAt2 = iData;
						if(gsLevelStat.mExpRaceAt2<gsLevelStat.mExpRaceAt)
						{
							gsLevelStat.mExpRaceAt2=gsLevelStat.mExpRaceAt;
							UpdateReportPage3(hDlg);
						}
					}
					break;
				case IDC_COMBO_COMBAT_CLASS_AT:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport3ComboClassAt, CB_GETCURSEL, 0,0);
						gsLevelStat.mExpClassAt = iData;
					}
					break;
				case IDC_COMBO_COMBAT_RACE_DF:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport3ComboRaceDf, CB_GETCURSEL, 0,0);
						gsLevelStat.mExpRaceDf = iData;
						gsLevelStat.mExpRaceDf2 = iData;
						UpdateReportPage3(hDlg);
					}
					break;
				case IDC_COMBO_COMBAT_RACE_DF2:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport3ComboRaceDf2, CB_GETCURSEL, 0,0);
						gsLevelStat.mExpRaceDf2 = iData;
						if(gsLevelStat.mExpRaceDf2<gsLevelStat.mExpRaceDf)
						{
							gsLevelStat.mExpRaceDf2=gsLevelStat.mExpRaceDf;
							UpdateReportPage3(hDlg);
						}
					}
					break;
				case IDC_COMBO_COMBAT_CLASS_DF:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport3ComboClassDf, CB_GETCURSEL, 0,0);
						gsLevelStat.mExpClassDf = iData;
					}
					break;
				case IDC_BUTTON_REPORT_LEVELEXP:
					OnSelChanged(hWndEdit, 3);
					ExpLevel(hWndReport3ProgressBar, ReportPath);
					SendMessage(hWndReport3ProgressBar, PBM_SETPOS, (WPARAM)0, 0);
					break;
			}
			break;
		case WM_PAINT :
			break;
	}
    return FALSE;
}

BOOL CALLBACK ReportPage4Proc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
//	int i;
	int iData;

	switch (message)
	{
		case WM_INITDIALOG:
			hWndReportPage4 = hDlg;

			hWndReport4ComboRaceAt = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE_AT);
			hWndReport4ComboRaceAt2 = GetDlgItem(hDlg, IDC_COMBO_COMBAT_RACE_AT2);
			hWndReport4ComboClassAt = GetDlgItem(hDlg, IDC_COMBO_COMBAT_CLASS_AT);

			hWndReport4ComboClassDf = GetDlgItem(hDlg, IDC_COMBO_COMBAT_CLASS_DF);

			hWndReport4ProgressBar = GetDlgItem(hDlg, IDC_PROGRESS1);

			ReportPage4InitDlg();
			/*
			for(i=0 ; i<DATA_RACESTAT_MAX ; i++)
			{
				SendMessage(hWndReport3ComboRaceAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));
				SendMessage(hWndReport3ComboRaceDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));
			}
			SendMessage(hWndReport3ComboRaceAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(DATA_RACESTAT_MAX));
//			SendMessage(hWndReport3ComboRaceDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));

			for(i=0 ; i<DATA_CLASSSTAT_MAX+1 ; i++)
			{
				SendMessage(hWndReport3ComboClassAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
				SendMessage(hWndReport3ComboClassDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
			}
*/
			SendMessage(hWndReport4ProgressBar, PBM_SETPOS, (WPARAM)0, 0);
			
			
			UpdateReportPage4(hDlg);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_COMBO_COMBAT_RACE_AT:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport4ComboRaceAt, CB_GETCURSEL, 0,0);
						gsLevelStat.mAllExpRaceAt = iData;
						gsLevelStat.mAllExpRaceAt2 = iData;
						UpdateReportPage4(hDlg);
					}
					break;
				case IDC_COMBO_COMBAT_RACE_AT2:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport4ComboRaceAt2, CB_GETCURSEL, 0,0);
						gsLevelStat.mAllExpRaceAt2 = iData;
						if(gsLevelStat.mAllExpRaceAt2<gsLevelStat.mAllExpRaceAt)
						{
							gsLevelStat.mAllExpRaceAt2=gsLevelStat.mAllExpRaceAt;
							UpdateReportPage4(hDlg);
						}
					}
					break;
				case IDC_COMBO_COMBAT_CLASS_AT:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport4ComboClassAt, CB_GETCURSEL, 0,0);
						gsLevelStat.mAllExpClassAt = iData;
					}
					break;
				case IDC_COMBO_COMBAT_CLASS_DF:
					if(HIWORD(wParam)==CBN_SELCHANGE)
					{
						iData=(int)SendMessage(hWndReport4ComboClassDf, CB_GETCURSEL, 0,0);
						gsLevelStat.mAllExpClassDf = iData;
					}
					break;
				case IDC_BUTTON_REPORT_LEVELEXP:
					OnSelChanged(hWndEdit, 3);
					AllExpLevel(hWndReport4ProgressBar, ReportPath);
					SendMessage(hWndReport4ProgressBar, PBM_SETPOS, (WPARAM)0, 0);
					break;
			}
			break;
		case WM_PAINT :
			break;
	}
    return FALSE;
}

void UpdateReportPage1(HWND hWnd)
{
	//	Set Data
	SendMessage(hWndReport1LevelCombo, CB_SETCURSEL, (WPARAM)gsLevelStat.mLevel-1, 0);
	SendMessage(hWndReport1ComboRace, CB_SETCURSEL, (WPARAM)gsLevelStat.mRace, 0);
	SendMessage(hWndReport1ComboRace2, CB_SETCURSEL, (WPARAM)gsLevelStat.mRace2, 0);
	SendMessage(hWndReport1ComboClass, CB_SETCURSEL, (WPARAM)gsLevelStat.mClass, 0);
}

void UpdateReportPage2(HWND hWnd)
{
	//	Set Data
	SendMessage(hWndReport2ComboRaceAt, CB_SETCURSEL, (WPARAM)gsLevelStat.mRaceAt, 0);
	SendMessage(hWndReport2ComboClassAt, CB_SETCURSEL, (WPARAM)gsLevelStat.mClassAt, 0);
	SendMessage(hWndReport2ComboLevelAt, CB_SETCURSEL, (WPARAM)gsLevelStat.mLevelAt-1, 0);
	SendMessage(hWndReport2ComboRaceDf, CB_SETCURSEL, (WPARAM)gsLevelStat.mRaceDf, 0);
	SendMessage(hWndReport2ComboClassDf, CB_SETCURSEL, (WPARAM)gsLevelStat.mClassDf, 0);
	SendMessage(hWndReport2ComboLevelDf, CB_SETCURSEL, (WPARAM)gsLevelStat.mLevelDf-1, 0);
}

void UpdateReportPage3(HWND hWnd)
{
	//	Set Data
	SendMessage(hWndReport3ComboRaceAt, CB_SETCURSEL, (WPARAM)gsLevelStat.mExpRaceAt, 0);
	SendMessage(hWndReport3ComboRaceAt2, CB_SETCURSEL, (WPARAM)gsLevelStat.mExpRaceAt2, 0);
	SendMessage(hWndReport3ComboClassAt, CB_SETCURSEL, (WPARAM)gsLevelStat.mExpClassAt, 0);
	SendMessage(hWndReport3ComboRaceDf, CB_SETCURSEL, (WPARAM)gsLevelStat.mExpRaceDf, 0);
	SendMessage(hWndReport3ComboRaceDf2, CB_SETCURSEL, (WPARAM)gsLevelStat.mExpRaceDf2, 0);
	SendMessage(hWndReport3ComboClassDf, CB_SETCURSEL, (WPARAM)gsLevelStat.mExpClassDf, 0);
}

void UpdateReportPage4(HWND hWnd)
{
	//	Set Data
	SendMessage(hWndReport4ComboRaceAt, CB_SETCURSEL, (WPARAM)gsLevelStat.mAllExpRaceAt, 0);
	SendMessage(hWndReport4ComboRaceAt2, CB_SETCURSEL, (WPARAM)gsLevelStat.mAllExpRaceAt2, 0);
	SendMessage(hWndReport4ComboClassAt, CB_SETCURSEL, (WPARAM)gsLevelStat.mAllExpClassAt, 0);
	SendMessage(hWndReport4ComboClassDf, CB_SETCURSEL, (WPARAM)gsLevelStat.mAllExpClassDf, 0);
}

void ReportPage1InitDlg(void)
{
	int i;

	//	종족 리스트 삭제
	SendMessage(hWndReport1ComboRace, CB_RESETCONTENT, 0, 0);
	SendMessage(hWndReport1ComboRace2, CB_RESETCONTENT, 0, 0);
	//	종족 리스트 추가
	for(i=0 ; i<pBalance->GetRaceMax() ; i++)
	{
		SendMessage(hWndReport1ComboRace, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
		SendMessage(hWndReport1ComboRace2, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
	}
	//	종족 기본 선택
	SendMessage(hWndReport1ComboRace, CB_SETCURSEL, gsLevelStat.mRace, 0);
	SendMessage(hWndReport1ComboRace2, CB_SETCURSEL, gsLevelStat.mRace2, 0);

	//	직업 리스트 삭제
	SendMessage(hWndReport1ComboClass, CB_RESETCONTENT, 0, 0);
	//	직업 리스트 추가
	for(i=0 ; i<giTCLASSCHART_COUNT ; i++)
	{
		SendMessage(hWndReport1ComboClass, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassStat(i)->GetName());
	}
	//	출력을 위한 직업 평균 리스트 추가
	for(i=0 ; i<pBalance->GetClassExCount() ; i++)
	{
		SendMessage(hWndReport1ComboClass, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
	}
	//	종족 기본 선택
	SendMessage(hWndReport1ComboClass, CB_SETCURSEL, gsLevelStat.mClass, 0);
}

void ReportPage2InitDlg(void)
{
	int i;

		//	종족 리스트 삭제
	SendMessage(hWndReport2ComboRaceAt, CB_RESETCONTENT, 0, 0);
	SendMessage(hWndReport2ComboRaceDf, CB_RESETCONTENT, 0, 0);
	//	종족 리스트 추가
	for(i=0 ; i<pBalance->GetRaceMax() ; i++)
	{
		SendMessage(hWndReport2ComboRaceAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
		SendMessage(hWndReport2ComboRaceDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
	}

/*
	//	출력을 위한 종족 평균 리스트 추가
	for(i=0 ; i<pBalance->GetRaceExCount() ; i++)
	{
		SendMessage(hWndReport2ComboRaceAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));
		SendMessage(hWndReport2ComboRaceDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceName(i));
	}
//*/
	//	종족 기본 선택
	SendMessage(hWndReport2ComboRaceAt, CB_SETCURSEL, gsLevelStat.mRaceAt, 0);
	SendMessage(hWndReport2ComboRaceDf, CB_SETCURSEL, gsLevelStat.mRaceDf, 0);

	//	직업 리스트 삭제
	SendMessage(hWndReport2ComboClassAt, CB_RESETCONTENT, gsLevelStat.mExpClassAt, 0);
	SendMessage(hWndReport2ComboClassDf, CB_RESETCONTENT, gsLevelStat.mExpClassDf, 0);
	//	직업 리스트 추가
	for(i=0 ; i<giTCLASSCHART_COUNT ; i++)
	{
		SendMessage(hWndReport2ComboClassAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassStat(i)->GetName());
		SendMessage(hWndReport2ComboClassDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassStat(i)->GetName());
	}

/*
	//	출력을 위한 직업 평균 리스트 추가
	for(i=0 ; i<pBalance->GetClassExCount() ; i++)
	{
		SendMessage(hWndReport2ComboClassAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
		SendMessage(hWndReport2ComboClassDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
	}
//*/
	//	종족 기본 선택
	SendMessage(hWndReport2ComboClassAt, CB_SETCURSEL, 0, 0);
	SendMessage(hWndReport2ComboClassDf, CB_SETCURSEL, 0, 0);
}

void ReportPage3InitDlg(void)
{
	int i;

		//	종족 리스트 삭제
	SendMessage(hWndReport3ComboRaceAt, CB_RESETCONTENT, 0, 0);
	SendMessage(hWndReport3ComboRaceAt2, CB_RESETCONTENT, 0, 0);
	SendMessage(hWndReport3ComboRaceDf, CB_RESETCONTENT, 0, 0);
	SendMessage(hWndReport3ComboRaceDf2, CB_RESETCONTENT, 0, 0);
	//	종족 리스트 추가
	for(i=0 ; i<pBalance->GetRaceMax() ; i++)
	{
		SendMessage(hWndReport3ComboRaceAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
		SendMessage(hWndReport3ComboRaceAt2, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
		SendMessage(hWndReport3ComboRaceDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
		SendMessage(hWndReport3ComboRaceDf2, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
	}
	//	종족 기본 선택
	SendMessage(hWndReport3ComboRaceAt, CB_SETCURSEL, gsLevelStat.mExpRaceAt, 0);
	SendMessage(hWndReport3ComboRaceAt2, CB_SETCURSEL, gsLevelStat.mExpRaceAt2, 0);
	SendMessage(hWndReport3ComboRaceDf, CB_SETCURSEL, gsLevelStat.mExpRaceDf, 0);
	SendMessage(hWndReport3ComboRaceDf2, CB_SETCURSEL, gsLevelStat.mExpRaceDf2, 0);

	//	직업 리스트 삭제
	SendMessage(hWndReport3ComboClassAt, CB_RESETCONTENT, 0, 0);
	SendMessage(hWndReport3ComboClassDf, CB_RESETCONTENT, 0, 0);
	//	직업 리스트 추가
	for(i=0 ; i<giTCLASSCHART_COUNT ; i++)
	{
		SendMessage(hWndReport3ComboClassAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassStat(i)->GetName());
		SendMessage(hWndReport3ComboClassDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassStat(i)->GetName());
	}
	//	출력을 위한 직업 평균 리스트 추가
	for(i=0 ; i<pBalance->GetClassExCount() ; i++)
	{
		SendMessage(hWndReport3ComboClassAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
		SendMessage(hWndReport3ComboClassDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
	}
	//	종족 기본 선택
	SendMessage(hWndReport3ComboClassAt, CB_SETCURSEL, gsLevelStat.mExpClassAt, 0);
	SendMessage(hWndReport3ComboClassDf, CB_SETCURSEL, gsLevelStat.mExpClassDf, 0);
}

void ReportPage4InitDlg(void)
{
	int i;

		//	종족 리스트 삭제
	SendMessage(hWndReport4ComboRaceAt, CB_RESETCONTENT, 0, 0);
	SendMessage(hWndReport4ComboRaceAt2, CB_RESETCONTENT, 0, 0);
	//	종족 리스트 추가
	for(i=0 ; i<pBalance->GetRaceMax() ; i++)
	{
		SendMessage(hWndReport4ComboRaceAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
		SendMessage(hWndReport4ComboRaceAt2, CB_ADDSTRING, 0, (LPARAM)pBalance->GetRaceStat(i)->GetName());
	}
	//	종족 기본 선택
	SendMessage(hWndReport4ComboRaceAt, CB_SETCURSEL, gsLevelStat.mAllExpRaceAt, 0);
	SendMessage(hWndReport4ComboRaceAt2, CB_SETCURSEL, gsLevelStat.mAllExpRaceAt2, 0);

	//	직업 리스트 삭제
	SendMessage(hWndReport4ComboClassAt, CB_RESETCONTENT, 0, 0);
	SendMessage(hWndReport4ComboClassDf, CB_RESETCONTENT, 0, 0);
	//	직업 리스트 추가
	for(i=0 ; i<giTCLASSCHART_COUNT ; i++)
	{
		SendMessage(hWndReport4ComboClassAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassStat(i)->GetName());
		SendMessage(hWndReport4ComboClassDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassStat(i)->GetName());
	}
	//	출력을 위한 직업 평균 리스트 추가
	for(i=0 ; i<pBalance->GetClassExCount() ; i++)
	{
		SendMessage(hWndReport4ComboClassAt, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
		SendMessage(hWndReport4ComboClassDf, CB_ADDSTRING, 0, (LPARAM)pBalance->GetClassName(i));
	}
	//	종족 기본 선택
	SendMessage(hWndReport4ComboClassAt, CB_SETCURSEL, gsLevelStat.mAllExpClassAt, 0);
	SendMessage(hWndReport4ComboClassDf, CB_SETCURSEL, gsLevelStat.mAllExpClassDf, 0);
}

void ReportPageAllInitDlg(void)
{
	ReportPage1InitDlg();
	ReportPage2InitDlg();
	ReportPage3InitDlg();
}

