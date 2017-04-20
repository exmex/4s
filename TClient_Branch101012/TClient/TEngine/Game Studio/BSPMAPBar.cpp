// BSPMAPBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BSPMAPFrame.h"
#include "BSPMAPDoc.h"
#include "BSPMAPBar.h"
#include "RebuildBSPDlg.h"
#include ".\bspmapbar.h"


// CBSPMAPBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CBSPMAPBar, CBarContent)
CBSPMAPBar::CBSPMAPBar(CWnd* pParent /*=NULL*/)
: CBarContent(CBSPMAPBar::IDD, pParent)
, m_strNodeID(_T(""))
, m_dwTotal(0)
, m_dwPVS(0)
, m_dwNodeCNT(0)
, m_strPVSINFO(_T(""))
, m_nLevel(0)
, m_dwRValue(0)
, m_dwGValue(0)
, m_dwBValue(0)
, m_bRegion(FALSE)
, m_bRegionID(0)
{
	m_pItem = NULL;
}

CBSPMAPBar::~CBSPMAPBar()
{
}

void CBSPMAPBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_NODEID, m_strNodeID);
	DDX_Text(pDX, IDC_STATIC_TOTAL, m_dwTotal);
	DDX_Text(pDX, IDC_STATIC_PVS, m_dwPVS);
	DDX_Text(pDX, IDC_STATIC_NODE, m_dwNodeCNT);
	DDX_Text(pDX, IDC_STATIC_PVSINFO, m_strPVSINFO);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_cDEL);
	DDX_Text(pDX, IDC_STATIC_LEVEL, m_nLevel);
	DDX_Control(pDX, IDC_SPIN_LEVEL, m_spinLevel);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_cColorBox);
	DDX_Control(pDX, IDC_STATIC_CLRCTRL, m_cControlBox);
	DDX_Text(pDX, IDC_EDIT_RVALUE, m_dwRValue);
	DDX_Text(pDX, IDC_EDIT_GVALUE, m_dwGValue);
	DDX_Text(pDX, IDC_EDIT_BVALUE, m_dwBValue);
	DDX_Control(pDX, IDC_SPIN_RVALUE, m_spinRValue);
	DDX_Control(pDX, IDC_SPIN_GVALUE, m_spinGValue);
	DDX_Control(pDX, IDC_SPIN_BVALUE, m_spinBValue);
	DDX_Control(pDX, IDC_BUTTON_PVS, m_cBuildPVS);
	DDX_Control(pDX, IDC_BUTTON_ADD, m_cADD);
	DDX_Control(pDX, IDC_EDIT_RVALUE, m_cRValue);
	DDX_Control(pDX, IDC_EDIT_GVALUE, m_cGValue);
	DDX_Control(pDX, IDC_EDIT_BVALUE, m_cBValue);
	DDX_Control(pDX, IDC_BUTTON_REBUILD, m_cRebuild);
	DDX_Control(pDX, IDC_BUTTON_OVERLAP, m_cOverlap);
	DDX_Control(pDX, IDC_BUTTON_EDIT_LIGHT, m_cEditLight);
	DDX_Control(pDX, IDC_BUTTON_EDIT_OBJ, m_cEditOBJ);
	DDX_Control(pDX, IDC_BUTTON_EDIT_SFX, m_cEditSFX);
	DDX_Control(pDX, IDC_BUTTON_EDIT_SND, m_cEditSND);
	DDX_Control(pDX, IDC_CHECK_EDIT_REGION, m_cRegion);
	DDX_Check(pDX, IDC_CHECK_EDIT_REGION, m_bRegion);
	DDX_Control(pDX, IDC_EDIT_REGION, m_cRegionID);
	DDX_Text(pDX, IDC_EDIT_REGION, m_bRegionID);
	DDX_Control(pDX, IDC_SPIN_REGION, m_spinRegionID);
}


BEGIN_MESSAGE_MAP(CBSPMAPBar, CBarContent)
	ON_MESSAGE( WM_COLOR_SELECTED, OnColorSelected)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_OBJ, OnBnClickedButtonEditObj)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_EN_CHANGE(IDC_EDIT_RVALUE, OnEnChangeEditRGB)
	ON_EN_CHANGE(IDC_EDIT_GVALUE, OnEnChangeEditRGB)
	ON_EN_CHANGE(IDC_EDIT_BVALUE, OnEnChangeEditRGB)
	ON_BN_CLICKED(IDC_BUTTON_REBUILD, OnBnClickedButtonRebuild)
	ON_BN_CLICKED(IDC_BUTTON_OVERLAP, OnBnClickedButtonOverlap)
	ON_BN_CLICKED(IDC_BUTTON_PVS, OnBnClickedButtonPvs)
	ON_BN_CLICKED(IDC_CHECK_EDIT_REGION, OnBnClickedCheckEditRegion)
	ON_EN_CHANGE(IDC_EDIT_REGION, OnEnChangeEditRegion)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_LIGHT, OnBnClickedButtonEditLight)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SFX, OnBnClickedButtonEditSfx)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_SND, OnBnClickedButtonEditSnd)
