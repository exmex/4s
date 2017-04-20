// 현승룡 ServiceGraph.cpp

// ServiceGraph.cpp : 구현 파일입니다.

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "NetCode.h"
#include "MainFrm.h"
#include ".\servicegraph.h"

// CServiceGraph

IMPLEMENT_DYNCREATE(CServiceGraph, CFormView)

CServiceGraph::CServiceGraph()
	: CFormView(CServiceGraph::IDD)
{
	m_bFlag = TRUE;
	m_dwMaxUser = 100;
	m_dwMaxPing = 100;
	m_dwTotalUser = 0;
	m_bWorldRelay = 0;
	m_vTOTALUSER.clear();
	m_vSERVICEGRAPH.clear();
	m_listFlag = FALSE; // 추가리스트 삭제
	
	m_pImageList.Create(IDB_BITMAP1, 16, 1, RGB(0, 128, 128));

	lineColor[0].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	lineColor[1].CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	lineColor[2].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
	lineColor[3].CreatePen(PS_SOLID, 1, RGB(255, 255, 0));
	lineColor[4].CreatePen(PS_SOLID, 1, RGB(255, 0, 255));
	lineColor[5].CreatePen(PS_SOLID, 1, RGB(128, 0, 0));
	lineColor[6].CreatePen(PS_SOLID, 1, RGB(0, 128, 0));
	lineColor[7].CreatePen(PS_SOLID, 1, RGB(0, 0, 128));
	lineColor[8].CreatePen(PS_SOLID, 1, RGB(128, 128, 0));
	lineColor[9].CreatePen(PS_SOLID, 1, RGB(128, 0, 128));
	lineColor[10].CreatePen(PS_DOT, 1, RGB(255, 0, 0));
	lineColor[11].CreatePen(PS_DOT, 1, RGB(0, 255, 0));
	lineColor[12].CreatePen(PS_DOT, 1, RGB(0, 0, 255));
	lineColor[13].CreatePen(PS_DOT, 1, RGB(255, 255, 0));
	lineColor[14].CreatePen(PS_DOT, 1, RGB(255, 0, 255));
	lineColor[15].CreatePen(PS_DOT, 1, RGB(128, 0, 0));
	lineColor[16].CreatePen(PS_DOT, 1, RGB(0, 128, 0));
	lineColor[17].CreatePen(PS_DOT, 1, RGB(0, 0, 128));
	lineColor[18].CreatePen(PS_DOT, 1, RGB(128, 128, 0));
	lineColor[19].CreatePen(PS_DOT, 1, RGB(128, 0, 128));

	lineTotalColor.CreatePen(PS_SOLID, 2, RGB(255, 100, 255));
}

CServiceGraph::~CServiceGraph()
{
	MemDC.DeleteDC();
	MemBitmap.DeleteObject();

	m_pImageList.DeleteImageList();

	for(BYTE i=0; i<MAX_ADD_SERVICE; i++)
		lineColor[i].DeleteObject();

	lineTotalColor.DeleteObject();

	m_vSERVICEGRAPH.clear();
}

void CServiceGraph::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CServiceGraph, CFormView)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_NOTIFY(LVN_BEGINDRAG, IDC_ADDGRAPHLIST, OnLvnBegindragAddGraphList)
	ON_BN_CLICKED(IDC_CLEARBUTTON, OnBnClickedClearButton)
END_MESSAGE_MAP()


// CServiceGraph 진단입니다.

#ifdef _DEBUG
void CServiceGraph::AssertValid() const
{
	CFormView::AssertValid();
}

void CServiceGraph::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CServiceGraph 메시지 처리기입니다.

void CServiceGraph::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}

