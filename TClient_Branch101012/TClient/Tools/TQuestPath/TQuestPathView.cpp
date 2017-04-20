// TQuestPathView.cpp : CTQuestPathView 클래스의 구현
//

#include "stdafx.h"
#include "TQuestPath.h"

#include "TQuestPathDoc.h"
#include "TQuestPathView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CTQuestPathView

IMPLEMENT_DYNCREATE(CTQuestPathView, CFormView)

BEGIN_MESSAGE_MAP(CTQuestPathView, CFormView)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_STEP, OnLvnItemchangedListStep)
	ON_BN_CLICKED(IDC_BUTTON_AUTO_SET, OnBnClickedButtonAutoSet)
	ON_BN_CLICKED(IDC_BUTTON_RESTORE, OnBnClickedButtonRestore)
	ON_BN_CLICKED(IDC_BUTTON_APPLY, OnBnClickedButtonApply)
END_MESSAGE_MAP()

// CTQuestPathView 생성/소멸

CTQuestPathView::CTQuestPathView()
	: CFormView(CTQuestPathView::IDD)
	, m_strNpcName(_T(""))
	, m_strPosX(_T(""))
	, m_strPosY(_T(""))
	, m_strPosZ(_T(""))
	, m_strMAP(_T(""))
	, m_strMSG(_T(""))
	, m_dwQuestID(0)
	, m_dwNpcID(0)
	, m_dwMapID(0)
{
	m_pTTITEM = NULL;
}

CTQuestPathView::~CTQuestPathView()
{
}

void CTQuestPathView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
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
	DDX_Control(pDX, IDC_STATIC_NPC_ID, m_cNpcID);
	DDX_Control(pDX, IDC_EDIT_MESSAGE, m_cMSG);
	DDX_Control(pDX, IDC_LIST_STEP, m_cTSTEP);
	DDX_Control(pDX, IDC_BUTTON_MAP, m_cMAP);
	DDX_Text(pDX, IDC_STATIC_QUEST_ID, m_dwQuestID);
	DDX_Text(pDX, IDC_EDIT_NPC_NAME, m_strNpcName);
	DDX_Text(pDX, IDC_EDIT_NPC_MAP_ID, m_dwMapID);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_X, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_Y, m_strPosY);
	DDX_Text(pDX, IDC_EDIT_NPC_POS_Z, m_strPosZ);
	DDX_Text(pDX, IDC_STATIC_NPC_ID, m_dwNpcID);
	DDX_Text(pDX, IDC_EDIT_MESSAGE, m_strMSG);
	DDX_Text(pDX, IDC_EDIT_MAP, m_strMAP);
}

BOOL CTQuestPathView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	// Window 클래스 또는 스타일을 수정합니다.

	return CFormView::PreCreateWindow(cs);
}

void CTQuestPathView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FOLDER);
	m_cMAP.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_FIND_NPC);
	m_cFindNPC.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PICK_POS);
	m_cPickPOS.SetIcon(hIcon);

	CRect rect;
	m_cTSTEP.GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	m_cTSTEP.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cTSTEP.InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	UpdateDATA();
}


// CTQuestPathView 진단

#ifdef _DEBUG
void CTQuestPathView::AssertValid() const
{
	CFormView::AssertValid();
}

void CTQuestPathView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CTQuestPathDoc* CTQuestPathView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CTQuestPathDoc)));
	return (CTQuestPathDoc*)m_pDocument;
}
#endif //_DEBUG


// CTQuestPathView 메시지 처리기

