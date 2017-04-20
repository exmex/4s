
// XListCtrl.cpp  Version 1.3
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This code is based on "Neat Stuff to do in List Controls Using Custom Draw"
// by Michael Dunn. See http://www.codeproject.com/listctrl/lvcustomdraw.asp
//
// Thanks to David Patrick for pointing out how to subclass header control
// if CXListCtrl is created via Create() instead of via dialog template.
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XListCtrl.h"


//#ifndef DO_NOT_INCLUDE_XCOMBOLIST
//#include "XComboBox.h"
//#endif

//#include "XEditBox.h"
//#include "XDateBox.h"
//#include "XSpinBox.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// specific
#include "MainFrm.h"
#include "HappyDoc.h"
#include ".\xlistctrl.h"


//------------------------------------------------------------------------
// XHeaderCtrl.cpp Start
//------------------------------------------------------------------------

/////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl
CXHeaderCtrl::CXHeaderCtrl()
{
	m_bDoubleBuffer = TRUE;
	m_iSpacing = 6;
	m_sizeArrow.cx = 8;
	m_sizeArrow.cy = 8;
	m_sizeImage.cx = 0;
	m_sizeImage.cy = 0;
	m_bStaticBorder = FALSE;
	m_nDontDropCursor = 0;
	m_bResizing = FALSE;
	m_nClickFlags = 0;

	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);	


	m_bTracking		= FALSE;
	m_bLMouseDown	= FALSE;

	m_bFlat = FALSE;
	m_colorHeader = RGB(0,0,0);

	m_nHeight		= 30;
	m_pTextFont		= NULL;

	for(int i = 0; i < 3; i++)
		m_biMouseImg[i].bUse = FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXHeaderCtrl::~CXHeaderCtrl()
{
}

///////////////////////////////////////////////////////////////////////////////
//
IMPLEMENT_DYNCREATE(CXHeaderCtrl, CHeaderCtrl)

BEGIN_MESSAGE_MAP(CXHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CXHeaderCtrl)
	ON_MESSAGE(HDM_INSERTITEM, OnInsertItem)
	ON_MESSAGE(HDM_DELETEITEM, OnDeleteItem)
	ON_MESSAGE(HDM_SETIMAGELIST, OnSetImageList)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ERASEBKGND()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ModifyProperty
BOOL CXHeaderCtrl::ModifyProperty(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case FH_PROPERTY_SPACING:
			m_iSpacing = (int)lParam;
			break;

		case FH_PROPERTY_ARROW:
			m_sizeArrow.cx = LOWORD(lParam);
			m_sizeArrow.cy = HIWORD(lParam);
			break;

		case FH_PROPERTY_STATICBORDER:
			m_bStaticBorder = (BOOL)lParam;
			break;

		case FH_PROPERTY_DONTDROPCURSOR:
			m_nDontDropCursor = (UINT)lParam;
			break;

		default:
			return FALSE;
	}

	Invalidate();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// DrawCtrl
void CXHeaderCtrl::DrawCtrl(CDC* pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;

	CRect rectClient, rectItem;
	GetClientRect(&rectClient);

	HCURSOR hCursor = GetCursor();
	HCURSOR hCursorArrow = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
	pDC->FillSolidRect(rectClip, m_cr3DFace);
	
	int iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen penHighLight(PS_SOLID, 1, m_cr3DHighLight);
	CPen penShadow(PS_SOLID, 1, m_cr3DShadow);
	CPen* pPen = pDC->GetCurrentPen();


	CFont* pFont = pDC->SelectObject(m_pTextFont);
	//CFont* pFont = pDC->SelectObject(GetFont());


	pDC->SetBkColor(m_cr3DFace);	
	//pDC->SetTextColor(m_crBtnText);
	pDC->SetTextColor(RGB(0,0,255));

	int iWidth = 0;

	for (int i = 0; i < iItems; i++)
	{
		int iItem = OrderToIndex(i);

		TCHAR szText[FLATHEADER_TEXT_MAX];

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));
		VERIFY(GetItemRect(iItem, rectItem));


		DWORD dwStyle = ::GetWindowLong(GetParent()->GetSafeHwnd(), GWL_STYLE);	
		POINT pointMouse;
		::GetCursorPos(&pointMouse);
		::ScreenToClient(m_hWnd, &pointMouse);			

		if( !(dwStyle&LVS_NOSORTHEADER) && 
			rectItem.PtInRect(pointMouse) && 
			hCursor == hCursorArrow )
		{
			if( !m_bLMouseDown )
				DrawMouseOver(pDC, rectItem);
			else
				DrawMouseDown(pDC, rectItem);
		}
		else
			DrawMouseOut(pDC, rectItem);


		if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right)
		{
			if (hditem.fmt & HDF_OWNERDRAW)
			{
				DRAWITEMSTRUCT disItem;
				disItem.CtlType = ODT_BUTTON;
				disItem.CtlID = GetDlgCtrlID();
				disItem.itemID = iItem;
				disItem.itemAction = ODA_DRAWENTIRE;
				disItem.itemState = 0;
				disItem.hwndItem = m_hWnd;
				disItem.hDC = pDC->m_hDC;
				disItem.rcItem = rectItem;
				disItem.itemData = 0;

				DrawItem(&disItem);
			}
			else
			{
				rectItem.DeflateRect(m_iSpacing, 0);
				DrawItem(pDC, rectItem, &hditem);
				rectItem.InflateRect(m_iSpacing, 0);

				//if (m_nClickFlags & MK_LBUTTON && m_iHotIndex == iItem && m_hdhtiHotItem.flags & HHT_ONHEADER)
				//	pDC->InvertRect(rectItem);
			}

			if (i < iItems-1)
			{
				//SIZE size = Global.GetBitmapSize(IDX_BMP_LIST_HEADER_DIVISION);
				//SIZE size2 = Global.GetBitmapSize(IDX_BMP_LIST_HEADER_SHADOW);
				//CDC tmpDC;
				//tmpDC.CreateCompatibleDC(pDC);
				//tmpDC.SelectObject(Global.GetBitmap(IDX_BMP_LIST_HEADER_DIVISION));
				//pDC->BitBlt(rectItem.right, rectItem.bottom-size.cy-size2.cy, size.cx, size.cy, 
				//	&tmpDC, 0, 0, SRCCOPY);
				//tmpDC.DeleteDC();				
			}
		}
		iWidth += hditem.cxy;
	}

	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);

	penHighLight.DeleteObject();
	penShadow.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CXHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT)
{
	ASSERT(FALSE);  // must override for self draw header controls
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CXHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	ASSERT(lphdi->mask & HDI_FORMAT);

	int iWidth = 0;

	CBitmap* pBitmap = NULL;
	BITMAP BitmapInfo;

	if (lphdi->fmt & HDF_BITMAP)
	{
		ASSERT(lphdi->mask & HDI_BITMAP);
		ASSERT(lphdi->hbm);

		pBitmap = CBitmap::FromHandle(lphdi->hbm);
		if (pBitmap)
			VERIFY(pBitmap->GetObject(sizeof(BITMAP), &BitmapInfo));
	}

	rect.left += ((iWidth = DrawImage(pDC, rect, lphdi, FALSE)) != 0) ? iWidth + m_iSpacing : 0;
	rect.right -= ((iWidth = DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE)) != 0) ? 
		iWidth + m_iSpacing : 0;

	DrawText(pDC, rect, lphdi);
}

///////////////////////////////////////////////////////////////////////////////
// DrawImage
int CXHeaderCtrl::DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight)
{
	CImageList* pImageList = GetImageList();
	int iWidth = 0;

	if (lphdi->iImage != XHEADERCTRL_NO_IMAGE)
	{
		if (pImageList)
		{
			if (rect.Width() > 0)
			{
				POINT point;

				point.y = rect.CenterPoint().y - (m_sizeImage.cy >> 1);

				if (bRight)
					point.x = rect.right - m_sizeImage.cx;
				else
					point.x = rect.left;

				SIZE size;
				size.cx = rect.Width()<m_sizeImage.cx ? rect.Width():m_sizeImage.cx;
				size.cy = m_sizeImage.cy;

				// save image list background color
				COLORREF rgb = pImageList->GetBkColor();

				// set image list background color to same as header control
				pImageList->SetBkColor(pDC->GetBkColor());
				pImageList->DrawIndirect(pDC, lphdi->iImage, point, size, CPoint(0, 0));
				pImageList->SetBkColor(rgb);

				iWidth = m_sizeImage.cx;
			}
		}
	}

	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawBitmap
int CXHeaderCtrl::DrawBitmap(CDC* pDC, 
								CRect rect, 
								LPHDITEM lphdi, 
								CBitmap* pBitmap, 
								BITMAP* pBitmapInfo, 
								BOOL bRight)
{
	UNUSED_ALWAYS(lphdi);

	int iWidth = 0;

	if (pBitmap)
	{
		iWidth = pBitmapInfo->bmWidth;
		if (iWidth <= rect.Width() && rect.Width() > 0)
		{
			POINT point;

			//point.y = rect.CenterPoint().y - (pBitmapInfo->bmHeight >> 1);
			point.y = rect.bottom - pBitmapInfo->bmHeight - 10;

			if (bRight)
				point.x = rect.right - iWidth;
			else
				point.x = rect.left;

			CDC dc;
			if (dc.CreateCompatibleDC(pDC) == TRUE) 
			{
				VERIFY(dc.SelectObject(pBitmap));
				iWidth = pDC->BitBlt(
					point.x, point.y, 
					pBitmapInfo->bmWidth, pBitmapInfo->bmHeight, 
					&dc, 
					0, 0, 
					SRCCOPY
				) ? iWidth:0;
			}
			else 
				iWidth = 0;
		}
		else
			iWidth = 0;
	}

	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
int CXHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	pDC->SetBkMode(TRANSPARENT);

	CSize size;
	
	pDC->SetTextColor(m_colorHeader);

	if (rect.Width() > 0 && lphdi->mask & HDI_TEXT && lphdi->fmt & HDF_STRING)
	{
		size = pDC->GetTextExtent(lphdi->pszText);

		rect.top = 5;
		// always center column headers
		pDC->DrawText(lphdi->pszText, -1, rect, 
			DT_CENTER|DT_END_ELLIPSIS|DT_SINGLELINE/*|DT_VCENTER*/);
	}

	size.cx = rect.Width()>size.cx ? size.cx:rect.Width();
	return size.cx>0 ? size.cx:0;
}

///////////////////////////////////////////////////////////////////////////////
// OnInsertItem
LRESULT CXHeaderCtrl::OnInsertItem(WPARAM, LPARAM)
{
	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnDeleteItem
LRESULT CXHeaderCtrl::OnDeleteItem(WPARAM, LPARAM)
{
	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnSetImageList
LRESULT CXHeaderCtrl::OnSetImageList(WPARAM, LPARAM lParam)
{
	CImageList* pImageList;
	pImageList = CImageList::FromHandle((HIMAGELIST)lParam);

	IMAGEINFO info;
	if (pImageList->GetImageInfo(0, &info))
	{
		m_sizeImage.cx = info.rcImage.right - info.rcImage.left;
		m_sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
	}

	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnLayout
LRESULT CXHeaderCtrl::OnLayout(WPARAM, LPARAM lParam)
{
#if 1
	LRESULT lResult = CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);

	if( m_nHeight > 0 )
	{
		LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;
		RECT *prc = lphdlayout->prc;
		WINDOWPOS *pwpos = lphdlayout->pwpos;

		pwpos->cy = m_nHeight;
		prc->top = m_nHeight;
	}
	return lResult;	
#else
	LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;

	if (m_bStaticBorder)
		lphdlayout->prc->right += GetSystemMetrics(SM_CXBORDER)*2;

	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
#endif
}

///////////////////////////////////////////////////////////////////////////////
// OnSysColorChange
void CXHeaderCtrl::OnSysColorChange() 
{
	TRACE(_T("in CXHeaderCtrl::OnSysColorChange\n"));

	CHeaderCtrl::OnSysColorChange();
	
	m_cr3DHighLight = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow    = ::GetSysColor(COLOR_3DSHADOW);
	m_cr3DFace      = ::GetSysColor(COLOR_3DFACE);
	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
}

///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL CXHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	UNUSED_ALWAYS(pDC);
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CXHeaderCtrl::OnPaint() 
{
    CPaintDC dc(this);

    if (m_bDoubleBuffer)
    {
        CMemDC MemDC(&dc);
		DrawCtrl(&MemDC);
    }
    else
        DrawCtrl(&dc);
}

void CXHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	//
    if(!m_bTracking) 
	{
        TRACKMOUSEEVENT t = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
        if (::_TrackMouseEvent(&t)) 
		{
            m_bTracking = TRUE;
        }
    }
	else
	{
		Invalidate();
	}

	CHeaderCtrl::OnMouseMove(nFlags, point);
}

LRESULT CXHeaderCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bTracking = FALSE;
    Invalidate();
	return 0L;
}

void CXHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLMouseDown = TRUE;

	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

void CXHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bLMouseDown = FALSE;

	CHeaderCtrl::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// DrawMouseOver
void CXHeaderCtrl::DrawMouseOver(CDC* pDC, CRect rect)
{
	if(m_biMouseImg[MOUSE_OVER].bUse)
	{
		SIZE size;
		size.cx = m_biMouseImg[MOUSE_OVER].bitmap.bmWidth;
		size.cy = m_biMouseImg[MOUSE_OVER].bitmap.bmHeight;

		CDC tmpDC;
		tmpDC.CreateCompatibleDC(pDC);
		tmpDC.SelectObject(m_biMouseImg[MOUSE_OVER].hBitmap);
		pDC->StretchBlt(rect.left, rect.bottom-size.cy, rect.Width(), size.cy, &tmpDC, 0, 0, size.cx, size.cy, SRCCOPY);
		tmpDC.DeleteDC();
	}
}

///////////////////////////////////////////////////////////////////////////////
// DrawMouseOut
void CXHeaderCtrl::DrawMouseOut(CDC* pDC, CRect rect)
{
	if(m_biMouseImg[MOUSE_OUT].bUse)
	{
		SIZE size;
		size.cx = m_biMouseImg[MOUSE_OUT].bitmap.bmWidth;
		size.cy = m_biMouseImg[MOUSE_OUT].bitmap.bmHeight;

		CDC tmpDC;
		tmpDC.CreateCompatibleDC(pDC);
		tmpDC.SelectObject(m_biMouseImg[MOUSE_OUT].hBitmap);
		pDC->StretchBlt(rect.left, rect.bottom-size.cy, rect.Width(), size.cy, &tmpDC, 0, 0, size.cx, size.cy, SRCCOPY);
		tmpDC.DeleteDC();
	}
}

///////////////////////////////////////////////////////////////////////////////
// DrawMouseDown
void CXHeaderCtrl::DrawMouseDown(CDC* pDC, CRect rect)
{
	if(m_biMouseImg[MOUSE_DOWN].bUse)
	{
		SIZE size;
		size.cx = m_biMouseImg[MOUSE_DOWN].bitmap.bmWidth;
		size.cy = m_biMouseImg[MOUSE_DOWN].bitmap.bmHeight;

		CDC tmpDC;
		tmpDC.CreateCompatibleDC(pDC);
		tmpDC.SelectObject(m_biMouseImg[MOUSE_DOWN].hBitmap);
		pDC->StretchBlt(rect.left, rect.bottom-size.cy, rect.Width(), size.cy, &tmpDC, 0, 0, size.cx, size.cy, SRCCOPY);
		tmpDC.DeleteDC();
	}
}

void CXHeaderCtrl::SetMouseImg(CString strOutFile,CString strOverFile, CString strDownFile)
{
	CString strFile =_T("");
	for(int i = 0; i < 3; i++)
	{
		if(i = MOUSE_OVER)
			strFile = strOverFile;
		else if( i = MOUSE_DOWN)
			strFile = strDownFile;
		else 
			strFile = strOutFile;

		if( strFile.IsEmpty() )
		{
			m_biMouseImg[i].bUse = FALSE;
			continue;
		}

		m_biMouseImg[i].bUse = TRUE;

		m_biMouseImg[i].hBitmap = (HBITMAP)::LoadImage(::AfxGetInstanceHandle(), 
			(LPCTSTR)strOverFile, 
			IMAGE_BITMAP, 
			0, 
			0, 
			LR_LOADFROMFILE|LR_CREATEDIBSECTION|LR_DEFAULTSIZE);

		::GetObject(m_biMouseImg[i].hBitmap, sizeof(BITMAP), &m_biMouseImg[i].bitmap);
	}
}

void CXHeaderCtrl::SetMouseImg(HBITMAP* phOver,HBITMAP* phOut, HBITMAP* phDown)
{
	HBITMAP* pBITMAP = NULL;
	for(BYTE i = 0; i < 3; i++)
	{
		if(i == MOUSE_OVER)
			pBITMAP = phOver;
		else if( i == MOUSE_DOWN)
			pBITMAP = phDown;
		else
			pBITMAP = phOut;

		if(pBITMAP == NULL)
		{
			m_biMouseImg[i].bUse = FALSE;
			continue;
		}

		m_biMouseImg[i].hBitmap = (*pBITMAP);
		::GetObject(m_biMouseImg[i].hBitmap, sizeof(BITMAP), &m_biMouseImg[i].bitmap);
	}
}
//------------------------------------------------------------------------
// XHeaderCtrl.cpp End
//------------------------------------------------------------------------






//------------------------------------------------------------------------
// XComboBox.cpp Start
//------------------------------------------------------------------------
UINT NEAR WM_XCOMBOBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XCOMBOBOX_CHANGE"));

/////////////////////////////////////////////////////////////////////////////
// CXComboBox

CXComboBox* CXComboBox::m_pXComboBox = NULL; 

CXComboBox::CXComboBox()
{
}

CXComboBox::~CXComboBox()
{
}


BEGIN_MESSAGE_MAP(CXComboBox, CComboBox)
	//{{AFX_MSG_MAP(CXComboBox)
	ON_WM_KILLFOCUS()
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCbnCloseup)
//	ON_WM_CREATE()
	//}}AFX_MSG_MAP	
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXComboBox message handlers

