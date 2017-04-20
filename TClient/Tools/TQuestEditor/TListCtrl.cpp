// TListCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "TListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTListCtrl

CTListCtrl::CTListCtrl()
{
}

CTListCtrl::~CTListCtrl()
{
}


BEGIN_MESSAGE_MAP(CTListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CTListCtrl)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTListCtrl message handlers

void CTListCtrl::SetCurSel( int nIndex)
{
	POSITION pos = GetFirstSelectedItemPosition();
	BOOL bUpdate = nIndex != -1;

	while(pos)
	{
		int nSel = GetNextSelectedItem(pos);

		if( nSel != nIndex )
		{
			SetItem(
				nSel, 0,
				LVIF_STATE,
				NULL, 0, 0,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				NULL);

			Update(nSel);
		}
		else
			bUpdate = FALSE;
	}

	if(bUpdate)
	{
		SetItem(
			nIndex, 0,
			LVIF_STATE,
			NULL, 0,
			LVIS_SELECTED|
			LVIS_FOCUSED,
			LVIS_SELECTED|
			LVIS_FOCUSED,
			NULL);

		Update(nIndex);
	}
}

int CTListCtrl::GetCurSel()
{
	POSITION pos = GetFirstSelectedItemPosition();

	if(pos)
		return GetNextSelectedItem(pos);

	return -1;
}

void CTListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	CListCtrl::OnLButtonDown(nFlags, point);
}

void CTListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();
	CListCtrl::OnRButtonDown(nFlags, point);
}
