// TPathWndMAIN.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TPathWndMAIN.h"


// CTPathWndMAIN 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTPathWndMAIN, CTPathWndBase)

CTPathWndMAIN::CTPathWndMAIN( CWnd *pParent /*=NULL*/)
: CTPathWndBase(CTPathWndMAIN::IDD, pParent)
, m_strNpcName(_T(""))
, m_strPosX(_T(""))
, m_strPosY(_T(""))
, m_strPosZ(_T(""))
, m_strMSG(_T(""))
, m_dwQuestID(0)
, m_dwNpcID(0)
, m_dwMapID(0)
, m_dwMonKind(0)
{
	m_pTTITEM = NULL;
}

CTPathWndMAIN::~CTPathWndMAIN()
{
}

void CTPathWndMAIN::DoDataExchange( CDataExchange *pDX)
{
	CTPathWndBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_SHOW_MON, m_cFindMON);
	DDX_Control(pDX, IDC_BUTTON_AUTO_SET, m_cAutoset);
	DDX_Control(pDX, IDC_BUTTON_RESTORE, m_cRestore);
	DDX_Control(pDX, IDC_BUTTON_APPLY, m_cApply);
	DDX_Control(pDX, IDC_BUTTON_SHOW_NPC, m_cFindNPC);
	DDX_Control(pDX, IDC_BUTTON_PICK_POS, m_cPickPOS);
	DDX_Control(pDX, IDC_STATIC_QUEST_ID, m_cQuestID);
	DDX_Control(pDX, IDC_EDIT_NPC_MAP_ID, m_cMapID);
	DDX_Control(pDX, IDC_EDIT_NPC_NAME, m_cNpcName);
	DDX_Control(pDX, IDC_EDIT_NPC_POS_X, m_cPosX);
	DDX_Control(pDX, IDC_EDIT_NPC_POS_Y, m_cPosY);
	DDX_Control(pDX, IDC_EDIT_NPC_POS_Z, m_cPosZ);
	DDX_Control(pDX, IDC_STATIC_MON_KIND, m_cMonKind);
	DDX_Control(pDX, IDC_STATIC_NPC_ID, m_cNpcID);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_cMSG);
	DDX_Control(pDX, IDC_LIST_MON, m_cMonName);
	DDX_Control(pDX, IDC_LIST_STEP, m_cTSTEP);
	DDX_Text(pDX, IDC_STATIC_QUEST_ID, m_dwQuestID);
	DDX_Text(pDX, IDC_EDIT_NPC_NAME, m_strNpcName);
	DDX_Text(pDX, IDC_EDIT_NPC_MAP_ID, m_dwMapID);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_X, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_Y, m_strPosY);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_Z, m_strPosZ);
	DDX_Text(pDX, IDC_STATIC_MON_KIND, m_dwMonKind);
	DDX_Text(pDX, IDC_STATIC_NPC_ID, m_dwNpcID);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMSG);
	DDX_Control(pDX, IDC_CHECK_LOCK, m_cLOCK);
}

BOOL CTPathWndMAIN::Create( CWnd *pParentWnd)
{
	BOOL bResult = CTPathWndBase::Create( CTPathWndMAIN::IDD, pParentWnd);

	if(bResult)
	{
		CWnd *pCtrl = GetDlgItem(IDC_BUTTON_SHOW_NPC);

		if(pCtrl)
		{
			HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FIND_NPC);
			((CButton *) pCtrl)->SetIcon(hIcon);

			pCtrl = GetDlgItem(IDC_BUTTON_SHOW_MON);
			if(pCtrl)
				((CButton *) pCtrl)->SetIcon(hIcon);
		}

		pCtrl = GetDlgItem(IDC_BUTTON_PICK_POS);
		if(pCtrl)
		{
			HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PICK_POS);
			((CButton *) pCtrl)->SetIcon(hIcon);
		}

		pCtrl = GetDlgItem(IDC_LIST_STEP);
		if(pCtrl)
		{
			CListCtrl *pList = (CListCtrl *) pCtrl;
			CRect rect;

			pList->GetClientRect(&rect);
			rect.DeflateRect( 9, 0);

			pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
			pList->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());
		}

		pCtrl = GetDlgItem(IDC_LIST_MON);
		if(pCtrl)
		{
			CListCtrl *pList = (CListCtrl *) pCtrl;
			CRect rect;

			pList->GetClientRect(&rect);
			rect.DeflateRect( 9, 0);
			DWORD dwWidth = rect.Width() / 4;

			pList->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
			pList->InsertColumn( 0, _T("ID"), LVCFMT_LEFT, dwWidth);
			pList->InsertColumn( 1, _T("Name"), LVCFMT_LEFT, rect.Width() - dwWidth);
		}

		UpdateDATA();
	}

	return bResult;
}

