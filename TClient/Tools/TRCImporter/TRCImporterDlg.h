// TRCImporterDlg.h : 헤더 파일
//

#pragma once

#define GERMANY 0
#define ENGLISH 1
#define JAPAN 2

// CTRCImporterDlg 대화 상자
class CTRCImporterDlg : public CDialog
{
// 생성
public:
	CTRCImporterDlg(CWnd* pParent = NULL);	// 표준 생성자

// 대화 상자 데이터
	enum { IDD = IDD_TRCIMPORTER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원


// 구현
protected:
	HICON m_hIcon;

	// 메시지 맵 함수를 생성했습니다.
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CString m_strRCPath;
	CString m_strCSV;
	int m_nNation;
	afx_msg void OnBnClickedOk();
	int Import(CString strCSVPath, CString strRCPath, CString strTargetPath, int nNation);
	afx_msg void OnBnClickedButton4();
	CString m_strTargetPath;
};
