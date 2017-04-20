#pragma once
#include "afxcmn.h"

// CDlgUploadProgress 대화 상자입니다.

class CDlgUploadProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgUploadProgress)

public:
	CDlgUploadProgress(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgUploadProgress();
	void SetPos(DWORD dwPos);
	CString m_strFileName;
	DWORD m_dwFileSize;

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_UPLOADPROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	CProgressCtrl m_progress;

	virtual BOOL OnInitDialog();
};
