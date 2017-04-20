#pragma once


// CGuideFrame 프레임입니다.

class CGuideFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CGuideFrame)
protected:
	CGuideFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CGuideFrame();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
