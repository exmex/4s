// TabCtrlHappy.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "TabCtrlHappy.h"
#include "MainFrm.h"
#include "HappyDoc.h"

// CTabCtrlHappy

IMPLEMENT_DYNAMIC(CTabCtrlHappy, CTabCtrl)
CTabCtrlHappy::CTabCtrlHappy()
{
#ifndef _AFX_NO_OCC_SUPPORT
	AfxEnableControlContainer ();
#endif // !_AFX_NO_OCC_SUPPORT
}

CTabCtrlHappy::~CTabCtrlHappy()
{
}


BEGIN_MESSAGE_MAP(CTabCtrlHappy, CTabCtrl)
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(TCN_SELCHANGE, OnTcnSelchange)
	ON_NOTIFY_REFLECT(TCN_SELCHANGING, OnTcnSelchanging)
END_MESSAGE_MAP()


int CTabCtrlHappy::AddTabPage (LPCTSTR pszTitle, int nPageID, CWnd* pTabPage)
{
	// Add a page to the tab control.
    TabDelete tabDelete;
    tabDelete.pTabPage = pTabPage;
    tabDelete.bDelete = FALSE;

    return AddPage (pszTitle, nPageID, tabDelete);
}

BOOL CTabCtrlHappy::RemoveTabPage (int nIndex)
{
	if (nIndex >= GetItemCount ())
		return FALSE;

	// Notify derived classes that the page is being destroyed.
	OnDestroyPage (nIndex, m_nPageIDs[nIndex]);

	// Switch pages if the page being deleted is the current page and it's
	// not the only remaining page.
	int nCount = GetItemCount ();
	if (nCount > 1 && nIndex == GetCurSel ())
	{
		int nPage = nIndex + 1;
		if (nPage >= nCount)
			nPage = nCount - 2;
		ActivateTabPage (nPage);
	}

	// Remove the page from the tab control.
	DeleteItem (nIndex);

	// Destroy the dialog (if any) that represents the page.
    TabDelete tabDelete = m_tabs[nIndex];
    CWnd* pWnd = tabDelete.pTabPage;
	if (pWnd != NULL)
	{
		pWnd->DestroyWindow ();	
		delete pWnd;
	}

	// Clean up, repaint, and return.
	m_tabs.RemoveAt (nIndex);
	m_hFocusWnd.RemoveAt (nIndex);
	m_nPageIDs.RemoveAt (nIndex);
	Invalidate ();
	return TRUE;
}

int CTabCtrlHappy::GetTabPageCount (void)
{
	return GetItemCount ();
}

BOOL CTabCtrlHappy::GetTabPageTitle (int nIndex, CString &strTitle)
{
	if (nIndex >= GetItemCount ())
		return FALSE;

	TCHAR szTitle[1024];

	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = szTitle;
	item.cchTextMax = sizeof szTitle / sizeof (TCHAR);

	if (!GetItem (nIndex, &item))
		return FALSE;

	strTitle = item.pszText;
	return TRUE;
}

BOOL CTabCtrlHappy::SetTabPageTitle (int nIndex, LPCTSTR pszTitle)
{
	if (nIndex >= GetItemCount ())
		return FALSE;

	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = (LPTSTR) pszTitle;
	
	BOOL bResult = SetItem (nIndex, &item);
	if (bResult)
		Invalidate ();
	return bResult;
}

int CTabCtrlHappy::GetTabPageID (int nIndex)
{
	if (nIndex >= GetItemCount ())
		return -1;

	return m_nPageIDs[nIndex];
}

int CTabCtrlHappy::SetTabPageID (int nIndex, int nPageID)
{
	if (nIndex >= GetItemCount ())
		return -1;

	int nOldPageID = m_nPageIDs[nIndex];
	m_nPageIDs[nIndex] = nPageID;
	return nOldPageID;
}

