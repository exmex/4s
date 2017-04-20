// SerchInstallTree.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "SerchInstallTree.h"
#include ".\serchinstalltree.h"

//#include "stdafx.h"
//#include "Happy.h"
//#include "Shlwapi.h"
//#include ".\autopatch.h"

// CSerchInstallTree

IMPLEMENT_DYNCREATE(CSerchInstallTree, CFormView)

CSerchInstallTree::CSerchInstallTree()
	: CFormView(CSerchInstallTree::IDD)
{
}

CSerchInstallTree::~CSerchInstallTree()
{
}

void CSerchInstallTree::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_INSTALL, m_treectrl);
	DDX_Control(pDX, IDC_LC_FILE, m_Listctrl);
}

BEGIN_MESSAGE_MAP(CSerchInstallTree, CFormView)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE_INSTALL, OnTvnItemexpandingTree)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_INSTALL, OnTvnSelchangedTreeInstall)
END_MESSAGE_MAP()


// CSerchInstallTree 진단입니다.

#ifdef _DEBUG
void CSerchInstallTree::AssertValid() const
{
	CFormView::AssertValid();
}

void CSerchInstallTree::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CSerchInstallTree 메시지 처리기입니다.

void CSerchInstallTree::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (GetSafeHwnd())
	{
		CRect rect,rect2;
		GetClientRect(&rect);
		rect2 = rect;
		rect.bottom = rect.bottom/2;
		rect2.top = rect.bottom;

		if (m_treectrl.GetSafeHwnd())
			m_treectrl.MoveWindow(&rect);

		if (m_Listctrl.GetSafeHwnd())
			m_Listctrl.MoveWindow(&rect2);

		ShowWindow(SW_SHOW);
		
	}
}

void CSerchInstallTree::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	static BYTE i=FALSE;
	if(!i)
	{
		SetImageList();
		SetTreeNodes();
		i=TRUE;
		//m_treectrl.SetBkColor( RGB( 200, 200, 200 ) );
	}
}
BOOL CSerchInstallTree::IsDirectory(CString strPath)
//폴더나 드라이브 인지 알아보는 함수
{
	CFileStatus fStatus;
	CFile::GetStatus(strPath,fStatus);
	
	if(IsDrive(strPath))
		return TRUE;

	if(fStatus.m_attribute & 0x10)
		return TRUE;
	else
		return FALSE;
	
}
BOOL CSerchInstallTree::IsDrive(CTreeCtrl * pTreeCtrl, HTREEITEM hItem)
{
	CString strPath=GetItemPath(pTreeCtrl, hItem);
	if(strPath.GetLength()>3)
		return FALSE;

	return TRUE;
}