void CTPathWndMAIN::UpdateDATA( int nType, LPARAM lParam)
{
	switch(nType)
	{
	case TUPDATE_RESET	: UpdateDATA(); break;
	case TUPDATE_NPC	:
		{
			CTQuestPathDoc *pDOC = GetDocument();

			if(pDOC)
			{
				m_strNpcName = pDOC->GetTNPCName(WORD(lParam));
				m_dwNpcID = DWORD(lParam);

				if(m_dwNpcID)
				{
					LPTNDATA pTNDATA = pDOC->m_vTGAMEDB.FindTNDATA(m_dwNpcID);

					if(pTNDATA)
					{
						m_strPosX.Format( IDS_FMT_POS_DATA, pTNDATA->m_fPosX);
						m_strPosY.Format( IDS_FMT_POS_DATA, pTNDATA->m_fPosY);
						m_strPosZ.Format( IDS_FMT_POS_DATA, pTNDATA->m_fPosZ);

						m_dwMapID = pTNDATA->m_wMapID;
					}

					m_cMonName.DeleteAllItems();
					m_dwMonKind = 0;
				}

				UpdateData(FALSE);
			}
		}

		break;

	case TUPDATE_MON	:
		{
			CTQuestPathDoc *pDOC = GetDocument();

			if(pDOC)
			{
				m_cMonName.DeleteAllItems();
				m_dwMonKind = DWORD(lParam);

				if(m_dwMonKind)
				{
					TMDATA vTMDATA;

					if(pDOC->CalcTMONKIND( &vTMDATA, m_dwMonKind))
					{
						m_strPosX.Format( IDS_FMT_POS_DATA, vTMDATA.m_fPosX);
						m_strPosY.Format( IDS_FMT_POS_DATA, vTMDATA.m_fPosY);
						m_strPosZ.Format( IDS_FMT_POS_DATA, vTMDATA.m_fPosZ);
						m_dwMapID = vTMDATA.m_wMapID;

						pDOC->UpdateTMONName( &m_cMonName, m_dwMonKind, m_dwMapID);
					}

					m_strNpcName = pDOC->GetTNPCName(0);
					m_dwNpcID = 0;
				}

				UpdateData(FALSE);
			}
		}

		break;

	case TUPDATE_POS	:
		{
			CTQuestPathDoc *pDOC = GetDocument();
			LPTTITEM pTTITEM = (LPTTITEM) lParam;

			if( pDOC && pTTITEM )
			{
				m_strPosX.Format( IDS_FMT_POS_DATA, pTTITEM->m_fPosX);
				m_strPosZ.Format( IDS_FMT_POS_DATA, pTTITEM->m_fPosZ);
				m_dwMapID = pTTITEM->m_dwMapID;

				m_strNpcName = pDOC->GetTNPCName(0);
				m_dwNpcID = 0;

				m_cMonName.DeleteAllItems();
				m_dwMonKind = 0;

				UpdateData(FALSE);
			}
		}

		break;
	}
}

void CTPathWndMAIN::UpdateTMAP()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
		if(m_pTTITEM)
		{
			CString strPOS;
			TTITEM vTPOS;

			strPOS.Format(
				_T("%.2f %.2f %.2f"),
				atof(m_strPosX),
				atof(m_strPosY),
				atof(m_strPosZ));

			vTPOS.m_dwMonKind = m_dwMonKind;
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


BEGIN_MESSAGE_MAP(CTPathWndMAIN, CTPathWndBase)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STEP, OnLvnItemchangedListStep)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_STEP, OnNMRclickListStep)
	ON_NOTIFY(NM_CLICK, IDC_LIST_STEP, OnNMClickListStep)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_SET, OnBnClickedButtonAutoSet)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnBnClickedButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
	ON_BN_CLICKED(IDC_CHECK_LOCK, OnBnClickedCheckLock)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_NPC, OnBnClickedButtonShowNpc)
	ON_BN_CLICKED(IDC_BUTTON_SHOW_MON, OnBnClickedButtonShowMon)
	ON_BN_CLICKED(IDC_BUTTON_PICK_POS, OnBnClickedButtonPickPos)
END_MESSAGE_MAP()


// CTPathWndMAIN 메시지 처리기입니다.

