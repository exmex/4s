#pragma once

class CTItemUpCashDlg;
class CTItemUpCashSlot;
class CTClientItem;

class CTItemUpCashSlot : public CTCtrlListSlot
{
	
public:
	TImageList*		m_pIcon;
	TComponent*		m_pName;
	TComponent*		m_pExplanation;

public:
	virtual void ShowComponent(BOOL bShow);
	virtual void Select(BOOL bSel);

public:
	CTItemUpCashSlot()
		: m_pIcon(NULL), m_pName(NULL)
	{}
		virtual ~CTItemUpCashSlot() {}
};


class CTItemUpCashDlg :	public CTClientUIBase
{
public:
	

	enum	{	MAX_ITEM_SLOT = 3	};	
	enum
	{
		MODE_ITEMUP = 0,
		MODE_REFINE,
		MODE_TOTAL
	};
	enum 
	{
		MAINTAIN_POTIONOFFORTUNE = 905,				// 행운의 물약100%
		MAINTAIN_POTIONOFFORTUNE150 = 921,				// 행운의 물약150%
		MAINTAIN_POTIONOFFORTUNE200 = 922,				// 행운의 물약200%
		MAINTAIN_PROTECTIONOFUPGRADING = 919,		// 강화 보호제
		MAINTAIN_LEGENDENCHARTBOOK = 911,			// 전설의 제작 비전서
		MAINTAIN_LEGENDREFINEBOOK = 912,			// 전설의 제련 비전서
		MAINTAIN_MAGICTRANSFERSPELL = 913,			// 마법속성 전이 주문서
	};

	enum
	{
		CASHITEM_POTIONOFFORTUNE = 7609,			// 행운의 물약
		CASHITEM_PROTECTIONOFUPGRADING = 7654,		// 강화 보호제
		CASHITEM_LEGENDENCHARTBOOK = 7611,			// 전설의 제작 비전서
		CASHITEM_LEGENDREFINEBOOK = 7612,			// 전설의 제련 비전서
		CASHITEM_MAGICTRANSFERSPELL = 7613,			// 마법속성 전이 주문서
	};

	TComponent*		m_pTopMessage;		// 상단 메시지
	TComponent*		m_pBottonMessage;	// 하단 메시지
	TComponent*		m_pBottonMessage2;	// 하단 메시지

	TEdit*			m_pItemMessage;		// 아이템 정보 출력

	INT				m_nSlotNum;
	INT				m_nMode;
	CTCtrlList*		m_pList;


	std::vector<CTClientItem*>	m_vItems;
	std::vector<CString>		m_vExplanation;
	
	

public:

	void	AddItem( LPTITEM pItem, CString strString );
	
	void	SetMode( INT nMode );
	INT		GetMode();

	void	Clear();



public:
	CTItemUpCashDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTItemUpCashDlg(void);

	void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual void ShowComponent( BOOL bVisible = TRUE);

};
