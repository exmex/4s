#include "StdAfx.h"
#include "THookSink.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


struct __TUI_API CTHookSink::HookSinkArray : public CArray< CTHookSink *, CTHookSink *>
{
	int Find( const CTHookSink *pHookSink) const
	{
		for( int i=0; i<GetSize(); i++)
		{
			const CTHookSink *pHookSinkExamine = GetAt(i);

			if( pHookSinkExamine == pHookSink )
				return i;
		}

		return -1;
	};

	void AddHead( CTHookSink *pHookSink)
	{
		InsertAt( 0, pHookSink);
	};

	void AddTail( CTHookSink *pHookSink)
	{
		InsertAt( GetSize(), pHookSink);
	};
};

struct __TUI_API CTHookSink::HookChains
{
	HookSinkArray m_HookSinkArray;
	HWND m_hWndHooked;
	WNDPROC m_pWNDPROC;

	static LRESULT CALLBACK m_HookWndProc(
		HWND hWnd,
		UINT nMessage,
		WPARAM wParam,
		LPARAM lParam);

	HookChains( HWND hWndHooked) : m_hWndHooked(hWndHooked)
	{
		m_pWNDPROC = (WNDPROC) ::SetWindowLong(
			m_hWndHooked,
			GWL_WNDPROC,
			(DWORD) m_HookWndProc);
	};

	~HookChains()
	{
		DestroyChains(FALSE);
	};

	void DestroyChains( BOOL bDelete)
	{
		for( int i=0; i<m_HookSinkArray.GetSize(); i++)
		{
			CTHookSink *pHookSink = m_HookSinkArray[i];

			if(pHookSink->IsAutoDeleteHookWndSink())
				delete pHookSink;
		}

		m_HookSinkArray.RemoveAll();
		::SetWindowLong(
			m_hWndHooked,
			GWL_WNDPROC,
			(DWORD) m_pWNDPROC);

		if(bDelete)
			delete this;
	};

	LRESULT HookChainsWindowProc(
		UINT nMessage,
		WPARAM &wParam,
		LPARAM &lParam)
	{
		for( int i=0; i<m_HookSinkArray.GetSize(); i++)
		{
			CTHookSink *pHookSink = m_HookSinkArray[i];

			if( nMessage == WM_NCDESTROY )
			{
				pHookSink->OnHookWndNcDestroy();
				continue;
			}

			LRESULT lResult = 0;
			if( pHookSink->OnHookWndMsg(
				lResult,
				m_hWndHooked,
				nMessage,
				wParam,
				lParam))
				return lResult;
		}

		WNDPROC pWNDPROC = m_pWNDPROC;
		HWND hWndHooked = m_hWndHooked;

		if( nMessage == WM_NCDESTROY )
			DestroyChains(TRUE);

		LRESULT lResult = ::CallWindowProc(
			pWNDPROC,
			hWndHooked,
			nMessage,
			wParam,
			lParam);

		return lResult;
	};
};


typedef CMap< HWND, HWND, CTHookSink::HookChains *, CTHookSink::HookChains *>	HookChainsMap;
static HookChainsMap g_HookChainsMap;

LRESULT CALLBACK CTHookSink::HookChains::m_HookWndProc( HWND hWnd,
													    UINT nMessage,
														WPARAM wParam,
														LPARAM lParam)
{
#ifdef _USRDLL
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
#endif
	LRESULT lResult = 0;
	MSG &refMsgMFCCurr = AfxGetThreadState()->m_lastSentMsg;
	MSG msgMFCSaved(refMsgMFCCurr);

	refMsgMFCCurr.hwnd = hWnd;
	refMsgMFCCurr.message = nMessage;
	refMsgMFCCurr.wParam = wParam;
	refMsgMFCCurr.lParam = lParam;

	CTHookSink::HookChains *pHookChains = NULL;
	if(g_HookChainsMap.Lookup( hWnd, pHookChains))
	{
		lResult = pHookChains->HookChainsWindowProc(
			nMessage,
			wParam,
			lParam);

		if( nMessage == WM_NCDESTROY )
			g_HookChainsMap.RemoveKey(hWnd);
	}
	refMsgMFCCurr = msgMFCSaved;

	return lResult;
}

