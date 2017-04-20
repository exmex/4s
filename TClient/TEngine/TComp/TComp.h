#ifndef _INC_TCOMP_H
#define _INC_TCOMP_H

#include <afx.h>
#include <afxwin.h>
#include <afxsock.h>
#include <afxdisp.h>
#include <afxdhtml.h>

#include <imm.h>

#pragma warning( disable : 4786 4530 4503)
#include <vector>
#include <list>
#include <map>
using namespace std;

#include "TCML.h"


class TComponent;
typedef list<TComponent *>				TCOMP_LIST;

#define HITTEST_TYPE_IMAGEMASK		0
#define HITTEST_TYPE_RECT					1

#define TTEXTSETTINGTYPE_NONE			(0)
#define TTEXTSETTINGTYPE_COMP			(1<<1)
#define TTEXTSETTINGTYPE_LINE			(1<<2)
#define TTEXTSETTINGTYPE_LINE_DOT		(1<<3)
#define TTEXTSETTINGTYPE_LINE_THICK		(1<<4)

#define TCHAR_MBCS_SINGLE				((BYTE) 0x01)
#define TCHAR_MBCS_LEAD					((BYTE) 0x02)
#define TCHAR_MBCS_TAIL					((BYTE) 0x03)

#define IS_MBCS_BYTE(c)					(BYTE(c) == TCHAR_MBCS_LEAD || BYTE(c) == TCHAR_MBCS_TAIL ? TRUE : FALSE)
#define IS_MBCS_LEAD(c)					(BYTE(c) == TCHAR_MBCS_LEAD ? TRUE : FALSE)
#define IS_MBCS_TAIL(c)					(BYTE(c) == TCHAR_MBCS_TAIL ? TRUE : FALSE)
#define MBCS_LEAD_TEST(c)				( ::IsDBCSLeadByte(c) )

#include "GDIFont.h"
#include "TListItem.h"
#include "TComponent.h"
#include "TImageList.h"
#include "THttpCtrl.h"
#include "TTabCtrl.h"
#include "TButton.h"
#include "TScroll.h"
#include "TGauge.h"
#include "TCombo.h"
#include "TCandidate.h"
#include "TEdit.h"
#include "TList.h"
#include "TFrame.h"
#include "TMeter.h"
#include "TMultiLineEdit.h"

void SetForceTachyonFont(LPCSTR strForceFont);
void SetTachyonFontSizeRate(INT nRate);
void SetTachyonFontQuality(LPCSTR strQuality);

void InitTachyonComponent( CTachyonRes* pImagePool, 
						   CTachyonRes* pMenuPool, 
						   TCMLParser* pParser);

void FinishTachyonComponent();

LPIMAGESET FindSprite(DWORD id);
CGDIFont *FindFont(DWORD id);
CString BuildMBCSInfo( CString& strTEXT);

#endif