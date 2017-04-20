#include "StdAfx.h"
#include "GlobalFunc.h"
#include "resource.h"
#include "TMonSpawnInspectorDlg.h"

CTMonSpawnInspectorDlg& GetMainDlg()
{
	static	CTMonSpawnInspectorDlg dlg;
	return dlg;
}

VOID SetProgressRange( INT lower, INT upper )
{
	GetMainDlg().m_ctrlWorkingProgress.SetRange32( lower, upper );
}

VOID SetProgressPos( INT nPos )
{
	GetMainDlg().m_ctrlWorkingProgress.SetPos( nPos );
}

VOID SetProgressText( LPCTSTR str )
{
	GetMainDlg().m_stcWorkingText.SetWindowText( str );
}

VOID SetProgressClear()
{
	SetProgressRange( 0, 0 );
	SetProgressPos( 0 );
}

BOOL PeekAndPump( DWORD& nCount, DWORD nMax )
{
	if( ++nCount < nMax )
		return TRUE;

	MSG msg;
	while(::PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE)) {
		if(!AfxGetApp()->PumpMessage()) {
			::PostQuitMessage(0);
			nCount = 0;
			return FALSE;
		}
	}
	LONG idle = 0;
	while(AfxGetApp()->OnIdle(idle++));
	nCount = 0;
	return TRUE;
}