END_MESSAGE_MAP()


// CBSPMAPBar 메시지 처리기입니다.

void CBSPMAPBar::SetItem( CBSPMAPItem *pItem)
{
	m_pItem = pItem;

	m_strPVSINFO.LoadString(m_pItem->m_BSPMAP.m_bPVS ? IDS_ENABLE : IDS_DISABLE);
	m_dwTotal = m_pItem->m_dwPolyCNT;
	m_nLevel = m_pItem->m_BSPMAP.m_nTexIDX;
	m_spinLevel.SetRange( 0, INT(m_pItem->m_vTEX.size()) - 1);
	m_spinLevel.SetPos(m_nLevel);

	m_cColorCtrl.SetupColor( RGB(
		GETVALUE_R(m_pItem->m_BSPMAP.m_dwAmbient),
		GETVALUE_G(m_pItem->m_BSPMAP.m_dwAmbient),
		GETVALUE_B(m_pItem->m_BSPMAP.m_dwAmbient)), TRUE);
	EnableAllCtrl(TRUE);
}

BOOL CBSPMAPBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	m_cControlBox.GetClientRect(&rect);
	m_cControlBox.ClientToScreen(&rect);
	ScreenToClient(&rect);
	rect.DeflateRect( 1, 1);

	if(!m_cColorCtrl.Create(
		NULL, NULL,
		WS_CHILD|WS_VISIBLE,
		rect, this, 0))
		return FALSE;
	m_cColorCtrl = CTColorCtrl::MODE_HSL_PANEL;

	m_spinRegionID.SetRange( 0, 255);
	m_spinRValue.SetRange( 0, 255);
	m_spinGValue.SetRange( 0, 255);
	m_spinBValue.SetRange( 0, 255);

	return TRUE;
}

void CBSPMAPBar::EnableAllCtrl( BOOL bEnable)
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();

	m_spinRegionID.EnableWindow(bEnable && m_bRegion && pDoc->m_nIndex != -1);
	m_cRegionID.EnableWindow(bEnable && m_bRegion && pDoc->m_nIndex != -1);

	m_cDEL.EnableWindow(bEnable && INT(m_pItem->m_vTEX.size()) > 1);
	m_cBuildPVS.EnableWindow(bEnable && !m_pItem->m_BSPMAP.m_bPVS);

	m_spinRValue.EnableWindow(bEnable);
	m_spinGValue.EnableWindow(bEnable);
	m_spinBValue.EnableWindow(bEnable);
	m_spinLevel.EnableWindow(bEnable);

	m_cColorCtrl.EnableWindow(bEnable);
	m_cEditLight.EnableWindow(bEnable);
	m_cEditOBJ.EnableWindow(bEnable);
	m_cEditSFX.EnableWindow(bEnable);
	m_cEditSND.EnableWindow(bEnable);
	m_cRebuild.EnableWindow(bEnable);
	m_cOverlap.EnableWindow(bEnable);
	m_cRegion.EnableWindow(bEnable);
	m_cRValue.EnableWindow(bEnable);
	m_cGValue.EnableWindow(bEnable);
	m_cBValue.EnableWindow(bEnable);
	m_cADD.EnableWindow(bEnable);
}

void CBSPMAPBar::SetMeshIDX()
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();

	if(!m_bRegion)
		pDoc->m_nIndex = -1;

	if( pDoc->m_nIndex != -1 )
		m_bRegionID = m_pItem->m_BSPMAP.m_pRegionID[pDoc->m_nIndex];

	UpdateData(FALSE);
	EnableAllCtrl(TRUE);

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
}

void CBSPMAPBar::SetNodeInfo( DWORD dwNodeID,
							  DWORD dwNodeCNT,
							  DWORD dwPVS)
{
	if( dwNodeID == NODEID_NULL )
		m_strNodeID.LoadString(IDS_NODE_NULL);
	else
		m_strNodeID.Format( "%d", dwNodeID);

	m_dwNodeCNT = dwNodeCNT;
	m_dwPVS = dwPVS;
	UpdateData(FALSE);
}

