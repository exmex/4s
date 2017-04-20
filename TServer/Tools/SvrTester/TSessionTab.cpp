// TSessionTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"
#include "TSessionTab.h"
#include ".\tsessiontab.h"

#include "MainFrm.h"

// CTSessionTab

IMPLEMENT_DYNCREATE(CTSessionTab, CFormView)

CTSessionTab::CTSessionTab()
	: CFormView(CTSessionTab::IDD)
{
}

CTSessionTab::~CTSessionTab()
{
}

void CTSessionTab::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listProtocol);
}

BEGIN_MESSAGE_MAP(CTSessionTab, CFormView)	
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST1, OnLvnBegindragList1)
	ON_WM_LBUTTONUP()	
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
END_MESSAGE_MAP()


// CTSessionTab 진단입니다.

#ifdef _DEBUG
void CTSessionTab::AssertValid() const
{
	CFormView::AssertValid();
}

void CTSessionTab::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CTSessionTab 메시지 처리기입니다.

void CTSessionTab::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	LV_COLUMN lvcolumn;//컬럼 설정 구조체 
	int i; 
	TCHAR *list[2] = {_T("Value"), _T("Protocol")}; 
	int width[2] = {70, 200}; 
	for(i = 0; i < 2; i++) 
	{ 
		lvcolumn.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH; 
        lvcolumn.fmt = LVCFMT_LEFT; 
		lvcolumn.pszText = list[i]; 
		lvcolumn.iSubItem = i; 
		lvcolumn.cx = width[i]; 
		m_listProtocol.InsertColumn(i, &lvcolumn); 
	} 

	m_listProtocol.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	m_Drag = FALSE; // Protocol List Drag
}

//////////////////////////////////////////////////////////////////////
// Protocol List Load
void CTSessionTab::LoadList()
{
	CString strValue, strProtocol;
	int i;

    CMainFrame* pFrm = (CMainFrame*)AfxGetMainWnd();	

	MAPPROTOCOL::iterator it;
	for(it = pFrm->m_mapProtocol.begin(); it != pFrm->m_mapProtocol.end(); it++)
	{
		strValue.Format("%d", (*it).first);
		strProtocol = (*it).second;

		i = m_listProtocol.GetItemCount();
		LVITEM item; 
		item.mask = LVIF_TEXT; 
		item.iItem = i; 
		item.iSubItem = 0; 
		item.pszText = strValue.GetBuffer(0); 
		m_listProtocol.InsertItem(&item); 

		item.iSubItem = 1; 
		item.pszText = strProtocol.GetBuffer(0);
		m_listProtocol.SetItem(&item);
	}
}
// Protocol List Clear
void CTSessionTab::ClearList()
{
	m_listProtocol.DeleteAllItems();
}
// Protocol List Drag
void CTSessionTab::OnLvnBegindragList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	SetCapture();
	m_Drag = TRUE;	
	m_ListSelect = pNMLV->iItem;

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();
	m_EditRect = pView->GetEditPosition();
	m_ListRect = pApp->GetTSessionFrm()->GetTSessionDlg()->GetListPosition();

	m_listProtocol.SetSelectionMark(m_ListSelect);
	
	*pResult = 0;
}
// Protocol List DblClk
void CTSessionTab::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{	
	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	CTSessionView* pView = pApp->GetTSessionView();
	if( pView->GetDocument()->m_mapSession.size() == 0 )
	{
		AfxMessageBox(_T("현재 열려있는 세션이 없습니다."));
		return;
	}	

	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);		
    	
	int ListSelect = pNMLV->iItem;

	CString str = m_listProtocol.GetItemText(ListSelect, 1);	
	pView->SetEditProtocol(str);	
	
	*pResult = 0;
}

void CTSessionTab::OnLButtonUp(UINT nFlags, CPoint point)
{
	if(m_Drag)
	{
		CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
		CTSessionView* pView = pApp->GetTSessionView();
		
		CString str = m_listProtocol.GetItemText(m_ListSelect, 1);

		ClientToScreen(&point);		
		if(m_EditRect.PtInRect(point))				
			pView->SetEditProtocol(str);
		if(m_ListRect.PtInRect(point))
			pApp->GetTSessionFrm()->GetTSessionDlg()->SetDisregardList(str);
		m_Drag = FALSE;

		ReleaseCapture();		
	}
	CFormView::OnLButtonUp(nFlags, point);
}
//////////////////////////////////////////////////////////////////////

// Enter / Escape block
BOOL CTSessionTab::PreTranslateMessage(MSG* pMsg)
{
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
        (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) )	
		return FALSE;

	return CFormView::PreTranslateMessage(pMsg);
}