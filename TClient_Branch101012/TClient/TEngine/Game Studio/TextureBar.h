#pragma once
#include "BarContent.h"
#include "GSListCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CTextureBar 대화 상자입니다.

class CTextureBar : public CBarContent
{
	DECLARE_DYNAMIC(CTextureBar)

	friend class CTextureView;
public:
	CTextureBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTextureBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_TEXTURE };

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
	void SetItem( CTextureItem *pItem);
	void UpdateCtrl();

	void DeleteTexture( int nIndex);
	void InsertTexture(
		CTextureItem *pItem,
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
	int GetFilterIndex( DWORD dwFilter);
	int GetTextureCompressionIndex( BYTE bFormat);

protected:
	CTColorCtrl m_cColorCtrl;
	CTextureItem *m_pItem;

	int m_nInitHeight;
	int m_nMipFilter;
	int m_nTextureOption;
	int m_nTextureFormat;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CGSListCtrl m_cImageList;
	CColorBox m_cColorBox;

	CSpinButtonCtrl m_spinRotation;
	CSpinButtonCtrl m_spinValueSU;
	CSpinButtonCtrl m_spinValueSV;
	CSpinButtonCtrl m_spinValueU;
	CSpinButtonCtrl m_spinValueV;

	CSpinButtonCtrl m_spinLODBias;
	CSpinButtonCtrl m_spinKey;

	CComboBox m_cMipFilter;
	CStatic m_cControlBox;

	CButton m_cDeleteSprite;
	CButton m_cDownSprite;
	CButton m_cDeleteKey;
	CButton m_cUpSprite;
	CButton m_cTimeSet;
	CButton m_cAddKey;

	CButton m_cSphere;
	CButton m_cPlane;
	CButton m_cBox;

	CEdit m_cLODBias;
	CEdit m_cValueSU;
	CEdit m_cValueSV;
	CEdit m_cValueU;
	CEdit m_cValueV;
	CEdit m_cValueR;

	DWORD m_dwTotalFrame;
	DWORD m_dwTotalTick;
	DWORD m_dwFrame;

	CString m_strRotation;
	CString m_strValueSU;
	CString m_strValueSV;
	CString m_strValueU;
	CString m_strValueV;

	CString m_strLODBias;
	CString m_strKey;

	CComboBox m_cTextureFormat;

	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnColorSelected( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlProc( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlStop( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPos( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyMove( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyCopy( WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize( UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonAddkey();
	afx_msg void OnEnChangeEditU();
	afx_msg void OnEnChangeEditV();
	afx_msg void OnBnClickedButtonDeletekey();
	afx_msg void OnEnChangeEditR();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonTimeset();
	afx_msg void OnLvnItemActivateListSprite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBegindragListSprite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListSprite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListSprite(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeleteImage();
	afx_msg void OnBnClickedButtonMovedown();
	afx_msg void OnBnClickedButtonMoveup();
	afx_msg void OnEnChangeEditLodbias();
	afx_msg void OnCbnSelchangeComboMipfilter();
	afx_msg void OnBnClickedRadioMeshSphere();
	afx_msg void OnBnClickedRadioMeshPlane();
	afx_msg void OnBnClickedRadioMeshBox();
	afx_msg void OnEnChangeEditSu();
	afx_msg void OnEnChangeEditSv();
	afx_msg void OnCbnSelchangeComboFullsize();
	afx_msg void OnCbnSelchangeComboFormat();
	afx_msg void OnBnClickedCheckShowTiling();
	BOOL m_bShowTiling;
};