void CTQuestPathView::EnableTermCtrl()
{
	CTQuestPathDoc *pDOC = GetDocument();
	BYTE bEnable = m_pTTITEM && pDOC && pDOC->m_pTQITEM && pDOC->m_pTQITEM->m_bType == TQITEM_MISSION && pDOC->m_pTQITEM->m_pMission && pDOC->m_pTQITEM->m_pComplete ? TRUE : FALSE;

	m_cAutoset.EnableWindow(bEnable);
	m_cRestore.EnableWindow(bEnable);
	m_cApply.EnableWindow(bEnable);
	m_cFindNPC.EnableWindow(bEnable);
	m_cPickPOS.EnableWindow(bEnable);
	m_cNpcName.EnableWindow(bEnable);
	m_cNpcID.EnableWindow(bEnable);
	m_cMapID.EnableWindow(bEnable);
	m_cPosX.EnableWindow(bEnable);
	m_cPosY.EnableWindow(bEnable);
	m_cPosZ.EnableWindow(bEnable);
	m_cMSG.EnableWindow(bEnable);
}

void CTQuestPathView::EnableCtrl()
{
	CTQuestPathDoc *pDOC = GetDocument();
	BYTE bEnable = pDOC && pDOC->m_pTQITEM && pDOC->m_pTQITEM->m_bType == TQITEM_MISSION && pDOC->m_pTQITEM->m_pMission ? TRUE : FALSE;

	m_cQuestID.EnableWindow(bEnable);
	m_cTSTEP.EnableWindow(bEnable);
}

void CTQuestPathView::UpdateDATA()
{
	CTQuestPathDoc *pDOC = GetDocument();

	m_cTSTEP.DeleteAllItems();
	m_dwQuestID = 0;
	m_pTTITEM = NULL;
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

void CTQuestPathView::UpdateTERM()
{
	CTQuestPathDoc *pDOC = GetDocument();

	m_strNpcName.Empty();
	m_strPosX.Empty();
	m_strPosY.Empty();
	m_strPosZ.Empty();
	m_strMSG.Empty();

	m_dwNpcID = 0;
	m_dwMapID = 0;

	if( m_pTTITEM && pDOC && pDOC->m_pTQITEM &&
		pDOC->m_pTQITEM->m_bType == TQITEM_MISSION &&
		pDOC->m_pTQITEM->m_pMission &&
		pDOC->m_pTQITEM->m_pComplete )
		UpdateTERMDATA();

	UpdateData(FALSE);
	EnableTermCtrl();
}

void CTQuestPathView::UpdateTERMDATA()
{
	CTQuestPathDoc *pDOC = GetDocument();

	m_dwNpcID = m_pTTITEM->m_dwNpcID;
	m_dwMapID = m_pTTITEM->m_dwMapID;

	m_strNpcName = pDOC->GetTNPCName((WORD) m_dwNpcID);
	m_strMSG = m_pTTITEM->m_vTTERM.m_strTermMSG;

	m_strPosX.Format( _T("%.2f"), m_pTTITEM->m_fPosX);
	m_strPosY.Format( _T("%.2f"), m_pTTITEM->m_fPosY);
	m_strPosZ.Format( _T("%.2f"), m_pTTITEM->m_fPosZ);
}

void CTQuestPathView::OnLvnItemchangedListStep( NMHDR *pNMHDR, LRESULT *pResult)
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

void CTQuestPathView::OnBnClickedButtonAutoSet()
{
	CTQuestPathDoc *pDOC = GetDocument();

	if(pDOC)
	{
		TTITEM vTTITEM;

		if(pDOC->GetAutosetData( &vTTITEM, pDOC->m_pTQITEM, m_pTTITEM))
		{
			m_strNpcName = pDOC->GetTNPCName((WORD) vTTITEM.m_dwNpcID);

			m_strPosX.Format( IDS_FMT_POS_DATA, vTTITEM.m_fPosX);
			m_strPosY.Format( IDS_FMT_POS_DATA, vTTITEM.m_fPosY);
			m_strPosZ.Format( IDS_FMT_POS_DATA, vTTITEM.m_fPosZ);

			m_dwMapID = vTTITEM.m_dwMapID;
			m_dwNpcID = vTTITEM.m_dwNpcID;

			UpdateData(FALSE);
		}
	}
}

void CTQuestPathView::OnBnClickedButtonRestore()
{
}

void CTQuestPathView::OnBnClickedButtonApply()
{
}
