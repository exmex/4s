// TextureBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "TextureBar.h"
#include "GSDialogBar.h"
#include "TextureDoc.h"
#include "TimeSettingDlg.h"
#include ".\texturebar.h"


// CTextureBar 대화 상자입니다.

typedef struct tagMIPFILTER
{
	CString m_strTitle;
	DWORD m_dwFilter;

	tagMIPFILTER( CString strTitle, DWORD dwFilter)
	{
		m_strTitle = strTitle;
		m_dwFilter = dwFilter;
	};

	~tagMIPFILTER()
	{
	};
} MIPFILTER, *LPMIPFILTER;

static MIPFILTER g_vMipFilter[] = {
	MIPFILTER( CString("None"), D3DTEXF_NONE),
	MIPFILTER( CString("Point"), D3DTEXF_POINT),
	MIPFILTER( CString("Linear"), D3DTEXF_LINEAR),
	MIPFILTER( CString("Anisotropic"), D3DTEXF_ANISOTROPIC),
	MIPFILTER( CString("Pyramidal Quad"), D3DTEXF_PYRAMIDALQUAD),
	MIPFILTER( CString("Gaussian Quad"), D3DTEXF_GAUSSIANQUAD)};


IMPLEMENT_DYNAMIC( CTextureBar, CBarContent)
CTextureBar::CTextureBar( CWnd* pParent /*=NULL*/)
: CBarContent(CTextureBar::IDD, pParent)
, m_dwTotalTick(0)
, m_dwTotalFrame(0)
, m_dwFrame(0)
, m_strKey(_T(""))
, m_strValueU(_T(""))
, m_strValueV(_T(""))
, m_strRotation(_T(""))
, m_nMipFilter(0)
, m_strLODBias(_T(""))
, m_strValueSU(_T(""))
, m_strValueSV(_T(""))
, m_nTextureOption(0)
, m_nTextureFormat(0)
, m_bShowTiling(TRUE)
{
	m_nInitHeight = 0;
	m_pItem = NULL;
}

CTextureBar::~CTextureBar()
{
}

void CTextureBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TOTAL_FRAME, m_dwTotalFrame);
	DDX_Text(pDX, IDC_STATIC_TOTAL_TICK, m_dwTotalTick);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_cColorBox);
	DDX_Control(pDX, IDC_STATIC_CLRCTRL, m_cControlBox);
	DDX_Control(pDX, IDC_LIST_SPRITE, m_cImageList);
	DDX_Control(pDX, IDC_BUTTON_ADDKEY, m_cAddKey);
	DDX_Control(pDX, IDC_BUTTON_DELETEKEY, m_cDeleteKey);
	DDX_Control(pDX, IDC_EDIT_U, m_cValueU);
	DDX_Control(pDX, IDC_SPIN_U, m_spinValueU);
	DDX_Control(pDX, IDC_EDIT_V, m_cValueV);
	DDX_Control(pDX, IDC_SPIN_V, m_spinValueV);
	DDX_Control(pDX, IDC_EDIT_R, m_cValueR);
	DDX_Control(pDX, IDC_SPIN_R, m_spinRotation);
	DDX_Control(pDX, IDC_SPIN_KEY, m_spinKey);
	DDX_Text(pDX, IDC_STATIC_FRAME, m_dwFrame);
	DDX_Text(pDX, IDC_STATIC_KEY, m_strKey);
	DDX_Text(pDX, IDC_EDIT_U, m_strValueU);
	DDX_Text(pDX, IDC_EDIT_V, m_strValueV);
	DDX_Text(pDX, IDC_EDIT_R, m_strRotation);
	DDX_Control(pDX, IDC_BUTTON_TIMESET, m_cTimeSet);
	DDX_Control(pDX, IDC_BUTTON_DELETE_IMAGE, m_cDeleteSprite);
	DDX_Control(pDX, IDC_BUTTON_MOVEUP, m_cUpSprite);
	DDX_Control(pDX, IDC_BUTTON_MOVEDOWN, m_cDownSprite);
	DDX_Control(pDX, IDC_COMBO_MIPFILTER, m_cMipFilter);
	DDX_CBIndex(pDX, IDC_COMBO_MIPFILTER, m_nMipFilter);
	DDX_Control(pDX, IDC_EDIT_LODBIAS, m_cLODBias);
	DDX_Text(pDX, IDC_EDIT_LODBIAS, m_strLODBias);
	DDX_Control(pDX, IDC_SPIN_LODBIAS, m_spinLODBias);
	DDX_Control(pDX, IDC_RADIO_MESH_SPHERE, m_cSphere);
	DDX_Control(pDX, IDC_RADIO_MESH_PLANE, m_cPlane);
	DDX_Control(pDX, IDC_RADIO_MESH_BOX, m_cBox);
	DDX_Control(pDX, IDC_EDIT_SU, m_cValueSU);
	DDX_Control(pDX, IDC_SPIN_SU, m_spinValueSU);
	DDX_Control(pDX, IDC_EDIT_SV, m_cValueSV);
	DDX_Control(pDX, IDC_SPIN_SV, m_spinValueSV);
	DDX_Text(pDX, IDC_EDIT_SU, m_strValueSU);
	DDX_Text(pDX, IDC_EDIT_SV, m_strValueSV);
	DDX_CBIndex(pDX, IDC_COMBO_FULLSIZE, m_nTextureOption);
	DDX_Control(pDX, IDC_COMBO_TEX_FORMAT, m_cTextureFormat);
	DDX_CBIndex(pDX,IDC_COMBO_TEX_FORMAT, m_nTextureFormat);
	DDX_Check(pDX, IDC_CHECK_SHOW_TILING, m_bShowTiling);
}


