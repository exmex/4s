#pragma once

class CTGuildPointLogFrame : public CTClientUIBase
{
public:
	CTGuildPointLogFrame( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildPointLogFrame();

public:
	TList* m_pList;
};