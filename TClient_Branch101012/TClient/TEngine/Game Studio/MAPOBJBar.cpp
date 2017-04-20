// MAPOBJBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "PreviewDlg.h"
#include "ObjectExchangeDlg.h"
#include "AdjustOBJPosDlg.h"
#include "MAPOBJBar.h"
#include "MainFrm.h"
#include "MAPDoc.h"
#include ".\mapobjbar.h"


VECTORMAPOBJBAR CMAPOBJBar::m_vMAPOBJBAR;

CTachyonObject *CMAPOBJBar::m_pTOBJ = NULL;
COBJItem *CMAPOBJBar::m_pOBJ = NULL;


// CMAPOBJBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CMAPOBJBar, CBarContent)

CMAPOBJBar::CMAPOBJBar(CWnd* pParent /*=NULL*/)
: CBarContent(CMAPOBJBar::IDD, pParent)
, m_nMode(0)
, m_nCLK(0)
, m_nCL(0)
, m_nMESH(0)
, m_nACT(0)
, m_nANI(0)
, m_strPosX(_T(""))
, m_strPosY(_T(""))
, m_strPosZ(_T(""))
, m_strRotX(_T(""))
, m_strRotY(_T(""))
, m_strRotZ(_T(""))
, m_strScaleX(_T(""))
, m_strScaleY(_T(""))
, m_strScaleZ(_T(""))
, m_dwOBJFUNC(0)
, m_dwACTFUNC(0)
, m_dwANIFUNC(0)
, m_strATTRVALUE(_T(""))
, m_strATTRTYPE(_T(""))
, m_bInsertOBJBrush(FALSE)
, m_nBrushSize(2)
, m_nBrushShape(0)
, m_nOBJCount(1)
, m_strOBJScaleMin(_T("0.8"))
, m_strOBJScaleMax(_T("1.2"))
, m_nVerticalOfPlane(0)
, m_strSELOBJCount(_T("SELCNT"))
{
	m_pItem = NULL;
	m_dwTick = 0;
}

CMAPOBJBar::~CMAPOBJBar()
{
}

void CMAPOBJBar::DoDataExchange( CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_INSERT, m_cInsertMode);
	DDX_Radio(pDX, IDC_RADIO_INSERT, m_nMode);
	DDX_Control(pDX, IDC_RADIO_EDIT, m_cEditMode);
	DDX_Control(pDX, IDC_COMBO_CLK, m_cCLK);
	DDX_CBIndex(pDX, IDC_COMBO_CLK, m_nCLK);
	DDX_Control(pDX, IDC_COMBO_CL, m_cCL);
	DDX_CBIndex(pDX, IDC_COMBO_CL, m_nCL);
	DDX_Control(pDX, IDC_COMBO_MESH, m_cMESH);
	DDX_CBIndex(pDX, IDC_COMBO_MESH, m_nMESH);
	DDX_Control(pDX, IDC_COMBO_ACT, m_cACT);
	DDX_CBIndex(pDX, IDC_COMBO_ACT, m_nACT);
	DDX_Control(pDX, IDC_COMBO_ANI, m_cANI);
	DDX_CBIndex(pDX, IDC_COMBO_ANI, m_nANI);
	DDX_Control(pDX, IDC_EDIT_POSX, m_cPosX);
	DDX_Text(pDX, IDC_EDIT_POSX, m_strPosX);
	DDX_Control(pDX, IDC_EDIT_POSY, m_cPosY);
	DDX_Text(pDX, IDC_EDIT_POSY, m_strPosY);
	DDX_Control(pDX, IDC_EDIT_POSZ, m_cPosZ);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_strPosZ);
	DDX_Control(pDX, IDC_EDIT_ROTX, m_cRotX);
	DDX_Text(pDX, IDC_EDIT_ROTX, m_strRotX);
	DDX_Control(pDX, IDC_EDIT_ROTY, m_cRotY);
	DDX_Text(pDX, IDC_EDIT_ROTY, m_strRotY);
	DDX_Control(pDX, IDC_EDIT_ROTZ, m_cRotZ);
	DDX_Text(pDX, IDC_EDIT_ROTZ, m_strRotZ);
	DDX_Control(pDX, IDC_EDIT_SCALEX, m_cScaleX);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_strScaleX);
	DDX_Control(pDX, IDC_EDIT_SCALEY, m_cScaleY);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_strScaleY);
	DDX_Control(pDX, IDC_EDIT_SCALEZ, m_cScaleZ);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_strScaleZ);
	DDX_Control(pDX, IDC_SPIN_POSX, m_spinPosX);
	DDX_Control(pDX, IDC_SPIN_POSY, m_spinPosY);
	DDX_Control(pDX, IDC_SPIN_POSZ, m_spinPosZ);
	DDX_Control(pDX, IDC_SPIN_ROTX, m_spinRotX);
	DDX_Control(pDX, IDC_SPIN_ROTY, m_spinRotY);
	DDX_Control(pDX, IDC_SPIN_ROTZ, m_spinRotZ);
	DDX_Control(pDX, IDC_SPIN_SCALEX, m_spinScaleX);
	DDX_Control(pDX, IDC_SPIN_SCALEY, m_spinScaleY);
	DDX_Control(pDX, IDC_SPIN_SCALEZ, m_spinScaleZ);
	DDX_Control(pDX, IDC_BUTTON_DELETE, m_cDelete);
	DDX_Control(pDX, IDC_EDIT_OBJFUNC, m_cOBJFUNC);
	DDX_Text(pDX, IDC_EDIT_OBJFUNC, m_dwOBJFUNC);
	DDX_Control(pDX, IDC_SPIN_OBJFUNC, m_spinOBJFUNC);
	DDX_Control(pDX, IDC_EDIT_ACTFUNC, m_cACTFUNC);
	DDX_Text(pDX, IDC_EDIT_ACTFUNC, m_dwACTFUNC);
	DDX_Control(pDX, IDC_SPIN_ACTFUNC, m_spinACTFUNC);
	DDX_Control(pDX, IDC_EDIT_ANIFUNC, m_cANIFUNC);
	DDX_Text(pDX, IDC_EDIT_ANIFUNC, m_dwANIFUNC);
	DDX_Control(pDX, IDC_SPIN_ANIFUNC, m_spinANIFUNC);
	DDX_Control(pDX, IDC_LIST_ATTR, m_cATTR);
	DDX_Control(pDX, IDC_EDIT_VALUE, m_cATTRVALUE);
	DDX_Text(pDX, IDC_EDIT_VALUE, m_strATTRVALUE);
	DDX_Control(pDX, IDC_STATIC_ATTRTYPE, m_cATTRTYPE);
	DDX_Text(pDX, IDC_STATIC_ATTRTYPE, m_strATTRTYPE);
	DDX_Control(pDX, IDC_BUTTON_LANDING, m_cLanding);
	DDX_Control(pDX, IDC_CHECK_INSERTOBJBYBRUSH, m_cInsertOBJBrush);
	DDX_Check(pDX, IDC_CHECK_INSERTOBJBYBRUSH, m_bInsertOBJBrush);
	DDX_Control(pDX, IDC_SPIN_OBJBRUSH_SIZE, m_spinBrushSize);
	DDX_Text(pDX, IDC_EDIT_OBJBRUSH_SIZE, m_nBrushSize);
	DDX_Control(pDX, IDC_COMBO_OBJBRUSH_SHAPE, m_cBrushShape);
	DDX_CBIndex(pDX, IDC_COMBO_OBJBRUSH_SHAPE, m_nBrushShape);
	DDX_Control(pDX, IDC_EDIT_OBJCOUNT, m_cRectInOBJCount);
	DDX_Text(pDX, IDC_EDIT_OBJCOUNT, m_nOBJCount);
	DDX_Control(pDX, IDC_SPIN_OBJCOUNT, m_spinRectInOBJCount);
	DDX_Control(pDX, IDC_EDIT_OBJBRUSH_SIZE, m_cBrushSize);
	DDX_Text(pDX, IDC_EDIT_OBJSCALE_MIN, m_strOBJScaleMin);
	DDX_Text(pDX, IDC_EDIT_OBJSCALE_MAX, m_strOBJScaleMax);
	DDX_Control(pDX, IDC_EDIT_OBJSCALE_MIN, m_cOBJMinSize);
	DDX_Control(pDX, IDC_EDIT_OBJSCALE_MAX, m_cOBJMaxSize);
	DDX_Control(pDX, IDC_COMBO_OBJXY_SHAPE2, m_cVerticalOfPlane);
	DDX_CBIndex(pDX, IDC_COMBO_OBJXY_SHAPE2, m_nVerticalOfPlane);
	DDX_Control(pDX, IDC_BUTTON_OBJECT_EXCHANGE, m_cOBJExchange);
	DDX_Control(pDX, IDC_BUTTON_RESETOBJ_ATTRI, m_cResetOBJATTR);
	DDX_Control(pDX, IDC_EDIT_SELOBJ_COUNT, m_cSELOBJCount);
	DDX_Text(pDX, IDC_EDIT_SELOBJ_COUNT, m_strSELOBJCount);
	DDX_Control(pDX, IDC_BUTTON_ADJUST_OBJECT_POS, m_cAdjustOBJPOS);
	DDX_Control(pDX, IDC_BUTTON_FINDOBJ, m_cFINDOBJ);
}


