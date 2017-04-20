#pragma once

class CTCommunityDlg : public CTFrameGroupBase
{
public:
	virtual void OnLButtonUp(UINT nFlags, CPoint pt);
	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual void ShowComponent(BOOL bVisible = TRUE);
	virtual void ResetPosition();

public:
	CTCommunityDlg(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTCommunityDlg();
};
