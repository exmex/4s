// TPathWndPOS.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TPathWndPOS.h"


// CTPathWndPOS 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTPathWndPOS, CTPathWndBase)

CTPathWndPOS::CTPathWndPOS( CWnd *pParent /*=NULL*/)
: CTPathWndBase(CTPathWndPOS::IDD, pParent)
, m_strPosX(_T(""))
, m_strPosZ(_T(""))
{
	m_bUpdateMAP = TRUE;

	m_pTUNIT = NULL;
	m_pTMAP = NULL;
}

CTPathWndPOS::~CTPathWndPOS()
{
}

void CTPathWndPOS::DoDataExchange( CDataExchange *pDX)
{
	CTPathWndBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_UNIT, m_cTUNIT);
	DDX_Control(pDX, IDC_LIST_MAP, m_cTMAP);
	DDX_Control(pDX, IDC_EDIT_NPC_POS_X, m_cPosX);
	DDX_Control(pDX, IDC_EDIT_NPC_POS_Z, m_cPosZ);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_cApply);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_X, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_Z, m_strPosZ);
}

BOOL CTPathWndPOS::Create( CWnd *pParentWnd)
{
	BOOL bResult = CTPathWndBase::Create( CTPathWndPOS::IDD, pParentWnd);

	if(bResult)
	{
		CWnd *pCtrl = GetDlgItem(IDC_LIST_MAP);

		if(pCtrl)
		{
			CListCtrl *pList = (CListCtrl *) pCtrl;
			CRect rect;

			pList->GetClientRect(&rect);
			rect.DeflateRect( 9, 0);

			pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
			pList->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());
		}

		pCtrl = GetDlgItem(IDC_LIST_UNIT);
		if(pCtrl)
		{
			CListCtrl *pList = (CListCtrl *) pCtrl;
			CRect rect;

			pList->GetClientRect(&rect);
			rect.DeflateRect( 9, 0);
			DWORD dwWidth = rect.Width() / 2;

			pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
			pList->InsertColumn( 0, _T("Unit X"), LVCFMT_LEFT, dwWidth);
			pList->InsertColumn( 1, _T("Unit Z"), LVCFMT_LEFT, rect.Width() - dwWidth);
		}
	}

	return bResult;
}

void CTPathWndPOS::UpdateDATA( int nType, LPARAM lParam)
{
	switch(nType)
	{
	case TUPDATE_RESET	:
		{
			LPTTITEM pTTITEM = (LPTTITEM) lParam;

			if(pTTITEM)
			{
				CTQuestPathDoc *pDOC = GetDocument();

				m_strPosX.Format( IDS_FMT_POS_DATA, pTTITEM->m_fPosX);
				m_strPosZ.Format( IDS_FMT_POS_DATA, pTTITEM->m_fPosZ);

				m_pTUNIT = NULL;
				m_pTMAP = NULL;

				if(pDOC)
				{
					m_pTUNIT = pDOC->FindTMAPIMG(
						pTTITEM->m_dwMapID,
						pTTITEM->m_fPosX,
						pTTITEM->m_fPosZ);
					m_pTMAP = pDOC->FindTMAPDATA(pTTITEM->m_dwMapID);
				}
			}

			UpdateData(FALSE);
			ResetTLIST();
		}

		break;

	case TUPDATE_POS	:
		{
			CTQuestPathDoc *pDOC = GetDocument();
			LPTTITEM pTTITEM = (LPTTITEM) lParam;

			if( pDOC && pTTITEM )
			{
				LPTMAPDATA pTMAP = pDOC->FindTMAPDATA(pTTITEM->m_dwMapID);
				LPTMAPIMG pTUNIT = pDOC->FindTMAPIMG(
					pTTITEM->m_dwMapID,
					pTTITEM->m_fPosX,
					pTTITEM->m_fPosZ);

				m_strPosX.Format( IDS_FMT_POS_DATA, pTTITEM->m_fPosX);
				m_strPosZ.Format( IDS_FMT_POS_DATA, pTTITEM->m_fPosZ);
				UpdateData(FALSE);

				if( m_pTMAP != pTMAP )
				{
					m_bUpdateMAP = FALSE;
					SelectTMAP(pTMAP);
					m_bUpdateMAP = TRUE;
				}

				if(!m_cTMAP.GetFirstSelectedItemPosition())
					ResetTMAP(NULL);
				else if( m_pTUNIT != pTUNIT )
				{
					SelectTUNIT(pTUNIT);

					if(!m_cTUNIT.GetFirstSelectedItemPosition())
						ResetTUNIT(NULL);
				}
			}
		}

		break;
	}
}

