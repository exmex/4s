// SFXBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "FramesetDlg.h"
#include "MainFrm.h"
#include "SFXFrame.h"
#include "SFXDoc.h"
#include "SFXBar.h"
#include ".\sfxbar.h"


// CSFXBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSFXBar, CBarContent)
CSFXBar::CSFXBar(CWnd* pParent /*=NULL*/)
: CBarContent(CSFXBar::IDD, pParent)
, m_nSFXTYPE(FALSE)
, m_nCLK(0)
, m_nCL(0)
, m_nMESH(0)
, m_nACT(0)
, m_nANI(0)
, m_strSizeX(_T(""))
, m_strSizeY(_T(""))
, m_strSFXSRC(_T(""))
, m_strANISRC(_T(""))
, m_dwTotalFrame(0)
, m_dwLifeTime(0)
, m_strKey(_T(""))
, m_dwFrame(0)
, m_dwValueA(0)
, m_dwValueR(0)
, m_dwValueG(0)
, m_dwValueB(0)
, m_dwOBJFuncID(0)
, m_dwACTFuncID(0)
, m_dwANIFuncID(0)
, m_bZEnable(FALSE)
, m_bZWrite(FALSE)
, m_dwCurLevel(6)
, m_dwSelLevel(6)
, m_bOptionLodLevel(CTachyonSFX::m_bOptionLodLevel)
, m_dwFPS(60)
{
	m_pItem = NULL;
	m_dwTick = 0;
	m_dwCount = 0;
}

CSFXBar::~CSFXBar()
{
}

void CSFXBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_PARTICLE, m_cParticle);
	DDX_Control(pDX, IDC_RADIO_SINGLE, m_cSingle);
	DDX_Control(pDX, IDC_BUTTON_SETPARTICLE, m_cSetParticle);
	DDX_Radio(pDX, IDC_RADIO_SINGLE, m_nSFXTYPE);
	DDX_Control(pDX, IDC_EDIT_SRC, m_cSFXSRC);
	DDX_Control(pDX, IDC_COMBO_CLK, m_cCLK);
	DDX_Control(pDX, IDC_COMBO_CL, m_cCL);
	DDX_Control(pDX, IDC_COMBO_MESH, m_cMESH);
	DDX_Control(pDX, IDC_COMBO_ACT, m_cACT);
	DDX_Control(pDX, IDC_COMBO_ANI, m_cANI);
	DDX_Control(pDX, IDC_EDIT_SIZEX, m_cSizeX);
	DDX_Control(pDX, IDC_EDIT_SIZEY, m_cSizeY);
	DDX_Control(pDX, IDC_SPIN_SIZEX, m_spinSizeX);
	DDX_Control(pDX, IDC_SPIN_SIZEY, m_spinSizeY);
	DDX_Control(pDX, IDC_EDIT_ANISRC, m_cANISRC);
	DDX_Control(pDX, IDC_BUTTON_DELANISRC, m_cDELAS);
	DDX_Control(pDX, IDC_BUTTON_TIMESET, m_cTimeSet);
	DDX_Control(pDX, IDC_STATIC_TOTAL_FRAME, m_cTotalFrame);
	DDX_Control(pDX, IDC_EDIT_LIFETIME, m_cLifeTime);
	DDX_Control(pDX, IDC_SPIN_LIFETIME, m_spinLifeTime);
	DDX_Control(pDX, IDC_STATIC_KEY, m_cKey);
	DDX_Control(pDX, IDC_SPIN_KEY, m_spinKey);
	DDX_Control(pDX, IDC_STATIC_FRAME, m_cFrame);
	DDX_Control(pDX, IDC_EDIT_A, m_cA);
	DDX_Control(pDX, IDC_SPIN_A, m_spinA);
	DDX_Control(pDX, IDC_EDIT_R, m_cR);
	DDX_Control(pDX, IDC_SPIN_R, m_spinR);
	DDX_Control(pDX, IDC_EDIT_G, m_cG);
	DDX_Control(pDX, IDC_SPIN_G, m_spinG);
	DDX_Control(pDX, IDC_EDIT_B, m_cB);
	DDX_Control(pDX, IDC_SPIN_B, m_spinB);
	DDX_Control(pDX, IDC_EDIT_OPLEVEL, m_cOptionLevel);
	DDX_Control(pDX, IDC_SPIN_OPLEVEL, m_spinOptionLevel);
	DDX_Control(pDX, IDC_EDIT_LODLEVEL, m_cLodLevel);
	DDX_Control(pDX, IDC_SPIN_LODLEVEL, m_spinLodLevel);
	DDX_Control(pDX, IDC_EDIT_CURLODLEVEL, m_cCurLodLevel);
	DDX_Control(pDX, IDC_SPIN_CURLODLEVEL, m_spinCurLodLevel);
	DDX_Control(pDX, IDC_EDIT_SETFPS, m_cSetFPS);
	DDX_Control(pDX, IDC_SPIN_SETFPS, m_spinFPS);
	DDX_Control(pDX, IDC_BUTTON_ADDKEY, m_cAddKey);
	DDX_Control(pDX, IDC_BUTTON_DELETEKEY, m_cDelKey);
	DDX_Control(pDX, IDC_LIST_COMBINE, m_cCSFX);
	DDX_CBIndex(pDX, IDC_COMBO_CLK, m_nCLK);
	DDX_CBIndex(pDX, IDC_COMBO_CL, m_nCL);
	DDX_CBIndex(pDX, IDC_COMBO_MESH, m_nMESH);
	DDX_CBIndex(pDX, IDC_COMBO_ACT, m_nACT);
	DDX_CBIndex(pDX, IDC_COMBO_ANI, m_nANI);
	DDX_Text(pDX, IDC_EDIT_SIZEX, m_strSizeX);
	DDX_Text(pDX, IDC_EDIT_SIZEY, m_strSizeY);
	DDX_Text(pDX, IDC_EDIT_SRC, m_strSFXSRC);
	DDX_Text(pDX, IDC_EDIT_ANISRC, m_strANISRC);
	DDX_Text(pDX, IDC_STATIC_TOTAL_FRAME, m_dwTotalFrame);
	DDX_Text(pDX, IDC_EDIT_LIFETIME, m_dwLifeTime);
	DDX_Text(pDX, IDC_STATIC_KEY, m_strKey);
	DDX_Text(pDX, IDC_STATIC_FRAME, m_dwFrame);
	DDX_Text(pDX, IDC_EDIT_A, m_dwValueA);
	DDX_Text(pDX, IDC_EDIT_R, m_dwValueR);
	DDX_Text(pDX, IDC_EDIT_G, m_dwValueG);
	DDX_Text(pDX, IDC_EDIT_B, m_dwValueB);
	DDX_Text(pDX, IDC_EDIT_OPLEVEL, m_bOptionLodLevel);
	DDX_Text(pDX, IDC_EDIT_LODLEVEL, m_dwSelLevel);
	DDX_Text(pDX, IDC_EDIT_CURLODLEVEL, m_dwCurLevel);
	DDX_Text(pDX, IDC_EDIT_SETFPS, m_dwFPS);
	DDX_Control(pDX, IDC_BUTTON_DELSRC, m_cDELSRC);
	DDX_Control(pDX, IDC_EDIT_OBJFUNC, m_cOBJFuncID);
	DDX_Text(pDX, IDC_EDIT_OBJFUNC, m_dwOBJFuncID);
	DDX_Control(pDX, IDC_SPIN_OBJFUNC, m_spinOBJFuncID);
	DDX_Control(pDX, IDC_EDIT_ACTFUNC, m_cACTFuncID);
	DDX_Text(pDX, IDC_EDIT_ACTFUNC, m_dwACTFuncID);
	DDX_Control(pDX, IDC_SPIN_ACTFUNC, m_spinACTFuncID);
	DDX_Control(pDX, IDC_EDIT_ANIFUNC, m_cANIFuncID);
	DDX_Text(pDX, IDC_EDIT_ANIFUNC, m_dwANIFuncID);
	DDX_Control(pDX, IDC_SPIN_ANIFUNC, m_spinANIFuncID);
	DDX_Control(pDX, IDC_CHECK_ZENABLE, m_cZEnable);
	DDX_Check(pDX, IDC_CHECK_ZENABLE, m_bZEnable);
	DDX_Control(pDX, IDC_CHECK_ZWRITE, m_cZWrite);
	DDX_Check(pDX, IDC_CHECK_ZWRITE, m_bZWrite);
	DDX_Control(pDX, IDC_COMBO_BLENDOP, m_cBlendOP);
	DDX_Control(pDX, IDC_COMBO_SRCBLEND, m_cSRCBlend);
	DDX_Control(pDX, IDC_COMBO_DESTBLEND, m_cDESTBlend);
}


