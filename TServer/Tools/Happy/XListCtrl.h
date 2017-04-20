// XListCtrl.h  Version 1.3
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

// 리스트컨트롤 속성
// View: Report
// No Sort Header : TRUE

#ifndef XLISTCTRL_H
#define XLISTCTRL_H

//#include "XHeaderCtrl.h"


//-----------------------------------------------------------------------------
// MemDC.h start
//-----------------------------------------------------------------------------

class CMemDC : public CDC
{
public:

    // constructor sets up the memory DC
    CMemDC(CDC* pDC) : CDC()
    {
        ASSERT(pDC != NULL);

        m_pDC = pDC;
        m_pOldBitmap = NULL;
        m_bMemDC = !pDC->IsPrinting();
              
        if (m_bMemDC)    // Create a Memory DC
        {
			pDC->GetClipBox(&m_rect);
            CreateCompatibleDC(pDC);
            m_bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
            m_pOldBitmap = SelectObject(&m_bitmap);
            SetWindowOrg(m_rect.left, m_rect.top);
        }
        else			// Make a copy of the relevent parts of the current DC for printing
        {
            m_bPrinting = pDC->m_bPrinting;
            m_hDC       = pDC->m_hDC;
            m_hAttribDC = pDC->m_hAttribDC;
        }
    }
    
    // Destructor copies the contents of the mem DC to the original DC
    ~CMemDC()
    {
        if (m_bMemDC) 
        {
            // Copy the offscreen bitmap onto the screen.
            m_pDC->BitBlt(m_rect.left, m_rect.top, m_rect.Width(), m_rect.Height(),
                          this, m_rect.left, m_rect.top, SRCCOPY);

            //Swap back the original bitmap.
            SelectObject(m_pOldBitmap);
        } else {
            // All we need to do is replace the DC with an illegal value,
            // this keeps us from accidently deleting the handles associated with
            // the CDC that was passed to the constructor.
            m_hDC = m_hAttribDC = NULL;
        }
    }

    // Allow usage as a pointer
    CMemDC* operator->() {return this;}
        
    // Allow usage as a pointer
    operator CMemDC*() {return this;}

private:
    CBitmap  m_bitmap;      // Offscreen bitmap
    CBitmap* m_pOldBitmap;  // bitmap originally found in CMemDC
    CDC*     m_pDC;         // Saves CDC passed in constructor
    CRect    m_rect;        // Rectangle of drawing area.
    BOOL     m_bMemDC;      // TRUE if CDC really is a Memory DC.
};
//-----------------------------------------------------------------------------
// MemDC.h end
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// XHeaderCtrl.h Start
//-----------------------------------------------------------------------------

#include <tchar.h>
//#include "MemDC.h"

#define FLATHEADER_TEXT_MAX	80

#define XHEADERCTRL_NO_IMAGE		0
#define XHEADERCTRL_UNCHECKED_IMAGE	1
#define XHEADERCTRL_CHECKED_IMAGE	2

#define FH_PROPERTY_SPACING			1
#define FH_PROPERTY_ARROW			2
#define FH_PROPERTY_STATICBORDER	3
#define FH_PROPERTY_DONTDROPCURSOR	4
#define FH_PROPERTY_DROPTARGET		5

typedef struct tagBITMAP_INFO
{
	BOOL	bUse;
	HBITMAP hBitmap;
	BITMAP	bitmap;
} BITMAP_INFO;

#define MOUSE_OUT					0
#define MOUSE_OVER					1
#define MOUSE_DOWN					2

class CXHeaderCtrl : public CHeaderCtrl
{
    DECLARE_DYNCREATE(CXHeaderCtrl)

// Construction
public:
	CXHeaderCtrl();
	virtual ~CXHeaderCtrl();

// Attributes
public:
	BOOL ModifyProperty(WPARAM wParam, LPARAM lParam);
	int  GetSpacing() { return m_iSpacing; }
	void SetSpacing(int nSpacing) { m_iSpacing = nSpacing; }
	
