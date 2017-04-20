// LuckyEventDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "LuckyEventDlg.h"
#include ".\luckyeventdlg.h"

#include "MainFrm.h"
#include "HappyDoc.h"


// CLuckyEventDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CLuckyEventDlg, CDialog)
CLuckyEventDlg::CLuckyEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLuckyEventDlg::IDD, pParent)
	,m_wSelectItemID(0)
{
}

CLuckyEventDlg::~CLuckyEventDlg()
{
}

void CLuckyEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LC_LUCKYEVENT, m_lcLkEvent);
	DDX_Control(pDX, IDC_CB_LKDAY, m_cbLkDay);
	DDX_Control(pDX, IDC_COMBO1, m_cbLKWorld);
}


BEGIN_MESSAGE_MAP(CLuckyEventDlg, CDialog)
	ON_BN_CLICKED(IDC_BTN_LKUPDATE, OnBnClickedBtnLkUpdate)
	ON_BN_CLICKED(IDC_BTN_LKREFRESH, OnBnClickedBtnLkRefresh)
	ON_NOTIFY(NM_CLICK, IDC_LC_LUCKYEVENT, OnNMClickLcLuckyevent)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_CB_LKDAY, OnCbnSelchangeCbLkday)
//	ON_WM_CREATE()
ON_BN_CLICKED(IDC_BTN_LKDEL, OnBnClickedBtnLkdel)
ON_BN_CLICKED(IDC_BTN_LKADD, OnBnClickedBtnLkadd)
ON_BN_CLICKED(IDC_CHK_LKANN, OnBnClickedChkLkann)
END_MESSAGE_MAP()


// CLuckyEventDlg 메시지 처리기입니다.

void CLuckyEventDlg::Init()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	ClearDlg();	

	while(m_cbLKWorld.GetCount())
		m_cbLKWorld.DeleteString(0);

	m_cbLKWorld.AddString(_T("SELECT"));

	//	월드서버 리스트 구하기
	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
	{	
		if( (*itG).second->m_bID != 0 )
			m_cbLKWorld.AddString((*itG).second->m_strName);
	}
	m_cbLKWorld.SetCurSel(0);


	while(m_cbLkDay.GetCount())
		m_cbLkDay.DeleteString(0);

	m_cbLkDay.AddString(_T("SUN"));
	m_cbLkDay.AddString(_T("MON"));
	m_cbLkDay.AddString(_T("TUE"));
	m_cbLkDay.AddString(_T("WED"));
	m_cbLkDay.AddString(_T("THU"));
	m_cbLkDay.AddString(_T("FRI"));
	m_cbLkDay.AddString(_T("SAT"));
	m_cbLkDay.SetCurSel(0);
}
void CLuckyEventDlg::ClearDlg()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	ClearData();
	ClearListItem();
	ClearCtrl();

	// 리스트 컨트롤의 모든 컬럼 삭제
	for(int i = 0; i < MAX_LUCKYEVENT_COLUMN ; i++)
        m_lcLkEvent.DeleteColumn(0);

	LV_COLUMN lvColumn;
	char*list[MAX_LUCKYEVENT_COLUMN]={"Day", "Hour", "Min", "Item 1", "Item 2", "Item 3", "Item 4", "Item 5", "Count", "Present","Announce"};
	int nWidth[MAX_LUCKYEVENT_COLUMN]={30,30,30,100,100,100,100,100,30,200,300};
	
	for(int i = 0; i< MAX_LUCKYEVENT_COLUMN; i++)
	{
		lvColumn.mask		= LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
		lvColumn.fmt		= LVCFMT_LEFT;
		lvColumn.pszText	= list[i];
		lvColumn.iSubItem	= i;
		lvColumn.cx			= nWidth[i];

		m_lcLkEvent.InsertColumn(i,&lvColumn);
		//m_listctrPos.InsertColumn(0,"NAME",LVCFMT_CENTER,180);
	}
	
	// 하나의 라인 전체가 선택되도록 설정
	m_lcLkEvent.SetExtendedStyle(LVS_EX_FULLROWSELECT);

	m_mapDay.insert(MAPBYTESTRING::value_type(1,_T("SUN")) );
	m_mapDay.insert(MAPBYTESTRING::value_type(2,_T("MON")) );
	m_mapDay.insert(MAPBYTESTRING::value_type(3,_T("TUE")) );
	m_mapDay.insert(MAPBYTESTRING::value_type(4,_T("WED")) );
	m_mapDay.insert(MAPBYTESTRING::value_type(5,_T("THU")) );
	m_mapDay.insert(MAPBYTESTRING::value_type(6,_T("FRI")) );
	m_mapDay.insert(MAPBYTESTRING::value_type(7,_T("SAT")) );
}

