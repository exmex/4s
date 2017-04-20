// TCombo.h: interface for the TCombo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCOMBO_H__10FFA74B_5D1A_4DD5_96D2_A939C392A0B5__INCLUDED_)
#define AFX_TCOMBO_H__10FFA74B_5D1A_4DD5_96D2_A939C392A0B5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class TCombo : public TComponent  
{
public:
	TCombo(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~TCombo();

protected:
	TComponent *m_pDrop;
	TComponent *m_pDownList;

	void DropDown();
	void PullUp();

public:
	MAP_LISTITEM* GetCurSelItem();
	MAP_LISTITEM* SetCurSelItem( int nLine );

	int AddString( LPCTSTR strText, int nColumn = 0 );
	int AddItem( LPCTSTR strText, DWORD data, int nColumn = 0 );
	int DelString(int nLine);
	int DelItem( int nLine );

	int GetSel();
	void RemoveAll();
	DWORD GetItemData( int nLine, int nColumn );

	MAP_LISTITEM* GetItem( int nLine );

	BOOL SetItemString( int nLine, int nColumn, LPCTSTR strText );
	BOOL SetItemData( int nLine, int nColumn, DWORD data );

public://Message Handler	
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnLoseFocus(TComponent *pSetFocus);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);

	virtual void OnNotify(DWORD from, WORD msg, LPVOID param);
};

#endif // !defined(AFX_TCOMBO_H__10FFA74B_5D1A_4DD5_96D2_A939C392A0B5__INCLUDED_)
