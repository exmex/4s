#pragma once
#include "MAPView.h"
#include "BSPMAPDoc.h"


// CBSPMAPView 뷰입니다.

class CBSPMAPView : public CMAPView
{
	DECLARE_DYNCREATE(CBSPMAPView)

protected:
	CBSPMAPView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CBSPMAPView();

public:
	CBSPMAPDoc* GetDocument();

public:
	MAPTBSPNODE m_mapPVS;
	MAPTEXVB m_mapLIGHT;
	MAPMESHVB m_mapVB;

	DWORD m_dwNodeCNT;
	DWORD m_dwNodeID;

protected:
	COleDropTarget m_DropTarget;
	int m_nIndex;

protected:
	void InsertTexture( CTextureItem *pItem);
	void RemoveTexture( int nIndex);

protected:
	virtual BYTE GetCursorPos( LPD3DXVECTOR3 pPOS);
	virtual void DrawContent();

	BYTE CanDrop( CItemBase *pItem);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual DROPEFFECT OnDragEnter(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual DROPEFFECT OnDragOver(COleDataObject* pDataObject, DWORD dwKeyState, CPoint point);
	virtual BOOL OnDrop(COleDataObject* pDataObject, DROPEFFECT dropEffect, CPoint point);
	virtual void OnDragLeave();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnHotketEditObject();
};

inline CBSPMAPDoc* CBSPMAPView::GetDocument()
   { return (CBSPMAPDoc *) m_pDocument;}
