// Game StudioView.cpp : implementation of the CGameStudioView class
//

#include "stdafx.h"
#include "Game Studio.h"

#include "Game StudioDoc.h"
#include "Game StudioView.h"

#include "TextureView.h"
#include "MAPOBJBar.h"
#include "MAPSFXBar.h"
#include "MAPSNDBar.h"
#include "OBJBar.h"
#include "MainFrm.h"
#include "ItemFinder.h"
#include "ItemScreenShot.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLIPFORMAT CGameStudioView::m_cfGS = ::RegisterClipboardFormat("Tachyon Game Studio Clipboard Format");
CImageList* CGameStudioView::m_pDragImage = NULL;


// CGameStudioView

IMPLEMENT_DYNCREATE(CGameStudioView, CTreeView)

BEGIN_MESSAGE_MAP(CGameStudioView, CTreeView)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnTvnDeleteitem)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnNMDblclk)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnNMRclick)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnTvnBegindrag)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnTvnSelchanged)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnTvnEndlabeledit)
END_MESSAGE_MAP()

// CGameStudioView construction/destruction

CGameStudioView::CGameStudioView()
:	m_hCutItem(NULL)
{
	m_listTreeImages.Create(
		IDB_TREE_IMAGE,
		17, 1,
		RGB( 255, 255, 255));
}

CGameStudioView::~CGameStudioView()
{
}

BOOL CGameStudioView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CTreeView::PreCreateWindow(cs);
}

int CGameStudioView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_DropTarget.Register(this);
	return 0;
}

void CGameStudioView::OnInitialUpdate()
{
	CTreeCtrl& cTree = GetTreeCtrl();

	LONG nStyle = GetWindowLong(
		cTree.GetSafeHwnd(),
		GWL_STYLE);

	SetWindowLong(
		cTree.GetSafeHwnd(),
		GWL_STYLE,
		nStyle
		|TVS_HASLINES
		|TVS_LINESATROOT
		|TVS_HASBUTTONS
		|TVS_SHOWSELALWAYS
		|TVS_EDITLABELS);

	cTree.SetImageList( &m_listTreeImages, TVSIL_NORMAL);
	DragAcceptFiles();

	CTreeView::OnInitialUpdate();
}


// CGameStudioView diagnostics

#ifdef _DEBUG
void CGameStudioView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CGameStudioView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CGameStudioDoc* CGameStudioView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGameStudioDoc)));
	return (CGameStudioDoc*)m_pDocument;
}
#endif //_DEBUG


// CGameStudioView message handlers

void CGameStudioView::OnDestroy()
{
	m_listTreeImages.DeleteImageList();
	CTreeView::OnDestroy();
}

void CGameStudioView::OnTvnDeleteitem( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	CTreeCtrl& cTree = GetTreeCtrl();
	CItemBase *pItem = (CItemBase *) cTree.GetItemData(pNMTreeView->itemOld.hItem);

	if(pItem)
		delete pItem;

	cTree.Invalidate();
	*pResult = 0;
}

