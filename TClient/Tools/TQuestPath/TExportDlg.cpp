// TExportDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TExportDlg.h"


// CTExportDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTExportDlg, CDialog)

CTExportDlg::CTExportDlg( CWnd *pParent /*=NULL*/)
:CDialog(CTExportDlg::IDD, pParent)
{
	m_listCheckImage.Create( IDB_BITMAP_CHECKIMG, 16, 0, RGB( 255, 255, 255));

	m_pTMAPDATA = NULL;
	m_pTVIEW = NULL;
}

CTExportDlg::~CTExportDlg()
{
}

void CTExportDlg::DoDataExchange( CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE_QUEST, m_cTQUEST);
	DDX_Control(pDX, IDC_TREE_MAP, m_cTMAP);
}


BEGIN_MESSAGE_MAP(CTExportDlg, CDialog)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE_QUEST, OnTvnKeydownTreeQuest)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE_MAP, OnTvnKeydownTreeMap)
	ON_NOTIFY(NM_CLICK, IDC_TREE_QUEST, OnNMClickTreeQuest)
	ON_NOTIFY(NM_CLICK, IDC_TREE_MAP, OnNMClickTreeMap)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_MCLEAR_ALL, OnBnClickedButtonMclearAll)
	ON_BN_CLICKED(IDC_BUTTON_MSEL_ALL, OnBnClickedButtonMselAll)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_ALL, OnBnClickedButtonClearAll)
	ON_BN_CLICKED(IDC_BUTTON_SEL_ALL, OnBnClickedButtonSelAll)
	ON_BN_CLICKED(IDC_BUTTON_MEXPORT, OnBnClickedButtonMexport)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnBnClickedButtonExport)
END_MESSAGE_MAP()


// CTExportDlg 메시지 처리기입니다.

BOOL CTExportDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	if( m_pTVIEW && m_pTMAPDATA )
	{
		HTREEITEM hITEM = m_pTVIEW->GetRootItem();
		MAPTMAPDATA::iterator itTMAP;

		m_cTQUEST.SetImageList(
			m_pTVIEW->GetImageList(TVSIL_NORMAL),
			TVSIL_NORMAL);

		m_cTQUEST.SetImageList(
			&m_listCheckImage,
			TVSIL_STATE);

		m_cTMAP.SetImageList(
			m_pTVIEW->GetImageList(TVSIL_NORMAL),
			TVSIL_NORMAL);

		m_cTMAP.SetImageList(
			&m_listCheckImage,
			TVSIL_STATE);

		while(hITEM)
		{
			InitTREE( hITEM, TVI_ROOT);
			hITEM = m_pTVIEW->GetNextSiblingItem(hITEM);
		}

		for( itTMAP = m_pTMAPDATA->begin(); itTMAP != m_pTMAPDATA->end(); itTMAP++)
		{
			LPTMAPDATA pTMAP = (*itTMAP).second;

			if(pTMAP)
			{
				MAPTMAPIMG::iterator itTUNIT;
				CString strTEXT;

				strTEXT.Format( IDS_FMT_MAPNAME, pTMAP->m_dwID);
				hITEM = m_cTMAP.InsertItem( strTEXT, 0, 1, TVI_ROOT);
				m_cTMAP.SetItemData( hITEM, (DWORD_PTR) pTMAP);

				for( itTUNIT = pTMAP->m_mapTUNIT.begin(); itTUNIT != pTMAP->m_mapTUNIT.end(); itTUNIT++)
				{
					LPTMAPIMG pTUNIT = (*itTUNIT).second;

					if(pTUNIT)
					{
						strTEXT.Format( IDS_FMT_UNITNAME, pTUNIT->m_vID.m_wUnitX, pTUNIT->m_vID.m_wUnitZ);
						HTREEITEM hTUNIT = m_cTMAP.InsertItem( strTEXT, 0, 1, hITEM);
						m_cTMAP.SetItemData( hTUNIT, (DWORD_PTR) pTUNIT);
					}
				}
			}
		}
	}

	return TRUE;
}

