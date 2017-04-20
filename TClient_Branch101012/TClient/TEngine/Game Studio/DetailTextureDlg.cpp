// DetailTextureDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Resource.h"
#include "EXTMAPDoc.h"
#include "Game StudioView.h"
#include "DetailTextureDlg.h"
#include "ReplaceDetailTexDlg.h"


CDetailTextureDlg CDetailTextureDlg::m_vDETAILTEXDlg;
// CDetailTextureDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDetailTextureDlg, CDialog)
CDetailTextureDlg::CDetailTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailTextureDlg::IDD, pParent)
	, m_strSCALE(_T(""))
{
	m_pItem = NULL;
}

CDetailTextureDlg::~CDetailTextureDlg()
{
}

void CDetailTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DETTEX, m_cLIST);
	DDX_Text(pDX, IDC_EDIT_SCALE, m_strSCALE);
	DDX_Control(pDX, IDC_EDIT_SCALE, m_cSCALE);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_cDELETE);
	DDX_Control(pDX, IDC_BUTTON_REPLACE, m_cREPLACE);
	DDX_Control(pDX, IDC_BUTTON_DOWN, m_cDOWN);
	DDX_Control(pDX, IDC_BUTTON_UP, m_cUP);
}


BEGIN_MESSAGE_MAP(CDetailTextureDlg, CDialog)
	ON_NOTIFY(LVN_DELETEITEM, IDC_LIST_DETTEX, OnLvnDeleteitemListDettex)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_DETTEX, OnLvnItemchangedListDettex)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_DETTEX, OnLvnBegindragListDettex)
	ON_EN_CHANGE(IDC_EDIT_SCALE, OnEnChangeEditScale)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, OnBnClickedButtonReplace)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_UP, OnBnClickedButtonUp)
	ON_BN_CLICKED(IDC_BUTTON_DOWN, OnBnClickedButtonDown)
	ON_BN_CLICKED(IDC_BUTTON_IMPORT, OnBnClickedButtonImport)
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnBnClickedButtonExport)
END_MESSAGE_MAP()

BOOL CDetailTextureDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	CListCtrl *pLIST = m_cLIST.GetListCtrl();
	CRect rect;

	pLIST->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	pLIST->GetClientRect(&rect);
	rect.DeflateRect( 8, 0);
	pLIST->InsertColumn( 0, _T("Current"), LVCFMT_LEFT, rect.Width() / 2);
	pLIST->InsertColumn( 1, _T("Replace to"), LVCFMT_LEFT, rect.Width() / 2);

	m_cLIST.m_DropTarget.Register(&m_cLIST);
	m_cLIST.m_pDragEnter = OnDragOver;
	m_cLIST.m_pDragOver = OnDragOver;
	m_cLIST.m_pDrop = OnDrop;
	m_cLIST.m_pLeave = OnDragLeave;
	EnableAllCtrl(TRUE);

	return TRUE;
}

void CDetailTextureDlg::StartEdit( CWnd *pParent, CEXTMAPItem *pItem)
{
	if( m_vDETAILTEXDlg.GetSafeHwnd() &&
		m_vDETAILTEXDlg.IsWindowVisible() &&
		m_vDETAILTEXDlg.m_pItem != pItem &&
		m_vDETAILTEXDlg.MessageBox(
		_T("Changed things of current detail texture set would be lost by this command.\nDo you want to continue ?"),
		_T("Game Studio"), MB_YESNO) == IDNO )
		return;

	if(!pItem)
	{
		if(m_vDETAILTEXDlg.GetSafeHwnd())
		{
			m_vDETAILTEXDlg.ShowWindow(SW_HIDE);
			m_vDETAILTEXDlg.SetItem(pItem);
		}

		return;
	}

	if(!pParent)
		pParent = AfxGetMainWnd();

	if(!m_vDETAILTEXDlg.GetSafeHwnd())
		m_vDETAILTEXDlg.Create( CDetailTextureDlg::IDD, pParent);

	if( pParent != m_vDETAILTEXDlg.GetParent() )
		m_vDETAILTEXDlg.SetParent(pParent);

	m_vDETAILTEXDlg.SetItem(pItem);
	m_vDETAILTEXDlg.ShowWindow(SW_SHOW);
}

