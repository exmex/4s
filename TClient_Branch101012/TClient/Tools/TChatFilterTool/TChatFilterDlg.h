#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CTChatFilterDlg 대화 상자
class CTChatFilterDlg : public CDialog
{
public:
	CTChatFilterDlg(CWnd* pParent = NULL);

	enum { IDD = IDD_TCHATFILTER_DIALOG };

	protected:
	virtual void			DoDataExchange(CDataExchange* pDX);
	virtual void			OnOK();

protected:
	HICON m_hIcon;

	virtual BOOL			OnInitDialog();
	virtual BOOL			PreTranslateMessage( MSG* pMsg );

	afx_msg void			OnPaint();
	afx_msg HCURSOR			OnQueryDragIcon();
	afx_msg void			OnLoadtxtFilterwordlist();
	afx_msg void			OnLoadtxtExceptionwordlist();
	afx_msg void			OnEnChangeEditIgnorenumchar();
	afx_msg void			OnEnChangeEditChat();
	afx_msg void			OnBnClickedCheckXspacebar();
	afx_msg void			OnBnClickedCheckXspchar();
	afx_msg void			OnBnClickedCheckChatallblock();
	afx_msg void			OnBnClickedCheckWordlist();
	afx_msg void			OnBnClickedCheckExceptlist();
	afx_msg void			OnEnChangeEditChatallblock();
	afx_msg void			OnFileLoadTCD();
	afx_msg void			OnFileLoadtxt();
	afx_msg void			OnSavetxtFilterwordlist();
	afx_msg void			OnSavetxtExceptionlist();
	afx_msg void			OnFileSaveTCD();
	afx_msg void			OnFileExit();

	DECLARE_MESSAGE_MAP()

public:

	// Option.
	CButton					m_IgnoreSpace;
	CButton					m_IgnoreNumChar;
	CButton					m_BlockAllSentence;
	CEdit					m_editChatBlock;

	// Controls.
	CEdit					m_editIgnoreNumChar;
	CEdit					m_editChat;
	CEdit					m_editResult;
	CButton					m_UseWord;
	CButton					m_UseException;
	CListCtrl				m_listWords;
	CListCtrl				m_listExcept;

	// Data.
	CTChatFilterFile		m_FilterFile;
	CTChatFilterSys			m_FilterSys;

public :

	void					InitControls();
	void					UpdateOptions();
	void					LoadWords();
	void					LoadException();
	void					UpdateChatFilterTest();
	bool					UpdateListWords();
	bool					UpdateListExcept();
	void					ReleaseList();
	void					ReleaseListExcept();
};
