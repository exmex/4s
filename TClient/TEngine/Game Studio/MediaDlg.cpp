// MediaDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "BarContent.h"
#include "MediaDoc.h"
#include "MediaDlg.h"
#include "PlayCtrl.h"
#include "MainFrm.h"


// CMediaDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC( CMediaDlg, CDialog)
CMediaDlg::CMediaDlg( CWnd* pParent /*=NULL*/)
: CDialog(CMediaDlg::IDD, pParent)
, m_dwVolume(0)
{
	m_pItem = NULL;
}

CMediaDlg::~CMediaDlg()
{
}

void CMediaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_VOLUME, m_cVolume);
	DDX_Text(pDX, IDC_STATIC_VOLUME, m_dwVolume);
	DDX_Control(pDX, IDC_STATIC_VOLUME, m_cValue);
}

BEGIN_MESSAGE_MAP( CMediaDlg, CDialog)
	ON_MESSAGE( WM_PLAYCTRL_PROC, OnPlayCtrlProc)
	ON_MESSAGE( WM_PLAYCTRL_PLAY, OnPlayCtrlPlay)
	ON_MESSAGE( WM_PLAYCTRL_STOP, OnPlayCtrlStop)
	ON_MESSAGE( WM_PLAYCTRL_POS, OnPlayCtrlPos)
	ON_MESSAGE( WM_MEDIA_MSG, OnMediaMsg)
	ON_WM_SIZE()
	ON_WM_VSCROLL()
END_MESSAGE_MAP()


// CMediaDlg 메시지 처리기입니다.

BOOL CMediaDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cValue.SetFont(CPlayCtrl::GetFont());
	m_cVolume.SetRange( 0, 100);
	m_cVolume.SetTicFreq(50);
	m_dwVolume = 100 - m_cVolume.GetPos();

	UpdateData(FALSE);
	return TRUE;
}

void CMediaDlg::OnSize( UINT nType, int cx, int cy)
{
	if( m_cVolume.GetSafeHwnd() &&
		m_cValue.GetSafeHwnd() )
	{
		CWnd *pPlayCtrl = GetDlgItem(IDC_CUSTOM_PLAYCTRL)->GetDlgItem(IDD_DIALOG_PLAYCTRL);

		CRect rcVolume;
		CRect rcValue;
		CRect rect;

		pPlayCtrl->GetWindowRect(&rect);
		ScreenToClient(&rect);

		m_cVolume.GetWindowRect(&rcVolume);
		m_cValue.GetWindowRect(&rcValue);
		ScreenToClient(&rcVolume);
		ScreenToClient(&rcValue);

		rcVolume.bottom = rect.bottom;
		rcValue.bottom = rect.bottom;

		m_cVolume.MoveWindow(&rcVolume);
		m_cValue.MoveWindow(&rcValue);
	}

	CDialog::OnSize( nType, cx, cy);
}

void CMediaDlg::SetItem( CMediaItem *pItem)
{
	SetVolume(pItem->m_bVolume);
	m_pItem = pItem;
}

void CMediaDlg::SetVolume( int nVolume)
{
	m_cVolume.SetPos(100 - nVolume);
	m_dwVolume = nVolume;

	UpdateData(FALSE);
}

void CMediaDlg::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if( pScrollBar == (CScrollBar *) &m_cVolume)
	{
		BYTE bMediaType = m_pItem->GetPlayerType();

		m_dwVolume = 100 - m_cVolume.GetPos();
		UpdateData(FALSE);

		switch(bMediaType)
		{
		case MEDIA_TMUSIC	:
			{
				CT3DMusic *pOBJ = (CT3DMusic *) CMainFrame::m_TachyonMedia.GetData(
					MEDIA_TMUSIC,
					m_pItem->m_dwID);

				if(pOBJ)
					pOBJ->m_bVolume = (BYTE) m_dwVolume;
			}

			break;

		case MEDIA_TSOUND	:
			{
				CD3DSound *pOBJ = (CD3DSound *) CMainFrame::m_TachyonMedia.GetData(
					MEDIA_TSOUND,
					m_pItem->m_dwID);

				if(pOBJ)
					pOBJ->m_bVolume = (BYTE) m_dwVolume;
			}

			break;

		case MEDIA_TSHOW	:
			{
				CTShow *pOBJ = (CTShow *) CMainFrame::m_TachyonMedia.GetData(
					MEDIA_TSHOW,
					m_pItem->m_dwID);

				if(pOBJ)
					pOBJ->m_bVolume = (BYTE) m_dwVolume;
			}

			break;
		}

		CMainFrame::m_TachyonMedia.ResetVolume(
			bMediaType,
			m_pItem->m_dwID);
		m_pItem->m_bVolume = (BYTE) m_dwVolume;
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

LRESULT CMediaDlg::OnPlayCtrlProc( WPARAM wParam, LPARAM lParam)
{
	DS3DLISTENER vLISTENER;

	CD3DSound::GetListener(&vLISTENER);
	vLISTENER.vPosition.x = 0.0f;
	vLISTENER.vPosition.y = 0.0f;
	vLISTENER.vPosition.z = 0.0f;
	CD3DSound::SetListener( &vLISTENER, DS3D_IMMEDIATE);

	SetPos();
	return 0;
}

LRESULT CMediaDlg::OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam)
{
	CMediaDoc *pDoc = (CMediaDoc *) m_pItem->GetMainDocument();
	DS3DLISTENER vLISTENER;

	CD3DSound::GetListener(&vLISTENER);
	vLISTENER.vPosition.x = 0.0f;
	vLISTENER.vPosition.y = 0.0f;
	vLISTENER.vPosition.z = 0.0f;
	CD3DSound::SetListener( &vLISTENER, DS3D_IMMEDIATE);

	CMainFrame::m_TachyonMedia.Play(
		m_pItem->GetPlayerType(),
		m_pItem->m_dwID,
		pDoc->m_nIndex);

	return 0;
}

