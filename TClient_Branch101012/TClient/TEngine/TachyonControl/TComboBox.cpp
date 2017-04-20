#include "stdafx.h"
#include "TComboBox.h"
#include "TPaintManager.h"
#include "TMemoryDC.h"
#include "TPopupMenuWnd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CTComboEditCtrlHook

CTComboEditCtrlHook::CTComboEditCtrlHook()
{
}

CTComboEditCtrlHook::~CTComboEditCtrlHook()
{
}

CTComboBox *CTComboEditCtrlHook::GetTComboBox()
{
	HWND hWndParent = ::GetParent(GetSafeHwnd());
	CTComboBox *pCombo = DYNAMIC_DOWNCAST(
		CTComboBox,
		FromHandlePermanent(hWndParent));

	return pCombo;
}


IMPLEMENT_DYNCREATE( CTComboEditCtrlHook, CEdit)

BEGIN_MESSAGE_MAP( CTComboEditCtrlHook, CEdit)
	//{{AFX_MSG_MAP(CTComboEditCtrlHook)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


LRESULT CTComboEditCtrlHook::WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	BOOL bFlushAutoComplete = FALSE;

	switch(message)
	{
	case WM_CONTEXTMENU	:
		if(GetTComboBox()->OnTrackComboContextMenu(GetSafeHwnd()))
			return 0;

		break;

	case WM_CUT			:
	case WM_COPY		:
	case WM_PASTE		:
	case WM_CLEAR		:
	case WM_UNDO		: bFlushAutoComplete = TRUE; break;
	case WM_COMMAND		:
		switch(LOWORD(wParam))
		{
		case ID_EDIT_CLEAR			:
		case ID_EDIT_CLEAR_ALL		:
		case ID_EDIT_COPY			:
		case ID_EDIT_CUT			:
		case ID_EDIT_FIND			:
		case ID_EDIT_PASTE			:
		case ID_EDIT_PASTE_LINK		:
		case ID_EDIT_PASTE_SPECIAL	:
		case ID_EDIT_REPEAT			:
		case ID_EDIT_REPLACE		:
		case ID_EDIT_SELECT_ALL		:
		case ID_EDIT_UNDO			:
		case ID_EDIT_REDO			: bFlushAutoComplete = TRUE; break;
		}

		break;
	}

	if(bFlushAutoComplete)
		GetTComboBox()->m_bAutoComplete = FALSE;

	return CEdit::WindowProc( message, wParam, lParam);
}


/////////////////////////////////////////////////////////////////////////////
// CTComboBox

CTComboBox::CTComboBox() : m_bAutoComplete(TRUE),
	m_bEnableAutoComplete(TRUE),
	m_bDrawing(FALSE),
	m_dwUpdateTimer(10),
	m_dwUpdatePeriod(50),
	m_pInnerEditHook(NULL)
{
}

CTComboBox::~CTComboBox()
{
	if(m_pInnerEditHook)
		delete m_pInnerEditHook;
}


IMPLEMENT_DYNCREATE( CTComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CTComboBox, CComboBox)
	//{{AFX_MSG_MAP(CTComboBox)
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_CONTROL_REFLECT( CBN_EDITUPDATE, OnEditCtrlUpdate)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CTComboBox message handlers

void CTComboBox::OnTimer( UINT nIDEvent)
{
	if( nIDEvent != m_dwUpdateTimer )
	{
		CComboBox::OnTimer(nIDEvent);
		return;
	}

	POINT ptCursorPos;
	GetCursorPos(&ptCursorPos);

	CRect rcItem;
	GetWindowRect(&rcItem);

	if(CTPopupMenuWnd::IsKeyPressed(MK_LBUTTON))
	{
		if(!GetDroppedState())
			KillTimer(m_dwUpdateTimer);

		if(m_bDrawing)
		{
			_OnDrawComboImpl(
				GetDroppedState(),
				FALSE);

			m_bDrawing = FALSE;
		}

		return;
	}

	if(!rcItem.PtInRect(ptCursorPos))
	{
		if(!GetDroppedState())
			KillTimer(m_dwUpdateTimer);

		_OnDrawComboImpl( FALSE, FALSE);
		m_bDrawing = FALSE;

		return;
	}

	if(m_bDrawing)
		return;

	m_bDrawing = TRUE;
	_OnDrawComboImpl( FALSE, TRUE);
}

void CTComboBox::_OnPaintImpl( BOOL bPressed, BOOL bHover)
{
	if(GetStyle()&(CBS_OWNERDRAWFIXED|CBS_OWNERDRAWVARIABLE))
	{
		Default();
		CClientDC dc(this);

		_OnDrawComboImpl(
			bPressed,
			bHover,
			&dc);
	}
	else
	{
		CRect rcClient;
		GetClientRect(&rcClient);

		CPaintDC dcPaint(this);
		CTPaintManager::ExcludeChildAreas(
			dcPaint.GetSafeHdc(),
			GetSafeHwnd());

		CTMemoryDC dcmm( &dcPaint, &rcClient);
		DefWindowProc( WM_PAINT, (WPARAM) dcmm.GetSafeHdc(), (LPARAM) 0);
		_OnDrawComboImpl( bPressed, bHover, &dcmm);
	}
}

void CTComboBox::OnPaint()
{
	if(GetExStyle()&(WS_EX_DLGMODALFRAME|WS_EX_CLIENTEDGE|WS_EX_STATICEDGE))
	{
		ModifyStyleEx(
			WS_EX_DLGMODALFRAME|
			WS_EX_CLIENTEDGE|
			WS_EX_STATICEDGE,
			0, SWP_FRAMECHANGED);
	}

	CPoint ptCursorPos;
	GetCursorPos(&ptCursorPos);

	CRect rcItem;
	GetWindowRect(&rcItem);

	BOOL bHover = FALSE;
	if(rcItem.PtInRect(ptCursorPos))
	{
		if(m_bDrawing)
			return;

		bHover = TRUE;
	}

	_OnPaintImpl( FALSE, bHover);
}