BOOL CSerchInstallTree::IsDrive(CString strPath)
{
	CString strDriveLetter;

	strDriveLetter=PickDriveLetter(strPath);

	if(strDriveLetter.GetLength()>3)
		return FALSE;

	return TRUE;
}
HTREEITEM CSerchInstallTree::AddTreeItem(CTreeCtrl * pTreeCtrl, CString strPath)
{
	CString strDir,strTitle;
	HTREEITEM hItem,hFound;
	
	if(!IsDirectory(strPath))//폴더가 아니면 그냥 지나가고
		return NULL;

	strDir=GetDir(strPath);//부모폴더의 경로를 찾아서
	ProperPath(strDir);
	hFound=FindItem(pTreeCtrl, strDir);//해당 부모 아이템을 찾음
	if(hFound)//있으면
	{
		strTitle=GetFileTitle(strPath);
		hItem=pTreeCtrl->InsertItem(strTitle,CLOSED_FOLDER,OPEN_FOLDER,hFound);
		return hItem;//자식 아이템으로 넣음
	}
	return NULL;
}
HTREEITEM CSerchInstallTree::AddTreeItem(CTreeCtrl * pTreeCtrl, HTREEITEM hParent,LPCTSTR szText,int nIcon,int nSelectedIcon)
{
	//트리 아이템을 올리는 함수

	TV_INSERTSTRUCT tvStruct;

	tvStruct.hParent=hParent;
	tvStruct.hInsertAfter=TVI_LAST;
	tvStruct.item.iImage=nIcon;
	tvStruct.item.iSelectedImage=nSelectedIcon;
	tvStruct.item.cchTextMax=::lstrlen(szText);
	tvStruct.item.pszText=(LPSTR)szText;
	tvStruct.item.mask=TVIF_TEXT|TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_PARAM;

	return pTreeCtrl->InsertItem(&tvStruct);
}
void CSerchInstallTree::AddChildFolderItem(CTreeCtrl * pTreeCtrl, HTREEITEM hParent)
//하부 폴더를 찾아서 트리뷰에 노드를 올리는 부분
{
	HTREEITEM hItem;
	CFileFind finder;
	CString strPath;

	strPath=GetItemPath(pTreeCtrl, hParent);
	ProperPath(strPath);
	strPath+="*.*";

	if(finder.FindFile(strPath))
	{
		BOOL bFlag=TRUE;
		while(bFlag)
		{
			bFlag=finder.FindNextFile();
			if(finder.IsDirectory() && !finder.IsDots())
			{
				hItem=AddTreeItem(pTreeCtrl, hParent,finder.GetFileTitle(),CLOSED_FOLDER,OPEN_FOLDER);
				SetButtonState(pTreeCtrl, hItem);
			}
		}
	}
	finder.Close();
}

void CSerchInstallTree::RefrashFile(CListCtrl * pListCtrl, CString strPath)
{
	if(!strPath.GetLength())
		return;

	pListCtrl->DeleteAllItems();

	CFileFind finder;
	strPath+="\\*.*";
	if(finder.FindFile(strPath))
	{
		int i = 0;
		BOOL bFlag=TRUE;
		while(bFlag)
		{
			bFlag=finder.FindNextFile();
			if(!finder.IsDirectory())// && !finder.IsDots())
			{
				pListCtrl->InsertItem(i,finder.GetFileName());
				//hItem=AddTreeItem(pTreeCtrl, hParent,finder.GetFileTitle(),CLOSED_FOLDER,OPEN_FOLDER);
				//SetButtonState(pTreeCtrl, hItem);
			}
		}
	}
}

