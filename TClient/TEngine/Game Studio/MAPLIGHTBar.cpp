// MAPLIGHTBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "MAPDoc.h"
#include "MAPLIGHTBar.h"
#include ".\maplightbar.h"


// CMAPLIGHTBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMAPLIGHTBar, CBarContent)
CMAPLIGHTBar::CMAPLIGHTBar(CWnd* pParent /*=NULL*/)
: CBarContent(CMAPLIGHTBar::IDD, pParent)
, m_nMode(0)
, m_nType(0)
, m_strSRC(_T(""))
, m_dwTextureID(0)
, m_dwRValueA(0)
, m_dwRValueD(0)
, m_dwGValueA(0)
, m_dwGValueD(0)
, m_dwBValueA(0)
, m_dwBValueD(0)
, m_bUseDL(TRUE)
, m_bUseLM(TRUE)
, m_strPosX(_T("0.00"))
, m_strPosY(_T("0.00"))
, m_strPosZ(_T("0.00"))
, m_strDirX(_T("1.00"))
, m_strDirY(_T("0.00"))
, m_strDirZ(_T("0.00"))
, m_strRange(_T("1.00"))
, m_strROT(_T("0.00"))
{
	m_pItem = NULL;
}

CMAPLIGHTBar::~CMAPLIGHTBar()
{
}

void CMAPLIGHTBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_INSERT, m_cInsertMode);
	DDX_Radio(pDX, IDC_RADIO_INSERT, m_nMode);
	DDX_Control(pDX, IDC_RADIO_EDIT, m_cEditMode);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_cType);
	DDX_CBIndex(pDX, IDC_COMBO_TYPE, m_nType);
	DDX_Control(pDX, IDC_EDIT_LIGHTSRC, m_cSRC);
	DDX_Text(pDX, IDC_EDIT_LIGHTSRC, m_strSRC);
	DDX_Control(pDX, IDC_BUTTON_DELLIGHTSRC, m_cDELSRC);
	DDX_Control(pDX, IDC_STATIC_AMBIENT, m_cColorBoxA);
	DDX_Control(pDX, IDC_STATIC_AMBIENTCTRL, m_cControlBoxA);
	DDX_Control(pDX, IDC_STATIC_DIFFUSE, m_cColorBoxD);
	DDX_Control(pDX, IDC_STATIC_DIFFUSECTRL, m_cControlBoxD);
	DDX_Control(pDX, IDC_EDIT_RVALUEA, m_cRValueA);
	DDX_Control(pDX, IDC_EDIT_GVALUEA, m_cGValueA);
	DDX_Control(pDX, IDC_EDIT_BVALUEA, m_cBValueA);
	DDX_Control(pDX, IDC_EDIT_RVALUED, m_cRValueD);
	DDX_Control(pDX, IDC_EDIT_GVALUED, m_cGValueD);
	DDX_Control(pDX, IDC_EDIT_BVALUED, m_cBValueD);
	DDX_Control(pDX, IDC_SPIN_RVALUEA, m_spinRValueA);
	DDX_Control(pDX, IDC_SPIN_GVALUEA, m_spinGValueA);
	DDX_Control(pDX, IDC_SPIN_BVALUEA, m_spinBValueA);
	DDX_Control(pDX, IDC_SPIN_RVALUED, m_spinRValueD);
	DDX_Control(pDX, IDC_SPIN_GVALUED, m_spinGValueD);
	DDX_Control(pDX, IDC_SPIN_BVALUED, m_spinBValueD);
	DDX_Text(pDX, IDC_EDIT_RVALUEA, m_dwRValueA);
	DDX_Text(pDX, IDC_EDIT_RVALUED, m_dwRValueD);
	DDX_Text(pDX, IDC_EDIT_GVALUEA, m_dwGValueA);
	DDX_Text(pDX, IDC_EDIT_GVALUED, m_dwGValueD);
	DDX_Text(pDX, IDC_EDIT_BVALUEA, m_dwBValueA);
	DDX_Text(pDX, IDC_EDIT_BVALUED, m_dwBValueD);
	DDX_Control(pDX, IDC_CHECK_USE_DYNAMIC, m_cUseDL);
	DDX_Check(pDX, IDC_CHECK_USE_DYNAMIC, m_bUseDL);
	DDX_Control(pDX, IDC_CHECK_USE_LIGHTMAP, m_cUseLM);
	DDX_Check(pDX, IDC_CHECK_USE_LIGHTMAP, m_bUseLM);
	DDX_Control(pDX, IDC_EDIT_POSX, m_cPosX);
	DDX_Control(pDX, IDC_EDIT_POSY, m_cPosY);
	DDX_Control(pDX, IDC_EDIT_POSZ, m_cPosZ);
	DDX_Text(pDX, IDC_EDIT_POSX, m_strPosX);
	DDX_Text(pDX, IDC_EDIT_POSY, m_strPosY);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_strPosZ);
	DDX_Control(pDX, IDC_EDIT_DIRX, m_cDirX);
	DDX_Control(pDX, IDC_EDIT_DIRY, m_cDirY);
	DDX_Control(pDX, IDC_EDIT_DIRZ, m_cDirZ);
	DDX_Text(pDX, IDC_EDIT_DIRX, m_strDirX);
	DDX_Text(pDX, IDC_EDIT_DIRY, m_strDirY);
	DDX_Text(pDX, IDC_EDIT_DIRZ, m_strDirZ);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_EDIT_RANGE, m_cRange);
	DDX_Text(pDX, IDC_EDIT_RANGE, m_strRange);
	DDX_Control(pDX, IDC_SPIN_RANGE, m_spinRange);
	DDX_Control(pDX, IDC_SPIN_POSX, m_spinPosX);
	DDX_Control(pDX, IDC_SPIN_POSY, m_spinPosY);
	DDX_Control(pDX, IDC_SPIN_POSZ, m_spinPosZ);
	DDX_Control(pDX, IDC_SPIN_DIRX, m_spinDirX);
	DDX_Control(pDX, IDC_SPIN_DIRY, m_spinDirY);
	DDX_Control(pDX, IDC_SPIN_DIRZ, m_spinDirZ);
	DDX_Control(pDX, IDC_SPIN_ROT, m_spinROT);
	DDX_Control(pDX, IDC_EDIT_ROT, m_cROT);
	DDX_Text(pDX, IDC_EDIT_ROT, m_strROT);
}


