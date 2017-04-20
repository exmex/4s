#include "stdafx.h"
#include "TMonSpawnInspector.h"
#include "TMonSpawnInspectorCnPDlg.h"
#include "TMonSpawnInspectorDlg.h"

IMPLEMENT_DYNAMIC(CTMonSpawnInspectorCnPDlg, CDialog)
CTMonSpawnInspectorCnPDlg::CTMonSpawnInspectorCnPDlg(CWnd* pParent /*=NULL*/)
: CDialog(CTMonSpawnInspectorCnPDlg::IDD, pParent)
{
}

CTMonSpawnInspectorCnPDlg::~CTMonSpawnInspectorCnPDlg()
{
}

void CTMonSpawnInspectorCnPDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DATA, m_edtDATA);
}

BOOL CTMonSpawnInspectorCnPDlg::OnInitDialog()
{
	UpdateData(FALSE);

	m_edtDATA.SetLimitText( 1024*1024*5 );
	return TRUE;
}


BEGIN_MESSAGE_MAP(CTMonSpawnInspectorCnPDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	//	ON_WM_CREATE()
END_MESSAGE_MAP()


// CTMonSpawnInspectorCnPDlg 메시지 처리기입니다.

void CTMonSpawnInspectorCnPDlg::OnBnClickedOk()
{
	UpdateData();
	if( m_pDest )
		m_edtDATA.GetWindowText( *m_pDest );
	OnOK();
}