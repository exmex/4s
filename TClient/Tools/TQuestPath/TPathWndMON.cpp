// TPathWndMON.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TPathWndMON.h"


// CTPathWndMON 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTPathWndMON, CTPathWndBase)

CTPathWndMON::CTPathWndMON( CWnd *pParent /*=NULL*/)
: CTPathWndBase(CTPathWndMON::IDD, pParent)
, m_strFilterNAME(_T(""))
, m_strPosX(_T(""))
, m_strPosY(_T(""))
, m_strPosZ(_T(""))
, m_dwFilterMapID(0)
, m_dwFilterMonID(0)
, m_dwFilterKIND(0)
, m_dwMapID(0)
, m_dwKIND(0)
{
}

CTPathWndMON::~CTPathWndMON()
{
}

void CTPathWndMON::DoDataExchange( CDataExchange *pDX)
{
	CTPathWndBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILTER_MON_NAME, m_strFilterNAME);
	DDX_Text(pDX, IDC_EDIT_MON_POS_X, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_MON_POS_Y, m_strPosY);
	DDX_Text(pDX, IDC_EDIT_MON_POS_Z, m_strPosZ);
	DDX_Text(pDX, IDC_FILTER_MON_KIND, m_dwFilterKIND);
	DDX_Text(pDX, IDC_FILTER_MAP_ID, m_dwFilterMapID);
	DDX_Text(pDX, IDC_FILTER_MON_ID, m_dwFilterMonID);
	DDX_Text(pDX, IDC_EDIT_MON_MAP_ID, m_dwMapID);
	DDX_Text(pDX, IDC_EDIT_MON_KIND, m_dwKIND);
	DDX_Control(pDX, IDC_CHECK_MON_KIND, m_cKIND);
	DDX_Control(pDX, IDC_CHECK_MON_NAME, m_cNAME);
	DDX_Control(pDX, IDC_CHECK_MON_ID, m_cMonID);
	DDX_Control(pDX, IDC_CHECK_MAP_ID, m_cMapID);
	DDX_Control(pDX, IDC_LIST_MON, m_cLIST);
}

BOOL CTPathWndMON::Create( CWnd *pParentWnd)
{
	BOOL bResult = CTPathWndBase::Create( CTPathWndMON::IDD, pParentWnd);

	if(bResult)
	{
		CWnd *pCtrl = GetDlgItem(IDC_LIST_MON);

		if(pCtrl)
		{
			CListCtrl *pList = (CListCtrl *) pCtrl;
			CRect rect;

			pList->GetClientRect(&rect);
			rect.DeflateRect( 9, 0);
			DWORD dwWidth = rect.Width() / 5;

			pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
			pList->InsertColumn( 0, _T("ID"), LVCFMT_LEFT, dwWidth);
			pList->InsertColumn( 1, _T("Name"), LVCFMT_LEFT, rect.Width() - dwWidth);
		}

		pCtrl = GetDlgItem(IDC_CHECK_MAP_ID);
		if(pCtrl)
			((CButton *) pCtrl)->SetCheck(FALSE);

		pCtrl = GetDlgItem(IDC_CHECK_MON_ID);
		if(pCtrl)
			((CButton *) pCtrl)->SetCheck(FALSE);

		pCtrl = GetDlgItem(IDC_CHECK_MON_KIND);
		if(pCtrl)
			((CButton *) pCtrl)->SetCheck(FALSE);

		pCtrl = GetDlgItem(IDC_CHECK_MON_NAME);
		if(pCtrl)
			((CButton *) pCtrl)->SetCheck(FALSE);
	}

	return bResult;
}

void CTPathWndMON::UpdateDATA( int nType, LPARAM lParam)
{
	m_cMapID.SetCheck(FALSE);
	m_cMonID.SetCheck(FALSE);
	m_cKIND.SetCheck(FALSE);
	m_cNAME.SetCheck(FALSE);
	UpdateTMDATA(0);

	m_dwKIND = nType == TUPDATE_MON ? DWORD(lParam) : 0;
	ResetTLIST();
}