void CTPathWndPOS::UpdateTMAP()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if( m_bUpdateMAP && pDOC )
		if( !m_pTMAP || !m_pTUNIT )
			pDOC->UpdateTMAP(NULL);
		else if(CheckValid())
		{
			CString strPOS;
			TTITEM vTPOS;

			GetDlgItemText( IDC_EDIT_NPC_POS_X, m_strPosX);
			GetDlgItemText( IDC_EDIT_NPC_POS_Z, m_strPosZ);

			strPOS.Format(
				_T("%.2f %.2f"),
				atof(m_strPosX),
				atof(m_strPosZ));

			sscanf( LPCSTR(strPOS),
				_T("%f %f"),
				&vTPOS.m_fPosX,
				&vTPOS.m_fPosZ);
			vTPOS.m_dwMapID = m_pTMAP->m_dwID;

			pDOC->UpdateTMAP(&vTPOS);
		}
}

BYTE CTPathWndPOS::CheckValid()
{
	if( !m_pTMAP || !m_pTUNIT )
		return FALSE;

	int nPosX = GetDlgItemInt(IDC_EDIT_NPC_POS_X) - m_pTMAP->m_dwUnitX * m_pTUNIT->m_vID.m_wUnitX;
	int nPosZ = GetDlgItemInt(IDC_EDIT_NPC_POS_Z) - m_pTMAP->m_dwUnitZ * m_pTUNIT->m_vID.m_wUnitZ;

	if( 0 <= nPosX && nPosX < INT(m_pTMAP->m_dwUnitX) &&
		0 <= nPosZ && nPosZ < INT(m_pTMAP->m_dwUnitZ) )
		return TRUE;

	return FALSE;
}

void CTPathWndPOS::SelectTMAP( LPTMAPDATA pTMAP)
{
	for( int i=0; i<m_cTMAP.GetItemCount(); i++)
	{
		LPTMAPDATA pTDATA = (LPTMAPDATA) m_cTMAP.GetItemData(i);

		if( pTMAP && pTMAP == pTDATA )
		{
			m_cTMAP.SetItem(
				i, 0,
				LVIF_STATE,
				NULL, 0,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				NULL);
		}
		else
		{
			m_cTMAP.SetItem(
				i, 0,
				LVIF_STATE,
				NULL, 0, 0,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				NULL);
		}

		m_cTMAP.Update(i);
	}

	POSITION pos = m_cTMAP.GetFirstSelectedItemPosition();
	if(pos)
		m_cTMAP.EnsureVisible( m_cTMAP.GetNextSelectedItem(pos), FALSE);
}

void CTPathWndPOS::SelectTUNIT( LPTMAPIMG pTUNIT)
{
	for( int i=0; i<m_cTUNIT.GetItemCount(); i++)
	{
		LPTMAPIMG pTIMG = (LPTMAPIMG) m_cTUNIT.GetItemData(i);

		if( pTUNIT && pTUNIT == pTIMG )
		{
			m_cTUNIT.SetItem(
				i, 0,
				LVIF_STATE,
				NULL, 0,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				NULL);
		}
		else
		{
			m_cTUNIT.SetItem(
				i, 0,
				LVIF_STATE,
				NULL, 0, 0,
				LVIS_SELECTED|
				LVIS_FOCUSED,
				NULL);
		}

		m_cTUNIT.Update(i);
	}

	POSITION pos = m_cTUNIT.GetFirstSelectedItemPosition();
	if(pos)
		m_cTUNIT.EnsureVisible( m_cTUNIT.GetNextSelectedItem(pos), FALSE);
}