DWORD CDetailTextureDlg::__ResetTILE( DWORD_PTR lParam)
{
	((CDetailTextureDlg *) ((LPBMPARAM) lParam)->m_pHost)->ResetTILE((LPBMPARAM) lParam);
	return 0;
}

void CALLBACK CDetailTextureDlg::OnDragLeave( CGSListCtrl *pList)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pList->CListView::OnDragLeave();
}

BYTE CDetailTextureDlg::CanInsert( CGSListCtrl *pList, DWORD dwTextureID)
{
	CListCtrl *pLIST = pList->GetListCtrl();
	int nCount = pLIST->GetItemCount();

	if(!dwTextureID)
		return FALSE;

	for( int i=0; i<nCount; i++)
	{
		LPDETTEX pDETTEX = (LPDETTEX) pLIST->GetItemData(i);

		if( pDETTEX && pDETTEX->m_dwTEXID == dwTextureID )
			return FALSE;
	}

	return TRUE;
}

BYTE CDetailTextureDlg::CanDrop( CGSListCtrl *pList, CItemBase *pItem)
{
	return pItem && pItem->m_bType == ITEM_TEXTURE && CanInsert( pList, pItem->m_dwID) ? TRUE : FALSE;
}

BYTE CDetailTextureDlg::CanReplace( int nIndex)
{
	CListCtrl *pLIST = m_cLIST.GetListCtrl();
	int nCount = pLIST->GetItemCount();

	if( nIndex < 0 || nIndex >= nCount || pLIST->GetItemData(nIndex) == NULL )
		return FALSE;

	LPDETTEX pDETTEX = (LPDETTEX) pLIST->GetItemData(nIndex);
	DWORD dwTexID = pDETTEX->m_dwTEXID;

	for( int i=0; i<nCount; i++)
	{
		pDETTEX = (LPDETTEX) pLIST->GetItemData(i);

		if( pDETTEX->m_dwTEXID != pDETTEX->m_dwREPID && pDETTEX->m_dwREPID == dwTexID )
			return FALSE;
	}

	return TRUE;
}

DROPEFFECT CALLBACK CDetailTextureDlg::OnDragOver( CGSListCtrl *pList,
												   COleDataObject *pDataObject,
												   DWORD dwKeyState,
												   CPoint point)
{
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_GSLIST	:
		{
			LPGSLISTITEM pItem = (LPGSLISTITEM) pData->m_pData;

			if( pItem->m_pHost == pList )
			{
				int nIndex = pList->GetListCtrl()->HitTest(point);

				if( nIndex != pList->GetCurSel() )
				{
					CGameStudioView::m_pDragImage->DragMove( CPoint(
						-10000,
						-10000));

					pList->SetCurSel(nIndex);
				}

				nResult = DROPEFFECT_MOVE;
			}
		}

		break;

	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanDrop( pList, pItem))
			{
				int nIndex = pList->GetListCtrl()->HitTest(point);

				if( nIndex != pList->GetCurSel() )
				{
					CGameStudioView::m_pDragImage->DragMove( CPoint(
						-10000,
						-10000));

					pList->SetCurSel(nIndex);
				}

				nResult = DROPEFFECT_MOVE;
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	pList->ClientToScreen(&screenpos);
	CGameStudioView::m_pDragImage->DragMove(screenpos);

	return nResult;
}

BOOL CALLBACK CDetailTextureDlg::OnDrop( CGSListCtrl *pList,
										 COleDataObject* pDataObject,
										 DROPEFFECT dropEffect,
										 CPoint point)
{
	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_GSLIST	:
		{
			LPGSLISTITEM pItem = (LPGSLISTITEM) pData->m_pData;

			if( pItem->m_pHost == pList )
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				((CDetailTextureDlg *) pList->GetParent())->MoveTexture( pItem->m_nIndex, pList->GetListCtrl()->HitTest(point));
			}
		}

		break;

	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanDrop( pList, pItem))
			{
				CListCtrl *pLIST = pList->GetListCtrl();
				int nIndex = pLIST->GetItemCount();

				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				nIndex = pLIST->InsertItem( nIndex, pItem->m_strName);
				LPDETTEX pDETTEX = new DETTEX();

				pDETTEX->m_dwTEXID = pItem->m_dwID;
				pDETTEX->m_dwREPID = pItem->m_dwID;

				pLIST->SetItemData( nIndex, (DWORD_PTR) pDETTEX);
				pLIST->SetItemText( nIndex, 1, pItem->m_strName);
				pList->SetCurSel(nIndex);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);
	OnDragLeave(pList);

	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

