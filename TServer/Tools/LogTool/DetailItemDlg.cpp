////////////////////////////////////////////////////////////////////////////////
//	File Name	:	DetailItemDlg.cpp
//
//	Copyright	:	(c) Zemi Interactive, Inc 2002~2008
//
////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogTool.h"
#include "DetailItemDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
//	CDetailItemDlg dialog
/////////////////////////////////////////////////////////////////////////////
CDetailItemDlg::CDetailItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDetailItemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDetailItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDetailItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDetailItemDlg)
	DDX_Control(pDX, IDC_LIST_DETAIL, m_ctrlDetail);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDetailItemDlg, CDialog)
	//{{AFX_MSG_MAP(CDetailItemDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDetailItemDlg message handlers

BOOL CDetailItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDetailItemDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	this->ShowWindow(SW_HIDE);
}


BOOL CDetailItemDlg::InsertList(int idx, LPCTSTR str)
{
	m_ctrlDetail.InsertString(idx, str);
	m_ctrlDetail.SetItemHeight(idx, 20);

	return TRUE;
}

void CDetailItemDlg::ClearAllItems()
{
	while(m_ctrlDetail.GetCount())
	{
		for(int i=0; i<m_ctrlDetail.GetCount(); i++)
		{
			m_ctrlDetail.DeleteString(i);
		}
	}
}
