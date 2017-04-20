#pragma once


// CTreeCtrlST

class CTreeCtrlST : public CTreeCtrl
{
	DECLARE_DYNAMIC(CTreeCtrlST)

public:
	CTreeCtrlST();
	virtual ~CTreeCtrlST();

protected:
	DECLARE_MESSAGE_MAP()
public:
	///////////////////////////////////////////////////////////////////////
	// Tree ID
	BYTE m_nID;
	// Tree Count
	DWORD m_TreeCnt;
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Get TVINSERTSTRUCT
	TVINSERTSTRUCT GetTVINSERTSTRUCT(HTREEITEM hTreeItem, CString pszText);
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Init TreeCtrl
	BOOL m_bImage;
	CImageList* m_pImageList;	

	void InitTreeCtrl(int nID);
	void InitTreeCtrl(int nID, int size, int iImage, int iSelectedImage, int iChangeImage);
	void SetImage(int size, int iImage, int iSelectedImage, int iChangeImage);

	// Change Image
	void ChangeImage(int iImage, int iSelectedImage);

	// Insert Tree Item
	HTREEITEM InsertTreeItem(CString strText, HTREEITEM hParent);
	///////////////////////////////////////////////////////////////////////	
	
	///////////////////////////////////////////////////////////////////////
	// RButton Context Menu
	afx_msg void OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);

	// Context Menu Insert Init / Delete All
	afx_msg void OnContextmenuInsertinit();
	afx_msg void OnContextmenuDeleteall();
	
	// Context Tree Menu Insert / Delete
	afx_msg void OnContextmenuInsert();
	afx_msg void OnContextmenuChildinsert();
	afx_msg void OnContextmenuDelete();
	///////////////////////////////////////////////////////////////////////

	///////////////////////////////////////////////////////////////////////
	// Label Edit
	BOOL m_bEdit;
	
	afx_msg void OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	///////////////////////////////////////////////////////////////////////	
};


