// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers
#endif

// Modify the following defines if you have to target a platform prior to the ones specified below.
// Refer to MSDN for the latest info on corresponding values for different platforms.
#ifndef WINVER				// Allow use of features specific to Windows 95 and Windows NT 4 or later.
#define WINVER 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif

#ifndef _WIN32_WINNT		// Allow use of features specific to Windows NT 4 or later.
#define _WIN32_WINNT 0x0400		// Change this to the appropriate value to target Windows 98 and Windows 2000 or later.
#endif						

#ifndef _WIN32_WINDOWS		// Allow use of features specific to Windows 98 or later.
#define _WIN32_WINDOWS 0x0410 // Change this to the appropriate value to target Windows Me or later.
#endif

#ifndef _WIN32_IE			// Allow use of features specific to IE 4.0 or later.
#define _WIN32_IE 0x0400	// Change this to the appropriate value to target IE 5.0 or later.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <math.h>

using namespace std;
#include <vector>
#include <map>


#define TOUTLINE_COLOR		RGB( 255, 249, 11)
#define TLINE_COLOR			RGB( 48, 11, 255)
#define TSELECT_COLOR		RGB( 255, 48, 11)

#define TTEXTOUTLINE_COLOR	RGB( 0, 0, 0)
#define TTEXT_COLOR			RGB( 255, 255, 255)

#define TBACKGROUND_COLOR	RGB( 255, 255, 255)
#define TGRAY_COLOR			RGB( 200, 200, 200)

#define TDEF_JOINT_HEIGHT	64
#define TDEF_JOINT_WIDTH	64
#define TDEF_SNAP_SIZE		32

#define TSPOT_SIZE			16
#define TDEF_TIME			20

#define TUNITCOUNT_X		7
#define TUNITCOUNT_Z		8

typedef enum TEDIT_MODE
{
	TEDIT_INFO_TSPOT = 0,
	TEDIT_SELECT_TSPOT,
	TEDIT_ADD_TSPOT,
	TEDIT_DELETE_TSPOT,
	TEDIT_EDIT_JOINT,
	TEDIT_MOVE_JOINT,
	TEDIT_ADD_JOINT,
	TEDIT_DELETE_JOINT,
	TEDIT_COUNT
} *LPTEDIT_MODE;

typedef struct tagTJOINT
{
	CRect m_vRECT;
	BYTE m_bJOINT;

	tagTJOINT()
	{
		m_vRECT.left = 0;
		m_vRECT.top = 0;
		m_vRECT.right = 0;
		m_vRECT.bottom = 0;

		m_bJOINT = 0;
	};
} TJOINT, *LPTJOINT;

typedef struct tagTSPOT
{
	int m_nPosX;
	int m_nPosY;

	tagTSPOT()
	{
		m_nPosX = 0;
		m_nPosY = 0;
	};
} TSPOT, *LPTSPOT;

typedef map< DWORD, LPTJOINT>	MAPTJOINT, *LPMAPTJOINT;
typedef vector<LPTSPOT>			VTSPOT, *LPVTSPOT;

#endif // _AFX_NO_AFXCMN_SUPPORT
