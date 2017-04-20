#pragma once

class CTDurationFrame : public CTClientUIBase
{
public:
	enum DAMAGE_LEVEL
	{
		DAMAGE_NORMAL,
		DAMAGE_YELLOW,
		DAMAGE_RED,
		DAMAGE_BREAK,
		DAMAGE_COUNT,
	};

protected:
	TImageList* m_vEquip[ES_COUNT];
	TComponent*	m_pInfo;

public:
	void Init();
	void Update();

protected:
	BOOL GetDurationPercent(BYTE bSlot, DWORD& outPrecent);

public:
	virtual BOOL HitTest( CPoint pt);
	virtual HRESULT Render( DWORD dwTickCount );

public:
	CTDurationFrame( TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTDurationFrame();
};
