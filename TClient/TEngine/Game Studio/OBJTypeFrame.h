#pragma once


// COBJTypeFrame 프레임입니다.

class COBJTypeFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(COBJTypeFrame)
protected:
	COBJTypeFrame();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~COBJTypeFrame();

public:
	DECLARE_MESSAGE_MAP()
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
};
