// FindQuestListDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TQuestEditor.h"
#include "FindQuestListDlg.h"
#include ".\findquestlistdlg.h"


// CFindQuestListDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFindQuestListDlg, CDialog)
CFindQuestListDlg::CFindQuestListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFindQuestListDlg::IDD, pParent)
	, m_pTree(NULL)
	, m_pHItem(NULL)
	, m_pSelectedItem(NULL)
{
	m_mapITEMLIST.clear();
	m_pHItem = NULL;
}

CFindQuestListDlg::~CFindQuestListDlg()
{
	m_pTree = NULL;
	m_pHItem = NULL;
	m_pSelectedItem = NULL;
	m_mapITEMLIST.clear();
	m_pHItem = NULL;
}

void CFindQuestListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FIND_QUEST, m_cFindQuestList);
}


BEGIN_MESSAGE_MAP(CFindQuestListDlg, CDialog)
	ON_BN_CLICKED(ID_SELECT_QUEST_ITEM, OnSelectedItem)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FIND_QUEST, OnSelectedItem)
END_MESSAGE_MAP()


// CFindQuestListDlg 메시지 처리기입니다.

BOOL CFindQuestListDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);			// Set small icon

	CRect rect;
	m_cFindQuestList.GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	m_cFindQuestList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	m_cFindQuestList.InsertColumn( 0, _T("ID"), LVCFMT_LEFT, 80);
	m_cFindQuestList.InsertColumn( 1, _T("Quest Title"), LVCFMT_LEFT, rect.Width() - 80);

	/*CRect rect;
	m_cFindQuestList.GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	m_cFindQuestList.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	m_cFindQuestList.InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());*/

	VECHITEM::iterator	itHItem;

	//int nCnt = 0;

	for( itHItem = m_pHItem->begin() ; itHItem != m_pHItem->end() ; itHItem++ )
	{
		LPTQUEST pQUEST = (LPTQUEST) m_pTree->GetItemData((*itHItem));
		m_mapITEMLIST.insert(MAPITEM::value_type(m_cFindQuestList.GetItemCount(), (*itHItem)));

		CString strQuestID;
		strQuestID.Format("%d", pQUEST->m_dwQuestID );
		
		int nIndex = m_cFindQuestList.InsertItem(m_cFindQuestList.GetItemCount(), strQuestID);

		m_cFindQuestList.SetItemText(nIndex, 1, pQUEST->m_strTitle );

		//m_cFindQuestList.InsertItem(m_cFindQuestList.GetItemCount(), pQUEST->m_strTitle );
	}
	return TRUE;  // return TRUE unless you set the focus to a control
}

void CFindQuestListDlg::InitListData(CTreeCtrl *pTree, VECHITEM* vHItem)
{
	m_pTree = pTree;
	m_pHItem = vHItem;
}

void CFindQuestListDlg::OnSelectedItem()
{
	MAPITEM::iterator itFind;
	itFind = m_mapITEMLIST.find( INT(m_cFindQuestList.GetFirstSelectedItemPosition() - 1));

	if( itFind != m_mapITEMLIST.end() )
		m_pSelectedItem = (*itFind).second;
	OnOK();
}

void CFindQuestListDlg::OnSelectedItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	OnSelectedItem();
}

void CFindQuestListDlg::GetSelectedItem(HTREEITEM* pSelectedItem)
{
	(*pSelectedItem) = m_pSelectedItem;
}