void CTExportDlg::OnDestroy()
{
	m_cTQUEST.SetImageList(
		NULL,
		TVSIL_NORMAL);

	m_cTQUEST.SetImageList(
		NULL,
		TVSIL_STATE);

	m_cTMAP.SetImageList(
		NULL,
		TVSIL_NORMAL);

	m_cTMAP.SetImageList(
		NULL,
		TVSIL_STATE);
	m_listCheckImage.DeleteImageList();

	CDialog::OnDestroy();
}

void CTExportDlg::InitTREE( HTREEITEM hITEM, HTREEITEM hDEST)
{
	hDEST = m_cTQUEST.InsertItem( m_pTVIEW->GetItemText(hITEM), 0, 1, hDEST);
	m_cTQUEST.SetItemData( hDEST, m_pTVIEW->GetItemData(hITEM));

	hITEM = m_pTVIEW->GetChildItem(hITEM);
	while(hITEM)
	{
		InitTREE( hITEM, hDEST);
		hITEM = m_pTVIEW->GetNextSiblingItem(hITEM);
	}
}

void CTExportDlg::OnNMClickTreeQuest( NMHDR *pNMHDR, LRESULT *pResult)
{
	DWORD dwPOS = GetMessagePos();
	UINT nFlags = 0;

	CPoint point(
		LOWORD(dwPOS),
		HIWORD(dwPOS));

	m_cTQUEST.ScreenToClient(&point);
	HTREEITEM hITEM = m_cTQUEST.HitTest(
		point,
		&nFlags);

	if( hITEM && (nFlags & TVHT_ONITEMSTATEICON) )
		OnCheckTREE( m_cTQUEST, hITEM);

	*pResult = 0;
}

void CTExportDlg::OnNMClickTreeMap( NMHDR *pNMHDR, LRESULT *pResult)
{
	DWORD dwPOS = GetMessagePos();
	UINT nFlags = 0;

	CPoint point(
		LOWORD(dwPOS),
		HIWORD(dwPOS));

	m_cTMAP.ScreenToClient(&point);
	HTREEITEM hITEM = m_cTMAP.HitTest(
		point,
		&nFlags);

	if( hITEM && (nFlags & TVHT_ONITEMSTATEICON) )
		OnCheckTREE( m_cTMAP, hITEM);

	*pResult = 0;
}

void CTExportDlg::OnTvnKeydownTreeQuest( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);

	if( pTVKeyDown && pTVKeyDown->wVKey == VK_SPACE )
	{
		HTREEITEM hITEM = m_cTQUEST.GetSelectedItem();

		if(hITEM)
			OnCheckTREE( m_cTQUEST, hITEM);
	}

	*pResult = 0;
}

void CTExportDlg::OnTvnKeydownTreeMap( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);

	if( pTVKeyDown && pTVKeyDown->wVKey == VK_SPACE )
	{
		HTREEITEM hITEM = m_cTMAP.GetSelectedItem();

		if(hITEM)
			OnCheckTREE( m_cTMAP, hITEM);
	}

	*pResult = 0;
}

void CTExportDlg::OnCheckTREE( CTreeCtrl& cTree,
							   HTREEITEM hITEM)
{
	if(!hITEM)
		return;
	BYTE bCHECK = GetCheck( cTree, hITEM);

	if( bCHECK == TCHECK_CHECK )
	{
		bCHECK = TCHECK_MIXED;
		SetCheck( cTree, hITEM, bCHECK);
	}

	bCHECK = (bCHECK + 1) % TCHECK_COUNT;
	CheckParent( cTree, hITEM, bCHECK);
	CheckChild( cTree, hITEM, bCHECK);
}

