#pragma once

#include "STType.h"

class CTMacro
{
public:
	CTMacro(CString sName, CString sParent);
	CTMacro(BOOL bEnable, BOOL bLoop, CString sName, CString sParent, VECTORPARAM vParam, VECTORPINFO vPInfo);
	~CTMacro(void);
public:
	////////////////////////////////////////////////////////////////
	BOOL m_bEnable; // Enable Flag

	BOOL m_bLoop; // Loop

    CString m_sName; // Macro Name
	CString m_sParent; // Macro Parent Name

	VECTORPARAM m_vParam; // Macro Paramater Vector

	VECTORPINFO m_vPInfo; // Macro Protocol Info Vector
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// Macro Enable Flag
	void SetMacroEnable(BOOL flag);
	BOOL GetMacroEnable();
	// Macro Valid Check
	BOOL ValidCheck(MAPPROTOCOL mapProtocol);
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// Macro Enable Flag
	void SetMacroLoop(BOOL flag);
	BOOL GetMacroLoop();
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// Macro
	void SetMacroName(CString sName, CString sParent);	
	void UpdateMacroName(CString sName);
	void UpdateMacroParent(CString sParent);
	CString GetMacroName();
	CString GetMacroParent();
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// Macro Paramater
	BOOL AddMacroParam(STRUCTPARAM sParam);
	int SearchMacroParam(CString sName, CString sType);
	void DelMacroParam(CString sName, CString sType);	
	void DelAllMacroParam();
	void SetMacroParam(VECTORPARAM vParam);
	VECTORPARAM GetMacroParam();
	BOOL IsEmptyParam();
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
    // Macro Protocol Info Vector
	BOOL AddMacroPInfo(STRUCTPINFO sPInfo);
	int SearchMacroPInfo(CString sName);	
	void DelMacroPInfo(CString sName);
	void DelAllMacroPInfo();
    VECTORPINFO GetMacroPInfo();
	BOOL IsEmptyPInfo();

	// Macro Protocol Info Struct Set / Get / Name Update
	void SetMacroPInfo(CString sName, STRUCTPINFO sPInfo);
	STRUCTPINFO GetMacroPInfo(CString sName);
	BOOL UpdatePInfoName(CString sOld, CString sName, DWORD pID);	

	// Macro Protocol Info Sleep Set / Clear
	void SetPInfoSleep(CString sName, int sleep);
	void ClearPInfoSleep(CString sName);

	// Macro Protocol Info Self Set / Get / Clear
	void SetPInfoSelf(CString sName, BOOL flag, VECTORPARAM vParam);
	int GetPInfoSelf(CString sName);
	void ClearPInfoSelf(CString sName);	

	// Macro Protocol Info Paramater Set / Clear
	void SetPInfoParam(CString sName, VECTORPARAM vParam);	
	void ClearPInfoParam(CString sName);
	////////////////////////////////////////////////////////////////
public:
	////////////////////////////////////////////////////////////////
	// Session Macro Info Send / Receive Vector
	VECTORSRINFO m_vSend;
	VECTORSRINFO m_vRecv;

	void SetSendRecvInfo();
	void SetSendRecvInfo(CString sName);
	VECTORSRINFO GetSendInfo();
	VECTORSRINFO GetRecvInfo();
	void ClearSendRecvInfo();

	// Get Current Send
	STRUCTPINFO GetCurSendInfo(int i);
	// Is Send Last Cnt
	BOOL IsSendLast(int i);
	////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////
	// Session Macro Self Info Vector
	VECTORSELFINFO m_vSelf;

	void SetSelfInfo(STRUCTPINFO pInfo);
	VECTORSELFINFO GetSelfInfo();
    void ClearSelfInfo();
	////////////////////////////////////////////////////////////////
};
