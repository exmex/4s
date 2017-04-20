// FRAMEBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "EditFontDlg.h"
#include "EditMenuDlg.h"
#include "FRAMEDoc.h"
#include "FRAMEBar.h"
#include "TCML.h"
#include "MainFrm.h"

// CFRAMEBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CFRAMEBar, CBarContent)
CFRAMEBar::CFRAMEBar(CWnd* pParent /*=NULL*/)
: CBarContent(CFRAMEBar::IDD, pParent)
, m_strDefault(_T(""))
, m_strDisable(_T(""))
, m_nWidth(0)
, m_nHeight(0)
, m_strMenuID(_T(""))
, m_strText(_T(""))
, m_strToolTip(_T(""))
, m_nPosX(0)
, m_nPosY(0)
, m_nHotPoint(0)
, m_bState(FALSE)
, m_bPasswd(FALSE)
, m_nAlign(0)
, m_nNotify(0)
, m_bCusColor(FALSE)
, m_dwRValue(0)
, m_dwGValue(0)
, m_dwBValue(0)
, m_dwAlpha(0)
, m_strTEXTFONT(_T(""))
{
	m_pItem = NULL;
}

CFRAMEBar::~CFRAMEBar()
{
}

void CFRAMEBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_DEFIMG, m_cDefault);
	DDX_Text(pDX, IDC_EDIT_DEFIMG, m_strDefault);
	DDX_Control(pDX, IDC_EDIT_DISIMG, m_cDisable);
	DDX_Text(pDX, IDC_EDIT_DISIMG, m_strDisable);
	DDX_Control(pDX, IDC_EDIT_WIDTH, m_cWidth);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_nWidth);
	DDX_Control(pDX, IDC_EDIT_HEIGHT, m_cHeight);
	DDX_Text(pDX, IDC_EDIT_HEIGHT, m_nHeight);
	DDX_Control(pDX, IDC_SPIN_WIDTH, m_spinWidth);
	DDX_Control(pDX, IDC_SPIN_HEIGHT, m_spinHeight);
	DDX_Control(pDX, IDC_BUTTON_DELDEFIMG, m_cDelDefault);
	DDX_Control(pDX, IDC_BUTTON_DELDISIMG, m_cDelDisable);
	DDX_Control(pDX, IDC_BUTTON_DELETE_CTRL, m_cDelCtrl);
	DDX_Control(pDX, IDC_BUTTON_MOVEUP, m_cUP);
	DDX_Control(pDX, IDC_BUTTON_MOVEDOWN, m_cDOWN);
	DDX_Control(pDX, IDC_LIST_CTRL, m_cCTRL);
	DDX_Control(pDX, IDC_EDIT_MENUID, m_cMenuID);
	DDX_Text(pDX, IDC_EDIT_MENUID, m_strMenuID);
	DDX_Control(pDX, IDC_BUTTON_MENUID, m_cMENU);
	DDX_Control(pDX, IDC_EDIT_TEXT, m_cText);
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
	DDX_Control(pDX, IDC_BUTTON_TEXTFONT, m_cTextFont);
	DDX_Control(pDX, IDC_EDIT_TOOLTIP, m_cToolTip);
	DDX_Text(pDX, IDC_EDIT_TOOLTIP, m_strToolTip);
	DDX_Control(pDX, IDC_BUTTON_TIPFONT, m_cTipFont);
	DDX_Control(pDX, IDC_EDIT_POSX, m_cPosX);
	DDX_Text(pDX, IDC_EDIT_POSX, m_nPosX);
	DDX_Control(pDX, IDC_EDIT_POSY, m_cPosY);
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
	DDX_Control(pDX, IDC_COMBO_ALIGN, m_cAlign);
	DDX_Radio(pDX, IDC_RADIO_HOT_TL, m_nHotPoint);
	DDX_Control(pDX, IDC_CHECK_STATEBTN, m_cState);
	DDX_Control(pDX, IDC_CHECK_PASSWD, m_cPasswd);
	DDX_Check(pDX, IDC_CHECK_STATEBTN, m_bState);
	DDX_Check(pDX, IDC_CHECK_PASSWD, m_bPasswd);
	DDX_CBIndex(pDX, IDC_COMBO_ALIGN, m_nAlign);
	DDX_Control(pDX, IDC_COMBO_NOTIFY, m_cNotify);
	DDX_CBIndex(pDX, IDC_COMBO_NOTIFY, m_nNotify);
	DDX_Control(pDX, IDC_CHECK_CUSTOMCLR, m_cCusColor);
	DDX_Check(pDX, IDC_CHECK_CUSTOMCLR, m_bCusColor);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_cColorBox);
	DDX_Control(pDX, IDC_STATIC_CLRCTRL, m_cControlBox);
	DDX_Control(pDX, IDC_EDIT_RVALUE, m_cRValue);
	DDX_Control(pDX, IDC_EDIT_GVALUE, m_cGValue);
	DDX_Control(pDX, IDC_EDIT_BVALUE, m_cBValue);
	DDX_Control(pDX, IDC_EDIT_ALPHA, m_cAlpha);
	DDX_Control(pDX, IDC_SPIN_RVALUE, m_spinRValue);
	DDX_Control(pDX, IDC_SPIN_GVALUE, m_spinGValue);
	DDX_Control(pDX, IDC_SPIN_BVALUE, m_spinBValue);
	DDX_Control(pDX, IDC_SPIN_ALPHA, m_spinAlpha);
	DDX_Text(pDX, IDC_EDIT_RVALUE, m_dwRValue);
	DDX_Text(pDX, IDC_EDIT_GVALUE, m_dwGValue);
	DDX_Text(pDX, IDC_EDIT_BVALUE, m_dwBValue);
	DDX_Text(pDX, IDC_EDIT_ALPHA, m_dwAlpha);
	DDX_Text(pDX, IDC_EDIT_FONT_VIEW_TEXT, m_strTEXTFONT);
	DDX_Text(pDX, IDC_EDIT_FONT_VIEW_TIP, m_strTIPFONT);
}


