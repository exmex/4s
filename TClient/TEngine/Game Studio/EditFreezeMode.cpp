// EditFreezeMode.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "EditFreezeMode.h"
#include ".\editfreezemode.h"



// CEditFreezeMode 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditFreezeModeDlg, CDialog)
CEditFreezeModeDlg::CEditFreezeModeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditFreezeModeDlg::IDD, pParent)
	, m_nUnitLength(0)
	, m_nTileLength(0)
{
}

CEditFreezeModeDlg::~CEditFreezeModeDlg()
{
	m_pDoc->ClearSELOBJ();;
	m_pDoc->RedrawAllView();
}

void CEditFreezeModeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, ID_BUTTON_NONFREEZE, m_cUnFreeze);
	DDX_Control(pDX, ID_BUTTON_FREEZE, m_cFreeze);
	DDX_Control(pDX, IDC_LIST_FREEZE, m_cFreezeObject);
	DDX_Control(pDX, IDC_LIST_NONFREEZE, m_cNonFreezeObject);
}


BEGIN_MESSAGE_MAP(CEditFreezeModeDlg, CDialog)
	ON_BN_CLICKED(ID_BUTTON_NONFREEZE, OnBnClickedButtonNonfreeze)
	ON_BN_CLICKED(ID_BUTTON_FREEZE, OnBnClickedButtonFreeze)
	ON_NOTIFY(NM_CLICK, IDC_LIST_NONFREEZE, OnNMClickListNonfreeze)
	ON_NOTIFY(NM_CLICK, IDC_LIST_FREEZE, OnNMClickListFreeze)
END_MESSAGE_MAP()


// CEditFreezeMode 메시지 처리기입니다.

BOOL CEditFreezeModeDlg::OnInitDialog(void)
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  
	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon(hIcon, TRUE);			// Set big icon
	SetIcon(hIcon, FALSE);		// Set small icon

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MOVELEFT);
	m_cFreeze.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_MOVERIGHT);
	m_cUnFreeze.SetIcon(hIcon);

	
	// Freeze Object List 속성 지정
	CRect rect;

	CListCtrl *pFreezeListCtrl = m_cFreezeObject.GetListCtrl();
	pFreezeListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pFreezeListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	pFreezeListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());


	// Non-Freeze Object List 속성 지정
	CListCtrl *pNonFreezeListCtrl = m_cNonFreezeObject.GetListCtrl();

	pNonFreezeListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pNonFreezeListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_INFOTIP);
	pNonFreezeListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	MAPDWORD::iterator itID;		// 오브젝트의 이름을 
	COBJItem *pOBJ;					// 검색하기 위한 값들

	// Freeze된 오브젝트들의 ID를 리스트에 넣기
	MAPOBJECT::iterator	itMAPOBJ;
	CHAR strID[128];


	for( itMAPOBJ = m_pDoc->m_mapOBJ.begin() ; itMAPOBJ != m_pDoc->m_mapOBJ.end() ; itMAPOBJ++ )
	{
		itID = m_pDoc->m_pMAP->m_mapOBJ.find( (*itMAPOBJ).first );
		pOBJ = CItemBase::GetOBJItem((*itID).second);
		
		float x = (*itMAPOBJ).second->m_vPosition._41;
		float z = (*itMAPOBJ).second->m_vPosition._43;
		
		int nUNITX = INT(x) / ( m_nUnitLength * m_nTileLength );
		int nUNITZ = INT(z) / ( m_nUnitLength * m_nTileLength );

		nUNITX *= m_nUnitLength;
		nUNITZ *= m_nUnitLength;

		if( m_pDoc->IsFreezeOBJ( (*itMAPOBJ).first ) )
		{	
			sprintf( strID, "%u 번\t (%s) %d : %d", (*itMAPOBJ).first, pOBJ->m_strName, nUNITX, nUNITZ );
			pFreezeListCtrl->InsertItem( pFreezeListCtrl->GetItemCount(), strID  );
		}
		else
		{
			sprintf( strID, "%u 번\t (%s) %d : %d", (*itMAPOBJ).first, pOBJ->m_strName, nUNITX, nUNITZ );
			pNonFreezeListCtrl->InsertItem( pNonFreezeListCtrl->GetItemCount(), strID  );
		}
	}

	return TRUE;
}

void CEditFreezeModeDlg::InitDocument(CMAPDoc* pDoc, int nUNITLength, int nTileLength )
{
	m_pDoc = pDoc;	
	m_nUnitLength = nUNITLength;
	m_nTileLength = nTileLength;
}


