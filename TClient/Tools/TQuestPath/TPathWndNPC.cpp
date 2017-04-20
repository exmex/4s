// TPathWndNPC.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TPathWndNPC.h"


// CTPathWndNPC 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTPathWndNPC, CTPathWndBase)

CTPathWndNPC::CTPathWndNPC( CWnd *pParent /*=NULL*/)
: CTPathWndBase(CTPathWndNPC::IDD, pParent)
, m_strFilterNpcName(_T(""))
, m_strPosX(_T(""))
, m_strPosY(_T(""))
, m_strPosZ(_T(""))
, m_dwFilterMapID(0)
, m_dwFilterNpcID(0)
, m_dwNpcID(0)
, m_dwMapID(0)
{
}

CTPathWndNPC::~CTPathWndNPC()
{
}

void CTPathWndNPC::DoDataExchange( CDataExchange *pDX)
{
	CTPathWndBase::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_FILTER_NPC_NAME, m_strFilterNpcName);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_X, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_Y, m_strPosY);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_Z, m_strPosZ);
	DDX_Text(pDX, IDC_FILTER_MAP_ID, m_dwFilterMapID);
	DDX_Text(pDX, IDC_FILTER_NPC_ID, m_dwFilterNpcID);
	DDX_Text(pDX, IDC_EDIT_NPC_MAP_ID, m_dwMapID);
	DDX_Text(pDX, IDC_EDIT_NPC_ID, m_dwNpcID);
	DDX_Control(pDX, IDC_CHECK_NPC_NAME, m_cNpcName);
	DDX_Control(pDX, IDC_CHECK_MAP_ID, m_cMapID);
	DDX_Control(pDX, IDC_CHECK_NPC_ID, m_cNpcID);
	DDX_Control(pDX, IDC_LIST_NPC, m_cLIST);
}

BOOL CTPathWndNPC::Create( CWnd *pParentWnd)
{
	BOOL bResult = CTPathWndBase::Create( CTPathWndNPC::IDD, pParentWnd);

	if(bResult)
	{
		CWnd *pCtrl = GetDlgItem(IDC_LIST_NPC);

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

		pCtrl = GetDlgItem(IDC_CHECK_NPC_ID);
		if(pCtrl)
			((CButton *) pCtrl)->SetCheck(FALSE);

		pCtrl = GetDlgItem(IDC_CHECK_NPC_NAME);
		if(pCtrl)
			((CButton *) pCtrl)->SetCheck(FALSE);
	}

	return bResult;
}

void CTPathWndNPC::UpdateDATA( int nType, LPARAM lParam)
{
	m_cNpcName.SetCheck(FALSE);
	m_cNpcID.SetCheck(FALSE);
	m_cMapID.SetCheck(FALSE);
	UpdateTNDATA(NULL);

	m_dwNpcID = nType == TUPDATE_NPC ? DWORD(lParam) : 0;
	ResetTLIST();
}

void CTPathWndNPC::UpdateTMAP()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
		if(m_dwNpcID)
		{
			CString strPOS;
			TTITEM vTPOS;

			strPOS.Format(
				_T("%.2f %.2f %.2f"),
				atof(m_strPosX),
				atof(m_strPosY),
				atof(m_strPosZ));

			vTPOS.m_dwMapID = m_dwMapID;
			vTPOS.m_dwNpcID = m_dwNpcID;

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


BEGIN_MESSAGE_MAP(CTPathWndNPC, CTPathWndBase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_NPC, OnLvnItemchangedListNpc)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_NPC, OnNMRclickListNpc)
	ON_NOTIFY(NM_CLICK, IDC_LIST_NPC, OnNMClickListNpc)
	ON_EN_CHANGE(IDC_FILTER_NPC_NAME, OnEnChangeFilterNpcName)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_EN_CHANGE(IDC_FILTER_MAP_ID, OnEnChangeFilterMapId)
	ON_EN_CHANGE(IDC_FILTER_NPC_ID, OnEnChangeFilterNpcId)
	ON_BN_CLICKED(IDC_CHECK_NPC_NAME, OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_MAP_ID, OnBnClickedCheck)
	ON_BN_CLICKED(IDC_CHECK_NPC_ID, OnBnClickedCheck)
END_MESSAGE_MAP()


// CTPathWndNPC 메시지 처리기입니다.

