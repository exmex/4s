// AnimationBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "Game StudioView.h"
#include "AnimationBar.h"
#include "AnimationDoc.h"
#include "TimeScaleDlg.h"
#include "PreviewDlg.h"
#include ".\animationbar.h"


// CAnimationBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CAnimationBar, CBarContent)
CAnimationBar::CAnimationBar(CWnd* pParent /*=NULL*/)
: CBarContent(CAnimationBar::IDD, pParent)
, m_strTimeScale(_T(""))
, m_dwTotalFrame(0)
, m_dwTotalTime(0)
, m_dwFrame(0)
, m_strKey(_T(""))
, m_strValueX(_T(""))
, m_strValueY(_T(""))
, m_strValueZ(_T(""))
, m_strValueRX(_T(""))
, m_strValueRY(_T(""))
, m_strValueRZ(_T(""))
, m_strValueSX(_T(""))
, m_strValueSY(_T(""))
, m_strValueSZ(_T(""))
, m_bAniLoop(FALSE)
, m_strAniLoopID(_T(""))
, m_strEventID(_T(""))
, m_bReverse(FALSE)
{
	m_pItem = NULL;
}

CAnimationBar::~CAnimationBar()
{
}

void CAnimationBar::DoDataExchange( CDataExchange* pDX)
{
	CBarContent::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_STATIC_TOTAL_FRAME, m_dwTotalFrame);
	DDX_Text(pDX, IDC_STATIC_TIMESCALE, m_strTimeScale);
	DDX_Text(pDX, IDC_STATIC_FRAME, m_dwFrame);
	DDX_Control(pDX, IDC_BUTTON_DELETE_ANIMATION, m_cDeleteAnimation);
	DDX_Control(pDX, IDC_BUTTON_MOVEDOWN, m_cDownAnimation);
	DDX_Control(pDX, IDC_BUTTON_MOVEUP, m_cUpAnimation);
	DDX_Control(pDX, IDC_BUTTON_DELETEKEY, m_cDeleteKey);
	DDX_Control(pDX, IDC_BUTTON_ADDKEY, m_cAddKey);
	DDX_Control(pDX, IDC_BUTTON_TIMESET, m_cTimeSet);
	DDX_Control(pDX, IDC_LIST_ANIMATION, m_cAnimationList);
	DDX_Control(pDX, IDC_SPIN_X, m_spinValueX);
	DDX_Control(pDX, IDC_SPIN_Y, m_spinValueY);
	DDX_Control(pDX, IDC_SPIN_Z, m_spinValueZ);
	DDX_Control(pDX, IDC_SPIN_RX, m_spinValueRX);
	DDX_Control(pDX, IDC_SPIN_RY, m_spinValueRY);
	DDX_Control(pDX, IDC_SPIN_RZ, m_spinValueRZ);
	DDX_Control(pDX, IDC_SPIN_SX, m_spinValueSX);
	DDX_Control(pDX, IDC_SPIN_SY, m_spinValueSY);
	DDX_Control(pDX, IDC_SPIN_SZ, m_spinValueSZ);
	DDX_Control(pDX, IDC_SPIN_KEY, m_spinKey);
	DDX_Control(pDX, IDC_EDIT_X, m_cValueX);
	DDX_Control(pDX, IDC_EDIT_Y, m_cValueY);
	DDX_Control(pDX, IDC_EDIT_Z, m_cValueZ);
	DDX_Control(pDX, IDC_EDIT_RX, m_cValueRX);
	DDX_Control(pDX, IDC_EDIT_RY, m_cValueRY);
	DDX_Control(pDX, IDC_EDIT_RZ, m_cValueRZ);
	DDX_Control(pDX, IDC_EDIT_SX, m_cValueSX);
	DDX_Control(pDX, IDC_EDIT_SY, m_cValueSY);
	DDX_Control(pDX, IDC_EDIT_SZ, m_cValueSZ);
	DDX_Text(pDX, IDC_EDIT_ANILOOPID, m_strAniLoopID);
	DDX_Text(pDX, IDC_EDIT_EVENT, m_strEventID);
	DDX_Text(pDX, IDC_STATIC_KEY, m_strKey);
	DDX_Text(pDX, IDC_EDIT_X, m_strValueX);
	DDX_Text(pDX, IDC_EDIT_Y, m_strValueY);
	DDX_Text(pDX, IDC_EDIT_Z, m_strValueZ);
	DDX_Text(pDX, IDC_EDIT_RX, m_strValueRX);
	DDX_Text(pDX, IDC_EDIT_RY, m_strValueRY);
	DDX_Text(pDX, IDC_EDIT_RZ, m_strValueRZ);
	DDX_Text(pDX, IDC_EDIT_SX, m_strValueSX);
	DDX_Text(pDX, IDC_EDIT_SY, m_strValueSY);
	DDX_Text(pDX, IDC_EDIT_SZ, m_strValueSZ);
	DDX_Control(pDX, IDC_CHECK_REVERSE, m_cReverse);
	DDX_Control(pDX, IDC_CHECK_ANILOOP, m_cAniLoop);
	DDX_Control(pDX, IDC_EDIT_ANILOOPID, m_cAniLoopID);
	DDX_Control(pDX, IDC_EDIT_EVENT, m_cEventID);
	DDX_Check(pDX, IDC_CHECK_REVERSE, m_bReverse);
	DDX_Check(pDX, IDC_CHECK_ANILOOP, m_bAniLoop);
	DDX_Text(pDX, IDC_STATIC_TOTAL_TIME, m_dwTotalTime);
}