BEGIN_MESSAGE_MAP(CFRAMEBar, CBarContent)
	ON_MESSAGE( WM_COLOR_SELECTED, OnColorSelected)
	ON_EN_CHANGE(IDC_EDIT_HEIGHT, OnEnChangeEditHeight)
	ON_EN_CHANGE(IDC_EDIT_WIDTH, OnEnChangeEditWidth)
	ON_EN_CHANGE(IDC_EDIT_POSX, OnEnChangeEditPosx)
	ON_EN_CHANGE(IDC_EDIT_POSY, OnEnChangeEditPosy)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_CTRL, OnLvnItemchangedListCtrl)
	ON_BN_CLICKED(IDC_BUTTON_DELDEFIMG, OnBnClickedButtonDeldefimg)
	ON_BN_CLICKED(IDC_BUTTON_DELDISIMG, OnBnClickedButtonDeldisimg)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_CTRL, OnBnClickedButtonDeleteCtrl)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, OnBnClickedButtonMovedown)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnBnClickedButtonMoveup)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_CTRL, OnLvnBegindragListCtrl)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_CTRL, OnLvnItemActivateListCtrl)
	ON_EN_CHANGE(IDC_EDIT_TEXT, OnEnChangeEditText)
	ON_EN_CHANGE(IDC_EDIT_TOOLTIP, OnEnChangeEditTooltip)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_CTRL, OnLvnEndlabeleditListCtrl)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_CTRL, OnLvnBeginlabeleditListCtrl)
	ON_BN_CLICKED(IDC_BUTTON_TIPFONT, OnBnClickedButtonTipfont)
	ON_BN_CLICKED(IDC_BUTTON_TEXTFONT, OnBnClickedButtonTextfont)
	ON_BN_CLICKED(IDC_BUTTON_MENUID, OnBnClickedButtonMenuid)
	ON_BN_CLICKED(IDC_RADIO_HOT_TL, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_TC, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_TR, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_CL, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_CC, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_CR, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_BL, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_BC, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_RADIO_HOT_BR, OnBnClickedRadioHotPoint)
	ON_BN_CLICKED(IDC_CHECK_PASSWD, OnBnClickedCheckPasswd)
	ON_BN_CLICKED(IDC_CHECK_STATEBTN, OnBnClickedCheckStatebtn)
	ON_CBN_SELCHANGE(IDC_COMBO_ALIGN, OnCbnSelchangeComboAlign)
	ON_CBN_SELCHANGE(IDC_COMBO_NOTIFY, OnCbnSelchangeComboNotify)
	ON_EN_CHANGE(IDC_EDIT_ALPHA, OnEnChangeEditAlpha)
	ON_EN_CHANGE(IDC_EDIT_RVALUE, OnEnChangeEditRGB)
	ON_EN_CHANGE(IDC_EDIT_GVALUE, OnEnChangeEditRGB)
	ON_EN_CHANGE(IDC_EDIT_BVALUE, OnEnChangeEditRGB)
	ON_BN_CLICKED(IDC_CHECK_CUSTOMCLR, OnBnClickedCheckCustomclr)
	ON_BN_CLICKED(IDC_BUTTON_FIND_CTRL, OnBnClickedButtonFindCtrl)
	ON_BN_CLICKED(IDC_BUTTON_FIND_DEFIMG, OnBnClickedButtonFindDefimg)
	ON_BN_CLICKED(IDC_BUTTON_FIND_DISIMG, OnBnClickedButtonFindDisimg)
