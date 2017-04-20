#pragma once

class CTSMSDlg : public CTClientUIBase
{
public:
	enum TYPE
	{
		TYPE_PERSON,
		TYPE_GUILD,
		TYPE_MAX
	};

protected:
	TYPE				m_eType;
	TEdit*				m_pTargetEdit;
	TMultiLineEdit*		m_pMsgEdit;
	TComponent*			m_pLenTxt;
	TComponent*			m_pWarnTxt;

	BYTE 				m_bInvenID;
	BYTE 				m_bInvenSlot;

	INT					m_nMaxLen;

public:
	void Set(TYPE eType, BYTE bInvenID, BYTE bInvenSlot);
	void SetItem(BYTE bInvenID, BYTE bInvenSlot);

	BYTE GetInvenID() const		{ return m_bInvenID; }
	BYTE GetInvenSlot() const	{ return m_bInvenSlot; }
	
	TEdit* GetCurEdit();
	void SendSMS();

public:
	virtual void OnKeyDown(UINT nChar, int nRepCnt, UINT nFlags);
	virtual HRESULT Render( DWORD dwTickCount );

public:
	CTSMSDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTSMSDlg();
};