BEGIN_MESSAGE_MAP(CAnimationBar, CBarContent)
	ON_MESSAGE( WM_PLAYCTRL_PROC, OnPlayCtrlProc)
	ON_MESSAGE( WM_PLAYCTRL_PLAY, OnPlayCtrlPlay)
	ON_MESSAGE( WM_PLAYCTRL_STOP, OnPlayCtrlStop)
	ON_MESSAGE( WM_PLAYCTRL_POS, OnPlayCtrlPos)
	ON_MESSAGE( WM_MESSAGEBOX, OnMessageBox)
	ON_MESSAGE( WM_MARKER_MOVE, OnKeyMove)
	ON_MESSAGE( WM_MARKER_COPY, OnKeyCopy)
	ON_BN_CLICKED(IDC_BUTTON_ADDKEY, OnBnClickedButtonAddkey)
	ON_BN_CLICKED(IDC_BUTTON_DELETEKEY, OnBnClickedButtonDeletekey)
	ON_EN_CHANGE(IDC_EDIT_RX, OnEnChangeEditRx)
	ON_EN_CHANGE(IDC_EDIT_RY, OnEnChangeEditRy)
	ON_EN_CHANGE(IDC_EDIT_RZ, OnEnChangeEditRz)
	ON_EN_CHANGE(IDC_EDIT_SX, OnEnChangeEditSx)
	ON_EN_CHANGE(IDC_EDIT_SY, OnEnChangeEditSy)
	ON_EN_CHANGE(IDC_EDIT_SZ, OnEnChangeEditSz)
	ON_EN_CHANGE(IDC_EDIT_X, OnEnChangeEditX)
	ON_EN_CHANGE(IDC_EDIT_Y, OnEnChangeEditY)
	ON_EN_CHANGE(IDC_EDIT_Z, OnEnChangeEditZ)
	ON_EN_CHANGE(IDC_EDIT_EVENT, OnEnChangeEditEvent)
	ON_EN_SETFOCUS(IDC_EDIT_EVENT, OnEnSetfocusEditEvent)
	ON_EN_KILLFOCUS(IDC_EDIT_EVENT, OnEnKillfocusEditEvent)
	ON_WM_VSCROLL()
	ON_WM_HSCROLL()
	ON_BN_CLICKED(IDC_BUTTON_TIMESET, OnBnClickedButtonTimeset)
	ON_NOTIFY(LVN_BEGINDRAG, IDC_LIST_ANIMATION, OnLvnBegindragListAnimation)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ANIMATION, OnLvnItemchangedListAnimation)
	ON_NOTIFY(NM_CLICK, IDC_LIST_ANIMATION, OnNMClickListAnimation)
	ON_BN_CLICKED(IDC_BUTTON_DELETE_ANIMATION, OnBnClickedButtonDeleteAnimation)
	ON_NOTIFY(LVN_ITEMACTIVATE, IDC_LIST_ANIMATION, OnLvnItemActivateListAnimation)
	ON_BN_CLICKED(IDC_BUTTON_MOVEDOWN, OnBnClickedButtonMovedown)
	ON_BN_CLICKED(IDC_BUTTON_MOVEUP, OnBnClickedButtonMoveup)
	ON_BN_CLICKED(IDC_CHECK_ANILOOP, OnBnClickedCheckAniloop)
	ON_BN_CLICKED(IDC_CHECK_REVERSE, OnBnClickedCheckReverse)
	ON_EN_CHANGE(IDC_EDIT_ANILOOPID, OnEnChangeEditAniloopid)
	ON_EN_SETFOCUS(IDC_EDIT_ANILOOPID, OnEnSetfocusEditAniloopid)
	ON_EN_KILLFOCUS(IDC_EDIT_ANILOOPID, OnEnKillfocusEditAniloopid)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_ANIMATION, OnNMDblclkListAnimation)
	ON_BN_CLICKED(IDC_BUTTON_CLEARKEY, OnBnClickedButtonClearkey)
END_MESSAGE_MAP()


// CAnimationBar 메시지 처리기입니다.

BOOL CAnimationBar::OnInitDialog()
{
	CBarContent::OnInitDialog();
	CRect rect;

	CListCtrl *pListCtrl = m_cAnimationList.GetListCtrl();
	pListCtrl->GetClientRect(&rect);
	rect.DeflateRect( 1, 0);

	pListCtrl->SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES|LVS_EX_INFOTIP);
	pListCtrl->InsertColumn( 0, NULL, LVCFMT_LEFT, rect.Width());

	m_cAnimationList.m_DropTarget.Register(&m_cAnimationList);
	m_cAnimationList.m_pDragEnter = OnAnimationListDragOver;
	m_cAnimationList.m_pDragOver = OnAnimationListDragOver;
	m_cAnimationList.m_pDrop = OnAnimationListDrop;
	m_cAnimationList.m_pLeave = OnAnimationListDragLeave;

	m_spinValueRX.SetRange( -100, 100);
	m_spinValueRY.SetRange( -100, 100);
	m_spinValueRZ.SetRange( -100, 100);
	m_spinValueSX.SetRange( -100, 100);
	m_spinValueSY.SetRange( -100, 100);
	m_spinValueSZ.SetRange( -100, 100);
	m_spinValueX.SetRange( -100, 100);
	m_spinValueY.SetRange( -100, 100);
	m_spinValueZ.SetRange( -100, 100);
	m_spinKey.SetRange( -100, 100);

	return TRUE;
}

void CAnimationBar::EnableAllCtrl( BOOL bEnable)
{
	m_cAnimationList.EnableWindow(bEnable);

	m_spinValueRX.EnableWindow(bEnable);
	m_spinValueRY.EnableWindow(bEnable);
	m_spinValueRZ.EnableWindow(bEnable);
	m_spinValueSX.EnableWindow(bEnable);
	m_spinValueSY.EnableWindow(bEnable);
	m_spinValueSZ.EnableWindow(bEnable);
	m_spinValueX.EnableWindow(bEnable);
	m_spinValueY.EnableWindow(bEnable);
	m_spinValueZ.EnableWindow(bEnable);
	m_spinKey.EnableWindow(bEnable);

	m_cAniLoopID.EnableWindow(bEnable && m_bAniLoop);
	m_cEventID.EnableWindow(bEnable);
	m_cValueRX.EnableWindow(bEnable);
	m_cValueRY.EnableWindow(bEnable);
	m_cValueRZ.EnableWindow(bEnable);
	m_cValueSX.EnableWindow(bEnable);
	m_cValueSY.EnableWindow(bEnable);
	m_cValueSZ.EnableWindow(bEnable);
	m_cValueX.EnableWindow(bEnable);
	m_cValueY.EnableWindow(bEnable);
	m_cValueZ.EnableWindow(bEnable);

	m_cDeleteAnimation.EnableWindow(bEnable);
	m_cDownAnimation.EnableWindow(bEnable);
	m_cUpAnimation.EnableWindow(bEnable);
	m_cDeleteKey.EnableWindow(bEnable);
	m_cAddKey.EnableWindow(bEnable);
	m_cAniLoop.EnableWindow(bEnable);
	m_cTimeSet.EnableWindow(bEnable);
	m_cReverse.EnableWindow(bEnable);
}