void CLuckyEventDlg::ClearListItem()
{
	int iCount = m_lcLkEvent.GetItemCount();
	for(int i = 0; i < iCount; i++)	
		m_lcLkEvent.DeleteItem(i);	
    
	m_lcLkEvent.DeleteAllItems();
}

void CLuckyEventDlg::InsertItemToListCtr(  LPLUCKYEVENT pLKEVENT,int _iRow )
{
	CString strTmp;
	LV_ITEM lvItem;
	int iRowCount = _iRow;

	if(iRowCount == -1 )
		iRowCount = m_lcLkEvent.GetItemCount();
	
	lvItem.mask		= LVIF_TEXT;
	lvItem.iItem	= iRowCount; // Row 인덱스
	
	for(int iCol = 0; iCol < MAX_LUCKYEVENT_COLUMN ; iCol++)
	{
		switch(iCol)
		{
		case 0 : FindDay(pLKEVENT->m_bDay,strTmp);				break;
		case 1 : strTmp.Format("%d",pLKEVENT->m_bHour );		break;
		case 2 : strTmp.Format("%d",pLKEVENT->m_bMin );			break;		
		case 3 : strTmp.Format("%s",pLKEVENT->m_strItem1 );		break;		
		case 4 : strTmp.Format("%s",pLKEVENT->m_strItem2 );		break;		
		case 5 : strTmp.Format("%s",pLKEVENT->m_strItem3 );		break;		
		case 6 : strTmp.Format("%s",pLKEVENT->m_strItem4 );		break;		
		case 7 : strTmp.Format("%s",pLKEVENT->m_strItem5 );		break;		
		case 8 : strTmp.Format("%d",pLKEVENT->m_bCount );		break;		
		case 9 : strTmp.Format("%s",pLKEVENT->m_strPresent );	break;
		case 10: strTmp.Format("%s",pLKEVENT->m_strAnnounce);	break;
		}
		
		lvItem.iSubItem = iCol ; // 컬럼 인덱스
		lvItem.pszText	= strTmp.GetBuffer(0);		
		
		if( iCol == 0 )
			m_lcLkEvent.InsertItem(&lvItem);
		else
            m_lcLkEvent.SetItem(&lvItem);
	}

	m_lcLkEvent.SetItemData(iRowCount, pLKEVENT->m_wID);
}

void CLuckyEventDlg::UpdateListData( LPLUCKYEVENT pLKEVENT )
{
	CString str;
	int iCount = m_lcLkEvent.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcLkEvent.GetItemData(i) == pLKEVENT->m_wID)
		{
			CString str;
			str.Format("%d",pLKEVENT->m_bHour);
			m_lcLkEvent.SetItemText(i,1,str);
			str.Format("%d",pLKEVENT->m_bMin);
			m_lcLkEvent.SetItemText(i,2,str);
			m_lcLkEvent.SetItemText(i,3,pLKEVENT->m_strItem1);
			m_lcLkEvent.SetItemText(i,4,pLKEVENT->m_strItem2);
			m_lcLkEvent.SetItemText(i,5,pLKEVENT->m_strItem3);
			m_lcLkEvent.SetItemText(i,6,pLKEVENT->m_strItem4);
			m_lcLkEvent.SetItemText(i,7,pLKEVENT->m_strItem5);
			str.Format("%d",pLKEVENT->m_bCount);
			m_lcLkEvent.SetItemText(i,8,str);
			m_lcLkEvent.SetItemText(i,9,pLKEVENT->m_strPresent);
			m_lcLkEvent.SetItemText(i,10,pLKEVENT->m_strAnnounce);

			SetDlgItemInt(IDC_EB_LKHOUR,pLKEVENT->m_bHour );
			SetDlgItemInt(IDC_EB_LKMIN,pLKEVENT->m_bMin );
			SetDlgItemInt(IDC_EB_LKITEM1,pLKEVENT->m_wItemID1 );
			SetDlgItemInt(IDC_EB_LKITEM2,pLKEVENT->m_wItemID2 );
			SetDlgItemInt(IDC_EB_LKITEM3,pLKEVENT->m_wItemID3 );
			SetDlgItemInt(IDC_EB_LKITEM4,pLKEVENT->m_wItemID4 );
			SetDlgItemInt(IDC_EB_LKITEM5,pLKEVENT->m_wItemID5 );
			SetDlgItemInt(IDC_EB_LKCOUNT,pLKEVENT->m_bCount );
			SetDlgItemText(IDC_EB_LKPRESENT,pLKEVENT->m_strPresent);
			SetDlgItemText(IDC_EB_LKANNOUNCE,pLKEVENT->m_strAnnounce);
			
			UpdateData(TRUE);
			break;
		}
	}
}
void CLuckyEventDlg::DeleteListData(WORD wID)
{		
	// 선택된 아이템 찾기	
	int iCount = m_lcLkEvent.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if(wID == m_lcLkEvent.GetItemData(i))
		{
			// list control 갱신
			m_lcLkEvent.DeleteItem(i);
			m_lcLkEvent.UpdateData();
			return;
		}
	}
}

