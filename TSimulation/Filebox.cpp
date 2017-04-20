#include "stdafx.h"
#include <windows.h>
#include <stdio.h>
#include <commdlg.h>
#include <shlobj.h>

#include "filebox.h"
#include "resource.h"

HINSTANCE hInstFileBox = NULL;
char *ResultPath;

BOOL SaveBox( HWND hwnd, char *filename, char *dir )
{
	 
	 OPENFILENAME opn;
	 BOOL ret;

	 char filetitle[200];
     memset( filetitle,0,200 );

	 memset( &opn,0,sizeof(OPENFILENAME) );

	 opn.lStructSize       = sizeof(OPENFILENAME);
	 opn.hwndOwner         = hwnd;
	 opn.hInstance         = hInstFileBox;

     opn.lpstrFilter       = "Stat File(*.sta;)\0*.*sta;\0\0";

	 opn.lpstrCustomFilter = NULL;
	 opn.nMaxCustFilter    = 0;
	 opn.nFilterIndex      = 1;

	 opn.lpstrFile         = filename;

	 opn.nMaxFile          = 500;
	 opn.lpstrFileTitle    = filetitle;
	 opn.nMaxFileTitle     = 99;
	 opn.lpstrInitialDir   = dir;
	 opn.lpstrTitle        = "Stat 파일 저장";

     opn.Flags             = OFN_FILEMUSTEXIST|OFN_EXPLORER|OFN_HIDEREADONLY|
		                     OFN_ENABLEHOOK;//|OFN_ENABLETEMPLATE;

	 opn.lpstrDefExt       = "sta";

     opn.lCustData         = 2;
	 opn.lpfnHook          = SaveBoxProc;

//	 opn.lpTemplateName    = MAKEINTRESOURCE( IDD_FILEOPEN );

	 ret=GetSaveFileName(&opn);

	 return ret;
}

UINT CALLBACK SaveBoxProc(HWND hwnd,unsigned mes,WPARAM wparam,LPARAM lparam)
{
	static LPOPENFILENAME lpopn;
	static char pfname[100]="";
//	static HWND hwndStatic;

	 switch(mes){
	        case WM_NOTIFY:
				 
				 switch( ((LPOFNOTIFY)lparam)->hdr.code )
				 {
					     case CDN_SELCHANGE :
							 {
		 						/*char szstring[100]="";
								int width, height, bpp;
								if( CommDlg_OpenSave_GetFilePath( GetParent(hwnd), pfname, sizeof(pfname) ) 
								  <=sizeof(pfname) )
								{
									PrevImageLoad(pfname);
									PrevImageView(hwndStatic);
									width=ImageWidth();
									height=ImageHeight();
									bpp=ImageBpp();
									if((width!=0)||(height!=0))
									{
										sprintf(szstring, "%d x %d x %d Bit - BMP", width, height, bpp);
										SetDlgItemText(hwnd, IDC_OPEN_EDIT, szstring);
									}
								}*/
							 }
					          break;
					     case CDN_FILEOK :
					          break;
						 case CDN_SHAREVIOLATION:
							  break;
				 }
                 break;
			case WM_PAINT:
//				PrevImageView(hwndStatic);
				 break;
			case WM_DESTROY:
				break;
			case WM_INITDIALOG :
//				hwndStatic = GetDlgItem(hwnd, IDC_OPEN_STATIC);
				 lpopn=(LPOPENFILENAME)lparam;
				 break;
	}
	return FALSE;
}

BOOL OpenBox( HWND hwnd, char *filename, char *dir )
{
	 
	 OPENFILENAME opn;
	 BOOL ret;

	 char filetitle[200];
     memset( filetitle,0,200 );

	 memset( &opn,0,sizeof(OPENFILENAME) );

	 opn.lStructSize       = sizeof(OPENFILENAME);
	 opn.hwndOwner         = hwnd;
	 opn.hInstance         = hInstFileBox;

     opn.lpstrFilter       = "Stat File(*.sta;)\0*.*sta;\0\0";

	 opn.lpstrCustomFilter = NULL;
	 opn.nMaxCustFilter    = 0;
	 opn.nFilterIndex      = 1;

	 opn.lpstrFile         = filename;

	 opn.nMaxFile          = 500;
	 opn.lpstrFileTitle    = filetitle;
	 opn.nMaxFileTitle     = 99;
	 opn.lpstrInitialDir   = dir;
	 opn.lpstrTitle        = "Stat 파일 열기";

     opn.Flags             = OFN_FILEMUSTEXIST|OFN_EXPLORER|OFN_HIDEREADONLY|
		                     OFN_ENABLEHOOK;//|OFN_ENABLETEMPLATE;

	 opn.lpstrDefExt       = "sta";

     opn.lCustData         = 2;
	 opn.lpfnHook          = OpenBoxProc;

//	 opn.lpTemplateName    = MAKEINTRESOURCE( IDD_FILEOPEN );

	 ret=GetOpenFileName(&opn);

	 return ret;
}

