#pragma once

#include "TMacroDlg.h"

// CTMacroFrm 프레임입니다.

class CTMacroFrm : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CTMacroFrm)
protected:
	CTMacroFrm();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CTMacroFrm();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void ActivateFrame(int nCmdShow = -1);
public:
	CDialogBar		m_wndSideBar;
	CTMacroDlg		m_dlgSideBar;	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	////////////////////////////////////////////////////////
	// Get Macro Dlg
	CTMacroDlg* GetTMacroDlg();
	////////////////////////////////////////////////////////	
};