void CServiceGraph::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	
	// 윈도우 크기 변경시 
	
	GetClientRect(&m_rectBack);

	m_rectGraph.top = 10;
	m_rectGraph.left = 50;
	m_rectGraph.right = m_rectBack.right - 50;
	m_rectGraph.bottom = m_rectBack.bottom - 200;

	m_rectUser.top = 10;
	m_rectUser.left = 50;
	m_rectUser.right = m_rectGraph.Width()-10;
	m_rectUser.bottom = m_rectGraph.Height()/2-10;

	m_rectPing.top = m_rectGraph.Height()/2+10;
	m_rectPing.left = 50;
	m_rectPing.right = m_rectGraph.Width()-10;
	m_rectPing.bottom = m_rectGraph.Height()-10;

	if(!m_bFlag)
	{
		CClientDC dc(this);
		MemDC.SelectObject(pOldBitmap);
		MemBitmap.DeleteObject();
    	MemBitmap.CreateCompatibleBitmap(&dc, m_rectGraph.Width(), m_rectGraph.Height());
		pOldBitmap = MemDC.SelectObject(&MemBitmap);
		DrawBackGround();
		DrawRect();
		Invalidate(FALSE);
	}

	m_rectList.top = m_rectGraph.bottom + 10;
	m_rectList.left = m_rectBack.left + 50;
	m_rectList.right = m_rectBack.right - 50;
	m_rectList.bottom = m_rectBack.bottom - 10;
	
	m_list.DestroyWindow();
	m_list.Create(WS_VISIBLE | WS_CHILD | WS_BORDER | LVS_REPORT,
		m_rectList, this, IDC_ADDGRAPHLIST);

	m_ClearButton.DestroyWindow();
	m_ClearButton.Create(_T("Clear"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		CRect(5, m_rectList.top, m_rectList.left - 5, m_rectList.top + 30), this, IDC_CLEARBUTTON);
	
	m_list.SetImageList(&m_pImageList, LVSIL_SMALL);

	LV_COLUMN lv;

	lv.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lv.fmt = LVCFMT_CENTER;
	
	char *list[MAX_COL] = {_T("WORLD"), _T("GROUP"), _T("SERVER"), _T("PING"), _T("CURRENT USER"), _T("PEEK USER")};

	lv.iSubItem = 0;
	int width[MAX_COL] = { 110, 110, 110, 60, 110, 80 };

	for(int i = 0; i < MAX_COL; i++)
	{
		lv.cx = width[i];
		lv.pszText = list[i];
		m_list.InsertColumn(i, &lv);
	}
}

void CServiceGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if(m_bFlag)
	{
        MemDC.CreateCompatibleDC(&dc);
		MemBitmap.CreateCompatibleBitmap(&dc, m_rectGraph.Width(), m_rectGraph.Height());
		pOldBitmap = MemDC.SelectObject(&MemBitmap);
		DrawBackGround();
		DrawRect();
		m_bFlag = FALSE;
	}
	dc.BitBlt(m_rectGraph.left, m_rectGraph.top, m_rectGraph.Width(), m_rectGraph.Height(), &MemDC, 0,0, SRCCOPY);
}

void CServiceGraph::ClearGraph() // 추가 서비스 변수 초기화
{
	m_dwMaxUser = 100;
	m_dwMaxPing = 100;
	m_dwTotalUser = 0;
	m_vTOTALUSER.clear();
	m_vSERVICEGRAPH.clear();
	m_list.DeleteAllItems();
	DrawBackGround();
	DrawRect();
	Invalidate(FALSE);
}

void CServiceGraph::DrawBackGround()
{
	if(!MemDC)
		return;

	MemDC.FillSolidRect(0, 0, m_rectGraph.Width(), m_rectGraph.Height(), RGB(255,255,255));

	CString str;

	MemDC.TextOut(5, m_rectUser.bottom-m_rectUser.Height()/2-5, _T("USER"));
	str.Format(_T("%d"), m_dwMaxUser);
	MemDC.TextOut(5, m_rectUser.top, str);
	MemDC.TextOut(15, m_rectUser.bottom-15, _T("0"));

	MemDC.TextOut(5, m_rectPing.bottom-m_rectPing.Height()/2-5, _T("PING"));
	str.Format(_T("%d"), m_dwMaxPing);
	MemDC.TextOut(5, m_rectPing.top, str);
	MemDC.TextOut(15, m_rectPing.bottom-15, _T("0"));
}

