#pragma once
#include "XListCtrl.h"
#include "afxwin.h"


// CPreVersion 대화 상자입니다.
enum {	STATE_NONE = 0,	
		STATE_NEW,		
		STATE_MOVE,		
		STATE_DEL,		
		STATE_NEWDEL,
		STATECOUNT};

class CPreVersion : public CDialog
{
	DECLARE_DYNAMIC(CPreVersion)

public:
	CPreVersion(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CPreVersion();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_VERSION };

protected:	
	VPATFILE		m_vPrePatchTable;
	BYTE			m_bSelectList;	//	1:TVERSION, 2:TPREVERSION

	CXListCtrl		m_lcVersion;
	CXListCtrl		m_lcPreVersion;
	CButton 		m_btnMove;
	CButton 		m_btnDel;

public:
	void Init();
	void InitList();
	void RefreshList();
	void AddNewFileList(LPVPATCHFILE pvList);

	void PushPreVerList(LPTPATFILE pPatch, DWORD dwData, int nItem = -1);
	void PushVerList(LPTPATFILE pPatch, DWORD dwData, int nItem = -1);
	
	inline void ClearPreVersionTable()	
		{ m_vPrePatchTable.clear(); }
	inline void PushPreVersionTable(TPATFILE pPath)
		{ m_vPrePatchTable.push_back(pPath); }
	inline void PushPreVersionTable(DWORD dwVersion, CString strPath, CString strName, DWORD dwSize, BYTE bState)
		{	TPATFILE pPath;pPath.m_dwVersion = dwVersion; pPath.m_strPath = strPath; pPath.m_strName = strName; strName; pPath.m_dwSize = dwSize; pPath.m_bState = bState;
			m_vPrePatchTable.push_back(pPath); }


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnTvermove();
	afx_msg void OnBnClickedBtnPredelete();
	afx_msg void OnBnClickedBtnPreupdate();
	afx_msg void OnNMClickLcTpreversion(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickLcTversion(NMHDR *pNMHDR, LRESULT *pResult);
};
