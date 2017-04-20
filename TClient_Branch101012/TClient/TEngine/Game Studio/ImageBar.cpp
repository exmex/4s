// ImageBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "ImageBar.h"
#include "GSDialogBar.h"
#include "ImageDoc.h"
#include "TimeSettingDlg.h"

// CImageBar 대화 상자입니다.

IMPLEMENT_DYNAMIC( CImageBar, CBarContent)
CImageBar::CImageBar( CWnd* pParent /*=NULL*/)
: CBarContent(CImageBar::IDD, pParent)
, m_dwTotalFrame(0)
, m_dwTotalTick(0)
, m_dwFrame(0)
, m_strKey(_T(""))
, m_dwValueA(0)
, m_dwValueR(0)
, m_dwValueG(0)
, m_dwValueB(0)
, m_nImageFormat(0)
{
	m_nInitHeight = 0;
	m_pItem = NULL;
}

CImageBar::~CImageBar()
{
}

void CImageBar::DoDataExchange(CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TOTAL_FRAME, m_dwTotalFrame);
	DDX_Text(pDX, IDC_STATIC_TOTAL_TICK, m_dwTotalTick);
	DDX_Text(pDX, IDC_STATIC_FRAME, m_dwFrame);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_cColorBox);
	DDX_Control(pDX, IDC_STATIC_CLRCTRL, m_cControlBox);
	DDX_Control(pDX, IDC_LIST_SPRITE, m_cImageList);
	DDX_Control(pDX, IDC_SPIN_A, m_spinValueA);
	DDX_Control(pDX, IDC_SPIN_R, m_spinValueR);
	DDX_Control(pDX, IDC_SPIN_G, m_spinValueG);
	DDX_Control(pDX, IDC_SPIN_B, m_spinValueB);
	DDX_Control(pDX, IDC_SPIN_KEY, m_spinKey);
	DDX_Control(pDX, IDC_EDIT_A, m_cValueA);
	DDX_Control(pDX, IDC_EDIT_R, m_cValueR);
	DDX_Control(pDX, IDC_EDIT_G, m_cValueG);
	DDX_Control(pDX, IDC_EDIT_B, m_cValueB);
	DDX_Text(pDX, IDC_STATIC_KEY, m_strKey);
	DDX_Text(pDX, IDC_EDIT_A, m_dwValueA);
	DDX_Text(pDX, IDC_EDIT_R, m_dwValueR);
	DDX_Text(pDX, IDC_EDIT_G, m_dwValueG);
	DDX_Text(pDX, IDC_EDIT_B, m_dwValueB);
	DDV_MinMaxUInt(pDX, m_dwValueA, 0, 255);
	DDV_MinMaxUInt(pDX, m_dwValueR, 0, 255);
	DDV_MinMaxUInt(pDX, m_dwValueG, 0, 255);
	DDV_MinMaxUInt(pDX, m_dwValueB, 0, 255);
	DDX_Control(pDX, IDC_BUTTON_DELETEKEY, m_cDeleteKey);
	DDX_Control(pDX, IDC_BUTTON_ADDKEY, m_cAddKey);
	DDX_Control(pDX, IDC_BUTTON_DELETE_IMAGE, m_cDeleteSprite);
	DDX_Control(pDX, IDC_BUTTON_MOVEDOWN, m_cDownSprite);
	DDX_Control(pDX, IDC_BUTTON_MOVEUP, m_cUpSprite);
	DDX_Control(pDX, IDC_BUTTON_TIMESET, m_cTimeSet);
	DDX_CBIndex(pDX, IDC_COMBO_IMG_FORMAT, m_nImageFormat);
	DDX_Control(pDX, IDC_COMBO_IMG_FORMAT, m_cImageFormat);
}