LRESULT CAnimationBar::OnPlayCtrlProc( WPARAM wParam, LPARAM lParam)
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	FLOAT fTotal = m_pItem->m_Aniset.GetTotalTime();

	if( fTotal == 0.0f )
	{
		pDoc->m_fTime = 0.0f;
		pCtrl->OnBnClickedButtonStop();

		return 0;
	}

	pDoc->m_fTime = CTMath::GetTimeValue((DWORD) wParam);
	if( lParam || pDoc->m_fTime < fTotal )
	{
		if( pDoc->m_fTime >= fTotal )
		{
			FLOAT fTime = pDoc->m_fTime - fTotal;

			pDoc->m_fTime = fTotal;
			pDoc->CalcFrameMatrix(TRUE);

			pCtrl->m_dwBeginTick = pCtrl->m_dwTickCount - CTMath::GetTimeTick(fTime);
			pDoc->m_fTime = fTime;
		}

		pCtrl->SetPos(GetPosFromTime(pDoc->m_fTime));
		UpdateCtrl();
	}
	else
	{
		pDoc->m_fTime = fTotal;
		pCtrl->SetPos(m_pItem->m_dwTotalFrame);
		pCtrl->OnBnClickedButtonStop();
	}
	pDoc->RedrawAllView();

	return 0;
}

LRESULT CAnimationBar::OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam)
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	FLOAT fTotal = m_pItem->m_Aniset.GetTotalTime();

	if( pDoc->m_fTime < fTotal )
		pCtrl->m_dwBeginTick -= CTMath::GetTimeTick(pDoc->m_fTime);
	else
		pDoc->CalcFrameMatrix(TRUE);
	EnableAllCtrl(FALSE);

	return 0;
}

LRESULT CAnimationBar::OnPlayCtrlStop( WPARAM wParam, LPARAM lParam)
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();

	EnableAllCtrl(TRUE);
	UpdateCtrl(GetPosFromTime(pDoc->m_fTime));

	return 0;
}

LRESULT CAnimationBar::OnPlayCtrlPos( WPARAM wParam, LPARAM lParam)
{
	UpdateCtrl(INT(lParam));
	return 0;
}

void CAnimationBar::SetItem( CAnimationItem *pItem)
{
	m_strTimeScale.Format( "%.2f", pItem->m_Aniset.m_fTimeScale);
	m_strAniLoopID = pItem->m_strLoopID;

	FLOAT fTime = pItem->m_Aniset.GetTotalTime() + 0.5f;
	m_dwTotalTime = CTMath::GetTimeTick( fTime );
	m_dwTotalFrame = pItem->m_dwTotalFrame;
	m_bAniLoop = pItem->m_Aniset.m_bLoop;
	m_cAniLoopID.EnableWindow(m_bAniLoop);
	m_pItem = pItem;

	if(m_strAniLoopID.IsEmpty())
		m_strAniLoopID.LoadString(IDS_NOTUSE);

	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	CListCtrl *pList = m_cAnimationList.GetListCtrl();
	int nCount = INT(m_pItem->m_Aniset.m_vKey.size());

	pCtrl->SetRange( 0, m_dwTotalFrame);
	pCtrl->m_cMarker.DelAll();
	pList->DeleteAllItems();

	for( int i=0; i<nCount; i++)
		pCtrl->m_cMarker.AddMark(GetPosFromTime(m_pItem->m_Aniset.m_vKey[i]->m_fTime));

	nCount = INT(m_pItem->m_vAniID.size());
	for( i=0; i<nCount; i++)
	{
		CAnimationItem *pAnimationItem = CItemBase::GetAnimationItem(m_pItem->m_vAniID[i]);
		int nIndex = pList->InsertItem( i, pAnimationItem->m_strName);

		pList->SetItemData( nIndex, (DWORD_PTR) pAnimationItem);
	}
	m_cAnimationList.UpdateColumnWidth();

	UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
}

FLOAT CAnimationBar::GetTimeFromIndex( int nIndex)
{
	FLOAT fTime = 0.0f;

	for( int i=0; i<nIndex; i++)
		fTime += m_pItem->m_Aniset.m_fTimeScale * ((FLOAT) m_pItem->m_Aniset.m_vAni[i]->m_pAni->GetInterval());

	return fTime;
}

FLOAT CAnimationBar::GetTimeFromPos( int nPos)
{
	return m_dwTotalFrame ? ((FLOAT) nPos) * m_pItem->m_Aniset.GetTotalTime() / ((FLOAT) m_dwTotalFrame) : 0.0f;
}

int CAnimationBar::GetIndexFromTime( FLOAT fTime)
{
	int nCount = INT(m_pItem->m_Aniset.m_vAni.size());
	FLOAT fTotal = 0.0f;

	for( int i=0; i<nCount; i++)
	{
		fTotal += m_pItem->m_Aniset.m_fTimeScale * ((FLOAT) m_pItem->m_Aniset.m_vAni[i]->m_pAni->GetInterval());
		if( fTime < fTotal )
			return i;
	}

	if( fTime == fTotal )
		return nCount - 1;

	return -1;
}

int CAnimationBar::GetPosFromTime( FLOAT fTime)
{
	FLOAT fPos = fTime * ((FLOAT) m_dwTotalFrame) / m_pItem->m_Aniset.GetTotalTime();
	return fPos == ((FLOAT) INT(fPos)) ? INT(fPos) : INT(fPos) + 1;
}

void CAnimationBar::UpdateCtrl( int nPos)
{
	if(m_pItem)
	{
		CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();

		pDoc->m_fTime = GetTimeFromPos(nPos);
		UpdateCtrl();
	}
}

