#pragma once
#include "afxwin.h"

struct tagBackIMG
{
	INT m_nSizeX;
	INT m_nSizeY;

	FLOAT m_fSizeRateX;
	FLOAT m_fSizeRateY;

	FLOAT m_fMaxDist;
	FLOAT m_fMinDist;
	FLOAT m_fDist;

	BYTE m_bFixSizeScreen;
	BYTE m_bPosFix;
};

typedef tagBackIMG BACKIMAGE, *LPBACKIMAGE;

// CSetBackIMGDlg 대화 상자입니다.

class CSetBackIMGDlg : public CDialog
{
	DECLARE_DYNAMIC(CSetBackIMGDlg)

public:
	CSetBackIMGDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetBackIMGDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SETBACKIMG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	virtual BOOL OnInitDialog();

public:
	void InitData(LPBACKIMAGE pBackIMG, LPVOID pWnd);

	DECLARE_MESSAGE_MAP()

public:
	LPBACKIMAGE m_pBackIMG; 

	LPVOID m_pVIEW;

	CEdit m_cDISTEDIT;
	CEdit m_cSizeXEDIT;
	CEdit m_cSizeYEDIT;
	CEdit m_cRateSizeXEDIT;
	CEdit m_cRateSizeYEDIT;

	CString m_strDist;
	CString m_strSizeX;
	CString m_strSizeY;
	CString m_strRateSizeX;
	CString m_strRateSizeY;
	
	CScrollBar m_cSizeXScroll;
	CScrollBar m_cSizeYScroll;
	CScrollBar m_cDistScroll;

	CButton m_cConstrain;

	BOOL m_bFixSizeScreen;
	BOOL m_bConstrain;

	int m_nDist;
	
public:
	void SetDistScrollPos(UINT nPos);
	
	void SetSizeXScrollPos(UINT nSizeX);
	void SetSizeYScrollPos(UINT nSizeY);

	void EnableAllCtrl(BYTE bEnable);

	void OnChangeSize();
public:
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedCheckScreenfix();
	afx_msg void OnBnClickedCheckConstrain();
	afx_msg void OnChangeSizeX();
	afx_msg void OnChangeSizeY();
	CButton m_cPOSFIX;
	BOOL m_bPOSFIX;
	afx_msg void OnBnClickedCheckFixpos();
};
