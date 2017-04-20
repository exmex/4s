#pragma once


class CTNPCTalkDlg : public CTNewQuestDlg
{
public:
	CTNewQuestDlg* m_pTFollowDlg;
	VTQUESTTEMP m_vTDEFTALK;

public:
	LPTQUEST GetCurSelTQuest();
	LPTQUEST GetCurTMission();
	LPTQUEST GetCurTQuest();

	void Talk( CString strTALK);
	void Talk( CString strTALK, CString strAnswer);
	void DefaultTalk();

public:
	virtual void ShowComponent( BOOL bVisible = TRUE);

	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point);
	virtual HRESULT Render( DWORD dwTickCount);

public:
	CTNPCTalkDlg( TComponent* pParent, LP_FRAMEDESC pDesc, CTClientChar* pHost, TCMLParser* pParser);
	virtual ~CTNPCTalkDlg();
};
