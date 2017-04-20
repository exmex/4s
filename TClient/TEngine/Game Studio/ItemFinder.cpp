// ItemFinder.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MainFrm.h"
#include "ItemFinder.h"

static CString g_strItemType[] =
{
	"Texture",
	"Image",
	"Media",
	"Animation",
	"Mesh",
	"Object",
	"SFX",
	"Map",
	"Control",
	"Frame"
};

// CItemFinder 대화 상자입니다.

IMPLEMENT_DYNAMIC(CItemFinder, CDialog)
CItemFinder::CItemFinder(CWnd* pParent /*=NULL*/)
	: CDialog(CItemFinder::IDD, pParent)
	, m_szFindItem(_T(""))
	, m_nItemType(0)
	, m_hRoot( NULL )
	, m_hCurItem( NULL )
	, m_hFSItem( NULL )
	, m_bFirstSearch( true )
	, m_nStringType(0)
	, m_nLocation(0)
	, m_nStep(0)
{
	CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();
	m_vDocs.push_back( pMainWnd->m_pDocTexture );
	m_vDocs.push_back( pMainWnd->m_pDocImage );
	m_vDocs.push_back( pMainWnd->m_pDocMedia );
	m_vDocs.push_back( pMainWnd->m_pDocAnimation );
	m_vDocs.push_back( pMainWnd->m_pDocMesh );
	m_vDocs.push_back( pMainWnd->m_pDocObject );
	m_vDocs.push_back( pMainWnd->m_pDocSFX );
	m_vDocs.push_back( pMainWnd->m_pDocMAP );
	m_vDocs.push_back( pMainWnd->m_pDocComp );
	m_vDocs.push_back( pMainWnd->m_pDocFrame );
}

CItemFinder::~CItemFinder()
{
}


HTREEITEM CItemFinder::SearchTree( CTreeCtrl& TCtrl, HTREEITEM hItem )
{
	if( hItem == NULL )
		return NULL;

	if( TCtrl.ItemHasChildren( hItem ) )
	{
		HTREEITEM hChild = TCtrl.GetChildItem( hItem );
		if( hChild == NULL )
			return NULL;

		while(1)
		{
			CItemBase* pItem = (CItemBase*)TCtrl.GetItemData( hChild );

			if( pItem->m_bMenuID != 1 && pItem->m_bType != ITEM_OBJTYPE )
			{
				if( m_nStringType == 0 && strstr( pItem->m_strName, m_szFindItem ) != 0 )
					m_stSearch.push( hChild );
				if( m_nStringType == 1 && strstr( pItem->m_strID, m_szFindItem ) != 0 )
					m_stSearch.push( hChild );
				if( m_nStringType == 2 && pItem->m_dwID == (DWORD)_ttoi((LPCTSTR)m_szFindItem) )
					m_stSearch.push( hChild );
			}
			else
			{
				SearchTree( TCtrl, hChild );
			}
			HTREEITEM hNext = TCtrl.GetNextItem( hChild, TVGN_NEXT );
			if( hNext == NULL )
				break;

			hChild = hNext;
		}
	}
	else
	{
		while(1)
		{
			CItemBase* pItem = (CItemBase*)TCtrl.GetItemData( hItem );

			if( pItem->m_bMenuID != 1 && pItem->m_bType != ITEM_OBJTYPE )	//	아이템일때.
			{
				if( m_nStringType == 0 && strstr( pItem->m_strName, m_szFindItem ) != 0 )
					m_stSearch.push( hItem );
				if( m_nStringType == 1 && strstr( pItem->m_strID, m_szFindItem ) != 0 )
					m_stSearch.push( hItem );
				if( m_nStringType == 2 && pItem->m_dwID == (DWORD)_ttoi((LPCTSTR)m_szFindItem) )
					m_stSearch.push( hItem );
			}

			HTREEITEM hNext = TCtrl.GetNextItem( hItem, TVGN_NEXT );
			if( hNext == NULL )
				break;

			hItem = hNext;
		}
	}

	return NULL;
}

void CItemFinder::ClearFinder()
{
	UpdateData();

	m_bFirstSearch = true;
	m_nStep = 0;

	while( !m_stItem.empty() )
		m_stItem.pop();

	while( !m_stSearch.empty() )
		m_stSearch.pop();	
}

void CItemFinder::SetData( CString szItem, int nItemType, int nStringType, int nLocation )
{
	m_szFindItem = szItem;
	m_nItemType = nItemType;
	m_nStringType = nStringType;
	m_nLocation = nLocation;

	UpdateData( FALSE );
}