CTHookSink::CTHookSink( BOOL bEnableDetailedWndHooks) : m_bEnableDetailedWndHooks(bEnableDetailedWndHooks)
{
}

CTHookSink::~CTHookSink()
{
}

LRESULT CTHookSink::OnHookWndMsgDefault()
{
	MSG &msgCurrMFC = AfxGetThreadState()->m_lastSentMsg;
	CTHookSink::HookChains *pHookChains = NULL;

	g_HookChainsMap.Lookup(
		msgCurrMFC.hwnd,
		pHookChains);

	return ::CallWindowProc(
		pHookChains->m_pWNDPROC,
		msgCurrMFC.hwnd,
		msgCurrMFC.message,
		msgCurrMFC.wParam,
		msgCurrMFC.lParam);
}

BOOL CTHookSink::OnHookWndMsg( LRESULT &lResult,
							   HWND hWndHooked,
							   UINT nMessage,
							   WPARAM &wParam,
							   LPARAM &lParam)
{
	if(!m_bEnableDetailedWndHooks)
		return FALSE;

	switch(nMessage)
	{
	case WM_COMMAND		:
		return OnHookCmdMsg(
			lResult,
			hWndHooked,
			HIWORD(wParam),
			LOWORD(wParam),
			(HWND) lParam);

	case WM_NOTIFY		:
		return OnHookNotifyMsg(
			lResult,
			hWndHooked,
			INT(wParam),
			(LPNMHDR)lParam);

	case WM_PAINT		:
		return OnHookPaintMsg(
			lResult,
			hWndHooked,
			(HDC) wParam);

	case WM_ERASEBKGND	:
		return OnHookEraseBackgroundMsg(
			lResult,
			hWndHooked,
			(HDC) wParam);

	case WM_PRINT		:
		return OnHookPrintMsg(
			lResult,
			hWndHooked,
			(HDC) wParam);

	case WM_NCPAINT		:
		return OnHookNcPaintMsg(
			lResult,
			hWndHooked,
			(HRGN) wParam);
	}

	return FALSE;
}

BOOL CTHookSink::OnHookCmdMsg( LRESULT &lResult,
							   HWND hWndHooked,
							   WORD wNotifyCode,
							   WORD wID,
							   HWND hWndCtrl)
{
	return FALSE;
}

BOOL CTHookSink::OnHookNotifyMsg( LRESULT &lResult,
								  HWND hWndHooked,
								  int nIdCtrl,
								  LPNMHDR lpnmhdr)
{
	return FALSE;
}

BOOL CTHookSink::OnHookPaintMsg( LRESULT &lResult,
								 HWND hWndHooked,
								 HDC hDC)
{
	return FALSE;
}

BOOL CTHookSink::OnHookEraseBackgroundMsg( LRESULT &lResult,
										   HWND hWndHooked,
										   HDC hDC)
{
	return FALSE;
}

BOOL CTHookSink::OnHookPrintMsg( LRESULT &lResult,
								 HWND hWndHooked,
								 HDC hDC)
{
	return FALSE;
}

BOOL CTHookSink::OnHookNcPaintMsg( LRESULT &lResult,
								   HWND hWndHooked,
								   HRGN hRgnUpdate)
{
	return FALSE;
}

void CTHookSink::OnHookWndNcDestroy()
{
}

void CTHookSink::OnHookWndAttach( HWND hWnd)
{
}

void CTHookSink::OnHookWndDetach( HWND hWnd)
{
}

BOOL CTHookSink::IsAutoDeleteHookWndSink()
{
	return FALSE;
}

