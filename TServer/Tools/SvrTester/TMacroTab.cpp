// TMacroTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"
#include "TMacroTab.h"
#include ".\tmacrotab.h"


// CTMacroTab

IMPLEMENT_DYNCREATE(CTMacroTab, CFormView)

CTMacroTab::CTMacroTab()
	: CFormView(CTMacroTab::IDD)
{		
	m_mapMacro.clear();
}

CTMacroTab::~CTMacroTab()
{	
	DeleteAllMacroMap();
}

void CTMacroTab::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_TreeMacro);
}

BEGIN_MESSAGE_MAP(CTMacroTab, CFormView)	
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnTvnSelchangedTree1)	
	ON_BN_CLICKED(IDC_BUTTON_LOOPSET, OnBnClickedButtonLoopset)
	ON_BN_CLICKED(IDC_BUTTON_LOOPCANCLE, OnBnClickedButtonLoopcancle)
END_MESSAGE_MAP()


// CTMacroTab 진단입니다.

#ifdef _DEBUG
void CTMacroTab::AssertValid() const
{
	CFormView::AssertValid();
}

void CTMacroTab::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CTMacroTab 메시지 처리기입니다.

// Enter / Escape block
BOOL CTMacroTab::PreTranslateMessage(MSG* pMsg)
{
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
        (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) )	
		return FALSE;

	return CFormView::PreTranslateMessage(pMsg);	
}

void CTMacroTab::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	m_TreeMacro.InitTreeCtrl(TMACROTABTREE, 16, IDI_ICON_TM1, IDI_ICON_TM2, IDI_ICON_TM3);
}

