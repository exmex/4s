#pragma once
#include "BarContent.h"
#include "GSListCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CImageBar 대화 상자입니다.

class CImageBar : public CBarContent
{
	DECLARE_DYNAMIC(CImageBar)

	friend class CImageView;
public:
	CImageBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CImageBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_IMAGE };

private:
	static void CALLBACK OnImageListDragLeave( CGSListCtrl *pList);
	static BOOL CanImageListDrop( CItemBase *pItem);

	static DROPEFFECT CALLBACK OnImageListDragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static BOOL CALLBACK OnImageListDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

public:
	void SetItem( CImageItem *pItem);
	void UpdateCtrl();

	void DeleteImage( int nIndex);
	void InsertImage(
		CImageItem *pItem,
		int nIndex);

	void MoveItemPos(
		int nFrom,
		int nTo);

protected:
	DWORD GetTickFromIndex( int nIndex);
	DWORD GetTickFromPos( int nPos);

	void EnableAllCtrl( BOOL bEnable);
	void MoveToImagePos( int nIndex);
	void UpdateCtrl( int nPos);

	int GetIndexFromTick( DWORD dwTick);
	int GetPosFromTick( DWORD dwTick);

	int GetImageCompressionIndex( BYTE bFormat);

protected:
	CTColorCtrl m_cColorCtrl;
	CImageItem *m_pItem;

	int m_nInitHeight;
	int m_nImageFormat;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CGSListCtrl m_cImageList;
	CColorBox m_cColorBox;

	CSpinButtonCtrl m_spinValueA;
	CSpinButtonCtrl m_spinValueR;
	CSpinButtonCtrl m_spinValueG;
	CSpinButtonCtrl m_spinValueB;
	CSpinButtonCtrl m_spinKey;

	CStatic m_cControlBox;
	CString m_strKey;

	CButton m_cDeleteSprite;
	CButton m_cDownSprite;
	CButton m_cUpSprite;
	CButton m_cTimeSet;

	CButton m_cDeleteKey;
	CButton m_cAddKey;

	CEdit m_cValueA;
	CEdit m_cValueR;
	CEdit m_cValueG;
	CEdit m_cValueB;

	DWORD m_dwTotalFrame;
	DWORD m_dwTotalTick;
	DWORD m_dwValueA;
	DWORD m_dwValueR;
	DWORD m_dwValueG;
	DWORD m_dwValueB;
	DWORD m_dwFrame;

	CComboBox m_cImageFormat;

	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnColorSelected( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlProc( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlStop( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPos( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyMove( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyCopy( WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonAddkey();
	afx_msg void OnBnClickedButtonDeletekey();
	afx_msg void OnEnChangeEditA();
	afx_msg void OnEnChangeEditR();
	afx_msg void OnEnChangeEditG();
	afx_msg void OnEnChangeEditB();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonTimeset();
	afx_msg void OnLvnBegindragListSprite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListSprite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListSprite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeleteImage();
	afx_msg void OnLvnItemActivateListSprite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonMovedown();
	afx_msg void OnBnClickedButtonMoveup();
	afx_msg void OnCbnSelchangeComboImgFormat();
};
