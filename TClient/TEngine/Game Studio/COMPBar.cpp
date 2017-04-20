// COMPBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "MainFrm.h"
#include "COMPDoc.h"
#include "COMPBar.h"
#include ".\compbar.h"

// CCOMPBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CCOMPBar, CBarContent)
CCOMPBar::CCOMPBar(CWnd* pParent /*=NULL*/)
: CBarContent(CCOMPBar::IDD, pParent)
, m_strDefault(_T(""))
, m_strDisable(_T(""))
, m_nCtrlType(0)
, m_nWidth(0)
, m_nHeight(0)
, m_nUseage(0)
, m_nPosX(0)
, m_nPosY(0)
, m_nHotPoint(0)
, m_nSuperLevel(0)
, m_strSOUND(_T(""))
, m_nHMargine(0)
, m_nVMargine(0)
{
	m_strSOUND.LoadString(IDS_NOTUSE);
	m_pItem = NULL;
}

CCOMPBar::~CCOMPBar()
{
}

void CCOMPBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DEFIMG, m_cDefault);
	DDX_Text(pDX, IDC_EDIT_DEFIMG, m_strDefault);
	DDX_Control(pDX, IDC_BUTTON_DELDEFIMG, m_cDelDefault);
	DDX_Control(pDX, IDC_EDIT_DISIMG, m_cDisable);
	DDX_Text(pDX, IDC_EDIT_DISIMG, m_strDisable);
	DDX_Control(pDX, IDC_BUTTON_DELDISIMG, m_cDelDisable);
	DDX_Control(pDX, IDC_COMBO_CTRLTYPE, m_cCtrlType);
	DDX_CBIndex(pDX, IDC_COMBO_CTRLTYPE, m_nCtrlType);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_cWidth);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_cHeight);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDX_Control(pDX, IDC_SPIN_WIDTH, m_spinWidth);
	DDX_Control(pDX, IDC_SPIN_HEIGHT, m_spinHeight);
	DDX_Control(pDX, IDC_BUTTON_DELETE_IMAGE, m_cDelIMG);
	DDX_Control(pDX, IDC_BUTTON_MOVEUP, m_cUP);
	DDX_Control(pDX, IDC_BUTTON_MOVEDOWN, m_cDOWN);
	DDX_Control(pDX, IDC_LIST_COMP, m_cCOMP);
	DDX_Control(pDX, IDC_COMBO_USEAGE, m_cUseage);
	DDX_CBIndex(pDX, IDC_COMBO_USEAGE, m_nUseage);
	DDX_Control(pDX, IDC_EDIT_POSX, m_cPosX);
	DDX_Control(pDX, IDC_EDIT_POSY, m_cPosY);
	DDX_Text(pDX, IDC_EDIT_POSX, m_nPosX);
	DDX_Text(pDX, IDC_EDIT_POSY, m_nPosY);
	DDX_Control(pDX, IDC_SPIN_POSX, m_spinPosX);
	DDX_Control(pDX, IDC_SPIN_POSY, m_spinPosY);
	DDX_Control(pDX, IDC_RADIO_HOT_TL, m_cHotTL);
	DDX_Control(pDX, IDC_RADIO_HOT_TC, m_cHotTC);
	DDX_Control(pDX, IDC_RADIO_HOT_TR, m_cHotTR);
	DDX_Control(pDX, IDC_RADIO_HOT_CL, m_cHotCL);
	DDX_Control(pDX, IDC_RADIO_HOT_CC, m_cHotCC);
	DDX_Control(pDX, IDC_RADIO_HOT_CR, m_cHotCR);
	DDX_Control(pDX, IDC_RADIO_HOT_BL, m_cHotBL);
	DDX_Control(pDX, IDC_RADIO_HOT_BC, m_cHotBC);
	DDX_Control(pDX, IDC_RADIO_HOT_BR, m_cHotBR);
	DDX_Radio(pDX, IDC_RADIO_HOT_TL, m_nHotPoint);
	DDX_Control(pDX, IDC_EDIT_SL, m_cSuperLevel);
	DDX_Text(pDX, IDC_EDIT_SL, m_nSuperLevel);
	DDX_Control(pDX, IDC_SPIN_SL, m_spinSuperLevel);
	DDX_Control(pDX, IDC_EDIT_SOUND, m_cSOUND);
	DDX_Text(pDX, IDC_EDIT_SOUND, m_strSOUND);
	DDX_Control(pDX, IDC_BUTTON_DELSOUND, m_cDELSND);
	DDX_Control(pDX, IDC_EDIT_HMARGINE, m_cHMargine);
	DDX_Text(pDX, IDC_EDIT_HMARGINE, m_nHMargine);
	DDX_Control(pDX, IDC_SPIN_HMARGINE, m_spinHMargine);
	DDX_Control(pDX, IDC_SPIN_VMARGINE, m_spinVMargine);
	DDX_Control(pDX, IDC_EDIT_VMARGINE, m_cVMargine);
	DDX_Text(pDX, IDC_EDIT_VMARGINE, m_nVMargine);

	DDX_Control(pDX, IDC_PRINTLIST, m_PrintList);
}


