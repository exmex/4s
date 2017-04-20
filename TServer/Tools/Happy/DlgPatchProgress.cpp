// DlgPatchProgress.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Happy.h"
#include "DlgPatchProgress.h"
#include ".\dlgpatchprogress.h"
#include "MainFrm.h"
#include "HappyDoc.h"


// CDlgPatchProgress 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgPatchProgress, CDialog)
CDlgPatchProgress::CDlgPatchProgress(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPatchProgress::IDD, pParent)
{
	m_bStatus = 0xFF;
}

CDlgPatchProgress::~CDlgPatchProgress()
{
}

void CDlgPatchProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_PATCH, m_progress);
}

void CDlgPatchProgress::SetProgress(BYTE bPos)
{
	m_progress.SetPos(bPos);
}

void CDlgPatchProgress::SetFileName(CString strName)
{
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PATCHFILENAME);
	pEdit->SetWindowText(strName);
}

void CDlgPatchProgress::SetStatus(BYTE bStatus)
{
	CEdit * pEdit = (CEdit *)GetDlgItem(IDC_EDIT_PATCHSTATUS);
	CString strStatus;
	switch(bStatus)
	{
	case 0: 				strStatus = _T("The patch has been successfully completed"); break; //"패치가 성공적으로 완료 되었습니다."
	case 1: 				strStatus = _T("Zipping the files..."); break; //파일 압축 중..
	case 2: 				strStatus = _T("Uploading the files..."); break; //파일 업로드 중...
	case 3: 				strStatus = _T("The error occurred during the patch"); break; //패치중 에러가 발생했습니다.
	case _FTP_CONNECT:		strStatus = _T("Connecting to a FTP server "); break;//FTP서버에 접속중입니다.....
	case _ERR_TEMPDIR:		strStatus = _T("Creating a temporary directory error occurred"); break; //임시 디렉토리 생성중 에러가 발생했습니다.
	case _ERR_CURRENTDIR:	strStatus = _T("During set the current directory the error occurred"); break; //현제 디렉토리 설정중 에러가 발생했습니다.
	case _ERR_ZIP:			strStatus = _T("Generating a Zip file error occurred"); break; //Zip 파일 생성중 에러가 발생했습니다.
	case _ERR_EMPTY:		strStatus = _T("IP/PORT/ACCOUNT/PASSWORD is empty"); break; //IP/PORT/ACCOUNT/PASSWORD가 비어있습니다.
	case _ERR_ADDRORACCOUNT:strStatus = _T("Reconfirm the IP/PORT/ACCOUNT/PASSWORD "); break; //IP/PORT/ACCOUNT/PASSWORD를 재확인 해주십시요.
	case _ERR_THREADFAIL:	strStatus = _T("Failed to create a thread"); break; //Thread 생성을 실패 하였습니다.
	case _ERR_DBWRITE:		strStatus = _T("The error has occurred during the Update DB"); break; //DB에 Update중 에러가 발생했습니다.
	}
	m_bStatus = bStatus;
	pEdit->SetWindowText(strStatus);
}

BEGIN_MESSAGE_MAP(CDlgPatchProgress, CDialog)
END_MESSAGE_MAP()


// CDlgPatchProgress 메시지 처리기입니다.

BOOL CDlgPatchProgress::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_progress.SetRange(0,100);
	m_progress.SetPos(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgPatchProgress::OnCancel()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return;
	}
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if(m_bStatus == 0 ||
		m_bStatus == 3 ||
		m_bStatus ==_ERR_TEMPDIR ||		
		m_bStatus ==_ERR_CURRENTDIR ||
		m_bStatus ==_ERR_ZIP ||	
		m_bStatus ==_ERR_EMPTY ||		
		m_bStatus ==_ERR_ADDRORACCOUNT ||
		m_bStatus ==_ERR_THREADFAIL ||
		m_bStatus ==_ERR_DBWRITE
		)
	{
		if(m_bStatus == 0 && pDoc->m_bPatchType == 2)
			pDoc->m_dlgPreVersion->Init();//ShowWindow(SW_SHOW);
		CDialog::OnCancel();
	}
}
