#pragma once


// CGuildChange 대화 상자입니다.

class CGuildChange : public CDialog
{
	DECLARE_DYNAMIC(CGuildChange)

public:
	CGuildChange(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CGuildChange();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_GUILDCHANGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
};
