// SelectObjByName.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "SelectObjByName.h"
#include ".\selectobjbyname.h"


// SelectObjByName 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSelectObjByName, CDialog)
CSelectObjByName::CSelectObjByName(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectObjByName::IDD, pParent)
	, m_nShowALL(0)
	, m_nUnitLength(0)
	, m_nTileLength(0)
{
}

CSelectObjByName::~CSelectObjByName()
{
}

void CSelectObjByName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OBJECT, m_cSelectObjectByName);
	DDX_Radio(pDX, IDC_RADIO_ENABLEOBJ, m_nShowALL);
}


BEGIN_MESSAGE_MAP(CSelectObjByName, CDialog)
	ON_NOTIFY(NM_CLICK, IDC_LIST_OBJECT, OnNMClickListObject)
	ON_NOTIFY(NM_RDBLCLK, IDC_LIST_OBJECT, OnNMDblclkListObject) // KSH. 오른쪽 더블클릭으로 수정.
	ON_BN_CLICKED(IDC_BUTTON_SELOBJ_DELETE, OnBnClickedButtonSelobjDelete)
	ON_BN_CLICKED(IDC_RADIO_ENABLEOBJ, OnBnClickedRadioEnableobj)
	ON_BN_CLICKED(IDC_RADIO_ALLOBJ, OnBnClickedRadioAllobj)
END_MESSAGE_MAP()


// SelectObjByName 메시지 처리기입니다.
BOOL CSelectObjByName::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);			// Set small icon

	// Object List 속성 지정
	CRect rect;

	CListCtrl *pSBNListCtrl = m_cSelectObjectByName.GetListCtrl();
	pSBNListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pSBNListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	pSBNListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());
	InitOBJList(pSBNListCtrl);

	return TRUE;
}

void CSelectObjByName::OnNMClickListObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	CListCtrl *pSelectList = m_cSelectObjectByName.GetListCtrl();
	CMAPOBJBar* pBar = m_pDoc->GetMAPOBJBar();

	m_pDoc->m_vSELOBJBUF.clear();
	m_pDoc->m_vSELOBJ.clear();

	POSITION pos = pSelectList->GetFirstSelectedItemPosition();
	while(pos)
	{
		int nIndex = pSelectList->GetNextSelectedItem(pos);

		if( nIndex >= 0 )
		{
			DWORD dwID = (DWORD) pSelectList->GetItemData(nIndex);
			m_pDoc->m_vSELOBJ.push_back(dwID);
		}
	}

	m_pDoc->UpdateOBJBAR();
	m_pDoc->UpdateLIGHTBAR();
	m_pDoc->UpdateSFXBAR();
	m_pDoc->UpdateSNDBAR();

	pBar->ResetSELOBJState();					// PSY 06/08/01 Copy Obj Select (Select OBJ OBJBar Pos Text Edit)
	pBar->UpdateData(FALSE);
	m_pDoc->RedrawAllView();
}

void CSelectObjByName::InitDocument( CMAPDoc* pDoc, int nUNITLength, int nTileLength)
{
	m_pDoc = pDoc;
	m_nUnitLength = nUNITLength;
	m_nTileLength = nTileLength;
}

void CSelectObjByName::OnNMDblclkListObject(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	OnBnClickedButtonSelobjDelete();
}

void CSelectObjByName::OnBnClickedButtonSelobjDelete()
{
	CListCtrl *pSelectList = m_cSelectObjectByName.GetListCtrl();
	CMAPOBJBar *pBar = m_pDoc->GetMAPOBJBar();

	POSITION pos = pSelectList->GetFirstSelectedItemPosition();
	while(pos)
	{
		int nIndex = pSelectList->GetNextSelectedItem(pos);

		if( nIndex >= 0 )
		{
			pSelectList->DeleteItem(nIndex);
			pos = pSelectList->GetFirstSelectedItemPosition();
		}
	}
	pBar->OnObjectDelete();

	m_pDoc->ClearSELOBJ();
	m_pDoc->RedrawAllView();
}

void CSelectObjByName::InitOBJList( CListCtrl *pSBNListCtrl)
{
	MAPOBJECT::iterator	itMAPOBJ;
	CHAR strID[128];

	pSBNListCtrl->DeleteAllItems();
	UpdateData(TRUE);

	for( itMAPOBJ = m_pDoc->m_mapOBJ.begin(); itMAPOBJ != m_pDoc->m_mapOBJ.end(); itMAPOBJ++)
		if( m_nShowALL || (!m_pDoc->IsHideOBJ((*itMAPOBJ).first) && !m_pDoc->IsFreezeOBJ((*itMAPOBJ).first)) )
		{
			MAPDWORD::iterator itID = m_pDoc->m_pMAP->m_mapOBJ.find((*itMAPOBJ).first);
			COBJItem *pOBJ = CItemBase::GetOBJItem((*itID).second);

			int nUNITX = INT((*itMAPOBJ).second->m_vPosition._41) / (m_nUnitLength * m_nTileLength);
			int nUNITZ = INT((*itMAPOBJ).second->m_vPosition._43) / (m_nUnitLength * m_nTileLength);

			nUNITX *= m_nUnitLength;
			nUNITZ *= m_nUnitLength;

			sprintf( strID, "%u 번\t (%s) %d : %d", (*itMAPOBJ).first, pOBJ->m_strName, nUNITX, nUNITZ);
			int nIndex = pSBNListCtrl->InsertItem( pSBNListCtrl->GetItemCount(), strID);
			pSBNListCtrl->SetItemData( nIndex, (*itMAPOBJ).first);
		}
}

void CSelectObjByName::OnBnClickedRadioEnableobj()
{
	CListCtrl *pSelectList = m_cSelectObjectByName.GetListCtrl();
	InitOBJList(pSelectList);
}

void CSelectObjByName::OnBnClickedRadioAllobj()
{
	CListCtrl *pSelectList = m_cSelectObjectByName.GetListCtrl();
	InitOBJList(pSelectList);
}