CXComboBox* CXComboBox::GetInstance()
{
	if(m_pXComboBox == NULL)
	{
		m_pXComboBox = new CXComboBox;
	}
	return m_pXComboBox;
}

void CXComboBox::DeleteInstance()
{
	if(m_pXComboBox != NULL)
	{
		if(m_pXComboBox->m_hWnd != NULL)
			m_pXComboBox->DestroyWindow();

		delete m_pXComboBox;
		m_pXComboBox = NULL;
	}
}

void CXComboBox::OnKillFocus(CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	::SendMessage(GetParent()->m_hWnd, WM_XCOMBOBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)GetParent()->GetDlgCtrlID() );		
}

//-----------------------------------------------------------------------------------------------
// Remember that PreTranslateMessage is only called by a message loop from inside a MFC program.
// If the host is another program for an ActiveX, you can not use PreTranslateMessage!
//-----------------------------------------------------------------------------------------------
BOOL CXComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message==WM_KEYDOWN ) 
	{
		// Enter Key
		if( pMsg->wParam == VK_RETURN || 
			pMsg->wParam == VK_ESCAPE ||
			pMsg->wParam == VK_TAB ) 
		{
			m_nVK = (int)pMsg->wParam;
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}
	}
	
	return CComboBox::PreTranslateMessage(pMsg);
}
void CXComboBox::OnCbnCloseup()
{
	// TODO: Add your message handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	::SendMessage(::GetParent(m_hWnd), WM_XCOMBOBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)GetParent()->GetDlgCtrlID() );
}

/*
int CXComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here	

	return 0;
}
*/

//------------------------------------------------------------------------
// XComboBox.cpp End
//------------------------------------------------------------------------








//------------------------------------------------------------------------
// XEditBox.cpp Start
//------------------------------------------------------------------------

UINT NEAR WM_XEDITBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XEDITBOX_CHANGE"));

/////////////////////////////////////////////////////////////////////////////
// CXEditBox

CXEditBox* CXEditBox::m_pXEditBox = NULL; 

CXEditBox::CXEditBox()
{
	m_nMinNum = -1;
	m_nMaxNum = -1;

	m_nVK = 0;
}

CXEditBox::~CXEditBox()
{
	CXSpinBox::DeleteInstance();
}


BEGIN_MESSAGE_MAP(CXEditBox, CEdit)
	//{{AFX_MSG_MAP(CXEditBox)
	ON_WM_KILLFOCUS()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_REGISTERED_MESSAGE(WM_XSPINBOX_DELTA,OnSpinDelta)
	ON_WM_PAINT()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXEditBox message handlers

CXEditBox* CXEditBox::GetInstance()
{
	if(m_pXEditBox == NULL)
	{
		m_pXEditBox = new CXEditBox;
	}
	return m_pXEditBox;
}

void CXEditBox::DeleteInstance()
{
	if(m_pXEditBox != NULL)
	{
		if(m_pXEditBox->m_hWnd != NULL)
			m_pXEditBox->DestroyWindow();

		delete m_pXEditBox;
		m_pXEditBox = NULL;
	}
}

void CXEditBox::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);	
	::SendMessage(GetParent()->m_hWnd, WM_XEDITBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)GetParent()->GetDlgCtrlID() );	
}

//-----------------------------------------------------------------------------------------------
// Remember that PreTranslateMessage is only called by a message loop from inside a MFC program.
// If the host is another program for an ActiveX, you can not use PreTranslateMessage!
//-----------------------------------------------------------------------------------------------
BOOL CXEditBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message==WM_KEYDOWN ) 
	{
		// Enter Key
		if( pMsg->wParam == VK_RETURN ||
			pMsg->wParam == VK_ESCAPE ||
			pMsg->wParam == VK_TAB ) 
		{
			m_nVK = (int)pMsg->wParam;
			::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}
	}
	
	return CEdit::PreTranslateMessage(pMsg);
}

LRESULT CXEditBox::OnSpinDelta(WPARAM wParam, LPARAM lParam)
{
	BOOL bUp = (BOOL)wParam;

	CString sText(_T(""));
	GetWindowText(sText);
	int nValue = _tstoi(sText);

	if( bUp )
	{
		nValue++;
		if( nValue <= m_nMaxNum )
		{
			sText.Format(_T("%d"), nValue);
			SetWindowText(sText);
		}
	}
	else
	{
		nValue--;
		if( nValue >= m_nMinNum )
		{
			sText.Format(_T("%d"), nValue);
			SetWindowText(sText);
		}
	}

	return 0L;
}

int CXEditBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here

	// Create Spin Box
	if( m_nMinNum >=0 && m_nMaxNum >=0 )
	{
		ModifyStyle(0, WS_CLIPCHILDREN);	
		CXSpinBox::DeleteInstance();
		CXSpinBox *pSpinBox = CXSpinBox::GetInstance();
		if( pSpinBox )
		{
			CRect rc;
			GetClientRect(&rc);
			rc.left = rc.right - 16;
			pSpinBox->Create(UDS_WRAP | UDS_SETBUDDYINT | UDS_AUTOBUDDY | UDS_NOTHOUSANDS | 
				WS_CHILD | WS_VISIBLE, rc, this, NULL);
			pSpinBox->SetRange(0, 1);
		}
	}

	return 0;
}

void CXEditBox::OnPaint()
{
	Default();

	if( m_nMinNum >=0 && m_nMaxNum >=0 &&
		CXSpinBox::m_pXSpinBox )
	{
		CXSpinBox::m_pXSpinBox->RedrawWindow();
	}
}

//------------------------------------------------------------------------
// XEditBox.cpp End
//------------------------------------------------------------------------





//------------------------------------------------------------------------
// XDateBox.cpp Start
//------------------------------------------------------------------------

UINT NEAR WM_XDATEBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XDATEBOX_CHANGE"));

/////////////////////////////////////////////////////////////////////////////
// CXDateBox

CXDateBox* CXDateBox::m_pXDateBox = NULL; 

CXDateBox::CXDateBox()
{
	m_nVK = 0;
}

CXDateBox::~CXDateBox()
{
}


BEGIN_MESSAGE_MAP(CXDateBox, CDateTimeCtrl)
	//{{AFX_MSG_MAP(CXDateBox)
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(DTN_CLOSEUP, OnCloseup)
	//}}AFX_MSG_MAP
	ON_WM_CREATE()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXDateBox message handlers

CXDateBox* CXDateBox::GetInstance()
{
	if(m_pXDateBox == NULL)
	{
		m_pXDateBox = new CXDateBox;
	}
	return m_pXDateBox;
}

void CXDateBox::DeleteInstance()
{
	if(m_pXDateBox != NULL)
	{
		if(m_pXDateBox->m_hWnd != NULL)
			m_pXDateBox->DestroyWindow();

		delete m_pXDateBox;
		m_pXDateBox = NULL;
	}
}

void CXDateBox::OnKillFocus(CWnd* pNewWnd) 
{
	CDateTimeCtrl::OnKillFocus(pNewWnd);
	
	// TODO: Add your message handler code here

	CMonthCalCtrl *pMonthWnd = GetMonthCalCtrl();
	if( !pMonthWnd )
	{
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		::SendMessage(::GetParent(m_hWnd), WM_XDATEBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)GetParent()->GetDlgCtrlID() );		
	}
}

//-----------------------------------------------------------------------------------------------
// Remember that PreTranslateMessage is only called by a message loop from inside a MFC program.
// If the host is another program for an ActiveX, you can not use PreTranslateMessage!
//-----------------------------------------------------------------------------------------------
BOOL CXDateBox::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if( pMsg->message==WM_KEYDOWN ) 
	{
		// Key
		if( pMsg->wParam == VK_RETURN || 
			pMsg->wParam == VK_ESCAPE ||
			pMsg->wParam == VK_TAB ||
			pMsg->wParam == VK_DELETE ) 
		{
			m_nVK = (int)pMsg->wParam;

			CMonthCalCtrl *pMonthWnd = GetMonthCalCtrl();
			if( !pMonthWnd )
				::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		}
	}
	
	return CDateTimeCtrl::PreTranslateMessage(pMsg);
}

// 날짜 포맷으로 부터 날짜 문자열 획득
CString CXDateBox::GetDate(CTime &time, CString &sDateFormat)
{
	int nStart=0, nFind=0;
	CString sDate(_T("")), sTemp(_T(""));

	// Year(yyyy)
	nFind = sDateFormat.Find(_T("yyyy"), nStart);	
	if( nFind >= 0 )
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%04d"), time.GetYear());
		sDate += sTemp;

		nStart = nFind + 4;
	}

	// Month(MM)
	nFind = sDateFormat.Find(_T("MM"), nStart);		
	if( nFind >= 0 ) 
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetMonth());
		sDate += sTemp;

		nStart = nFind + 2;
	}

	// Day(dd)
	nFind = sDateFormat.Find(_T("dd"), nStart);		
	if( nFind >= 0 ) 
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetDay());
		sDate += sTemp;

		nStart = nFind + 2;
	}

	// Hour(HH)
	nFind = sDateFormat.Find(_T("HH"), nStart);		
	if( nFind >= 0 ) 
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetHour());
		sDate += sTemp;

		nStart = nFind + 2;
	}

	// Minute(mm)
	nFind = sDateFormat.Find(_T("mm"), nStart);		
	if( nFind >= 0 )
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetMinute());
		sDate += sTemp;

		nStart = nFind + 2;
	}

	// Seconds(ss)
	nFind = sDateFormat.Find(_T("ss"), nStart);		
	if( nFind >= 0 ) 
	{
		sDate += sDateFormat.Mid(nStart, nFind-nStart);

		sTemp.Format(_T("%02d"), time.GetSecond());
		sDate += sTemp;		

		nStart = nFind + 2;
	}

	sDate += sDateFormat.Mid(nStart);

	sDate.Remove(_T('\''));
	return sDate;
}

