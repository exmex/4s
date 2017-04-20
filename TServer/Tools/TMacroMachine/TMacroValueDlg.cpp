// TMacroValueDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "TMacroMachine.h"
#include "TMacroValueDlg.h"
#include ".\tmacrovaluedlg.h"


// CTMacroValueDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CTMacroValueDlg, CDialog)
CTMacroValueDlg::CTMacroValueDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTMacroValueDlg::IDD, pParent)
{
	m_bValueType = 0;
	m_bType = 0;
	m_strName = _T("");
	m_strValue = _T("");
}

CTMacroValueDlg::~CTMacroValueDlg()
{
}

void CTMacroValueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbValueType);
	DDX_Control(pDX, IDC_COMBO2, m_cbType);
}


BEGIN_MESSAGE_MAP(CTMacroValueDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_CBN_SELCHANGE(IDC_COMBO2, OnCbnSelchangeCombo2)
END_MESSAGE_MAP()


// CTMacroValueDlg 메시지 처리기입니다.

BOOL CTMacroValueDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cbValueType.AddString(STR_TYPE_BYTE);
	m_cbValueType.AddString(STR_TYPE_WORD);
	m_cbValueType.AddString(STR_TYPE_DWORD);
	m_cbValueType.AddString(STR_TYPE_FLOAT);
	m_cbValueType.AddString(STR_TYPE_STRING);
	m_cbValueType.AddString(STR_TYPE_INT64);
	m_cbValueType.AddString(STR_TYPE_SOCKADDR);
	m_cbValueType.SetCurSel(0);

	m_cbType.AddString(STR_TYPE_INPUT);
	m_cbType.AddString(STR_TYPE_OUTPUT);
	m_cbType.AddString(STR_TYPE_SELF);
	m_cbType.AddString(STR_TYPE_END);
	m_cbType.SetCurSel(0);

	if(!m_strName.IsEmpty())
	{
		m_cbValueType.SetCurSel(m_bValueType);
		m_cbType.SetCurSel(m_bType);
		SetDlgItemText(IDC_EDIT1, m_strName);
		SetDlgItemText(IDC_EDIT2, m_strValue);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CTMacroValueDlg::SetValue(BYTE bValueType, BYTE bType, CString strName, CString strValue)
{
	m_bValueType = bValueType;
	m_bType = bType;
	m_strName = strName;
	m_strValue = strValue;
}

void CTMacroValueDlg::OnCbnSelchangeCombo2()
{
	GetDlgItem(IDC_EDIT1)->EnableWindow(TRUE);
	GetDlgItem(IDC_EDIT2)->EnableWindow(TRUE);

	SetDlgItemText(IDC_EDIT1, _T(""));
	SetDlgItemText(IDC_EDIT2, _T(""));

	if(m_cbType.GetCurSel() == TYPE_OUTPUT)
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	else if(m_cbType.GetCurSel() == TYPE_SELF)
	{
		SetDlgItemText(IDC_EDIT1, STR_SELF_NAME);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	}
	else if(m_cbType.GetCurSel() == TYPE_END)
	{
		SetDlgItemText(IDC_EDIT1, STR_END_NAME);
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	}
}

void CTMacroValueDlg::OnBnClickedOk()
{
	m_bValueType = m_cbValueType.GetCurSel();
	m_bType = m_cbType.GetCurSel();
	GetDlgItemText(IDC_EDIT1, m_strName);
	GetDlgItemText(IDC_EDIT2, m_strValue);

	//md5 encrypt
	//m_strValue = EncryptPW(m_strValue);

	if(m_strName == _T(""))
	{
		AfxMessageBox(_T("이름에 공백을 넣을수는 없습니다."));
		return;
	}

	OnOK();
}

CString CTMacroValueDlg::EncryptPW(CString& strSOURCE)
{
	MD5 context;
	context.update( (unsigned char*) strSOURCE.GetBuffer(), strSOURCE.GetLength() );
	context.finalize();

	char* pstr = context.hex_digest();
	CString strRESULT( pstr );
	delete [] pstr;
	
	return strRESULT;
}