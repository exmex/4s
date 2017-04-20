// MachineGraph.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "MachineGraph.h"
#include ".\machinegraph.h"
#include "Mainfrm.h"
#include "HappyDoc.h"
#include "Machine.h"


// CMachineGraph

IMPLEMENT_DYNCREATE(CMachineGraph, CFormView)

CMachineGraph::CMachineGraph()
	: CFormView(CMachineGraph::IDD)
{
	m_bFlag = TRUE;

	m_lineColor[0].CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_lineColor[1].CreatePen(PS_SOLID, 1, RGB(0, 200, 0));
	m_lineColor[2].CreatePen(PS_SOLID, 1, RGB(0, 0, 255));
}

CMachineGraph::~CMachineGraph()
{
	MemDC.DeleteDC();
	MemBitmap.DeleteObject();

	m_lineColor[0].DeleteObject();
	m_lineColor[1].DeleteObject();
	m_lineColor[2].DeleteObject();
}

void CMachineGraph::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMachineGraph, CFormView)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CMachineGraph 진단입니다.

#ifdef _DEBUG
void CMachineGraph::AssertValid() const
{
	CFormView::AssertValid();
}

void CMachineGraph::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CMachineGraph 메시지 처리기입니다.

void CMachineGraph::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
}

void CMachineGraph::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	
	GetClientRect(&m_rectBack);

	int height = m_rectBack.Height()/4;

	m_rectCPU.top = m_rectBack.top + 20;
	m_rectCPU.left = m_rectBack.left + 50;
	m_rectCPU.bottom = m_rectCPU.top + height;
	m_rectCPU.right = m_rectBack.right - 20;

	m_rectMEM.top = m_rectBack.Height()/2 - height/2;
	m_rectMEM.left = m_rectBack.left + 50;
	m_rectMEM.bottom = m_rectBack.Height()/2 + height/2;
	m_rectMEM.right = m_rectBack.right - 20;

	m_rectNET.bottom = m_rectBack.bottom - 20;
	m_rectNET.left = m_rectBack.left + 50;
	m_rectNET.top = m_rectNET.bottom - height;
	m_rectNET.right = m_rectBack.right - 20;

	if(!m_bFlag)
	{
		CClientDC dc(this);
		MemDC.SelectObject(pOldBitmap);
		MemBitmap.DeleteObject();
    	MemBitmap.CreateCompatibleBitmap(&dc, m_rectBack.Width(), m_rectBack.Height());
		pOldBitmap = MemDC.SelectObject(&MemBitmap);
		ClearGraph();
		Invalidate(FALSE);
	}
}

BOOL CMachineGraph::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CFormView::OnEraseBkgnd(pDC);
}

void CMachineGraph::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CFormView::OnPaint()을(를) 호출하지 마십시오.

	if(m_bFlag)
	{
		MemDC.CreateCompatibleDC(&dc);
		MemBitmap.CreateCompatibleBitmap(&dc, m_rectBack.Width(), m_rectBack.Height());
		pOldBitmap = MemDC.SelectObject(&MemBitmap);
		ClearGraph();
		m_bFlag = FALSE;
	}

	dc.BitBlt(0, 0, m_rectBack.Width(), m_rectBack.Height(), &MemDC, 0,0, SRCCOPY);
}

