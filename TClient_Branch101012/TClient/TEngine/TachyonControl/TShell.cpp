#include "StdAfx.h"
#include "TShell.h"

#if _WIN32_IE >= 0x0400


/////////////////////////////////////////////////////////////////////////////
// CTShell

CTShell::CTShell( LPCTSTR lpszFileName)
{
	m_strFileName = lpszFileName;
	SplitPath( m_strFileName, m_szDrive, m_szDir, m_szFname, m_szExt);
}

CTShell::~CTShell()
{
}

int CALLBACK CTShell::BrowseCtrlCallback( HWND hwnd,
										  UINT nMsg,
										  LPARAM lParam,
										  LPARAM lpData)
{
	CTShell *pShellInfo = (CTShell *) lpData;

	if(pShellInfo->m_strSelDir.IsEmpty())
		return FALSE;

	switch(nMsg)
	{
	case BFFM_INITIALIZED	:
		{
			::SendMessage(
				hwnd,
				BFFM_SETSELECTION,
				TRUE,
				(LPARAM) (LPCTSTR) pShellInfo->m_strSelDir);
		}

		break;
	}

	return FALSE;
}

BOOL CTShell::BrowseForFolder( CWnd *pParentWnd)
{
	LPMALLOC pMalloc;

	if( ::SHGetMalloc(&pMalloc) != NOERROR )
		return FALSE;

	LPITEMIDLIST pidl;
	BROWSEINFO bInfo;

	ZeroMemory( (PVOID) &bInfo, sizeof(BROWSEINFO));
	if(!m_strInitDir.IsEmpty())
	{
		LPSHELLFOLDER pDesktopFolder;
		OLECHAR olePath[MAX_PATH];
		ULONG dwAttributes;
		ULONG nEaten;
		HRESULT hr;

		if(SUCCEEDED(SHGetDesktopFolder(&pDesktopFolder)))
		{
#if defined(_UNICODE)
			_tcsncpy( olePath, m_strInitDir, MAX_PATH);
#else
			MultiByteToWideChar(
				CP_ACP,
				MB_PRECOMPOSED,
				m_strInitDir,
				-1,
				olePath,
				MAX_PATH);
#endif
			hr = pDesktopFolder->ParseDisplayName(
				NULL,
				NULL,
				olePath,
				&nEaten,
				&pidl,
				&dwAttributes);

			if(FAILED(hr))
			{
				pMalloc->Free(pidl);
				pMalloc->Release();
				pDesktopFolder->Release();

				return FALSE;
			}

			bInfo.pidlRoot = pidl;
			pDesktopFolder->Release();
		}
	}

	bInfo.hwndOwner = pParentWnd->GetSafeHwnd();
	bInfo.pszDisplayName = m_strPath.GetBuffer(MAX_PATH);
	bInfo.lpszTitle = m_strTitle.IsEmpty() ? _T("Open") : m_strTitle;
	bInfo.ulFlags = BIF_RETURNFSANCESTORS|BIF_RETURNONLYFSDIRS;
	bInfo.lpfn = BrowseCtrlCallback;
	bInfo.lParam = (LPARAM) this;

	if( (pidl = ::SHBrowseForFolder(&bInfo)) == NULL )
		return FALSE;

	m_strPath.ReleaseBuffer();
	m_nImageIndex = bInfo.iImage;

	if(!::SHGetPathFromIDList( pidl, m_strPath.GetBuffer(MAX_PATH)))
	{
		pMalloc->Free(pidl);
		pMalloc->Release();

		return FALSE;
	}
	m_strPath.ReleaseBuffer();

	pMalloc->Free(pidl);
	pMalloc->Release();

	return TRUE;
}

void CTShell::GetSystemImageList( CImageList *pSmallList, CImageList *pLargeList)
{
	SHFILEINFO ssfi;
	SHFILEINFO lsfi;

	HIMAGELIST hSystemSmallImageList = (HIMAGELIST) ::SHGetFileInfo(
		(LPCTSTR) _T("C:\\"),
		0, &ssfi,
		sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX|
		SHGFI_SMALLICON);

	pSmallList->Attach(hSystemSmallImageList);
	HIMAGELIST hSystemLargeImageList = (HIMAGELIST) ::SHGetFileInfo(
		(LPCTSTR)_T("C:\\"),
		0, &lsfi,
		sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX|
		SHGFI_ICON);

	pLargeList->Attach(hSystemLargeImageList);
}