void CLuckyEventDlg::ItemSelect()
{
	// Document 포인트 얻기
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	m_wSelectItemID = 0;

	// 선택된 아이템 찾기
	int iCount = m_lcLkEvent.GetItemCount();
	for(int i = 0; i < iCount; i++)
	{
		if( m_lcLkEvent.GetItemState(i, LVIS_SELECTED) != 0 )
		{				
			BYTE bDay = 0;
			BYTE bHour = 0; 
			BYTE bMin = 0;
			CString strTemp;
			MAPSTRINGBYTE::iterator it;
			
			strTemp = m_lcLkEvent.GetItemText(i,0);
			MAPBYTESTRING::iterator itD;
			for(itD = m_mapDay.begin(); itD != m_mapDay.end(); itD++)
			{
				if( (*itD).second == strTemp )
				{
					bDay = (*itD).first;
					break;
				}
			}
			LPLUCKYEVENT pLKEVENT = pDoc->GetLuckyEvent((WORD)m_lcLkEvent.GetItemData(i));
			if(!pLKEVENT)
				return;

			CButton* pCheck = (CButton*)GetDlgItem(IDC_CHK_LKANN);
			if(!pCheck)
				return;

			pCheck->SetCheck(!pLKEVENT->m_bCount);
			OnBnClickedChkLkann();
			
			m_cbLkDay.SetCurSel(bDay-1);
			
			strTemp = m_lcLkEvent.GetItemText(i,1);
			bHour = atoi(strTemp);
			SetDlgItemInt(IDC_EB_LKHOUR, bHour);
			
			strTemp = m_lcLkEvent.GetItemText(i,2);
			bMin = atoi(strTemp);
			SetDlgItemInt(IDC_EB_LKMIN, bMin);

			strTemp = m_lcLkEvent.GetItemText(i,8);
			SetDlgItemInt(IDC_EB_LKCOUNT,atoi(strTemp) );

			strTemp = m_lcLkEvent.GetItemText(i,9);
			SetDlgItemText(IDC_EB_LKPRESENT,strTemp);

			strTemp = m_lcLkEvent.GetItemText(i,10);
			SetDlgItemText(IDC_EB_LKANNOUNCE,strTemp);

			SetDlgItemText(IDC_EB_LKTITLE,pLKEVENT->m_strTitle);
			SetDlgItemText(IDC_EB_LKMESSAGE,pLKEVENT->m_strMessage);

			SetDlgItemInt(IDC_EB_LKITEM1,pLKEVENT->m_wItemID1 );
			SetDlgItemInt(IDC_EB_LKITEM2,pLKEVENT->m_wItemID2 );
			SetDlgItemInt(IDC_EB_LKITEM3,pLKEVENT->m_wItemID3 );
			SetDlgItemInt(IDC_EB_LKITEM4,pLKEVENT->m_wItemID4 );
			SetDlgItemInt(IDC_EB_LKITEM5,pLKEVENT->m_wItemID5 );

			m_wSelectItemID = pLKEVENT->m_wID;

			return;
		}
	}
}


void CLuckyEventDlg::OnBnClickedBtnLkdel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if( !m_lcLkEvent.GetItemCount())
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
	if(!m_wSelectItemID)
	{
		AfxMessageBox("No Select Data");
		return;
	}

	LUCKYEVENT stLKEVENT;
	stLKEVENT.Reset();
	stLKEVENT.m_wID = m_wSelectItemID;
	pDoc->SendCT_EVENTQUARTERUPDATE_REQ(GetWorldGroup(), (BYTE)EK_DEL, &stLKEVENT);
}