void CMachineGraph::UpdateMachineGraph(BYTE bMachineID)
{
	CMainFrame * pFrm = (CMainFrame *)AfxGetMainWnd();
	CMachine * pMachine = (CMachine *)pFrm->m_wndSplitter.GetView(pFrm->m_nMachine);

	BYTE bSelected = pMachine->GetSelectedMachineID();
	if(bSelected != bMachineID)
		return;

	CHappyDoc* pDoc = (CHappyDoc*)pFrm->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrm->GetFirstDocument();
		if(!pDoc)
			return;
	}

	DrawRect();

	MAPPLATFORM::iterator it = pDoc->m_mapPLATFORM.find(bMachineID);
	VPLATFORM vPlatform = (*it).second;
	if(vPlatform.size() == 0 )
		return;

	DWORD bCnt = (DWORD)vPlatform.size()-1;

	CPen *oldPen;
	CString strValue;

	int x = m_rectCPU.right;
	double y = (double)m_rectCPU.Height()/100.0;
	oldPen = MemDC.SelectObject(&m_lineColor[0]);

	strValue.Format(_T("%d %%"), vPlatform[bCnt].m_dwCPU);
	MemDC.TextOut(m_rectCPU.left+m_rectCPU.Width()/2, (m_rectCPU.bottom-m_rectCPU.Height()/2)-5, strValue);

	for(DWORD i=bCnt; i>=0; i--)
	{
		if(i==0) break;

		MemDC.MoveTo(x, (int)((double)m_rectCPU.bottom-(double)vPlatform[i].m_dwCPU*y));
		x-=WIDTH_LENGTH;
		if(x<=m_rectCPU.left)
			x=m_rectCPU.left;
		MemDC.LineTo(x, (int)((double)m_rectCPU.bottom-(double)vPlatform[i-1].m_dwCPU*y));

		if(x==m_rectCPU.left) break;
	}
	MemDC.SelectObject(oldPen);

	x = m_rectMEM.right;
	oldPen = MemDC.SelectObject(&m_lineColor[1]);

	strValue.Format(_T("%d %%"), vPlatform[bCnt].m_dwMEM);
	MemDC.TextOut(m_rectMEM.left+m_rectMEM.Width()/2, m_rectBack.Height()/2-5, strValue);

	for(DWORD i=bCnt; i>=0; i--)
	{
		if(i==0) break;

		MemDC.MoveTo(x, (int)((double)m_rectMEM.bottom-(double)vPlatform[i].m_dwMEM*y));
		x-=WIDTH_LENGTH;
		if(x<=m_rectMEM.left)
			x=m_rectMEM.left;
		MemDC.LineTo(x, (int)((double)m_rectMEM.bottom-(double)vPlatform[i-1].m_dwMEM*y));

		if(x==m_rectMEM.left) break;
	}
	MemDC.SelectObject(oldPen);

	x = m_rectNET.right;
	y = (double)m_rectNET.Height()/500.0;
	oldPen = MemDC.SelectObject(&m_lineColor[2]);

	strValue.Format(_T("%.2f %%"), vPlatform[bCnt].m_fNET);
	MemDC.TextOut(m_rectNET.left+m_rectNET.Width()/2, (m_rectNET.bottom-m_rectNET.Height()/2)-5, strValue);

	for(DWORD i=bCnt; i>=0; i--)
	{
		if(i==0) break;

		BYTE yy = (BYTE)(vPlatform[i].m_fNET * 100);
		MemDC.MoveTo(x, (int)((double)m_rectNET.bottom-(double)yy*y));
		x-=WIDTH_LENGTH;
		if(x<=m_rectNET.left)
			x=m_rectNET.left;

		yy = (BYTE)(vPlatform[i-1].m_fNET * 100);
		MemDC.LineTo(x, (int)((double)m_rectNET.bottom-(double)yy*y));

		if(x==m_rectNET.left) break;
	}
	MemDC.SelectObject(oldPen);

	Invalidate(FALSE);
}

void CMachineGraph::DrawBackGround()
{
	if(!MemDC)
		return;

	MemDC.FillSolidRect(&m_rectBack, RGB(255, 255, 255));

	MemDC.TextOut(10, (m_rectCPU.bottom-m_rectCPU.Height()/2)-5, _T("CPU"));
	MemDC.TextOut(m_rectCPU.left-25, m_rectCPU.top, _T("100"));
	MemDC.TextOut(m_rectCPU.left-10, m_rectCPU.bottom-15, _T("0"));

	MemDC.TextOut(10, m_rectBack.Height()/2-5, _T("MEM"));
	MemDC.TextOut(m_rectMEM.left-25, m_rectMEM.top, _T("100"));
	MemDC.TextOut(m_rectMEM.left-10, m_rectMEM.bottom-15, _T("0"));

	MemDC.TextOut(10, (m_rectNET.bottom-m_rectNET.Height()/2)-5, _T("NET"));
	MemDC.TextOut(m_rectNET.left-25, m_rectNET.top, _T("5.0"));
	MemDC.TextOut(m_rectNET.left-25, m_rectNET.bottom-15, _T("0.0"));
}

void CMachineGraph::DrawRect()
{
	if(!MemDC)
		return;

	MemDC.FillSolidRect(m_rectCPU.left-1, m_rectCPU.top-1, m_rectCPU.Width()+2, m_rectCPU.Height()+2, RGB(200, 200, 200));
	MemDC.FillSolidRect(m_rectMEM.left-1, m_rectMEM.top-1, m_rectMEM.Width()+2, m_rectMEM.Height()+2, RGB(200, 200, 200));
	MemDC.FillSolidRect(m_rectNET.left-1, m_rectNET.top-1, m_rectNET.Width()+2, m_rectNET.Height()+2, RGB(200, 200, 200));

	CPen LineColor(PS_SOLID, 1, RGB(255, 255, 255));
	CPen *oldPen = MemDC.SelectObject(&LineColor);

	int height = m_rectCPU.Height()/10;
	for(int i=1; i<10; i++)
	{
		MemDC.MoveTo(50, m_rectCPU.top+height*i); 
		MemDC.LineTo(m_rectCPU.right, m_rectCPU.top+height*i);
	}

	for(int i=1; i<10; i++)
	{
		MemDC.MoveTo(50, m_rectMEM.top+height*i); 
		MemDC.LineTo(m_rectMEM.right, m_rectMEM.top+height*i);
	}

	for(int i=1; i<10; i++)
	{
		MemDC.MoveTo(50, m_rectNET.top+height*i); 
		MemDC.LineTo(m_rectNET.right, m_rectNET.top+height*i);
	}

	MemDC.SelectObject(oldPen);
	LineColor.DeleteObject();
}

void CMachineGraph::ClearGraph()
{
	DrawBackGround();
	DrawRect();
}