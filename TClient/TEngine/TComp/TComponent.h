// TComponent.h: interface for the TComponent class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCOMPONENT_H__CB3C944E_C828_4522_A574_9628CC8D9243__INCLUDED_)
#define AFX_TCOMPONENT_H__CB3C944E_C828_4522_A574_9628CC8D9243__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define SAFE_NOTIFY(from, msg, param)		if(m_pParent) m_pParent->OnNotify(from, msg, param)

class TComponent;
class CD3DFont;

typedef vector< TComponent* >	TCOMP_ARRAY;

class TComponent
{
public:
	struct TextLine
	{
		LPDIRECT3DVERTEXBUFFER9 m_pVB;
		INT m_nVertexCount;
		INT m_nStartX, m_nStartY;
		INT m_nEndX, m_nEndY;
		DWORD m_dwColor;
		INT m_nDotLength, m_nSpaceLength;

		TextLine();
		~TextLine();
		void MakeLine( LPDIRECT3DDEVICE9, INT StartX, INT StartY, INT EndX, INT EndY, DWORD dwColor );
		void MakeDotLine( LPDIRECT3DDEVICE9, INT StartX, INT StartY, INT EndX, INT EndY, DWORD dwColor, INT DotLength, INT SpaceLength);
		void Draw( LPDIRECT3DDEVICE9 );

	private:
		void _CreateVB( LPDIRECT3DDEVICE9 pDevice, INT nCount ); 
	};

	struct TextSetting
	{
		INT 	iStart;
		INT 	iEnd;
		DWORD	dwColor;
		DWORD	dwUser;
		INT	iType;

		TextSetting()
			: iStart(0), iEnd(0), dwColor(0), dwUser(0), iType(0) {}

		bool operator < (const TextSetting& l) const
		{
			if( iStart == l.iStart )
			{
				if( iEnd == l.iEnd )
					return dwColor < l.dwColor;
				else
					return iEnd < l.iEnd;
			}
			else
				return iStart < l.iStart;
		}
	};
	typedef ::std::set<TextSetting> TextSettingSet;

	struct TextOutputData
	{
		INT			iDepth;
		CD3DFont*	pFont;
		DWORD		dwUser;
		INT			iLine;
		CPoint		ptPoint;
		CSize		szSize;
		CString		strText;
		TextLine*		pTextLine;
        		
		TextOutputData()
			: iDepth(0), pFont(NULL), dwUser(0), iLine(0), 
			  ptPoint(0,0), szSize(0,0), strText(""), pTextLine(NULL)
		{}

		bool operator < (const TextOutputData& l) const
		{
			if( iDepth == l.iDepth )
				return pFont < l.pFont;
			else 
				return iDepth < l.iDepth;
		}
	};
	typedef ::std::multiset<TextOutputData> TextOutputDataSet;

public:
	static BOOL m_bUseWordBreak;

public:
	CString m_strText;
	CString m_strTooltip;

	DWORD m_id;
	TCOMMAND m_menu[TCML_MENU_COUNT];
	DWORD m_dwMediaID;
	DWORD m_dwColor;
	int m_index;

	LP_FRAMEDESC m_pDESC;
	BOOL m_blIsCaret;

	BYTE m_bUseImageClrForText;
	BYTE m_bWordBreak;
	BYTE m_bVCenter;
	BYTE m_bNoHIT;
	BYTE m_bShrink;
	BYTE m_bDidShrink;
	TComponent* m_pNextTab;

public:
// Text Output & Setting
	TextSettingSet m_TextSettingSet;
	TextOutputDataSet m_TextOutputDataSet;
	CString m_strPrvText;
	VECTORSTRING m_vTextLine;
	BOOL m_bNeedUpdateTextSetting;
	BYTE m_PrvFlagAlign;
	CRect m_rcPrv;
	BYTE m_bHitTestType;

public:
//Templates
	BYTE m_bType;
	CRect m_rcSrc;
	CRect m_rc;

	BYTE m_flagBlt;
	BYTE m_flagAlign;

	float m_fScaleX;
	float m_fScaleY;

//Default images
	LPIMAGESET m_pEnable;
	LPIMAGESET m_pDisable;

//Kid components
	TCOMP_LIST m_kids;
	
	TComponent* m_pParent;

	DWORD m_style;
	BOOL m_bFocus;
	BOOL m_bEnable;
	BOOL m_bVisible;	

//Font
	CD3DFont* m_pTipFont;
	CD3DFont* m_pFont;