BEGIN_MESSAGE_MAP(CCOMPBar, CBarContent)
	ON_CBN_SELCHANGE(IDC_COMBO_CTRLTYPE, OnCbnSelchangeComboCtrltype)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, OnEnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnEnChangeEditWidth)
	ON_CBN_SELCHANGE(IDC_COMBO_USEAGE, OnCbnSelchangeComboUseage)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMP, OnLvnItemchangedListComp)
	ON_BN_CLICKED(IDC_BUTTON_DELDEFIMG, OnBnClickedButtonDeldefimg)
	ON_BN_CLICKED(IDC_BUTTON_DELDISIMG, OnBnClickedButtonDeldisimg)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_IMAGE, OnBnClickedButtonDeleteImage)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, OnBnClickedButtonMovedown)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnBnClickedButtonMoveup)
	ON_EN_CHANGE(IDC_EDIT_POSX, OnEnChangeEditPosx)
	ON_EN_CHANGE(IDC_EDIT_POSY, OnEnChangeEditPosy)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_COMP, OnLvnBegindragListComp)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_COMP, OnLvnItemActivateListComp)
	ON_BN_CLICKED(IDC_RADIO_HOT_TL, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_TC, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_TR, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_CL, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_CC, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_CR, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_BL, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_BC, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_BR, OnBnClickedRadioHotPoint)
	ON_EN_CHANGE(IDC_EDIT_SL, OnEnChangeEditSl)
	ON_BN_CLICKED(IDC_BUTTON_DELSOUND, OnBnClickedButtonDelsound)
	ON_EN_CHANGE(IDC_EDIT_HMARGINE, OnEnChangeEditHmargine)
	ON_EN_CHANGE(IDC_EDIT_VMARGINE, OnEnChangeEditVmargine)
	ON_BN_CLICKED(IDC_BUTTON_ALL_MOVE, OnBnClickedButtonAllMove)
	ON_BN_CLICKED(IDC_PRINTLIST, OnBnClickedPrintlist)
	ON_BN_CLICKED(IDC_BUTTON_FIND_COMP, OnBnClickedButtonFindComp)
	ON_BN_CLICKED(IDC_BUTTON_FIND_DEFIMG, OnBnClickedButtonFindDefimg)
	ON_BN_CLICKED(IDC_BUTTON_FIND_DISIMG, OnBnClickedButtonFindDisimg)
	ON_BN_CLICKED(IDC_EXPORTIMGS, OnBnClickedExportimgs)
END_MESSAGE_MAP()


// CCOMPBar 메시지 처리기입니다.