BEGIN_MESSAGE_MAP(CTextureBar, CBarContent)
	ON_BN_CLICKED(IDC_BUTTON_DELETEKEY, OnBnClickedButtonDeletekey)
	ON_BN_CLICKED(IDC_BUTTON_ADDKEY, OnBnClickedButtonAddkey)
	ON_BN_CLICKED(IDC_BUTTON_TIMESET, OnBnClickedButtonTimeset)
	ON_EN_CHANGE(IDC_EDIT_U, OnEnChangeEditU)
	ON_EN_CHANGE(IDC_EDIT_V, OnEnChangeEditV)
	ON_EN_CHANGE(IDC_EDIT_R, OnEnChangeEditR)
	ON_MESSAGE( WM_COLOR_SELECTED, OnColorSelected)
	ON_MESSAGE( WM_PLAYCTRL_PROC, OnPlayCtrlProc)
	ON_MESSAGE( WM_PLAYCTRL_PLAY, OnPlayCtrlPlay)
	ON_MESSAGE( WM_PLAYCTRL_STOP, OnPlayCtrlStop)
	ON_MESSAGE( WM_PLAYCTRL_POS, OnPlayCtrlPos)
	ON_MESSAGE( WM_MARKER_MOVE, OnKeyMove)
	ON_MESSAGE( WM_MARKER_COPY, OnKeyCopy)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_SPRITE, OnLvnBegindragListSprite)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SPRITE, OnLvnItemchangedListSprite)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPRITE, OnNMClickListSprite)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_IMAGE, OnBnClickedButtonDeleteImage)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_SPRITE, OnLvnItemActivateListSprite)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, OnBnClickedButtonMovedown)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnBnClickedButtonMoveup)
	ON_EN_CHANGE(IDC_EDIT_LODBIAS, OnEnChangeEditLodbias)
	ON_CBN_SELCHANGE(IDC_COMBO_MIPFILTER, OnCbnSelchangeComboMipfilter)
	ON_BN_CLICKED(IDC_RADIO_MESH_SPHERE, OnBnClickedRadioMeshSphere)
	ON_BN_CLICKED(IDC_RADIO_MESH_PLANE, OnBnClickedRadioMeshPlane)
	ON_BN_CLICKED(IDC_RADIO_MESH_BOX, OnBnClickedRadioMeshBox)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_WM_SIZE()
	ON_EN_CHANGE(IDC_EDIT_SU, OnEnChangeEditSu)
	ON_EN_CHANGE(IDC_EDIT_SV, OnEnChangeEditSv)
	ON_CBN_SELCHANGE(IDC_COMBO_FULLSIZE, OnCbnSelchangeComboFullsize)
	ON_CBN_SELCHANGE(IDC_COMBO_TEX_FORMAT, OnCbnSelchangeComboFormat)
	ON_BN_CLICKED(IDC_CHECK_SHOW_TILING, OnBnClickedCheckShowTiling)
END_MESSAGE_MAP()


// CTextureBar 메시지 처리기입니다.

