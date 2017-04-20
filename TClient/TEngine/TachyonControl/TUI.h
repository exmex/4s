#if (!defined __TUI_H)
#define __TUI_H

#if (!defined __TMFC_DEF_H)
	#include <TMfcDef.h>
#endif // __TMFC_DEF_H

#ifndef __AFXWIN_H__
	#include <AfxWin.h>         // MFC core and standard components
#endif

#ifndef __AFXEXT_H__
	#include <afxext.h>         // MFC extensions
#endif

#ifndef _AFX_NO_OLE_SUPPORT
	#ifndef __AFXOLE_H__
		#include <AfxOle.h>     // MFC OLE classes
	#endif
	#ifndef __AFXODLGS_H__
		#include <AfxOdlgs.h>   // MFC OLE dialog classes
	#endif
	#ifndef __AFXDISP_H__
		#include <AfxDisp.h>    // MFC Automation classes
	#endif
#endif // _AFX_NO_OLE_SUPPORT

#ifndef __AFXTEMPL_H__
	#include <AfxTempl.h>
#endif

#ifndef __AFXMT_H__
	#include <AfxMt.h>
#endif

#ifndef __AFXCMN_H__
	#include <AfxCmn.h>
#endif

#ifndef __AFXDTCTL_H__
	#include <AfxDtCtl.h>
#endif

#ifndef __AFXCONV_H__
	#include <AfxConv.h>
#endif

#if (!defined __TUI_IMPL__)

	#if (!defined __TMFC_DEF_H)
		#include <TMfcDef.h>
	#endif

	#if (!defined __TBTNONFLAT_H)
		#include <TBtnOnFlat.h>
	#endif

	#if (!defined __TBUTTON_H)
		#include <TButton.h>
	#endif

	#if (!defined __TCOMBOBOX_H)
		#include <TComboBox.h>
	#endif

	#if (!defined __TEDIT_H)
		#include <TEdit.h>
	#endif

	#if (!defined __TCMDMANAGER_H)
		#include <TCmdManager.h>
	#endif

	#if (!defined __TPAINT_MANAGER_H)
		#include <TPaintManager.h>
	#endif

	#if (!defined __TCONTROLBAR_H)
		#include <TControlBar.h>
	#endif

	#if (!defined __TTOOLCONTROLBAR_H)
		#include <TToolControlBar.h>
	#endif

	#if (!defined __TMENUCONTROLBAR_H)
		#include <TMenuControlBar.h>
	#endif

	#if (!defined __TSTATUSCONTROLBAR_H)
		#include <TStatusControlBar.h>
	#endif

	#if (!defined __TPOPUP_MENU_WND_H)
		#include <TPopupMenuWnd.h>
	#endif

	#if (!defined __TRESIZABLEDIALOG_H)
		#include <TResizableDialog.h>
	#endif

	#if (!defined __TCOLORCTRL_H)
		#include <TColorCtrl.h>
	#endif

	#if (!defined __TCOLORDLG_H)
		#include <TColorDlg.h>
	#endif

	#if (!defined __TTEMPL_H)
		#include <TTempl.h>
	#endif

	#if (!defined __TTABCTRL_H)
		#include <TTabCtrl.h>
	#endif

	#if (!defined __TTABCTRLBAR_H)
		#include <TTabControlBar.h>
	#endif

	#if (!defined __TSHELL_H)
		#include <TShell.h>
	#endif

#endif // #if (!defined __TUI_IMPL__)

#endif // #if (!defined __TUI_H)