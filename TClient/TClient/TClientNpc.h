#pragma once


class CTClientNpc : public CTClientObjBase
{
public:
	LPTPOPUPMENU m_pTPOPUP;
	LPTNPCTEMP m_pTNPC;

	CString m_strTITLE;
	CString m_strNAME;

	DWORD m_dwLeftRndActTick;
	DWORD m_dwQuestID;
	DWORD m_dwExecID;
	WORD m_wItemID;

	BYTE m_bOriginalContryID;
	BYTE m_bRandACT;
	BYTE m_bLogic;
	BYTE m_bCamp;

public:
	void InitTNPC(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		LPTNPC pTNPC);

	void SetLogicAction( DWORD dwActionID);
	void OpenBOX( BYTE bOPEN);
	void ResetBOX();

	void CalcRandomAction(
		CD3DDevice *pDevice,
		CTachyonRes *pRES,
		DWORD dwTick);
	void SetLeftRndActTick();

protected:
	BYTE CheckRequiredQuest();
	BYTE CheckRequiredItem();
	BYTE CanRandACT();

public:
	virtual BYTE CheckRequired();
	virtual BYTE GetDrawName();
	virtual void ReleaseData();
	virtual void Release();

	virtual BYTE GetDrawNodeMark(
		LPTTERMPOS pTNODEPATH,
		LPTNPCPORTAL pTNODEPORTAL);

	virtual CString GetTitle();
	virtual CString GetName();

	virtual void CalcTick(
		CD3DDevice* pDevice,
		CTachyonRes *pRES,
		DWORD dwTick);
	virtual LRESULT OnActEndMsg();

public:
	CTClientNpc();
	virtual ~CTClientNpc();
};
