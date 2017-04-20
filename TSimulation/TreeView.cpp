#include "stdafx.h"
#include <windows.h>
#include "TreeView.h"

int treepos=0;
TV_INSERTSTRUCT tvstruct[TREEITEMMAX];
HTREEITEM itemlist[TREEITEMMAX];

int Objecttreepos=0;
TV_INSERTSTRUCT Objecttvstruct[TREEITEMMAX];
HTREEITEM Objectitemlist[TREEITEMMAX];

HTREEITEM AddTreeviewData(HWND hwnd,HTREEITEM parent,LPSTR text,int image,int selimage)
{
	  HTREEITEM item;
	  tvstruct[treepos].hParent=parent;
	  tvstruct[treepos].hInsertAfter=TVI_LAST;//TVI_SORT;
	  tvstruct[treepos].item.pszText=text;
	  tvstruct[treepos].item.mask=TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE;
	  tvstruct[treepos].item.iImage=image;
	  tvstruct[treepos].item.iSelectedImage=selimage;
	  item=(HTREEITEM)SendMessage(hwnd,TVM_INSERTITEM,0,(LPARAM)(LPTV_INSERTSTRUCT)&tvstruct[treepos]);
	  itemlist[treepos]=item;
	  treepos++;
	  return item;
}

LPSTR FindTreeViewData(HTREEITEM item)
{
	int i;
	for(i=0;i<TREEITEMMAX;i++)
		if(itemlist[i]==item)
			return tvstruct[i].item.pszText;
	return NULL;
}

WORD FindTreeViewDataNum(HTREEITEM item)
{
	int i;
	for(i=0;i<TREEITEMMAX;i++)
		if(itemlist[i]==item)
			return i;
	return -1;
}

void TreeViewReset(void)
{
	treepos=0;
}

HTREEITEM ObjectAddTreeviewData(HWND hwnd,HTREEITEM parent,LPSTR text,int image,int selimage)
{
	  HTREEITEM item;
	  Objecttvstruct[Objecttreepos].hParent=parent;
	  Objecttvstruct[Objecttreepos].hInsertAfter=TVI_LAST;//TVI_SORT;
	  Objecttvstruct[Objecttreepos].item.pszText=text;
	  Objecttvstruct[Objecttreepos].item.mask=TVIF_TEXT;
//	  Objecttvstruct[Objecttreepos].item.mask=TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE;
//	  Objecttvstruct[Objecttreepos].item.iImage=image;
//	  Objecttvstruct[Objecttreepos].item.iSelectedImage=selimage;
	  item=(HTREEITEM)SendMessage(hwnd,TVM_INSERTITEM,0,(LPARAM)(LPTV_INSERTSTRUCT)&Objecttvstruct[Objecttreepos]);
	  Objectitemlist[Objecttreepos]=item;
	  Objecttreepos++;
	  return item;
}

LPSTR ObjectFindTreeViewData(HTREEITEM item)
{
	int i;
	for(i=0;i<TREEITEMMAX;i++)
		if(Objectitemlist[i]==item)
			return Objecttvstruct[i].item.pszText;
	return NULL;
}

WORD ObjectFindTreeViewDataNum(HTREEITEM item)
{
	int i;
	for(i=0;i<TREEITEMMAX;i++)
		if(Objectitemlist[i]==item)
			return i;
	return -1;
}

void ObjectTreeViewReset(void)
{
	Objecttreepos=0;
}

int AddListviewCollum(HWND hwnd,int iItem,int cx,LPSTR text)
{
	LV_COLUMN colum;
	colum.mask=LVCF_FMT|LVCF_SUBITEM|LVCF_TEXT |LVCF_WIDTH;
	colum.fmt=LVCFMT_CENTER;
	colum.cx=cx;
	colum.pszText=text;
	colum.cchTextMax=strlen(text)+5;
	colum.iSubItem=iItem;
	return ListView_InsertColumn(hwnd,iItem,&colum);
}