LRESULT CBSPMAPBar::OnColorSelected( WPARAM wParam, LPARAM lParam)
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();

	m_dwRValue = GetRValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
	m_dwGValue = GetGValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
	m_dwBValue = GetBValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

	m_cColorBox.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
	m_pItem->m_BSPMAP.m_dwAmbient = VALUEARGB(
		0xFF,
		m_dwRValue,
		m_dwGValue,
		m_dwBValue);
	UpdateData(FALSE);

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();

	return 0;
}

void CBSPMAPBar::OnBnClickedButtonAdd()
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();

	LPTEXTURESET *pTEX = new LPTEXTURESET[m_pItem->m_BSPMAP.m_dwMeshCount];
	LPMAPDWORD pID = new MAPDWORD;

	memset( pTEX, 0x00, m_pItem->m_BSPMAP.m_dwMeshCount * sizeof(LPTEXTURESET));
	pID->clear();

	m_pItem->m_BSPMAP.m_nTexIDX++;
	m_nLevel++;

	m_pItem->m_BSPMAP.m_vTexture.insert( m_pItem->m_BSPMAP.m_vTexture.begin() + m_nLevel, pTEX);
	m_pItem->m_vTEX.insert( m_pItem->m_vTEX.begin() + m_nLevel, pID);
	m_spinLevel.SetRange( 0, INT(m_pItem->m_vTEX.size()) - 1);

	m_spinLevel.SetPos(m_nLevel);
	UpdateData(FALSE);

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
	EnableAllCtrl(TRUE);
}

void CBSPMAPBar::OnBnClickedButtonDelete()
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();

	VECTORTEXSET::iterator itTEX = m_pItem->m_BSPMAP.m_vTexture.begin() + m_nLevel;
	VECTORMAPDWORD::iterator itID = m_pItem->m_vTEX.begin() + m_nLevel;

	(*itID)->clear();
	delete (*itID);
	delete[] (*itTEX);

	m_pItem->m_BSPMAP.m_vTexture.erase(itTEX);
	m_pItem->m_vTEX.erase(itID);
	m_spinLevel.SetRange( 0, INT(m_pItem->m_vTEX.size()) - 1);

	if( m_nLevel >= INT(m_pItem->m_vTEX.size()) )
	{
		m_pItem->m_BSPMAP.m_nTexIDX = INT(m_pItem->m_vTEX.size()) - 1;
		m_nLevel = m_pItem->m_BSPMAP.m_nTexIDX;

		m_spinLevel.SetPos(m_nLevel);
	}

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
	EnableAllCtrl(TRUE);
}

void CBSPMAPBar::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar *pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinLevel && INT(nPos) != m_nLevel )
	{
		CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();

		m_pItem->m_BSPMAP.m_nTexIDX = nPos;
		m_nLevel = nPos;
		UpdateData(FALSE);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}

	CBarContent::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CBSPMAPBar::OnEnChangeEditRGB()
{
	if(m_pItem)
	{
		UpdateData();
		m_cColorCtrl.SetupColor( RGB(
			m_dwRValue,
			m_dwGValue,
			m_dwBValue), TRUE);
	}
}

