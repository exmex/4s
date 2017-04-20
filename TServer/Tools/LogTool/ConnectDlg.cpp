/////////////////////////////////////////////////////////////////////////////
//	File Name	:	ConnectDlg.cpp
//
//	Copyright	:	(c) Zemi Interactive, Inc 2002~2008
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "LogTool.h"
#include "ConnectDlg.h"
#include "Profile.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg dialog


CConnectDlg::CConnectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CConnectDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CConnectDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CConnectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CConnectDlg)
	DDX_Control(pDX, IDC_PASSWORD, m_ctrlPassword);
	DDX_Control(pDX, IDC_ACCOUNT, m_ctrlAccount);
	DDX_Control(pDX, IDC_COMBO_CONNECT, m_ctrlConnect);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CConnectDlg, CDialog)
	//{{AFX_MSG_MAP(CConnectDlg)
	ON_BN_CLICKED(IDOK, OnOk)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CConnectDlg message handlers

BOOL CConnectDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	

	CProfile ServerConfig("LogTool.ini");
	int nList = ServerConfig.GetIntKey("GENERAL", "NUM_SERVERS", 0);

	if(GETINSTANCE()->m_pServList)
	{
		for(int i=0; i<nList; i++)
		{
			m_ctrlConnect.InsertString(i, GETINSTANCE()->m_pServList[i].szServName);
		}
	}
	else
	{
		GETINSTANCE()->m_pServList = new SERVERINFO[nList];

		if(GETINSTANCE()->m_pServList)
		{
			
			char szName[20];

			for(int i=0; i<nList; i++)
			{
				wsprintf(szName, "SERVER %d", i+1);
				ServerConfig.GetStringKey(szName, "SERV_NAME",		GETINSTANCE()->m_pServList[i].szServName,		sizeof(GETINSTANCE()->m_pServList[i].szServName),		"");
				ServerConfig.GetStringKey(szName, "LOGDB_ADDR",		GETINSTANCE()->m_pServList[i].szLogServAddr,	sizeof(GETINSTANCE()->m_pServList[i].szLogServAddr),	"");
				ServerConfig.GetStringKey(szName, "LOGDB_DSN",		GETINSTANCE()->m_pServList[i].szLogServDSN,		sizeof(GETINSTANCE()->m_pServList[i].szLogServDSN),		"");
				ServerConfig.GetStringKey(szName, "GAMEDB_ADDR",	GETINSTANCE()->m_pServList[i].szGameServAddr,	sizeof(GETINSTANCE()->m_pServList[i].szGameServAddr),	"");
				ServerConfig.GetStringKey(szName, "CHARTDB_ADDR",	GETINSTANCE()->m_pServList[i].szChartServAddr,	sizeof(GETINSTANCE()->m_pServList[i].szChartServAddr),	"");

				m_ctrlConnect.InsertString(i, GETINSTANCE()->m_pServList[i].szServName);
			}
		}
	}

	m_ctrlConnect.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectDlg::OnOk() 
{
	// TODO: Add your control notification handler code here
	m_nConnectIdx = m_ctrlConnect.GetCurSel();
	m_ctrlAccount.GetWindowText(m_strAccID);
	m_ctrlPassword.GetWindowText(m_strPWD);

	m_ctrlConnect.GetWindowText(m_strDBName);

	EndDialog(1);
}

BOOL CConnectDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class

	return CDialog::DestroyWindow();
}