BOOL CTextureBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	HICON hIcon = AfxGetApp()->LoadIcon(IDI_ICON_SPHERE);
	m_cSphere.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_PLANE);
	m_cPlane.SetIcon(hIcon);

	hIcon = AfxGetApp()->LoadIcon(IDI_ICON_BOX);
	m_cBox.SetIcon(hIcon);

	m_cControlBox.GetClientRect(&rect);
	m_cControlBox.ClientToScreen(&rect);
	ScreenToClient(&rect);
	rect.DeflateRect( 1, 1);

	if(!m_cColorCtrl.Create(
		NULL, NULL,
		WS_CHILD|WS_VISIBLE,
		rect, this, 0))
		return FALSE;

	CListCtrl *pListCtrl = m_cImageList.GetListCtrl();
	pListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	pListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	for( int i=0; i<6; i++)
		m_cMipFilter.AddString(g_vMipFilter[i].m_strTitle);

	m_cImageList.m_DropTarget.Register(&m_cImageList);
	m_cImageList.m_pDragEnter = OnImageListDragOver;
	m_cImageList.m_pDragOver = OnImageListDragOver;
	m_cImageList.m_pDrop = OnImageListDrop;
	m_cImageList.m_pLeave = OnImageListDragLeave;

	m_spinRotation.SetRange( -100, 100);
	m_spinValueSU.SetRange( -100, 100);
	m_spinValueSV.SetRange( -100, 100);
	m_spinValueU.SetRange( -100, 100);
	m_spinValueV.SetRange( -100, 100);

	m_spinLODBias.SetRange( -100, 100);
	m_spinKey.SetRange( -100, 100);

	for( int i=0; i<SELECT_FORMAT::COUNT; i++ )
	{
		m_cTextureFormat.AddString( SELECT_FORMAT::AVAILABLE[ i ].m_strTitle);
	}

	return TRUE;
}

void CTextureBar::EnableAllCtrl( BOOL bEnable)
{
	m_spinRotation.EnableWindow(bEnable);
	m_spinLODBias.EnableWindow(bEnable);
	m_spinValueSU.EnableWindow(bEnable);
	m_spinValueSV.EnableWindow(bEnable);
	m_spinValueU.EnableWindow(bEnable);
	m_spinValueV.EnableWindow(bEnable);
	m_spinKey.EnableWindow(bEnable);

	m_cLODBias.EnableWindow(bEnable);
	m_cValueSU.EnableWindow(bEnable);
	m_cValueSV.EnableWindow(bEnable);
	m_cValueU.EnableWindow(bEnable);
	m_cValueV.EnableWindow(bEnable);
	m_cValueR.EnableWindow(bEnable);

	m_cDeleteSprite.EnableWindow(bEnable);
	m_cDownSprite.EnableWindow(bEnable);
	m_cUpSprite.EnableWindow(bEnable);

	m_cMipFilter.EnableWindow(bEnable);
	m_cImageList.EnableWindow(bEnable);
	m_cTimeSet.EnableWindow(bEnable);

	m_cDeleteKey.EnableWindow(bEnable);
	m_cAddKey.EnableWindow(bEnable);

	m_cTextureFormat.EnableWindow(bEnable);
}

LRESULT CTextureBar::OnColorSelected( WPARAM wParam, LPARAM lParam)
{
	m_cColorBox.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
	if(m_pItem)
	{
		CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();

		m_pItem->m_dwBackColor = ((CTColorCtrl::selection_colors *) lParam)->m_clrNew;
		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}

	return 0;
}

LRESULT CTextureBar::OnPlayCtrlProc( WPARAM wParam, LPARAM lParam)
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	pDoc->m_dwTick = (DWORD) wParam;

	if( lParam || pDoc->m_dwTick < m_pItem->GetTEXTURESET()->m_dwTotalTick )
	{
		pDoc->m_dwTick %= m_pItem->GetTEXTURESET()->m_dwTotalTick;
		pCtrl->SetPos(GetPosFromTick(pDoc->m_dwTick));
		UpdateCtrl();
	}
	else
	{
		pDoc->m_dwTick = m_pItem->GetTEXTURESET()->m_dwTotalTick;
		pCtrl->SetPos(m_pItem->m_dwTotalFrame);
		pCtrl->OnBnClickedButtonStop();
	}
	pDoc->RedrawAllView();

	return 0;
}

LRESULT CTextureBar::OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam)
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	if( pDoc->m_dwTick < m_dwTotalTick )
		pCtrl->m_dwBeginTick -= pDoc->m_dwTick;
	EnableAllCtrl(FALSE);

	return 0;
}

LRESULT CTextureBar::OnPlayCtrlStop( WPARAM wParam, LPARAM lParam)
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	EnableAllCtrl(TRUE);
	UpdateCtrl(GetPosFromTick(pDoc->m_dwTick));

	return 0;
}

LRESULT CTextureBar::OnPlayCtrlPos( WPARAM wParam, LPARAM lParam)
{
	UpdateCtrl(INT(lParam));
	return 0;
}

void CTextureBar::OnSize( UINT nType, int cx, int cy)
{
	CBarContent::OnSize( nType, cx, cy);

	if( m_nInitHeight == 0 )
		m_nInitHeight = cy;
	else if( m_nInitHeight < cy )
	{
		CGSDialogBar *pControlBar = (CGSDialogBar *) GetParent();
		CRect rect;

		m_cImageList.GetWindowRect(&rect);
		ScreenToClient(&rect);

		rect.bottom += cy - m_nInitHeight;
		m_nInitHeight = cy;
		m_cImageList.MoveWindow(&rect);

		GetWindowRect(&rect);
		pControlBar->m_szContent = rect.Size();
		pControlBar->m_szDefault = rect.Size();
	}
}