void CServiceGraph::DrawRect()
{
	if(!MemDC)
		return;

	MemDC.FillSolidRect(m_rectUser.left-1, m_rectUser.top-1, m_rectUser.Width()+2, m_rectUser.Height()+2, RGB(200,200,200));
	MemDC.FillSolidRect(m_rectPing.left-1, m_rectPing.top-1, m_rectPing.Width()+2, m_rectPing.Height()+2, RGB(200,200,200));

	CPen LineColor(PS_SOLID, 1, RGB(255, 255, 255));
	CPen *oldPen = MemDC.SelectObject(&LineColor);	

	for(BYTE i = 1; i < 10; i++) 
	{
		MemDC.MoveTo(m_rectUser.left, m_rectUser.top + i*m_rectUser.Height()/10); 
		MemDC.LineTo(m_rectUser.right, m_rectUser.top + i*m_rectUser.Height()/10);
	}

	for(BYTE i = 1; i < 10; i++) 
	{
		MemDC.MoveTo(m_rectPing.left, m_rectPing.top + i*m_rectPing.Height()/10); 
		MemDC.LineTo(m_rectPing.right, m_rectPing.top + i*m_rectPing.Height()/10);
	}

	MemDC.SelectObject(oldPen);
	LineColor.DeleteObject();
}

void CServiceGraph::UpdateServiceGraph(DWORD dwID)
{	
	BYTE bFind = 0;
	for(BYTE i=0; i<m_vSERVICEGRAPH.size(); i++)
	{
		if(dwID==m_vSERVICEGRAPH[i])
		{
			bFind=1;
			break;
		}
	}

	if(!bFind)
		return;

	DrawService();
	DrawGraph();
}

void CServiceGraph::DrawService()
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc) return;

	m_dwTotalUser = 0;
	m_bWorldRelay = 0;

	for(BYTE i=0; i<m_vSERVICEGRAPH.size(); i++)
	{
		MAPSERVICEGRAPH::iterator itU = pDoc->m_mapSERVICEGRAPH.find(m_vSERVICEGRAPH[i]);
		if(itU!=pDoc->m_mapSERVICEGRAPH.end() && 
			( *itU).second.m_pSvrType->m_bType == SVRGRP_WORLDSVR || (*itU).second.m_pSvrType->m_bType == SVRGRP_RLYSVR )
		{
			m_bWorldRelay = 1;
			m_dwTotalUser = (*itU).second.m_vCurUser[(*itU).second.m_vCurUser.size()-1];
		}			
	}
	
	for(BYTE i=0; i<m_vSERVICEGRAPH.size(); i++)
	{
		MAPSERVICEGRAPH::iterator find = pDoc->m_mapSERVICEGRAPH.find(m_vSERVICEGRAPH[i]);
		if(find!=pDoc->m_mapSERVICEGRAPH.end())
			DrawServiceList(i, (*find).second);
	}
}

void CServiceGraph::DrawGraph()
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc) return;

	DrawBackGround();
	DrawRect();

	BYTE bResult = 0;
	bResult = DrawTotalUser();

	if(!bResult)
	{
		for(BYTE i=0; i<m_vSERVICEGRAPH.size(); i++)
		{
			MAPSERVICEGRAPH::iterator find = pDoc->m_mapSERVICEGRAPH.find(m_vSERVICEGRAPH[i]);
			if(find!=pDoc->m_mapSERVICEGRAPH.end())
			{
				bResult = DrawServiceGraph((*find).second.m_bColor, (*find).second.m_vCurUser, (*find).second.m_vPing);
				if(bResult) 
					break;
			}
		}
	}

	switch(bResult)
	{
	case 1:
		m_dwMaxUser *= 2;
		DrawGraph();
		break;
	case 2:
		m_dwMaxPing *= 2;
		DrawGraph();
		break;
	case 0:
		Invalidate(FALSE);
		break;
	}
}