void CAnimationBar::UpdateCtrl()
{
	if(m_pItem)
	{
		CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);
		int nSel = GetIndexFromTime(pDoc->m_fTime);
		m_cAnimationList.SetCurSel(nSel);

		m_pItem->m_Aniset.m_fCurTime = pDoc->m_fTime;
		ANIKEY vKey = m_pItem->m_Aniset.GetAniKey();
		BOOL bEnable = nIndex != -1;

		if(!pCtrl->m_bPlay)
		{
			m_spinValueRX.EnableWindow(bEnable);
			m_spinValueRY.EnableWindow(bEnable);
			m_spinValueRZ.EnableWindow(bEnable);
			m_spinValueSX.EnableWindow(bEnable);
			m_spinValueSY.EnableWindow(bEnable);
			m_spinValueSZ.EnableWindow(bEnable);
			m_spinValueX.EnableWindow(bEnable);
			m_spinValueY.EnableWindow(bEnable);
			m_spinValueZ.EnableWindow(bEnable);
			m_cValueRX.EnableWindow(bEnable);
			m_cValueRY.EnableWindow(bEnable);
			m_cValueRZ.EnableWindow(bEnable);
			m_cValueSX.EnableWindow(bEnable);
			m_cValueSY.EnableWindow(bEnable);
			m_cValueSZ.EnableWindow(bEnable);
			m_cValueX.EnableWindow(bEnable);
			m_cValueY.EnableWindow(bEnable);
			m_cValueZ.EnableWindow(bEnable);
			m_cEventID.EnableWindow(bEnable);

			m_cDeleteKey.EnableWindow(bEnable);
			m_cAddKey.EnableWindow(!bEnable);
		}

		m_strValueRX.Format( "%.2f", vKey.m_fRotX * 180.0f / D3DX_PI);
		m_strValueRY.Format( "%.2f", vKey.m_fRotY * 180.0f / D3DX_PI);
		m_strValueRZ.Format( "%.2f", vKey.m_fRotZ * 180.0f / D3DX_PI);
		m_strValueSX.Format( "%.2f", vKey.m_fScaleX);
		m_strValueSY.Format( "%.2f", vKey.m_fScaleY);
		m_strValueSZ.Format( "%.2f", vKey.m_fScaleZ);
		m_strValueX.Format( "%.2f", vKey.m_fPosX);
		m_strValueY.Format( "%.2f", vKey.m_fPosY);
		m_strValueZ.Format( "%.2f", vKey.m_fPosZ);

		if(bEnable)
			m_strEventID = m_pItem->m_vEventID[nIndex];

		if( nSel >= 0 )
		{
			m_bReverse = m_pItem->m_vReverse[nSel];
			m_cReverse.EnableWindow(TRUE);
		}
		else
			m_cReverse.EnableWindow(FALSE);

		if(m_strEventID.IsEmpty())
			m_strEventID.LoadString(IDS_NOTUSE);

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

void CAnimationBar::OnBnClickedButtonAddkey()
{
	if( m_pItem && m_dwTotalFrame )
	{
		CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

		if( nIndex == -1 )
		{
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
			LPANIKEY pKey = new ANIKEY();

			m_pItem->m_Aniset.m_fCurTime = pDoc->m_fTime;
			*pKey = m_pItem->m_Aniset.GetAniKey();
			m_pItem->m_Aniset.InsertKey(pKey);

			nIndex = m_pItem->m_Aniset.FindKey(pKey->m_fTime);
			m_pItem->m_vEventID.insert(
				m_pItem->m_vEventID.begin() + nIndex,
				CString(_T("")));

			int nPos = pCtrl->m_cPlayCtrl.GetPos();
			pCtrl->m_cMarker.AddMark(nPos - pCtrl->m_cPlayCtrl.GetRangeMin());
			UpdateCtrl(nPos);
		}
	}
}

void CAnimationBar::OnBnClickedButtonDeletekey()
{
	if( m_pItem && m_dwTotalFrame )
	{
		CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
		int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

		if( nIndex != -1 )
		{
			CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
			int nPos = pCtrl->m_cPlayCtrl.GetPos();

			pCtrl->m_cMarker.DelMark(nPos - pCtrl->m_cPlayCtrl.GetRangeMin());
			CItemBase::ReleaseLocalID( m_pItem->m_vEventID[nIndex], m_pItem);
			m_pItem->m_Aniset.DeleteKey(pDoc->m_fTime);
			m_pItem->m_vEventID.erase(m_pItem->m_vEventID.begin() + nIndex);
			UpdateCtrl(nPos);
		}
	}
}

void CAnimationBar::OnEnChangeEditRx()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->m_Aniset.m_vKey[nIndex]->m_fRotX = (FLOAT) atof(m_strValueRX) * D3DX_PI / 180.0f;

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CAnimationBar::OnEnChangeEditRy()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->m_Aniset.m_vKey[nIndex]->m_fRotY = (FLOAT) atof(m_strValueRY) * D3DX_PI / 180.0f;

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CAnimationBar::OnEnChangeEditRz()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->m_Aniset.m_vKey[nIndex]->m_fRotZ = (FLOAT) atof(m_strValueRZ) * D3DX_PI / 180.0f;

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CAnimationBar::OnEnChangeEditSx()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->m_Aniset.m_vKey[nIndex]->m_fScaleX = (FLOAT) atof(m_strValueSX);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CAnimationBar::OnEnChangeEditSy()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->m_Aniset.m_vKey[nIndex]->m_fScaleY = (FLOAT) atof(m_strValueSY);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CAnimationBar::OnEnChangeEditSz()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->m_Aniset.m_vKey[nIndex]->m_fScaleZ = (FLOAT) atof(m_strValueSZ);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CAnimationBar::OnEnChangeEditX()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->m_Aniset.m_vKey[nIndex]->m_fPosX = (FLOAT) atof(m_strValueX);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CAnimationBar::OnEnChangeEditY()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->m_Aniset.m_vKey[nIndex]->m_fPosY = (FLOAT) atof(m_strValueY);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CAnimationBar::OnEnChangeEditZ()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		m_pItem->m_Aniset.m_vKey[nIndex]->m_fPosZ = (FLOAT) atof(m_strValueZ);

		if(!pDoc->GetPlayCtrl()->m_bPlay)
			pDoc->RedrawAllView();
	}
}

void CAnimationBar::OnEnChangeEditEvent()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		UpdateData();
		if(!CItemBase::CheckID(m_strEventID))
		{
			CString strMessage;
			CString strTitle;

			strMessage.LoadString(IDS_ERR_INVALID_ID);
			strTitle.LoadString(IDR_MAINFRAME);

			m_strEventID = m_pItem->m_vEventID[nIndex];
			UpdateData(FALSE);

			MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
			m_cEventID.SetFocus();
		}
	}
}

void CAnimationBar::OnEnSetfocusEditEvent()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		m_strEventID = m_pItem->m_vEventID[nIndex];
		UpdateData(FALSE);
	}
}