void CTextureBar::SetItem( CTextureItem *pItem)
{
	m_nMipFilter = GetFilterIndex(pItem->GetTEXTURESET()->m_dwMipFilter);
	m_strLODBias.Format( "%.2f", pItem->GetTEXTURESET()->m_fMipBias);

	m_dwTotalTick = pItem->GetTEXTURESET()->m_dwTotalTick;
	m_dwTotalFrame = pItem->m_dwTotalFrame;
	m_cColorCtrl.SetupColor( pItem->m_dwBackColor, TRUE);
	m_pItem = pItem;

	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	CListCtrl *pList = m_cImageList.GetListCtrl();
	int nCount = INT(m_pItem->GetTEXTURESET()->m_vKey.size());

	pCtrl->SetRange( 0, m_dwTotalFrame);
	pCtrl->m_cMarker.DelAll();
	pList->DeleteAllItems();

	for( int i=0; i<nCount; i++)
		pCtrl->m_cMarker.AddMark(GetPosFromTick(m_pItem->GetTEXTURESET()->m_vKey[i]->m_dwTick));

	nCount = INT(m_pItem->m_vTextureID.size());
	for( i=0; i<nCount; i++)
	{
		CTextureItem *pTextureItem = CItemBase::GetTextureItem(m_pItem->m_vTextureID[i]);
		int nIndex = pList->InsertItem( i, pTextureItem->m_strName);

		pList->SetItemData( nIndex, (DWORD_PTR) pTextureItem);
	}
	m_cImageList.UpdateColumnWidth();

	m_cSphere.SetCheck(pDoc->m_bRenderType == RT_SPHERE ? 1 : 0);
	m_cPlane.SetCheck(pDoc->m_bRenderType == RT_PLANE ? 1 : 0);
	m_cBox.SetCheck(pDoc->m_bRenderType == RT_BOX ? 1 : 0);
	m_nTextureOption = m_pItem->m_bTextureOption;
	m_nTextureFormat = GetTextureCompressionIndex(pItem->m_bTextureFormat);

	UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
}

DWORD CTextureBar::GetTickFromIndex( int nIndex)
{
	int nCount = INT(m_pItem->m_vTextureID.size());
	DWORD dwTick = nIndex * m_pItem->GetTEXTURESET()->m_dwTotalTick / nCount;

	if(nIndex * m_pItem->GetTEXTURESET()->m_dwTotalTick % nCount)
		dwTick++;

	return dwTick;
}

DWORD CTextureBar::GetTickFromPos( int nPos)
{
	return m_dwTotalFrame ? nPos * m_dwTotalTick / m_dwTotalFrame : 0;
}

int CTextureBar::GetIndexFromTick( DWORD dwTick)
{
	int nCount = INT(m_pItem->m_vTextureID.size());
	int nIndex = dwTick * nCount / m_pItem->GetTEXTURESET()->m_dwTotalTick;

	return nIndex < nCount ? nIndex : nIndex - 1;
}

int CTextureBar::GetPosFromTick( DWORD dwTick)
{
	int nPos = dwTick * m_dwTotalFrame / m_dwTotalTick;
	return dwTick * m_dwTotalFrame % m_dwTotalTick ? nPos + 1 : nPos;
}

int CTextureBar::GetFilterIndex( DWORD dwFilter)
{
	for( int i=0; i<6; i++)
		if( g_vMipFilter[i].m_dwFilter == dwFilter )
			return i;

	return -1;
}

int CTextureBar::GetTextureCompressionIndex( BYTE bFormat)
{
	for( int i=0; i<SELECT_FORMAT::COUNT; i++)
	{
		if( SELECT_FORMAT::AVAILABLE[ i ].m_bFormat == bFormat )
			return i;
	}

	return -1;
}

void CTextureBar::UpdateCtrl( int nPos)
{
	if(m_pItem)
	{
		CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();

		pDoc->m_dwTick = GetTickFromPos(nPos);
		UpdateCtrl();
	}
}