void CServiceGraph::DrawServiceList(BYTE bCnt, SERVICEGRAPH sGraph)
{	
	CString str;
	LV_ITEM item;
	item.iItem = bCnt;
	
	if( sGraph.m_bColor >= 10 )
	{
		item.iImage = sGraph.m_bColor - 10;
	}
	else
		item.iImage = sGraph.m_bColor;
	
	item.mask = LVIF_TEXT | LVIF_IMAGE;		

	for(BYTE i = 0; i < MAX_COL; i++)
	{
		item.iSubItem = i;
		switch(i)
		{
		case 0: str = sGraph.m_strWorld; break;
		case 1: str = sGraph.m_strGroup; break;
		case 2: str = sGraph.m_strService; break;
		case 3: str.Format(_T("%d"), sGraph.m_vPing[sGraph.m_vPing.size()-1]); break;		
		case 4: str.Format(_T("%d"), sGraph.m_vCurUser[sGraph.m_vCurUser.size()-1]); break;		
		case 5: str.Format(_T("%d"), sGraph.m_dwPeekUser); break;		
		}
		item.pszText = NULL;
		item.pszText = str.GetBuffer(0);

		if(bCnt < m_list.GetItemCount())
		{
			CString strTemp;
			strTemp = m_list.GetItemText(item.iItem, item.iSubItem);
			if(lstrcmp(strTemp, str))
				m_list.SetItem(&item);
		}
		else
			(i==0) ? m_list.InsertItem(&item) : m_list.SetItem(&item);
	}
	UpdateData(FALSE);

	if(!m_bWorldRelay)
        m_dwTotalUser += sGraph.m_vCurUser[sGraph.m_vCurUser.size()-1];
}

BYTE CServiceGraph::DrawServiceGraph(BYTE bColor, VDWORD vCurUser, VDWORD vPing)
{
	CPen *oldPen;	
	oldPen = MemDC.SelectObject(&lineColor[bColor]);

	int x = m_rectUser.right;
	double y = (double)m_rectUser.Height()/(double)m_dwMaxUser;

	for(DWORD i=(DWORD)vCurUser.size()-1; i>=0; i--)
	{
		if(i==0) break;

		if( vCurUser[i] >= m_dwMaxUser ||
            vCurUser[i-1] >= m_dwMaxUser )
			return 1;

		MemDC.MoveTo(x, (int)((double)m_rectUser.bottom-(double)vCurUser[i]*y));
		x-=WIDTH_LENGTH;
		if(x<=m_rectUser.left)
			x=m_rectUser.left;
		MemDC.LineTo(x, (int)((double)m_rectUser.bottom-(double)vCurUser[i-1]*y));

		if(x==m_rectUser.left) break;
	}

	x = m_rectPing.right;
	y = (double)m_rectPing.Height()/(double)m_dwMaxPing;

	for(DWORD i=(DWORD)vPing.size()-1; i>=0; i--)
	{
		if(i==0) break;

		if( vPing[i] >= m_dwMaxPing ||
            vPing[i-1] >= m_dwMaxPing )
			return 2;

		MemDC.MoveTo(x, (int)((double)m_rectPing.bottom-(double)vPing[i]*y));
		x-=WIDTH_LENGTH;
		if(x<=m_rectPing.left)
			x=m_rectPing.left;
		MemDC.LineTo(x, (int)((double)m_rectPing.bottom-(double)vPing[i-1]*y));

		if(x==m_rectPing.left) break;
	}

	MemDC.SelectObject(oldPen);	

	return 0;
}

