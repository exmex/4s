// SavePosDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "GMTool.h"
#include "SavePosDlg.h"
#include ".\saveposdlg.h"

#include "GMToolDoc.h"

// CSavePosDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CSavePosDlg, CDialog)
CSavePosDlg::CSavePosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSavePosDlg::IDD, pParent)
	, m_strSaveName(_T(""))
{
}

CSavePosDlg::~CSavePosDlg()
{
}

void CSavePosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SAVENAME, m_editSaveName);
}


BEGIN_MESSAGE_MAP(CSavePosDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// CSavePosDlg 메시지 처리기입니다.

void CSavePosDlg::Init()
{
	m_strSaveName = "";
	SetDlgItemText(IDC_EDIT_SAVENAME,"");
	m_editSaveName.SetFocus();
	
}

void CSavePosDlg::SetUserPos(USERPOS& _UserPos)
{
	m_stUserPos.ChannelNum = _UserPos.ChannelNum;
	m_stUserPos.MapID = _UserPos.MapID;
	m_stUserPos.PosX = _UserPos.PosX;
	m_stUserPos.PosY = _UserPos.PosY;
	m_stUserPos.PosZ = _UserPos.PosZ;
	strcpy(m_stUserPos.POSNAME, _UserPos.POSNAME);
	
}

void CSavePosDlg::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData();
	
	GetDlgItemText(IDC_EDIT_SAVENAME,m_strSaveName);
	if( m_strSaveName == _T(""))
	{
		MessageBox(MSG_CHARNAME_NULL, MB_OK);// 이름을 입력해야 합니다
	}
	else
	{		
		CGMToolDoc * pDoc = CGMToolDoc::GetInstance();
		if(!pDoc)
			return;
		
		strcpy(m_stUserPos.POSNAME , m_strSaveName);		
		pDoc->SavePos(m_stUserPos);
	}

	OnOK();
}

BOOL CSavePosDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW, 0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
