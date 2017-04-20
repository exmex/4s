// TGauge.h: interface for the TGauge class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TGAUGE_H__E2E50978_C747_4E08_BBDE_786297EAF213__INCLUDED_)
#define AFX_TGAUGE_H__E2E50978_C747_4E08_BBDE_786297EAF213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TComponent.h"

#define DEFAULT_GAUGE_REFRESH				5

class TGauge : public TComponent  
{
public:
	TGauge( TComponent* pParent, LP_FRAMEDESC pDesc );
	virtual ~TGauge();

	virtual HRESULT DrawKids( DWORD dwTickCount );
	virtual void OnMouseMove( UINT nFlags, CPoint pt );

	int GetCurValue();
	int GetMaxValue();
	void SetRefreshTime( DWORD dwRefreshTime );
	void IncGauge( int nInc, BOOL bContinuous = TRUE );
	void DecGauge( int nDec, BOOL bContinuous = TRUE );
	void SetGauge( int nCurValue, int nMaxValue, BOOL bContinuous = FALSE );
	void SetCurrBarImage( BYTE bCurrBarImage );
	void SetShowTextOnMouse( BOOL b );
	
protected:
	TCOMP_ARRAY m_vBar;

	DWORD m_dwLastUpdate;
	DWORD m_dwLapTick;

	int m_nMaxValue;
	int m_nCurValue;
	int m_nNextValue;
	int m_nPrevValue;

	BOOL m_bShowCurValue;
};

#endif // !defined(AFX_TGAUGE_H__E2E50978_C747_4E08_BBDE_786297EAF213__INCLUDED_)