BEGIN_MESSAGE_MAP(CMAPLIGHTBar, CBarContent)
	ON_WM_DESTROY()
	ON_WM_VSCROLL()
	ON_MESSAGE( WM_COLOR_SELECTED, OnColorSelected)
	ON_BN_CLICKED(IDC_BUTTON_BACK, OnBnClickedButtonBack)
	ON_EN_CHANGE(IDC_EDIT_RVALUEA, OnEnChangeEditRGBA)
	ON_EN_CHANGE(IDC_EDIT_GVALUEA, OnEnChangeEditRGBA)
	ON_EN_CHANGE(IDC_EDIT_BVALUEA, OnEnChangeEditRGBA)
	ON_EN_CHANGE(IDC_EDIT_RVALUED, OnEnChangeEditRGBD)
	ON_EN_CHANGE(IDC_EDIT_GVALUED, OnEnChangeEditRGBD)
	ON_EN_CHANGE(IDC_EDIT_BVALUED, OnEnChangeEditRGBD)
	ON_BN_CLICKED(IDC_RADIO_EDIT, OnBnClickedRadioEdit)
	ON_BN_CLICKED(IDC_RADIO_INSERT, OnBnClickedRadioInsert)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnCbnSelchangeComboType)
	ON_BN_CLICKED(IDC_BUTTON_DELLIGHTSRC, OnBnClickedButtonDellightsrc)
	ON_BN_CLICKED(IDC_CHECK_USE_LIGHTMAP, OnBnClickedCheckUseLightmap)
	ON_BN_CLICKED(IDC_CHECK_USE_DYNAMIC, OnBnClickedCheckUseDynamic)
	ON_EN_CHANGE(IDC_EDIT_POSX, OnEnChangeEditPOS)
	ON_EN_CHANGE(IDC_EDIT_POSY, OnEnChangeEditPOS)
	ON_EN_CHANGE(IDC_EDIT_POSZ, OnEnChangeEditPOS)
	ON_EN_CHANGE(IDC_EDIT_DIRX, OnEnChangeEditDIR)
	ON_EN_CHANGE(IDC_EDIT_DIRY, OnEnChangeEditDIR)
	ON_EN_CHANGE(IDC_EDIT_DIRZ, OnEnChangeEditDIR)
	ON_EN_CHANGE(IDC_EDIT_RANGE, OnEnChangeEditRange)
	ON_EN_CHANGE(IDC_EDIT_ROT, OnEnChangeEditROT)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