void CAnimationBar::OnEnKillfocusEditEvent()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_pItem->m_Aniset.FindKey(pDoc->m_fTime);

	if( nIndex != -1 )
	{
		if( m_pItem->m_vEventID[nIndex] == m_strEventID )
		{
			if(m_strEventID.IsEmpty())
			{
				m_strEventID.LoadString(IDS_NOTUSE);
				UpdateData(FALSE);
			}

			return;
		}

		if(!m_pItem->m_vEventID[nIndex].IsEmpty())
		{
			CItemBase::ReleaseLocalID( m_pItem->m_vEventID[nIndex], m_pItem);
			m_pItem->m_Aniset.m_vKey[nIndex]->m_dwEventID = 0;
		}

		if(!m_strEventID.IsEmpty())
			if(!m_pItem->CheckDupLocalID( m_pItem->m_vEventID[nIndex], m_strEventID))
			{
				m_strEventID = m_pItem->m_vEventID[nIndex];
				if(m_strEventID.IsEmpty())
					m_strEventID.LoadString(IDS_NOTUSE);
				UpdateData(FALSE);

				PostMessage(WM_MESSAGEBOX);
			}
			else
			{
				m_pItem->m_Aniset.m_vKey[nIndex]->m_dwEventID = CItemBase::RegLocalID( m_strEventID, m_pItem);
				m_pItem->m_vEventID[nIndex] = m_strEventID;
			}
		else
		{
			m_pItem->m_vEventID[nIndex] = m_strEventID;

			m_strEventID.LoadString(IDS_NOTUSE);
			UpdateData(FALSE);
		}
	}
}

LRESULT CAnimationBar::OnKeyMove( WPARAM wParam, LPARAM lParam)
{
	int nFrom = INT(wParam);
	int nTo = INT(lParam);

	if(m_pItem)
	{
		CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nMin = pCtrl->m_cPlayCtrl.GetRangeMin();

		nFrom += nMin;
		nTo += nMin;

		if( nTo != -1 )
		{
			VECTORANIKEY::iterator it = m_pItem->m_Aniset.m_vKey.begin();
			FLOAT fFrom = GetTimeFromPos(nFrom);
			FLOAT fTo = GetTimeFromPos(nTo);

			CString strEventID(_T(""));
			LPANIKEY pKey = NULL;

			int nIndex = m_pItem->m_Aniset.FindKey(fTo);
			m_pItem->m_Aniset.DeleteKey(fTo);
			if( nIndex != -1 )
				m_pItem->m_vEventID.erase(m_pItem->m_vEventID.begin() + nIndex);

			nIndex = m_pItem->m_Aniset.FindKey(fFrom);
			if( nIndex != -1 )
			{
				strEventID = m_pItem->m_vEventID[nIndex];
				m_pItem->m_vEventID.erase(m_pItem->m_vEventID.begin() + nIndex);
			}

			while( it != m_pItem->m_Aniset.m_vKey.end() )
				if( (*it)->m_fTime == fFrom )
				{
					pKey = (*it);
					m_pItem->m_Aniset.m_vKey.erase(it);

					break;
				}
				else
					it++;

			if(pKey)
			{
				pKey->m_fTime = fTo;
				m_pItem->m_Aniset.InsertKey(pKey);

				nIndex = m_pItem->m_Aniset.FindKey(fTo);
				m_pItem->m_vEventID.insert(
					m_pItem->m_vEventID.begin() + nIndex,
					strEventID);
			}
		}
		else
		{
			FLOAT fFrom = GetTimeFromPos(nFrom);
			int nIndex = m_pItem->m_Aniset.FindKey(fFrom);

			if( nIndex != -1 )
			{
				CItemBase::ReleaseLocalID( m_pItem->m_vEventID[nIndex], m_pItem);
				m_pItem->m_vEventID.erase(m_pItem->m_vEventID.begin() + nIndex);
			}

			m_pItem->m_Aniset.DeleteKey(fFrom);
		}

		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}

	return 0;
}

LRESULT CAnimationBar::OnKeyCopy( WPARAM wParam, LPARAM lParam)
{
	int nFrom = INT(wParam);
	int nTo = INT(lParam);

	if(m_pItem)
	{
		CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nMin = pCtrl->m_cPlayCtrl.GetRangeMin();
		int nIndex = -1;

		nFrom += nMin;
		nTo += nMin;

		FLOAT fFrom = GetTimeFromPos(nFrom);
		FLOAT fTo = GetTimeFromPos(nTo);

		nIndex = m_pItem->m_Aniset.FindKey(fTo);
		m_pItem->m_Aniset.DeleteKey(fTo);
		if( nIndex != -1 )
			m_pItem->m_vEventID.erase(m_pItem->m_vEventID.begin() + nIndex);

		nIndex = m_pItem->m_Aniset.FindKey(fFrom);
		if( nIndex != -1 )
		{
			LPANIKEY pKey = new ANIKEY();

			pKey->m_dwEventID = 0;
			pKey->m_fScaleX = m_pItem->m_Aniset.m_vKey[nIndex]->m_fScaleX;
			pKey->m_fScaleY = m_pItem->m_Aniset.m_vKey[nIndex]->m_fScaleY;
			pKey->m_fScaleZ = m_pItem->m_Aniset.m_vKey[nIndex]->m_fScaleZ;
			pKey->m_fRotX = m_pItem->m_Aniset.m_vKey[nIndex]->m_fRotX;
			pKey->m_fRotY = m_pItem->m_Aniset.m_vKey[nIndex]->m_fRotY;
			pKey->m_fRotZ = m_pItem->m_Aniset.m_vKey[nIndex]->m_fRotZ;
			pKey->m_fPosX = m_pItem->m_Aniset.m_vKey[nIndex]->m_fPosX;
			pKey->m_fPosY = m_pItem->m_Aniset.m_vKey[nIndex]->m_fPosY;
			pKey->m_fPosZ = m_pItem->m_Aniset.m_vKey[nIndex]->m_fPosZ;
			pKey->m_fTime = fTo;

			m_pItem->m_Aniset.InsertKey(pKey);
			m_pItem->m_vEventID.insert(
				m_pItem->m_vEventID.begin() + m_pItem->m_Aniset.FindKey(fTo),
				CString(_T("")));
		}

		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}

	return 0;
}