void CTextureBar::UpdateCtrl()
{
	if(m_pItem)
	{
		CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nIndex = m_pItem->GetTEXTURESET()->FindKey(pDoc->m_dwTick);
		m_cImageList.SetCurSel(GetIndexFromTick(pDoc->m_dwTick));

		m_pItem->GetTEXTURESET()->m_dwCurTick = pDoc->m_dwTick;
		UVKEY vKey = m_pItem->GetTEXTURESET()->GetUV();
		BOOL bEnable = nIndex != -1;

		if(!pCtrl->m_bPlay)
		{
			m_spinRotation.EnableWindow(bEnable);
			m_spinValueSU.EnableWindow(bEnable);
			m_spinValueSV.EnableWindow(bEnable);
			m_spinValueU.EnableWindow(bEnable);
			m_spinValueV.EnableWindow(bEnable);
			m_cValueSU.EnableWindow(bEnable);
			m_cValueSV.EnableWindow(bEnable);
			m_cValueU.EnableWindow(bEnable);
			m_cValueV.EnableWindow(bEnable);
			m_cValueR.EnableWindow(bEnable);

			m_cDeleteKey.EnableWindow(bEnable);
			m_cAddKey.EnableWindow(!bEnable);
		}

		m_strRotation.Format( "%.2f", vKey.m_fKeyR * 180.0f / D3DX_PI);
		m_strValueSU.Format( "%.2f", vKey.m_fKeySU);
		m_strValueSV.Format( "%.2f", vKey.m_fKeySV);
		m_strValueU.Format( "%.2f", vKey.m_fKeyU);
		m_strValueV.Format( "%.2f", vKey.m_fKeyV);
		m_dwFrame = pCtrl->m_cPlayCtrl.GetPos();

		if(bEnable)
			m_strKey.Format( "%d", nIndex);
		else
			m_strKey.Format("");

		if(!pCtrl->m_bPlay)
			pDoc->RedrawAllView();
		UpdateData(FALSE);
	}
}

void CTextureBar::OnBnClickedButtonAddkey()
{
	if( m_pItem && m_dwTotalFrame )
	{
		CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->GetTEXTURESET()->FindKey(pDoc->m_dwTick);

		if( nIndex == -1 )
		{
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
			LPUVKEY pKey = new UVKEY();

			m_pItem->GetTEXTURESET()->m_dwCurTick = pDoc->m_dwTick;
			*pKey = m_pItem->GetTEXTURESET()->GetUV();
			m_pItem->GetTEXTURESET()->InsertKey(pKey);

			int nPos = pCtrl->m_cPlayCtrl.GetPos();
			pCtrl->m_cMarker.AddMark(nPos - pCtrl->m_cPlayCtrl.GetRangeMin());
			UpdateCtrl(nPos);
		}
	}
}

void CTextureBar::OnBnClickedButtonDeletekey()
{
	if( m_pItem && m_dwTotalFrame )
	{
		CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->GetTEXTURESET()->FindKey(pDoc->m_dwTick);

		if( nIndex != -1 )
		{
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
			int nPos = pCtrl->m_cPlayCtrl.GetPos();

			pCtrl->m_cMarker.DelMark(nPos - pCtrl->m_cPlayCtrl.GetRangeMin());
			m_pItem->GetTEXTURESET()->DeleteKey(pDoc->m_dwTick);
			UpdateCtrl(nPos);
		}
	}
}

void CTextureBar::OnEnChangeEditU()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->GetTEXTURESET()->FindKey(pDoc->m_dwTick);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeyU = (FLOAT) atof(m_strValueU);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CTextureBar::OnEnChangeEditV()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->GetTEXTURESET()->FindKey(pDoc->m_dwTick);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeyV = (FLOAT) atof(m_strValueV);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CTextureBar::OnEnChangeEditSu()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->GetTEXTURESET()->FindKey(pDoc->m_dwTick);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeySU = (FLOAT) atof(m_strValueSU);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CTextureBar::OnEnChangeEditSv()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->GetTEXTURESET()->FindKey(pDoc->m_dwTick);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeySV = (FLOAT) atof(m_strValueSV);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CTextureBar::OnEnChangeEditR()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->GetTEXTURESET()->FindKey(pDoc->m_dwTick);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeyR = (FLOAT) atof(m_strRotation) * D3DX_PI / 180.0f;

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CTextureBar::OnEnChangeEditLodbias()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();

	UpdateData();
	m_pItem->GetTEXTURESET()->m_fMipBias = (FLOAT) atof(m_strLODBias);

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
}

void CTextureBar::OnCbnSelchangeComboMipfilter()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();

	UpdateData();
	if( m_nMipFilter != CB_ERR )
		m_pItem->GetTEXTURESET()->m_dwMipFilter = g_vMipFilter[m_nMipFilter].m_dwFilter;

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
}

