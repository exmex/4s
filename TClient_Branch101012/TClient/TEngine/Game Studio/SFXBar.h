#pragma once
#include "BarContent.h"
#include "GSListCtrl.h"
#include "GSEditCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "FrameSkip.h"


// CSFXBar 대화 상자입니다.

class CSFXBar : public CBarContent
{
	DECLARE_DYNAMIC(CSFXBar)

public:
	CSFXBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSFXBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SFX };

private:
	static void CALLBACK OnSFXDragLeave( CGSListCtrl *pList);
	static void CALLBACK OnSRCDragLeave( CGSEditCtrl *pEdit);
	static void CALLBACK OnANIDragLeave( CGSEditCtrl *pEdit);

	static BOOL CanSFXDrop( CSFXBar *pBar, CItemBase *pItem);
	static BOOL CanSRCDrop( CSFXBar *pBar, CItemBase *pItem);
	static BOOL CanANIDrop( CSFXBar *pBar, CItemBase *pItem);

	static DROPEFFECT CALLBACK OnSFXDragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static DROPEFFECT CALLBACK OnSRCDragOver(
		CGSEditCtrl *pEdit,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static DROPEFFECT CALLBACK OnANIDragOver(
		CGSEditCtrl *pEdit,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static BOOL CALLBACK OnSFXDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static BOOL CALLBACK OnSRCDrop(
		CGSEditCtrl *pEdit,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static BOOL CALLBACK OnANIDrop(
		CGSEditCtrl *pEdit,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

public:
	void SetItem( CSFXItem *pItem);
	void UpdateCtrl( int nPos);
	void UpdateCtrl();

	void DeleteSFX( int nIndex);
	void InsertSFX(
		CSFXItem *pItem,
		int nIndex);

	void MoveItemPos(
		int nFrom,
		int nTo);

protected:
	CSFXItem *m_pItem;
	DWORD m_dwTick;

protected:
	int GetPosFromTick( DWORD dwTick);
	DWORD GetTickFromPos( int nPos);
	DWORD GetBlend( int nIndex);

	void EnableAllCtrl( BOOL bEnable);
	void BeginUpdateInst();
	void EndUpdateInst();
	void InitSFX();

	void SetANISRC( CAnimationItem *pItem);
	void SetSFXSRC( CItemBase *pItem);

	void SetCLKList( COBJItem *pItem);
	void SetACTList( COBJItem *pItem);

protected:
	virtual void DoDataExchange( CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CSpinButtonCtrl m_spinOBJFuncID;
	CSpinButtonCtrl m_spinACTFuncID;
	CSpinButtonCtrl m_spinANIFuncID;
	CSpinButtonCtrl m_spinLifeTime;
	CSpinButtonCtrl m_spinSizeX;
	CSpinButtonCtrl m_spinSizeY;
	CSpinButtonCtrl m_spinKey;
	CSpinButtonCtrl m_spinA;
	CSpinButtonCtrl m_spinR;
	CSpinButtonCtrl m_spinG;
	CSpinButtonCtrl m_spinB;
	CSpinButtonCtrl m_spinOptionLevel;
	CSpinButtonCtrl m_spinLodLevel;
	CSpinButtonCtrl m_spinCurLodLevel;
	CSpinButtonCtrl m_spinFPS;

	CGSEditCtrl m_cSFXSRC;
	CGSEditCtrl m_cANISRC;
	CGSListCtrl m_cCSFX;

	CButton m_cSetParticle;
	CButton m_cParticle;
	CButton m_cTimeSet;
	CButton m_cZEnable;
	CButton m_cZWrite;
	CButton m_cAddKey;
	CButton m_cDelKey;
	CButton m_cSingle;
	CButton m_cDELSRC;
	CButton m_cDELAS;

	CEdit m_cOBJFuncID;
	CEdit m_cACTFuncID;
	CEdit m_cANIFuncID;
	CEdit m_cLifeTime;
	CEdit m_cSizeX;
	CEdit m_cSizeY;
	CEdit m_cA;
	CEdit m_cR;
	CEdit m_cG;
	CEdit m_cB;
	CEdit m_cOptionLevel;
	CEdit m_cLodLevel;
	CEdit m_cCurLodLevel;
	CEdit m_cSetFPS;

	CComboBox m_cDESTBlend;
	CComboBox m_cSRCBlend;
	CComboBox m_cBlendOP;
	CComboBox m_cCLK;
	CComboBox m_cCL;
	CComboBox m_cMESH;
	CComboBox m_cACT;
	CComboBox m_cANI;

	CStatic m_cTotalFrame;
	CStatic m_cFrame;
	CStatic m_cKey;

	CString m_strSFXSRC;
	CString m_strANISRC;
	CString m_strSizeX;
	CString m_strSizeY;
	CString m_strKey;

	DWORD m_dwTotalFrame;
	DWORD m_dwOBJFuncID;
	DWORD m_dwACTFuncID;
	DWORD m_dwANIFuncID;
	DWORD m_dwLifeTime;
	DWORD m_dwValueA;
	DWORD m_dwValueR;
	DWORD m_dwValueG;
	DWORD m_dwValueB;
	DWORD m_dwFrame;

	BOOL m_bZEnable;
	BOOL m_bZWrite;

	int m_nSFXTYPE;
	int m_nCLK;
	int m_nCL;
	int m_nMESH;
	int m_nACT;
	int m_nANI;

	BYTE m_bOptionLodLevel;
	DWORD m_dwCurLevel;
	DWORD m_dwSelLevel;

	CFrameSkip m_FrameRender;
	DWORD m_dwFPS;
	DWORD m_dwCount;

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnPlayCtrlProc( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlStop( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPos( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyMove( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnKeyCopy( WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonSetparticle();
	afx_msg void OnBnClickedRadioParticle();
	afx_msg void OnBnClickedRadioSingle();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnEnChangeEditSizex();
	afx_msg void OnEnChangeEditSizey();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonDelsrc();
	afx_msg void OnBnClickedButtonDelanisrc();
	afx_msg void OnCbnSelchangeComboClk();
	afx_msg void OnCbnSelchangeComboCl();
	afx_msg void OnCbnSelchangeComboMesh();
	afx_msg void OnCbnSelchangeComboAct();
	afx_msg void OnCbnSelchangeComboAni();
	afx_msg void OnEnChangeEditLifetime();
	afx_msg void OnBnClickedButtonAddkey();
	afx_msg void OnBnClickedButtonDeletekey();
	afx_msg void OnEnChangeEditA();
	afx_msg void OnEnChangeEditR();
	afx_msg void OnEnChangeEditG();
	afx_msg void OnEnChangeEditB();
	afx_msg void OnLvnBegindragListCombine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonTimeset();
	afx_msg void OnBnClickedButtonMoveup();
	afx_msg void OnBnClickedButtonMovedown();
	afx_msg void OnBnClickedButtonDeleteSfx();
	afx_msg void OnLvnItemActivateListCombine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditObjfunc();
	afx_msg void OnEnChangeEditActfunc();
	afx_msg void OnEnChangeEditAnifunc();
	afx_msg void OnBnClickedCheckZenable();
	afx_msg void OnBnClickedCheckZwrite();
	afx_msg void OnCbnSelchangeComboBlendop();
	afx_msg void OnCbnSelchangeComboDestblend();
	afx_msg void OnCbnSelchangeComboSrcblend();
	afx_msg void OnEnChangeEditLodlevel();
	afx_msg void OnDeltaposSpinLodlevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditCurlodlevel();
	afx_msg void OnDeltaposSpinCurlodlevel(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListCombine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditGloballodlevel();
	afx_msg void OnNMKillfocusListCombine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditListCombine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditListCombine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditSetfps();
	afx_msg void OnLvnItemchangedListCombine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnKeydownListCombine(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonFindsfx();
};
