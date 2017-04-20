// TPathWndMAP.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TPathWndMAP.h"


// CTPathWndMAP 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTPathWndMAP, CTPathWndBase)

CTPathWndMAP::CTPathWndMAP( CWnd *pParent /*=NULL*/)
: CTPathWndBase(CTPathWndMAP::IDD, pParent)
, m_nEditMode(0)
, m_strPosX(_T(""))
, m_strPosZ(_T(""))
{
	m_pTUNIT = NULL;
	m_pTMAP = NULL;
	m_wPointID = 0;
}

CTPathWndMAP::~CTPathWndMAP()
{
}

void CTPathWndMAP::DoDataExchange( CDataExchange *pDX)
{
	CTPathWndBase::DoDataExchange(pDX);
	DDX_Radio(pDX, IDC_RADIO_TEST_PATH, m_nEditMode);
	DDX_Control(pDX, IDC_BUTTON_CLEAR, m_cClearPath);
	DDX_Control(pDX, IDC_LIST_UNIT, m_cTUNIT);
	DDX_Control(pDX, IDC_LIST_MAP, m_cTMAP);
	DDX_Control(pDX, IDC_EDIT_NPC_POS_X, m_cPosX);
	DDX_Control(pDX, IDC_EDIT_NPC_POS_Z, m_cPosZ);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_X, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_Z, m_strPosZ);
}

BOOL CTPathWndMAP::Create( CWnd *pParentWnd)
{
	BOOL bResult = CTPathWndBase::Create( CTPathWndMAP::IDD, pParentWnd);

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

void CTPathWndMAP::UpdateDATA( int nType, LPARAM lParam)
{
	switch(nType)
	{
	case TUPDATE_RESET	:
		{
			LPTTITEM pTTITEM = (LPTTITEM) lParam;

			if(pTTITEM)
			{
				CTQuestPathDoc *pDOC = GetDocument();

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

			OnBnClickedRadioMode();
			ResetTLIST();
		}

		break;

	case TUPDATE_POS	:
		{
			m_wPointID = 0;

			if( m_pTUNIT && m_pTMAP )
			{
				MAPT2DPOINT::iterator finder = m_pTUNIT->m_mapTPOINT.find((WORD) lParam);

				if( finder != m_pTUNIT->m_mapTPOINT.end() )
				{
					m_wPointID = (*finder).first;

					m_strPosX.Format( IDS_FMT_POS_DATA, FLOAT(m_pTMAP->m_dwUnitX * m_pTUNIT->m_vID.m_wUnitX) + GET2D_X((*finder).second));
					m_strPosZ.Format( IDS_FMT_POS_DATA, FLOAT(m_pTMAP->m_dwUnitZ * m_pTUNIT->m_vID.m_wUnitZ) + GET2D_Y((*finder).second));
					UpdateData(FALSE);
				}
			}

			EnableCtrl();
		}

		break;
	}
}

void CTPathWndMAP::UpdateTMAP()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
		if( m_pTMAP && m_pTUNIT )
		{
			TTITEM vTPOS;

			vTPOS.m_fPosX = FLOAT(m_pTMAP->m_dwUnitX) * (FLOAT(m_pTUNIT->m_vID.m_wUnitX) + 0.5f);
			vTPOS.m_fPosZ = FLOAT(m_pTMAP->m_dwUnitZ) * (FLOAT(m_pTUNIT->m_vID.m_wUnitZ) + 0.5f);
			vTPOS.m_dwMapID = m_pTMAP->m_dwID;

			pDOC->UpdateTMAP(&vTPOS);
		}
		else
			pDOC->UpdateTMAP(NULL);
}


BEGIN_MESSAGE_MAP(CTPathWndMAP, CTPathWndBase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MAP, OnLvnItemchangedListMap)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MAP, OnNMRclickListMap)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MAP, OnNMClickListMap)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_UNIT, OnLvnItemchangedListUnit)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_UNIT, OnNMRclickListUnit)
	ON_NOTIFY(NM_CLICK, IDC_LIST_UNIT, OnNMClickListUnit)
	ON_BN_CLICKED(IDC_RADIO_MOVE_POINT, OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_TEST_PATH, OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_ADD_POINT, OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_DEL_POINT, OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_ADD_BLOCK, OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_ADD_PATH, OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_RADIO_DEL_LINE, OnBnClickedRadioMode)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, OnBnClickedButtonClear)
	ON_EN_CHANGE(IDC_EDIT_NPC_POS_X, OnEnChangeEditPOS)
	ON_EN_CHANGE(IDC_EDIT_NPC_POS_Z, OnEnChangeEditPOS)
