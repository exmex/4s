#pragma once
// CSerchInstallTree 폼 뷰입니다.

class CSerchInstallTree : public CFormView
{
	DECLARE_DYNCREATE(CSerchInstallTree)

protected:
	CSerchInstallTree();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CSerchInstallTree();

	void SetImageList();
	HICON GetIconHandle(int nIndex);

private:
	
	enum{ CLOSED_FOLDER =0,
		OPEN_FOLDER   ,
		FLOPPY_DRIVE  ,
		FIXED_DRIVE   ,
		NET_DRIVE     ,
		CDROM_DRIVE   ,
		MY_COM        ,
		RECYCLE_BIN   ,
		DESKTOP       ,
		CONTROL_PANEL };

	CImageList m_imageList;
	CString         m_strFolder;
	CString			m_strMyComputer; //내컴퓨터

public:
	HTREEITEM AddTreeItem(CTreeCtrl * pTreeCtrl, CString strPath);
	HTREEITEM AddTreeItem(CTreeCtrl * pTreeCtrl, HTREEITEM hItem,LPCTSTR szText,int nIcon,int nSelectedIcon);
	CString GetSystemDir() const;
	void ProperPath(CString& strPath);
	void SetTreeNodes();
	void SetDrives(CTreeCtrl * pTreeCtrl, HTREEITEM& hParent);
	void SetButtonState(CTreeCtrl * pTreeCtrl, HTREEITEM hItem);
	int GetDriveIcon(CString& strDrive);
	CString GetVolumeName(CString strDrive,CString strTypeName);
	CString GetItemPath(CTreeCtrl * pTreeCtrl, HTREEITEM hItem);
	CString GetSpecialFolderPath(int nFolder);
	CString PickDriveLetter(CString strVolumeName);
	CString GetFileTitle(CString strPath);
	BOOL IsDirectory(CString strPath);
	BOOL IsDrive(CString strPath);
	BOOL IsDrive(CTreeCtrl * pTreeCtrl, HTREEITEM hItem);
	CString GetDir(CString strPath);
	HTREEITEM FindItem(CTreeCtrl * pTreeCtrl, CString strPath);
	void AddChildFolderItem(CTreeCtrl * pTreeCtrl, HTREEITEM hParent);
	void DeleteAllChildren(CTreeCtrl * pTreeCtrl, HTREEITEM hItem);
	BYTE OpenInstallFiles();
	CString GetFolder(){return m_strFolder;}
	void RefrashFile(CListCtrl * pListCtrl, CString strPath);

public:
	enum { IDD = IDD_SERCHINSTALLTREE };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treectrl;
	CListCtrl m_Listctrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_buttonPrevFile;
	CButton m_buttonNextFile;
	virtual void OnInitialUpdate();
	afx_msg void OnTvnItemexpandingTree(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusTreePrevfile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeNextfile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeInstall(NMHDR *pNMHDR, LRESULT *pResult);
};


