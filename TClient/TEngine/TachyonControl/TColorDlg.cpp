#include "stdafx.h"
#include "Resource.h"
#include "TColorDlg.h"
#include "TPaintManager.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTColorDlg dialog

CTColorDlg::CTColorDlg( COLORREF clrInit,
					    COLORREF clrNew,
						CWnd *pParent) : CTResizableDialog( IDD_TCOLOR_DIALOG, pParent),
						m_clrInit(clrInit),
						m_clrNew(clrNew),
						m_strCaption(_T("")),
						m_bDialogInitColplete(FALSE),
						m_bSyncingValues(FALSE),
						m_wndColorCtrl(CTColorCtrl::MODE_HEXAGON),
						m_wndColorDiff(CTColorCtrl::MODE_DIFF)
{
	//{{AFX_DATA_INIT(CTColorDlg)
	//}}AFX_DATA_INIT

	m_hIcon = AfxGetApp()->LoadIcon(IDD_TCOLOR_DIALOG);
}

void CTColorDlg::DoDataExchange(CDataExchange* pDX)
{
	CTResizableDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTColorDlg)
	DDX_Control(pDX, IDC_SPIN_C, m_SpinC);
	DDX_Control(pDX, IDC_SPIN_M, m_SpinM);
	DDX_Control(pDX, IDC_SPIN_Y, m_SpinY);
	DDX_Control(pDX, IDC_SPIN_S, m_SpinS);
	DDX_Control(pDX, IDC_SPIN_R, m_SpinR);
	DDX_Control(pDX, IDC_SPIN_L, m_SpinL);
	DDX_Control(pDX, IDC_SPIN_H, m_SpinH);
	DDX_Control(pDX, IDC_SPIN_G, m_SpinG);
	DDX_Control(pDX, IDC_SPIN_B, m_SpinB);
	DDX_Control(pDX, IDC_EDIT_S, m_EditS);
	DDX_Control(pDX, IDC_EDIT_R, m_EditR);
	DDX_Control(pDX, IDC_EDIT_L, m_EditL);
	DDX_Control(pDX, IDC_EDIT_H, m_EditH);
	DDX_Control(pDX, IDC_EDIT_G, m_EditG);
	DDX_Control(pDX, IDC_EDIT_B, m_EditB);
	DDX_Control(pDX, IDC_EDIT_C, m_EditC);
	DDX_Control(pDX, IDC_EDIT_M, m_EditM);
	DDX_Control(pDX, IDC_EDIT_Y, m_EditY);
	DDX_Control(pDX, IDC_COMBO_SEL_TYPE, m_ComboType);
	DDX_Control(pDX, IDCANCEL, m_BtnCancel);
	DDX_Control(pDX, IDOK, m_BtnOK);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTColorDlg, CTResizableDialog)
	//{{AFX_MSG_MAP(CTColorDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CONTEXTMENU()
	ON_CBN_SELENDOK(IDC_COMBO_SEL_TYPE, OnSelendokComboSelType)
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_C, SyncFromEditCMY)
	ON_EN_CHANGE(IDC_EDIT_M, SyncFromEditCMY)
	ON_EN_CHANGE(IDC_EDIT_Y, SyncFromEditCMY)
	ON_EN_CHANGE(IDC_EDIT_R, SyncFromEditRGB)
	ON_EN_CHANGE(IDC_EDIT_B, SyncFromEditRGB)
	ON_EN_CHANGE(IDC_EDIT_G, SyncFromEditRGB)
	ON_EN_CHANGE(IDC_EDIT_H, SyncFromEditHSL)
	ON_EN_CHANGE(IDC_EDIT_S, SyncFromEditHSL)
	ON_EN_CHANGE(IDC_EDIT_L, SyncFromEditHSL)
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE( CTColorCtrl::m_nMsgColorChanged, OnColorChanged)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTColorDlg message handlers