CString CTShell::GetFileName( LPCTSTR lpszFileName)
{
	if( lpszFileName != NULL )
		SplitPath( lpszFileName, m_szDrive, m_szDir, m_szFname, m_szExt);

	CString str;
	str.Format( _T("%s%s"), m_szFname, m_szExt);

	return str;
}

CString CTShell::GetFileDrive( LPCTSTR lpszFileName)
{
	if( lpszFileName != NULL )
		SplitPath( lpszFileName, m_szDrive, m_szDir, m_szFname, m_szExt);

	return m_szDrive;
}

CString CTShell::GetRoot( LPCTSTR lpszFileName)
{
	if( lpszFileName != NULL )
		SplitPath( lpszFileName, m_szDrive, m_szDir, m_szFname, m_szExt);

	CString str;
	str.Format( _T("%s%s"), m_szDrive, m_szDir);

	return str;
}

CString CTShell::GetFileTitle( LPCTSTR lpszFileName)
{
	if( lpszFileName != NULL )
		SplitPath( lpszFileName, m_szDrive, m_szDir, m_szFname, m_szExt);

	return m_szFname;
}

CString CTShell::GetFileExt( LPCTSTR lpszFileName)
{
	if( lpszFileName != NULL )
		SplitPath( lpszFileName, m_szDrive, m_szDir, m_szFname, m_szExt);

	return m_szExt;
}

CString CTShell::GetFileType( LPCTSTR lpszFileName)
{
	SHFILEINFO sfi;

	::SHGetFileInfo(
		lpszFileName == NULL ? m_strFileName : lpszFileName,
		0, &sfi,
		sizeof(SHFILEINFO),
		SHGFI_TYPENAME);

	return sfi.szTypeName;
}

CString CTShell::GetDisplayName( LPCTSTR lpszFileName)
{
	SHFILEINFO sfi;

	::SHGetFileInfo(
		lpszFileName == NULL ? m_strFileName : lpszFileName,
		0, &sfi,
		sizeof(SHFILEINFO),
		SHGFI_DISPLAYNAME);

	return sfi.szDisplayName;
}

int CTShell::GetIconIndex( LPCTSTR lpszFileName, UINT nFlags)
{
	SHFILEINFO sfi;

	::SHGetFileInfo(
		lpszFileName == NULL ? m_strFileName : lpszFileName,
		0, &sfi,
		sizeof(SHFILEINFO),
		SHGFI_SYSICONINDEX|nFlags);

	return sfi.iIcon;
}

BOOL CTShell::Exist( LPCTSTR lpszFileName)
{
	WIN32_FIND_DATA fd;
	HANDLE hFind = FindFirstFile( lpszFileName == NULL ? m_strFileName : lpszFileName, &fd);

	if( hFind != INVALID_HANDLE_VALUE )
		FindClose(hFind);

	return hFind != INVALID_HANDLE_VALUE;
}

CString CTShell::GetLastWriteTime( CTime *pTime)
{
	if(!pTime)
		return _T("");

	CString strTime;
	strTime.Format(
		_T("%.2d/%.2d/%d %.2d:%.2d"),
		pTime->GetMonth(),
		pTime->GetDay(),
		pTime->GetYear(),
		pTime->GetHour(),
		pTime->GetSecond());

	return strTime;
}

CString CTShell::GetFileSizeString( LPCTSTR lpszFileSize)
{
	CString strFileSize(lpszFileSize);
	UINT nSize = _ttoi(strFileSize);

	if( nSize == 0 )
		return _T("0KB");

	nSize /= 1000;
	if( nSize == 0 )
		return _T("1KB");

	strFileSize.Format( _T("%dKB"), nSize);
	return strFileSize;
}

LPITEMIDLIST CTShell::Next( LPCITEMIDLIST pidl)
{
	LPSTR lpMem = (LPSTR) pidl;
	lpMem += pidl->mkid.cb;

	return (LPITEMIDLIST) lpMem;
}

