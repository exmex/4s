// ShrinkTMODlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


typedef struct tagTSIMPLEOBJ
{
	DWORD m_dwItemID;

	D3DXVECTOR3 m_vSCALE;
	D3DXVECTOR3 m_vROT;
	D3DXVECTOR3 m_vPOS;
} TSIMPLEOBJ, *LPTSIMPLEOBJ;

typedef vector<LPTSIMPLEOBJ>		VTSIMPLEOBJ, *LPVTSIMPLEOBJ;


// CShrinkTMODlg 대화 상자
class CShrinkTMODlg : public CDialog
{
// 생성
public:
	CShrinkTMODlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_SHRINKTMO_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원

public:
	BYTE IsTMOFile( CString strFileName);
	BYTE FindMAPOBJ(
		LPVTSIMPLEOBJ pBUF,
		D3DXVECTOR3& vSCALE,
		D3DXVECTOR3& vROT,
		D3DXVECTOR3& vPOS,
		DWORD dwItemID);

	CString LoadString( CFile *pFile);
	void WriteString(
		CFile *pFile,
		CString& strValue);

	void Regulate(
		CString strSRC,
		CString strDEST);

	void Regulate(
		CFile *pSRC,
		CFile *pDEST);

public:
	CString m_strDESTFOLDER;
	CString m_strSRCFOLDER;

	CString m_strDEST;
	CString m_strSRC;

	CButton m_cDEST;
	CButton m_cSRC;

// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonDest();
	afx_msg void OnBnClickedButtonSrcfolder();
	afx_msg void OnBnClickedRegulate();
};
