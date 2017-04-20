// ServiceList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "NetCode.h"

// CServiceList

IMPLEMENT_DYNCREATE(CServiceList, CFormView)

CServiceList::CServiceList()
	: CFormView(CServiceList::IDD)
{
}

CServiceList::~CServiceList()
{
}

void CServiceList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CServiceList, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CServiceList 진단입니다.

#ifdef _DEBUG
void CServiceList::AssertValid() const
{
	CFormView::AssertValid();
}

void CServiceList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CServiceList 메시지 처리기입니다.

void CServiceList::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	if (GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		if (m_listCtrl.GetSafeHwnd())
			m_listCtrl.MoveWindow(&rect);
	}
}

void CServiceList::UpdateServiceList(DWORD dwID)
{
    CHappyDoc * pDoc = (CHappyDoc *)GetDocument();	
	if(!pDoc) return;

	if(dwID == 0) m_listCtrl.DeleteAllItems(); // 06/4/14 현승룡 List Data Update를 위해 수정
	int listCnt = -1;
	int listTotalCnt = m_listCtrl.GetItemCount();

	MAPSERVICE::iterator it;
	if(pDoc->m_bViewAllService)
	{		
		for(it=pDoc->m_mapService.begin(); it!=pDoc->m_mapService.end(); it++)
		{
			if(dwID) // 06/4/14 현승룡 List Data Update를 위해 수정
				AddItemDataUpdate((*it).second, ++listCnt); 
			else
				AddItemData((*it).second);
		}		
		return;
	}

	BYTE bSvrID = SVRID(pDoc->m_dwSelectedService);
	BYTE bType = SVRTYPE(pDoc->m_dwSelectedService);
	BYTE bGroup = SVRGROUP(pDoc->m_dwSelectedService);	

	for(it=pDoc->m_mapService.begin(); it!= pDoc->m_mapService.end(); it++)
	{
		LPHAPPYSERVICE pSvs = (*it).second;
		if(bSvrID)
		{
			if(pSvs->m_dwID == pDoc->m_dwSelectedService &&
				pSvs->m_pSvrType->m_bType == SVRTYPE(pDoc->m_dwSelectedService) &&
				pSvs->m_pGroup->m_bID == SVRGROUP(pDoc->m_dwSelectedService))
			{
				if(dwID) // 06/4/14 현승룡 List Data Update를 위해 수정
					AddItemDataUpdate(pSvs, ++listCnt); 
				else
					AddItemData(pSvs); 				
			}
		}
		else if( bType )
		{
			if( pSvs->m_pSvrType->m_bType == SVRTYPE(pDoc->m_dwSelectedService) &&
				pSvs->m_pGroup->m_bID == SVRGROUP(pDoc->m_dwSelectedService))
			{
				if(dwID) // 06/4/14 현승룡 List Data Update를 위해 수정
					AddItemDataUpdate(pSvs, ++listCnt); 
				else
					AddItemData(pSvs); 				
			}
		}
		else
		{
			if(pSvs->m_pGroup->m_bID == SVRGROUP(pDoc->m_dwSelectedService))
			{
				if(dwID) // 06/4/14 현승룡 List Data Update를 위해 수정
					AddItemDataUpdate(pSvs, ++listCnt); 
				else
					AddItemData(pSvs); 				
			}				
		}
	}
	if( listCnt < listTotalCnt-1 ) // 06/4/14 현승룡 List Data Update를 위해 수정
	{
		for(int i = listCnt; i < listTotalCnt-1; i++)
			m_listCtrl.DeleteItem(listCnt+1);

	}
	UpdateWindow();
}


