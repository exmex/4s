// TMeter.h: interface for the TMeter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TMETER_H__5D24D030_1244_4E00_B8B5_0A800BFD2364__INCLUDED_)
#define AFX_TMETER_H__5D24D030_1244_4E00_B8B5_0A800BFD2364__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TComponent.h"

class TMeter : public TComponent  
{
public:	
	int GetValue();
	void SetValue(int nValue);	

	TMeter(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~TMeter();

protected:
	virtual HRESULT DrawKids( DWORD dwTickCount );
	void SetIncremental(CSize &size);
	void SetIncrementalStart(CPoint &point, CSize size);	

	TComponent *	m_pSuper;
	TComponent *	m_pSub[MAX_METER_SUBLEVEL];

	int				m_nValue;
	int				m_nSubLevel;
	int				m_nSuperLevel;

	DWORD			m_dwTotalTick;
};

#endif // !defined(AFX_TMETER_H__5D24D030_1244_4E00_B8B5_0A800BFD2364__INCLUDED_)