BEGIN_MESSAGE_MAP(CImageBar, CBarContent)
	ON_MESSAGE( WM_COLOR_SELECTED, OnColorSelected)
	ON_MESSAGE( WM_PLAYCTRL_PROC, OnPlayCtrlProc)
	ON_MESSAGE( WM_PLAYCTRL_PLAY, OnPlayCtrlPlay)
	ON_MESSAGE( WM_PLAYCTRL_STOP, OnPlayCtrlStop)
	ON_MESSAGE( WM_PLAYCTRL_POS, OnPlayCtrlPos)
	ON_MESSAGE( WM_MARKER_MOVE, OnKeyMove)
	ON_MESSAGE( WM_MARKER_COPY, OnKeyCopy)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_ADDKEY, OnBnClickedButtonAddkey)
	ON_BN_CLICKED(IDC_BUTTON_DELETEKEY, OnBnClickedButtonDeletekey)
	ON_EN_CHANGE(IDC_EDIT_A, OnEnChangeEditA)
	ON_EN_CHANGE(IDC_EDIT_R, OnEnChangeEditR)
	ON_EN_CHANGE(IDC_EDIT_G, OnEnChangeEditG)
	ON_EN_CHANGE(IDC_EDIT_B, OnEnChangeEditB)
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_TIMESET, OnBnClickedButtonTimeset)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_SPRITE, OnLvnBegindragListSprite)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_SPRITE, OnLvnItemchangedListSprite)
	ON_NOTIFY(NM_CLICK, IDC_LIST_SPRITE, OnNMClickListSprite)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_IMAGE, OnBnClickedButtonDeleteImage)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_SPRITE, OnLvnItemActivateListSprite)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, OnBnClickedButtonMovedown)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnBnClickedButtonMoveup)
	ON_CBN_SELCHANGE(IDC_COMBO_IMG_FORMAT, OnCbnSelchangeComboImgFormat)
END_MESSAGE_MAP()


// CImageBar 메시지 처리기입니다.

BOOL CImageBar::OnInitDialog()
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

	CListCtrl *pListCtrl = m_cImageList.GetListCtrl();
	pListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	pListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cImageList.m_DropTarget.Register(&m_cImageList);
	m_cImageList.m_pDragEnter = OnImageListDragOver;
	m_cImageList.m_pDragOver = OnImageListDragOver;
	m_cImageList.m_pDrop = OnImageListDrop;
	m_cImageList.m_pLeave = OnImageListDragLeave;

	m_spinValueA.SetRange( 0, 255);
	m_spinValueR.SetRange( 0, 255);
	m_spinValueG.SetRange( 0, 255);
	m_spinValueB.SetRange( 0, 255);
	m_spinKey.SetRange( -100, 100);

	for( INT i=0 ; i < SELECT_FORMAT::COUNT ; ++i )
	{
		m_cImageFormat.AddString( SELECT_FORMAT::AVAILABLE[ i ].m_strTitle);
	}

	return TRUE;
}

void CImageBar::EnableAllCtrl( BOOL bEnable)
{
	m_spinValueA.EnableWindow(bEnable);
	m_spinValueR.EnableWindow(bEnable);
	m_spinValueG.EnableWindow(bEnable);
	m_spinValueB.EnableWindow(bEnable);
	m_spinKey.EnableWindow(bEnable);

	m_cValueA.EnableWindow(bEnable);
	m_cValueR.EnableWindow(bEnable);
	m_cValueG.EnableWindow(bEnable);
	m_cValueB.EnableWindow(bEnable);

	m_cDeleteSprite.EnableWindow(bEnable);
	m_cDownSprite.EnableWindow(bEnable);
	m_cUpSprite.EnableWindow(bEnable);

	m_cImageList.EnableWindow(bEnable);
	m_cTimeSet.EnableWindow(bEnable);

	m_cDeleteKey.EnableWindow(bEnable);
	m_cAddKey.EnableWindow(bEnable);
}

LRESULT CImageBar::OnColorSelected( WPARAM wParam, LPARAM lParam)
{
	m_cColorBox.SetColor(((CTColorCtrl::selection_colors *) lParam)->m_clrNew);
	if(m_pItem)
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();

		m_pItem->m_dwBackColor = ((CTColorCtrl::selection_colors *) lParam)->m_clrNew;
		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}

	return 0;
}