LRESULT CMediaDlg::OnPlayCtrlStop( WPARAM wParam, LPARAM lParam)
{
	CMediaDoc *pDoc = (CMediaDoc *) m_pItem->GetMainDocument();

	CMainFrame::m_TachyonMedia.Pause(
		m_pItem->GetPlayerType(),
		m_pItem->m_dwID,
		pDoc->m_nIndex);

	return 0;
}

LRESULT CMediaDlg::OnPlayCtrlPos( WPARAM wParam, LPARAM lParam)
{
	CMediaDoc *pDoc = (CMediaDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pPlayCtrl = pDoc->GetPlayCtrl();

	LONGLONG lLength = CMainFrame::m_TachyonMedia.GetLength(
		m_pItem->GetPlayerType(),
		m_pItem->m_dwID);
	LONGLONG lPos = lLength * pPlayCtrl->m_cPlayCtrl.GetPos() / 100;

	CMainFrame::m_TachyonMedia.SetPos(
		m_pItem->GetPlayerType(),
		m_pItem->m_dwID,
		pDoc->m_nIndex,
		lPos);

	return 0;
}

LRESULT CMediaDlg::OnMediaMsg( WPARAM wParam, LPARAM lParam)
{
	CMediaDoc *pDoc = (CMediaDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pPlayCtrl = pDoc->GetPlayCtrl();

	IMediaEventEx *pEvent = (IMediaEventEx *) lParam;
	LONG lParam1;
	LONG lParam2;
	LONG lEvent;

	while(SUCCEEDED(pEvent->GetEvent( &lEvent, &lParam1, &lParam2, 0)))
	{
		pEvent->FreeEventParams( lEvent, lParam1, lParam2);

		switch(lEvent)
		{
		case EC_COMPLETE	:
			{
				if(pPlayCtrl->m_bLoop)
				{
					CMainFrame::m_TachyonMedia.Stop( MEDIA_TSHOW, m_pItem->m_dwID, pDoc->m_nIndex);
					CMainFrame::m_TachyonMedia.Play( MEDIA_TSHOW, m_pItem->m_dwID, pDoc->m_nIndex);
				}
				else
				{
					pPlayCtrl->OnBnClickedButtonStop();
					CMainFrame::m_TachyonMedia.Stop( MEDIA_TSHOW, m_pItem->m_dwID, pDoc->m_nIndex);
				}
			}

			break;
		}
	}
	SetPos();

	return 0;
}

void CMediaDlg::SetPos()
{
	CMediaDoc *pDoc = (CMediaDoc *) m_pItem->GetMainDocument();
	CPlayCtrl *pPlayCtrl = pDoc->GetPlayCtrl();
	BYTE bType = m_pItem->GetPlayerType();

	LONGLONG lLength = CMainFrame::m_TachyonMedia.GetLength(
		m_pItem->GetPlayerType(),
		m_pItem->m_dwID);

	LONGLONG lPos = CMainFrame::m_TachyonMedia.GetPos(
		m_pItem->GetPlayerType(),
		m_pItem->m_dwID,
		pDoc->m_nIndex);

	if( bType != MEDIA_TSHOW &&
		!CMainFrame::m_TachyonMedia.IsPlay(
		bType,
		m_pItem->m_dwID,
		pDoc->m_nIndex))
	{
		if(pPlayCtrl->m_bLoop)
		{
			CMainFrame::m_TachyonMedia.Stop( bType, m_pItem->m_dwID, pDoc->m_nIndex);
			CMainFrame::m_TachyonMedia.Play( bType, m_pItem->m_dwID, pDoc->m_nIndex);
		}
		else
		{
			pPlayCtrl->OnBnClickedButtonStop();
			CMainFrame::m_TachyonMedia.Stop( bType, m_pItem->m_dwID, pDoc->m_nIndex);
		}

		lPos = 0;
	}

	lPos = lPos * 100 / lLength;
	pPlayCtrl->SetPos(INT(lPos));
}
