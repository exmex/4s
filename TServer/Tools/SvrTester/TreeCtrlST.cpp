// TreeCtrlST.cpp : 구현 파일입니다.
//

#include "stdafx.h"

#include "TreeCtrlST.h"
#include ".\treectrlst.h"

#include "SvrTester.h"
#include "MainFrm.h"


// CTreeCtrlST

IMPLEMENT_DYNAMIC(CTreeCtrlST, CTreeCtrl)
CTreeCtrlST::CTreeCtrlST()
{
	m_nID = 0;
	m_TreeCnt = 0;
	m_bEdit = FALSE;
	m_bImage = FALSE;
}

CTreeCtrlST::~CTreeCtrlST()
{
	m_nID = 0;
	m_TreeCnt = 0;
	m_bEdit = FALSE;

	if( m_bImage )
	{
		delete m_pImageList;
        m_bImage = FALSE;
	}
}


BEGIN_MESSAGE_MAP(CTreeCtrlST, CTreeCtrl)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnTvnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnTvnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_CONTEXTTREEMENU_INSERT, OnContextmenuInsert)
	ON_COMMAND(ID_CONTEXTTREEMENU_CHILDINSERT, OnContextmenuChildinsert)
	ON_COMMAND(ID_CONTEXTTREEMENU_DELETE, OnContextmenuDelete)
	ON_COMMAND(ID_CONTEXTMENU_INSERTINIT, OnContextmenuInsertinit)
	ON_COMMAND(ID_CONTEXTMENU_DELETEALL, OnContextmenuDeleteall)
END_MESSAGE_MAP()


// CTreeCtrlST 메시지 처리기입니다.

///////////////////////////////////////////////////////////////////////
// Init TreeCtrl
void CTreeCtrlST::InitTreeCtrl(int nID)
{
	m_bImage = FALSE;
	m_nID = nID;	
}
void CTreeCtrlST::InitTreeCtrl(int nID, int size, int iImage, int iSelectedImage, int iChangeImage)
{
	m_bImage = TRUE;
	SetImage(size, iImage, iSelectedImage, iChangeImage);

	m_nID = nID;	
}
void CTreeCtrlST::SetImage(int size, int iImage, int iSelectedImage, int iChangeImage)
{
	if( !m_bImage )
        return;

	m_pImageList= new CImageList;
    m_pImageList->Create(size, size, ILC_COLOR4 | ILC_MASK, 2, 2); 
	m_pImageList->Add(AfxGetApp()->LoadIcon(iImage)); 
	m_pImageList->Add(AfxGetApp()->LoadIcon(iSelectedImage));
	if(iChangeImage != 0)
		m_pImageList->Add(AfxGetApp()->LoadIcon(iChangeImage));
	SetImageList(m_pImageList, TVSIL_NORMAL);
}
// Change Image
void CTreeCtrlST::ChangeImage(int iImage, int iSelectedImage)
{
	if( !m_bImage )
        return;

    HTREEITEM hTreeItem = GetSelectedItem();	
	SetItemImage(hTreeItem, iImage, iSelectedImage); 
}

// Insert Tree Item
HTREEITEM CTreeCtrlST::InsertTreeItem(CString strText, HTREEITEM hParent)
{
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = hParent;
    tvInsert.hInsertAfter = NULL;
	if( m_bImage )
	{
		tvInsert.item.iImage = 0; 
		tvInsert.item.iSelectedImage = 1; 
		tvInsert.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT; 
	}
	else
	{
		tvInsert.item.mask = TVIF_TEXT;
	}	
	tvInsert.item.pszText = strText.GetBuffer(0);

	HTREEITEM hItem = InsertItem(&tvInsert);	
	SetItemData(hItem, m_TreeCnt);	

	m_TreeCnt++;

	return hItem;
}
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// RButton Context Menu
void CTreeCtrlST::OnNMRclick(NMHDR *pNMHDR, LRESULT *pResult)
{	
	SendMessage(WM_CONTEXTMENU, (WPARAM) m_hWnd, GetMessagePos()); 

	*pResult = 0;	
}
void CTreeCtrlST::OnContextMenu(CWnd* pWnd, CPoint point)
{
	CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();
	if(pFrm->GetEnableManagerConnect())
		return;

    ScreenToClient(&point);

    UINT uFlags;
    HTREEITEM hTreeItem;    

    hTreeItem = HitTest( point, &uFlags );     

	CMenu menu;
    CMenu* pPopup;

    // the font popup is stored in a resource
    menu.LoadMenu(IDR_MENU_TREE);

    if( hTreeItem == NULL )
	{		
		pPopup = menu.GetSubMenu(0); 		
	}
	else
	{
		SelectItem(hTreeItem);
		pPopup = menu.GetSubMenu(1); 
	}	

    ClientToScreen(&point); 
    pPopup->TrackPopupMenu( TPM_LEFTALIGN, point.x, point.y, this ); 
}