void CXDateBox::OnCloseup(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
	::SendMessage(::GetParent(m_hWnd), WM_XDATEBOX_CHANGE, (WPARAM)m_nVK, (LPARAM)GetParent()->GetDlgCtrlID() );	

	*pResult = 0;
}

int CXDateBox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDateTimeCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here

	return 0;
}
//------------------------------------------------------------------------
// XDateBox.cpp End
//------------------------------------------------------------------------






//------------------------------------------------------------------------
// XSpinBox.cpp Start
//------------------------------------------------------------------------

#define SPIN_MODE_UP		1
#define SPIN_MODE_DOWN		0
#define SPIN_MODE_NOTUSE	-1

UINT NEAR WM_XSPINBOX_CHANGING = ::RegisterWindowMessage(_T("WM_XSPINBOX_CHANGING"));
UINT NEAR WM_XSPINBOX_DELTA = ::RegisterWindowMessage(_T("WM_XSPINBOX_DELTA"));

/////////////////////////////////////////////////////////////////////////////
// CXSpinBox

CXSpinBox* CXSpinBox::m_pXSpinBox = NULL; 

CXSpinBox::CXSpinBox()
{
}

CXSpinBox::~CXSpinBox()
{
}

BEGIN_MESSAGE_MAP(CXSpinBox, CSpinButtonCtrl)
	//{{AFX_MSG_MAP(CXSpinBox)
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(UDN_DELTAPOS, OnDeltapos)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXSpinBox message handlers

CXSpinBox* CXSpinBox::GetInstance()
{
	if(m_pXSpinBox == NULL)
	{
		m_pXSpinBox = new CXSpinBox;
	}
	return m_pXSpinBox;
}

void CXSpinBox::DeleteInstance()
{
	if(m_pXSpinBox != NULL)
	{
		if(m_pXSpinBox->m_hWnd != NULL)
			m_pXSpinBox->DestroyWindow();

		delete m_pXSpinBox;
		m_pXSpinBox = NULL;
	}
}

void CXSpinBox::OnDeltapos(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	*pResult = 1;

	int nLower=0, nUpper=0;
	GetRange(nLower, nUpper);

	int nPos = pNMUpDown->iPos;

	// Down
	if( pNMUpDown->iDelta < 0 )
	{
		//TRACE2("Down : iDelta(%d), iPos(%d)\n", pNMUpDown->iDelta, pNMUpDown->iPos);
		nPos -= 1;
		if( nPos < nLower )
			nPos = nLower;

		::PostMessage(GetParent()->m_hWnd, WM_XSPINBOX_DELTA, (WPARAM)FALSE, (LPARAM)NULL);
	}
	// Up
	else if( pNMUpDown->iDelta > 0 )
	{
		//TRACE2("Up : iDelta(%d), iPos(%d)\n", pNMUpDown->iDelta, pNMUpDown->iPos);
		nPos += 1;
		if( nPos > nUpper )
			nPos = nUpper;

		::PostMessage(GetParent()->m_hWnd, WM_XSPINBOX_DELTA, (WPARAM)TRUE, (LPARAM)NULL);
	}
	else /*if( pNMUpDown->iDelta == 0 ) */
	{
		return;
	}

	int nResult = 0;
	::SendMessage(GetParent()->m_hWnd, WM_XSPINBOX_CHANGING, (WPARAM)nPos, (LPARAM)&nResult);

	if( nResult == 0 )
		SetPos(nPos);
}
//------------------------------------------------------------------------
// XSpinBox.cpp End
//------------------------------------------------------------------------






//------------------------------------------------------------------------
// XListCtrl.cpp Start
//------------------------------------------------------------------------
UINT NEAR WM_XLISTCTRL_EDITBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XLISTCTRL_EDITBOX_CHANGE"));
UINT NEAR WM_XLISTCTRL_DATEBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XLISTCTRL_DATEBOX_CHANGE"));
UINT NEAR WM_XLISTCTRL_COMBOBOX_CHANGE = ::RegisterWindowMessage(_T("WM_XLISTCTRL_COMBOBOX_CHANGE"));
UINT NEAR WM_XLISTCTRL_CHECKBOX_CLICKING = ::RegisterWindowMessage(_T("WM_XLISTCTRL_CHECKBOX_CLICKING"));
UINT NEAR WM_XLISTCTRL_CHANGE = ::RegisterWindowMessage(_T("WM_XLISTCTRL_CHANGE"));
UINT NEAR WM_XCTRL_SELECT = ::RegisterWindowMessage(_T("WM_XCTRL_SELECT"));

/////////////////////////////////////////////////////////////////////////////
// CXListCtrl

CXListCtrl::CXListCtrl()
{
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	m_nComboItem            = -1;
	m_nComboSubItem         = -1;
#endif

	m_nEditItem				= -1;
	m_nEditSubItem			= -1;

	m_nDateItem				= -1;
	m_nDateSubItem			= -1;

	m_dwExtendedStyleX      = 0;
	m_bHeaderIsSubclassed   = FALSE;

	m_cr3DFace              = ::GetSysColor(COLOR_3DFACE);
	m_cr3DHighLight         = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow            = ::GetSysColor(COLOR_3DSHADOW);
	m_crBtnFace             = ::GetSysColor(COLOR_BTNFACE); //RGB(240,240,240);
	m_crBtnShadow           = ::GetSysColor(COLOR_BTNSHADOW);
	m_crBtnText             = ::GetSysColor(COLOR_BTNTEXT);
	m_crGrayText            = ::GetSysColor(COLOR_GRAYTEXT);
	m_crHighLight           = ::GetSysColor(COLOR_HIGHLIGHT);//RGB(220,220,250);
	m_crHighLightText       = ::GetSysColor(COLOR_HIGHLIGHTTEXT);//RGB(0,0,0);
	m_crWindow              = ::GetSysColor(COLOR_WINDOW);//RGB(255,255,255);
	m_crWindowText          = ::GetSysColor(COLOR_WINDOWTEXT);
	m_crBorder				= RGB(0,0,0);//black color
	m_crSelalwaysText		= m_crWindowText;
	m_crSelalwaysTextBg		= m_crBtnFace;

	m_bFocusRect			= TRUE;
	m_bResize				= TRUE;
	//m_strNoItemMsg			= _T("There are no items to show in this view.");
	m_strNoItemMsg			= _T("");
	m_nStatusColumn			= -1;
	m_crBgProgress			= ::GetSysColor(COLOR_WINDOW);/*::GetSysColor(COLOR_BTNFACE);*/
	m_bUnderLine			= TRUE;
	m_crUnderLine			= RGB(0,0,0); //black color
	m_bHeaderTracking		= FALSE;
	m_bHeaderDraging		= FALSE;
	
	m_bUseTracking			= FALSE;
	m_bTracking				= FALSE;
	m_nTrackingItem			= -1;
	m_bSelAlwaysColor		= FALSE;
	m_crTracking			= ::GetSysColor(COLOR_GRAYTEXT);// RGB(230,250,230);

	m_pTextFont				= NULL;
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXListCtrl::~CXListCtrl()
{
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	CXComboBox::DeleteInstance();
#endif
	CXEditBox::DeleteInstance();
	CXDateBox::DeleteInstance();
}

///////////////////////////////////////////////////////////////////////////////
//
BEGIN_MESSAGE_MAP(CXListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CXListCtrl)
	ON_NOTIFY_REFLECT_EX(LVN_COLUMNCLICK, OnColumnClick)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ERASEBKGND()
	ON_WM_NCPAINT()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	//}}AFX_MSG_MAP

	ON_REGISTERED_MESSAGE(WM_XEDITBOX_CHANGE,OnEditChange)
	ON_REGISTERED_MESSAGE(WM_XDATEBOX_CHANGE,OnDateChange)

#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	ON_REGISTERED_MESSAGE(WM_XCOMBOBOX_CHANGE,OnComboChange)
#endif

#ifndef NO_XLISTCTRL_TOOL_TIPS
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
#endif	

END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CXListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// for Dialog based applications, this is a good place
	// to subclass the header control because the OnCreate()
	// function does not get called.

	SubclassHeaderControl();
}

///////////////////////////////////////////////////////////////////////////////
// OnCreate
int CXListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
	{
		ASSERT(FALSE);
		return -1;
	}

	// When the CXListCtrl object is created via a call to Create(), instead
	// of via a dialog box template, we must subclass the header control
	// window here because it does not exist when the PreSubclassWindow()
	// function is called.

	SubclassHeaderControl();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// SubclassHeaderControl