void CAnimationBar::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinValueX )
	{
		FLOAT fValueX = (FLOAT) atof(m_strValueX);

		fValueX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueX.SetPos(0);

		m_strValueX.Format( "%.2f", fValueX);
		UpdateData(FALSE);
		OnEnChangeEditX();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueY )
	{
		FLOAT fValueY = (FLOAT) atof(m_strValueY);

		fValueY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueY.SetPos(0);

		m_strValueY.Format( "%.2f", fValueY);
		UpdateData(FALSE);
		OnEnChangeEditY();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueZ )
	{
		FLOAT fValueZ = (FLOAT) atof(m_strValueZ);

		fValueZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueZ.SetPos(0);

		m_strValueZ.Format( "%.2f", fValueZ);
		UpdateData(FALSE);
		OnEnChangeEditZ();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueSX )
	{
		FLOAT fValueSX = (FLOAT) atof(m_strValueSX);

		fValueSX += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueSX.SetPos(0);

		m_strValueSX.Format( "%.2f", fValueSX);
		UpdateData(FALSE);
		OnEnChangeEditSx();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueSY )
	{
		FLOAT fValueSY = (FLOAT) atof(m_strValueSY);

		fValueSY += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueSY.SetPos(0);

		m_strValueSY.Format( "%.2f", fValueSY);
		UpdateData(FALSE);
		OnEnChangeEditSy();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueSZ )
	{
		FLOAT fValueSZ = (FLOAT) atof(m_strValueSZ);

		fValueSZ += ((FLOAT) INT(nPos)) / 100.0f;
		m_spinValueSZ.SetPos(0);

		m_strValueSZ.Format( "%.2f", fValueSZ);
		UpdateData(FALSE);
		OnEnChangeEditSz();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueRX )
	{
		FLOAT fValueRX = (FLOAT) atof(m_strValueRX);

		fValueRX += (FLOAT) INT(nPos);
		m_spinValueRX.SetPos(0);

		m_strValueRX.Format( "%.2f", fValueRX);
		UpdateData(FALSE);
		OnEnChangeEditRx();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueRY )
	{
		FLOAT fValueRY = (FLOAT) atof(m_strValueRY);

		fValueRY += (FLOAT) INT(nPos);
		m_spinValueRY.SetPos(0);

		m_strValueRY.Format( "%.2f", fValueRY);
		UpdateData(FALSE);
		OnEnChangeEditRy();
	}

	if( pScrollBar == (CScrollBar *) &m_spinValueRZ )
	{
		FLOAT fValueRZ = (FLOAT) atof(m_strValueRZ);

		fValueRZ += (FLOAT) INT(nPos);
		m_spinValueRZ.SetPos(0);

		m_strValueRZ.Format( "%.2f", fValueRZ);
		UpdateData(FALSE);
		OnEnChangeEditRz();
	}

	CBarContent::OnVScroll( nSBCode, nPos, pScrollBar);
}

void CAnimationBar::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_spinKey && nPos != 0 &&
		!m_pItem->m_Aniset.m_vKey.empty() )
	{
		CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nCount = INT(m_pItem->m_Aniset.m_vKey.size());
		int nPrev = nCount - 1;
		int nNext = 0;

		m_spinKey.SetFocus();
		for( int i=0; i<nCount; i++)
		{
			if( pDoc->m_fTime < m_pItem->m_Aniset.m_vKey[i]->m_fTime )
			{
				nNext = i;
				break;
			}

			if( pDoc->m_fTime == m_pItem->m_Aniset.m_vKey[i]->m_fTime )
			{
				nNext = i + 1 < nCount ? i + 1 : 0;
				break;
			}

			nPrev = i;
		}

		FLOAT fTime = 0.0f;
		if( INT(nPos) < 0 )
			fTime = m_pItem->m_Aniset.m_vKey[nPrev]->m_fTime;
		else
			fTime = m_pItem->m_Aniset.m_vKey[nNext]->m_fTime;

		nNext = GetPosFromTime(fTime);
		m_spinKey.SetPos(0);

		pCtrl->m_cPlayCtrl.SetPos(nNext);
		UpdateCtrl(nNext);
	}

	CBarContent::OnHScroll( nSBCode, nPos, pScrollBar);
}

void CAnimationBar::OnBnClickedButtonTimeset()
{
	CTimeScaleDlg dlg;

	dlg.m_strTimeScale = m_strTimeScale;
	dlg.m_dwTotalFrame = m_dwTotalFrame;

	if( dlg.DoModal() == IDOK )
	{
		CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		int nCount = INT(m_pItem->m_Aniset.m_vKey.size());
		DWORD dwTotalFrame = m_dwTotalFrame;
		VECTORSTRING vEventID;
		VECTORANIKEY vKey;

		for( int i=0; i<nCount; i++)
		{
			m_pItem->m_Aniset.m_vKey[i]->m_fTime = (FLOAT) GetPosFromTime(m_pItem->m_Aniset.m_vKey[i]->m_fTime);
			vEventID.push_back(m_pItem->m_vEventID[i]);
			vKey.push_back(m_pItem->m_Aniset.m_vKey[i]);
		}
		m_pItem->m_Aniset.m_vKey.clear();
		m_pItem->m_vEventID.clear();

		m_pItem->m_Aniset.m_fTimeScale = (FLOAT) atof(dlg.m_strTimeScale);
		m_pItem->m_dwTotalFrame = dlg.m_dwTotalFrame;
		m_strTimeScale = dlg.m_strTimeScale;
		m_dwTotalFrame = dlg.m_dwTotalFrame;
		FLOAT fTime = m_pItem->m_Aniset.GetTotalTime() + 0.5f;
		m_dwTotalTime = CTMath::GetTimeTick( fTime );

		for( i=0; i<nCount; i++)
		{
			vKey[i]->m_fTime = GetTimeFromPos(INT(vKey[i]->m_fTime * m_dwTotalFrame / dwTotalFrame));

			int nIndex = m_pItem->m_Aniset.FindKey(vKey[i]->m_fTime);
			if( nIndex != -1 )
				m_pItem->m_vEventID.erase(m_pItem->m_vEventID.begin() + nIndex);
			m_pItem->m_Aniset.DeleteKey(vKey[i]->m_fTime);

			m_pItem->m_Aniset.InsertKey(vKey[i]);
			m_pItem->m_vEventID.insert(
				m_pItem->m_vEventID.begin() + m_pItem->m_Aniset.FindKey(vKey[i]->m_fTime),
				vEventID[i]);
		}
		vEventID.clear();
		vKey.clear();

		pCtrl->SetRange( 0, m_dwTotalFrame);
		UpdateCtrl(pCtrl->m_cPlayCtrl.GetPos());
	}
}

