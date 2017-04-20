#pragma once

class CTCharPvPInfoDlg : public CTClientUIBase
{
public:
	CTCharPvPInfoDlg( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTCharPvPInfoDlg();

	void ResetData(
		BYTE bTabIndex,
		DWORD dwRankOrder,
		BYTE bRankPercent,
		DWORD* dwKillCountByClass,
		DWORD* dwDieCountByClass,
		VTLATESTPVPINFO,
		DWORD dwMonthRankOrder = 0,
		BYTE bMonthRankPercent = 0,
		WORD wMonthWin = 0,
		WORD wMonthLose = 0 );

	void ResetTabButton( BYTE bTabIndex );
	

	void ClearH();

public:
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );

public:
	BYTE m_bTabIndex;
	VTLATESTPVPINFO m_vLatestPvPInfo;

	TComponent* m_pH_TITLE;
	TComponent* m_pH_COL[2][4];
	TComponent* m_pH_VALUE[2][4];
	
	TComponent* m_pHL_TITLE;
	TComponent* m_pHR_TITLE;
	
	TComponent* m_pM_L[7];
	TComponent* m_pM_M[7];
	TComponent* m_pM_R[7];

	TComponent* m_pL_TITLE;
	TComponent* m_pL_COL1[10];
	TComponent* m_pL_COL2[10];
	TComponent* m_pL_COL3[10];
};