UINT CTShell::GetSize( LPCITEMIDLIST pidl)
{
	UINT nTotal = 0;

	if(pidl)
	{
		nTotal += sizeof(pidl->mkid.cb);

		while(pidl->mkid.cb)
		{
			nTotal += pidl->mkid.cb;
			pidl = Next(pidl);
		}
	}

	return nTotal;
}

LPITEMIDLIST CTShell::Create( UINT nSize)
{
	LPMALLOC lpMalloc;
	HRESULT hr;
	LPITEMIDLIST pidl = NULL;

	hr = SHGetMalloc(&lpMalloc);
	if(FAILED(hr))
		return 0;

	pidl = (LPITEMIDLIST) lpMalloc->Alloc(nSize);
	if(pidl)
		memset( pidl, 0, nSize);

	if(lpMalloc)
		lpMalloc->Release();

	return pidl;
}

LPITEMIDLIST CTShell::ConcatPidls( LPCITEMIDLIST pidl1, LPCITEMIDLIST pidl2)
{
	LPITEMIDLIST pidlNew;
	UINT cb1;
	UINT cb2;

	if(pidl1)
		cb1 = GetSize(pidl1) - sizeof(pidl1->mkid.cb);
	else
		cb1 = 0;

	cb2 = GetSize(pidl2);
	pidlNew = Create(cb1 + cb2);

	if(pidlNew)
	{
		if(pidl1)
			memcpy( pidlNew, pidl1, cb1);

		memcpy( ((LPSTR) pidlNew) + cb1, pidl2, cb2);
	}

	return pidlNew;
}

LPITEMIDLIST CTShell::CopyITEMID( LPMALLOC lpMalloc, LPITEMIDLIST lpi)
{
	LPITEMIDLIST lpiTemp;

	lpiTemp = (LPITEMIDLIST) lpMalloc->Alloc(lpi->mkid.cb + sizeof(lpi->mkid.cb));
	CopyMemory( (PVOID) lpiTemp, (CONST VOID *) lpi, lpi->mkid.cb + sizeof(lpi->mkid.cb));

	return lpiTemp;
}

BOOL CTShell::GetName( LPSHELLFOLDER lpsf, LPITEMIDLIST lpi, DWORD dwFlags, LPTSTR lpFriendlyName)
{
	BOOL bSuccess = TRUE;
	STRRET str;

	if( NOERROR == lpsf->GetDisplayNameOf( lpi, dwFlags, &str) )
	{
		switch(str.uType)
		{
		case STRRET_WSTR	:
#if defined(_UNICODE)
			_tcscpy( lpFriendlyName, str.pOleStr);
#else
			WideCharToMultiByte(
				CP_ACP, 0,
				str.pOleStr, -1,
				lpFriendlyName,
				MAX_PATH,
				NULL,
				NULL);
#endif
			break;

		case STRRET_OFFSET	:
#if defined(_UNICODE)
			MultiByteToWideChar(
				CP_ACP,
				MB_PRECOMPOSED,
				(LPSTR) lpi + str.uOffset,
				-1, lpFriendlyName,
				MAX_PATH);
#else
			_tcscpy( lpFriendlyName, (LPSTR) lpi + str.uOffset);
#endif
			break;

		case STRRET_CSTR	:
#if defined(_UNICODE)
			MultiByteToWideChar(
				CP_ACP,
				MB_PRECOMPOSED,
				str.cStr, -1,
				lpFriendlyName,
				MAX_PATH);
#else
			_tcscpy( lpFriendlyName, (LPSTR) str.cStr);
#endif
			break;

		default				: bSuccess = FALSE; break;
		}
	}
	else
		bSuccess = FALSE;

	return bSuccess;
}

