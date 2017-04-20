#pragma once
#include "ProgressDlg.h"
#include "BarContent.h"
#include "GSListCtrl.h"
#include "GSEditCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"

#define REGID_IMG_SIZE 10

class CEXTMAPBar;
typedef struct tagEMBPARAM
{
	CProgressDlg *m_pWnd;
	CEXTMAPBar *m_pHost;

	CString m_strFileName;

	int m_nType;

	BYTE m_bOBJCOLLISION;

	tagEMBPARAM()
	{
		m_pHost = NULL;
		m_pWnd = NULL;

		m_strFileName.Empty();
		m_nType = 0;
	};
} EMBPARAM, *LPEMBPARAM;

typedef vector<LPOINT>	VECLPOINT, *LPVECLPOINT;

// CEXTMAPBar 대화 상자입니다.

class CEXTMAPBar : public CBarContent
{
	DECLARE_DYNAMIC(CEXTMAPBar)

public:
	CEXTMAPBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CEXTMAPBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_EXTMAP };

private:
	static void CALLBACK OnGEODragLeave( CGSListCtrl *pList);
	static BOOL CanGEODrop( CItemBase *pItem);

	static DROPEFFECT CALLBACK OnGEODragOver(
		CGSListCtrl *pList,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static BOOL CALLBACK OnGEODrop(
		CGSListCtrl *pList,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

	static DWORD __BuildSHADOWTEX( DWORD_PTR lParam);
	static DWORD __BuildTILETEX( DWORD_PTR lParam);
	static DWORD __BuildHEIGHT( DWORD_PTR lParam);
	static DWORD __BuildSHADE( DWORD_PTR lParam);
	static DWORD __BuildGEO( DWORD_PTR lParam);
	static DWORD __SaveHeight(DWORD_PTR lParam);		// Map을 높이 데이터를 Height Image(TGA)로 저장
	static DWORD __SaveMinimap(DWORD_PTR lParam);		// Map의 이미지를 미니맵이미지로 저장

protected:
	void EnableAllCtrl( BOOL bEnable);

public:
	void AddGEO( CTextureItem *pItem);

	void BuildSHADOWTEX( LPEMBPARAM pParam);
	void BuildTILETEX( LPEMBPARAM pParam);
	void BuildHEIGHT( LPEMBPARAM pParam);
	void BuildSHADE( LPEMBPARAM pParam);
	void BuildGEO( LPEMBPARAM pParam);
	void SaveHeight(LPEMBPARAM pParam);					// Map을 높이 데이터를 Height Image(TGA)로 저장
	void SaveMinimap(LPEMBPARAM pParam);				// Map의 이미지를 미니맵이미지로 저장

	void SetItem( CEXTMAPItem *pItem);
	void OnSelectUNIT( int nIndex);
	void ResetDETAIL();
	int GetUNIT();

	void OnEditHeight();
	void OnEditTexture();
	void OnEditDetail();
	void OnHeightBrush();
	void OnMakeSmooth();
	void OnMakeFlat();
	void OnEditObject();
	void OnMakeAddFlat();

protected:
	CTColorCtrl m_cColorCtrl;
	CEXTMAPItem *m_pItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CGSListCtrl m_cGEOTEX;
	CGSListCtrl m_cUNIT;
	CListCtrl m_cDETAIL;

	CSpinButtonCtrl m_spinIntensity;
	CSpinButtonCtrl m_spinSpecRange;
	CSpinButtonCtrl m_spinRegionID;
	CSpinButtonCtrl m_spinSpecMax;
	CSpinButtonCtrl m_spinHeight;
	CSpinButtonCtrl m_spinBrushH;
	CSpinButtonCtrl m_spinFlatH;
	CSpinButtonCtrl m_spinAddFlatH;
	CSpinButtonCtrl m_spinSize;

	CColorBox m_cColorBox;
	CStatic m_cControlBox;

	CButton m_cBuildTileTex;
	CButton m_cBuildHeight;
	CButton m_cDisableUNIT;
	CButton m_cBuildGeoTex;
	CButton m_cEditTexture;
	CButton m_cDisableAll;
	CButton m_cEditHeight;
	CButton m_cSaveGeoTex;
	CButton m_cLoadGeoTex;
	CButton m_cBuildShade;
	CButton m_cEditDetail;
	CButton m_cEnableAll;
	CButton m_cOLGeoTex;
	CButton m_cEditOBJ;
	CButton m_cEditSFX;
	CButton m_cEditSND;
	CButton m_cHBrush;
	CButton m_cRegion;
	CButton m_cSmooth;
	CButton m_cFlat;
	CButton m_cHideMap;		// 09-04-03 임진영
	CButton m_cAddFlat;		// 09-03-27 임진영
	CButton m_cSaveHeight;	// 06-02-07 박상연
	CButton m_cSaveMinimap;	// 06-03-22 박상연
	CButton m_cRegionDataTex;	
	CButton m_cEditEnableUNIT;

	CEdit m_cIntensity;
	CEdit m_cSpecRange;
	CEdit m_cRegionID;
	CEdit m_cSpecMax;
	CEdit m_cHeight;
	CEdit m_cBrushH;
	CEdit m_cFlatH;
	CEdit m_cAddFlatH;	// 09-03-30 임진영
	CEdit m_cSize;

	CString m_strIntensity;
	CString m_strSpecRange;
	CString m_strSpecMax;
	CString m_strHeight;
	CString m_strBrushH;
	CString m_strFlatH;
	CString m_strAddFlatH;
	//////////////////////////////////////////////
	// 맵의 Height 값을 Image로 저장할 폴더/파일명
	CString m_strDestFolder;
	CString m_strDestFile;

	CStatic m_cStaticDetailTex;
	CString m_strDetailTexCount;

	CComboBox m_cAttenuation;
	CComboBox m_cShape;
    
	int m_nAttenuation;
	int m_nShape;
	int m_nSize;

	BOOL m_bDisableUNIT;
	BOOL m_bUseSHADER;
	BOOL m_bEditTEX;
	BOOL m_bSmooth;
	BOOL m_bRegion;
	BOOL m_bDetail;
	BOOL m_bHBrush;
	BOOL m_bEditH;
	BOOL m_bFlatH;
	BOOL m_bAddFlatH;		// 09-03-27 임진영
	BOOL m_bHideMap;		// 09-04-03 임진영
	BYTE m_bRegionID;

	void OpenEditDetTexDlg() { OnBnClickedButtonEditDettex(); }

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnColorSelected( WPARAM wParam, LPARAM lParam);
	afx_msg void OnLvnItemchangedListUnit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMClickListUnit(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnLvnItemchangedListGeotex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditHeight();
	afx_msg void OnNMDblclkListGeotex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonShade();
	afx_msg void OnBnClickedButtonGeometry();
	afx_msg void OnBnClickedButtonSaveGeotex();
	afx_msg void OnBnClickedButtonLoadGeotex();
	afx_msg void OnBnClickedButtonOlGeotex();
	afx_msg void OnBnClickedCheckHideMap();
	afx_msg void OnBnClickedCheckEditHeight();
	afx_msg void OnBnClickedCheckEditTexture();
	afx_msg void OnBnClickedCheckFlat();
	afx_msg void OnBnClickedCheckAddFlat();
	afx_msg void OnCbnSelchangeComboAttenuation();
	afx_msg void OnCbnSelchangeComboShape();
	afx_msg void OnBnClickedButtonEditObj();
	afx_msg void OnEnChangeEditFlath();
	afx_msg void OnEnChangeEditAddFlath();
	afx_msg void OnEnChangeEditSize();
	afx_msg void OnBnClickedButtonEnableAll();
	afx_msg void OnBnClickedButtonDisableAll();
	afx_msg void OnBnClickedCheckDisableUnit();
	afx_msg void OnBnClickedButtonBuildHeight();
	afx_msg void OnBnClickedCheckEditRegion();
	afx_msg void OnEnChangeEditRegion();
	afx_msg void OnBnClickedButtonEditSfx();
	afx_msg void OnBnClickedButtonEditSnd();
	afx_msg void OnBnClickedCheckEditDetail();
	afx_msg void OnLvnItemchangedListDetailtex(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCheckHbrush();
	afx_msg void OnEnChangeEditHbrush();
	afx_msg void OnBnClickedCheckSmooth();
	afx_msg void OnBnClickedCheckSpecular();
	afx_msg void OnEnChangeEditIntensity();
	afx_msg void OnEnChangeEditMaxcolor();
	afx_msg void OnEnChangeEditSpecrange();
	afx_msg void OnBnClickedButtonTiletex();
	afx_msg void OnBnClickedButtonShadowtex();
	afx_msg void OnBnClickedButtonSaveHeightmap();
	afx_msg void OnBnClickedButtonSaveMinimap();
	afx_msg void OnBnClickedButtonLoadRegiontex();
	afx_msg void OnBnClickedButtonEditEnableUnit();
	afx_msg void OnBnClickedButtonEditDettex();
	afx_msg void OnBnClickedButtonSaveShadow();
	afx_msg void OnBnClickedButtonLoadShadow();
};
