// EditJointDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TPathTime.h"
#include "EditJointDlg.h"


// CEditJointDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CEditJointDlg, CDialog)
CEditJointDlg::CEditJointDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditJointDlg::IDD, pParent)
{
	for( int i=0; i<8; i++)
		m_vJOINT[i] = FALSE;

	m_vRECT.SetRectEmpty();
	m_pTJOINT = NULL;
	m_dwID = 0;
}

CEditJointDlg::~CEditJointDlg()
{
}

void CEditJointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_LEFT, m_vRECT.left);
	DDX_Text(pDX, IDC_EDIT_TOP, m_vRECT.top);
	DDX_Text(pDX, IDC_EDIT_RIGHT, m_vRECT.right);
	DDX_Text(pDX, IDC_EDIT_BOTTOM, m_vRECT.bottom);
	DDX_Check(pDX, IDC_BUTTON_JOINT_0, m_vJOINT[0]);
	DDX_Check(pDX, IDC_BUTTON_JOINT_1, m_vJOINT[1]);
	DDX_Check(pDX, IDC_BUTTON_JOINT_2, m_vJOINT[2]);
	DDX_Check(pDX, IDC_BUTTON_JOINT_3, m_vJOINT[3]);
	DDX_Check(pDX, IDC_BUTTON_JOINT_4, m_vJOINT[4]);
	DDX_Check(pDX, IDC_BUTTON_JOINT_5, m_vJOINT[5]);
	DDX_Check(pDX, IDC_BUTTON_JOINT_6, m_vJOINT[6]);
	DDX_Check(pDX, IDC_BUTTON_JOINT_7, m_vJOINT[7]);
}


BEGIN_MESSAGE_MAP(CEditJointDlg, CDialog)
END_MESSAGE_MAP()


// CEditJointDlg 메시지 처리기입니다.

BOOL CEditJointDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	HICON hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	CString strTITLE;

	strTITLE.Format( _T("Edit Joint (ID = %d)"), m_dwID);
	SetIcon( hIcon, FALSE);
	SetIcon( hIcon, TRUE);

	for( int i=0; i<8; i++)
		m_vJOINT[i] = m_pTJOINT->m_bJOINT & (BYTE(1) << i) ? TRUE : FALSE;
	m_vRECT = m_pTJOINT->m_vRECT;

	SetWindowText(LPCSTR(strTITLE));
	UpdateData(FALSE);

	return TRUE;
}

void CEditJointDlg::OnOK()
{
	UpdateData();
	m_pTJOINT->m_vRECT = m_vRECT;
	m_pTJOINT->m_bJOINT = 0;

	for( int i=0; i<8; i++)
		if(m_vJOINT[i])
			m_pTJOINT->m_bJOINT |= BYTE(1) << i;

	CDialog::OnOK();
}