BOOL CCOMPBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEL);
	m_cDelDefault.SetIcon(hIcon);
	m_cDelDisable.SetIcon(hIcon);
	m_cDELSND.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HOTTL);
	m_cHotTL.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HOTTC);
	m_cHotTC.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HOTTR);
	m_cHotTR.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HOTCL);
	m_cHotCL.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HOTCC);
	m_cHotCC.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HOTCR);
	m_cHotCR.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HOTBL);
	m_cHotBL.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HOTBC);
	m_cHotBC.SetIcon(hIcon);
	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HOTBR);
	m_cHotBR.SetIcon(hIcon);

	CListCtrl *pListCtrl = m_cCOMP.GetListCtrl();
	pListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	pListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cDefault.m_DropTarget.Register(&m_cDefault);
	m_cDefault.m_pDragEnter = OnDefaultDragOver;
	m_cDefault.m_pDragOver = OnDefaultDragOver;
	m_cDefault.m_pDrop = OnDefaultDrop;
	m_cDefault.m_pLeave = OnDefaultDragLeave;

	m_cDisable.m_DropTarget.Register(&m_cDisable);
	m_cDisable.m_pDragEnter = OnDisableDragOver;
	m_cDisable.m_pDragOver = OnDisableDragOver;
	m_cDisable.m_pDrop = OnDisableDrop;
	m_cDisable.m_pLeave = OnDisableDragLeave;

	m_cSOUND.m_DropTarget.Register(&m_cSOUND);
	m_cSOUND.m_pDragEnter = OnSNDDragOver;
	m_cSOUND.m_pDragOver = OnSNDDragOver;
	m_cSOUND.m_pDrop = OnSNDDrop;
	m_cSOUND.m_pLeave = OnSNDDragLeave;

	m_cCOMP.m_DropTarget.Register(&m_cCOMP);
	m_cCOMP.m_pDragEnter = OnCOMPDragOver;
	m_cCOMP.m_pDragOver = OnCOMPDragOver;
	m_cCOMP.m_pDrop = OnCOMPDrop;
	m_cCOMP.m_pLeave = OnCOMPDragLeave;

	m_spinHMargine.SetRange( -10000, 10000);
	m_spinVMargine.SetRange( -10000, 10000);

	m_spinSuperLevel.SetRange( 0, 10000);
	m_spinHeight.SetRange( 0, 10000);
	m_spinWidth.SetRange( 0, 10000);
	m_spinPosX.SetRange( 0, 10000);
	m_spinPosY.SetRange( 0, 10000);

	return TRUE;
}

void CCOMPBar::SetItem( CCOMPItem *pItem)
{
	CListCtrl *pListCtrl = m_cCOMP.GetListCtrl();
	m_pItem = pItem;

	int nCount = INT(m_pItem->m_vCOMPITEM.size());
	pListCtrl->DeleteAllItems();

	for( int i=0; i<nCount; i++)
		pListCtrl->InsertItem( i, m_pItem->m_vCOMPITEM[i]->m_strName);

	if(m_pItem->m_pDefault)
		m_strDefault = m_pItem->m_pDefault->m_strName;
	else
		m_strDefault.LoadString(IDS_NOTUSE);

	if(m_pItem->m_pDisable)
		m_strDisable = m_pItem->m_pDisable->m_strName;
	else
		m_strDisable.LoadString(IDS_NOTUSE);

	m_cCtrlType.ResetContent();
	m_cCtrlType.AddString("None");
	m_cCtrlType.AddString("Button");
	m_cCtrlType.AddString("Scroll");
	m_cCtrlType.AddString("Edit");
	m_cCtrlType.AddString("List");
	m_cCtrlType.AddString("Combo");
	m_cCtrlType.AddString("Guage");
	m_cCtrlType.AddString("Imagelist");
	m_cCtrlType.AddString("Meter");
	m_cCtrlType.AddString("Tab");

	m_nHotPoint = ((CCOMPDoc *) m_pItem->GetMainDocument())->m_bHotPoint;
	m_nCtrlType = m_pItem->m_bCtrlType;
	m_nSuperLevel = m_pItem->m_nLevel;
	m_nHMargine = m_pItem->m_nHMargine;
	m_nVMargine = m_pItem->m_nVMargine;
	m_nWidth = m_pItem->m_nWidth;
	m_nHeight = m_pItem->m_nHeight;

	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CCOMPBar::EnableAllCtrl( BOOL bEnable)
{
	m_cDelDefault.EnableWindow(bEnable && m_pItem->m_dwDefaultID != 0);
	m_cDelDisable.EnableWindow(bEnable && m_pItem->m_dwDisableID != 0);
	m_cUseage.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1);
	m_cDelIMG.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1);
	m_cDOWN.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1);
	m_cUP.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1);

	m_cDELSND.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1 && m_pItem->m_vCOMP[m_cCOMP.GetCurSel()]->m_dwMediaID != 0);
	m_cSOUND.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1);

	m_spinPosX.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1);
	m_spinPosY.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1);

	m_cPosX.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1);
	m_cPosY.EnableWindow(bEnable && m_cCOMP.GetCurSel() != -1);

	m_spinSuperLevel.EnableWindow(bEnable && m_nCtrlType == CT_METER);
	m_cSuperLevel.EnableWindow(bEnable && m_nCtrlType == CT_METER);

	m_cHotTL.EnableWindow(bEnable);
	m_cHotTC.EnableWindow(bEnable);
	m_cHotTR.EnableWindow(bEnable);
	m_cHotCL.EnableWindow(bEnable);
	m_cHotCC.EnableWindow(bEnable);
	m_cHotCR.EnableWindow(bEnable);
	m_cHotBL.EnableWindow(bEnable);
	m_cHotBC.EnableWindow(bEnable);
	m_cHotBR.EnableWindow(bEnable);

	m_cDefault.EnableWindow(bEnable);
	m_cDisable.EnableWindow(bEnable);
	m_cCOMP.EnableWindow(bEnable);

	m_spinHeight.EnableWindow(bEnable);
	m_spinWidth.EnableWindow(bEnable);
	m_cCtrlType.EnableWindow(bEnable);

	m_spinHMargine.EnableWindow(bEnable);
	m_spinVMargine.EnableWindow(bEnable);

	m_cHMargine.EnableWindow(bEnable);
	m_cVMargine.EnableWindow(bEnable);

	m_cHeight.EnableWindow(bEnable);
	m_cWidth.EnableWindow(bEnable);
}

