#if (!defined __TSHELL_H)
#define __TSHELL_H


#if _WIN32_IE >= 0x0400

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <shlobj.h>

#define WM_SHELL_NOTIFY		WM_USER + 1
#define NM_SH_SHELLMENU		1


class __TUI_API CTShell
{
public:
	CTShell( LPCTSTR lpszFileName = NULL);
	virtual ~CTShell();

public:
	CString m_strInitDir;
	CString m_strSelDir;
	CString m_strTitle;
	CString m_strPath;

	int m_nImageIndex;

protected:
	CString m_strFileName;

	char m_szDrive[_MAX_DRIVE];
	char m_szFname[_MAX_FNAME];
	char m_szDir[_MAX_DIR];
	char m_szExt[_MAX_EXT];

public:
	virtual void SplitPath(
		CString strFileName,
		char* szDrive,
		char* szDir,
		char* szFile,
		char* szExt);

	virtual LONG GetRegKey(
		HKEY key,
		LPCTSTR subkey,
		LPTSTR pData);

	virtual HINSTANCE ShellExec(
		LPCTSTR lpszExec,
		int nShow);

	virtual CString GetFileSizeString( LPCTSTR lpszFileSize);
	virtual CString GetLastWriteTime( CTime* pTime);
	virtual CString GetFileDrive( LPCTSTR lpszFileName = NULL);

	virtual BOOL Exist( LPCTSTR lpszFileName = NULL);
	virtual BOOL BrowseForFolder( CWnd* pParentWnd);

	virtual int GetIconIndex(
		LPCTSTR lpszFileName = NULL,
		UINT nFlags = SHGFI_SMALLICON);

	virtual CString GetDisplayName( LPCTSTR lpszFileName = NULL);
	virtual CString GetFileType( LPCTSTR lpszFileName = NULL);
	virtual CString GetFileExt( LPCTSTR lpszFileName = NULL);
	virtual CString GetFileTitle( LPCTSTR lpszFileName = NULL);
	virtual CString GetRoot( LPCTSTR lpszFileName = NULL);
	virtual CString GetFileName( LPCTSTR lpszFileName = NULL);

	virtual void GetSystemImageList(
		CImageList* pSmallList,
		CImageList* pLargeList);

	static int CALLBACK BrowseCtrlCallback(
		HWND hwnd,
		UINT nMsg,
		LPARAM lParam,
		LPARAM lpData);

	virtual LPITEMIDLIST Next( LPCITEMIDLIST pidl);
	virtual UINT GetSize( LPCITEMIDLIST pidl);
	virtual LPITEMIDLIST Create( UINT nSize);

	virtual LPITEMIDLIST ConcatPidls(
		LPCITEMIDLIST pidl1,
		LPCITEMIDLIST pidl2);

	virtual LPITEMIDLIST CopyITEMID(
		LPMALLOC lpMalloc,
		LPITEMIDLIST lpi);

	virtual BOOL GetName(
		LPSHELLFOLDER lpsf,
		LPITEMIDLIST lpi,
		DWORD dwFlags,
		LPTSTR lpFriendlyName);

	virtual LPITEMIDLIST GetFullyQualPidl(
		LPSHELLFOLDER lpsf,
		LPITEMIDLIST lpi);

	virtual BOOL DoTheMenuThing(
		HWND hwnd,
		LPSHELLFOLDER lpsfParent,
		LPITEMIDLIST lpi,
		LPPOINT lppt);

	virtual int	GetIcon(
		LPITEMIDLIST lpi,
		UINT nFlags);

	virtual void GetNormalAndSelectedIcons(
		LPITEMIDLIST lpifq,
		LPTV_ITEM lptvitem);

	virtual LPITEMIDLIST DuplicateItem(
		LPMALLOC pMalloc,
		LPITEMIDLIST pidl);

	virtual LPITEMIDLIST GetNextItem( LPITEMIDLIST pidl);
	virtual UINT GetItemCount( LPITEMIDLIST pidl);
	virtual BOOL DoExplorerMenu(
		HWND hwnd,
		LPCTSTR pszPath,
		POINT point);
};


/////////////////////////////////////////////////////////////////////////////

#endif // _WIN32_IE >= 0x0400
#endif // #if (!defined __TSHELL_H)