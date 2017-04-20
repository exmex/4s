// TMultiLineEdit.h: interface for the TMultiLineEdit class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMULTILINEEDIT_H__B581C2D6_C499_4588_A482_AFAFDB23AEDC__INCLUDED_)
#define AFX_TMULTILINEEDIT_H__B581C2D6_C499_4588_A482_AFAFDB23AEDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TEdit.h"

class TMultiLineEdit : public TEdit
{
public:
	struct LineInfo
	{
		INT nLineIdx;
		INT nStart;
		INT nEnd;
		
		CPoint	  ptBase;
		VECTORINT vCharSize;
	};
	typedef std::vector<LineInfo>	LineInfoArray;

public:
	TMultiLineEdit(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~TMultiLineEdit();

protected:
	TScroll*	m_pScroll;
	int			m_nFontHeight;
	int			m_nLinePerPage;
	
	int			m_nPrvCaret;

	int			m_nScrollPos;
	BOOL		m_bShowCaret;
	INT			m_nDefVMargine;

	int			m_nCtLine;
	int			m_nCtPosInLine;
	
	BOOL		m_bSetupCUD;
	int			m_nCUDLinePos;

	LineInfoArray m_vLineInfos;

public:
	int			m_nWheelMoveValue;

public:
    void InsertReturnChar();
	void MoveScrollToCaret(BOOL bTop);
	void MoveScrollToCaretAuto();
	
	INT GetScrollPos();
	INT GetScrollRng();

	void Update();

public:
	virtual HRESULT DrawText();

	virtual void InsertString(const CString& strText);
	virtual void InsertString(const CString& strText, DWORD dwColor, DWORD dwID);
	virtual void PushFrontString(const CString& strText);

	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnMouseMove(UINT nFlags, CPoint pt);
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnChar(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnImeComposition(HWND hWnd, WPARAM wParam, LPARAM lParam);
	virtual HRESULT TextOut(const TextOutputData& ptd);

protected:
	virtual void SetCaret(INT nLine, INT nPos);
	virtual void UpCaret();
	virtual void DownCaret();
	virtual void LeftCaret(BOOL bDouble);
	virtual void RightCaret(BOOL bDouble);
	virtual void RightCaretCount(UINT uCount);
	virtual void MoveCaretLineFront();
	virtual void MoveCaretLineBack();
	
	CSize GetTextArea();

	BOOL SplitTextByCrLf(const CString& strTEXT, INT& nPos, CString& outLINE);
	void SplitTextByLine(const CString& strTEXT, VECTORSTRING& outLINESTR, LineInfoArray& outLINEINF);
	void RecalcScrollRange(const VECTORSTRING& vLINESTR);
	void BuildTextOutputData(const VECTORSTRING& vLINESTR, LineInfoArray& outLINEINF);
	void LocateCaret(INT nCaret);

#ifdef DEBUG
	void _LOG_( LPCSTR func, LPCSTR file, LONG line );
#else
	void _LOG_( LPCSTR func, LPCSTR file, LONG line ) {}
#endif
};

#endif // !defined(AFX_TMULTILINEEDIT_H__B581C2D6_C499_4588_A482_AFAFDB23AEDC__INCLUDED_)
