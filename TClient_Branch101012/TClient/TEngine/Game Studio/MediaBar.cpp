// MediaBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "MediaBar.h"
#include "MediaDlg.h"


// CMediaBar

IMPLEMENT_DYNAMIC(CMediaBar, CGSDialogBar)
CMediaBar::CMediaBar()
{
}

CMediaBar::~CMediaBar()
{
	if(m_pContent)
		delete m_pContent;
}

BEGIN_MESSAGE_MAP(CMediaBar, CGSDialogBar)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CMediaBar 메시지 처리기입니다.

BOOL CMediaBar::Create( CWnd* pParentWnd, DWORD dwStyle)
{
	m_dwStyle = dwStyle & CBRS_ALIGN_ANY;
	CRect rect;

	BOOL bResult = CControlBar::Create(
		NULL, NULL,
		WS_VISIBLE|WS_CHILD|dwStyle,
		CRect(
		0, 0,
		10, 10),
		pParentWnd,
		IDD_PLAYCTRL_BAR);

	m_pContent = new CMediaDlg();
	m_pContent->Create( IDD_MEDIA_BAR, this);
	m_pContent->UpdateWindow();
	m_pContent->ShowWindow(SW_SHOW);

	CPlayCtrl *pCtrl = GetPlayCtrl();
	if(pCtrl)
	{
		pCtrl->GetWindowRect(&rect);

		rect.InflateRect( 0, 3, 0, 8);
		m_szDefault = rect.Size();
		m_szContent = rect.Size();
	}

	return bResult;
}

void CMediaBar::OnDestroy()
{
	if(m_pContent)
	{
		if(m_pContent->GetSafeHwnd())
			m_pContent->DestroyWindow();

		delete m_pContent;
		m_pContent = NULL;
	}

	CGSDialogBar::OnDestroy();
}

CPlayCtrl* CMediaBar::GetPlayCtrl()
{
	if(!m_pContent)
		return NULL;

	CWnd *pContainer = m_pContent->GetDlgItem(IDC_CUSTOM_PLAYCTRL);
	if(!pContainer)
		return NULL;

	return (CPlayCtrl *) pContainer->GetDlgItem(IDD_DIALOG_PLAYCTRL);
}