void CCOMPBar::OnCbnSelchangeComboCtrltype()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_bCtrlType = m_nCtrlType;
		ResetCOMP(m_cCOMP.GetCurSel());
	}
}

void CCOMPBar::OnEnChangeEditHeight()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_nHeight = m_nHeight;
	}
}

void CCOMPBar::OnEnChangeEditWidth()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_nWidth = m_nWidth;
	}
}

void CCOMPBar::OnEnChangeEditHmargine()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_nHMargine = m_nHMargine;
	}
}

void CCOMPBar::OnEnChangeEditVmargine()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_nVMargine = m_nVMargine;
	}
}

void CCOMPBar::OnCbnSelchangeComboUseage()
{
	if(m_pItem)
	{
		int nIndex = m_cCOMP.GetCurSel();

		UpdateData();
		if( nIndex != -1 )
			m_pItem->m_vCOMP[nIndex]->m_bUseage = m_nUseage;
	}
}

void CCOMPBar::OnEnChangeEditPosx()
{
	if(m_pItem)
	{
		int nIndex = m_cCOMP.GetCurSel();

		UpdateData();
		if( nIndex != -1 )
		{
			m_pItem->m_vCOMP[nIndex]->m_nPosX = m_nPosX;
			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CCOMPBar::OnEnChangeEditPosy()
{
	if(m_pItem)
	{
		int nIndex = m_cCOMP.GetCurSel();

		UpdateData();
		if( nIndex != -1 )
		{
			m_pItem->m_vCOMP[nIndex]->m_nPosY = m_nPosY;
			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CCOMPBar::OnLvnItemchangedListComp( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( m_cCOMP.m_bUISelect && (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		ResetCOMP(pNMLV->iItem);

	EnableAllCtrl(TRUE);
	m_pItem->GetMainDocument()->UpdateAllViews(NULL);

	*pResult = 0;
}

void CCOMPBar::ResetCOMP( int nIndex)
{
	m_cUseage.ResetContent();
	m_cUseage.AddString("None");

	switch(m_nCtrlType)
	{
	case CT_NONE	:
	case CT_IMGLIST	:
	case CT_TABCTRL	: break;
	case CT_BUTTON	:
		{
			m_cUseage.AddString("Down");
			m_cUseage.AddString("Up");
			m_cUseage.AddString("Hover");
		}

		break;

	case CT_SCROLL	:
		{
			m_cUseage.AddString("Up/Left");
			m_cUseage.AddString("Down/Right");
			m_cUseage.AddString("Bar");
		}

		break;

	case CT_EDIT	:
		{
			m_cUseage.AddString("Caret");
		}

		break;

	case CT_LIST	:
		{
			m_cUseage.AddString("Item");
			m_cUseage.AddString("Vertical");
		}

		break;

	case CT_COMBO	:
		{
			m_cUseage.AddString("Drop");
			m_cUseage.AddString("Down List");
		}

		break;

	case CT_GAUGE	:
		{
			m_cUseage.AddString("Bar");
		}

		break;

	case CT_METER	:
		{
			m_cUseage.AddString("Sub Level");
			m_cUseage.AddString("Super Level");
		}

		break;
	}

	if( nIndex != -1 )
	{
		CMediaItem *pSND = CItemBase::GetMediaItem(m_pItem->m_vCOMP[nIndex]->m_dwMediaID);

		if(!pSND)
		{
			m_pItem->m_vCOMP[nIndex]->m_dwMediaID = 0;
			m_strSOUND.LoadString(IDS_NOTUSE);
		}
		else
			m_strSOUND = pSND->m_strName;

		m_nUseage = m_pItem->m_vCOMP[nIndex]->m_bUseage;
		m_nPosX = m_pItem->m_vCOMP[nIndex]->m_nPosX;
		m_nPosY = m_pItem->m_vCOMP[nIndex]->m_nPosY;
		UpdateData(FALSE);

		if( m_cUseage.GetCurSel() == -1 )
		{
			m_pItem->m_vCOMP[nIndex]->m_bUseage = 0;
			m_nUseage = 0;
			UpdateData(FALSE);
		}
	}

	EnableAllCtrl(TRUE);
	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CCOMPBar::SetDefault( CImageItem *pIMG)
{
	m_pItem->m_dwDefaultID = pIMG->m_dwID;
	m_pItem->m_pDefault = pIMG;

	m_strDefault = pIMG->m_strName;
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CCOMPBar::SetDisable( CImageItem *pIMG)
{
	m_pItem->m_dwDisableID = pIMG->m_dwID;
	m_pItem->m_pDisable = pIMG;

	m_strDisable = pIMG->m_strName;
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CCOMPBar::SetSOUND( CMediaItem *pSND)
{
	if(m_pItem)
	{
		int nIndex = m_cCOMP.GetCurSel();

		if( nIndex != -1 )
		{
			m_pItem->m_vCOMP[nIndex]->m_dwMediaID = pSND->m_dwID;
			ResetCOMP(nIndex);
		}
	}
}

void CCOMPBar::OnBnClickedButtonDeldefimg()
{
	m_pItem->m_dwDefaultID = 0;
	m_pItem->m_pDefault = NULL;

	m_strDefault.LoadString(IDS_NOTUSE);
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CCOMPBar::OnBnClickedButtonDeldisimg()
{
	m_pItem->m_dwDisableID = 0;
	m_pItem->m_pDisable = NULL;

	m_strDisable.LoadString(IDS_NOTUSE);
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CCOMPBar::OnBnClickedButtonDelsound()
{
	if(m_pItem)
	{
		int nIndex = m_cCOMP.GetCurSel();

		if( nIndex != -1 )
		{
			m_pItem->m_vCOMP[nIndex]->m_dwMediaID = 0;
			ResetCOMP(nIndex);
		}
	}
}

void CCOMPBar::OnBnClickedButtonDeleteImage()
{
	DeleteCOMP(m_cCOMP.GetCurSel());
}

void CCOMPBar::OnBnClickedButtonMovedown()
{
	int nIndex = m_cCOMP.GetCurSel();

	if( nIndex == INT(m_pItem->m_vCOMP.size() - 1) )
		MoveItemPos( nIndex, 0);
	else
		MoveItemPos( nIndex, nIndex + 1);
}

void CCOMPBar::OnBnClickedButtonMoveup()
{
	int nIndex = m_cCOMP.GetCurSel();

	if( nIndex == 0 )
		MoveItemPos( nIndex, INT(m_pItem->m_vCOMP.size() - 1));
	else
		MoveItemPos( nIndex, nIndex - 1);
}

void CALLBACK CCOMPBar::OnDefaultDragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

void CALLBACK CCOMPBar::OnDisableDragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

void CALLBACK CCOMPBar::OnSNDDragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

void CALLBACK CCOMPBar::OnCOMPDragLeave( CGSListCtrl *pList)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pList->CListView::OnDragLeave();
}

BOOL CCOMPBar::CanDefaultDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_IMAGE;
}

BOOL CCOMPBar::CanDisableDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_IMAGE;
}

BOOL CCOMPBar::CanSNDDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_MEDIA && ((CMediaItem *) pItem)->GetPlayerType() == MEDIA_TSOUND;
}

BOOL CCOMPBar::CanCOMPDrop( CCOMPBar *pBar, CItemBase *pItem)
{
	return pItem->m_bType == ITEM_COMP && !((CCOMPItem *) pItem)->FindCOMPItem(pBar->m_pItem->m_dwID);
}

DROPEFFECT CALLBACK CCOMPBar::OnDefaultDragOver( CGSEditCtrl *pEdit,
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
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanDefaultDrop(pItem))
				nResult = DROPEFFECT_MOVE;
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	pEdit->ClientToScreen(&screenpos);
	CGameStudioView::m_pDragImage->DragMove(screenpos);

	return nResult;
}

DROPEFFECT CALLBACK CCOMPBar::OnDisableDragOver( CGSEditCtrl *pEdit,
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
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanDisableDrop(pItem))
				nResult = DROPEFFECT_MOVE;
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	pEdit->ClientToScreen(&screenpos);
	CGameStudioView::m_pDragImage->DragMove(screenpos);

	return nResult;
}

DROPEFFECT CALLBACK CCOMPBar::OnSNDDragOver( CGSEditCtrl *pEdit,
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
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanSNDDrop(pItem))
				nResult = DROPEFFECT_MOVE;
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	pEdit->ClientToScreen(&screenpos);
	CGameStudioView::m_pDragImage->DragMove(screenpos);

	return nResult;
}

DROPEFFECT CALLBACK CCOMPBar::OnCOMPDragOver( CGSListCtrl *pList,
											  COleDataObject *pDataObject,
											  DWORD dwKeyState,
											  CPoint point)
{
	CCOMPBar *pBar = (CCOMPBar *) pList->GetParent();
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_GSLIST	:
		{
			LPGSLISTITEM pItem = (LPGSLISTITEM) pData->m_pData;

			if( pItem->m_pHost == pBar )
			{
				int nIndex = pList->GetListCtrl()->HitTest(point);

				if( nIndex != pList->GetCurSel() )
				{
					CGameStudioView::m_pDragImage->DragMove( CPoint(
						-10000,
						-10000));

					pList->SetCurSel(nIndex);
					pBar->ResetCOMP(nIndex);
				}

				nResult = DROPEFFECT_MOVE;
			}
		}

		break;

	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanCOMPDrop( pBar, pItem))
			{
				int nIndex = pList->GetListCtrl()->HitTest(point);

				if( nIndex != pList->GetCurSel() )
				{
					CGameStudioView::m_pDragImage->DragMove( CPoint(
						-10000,
						-10000));

					pList->SetCurSel(nIndex);
					pBar->ResetCOMP(nIndex);
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

BOOL CALLBACK CCOMPBar::OnDefaultDrop( CGSEditCtrl *pEdit,
									   COleDataObject* pDataObject,
									   DROPEFFECT dropEffect,
									   CPoint point)
{
	CCOMPBar *pBar = (CCOMPBar *) pEdit->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanDefaultDrop(pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->SetDefault((CImageItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnDefaultDragLeave(pEdit);
	return pEdit->CEditView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CALLBACK CCOMPBar::OnDisableDrop( CGSEditCtrl *pEdit,
									   COleDataObject* pDataObject,
									   DROPEFFECT dropEffect,
									   CPoint point)
{
	CCOMPBar *pBar = (CCOMPBar *) pEdit->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanDefaultDrop(pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->SetDisable((CImageItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnDisableDragLeave(pEdit);
	return pEdit->CEditView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CALLBACK CCOMPBar::OnSNDDrop( CGSEditCtrl *pEdit,
								   COleDataObject* pDataObject,
								   DROPEFFECT dropEffect,
								   CPoint point)
{
	CCOMPBar *pBar = (CCOMPBar *) pEdit->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanSNDDrop(pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->SetSOUND((CMediaItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnSNDDragLeave(pEdit);
	return pEdit->CEditView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CALLBACK CCOMPBar::OnCOMPDrop( CGSListCtrl *pList,
								    COleDataObject* pDataObject,
									DROPEFFECT dropEffect,
									CPoint point)
{
	CCOMPBar *pBar = (CCOMPBar *) pList->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_GSLIST	:
		{
			LPGSLISTITEM pItem = (LPGSLISTITEM) pData->m_pData;

			if( pItem->m_pHost == pBar )
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->MoveItemPos( pItem->m_nIndex, pList->GetListCtrl()->HitTest(point));
			}
		}

		break;

	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanCOMPDrop( pBar, pItem))
			{
				LPCOMP pCOMP = new COMP();

				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pCOMP->m_dwCompID = pItem->m_dwID;
				pCOMP->m_bUseage = 0;
				pCOMP->m_nPosX = 0;
				pCOMP->m_nPosY = 0;

				pBar->InsertCOMP(
					pCOMP,
					pList->GetListCtrl()->HitTest(point));
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnCOMPDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

void CCOMPBar::DeleteCOMP( int nIndex)
{
	CListCtrl *pListCtrl = m_cCOMP.GetListCtrl();

	delete m_pItem->m_vCOMP[nIndex];
	m_pItem->m_vCOMPITEM.erase(m_pItem->m_vCOMPITEM.begin() + nIndex);
	m_pItem->m_vCOMP.erase(m_pItem->m_vCOMP.begin() + nIndex);

	pListCtrl->DeleteItem(nIndex);
	m_cCOMP.UpdateColumnWidth();

	if( nIndex >= pListCtrl->GetItemCount() )
		nIndex = pListCtrl->GetItemCount() - 1;
	m_cCOMP.SetCurSel(nIndex);
	ResetCOMP(nIndex);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
	EnableAllCtrl(TRUE);
}

void CCOMPBar::InsertCOMP( LPCOMP pCOMP, int nIndex)
{
	CCOMPItem *pItem = CItemBase::GetCOMPItem(pCOMP->m_dwCompID);
	CListCtrl *pListCtrl = m_cCOMP.GetListCtrl();

	if( nIndex == -1 )
		nIndex = INT(m_pItem->m_vCOMP.size());

	m_pItem->m_vCOMP.insert(
		m_pItem->m_vCOMP.begin() + nIndex,
		pCOMP);

	m_pItem->m_vCOMPITEM.insert(
		m_pItem->m_vCOMPITEM.begin() + nIndex,
		pItem);

	nIndex = pListCtrl->InsertItem( nIndex, pItem->m_strName);
	m_cCOMP.SetCurSel(nIndex);
	ResetCOMP(nIndex);
	m_cCOMP.UpdateColumnWidth();

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
	EnableAllCtrl(TRUE);
}

void CCOMPBar::MoveItemPos( int nFrom, int nTo)
{
	if( nFrom == nTo )
		return;

	CListCtrl *pListCtrl = m_cCOMP.GetListCtrl();
	LPCOMP pCOMP = new COMP();

	pCOMP->m_dwCompID = m_pItem->m_vCOMP[nFrom]->m_dwCompID;
	pCOMP->m_bUseage = m_pItem->m_vCOMP[nFrom]->m_bUseage;
	pCOMP->m_nPosX = m_pItem->m_vCOMP[nFrom]->m_nPosX;
	pCOMP->m_nPosY = m_pItem->m_vCOMP[nFrom]->m_nPosY;

	DeleteCOMP(nFrom);
	InsertCOMP( pCOMP, nTo);
}

void CCOMPBar::OnLvnBegindragListComp(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	HGLOBAL hGlobal = ::GlobalAlloc( GMEM_SHARE|GMEM_MOVEABLE, sizeof(GSCDATA));
	COleDataSource source;
	GSLISTITEM vItem;

	CGameStudioView::m_pDragImage = m_cCOMP.GetListCtrl()->CreateDragImage(
		pNMLV->iItem,
		&pNMLV->ptAction);

	CGameStudioView::m_pDragImage->BeginDrag( 0, CPoint(0, 0));
	CGameStudioView::m_pDragImage->DragEnter(
		GetDesktopWindow(),
		CPoint( -10000, -10000));

	vItem.m_nIndex = pNMLV->iItem;
	vItem.m_pHost = this;

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

void CCOMPBar::OnLvnItemActivateListComp(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

//	DeleteCOMP(pNMIA->iItem);
	*pResult = 0;
}

void CCOMPBar::OnBnClickedRadioHotPoint()
{
	CCOMPDoc *pDoc = (CCOMPDoc *) m_pItem->GetMainDocument();
	UpdateData();

	pDoc->m_bHotPoint = m_nHotPoint;
}

void CCOMPBar::OnEnChangeEditSl()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_nLevel = m_nSuperLevel;
	}
}

void CCOMPBar::OnBnClickedButtonAllMove()
{
	if(m_pItem)
	{
		for( int i=0; i<INT(m_pItem->m_vCOMP.size()); i++)
		{
			m_pItem->m_vCOMP[i]->m_nPosX = 0;
			m_pItem->m_vCOMP[i]->m_nPosY = 0;
		}

		m_pItem->GetMainDocument()->UpdateAllViews(NULL);
	}
}

void CCOMPBar::OnBnClickedPrintlist()
{
	CListCtrl *pListCtrl = m_cCOMP.GetListCtrl();
	int size = pListCtrl->GetItemCount();
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition();
	char szFilter[] = "Text(*.*) | *.txt";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if(dlg.DoModal() == IDOK)
	{
		CString FileName = dlg.GetPathName();
		CString str;
		FILE* fp = fopen(FileName, "w");
		for(int i = 0; i < size; i++)
		{
			str = pListCtrl->GetItemText(i, 0);
			fprintf(fp, "%s\t%d\n", str, i);
		}
		fclose(fp);
	}
}



void CCOMPBar::OnBnClickedButtonFindComp()
{
	BOOL bFind = FALSE;
	DWORD dwID = 0;

	if(m_pItem)
	{
		int nIndex = m_cCOMP.GetCurSel();

		UpdateData();
		if( nIndex != -1 )
		{
			bFind = TRUE;
			dwID = m_pItem->m_vCOMP[nIndex]->m_dwCompID;
		}
	}

	if( bFind && dwID != 0 )
	{
		// dwID로 아이템을 찾자.

		CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();

		if( pMainWnd )
		{
			HTREEITEM hItem = NULL;
			hItem = pMainWnd->m_pDocComp->FindItemByID( dwID );

			if( hItem )
			{
				pMainWnd->m_pDocComp->GetTreeCtrl().SelectItem( hItem );
				pMainWnd->SetActiveWorkspace( 8 );
			}
		}
	}
}

void CCOMPBar::OnBnClickedButtonFindDefimg()
{
	if( m_pItem )
	{
        CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();

		if( pMainWnd )
		{
			HTREEITEM hItem = NULL;
			hItem = pMainWnd->m_pDocImage->FindItemByID( m_pItem->m_dwDefaultID );

			if( hItem )
			{
				pMainWnd->m_pDocImage->GetTreeCtrl().SelectItem( hItem );
				pMainWnd->SetActiveWorkspace( 1 );
			}
		}
	}
}
void CCOMPBar::OnBnClickedButtonFindDisimg()
{
	if( m_pItem )
	{
		CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();

		if( pMainWnd )
		{
			HTREEITEM hItem = NULL;
			hItem = pMainWnd->m_pDocImage->FindItemByID( m_pItem->m_dwDisableID );

			if( hItem )
			{
				pMainWnd->m_pDocImage->GetTreeCtrl().SelectItem( hItem );
				pMainWnd->SetActiveWorkspace( 1 );
			}
		}
	}
}

void CCOMPBar::OnBnClickedExportimgs()
{
	if( !m_pItem )
		return;

	LPDIRECT3DDEVICE9 pDevice = CMainFrame::m_3DDevice.m_pDevice;

	char szFilter[] = "Image(*.*) | *.tga";
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY, szFilter);
	if( dlg.DoModal() == IDOK )
	{
		CString strPath = dlg.GetPathName();
		
		int nCnt = (int)(m_pItem->m_vCOMPITEM.size());
		for(int i=0; i<nCnt; ++i)
		{
			CCOMPItem* pComp = m_pItem->m_vCOMPITEM[i];
			if( pComp->m_pDefault )
			{
				IMAGESET* pImgset = &(pComp->m_pDefault->m_ImageSet);
				if( !pImgset->m_vImage.empty() )
				{
					CString strFileName;
					strFileName.Format("%s%d.tga", strPath,i);

					CD3DImage* pImg = pImgset->m_vImage.front();
					pImg->SaveImageFile(pDevice,strFileName,D3DXIFF_TGA);
				}
			}
		}
	}
}










