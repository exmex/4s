// PlayCtrlBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Game Studio.h"
#include "PlayCtrlBar.h"


// CPlayCtrlBar

IMPLEMENT_DYNAMIC(CPlayCtrlBar, CGSDialogBar)
CPlayCtrlBar::CPlayCtrlBar()
{
}

CPlayCtrlBar::~CPlayCtrlBar()
{
	if(m_pContent)
		delete m_pContent;
}

BEGIN_MESSAGE_MAP(CPlayCtrlBar, CGSDialogBar)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CPlayCtrlBar 메시지 처리기입니다.

BOOL CPlayCtrlBar::Create( CWnd* pParentWnd, DWORD dwStyle)
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

	m_pContent = new CDialog();
	m_pContent->Create( IDD_PLAYCTRL_BAR, this);
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

void CPlayCtrlBar::OnDestroy()
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

CPlayCtrl* CPlayCtrlBar::GetPlayCtrl()
{
	if(!m_pContent)
		return NULL;

	CWnd *pContainer = m_pContent->GetDlgItem(IDC_CUSTOM_PLAYCTRL);
	if(!pContainer)
		return NULL;

	return (CPlayCtrl *) pContainer->GetDlgItem(IDD_DIALOG_PLAYCTRL);
}
