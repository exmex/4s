// PatchList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "HappyDoc.h"

// CPatchList

IMPLEMENT_DYNCREATE(CPatchList, CFormView)

CPatchList::CPatchList()
	: CFormView(CPatchList::IDD)
{
}

CPatchList::~CPatchList()
{
}

void CPatchList::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_listCtrl);
}

BEGIN_MESSAGE_MAP(CPatchList, CFormView)
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPatchList 진단입니다.

#ifdef _DEBUG
void CPatchList::AssertValid() const
{
	CFormView::AssertValid();
}

void CPatchList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG


// CPatchList 메시지 처리기입니다.

void CPatchList::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);

	if (GetSafeHwnd())
	{
		CRect rect;
		GetClientRect(&rect);
		if (m_listCtrl.GetSafeHwnd())
			m_listCtrl.MoveWindow(&rect);
	}
}

void CPatchList::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	static int i=0;
	if(i) return;

	m_listCtrl.DeleteAllItems();

	char *text[MAX_PATCH_FILE_COLUMN] =
		{"LOCAL FILE PATH",
		 "FILE NAME",
		 "FILE SIZE"};

	int width[MAX_PATCH_FILE_COLUMN] = {250,250,200};

	LV_COLUMN lv;
	lv.mask = LVCF_FMT | LVCF_SUBITEM | LVCF_TEXT | LVCF_WIDTH;
	lv.fmt = LVCFMT_LEFT;
	for(int j=0; j<MAX_PATCH_FILE_COLUMN; j++)
	{
		lv.pszText = text[j];
		lv.iSubItem = j;
		lv.cx = width[j];
		m_listCtrl.InsertColumn(j, &lv);
	}
	i++;
}

void CPatchList::UpdatePatchList()
{
	m_listCtrl.DeleteAllItems();

	LV_ITEM item;
	item.mask = LVIF_TEXT;
	item.iItem = m_listCtrl.GetItemCount();
	TCHAR szText[_MAX_PATH];
	CHappyDoc * pDoc = (CHappyDoc *)GetDocument();
	for(DWORD i=0; i<pDoc->m_vPatchFiles.size(); i++)
	{
		LPPATCHFILE pFile = pDoc->m_vPatchFiles[i];
		item.iSubItem = 0;
		lstrcpy(szText, pFile->m_strFullFile);
		item.pszText = szText;
		m_listCtrl.InsertItem(&item);
		item.iSubItem = 1;
		lstrcpy(szText, pFile->m_strFileName);
		item.pszText = szText;
		m_listCtrl.SetItem(&item);
		item.iSubItem = 2;
		itoa(pFile->m_dwSize,szText,10);
		lstrcpy(item.pszText, szText);
		m_listCtrl.SetItem(&item);
	}
	UpdateWindow();
}