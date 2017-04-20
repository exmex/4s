#pragma once
#include "ItemBase.h"
#include "afxcmn.h"
#include "afxwin.h"


// CMediaDlg 대화 상자입니다.

class CMediaDlg : public CDialog
{
	DECLARE_DYNAMIC(CMediaDlg)

public:
	CMediaDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMediaDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MEDIA_BAR };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CMediaItem* m_pItem;

public:
	void SetItem( CMediaItem *pItem);
	void SetVolume( int nVolume);
	void SetPos();

public:
	CSliderCtrl m_cVolume;
	CStatic m_cValue;

	DWORD m_dwVolume;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnPlayCtrlProc( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlStop( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPos( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMediaMsg( WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