LRESULT CTextureBar::OnKeyMove( WPARAM wParam, LPARAM lParam)
{
	int nFrom = INT(wParam);
	int nTo = INT(lParam);

	if(m_pItem)
	{
		CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nMin = pCtrl->m_cPlayCtrl.GetRangeMin();

		nFrom += nMin;
		nTo += nMin;

		if( nTo != -1 )
		{
			VECTORUVKEY::iterator it = m_pItem->GetTEXTURESET()->m_vKey.begin();
			DWORD dwFrom = GetTickFromPos(nFrom);
			DWORD dwTo = GetTickFromPos(nTo);
			LPUVKEY pKey = NULL;

			m_pItem->GetTEXTURESET()->DeleteKey(dwTo);
			while( it != m_pItem->GetTEXTURESET()->m_vKey.end() )
				if( (*it)->m_dwTick == dwFrom )
				{
					pKey = (*it);
					m_pItem->GetTEXTURESET()->m_vKey.erase(it);

					break;
				}
				else
					it++;
			m_pItem->GetTEXTURESET()->DeleteKey(dwFrom);

			if(pKey)
			{
				pKey->m_dwTick = dwTo;
				m_pItem->GetTEXTURESET()->InsertKey(pKey);
			}
		}
		else
			m_pItem->GetTEXTURESET()->DeleteKey(GetTickFromPos(nFrom));

		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}

	return 0;
}

LRESULT CTextureBar::OnKeyCopy( WPARAM wParam, LPARAM lParam)
{
	int nFrom = INT(wParam);
	int nTo = INT(lParam);

	if(m_pItem)
	{
		CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nMin = pCtrl->m_cPlayCtrl.GetRangeMin();
		int nIndex = -1;

		nFrom += nMin;
		nTo += nMin;

		DWORD dwFrom = GetTickFromPos(nFrom);
		DWORD dwTo = GetTickFromPos(nTo);

		m_pItem->GetTEXTURESET()->DeleteKey(dwTo);
		nIndex = m_pItem->GetTEXTURESET()->FindKey(dwFrom);

		if( nIndex != -1 )
		{
			LPUVKEY pKey = new UVKEY();

			pKey->m_fKeySU = m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeySU;
			pKey->m_fKeySV = m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeySV;
			pKey->m_fKeyU = m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeyU;
			pKey->m_fKeyV = m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeyV;
			pKey->m_fKeyR = m_pItem->GetTEXTURESET()->m_vKey[nIndex]->m_fKeyR;
			pKey->m_dwTick = dwTo;

			m_pItem->GetTEXTURESET()->InsertKey(pKey);
		}

		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}

	return 0;
}

void CTextureBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinLODBias )
	{
		FLOAT fBias = (FLOAT) atof(m_strLODBias);

		fBias += ((FLOAT) INT(nPos)) / 2.0f;
		m_spinLODBias.SetPos(0);

		m_strLODBias.Format( "%.2f", fBias);
		UpdateData(FALSE);
		OnEnChangeEditLodbias();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueSU )
	{
		FLOAT fValueSU = (FLOAT) atof(m_strValueSU);

		fValueSU += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueSU.SetPos(0);

		m_strValueSU.Format( "%.2f", fValueSU);
		UpdateData(FALSE);
		OnEnChangeEditSu();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueSV )
	{
		FLOAT fValueSV = (FLOAT) atof(m_strValueSV);

		fValueSV += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueSV.SetPos(0);

		m_strValueSV.Format( "%.2f", fValueSV);
		UpdateData(FALSE);
		OnEnChangeEditSv();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueU )
	{
		FLOAT fValueU = (FLOAT) atof(m_strValueU);

		fValueU += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueU.SetPos(0);

		m_strValueU.Format( "%.2f", fValueU);
		UpdateData(FALSE);
		OnEnChangeEditU();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueV )
	{
		FLOAT fValueV = (FLOAT) atof(m_strValueV);

		fValueV += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueV.SetPos(0);

		m_strValueV.Format( "%.2f", fValueV);
		UpdateData(FALSE);
		OnEnChangeEditV();
	}

	if( pScrollBar == (CScrollBar *) &m_spinRotation )
	{
		FLOAT fRotation = (FLOAT) atof(m_strRotation);

		fRotation += (FLOAT) INT(nPos);
		m_spinRotation.SetPos(0);

		m_strRotation.Format( "%.2f", fRotation);
		UpdateData(FALSE);
		OnEnChangeEditR();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CTextureBar::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinKey && nPos != 0 &&
		!m_pItem->GetTEXTURESET()->m_vKey.empty() )
	{
		CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nCount = INT(m_pItem->GetTEXTURESET()->m_vKey.size());
		int nPrev = nCount - 1;
		int nNext = 0;

		for( int i=0; i<nCount; i++)
		{
			if( pDoc->m_dwTick < m_pItem->GetTEXTURESET()->m_vKey[i]->m_dwTick )
			{
				nNext = i;
				break;
			}

			if( pDoc->m_dwTick == m_pItem->GetTEXTURESET()->m_vKey[i]->m_dwTick )
			{
				nNext = i + 1 < nCount ? i + 1 : 0;
				break;
			}

			nPrev = i;
		}

		if( INT(nPos) < 0 )
			nNext = INT(m_pItem->GetTEXTURESET()->m_vKey[nPrev]->m_dwTick);
		else
			nNext = INT(m_pItem->GetTEXTURESET()->m_vKey[nNext]->m_dwTick);

		nNext = GetPosFromTick((DWORD) nNext);
		m_spinKey.SetPos(0);

		pCtrl->m_cPlayCtrl.SetPos(nNext);
		UpdateCtrl(nNext);
	}

	CBarContent::OnHScroll( nSBCode, nPos, pScrollBar);
}

