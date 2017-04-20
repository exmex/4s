#pragma once
#include "afxcmn.h"


// CDlgPatchProgress 대화 상자입니다.

class CDlgPatchProgress : public CDialog
{
	DECLARE_DYNAMIC(CDlgPatchProgress)

public:
	CDlgPatchProgress(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgPatchProgress();

	void SetProgress(BYTE bPos);
	void SetStatus(BYTE bStatus);
	void SetFileName(CString strName);

	BYTE m_bStatus;
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PATCHPROGRESS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl m_progress;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnCancel();
};