void CGameStudioView::OnNMDblclk( NMHDR *pNMHDR, LRESULT *pResult)
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	CItemBase *pItem = (CItemBase *) cTree.GetItemData(itemPos);
	CMultiDocTemplate *pDocTemplate = NULL;

	*pResult = 0;
	if(!pItem)
		return;

	if(pItem->GetMainDocument())
	{
		CFrameWnd *pWnd = pItem->GetMainDocument()->GetFrameWnd();

		if(pWnd)
		{
			pWnd->BringWindowToTop();
			*pResult = 1;

			return;
		}
	}

	switch(pItem->m_bType)
	{
	case ITEM_ANIMATION		: pDocTemplate = theApp.m_pAnimationTemplate; break;
	case ITEM_OBJTYPE		: pDocTemplate = theApp.m_pOBJTypeTemplate; break;
	case ITEM_TEXTURE		: pDocTemplate = theApp.m_pTextureTemplate; break;
	case ITEM_IMAGE			: pDocTemplate = theApp.m_pImageTemplate; break;
	case ITEM_MEDIA			: pDocTemplate = theApp.m_pMediaTemplate; break;
	case ITEM_MESH			: pDocTemplate = theApp.m_pMeshTemplate; break;
	case ITEM_OBJECT		: pDocTemplate = theApp.m_pOBJTemplate; break;
	case ITEM_SFX			: pDocTemplate = theApp.m_pSFXTemplate; break;
	case ITEM_EXTMAP		: pDocTemplate = theApp.m_pEXTMAPTemplate; break;
	case ITEM_COMP			: pDocTemplate = theApp.m_pCOMPTemplate; break;
	case ITEM_FRAME			: pDocTemplate = theApp.m_pFRAMETemplate; break;
	case ITEM_BSPMAP		:
		{
			pDocTemplate = theApp.m_pBSPMAPTemplate;
			CTachyonDoc::CloseAllBSP();
		}

		break;
	}

	if(pDocTemplate)
	{
		CTachyonDoc *pDoc = (CTachyonDoc *) pDocTemplate->CreateNewDocument();
		BOOL bAutoDelete = pDoc->m_bAutoDelete;

		pDoc->m_bAutoDelete = FALSE;
		CFrameWnd* pFrame = pDocTemplate->CreateNewFrame(
			pDoc,
			NULL);
		pDoc->InitData(pItem);

		pDoc->m_bAutoDelete = bAutoDelete;
		pDocTemplate->InitialUpdateFrame(
			pFrame,
			pDoc,
			TRUE);

		*pResult = 1;
	}
}

void CGameStudioView::OnNMRclick( NMHDR *pNMHDR, LRESULT *pResult)
{
	HTREEITEM itemPos = NULL;
	CMenu *pMenu = NULL;

	CTreeCtrl& cTree = GetTreeCtrl();
	CPoint point;

	GetCursorPos(&point);
	ScreenToClient(&point);
	*pResult = 1;

	itemPos = cTree.HitTest(point);
	ClientToScreen(&point);

	if(itemPos)
	{
		CItemBase *pItem = (CItemBase *) cTree.GetItemData(itemPos);
		cTree.SelectItem(itemPos);

		pMenu = theApp.m_menuPopup.GetSubMenu(pItem->m_bMenuID);
	}
	else
		pMenu = theApp.m_menuPopup.GetSubMenu(0);

	if(pMenu)
		TrackPopupMenu( point, pMenu);
}

void CGameStudioView::TrackPopupMenu( CPoint& point, CMenu *pMenu)
{
	CTPopupMenuWnd *pPopup = new CTPopupMenuWnd();

	pPopup->CreatePopupMenu(GetSafeHwnd());
	pPopup->UpdateFromMenu(
		GetSafeHwnd(),
		pMenu,
		FALSE);

	pPopup->TrackPopupMenu(
		TPM_RIGHTALIGN|
		TPM_LEFTBUTTON,
		point.x,
		point.y);
}

void CGameStudioView::OnEditCopy()
{
	CGameStudioDoc* pDoc = GetDocument();	
	CEdit *pEdit = GetTreeCtrl().GetEditControl();

	if(pEdit)
		pEdit->Copy();
   
	// 06/05/18 PSY
	// Copy를 위한 Src Item 저장
	/////////////////////////////////////////////////////////////
	CTreeCtrl &cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();
	CItemBase *pItem = (CItemBase *) cTree.GetItemData(itemPos);

	if( !pItem )
		return;

	pDoc->m_pSrcItem = pItem;
	/////////////////////////////////////////////////////////////


	//---------------------------08.07.15 KSH
	///	Item Copy.
	m_bCopy = TRUE;
	m_bCut = FALSE;

	//---------------------------
}

