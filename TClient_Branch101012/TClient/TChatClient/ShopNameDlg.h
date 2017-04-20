#pragma once


// CShopNameDlg 대화 상자입니다.

class CShopNameDlg : public CDialog
{
	DECLARE_DYNAMIC(CShopNameDlg)

public:
	CShopNameDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CShopNameDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_SHOP_NAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CString m_strNAME;
	virtual BOOL OnInitDialog();
protected:
	virtual void OnOK();
};