void CItemFinder::FindItem()
{
	OnBnClickedFindnext();
}

void CItemFinder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_FINDITEMNAME, m_szFindItem);
	DDX_Control(pDX, IDC_COMBO_ITEM, m_ctrlItemType);
	DDX_CBIndex(pDX, IDC_COMBO_ITEM, m_nItemType);
	DDX_Radio(pDX, IDC_RADIO_FINDNAME, m_nStringType);
	DDX_Radio(pDX, IDC_RADIO_FNDALL, m_nLocation);
}

BOOL CItemFinder::OnInitDialog()
{
	CDialog::OnInitDialog();

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();

	std::vector<CGameStudioDoc*>::iterator itor;
	CView* pView = ((CMainFrame*)AfxGetMainWnd())->m_wndWorkSpace.GetActiveView();
	CGameStudioDoc* pDoc = (CGameStudioDoc*)pView->GetDocument();
	int i=0, CBNum = 0;
	for( itor = m_vDocs.begin(); itor != m_vDocs.end(); ++i )
	{
		if( (*itor)->GetTreeCtrl().GetRootItem() != NULL )
		{
			if( pView )
			{
				if( pDoc->m_bType == (*itor)->m_bType )
					m_nItemType = CBNum;
			}
			m_ctrlItemType.AddString( g_strItemType[i] );
			++itor;
			++CBNum;
		}
		else
		{
			itor = m_vDocs.erase( itor );
		}
	}

	m_ctrlItemType.AddString( "All" );

	UpdateData(FALSE);

	return TRUE;
}


BEGIN_MESSAGE_MAP(CItemFinder, CDialog)
	ON_BN_CLICKED(IDC_FINDNEXT, OnBnClickedFindnext)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_CBN_SELCHANGE(IDC_COMBO_ITEM, OnCbnSelchangeComboItem)
	ON_BN_CLICKED(IDC_BUTTON2, OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, OnBnClickedButton3)
	ON_BN_CLICKED(IDC_RADIO_FNDALL, OnBnClickedRadioFndall)
END_MESSAGE_MAP()


// CItemFinder 메시지 처리기입니다.

void CItemFinder::OnBnClickedFindnext()
{
	CString temp = m_szFindItem;
	int strType = m_nStringType;
	int Location = m_nLocation;

	UpdateData(TRUE);
	UpdateData(FALSE);

	//	검색어가 바뀌었다면.. clear found stack.
	if( temp != m_szFindItem )
		ClearFinder();
	if( strType != m_nStringType )
		ClearFinder();
	if( Location != m_nLocation )
		ClearFinder();

	if( m_nStep != 0 && m_nStep >= m_stSearch.size() )
		m_nStep = (int)(m_stSearch.size()-1);

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();

	if( pMainWnd && ( m_szFindItem.GetLength() != 0 ) && ( m_vDocs.size() != 0 ) )
	{
		HTREEITEM hItem = NULL;
		if( m_bFirstSearch )
		{
			//if( m_nItemType == m_ctrlItemType.GetCount()-1 )	//	All.
			//	for( int i=0; i<m_ctrlItemType.GetCount()-1; ++i )
			//	{
			//		SearchTree( m_vDocs[ i ]->GetTreeCtrl(),
			//						m_vDocs[ i ]->GetTreeCtrl().GetRootItem() );
			//	}
			//else
			if( m_nLocation == 0 )	//	폴더 & 파일 이름 모두검색.
			{
				std::vector<HTREEITEM> hList;
				HTREEITEM hRoot;
				CTreeCtrl& cTree = m_vDocs[ m_nItemType ]->GetTreeCtrl();
				hRoot = cTree.GetRootItem();
				hList.push_back( hRoot );

				while(1)
				{
					hRoot = cTree.GetNextItem( hRoot, TVGN_NEXT );
					if( hRoot != NULL )
						hList.push_back( hRoot );
					else
						break;
				}

				for( int i=0; i<hList.size(); ++i )
					SearchTree( m_vDocs[ m_nItemType ]->GetTreeCtrl(), hList[i] );
			}
			else	//	폴더만 검색.
			{
				CTreeCtrl& cTree = m_vDocs[ m_nItemType ]->GetTreeCtrl();
				SearchTree( cTree, cTree.GetSelectedItem() );
			}
		}

		if( !m_stSearch.empty() )
			m_hCurItem = m_stSearch.c[m_nStep++];
		else
		{
			m_hCurItem = NULL;
			ClearFinder();
		}

		if( m_hCurItem )
		{
			//if( m_nItemType == m_ctrlItemType.GetCount()-1 )
			//	for( int i=0; i<m_ctrlItemType.GetCount()-1; ++i )
			//	{
			//		if( m_vDocs[i]->GetTreeCtrl().GetCheck( m_hCurItem ) )
			//		{
			//			m_nItemType = i;
			//			break;
			//		}
			//	}

			m_vDocs[ m_nItemType ]->GetTreeCtrl().SelectItem( m_hCurItem );

			switch( m_vDocs[ m_nItemType ]->m_bType )
			{
			case ITEM_TEXTURE :		pMainWnd->SetActiveWorkspace( 0 );break;
			case ITEM_IMAGE :		pMainWnd->SetActiveWorkspace( 1 );break;
			case ITEM_MEDIA :		pMainWnd->SetActiveWorkspace( 2 );break;
			case ITEM_ANIMATION :	pMainWnd->SetActiveWorkspace( 3 );break;
			case ITEM_MESH :		pMainWnd->SetActiveWorkspace( 4 );break;
			case ITEM_OBJECT :		pMainWnd->SetActiveWorkspace( 5 );break;
			case ITEM_SFX :			pMainWnd->SetActiveWorkspace( 6 );break;
			case ITEM_EXTMAP :		pMainWnd->SetActiveWorkspace( 7 );break;
			case ITEM_COMP :		pMainWnd->SetActiveWorkspace( 8 );break;
			case ITEM_FRAME :		pMainWnd->SetActiveWorkspace( 9 );break;
			}
		}
		else
		{
//			m_stSearch.pop();
			MessageBox( "찾을 수 없습니다.", "Error", MB_ICONERROR );
			ClearFinder();
		}
	}
}