BEGIN_MESSAGE_MAP(CTPathWndPOS, CTPathWndBase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MAP, OnLvnItemchangedListMap)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MAP, OnNMRclickListMap)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MAP, OnNMClickListMap)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_UNIT, OnLvnItemchangedListUnit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_UNIT, OnNMRclickListUnit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_UNIT, OnNMClickListUnit)
	ON_EN_CHANGE(IDC_EDIT_NPC_POS_X, OnEnChangeEditPOS)
	ON_EN_CHANGE(IDC_EDIT_NPC_POS_Z, OnEnChangeEditPOS)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
END_MESSAGE_MAP()


// CTPathWndPOS 메시지 처리기입니다.

void CTPathWndPOS::ResetTLIST()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		MAPTMAPDATA::iterator itTMAP;
		LPTMAPIMG pTUNIT = m_pTUNIT;

		m_cTUNIT.DeleteAllItems();
		m_cTMAP.DeleteAllItems();

		for( itTMAP = pDOC->m_mapTMAPDB.begin(); itTMAP != pDOC->m_mapTMAPDB.end(); itTMAP++)
		{
			LPTMAPDATA pTMAP = (*itTMAP).second;

			if(pTMAP)
			{
				CString strMapID;

				strMapID.Format( _T("%d"), pTMAP->m_dwID);
				int nIndex = m_cTMAP.InsertItem(
					m_cTMAP.GetItemCount(),
					strMapID);
				m_cTMAP.SetItemData( nIndex, (DWORD_PTR) pTMAP);
			}
		}

		m_bUpdateMAP = FALSE;
		SelectTMAP(m_pTMAP);
		m_bUpdateMAP = TRUE;

		if(!m_cTMAP.GetFirstSelectedItemPosition())
			ResetTMAP(NULL);
		else if(pTUNIT)
		{
			SelectTUNIT(pTUNIT);

			if(!m_cTUNIT.GetFirstSelectedItemPosition())
				ResetTUNIT(NULL);
		}
	}
}

void CTPathWndPOS::ResetTMAP( LPTMAPDATA pTMAP)
{
	MAPTMAPIMG::iterator itTUNIT;

	m_cTUNIT.DeleteAllItems();
	ResetTUNIT(NULL);
	m_pTMAP = pTMAP;

	if(!pTMAP)
		return;

	for( itTUNIT = pTMAP->m_mapTUNIT.begin(); itTUNIT != pTMAP->m_mapTUNIT.end(); itTUNIT++)
	{
		LPTMAPIMG pTUNIT = (*itTUNIT).second;

		if(pTUNIT)
		{
			CString strTEXT;

			strTEXT.Format( _T("%d"), pTUNIT->m_vID.m_wUnitX);
			int nIndex = m_cTUNIT.InsertItem(
				m_cTUNIT.GetItemCount(),
				strTEXT);

			strTEXT.Format( _T("%d"), pTUNIT->m_vID.m_wUnitZ);
			m_cTUNIT.SetItemText( nIndex, 1, strTEXT);
			m_cTUNIT.SetItemData( nIndex, (DWORD_PTR) pTUNIT);
		}
	}
}