	CGDIFont* m_pTipGDIFont;
	CGDIFont* m_pGDIFont;

	COLORREF m_clrFg;
	COLORREF m_clrBg;

	int m_nTextExtent;
	int m_nVMargine;
	int m_nHMargine;

	DWORD m_dwTotalTick;
	BOOL m_blDoSprite;
	int m_nSpriteIndex;

	RECT m_rcWithKid;
	BOOL m_bRecalcKidRect;

protected:
	void Init();
	void Clear();
	void ClearKids();
	void ShrinkText( CString& text );	
	
	TComponent* ComponentNew( LP_FRAMEDESC pDesc );
	LP_FRAMEDESC RetrieveKidDesc( DWORD id, LP_FRAMEDESC pParent );

	virtual void CreateFrom( LP_FRAMEDESC pDesc );
	virtual void BuildMe( LP_FRAMEDESC pDesc );
	virtual void BuildFont( LP_FRAMEDESC pDesc );
	virtual void BuildKid( LP_FRAMEDESC pDesc );

public:	
	TComponent( TComponent* pParent, LP_FRAMEDESC pDesc );
	virtual ~TComponent();

	virtual DWORD SetFocus( DWORD dwID);
	virtual BOOL HitRect( CPoint pt);
	virtual BOOL HitTest( CPoint pt);

	virtual void EnableComponent( BOOL bEnable = TRUE);
	virtual void ShowComponent( BOOL bVisible = TRUE);
	virtual void GetComponentRectWithKid( LPRECT lpRect);
	virtual void GetComponentRect( LPRECT lpRect);
	virtual void SetTextClr( DWORD clrText);
	virtual void ResetFocus();
	virtual BOOL IsVisible();
	virtual BOOL IsEnable();
	virtual void NotifyComponentUpdate();

	virtual void SetTextClr(
		DWORD dwColor,
		DWORD dwLineColor,
		BYTE bOutLine);

	DWORD GetID();
	BOOL CanFocus();
	BOOL GetFocus();
	DWORD GetStyle();
	int GetKidsCount();
	BOOL AreYou( DWORD id );
	BOOL IsTypeOf( BYTE bType );
	void SetSrcRect( CRect rc );
	void SetStyle( DWORD style );
	CString GetComponentText();
	LPIMAGESET GetDefaultImage();

	float GetScaleX() { return m_fScaleX; }
	float GetScaleY() { return m_fScaleY; }
	
	void SetScale(float fScaleX, float fScaleY, BOOL bApplyChildren);

	virtual void MoveComponent( CPoint pt );	
	void SetComponentRect( CRect rc );
	void SetFocus( BOOL bFocus= TRUE );	
	void SetComponentSize( CSize size );
	void GetComponentPos( LPPOINT lpPt );	
	void GetComponentSize( LPSIZE lpSize );
	void SetComponentText( LPCTSTR strText );
	void SetComponentINT( int n );
	void ResetParent( TComponent *pParent);

	void GetComponentRealRect( LPRECT lpRect);
	void GetComponentRealPos( LPPOINT lpPt );	
	void GetComponentRealSize( LPSIZE lpSize );

	TComponent* FindKid( DWORD id );
	TComponent* GetParent();

	BOOL EndOfKids( TCOMP_LIST::iterator it );
	TCOMP_LIST::iterator GetFirstKidsFinder();

	TComponent* GetNextKid( TCOMP_LIST::iterator &it );
	TComponent* AddKid( LP_FRAMEDESC pDesc);
	TComponent* AddKid( TComponent *pKid );
	void RemoveKid( TComponent *pKid);
	void RemoveKidWithoutRearrage( TComponent* pKid );

	virtual int GetTextExtent();
	virtual void ClearText();

	void SetTextAlign( BYTE bAlign);
	void SetTextExtent( int nExtent );
	void InComponentScreenPt( CPoint* ppt );
	void ScreenToComponent( CRect* prc);
	void ScreenToComponent( CPoint* ppt);
	void ComponentToScreen( CRect* prc);
	void ComponentToScreen( CPoint* ppt);
	void GetTextExtentPoint( LPCTSTR strText, CSize& size );
	void TextToPoint( LPCTSTR strText, int nPos, CPoint& pt, int nLine = 0xFF );	

	void SetSprite( BOOL blDoSprite, int nSpriteIndex = 0 );

