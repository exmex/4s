#pragma once

// CTabCtrlHappy

typedef struct tagDLGTEMPLATEEX{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
} DLGTEMPLATEEX;

class CTabCtrlHappy : public CTabCtrl
{
	DECLARE_DYNAMIC(CTabCtrlHappy)

public:
	CTabCtrlHappy();
	virtual ~CTabCtrlHappy();

public:
	int		AddTabPage (LPCTSTR pszTitle, int nPageID, CWnd * pTabPage);
	BOOL	RemoveTabPage (int nIndex);
	int		GetTabPageCount (void);
	BOOL	GetTabPageTitle (int nIndex, CString& strTitle);
	BOOL	SetTabPageTitle (int nIndex, LPCTSTR pszTitle);
	int		GetTabPageID (int nIndex);
	int		SetTabPageID (int nIndex, int nPageID);
	BOOL	ActivateTabPage (int nIndex);
	int		GetTabActivePage (void);
	CWnd*	GetTabPage (int nIndex);
	int		GetTabPageIndex (int nPageID);
	void	OnMoveWindow();

protected:
    struct TabDelete
	{
        CWnd * pTabPage;
        BOOL bDelete;
    };
    CArray<TabDelete, TabDelete> m_tabs;
	CArray<HWND, HWND> m_hFocusWnd;
	CArray<int, int> m_nPageIDs;

	int AddPage (LPCTSTR pszTitle, int nPageID, TabDelete tabDelete);

    virtual BOOL OnInitPage (int nIndex, int nPageID);
	virtual void OnActivatePage (int nIndex, int nPageID);
	virtual void OnDeactivatePage (int nIndex, int nPageID);
	virtual void OnDestroyPage (int nIndex, int nPageID);

#ifdef _DEBUG
	BOOL CheckDialogTemplate (LPCTSTR pszTemplateName);
#endif // _DEBUG
	void ResizeWindow(int nIndex, int cx, int cy);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnTcnSelchange(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnSelchanging(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
};