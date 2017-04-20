// ChatBanList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "ChatBanList.h"
#include ".\chatbanlist.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CChatBanList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CChatBanList, CDialog)
CChatBanList::CChatBanList(CWnd* pParent /*=NULL*/)
	: CDialog(CChatBanList::IDD, pParent)
{
}

CChatBanList::~CChatBanList()
{
}

void CChatBanList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LC_BANLIST, m_lcChatBanList);
	DDX_Control(pDX, IDC_EB_BANREASON, m_ebBanReason);
}


BEGIN_MESSAGE_MAP(CChatBanList, CDialog)
	ON_BN_CLICKED(IDC_BTN_VIEWBANLIST, OnBnClickedBtnViewbanlist)
	ON_BN_CLICKED(IDC_BTN_DELETEBAN, OnBnClickedBtnDeleteban)
	ON_BN_CLICKED(IDC_BTN_CLEARBANLIST, OnBnClickedBtnClearbanlist)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)	
	ON_NOTIFY(NM_CLICK, IDC_LC_BANLIST, OnNMClickLcBanlist)	
END_MESSAGE_MAP()


// CChatBanList 메시지 처리기입니다.

void CChatBanList::Init()
{
	// 리스트 컨트롤의 모든 컬럼 삭제	
	for(int i = 0; i < MAX_CHATBANLIST_COLUMN ; i++)
        m_lcChatBanList.DeleteColumn(0);

	
	LV_COLUMN lvColumn;
	char*list[MAX_CHATBANLIST_COLUMN]={"id","OPName","Ban Name", "Start Time", "Ban Time"};
	int nWidth[MAX_CHATBANLIST_COLUMN]={0,90,90,100,60};
	
	for(int i = 0; i< MAX_CHATBANLIST_COLUMN; i++)
	{
		lvColumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt		= LVCFMT_LEFT;
		lvColumn.pszText	= list[i];
		lvColumn.iSubItem	= i;
		lvColumn.cx			= nWidth[i];

		m_lcChatBanList.InsertColumn(i,&lvColumn);		
	}
	
	// 하나의 라인 전체가 선택되도록 설정
	m_lcChatBanList.SetExtendedStyle(LVS_EX_FULLROWSELECT);	
}


void CChatBanList::InsertToListCtrl(DWORD dwID,CString strName, __time64_t time, WORD wMin, CString strReason, CString strOPName)
{
	DWORD dwIndex;
	CString strTmp;
	CTime t(time);
	LV_ITEM lvItem;

	dwIndex = m_lcChatBanList.GetItemCount();

	LPBANLIST pBANLIST = new BANLIST();
	pBANLIST->m_dwID = dwID;
	pBANLIST->m_strBanName = strName;
	pBANLIST->m_strOPName = strOPName;
	pBANLIST->m_strReason = strReason;
	pBANLIST->m_tStartTime = time;
	pBANLIST->m_wBanTime = wMin;

	m_mapBANLIST.insert(MAPBANLIST::value_type(dwID,pBANLIST));

	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= dwIndex; // Row 인덱스
	
	for(int iCol = 0; iCol < MAX_CHATBANLIST_COLUMN ; iCol++)
	{
		switch(iCol)
		{
		case 0 : strTmp.Format("%d",dwID); break;
		case 1 : strTmp = strOPName;	break;
		case 2 : strTmp = strName;		break;
		case 3 : strTmp.Format("%d-%d-%d %d:%d",t.GetYear(), t.GetMonth(), t.GetDay(), t.GetHour(), t.GetMinute() ); break;
		case 4 : strTmp.Format("%d",wMin );		break;		
		}
		
		lvItem.iSubItem = iCol ; // 컬럼 인덱스
		lvItem.pszText	= strTmp.GetBuffer(0);
		
		if( iCol == 0 )
			m_lcChatBanList.InsertItem(&lvItem);
		else
            m_lcChatBanList.SetItem(&lvItem);
	}

	UpdateData();
}

void CChatBanList::OnBnClickedBtnViewbanlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	pDoc->SendCT_CHATBANLIST_REQ();

	ClearListCtrl();
}

void CChatBanList::OnBnClickedBtnDeleteban()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( !m_lcChatBanList.GetItemCount())
		return;

	if(AfxMessageBox("Are you sure you want to delete?",MB_YESNO) == IDNO ) //진정 삭제하시겠습니까
		return;
	

	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	
	// 선택된 아이템 찾기	
	int iCount = m_lcChatBanList.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcChatBanList.GetItemState(i, LVIS_SELECTED) != 0 )
		{
			DWORD dwID;
			CString str = _T("");
			str = m_lcChatBanList.GetItemText(i,0);
			dwID = atoi(str);

			MAPBANLIST::iterator itF = m_mapBANLIST.find(dwID);
			if(itF == m_mapBANLIST.end() )
				return;

			dwID = (*itF).second->m_dwID;
			pDoc->SendCT_CHATBANLISTDEL_REQ(dwID);
			// list control 갱신
			m_lcChatBanList.DeleteItem(i);
			m_lcChatBanList.UpdateData();

			SetDlgItemText(IDC_EB_BANREASON,_T(""));
			
			delete (*itF).second;
			m_mapBANLIST.erase(itF);
			
			break;
		}
	}
}

void CChatBanList::OnBnClickedBtnClearbanlist()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if( !m_lcChatBanList.GetItemCount())
		return;

	if(AfxMessageBox("Are you sure you want to clear all ?",MB_YESNO) == IDNO ) //진정 삭제하시겠습니까
		return;

	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	pDoc->SendCT_CHATBANLISTDEL_REQ(0);

	ClearListCtrl();
	SetDlgItemText(IDC_EB_BANREASON,_T(""));
}

void CChatBanList::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClearListCtrl();
	SetDlgItemText(IDC_EB_BANREASON,_T(""));

	OnCancel();
}

void CChatBanList::ClearListCtrl()
{
	int iCount = m_lcChatBanList.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		m_lcChatBanList.DeleteItem(i);
	}
    
	m_lcChatBanList.DeleteAllItems();
	UpdateData();

	MAPBANLIST::iterator it;
	for( it = m_mapBANLIST.begin(); it != m_mapBANLIST.end(); it++)	
		delete (*it).second;
	m_mapBANLIST.clear();		
}

BOOL CChatBanList::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.	
	if( (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) ||
        (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) )	
		return FALSE;

	if( pMsg->message == WM_KEYDOWN) 
	{
		switch(pMsg->wParam)
		{
		case VK_RETURN:
		case VK_ESCAPE:
			return FALSE;

		}
	}
	else if(pMsg->message == WM_KEYUP)
	{
		switch(pMsg->wParam)
		{
		case VK_UP:
		case VK_DOWN:
			SelectItem();
			break;
		}
	}
			

	return CDialog::PreTranslateMessage(pMsg);
}

void CChatBanList::OnNMClickLcBanlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
	
	SelectItem();	
}

void CChatBanList::SelectItem()
{
	// 선택된 아이템 찾기	
	int iCount = m_lcChatBanList.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcChatBanList.GetItemState(i, LVIS_SELECTED) != 0 )
		{
			DWORD dwID;
			CString str = _T("");
			str = m_lcChatBanList.GetItemText(i,0);
			dwID = atoi(str);

			MAPBANLIST::iterator itF = m_mapBANLIST.find(dwID);
			if(itF != m_mapBANLIST.end())
				str = (*itF).second->m_strReason;

			SetDlgItemText(IDC_EB_BANREASON,str);
			break;
		}
	}
}