void CTextureBar::OnBnClickedButtonTimeset()
{
	CTimeSettingDlg dlg;

	dlg.m_dwTotalFrame = m_dwTotalFrame;
	dlg.m_dwTotalTick = m_dwTotalTick;

	if( dlg.DoModal() == IDOK )
	{
		CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nCount = INT(m_pItem->GetTEXTURESET()->m_vKey.size());
		DWORD dwTotalFrame = m_dwTotalFrame;
		VECTORUVKEY vKey;

		for( int i=0; i<nCount; i++)
		{
			m_pItem->GetTEXTURESET()->m_vKey[i]->m_dwTick = (DWORD) GetPosFromTick(m_pItem->GetTEXTURESET()->m_vKey[i]->m_dwTick);
			vKey.push_back(m_pItem->GetTEXTURESET()->m_vKey[i]);
		}
		m_pItem->GetTEXTURESET()->m_vKey.clear();

		m_dwTotalFrame = dlg.m_dwTotalFrame;
		m_dwTotalTick = dlg.m_dwTotalTick;

		for( i=0; i<nCount; i++)
		{
			vKey[i]->m_dwTick = GetTickFromPos(vKey[i]->m_dwTick * m_dwTotalFrame / dwTotalFrame);
			m_pItem->GetTEXTURESET()->DeleteKey(vKey[i]->m_dwTick);
			m_pItem->GetTEXTURESET()->InsertKey(vKey[i]);
		}
		vKey.clear();

		m_pItem->GetTEXTURESET()->m_dwTotalTick = m_dwTotalTick;
		m_pItem->m_dwTotalFrame = m_dwTotalFrame;

		pCtrl->SetRange( 0, m_dwTotalFrame);
		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}
}

void CALLBACK CTextureBar::OnImageListDragLeave( CGSListCtrl *pList)
{
	CTextureBar *pBar = (CTextureBar *) pList->GetParent();
	CTextureDoc *pDoc = (CTextureDoc *) pBar->m_pItem->GetMainDocument();

	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));
	pList->SetCurSel(pBar->GetIndexFromTick(pDoc->m_dwTick));

	pList->CListView::OnDragLeave();
}