BEGIN_MESSAGE_MAP(CSFXBar, CBarContent)
	ON_MESSAGE( WM_PLAYCTRL_PROC, OnPlayCtrlProc)
	ON_MESSAGE( WM_PLAYCTRL_PLAY, OnPlayCtrlPlay)
	ON_MESSAGE( WM_PLAYCTRL_STOP, OnPlayCtrlStop)
	ON_MESSAGE( WM_PLAYCTRL_POS, OnPlayCtrlPos)
	ON_MESSAGE( WM_MARKER_MOVE, OnKeyMove)
	ON_MESSAGE( WM_MARKER_COPY, OnKeyCopy)
	ON_BN_CLICKED(IDC_BUTTON_SETPARTICLE, OnBnClickedButtonSetparticle)
	ON_BN_CLICKED(IDC_RADIO_PARTICLE, OnBnClickedRadioParticle)
	ON_BN_CLICKED(IDC_RADIO_SINGLE, OnBnClickedRadioSingle)
	ON_EN_CHANGE(IDC_EDIT_SIZEX, OnEnChangeEditSizex)
	ON_EN_CHANGE(IDC_EDIT_SIZEY, OnEnChangeEditSizey)
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_DELSRC, OnBnClickedButtonDelsrc)
	ON_BN_CLICKED(IDC_BUTTON_DELANISRC, OnBnClickedButtonDelanisrc)
	ON_CBN_SELCHANGE(IDC_COMBO_CLK, OnCbnSelchangeComboClk)
	ON_CBN_SELCHANGE(IDC_COMBO_CL, OnCbnSelchangeComboCl)
	ON_CBN_SELCHANGE(IDC_COMBO_MESH, OnCbnSelchangeComboMesh)
	ON_CBN_SELCHANGE(IDC_COMBO_ACT, OnCbnSelchangeComboAct)
	ON_CBN_SELCHANGE(IDC_COMBO_ANI, OnCbnSelchangeComboAni)
	ON_EN_CHANGE(IDC_EDIT_LIFETIME, OnEnChangeEditLifetime)
	ON_BN_CLICKED(IDC_BUTTON_ADDKEY, OnBnClickedButtonAddkey)
	ON_BN_CLICKED(IDC_BUTTON_DELETEKEY, OnBnClickedButtonDeletekey)
	ON_EN_CHANGE(IDC_EDIT_A, OnEnChangeEditA)
	ON_EN_CHANGE(IDC_EDIT_R, OnEnChangeEditR)
	ON_EN_CHANGE(IDC_EDIT_G, OnEnChangeEditG)
	ON_EN_CHANGE(IDC_EDIT_B, OnEnChangeEditB)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_COMBINE, OnLvnBegindragListCombine)
	ON_BN_CLICKED(IDC_BUTTON_TIMESET, OnBnClickedButtonTimeset)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnBnClickedButtonMoveup)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, OnBnClickedButtonMovedown)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_SFX, OnBnClickedButtonDeleteSfx)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_COMBINE, OnLvnItemActivateListCombine)
	ON_EN_CHANGE(IDC_EDIT_OBJFUNC, OnEnChangeEditObjfunc)
	ON_EN_CHANGE(IDC_EDIT_ACTFUNC, OnEnChangeEditActfunc)
	ON_EN_CHANGE(IDC_EDIT_ANIFUNC, OnEnChangeEditAnifunc)
	ON_BN_CLICKED(IDC_CHECK_ZENABLE, OnBnClickedCheckZenable)
	ON_BN_CLICKED(IDC_CHECK_ZWRITE, OnBnClickedCheckZwrite)
	ON_CBN_SELCHANGE(IDC_COMBO_BLENDOP, OnCbnSelchangeComboBlendop)
	ON_CBN_SELCHANGE(IDC_COMBO_DESTBLEND, OnCbnSelchangeComboDestblend)
	ON_CBN_SELCHANGE(IDC_COMBO_SRCBLEND, OnCbnSelchangeComboSrcblend)
	ON_EN_CHANGE(IDC_EDIT_LODLEVEL, OnEnChangeEditLodlevel)
	ON_EN_CHANGE(IDC_EDIT_CURLODLEVEL, OnEnChangeEditCurlodlevel)
	ON_NOTIFY(NM_CLICK, IDC_LIST_COMBINE, OnNMClickListCombine)
	ON_EN_CHANGE(IDC_EDIT_OPLEVEL, OnEnChangeEditGloballodlevel)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST_COMBINE, OnNMKillfocusListCombine)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_LIST_COMBINE, OnLvnBeginlabeleditListCombine)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_LIST_COMBINE, OnLvnEndlabeleditListCombine)
	ON_EN_CHANGE(IDC_EDIT_SETFPS, OnEnChangeEditSetfps)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_COMBINE, OnLvnItemchangedListCombine)
	ON_NOTIFY(LVN_KEYDOWN, IDC_LIST_COMBINE, OnLvnKeydownListCombine)
	ON_BN_CLICKED(IDC_BUTTON_FINDSFX, OnBnClickedButtonFindsfx)
END_MESSAGE_MAP()


// CSFXBar 메시지 처리기입니다.

void CSFXBar::OnBnClickedButtonSetparticle()
{
	CSFXFrame *pFrame = (CSFXFrame *) GetParentFrame();

	pFrame->m_wndControlBar.ShowWindow(SW_HIDE);
	pFrame->m_wndParticleBar.ShowWindow(SW_SHOW);
	pFrame->DockControlBar(&pFrame->m_wndParticleBar);
	pFrame->RepositionBars( 0, 0xFFFFFFFF, 0);
}

BOOL CSFXBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEL);
	m_cDELSRC.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_DEL);
	m_cDELAS.SetIcon(hIcon);

	CListCtrl *pListCtrl = m_cCSFX.GetListCtrl();
	pListCtrl->GetClientRect(&rect);
//	rect.DeflateRect( 1, 0);

	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	pListCtrl->InsertColumn( 0, "Name", LVCFMT_LEFT, 80 );
	pListCtrl->InsertColumn( 1, "LOD", LVCFMT_LEFT, 35 );

	m_cCSFX.m_DropTarget.Register(&m_cCSFX);
	m_cCSFX.m_pDragEnter = OnSFXDragOver;
	m_cCSFX.m_pDragOver = OnSFXDragOver;
	m_cCSFX.m_pDrop = OnSFXDrop;
	m_cCSFX.m_pLeave = OnSFXDragLeave;

	m_cSFXSRC.m_DropTarget.Register(&m_cSFXSRC);
	m_cSFXSRC.m_pDragEnter = OnSRCDragOver;
	m_cSFXSRC.m_pDragOver = OnSRCDragOver;
	m_cSFXSRC.m_pDrop = OnSRCDrop;
	m_cSFXSRC.m_pLeave = OnSRCDragLeave;

	m_cANISRC.m_DropTarget.Register(&m_cANISRC);
	m_cANISRC.m_pDragEnter = OnANIDragOver;
	m_cANISRC.m_pDragOver = OnANIDragOver;
	m_cANISRC.m_pDrop = OnANIDrop;
	m_cANISRC.m_pLeave = OnANIDragLeave;

	m_spinOBJFuncID.SetRange( 0, 10000);
	m_spinACTFuncID.SetRange( 0, 10000);
	m_spinANIFuncID.SetRange( 0, 10000);
	m_spinLifeTime.SetRange( 0, 10000);
	m_spinSizeX.SetRange( -100, 100);
	m_spinSizeY.SetRange( -100, 100);
	m_spinKey.SetRange( -100, 100);
	m_spinA.SetRange( 0, 255);
	m_spinR.SetRange( 0, 255);
	m_spinG.SetRange( 0, 255);
	m_spinB.SetRange( 0, 255);
	m_spinOptionLevel.SetRange( 1, 6);
	m_spinLodLevel.SetRange( 1, 6);
	m_spinCurLodLevel.SetRange( 1, 6);
	m_spinFPS.SetRange( 1, 1000);

	m_FrameRender.SetFramePerSec( (float)m_dwFPS );

	return TRUE;
}