	void SetFlat(BOOL bFlat=TRUE) { m_bFlat = bFlat; }
	void SetColorHeader(COLORREF color) { m_colorHeader = color; } //Text Color
	void SetTextColor(COLORREF color) { m_colorHeader = color;}
	void SetBgColor(COLORREF color) { m_cr3DFace = color; }
	void SetHeight(int nHeight) { m_nHeight = nHeight; RedrawWindow(); };
	void SetTextFont(CFont *pTextFont) { m_pTextFont = pTextFont; RedrawWindow(); }
	void SetMouseImg(HBITMAP* phOver,HBITMAP* phOut, HBITMAP* phDown);
	void SetMouseImg(CString strOverFile,CString strOutFile, CString strDownFile); // File Name
	

// Overrides
public:
	virtual void DrawItem(LPDRAWITEMSTRUCT);
	virtual void DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXHeaderCtrl)
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL m_bDoubleBuffer;
	int m_iSpacing;
	SIZE m_sizeImage;
	SIZE m_sizeArrow;
	BOOL m_bStaticBorder;
	UINT m_nDontDropCursor;
	BOOL m_bResizing;
	UINT m_nClickFlags;
	CPoint m_ptClickPoint;

	COLORREF m_cr3DHighLight;
	COLORREF m_cr3DShadow;
	COLORREF m_cr3DFace;
	COLORREF m_crBtnText;	

	BOOL m_bTracking;
	BOOL m_bLMouseDown;

	BOOL m_bFlat;
	COLORREF m_colorHeader;

	int m_nHeight;
	CFont *m_pTextFont;

	BITMAP_INFO		m_biMouseImg[3]; // 0-Out.  1-Over.  2-Down.

	void DrawCtrl(CDC* pDC);
	int DrawImage(CDC* pDC, CRect rect, LPHDITEM hdi, BOOL bRight);
	int DrawBitmap(CDC* pDC, CRect rect, LPHDITEM hdi, CBitmap* pBitmap, 
		BITMAP* pBitmapInfo, BOOL bRight);
	int DrawText (CDC* pDC, CRect rect, LPHDITEM lphdi);

	void DrawMouseOver(CDC* pDC, CRect rect);
	void DrawMouseOut(CDC* pDC, CRect rect);
	void DrawMouseDown(CDC* pDC, CRect rect);