DROPEFFECT CALLBACK CTextureBar::OnImageListDragOver( CGSListCtrl *pList,
													  COleDataObject *pDataObject,
													  DWORD dwKeyState,
													  CPoint point)
{
	CTextureBar *pBar = (CTextureBar *) pList->GetParent();
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

			if(CanImageListDrop(pItem))
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

BOOL CALLBACK CTextureBar::OnImageListDrop( CGSListCtrl *pList,
										    COleDataObject* pDataObject,
											DROPEFFECT dropEffect,
											CPoint point)
{
	CTextureBar *pBar = (CTextureBar *) pList->GetParent();

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

			if(CanImageListDrop(pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->InsertTexture( (CTextureItem *) pItem, pList->GetListCtrl()->HitTest(point));
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnImageListDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CTextureBar::CanImageListDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_TEXTURE && ((CTextureItem *) pItem)->GetLPDIRECT3DTEXTURE9();
}

void CTextureBar::OnLvnBegindragListSprite( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	HGLOBAL hGlobal = ::GlobalAlloc( GMEM_SHARE|GMEM_MOVEABLE, sizeof(GSCDATA));
	COleDataSource source;
	GSLISTITEM vItem;

	CGameStudioView::m_pDragImage = m_cImageList.GetListCtrl()->CreateDragImage(
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

void CTextureBar::OnLvnItemchangedListSprite( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( m_cImageList.m_bUISelect && (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		MoveToImagePos(pNMLV->iItem);

	*pResult = 0;
}

void CTextureBar::OnNMClickListSprite( NMHDR *pNMHDR, LRESULT *pResult)
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	int nIndex = m_cImageList.GetCurSel();

	if( nIndex == -1 )
		m_cImageList.SetCurSel(GetIndexFromTick(pDoc->m_dwTick));

	*pResult = 0;
}

void CTextureBar::MoveToImagePos( int nIndex)
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	nIndex = GetPosFromTick(GetTickFromIndex(nIndex));
	pCtrl->m_cPlayCtrl.SetPos(nIndex);
	UpdateCtrl(nIndex);
}

void CTextureBar::InsertTexture( CTextureItem *pItem, int nIndex)
{
	CListCtrl *pListCtrl = m_cImageList.GetListCtrl();

	if( nIndex == -1 )
		nIndex = INT(m_pItem->m_vTextureID.size());

	m_pItem->m_vTextureID.insert(
		m_pItem->m_vTextureID.begin() + nIndex,
		pItem->m_dwID);

	m_pItem->GetTEXTURESET()->InsertTextureItem( nIndex, pItem);

	nIndex = pListCtrl->InsertItem( nIndex, pItem->m_strName);
	pListCtrl->SetItemData( nIndex, (DWORD_PTR) pItem);

	if( !m_pItem->GetLPDIRECT3DTEXTURE9() && CGameStudioDoc::m_pPreview &&
		CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem == m_pItem )
		CGameStudioDoc::m_pPreview->Invalidate();

	m_cImageList.UpdateColumnWidth();
	MoveToImagePos(nIndex);
}

void CTextureBar::DeleteTexture( int nIndex)
{
	CListCtrl *pListCtrl = m_cImageList.GetListCtrl();

	m_pItem->GetTEXTURESET()->EraseTextureItem( nIndex);

	m_pItem->m_vTextureID.erase(m_pItem->m_vTextureID.begin() + nIndex);

	pListCtrl->DeleteItem(nIndex);
	m_cImageList.UpdateColumnWidth();

	if( !m_pItem->GetLPDIRECT3DTEXTURE9() && CGameStudioDoc::m_pPreview &&
		CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem == m_pItem )
		CGameStudioDoc::m_pPreview->Invalidate();

	UpdateCtrl();
}

void CTextureBar::MoveItemPos( int nFrom, int nTo)
{
	if( nFrom == nTo )
		return;

	CListCtrl *pListCtrl = m_cImageList.GetListCtrl();
	CTextureItem *pItem = (CTextureItem *) pListCtrl->GetItemData(nFrom);

	DeleteTexture(nFrom);
	InsertTexture( pItem, nTo);
}

void CTextureBar::OnBnClickedButtonDeleteImage()
{
	DeleteTexture(m_cImageList.GetCurSel());
}

void CTextureBar::OnLvnItemActivateListSprite(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

//	DeleteTexture(pNMIA->iItem);
	*pResult = 0;
}

void CTextureBar::OnBnClickedButtonMovedown()
{
	int nIndex = m_cImageList.GetCurSel();

	if( nIndex == INT(m_pItem->m_vTextureID.size() - 1) )
		MoveItemPos( nIndex, 0);
	else
		MoveItemPos( nIndex, nIndex + 1);
}

void CTextureBar::OnBnClickedButtonMoveup()
{
	int nIndex = m_cImageList.GetCurSel();

	if( nIndex == 0 )
		MoveItemPos( nIndex, INT(m_pItem->m_vTextureID.size() - 1));
	else
		MoveItemPos( nIndex, nIndex - 1);
}

void CTextureBar::OnBnClickedRadioMeshSphere()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();

	D3DXMatrixIdentity(&pDoc->m_matObject);
	pDoc->m_bRenderType = RT_SPHERE;

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
}

void CTextureBar::OnBnClickedRadioMeshPlane()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();

	D3DXMatrixIdentity(&pDoc->m_matObject);
	pDoc->m_bRenderType = RT_PLANE;

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
}

void CTextureBar::OnBnClickedRadioMeshBox()
{
	CTextureDoc *pDoc = (CTextureDoc *) m_pItem->GetMainDocument();

	D3DXMatrixIdentity(&pDoc->m_matObject);
	pDoc->m_bRenderType = RT_BOX;

	if(!pDoc->GetPlayCtrl()->m_bPlay)
		pDoc->RedrawAllView();
}

void CTextureBar::OnCbnSelchangeComboFullsize()
{
	if(m_pItem)
	{
		UpdateData();
		m_pItem->m_bTextureOption = BYTE(m_nTextureOption);
	}
}

void CTextureBar::OnCbnSelchangeComboFormat()
{
	if(m_pItem)
	{
		UpdateData();

		if( m_nTextureFormat != CB_ERR )
			m_pItem->m_bTextureFormat = SELECT_FORMAT::AVAILABLE[m_nTextureFormat].m_bFormat;
	}
}

void CTextureBar::OnBnClickedCheckShowTiling()
{
	UpdateData();
}