void CBSPMAPBar::OnBnClickedButtonRebuild()
{
	CRebuildBSPDlg dlg;

	if( dlg.DoModal() == IDOK )
	{
		CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();

		CTBSPPortal::m_bBasePortalID = m_pItem->m_bBaseID;
		CTBSPNode::m_bBaseNodeID = m_pItem->m_bBaseID;

		m_pItem->m_BSPMAP.m_bPVS = dlg.m_bPVS;
		m_pItem->Build(dlg.m_strFile);

		if(dlg.m_bLIGHT)
			m_pItem->ClearLIGHT();

		if(dlg.m_bTEX)
		{
			m_pItem->ClearTEX();
			m_nLevel = 0;
		}

		if(dlg.m_bOBJ)
			m_pItem->ClearOBJ();

		if(dlg.m_bSFX)
			m_pItem->ClearSFX();

		if(dlg.m_bSND)
			m_pItem->ClearSND();

		m_pItem->m_BSPMAP.m_nTexIDX = m_nLevel;
		m_pItem->Regulate();

		SetItem(m_pItem);
		pDoc->ClearPVS();
		pDoc->BuildOBJ();

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CBSPMAPBar::OnBnClickedButtonOverlap()
{
	CFileDialog dlg(
		TRUE,
		_T("*.tmf"),
		NULL,
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		_T("Mesh Files (*.tmf)|*.tmf|All Files (*.*)|*.*||"));

	if( dlg.DoModal() == IDOK )
	{
		CFile file( dlg.GetPathName(), CFile::modeRead|CFile::typeBinary);
		DWORD dwLength = (DWORD) file.GetLength();

		if( dwLength > 0 )
		{
			CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();
			LPBYTE pDATA = new BYTE[dwLength];

			MAPMESHVB::iterator itMESH;
			MAPTEXVB::iterator itLIGHT;

			MAPTBSPNODE mapPVS;
			MAPTEXVB mapLIGHT;
			MAPMESHVB mapVB;

			file.Read( pDATA, dwLength);
			CTBSPNode::m_pProcLevel = NULL;
			CTBSPNode::m_pProcStep = NULL;
			m_pItem->m_BSPMAP.Overlap(pDATA);
			m_pItem->Regulate();
			delete[] pDATA;

			mapLIGHT.clear();
			mapPVS.clear();
			mapVB.clear();

			m_pItem->m_BSPMAP.BuildPVS( &mapPVS, &mapLIGHT, &mapVB, NODEID_NULL);
			m_pItem->m_dwPolyCNT = 0;

			for( itMESH = mapVB.begin(); itMESH != mapVB.end(); itMESH++)
			{
				m_pItem->m_dwPolyCNT += (*itMESH).second->m_dwCount;
				delete (*itMESH).second;
			}

			for( itLIGHT = mapLIGHT.begin(); itLIGHT != mapLIGHT.end(); itLIGHT++)
				delete (*itLIGHT).second;

			mapLIGHT.clear();
			mapPVS.clear();
			mapVB.clear();

			SetItem(m_pItem);
			pDoc->ClearPVS();

			if(!pDoc->GetPlayCtrl()->m_bPlay)
				pDoc->RedrawAllView();
		}
	}
}

void CBSPMAPBar::OnBnClickedButtonPvs()
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();
	CProgressDlg Progress;
	BMPARAM Param;

	m_pItem->m_BSPMAP.m_Head.ClearPVS();
	m_pItem->m_BSPMAP.m_bPVS = FALSE;

	Param.m_strFileName.Empty();
	Param.m_pWnd = &Progress;
	Param.m_pHost = m_pItem;

	Progress.m_strCancel.LoadString(IDS_CANCEL_BUILD_PVS);
	Progress.m_strTitle.LoadString(IDS_BUILD_PVS);
	Progress.m_pThreadFunc = (AFX_THREADPROC) BuildPVS;
	Progress.m_lParam = &Param;
	Progress.DoModal();

	SetItem(m_pItem);
	pDoc->ClearPVS();

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
}

DWORD CBSPMAPBar::BuildPVS( DWORD_PTR lParam)
{
	CTBSPNode::m_pProcLevel = (FNPROCLEVEL) CBSPMAPItem::__BuildLevel;
	CTBSPNode::m_pProcStep = (FNPROCSTEP) CBSPMAPItem::__BuildStep;
	((CBSPMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->m_BSPMAP.m_Head.BuildPVS((LPARAM) lParam);
	((CBSPMAPItem *) ((LPBMPARAM) lParam)->m_pHost)->m_BSPMAP.m_bPVS = TRUE;
	CTBSPNode::m_pProcLevel = NULL;
	CTBSPNode::m_pProcStep = NULL;

	((LPBMPARAM) lParam)->m_pWnd->SendMessage(
		WM_COMMAND,
		ID_PROGRESS_EXIT);

	return 0;
}

void CBSPMAPBar::OnBnClickedCheckEditRegion()
{
	UpdateData();
	SetMeshIDX();
}

void CBSPMAPBar::OnEnChangeEditRegion()
{
	if(m_pItem)
	{
		CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();
		UpdateData();

		if( pDoc->m_nIndex != -1 )
		{
			MAPDWORD::iterator finder = m_pItem->m_mapRegionID.find(pDoc->m_nIndex);

			if( finder == m_pItem->m_mapRegionID.end() )
				m_pItem->m_mapRegionID.insert( MAPDWORD::value_type( pDoc->m_nIndex, m_bRegionID));
			else
				(*finder).second = m_bRegionID;

			m_pItem->m_BSPMAP.m_pRegionID[pDoc->m_nIndex] = m_bRegionID;
		}
	}
}

void CBSPMAPBar::OnBnClickedButtonEditLight()
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();
	CBSPMAPFrame *pFrame = (CBSPMAPFrame *) GetParentFrame();
	BOOL bRegion = m_bRegion;

	m_bRegion = FALSE;
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
		SetMeshIDX();

	pDoc->m_bEditLIGHT = TRUE;
	pDoc->m_bEditSFX = FALSE;
	pDoc->m_bEditSND = FALSE;
	pDoc->m_bEditOBJ = FALSE;
	pDoc->ClearSELLIGHT();
	pDoc->ClearSELOBJ();
	pDoc->ClearSELSFX();
	pDoc->ClearSELSND();

	pDoc->RedrawAllView();
	EnableAllCtrl(TRUE);

	pFrame->m_wndControlBar.ShowWindow(SW_HIDE);
	pFrame->m_wndMAPLIGHTBar.ShowWindow(SW_SHOW);
	pFrame->DockControlBar(&pFrame->m_wndMAPLIGHTBar);
	pFrame->RepositionBars( 0, 0xFFFFFFFF, 0);
}

void CBSPMAPBar::OnBnClickedButtonEditObj()
{
	OnEditObject();
}

void CBSPMAPBar::OnEditObject()
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();
	CBSPMAPFrame *pFrame = (CBSPMAPFrame *) GetParentFrame();
	BOOL bRegion = m_bRegion;

	m_bRegion = FALSE;
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
		SetMeshIDX();

	pDoc->RedrawAllView();
	EnableAllCtrl(TRUE);

	pDoc->m_bEditLIGHT = FALSE;
	pDoc->m_bEditSFX = FALSE;
	pDoc->m_bEditSND = FALSE;
	pDoc->m_bEditOBJ = TRUE;
	pDoc->ClearSELLIGHT();
	pDoc->ClearSELOBJ();
	pDoc->ClearSELSFX();
	pDoc->ClearSELSND();

	pFrame->m_wndControlBar.ShowWindow(SW_HIDE);
	pFrame->m_wndMAPOBJBar.ShowWindow(SW_SHOW);
	pFrame->DockControlBar(&pFrame->m_wndMAPOBJBar);
	pFrame->RepositionBars( 0, 0xFFFFFFFF, 0);
}

void CBSPMAPBar::OnBnClickedButtonEditSfx()
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();
	CBSPMAPFrame *pFrame = (CBSPMAPFrame *) GetParentFrame();
	BOOL bRegion = m_bRegion;

	m_bRegion = FALSE;
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
		SetMeshIDX();

	pDoc->RedrawAllView();
	EnableAllCtrl(TRUE);

	pDoc->m_bEditLIGHT = FALSE;
	pDoc->m_bEditSND = FALSE;
	pDoc->m_bEditOBJ = FALSE;
	pDoc->m_bEditSFX = TRUE;
	pDoc->ClearSELLIGHT();
	pDoc->ClearSELOBJ();
	pDoc->ClearSELSFX();
	pDoc->ClearSELSND();

	pFrame->m_wndControlBar.ShowWindow(SW_HIDE);
	pFrame->m_wndMAPSFXBar.ShowWindow(SW_SHOW);
	pFrame->DockControlBar(&pFrame->m_wndMAPSFXBar);
	pFrame->RepositionBars( 0, 0xFFFFFFFF, 0);
}

void CBSPMAPBar::OnBnClickedButtonEditSnd()
{
	CBSPMAPDoc *pDoc = (CBSPMAPDoc *) m_pItem->GetMainDocument();
	CBSPMAPFrame *pFrame = (CBSPMAPFrame *) GetParentFrame();
	BOOL bRegion = m_bRegion;

	m_bRegion = FALSE;
	UpdateData(FALSE);

	if( bRegion != m_bRegion )
		SetMeshIDX();

	pDoc->RedrawAllView();
	EnableAllCtrl(TRUE);

	pDoc->m_bEditLIGHT = FALSE;
	pDoc->m_bEditSFX = FALSE;
	pDoc->m_bEditOBJ = FALSE;
	pDoc->m_bEditSND = TRUE;
	pDoc->ClearSELLIGHT();
	pDoc->ClearSELOBJ();
	pDoc->ClearSELSFX();
	pDoc->ClearSELSND();

	pFrame->m_wndControlBar.ShowWindow(SW_HIDE);
	pFrame->m_wndMAPSNDBar.ShowWindow(SW_SHOW);
	pFrame->DockControlBar(&pFrame->m_wndMAPSNDBar);
	pFrame->RepositionBars( 0, 0xFFFFFFFF, 0);
}