void CLuckyEventDlg::OnBnClickedBtnLkadd()
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

	CString str = _T("");
	
	LUCKYEVENT stLKEVENT;
	stLKEVENT.Reset();

	if(!ReadDlgText(stLKEVENT))
		return;

	BYTE bGroup = GetWorldGroup();
	if(!bGroup)
		return;
	pDoc->SendCT_EVENTQUARTERUPDATE_REQ(bGroup, (BYTE)EK_ADD, &stLKEVENT);
}

void CLuckyEventDlg::OnBnClickedBtnLkUpdate()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if(AfxMessageBox("Are you sure you want to update?",MB_YESNO) == IDNO )
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
	
	LUCKYEVENT stLKEVENT;
	stLKEVENT.Reset();

	if(!m_wSelectItemID)
	{
		AfxMessageBox("No Select Data");
		return;
	}

	stLKEVENT.m_wID = m_wSelectItemID;

	if(!ReadDlgText(stLKEVENT))
		return;

	BYTE bGroup = GetWorldGroup();
	if(!bGroup)
		return;

	pDoc->SendCT_EVENTQUARTERUPDATE_REQ(bGroup, (BYTE)EK_UPDATE, &stLKEVENT);
}



void CLuckyEventDlg::OnBnClickedBtnLkRefresh()
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

	BYTE bGroup = GetWorldGroup();
	if(!bGroup)
		return;

	pDoc->SendCT_EVENTQUARTERLIST_REQ(bGroup, m_cbLkDay.GetCurSel()+1);

	ClearDlg();
}

void CLuckyEventDlg::FindDay(BYTE bDay, CString& strReturn)
{	
	MAPBYTESTRING::iterator itD = m_mapDay.find(bDay);
	if( itD != m_mapDay.end())
		strReturn =  (*itD).second;	
}

BYTE CLuckyEventDlg::FindDay(CString str)
{
	MAPBYTESTRING::iterator itD;
	for(itD = m_mapDay.begin(); itD != m_mapDay.end(); itD++)
		if( (*itD).second == str )
			return (*itD).first;


	return 0;
}

void CLuckyEventDlg::OnNMClickLcLuckyevent(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	ItemSelect();
}

void CLuckyEventDlg::ClearCtrl()
{
	SetDlgItemInt(IDC_EB_LKHOUR, 0 );
	SetDlgItemInt(IDC_EB_LKMIN,  0 );
	SetDlgItemInt(IDC_EB_LKITEM1,0 );
	SetDlgItemInt(IDC_EB_LKITEM2,0 );
	SetDlgItemInt(IDC_EB_LKITEM3,0 );
	SetDlgItemInt(IDC_EB_LKITEM4,0 );
	SetDlgItemInt(IDC_EB_LKITEM5,0 );
	SetDlgItemInt(IDC_EB_LKCOUNT,0 );
	SetDlgItemText(IDC_EB_LKPRESENT,_T(""));
	SetDlgItemText(IDC_EB_LKANNOUNCE,_T(""));	
}

void CLuckyEventDlg::ClearData()
{
	m_mapDay.clear();
}
BOOL CLuckyEventDlg::PreTranslateMessage(MSG* pMsg)
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
			ItemSelect();
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CLuckyEventDlg::OnCbnSelchangeCombo1()
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
	
	BYTE bGroup = GetWorldGroup();

	ClearDlg();
	if(!bGroup)
		return;

	pDoc->SendCT_EVENTQUARTERLIST_REQ(bGroup, 1);
}

void CLuckyEventDlg::OnCbnSelchangeCbLkday()
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

	BYTE bGroup = GetWorldGroup();

	if(!bGroup)
		return;

	pDoc->SendCT_EVENTQUARTERLIST_REQ(bGroup, m_cbLkDay.GetCurSel()+1);

	ClearDlg();
}

BYTE CLuckyEventDlg::GetWorldGroup()
{

	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return 0;
	}

	CString str = _T("");

	m_cbLKWorld.GetLBText(m_cbLKWorld.GetCurSel(), str);

	MAPGROUP::iterator itG;
	for(itG=pDoc->m_mapGroup.begin(); itG!=pDoc->m_mapGroup.end(); itG++)
		if((*itG).second->m_strName == str)
			break;
	if( itG == pDoc->m_mapGroup.end() )
	{
		AfxMessageBox(_T("Not found server"));
		return 0;
	}
	return (*itG).second->m_bID;
}