END_MESSAGE_MAP()


// CMAPLIGHTBar 메시지 처리기입니다.

void CMAPLIGHTBar::SetItem( CMAPItem *pItem)
{
	m_pItem = pItem;

	m_cColorCtrlA.SetupColor( 0x00000000, TRUE);
	m_cColorCtrlD.SetupColor( 0x00000000, TRUE);

	UpdateItem();
}

void CMAPLIGHTBar::UpdateItem()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	if(m_nMode)
	{
		D3DXVECTOR3 vPOS = pDoc->GetSELLIGHTPOS();

		m_strPosX.Format( "%.2f", vPOS.x);
		m_strPosY.Format( "%.2f", vPOS.y);
		m_strPosZ.Format( "%.2f", vPOS.z);
		m_strSRC.LoadString(IDS_NOTUSE);
		m_dwTextureID = 0;

		if( INT(pDoc->m_vSELLIGHT.size()) == 1 )
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[0]);

			if( finder != m_pItem->m_mapLIGHT.end() )
			{
				m_dwRValueA = GETVALUE_R((*finder).second->m_dwAmbient);
				m_dwGValueA = GETVALUE_G((*finder).second->m_dwAmbient);
				m_dwBValueA = GETVALUE_B((*finder).second->m_dwAmbient);
				m_dwRValueD = GETVALUE_R((*finder).second->m_dwDiffuse);
				m_dwGValueD = GETVALUE_G((*finder).second->m_dwDiffuse);
				m_dwBValueD = GETVALUE_B((*finder).second->m_dwDiffuse);
				m_nType = (*finder).second->m_bType;

				m_strRange.Format( "%.2f", (*finder).second->m_fRange);
				m_cColorCtrlA.SetupColor( RGB(
					m_dwRValueA,
					m_dwGValueA,
					m_dwBValueA), FALSE);

				m_cColorBoxA.SetColor( RGB(
					m_dwRValueA,
					m_dwGValueA,
					m_dwBValueA));

				m_cColorCtrlD.SetupColor( RGB(
					m_dwRValueD,
					m_dwGValueD,
					m_dwBValueD), FALSE);

				m_cColorBoxD.SetColor( RGB(
					m_dwRValueD,
					m_dwGValueD,
					m_dwBValueD));

				switch(m_nType)
				{
				case MLT_DIRECTION	:
					{
						m_strDirX.Format( "%.2f", (*finder).second->m_vDIR.x);
						m_strDirY.Format( "%.2f", (*finder).second->m_vDIR.y);
						m_strDirZ.Format( "%.2f", (*finder).second->m_vDIR.z);
					}

					break;

				case MLT_POINT		:
					{
						CTextureItem *pItem = CItemBase::GetTextureItem((*finder).second->m_dwTextureID);

						if(!pItem)
						{
							(*finder).second->m_dwTextureID = 0;
							m_dwTextureID = 0;

							m_strSRC.LoadString(IDS_NOTUSE);
						}
						else
						{
							m_dwTextureID = pItem->m_dwID;
							m_strSRC = pItem->m_strName;
						}

						m_strROT.Format( "%.2f", (*finder).second->m_fROT * 180.0f / D3DX_PI);
						m_bUseDL = (*finder).second->m_bUseDL;
						m_bUseLM = (*finder).second->m_bUseLM;
					}

					break;
				}
			}
		}
	}
	else
	{
		CTextureItem *pItem = CItemBase::GetTextureItem(m_dwTextureID);

		if(pItem)
		{
			m_dwTextureID = pItem->m_dwID;
			m_strSRC = pItem->m_strName;
		}
		else
		{
			m_strSRC.LoadString(IDS_NOTUSE);
			m_dwTextureID = 0;
		}
	}

	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
}