LRESULT CImageBar::OnPlayCtrlProc( WPARAM wParam, LPARAM lParam)
{
	CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	pDoc->m_dwTick = (DWORD) wParam;

	if( lParam || pDoc->m_dwTick < m_pItem->m_ImageSet.m_dwTotalTick )
	{
		pDoc->m_dwTick %= m_pItem->m_ImageSet.m_dwTotalTick;
		pCtrl->SetPos(GetPosFromTick(pDoc->m_dwTick));
		UpdateCtrl();
	}
	else
	{
		pDoc->m_dwTick = m_pItem->m_ImageSet.m_dwTotalTick;
		pCtrl->SetPos(m_pItem->m_dwTotalFrame);
		pCtrl->OnBnClickedButtonStop();
	}
	pDoc->RedrawAllView();

	return 0;
}

LRESULT CImageBar::OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam)
{
	CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	if( pDoc->m_dwTick < m_dwTotalTick )
		pCtrl->m_dwBeginTick -= pDoc->m_dwTick;
	EnableAllCtrl(FALSE);

	return 0;
}

LRESULT CImageBar::OnPlayCtrlStop( WPARAM wParam, LPARAM lParam)
{
	CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	EnableAllCtrl(TRUE);
	UpdateCtrl(GetPosFromTick(pDoc->m_dwTick));

	return 0;
}

LRESULT CImageBar::OnPlayCtrlPos( WPARAM wParam, LPARAM lParam)
{
	UpdateCtrl(INT(lParam));
	return 0;
}

void CImageBar::OnSize( UINT nType, int cx, int cy)
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

void CImageBar::SetItem( CImageItem *pItem)
{
	m_dwTotalTick = pItem->m_ImageSet.m_dwTotalTick;
	m_dwTotalFrame = pItem->m_dwTotalFrame;
	m_cColorCtrl.SetupColor( pItem->m_dwBackColor, TRUE);
	m_pItem = pItem;

	CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	CListCtrl *pList = m_cImageList.GetListCtrl();
	int nCount = INT(m_pItem->m_ImageSet.m_vKey.size());

	pCtrl->SetRange( 0, m_dwTotalFrame);
	pCtrl->m_cMarker.DelAll();
	pList->DeleteAllItems();

	for( int i=0; i<nCount; i++)
		pCtrl->m_cMarker.AddMark(GetPosFromTick(m_pItem->m_ImageSet.m_vKey[i]->m_dwTick));

	nCount = INT(m_pItem->m_vImageID.size());
	for( i=0; i<nCount; i++)
	{
		CImageItem *pImageItem = CItemBase::GetImageItem(m_pItem->m_vImageID[i]);
		int nIndex = pList->InsertItem( i, pImageItem->m_strName);

		pList->SetItemData( nIndex, (DWORD_PTR) pImageItem);
	}
	m_cImageList.UpdateColumnWidth();
	m_nImageFormat = GetImageCompressionIndex(m_pItem->m_bImageFormat);

	UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
}

DWORD CImageBar::GetTickFromIndex( int nIndex)
{
	int nCount = INT(m_pItem->m_vImageID.size());
	DWORD dwTick = nIndex * m_pItem->m_ImageSet.m_dwTotalTick / nCount;

	if(nIndex * m_pItem->m_ImageSet.m_dwTotalTick % nCount)
		dwTick++;

	return dwTick;
}

DWORD CImageBar::GetTickFromPos( int nPos)
{
	return m_dwTotalFrame ? nPos * m_dwTotalTick / m_dwTotalFrame : 0;
}

int CImageBar::GetIndexFromTick( DWORD dwTick)
{
	int nCount = INT(m_pItem->m_vImageID.size());
	int nIndex = dwTick * nCount / m_pItem->m_ImageSet.m_dwTotalTick;

	return nIndex < nCount ? nIndex : nIndex - 1;
}

int CImageBar::GetPosFromTick( DWORD dwTick)
{
	int nPos = dwTick * m_dwTotalFrame / m_dwTotalTick;
	return dwTick * m_dwTotalFrame % m_dwTotalTick ? nPos + 1 : nPos;
}

