#pragma once
#include "afxwin.h"
#include "afxcmn.h"

#pragma once


class CItemPreview : public CStatic
{
	DECLARE_MESSAGE_MAP()

public :

	CItemBase* m_pItem;
	int m_nCamDist;
	int m_nCamRotateX;
	int m_nColorR;
	int m_nColorG;
	int m_nColorB;

public :

	CItemPreview();
	virtual ~CItemPreview();

	BOOL DrawOBJ( COBJItem* _pItem );
	BOOL RenderItem( COBJItem* _pItem );
	void SetItem( CItemBase* _pItem );

public :
	afx_msg void OnPaint();
};

// CItemScreenShot 대화 상자입니다.

class CItemScreenShot : public CDialog
{
	DECLARE_DYNAMIC(CItemScreenShot)

public :

	CItemPreview m_ctrlPreview;

private :

	void SearchTree( CTreeCtrl& TCtrl, HTREEITEM hItem, CString strPath );
	BOOL Save();
	BOOL SaveModeAll( CTreeCtrl& TCtrl );
	BOOL SaveModeSelectedFolder( CTreeCtrl& TCtrl );
	BOOL SaveModeSelectedItem( CTreeCtrl& TCtrl );
	CString OpenSaveDirectory();
	BOOL ScreenShot( COBJItem* pItem, CString strPath );
	void ParentDirectory( CString& strPath );

public:
	CItemScreenShot(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CItemScreenShot();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_ITEMSCREENSHOT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	virtual void PostNcDestroy();
	virtual BOOL OnInitDialog();
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	CComboBox m_ctrlItemType;
	CComboBox m_ctrlSaveMode;
	CComboBox m_ctrlTextureType;
	CComboBox m_ctrlTextureSize;
	
	CSpinButtonCtrl m_spinCamDist;
	CSpinButtonCtrl m_spinCamRotateX;
	CSpinButtonCtrl m_spinColorR;
	CSpinButtonCtrl m_spinColorG;
	CSpinButtonCtrl m_spinColorB;

	CProgressCtrl m_ctrlProgress;

private:
	CString m_strPath;
	int m_nItemType;
	int m_nSaveMode;
	int m_nTextureType;
	int m_nTextureSize;
	bool m_bInitDialog;

public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnCbnSelchangeComboScItemtype();
	afx_msg void OnCbnSelchangeComboScSavemode();
	afx_msg void OnCbnSelchangeComboScTexturetype();
	afx_msg void OnCbnSelchangeComboScTexturesize();
	afx_msg void OnEnChangeEditScColorr();
	afx_msg void OnEnChangeEditScColorg();
	afx_msg void OnEnChangeEditScColorb();
	afx_msg void OnEnChangeEditScCamdist();
	afx_msg void OnEnChangeEditScCamrotx();
};