void CTExportDlg::CheckChild( CTreeCtrl& cTree,
							  HTREEITEM hITEM,
							  BYTE bCHECK)
{
	HTREEITEM hCHILD = cTree.GetChildItem(hITEM);

	while(hCHILD)
	{
		CheckChild( cTree, hCHILD, bCHECK);
		SetCheck( cTree, hCHILD, bCHECK);

		hCHILD = cTree.GetNextSiblingItem(hCHILD);
	}
}

void CTExportDlg::CheckParent( CTreeCtrl& cTree,
							   HTREEITEM hITEM,
							   BYTE bCHECK)
{
	HTREEITEM hPARENT = cTree.GetParentItem(hITEM);

	if(hPARENT)
	{
		HTREEITEM hCHILD = cTree.GetChildItem(hPARENT);

		while(hCHILD)
		{
			BYTE bCHILD = hITEM == hCHILD ? bCHECK : GetCheck( cTree, hCHILD);

			if( bCHILD == TCHECK_MIXED || bCHECK != bCHILD )
			{
				bCHECK = TCHECK_MIXED;
				break;
			}

			hCHILD = cTree.GetNextSiblingItem(hCHILD);
		}

		if( bCHECK != GetCheck( cTree, hPARENT) )
		{
			CheckParent( cTree, hPARENT, bCHECK);
			SetCheck( cTree, hPARENT, bCHECK);
		}
	}
}

BYTE CTExportDlg::GetCheck( CTreeCtrl& cTree,
						    HTREEITEM hITEM)
{
	return BYTE(cTree.GetItemState( hITEM, TVIS_STATEIMAGEMASK) >> TCHECKMASK_BIT) - 1;
}

void CTExportDlg::SetCheck( CTreeCtrl& cTree,
						    HTREEITEM hITEM,
						    BYTE bCHECK)
{
	cTree.SetItemState(
		hITEM,
		DWORD(bCHECK + 1) << TCHECKMASK_BIT,
		TVIS_STATEIMAGEMASK);
}

void CTExportDlg::OnBnClickedButtonSelAll()
{
	CheckChild( m_cTQUEST, TVI_ROOT, TCHECK_CHECK);
}

void CTExportDlg::OnBnClickedButtonClearAll()
{
	CheckChild( m_cTQUEST, TVI_ROOT, TCHECK_NONE);
}

void CTExportDlg::OnBnClickedButtonMselAll()
{
	CheckChild( m_cTMAP, TVI_ROOT, TCHECK_CHECK);
}

void CTExportDlg::OnBnClickedButtonMclearAll()
{
	CheckChild( m_cTMAP, TVI_ROOT, TCHECK_NONE);
}