BOOL CTColorDlg::OnInitDialog()
{
	CTResizableDialog::OnInitDialog();

	SetIcon( m_hIcon, FALSE);
	SetIcon( m_hIcon, TRUE);

	if(!m_strCaption.IsEmpty())
	{
		int nCP = m_strCaption.ReverseFind(_T('\n'));

		if( nCP > 0 )
		{
			int nLen = m_strCaption.GetLength();
			int nCPR = nLen - nCP - 1;

			m_strCaption = m_strCaption.Right(nCPR);
		}

		if(!m_strCaption.IsEmpty())
			SetWindowText(m_strCaption);
	}

	m_wndColorCtrl.m_selClrs.m_clrCurrent = m_clrInit;
	m_wndColorCtrl.m_selClrs.m_clrPrev = m_clrInit;
	m_wndColorCtrl.m_selClrs.m_clrNew = m_clrNew;

	m_wndColorDiff.m_selClrs.m_clrCurrent = m_clrInit;
	m_wndColorDiff.m_selClrs.m_clrPrev = m_clrInit;
	m_wndColorDiff.m_selClrs.m_clrNew = m_clrInit;

	m_ComboType.SetCurSel(0);
	AddAnchor( IDC_COMBO_SEL_TYPE, __RDA_LT, __RDA_RT);
	AddAnchor( IDC_STATIC_HEX_FULL, __RDA_LT, __RDA_RB);
	AddAnchor( IDC_SPIN_R, __RDA_RT);
	AddAnchor( IDC_STATIC_R, __RDA_RT);
	AddAnchor( IDC_EDIT_R, __RDA_RT);
	AddAnchor( IDC_SPIN_G, __RDA_RT);
	AddAnchor( IDC_STATIC_G, __RDA_RT);
	AddAnchor( IDC_EDIT_G, __RDA_RT);
	AddAnchor( IDC_SPIN_B, __RDA_RT);
	AddAnchor( IDC_STATIC_B, __RDA_RT);
	AddAnchor( IDC_EDIT_B, __RDA_RT);
	AddAnchor( IDC_SPIN_H, __RDA_RT);
	AddAnchor( IDC_STATIC_H, __RDA_RT);
	AddAnchor( IDC_EDIT_H, __RDA_RT);
	AddAnchor( IDC_SPIN_S, __RDA_RT);
	AddAnchor( IDC_STATIC_S, __RDA_RT);
	AddAnchor( IDC_EDIT_S, __RDA_RT);
	AddAnchor( IDC_SPIN_L, __RDA_RT);
	AddAnchor( IDC_STATIC_L, __RDA_RT);
	AddAnchor( IDC_EDIT_L, __RDA_RT);
	AddAnchor( IDC_SPIN_C, __RDA_RT);
	AddAnchor( IDC_STATIC_C, __RDA_RT);
	AddAnchor( IDC_EDIT_C, __RDA_RT);
	AddAnchor( IDC_SPIN_M, __RDA_RT);
	AddAnchor( IDC_STATIC_M, __RDA_RT);
	AddAnchor( IDC_EDIT_M, __RDA_RT);
	AddAnchor( IDC_SPIN_Y, __RDA_RT);
	AddAnchor( IDC_STATIC_Y, __RDA_RT);
	AddAnchor( IDC_EDIT_Y, __RDA_RT);
	AddAnchor( IDC_STATIC_TXT_NEW, __RDA_RT);
	AddAnchor( IDC_STATIC_TXT_CURRENT, __RDA_RT);
	AddAnchor( IDC_STATIC_CMP, __RDA_RT);
	AddAnchor( IDOK, __RDA_LB);
	AddAnchor( IDCANCEL, __RDA_LB);
	ShowSizeGrip(TRUE);

	m_wndColorCtrl.SubclassDlgItem(
		IDC_STATIC_HEX_FULL,
		this);

	m_wndColorDiff.SubclassDlgItem(
		IDC_STATIC_CMP,
		this);

	m_SpinR.SetRange( 0, 255);
	m_SpinG.SetRange( 0, 255);
	m_SpinB.SetRange( 0, 255);
	m_SpinH.SetRange( 0, 255);
	m_SpinL.SetRange( 0, 255);
	m_SpinS.SetRange( 0, 255);
	m_SpinC.SetRange( 0, 255);
	m_SpinM.SetRange( 0, 255);
	m_SpinY.SetRange( 0, 255);

	m_bDialogInitColplete = TRUE;
	OnColorChanged(
		(WPARAM) m_wndColorCtrl.GetSafeHwnd(),
		(LPARAM) &m_wndColorCtrl.m_selClrs);

	m_wndColorDiff.SetupColor( m_clrInit, FALSE);
	return TRUE;
}