BOOL CTabCtrlHappy::ActivateTabPage (int nIndex)
{
	if (nIndex >= GetItemCount ())
		return FALSE;

	// Do nothing if the specified page is already active.
	if (nIndex == GetCurSel ())
		return TRUE;

	// Deactivate the current page.
	int nOldIndex = GetCurSel ();

	if (nIndex != -1)
	{
        TabDelete tabDelete = m_tabs[nOldIndex];
        CWnd* pWnd = tabDelete.pTabPage;
		if (pWnd != NULL)
		{
			m_hFocusWnd[nOldIndex] = ::GetFocus ();
			pWnd->ShowWindow (SW_HIDE);
		}
	}

	// Activate the new one.
	SetCurSel (nIndex);
    TabDelete tabDelete = m_tabs[nIndex];
    CWnd* pWnd = tabDelete.pTabPage;

	if (pWnd != NULL)
	{
		::SetFocus (m_hFocusWnd[nIndex]);
		CRect rect;
		GetClientRect (&rect);
		ResizeWindow(nIndex, rect.Width (), rect.Height ());
		pWnd->ShowWindow (SW_SHOW);
	}
	return TRUE;
}

int CTabCtrlHappy::GetTabActivePage (void)
{
	return GetCurSel ();
}

CWnd* CTabCtrlHappy::GetTabPage (int nIndex)
{
	if (nIndex == -1 || nIndex >= GetItemCount ())
		return NULL;

    TabDelete tabDelete = m_tabs[nIndex];
    return (CWnd*) tabDelete.pTabPage;
}

int CTabCtrlHappy::GetTabPageIndex (int nPageID)
{
	int nCount = GetItemCount ();
	if (nCount == 0)
		return -1;

	for (int i=0; i<nCount; i++)
	{
		if (m_nPageIDs[i] == nPageID)
			return i;
	}

	return -1;
}

/////////////////////////////////////////////////////////////////////////////
// Private helper functions

#ifdef _DEBUG
BOOL CTabCtrlHappy::CheckDialogTemplate (LPCTSTR pszTemplateName)
{
	// Verify that the dialog resource exists.
	ASSERT (pszTemplateName != NULL);
	HINSTANCE hInstance = AfxFindResourceHandle (pszTemplateName, RT_DIALOG);
	HRSRC hResource = ::FindResource (hInstance, pszTemplateName, RT_DIALOG);

	if (hResource == NULL)
		return FALSE; // Resource doesn't exist

	HGLOBAL hTemplate = LoadResource (hInstance, hResource);
	ASSERT (hTemplate != NULL);

	// Get the dialog's style bits.
	DLGTEMPLATEEX* pTemplate = (DLGTEMPLATEEX*) LockResource (hTemplate);

	DWORD dwStyle;
	if (pTemplate->signature == 0xFFFF)
		dwStyle = pTemplate->style;
	else
		dwStyle = ((DLGTEMPLATE*) pTemplate)->style;

	UnlockResource (hTemplate);
	FreeResource (hTemplate);

	// Verify that the dialog is an invisible child window.
	if (dwStyle & WS_VISIBLE)
		return FALSE; // WS_VISIBLE flag is set

	if (!(dwStyle & WS_CHILD))
		return FALSE; // WS_CHILD flag isn't set

	// Verify that the dialog has no border and no title bar.
	if (dwStyle & (WS_BORDER | WS_THICKFRAME | DS_MODALFRAME))
		return FALSE; // One or more border flags are set

	if (dwStyle & WS_CAPTION)
		return FALSE; // WS_CAPTION flag is set

	return TRUE;
}
#endif // _DEBUG

void CTabCtrlHappy::ResizeWindow (int nIndex, int cx, int cy)
{
	if (nIndex != -1)
	{
        TabDelete tabDelete = m_tabs[nIndex];
        CWnd* pWnd = tabDelete.pTabPage;

		if (pWnd != NULL)
		{
			CRect rect;
			GetItemRect (nIndex, &rect);

			int x, y, nWidth, nHeight;
			DWORD dwStyle = GetStyle ();

			if (dwStyle & TCS_VERTICAL)
			{ // Vertical tabs
				int nTabWidth =
					rect.Width () * GetRowCount ();
				x = (dwStyle & TCS_RIGHT) ? 4 : nTabWidth + 4;
				y = 4;
				nWidth = cx - nTabWidth - 8;
				nHeight = cy - 8;
			}
			else
			{ // Horizontal tabs
				int nTabHeight =
					rect.Height () * GetRowCount ();
				x = 4;
				y = (dwStyle & TCS_BOTTOM) ? 4 : nTabHeight + 4;
				nWidth = cx - 8;
				nHeight = cy - nTabHeight - 8;
			}
			pWnd->SetWindowPos (NULL, x, y, nWidth, nHeight, SWP_NOZORDER);
		}
	}
}