BYTE CServiceGraph::DrawTotalUser()
{
	m_vTOTALUSER.push_back(m_dwTotalUser);
	CString str;
	str.Format(_T("%d"), m_dwTotalUser);
	MemDC.TextOut(5,m_rectUser.bottom-m_rectUser.Height()/2+10, str);

	CPen *oldPen;	
	oldPen = MemDC.SelectObject(&lineTotalColor);

	int x = m_rectUser.right;
	double y = (double)m_rectUser.Height()/(double)m_dwMaxUser;

	for(DWORD i=(DWORD)m_vTOTALUSER.size()-1; i>=0; i--)
	{
		if(i==0) break;

		if( m_vTOTALUSER[i] >= m_dwMaxUser ||
            m_vTOTALUSER[i-1] >= m_dwMaxUser )
			return 1;

		MemDC.MoveTo(x, (int)((double)m_rectUser.bottom-(double)m_vTOTALUSER[i]*y));
		x-=WIDTH_LENGTH;
		if(x<=m_rectUser.left)
			x=m_rectUser.left;
		MemDC.LineTo(x, (int)((double)m_rectUser.bottom-(double)m_vTOTALUSER[i-1]*y));

		if(x==m_rectUser.left) break;
	}

	MemDC.SelectObject(oldPen);	

	return 0;
}

// 추가 / 삭제
void CServiceGraph::AddServiceGraph(DWORD dwID)
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc) return;

	if(m_vSERVICEGRAPH.size() == MAX_ADD_SERVICE)
	{
		AfxMessageBox(_T("No more added"));//더 이상 추가할 수 없습니다
		return;
	}

	for(BYTE i=0; i<m_vSERVICEGRAPH.size(); i++)
	{
		if(dwID==m_vSERVICEGRAPH[i])
			return;
	}

	MAPSERVICEGRAPH::iterator it;

	BYTE colorCheck[20] = {0};
	for(BYTE i=0; i<m_vSERVICEGRAPH.size(); i++)
	{
		it = pDoc->m_mapSERVICEGRAPH.find(m_vSERVICEGRAPH[i]);
		if(it!=pDoc->m_mapSERVICEGRAPH.end())
			colorCheck[(*it).second.m_bColor] = 1;
	}

	BYTE bColor;
	for(BYTE i=0; i<MAX_ADD_SERVICE; i++)
	{
		if( colorCheck[i] != 1 )
		{
			bColor = i;
			break;
		}
	}

	it = pDoc->m_mapSERVICEGRAPH.find(dwID);
	if(it!=pDoc->m_mapSERVICEGRAPH.end())
	{
		(*it).second.m_bColor = i;
		m_vSERVICEGRAPH.push_back(dwID);
	}

	DrawGraph();
}

