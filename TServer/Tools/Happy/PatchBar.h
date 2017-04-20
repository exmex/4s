#pragma once

// CPatchBar 대화 상자입니다.

class CPatchBar : public CDialogBar
{
	DECLARE_DYNAMIC(CPatchBar)

public:
	CPatchBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPatchBar();

	void GetFtp(CString *strAddr, CString *strUser, CString *strPasswd, WORD *wPort);
	void SetFtp(CString *strAddr, CString *strUser, DWORD *wPort);
// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PATCHBAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnUpdateCompareShow(CCmdUI *pCmdUI);		// 06/02/01 박상연 
	afx_msg void OnUpdatePatchUpdateShow(CCmdUI *pCmdUI);	// 06/02/01 박상연 

public:
	BYTE	m_bEnableCompare;								// Compare 버튼 활성화 상태
	BYTE	m_bEnablePatchUpdate;							// PatchUpdate 버튼 활성화 상태	

	DWORD m_dwIP;
	DWORD m_dwPort;
	CString m_strUserID;
	CString m_strPassword;
};