void CSFXBar::InitSFX()
{
	CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	m_pItem->m_TSFX.InitSFX(
		&CMainFrame::m_3DDevice,
		&m_pItem->m_SFX);

	switch(pDoc->m_pItem->m_SFX.m_bSFXType)
	{
	case SFX_PARTICLE	:
		{
			pCtrl->SetPos(0);
			UpdateCtrl(0);
		}

		break;

	case SFX_SINGLE		: UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos()); break;
	}

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void CSFXBar::SetItem( CSFXItem *pItem)
{
	static int nBlendOP[] = {
		0,		// D3DBLENDOP_ADD
		0,		// D3DBLENDOP_ADD
		1,		// D3DBLENDOP_SUBTRACT
		2,		// D3DBLENDOP_REVSUBTRACT
		3,		// D3DBLENDOP_MIN
		4};		// D3DBLENDOP_MAX

	static int nBlend[] = {
		0,		// D3DBLEND_ZERO
		0,		// D3DBLEND_ZERO
		1,		// D3DBLEND_ONE
		2,		// D3DBLEND_SRCCOLOR
		3,		// D3DBLEND_INVSRCCOLOR
		4,		// D3DBLEND_SRCALPHA
		5,		// D3DBLEND_INVSRCALPHA
		6,		// D3DBLEND_DESTALPHA
		7,		// D3DBLEND_INVDESTALPHA
		8,		// D3DBLEND_DESTCOLOR
		9,		// D3DBLEND_INVDESTCOLOR
		10,		// D3DBLEND_SRCALPHASAT
		11,		// D3DBLEND_BOTHSRCALPHA
		12,		// D3DBLEND_BOTHINVSRCALPHA
		13,		// D3DBLEND_BLENDFACTOR
		14};	// D3DBLEND_INVBLENDFACTOR

	CSFXDoc *pDoc = (CSFXDoc *) pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	m_pItem = pItem;
	switch(m_pItem->m_SFX.m_bSRCType)
	{
	case SFXSRC_OBJ	:
		{
			COBJItem *pItem = CItemBase::GetOBJItem(m_pItem->m_dwSFXSRC);

			if(pItem)
			{
				SetCLKList(pItem);
				SetACTList(pItem);

				m_strSFXSRC = pItem->m_strName;
			}
			else
				m_strSFXSRC.LoadString(IDS_NOTUSE);
		}

		break;

	case SFXSRC_SFX	:
		{
			CSFXItem *pItem = CItemBase::GetSFXItem(m_pItem->m_dwSFXSRC);

			if(pItem)
				m_strSFXSRC = pItem->m_strName;
			else
				m_strSFXSRC.LoadString(IDS_NOTUSE);
		}

		break;

	case SFXSRC_TEX	:
		{
			CTextureItem *pItem = CItemBase::GetTextureItem(m_pItem->m_dwSFXSRC);

			if(pItem)
				m_strSFXSRC = pItem->m_strName;
			else
				m_strSFXSRC.LoadString(IDS_NOTUSE);
		}

		break;
	}

	m_cDESTBlend.SetCurSel(nBlend[m_pItem->m_SFX.m_dwDESTBlend]);
	m_cSRCBlend.SetCurSel(nBlend[m_pItem->m_SFX.m_dwSRCBlend]);
	m_cBlendOP.SetCurSel(nBlendOP[m_pItem->m_SFX.m_dwBlendOP]);

	m_strSizeX.Format( "%.2f", m_pItem->m_SFX.m_fSizeX);
	m_strSizeY.Format( "%.2f", m_pItem->m_SFX.m_fSizeY);
	m_bZEnable = m_pItem->m_SFX.m_bZEnable;
	m_bZWrite = m_pItem->m_SFX.m_bZWrite;

	CAnimationItem *pANI = CItemBase::GetAnimationItem(m_pItem->m_dwANISRC);
	if(pANI)
		m_strANISRC = pANI->m_strName;
	else
		m_strANISRC.LoadString(IDS_NOTUSE);

	m_dwTotalFrame = m_pItem->m_dwTotalFrame;
	m_dwLifeTime = m_pItem->m_SFX.m_dwLifeTime;
	m_nSFXTYPE = m_pItem->m_SFX.m_bSFXType;

	m_dwOBJFuncID = m_pItem->m_SFX.m_dwOBJFuncID;
	m_dwACTFuncID = m_pItem->m_SFX.m_dwACTFuncID;
	m_dwANIFuncID = m_pItem->m_SFX.m_dwANIFuncID;

	CListCtrl *pListCtrl = m_cCSFX.GetListCtrl();
	int nCount = INT(m_pItem->m_SFX.m_vKey.size());

	pCtrl->EnableCtrl(pANI ? TRUE : FALSE);
	pCtrl->SetRange( 0, m_dwTotalFrame);
	pCtrl->m_cMarker.DelAll();
	pListCtrl->DeleteAllItems();

	for( int i=0; i<nCount; i++)
		pCtrl->m_cMarker.AddMark(GetPosFromTick(m_pItem->m_SFX.m_vKey[i]->m_dwTick));

	nCount = INT(m_pItem->m_vSFXID.size());
	for( i=0; i<nCount; i++)
	{
		CSFXItem *pSFX = CItemBase::GetSFXItem(m_pItem->m_vSFXID[i]);
        int nIndex = pListCtrl->InsertItem( i, pSFX->m_strName);
		pListCtrl->SetItemData( nIndex, (DWORD_PTR) pSFX);
		CString str;
		str.Format( "Lv %d", pSFX->m_SFX.m_dwLodLevel );
		pListCtrl->SetItemText( i, 1, str );
	}
//	m_cCSFX.UpdateColumnWidth();

	if( m_pItem )
		m_dwCurLevel = m_pItem->m_SFX.m_dwLodLevel;

	UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	EnableAllCtrl(TRUE);
}

void CSFXBar::SetCLKList( COBJItem *pItem)
{
	MAPCLKIND::iterator it;

	m_cMESH.ResetContent();
	m_cCLK.ResetContent();
	m_cCL.ResetContent();

	int nIndex = m_cCLK.InsertString( 0, "NONE");
	m_cCLK.SetItemData( nIndex, 0);

	for( it = pItem->m_OBJ.m_mapCLKIND.begin(); it != pItem->m_OBJ.m_mapCLKIND.end(); it++)
	{
		MAPSTR::iterator finder = pItem->m_mapNAME.find((*it).first);

		nIndex = m_cCLK.InsertString( m_cCLK.GetCount(), (*finder).second);
		m_cCLK.SetItemData( nIndex, (*it).first);

		if( (*it).first == m_pItem->m_dwCurCLK )
		{
			m_cCLK.SetCurSel(nIndex);
			OnCbnSelchangeComboClk();
		}
	}

	if( m_cCLK.GetCurSel() == -1 )
	{
		m_cCLK.SetCurSel(0);
		OnCbnSelchangeComboClk();
	}

	UpdateData();
}

void CSFXBar::SetACTList( COBJItem *pItem)
{
	MAPACTION::iterator it;

	m_cACT.ResetContent();
	m_cANI.ResetContent();

	int nIndex = m_cACT.InsertString( 0, "NONE");
	m_cACT.SetItemData( nIndex, 0);

	for( it = pItem->m_OBJ.m_mapACTION.begin(); it != pItem->m_OBJ.m_mapACTION.end(); it++)
	{
		MAPSTR::iterator finder = pItem->m_mapNAME.find((*it).first);

		nIndex = m_cACT.InsertString( m_cACT.GetCount(), (*finder).second);
		m_cACT.SetItemData( nIndex, (*it).first);

		if( (*it).first == m_pItem->m_SFX.m_dwACT )
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

	UpdateData();
}

void CSFXBar::OnCbnSelchangeComboClk()
{
	COBJItem *pOBJ = CItemBase::GetOBJItem(m_pItem->m_dwSFXSRC);
	int nIndex = m_cCLK.GetCurSel();

	m_cMESH.ResetContent();
	m_cCL.ResetContent();

	if( nIndex > 0 && pOBJ )
	{
		DWORD dwCLK = (DWORD) m_cCLK.GetItemData(nIndex);
		MAPCLKIND::iterator itCLK = m_pItem->m_SFX.m_SRC.m_pOBJ->m_mapCLKIND.find(dwCLK);

		if( itCLK != m_pItem->m_SFX.m_SRC.m_pOBJ->m_mapCLKIND.end() )
		{
			MAPCLKINST::iterator itCI = m_pItem->m_SFX.m_mapCLK.find(dwCLK);
			MAPCLOTH::iterator itCL;

			LPCLKIND pCLK = (*itCLK).second;
			LPCLKINST pCI = NULL;

			if( itCI == m_pItem->m_SFX.m_mapCLK.end() )
			{
				pCI = new CLKINST();
				m_pItem->m_SFX.m_mapCLK.insert( MAPCLKINST::value_type( dwCLK, pCI));
			}
			else
				pCI = (*itCI).second;

			nIndex = m_cCL.InsertString( 0, "NONE");
			m_cCL.SetItemData( nIndex, 0);
			m_pItem->m_dwCurCLK = dwCLK;

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
				m_cCL.SetCurSel(0);
				OnCbnSelchangeComboCl();
			}
		}
	}
	else
		m_pItem->m_dwCurCLK = 0;

	UpdateData();
	InitSFX();

	EnableAllCtrl(TRUE);
	BeginUpdateInst();
	EndUpdateInst();
}

void CSFXBar::OnCbnSelchangeComboCl()
{
	COBJItem *pOBJ = CItemBase::GetOBJItem(m_pItem->m_dwSFXSRC);
	int nIndex = m_cCLK.GetCurSel();

	m_cMESH.ResetContent();
	if( nIndex > 0 && pOBJ )
	{
		DWORD dwCLK = (DWORD) m_cCLK.GetItemData(nIndex);

		MAPCLKINST::iterator itCI = m_pItem->m_SFX.m_mapCLK.find(dwCLK);
		LPCLKINST pCI = (*itCI).second;

		nIndex = m_cCL.GetCurSel();
		if( nIndex > 0 )
		{
			DWORD dwCL = (DWORD) m_cCL.GetItemData(nIndex);
			MAPCLKIND::iterator itCLK = m_pItem->m_SFX.m_SRC.m_pOBJ->m_mapCLKIND.find(dwCLK);

			if( itCLK != m_pItem->m_SFX.m_SRC.m_pOBJ->m_mapCLKIND.end() )
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
						m_cMESH.SetCurSel(0);
						OnCbnSelchangeComboMesh();
					}
				}
			}
		}
		else
			pCI->m_dwCurCL = 0;
	}

	UpdateData();
	InitSFX();

	EnableAllCtrl(TRUE);
	BeginUpdateInst();
	EndUpdateInst();
}