LPITEMIDLIST CTShell::GetFullyQualPidl( LPSHELLFOLDER lpsf, LPITEMIDLIST lpi)
{
	OLECHAR szOleChar[MAX_PATH];
	TCHAR szBuff[MAX_PATH];

	LPSHELLFOLDER lpsfDeskTop;
	LPITEMIDLIST lpifq;
	ULONG nAttribs;
	ULONG nEaten;
	HRESULT hr;

	if(!GetName( lpsf, lpi, SHGDN_FORPARSING, szBuff))
		return NULL;

	hr = SHGetDesktopFolder(&lpsfDeskTop);
	if(FAILED(hr))
		return NULL;

#if defined(_UNICODE)
	hr = lpsfDeskTop->ParseDisplayName(
		NULL,
		NULL,
		szOleChar,
		&nEaten,
		&lpifq,
		&nAttribs);
#else
	MultiByteToWideChar(
		CP_ACP,
		MB_PRECOMPOSED,
		szBuff, -1,
		(USHORT *) szOleChar,
		sizeof(szOleChar));

	hr = lpsfDeskTop->ParseDisplayName(
		NULL,
		NULL,
		szOleChar,
		&nEaten,
		&lpifq,
		&nAttribs);
#endif
	lpsfDeskTop->Release();

	if(FAILED(hr))
		return NULL;

	return lpifq;
}

BOOL CTShell::DoTheMenuThing( HWND hwnd,
							  LPSHELLFOLDER lpsfParent,
							  LPITEMIDLIST lpi,
							  LPPOINT lppt)
{
	CMINVOKECOMMANDINFO cmi;
	LPCONTEXTMENU lpcm;

	TCHAR szTemp[64];
	HMENU hMenu;
	HRESULT hr;

	BOOL bSuccess = TRUE;
	int nCmd;

	hr = lpsfParent->GetUIObjectOf(
		hwnd, 1,
		(const struct _ITEMIDLIST **) &lpi,
		IID_IContextMenu,
		0, (LPVOID *) &lpcm);

	if(SUCCEEDED(hr))
	{
		hMenu = CreatePopupMenu();

		if(hMenu)
		{
			hr = lpcm->QueryContextMenu(
				hMenu,
				0, 1,
				0x7fff,
				CMF_EXPLORE);

			if(SUCCEEDED(hr))
			{
				nCmd = TrackPopupMenu(
					hMenu,
					TPM_LEFTALIGN|
					TPM_RETURNCMD|
					TPM_RIGHTBUTTON,
					lppt->x,
					lppt->y,
					0, hwnd,
					NULL);

				if(nCmd)
				{
					cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
					cmi.fMask = 0;
					cmi.hwnd = hwnd;
					cmi.lpVerb = (LPCSTR) MAKEINTRESOURCE(nCmd - 1);
					cmi.lpParameters = NULL;
					cmi.lpDirectory = NULL;
					cmi.nShow = SW_SHOWNORMAL;
					cmi.dwHotKey = 0;
					cmi.hIcon = NULL;

					hr = lpcm->InvokeCommand(&cmi);
					if(!SUCCEEDED(hr))
					{
						wsprintf( szTemp, _T("InvokeCommand failed. hr = %lx"), hr);
						AfxMessageBox(szTemp);
					}
					else
					{
						::SendMessage(
							hwnd,
							WM_SHELL_NOTIFY,
							NM_SH_SHELLMENU,
							(LPARAM) 0);
					}
				}
			}
			else
				bSuccess = FALSE;

			DestroyMenu(hMenu);
		}
		else
			bSuccess = FALSE;

		lpcm->Release();
	}
	else
	{
		wsprintf( szTemp, _T("GetUIObjectOf failed! hr = %lx"), hr);
		AfxMessageBox(szTemp);
		bSuccess = FALSE;
	}

	return bSuccess;
}

int CTShell::GetIcon( LPITEMIDLIST lpi, UINT nFlags)
{
	SHFILEINFO sfi;

	SHGetFileInfo(
		(LPCTSTR)lpi,
		0, &sfi,
		sizeof(SHFILEINFO),
		nFlags);

	return sfi.iIcon;
}

void CTShell::GetNormalAndSelectedIcons( LPITEMIDLIST lpifq, LPTV_ITEM lptvitem)
{
	lptvitem->iImage = GetIcon(
		lpifq,
		SHGFI_PIDL|
		SHGFI_SYSICONINDEX|
		SHGFI_SMALLICON);

	lptvitem->iSelectedImage = GetIcon(
		lpifq,
		SHGFI_PIDL|
		SHGFI_SYSICONINDEX|
		SHGFI_SMALLICON|
		SHGFI_OPENICON);
}