	DWORD GetRepeatCount();
	void ResetAnimation();
	void ResetTotalTick( DWORD);

public:
	// message handlers
	virtual void OnChar( UINT nChar, int nRepCnt, UINT nFlags );
	virtual void OnKeyUp( UINT nChar, int nRepCnt, UINT nFlags );
	virtual void OnKeyDown( UINT nChar, int nRepCnt, UINT nFlags );
	virtual void OnSetFocus( TComponent* pLoseFocus );
	virtual void OnLoseFocus( TComponent* pSetFocus );
	virtual void OnMouseMove( UINT nFlags, CPoint pt );
	virtual void OnLButtonUp( UINT nFlags, CPoint pt );
	virtual void OnMButtonUp( UINT nFlags, CPoint pt );
	virtual void OnRButtonUp( UINT nFlags, CPoint pt );
	virtual void OnLButtonDown( UINT nFlags, CPoint pt );	
	virtual void OnMButtonDown( UINT nFlags, CPoint pt );	
	virtual void OnRButtonDown( UINT nFlags, CPoint pt );
	virtual void OnLButtonDblClk( UINT nFlags, CPoint pt );	
	virtual void OnMButtonDblClk( UINT nFlags, CPoint pt );	
	virtual void OnRButtonDblClk( UINT nFlags, CPoint pt );
	virtual void OnImeComposition( HWND hWnd, WPARAM wParam, LPARAM lParam );
	virtual void OnImeNotify( HWND hWnd, WPARAM wParam, LPARAM lParam );
	virtual BOOL OnInputLangChangeRequest( HWND hWnd, WPARAM wParam, LPARAM lParam );
	virtual void OnLostDevice();
	virtual void OnNotify( DWORD from, WORD msg, LPVOID param );
	virtual BOOL DoMouseWheel( UINT nFlags, short zDelta, CPoint pt);
	virtual BOOL OnSetCursor( CWnd* pWnd, UINT nHitTest, UINT message, CPoint pt);

public:
	//Render
	BOOL CanProcess();

	CD3DFont* Get3DTipFont();
	CGDIFont* GetTipFont();
	CD3DFont* Get3DFont();
	CGDIFont* GetFont();

	void SetFont( CGDIFont* pGDIFont, CGDIFont* pTipGDIFont);
	void Create3DFont( CGDIFont* pGDIFont, CGDIFont* pTipGDIFont);
	virtual HRESULT Render( DWORD dwTickCount );
	virtual HRESULT DrawKids( DWORD dwTickCount );
	virtual HRESULT DrawText();
	virtual HRESULT DrawDefault( DWORD dwTickCount, LPRECT lpRect = NULL);
	
	virtual HRESULT TextOut( CString& strText, int x, int y );
	virtual HRESULT TextOut( const TextOutputData& ptd );
	
	virtual VOID SeparateTextFitSize( CString& strTEXT, CSize szCTRL, VECTORSTRING& outResult, BOOL bAcc=FALSE );
	virtual VOID UpdateTextOutputData( VECTORSTRING& vTextLine, const CSize szCTRL, TextOutputDataSet& outResult );
	virtual VOID DoTextOut( TextOutputDataSet& );

public:
	// Text Setting
    void AddTextSetting(INT iStart, INT iEnd, DWORD dwColor, DWORD dwUser=NULL, INT iType = 0);
	
	void RemoveTextSetting(INT iStart, INT iEnd);
	void RemoveTextSetting(const TextSetting& data);
	
	void ResetTextSetting();
	
	BOOL IsTextSettingEmpty() const	{ return m_TextSettingSet.empty(); }
	INT GetTextSettingSize() const	{ return (INT)m_TextSettingSet.size(); }

	BOOL FindTextSetting(INT iIndex, TextSetting& outSetting);
	
	TextSettingSet::iterator GetTextSettingItr(INT iIndex);
	TextSettingSet::iterator GetTextSettingItrBegin();
	TextSettingSet::iterator GetTextSettingItrEnd();

	BOOL HitTestTextSetting(const CPoint& pt, DWORD* outUser=NULL);

	TComponent* FindShrinkInChilds( POINT pt );

#ifdef DEBUG
	void _LOG_TEXT_SETTING( LPCSTR func, LPCSTR file, LONG line );
#else 
	void _LOG_TEXT_SETTING( LPCSTR func, LPCSTR file, LONG line ) {}
#endif

protected:
	void ClearTextOutputData();
};

#endif // !defined(AFX_TCOMPONENT_H__CB3C944E_C828_4522_A574_9628CC8D9243__INCLUDED_)
