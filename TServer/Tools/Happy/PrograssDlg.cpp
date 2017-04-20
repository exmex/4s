// PrograssDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "PrograssDlg.h"
#include ".\prograssdlg.h"


// CPrograssDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CPrograssDlg, CDialog)
CPrograssDlg::CPrograssDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPrograssDlg::IDD, pParent)
{
	m_dwTotal = 0;
	m_dwCurrent = 0;
}

CPrograssDlg::~CPrograssDlg()
{
}

void CPrograssDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_hProgressBar);
}

void CPrograssDlg::SetProgressRange(DWORD dwTotal)
{
	m_hProgressBar.SetRange( 0, 100 );
	m_dwTotal = dwTotal;
	m_dwCurrent = 0;
	m_hProgressBar.SetPos(0);
}
void CPrograssDlg::SetProgressVal(DWORD dwVal)
{
	
	m_dwCurrent = dwVal;
	int nPos = dwVal / (m_dwTotal / 100);
	m_hProgressBar.SetPos(nPos);
}

BEGIN_MESSAGE_MAP(CPrograssDlg, CDialog)
	ON_BN_CLICKED(IDC_BT_COMPLITE, OnBnClickedBtComplite)
END_MESSAGE_MAP()
