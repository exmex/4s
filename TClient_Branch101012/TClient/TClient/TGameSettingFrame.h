#pragma once

#define OPTION_COUNT 11

class CTGameSettingFrame : public ITInnerFrame
{
public:
	TList*		m_pTLIST;
	TButton** m_pTCHECK;
	
	static LPBYTE TOPTIONPTR[OPTION_COUNT];
	static DWORD TNAME[OPTION_COUNT];

public:
	virtual void RequestInfo()	{}
	virtual void ResetInfo()	{}

public:
	virtual void OnLButtonDown( UINT nFlags, CPoint pt);
	virtual HRESULT Render(DWORD dwTickCount);

public:
	void UpdateUI();

public:
	CTGameSettingFrame(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGameSettingFrame();
};