void CImageBar::UpdateCtrl( int nPos)
{
	if(m_pItem)
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();

		pDoc->m_dwTick = GetTickFromPos(nPos);
		UpdateCtrl();
	}
}

void CImageBar::UpdateCtrl()
{
	if(m_pItem)
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nIndex = m_pItem->m_ImageSet.FindKey(pDoc->m_dwTick);
		m_cImageList.SetCurSel(GetIndexFromTick(pDoc->m_dwTick));

		m_pItem->m_ImageSet.m_dwCurTick = pDoc->m_dwTick;
		DWORD dwColor = m_pItem->m_ImageSet.GetColor();
		BOOL bEnable = nIndex != -1;

		if(!pCtrl->m_bPlay)
		{
			m_spinValueA.EnableWindow(bEnable);
			m_spinValueR.EnableWindow(bEnable);
			m_spinValueG.EnableWindow(bEnable);
			m_spinValueB.EnableWindow(bEnable);
			m_cValueA.EnableWindow(bEnable);
			m_cValueR.EnableWindow(bEnable);
			m_cValueG.EnableWindow(bEnable);
			m_cValueB.EnableWindow(bEnable);

			m_cDeleteKey.EnableWindow(bEnable);
			m_cAddKey.EnableWindow(!bEnable);
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
}

void CImageBar::OnBnClickedButtonAddkey()
{
	if( m_pItem && m_dwTotalFrame )
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_ImageSet.FindKey(pDoc->m_dwTick);

		if( nIndex == -1 )
		{
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
			LPCLRKEY pKey = new CLRKEY();

			m_pItem->m_ImageSet.m_dwCurTick = pDoc->m_dwTick;
			pKey->m_dwColor = m_pItem->m_ImageSet.GetColor();
			pKey->m_dwTick = pDoc->m_dwTick;
			m_pItem->m_ImageSet.InsertKey(pKey);

			int nPos = pCtrl->m_cPlayCtrl.GetPos();
			pCtrl->m_cMarker.AddMark(nPos - pCtrl->m_cPlayCtrl.GetRangeMin());
			UpdateCtrl(nPos);
		}
	}
}

void CImageBar::OnBnClickedButtonDeletekey()
{
	if( m_pItem && m_dwTotalFrame )
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_ImageSet.FindKey(pDoc->m_dwTick);

		if( nIndex != -1 )
		{
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
			int nPos = pCtrl->m_cPlayCtrl.GetPos();

			pCtrl->m_cMarker.DelMark(nPos - pCtrl->m_cPlayCtrl.GetRangeMin());
			m_pItem->m_ImageSet.DeleteKey(pDoc->m_dwTick);
			UpdateCtrl(nPos);
		}
	}
}

void CImageBar::OnEnChangeEditA()
{
	if(m_pItem)
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_ImageSet.FindKey(pDoc->m_dwTick);

		if( nIndex != -1 )
		{
			UpdateData();
			m_pItem->m_ImageSet.m_vKey[nIndex]->m_dwColor &= 0x00FFFFFF;
			m_pItem->m_ImageSet.m_vKey[nIndex]->m_dwColor |= ((m_dwValueA & 0x000000FF) << 24);

			if(!pDoc->GetPlayCtrl()->m_bPlay)
				pDoc->RedrawAllView();
		}
	}
}

void CImageBar::OnEnChangeEditR()
{
	if(m_pItem)
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_ImageSet.FindKey(pDoc->m_dwTick);

		if( nIndex != -1 )
		{
			UpdateData();
			m_pItem->m_ImageSet.m_vKey[nIndex]->m_dwColor &= 0xFF00FFFF;
			m_pItem->m_ImageSet.m_vKey[nIndex]->m_dwColor |= ((m_dwValueR & 0x000000FF) << 16);

			if(!pDoc->GetPlayCtrl()->m_bPlay)
				pDoc->RedrawAllView();
		}
	}
}