LONG CTShell::GetRegKey( HKEY key, LPCTSTR subkey, LPTSTR pData)
{
	HKEY hkey;
	LONG retval = RegOpenKeyEx(
		key,
		subkey, 0,
		KEY_QUERY_VALUE,
		&hkey);

	if( retval == ERROR_SUCCESS )
	{
		long datasize = MAX_PATH;
		TCHAR data[MAX_PATH];

		RegQueryValue(
			hkey,
			NULL,
			data,
			&datasize);

		_tcscpy( pData, data);
		RegCloseKey(hkey);
	}

	return retval;
}

HINSTANCE CTShell::ShellExec( LPCTSTR lpszExec, int nShow)
{
	TCHAR key[MAX_PATH + MAX_PATH];
	HINSTANCE result = ShellExecute(
		NULL,
		_T("open"),
		lpszExec,
		NULL,
		NULL,
		nShow);

	if( (UINT) result <= HINSTANCE_ERROR )
	{
		if( GetRegKey( HKEY_CLASSES_ROOT, _T(".htm"), key) == ERROR_SUCCESS )
		{
			lstrcat( key, _T("\\shell\\open\\command"));

			if( GetRegKey( HKEY_CLASSES_ROOT, key, key) == ERROR_SUCCESS )
			{
				TCHAR *pos;
				pos = _tcsstr( key, _T("\"%1\""));

				if( pos == NULL )
				{
					pos = _tcsstr( key, _T("%1"));

					if( pos == NULL )
						pos = key + _tcslen(key) - 1;
					else
						*pos = _T('\0');
				}
				else
					*pos = _T('\0');

				lstrcat( pos, _T(" "));
				lstrcat( pos, lpszExec);

				PROCESS_INFORMATION pi;
				STARTUPINFO si;

				::ZeroMemory( &si, sizeof(si));
				si.cb = sizeof(si);

				if(!::CreateProcess(
					NULL,
					pos,
					NULL,
					NULL,
					FALSE,
					0, NULL,
					NULL,
					&si, &pi))
					return NULL;

				result = (HINSTANCE) pi.hProcess;
			}
		}
	}

	return result;
}

void CTShell::SplitPath( CString strFileName, char* szDrive, char* szDir, char* szFile, char* szExt)
{
#if defined(_UNICODE)
	char szFileName[MAX_PATH];

	::WideCharToMultiByte(
		CP_ACP, 0,
		strFileName,
		-1, szFileName,
		MAX_PATH,
		NULL,
		NULL);

	_splitpath( szFileName, szDrive, szDir, szFile, szExt);
#else
	_splitpath( strFileName, szDrive, szDir, szFile, szExt);
#endif
}

LPITEMIDLIST CTShell::DuplicateItem( LPMALLOC pMalloc, LPITEMIDLIST pidl)
{
	LPITEMIDLIST pidlNew;
	USHORT nLen;

	nLen = pidl->mkid.cb;
	if( nLen == 0 )
		return NULL;

	pidlNew = (LPITEMIDLIST) pMalloc->Alloc(nLen + sizeof(USHORT));
	if( pidlNew == NULL )
		return NULL;

	CopyMemory( pidlNew, pidl, nLen);
	*((USHORT*) (((LPBYTE) pidlNew) + nLen)) = 0;

	return pidlNew;
}

LPITEMIDLIST CTShell::GetNextItem( LPITEMIDLIST pidl)
{
	USHORT nLen;

	if( (nLen = pidl->mkid.cb) == 0 )
		return NULL;

	return (LPITEMIDLIST) (((LPBYTE) pidl) + nLen);
}

UINT CTShell::GetItemCount( LPITEMIDLIST pidl)
{
	USHORT nLen;
	UINT nCount;

	nCount = 0;
	while( (nLen = pidl->mkid.cb) != 0 )
	{
		pidl = GetNextItem(pidl);
		nCount++;
	}

	return nCount;
}