END_MESSAGE_MAP()


// CFRAMEBar 메시지 처리기입니다.

BOOL CFRAMEBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEL);
	m_cDelDefault.SetIcon(hIcon);
	m_cDelDisable.SetIcon(hIcon);

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

	CListCtrl *pListCtrl = m_cCTRL.GetListCtrl();
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

	m_cCTRL.m_DropTarget.Register(&m_cCTRL);
	m_cCTRL.m_pDragEnter = OnCTRLDragOver;
	m_cCTRL.m_pDragOver = OnCTRLDragOver;
	m_cCTRL.m_pDrop = OnCTRLDrop;
	m_cCTRL.m_pLeave = OnCTRLDragLeave;

	m_spinHeight.SetRange( 0, 10000);
	m_spinWidth.SetRange( 0, 10000);
	m_spinPosX.SetRange( 0, 10000);
	m_spinPosY.SetRange( 0, 10000);
	m_spinRValue.SetRange( 0, 255);
	m_spinGValue.SetRange( 0, 255);
	m_spinBValue.SetRange( 0, 255);
	m_spinAlpha.SetRange( 0, 100);

	return TRUE;
}

void CFRAMEBar::SetItem( CFRAMEItem *pItem)
{
	CListCtrl *pListCtrl = m_cCTRL.GetListCtrl();
	m_pItem = pItem;

	int nCount = INT(m_pItem->m_vCOMP.size());
	pListCtrl->DeleteAllItems();

	for( int i=0; i<nCount; i++)
		pListCtrl->InsertItem( i, m_pItem->m_vCTRL[i]->m_strCtrlID);

	if(m_pItem->m_pDefault)
		m_strDefault = m_pItem->m_pDefault->m_strName;
	else
		m_strDefault.LoadString(IDS_NOTUSE);

	if(m_pItem->m_pDisable)
		m_strDisable = m_pItem->m_pDisable->m_strName;
	else
		m_strDisable.LoadString(IDS_NOTUSE);

	m_nHotPoint = ((CFRAMEDoc *) m_pItem->GetMainDocument())->m_bHotPoint;
	m_nWidth = m_pItem->m_nWidth;
	m_nHeight = m_pItem->m_nHeight;

	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CFRAMEBar::EnableAllCtrl( BOOL bEnable)
{
	int nCurSel = m_cCTRL.GetCurSel();

	m_cDelDefault.EnableWindow(bEnable && m_pItem->m_dwDefaultID != 0);
	m_cDelDisable.EnableWindow(bEnable && m_pItem->m_dwDisableID != 0);
	m_cTextFont.EnableWindow(bEnable && nCurSel != -1);
	m_cTipFont.EnableWindow(bEnable && nCurSel != -1);
	m_cDelCtrl.EnableWindow(bEnable && nCurSel != -1);
	m_cToolTip.EnableWindow(bEnable && nCurSel != -1);
	m_cMenuID.EnableWindow(bEnable && nCurSel != -1);
	m_cMENU.EnableWindow(bEnable && nCurSel != -1);
	m_cText.EnableWindow(bEnable && nCurSel != -1);
	m_cDOWN.EnableWindow(bEnable && nCurSel != -1);
	m_cUP.EnableWindow(bEnable && nCurSel != -1);

	m_spinPosX.EnableWindow(bEnable && nCurSel != -1);
	m_spinPosY.EnableWindow(bEnable && nCurSel != -1);
	m_cPosX.EnableWindow(bEnable && nCurSel != -1);
	m_cPosY.EnableWindow(bEnable && nCurSel != -1);

	m_cPasswd.EnableWindow(bEnable && nCurSel != -1 && m_pItem->m_vCOMP[nCurSel]->m_bCtrlType == CT_EDIT);
	m_cState.EnableWindow(bEnable && nCurSel != -1 && m_pItem->m_vCOMP[nCurSel]->m_bCtrlType == CT_BUTTON);
	m_cCusColor.EnableWindow(bEnable && nCurSel != -1);
	m_cNotify.EnableWindow(bEnable && nCurSel != -1);
	m_cAlign.EnableWindow(bEnable && nCurSel != -1);

	m_spinRValue.EnableWindow(bEnable && nCurSel != -1 && (m_pItem->m_vCTRL[nCurSel]->m_dwFlag&TS_CUSTOM_COLOR));
	m_spinGValue.EnableWindow(bEnable && nCurSel != -1 && (m_pItem->m_vCTRL[nCurSel]->m_dwFlag&TS_CUSTOM_COLOR));
	m_spinBValue.EnableWindow(bEnable && nCurSel != -1 && (m_pItem->m_vCTRL[nCurSel]->m_dwFlag&TS_CUSTOM_COLOR));
	m_spinAlpha.EnableWindow(bEnable && nCurSel != -1 && (m_pItem->m_vCTRL[nCurSel]->m_dwFlag&TS_CUSTOM_COLOR));
	m_cRValue.EnableWindow(bEnable && nCurSel != -1 && (m_pItem->m_vCTRL[nCurSel]->m_dwFlag&TS_CUSTOM_COLOR));
	m_cGValue.EnableWindow(bEnable && nCurSel != -1 && (m_pItem->m_vCTRL[nCurSel]->m_dwFlag&TS_CUSTOM_COLOR));
	m_cBValue.EnableWindow(bEnable && nCurSel != -1 && (m_pItem->m_vCTRL[nCurSel]->m_dwFlag&TS_CUSTOM_COLOR));
	m_cAlpha.EnableWindow(bEnable && nCurSel != -1 && (m_pItem->m_vCTRL[nCurSel]->m_dwFlag&TS_CUSTOM_COLOR));
	m_cColorCtrl.EnableWindow(bEnable && nCurSel != -1 && (m_pItem->m_vCTRL[nCurSel]->m_dwFlag&TS_CUSTOM_COLOR));

	m_cHotTL.EnableWindow(bEnable);
	m_cHotTC.EnableWindow(bEnable);
	m_cHotTR.EnableWindow(bEnable);
	m_cHotCL.EnableWindow(bEnable);
	m_cHotCC.EnableWindow(bEnable);
	m_cHotCR.EnableWindow(bEnable);
	m_cHotBL.EnableWindow(bEnable);
	m_cHotBC.EnableWindow(bEnable);
	m_cHotBR.EnableWindow(bEnable);

	m_cColorBox.EnableWindow(bEnable);
	m_cDefault.EnableWindow(bEnable);
	m_cDisable.EnableWindow(bEnable);
	m_cCTRL.EnableWindow(bEnable);

	m_spinHeight.EnableWindow(bEnable);
	m_spinWidth.EnableWindow(bEnable);

	m_cHeight.EnableWindow(bEnable);
	m_cWidth.EnableWindow(bEnable);
}

void CFRAMEBar::OnEnChangeEditHeight()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_nHeight = m_nHeight;
	}
}