void CXListCtrl::SubclassHeaderControl()
{
	if (m_bHeaderIsSubclassed)
		return;

	// if the list control has a header control window, then
	// subclass it

	// Thanks to Alberto Gattegno and Alon Peleg젨and their article
	// "A Multiline Header Control Inside a CListCtrl" for easy way
	// to determine if the header control exists.

	CHeaderCtrl* pHeader = GetHeaderCtrl();
	if (pHeader)
	{
		VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd));
		m_bHeaderIsSubclassed = TRUE;
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnCustomDraw
void CXListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
	int nSubItem = pLVCD->iSubItem;

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.

		*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.

		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) pLVCD->nmcd.lItemlParam;
		if( !pXLCD )
			return;
		
		COLORREF crText  = pXLCD[nSubItem].crText;
		COLORREF crBkgnd = pXLCD[nSubItem].crBackground;
		
		if( !pXLCD[0].bEnabled )
			crText = m_crGrayText;

		// store the colors back in the NMLVCUSTOMDRAW struct
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;

		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);
		CRect rect;
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

		// Progress
		if(pXLCD && (pXLCD[nSubItem].bShowProgress))
			DrawProgress(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
		// ComboBox
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
		else if(pXLCD && (pXLCD[nSubItem].bCombo))
			DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
#endif
		// EditBox
		else if(pXLCD && (pXLCD[nSubItem].bEditBox))
			DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
		// DateBox
		else if( pXLCD && (pXLCD[nSubItem].bDateBox))
			DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
		// CheckBox
		else if(pXLCD && (pXLCD[nSubItem].nCheckedState != -1))
			DrawCheckbox(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
		// Default
		else
		{
			rect.left += DrawImage(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
			DrawText(nItem, nSubItem, pDC, crText, crBkgnd, rect, pXLCD);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// DrawProgress
void CXListCtrl::DrawProgress(int nItem,
							  int nSubItem,
							  CDC *pDC,
							  COLORREF crText,
							  COLORREF crBkgnd,
							  CRect& rect,
							  XLISTCTRLDATA *pXLCD)
{
	UNUSED_ALWAYS(nItem);

	ASSERT(pDC);
	ASSERT(pXLCD);
	
	CFont *pOldFont = pDC->SelectObject(m_pTextFont);

	CPen newPen(PS_SOLID, 1, m_crUnderLine);
	CPen *pOldPen = pDC->SelectObject(&newPen);	

	// Progress 전체(외부/내부) 바탕색 칠하기
	COLORREF crWindow = m_crWindow;
	COLORREF crWindowText = m_crWindowText;
	GetDrawColors(nItem, nSubItem, crWindowText, crWindow);
	pDC->FillSolidRect(rect, crWindow);

	if( m_bUnderLine )
	{
		pDC->MoveTo( rect.left-1, rect.bottom );
		pDC->LineTo( rect.right+1, rect.bottom );
	}
	rect.left += 1;
	rect.right -= 1;
	rect.top += 1;

	// fill interior with light gray
	CRect InteriorRect;
	InteriorRect = rect;
	InteriorRect.top += 1;
	InteriorRect.bottom -= 2;
	InteriorRect.left += 2;
	InteriorRect.right -= 1;

	// Progress 내부 배경 칠하기
	pDC->FillSolidRect(InteriorRect, m_crBgProgress);

	// Progress 테두리 그리기
	pDC->Draw3dRect(InteriorRect, RGB(200,200,200), RGB(200,200,200));

	if (pXLCD[nSubItem].nProgressPercent >= 0)
	{
		// draw progress bar and text
		CRect LeftRect;
		LeftRect = rect;
		LeftRect.left += 1;
		LeftRect.top += 1;

		int w = (LeftRect.Width() * pXLCD[nSubItem].nProgressPercent) / 100;
		if( w < 3 )
			LeftRect.right = LeftRect.left + 6;
		else
			LeftRect.right = LeftRect.left + w;

		LeftRect.top += 1;
		LeftRect.bottom -= 3;
		LeftRect.left += 2;
		LeftRect.right -= 2;
		
		if( pXLCD[nSubItem].nProgressPercent )
				pDC->FillSolidRect(LeftRect, pXLCD[nSubItem].crBar);

		if( pXLCD[nSubItem].nTextLeftmode == 1 )
		{
			CRect RectHalp;
			RectHalp = LeftRect;
			RectHalp.right = RectHalp.right/2; 

			if(pXLCD[nSubItem].nProgressPercent)
				pDC->FillSolidRect(RectHalp, RGB(102,153,255));
		}

		if( pXLCD[nSubItem].bShowProgressMessage )
		{
			CString str, format;
			format = pXLCD[nSubItem].strProgressMessage;
			if( format.IsEmpty() )
				str.Format(_T("%d%%"), pXLCD[nSubItem].nProgressPercent);
			else
				str.Format(format, pXLCD[nSubItem].nProgressPercent);

			pDC->SetBkMode(TRANSPARENT);

			CRect TextRect;
			TextRect = rect;
			TextRect.left -= 3;
			TextRect.DeflateRect(1, 1);			

			pDC->SetTextColor(crText);
			if(pXLCD[nSubItem].nTextLeftmode == 1)
				pDC->DrawText(str, &TextRect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
			else
				pDC->DrawText(str, &TextRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		}
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

#ifndef DO_NOT_INCLUDE_XCOMBOLIST

///////////////////////////////////////////////////////////////////////////////
// ShowComboBox
void CXListCtrl::ShowComboBox(int nItem, int nSubItem)
{

#ifdef _DEBUG
	DWORD dwExStyle = GetExtendedStyle();
	if ((dwExStyle & LVS_EX_FULLROWSELECT) == 0)
	{
		TRACE(_T("XListCtrl: combo boxes require LVS_EX_FULLROWSELECT style\n"));
		ASSERT(FALSE);
	}
#endif

	if(!m_bEnableSubCtrl)
		return;

	if( nItem<0 || nSubItem<0 )
		return;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if( !pXLCD )
		return;

	int nCnt = (int)pXLCD[nSubItem].sa.GetSize();
	if( !nCnt )
		return;

	CXComboBox::DeleteInstance();
	CXComboBox *pComboBox = CXComboBox::GetInstance();
	if( !pComboBox )
		return;	

	CRect rc;
	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rc);
	rc.left += 1;
	rc.right -= 1;

	int nShowRows = nCnt + 2;
	if(pXLCD[nSubItem].nComboListHeight)
		nShowRows = pXLCD[nSubItem].nComboListHeight;
	
	pComboBox->Create(WS_CHILD | CBS_DROPDOWNLIST | WS_VISIBLE | WS_VSCROLL, 
					  CRect(rc.left, rc.top, rc.right, rc.top+rc.Height()*nShowRows), 
					  this, 
					  NULL);

	for(int i=0; i<nCnt; i++ )
		pComboBox->AddString( pXLCD[nSubItem].sa.GetAt(i) );

	pComboBox->SelectString(0, GetItemText(nItem, nSubItem) );
	pComboBox->SetFont(m_pTextFont);
	pComboBox->SetFocus();

	::SendMessage( pComboBox->m_hWnd, CB_SETITEMHEIGHT, (WPARAM)-1, rc.Height()-2);

	m_nComboItem = nItem;
	m_nComboSubItem = nSubItem;
}
#endif


///////////////////////////////////////////////////////////////////////////////
// ShowEditBox
void CXListCtrl::ShowEditBox(int nItem, int nSubItem)
{

#ifdef _DEBUG
	DWORD dwExStyle = GetExtendedStyle();
	if ((dwExStyle & LVS_EX_FULLROWSELECT) == 0)
	{
		TRACE(_T("XListCtrl: combo boxes require LVS_EX_FULLROWSELECT style\n"));
		ASSERT(FALSE);
	}
#endif

	if(!m_bEnableSubCtrl)
		return;

	if( nItem<0 || nSubItem<0 )
		return;

	int nRow = nItem; 
	int nCol = nSubItem;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nRow);
	if( !pXLCD )
		return;

	CXEditBox::DeleteInstance();
	CXEditBox *pEditBox = CXEditBox::GetInstance();
	if( !pEditBox )
		return;	

	pEditBox->SetMinNum( pXLCD[nCol].nEditMinNum );
	pEditBox->SetMaxNum( pXLCD[nCol].nEditMaxNum );

	DWORD dwStyle = pXLCD[nCol].dwEditStyle;
	if( pXLCD[nCol].nEditMinNum >= 0 && pXLCD[nCol].nEditMaxNum >= 0 )
	{
		dwStyle |= ES_NUMBER;
	}

	CRect rc;
	GetSubItemRect(nRow, nCol, LVIR_BOUNDS, rc);
	rc.top += 1;
	rc.left += 2;
	rc.bottom -= 1;

	pEditBox->Create(dwStyle, rc, this, NULL);
	pEditBox->SetFont(m_pTextFont);
	pEditBox->SetLimitText(pXLCD[nCol].nEditLimitText);
	pEditBox->SetWindowText( GetItemText(nRow, nCol) );
	pEditBox->SetSel(0, -1);
	pEditBox->SetFocus();

	m_nEditItem = nRow;
	m_nEditSubItem = nCol;
}

///////////////////////////////////////////////////////////////////////////////
// ShowDateBox
void CXListCtrl::ShowDateBox(int nItem, int nSubItem)
{

#ifdef _DEBUG
	DWORD dwExStyle = GetExtendedStyle();
	if ((dwExStyle & LVS_EX_FULLROWSELECT) == 0)
	{
		TRACE(_T("XListCtrl: combo boxes require LVS_EX_FULLROWSELECT style\n"));
		ASSERT(FALSE);
	}
#endif

	if(!m_bEnableSubCtrl)
		return;

	if( nItem<0 || nSubItem<0 )
		return;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if( !pXLCD )
		return;

	CXDateBox::DeleteInstance();
	CXDateBox *pDateBox = CXDateBox::GetInstance();
	if( !pDateBox )
		return;	

	CRect rc;
	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rc);
	rc.left += 1;
	rc.right -= 1;
	rc.bottom += 4;

	pDateBox->Create(pXLCD[nSubItem].dwDateStyle, rc, this, NULL);
	pDateBox->SetFont(m_pTextFont);
	pDateBox->SetFormat(pXLCD[nSubItem].sDateFormat);
	pDateBox->SetTime(&pXLCD[nSubItem].date);
	pDateBox->SetFocus();

	m_nDateItem = nItem;
	m_nDateSubItem = nSubItem;
}

///////////////////////////////////////////////////////////////////////////////
// DrawCheckbox
void CXListCtrl::DrawCheckbox(int nItem,
							  int nSubItem,
							  CDC *pDC,
							  COLORREF crText,
							  COLORREF crBkgnd,
							  CRect& rect,
							  XLISTCTRLDATA *pXLCD)
{
	ASSERT(pDC);
	ASSERT(pXLCD);

	CFont *pOldFont = pDC->SelectObject(m_pTextFont);
	CPen newPen(PS_SOLID, 1, m_crUnderLine);
	CPen *pOldPen = pDC->SelectObject(&newPen);	

	GetDrawColors(nItem, nSubItem, crText, crBkgnd);

	pDC->FillSolidRect(&rect, crBkgnd);

	if( m_bUnderLine )
	{
		pDC->MoveTo( rect.left-1, rect.bottom );
		pDC->LineTo( rect.right+1, rect.bottom );
	}
	rect.left += 1;
	rect.right -= 1;
	rect.top += 1;
	//rect.bottom -= 1;

	CRect chkboxrect;
	chkboxrect = rect;
	chkboxrect.top += 1;
	chkboxrect.bottom -= 1;
	chkboxrect.left += 2;		// line up checkbox with header checkbox
	chkboxrect.right = chkboxrect.left + chkboxrect.Height();	// width = height

	CString str(_T(""));
	str = GetItemText(nItem, nSubItem);

	if (str.IsEmpty())
	{
		// center the checkbox
		chkboxrect.left = rect.left + rect.Width()/2 - chkboxrect.Height()/2 - 1;
		chkboxrect.right = chkboxrect.left + chkboxrect.Height();
	}

	// fill rect around checkbox with white
	pDC->FillSolidRect(&chkboxrect, m_crWindow);

	chkboxrect.left += 1;

	// draw border
	pDC->DrawEdge(&chkboxrect, EDGE_SUNKEN, BF_RECT);

	if (pXLCD[nSubItem].nCheckedState == 1)
	{
		CPen *pOldPen = NULL;

		CPen graypen(PS_SOLID, 1, m_crGrayText);
		CPen blackpen(PS_SOLID, 1, RGB(0,0,0));

		if (pXLCD[0].bEnabled)
			pOldPen = pDC->SelectObject(&blackpen);
		else
			pOldPen = pDC->SelectObject(&graypen);

		// draw the checkmark
		int x = chkboxrect.left + 9;
		ASSERT(x < chkboxrect.right);
		int y = chkboxrect.top + 3;
		int i;
		for (i = 0; i < 4; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y++;
		}
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y--;
		}

		if (pOldPen)
			pDC->SelectObject(pOldPen);
	}

	if (!str.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		CRect textrect;
		textrect = rect;
		textrect.left = chkboxrect.right + 4;

		pDC->DrawText(str, &textrect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// GetDrawColors
void CXListCtrl::GetDrawColors(int nItem,
							   int nSubItem,
							   COLORREF& colorText,
							   COLORREF& colorBkgnd)
{
	DWORD dwStyle    = GetStyle();
	DWORD dwExStyle  = GetExtendedStyle();

	COLORREF crText  = colorText;
	COLORREF crBkgnd = colorBkgnd;

	if (GetItemState(nItem, LVIS_SELECTED) && m_bFocusRect)
	{
		if (dwExStyle & LVS_EX_FULLROWSELECT)
		{
			// selected?  if so, draw highlight background
			crText  = m_crHighLightText;
			crBkgnd = m_crHighLight;

			// has focus?  if not, draw gray background
			if (m_hWnd != ::GetFocus())
			{
				if (dwStyle & LVS_SHOWSELALWAYS) // LVS_SHOWSELALWAYS : 아이템을 선택후 다른 컨트롤에 포커스가 옮겨가도 선택된것을 표시하는 플래그.
				{
					if(m_bSelAlwaysColor)
					{
						crText  = m_crSelalwaysText;
						crBkgnd = m_crSelalwaysTextBg;
					}
					else
					{
						crText  = colorText;
						crBkgnd = m_crBtnFace;
					}					
				}
				else
				{
					crText  = colorText;
					crBkgnd = colorBkgnd;
				}
			}
		}
	}

	// 해당 Item이 Tracking Item과 동일하면 배경 색을 Tracking Color로 저장한다.
	if( m_nTrackingItem >= 0 && m_bUseTracking)
	{
		if( nItem == m_nTrackingItem )
			crBkgnd = m_crTracking;
	}

	colorText = crText;
	colorBkgnd = crBkgnd;
}

///////////////////////////////////////////////////////////////////////////////
// DrawImage
int CXListCtrl::DrawImage(int nItem,
						  int nSubItem,
						  CDC* pDC,
						  COLORREF crText,
						  COLORREF crBkgnd,
						  CRect rect,
  						  XLISTCTRLDATA *pXLCD)
{
	GetDrawColors(nItem, nSubItem, crText, crBkgnd);

	pDC->FillSolidRect(&rect, crBkgnd);

	CPen newPen(PS_SOLID, 1, m_crUnderLine);
	CPen *pOldPen = pDC->SelectObject(&newPen);	
	if( m_bUnderLine )
	{
		pDC->MoveTo( rect.left-1, rect.bottom );
		pDC->LineTo( rect.right+1, rect.bottom );
	}

	int nWidth = 0;

	CImageList* pImageList = GetImageList(LVSIL_NORMAL);
	if(pImageList)
	{
		SIZE sizeImage;
		sizeImage.cx = sizeImage.cy = 0;
		IMAGEINFO info;

		int nImage = -1;
		if (pXLCD)
			nImage = pXLCD[nSubItem].nImage;

		if (nImage == -1)
			return 0;

		if (pImageList->GetImageInfo(nImage, &info))
		{
			sizeImage.cx = info.rcImage.right - info.rcImage.left;
			sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
		}
		
		if (nImage >= 0)
		{
			if (rect.Width() > 0)
			{
				if( pXLCD[nSubItem].bImageCenter )
					rect.left += rect.Width()/2-sizeImage.cx/2;
				else
					rect.left += m_HeaderCtrl.GetSpacing();

				POINT pt;					
				pt.y = rect.CenterPoint().y - (sizeImage.cy >> 1);
				pt.x = rect.left;
				
				SIZE size;
				size.cx = rect.Width() < sizeImage.cx ? rect.Width() : sizeImage.cx;
				size.cy = rect.Height() < sizeImage.cy ? rect.Height() : sizeImage.cy;

				COLORREF rgb = pImageList->GetBkColor();	// save image list background color					
				pImageList->SetBkColor(crBkgnd);			// set image list background color
				pImageList->DrawIndirect(pDC, nImage, pt, size, CPoint(0, 0));
				pImageList->SetBkColor(rgb);
			
				if( pXLCD[nSubItem].bImageCenter )
					nWidth = rect.right;
				else
					nWidth = sizeImage.cx + m_HeaderCtrl.GetSpacing();
			}
		}
	}
	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();

	return nWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
void CXListCtrl::DrawText(int nItem,
						  int nSubItem,
						  CDC *pDC,
						  COLORREF crText,
						  COLORREF crBkgnd,
						  CRect& rect,
						  XLISTCTRLDATA *pXLCD)
{
	ASSERT(pDC);
	ASSERT(pXLCD);

	CRect rcNew;
	rcNew = rect;

	CFont *pOldFont = pDC->SelectObject(m_pTextFont);
	CPen newPen(PS_SOLID, 1, m_crUnderLine);
	CPen *pOldPen = pDC->SelectObject(&newPen);

	if( nSubItem == m_nStatusColumn )
	{
		COLORREF crTemp = m_crWindow;
		GetDrawColors(nItem, nSubItem, crText, crTemp);

		pDC->FillSolidRect(&rect, crTemp);
		rcNew.top	+= 1;
		rcNew.left	+= 2;
		rcNew.right	-= 2;
		rcNew.bottom-= 2;
		pDC->FillSolidRect(&rcNew, crBkgnd);
	}
	else
	{
		GetDrawColors(nItem, nSubItem, crText, crBkgnd);
		pDC->FillSolidRect(&rect, crBkgnd);
	}

	CString str(_T(""));
	str = GetItemText(nItem, nSubItem);	
	if (!str.IsEmpty())
	{
		// get text justification
		HDITEM hditem;
		hditem.mask = HDI_FORMAT;
		m_HeaderCtrl.GetItem(nSubItem, &hditem);
		int nFmt = hditem.fmt & HDF_JUSTIFYMASK;
		UINT nFormat = DT_VCENTER | DT_SINGLELINE;
		//if (nFmt == HDF_CENTER)
		//{
		//	nFormat |= DT_CENTER;
		//}
		//else if (nFmt == HDF_LEFT)
		//{
		//	nFormat |= DT_LEFT;
		//	rcNew.left += 4;
		//}
		//else
		//{
		//	nFormat |= DT_RIGHT;
		//}
		nFormat |= DT_CENTER;

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		rcNew.OffsetRect(0, 1);
		pDC->DrawText(str, &rcNew, nFormat);
	}

	if( m_bUnderLine )
	{
		pDC->MoveTo( rect.left-1, rect.bottom );
		pDC->LineTo( rect.right+1, rect.bottom );
	}

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	newPen.DeleteObject();
}

///////////////////////////////////////////////////////////////////////////////
// GetSubItemRect
BOOL CXListCtrl::GetSubItemRect(int nItem,
								int nSubItem,
								int nArea,
								CRect& rect)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL bRC = CListCtrl::GetSubItemRect(nItem, nSubItem, nArea, rect);

	// if nSubItem == 0, the rect returned by CListCtrl::GetSubItemRect
	// is the entire row, so use left edge of second subitem
	if (nSubItem == 0)
	{
		if (GetColumns() > 1)
		{
			CRect rect1;
			bRC = GetSubItemRect(nItem, 1, LVIR_BOUNDS, rect1);
			rect.right = rect1.left;
		}
	}

	// 마우스가 올라왔을때 Grid line이 지워지는 문제를 보완
	rect.bottom -= 1;

	return bRC;
}

BOOL CXListCtrl::SelectItem(int nItem, int nSubItem)
{
	if(nItem < 0 || nSubItem < 0)
		return FALSE;
	if(nSubItem > GetColumns() || nItem > GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
		return FALSE;

	if( !pXLCD[nSubItem].bEnabled )
		return FALSE;

	//LVITEM lvi;
	//lvi.stateMask = nStateMask;
	//lvi.state = nState;
	//SendMessage(m_hWnd, LVM_SETITEMSTATE, nItem, (LPARAM)&lvi);
	SetItemState(nItem, LVIS_SELECTED, LVIS_SELECTED);	
	
	// Check Box
	if( pXLCD[nSubItem].nCheckedState >= 0 )
	{
		pXLCD[nSubItem].nCheckedState = !pXLCD[nSubItem].nCheckedState;
		UpdateSubItem(nItem, nSubItem);

		CWnd *pWnd = GetParent();
		if( !pWnd )
			pWnd = GetOwner();

		if( pWnd && ::IsWindow(pWnd->m_hWnd) )
		{
			NMITEM NMI;
			NMI.hwndFrom = m_hWnd;
			NMI.iItem = nItem;
			NMI.iSubItem = nSubItem;
			NMI.sText = _T("");
			pWnd->SendMessage(WM_XLISTCTRL_CHECKBOX_CLICKING, (WPARAM)&NMI, (LPARAM)pXLCD[nSubItem].nCheckedState);
			pWnd->PostMessage(WM_XLISTCTRL_CHANGE, (WPARAM)MAKELONG(nSubItem,nItem), (LPARAM)m_hWnd);
		}

		// now update checkbox in header
		// -1 = no checkbox in column header
		if( GetHeaderCheckedState(nSubItem) != XHEADERCTRL_NO_IMAGE )
		{
			int nCheckedCount = CountCheckedItems(nSubItem);

			if( nCheckedCount == GetItemCount() )
				SetHeaderCheckedState(nSubItem, XHEADERCTRL_CHECKED_IMAGE);
			else
				SetHeaderCheckedState(nSubItem, XHEADERCTRL_UNCHECKED_IMAGE);
		}
		return TRUE;
	}
	// Combo Box
#ifndef DO_NOT_INCLUDE_XCOMBOLIST
	else if( pXLCD[nSubItem].bCombo )
	{
		ShowComboBox(nItem, nSubItem);
		return TRUE;
	}
#endif
	// Edit Box
	else if( pXLCD[nSubItem].bEditBox )
	{
		ShowEditBox(nItem, nSubItem);
		return TRUE;
	}
	// Date Box
	else if( pXLCD[nSubItem].bDateBox )
	{
		ShowDateBox(nItem, nSubItem);
		return TRUE;
	}
	return FALSE;
}
///////////////////////////////////////////////////////////////////////////////
// OnLButtonDown
void CXListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
//	TRACE(_T("in CXListCtrl::OnLButtonDown\n"));

	//
	int nItem = -1;
	CRect rect;
	for(int i=0; i<GetItemCount(); i++)
	{
		if (CListCtrl::GetItemRect(i, &rect, LVIR_BOUNDS))
		{
			if (rect.PtInRect(point))
			{
				nItem = i;
				break;
			}
		}
	}

	if( nItem != -1 )
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
		if (!pXLCD)
			return;

		if( !pXLCD[0].bEnabled )
			return;	

		// check if a subitem checkbox was clicked
		CRect rect;
		int nSubItem = -1;
		for( int i=0; i<GetColumns(); i++ )
		{
			GetSubItemRect(nItem, i, LVIR_BOUNDS, rect);
			if (rect.PtInRect(point))
			{
				nSubItem = i;
				break;
			}
		}
		if(SelectItem(nItem, nSubItem))
			return;
	}
	CListCtrl::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CXListCtrl::OnPaint()
{
	Default();
	
	if(GetItemCount() <= 0)
	{
		CDC* pDC = GetDC();
		int nSavedDC = pDC->SaveDC();

		CRect rc;
		GetWindowRect(&rc);
		ScreenToClient(&rc);
		CHeaderCtrl* pHC = GetHeaderCtrl();
		if (pHC != NULL)
		{
			CRect rcH;
			pHC->GetItemRect(0, &rcH);
			rc.top += rcH.bottom;
		}

		CString strText = m_strNoItemMsg;
		COLORREF crText = m_crWindowText;
		COLORREF crBkgnd = m_crWindow;		
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		pDC->SelectStockObject(ANSI_VAR_FONT);
		pDC->FillSolidRect(&rc, crBkgnd);

		rc.top += 10;
		pDC->DrawText(strText, -1, rc, DT_CENTER | DT_WORDBREAK | DT_NOPREFIX | DT_NOCLIP);

		pDC->RestoreDC(nSavedDC);
		ReleaseDC(pDC);
	}
	m_HeaderCtrl.RedrawWindow();
}

///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL CXListCtrl::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default	
#if 1
	CRect clip;
	m_HeaderCtrl.GetWindowRect(&clip);		// get rect of the control
	ScreenToClient(&clip);
	pDC->ExcludeClipRect(&clip);
	pDC->GetClipBox(&clip);
	pDC->FillSolidRect(clip, m_crWindow);
	return TRUE;
#else
	return TRUE;	
#endif
	return CListCtrl::OnEraseBkgnd(pDC);
}

///////////////////////////////////////////////////////////////////////////////
// InsertItem
int CXListCtrl::InsertItem(const LVITEM* pItem)
{
	ASSERT(pItem->iItem >= 0);
	if (pItem->iItem < 0)
		return -1;

	int index = CListCtrl::InsertItem(pItem);

	if (index < 0)
		return index;

	XLISTCTRLDATA *pXLCD = new XLISTCTRLDATA [GetColumns()];
	ASSERT(pXLCD);
	if (!pXLCD)
		return -1;

	pXLCD[0].crText       = m_crWindowText;
	pXLCD[0].crBackground = m_crWindow;
	pXLCD[0].nImage       = pItem->iImage;

	CListCtrl::SetItemData(index, (DWORD_PTR) pXLCD);

	return index;
}

///////////////////////////////////////////////////////////////////////////////
// InsertItem
int CXListCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	ASSERT(nItem >= 0);
	if (nItem < 0)
		return -1;

	return InsertItem(nItem,
					  lpszItem,
					  m_crWindowText,
					  m_crWindow);
}

///////////////////////////////////////////////////////////////////////////////
// InsertItem
int CXListCtrl::InsertItem(int nItem,
						   LPCTSTR lpszItem,
						   COLORREF crText,
						   COLORREF crBackground)
{
	ASSERT(nItem >= 0);
	if (nItem < 0)
		return -1;

	int index = CListCtrl::InsertItem(nItem, lpszItem);

	if (index < 0)
		return index;

	XLISTCTRLDATA *pXLCD = new XLISTCTRLDATA [GetColumns()];
	ASSERT(pXLCD);
	if (!pXLCD)
		return -1;

	pXLCD[0].crText       = crText;
	pXLCD[0].crBackground = crBackground;
	CListCtrl::SetItemData(index, (DWORD_PTR) pXLCD);

	return index;
}

///////////////////////////////////////////////////////////////////////////////
// SetItem
int CXListCtrl::SetItem(const LVITEM* pItem)
{
	ASSERT(pItem->iItem >= 0);
	if (pItem->iItem < 0)
		return -1;

	BOOL rc = CListCtrl::SetItem(pItem);

	if (!rc)
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(pItem->iItem);
	if (pXLCD)
	{
		pXLCD[pItem->iSubItem].nImage = pItem->iImage;
		UpdateSubItem(pItem->iItem, pItem->iSubItem);
		rc = TRUE;
	}
	else
	{
		rc = FALSE;
	}

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// GetItemImage
int CXListCtrl::GetItemImage(int nItem, int nSubItem)
{
	int nImage = -1;

	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return nImage;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return nImage;	

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
		nImage = pXLCD[nSubItem].nImage;

	return nImage;
}

///////////////////////////////////////////////////////////////////////////////
// SetItemImage
BOOL CXListCtrl::SetItemImage(int nItem, int nSubItem, int nImage, BOOL bImageCenter/*=FALSE*/)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL rc = TRUE;

	if (nItem < 0)
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
	{
		// 기존 데이터와 비교후 틀리면 업데이트
		if( pXLCD[nSubItem].nImage == nImage && 
			pXLCD[nSubItem].bImageCenter == bImageCenter )
			return rc;

		pXLCD[nSubItem].nImage = nImage;
		pXLCD[nSubItem].bImageCenter = bImageCenter;
	}

	UpdateSubItem(nItem, nSubItem);

	return rc;
}

BOOL CXListCtrl::SetItemInt(int nItem, int nSubItem, int nValue)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	char str[256] = {0,};
	_itoa(nValue,str,10);

	return SetItemText(nItem, nSubItem, (LPCTSTR)str);
}

BOOL CXListCtrl::SetItemInt(int nItem, int nSubItem, int nValue,
					COLORREF crText, COLORREF crBackground)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	char str[256] = {0,};
	_itoa(nValue,str,10);
	
	return SetItemText(nItem, nSubItem, (LPCTSTR)str, crText, crBackground);
}

///////////////////////////////////////////////////////////////////////////////
// SetItemText
BOOL CXListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL rc = TRUE;

	if (nItem < 0)
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);

	// 기존 데이터와 비교후 틀리면 업데이트
	if( !pXLCD[nSubItem].strProgressMessage.Compare( lpszText ))
		return FALSE;

	pXLCD[nSubItem].strProgressMessage = lpszText;

	if (lpszText)
		rc = CListCtrl::SetItemText(nItem, nSubItem, lpszText);
	
	UpdateSubItem(nItem, nSubItem);

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// SetItemText
//
// This function will set the text and colors for a subitem.  If lpszText
// is NULL, only the colors will be set.  If a color value is -1, the display
// color will be set to the default Windows color.
//
BOOL CXListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText,
					COLORREF crText, COLORREF crBackground)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL rc = TRUE;

	if (nItem < 0)
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);

	// 기존 데이터와 비교후 틀리면 업데이트
	if( !pXLCD[nSubItem].strProgressMessage.Compare( lpszText ) && 
		pXLCD[nSubItem].crBackground == crBackground &&
		pXLCD[nSubItem].crText == crText )
		return FALSE;

	pXLCD[nSubItem].strProgressMessage = lpszText;

	if (lpszText)
		rc = CListCtrl::SetItemText(nItem, nSubItem, lpszText);
	
	if (pXLCD)
	{
		pXLCD[nSubItem].crText       = (crText == -1) ? m_crWindowText : crText;
		pXLCD[nSubItem].crBackground = (crBackground == -1) ? m_crWindow : crBackground;
	}

	UpdateSubItem(nItem, nSubItem);

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// SetItemTextColor
//
BOOL CXListCtrl::SetItemTextColor(int nItem, int nSubItem, COLORREF crText, COLORREF crBackground)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	if (nItem < 0)
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);

	// 기존 데이터와 비교후 틀리면 업데이트
	if( pXLCD[nSubItem].crBackground == crBackground &&
		pXLCD[nSubItem].crText == crText )
		return FALSE;

	if (pXLCD)
	{
		pXLCD[nSubItem].crText       = (crText == -1) ? m_crWindowText : crText;
		pXLCD[nSubItem].crBackground = (crBackground == -1) ? m_crWindow : crBackground;
	}

	UpdateSubItem(nItem, nSubItem);

	return TRUE;
}

