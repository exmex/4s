#pragma once


// CLocalInit 대화 상자입니다.

class CLocalInit : public CDialog
{
	DECLARE_DYNAMIC(CLocalInit)

public:
	CLocalInit(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CLocalInit();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_LOCALINIT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
