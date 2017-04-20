#include <commctrl.h>

#define TREEITEMMAX	1000

extern TV_INSERTSTRUCT tvstruct[TREEITEMMAX];
extern HTREEITEM itemlist[TREEITEMMAX];
extern int treepos;

extern TV_INSERTSTRUCT Objecttvstruct[TREEITEMMAX];
extern HTREEITEM Objectitemlist[TREEITEMMAX];
extern int Objecttreepos;

HTREEITEM AddTreeviewData(HWND,HTREEITEM,LPSTR,int,int);
LPSTR FindTreeViewData(HTREEITEM);
WORD FindTreeViewDataNum(HTREEITEM);
void TreeViewReset(void);

HTREEITEM ObjectAddTreeviewData(HWND,HTREEITEM,LPSTR,int,int);
LPSTR ObjectFindTreeViewData(HTREEITEM);
WORD ObjectFindTreeViewDataNum(HTREEITEM);
void ObjectTreeViewReset(void);
int AddListviewCollum(HWND,int,int,LPSTR);
int AddListviewData(HWND,int,int,LPSTR);

extern TV_INSERTSTRUCT Upgradetvstruct[108];
extern HTREEITEM Upgradeitemlist[108];
extern int Upgradetreepos;

HTREEITEM UpgradeAddTreeviewData(HWND,HTREEITEM,LPSTR,int,int);
LPSTR UpgradeFindTreeViewData(HTREEITEM);
WORD UpgradeFindTreeViewDataNum(HTREEITEM);
void UpgradeTreeViewReset(void);

extern TV_INSERTSTRUCT Specialtvstruct[80];
extern HTREEITEM Specialitemlist[80];
extern BYTE SpecialitemlistNum[80];
extern int Specialtreepos;

HTREEITEM SpecialAddTreeviewData(HWND,HTREEITEM,LPSTR,int,int);
LPSTR SpecialFindTreeViewData(HTREEITEM);
WORD SpecialFindTreeViewDataNum(HTREEITEM);
void SpecialTreeViewReset(void);