void CSFXBar::OnCbnSelchangeComboMesh()
{
	COBJItem *pOBJ = CItemBase::GetOBJItem(m_pItem->m_dwSFXSRC);
	int nIndex = m_cCLK.GetCurSel();

	if( nIndex > 0 && pOBJ )
	{
		DWORD dwCLK = (DWORD) m_cCLK.GetItemData(nIndex);
		MAPCLKINST::iterator itCI = m_pItem->m_SFX.m_mapCLK.find(dwCLK);

		if( itCI != m_pItem->m_SFX.m_mapCLK.end() )
		{
			LPCLKINST pCI = (*itCI).second;
			nIndex = m_cMESH.GetCurSel();

			if( nIndex > 0 )
				pCI->m_dwCurMESH = (DWORD) m_cMESH.GetItemData(nIndex);
			else
				pCI->m_dwCurMESH = 0;
		}
	}

	UpdateData();
	InitSFX();

	EnableAllCtrl(TRUE);
	BeginUpdateInst();
	EndUpdateInst();
}

void CSFXBar::OnCbnSelchangeComboAct()
{
	COBJItem *pOBJ = CItemBase::GetOBJItem(m_pItem->m_dwSFXSRC);
	int nIndex = m_cACT.GetCurSel();

	m_cANI.ResetContent();
	if( nIndex > 0 && pOBJ )
	{
		DWORD dwACT = (DWORD) m_cACT.GetItemData(nIndex);
		MAPACTION::iterator itACT = pOBJ->m_OBJ.m_mapACTION.find(dwACT);

		if( itACT != pOBJ->m_OBJ.m_mapACTION.end() )
		{
			LPACTION pACT = (*itACT).second;
			MAPANI::iterator itANI;

			nIndex = m_cANI.InsertString( 0, "NONE");
			m_cANI.SetItemData( nIndex, 0);
			m_pItem->m_SFX.m_dwACT = dwACT;

			for( itANI = pACT->m_mapANI.begin(); itANI != pACT->m_mapANI.end(); itANI++)
			{
				MAPSTR::iterator itSTR = pOBJ->m_mapNAME.find((*itANI).first);

				nIndex = m_cANI.InsertString( m_cANI.GetCount(), (*itSTR).second);
				m_cANI.SetItemData( nIndex, (*itANI).first);

				if( (*itANI).first == m_pItem->m_SFX.m_dwANI )
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
		}
	}
	else
		m_pItem->m_SFX.m_dwACT = 0;

	UpdateData();
	InitSFX();

	EnableAllCtrl(TRUE);
	BeginUpdateInst();
	EndUpdateInst();
}

void CSFXBar::OnCbnSelchangeComboAni()
{
	int nIndex = m_cANI.GetCurSel();

	if( nIndex > 0 )
		m_pItem->m_SFX.m_dwANI = (DWORD) m_cANI.GetItemData(nIndex);
	else
		m_pItem->m_SFX.m_dwANI = 0;

	UpdateData();
	InitSFX();

	EnableAllCtrl(TRUE);
	BeginUpdateInst();
	EndUpdateInst();
}

void CSFXBar::EnableAllCtrl( BOOL bEnable)
{
	m_spinOBJFuncID.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ);
	m_spinACTFuncID.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ);
	m_spinANIFuncID.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ);
	m_cOBJFuncID.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ);
	m_cACTFuncID.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ);
	m_cANIFuncID.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ);

	m_cCLK.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ && m_cCLK.GetCount() > 0);
	m_cCL.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ && m_cCL.GetCount() > 0);
	m_cMESH.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ && m_cMESH.GetCount() > 0);
	m_cACT.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ && m_cACT.GetCount() > 0);
	m_cANI.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_OBJ && m_cANI.GetCount() > 0);

	m_spinSizeX.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_TEX);
	m_spinSizeY.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_TEX);
	m_cDESTBlend.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_TEX);
	m_cSRCBlend.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_TEX);
	m_cBlendOP.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_TEX);
	m_cZEnable.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_TEX);
	m_cZWrite.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_TEX);
	m_cSizeX.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_TEX);
	m_cSizeY.EnableWindow(bEnable && m_pItem->m_SFX.m_bSRCType == SFXSRC_TEX);

	m_cSetParticle.EnableWindow(m_pItem->m_SFX.m_bSFXType == SFX_PARTICLE);
	m_cDELSRC.EnableWindow(bEnable && m_pItem->m_dwSFXSRC != 0);
	m_cDELAS.EnableWindow(bEnable && m_pItem->m_dwANISRC != 0);

	m_cAddKey.EnableWindow(bEnable);
	m_cDelKey.EnableWindow(bEnable);
	m_spinKey.EnableWindow(bEnable);
	m_spinA.EnableWindow(bEnable);
	m_spinR.EnableWindow(bEnable);
	m_spinG.EnableWindow(bEnable);
	m_spinB.EnableWindow(bEnable);
//	m_spinOptionLevel.EnableWindow(bEnable);
	m_spinCurLodLevel.EnableWindow(bEnable);
	m_spinLodLevel.EnableWindow(bEnable && m_cCSFX.GetCurSel() >= 0);

	m_cA.EnableWindow(bEnable);
	m_cR.EnableWindow(bEnable);
	m_cG.EnableWindow(bEnable);
	m_cB.EnableWindow(bEnable);
//	m_cOptionLevel.EnableWindow(bEnable);
	m_cCurLodLevel.EnableWindow(bEnable);
	m_cLodLevel.EnableWindow(bEnable && m_cCSFX.GetCurSel() >= 0);

	m_spinLifeTime.EnableWindow(bEnable);
	m_cLifeTime.EnableWindow(bEnable);
	m_cParticle.EnableWindow(bEnable);
	m_cSingle.EnableWindow(bEnable);
	m_cTimeSet.EnableWindow(bEnable);
	m_cSFXSRC.EnableWindow(bEnable);
	m_cANISRC.EnableWindow(bEnable);
	m_cCSFX.EnableWindow(bEnable);

	m_cTotalFrame.EnableWindow(bEnable);
	m_cFrame.EnableWindow(bEnable);
	m_cKey.EnableWindow(bEnable);
}

void CALLBACK CSFXBar::OnSRCDragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

void CALLBACK CSFXBar::OnANIDragLeave( CGSEditCtrl *pEdit)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pEdit->CEditView::OnDragLeave();
}

void CALLBACK CSFXBar::OnSFXDragLeave( CGSListCtrl *pList)
{
	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));

	pList->CListView::OnDragLeave();
}

BOOL CSFXBar::CanSRCDrop( CSFXBar *pBar, CItemBase *pItem)
{
	return pBar->m_pItem != pItem && (
		pItem->m_bType == ITEM_TEXTURE ||
		pItem->m_bType == ITEM_OBJECT ||
		pItem->m_bType == ITEM_SFX );
}

BOOL CSFXBar::CanANIDrop( CSFXBar *pBar, CItemBase *pItem)
{
	return pItem->m_bType == ITEM_ANIMATION;
}

BOOL CSFXBar::CanSFXDrop( CSFXBar *pBar, CItemBase *pItem)
{
	return pItem->m_bType == ITEM_SFX && !((CSFXItem *) pItem)->FindSFX(pBar->m_pItem->m_dwID);
}

DROPEFFECT CALLBACK CSFXBar::OnSRCDragOver( CGSEditCtrl *pEdit,
										    COleDataObject *pDataObject,
											DWORD dwKeyState,
											CPoint point)
{
	CSFXBar *pBar = (CSFXBar *) pEdit->GetParent();
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanSRCDrop( pBar, pItem))
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

DROPEFFECT CALLBACK CSFXBar::OnANIDragOver( CGSEditCtrl *pEdit,
										    COleDataObject *pDataObject,
											DWORD dwKeyState,
											CPoint point)
{
	CSFXBar *pBar = (CSFXBar *) pEdit->GetParent();
	CPoint screenpos = point;

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);
	DROPEFFECT nResult = DROPEFFECT_NONE;

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanANIDrop( pBar, pItem))
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