void CEditFreezeModeDlg::OnBnClickedButtonNonfreeze()
{
	CString strName;
	CListCtrl *pFreezeList = m_cFreezeObject.GetListCtrl();
	CListCtrl *pNonFreezeList = m_cNonFreezeObject.GetListCtrl();

	int nIndex;
	DWORD dwID = 0;

	POSITION pos = pFreezeList->GetFirstSelectedItemPosition();

	while( ( nIndex = pFreezeList->GetNextSelectedItem(pos)) >= 0 )
	{
		strName = pFreezeList->GetItemText( nIndex, 0);
		
		sscanf(strName, "%u" , &dwID);

		if( m_pDoc->IsFreezeOBJ( dwID ) )
		{
			/*int nCnt = m_pDoc->GetFreezeOBJCNT(dwID);
			if( nCnt >= 0 )
				m_pDoc->m_vFREEZEOBJ.erase( m_pDoc->m_vFREEZEOBJ.begin() + nCnt );*/
			m_pDoc->EraseFREEZEOBJ(dwID);
		}
	}

	pos = pFreezeList->GetFirstSelectedItemPosition();	

	while( ( nIndex = pFreezeList->GetNextSelectedItem(pos)) >= 0 )
	{
		strName = pFreezeList->GetItemText( nIndex, 0);
		pNonFreezeList->InsertItem(pNonFreezeList->GetItemCount(), strName );
		sscanf(strName, "%u" , &dwID);		
		
		pFreezeList->DeleteItem(nIndex);

		pos = pFreezeList->GetFirstSelectedItemPosition();
	}
	
	m_pDoc->ClearSELOBJ();;
	m_pDoc->RedrawAllView();
}

void CEditFreezeModeDlg::OnBnClickedButtonFreeze()
{
	CString strName;
	CListCtrl *pFreezeList = m_cFreezeObject.GetListCtrl();
	CListCtrl *pNonFreezeList = m_cNonFreezeObject.GetListCtrl();

	int nIndex;
	DWORD dwID = 0;

	POSITION pos = pNonFreezeList->GetFirstSelectedItemPosition();

	while( ( nIndex = pNonFreezeList->GetNextSelectedItem(pos)) >= 0 )
	{
		strName = pNonFreezeList->GetItemText( nIndex, 0);
		sscanf(strName, "%u" , &dwID);
		
		if( !m_pDoc->IsFreezeOBJ( dwID ) )
			m_pDoc->AddFREEZEOBJ(dwID);
	}

	pos = pNonFreezeList->GetFirstSelectedItemPosition();	

	while( ( nIndex = pNonFreezeList->GetNextSelectedItem(pos)) >= 0 )
	{
		strName = pNonFreezeList->GetItemText( nIndex, 0);
		pFreezeList->InsertItem(pNonFreezeList->GetItemCount(), strName );
		sscanf(strName, "%u" , &dwID);		
		
		pNonFreezeList->DeleteItem(nIndex);

		pos = pNonFreezeList->GetFirstSelectedItemPosition();
	}
	m_pDoc->ClearSELOBJ();;
	m_pDoc->RedrawAllView();
}

void CEditFreezeModeDlg::OnNMClickListNonfreeze(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;
	int nIndex;
	CString strName;
	CListCtrl *pNonFreezeList		= m_cNonFreezeObject.GetListCtrl();
	
	POSITION pos = pNonFreezeList->GetFirstSelectedItemPosition();
	
	DWORD dwID = 0;

	m_pDoc->ClearSELOBJ();;

	while( ( nIndex = pNonFreezeList->GetNextSelectedItem(pos)) >= 0 )
	{
		
		strName = pNonFreezeList->GetItemText( nIndex, 0);
		sscanf(strName, "%u" , &dwID);
	
		m_pDoc->m_vSELOBJ.push_back( dwID );								
	}
	m_pDoc->RedrawAllView();
}

void CEditFreezeModeDlg::OnNMClickListFreeze(NMHDR *pNMHDR, LRESULT *pResult)
{
	/**pResult = 0;
	int nIndex;
	CString strName;
	CListCtrl *pFreezeList		= m_cFreezeObject.GetListCtrl();
	
	POSITION pos = pFreezeList->GetFirstSelectedItemPosition();
	
	DWORD dwID = 0;

	m_pDoc->ClearSELOBJ();;

	while( ( nIndex = pFreezeList->GetNextSelectedItem(pos)) >= 0 )
	{
		
		strName = pFreezeList->GetItemText( nIndex, 0);
		sscanf(strName, "%u" , &dwID);
	
		m_pDoc->m_vSELOBJ.push_back( dwID );								
	}
	m_pDoc->RedrawAllView();*/
}