void CServiceGraph::OnLButtonUp(UINT nFlags, CPoint point)
{
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	if(!pDoc) return;

	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = (CServiceTree *)pFrm->m_wndSplitter.GetView(pFrm->m_nTree);	

	// 리스트 추가
	if(pTree->m_DragFlag)
    {
		// 그래프 영역 안
		if(point.x > m_rectGraph.left && point.x < m_rectGraph.right &&	
			point.y > m_rectGraph.top && point.y < m_rectGraph.bottom)
		{
			HTREEITEM hGroup = (pTree->m_treeCtrl).GetParentItem(pTree->m_hDragItem);
			HTREEITEM hWorld = (pTree->m_treeCtrl).GetParentItem(hGroup);

			if( hWorld == NULL )
			{
				CString strWorld = (pTree->m_treeCtrl).GetItemText(hGroup);
				CString strGroup = (pTree->m_treeCtrl).GetItemText(pTree->m_hDragItem);

				MAPSERVICEGRAPH::iterator it; // 서비스 검색
				for(it = pDoc->m_mapSERVICEGRAPH.begin(); it != pDoc->m_mapSERVICEGRAPH.end(); it++)
				{
					SERVICEGRAPH sGraph = (*it).second;
					if( !lstrcmp(strWorld, sGraph.m_strWorld) && 
						!lstrcmp(strGroup, sGraph.m_strGroup) )
						AddServiceGraph(sGraph.m_dwID);
				}
			}
			else
			{
				CString strWorld = (pTree->m_treeCtrl).GetItemText(hWorld);
				CString strGroup = (pTree->m_treeCtrl).GetItemText(hGroup);
				CString strService = (pTree->m_treeCtrl).GetItemText(pTree->m_hDragItem);

				MAPSERVICEGRAPH::iterator it; // 서비스 검색
				for(it = pDoc->m_mapSERVICEGRAPH.begin(); it != pDoc->m_mapSERVICEGRAPH.end(); it++)
				{
					SERVICEGRAPH sGraph = (*it).second;
					if( !lstrcmp(strWorld, sGraph.m_strWorld) &&
						!lstrcmp(strGroup, sGraph.m_strGroup) &&
						!lstrcmp(strService, sGraph.m_strService) )
					{
						AddServiceGraph(sGraph.m_dwID);
						break;
					}
				}
			}
		}		

		ReleaseCapture();
		pTree->m_hDragItem = NULL;
		pTree->m_DragFlag = 0;
		pTree->m_Cursor = LoadCursor(0, IDC_ARROW);
	}

	// 리스트 삭제
	if( m_listFlag )
	{
		ReleaseCapture();
		m_listFlag = FALSE;
		pTree->m_Cursor = LoadCursor(0, IDC_ARROW);

		if( point.x < m_rectList.right && point.x > m_rectList.left &&
			point.y < m_rectList.bottom && point.y > m_rectList.top )
			return;

		for(BYTE i=0; i<m_vSERVICEGRAPH.size(); i++)
		{
			MAPSERVICEGRAPH::iterator find = pDoc->m_mapSERVICEGRAPH.find(m_vSERVICEGRAPH[i]);
			if(find!=pDoc->m_mapSERVICEGRAPH.end())
			{
				if( !lstrcmp((*find).second.m_strWorld, m_list.GetItemText(m_listDrag, 0)) &&
					!lstrcmp((*find).second.m_strGroup, m_list.GetItemText(m_listDrag, 1)) &&
					!lstrcmp((*find).second.m_strService, m_list.GetItemText(m_listDrag, 2)) )
				{
					m_list.DeleteItem(m_listDrag);
					break;
				}
			}
		}

		m_vSERVICEGRAPH.erase(m_vSERVICEGRAPH.begin()+i);
		(m_vSERVICEGRAPH.size() == 0) ?	ClearGraph() : DrawGraph();
	}

	CFormView::OnLButtonUp(nFlags, point);
}

void CServiceGraph::OnMouseMove(UINT nFlags, CPoint point)
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = (CServiceTree *)pFrm->m_wndSplitter.GetView(pFrm->m_nTree);
	if(pTree->m_DragFlag)
		SetCapture();

	CFormView::OnMouseMove(nFlags, point);
}

BOOL CServiceGraph::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{	
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = (CServiceTree *)pFrm->m_wndSplitter.GetView(pFrm->m_nTree);
	SetCursor(pTree->m_Cursor);

	return TRUE;
	//return CFormView::OnSetCursor(pWnd, nHitTest, message);
}

void CServiceGraph::OnLvnBegindragAddGraphList(NMHDR *pNMHDR, LRESULT *pResult) // 추가리스트삭제
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);	

	m_listDrag = pNMLV->iItem;
	m_listFlag = TRUE;

	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CServiceTree * pTree = (CServiceTree *)pFrm->m_wndSplitter.GetView(pFrm->m_nTree);	
	
	pTree->m_Cursor = LoadCursor(0, IDC_WAIT);
	SetCursor(pTree->m_Cursor);
	SetCapture();

	*pResult = 0;
}

void CServiceGraph::OnBnClickedClearButton() // 추가리스트 초기화
{
	ClearGraph();
}