DROPEFFECT CALLBACK CSFXBar::OnSFXDragOver( CGSListCtrl *pList,
										    COleDataObject *pDataObject,
											DWORD dwKeyState,
											CPoint point)
{
	CSFXBar *pBar = (CSFXBar *) pList->GetParent();
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
				}

				nResult = DROPEFFECT_MOVE;
			}
		}

		break;

	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanSFXDrop( pBar, pItem))
			{
				int nIndex = pList->GetListCtrl()->HitTest(point);

				if( nIndex != pList->GetCurSel() )
				{
					CGameStudioView::m_pDragImage->DragMove( CPoint(
						-10000,
						-10000));

					pList->SetCurSel(nIndex);
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

BOOL CALLBACK CSFXBar::OnSRCDrop( CGSEditCtrl *pEdit,
								  COleDataObject* pDataObject,
								  DROPEFFECT dropEffect,
								  CPoint point)
{
	CSFXBar *pBar = (CSFXBar *) pEdit->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanSRCDrop( pBar, pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->SetSFXSRC(pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnSRCDragLeave(pEdit);
	return pEdit->CEditView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CALLBACK CSFXBar::OnANIDrop( CGSEditCtrl *pEdit,
								  COleDataObject* pDataObject,
								  DROPEFFECT dropEffect,
								  CPoint point)
{
	CSFXBar *pBar = (CSFXBar *) pEdit->GetParent();

	HGLOBAL hGlobal = pDataObject->GetGlobalData(CGameStudioView::m_cfGS);
	LPGSCDATA pData = (LPGSCDATA) ::GlobalLock(hGlobal);

	switch(pData->m_bType)
	{
	case GSCT_ITEM		:
		{
			CItemBase *pItem = (CItemBase *) pData->m_pData;

			if(CanANIDrop( pBar, pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->SetANISRC((CAnimationItem *) pItem);
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnANIDragLeave(pEdit);
	return pEdit->CEditView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CALLBACK CSFXBar::OnSFXDrop( CGSListCtrl *pList,
								  COleDataObject* pDataObject,
								  DROPEFFECT dropEffect,
								  CPoint point)
{
	CSFXBar *pBar = (CSFXBar *) pList->GetParent();

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

			if(CanSFXDrop( pBar, pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->InsertSFX( (CSFXItem *) pItem, pList->GetListCtrl()->HitTest(point));
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnSFXDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

void CSFXBar::OnBnClickedButtonDelanisrc()
{
	CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	m_pItem->m_SFX.m_pANISRC = NULL;
	m_pItem->m_dwANISRC = 0;

	m_strANISRC.LoadString(IDS_NOTUSE);
	pCtrl->EnableCtrl(FALSE);

	UpdateData(FALSE);
	InitSFX();

	EnableAllCtrl(TRUE);
}

void CSFXBar::SetANISRC( CAnimationItem *pItem)
{
	CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	m_pItem->m_SFX.m_pANISRC = &pItem->m_Aniset;
	m_pItem->m_dwANISRC = pItem->m_dwID;

	m_strANISRC = pItem->m_strName;
	pCtrl->EnableCtrl(TRUE);

	UpdateData(FALSE);
	InitSFX();

	EnableAllCtrl(TRUE);
}

void CSFXBar::OnBnClickedButtonDelsrc()
{
	BeginUpdateInst();

	if(m_pItem->m_dwSFXSRC)
		switch(m_pItem->m_SFX.m_bSRCType)
		{
		case SFXSRC_OBJ	:
			{
				MAPCLKINST::iterator it;

				m_pItem->m_SFX.m_dwACT = 0;
				m_pItem->m_SFX.m_dwANI = 0;

				m_cMESH.ResetContent();
				m_cCLK.ResetContent();
				m_cCL.ResetContent();
				m_cACT.ResetContent();
				m_cANI.ResetContent();

				for( it = m_pItem->m_SFX.m_mapCLK.begin(); it != m_pItem->m_SFX.m_mapCLK.end(); it++)
					delete (*it).second;
				m_pItem->m_SFX.m_mapCLK.clear();
			}

			break;

		case SFXSRC_SFX	:
		case SFXSRC_TEX	: break;
		}

	m_pItem->m_TSFX.Release();
	m_pItem->m_dwSFXSRC = 0;

	m_pItem->m_SFX.m_bSRCType = SFXSRC_TEX;
	m_pItem->m_SFX.m_SRC.m_pTEX = NULL;

	m_strSFXSRC.LoadString(IDS_NOTUSE);
	UpdateData(FALSE);
	InitSFX();

	EnableAllCtrl(TRUE);
	EndUpdateInst();
}

void CSFXBar::SetSFXSRC( CItemBase *pItem)
{
	BeginUpdateInst();

	if(m_pItem->m_dwSFXSRC)
		switch(m_pItem->m_SFX.m_bSRCType)
		{
		case SFXSRC_OBJ	:
			{
				MAPCLKINST::iterator it;

				m_pItem->m_SFX.m_dwACT = 0;
				m_pItem->m_SFX.m_dwANI = 0;

				m_cMESH.ResetContent();
				m_cCLK.ResetContent();
				m_cCL.ResetContent();
				m_cACT.ResetContent();
				m_cANI.ResetContent();

				for( it = m_pItem->m_SFX.m_mapCLK.begin(); it != m_pItem->m_SFX.m_mapCLK.end(); it++)
					delete (*it).second;
				m_pItem->m_SFX.m_mapCLK.clear();
			}

			break;

		case SFXSRC_SFX	:
		case SFXSRC_TEX	: break;
		}

	m_pItem->m_TSFX.Release();
	m_pItem->m_dwSFXSRC = pItem->m_dwID;

	switch(pItem->m_bType)
	{
	case ITEM_TEXTURE	:
		{
			m_pItem->m_SFX.m_SRC.m_pTEX = ((CTextureItem *) pItem)->GetTEXTURESET();
			m_pItem->m_SFX.m_bSRCType = SFXSRC_TEX;
		}

		break;

	case ITEM_OBJECT	:
		{
			m_pItem->m_SFX.m_SRC.m_pOBJ = &((COBJItem *) pItem)->m_OBJ;
			m_pItem->m_SFX.m_bSRCType = SFXSRC_OBJ;

			SetCLKList((COBJItem *) pItem);
			SetACTList((COBJItem *) pItem);
		}

		break;

	case ITEM_SFX		:
		{
			m_pItem->m_SFX.m_SRC.m_pSFX = &((CSFXItem *) pItem)->m_SFX;
			m_pItem->m_SFX.m_bSRCType = SFXSRC_SFX;
		}

		break;
	}

	m_strSFXSRC = pItem->m_strName;
	pItem->Regulate();
	UpdateData(FALSE);
	InitSFX();

	EnableAllCtrl(TRUE);
	EndUpdateInst();
}

void CSFXBar::OnBnClickedRadioParticle()
{
	UpdateData();
	m_pItem->m_SFX.m_bSFXType = SFX_PARTICLE;
	InitSFX();

	EnableAllCtrl(TRUE);
}

void CSFXBar::OnBnClickedRadioSingle()
{
	UpdateData();
	m_pItem->m_SFX.m_bSFXType = SFX_SINGLE;
	InitSFX();

	EnableAllCtrl(TRUE);
}

void CSFXBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinSizeX )
	{
		FLOAT fSizeX = (FLOAT) atof(m_strSizeX);

		fSizeX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinSizeX.SetPos(0);

		m_strSizeX.Format( "%.2f", fSizeX);
		UpdateData(FALSE);
		OnEnChangeEditSizex();
	}

	if( pScrollBar == (CScrollBar *) &m_spinSizeY )
	{
		FLOAT fSizeY = (FLOAT) atof(m_strSizeY);

		fSizeY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinSizeY.SetPos(0);

		m_strSizeY.Format( "%.2f", fSizeY);
		UpdateData(FALSE);
		OnEnChangeEditSizey();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CSFXBar::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinKey && nPos != 0 &&
		!m_pItem->m_SFX.m_vKey.empty() )
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nCount = INT(m_pItem->m_SFX.m_vKey.size());
		int nPrev = nCount - 1;
		int nNext = 0;

		for( int i=0; i<nCount; i++)
		{
			if( m_dwTick < m_pItem->m_SFX.m_vKey[i]->m_dwTick )
			{
				nNext = i;
				break;
			}

			if( m_dwTick == m_pItem->m_SFX.m_vKey[i]->m_dwTick )
			{
				nNext = i + 1 < nCount ? i + 1 : 0;
				break;
			}

			nPrev = i;
		}

		if( INT(nPos) < 0 )
			nNext = INT(m_pItem->m_SFX.m_vKey[nPrev]->m_dwTick);
		else
			nNext = INT(m_pItem->m_SFX.m_vKey[nNext]->m_dwTick);

		nNext = GetPosFromTick((DWORD) nNext);
		m_spinKey.SetPos(0);

		pCtrl->m_cPlayCtrl.SetPos(nNext);
		UpdateCtrl(nNext);
	}

	CBarContent::OnHScroll( nSBCode, nPos, pScrollBar);
}

void CSFXBar::OnEnChangeEditSizex()
{
	UpdateData();
	m_pItem->m_SFX.m_fSizeX = (FLOAT) atof(m_strSizeX);
	InitSFX();

	BeginUpdateInst();
	EndUpdateInst();
}

void CSFXBar::OnEnChangeEditSizey()
{
	UpdateData();
	m_pItem->m_SFX.m_fSizeY = (FLOAT) atof(m_strSizeY);
	InitSFX();

	BeginUpdateInst();
	EndUpdateInst();
}

void CSFXBar::OnEnChangeEditLifetime()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_SFX.m_dwLifeTime = m_dwLifeTime;
		InitSFX();
	}
}

void CSFXBar::OnBnClickedButtonTimeset()
{
	CFramesetDlg dlg;

	dlg.m_dwTotalFrame = m_dwTotalFrame;
	if( dlg.DoModal() == IDOK )
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nCount = INT(m_pItem->m_SFX.m_vKey.size());
		DWORD dwTotalFrame = m_dwTotalFrame;
		VECTORCLRKEY vKey;

		for( int i=0; i<nCount; i++)
		{
			m_pItem->m_SFX.m_vKey[i]->m_dwTick = (DWORD) GetPosFromTick(m_pItem->m_SFX.m_vKey[i]->m_dwTick);
			vKey.push_back(m_pItem->m_SFX.m_vKey[i]);
		}
		m_pItem->m_SFX.m_vKey.clear();

		m_dwTotalFrame = dlg.m_dwTotalFrame;
		for( i=0; i<nCount; i++)
		{
			vKey[i]->m_dwTick = GetTickFromPos(vKey[i]->m_dwTick * m_dwTotalFrame / dwTotalFrame);
			m_pItem->m_SFX.DeleteKey(vKey[i]->m_dwTick);
			m_pItem->m_SFX.InsertKey(vKey[i]);
		}
		vKey.clear();

		m_pItem->m_dwTotalFrame = m_dwTotalFrame;
		pCtrl->SetRange( 0, m_dwTotalFrame);

		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}
}

void CSFXBar::OnBnClickedButtonAddkey()
{
	if( m_pItem && m_dwTotalFrame )
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_SFX.FindKey(m_dwTick);

		if( nIndex == -1 )
		{
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
			LPCLRKEY pKey = new CLRKEY();

			pKey->m_dwColor = m_pItem->m_SFX.GetColor(m_dwTick);
			pKey->m_dwTick = m_dwTick;
			m_pItem->m_SFX.InsertKey(pKey);

			int nPos = pCtrl->m_cPlayCtrl.GetPos();
			pCtrl->m_cMarker.AddMark(nPos - pCtrl->m_cPlayCtrl.GetRangeMin());
			UpdateCtrl(nPos);
		}
	}
}

void CSFXBar::OnBnClickedButtonDeletekey()
{
	if( m_pItem && m_dwTotalFrame )
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_SFX.FindKey(m_dwTick);

		if( nIndex != -1 )
		{
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
			int nPos = pCtrl->m_cPlayCtrl.GetPos();

			pCtrl->m_cMarker.DelMark(nPos - pCtrl->m_cPlayCtrl.GetRangeMin());
			m_pItem->m_SFX.DeleteKey(m_dwTick);
			UpdateCtrl(nPos);
		}
	}
}

void CSFXBar::OnEnChangeEditA()
{
	if(m_pItem)
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_SFX.FindKey(m_dwTick);

		if( nIndex != -1 )
		{
			UpdateData();
			m_pItem->m_SFX.m_vKey[nIndex]->m_dwColor &= 0x00FFFFFF;
			m_pItem->m_SFX.m_vKey[nIndex]->m_dwColor |= ((m_dwValueA & 0x000000FF) << 24);

			if(!pDoc->GetPlayCtrl()->m_bPlay)
				pDoc->RedrawAllView();
		}
	}
}

void CSFXBar::OnEnChangeEditR()
{
	if(m_pItem)
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_SFX.FindKey(m_dwTick);

		if( nIndex != -1 )
		{
			UpdateData();
			m_pItem->m_SFX.m_vKey[nIndex]->m_dwColor &= 0xFF00FFFF;
			m_pItem->m_SFX.m_vKey[nIndex]->m_dwColor |= ((m_dwValueR & 0x000000FF) << 16);

			if(!pDoc->GetPlayCtrl()->m_bPlay)
				pDoc->RedrawAllView();
		}
	}
}

void CSFXBar::OnEnChangeEditG()
{
	if(m_pItem)
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_SFX.FindKey(m_dwTick);

		if( nIndex != -1 )
		{
			UpdateData();
			m_pItem->m_SFX.m_vKey[nIndex]->m_dwColor &= 0xFFFF00FF;
			m_pItem->m_SFX.m_vKey[nIndex]->m_dwColor |= ((m_dwValueG & 0x000000FF) << 8);

			if(!pDoc->GetPlayCtrl()->m_bPlay)
				pDoc->RedrawAllView();
		}
	}
}

void CSFXBar::OnEnChangeEditB()
{
	if(m_pItem)
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_SFX.FindKey(m_dwTick);

		if( nIndex != -1 )
		{
			UpdateData();
			m_pItem->m_SFX.m_vKey[nIndex]->m_dwColor &= 0xFFFFFF00;
			m_pItem->m_SFX.m_vKey[nIndex]->m_dwColor |= (m_dwValueB & 0x000000FF);

			if(!pDoc->GetPlayCtrl()->m_bPlay)
				pDoc->RedrawAllView();
		}
	}
}

LRESULT CSFXBar::OnKeyMove( WPARAM wParam, LPARAM lParam)
{
	int nFrom = INT(wParam);
	int nTo = INT(lParam);

	if(m_pItem)
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nMin = pCtrl->m_cPlayCtrl.GetRangeMin();

		nFrom += nMin;
		nTo += nMin;

		if( nTo != -1 )
		{
			VECTORCLRKEY::iterator it = m_pItem->m_SFX.m_vKey.begin();
			DWORD dwFrom = GetTickFromPos(nFrom);
			DWORD dwTo = GetTickFromPos(nTo);
			LPCLRKEY pKey = NULL;

			m_pItem->m_SFX.DeleteKey(dwTo);
			while( it != m_pItem->m_SFX.m_vKey.end() )
				if( (*it)->m_dwTick == dwFrom )
				{
					pKey = (*it);
					m_pItem->m_SFX.m_vKey.erase(it);

					break;
				}
				else
					it++;
			m_pItem->m_SFX.DeleteKey(dwFrom);

			if(pKey)
			{
				pKey->m_dwTick = dwTo;
				m_pItem->m_SFX.InsertKey(pKey);
			}
		}
		else
			m_pItem->m_SFX.DeleteKey(GetTickFromPos(nFrom));

		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}

	return 0;
}

