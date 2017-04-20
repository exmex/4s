// IPChangeDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GMTool.h"
#include "IPChangeDlg.h"
#include ".\ipchangedlg.h"

#include "GMTool.h"
#include "GMToolDlg.h"

// CIPChangeDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CIPChangeDlg, CDialog)
CIPChangeDlg::CIPChangeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIPChangeDlg::IDD, pParent)	
{
}

CIPChangeDlg::~CIPChangeDlg()
{
}

void CIPChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_ctrIPAddr);
}


BEGIN_MESSAGE_MAP(CIPChangeDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CIPChangeDlg 메시지 처리기입니다.

void CIPChangeDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	DWORD dwPort = GetDlgItemInt(IDC_EDIT_PORT);
	if( dwPort <= 0 )
	{
		MessageBox(MSG_ENTER_AGAIN,MB_OK);
		return;
	}

	CString strIP;
	m_ctrIPAddr.GetWindowText(strIP);
	if(strIP == _T("0.0.0.0") )
	{
		MessageBox(MSG_ENTER_AGAIN,MB_OK);
		return;
	}

	CGMToolApp *pApp = (CGMToolApp *)AfxGetApp(); // App 포인프 얻어내서
	CGMToolDlg *pdlgGMTool = (CGMToolDlg *) pApp->GetMainWnd(); // 메인윈도우 얻어서 접근
	if(pdlgGMTool)
	{
		pdlgGMTool->SetIPPort(strIP,dwPort);
		pdlgGMTool->SetFocus();
	}

	OnOK();
}