void CALLBACK CAnimationBar::OnAnimationListDragLeave( CGSListCtrl *pList)
{
	CAnimationBar *pBar = (CAnimationBar *) pList->GetParent();
	CAnimationDoc *pDoc = (CAnimationDoc *) pBar->m_pItem->GetMainDocument();

	CGameStudioView::m_pDragImage->DragMove( CPoint(
		-10000,
		-10000));
	pList->SetCurSel(pBar->GetIndexFromTime(pDoc->m_fTime));

	pList->CListView::OnDragLeave();
}

DROPEFFECT CALLBACK CAnimationBar::OnAnimationListDragOver( CGSListCtrl *pList,
														    COleDataObject *pDataObject,
															DWORD dwKeyState,
															CPoint point)
{
	CAnimationBar *pBar = (CAnimationBar *) pList->GetParent();
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

			if(CanAnimationListDrop(pItem))
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

BOOL CALLBACK CAnimationBar::OnAnimationListDrop( CGSListCtrl *pList,
												  COleDataObject* pDataObject,
												  DROPEFFECT dropEffect,
												  CPoint point)
{
	CAnimationBar *pBar = (CAnimationBar *) pList->GetParent();

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

			if(CanAnimationListDrop(pItem))
			{
				CGameStudioView::m_pDragImage->DragMove( CPoint(
					-10000,
					-10000));

				pBar->InsertAnimation(
					(CAnimationItem *) pItem,
					FALSE,
					pList->GetListCtrl()->HitTest(point));
			}
		}

		break;
	}

	::GlobalUnlock(hGlobal);
	::GlobalFree(hGlobal);

	OnAnimationListDragLeave(pList);
	return pList->CListView::OnDrop( pDataObject, dropEffect, point);
}

BOOL CAnimationBar::CanAnimationListDrop( CItemBase *pItem)
{
	return pItem->m_bType == ITEM_ANIMATION && ((CAnimationItem *) pItem)->GetAnimation().IsEnable();
}

void CAnimationBar::OnLvnBegindragListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	HGLOBAL hGlobal = ::GlobalAlloc( GMEM_SHARE|GMEM_MOVEABLE, sizeof(GSCDATA));
	COleDataSource source;
	GSLISTITEM vItem;

	CGameStudioView::m_pDragImage = m_cAnimationList.GetListCtrl()->CreateDragImage(
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

void CAnimationBar::OnLvnItemchangedListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	if( m_cAnimationList.m_bUISelect && (pNMLV->uChanged & LVIF_STATE) &&
		(pNMLV->uNewState & LVIS_SELECTED) &&
		(pNMLV->uOldState & LVIS_SELECTED) != (pNMLV->uNewState & LVIS_SELECTED) )
		MoveToAnimationPos(pNMLV->iItem);

	*pResult = 0;
}

void CAnimationBar::OnNMClickListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_cAnimationList.GetCurSel();

	if( nIndex == -1 )
		m_cAnimationList.SetCurSel(GetIndexFromTime(pDoc->m_fTime));

	*pResult = 0;
}

void CAnimationBar::OnNMDblclkListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	int nIndex = m_cAnimationList.GetCurSel();

	if( nIndex == -1 )
		m_cAnimationList.SetCurSel(GetIndexFromTime(pDoc->m_fTime));

	*pResult = 0;
}

void CAnimationBar::MoveToAnimationPos( int nIndex)
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

	nIndex = GetPosFromTime(GetTimeFromIndex(nIndex));
	pCtrl->m_cPlayCtrl.SetPos(nIndex);
	UpdateCtrl(nIndex);
}

void CAnimationBar::InsertAnimation( CAnimationItem *pItem,
									 BOOL bReverse,
									 int nIndex)
{
	CListCtrl *pListCtrl = m_cAnimationList.GetListCtrl();
	BOOL bReset = m_pItem->m_Aniset.m_vAni.empty();

	if( nIndex == -1 )
		nIndex = INT(m_pItem->m_vAniID.size());

	m_pItem->m_vAniID.insert(
		m_pItem->m_vAniID.begin() + nIndex,
		pItem->m_dwID);

	m_pItem->m_vReverse.insert(
		m_pItem->m_vReverse.begin() + nIndex,
		bReverse);

	TimeToPos();
	m_pItem->m_Aniset.InsertAniData(
		&pItem->GetAnimation(),
		bReverse,
		nIndex);
	PosToTime();

	nIndex = pListCtrl->InsertItem( nIndex, pItem->m_strName);
	pListCtrl->SetItemData( nIndex, (DWORD_PTR) pItem);

	if( !m_pItem->GetAnimation().IsEnable() && CGameStudioDoc::m_pPreview &&
		CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem == m_pItem )
		CGameStudioDoc::m_pPreview->Invalidate();

	m_cAnimationList.UpdateColumnWidth();
	MoveToAnimationPos(nIndex);

	if(bReset)
		((CAnimationDoc *) m_pItem->GetMainDocument())->ResetCamera();
}

void CAnimationBar::DeleteAnimation( int nIndex)
{
	CListCtrl *pListCtrl = m_cAnimationList.GetListCtrl();

	m_pItem->m_vAniID.erase(m_pItem->m_vAniID.begin() + nIndex);
	m_pItem->m_vReverse.erase(m_pItem->m_vReverse.begin() + nIndex);
	TimeToPos();
	m_pItem->m_Aniset.DeleteAniData(nIndex);
	PosToTime();

	pListCtrl->DeleteItem(nIndex);
	m_cAnimationList.UpdateColumnWidth();

	if( !m_pItem->GetAnimation().IsEnable() && CGameStudioDoc::m_pPreview &&
		CGameStudioDoc::m_pPreview->m_cPreviewCtrl.m_pItem == m_pItem )
		CGameStudioDoc::m_pPreview->Invalidate();

	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	pDoc->m_fTime = GetTimeFromPos(pCtrl->m_cPlayCtrl.GetPos());

	if(m_pItem->m_Aniset.m_vAni.empty())
		pDoc->ResetCamera();

	UpdateCtrl();
}