void CTExportDlg::OnBnClickedButtonMexport()
{
	CFileDialog dlg(
		FALSE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Map Path Data Files (*.txt)|*.txt||"));

	if( dlg.DoModal() == IDOK )
	{
		HTREEITEM hITEM = m_cTMAP.GetRootItem();

		CStdioFile vFILE(
			dlg.GetPathName(),
			CFile::modeCreate|
			CFile::modeWrite|
			CFile::typeText);

		while(hITEM)
		{
			ExportTMAP( &vFILE, hITEM);
			hITEM = m_cTMAP.GetNextSiblingItem(hITEM);
		}

		AfxMessageBox(IDS_MSG_EXPORT);
	}
}

void CTExportDlg::OnBnClickedButtonExport()
{
	CFileDialog dlg(
		FALSE,
		_T("*.txt"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Quest Term Data Files (*.txt)|*.txt||"));

	if( dlg.DoModal() == IDOK )
	{
		HTREEITEM hITEM = m_cTQUEST.GetRootItem();

		CStdioFile vFILE(
			dlg.GetPathName(),
			CFile::modeCreate|
			CFile::modeWrite|
			CFile::typeText);

		while(hITEM)
		{
			ExportDATA( &vFILE, hITEM);
			hITEM = m_cTQUEST.GetNextSiblingItem(hITEM);
		}

		AfxMessageBox(IDS_MSG_EXPORT);
	}
}

void CTExportDlg::ExportTMAP( CStdioFile *pFILE,
							  HTREEITEM hITEM)
{
	if( GetCheck( m_cTMAP, hITEM) == TCHECK_NONE )
		return;

	LPTMAPDATA pTMAP = (LPTMAPDATA) m_cTMAP.GetItemData(hITEM);
	if(!pTMAP)
		return;

	hITEM = m_cTMAP.GetChildItem(hITEM);
	while(hITEM)
	{
		if( GetCheck( m_cTMAP, hITEM) == TCHECK_CHECK )
		{
			LPTMAPIMG pTUNIT = (LPTMAPIMG) m_cTMAP.GetItemData(hITEM);

			if(pTUNIT)
			{
				MAPT2DPOINT::iterator itTPOINT;
				MAPDWORD::iterator itTLINE;

				for( itTPOINT = pTUNIT->m_mapTPOINT.begin(); itTPOINT != pTUNIT->m_mapTPOINT.end(); itTPOINT++)
				{
					CString strTDATA;

					strTDATA.Format(
						IDS_FMT_MEXPORT,
						TMAPEXPORT_POINT,
						pTMAP->m_dwID,
						pTUNIT->m_vID.m_wUnitX,
						pTUNIT->m_vID.m_wUnitZ,
						(*itTPOINT).first,
						GET2DL_X((*itTPOINT).second),
						GET2DL_Y((*itTPOINT).second));

					pFILE->WriteString(strTDATA);
				}

				for( itTLINE = pTUNIT->m_mapTLINE.begin(); itTLINE != pTUNIT->m_mapTLINE.end(); itTLINE++)
				{
					CString strTDATA;

					strTDATA.Format(
						IDS_FMT_MEXPORT,
						TMAPEXPORT_LINE,
						pTMAP->m_dwID,
						pTUNIT->m_vID.m_wUnitX,
						pTUNIT->m_vID.m_wUnitZ,
						LOWORD((*itTLINE).first),
						HIWORD((*itTLINE).first),
						(*itTLINE).second);

					pFILE->WriteString(strTDATA);
				}
			}
		}

		hITEM = m_cTMAP.GetNextSiblingItem(hITEM);
	}
}

void CTExportDlg::ExportDATA( CStdioFile *pFILE,
							  HTREEITEM hITEM)
{
	if( GetCheck( m_cTQUEST, hITEM) == TCHECK_NONE )
		return;
	LPTQITEM pTQITEM = (LPTQITEM) m_cTQUEST.GetItemData(hITEM);

	if( pTQITEM && pTQITEM->m_pMission &&
		pTQITEM->m_bType == TQITEM_MISSION )
	{
		int nCount = INT(pTQITEM->m_vTTERM.size());

		for( int i=0; i<nCount; i++)
		{
			LPTTITEM pTTITEM = pTQITEM->m_vTTERM[i];

			if(pTTITEM)
			{
				CString strDATA;

				strDATA.Format(
					IDS_FMT_EXPORT,
					pTQITEM->m_pMission->m_dwClassID,
					pTQITEM->m_dwID,
					pTTITEM->m_vTTERM.m_bTermType,
					pTTITEM->m_vTTERM.m_dwTermID,
					pTTITEM->m_dwMonKind,
					pTTITEM->m_dwNpcID,
					pTTITEM->m_dwMapID,
					pTTITEM->m_fPosX,
					pTTITEM->m_fPosY,
					pTTITEM->m_fPosZ);

				pFILE->WriteString(LPCSTR(strDATA));
			}
		}
	}

	hITEM = m_cTQUEST.GetChildItem(hITEM);
	while(hITEM)
	{
		ExportDATA( pFILE, hITEM);
		hITEM = m_cTQUEST.GetNextSiblingItem(hITEM);
	}
}