BYTE CLuckyEventDlg::ReadDlgText(LUCKYEVENT& stLKEVENT)
{
	CString str;
	stLKEVENT.m_bDay	 = m_cbLkDay.GetCurSel() +1;
	stLKEVENT.m_bHour	 = GetDlgItemInt(IDC_EB_LKHOUR);
	stLKEVENT.m_bMin	 = GetDlgItemInt(IDC_EB_LKMIN);
	
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHK_LKANN);
	if(!pCheck)
		return FALSE;

	BYTE bCheckAnnounce = pCheck->GetCheck();

	if(!bCheckAnnounce)
	{
		stLKEVENT.m_wItemID1 = GetDlgItemInt(IDC_EB_LKITEM1);
		stLKEVENT.m_wItemID2 = GetDlgItemInt(IDC_EB_LKITEM2);
		stLKEVENT.m_wItemID3 = GetDlgItemInt(IDC_EB_LKITEM3);
		stLKEVENT.m_wItemID4 = GetDlgItemInt(IDC_EB_LKITEM4);
		stLKEVENT.m_wItemID5 = GetDlgItemInt(IDC_EB_LKITEM5);
		stLKEVENT.m_bCount	 = GetDlgItemInt(IDC_EB_LKCOUNT);

		if(!stLKEVENT.m_wItemID1 
			&& !stLKEVENT.m_wItemID2 
			&& !stLKEVENT.m_wItemID3 
			&& !stLKEVENT.m_wItemID4
			&& !stLKEVENT.m_wItemID5)
		{
			AfxMessageBox("Enter the correct Item ID");
			return FALSE;
		}

		if(!stLKEVENT.m_bCount)
		{
			AfxMessageBox("Enter the correct Item Count");
			return FALSE;
		}

		GetDlgItemText(IDC_EB_LKPRESENT,str);
		if(str.IsEmpty() || str.GetLength() > 50)
		{
			AfxMessageBox("Enter the correct Present");
			return FALSE;
		}
		stLKEVENT.m_strPresent = str;

		GetDlgItemText(IDC_EB_LKTITLE,str);
		if(str.IsEmpty() || str.GetLength() > 50)
		{
			AfxMessageBox("Enter the correct Title");
			return FALSE;
		}
		stLKEVENT.m_strTitle = str;

		GetDlgItemText(IDC_EB_LKMESSAGE,str);
		if(str.IsEmpty() || str.GetLength() > 500)
		{
			AfxMessageBox("Enter the correct Message");
			return FALSE;
		}
		stLKEVENT.m_strMessage = str;
	}
	
	GetDlgItemText(IDC_EB_LKANNOUNCE,str);
	if(str.IsEmpty() || str.GetLength() > 1024)
	{
		AfxMessageBox("Enter the correct Announce");
		return FALSE;
	}
	stLKEVENT.m_strAnnounce = str;

	return TRUE;
};

void CLuckyEventDlg::OnBnClickedChkLkann()
{
	CButton* pCheck = (CButton*)GetDlgItem(IDC_CHK_LKANN);
	if(!pCheck)
		return;

	BYTE bCheckAnnounce = pCheck->GetCheck();

	GetDlgItem(IDC_EB_LKITEM1)->EnableWindow(!bCheckAnnounce);
	GetDlgItem(IDC_EB_LKITEM2)->EnableWindow(!bCheckAnnounce);
	GetDlgItem(IDC_EB_LKITEM3)->EnableWindow(!bCheckAnnounce);
	GetDlgItem(IDC_EB_LKITEM4)->EnableWindow(!bCheckAnnounce);
	GetDlgItem(IDC_EB_LKITEM5)->EnableWindow(!bCheckAnnounce);
	GetDlgItem(IDC_EB_LKCOUNT)->EnableWindow(!bCheckAnnounce);
	GetDlgItem(IDC_EB_LKTITLE)->EnableWindow(!bCheckAnnounce);
	GetDlgItem(IDC_EB_LKMESSAGE)->EnableWindow(!bCheckAnnounce);
	GetDlgItem(IDC_EB_LKPRESENT)->EnableWindow(!bCheckAnnounce);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
