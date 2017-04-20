// GDIFont.h: interface for the CGDIFont class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GDIFONT_H__869B4CE4_A268_407D_A44F_3CD01B9C54A9__INCLUDED_)
#define AFX_GDIFONT_H__869B4CE4_A268_407D_A44F_3CD01B9C54A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CGDIFont  
{
public:
	CGDIFont();
	virtual ~CGDIFont();

	CFont* m_pFont;
	COLORREF m_clr;
};

typedef map	<DWORD, CGDIFont *>		GDIFONT_MAP; 

#endif // !defined(AFX_GDIFONT_H__869B4CE4_A268_407D_A44F_3CD01B9C54A9__INCLUDED_)