void CGameStudioView::OnEditPaste()
{
	CEdit *pEdit = GetTreeCtrl().GetEditControl();	
	if(pEdit)
		pEdit->Paste();

	// 06/05/18 PSY
	// Dest Pos에 복사된 Item 추가
	/////////////////////////////////////////////////////////////
	CGameStudioDoc* pDoc = GetDocument();
	CTreeCtrl &cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();

	if( m_bCopy )
		pDoc->AddCopyItem();

	/////////////////////////////////////////////////////////////
	
	
	//---------------------------08.07.15 KSH
	///	Item Paste.
	if( m_bCut )
	{
		CItemBase* pItemSRC = (CItemBase*)cTree.GetItemData( m_hCutItem );
		CItemBase* pItemDEST = (CItemBase*)cTree.GetItemData( itemPos );
		if( CanDrop( pItemSRC, pItemDEST ) &&
			pDoc->CanBeInsert( pItemDEST->m_itemPos, cTree, pItemSRC->m_strName ) )
		{
			//--	하위폴더로 move 막기.
			std::stack<HTREEITEM> stItem;
			stItem.push( m_hCutItem );

			while( !stItem.empty() )
			{
				HTREEITEM hCurItem = stItem.top();
				stItem.pop();

				CItemBase* pChildData = (CItemBase*)cTree.GetItemData( hCurItem );
				if( pItemDEST->m_strName == pChildData->m_strName && pChildData->m_bMenuID == 1 )
				{
					MessageBox( "하위폴더로 이동할 수 없습니다." );
					return ;
				}

				if( cTree.ItemHasChildren( hCurItem ) )
				{
					HTREEITEM hChildItem = cTree.GetChildItem( hCurItem );
					stItem.push( hChildItem );

					while(1)
					{
						HTREEITEM hNextItem = cTree.GetNextItem( hChildItem, TVGN_NEXT );
						if( hNextItem == NULL )
							break;
						stItem.push( hNextItem );
						hChildItem = hNextItem;
					}
				}
			}
			//--

			MoveTree( pItemSRC, pItemDEST );
		}
		else
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_DUPLICATEITEM);
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		}
	}
	
//	m_bCut = FALSE;

	//---------------------------
}

void CGameStudioView::OnEditCut()
{
	CEdit *pEdit = GetTreeCtrl().GetEditControl();

	if(pEdit)
		pEdit->Cut();

	//---------------------------08.07.15 KSH
	///	Item Cut.
	m_bCut = TRUE;
	m_bCopy = FALSE;

	CTreeCtrl &cTree = GetTreeCtrl();
	m_hCutItem = cTree.GetSelectedItem();

	//---------------------------
}

void CGameStudioView::OnEditUndo()
{
	CEdit *pEdit = GetTreeCtrl().GetEditControl();

	if(pEdit)
		pEdit->Undo();

	if( m_bCut )
	{
		m_bCut = FALSE;
		m_hCutItem = NULL;
	}
	if( m_bCopy )
	{
		m_bCopy = FALSE;
	}
}

void CGameStudioView::InitDragResource()
{
	if(m_pDragImage)
	{
		delete m_pDragImage;
		m_pDragImage = NULL;
	}
}

void CGameStudioView::MoveTree( CItemBase *pItemSRC, CItemBase *pItemDEST)
{
	CGameStudioDoc *pDoc = GetDocument();

	HTREEITEM itemPos = pItemSRC->m_itemPos;
	CTreeCtrl& cTree = GetTreeCtrl();

	pDoc->AddItem( pItemDEST->m_itemPos, pItemSRC);
	cTree.Expand( pItemDEST->m_itemPos, TVE_EXPAND );
	cTree.SelectItem( pItemSRC->m_itemPos );
	cTree.SetItemData( itemPos, NULL);

	HTREEITEM itemChild = cTree.GetChildItem(itemPos);
	while(itemChild)
	{
		CItemBase *pItem = (CItemBase *) cTree.GetItemData(itemChild);

		itemChild = cTree.GetNextSiblingItem(itemChild);
		MoveTree( pItem, pItemSRC);
	}

	cTree.DeleteItem(itemPos);
}

BOOL CGameStudioView::CanDrop( CItemBase *pItemSRC, CItemBase *pItemDEST)
{
	CTreeCtrl& cTree = GetTreeCtrl();

	if( pItemSRC == pItemDEST )
		return FALSE;

	if( pItemDEST->m_itemPos == cTree.GetParentItem(pItemSRC->m_itemPos) )
		return FALSE;

	int nCount = (int) pItemSRC->m_vDropTarget.size();
	for( int i=0; i<nCount; i++)
		if( pItemSRC->m_vDropTarget[i] == pItemDEST->m_bType )
			return TRUE;

	return FALSE;
}