void CDetailTextureDlg::OnLvnBegindragListDettex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	HGLOBAL hGlobal = ::GlobalAlloc( GMEM_SHARE|GMEM_MOVEABLE, sizeof(GSCDATA));
	COleDataSource source;
	GSLISTITEM vItem;

	CGameStudioView::m_pDragImage = m_cLIST.GetListCtrl()->CreateDragImage(
		pNMLV->iItem,
		&pNMLV->ptAction);

	CGameStudioView::m_pDragImage->BeginDrag( 0, CPoint(0, 0));
	CGameStudioView::m_pDragImage->DragEnter(
		GetDesktopWindow(),
		CPoint( -10000, -10000));

	vItem.m_nIndex = pNMLV->iItem;
	vItem.m_pHost = &m_cLIST;

	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	pData->m_pData = (LPVOID) &vItem;
	pData->m_bType = GSCT_GSLIST;
	::GlobalUnlock(hGlobal);

	source.CacheGlobalData( CGameStudioView::m_cfGS, hGlobal);
	source.DoDragDrop();

	CGameStudioView::m_pDragImage->DragLeave(NULL);
	CGameStudioView::m_pDragImage->EndDrag();

	::GlobalFree(hGlobal);
	CGameStudioView::InitDragResource();

	*pResult = 0;
}

void CDetailTextureDlg::SetItem( CEXTMAPItem *pItem)
{
	CListCtrl *pLIST = m_cLIST.GetListCtrl();

	pLIST->DeleteAllItems();
	m_pItem = pItem;

	if(m_pItem)
	{
		int nCount = INT(m_pItem->m_vDETAIL.size());

		for( int i=0; i<nCount; i++)
		{
			CTextureItem *pTEX = CItemBase::GetTextureItem(m_pItem->m_vDETAIL[i]);
			CString strNAME;

			if(!pTEX)
				strNAME.Format( _T("Unknown (ID : 0x%08X)"), m_pItem->m_vDETAIL[i]);
			else
				strNAME = pTEX->m_strName;

			if(CanInsert( &m_cLIST, m_pItem->m_vDETAIL[i]))
			{
				int nIndex = pLIST->InsertItem( pLIST->GetItemCount(), strNAME);
				LPDETTEX pDETTEX = new DETTEX();

				pDETTEX->m_dwTEXID = m_pItem->m_vDETAIL[i];
				pDETTEX->m_dwREPID = m_pItem->m_vDETAIL[i];
				pDETTEX->m_fSCALE = m_pItem->m_vDETAILS[i];

				pLIST->SetItemData( nIndex, (DWORD_PTR) pDETTEX);
				pLIST->SetItemText( nIndex, 1, strNAME);
			}
		}
	}
}

void CDetailTextureDlg::MoveTexture( int nFROM, int nTO)
{
	if( nFROM == nTO )
		return;
	CListCtrl *pLIST = m_cLIST.GetListCtrl();

	if( nFROM < 0 || nFROM >= pLIST->GetItemCount() )
		return;

	LPDETTEX pDETTEX = (LPDETTEX) pLIST->GetItemData(nFROM);
	CString strNAME = pLIST->GetItemText( nFROM, 0);
	CString strREP = pLIST->GetItemText( nFROM, 1);

	pLIST->SetItemData( nFROM, NULL);
	pLIST->DeleteItem(nFROM);

	nTO = pLIST->InsertItem( nTO, strNAME);
	pLIST->SetItemData( nTO, (DWORD_PTR) pDETTEX);
	pLIST->SetItemText( nTO, 1, strREP);
	m_cLIST.SetCurSel(nTO);
}