BEGIN_MESSAGE_MAP( CMAPOBJBar, CBarContent)
	ON_MESSAGE( WM_PLAYCTRL_PROC, OnPlayCtrlProc)
	ON_MESSAGE( WM_PLAYCTRL_PLAY, OnPlayCtrlPlay)
	ON_MESSAGE( WM_PLAYCTRL_STOP, OnPlayCtrlStop)
	ON_MESSAGE( WM_MESSAGEBOX, OnMessageBox)
	ON_BN_CLICKED( IDC_BUTTON_BACK, OnBnClickedButtonBack)
	ON_CBN_SELCHANGE(IDC_COMBO_CLK, OnCbnSelchangeComboClk)
	ON_CBN_SELCHANGE(IDC_COMBO_CL, OnCbnSelchangeComboCl)
	ON_CBN_SELCHANGE(IDC_COMBO_MESH, OnCbnSelchangeComboMesh)
	ON_CBN_SELCHANGE(IDC_COMBO_ACT, OnCbnSelchangeComboAct)
	ON_CBN_SELCHANGE(IDC_COMBO_ANI, OnCbnSelchangeComboAni)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_EDIT, OnBnClickedRadioEdit)
	ON_BN_CLICKED(IDC_RADIO_INSERT, OnBnClickedRadioInsert)
	ON_EN_CHANGE(IDC_EDIT_POSX, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_POSY, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_POSZ, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_ROTX, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_ROTY, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_ROTZ, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_SCALEX, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_SCALEY, OnEnChangeEditData)
	ON_EN_CHANGE(IDC_EDIT_SCALEZ, OnEnChangeEditData)
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_EN_CHANGE(IDC_EDIT_OBJFUNC, OnEnChangeEditObjfunc)
	ON_EN_CHANGE(IDC_EDIT_ACTFUNC, OnEnChangeEditActfunc)
	ON_EN_CHANGE(IDC_EDIT_ANIFUNC, OnEnChangeEditAnifunc)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ATTR, OnLvnItemchangedListAttr)
	ON_EN_CHANGE(IDC_EDIT_VALUE, OnEnChangeEditValue)
	ON_BN_CLICKED(IDC_BUTTON_LANDING, OnBnClickedButtonLanding)
	ON_BN_CLICKED(IDC_BUTTON_HIDE, OnBnClickedButtonHide)
	ON_BN_CLICKED(IDC_BUTTON_FREEZE, OnBnClickedButtonFreeze)
	ON_BN_CLICKED(IDC_BUTTON_UNFREEZEALL, OnBnClickedButtonUnfreezeall)
	ON_BN_CLICKED(IDC_BUTTON_UNHIDEALL, OnBnClickedButtonUnhideall)
	ON_EN_CHANGE(IDC_EDIT_OBJBRUSH_SIZE, OnEnChangeEditObjbrushSize)
	ON_CBN_SELCHANGE(IDC_COMBO_OBJBRUSH_SHAPE, OnCbnSelchangeComboObjbrushShape)
	ON_EN_CHANGE(IDC_EDIT_OBJCOUNT, OnEnChangeEditObjcount)
	ON_BN_CLICKED(IDC_CHECK_INSERTOBJBYBRUSH, OnBnClickedCheckInsertobjbybrush)
	ON_BN_CLICKED(IDC_BUTTON_OBJECT_EXCHANGE, OnBnClickedButtonObjectExchange)
	ON_BN_CLICKED(IDC_BUTTON_RESETOBJ_ATTRI, OnBnClickedButtonResetobjAttri)
	ON_BN_CLICKED(IDC_BUTTON_ADJUST_OBJECT_POS, OnBnClickedButtonAdjustObjectPos)
	ON_BN_CLICKED(IDC_BUTTON_FINDOBJ, OnBnClickedButtonFindobj)
	ON_BN_CLICKED(IDC_BUTTON_HIDECOLL, OnBnClickedButtonHidecoll)
	ON_BN_CLICKED(IDC_BUTTON_UNHIDECOLL, OnBnClickedButtonUnhidecoll)
	ON_BN_CLICKED(IDC_BUTTON_HIDESILH, OnBnClickedButtonHidesilh)
	ON_BN_CLICKED(IDC_BUTTON_UNHIDESILH, OnBnClickedButtonUnhidesilh)
	ON_BN_CLICKED(IDC_BUTTON_HIDEWATER, OnBnClickedButtonHidewater)
	ON_BN_CLICKED(IDC_BUTTON_UNHIDEWATER, OnBnClickedButtonUnhidewater)
END_MESSAGE_MAP()


// CMAPOBJBar 메시지 처리기입니다.

void CMAPOBJBar::InitOBJBAR()
{
	m_vMAPOBJBAR.clear();
	m_pTOBJ = NULL;
	m_pOBJ = NULL;
}

void CMAPOBJBar::ReleaseOBJBAR()
{
	m_vMAPOBJBAR.clear();

	if(m_pTOBJ)
	{
		delete m_pTOBJ;
		m_pTOBJ = NULL;
	}
	m_pOBJ = NULL;
}

void CMAPOBJBar::SetOBJ( CItemBase *pItem)
{
	BOOL bUpdate = m_pOBJ != pItem;

	if(bUpdate)
	{
		m_pOBJ = pItem && pItem->m_bType == ITEM_OBJECT ? (COBJItem *) pItem : NULL;
		int nCount = INT(m_vMAPOBJBAR.size());

		if(m_pTOBJ)
		{
			delete m_pTOBJ;
			m_pTOBJ = NULL;
		}

		if(m_pOBJ)
		{
			m_pTOBJ = new CTachyonObject();
			m_pTOBJ->InitOBJ(&m_pOBJ->m_OBJ);
		}

//		for( int i=0; i<nCount; i++)
//			if(!m_vMAPOBJBAR[i]->m_nMode)
//				m_vMAPOBJBAR[i]->UpdateItem();
	}
}

void CMAPOBJBar::UpdateItem()
{
	if(m_nMode)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

		m_cATTR.DeleteAllItems();
		m_cMESH.ResetContent();
		m_cCLK.ResetContent();
		m_cACT.ResetContent();
		m_cANI.ResetContent();
		m_cCL.ResetContent();

		if( INT(pDoc->m_vSELOBJ.size()) == 1 )
		{
			MAPMAPOBJ::iterator itOBJ = pDoc->m_pMAP->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
			MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

			SetCLKList();
			SetACTList();

			m_dwOBJFUNC = (*itOBJ).second->m_dwOBJFuncID;
			m_dwACTFUNC = (*itOBJ).second->m_dwACTFuncID;
			m_dwANIFUNC = (*itOBJ).second->m_dwANIFuncID;

			if( itID != pDoc->m_pMAP->m_mapOBJ.end() )
			{
				COBJItem *pOBJ = CItemBase::GetOBJItem((*itID).second);

				if(pOBJ)
				{
					m_pItem->SyncOBJATTR(
						pOBJ,
						(*itOBJ).second);

					MAPATTRIBUTE::iterator itATTR = (*itOBJ).second->m_mapATTR.begin();
					int nATTR = 0;

					while(itATTR != (*itOBJ).second->m_mapATTR.end())
					{
						MAPATTRIBUTE::iterator itNEXT = itATTR;
						itNEXT++;

						MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find((*itATTR).first);
						if( itSTR == pOBJ->m_mapNAME.end() )
						{
							delete (*itATTR).second;
							(*itOBJ).second->m_mapATTR.erase(itATTR);
						}
						else
						{
							int nIndex = m_cATTR.InsertItem( nATTR, (*itSTR).second);
							m_cATTR.SetItemData( nIndex, (*itATTR).first);
							nATTR++;
						}
						itATTR = itNEXT;
					}
				}
			}
		}
		if( INT(pDoc->m_vSELOBJ.size()) > 0 )
			ResetSELOBJState();
		UpdateData(FALSE);
	}
	else
	{
		SetCLKList();
		SetACTList();
	}

	EnableAllCtrl(TRUE);
}

BOOL CMAPOBJBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	m_cATTR.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	m_cATTR.GetClientRect(&rect);
	rect.DeflateRect( 1, 0);
	m_cATTR.InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_spinOBJFUNC.SetRange( 0, 10000);
	m_spinACTFUNC.SetRange( 0, 10000);
	m_spinANIFUNC.SetRange( 0, 10000);

	m_spinScaleX.SetRange( -100, 100);
	m_spinScaleY.SetRange( -100, 100);
	m_spinScaleZ.SetRange( -100, 100);
	m_spinPosX.SetRange( -100, 100);
	m_spinPosY.SetRange( -100, 100);
	m_spinPosZ.SetRange( -100, 100);
	m_spinRotX.SetRange( -100, 100);
	m_spinRotY.SetRange( -100, 100);
	m_spinRotZ.SetRange( -100, 100);

	m_spinBrushSize.SetRange( 1, 1000 );
	m_spinRectInOBJCount.SetRange( 1, 1000 );

	return TRUE;
}

