// EditHideMode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EditHideMode.h"


// CEditHideMode 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditHideModeDlg, CDialog)
CEditHideModeDlg::CEditHideModeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditHideModeDlg::IDD, pParent)
	, m_nUnitLength(0)
	, m_nTileLength(0)
{
}

CEditHideModeDlg::~CEditHideModeDlg()
{
	m_pDoc->ClearSELOBJ();
	m_pDoc->RedrawAllView();
}

void CEditHideModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_BUTTON_NONHIDE, m_cUnHide);
	DDX_Control(pDX, ID_BUTTON_HIDE, m_cHide);
	DDX_Control(pDX, IDC_LIST_HIDE, m_cHideObject);
	DDX_Control(pDX, IDC_LIST_NONHIDE, m_cNonHideObject);
}


BEGIN_MESSAGE_MAP(CEditHideModeDlg, CDialog)
	ON_BN_CLICKED(ID_BUTTON_NONHIDE, OnBnClickedButtonNonhide)
	ON_BN_CLICKED(ID_BUTTON_HIDE, OnBnClickedButtonHide)
	ON_NOTIFY(NM_CLICK, IDC_LIST_NONHIDE, OnNMClickListNonhide)
END_MESSAGE_MAP()


// CEditHideMode 메시지 처리기입니다.

BOOL CEditHideModeDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);			// Set small icon

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MOVELEFT);
	m_cHide.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MOVERIGHT);
	m_cUnHide.SetIcon(hIcon);

	// Hide Object List 속성 지정
	CRect rect;

	CListCtrl *pHideListCtrl = m_cHideObject.GetListCtrl();
	pHideListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pHideListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	pHideListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	// Non-Hide Object List 속성 지정
	CListCtrl *pNonHideListCtrl = m_cNonHideObject.GetListCtrl();
	pNonHideListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pNonHideListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	pNonHideListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	MAPOBJECT::iterator	itMAPOBJ;
	CHAR strID[128];

	pNonHideListCtrl->DeleteAllItems();
	pHideListCtrl->DeleteAllItems();

	for( itMAPOBJ = m_pDoc->m_mapOBJ.begin(); itMAPOBJ != m_pDoc->m_mapOBJ.end(); itMAPOBJ++)
		if(!m_pDoc->IsFreezeOBJ((*itMAPOBJ).first))
		{
			CListCtrl *pLIST = m_pDoc->IsHideOBJ((*itMAPOBJ).first) ? pHideListCtrl : pNonHideListCtrl;

			MAPDWORD::iterator itID = m_pDoc->m_pMAP->m_mapOBJ.find((*itMAPOBJ).first);
			COBJItem *pOBJ = CItemBase::GetOBJItem((*itID).second);

			int nUNITX = INT((*itMAPOBJ).second->m_vPosition._41) / (m_nUnitLength * m_nTileLength);
			int nUNITZ = INT((*itMAPOBJ).second->m_vPosition._43) / (m_nUnitLength * m_nTileLength);

			nUNITX *= m_nUnitLength;
			nUNITZ *= m_nUnitLength;

			sprintf( strID, "%u 번\t (%s) %d : %d", (*itMAPOBJ).first, pOBJ->m_strName, nUNITX, nUNITZ);
			int nIndex = pLIST->InsertItem( pLIST->GetItemCount(), strID);
			pLIST->SetItemData( nIndex, (*itMAPOBJ).first);
		}

	return TRUE;
}

void CEditHideModeDlg::OnBnClickedButtonNonhide()
{
	CListCtrl *pNonHideList = m_cNonHideObject.GetListCtrl();
	CListCtrl *pHideList = m_cHideObject.GetListCtrl();

	POSITION pos = pHideList->GetFirstSelectedItemPosition();
	while(pos)
	{
		int nIndex = pHideList->GetNextSelectedItem(pos);

		if( nIndex >= 0 )
		{
			int nINSERT = pNonHideList->InsertItem( pNonHideList->GetItemCount(), pHideList->GetItemText( nIndex, 0));
			DWORD dwID = (DWORD) pHideList->GetItemData(nIndex);

			pNonHideList->SetItemData( nINSERT, dwID);
			pHideList->DeleteItem(nIndex);
			m_pDoc->EraseHIDEOBJ(dwID);

			pos = pHideList->GetFirstSelectedItemPosition();
		}
	}

	m_pDoc->RedrawAllView();
}

void CEditHideModeDlg::OnBnClickedButtonHide()
{
	CMAPOBJBar* pBar = m_pDoc->GetMAPOBJBar();

	CListCtrl *pNonHideList = m_cNonHideObject.GetListCtrl();
	CListCtrl *pHideList = m_cHideObject.GetListCtrl();

	POSITION pos = pNonHideList->GetFirstSelectedItemPosition();
	while(pos)
	{
		int nIndex = pNonHideList->GetNextSelectedItem(pos);

		if( nIndex >= 0 )
		{
			int nINSERT = pHideList->InsertItem( pHideList->GetItemCount(), pNonHideList->GetItemText( nIndex, 0));
			DWORD dwID = (DWORD) pNonHideList->GetItemData(nIndex);

			pHideList->SetItemData( nINSERT, dwID);
			pNonHideList->DeleteItem(nIndex);
			m_pDoc->AddHIDEOBJ(dwID);

			pos = pNonHideList->GetFirstSelectedItemPosition();
		}
	}

	m_pDoc->ClearSELOBJ();
	m_pDoc->UpdateOBJBAR();
	m_pDoc->UpdateLIGHTBAR();
	m_pDoc->UpdateSFXBAR();
	m_pDoc->UpdateSNDBAR();

	pBar->UpdateData(FALSE);
	m_pDoc->RedrawAllView();
}

void CEditHideModeDlg::InitDocument( CMAPDoc* pDoc, int nUNITLength, int nTileLength)
{
	m_pDoc = pDoc;
	m_nUnitLength = nUNITLength;
	m_nTileLength = nTileLength;
}

void CEditHideModeDlg::OnNMClickListNonhide(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	CListCtrl *pNonHideList = m_cNonHideObject.GetListCtrl();
	CMAPOBJBar* pBar = m_pDoc->GetMAPOBJBar();

	m_pDoc->m_vSELOBJBUF.clear();
	m_pDoc->m_vSELOBJ.clear();

	POSITION pos = pNonHideList->GetFirstSelectedItemPosition();
	while(pos)
	{
		int nIndex = pNonHideList->GetNextSelectedItem(pos);

		if( nIndex >= 0 )
		{
			DWORD dwID = (DWORD) pNonHideList->GetItemData(nIndex);
			m_pDoc->m_vSELOBJ.push_back(dwID);
		}
	}

	m_pDoc->UpdateOBJBAR();
	m_pDoc->UpdateLIGHTBAR();
	m_pDoc->UpdateSFXBAR();
	m_pDoc->UpdateSNDBAR();

	pBar->ResetSELOBJState();
	pBar->UpdateData(FALSE);
	m_pDoc->RedrawAllView();
}