void CTColorDlg::OnPaint()
{
	if(IsIconic())
	{
		CPaintDC dc(this);

		SendMessage( WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
		int nIconX = GetSystemMetrics(SM_CXICON);
		int nIconY = GetSystemMetrics(SM_CYICON);

		CRect rect;
		GetClientRect(&rect);

		int x = (rect.Width() - nIconX + 1) / 2;
		int y = (rect.Height() - nIconY + 1) / 2;
		dc.DrawIcon( x, y, m_hIcon);
	}
	else
		CTResizableDialog::OnPaint();
}

HCURSOR CTColorDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTColorDlg::OnContextMenu( CWnd* pWnd, CPoint point)
{
}

void CTColorDlg::OnSelendokComboSelType()
{
	if(!m_bDialogInitColplete)
		return;

	m_wndColorCtrl = (CTColorCtrl::e_mode) m_ComboType.GetCurSel();
}

void CTColorDlg::OnSize( UINT nType, int cx, int cy)
{
	CTResizableDialog::OnSize( nType, cx, cy);
	Invalidate();
	UpdateWindow();
}

LRESULT CTColorDlg::OnColorChanged( WPARAM wParam, LPARAM lParam)
{
	if(m_bSyncingValues)
		return 0;

	m_bSyncingValues = TRUE;
	CTColorCtrl::selection_colors *pClrs = (CTColorCtrl::selection_colors *) lParam;

	SyncToEditRGB(
		pClrs->m_clrNew,
		TRUE,
		TRUE,
		TRUE);

	m_bSyncingValues = FALSE;
	return 0;
}

void CTColorDlg::SyncToEditRGB( COLORREF clr,
							    BOOL bSyncRGB,
								BOOL bSyncCMY,
								BOOL bSyncHLS)
{
	if(bSyncRGB)
	{
		CString strR;
		CString strG;
		CString strB;

		strR.Format( _T("%d"), GetRValue(clr));
		strG.Format( _T("%d"), GetGValue(clr));
		strB.Format( _T("%d"), GetBValue(clr));
		m_EditR.SetWindowText(strR);
		m_EditG.SetWindowText(strG);
		m_EditB.SetWindowText(strB);
	}

	if(bSyncCMY)
	{
		COLORREF clrCMY = CTPaintManager::RGBtoCMY(clr);
		CString strC;
		CString strM;
		CString strY;

		strC.Format( _T("%d"), GetRValue(clrCMY));
		strM.Format( _T("%d"), GetGValue(clrCMY));
		strY.Format( _T("%d"), GetBValue(clrCMY));
		m_EditC.SetWindowText(strC);
		m_EditM.SetWindowText(strM);
		m_EditY.SetWindowText(strY);
	}

	if(bSyncHLS)
	{
		CString strH;
		CString strS;
		CString strL;

		double fH=0.0f;
		double fS=0.0f;
		double fL=0.0f;

		CTPaintManager::RGBtoHSL( clr, &fH, &fS, &fL);
		int nH;
		int nS;
		int nL;

		nH = min( 255, (UINT) (0.5f + fH * 255.0f));
		nS = min( 255, (UINT) (0.5f + fS * 255.0f));
		nL = min( 255, (UINT) (0.5f + fL * 255.0f));

		strH.Format( _T("%d"), nH);
		strS.Format( _T("%d"), nS);
		strL.Format( _T("%d"), nL);

		m_EditH.SetWindowText(strH);
		m_EditS.SetWindowText(strS);
		m_EditL.SetWindowText(strL);
	}

	if( m_wndColorCtrl.m_selClrs.m_clrNew != clr )
		m_wndColorCtrl.SetupColor( clr, FALSE);

	if( m_wndColorDiff.m_selClrs.m_clrNew != clr )
		m_wndColorDiff.SetupColor( clr, FALSE);

	m_clrNew = m_wndColorCtrl.m_selClrs.m_clrNew;
}

void CTColorDlg::SyncFromEditRGB()
{
	if( !m_bDialogInitColplete || m_bSyncingValues )
		return;

	CString strMsgInvalidRange(_T("Value must be in range 0..255"));
	CString strValue;

	m_bSyncingValues = TRUE;
	m_EditR.GetWindowText(strValue);

	int nR = _ttol(strValue);
	if( nR < 0 || nR > 255 )
	{
		AfxMessageBox(strMsgInvalidRange);
		m_EditR.SetFocus();
		m_bSyncingValues = FALSE;

		return;
	}

	m_EditG.GetWindowText(strValue);
	int nG = _ttol(strValue);
	if( nG < 0 || nG > 255 )
	{
		AfxMessageBox(strMsgInvalidRange);
		m_EditG.SetFocus();
		m_bSyncingValues = FALSE;

		return;
	}

	m_EditB.GetWindowText(strValue);
	int nB = _ttol(strValue);
	if( nB < 0 || nB > 255 )
	{
		AfxMessageBox(strMsgInvalidRange);
		m_EditB.SetFocus();
		m_bSyncingValues = FALSE;

		return;
	}

	COLORREF clr = RGB( nR, nG, nB);
	SyncToEditRGB( clr, FALSE, TRUE, TRUE);
	m_bSyncingValues = FALSE;
}

void CTColorDlg::SyncFromEditCMY()
{
	if( !m_bDialogInitColplete || m_bSyncingValues )
		return;

	CString strMsgInvalidRange(_T("Value must be in range 0..255"));
	CString strValue;

	m_bSyncingValues = TRUE;
	m_EditC.GetWindowText(strValue);

	int nC = _ttol(strValue);
	if( nC < 0 || nC > 255 )
	{
		AfxMessageBox(strMsgInvalidRange);
		m_EditC.SetFocus();
		m_bSyncingValues = FALSE;

		return;
	}

	m_EditM.GetWindowText(strValue);
	int nM = _ttol(strValue);
	if( nM < 0 || nM > 255 )
	{
		AfxMessageBox(strMsgInvalidRange);
		m_EditM.SetFocus();
		m_bSyncingValues = FALSE;

		return;
	}

	m_EditY.GetWindowText(strValue);
	int nY = _ttol(strValue);
	if( nY < 0 || nY > 255 )
	{
		AfxMessageBox(strMsgInvalidRange);
		m_EditY.SetFocus();
		m_bSyncingValues = FALSE;

		return;
	}

	COLORREF clr = CTPaintManager::CMYtoRGB(RGB( nC, nM, nY));
	SyncToEditRGB( clr, TRUE, FALSE, TRUE);
	m_bSyncingValues = FALSE;
}

void CTColorDlg::SyncFromEditHSL()
{
	if( !m_bDialogInitColplete || m_bSyncingValues )
		return;

	CString strMsgInvalidRange(_T("Value must be in range 0..255"));
	CString strValue;

	m_bSyncingValues = TRUE;
	m_EditH.GetWindowText(strValue);

	int nH = _ttol(strValue);
	if( nH < 0 || nH > 255 )
	{
		AfxMessageBox(strMsgInvalidRange);
		m_EditH.SetFocus();
		m_bSyncingValues = FALSE;

		return;
	}

	m_EditS.GetWindowText(strValue);
	int nS = _ttol(strValue);
	if( nS < 0 || nS > 255 )
	{
		AfxMessageBox(strMsgInvalidRange);
		m_EditS.SetFocus();
		m_bSyncingValues = FALSE;

		return;
	}

	m_EditL.GetWindowText(strValue);
	int nL = _ttol(strValue);
	if( nL < 0 || nL > 255 )
	{
		AfxMessageBox(strMsgInvalidRange);
		m_EditL.SetFocus();
		m_bSyncingValues = FALSE;

		return;
	}

	COLORREF clr = CTPaintManager::HLStoRGB(
		min( 1.0f, (double) (0.5f + nH) / 255.0f),
		min( 1.0f, (double) (0.5f + nL) / 255.0f),
		min( 1.0f, (double) (0.5f + nS) / 255.0f));

	SyncToEditRGB( clr, TRUE, TRUE, FALSE);
	m_bSyncingValues = FALSE;
}