void CMAPOBJBar::EnableAllCtrl( BOOL bEnable)
{
	m_cResetOBJATTR.EnableWindow(bEnable);
	m_cInsertMode.EnableWindow(bEnable);
	m_cEditMode.EnableWindow(bEnable);

	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	if(m_nMode)
	{
		m_spinScaleX.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_spinScaleY.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_spinScaleZ.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_spinPosX.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_spinPosY.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_spinPosZ.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_spinRotX.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_spinRotY.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_spinRotZ.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cLanding.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cDelete.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());

		m_spinOBJFUNC.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);
		m_spinACTFUNC.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);
		m_spinANIFUNC.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);
		m_cOBJFUNC.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);
		m_cACTFUNC.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);
		m_cANIFUNC.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);
		m_cFINDOBJ.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);

		m_cCLK.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);
		m_cCL.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1 && m_cCL.GetCount() != 0);
		m_cMESH.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1 && m_cMESH.GetCount() != 0);

		m_cACT.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);
		m_cANI.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1 && m_cANI.GetCount() != 0);

		m_cATTRVALUE.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1 && m_cATTR.GetFirstSelectedItemPosition());
		m_cATTRTYPE.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1 && m_cATTR.GetFirstSelectedItemPosition());
		m_cATTR.EnableWindow(bEnable && INT(pDoc->m_vSELOBJ.size()) == 1);

		m_cAdjustOBJPOS.EnableWindow( m_nMode && INT(pDoc->m_vSELOBJ.size()) == 1 && m_nCL != 0 && m_nMESH != 0 );
		m_cOBJExchange.EnableWindow( m_nMode && INT(pDoc->m_vSELOBJ.size()) == 1 && m_nCL != 0 && m_nMESH != 0 );
		m_spinRectInOBJCount.EnableWindow(FALSE);
		m_cVerticalOfPlane.EnableWindow(FALSE);
		m_cRectInOBJCount.EnableWindow(FALSE);
		m_cSELOBJCount.EnableWindow(bEnable);
		m_spinBrushSize.EnableWindow(FALSE);
		m_cOBJMaxSize.EnableWindow(FALSE);
		m_cOBJMinSize.EnableWindow(FALSE);
		m_cBrushShape.EnableWindow(FALSE);
		m_cBrushSize.EnableWindow(FALSE);

		m_cScaleX.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cScaleY.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cScaleZ.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cPosX.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cPosY.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cPosZ.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cRotX.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cRotY.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());
		m_cRotZ.EnableWindow(bEnable && !pDoc->m_vSELOBJ.empty());	
	}
	else
	{
		m_spinScaleX.EnableWindow(FALSE);
		m_spinScaleY.EnableWindow(FALSE);
		m_spinScaleZ.EnableWindow(FALSE);
		m_spinPosX.EnableWindow(FALSE);
		m_spinPosY.EnableWindow(FALSE);
		m_spinPosZ.EnableWindow(FALSE);
		m_spinRotX.EnableWindow(FALSE);
		m_spinRotY.EnableWindow(FALSE);
		m_spinRotZ.EnableWindow(FALSE);
		m_cLanding.EnableWindow(FALSE);
		m_cDelete.EnableWindow(FALSE);
		m_cFINDOBJ.EnableWindow(FALSE);

		m_cATTRVALUE.EnableWindow(FALSE);
		m_cATTRTYPE.EnableWindow(FALSE);
		m_cATTR.EnableWindow(FALSE);

		m_spinOBJFUNC.EnableWindow(bEnable && m_pOBJ);
		m_spinACTFUNC.EnableWindow(bEnable && m_pOBJ);
		m_spinANIFUNC.EnableWindow(bEnable && m_pOBJ);
		m_cOBJFUNC.EnableWindow(bEnable && m_pOBJ);
		m_cACTFUNC.EnableWindow(bEnable && m_pOBJ);
		m_cANIFUNC.EnableWindow(bEnable && m_pOBJ);

		m_cCLK.EnableWindow(bEnable && m_pOBJ);
		m_cCL.EnableWindow(bEnable && m_pOBJ && m_cCL.GetCount() != 0);
		m_cMESH.EnableWindow(bEnable && m_pOBJ && m_cMESH.GetCount() != 0);

		m_cACT.EnableWindow(bEnable && m_pOBJ);
		m_cANI.EnableWindow(bEnable && m_pOBJ && m_cANI.GetCount() != 0);

		m_spinRectInOBJCount.EnableWindow(bEnable && m_bInsertOBJBrush);
		m_cVerticalOfPlane.EnableWindow(bEnable && m_bInsertOBJBrush);
		m_cRectInOBJCount.EnableWindow(bEnable && m_bInsertOBJBrush);
		m_spinBrushSize.EnableWindow(bEnable && m_bInsertOBJBrush);
		m_cBrushShape.EnableWindow(bEnable && m_bInsertOBJBrush);
		m_cOBJMinSize.EnableWindow(bEnable && m_bInsertOBJBrush);
		m_cOBJMaxSize.EnableWindow(bEnable && m_bInsertOBJBrush);
		m_cBrushSize.EnableWindow(bEnable && m_bInsertOBJBrush);
		m_cAdjustOBJPOS.EnableWindow(FALSE);
		m_cOBJExchange.EnableWindow(FALSE);
		m_cSELOBJCount.EnableWindow(FALSE);

		m_cScaleX.EnableWindow(FALSE);
		m_cScaleY.EnableWindow(FALSE);
		m_cScaleZ.EnableWindow(FALSE);
		m_cPosX.EnableWindow(FALSE);
		m_cPosY.EnableWindow(FALSE);
		m_cPosZ.EnableWindow(FALSE);
		m_cRotX.EnableWindow(FALSE);
		m_cRotY.EnableWindow(FALSE);
		m_cRotZ.EnableWindow(FALSE);
	}
}

void CMAPOBJBar::SetItem( CMAPItem *pItem)
{
	m_pItem = pItem;
	UpdateItem();
}

void CMAPOBJBar::OnBnClickedButtonBack()
{
	OnBack();
}

void CMAPOBJBar::OnBack()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	CFrameWnd *pFrame = GetParentFrame();

	pDoc->ClearSELOBJ();
	pDoc->m_bEditOBJ = FALSE;
	
	m_bInsertOBJBrush = FALSE;
	m_strSELOBJCount.Format("SELCNT");

	pFrame->SendMessage( WM_COMMAND, ID_MAP_BACK);
	pDoc->RedrawAllView();
}

void CMAPOBJBar::SetCLKList()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	MAPCLKIND::iterator it;

	CTachyonObject *pTOBJ = NULL;
	COBJItem *pOBJ = NULL;

	if(m_nMode)
	{
		MAPDWORD::iterator itOBJ = m_pItem->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
		MAPOBJECT::iterator itTOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

		pOBJ = CItemBase::GetOBJItem((*itOBJ).second);
		pTOBJ = (*itTOBJ).second;
	}
	else
	{
		pTOBJ = m_pTOBJ;
		pOBJ = m_pOBJ;
	}

	m_cMESH.ResetContent();
	m_cCLK.ResetContent();
	m_cCL.ResetContent();

	if( !pOBJ || !pTOBJ )
		return;

	int nIndex = m_cCLK.InsertString( 0, "NONE");
	m_cCLK.SetItemData( nIndex, 0);

	for( it = pOBJ->m_OBJ.m_mapCLKIND.begin(); it != pOBJ->m_OBJ.m_mapCLKIND.end(); it++)
	{
		MAPSTR::iterator finder = pOBJ->m_mapNAME.find((*it).first);

		nIndex = m_cCLK.InsertString( m_cCLK.GetCount(), (*finder).second);
		m_cCLK.SetItemData( nIndex, (*it).first);

		if( (*it).first == pTOBJ->m_OBJ.m_dwCurCLK )
		{
			m_cCLK.SetCurSel(nIndex);
			OnCbnSelchangeComboClk();
		}
	}

	if( m_cCLK.GetCurSel() == -1 )
	{
		m_cCLK.SetCurSel(m_cCLK.GetCount() > 1 ? 1 : 0);
		OnCbnSelchangeComboClk();
	}

	pDoc->RedrawAllView();
	UpdateData();
}

void CMAPOBJBar::SetACTList()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	MAPACTION::iterator it;

	CTachyonObject *pTOBJ = NULL;
	COBJItem *pOBJ = NULL;

	if(m_nMode)
	{
		MAPDWORD::iterator itOBJ = m_pItem->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
		MAPOBJECT::iterator itTOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

		pOBJ = CItemBase::GetOBJItem((*itOBJ).second);
		pTOBJ = (*itTOBJ).second;
	}
	else
	{
		pTOBJ = m_pTOBJ;
		pOBJ = m_pOBJ;
	}

	m_cACT.ResetContent();
	m_cANI.ResetContent();

	if( !pOBJ || !pTOBJ )
		return;

	int nIndex = m_cACT.InsertString( 0, "NONE");
	m_cACT.SetItemData( nIndex, 0);

	for( it = pOBJ->m_OBJ.m_mapACTION.begin(); it != pOBJ->m_OBJ.m_mapACTION.end(); it++)
	{
		MAPSTR::iterator finder = pOBJ->m_mapNAME.find((*it).first);

		nIndex = m_cACT.InsertString( m_cACT.GetCount(), (*finder).second);
		m_cACT.SetItemData( nIndex, (*it).first);

		if( (*it).first == pTOBJ->m_OBJ.m_dwCurACT )
		{
			m_cACT.SetCurSel(nIndex);
			OnCbnSelchangeComboAct();
		}
	}

	if( m_cACT.GetCurSel() == -1 )
	{
		m_cACT.SetCurSel(0);
		OnCbnSelchangeComboAct();
	}

	pDoc->RedrawAllView();
	UpdateData();
}

