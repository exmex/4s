// DlgConnect.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "MainFrm.h"
#include "HappyDoc.h"
#include "HappyView.h"
#include "DlgConnect.h"
#include ".\dlgconnect.h"




// CDlgConnect 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgConnect, CDialog)
CDlgConnect::CDlgConnect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgConnect::IDD, pParent)
	, m_dwIP(0)
	, m_dwPort(0)
	, m_strUserID(_T(""))
	, m_strPassword(_T(""))
{		
}

CDlgConnect::~CDlgConnect()
{
}

void CDlgConnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_IPAddress(pDX, IDC_IPADDRESS_IP, m_dwIP);
	DDX_Text(pDX, IDC_EDIT_PORT, m_dwPort);
	DDX_Text(pDX, IDC_EDIT_USER, m_strUserID);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_strPassword);
	DDX_Control(pDX, IDC_COMBO1, m_cbAccount);
}

BEGIN_MESSAGE_MAP(CDlgConnect, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, OnCbnSelchangeCombo1)
END_MESSAGE_MAP()


BOOL CDlgConnect::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	
	//CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PASSWORD);
	//if(pMsg->message == WM_KEYDOWN)
	//{
	//	if(pMsg->hwnd == pEdit->m_hWnd)
	//	{
	//		if(pMsg->wParam == VK_RETURN)
	//			return FALSE;
	//	}
	//}   

	return CDialog::PreTranslateMessage(pMsg);
}

void CDlgConnect::Init()
{
	while(m_cbAccount.GetCount())
		m_cbAccount.DeleteString(0);
	
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}

	SetCurrentDirectory(pDoc->m_szCurrentDirectory);
	CStdioFile file;
	CFileException exFile;	
	if( file.Open( "HappyIP.txt", CFile::modeRead | CFile::typeText,&exFile ) )
	{
		ACCOUNT stAccount;
		BYTE bIndex = 0;
		while(TRUE)
		{			
			if(!file.ReadString(stAccount.m_str[bIndex]))
			{
				break;
			}			
			bIndex++;
			bIndex = bIndex % 5;
			if(bIndex == 0 )
			{
				CString str = stAccount.m_str[0];
				m_cbAccount.AddString(str);
				m_mapACCOUNT.insert(map<CString,ACCOUNT>::value_type(stAccount.m_str[0],stAccount));				
			}
		}

		file.Close();
		GetDlgItem(IDC_COMBO1)->ShowWindow(SW_SHOW);
	}
	else
	{
		TCHAR errmsg[1024];
		exFile.GetErrorMessage(errmsg,1024);

		CString strErr = errmsg;
		//AfxMessageBox(errmsg);
	}
}
void CDlgConnect::OnCbnSelchangeCombo1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	m_cbAccount.GetLBText(m_cbAccount.GetCurSel(),str);
	if(str == _T(""))
		return;

	map<CString,ACCOUNT>::iterator it = m_mapACCOUNT.find(str);
	if( it == m_mapACCOUNT.end())
		return;

	ACCOUNT stAccount = (*it).second;
	str = stAccount.m_str[1];
	CString tok,strReverseIP;
	int nPos,nPoint;
	nPos = nPoint = 0;
	tok = str.Tokenize(_T("."),nPos);
	while(tok != _T(""))
	{
		strReverseIP = tok + strReverseIP;
		if(nPoint < 3)
		{
			strReverseIP = _T(".") + strReverseIP;
			nPoint++;
		}
		tok = str.Tokenize(_T("."),nPos); //AfxExtractSubString		
	}

	m_dwIP = inet_addr(strReverseIP.GetBuffer(0));
	m_dwPort = atoi(stAccount.m_str[2].GetBuffer(0));
	m_strUserID = stAccount.m_str[3];
	m_strPassword = stAccount.m_str[4];

	UpdateData(FALSE);
}

// 모달 다이얼로그는 OnInitDialog() 가 실행되어야만 컨트롤(m_cbAccount)이 생성된다.
BOOL CDlgConnect::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	
	Init();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