// 아이템을 선택후 다른 컨트롤에 포커스가 옮겨가도 선택된 아이템의 텍스트를 지정.
BOOL CXListCtrl::SetItemTextSelAlwaysColor(COLORREF crText, COLORREF crBackground)
{
	m_bSelAlwaysColor = TRUE;
	m_crSelalwaysText = crText;
	m_crSelalwaysTextBg = crBackground;

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// DeleteItem
BOOL CXListCtrl::DeleteItem(int nItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
		delete [] pXLCD;
	CListCtrl::SetItemData(nItem, 0);
	return CListCtrl::DeleteItem(nItem);
}

///////////////////////////////////////////////////////////////////////////////
// DeleteAllItems
BOOL CXListCtrl::DeleteAllItems()
{
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(i);
		if (pXLCD)
			delete [] pXLCD;
		CListCtrl::SetItemData(i, 0);
	}

	return CListCtrl::DeleteAllItems();
}

///////////////////////////////////////////////////////////////////////////////
// OnDestroy
void CXListCtrl::OnDestroy()
{
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(i);
		if (pXLCD)
			delete [] pXLCD;
		CListCtrl::SetItemData(i, 0);
	}

	CListCtrl::OnDestroy();
}

///////////////////////////////////////////////////////////////////////////////
// SetProgress
//
// This function creates a progress bar in the specified subitem.  The
// UpdateProgress function may then be called to update the progress
// percent.  If bShowProgressText is TRUE, either the default text
// of "n%" or the custom percent text (lpszProgressText) will be
// displayed.  If bShowProgressText is FALSE, only the progress bar
// will be displayed, with no text.
//
// Note that the lpszProgressText string should include the format
// specifier "%d":  e.g., "Pct %d%%"
//
BOOL CXListCtrl::SetProgress(int nItem,
							 int nSubItem,
							 BOOL bShowProgressText /*= TRUE*/,
							 LPCTSTR lpszProgressText /*= NULL*/)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	pXLCD[nSubItem].bShowProgress        = TRUE;
	pXLCD[nSubItem].nProgressPercent     = 0;
	pXLCD[nSubItem].bShowProgressMessage = bShowProgressText;
	pXLCD[nSubItem].strProgressMessage   = lpszProgressText;

	UpdateSubItem(nItem, nSubItem);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// DeleteProgress