void CMAPLIGHTBar::EnableAllCtrl( BOOL bEnable)
{
	m_cInsertMode.EnableWindow(bEnable);
	m_cEditMode.EnableWindow(bEnable);

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
		BOOL bSINGLE = INT(pDoc->m_vSELLIGHT.size()) == 1;
		bEnable = bEnable && !pDoc->m_vSELLIGHT.empty();

		m_cDELSRC.EnableWindow( bEnable && bSINGLE && m_nType == MLT_POINT && m_dwTextureID != 0 );
		m_spinROT.EnableWindow( bEnable && bSINGLE && m_nType == MLT_POINT );
		m_cUseDL.EnableWindow( bEnable && bSINGLE && m_nType == MLT_POINT );
		m_cUseLM.EnableWindow( bEnable && bSINGLE && m_nType == MLT_POINT );
		m_cSRC.EnableWindow( bEnable && bSINGLE && m_nType == MLT_POINT );
		m_cROT.EnableWindow( bEnable && bSINGLE && m_nType == MLT_POINT );

		m_cColorCtrlD.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_spinRValueD.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_spinGValueD.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_spinBValueD.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_cRValueD.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_cGValueD.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_cBValueD.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_spinDirX.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_spinDirY.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_spinDirZ.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_cDirX.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_cDirY.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );
		m_cDirZ.EnableWindow( bEnable && bSINGLE && m_nType == MLT_DIRECTION );

		m_cColorCtrlA.EnableWindow( bEnable && bSINGLE );
		m_spinRValueA.EnableWindow( bEnable && bSINGLE );
		m_spinGValueA.EnableWindow( bEnable && bSINGLE );
		m_spinBValueA.EnableWindow( bEnable && bSINGLE );
		m_spinRange.EnableWindow( bEnable && bSINGLE );
		m_cRValueA.EnableWindow( bEnable && bSINGLE );
		m_cGValueA.EnableWindow( bEnable && bSINGLE );
		m_cBValueA.EnableWindow( bEnable && bSINGLE );
		m_cRange.EnableWindow( bEnable && bSINGLE );
		m_cType.EnableWindow( bEnable && bSINGLE );

		m_spinPosX.EnableWindow(bEnable);
		m_spinPosY.EnableWindow(bEnable);
		m_spinPosZ.EnableWindow(bEnable);

		m_cDelete.EnableWindow(bEnable);
		m_cPosX.EnableWindow(bEnable);
		m_cPosY.EnableWindow(bEnable);
		m_cPosZ.EnableWindow(bEnable);
	}
	else
	{
		m_spinDirX.EnableWindow( bEnable && m_nType == MLT_DIRECTION );
		m_spinDirY.EnableWindow( bEnable && m_nType == MLT_DIRECTION );
		m_spinDirZ.EnableWindow( bEnable && m_nType == MLT_DIRECTION );
		m_cDirX.EnableWindow( bEnable && m_nType == MLT_DIRECTION );
		m_cDirY.EnableWindow( bEnable && m_nType == MLT_DIRECTION );
		m_cDirZ.EnableWindow( bEnable && m_nType == MLT_DIRECTION );

		m_cDELSRC.EnableWindow( bEnable && m_nType == MLT_POINT && m_dwTextureID != 0 );
		m_spinROT.EnableWindow( bEnable && m_nType == MLT_POINT );
		m_cUseDL.EnableWindow( bEnable && m_nType == MLT_POINT );
		m_cUseLM.EnableWindow( bEnable && m_nType == MLT_POINT );
		m_cROT.EnableWindow( bEnable && m_nType == MLT_POINT );
		m_cSRC.EnableWindow( bEnable && m_nType == MLT_POINT );

		m_spinRValueA.EnableWindow(bEnable);
		m_spinGValueA.EnableWindow(bEnable);
		m_spinBValueA.EnableWindow(bEnable);
		m_spinRValueD.EnableWindow(bEnable);
		m_spinGValueD.EnableWindow(bEnable);
		m_spinBValueD.EnableWindow(bEnable);
		m_spinRange.EnableWindow(bEnable);
		m_cType.EnableWindow(bEnable);

		m_cColorCtrlA.EnableWindow(bEnable);
		m_cColorCtrlD.EnableWindow(bEnable);
		m_cRValueA.EnableWindow(bEnable);
		m_cGValueA.EnableWindow(bEnable);
		m_cBValueA.EnableWindow(bEnable);
		m_cRValueD.EnableWindow(bEnable);
		m_cGValueD.EnableWindow(bEnable);
		m_cBValueD.EnableWindow(bEnable);
		m_cRange.EnableWindow(bEnable);

		m_spinPosX.EnableWindow(FALSE);
		m_spinPosY.EnableWindow(FALSE);
		m_spinPosZ.EnableWindow(FALSE);
		m_cDelete.EnableWindow(FALSE);
		m_cPosX.EnableWindow(FALSE);
		m_cPosY.EnableWindow(FALSE);
		m_cPosZ.EnableWindow(FALSE);
	}
}

