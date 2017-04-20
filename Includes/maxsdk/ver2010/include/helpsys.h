/**********************************************************************
*<

	FILE: helpsys.h

	DESCRIPTION: Help Class include file.

	CREATED BY: greg finch

	HISTORY: Clean-up and added code to support individual html 
			 files (GG:09/26/00)

*>
**********************************************************************/

#ifndef _HELPSYS_H_
#define _HELPSYS_H_

#include "maxheap.h"
#include "contextids.h"
#include "export.h"

#define F1Focus(cmd,data)	getHelpSys().setHelpFocus(cmd,data)
#define F1Help()			getHelpSys().doHelpFocus()
#define DoHelp(cmd,data)	getHelpSys().help(cmd, data)
#define GetClickHelp()		getHelpSys().getClickHelp()

class DllExport HelpSys: public MaxHeapOperators {

public:
	
				HelpSys						( );
				~HelpSys					( );

    void		setAppHInst					(HINSTANCE h);
	void		setClickHelp				(int onOff);
	int			getClickHelp				( ) { return clickHelp; }

	/**
	 * \brief Deprecated. Does nothing.
	 *
	 * This function used to set the parent window for the help window.  As
	 * of 3ds Max 2009, the help window is always created as a top level window
	 * to avoid interfering with the main application's modeless dialog focus,
	 * and to have the help window appear in the system taskbar.
	 *
	 * \deprecated As of 3ds Max 2009, does nothing.
	 */
	__declspec(deprecated("Deprecated and no longer has any effect"))
		void setHelpHWnd(HWND h);
	
	/**
	 * \brief Returns the help window's parent window.  Always returns null.
	 *
	 * As of 3ds Max 2009, the help window is always created as a top level window
	 * to avoid interfering with the main application's modeless dialog focus,
	 * and to have the help window appear in the system taskbar.
	 */
	HWND getHelpHWnd();

	void		setHelpFocus				(UINT uCommand, DWORD dwData);
	int			doHelpFocus					( );
	int			help						(UINT uCommand, ULONG_PTR dwData);
	void		setExportedFunctionPointers	(void (*enableAcc)(), void (*disableAcc)(), BOOL (*accEnabled)());
	
private:

	int			clickHelp;
	HCURSOR		helpCursor;
	HCURSOR		savedCursor;
	UINT		focusCmd;
	DWORD		focusData;

	// helpHWnd - the parent for the help window is kept constant at NULL
	// This makes the help window appear in the taskbar and does not interfere
	// with the rest of max's windows.  The user is free to raise and lower
	// the windows.  Setting the main max window as the parent would cause
	// the help window to always appear on top, and it got in the user's way.
	static const HWND kHelpHWnd;
};

struct IDPair {
	DWORD CID;
	DWORD HID;
};

DllExport DWORD     CIDtoHID(int CID, IDPair *array);
DllExport void      SetDialogHelpIDs(HWND hDlg, IDPair *array);
DllExport HelpSys & getHelpSys(void);
DllExport HWND		GetHTMLHelpHWnd();

#endif // _HELPSYS_H_
