#pragma once
class CGSEditCtrl;

typedef DROPEFFECT (CALLBACK *FNEDITDRAGDROP)( CGSEditCtrl *, COleDataObject *, DWORD, CPoint);
typedef BOOL (CALLBACK *FNONEDITDROP)( CGSEditCtrl *, COleDataObject *, DROPEFFECT, CPoint);
typedef void (CALLBACK *FNONEDITDRAGLEAVE)(CGSEditCtrl *);


// CGSEditCtrl ∫‰¿‘¥œ¥Ÿ.

class CGSEditCtrl : public CEditView
{
public:
	CGSEditCtrl();
	virtual ~CGSEditCtrl();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CEdit *GetEditCtrl();

public:
	FNEDITDRAGDROP m_pDragEnter;
	FNEDITDRAGDROP m_pDragOver;

	FNONEDITDRAGLEAVE m_pLeave;
	FNONEDITDROP m_pDrop;

	COleDropTarget m_DropTarget;

protected:
	virtual void PostNcDestroy();

public:
	DECLARE_MESSAGE_MAP()
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnDragLeave();
	afx_msg int OnMouseActivate( CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEditChange();
};
