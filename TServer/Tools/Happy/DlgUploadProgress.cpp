// DlgUploadProgress.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "DlgUploadProgress.h"
#include ".\dlguploadprogress.h"

// CDlgUploadProgress 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgUploadProgress, CDialog)
CDlgUploadProgress::CDlgUploadProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgUploadProgress::IDD, pParent)
{
}

CDlgUploadProgress::~CDlgUploadProgress()
{

}

void CDlgUploadProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_SERVICEUPLOAD, m_progress);
}


BEGIN_MESSAGE_MAP(CDlgUploadProgress, CDialog)
END_MESSAGE_MAP()


// CDlgUploadProgress 메시지 처리기입니다.

BOOL CDlgUploadProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_progress.SetRange(0, 100);
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_CURFILENAME);
	CString strText;
	strText.Format("%s(0/%d)",m_strFileName,m_dwFileSize);
	pEdit->SetWindowText(strText);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CDlgUploadProgress::SetPos(DWORD dwPos)
{
	m_progress.SetPos(dwPos);
}