void CTPathWndMAIN::EnableTermCtrl()
{
	CTQuestPathDoc *pDOC = GetDocument();
	BYTE bLOCK = m_pTTITEM && pDOC && pDOC->m_pTQITEM && pDOC->m_pTQITEM->m_bType == TQITEM_MISSION && pDOC->m_pTQITEM->m_pMission && pDOC->m_pTQITEM->m_pComplete ? TRUE : FALSE;
	BYTE bEnable = bLOCK && !m_pTTITEM->m_bLOCK ? TRUE : FALSE;

	m_cAutoset.EnableWindow(bEnable);
	m_cRestore.EnableWindow(bEnable);
	m_cApply.EnableWindow(bEnable);
	m_cMonName.EnableWindow(bEnable);
	m_cMonKind.EnableWindow(bEnable);
	m_cFindMON.EnableWindow(bEnable);
	m_cFindNPC.EnableWindow(bEnable);
	m_cPickPOS.EnableWindow(bEnable);
	m_cNpcName.EnableWindow(bEnable);
	m_cNpcID.EnableWindow(bEnable);
	m_cMapID.EnableWindow(bEnable);
	m_cPosX.EnableWindow(bEnable);
	m_cPosY.EnableWindow(bEnable);
	m_cPosZ.EnableWindow(bEnable);
	m_cMSG.EnableWindow(bEnable);
	m_cLOCK.EnableWindow(bLOCK);
}

void CTPathWndMAIN::EnableCtrl()
{
	CTQuestPathDoc *pDOC = GetDocument();
	BYTE bEnable = pDOC && pDOC->m_pTQITEM && pDOC->m_pTQITEM->m_bType == TQITEM_MISSION && pDOC->m_pTQITEM->m_pMission ? TRUE : FALSE;

	m_cQuestID.EnableWindow(bEnable);
	m_cTSTEP.EnableWindow(bEnable);
}

void CTPathWndMAIN::UpdateDATA()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(m_cTSTEP.GetSafeHwnd())
		m_cTSTEP.DeleteAllItems();

	m_pTTITEM = NULL;
	m_dwQuestID = 0;
	UpdateTERM();

	if( pDOC && pDOC->m_pTQITEM && pDOC->m_pTQITEM->m_bType == TQITEM_MISSION && pDOC->m_pTQITEM->m_pMission )
	{
		m_dwQuestID = pDOC->m_pTQITEM->m_pMission->m_dwQuestID;

		for( int i=0; i<INT(pDOC->m_pTQITEM->m_vTTERM.size()); i++)
		{
			LPTTITEM pTTITEM = pDOC->m_pTQITEM->m_vTTERM[i];
			CString strTEXT;

			switch(pTTITEM->m_vTTERM.m_bTermType)
			{
			case QTT_TSTART_POS	: strTEXT.LoadString(IDS_TQUEST_START_POS); break;
			case QTT_TCOMP_POS	: strTEXT.LoadString(IDS_TQUEST_COMP_POS); break;
			default				: strTEXT.Format( IDS_FMT_OBJECTIVE_POS, m_cTSTEP.GetItemCount()); break;
			}

			int nIndex = m_cTSTEP.InsertItem( m_cTSTEP.GetItemCount(), strTEXT);
			m_cTSTEP.SetItemData( nIndex, (DWORD_PTR) pTTITEM);
		}
	}

	UpdateData(FALSE);
	EnableCtrl();
}

void CTPathWndMAIN::UpdateTERM()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(m_cMonName.GetSafeHwnd())
		m_cMonName.DeleteAllItems();

	if(m_cLOCK.GetSafeHwnd())
		m_cLOCK.SetCheck(FALSE);

	m_strNpcName.Empty();
	m_strPosX.Empty();
	m_strPosY.Empty();
	m_strPosZ.Empty();
	m_strMSG.Empty();

	m_dwMonKind = 0;
	m_dwNpcID = 0;
	m_dwMapID = 0;

	if( m_pTTITEM && pDOC && pDOC->m_pTQITEM &&
		pDOC->m_pTQITEM->m_bType == TQITEM_MISSION &&
		pDOC->m_pTQITEM->m_pMission &&
		pDOC->m_pTQITEM->m_pComplete )
	{
		m_cLOCK.SetCheck(m_pTTITEM->m_bLOCK ? TRUE : FALSE);
		UpdateTERMDATA(m_pTTITEM);
	}

	UpdateData(FALSE);
	EnableTermCtrl();
}

void CTPathWndMAIN::UpdateTERMDATA( LPTTITEM pTTITEM)
{
	CTQuestPathDoc *pDOC = GetDocument();

	if( pDOC && pTTITEM )
	{
		m_dwMonKind = pTTITEM->m_dwMonKind;
		m_dwNpcID = pTTITEM->m_dwNpcID;
		m_dwMapID = pTTITEM->m_dwMapID;

		if(m_cMonName.GetSafeHwnd())
		{
			m_cMonName.DeleteAllItems();

			if(m_dwMonKind)
				pDOC->UpdateTMONName( &m_cMonName, m_dwMonKind, m_dwMapID);
		}

		m_strNpcName = pDOC->GetTNPCName((WORD) m_dwNpcID);
		m_strMSG = pTTITEM->m_vTTERM.m_strTermMSG;

		m_strPosX.Format( IDS_FMT_POS_DATA, pTTITEM->m_fPosX);
		m_strPosY.Format( IDS_FMT_POS_DATA, pTTITEM->m_fPosY);
		m_strPosZ.Format( IDS_FMT_POS_DATA, pTTITEM->m_fPosZ);

		pDOC->UpdateTMAP(pTTITEM);
		UpdateData(FALSE);
	}
}

