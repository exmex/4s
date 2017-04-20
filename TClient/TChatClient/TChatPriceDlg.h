#pragma once

class CTChatItem;

// CTChatPriceDlg 대화 상자입니다.

class CTChatPriceDlg : public CDialog
{
	DECLARE_DYNAMIC(CTChatPriceDlg)

public:
	CTChatPriceDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTChatPriceDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_PRICE };

public:
	CTChatItem *m_pTITEM;

	DWORD m_dwRune;
	DWORD m_dwLuna;
	DWORD m_dwCron;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
