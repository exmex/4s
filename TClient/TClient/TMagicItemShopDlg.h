#pragma once

#ifdef MAGICITEMSHOP

class CTMagicItemShopDlg : public CTShopBaseDlg
{
public:
	virtual ITDetailInfoPtr GetTInfoKey(const CPoint& point );
	virtual BOOL GetTChatInfo(const CPoint& point, TCHATINFO& outInfo );

	virtual TDROPINFO OnDrop(CPoint point);
	virtual BYTE OnBeginDrag(LPTDRAG pDRAG, CPoint point);

public:
	CTMagicItemShopDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTMagicItemShopDlg();
};

#endif