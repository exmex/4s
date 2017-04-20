#pragma once
#include "afxcmn.h"
#include "afxwin.h"


#define  CLOSED_FOLDER 0
#define  OPEN_FOLDER   1
#define  FLOPPY_DRIVE  2
#define  FIXED_DRIVE   3
#define  NET_DRIVE     4
#define  CDROM_DRIVE   5
#define  MY_COM        6
#define  RECYCLE_BIN   7
#define  DESKTOP       8
#define  CONTROL_PANEL 9

// CAutoPatch 폼 뷰입니다.

class CAutoPatch : public CFormView
{
	DECLARE_DYNCREATE(CAutoPatch)

protected:
	CAutoPatch();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CAutoPatch();

	void SetImageList();
	HICON GetIconHandle(int nIndex);

private:
	CImageList m_imageList;
	CString			m_strPrevFolder;
	CString         m_strNextFolder;
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
	BYTE OnCompare();
	void SearchNew(LPCTSTR strPrev, LPCTSTR strNext);
	LPPATCHFILE CheckExist(LPCTSTR strFile1, LPCTSTR strFile2);
	void AddPatchFile(LPPATCHFILE pFile);
	void SearchDifference(LPCTSTR strPrev, LPCTSTR strNext);
	LPPATCHFILE CompareFiles(LPCTSTR strFile1, LPCTSTR strFile2);
	void SeparatePath(LPPATCHFILE pFile);
public:
	enum { IDD = IDD_AUTOPATCH };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treectrlPrevFile;
	CTreeCtrl m_treectrlNextFile;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CButton m_buttonPrevFile;
	CButton m_buttonNextFile;
	virtual void OnInitialUpdate();
	afx_msg void OnTvnItemexpandingTreePrevfile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnItemexpandingTreeNextfile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusTreePrevfile(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchangedTreeNextfile(NMHDR *pNMHDR, LRESULT *pResult);
};