/////////////////////////////////////////////////////////////////////////////
// Overridables

BOOL CTabCtrlHappy::OnInitPage (int nIndex, int nPageID)
{
	// TODO: Override in derived class to initialise pages.
	return TRUE;
}

void CTabCtrlHappy::OnActivatePage (int nIndex, int nPageID)
{
	// TODO: Override in derived class to respond to page activations.
}

void CTabCtrlHappy::OnDeactivatePage (int nIndex, int nPageID)
{
	// TODO: Override in derived class to respond to page deactivations.
}

void CTabCtrlHappy::OnDestroyPage (int nIndex, int nPageID)
{
	// TODO: Override in derived class to free resources.
}

/////////////////////////////////////////////////////////////////////////////
// Message handlers

int CTabCtrlHappy::AddPage (LPCTSTR pszTitle, int nPageID, TabDelete tabDelete)
{
	// Add a page to the tab control.
	TC_ITEM item;
	item.mask = TCIF_TEXT;
	item.pszText = (LPTSTR) pszTitle;
	int nIndex = GetItemCount ();
	
	if (InsertItem (nIndex, &item) == -1)
		return -1;

    if (NULL == tabDelete.pTabPage) {
		// Fail - no point calling the function with a NULL pointer!
		DeleteItem (nIndex);
		return -1;
	}
	else {
		// Record the address of the dialog object and the page ID.
		int nArrayIndex = (int)m_tabs.Add (tabDelete);
		ASSERT (nIndex == nArrayIndex);

		nArrayIndex = (int)m_nPageIDs.Add (nPageID);
		ASSERT (nIndex == nArrayIndex);

		// Size and position the dialog box within the view.
        tabDelete.pTabPage->SetParent (this); // Just to be sure

		CRect rect;
		GetClientRect (&rect);

		if (rect.Width () > 0 && rect.Height () > 0)
			ResizeWindow(nIndex, rect.Width (), rect.Height ());

		// Initialize the page.
		if (OnInitPage (nIndex, nPageID)) {
			// Make sure the first control in the dialog is the one that
			// receives the input focus when the page is displayed.
			HWND hwndFocus = tabDelete.pTabPage->GetTopWindow ()->m_hWnd;
			nArrayIndex = (int)m_hFocusWnd.Add (hwndFocus);
			ASSERT (nIndex == nArrayIndex);
		}
		else {
			// Make the control that currently has the input focus is the one
			// that receives the input focus when the page is displayed.
			m_hFocusWnd.Add (::GetFocus ());
		}

		// If this is the first page added to the view, make it visible.
		if (nIndex == 0)
			tabDelete.pTabPage->ShowWindow (SW_SHOW);
		else
			tabDelete.pTabPage->ShowWindow (SW_HIDE);
	}
	return nIndex;
}

void CTabCtrlHappy::OnMoveWindow()
{
	CRect rect;
	GetClientRect (&rect);

	if (rect.Width () > 0 && rect.Height () > 0)
		ResizeWindow(GetTabActivePage(), rect.Width (), rect.Height ());
}

// CTabCtrlHappy 메시지 처리기입니다.
void CTabCtrlHappy::OnDestroy()
{
	CTabCtrl::OnDestroy();

	int nCount = (int)m_tabs.GetSize ();

	// Destroy dialogs and delete CTabCtrlTab objects.
	if (nCount > 0)
	{
		for (int i=nCount - 1; i>=0; i--) {
			OnDestroyPage (i, m_nPageIDs[i]);
            TabDelete tabDelete = m_tabs[i];
            CWnd* pWnd = tabDelete.pTabPage;
			if (pWnd != NULL)
			{
				pWnd->DestroyWindow ();
				for(int j=0; j<i; j++)
				{
					if(m_tabs[j].pTabPage == pWnd)
					m_tabs[j].pTabPage = NULL;
				}

                if (TRUE == tabDelete.bDelete)
					delete pWnd;
			}
		}
	}

	// Clean up the internal arrays.
	m_tabs.RemoveAll ();
	m_hFocusWnd.RemoveAll ();
	m_nPageIDs.RemoveAll ();
}

