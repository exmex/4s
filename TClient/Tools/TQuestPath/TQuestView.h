#pragma once


// CTQuestView 뷰입니다.

class CTQuestView : public CTreeView
{
	DECLARE_DYNCREATE(CTQuestView)

public:
	CTQuestPathDoc* GetDocument() const;

public:
	CImageList m_listTreeImage;

public:
	virtual BOOL Create(
		LPCTSTR lpszClassName,
		LPCTSTR lpszWindowName,
		DWORD dwStyle,
		const RECT& rect,
		CWnd* pParentWnd,
		UINT nID,
		CCreateContext* pContext = NULL);

protected:
	CTQuestView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTQuestView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
	afx_msg void OnTvnDeleteitem(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // TQuestView.cpp의 디버그 버전
inline CTQuestPathDoc* CTQuestView::GetDocument() const
   { return reinterpret_cast<CTQuestPathDoc*>(m_pDocument); }
#endif