void CXListCtrl::DeleteProgress(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
		return;

	pXLCD[nSubItem].bShowProgress = FALSE;
	pXLCD[nSubItem].nProgressPercent = 0;

	UpdateSubItem(nItem, nSubItem);
}

///////////////////////////////////////////////////////////////////////////////
// UpdateProgress
void CXListCtrl::UpdateProgress(int nItem, int nSubItem, int nPercent, COLORREF crText, COLORREF crBar, CString ProgressText/*=_T("")*/)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return;

	ASSERT(nPercent >= 0 && nPercent <= 100);

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
		return;

	// 기존 데이터와 비교후 틀리면 업데이트
	if( ! pXLCD[nSubItem].strProgressMessage.Compare( ProgressText ) && 
		  pXLCD[nSubItem].nProgressPercent == nPercent &&
		  pXLCD[nSubItem].crBar == crBar )
		return;

	pXLCD[nSubItem].nProgressPercent = nPercent;
	pXLCD[nSubItem].crBar = crBar;
	pXLCD[nSubItem].crText = crText;
	pXLCD[nSubItem].strProgressMessage = ProgressText;

	UpdateSubItem(nItem, nSubItem);
}

///////////////////////////////////////////////////////////////////////////////
// UpdateProgress
void CXListCtrl::UpdateProgressLeftText(int nItem, int nSubItem, int nPercent, COLORREF	crBar, CString ProgressText, int nTextLeftmode)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return;

	ASSERT(nPercent >= 0 && nPercent <= 100);

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
		return;

	// 기존 데이터와 비교후 틀리면 업데이트
	if( ! pXLCD[nSubItem].strProgressMessage.Compare( ProgressText ) && 
		  pXLCD[nSubItem].nProgressPercent == nPercent &&
		  pXLCD[nSubItem].crBar == crBar )
		return;

	pXLCD[nSubItem].nProgressPercent = nPercent;
	pXLCD[nSubItem].crBar = crBar;
	pXLCD[nSubItem].strProgressMessage = ProgressText;
	pXLCD[nSubItem].nTextLeftmode = nTextLeftmode;

	UpdateSubItem(nItem, nSubItem);
}

///////////////////////////////////////////////////////////////////////////////
// SetCheckbox
BOOL CXListCtrl::SetCheckbox(int nItem, int nSubItem, int nCheckedState)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;
	ASSERT(nCheckedState == 0 || nCheckedState == 1 || nCheckedState == -1);

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update checkbox in subitem
	pXLCD[nSubItem].nCheckedState = nCheckedState;
	UpdateSubItem(nItem, nSubItem);

	// now update checkbox in column header
	// -1 = no checkbox in column header
	if (GetHeaderCheckedState(nSubItem) != XHEADERCTRL_NO_IMAGE)
	{
		int nCheckedCount = CountCheckedItems(nSubItem);

		if (nCheckedCount == GetItemCount())
			SetHeaderCheckedState(nSubItem, XHEADERCTRL_CHECKED_IMAGE);
		else
			SetHeaderCheckedState(nSubItem, XHEADERCTRL_UNCHECKED_IMAGE);
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetCheckbox
int CXListCtrl::GetCheckbox(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return -1;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return -1;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return -1;
	}

	return pXLCD[nSubItem].nCheckedState;
}

///////////////////////////////////////////////////////////////////////////////
// GetEnabled
//
// Note that GetEnabled and SetEnabled only Get/Set the enabled flag from
// subitem 0, since this is a per-row flag.
//
BOOL CXListCtrl::GetEnabled(int nItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	return pXLCD[0].bEnabled;
}

///////////////////////////////////////////////////////////////////////////////
// SetEnabled
BOOL CXListCtrl::SetEnabled(int nItem, BOOL bEnable)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	pXLCD[0].bEnabled = bEnable;

	CRect rect;
	GetItemRect(nItem, &rect, LVIR_BOUNDS);
	InvalidateRect(&rect);
	UpdateWindow();

	return TRUE;
}

BOOL CXListCtrl::SetSubItemEnabled(int nItem, int nSubItem, BOOL bEnable)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
		return FALSE;

	pXLCD[nSubItem].bEnabled = bEnable;

	CRect rect;
	GetItemRect(nItem, &rect, LVIR_BOUNDS);
	InvalidateRect(&rect);
	UpdateWindow();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// SetBold
BOOL CXListCtrl::SetBold(int nItem, int nSubItem, BOOL bBold)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update bold flag
	pXLCD[nSubItem].bBold = bBold;

	UpdateSubItem(nItem, nSubItem);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetBold
BOOL CXListCtrl::GetBold(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update bold flag
	return pXLCD[nSubItem].bBold;
}

///////////////////////////////////////////////////////////////////////////////
// SetDateBox
BOOL CXListCtrl::SetDateBox(int nItem, 
							int nSubItem, 
							BOOL bDateBox/*=TRUE*/, 
							CString sDateFormat/*=_T("yyyy-MM-dd_HH:mm:ss")*/,
							DWORD dwDateStyle/*=WS_VISIBLE|WS_CHILD|WS_TABSTOP|DTS_SHORTDATEFORMAT*/)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	pXLCD[nSubItem].bDateBox = bDateBox;

	if( !sDateFormat.IsEmpty() )
		pXLCD[nSubItem].sDateFormat = sDateFormat;

	if( dwDateStyle != NULL )
		pXLCD[nSubItem].dwDateStyle = dwDateStyle;

	return TRUE;
}

void CXListCtrl::UpdateDate(int nItem, int nSubItem, CTime time, COLORREF crText, COLORREF crBackground)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
		return;

	if( !pXLCD[nSubItem].bDateBox )
		return;

	// 기존 데이터와 비교후 틀리면 업데이트
	if( pXLCD[nSubItem].date.GetTime() == time.GetTime() &&
		pXLCD[nSubItem].crBackground == crBackground &&
		pXLCD[nSubItem].crText == crText )
		return;

	pXLCD[nSubItem].date		 = time;
	pXLCD[nSubItem].crText       = (crText == -1) ? m_crWindowText : crText;
	pXLCD[nSubItem].crBackground = (crBackground == -1) ? m_crWindow : crBackground;

	CString sDate = CXDateBox::GetDate(time, pXLCD[nSubItem].sDateFormat);

	CListCtrl::SetItemText( nItem, nSubItem, sDate );	

	UpdateSubItem(nItem, nSubItem);
}

///////////////////////////////////////////////////////////////////////////////
// SetEditBox
BOOL CXListCtrl::SetEditBox(int nItem, 
							int nSubItem, 
							BOOL bEditBox/*=TRUE*/, 
							int nEditMinNum/*=-1*/,
							int nEditMaxNum/*=-1*/,
							int nEditLimitText/*=-1*/,
							BOOL bEditChColor/*=FALSE*/,
							COLORREF crChangeText/*=NULL*/,
							DWORD dwEditStyle/*=ES_AUTOHSCROLL|ES_AUTOVSCROLL|ES_LEFT|ES_NOHIDESEL|WS_CHILD|WS_VISIBLE*/)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	pXLCD[nSubItem].bEditBox = bEditBox;

	if( nEditLimitText != -1 )
		pXLCD[nSubItem].nEditLimitText = nEditLimitText;

	if( nEditMinNum != -1 )
		pXLCD[nSubItem].nEditMinNum = nEditMinNum;

	if( nEditMaxNum != -1 )
		pXLCD[nSubItem].nEditMaxNum = nEditMaxNum;

	if( dwEditStyle != NULL )
		pXLCD[nSubItem].dwEditStyle = dwEditStyle;

	pXLCD[nSubItem].bEditChColor = bEditChColor;

	if( crChangeText != NULL )
		pXLCD[nSubItem].crChangeText = crChangeText;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// SetComboBox
//
// Note:  SetItemText may also be used to set the initial combo selection.
//
BOOL CXListCtrl::SetComboBox(int nItem,
							 int nSubItem,
							 BOOL bEnableCombo,
							 CStringArray *psa/*=NULL*/)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	pXLCD[nSubItem].bCombo = bEnableCombo;

	if( psa )
	{
		pXLCD[nSubItem].sa.RemoveAll();
		int nCnt = (int)psa->GetSize();
		int i=0;
		for(i=0; i<nCnt; i++)
			pXLCD[nSubItem].sa.Add( psa->GetAt(i) );
	}
		
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetComboText
//
// Actually this does nothing more than GetItemText()
//
CString	CXListCtrl::GetComboText(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return _T("");
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return _T("");

	CString str(_T(""));
	str = GetItemText(nItem, nSubItem);

	return str;
}

CString CXListCtrl::GetComboText(int nItem, int nSubItem,int nIndex)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return _T("");
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return _T("");

	
	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return _T("");
	}

	// update flag
	if(!pXLCD[nSubItem].bCombo)
        return FALSE;
	
	int nCnt = (int)pXLCD[nSubItem].sa.GetCount();
	if(nIndex > nCnt)
		return _T("");

	
	CString str(_T(""));
	str = pXLCD[nSubItem].sa.GetAt(nIndex);

	return str;
}

///////////////////////////////////////////////////////////////////////////////
// GetComboText. 091203
int CXListCtrl::GetComboCurSel(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	
	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	if(!pXLCD[nSubItem].bCombo)
        return FALSE;

	CString str(_T(""));
	str = GetItemText(nItem, nSubItem);


	int nCnt = (int)pXLCD[nSubItem].sa.GetCount();
	for(int i = 0; i < nCnt; i++)
	{
		if( pXLCD[nSubItem].sa.GetAt(i) == str)
			return i;
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// GetComboText. 091203
int CXListCtrl::GetComboDropDownRows(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	
	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	if(!pXLCD[nSubItem].bCombo)
        return FALSE;

	return pXLCD[nSubItem].nComboListHeight;
}

int CXListCtrl::GetComboItemCount(int nItem, int nSubItem)
{
	
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	
	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	if(!pXLCD[nSubItem].bCombo)
        return FALSE;

	int nCount = (int)pXLCD[nSubItem].sa.GetCount();
	
	return nCount;
}

///////////////////////////////////////////////////////////////////////////////
// SetCurSel
BOOL CXListCtrl::SetCurSel(int nItem)
{
	return SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,
		LVIS_FOCUSED | LVIS_SELECTED);
}

///////////////////////////////////////////////////////////////////////////////
// SetComboCurSel
BOOL CXListCtrl::SetComboCurSel(int nItem, int nSubItem, int nCurSel)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	
	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	if(!pXLCD[nSubItem].bCombo)
        return FALSE;

	int nCnt = (int)pXLCD[nSubItem].sa.GetCount();
	if( nCurSel > (nCnt - 1) )
		return FALSE;

	CString str = pXLCD[nSubItem].sa.GetAt(nCurSel);
	SetItemText(nItem,nSubItem,str);

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////
// SetComboDropDownRows
BOOL CXListCtrl::SetComboDropDownRows(int nItem, int nSubItem,int nRows)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	
	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	if(!pXLCD[nSubItem].bCombo)
        return FALSE;

	pXLCD[nSubItem].nComboListHeight = nRows;

	return TRUE;
}

BOOL CXListCtrl::SetEditBoxAlign(int nItem, int nSubItem, DWORD dwAlign /* = ES_LEFT */)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	pXLCD[nSubItem].dwEditStyle &= ~ES_LEFT;
	pXLCD[nSubItem].dwEditStyle &= ~ES_RIGHT;
	pXLCD[nSubItem].dwEditStyle &= ~ES_CENTER;

	pXLCD[nSubItem].dwEditStyle |= dwAlign; 

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// AddStringCombo
BOOL CXListCtrl::AddStringCombo(int nItem, int nSubItem,CString str)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	
	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	if(!pXLCD[nSubItem].bCombo)
        return FALSE;

	pXLCD[nSubItem].sa.Add(str);
	
	return TRUE;
}

BOOL CXListCtrl::DelStringCombo(int nItem, int nSubItem,int nIndex)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	
	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	if(!pXLCD[nSubItem].bCombo)
        return FALSE;

	if(nIndex > pXLCD[nSubItem].sa.GetCount() )
		return FALSE;

	pXLCD[nSubItem].sa.RemoveAt(nIndex);
	
	return TRUE;
}