void CTPathWndMON::UpdateTMAP()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
		if(m_dwKIND)
		{
			CString strPOS;
			TTITEM vTPOS;

			strPOS.Format(
				_T("%.2f %.2f %.2f"),
				atof(m_strPosX),
				atof(m_strPosY),
				atof(m_strPosZ));

			vTPOS.m_dwMonKind = m_dwKIND;
			vTPOS.m_dwMapID = m_dwMapID;

			sscanf( LPCSTR(strPOS),
				_T("%f %f %f"),
				&vTPOS.m_fPosX,
				&vTPOS.m_fPosY,
				&vTPOS.m_fPosZ);

			pDOC->UpdateTMAP(&vTPOS);
		}
		else
			pDOC->UpdateTMAP(NULL);
}


BEGIN_MESSAGE_MAP(CTPathWndMON, CTPathWndBase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_MON, OnLvnItemchangedListMon)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MON, OnNMRclickListMon)
	ON_NOTIFY(NM_CLICK, IDC_LIST_MON, OnNMClickListMon)
	ON_EN_CHANGE(IDC_FILTER_MON_NAME, OnEnChangeFilterMonName)
	ON_EN_CHANGE(IDC_FILTER_MON_KIND, OnEnChangeFilterMonKind)
	ON_EN_CHANGE(IDC_FILTER_MON_ID, OnEnChangeFilterMonId)
	ON_EN_CHANGE(IDC_FILTER_MAP_ID, OnEnChangeFilterMapId)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_CHECK_MON_NAME, OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_MON_KIND, OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_MON_ID, OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_MAP_ID, OnBnClickedCheck)
END_MESSAGE_MAP()


// CTPathWndMON 메시지 처리기입니다.

void CTPathWndMON::ResetTLIST()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		BYTE bMapID = m_cMapID.GetCheck() ? TRUE : FALSE;
		BYTE bMonID = m_cMonID.GetCheck() ? TRUE : FALSE;
		BYTE bKIND = m_cKIND.GetCheck() ? TRUE : FALSE;
		BYTE bNAME = m_cNAME.GetCheck() ? TRUE : FALSE;

		m_cLIST.DeleteAllItems();
		MAPDWORD mapID;
		mapID.clear();

		for( int i=0; i<INT(pDOC->m_vTGAMEDB.m_vTMDATA.size()); i++)
		{
			LPTMDATA pTMDATA = pDOC->m_vTGAMEDB.m_vTMDATA[i];

			if( pTMDATA && mapID.find(pTMDATA->m_wMonID) == mapID.end() )
			{
				CString strNAME = pDOC->GetTFMTName(
					pTMDATA->m_strTITLE,
					pTMDATA->m_strNAME);
				BYTE bADD = TRUE;

				if( bMonID && m_dwFilterMonID && m_dwFilterMonID != pTMDATA->m_wMonID )
					bADD = FALSE;

				if( bADD && bKIND && m_dwFilterKIND && m_dwFilterKIND != pTMDATA->m_wKind )
					bADD = FALSE;

				if( bADD && bMapID && m_dwFilterMapID != pTMDATA->m_wMapID )
					bADD = FALSE;

				if( bADD && bNAME && !m_strFilterNAME.IsEmpty() &&
					strNAME.Find(LPCSTR(m_strFilterNAME)) == -1 )
					bADD = FALSE;

				if(bADD)
				{
					CString strTEXT;

					strTEXT.Format(
						_T("%d"),
						pTMDATA->m_wMonID);

					int nIndex = m_cLIST.InsertItem(
						m_cLIST.GetItemCount(),
						strTEXT);

					m_cLIST.SetItemText(
						nIndex, 1,
						strNAME);

					m_cLIST.SetItemData(
						nIndex,
						DWORD(pTMDATA->m_wKind));

					mapID.insert( MAPDWORD::value_type(
						pTMDATA->m_wMonID,
						pTMDATA->m_wMonID));
				}
			}
		}

		BYTE bFIND = FALSE;
		mapID.clear();

		for( int i=0; i<m_cLIST.GetItemCount(); i++)
		{
			DWORD dwKIND = (DWORD) m_cLIST.GetItemData(i);

			if( !bFIND && m_dwKIND && m_dwKIND == dwKIND )
			{
				m_cLIST.SetItem(
					i, 0,
					LVIF_STATE,
					NULL, 0,
					LVIS_SELECTED|
					LVIS_FOCUSED,
					LVIS_SELECTED|
					LVIS_FOCUSED,
					NULL);

				bFIND = TRUE;
			}
			else
			{
				m_cLIST.SetItem(
					i, 0,
					LVIF_STATE,
					NULL, 0, 0,
					LVIS_SELECTED|
					LVIS_FOCUSED,
					NULL);
			}

			m_cLIST.Update(i);
		}

		POSITION pos = m_cLIST.GetFirstSelectedItemPosition();
		if(pos)
			m_cLIST.EnsureVisible( m_cLIST.GetNextSelectedItem(pos), FALSE);
		else
			UpdateTMDATA(0);
	}
}

