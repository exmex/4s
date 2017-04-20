#pragma once

class CTMessengerBase : public ITInnerFrame
{
protected:
	CTMsgGroupList*			m_pGroupList;
	//TComponent*				m_pAddMemberBtn;
	//TComponent*				m_pBlockListBtn;

public:
	void SetMsgGroupList(CTMsgGroupList* pList);
	CTMsgGroupList* GetMsgGroupList() const				{ return m_pGroupList; }

public:
	virtual void RequestInfo()	{}
	virtual void ResetInfo()	{}

	virtual void OnLButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnRButtonDown(UINT nFlags, CPoint pt);	
	virtual void OnLButtonDblClk(UINT nFlags, CPoint pt);

public:
	CTMessengerBase(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMessengerBase();
};