void CMAPLIGHTBar::OnBnClickedButtonBack()
{
	OnBack();
}

void CMAPLIGHTBar::OnBack()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	CFrameWnd *pFrame = GetParentFrame();

	pDoc->ClearSELLIGHT();
	pDoc->m_bEditLIGHT = FALSE;

	pFrame->SendMessage( WM_COMMAND, ID_MAP_BACK);
	pDoc->RedrawAllView();
}

BOOL CMAPLIGHTBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEL);
	m_cDELSRC.SetIcon(hIcon);

	m_cControlBoxA.GetClientRect(&rect);
	m_cControlBoxA.ClientToScreen(&rect);
	ScreenToClient(&rect);
	rect.DeflateRect( 1, 1);

	if(!m_cColorCtrlA.Create(
		NULL, NULL,
		WS_CHILD|WS_VISIBLE,
		rect, this, 0))
		return FALSE;
	m_cColorCtrlA = CTColorCtrl::MODE_HSL_PANEL;

	m_cControlBoxD.GetClientRect(&rect);
	m_cControlBoxD.ClientToScreen(&rect);
	ScreenToClient(&rect);
	rect.DeflateRect( 1, 1);

	if(!m_cColorCtrlD.Create(
		NULL, NULL,
		WS_CHILD|WS_VISIBLE,
		rect, this, 0))
		return FALSE;
	m_cColorCtrlD = CTColorCtrl::MODE_HSL_PANEL;

	m_cSRC.m_DropTarget.Register(&m_cSRC);
	m_cSRC.m_pDragEnter = OnDragOver;
	m_cSRC.m_pDragOver = OnDragOver;
	m_cSRC.m_pDrop = OnDrop;
	m_cSRC.m_pLeave = OnDragLeave;

	m_spinRange.SetRange( -100, 100);
	m_spinPosX.SetRange( -100, 100);
	m_spinPosY.SetRange( -100, 100);
	m_spinPosZ.SetRange( -100, 100);
	m_spinDirX.SetRange( -100, 100);
	m_spinDirY.SetRange( -100, 100);
	m_spinDirZ.SetRange( -100, 100);
	m_spinROT.SetRange( -100, 100);

	m_spinRValueA.SetRange( 0, 255);
	m_spinGValueA.SetRange( 0, 255);
	m_spinBValueA.SetRange( 0, 255);
	m_spinRValueD.SetRange( 0, 255);
	m_spinGValueD.SetRange( 0, 255);
	m_spinBValueD.SetRange( 0, 255);

	return TRUE;
}

LRESULT CMAPLIGHTBar::OnColorSelected( WPARAM wParam, LPARAM lParam)
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	if( (HWND) wParam == m_cColorCtrlA.GetSafeHwnd() )
	{
		m_dwRValueA = GetRValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
		m_dwGValueA = GetGValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
		m_dwBValueA = GetBValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

		m_cColorBoxA.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
	}

	if( (HWND) wParam == m_cColorCtrlD.GetSafeHwnd() )
	{
		m_dwRValueD = GetRValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
		m_dwGValueD = GetGValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
		m_dwBValueD = GetBValue(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);

		m_cColorBoxD.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
	}
	UpdateData(FALSE);

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		for( int i=0; i<INT(pDoc->m_vSELLIGHT.size()); i++)
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[i]);

			(*finder).second->m_dwAmbient = VALUEARGB(
				0xFF,
				m_dwRValueA,
				m_dwGValueA,
				m_dwBValueA);

			(*finder).second->m_dwDiffuse = VALUEARGB(
				0xFF,
				m_dwRValueD,
				m_dwGValueD,
				m_dwBValueD);
		}

		pDoc->UpdateLIGHT();
	}
	else if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();

	return 0;
}

