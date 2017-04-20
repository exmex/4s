// Machine.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"

#include "MainFrm.h"
#include "HappyDoc.h"
#include "HappyView.h"
#include ".\machine.h"


// CMachine

IMPLEMENT_DYNCREATE(CMachine, CFormView)

CMachine::CMachine()
	: CFormView(CMachine::IDD)
{
}

CMachine::~CMachine()
{
}

void CMachine::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_MACHINE, m_treeCtrl);
}

BEGIN_MESSAGE_MAP(CMachine, CFormView)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_MACHINE, OnTvnSelchangedTreeMachine)
END_MESSAGE_MAP()


// CMachine 진단입니다.

#ifdef _DEBUG
void CMachine::AssertValid() const
{
	CFormView::AssertValid();
}

void CMachine::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CMachine 메시지 처리기입니다.

void CMachine::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		if (m_treeCtrl.GetSafeHwnd())
		{
			m_treeCtrl.MoveWindow(&rect);
			ShowWindow(SW_SHOW);
		}
	}
	GetClientRect(&treeRect); // 현승룡 Graph
}

void CMachine::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	m_strSelected = _T("");
}

void CMachine::OnDestroy()
{
	CFormView::OnDestroy();

	CImageList * pImageList = m_treeCtrl.GetImageList(TVSIL_NORMAL);
	if(pImageList)
		delete pImageList;
}

void CMachine::InsertInitData()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	m_treeCtrl.DeleteAllItems();

	CImageList * pImageList = m_treeCtrl.GetImageList(TVSIL_NORMAL);
	if(!pImageList)
	{
		pImageList = new CImageList();
		pImageList->Create(16,16,ILC_COLOR4,3,3);
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_GROUP_NOMAL));
		pImageList->Add(AfxGetApp()->LoadIcon(IDR_GROUP_SELECT));
	}
	m_treeCtrl.SetImageList(pImageList, TVSIL_NORMAL);

	TCHAR szText[50];
	TVINSERTSTRUCT tv;
	tv.hInsertAfter = TVI_LAST;
	tv.item.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM;

	MAPMACHINE::iterator itS;
	for(itS=pDoc->m_mapMachine.begin(); itS!=pDoc->m_mapMachine.end(); itS++)
	{
		LPHAPPYMACHINE pMachine = (*itS).second;
		tv.hParent = 0;
		lstrcpy(szText, pMachine->m_strName);
		tv.item.pszText = szText;
		tv.item.iImage = 0;
		tv.item.iSelectedImage = 1;
		m_treeCtrl.InsertItem(&tv);
	}
}

void CMachine::ClearMachineTree()
{
	m_treeCtrl.DeleteAllItems();
	UpdateWindow();

	m_strSelected = _T("");
}

BYTE CMachine::GetSelectedMachineID()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return 0;
	}

	MAPMACHINE::iterator it;
	for(it=pDoc->m_mapMachine.begin(); it!=pDoc->m_mapMachine.end(); it++)
	{
		if(m_strSelected == (*it).second->m_strName)
			return (*it).second->m_bID;
	}

	return 0;
}
void CMachine::OnTvnSelchangedTreeMachine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	HTREEITEM hItem = pNMTreeView->itemNew.hItem;
	m_strSelected = m_treeCtrl.GetItemText(hItem);
	*pResult = 0;
}
