#pragma once


class CFrameGroup : public TFrame
{
public:
	CFrameGroup();
	virtual ~CFrameGroup();

public:
	TCMLParser *m_pTParser;

	MAPFRAME m_mapFrame;
	DWORD m_dwLevel;

public:
	virtual void ResetFocus();

	TFrame *FindFrame( DWORD dwLevel);
	TFrame *GetCurrentFrame();
	DWORD GetLevel();

	void SetParent( TComponent *pParent);
	void SetLevel( DWORD dwLevel);
	void DelFrame( DWORD dwLevel);
	void ClearFrame();

	void AddFrame(
		DWORD dwFrameID,
		DWORD dwLevel);
};