void CTPathWndPOS::ResetTUNIT( LPTMAPIMG pTUNIT)
{
	m_pTUNIT = pTUNIT;

	if( m_pTMAP && m_pTUNIT )
	{
		CString strPOS;
		FLOAT fPosX;
		FLOAT fPosZ;

		GetDlgItemText( IDC_EDIT_NPC_POS_X, m_strPosX);
		GetDlgItemText( IDC_EDIT_NPC_POS_Z, m_strPosZ);

		strPOS.Format(
			_T("%.2f %.2f"),
			atof(m_strPosX),
			atof(m_strPosZ));

		sscanf( LPCSTR(strPOS),
			_T("%f %f"),
			&fPosX,
			&fPosZ);

		while( fPosX > m_pTMAP->m_dwUnitX )
			fPosX -= m_pTMAP->m_dwUnitX;

		while( fPosZ > m_pTMAP->m_dwUnitZ )
			fPosZ -= m_pTMAP->m_dwUnitZ;

		fPosX += FLOAT(m_pTMAP->m_dwUnitX * m_pTUNIT->m_vID.m_wUnitX);
		fPosZ += FLOAT(m_pTMAP->m_dwUnitZ * m_pTUNIT->m_vID.m_wUnitZ);

		m_strPosX.Format( IDS_FMT_POS_DATA, fPosX);
		m_strPosZ.Format( IDS_FMT_POS_DATA, fPosZ);
		UpdateData(FALSE);
	}

	EnableCtrl();
	UpdateTMAP();
}

void CTPathWndPOS::EnableCtrl()
{
	BYTE bEnable = m_pTMAP && m_pTUNIT ? TRUE : FALSE;
	BYTE bApply = CheckValid();

	m_cApply.EnableWindow(bApply);
	m_cPosX.EnableWindow(bEnable);
	m_cPosZ.EnableWindow(bEnable);
}

void CTPathWndPOS::OnLvnItemchangedListMap( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		ResetTMAP((LPTMAPDATA) pNMLV->lParam);

	*pResult = 0;
}

void CTPathWndPOS::OnNMRclickListMap( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTMAP.GetFirstSelectedItemPosition())
		ResetTMAP(NULL);

	*pResult = 0;
}

void CTPathWndPOS::OnNMClickListMap( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTMAP.GetFirstSelectedItemPosition())
		ResetTMAP(NULL);

	*pResult = 0;
}

void CTPathWndPOS::OnLvnItemchangedListUnit( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		ResetTUNIT((LPTMAPIMG) pNMLV->lParam);

	*pResult = 0;
}

void CTPathWndPOS::OnNMRclickListUnit( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTUNIT.GetFirstSelectedItemPosition())
		ResetTUNIT(NULL);

	*pResult = 0;
}

void CTPathWndPOS::OnNMClickListUnit( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTUNIT.GetFirstSelectedItemPosition())
		ResetTUNIT(NULL);

	*pResult = 0;
}

void CTPathWndPOS::OnEnChangeEditPOS()
{
	if( m_pTMAP && m_pTUNIT )
		UpdateTMAP();

	EnableCtrl();
}

void CTPathWndPOS::OnBnClickedButtonCancel()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		pDOC->m_nPathMode = TEDITPATH_TEST_PATH;
		pDOC->EnableTPATHWND(TPATHWND_MAIN);
	}
}

void CTPathWndPOS::OnBnClickedButtonApply()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if( pDOC && CheckValid() )
	{
		CString strPOS;
		TTITEM vTPOS;

		GetDlgItemText( IDC_EDIT_NPC_POS_X, m_strPosX);
		GetDlgItemText( IDC_EDIT_NPC_POS_Z, m_strPosZ);

		strPOS.Format(
			_T("%.2f %.2f"),
			atof(m_strPosX),
			atof(m_strPosZ));

		sscanf( LPCSTR(strPOS),
			_T("%f %f"),
			&vTPOS.m_fPosX,
			&vTPOS.m_fPosZ);

		pDOC->m_nPathMode = TEDITPATH_TEST_PATH;
		vTPOS.m_dwMapID = m_pTMAP->m_dwID;

		pDOC->UpdateDATA( TPATHWND_MAIN, TUPDATE_POS, (LPARAM) &vTPOS);
		pDOC->EnableTPATHWND(TPATHWND_MAIN);
	}
}