LRESULT CSFXBar::OnKeyCopy( WPARAM wParam, LPARAM lParam)
{
	int nFrom = INT(wParam);
	int nTo = INT(lParam);

	if(m_pItem)
	{
		CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nMin = pCtrl->m_cPlayCtrl.GetRangeMin();
		int nIndex = -1;

		nFrom += nMin;
		nTo += nMin;

		DWORD dwFrom = GetTickFromPos(nFrom);
		DWORD dwTo = GetTickFromPos(nTo);

		m_pItem->m_SFX.DeleteKey(dwTo);
		nIndex = m_pItem->m_SFX.FindKey(dwFrom);

		if( nIndex != -1 )
		{
			LPCLRKEY pKey = new CLRKEY();

			pKey->m_dwColor = m_pItem->m_SFX.m_vKey[nIndex]->m_dwColor;
			pKey->m_dwTick = dwTo;

			m_pItem->m_SFX.InsertKey(pKey);
		}

		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}

	return 0;
}

int CSFXBar::GetPosFromTick( DWORD dwTick)
{
	if(m_pItem->m_SFX.m_pANISRC)
	{
		DWORD dwTotalTick = CTMath::GetTimeTick(m_pItem->m_SFX.m_pANISRC->GetTotalTime() + 0.5f);
		int nPos = dwTick * m_dwTotalFrame / dwTotalTick;

		return dwTick * m_dwTotalFrame % dwTotalTick ? nPos + 1 : nPos;
	}

	return 0;
}

DWORD CSFXBar::GetTickFromPos( int nPos)
{
	if(m_pItem->m_SFX.m_pANISRC)
	{
		DWORD dwTotalTick = CTMath::GetTimeTick(m_pItem->m_SFX.m_pANISRC->GetTotalTime() + 0.5f);
		return m_dwTotalFrame ? nPos * dwTotalTick / m_dwTotalFrame : 0;
	}

	return 0;
}

void CSFXBar::UpdateCtrl( int nPos)
{
	m_pItem->m_TSFX.InitSFX(
		&CMainFrame::m_3DDevice,
		&m_pItem->m_SFX);

	if( m_pItem->m_SFX.m_bSFXType == SFX_SINGLE )
	{
		DWORD dwTick = GetTickFromPos(nPos);

		m_pItem->m_TSFX.CalcTick(
			CMainFrame::m_3DDevice.m_pDevice,
			dwTick);
	}

	UpdateCtrl();
}