void CItemFinder::PostNcDestroy()
{
	((CMainFrame*)AfxGetMainWnd())->m_pFinderDlg = NULL;
	delete this;

	CDialog::PostNcDestroy();
}


void CItemFinder::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	DestroyWindow();
}

//!	Find Next.
void CItemFinder::OnOK()
{
	OnBnClickedFindnext();
}


void CItemFinder::OnCbnSelchangeComboItem()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	ClearFinder();
}

//!	Find Prev.
void CItemFinder::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();

	if( pMainWnd && m_stSearch.size() > 1 )
	{
//		m_stItem.push( m_hCurItem );
//		m_stSearch.pop();
//		m_hCurItem = m_stSearch.top();
		if( !m_stSearch.empty() )
		{
			if( m_nStep != 0 )
				m_nStep--;

			m_hCurItem = m_stSearch.c[m_nStep];
		}
		else
			m_hCurItem = NULL;

		if( m_hCurItem )
		{
			//for( int i=0; i<m_ctrlItemType.GetCount()-1; ++i )
			//{
			//	if( m_vDocs[ i ]->GetTreeCtrl().SelectItem( m_hCurItem ) )
			//	{
			//		m_nItemType = i;
			//		break;
			//	}
			//}

			m_vDocs[ m_nItemType ]->GetTreeCtrl().SelectItem( m_hCurItem );
			switch( m_vDocs[ m_nItemType ]->m_bType )
			{
			case ITEM_TEXTURE :		pMainWnd->SetActiveWorkspace( 0 );break;
			case ITEM_IMAGE :		pMainWnd->SetActiveWorkspace( 1 );break;
			case ITEM_MEDIA :		pMainWnd->SetActiveWorkspace( 2 );break;
			case ITEM_ANIMATION :	pMainWnd->SetActiveWorkspace( 3 );break;
			case ITEM_MESH :		pMainWnd->SetActiveWorkspace( 4 );break;
			case ITEM_OBJECT :		pMainWnd->SetActiveWorkspace( 5 );break;
			case ITEM_SFX :			pMainWnd->SetActiveWorkspace( 6 );break;
			case ITEM_EXTMAP :		pMainWnd->SetActiveWorkspace( 7 );break;
			case ITEM_COMP :		pMainWnd->SetActiveWorkspace( 8 );break;
			case ITEM_FRAME :		pMainWnd->SetActiveWorkspace( 9 );break;
			}
		}
		else
			MessageBox( "찾지 못했습니다.", "Error", MB_ICONERROR );
	}
}

void CItemFinder::OnBnClickedButton3()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	ClearFinder();
}

void CItemFinder::OnBnClickedRadioFndall()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
