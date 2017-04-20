#pragma once
#include "afxwin.h"
#include "TGiftSet.h"


// CTGiftItem 대화 상자입니다.

class CTGiftItem : public CDialog
{
	DECLARE_DYNAMIC(CTGiftItem)

public:
	CTGiftItem(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTGiftItem();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GIFTTOOL };

	void	Init();
	void	ShowSetting(BYTE bShow);
	void	ReturnMsg(BYTE bRet);
	BYTE	GetWorld(BYTE& p_bOut);
	LPMAPCMGIFT	GetGiftItemMap();
	void	CreateGiftSet();

public:
	CComboBox m_cbWorld;
	CTGiftSet* m_pDlgGiftSet;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGiftSet();
	afx_msg void OnBnClickedOk();
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