void CGameStudioView::OnTvnBegindrag( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW> (pNMHDR);

	CTreeCtrl& cTree = GetTreeCtrl();
	CItemBase *pItem = (CItemBase *) cTree.GetItemData(pNMTreeView->itemNew.hItem);

	if( pItem && pItem->m_nDropEffect != DROPEFFECT_NONE )
	{
		HGLOBAL hGlobal = ::GlobalAlloc( GMEM_SHARE|GMEM_MOVEABLE, sizeof(GSCDATA));
		COleDataSource source;

		cTree.SelectDropTarget(pNMTreeView->itemNew.hItem);
		m_pDragImage = cTree.CreateDragImage(pNMTreeView->itemNew.hItem);
		m_pDragImage->BeginDrag( 0, CPoint(0, 0));
		m_pDragImage->DragEnter(
			GetDesktopWindow(),
			CPoint( -10000, -10000));

		LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
		pData->m_pData = (LPVOID) pItem;
		pData->m_bType = GSCT_ITEM;
		::GlobalUnlock(hGlobal);

		source.CacheGlobalData( m_cfGS, hGlobal);
		source.DoDragDrop();

		m_pDragImage->DragLeave(NULL);
		m_pDragImage->EndDrag();

		::GlobalFree(hGlobal);
		InitDragResource();
	}

	*pResult = 0;
}

DROPEFFECT CGameStudioView::OnDragEnter( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	return CTreeView::OnDragOver( pDataObject, dwKeyState, point);
}

DROPEFFECT CGameStudioView::OnDragOver( COleDataObject* pDataObject, DWORD dwKeyState, CPoint point)
{
	CTreeCtrl& cTree = GetTreeCtrl();
	CPoint screenpos = point;

	HTREEITEM itemPos = cTree.HitTest(point);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	if(itemPos)
	{
		HGLOBAL hGlobal = pDataObject->GetGlobalData(m_cfGS);
		LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

		switch(pData->m_bType)
		{
		case GSCT_ITEM		:
			{
				CItemBase *pItemDEST = (CItemBase *) cTree.GetItemData(itemPos);
				CItemBase *pItemSRC = (CItemBase *) pData->m_pData;

				if(CanDrop( pItemSRC, pItemDEST))
				{
					if( itemPos != cTree.GetDropHilightItem() )
					{
						m_pDragImage->DragMove( CPoint(
							-10000,
							-10000));

						cTree.SelectDropTarget(itemPos);
					}

					nResult = pItemSRC->m_nDropEffect;
				}
				else if(cTree.GetDropHilightItem())
					OnDragLeave();
			}

			break;
		}

		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);
	}
	else if(cTree.GetDropHilightItem())
		OnDragLeave();


	///-------------------------------- 08.07.15 KSH
	//	아이템 드래그시에 자동스크롤.
	CRect rt;
	GetClientRect( &rt );

	if( (short)point.y < rt.top + 15 )
	{
		ImageList_DragShowNolock(FALSE);
		SendMessage( WM_VSCROLL, MAKEWPARAM(SB_LINEUP, 0), NULL );
		m_pDragImage->DragMove( CPoint(-10000,-10000) );
		UpdateWindow();
		ImageList_DragShowNolock(TRUE);
		return nResult;
	}

	if( (short)point.y > rt.bottom - 15 )
	{
		ImageList_DragShowNolock(FALSE);
		SendMessage( WM_VSCROLL, MAKEWPARAM(SB_LINEDOWN, 0), NULL );
		m_pDragImage->DragMove( CPoint(-10000,-10000) );
		UpdateWindow();
		ImageList_DragShowNolock(TRUE);
		return nResult;
	}
	///--------------------------------

	ClientToScreen(&screenpos);
	m_pDragImage->DragMove(screenpos);

	return nResult;
}

void CGameStudioView::OnDragLeave()
{
	CTreeCtrl& cTree = GetTreeCtrl();

	m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));
	cTree.SelectDropTarget(NULL);

	CTreeView::OnDragLeave();
}