void CSFXBar::UpdateCtrl()
{
	CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	DWORD dwTick = GetTickFromPos(pCtrl->m_cPlayCtrl.GetPos());

	int nIndex = m_pItem->m_SFX.FindKey(dwTick);
	DWORD dwColor = m_pItem->m_SFX.GetColor(dwTick);
	BOOL bEnable = nIndex != -1;

	if(!pCtrl->m_bPlay)
	{
		m_spinA.EnableWindow(bEnable);
		m_spinR.EnableWindow(bEnable);
		m_spinG.EnableWindow(bEnable);
		m_spinB.EnableWindow(bEnable);
//		m_spinLodLevel.EnableWindow(bEnable);
//		m_spinCurLodLevel.EnableWindow(bEnable);
		m_cA.EnableWindow(bEnable);
		m_cR.EnableWindow(bEnable);
		m_cG.EnableWindow(bEnable);
		m_cB.EnableWindow(bEnable);
//		m_cLodLevel.EnableWindow(bEnable);
//		m_cCurLodLevel.EnableWindow(bEnable);

		m_cDelKey.EnableWindow(bEnable);
		m_cAddKey.EnableWindow(!bEnable);

		m_dwTick = dwTick;
	}

	m_dwFrame = pCtrl->m_cPlayCtrl.GetPos();
	m_dwValueA = GETVALUE_A(dwColor);
	m_dwValueR = GETVALUE_R(dwColor);
	m_dwValueG = GETVALUE_G(dwColor);
	m_dwValueB = GETVALUE_B(dwColor);

	if(bEnable)
		m_strKey.Format( "%d", nIndex);
	else
		m_strKey.Format("");

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
	UpdateData(FALSE);
}

void CSFXBar::InsertSFX( CSFXItem *pItem, int nIndex)
{
	CListCtrl *pListCtrl = m_cCSFX.GetListCtrl();

	if( nIndex == -1 )
		nIndex = INT(m_pItem->m_vSFXID.size());

	m_pItem->m_vSFXID.insert(
		m_pItem->m_vSFXID.begin() + nIndex,
		pItem->m_dwID);

	m_pItem->m_SFX.m_vSFX.insert(
		m_pItem->m_SFX.m_vSFX.begin() + nIndex,
		&pItem->m_SFX);

	nIndex = pListCtrl->InsertItem( nIndex, pItem->m_strName);
	pListCtrl->SetItemData( nIndex, (DWORD_PTR) pItem);
	CString str;
	str.Format( "Lv %d", pItem->m_SFX.m_dwLodLevel );
	pListCtrl->SetItemText( nIndex, 1, str );

//	m_cCSFX.UpdateColumnWidth();
	InitSFX();
}

void CSFXBar::DeleteSFX( int nIndex)
{
	CListCtrl *pListCtrl = m_cCSFX.GetListCtrl();

	m_pItem->m_SFX.m_vSFX.erase(m_pItem->m_SFX.m_vSFX.begin() + nIndex);
	m_pItem->m_vSFXID.erase(m_pItem->m_vSFXID.begin() + nIndex);

	pListCtrl->DeleteItem(nIndex);
//	m_cCSFX.UpdateColumnWidth();

	UpdateCtrl();
	InitSFX();
}

void CSFXBar::MoveItemPos( int nFrom, int nTo)
{
	if( nFrom == nTo )
		return;

	CListCtrl *pListCtrl = m_cCSFX.GetListCtrl();
	CSFXItem *pItem = (CSFXItem *) pListCtrl->GetItemData(nFrom);

	DeleteSFX(nFrom);
	InsertSFX( pItem, nTo);
}

LRESULT CSFXBar::OnPlayCtrlProc( WPARAM wParam, LPARAM lParam)
{
	CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	static DWORD dwPrevTick = GetTickCount();
	static DWORD dwPrevTickFPS = GetTickCount();
	DWORD dwCurTick = GetTickCount();

	if( pCtrl->m_dwTickCount == 0 )
	{
		dwPrevTick = GetTickCount();
		dwPrevTickFPS = GetTickCount();
	}

	float dt = (dwCurTick - dwPrevTick) / 1000.0f;
	dwPrevTick = GetTickCount();
	if( !m_FrameRender.Update( dt ) )
		return 0;

	if( dwCurTick - dwPrevTickFPS > 1000 )
	{
		while( dwCurTick - dwPrevTickFPS > 1000 )
			dwPrevTickFPS += 1000;

		pDoc->SetFPS( m_dwCount );
		m_dwCount = 0;
	}
	else
	{
		++m_dwCount;
	}

	DWORD dwTick = (DWORD) wParam;

	if( dwTick > m_dwTick )
		dwTick -= m_dwTick;
	else
		dwTick = 0;
	m_dwTick = (DWORD) wParam;

	if(m_pItem->m_SFX.m_pANISRC)
	{
		DWORD dwTotalTick = CTMath::GetTimeTick(m_pItem->m_SFX.m_pANISRC->GetTotalTime());

		if( m_dwTick >= dwTotalTick )
		{
			m_dwTick -= dwTotalTick;
			dwTick -= m_dwTick;

			if(!pCtrl->m_bLoop)
			{
				pCtrl->SetPos(m_pItem->m_dwTotalFrame);
				m_pItem->m_TSFX.m_bDeleted = TRUE;
			}
			else
				m_pItem->m_TSFX.CalcTick( CMainFrame::m_3DDevice.m_pDevice, dwTick);

			pCtrl->m_dwBeginTick = pCtrl->m_dwTickCount - m_dwTick;
			dwTick = m_dwTick;
		}
	}

	if(m_pItem->m_TSFX.IsDead())
	{
		if( !pCtrl->m_bLoop )
		{
			m_dwTick = m_pItem->m_SFX.m_pANISRC ? CTMath::GetTimeTick(m_pItem->m_SFX.m_pANISRC->GetTotalTime()) : 0;
			m_pItem->m_TSFX.m_bDeleted = FALSE;

			pCtrl->OnBnClickedButtonStop();
		}
		else
		{
			m_dwTick = 0;
			pCtrl->m_dwTickCount = 0;
			pCtrl->SetPos( 0 );
			pCtrl->OnBnClickedButtonStop();
			pCtrl->OnBnClickedButtonPlay();
			pDoc->SetFPS( m_dwFPS );
		}
	}
	else
		m_pItem->m_TSFX.CalcTick( CMainFrame::m_3DDevice.m_pDevice, dwTick);

	if(!m_pItem->m_TSFX.m_bDeleted)
	{
		pCtrl->SetPos(GetPosFromTick(m_dwTick));
		UpdateCtrl();
	}
	pDoc->RedrawAllView();

	return 0;
}