void CMAPOBJBar::OnCbnSelchangeComboClk()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nIndex = m_cCLK.GetCurSel();

	CTachyonObject *pTOBJ = NULL;
	COBJItem *pOBJ = NULL;

	if(m_nMode)
	{
		MAPDWORD::iterator itOBJ = m_pItem->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
		MAPOBJECT::iterator itTOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

		pOBJ = CItemBase::GetOBJItem((*itOBJ).second);
		pTOBJ = (*itTOBJ).second;
	}
	else
	{
		pTOBJ = m_pTOBJ;
		pOBJ = m_pOBJ;
	}

	m_cMESH.ResetContent();
	m_cCL.ResetContent();

	if( nIndex > 0 && pOBJ && pTOBJ )
	{
		DWORD dwCLK = (DWORD) m_cCLK.GetItemData(nIndex);
		MAPCLKIND::iterator itCLK = pOBJ->m_OBJ.m_mapCLKIND.find(dwCLK);

		if( itCLK != pOBJ->m_OBJ.m_mapCLKIND.end() )
		{
			MAPCLKINST::iterator itCI = pTOBJ->m_OBJ.m_mapCLK.find(dwCLK);
			MAPCLOTH::iterator itCL;

			LPCLKIND pCLK = (*itCLK).second;
			LPCLKINST pCI = NULL;

			if( itCI == pTOBJ->m_OBJ.m_mapCLK.end() )
			{
				pCI = new CLKINST();
				pTOBJ->m_OBJ.m_mapCLK.insert( MAPCLKINST::value_type( dwCLK, pCI));
			}
			else
				pCI = (*itCI).second;

			nIndex = m_cCL.InsertString( 0, "NONE");
			m_cCL.SetItemData( nIndex, 0);

			for( itCL = pCLK->m_mapCLOTH.begin(); itCL != pCLK->m_mapCLOTH.end(); itCL++)
			{
				MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find((*itCL).first);

				nIndex = m_cCL.InsertString( m_cCL.GetCount(), (*itSTR).second);
				m_cCL.SetItemData( nIndex, (*itCL).first);

				if( pCI->m_dwCurCL == (*itCL).first )
				{
					m_cCL.SetCurSel(nIndex);
					OnCbnSelchangeComboCl();
				}
			}

			if( m_cCL.GetCurSel() == -1 )
			{
				m_cCL.SetCurSel(m_cCL.GetCount() > 1 ? 1 : 0);
				OnCbnSelchangeComboCl();
			}
		}
	}

	pDoc->RedrawAllView();
	UpdateData();
	EnableAllCtrl(TRUE);
}

void CMAPOBJBar::OnCbnSelchangeComboCl()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nIndex = m_cCLK.GetCurSel();

	CTachyonObject *pTOBJ = NULL;
	LPMAPOBJ pMAPOBJ = NULL;
	COBJItem *pOBJ = NULL;

	if(m_nMode)
	{
		MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
		MAPDWORD::iterator itOBJ = m_pItem->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
		MAPOBJECT::iterator itTOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

		pOBJ = CItemBase::GetOBJItem((*itOBJ).second);
		pMAPOBJ = (*itMAPOBJ).second;
		pTOBJ = (*itTOBJ).second;
	}
	else
	{
		pTOBJ = m_pTOBJ;
		pOBJ = m_pOBJ;
	}

	m_cMESH.ResetContent();
	if( nIndex > 0 && pOBJ && pTOBJ )
	{
		DWORD dwCLK = (DWORD) m_cCLK.GetItemData(nIndex);

		MAPCLKINST::iterator itCI = pTOBJ->m_OBJ.m_mapCLK.find(dwCLK);
		LPCLKINST pCI = (*itCI).second;

		nIndex = m_cCL.GetCurSel();
		if( nIndex > 0 )
		{
			DWORD dwCL = (DWORD) m_cCL.GetItemData(nIndex);
			MAPCLKIND::iterator itCLK = pOBJ->m_OBJ.m_mapCLKIND.find(dwCLK);

			if( itCLK != pOBJ->m_OBJ.m_mapCLKIND.end() )
			{
				LPCLKIND pCLK = (*itCLK).second;
				MAPCLOTH::iterator itCL = pCLK->m_mapCLOTH.find(dwCL);

				if( itCL != pCLK->m_mapCLOTH.end() )
				{
					LPCLOTH pCL = (*itCL).second;
					MAPMESH::iterator itMESH;

					nIndex = m_cMESH.InsertString( 0, "NONE");
					m_cMESH.SetItemData( nIndex, 0);
					pCI->m_dwCurCL = (*itCL).first;

					for( itMESH = pCL->m_mapMESH.begin(); itMESH != pCL->m_mapMESH.end(); itMESH++)
					{
						MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find((*itMESH).first);

						nIndex = m_cMESH.InsertString( m_cMESH.GetCount(), (*itSTR).second);
						m_cMESH.SetItemData( nIndex, (*itMESH).first);

						if( pCI->m_dwCurMESH == (*itMESH).first )
						{
							m_cMESH.SetCurSel(nIndex);
							OnCbnSelchangeComboMesh();
						}
					}

					if( m_cMESH.GetCurSel() == -1 )
					{
						m_cMESH.SetCurSel(m_cMESH.GetCount() > 1 ? 1 : 0);
						OnCbnSelchangeComboMesh();
					}
				}
			}
		}
		else
			pCI->m_dwCurCL = 0;

		pTOBJ->SetCloth(
			&CMainFrame::m_3DDevice,
			dwCLK,
			pCI->m_dwCurCL,
			pCI->m_dwCurMESH);
		pTOBJ->ResetOBJPart(&CMainFrame::m_3DDevice);

		if(pMAPOBJ)
		{
			MAPCLKINST::iterator itOBJCI = pMAPOBJ->m_mapCLK.find(dwCLK);
			LPCLKINST pOBJCI = (*itOBJCI).second;

			pOBJCI->m_dwCurMESH = pCI->m_dwCurMESH;
			pOBJCI->m_dwCurCL = pCI->m_dwCurCL;
		}
	}

	pDoc->RedrawAllView();
	UpdateData();
	EnableAllCtrl(TRUE);
}

void CMAPOBJBar::OnCbnSelchangeComboMesh()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nIndex = m_cCLK.GetCurSel();

	CTachyonObject *pTOBJ = NULL;
	LPMAPOBJ pMAPOBJ = NULL;

	if(m_nMode)
	{
		MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
		MAPOBJECT::iterator itTOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

		pMAPOBJ = (*itMAPOBJ).second;
		pTOBJ = (*itTOBJ).second;
	}
	else
		pTOBJ = m_pTOBJ;

	if( nIndex > 0 && pTOBJ )
	{
		DWORD dwCLK = (DWORD) m_cCLK.GetItemData(nIndex);
		MAPCLKINST::iterator itCI = pTOBJ->m_OBJ.m_mapCLK.find(dwCLK);

		if( itCI != pTOBJ->m_OBJ.m_mapCLK.end() )
		{
			LPCLKINST pCI = (*itCI).second;
			nIndex = m_cMESH.GetCurSel();

			if( nIndex > 0 )
				pCI->m_dwCurMESH = (DWORD) m_cMESH.GetItemData(nIndex);
			else
				pCI->m_dwCurMESH = 0;

			pTOBJ->SetCloth(
				&CMainFrame::m_3DDevice,
				dwCLK,
				pCI->m_dwCurCL,
				pCI->m_dwCurMESH);
			pTOBJ->ResetOBJPart(&CMainFrame::m_3DDevice);

			if(pMAPOBJ)
			{
				MAPCLKINST::iterator itOBJCI = pMAPOBJ->m_mapCLK.find(dwCLK);
				LPCLKINST pOBJCI = (*itOBJCI).second;

				pOBJCI->m_dwCurMESH = pCI->m_dwCurMESH;
				pOBJCI->m_dwCurCL = pCI->m_dwCurCL;
			}
		}
	}

	pDoc->RedrawAllView();
	UpdateData();
	EnableAllCtrl(TRUE);
}

void CMAPOBJBar::OnCbnSelchangeComboAct()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nIndex = m_cACT.GetCurSel();

	CTachyonObject *pTOBJ = NULL;
	LPMAPOBJ pMAPOBJ = NULL;
	COBJItem *pOBJ = NULL;

	if(m_nMode)
	{
		MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
		MAPDWORD::iterator itOBJ = m_pItem->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
		MAPOBJECT::iterator itTOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

		pOBJ = CItemBase::GetOBJItem((*itOBJ).second);
		pMAPOBJ = (*itMAPOBJ).second;
		pTOBJ = (*itTOBJ).second;
	}
	else
	{
		pTOBJ = m_pTOBJ;
		pOBJ = m_pOBJ;
	}

	m_cANI.ResetContent();
	if( nIndex >= 0 && pOBJ && pTOBJ )
	{
		DWORD dwACT = (DWORD) m_cACT.GetItemData(nIndex);
		MAPACTION::iterator itACT = pOBJ->m_OBJ.m_mapACTION.find(dwACT);

		if( itACT != pOBJ->m_OBJ.m_mapACTION.end() )
		{
			MAPACTINST::iterator itAI = pTOBJ->m_OBJ.m_mapACT.find(dwACT);
			MAPANI::iterator itANI;

			LPACTION pACT = (*itACT).second;
			LPACTINST pAI = (*itAI).second;

			nIndex = m_cANI.InsertString( 0, "NONE");
			m_cANI.SetItemData( nIndex, 0);
			pTOBJ->m_OBJ.m_dwCurACT = dwACT;

			for( itANI = pACT->m_mapANI.begin(); itANI != pACT->m_mapANI.end(); itANI++)
			{
				MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find((*itANI).first);

				nIndex = m_cANI.InsertString( m_cANI.GetCount(), (*itSTR).second);
				m_cANI.SetItemData( nIndex, (*itANI).first);

				if( (*itANI).first == pAI->m_dwCurANI )
				{
					m_cANI.SetCurSel(nIndex);
					OnCbnSelchangeComboAni();
				}
			}

			if( m_cANI.GetCurSel() == -1 )
			{
				m_cANI.SetCurSel(0);
				OnCbnSelchangeComboAni();
			}

			DWORD dwActID = pTOBJ->m_OBJ.m_dwCurACT;
			DWORD dwAniID = pAI->m_dwCurANI;

			pTOBJ->m_OBJ.m_dwCurACT = 0;
			pAI->m_dwCurANI = 0;

			pTOBJ->SetAction(
				dwActID,
				dwAniID);

			if(pMAPOBJ)
			{
				pMAPOBJ->m_dwCurACT = pTOBJ->m_OBJ.m_dwCurACT;
				pMAPOBJ->m_dwCurANI = pAI->m_dwCurANI;
			}
		}
		else
		{
			pTOBJ->SetAction( 0, 0);

			if(pMAPOBJ)
			{
				pMAPOBJ->m_dwCurACT = 0;
				pMAPOBJ->m_dwCurANI = 0;
			}
		}
	}

	pDoc->RedrawAllView();
	UpdateData();
	EnableAllCtrl(TRUE);
}

