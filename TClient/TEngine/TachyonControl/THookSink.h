#if (!defined __THOOKSINK_H)
#define __THOOKSINK_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H


class __TUI_API CTHookSink
{
	friend struct HookChains;

public:
	struct HookSinkArray;
	struct HookChains;

	BOOL m_bEnableDetailedWndHooks;

	typedef CList< HWND, HWND>	HookedWndList;
	CTHookSink( BOOL bEnableDetailedWndHooks = TRUE);
	~CTHookSink();

	virtual BOOL IsAutoDeleteHookWndSink();
	virtual LRESULT OnHookWndMsgDefault();

	virtual BOOL OnHookWndMsg(
		LRESULT &lResult,
		HWND hWndHooked,
		UINT nMessage,
		WPARAM &wParam,
		LPARAM &lParam);

	virtual BOOL OnHookCmdMsg(
		LRESULT &lResult,
		HWND hWndHooked,
		WORD wNotifyCode,
		WORD wID,
		HWND hWndCtrl);

	virtual BOOL OnHookNotifyMsg(
		LRESULT &lResult,
		HWND hWndHooked,
		int nIdCtrl,
		LPNMHDR lpnmhdr);

	virtual BOOL OnHookPaintMsg(
		LRESULT &lResult,
		HWND hWndHooked,
		HDC hDC);

	virtual BOOL OnHookEraseBackgroundMsg(
		LRESULT &lResult,
		HWND hWndHooked,
		HDC hDC);

	virtual BOOL OnHookPrintMsg(
		LRESULT &lResult,
		HWND hWndHooked,
		HDC hDC);

	virtual BOOL OnHookNcPaintMsg(
		LRESULT &lResult,
		HWND hWndHooked,
		HRGN hRgnUpdate);

	virtual void OnHookWndNcDestroy();
	virtual void RemoveAllWndHooks();

	virtual void OnHookWndAttach( HWND hWnd);
	virtual void OnHookWndDetach( HWND hWnd);

	virtual BOOL SetupHookWndSink(
		HWND hWnd,
		BOOL bRemove = FALSE,
		BOOL bAddToHead = FALSE);

	ULONG SetupHookWndSinkToChilds(
		HWND hWnd,
		UINT *pDlgCtrlIDs = NULL,
		ULONG nCountOfDlgCtrlIDs = 0,
		BOOL bDeep = FALSE);

	void GetHookedWindows( HookedWndList& _list);
	BOOL IsHookedWindow( HWND hWnd);
}; // class CTHookSink

#endif // #if (!defined __THOOKSINK_H)