void CFRAMEBar::OnEnChangeEditWidth()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_nWidth = m_nWidth;
	}
}

void CFRAMEBar::OnEnChangeEditPosx()
{
	if(m_pItem)
	{
		int nIndex = m_cCTRL.GetCurSel();

		UpdateData();
		if( nIndex != -1 )
		{
			m_pItem->m_vCTRL[nIndex]->m_nPosX = m_nPosX;
			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CFRAMEBar::OnEnChangeEditPosy()
{
	if(m_pItem)
	{
		int nIndex = m_cCTRL.GetCurSel();

		UpdateData();
		if( nIndex != -1 )
		{
			m_pItem->m_vCTRL[nIndex]->m_nPosY = m_nPosY;
			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CFRAMEBar::OnEnChangeEditText()
{
	if(m_pItem)
	{
		int nIndex = m_cCTRL.GetCurSel();

		UpdateData();
		if( nIndex != -1 )
		{
			m_pItem->m_vCTRL[nIndex]->m_strText = m_strText;
			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CFRAMEBar::OnEnChangeEditTooltip()
{
	if(m_pItem)
	{
		int nIndex = m_cCTRL.GetCurSel();

		UpdateData();
		if( nIndex != -1 )
		{
			m_pItem->m_vCTRL[nIndex]->m_strToolTip = m_strToolTip;
			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CFRAMEBar::OnLvnItemchangedListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( m_cCTRL.m_bUISelect && (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		ResetCTRL(pNMLV->iItem);

	EnableAllCtrl(TRUE);
	m_pItem->GetMainDocument()->UpdateAllViews(NULL);

	*pResult = 0;
}

void CFRAMEBar::ResetCTRL( int nIndex)
{
	if( nIndex != -1 )
	{
		UpdateData();

		m_strMenuID = m_pItem->m_vCTRL[nIndex]->m_strMenuID[m_nNotify];
		m_strToolTip = m_pItem->m_vCTRL[nIndex]->m_strToolTip;
		m_strText = m_pItem->m_vCTRL[nIndex]->m_strText;

		m_strTEXTFONT = m_pItem->m_vCTRL[nIndex]->m_strFontID;
		m_strTIPFONT = m_pItem->m_vCTRL[nIndex]->m_strTipID;

		m_nPosX = m_pItem->m_vCTRL[nIndex]->m_nPosX;
		m_nPosY = m_pItem->m_vCTRL[nIndex]->m_nPosY;

		m_bCusColor = m_pItem->m_vCTRL[nIndex]->m_dwFlag & TS_CUSTOM_COLOR ? TRUE : FALSE;
		m_bState = m_pItem->m_vCTRL[nIndex]->m_dwFlag & TBS_STATE_BUTTON ? TRUE : FALSE;
		m_bPasswd = m_pItem->m_vCTRL[nIndex]->m_dwFlag & TES_PASSWORD ? TRUE : FALSE;
		m_nAlign = m_pItem->m_vCTRL[nIndex]->m_bAlign;

		m_dwAlpha = GETVALUE_A(m_pItem->m_vCTRL[nIndex]->m_dwColor) * 100 / 255;
		m_cColorCtrl.SetupColor( RGB(
			GETVALUE_R(m_pItem->m_vCTRL[nIndex]->m_dwColor),
			GETVALUE_G(m_pItem->m_vCTRL[nIndex]->m_dwColor),
			GETVALUE_B(m_pItem->m_vCTRL[nIndex]->m_dwColor)), TRUE);
		UpdateData(FALSE);
	}

	EnableAllCtrl(TRUE);
	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CFRAMEBar::SetDefault( CImageItem *pIMG)
{
	m_pItem->m_dwDefaultID = pIMG->m_dwID;
	m_pItem->m_pDefault = pIMG;

	m_strDefault = pIMG->m_strName;
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CFRAMEBar::SetDisable( CImageItem *pIMG)
{
	m_pItem->m_dwDisableID = pIMG->m_dwID;
	m_pItem->m_pDisable = pIMG;

	m_strDisable = pIMG->m_strName;
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CFRAMEBar::OnBnClickedButtonDeldefimg()
{
	m_pItem->m_dwDefaultID = 0;
	m_pItem->m_pDefault = NULL;

	m_strDefault.LoadString(IDS_NOTUSE);
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CFRAMEBar::OnBnClickedButtonDeldisimg()
{
	m_pItem->m_dwDisableID = 0;
	m_pItem->m_pDisable = NULL;

	m_strDisable.LoadString(IDS_NOTUSE);
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CFRAMEBar::OnBnClickedButtonDeleteCtrl()
{
	DeleteCTRL(m_cCTRL.GetCurSel());
}

void CFRAMEBar::OnBnClickedButtonMovedown()
{
	int nIndex = m_cCTRL.GetCurSel();

	if( nIndex == INT(m_pItem->m_vCTRL.size() - 1) )
		MoveItemPos( nIndex, 0);
	else
		MoveItemPos( nIndex, nIndex + 1);
}

void CFRAMEBar::OnBnClickedButtonMoveup()
{
	int nIndex = m_cCTRL.GetCurSel();

	if( nIndex == 0 )
		MoveItemPos( nIndex, INT(m_pItem->m_vCOMP.size() - 1));
	else
		MoveItemPos( nIndex, nIndex - 1);
}

void CALLBACK CFRAMEBar::OnDefaultDragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

void CALLBACK CFRAMEBar::OnDisableDragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

void CALLBACK CFRAMEBar::OnCTRLDragLeave( CGSListCtrl *pList)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pList->CListView::OnDragLeave();
}

BOOL CFRAMEBar::CanDefaultDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_IMAGE;
}

BOOL CFRAMEBar::CanDisableDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_IMAGE;
}

BOOL CFRAMEBar::CanCTRLDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_COMP;
}

DROPEFFECT CALLBACK CFRAMEBar::OnDefaultDragOver( CGSEditCtrl *pEdit,
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

DROPEFFECT CALLBACK CFRAMEBar::OnDisableDragOver( CGSEditCtrl *pEdit,
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

DROPEFFECT CALLBACK CFRAMEBar::OnCTRLDragOver( CGSListCtrl *pList,
											   COleDataObject *pDataObject,
											   DWORD dwKeyState,
											   CPoint point)
{
	CFRAMEBar *pBar = (CFRAMEBar *) pList->GetParent();
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
					pBar->ResetCTRL(nIndex);
				}

				nResult = DROPEFFECT_MOVE;
			}
		}

		break;

	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanCTRLDrop(pItem))
			{
				int nIndex = pList->GetListCtrl()->HitTest(point);

				if( nIndex != pList->GetCurSel() )
				{
					CGameStudioView::m_pDragImage->DragMove( CPoint(
						-10000,
						-10000));

					pList->SetCurSel(nIndex);
					pBar->ResetCTRL(nIndex);
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

BOOL CALLBACK CFRAMEBar::OnDefaultDrop( CGSEditCtrl *pEdit,
									    COleDataObject* pDataObject,
										DROPEFFECT dropEffect,
										CPoint point)
{
	CFRAMEBar *pBar = (CFRAMEBar *) pEdit->GetParent();

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

BOOL CALLBACK CFRAMEBar::OnDisableDrop( CGSEditCtrl *pEdit,
									    COleDataObject* pDataObject,
										DROPEFFECT dropEffect,
										CPoint point)
{
	CFRAMEBar *pBar = (CFRAMEBar *) pEdit->GetParent();

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

BOOL CALLBACK CFRAMEBar::OnCTRLDrop( CGSListCtrl *pList,
									 COleDataObject* pDataObject,
									 DROPEFFECT dropEffect,
									 CPoint point)
{
	CFRAMEBar *pBar = (CFRAMEBar *) pList->GetParent();

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

			if(CanCTRLDrop(pItem))
			{
				LPCTRL pCTRL = new CTRL();

				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				for( int i=0; i<TCML_MENU_COUNT; i++)
					pCTRL->m_strMenuID[i].Empty();

				pCTRL->m_strToolTip.Empty();
				pCTRL->m_strCtrlID.Empty();
				pCTRL->m_strFontID.Empty();
				pCTRL->m_strTipID.Empty();
				pCTRL->m_strText.Empty();

				pCTRL->m_dwCompID = pItem->m_dwID;
				pCTRL->m_dwColor = 0xFF000000;
				pCTRL->m_bAlign = GSTA_LEFT;
				pCTRL->m_dwFlag = 0;
				pCTRL->m_nPosX = 0;
				pCTRL->m_nPosY = 0;

				pBar->InsertCTRL(
					pCTRL,
					pList->GetListCtrl()->HitTest(point));
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnCTRLDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

void CFRAMEBar::DeleteCTRL( int nIndex)
{
	CListCtrl *pListCtrl = m_cCTRL.GetListCtrl();

	CItemBase::ReleaseLocalID( m_pItem->m_vCTRL[nIndex]->m_strCtrlID, m_pItem);
	delete m_pItem->m_vCTRL[nIndex];

	m_pItem->m_vCTRL.erase(m_pItem->m_vCTRL.begin() + nIndex);
	m_pItem->m_vCOMP.erase(m_pItem->m_vCOMP.begin() + nIndex);

	pListCtrl->DeleteItem(nIndex);
	m_cCTRL.UpdateColumnWidth();

	if( nIndex >= pListCtrl->GetItemCount() )
		nIndex = pListCtrl->GetItemCount() - 1;
	m_cCTRL.SetCurSel(nIndex);
	ResetCTRL(nIndex);

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
	EnableAllCtrl(TRUE);
}

void CFRAMEBar::InsertCTRL( LPCTRL pCTRL, int nIndex)
{
	CCOMPItem *pItem = CItemBase::GetCOMPItem(pCTRL->m_dwCompID);
	CListCtrl *pListCtrl = m_cCTRL.GetListCtrl();

	if( nIndex == -1 )
		nIndex = INT(m_pItem->m_vCTRL.size());

	if(pCTRL->m_strCtrlID.IsEmpty())
		pCTRL->m_strCtrlID = CItemBase::GetNewID(IDS_FMT_NEWCTRL);
	CItemBase::RegLocalID( pCTRL->m_strCtrlID, m_pItem);

	m_pItem->m_vCTRL.insert(
		m_pItem->m_vCTRL.begin() + nIndex,
		pCTRL);

	m_pItem->m_vCOMP.insert(
		m_pItem->m_vCOMP.begin() + nIndex,
		pItem);

	nIndex = pListCtrl->InsertItem( nIndex, pCTRL->m_strCtrlID);
	m_cCTRL.SetCurSel(nIndex);
	ResetCTRL(nIndex);
	m_cCTRL.UpdateColumnWidth();

	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
	EnableAllCtrl(TRUE);
}

void CFRAMEBar::MoveItemPos( int nFrom, int nTo)
{
	if( nFrom == nTo )
		return;

	CListCtrl *pListCtrl = m_cCTRL.GetListCtrl();
	LPCTRL pCTRL = new CTRL();

	for( int i=0; i<TCML_MENU_COUNT; i++)
		pCTRL->m_strMenuID[i] = m_pItem->m_vCTRL[nFrom]->m_strMenuID[i];

	pCTRL->m_strToolTip = m_pItem->m_vCTRL[nFrom]->m_strToolTip;
	pCTRL->m_strCtrlID = m_pItem->m_vCTRL[nFrom]->m_strCtrlID;
	pCTRL->m_strFontID = m_pItem->m_vCTRL[nFrom]->m_strFontID;
	pCTRL->m_strTipID = m_pItem->m_vCTRL[nFrom]->m_strTipID;
	pCTRL->m_strText = m_pItem->m_vCTRL[nFrom]->m_strText;

	pCTRL->m_dwCompID = m_pItem->m_vCTRL[nFrom]->m_dwCompID;
	pCTRL->m_dwColor = m_pItem->m_vCTRL[nFrom]->m_dwColor;
	pCTRL->m_bAlign = m_pItem->m_vCTRL[nFrom]->m_bAlign;
	pCTRL->m_dwFlag = m_pItem->m_vCTRL[nFrom]->m_dwFlag;
	pCTRL->m_nPosX = m_pItem->m_vCTRL[nFrom]->m_nPosX;
	pCTRL->m_nPosY = m_pItem->m_vCTRL[nFrom]->m_nPosY;

	DeleteCTRL(nFrom);
	InsertCTRL( pCTRL, nTo);
}

void CFRAMEBar::OnLvnBegindragListCtrl( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	HGLOBAL hGlobal = ::GlobalAlloc( GMEM_SHARE|GMEM_MOVEABLE, sizeof(GSCDATA));
	COleDataSource source;
	GSLISTITEM vItem;

	CGameStudioView::m_pDragImage = m_cCTRL.GetListCtrl()->CreateDragImage(
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

void CFRAMEBar::OnLvnItemActivateListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

//	DeleteCTRL(pNMIA->iItem);
	*pResult = 0;
}

void CFRAMEBar::OnLvnEndlabeleditListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CListCtrl *pListCtrl = m_cCTRL.GetListCtrl();
	CString strNEW(pDispInfo->item.pszText);

	if( !strNEW.IsEmpty() && strNEW != m_pItem->m_vCTRL[pDispInfo->item.iItem]->m_strCtrlID )
		if(!CItemBase::CheckID(strNEW))
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_INVALID_ID);
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		}
		else if(!m_pItem->CheckDupLocalID( m_pItem->m_vCTRL[pDispInfo->item.iItem]->m_strCtrlID, strNEW))
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_DUP_ATTRID);
			strTitle.LoadString(IDR_MAINFRAME);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		}
		else
		{
			CItemBase::ReleaseLocalID( m_pItem->m_vCTRL[pDispInfo->item.iItem]->m_strCtrlID, m_pItem);
			CItemBase::RegLocalID( strNEW, m_pItem);

			m_pItem->m_vCTRL[pDispInfo->item.iItem]->m_strCtrlID = strNEW;
			pListCtrl->SetItemText( pDispInfo->item.iItem, 0, strNEW);
		}

	*pResult = 0;
}

void CFRAMEBar::OnLvnBeginlabeleditListCtrl(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	CEdit *pEdit = m_cCTRL.GetListCtrl()->GetEditControl();
	pEdit->ModifyStyle( 0, ES_UPPERCASE);
	*pResult = 0;
}

void CFRAMEBar::OnBnClickedButtonTipfont()
{
	int nIndex = m_cCTRL.GetCurSel();

	if( nIndex != -1 )
	{
		CEditFontDlg dlg;

		dlg.m_strSEL = m_pItem->m_vCTRL[nIndex]->m_strTipID;
		if( dlg.DoModal() == IDOK )
		{
			m_pItem->m_vCTRL[nIndex]->m_strTipID = dlg.m_strSEL;
			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CFRAMEBar::OnBnClickedButtonTextfont()
{
	int nIndex = m_cCTRL.GetCurSel();

	if( nIndex != -1 )
	{
		CEditFontDlg dlg;

		dlg.m_strSEL = m_pItem->m_vCTRL[nIndex]->m_strFontID;
		if( dlg.DoModal() == IDOK )
		{
			m_pItem->m_vCTRL[nIndex]->m_strFontID = dlg.m_strSEL;
			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CFRAMEBar::OnBnClickedButtonMenuid()
{
	int nIndex = m_cCTRL.GetCurSel();

	if( nIndex != -1 )
	{
		CEditMenuDlg dlg;

		UpdateData();
		dlg.m_strSEL = m_pItem->m_vCTRL[nIndex]->m_strMenuID[m_nNotify];

		if( dlg.DoModal() == IDOK )
		{
			m_pItem->m_vCTRL[nIndex]->m_strMenuID[m_nNotify] = dlg.m_strSEL;
			m_strMenuID = dlg.m_strSEL;

			UpdateData(FALSE);
			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CFRAMEBar::OnBnClickedRadioHotPoint()
{
	CFRAMEDoc *pDoc = (CFRAMEDoc *) m_pItem->GetMainDocument();
	UpdateData();

	pDoc->m_bHotPoint = m_nHotPoint;
}

void CFRAMEBar::OnBnClickedCheckPasswd()
{
	int nIndex = m_cCTRL.GetCurSel();
	UpdateData();

	if( nIndex != -1 )
		if(m_bPasswd)
			m_pItem->m_vCTRL[nIndex]->m_dwFlag |= TES_PASSWORD;
		else
			m_pItem->m_vCTRL[nIndex]->m_dwFlag &= ~TES_PASSWORD;
}

void CFRAMEBar::OnBnClickedCheckStatebtn()
{
	int nIndex = m_cCTRL.GetCurSel();
	UpdateData();

	if( nIndex != -1 )
		if(m_bState)
			m_pItem->m_vCTRL[nIndex]->m_dwFlag |= TBS_STATE_BUTTON;
		else
			m_pItem->m_vCTRL[nIndex]->m_dwFlag &= ~TBS_STATE_BUTTON;
}

void CFRAMEBar::OnCbnSelchangeComboAlign()
{
	int nIndex = m_cCTRL.GetCurSel();
	UpdateData();

	if( nIndex != -1 )
	{
		m_pItem->m_vCTRL[nIndex]->m_bAlign = m_nAlign;
		m_pItem->GetMainDocument()->UpdateAllViews(NULL);
	}
}

void CFRAMEBar::OnCbnSelchangeComboNotify()
{
	int nIndex = m_cCTRL.GetCurSel();

	if( nIndex != -1 )
	{
		UpdateData();
		ResetCTRL(nIndex);
	}
}

LRESULT CFRAMEBar::OnColorSelected( WPARAM wParam, LPARAM lParam)
{
	int nIndex = m_cCTRL.GetCurSel();

	if( nIndex != -1 )
	{
		m_dwRValue = GetRValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
		m_dwGValue = GetGValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
		m_dwBValue = GetBValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

		m_cColorBox.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
		m_pItem->m_vCTRL[nIndex]->m_dwColor = VALUEARGB(
			m_dwAlpha * 255 / 100,
			m_dwRValue,
			m_dwGValue,
			m_dwBValue);
	}

	UpdateData(FALSE);
	m_pItem->GetMainDocument()->UpdateAllViews(NULL);

	return 0;
}

void CFRAMEBar::OnEnChangeEditAlpha()
{
	if(m_pItem)
	{
		int nIndex = m_cCTRL.GetCurSel();
		UpdateData();

		if( nIndex != -1 )
		{
			m_pItem->m_vCTRL[nIndex]->m_dwColor = VALUEARGB(
				m_dwAlpha * 255 / 100,
				m_dwRValue,
				m_dwGValue,
				m_dwBValue);

			m_pItem->GetMainDocument()->UpdateAllViews(NULL);
		}
	}
}

void CFRAMEBar::OnEnChangeEditRGB()
{
	if(m_pItem)
	{
		int nIndex = m_cCTRL.GetCurSel();
		UpdateData();

		if( nIndex != -1 )
		{
			m_cColorCtrl.SetupColor( RGB(
				m_dwRValue,
				m_dwGValue,
				m_dwBValue), TRUE);
		}
	}
}

void CFRAMEBar::OnBnClickedCheckCustomclr()
{
	int nIndex = m_cCTRL.GetCurSel();
	UpdateData();

	if( nIndex != -1 )
		if(m_bCusColor)
			m_pItem->m_vCTRL[nIndex]->m_dwFlag |= TS_CUSTOM_COLOR;
		else
			m_pItem->m_vCTRL[nIndex]->m_dwFlag &= ~TS_CUSTOM_COLOR;

	EnableAllCtrl(TRUE);
	m_pItem->GetMainDocument()->UpdateAllViews(NULL);
}

void CFRAMEBar::OnBnClickedButtonFindCtrl()
{
	int nIndex = m_cCTRL.GetCurSel();

	if( nIndex != -1 )
	{
		if( m_pItem )
		{
			CMainFrame* pMainWnd = (CMainFrame*)AfxGetApp()->GetMainWnd();
			
			HTREEITEM hItem = NULL;

			if( pMainWnd )
				hItem = pMainWnd->m_pDocComp->FindItemByID( m_pItem->m_vCTRL[nIndex]->m_dwCompID );

            if( hItem != NULL )
			{
				pMainWnd->m_pDocComp->GetTreeCtrl().SelectItem( hItem );
				pMainWnd->SetActiveWorkspace( 8 );
			}
		}
	}
}

void CFRAMEBar::OnBnClickedButtonFindDefimg()
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

void CFRAMEBar::OnBnClickedButtonFindDisimg()
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