void CTPathWndMAIN::OnLvnItemchangedListStep( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	POSITION pos = m_cTSTEP.GetFirstSelectedItemPosition();

	int nIndex = pos ? m_cTSTEP.GetNextSelectedItem(pos) : -1;
	LPTTITEM pNEW = nIndex != -1 ? (LPTTITEM) m_cTSTEP.GetItemData(nIndex) : NULL;

	if( m_pTTITEM != pNEW )
	{
		m_pTTITEM = pNEW;
		UpdateTERM();
	}

	*pResult = 0;
}

void CTPathWndMAIN::OnNMClickListStep( NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTSTEP.GetFirstSelectedItemPosition())
		UpdateTMAP();

	*pResult = 0;
}

void CTPathWndMAIN::OnNMRclickListStep(NMHDR *pNMHDR, LRESULT *pResult)
{
	if(!m_cTSTEP.GetFirstSelectedItemPosition())
		UpdateTMAP();

	*pResult = 0;
}

void CTPathWndMAIN::OnBnClickedButtonAutoSet()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		TTITEM vTTITEM;

		if(pDOC->GetAutosetData( &vTTITEM, pDOC->m_pTQITEM, m_pTTITEM))
			UpdateTERMDATA(&vTTITEM);
	}
}

void CTPathWndMAIN::OnBnClickedButtonRestore()
{
	UpdateTERMDATA(m_pTTITEM);
}

void CTPathWndMAIN::OnBnClickedButtonApply()
{
	if(m_pTTITEM)
	{
		CString strPOS;
		UpdateData();

		strPOS.Format(
			_T("%.2f %.2f %.2f"),
			atof(m_strPosX),
			atof(m_strPosY),
			atof(m_strPosZ));

		m_pTTITEM->m_dwMonKind = m_dwMonKind;
		m_pTTITEM->m_dwMapID = m_dwMapID;
		m_pTTITEM->m_dwNpcID = m_dwNpcID;

		sscanf( LPCSTR(strPOS),
			_T("%f %f %f"),
			&m_pTTITEM->m_fPosX,
			&m_pTTITEM->m_fPosY,
			&m_pTTITEM->m_fPosZ);

		UpdateTERMDATA(m_pTTITEM);
	}
}

void CTPathWndMAIN::OnBnClickedCheckLock()
{
	if(m_pTTITEM)
	{
		m_pTTITEM->m_bLOCK = m_cLOCK.GetCheck() ? TRUE : FALSE;

		if(m_pTTITEM->m_bLOCK)
			OnBnClickedButtonApply();
	}

	EnableTermCtrl();
}

void CTPathWndMAIN::OnBnClickedButtonShowNpc()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		pDOC->UpdateDATA( TPATHWND_NPC, m_dwNpcID ? TUPDATE_NPC : TUPDATE_NONE, m_dwNpcID);
		pDOC->EnableTPATHWND(TPATHWND_NPC);
	}
}

void CTPathWndMAIN::OnBnClickedButtonShowMon()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		pDOC->UpdateDATA( TPATHWND_MON, m_dwMonKind ? TUPDATE_MON : TUPDATE_NONE, m_dwMonKind);
		pDOC->EnableTPATHWND(TPATHWND_MON);
	}
}

void CTPathWndMAIN::OnBnClickedButtonPickPos()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		CString strPOS;
		TTITEM vTPOS;
		UpdateData();

		strPOS.Format(
			_T("%.2f %.2f %.2f"),
			atof(m_strPosX),
			atof(m_strPosY),
			atof(m_strPosZ));

		vTPOS.m_dwMapID = m_dwMapID;
		sscanf( LPCSTR(strPOS),
			_T("%f %f %f"),
			&vTPOS.m_fPosX,
			&vTPOS.m_fPosY,
			&vTPOS.m_fPosZ);

		pDOC->UpdateDATA( TPATHWND_POS, TUPDATE_RESET, (LPARAM) &vTPOS);
		pDOC->EnableTPATHWND(TPATHWND_POS);
		pDOC->m_nPathMode = TEDITPATH_PICK_POS;
	}
}
