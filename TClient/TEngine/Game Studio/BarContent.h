#pragma once

#define WM_COLOR_SELECTED		WM_USER + 1
#define WM_PLAYCTRL_PROC		WM_USER + 2
#define WM_PLAYCTRL_PLAY		WM_USER + 3
#define WM_PLAYCTRL_STOP		WM_USER + 4
#define WM_PLAYCTRL_POS			WM_USER + 5

#define WM_MARKER_MOVE			WM_USER + 6
#define WM_MARKER_COPY			WM_USER + 7
#define WM_MEDIA_MSG			WM_USER + 8

#define WM_LISTCTRL_UPDATE		WM_USER + 9
#define WM_MESSAGEBOX			WM_USER + 10


typedef struct tagGSLISTITEM
{
	CWnd *m_pHost;
	int m_nIndex;

	tagGSLISTITEM()
	{
		m_pHost = NULL;
		m_nIndex = -1;
	};
} GSLISTITEM, *LPGSLISTITEM;


class CColorBox : public CStatic
{
public:
	void SetColor( COLORREF color);

public:
	COLORREF m_Color;

public:
	CColorBox();
	virtual ~CColorBox();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnEraseBkgnd( CDC* pDC);
	afx_msg void OnPaint();
};


// CBarContent 대화 상자입니다.

class CBarContent : public CDialog
{
	DECLARE_DYNAMIC(CBarContent)

public:
	CBarContent( UINT nTemplate, CWnd* pParent = NULL);
	virtual ~CBarContent();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
protected:
	virtual void OnOK();
};