void CTabCtrlHappy::OnSetFocus(CWnd* pOldWnd)
{
	CTabCtrl::OnSetFocus(pOldWnd);

	// Set the focus to a control on the current page.
	int nIndex = GetCurSel ();
	if (nIndex != -1)
		::SetFocus (m_hFocusWnd[nIndex]);
}

void CTabCtrlHappy::OnKillFocus(CWnd* pNewWnd)
{
	CTabCtrl::OnKillFocus(pNewWnd);

	// Save the HWND of the control that holds the input focus.
	int nIndex = GetCurSel ();
	if (nIndex != -1)
		m_hFocusWnd[nIndex] = ::GetFocus ();	
}

void CTabCtrlHappy::OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nIndex = GetCurSel ();
	if (nIndex == -1)
		return;

	// Show the new page.
    TabDelete tabDelete = m_tabs[nIndex];
    CWnd* pWnd = tabDelete.pTabPage;

	if (pWnd != NULL) {
		::SetFocus (m_hFocusWnd[nIndex]);
		CRect rect;
		GetClientRect (&rect);
		ResizeWindow(nIndex, rect.Width (), rect.Height ());
		pWnd->ShowWindow (SW_SHOW);
	}

	BYTE bCurPage = 0;

	// Notify derived classes that the selection has changed.
	OnActivatePage (nIndex, m_nPageIDs[nIndex]);
	CMainFrame * pFrm = (CMainFrame *)GetParentFrame();	
	switch(nIndex)
	{
	case 0:		
		if(pFrm->GetGraphCheck()) // 현승룡 Graph
		{
			pFrm->m_wndSplitter.ShowView(pFrm->m_nGraph);
			bCurPage = 1;
		}
		else
			pFrm->m_wndSplitter.ShowView(pFrm->m_nList);
		pFrm->ShowToolBar(nIndex);
		break;
	case 1:
		pFrm->m_wndSplitter.ShowView(pFrm->m_nListPatch);
		pFrm->ShowToolBar(nIndex);
		bCurPage = 2;
		break;
	case 2: // 현승룡 Machine
		pFrm->m_wndSplitter.ShowView(pFrm->m_nMachineGraph);
		pFrm->ShowToolBar(0);
		bCurPage = 3;
		break;
	}
	pFrm->SetCurView(bCurPage);

	*pResult = 0;
}

void CTabCtrlHappy::OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult)
{
	// Notify derived classes that the selection is changing.
	int nIndex = GetCurSel ();
	if (nIndex == -1)
		return;

	OnDeactivatePage (nIndex, m_nPageIDs[nIndex]);

	// Save the input focus and hide the old page.
    TabDelete tabDelete = m_tabs[nIndex];
    CWnd* pWnd = tabDelete.pTabPage;

	if (pWnd != NULL) {
		m_hFocusWnd[nIndex] = ::GetFocus ();
		pWnd->ShowWindow (SW_HIDE);
	}

	*pResult = 0;
}

BOOL CTabCtrlHappy::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
#ifndef _AFX_NO_OCC_SUPPORT
	if (nCode == CN_EVENT)
		return GetParent ()->OnCmdMsg (nID, nCode, pExtra, pHandlerInfo);
#endif // !_AFX_NO_OCC_SUPPORT

	return CTabCtrl::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

BOOL CTabCtrlHappy::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	GetParent ()->SendMessage (WM_COMMAND, wParam, lParam);
	
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = (CServiceTree *)pFrm->m_wndSplitter.GetView(pFrm->m_nTree);
	switch(wParam)
	{
	case 2000: pTree->MapSvrConnect(); break;
	case 2001: pTree->MapSvrDisConnect();break;
	default:break;
	}

	return CTabCtrl::OnCommand(wParam, lParam);
}

BOOL CTabCtrlHappy::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	GetParent()->SendMessage (WM_NOTIFY, wParam, lParam);
	return CTabCtrl::OnNotify(wParam, lParam, pResult);
}