// Generated message map functions
protected:
	//{{AFX_MSG(CXHeaderCtrl)
	afx_msg LRESULT OnDeleteItem(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnInsertItem(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnLayout(WPARAM wparam, LPARAM lparam);
	afx_msg LRESULT OnSetImageList(WPARAM wparam, LPARAM lparam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSysColorChange();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------
// XHeaderCtrl.h End
//------------------------------------------------------------------------





//------------------------------------------------------------------------
// XComboBox.h Start
//------------------------------------------------------------------------
extern UINT NEAR WM_XCOMBOBOX_CHANGE;

class CXComboBox : public CComboBox
{
// Construction
public:
	CXComboBox();

// Attributes
private:
	int m_nVK;

	// Singleton instance
	static CXComboBox* m_pXComboBox;

// Operations
public:
	// Returns the instance of the class
	static CXComboBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXComboBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXComboBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCbnCloseup();
//	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------
// XComboBox.h End
//------------------------------------------------------------------------







//------------------------------------------------------------------------
// XEditBox.h Start
//------------------------------------------------------------------------
extern UINT NEAR WM_XEDITBOX_CHANGE;

class CXEditBox : public CEdit
{
// Construction
public:
	CXEditBox();

// Attributes
private:
	// Singleton instance
	static CXEditBox* m_pXEditBox;
	int m_nMinNum;
	int m_nMaxNum;

	int m_nVK;

// Operations
public:
	// Returns the instance of the class
	static CXEditBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();
	
	void SetMinNum(int nMinNum){ m_nMinNum = nMinNum; }
	void SetMaxNum(int nMaxNum){ m_nMaxNum = nMaxNum; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXEditBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXEditBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXEditBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg LRESULT OnSpinDelta( WPARAM wParam, LPARAM lParam );

	DECLARE_MESSAGE_MAP()
};

//------------------------------------------------------------------------
// XEditBox.h End
//------------------------------------------------------------------------






//------------------------------------------------------------------------
// XDateBox.h Start
//------------------------------------------------------------------------
extern UINT NEAR WM_XDATEBOX_CHANGE;

class CXDateBox : public CDateTimeCtrl
{
// Construction
public:
	CXDateBox();

// Attributes
public:
	

private:
	int m_nVK;

	// Singleton instance
	static CXDateBox* m_pXDateBox;

// Operations
public:
	// Returns the instance of the class
	static CXDateBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();

	static CString GetDate(CTime &time, CString &sDateFormat);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXDateBox)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXDateBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXDateBox)
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCloseup(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
//------------------------------------------------------------------------
// XDateBox.h End
//------------------------------------------------------------------------







//------------------------------------------------------------------------
// XSpinBox.h Start
//------------------------------------------------------------------------
extern UINT NEAR WM_XSPINBOX_CHANGING;
extern UINT NEAR WM_XSPINBOX_DELTA;

class CXSpinBox : public CSpinButtonCtrl
{
// Construction
public:
	CXSpinBox();

// Attributes
	// Singleton instance
	static CXSpinBox* m_pXSpinBox;

// Operations
public:
	// Returns the instance of the class
	static CXSpinBox* GetInstance();

	// Deletes the instance of the class
	static void DeleteInstance();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXSpinBox)
	public:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CXSpinBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CXSpinBox)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------
// XSpinBox.h End
//------------------------------------------------------------------------







//------------------------------------------------------------------------
// XListCtrl.h Start
//------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////
// Style bits for use with SetExtendedStyleX()
// none defined yet
///////////////////////////////////////////////////////////////////////////////
// CXListCtrl data

extern UINT NEAR WM_XLISTCTRL_COMBOBOX_CHANGE;
extern UINT NEAR WM_XLISTCTRL_CHECKBOX_CLICKING;
extern UINT NEAR WM_XLISTCTRL_EDITBOX_CHANGE;
extern UINT NEAR WM_XLISTCTRL_DATEBOX_CHANGE;
extern UINT NEAR WM_XLISTCTRL_CHANGE;
extern UINT NEAR WM_XCTRL_SELECT;

class CSortData
{
public:
    CSortData(CString str, DWORD dw) 
	{
		m_strText = str;
		m_dw = dw;
	};
    ~CSortData(){};

    CString m_strText;
    DWORD m_dw;
};


typedef struct tagNMITEM
{
	HWND hwndFrom;
	int	iItem;
	int	iSubItem;
	CString sText;
} NMITEM;

struct XLISTCTRLDATA
{
	// ctor
	XLISTCTRLDATA()
	{
		bEnabled             = TRUE;
		crText               = ::GetSysColor(COLOR_WINDOWTEXT);
		crBackground         = RGB(255,255,255);//::GetSysColor(COLOR_WINDOW);
		bShowProgress        = FALSE;
		nProgressPercent     = 0;
		strProgressMessage   = _T("");
		bShowProgressMessage = TRUE;
		nCheckedState        = -1;
		bBold                = FALSE;
		nImage               = -1;
		bImageCenter		 = FALSE;

		bEditChColor		 = FALSE;
		crChangeText		 = RGB(255,0,0);
		crBar				 = RGB(0,200,0);

		nTextLeftmode		 = 0;

#ifndef NO_XLISTCTRL_TOOL_TIPS
		strToolTip           = _T("");
#endif
		//
		dwItemData           = NULL;

		// for Combo Box
		bCombo               = FALSE;
		nComboListHeight	 = 0;

		// for Date Box
		bDateBox			 = FALSE;
		sDateFormat			 = _T("yyyy-MM-dd_HH:mm:ss");
		dwDateStyle			 = DTS_SHORTDATEFORMAT|WS_VISIBLE|WS_CHILD|WS_TABSTOP;

		// for Edit Box
		bEditBox			 = FALSE;
		nEditLimitText		 = -1;
		nEditMinNum			 = -1;
		nEditMaxNum			 = -1;
		dwEditStyle			 = ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE;
	}

	BOOL			bEnabled;				// TRUE = enabled, FALSE = disabled (gray text)
	BOOL			bBold;					// TRUE = display bold text
	int				nImage;					// index in image list, else -1
	BOOL			bImageCenter;

#ifndef NO_XLISTCTRL_TOOL_TIPS
	CString			strToolTip;				// tool tip text for cell
#endif

	// for combo
	BOOL			bCombo;					// TRUE = display combobox
	CStringArray	sa;						// string array for combo listbox
	int				nComboListHeight;		// combo listbox height (in rows)
	int				nInitialComboSel;		// initial combo listbox selection (0 = first)

	// for color
	COLORREF		crText;
	COLORREF		crBackground;	
	COLORREF		crBar;

	// for editbox
	BOOL			bEditBox;
	int				nEditLimitText;
	int				nEditMinNum;
	int				nEditMaxNum;
	DWORD			dwEditStyle;
	BOOL			bEditChColor;
	COLORREF		crChangeText;

	// for datebox
	BOOL			bDateBox;
	CString			sDateFormat;
	DWORD			dwDateStyle;
	CTime			date;

	// for progress
	BOOL			bShowProgress;				// true = show progress control
	int				nProgressPercent;			// 0 - 100
	CString			strProgressMessage;			// custom message for progress indicator -
												// MUST INCLUDE %d
	BOOL			bShowProgressMessage;		// TRUE = display % message, or custom message
												// if one is supplied
	// for checkbox
	int				nCheckedState;				// -1 = don't show, 0 = unchecked, 1 = checked

	//
	int				nTextLeftmode;

	//
	DWORD			dwItemData;					// pointer to app's data
};


///////////////////////////////////////////////////////////////////////////////
// CXListCtrl class

// nItem = Row.   nSubItem = Col

class CXListCtrl : public CListCtrl
{
// Construction
public:
	CXListCtrl();
	virtual ~CXListCtrl();

//	CBitmap m_BmpBkg;

// Attributes
public:

// Operations
public:
	void	EnableFocusRect(BOOL bFocusRect=TRUE){m_bFocusRect=bFocusRect;}
	void	EnableResize(BOOL bResize=TRUE){m_bResize=bResize;}

	void	SetNoItemMsg(CString strNoItemMsg){m_strNoItemMsg=strNoItemMsg;}
	void	SetStatusColumn(int nSubItem){m_nStatusColumn=nSubItem;}
	void	SetEnableSubCtrl(BOOL bEnable){m_bEnableSubCtrl = bEnable;}

	void	SetBgColor(COLORREF crBg){m_crWindow=crBg;}
	void	SetBgColorProgress(COLORREF crBg){m_crBgProgress=crBg;}
	void	SetUnderLine(BOOL bUnderLine=TRUE){m_bUnderLine=bUnderLine;}
	void	SetColorUnderLine(COLORREF crUnderLine){m_crUnderLine=crUnderLine;}
	void	SetHigLightColor(COLORREF crHighLight,COLORREF crHighLightText);
	void	SetTrackingColor(COLORREF crTracking);
	void	SetBorderColor(COLORREF crBorder);
	void	SetTracking(BOOL bTracking);

	BOOL	GetProgressColor(int nItem, int nSubItem, COLORREF &cf);
	void	GetDrawColors(int nItem, int nSubItem, COLORREF& colorText, COLORREF& colorBkgnd);
	

	int		CountCheckedItems(int nSubItem);
	BOOL	DeleteAllItems();
	BOOL	DeleteItem(int nItem);
	void	DeleteProgress(int nItem, int nSubItem);
	BOOL	GetBold(int nItem, int nSubItem);
	int		GetCheckbox(int nItem, int nSubItem);
	int		GetColumns();
	CString	GetComboText(int nItem, int nSubItem);
	CString	GetComboText(int nItem, int nSubItem,int nIndex);
	int		GetComboCurSel(int nItem, int nSubItem);
	int		GetComboDropDownRows(int nItem, int nSubItem);
	int		GetComboItemCount(int nItem, int nSubItem);
	int		GetCurSel();
	BOOL	GetEnabled(int nItem);
	DWORD	GetExtendedStyleX() { return m_dwExtendedStyleX; }
	int		GetHeaderCheckedState(int nSubItem);
	DWORD	GetItemData(int nItem);
	int		GetItemInt(int nItem, int nSubItem);
	BOOL	GetSubItemRect(int iItem, int iSubItem, int nArea, CRect& rect);
	BOOL	SelectItem(int nItem, int nSubItem);
	
	

	int		InsertItem(int nItem, LPCTSTR lpszItem);
	int		InsertItem(int nItem,
					   LPCTSTR lpszItem,
					   COLORREF crText,
					   COLORREF crBackground);
	int		InsertItem(const LVITEM* pItem);
	BOOL	SetBold(int nItem, int nSubItem, BOOL bBold);

	
	BOOL	SetComboBox(int nItem, 
						int nSubItem, 
						BOOL bEnableCombo, 
						CStringArray *psa=NULL);
	BOOL	SetEditBox(int nItem,
					   int nSubItem,
					   BOOL bEditBox=TRUE,
					   int nEditMinNum=-1,
					   int nEditMaxNum=-1,
					   int nEditLimitText=-1,
					   BOOL bEditChColor=FALSE,
					   COLORREF crChangeText=NULL,
					   DWORD dwEditStyle=ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE);
	BOOL	SetDateBox(int nItem, 
					   int nSubItem, 
					   BOOL bDateBox=TRUE,
					   CString sDateFormat=_T("yyyy-MM-dd_HH:mm:ss"),
					   DWORD dwDateStyle=WS_VISIBLE|WS_CHILD|WS_TABSTOP|DTS_SHORTDATEFORMAT);
	void	UpdateDate(int nItem, int nSubItem, CTime time, COLORREF crText, COLORREF crBackground);
	

	BOOL	AddStringCombo(int nItem, int nSubItem,CString str);
	BOOL	DelStringCombo(int nItem, int nSubItem,CString str);
	BOOL	DelStringCombo(int nItem, int nSubItem,int nIndex);
	BOOL	SetCheckbox(int nItem, int nSubItem, int nCheckedState);
	BOOL	SetCurSel(int nItem);
	BOOL	SetComboCurSel(int nItem, int nSubItem, int nCurSel);
	BOOL	SetComboDropDownRows(int nItem, int nSubItem,int nRows);
	BOOL	SetEnabled(int nItem, BOOL bEnable);
	BOOL	SetSubItemEnabled(int nItem, int nSubItem, BOOL bEnable);
	BOOL	SetEditBoxAlign(int nItem, int nSubItem, DWORD dwAlign = ES_LEFT); //ES_LEFT, ES_CENTER, ES_RIGHT
	void	ResizeListColumn(int arPercent[]); // List Control 크기에 맞춰서 비율로 컬럼 사이즈 조정. arPercent 배열의 마지막 값은 0 이어야 한다.
	DWORD	SetExtendedStyleX(DWORD dwNewStyle) 
	{
		DWORD dwOldStyle = m_dwExtendedStyleX;
		m_dwExtendedStyleX = dwNewStyle;
		return dwOldStyle;
	}

	BOOL	SetHeaderCheckedState(int nSubItem, int nCheckedState);
	int		SetItem(const LVITEM* pItem);
	BOOL	SetItemData(int nItem, DWORD dwData);
	
	BOOL	SetItemImage(int nItem, int nSubItem, int nImage, BOOL bImageCenter=FALSE);
	int		GetItemImage(int nItem, int nSubItem);
	
	BOOL	SetItemInt( int nItem, int nSubItem, int nValue);
	BOOL	SetItemInt( int nItem, int nSubItem, int nValue, COLORREF crText, COLORREF crBackground);

	BOOL	SetItemText(int nItem, int nSubItem, LPCTSTR lpszText); 
	BOOL	SetItemText(int nItem, 
						int nSubItem, 
						LPCTSTR lpszText,
						COLORREF crText, 
						COLORREF crBackground);
	BOOL	SetItemTextColor(int nItem, 
							 int nSubItem, 
							 COLORREF crText, 
							 COLORREF crBackground);
	BOOL	SetItemTextSelAlwaysColor(COLORREF crText, COLORREF crBackground);
	BOOL	SetProgress(int nItem, 
						int nSubItem, 
						BOOL bShowProgressText = TRUE, 
						LPCTSTR lpszProgressText = NULL);
	
	void	UpdateProgress(int nItem, int nSubItem, int nPercent, COLORREF crText, COLORREF crBar, CString ProgressText=_T(""));
	
	void	UpdateProgressLeftText(int nItem, int nSubItem, int nPercent, COLORREF crBar,
								   CString ProgressText, int nTextLeftmode);
	void	UpdateSubItem(int nItem, int nSubItem);

	
	virtual void Sort(int nSubItem, BOOL bSort); // bSort = TRUE:내림차순, FALSE:오름차순

	void	SetRowHeight(int nRowHeight);
	void	SetTextFont(CFont *pTextFont) { m_pTextFont = pTextFont; RedrawWindow(); }

#ifndef NO_XLISTCTRL_TOOL_TIPS
	void DeleteAllToolTips();
	BOOL SetItemToolTipText(int nItem, int nSubItem, LPCTSTR lpszToolTipText);
	CString GetItemToolTipText(int nItem, int nSubItem);
	virtual int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
#endif

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXListCtrl)
	public:
	virtual void PreSubclassWindow();
	protected:
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	//}}AFX_VIRTUAL

// Implementation
public:
	CXHeaderCtrl	m_HeaderCtrl;

protected:
	CImageList		m_cImageList;		// Image list for the header control

	BOOL			m_bUnderLine;		// 언더라인 그리기의 유무 설정
	BOOL			m_bResize;			// 마우스 드래그로 헤더 컬럼의 리사이즈 유무 설정
	BOOL			m_bFocusRect;		// Focus Rect 표시 유무
	CString			m_strNoItemMsg;		// 아이템이 없을때 표시 할 텍스트 설정
	int				m_nStatusColumn;	// 상태 컬럼의 Rect(백그라운드 컬러영역)을 재설정할 컬럼 설정
	BOOL			m_bEnableSubCtrl;	// 리스트에 포함된 모든 컨트롤 상태

	COLORREF		m_crBgProgress;		// 프로그레스의 배경색	
	COLORREF		m_crUnderLine;		// 언더라인의 색

	BOOL			m_bHeaderTracking;	// 현재 헤더가 트래킹중인지 판단하는 플래그
	BOOL			m_bHeaderDraging;	// 현재 헤더가 드래깅중인지 판단하는 플래그

	BOOL			m_bTracking;
	BOOL			m_bUseTracking;		// 트래킹을 사용할 것인가
	BOOL			m_bSelAlwaysColor;  // 아이템을 선택후 다른 컨트롤에 포커스가 옮겨간 후 선택된 아이템의 텍스트를 지정한 색으로 보여줄 것인가.
	
	CFont		*	m_pTextFont;

protected:
	void DrawCheckbox(int nItem, 
					  int nSubItem, 
					  CDC *pDC, 
					  COLORREF crText,
					  COLORREF crBkgnd,
					  CRect& rect, 
					  XLISTCTRLDATA *pCLD);
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	void ShowComboBox(int nItem, int nSubItem);
#endif
	void ShowEditBox(int nItem, int nSubItem);
	void ShowDateBox(int nItem, int nSubItem);

	int DrawImage(int nItem, 
				  int nSubItem, 
				  CDC* pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect rect,
				  XLISTCTRLDATA *pXLCD);

	void DrawProgress(int nItem, 
					  int nSubItem, 
					  CDC *pDC, 
					  COLORREF crText,
					  COLORREF crBkgnd,
					  CRect& rect, 
					  XLISTCTRLDATA *pCLD);
	void DrawText(int nItem, 
				  int nSubItem, 
				  CDC *pDC, 
				  COLORREF crText,
				  COLORREF crBkgnd,
				  CRect& rect, 
				  XLISTCTRLDATA *pCLD);
	void SubclassHeaderControl();

	BOOL			m_bHeaderIsSubclassed;
	DWORD			m_dwExtendedStyleX;

	COLORREF		m_cr3DFace;
	COLORREF		m_cr3DHighLight;
	COLORREF		m_cr3DShadow;
	COLORREF		m_crBtnFace;
	COLORREF		m_crBtnShadow;
	COLORREF		m_crBtnText;
	COLORREF		m_crGrayText;
	COLORREF		m_crHighLight;
	COLORREF		m_crHighLightText;
	COLORREF		m_crWindow;
	COLORREF		m_crWindowText;
	COLORREF		m_crTracking;
	COLORREF		m_crBorder;
	COLORREF		m_crSelalwaysText;
	COLORREF		m_crSelalwaysTextBg;

	
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	int				m_nComboItem;
	int				m_nComboSubItem;
#endif

	int				m_nEditItem;
	int				m_nEditSubItem;
	int				m_nDateItem;
	int				m_nDateSubItem;	
	int				m_nTrackingItem;


	// Generated message map functions
protected:
	//{{AFX_MSG(CXListCtrl)
	afx_msg BOOL OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg void OnDestroy();
	afx_msg void OnSysColorChange();
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnNcPaint();
	//}}AFX_MSG

	afx_msg LRESULT OnEditChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnDateChange( WPARAM wParam, LPARAM lParam );
	afx_msg LRESULT OnSpinChanging(WPARAM wParam, LPARAM lParam);

#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	afx_msg LRESULT OnComboChange( WPARAM wParam, LPARAM lParam );
#endif

	void SetLButtonDown(int nStartItem, int nStartSubItem);
	static int CALLBACK CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort);

#ifndef NO_XLISTCTRL_TOOL_TIPS
	virtual afx_msg BOOL OnToolTipText(UINT id, NMHDR * pNMHDR, LRESULT * pResult);
#endif
	DECLARE_MESSAGE_MAP()
};
//------------------------------------------------------------------------
// XListCtrl.h End
//------------------------------------------------------------------------


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


#endif //XLISTCTRL_H