void CAnimationBar::MoveItemPos( int nFrom, int nTo)
{
	if( nFrom == nTo )
		return;

	CListCtrl *pListCtrl = m_cAnimationList.GetListCtrl();
	CAnimationItem *pItem = (CAnimationItem *) pListCtrl->GetItemData(nFrom);
	BOOL bReverse = m_pItem->m_vReverse[nFrom];

	DeleteAnimation(nFrom);
	InsertAnimation(
		pItem,
		bReverse,
		nTo);
}

void CAnimationBar::OnBnClickedButtonDeleteAnimation()
{
	DeleteAnimation(m_cAnimationList.GetCurSel());
}

void CAnimationBar::OnLvnItemActivateListAnimation( NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMIA = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//DeleteAnimation(pNMIA->iItem);
	*pResult = 0;
}

void CAnimationBar::OnBnClickedButtonMovedown()
{
	int nIndex = m_cAnimationList.GetCurSel();

	if( nIndex == INT(m_pItem->m_vAniID.size() - 1) )
		MoveItemPos( nIndex, 0);
	else
		MoveItemPos( nIndex, nIndex + 1);
}

void CAnimationBar::OnBnClickedButtonMoveup()
{
	int nIndex = m_cAnimationList.GetCurSel();

	if( nIndex == 0 )
		MoveItemPos( nIndex, INT(m_pItem->m_vAniID.size() - 1));
	else
		MoveItemPos( nIndex, nIndex - 1);
}

void CAnimationBar::OnBnClickedCheckAniloop()
{
	UpdateData();
	m_pItem->m_Aniset.m_bLoop = m_bAniLoop;
	m_cAniLoopID.EnableWindow(m_bAniLoop);
}

void CAnimationBar::OnBnClickedCheckReverse()
{
	CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();
	int nIndex = m_cAnimationList.GetCurSel();

	UpdateData();
	m_pItem->m_Aniset.m_vAni[nIndex]->m_bReverse = m_bReverse;
	m_pItem->m_vReverse[nIndex] = m_bReverse;

	if(!pCtrl->m_bPlay)
		pDoc->RedrawAllView();
}

void CAnimationBar::OnEnChangeEditAniloopid()
{
	UpdateData();
	if(!CItemBase::CheckID(m_strAniLoopID))
	{
		CString strMessage;
		CString strTitle;

		strMessage.LoadString(IDS_ERR_INVALID_ID);
		strTitle.LoadString(IDR_MAINFRAME);

		m_strAniLoopID = m_pItem->m_strLoopID;
		UpdateData(FALSE);

		MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);
		m_cAniLoopID.SetFocus();
	}
}

void CAnimationBar::OnEnSetfocusEditAniloopid()
{
	m_strAniLoopID = m_pItem->m_strLoopID;
	UpdateData(FALSE);
}

void CAnimationBar::OnEnKillfocusEditAniloopid()
{
	if( m_pItem->m_strLoopID == m_strAniLoopID )
	{
		if(m_strAniLoopID.IsEmpty())
		{
			m_strAniLoopID.LoadString(IDS_NOTUSE);
			UpdateData(FALSE);
		}

		return;
	}

	if(!m_pItem->m_strLoopID.IsEmpty())
	{
		CItemBase::ReleaseLocalID( m_pItem->m_strLoopID, m_pItem);
		m_pItem->m_Aniset.m_dwLoopID = 0;
	}

	if(!m_strAniLoopID.IsEmpty())
		if(!m_pItem->CheckDupLocalID( m_pItem->m_strLoopID, m_strAniLoopID))
		{
			m_strAniLoopID = m_pItem->m_strLoopID;
			if(m_strAniLoopID.IsEmpty())
				m_strAniLoopID.LoadString(IDS_NOTUSE);
			UpdateData(FALSE);

			PostMessage(WM_MESSAGEBOX);
		}
		else
		{
			m_pItem->m_Aniset.m_dwLoopID = CItemBase::RegLocalID( m_strAniLoopID, m_pItem);
			m_pItem->m_strLoopID = m_strAniLoopID;
		}
	else
	{
		m_pItem->m_strLoopID = m_strAniLoopID;

		m_strAniLoopID.LoadString(IDS_NOTUSE);
		UpdateData(FALSE);
	}
}

void CAnimationBar::TimeToPos()
{
	int nCount = INT(m_pItem->m_Aniset.m_vKey.size());

	for( int i=0; i<nCount; i++)
		m_pItem->m_Aniset.m_vKey[i]->m_fTime = (FLOAT) GetPosFromTime(m_pItem->m_Aniset.m_vKey[i]->m_fTime);
}

void CAnimationBar::PosToTime()
{
	int nCount = INT(m_pItem->m_Aniset.m_vKey.size());

	for( int i=0; i<nCount; i++)
		m_pItem->m_Aniset.m_vKey[i]->m_fTime = GetTimeFromPos(INT(m_pItem->m_Aniset.m_vKey[i]->m_fTime));
}

LRESULT CAnimationBar::OnMessageBox( WPARAM wParam, LPARAM lParam)
{
	CString strMessage;
	CString strTitle;

	strMessage.LoadString(IDS_ERR_DUP_ATTRID);
	strTitle.LoadString(IDR_MAINFRAME);
	MessageBox( strMessage, strTitle, MB_OK|MB_ICONHAND);

	return 0;
}

void CAnimationBar::OnBnClickedButtonClearkey()
{
	if( m_pItem && m_dwTotalFrame )
	{
		CAnimationDoc *pDoc = (CAnimationDoc *) m_pItem->GetMainDocument();
		CPlayCtrl *pCtrl = pDoc->GetPlayCtrl();

		if( pDoc && pCtrl )
		{
			m_pItem->m_Aniset.ClearKey();

			VECTORSTRING::iterator it, end;
			it = m_pItem->m_vEventID.begin();
			end = m_pItem->m_vEventID.end();
			for(; it != end ; ++it)
				CItemBase::ReleaseLocalID( *it, m_pItem );

			m_pItem->m_vEventID.clear();
			pDoc->m_fTime = 0.0f;

			pCtrl->m_cMarker.DelAll();
			UpdateCtrl();
			pDoc->RedrawAllView();
		}
	}
}