/////////////////////////////////////////////////////////////////////////////////////
// Get Current Selected Macro
CTMacro* CTMacroTab::GetCurSelectedMacro()
{
	HTREEITEM hItem = m_TreeMacro.GetSelectedItem();
	if(!hItem) 
		return NULL;

	DWORD mID = (DWORD)m_TreeMacro.GetItemData(hItem);
	MAPMACRO::iterator finder = m_mapMacro.find(mID);
	CTMacro* pMacro = (*finder).second;	

	return pMacro;
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Macro Map Insert / Delete / Update
void CTMacroTab::InsertMacroMap(DWORD mID, CString strName, CString strParent)
{	
    CTMacro* pMacro = new CTMacro(strName, strParent);	
	m_mapMacro.insert(MAPMACRO::value_type(mID, pMacro));

	TRACE(_T("Macro Map Insert : %ld %s\n"), mID, pMacro->GetMacroName());
}
void CTMacroTab::DeleteMacroMap(HTREEITEM hItem)
{
	DWORD mID = (DWORD)m_TreeMacro.GetItemData(hItem);
	MAPMACRO::iterator finder = m_mapMacro.find(mID);
	CTMacro* pMacro = (*finder).second;
	delete pMacro;
	m_mapMacro.erase(mID);

	if(m_TreeMacro.ItemHasChildren(hItem))
	{
		HTREEITEM hNextItem;
		HTREEITEM hChildItem = m_TreeMacro.GetChildItem(hItem);		

		while (hChildItem != NULL)
		{			
			DeleteMacroMap(hChildItem);			
			hNextItem = m_TreeMacro.GetNextItem(hChildItem, TVGN_NEXT);			
			hChildItem = hNextItem;
		}
	}	

	if(m_mapMacro.size() == 0)
        m_TreeMacro.m_TreeCnt = 0;
}
void CTMacroTab::DeleteAllMacroMap()
{
	MAPMACRO::iterator it;
	for(it = m_mapMacro.begin(); it != m_mapMacro.end(); it++)
	{
		CTMacro* pMacro = (*it).second;
		delete pMacro;
	}
    m_mapMacro.clear();	
}
BOOL CTMacroTab::UpdateMacroMap(CString strOld, CString strNew)
{
	MAPMACRO::iterator it;
	for(it = m_mapMacro.begin(); it != m_mapMacro.end(); it++)
	{
		CTMacro* pMacro = (*it).second;
		if( pMacro->GetMacroName() == strNew )
			return FALSE;
	}

	for(it = m_mapMacro.begin(); it != m_mapMacro.end(); it++)
	{
		CTMacro* pMacro = (*it).second;
		if( pMacro->GetMacroName() == strOld )		
			pMacro->UpdateMacroName(strNew);
		
		if( pMacro->GetMacroParent() == strOld )		
			pMacro->UpdateMacroParent(strNew);		
	}

	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Macro Tree Sel Changed
void CTMacroTab::OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

    CTMacro* pMacro = GetCurSelectedMacro();

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	pApp->GetTMacroFrm()->GetTMacroDlg()->MacroParamView(pMacro);
	pApp->GetTMacroView()->MacroPInfoView(pMacro);
	
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Macro Loop Set / Cancle
void CTMacroTab::OnBnClickedButtonLoopset()
{
	HTREEITEM hItem = m_TreeMacro.GetSelectedItem();
	if(!hItem) 
		return;	

	DWORD mID = (DWORD)m_TreeMacro.GetItemData(hItem);
	MAPMACRO::iterator finder = m_mapMacro.find(mID);
	CTMacro* pMacro = (*finder).second;	
	pMacro->SetMacroLoop(TRUE);

	m_TreeMacro.ChangeImage(2, 2);
}
void CTMacroTab::OnBnClickedButtonLoopcancle()
{
	HTREEITEM hItem = m_TreeMacro.GetSelectedItem();
	if(!hItem) 
		return;

	DWORD mID = (DWORD)m_TreeMacro.GetItemData(hItem);
	MAPMACRO::iterator finder = m_mapMacro.find(mID);
	CTMacro* pMacro = (*finder).second;	
	pMacro->SetMacroLoop(FALSE);

	m_TreeMacro.ChangeImage(0, 1);
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// All Protocol Valid Check
void CTMacroTab::AllValidCheck(MAPPROTOCOL mapProtocol)
{
	MAPMACRO::iterator it;
	for(it = m_mapMacro.begin(); it != m_mapMacro.end(); it++)
	{
		CTMacro* pMacro = (*it).second;
		pMacro->ValidCheck(mapProtocol);
	}
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// File Load Macro Map Insert
void CTMacroTab::FLoadInsertMacroMap(DWORD mID, CTMacro* pMacro)
{
	m_mapMacro.insert(MAPMACRO::value_type(mID, pMacro));
    TRACE(_T("File Load Macro Map Insert : %ld %s\n"), mID, pMacro->GetMacroName());
}
/////////////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////////////
// Macro View
void CTMacroTab::MacroView()
{
	DWORD tCnt = 0;

	MAPMACRO::iterator it;
	for(it = m_mapMacro.begin(); it != m_mapMacro.end(); it++)
	{
		DWORD mID = (*it).first;
		if( tCnt < mID )
			tCnt = mID;
		CTMacro* pMacro = (*it).second;

		CString sName = pMacro->GetMacroName();
		CString sParent = pMacro->GetMacroParent();
		if( sParent == _T("") )
		{
			HTREEITEM hParent = m_TreeMacro.InsertTreeItem(sName, NULL);
			m_TreeMacro.SetItemData(hParent, mID);
			m_TreeMacro.SelectItem(hParent);
			if( pMacro->GetMacroLoop() )
				m_TreeMacro.ChangeImage(2, 2);
			MacroChildView(sName, hParent);
		}
	}
	m_TreeMacro.m_TreeCnt = tCnt+1;
    
	m_TreeMacro.SelectItem(m_TreeMacro.GetRootItem());
}
void CTMacroTab::MacroChildView(CString strName, HTREEITEM hParent)
{
	MAPMACRO::iterator it;
	for(it = m_mapMacro.begin(); it != m_mapMacro.end(); it++)
	{
		DWORD mID = (*it).first;
		CTMacro* pMacro = (*it).second;

		CString sName = pMacro->GetMacroName();
		CString sParent = pMacro->GetMacroParent();
		if( sParent == strName )
		{
			HTREEITEM hP = m_TreeMacro.InsertTreeItem(sName, hParent);
			m_TreeMacro.SetItemData(hP, mID);
			m_TreeMacro.SelectItem(hP);
			if( pMacro->GetMacroLoop() )
				m_TreeMacro.ChangeImage(2, 2);
			MacroChildView(sName, hP);
		}
	}
}
void CTMacroTab::ClearMacroView()
{
	m_TreeMacro.DeleteAllItems();
	m_TreeMacro.m_TreeCnt = 0;

	CSvrTesterApp* pApp = (CSvrTesterApp*)AfxGetApp();
	pApp->GetTMacroFrm()->GetTMacroDlg()->ClearMacroParamView();
	pApp->GetTMacroView()->ClearMacroPInfoView();
}
/////////////////////////////////////////////////////////////////////////////////////