UINT CALLBACK OpenBoxProc(HWND hwnd,unsigned mes,WPARAM wparam,LPARAM lparam)
{
	static LPOPENFILENAME lpopn;
	static char pfname[100]="";
//	static HWND hwndStatic;

	 switch(mes){
	        case WM_NOTIFY:
				 
				 switch( ((LPOFNOTIFY)lparam)->hdr.code )
				 {
					     case CDN_SELCHANGE :
							 {
		 						/*char szstring[100]="";
								int width, height, bpp;
								if( CommDlg_OpenSave_GetFilePath( GetParent(hwnd), pfname, sizeof(pfname) ) 
								  <=sizeof(pfname) )
								{
									PrevImageLoad(pfname);
									PrevImageView(hwndStatic);
									width=ImageWidth();
									height=ImageHeight();
									bpp=ImageBpp();
									if((width!=0)||(height!=0))
									{
										sprintf(szstring, "%d x %d x %d Bit - BMP", width, height, bpp);
										SetDlgItemText(hwnd, IDC_OPEN_EDIT, szstring);
									}
								}*/
							 }
					          break;
					     case CDN_FILEOK :
					          break;
						 case CDN_SHAREVIOLATION:
							  break;
				 }
                 break;
			case WM_PAINT:
//				PrevImageView(hwndStatic);
				 break;
			case WM_DESTROY:
				break;
			case WM_INITDIALOG :
//				hwndStatic = GetDlgItem(hwnd, IDC_OPEN_STATIC);
				 lpopn=(LPOPENFILENAME)lparam;
				 break;
	}
	return FALSE;
}

//*
BOOL PathBox( HWND hwnd, HINSTANCE hInst, char *filename, char *dir )
{
	 
	 OPENFILENAME opn;
	 BOOL ret=FALSE;

	 char filetitle[200];
     memset( filetitle,0,200 );

	 memset( &opn,0,sizeof(OPENFILENAME) );

	 opn.lStructSize       = sizeof(OPENFILENAME);
	 opn.hwndOwner         = hwnd;
	 opn.hInstance         = hInst;

     opn.lpstrFilter       = "보고서 파일 경로 지정\0*.;\0\0";

	 opn.lpstrCustomFilter = NULL;
	 opn.nMaxCustFilter    = 0;
	 opn.nFilterIndex      = 1;

	 opn.lpstrFile         = filename;

	 opn.nMaxFile          = 500;
	 opn.lpstrFileTitle    = filetitle;
	 opn.nMaxFileTitle     = 99;
	 opn.lpstrInitialDir   = dir;
	 opn.lpstrTitle        = "경로 지정";

     opn.Flags             = OFN_PATHMUSTEXIST|
							OFN_EXPLORER|
							OFN_HIDEREADONLY|
		                     OFN_ENABLEHOOK|OFN_ENABLETEMPLATE
							 ;

	 opn.lpstrDefExt       = "";

     opn.lCustData         = 2;
	 opn.lpfnHook          = PathBoxProc;

	 opn.lpTemplateName    = MAKEINTRESOURCE( IDD_PATH );

	 ResultPath = dir;

	 ret=GetOpenFileName(&opn);

	 return ret;
}

UINT CALLBACK PathBoxProc(HWND hwnd,unsigned mes,WPARAM wparam,LPARAM lparam)
{
	static LPOPENFILENAME lpopn;
	static char pfname[_MAX_FNAME]="";

	 switch(mes)
	 {
	        case WM_NOTIFY:
				 switch( ((LPOFNOTIFY)lparam)->hdr.code )
				 {
					     case CDN_FOLDERCHANGE :
							 {
								 CommDlg_OpenSave_GetFolderPath( GetParent(hwnd), pfname, sizeof(pfname));
								 SetDlgItemText(hwnd, IDC_EDIT_PATH, pfname);
								 strcpy(ResultPath, pfname);
							 }
					          break;
					     case CDN_FILEOK :
					          break;
						 case CDN_SHAREVIOLATION:
							  break;
				 }
                 break;
			case WM_PAINT:
				 break;
			case WM_DESTROY:
				break;
			case WM_INITDIALOG :
				 lpopn=(LPOPENFILENAME)lparam;

				 break;
	}
	return FALSE;
}
//*/