BOOL CXListCtrl::DelStringCombo(int nItem, int nSubItem,CString str)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	
	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	if(!pXLCD[nSubItem].bCombo)
        return FALSE;

	CString TargetStr=_T("");
	int nCount = (int)pXLCD[nSubItem].sa.GetCount();
	for(int i = 0; i < nCount; i++)
	{
		TargetStr = pXLCD[nSubItem].sa.GetAt(i);
		if(str == TargetStr)
		{
			pXLCD[nSubItem].sa.RemoveAt(i);
			break;
		}
	}

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
// SetColumnWidthPercent
void CXListCtrl::ResizeListColumn(int arPercent[])
{
	SCROLLINFO si;
	CRect rc;
	GetWindowRect(&rc);
	int nMargin=0;

	// 스크롤 유무에 따른 마진 추가
	GetScrollInfo(SB_VERT, &si);	
	if( si.nPage && si.nPage <= (UINT)si.nMax ) 
	{
		nMargin = ::GetSystemMetrics(SM_CXVSCROLL);
	}

	// 보더 유무에 따른 마진 추가
	DWORD dwExStyle = GetExStyle();
	if( dwExStyle&WS_EX_STATICEDGE || 
		dwExStyle&WS_EX_CLIENTEDGE )
	{
		nMargin += ::GetSystemMetrics(SM_CXEDGE)*2;
	}

	int nWidth = 0;
	int nLastWidth = rc.Width()-nMargin;
	for( int i=0; ; i++ )
	{
		// 마지막 컬럼일 경우
		if( arPercent[i] == 0 )
		{
			SetColumnWidth(i, nLastWidth);
			break;
		}

		nWidth = (rc.Width()*arPercent[i])/100;
		SetColumnWidth(i, nWidth);
		nLastWidth -= nWidth;
	}
	RedrawWindow();
}

void CXListCtrl::SetHigLightColor(COLORREF crHighLight,COLORREF crHighLightText)
{
	m_crHighLight     = crHighLight;
	m_crHighLightText = crHighLightText;
}

void CXListCtrl::SetTrackingColor(COLORREF crTracking)
{
	m_crTracking = crTracking;
}

void CXListCtrl::SetBorderColor(COLORREF crBorder)
{
	m_crBorder = crBorder;
}

void CXListCtrl::SetTracking(BOOL bTracking)
{
	m_bUseTracking = bTracking;
}

///////////////////////////////////////////////////////////////////////////////
// GetCurSel - returns selected item number, or -1 if no item selected
//
// Note:  for single-selection lists only
//
int CXListCtrl::GetCurSel()
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = GetNextSelectedItem(pos);
	return nSelectedItem;
}

///////////////////////////////////////////////////////////////////////////////
// UpdateSubItem
void CXListCtrl::UpdateSubItem(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return;

	CRect rect;
	if (nSubItem == -1)
		GetItemRect(nItem, &rect, LVIR_BOUNDS);
	else
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

	InvalidateRect(&rect);
	UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////
// GetColumns
int CXListCtrl::GetColumns()
{
	return GetHeaderCtrl()->GetItemCount();
}

///////////////////////////////////////////////////////////////////////////////
// GetItemData
//
// The GetItemData and SetItemData functions allow for app-specific data
// to be stored, by using an extra field in the XLISTCTRLDATA struct.
//
DWORD CXListCtrl::GetItemData(int nItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return 0;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return 0;
	}

	return pXLCD->dwItemData;
}

///////////////////////////////////////////////////////////////////////////////
// SetItemData
BOOL CXListCtrl::SetItemData(int nItem, DWORD dwData)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	pXLCD->dwItemData = dwData;

	return TRUE;
}

int CXListCtrl::GetItemInt(int nItem, int nSubItem)
{
	return atoi( GetItemText(nItem,nSubItem) );
}

///////////////////////////////////////////////////////////////////////////////
// GetHeaderCheckedState
//
// The GetHeaderCheckedState and SetHeaderCheckedState may be used to toggle
// the checkbox in a column header.
//     0 = no checkbox
//     1 = unchecked
//     2 = checked
//
int CXListCtrl::GetHeaderCheckedState(int nSubItem)
{
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return -1;

	HDITEM hditem;

	// use the image index (0 or 1) to indicate the checked status
	hditem.mask = HDI_IMAGE;
	m_HeaderCtrl.GetItem(nSubItem, &hditem);
	return hditem.iImage;
}

///////////////////////////////////////////////////////////////////////////////
// SetHeaderCheckedState
BOOL CXListCtrl::SetHeaderCheckedState(int nSubItem, int nCheckedState)
{
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;
	ASSERT(nCheckedState == 0 || nCheckedState == 1 || nCheckedState == 2);

	HDITEM hditem;

	hditem.mask = HDI_IMAGE;
	hditem.iImage = nCheckedState;
	m_HeaderCtrl.SetItem(nSubItem, &hditem);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// OnColumnClick
BOOL CXListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;

	int nSubItem = pnmlv->iSubItem;

	int nCheckedState = GetHeaderCheckedState(nSubItem);

	// 0 = no checkbox
	if (nCheckedState != XHEADERCTRL_NO_IMAGE)
	{
		nCheckedState = (nCheckedState == 1) ? 2 : 1;
		SetHeaderCheckedState(nSubItem, nCheckedState);

		m_HeaderCtrl.UpdateWindow();

		for (int n = 0; n < GetItemCount(); n++)
		{
			XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(n);
			if (!pXLCD)
			{
				continue;
			}

			if (pXLCD[nSubItem].nCheckedState != -1)
			{
				pXLCD[nSubItem].nCheckedState = nCheckedState - 1;
				UpdateSubItem(n, nSubItem);
			}
		}
	}

	*pResult = 0;
	return FALSE;		// return FALSE to send message to parent also -
						// NOTE:  MSDN documentation is incorrect
}

///////////////////////////////////////////////////////////////////////////////
// CountCheckedItems
int CXListCtrl::CountCheckedItems(int nSubItem)
{
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return 0;

	int nCount = 0;

	for (int n = 0; n < GetItemCount(); n++)
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(n);
		if (!pXLCD)
		{
			continue;
		}

		if (pXLCD[nSubItem].nCheckedState == 1)
			nCount++;
	}

	return nCount;
}

///////////////////////////////////////////////////////////////////////////////
// OnSysColorChange
void CXListCtrl::OnSysColorChange()
{
	TRACE(_T("in CXListCtrl::OnSysColorChange\n"));

	CListCtrl::OnSysColorChange();

	m_cr3DFace        = ::GetSysColor(COLOR_3DFACE);
	m_cr3DHighLight   = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow      = ::GetSysColor(COLOR_3DSHADOW);
	m_crBtnFace       = RGB(240,240,240);//::GetSysColor(COLOR_BTNFACE);
	m_crBtnShadow     = ::GetSysColor(COLOR_BTNSHADOW);
	m_crBtnText       = ::GetSysColor(COLOR_BTNTEXT);
	m_crGrayText      = ::GetSysColor(COLOR_GRAYTEXT);
	m_crHighLight     = RGB(245,245,245);//::GetSysColor(COLOR_HIGHLIGHT);//RGB(220,220,250);//
	m_crHighLightText = RGB(140,140,140);//::GetSysColor(COLOR_HIGHLIGHTTEXT);//RGB(0,0,0);//
	m_crWindow        = RGB(255,255,255);//::GetSysColor(COLOR_WINDOW);
	m_crWindowText    = ::GetSysColor(COLOR_WINDOWTEXT);
}

void CXListCtrl::SetLButtonDown(int nStartItem, int nStartSubItem)
{
	BOOL bFind = FALSE;
	int nCntItem = GetItemCount();
	int nCntSubItem = GetColumns();
	int i = nStartItem;
	int k = nStartSubItem+1;
	for( ; i<nCntItem; i++, k=0 )
	{
		for(; k<nCntSubItem; k++)
		{
			XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *)CListCtrl::GetItemData(i);
			if( pXLCD )
			{
				if( !pXLCD[k].bEditBox && !pXLCD[k].bDateBox && !pXLCD[k].bCombo )
					continue;
			
				CRect rc;
				GetSubItemRect(i, k, LVIR_BOUNDS, rc);
				CPoint pt(rc.left+rc.Width()/2, rc.top+rc.Height()/2);
				PostMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(pt.x, pt.y));
				bFind = TRUE;
				return;
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnEditChange
LRESULT CXListCtrl::OnEditChange(WPARAM wParam, LPARAM lParam)
{
	if( m_nEditItem == -1 || m_nEditSubItem == -1 )
		return 0L;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *)CListCtrl::GetItemData(m_nEditItem);
	if( !pXLCD )
		return 0L;

	CXEditBox *pEditBox = CXEditBox::GetInstance();
	if(pEditBox == NULL || pEditBox->m_hWnd == NULL)
		return 0L;	
	
	CString sText(_T(""));
	pEditBox->GetWindowText(sText);	

	if( pXLCD[m_nEditSubItem].nEditMinNum >= 0 &&
		pXLCD[m_nEditSubItem].nEditMaxNum >= 0 )
	{
		int nEditNum = _tstoi(sText);
		if( nEditNum < pXLCD[m_nEditSubItem].nEditMinNum )
		{
			CString sMsg(_T(""));
			sMsg.Format(_T("입력값이 %d 이상 이어야 합니다."), pXLCD[m_nEditSubItem].nEditMinNum);
			AfxMessageBox(sMsg);
			return 0L;
			
		}
		else if( nEditNum > pXLCD[m_nEditSubItem].nEditMaxNum )
		{
			CString sMsg(_T(""));
			sMsg.Format(_T("입력값이 %d 이하 이어야 합니다."), pXLCD[m_nEditSubItem].nEditMaxNum);
			AfxMessageBox(sMsg);
			return 0L;
		}
	}

	if(pXLCD[m_nEditSubItem].bEditChColor)
	{
		if(sText != GetItemText(m_nEditItem, m_nEditSubItem))
			SetItemText(m_nEditItem, m_nEditSubItem, sText,pXLCD[m_nEditSubItem].crChangeText, pXLCD[m_nEditSubItem].crBackground);
	}
	else
		SetItemText(m_nEditItem, m_nEditSubItem, sText);

	UpdateSubItem(m_nEditItem, m_nEditSubItem);

	CWnd *pWnd = GetParent();
	if( !pWnd )
		pWnd = GetOwner();
	
	if( pWnd && ::IsWindow(pWnd->m_hWnd) )
	{
		pWnd->SendMessage(WM_XLISTCTRL_EDITBOX_CHANGE, (WPARAM)m_nEditItem, (LPARAM)m_nEditSubItem);
		pWnd->PostMessage(WM_XLISTCTRL_CHANGE, (WPARAM)MAKELONG(m_nEditSubItem, m_nEditItem), (LPARAM)m_hWnd);
	}

	// Edit Box에서 탭버튼을 클릭했으면 리스트의 다음 컨트롤(EditBox,ComboBox,DateBox)을 활성화 시킨다.
	if( wParam == VK_TAB )
		SetLButtonDown(m_nEditItem, m_nEditSubItem);

	//////////////////////////////////////////////////////////////////////////
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return 0L;
	}

	pDoc->ChangeXListCtrl((DWORD)lParam,m_nEditItem,m_nEditSubItem,1);
	//////////////////////////////////////////////////////////////////////////

	m_nEditItem = -1;
	m_nEditSubItem = -1;

	return 0L;
}

///////////////////////////////////////////////////////////////////////////////
// OnEditChange
LRESULT CXListCtrl::OnDateChange(WPARAM wParam, LPARAM lParam)
{
	SetFocus();

	if( m_nDateItem == -1 || m_nDateSubItem == -1 )
		return 0L;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *)CListCtrl::GetItemData(m_nDateItem);
	if( !pXLCD )
		return 0L;

	CXDateBox *pDateBox = CXDateBox::GetInstance();
	if(pDateBox == NULL || pDateBox->m_hWnd == NULL)
		return 0L;

	if( wParam == VK_ESCAPE || wParam == VK_DELETE )
		return 0L;

	pDateBox->GetTime( pXLCD[m_nDateSubItem].date );
	
	CString sText(_T(""));
	pDateBox->GetWindowText(sText);
	SetItemText(m_nDateItem, m_nDateSubItem, sText);

	UpdateSubItem(m_nDateItem, m_nDateSubItem);

	CWnd *pWnd = GetParent();
	if( !pWnd )
		pWnd = GetOwner();

	if( pWnd && ::IsWindow(pWnd->m_hWnd) )
	{
		pWnd->SendMessage(WM_XLISTCTRL_DATEBOX_CHANGE, m_nDateItem, m_nDateSubItem);
		pWnd->PostMessage(WM_XLISTCTRL_CHANGE, (WPARAM)MAKELONG(m_nDateSubItem, m_nDateItem), (LPARAM)m_hWnd);
	}

	// Date Box에서 탭버튼을 클릭했으면 리스트의 다음 컨트롤(EditBox,ComboBox,DateBox)을 활성화 시킨다.
	if( wParam == VK_TAB )
		SetLButtonDown(m_nDateItem, m_nDateSubItem);

	//////////////////////////////////////////////////////////////////////////
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return 0L;
	}

	pDoc->ChangeXListCtrl((DWORD)lParam,m_nDateItem,m_nDateSubItem,3);
	//////////////////////////////////////////////////////////////////////////

	m_nDateItem = -1;
	m_nDateSubItem = -1;

	return 0L;
}

