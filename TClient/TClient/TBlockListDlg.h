#pragma once

struct BLOCKCHAR
{
	DWORD m_dwCharID;
	CString m_strCharName;
	BYTE m_bBlockOption;

	BLOCKCHAR()
		: m_dwCharID(0), m_bBlockOption(0)
	{};
};

struct BLOCKOPBTN
{
	INT m_nIndex;
	TButton* m_pOpButton[4];
	BOOL m_bOpShow[4];

	BLOCKOPBTN()
	{
		m_nIndex = 0;
		for( INT i=0 ; i < 4 ; ++i )
		{
			m_pOpButton[ i ] = NULL;
			m_bOpShow[ i ] = FALSE;
		}
	}
};

class CTBlockListDlg;

class CTBlockList : public TList
{
public:
	CTBlockList(CTBlockListDlg *pParent, LP_FRAMEDESC pDesc);
	virtual ~CTBlockList();

	virtual void OnNotify( DWORD from, WORD msg, LPVOID param );
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);

protected:
	CTBlockListDlg* m_pParent;
};

class CTBlockListDlg : public ITInnerFrame
{
protected:
	CTBlockList* m_pList;
	TComponent* m_pAddBtn;
	TComponent* m_pDelBtn;
	TComponent* m_pOpButton[4];
	CPoint m_vOpBtnOffset[4];
	std::vector<BLOCKOPBTN> m_vBlockOpBtns;
	std::vector<BLOCKCHAR> m_vBlockChars;

public:
	void Add(
		DWORD dwCharID,
		const CString& strName,
		BYTE bOption );

	void Remvoe(INT nIndex);
	void Clear();
	
	BOOL IsEmpty() const;
	UINT GetCount() const;
	
	CString GetName(INT nIndex);
	BYTE GetBlockOption(INT nIndex);
	INT Find(const CString& strName);

	void _UpdateOpButton();
   
public:
	virtual void RequestInfo();
	virtual void ResetInfo();

	virtual void OnLButtonDown(UINT nFlags, CPoint pt);
	virtual void ShowComponent( BOOL bVisible = 1 );

public:
	CTBlockListDlg(TComponent* pParent, LP_FRAMEDESC pDesc);
	virtual ~CTBlockListDlg();
};