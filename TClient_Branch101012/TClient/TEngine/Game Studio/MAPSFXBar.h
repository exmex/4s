#pragma once
#include "BarContent.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


class CMAPSFXBar;
typedef vector<CMAPSFXBar *>		VECTORMAPSFXBAR;


// CMAPSFXBar 대화 상자입니다.

class CMAPSFXBar : public CBarContent
{
	DECLARE_DYNAMIC(CMAPSFXBar)

public:
	CMAPSFXBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CMAPSFXBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MAPSFX };

public:
	static VECTORMAPSFXBAR m_vMAPSFXBAR;
	static CSFXItem *m_pSFX;

	static LPDIRECT3DTEXTURE9 m_pTEX;
	static CTachyonMesh m_MESH;

protected:
	void EnableAllCtrl( BOOL bEnable);

public:
	static void RenderSFXBOX(
		D3DXVECTOR3& vPOS,
		D3DXVECTOR3& vROT);

	static void SetSFX( CItemBase *pItem);
	static void ReleaseSFXBAR();
	static void InitSFXBAR();

public:
	void SetItem( CMAPItem *pItem);
	void UpdateItem();
	void OnBack();

protected:
	CMAPItem *m_pItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CSpinButtonCtrl m_spinPosX;
	CSpinButtonCtrl m_spinPosY;
	CSpinButtonCtrl m_spinPosZ;
	CSpinButtonCtrl m_spinRotX;
	CSpinButtonCtrl m_spinRotY;
	CSpinButtonCtrl m_spinRotZ;
	CSpinButtonCtrl m_spinBias;

	CButton m_cInsertMode;
	CButton m_cEditMode;
	CButton m_cDelete;
	CStatic m_cName;

	CEdit m_cPosX;
	CEdit m_cPosY;
	CEdit m_cPosZ;
	CEdit m_cRotX;
	CEdit m_cRotY;
	CEdit m_cRotZ;
	CEdit m_cBias;

	CString m_strName;
	CString m_strPosX;
	CString m_strPosY;
	CString m_strPosZ;
	CString m_strRotX;
	CString m_strRotY;
	CString m_strRotZ;

	BYTE m_bBias;
	int m_nMode;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg LRESULT OnMessageBox( WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonBack();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedRadioEdit();
	afx_msg void OnBnClickedRadioInsert();
	afx_msg void OnEnChangeEditData();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonDelete();
	afx_msg void OnEnChangeEditBias();
};
