#pragma once

class CTKeySettingDlg : public CTClientUIBase
{
public:
	enum Result
	{
		RT_NONE,
		RT_SUCCESS,
		RT_WAITINPUT,
		RT_CANNOTUSE,
		RT_REPLACE
	};

protected:
	TList*			m_pList;
	TComponent*		m_pInfo;
	
	BOOL			m_bNeedUpdate;
	BOOL			m_bWaitInput;

	Result			m_eLastResult;

protected:
	void Update(DWORD dwTick);
	
public:
	void NotifyUpdate();
	void SetToDefault();
	
	void SetResult(Result eResult);
	void SetReplaceResult(TKEY_SET eSrcKeySet, TKEY_SET eRepKeySet);

	BOOL IsWaitInput() const		{ return m_bWaitInput; }

public:
	virtual HRESULT Render( DWORD dwTickCount );

	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual void OnKeyUp(UINT nChar, int nRepCnt, UINT nFlags);

	virtual void OnLButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);	

	virtual void OnMButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnMButtonUp(UINT nFlags, CPoint pt);

	virtual void OnRButtonDown(UINT nFlags, CPoint pt);
	virtual void OnRButtonUp(UINT nFlags, CPoint pt);

	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);	
	virtual void OnMButtonDblClk(UINT nFlags, CPoint pt);	
	virtual void OnRButtonDblClk(UINT nFlags, CPoint pt);

public:
	CTKeySettingDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTKeySettingDlg();
};