void CMAPOBJBar::OnCbnSelchangeComboAni()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nIndex = m_cANI.GetCurSel();

	CTachyonObject *pTOBJ = NULL;
	LPMAPOBJ pMAPOBJ = NULL;

	if(m_nMode)
	{
		MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
		MAPOBJECT::iterator itTOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

		pMAPOBJ = (*itMAPOBJ).second;
		pTOBJ = (*itTOBJ).second;
	}
	else
		pTOBJ = m_pTOBJ;

	if( nIndex >= 0 )
	{
		MAPACTINST::iterator itAI = pTOBJ->m_OBJ.m_mapACT.find(pTOBJ->m_OBJ.m_dwCurACT);

		if( itAI != pTOBJ->m_OBJ.m_mapACT.end() )
		{
			LPACTINST pAI = (*itAI).second;

			if(pAI)
				pAI->m_dwCurANI = (DWORD) m_cANI.GetItemData(nIndex);

			DWORD dwActID = pTOBJ->m_OBJ.m_dwCurACT;
			DWORD dwAniID = pAI->m_dwCurANI;

			pTOBJ->m_OBJ.m_dwCurACT = 0;
			pAI->m_dwCurANI = 0;

			pTOBJ->SetAction(
				dwActID,
				dwAniID);

			if(pMAPOBJ)
			{
				pMAPOBJ->m_dwCurACT = pTOBJ->m_OBJ.m_dwCurACT;
				pMAPOBJ->m_dwCurANI = pAI->m_dwCurANI;
			}
		}
		else
		{
			pTOBJ->SetAction( pTOBJ->m_OBJ.m_dwCurACT, 0);

			if(pMAPOBJ)
			{
				pMAPOBJ->m_dwCurACT = pTOBJ->m_OBJ.m_dwCurACT;
				pMAPOBJ->m_dwCurANI = 0;
			}
		}
	}

	pDoc->RedrawAllView();
	UpdateData();
	EnableAllCtrl(TRUE);
}

int CMAPOBJBar::OnCreate( LPCREATESTRUCT lpCreateStruct)
{
	if (CBarContent::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_vMAPOBJBAR.push_back(this);
	return 0;
}

void CMAPOBJBar::OnDestroy()
{
	CBarContent::OnDestroy();
	VECTORMAPOBJBAR::iterator it = m_vMAPOBJBAR.begin();
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	while( it != m_vMAPOBJBAR.end() )
		if( (*it) == this )
			m_vMAPOBJBAR.erase(it);
		else
			it++;
	pDoc->ClearSELOBJ();
}

void CMAPOBJBar::OnBnClickedRadioEdit()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	
	// 06/04/24 PSY
	///////////////////////////////////////
	m_cInsertOBJBrush.EnableWindow( FALSE );
	m_cInsertOBJBrush.SetCheck( BST_UNCHECKED );
	m_bInsertOBJBrush = FALSE;
	////////////////////////////////////////
	
	pDoc->ClearSELOBJ();
	UpdateData();
	UpdateItem();

	pDoc->m_bInsertOBJ = m_nMode == 0;
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedRadioInsert()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	
	// 06/04/24 PSY
	///////////////////////////////////////
	m_cInsertOBJBrush.EnableWindow( TRUE );
	m_cInsertOBJBrush.SetCheck( BST_UNCHECKED );
	m_bInsertOBJBrush = TRUE;
	////////////////////////////////////////
	
	// 06/08/02 PSY
	m_strSELOBJCount.Format("SELCNT");

	pDoc->ClearSELOBJ();
	UpdateData();
	UpdateItem();

	pDoc->m_bInsertOBJ = m_nMode == 0;
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinPosX )
	{
		FLOAT fPosX = (FLOAT) atof(m_strPosX);

		fPosX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinPosX.SetPos(0);

		m_strPosX.Format( "%.4f", fPosX);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinPosY )
	{
		FLOAT fPosY = (FLOAT) atof(m_strPosY);

		fPosY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinPosY.SetPos(0);

		m_strPosY.Format( "%.4f", fPosY);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinPosZ )
	{
		FLOAT fPosZ = (FLOAT) atof(m_strPosZ);

		fPosZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinPosZ.SetPos(0);

		m_strPosZ.Format( "%.4f", fPosZ);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinRotX )
	{
		FLOAT fRotX = (FLOAT) atof(m_strRotX);

		fRotX += (FLOAT) INT(nPos);
		m_spinRotX.SetPos(0);

		m_strRotX.Format( "%.2f", fRotX);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinRotY )
	{
		FLOAT fRotY = (FLOAT) atof(m_strRotY);

		fRotY += (FLOAT) INT(nPos);
		m_spinRotY.SetPos(0);

		m_strRotY.Format( "%.2f", fRotY);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinRotZ )
	{
		FLOAT fRotZ = (FLOAT) atof(m_strRotZ);

		fRotZ += (FLOAT) INT(nPos);
		m_spinRotZ.SetPos(0);

		m_strRotZ.Format( "%.2f", fRotZ);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinScaleX )
	{
		FLOAT fScaleX = (FLOAT) atof(m_strScaleX);

		fScaleX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinScaleX.SetPos(0);

		m_strScaleX.Format( "%.2f", fScaleX);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinScaleY )
	{
		FLOAT fScaleY = (FLOAT) atof(m_strScaleY);

		fScaleY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinScaleY.SetPos(0);

		m_strScaleY.Format( "%.2f", fScaleY);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	if( pScrollBar == (CScrollBar *) &m_spinScaleZ )
	{
		FLOAT fScaleZ = (FLOAT) atof(m_strScaleZ);

		fScaleZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinScaleZ.SetPos(0);

		m_strScaleZ.Format( "%.2f", fScaleZ);
		UpdateData(FALSE);
		OnEnChangeEditData();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CMAPOBJBar::OnEnChangeEditData()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	UpdateData();

	if(pDoc)
	{
		int nCount = INT(pDoc->m_vSELOBJ.size());

		D3DXVECTOR3 vSCALE = pDoc->GetSELOBJSCALE();
		D3DXVECTOR3 vPOS = pDoc->GetSELOBJPOS();
		D3DXVECTOR3 vROT = pDoc->GetSELOBJROT();

		vSCALE.x = ((FLOAT) atof(m_strScaleX)) - vSCALE.x;
		vSCALE.y = ((FLOAT) atof(m_strScaleY)) - vSCALE.y;
		vSCALE.z = ((FLOAT) atof(m_strScaleZ)) - vSCALE.z;

		vPOS.x = ((FLOAT) atof(m_strPosX)) - vPOS.x;
		vPOS.y = ((FLOAT) atof(m_strPosY)) - vPOS.y;
		vPOS.z = ((FLOAT) atof(m_strPosZ)) - vPOS.z;

		vROT.x = ((FLOAT) atof(m_strRotX)) * D3DX_PI / 180.0f - vROT.x;
		vROT.y = ((FLOAT) atof(m_strRotY)) * D3DX_PI / 180.0f - vROT.y;
		vROT.z = ((FLOAT) atof(m_strRotZ)) * D3DX_PI / 180.0f - vROT.z;
		
		for( int i=0; i<nCount; i++)
		{
			MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[i]);
			MAPOBJECT::iterator itOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[i]);

			CTachyonObject *pOBJ = (*itOBJ).second;
			LPMAPOBJ pMAPOBJ = (*itMAPOBJ).second;

			D3DXMATRIX vScale;
			D3DXMATRIX vRotX;
			D3DXMATRIX vRotY;
			D3DXMATRIX vRotZ;
			D3DXMATRIX vPos;

			D3DXMatrixScaling(
				&vScale,
				pMAPOBJ->m_vSCALE.x + vSCALE.x,
				pMAPOBJ->m_vSCALE.y + vSCALE.y,
				pMAPOBJ->m_vSCALE.z + vSCALE.z);

			D3DXMatrixRotationX(
				&vRotX,
				pMAPOBJ->m_vROT.x + vROT.x);

			D3DXMatrixRotationY(
				&vRotY,
				pMAPOBJ->m_vROT.y + vROT.y);

			D3DXMatrixRotationZ(
				&vRotZ,
				pMAPOBJ->m_vROT.z + vROT.z);

			D3DXMatrixTranslation(
				&vPos,
				pMAPOBJ->m_vPOS.x + vPOS.x,
				pMAPOBJ->m_vPOS.y + vPOS.y,
				pMAPOBJ->m_vPOS.z + vPOS.z);

			pOBJ->m_vWorld = vRotX * vRotY * vRotZ * vPos;
			pOBJ->m_vScale = vScale;
		}

		pDoc->RedrawAllView();
	}
}

