// TList.h: interface for the TList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TLIST_H__994FECA7_8B89_4D62_8AD4_9F5AAC637818__INCLUDED_)
#define AFX_TLIST_H__994FECA7_8B89_4D62_8AD4_9F5AAC637818__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "TListItem.h"

typedef vector< TCOMP_ARRAY* >	VECTOR_TCOMP_ARRAY;

class TList : public TComponent  
{
public:
	TList(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~TList();

	TComponent*			m_pScroll;
	int					m_nWheelMoveValue;

public:
	int					m_nTop;
	int					m_nCurSel;
	int					m_nItemPerPage;
	int					m_nColumnCount;
	BOOL				m_blUserColor;

	VECTOR_MAP_LISTITEM	m_items;
	VECTOR_TCOMP_ARRAY	m_itemholders;
	BOOL				m_bNeedUpdateRender;

	void InitItemHolder( unsigned int item, int nColumnCount, LP_FRAMEDESC pDesc );
	void RemoveItemHolder();
	void UpdateScrollPosition();
	void SelectUp(int nLine);
	void SelectDown(int nLine);

public:
	virtual void SetTextClr( DWORD clrText);
	virtual int GetTextExtent();

	virtual void SetTextClr(
		DWORD dwColor,
		DWORD dwLineColor,
		BYTE bOutLine);

	int GetSel();
	int GetTop();
	int GetItemPerPage();
	void RemoveAll();
	int GetItemCount();
	CSize GetItemSize( int nColumn);
	void GetItemSize( LPSIZE lpSize );
	MAP_LISTITEM* GetCurSelItem();
	DWORD GetItemData( int nLine, int nColumn );
	MAP_LISTITEM* GetItem( int nLine );
	INT GetCurSelIndex() { return m_nCurSel; }

	TListItem* GetHitItem(const CPoint& pt, INT* outColumnIndex = NULL);
	DWORD GetHitItemData(const CPoint& pt);

	BYTE HitItemArea( CPoint point);

	int AddString( LPCTSTR strText, int nColumn = 0 );
	int AddInt( int nText, int nColumn = 0 );
	int AddItem( LPCTSTR strText, DWORD data, int nColumn = 0 );
	int DelItem( int nLine );
	int DelString( int nLine );

	BOOL SetItemString( int nLine, int nColumn, LPCTSTR strText );
	BOOL SetItemInt( int nLine, int nColumn, int nText );
	BOOL SetItemData( int nLine, int nColumn, DWORD data );
	BOOL SetImageIndex( int nLine, int nColumn, BOOL blIsImageList, int nImageIndex );
	BOOL SetItemDataAllColumn( int nLine, DWORD data );
	CString GetItemString( int nLine, int nColumn );

	void ApplyUserColor( BOOL blUserColor );
	void SetUserColor( int nLine, int nColumn, DWORD dwTextColor );
	void SetUseShadowFont(BOOL bUse);

	MAP_LISTITEM* SetCurSelItem( int nLine );
	MAP_LISTITEM* SetTopSelItem( int nLine );

	void AddTextSetting(INT iRow, INT iColunm, INT iStart, INT iEnd, DWORD dwColor, DWORD dwInfo);
	void SetItemGM(int nCol, DWORD dwEvent, DWORD dwGM);

	TComponent* FindShrinkInChilds( POINT pt );

	void SetColumnAlign( int nCol, BYTE bAlign);
	void SetTop( int nLine );
	int GetColWidth(int nCol);

	void ClipAndInsertString( CString& strTEXT );

	INT GetMaxItemHeight();
	void SetHeight2( INT nHeight );
	void UpdateList();
    void LastItemToFront();
	void ShowScroll( bool bVisible=true );

public://Message Handler	
	virtual void OnLButtonDblClk( UINT nFlags, CPoint pt );
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void OnLButtonUp( UINT nFlags, CPoint pt );
	virtual void OnMouseMove( UINT nFlags, CPoint pt );
	virtual void OnRButtonDown( UINT nFlags, CPoint pt );
	virtual void OnRButtonUp( UINT nFlags, CPoint pt );
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual HRESULT Render( DWORD dwTickCount );
	virtual void OnNotify( DWORD from, WORD msg, LPVOID param );
	virtual void OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags );
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
};

#endif // !defined(AFX_TLIST_H__994FECA7_8B89_4D62_8AD4_9F5AAC637818__INCLUDED_)
