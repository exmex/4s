#pragma once


// CTFontDialogView 폼 뷰입니다.

class CTFontDialogView : public CFormView
{
	DECLARE_DYNCREATE(CTFontDialogView)

	enum { TDEFAULT_FONT_TYPE = 4 };

protected:
	CTFontDialogView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTFontDialogView();

public:
	vtLPTFONT_TYPE	m_vtFontType;

public:
	void	InitFontData();
	void	ReleaseFontData();

	void	UpdateFont();

public:
	enum { IDD = IDD_FONTDIALOG_FORMVIEW };
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	LOGFONT lf;

	CString m_strFontName;
	CString m_strText;
	
	INT m_nFontSize;
	INT m_nFontRender;
	CRect m_rtPos;

	BOOL m_bOutLine;
	BOOL m_bShadow;

	DWORD m_dwFontColor;


	
	virtual void OnInitialUpdate();
	afx_msg void OnEnChangeEditFontText();
	afx_msg void OnBnClickedCheckFont2Outline();
	afx_msg void OnBnClickedCheckFont2Shadow();
	afx_msg void OnCbnSelchangeComboFontRender();
	afx_msg void OnBnClickedBtnFontFontselect();
	afx_msg void OnCbnSelchangeComboFontFontsize();
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnBnClickedBtnFontFontselect2();
	afx_msg void OnBnClickedBtnImgLoad();
};


