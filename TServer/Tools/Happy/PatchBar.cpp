// PatchBar.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "PatchBar.h"
#include ".\patchbar.h"


// CPatchBar 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPatchBar, CDialogBar)
CPatchBar::CPatchBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
	, m_dwIP(0)
	, m_dwPort(0)
	, m_strUserID(_T(""))
	, m_strPassword(_T(""))
{
}

CPatchBar::~CPatchBar()
{
}

void CPatchBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	
	DDX_IPAddress(pDX, IDC_IPADDRESS_FTP, m_dwIP);
	DDX_Text(pDX, IDC_EDIT_FTP_ACCOUNT, m_strUserID);
	DDX_Text(pDX, IDC_EDIT_FTP_PASSWD, m_strPassword);
}

void CPatchBar::GetFtp(CString *strAddr, CString *strUser, CString *strPasswd, WORD *wPort)
{
	GetDlgItemText( IDC_IPADDRESS_FTP, *strAddr );
	GetDlgItemText( IDC_EDIT_FTP_ACCOUNT, *strUser );
	GetDlgItemText( IDC_EDIT_FTP_PASSWD, *strPasswd);
	*wPort = GetDlgItemInt( IDC_EDIT_FTP_PORT );
}

void CPatchBar::SetFtp(CString *strAddr, CString *strUser, DWORD *dwPort)
{
	SetDlgItemText( IDC_IPADDRESS_FTP, *strAddr );
	SetDlgItemText( IDC_EDIT_FTP_ACCOUNT, *strUser );
	SetDlgItemInt( IDC_EDIT_FTP_PORT, *dwPort );
}

// 06/02/01 박상연 
void CPatchBar::OnUpdateCompareShow(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bEnableCompare);
}

// 06/02/01 박상연 
void CPatchBar::OnUpdatePatchUpdateShow(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_bEnablePatchUpdate);
}


BEGIN_MESSAGE_MAP(CPatchBar, CDialogBar)
	ON_UPDATE_COMMAND_UI( IDC_BUTTON_COMPARE, OnUpdateCompareShow )
	ON_UPDATE_COMMAND_UI( IDC_BUTTON_PATCHUP, OnUpdatePatchUpdateShow )
END_MESSAGE_MAP()

