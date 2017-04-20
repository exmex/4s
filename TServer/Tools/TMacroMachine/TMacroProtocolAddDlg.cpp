// TMacroProtocolAddDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TMacroMachine.h"
#include "TMacroProtocolAddDlg.h"
#include ".\tmacroprotocoladddlg.h"


// CTMacroProtocolAddDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTMacroProtocolAddDlg, CDialog)
CTMacroProtocolAddDlg::CTMacroProtocolAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMacroProtocolAddDlg::IDD, pParent)
{
}

CTMacroProtocolAddDlg::~CTMacroProtocolAddDlg()
{
	m_dwSelectIndex = 0;
}

void CTMacroProtocolAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbProtocol);
	DDX_Control(pDX, IDC_COMBO3, m_cbSelf);
	DDX_Control(pDX, IDC_COMBO2, m_cbLoop);
	DDX_Control(pDX, IDC_CHECK1, m_ckRoot);
}


BEGIN_MESSAGE_MAP(CTMacroProtocolAddDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BTN_FINDPT, OnBnClickedBtnFindProtocol)
END_MESSAGE_MAP()


// CTMacroProtocolAddDlg 메시지 처리기입니다.
BOOL CTMacroProtocolAddDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cbSelf.AddString(_T("FALSE"));
	m_cbSelf.AddString(_T("TRUE"));
	m_cbSelf.SetCurSel(0);

	m_cbLoop.AddString(_T("FALSE"));
	m_cbLoop.AddString(_T("TRUE"));
	m_cbLoop.SetCurSel(0);
	
	SetDlgItemText(IDC_EDIT1, _T("0"));

	MAPTPROTOCOL::iterator it;
	for(it=m_mapProtocol.begin(); it!=m_mapProtocol.end(); it++)
	{
		DWORD dwID = m_cbProtocol.AddString((*it).second);
		m_cbProtocol.SetItemData(dwID, (*it).first);
	}
	m_cbProtocol.SetCurSel(0);

	SetProtocolAttr();

	m_dwSelectIndex = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTMacroProtocolAddDlg::SetProtocol(MAPTPROTOCOL mapProtocol)
{
	m_mapProtocol = mapProtocol;
}

void CTMacroProtocolAddDlg::OnCbnSelchangeCombo1()
{
	SetProtocolAttr();
}

void CTMacroProtocolAddDlg::OnBnClickedOk()
{
	m_dwID = DWORD(m_cbProtocol.GetItemData(m_cbProtocol.GetCurSel()));
	m_cbProtocol.GetLBText(m_cbProtocol.GetCurSel(), m_strName);
	m_bSelf = m_cbSelf.GetCurSel();
	m_bLoop = m_cbLoop.GetCurSel();

	CString str;
	GetDlgItemText(IDC_EDIT1, str);
	m_wSleep = atoi(str);

	m_bCheck = m_ckRoot.GetCheck();

	m_dwSelectIndex = 0;
	OnOK();
}

void CTMacroProtocolAddDlg::SetProtocolAttr()
{
	DWORD dwID = DWORD(m_cbProtocol.GetItemData(m_cbProtocol.GetCurSel()));

	MAPTPROTOCOL::iterator find = m_mapProtocol.find(dwID);
	if(find==m_mapProtocol.end())
		return;

	if(strstr((*find).second, _T("_REQ")))
	{
		m_cbSelf.SetCurSel(0);
		m_cbLoop.SetCurSel(0);

		GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO3)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(TRUE);
		m_ckRoot.SetCheck(TRUE);
	}
	else
	{
		SetDlgItemText(IDC_EDIT1, _T("0"));
		m_cbSelf.SetCurSel(0);
		m_cbLoop.SetCurSel(0);

		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO3)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO2)->EnableWindow(FALSE);
		m_ckRoot.SetCheck(FALSE);
	}
}
void CTMacroProtocolAddDlg::OnBnClickedBtnFindProtocol()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString szProtocol;
	CString strTemp;
	strTemp.Empty();
	GetDlgItemText(IDC_EDIT_FINDPT,strTemp);
	if (strTemp.IsEmpty() )
		return;


	MAPTPROTOCOL::iterator it = m_mapProtocol.begin();
	DWORD dwIndex = 0;
	while(dwIndex < m_dwSelectIndex )
	{
		it++;
		dwIndex++;
	}

	if(it == m_mapProtocol.end() )
	{
		m_dwSelectIndex = 0;
		it = m_mapProtocol.begin();
	}

	strTemp.MakeUpper();
	for( ; it != m_mapProtocol.end(); it++, m_dwSelectIndex++)
	{
		szProtocol = (*it).second;		
		if( szProtocol.Find(strTemp) != -1 )		
		{
			m_cbProtocol.SetFocus();
			m_cbProtocol.SetCurSel(m_dwSelectIndex);
			m_dwSelectIndex++;
			return;
		}
	}

	if( it == m_mapProtocol.end() )
	{
		m_dwSelectIndex = 0;
		AfxMessageBox("Not Found    ",MB_OK);
	}
}