BOOL CTHookSink::SetupHookWndSink( HWND hWnd,
								   BOOL bRemove,
								   BOOL bAddToHead)
{
	if( hWnd == NULL )
		return FALSE;

	if( !bRemove && !::IsWindow(hWnd) )
		return FALSE;

	CTHookSink::HookChains *pHookChains = NULL;
	g_HookChainsMap.Lookup( hWnd, pHookChains);

	if(bRemove)
	{
		if( pHookChains == NULL )
			return TRUE;

		int pos = pHookChains->m_HookSinkArray.Find(this);
		if( pos < 0 )
			return TRUE;

		OnHookWndDetach(hWnd);
		pHookChains->m_HookSinkArray.RemoveAt(pos);

		if(IsAutoDeleteHookWndSink())
			delete this;

		return TRUE;
	}

	if( pHookChains == NULL )
	{
		pHookChains = new CTHookSink::HookChains(hWnd);
		g_HookChainsMap.SetAt( hWnd, pHookChains);
	}
	else
	{
		int pos = pHookChains->m_HookSinkArray.Find(this);
		if( pos >= 0 )
			return TRUE;
	}

	if(bAddToHead)
		pHookChains->m_HookSinkArray.AddHead(this);
	else
		pHookChains->m_HookSinkArray.AddTail(this);

	OnHookWndAttach(hWnd);
	return TRUE;
}

ULONG CTHookSink::SetupHookWndSinkToChilds( HWND hWnd,
										    UINT *pDlgCtrlIDs,
											ULONG nCountOfDlgCtrlIDs,
											BOOL bDeep)
{
	if( hWnd == NULL )
		return 0;

	if(!::IsWindow(hWnd))
		return 0;

	ULONG nCountOfHooks = 0;
	hWnd = ::GetWindow( hWnd, GW_CHILD);

	for(; hWnd; hWnd = ::GetWindow( hWnd, GW_HWNDNEXT))
	{
		BOOL bSetupHook = TRUE;

		if( nCountOfDlgCtrlIDs > 0 && pDlgCtrlIDs )
		{
			bSetupHook = FALSE;
			UINT nDlgCtrlID = ::GetDlgCtrlID(hWnd);

			for( ULONG i=0; i<nCountOfDlgCtrlIDs; i++)
			{
				if( pDlgCtrlIDs[i] == nDlgCtrlID )
				{
					bSetupHook = TRUE;
					break;
				}
			}
		}

		if( bSetupHook && SetupHookWndSink(hWnd) )
			nCountOfHooks++;

		if(bDeep)
		{
			nCountOfHooks += SetupHookWndSinkToChilds(
				hWnd,
				pDlgCtrlIDs,
				nCountOfDlgCtrlIDs,
				bDeep);
		}
	}

	return nCountOfHooks;
}

void CTHookSink::RemoveAllWndHooks()
{
	HookedWndList _list;
	GetHookedWindows(_list);

	POSITION pos = _list.GetHeadPosition();
	for( int i=0; i<_list.GetCount(); i++)
	{
		HWND hWndHooked = _list.GetNext(pos);
		SetupHookWndSink( hWndHooked, FALSE);
	}
}

void CTHookSink::GetHookedWindows( HookedWndList& _list)
{
	_list.RemoveAll();
	POSITION pos = g_HookChainsMap.GetStartPosition();

	while(pos)
	{
		CTHookSink::HookChains *pHookChains = NULL;
		HWND hWndHooked = NULL;

		g_HookChainsMap.GetNextAssoc(
			pos,
			hWndHooked,
			pHookChains);

		if( pHookChains->m_HookSinkArray.Find(this) < 0 )
			continue;

		_list.AddTail(pHookChains->m_hWndHooked);
	}
}

BOOL CTHookSink::IsHookedWindow( HWND hWnd)
{
	CTHookSink::HookChains *pHookChains = NULL;

	if(!g_HookChainsMap.Lookup( hWnd, pHookChains))
		return FALSE;

	if( pHookChains->m_HookSinkArray.Find(this) >= 0 )
		return TRUE;

	return FALSE;
}
