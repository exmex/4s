// EventDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "MainFrm.h"
#include "EventDlg.h"

// CEventDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEventDlg, CDialog)
CEventDlg::CEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEventDlg::IDD, pParent),
	m_pdlg(NULL)
{
}

CEventDlg::~CEventDlg()
{
}

void CEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_lcEvent);
}


BEGIN_MESSAGE_MAP(CEventDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_ADDEVENT, OnBnClickedBtnAddEvent)
	ON_BN_CLICKED(IDC_BTN_DELEVENT, OnBnClickedBtnDeleteEvent)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_BN_CLICKED(IDC_BTN_UPDATE, OnBnClickedBtnUpdate)
END_MESSAGE_MAP()


// CEventDlg 메시지 처리기입니다.

void CEventDlg::OnBnClickedBtnAddEvent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CEventInfo dlg;
	m_pdlg = new CEventInfo(this);
	m_pdlg->SetEventID(0,EK_ADD);
	m_pdlg->DoModal();
	delete m_pdlg;
	m_pdlg = NULL;
}

void CEventDlg::OnBnClickedBtnDeleteEvent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	DWORD dwIndex = ItemSelect();
	if( !dwIndex )
		return;

	MAPEVENTINFO::iterator it = pDoc->m_mapEventInfo.find(dwIndex);//stEVENT.m_dwIndex);
	if( it == pDoc->m_mapEventInfo.end() )	
		return;


	if( (*it).second.m_bState )
	{
		
		if( AfxMessageBox(_T("This event is already in progress! Do you still wish to delete?"),MB_YESNO) != IDYES)
			return;
	}
	else
	{
		if( AfxMessageBox(_T("Are you Sure you want to delete?"),MB_YESNO) != IDYES)
			return;
	}
	//(*it).second.m_dStartDate = 0;
	//(*it).second.m_dEndDate = 0;

	pDoc->SendCT_EVENTCHANGE_REQ(EK_DEL,&(*it).second);	
}

BOOL CEventDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CEventDlg::Init()
{
	// 리스트 컨트롤의 모든 컬럼 삭제
	for(int i = 0; i < MAX_EVENTLIST_COLUMN ; i++)
        m_lcEvent.DeleteColumn(0);

	
	LV_COLUMN lvColumn;
	char*list[MAX_EVENTLIST_COLUMN]={"Index", "Title","Kind","Value (%)","State"};
	int nWidth[MAX_EVENTLIST_COLUMN]={40,200,100,60,50};
	
	for(int i = 0; i< MAX_EVENTLIST_COLUMN; i++)
	{
		lvColumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt		= LVCFMT_CENTER;
		lvColumn.pszText	= list[i];
		lvColumn.iSubItem	= i;
		lvColumn.cx			= nWidth[i];

		m_lcEvent.InsertColumn(i,&lvColumn);
		//m_listctrPos.InsertColumn(0,"NAME",LVCFMT_CENTER,180);
	}
	
	// 하나의 라인 전체가 선택되도록 설정
	m_lcEvent.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

void CEventDlg::AddListData(LPEVENTINFO pData)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	CString strTmp;
	LV_ITEM lvItem;
	INT nCount = m_lcEvent.GetItemCount();

	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= nCount; // Row 인덱스
	
	for(int iCol = 0; iCol < MAX_EVENTLIST_COLUMN ; iCol++)
	{
		switch(iCol)
		{
		case 0 : strTmp.Format("%d",pData->m_dwIndex);	break;
		case 1 : strTmp.Format("%s",pData->m_strTitle ); break;
		case 2 : 
			{
				CString strEventName = _T("");
				MAPBYTESTRING::iterator it =  pDoc->m_mapEventName.find(pData->m_bID);
				if( it != pDoc->m_mapEventName.end())
					strEventName = (*it).second;

				strTmp.Format("%s",strEventName);
			}
			break;
		case 3 : strTmp.Format("%d",pData->m_wValue);	break;
		case 4 : 
			{			
				if(pData->m_bState)
					strTmp.Format("%s",_T("Run"));
				else
					strTmp.Format("%s",_T("-"));
			}
			break;
		}
		
		lvItem.iSubItem = iCol ; // 컬럼 인덱스
		lvItem.pszText	= strTmp.GetBuffer(0);
		
		if( iCol == 0 )
			m_lcEvent.InsertItem(&lvItem);
		else
            m_lcEvent.SetItem(&lvItem);
	}
}

void CEventDlg::ClearListItem()
{
	int iCount = m_lcEvent.GetItemCount();
	for(int i = 0; i < iCount; i++)	
		m_lcEvent.DeleteItem(0);	
    
	m_lcEvent.DeleteAllItems();
}

DWORD CEventDlg::ItemSelect()
{
	// 선택된 아이템 찾기	
	DWORD dwID = 0;
	int iCount = m_lcEvent.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcEvent.GetItemState(i, LVIS_SELECTED) != 0 )
		{			
			CString str = _T("");
			str = m_lcEvent.GetItemText(i,0);
			dwID = (DWORD)atoi(str);

			break;
		}
	}

	return dwID;
}
BOOL CEventDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
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
			ItemSelect();
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}


void CEventDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	DWORD dwEventIndex = ItemSelect();
	if(!dwEventIndex)
		return;

	//CEventInfo dlg;
	m_pdlg = new CEventInfo(this);
	m_pdlg->SetEventID(dwEventIndex,EK_UPDATE);
	m_pdlg->DoModal();
	delete m_pdlg;
	m_pdlg = NULL;
}

void CEventDlg::OnBnClickedBtnUpdate()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	pDoc->SendCT_EVENTLIST_REQ();
	pDoc->SendCT_CASHITEMLIST_REQ();
}