void CImageBar::OnEnChangeEditG()
{
	if(m_pItem)
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_ImageSet.FindKey(pDoc->m_dwTick);

		if( nIndex != -1 )
		{
			UpdateData();
			m_pItem->m_ImageSet.m_vKey[nIndex]->m_dwColor &= 0xFFFF00FF;
			m_pItem->m_ImageSet.m_vKey[nIndex]->m_dwColor |= ((m_dwValueG & 0x000000FF) << 8);

			if(!pDoc->GetPlayCtrl()->m_bPlay)
				pDoc->RedrawAllView();
		}
	}
}

void CImageBar::OnEnChangeEditB()
{
	if(m_pItem)
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_ImageSet.FindKey(pDoc->m_dwTick);

		if( nIndex != -1 )
		{
			UpdateData();
			m_pItem->m_ImageSet.m_vKey[nIndex]->m_dwColor &= 0xFFFFFF00;
			m_pItem->m_ImageSet.m_vKey[nIndex]->m_dwColor |= (m_dwValueB & 0x000000FF);

			if(!pDoc->GetPlayCtrl()->m_bPlay)
				pDoc->RedrawAllView();
		}
	}
}

LRESULT CImageBar::OnKeyMove( WPARAM wParam, LPARAM lParam)
{
	int nFrom = INT(wParam);
	int nTo = INT(lParam);

	if(m_pItem)
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nMin = pCtrl->m_cPlayCtrl.GetRangeMin();

		nFrom += nMin;
		nTo += nMin;

		if( nTo != -1 )
		{
			VECTORCLRKEY::iterator it = m_pItem->m_ImageSet.m_vKey.begin();
			DWORD dwFrom = GetTickFromPos(nFrom);
			DWORD dwTo = GetTickFromPos(nTo);
			LPCLRKEY pKey = NULL;

			m_pItem->m_ImageSet.DeleteKey(dwTo);
			while( it != m_pItem->m_ImageSet.m_vKey.end() )
				if( (*it)->m_dwTick == dwFrom )
				{
					pKey = (*it);
					m_pItem->m_ImageSet.m_vKey.erase(it);

					break;
				}
				else
					it++;
			m_pItem->m_ImageSet.DeleteKey(dwFrom);

			if(pKey)
			{
				pKey->m_dwTick = dwTo;
				m_pItem->m_ImageSet.InsertKey(pKey);
			}
		}
		else
			m_pItem->m_ImageSet.DeleteKey(GetTickFromPos(nFrom));

		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}

	return 0;
}

LRESULT CImageBar::OnKeyCopy( WPARAM wParam, LPARAM lParam)
{
	int nFrom = INT(wParam);
	int nTo = INT(lParam);

	if(m_pItem)
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nMin = pCtrl->m_cPlayCtrl.GetRangeMin();
		int nIndex = -1;

		nFrom += nMin;
		nTo += nMin;

		DWORD dwFrom = GetTickFromPos(nFrom);
		DWORD dwTo = GetTickFromPos(nTo);

		m_pItem->m_ImageSet.DeleteKey(dwTo);
		nIndex = m_pItem->m_ImageSet.FindKey(dwFrom);

		if( nIndex != -1 )
		{
			LPCLRKEY pKey = new CLRKEY();

			pKey->m_dwColor = m_pItem->m_ImageSet.m_vKey[nIndex]->m_dwColor;
			pKey->m_dwTick = dwTo;

			m_pItem->m_ImageSet.InsertKey(pKey);
		}

		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}

	return 0;
}

void CImageBar::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinKey && nPos != 0 &&
		!m_pItem->m_ImageSet.m_vKey.empty() )
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nCount = INT(m_pItem->m_ImageSet.m_vKey.size());
		int nPrev = nCount - 1;
		int nNext = 0;

		for( int i=0; i<nCount; i++)
		{
			if( pDoc->m_dwTick < m_pItem->m_ImageSet.m_vKey[i]->m_dwTick )
			{
				nNext = i;
				break;
			}

			if( pDoc->m_dwTick == m_pItem->m_ImageSet.m_vKey[i]->m_dwTick )
			{
				nNext = i + 1 < nCount ? i + 1 : 0;
				break;
			}

			nPrev = i;
		}

		if( INT(nPos) < 0 )
			nNext = INT(m_pItem->m_ImageSet.m_vKey[nPrev]->m_dwTick);
		else
			nNext = INT(m_pItem->m_ImageSet.m_vKey[nNext]->m_dwTick);

		nNext = GetPosFromTick((DWORD) nNext);
		m_spinKey.SetPos(0);

		pCtrl->m_cPlayCtrl.SetPos(nNext);
		UpdateCtrl(nNext);
	}

	CBarContent::OnHScroll( nSBCode, nPos, pScrollBar);
}