void CMAPOBJBar::OnBnClickedButtonDelete()
{
	OnObjectDelete();
}

void CMAPOBJBar::OnObjectDelete()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nCount = INT(pDoc->m_vSELOBJ.size());

	for( int i=0; i<nCount; i++)
	{
		MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[i]);
		MAPOBJECT::iterator itTOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[i]);
		MAPDWORD::iterator itOBJ = m_pItem->m_mapOBJ.find(pDoc->m_vSELOBJ[i]);

		if( itTOBJ != pDoc->m_mapOBJ.end() )
		{		
			delete (*itTOBJ).second;
			pDoc->m_mapOBJ.erase(itTOBJ);
		}

		if( itMAPOBJ != m_pItem->m_pMAP->m_mapOBJ.end() )
		{
			delete (*itMAPOBJ).second;
			m_pItem->m_pMAP->m_mapOBJ.erase(itMAPOBJ);
		}

		if( itOBJ != m_pItem->m_mapOBJ.end() )
			m_pItem->m_mapOBJ.erase(itOBJ);
	}

	pDoc->m_vSELOBJ.clear();
	UpdateItem();
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedButtonLanding()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nCount = INT(pDoc->m_vSELOBJ.size());

	VECTORDWORD vSELOBJ;
	vSELOBJ.clear();

	for( int i=0; i<nCount; i++)
		vSELOBJ.push_back(pDoc->m_vSELOBJ[i]);
	pDoc->ClearSELOBJ();

	for( i=0; i<nCount; i++)
	{
		MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(vSELOBJ[i]);
		MAPOBJECT::iterator itTOBJ = pDoc->m_mapOBJ.find(vSELOBJ[i]);

		CTachyonObject *pTOBJ = (*itTOBJ).second;
		LPMAPOBJ pMAPOBJ = (*itMAPOBJ).second;

		D3DXVECTOR3 vPOS(
			0.0f,
			pDoc->GetHeight(
			pMAPOBJ->m_vPOS.x,
			pMAPOBJ->m_vPOS.y,
			pMAPOBJ->m_vPOS.z) - pMAPOBJ->m_vPOS.y,
			0.0f);

		vPOS.y = FLOAT(INT(vPOS.y * 100.0f));
		vPOS.y /= 100.0f;

		pMAPOBJ->m_vPOS.y += vPOS.y;
		pTOBJ->m_vWorld._42 = pMAPOBJ->m_vPOS.y;

		pDoc->m_vSELOBJ.push_back(vSELOBJ[i]);
		pDoc->PushUndoBUF(
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
			vPOS,
			D3DXVECTOR3( 0.0f, 0.0f, 0.0f),
			BT_OBJ);
		pDoc->m_vSELOBJ.clear();
	}

	for( i=0; i<nCount; i++)
		pDoc->m_vSELOBJ.push_back(vSELOBJ[i]);
	vSELOBJ.clear();

	pDoc->RedrawAllView();
	UpdateItem();
}

void CMAPOBJBar::OnEnChangeEditObjfunc()
{
	if(m_pItem)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
		UpdateData();

		if(pDoc)
		{
			MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
			MAPOBJECT::iterator itOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

			CTachyonObject *pOBJ = (*itOBJ).second;
			LPMAPOBJ pMAPOBJ = (*itMAPOBJ).second;

			pMAPOBJ->m_dwOBJFuncID = m_dwOBJFUNC;
			pOBJ->m_dwOBJSNDFuncID = m_dwOBJFUNC;
		}
	}
}

void CMAPOBJBar::OnEnChangeEditActfunc()
{
	if(m_pItem)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
		UpdateData();

		if(pDoc)
		{
			MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
			MAPOBJECT::iterator itOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

			CTachyonObject *pOBJ = (*itOBJ).second;
			LPMAPOBJ pMAPOBJ = (*itMAPOBJ).second;

			pMAPOBJ->m_dwACTFuncID = m_dwACTFUNC;
			pOBJ->m_dwACTSNDFuncID = m_dwACTFUNC;
		}
	}
}

void CMAPOBJBar::OnEnChangeEditAnifunc()
{
	if(m_pItem)
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
		UpdateData();

		if(pDoc)
		{
			MAPMAPOBJ::iterator itMAPOBJ = m_pItem->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);
			MAPOBJECT::iterator itOBJ = pDoc->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

			CTachyonObject *pOBJ = (*itOBJ).second;
			LPMAPOBJ pMAPOBJ = (*itMAPOBJ).second;

			pMAPOBJ->m_dwANIFuncID = m_dwANIFUNC;
			pOBJ->m_dwANISNDFuncID = m_dwANIFUNC;
		}
	}
}

LRESULT CMAPOBJBar::OnPlayCtrlProc( WPARAM wParam, LPARAM lParam)
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	DWORD dwTick = (DWORD) wParam;

	CMAPSFXBar *pSFXBar = pDoc->GetMAPSFXBar();
	CMAPSNDBar *pSNDBar = pDoc->GetMAPSNDBar();

	dwTick -= m_dwTick;
	m_dwTick = (DWORD) wParam;

	MAPOBJECT::iterator itOBJ;
	MAPMAPSFX::iterator itSFX;
	MAPMAPSND::iterator itSND;

	for( itSFX = pDoc->m_pMAP->m_pMAP->m_mapSFX.begin(); itSFX != pDoc->m_pMAP->m_pMAP->m_mapSFX.end(); itSFX++)
	{
		MAPDWORD::iterator finder = pDoc->m_mapSFX.find((*itSFX).first);

		if( finder == pDoc->m_mapSFX.end() )
		{
			CTachyonSFX *pSFX = new CTachyonSFX();
			D3DXVECTOR3 vPOS( 0.0f, 0.0f, 0.0f);
			D3DXVECTOR3 vROT( 0.0f, 0.0f, 0.0f);
			D3DXMATRIX vRotX;
			D3DXMATRIX vRotY;
			D3DXMATRIX vRotZ;

			if(pDoc->IsSelSFX((*itSFX).first))
			{
				vPOS = D3DXVECTOR3(
					(FLOAT) atof(pSFXBar->m_strPosX),
					(FLOAT) atof(pSFXBar->m_strPosY),
					(FLOAT) atof(pSFXBar->m_strPosZ)) - pDoc->GetSELSFXPOS();

				vROT = D3DXVECTOR3(
					(FLOAT) atof(pSFXBar->m_strRotX) * D3DX_PI / 180.0f,
					(FLOAT) atof(pSFXBar->m_strRotY) * D3DX_PI / 180.0f,
					(FLOAT) atof(pSFXBar->m_strRotZ) * D3DX_PI / 180.0f) - pDoc->GetSELSFXROT();
			}

			pSFX->InitSFX(
				&CMainFrame::m_3DDevice,
				(*itSFX).second->m_pSFX);

			pSFX->m_bHasHost = TRUE;
			pSFX->m_pHost = this;

			D3DXMatrixRotationX(
				&vRotX,
				(*itSFX).second->m_vROT.x + vROT.x);

			D3DXMatrixRotationY(
				&vRotY,
				(*itSFX).second->m_vROT.y + vROT.y);

			D3DXMatrixRotationZ(
				&vRotZ,
				(*itSFX).second->m_vROT.z + vROT.z);

			pSFX->m_vGlobal = vRotX * vRotY * vRotZ;
			pSFX->m_vGlobal._41 = (*itSFX).second->m_vPOS.x + vPOS.x;
			pSFX->m_vGlobal._42 = (*itSFX).second->m_vPOS.y + vPOS.y;
			pSFX->m_vGlobal._43 = (*itSFX).second->m_vPOS.z + vPOS.z;

			DWORD dwInstID = CTachyonSFX::RegSFX( pSFX, (*itSFX).second->m_bBias);
			pDoc->m_mapSFX.insert( MAPDWORD::value_type( (*itSFX).first, dwInstID));
		}
	}

	for( itSND = pDoc->m_pMAP->m_pMAP->m_mapSND.begin(); itSND != pDoc->m_pMAP->m_pMAP->m_mapSND.end(); itSND++)
	{
		MAPDWORD::iterator finder = pDoc->m_mapSND.find((*itSND).first);

		if( finder == pDoc->m_mapSND.end() )
		{
			CD3DSound *pSND = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
				MEDIA_TSOUND,
				(*itSND).second->m_dwMediaID);
			DWORD dwIndex = pSND->Lock();
			LPDIRECTSOUND3DBUFFER p3DBUF = pSND->GetDS3D(dwIndex);

			if(p3DBUF)
			{
				D3DXVECTOR3 vPOS( 0.0f, 0.0f, 0.0f);

				if(pDoc->IsSelSND((*itSND).first))
				{
					vPOS = D3DXVECTOR3(
						(FLOAT) atof(pSNDBar->m_strPosX),
						(FLOAT) atof(pSNDBar->m_strPosY),
						(FLOAT) atof(pSNDBar->m_strPosZ)) - pDoc->GetSELSNDPOS();
				}

				p3DBUF->SetPosition(
					(*itSND).second->m_vPOS.x + vPOS.x,
					(*itSND).second->m_vPOS.y + vPOS.y,
					(*itSND).second->m_vPOS.z + vPOS.z,
					DS3D_IMMEDIATE);
			}

			pDoc->m_mapSND.insert( MAPDWORD::value_type( (*itSND).first, dwIndex));
		}
	}
	pDoc->CalcMAPTick(m_dwTick);

	for( itOBJ = pDoc->m_mapOBJ.begin(); itOBJ != pDoc->m_mapOBJ.end(); itOBJ++)
	{
		CTachyonObject *pOBJ = (*itOBJ).second;
		pOBJ->CalcTick( CMainFrame::m_3DDevice.m_pDevice, dwTick);
	}
	pDoc->CalcSFX(dwTick);
	pDoc->RedrawAllView();

	return 0;
}