void CMAPLIGHTBar::OnEnChangeEditRGBA()
{
	if(m_pItem)
	{
		UpdateData();
		m_cColorCtrlA.SetupColor( RGB(
			m_dwRValueA,
			m_dwGValueA,
			m_dwBValueA), TRUE);
	}
}

void CMAPLIGHTBar::OnEnChangeEditRGBD()
{
	if(m_pItem)
	{
		UpdateData();
		m_cColorCtrlD.SetupColor( RGB(
			m_dwRValueD,
			m_dwGValueD,
			m_dwBValueD), TRUE);
	}
}

void CMAPLIGHTBar::OnBnClickedRadioEdit()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	pDoc->ClearSELLIGHT();
	UpdateData();
	UpdateItem();

	pDoc->m_bInsertLIGHT = m_nMode == 0;
	pDoc->RedrawAllView();
}

void CMAPLIGHTBar::OnBnClickedRadioInsert()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	pDoc->ClearSELLIGHT();
	UpdateData();
	UpdateItem();

	pDoc->m_bInsertLIGHT = m_nMode == 0;
	pDoc->RedrawAllView();
}

void CMAPLIGHTBar::OnDestroy()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	CBarContent::OnDestroy();
	pDoc->ClearSELLIGHT();
}

void CMAPLIGHTBar::OnCbnSelchangeComboType()
{
	UpdateData();

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		for( int i=0; i<INT(pDoc->m_vSELLIGHT.size()); i++)
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[i]);
			(*finder).second->m_bType = m_nType;
		}

		pDoc->UpdateLIGHT();
	}

	EnableAllCtrl(TRUE);
}

void CMAPLIGHTBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinPosX )
	{
		FLOAT fPosX = (FLOAT) atof(m_strPosX);

		fPosX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinPosX.SetPos(0);

		m_strPosX.Format( "%.2f", fPosX);
		UpdateData(FALSE);
		OnEnChangeEditPOS();
	}

	if( pScrollBar == (CScrollBar *) &m_spinPosY )
	{
		FLOAT fPosY = (FLOAT) atof(m_strPosY);

		fPosY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinPosY.SetPos(0);

		m_strPosY.Format( "%.2f", fPosY);
		UpdateData(FALSE);
		OnEnChangeEditPOS();
	}

	if( pScrollBar == (CScrollBar *) &m_spinPosZ )
	{
		FLOAT fPosZ = (FLOAT) atof(m_strPosZ);

		fPosZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinPosZ.SetPos(0);

		m_strPosZ.Format( "%.2f", fPosZ);
		UpdateData(FALSE);
		OnEnChangeEditPOS();
	}

	if( pScrollBar == (CScrollBar *) &m_spinDirX )
	{
		FLOAT fDirX = (FLOAT) atof(m_strDirX);

		fDirX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinDirX.SetPos(0);

		m_strDirX.Format( "%.2f", fDirX);
		UpdateData(FALSE);
		OnEnChangeEditDIR();
	}

	if( pScrollBar == (CScrollBar *) &m_spinDirY )
	{
		FLOAT fDirY = (FLOAT) atof(m_strDirY);

		fDirY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinDirY.SetPos(0);

		m_strDirY.Format( "%.2f", fDirY);
		UpdateData(FALSE);
		OnEnChangeEditDIR();
	}

	if( pScrollBar == (CScrollBar *) &m_spinDirZ )
	{
		FLOAT fDirZ = (FLOAT) atof(m_strDirZ);

		fDirZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinDirZ.SetPos(0);

		m_strDirZ.Format( "%.2f", fDirZ);
		UpdateData(FALSE);
		OnEnChangeEditDIR();
	}

	if( pScrollBar == (CScrollBar *) &m_spinRange )
	{
		FLOAT fRange = (FLOAT) atof(m_strRange);

		fRange += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinRange.SetPos(0);

		m_strRange.Format( "%.2f", fRange);
		UpdateData(FALSE);
		OnEnChangeEditRange();
	}

	if( pScrollBar == (CScrollBar *) &m_spinROT )
	{
		FLOAT fROT = (FLOAT) atof(m_strROT);

		fROT += (FLOAT) INT(nPos);
		m_spinROT.SetPos(0);

		m_strROT.Format( "%.2f", fROT);
		UpdateData(FALSE);
		OnEnChangeEditROT();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CMAPLIGHTBar::OnBnClickedCheckUseDynamic()
{
	UpdateData();

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		for( int i=0; i<INT(pDoc->m_vSELLIGHT.size()); i++)
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[i]);
			(*finder).second->m_bUseDL = m_bUseDL;
		}

		pDoc->UpdateLIGHT();
	}
}