void CDetailTextureDlg::EnableAllCtrl( BYTE bEnable)
{
	CListCtrl *pLIST = m_cLIST.GetListCtrl();
	int nIndex = m_cLIST.GetCurSel();

	bEnable = !bEnable || nIndex < 0 || nIndex >= pLIST->GetItemCount() ? FALSE : TRUE;
	m_cSCALE.EnableWindow(bEnable);

	m_cREPLACE.EnableWindow(bEnable && CanReplace(nIndex) ? TRUE : FALSE);
	m_cDELETE.EnableWindow(bEnable);
	m_cDOWN.EnableWindow(bEnable);
	m_cUP.EnableWindow(bEnable);
}

void CDetailTextureDlg::OnLvnDeleteitemListDettex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	LPDETTEX pDETTEX = (LPDETTEX) pNMLV->lParam;

	if(pDETTEX)
		delete pDETTEX;

	*pResult = 0;
}

void CDetailTextureDlg::OnLvnItemchangedListDettex(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( m_cLIST.m_bUISelect && (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		LPDETTEX pDETTEX = (LPDETTEX) pNMLV->lParam;

		if(pDETTEX)
		{
			m_strSCALE.Format( _T("%.2f"), pDETTEX->m_fSCALE);
			UpdateData(FALSE);
		}
	}
	EnableAllCtrl(TRUE);

	*pResult = 0;
}

void CDetailTextureDlg::OnEnChangeEditScale()
{
	CListCtrl *pLIST = m_cLIST.GetListCtrl();
	int nIndex = m_cLIST.GetCurSel();

	if( nIndex < 0 || nIndex >= pLIST->GetItemCount() )
		return;

	LPDETTEX pDETTEX = (LPDETTEX) pLIST->GetItemData(nIndex);
	UpdateData();
	pDETTEX->m_fSCALE = (FLOAT) atof(LPCSTR(m_strSCALE));
}

void CDetailTextureDlg::OnBnClickedButtonReplace()
{
	CReplaceDetailTexDlg dlg;

	dlg.m_pLIST = m_cLIST.GetListCtrl();
	if( CanReplace(m_cLIST.GetCurSel()) && dlg.DoModal() == IDOK )
	{
		CListCtrl *pLIST = m_cLIST.GetListCtrl();
		int nIndex = m_cLIST.GetCurSel();

		if( dlg.m_dwTexID && nIndex >= 0 && nIndex < pLIST->GetItemCount() )
		{
			LPDETTEX pDETTEX = (LPDETTEX) pLIST->GetItemData(nIndex);

			if(pDETTEX)
			{
				pLIST->SetItemText( nIndex, 1, dlg.m_strNAME);
				pDETTEX->m_dwREPID = dlg.m_dwTexID;
			}
		}
	}
}

void CDetailTextureDlg::OnBnClickedButtonDelete()
{
	CListCtrl *pLIST = m_cLIST.GetListCtrl();
	int nIndex = m_cLIST.GetCurSel();

	if( nIndex < 0 || nIndex >= pLIST->GetItemCount() )
		return;
	pLIST->DeleteItem(nIndex);
}

void CDetailTextureDlg::OnBnClickedButtonUp()
{
	CListCtrl *pLIST = m_cLIST.GetListCtrl();

	int nCount = pLIST->GetItemCount();
	int nIndex = m_cLIST.GetCurSel();

	if( nIndex < 0 || nIndex >= nCount )
		return;

	int nTO = nIndex > 0 ? nIndex : nCount;
	nTO--;
	MoveTexture( nIndex, nTO);
}

void CDetailTextureDlg::OnBnClickedButtonDown()
{
	CListCtrl *pLIST = m_cLIST.GetListCtrl();

	int nCount = pLIST->GetItemCount();
	int nIndex = m_cLIST.GetCurSel();

	if( nIndex < 0 || nIndex >= nCount )
		return;

	int nTO = nCount == 0 ? 0 : (nIndex + 1) % nCount;
	MoveTexture( nIndex, nTO);
}

void CDetailTextureDlg::OnBnClickedButtonImport()
{
	CFileDialog dlg(
		TRUE,
		_T("*.dpl"),
		NULL,
		OFN_HIDEREADONLY,
		_T("Detail Palette Files (*.dpl)|*.dpl; *.dds|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CStdioFile file(
			dlg.GetPathName(),
			CFile::modeRead|
			CFile::typeText);

		CListCtrl *pLIST = m_cLIST.GetListCtrl();
		CString strLINE;

		pLIST->DeleteAllItems();
		while(file.ReadString(strLINE))
		{
			strLINE.Trim();

			if(!strLINE.IsEmpty())
			{
				DWORD dwTexID = 0;
				FLOAT fScale = 0.0f;

				sscanf( LPCSTR(strLINE), _T("%u %f"), &dwTexID, &fScale);
				CTextureItem *pItem = CItemBase::GetTextureItem(dwTexID);
				CString strNAME;

				if(!pItem)
					strNAME.Format( _T("Unknown (ID : 0x%08X)"), dwTexID);
				else
					strNAME = pItem->m_strName;

				if(CanInsert( &m_cLIST, dwTexID))
				{
					int nIndex = pLIST->InsertItem( pLIST->GetItemCount(), strNAME);
					LPDETTEX pDETTEX = new DETTEX();

					pDETTEX->m_dwTEXID = dwTexID;
					pDETTEX->m_dwREPID = dwTexID;
					pDETTEX->m_fSCALE = fScale;

					pLIST->SetItemData( nIndex, (DWORD_PTR) pDETTEX);
					pLIST->SetItemText( nIndex, 1, strNAME);
				}
			}
		}
	}
}

void CDetailTextureDlg::OnBnClickedButtonExport()
{
	CFileDialog dlg(
		FALSE,
		_T("*.dpl"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Detail Palette Files (*.dpl)|*.dpl; *.dds|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CStdioFile file(
			dlg.GetPathName(),
			CFile::modeCreate|
			CFile::modeWrite|
			CFile::typeText);

		CListCtrl *pLIST = m_cLIST.GetListCtrl();
		int nCount = pLIST->GetItemCount();

		for( int i=0; i<nCount; i++)
		{
			LPDETTEX pDETTEX = (LPDETTEX) pLIST->GetItemData(i);
			CString strLINE;

			strLINE.Format(
				_T("%u %.2f //%s\n"),
				pDETTEX->m_dwTEXID,
				pDETTEX->m_fSCALE,
				pLIST->GetItemText( i, 0));

			file.WriteString(strLINE);
		}
	}
}

void CDetailTextureDlg::OnCancel()
{
	SetItem(NULL);
	CDialog::OnCancel();
}

void CDetailTextureDlg::OnOK()
{
	if(m_pItem)
	{
		CEXTMAPDoc *pDoc = (CEXTMAPDoc *) m_pItem->GetMainDocument();

		if(pDoc)
			pDoc->UpdateData();

		CProgressDlg Progress;
		BMPARAM Param;

		Param.m_pHost = (CEXTMAPItem *) this;
		Param.m_strFileName.Empty();
		Param.m_pWnd = &Progress;

		Progress.m_strCancel.LoadString(IDS_CANCEL_RESETDETTEX);
		Progress.m_strTitle.LoadString(IDS_RESET_DETTEX);
		Progress.m_pThreadFunc = (AFX_THREADPROC) CDetailTextureDlg::__ResetTILE;
		Progress.m_lParam = &Param;
		Progress.DoModal();

		if(pDoc)
		{
			pDoc->Reload();
			pDoc->ResetDETAIL();
			pDoc->RefreshView();
		}
	}
	SetItem(NULL);

	CDialog::OnOK();
}

void CDetailTextureDlg::ResetTILE( LPBMPARAM lParam)
{
	lParam->m_pWnd->m_strStatus.LoadString(IDS_EXP_GEO);
	lParam->m_pWnd->m_cProgressBar.SetPos(0);
	lParam->m_pWnd->m_bPercent = 0;
	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_UPDATE);

	if(!m_pItem)
	{
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_EXIT);

		return;
	}

	CListCtrl *pLIST = m_cLIST.GetListCtrl();
	int nCount = pLIST->GetItemCount();

	VECTORDWORD vDETAIL;
	MAPFLOAT mapDETAILS;

	MAPDWORD mapREPLACE;
	MAPDWORD mapINDEX;
	MAPDWORD mapTILE;

	mapDETAILS.clear();
	vDETAIL.clear();

	mapREPLACE.clear();
	mapINDEX.clear();
	mapTILE.clear();

	for( int i=0; i<nCount; i++)
	{
		LPDETTEX pDETTEX = (LPDETTEX) pLIST->GetItemData(i);

		if( pDETTEX && pDETTEX->m_dwREPID )
		{
			if( mapREPLACE.find(pDETTEX->m_dwTEXID) == mapREPLACE.end() )
				mapREPLACE.insert( MAPDWORD::value_type( pDETTEX->m_dwTEXID, pDETTEX->m_dwREPID));

			if( mapDETAILS.find(pDETTEX->m_dwTEXID) == mapDETAILS.end() )
				mapDETAILS.insert( MAPFLOAT::value_type( pDETTEX->m_dwTEXID, pDETTEX->m_fSCALE));

			if( mapTILE.find(pDETTEX->m_dwREPID) == mapTILE.end() )
			{
				mapTILE.insert( MAPDWORD::value_type( pDETTEX->m_dwREPID, DWORD(vDETAIL.size())));
				vDETAIL.push_back(pDETTEX->m_dwREPID);
			}
		}
	}

	nCount = INT(m_pItem->m_vDETAIL.size());
	for( i=0; i<nCount; i++)
	{
		BYTE bTILE = TILE_NULL;

		if(m_pItem->m_vDETAIL[i])
		{
			MAPDWORD::iterator itREPLACE = mapREPLACE.find(m_pItem->m_vDETAIL[i]);

			if( itREPLACE != mapREPLACE.end() && (*itREPLACE).second )
			{
				MAPDWORD::iterator finder = mapTILE.find((*itREPLACE).second);

				if( finder != mapTILE.end() )
					bTILE = BYTE((*finder).second);
			}
		}

		mapINDEX.insert( MAPDWORD::value_type( i, bTILE));
	}

	m_pItem->m_vDETAILS.clear();
	m_pItem->m_vDETAIL.clear();

	nCount = INT(vDETAIL.size());
	for( i=0; i<nCount; i++)
	{
		MAPFLOAT::iterator finder = mapDETAILS.find(vDETAIL[i]);
		FLOAT fSCALE = finder != mapDETAILS.end() ? (*finder).second : 32.0f;

		m_pItem->m_vDETAIL.push_back(vDETAIL[i]);
		m_pItem->m_vDETAILS.push_back(fSCALE);
	}

	LPBYTE pDETAIL = new BYTE[m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitLength];
	nCount = m_pItem->m_HugeMAP.m_nUnitCountX * m_pItem->m_HugeMAP.m_nUnitCountZ;

	for( i=0; i<nCount; i++)
	{
		m_pItem->LoadUNIT(
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			pDETAIL,
			NULL, i);

		m_pItem->ResetDETAIL(
			&mapINDEX,
			pDETAIL,
			m_pItem->m_HugeMAP.m_nUnitLength * m_pItem->m_HugeMAP.m_nUnitLength);

		m_pItem->UpdateUNIT(
			NULL,
			NULL,
			NULL,
			NULL,
			NULL,
			pDETAIL,
			NULL, i);

		m_pItem->BuildTILETEX(
			i % m_pItem->m_HugeMAP.m_nUnitCountX,
			i / m_pItem->m_HugeMAP.m_nUnitCountX);

		lParam->m_pWnd->m_bPercent = (BYTE) (i * 100 / nCount);
		lParam->m_pWnd->m_cProgressBar.SetPos(lParam->m_pWnd->m_bPercent);
		lParam->m_pWnd->SendMessage(
			WM_COMMAND,
			ID_PROGRESS_UPDATE);
	}

	delete[] pDETAIL;
	pDETAIL = NULL;

	mapDETAILS.clear();
	vDETAIL.clear();

	mapREPLACE.clear();
	mapINDEX.clear();
	mapTILE.clear();

	lParam->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);
}