LRESULT CMAPOBJBar::OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam)
{
	m_dwTick = 0;
	return 0;
}

LRESULT CMAPOBJBar::OnPlayCtrlStop( WPARAM wParam, LPARAM lParam)
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	pCtrl->m_cPlayCtrl.EnableWindow(FALSE);
	pCtrl->m_cMarker.EnableWindow(FALSE);
	pCtrl->m_cMarker.Invalidate(FALSE);
	pCtrl->m_cLoop.EnableWindow(FALSE);
	m_dwTick = 0;

	MAPDWORD::iterator itSFX;
	MAPDWORD::iterator itSND;

	for( itSND = pDoc->m_mapSND.begin(); itSND != pDoc->m_mapSND.end(); itSND++)
	{
		MAPMAPSND::iterator finder = m_pItem->m_pMAP->m_mapSND.find((*itSND).first);

		if( finder != m_pItem->m_pMAP->m_mapSND.end() )
		{
			CD3DSound *pSND = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
				MEDIA_TSOUND,
				(*finder).second->m_dwMediaID);

			CMainFrame::m_TachyonMedia.Stop(
				MEDIA_TSOUND,
				(*finder).second->m_dwMediaID,
				(*itSND).second);

			pSND->Unlock((*itSND).second);
		}
	}

	for( itSFX = pDoc->m_mapSFX.begin(); itSFX != pDoc->m_mapSFX.end(); itSFX++)
		CTachyonSFX::Release((*itSFX).second);

	pDoc->CalcMAPTick(m_dwTick);
	pDoc->m_mapSFX.clear();
	pDoc->m_mapSND.clear();
	pDoc->RedrawAllView();

	return 0;
}

LRESULT CMAPOBJBar::OnMessageBox( WPARAM wParam, LPARAM lParam)
{
	CString strMessage;
	CString strTitle;

	strMessage.LoadString(IDS_ERR_DUP_ATTRID);
	strTitle.LoadString(IDR_MAINFRAME);
	MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

	return 0;
}

void CMAPOBJBar::OnLvnItemchangedListAttr(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( (pNMLV->uChanged & LVIF_STATE) && (pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
		DWORD dwID = (DWORD) m_cATTR.GetItemData(pNMLV->iItem);
		MAPMAPOBJ::iterator itOBJ = pDoc->m_pMAP->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

		if( itOBJ != pDoc->m_pMAP->m_pMAP->m_mapOBJ.end() )
		{
			MAPATTRIBUTE::iterator itATTR = (*itOBJ).second->m_mapATTR.find(dwID);

			if( itATTR != (*itOBJ).second->m_mapATTR.end() )
				switch((*itATTR).second->m_bValueType)
				{
				case ATTRTYPE_STRING	:
					{
						m_strATTRVALUE = CString((*itATTR).second->m_pValue).Left((*itATTR).second->m_nSize);
						m_strATTRTYPE.LoadString(IDS_ATTR_STRING);
					}

					break;

				case ATTRTYPE_FLOAT		:
					{
						m_strATTRVALUE.Format( "%.2f", *((FLOAT *) (*itATTR).second->m_pValue));
						m_strATTRTYPE.LoadString(IDS_ATTR_FLOAT);
					}

					break;

				case ATTRTYPE_INT		:
					{
						m_strATTRVALUE.Format( "%d", *((int *) (*itATTR).second->m_pValue));
						m_strATTRTYPE.LoadString(IDS_ATTR_INT);
					}

					break;
				}

			UpdateData(FALSE);
		}
	}

	EnableAllCtrl(TRUE);
	*pResult = 0;
}

void CMAPOBJBar::OnEnChangeEditValue()
{
	if(m_pItem)
	{
		UpdateData();
		POSITION pos = m_cATTR.GetFirstSelectedItemPosition();

		if(pos)
		{
			CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
			int nIndex = m_cATTR.GetNextSelectedItem(pos);
			DWORD dwID = (DWORD) m_cATTR.GetItemData(nIndex);
			MAPMAPOBJ::iterator itOBJ = pDoc->m_pMAP->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

			if( itOBJ != pDoc->m_pMAP->m_pMAP->m_mapOBJ.end() )
			{
				MAPATTRIBUTE::iterator itATTR = (*itOBJ).second->m_mapATTR.find(dwID);

				switch((*itATTR).second->m_bValueType)
				{
				case ATTRTYPE_STRING	:
					{
						if((*itATTR).second->m_pValue)
							delete[] (*itATTR).second->m_pValue;

						(*itATTR).second->m_nSize = m_strATTRVALUE.GetLength();
						(*itATTR).second->m_pValue = new BYTE[(*itATTR).second->m_nSize];

						memcpy( (*itATTR).second->m_pValue, LPCSTR(m_strATTRVALUE), (*itATTR).second->m_nSize);
					}

					break;

				case ATTRTYPE_FLOAT		:
					{
						if((*itATTR).second->m_pValue)
							delete[] (*itATTR).second->m_pValue;

						(*itATTR).second->m_pValue = new BYTE[sizeof(FLOAT)];
						(*itATTR).second->m_nSize = sizeof(FLOAT);

						*((FLOAT *) (*itATTR).second->m_pValue) = (FLOAT) atof(m_strATTRVALUE);
					}

					break;

				case ATTRTYPE_INT		:
					{
						if((*itATTR).second->m_pValue)
							delete[] (*itATTR).second->m_pValue;

						(*itATTR).second->m_pValue = new BYTE[sizeof(int)];
						(*itATTR).second->m_nSize = sizeof(int);

						*((int *) (*itATTR).second->m_pValue) = atoi(m_strATTRVALUE);
					}

					break;
				}
			}
		}
	}
}

void CMAPOBJBar::OnBnClickedButtonHide()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nCount = INT(pDoc->m_vSELOBJ.size());

	for( int i=0; i<nCount; i++)
		pDoc->AddHIDEOBJ(pDoc->m_vSELOBJ[i]);

	pDoc->ClearSELOBJ();
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedButtonFreeze()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	int nCount = INT(pDoc->m_vSELOBJ.size());

	for( int i=0; i<nCount; i++)
		pDoc->AddFREEZEOBJ(pDoc->m_vSELOBJ[i]);

	pDoc->ClearSELOBJ();
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedButtonUnfreezeall()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	pDoc->m_mapFREEZE.clear();
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedButtonUnhideall()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	pDoc->m_mapHIDE.clear();
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnEnChangeEditObjbrushSize()
{
	if( m_pItem )
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
		UpdateData();

		pDoc->RedrawAllView();
	}
}

void CMAPOBJBar::OnCbnSelchangeComboObjbrushShape()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	UpdateData();

	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnEnChangeEditObjcount()
{
	if( m_pItem )
	{
		CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
		UpdateData();
	}
}

void CMAPOBJBar::OnBnClickedCheckInsertobjbybrush()
{
	m_bInsertOBJBrush = !m_bInsertOBJBrush;
	EnableAllCtrl(TRUE);	
}

void CMAPOBJBar::OnBnClickedButtonObjectExchange()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	CObjectExchangeDlg dlg;

	MAPMAPOBJ::iterator itFindOBJID;
	MAPOBJECT::iterator itOBJ;

	if( pDoc->m_vSELOBJ.size() > 1 )
	{
		AfxMessageBox(_T("Too Many OBJ Selected"));
		return;
	}
	if( pDoc->m_vSELOBJ.empty() )
	{
		AfxMessageBox(_T("Not OBJ Selected"));
		return;
	}

	itFindOBJID = pDoc->m_pMAP->m_pMAP->m_mapOBJ.find( pDoc->m_vSELOBJ[0] );
	
	if( itFindOBJID == pDoc->m_pMAP->m_pMAP->m_mapOBJ.end() )
		return;

	(*itFindOBJID).second->m_vSCALE.x = ((FLOAT) atof(m_strScaleX));
	(*itFindOBJID).second->m_vSCALE.y = ((FLOAT) atof(m_strScaleY));
	(*itFindOBJID).second->m_vSCALE.z = ((FLOAT) atof(m_strScaleZ));

	(*itFindOBJID).second->m_vPOS.x = ((FLOAT) atof(m_strPosX));
	(*itFindOBJID).second->m_vPOS.y = ((FLOAT) atof(m_strPosY));
	(*itFindOBJID).second->m_vPOS.z = ((FLOAT) atof(m_strPosZ));

	(*itFindOBJID).second->m_vROT.x = ((FLOAT) atof(m_strRotX)) * D3DX_PI / 180.0f;
	(*itFindOBJID).second->m_vROT.y = ((FLOAT) atof(m_strRotY)) * D3DX_PI / 180.0f;
	(*itFindOBJID).second->m_vROT.z = ((FLOAT) atof(m_strRotZ)) * D3DX_PI / 180.0f;
	
	if( dlg.DoModal() == IDOK )
	{
		switch( dlg.m_bOBJExchange )
		{
		case 0:
			if( MessageBox( "Replace?", NULL, MB_OKCANCEL ) == IDOK )
				pDoc->ObjectReplace(dlg.m_bAllOBJ);
			break;
		case 1:
			if( MessageBox( "Attach?", NULL, MB_OKCANCEL ) == IDOK )
				pDoc->ObjectAttach(dlg.m_bAllOBJ);
			break;
		}
	}
}