CString CSerchInstallTree::GetFileTitle(CString strPath)
//파일의 전체경로중에서 이름만 추출하는 함수
{
	CString strTitle;
	if(strPath.GetLength()==3)
		return strPath;

	if(strPath.Right(1)=="\\")
		strPath=strPath.Left(strPath.GetLength()-1);

	int nPos=strPath.ReverseFind('\\');
	strTitle=strPath.Mid(nPos+1);
	
	return strTitle;
}
void CSerchInstallTree::ProperPath(CString &strPath)
{
	if(strPath.Right(1)!="\\")
		strPath+="\\";
}
HICON CSerchInstallTree::GetIconHandle(int nIndex)
//Shell32.dll에서 해당 아이콘의 핸들을 얻어내는 함수
{
	CString strPath;

	strPath=GetSystemDir();//윈도우 시스템 디렉토리를 구하고
	ProperPath(strPath);
	strPath=strPath+"Shell32.dll";//Shell32.dll파일의 아이콘을 추출

	HICON hIconSmall,hIconLarge;

	::ExtractIconEx(strPath,nIndex,&hIconLarge,&hIconSmall,1);
	
	return hIconSmall;//작은 아이콘을 반환
}
CString CSerchInstallTree::GetSystemDir() const
//윈도우즈 시스템 폴더의 경로를 반환
{
	TCHAR szPath[MAX_PATH];
	::GetSystemDirectory(szPath,MAX_PATH);

	return szPath;
}
void CSerchInstallTree::SetImageList()//이미지 리스트 초기화
//Shell32.dll에서 아이콘을 추출해서 이미기 리스트에 넣는 함수
{
	m_imageList.Create(16,16,ILC_COLOR8|ILC_MASK,1,1);
	HICON hIcon;

	hIcon=GetIconHandle(3);
	m_imageList.Add(hIcon);//CLOSED_FOLDER
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(4);
	m_imageList.Add(hIcon);//OPEN_FOLDER
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(6);
	m_imageList.Add(hIcon);//FLOPPY_DRIVE
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(8);
	m_imageList.Add(hIcon);//FIXED_DRIVE
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(9);
	m_imageList.Add(hIcon);//NET_DRIVE
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(11);
	m_imageList.Add(hIcon);//CDROM_DRIVE
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(15);
	m_imageList.Add(hIcon);//MY_COM
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(32);
	m_imageList.Add(hIcon);//RECYCLE_BIN
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(34);
	m_imageList.Add(hIcon);//DESKTOP
	::DestroyIcon(hIcon);
	hIcon=GetIconHandle(35);
	m_imageList.Add(hIcon);//CONTROL_PANEL
	::DestroyIcon(hIcon);
	m_treectrl.SetImageList(&m_imageList,TVSIL_NORMAL);
	m_imageList.Detach();
}
void CSerchInstallTree::SetTreeNodes()//최초로 트리뷰에 노드를 올리는 부분
{
	LPSHELLFOLDER lpsf = NULL;
	LPITEMIDLIST lpi_MyComputer = NULL;
	TCHAR pszName[MAX_PATH];
	SHGetDesktopFolder(&lpsf);
	SHGetSpecialFolderLocation(NULL, CSIDL_DRIVES, &lpi_MyComputer);

	LPMALLOC pMalloc;
	SHGetMalloc(&pMalloc);
	STRRET str;
	HRESULT hr;
	hr = lpsf->GetDisplayNameOf(lpi_MyComputer, SHGDN_NORMAL, &str);

	if (SUCCEEDED(hr))
	{
		hr = StrRetToBuf(&str, lpi_MyComputer, pszName, MAX_PATH);
		if (SUCCEEDED(hr))
		{
			if (str.uType == STRRET_WSTR) // 이 if 문의 용도는 잘 모르겠음.
				pMalloc->Free(str.pOleStr);
		}
	}

	if (FAILED(hr))
		_tcscpy(pszName, _T("My Computer"));

	lpsf->Release();
	pMalloc->Free(lpi_MyComputer);

	m_strMyComputer = pszName;

	HTREEITEM hItem;

	hItem=AddTreeItem(&m_treectrl, NULL,m_strMyComputer,MY_COM,MY_COM);
	SetDrives(&m_treectrl, hItem);//드라이브를 검색해서 올림
}