void CMAPLIGHTBar::OnBnClickedCheckUseLightmap()
{
	UpdateData();

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		for( int i=0; i<INT(pDoc->m_vSELLIGHT.size()); i++)
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[i]);
			(*finder).second->m_bUseLM = m_bUseLM;
		}

		pDoc->UpdateLIGHT();
	}
}

void CMAPLIGHTBar::OnEnChangeEditPOS()
{
	UpdateData();

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
		pDoc->UpdateLIGHT();
	}
}

void CMAPLIGHTBar::OnEnChangeEditDIR()
{
	UpdateData();

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		for( int i=0; i<INT(pDoc->m_vSELLIGHT.size()); i++)
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[i]);

			(*finder).second->m_vDIR.x = (FLOAT) atof(m_strDirX);
			(*finder).second->m_vDIR.y = (FLOAT) atof(m_strDirY);
			(*finder).second->m_vDIR.z = (FLOAT) atof(m_strDirZ);
		}

		pDoc->UpdateLIGHT();
	}
}

void CMAPLIGHTBar::OnEnChangeEditROT()
{
	UpdateData();

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		for( int i=0; i<INT(pDoc->m_vSELLIGHT.size()); i++)
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[i]);
			(*finder).second->m_fROT = ((FLOAT) atof(m_strROT)) * D3DX_PI / 180.0f;
		}

		pDoc->UpdateLIGHT();
	}
}

void CMAPLIGHTBar::OnEnChangeEditRange()
{
	UpdateData();

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		for( int i=0; i<INT(pDoc->m_vSELLIGHT.size()); i++)
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[i]);
			(*finder).second->m_fRange = (FLOAT) atof(m_strRange);
		}

		pDoc->UpdateLIGHT();
	}
}

void CALLBACK CMAPLIGHTBar::OnDragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

BYTE CMAPLIGHTBar::CanDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_TEXTURE ? TRUE : FALSE;
}

DROPEFFECT CALLBACK CMAPLIGHTBar::OnDragOver( CGSEditCtrl *pEdit,
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

			if(CanDrop(pItem))
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

BOOL CALLBACK CMAPLIGHTBar::OnDrop( CGSEditCtrl *pEdit,
								    COleDataObject* pDataObject,
									DROPEFFECT dropEffect,
									CPoint point)
{
	CMAPLIGHTBar *pBar = (CMAPLIGHTBar *) pEdit->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanDrop(pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->SetSRC((CTextureItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnDragLeave(pEdit);
	return pEdit->CEditView::OnDrop( pDataObject, dropEffect, point);
}

void CMAPLIGHTBar::SetSRC( CTextureItem *pItem)
{
	m_dwTextureID = pItem->m_dwID;
	m_strSRC = pItem->m_strName;
	UpdateData(FALSE);

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		for( int i=0; i<INT(pDoc->m_vSELLIGHT.size()); i++)
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[i]);
			(*finder).second->m_dwTextureID = m_dwTextureID;
		}

		pDoc->UpdateLIGHT();
	}

	EnableAllCtrl(TRUE);
}

void CMAPLIGHTBar::OnBnClickedButtonDellightsrc()
{
	m_strSRC.LoadString(IDS_NOTUSE);
	m_dwTextureID = 0;
	UpdateData(FALSE);

	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		for( int i=0; i<INT(pDoc->m_vSELLIGHT.size()); i++)
		{
			MAPMAPLIGHT::iterator finder = m_pItem->m_mapLIGHT.find(pDoc->m_vSELLIGHT[i]);
			(*finder).second->m_dwTextureID = m_dwTextureID;
		}

		pDoc->UpdateLIGHT();
	}

	EnableAllCtrl(TRUE);
}

void CMAPLIGHTBar::OnBnClickedButtonDelete()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	pDoc->DeleteLIGHT();
	UpdateItem();

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
}