void CTPathWndMON::OnBnClickedCheck()
{
	UpdateData();
	ResetTLIST();
}

void CTPathWndMON::OnEnChangeFilterMapId()
{
	if(m_cMapID.GetCheck())
	{
		m_dwFilterMapID = GetDlgItemInt(IDC_FILTER_MAP_ID);
		ResetTLIST();
	}
}

void CTPathWndMON::OnEnChangeFilterMonId()
{
	if(m_cMonID.GetCheck())
	{
		m_dwFilterMonID = GetDlgItemInt(IDC_FILTER_MON_ID);
		ResetTLIST();
	}
}

void CTPathWndMON::OnEnChangeFilterMonKind()
{
	if(m_cKIND.GetCheck())
	{
		m_dwFilterKIND = GetDlgItemInt(IDC_FILTER_MON_KIND);
		ResetTLIST();
	}
}

void CTPathWndMON::OnEnChangeFilterMonName()
{
	if(m_cNAME.GetCheck())
	{
		GetDlgItemText( IDC_FILTER_MON_NAME, m_strFilterNAME);
		ResetTLIST();
	}
}

void CTPathWndMON::OnLvnItemchangedListMon( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) && (pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		DWORD dwKIND = 0;

		if(pNMLV->uNewState & LVIS_SELECTED)
		{
			dwKIND = (DWORD) pNMLV->lParam;
			if(!dwKIND)
				UpdateTMAP();
		}

		UpdateTMDATA(dwKIND);
	}

	*pResult = 0;
}

void CTPathWndMON::OnNMClickListMon( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cLIST.GetFirstSelectedItemPosition())
		UpdateTMAP();

	*pResult = 0;
}

void CTPathWndMON::OnNMRclickListMon( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cLIST.GetFirstSelectedItemPosition())
		UpdateTMAP();

	*pResult = 0;
}

void CTPathWndMON::UpdateTMDATA( DWORD dwKIND)
{
	m_strPosX.Format( IDS_FMT_POS_DATA, 0.0f);
	m_strPosY.Format( IDS_FMT_POS_DATA, 0.0f);
	m_strPosZ.Format( IDS_FMT_POS_DATA, 0.0f);

	m_dwMapID = 0;
	m_dwKIND = 0;

	if(dwKIND)
	{
		CTQuestPathDoc *pDOC = GetDocument();

		if(pDOC)
		{
			TMDATA vTMDATA;

			if(pDOC->CalcTMONKIND( &vTMDATA, dwKIND))
			{
				m_strPosX.Format( IDS_FMT_POS_DATA, vTMDATA.m_fPosX);
				m_strPosY.Format( IDS_FMT_POS_DATA, vTMDATA.m_fPosY);
				m_strPosZ.Format( IDS_FMT_POS_DATA, vTMDATA.m_fPosZ);

				m_dwMapID = vTMDATA.m_wMapID;
				m_dwKIND = dwKIND;

				UpdateTMAP();
			}
		}
	}

	UpdateData(FALSE);
}

void CTPathWndMON::OnBnClickedButtonApply()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		pDOC->UpdateDATA( TPATHWND_MAIN, TUPDATE_MON, m_dwKIND);
		pDOC->EnableTPATHWND(TPATHWND_MAIN);
	}
}

void CTPathWndMON::OnBnClickedButtonCancel()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
		pDOC->EnableTPATHWND(TPATHWND_MAIN);
}