LRESULT CSFXBar::OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam)
{
	CSFXDoc *pDoc = (CSFXDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	if( m_pItem->m_SFX.m_bSFXType == SFX_PARTICLE || (
		m_pItem->m_SFX.m_pANISRC &&
		CTMath::GetTimeTick(m_pItem->m_SFX.m_pANISRC->GetTotalTime()) <= m_dwTick ))
	{
		pCtrl->SetPos(0);
		m_dwTick = 0;
	}
	else
		pCtrl->m_dwBeginTick -= m_dwTick;

	InitSFX();
	EnableAllCtrl(FALSE);

	return 0;
}

LRESULT CSFXBar::OnPlayCtrlStop( WPARAM wParam, LPARAM lParam)
{
	EnableAllCtrl(TRUE);
	UpdateCtrl(GetPosFromTick(m_dwTick));

	return 0;
}

LRESULT CSFXBar::OnPlayCtrlPos( WPARAM wParam, LPARAM lParam)
{
	UpdateCtrl(INT(lParam));
	return 0;
}

void CSFXBar::OnLvnBegindragListCombine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	HGLOBAL hGlobal = ::GlobalAlloc( GMEM_SHARE|GMEM_MOVEABLE, sizeof(GSCDATA));
	COleDataSource source;
	GSLISTITEM vItem;

	CGameStudioView::m_pDragImage = m_cCSFX.GetListCtrl()->CreateDragImage(
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

void CSFXBar::OnBnClickedButtonMoveup()
{
	int nIndex = m_cCSFX.GetCurSel();
	if( nIndex < 0 )
		return ;

	if( nIndex == 0 )
		MoveItemPos( nIndex, INT(m_pItem->m_vSFXID.size() - 1));
	else
		MoveItemPos( nIndex, nIndex - 1);
}

void CSFXBar::OnBnClickedButtonMovedown()
{
	int nIndex = m_cCSFX.GetCurSel();
	if( nIndex < 0 )
		return ;

	if( nIndex == INT(m_pItem->m_vSFXID.size() - 1) )
		MoveItemPos( nIndex, 0);
	else
		MoveItemPos( nIndex, nIndex + 1);
}

void CSFXBar::OnBnClickedButtonDeleteSfx()
{
	int nCount = m_cCSFX.GetListCtrl()->GetItemCount();
	int nIndex = m_cCSFX.GetCurSel();
	if( nCount == 0 || nIndex == -1 )
		return ;

	DeleteSFX(m_cCSFX.GetCurSel());
}

void CSFXBar::OnLvnItemActivateListCombine(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

//	DeleteSFX(pNMIA->iItem);
	*pResult = 0;
}

void CSFXBar::BeginUpdateInst()
{
	MAPTSFX::iterator it;

	for( it = CTachyonSFX::m_mapINST.begin(); it != CTachyonSFX::m_mapINST.end(); it++)
		if( (*it).second->m_pSFX == &m_pItem->m_SFX )
		{
			(*it).second->Release();
			(*it).second->m_pSFX = &m_pItem->m_SFX;
		}
}

void CSFXBar::EndUpdateInst()
{
	MAPTSFX::iterator it;

	for( it = CTachyonSFX::m_mapINST.begin(); it != CTachyonSFX::m_mapINST.end(); it++)
		if( (*it).second->m_pSFX == &m_pItem->m_SFX )
		{
			(*it).second->m_pSFX = NULL;
			(*it).second->InitSFX(
				&CMainFrame::m_3DDevice,
				&m_pItem->m_SFX);
		}
}

void CSFXBar::OnEnChangeEditObjfunc()
{
	if(m_pItem)
	{
		UpdateData();

		m_pItem->m_SFX.m_dwOBJFuncID = m_dwOBJFuncID;
		InitSFX();
	}
}

void CSFXBar::OnEnChangeEditActfunc()
{
	if(m_pItem)
	{
		UpdateData();

		m_pItem->m_SFX.m_dwACTFuncID = m_dwACTFuncID;
		InitSFX();
	}
}

void CSFXBar::OnEnChangeEditAnifunc()
{
	if(m_pItem)
	{
		UpdateData();

		m_pItem->m_SFX.m_dwANIFuncID = m_dwANIFuncID;
		InitSFX();
	}
}

void CSFXBar::OnBnClickedCheckZenable()
{
	UpdateData();

	m_pItem->m_SFX.m_bZEnable = (BYTE) m_bZEnable;
	InitSFX();
}

void CSFXBar::OnBnClickedCheckZwrite()
{
	UpdateData();

	m_pItem->m_SFX.m_bZWrite = (BYTE) m_bZWrite;
	InitSFX();
}

void CSFXBar::OnCbnSelchangeComboBlendop()
{
	static DWORD dwBlendOP[] = {
		D3DBLENDOP_ADD,
		D3DBLENDOP_SUBTRACT,
		D3DBLENDOP_REVSUBTRACT,
		D3DBLENDOP_MIN,
		D3DBLENDOP_MAX};

	m_pItem->m_SFX.m_dwBlendOP = dwBlendOP[m_cBlendOP.GetCurSel()];
	InitSFX();
}

void CSFXBar::OnCbnSelchangeComboDestblend()
{
	m_pItem->m_SFX.m_dwDESTBlend = GetBlend(m_cDESTBlend.GetCurSel());
	InitSFX();
}

void CSFXBar::OnCbnSelchangeComboSrcblend()
{
	m_pItem->m_SFX.m_dwSRCBlend = GetBlend(m_cSRCBlend.GetCurSel());
	InitSFX();
}

DWORD CSFXBar::GetBlend( int nIndex)
{
	static DWORD dwBlend[] = {
		D3DBLEND_ZERO,
		D3DBLEND_ONE,
		D3DBLEND_SRCCOLOR,
		D3DBLEND_INVSRCCOLOR,
		D3DBLEND_SRCALPHA,
		D3DBLEND_INVSRCALPHA,
		D3DBLEND_DESTALPHA,
		D3DBLEND_INVDESTALPHA,
		D3DBLEND_DESTCOLOR,
		D3DBLEND_INVDESTCOLOR,
		D3DBLEND_SRCALPHASAT,
		D3DBLEND_BOTHSRCALPHA,
		D3DBLEND_BOTHINVSRCALPHA,
		D3DBLEND_BLENDFACTOR,
		D3DBLEND_INVBLENDFACTOR};

	return dwBlend[nIndex];
}

void CSFXBar::OnEnChangeEditLodlevel()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CBarContent::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if( !m_pItem )
		return ;

	UINT nIndex = m_cCSFX.GetCurSel();
	if( nIndex < 0 )
		return ;

	UpdateData();
	CListCtrl* pListCtrl = m_cCSFX.GetListCtrl();
	CSFXItem* pItem = (CSFXItem*)pListCtrl->GetItemData( nIndex );
	if( pItem )
	{
		pItem->m_SFX.m_dwLodLevel = m_dwSelLevel;
		CString str;
		str.Format( "Lv %d", m_dwSelLevel );
		pListCtrl->SetItemText( nIndex, 1, str );
	}

	InitSFX();
}


void CSFXBar::OnEnChangeEditCurlodlevel()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CBarContent::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if( !m_pItem )
		return ;

	UpdateData();
	m_pItem->m_SFX.m_dwLodLevel = m_dwCurLevel;
	InitSFX();
}


void CSFXBar::OnNMClickListCombine(NMHDR *pNMHDR, LRESULT *pResult)
{
	CListCtrl* pListCtrl = m_cCSFX.GetListCtrl();
	int nCount = m_cCSFX.GetListCtrl()->GetItemCount();
	int nIndex = m_cCSFX.GetCurSel();
	if( nCount == 0 || nIndex == -1 )
	{
		m_cLodLevel.EnableWindow(FALSE);
		m_spinLodLevel.EnableWindow(FALSE);
		return ;
	}

	CSFXItem* pItem = (CSFXItem*)pListCtrl->GetItemData( nIndex );
	if( pItem )
	{
		m_dwSelLevel = pItem->m_SFX.m_dwLodLevel;
		m_cLodLevel.EnableWindow(TRUE);
		m_spinLodLevel.EnableWindow(TRUE);
		CString str;
		str.Format( "Lv %d", m_dwSelLevel );
		pListCtrl->SetItemText( nIndex, 1, str );

		UpdateData(FALSE);
		InitSFX();
	}
}

void CSFXBar::OnEnChangeEditGloballodlevel()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CBarContent::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if( !m_pItem )
		return ;

	UpdateData();
	CTachyonSFX::m_bOptionLodLevel = m_bOptionLodLevel;
}

void CSFXBar::OnNMKillfocusListCombine(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

}

void CSFXBar::OnLvnBeginlabeleditListCombine(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


}

void CSFXBar::OnLvnEndlabeleditListCombine(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}

void CSFXBar::OnEnChangeEditSetfps()
{
	// TODO:  RICHEDIT 컨트롤인 경우 이 컨트롤은
	// CBarContent::마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여
	// CRichEditCtrl().SetEventMask()를 호출하도록 OnInitDialog() 함수를 재지정하지 않으면
	// 이 알림을 보내지 않습니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if( !m_pItem )
		return ;

	UpdateData();
	m_FrameRender.SetFramePerSec( (float)m_dwFPS );
}


void CSFXBar::OnLvnItemchangedListCombine(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	CListCtrl* pListCtrl = m_cCSFX.GetListCtrl();
	int nCount = m_cCSFX.GetListCtrl()->GetItemCount();
	int nIndex = m_cCSFX.GetCurSel();
	if( nCount == 0 || nIndex == -1 )
	{
		m_cLodLevel.EnableWindow(FALSE);
		m_spinLodLevel.EnableWindow(FALSE);
		return ;
	}

	CSFXItem* pItem = (CSFXItem*)pListCtrl->GetItemData( nIndex );
	if( pItem )
	{
		m_dwSelLevel = pItem->m_SFX.m_dwLodLevel;
		m_cLodLevel.EnableWindow(TRUE);
		m_spinLodLevel.EnableWindow(TRUE);
		CString str;
		str.Format( "Lv %d", m_dwSelLevel );
		pListCtrl->SetItemText( nIndex, 1, str );

		UpdateData(FALSE);
		InitSFX();
	}
}

void CSFXBar::OnLvnKeydownListCombine(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( !m_pItem )
		return ;

	UINT nIndex = m_cCSFX.GetCurSel();
	if( nIndex < 0 )
		return ;

	UpdateData();

	LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

	switch( pLVKeyDow->wVKey )
	{
	case VK_LEFT : if( m_dwSelLevel > 1 ) --m_dwSelLevel; break;
	case VK_RIGHT : if( m_dwSelLevel < 6 ) ++m_dwSelLevel; break;
	}

	UpdateData( FALSE );

	CListCtrl* pListCtrl = m_cCSFX.GetListCtrl();
	CSFXItem* pItem = (CSFXItem*)pListCtrl->GetItemData( nIndex );
	if( pItem )
	{
		pItem->m_SFX.m_dwLodLevel = m_dwSelLevel;
		CString str;
		str.Format( "Lv %d", m_dwSelLevel );
		pListCtrl->SetItemText( nIndex, 1, str );
	}

	InitSFX();
}

void CSFXBar::OnBnClickedButtonFindsfx()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CSFXDoc* pDoc = (CSFXDoc*) m_pItem->GetMainDocument();
	CMainFrame* pFrame = (CMainFrame*) AfxGetMainWnd();

	if( pFrame && pFrame->m_pDocSFX && pDoc && pDoc->m_pItem )
	{
		
		CListCtrl* pListCtrl = m_cCSFX.GetListCtrl();
		int nIndex = m_cCSFX.GetCurSel();
		CSFXItem* pItem = (CSFXItem*)pListCtrl->GetItemData( nIndex );

		if( pItem )
		{
			CTreeCtrl& cTree = pFrame->m_pDocSFX->GetTreeCtrl();
			cTree.SelectItem( pItem->m_itemPos );
		}
	}
}

