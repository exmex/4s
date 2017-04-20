#pragma once


// CGSToolTipWnd

class CGSToolTipWnd : public CWnd
{
	DECLARE_DYNAMIC(CGSToolTipWnd)

public:
	CGSToolTipWnd();
	virtual ~CGSToolTipWnd();

public:
	CString m_strText;
	CRect m_rect;

public:
	void SetText( CString strText);
	void SetPos( CPoint point);
	void Darw( CDC *pDC);

public:
	virtual BOOL Create( CWnd *pParent);

public:
	DECLARE_MESSAGE_MAP()
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
};