void CMAPOBJBar::OnBnClickedButtonResetobjAttri()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	pDoc->ResetOBJATTR();
}

void CMAPOBJBar::OnBnClickedButtonAdjustObjectPos()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	CPoint* pt = new CPoint;
	CAdjustOBJPosDlg dlg(pDoc);
	if (dlg.DoModal() == IDOK)
	{
		(*pt) = dlg.GetUnitXZ();
		if( (*pt) == CPoint(-1,-1) )
			pt = NULL;
		pDoc->AdjustObjectPos(pt, &dlg.GetOBJMoveOffset(), dlg.GetAttachType());
	}
}

void CMAPOBJBar::ResetSELOBJState()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	D3DXVECTOR3 vSELPOS = pDoc->GetSELOBJPOS();
	D3DXVECTOR3 vSELROT = pDoc->GetSELOBJROT();
	D3DXVECTOR3 vSELSCALE = pDoc->GetSELOBJSCALE();

	m_strPosX.Format("%.4f",vSELPOS.x);		
	m_strPosY.Format("%.4f",vSELPOS.y);
	m_strPosZ.Format("%.4f",vSELPOS.z);

	m_strRotX.Format("%.2f",vSELROT.x * 180.0f / D3DX_PI);		
	m_strRotY.Format("%.2f",vSELROT.y * 180.0f / D3DX_PI);
	m_strRotZ.Format("%.2f",vSELROT.z * 180.0f / D3DX_PI);

	m_strScaleX.Format("%.2f",vSELSCALE.x );		
	m_strScaleY.Format("%.2f",vSELSCALE.y );
	m_strScaleZ.Format("%.2f",vSELSCALE.z );

	if(!m_cInsertMode.GetCheck())
		m_strSELOBJCount.Format("%d",pDoc->m_vSELOBJ.size());
}

void CMAPOBJBar::OnBnClickedButtonFindobj()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();
	CMainFrame *pTFRAME = (CMainFrame *) AfxGetMainWnd();

	if( pTFRAME && pTFRAME->m_pDocObject && pDoc && INT(pDoc->m_vSELOBJ.size()) == 1 )
	{
		MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapOBJ.find(pDoc->m_vSELOBJ[0]);

		if( itID != pDoc->m_pMAP->m_mapOBJ.end() )
		{
			COBJItem *pOBJ = CItemBase::GetOBJItem((*itID).second);

			if(pOBJ)
			{
				CTreeCtrl& cTree = pTFRAME->m_pDocObject->GetTreeCtrl();

				pTFRAME->m_wndWorkSpace.SetActiveView(5);
				cTree.SelectItem(pOBJ->m_itemPos);
			}
		}
	}
}

void CMAPOBJBar::OnBnClickedButtonHidecoll()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	MAPOBJECT::iterator itor = pDoc->m_mapOBJ.begin();
	while( itor != pDoc->m_mapOBJ.end() )
	{
		CTachyonObject* pTOBJ = (*itor).second;
		MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapOBJ.find( (*itor).first );
		if( itID != pDoc->m_pMAP->m_mapOBJ.end() )
		{
			COBJItem* pOBJ = m_pItem->GetOBJItem( (*itID).second );
			if( pOBJ )
			{
				DWORD dwTypeID = pOBJ->GetAttributeID("ID_OBJ_TYPE");
				int nType = pTOBJ->GetAttrINT( dwTypeID );

				if( nType == 6 ) // OT_COLLISION
				{
					pDoc->AddHIDEOBJ( (*itor++).first );
					continue;
				}
			}
		}
		++itor;
	}

	pDoc->ClearSELOBJ();
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedButtonUnhidecoll()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	MAPDWORD::iterator it = pDoc->m_mapHIDE.begin();
	while( it != pDoc->m_mapHIDE.end() )
	{
		MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapOBJ.find((*it).first);
		MAPOBJECT::iterator finder = pDoc->m_mapOBJ.find((*it).first);

		MAPDWORD::iterator itNEXT = it;
		itNEXT++;

		if( itID != pDoc->m_pMAP->m_mapOBJ.end() &&
			finder != pDoc->m_mapOBJ.end() )
		{
			COBJItem *pOBJ = m_pItem->GetOBJItem((*itID).second);
			CTachyonObject *pTOBJ = (*finder).second;

			if( pTOBJ && pOBJ && pTOBJ->GetAttrINT(pOBJ->GetAttributeID("ID_OBJ_TYPE")) == 6 )
				pDoc->m_mapHIDE.erase(it);
		}

		it = itNEXT;
	}

	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedButtonHidesilh()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	MAPOBJECT::iterator itor = pDoc->m_mapOBJ.begin();
	while( itor != pDoc->m_mapOBJ.end() )
	{
		CTachyonObject* pTOBJ = (*itor).second;
		MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapOBJ.find( (*itor).first );
		if( itID != pDoc->m_pMAP->m_mapOBJ.end() )
		{
			COBJItem* pOBJ = m_pItem->GetOBJItem( (*itID).second );
			if( pOBJ )
			{
				DWORD dwTypeID = pOBJ->GetAttributeID("ID_OBJ_TYPE");
				int nType = pTOBJ->GetAttrINT( dwTypeID );

				if( nType == 12 ) // OT_SILHOUETTE
				{
					pDoc->AddHIDEOBJ( (*itor++).first );
					continue;
				}
			}
		}
		++itor;
	}

	pDoc->ClearSELOBJ();
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedButtonUnhidesilh()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	MAPDWORD::iterator it = pDoc->m_mapHIDE.begin();
	while( it != pDoc->m_mapHIDE.end() )
	{
		MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapOBJ.find((*it).first);
		MAPOBJECT::iterator finder = pDoc->m_mapOBJ.find((*it).first);

		MAPDWORD::iterator itNEXT = it;
		itNEXT++;

		if( itID != pDoc->m_pMAP->m_mapOBJ.end() &&
			finder != pDoc->m_mapOBJ.end() )
		{
			COBJItem *pOBJ = m_pItem->GetOBJItem((*itID).second);
			CTachyonObject *pTOBJ = (*finder).second;

			if( pTOBJ && pOBJ && pTOBJ->GetAttrINT(pOBJ->GetAttributeID("ID_OBJ_TYPE")) == 12 )
				pDoc->m_mapHIDE.erase(it);
		}

		it = itNEXT;
	}

	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedButtonHidewater()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	MAPOBJECT::iterator itor = pDoc->m_mapOBJ.begin();
	while( itor != pDoc->m_mapOBJ.end() )
	{
		CTachyonObject* pTOBJ = (*itor).second;
		MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapOBJ.find( (*itor).first );
		if( itID != pDoc->m_pMAP->m_mapOBJ.end() )
		{
			COBJItem* pOBJ = m_pItem->GetOBJItem( (*itID).second );
			if( pOBJ )
			{
				DWORD dwTypeID = pOBJ->GetAttributeID("ID_OBJ_TYPE");
				int nType = pTOBJ->GetAttrINT( dwTypeID );

				if( nType == 5 ) // OT_WATER
				{
					pDoc->AddHIDEOBJ( (*itor++).first );
					continue;
				}
			}
		}
		++itor;
	}

	pDoc->ClearSELOBJ();
	UpdateData(FALSE);
	EnableAllCtrl(TRUE);
	pDoc->RedrawAllView();
}

void CMAPOBJBar::OnBnClickedButtonUnhidewater()
{
	CMAPDoc *pDoc = (CMAPDoc *) m_pItem->GetMainDocument();

	MAPDWORD::iterator it = pDoc->m_mapHIDE.begin();
	while( it != pDoc->m_mapHIDE.end() )
	{
		MAPDWORD::iterator itID = pDoc->m_pMAP->m_mapOBJ.find((*it).first);
		MAPOBJECT::iterator finder = pDoc->m_mapOBJ.find((*it).first);

		MAPDWORD::iterator itNEXT = it;
		itNEXT++;

		if( itID != pDoc->m_pMAP->m_mapOBJ.end() &&
			finder != pDoc->m_mapOBJ.end() )
		{
			COBJItem *pOBJ = m_pItem->GetOBJItem((*itID).second);
			CTachyonObject *pTOBJ = (*finder).second;

			if( pTOBJ && pOBJ && pTOBJ->GetAttrINT(pOBJ->GetAttributeID("ID_OBJ_TYPE")) == 5 )
				pDoc->m_mapHIDE.erase(it);
		}

		it = itNEXT;
	}

	pDoc->RedrawAllView();}