void CSerchInstallTree::SetDrives(CTreeCtrl * pTreeCtrl, HTREEITEM& hParent)//드라이브를 검색하는 부분
{
	int nIndex=0;
	DWORD dwDrives;
	char cDrive;
	CString strDrive;
	CString strVolumeName;
	int nIcon;
	SHFILEINFO shInfo;
	HTREEITEM hItem;

	dwDrives=::GetLogicalDrives();//현재 존재하는 드라이브의 상태를 비트단위로 표시

	while(dwDrives)
	{
		if (dwDrives & 1)
		{
			cDrive='A'+nIndex;
			strDrive=CString(cDrive)+":\\";
			nIcon=GetDriveIcon(strDrive);
			SHGetFileInfo(strDrive,0,&shInfo,sizeof(SHFILEINFO),SHGFI_TYPENAME);
			strVolumeName=GetVolumeName(strDrive,shInfo.szTypeName);//드리이브의 볼륨이름을 구함
			hItem=AddTreeItem(pTreeCtrl, hParent,strVolumeName,nIcon,nIcon);
			SetButtonState(pTreeCtrl, hItem);
		}
		nIndex++;
		dwDrives>>=1;
	}
}
void CSerchInstallTree::SetButtonState( CTreeCtrl * pTreeCtrl, HTREEITEM hItem)//하부 디렉토리가 있는지 
//알아내서 노드의 상태를 표시함
{
	CFileFind finder;
	CString strPath;

	strPath=GetItemPath(pTreeCtrl, hItem);
	ProperPath(strPath);
	strPath+="*.*";
	if(finder.FindFile(strPath)==TRUE)
	{
		BOOL bFind=TRUE;
		while(bFind)
		{
			bFind=finder.FindNextFile();
			if(finder.IsDirectory() && !finder.IsDots())
			{
				AddTreeItem(pTreeCtrl, hItem,"",CLOSED_FOLDER,OPEN_FOLDER);//공란의 더미 아이템을 만듬
				break;
			}
		}
	}
	finder.Close();
}
int CSerchInstallTree::GetDriveIcon(CString& strDrive )//드라이브 아이콘인덱스를 구함
{
	int nType,nIcon;

	nType=::GetDriveType((LPCTSTR)strDrive);

	switch(nType)
	{
	case DRIVE_REMOVABLE:
		nIcon=FLOPPY_DRIVE;
		break;
	case DRIVE_REMOTE:
	case DRIVE_RAMDISK:
		nIcon=NET_DRIVE;
		break;
	case DRIVE_FIXED:
		nIcon=FIXED_DRIVE;
		break;
	case DRIVE_CDROM:
		nIcon=CDROM_DRIVE;
	}
	return nIcon;
}

CString CSerchInstallTree::GetItemPath(CTreeCtrl * pTreeCtrl, HTREEITEM hItem)//트리아이템의 경로를 구함
{
	HTREEITEM hParent;
	CString strTitle;
	CString strPath;
	CString strParent;
	CString strDrive;
	int nPos;

	strTitle=pTreeCtrl->GetItemText(hItem);
	
	if(strTitle.Find(":")!=-1)
	{
		strPath=PickDriveLetter(strTitle);//드라이브 경로
		return strPath;
	}
	
	if(strTitle == m_strMyComputer)
	{
		strPath="";
		return strPath;
	}

	hParent=pTreeCtrl->GetParentItem(hItem);
	strPath=pTreeCtrl->GetItemText(hItem);

	while(hParent && (pTreeCtrl->GetItemText(hParent)!= m_strMyComputer ))
	{
		strParent=pTreeCtrl->GetItemText(hParent);

		if ((nPos=strParent.Find(":"))!=-1)
			strParent=strParent.Mid(nPos-1,2);

		ProperPath(strParent);
		strPath=strParent+strPath;
		hItem=hParent;
		hParent=pTreeCtrl->GetParentItem(hItem);
	}
	return strPath;
}
CString CSerchInstallTree::GetSpecialFolderPath(int nFolder)
//특수폴더의 경로를 반환
{
	TCHAR szPath[MAX_PATH];

	if(::SHGetSpecialFolderPath(0,szPath,nFolder,FALSE))
		return szPath;
	return "";
}
CString CSerchInstallTree::PickDriveLetter(CString strVolumeName)
//볼륨 이름에서 드라이브 이름만 추출하는 함수
{
	CString strDriveLetter;
	int nPos=strVolumeName.Find(":");

	strDriveLetter=strVolumeName.Mid(nPos-1,2);
	strDriveLetter+="\\";

	return strDriveLetter;
}
CString CSerchInstallTree::GetDir(CString strPath)
//해당 경로에서 부모 폴더의 경로를 반환함
{
	CString strDir=strPath;
	if(strDir.Right(1)=="\\")
		strDir=strDir.Left(strDir.GetLength()-1);
	int nPos=strDir.ReverseFind('\\');
	strDir=strDir.Left(nPos);

	return strDir;
}
HTREEITEM CSerchInstallTree::FindItem(CTreeCtrl * pTreeCtrl, CString strPath)
//해당 경로를 가지고 있는 아이템을 찾음
{
	CString strTreePath;

	HTREEITEM hItem=pTreeCtrl->GetFirstVisibleItem();

	while(hItem)
	{
		strTreePath=GetItemPath(pTreeCtrl, hItem);
		ProperPath(strTreePath);
		ProperPath(strPath);
		if(strTreePath==strPath)
			return hItem;
		hItem=pTreeCtrl->GetNextVisibleItem(hItem);
	}
	return NULL;
}
CString CSerchInstallTree::GetVolumeName(CString  strDrive,CString strTypeName)
//드라이브의 볼륨 이름을 반환
{
	TCHAR szVolumeName[MAX_PATH]={0};
	CString strFullName;

	::GetVolumeInformation(strDrive,szVolumeName,MAX_PATH,0,0,0,0,0);
	int nDrive=lstrlen(szVolumeName);

	if(nDrive>0)
		strFullName=szVolumeName;
	else
		strFullName=strTypeName;
	strFullName+=" ("+strDrive+")";

	return strFullName;
}
void CSerchInstallTree::DeleteAllChildren(CTreeCtrl * pTreeCtrl, HTREEITEM hItem)
//하부노드를 모두 삭제하는 함수
{
	HTREEITEM hChildItem,hNextItem;

	if(pTreeCtrl->ItemHasChildren(hItem)==FALSE)
		return;

	hChildItem=pTreeCtrl->GetChildItem(hItem);
	while(hChildItem && (hNextItem=pTreeCtrl->GetNextSiblingItem(hChildItem)))
	{
		pTreeCtrl->DeleteItem(hChildItem);
		hChildItem=hNextItem;
	}
	pTreeCtrl->DeleteItem(hChildItem);
}