int AddListviewData(HWND hwnd,int iItem,int iSubItem,LPSTR text)
{
	LV_ITEM item;
	
	if(iSubItem>0)
	{
		ListView_SetItemText(hwnd,iItem,iSubItem,text);
		return 0;
	}
	
	item.mask=LVIF_TEXT;//|LVIF_IMAGE;
	item.iItem=iItem;
	item.iSubItem=iSubItem;
	item.pszText=text;
	item.cchTextMax=80;
	return ListView_InsertItem(hwnd,&item);
}

int Upgradetreepos=0;
TV_INSERTSTRUCT Upgradetvstruct[108];
HTREEITEM Upgradeitemlist[108];

HTREEITEM UpgradeAddTreeviewData(HWND hwnd,HTREEITEM parent,LPSTR text,int image,int selimage)
{
	  HTREEITEM item;
	  Upgradetvstruct[Upgradetreepos].hParent=parent;
	  Upgradetvstruct[Upgradetreepos].hInsertAfter=TVI_LAST;//TVI_SORT;
	  Upgradetvstruct[Upgradetreepos].item.pszText=text;
	  Upgradetvstruct[Upgradetreepos].item.mask=TVIF_TEXT;
//	  Upgradetvstruct[Upgradetreepos].item.mask=TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE;
//	  Upgradetvstruct[Upgradetreepos].item.iImage=image;
//	  Upgradetvstruct[Upgradetreepos].item.iSelectedImage=selimage;
	  item=(HTREEITEM)SendMessage(hwnd,TVM_INSERTITEM,0,(LPARAM)(LPTV_INSERTSTRUCT)&Upgradetvstruct[Upgradetreepos]);
	  Upgradeitemlist[Upgradetreepos]=item;
	  Upgradetreepos++;
	  return item;
}

LPSTR UpgradeFindTreeViewData(HTREEITEM item)
{
	int i;
	for(i=0;i<TREEITEMMAX;i++)
		if(Upgradeitemlist[i]==item)
			return Upgradetvstruct[i].item.pszText;
	return NULL;
}

WORD UpgradeFindTreeViewDataNum(HTREEITEM item)
{
	int i;
	for(i=0;i<TREEITEMMAX;i++)
		if(Upgradeitemlist[i]==item)
			return i;
	return -1;
}

void UpgradeTreeViewReset(void)
{
	Upgradetreepos=0;
}

int Specialtreepos=0;
TV_INSERTSTRUCT Specialtvstruct[80];
HTREEITEM Specialitemlist[80];
BYTE SpecialitemlistNum[80];

HTREEITEM SpecialAddTreeviewData(HWND hwnd,HTREEITEM parent,LPSTR text,int image,int selimage)
{
	  HTREEITEM item;
	  Specialtvstruct[Specialtreepos].hParent=parent;
	  Specialtvstruct[Specialtreepos].hInsertAfter=TVI_LAST;//TVI_SORT;
	  Specialtvstruct[Specialtreepos].item.pszText=text;
	  Specialtvstruct[Specialtreepos].item.mask=TVIF_TEXT;
//	  Specialtvstruct[Specialtreepos].item.mask=TVIF_IMAGE | TVIF_TEXT | TVIF_SELECTEDIMAGE;
//	  Specialtvstruct[Specialtreepos].item.iImage=image;
//	  Specialtvstruct[Specialtreepos].item.iSelectedImage=selimage;
	  item=(HTREEITEM)SendMessage(hwnd,TVM_INSERTITEM,0,(LPARAM)(LPTV_INSERTSTRUCT)&Specialtvstruct[Specialtreepos]);
	  Specialitemlist[Specialtreepos]=item;
	  Specialtreepos++;
	  return item;
}

LPSTR SpecialFindTreeViewData(HTREEITEM item)
{
	int i;
	for(i=0;i<TREEITEMMAX;i++)
		if(Specialitemlist[i]==item)
			return Specialtvstruct[i].item.pszText;
	return NULL;
}

WORD SpecialFindTreeViewDataNum(HTREEITEM item)
{
	int i;
	for(i=0;i<TREEITEMMAX;i++)
		if(Specialitemlist[i]==item)
			return i;
	return -1;
}

void SpecialTreeViewReset(void)
{
	Specialtreepos=0;
}
