// TQuestEditorView.cpp : implementation of the CTQuestEditorView class
//

#include "stdafx.h"
#include "TQuestEditor.h"

#include "TQuestEditorDoc.h"
#include "TQuestEditorView.h"
#include "TTitleSet.h"
#include ".\tquesteditorview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CTQuestEditorApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorView

IMPLEMENT_DYNCREATE(CTQuestEditorView, CTreeView)

BEGIN_MESSAGE_MAP(CTQuestEditorView, CTreeView)
	//{{AFX_MSG_MAP(CTQuestEditorView)
	ON_WM_DESTROY()
	ON_NOTIFY_REFLECT(TVN_DELETEITEM, OnDeleteitem)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CTreeView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CTreeView::OnFilePrintPreview)
	ON_COMMAND(ID_CHILD_EXPAND, OnChildTreeState)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorView construction/destruction

CTQuestEditorView::CTQuestEditorView()
{
	m_listTreeImage.Create( IDB_BITMAP_TREEIMG, 16, 0, RGB( 255, 255, 255));
	m_pCopySourceQUEST = NULL;
}

CTQuestEditorView::~CTQuestEditorView()
{
}

BOOL CTQuestEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorView drawing

void CTQuestEditorView::OnDraw(CDC* pDC)
{
	CTQuestEditorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
}

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorView printing

BOOL CTQuestEditorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CTQuestEditorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CTQuestEditorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorView diagnostics

#ifdef _DEBUG
void CTQuestEditorView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CTQuestEditorView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CTQuestEditorDoc* CTQuestEditorView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTQuestEditorDoc)));
	return (CTQuestEditorDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CTQuestEditorView message handlers

void CTQuestEditorView::OnDestroy() 
{
	CTreeCtrl& cTree = GetTreeCtrl();

	cTree.DeleteAllItems();
	m_listTreeImage.DeleteImageList();

	CTreeView::OnDestroy();
}

void CTQuestEditorView::OnInitialUpdate() 
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

	cTree.SetImageList( &m_listTreeImage, TVSIL_NORMAL);
	CTreeView::OnInitialUpdate();
}

void CTQuestEditorView::OnDeleteitem( NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*) pNMHDR;
	LPTQUEST pQUEST = (LPTQUEST) pNMTreeView->itemOld.lParam;

	if(pQUEST)
		delete pQUEST;

	*pResult = 0;
}

void CTQuestEditorView::OnEndlabeledit( NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*) pNMHDR;
	LPTQUEST pQUEST = (LPTQUEST) pTVDispInfo->item.lParam;
	CTreeCtrl& cTree = GetTreeCtrl();
	
	CString strTitle = cTree.GetItemText(pTVDispInfo->item.hItem);
	CString strNew = pTVDispInfo->item.pszText;
	
	// 상연
	if( pQUEST && !strNew.IsEmpty() && strTitle != strNew )
	{
		CTQuestEditorDoc *pDoc = GetDocument();
		CTTitleSet set(&pDoc->m_db);

		set.m_strFilter.Format( "dwQuestID = %d", pQUEST->m_dwQuestID);
		if(set.Open())
		{
			set.Edit();
			// 01/19 박상연 퀘스트 + 퀘스트 아이디
			//-------------------------------------------------------------------------
            CHAR strQuestID[255];
			pQUEST->m_strTitle = strNew; 
			sprintf( strQuestID, "%d", pQUEST->m_dwQuestID );
			set.m_szTitle = strNew;
			
			set.Update();
			set.Close();
						
			cTree.SetItemText( pTVDispInfo->item.hItem, strNew + "( " + strQuestID + " )" );
		}
	}

	*pResult = 0;
}

void CTQuestEditorView::OnSelchanged( NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*) pNMHDR;
	LPTQUEST pQUEST = (LPTQUEST) pNMTreeView->itemNew.lParam;
	CTQuestEditorDoc *pDoc = GetDocument();

	pDoc->UpdateData(pQUEST);
	*pResult = 0;
}

