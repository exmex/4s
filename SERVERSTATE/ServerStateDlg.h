// ServerStateDlg.h : 헤더 파일
//

#pragma once


// CServerStateDlg 대화 상자
class CServerStateDlg : public CDialog
{
// 생성
public:
	CServerStateDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_SERVERSTATE_DIALOG };

protected:
	virtual void DoDataExchange( CDataExchange* pDX);	// DDX/DDV 지원

	CListCtrl m_ctrlServerState;
	BOOL m_bChecked;

protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	void DoBeep( BOOL flag);

	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedCheckButton();
	afx_msg void OnBnClickedListButton();
	afx_msg void OnBnClickedCheck1();
	afx_msg void OnLvnDeleteitemList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
};
