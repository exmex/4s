#pragma once
#include "BarContent.h"
#include "GSEditCtrl.h"
#include "ItemBase.h"
#include "afxwin.h"
#include "afxcmn.h"


// CParticleBar 대화 상자입니다.

class CParticleBar : public CBarContent
{
	DECLARE_DYNAMIC(CParticleBar)

public:
	CParticleBar(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CParticleBar();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_PARTICLE };

private:
	static void CALLBACK OnANIDragLeave( CGSEditCtrl *pEdit);
	static BOOL CanANIDrop( CItemBase *pItem);

	static DROPEFFECT CALLBACK OnANIDragOver(
		CGSEditCtrl *pEdit,
		COleDataObject *pDataObject,
		DWORD dwKeyState,
		CPoint point);

	static BOOL CALLBACK OnANIDrop(
		CGSEditCtrl *pEdit,
		COleDataObject* pDataObject,
		DROPEFFECT dropEffect,
		CPoint point);

public:
	void SetItem( CSFXItem *pItem);

protected:
	void InitSFX();

protected:
	CSFXItem *m_pItem;

protected:
	void SetSANISRC( CAnimationItem *pItem);
	void EnableAllCtrl( BOOL bEnable);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

public:
	CGSEditCtrl m_cSANI;

	CSpinButtonCtrl m_spinEScaleX;
	CSpinButtonCtrl m_spinEScaleY;
	CSpinButtonCtrl m_spinEScaleZ;
	CSpinButtonCtrl m_spinEPosX;
	CSpinButtonCtrl m_spinEPosY;
	CSpinButtonCtrl m_spinEPosZ;
	CSpinButtonCtrl m_spinERotX;
	CSpinButtonCtrl m_spinERotY;
	CSpinButtonCtrl m_spinERotZ;
	CSpinButtonCtrl m_spinETime;

	CSpinButtonCtrl m_spinLifeTime;
	CSpinButtonCtrl m_spinFollow;
	CSpinButtonCtrl m_spinGTick;
	CSpinButtonCtrl m_spinGCnt;
	CSpinButtonCtrl m_spinOutX;
	CSpinButtonCtrl m_spinOutY;
	CSpinButtonCtrl m_spinOutZ;
	CSpinButtonCtrl m_spinInX;
	CSpinButtonCtrl m_spinInY;
	CSpinButtonCtrl m_spinInZ;

	CEdit m_cEScaleX;
	CEdit m_cEScaleY;
	CEdit m_cEScaleZ;
	CEdit m_cEPosX;
	CEdit m_cEPosY;
	CEdit m_cEPosZ;
	CEdit m_cERotX;
	CEdit m_cERotY;
	CEdit m_cERotZ;
	CEdit m_cETime;

	CEdit m_cLifeTime;
	CEdit m_cFValue;
	CEdit m_cGTick;
	CEdit m_cGCnt;
	CEdit m_cOutX;
	CEdit m_cOutY;
	CEdit m_cOutZ;
	CEdit m_cInX;
	CEdit m_cInY;
	CEdit m_cInZ;

	CComboBox m_cSKind;
	CButton m_cDELSANI;
	CButton m_cStrict;
	CButton m_cFollow;
	CButton m_cBasic;

	CString m_strEScaleX;
	CString m_strEScaleY;
	CString m_strEScaleZ;
	CString m_strFollow;
	CString m_strEPosX;
	CString m_strEPosY;
	CString m_strEPosZ;
	CString m_strERotX;
	CString m_strERotY;
	CString m_strERotZ;
	CString m_strOutX;
	CString m_strOutY;
	CString m_strOutZ;
	CString m_strInX;
	CString m_strInY;
	CString m_strInZ;
	CString m_strSANI;

	DWORD m_dwLifeTime;
	DWORD m_dwGTick;
	DWORD m_dwETime;
	DWORD m_dwGCnt;

	BOOL m_bStrict;
	BOOL m_bFollow;

	int m_nSKind;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedButtonSetbasic();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonDelsani();
	afx_msg void OnEnChangeEditMaxx();
	afx_msg void OnEnChangeEditMaxy();
	afx_msg void OnEnChangeEditMaxz();
	afx_msg void OnEnChangeEditMinx();
	afx_msg void OnEnChangeEditMiny();
	afx_msg void OnEnChangeEditMinz();
	afx_msg void OnEnChangeEditPepx();
	afx_msg void OnEnChangeEditPepy();
	afx_msg void OnEnChangeEditPepz();
	afx_msg void OnEnChangeEditPerx();
	afx_msg void OnEnChangeEditPery();
	afx_msg void OnEnChangeEditPerz();
	afx_msg void OnEnChangeEditPesx();
	afx_msg void OnEnChangeEditPesy();
	afx_msg void OnEnChangeEditPesz();
	afx_msg void OnEnChangeEditPet();
	afx_msg void OnEnChangeEditPgc();
	afx_msg void OnEnChangeEditPgt();
	afx_msg void OnEnChangeEditPlt();
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeComboSpraykind();
	afx_msg void OnBnClickedCheckStatic();
	afx_msg void OnBnClickedCheckFollow();
	afx_msg void OnEnChangeEditFollow();
};
