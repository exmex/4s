#pragma once
#include "BarContent.h"
#include "GSListCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CAnimationBar 대화 상자입니다.

class CAnimationBar : public CBarContent
{
	DECLARE_DYNAMIC(CAnimationBar)

public:
	CAnimationBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CAnimationBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_ANIMATION };

private:
	static void CALLBACK OnAnimationListDragLeave( CGSListCtrl *pList);
	static BOOL CanAnimationListDrop( CItemBase *pItem);

	static DROPEFFECT CALLBACK OnAnimationListDragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static BOOL CALLBACK OnAnimationListDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

public:
	void SetItem( CAnimationItem *pItem);
	void UpdateCtrl();

	void DeleteAnimation( int nIndex);
	void InsertAnimation(
		CAnimationItem *pItem,
		BOOL bReverse,
		int nIndex);

	void MoveItemPos(
		int nFrom,
		int nTo);

protected:
	CAnimationItem *m_pItem;

protected:
	FLOAT GetTimeFromIndex( int nIndex);
	FLOAT GetTimeFromPos( int nPos);

	void MoveToAnimationPos( int nIndex);
	void EnableAllCtrl( BOOL bEnable);
	void UpdateCtrl( int nPos);
	void TimeToPos();
	void PosToTime();

	int GetIndexFromTime( FLOAT fTime);
	int GetPosFromTime( FLOAT fTime);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CGSListCtrl m_cAnimationList;

	CSpinButtonCtrl m_spinValueRX;
	CSpinButtonCtrl m_spinValueRY;
	CSpinButtonCtrl m_spinValueRZ;
	CSpinButtonCtrl m_spinValueSX;
	CSpinButtonCtrl m_spinValueSY;
	CSpinButtonCtrl m_spinValueSZ;
	CSpinButtonCtrl m_spinValueX;
	CSpinButtonCtrl m_spinValueY;
	CSpinButtonCtrl m_spinValueZ;
	CSpinButtonCtrl m_spinKey;

	CEdit m_cAniLoopID;
	CEdit m_cEventID;
	CEdit m_cValueRX;
	CEdit m_cValueRY;
	CEdit m_cValueRZ;
	CEdit m_cValueSX;
	CEdit m_cValueSY;
	CEdit m_cValueSZ;
	CEdit m_cValueX;
	CEdit m_cValueY;
	CEdit m_cValueZ;

	CButton m_cDeleteAnimation;
	CButton m_cDownAnimation;
	CButton m_cUpAnimation;
	CButton m_cDeleteKey;
	CButton m_cAddKey;
	CButton m_cAniLoop;
	CButton m_cTimeSet;
	CButton m_cReverse;

	CString m_strTimeScale;
	CString m_strKey;
	CString m_strValueRX;
	CString m_strValueRY;
	CString m_strValueRZ;
	CString m_strValueSX;
	CString m_strValueSY;
	CString m_strValueSZ;
	CString m_strValueX;
	CString m_strValueY;
	CString m_strValueZ;
	CString m_strAniLoopID;
	CString m_strEventID;

	DWORD m_dwTotalFrame;
	DWORD m_dwTotalTime;
	DWORD m_dwFrame;

	BOOL m_bAniLoop;
	BOOL m_bReverse;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnPlayCtrlProc( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlStop( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPos( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageBox( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyMove( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyCopy( WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonAddkey();
	afx_msg void OnBnClickedButtonDeletekey();
	afx_msg void OnEnChangeEditRx();
	afx_msg void OnEnChangeEditRy();
	afx_msg void OnEnChangeEditRz();
	afx_msg void OnEnChangeEditSx();
	afx_msg void OnEnChangeEditSy();
	afx_msg void OnEnChangeEditSz();
	afx_msg void OnEnChangeEditX();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnEnChangeEditZ();
	afx_msg void OnEnChangeEditEvent();
	afx_msg void OnEnSetfocusEditEvent();
	afx_msg void OnEnKillfocusEditEvent();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonTimeset();
	afx_msg void OnLvnBegindragListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeleteAnimation();
	afx_msg void OnLvnItemActivateListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMovedown();
	afx_msg void OnBnClickedButtonMoveup();
	afx_msg void OnBnClickedCheckAniloop();
	afx_msg void OnBnClickedCheckReverse();
	afx_msg void OnEnChangeEditAniloopid();
	afx_msg void OnEnSetfocusEditAniloopid();
	afx_msg void OnEnKillfocusEditAniloopid();
	afx_msg void OnNMDblclkListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonClearkey();
};