BOOL CTShell::DoExplorerMenu( HWND hwnd, LPCTSTR pszPath, POINT point)
{
	LPSHELLFOLDER psfNextFolder;
	LPSHELLFOLDER psfFolder;

	LPITEMIDLIST pidlNextItem;
	LPITEMIDLIST pidlMain;
	LPITEMIDLIST pidlItem;

	LPCONTEXTMENU pContextMenu;
	LPITEMIDLIST *ppidl;
	LPMALLOC pMalloc;

	CMINVOKECOMMANDINFO ici;
	ULONG nLength;
	ULONG nAttr;

	WCHAR wszPath[MAX_PATH];
	TCHAR szPath[MAX_PATH];
	HMENU hMenu;

	BOOL bResult;
	UINT nCount;
	UINT nCmd;

	GetFullPathName(
		pszPath,
		sizeof(szPath) / sizeof(TCHAR),
		szPath,
		NULL);

#if defined(_UNICODE)
	_tcscpy( wszPath, szPath);
#else
	MultiByteToWideChar(
		CP_ACP, 0,
		pszPath, -1,
		wszPath,
		sizeof(wszPath) / sizeof(WCHAR));
#endif
	bResult = FALSE;

	if(!SUCCEEDED(SHGetMalloc(&pMalloc)))
		return bResult;

	if(!SUCCEEDED(SHGetDesktopFolder(&psfFolder)))
	{
		pMalloc->Release();
		return bResult;
	}

	if( SUCCEEDED(psfFolder->ParseDisplayName( hwnd, NULL, wszPath, &nLength, &pidlMain, &nAttr)) && pidlMain != NULL )
	{
		nCount = GetItemCount(pidlMain);

		if(nCount)
		{
			pidlItem = pidlMain;

			while(nCount)
			{
				nCount--;
				pidlNextItem = DuplicateItem( pMalloc, pidlItem);

				if( pidlNextItem == NULL )
				{
					pMalloc->Free(pidlMain);
					psfFolder->Release();
					pMalloc->Release();

					return bResult;
				}

				if(!SUCCEEDED(psfFolder->BindToObject(
					pidlNextItem,
					NULL, IID_IShellFolder,
					(LPVOID *) &psfNextFolder)))
				{
					pMalloc->Free(pidlNextItem);
					pMalloc->Free(pidlMain);
					psfFolder->Release();
					pMalloc->Release();

					return bResult;
				}

				psfFolder->Release();
				psfFolder = psfNextFolder;

				pMalloc->Free(pidlNextItem);
				pidlItem = GetNextItem(pidlItem);
			}
			ppidl = &pidlItem;

			if( SUCCEEDED(psfFolder->GetUIObjectOf(
				hwnd, 1,
				(const struct _ITEMIDLIST **) ppidl,
				IID_IContextMenu,
				NULL, (LPVOID *) &pContextMenu)))
			{
				hMenu = CreatePopupMenu();

				if(SUCCEEDED(pContextMenu->QueryContextMenu( hMenu, 0, 1, 0x7FFF, CMF_EXPLORE)))
				{
					nCmd = TrackPopupMenu(
						hMenu,
						TPM_LEFTALIGN|
						TPM_LEFTBUTTON|
						TPM_RIGHTBUTTON|
						TPM_RETURNCMD,
						point.x,
						point.y,
						0, hwnd,
						NULL);

					if(nCmd)
					{
						ici.lpVerb = (LPCSTR) MAKEINTRESOURCE(nCmd - 1);
						ici.cbSize = sizeof(CMINVOKECOMMANDINFO);
						ici.nShow = SW_SHOWNORMAL;
						ici.lpParameters = NULL;
						ici.lpDirectory = NULL;
						ici.dwHotKey = 0;
						ici.hIcon = NULL;
						ici.hwnd = hwnd;
						ici.fMask = 0;

						if(SUCCEEDED(pContextMenu->InvokeCommand(&ici)))
							bResult = TRUE;
					}
				}

				DestroyMenu(hMenu);
				pContextMenu->Release();
			}
		}

		pMalloc->Free(pidlMain);
	}

	psfFolder->Release();
	pMalloc->Release();

	return bResult;
}

#endif // (_WIN32_IE >= 0x0400)