void CTPathWndNPC::ResetTLIST()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		MAPTNDATA::iterator itTNDATA;
		m_cLIST.DeleteAllItems();

		BYTE bNpcName = m_cNpcName.GetCheck() ? TRUE : FALSE;
		BYTE bNpcID = m_cNpcID.GetCheck() ? TRUE : FALSE;
		BYTE bMapID = m_cMapID.GetCheck() ? TRUE : FALSE;

		for( itTNDATA = pDOC->m_vTGAMEDB.m_mapTNDATA.begin(); itTNDATA != pDOC->m_vTGAMEDB.m_mapTNDATA.end(); itTNDATA++)
		{
			LPTNDATA pTNDATA = (*itTNDATA).second;

			if(pTNDATA)
			{
				CString strNAME = pDOC->GetTNPCName(pTNDATA->m_wID);
				BYTE bADD = TRUE;

				if( bNpcID && m_dwFilterNpcID && m_dwFilterNpcID != pTNDATA->m_wID )
					bADD = FALSE;

				if( bADD && bMapID && m_dwFilterMapID != pTNDATA->m_wMapID )
					bADD = FALSE;

				if( bADD && bNpcName && !m_strFilterNpcName.IsEmpty() &&
					strNAME.Find(LPCSTR(m_strFilterNpcName)) == -1 )
					bADD = FALSE;

				if(bADD)
				{
					CString strTEXT;

					strTEXT.Format(
						_T("%d"),
						pTNDATA->m_wID);

					int nIndex = m_cLIST.InsertItem(
						m_cLIST.GetItemCount(),
						strTEXT);

					m_cLIST.SetItemText(
						nIndex, 1,
						strNAME);

					m_cLIST.SetItemData(
						nIndex,
						(DWORD_PTR) pTNDATA);
				}
			}
		}

		for( int i=0; i<m_cLIST.GetItemCount(); i++)
		{
			LPTNDATA pTNDATA = (LPTNDATA) m_cLIST.GetItemData(i);

			if( pTNDATA && m_dwNpcID == pTNDATA->m_wID )
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
			UpdateTNDATA(NULL);
	}
}

void CTPathWndNPC::OnEnChangeFilterNpcName()
{
	if(m_cNpcName.GetCheck())
	{
		GetDlgItemText( IDC_FILTER_NPC_NAME, m_strFilterNpcName);
		ResetTLIST();
	}
}

void CTPathWndNPC::OnEnChangeFilterMapId()
{
	if(m_cMapID.GetCheck())
	{
		m_dwFilterMapID = GetDlgItemInt(IDC_FILTER_MAP_ID);
		ResetTLIST();
	}
}

void CTPathWndNPC::OnEnChangeFilterNpcId()
{
	if(m_cNpcID.GetCheck())
	{
		m_dwFilterNpcID = GetDlgItemInt(IDC_FILTER_NPC_ID);
		ResetTLIST();
	}
}

void CTPathWndNPC::OnBnClickedCheck()
{
	UpdateData();
	ResetTLIST();
}

void CTPathWndNPC::OnLvnItemchangedListNpc( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) && (pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		LPTNDATA pTNDATA = NULL;

		if(pNMLV->uNewState & LVIS_SELECTED)
			pTNDATA = (LPTNDATA) pNMLV->lParam;

		UpdateTNDATA(pTNDATA);
	}

	*pResult = 0;
}

void CTPathWndNPC::OnNMRclickListNpc( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cLIST.GetFirstSelectedItemPosition())
		UpdateTMAP();

	*pResult = 0;
}

void CTPathWndNPC::OnNMClickListNpc( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cLIST.GetFirstSelectedItemPosition())
		UpdateTMAP();

	*pResult = 0;
}

void CTPathWndNPC::UpdateTNDATA( LPTNDATA pTNDATA)
{
	m_strPosX.Format( IDS_FMT_POS_DATA, 0.0f);
	m_strPosY.Format( IDS_FMT_POS_DATA, 0.0f);
	m_strPosZ.Format( IDS_FMT_POS_DATA, 0.0f);

	m_dwMapID = 0;
	m_dwNpcID = 0;

	if(pTNDATA)
	{
		m_strPosX.Format( IDS_FMT_POS_DATA, pTNDATA->m_fPosX);
		m_strPosY.Format( IDS_FMT_POS_DATA, pTNDATA->m_fPosY);
		m_strPosZ.Format( IDS_FMT_POS_DATA, pTNDATA->m_fPosZ);

		m_dwMapID = pTNDATA->m_wMapID;
		m_dwNpcID = pTNDATA->m_wID;

		UpdateTMAP();
	}

	UpdateData(FALSE);
}

void CTPathWndNPC::OnBnClickedButtonApply()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		pDOC->UpdateDATA( TPATHWND_MAIN, TUPDATE_NPC, m_dwNpcID);
		pDOC->EnableTPATHWND(TPATHWND_MAIN);
	}
}

void CTPathWndNPC::OnBnClickedButtonCancel()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
		pDOC->EnableTPATHWND(TPATHWND_MAIN);
}
