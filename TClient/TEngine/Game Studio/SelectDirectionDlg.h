#pragma once
#include "afxwin.h"
#include "EXTMAPDoc.h"

// CSelectDirection 대화 상자입니다.

enum 
{
	_FDA_1 = 0,		// 11.25f 
	_FDA_2,			// 22.5f
	_FDA_3,			// 33.75f
	_FDA_4,			// 45.0f
	_FDA_5,			// 56.25f
	_FDA_6,			// 67.5f
	_FDA_7,			// 75.75f
	_FDA_8,			// 90.0f
	_FDA_9,			// 101.25f
	_FDA_10,		// 112.25f
	_FDA_11,		// 123.75f
	_FDA_12,		// 135.0f
	_FDA_13,		// 146.25f
	_FDA_14,		// 157.5f
	_FDA_15,		// 168.75f
	_FDA_16,		// 180.0f
	_FDA_17,		// 191.25f
	_FDA_18,		// 202.5f
	_FDA_19,		// 213.75f
	_FDA_20,		// 225.0f
	_FDA_21,		// 236.25f
	_FDA_22,		// 247.5f
	_FDA_23,		// 258.75f
	_FDA_24,		// 270.0f
	_FDA_25,		// 281.25f
	_FDA_26,		// 292.5f
	_FDA_27,		// 303.75f
	_FDA_28,		// 315.0f
	_FDA_29,		// 326.25f
	_FDA_30,		// 337.5f
	_FDA_31,		// 348.75f
	_FDA_32,		// 360.0f
	_FAR_IMG_CNT
};

#define BASIC_FAR_DIR_ANG 360.0f / _FAR_IMG_CNT

class CSelectDirectionDlg : public CDialog
{
	DECLARE_DYNAMIC(CSelectDirectionDlg)

public:
	CSelectDirectionDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CSelectDirectionDlg(CEXTMAPDoc* pDoc, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSelectDirectionDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_FARIMG_DIRECTION };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
protected:
	CEXTMAPDoc *m_pDoc;
	DECLARE_MESSAGE_MAP()

public:
	BOOL m_bImgDirection[_FAR_IMG_CNT];
	FARIMGPT m_pFARIMG[_FAR_IMG_CNT];
	FLOAT m_fAngle[_FAR_IMG_CNT];

public:
	void OnChangeCheckState(BYTE nANG);
	void ChangeFixDirection(void);

public:
	afx_msg void OnBnClickedCheckAng1();
	afx_msg void OnBnClickedCheckAng2();
	afx_msg void OnBnClickedCheckAng3();
	afx_msg void OnBnClickedCheckAng4();
	afx_msg void OnBnClickedCheckAng5();
	afx_msg void OnBnClickedCheckAng6();
	afx_msg void OnBnClickedCheckAng7();
	afx_msg void OnBnClickedCheckAng8();
	afx_msg void OnBnClickedCheckAng9();
	afx_msg void OnBnClickedCheckAng10();
	afx_msg void OnBnClickedCheckAng11();
	afx_msg void OnBnClickedCheckAng12();
	afx_msg void OnBnClickedCheckAng13();
	afx_msg void OnBnClickedCheckAng14();
	afx_msg void OnBnClickedCheckAng15();
	afx_msg void OnBnClickedCheckAng16();
	afx_msg void OnBnClickedCheckAng17();
	afx_msg void OnBnClickedCheckAng18();
	afx_msg void OnBnClickedCheckAng19();
	afx_msg void OnBnClickedCheckAng20();
	afx_msg void OnBnClickedCheckAng21();
	afx_msg void OnBnClickedCheckAng22();
	afx_msg void OnBnClickedCheckAng23();
	afx_msg void OnBnClickedCheckAng24();
	afx_msg void OnBnClickedCheckAng25();
	afx_msg void OnBnClickedCheckAng26();
	afx_msg void OnBnClickedCheckAng27();
	afx_msg void OnBnClickedCheckAng28();
	afx_msg void OnBnClickedCheckAng29();
	afx_msg void OnBnClickedCheckAng30();
	afx_msg void OnBnClickedCheckAng31();
	afx_msg void OnBnClickedCheckAng32();
	afx_msg void OnBnClickedMakeFarImage();
	afx_msg void OnBnClickedRadioDirFour();
	afx_msg void OnBnClickedRadioDirEight();
	afx_msg void OnBnClickedRadioDirSixteen();
	afx_msg void OnBnClickedRadioThirtytwo();
	int m_nDir;
	afx_msg void OnBnClickedRadioDirFourx();
};
