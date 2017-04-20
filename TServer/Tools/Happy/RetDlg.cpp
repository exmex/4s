// RetDlg.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "RetDlg.h"


// CRetDlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CRetDlg, CDialog)
CRetDlg::CRetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRetDlg::IDD, pParent)
{
}

CRetDlg::~CRetDlg()
{
}

void CRetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LB_RET, m_lbRet);
}


BEGIN_MESSAGE_MAP(CRetDlg, CDialog)
END_MESSAGE_MAP()


// CRetDlg 메시지 처리기입니다.


void CRetDlg::SetMsg(CString str)
{
	/*
	CString strFont = _T("2002L");
	CFont Font;
	LOGFONT lf;	
	ZeroMemory(&lf, sizeof(lf));
	lf.lfHeight = 100;
	strcpy(lf.lfFaceName, strFont);
	Font.CreateFontIndirect(&lf);
*/
	//CFont Font;
	//Font.CreateFont(16,0,0,0,100,FALSE,FALSE,
	//	0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,
	//	CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
	//	NULL,"2002L");

	//m_lbRet.SetFont(&Font);

	CRect rect;
	GetClientRect(&rect);
	
	//CRect rRect;
	//GetDlgItem(IDOK)->GetWindowRect(&rRect);
	DWORD dwCharWidth = 8;	
	DWORD dwLen = str.GetLength();
	DWORD width = dwLen * dwCharWidth;
	m_lbRet.MoveWindow( (rect.Width() - width)/2 + 5, rect.top + 25,width + 5, 20);
	
	m_lbRet.SetWindowText(str);
}