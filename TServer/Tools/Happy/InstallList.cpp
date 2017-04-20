// InstallList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"
#include "InstallList.h"

// CInstallList

IMPLEMENT_DYNCREATE(CInstallList, CFormView)

CInstallList::CInstallList()
	: CFormView(CInstallList::IDD)
{
}

CInstallList::~CInstallList()
{
}

void CInstallList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LB_INSTALL, m_listCtrl);
	DDX_Control(pDX, IDC_LB_INSTALL2, m_listCtrl2);
}

BEGIN_MESSAGE_MAP(CInstallList, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CInstallList 진단입니다.

#ifdef _DEBUG
void CInstallList::AssertValid() const
{
	CFormView::AssertValid();
}

void CInstallList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CInstallList 메시지 처리기입니다.
void CInstallList::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
	{
		CRect rect, rect1, rect2;
		GetClientRect(&rect);
		rect1 = rect;
		rect2 = rect;
		rect1.bottom = rect.bottom/2;
		rect2.top = rect1.bottom +10;

		if (m_listCtrl.GetSafeHwnd())
			m_listCtrl.MoveWindow(&rect1);
		if (m_listCtrl2.GetSafeHwnd())
			m_listCtrl2.MoveWindow(&rect2);
	}
}

void CInstallList::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	static BYTE bStart=FALSE;
	if(bStart) return;

	m_listCtrl.DeleteAllItems();
	m_listCtrl2.DeleteAllItems();

	char *text[MAX_INSTALL_FILE_COLUMN] =
		{"FILE NAME",
		 "FILE SIZE",
		 "CHECKSUM"};

	int width[MAX_INSTALL_FILE_COLUMN] = {250,200,232};

	LV_COLUMN lv;
	lv.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lv.fmt = LVCFMT_LEFT;
	for(int i=0; i<MAX_INSTALL_FILE_COLUMN; i++)
	{
		lv.pszText = text[i];
		lv.iSubItem = i;
		lv.cx = width[i];
		m_listCtrl.InsertColumn(i, &lv);
		m_listCtrl2.InsertColumn(i, &lv);
	}
	bStart = TRUE;
}


BYTE CInstallList::UpdateList(BYTE bListNum, LPQINSTALL pFile)
{	
	CListCtrl* pList;
	if(!pFile)
		return FALSE;

	switch(bListNum)
	{
	case OLD_LIST: 	pList = &m_listCtrl;	break;
	case NEW_LIST: 	pList = &m_listCtrl2;	break;
	default: return FALSE;
	}
	pList->DeleteAllItems();

	LV_ITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = 0;

	for(QINSTALL::iterator it = pFile->begin(); it != pFile->end(); it++)
	{
		CString strText;

		item.iSubItem = 0;
		strText = it->m_strName;
		item.pszText = strText.GetBuffer(0);
		pList->InsertItem(&item);

		item.iSubItem = 1;		
		strText.Format("%d", it->m_dwVal);
		item.pszText = strText.GetBuffer(0);
		pList->SetItem(&item);

		item.iSubItem = 2;
		strText = it->m_strCheck;
		item.pszText = strText.GetBuffer(0);
		pList->SetItem(&item);
		item.iItem++;
	}

	return TRUE;
}