// Context Menu Insert Init / Delete All
void CTreeCtrlST::OnContextmenuInsertinit()
{	
	if( GetCount() != 0 )
	{
		AfxMessageBox(_T("아이탬을 선택해 주세요"));
		return;
	}

	CString strText;
	strText.Format(_T("<New Item %ld>"), m_TreeCnt);

	switch(m_nID)
	{
	case TMACROTABTREE:
		{	
			CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
			pFrm->GetTabGroup()->GetTMacroTabView()->InsertMacroMap(m_TreeCnt, strText, _T(""));

			HTREEITEM hItem = InsertTreeItem(strText, NULL);			
			SelectItem(hItem);
		}
		break;
	case TMACROVIEWTREE:
		{
			CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
			if( !pApp->GetTMacroView()->InsertPInfoVector(strText, _T("")) )
				return;

			HTREEITEM hItem = InsertTreeItem(strText, NULL);			
			SelectItem(hItem);
		}
		break;
	}	
}
void CTreeCtrlST::OnContextmenuDeleteall()
{
	DeleteAllItems();
	m_TreeCnt = 0;

	switch(m_nID)
	{
	case TMACROTABTREE:
		{
			CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
			pFrm->GetTabGroup()->GetTMacroTabView()->ClearMacroView();
			pFrm->GetTabGroup()->GetTMacroTabView()->DeleteAllMacroMap();			
		}
		break;		
	case TMACROVIEWTREE:
		{
			CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
			pApp->GetTMacroView()->DeleteAllPInfoVector();
		}
		break;
	}	
}

// Context Tree Menu Insert / Delete
void CTreeCtrlST::OnContextmenuInsert()
{
	CString strText;
	strText.Format(_T("<New Item %ld>"), m_TreeCnt);
    HTREEITEM hTreeItem = GetSelectedItem();
	hTreeItem = GetParentItem(hTreeItem);		

	switch(m_nID)
	{
	case TMACROTABTREE:
		{
			CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
			pFrm->GetTabGroup()->GetTMacroTabView()->InsertMacroMap(m_TreeCnt, strText, GetItemText(hTreeItem));

			HTREEITEM hItem = InsertTreeItem(strText, hTreeItem);		
			SelectItem(hItem);
		}
		break;
	case TMACROVIEWTREE:
		{
			CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
			if( !pApp->GetTMacroView()->InsertPInfoVector(strText, GetItemText(hTreeItem)) )
				return;			

			HTREEITEM hItem = InsertTreeItem(strText, hTreeItem);
			SelectItem(hItem);
		}
		break;
	}	
}
void CTreeCtrlST::OnContextmenuChildinsert()
{
	CString strText;
	strText.Format(_T("<New Child Item %ld>"), m_TreeCnt);
	HTREEITEM hTreeItem = GetSelectedItem();	

	switch(m_nID)
	{
	case TMACROTABTREE:
		{
			CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
			pFrm->GetTabGroup()->GetTMacroTabView()->InsertMacroMap(m_TreeCnt, strText, GetItemText(hTreeItem));

			HTREEITEM hItem = InsertTreeItem(strText, hTreeItem);			
			SelectItem(hItem);
		}
		break;
	case TMACROVIEWTREE:
		{
			CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
			if( !pApp->GetTMacroView()->InsertPInfoVector(strText, GetItemText(hTreeItem)) )
				return;

            HTREEITEM hItem = InsertTreeItem(strText, hTreeItem);
			SelectItem(hItem);
		}
		break;
	}	
}
void CTreeCtrlST::OnContextmenuDelete()
{	
	HTREEITEM hTreeItem = GetSelectedItem();

	switch(m_nID)
	{
	case TMACROTABTREE:
		{
			CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();
			pFrm->GetTabGroup()->GetTMacroTabView()->DeleteMacroMap(hTreeItem);
		}
		break;
	case TMACROVIEWTREE:
		{
			CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
			pApp->GetTMacroView()->DeletePInfoVector(GetItemText(hTreeItem));
		}
		break;
	}	
	
	DeleteItem(hTreeItem);
}
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
// Label Edit
void CTreeCtrlST::OnTvnBeginlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);	
	m_bEdit = TRUE;
	*pResult = 0;
}
void CTreeCtrlST::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	m_bEdit = FALSE;
	*pResult = 0;
}
BOOL CTreeCtrlST::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN )
	{
		if(m_bEdit)
		{
			CEdit *pEdit = GetEditControl();
			CString str;
			pEdit->GetWindowText(str);
			CString sTrim = str.Trim(_T("\t "));
			if(str.GetLength() > 0 && sTrim.GetLength() > 0)
			{
				HTREEITEM hTreeItem = GetSelectedItem();

				switch(m_nID)
				{
				case TMACROTABTREE:
					{
						CMainFrame *pFrm = (CMainFrame *)AfxGetMainWnd();		
						if( pFrm->GetTabGroup()->GetTMacroTabView()->UpdateMacroMap(GetItemText(hTreeItem), str) )								
							SetItemText(hTreeItem, str);				
						else
							AfxMessageBox(_T("이미 같은 이름이 존재합니다."));
					}
					break;
				case TMACROVIEWTREE:
					{
						CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
						if( pApp->GetTMacroView()->UpdatePInfoVector(GetItemText(hTreeItem), str) )
							SetItemText(hTreeItem, str);
						else
							AfxMessageBox(_T("이미 같은 이름이 존재합니다."));
					}
					break;
				}				
				SelectItem(hTreeItem);				
			}
			SetFocus();
			m_bEdit = FALSE;
			return TRUE;
		}
	}
	if( pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE )	
	{
		if(m_bEdit)
		{
			SetFocus();
			m_bEdit = FALSE;
			return TRUE;
		}
	}

	return CTreeCtrl::PreTranslateMessage(pMsg);
}
///////////////////////////////////////////////////////////////////////