void CImageBar::OnBnClickedButtonTimeset()
{
	CTimeSettingDlg dlg;

	dlg.m_dwTotalFrame = m_dwTotalFrame;
	dlg.m_dwTotalTick = m_dwTotalTick;

	if( dlg.DoModal() == IDOK )
	{
		CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nCount = INT(m_pItem->m_ImageSet.m_vKey.size());
		DWORD dwTotalFrame = m_dwTotalFrame;
		VECTORCLRKEY vKey;

		for( int i=0; i<nCount; i++)
		{
			m_pItem->m_ImageSet.m_vKey[i]->m_dwTick = (DWORD) GetPosFromTick(m_pItem->m_ImageSet.m_vKey[i]->m_dwTick);
			vKey.push_back(m_pItem->m_ImageSet.m_vKey[i]);
		}
		m_pItem->m_ImageSet.m_vKey.clear();

		m_dwTotalFrame = dlg.m_dwTotalFrame;
		m_dwTotalTick = dlg.m_dwTotalTick;

		for( i=0; i<nCount; i++)
		{
			vKey[i]->m_dwTick = GetTickFromPos(vKey[i]->m_dwTick * m_dwTotalFrame / dwTotalFrame);
			m_pItem->m_ImageSet.DeleteKey(vKey[i]->m_dwTick);
			m_pItem->m_ImageSet.InsertKey(vKey[i]);
		}
		vKey.clear();

		m_pItem->m_ImageSet.m_dwTotalTick = m_dwTotalTick;
		m_pItem->m_dwTotalFrame = m_dwTotalFrame;

		pCtrl->SetRange( 0, m_dwTotalFrame);
		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}
}

void CALLBACK CImageBar::OnImageListDragLeave( CGSListCtrl *pList)
{
	CImageBar *pBar = (CImageBar *) pList->GetParent();
	CImageDoc *pDoc = (CImageDoc *) pBar->m_pItem->GetMainDocument();

	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));
	pList->SetCurSel(pBar->GetIndexFromTick(pDoc->m_dwTick));

	pList->CListView::OnDragLeave();
}