void CServiceList::AddItemData(LPHAPPYSERVICE pService)
{
	LV_ITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = m_listCtrl.GetItemCount();
	TCHAR szText[50];

	for(int i=0; i< MAX_SERVICE_STAT_COLUMN; i++)
	{
		switch(i)
		{
		case 0: lstrcpy(szText, pService->m_strName); break;
		case 1: itoa(pService->m_dwCurSession, szText, 10); break;
		case 2: itoa(pService->m_dwCurUser, szText, 10); break;
		case 3: itoa(pService->m_dwMaxUser, szText, 10); break;
		case 4: itoa(pService->m_dwActiveUser, szText, 10); break;
		case 5: itoa(pService->m_dwPing, szText, 10); break;
		case 6: itoa(pService->m_dwStopCount, szText, 10); break;
		case 7:
			{
				// 06/04/11 현승룡 시간 디폴트 0-0-0 0:0
				if(pService->m_nStopTime)
				{
					CTime t(pService->m_nStopTime);
					CString strTime;
					strTime.Format("%d-%d-%d %d:%d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
					lstrcpy(szText, strTime);
				}
				else
					lstrcpy(szText, "0-0-0 0:0");				
				break;
			}
		case 8:
			{
				// 06/04/11 현승룡 시간 디폴트 0-0-0 0:0
				if(pService->m_nPickTime)
				{
                    CTime t(pService->m_nPickTime);
					CString strTime;
					strTime.Format("%d-%d-%d %d:%d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
					lstrcpy(szText, strTime);
				}
				else
					lstrcpy(szText, "0-0-0 0:0");
				break;
			}
		}
		item.iSubItem = i;
		item.pszText = szText;
		if(i==0) m_listCtrl.InsertItem(&item);
		else m_listCtrl.SetItem(&item);
	}
}
// 06/4/14 현승룡 List Data Update
void CServiceList::AddItemDataUpdate(LPHAPPYSERVICE pService, int listCnt)
{		
	if(listCnt+1 > m_listCtrl.GetItemCount()) 
	{
		AddItemData(pService);
		return;
	}

	TCHAR szText[50];

	for(int i=0; i< MAX_SERVICE_STAT_COLUMN; i++)
	{
		switch(i)
		{
		case 0: lstrcpy(szText, pService->m_strName); break;
		case 1:	itoa(pService->m_dwCurSession, szText, 10);	break;
		case 2:	itoa(pService->m_dwCurUser, szText, 10); break;
		case 3: itoa(pService->m_dwMaxUser, szText, 10); break;
		case 4: itoa(pService->m_dwActiveUser, szText, 10); break;
		case 5: itoa(pService->m_dwPing, szText, 10); break;
		case 6: itoa(pService->m_dwStopCount, szText, 10); break;
		case 7:
			{
				// 06/04/11 현승룡 시간 디폴트 0-0-0 0:0
				if(pService->m_nStopTime)
				{
					CTime t(pService->m_nStopTime);
					CString strTime;
					strTime.Format("%d-%d-%d %d:%d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
					lstrcpy(szText, strTime);					
				}
				else
					lstrcpy(szText, "0-0-0 0:0");				
				break;
			}
		case 8:
			{
				// 06/04/11 현승룡 시간 디폴트 0-0-0 0:0
				if(pService->m_nPickTime)
				{
                    CTime t(pService->m_nPickTime);
					CString strTime;
					strTime.Format("%d-%d-%d %d:%d",t.GetYear(),t.GetMonth(),t.GetDay(),t.GetHour(),t.GetMinute());
					lstrcpy(szText, strTime);
				}
				else
					lstrcpy(szText, "0-0-0 0:0");
				break;
			}
		}

		if(lstrcmp(m_listCtrl.GetItemText(listCnt, i), szText))
			m_listCtrl.SetItemText(listCnt, i, szText);		
	}
}

void CServiceList::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	static int i=0;
	if(i) return;

	char *text[MAX_SERVICE_STAT_COLUMN] =
		{"SERVICE NAME",
		 "CURRENT SESSION",
		 "CURRENT USER",
		 "MAX USER",
		 "ACTIVE USER",
		 "PING",
		 "STOP COUNT",
		 "LATEST START",
		 "PICK TIME"};

	int width[MAX_SERVICE_STAT_COLUMN] = {100,130,100,100,100,100,100,150,150};

	LV_COLUMN lv;
	lv.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lv.fmt = LVCFMT_CENTER;
	for(int j=0; j<MAX_SERVICE_STAT_COLUMN; j++)
	{
		lv.pszText = text[j];
		lv.iSubItem = j;
		lv.cx = width[j];
		m_listCtrl.InsertColumn(j, &lv);
	}
	i++;

	m_listCtrl.SetExtendedStyle(LVS_EX_FULLROWSELECT);
}