END_MESSAGE_MAP()


// CTPathWndMAP 메시지 처리기입니다.

void CTPathWndMAP::ResetTLIST()
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

		for( int i=0; i<m_cTMAP.GetItemCount(); i++)
		{
			LPTMAPDATA pTMAP = (LPTMAPDATA) m_cTMAP.GetItemData(i);

			if( m_pTMAP && m_pTMAP == pTMAP )
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
		{
			m_cTMAP.EnsureVisible( m_cTMAP.GetNextSelectedItem(pos), FALSE);

			if(pTUNIT)
			{
				for(int i=0; i<m_cTUNIT.GetItemCount(); i++)
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

				pos = m_cTUNIT.GetFirstSelectedItemPosition();
				if(pos)
					m_cTUNIT.EnsureVisible( m_cTUNIT.GetNextSelectedItem(pos), FALSE);
				else
					ResetTUNIT(NULL);
			}
		}
		else
			ResetTMAP(NULL);
	}
}

void CTPathWndMAP::ResetTMAP( LPTMAPDATA pTMAP)
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

void CTPathWndMAP::ResetTUNIT( LPTMAPIMG pTUNIT)
{
	m_pTUNIT = pTUNIT;
	UpdateTMAP();

	m_cClearPath.EnableWindow(m_pTUNIT ? TRUE : FALSE);
}

void CTPathWndMAP::EnableCtrl()
{
	BYTE bEnable = m_wPointID && m_nEditMode == TEDITPATH_MOVE_POINT ? TRUE : FALSE;

	m_cPosX.EnableWindow(bEnable);
	m_cPosZ.EnableWindow(bEnable);
}

void CTPathWndMAP::OnLvnItemchangedListMap( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		ResetTMAP((LPTMAPDATA) pNMLV->lParam);

	*pResult = 0;
}

void CTPathWndMAP::OnNMRclickListMap( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTMAP.GetFirstSelectedItemPosition())
		ResetTMAP(NULL);

	*pResult = 0;
}

void CTPathWndMAP::OnNMClickListMap( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTMAP.GetFirstSelectedItemPosition())
		ResetTMAP(NULL);

	*pResult = 0;
}

void CTPathWndMAP::OnLvnItemchangedListUnit( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		ResetTUNIT((LPTMAPIMG) pNMLV->lParam);

	*pResult = 0;
}

void CTPathWndMAP::OnNMRclickListUnit( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTUNIT.GetFirstSelectedItemPosition())
		ResetTUNIT(NULL);

	*pResult = 0;
}

void CTPathWndMAP::OnNMClickListUnit( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTUNIT.GetFirstSelectedItemPosition())
		ResetTUNIT(NULL);

	*pResult = 0;
}

void CTPathWndMAP::OnBnClickedRadioMode()
{
	CTQuestPathDoc *pDOC = GetDocument();

	UpdateData();
	EnableCtrl();

	if(pDOC)
	{
		pDOC->m_nPathMode = m_nEditMode;
		pDOC->RedrawTMAP();
	}
}

void CTPathWndMAP::OnEnChangeEditPOS()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if( pDOC && m_pTMAP && m_pTUNIT && m_wPointID && m_nEditMode == TEDITPATH_MOVE_POINT )
	{
		CString strTEXT;
		FLOAT fPosX;
		FLOAT fPosZ;

		GetDlgItemText( IDC_EDIT_NPC_POS_X, strTEXT);
		fPosX = (FLOAT) atof(LPCSTR(strTEXT));

		GetDlgItemText( IDC_EDIT_NPC_POS_Z, strTEXT);
		fPosZ = (FLOAT) atof(LPCSTR(strTEXT));

		fPosX -= FLOAT(m_pTMAP->m_dwUnitX * m_pTUNIT->m_vID.m_wUnitX);
		fPosZ -= FLOAT(m_pTMAP->m_dwUnitZ * m_pTUNIT->m_vID.m_wUnitZ);

		if( 0.0f <= fPosX && fPosX < m_pTMAP->m_dwUnitX &&
			0.0f <= fPosZ && fPosZ < m_pTMAP->m_dwUnitZ )
		{
			pDOC->MovePathPoint( m_pTUNIT, m_wPointID, fPosX, fPosZ);
			pDOC->RedrawTMAP();
		}
	}
}

void CTPathWndMAP::OnBnClickedButtonClear()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if( pDOC && m_pTUNIT && AfxMessageBox( IDS_MSG_PATH_CLEAR, MB_YESNO) == IDYES )
	{
		pDOC->ClearPathData(m_pTUNIT);
		pDOC->RedrawTMAP();
	}
}