DROPEFFECT CALLBACK CImageBar::OnImageListDragOver( CGSListCtrl *pList,
												    COleDataObject *pDataObject,
													DWORD dwKeyState,
													CPoint point)
{
	CImageBar *pBar = (CImageBar *) pList->GetParent();
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

BOOL CALLBACK CImageBar::OnImageListDrop( CGSListCtrl *pList,
										  COleDataObject* pDataObject,
										  DROPEFFECT dropEffect,
										  CPoint point)
{
	CImageBar *pBar = (CImageBar *) pList->GetParent();

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

				pBar->InsertImage( (CImageItem *) pItem, pList->GetListCtrl()->HitTest(point));
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnImageListDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CImageBar::CanImageListDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_IMAGE && ((CImageItem *) pItem)->m_Image.IsEnable();
}

void CImageBar::OnLvnBegindragListSprite(NMHDR *pNMHDR, LRESULT *pResult)
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

void CImageBar::OnLvnItemchangedListSprite(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( m_cImageList.m_bUISelect && (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		MoveToImagePos(pNMLV->iItem);

	*pResult = 0;
}

void CImageBar::OnNMClickListSprite(NMHDR *pNMHDR, LRESULT *pResult)
{
	CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
	int nIndex = m_cImageList.GetCurSel();

	if( nIndex == -1 )
		m_cImageList.SetCurSel(GetIndexFromTick(pDoc->m_dwTick));

	*pResult = 0;
}

void CImageBar::MoveToImagePos( int nIndex)
{
	CImageDoc *pDoc = (CImageDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	nIndex = GetPosFromTick(GetTickFromIndex(nIndex));
	pCtrl->m_cPlayCtrl.SetPos(nIndex);
	UpdateCtrl(nIndex);
}

void CImageBar::InsertImage( CImageItem *pItem, int nIndex)
{
	CListCtrl *pListCtrl = m_cImageList.GetListCtrl();

	if( nIndex == -1 )
		nIndex = INT(m_pItem->m_vImageID.size());

	m_pItem->m_vImageID.insert(
		m_pItem->m_vImageID.begin() + nIndex,
		pItem->m_dwID);

	m_pItem->m_ImageSet.m_vImage.insert(
		m_pItem->m_ImageSet.m_vImage.begin() + nIndex,
		&pItem->m_Image);

	nIndex = pListCtrl->InsertItem( nIndex, pItem->m_strName);
	pListCtrl->SetItemData( nIndex, (DWORD_PTR) pItem);

	if( !pItem->m_Image.IsEnable() && CGameStudioDoc::m_pPreview &&
		CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem == m_pItem )
		CGameStudioDoc::m_pPreview->Invalidate();

	m_cImageList.UpdateColumnWidth();
	MoveToImagePos(nIndex);
}

void CImageBar::DeleteImage( int nIndex)
{
	CListCtrl *pListCtrl = m_cImageList.GetListCtrl();

	m_pItem->m_ImageSet.m_vImage.erase(m_pItem->m_ImageSet.m_vImage.begin() + nIndex);
	m_pItem->m_vImageID.erase(m_pItem->m_vImageID.begin() + nIndex);

	pListCtrl->DeleteItem(nIndex);
	m_cImageList.UpdateColumnWidth();

	if( !m_pItem->m_Image.IsEnable() && CGameStudioDoc::m_pPreview &&
		CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem == m_pItem )
		CGameStudioDoc::m_pPreview->Invalidate();

	UpdateCtrl();
}

void CImageBar::MoveItemPos( int nFrom, int nTo)
{
	if( nFrom == nTo )
		return;

	CListCtrl *pListCtrl = m_cImageList.GetListCtrl();
	CImageItem *pItem = (CImageItem *) pListCtrl->GetItemData(nFrom);

	DeleteImage(nFrom);
	InsertImage( pItem, nTo);
}

void CImageBar::OnBnClickedButtonDeleteImage()
{
	DeleteImage(m_cImageList.GetCurSel());
}

void CImageBar::OnLvnItemActivateListSprite(NMHDR *pNMHDR, LRESULT *pResult)
{
//	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

//	DeleteImage(pNMIA->iItem);
	*pResult = 0;
}

void CImageBar::OnBnClickedButtonMovedown()
{
	int nIndex = m_cImageList.GetCurSel();

	if( nIndex == INT(m_pItem->m_vImageID.size() - 1) )
		MoveItemPos( nIndex, 0);
	else
		MoveItemPos( nIndex, nIndex + 1);
}

void CImageBar::OnBnClickedButtonMoveup()
{
	int nIndex = m_cImageList.GetCurSel();

	if( nIndex == 0 )
		MoveItemPos( nIndex, INT(m_pItem->m_vImageID.size() - 1));
	else
		MoveItemPos( nIndex, nIndex - 1);
}

void CImageBar::OnCbnSelchangeComboImgFormat()
{
	if(m_pItem)
	{
		UpdateData();

		if( m_nImageFormat != CB_ERR )
			m_pItem->m_bImageFormat = SELECT_FORMAT::AVAILABLE[m_nImageFormat].m_bFormat;
	}
}

int CImageBar::GetImageCompressionIndex( BYTE bFormat)
{
	for( int i=0; i<3; i++)
		if( SELECT_FORMAT::AVAILABLE[ i ].m_bFormat == bFormat )
			return i;

	return -1;
}