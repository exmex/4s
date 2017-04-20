#pragma once
class CGSListCtrl;

typedef DROPEFFECT (CALLBACK *FNDRAGDROP)( CGSListCtrl *, COleDataObject *, DWORD, CPoint);
typedef BOOL (CALLBACK *FNONDROP)( CGSListCtrl *, COleDataObject *, DROPEFFECT, CPoint);
typedef void (CALLBACK *FNONDRAGLEAVE)(CGSListCtrl *);


// CGSListCtrl ¿‘¥œ¥Ÿ.

class CGSListCtrl : public CListView
{
public:
	CGSListCtrl();
	virtual ~CGSListCtrl();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CListCtrl *GetListCtrl();

	void SetCurSel( int nIndex);
	void UpdateColumnWidth();
	int GetCurSel();

public:
	FNDRAGDROP m_pDragEnter;
	FNDRAGDROP m_pDragOver;

	FNONDRAGLEAVE m_pLeave;
	FNONDROP m_pDrop;

	COleDropTarget m_DropTarget;
	BOOL m_bUISelect;

protected:
	virtual void PostNcDestroy();

public:
	DECLARE_MESSAGE_MAP()
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnDragLeave();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
};
