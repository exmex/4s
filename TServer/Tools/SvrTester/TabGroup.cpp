// TabGroup.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "SvrTester.h"
#include "TabGroup.h"

#include ".\tabgroup.h"

// CTabGroup 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTabGroup, CDialog)
CTabGroup::CTabGroup(CWnd* pParent /*=NULL*/)
	: CDialog(CTabGroup::IDD, pParent)
{
	m_pImageList = NULL;
}

CTabGroup::~CTabGroup()
{
	if(m_pImageList)
		delete m_pImageList;
}

void CTabGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTabGroup, CDialog)
END_MESSAGE_MAP()


// CTabGroup 메시지 처리기입니다.

BOOL CTabGroup::OnInitDialog()
{
	CDialog::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);

	m_TabGroup.Create(TCS_BOTTOM | TCS_FIXEDWIDTH |	WS_CHILD | WS_VISIBLE,
		rect, this, IDD_TABGROUP);

	// Image List	
	m_pImageList= new CImageList; 
    m_pImageList->Create(32, 32, ILC_COLOR4 | ILC_MASK, 2, 2); 
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_MACRO)); 
	m_pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_SESSION));

	m_TabGroup.SetImageList(m_pImageList);

	// Item Insert
	TC_ITEM item;
	item.mask = TCIF_TEXT | TCIF_IMAGE;

	item.pszText = _T("Session");
	item.iImage = 0;
	m_TabGroup.InsertItem(0, &item);

	item.pszText = _T("Macro");
	item.iImage = 1;
	m_TabGroup.InsertItem(1, &item);

	m_TabGroup.AddView(TSESSION, RUNTIME_CLASS(CTSessionTab), NULL); // View Add
	m_TabGroup.AddView(TMACRO, RUNTIME_CLASS(CTMacroTab), NULL); // View Add

	m_TabGroup.SetCurSel(0);
	m_TabGroup.SetActiveView(TSESSION); // Active View Set

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

//////////////////////////////////////////////////////////////////////
// Session Protocol List Load / Clear
void CTabGroup::TSessionTabLoadList()
{
	CTSessionTab *pView = GetTSessionTabView();
	pView->LoadList();
}
void CTabGroup::TSessionListClear()
{
	CTSessionTab *pView = GetTSessionTabView();
	pView->ClearList();
}
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// Session / Macro Tab Active
void CTabGroup::TSessionActive()
{
	m_TabGroup.SetCurSel(0);
	m_TabGroup.SetActiveView(TSESSION);
	m_TabGroup.TabSelchange();
}
void CTabGroup::TMacroActive()
{
	m_TabGroup.SetCurSel(1);
	m_TabGroup.SetActiveView(TMACRO);
	m_TabGroup.TabSelchange();
}

// Get Current Tab
BOOL CTabGroup::GetCurTab()
{
	return m_TabGroup.GetCurSel();
}

// Get Session / Macro Tab View
CTSessionTab* CTabGroup::GetTSessionTabView()
{
	return (CTSessionTab*)m_TabGroup.GetView(TSESSION);
}
CTMacroTab* CTabGroup::GetTMacroTabView()
{
	return (CTMacroTab*)m_TabGroup.GetView(TMACRO);
}
//////////////////////////////////////////////////////////////////////

// Enter / Escape block
BOOL CTabGroup::PreTranslateMessage(MSG* pMsg)
{
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
        (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) )	
		return FALSE;

	return CDialog::PreTranslateMessage(pMsg);
}