#ifndef DO_NOT_INCLUDE_XCOMBOLIST
///////////////////////////////////////////////////////////////////////////////
// OnComboChange
LRESULT CXListCtrl::OnComboChange(WPARAM wParam, LPARAM lParam)
{
	if( m_nComboItem == -1 || m_nComboSubItem == -1 )
		return 0L;

	CXComboBox *pComboBox = CXComboBox::GetInstance();
	if(pComboBox == NULL || pComboBox->m_hWnd == NULL)
		return 0L;

	int nIndex = pComboBox->GetCurSel();
	if( nIndex < 0 )
		return 0L;

	CString sText(_T(""));
	pComboBox->GetLBText(nIndex, sText);
	SetItemText(m_nComboItem, m_nComboSubItem, sText);
	UpdateSubItem(m_nComboItem, m_nComboSubItem);

	CWnd *pWnd = GetParent();
	if( !pWnd )
		pWnd = GetOwner();

	if( pWnd && ::IsWindow(pWnd->m_hWnd) )
	{
		pWnd->SendMessage(WM_XLISTCTRL_COMBOBOX_CHANGE, m_nComboItem, m_nComboSubItem);
		pWnd->PostMessage(WM_XLISTCTRL_CHANGE, (WPARAM)MAKELONG(m_nComboSubItem, m_nComboItem), (LPARAM)m_hWnd);
	}

	// Combo Box에서 탭버튼을 클릭했으면 리스트의 다음 컨트롤(EditBox,ComboBox,DateBox)을 활성화 시킨다.
	if( wParam == VK_TAB )
		SetLButtonDown(m_nComboItem, m_nComboSubItem);

	//////////////////////////////////////////////////////////////////////////
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CHappyDoc* pDoc = (CHappyDoc*)pFrame->GetActiveDocument();
	if(!pDoc)
	{
		pDoc = (CHappyDoc *)pFrame->GetFirstDocument();
		if(!pDoc)
			return 0L;
	}

	pDoc->ChangeXListCtrl((DWORD)lParam,m_nComboItem,m_nComboSubItem,2);
	//////////////////////////////////////////////////////////////////////////

	m_nComboItem = -1;
	m_nComboSubItem = -1;

	return 0L;
}
#endif

#ifndef NO_XLISTCTRL_TOOL_TIPS

///////////////////////////////////////////////////////////////////////////////
// OnToolHitTest
int CXListCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	LVHITTESTINFO lvhitTestInfo;
	
	lvhitTestInfo.pt = point;
	
	int nRow = ListView_SubItemHitTest(this->m_hWnd, &lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;
//	TRACE(_T("in CToolTipListCtrl::OnToolHitTest: %d,%d\n"), nRow, nSubItem);

	UINT nFlags = lvhitTestInfo.flags;

	// nFlags is 0 if the SubItemHitTest fails
	// Therefore, 0 & <anything> will equal false
	if (nFlags & LVHT_ONITEMLABEL)
	{
		// If it did fall on a list item,
		// and it was also hit one of the
		// item specific subitems we wish to show tool tips for
		
		// get the client (area occupied by this control
		RECT rcClient;
		GetClientRect(&rcClient);
		
		// fill in the TOOLINFO structure
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT) (nRow * 1000 + nSubItem + 1);
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rcClient;
		
		return (int)pTI->uId;	// By returning a unique value per listItem,
							// we ensure that when the mouse moves over another
							// list item, the tooltip will change
	}
	else
	{
		//Otherwise, we aren't interested, so let the message propagate
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnToolTipText
BOOL CXListCtrl::OnToolTipText(UINT /*id*/, NMHDR * pNMHDR, LRESULT * pResult)
{
	UINT nID = (UINT)pNMHDR->idFrom;
//	TRACE(_T("in CXListCtrl::OnToolTipText: id=%d\n"), nID);
	
	// check if this is the automatic tooltip of the control
	if (nID == 0) 
		return TRUE;	// do not allow display of automatic tooltip,
						// or our tooltip will disappear
	
	// handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	*pResult = 0;
	
	// get the mouse position
	const MSG* pMessage;
	pMessage = GetCurrentMessage();
	ASSERT(pMessage);
	CPoint pt;
	pt = pMessage->pt;		// get the point from the message
	ScreenToClient(&pt);	// convert the point's coords to be relative to this control
	
	// see if the point falls onto a list item
	
	LVHITTESTINFO lvhitTestInfo;
	
	lvhitTestInfo.pt = pt;
	
	int nItem = SubItemHitTest(&lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;
	
	UINT nFlags = lvhitTestInfo.flags;
	
	// nFlags is 0 if the SubItemHitTest fails
	// Therefore, 0 & <anything> will equal false
	if (nFlags & LVHT_ONITEMLABEL)
	{
		// If it did fall on a list item,
		// and it was also hit one of the
		// item specific subitems we wish to show tooltips for
		
		CString strToolTip;
		strToolTip = _T("");

		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
		if (pXLCD)
		{
			strToolTip = pXLCD[nSubItem].strToolTip;
		}

		if (!strToolTip.IsEmpty())
		{
			// If there was a CString associated with the list item,
			// copy it's text (up to 80 characters worth, limitation 
			// of the TOOLTIPTEXT structure) into the TOOLTIPTEXT 
			// structure's szText member
			
#ifndef _UNICODE
			if (pNMHDR->code == TTN_NEEDTEXTA)
				lstrcpyn(pTTTA->szText, strToolTip, 80);
			else
				_mbstowcsz(pTTTW->szText, strToolTip, 80);
#else
			if (pNMHDR->code == TTN_NEEDTEXTA)
				_wcstombsz(pTTTA->szText, strToolTip, 80);
			else
				lstrcpyn(pTTTW->szText, strToolTip, 80);
#endif
			return FALSE;	 // we found a tool tip,
		}
	}
	
	return FALSE;	// we didn't handle the message, let the 
					// framework continue propagating the message
}

///////////////////////////////////////////////////////////////////////////////
// SetItemToolTipText
BOOL CXListCtrl::SetItemToolTipText(int nItem, int nSubItem, LPCTSTR lpszToolTipText)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	pXLCD[nSubItem].strToolTip = lpszToolTipText;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetItemToolTipText
CString CXListCtrl::GetItemToolTipText(int nItem, int nSubItem)
{
	CString strToolTip;
	strToolTip = _T("");

	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return strToolTip;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return strToolTip;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
	{
		strToolTip = pXLCD[nSubItem].strToolTip;
	}

	return strToolTip;
}

///////////////////////////////////////////////////////////////////////////////
// DeleteAllToolTips
void CXListCtrl::DeleteAllToolTips()
{
	int nRow = GetItemCount();
	int nCol = GetColumns();

	for (int n = 0; n < nRow; n++)
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(n);
		if (pXLCD)
			for (int nSubItem = 0; nSubItem < nCol; nSubItem++)
				pXLCD[nSubItem].strToolTip = _T("");
	}
}

#endif


BOOL CXListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class

	HD_NOTIFY *pHDN = (HD_NOTIFY*)lParam;

	// 컬럼 사이즈가 20 이하가 안되게 한다.
	if( (pHDN->hdr.code == HDN_ITEMCHANGINGW || pHDN->hdr.code == HDN_ITEMCHANGINGA) 
		&& pHDN->pitem->cxy < 20 )		
    {		
        *pResult = TRUE;                // disable change		
        return TRUE;                    // Processed message		
    }

	switch( pHDN->hdr.code/*((NMHDR*)lParam)->code*/ )
	{
	case HDN_BEGINTRACKW:	
	case HDN_BEGINTRACKA:	m_bHeaderTracking=TRUE;		break;
	case HDN_ENDTRACKW :	
	case HDN_ENDTRACKA :	m_bHeaderTracking=FALSE;	break;
	case HDN_BEGINDRAG :	m_bHeaderDraging=TRUE;		break;
	case HDN_ENDDRAG :		m_bHeaderDraging=FALSE;		break;
	}

	if( m_bHeaderTracking )
	{
		// 마우스의 트래킹 방지
		if( !m_bResize )
		{
			*pResult = TRUE;							
			return TRUE;
		}
	}

	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

void CXListCtrl::SetRowHeight(int nRowHeight)
{
	CFont font;
	LOGFONT logfont;

	CDC *pDC = GetDC();
	if( !pDC ) return;

	int nRevision=7;

	logfont.lfHeight = -MulDiv(nRowHeight-nRevision, GetDeviceCaps(pDC->m_hDC, LOGPIXELSY), 72);
	ReleaseDC( pDC );

	logfont.lfEscapement	= 0;
	logfont.lfOrientation	= 0;
	logfont.lfWeight		= FW_NORMAL;
	logfont.lfItalic		= FALSE;
	logfont.lfUnderline		= FALSE;
	logfont.lfStrikeOut		= FALSE;
	logfont.lfCharSet		= ANSI_CHARSET;
	logfont.lfOutPrecision	= OUT_DEFAULT_PRECIS;
	logfont.lfClipPrecision	= CLIP_DEFAULT_PRECIS;
	logfont.lfQuality		= PROOF_QUALITY;
	logfont.lfPitchAndFamily= DEFAULT_PITCH;
	_tcscpy(logfont.lfFaceName, _T("System"));
	font.CreateFontIndirect(&logfont);

	SetFont(&font);

	font.DeleteObject();
}

BOOL CXListCtrl::GetProgressColor(int nItem, int nSubItem, COLORREF &cf)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
		return FALSE;

	cf = pXLCD[nSubItem].crBar;
	return TRUE;
}

void CXListCtrl::Sort(int nSubItem, BOOL bSort)
{
	if( GetItemCount() <= 0 )
		return;

	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if( nSubItem < 0 || nSubItem >= GetColumns() )
		return;

	SetRedraw(FALSE);

	CString str(_T(""));
    int nCount = GetItemCount();
    for(int i=0; i<nCount; i++) 
    {
		DWORD dw = GetItemData(i);
		str = GetItemText(i, nSubItem) + GetItemText(i, 0 );
		SetItemData( i, (DWORD)new CSortData(str, dw) );
    }

	CListCtrl::SortItems(&CompareFunc, bSort);

    for(int i=0; i<nCount; i++)
    {
        CSortData* pItem = (CSortData*)GetItemData(i);
        ASSERT( pItem );
        SetItemData(i, pItem->m_dw);
        delete pItem;
    }

	SetRedraw(TRUE);
}

int CALLBACK CXListCtrl::CompareFunc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// XLIST 사용시 캐스팅
	XLISTCTRLDATA *pXLCD1 = (XLISTCTRLDATA *)lParam1;
	XLISTCTRLDATA *pXLCD2 = (XLISTCTRLDATA *)lParam2;
	if(!pXLCD1 || !pXLCD2)
		return 0;

	// ITEMDATA
    CSortData *pLc1 = (CSortData*)pXLCD1->dwItemData;
	CSortData *pLc2 = (CSortData*)pXLCD2->dwItemData;
	if(!pLc1 || !pLc2)
		return 0;

    int nReturn = pLc1->m_strText.Compare(pLc2->m_strText);

    if(lParamSort)
        return nReturn;
    else
        return -nReturn;
}

BOOL CXListCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class

	if (pMsg->message == WM_MOUSEMOVE && GetFocus() == this )
	{
		// 현재 마우스 위치를 가져온다.
		CRect rect;
		POINT point;
		::GetCursorPos(&point);
		::ScreenToClient(m_hWnd, &point);	

		// 현재 마우스 위치에 해당하는 Item을 가져온다.
		LVHITTESTINFO lvhitTestInfo;	
		lvhitTestInfo.pt = point;	
		int nItem = SubItemHitTest(&lvhitTestInfo);
		int nSubItem = lvhitTestInfo.iSubItem;

		// nItem과 nSubItem의 값이 정상이고 현재 커서가 일반커서이면 다음을 수행한다.
		if( nItem >= 0 && nSubItem >= 0 &&
			nItem != m_nTrackingItem &&
			GetCursor() == AfxGetApp()->LoadStandardCursor(IDC_ARROW) )
		{			
			// Tracking 플래그를 설정하고 마우스 포인터가 윈도우를 벗어났을때
			// WM_MOUSELEAVE 이벤트가 발생하도록 설정한다.
			if(!m_bTracking) 
			{
				TRACKMOUSEEVENT t = { sizeof(TRACKMOUSEEVENT), TME_LEAVE, m_hWnd, 0 };
				if (::_TrackMouseEvent(&t)) 
				{
					m_bTracking = TRUE;
				}
			}
			
			// 이전 Tracking Item을 Invalidate 한다.		
			if( m_nTrackingItem >= 0 && m_bUseTracking )
			{				
				GetItemRect(m_nTrackingItem, &rect, LVIR_BOUNDS);
				InvalidateRect(&rect, FALSE);
			}			

			// 새로운 Tracking Item을 저장하고 Invalite 한다.
			m_nTrackingItem = nItem;
			GetItemRect(nItem, &rect, LVIR_BOUNDS);			
			InvalidateRect(&rect, FALSE);
		}

		//m_HeaderCtrl.SendMessage(WM_MOUSEMOVE, pMsg->wParam, pMsg->lParam);
		//return TRUE;
	}

	return CListCtrl::PreTranslateMessage(pMsg);
}

LRESULT CXListCtrl::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	// Tracking 플래그를 클리어한다.
	// Tracking Item을 Invalidate한다.
	// Tracking Item을 클리어한다.
	CRect rect;
	m_bTracking = FALSE;	
	if( m_nTrackingItem >= 0 && m_bUseTracking )
	{
		GetItemRect(m_nTrackingItem, &rect, LVIR_BOUNDS);
		InvalidateRect(&rect, FALSE);
		m_nTrackingItem = -1;
	}
	return 0L;
}

void CXListCtrl::OnNcPaint()
{
	Default();

	DWORD dwExStyle = GetExStyle();
	if( dwExStyle&WS_EX_STATICEDGE ||
		dwExStyle&WS_EX_CLIENTEDGE )
	{
		CRect rc;
		GetWindowRect(&rc);

		//Non-Client 영역의 DC를 얻어온다.
		CWindowDC dc(this);
		dc.Draw3dRect(0, 0, rc.Width(), rc.Height(), m_crBorder, m_crBorder);
	}
}
//------------------------------------------------------------------------
// XListCtrl.cpp End
//------------------------------------------------------------------------