void CTComboBox::_OnDrawComboImpl( BOOL bPressed, BOOL bHover, CDC *pDC)
{
	CRect rectClient;
	GetClientRect(&rectClient);

	BOOL bCallReleaseDC = FALSE;
	if(!pDC)
	{
		pDC = GetDC();
		bCallReleaseDC = TRUE;
	}

	BOOL bEnabled = IsWindowEnabled();
	BOOL bPushed = bPressed || GetDroppedState();

	if(!bEnabled)
	{
		bPushed = FALSE;
		bHover = FALSE;
	}

	if(CTPopupMenuWnd::IsMenuTracking())
		bHover = FALSE;
	else if(!bHover)
	{
		HWND hWndFocus = ::GetFocus();

		if( hWndFocus == GetSafeHwnd() || ::IsChild( GetSafeHwnd(), hWndFocus) )
			bHover = TRUE;
	}

	g_PaintManager->PaintComboFrame(
		*pDC,
		rectClient,
		bHover,
		bPushed,
		bEnabled);

	if(bCallReleaseDC)
		ReleaseDC(pDC);
}

BOOL CTComboBox::PreTranslateMessage( MSG* pMsg)
{
	if(m_bEnableAutoComplete)
	{
		if( pMsg->message == WM_SYSCHAR )
			return TRUE;

		if( pMsg->message == WM_KEYDOWN )
		{
			m_bAutoComplete = TRUE;
			int nVirtKey = INT(pMsg->wParam);

			if( nVirtKey == VK_DELETE || nVirtKey == VK_BACK )
				m_bAutoComplete = FALSE;
		}
	}
	else
		m_bAutoComplete = FALSE;

	return CComboBox::PreTranslateMessage(pMsg);
}

void CTComboBox::OnEditCtrlUpdate()
{
	if(!m_bEnableAutoComplete)
	{
		Default();
		return;
	}

	if(!m_bAutoComplete)
		return;

	CString strValue;
	GetWindowText(strValue);
	int nLength = strValue.GetLength();

	DWORD dwCurSel = GetEditSel();
	WORD wStart = LOWORD(dwCurSel);
	WORD wEnd = HIWORD(dwCurSel);

	if( SelectString( -1, strValue) == CB_ERR )
	{
		SetWindowText(strValue);

		if( dwCurSel != CB_ERR )
			SetEditSel( wStart, wEnd);
	}

	if( wEnd < nLength && dwCurSel != CB_ERR )
		SetEditSel( wStart, wEnd);
	else
		SetEditSel( nLength, -1);
}

BOOL CTComboBox::OnEraseBkgnd( CDC* pDC)
{
	return TRUE;
}

LRESULT CTComboBox::WindowProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_CONTEXTMENU		:
		if(OnTrackComboContextMenu(GetSafeHwnd()))
			return 0;

		break;

	case WM_ENABLE			:
		{
			HWND hWndEdit = ::GetWindow( GetSafeHwnd(), GW_CHILD);

			if( !hWndEdit || !::IsWindow(hWndEdit) )
				break;

			::EnableWindow( hWndEdit, TRUE);
			::SendMessage( hWndEdit, EM_SETREADONLY, !wParam, 0L);
			Invalidate();
		}

		break;

	case WM_LBUTTONUP		: Invalidate(); break;
	case WM_MOUSEMOVE		:
		{
			SetTimer( m_dwUpdateTimer, m_dwUpdatePeriod, NULL);
			OnTimer(m_dwUpdateTimer);
		}

		break;

	case WM_SETFOCUS		:
		{
			LRESULT lResult = CComboBox::WindowProc( message, wParam, lParam);

			_OnDrawComboImpl(
				GetDroppedState(),
				TRUE);

			return lResult;
		}

		break;

	case WM_KILLFOCUS		:
		{
			LRESULT lResult = CComboBox::WindowProc( message, wParam, lParam);

			if(GetDroppedState())
				ShowDropDown(FALSE);

			_OnDrawComboImpl(
				GetDroppedState(),
				FALSE);

			return lResult;
		}

		break;

	case WM_LBUTTONDOWN		:
		{
			DWORD dwStyle = GetStyle();
			DWORD dwType = dwStyle&0x0003L;

			if( dwType == CBS_DROPDOWN || dwType == CBS_DROPDOWNLIST )
			{
				HWND hWndOwn = GetSafeHwnd();
				CWnd *pWndParent = GetParent();

				if(pWndParent)
				{
					pWndParent->SendMessage(WM_CANCELMODE);

					if(!::IsWindow(hWndOwn))
						return TRUE;
				}

				BOOL bDropped = GetDroppedState();
				SetFocus();

				_OnDrawComboImpl(
					!bDropped,
					TRUE);

				::PostMessage( hWndOwn, CB_SHOWDROPDOWN, !bDropped, 0);
				return TRUE;
			}
		}

		break;
	}

	return CComboBox::WindowProc( message, wParam, lParam);
}

void CTComboBox::PreSubclassWindow()
{
	CComboBox::PreSubclassWindow();

	if(!m_pInnerEditHook)
	{
		HWND hWndEdit = ::GetWindow( GetSafeHwnd(), GW_CHILD);

		if( hWndEdit && ::IsWindow(hWndEdit) )
		{
			m_pInnerEditHook = new CTComboEditCtrlHook();
			m_pInnerEditHook->SubclassWindow(hWndEdit);
		}
	}
}

BOOL CTComboBox::OnTrackComboContextMenu( HWND hWndSrc)
{
	return FALSE;
}