BOOL CGameStudioView::OnDrop( COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point)
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.HitTest(point);

	if(itemPos)
	{
		HGLOBAL hGlobal = pDataObject->GetGlobalData(m_cfGS);
		LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

		switch(pData->m_bType)
		{
		case GSCT_ITEM		:
			{
				CItemBase *pItemDEST = (CItemBase *) cTree.GetItemData(itemPos);
				CItemBase *pItemSRC = (CItemBase *) pData->m_pData;
				CGameStudioDoc *pDoc = GetDocument();

				if( CanDrop( pItemSRC, pItemDEST) && pDoc->CanBeInsert( pItemDEST->m_itemPos, cTree, pItemSRC->m_strName) )
				{
					//--	하위폴더로 move 막기.
					std::stack<HTREEITEM> stItem;
					stItem.push( pItemSRC->m_itemPos );

					while( !stItem.empty() )
					{
						HTREEITEM hCurItem = stItem.top();
						stItem.pop();

						CItemBase* pChildData = (CItemBase*)cTree.GetItemData( hCurItem );
						if( pItemDEST->m_strName == pChildData->m_strName && pChildData->m_bMenuID == 1 )
						{
							::GlobalUnlock(hGlobal);
							::GlobalFree(hGlobal);
							OnDragLeave();
							MessageBox( "하위폴더로 이동할 수 없습니다." );
							return CTreeView::OnDrop( pDataObject, dropEffect, point);
						}

						if( cTree.ItemHasChildren( hCurItem ) )
						{
							HTREEITEM hChildItem = cTree.GetChildItem( hCurItem );
							stItem.push( hChildItem );

							while(1)
							{
								HTREEITEM hNextItem = cTree.GetNextItem( hChildItem, TVGN_NEXT );
								if( hNextItem == NULL )
									break;
								stItem.push( hNextItem );
								hChildItem = hNextItem;
							}
						}
					}
					//--

					switch(dropEffect)
					{
					case DROPEFFECT_MOVE	: MoveTree( pItemSRC, pItemDEST); break;
					}
				}
				else
				{
					CString strMessage;
					CString strTitle;

					strMessage.LoadString(IDS_ERR_DUPLICATEITEM);
					strTitle.LoadString(IDR_MAINFRAME);

					MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
				}
			}

			break;
		}

		::GlobalUnlock(hGlobal);
		::GlobalFree(hGlobal);
	}

	OnDragLeave();
	return CTreeView::OnDrop( pDataObject, dropEffect, point);
}

void CGameStudioView::OnSetFocus( CWnd* pOldWnd)
{
	CTreeCtrl& cTree = GetTreeCtrl();
	HTREEITEM itemPos = cTree.GetSelectedItem();
	CItemBase *pItem = NULL;

	if(itemPos)
	{
		pItem = (CItemBase *) cTree.GetItemData(itemPos);
		if(!pItem->m_bPreviewID)
			pItem = NULL;
	}

	if(CGameStudioDoc::m_pPreview)
		CGameStudioDoc::m_pPreview->SetItem(pItem);
	if(CMainFrame::m_pScreenShotDlg)
	{
		CMainFrame::m_pScreenShotDlg->m_ctrlPreview.SetItem(pItem);
		CMainFrame::m_pScreenShotDlg->m_ctrlPreview.OnPaint();
	}
	CMAPOBJBar::SetOBJ(pItem);
	CMAPSFXBar::SetSFX(pItem);
	CMAPSNDBar::SetSND(pItem);
	COBJBar::SetDEST(pItem);

	CTreeView::OnSetFocus(pOldWnd);
}

