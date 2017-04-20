#pragma once
#include "GuideView.h"
#include "COMPDoc.h"


// CCOMPView ∫‰¿‘¥œ¥Ÿ.

class CCOMPView : public CGuideView
{
	DECLARE_DYNCREATE(CCOMPView)

protected:
	CCOMPView();
	virtual ~CCOMPView();

public:
	CCOMPDoc* GetDocument();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CRect GetItemRect( CPoint point);
	virtual CRect GetItemRect(int nCnt);
	virtual CRect GetItemRect();
	virtual CRect GetSelectItemRECT(void);

	virtual void DoMoveItem( CPoint point);
	virtual void AddItem( CItemBase *pItem);

	virtual BYTE CanDrop( CItemBase *pItem);

	virtual GUIDEHIT HitTest( CPoint point);
	
	virtual void DrawContent();
	virtual void HitTestRect(CRect rect);

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL IsSelectedItem(int nSelected);
	virtual void DeleteSelectedITEM(int nSelected);
	virtual void InsertSelectITEM(int nSelect);
	virtual BOOL IsPreRectInDrag(int nSelect);

	virtual void MoveItemByKeyBoard(CPoint pt);
	virtual CPoint GetRectDist(void);
};

inline CCOMPDoc* CCOMPView::GetDocument()
   { return (CCOMPDoc *) m_pDocument;}
