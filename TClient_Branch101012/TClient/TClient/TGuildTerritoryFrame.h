#pragma once

class CTGuildTerritoryFrame : public ITInnerFrame
{
public:
	enum 
	{
		MODE_CASTLE=0,
		MODE_LOCAL,
		MODE_MISSION,
		MODE_COUNT
	};

	typedef CTGuildCommander::GuildDetInfo			GuildDetInfo;
	typedef CTGuildCommander::Territory				Territory;
	typedef CTGuildCommander::LocalTerritory		LocalTerritory;

protected:
	TList*		m_pList;
	INT			m_nCurMode;

	TButton*	m_vTabBtn[MODE_COUNT];
	TComponent* m_vCol[6];
	TComponent* m_vColBack[2];

public:
	void SetCurMode(INT nMode);
	INT GetCurMode() const { return m_nCurMode; }

public:
	virtual void RequestInfo();
	virtual void ResetInfo();

public:
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );
	virtual void SwitchFocus(TComponent *pCandidate);
	virtual ITDetailInfoPtr GetTInfoKey( const CPoint& point );
	virtual void ShowComponent(BOOL bVisible = 1);

public:
	CTGuildTerritoryFrame(TComponent *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTGuildTerritoryFrame();
};