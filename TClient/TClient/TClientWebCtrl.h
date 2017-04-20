#pragma once


// CTClientWebCtrl

class CTClientWebCtrl : public CWnd
{
	DECLARE_DYNAMIC(CTClientWebCtrl)

public:
	BYTE Navigate(
		CString strURL,
		int nLeft,
		int nTop,
		int nWidth,
		int nHeight,
		BYTE bToolBar,
		BYTE bMenuBar,
		BYTE bStatusBar,
		BYTE bResizable,
		BYTE bMinimizeHost,
		HWND hPOS);

public:
	IWebBrowser2 *m_pWebCtrl;
	CWnd *m_pActHost;

public:
	CTClientWebCtrl();
	virtual ~CTClientWebCtrl();
	virtual BOOL Create();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg void OnDestroy();
};