void CGameStudioView::OnTvnSelchanged( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	CItemBase *pItem = (CItemBase *) pNMTreeView->itemNew.lParam;

	if(!pItem->m_bPreviewID)
		pItem = NULL;

	if(CGameStudioDoc::m_pPreview)
	{
		//DWORD dwPass;

		//if(FAILED(CMainFrame::m_3DDevice.m_pDevice->ValidateDevice(&dwPass)))
		//{
		//	CMainFrame::RestoreDevice();
		//}
		CGameStudioDoc::m_pPreview->SetItem(pItem);
	}
	if(CMainFrame::m_pScreenShotDlg)
	{
		CMainFrame::m_pScreenShotDlg->m_ctrlPreview.SetItem(pItem);
		CMainFrame::m_pScreenShotDlg->m_ctrlPreview.OnPaint();
	}

	CMAPOBJBar::SetOBJ(pItem);
	CMAPSFXBar::SetSFX(pItem);
	CMAPSNDBar::SetSND(pItem);
	COBJBar::SetDEST(pItem);


	///----------------------------------08.07.15
	//	StatusBar에 텍스쳐ID 출력.

	if( pItem && pItem->m_bType == ITEM_TEXTURE )
	{
		CMainFrame *pMainFrame = (CMainFrame *) AfxGetMainWnd();
		CString strText;

		strText.Format( "ID : %d", pItem->m_dwID );
		pMainFrame->SetPaneText(
			strText,
			ID_PANE_POSX);

		strText.Format( "" );
		pMainFrame->SetPaneText(
			strText,
			ID_PANE_POSY);

		strText.Format( "" );
		pMainFrame->SetPaneText(
			strText,
			ID_PANE_POSZ);
	}
	///----------------------------------08.09.01
	//	StatusBar에 SFX ID 출력.
	else if( pItem && pItem->m_bType == ITEM_SFX )
	{
		CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
		CString strText;

		strText.Format( "ID : %d", pItem->m_dwID );
		pMainFrame->SetPaneText( strText, ID_PANE_POSX );

		strText.Format( "" );
		pMainFrame->SetPaneText( strText, ID_PANE_POSY );

		strText.Format( "" );
		pMainFrame->SetPaneText( strText, ID_PANE_POSZ );
	}
	///----------------------------------10.11.04
	// StatusBar에 Media ID 출력
	else if ( pItem && pItem->m_bType == ITEM_MEDIA )
	{
		CMainFrame* pMainFrame = (CMainFrame*) AfxGetMainWnd();
		CString strText;

		strText.Format( "ID: %d", pItem->m_dwID );
		pMainFrame->SetPaneText( strText, ID_PANE_POSX );

		strText.Format( "" );
		pMainFrame->SetPaneText( strText, ID_PANE_POSY );

		strText.Format( "" );
		pMainFrame->SetPaneText( strText, ID_PANE_POSZ );
	}
	///----------------------------------

	*pResult = 0;
}

void CGameStudioView::OnTvnEndlabeledit( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	CItemBase *pItem = (CItemBase *) pTVDispInfo->item.lParam;
	CString strName(pTVDispInfo->item.pszText);

	if( pItem && strName != _T("") &&
		strName != pItem->m_strName )
	{
		CGameStudioDoc *pDoc = GetDocument();
		CTreeCtrl& cTree = GetTreeCtrl();

		if(pDoc->CanBeInsert( cTree.GetParentItem(pItem->m_itemPos), cTree, strName))
		{
			pItem->m_strName = strName;
			cTree.SetItemText(
				pItem->m_itemPos,
				pItem->m_strName);

			cTree.Invalidate();
			CTachyonDoc::UpdateAll(NULL);
		}
		else
		{
			CString strMessage;
			CString strTitle;

			strMessage.Format( IDS_ERR_DUP_NAME, pItem->m_strName);
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		}
	}

	*pResult = 0;
}

BOOL CGameStudioView::PreTranslateMessage(MSG* pMsg)
{
    switch(pMsg->message) 
    {
        case WM_KEYDOWN:
            switch(int(pMsg->wParam)) 
            {
                case VK_F2:
					{
						CTreeCtrl& cTree = GetTreeCtrl();
						HTREEITEM hItem = cTree.GetSelectedItem();
						if( hItem)
						{
							cTree.SetFocus();
							CEdit* pEdit = cTree.EditLabel(hItem);
							ASSERT(pEdit != NULL);
						}
					}
                    return TRUE;

                case VK_RETURN:
					{
						CTreeCtrl& cTree = GetTreeCtrl();
						if( cTree.GetEditControl() != NULL &&
							cTree.GetSelectedItem() != NULL )
						{
							cTree.GetEditControl()->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
							return TRUE;
						}
					}
                    break;
                case VK_ESCAPE:
					{
						m_bCut = m_bCopy = FALSE;
						m_hCutItem = NULL;
						
						CTreeCtrl& cTree = GetTreeCtrl();
						if( cTree.GetEditControl() != NULL &&
							cTree.GetSelectedItem() != NULL )
						{
							CString szItemText = cTree.GetItemText(cTree.GetSelectedItem());
							cTree.GetEditControl()->SetWindowText(szItemText);
							cTree.GetEditControl()->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
							return TRUE;
						}
					}
					break;
			}
			break;
	}

	return CTreeView::PreTranslateMessage(pMsg);
}
