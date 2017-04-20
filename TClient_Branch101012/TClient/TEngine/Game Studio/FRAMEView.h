#pragma once
#include "GuideView.h"
#include "FRAMEDoc.h"


// CFRAMEView ∫‰¿‘¥œ¥Ÿ.

class CFRAMEView : public CGuideView
{
	DECLARE_DYNCREATE(CFRAMEView)

protected:
	CFRAMEView();
	virtual ~CFRAMEView();

public:
	CFRAMEDoc* GetDocument();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual CRect GetItemRect( CPoint point);
	virtual CRect GetItemRect(int nCnt);
	virtual CRect GetItemRect();
	virtual void DoMoveItem( CPoint point);
	virtual void AddItem( CItemBase *pItem);

	virtual BYTE CanDrop( CItemBase *pItem);

	virtual GUIDEHIT HitTest( CPoint point);
	

	virtual void DrawContent();
	virtual void HitTestRect(CRect rect);

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	
	void MoveItems(CPoint pt, CPoint ptMouseHit , BYTE bINPUTTYPE);
	virtual BOOL IsSelectedItem(int nSelected);
	virtual void DeleteSelectedITEM(int nSelected);
	virtual void InsertSelectITEM(int nSelect);
	virtual BOOL IsPreRectInDrag(int nSelect);
	virtual void DeletePreRectInITEM(int nSelected);
	virtual void InsertPreRectInITEM(int nSelect);
	virtual CRect GetSelectItemRECT(void);

	virtual void MoveItemByKeyBoard(CPoint pt);
	virtual CPoint GetRectDist(void);
};

inline CFRAMEDoc* CFRAMEView::GetDocument()
   { return (CFRAMEDoc *) m_pDocument;}
