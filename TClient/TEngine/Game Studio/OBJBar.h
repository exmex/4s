#pragma once
#include "BarContent.h"
#include "GSListCtrl.h"
#include "GSEditCtrl.h"
#include "ItemBase.h"
#include "afxcmn.h"
#include "afxwin.h"


class COBJBar;
typedef vector<COBJBar *>		VECTOROBJBAR;

// COBJBar 대화 상자입니다.

class COBJBar : public CBarContent
{
	DECLARE_DYNAMIC(COBJBar)

public:
	COBJBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~COBJBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_OBJECT};

public:
	static VECTOROBJBAR m_vOBJBAR;
	static COBJItem *m_pDEST;

public:
	static void SetDEST( CItemBase *pItem);
	static void ReleaseOBJBAR();
	static void InitOBJBAR();

private:
	static void CALLBACK OnMESHDragLeave( CGSListCtrl *pList);
	static void CALLBACK OnSFXDragLeave( CGSListCtrl *pList);
	static void CALLBACK OnSNDDragLeave( CGSListCtrl *pList);
	static void CALLBACK OnANIDragLeave( CGSListCtrl *pList);
	static void CALLBACK OnEODragLeave( CGSEditCtrl *pEdit);

	static BOOL CanMESHDrop( COBJBar *pBar, CItemBase *pItem);
	static BOOL CanSFXDrop( COBJBar *pBar, CItemBase *pItem);
	static BOOL CanSNDDrop( COBJBar *pBar, CItemBase *pItem);
	static BOOL CanANIDrop( COBJBar *pBar, CItemBase *pItem);
	static BOOL CanEODrop( COBJBar *pBar, CItemBase *pItem);

	static DROPEFFECT CALLBACK OnMESHDragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static DROPEFFECT CALLBACK OnANIDragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static DROPEFFECT CALLBACK OnSFXDragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static DROPEFFECT CALLBACK OnSNDDragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static DROPEFFECT CALLBACK OnEODragOver(
		CGSEditCtrl *pEdit,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static BOOL CALLBACK OnMESHDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static BOOL CALLBACK OnSFXDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static BOOL CALLBACK OnSNDDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static BOOL CALLBACK OnANIDrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static BOOL CALLBACK OnEODrop(
		CGSEditCtrl *pEdit,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

public:
	void SetItem( COBJItem *pItem);

protected:
	CTColorCtrl m_cColorCtrl;
	CTColorCtrl m_cAmbientControlCtrl;
	COBJItem *m_pItem;

	DWORD m_dwTick;
	BYTE m_bCurTEX;

protected:
	CString GetNewName( DWORD dwFMT);
	int GetPosFromTime( FLOAT fTime);

	FLOAT GetTimeFromPos( int nPos);
	BOOL IsEmptyOBJ();

	void UpdatePos( int nPos);
	void LoadPivot();
	void LoadTEX();

	void SetAction();
	void SetCloth();

	void DeleteMESH( int nIndex);
	void DeleteANI( int nIndex);
	void DeleteCurSFX();
	void DeleteCurSND();

	void InitSNDList( LPMAPSNDINST pSND);
	void InitSFXList( LPMAPSFXINST pSFX);
	void InitSNDList();
	void InitSFXList();

	void InsertSND( CMediaItem *pItem);
	void InsertSFX( CSFXItem *pItem);
	void InsertEO( COBJItem *pItem);
	void InsertMeshItem(
		CMeshItem *pItem,
		int nIndex);

	void InsertAniItem(
		CAnimationItem *pItem,
		int nIndex);

public:
	void EnableAllCtrl( BOOL bEnable);

	void SetActionList();
	void SetClothList();

	void SetEOACTList();
	void SetEOCLKList();

	void SetCurSND();
	void SetCurSFX();
	void SetCurSel(
		DWORD dwCLK,
		DWORD dwCL,
		DWORD dwMESH,
		int nIndex);

	CTachyonObject *GetCurEO();
	COBJItem *GetCurEI();

	LPACTINST GetCurEAI();
	LPCLKINST GetCurECI();

	LPACTINST GetCurAI();
	LPCLKINST GetCurCI();

	LPACTION GetCurEACT();
	LPCLKIND GetCurECLK();
	LPCLOTH GetCurECL();

	LPSNDINST GetCurSND();
	LPSFXINST GetCurSFX();
	LPACTION GetCurACT();
	LPCLKIND GetCurCLK();
	LPCLOTH GetCurCL();
	LPMESH GetCurMESH();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CSpinButtonCtrl m_spinSFXFUNCTION;
	CSpinButtonCtrl m_spinSNDFUNCTION;
	CSpinButtonCtrl m_spinSFXOBJFUNC;
	CSpinButtonCtrl m_spinSFXACTFUNC;
	CSpinButtonCtrl m_spinSFXANIFUNC;
	CSpinButtonCtrl m_spinSNDOBJFUNC;
	CSpinButtonCtrl m_spinSNDACTFUNC;
	CSpinButtonCtrl m_spinSNDANIFUNC;
	CSpinButtonCtrl m_spinMeshOrder;
	CSpinButtonCtrl m_spinIntensity;
	CSpinButtonCtrl m_spinMinAlpha;
	CSpinButtonCtrl m_spinEStartAT;
	CSpinButtonCtrl m_spinSStartAT;
	CSpinButtonCtrl m_spinRValue;
	CSpinButtonCtrl m_spinGValue;
	CSpinButtonCtrl m_spinBValue;
	CSpinButtonCtrl m_spinAlpha;
	CSpinButtonCtrl m_spinEPosX;
	CSpinButtonCtrl m_spinEPosY;
	CSpinButtonCtrl m_spinEPosZ;
	CSpinButtonCtrl m_spinERotX;
	CSpinButtonCtrl m_spinERotY;
	CSpinButtonCtrl m_spinERotZ;
	CSpinButtonCtrl m_spinBias;
	CSpinButtonCtrl m_spinRValue2;
	CSpinButtonCtrl m_spinGValue2;
	CSpinButtonCtrl m_spinBValue2;
	CSpinButtonCtrl m_spinAlpha2;

	CColorBox m_cColorBox;
	CStatic m_cControlBox;

	CColorBox m_cAmbientColorBox;
	CStatic m_cAmbientControlBox;

	CGSListCtrl m_cSOUND;
	CGSListCtrl m_cEOCLK;
	CGSListCtrl m_cMESH;
	CGSListCtrl m_cACT;
	CGSListCtrl m_cANI;
	CGSListCtrl m_cCLK;
	CGSListCtrl m_cSFX;
	CGSListCtrl m_cCL;
	CGSListCtrl m_cMI;
	CGSListCtrl m_cHEBA_LIST;
	CGSEditCtrl m_cEO;

	CComboBox m_cDESTBlend;
	CComboBox m_cSRCBlend;
	CComboBox m_cBlendOP;

	CComboBox m_cEPivotID;
	CComboBox m_cEOMESH;
	CComboBox m_cEOACT;
	CComboBox m_cEOANI;
	CComboBox m_cPIVOT;
	CComboBox m_cEOCL;
	CComboBox m_cOP;
	CComboBox m_cHEBA_ACT;
	CComboBox m_cUseDirLightOp;

	CButton m_cShowSelected;
	CButton m_cSETESTART;
	CButton m_cSETSSTART;
	CButton m_cEditPivot;
	CButton m_cCOPYSND;
	CButton m_cZEnable;
	CButton m_cZWrite;
	CButton m_cOBJSFX;
	CButton m_cACTSFX;
	CButton m_cANISFX;
	CButton m_cOBJSND;
	CButton m_cACTSND;
	CButton m_cANISND;
	CButton m_cSStop;
	CButton m_cEStop;
	CButton m_cDelEO;
	CButton m_c2side;
	CButton m_cSTG1;
	CButton m_cSTG2;
	CButton m_cTEX;
	CButton m_cCLR;
	CButton m_cENV;
	CButton m_cHebaAdd;
	CButton m_cHebaDelete;
	CButton m_cUseDirLight;

	CEdit m_cSFXFUNCTION;
	CEdit m_cSNDFUNCTION;
	CEdit m_cSFXOBJFUNC;
	CEdit m_cSFXACTFUNC;
	CEdit m_cSFXANIFUNC;
	CEdit m_cSNDOBJFUNC;
	CEdit m_cSNDACTFUNC;
	CEdit m_cSNDANIFUNC;
	CEdit m_cIntensity;
	CEdit m_cMinAlpha;
	CEdit m_cSStartAT;
	CEdit m_cEStartAT;
	CEdit m_cPivotID;
	CEdit m_cColorID;
	CEdit m_cRValue;
	CEdit m_cGValue;
	CEdit m_cBValue;
	CEdit m_cAlpha;
	CEdit m_cORDER;
	CEdit m_cEPosX;
	CEdit m_cEPosY;
	CEdit m_cEPosZ;
	CEdit m_cERotX;
	CEdit m_cERotY;
	CEdit m_cERotZ;
	CEdit m_cBias;
	CEdit m_cRValue2;
	CEdit m_cGValue2;
	CEdit m_cBValue2;
	CEdit m_cAlpha2;

	CString m_strIntensity;
	CString m_strColorID;
	CString m_strPivotID;
	CString m_strEPosX;
	CString m_strEPosY;
	CString m_strEPosZ;
	CString m_strERotX;
	CString m_strERotY;
	CString m_strERotZ;

	DWORD m_dwSFXFUNCTION;
	DWORD m_dwSNDFUNCTION;
	DWORD m_dwSFXOBJFUNC;
	DWORD m_dwSFXACTFUNC;
	DWORD m_dwSFXANIFUNC;
	DWORD m_dwSNDOBJFUNC;
	DWORD m_dwSNDACTFUNC;
	DWORD m_dwSNDANIFUNC;
	DWORD m_dwEStartAT;
	DWORD m_dwSStartAT;
	DWORD m_dwRValue;
	DWORD m_dwGValue;
	DWORD m_dwBValue;
	DWORD m_dwAlpha;
	DWORD m_dwRValue_Amb;
	DWORD m_dwGValue_Amb;
	DWORD m_dwBValue_Amb;
	DWORD m_dwAlpha_Amb;
	DWORD m_dwORDER;

	BOOL m_bShowSelected;
	BOOL m_bEditPivot;
	BOOL m_bZEnable;
	BOOL m_bZWrite;
	BOOL m_b2side;
	BOOL m_bEStop;
	BOOL m_bSStop;

	BYTE m_bMinAlpha;
	BYTE m_bBias;

	int m_nEPivotID;
	int m_nSFXKind;
	int m_nSNDKind;
	int m_nEOMESH;
	int m_nEOACT;
	int m_nEOANI;
	int m_nPIVOT;
	int m_nEOCL;
	int m_nSTG;
	int m_nTEX;
	int m_nOP;
	int m_nHEBA_ACT;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnColorSelected( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlProc( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPlay( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlStop( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnPlayCtrlPos( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMessageBox( WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnOBJMessage( WPARAM wParam, LPARAM lParam);
	afx_msg void OnLvnItemchangedListAction(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListClothkind(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListCloth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListClothdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedListMeshorder(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListAction(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListMeshorder(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListCloth(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListClothdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListClothkind(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListClothdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRDblclkListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioStage();
	afx_msg void OnBnClickedRadioMitex();
	afx_msg void OnEnChangeEditMeshorder();
	afx_msg void OnCbnSelchangeComboSop();
	afx_msg void OnBnClickedCheckShowSelected();
	afx_msg void OnBnClickedCheckZenable();
	afx_msg void OnBnClickedCheckZwrite();
	afx_msg void OnBnClickedCheck2side();
	afx_msg void OnEnChangeEditColor();
	afx_msg void OnEnChangeEditColor2();
	afx_msg void OnEnChangeEditColorid();
	afx_msg void OnEnSetfocusEditColorid();
	afx_msg void OnEnKillfocusEditColorid();
	afx_msg void OnEnKillfocusEditPivot();
	afx_msg void OnEnSetfocusEditPivot();
	afx_msg void OnCbnSelchangeComboPivot();
	afx_msg void OnBnClickedCheckEditpivot();
	afx_msg void OnLvnItemchangedListEck(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboEc();
	afx_msg void OnCbnSelchangeComboEcd();
	afx_msg void OnCbnSelchangeComboEa();
	afx_msg void OnCbnSelchangeComboEani();
	afx_msg void OnNMClickListEck(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonDeleo();
	afx_msg void OnLvnEndlabeleditListClothdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnEndlabeleditListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditListClothdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnBeginlabeleditListAnimation(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioSFX();
	afx_msg void OnLvnItemchangedListSfx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListSfx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListSfx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRdblclkListSfx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSfx(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditPosX();
	afx_msg void OnEnChangeEditPosY();
	afx_msg void OnEnChangeEditPosZ();
	afx_msg void OnEnChangeEditRotX();
	afx_msg void OnEnChangeEditRotY();
	afx_msg void OnEnChangeEditRotZ();
	afx_msg void OnEnChangeEditBias();
	afx_msg void OnEnChangeEditStart();
	afx_msg void OnBnClickedCheckDelonstop();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeComboOep();
	afx_msg void OnLvnItemchangedListSoundid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListSoundid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRclickListSoundid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRdblclkListSoundid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMDblclkListSoundid(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedRadioSND();
	afx_msg void OnEnChangeEditSstart();
	afx_msg void OnEnChangeEditFunction();
	afx_msg void OnBnClickedCheckSdelonstop();
	afx_msg void OnEnChangeEditObjfunc();
	afx_msg void OnEnChangeEditActfunc();
	afx_msg void OnEnChangeEditAnifunc();
	afx_msg void OnBnClickedButtonSetestart();
	afx_msg void OnBnClickedButtonSetsstart();
	afx_msg void OnEnChangeEditPivot();
	afx_msg void OnEnChangeEditMinalpha();
	afx_msg void OnEnChangeEditIntensity();
	afx_msg void OnEnChangeEditSfxfunction();
	afx_msg void OnEnChangeEditObjsfxfunc();
	afx_msg void OnEnChangeEditActsfxfunc();
	afx_msg void OnEnChangeEditAnisfxfunc();
	afx_msg void OnCbnSelchangeComboBlendop();
	afx_msg void OnCbnSelchangeComboDestblend();
	afx_msg void OnCbnSelchangeComboSrcblend();
	afx_msg void OnBnClickedCheckBatchRegSndfunc();
	afx_msg void OnBnClickedCopySnd();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedDeleteClothdata();
	afx_msg void OnLvnKeydownListClothdata(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckDeleteani();
	afx_msg void OnBnClickedCheckDeleffect();
	afx_msg void OnBnClickedCheckDelsound();
	afx_msg void OnBnClickedButtonSfxup();
	afx_msg void OnBnClickedButtonSfxdown();
	afx_msg void OnBnClickedButtonHebaDelete();
	afx_msg void OnBnClickedButtonHebaAdd();
//	afx_msg void OnNMThemeChangedComboHebaAct(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedFindObjtexture();
	afx_msg void OnBnClickedCheckUsedirlight();
	afx_msg void OnCbnSelchangeComboUsedirlightop();
};