void CSerchInstallTree::OnTvnItemexpandingTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hItem=pNMTreeView->itemNew.hItem;
	HTREEITEM hParent;

	if(pNMTreeView->action=TVE_EXPAND)//펼칠때
	{
		hParent=m_treectrl.GetParentItem(hItem);
		if(hParent)
		{
			DeleteAllChildren(&m_treectrl, hItem);//자식 아이템 다 지우고(더미아이템 지우기)
			AddChildFolderItem(&m_treectrl, hItem);//새로 자식 아이템을 올림
		}
	}
	else//닫을 때
	{
		DeleteAllChildren(&m_treectrl, hItem);
		SetButtonState(&m_treectrl, hItem);
	}
	m_treectrl.SelectItem(hItem);

	*pResult = 0;
}

void CSerchInstallTree::OnNMKillfocusTreePrevfile(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hItem=pNMTreeView->itemOld.hItem;

	*pResult = 0;
}

BYTE CSerchInstallTree::OpenInstallFiles()
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	m_strFolder = GetItemPath(&m_treectrl, m_treectrl.GetSelectedItem());

	if(!m_strFolder.IsEmpty())
	{
		CHappyApp *pApp = (CHappyApp *)AfxGetApp();
		pApp->BeginWaitCursor();

		WIN32_FIND_DATA FileData;
		if(SetCurrentDirectory(m_strFolder))
		{
			HANDLE hSearch = FindFirstFile(_T("*"), &FileData);
			if(hSearch!=INVALID_HANDLE_VALUE)
			{
				do
				{
					if(FileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
						continue;

					TINSTALL tFile;
					tFile.m_dwVal = FileData.nFileSizeLow;
					tFile.m_strName = FileData.cFileName;

					pDoc->AddInstallFile(tFile);
					pDoc->m_dwInstallSize+= FileData.nFileSizeLow;

				}while(FindNextFile(hSearch, &FileData) && !pDoc->m_bSameFileExist);
				FindClose(hSearch);
			}
		}

		pApp->EndWaitCursor();
		return TRUE;
	}
	return FALSE;
}
void CSerchInstallTree::OnTvnSelchangedTreeInstall(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	HTREEITEM hItem=pNMTreeView->itemNew.hItem;
	RefrashFile(&m_Listctrl, GetItemPath(&m_treectrl, hItem));
}