void CTQuestEditorView::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	*pResult = pTVDispInfo->item.lParam ? 0 : 1;

	// 01/19 박상연 퀘스트 + 퀘스트 아이디
	//-------------------------------------------------------------------------
	CTreeCtrl& cTree = GetTreeCtrl();
	LPTQUEST pQUEST = (LPTQUEST) pTVDispInfo->item.lParam;

	CEdit* pEdit = cTree.GetEditControl();
	pEdit->SetSel(0, strlen( cTree.GetItemText(pTVDispInfo->item.hItem) ));
	pEdit->ReplaceSel( pQUEST->m_strTitle );
	//-------------------------------------------------------------------------
}

void CTQuestEditorView::OnEditCopy() 
{
	CEdit *pEdit = GetTreeCtrl().GetEditControl();

	if(pEdit)
		pEdit->Copy();
	else
	{
		CTQuestEditorDoc* pDoc = this->GetDocument();
		HTREEITEM itemPos = pDoc->GetTreeCtrl()->GetSelectedItem();
		if( itemPos )
			m_pCopySourceQUEST = (LPTQUEST) pDoc->GetTreeCtrl()->GetItemData( itemPos );
		else
			m_pCopySourceQUEST = NULL;
	}
}

void CTQuestEditorView::OnEditCut() 
{
	CEdit *pEdit = GetTreeCtrl().GetEditControl();

	if(pEdit)
		pEdit->Cut();
}

void CTQuestEditorView::OnEditPaste() 
{
	CEdit *pEdit = GetTreeCtrl().GetEditControl();

	if(pEdit)
		pEdit->Paste();
	else if( m_pCopySourceQUEST )
	{
		CTQuestEditorDoc* pDoc = this->GetDocument();
		HTREEITEM itemPos = pDoc->GetTreeCtrl()->GetSelectedItem();
		if(itemPos)
		{
			LPTQUEST pQUEST = pDoc->AddQuest( m_pCopySourceQUEST );

			if( pQUEST )
			{
				for( size_t i=0 ; i < m_pCopySourceQUEST->m_vCondition.size() ; ++i )
					pDoc->AddCond( pQUEST, m_pCopySourceQUEST->m_vCondition[ i ] );

				for( size_t i=0 ; i < m_pCopySourceQUEST->m_vTerm.size() ; ++i )
					pDoc->AddTerm( pQUEST, m_pCopySourceQUEST->m_vTerm[ i ] );

				for( size_t i=0 ; i < m_pCopySourceQUEST->m_vReward.size() ; ++i )
					pDoc->AddReward( pQUEST, m_pCopySourceQUEST->m_vReward[ i ] );
			}
		}
	}
}

void CTQuestEditorView::OnEditUndo() 
{
	CEdit *pEdit = GetTreeCtrl().GetEditControl();

	if(pEdit)
		pEdit->Undo();
}

void CTQuestEditorView::OnRclick( NMHDR* pNMHDR, LRESULT* pResult) 
{
	CTQuestEditorDoc *pDoc = GetDocument();

	HTREEITEM itemPos = NULL;
	CMenu *pMenu = NULL;

	CTreeCtrl& cTree = GetTreeCtrl();
	CPoint point;

	GetCursorPos(&point);
	ScreenToClient(&point);
	*pResult = 0;

	itemPos = cTree.HitTest(point);
	ClientToScreen(&point);

	if(itemPos)
	{
		if(cTree.GetItemData(itemPos))
			pMenu = theApp.m_popup.GetSubMenu(2);
		else
			pMenu = theApp.m_popup.GetSubMenu(1);

		cTree.SelectItem(itemPos);
	}
	else if(!pDoc->m_db.IsOpen())
		pMenu = theApp.m_popup.GetSubMenu(0);

	if(pMenu)
		pMenu->TrackPopupMenu( 0, point.x, point.y, this);
}

void CTQuestEditorView::OnChildTreeState()
{
	CTreeCtrl *pTree = GetDocument()->GetTreeCtrl();
	HTREEITEM hItem = pTree->GetSelectedItem();
	
	if( pTree->GetChildItem(hItem) == pTree->GetNextVisibleItem(hItem))
	{
		pTree->Expand(hItem, TVE_COLLAPSE);
		GetDocument()->ChildCollapse(pTree->GetChildItem(hItem));	
	}
	else
	{
		pTree->Expand(hItem, TVE_EXPAND);
		GetDocument()->ChildExpand(pTree->GetChildItem(hItem));	
	}
}
