#pragma once

class CTMonSpawnInspectorDlg;
CTMonSpawnInspectorDlg& GetMainDlg();

VOID SetProgressRange( INT lower, INT upper );
VOID SetProgressPos( INT nPos );
VOID SetProgressText( LPCTSTR );
VOID SetProgressClear();
BOOL PeekAndPump( DWORD& nCount, DWORD nMax );